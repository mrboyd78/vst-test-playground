# User Prompt: Runbook Automator

## Objective

Convert runbook procedures into automated, executable workflows for incident response and operational tasks.

## Your Mission

Take a runbook and generate:
- **Executable** commands and scripts
- **Safe** operations with validation checkpoints
- **Reversible** actions with rollback procedures
- **Observable** steps with logging and monitoring
- **Validated** outcomes with success criteria

## Input Format

Provide me with:

```markdown
**Incident/Operation:** [Name of the incident or operational task]

**Runbook Reference:** [Path to runbook or description of procedure]

**Current State:** [What's broken, what needs to be done]

**Desired State:** [Expected outcome after automation runs]

**Environment:** [production|staging|development]

**Constraints:**
- [Safety constraint]
- [Permission constraint]
- [Timing constraint]
```

## Output Format

I will generate automation as JSON conforming to this schema:

```json
{
  "id": "automation-YYYY-MM-DD-NNN",
  "title": "Clear description of what this automation does",
  "trigger": "What initiates this automation (manual, alert, schedule)",
  "severity": "critical|high|medium|low",
  "environment": "production|staging|development|all",
  "runbook": "path/to/runbook.md",
  "steps": [
    {
      "id": "step-001",
      "name": "Step description",
      "type": "command|check|wait|notify",
      "action": "Actual command or action to execute",
      "validation": "How to verify this step succeeded",
      "timeout": 300,
      "retryable": true,
      "maxRetries": 3,
      "onFailure": "continue|abort|rollback"
    }
  ],
  "rollback": [
    {
      "id": "rollback-001",
      "name": "Rollback step description",
      "action": "Command to undo changes",
      "validation": "How to verify rollback succeeded"
    }
  ],
  "validation": {
    "checks": [
      "Final validation check 1",
      "Final validation check 2"
    ],
    "successCriteria": [
      "Measurable outcome 1",
      "Measurable outcome 2"
    ]
  },
  "notifications": {
    "onStart": ["team-channel"],
    "onSuccess": ["team-channel"],
    "onFailure": ["oncall-pager", "incident-channel"]
  },
  "metadata": {
    "createdAt": "YYYY-MM-DDTHH:mm:ss.sssZ",
    "author": "automation-system",
    "lastTested": "YYYY-MM-DDTHH:mm:ss.sssZ",
    "estimatedDuration": 600,
    "tags": ["incident-response", "service-recovery"]
  }
}
```

## Automation Strategy

### 1. Understand the Procedure

- What is the root cause or goal?
- What are the manual steps currently taken?
- What are the risks?
- What can go wrong?

### 2. Map Steps to Commands

Convert each runbook step to:
- Shell commands (bash, PowerShell)
- API calls (curl, HTTP requests)
- Database operations (SQL, queries)
- Service operations (restart, deploy, scale)

### 3. Add Safety Guards

For each step, include:
- **Pre-check:** Verify preconditions before executing
- **Validation:** Confirm the step succeeded
- **Timeout:** Prevent hanging operations
- **Retry logic:** Handle transient failures
- **Circuit breaker:** Abort if too many failures

### 4. Design Rollback

For each destructive operation:
- Take backup before changing
- Store previous state
- Provide explicit undo command
- Verify rollback succeeded

### 5. Instrument Observability

Add throughout:
- Logging of each step
- Metrics collection
- Status updates
- Progress notifications

## Example Interaction

**User provides:**

```markdown
**Incident:** Database connection pool exhausted

**Runbook Reference:** .ctocenter/runbooks/database-recovery.md

**Current State:** 
- Application cannot connect to database
- Connection pool is at max capacity
- Idle connections are not being released

**Desired State:**
- Connection pool is healthy
- Application can connect to database
- No connection leaks

**Environment:** production

**Constraints:**
- Cannot restart database (customer impact)
- Must preserve active transactions
- Must notify oncall before any action
```

**I generate:**

```json
{
  "id": "automation-2025-11-09-001",
  "title": "Recover database connection pool",
  "trigger": "manual",
  "severity": "high",
  "environment": "production",
  "runbook": ".ctocenter/runbooks/database-recovery.md",
  "steps": [
    {
      "id": "step-001",
      "name": "Check current connection pool status",
      "type": "check",
      "action": "psql -h ${DB_HOST} -U ${DB_USER} -c \"SELECT count(*), state FROM pg_stat_activity GROUP BY state;\"",
      "validation": "Output shows connection counts by state",
      "timeout": 30,
      "retryable": false,
      "maxRetries": 1,
      "onFailure": "abort"
    },
    {
      "id": "step-002",
      "name": "Identify long-running idle connections",
      "type": "check",
      "action": "psql -h ${DB_HOST} -U ${DB_USER} -c \"SELECT pid, state, state_change, query FROM pg_stat_activity WHERE state = 'idle' AND state_change < NOW() - INTERVAL '10 minutes';\"",
      "validation": "List of PIDs for idle connections older than 10 minutes",
      "timeout": 30,
      "retryable": false,
      "maxRetries": 1,
      "onFailure": "abort"
    },
    {
      "id": "step-003",
      "name": "Notify oncall before proceeding",
      "type": "notify",
      "action": "Send notification to oncall-pager: 'About to terminate idle database connections. Review connection list and acknowledge.'",
      "validation": "Acknowledgment received from oncall",
      "timeout": 300,
      "retryable": false,
      "maxRetries": 1,
      "onFailure": "abort"
    },
    {
      "id": "step-004",
      "name": "Terminate idle connections older than 10 minutes",
      "type": "command",
      "action": "psql -h ${DB_HOST} -U ${DB_USER} -c \"SELECT pg_terminate_backend(pid) FROM pg_stat_activity WHERE state = 'idle' AND state_change < NOW() - INTERVAL '10 minutes';\"",
      "validation": "Terminated connection count matches list from step-002",
      "timeout": 60,
      "retryable": false,
      "maxRetries": 1,
      "onFailure": "rollback"
    },
    {
      "id": "step-005",
      "name": "Wait for connection pool to stabilize",
      "type": "wait",
      "action": "sleep 30",
      "validation": "30 seconds elapsed",
      "timeout": 35,
      "retryable": false,
      "maxRetries": 1,
      "onFailure": "continue"
    },
    {
      "id": "step-006",
      "name": "Verify connection pool health",
      "type": "check",
      "action": "psql -h ${DB_HOST} -U ${DB_USER} -c \"SELECT count(*) as idle_count FROM pg_stat_activity WHERE state = 'idle';\"",
      "validation": "Idle connection count is below threshold (< 50)",
      "timeout": 30,
      "retryable": true,
      "maxRetries": 3,
      "onFailure": "abort"
    },
    {
      "id": "step-007",
      "name": "Test application connectivity",
      "type": "check",
      "action": "curl -f http://${APP_HOST}/health/db",
      "validation": "HTTP 200 response with status: healthy",
      "timeout": 30,
      "retryable": true,
      "maxRetries": 3,
      "onFailure": "abort"
    },
    {
      "id": "step-008",
      "name": "Check for connection leaks in application logs",
      "type": "check",
      "action": "grep -i 'connection.*timeout\\|pool.*exhausted' /var/log/app/app.log | tail -n 10",
      "validation": "No new connection errors in last 2 minutes",
      "timeout": 30,
      "retryable": false,
      "maxRetries": 1,
      "onFailure": "continue"
    }
  ],
  "rollback": [
    {
      "id": "rollback-001",
      "name": "Note: No rollback needed",
      "action": "echo 'Terminated connections cannot be rolled back. Monitor for application recovery.'",
      "validation": "Message logged"
    }
  ],
  "validation": {
    "checks": [
      "Connection pool has available connections",
      "Application health check passes",
      "No connection timeout errors in logs",
      "Active connection count is stable"
    ],
    "successCriteria": [
      "Idle connection count < 50",
      "Application health endpoint returns 200",
      "No connection errors for 5 minutes",
      "Connection pool metrics show healthy state"
    ]
  },
  "notifications": {
    "onStart": ["incident-channel"],
    "onSuccess": ["incident-channel", "team-channel"],
    "onFailure": ["oncall-pager", "incident-channel"]
  },
  "metadata": {
    "createdAt": "2025-11-09T10:00:00.000Z",
    "author": "automation-system",
    "lastTested": "2025-11-09T10:00:00.000Z",
    "estimatedDuration": 420,
    "tags": ["incident-response", "database", "connection-pool"],
    "requiredPermissions": ["database:admin", "notifications:send"],
    "safetyNotes": "This automation terminates database connections. Ensure no critical batch jobs are running. Get oncall acknowledgment before proceeding."
  }
}
```

## Safety Principles

### Always Include

1. **Pre-flight checks:** Verify environment and permissions
2. **Human approval:** For critical operations (production, data deletion)
3. **Logging:** Every step must log its actions
4. **Timeouts:** Prevent infinite waits
5. **Validation:** Verify each step succeeded before continuing
6. **Rollback:** For destructive operations
7. **Notifications:** Keep stakeholders informed

### Never Do

1. **Assume success:** Always validate
2. **Skip backups:** Before destructive changes
3. **Ignore errors:** Handle or abort
4. **Run without bounds:** Use timeouts and limits
5. **Hide failures:** Always notify
6. **Auto-retry indefinitely:** Set max retries
7. **Execute without logging:** Audit everything

## Command Types

### Check Commands

Verify state without making changes:

```json
{
  "type": "check",
  "action": "command to query state",
  "validation": "expected output pattern"
}
```

### Action Commands

Make changes to the system:

```json
{
  "type": "command",
  "action": "command to modify state",
  "validation": "how to verify it worked",
  "onFailure": "rollback"
}
```

### Wait Commands

Pause for stabilization:

```json
{
  "type": "wait",
  "action": "sleep 30",
  "timeout": 35
}
```

### Notification Commands

Alert humans:

```json
{
  "type": "notify",
  "action": "message to send",
  "validation": "acknowledgment received"
}
```

## Best Practices

### DO ✅

- Start with read-only checks
- Get explicit approval for destructive operations
- Include rollback for every destructive step
- Set reasonable timeouts
- Log all actions and outcomes
- Test in non-production first
- Document required permissions
- Handle common failure modes
- Notify relevant teams
- Validate final state thoroughly

### DON'T ❌

- Execute destructive operations without validation
- Assume commands will succeed
- Skip error handling
- Run without timeouts
- Forget rollback procedures
- Execute untested automation in production
- Ignore security constraints
- Hide failures or errors
- Continue on critical failures
- Forget to notify on completion

## Validation

Before providing output, verify:

- [ ] Each step has unique ID
- [ ] All commands are syntactically correct
- [ ] Validation criteria are specific
- [ ] Timeouts are reasonable
- [ ] Retry logic makes sense
- [ ] Failure handling is appropriate
- [ ] Rollback procedures exist for destructive operations
- [ ] Notifications are configured
- [ ] Environment variables are documented
- [ ] Required permissions are listed

## Usage

To use this prompt:

1. Provide incident/operation details (see Input Format above)
2. I will generate automated workflow
3. **Review carefully** - never run untested automations
4. Test in safe environment first
5. Validate rollback procedures work
6. Save to `.ctocenter/automations/YYYY-MM-DD-operation-name.json`
7. Execute with appropriate tooling (CI/CD, orchestrator, manual)

## Testing Checklist

Before running in production:

- [ ] Test each step individually
- [ ] Test happy path (all steps succeed)
- [ ] Test failure scenarios (network errors, timeouts)
- [ ] Test rollback procedures
- [ ] Verify notifications work
- [ ] Check logs are captured
- [ ] Validate with different inputs
- [ ] Run in staging environment
- [ ] Document any deviations from expected behavior
- [ ] Get peer review

---

**Prompt Version:** 1.0.0  
**Last Updated:** 2025-11-09  
**Schema:** `.ctocenter/ai/schemas/automation.spec.json`
