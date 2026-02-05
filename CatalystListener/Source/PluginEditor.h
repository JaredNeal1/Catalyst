#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class StatusIndicator : public juce::Component
{
public:
    void setActive(bool isActive) { active = isActive; repaint(); }
    bool isActive() const { return active; }

    void paint(juce::Graphics& g) override
    {
        const auto bounds = getLocalBounds().toFloat();
        g.setColour(active ? juce::Colours::limegreen : juce::Colours::darkred);
        g.fillEllipse(bounds.reduced(1.0f));
        g.setColour(juce::Colours::black);
        g.drawEllipse(bounds.reduced(1.0f), 1.0f);
    }

private:
    bool active = false;
};

class RoundLearnButton : public juce::Button
{
public:
    RoundLearnButton() : juce::Button("Learn") {}

    void paintButton(juce::Graphics& g, bool, bool) override
    {
        const auto bounds = getLocalBounds().toFloat();
        const auto diameter = juce::jmin(bounds.getWidth(), bounds.getHeight());
        const auto circle = juce::Rectangle<float>(0, 0, diameter, diameter)
                                .withCentre(bounds.getCentre());

        const bool active = getToggleState();
        g.setColour(active ? juce::Colours::orange : juce::Colours::darkgrey);
        g.fillEllipse(circle);

        g.setColour(juce::Colours::black);
        g.drawEllipse(circle, 2.0f);

        g.setColour(juce::Colours::white);
        g.setFont(14.0f);
        g.drawFittedText(active ? "Learning" : "Learn",
                         circle.toNearestInt(),
                         juce::Justification::centred,
                         1);
    }
};

class CatalystListenerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                             private juce::Timer
{
public:
    explicit CatalystListenerAudioProcessorEditor(CatalystListenerAudioProcessor&);
    ~CatalystListenerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    CatalystListenerAudioProcessor& audioProcessor;
    RoundLearnButton learnButton;
    StatusIndicator statusIndicator;
    float smoothedDb = -60.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CatalystListenerAudioProcessorEditor)
};
