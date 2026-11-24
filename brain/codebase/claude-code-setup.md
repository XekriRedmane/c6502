# Claude Code setup in this repository

- Claude Code finds a project's skills only in `.claude/skills/`. Brainmaxxing's README puts them in `.agents/skills/`, which Codex reads, so the brainmaxxing skills live in `.claude/skills/` here and their script paths name that folder.
- A hook's `matcher` is tested against the tool's name, never the file path. Upstream brainmaxxing's `"brain/"` matcher on `PostToolUse` matched nothing, so the index hook here fires on `Write|Edit|MultiEdit` and exits early when no note was added or removed.
- Hook scripts read their JSON input with `jq` (winget's `jqlang.jq`). `.claude/hooks/reflect-before-stop.sh` lets a subagent stop when `jq` is missing, because without it the hook can't read `stop_hook_active` and would block every stop.
- Each hook command runs as `bash "$CLAUDE_PROJECT_DIR/.claude/hooks/<script>"`, so the hook doesn't depend on a script's executable bit and works in any clone.
- `.gitignore` ignores only `.claude/worktrees/` in `.claude/`. The worktrees are git checkouts, which can't be committed inside the repository, and the hooks, settings, and skills are shared.
- The `SubagentStop` hook blocks each subagent's first stop and tells it to run `reflect` and commit the notes in its own worktree. It lets the second stop through (`stop_hook_active`). It names brainmaxxing's `reflect`, because `pstack:reflect` has `disable-model-invocation: true` and waits for approval, so a subagent can't run it. Robert keeps both skills.
- The hook doesn't make a subagent reflect when the subagent ends by handing back its report (`SubagentHandback`). G3 of step 8 handed back on 23 Nov 2026, the hook's block was recorded as the last entry of its transcript, and the subagent took no further turn. So the briefs of the port's groups tell each agent to run `reflect` before it writes its report, and the hook stays for subagents that end with a plain final message.
- On 23 Nov 2026 Robert chose a Claude-only setup. A Codex copy (`.agents/`, `.codex/`, `AGENTS.md`) appeared after a restart and was deleted, so `.claude/` holds the only copy of the skills and hooks.

Related: [[codebase/t3-restarts]], [[principles/encode-lessons-in-structure]]
