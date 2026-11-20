# Porting c6502 to C++

Robert decided on 19 Nov 2026 to port the compiler from Python to C++.
The Python compiler had been made about twice as fast that day, and 20
`fuzz.stmts` programs still took 16.9 seconds against a target of 5.
This document records the decisions and the plan, and is updated as
stages land.

## Decisions

- **One stage at a time, with the Python compiler as the reference.**
  Each part of the pipeline moves to C++ only when it produces
  byte-for-byte the same output as the Python code it replaces. While
  the port is under way, the Python front end hands its IR to the C++
  stages through a text dump, and the boundary moves toward the front
  until no Python is left in the compiler.
- **The Python compiler is frozen for features.** Bug fixes still go
  in, and each is ported too, so the C++ code aims at a fixed target.
- **Scope.** Everything `compile.py`, `nm.py`, and `objdump.py` do
  moves to C++: preprocessing, parsing, every pass, assembly to ELF,
  and linking. The simulator (`sim/`, on py65), the tests, and the
  fuzzers stay in Python and drive the C++ executables.
- **The parser is written by hand,** as recursive descent with
  precedence climbing for expressions. It must give the ASTs and the
  error messages that `parser.py` gives today. `c99.lark` stops being
  the source of truth when it lands.
- **Build.** CMake and C++20. MSVC 2022 Build Tools on Windows, and
  GCC or Clang in the Docker image.

## How each stage is checked

- **The IR is defined once.** `asdl.py` generates the C++ IR classes
  from the same four `.asdl` files as the Python dataclasses, and a
  text dump format with a reader and a writer on both sides. A Python
  dump read by C++ and dumped again must come out the same bytes.
- **Each pass is checked on real inputs.** A capture harness runs the
  Python pipeline over the corpus and records every call of a pass:
  its input, its arguments, and its output. The C++ port of the pass
  must turn each recorded input into the recorded output.
- **The corpus.** The examples (`examples/*.c` and the three games'
  units), every C file under `tests/chapter_*/`, and seeds of the
  `fuzz.stmts`, `fuzz.calls`, `fuzz.exprs`, and `fuzz.yarpgen`
  programs, each compiled with `-S` and with `-S -O`.
- **End to end.** Once a contiguous run of stages is in C++, the
  pipeline that uses them must give the same `-S` output as the
  Python one on the whole corpus, and the suite and the fuzzers must
  pass with it.

## Order

1. **Groundwork.** The `cpp/` tree with its CMake build and a test
   runner, the generated C++ IR for `asm.asdl` (then the other three),
   the dump format on both sides, and the capture harness. One small
   asm pass ported end to end shows that the pieces fit.
2. **The asm back end after prologue synthesis.** The region of
   `compile.optimized_asm` from `apply_licm` through `apply_shared_tails`
   and `apply_outline_runs`: the peephole fixed points, the counter
   promotions, slot coalescing, carry tracking, jump tables, long
   branches, and short jumps. It took about three quarters of the
   fuzzers' time. Python hands it the asm program and the symbol tables
   it reads.
3. **Emission.** `asm_to_asm2` and `asm_emit`, so that the C++ side
   writes the `-S` text.
4. **The asm optimizer and code generation.** `tac_to_asm`, the asm SSA
   round trip and register allocation, `replace_pseudoregisters`, and
   the zero-page allocators.
5. **The TAC optimizer,** with `c99_to_tac` and the inliner.
6. **The front end.** The preprocessor, the hand-written parser, and
   the c99 passes through the type checker.
7. **Assembler and linker.** `binutils/` and `passes/linker.py`, with
   `nm` and `objdump`.
8. **Retiring the Python compiler.** The tests that call Python passes
   directly are ported to C++ unit tests or replaced by the corpus
   checks, and the Python compiler is removed.

## Status

- 19 Nov 2026: decisions made, groundwork started.
- 19 Nov 2026: the groundwork landed. The CMake build works with MSVC,
  GCC, and Clang, with doctest for the C++ tests (`cpp/README.md` has
  the commands and the conventions for porting a pass). The C++ IR is
  generated from all four `.asdl` files (`asdl.py --cpp`, into
  `cpp/generated/`). The dump format (`ir_dump.py`) has a reader and a
  writer on both sides, and a compiled example's four IRs make the
  trip from Python to C++ and back byte for byte. The capture harness
  (`tools/capture_passes.py`, checked with `tools/check_captures.py`)
  records the calls of the passes of `optimized_asm` from `apply_licm`
  on. `split_mem_to_mem` is ported and matches the Python pass on all
  7,049 distinct calls of the corpus.
- 19 Nov 2026: the helpers that the asm passes of stage 2 share are
  ported: `asm_liveness`, `asm_aliasing`, what the asm passes use of
  `decimal_arith`, the asm CFG with its dominators and the tolerant
  builder from `redundant_load`, the helpers that other passes import
  from `asm_dead_store`, `inc_peephole`, and `dead_a_arith`,
  `call_edges`, and the instruction sizes of `sim/assembler.py`. The
  runtime layout, the user zero-page globals, and the pointer proof,
  which Python keeps in module globals, are one `AsmContext` that
  every C++ asm pass receives (`cpp/README.md`, "The compile's
  context"), and the pass captures record the context of each call.
  The helpers are checked on what the passes see:
  `tools/capture_helpers.py` evaluates each one on every function each
  pass is given over the examples, the games, and the fuzz seeds, at
  every instruction index or on every pair of operands, and `c6502cpp
  check-helpers` compares the C++ values with those, about 2.1 billion
  values in 54,677 records, with no difference. The passes of the
  region can now be ported, several at a time.
- 20 Nov 2026: the twelve passes of the loops and the index
  registers: `apply_licm`, `promote_loop_carried_to_a`,
  `loop_counter_to_x`, `counter_to_index_reg`, `x_save_slot_load`,
  `dual_index_promotion`, `loop_invariant_a`, `rotate_loop_test`, and
  the peephole fixed point's `element_in_place`, `loop_constant_a`,
  `dec_register_peephole`, and `index_inc_cmp_y`. Each matches the
  Python pass on every distinct call of the corpus, from 926 calls of
  most of the pipeline's passes to 7051 of `element_in_place`.
  `rotate_loop_test` reads `long_branches._invert` as
  `passes::long_branches::invert`.
- 20 Nov 2026: the control-flow and whole-program passes of stage 2,
  which are the branch and jump peepholes (`dead_label_drop`,
  `unreachable_drop`, `branch_invert`, `branch_to_next_drop`,
  `branch_through_jump`, `branch_thread`, `branch_value_thread`,
  `tail_call`, `tail_merge`, `cross_jump`), `slot_coalescing`,
  `ret_slot_coalescing`, `jump_table`, `copy_through_index`,
  `long_branches`, `short_jumps`, `shared_tails`, and `outline_runs`.
  Each matches the Python pass on every distinct call that the default
  corpus makes of it, 60,282 calls in all (cpp/README.md, "The
  control-flow and whole-program passes").
- 20 Nov 2026: the dataflow passes of the fixed point are ported:
  `redundant_load` (7,036 distinct calls of the corpus),
  `memory_value_propagation` (7,038), `remat` (6,783),
  `asm_dead_store` (6,746), `redundant_store` (6,779), and
  `mem_const_prop` (4,980), each with no difference. Two bugs found on
  the way were fixed in both compilers. `mem_const_prop` kept the
  constant it knew A held across a move that `asm_emit` writes through
  A, such as `Mov(#7, M)`, and `memory_value_propagation` gave Y a new
  identity only at a `Mov` or a write of X or Y, so an `arr,Y` it had
  recorded outlived the `LDY #off` of a `CMP (zp),Y` or an `ADC
  (zp),Y`.
