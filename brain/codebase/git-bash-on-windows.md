# Git Bash and Python on this Windows machine

- A heredoc run through the Bash tool halves the backslashes it passes on, even with a quoted delimiter. `uv run python - <<'EOF'` passes the worktree guard, and a single `\n` in a string literal reaches Python intact, but `\\` arrives as `\`. An edit script's `"\\n"` then became a newline in a file, and an old text meant to match a literal `\n` matched nothing. Write any edit script that holds a backslash with the Write tool, run it from its file, and make every replacement assert that it matched once.
- Windows Python reads `/c/w/...` as a path on the current drive's root and fails. Give Python scripts `C:/w/...`.
- The system PATH is stored as REG_SZ since about 23 Nov 2026, so its `%STM32CLT_PATH%` entries don't expand and cmake isn't found. Put `/c/ST/STM32CubeCLT_1.22.0/CMake/bin` on the PATH for each command that runs cmake or ctest, until Robert changes the value back to REG_EXPAND_SZ.
- Git Bash rewrites an argument or environment value that looks like a Unix path when it starts a Windows program, so `CL=/MP` reached cl.exe as `C:/Program Files/Git/MP`. Spell a Windows tool's option with a dash where the tool takes one (`-MP`), or set `MSYS_NO_PATHCONV=1` for the command.
- `python3` is the Microsoft Store stub. Run Python with `uv run python`.
- Don't check line endings with grep. Git Bash's grep strips the CR from each line it reads unless given `-U`, so `grep -c $'\r'` reports 0 for a CRLF file, and a check built on it converted 38 files the wrong way and then all 90 back. Count `\r\n` against bare `\n` in the bytes with Python. docs/ is LF in HEAD apart from four files.
- Python on this machine opens a file as cp1252 unless told otherwise, so an em-dash or a "±" fails to match or is written wrongly. A script that reads or writes text names `encoding="utf-8"`, and `newline=""` when it rewrites a file in place.
- A redirected stdout on Windows writes CR LF in cp1252. Never regenerate a checked-in file (an example's `.asm`, a golden file, a generated list) by redirecting a command's output. Use the tool's own `-o` or `--update`, or have the script open its output with `encoding="utf-8", newline="\n"`, and compare its line endings with HEAD.

Related: [[codebase/worktree-guard]], [[principles/make-checks-show-their-coverage]]
