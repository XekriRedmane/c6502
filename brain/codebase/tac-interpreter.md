# The TAC interpreter's limits

- tac_sim.py and its C++ port (cpp/src/tac_sim.h, 23 Nov 2026) give the same results on the whole corpus, and tools/compare_tac_sim.py checks that, with the observer of fuzz/tac_passes.py, in about 5 minutes with 14 processes.
- A pass that fuzz/tac_passes.py names isn't always a bug of the pass. Before the fixes of 23 Nov 2026 it named to_ssa, from_ssa, and DirectMemberAccess for five chapter programs, and each time the interpreter was the cause: a Phi reading `int c;` before the loop that assigns it, and an `extern` object with no StaticVariable. Run the program by hand and look at what raised before blaming the pass.
- An object at a fixed address (`[[c6502::address(addr)]]`) gets no memory in the interpreter, and a use of it raises KeyError. Giving it zeroed memory made examples/lode_runner/main.c's polling loop run forever, which hung the comparison, since neither interpreter has a step limit.
- A program whose `main` never returns hangs both interpreters. A tool that runs the whole corpus through them should be checked first with a timeout per program (a subprocess of `c6502cpp backend` with `timeout=`), which finds such a program in about a minute.
- Python's interpreter raises RecursionError at about 330 calls deep, and the C++ one runs to `kMaxCallDepth` (100,000, cpp/src/tac_sim.h), so the two differ on a deep recursion without either being wrong. tools/write_golden.py runs tac_sim.py in a thread with a 128 MB stack and a recursion limit of a million, where Python's gives the value too. Python on Windows rejects a `threading.stack_size` of 256 MB or more with ValueError.
- `PhaseDriver.observer` names each step by the pass's class (`type(p).__name__`), which the C++ driver's table holds as `NamedPass::class_name`. Both rule sets are `RuleSet`.

Related: [[codebase/checks-after-python]], [[principles/prove-it-works]], [[principles/fix-root-causes]]
