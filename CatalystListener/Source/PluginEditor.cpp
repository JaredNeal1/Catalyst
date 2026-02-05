#include "PluginEditor.h"

CatalystListenerAudioProcessorEditor::CatalystListenerAudioProcessorEditor(CatalystListenerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(220, 220);

    learnButton.setClickingTogglesState(true);
    learnButton.onClick = [this]
    {
        const bool active = learnButton.getToggleState();
        statusIndicator.setActive(active);
        audioProcessor.setLearnEnabled(active);
    };
    addAndMakeVisible(learnButton);

    statusIndicator.setActive(false);
    addAndMakeVisible(statusIndicator);

    startTimerHz(30);
}

CatalystListenerAudioProcessorEditor::~CatalystListenerAudioProcessorEditor() = default;

void CatalystListenerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    auto bounds = getLocalBounds().reduced(24);
    const int meterHeight = 10;
    const int meterWidth = 140;
    auto meterBounds = juce::Rectangle<int>(meterWidth, meterHeight)
                           .withCentre({ bounds.getCentreX(), bounds.getBottom() - 18 });

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(meterBounds);

    const float normalized = juce::jlimit(0.0f, 1.0f, (smoothedDb + 60.0f) / 60.0f);
    auto fillBounds = meterBounds.withWidth(static_cast<int>(meterBounds.getWidth() * normalized));
    g.setColour(juce::Colours::limegreen);
    g.fillRect(fillBounds);
    g.setColour(juce::Colours::black);
    g.drawRect(meterBounds);
}

void CatalystListenerAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(24);

    const int buttonSize = 120;
    auto buttonBounds = juce::Rectangle<int>(buttonSize, buttonSize)
                            .withCentre(bounds.getCentre());
    learnButton.setBounds(buttonBounds);

    const int indicatorSize = 10;
    auto indicatorBounds = juce::Rectangle<int>(indicatorSize, indicatorSize)
                               .withCentre({ buttonBounds.getCentreX(),
                                             buttonBounds.getBottom() + 14 });
    statusIndicator.setBounds(indicatorBounds);
}

void CatalystListenerAudioProcessorEditor::timerCallback()
{
    const float targetDb = audioProcessor.getInputLevelDb();
    smoothedDb = 0.85f * smoothedDb + 0.15f * targetDb;
    repaint();
}
