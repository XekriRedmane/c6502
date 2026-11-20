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

## Step 4: code generation and the asm optimizer

Step 4 is the part of a compile between the TAC optimizer and
`apply_licm`. The TAC passes at the start of `optimized_asm`
(`apply_row_address_tables`, `optimize_tac`,
`dispatch_const_pointer_arrays`, `apply_split_tables`) belong to step
5. This section lists the calls of the stage, what each reads, and the
groups in which several agents can port them at once. The survey was
made on 20 Nov 2026 from compile.py at de336d0.

### The calls

Under `-O`, `_run_stage` and `optimized_asm` make these calls in this
order. "Tables" means the symbol table, the type table, or both. Each
has an entry in `tools/capture_passes.STAGE_PASSES` under the name in
the first column.

| Capture name | Python | Lines | Reads besides its IR | Returns |
|---|---|---|---|---|
| `plan_zero_page` | `zp_globals.plan_zero_page` | 570 | TAC, tables, `promote` | `ZpGlobals` |
| `select_abi` | `abi_selection.select_abi` | 1165 | TAC, the c99 program, types | the ABI |
| `choose_global_carry_returns`, `choose_carry_returns` | `carry_return_choice` | 240 | TAC, symbols, ABI | a set, and sets `FunAttr.carry_return` in `symbols` |
| `recursive_pool_functions`, `pool_eligible` | `zp_local_allocation` | 580 | TAC, ABI | cycles, a set |
| `find_shares` | `slot_sharing.find_shares` | 965 | TAC, ABI, tables, three sets | the shares and the bindings |
| `allocate_zp_slots` | `zp_slot_allocation` | 400 | TAC, ABI, shares, reserved bytes, ranges | the ABI with addresses, the slot symbols |
| `pinned_param_copies`, `slot_aliases` | `slot_sharing` | | TAC, ABI, tables / the shares | |
| `tac_to_asm` | `tac_to_asm.translate_program` | 5600 | TAC, tables, ABI, cycles, bindings, copies, aliases, the user zero-page globals | asm with pseudos, and the `<param>.reg` locals added to `symbols` |
| `asm_optimizer` | `optimization_asm.optimizer.optimize_program` | 6500 | asm, ABI, symbols, pools, reserved bytes, cycles | asm, the colorings |
| `compute_local_bytes`, `compute_address_taken_bytes` | `function_local_sizing` | 240 | asm, tables | bytes by function |
| `allocate_function_locals` | `zp_local_allocation` | | TAC, ABI, byte counts, shares, ranges | the pools |
| `address_taken_assignments`, `address_taken_slot_symbols` | `address_taken_zp` | 230 | asm, pools, colorings, tables | addresses and names |
| `replace_pseudoregisters_bare_exit` | `replace_pseudoregisters` | 900 | asm, tables, colorings, ABI, pools, assignments | asm, the frames |
| `local_slot_names`, `local_slot_symbols` | `zp_slot_naming`, `zp_local_allocation` | 290 | pools, colorings, assignments | names, addresses |
| `lower_data_load_address` | `lower_data_load_address` | 170 | asm, slot symbols, byte names | asm |
| `synthesize_prologue` | `prologue_synthesis` | 190 | asm, the frames | asm |

The asm optimizer runs once to size each function's locals and then
once more for each allocation of the pools, up to four times
(`_POOL_RETRIES`). After the back end's region, step 3 ported the
work that reads the asm program (`link_records`, `place_zp_globals`,
`check_shares`, `prune_unused_locals`, `hargs_need`, `section_items`,
and the formatting of the link metadata). What is left there in Python
reads the TAC or the tables: `changed_param_slots`,
`preferred_symbols`, and `build_metadata`. Without `-O`, `_codegen`
calls `fixed_address_layouts`, `tac_to_asm` with that ABI,
`replace_pseudoregisters`, the region and emission, and
`pinned_slot_symbols`. `plan_zero_page` runs
`tac_to_asm` itself, only when the runtime's block has to move, to
measure the HARGS bytes the program uses.

Every value these read is in the compile's tables or the
`AsmContext`. None of them reads `os.environ`, and the only module
state they read is what `AsmContext` holds: `runtime_layout.current()`,
and the user zero-page globals, which `tac_to_asm` reads through
`pointer_restage.pointer_in_zero_page`. cpp/README.md ("The compile's
tables") describes the C++ form of the tables and the check that they
make the trip from Python and back.

### The helpers the groups share

These are ported and checked on the corpus (cpp/README.md, "Checking
the helpers").

- `replace_pseudoregisters.sizeof` (`sizeof_type` in C++) and
  `size_of_name`, which abi_selection, slot_sharing, zp_globals, the
  sizing passes, the asm optimizer, and replace_pseudoregisters call.
- `zp_slot_naming`'s `register_param_local`, `source_spelling`,
  `parse_pseudo_name`, and `param_slot_symbols`.
- `optimization.var_visit`'s `vals_in`, `uses_in`, `defs_in`, and
  `count_uses`, which the TAC side reads everywhere.
- `slot_sharing.reach_sets`, `coexist`, `share_conflicts`, and the
  methods of `SlotShare`.
- `Pool`, `Range` (a Python `range`), and `RuntimeLayout.bytes`.

These are shared too but belong to one group, which ports them first
and says so to the others.

- The TAC CFG (`passes/optimization/cfg.py`, 340 lines) and
  `recursion_saves._live_out` and `defs_in`, which `find_shares`,
  `plan_saves` (for `tac_to_asm`), and `hw_stack_cycles` read. Group A.
- `ssa_destruction._order_parallel_copies`, which `tac_to_asm` calls
  for the register arguments of a call. Group D.
- An asm CFG whose blocks own their instructions. The ported
  `optimization_asm/cfg` keeps index ranges into the function, and SSA
  construction and destruction edit the blocks in place. Group D.
- `optimizer._all_operands_in`, which regalloc and
  `function_local_sizing` call too. Group F.

### The groups

Each group ports its modules with the conventions of cpp/README.md
("Porting a pass"), registers each capture name it ports with a runner
in `src/passes/stage_registry.cpp`, and checks it on a capture of the
whole corpus. A group that ports a function the table above doesn't
record, such as one of the asm optimizer's sub-passes, adds it to
`STAGE_PASSES` with a signature, as `module:name` when its caller
reaches it through its module. `_optimize_function` calls the
sub-passes by the names it imports, so `to_ssa` is
`passes.optimization_asm.optimizer:to_ssa`.

- **A. The ABI and the call graph, on TAC.** `abi_selection`
  (`select_abi`, `fixed_address_layouts`, `pinned_slot_symbols`),
  `carry_return_choice`, the eligibility parts of `zp_local_allocation`
  (`pool_eligible`, `recursive_pool_functions`, `_determine_eligibility`),
  `recursion_saves`, and the TAC CFG. About 2,300 lines.
- **B. Sharing and the zero-page allocators.** `slot_sharing`
  (`find_shares`, `pinned_param_copies`, `slot_aliases`,
  `changed_param_slots`, `preferred_symbols`), `zp_slot_allocation`,
  `zp_local_allocation.allocate_function_locals` and
  `build_local_slot_symbols`, `zp_globals` (`plan_zero_page`,
  `allocate_zp_globals`, `access_weights`), `runtime_layout`
  (`choose_layout`), and `zp_link_metadata.build_metadata`. About 2,400
  lines. Step 3 ported `check_shares`, `place_zp_globals`,
  `link_records`, `hargs_need`, and `check_hargs`. B reads A's
  `_live_out`, and `plan_zero_page` reads `tac_to_asm` when the
  runtime's block moves, which it can take as a function argument
  until group C lands.
- **C. `tac_to_asm`**, with `pointer_restage`, `significant_bytes`,
  `in_place_member_update`, `shift_cost`, and the pieces of
  `bit_precise`, `split_tables.plane_init`, and
  `compare_subtract.compared_difference` it calls. 5,600 lines in
  three parts that share about 300 lines of the `Translator`'s queries
  (`_size_of`, `_is_*_val`, `make_label`, `_byte_at`, `_hargs`,
  `_translate_helper_call`), which C1 ports first.
  - C1: the driver, calls and the ABI, and memory and pointers (lines
    1566-2316, 2322-2520, 2892-3171, 3176-3678).
  - C2: arithmetic, the inline shifts, divisions, and products, and
    the free shift and divide helpers (723-1414, 3683-4590).
  - C3: casts, unary operators, conditional jumps, and comparisons
    (2521-2891, 4591-5328).
  It calls `plan_saves` from A and `_order_parallel_copies` from D.
  Its labels are numbered by two counters over the whole program, so
  the order of the lowering is part of its output.
- **D. The asm SSA round trip.** `optimization_asm`'s
  `ssa_construction`, `ssa_destruction`, `copy_propagation`,
  `backward_copy_propagation`, `byte_dce`, and `or_zero_absorb`, the
  asm CFG with blocks of their own, and `decimal_arith`'s
  `pseudo_reads`, `pseudo_writes`, `map_reads`, `map_writes`, and
  `early_write_pairs`. About 2,300 lines.
- **E. Register allocation.** `optimization_asm`'s `liveness`,
  `interference`, `coalescing`, `hwreg_eligibility`, and `regalloc`,
  with `register_allocation`'s `_find_fit` and `_blocked_bytes` and the
  `InterferenceGraph`. About 2,200 lines. It works on the SSA form D
  gives, and it can start on captured calls of `compute_liveness` and
  `color_graph` before D lands.
- **F. The program level and the end of the stage.**
  `optimizer.optimize_program` and `_optimize_function`,
  `apply_coloring`, `const_static_fold`, `dead_static`,
  `function_local_sizing`, `address_taken_zp`, `zp_slot_naming`'s
  `local_slot_names` and `compute_local_slot_names`,
  `replace_pseudoregisters` (both forms), `lower_data_load_address`,
  and last the stage's driver in C++, compile.py's
  sequence from `select_abi` to `synthesize_prologue` with the pool
  retries, so that the stage, the region, and emission run as one
  process. About 2,700 lines.

A, C, and D can start at once, and B, E, and F once the helpers they
read from A and D exist or have an agreed interface. The groups should
agree first on the fields of `HwRegEligibility` (E and F), the SSA
names (`<name>.b<k>.v<N>`, D and E), and `_all_operands_in` (E and F).
`Coloring`, `FrameDims`, the ABI, the shares, the pools, and
`ZpGlobals` are already C++ types (src/tagged_json.h lists them).

### What the port has to keep

The surveys found these places where the Python output depends on
something a straightforward C++ port could change.

- `find_shares` keys its bindings by `id()` (slot_sharing.py:327,
  :527), and `allocate_zp_slots` its shares (zp_slot_allocation.py:211,
  :259). The C++ port keys them by index.
- The order of `Coloring.assignments` decides the slot names
  (zp_slot_naming.py:178, the first name from the source at an address
  names it). It is the reverse of the elimination order, then the
  leftovers sorted, then the coalesced names in `rep_map`'s order
  (regalloc.py:505, optimizer.py:458).
- The order of the `Phi`s in a block follows the `defs` dict of SSA
  construction, which decides the order of the moves `from_ssa` writes.
- The symbol table's order decides where `plan_zero_page` places the
  zero-page objects, and its promotion sorts by a float key `-w/size`
  (zp_globals.py:316).
- `const_static_fold` takes bytes of Python ints that may be negative
  or wider than 64 bits.

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
- 20 Nov 2026: the arithmetic, carry, and load/store peepholes of the
  fixed point are ported, 36 of the passes that
  `tools/capture_passes.py` records, among them `carry_known`, which
  runs between the fixed points, and both stages of `adc_commute`.
  Each matches the Python pass on every distinct call of the whole
  corpus in both modes. The calls checked per pass run from 1,180 for
  `carry_known` to 7,052 for `carry_halve` and `carry_in_add`, and the
  Python tests that run each pass on asm are ported to doctest.
- 20 Nov 2026: stage 2 is done. The peephole fixed point and the
  region of `optimized_asm` from `apply_licm` through
  `apply_outline_runs` are one C++ driver (`passes::optimized_region`),
  and so are the unoptimized pipeline's fixed point and long branches.
  With `C6502_CPP_BACKEND=1`, compile.py hands the region to `c6502cpp
  backend` (cpp/README.md, "The back end through c6502cpp") and goes on
  with the result. `tools/compare_backends.py` compiled the 3,576
  compiles of the corpus (1,788 programs, each with `-S` and `-S -O`)
  both ways, and every output, exit status, and message was the same.
  The suite passes with the variable set (4,840 tests, with
  `C6502_CHECK_IR=1`) and without it, and so do the four fuzzers with
  it set (300 programs each of `exprs`, `calls`, and `stmts`, and 100
  of `yarpgen`, none wrong). The times with the variable unset and set
  were 2.1 and 1.9 seconds for guards.c, 15.2 to 16.1 and 14.6 seconds
  for the Lode Runner units, and 18.5 to 19.0 and 10.6 seconds for 20
  `fuzz.stmts` programs. A compile of guards.c spends 0.16 s of its
  1.9 s in the C++ process, and the 20 fuzz programs 2.5 s of 9.8 s,
  of which reading and writing the dumps on the C++ side is about 1 ms
  per call. Most of the C++ time is the fixed point, where
  `redundant_load` and `memory_value_propagation` take the most. Each
  separate compile starts with 0.62 s of Python imports, which is half
  of the units' time, and in the fuzzer most of the Python time is the
  TAC optimizer (with the IR checks the fuzzers turn on), the asm
  optimizer, and the Lark parser.
- 20 Nov 2026: stage 3 is done. `asm_to_asm2`, `asm_emit` with its Y
  peephole, and the work between the region and emission that reads
  the asm program (`link_records`, `place_zp_globals`, `check_shares`,
  `prune_unused_locals`, `hargs_need`, `section_items`, and the
  formatting of the link metadata) are ported. With
  `C6502_CPP_BACKEND=1`, one call of `c6502cpp backend` runs the region
  and all of them, so the `-S` text comes from C++ and Python reads no
  program back (cpp/README.md, "Emission"). The link metadata's records
  that read the TAC, the ABI, or the symbol table are still computed in
  Python and sent with the program. Each ported function matches the
  Python one on every distinct call of the corpus, 11,580 calls, and
  `tools/compare_backends.py` found the same output, exit status, and
  messages for all 3,576 compiles. The suite passes with the variable
  set and without it. The four fuzzers pass with it set, none wrong,
  though they run the simulator harness, which sends only the region to
  C++. A bug found on the way was fixed: without -O, a function that
  read an argument above `Frame(255)` made `asm_emit` raise a
  `ValueError`, and `asm_to_asm2` now reports such an access as a
  `FrameSizeError` (`fuzz.exprs` seed 43). The first version of that
  fix rejected any function whose arguments reached past `Frame(255)`,
  read or not, which turned away two programs that compiled correctly
  before, among them tests/chapter_18's
  `param_calling_conventions.c`, so the check moved to the frame
  accesses themselves. With the variable unset and set, guards.c
  took 2.2 and 1.9 seconds, the Lode Runner units 15.8 and 14.0, and
  20 `fuzz.stmts` programs 18.8 and 10.6. A compile of guards.c spends
  0.22 s in the C++ process. The rest is Python's imports (0.6 s), the
  TAC optimizer with the inliner's cost estimates, the asm optimizer,
  and the parser.
- 20 Nov 2026: the groundwork of step 4 ("Step 4" above). The symbol
  and type tables, the ABI, the shares, the pools, the colorings, the
  frames, and the zero-page plan are C++ types, read from and written to
  the capture harness's tagged JSON byte for byte (cpp/README.md, "The
  compile's tables"). `tools/capture_passes.py --passes stage` records
  the 24 entry points of the stage that emission's table doesn't, and
  `check-captures` reads every value of each of the 30,273 distinct
  calls of the corpus into C++ and
  writes it back unchanged. The helpers the groups share (`sizeof`,
  `size_of_name`, the slot-name spellings, `var_visit`, `reach_sets`,
  `Pool`) match on about 1.04 million values of the corpus
  (`capture_helpers.py --stage-only`), and `synthesize_prologue` is
  ported and matches on all 937 of its distinct calls. The capture
  found two bugs, fixed in Python, since neither pass is ported:
  `c99_to_tac` folded an out-of-range floating cast to a constant of the
  whole value (a 1000-bit `ConstULong` for `(unsigned long)200e300`),
  and the type checker let fourteen invalid programs through to crash
  in later passes, such as `10 & ptr` and `foo(s)` with a struct for an
  `int` parameter.
- 20 Nov 2026: group D of step 4, the asm SSA round trip. `to_ssa`,
  `copy_propagate`, `backward_copy_propagate`, `byte_dce`,
  `absorb_zero_load`, `from_ssa`, and `_order_parallel_copies` (as
  `order_parallel_copies` in `src/passes/optimization_asm/parallel_copies.h`,
  for group C) are ported, with an asm CFG whose blocks own their
  instructions (`owned_cfg.h`) and `decimal_arith`'s `pseudo_reads`,
  `pseudo_writes`, `map_reads`, `map_writes`, and `early_write_pairs`.
  Each is a stage call of `STAGE_PASSES` and matches the Python function
  on every distinct call of the corpus: 3,670 of `to_ssa`, 6,275 each of
  `copy_propagate` and `backward_copy_propagate`, 5,975 of `byte_dce`,
  3,926 of `absorb_zero_load`, 6,300 of `from_ssa`, and 8,499 of
  `order_parallel_copies`.
