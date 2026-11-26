# The TAC interpreter's limits

- The TAC interpreter is cpp/src/tac_sim.h, the port of tac_sim.py (23 Nov 2026), which gave the same results on the whole corpus before group R of step 8 removed the Python one. The golden files' mode `tacsim` holds its results on the corpus, with the observer of fuzz/tac_passes.py.
- A pass that fuzz/tac_passes.py names isn't always a bug of the pass. Before the fixes of 23 Nov 2026 it named to_ssa, from_ssa, and DirectMemberAccess for five chapter programs, and each time the interpreter was the cause: a Phi reading `int c;` before the loop that assigns it, and an `extern` object with no StaticVariable. Run the program by hand and look at what raised before blaming the pass.
- An object at a fixed address (`[[c6502::address(addr)]]`) gets no memory in the interpreter, and a use of it raises KeyError. Giving it zeroed memory made examples/lode_runner/main.c's polling loop run forever, which hung the comparison, since the interpreter has no step limit.
- A program whose `main` never returns hangs the interpreter. A tool that runs the whole corpus through it should be checked first with a timeout per program (a subprocess of `c6502cpp backend` with `timeout=`), which finds such a program in about a minute.
- The interpreter runs to `kMaxCallDepth` (100,000 calls, cpp/src/tac_sim.h), where the Python one raised RecursionError at about 330 calls deep. The golden files were written with Python's in a thread with a 128 MB stack and a recursion limit of a million, so they hold the value of a deep recursion too.
- The observer names each step by the pass's class name, which the C++ driver's table holds as `NamedPass::class_name`. Both rule sets are `RuleSet`.

Related: [[codebase/checks-after-python]], [[principles/prove-it-works]], [[principles/fix-root-causes]]
