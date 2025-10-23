# GitHub Copilot Agent Prompts

This directory contains GitHub Copilot agent prompts designed to enhance development workflows for the VST Test Playground project.

## Available Prompts

### Project Management Prompts

#### 1. Create GitHub Issues from Implementation Plan
**File**: `create-github-issues-feature-from-implementation-plan.prompt.md`

**Purpose**: Automatically generate structured GitHub issues from implementation plans.

**Usage**: Point the agent at an implementation plan document, and it will create appropriately templated issues (feature/chore/bug) with proper labels, acceptance criteria, and dependencies.

**When to Use**:
- Starting a new epic or feature
- Breaking down implementation plans into actionable tasks
- Ensuring consistent issue quality and structure

#### 2. Git Flow Branch Creator
**File**: `git-flow-branch-creator.prompt.md`

**Purpose**: Intelligently analyze git changes and create appropriate Git Flow branches.

**Usage**: Run when you have changes and need to create a new branch. The agent analyzes your diffs and determines whether to create a feature, release, or hotfix branch.

**When to Use**:
- Before starting work on new features
- When preparing releases
- When fixing critical production bugs
- When unsure which branch type to use

### Requirements & Planning Prompts

#### 3. Epic PRD Generator
**File**: `breakdown-epic-pm.prompt.md`

**Purpose**: Create comprehensive Epic-level Product Requirements Documents.

**Usage**: Provide a high-level epic idea, and the agent generates a detailed PRD covering problem statement, solution, user personas, requirements, and success metrics.

**When to Use**:
- Planning new major features or capabilities
- Quarterly or annual roadmap planning
- Seeking stakeholder alignment on large initiatives

#### 4. Feature PRD Generator
**File**: `breakdown-feature-prd.prompt.md`

**Purpose**: Break down epics into detailed feature-level PRDs.

**Usage**: Reference a parent epic and provide a feature idea. The agent creates a detailed PRD with user stories, requirements, acceptance criteria, and implementation considerations.

**When to Use**:
- Implementing specific features within an epic
- Defining precise requirements for development
- Creating technical specifications

### Testing & QA Prompts

#### 5. Test Planning & QA Strategy
**File**: `breakdown-test.prompt.md`

**Purpose**: Generate comprehensive test strategies and quality assurance plans for VST plugin features.

**Usage**: Reference a feature PRD and implementation plan. The agent creates test strategies, test cases, QA plans, and test issue templates.

**When to Use**:
- Before implementing new features
- Planning release testing
- Establishing quality gates
- Creating test automation strategies

## Directory Structure

```
.github/
├── ISSUE_TEMPLATE/          # GitHub issue templates
│   ├── feature_request.yml  # Template for feature requests
│   ├── bug_report.yml       # Template for bug reports
│   └── chore_request.yml    # Template for chores/maintenance
├── copilot-prompts/         # This directory
│   ├── README.md           # This file
│   ├── create-github-issues-feature-from-implementation-plan.prompt.md
│   ├── git-flow-branch-creator.prompt.md
│   ├── breakdown-epic-pm.prompt.md
│   ├── breakdown-feature-prd.prompt.md
│   └── breakdown-test.prompt.md
└── workflows/              # GitHub Actions workflows
    └── build.yml          # CI/CD build workflow
```

## How to Use Copilot Agent Prompts

### In GitHub Copilot Chat

1. **Reference the prompt**: Mention the prompt file in your chat
   ```
   @workspace Use the git-flow-branch-creator prompt to create a branch for my changes
   ```

2. **Provide context**: Include relevant files or information
   ```
   @workspace Using breakdown-feature-prd.prompt.md, create a PRD for a spectrum analyzer feature
   ```

3. **Follow agent guidance**: The agent will ask clarifying questions if needed

### In VS Code

1. Open GitHub Copilot Chat (Ctrl+Alt+I / Cmd+Alt+I)
2. Reference the prompt file from `.github/copilot-prompts/`
3. The agent will guide you through the workflow

### Via CLI

If using GitHub CLI with Copilot:
```bash
gh copilot explain "Use the create-github-issues prompt on docs/implementation-history/IMPLEMENTATION_PLAN.md"
```

## Prompt Customization

These prompts are tailored for VST plugin development with JUCE. They include:

- **Audio-specific considerations**: Real-time safety, threading, DSP
- **JUCE framework patterns**: APVTS, processBlock, AudioProcessor
- **WebView integration**: Parameter bridges, UI communication
- **DAW compatibility**: Cross-DAW testing, automation, state persistence
- **Plugin-specific testing**: Real-time safety, performance profiling

### Adapting Prompts

To customize for your needs:

1. Copy the relevant prompt file
2. Modify the examples and templates
3. Adjust quality criteria and requirements
4. Update file paths and directory structures
5. Save with a new name or replace the original

## Workflow Examples

### Example 1: Starting a New Feature

1. **Create Epic PRD**: Use `breakdown-epic-pm.prompt.md`
   ```
   @workspace Create an Epic PRD for a multi-band compressor using breakdown-epic-pm.prompt.md
   ```

2. **Create Feature PRDs**: Use `breakdown-feature-prd.prompt.md`
   ```
   @workspace Create feature PRDs for the multi-band compressor epic
   ```

3. **Create Test Strategy**: Use `breakdown-test.prompt.md`
   ```
   @workspace Generate test strategy for the compressor feature
   ```

4. **Create Issues**: Use `create-github-issues-feature-from-implementation-plan.prompt.md`
   ```
   @workspace Create GitHub issues from the implementation plan
   ```

5. **Create Branch**: Use `git-flow-branch-creator.prompt.md`
   ```
   @workspace Create a feature branch for my changes
   ```

### Example 2: Bug Fix Workflow

1. **Analyze Changes**: Use `git-flow-branch-creator.prompt.md`
   ```
   @workspace Analyze my changes and suggest a branch name
   ```
   → Creates `hotfix/1.2.1-audio-glitch` if critical

2. **Create Bug Report Issue**: Manually create using `.github/ISSUE_TEMPLATE/bug_report.yml`

3. **Fix and Test**: Implement fix with proper tests

4. **Create PR**: Reference issue in PR description

### Example 3: Release Preparation

1. **Create Release Branch**: Use `git-flow-branch-creator.prompt.md`
   ```
   @workspace Create release branch for version 1.2.0
   ```
   → Creates `release/1.2.0`

2. **Generate Test Plan**: Use `breakdown-test.prompt.md`
   ```
   @workspace Create release testing plan
   ```

3. **Create Release Issues**: Use `create-github-issues-feature-from-implementation-plan.prompt.md`

## Best Practices

### For Project Managers

1. **Start with Epics**: Use Epic PRD generator first for major initiatives
2. **Break Down Features**: Create feature PRDs for each implementable unit
3. **Track with Issues**: Convert plans to issues for visibility
4. **Update Regularly**: Keep PRDs and issues in sync with implementation

### For Developers

1. **Use Git Flow**: Let the branch creator guide branch naming
2. **Reference PRDs**: Link code changes back to requirements
3. **Test Thoroughly**: Use test strategy prompts to ensure coverage
4. **Document Changes**: Update docs as you implement

### For QA Engineers

1. **Test Strategy First**: Create comprehensive test plans before coding
2. **Real-Time Safety**: Always test audio thread safety for DSP changes
3. **DAW Compatibility**: Test in multiple DAWs (Reaper, Ableton, Logic, etc.)
4. **Performance Profiling**: Measure CPU, latency, and memory usage

## Integration with Issue Templates

The prompts work seamlessly with the issue templates in `.github/ISSUE_TEMPLATE/`:

- **feature_request.yml**: Used when creating feature issues
- **bug_report.yml**: Used for bug/hotfix issues
- **chore_request.yml**: Used for maintenance/refactoring tasks

The `create-github-issues` prompt automatically selects the appropriate template based on the issue type.

## VST Plugin Development Specifics

These prompts include special considerations for audio plugin development:

### Audio Thread Safety
- Prompts remind about no allocations/locks in `processBlock()`
- Test strategies include real-time safety validation
- Issue templates include audio thread impact assessment

### JUCE Framework Patterns
- APVTS for parameter management
- WebSliderRelay for WebView integration
- Proper gesture management for automation
- Thread-safe communication patterns

### DAW Compatibility
- Cross-DAW testing requirements
- Automation recording/playback validation
- State persistence across sessions
- Plugin scanning and validation

### Performance Requirements
- CPU usage targets (<15% typical)
- Latency targets (<3ms parameter, <5ms audio)
- UI frame rate (60fps)
- Memory footprint monitoring

## Troubleshooting

### Prompt Not Working?

1. **Check file path**: Ensure you're referencing the correct path
2. **Provide context**: Include relevant files or information
3. **Be specific**: Clearly state what you want the agent to do
4. **Review output**: The agent may ask clarifying questions

### Generated Output Incomplete?

1. **Provide more context**: Reference related PRDs or documentation
2. **Answer questions**: The agent may need clarification
3. **Iterate**: Refine the output through follow-up prompts

### Customization Needed?

1. **Edit prompts**: Modify templates and examples in the prompt files
2. **Add examples**: Include project-specific examples
3. **Update requirements**: Adjust quality criteria and targets

## Contributing

When adding or modifying prompts:

1. **Follow structure**: Use the existing format and YAML frontmatter
2. **Include examples**: Provide VST plugin-specific examples
3. **Test thoroughly**: Verify prompts work with GitHub Copilot
4. **Update README**: Document new prompts here
5. **Link templates**: Ensure integration with issue templates

## Additional Resources

- **GitHub Copilot Documentation**: https://docs.github.com/en/copilot
- **awesome-copilot Repository**: https://github.com/github/awesome-copilot
- **JUCE Framework**: https://juce.com/
- **VST3 SDK**: https://steinbergmedia.github.io/vst3_doc/

---

**Last Updated**: October 2025  
**Maintainer**: VST Test Playground Team
