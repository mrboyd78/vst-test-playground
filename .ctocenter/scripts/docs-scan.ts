#!/usr/bin/env node
/**
 * docs-scan.ts
 * 
 * Scans the repository for documentation drift by comparing:
 * - Code structure against documentation
 * - API changes against reference docs
 * - Configuration changes against guides
 * 
 * Outputs machine-readable JSON and exits with non-zero code if drift exists.
 */

import * as fs from 'fs';
import * as path from 'path';

interface ProjectMap {
  timestamp: string;
  repository: string;
  structure: {
    directories: string[];
    files: string[];
  };
  frameworks: string[];
  buildSystem: string | null;
  packageManager: string | null;
  languages: string[];
  packages: any[];
  apps: string[];
  infrastructure: string[];
  documentation: string[];
}

interface DriftIssue {
  type: 'missing_doc' | 'outdated_doc' | 'orphaned_doc' | 'structural_change';
  severity: 'critical' | 'high' | 'medium' | 'low';
  file?: string;
  description: string;
  recommendation: string;
}

interface DriftReport {
  timestamp: string;
  hasDrift: boolean;
  issues: DriftIssue[];
  summary: {
    total: number;
    critical: number;
    high: number;
    medium: number;
    low: number;
  };
}

class DocsScanner {
  private repoRoot: string;
  private projectMap: ProjectMap | null = null;
  private issues: DriftIssue[] = [];

  constructor(repoRoot: string) {
    this.repoRoot = repoRoot;
  }

  private loadProjectMap(): boolean {
    const mapPath = path.join(this.repoRoot, '.ctocenter', 'state', 'project-map.json');
    
    if (!fs.existsSync(mapPath)) {
      console.error('Error: project-map.json not found. Run repository scan first.');
      return false;
    }

    try {
      const content = fs.readFileSync(mapPath, 'utf-8');
      this.projectMap = JSON.parse(content);
      return true;
    } catch (error) {
      console.error('Error reading project-map.json:', error);
      return false;
    }
  }

  private checkCoreDocumentation(): void {
    const requiredDocs = [
      { path: 'README.md', severity: 'critical' as const },
      { path: 'docs/guides/DEVELOPMENT.md', severity: 'high' as const },
      { path: 'docs/guides/TESTING.md', severity: 'high' as const },
      { path: 'docs/reference/REFERENCE.md', severity: 'medium' as const },
    ];

    for (const doc of requiredDocs) {
      const fullPath = path.join(this.repoRoot, doc.path);
      if (!fs.existsSync(fullPath)) {
        this.issues.push({
          type: 'missing_doc',
          severity: doc.severity,
          file: doc.path,
          description: `Required documentation file is missing: ${doc.path}`,
          recommendation: `Create ${doc.path} using appropriate template`
        });
      }
    }
  }

  private checkSourceCodeDocumentation(): void {
    if (!this.projectMap) return;

    const sourceFiles = this.projectMap.structure.files.filter(
      f => f.startsWith('Source/') && (f.endsWith('.cpp') || f.endsWith('.h'))
    );

    // Check if there's reference documentation for main components
    const coreComponents = ['PluginProcessor', 'PluginEditor', 'WebView'];
    
    for (const component of coreComponents) {
      const hasSourceFiles = sourceFiles.some(f => f.includes(component));
      const hasDocumentation = this.projectMap.documentation.some(
        doc => doc.toLowerCase().includes(component.toLowerCase())
      );

      if (hasSourceFiles && !hasDocumentation) {
        this.issues.push({
          type: 'missing_doc',
          severity: 'medium',
          file: `docs/reference/${component}.md`,
          description: `Core component ${component} exists but lacks reference documentation`,
          recommendation: `Create reference documentation for ${component} in docs/reference/`
        });
      }
    }
  }

  private checkBuildSystemDocumentation(): void {
    if (!this.projectMap) return;

    const hasCMake = this.projectMap.buildSystem === 'CMake';
    const hasCMakeDoc = this.projectMap.documentation.some(
      doc => doc.toLowerCase().includes('cmake') || doc.toLowerCase().includes('build')
    );

    if (hasCMake && !hasCMakeDoc) {
      this.issues.push({
        type: 'missing_doc',
        severity: 'high',
        file: 'docs/guides/BUILD.md',
        description: 'CMake build system is used but build documentation is minimal',
        recommendation: 'Create comprehensive build guide in docs/guides/BUILD.md'
      });
    }
  }

  private checkStructuralChanges(): void {
    if (!this.projectMap) return;

    // Check if new directories have been added that might need documentation
    const potentialAppDirs = this.projectMap.structure.directories.filter(
      dir => ['src', 'lib', 'packages', 'modules', 'components'].some(
        pattern => dir.toLowerCase().includes(pattern)
      )
    );

    for (const dir of potentialAppDirs) {
      const dirName = path.basename(dir);
      const hasDoc = this.projectMap.documentation.some(
        doc => doc.toLowerCase().includes(dirName.toLowerCase())
      );

      if (!hasDoc && !dir.includes('node_modules') && !dir.includes('build')) {
        this.issues.push({
          type: 'missing_doc',
          severity: 'low',
          description: `Directory ${dir} appears to contain code but lacks documentation`,
          recommendation: `Consider adding documentation for ${dir} structure and purpose`
        });
      }
    }
  }

  private checkDocumentationFreshness(): void {
    if (!this.projectMap) return;

    const docFiles = this.projectMap.documentation;
    const now = new Date();
    const sixMonthsAgo = new Date(now.getTime() - 180 * 24 * 60 * 60 * 1000);

    for (const docPath of docFiles) {
      const fullPath = path.join(this.repoRoot, docPath);
      
      if (!fs.existsSync(fullPath)) {
        this.issues.push({
          type: 'orphaned_doc',
          severity: 'low',
          file: docPath,
          description: `Documentation file referenced but not found: ${docPath}`,
          recommendation: 'Remove reference or restore file'
        });
        continue;
      }

      try {
        const stats = fs.statSync(fullPath);
        if (stats.mtime < sixMonthsAgo) {
          this.issues.push({
            type: 'outdated_doc',
            severity: 'low',
            file: docPath,
            description: `Documentation may be stale (not updated in 6+ months): ${docPath}`,
            recommendation: 'Review and update if changes have occurred'
          });
        }
      } catch (error) {
        // Ignore stat errors
      }
    }
  }

  private checkADRs(): void {
    const adrDir = path.join(this.repoRoot, '.ctocenter', 'adrs');
    
    if (!fs.existsSync(adrDir)) {
      this.issues.push({
        type: 'missing_doc',
        severity: 'medium',
        description: 'No ADR (Architecture Decision Records) directory found',
        recommendation: 'Create .ctocenter/adrs/ directory and start documenting key decisions'
      });
    }
  }

  public scan(): DriftReport {
    console.log('🔍 Scanning repository for documentation drift...\n');

    if (!this.loadProjectMap()) {
      process.exit(1);
    }

    console.log('Checking core documentation...');
    this.checkCoreDocumentation();

    console.log('Checking source code documentation...');
    this.checkSourceCodeDocumentation();

    console.log('Checking build system documentation...');
    this.checkBuildSystemDocumentation();

    console.log('Checking for structural changes...');
    this.checkStructuralChanges();

    console.log('Checking documentation freshness...');
    this.checkDocumentationFreshness();

    console.log('Checking ADRs...');
    this.checkADRs();

    const summary = {
      total: this.issues.length,
      critical: this.issues.filter(i => i.severity === 'critical').length,
      high: this.issues.filter(i => i.severity === 'high').length,
      medium: this.issues.filter(i => i.severity === 'medium').length,
      low: this.issues.filter(i => i.severity === 'low').length,
    };

    const report: DriftReport = {
      timestamp: new Date().toISOString(),
      hasDrift: this.issues.length > 0,
      issues: this.issues,
      summary
    };

    return report;
  }

  public printReport(report: DriftReport): void {
    console.log('\n' + '='.repeat(80));
    console.log('📊 DOCUMENTATION DRIFT REPORT');
    console.log('='.repeat(80));
    console.log(`Timestamp: ${report.timestamp}`);
    console.log(`Status: ${report.hasDrift ? '⚠️  DRIFT DETECTED' : '✅ NO DRIFT'}`);
    console.log('\nSummary:');
    console.log(`  Total Issues: ${report.summary.total}`);
    console.log(`  Critical: ${report.summary.critical}`);
    console.log(`  High: ${report.summary.high}`);
    console.log(`  Medium: ${report.summary.medium}`);
    console.log(`  Low: ${report.summary.low}`);

    if (report.issues.length > 0) {
      console.log('\n' + '-'.repeat(80));
      console.log('Issues Found:\n');

      for (const issue of report.issues) {
        const icon = {
          critical: '🔴',
          high: '🟠',
          medium: '🟡',
          low: '🔵'
        }[issue.severity];

        console.log(`${icon} [${issue.severity.toUpperCase()}] ${issue.type}`);
        if (issue.file) {
          console.log(`   File: ${issue.file}`);
        }
        console.log(`   Description: ${issue.description}`);
        console.log(`   Recommendation: ${issue.recommendation}`);
        console.log('');
      }
    }

    console.log('='.repeat(80));
  }

  public saveReport(report: DriftReport, outputPath: string): void {
    fs.writeFileSync(outputPath, JSON.stringify(report, null, 2));
    console.log(`\n💾 Report saved to: ${outputPath}`);
  }
}

// Main execution
if (require.main === module) {
  const repoRoot = process.argv[2] || process.cwd();
  const outputPath = path.join(repoRoot, '.ctocenter', 'state', 'drift-report.json');

  const scanner = new DocsScanner(repoRoot);
  const report = scanner.scan();
  
  scanner.printReport(report);
  scanner.saveReport(report, outputPath);

  // Exit with non-zero code if drift detected (for CI)
  if (report.hasDrift && report.summary.critical > 0 || report.summary.high > 0) {
    console.log('\n❌ Drift detection failed (critical or high severity issues found)');
    process.exit(1);
  } else if (report.hasDrift) {
    console.log('\n⚠️  Documentation drift detected but only low/medium severity');
    process.exit(0);
  } else {
    console.log('\n✅ No documentation drift detected');
    process.exit(0);
  }
}

export { DocsScanner, DriftReport, DriftIssue };
