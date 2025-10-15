#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class WebView  : public juce::WebBrowserComponent
{
public:
    WebView(const juce::WebBrowserComponent::Options& options);
    ~WebView() override;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebView)
};