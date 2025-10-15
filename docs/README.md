# VST Test Playground - Documentation

Welcome to the VST Test Playground documentation! This guide will help you understand, build, and extend this JUCE-based VST3 plugin project.

## 📚 Documentation Structure

### Guides

Step-by-step tutorials and how-to guides for common tasks:

- **[Development Guide](guides/DEVELOPMENT.md)** - Architecture overview and development workflow
- **[Quick Start Testing](guides/QUICKSTART_TESTING.md)** - Test the plugin after building
- **[User Manual](guides/USER_MANUAL.md)** - Complete guide for plugin users
- **[Validation Steps](guides/VALIDATION_STEPS.md)** - Manual validation and testing procedures
- **[JUCE Submodule Management](guides/JUCE_SUBMODULE.md)** - Working with the JUCE dependency

### Reference

Quick reference materials and API documentation:

- **[Quick Reference](reference/REFERENCE.md)** - Common tasks and code snippets

### Implementation History

Historical documentation from previous implementations (archive):

- **[Code Review](implementation-history/CODE_REVIEW.md)** - Past code review findings
- **[Implementation Plan](implementation-history/IMPLEMENTATION_PLAN.md)** - Previous implementation strategy
- **[Implementation Status](implementation-history/IMPLEMENTATION_STATUS.md)** - Historical status reports
- **[Implementation Summary](implementation-history/IMPLEMENTATION_SUMMARY.md)** - Past implementation details

## 🚀 Quick Start

New to the project? Start here:

1. **First Time Setup**: Read the main [README.md](../README.md) in the project root
2. **Clone with Submodules**: Follow the [JUCE Submodule Guide](guides/JUCE_SUBMODULE.md)
3. **Build the Project**: Use the build instructions in the main README
4. **Test the Plugin**: Follow the [Quick Start Testing](guides/QUICKSTART_TESTING.md) guide

## 🎯 Common Tasks

### Building the Project

See the main [README.md](../README.md) for platform-specific build instructions.

### Adding New Parameters

Refer to the [Development Guide](guides/DEVELOPMENT.md) for parameter management.

### Updating JUCE Version

See the [JUCE Submodule Management](guides/JUCE_SUBMODULE.md) guide.

### Testing Changes

Follow the [Validation Steps](guides/VALIDATION_STEPS.md) for comprehensive testing.

## 🔧 Project Structure

```
vst-test-playground/
├── JUCE/                   # JUCE framework (git submodule)
├── Source/                 # Plugin source code
│   ├── PluginProcessor.*   # Audio processing
│   ├── PluginEditor.*      # GUI editor
│   └── WebView.*          # WebView integration
├── Tests/                  # Unit tests
├── docs/                   # This documentation
│   ├── guides/            # How-to guides
│   ├── reference/         # Quick reference
│   └── implementation-history/  # Historical docs
├── CMakeLists.txt         # Build configuration
└── README.md              # Main project README
```

## 📖 Key Concepts

### Audio Processing

The plugin uses JUCE's `AudioProcessor` class for audio processing. All DSP code runs in a real-time thread and must be lock-free and allocation-free.

### Parameter Management

Parameters are managed via `AudioProcessorValueTreeState` (APVTS), which provides:
- Thread-safe parameter access
- Automatic DAW automation support
- Undo/redo functionality
- State persistence

### WebView Integration

The plugin uses JUCE's WebView2 integration for a modern web-based UI, allowing rapid development with HTML/CSS/JavaScript.

### Git Submodules

JUCE is included as a git submodule, keeping the repository clean while ensuring consistent versions across all developers.

## 🐛 Troubleshooting

### Build Issues

1. Check that JUCE submodule is initialized: `git submodule update --init --recursive`
2. Verify all system dependencies are installed (see main README)
3. Ensure CMake version is 3.22 or higher

### WebView Issues

- On Windows, ensure WebView2 Runtime is installed
- Check that WebGUI build completed successfully
- Verify `NEEDS_WEBVIEW2 TRUE` in CMakeLists.txt

### Submodule Issues

See the [JUCE Submodule Management](guides/JUCE_SUBMODULE.md) guide for common submodule problems.

## 📝 Contributing

When contributing documentation:

1. **Guides** - Add step-by-step tutorials to `guides/`
2. **Reference** - Add quick reference materials to `reference/`
3. **Update TOC** - Update this README when adding new documents
4. **Keep Current** - Update docs when changing code

## 🔗 External Resources

- [JUCE Documentation](https://docs.juce.com/)
- [JUCE Tutorials](https://juce.com/learn/tutorials)
- [JUCE Forum](https://forum.juce.com/)
- [VST3 SDK Documentation](https://steinbergmedia.github.io/vst3_doc/)
- [Git Submodules Guide](https://git-scm.com/book/en/v2/Git-Tools-Submodules)

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/mrboyd78/vst-test-playground/issues)
- **Discussions**: [GitHub Discussions](https://github.com/mrboyd78/vst-test-playground/discussions)

---

**Last Updated**: October 2025  
**JUCE Version**: 8.0.10
