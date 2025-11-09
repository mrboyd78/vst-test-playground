# User Prompt: Task Writer

## Objective

Convert high-level objectives, epics, or feature requests into structured, actionable tasks that conform to the task specification schema.

## Your Mission

Take a strategic goal or requirement and break it down into:
- **Specific** tasks that can be assigned and tracked
- **Measurable** acceptance criteria
- **Actionable** steps with clear ownership
- **Realistic** estimates and dependencies
- **Time-bound** milestones

## Input Format

Provide me with:

```markdown
**Objective:** [High-level goal or epic]

**Context:** [Background information, constraints, requirements]

**Success Looks Like:** [Desired outcome]

**Constraints:**
- [Technical constraint]
- [Time constraint]
- [Resource constraint]
```

## Output Format

I will generate tasks as JSON conforming to this schema:

```json
{
  "id": "task-YYYY-MM-DD-NNN",
  "title": "Clear, action-oriented title (max 100 chars)",
  "description": "Detailed description of what needs to be done",
  "type": "feature|bug|chore|docs|test|refactor",
  "priority": "critical|high|medium|low",
  "estimatedHours": 0,
  "dependencies": ["task-id-1", "task-id-2"],
  "acceptanceCriteria": [
    "Specific, testable criterion 1",
    "Specific, testable criterion 2"
  ],
  "technicalNotes": "Implementation guidance, gotchas, references",
  "relatedDocuments": [
    {
      "type": "adr|playbook|runbook|reference",
      "path": "path/to/document.md",
      "reason": "Why this document is relevant"
    }
  ],
  "tags": ["frontend", "backend", "infrastructure", "security"],
  "assignee": "Optional: team or person",
  "milestone": "Optional: release or sprint",
  "createdAt": "YYYY-MM-DDTHH:mm:ss.sssZ",
  "updatedAt": "YYYY-MM-DDTHH:mm:ss.sssZ"
}
```

## Task Breakdown Strategy

### 1. Analyze the Objective

- What is the core problem being solved?
- Who are the stakeholders?
- What are the technical domains involved?

### 2. Identify Work Streams

Break into parallel tracks:
- **Frontend:** UI/UX changes
- **Backend:** API, business logic, data
- **Infrastructure:** Deployment, scaling, monitoring
- **Documentation:** User guides, API docs, ADRs
- **Testing:** Unit, integration, E2E tests

### 3. Define Dependencies

- What must be done first?
- What can be parallelized?
- What blocks other work?

### 4. Estimate Complexity

For each task, consider:
- **Simple (1-4 hours):** Well-defined, minimal unknowns
- **Medium (4-16 hours):** Some complexity or research needed
- **Complex (16-40 hours):** Significant unknowns, multiple systems
- **Epic (40+ hours):** Break down further

### 5. Write Acceptance Criteria

Each criterion should:
- Be testable (manual or automated)
- Be specific (no ambiguity)
- Be complete (covers all aspects)
- Be achievable (within task scope)

## Example Interaction

**User provides:**

```markdown
**Objective:** Add dark mode support to the application

**Context:** 
- Users have requested dark mode
- Design team has provided dark mode color palette
- Should respect system preferences
- Must work across all pages

**Success Looks Like:**
- Users can toggle dark mode on/off
- Dark mode persists across sessions
- All UI elements are readable in dark mode
- Performance is not impacted

**Constraints:**
- Must ship in next release (2 weeks)
- Cannot break existing themes
- Must be accessible (WCAG AA)
```

**I generate:**

```json
{
  "tasks": [
    {
      "id": "task-2025-11-09-001",
      "title": "Implement dark mode color system and theme context",
      "description": "Create a theme system that supports light and dark modes using CSS variables. Implement React context for theme state management. Ensure theme persists to localStorage.",
      "type": "feature",
      "priority": "high",
      "estimatedHours": 8,
      "dependencies": [],
      "acceptanceCriteria": [
        "CSS variables defined for both light and dark themes",
        "ThemeContext provides current theme and toggle function",
        "Theme preference persists to localStorage",
        "Theme changes apply globally without page refresh",
        "System preference is detected and used as default"
      ],
      "technicalNotes": "Use CSS custom properties for theme tokens. Consider using prefers-color-scheme media query for initial detection. Store theme in localStorage as 'theme-preference'.",
      "relatedDocuments": [
        {
          "type": "reference",
          "path": "docs/reference/STYLING.md",
          "reason": "Current styling conventions"
        }
      ],
      "tags": ["frontend", "ux"],
      "milestone": "v2.1.0"
    },
    {
      "id": "task-2025-11-09-002",
      "title": "Update all UI components for dark mode compatibility",
      "description": "Audit all React components and update to use theme-aware styles. Replace hardcoded colors with theme variables. Test readability and contrast in both modes.",
      "type": "feature",
      "priority": "high",
      "estimatedHours": 16,
      "dependencies": ["task-2025-11-09-001"],
      "acceptanceCriteria": [
        "All components use theme variables instead of hardcoded colors",
        "Text contrast meets WCAG AA standards in both modes",
        "Images and icons are visible in both modes",
        "Interactive elements have appropriate hover/focus states",
        "No visual regressions in light mode"
      ],
      "technicalNotes": "Priority components: Header, Sidebar, Forms, Modals, Cards. Use a consistent naming convention for theme variables (e.g., --color-background-primary). Test with Chrome DevTools' color contrast checker.",
      "relatedDocuments": [],
      "tags": ["frontend", "accessibility"],
      "milestone": "v2.1.0"
    },
    {
      "id": "task-2025-11-09-003",
      "title": "Create dark mode toggle UI component",
      "description": "Design and implement a toggle switch for users to manually change theme. Place in settings and header. Include smooth transition animations.",
      "type": "feature",
      "priority": "medium",
      "estimatedHours": 4,
      "dependencies": ["task-2025-11-09-001"],
      "acceptanceCriteria": [
        "Toggle switch is accessible (keyboard navigation, screen readers)",
        "Current theme state is visually indicated",
        "Theme changes smoothly with transition animation",
        "Toggle is consistently placed across all views",
        "Icon or label clearly indicates purpose"
      ],
      "technicalNotes": "Consider using a sun/moon icon. Transition duration should be ~200ms. Use aria-label for accessibility.",
      "relatedDocuments": [],
      "tags": ["frontend", "ux", "accessibility"],
      "milestone": "v2.1.0"
    },
    {
      "id": "task-2025-11-09-004",
      "title": "Add dark mode documentation and user guide",
      "description": "Document how to use dark mode feature. Update user manual with screenshots. Document the theming system for developers.",
      "type": "docs",
      "priority": "medium",
      "estimatedHours": 3,
      "dependencies": ["task-2025-11-09-003"],
      "acceptanceCriteria": [
        "User guide includes how to enable/disable dark mode",
        "Screenshots show both light and dark mode",
        "Developer documentation explains theme system",
        "CSS variable naming convention is documented",
        "Accessibility considerations are noted"
      ],
      "technicalNotes": "Update docs/guides/USER_MANUAL.md and create docs/reference/THEMING.md",
      "relatedDocuments": [],
      "tags": ["docs"],
      "milestone": "v2.1.0"
    },
    {
      "id": "task-2025-11-09-005",
      "title": "Test dark mode across browsers and devices",
      "description": "Comprehensive testing of dark mode in different browsers, devices, and with assistive technologies. Fix any compatibility issues.",
      "type": "test",
      "priority": "high",
      "estimatedHours": 6,
      "dependencies": ["task-2025-11-09-002", "task-2025-11-09-003"],
      "acceptanceCriteria": [
        "Dark mode works in Chrome, Firefox, Safari, Edge",
        "Dark mode works on iOS and Android devices",
        "No console errors or warnings",
        "Performance metrics unchanged (Lighthouse score)",
        "Screen readers announce theme changes appropriately"
      ],
      "technicalNotes": "Test matrix: Chrome/Firefox/Safari/Edge on Windows/macOS, Safari on iOS, Chrome on Android. Use BrowserStack if needed. Run Lighthouse audits before/after.",
      "relatedDocuments": [],
      "tags": ["test", "accessibility", "qa"],
      "milestone": "v2.1.0"
    }
  ],
  "summary": {
    "totalTasks": 5,
    "totalEstimatedHours": 37,
    "criticalPath": ["task-2025-11-09-001", "task-2025-11-09-002", "task-2025-11-09-005"],
    "parallelizable": ["task-2025-11-09-003", "task-2025-11-09-004"],
    "risks": [
      "Cross-browser compatibility may require more time than estimated",
      "Image assets may need dark mode variants",
      "Third-party components may not support theming"
    ]
  }
}
```

## Best Practices

### DO ✅

- Break large objectives into tasks <16 hours each
- Include specific acceptance criteria (not "works well")
- Identify dependencies explicitly
- Tag appropriately for filtering
- Reference relevant documentation
- Consider non-functional requirements (performance, security, accessibility)
- Include both implementation and testing tasks
- Estimate realistically (pad for unknowns)

### DON'T ❌

- Create tasks that are too vague ("improve the system")
- Forget about testing and documentation tasks
- Ignore dependencies (causes blocking)
- Over-estimate or under-estimate (be realistic)
- Skip acceptance criteria (how will we know it's done?)
- Forget about edge cases and error handling
- Assume context is obvious (be explicit)

## Validation

Before providing output, verify:

- [ ] Each task has a unique ID
- [ ] All required fields are present
- [ ] Acceptance criteria are specific and testable
- [ ] Dependencies reference valid task IDs
- [ ] Estimated hours are reasonable
- [ ] Type and priority are from allowed values
- [ ] Related documents exist or are planned
- [ ] Tags are relevant and consistent

## Usage

To use this prompt:

1. Provide your objective and context (see Input Format above)
2. I will analyze and break it down
3. Review the generated tasks
4. Request refinements if needed
5. Save the output to `.ctocenter/tasks/YYYY-MM-DD-objective-name.json`
6. Import tasks into your project management tool

---

**Prompt Version:** 1.0.0  
**Last Updated:** 2025-11-09  
**Schema:** `.ctocenter/ai/schemas/task.spec.json`
