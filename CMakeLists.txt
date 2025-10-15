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
    USE_BINARY_DATA TRUE
    NEEDS_WEBVIEW2 TRUE
)

# Configure WebView2 static linking for Windows
if(WIN32)
    set_target_properties(VstTestPlayground PROPERTIES
        JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING ON
    )
endif()

# Build WebGUI before creating binary data
set(WEBGUI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/WebGUI/ui")
set(WEBGUI_BUILD_DIR "${WEBGUI_SOURCE_DIR}/dist")

# Custom target to build React app
add_custom_target(BuildWebGUI
    COMMAND npm install --silent
    COMMAND npm run build
    WORKING_DIRECTORY ${WEBGUI_SOURCE_DIR}
    COMMENT "Building WebGUI with Vite..."
    VERBATIM
)

# Create binary data from built output
juce_add_binary_data(VstTestPlayground_BinaryData
    SOURCES
        ${WEBGUI_BUILD_DIR}/index.html
)

# Ensure WebGUI builds before binary data is created
add_dependencies(VstTestPlayground_BinaryData BuildWebGUI)

target_compile_definitions(VstTestPlayground 
    PRIVATE 
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_WEB_BROWSER=1
    JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1
)

target_sources(VstTestPlayground PRIVATE
    Source/PluginProcessor.h
    Source/PluginEditor.h
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/CustomLookAndFeel.cpp
    Source/WebView.cpp
)

# Set C++ standard to 20 for modern features
target_compile_features(VstTestPlayground PUBLIC cxx_std_20)

target_link_libraries(VstTestPlayground
    PRIVATE
    VstTestPlayground_BinaryData
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

#==============================================================================
# Unit Tests
#==============================================================================
if(JUCE_BUILD_EXTRAS AND CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    juce_add_console_app(VstTestPlayground_Tests
        PRODUCT_NAME "VstTestPlayground Tests"
    )

    juce_generate_juce_header(VstTestPlayground_Tests)

    target_sources(VstTestPlayground_Tests PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/CustomLookAndFeel.cpp
        Source/WebView.cpp
        Tests/Main.cpp
        Tests/ParameterTests.cpp

    )

    target_compile_definitions(VstTestPlayground_Tests PRIVATE
        JUCE_UNIT_TESTS=1
        JucePlugin_Name="VstTestPlayground"
    )

    target_link_libraries(VstTestPlayground_Tests PRIVATE
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
endif()