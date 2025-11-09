# AI Command Hub

**Purpose:** Central orchestration point for AI-powered operational automation and task generation within the CTO Command Center.

## Overview

The AI Command Hub provides structured prompts, schemas, and examples that enable AI agents to:
- Generate machine-actionable tasks from high-level objectives
- Automate runbook execution for incident response
- Create and maintain operational documentation
- Detect and remediate documentation drift
- Generate release artifacts

## Components

### 1. Prompts (`prompts/`)

Structured prompt templates for AI agents:

- **AGENT_BRIEF.system.md** - System-level context for all AI agents
- **TASK_WRITER.user.md** - Generate structured, actionable tasks
- **RUNBOOK_AUTOMATOR.user.md** - Automate incident response procedures

### 2. Schemas (`schemas/`)

JSON schemas for validating AI-generated outputs:

- **task.spec.json** - Task structure specification
- **automation.spec.json** - Automation workflow specification

### 3. Examples (`examples/`)

Reference examples demonstrating correct output format:

- **example-task.json** - Sample task structure
- **example-automation.json** - Sample automation workflow

## Usage

### Generate Tasks from Objectives

```bash
# Use the TASK_WRITER prompt to convert high-level goals into actionable tasks
# Output should conform to task.spec.json schema
```

**Input:** High-level objective or epic  
**Output:** Structured JSON tasks saved to `.ctocenter/tasks/`

### Automate Runbook Execution

```bash
# Use the RUNBOOK_AUTOMATOR prompt for incident response
# Output should conform to automation.spec.json schema
```

**Input:** Incident symptoms and runbook reference  
**Output:** Automated remediation steps saved to `.ctocenter/automations/`

### Validate Outputs

All AI-generated artifacts must validate against their respective schemas:

```bash
# Validate task output
npm run ctocenter:validate-task <task-file.json>

# Validate automation output
npm run ctocenter:validate-automation <automation-file.json>
```

## Workflow

### 1. Define Objective

Start with a clear, high-level objective:
- "Implement feature X"
- "Resolve incident Y"
- "Update documentation for Z"

### 2. Select Appropriate Prompt

Choose the prompt that matches your goal:
- Strategic work → TASK_WRITER
- Incident response → RUNBOOK_AUTOMATOR
- Documentation → AGENT_BRIEF (context for other prompts)

### 3. Execute with AI Agent

Provide the prompt and objective to your AI agent (GitHub Copilot, ChatGPT, etc.)

### 4. Validate Output

Ensure the output:
- Conforms to the relevant schema
- Is complete and actionable
- Includes all required fields

### 5. Save and Track

Store validated outputs in the appropriate directory:
- Tasks → `.ctocenter/tasks/YYYY-MM-DD-task-name.json`
- Automations → `.ctocenter/automations/YYYY-MM-DD-automation-name.json`

### 6. Execute and Verify

Implement the generated tasks or run the automation, then verify results.

## Integration Points

### With Playbooks

Generated tasks can reference playbooks for standardized procedures:

```json
{
  "task": "Deploy feature X",
  "playbook": ".ctocenter/playbooks/deployment.md",
  "automated": true
}
```

### With Runbooks

Automations execute runbook steps for incident response:

```json
{
  "trigger": "Service outage detected",
  "runbook": ".ctocenter/runbooks/service-recovery.md",
  "steps": [...]
}
```

### With ADRs

Tasks may require creating or updating ADRs:

```json
{
  "task": "Implement new architecture",
  "requires_adr": true,
  "adr_template": ".ctocenter/templates/ADR.template.md"
}
```

## Best Practices

### For Task Generation

1. **Be Specific:** Provide detailed context and constraints
2. **Define Success:** Include clear acceptance criteria
3. **Consider Dependencies:** Map prerequisites and blockers
4. **Estimate Scope:** Include time and complexity estimates

### For Automation

1. **Test Safely:** Always test automations in non-production first
2. **Include Rollback:** Every automation needs a rollback procedure
3. **Monitor Execution:** Log all steps and their outcomes
4. **Handle Failures:** Define failure modes and recovery

### For Documentation

1. **Keep Updated:** Regenerate when source code changes
2. **Link Bidirectionally:** Connect tasks, ADRs, and runbooks
3. **Version Control:** Track changes to all generated artifacts
4. **Review Regularly:** Validate AI outputs for accuracy

## Advanced Usage

### Chaining Prompts

Complex objectives may require multiple prompts in sequence:

1. TASK_WRITER → Generate high-level tasks
2. AGENT_BRIEF → Provide context for execution
3. RUNBOOK_AUTOMATOR → Handle specific operational procedures

### Custom Prompts

Create project-specific prompts in `prompts/custom/`:

```
.ctocenter/ai/prompts/custom/
  my-team-workflow.prompt.md
  deployment-automation.prompt.md
```

### Feedback Loops

Improve prompts based on outcomes:

1. Track prompt → output → result
2. Identify patterns in failures or suboptimal outputs
3. Refine prompts and schemas
4. Update examples

## Security Considerations

⚠️ **Important Security Notes:**

- **Never include secrets** in tasks or automations
- **Review AI outputs** before executing
- **Limit permissions** for automation execution
- **Audit all changes** made by automations
- **Test in isolation** before production use

## Troubleshooting

### AI Output Doesn't Match Schema

**Problem:** Generated JSON fails schema validation

**Solutions:**
1. Check the prompt includes schema reference
2. Provide the schema as part of the prompt context
3. Use the example files as reference in the prompt
4. Manually fix and provide feedback to improve the prompt

### Automation Fails During Execution

**Problem:** Generated automation steps fail

**Solutions:**
1. Verify runbook accuracy first
2. Test each step manually
3. Check for missing dependencies or permissions
4. Update the automation with corrected steps
5. Improve the RUNBOOK_AUTOMATOR prompt

### Tasks Are Too Vague

**Problem:** Generated tasks lack detail

**Solutions:**
1. Provide more context in the objective
2. Reference relevant ADRs and documentation
3. Include technical constraints and requirements
4. Use examples to show desired detail level

## Support and Feedback

To improve the AI Command Hub:

1. **Document Issues:** Create a GitHub issue with prompt + output + expected result
2. **Share Successes:** Document effective prompt patterns
3. **Update Examples:** Add real-world examples that worked well
4. **Refine Schemas:** Propose schema improvements via PR

## Related Documentation

- [USAGE.md](../../docs/cto/USAGE.md) - General CTO Command Center usage
- [QUICKSTART.md](../../docs/cto/QUICKSTART.md) - Quick start guide
- [GLOSSARY.md](../../docs/cto/GLOSSARY.md) - Terms and definitions

---

**Last Updated:** 2025-11-09  
**Maintained By:** Engineering Operations
