#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CatalystPerformerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit CatalystPerformerAudioProcessorEditor(CatalystPerformerAudioProcessor&);
    ~CatalystPerformerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    CatalystPerformerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CatalystPerformerAudioProcessorEditor)
};
