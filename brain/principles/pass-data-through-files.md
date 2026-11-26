# Pass Data Through Files

Hand code and data between tools as files, written by the Write tool or by the tool's own output flag, and pass the file's path. A shell string, a pipe, or a redirected stream on this machine can rewrite what passes through it.

**Why:** Each channel between two tools here has changed bytes on the way. A heredoc halved backslashes, Git Bash turned `CL=/MP` into a path, a redirected stdout wrote CR LF in cp1252, Python's pipes read cp1252, and a pipe through `tail` dropped a failure list. Each failure was silent, and each was found only after its output had been trusted.

**Pattern:**
- Write a script with the Write tool and run it from its file. The heredoc hook enforces this for Bash.
- Regenerate a checked-in file with the tool's own `-o` or `--update`, never by redirecting its stdout.
- Send a long run's output to a file and read the file.
- Where a channel can't be avoided, check the bytes that arrived, with `eol.py --style` for line endings or a count of what was expected.
- Before adding a new channel, such as a subprocess protocol, decide how its bytes are framed. The shims' frame protocol (`C6502CPP_FRAMES=1`) lets Python write the command's bytes to its own streams, so the output never passes through a text-mode pipe.

**Evidence:** [[codebase/git-bash-on-windows]], [[codebase/long-runs-and-scratch-scripts]], [[codebase/compile-command]], [[codebase/worktree-guard]].
