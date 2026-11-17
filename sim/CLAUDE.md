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
- `linked.py` — `build_linked_sim`: runs an ELF executable that
  linking with `compile.py` wrote. It loads each segment at its load
  address, reads the symbols, and installs the runtime for the
  program's layout (its SSP, FP, HARGS, and DPTR symbols). A program
  that calls the runtime's helpers is linked with their addresses
  (`runtime_defsyms`, `defsym_args`), which are the same for every
  layout, and `build_linked_sim` checks that they match.
- `apple2.py` — `Apple2`, a stand-in Apple II for running a whole
  program: the Monitor's `HOME`, `CLREOL`, `VTAB`, `RDKEY`, `GETLN1`,
  `COUT`, and `COUT1` over a 40-by-24 screen with its text window, a
  printer at `$C100` (where CSW points when COUT should print), and
  DOS 3.3's `RWTS` over `.dsk` images keyed by slot and drive. Keys and
  lines come from a script. Every call is recorded in `calls`, so two
  programs can be compared call for call, and `run` stops at a `BRK`,
  a `JMP` to itself, or a line input with the script used up.
  `tests/test_zork_interpreter.py` uses it.
- `lode_runner.py` — `Machine`, a stand-in Apple II for Broderbund's
  Lode Runner and examples/lode_runner. It records every access to
  the soft switches at `$C000-$C0FF` and every call of RWTS (a Python
  hook at `$B7B5` over a copy of the game's disk) as an event, with
  CRCs of the two high-resolution pages, so that the original and the
  C program can be compared event for event (`first_difference`).
  Keys come from a script indexed by the number of KBD reads. The
  soft switches are found by decoding each instruction's address
  before it runs. `original_machine` and `program_machine` set up the
  two programs. `tests/test_lode_runner.py` uses it.
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
- `build_sim(optimize=False)` runs the same passes as `compile.py
  -S`, including the peephole fixedpoint, so unoptimized sim
  tests execute the code the CLI emits.
- `build_runtime` lays down `icall` (`JMP (DPTR)`, 3 raw bytes) after
  the assembled helpers, so indirect calls through function pointers
  run in the sim.
- The boot stub calls `__zp_init` before `main` when the program has
  one, which stores the initial values of its zero-page globals
  (`passes/zp_globals.py`).
