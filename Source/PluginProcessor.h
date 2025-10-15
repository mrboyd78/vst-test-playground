#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

/**
    The main audio processor for the VST plugin.
    This class handles all audio processing, parameter management, and editor creation.
*/
class VstTestPlaygroundAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    VstTestPlaygroundAudioProcessor();
    ~VstTestPlaygroundAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    //==============================================================================
    juce::AudioProcessorValueTreeState apvts; /**< Manages the plugin's parameters. */

private:
    //==============================================================================
    /**
        Creates the parameter layout for the AudioProcessorValueTreeState.
    */
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::UndoManager undoManager; /**< Manages undo/redo operations. */
    juce::dsp::Gain<float> gain; /**< The gain processor. */
    std::atomic<float>* gainParameter = nullptr; /**< A pointer to the gain parameter. */
    float previousGainDB = 0.0f; /**< The previous gain value in dB. */

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VstTestPlaygroundAudioProcessor)
};