#include "PluginProcessor.h"
#include "PluginEditor.h"

CatalystListenerAudioProcessor::CatalystListenerAudioProcessor()
    : juce::AudioProcessor(BusesProperties()
                              .withInput("Input", juce::AudioChannelSet::stereo(), true)
                              .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    outputDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                    .getChildFile("Catalyst")
                    .getChildFile("Listener");
    outputDir.createDirectory();

    startTimerHz(2);
}

CatalystListenerAudioProcessor::~CatalystListenerAudioProcessor() = default;

const juce::String CatalystListenerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CatalystListenerAudioProcessor::acceptsMidi() const
{
    return false;
}

bool CatalystListenerAudioProcessor::producesMidi() const
{
    return false;
}

bool CatalystListenerAudioProcessor::isMidiEffect() const
{
    return false;
}

double CatalystListenerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CatalystListenerAudioProcessor::getNumPrograms()
{
    return 1;
}

int CatalystListenerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CatalystListenerAudioProcessor::setCurrentProgram(int)
{
}

const juce::String CatalystListenerAudioProcessor::getProgramName(int)
{
    return {};
}

void CatalystListenerAudioProcessor::changeProgramName(int, const juce::String&)
{
}

void CatalystListenerAudioProcessor::prepareToPlay(double sampleRate, int)
{
    sampleRateHz = sampleRate;
    chunkSamples = static_cast<int>(sampleRateHz * 4.0); // 4 seconds
    ensureCaptureBuffers(getTotalNumInputChannels());
    captureWritePos = 0;
    captureSamplesFilled = 0;
}

void CatalystListenerAudioProcessor::releaseResources()
{
}

#if !JucePlugin_PreferredChannelConfigurations
bool CatalystListenerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}
#endif

void CatalystListenerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    float rms = 0.0f;
    for (int ch = 0; ch < totalNumInputChannels; ++ch)
        rms = juce::jmax(rms, buffer.getRMSLevel(ch, 0, buffer.getNumSamples()));
    inputRms.store(rms, std::memory_order_relaxed);

    if (learnEnabled.load(std::memory_order_relaxed))
    {
        if (captureLock.tryEnter())
        {
            ensureCaptureBuffers(totalNumInputChannels);
            const int numSamples = buffer.getNumSamples();
            const int ringSize = captureBuffer.getNumSamples();

            int remaining = numSamples;
            int sourcePos = 0;
            while (remaining > 0)
            {
                const int block = juce::jmin(remaining, ringSize - captureWritePos);
                for (int ch = 0; ch < captureBuffer.getNumChannels(); ++ch)
                    captureBuffer.copyFrom(ch, captureWritePos, buffer, ch, sourcePos, block);

                captureWritePos = (captureWritePos + block) % ringSize;
                sourcePos += block;
                remaining -= block;
            }

            captureSamplesFilled = juce::jmin(captureSamplesFilled + numSamples, ringSize);
            captureLock.exit();
        }
    }
}

void CatalystListenerAudioProcessor::processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    double rms = 0.0;
    for (int ch = 0; ch < totalNumInputChannels; ++ch)
        rms = juce::jmax(rms, buffer.getRMSLevel(ch, 0, buffer.getNumSamples()));
    inputRms.store(static_cast<float>(rms), std::memory_order_relaxed);

    if (learnEnabled.load(std::memory_order_relaxed))
    {
        if (captureLock.tryEnter())
        {
            ensureCaptureBuffers(totalNumInputChannels);
            const int numSamples = buffer.getNumSamples();
            const int ringSize = captureBuffer.getNumSamples();

            int remaining = numSamples;
            int sourcePos = 0;
            while (remaining > 0)
            {
                const int block = juce::jmin(remaining, ringSize - captureWritePos);
                for (int ch = 0; ch < captureBuffer.getNumChannels(); ++ch)
                {
                    for (int i = 0; i < block; ++i)
                        captureBuffer.setSample(ch, captureWritePos + i,
                                                static_cast<float>(buffer.getSample(ch, sourcePos + i)));
                }

                captureWritePos = (captureWritePos + block) % ringSize;
                sourcePos += block;
                remaining -= block;
            }

            captureSamplesFilled = juce::jmin(captureSamplesFilled + numSamples, ringSize);
            captureLock.exit();
        }
    }
}

bool CatalystListenerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* CatalystListenerAudioProcessor::createEditor()
{
    return new CatalystListenerAudioProcessorEditor(*this);
}

void CatalystListenerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ignoreUnused(destData);
}

void CatalystListenerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
}

float CatalystListenerAudioProcessor::getInputLevelDb() const noexcept
{
    const float rms = inputRms.load(std::memory_order_relaxed);
    if (rms <= 0.000001f)
        return -60.0f;
    return juce::Decibels::gainToDecibels(rms, -60.0f);
}

void CatalystListenerAudioProcessor::setLearnEnabled(bool enabled)
{
    learnEnabled.store(enabled, std::memory_order_relaxed);
}

void CatalystListenerAudioProcessor::ensureCaptureBuffers(int numChannels)
{
    if (chunkSamples <= 0)
        return;

    const int desiredChannels = juce::jmax(1, numChannels);
    const int desiredSamples = chunkSamples * 2;

    if (captureBuffer.getNumChannels() != desiredChannels
        || captureBuffer.getNumSamples() != desiredSamples)
    {
        captureBuffer.setSize(desiredChannels, desiredSamples, false, false, true);
        captureBuffer.clear();
        writeBuffer.setSize(desiredChannels, chunkSamples, false, false, true);
        writeBuffer.clear();
        captureWritePos = 0;
        captureSamplesFilled = 0;
    }
}

void CatalystListenerAudioProcessor::timerCallback()
{
    if (!learnEnabled.load(std::memory_order_relaxed))
        return;

    if (chunkSamples <= 0 || captureSamplesFilled < chunkSamples)
        return;

    writeSnapshotToDisk();
}

void CatalystListenerAudioProcessor::writeSnapshotToDisk()
{
    if (!captureLock.tryEnter())
        return;

    const int ringSize = captureBuffer.getNumSamples();
    const int start = (captureWritePos - chunkSamples + ringSize) % ringSize;
    const int firstBlock = juce::jmin(chunkSamples, ringSize - start);
    const int secondBlock = chunkSamples - firstBlock;

    for (int ch = 0; ch < captureBuffer.getNumChannels(); ++ch)
    {
        writeBuffer.copyFrom(ch, 0, captureBuffer, ch, start, firstBlock);
        if (secondBlock > 0)
            writeBuffer.copyFrom(ch, firstBlock, captureBuffer, ch, 0, secondBlock);
    }

    captureLock.exit();

    outputDir.createDirectory();
    const auto stamp = juce::Time::getCurrentTime();
    const auto tag = stamp.toString(true, true).replaceCharacter(':', '-').replaceCharacter(' ', '_');

    const auto wavTmp = outputDir.getChildFile("session.wav.tmp");
    const auto wavFinal = outputDir.getChildFile("session.wav");

    {
        wavTmp.deleteFile();
        std::unique_ptr<juce::FileOutputStream> outStream(wavTmp.createOutputStream());
        if (outStream != nullptr)
        {
            juce::WavAudioFormat format;
            std::unique_ptr<juce::AudioFormatWriter> writer(
                format.createWriterFor(outStream.get(), sampleRateHz,
                                       static_cast<unsigned int>(writeBuffer.getNumChannels()),
                                       16, {}, 0));
            if (writer != nullptr)
            {
                outStream.release();
                writer->writeFromAudioSampleBuffer(writeBuffer, 0, writeBuffer.getNumSamples());
            }
        }
    }

    wavFinal.deleteFile();
    wavTmp.moveFileTo(wavFinal);

    juce::DynamicObject::Ptr meta = new juce::DynamicObject();
    meta->setProperty("version", ++writeCounter);
    meta->setProperty("timestamp", tag);
    meta->setProperty("sample_rate", sampleRateHz);
    meta->setProperty("num_samples", chunkSamples);
    meta->setProperty("channels", writeBuffer.getNumChannels());

    const auto jsonTmp = outputDir.getChildFile("session.json.tmp");
    const auto jsonFinal = outputDir.getChildFile("session.json");
    jsonTmp.deleteFile();
    jsonTmp.replaceWithText(juce::JSON::toString(meta.get()));
    jsonFinal.deleteFile();
    jsonTmp.moveFileTo(jsonFinal);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CatalystListenerAudioProcessor();
}
