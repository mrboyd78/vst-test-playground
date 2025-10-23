---
mode: 'agent'
description: 'Epic Product Requirements Document (PRD) generator for VST plugin features, creating detailed specifications for new epics.'
tools: ['codebase', 'search', 'create_file', 'read_file']
---

# Epic Product Requirements Document (PRD) Generator

Generate comprehensive Epic-level Product Requirements Documents (PRDs) for VST Test Playground features. These PRDs serve as the foundation for technical architecture specifications and implementation plans.

## Purpose

This agent helps Product Managers translate high-level ideas into detailed Epic-level PRDs for JUCE-based VST3 plugin development. The output serves as the single source of truth for engineering teams.

## Output Location

The PRD will be saved to:
```
/docs/ways-of-work/plan/{epic-name}/epic.md
```

## PRD Structure

### 1. Epic Name

A clear, concise, and descriptive name for the epic.

**Examples for VST Plugins:**
- "WebView-Based Plugin UI System"
- "Advanced Parameter Automation Framework"
- "Real-Time FFT Visualization Engine"
- "Preset Management and Sharing Platform"
- "Multi-Band Audio Processing Suite"

### 2. Goal

#### Problem
Describe the user problem or business need this epic addresses (3-5 sentences).

**Template:**
```markdown
## Problem
Current VST plugins [describe limitation]. This creates [specific pain point] for [target users]. As a result, [negative outcome]. Competitors [competitive situation]. Users need [specific need].
```

**Example:**
```markdown
## Problem
Current VST plugins rely on native JUCE components for UI, resulting in slow iteration cycles and platform-specific behavior inconsistencies. Designers must wait for C++ recompilation for every visual change, which can take 5-10 minutes per iteration. This significantly slows down UI development and makes rapid A/B testing impractical. Modern web technologies offer 10-100x faster development cycles with hot reloading, but lack integration with JUCE's audio processing. Plugin developers need a way to use modern web UI frameworks while maintaining native audio performance.
```

#### Solution
Explain how this epic solves the problem at a high level.

**Template:**
```markdown
## Solution
Implement [high-level approach] that enables [capability]. This solution will [key benefit] while maintaining [important constraint]. The system will use [technology/pattern] to achieve [outcome].
```

**Example:**
```markdown
## Solution
Implement a WebView2-based UI system that embeds modern web technologies (React/Vue/Svelte) within JUCE's plugin framework. This solution enables rapid UI iteration with hot reloading while maintaining real-time audio processing performance. The system uses a thread-safe bridge between the WebView and audio processor, with sub-5ms parameter latency and 60fps update rates.
```

#### Impact
Expected outcomes and metrics to be improved.

**Metrics for VST Plugins:**
- UI development iteration speed (minutes per change)
- Designer independence (% of UI changes requiring C++ knowledge)
- Cross-platform consistency (% of UI that looks identical across DAWs)
- Parameter latency (milliseconds from UI to audio thread)
- UI frame rate (fps)
- Plugin load time (seconds)
- Binary size (MB)
- User satisfaction score
- Bug density in UI code
- Time to implement new feature (hours)

### 3. User Personas

Describe the target users for this epic.

**Common VST Plugin Personas:**

#### Plugin Developer
- **Role**: Creates VST3 plugins using the framework
- **Goals**: Fast iteration, maintainable code, professional results
- **Pain Points**: Slow compile times, platform quirks, GUI complexity
- **Technical Skills**: C++, audio DSP, JUCE framework knowledge

#### UI/UX Designer
- **Role**: Designs plugin interfaces
- **Goals**: Creative freedom, rapid prototyping, pixel-perfect results
- **Pain Points**: Dependency on developers, slow feedback loops, platform limitations
- **Technical Skills**: HTML/CSS/JS, design tools, limited C++ knowledge

#### End User (Music Producer)
- **Role**: Uses the plugin in a DAW
- **Goals**: Intuitive interface, fast workflow, reliable operation
- **Pain Points**: Confusing UIs, performance issues, crashes
- **Technical Skills**: DAW operation, basic audio engineering, no programming

#### Product Manager
- **Role**: Defines product direction
- **Goals**: User satisfaction, competitive features, on-time delivery
- **Pain Points**: Long development cycles, scope creep, technical debt
- **Technical Skills**: Product strategy, market analysis, basic technical understanding

### 4. High-Level User Journeys

Describe key user journeys and workflows enabled by this epic.

**Template:**
```markdown
## User Journey: [Journey Name]

**Persona**: [Which user persona]

**Scenario**: [Context and goal]

**Steps**:
1. [User action]
2. [System response]
3. [User action]
4. [Outcome]

**Success Criteria**: [What makes this journey successful]
```

**Example:**
```markdown
## User Journey: Rapid UI Iteration During Development

**Persona**: Plugin Developer

**Scenario**: Developer is refining the layout of a filter UI while testing in a DAW

**Steps**:
1. Developer makes CSS changes to the filter panel layout
2. Browser hot-reloads within 500ms, showing updates in DAW
3. Developer tests parameter adjustments in real-time
4. Satisfied with changes, developer commits to version control

**Success Criteria**: 
- Change visibility under 1 second
- No need to recompile C++ code
- No need to restart DAW or reload plugin
- Changes persist across sessions
```

### 5. Business Requirements

#### Functional Requirements

A detailed, bulleted list of what the epic must deliver from a business perspective.

**Categories for VST Plugins:**
- **Audio Processing**: DSP features, real-time requirements, latency targets
- **User Interface**: Visual design, interaction patterns, controls
- **Parameter Management**: Automation, presets, MIDI mapping
- **Integration**: DAW compatibility, plugin formats, APIs
- **Performance**: CPU usage, memory footprint, startup time
- **Quality**: Stability, audio quality, thread safety
- **Workflow**: User efficiency, common task optimization
- **Extensibility**: Plugin architecture, customization points

**Example:**
```markdown
### Functional Requirements

**WebView Integration:**
- [ ] Embed WebView2 (Windows)/WebKit (macOS/Linux) in plugin editor
- [ ] Support React, Vue, and Svelte frameworks
- [ ] Hot reload during development without recompiling C++
- [ ] Production build bundles assets into plugin binary

**Parameter Communication:**
- [ ] Bidirectional parameter sync between WebView and audio processor
- [ ] Sub-5ms latency from UI interaction to parameter change
- [ ] Support for DAW automation recording and playback
- [ ] 60fps update rate for visual parameter feedback

**Development Experience:**
- [ ] npm-based build system for web assets
- [ ] TypeScript support with JUCE type definitions
- [ ] Vite/Webpack build integration
- [ ] Live debugging with browser DevTools
```

#### Non-Functional Requirements

Quality attributes and constraints.

**Categories for VST Plugins:**
- **Performance**: Real-time processing requirements, latency targets
- **Security**: Code signing, sandboxing, vulnerability protection
- **Accessibility**: WCAG compliance, keyboard navigation, screen readers
- **Compatibility**: DAW versions, OS versions, plugin formats
- **Scalability**: Parameter count, preset size, UI complexity
- **Maintainability**: Code quality, documentation, test coverage
- **Reliability**: Crash resistance, data integrity, error recovery
- **Usability**: Learning curve, efficiency, error prevention

**Example:**
```markdown
### Non-Functional Requirements

**Performance:**
- [ ] Audio processing maintains <1ms latency at 64 sample buffer size
- [ ] UI updates at stable 60fps with <16ms frame time
- [ ] Plugin load time <3 seconds including WebView initialization
- [ ] CPU usage <5% idle, <15% under normal use

**Security:**
- [ ] WebView sandbox prevents file system access outside plugin folder
- [ ] Content Security Policy restricts external resource loading
- [ ] No execution of untrusted JavaScript
- [ ] HTTPS only for any network requests

**Compatibility:**
- [ ] Windows 10/11 with WebView2 Runtime
- [ ] macOS 11+ with system WebKit
- [ ] Linux with WebKitGTK 2.40+
- [ ] Works in Reaper, Ableton Live, FL Studio, Logic Pro, Bitwig

**Accessibility:**
- [ ] Keyboard navigation for all controls
- [ ] WCAG 2.1 AA compliance for UI
- [ ] Screen reader compatibility
- [ ] High contrast mode support
```

### 6. Success Metrics

Key Performance Indicators (KPIs) to measure epic success.

**Metric Categories:**
- **Development Velocity**: Time to implement features, iteration speed
- **Quality**: Bug rate, crash rate, user satisfaction
- **Performance**: Latency, CPU usage, memory usage, FPS
- **Adoption**: Usage rate, user retention, feature utilization
- **Business**: Revenue impact, cost savings, market position

**Example:**
```markdown
## Success Metrics

**Development Velocity:**
- 10x faster UI iteration (from 10 minutes to <1 minute per change)
- 50% reduction in UI-related bugs
- 80% of UI changes require no C++ modifications

**Performance:**
- <3ms parameter latency (measure: UI click to processor update)
- 60fps UI frame rate under normal use
- <5% CPU overhead for WebView rendering

**Quality:**
- Zero audio thread blocking from UI operations
- <1 crash per 1000 plugin loads
- User satisfaction score >4.5/5 for UI responsiveness

**Adoption:**
- 100% of new plugins use WebView UI by Q2
- Developer satisfaction >4.0/5 for UI development experience
```

### 7. Out of Scope

Clearly list what is NOT included to avoid scope creep.

**Example:**
```markdown
## Out of Scope

**Not Included in This Epic:**
- [ ] Mobile/iOS plugin support (future epic)
- [ ] Remote UI control over network (future feature)
- [ ] 3D graphics or GPU-accelerated rendering (use native if needed)
- [ ] Video playback in UI (outside plugin scope)
- [ ] Drag-and-drop audio file loading (separate epic)
- [ ] Built-in preset browser UI (comes after parameter system)
- [ ] MIDI visualization (separate feature)
- [ ] Undo/redo for parameter changes (APVTS handles this)
- [ ] Custom WebView versions (use system-provided only)
```

### 8. Business Value

Estimate business value with justification.

**Value Levels:**
- **Critical**: Required for product viability or competitive parity
- **High**: Significant impact on key metrics or user satisfaction
- **Medium**: Meaningful improvement but not differentiating
- **Low**: Nice to have, minor impact

**Example:**
```markdown
## Business Value

**Level**: High

**Justification**:
- **Competitive Advantage**: Modern web-based UIs are becoming table stakes in premium plugin market. Leading competitors (Plugin Alliance, iZotope) are moving to web technologies.
- **Development Efficiency**: 10x faster iteration reduces time-to-market by 30-50% for UI-heavy features, translating to $50K+ annual savings in development costs.
- **Designer Autonomy**: Enables designers to work independently 80% of the time, eliminating bottleneck on C++ developers.
- **User Experience**: Enables sophisticated, responsive UIs that match modern expectations, improving user satisfaction and reducing support burden.
- **Technical Debt**: Modernizes UI stack, making it easier to hire developers familiar with web technologies vs. JUCE/native UI experts.

**ROI Estimate**: 3-6 months based on development time savings and reduced bug rates.
```

## VST Plugin Epic Categories

Common epic categories for VST plugin development:

### Audio Processing Epics
- Filter banks and processing chains
- FFT/spectral processing
- Dynamic range processing
- Modulation systems
- MIDI processing

### User Interface Epics
- WebView integration
- Custom look and feel
- Visual feedback systems
- Preset browsers
- Parameter visualization

### Architecture Epics
- Plugin format support (VST3, AU, AAX)
- State management and persistence
- Threading and real-time safety
- Build system modernization
- Testing infrastructure

### Integration Epics
- DAW-specific features
- External hardware integration
- Network synchronization
- Plugin communication
- Licensing systems

## Example Template Usage

### When to Use This Agent

1. **New Major Feature Area**: Starting work on a substantial new capability
2. **Product Planning**: Quarterly or annual planning cycles
3. **Stakeholder Alignment**: Need clear documentation for technical and business teams
4. **Architecture Planning**: Before creating technical specifications
5. **Scope Definition**: Clarifying boundaries of what will and won't be built

### Prerequisites

Before generating an Epic PRD:
- [ ] Market research completed
- [ ] Competitive analysis available
- [ ] User research or feedback gathered
- [ ] Technical feasibility assessed
- [ ] Stakeholder input collected
- [ ] Success metrics identified

### Review Process

After generating the PRD:
1. **Technical Review**: Engineering team reviews feasibility
2. **Business Review**: Product/business team reviews value and scope
3. **User Review**: Designer/UX reviews user experience aspects
4. **Refinement**: Iterate based on feedback
5. **Approval**: Get sign-off from stakeholders
6. **Handoff**: Use PRD to create technical architecture document

## Workflow

### Step 1: Gather Information

Ask the user for:
- **Epic Idea**: High-level description of what needs to be built
- **Target Users**: Who will use or benefit from this?
- **Problem Context**: What pain points exist today?
- **Success Vision**: What does success look like?
- **Constraints**: Any technical, budget, or timeline constraints?

### Step 2: Research Context

Review existing documentation:
- Current plugin architecture
- Related features or systems
- Previous implementation plans
- Technical debt items
- User feedback or issues

### Step 3: Generate PRD

Create comprehensive PRD following the structure above, with:
- Clear problem and solution statements
- Detailed user personas and journeys
- Comprehensive requirements
- Measurable success metrics
- Explicit scope boundaries

### Step 4: Save and Link

Save PRD to `/docs/ways-of-work/plan/{epic-name}/epic.md` and:
- Create directory structure if needed
- Link to related documentation
- Update docs index
- Notify stakeholders

## Best Practices

1. **Be Specific**: Avoid vague language; use concrete examples
2. **Quantify Impact**: Use numbers for metrics and targets
3. **Consider All Users**: Include developers, designers, and end users
4. **Define Boundaries**: Clear out-of-scope prevents creep
5. **Link to Research**: Reference user studies, competitive analysis
6. **Keep Maintainable**: Update PRD as requirements evolve
7. **Audio First**: Remember real-time constraints and audio quality
8. **Platform Aware**: Consider Windows, macOS, and Linux differences
9. **DAW Context**: Think about plugin in DAW workflow
10. **Performance Minded**: Always include performance requirements

## Validation Checklist

Before finalizing an Epic PRD:
- [ ] Problem clearly stated with user impact
- [ ] Solution approach is well-defined
- [ ] All user personas identified
- [ ] Key user journeys documented
- [ ] Functional requirements are complete and testable
- [ ] Non-functional requirements include performance targets
- [ ] Success metrics are measurable
- [ ] Out of scope is explicitly listed
- [ ] Business value is quantified
- [ ] Technical feasibility confirmed
- [ ] Stakeholder input incorporated
- [ ] Related documentation linked

---

**Usage**: Provide this agent with an epic idea and context, and it will generate a comprehensive Epic PRD ready for technical planning and implementation.
