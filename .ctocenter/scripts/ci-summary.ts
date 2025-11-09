#!/usr/bin/env node
/**
 * ci-summary.ts
 * 
 * Generates CI build summaries with key metrics and status.
 * Useful for GitHub Actions summary output.
 */

import * as fs from 'fs';
import * as path from 'path';

interface CISummary {
  timestamp: string;
  build: {
    status: 'success' | 'failure' | 'pending';
    duration?: number;
    commit: string;
    branch: string;
  };
  tests?: {
    total: number;
    passed: number;
    failed: number;
    skipped: number;
  };
  coverage?: {
    lines: number;
    branches: number;
    functions: number;
  };
  artifacts?: {
    name: string;
    size: number;
    path: string;
  }[];
  warnings: string[];
  errors: string[];
}

class CISummaryGenerator {
  private repoRoot: string;
  private summary: CISummary;

  constructor(repoRoot: string) {
    this.repoRoot = repoRoot;
    this.summary = {
      timestamp: new Date().toISOString(),
      build: {
        status: 'pending',
        commit: this.getCommitHash(),
        branch: this.getBranch()
      },
      warnings: [],
      errors: []
    };
  }

  private getCommitHash(): string {
    try {
      const { execSync } = require('child_process');
      return execSync('git rev-parse HEAD', { 
        cwd: this.repoRoot, 
        encoding: 'utf-8' 
      }).trim();
    } catch {
      return 'unknown';
    }
  }

  private getBranch(): string {
    try {
      const { execSync } = require('child_process');
      return execSync('git branch --show-current', { 
        cwd: this.repoRoot, 
        encoding: 'utf-8' 
      }).trim();
    } catch {
      return 'unknown';
    }
  }

  public setBuildStatus(status: 'success' | 'failure' | 'pending', duration?: number): void {
    this.summary.build.status = status;
    if (duration) {
      this.summary.build.duration = duration;
    }
  }

  public setTestResults(total: number, passed: number, failed: number, skipped: number): void {
    this.summary.tests = { total, passed, failed, skipped };
  }

  public setCoverage(lines: number, branches: number, functions: number): void {
    this.summary.coverage = { lines, branches, functions };
  }

  public addArtifact(name: string, path: string, size: number): void {
    if (!this.summary.artifacts) {
      this.summary.artifacts = [];
    }
    this.summary.artifacts.push({ name, path, size });
  }

  public addWarning(warning: string): void {
    this.summary.warnings.push(warning);
  }

  public addError(error: string): void {
    this.summary.errors.push(error);
  }

  private formatBytes(bytes: number): string {
    if (bytes === 0) return '0 B';
    const k = 1024;
    const sizes = ['B', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return Math.round(bytes / Math.pow(k, i) * 100) / 100 + ' ' + sizes[i];
  }

  private formatDuration(seconds: number): string {
    if (seconds < 60) {
      return `${seconds}s`;
    }
    const minutes = Math.floor(seconds / 60);
    const secs = seconds % 60;
    return `${minutes}m ${secs}s`;
  }

  public generateMarkdown(): string {
    const statusEmoji = {
      success: '✅',
      failure: '❌',
      pending: '⏳'
    };

    let md = '# CI Build Summary\n\n';
    
    // Build status
    md += `## ${statusEmoji[this.summary.build.status]} Build Status: ${this.summary.build.status.toUpperCase()}\n\n`;
    md += `- **Commit:** \`${this.summary.build.commit.substring(0, 7)}\`\n`;
    md += `- **Branch:** \`${this.summary.build.branch}\`\n`;
    md += `- **Timestamp:** ${this.summary.timestamp}\n`;
    if (this.summary.build.duration) {
      md += `- **Duration:** ${this.formatDuration(this.summary.build.duration)}\n`;
    }
    md += '\n';

    // Test results
    if (this.summary.tests) {
      const { total, passed, failed, skipped } = this.summary.tests;
      const passRate = total > 0 ? Math.round((passed / total) * 100) : 0;
      
      md += '## 🧪 Test Results\n\n';
      md += `| Total | Passed | Failed | Skipped | Pass Rate |\n`;
      md += `|-------|--------|--------|---------|----------|\n`;
      md += `| ${total} | ${passed} ✅ | ${failed} ❌ | ${skipped} ⏭️ | ${passRate}% |\n\n`;
    }

    // Coverage
    if (this.summary.coverage) {
      const { lines, branches, functions } = this.summary.coverage;
      
      md += '## 📊 Code Coverage\n\n';
      md += `| Metric | Coverage |\n`;
      md += `|--------|----------|\n`;
      md += `| Lines | ${lines}% |\n`;
      md += `| Branches | ${branches}% |\n`;
      md += `| Functions | ${functions}% |\n\n`;
    }

    // Artifacts
    if (this.summary.artifacts && this.summary.artifacts.length > 0) {
      md += '## 📦 Build Artifacts\n\n';
      for (const artifact of this.summary.artifacts) {
        md += `- **${artifact.name}** (${this.formatBytes(artifact.size)})\n`;
        md += `  - Path: \`${artifact.path}\`\n`;
      }
      md += '\n';
    }

    // Warnings
    if (this.summary.warnings.length > 0) {
      md += '## ⚠️ Warnings\n\n';
      for (const warning of this.summary.warnings) {
        md += `- ${warning}\n`;
      }
      md += '\n';
    }

    // Errors
    if (this.summary.errors.length > 0) {
      md += '## ❌ Errors\n\n';
      for (const error of this.summary.errors) {
        md += `- ${error}\n`;
      }
      md += '\n';
    }

    return md;
  }

  public save(outputPath: string): void {
    const dir = path.dirname(outputPath);
    if (!fs.existsSync(dir)) {
      fs.mkdirSync(dir, { recursive: true });
    }

    const markdown = this.generateMarkdown();
    fs.writeFileSync(outputPath, markdown);
    console.log(`CI summary saved to: ${outputPath}`);

    // Also save JSON
    const jsonPath = outputPath.replace(/\.md$/, '.json');
    fs.writeFileSync(jsonPath, JSON.stringify(this.summary, null, 2));
    console.log(`CI summary JSON saved to: ${jsonPath}`);
  }

  public getSummary(): CISummary {
    return this.summary;
  }
}

// Main execution
if (require.main === module) {
  const repoRoot = process.cwd();
  
  // Example usage
  const generator = new CISummaryGenerator(repoRoot);
  
  // These would typically be populated from CI environment or test results
  generator.setBuildStatus('success', 180);
  generator.setTestResults(42, 40, 2, 0);
  generator.setCoverage(85, 78, 90);
  
  // Add build artifacts (example)
  const buildDir = path.join(repoRoot, 'build');
  if (fs.existsSync(buildDir)) {
    try {
      const files = fs.readdirSync(buildDir);
      for (const file of files) {
        const filePath = path.join(buildDir, file);
        const stats = fs.statSync(filePath);
        if (stats.isFile()) {
          generator.addArtifact(file, filePath, stats.size);
        }
      }
    } catch (error) {
      generator.addWarning('Could not read build artifacts');
    }
  }

  const markdown = generator.generateMarkdown();
  console.log(markdown);

  const outputPath = path.join(repoRoot, '.ctocenter', 'state', 'ci-summary.md');
  generator.save(outputPath);

  console.log('\n✅ CI summary generation complete');
}

export { CISummaryGenerator, CISummary };
