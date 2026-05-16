# sim/CLAUDE.md

In-process 6502 simulator. Drives compiled C through the full pipeline,
assembles the result with a runtime stub, and executes on a py65 6502
MPU until the boot stub's BRK fires.

## Module roster

- `assembler.py` — pure-Python in-process assembler from `asm_ast.
  Program` to a memory image. Two-pass design: pass 1 walks every top-
  level entry in source order, computing the byte size of each
  instruction; pass 2 emits bytes with all symbols resolved.
  `_prologue_size` / `_ret_size` / `_emit_prologue` / `_emit_ret`
  mirror the naive lowering in `passes.asm_to_asm2`, so
  `instruction_size` (used by `passes.long_branches`) and the
  assembler stay byte-aligned with what `asm_emit` produces.
- `harness.py` — top-level test harness: take C source, run the full
  compiler pipeline, assemble with a runtime stub, and execute on a
  py65 6502 simulator until BRK.
- `runtime.py` — runtime stub: zero-page reservations, boot stub,
  reset vector, and Python-implemented hooks for the 6502 helpers
  (`mul*` / `divmod*` / `asl*` / `asr*` / `lsr*`, plus FP slots). Each
  helper is given a fixed trap address; the harness intercepts PC at
  that address and dispatches to a Python implementation. This lets
  every chapter test run end-to-end without the real 6502 runtime
  library (not yet in the repo).
- `runtime_helpers.py` — 6502 assembly implementations of the runtime
  helpers, expressed as `asm_ast.Function` objects. `build_runtime`
  assembles these alongside the user program and binds their symbols,
  so a `JSR udivmod8` in user code lands on the real 6502 routine
  instead of a Python trap.

## Notes

- See `docs/sim_findings.md` for known divergences between the
  Python-trap runtime and the 6502-asm runtime.
- The simulator is what `tests/test_sim_*` harnesses drive.
