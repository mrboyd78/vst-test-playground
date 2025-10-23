# VST3 Plugin Template

A production-ready VST3 plugin template using JUCE 8 and CMake. Start building professional audio plugins in minutes!

## Features

- ✅ Modern C++17 codebase
- ✅ CMake build system
- ✅ JUCE 8 framework
- ✅ Parameter management with AudioProcessorValueTreeState
- ✅ Professional GUI with resizable editor
- ✅ DSP processing with JUCE DSP module
- ✅ State persistence (save/load presets)
- ✅ Cross-platform (Windows, macOS, Linux)
- ✅ GitHub Actions CI/CD ready
- ✅ Well-documented code
- ✅ **GitHub Copilot workflow prompts** for enhanced productivity
- ✅ **Structured issue templates** for consistent project management
- ✅ **Ways-of-work documentation** for planning and requirements

## Prerequisites

**Note:** JUCE 8.0.10 is included as a git submodule - you don't need to download it separately!

### Windows
- Visual Studio 2022 (Community or higher) with "Desktop development with C++" workload
- CMake 3.22 or higher

### macOS
- Xcode 13 or higher
- CMake 3.22 or higher

### Linux
- GCC 9+ or Clang 10+
- CMake 3.22 or higher
- JUCE dependencies: `sudo apt-get install libasound2-dev libjack-dev libcurl4-openssl-dev libfreetype6-dev libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev libwebkit2gtk-4.1-dev libglu1-mesa-dev mesa-common-dev`

## Quick Start

### 1. Clone This Repository

Clone the repository with submodules to automatically fetch JUCE:

```bash
git clone --recursive https://github.com/mrboyd78/vst-test-playground.git
cd vst-test-playground
```

If you already cloned without `--recursive`, initialize the submodules:

```bash
git submodule update --init --recursive
```

**Note:** JUCE is now managed as a git submodule at version 8.0.10. This keeps the repository clean and makes updates easier.

### 2. Configure Your Plugin

Edit `CMakeLists.txt` and customize these values:

```cmake
set(PLUGIN_NAME "MyAwesomePlugin")
set(PLUGIN_VERSION "1.0.0")
set(COMPANY_NAME "YourCompany")
set(PLUGIN_MANUFACTURER_CODE "Manu")  # Get unique codes at juce.com
set(PLUGIN_CODE "Plug")
set(PLUGIN_IS_SYNTH FALSE)            # TRUE for synth, FALSE for effect
```

### 3. Build

#### Windows (PowerShell)

```powershell
# Initialize Visual Studio environment
. C:\Users\YourName\build_vst3.ps1

# Configure (JUCE is already available via submodule)
cmake -G "Visual Studio 17 2022" -A x64 -B build

# Build
cmake --build build --config Release
```

#### macOS/Linux

```bash
# Configure (JUCE is already available via submodule)
cmake -B build

# Build
cmake --build build --config Release
```

### 4. Find Your Plugin

**Windows:** `C:\Program Files\Common Files\VST3\YourPlugin.vst3`

**macOS:** `~/Library/Audio/Plug-Ins/VST3/YourPlugin.vst3`

**Linux:** `~/.vst3/YourPlugin.vst3`

## Project Structure

```
vst3-plugin-template/
├── CMakeLists.txt           # Build configuration
├── README.md                # This file
├── .gitignore              # Git ignore rules
├── LICENSE                 # License file
├── include/                # Header files
│   ├── PluginProcessor.h   # Main audio processor
│   └── PluginEditor.h      # GUI editor
├── source/                 # Implementation files
│   ├── PluginProcessor.cpp
│   └── PluginEditor.cpp
├── docs/                   # Documentation
│   └── DEVELOPMENT.md      # Development guide
└── .github/                # GitHub Actions
    └── workflows/
        └── build.yml       # CI/CD pipeline
```

## Development

### Adding Parameters

Edit `PluginProcessor.cpp` in the `createParameterLayout()` function:

```cpp
layout.add(std::make_unique<juce::AudioParameterFloat>(
    "cutoff",                                    // Parameter ID
    "Cutoff",                                    // Display name
    juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f),
    1000.0f                                      // Default value
));
```

### Adding GUI Controls

In `PluginEditor.h`, add member variables:

```cpp
juce::Slider cutoffSlider;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
```

In `PluginEditor.cpp` constructor, set up and attach:

```cpp
addAndMakeVisible(cutoffSlider);
cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
    audioProcessor.getValueTreeState(),
    "cutoff",
    cutoffSlider
);
```

### Adding DSP Processing

Use JUCE DSP modules in `PluginProcessor.h`:

```cpp
juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                juce::dsp::IIR::Coefficients<float>> filter;
```

Initialize in `prepareToPlay()` and process in `processBlock()`.

## Building Different Formats

Edit `CMakeLists.txt` to build additional formats:

```cmake
set(PLUGIN_FORMATS VST3 AU Standalone)  # Add AU, AAX, or Standalone
```

## Tips

### Get Unique Plugin Codes
Visit [juce.com/admin/registration](https://juce.com/admin/registration) to get unique manufacturer and plugin codes for distribution.

### Disable JUCE Splash Screen
If you have a JUCE license, set in `CMakeLists.txt`:
```cmake
JUCE_DISPLAY_SPLASH_SCREEN=0
```

### Debug Mode
Build in Debug mode for development:
```bash
cmake --build build --config Debug
```

### Generic Editor
For quick prototyping, use the generic editor:
```cpp
// In PluginProcessor.cpp createEditor()
return new juce::GenericAudioProcessorEditor(*this);
```

## Testing

Load your plugin in a DAW:
- **Free DAWs:** Reaper (60-day trial), Cakewalk, Tracktion Waveform
- **Plugin Hosts:** AudioPluginHost (included with JUCE), Hosting AU

## CI/CD

This template includes GitHub Actions for automated building on push/PR. See `.github/workflows/build.yml`.

## Resources

- [JUCE Documentation](https://docs.juce.com/)
- [JUCE Forum](https://forum.juce.com/)
- [JUCE Tutorials](https://juce.com/learn/tutorials)
- [The Audio Programmer YouTube](https://www.youtube.com/c/TheAudioProgrammer)

## Workflow Tools

This repository includes GitHub Copilot-powered workflow automation to enhance productivity:

### 🤖 AI-Powered Agents

- **[GitHub Copilot Prompts](.github/copilot-prompts/README.md)** - Automated workflow agents
  - Create structured GitHub issues from implementation plans
  - Intelligent Git Flow branch creation
  - Generate comprehensive PRDs for epics and features
  - Create test strategies and QA plans

### 📝 Issue Templates

- **[Feature Request](.github/ISSUE_TEMPLATE/feature_request.yml)** - Structured feature proposals
- **[Bug Report](.github/ISSUE_TEMPLATE/bug_report.yml)** - Detailed bug reports
- **[Chore Request](.github/ISSUE_TEMPLATE/chore_request.yml)** - Maintenance and refactoring tasks

### 📋 Planning Documentation

- **[Ways of Work](docs/ways-of-work/README.md)** - Structured approach to planning
  - Epic and feature PRD templates
  - Implementation planning structure
  - Test strategy documentation

**Learn More**: See [GitHub Copilot Prompts Documentation](.github/copilot-prompts/README.md) for detailed usage instructions.

## License

This template is provided under the MIT License. See LICENSE file.

JUCE itself is licensed separately - see [JUCE licensing](https://juce.com/juce-8-licence/) for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Support

- Open an issue for bugs or feature requests
- Check existing issues before creating new ones
- Provide detailed information and code samples when reporting bugs

---

**Happy Plugin Development! 🎵**
