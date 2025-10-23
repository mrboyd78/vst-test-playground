---
mode: 'agent'
description: 'Create GitHub Issues from implementation plan phases using feature_request.yml, bug_report.yml, or chore_request.yml templates.'
tools: ['codebase', 'search', 'github', 'create_issue', 'search_issues', 'update_issue']
---

# Create GitHub Issues from Implementation Plan

Create GitHub Issues for the implementation plan at a specified file in the VST Test Playground repository.

## Purpose

This agent helps you translate implementation plans into actionable GitHub issues, ensuring structured project management and clear task tracking for JUCE-based VST3 plugin development.

## Process

1. **Analyze Implementation Plan**: Read and parse the implementation plan document
2. **Identify Phases**: Break down the plan into distinct implementation phases or tasks
3. **Check Existing Issues**: Use `search_issues` to verify if issues already exist for these phases
4. **Create/Update Issues**: 
   - Create new issues using `create_issue` with appropriate templates
   - Update existing issues using `update_issue` if they need refinement
5. **Apply Labels**: Use appropriate labels (feature, chore, bug, testing, etc.)

## Requirements

### Implementation Plan Structure

The implementation plan should be located in one of these locations:
- `/docs/ways-of-work/plan/{epic-name}/implementation-plan.md`
- `/docs/implementation-history/IMPLEMENTATION_PLAN.md`
- Custom location specified by user

### Issue Templates

Use the appropriate template based on the task type:
- **Feature Request**: `.github/ISSUE_TEMPLATE/feature_request.yml` - For new features or enhancements
- **Chore Request**: `.github/ISSUE_TEMPLATE/chore_request.yml` - For refactoring, technical debt, or maintenance
- **Bug Report**: `.github/ISSUE_TEMPLATE/bug_report.yml` - For fixing identified issues

## Issue Creation Guidelines

### One Issue Per Phase

Create a single, focused issue for each implementation phase with:
- **Clear Title**: Phase name from the implementation plan
- **Detailed Description**: Phase details, requirements, and context
- **Acceptance Criteria**: Measurable completion criteria
- **Component Labels**: Appropriate labels for the affected components

### VST-Specific Context

Include relevant plugin development context:
- **Audio Processing**: For DSP, real-time processing, or audio threading
- **GUI/Editor**: For UI components or PluginEditor changes
- **WebView**: For WebView2 integration features
- **Parameters**: For parameter management or APVTS
- **Build System**: For CMake or JUCE configuration changes
- **Testing**: For test infrastructure or test cases

### Issue Content Template

```markdown
# [Phase Name]

## Epic
[Link to parent epic or implementation plan]

## Description
[Detailed description of what needs to be implemented]

## Technical Requirements
- [ ] Requirement 1
- [ ] Requirement 2
- [ ] Requirement 3

## Acceptance Criteria
- [ ] Criterion 1: [Specific, measurable outcome]
- [ ] Criterion 2: [Specific, measurable outcome]
- [ ] All tests pass
- [ ] Code reviewed and approved
- [ ] Documentation updated

## Related Components
- PluginProcessor: [Changes needed]
- PluginEditor: [Changes needed]
- WebView: [Changes needed]
- Tests: [New test cases]

## Dependencies
- [ ] Dependency 1 (Issue #XX)
- [ ] Dependency 2 (Issue #XX)

## Implementation Notes
[Technical notes, architecture decisions, or JUCE-specific considerations]

## Testing Strategy
- Unit tests: [What to test]
- Integration tests: [What to test]
- Manual validation: [How to verify]
```

## Workflow Steps

### Step 1: Read Implementation Plan

```javascript
// Use codebase tool to read the implementation plan
const plan = await readFile('docs/implementation-history/IMPLEMENTATION_PLAN.md');
```

### Step 2: Parse Plan Structure

Extract phases with their:
- Phase names/titles
- Descriptions
- Requirements
- Dependencies
- Acceptance criteria

### Step 3: Search for Existing Issues

```javascript
// Check if issues already exist
const existingIssues = await search_issues({
  query: 'is:issue label:feature label:chore [phase-keyword]'
});
```

### Step 4: Create Issues

For each phase without an existing issue:

```javascript
await create_issue({
  title: `[Feature]: ${phaseName}`,
  body: issueContent,
  labels: ['feature', 'audio-processing'], // Or appropriate labels
  assignees: [], // Optional
  milestone: 'v1.0.0' // Optional
});
```

### Step 5: Link Dependencies

When creating issues, reference dependencies:
- "Blocked by #XX"
- "Depends on #XX"
- "Related to #XX"

## Labels System

### Primary Labels
- `feature`: New functionality
- `chore`: Maintenance or refactoring
- `bug`: Bug fixes
- `enhancement`: Improvements to existing features

### Component Labels
- `audio-processing`: DSP or PluginProcessor changes
- `gui`: UI or PluginEditor changes
- `webview`: WebView2 integration
- `parameters`: Parameter management
- `build-system`: CMake or build configuration
- `testing`: Test infrastructure or test cases
- `documentation`: Documentation updates

### Priority Labels
- `priority:critical`: Critical for release
- `priority:high`: Important but not blocking
- `priority:medium`: Standard priority
- `priority:low`: Nice to have

### Status Labels
- `status:ready`: Ready to be worked on
- `status:in-progress`: Currently being worked on
- `status:blocked`: Blocked by dependencies
- `status:review`: In code review

## Example Usage

### Example 1: WebView Integration Plan

**Implementation Plan Phase:**
```markdown
### Phase 2: WebView Parameter Synchronization
Implement bidirectional parameter communication between WebView UI and JUCE processor.

Requirements:
- WebSliderRelay setup for each parameter
- JavaScript event listeners for parameter changes
- Gesture management for DAW automation
- Real-time update throttling for performance
```

**Generated Issue:**
```markdown
Title: [Feature]: WebView Parameter Synchronization

## Description
Implement bidirectional parameter communication between the WebView-based UI and the JUCE audio processor, enabling real-time parameter updates and proper DAW automation support.

## Technical Requirements
- [ ] Create WebSliderRelay for each APVTS parameter
- [ ] Implement JavaScript event listeners for parameter changes
- [ ] Add gesture management (sliderDragStarted/Ended)
- [ ] Implement update throttling (60fps max)
- [ ] Add error handling for WebView communication

## Acceptance Criteria
- [ ] Parameters update in real-time from UI to processor
- [ ] DAW automation properly records parameter changes
- [ ] UI reflects automation playback correctly
- [ ] No audio glitches during parameter changes
- [ ] Performance maintains 60fps UI updates
- [ ] All unit tests pass
- [ ] Integration tests cover parameter workflows

## Related Components
- PluginProcessor: Parameter value tree state management
- PluginEditor: WebBrowserComponent integration
- WebGUI: JavaScript parameter bridge implementation

## Dependencies
- [ ] #XX - Basic WebView integration complete
- [ ] #XX - APVTS parameter setup complete

## Testing Strategy
- Unit: Test WebSliderRelay creation and parameter binding
- Integration: Test full parameter round-trip (UI → Processor → UI)
- Manual: Test automation recording in Reaper/Ableton
```

Labels: `feature`, `webview`, `parameters`, `priority:high`

### Example 2: Build System Improvements

**Implementation Plan Phase:**
```markdown
### Phase 1: Modernize CMake Configuration
Update CMake to use JUCE 8 best practices and improve cross-platform builds.

Requirements:
- Update to CMake 3.25+
- Use JUCE's juce_add_plugin properly
- Add static WebView2 linking option
- Improve dependency management
```

**Generated Issue:**
```markdown
Title: [Chore]: Modernize CMake Configuration

## Description
Update the CMake build system to follow JUCE 8 best practices, improve cross-platform compatibility, and add static WebView2 linking for Windows deployments.

## Scope of Work
- CMakeLists.txt: Main build configuration
- README.md: Update build instructions
- .github/workflows/build.yml: Update CI/CD configuration

## Acceptance Criteria
- [ ] CMake minimum version updated to 3.25
- [ ] juce_add_plugin configured with modern options
- [ ] Static WebView2 linking works on Windows
- [ ] All platforms build successfully (Windows, macOS, Linux)
- [ ] CI/CD pipeline passes
- [ ] Build documentation updated

## Risks and Considerations
- May require developers to update their CMake version
- Static WebView2 linking increases binary size
- Need to test on all platforms before merging

Labels: `chore`, `build-system`, `priority:medium`
```

## Best Practices

1. **Keep Issues Focused**: Each issue should represent a single, cohesive unit of work (typically 1-5 days)
2. **Clear Acceptance Criteria**: Every issue must have measurable completion criteria
3. **Link Dependencies**: Always link to blocking or related issues
4. **Update Existing Issues**: If an issue already exists, update it rather than creating duplicates
5. **Use Appropriate Templates**: Choose the right template (feature/chore/bug) for each issue
6. **Include Testing**: Every issue should specify how to test/verify the implementation
7. **JUCE Context**: Include JUCE-specific considerations (threading, real-time safety, etc.)

## Validation Checklist

Before creating each issue, verify:
- [ ] Issue title is clear and descriptive
- [ ] Description provides sufficient context
- [ ] Acceptance criteria are measurable
- [ ] Appropriate template is used
- [ ] Correct labels are applied
- [ ] Dependencies are identified and linked
- [ ] Testing strategy is defined
- [ ] No duplicate issues exist

## Success Metrics

A successful issue creation session should result in:
- **Clear Roadmap**: All implementation phases have corresponding issues
- **Proper Prioritization**: Labels correctly indicate priority and status
- **Linked Dependencies**: Issues reference their dependencies
- **Actionable Tasks**: Each issue can be picked up and worked on immediately
- **Comprehensive Testing**: Testing strategy is defined for each issue

## Notes for VST Plugin Development

When creating issues for VST plugin development, consider:

### Audio Thread Safety
- Flag any work that touches the audio processing thread
- Require real-time safety analysis for DSP changes
- Mandate lock-free implementation for processBlock changes

### Parameter Management
- Ensure APVTS is used for all parameters
- Verify automation compatibility requirements
- Include gesture management in acceptance criteria

### WebView Integration
- Consider WebView2 availability and fallbacks
- Account for thread synchronization between UI and audio
- Include hot-reload testing for WebView changes

### Build and Distribution
- Consider code signing requirements
- Account for platform-specific dependencies
- Include installer/packaging considerations

### Testing Requirements
- Unit tests for business logic
- Integration tests for plugin workflows
- Manual DAW testing procedures
- Performance benchmarking where relevant

---

**Usage**: Simply point this agent at your implementation plan file, and it will generate structured GitHub issues ready for your development workflow.
