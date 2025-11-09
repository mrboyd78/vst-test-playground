# CTO Command Center - Usage Guide

**Version:** 1.0.0  
**Last Updated:** 2025-11-09

## Table of Contents

<!-- TOC -->
## Table of Contents

- [Table of Contents](#table-of-contents)
- [Table of Contents](#table-of-contents)
- [Introduction](#introduction)
- [Quick Start](#quick-start)
  - [Prerequisites](#prerequisites)
  - [Initial Setup](#initial-setup)
- [Daily Operations](#daily-operations)
  - [1. Check Documentation Alignment](#1-check-documentation-alignment)
  - [2. Create or Update ADRs](#2-create-or-update-adrs)
  - [3. Update Documentation](#3-update-documentation)
  - [4. Open Pull Request](#4-open-pull-request)
- [Weekly Tasks](#weekly-tasks)
  - [Monday: Review Drift Report](#monday-review-drift-report)
  - [Update Project Map](#update-project-map)
  - [Validate ADRs](#validate-adrs)
- [Release Management](#release-management)
  - [Preparing a Release](#preparing-a-release)
  - [Automated Release Workflow](#automated-release-workflow)
- [AI Command Hub](#ai-command-hub)
  - [Using the Task Writer](#using-the-task-writer)
  - [Using the Runbook Automator](#using-the-runbook-automator)
  - [Best Practices for AI Hub](#best-practices-for-ai-hub)
- [Documentation Management](#documentation-management)
  - [Documentation Structure](#documentation-structure)
  - [Creating Documentation](#creating-documentation)
  - [Maintaining Documentation](#maintaining-documentation)
- [Architecture Decision Records (ADRs)](#architecture-decision-records-adrs)
  - [When to Create an ADR](#when-to-create-an-adr)
  - [ADR Lifecycle](#adr-lifecycle)
  - [Creating an ADR](#creating-an-adr)
  - [Updating ADR Status](#updating-adr-status)
  - [Validating ADRs](#validating-adrs)
- [Drift Detection and Resolution](#drift-detection-and-resolution)
  - [Understanding Drift](#understanding-drift)
  - [Running Drift Detection](#running-drift-detection)
  - [Interpreting Drift Reports](#interpreting-drift-reports)
  - [Resolving Drift](#resolving-drift)
  - [Accepting Risk](#accepting-risk)
- [Troubleshooting](#troubleshooting)
  - ["project-map.json not found"](#project-mapjson-not-found)
  - ["ADR numbering gap detected"](#adr-numbering-gap-detected)
  - ["Table of contents out of sync"](#table-of-contents-out-of-sync)
  - ["Cannot run TypeScript scripts"](#cannot-run-typescript-scripts)
  - ["CI validation failing"](#ci-validation-failing)
- [Advanced Usage](#advanced-usage)
  - [Custom Prompts](#custom-prompts)
  - [Extending Scripts](#extending-scripts)
  - [Integration with Tools](#integration-with-tools)
  - [Customizing Workflows](#customizing-workflows)
- [Related Documentation](#related-documentation)
- [Support](#support)

<!-- /TOC -->

## Introduction

The CTO Command Center is a comprehensive framework for maintaining operational excellence, documentation alignment, and engineering productivity. It provides:

- **Automated drift detection** to keep documentation synchronized with code
- **Architecture Decision Records (ADRs)** for tracking significant technical decisions
- **Playbooks and Runbooks** for standardized operational procedures
- **AI-powered task generation** and automation
- **Release management** tools for changelogs and release notes
- **CI/CD integration** for enforcing alignment

## Quick Start

### Prerequisites

- Node.js 18+ (for TypeScript scripts)
- TypeScript (`npm install -g typescript ts-node @types/node`)
- Git
- Access to repository

### Initial Setup

1. **Verify installation:**
   ```bash
   ls -la .ctocenter/
   ```

2. **Run initial scan:**
   ```bash
   cd .ctocenter/scripts
   ts-node docs-scan.ts
   ```

3. **Check for drift:**
   ```bash
   cat .ctocenter/state/drift-report.json
   ```

4. **Review project map:**
   ```bash
   cat .ctocenter/state/project-map.json
   ```

## Daily Operations

### 1. Check Documentation Alignment

Before starting work, verify documentation is aligned:

```bash
cd .ctocenter/scripts
ts-node docs-scan.ts
```

**Interpret results:**
- ✅ No drift: Proceed with confidence
- ⚠️ Low/Medium: Note issues, fix when convenient
- ❌ Critical/High: Address before proceeding

### 2. Create or Update ADRs

When making architectural decisions:

```bash
# Determine next ADR number
ls .ctocenter/adrs/ | sort | tail -1

# Copy template
cp .ctocenter/templates/ADR.template.md .ctocenter/adrs/ADR-0001-your-decision.md

# Edit the ADR with your decision details
```

### 3. Update Documentation

After code changes:

1. Update relevant docs in `docs/`
2. Run TOC sync:
   ```bash
   cd .ctocenter/scripts
   ts-node toc-sync.ts --all
   ```

3. Verify drift resolution:
   ```bash
   ts-node docs-scan.ts
   ```

### 4. Open Pull Request

Use the PR template (`.github/PULL_REQUEST_TEMPLATE.md`) which includes:
- CTO Command Center alignment checklist
- Documentation requirements
- ADR confirmation

## Weekly Tasks

### Monday: Review Drift Report

The weekly drift check runs automatically every Monday at 9 AM UTC.

**To review:**

1. Check GitHub Issues for the weekly drift report
2. Review drift report artifact in Actions
3. Prioritize issues:
   - Critical: Fix immediately
   - High: Fix this week
   - Medium: Schedule for next sprint
   - Low: Backlog

**To manually trigger:**

```bash
# Via GitHub Actions UI: Run "CTO Center Weekly Drift Check"
# Or locally:
cd .ctocenter/scripts
ts-node docs-scan.ts > /tmp/drift-report.txt
```

### Update Project Map

If repository structure changed:

```bash
# Regenerate project map
node /tmp/generate-project-map.js > .ctocenter/state/project-map.json

# Commit changes
git add .ctocenter/state/project-map.json
git commit -m "chore: update project map"
```

### Validate ADRs

Ensure ADRs are properly maintained:

```bash
cd .ctocenter/scripts
ts-node adr-validate.ts
```

Fix any issues before they accumulate.

## Release Management

### Preparing a Release

1. **Generate changelog:**
   ```bash
   cd .ctocenter/scripts
   ts-node changelog.ts --from v1.0.0 --to v1.1.0
   ```

2. **Generate release notes:**
   ```bash
   ts-node release-notes.ts 1.1.0
   ```

3. **Review generated files:**
   - `CHANGELOG.md`
   - `docs/releases/v1.1.0.md`

4. **Update documentation:**
   ```bash
   ts-node toc-sync.ts --all
   ```

5. **Create tag and push:**
   ```bash
   git tag -a v1.1.0 -m "Release v1.1.0"
   git push origin v1.1.0
   ```

6. **GitHub Actions will:**
   - Generate release artifacts
   - Create draft GitHub release
   - Run all validations

7. **Manual steps:**
   - Review draft release
   - Add any manual highlights
   - Publish release

### Automated Release Workflow

Push a tag to trigger automated release:

```bash
git tag v1.1.0
git push origin v1.1.0
```

This triggers `.github/workflows/ctocenter-release.yml` which:
- Generates changelog
- Creates release notes
- Updates TOC
- Creates GitHub release (draft)

## AI Command Hub

### Using the Task Writer

**Purpose:** Convert high-level objectives into actionable tasks.

1. **Review the prompt:**
   ```bash
   cat .ctocenter/ai/prompts/TASK_WRITER.user.md
   ```

2. **Provide to your AI assistant:**
   - System prompt: `AGENT_BRIEF.system.md`
   - User prompt: `TASK_WRITER.user.md`
   - Your objective and context

3. **AI generates JSON tasks** conforming to `task.spec.json`

4. **Validate output:**
   ```bash
   # Check against schema (if you have a JSON schema validator)
   jsonschema -i task.json .ctocenter/ai/schemas/task.spec.json
   ```

5. **Save tasks:**
   ```bash
   cp generated-task.json .ctocenter/tasks/2025-11-09-feature-name.json
   ```

6. **Import into your project management tool**

### Using the Runbook Automator

**Purpose:** Automate operational procedures.

1. **Review the prompt:**
   ```bash
   cat .ctocenter/ai/prompts/RUNBOOK_AUTOMATOR.user.md
   ```

2. **Provide incident/operation details to AI**

3. **AI generates automation JSON** conforming to `automation.spec.json`

4. **IMPORTANT: Review carefully** - never run untested automations

5. **Test in safe environment:**
   - Use staging/development environment
   - Test each step individually
   - Verify rollback procedures

6. **Save automation:**
   ```bash
   cp generated-automation.json .ctocenter/automations/2025-11-09-operation-name.json
   ```

### Best Practices for AI Hub

- ✅ Always validate AI outputs against schemas
- ✅ Review generated code/commands before executing
- ✅ Test automations in non-production first
- ✅ Keep prompts updated based on learnings
- ✅ Document successful patterns
- ❌ Never run AI-generated automations blindly
- ❌ Don't skip validation steps
- ❌ Don't trust without verification

## Documentation Management

### Documentation Structure

```
docs/
  cto/              # CTO Command Center documentation
  guides/           # User and developer guides
  reference/        # Technical reference
  ways-of-work/     # Process documentation
  releases/         # Release notes
  migration/        # Migration guides

.ctocenter/
  adrs/             # Architecture Decision Records
  playbooks/        # Operational playbooks
  runbooks/         # Incident response runbooks
  templates/        # Document templates
  ai/               # AI Command Hub
  scripts/          # Automation scripts
  state/            # Current state and reports
```

### Creating Documentation

**Playbook (Standard procedures):**
```bash
cp .ctocenter/templates/PLAYBOOK.template.md .ctocenter/playbooks/deployment.md
# Edit with your procedure
```

**Runbook (Incident response):**
```bash
cp .ctocenter/templates/RUNBOOK.template.md .ctocenter/runbooks/service-recovery.md
# Edit with recovery steps
```

**Postmortem:**
```bash
cp .ctocenter/templates/POSTMORTEM.template.md docs/postmortems/2025-11-09-outage.md
# Document the incident
```

### Maintaining Documentation

1. **Keep it current:**
   - Update docs when code changes
   - Run drift detection regularly
   - Fix high-priority drift immediately

2. **Use templates:**
   - Ensures consistency
   - Includes all required sections
   - Makes documents scannable

3. **Link documents:**
   - ADRs reference related playbooks
   - Runbooks reference playbooks
   - Cross-reference frequently

4. **Version documentation:**
   - Track changes in git
   - Note version in migration guides
   - Archive old versions

## Architecture Decision Records (ADRs)

### When to Create an ADR

Create an ADR when making decisions about:
- System architecture
- Technology selection
- Design patterns
- Build/deployment approach
- Security architecture
- Performance optimizations
- API design

### ADR Lifecycle

1. **Proposed** - Decision under consideration
2. **Accepted** - Decision approved and implemented
3. **Deprecated** - No longer recommended (but may exist in code)
4. **Superseded** - Replaced by a newer ADR

### Creating an ADR

```bash
# Find next number
NEXT_NUM=$(printf "%04d" $(($(ls .ctocenter/adrs/ADR-*.md 2>/dev/null | wc -l) + 1)))

# Create ADR
cp .ctocenter/templates/ADR.template.md .ctocenter/adrs/ADR-${NEXT_NUM}-title.md

# Edit with your decision
```

### Updating ADR Status

When a decision changes status:

1. Update the `Status:` field in the ADR
2. If superseding, add `Superseded By:` section
3. If superseded, update new ADR with `Supersedes:` section
4. Commit changes

### Validating ADRs

```bash
cd .ctocenter/scripts
ts-node adr-validate.ts
```

This checks:
- Sequential numbering
- Valid status values
- Required sections present
- Proper superseding links

## Drift Detection and Resolution

### Understanding Drift

**Documentation drift** occurs when:
- Code changes but docs don't
- New features lack documentation
- Build system changes aren't documented
- API changes aren't reflected in reference docs

### Running Drift Detection

**Manual:**
```bash
cd .ctocenter/scripts
ts-node docs-scan.ts
```

**Automated:**
- Runs on every PR (`.github/workflows/ctocenter-validate.yml`)
- Runs weekly (`.github/workflows/ctocenter-weekly-drift.yml`)
- Can be triggered manually in GitHub Actions

### Interpreting Drift Reports

Drift report (`drift-report.json`) contains:

```json
{
  "hasDrift": true,
  "issues": [
    {
      "type": "missing_doc",
      "severity": "high",
      "file": "docs/reference/NewFeature.md",
      "description": "...",
      "recommendation": "..."
    }
  ],
  "summary": {
    "total": 5,
    "critical": 0,
    "high": 2,
    "medium": 2,
    "low": 1
  }
}
```

### Resolving Drift

**Priority:** Critical > High > Medium > Low

**Steps:**
1. Read the drift report
2. For each issue:
   - Understand the problem
   - Follow the recommendation
   - Create/update documentation
3. Re-run drift detection
4. Repeat until clean

**Common fixes:**
- Create missing docs: Use templates
- Update stale docs: Review and refresh content
- Fix broken links: Update or remove
- Document new features: Add to guides and reference

### Accepting Risk

Sometimes drift is acceptable:
1. Document why in an ADR
2. Add to `.ctocenter/state/accepted-risks.md`
3. Update drift scanner to ignore (if needed)

## Troubleshooting

### "project-map.json not found"

**Solution:**
```bash
node /tmp/generate-project-map.js > .ctocenter/state/project-map.json
```

### "ADR numbering gap detected"

**Solution:**
Renumber ADRs to be sequential:
```bash
cd .ctocenter/adrs
# Manually rename files to fill gaps
# ADR-0001, ADR-0002, ADR-0003, etc.
```

### "Table of contents out of sync"

**Solution:**
```bash
cd .ctocenter/scripts
ts-node toc-sync.ts --all
git add .
git commit -m "docs: sync table of contents"
```

### "Cannot run TypeScript scripts"

**Solution:**
```bash
npm install -g typescript ts-node @types/node
```

### "CI validation failing"

**Solutions:**
1. Run locally: `cd .ctocenter/scripts && ts-node docs-scan.ts`
2. Fix reported issues
3. Ensure all docs are committed
4. Push and re-run CI

## Advanced Usage

### Custom Prompts

Create project-specific AI prompts:

```bash
mkdir -p .ctocenter/ai/prompts/custom
vim .ctocenter/ai/prompts/custom/my-workflow.prompt.md
```

### Extending Scripts

Scripts are in TypeScript and can be extended:

```typescript
// .ctocenter/scripts/custom-check.ts
import { DocsScanner } from './docs-scan';

class CustomScanner extends DocsScanner {
  // Add custom checks
}
```

### Integration with Tools

**Slack notifications:**
```bash
# In CI, after drift detection
curl -X POST -H 'Content-type: application/json' \
  --data '{"text":"Drift detected!"}' \
  $SLACK_WEBHOOK_URL
```

**Jira integration:**
- Use GitHub Actions to create Jira tickets for drift
- Link ADRs to Jira epics
- Auto-update Jira from GitHub

**IDE integration:**
- Configure VSCode tasks for common commands
- Add run configurations in IntelliJ
- Create shell aliases for frequent tasks

### Customizing Workflows

Edit `.github/workflows/ctocenter-*.yml` to:
- Change schedule (cron expressions)
- Add custom validation steps
- Integrate with other tools
- Modify notification behavior

---

## Related Documentation

- [Quick Start Guide](./QUICKSTART.md) - Get started in 5 minutes
- [Roles and Responsibilities](./ROLES.md) - Who does what
- [Glossary](./GLOSSARY.md) - Terms and definitions
- [AI Command Hub](../../.ctocenter/ai/HUB.md) - AI automation guide

## Support

- **Issues:** Open a GitHub issue with the `cto-center` label
- **Documentation:** Check `.ctocenter/ai/HUB.md` and templates
- **Updates:** Watch for weekly drift reports

---

**Maintained by:** Engineering Operations  
**Last Review:** 2025-11-09
