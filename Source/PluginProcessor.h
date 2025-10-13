#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class VstTestPlaygroundAudioProcessor : public juce::AudioProcessor
{
public:
    VstTestPlaygroundAudioProcessor();
    ~VstTestPlaygroundAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Our parameter manager, as recommended by the guide
    juce::AudioProcessorValueTreeState apvts;

private:
    // Function to create the parameter layout
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // A simple sine wave oscillator
    juce::dsp::Oscillator<float> oscillator;
    // A gain processor to control the volume
    juce::dsp::Gain<float> gain;

    // Atomic pointers to our parameters for real-time safe access
    std::atomic<float>* gainParam = nullptr;
    std::atomic<float>* onOffParam = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VstTestPlaygroundAudioProcessor)
};