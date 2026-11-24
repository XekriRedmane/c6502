# What a T3 server restart stops

- A restart of the T3 server cancels every background subagent and background shell command of the session. None of them reports back, and a notice after the restart lists them.
- Before resuming an agent, look at its worktree: `git log main..worktree-agent-<id>` for its commits and `git -C .claude/worktrees/agent-<id> status --short` for edits it hadn't committed. Its check worktrees under `C:/w/` survive too.
- Resume an agent with `SendMessage` to its id. Tell it what it had committed, that its background commands were stopped and must be rerun, and what has landed on `origin/main` since its brief, so that it merges before its final checks.
- A check of main that was running in the background has to be started again. Its output file shows how far it got.

Related: [[codebase/worktree-guard]], [[principles/make-operations-idempotent]]
