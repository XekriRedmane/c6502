# Commands in a worktree-isolated agent

- The worktree guard refuses a Bash command it can't prove stays in the worktree: a heredoc that writes a file, a `cat > file <<EOF` anywhere, and a pipeline or loop that mixes `git` with other commands. Write scripts with the Write tool, under C:/w/ or the scratchpad, run them with `uv run python <path>` as a command of their own, and give each `git` command its own call.
- `taskkill //IM python.exe` is refused, since other agents' processes share the machine. Stop a background command with TaskStop, which also ends its process pool.
- A file edited while MSVC is building can keep the object compiled from the old text, and the link then fails with an unresolved symbol. Touch the edited files and build again.
- To split one working tree into a commit per piece, keep a copy of the final files, write the earlier versions with a script whose every replacement asserts that it matches once, and copy each version in before its `git add`. Each fix then gets its own commit after the code it fixes.
- The guard also refuses a command that runs git after `export PATH=...` (it can't tell which git runs), and one that names a word starting with `.git` beside other commands. Run cmake with the exported PATH in one call and git in another.
- A Bash heredoc halves the backslashes in what it writes, so a Python edit whose old text holds `\n` in a string literal silently matches nothing. Write such scripts with the Write tool, and make every replacement assert that it matched once.
- A new .cpp under cpp/src made while a build is running isn't in that build, and the link fails with an unresolved symbol. The glob's CONFIGURE_DEPENDS picks it up at the start of the next build.
- `cmake --build ... --target c6502_tests` under MSBuild doesn't run ZERO_CHECK, so a new file under cpp/tests isn't compiled and its cases are silently missing. Run `cmake -S cpp -B cpp/build` first, and count the cases (`c6502_tests -tc="<pattern>"`) to see them run.
- A wait loop that greps a build log for `error` matches `test_parser_errors.cpp` and ends early. Grep for `error C`, `error LNK`, or the last target's `->` line.
- Windows Python reads `/c/w/...` as a path on the current drive's root and fails. Give Python scripts `C:/w/...`.
- `uv run python - <<'EOF'` (Python on stdin, quoted delimiter) passes the guard, and a single `\n` in its string literals reaches Python intact, but `\\` arrives as `\`, so `"\\n"` became a newline in a file. Keep edit scripts with doubled backslashes in files written by the Write tool.
- `bc` isn't installed in Git Bash. Time a command with `date +%s%3N` and `$(( ))`.
- `git commit` takes everything staged, including a `git rm` made long before for a later commit. G7's first commit silently carried a test module's deletion. Run `git diff --cached --name-status` before every commit, and stage a removal only when its commit is next.
- A cleanup glob like `rm -rf /c/w/g7/rec_*` also matches a script named `rec_all.sh` beside the directories it's meant for. Give scripts names outside the glob.
