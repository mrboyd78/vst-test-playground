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
