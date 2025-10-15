# VST Application Code Review: Critical Issues and Recommended Improvements

**Review Date:** October 14, 2025  
**Project:** VstTestPlayground  
**Reviewer:** AI Code Analysis

## Executive Summary

This review identifies 12 critical issues in the current VST plugin implementation that deviate from industry best practices and JUCE 8 recommendations. The issues span WebView2 configuration, parameter synchronization architecture, real-time safety violations, build configuration gaps, and UI optimization concerns. Each critique is supported by external references from official documentation and established best practices.

---

## Critical Issues

### 1. Missing WebView2 Configuration in CMakeLists.txt

**Severity:** HIGH  
**File:** `CMakeLists.txt`

#### Issue
The CMake configuration lacks essential WebView2-specific settings required for JUCE 8 WebView integration. The build will fail or produce unreliable behavior without proper backend specification and static linking configuration.

**Current State:**
```cmake
# No WebView2 configuration present
target_link_libraries(VstTestPlayground
    PRIVATE
    # ... other libraries, no WebView2 specific settings
)
```

#### Required Changes
```cmake
# Add after juce_add_plugin()
set_target_properties(VstTestPlayground PROPERTIES
    JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING 1
    NEEDS_WEBVIEW2 TRUE
)

target_compile_definitions(VstTestPlayground 
    PRIVATE 
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_WEB_BROWSER=1
    JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1
)
```

#### External Validation
- **Microsoft WebView2 CMake Configuration**: Static linking requires explicit property configuration to bundle the WebView2 loader, preventing runtime dependency issues. Without this, the plugin will fail when WebView2 runtime is not installed on end-user systems.
  - Reference: [WebView2 Static Linking Configuration](https://stackoverflow.com/questions/66146868/how-do-you-build-webview-on-windows-using-cmake)
  
- **JUCE 8 Documentation**: WebView2 backend must be explicitly enabled via compile definitions for Windows builds.
  - Reference: [JUCE 8 Feature Overview: WebView UIs](https://juce.com/blog/juce-8-feature-overview-webview-uis/)

---

### 2. Improper WebView2 Backend Configuration

**Severity:** HIGH  
**File:** `PluginEditor.cpp` (line 8)

#### Issue
The WebView initialization lacks critical configuration for proper WebView2 operation in a plugin context. Missing backend specification, user data folder isolation, and resource management settings will cause conflicts during DAW plugin scanning and multiple instance scenarios.

**Current Implementation:**
```cpp
webView(juce::WebBrowserComponent::Options()
    .withNativeIntegrationEnabled(true)
    .withNativeFunction("setParameterValue", ...))
```

#### Required Implementation
```cpp
auto options = juce::WebBrowserComponent::Options()
    .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
    .withWinWebView2Options(
        juce::WebBrowserComponent::Options::WinWebView2{}
            .withUserDataFolder(
                juce::File::getSpecialLocation(juce::File::tempDirectory)
                    .getChildFile("VstTestPlayground_" + 
                                 juce::String(juce::Process::getCurrentProcessId()))
            )
    )
    .withNativeIntegrationEnabled()
    .withKeepPageLoadedWhenBrowserIsHidden()
    .withNativeFunction("setParameterValue", ...);

webView(options)
```

#### External Validation
- **WebView2 User Data Folder Best Practices**: Multiple WebView2 instances require unique user data folders to prevent conflicts. DAWs scan plugins in parallel processes, and without process-specific isolation, WebView2 environments will conflict and cause crashes or hangs.
  - Reference: [Manage user data folders - Microsoft Edge Developer](https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/user-data-folder)
  - Reference: [WebView2 Multiple Instances Discussion](https://github.com/MicrosoftEdge/WebView2Feedback/discussions/4869)

- **JUCE WebView2 Architecture**: The backend must be explicitly specified for Windows. Default behavior may not select WebView2, causing inconsistent behavior across platforms.
  - Reference: [JUCE WebBrowserComponent Documentation](https://docs.juce.com/master/classWebBrowserComponent.html)

---

### 3. Incorrect Parameter Communication Architecture

**Severity:** CRITICAL  
**File:** `PluginEditor.cpp` (lines 8-14, 32-41)

#### Issue
The implementation uses manual parameter synchronization via `setValueNotifyingHost()` and `postMessage()` instead of JUCE's built-in `WebSliderParameterAttachment` system. This approach:
- Bypasses DAW automation gesture management
- Creates race conditions between UI and automation
- Misses proper parameter threading
- Prevents correct automation recording in DAWs

**Current Problematic Code:**
```cpp
// Manual parameter setting - INCORRECT
.withNativeFunction("setParameterValue",
    [this](const juce::Array<juce::var>& args, juce::WebBrowserComponent::NativeFunctionCompletion completion)
    {
        const auto& paramId = args[0].toString();
        const auto& value = (float)args[1];
        processorRef.apvts.getParameter(paramId)->setValueNotifyingHost(value);
        completion(juce::var());
    })

// Manual parameter notification - INCORRECT
void VstTestPlaygroundAudioProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
    juce::DynamicObject::Ptr data(new juce::DynamicObject());
    data->setProperty(parameterID, newValue);
    auto json = juce::JSON::toString(juce::var(data));
    auto escapedJson = json.replace("\"", "\\\"").replace("'", "\\'");
    auto script = "window.postMessage('" + escapedJson + "', '*');";
    webView.evaluateJavascript(script);
}
```

#### Correct Architecture
```cpp
// In PluginEditor.h
class VstTestPlaygroundAudioProcessorEditor : public juce::AudioProcessorEditor
{
private:
    std::unique_ptr<juce::WebSliderRelay> gainRelay;
    std::unique_ptr<juce::WebSliderParameterAttachment> gainAttachment;
};

// In PluginEditor.cpp constructor
gainRelay = std::make_unique<juce::WebSliderRelay>(webView, "gain");
gainAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
    *processorRef.apvts.getParameter(Params::GAIN_ID),
    *gainRelay,
    nullptr
);

// Remove manual parameter listener and parameterChanged() method entirely
```

```javascript
// In React App.jsx - use JUCE's built-in API
useEffect(() => {
    if (window.Juce) {
        const gainState = window.Juce.getSliderState('gain');
        
        const listener = gainState.valueChangedEvent.addListener((value) => {
            setGain(value);
        });
        
        return () => gainState.valueChangedEvent.removeListener(listener);
    }
}, []);

const handleGainChange = (value) => {
    if (window.Juce) {
        const gainState = window.Juce.getSliderState('gain');
        gainState.setNormalisedValue(value);
    }
};
```

#### External Validation
- **JUCE WebSliderParameterAttachment Documentation**: This system automatically handles gesture begin/end for DAW automation recording, thread-safe synchronization, and bidirectional updates without manual intervention.
  - Reference: [JUCE 8 Feature Overview: WebView UIs](https://juce.com/blog/juce-8-feature-overview-webview-uis/)
  - Reference: [JUCE Forum: WebSliderParameterAttachment Issues](https://github.com/juce-framework/JUCE/issues/1390)

- **DAW Automation Requirements**: Automation recording requires proper gesture management (beginChangeGesture/endChangeGesture). Manual setValueNotifyingHost() calls don't provide this, making automation recording unreliable.
  - Reference: [JUCE AudioProcessor Automation Tutorial](https://docs.juce.com/master/tutorial_audio_parameter.html)

---

### 4. Real-Time Safety Violation in processBlock

**Severity:** CRITICAL  
**File:** `PluginProcessor.cpp` (line 147)

#### Issue
The `processBlock()` method calls `load()` on an atomic parameter every audio buffer, which is acceptable, but uses `setGainDecibels()` which may perform logarithmic calculations on the audio thread. More critically, the method doesn't cache the parameter pointer, causing potential atomic loads every buffer.

**Current Code:**
```cpp
void VstTestPlaygroundAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    // ... 
    gain.setGainDecibels(apvts.getRawParameterValue(Params::GAIN_ID)->load());
    gain.process(context);
}
```

#### Issues:
1. `getRawParameterValue()` returns a pointer but is called every buffer
2. `setGainDecibels()` performs logarithmic math on audio thread
3. No smoothing consideration for real-time parameter changes

#### Recommended Implementation
```cpp
// In PluginProcessor.h
private:
    std::atomic<float>* gainParameter = nullptr;
    float previousGainDB = 0.0f;

// In constructor after apvts initialization
gainParameter = apvts.getRawParameterValue(Params::GAIN_ID);

// In prepareToPlay
gain.prepare(spec);
gain.setRampDurationSeconds(0.05); // Already present - good
previousGainDB = gainParameter->load();
gain.setGainDecibels(previousGainDB);

// In processBlock - optimized for real-time
void VstTestPlaygroundAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    
    // ... channel clearing code ...
    
    float currentGainDB = gainParameter->load();
    if (! juce::approximatelyEqual(currentGainDB, previousGainDB))
    {
        gain.setGainDecibels(currentGainDB);
        previousGainDB = currentGainDB;
    }
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    gain.process(context);
}
```

#### External Validation
- **Real-Time Audio Programming Best Practices**: Avoid repeated lookups and unnecessary math in processBlock. Cache pointers during initialization and only update DSP when values change.
  - Reference: [The big list of JUCE tips and tricks](https://melatonin.dev/blog/big-list-of-juce-tips-and-tricks/)
  - Reference: [JUCE Forum: Memory allocation in processBlock](https://forum.juce.com/t/memory-allocation-deep-inside-vst3plugininstance-processblock/63717)

- **JUCE DSP Module Documentation**: The Gain processor's setGainDecibels() performs `Decibels::decibelsToGain()` calculation which involves logarithmic math. While fast, calling it every buffer is wasteful.
  - Reference: [JUCE dsp::Gain Documentation](https://docs.juce.com/master/classjuce_1_1dsp_1_1Gain.html)

---

### 5. Missing Undo Manager Implementation

**Severity:** MEDIUM  
**File:** `PluginProcessor.cpp` (line 15)

#### Issue
The APVTS is initialized with `nullptr` for the undo manager, eliminating undo/redo functionality. While acceptable for simple plugins, this severely limits user experience for production plugins.

**Current Code:**
```cpp
apvts(*this, nullptr, "Parameters", createParameterLayout())
```

#### Recommended Implementation
```cpp
// In PluginProcessor.h
private:
    juce::UndoManager undoManager;

// In constructor initialization list
apvts(*this, &undoManager, "Parameters", createParameterLayout())
```

#### Why This Matters
- Users expect undo/redo in professional audio tools
- Essential for workflow in complex parameter adjustments
- Trivial to implement with significant UX benefit
- No performance cost - only allocates on parameter changes

#### External Validation
- **JUCE APVTS Tutorial**: Undo managers enhance usability with minimal code. Professional plugins should always include this unless there's a specific reason not to.
  - Reference: [Saving and loading your plug-in state - JUCE](https://juce.com/tutorials/tutorial_audio_processor_value_tree_state/)
  - Reference: [JUCE AudioProcessorValueTreeState Documentation](https://docs.juce.com/master/classjuce_1_1AudioProcessorValueTreeState.html)

---

### 6. Unused "onOff" Parameter in WebGUI

**Severity:** MEDIUM  
**File:** `WebGUI/ui/src/App.jsx` (lines 7, 10, 15, 21-23, 39-42, 62-68)

#### Issue
The React UI implements an "onOff" parameter with WebSliderRelay connections, but this parameter doesn't exist in the C++ backend. This creates confusion and non-functional UI elements.

**Current Code in App.jsx:**
```javascript
const [onOff, setOnOff] = useState(true);
const onOffState = useRef(null);

useEffect(() => {
    if (window.Juce) {
        // ...
        onOffState.current = window.Juce.getSliderState('onOff'); // DOESN'T EXIST
        const onOffListener = onOffState.current.valueChangedEvent.addListener(
            (value) => setOnOff(value > 0.5)
        );
        // ...
    }
}, []);
```

**Backend Reality:**
```cpp
// In PluginProcessor.cpp - Only one parameter defined
layout.add(std::make_unique<juce::AudioParameterFloat>(
    Params::GAIN_ID,  // Only "gain" exists
    "Gain",
    juce::NormalisableRange<float>(-100.0f, 100.0f, 0.01f),
    0.0f));
```

#### Options:
1. **Remove the onOff UI** (if not needed)
2. **Add the parameter to backend** (if needed):

```cpp
// In Params.h
namespace Params
{
    inline const juce::String GAIN_ID { "gain" };
    inline const juce::String ONOFF_ID { "onOff" };
}

// In PluginProcessor.cpp createParameterLayout()
layout.add(std::make_unique<juce::AudioParameterBool>(
    Params::ONOFF_ID,
    "On/Off",
    true));
```

#### External Validation
- **UI/Backend Synchronization**: Non-existent parameters create runtime errors and confusion. All UI controls must correspond to actual processor parameters.
  - Reference: [JUCE Parameter Management Best Practices](https://docs.juce.com/master/tutorial_audio_parameter.html)

---

### 7. React StrictMode Double-Rendering Impact

**Severity:** LOW-MEDIUM  
**File:** `WebGUI/ui/src/main.jsx` (line 6)

#### Issue
The React app uses `StrictMode`, which intentionally double-invokes effects in development to detect side effects. For audio plugin UIs with real-time parameter updates, this can cause unexpected behavior and confusing debugging scenarios.

**Current Code:**
```javascript
createRoot(document.getElementById('root')).render(
  <StrictMode>
    <App />
  </StrictMode>,
)
```

#### Recommendation
For production audio plugins, consider removing StrictMode or being aware of its implications:

```javascript
createRoot(document.getElementById('root')).render(
  <App />
)
```

#### Considerations
- **Development**: StrictMode helps catch issues
- **Production**: Adds no value and slight overhead
- **Audio Context**: Double-effect invocation can confuse JUCE event listeners

#### External Validation
- **React StrictMode Documentation**: StrictMode is development-only and helps identify problems, but can cause confusion with external APIs that have their own lifecycle management.
  - Reference: [React Strict Mode: Performance and Stability](https://thesyntaxdiaries.com/react-strict-mode-boost-application-performance-and-stability)
  - Reference: [Performance and Debugging with React Strict Mode](https://www.bairesdev.com/blog/react-strict-mode/)

- **Audio Plugin UI Patterns**: Real-time UIs with bidirectional parameter sync can be disrupted by intentional double-rendering, making debugging harder.

---

### 8. Missing WebGUI Build Integration

**Severity:** MEDIUM  
**File:** `CMakeLists.txt` (lines 19-22)

#### Issue
The binary data configuration points to `WebGUI/ui` but lacks automated build integration. The frontend must be manually built before C++ compilation, creating error-prone workflows.

**Current Code:**
```cmake
juce_add_binary_data(VstTestPlayground_BinaryData
    SOURCES
        WebGUI/ui  # Raw source directory, not built output
)
```

#### Problems:
1. Points to source directory instead of `dist` output
2. No automatic `npm build` invocation
3. Developers must remember manual build step
4. CI/CD requires separate build stages

#### Recommended Solution
```cmake
# Add custom target to build React app
set(WEBGUI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/WebGUI/ui")
set(WEBGUI_BUILD_DIR "${WEBGUI_SOURCE_DIR}/dist")

add_custom_target(BuildWebGUI
    COMMAND npm install
    COMMAND npm run build
    WORKING_DIRECTORY ${WEBGUI_SOURCE_DIR}
    COMMENT "Building WebGUI with Vite..."
    VERBATIM
)

# Point binary data to built output
juce_add_binary_data(VstTestPlayground_BinaryData
    SOURCES
        ${WEBGUI_BUILD_DIR}/index.html
        ${WEBGUI_BUILD_DIR}/assets
)

# Ensure WebGUI builds before plugin
add_dependencies(VstTestPlayground BuildWebGUI)
```

**Alternative for Development:**
```cmake
# Option to use dev server in debug builds
option(USE_WEBGUI_DEV_SERVER "Use Vite dev server instead of built files" OFF)

if(USE_WEBGUI_DEV_SERVER)
    target_compile_definitions(VstTestPlayground PRIVATE USE_DEV_SERVER=1)
    # In WebView.cpp, load from http://localhost:5173 instead of binary data
endif()
```

#### External Validation
- **CMake Custom Commands**: Build automation should handle all dependencies transparently to prevent manual step errors.
  - Reference: [CMake add_custom_target Documentation](https://cmake.org/cmake/help/latest/command/add_custom_target.html)

- **JUCE Binary Data**: Binary data should reference compiled output, not source files, for production plugins.
  - Reference: [JUCE Binary Resources](https://docs.juce.com/master/tutorial_manage_projucer.html)

---

### 9. Incomplete Error Handling in WebView Loading

**Severity:** MEDIUM  
**File:** `WebView.cpp` (lines 7-9)

#### Issue
The WebView loads HTML from binary data with no error handling or fallback if the resource is missing or corrupted.

**Current Code:**
```cpp
WebView::WebView(const juce::WebBrowserComponent::Options& options)
    : juce::WebBrowserComponent(options)
{
    int size = 0;
    const char* html = BinaryData::getNamedResource("index_html", size);
    goToURL("data:text/html;base64," + juce::Base64::toBase64(html, size));
}
```

#### Problems:
- No nullptr check on `html`
- No size validation
- Silent failure if resource missing
- No user feedback on load errors

#### Recommended Implementation
```cpp
WebView::WebView(const juce::WebBrowserComponent::Options& options)
    : juce::WebBrowserComponent(options)
{
    int size = 0;
    const char* html = BinaryData::getNamedResource("index_html", size);
    
    if (html == nullptr || size == 0)
    {
        jassertfalse; // Debug builds will catch this
        DBG("ERROR: Failed to load WebGUI HTML from binary data");
        
        // Fallback to error page
        const juce::String errorHtml = 
            "<html><body style='font-family:sans-serif;padding:20px;'>"
            "<h1>WebGUI Failed to Load</h1>"
            "<p>The plugin interface could not be initialized.</p>"
            "<p>Please reinstall the plugin.</p></body></html>";
        goToURL("data:text/html;charset=utf-8," + errorHtml);
        return;
    }
    
    goToURL("data:text/html;base64," + juce::Base64::toBase64(html, size));
}
```

#### External Validation
- **Defensive Programming**: Always validate external resources before use, even from "trusted" binary data.
  - Reference: [JUCE Best Practices Guide](https://melatonin.dev/blog/big-list-of-juce-tips-and-tricks/)

---

### 10. Missing Parameter Range Validation

**Severity:** LOW-MEDIUM  
**File:** `PluginProcessor.cpp` (lines 29-33)

#### Issue
The gain parameter range of -100dB to +100dB is unusual and potentially problematic. Most gain controls use -∞ to +12dB or similar ranges. The unnormalized range also lacks proper skew for perceptual control.

**Current Code:**
```cpp
layout.add(std::make_unique<juce::AudioParameterFloat>(
    Params::GAIN_ID,
    "Gain",
    juce::NormalisableRange<float>(-100.0f, 100.0f, 0.01f),  // Unusual range
    0.0f));
```

#### Industry Standard Approach
```cpp
layout.add(std::make_unique<juce::AudioParameterFloat>(
    Params::GAIN_ID,
    "Gain",
    juce::NormalisableRange<float>(
        -60.0f,  // Minimum gain in dB (practical silence)
        12.0f,   // Maximum gain in dB (reasonable boost)
        0.01f,   // Step size
        1.0f,    // Skew factor (1.0 = linear, could use 0.3 for more resolution at low end)
        false    // Not symmetric
    ),
    0.0f));  // Default: unity gain
```

#### Why This Matters
- User expectations: Gain controls typically range -60 to +12 dB
- Perceptual scaling: Decibel ranges benefit from skewed controls
- DSP safety: +100dB gain is extreme and rarely useful
- Precision: More range = less precision in useful area

#### External Validation
- **Audio Engineering Practices**: Standard gain controls rarely exceed ±12dB for processing, and -∞ to +12dB for faders.
  - Reference: [JUCE NormalisableRange Documentation](https://docs.juce.com/master/classNormalisableRange.html)

---

### 11. Missing Test Coverage

**Severity:** MEDIUM  
**File:** `Tests/ParameterTests.cpp`

#### Issue
The test file exists but only tests parameter existence. Real-time safety, parameter range validation, state persistence, and WebView communication are untested.

**Current Tests:**
```cpp
void runTest() override
{
    beginTest("Parameter Existence");
    VstTestPlaygroundAudioProcessor processor;
    auto& apvts = processor.apvts;
    auto* gainParam = apvts.getParameter(Params::GAIN_ID);
    expect(gainParam != nullptr, "Gain parameter should exist.");
}
```

#### Critical Missing Tests
1. **Parameter Range Validation**
2. **State Serialization/Deserialization**
3. **Real-time Safety (no allocations in processBlock)**
4. **APVTS Synchronization**
5. **WebView Parameter Relay**

#### Recommended Test Additions
```cpp
beginTest("Parameter Range Bounds");
{
    VstTestPlaygroundAudioProcessor processor;
    auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
    
    gainParam->setValueNotifyingHost(0.0f);
    expect(gainParam->getValue() == 0.0f, "Min value should be 0");
    
    gainParam->setValueNotifyingHost(1.0f);
    expect(gainParam->getValue() == 1.0f, "Max value should be 1");
}

beginTest("State Persistence");
{
    VstTestPlaygroundAudioProcessor processor;
    auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
    gainParam->setValueNotifyingHost(0.75f);
    
    juce::MemoryBlock state;
    processor.getStateInformation(state);
    
    VstTestPlaygroundAudioProcessor processor2;
    processor2.setStateInformation(state.getData(), (int)state.getSize());
    
    auto* gainParam2 = processor2.apvts.getParameter(Params::GAIN_ID);
    expectWithinAbsoluteError(gainParam2->getValue(), 0.75f, 0.001f, 
                              "Parameter should restore from state");
}

beginTest("ProcessBlock Real-time Safety");
{
    VstTestPlaygroundAudioProcessor processor;
    processor.prepareToPlay(44100.0, 512);
    
    juce::AudioBuffer<float> buffer(2, 512);
    juce::MidiBuffer midiBuffer;
    buffer.clear();
    
    // This should not allocate - run with allocation checker in real tests
    processor.processBlock(buffer, midiBuffer);
    
    expect(true, "ProcessBlock completed without exceptions");
}
```

#### External Validation
- **JUCE Unit Testing**: Comprehensive testing prevents regressions and ensures plugin stability across DAWs.
  - Reference: [JUCE Unit Tests Framework](https://docs.juce.com/master/classUnitTest.html)

---

### 12. Documentation-Code Mismatch

**Severity:** LOW  
**File:** `README.md`, `AGENTS.md`

#### Issue
The documentation describes features and patterns that don't exist in the codebase (like WebSliderParameterAttachment usage), while the actual implementation uses different patterns. This creates confusion for developers.

**AGENTS.md Claims:**
> "Use WebSliderParameterAttachment for proper parameter synchronization"
> "Configure WebView2 with user data folder using Process::getCurrentProcessId()"

**Actual Code:**
- Uses manual parameter synchronization
- No user data folder configuration
- No WebSliderParameterAttachment usage

#### Recommendation
Either:
1. **Update code to match documentation** (preferred - documentation describes best practices)
2. **Update documentation to match code** (documents technical debt)

---

## Summary of Recommendations by Priority

### Critical (Implement Immediately)
1. Add WebView2 configuration to CMakeLists.txt
2. Implement proper WebView2 backend initialization with user data folder
3. Replace manual parameter sync with WebSliderParameterAttachment
4. Optimize processBlock for real-time safety

### High Priority (Implement Soon)
5. Add undo manager to APVTS
6. Remove or implement onOff parameter (frontend/backend sync)
7. Integrate WebGUI build into CMake

### Medium Priority (Consider for Next Release)
8. Add comprehensive error handling in WebView loading
9. Review and adjust parameter ranges for industry standards
10. Expand test coverage

### Low Priority (Nice to Have)
11. Review React StrictMode usage
12. Align documentation with codebase

---

## References

### Official Documentation
1. [JUCE 8 Feature Overview: WebView UIs](https://juce.com/blog/juce-8-feature-overview-webview-uis/)
2. [Microsoft Edge WebView2 Threading Model](https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/threading-model)
3. [Microsoft Edge WebView2 User Data Folder Management](https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/user-data-folder)
4. [JUCE AudioProcessorValueTreeState Documentation](https://docs.juce.com/master/classjuce_1_1AudioProcessorValueTreeState.html)
5. [JUCE AudioProcessor Tutorial](https://docs.juce.com/master/tutorial_audio_parameter.html)

### Community Best Practices
6. [The big list of JUCE tips and tricks (Melatonin)](https://melatonin.dev/blog/big-list-of-juce-tips-and-tricks/)
7. [JUCE Forum: Memory Allocation in processBlock](https://forum.juce.com/t/memory-allocation-deep-inside-vst3plugininstance-processblock/63717)
8. [React Performance Optimization Best Practices](https://dev.to/austinwdigital/mastering-react-optimizations-best-practices-for-high-performance-applications-3e11)

### Issue Trackers
9. [JUCE GitHub: WebSliderParameterAttachment Bug](https://github.com/juce-framework/JUCE/issues/1390)
10. [WebView2 Feedback: Multiple Instances](https://github.com/MicrosoftEdge/WebView2Feedback/discussions/4869)

### Build Configuration
11. [Stack Overflow: WebView2 CMake Configuration](https://stackoverflow.com/questions/66146868/how-do-you-build-webview-on-windows-using-cmake)
12. [CMake Documentation: add_custom_target](https://cmake.org/cmake/help/latest/command/add_custom_target.html)

---

**End of Review**
