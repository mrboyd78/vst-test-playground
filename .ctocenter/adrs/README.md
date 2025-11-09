# Architecture Decision Records (ADRs)

This directory contains Architecture Decision Records documenting significant technical decisions made in this project.

## Format

ADRs follow the naming convention: `ADR-NNNN-title-in-kebab-case.md`

- NNNN: Four-digit sequential number (0001, 0002, etc.)
- Title: Brief, descriptive title in kebab-case

## Status

ADRs can have the following statuses:
- **Proposed**: Under consideration
- **Accepted**: Decision approved and implemented
- **Deprecated**: Decision no longer recommended (but may still be in use)
- **Superseded**: Replaced by a newer ADR

## Template

Use the template at `.ctocenter/templates/ADR.template.md` to create new ADRs.

## Creating a New ADR

1. Determine the next sequential number
2. Copy the template: `cp .ctocenter/templates/ADR.template.md .ctocenter/adrs/ADR-NNNN-your-title.md`
3. Fill in all sections
4. Submit for review via pull request
5. Update status as decision progresses

## Validation

ADRs are automatically validated by CI to ensure:
- Sequential numbering (no gaps or duplicates)
- Valid status values
- Required sections present
- Proper superseding links

Run validation locally:
```bash
cd .ctocenter/scripts
ts-node adr-validate.ts
```

## Index

<!-- This section will be populated as ADRs are created -->

_No ADRs created yet. Start by creating ADR-0001!_

## Guidelines

### When to Create an ADR

Create an ADR when making decisions about:
- System architecture
- Technology selection
- Design patterns
- Build/deployment approach
- Security architecture
- Performance optimizations
- API design

### What to Include

**Context**: Why are we making this decision? What forces are at play?

**Decision**: What have we decided to do?

**Consequences**: What are the implications of this decision?

### Status Transitions

Valid transitions:
- Proposed → Accepted
- Proposed → Deprecated
- Accepted → Deprecated
- Accepted → Superseded

### Superseding ADRs

When superseding an ADR:
1. Create new ADR with updated decision
2. Add "Supersedes: ADR-XXXX" to new ADR
3. Update old ADR status to "Superseded"
4. Add "Superseded By: ADR-YYYY" to old ADR

## Resources

- [ADR Template](../.ctocenter/templates/ADR.template.md)
- [Usage Guide](../../docs/cto/USAGE.md)
- [GitHub Copilot Prompt for ADRs](https://adr.github.io/)

---

**Questions?** See [CTO Command Center Documentation](../../docs/cto/)
