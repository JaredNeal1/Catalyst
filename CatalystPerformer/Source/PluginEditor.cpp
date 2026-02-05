#include "PluginEditor.h"

CatalystPerformerAudioProcessorEditor::CatalystPerformerAudioProcessorEditor(CatalystPerformerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 200);
}

CatalystPerformerAudioProcessorEditor::~CatalystPerformerAudioProcessorEditor() = default;

void CatalystPerformerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkslategrey);

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawFittedText("Catalyst Performer", getLocalBounds(), juce::Justification::centred, 1);
}

void CatalystPerformerAudioProcessorEditor::resized()
{
}
