#!/usr/bin/env node
/**
 * release-notes.ts
 * 
 * Generates release notes from changelog and project metadata.
 * Creates comprehensive release documentation with highlights and migration notes.
 */

import * as fs from 'fs';
import * as path from 'path';
import { execSync } from 'child_process';

interface ReleaseNotes {
  version: string;
  date: string;
  highlights: string[];
  changes: string;
  migration?: string;
  contributors: string[];
}

class ReleaseNotesGenerator {
  private repoRoot: string;
  private version: string;

  constructor(repoRoot: string, version: string) {
    this.repoRoot = repoRoot;
    this.version = version;
  }

  private getChangelogSection(): string {
    const changelogPath = path.join(this.repoRoot, 'CHANGELOG.md');
    
    if (!fs.existsSync(changelogPath)) {
      console.warn('⚠️  CHANGELOG.md not found');
      return '';
    }

    const content = fs.readFileSync(changelogPath, 'utf-8');
    
    // Extract section for this version
    const versionRegex = new RegExp(`## \\[${this.version.replace(/\./g, '\\.')}\\][^]*?(?=## \\[|$)`, 'g');
    const match = content.match(versionRegex);
    
    return match ? match[0] : '';
  }

  private extractHighlights(changelogSection: string): string[] {
    const highlights: string[] = [];
    
    // Look for features and breaking changes
    const featuresMatch = changelogSection.match(/### ✨ Features\n\n((?:- .+\n?)+)/);
    if (featuresMatch) {
      const features = featuresMatch[1].split('\n').filter(f => f.trim());
      highlights.push(...features.slice(0, 3).map(f => f.replace(/^- /, '')));
    }

    const breakingMatch = changelogSection.match(/### ⚠️ BREAKING CHANGES\n\n((?:- .+\n?)+)/);
    if (breakingMatch) {
      const breaking = breakingMatch[1].split('\n').filter(f => f.trim());
      highlights.push(...breaking.map(b => '⚠️ ' + b.replace(/^- /, '')));
    }

    return highlights;
  }

  private getContributors(): string[] {
    try {
      const gitLog = execSync(
        `git log --format='%an' | sort -u`,
        { cwd: this.repoRoot, encoding: 'utf-8' }
      );
      return gitLog.split('\n').filter(name => name.trim());
    } catch (error) {
      console.warn('Could not get contributors:', error);
      return [];
    }
  }

  private checkForMigrationGuide(): string | undefined {
    const migrationPath = path.join(this.repoRoot, 'docs', 'migration', `v${this.version}.md`);
    
    if (fs.existsSync(migrationPath)) {
      return `See [Migration Guide](./docs/migration/v${this.version}.md) for detailed upgrade instructions.`;
    }
    
    return undefined;
  }

  public generate(): ReleaseNotes {
    console.log(`📋 Generating release notes for v${this.version}...\n`);

    const changelogSection = this.getChangelogSection();
    const highlights = this.extractHighlights(changelogSection);
    const contributors = this.getContributors();
    const migration = this.checkForMigrationGuide();

    return {
      version: this.version,
      date: new Date().toISOString().split('T')[0],
      highlights,
      changes: changelogSection,
      migration,
      contributors
    };
  }

  public formatMarkdown(notes: ReleaseNotes): string {
    let markdown = `# Release Notes: v${notes.version}\n\n`;
    markdown += `**Release Date:** ${notes.date}\n\n`;

    if (notes.highlights.length > 0) {
      markdown += `## 🎉 Highlights\n\n`;
      for (const highlight of notes.highlights) {
        markdown += `${highlight}\n`;
      }
      markdown += '\n';
    }

    markdown += `## 📝 Full Changelog\n\n`;
    markdown += notes.changes + '\n';

    if (notes.migration) {
      markdown += `## 🔄 Migration\n\n`;
      markdown += notes.migration + '\n\n';
    }

    if (notes.contributors.length > 0) {
      markdown += `## 👥 Contributors\n\n`;
      markdown += `Thank you to all ${notes.contributors.length} contributors who made this release possible!\n\n`;
    }

    markdown += `## 📦 Assets\n\n`;
    markdown += `- [Source code (zip)](../../archive/refs/tags/v${notes.version}.zip)\n`;
    markdown += `- [Source code (tar.gz)](../../archive/refs/tags/v${notes.version}.tar.gz)\n\n`;

    markdown += `---\n\n`;
    markdown += `For full documentation, visit the [project repository](../../).\n`;

    return markdown;
  }

  public save(outputPath: string, content: string): void {
    const dir = path.dirname(outputPath);
    if (!fs.existsSync(dir)) {
      fs.mkdirSync(dir, { recursive: true });
    }

    fs.writeFileSync(outputPath, content);
    console.log(`\n💾 Release notes saved to: ${outputPath}`);
  }
}

// Main execution
if (require.main === module) {
  const repoRoot = process.cwd();
  const version = process.argv[2];

  if (!version) {
    console.error('Error: Version number required');
    console.error('Usage: release-notes.ts <version>');
    console.error('Example: release-notes.ts 1.0.0');
    process.exit(1);
  }

  // Remove 'v' prefix if present
  const cleanVersion = version.replace(/^v/, '');

  const generator = new ReleaseNotesGenerator(repoRoot, cleanVersion);
  const notes = generator.generate();
  const content = generator.formatMarkdown(notes);

  console.log('\n' + '='.repeat(80));
  console.log('Generated Release Notes:');
  console.log('='.repeat(80));
  console.log(content);
  console.log('='.repeat(80));

  const outputPath = path.join(repoRoot, 'docs', 'releases', `v${cleanVersion}.md`);
  generator.save(outputPath, content);

  console.log('\n✅ Release notes generation complete');
  console.log(`\nNext steps:`);
  console.log(`1. Review the generated notes at ${outputPath}`);
  console.log(`2. Edit to add any manual highlights or notes`);
  console.log(`3. Create a GitHub release using this content`);
  console.log(`4. Tag the release: git tag -a v${cleanVersion} -m "Release v${cleanVersion}"`);
}

export { ReleaseNotesGenerator, ReleaseNotes };
