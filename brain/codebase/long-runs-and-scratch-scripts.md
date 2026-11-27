# Long runs and scratch scripts

- Send the full output of a suite, fuzzer, or build to a file and read the file afterwards. Piping it through `tail` or `sort` lost the failure list, and ctest's pass or fail line, often enough that 10 to 20 minute runs had to be repeated.
- A lead that waits for background agents or a long run can't sleep in a foreground Bash call, which the harness refuses. A scratch script that sleeps and then prints the lock's holder and each agent worktree's newest commit (C:/w/g1/pause.sh in G1), run in the foreground with a timeout just under 10 minutes, waits and shows progress, and the agents' reports arrive between calls.
- Start a long run with the Bash tool's `run_in_background`. A `&` inside a foreground call is killed when that call's shell exits.
- A script whose stdout is redirected to a data file writes its own messages to stderr, or they end up in the data.
- Never name a scratch script after a standard-library module. A `bisect.py` beside a script shadowed the module, and importing `random` failed.
- Stop a process by its ID or with TaskStop. A kill filter that matches text in command lines matched its own shell's command line and killed it.
- TaskStop on a background `bash script.sh` ended the Bash tool's shell but left the script's own bash running its loop, and a second copy started after it built the same worktree in the same build directory, so MSBuild failed on locked `.obj` files. After stopping a script that loops, list its processes (`Get-CimInstance Win32_Process` filtered on the script's name) and stop them before starting it again.
- `compile.py -o` takes only `.asm` or `.s` for `-S`, and with stderr redirected its error was silent. Read stderr when a step produces no file.
- The session's scratchpad is shared with the agents of earlier groups, and their logs stay there. A suite log named `base_on.log` was G9's from hours before, and its count looked like a run of the current tree. Keep a run's files in a subdirectory of your own and check the file's time before reading a count from it.
- Make scripted edits with the exact-edits skill (~/.claude/skills/exact-edits/edit.py). A hand-written script that called `open(path, "wb").write(text)` with a `str` truncated the file and then raised TypeError, which emptied a test module in group R. Restore a file that a failed script touched with `git checkout <path>`.

Related: [[codebase/git-bash-on-windows]], [[codebase/msbuild-stale-builds]], [[codebase/t3-restarts]], [[principles/make-checks-show-their-coverage]], [[principles/serialize-shared-state-mutations]], [[principles/pass-data-through-files]]
