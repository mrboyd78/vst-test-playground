#!/usr/bin/env node
/**
 * changelog.ts
 * 
 * Generates changelog from git commits and GitHub metadata.
 * Groups commits by type (feat, fix, docs, etc.) and formats for release notes.
 */

import * as fs from 'fs';
import * as path from 'path';
import { execSync } from 'child_process';

interface Commit {
  hash: string;
  date: string;
  author: string;
  message: string;
  type: string;
  scope?: string;
  breaking: boolean;
}

interface ChangelogSection {
  title: string;
  commits: Commit[];
}

class ChangelogGenerator {
  private repoRoot: string;
  private fromTag?: string;
  private toTag?: string;

  constructor(repoRoot: string, fromTag?: string, toTag?: string) {
    this.repoRoot = repoRoot;
    this.fromTag = fromTag;
    this.toTag = toTag || 'HEAD';
  }

  private getCommits(): Commit[] {
    const range = this.fromTag ? `${this.fromTag}..${this.toTag}` : this.toTag;
    
    let gitLog: string;
    try {
      gitLog = execSync(
        `git log ${range} --pretty=format:"%H|%ad|%an|%s" --date=short`,
        { cwd: this.repoRoot, encoding: 'utf-8' }
      );
    } catch (error) {
      console.error('Error getting git log:', error);
      return [];
    }

    const commits: Commit[] = [];
    const lines = gitLog.split('\n').filter(line => line.trim());

    for (const line of lines) {
      const [hash, date, author, message] = line.split('|');
      const commit = this.parseCommit(hash, date, author, message);
      commits.push(commit);
    }

    return commits;
  }

  private parseCommit(hash: string, date: string, author: string, message: string): Commit {
    // Parse conventional commit format: type(scope): message
    const conventionalMatch = message.match(/^(\w+)(?:\(([^)]+)\))?:\s*(.+)$/);
    
    let type = 'other';
    let scope: string | undefined;
    let actualMessage = message;
    let breaking = false;

    if (conventionalMatch) {
      type = conventionalMatch[1].toLowerCase();
      scope = conventionalMatch[2];
      actualMessage = conventionalMatch[3];
    }

    // Check for breaking changes
    if (message.includes('BREAKING CHANGE') || message.includes('!:')) {
      breaking = true;
    }

    return {
      hash,
      date,
      author,
      message: actualMessage,
      type,
      scope,
      breaking
    };
  }

  private groupCommits(commits: Commit[]): ChangelogSection[] {
    const sections: { [key: string]: Commit[] } = {
      breaking: [],
      feat: [],
      fix: [],
      perf: [],
      refactor: [],
      docs: [],
      test: [],
      build: [],
      ci: [],
      chore: [],
      other: []
    };

    for (const commit of commits) {
      if (commit.breaking) {
        sections.breaking.push(commit);
      } else if (sections[commit.type]) {
        sections[commit.type].push(commit);
      } else {
        sections.other.push(commit);
      }
    }

    const result: ChangelogSection[] = [];
    const titles: { [key: string]: string } = {
      breaking: '⚠️ BREAKING CHANGES',
      feat: '✨ Features',
      fix: '🐛 Bug Fixes',
      perf: '⚡ Performance',
      refactor: '♻️ Code Refactoring',
      docs: '📝 Documentation',
      test: '✅ Tests',
      build: '🏗️ Build System',
      ci: '👷 CI/CD',
      chore: '🔧 Chores',
      other: '📦 Other Changes'
    };

    for (const [key, commits] of Object.entries(sections)) {
      if (commits.length > 0) {
        result.push({
          title: titles[key] || key,
          commits
        });
      }
    }

    return result;
  }

  private formatMarkdown(sections: ChangelogSection[]): string {
    const version = this.toTag === 'HEAD' ? 'Unreleased' : this.toTag;
    const date = new Date().toISOString().split('T')[0];
    
    let markdown = `## [${version}] - ${date}\n\n`;

    for (const section of sections) {
      markdown += `### ${section.title}\n\n`;
      
      for (const commit of section.commits) {
        const scope = commit.scope ? `**${commit.scope}:** ` : '';
        const hash = commit.hash.substring(0, 7);
        markdown += `- ${scope}${commit.message} ([${hash}](../../commit/${commit.hash}))\n`;
      }
      
      markdown += '\n';
    }

    return markdown;
  }

  public generate(): string {
    console.log('📝 Generating changelog...\n');
    
    const commits = this.getCommits();
    console.log(`Found ${commits.length} commits`);

    if (commits.length === 0) {
      return '## No changes\n\n';
    }

    const sections = this.groupCommits(commits);
    const markdown = this.formatMarkdown(sections);

    return markdown;
  }

  public save(outputPath: string, content: string): void {
    const dir = path.dirname(outputPath);
    if (!fs.existsSync(dir)) {
      fs.mkdirSync(dir, { recursive: true });
    }

    // If CHANGELOG.md exists, prepend new content
    if (fs.existsSync(outputPath)) {
      const existing = fs.readFileSync(outputPath, 'utf-8');
      // Insert new content after the header
      const headerMatch = existing.match(/^#[^\n]+\n+/);
      if (headerMatch) {
        const header = headerMatch[0];
        const rest = existing.substring(header.length);
        content = header + content + rest;
      } else {
        content = content + '\n' + existing;
      }
    } else {
      // Create new CHANGELOG.md with header
      content = `# Changelog\n\nAll notable changes to this project will be documented in this file.\n\nThe format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),\nand this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).\n\n${content}`;
    }

    fs.writeFileSync(outputPath, content);
    console.log(`\n💾 Changelog saved to: ${outputPath}`);
  }
}

// Main execution
if (require.main === module) {
  const repoRoot = process.cwd();
  const args = process.argv.slice(2);
  
  let fromTag: string | undefined;
  let toTag: string | undefined;
  
  // Parse arguments: --from <tag> --to <tag>
  for (let i = 0; i < args.length; i++) {
    if (args[i] === '--from' && i + 1 < args.length) {
      fromTag = args[i + 1];
      i++;
    } else if (args[i] === '--to' && i + 1 < args.length) {
      toTag = args[i + 1];
      i++;
    }
  }

  const generator = new ChangelogGenerator(repoRoot, fromTag, toTag);
  const content = generator.generate();
  
  console.log('\n' + '='.repeat(80));
  console.log('Generated Changelog:');
  console.log('='.repeat(80));
  console.log(content);
  console.log('='.repeat(80));

  const outputPath = path.join(repoRoot, 'CHANGELOG.md');
  generator.save(outputPath, content);
  
  console.log('\n✅ Changelog generation complete');
}

export { ChangelogGenerator };
