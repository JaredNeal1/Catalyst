#pragma once

#include <JuceHeader.h>
#include <atomic>

class CatalystListenerAudioProcessor : public juce::AudioProcessor,
                                       private juce::Timer
{
public:
    CatalystListenerAudioProcessor();
    ~CatalystListenerAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #if !JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
   #endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void processBlock(juce::AudioBuffer<double>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    float getInputLevelDb() const noexcept;
    void setLearnEnabled(bool enabled);

private:
    void timerCallback() override;
    void ensureCaptureBuffers(int numChannels);
    void writeSnapshotToDisk();

    std::atomic<float> inputRms { 0.0f };
    std::atomic<bool> learnEnabled { false };
    juce::SpinLock captureLock;
    juce::AudioBuffer<float> captureBuffer;
    juce::AudioBuffer<float> writeBuffer;
    int captureWritePos = 0;
    int captureSamplesFilled = 0;
    int chunkSamples = 0;
    double sampleRateHz = 44100.0;
    juce::File outputDir;
    int writeCounter = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CatalystListenerAudioProcessor)
};
