# What the worktree guard refuses

- An agent with worktree isolation runs its Bash commands through a guard that refuses any command it can't prove stays in the worktree. It refuses a heredoc that writes a file, `cat > file <<EOF` anywhere, a pipeline or loop that mixes `git` with other commands, a command that runs git after `export PATH=...` (the guard can't tell which git runs), and a command that names a word starting with `.git` beside other commands.
- Write scripts with the Write tool under C:/w/ or the scratchpad, and run each with `uv run python <path>` as a command of its own. Give each `git` command its own call, and run cmake with an exported PATH in a call without git.
- `taskkill //IM python.exe` is refused, since other agents' processes share the machine. Stop a background command with TaskStop. It ends a pytest worker pool, but a looping script's own bash survives it ([[codebase/long-runs-and-scratch-scripts]]).
- `git commit` takes everything staged, including a `git rm` made long before for a later commit, which carried a test module's deletion into an unrelated commit in G7. Run `git diff --cached --name-status` before every commit, and stage a removal only when its commit is next.

Related: [[codebase/git-bash-on-windows]], [[codebase/msbuild-stale-builds]], [[codebase/t3-restarts]]
