# Long runs and scratch scripts

- Send the full output of a suite, fuzzer, or build to a file and read the file afterwards. Piping it through `tail` or `sort` lost the failure list, and ctest's pass or fail line, often enough that 10 to 20 minute runs had to be repeated.
- Start a long run with the Bash tool's `run_in_background`. A `&` inside a foreground call is killed when that call's shell exits.
- A script whose stdout is redirected to a data file writes its own messages to stderr, or they end up in the data.
- Never name a scratch script after a standard-library module. A `bisect.py` beside a script shadowed the module, and importing `random` failed.
- Stop a process by its ID or with TaskStop. A kill filter that matches text in command lines matched its own shell's command line and killed it.
- `compile.py -o` takes only `.asm` or `.s` for `-S`, and with stderr redirected its error was silent. Read stderr when a step produces no file.

Related: [[codebase/git-bash-on-windows]], [[principles/make-checks-show-their-coverage]]
