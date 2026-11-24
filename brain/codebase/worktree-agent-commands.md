# Commands in a worktree-isolated agent

- The worktree guard refuses a Bash command it can't prove stays in the worktree: a heredoc that writes a file, a `cat > file <<EOF` anywhere, and a pipeline or loop that mixes `git` with other commands. Write scripts with the Write tool, under C:/w/ or the scratchpad, run them with `uv run python <path>` as a command of their own, and give each `git` command its own call.
- `taskkill //IM python.exe` is refused, since other agents' processes share the machine. Stop a background command with TaskStop, which also ends its process pool.
- A file edited while MSVC is building can keep the object compiled from the old text, and the link then fails with an unresolved symbol. Touch the edited files and build again.
- To split one working tree into a commit per piece, keep a copy of the final files, write the earlier versions with a script whose every replacement asserts that it matches once, and copy each version in before its `git add`. Each fix then gets its own commit after the code it fixes.
- The guard also refuses a command that runs git after `export PATH=...` (it can't tell which git runs), and one that names a word starting with `.git` beside other commands. Run cmake with the exported PATH in one call and git in another.
- A Bash heredoc halves the backslashes in what it writes, so a Python edit whose old text holds `\n` in a string literal silently matches nothing. Write such scripts with the Write tool, and make every replacement assert that it matched once.
- A new .cpp under cpp/src made while a build is running isn't in that build, and the link fails with an unresolved symbol. The glob's CONFIGURE_DEPENDS picks it up at the start of the next build.
- `bc` isn't installed in Git Bash. Time a command with `date +%s%3N` and `$(( ))`.
