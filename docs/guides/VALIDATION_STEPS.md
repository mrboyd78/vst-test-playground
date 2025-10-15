# Track 5: Integration & Validation - MANUAL STEPS REQUIRED

**Status:** ⏳ AWAITING EXECUTION

Since PowerShell 6+ (pwsh) is not available on this system, the integration and validation phase requires manual execution. All code changes have been successfully implemented and are ready for testing.

## Prerequisites Check

Before proceeding, ensure you have:

- [ ] **PowerShell 6+** OR **Windows PowerShell 5.1+**
- [ ] **Node.js** (v18 or higher) - [Download](https://nodejs.org/)
- [ ] **npm** (comes with Node.js)
- [ ] **CMake** (3.22 or higher) - [Download](https://cmake.org/download/)
- [ ] **Visual Studio 2022** with C++ workload
- [ ] **JUCE 8.x** in the `JUCE` subdirectory

## Quick Validation (5 minutes)

### Option A: Using the Automated Script (Recommended)

```powershell
# Open PowerShell as Administrator in the project directory
cd C:\Users\mrchr\Documents\GitHub\vst-test-playground

# Run the validation script
.\validate_build.ps1
```

This script will:
1. ✓ Check all prerequisites
2. ✓ Build the WebGUI automatically
3. ✓ Configure CMake
4. ✓ Build the plugin
5. ✓ Run all tests
6. ✓ Show summary and next steps

### Option B: Manual Step-by-Step

If the script doesn't work, follow these manual steps:

#### Step 1: Verify Prerequisites
```powershell
node --version      # Should show v18.x or higher
npm --version       # Should show 9.x or higher
cmake --version     # Should show 3.22 or higher
```

#### Step 2: Build WebGUI
```powershell
cd WebGUI\ui
npm install
npm run build
cd ..\..
```

**Verify:** Check that `WebGUI\ui\dist\index.html` exists

#### Step 3: Configure CMake
```powershell
# Clean previous build (optional)
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue

# Configure
cmake -G "Visual Studio 17 2022" -A x64 -B build
```

**Expected:** No errors, should see "Build files written to: build"

#### Step 4: Build Plugin
```powershell
cmake --build build --config Release
```

**Expected:** Build completes with "Build succeeded"

#### Step 5: Run Tests
```powershell
.\build\Release\VstTestPlayground_Tests.exe
```

**Expected:** All 6 tests pass:
- ✓ Parameter Existence
- ✓ Parameter Range Validation
- ✓ State Persistence
- ✓ ProcessBlock Execution
- ✓ Gain Parameter Affects Output
- ✓ Undo Manager Integration

#### Step 6: Locate Plugin
```powershell
# The plugin should be installed at:
# C:\Program Files\Common Files\VST3\VstTestPlayground.vst3
```

## Validation Checklist

Once built, verify these items:

### Build Validation
- [ ] WebGUI built without errors (`WebGUI\ui\dist\` contains files)
- [ ] CMake configured successfully
- [ ] Plugin compiled without errors
- [ ] All 6 unit tests pass
- [ ] Plugin file exists at VST3 location

### Functional Validation (Load in DAW)
- [ ] Plugin loads in DAW without crashing
- [ ] WebView UI displays correctly (not blank/error)
- [ ] Gain knob is visible and interactive
- [ ] Gain parameter shows dB value (-60 to +12 range)
- [ ] Moving knob affects audio output
- [ ] Parameter automation records in DAW
- [ ] Multiple instances can be opened simultaneously
- [ ] Undo/redo works (Ctrl+Z after parameter change)
- [ ] Presets save and load correctly

## Common Issues & Solutions

### Issue: npm not found
```powershell
# Install Node.js from https://nodejs.org/
# Restart PowerShell after installation
```

### Issue: CMake not found
```powershell
# Install CMake from https://cmake.org/download/
# Or via Visual Studio Installer
```

### Issue: "Visual Studio 17 2022" generator not found
```powershell
# Use this instead for VS 2019:
cmake -G "Visual Studio 16 2019" -A x64 -B build

# Or for Ninja:
cmake -G "Ninja" -B build
```

### Issue: WebGUI build fails
```powershell
# Manually check what's wrong:
cd WebGUI\ui
npm install --verbose
npm run build --verbose
```

### Issue: Tests fail
```powershell
# Run in Debug mode for more info:
cmake --build build --config Debug
.\build\Debug\VstTestPlayground_Tests.exe
```

### Issue: Plugin not found in VST3 folder
```powershell
# Check build output, it might be at:
dir build\VstTestPlayground_artefacts\Release\VST3\VstTestPlayground.vst3

# Manually copy if needed:
Copy-Item -Recurse "build\VstTestPlayground_artefacts\Release\VST3\VstTestPlayground.vst3" `
    "C:\Program Files\Common Files\VST3\"
```

## Expected Build Output

### Successful WebGUI Build
```
> npm run build

vite v7.1.7 building for production...
✓ 145 modules transformed.
dist/index.html                   0.46 kB │ gzip: 0.30 kB
dist/assets/index-[hash].css      1.89 kB │ gzip: 0.95 kB
dist/assets/index-[hash].js      143.21 kB │ gzip: 46.15 kB
✓ built in 1.23s
```

### Successful CMake Configuration
```
-- The C compiler identification is MSVC 19.39.33523.0
-- The CXX compiler identification is MSVC 19.39.33523.0
-- Configuring done (5.2s)
-- Generating done (0.3s)
-- Build files have been written to: build
```

### Successful Build
```
Build started...
...
VstTestPlayground.vcxproj -> build\VstTestPlayground_artefacts\Release\VST3\VstTestPlayground.vst3
...
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

### Successful Tests
```
Parameter Tests for VstTestPlayground

Starting test: Parameter Existence...
   ok

Starting test: Parameter Range Validation...
   ok

Starting test: State Persistence...
   ok

Starting test: ProcessBlock Execution...
   ok

Starting test: Gain Parameter Affects Output...
   ok

Starting test: Undo Manager Integration...
   ok

Tests completed successfully!
```

## Performance Benchmarks

After a successful build, you should see:

- **Build Time:** 2-5 minutes (first build), 30-60 seconds (incremental)
- **Plugin Load Time:** < 3 seconds in DAW
- **CPU Usage (idle):** < 0.5%
- **CPU Usage (processing):** < 1% @ 44.1kHz/512 samples
- **Memory Usage:** ~50-100 MB (includes WebView2 runtime)

## Next Actions

After successful validation:

1. **Test in DAW** - See `QUICKSTART_TESTING.md` for detailed test procedures
2. **Create Backup** - Save a copy of the working build
3. **Version Control** - Commit all changes with clear message
4. **Documentation** - Update README.md with new features
5. **Optimization** - Profile in real-world scenarios

## Validation Status Report

Complete this after running validation:

```
Date: _______________
Build Environment: Windows / macOS / Linux
Visual Studio Version: _______________
CMake Version: _______________
Node.js Version: _______________

✓ WebGUI Build:        PASS / FAIL
✓ CMake Configure:     PASS / FAIL
✓ Plugin Build:        PASS / FAIL
✓ Unit Tests:          PASS / FAIL (__ / 6 passed)
✓ DAW Load Test:       PASS / FAIL / NOT TESTED
✓ Parameter Test:      PASS / FAIL / NOT TESTED
✓ Automation Test:     PASS / FAIL / NOT TESTED
✓ Multi-Instance Test: PASS / FAIL / NOT TESTED

Notes:
_________________________________________
_________________________________________
_________________________________________
```

## Support

If you encounter issues:

1. Check the error message carefully
2. Review the "Common Issues & Solutions" section above
3. Check build logs in `build\` directory
4. Verify all prerequisites are correctly installed
5. Try a clean rebuild (delete `build` and `WebGUI\ui\dist` folders)

---

**Status:** Ready for manual execution - Follow Option A or Option B above

Once validation is complete, update the IMPLEMENTATION_PLAN.md with final status.
