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
  assembler stay byte-aligned with what `asm_emit` produces. A packed
  BCD `DecimalArith` assembles as `asm_to_asm2.expand_decimal_arith`
  writes it, `SED`, `CLC` or `SEC`, an `LDA` / `ADC` (`SBC`) / `STA`
  per byte, and `CLD`. py65 runs `ADC` and `SBC` in decimal mode as
  `builtin_functions.adc_byte` / `sbc_byte` compute them.
- `harness.py` — top-level test harness: take C source, run the full
  compiler pipeline, assemble with a runtime stub, and execute on a
  py65 6502 simulator until BRK.
- `linked.py` — `build_linked_sim`: runs an ELF executable that
  linking with `compile.py` wrote. It loads each segment at its load
  address, reads the symbols, and installs the runtime for the
  program's layout (its SSP, FP, HARGS, and DPTR symbols). A program
  linked without `--defsym` holds the helpers it calls from the
  runtime library (`passes/runtime_library.py`) and runs them. One
  linked with the simulator's helper addresses (`runtime_defsyms`,
  `defsym_args`), which are the same for every layout, calls the
  simulator's copies, and `build_linked_sim` checks that the
  addresses match.
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
- `ali_baba.py` — `Machine`, a stand-in Apple II for Electronic Arts'
  Ali Baba and examples/ali_baba, built like `lode_runner.py`. It
  records every soft-switch access, every call of a Monitor routine
  (stand-ins for WAIT, PREAD, SETPWRC, and the text routines the
  HRCG's control characters reach, with the argument each reads),
  and every call from the game into the kept disk code at `$B300` and
  `$B7B5`, with a CRC of hi-res page 1, so the original and the C
  program can be compared event for event. COUT isn't an event, since
  the C program calls the HRCG directly. Keys and disk swaps come from
  a script indexed by the number of KBD reads. `original_machine`
  starts the original from main.bin as the loader leaves it (the
  HRCG and the disk code staged in page 1), and `program_machine`
  loads a linked executable over the same image, runs `__zp_init`,
  and enters `main`. `coverage` reports which of the original's
  routines a run executes. `tests/test_ali_baba.py` uses it.
- `disk2.py` — a Disk II controller and drives emulated a nibble at a
  time: a `.dsk` image converted to DOS 3.3 tracks (address and data
  fields, 6-and-2 encoding, the sector skew) and back, stepper phases
  and half tracks, the motor, Q6/Q7, writes, and write protect. The
  disk moves one nibble per read of the data latch, so what code reads
  doesn't depend on its timing. `WIDE_TRACK` reads the half tracks
  around track 5 as track 5, as the wide track Ali Baba's copy
  protection checks would read.
- `runtime.py` — runtime stub: zero-page reservations, boot stub,
  reset vector, and Python-implemented hooks for the 6502 helpers
  (`mul*` / `divmod*` / `asl*` / `asr*` / `lsr*`, plus FP slots). Each
  helper is given a fixed trap address; the harness intercepts PC at
  that address and dispatches to a Python implementation. The helpers
  with 6502 code run as 6502 code, and the hooks cover the rest (the
  64-bit and floating-point helpers), so every chapter test runs
  end-to-end.
- The runtime helpers' 6502 code, which `build_runtime` assembles
  at $E000, is in `runtime/helpers.py` (see runtime/CLAUDE.md).

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
