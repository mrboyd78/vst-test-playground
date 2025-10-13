cmake_minimum_required(VERSION 3.22)
project(VstTestPlayground VERSION 0.0.1)

# Add JUCE as a subdirectory or via find_package
# This assumes JUCE is located in a 'JUCE' subdirectory
add_subdirectory(JUCE)

juce_add_plugin(VstTestPlayground
    PRODUCT_NAME "VstTestPlayground"
    COMPANY_NAME "mrboyd78"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    PLUGIN_MANUFACTURER_CODE "MrBd"
    PLUGIN_CODE "Vstp"
    FORMATS VST3 AU
    # Enable WebView2 and static linking as recommended
    NEEDS_WEBVIEW2 TRUE
    JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING TRUE
)

target_sources(VstTestPlayground PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

# Set C++ standard to 20 for modern features
target_compile_features(VstTestPlayground PUBLIC cxx_std_20)

juce_generate_juce_header(VstTestPlayground)