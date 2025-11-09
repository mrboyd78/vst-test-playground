# Runbook: [Service/System Name] - [Issue Type]

**Status:** Active | Deprecated  
**Severity:** Critical | High | Medium | Low  
**Owner:** [Team/Person]  
**Last Updated:** YYYY-MM-DD

## Overview

**Symptoms:**
- [Observable symptom 1]
- [Observable symptom 2]

**Impact:**
- [Business impact]
- [User impact]
- [System impact]

## Triage

### Quick Checks

1. **Check [System/Service] Status**
   ```bash
   # Command to check status
   ```
   Expected: [What you should see]

2. **Check [Dependencies]**
   ```bash
   # Command to check dependencies
   ```
   Expected: [What you should see]

### Escalation Criteria

Escalate if:
- [ ] [Condition that requires escalation]
- [ ] [Condition that requires escalation]

**Escalation Contact:** [Team/Person/Channel]

## Resolution Steps

### Immediate Actions (0-5 minutes)

1. **[Action Name]**
   ```bash
   # Command or procedure
   ```
   **Expected Result:** [What should happen]

2. **[Action Name]**
   ```bash
   # Command or procedure
   ```
   **Expected Result:** [What should happen]

### Investigation (5-15 minutes)

1. **Check Logs**
   ```bash
   # Log location and grep commands
   ```
   **Look for:** [Error patterns or indicators]

2. **Check Metrics**
   - Dashboard: [Link to monitoring dashboard]
   - Key metrics: [Specific metrics to check]

### Remediation

1. **[Remediation Step 1]**
   ```bash
   # Commands or procedures
   ```
   **Verification:**
   ```bash
   # How to verify fix worked
   ```

2. **[Remediation Step 2]**
   ```bash
   # Commands or procedures
   ```
   **Verification:**
   ```bash
   # How to verify fix worked
   ```

## Post-Incident

### Verification Checklist

- [ ] Service is responding normally
- [ ] Metrics have returned to baseline
- [ ] No errors in logs
- [ ] Users can access the system

### Communication

- [ ] Update status page
- [ ] Notify stakeholders
- [ ] Document in incident tracker

### Follow-up

- [ ] Create postmortem document
- [ ] File bugs for root cause fixes
- [ ] Update runbook if needed

## Related Documents

- **Playbooks:** [Links to related playbooks]
- **ADRs:** [Links to architecture decisions]
- **Postmortems:** [Links to similar incidents]

## Changelog

| Date | Author | Changes |
|------|--------|---------|
| YYYY-MM-DD | [Name] | Initial version |
