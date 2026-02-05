#include "PluginEditor.h"

CatalystListenerAudioProcessorEditor::CatalystListenerAudioProcessorEditor(CatalystListenerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 200);
}

CatalystListenerAudioProcessorEditor::~CatalystListenerAudioProcessorEditor() = default;

void CatalystListenerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawFittedText("Catalyst Listener", getLocalBounds(), juce::Justification::centred, 1);
}

void CatalystListenerAudioProcessorEditor::resized()
{
}
