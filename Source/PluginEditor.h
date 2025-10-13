#pragma once

#include "PluginProcessor.h"
#include <juce_gui_extra/juce_gui_extra.h>

class VstTestPlaygroundAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit VstTestPlaygroundAudioProcessorEditor(VstTestPlaygroundAudioProcessor&);
    ~VstTestPlaygroundAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    VstTestPlaygroundAudioProcessor& processorRef;
    
    // The WebView component that will host our UI
    std::unique_ptr<juce::WebBrowserComponent> webBrowserComponent;

    // Parameter relays and attachments, as recommended
    std::unique_ptr<juce::WebSliderRelay> gainRelay;
    std::unique_ptr<juce::WebSliderParameterAttachment> gainAttachment;

    std::unique_ptr<juce::WebSliderRelay> onOffRelay;
    std::unique_ptr<juce::WebSliderParameterAttachment> onOffAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VstTestPlaygroundAudioProcessorEditor)
};