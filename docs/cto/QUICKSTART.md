# CTO Command Center - Quick Start Guide

Get up and running with the CTO Command Center in 5 minutes!

## 🚀 Quick Setup

### 1. Verify Installation

```bash
# Check structure exists
ls -la .ctocenter/

# Should see:
# state/        - Current state and reports
# templates/    - Document templates
# ai/           - AI Command Hub
# scripts/      - Automation scripts
```

### 2. Install Dependencies

```bash
# Install TypeScript (if not already installed)
npm install -g typescript ts-node @types/node
```

### 3. Run Initial Scan

```bash
cd .ctocenter/scripts
ts-node docs-scan.ts
```

**Expected output:**
- ✅ No drift detected (clean repository)
- ⚠️ Some drift detected (see recommendations)

## 📋 Daily Workflow

### Morning Routine

```bash
# 1. Check for drift
cd .ctocenter/scripts
ts-node docs-scan.ts

# 2. Review any issues
cat .ctocenter/state/drift-report.json

# 3. Update documentation if needed
vim docs/guides/YOUR_DOC.md

# 4. Sync table of contents
ts-node toc-sync.ts --all
```

### When Making Code Changes

1. **Write code** ✍️
2. **Update docs** 📝 (if needed)
3. **Create ADR** 📋 (for architectural decisions)
4. **Run drift check** 🔍
5. **Open PR** 🚀

### Opening a Pull Request

The PR template automatically includes:
- ✅ CTO Command Center alignment checklist
- ✅ Documentation requirements
- ✅ ADR confirmation

Just fill it out!

## 🎯 Common Tasks

### Create an ADR

```bash
# Get next number
NEXT=$(printf "%04d" $(($(ls .ctocenter/adrs/*.md 2>/dev/null | wc -l) + 1)))

# Create from template
cp .ctocenter/templates/ADR.template.md .ctocenter/adrs/ADR-${NEXT}-your-title.md

# Edit it
vim .ctocenter/adrs/ADR-${NEXT}-your-title.md

# Validate
cd .ctocenter/scripts
ts-node adr-validate.ts
```

### Generate Changelog

```bash
cd .ctocenter/scripts

# From last tag to HEAD
ts-node changelog.ts --from v1.0.0 --to HEAD

# Review
cat CHANGELOG.md
```

### Fix Documentation Drift

```bash
# 1. Identify issues
cd .ctocenter/scripts
ts-node docs-scan.ts

# 2. Fix each issue
# (Follow recommendations in the report)

# 3. Verify fixed
ts-node docs-scan.ts

# Should show: ✅ No drift detected
```

### Sync Table of Contents

```bash
cd .ctocenter/scripts

# Single file
ts-node toc-sync.ts README.md

# All docs
ts-node toc-sync.ts --all
```

## 🤖 AI Command Hub

### Generate Tasks

1. **Read the prompt:**
   ```bash
   cat .ctocenter/ai/prompts/TASK_WRITER.user.md
   ```

2. **Give to AI** (ChatGPT, Copilot, etc.) with your objective

3. **Save output:**
   ```bash
   cat > .ctocenter/tasks/2025-11-09-my-feature.json
   # Paste AI-generated JSON
   ```

4. **Use tasks** in your project management tool

### Automate Runbooks

1. **Read the prompt:**
   ```bash
   cat .ctocenter/ai/prompts/RUNBOOK_AUTOMATOR.user.md
   ```

2. **Give to AI** with incident details

3. **⚠️ REVIEW CAREFULLY** - Don't run blindly!

4. **Test in staging** before production

5. **Save automation:**
   ```bash
   cat > .ctocenter/automations/2025-11-09-recovery.json
   # Paste AI-generated JSON
   ```

## 🔄 CI/CD Integration

### Workflows Run Automatically

**On every PR:**
- Documentation drift check
- ADR validation
- TOC synchronization check

**Weekly (Mondays 9 AM UTC):**
- Comprehensive drift scan
- Creates GitHub issue with report

**On version tags (`v*.*.*`):**
- Generate changelog
- Create release notes
- Create draft GitHub release

### Manual Triggers

Go to **Actions** tab → Select workflow → **Run workflow**

## 📊 Monitoring

### Check Current State

```bash
# Project map
cat .ctocenter/state/project-map.json

# Signals
cat .ctocenter/state/signals.json

# Latest drift report
cat .ctocenter/state/drift-report.json

# ADR validation
cat .ctocenter/state/adr-validation.json
```

### Weekly Review

Every Monday, check:
1. ✅ Weekly drift report issue
2. ✅ No critical/high severity issues
3. ✅ All ADRs valid
4. ✅ Documentation up to date

## 🎓 Learning Path

### Level 1: Basic Usage (Day 1)
- ✅ Run drift scan
- ✅ Create an ADR
- ✅ Open PR with template
- ✅ Fix simple drift

### Level 2: Regular Operations (Week 1)
- ✅ Generate changelog
- ✅ Use AI task writer
- ✅ Create playbook/runbook
- ✅ Resolve medium drift

### Level 3: Advanced (Month 1)
- ✅ Customize workflows
- ✅ Extend scripts
- ✅ Create custom AI prompts
- ✅ Integrate with tools

## 🆘 Quick Troubleshooting

### Script won't run

```bash
npm install -g typescript ts-node @types/node
```

### Drift won't clear

1. Read recommendations in report
2. Update specific files mentioned
3. Commit changes
4. Re-run scan

### ADR validation fails

```bash
cd .ctocenter/scripts
ts-node adr-validate.ts

# Fix issues listed
# Common: numbering gaps, missing sections
```

### CI fails

1. Run checks locally first
2. Fix all issues
3. Commit everything
4. Push and re-run

## 📚 Next Steps

- **Deep dive:** Read [USAGE.md](./USAGE.md) for comprehensive guide
- **Understand roles:** See [ROLES.md](./ROLES.md)
- **Learn terms:** Check [GLOSSARY.md](./GLOSSARY.md)
- **AI automation:** Explore [AI Hub](../../.ctocenter/ai/HUB.md)

## 🎯 Success Metrics

You're using the CTO Command Center effectively when:

- ✅ No documentation drift (or only low severity)
- ✅ All ADRs validated and sequential
- ✅ PRs include documentation updates
- ✅ Changelogs generated automatically
- ✅ Weekly drift issues are empty or quickly resolved
- ✅ Team knows where to find operational docs

## 💡 Pro Tips

1. **Run drift check before starting work** - Start clean
2. **Create ADRs proactively** - Don't wait
3. **Use AI Hub for repetitive tasks** - Save time
4. **Keep docs near code** - Easy to maintain
5. **Link everything** - ADRs ↔ Playbooks ↔ Code
6. **Automate validation** - Let CI catch issues
7. **Review weekly reports** - Stay ahead of drift

---

**Time to value:** 5 minutes  
**Mastery:** 1 week  
**Maintenance:** 15 min/week

🚀 **Ready to start?** Run your first drift scan now!

```bash
cd .ctocenter/scripts && ts-node docs-scan.ts
```
