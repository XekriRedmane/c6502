# Claude Code setup in this repository

- Claude Code finds a project's skills only in `.claude/skills/`. Brainmaxxing's README puts them in `.agents/skills/`, which Codex reads, so the brainmaxxing skills live in `.claude/skills/` here and their script paths name that folder.
- A hook's `matcher` is tested against the tool's name, never the file path. Upstream brainmaxxing's `"brain/"` matcher on `PostToolUse` matched nothing, so the index hook here fires on `Write|Edit|MultiEdit` and exits early when no note was added or removed.
- `jq` isn't installed on this machine. Hook scripts read the JSON on stdin with `grep` and `sed`, as `.claude/hooks/reflect-before-stop.sh` does.
- Each hook command runs as `bash "$CLAUDE_PROJECT_DIR/.claude/hooks/<script>"`, so the hook doesn't depend on a script's executable bit and works in any clone.
- `.gitignore` ignores only `.claude/worktrees/` in `.claude/`. The worktrees are git checkouts, which can't be committed inside the repository, and the hooks, settings, and skills are shared.
- The `SubagentStop` hook blocks each subagent's first stop and tells it to run `reflect` and commit the notes in its own worktree. It lets the second stop through (`stop_hook_active`). It names brainmaxxing's `reflect`, because `pstack:reflect` has `disable-model-invocation: true` and waits for approval, so a subagent can't run it. Robert keeps both skills.
- On 23 Nov 2026 Robert chose a Claude-only setup. A Codex copy (`.agents/`, `.codex/`, `AGENTS.md`) appeared after a restart and was deleted, so `.claude/` holds the only copy of the skills and hooks.
