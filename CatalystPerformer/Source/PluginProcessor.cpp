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

void CatalystPerformerAudioProcessor::prepareToPlay(double, int)
{
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
    juce::ignoreUnused(midiMessages);
}

void CatalystPerformerAudioProcessor::processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    buffer.clear();
    juce::ignoreUnused(midiMessages);
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
