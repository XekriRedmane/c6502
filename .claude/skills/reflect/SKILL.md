---
name: reflect
description: >-
  Saves this session's mistakes, corrections, and codebase knowledge to c6502's brain/
  vault or its skills. Subagents run it before handing back. Triggers: "reflect",
  "remember this".
---

# Reflect

Review the conversation and persist learnings — to `brain/`, to skill files, or as structural enforcement.

## Process

1. **Read `brain/index.md`** to understand what notes already exist (the SessionStart hook has already shown it to the main session; a subagent reads the file)
2. **Scan the conversation** for:
   - Mistakes made and corrections received
   - User preferences and workflow patterns
   - Codebase knowledge gained (architecture, gotchas, patterns)
   - Tool/library quirks discovered
   - Decisions made and their rationale
   - Friction in skill execution, orchestration, or delegation
   - Repeated manual steps that could be automated or encoded
3. **Skip** anything trivial or already captured in existing brain files
4. **Route each learning** to the right destination (see Routing below)

## Routing

Not everything belongs in the brain. Route each learning to where it will have the most impact.

### Structural enforcement check

Before routing a learning to `brain/`, ask: can this be a lint rule, script, metadata flag, or runtime check? If yes, encode it structurally and skip the brain note. See `brain/principles/encode-lessons-in-structure.md`.

### Brain files (`brain/`)

Codebase knowledge, principles, gotchas — anything that informs future sessions. This is the default destination. Use the `brain` skill for writing conventions.

- One topic per file. File name = topic slug.
- Group in directories with index files using `[[wikilinks]]`.
- No inlined content in index files.

### Skill improvements (`.claude/skills/<skill>/`)

If a learning is about how a specific skill works — its process, prompts, or edge cases — update the skill directly.

### Backlog items

A bug found along the way gets fixed now, with a test. Other follow-up work that can't be done during reflection, such as a rewrite or a missing tool, goes in the final report.

### In a subagent

A subagent runs this skill before its SubagentHandback. The SubagentStop hook can't make a subagent reflect once it has handed back its report. Before committing the notes, run `python ~/.claude/skills/exact-edits/eol.py` from the worktree's root, and `eol.py --fix` if it reports a file whose ending differs from HEAD. A subagent whose brief says it is read-only, such as meditate's auditor and reviewer, skips this skill.

## Summary

```
## Reflect Summary
- Brain: [files created/updated, one-line each]
- Skills: [skill files modified, one-line each]
- Structural: [rules/scripts/checks added]
- Todos: [follow-up items filed]
```
