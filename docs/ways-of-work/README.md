# Ways of Work - Planning Documentation

This directory contains planning documentation for features and epics, following a structured approach to product development.

## Directory Structure

```
ways-of-work/
└── plan/
    └── {epic-name}/                    # Each epic gets its own directory
        ├── epic.md                     # Epic-level PRD
        ├── architecture.md             # Technical architecture (optional)
        └── {feature-name}/             # Each feature within the epic
            ├── prd.md                  # Feature PRD
            ├── technical-breakdown.md   # Technical specification (optional)
            ├── implementation-plan.md   # Implementation roadmap (optional)
            ├── test-strategy.md        # Test planning (optional)
            ├── test-issues-checklist.md # Test tracking (optional)
            └── qa-plan.md              # Quality assurance (optional)
```

## How to Use This Structure

### Starting a New Epic

1. **Create Epic Directory**:
   ```bash
   mkdir -p docs/ways-of-work/plan/my-epic-name
   ```

2. **Generate Epic PRD**:
   Use the Epic PRD generator prompt in `.github/copilot-prompts/breakdown-epic-pm.prompt.md`

3. **Create Epic PRD File**:
   ```bash
   # Save the generated PRD
   touch docs/ways-of-work/plan/my-epic-name/epic.md
   ```

### Adding a Feature to an Epic

1. **Create Feature Directory**:
   ```bash
   mkdir -p docs/ways-of-work/plan/my-epic-name/my-feature-name
   ```

2. **Generate Feature PRD**:
   Use the Feature PRD generator prompt in `.github/copilot-prompts/breakdown-feature-prd.prompt.md`

3. **Create Feature PRD File**:
   ```bash
   # Save the generated PRD
   touch docs/ways-of-work/plan/my-epic-name/my-feature-name/prd.md
   ```

4. **Add Supporting Documents** (optional):
   - `technical-breakdown.md`: Detailed technical specifications
   - `implementation-plan.md`: Phase-by-phase implementation roadmap
   - `test-strategy.md`: Comprehensive test planning
   - `qa-plan.md`: Quality assurance and validation plan

### Creating GitHub Issues from Plans

Once you have an implementation plan:

1. Use the issue creation prompt: `.github/copilot-prompts/create-github-issues-feature-from-implementation-plan.prompt.md`
2. Point it at your `implementation-plan.md` file
3. It will automatically create structured GitHub issues with proper templates

## Example Epic Structure

Here's an example for a "WebView UI System" epic:

```
ways-of-work/plan/webview-ui-system/
├── epic.md                                      # Epic PRD
├── webview-integration-core/
│   ├── prd.md                                  # Core integration feature PRD
│   ├── technical-breakdown.md                   # Technical specs
│   ├── implementation-plan.md                   # Implementation roadmap
│   └── test-strategy.md                        # Test plan
├── parameter-bridge/
│   ├── prd.md                                  # Parameter bridge feature PRD
│   └── implementation-plan.md                   # Implementation roadmap
└── hot-reload-system/
    ├── prd.md                                  # Hot reload feature PRD
    └── test-strategy.md                        # Test plan
```

## Document Templates

### Epic PRD (epic.md)

Generated using `breakdown-epic-pm.prompt.md`, includes:
- Epic name and goal
- Problem statement and solution
- User personas
- High-level user journeys
- Business requirements (functional and non-functional)
- Success metrics
- Out of scope
- Business value

### Feature PRD (prd.md)

Generated using `breakdown-feature-prd.prompt.md`, includes:
- Feature name
- Link to parent epic
- Goal (problem, solution, impact)
- User personas
- User stories
- Requirements (functional and non-functional)
- Acceptance criteria
- Out of scope

### Implementation Plan (implementation-plan.md)

Optionally created manually or with technical team, includes:
- Implementation phases
- Technical tasks per phase
- Dependencies between phases
- Estimated effort
- Risk mitigation

### Test Strategy (test-strategy.md)

Generated using `breakdown-test.prompt.md`, includes:
- Test scope and objectives
- Test types (unit, integration, real-time safety, DAW compatibility)
- Test coverage targets
- Quality metrics
- Test environment requirements

## Best Practices

1. **Start with Epic**: Always create an epic-level PRD before diving into features
2. **Link Documents**: Each feature PRD should link back to its parent epic
3. **Keep Updated**: Update PRDs as requirements evolve during implementation
4. **Use Prompts**: Leverage GitHub Copilot prompts for consistent documentation
5. **Version Control**: All documentation is version controlled alongside code
6. **Review Process**: PRDs should be reviewed by stakeholders before implementation
7. **Track Issues**: Link GitHub issues back to their source PRDs

## Naming Conventions

### Directory Names
- Use lowercase with hyphens: `webview-ui-system`, `spectrum-analyzer`
- Keep names concise but descriptive
- Avoid special characters except hyphens

### File Names
- Use standard names: `epic.md`, `prd.md`, `implementation-plan.md`
- Keep consistent across all features
- Use `.md` extension for Markdown files

## Integration with Development Workflow

This structure integrates with:

- **GitHub Issues**: Created from implementation plans using prompts
- **GitHub Projects**: Issues organized by epic and feature
- **Git Branches**: Branch names reference feature directories
- **Pull Requests**: PRs link back to PRDs and issues
- **Documentation**: Technical docs reference PRD decisions

## VST Plugin-Specific Considerations

When creating plans for VST plugin features, consider:

### Audio Processing
- Real-time safety requirements
- DSP algorithm specifications
- Performance targets (CPU, latency)
- Thread safety considerations

### User Interface
- WebView integration approach
- Parameter synchronization patterns
- Visual design specifications
- Accessibility requirements

### DAW Integration
- Compatibility matrix (Reaper, Ableton, Logic, etc.)
- Automation behavior
- State persistence
- Plugin format support (VST3, AU, AAX)

### Testing
- Real-time safety validation
- Performance profiling
- Cross-DAW compatibility testing
- Regression testing strategy

## Getting Started

If you're new to this structure:

1. **Read the prompts**: Review `.github/copilot-prompts/README.md`
2. **See examples**: Check `docs/implementation-history/` for past planning docs
3. **Start small**: Begin with a simple feature PRD
4. **Iterate**: Refine your process as you learn what works

## Questions or Feedback?

- Open a [GitHub Discussion](https://github.com/mrboyd78/vst-test-playground/discussions)
- Create an issue using [Chore Request](.github/ISSUE_TEMPLATE/chore_request.yml) template
- Suggest improvements to this structure

---

**Last Updated**: October 2025  
**Maintained by**: VST Test Playground Team
