---
name: brain
description: >-
  Conventions for adding, editing, or deleting notes in brain/. Triggers: "add to brain",
  edits under brain/.
---

# Brain

Persistent memory across sessions. Obsidian vault at `brain/`.

Every agent and session reads the brain, so a wrong or speculative note misleads all of them. Write a note only when it changes how a later task would be done.

## Before Writing

Read `brain/index.md` first. Then read the relevant entrypoint for your topic:

- `brain/principles.md` for principle updates

For directories without a dedicated index file yet, scan nearby files directly and edit an existing note when possible.

## Structure

```
brain/
├── index.md              <- root entry point, links to everything
├── principles.md         <- index for principles/
├── principles/           <- engineering and design principles
├── codebase/             <- project-specific knowledge and gotchas
└── plans/                <- feature plans
```

**Rules:**

- One topic per file. `brain/codebase/deploy-gotchas.md`, not a mega-file.
- Maintain existing index entrypoints: `brain/index.md`, `brain/principles.md`.
- If you introduce a new top-level category, add an index-style entrypoint for it (links only, no inlined content).
- `brain/index.md` is the root. Every brain file must be reachable from it.
- File names: lowercase, hyphenated. `worktree-gotchas.md`.

## Wikilinks

Format: `[[section/file-name]]`. Resolution order: same directory, then relative path, then vault root. Heading anchors (`[[file#heading]]`) are stripped during resolution.

## Writing Style

- Bullets over prose. No preamble. Each bullet is a full sentence or more, following the writing rules in `~/.claude/CLAUDE.md`: no verbless fragments, no verbs of volition on things, and the reason beside the fact.
- Plain markdown with `# Title`. No Obsidian frontmatter.
- Keep notes under ~50 lines. Split if longer.

## After Writing

The PostToolUse hook (`.claude/hooks/auto-index-brain.sh`) rebuilds `brain/index.md` when a note is added or removed. Update `brain/codebase.md` and `brain/principles.md` by hand, and keep them to links.

## Durability Test

Ask: "Would I include this in a prompt for a *different* task?"

- **Yes** -> write to `brain/`. It's durable knowledge.
- **No, it's plan-specific** -> update the plan's docs instead.
- **No, it's a skill issue** -> update the skill file directly.
- **No, it needs follow-up work** -> fix a bug now, with a test. Put any other follow-up in your final report.

## Maintenance

- Delete outdated or subsumed notes.
- Merge overlapping notes before adding new ones.
