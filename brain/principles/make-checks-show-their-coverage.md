# Make Checks Show Their Coverage

A passing check is evidence only for what it examined. Each check fails, or prints a count, when it examines less than it should.

**Why:** Several checks in this repository passed while examining nothing. MSBuild left a new test file out of a build, and the run passed without its cases. A heredoc halved the backslashes in an edit script, and a replacement matched nothing without an error. An index recorded before a refactor listed calls that no longer happened. [[principles/prove-it-works]] covers a check that fails for the wrong reason, and this principle covers one that passes for the wrong reason.

**Pattern:**
- Make scripted edits with exact-edits' edit.py, which refuses a passage that doesn't occur exactly once and writes no file until every passage matches.
- Count the cases a run executed, and compare the count with the number expected.
- Give a run over a corpus a time limit for each program, so that a program that never returns ends as a failure.
- Record again before trusting a record made before the code changed.
- To show that a new test fails without its fix, reset only the fixed source files to HEAD. A plain `git stash` hides the new test too, and the check passes without proving anything.

**Evidence:** [[codebase/msbuild-stale-builds]], [[codebase/git-bash-on-windows]], [[codebase/checks-after-python]], [[codebase/tac-interpreter]], and [[codebase/long-runs-and-scratch-scripts]].
