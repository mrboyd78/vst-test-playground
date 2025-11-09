#!/usr/bin/env node
/**
 * toc-sync.ts
 * 
 * Synchronizes table of contents in markdown files.
 * Generates and updates TOC based on headings.
 */

import * as fs from 'fs';
import * as path from 'path';

interface Heading {
  level: number;
  text: string;
  slug: string;
}

class TOCSync {
  private repoRoot: string;

  constructor(repoRoot: string) {
    this.repoRoot = repoRoot;
  }

  private slugify(text: string): string {
    return text
      .toLowerCase()
      .replace(/[^\w\s-]/g, '')
      .replace(/\s+/g, '-')
      .replace(/-+/g, '-')
      .trim();
  }

  private extractHeadings(content: string): Heading[] {
    const headings: Heading[] = [];
    const lines = content.split('\n');

    for (const line of lines) {
      const match = line.match(/^(#{1,6})\s+(.+)$/);
      if (match) {
        const level = match[1].length;
        const text = match[2].trim();
        
        // Skip the main title (h1) from TOC
        if (level === 1) continue;
        
        headings.push({
          level,
          text,
          slug: this.slugify(text)
        });
      }
    }

    return headings;
  }

  private generateTOC(headings: Heading[]): string {
    if (headings.length === 0) {
      return '';
    }

    let toc = '## Table of Contents\n\n';

    for (const heading of headings) {
      const indent = '  '.repeat(heading.level - 2);
      toc += `${indent}- [${heading.text}](#${heading.slug})\n`;
    }

    return toc + '\n';
  }

  private updateTOC(content: string, newTOC: string): string {
    // Check if TOC markers exist
    const tocStartMarker = '<!-- TOC -->';
    const tocEndMarker = '<!-- /TOC -->';
    
    const hasMarkers = content.includes(tocStartMarker) && content.includes(tocEndMarker);

    if (hasMarkers) {
      // Replace existing TOC
      const regex = new RegExp(
        `${tocStartMarker}[\\s\\S]*?${tocEndMarker}`,
        'g'
      );
      return content.replace(regex, `${tocStartMarker}\n${newTOC}${tocEndMarker}`);
    }

    // Check for "## Table of Contents" heading
    const tocHeadingRegex = /^## Table of Contents\n\n(?:(?!^##)[\s\S])*?\n/m;
    if (tocHeadingRegex.test(content)) {
      return content.replace(tocHeadingRegex, newTOC);
    }

    // No TOC found, insert after first heading
    const firstHeadingMatch = content.match(/^#[^\n]+\n+/);
    if (firstHeadingMatch) {
      const insertPos = firstHeadingMatch[0].length;
      return content.slice(0, insertPos) + 
             `${tocStartMarker}\n${newTOC}${tocEndMarker}\n\n` +
             content.slice(insertPos);
    }

    // Prepend to content if no heading found
    return `${tocStartMarker}\n${newTOC}${tocEndMarker}\n\n${content}`;
  }

  public processFile(filePath: string): boolean {
    console.log(`Processing: ${path.relative(this.repoRoot, filePath)}`);

    if (!fs.existsSync(filePath)) {
      console.error(`File not found: ${filePath}`);
      return false;
    }

    const content = fs.readFileSync(filePath, 'utf-8');
    const headings = this.extractHeadings(content);

    if (headings.length < 2) {
      console.log('  ↳ Skipped (too few headings for TOC)');
      return true;
    }

    const newTOC = this.generateTOC(headings);
    const updatedContent = this.updateTOC(content, newTOC);

    if (updatedContent === content) {
      console.log('  ↳ No changes needed');
      return true;
    }

    fs.writeFileSync(filePath, updatedContent);
    console.log('  ↳ Updated ✓');
    return true;
  }

  public processDirectory(dirPath: string, recursive: boolean = true): number {
    let filesProcessed = 0;

    if (!fs.existsSync(dirPath)) {
      console.error(`Directory not found: ${dirPath}`);
      return 0;
    }

    const entries = fs.readdirSync(dirPath, { withFileTypes: true });

    for (const entry of entries) {
      const fullPath = path.join(dirPath, entry.name);

      if (entry.isDirectory() && recursive) {
        if (entry.name !== 'node_modules' && !entry.name.startsWith('.')) {
          filesProcessed += this.processDirectory(fullPath, recursive);
        }
      } else if (entry.isFile() && entry.name.endsWith('.md')) {
        if (this.processFile(fullPath)) {
          filesProcessed++;
        }
      }
    }

    return filesProcessed;
  }
}

// Main execution
if (require.main === module) {
  const repoRoot = process.cwd();
  const args = process.argv.slice(2);

  if (args.length === 0) {
    console.log('📚 TOC Sync - Table of Contents Synchronizer\n');
    console.log('Usage:');
    console.log('  toc-sync.ts <file.md>           - Process single file');
    console.log('  toc-sync.ts <directory>         - Process all .md files in directory');
    console.log('  toc-sync.ts --all               - Process all docs/**/*.md files');
    console.log('\nExamples:');
    console.log('  toc-sync.ts README.md');
    console.log('  toc-sync.ts docs/guides');
    console.log('  toc-sync.ts --all');
    process.exit(0);
  }

  const sync = new TOCSync(repoRoot);

  console.log('📚 Synchronizing Table of Contents...\n');

  let filesProcessed = 0;

  if (args[0] === '--all') {
    const docsDir = path.join(repoRoot, 'docs');
    filesProcessed = sync.processDirectory(docsDir, true);
    
    // Also process root README
    const readmePath = path.join(repoRoot, 'README.md');
    if (fs.existsSync(readmePath)) {
      if (sync.processFile(readmePath)) {
        filesProcessed++;
      }
    }
  } else {
    const targetPath = path.resolve(repoRoot, args[0]);
    const stat = fs.statSync(targetPath);

    if (stat.isDirectory()) {
      filesProcessed = sync.processDirectory(targetPath, true);
    } else if (stat.isFile()) {
      if (sync.processFile(targetPath)) {
        filesProcessed++;
      }
    }
  }

  console.log(`\n✅ Processed ${filesProcessed} file(s)`);
  console.log('\nTOC synchronization complete!');
}

export { TOCSync };
