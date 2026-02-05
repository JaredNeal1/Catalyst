#include "PluginProcessor.h"
#include "PluginEditor.h"

CatalystPerformerAudioProcessor::CatalystPerformerAudioProcessor()
    : juce::AudioProcessor(BusesProperties())
{
}

CatalystPerformerAudioProcessor::~CatalystPerformerAudioProcessor() = default;

const juce::String CatalystPerformerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CatalystPerformerAudioProcessor::acceptsMidi() const
{
    return true;
}

bool CatalystPerformerAudioProcessor::producesMidi() const
{
    return true;
}

bool CatalystPerformerAudioProcessor::isMidiEffect() const
{
    return true;
}

double CatalystPerformerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CatalystPerformerAudioProcessor::getNumPrograms()
{
    return 1;
}

int CatalystPerformerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CatalystPerformerAudioProcessor::setCurrentProgram(int)
{
}

const juce::String CatalystPerformerAudioProcessor::getProgramName(int)
{
    return {};
}

void CatalystPerformerAudioProcessor::changeProgramName(int, const juce::String&)
{
}

void CatalystPerformerAudioProcessor::prepareToPlay(double sampleRate, int)
{
    sampleRateHz = sampleRate;
    wasPlaying = false;
    noteOnSent = false;
    samplesUntilNoteOff = 0;
}

void CatalystPerformerAudioProcessor::releaseResources()
{
}

#if !JucePlugin_PreferredChannelConfigurations
bool CatalystPerformerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet().isDisabled();
}
#endif

void CatalystPerformerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // MIDI FX: no audio processing, just pass MIDI through for now.
    buffer.clear();

    juce::AudioPlayHead::CurrentPositionInfo posInfo;
    const auto hasPlayhead = getPlayHead() != nullptr && getPlayHead()->getCurrentPosition(posInfo);
    const bool isPlaying = hasPlayhead && posInfo.isPlaying;

    if (isPlaying && !wasPlaying)
    {
        // Transport just started: send a test C4 note on.
        const int midiChannel = 1;
        const int midiNote = 60; // C4
        midiMessages.addEvent(juce::MidiMessage::noteOn(midiChannel, midiNote, (juce::uint8) 100), 0);
        noteOnSent = true;
        samplesUntilNoteOff = static_cast<int>(sampleRateHz * 0.5); // 500ms
    }

    if (noteOnSent)
    {
        samplesUntilNoteOff -= buffer.getNumSamples();
        if (samplesUntilNoteOff <= 0)
        {
            const int midiChannel = 1;
            const int midiNote = 60; // C4
            midiMessages.addEvent(juce::MidiMessage::noteOff(midiChannel, midiNote), 0);
            noteOnSent = false;
            samplesUntilNoteOff = 0;
        }
    }

    if (!isPlaying && wasPlaying && noteOnSent)
    {
        const int midiChannel = 1;
        const int midiNote = 60; // C4
        midiMessages.addEvent(juce::MidiMessage::noteOff(midiChannel, midiNote), 0);
        noteOnSent = false;
        samplesUntilNoteOff = 0;
    }

    wasPlaying = isPlaying;
}

void CatalystPerformerAudioProcessor::processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    buffer.clear();

    juce::AudioPlayHead::CurrentPositionInfo posInfo;
    const auto hasPlayhead = getPlayHead() != nullptr && getPlayHead()->getCurrentPosition(posInfo);
    const bool isPlaying = hasPlayhead && posInfo.isPlaying;

    if (isPlaying && !wasPlaying)
    {
        const int midiChannel = 1;
        const int midiNote = 60; // C4
        midiMessages.addEvent(juce::MidiMessage::noteOn(midiChannel, midiNote, (juce::uint8) 100), 0);
        noteOnSent = true;
        samplesUntilNoteOff = static_cast<int>(sampleRateHz * 0.5);
    }

    if (noteOnSent)
    {
        samplesUntilNoteOff -= buffer.getNumSamples();
        if (samplesUntilNoteOff <= 0)
        {
            const int midiChannel = 1;
            const int midiNote = 60; // C4
            midiMessages.addEvent(juce::MidiMessage::noteOff(midiChannel, midiNote), 0);
            noteOnSent = false;
            samplesUntilNoteOff = 0;
        }
    }

    if (!isPlaying && wasPlaying && noteOnSent)
    {
        const int midiChannel = 1;
        const int midiNote = 60; // C4
        midiMessages.addEvent(juce::MidiMessage::noteOff(midiChannel, midiNote), 0);
        noteOnSent = false;
        samplesUntilNoteOff = 0;
    }

    wasPlaying = isPlaying;
}

bool CatalystPerformerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* CatalystPerformerAudioProcessor::createEditor()
{
    return new CatalystPerformerAudioProcessorEditor(*this);
}

void CatalystPerformerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ignoreUnused(destData);
}

void CatalystPerformerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CatalystPerformerAudioProcessor();
}
