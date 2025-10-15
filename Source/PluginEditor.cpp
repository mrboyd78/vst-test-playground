#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Params.h"

//==============================================================================
VstTestPlaygroundAudioProcessorEditor::VstTestPlaygroundAudioProcessorEditor (VstTestPlaygroundAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    setLookAndFeel(&customLookAndFeel);

    // Configure WebView2 with proper backend and user data folder
    auto options = juce::WebBrowserComponent::Options()
        .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
        .withWinWebView2Options(
            juce::WebBrowserComponent::Options::WinWebView2{}
                .withUserDataFolder(
                    juce::File::getSpecialLocation(juce::File::tempDirectory)
                        .getChildFile("VstTestPlayground")
                )
        )
        .withNativeIntegrationEnabled()
        .withKeepPageLoadedWhenBrowserIsHidden();

    webView = std::make_unique<WebView>(options);
    addAndMakeVisible(webView.get());

    // Set up WebSliderRelay for proper DAW automation
    gainRelay = std::make_unique<juce::WebSliderRelay>(Params::gain.id);

    processorRef.apvts.addParameterListener(Params::gain.id, this);

    setSize (400, 300);
}

VstTestPlaygroundAudioProcessorEditor::~VstTestPlaygroundAudioProcessorEditor()
{
    processorRef.apvts.removeParameterListener(Params::gain.id, this);
    gainRelay.reset();
    webView.reset();
    setLookAndFeel(nullptr);
}

void VstTestPlaygroundAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void VstTestPlaygroundAudioProcessorEditor::resized()
{
    if (webView)
        webView->setBounds(getLocalBounds());
}

void VstTestPlaygroundAudioProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
    // Parameter changes are automatically handled by WebSliderRelay
    // This method can be used for additional UI updates if needed
    juce::ignoreUnused(parameterID, newValue);
}