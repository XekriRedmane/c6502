# A def index entry for a global isn't its reaching definition

- `framework::build_def_idx` maps every name a function writes to its last definition, globals and address-taken locals included. In SSA form only an SSA name's entry is the definition its reads see. A global's entry may come after the read, on another path, or be overwritten by a call.
- The compare narrowing of `fold_producer_jumps` (cmp_zero_jump_fold.cpp) accepted any operand read once and followed its def index entry, so `if (z < k) for (...) z = (p ? i : i);` compared `i` before the loop set it. fuzz.stmts seed 854 found it under the IR checks on 26 Nov 2026, as "JumpIfCmp reads @2.i.2, whose definition doesn't dominate it", and 11b5d0de requires an SSA name in SSA form. Look for the same gate in any pass that follows a def index entry.
- A failure that the IR checks report at compile time reduces fastest with a script that deletes lines while `cpp/build/bin/c6502cpp compile -S -O` of the text, with `C6502_CHECK_IR=1`, still prints the same message. `c6502cpp fuzz reduce` runs every path, the simulator included, at each step. Line deletion took 131 lines to 53, and narrowing by hand from there took a few minutes.
- `c6502cpp compile --tac -O` prints the optimized TAC of the reduced program, which showed the wrong compare at once.

Related: [[codebase/tac-interpreter]], [[codebase/simulator-cpu]], [[principles/fix-root-causes]]
