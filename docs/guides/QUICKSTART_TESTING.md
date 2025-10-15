# Quick Start: Testing Your Updated Plugin

## What Changed?

All critical issues from the code review have been fixed:
- ✅ WebView2 properly configured with static linking
- ✅ Automated WebGUI build integration
- ✅ Proper DAW automation support (WebSliderParameterAttachment)
- ✅ Real-time safety optimizations in processBlock
- ✅ Undo/redo support added
- ✅ Industry-standard parameter ranges (-60dB to +12dB)
- ✅ Comprehensive error handling
- ✅ Full test suite

## Prerequisites

Before building, ensure you have:
- ✅ Visual Studio 2022 (Windows) or Xcode (macOS)
- ✅ CMake 3.22 or higher
- ✅ JUCE 8.x in your `JUCE` directory
- ✅ **Node.js and npm installed** (required for WebGUI build)

## Build Steps

### 1. Configure the Build

```bash
# Windows (PowerShell)
cmake -G "Visual Studio 17 2022" -A x64 -B build

# macOS/Linux
cmake -B build
```

**Note:** The WebGUI will automatically build during CMake configuration thanks to the new `BuildWebGUI` target.

### 2. Build the Plugin

```bash
# Release build (recommended for testing)
cmake --build build --config Release

# Debug build (for development)
cmake --build build --config Debug
```

### 3. Run Tests

```bash
# Windows
.\build\Release\VstTestPlayground_Tests.exe

# macOS/Linux
./build/Release/VstTestPlayground_Tests
```

**Expected Output:**
```
Parameter Tests for VstTestPlayground
  Parameter Existence... PASSED
  Parameter Range Validation... PASSED
  State Persistence... PASSED
  ProcessBlock Execution... PASSED
  Gain Parameter Affects Output... PASSED
  Undo Manager Integration... PASSED

All tests completed successfully!
```

### 4. Find Your Plugin

**Windows:** `C:\Program Files\Common Files\VST3\VstTestPlayground.vst3`

**macOS:** `~/Library/Audio/Plug-Ins/VST3/VstTestPlayground.vst3`

**Linux:** `~/.vst3/VstTestPlayground.vst3`

## Testing in a DAW

### 1. Load the Plugin
- Open your DAW (Reaper, Ableton, FL Studio, etc.)
- Create a new track
- Load "VstTestPlayground" as an insert effect

### 2. Test Parameter Automation
1. Move the gain knob in the plugin UI
2. Enable automation recording in your DAW
3. Move the gain knob again
4. Stop recording
5. **Verify:** Automation should be recorded properly (this was broken before!)

### 3. Test Multiple Instances
1. Add the plugin to 2-3 different tracks
2. Open all plugin windows simultaneously
3. **Verify:** All instances work without conflicts (user data folder isolation)

### 4. Test Undo/Redo
1. Change the gain parameter
2. Use Ctrl+Z (or Cmd+Z on Mac) in the DAW
3. **Verify:** Parameter value reverts (new feature!)

### 5. Test Preset Saving
1. Adjust the gain to a specific value
2. Save as a preset in your DAW
3. Change the gain to a different value
4. Load the saved preset
5. **Verify:** Gain returns to saved value

## Common Issues & Solutions

### Issue: CMake can't find npm

**Error:** `'npm' is not recognized as an internal or external command`

**Solution:**
```bash
# Install Node.js from https://nodejs.org/
# Then verify:
npm --version
```

### Issue: WebGUI build fails

**Error:** `Error: Cannot find module '...'`

**Solution:**
```bash
# Manually build WebGUI first
cd WebGUI/ui
npm install
npm run build
cd ../..

# Then retry CMake build
cmake --build build --config Release
```

### Issue: Plugin doesn't show WebView

**Error:** Black window or "WebGUI Failed to Load" message

**Solution:**
1. Check that `WebGUI/ui/dist/index.html` exists
2. Verify the build completed without errors
3. Check build logs for BinaryData generation errors

### Issue: Parameters don't respond

**Error:** Moving knob doesn't change audio

**Solution:**
1. Ensure you're using the Release build
2. Check that audio is passing through (input -> output)
3. Try gain values between -12dB and +6dB first

## Development Workflow

### Hot Reloading (Optional)

For faster UI iteration during development:

1. **Start Vite dev server:**
```bash
cd WebGUI/ui
npm run dev
```

2. **Modify WebView.cpp temporarily:**
```cpp
// In WebView constructor, replace goToURL line with:
goToURL("http://localhost:5173");
```

3. **Rebuild plugin** - now UI changes auto-reload!

4. **Before final build:** Revert WebView.cpp to load from BinaryData

### Adding New Parameters

1. **Add to Params.h:**
```cpp
namespace Params
{
    inline const juce::String GAIN_ID { "gain" };
    inline const juce::String NEW_PARAM_ID { "newparam" };  // Add this
}
```

2. **Add to PluginProcessor.cpp createParameterLayout():**
```cpp
layout.add(std::make_unique<juce::AudioParameterFloat>(
    Params::NEW_PARAM_ID,
    "New Parameter",
    juce::NormalisableRange<float>(0.0f, 1.0f),
    0.5f));
```

3. **Add to PluginEditor (in constructor):**
```cpp
newParamRelay = std::make_unique<juce::WebSliderRelay>(*webView, Params::NEW_PARAM_ID);
newParamAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
    *processorRef.apvts.getParameter(Params::NEW_PARAM_ID),
    *newParamRelay,
    nullptr
);
```

4. **Add to App.jsx:**
```javascript
const newParamState = window.Juce.getSliderState('newparam');
// Add UI control...
```

5. **Rebuild and test!**

## Performance Validation

### Real-Time Safety Check

Use a profiler to verify no allocations in processBlock:

**Windows (Visual Studio):**
1. Build in Debug mode
2. Run with Performance Profiler
3. Enable "Memory Usage"
4. Process audio
5. Verify no allocations during processBlock

**macOS (Instruments):**
1. Build plugin
2. Load in DAW
3. Attach Instruments (Allocations template)
4. Process audio
5. Verify no allocations in processBlock

### CPU Usage

Typical CPU usage should be:
- **Idle:** < 0.1%
- **Processing (512 samples @ 44.1kHz):** < 1%
- **With WebView visible:** < 2%

## Next Steps

1. ✅ **Build the plugin** (see above)
2. ✅ **Run tests** to verify everything works
3. ✅ **Test in your DAW** to confirm automation works
4. ✅ **Try multiple instances** to verify isolation works
5. 📝 Consider adding more DSP (filters, effects, etc.)
6. 📝 Enhance the UI with more controls
7. 📝 Add preset management
8. 📝 Set up CI/CD for automated builds

## Documentation

- **Code Review:** `docs/CODE_REVIEW.md` - All issues found
- **Implementation Plan:** `docs/IMPLEMENTATION_PLAN.md` - What was done
- **Implementation Summary:** `docs/IMPLEMENTATION_SUMMARY.md` - Detailed changes
- **This Guide:** `docs/QUICKSTART_TESTING.md`

## Getting Help

If you encounter issues:

1. Check build logs for specific errors
2. Verify Node.js and npm are installed
3. Ensure JUCE 8.x is properly installed
4. Review the Implementation Summary for breaking changes
5. Check that all prerequisites are met

## Success Checklist

- [ ] Project builds without errors
- [ ] All unit tests pass
- [ ] Plugin loads in DAW
- [ ] WebView UI displays correctly
- [ ] Gain parameter responds to knob
- [ ] Audio processing works (input → gain → output)
- [ ] DAW automation records parameter changes
- [ ] Multiple instances work simultaneously
- [ ] Undo/redo works for parameters
- [ ] Presets save and load correctly

**If all items are checked, you're ready for production! 🎉**

---

*For detailed technical information, see the Implementation Summary.*
