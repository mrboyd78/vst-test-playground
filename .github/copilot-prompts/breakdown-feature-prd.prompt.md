---
mode: 'agent'
description: 'Feature Product Requirements Document (PRD) generator for VST plugin features, breaking down epics into detailed feature specifications.'
tools: ['codebase', 'search', 'create_file', 'read_file']
---

# Feature Product Requirements Document (PRD) Generator

Generate detailed Feature-level Product Requirements Documents (PRDs) for VST Test Playground, breaking down epics into implementable feature specifications.

## Purpose

This agent helps Product Managers translate Epic-level requirements into detailed Feature PRDs that engineering teams can use to create technical specifications and implementation plans.

## Output Location

The PRD will be saved to:
```
/docs/ways-of-work/plan/{epic-name}/{feature-name}/prd.md
```

## Feature PRD Structure

### 1. Feature Name

A clear, concise, and descriptive name for the feature.

**Examples for VST Plugin Features:**
- "WebView Parameter Slider Component"
- "FFT Spectrum Analyzer Display"
- "Preset Save and Load System"
- "MIDI CC Parameter Mapping"
- "Real-Time Waveform Visualization"

### 2. Epic

Link to parent Epic PRD and Architecture documents.

**Template:**
```markdown
## Epic

**Parent Epic**: [WebView-Based Plugin UI System](/docs/ways-of-work/plan/webview-ui/epic.md)

**Epic Architecture**: [Technical Architecture](/docs/ways-of-work/plan/webview-ui/architecture.md)

**Related Features**:
- [WebView Integration Core](/docs/ways-of-work/plan/webview-ui/webview-core/prd.md)
- [Parameter Bridge System](/docs/ways-of-work/plan/webview-ui/parameter-bridge/prd.md)
```

### 3. Goal

#### Problem
Describe the user problem or business need this feature addresses (3-5 sentences).

**Example:**
```markdown
## Problem

Plugin users need responsive, visual feedback when adjusting parameters, but current JUCE sliders lack modern interaction patterns and visual polish. Designers want to implement custom slider designs with animations and value displays, but creating custom JUCE components requires extensive C++ knowledge. Each slider needs to maintain sub-5ms latency for automation recording while providing smooth visual feedback at 60fps. The feature must work identically across all DAWs and platforms to ensure consistent user experience.
```

#### Solution
Explain how this feature solves the problem.

**Example:**
```markdown
## Solution

Implement a React-based parameter slider component that communicates with JUCE's AudioProcessorValueTreeState through WebSliderRelay. The component will use Juce.getSliderState() for bidirectional parameter synchronization, implementing proper gesture management for DAW automation. CSS animations and requestAnimationFrame will provide smooth 60fps visuals while throttling parameter updates to prevent audio thread overload. The component will be fully customizable via props, enabling designers to create unique slider styles without touching C++ code.
```

#### Impact
Expected outcomes and metrics to be improved.

**Example:**
```markdown
## Impact

**Development Speed**: 10x faster slider iteration (from 30 minutes to <3 minutes per design change)

**User Experience**: Smooth 60fps animations vs. native JUCE's 20-30fps on some platforms

**Designer Independence**: 100% of slider customization achievable in CSS/React

**Performance**: <3ms parameter latency, <0.5% CPU overhead per slider

**Code Reusability**: Single component used across all parameters, reducing code by 70%
```

### 4. User Personas

Describe the target users for this feature.

**Template:**
```markdown
## User Personas

### Primary: [Persona Name]
- **Role**: [Role description]
- **Use Case**: [How they'll use this feature]
- **Success Criteria**: [What makes it successful for them]

### Secondary: [Persona Name]
- **Role**: [Role description]
- **Use Case**: [How they'll use this feature]
- **Success Criteria**: [What makes it successful for them]
```

**Example:**
```markdown
## User Personas

### Primary: UI/UX Designer
- **Role**: Designs plugin interface components
- **Use Case**: Creates custom slider designs for filter cutoff, resonance, and gain parameters using CSS and React props
- **Success Criteria**: Can iterate on slider design 5+ times per hour without developer help

### Secondary: Plugin Developer
- **Role**: Integrates components into plugin architecture
- **Use Case**: Adds new parameters to plugin and connects them to pre-built slider component
- **Success Criteria**: <10 minutes to add a new parameter with fully functional slider

### Secondary: End User (Music Producer)
- **Role**: Uses plugin in DAW
- **Use Case**: Adjusts filter cutoff with smooth, responsive slider while recording automation
- **Success Criteria**: Feels responsive, no lag, automation records correctly in DAW
```

### 5. User Stories

Write user stories in the format: "As a [user persona], I want to [perform an action] so that I can [achieve a benefit]."

**Example:**
```markdown
## User Stories

### Designer Stories
- As a UI designer, I want to customize slider appearance with CSS so that I can match the plugin's visual brand without learning C++
- As a UI designer, I want to see slider changes update instantly in the browser so that I can iterate rapidly on visual design
- As a UI designer, I want to add custom value displays and tooltips so that I can improve parameter feedback

### Developer Stories
- As a plugin developer, I want to connect a parameter to a slider with one line of code so that I can add new controls quickly
- As a plugin developer, I want sliders to automatically handle automation gestures so that I don't have to implement gesture management repeatedly
- As a plugin developer, I want slider performance to be optimized by default so that I can add many sliders without performance concerns

### End User Stories
- As a music producer, I want parameter sliders to respond immediately to mouse movement so that I can make precise adjustments in real-time
- As a music producer, I want slider automation to record accurately in my DAW so that I can create dynamic parameter movements in my mix
- As a music producer, I want sliders to show current values clearly so that I can recall settings visually without checking numbers
```

### 6. Requirements

#### Functional Requirements

Detailed, bulleted list of what the system must do. Be specific and unambiguous.

**Categories for VST Features:**
- Parameter interaction and control
- Visual feedback and display
- DAW integration and automation
- Performance and responsiveness
- Error handling and edge cases
- Accessibility and keyboard control
- State persistence and restoration

**Example:**
```markdown
## Functional Requirements

### Parameter Communication
- [ ] Slider connects to APVTS parameter via WebSliderRelay
- [ ] Bidirectional sync: UI changes update processor, automation updates UI
- [ ] Gesture management: sliderDragStarted() on mousedown, sliderDragEnded() on mouseup
- [ ] Normalized value range (0.0-1.0) with automatic denormalization for display
- [ ] Throttled updates: Max 60 parameter changes per second to avoid overwhelming audio thread

### Visual Behavior
- [ ] Smooth CSS transitions for value changes (<16ms frame time)
- [ ] Custom track, thumb, and fill styling via CSS classes
- [ ] Value display with formatted units (Hz, dB, %, etc.)
- [ ] Hover state with value preview tooltip
- [ ] Focus indicator for keyboard navigation

### Interaction Patterns
- [ ] Click-and-drag vertical/horizontal adjustment
- [ ] Double-click to reset to default value
- [ ] Shift+drag for fine adjustment (10x precision)
- [ ] Scroll wheel support (Ctrl+scroll for fine)
- [ ] Touch/trackpad gesture support
- [ ] Text input for direct value entry

### DAW Integration
- [ ] Automation recording captures parameter changes correctly
- [ ] Automation playback updates slider position in real-time
- [ ] Parameter learn mode for MIDI CC mapping
- [ ] Host undo/redo integration via APVTS
- [ ] Parameter name and value displayed in DAW's generic editor

### Edge Cases
- [ ] Handle invalid parameter IDs gracefully with error logging
- [ ] Prevent value updates during parameter transitions
- [ ] Handle rapid parameter changes without dropped updates
- [ ] Maintain state when plugin UI is hidden/shown
- [ ] Prevent race conditions between UI and automation updates
```

#### Non-Functional Requirements

Quality attributes and constraints.

**Example:**
```markdown
## Non-Functional Requirements

### Performance
- [ ] Parameter latency: <3ms from mouse event to processor update
- [ ] Visual update rate: Stable 60fps (16.67ms per frame) during interaction
- [ ] CPU overhead: <0.5% per slider on modern CPU (measured with 10 simultaneous sliders)
- [ ] Memory footprint: <1MB for slider component code
- [ ] Bundle size: <50KB for minified component

### Compatibility
- [ ] Works in Chrome 90+, Safari 14+, Edge 90+ (WebView2 backends)
- [ ] Compatible with React 17+, React 18+
- [ ] Tested in Reaper, Ableton Live, FL Studio, Logic Pro, Bitwig
- [ ] Windows 10/11, macOS 11+, Linux with WebKitGTK 2.40+

### Accessibility
- [ ] WCAG 2.1 AA compliance for color contrast
- [ ] Keyboard navigation: Tab to focus, Arrow keys to adjust, Enter to input value
- [ ] Screen reader announces parameter name, value, and range
- [ ] Focus visible indicator with 3:1 contrast ratio
- [ ] Touch targets minimum 44x44px

### Reliability
- [ ] Zero audio glitches caused by slider interaction
- [ ] Graceful degradation if WebView communication fails
- [ ] Parameter values persist correctly on save/load
- [ ] No memory leaks over 1000+ parameter changes
- [ ] Thread-safe parameter access from audio and UI threads

### Maintainability
- [ ] Component API documented with JSDoc/TypeScript types
- [ ] Unit tests cover all interaction patterns (>90% coverage)
- [ ] Integration tests verify APVTS synchronization
- [ ] Visual regression tests for styling consistency
- [ ] Example usage in Storybook or similar
```

### 7. Acceptance Criteria

For each user story or major requirement, provide acceptance criteria.

**Format Options:**
- Checklist format
- Given/When/Then format
- Scenario-based format

**Example:**
```markdown
## Acceptance Criteria

### Criterion 1: Basic Parameter Control
**Given** a plugin with a cutoff parameter connected to a WebView slider
**When** I drag the slider from minimum to maximum
**Then** the audio cutoff parameter changes from 20Hz to 20kHz in real-time
**And** I hear the filter sweep smoothly with no audio glitches
**And** the slider visual updates at 60fps

### Criterion 2: DAW Automation Recording
**Given** a plugin with automation-enabled parameters
**When** I start recording automation in my DAW
**And** I adjust the slider
**Then** the DAW records the parameter changes
**And** playback reproduces the exact movement
**And** the slider position matches the recorded automation

### Criterion 3: Designer Customization
**Given** a designer wants to customize the slider appearance
**When** they modify the CSS classes for track, thumb, and fill
**Then** the visual appearance updates in real-time (with hot reload)
**And** no C++ recompilation is required
**And** the functionality remains unchanged

### Criterion 4: Performance Under Load
**Given** a plugin with 20 active sliders
**When** I adjust multiple sliders simultaneously
**Then** each slider maintains <3ms parameter latency
**And** the UI maintains 60fps frame rate
**And** CPU usage remains under 10% total

### Criterion 5: Accessibility
**Given** a keyboard-only user
**When** they Tab to a slider and use Arrow keys
**Then** they can adjust the parameter with fine control
**And** screen readers announce the parameter name and value
**And** visual focus indicator is clearly visible

### Criterion 6: Error Resilience
**Given** a slider configured with an invalid parameter ID
**When** the component initializes
**Then** an error is logged to the console
**And** the slider displays in a disabled state
**And** no crash or undefined behavior occurs
**And** other sliders continue to function normally
```

### 8. Out of Scope

Clearly list what is NOT included in this feature.

**Example:**
```markdown
## Out of Scope

**Not Included in This Feature:**
- [ ] 2D X-Y pad control (separate feature)
- [ ] Knob-style rotary control (separate feature)
- [ ] Multi-slider gang control (future enhancement)
- [ ] Slider modulation visualization (separate feature)
- [ ] MIDI velocity response (not applicable to sliders)
- [ ] Built-in preset management (separate system)
- [ ] Slider automation curve editing (DAW feature)
- [ ] Network-based remote control (future epic)
- [ ] Custom easing curves for automation (use DAW's)
- [ ] A/B parameter comparison (separate feature)
```

## Feature Types for VST Plugins

### UI Component Features
- Sliders, knobs, buttons, toggles
- Meters, visualizations, graphs
- Preset browsers, file pickers
- Custom controls and widgets

### Audio Processing Features
- Filters, EQs, dynamics processors
- FFT analysis, spectral processing
- Modulation sources and targets
- MIDI processing and generation

### Integration Features
- Parameter automation and mapping
- Preset save/load systems
- MIDI learn and CC mapping
- DAW-specific optimizations

### Infrastructure Features
- State management and persistence
- Threading and synchronization
- Performance optimization
- Testing and debugging tools

## Workflow

### Step 1: Understand Parent Epic

Review the parent Epic PRD to understand:
- Overall goals and vision
- User personas
- Success metrics
- Constraints and requirements

### Step 2: Gather Feature Details

Ask the user for:
- **Feature Idea**: Specific capability to build
- **User Stories**: Who needs this and why
- **Technical Approach**: High-level implementation strategy
- **Dependencies**: What must exist first
- **Success Metrics**: How to measure success

### Step 3: Define Scope

Determine:
- What's included in this feature
- What's explicitly out of scope
- How this feature fits in the epic
- What features depend on this

### Step 4: Specify Requirements

Create detailed requirements covering:
- Functional behavior (what it does)
- Non-functional qualities (how well it performs)
- User interactions and workflows
- Error cases and edge conditions
- Performance targets and constraints

### Step 5: Write Acceptance Criteria

For each user story, define:
- Given: Initial conditions
- When: User action or trigger
- Then: Expected outcome
- And: Additional expectations

### Step 6: Save and Link

Save PRD to `/docs/ways-of-work/plan/{epic-name}/{feature-name}/prd.md` and:
- Link to parent Epic
- Link to related features
- Update documentation index
- Create implementation plan directory

## Best Practices

1. **Single Responsibility**: Each feature should do one thing well
2. **Testable Requirements**: Every requirement should be verifiable
3. **Performance Targets**: Include specific latency/FPS/CPU numbers
4. **Error Handling**: Specify behavior for edge cases
5. **Accessibility First**: Include keyboard and screen reader support
6. **Cross-Platform**: Consider Windows, macOS, and Linux differences
7. **DAW Context**: Think about plugin in real DAW workflows
8. **User-Centric**: Write from user's perspective, not implementation
9. **Linked Documentation**: Reference parent Epic and related features
10. **Future-Proof**: Note future enhancements in out-of-scope

## VST Plugin-Specific Considerations

### Real-Time Safety
- Specify thread-safety requirements
- Define lock-free communication patterns
- Set latency and blocking time limits
- Consider audio thread priority

### DAW Compatibility
- Test in multiple DAWs
- Handle DAW-specific quirks
- Verify automation behavior
- Check state persistence

### Parameter Management
- Use APVTS for all parameters
- Implement gesture management
- Support undo/redo
- Enable parameter learn

### WebView Integration
- Define JavaScript API contracts
- Specify event communication patterns
- Set performance budgets
- Plan for hot reload support

### Build and Distribution
- Consider bundle size impact
- Plan asset compilation
- Address platform-specific builds
- Think about code signing

## Validation Checklist

Before finalizing a Feature PRD:
- [ ] Feature name is clear and specific
- [ ] Parent Epic is linked
- [ ] Problem statement shows user impact
- [ ] Solution is technically feasible
- [ ] All user personas are addressed
- [ ] User stories follow As a/I want to/So that format
- [ ] Functional requirements are complete and testable
- [ ] Non-functional requirements include performance targets
- [ ] Acceptance criteria use Given/When/Then or equivalent
- [ ] Out of scope prevents feature creep
- [ ] Dependencies are identified
- [ ] Success metrics are measurable
- [ ] JUCE/VST3 constraints are considered
- [ ] Cross-platform issues are addressed

## Example Workflow

### Input from User
```
I want to add an FFT spectrum analyzer to the plugin UI that shows frequency content in real-time.
```

### Agent Questions
```
1. What visual style do you want? (Bars, filled curve, line graph?)
2. How many FFT bins? (Resolution vs. performance trade-off)
3. What update rate? (30fps, 60fps?)
4. Should it show input, output, or both?
5. Any special features? (Peak hold, averaging, freeze?)
6. Which Epic does this belong to? (WebView UI System?)
```

### Generated PRD
Creates comprehensive PRD at:
```
/docs/ways-of-work/plan/webview-ui/spectrum-analyzer/prd.md
```

With sections covering:
- Problem: Users can't visualize frequency content
- Solution: Real-time FFT analyzer with WebGL rendering
- User Stories: For designers, developers, and end users
- Requirements: FFT processing, WebGL rendering, parameter hooks
- Acceptance Criteria: Visual accuracy, performance, responsiveness
- Out of Scope: Spectrogram, sonogram, other visualizations

---

**Usage**: Provide this agent with a feature idea and parent Epic context, and it will generate a detailed Feature PRD ready for technical specification and implementation planning.
