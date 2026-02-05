#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CatalystListenerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit CatalystListenerAudioProcessorEditor(CatalystListenerAudioProcessor&);
    ~CatalystListenerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    CatalystListenerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CatalystListenerAudioProcessorEditor)
};
