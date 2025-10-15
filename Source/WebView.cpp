#include "WebView.h"
#ifndef JUCE_UNIT_TESTS
#include "BinaryData.h"
#endif

WebView::WebView(const juce::WebBrowserComponent::Options& options)
    : juce::WebBrowserComponent(options)
{
#if JUCE_DEBUG || JUCE_UNIT_TESTS
    goToURL("http://localhost:3000");
#else
    int size = 0;
    const char* html = BinaryData::getNamedResource("index_html", size);
    
    if (html == nullptr || size == 0)
    {
        jassertfalse; // Debug builds will catch this
        DBG("ERROR: Failed to load WebGUI HTML from binary data");
        
        // Fallback to error page
        const juce::String errorHtml = 
            "<!DOCTYPE html><html><head><title>Error</title></head>"
            "<body style='font-family:sans-serif;padding:20px;background:#1a1a1a;color:#fff;'>"
            "<h1>WebGUI Failed to Load</h1>"
            "<p>The plugin interface could not be initialized.</p>"
            "<p>Please reinstall the plugin or check that the build completed successfully.</p>"
            "</body></html>";
        goToURL("data:text/html;charset=utf-8," + juce::URL::addEscapeChars(errorHtml, false));
        return;
    }
    
    goToURL("data:text/html;base64," + juce::Base64::toBase64(html, size));
#endif
}

WebView::~WebView()
{
}
