#include "PluginProcessor.h"
#include "PluginEditor.h"

CatalystListenerAudioProcessor::CatalystListenerAudioProcessor()
    : juce::AudioProcessor(BusesProperties()
                              .withInput("Input", juce::AudioChannelSet::stereo(), true)
                              .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
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

void CatalystListenerAudioProcessor::prepareToPlay(double, int)
{
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
}

void CatalystListenerAudioProcessor::processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
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

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CatalystListenerAudioProcessor();
}
