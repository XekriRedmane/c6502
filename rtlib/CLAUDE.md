# rtlib/CLAUDE.md

The runtime library: the 6502 helpers that compiled code calls for
operations it doesn't do inline, such as `udivmod16` for a 16-bit
division by a variable, or a multiply of two 16-bit values. They live
here because every linked program takes them from here, and the
simulator runs the same code. The package isn't named `runtime`, since
a script run from inside `sim/` would then find `sim/runtime.py` under
that bare name and fail to import the package.

- `helpers.py` — the helpers as `asm_ast.Function`s
  (`all_helper_functions`, and `ASM_IMPLEMENTED`, the names of those
  with 6502 code). Each reads its operands at the low offsets of
  `HARGS` and leaves its results at higher offsets, as `tac_to_asm`
  passes them; the module docstring and docs/architecture.md give each
  layout. The linker emits the ones a program calls with `asm_emit`,
  assembles them into an object named `runtime.o`, and links that in
  (`passes/runtime_library.py`), so a program links with no
  `--defsym`. `sim.runtime.build_runtime` assembles all of them at
  `$E000` for the simulator's harness, and a helper with no 6502 code
  (the 64-bit and floating-point ones, `icall`) runs as a Python hook
  there. Each was checked against its hook on edge cases and random
  operands; `tests/test_runtime_helpers.py` tests them through the
  harness, and `tests/test_runtime_library.py` through linked C.

`docs/linker_scripts.md` says where a linker script puts the helpers.
