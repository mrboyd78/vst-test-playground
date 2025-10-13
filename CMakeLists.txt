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
    FORMATS VST3
)

target_compile_definitions(VstTestPlayground PRIVATE JUCE_VST3_CAN_REPLACE_VST2=0)

target_sources(VstTestPlayground PRIVATE
    Source/PluginProcessor.h
    Source/PluginEditor.h
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

# Set C++ standard to 20 for modern features
target_compile_features(VstTestPlayground PUBLIC cxx_std_20)

target_link_libraries(VstTestPlayground
    PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

juce_generate_juce_header(VstTestPlayground)