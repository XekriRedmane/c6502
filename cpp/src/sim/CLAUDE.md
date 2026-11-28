# cpp/src/sim/CLAUDE.md

The in-process 6502 simulator that the C++ tests, the fuzzers' runner,
and `c6502cpp game-run` run programs on: a CPU that executes as py65
1.2.0 does, an assembler that lays an asm program down in a 64K image,
the runtime the programs boot and call helpers in, and the stand-in
Apple IIs that the three games run on. It was ported from sim/'s
Python in step 9 of the port (docs/cpp_port.md), and cpp/README.md
("The simulator's CPU", "The simulator's harness", and "The games'
machines") describes how each part was checked against the Python.

- `cpu.h` / `cpu.cpp` — `Cpu`, the 6502, with py65's departures from
  the NMOS 6502, its trap table, and the callbacks on an address range.
  Its expected values and cycle counts were all taken on py65, and
  cpp/tests/test_cpu.cpp runs it on py65's single steps and decimal-mode
  results frozen in cpp/tests/frozen/cpu/, which can't be written again
  since the Python tool that wrote them is deleted.
- `cpu_run.h` / `cpu_run.cpp` — zlib's CRC-32 (`crc32`) and the digest
  of the CPU's state after an instruction (`digest`), which `game-run`
  and the tests use.
- `assembler.h` / `assembler.cpp` — `assemble`, which lays an asm
  program down in two passes, and `instruction_size`, the byte size of
  each instruction, which the long-branch pass
  (cpp/src/passes/long_branches.h) and the other passes that measure
  code read. The prologue and the `Ret` take the sizes of the naive
  lowering in cpp/src/passes/asm_to_asm2.cpp, so the assembler stays
  byte-aligned with what emission produces. A packed BCD `DecimalArith`
  assembles as `expand_decimal_arith` (cpp/src/passes/asm_to_asm2.h)
  writes it, `SED`, `CLC` or `SEC`, an `LDA` / `ADC` (`SBC`) / `STA` per
  byte, and `CLD`. A program whose bytes would lie past $FFFF or below
  $0000 is an `AssemblerError`.
- `runtime.h` / `runtime.cpp` — the boot stub at $0600, which calls
  `__zp_init` before `main` when the program has one
  (cpp/src/passes/zp_globals.h), the helpers with 6502 code from
  cpp/src/rtlib/ assembled at $E000, `icall` (`JMP (DPTR)`) after them,
  so that indirect calls run, and a trap address with a hook for each
  helper with no 6502 code (the 64-bit and floating-point ones), which
  computes with cpp/src/fp_arith.h. The addresses are the same for
  every layout.
- `harness.h` / `harness.cpp` — `build_sim` of an asm program,
  `build_linked_sim` of a linked executable, `Simulation`, `SimResult`,
  and `defsym_args`, which gives the runtime's helper addresses as
  `--defsym` arguments. A program linked without `--defsym` holds the
  helpers it calls from the runtime library
  (cpp/src/passes/runtime_library.h) and runs them, and one linked with
  `defsym_args` calls the simulator's copies, whose addresses
  `build_linked_sim` checks.
- `source_sim.h` / `source_sim.cpp` — `build_source_sim`, which
  compiles a C source through `passes::front_end` and `passes::codegen`,
  the passes `c6502cpp compile -S` runs, and loads it. The tests'
  `build_sim` (cpp/tests/sim_case.h) and the fuzzers' runner call it.
- `apple2.h` — Zork's Apple II: the Monitor's `HOME`, `CLREOL`, `VTAB`,
  `RDKEY`, `GETLN1`, `COUT`, and `COUT1` over a 40-by-24 screen with its
  text window, a printer, and DOS 3.3's `RWTS` over `.dsk` images keyed
  by slot and drive. Keys and lines come from a script, and every call
  is recorded, so two programs can be compared call for call.
- `lode_runner.h` — Lode Runner's machine, which records every access
  to the soft switches at `$C000-$C0FF` and every call of RWTS as an
  event, with CRCs of the two high-resolution pages, so that the
  original and examples/lode_runner can be compared event for event.
- `ali_baba.h` — Ali Baba's machine, built like Lode Runner's, with
  stand-ins for the Monitor routines the game calls and a Disk II, so
  that the game's own disk code runs. `coverage` reports which of the
  original's routines a run executes.
- `disk2.h` — a Disk II controller and drives emulated a nibble at a
  time, with the half tracks that Ali Baba's copy protection checks.
- `game_machine.h` — what Lode Runner's and Ali Baba's machines share:
  the decoding of each instruction's soft-switch access before it runs,
  the CRC of a hi-res page, and the names of routines.
- `games.h` — the games' scripts and the setups the tests play them in.
  The games' files come from ZORK_DSK, LODE_RUNNER_DIR, and
  ALI_BABA_DIR, which name files from the reverse-engineering projects.
- `game_run.h` — `c6502cpp game-run GAME SCRIPT [--exe PATH]`, which
  plays one script and writes its events, and `c6502cpp game-run
  ali_baba coverage`.

`docs/sim_findings.md` gives the reason for each chapter program that
`sim_skips()` in cpp/tests/sim_chapters.cpp skips.
