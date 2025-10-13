#include "PluginEditor.h"

VstTestPlaygroundAudioProcessorEditor::VstTestPlaygroundAudioProcessorEditor(VstTestPlaygroundAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    // Instantiate the WebBrowserComponent
    webBrowserComponent = std::make_unique<juce::WebBrowserComponent>();

    // Configure the options as per the guide's recommendations
    auto options = juce::WebBrowserComponent::Options()
        .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
        .withWinWebView2Options(
            juce::WebBrowserComponent::Options::WinWebView2{}
                .withUserDataFolder(juce::File::getSpecialLocation(juce::File::tempDirectory)
                    .getChildFile("VstTestPlayground_" + juce::String(juce::Process::getCurrentProcessId())))
        )
        .withNativeIntegrationEnabled()
        .withKeepPageLoadedWhenBrowserIsHidden();

    webBrowserComponent->setOptions(options);

    // Create relays and attachments for parameter synchronization
    gainRelay = std::make_unique<juce::WebSliderRelay>(*webBrowserComponent, "GAIN");
    gainAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *processorRef.apvts.getParameter("GAIN"), *gainRelay, nullptr);

    onOffRelay = std::make_unique<juce::WebSliderRelay>(*webBrowserComponent, "ONOFF");
    onOffAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *processorRef.apvts.getParameter("ONOFF"), *onOffRelay, nullptr);

    addAndMakeVisible(*webBrowserComponent);

    // Set up hot-reloading for development
#if JUCE_DEBUG
    webBrowserComponent->goToURL("http://localhost:3000");
#else
    // In release, this would load from BinaryData
    // For now, we'll keep it simple. A resource provider will be added later.
    webBrowserComponent->goToURL("about:blank");
#endif

    setSize(400, 300);
}

VstTestPlaygroundAudioProcessorEditor::~VstTestPlaygroundAudioProcessorEditor()
{
    // Important to release resources correctly
    webBrowserComponent->goToURL("about:blank");
}

void VstTestPlaygroundAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void VstTestPlaygroundAudioProcessorEditor::resized()
{
    webBrowserComponent->setBounds(getLocalBounds());
}