# CTO Command Center - Glossary

## Core Concepts

### CTO Command Center
A comprehensive framework for maintaining operational excellence, documentation alignment, and engineering productivity through automation, standardized processes, and AI-powered tools.

### Documentation Drift
The divergence between code reality and documentation. Occurs when code changes but documentation remains stale, or when new features lack documentation.

### Alignment
The state where documentation accurately reflects the current codebase, architecture, and operational procedures. Achieved through continuous validation and updates.

## Documents and Artifacts

### ADR (Architecture Decision Record)
A document that captures an important architectural decision along with its context and consequences. ADRs create a historical record of why decisions were made.

**Statuses:**
- **Proposed:** Under consideration
- **Accepted:** Approved and implemented
- **Deprecated:** No longer recommended
- **Superseded:** Replaced by a newer ADR

### Playbook
A document describing a standard operational procedure. Step-by-step instructions for executing routine tasks (deployments, migrations, etc.).

**Example:** Deployment playbook, database migration playbook

### Runbook
A document providing procedures for responding to specific incidents or operational issues. Focus on triage, resolution, and recovery.

**Example:** Service outage runbook, database recovery runbook

### Postmortem
A document analyzing an incident after it has been resolved. Includes timeline, root cause, impact, and action items to prevent recurrence.

### Project Map
A JSON file (`project-map.json`) containing the scanned structure of the repository including directories, files, frameworks, languages, and documentation locations.

### Signals
A JSON file (`signals.json`) containing current health indicators for documentation, code quality, drift status, and alignment metrics.

### Drift Report
A JSON file (`drift-report.json`) containing the results of a documentation drift scan, including all detected issues and their severity.

## AI Command Hub

### AI Command Hub
A collection of prompts, schemas, and examples that enable AI agents to generate structured tasks and automations conforming to project standards.

### System Prompt
A prompt that provides context and instructions for AI agents. Sets the role, capabilities, and constraints for the AI.

**Example:** `AGENT_BRIEF.system.md`

### User Prompt
A prompt that provides specific instructions for a task. Combined with system prompt to generate outputs.

**Examples:** `TASK_WRITER.user.md`, `RUNBOOK_AUTOMATOR.user.md`

### Task Specification
A JSON schema (`task.spec.json`) defining the structure of actionable tasks. Includes fields for title, description, acceptance criteria, dependencies, etc.

### Automation Specification
A JSON schema (`automation.spec.json`) defining the structure of automated workflows. Includes steps, validation, rollback procedures, etc.

## Processes and Operations

### Drift Detection
The automated process of scanning the repository to identify documentation that is missing, outdated, or inconsistent with the current codebase.

**Tools:** `docs-scan.ts`

### Drift Resolution
The process of fixing identified drift by updating documentation, creating missing docs, or removing obsolete docs.

### ADR Validation
The automated process of checking ADRs for proper format, sequential numbering, valid statuses, and required sections.

**Tools:** `adr-validate.ts`

### TOC (Table of Contents) Synchronization
The automated process of generating and updating tables of contents in markdown files based on their heading structure.

**Tools:** `toc-sync.ts`

### Changelog Generation
The automated process of creating a changelog from git commit history, grouped by type (features, fixes, etc.).

**Tools:** `changelog.ts`

### Release Notes Generation
The automated process of creating comprehensive release notes from changelog and project metadata.

**Tools:** `release-notes.ts`

## CI/CD and Automation

### CI Validation Workflow
GitHub Actions workflow that runs on every PR to validate documentation alignment, ADRs, and TOC synchronization.

**File:** `.github/workflows/ctocenter-validate.yml`

### Weekly Drift Workflow
GitHub Actions workflow that runs weekly (Mondays) to scan for drift and create GitHub issues with findings.

**File:** `.github/workflows/ctocenter-weekly-drift.yml`

### Release Workflow
GitHub Actions workflow that triggers on version tags to generate changelog, release notes, and create draft GitHub releases.

**File:** `.github/workflows/ctocenter-release.yml`

### CI Summary
A generated summary of CI build results including test results, coverage, artifacts, warnings, and errors.

**Tools:** `ci-summary.ts`

## Severity Levels

### Critical
Issues that prevent the system from functioning or represent immediate security risks. Must be addressed immediately.

**Example:** Missing documentation for production APIs

### High
Important issues that significantly impact usability, maintainability, or reliability. Should be addressed soon.

**Example:** Outdated deployment documentation

### Medium
Moderate issues that affect quality but don't block operations. Should be addressed in normal workflow.

**Example:** Missing reference documentation for a utility module

### Low
Minor issues that have minimal impact. Can be addressed when convenient.

**Example:** Stale documentation older than 6 months

## File Types and Locations

### `.ctocenter/`
Root directory for all CTO Command Center files and state.

### `.ctocenter/state/`
Current state files including project map, signals, and reports.

### `.ctocenter/templates/`
Document templates for playbooks, runbooks, ADRs, and postmortems.

### `.ctocenter/ai/`
AI Command Hub files including prompts, schemas, and examples.

### `.ctocenter/scripts/`
TypeScript automation scripts for drift detection, validation, changelog generation, etc.

### `.ctocenter/adrs/`
Architecture Decision Records following the naming pattern `ADR-NNNN-title.md`.

### `.ctocenter/playbooks/`
Operational playbooks for standard procedures.

### `.ctocenter/runbooks/`
Incident response runbooks for operational issues.

### `docs/cto/`
Documentation about the CTO Command Center itself (usage guides, roles, glossary).

## Common Acronyms

**ADR** - Architecture Decision Record  
**CI/CD** - Continuous Integration / Continuous Deployment  
**TOC** - Table of Contents  
**PR** - Pull Request  
**QA** - Quality Assurance  
**SRE** - Site Reliability Engineering  
**PM** - Product Manager  
**CTO** - Chief Technology Officer  
**JSON** - JavaScript Object Notation  
**API** - Application Programming Interface  
**VST** - Virtual Studio Technology (audio plugin format)  
**JUCE** - Framework for audio application development

## Status Terms

### In Alignment
State where documentation matches code reality. No drift detected or only low-severity issues present.

### Drifted
State where significant documentation gaps or inconsistencies exist. High or critical severity issues detected.

### Validated
State where ADRs pass all validation checks including sequential numbering, valid statuses, and required sections.

### Synchronized
State where TOC in documents matches the actual heading structure. No manual updates needed.

## Workflow Terms

### Drift Scan
The act of running `docs-scan.ts` to check for documentation drift.

### ADR Review
The process of reviewing proposed ADRs for technical accuracy, completeness, and alignment with existing decisions.

### Rollback
The process of undoing changes made by an automation or deployment. Critical for safe operation.

### Validation
The process of checking that a document, configuration, or automation meets required standards and specifications.

### Artifact
A file or set of files produced by a build or automation process. Examples: compiled binaries, generated documentation, test reports.

## Team Terms

### CODEOWNERS
A file defining who is responsible for reviewing changes to specific files or directories. Enables automatic reviewer assignment.

### Maintainer
A person with write access to the repository who can merge PRs and make direct changes.

### Contributor
Anyone who submits code, documentation, or other changes to the repository via PRs.

### Reviewer
A person assigned to review a PR before it can be merged. Verifies quality, correctness, and adherence to standards.

## Best Practices

### Idempotency
The property of operations that can be applied multiple times without changing the result beyond the initial application.

**Example:** Running `toc-sync.ts` multiple times produces the same result.

### Fail Fast
The principle of detecting and reporting errors as soon as possible rather than allowing them to propagate.

**Example:** CI validation fails immediately when drift is detected.

### Continuous Validation
The practice of running validation checks frequently (on every PR, daily, weekly) rather than only during releases.

### Self-Service
The ability for team members to perform operations independently without requiring manual intervention from specialists.

**Example:** Developers can run drift scans and fix issues without waiting for tech writers.

## Metrics

### Drift Score
A quantitative measure of documentation drift based on the number and severity of detected issues.

**Formula:** `(Critical × 10) + (High × 5) + (Medium × 2) + (Low × 1)`

### ADR Coverage
The percentage of significant architectural decisions that have corresponding ADRs.

### Documentation Freshness
The average age of documentation files or the percentage updated within a time period (e.g., last 6 months).

### Alignment Rate
The percentage of time the repository is in alignment (no critical or high severity drift).

### Resolution Time
The average time taken to fix drift issues from detection to resolution.

## Related Technologies

### TypeScript
A typed superset of JavaScript used for CTO Command Center automation scripts. Provides better tooling and error detection.

### Node.js
JavaScript runtime used to execute TypeScript scripts via `ts-node`.

### GitHub Actions
CI/CD platform integrated with GitHub for running automated workflows.

### JSON Schema
A vocabulary for annotating and validating JSON documents. Used to define task and automation specifications.

### Markdown
Lightweight markup language used for all documentation files. Easy to read and write, supports version control well.

### Git
Version control system tracking all changes to code and documentation. Enables history, collaboration, and rollback.

---

## Quick Reference

| Term | Category | Quick Definition |
|------|----------|-----------------|
| ADR | Document | Architectural decision record |
| Drift | Problem | Documentation out of sync with code |
| Playbook | Document | Standard operational procedure |
| Runbook | Document | Incident response procedure |
| Alignment | Goal | Docs match reality |
| Validation | Process | Check for correctness |
| Artifact | Output | Generated file or report |
| Idempotent | Property | Safe to run multiple times |

---

**Not finding a term?** [Open an issue](../../issues) to request additions to this glossary.

**Last Updated:** 2025-11-09
