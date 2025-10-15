#pragma once

#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "WebView.h"

/**
    The editor for the VST plugin.
    This class creates and manages the plugin's user interface.
*/
class VstTestPlaygroundAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                         public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    explicit VstTestPlaygroundAudioProcessorEditor (VstTestPlaygroundAudioProcessor&);
    ~VstTestPlaygroundAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    /**
        Called when a parameter is changed in the AudioProcessorValueTreeState.
    */
    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    //==============================================================================
    VstTestPlaygroundAudioProcessor& processorRef; /**< A reference to the audio processor. */

    CustomLookAndFeel customLookAndFeel; /**< The custom look and feel for the UI. */

    std::unique_ptr<WebView> webView; /**< The web view that displays the UI. */
    std::unique_ptr<juce::WebSliderRelay> gainRelay; /**< Relays parameter changes to the web view. */

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VstTestPlaygroundAudioProcessorEditor)
};