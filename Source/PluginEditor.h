#pragma once

#include "PluginProcessor.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include "CustomLookAndFeel.h"

class VstTestPlaygroundAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit VstTestPlaygroundAudioProcessorEditor (VstTestPlaygroundAudioProcessor&);
    ~VstTestPlaygroundAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    VstTestPlaygroundAudioProcessor& processorRef;

    CustomLookAndFeel customLookAndFeel;

    juce::Slider gainSlider;
    juce::Label gainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VstTestPlaygroundAudioProcessorEditor)
};