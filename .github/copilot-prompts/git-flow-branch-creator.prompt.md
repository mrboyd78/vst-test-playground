---
description: 'Intelligent Git Flow branch creator for VST plugin development that analyzes git changes and creates appropriate branches following the Git Flow branching model.'
tools: ['run_in_terminal', 'get_terminal_output']
mode: 'agent'
---

# Git Flow Branch Creator for VST Test Playground

Analyzes your current git changes and intelligently creates appropriate Git Flow branches for JUCE VST3 plugin development.

## Purpose

This agent automates branch creation by analyzing your changes and determining the correct branch type (feature/release/hotfix) according to Git Flow conventions, with special considerations for audio plugin development.

## Workflow

**Follow these steps:**

1. Run `git status` to review current repository state
2. Run `git diff` (unstaged) or `git diff --cached` (staged) to analyze changes
3. Analyze changes using the Git Flow framework below
4. Determine appropriate branch type
5. Generate semantic branch name
6. Create and switch to the new branch
7. Provide summary and next steps

## Git Flow Branch Types for VST Plugins

### Feature Branches

**Purpose**: New features, enhancements, non-critical improvements

- **Branch from**: `develop` or `main`
- **Merge to**: `develop` or `main`
- **Naming**: `feature/descriptive-name` or `feature/ticket-number-description`

**Indicators for VST Plugin Development:**
- New audio processing features (filters, effects, generators)
- GUI/editor enhancements or new controls
- WebView UI components or pages
- New parameters or parameter groups
- API additions (new methods, classes)
- Performance improvements (non-critical)
- Code refactoring or modernization
- Documentation additions
- Test infrastructure improvements
- Build system enhancements

**Examples:**
- `feature/webview-spectrum-analyzer`
- `feature/stereo-width-control`
- `feature/preset-management`
- `feature/parameter-automation-curves`
- `feature/PLUGIN-123-midi-cc-mapping`

### Release Branches

**Purpose**: Release preparation, version bumps, final testing

- **Branch from**: `develop`
- **Merge to**: `develop` AND `main`
- **Naming**: `release/X.Y.Z` or `release-X.Y.Z`

**Indicators:**
- Version number changes in CMakeLists.txt
- Plugin version updates
- Build configuration for release
- Documentation finalization
- Release notes updates
- Dependency version locks
- Minor bug fixes before release
- Plugin code signing preparation

**Examples:**
- `release/1.2.0`
- `release/2.0.0-beta`
- `release/1.1.5`

### Hotfix Branches

**Purpose**: Critical production bug fixes requiring immediate deployment

- **Branch from**: `main`
- **Merge to**: `develop` AND `main`
- **Naming**: `hotfix/X.Y.Z` or `hotfix/critical-issue`

**Indicators:**
- Audio glitches or crashes in production plugins
- Security vulnerabilities
- DAW compatibility issues
- Critical parameter automation bugs
- Plugin state corruption
- Memory leaks or crashes
- Build failures in production
- Code signing issues

**Examples:**
- `hotfix/1.2.1-audio-buffer-overflow`
- `hotfix/parameter-crash`
- `hotfix/daw-automation-fix`
- `hotfix/1.2.1`

## Branch Naming Conventions

### Feature Branches
```
feature/[ticket-number-]descriptive-name

Examples:
- feature/webview-parameter-bridge
- feature/PROJ-456-fft-visualization
- feature/multi-band-compressor
- feature/preset-browser-ui
```

### Release Branches
```
release/X.Y.Z

Examples:
- release/1.0.0
- release/2.1.0
- release/1.2.0-beta
```

### Hotfix Branches
```
hotfix/X.Y.Z OR hotfix/critical-description

Examples:
- hotfix/1.2.1
- hotfix/buffer-overflow-fix
- hotfix/automation-recording-crash
```

## Analysis Framework

### Step 1: Identify Changed Files

Categorize files by type:

**Audio Processing Files:**
- `Source/PluginProcessor.h/cpp`
- `Source/DSP/*.h/cpp`
- Audio algorithm implementations

**GUI/Editor Files:**
- `Source/PluginEditor.h/cpp`
- `Source/UI/*.h/cpp`
- Look and feel classes

**WebView Files:**
- `Source/WebView*.h/cpp`
- `WebGUI/**/*` (HTML/CSS/JS)
- Web integration bridges

**Build System Files:**
- `CMakeLists.txt`
- `*.cmake`
- Build scripts

**Configuration Files:**
- Plugin metadata
- Version numbers
- `.github/workflows/*`

**Documentation:**
- `README.md`
- `docs/**/*.md`
- Code comments (significant)

**Tests:**
- `Tests/**/*`
- Test fixtures or mocks

### Step 2: Analyze Change Nature

**Additive Changes (→ Feature):**
- New classes or methods
- New UI components
- New parameters
- New audio processing chains
- New WebView pages
- Enhanced functionality

**Corrective Changes (→ Hotfix if critical, Feature if not):**
- Bug fixes
- Crash fixes
- Performance fixes
- Compatibility fixes
- Security patches

**Preparatory Changes (→ Release):**
- Version bumps
- Release notes
- Build configuration for release
- Documentation finalization
- Dependency locks

### Step 3: Assess Urgency

**Critical (→ Hotfix):**
- Plugin crashes in production
- Audio glitches causing distortion
- Data corruption or state loss
- Security vulnerabilities
- DAW incompatibility

**Standard (→ Feature):**
- New features
- Enhancements
- Non-critical bug fixes
- Refactoring
- Documentation

**Staged (→ Release):**
- Preparing for version release
- Final testing phase
- Version number updates

## Decision Tree

```
START
  ↓
Does it fix a critical production bug? → YES → HOTFIX
  ↓ NO
Is this release preparation (version bump, final tweaks)? → YES → RELEASE
  ↓ NO
Is this new functionality, enhancement, or standard work? → YES → FEATURE
  ↓
DEFAULT → FEATURE
```

## VST Plugin-Specific Considerations

### Audio Thread Safety
If changes touch `processBlock()` or audio rendering:
- Include "audio-thread" in branch description
- Flag for real-time safety review
- Example: `feature/audio-thread-safe-buffer-resize`

### Parameter Changes
If adding/modifying parameters:
- Include parameter name in branch
- Consider automation compatibility
- Example: `feature/add-resonance-parameter`

### WebView Integration
If working with WebView:
- Specify UI or bridge work
- Example: `feature/webview-preset-browser`

### Build System Changes
If modifying CMake or builds:
- Specify platform if relevant
- Example: `feature/windows-static-webview2`

### Breaking Changes
If changes break plugin compatibility:
- Mark as major version or breaking
- Example: `feature/breaking-parameter-redesign`

## Edge Cases

### Mixed Changes
**Scenario**: Changes include both features and bug fixes

**Resolution**: 
1. Prioritize the most significant change type
2. Or suggest splitting into multiple branches
3. Example: Create feature branch, commit features first, then create separate hotfix for bugs

### No Changes Detected
**Scenario**: No changes in git status/diff

**Resolution**: 
- Inform user and suggest checking git status
- Verify working directory is clean
- Ask user what they intend to work on

### Already on Feature Branch
**Scenario**: Currently on a feature/hotfix/release branch

**Resolution**:
- Analyze if new branch is needed
- Suggest continuing on current branch if appropriate
- Or create sub-branch if work diverges

### Conflicting Branch Names
**Scenario**: Suggested branch name already exists

**Resolution**:
- Append incremental suffix (-v2, -alt, etc.)
- Suggest alternative descriptive names
- Check out existing branch if it's the right one

## Command Execution

### Creating Feature Branch
```bash
git checkout -b feature/descriptive-name develop
# or
git checkout -b feature/descriptive-name main
```

### Creating Release Branch
```bash
git checkout -b release/X.Y.Z develop
```

### Creating Hotfix Branch
```bash
git checkout -b hotfix/X.Y.Z main
# or
git checkout -b hotfix/critical-description main
```

## Validation Checklist

### Pre-Analysis
- [ ] Repository in clean state (or changes are intentional)
- [ ] Current branch is appropriate starting point
- [ ] Remote repository is up to date
- [ ] JUCE submodule is properly initialized

### Analysis Quality
- [ ] All modified files analyzed
- [ ] Change type correctly identified
- [ ] Branch type follows Git Flow principles
- [ ] Branch name is semantic and descriptive
- [ ] VST-specific considerations addressed

### Execution Safety
- [ ] Source branch (develop/main) exists and is accessible
- [ ] Proposed branch name doesn't conflict
- [ ] User has permissions to create branches
- [ ] JUCE submodule won't cause conflicts

## Examples

### Example 1: New WebView Feature

**Git Diff Output:**
```diff
+ // WebGUI/src/components/SpectrumAnalyzer.jsx
+ import React from 'react';
+ 
+ const SpectrumAnalyzer = ({ fftData }) => {
+   // Render FFT visualization
+ };

+ // Source/WebViewBridge.cpp
+ void WebViewBridge::emitFFTData(const std::vector<float>& data)
+ {
+     webComponent.emitEventIfBrowserIsVisible("fftData", dataJSON);
+ }
```

**Analysis:**
- New files in WebGUI directory
- New method in WebViewBridge
- Additive changes (new feature)
- Not critical or urgent
- WebView-related work

**Decision**: FEATURE branch

**Branch Name**: `feature/webview-spectrum-analyzer`

**Command**:
```bash
git checkout -b feature/webview-spectrum-analyzer develop
```

### Example 2: Critical Crash Fix

**Git Diff Output:**
```diff
  void PluginProcessor::processBlock(AudioBuffer<float>& buffer)
  {
-     if (buffer.getNumChannels() > 0)
+     if (buffer.getNumChannels() > 0 && buffer.getNumSamples() > 0)
      {
          // Process audio
      }
  }
```

**Analysis:**
- Changes in processBlock (audio thread)
- Fixes crash condition
- Critical for production
- Affects all users

**Decision**: HOTFIX branch

**Branch Name**: `hotfix/1.2.1-processblock-crash`

**Command**:
```bash
git checkout -b hotfix/1.2.1-processblock-crash main
```

### Example 3: Release Preparation

**Git Diff Output:**
```diff
  # CMakeLists.txt
- set(PLUGIN_VERSION "1.1.5")
+ set(PLUGIN_VERSION "1.2.0")

  # CHANGELOG.md
+ ## [1.2.0] - 2025-10-23
+ ### Added
+ - WebView spectrum analyzer
+ - Preset management system
```

**Analysis:**
- Version number change
- Release notes update
- Preparatory work for release
- No new features or bug fixes

**Decision**: RELEASE branch

**Branch Name**: `release/1.2.0`

**Command**:
```bash
git checkout -b release/1.2.0 develop
```

### Example 4: Parameter Addition

**Git Diff Output:**
```diff
  AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
  {
      layout.add(std::make_unique<AudioParameterFloat>(
+         "resonance", "Resonance",
+         NormalisableRange<float>(0.0f, 1.0f), 0.5f));
+     layout.add(std::make_unique<AudioParameterFloat>(
          "cutoff", "Cutoff",
          NormalisableRange<float>(20.0f, 20000.0f), 1000.0f));
  }
```

**Analysis:**
- New parameter added
- Enhancement to existing feature
- Not critical
- Parameter-related work

**Decision**: FEATURE branch

**Branch Name**: `feature/add-resonance-parameter`

**Command**:
```bash
git checkout -b feature/add-resonance-parameter develop
```

## Next Steps After Branch Creation

1. **Verify Branch**:
   ```bash
   git branch --show-current
   git status
   ```

2. **Stage and Commit Changes**:
   ```bash
   git add .
   git commit -m "Descriptive commit message"
   ```

3. **Push Branch** (if ready):
   ```bash
   git push -u origin <branch-name>
   ```

4. **Create PR** (when work is complete):
   - Link to related issues
   - Include testing instructions
   - Add appropriate reviewers
   - Fill out PR template

5. **Follow Git Flow Merge Strategy**:
   - Use `--no-ff` flag to preserve branch history
   - Tag releases on main branch
   - Delete branch after successful merge

## Git Flow Quick Reference

### Main Branches
- **main**: Production-ready code, every commit is a release
- **develop**: Integration branch for features, latest development changes

### Supporting Branches
- **feature**: Branch from develop, merge back to develop
- **release**: Branch from develop, merge to both develop and main
- **hotfix**: Branch from main, merge to both develop and main

### Merge Strategy
- Always use `--no-ff` flag to preserve branch history
- Tag releases on main branch
- Delete branches after successful merge
- Keep main and develop up to date

---

**Usage**: Run this agent when you have changes and need to create a new branch. It will analyze your changes and create the appropriate Git Flow branch automatically.
