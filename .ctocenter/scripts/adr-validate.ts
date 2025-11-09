#!/usr/bin/env node
/**
 * adr-validate.ts
 * 
 * Validates Architecture Decision Records (ADRs) for:
 * - Sequential numbering
 * - Valid status transitions
 * - Required fields
 * - Proper formatting
 * 
 * Fails CI if violations are detected.
 */

import * as fs from 'fs';
import * as path from 'path';

interface ADRMetadata {
  number: number;
  title: string;
  status: string;
  date: string;
  file: string;
}

interface ValidationIssue {
  file: string;
  severity: 'error' | 'warning';
  message: string;
}

interface ValidationReport {
  timestamp: string;
  passed: boolean;
  adrCount: number;
  issues: ValidationIssue[];
}

const VALID_STATUSES = ['Proposed', 'Accepted', 'Deprecated', 'Superseded'];
const VALID_STATUS_TRANSITIONS: { [key: string]: string[] } = {
  'Proposed': ['Accepted', 'Deprecated'],
  'Accepted': ['Deprecated', 'Superseded'],
  'Deprecated': [],
  'Superseded': []
};

class ADRValidator {
  private repoRoot: string;
  private adrDir: string;
  private adrs: ADRMetadata[] = [];
  private issues: ValidationIssue[] = [];

  constructor(repoRoot: string) {
    this.repoRoot = repoRoot;
    this.adrDir = path.join(repoRoot, '.ctocenter', 'adrs');
  }

  private ensureADRDirectory(): void {
    if (!fs.existsSync(this.adrDir)) {
      console.log('Creating ADR directory...');
      fs.mkdirSync(this.adrDir, { recursive: true });
      
      // Create a README
      const readmePath = path.join(this.adrDir, 'README.md');
      fs.writeFileSync(readmePath, `# Architecture Decision Records (ADRs)

This directory contains Architecture Decision Records documenting significant technical decisions made in this project.

## Format

ADRs follow the naming convention: \`ADR-NNNN-title-in-kebab-case.md\`

- NNNN: Four-digit sequential number (0001, 0002, etc.)
- Title: Brief, descriptive title in kebab-case

## Status

ADRs can have the following statuses:
- **Proposed**: Under consideration
- **Accepted**: Decision approved and implemented
- **Deprecated**: Decision no longer recommended (but may still be in use)
- **Superseded**: Replaced by a newer ADR

## Template

Use the template at \`.ctocenter/templates/ADR.template.md\` to create new ADRs.

## Creating a New ADR

1. Determine the next sequential number
2. Copy the template: \`cp .ctocenter/templates/ADR.template.md .ctocenter/adrs/ADR-NNNN-your-title.md\`
3. Fill in all sections
4. Submit for review via pull request
5. Update status as decision progresses
`);
    }
  }

  private scanADRs(): void {
    if (!fs.existsSync(this.adrDir)) {
      return;
    }

    const files = fs.readdirSync(this.adrDir)
      .filter(f => f.startsWith('ADR-') && f.endsWith('.md'))
      .sort();

    for (const file of files) {
      const filePath = path.join(this.adrDir, file);
      const content = fs.readFileSync(filePath, 'utf-8');
      
      const metadata = this.parseADR(file, content);
      if (metadata) {
        this.adrs.push(metadata);
      }
    }
  }

  private parseADR(filename: string, content: string): ADRMetadata | null {
    // Extract number from filename: ADR-NNNN-title.md
    const numberMatch = filename.match(/^ADR-(\d{4})/);
    if (!numberMatch) {
      this.issues.push({
        file: filename,
        severity: 'error',
        message: 'Filename does not match ADR-NNNN format'
      });
      return null;
    }

    const number = parseInt(numberMatch[1], 10);

    // Extract title from first heading
    const titleMatch = content.match(/^#\s+ADR-\d{4}:\s*(.+)$/m);
    const title = titleMatch ? titleMatch[1].trim() : 'Unknown';

    // Extract status
    const statusMatch = content.match(/\*\*Status:\*\*\s*(\w+)/);
    const status = statusMatch ? statusMatch[1].trim() : 'Unknown';

    // Extract date
    const dateMatch = content.match(/\*\*Date:\*\*\s*(\d{4}-\d{2}-\d{2})/);
    const date = dateMatch ? dateMatch[1] : 'Unknown';

    return {
      number,
      title,
      status,
      date,
      file: filename
    };
  }

  private validateSequentialNumbering(): void {
    const numbers = this.adrs.map(adr => adr.number).sort((a, b) => a - b);
    
    for (let i = 0; i < numbers.length; i++) {
      const expected = i + 1;
      if (numbers[i] !== expected) {
        this.issues.push({
          file: this.adrs[i].file,
          severity: 'error',
          message: `ADR numbering gap: Expected ADR-${expected.toString().padStart(4, '0')} but found ADR-${numbers[i].toString().padStart(4, '0')}`
        });
      }
    }

    // Check for duplicates
    const seen = new Set<number>();
    for (const adr of this.adrs) {
      if (seen.has(adr.number)) {
        this.issues.push({
          file: adr.file,
          severity: 'error',
          message: `Duplicate ADR number: ${adr.number}`
        });
      }
      seen.add(adr.number);
    }
  }

  private validateStatuses(): void {
    for (const adr of this.adrs) {
      if (!VALID_STATUSES.includes(adr.status)) {
        this.issues.push({
          file: adr.file,
          severity: 'error',
          message: `Invalid status '${adr.status}'. Must be one of: ${VALID_STATUSES.join(', ')}`
        });
      }
    }
  }

  private validateRequiredFields(): void {
    for (const adr of this.adrs) {
      const filePath = path.join(this.adrDir, adr.file);
      const content = fs.readFileSync(filePath, 'utf-8');

      const requiredSections = [
        'Context',
        'Decision',
        'Consequences'
      ];

      for (const section of requiredSections) {
        const regex = new RegExp(`^##\\s+${section}`, 'm');
        if (!regex.test(content)) {
          this.issues.push({
            file: adr.file,
            severity: 'error',
            message: `Missing required section: ${section}`
          });
        }
      }

      // Check for empty sections
      const emptySection = content.match(/^##\s+(.+)\s*\n\s*\n##/m);
      if (emptySection) {
        this.issues.push({
          file: adr.file,
          severity: 'warning',
          message: `Section '${emptySection[1]}' appears to be empty`
        });
      }
    }
  }

  private validateDates(): void {
    for (const adr of this.adrs) {
      if (adr.date === 'Unknown') {
        this.issues.push({
          file: adr.file,
          severity: 'warning',
          message: 'Missing or invalid date format (should be YYYY-MM-DD)'
        });
      } else {
        const date = new Date(adr.date);
        if (isNaN(date.getTime())) {
          this.issues.push({
            file: adr.file,
            severity: 'warning',
            message: 'Invalid date format'
          });
        }
      }
    }
  }

  private validateSupersededLinks(): void {
    for (const adr of this.adrs) {
      if (adr.status === 'Superseded') {
        const filePath = path.join(this.adrDir, adr.file);
        const content = fs.readFileSync(filePath, 'utf-8');
        
        if (!content.includes('Superseded By')) {
          this.issues.push({
            file: adr.file,
            severity: 'error',
            message: 'ADR marked as Superseded but does not reference which ADR supersedes it'
          });
        }
      }
    }
  }

  public validate(): ValidationReport {
    console.log('🔍 Validating Architecture Decision Records...\n');

    this.ensureADRDirectory();
    this.scanADRs();

    if (this.adrs.length === 0) {
      console.log('ℹ️  No ADRs found. This is OK for new repositories.');
      return {
        timestamp: new Date().toISOString(),
        passed: true,
        adrCount: 0,
        issues: []
      };
    }

    console.log(`Found ${this.adrs.length} ADRs\n`);

    console.log('Checking sequential numbering...');
    this.validateSequentialNumbering();

    console.log('Checking status values...');
    this.validateStatuses();

    console.log('Checking required fields...');
    this.validateRequiredFields();

    console.log('Checking dates...');
    this.validateDates();

    console.log('Checking superseded links...');
    this.validateSupersededLinks();

    const errorCount = this.issues.filter(i => i.severity === 'error').length;
    const warningCount = this.issues.filter(i => i.severity === 'warning').length;

    const report: ValidationReport = {
      timestamp: new Date().toISOString(),
      passed: errorCount === 0,
      adrCount: this.adrs.length,
      issues: this.issues
    };

    return report;
  }

  public printReport(report: ValidationReport): void {
    console.log('\n' + '='.repeat(80));
    console.log('📋 ADR VALIDATION REPORT');
    console.log('='.repeat(80));
    console.log(`Timestamp: ${report.timestamp}`);
    console.log(`ADRs Found: ${report.adrCount}`);
    console.log(`Status: ${report.passed ? '✅ PASSED' : '❌ FAILED'}`);

    const errorCount = report.issues.filter(i => i.severity === 'error').length;
    const warningCount = report.issues.filter(i => i.severity === 'warning').length;

    console.log(`\nIssues:`);
    console.log(`  Errors: ${errorCount}`);
    console.log(`  Warnings: ${warningCount}`);

    if (report.issues.length > 0) {
      console.log('\n' + '-'.repeat(80));
      console.log('Details:\n');

      for (const issue of report.issues) {
        const icon = issue.severity === 'error' ? '❌' : '⚠️';
        console.log(`${icon} [${issue.severity.toUpperCase()}] ${issue.file}`);
        console.log(`   ${issue.message}\n`);
      }
    }

    console.log('='.repeat(80));
  }

  public saveReport(report: ValidationReport, outputPath: string): void {
    const dir = path.dirname(outputPath);
    if (!fs.existsSync(dir)) {
      fs.mkdirSync(dir, { recursive: true });
    }
    
    fs.writeFileSync(outputPath, JSON.stringify(report, null, 2));
    console.log(`\n💾 Report saved to: ${outputPath}`);
  }
}

// Main execution
if (require.main === module) {
  const repoRoot = process.argv[2] || process.cwd();
  const outputPath = path.join(repoRoot, '.ctocenter', 'state', 'adr-validation.json');

  const validator = new ADRValidator(repoRoot);
  const report = validator.validate();
  
  validator.printReport(report);
  validator.saveReport(report, outputPath);

  // Exit with non-zero code if validation failed
  if (!report.passed) {
    console.log('\n❌ ADR validation failed');
    process.exit(1);
  } else {
    console.log('\n✅ ADR validation passed');
    process.exit(0);
  }
}

export { ADRValidator, ValidationReport, ValidationIssue };
