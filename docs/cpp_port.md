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
  and linking. On 21 Nov 2026 Robert widened the scope to the rest of
  the repository's Python: the simulator (`sim/`, on py65), the test
  suite, and the fuzzers move to C++ too, so that no Python is left.
  Until step 9, the simulator, the tests, and the fuzzers stay in
  Python and drive the C++ executables.
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
9. **The simulator, the tests, and the fuzzers.** A C++ 6502 simulator
   replaces py65 and `sim/`, with the Python hooks that stand in for
   the runtime helpers that have no 6502 code yet. It is checked
   against py65 instruction by instruction (the registers, the flags,
   the memory written, and the cycles) on the corpus's programs and on
   the games' scripted runs. The test suite, the TAC interpreter
   `tac_sim`, and the fuzzers with their reducer move to C++ programs
   and doctest, and then no Python is left in the repository.

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
- The TAC CFG's dominance frontiers and natural loop bodies, and the set
  of reachable blocks `natural_loops` walks, are Python sets of block
  ids, and SSA construction and the loop passes walk them. Their order
  is the order of CPython's hash table, which `PyIntSet`
  (`cpp/src/py_int_set.h`) reproduces.

## Step 5: the TAC optimizer, c99_to_tac, and the inliner

Step 5 is the part of a compile between the type checker and the TAC
that `C6502_CPP_BACKEND=1` handed to C++ when step 4 was done:
`c99_to_tac`, the passes of
`translate_for_codegen`, those of `_optimized_tac`, and the TAC
optimizer they call. `unroll_program` works on the c99 AST and belongs
to step 6. This section lists the calls, what each reads, the helpers
the passes share, and the groups in which several agents can port them
at once. The survey was made on 21 Nov 2026 from compile.py at
039efa2.

### The calls

Under `-O`, `translate_for_codegen` and `_optimized_tac` make these
calls in this order. Without `-O` only `c99_to_tac` runs, and the
`--tac -O` stage runs `c99_to_tac`, `inline_functions`, and
`optimize_tac`. Each has an entry in
`tools/capture_passes.TAC_STAGE_PASSES` under the name in the first
column.

| Capture name | Python | Lines | Reads besides its IR | Returns, and what it changes |
|---|---|---|---|---|
| `c99_to_tac` | `c99_to_tac.translate_program` | 5350 | the type-checked c99 program, the symbol and type tables, `check_marked_calls` | the TAC program. It changes the c99 program (`short` and `nullptr` everywhere, `bool` and the bit-precise types in the top-level declarations), adds a symbol per temporary, gives the symbols their erased types with `returns_bool` and `was_bool`, and erases the type table's member types. |
| `inline_functions` | `optimization.inline_functions` | 640 | every symbol (for the next free `@N` and `%N`), the c99 declarations, `abi_selection`'s `_address_taken`, `_function_decls_by_name`, `_has_register_local`, `_has_register_request`, and `optimize_function` of a copy of each candidate | the TAC program, with a symbol for each copied local |
| `return_pointers_in_zero_page` | `return_in_zero_page` | 120 | symbols, `_address_taken` | the TAC program, with an `F@ret` zero-page static |
| `row_address_tables` | `optimization.row_address_tables` | 440 | symbols, types, `split_tables.plane_init`, `strength_reduction._two_power_terms`, `shift_cost` | the TAC program, with the `A@rows`, `A@lo`, and `A@hi` tables |
| `optimize_tac` | `optimization.optimize_program` | 330 | symbols | the TAC program, with `optimize_function` of each function |
| `tac_optimize_function` | `optimization.optimizer.optimize_function` | | symbols | the function, run through the driver below |
| `dispatch_const_pointer_arrays` | `optimization.dispatch_pointer_array` | 370 | symbols (the `AddressInit`s of `static const` pointer arrays) | the TAC program |
| `split_tables` | `optimization.split_tables` | 540 | symbols | the TAC program, with each split table's symbol replaced by an array of bytes in plane order |

`optimize_function` runs `PhaseDriver.apply` (framework/driver.py) on
one function. Each pass takes the function and a `PassContext`, which
holds the symbol table and, in SSA form, `ssa_dsts`, the names SSA
construction renamed. The driver runs the passes of six lists in this
order, and `tools/capture_passes.tac_passes()` records each call as
`tac_<name>` with the pass's `name`.

1. `pre_ssa`, once each: `rotate_signed_countdown_loops` (700 lines)
   and `noreturn_calls` (80).
2. `to_ssa` (`tac_to_ssa`, 650 lines), which returns the function and
   `ssa_dsts` and adds a symbol for each SSA name.
3. `pre_fixedpoint`: `fold_static_const_reads` (150), which reads
   every symbol of the table.
4. `fixedpoint`, swept until a sweep leaves the function equal to what
   it started with, or 256 times: `constant_fold` (1200),
   `distribute_constant` (155), `fold_into_constant_phi` (145),
   `reduce_strength` (530), `fold_producer_jumps` (the rules of
   `cmp_zero_jump_fold`, `and_zero_jump_fold`, `lnot_jump_fold`, and
   `nonnegative_compare`, 1350 in all), `eliminate_dead_loops` (225),
   `thread_constant_jumps` (370), `eliminate_unreachable_code` (240),
   `eliminate_common_subexpressions` (180), `hoist_loop_invariants`
   (120), `copy_propagate` (330), `eliminate_dead_stores` (200),
   `read_stored_globals` (170), `fold_copies_in_fixedpoint` (300),
   `reassoc_constants` (260), `reassoc_offset_constant` (145),
   `direct_member_access` (195), `recognize_indexed_store` (130),
   `recognize_indexed_load` (110), `fold_truncate_extend` and
   `fold_extend_extend` (230), `sink_increments` (405),
   `sink_and_past_branch` (645), `narrow_widened_arith` (630),
   `narrow_byte_range` (285), and `narrow_byte_bitwise` (190).
5. `post_fixedpoint`, once each: `phi_offset` (265),
   `static_indexed_access` (220), `recognize_indirect_indexed` (605),
   `hoist_subtract_to_compare` (375), `hoist_divmod_pairs` (325), and
   `count_loops_down` (230).
6. `from_ssa` (`tac_from_ssa`, 330), which adds a symbol for each
   temporary that breaks a cycle of copies.
7. `post_destruction`, once each: `fold_copies_post_destruction`,
   `move_steps_onto_edges` (190), and `split_three_way_compares` (615).
8. `post_destruction_fixedpoint`, swept like the first:
   `fold_short_circuit_jump` (550) and `thread_known_compares` (565).

Most passes read the symbols of the names they see. Seventeen add
symbols for new temporaries, each named by probing the table for the
first free `<base>.<tag><k>`, and thirteen add the new names to
`ssa_dsts` too. Under `C6502_CHECK_IR=1` the driver checks the TAC
after every pass, which the C++ driver can leave to one check of its
result, as step 4 does. No pass reads `os.environ` or module state
apart from the driver's `observer`, which only `fuzz/tac_passes.py`
sets.

### The helpers the groups share

These are ported and checked on the corpus.

- From earlier steps: the TAC CFG (`passes::tac_cfg`), `var_visit`'s
  `vals_in`, `uses_in`, `defs_in`, and `count_uses`,
  `recursion_saves::defs_in`, `shift_cost`, `split_tables::plane_init`,
  `compare_subtract::compared_difference`, and the parts of
  `bit_precise` that code generation reads.
- `var_visit.rewrite_uses_in` (`passes::rewrite_uses_in`), which the
  framework's operand rewrite, `phi_constant_fold`,
  `read_stored_global`, and `compare_subtract` call.
- `value_range` (`passes::value_range`: `int_width`, `type_max`,
  `is_unsigned_type`, `max_value`), which strength reduction,
  `narrow_byte_range`, `nonnegative_compare`, `lnot_jump_fold`,
  `short_circuit_jump_fold`, and `known_compare_thread` read.
- The run loops of framework/ (`passes::framework` in
  `src/passes/optimization/framework.h`): `PassContext`, the window scan
  of `WindowPass` (`run_window`) and of `RuleSet` (`run_rules`, with
  `RuleEnv`, `single_use`, and `observable_names`), the def-use walk of
  `DefUsePass` (`run_def_use`, with `DefUseEnv` and `DefUseRewrite`),
  `SinkPass` (`run_sink`), and `OperandRewritePass`
  (`run_operand_rewrite`). The pattern DSL of framework/patterns.py
  isn't ported. Each loop takes one function that matches and rewrites,
  testing the instruction's kind and fields directly, and returns none
  where the Python pattern doesn't match or the rewrite declines.

These are shared too but belong to one group, which ports them first
and says so to the others.

- `fp_arith` (250 lines) and `builtin_functions`' `bcd_add`, `bcd_sub`,
  `bin_to_bcd`, and `stdbit`, which `c99_to_tac` and constant folding
  both call. Group C.
- `c99_to_tac._tac_const_for`, which `static_const_fold` calls. Group A,
  or group C as a copy until A lands.
- `cmp_zero_jump_fold.extend_source_unchanged` and
  `narrow_widened_arith`'s `var_unchanged`, `_register_temp`,
  `_byte_width`, `_is_unsigned`, and `_narrow_constant`, which
  `recognize_indirect_indexed` and `sink_increment` read. Group D.
- `extend_add_chain` (195 lines), whose `is_1_byte_val` constant folding's
  `distribute_constant` reads. Group F.
- `constant_folding._fold` and `_is_integer_const`, which
  `phi_constant_fold` and `jump_thread` read, and
  `strength_reduction`'s `_zero_for_c99_type`, `_scalar_int_size`, and
  `_two_power_terms`, which `narrow_byte_range`, `reassoc_offset`, and
  `row_address_tables` read. Group C.
- `split_tables._width`, which `recognize_indirect_indexed` reads.
  Group H.

### The groups

Each group ports its modules with the conventions of cpp/README.md
("Porting a pass"), gives each capture name it ports a runner in
`src/passes/optimization/tac_stage.cpp`, as `tac_fold_truncate_extend`
has, and checks it on a capture of the whole corpus (`--passes` with
the group's names). The lines below count the Python modules.

- **A. `c99_to_tac`**, about 5,350 lines in two parts that share the
  `Translator`'s queries (`make_temporary_variable_name`, `make_label`,
  `_sizeof`, the volatile helpers), which A1 ports first.
  - A1: the free functions of the module (lines 419-1799: the erasure
    of `short`, `nullptr_t`, `bool`, and the bit-precise types, the TAC
    constants, the static initializers, the index scales), and the
    Translator's driver, builtins, `<stdbit.h>` loops, functions,
    statics, blocks, and declarations (1800-2577).
  - A2: statements, calls, expressions, pointer arithmetic,
    initializers, lvalues and subscripts, increments, compound
    assignments, casts, the bit-precise padding, conversions to
    `bool`, and the short-circuit operators (2578-5333).
  Its temporaries and labels are numbered by two counters over the
  program, so the order of the lowering is part of its output, and it
  changes its three inputs in place, which its runner reports on the
  `mutated` line.
- **B. The driver and SSA.** `optimizer.py`, `PhaseDriver.apply` with
  its two fixed points and their caps, `ssa_construction`,
  `ssa_destruction`, and the two passes before SSA, `loop_rotate` and
  `noreturn_call`. About 2,100 lines. The driver can't run in C++ until
  every pass of its lists is ported, so B ports it with a table of the
  passes, which the other groups fill, and checks SSA construction and
  destruction on their own captures (`tac_to_ssa`, `tac_from_ssa`).
- **C. Folding.** `constant_folding` (with the const-array and plane
  loads), `phi_constant_fold`, `static_const_fold`,
  `distribute_constant`, `strength_reduction`, `reassoc_const`,
  `reassoc_offset`, and `fp_arith` and `builtin_functions`. About 3,050
  lines.
- **D. Jump folds and narrowing.** `cmp_zero_jump_fold`,
  `and_zero_jump_fold`, `lnot_jump_fold`, `nonnegative_compare` (the
  rules of `fold_producer_jumps`, which run as one `RuleSet` in that
  order), `narrow_widened_arith`, `narrow_byte_range`, and
  `narrow_byte_bitwise`. About 2,450 lines.
- **E. Dataflow over the CFG.** `copy_propagation`,
  `dead_store_elimination`, `unreachable_code_elimination`,
  `dead_loop_elimination`, `jump_thread`, `cse`, `licm`,
  `copy_folding` (both phases), and `read_stored_global`. About 2,130
  lines.
- **F. Addressing and sinking.** `extend_add_chain`, the three
  recognizers, `direct_member_access`, `static_indexed_access`,
  `phi_offset`, `sink_increment`, and `sink_and_past_branch`. About
  2,770 lines.
- **G. After the fixed point and after SSA.** `compare_subtract`,
  `divmod_hoist`, `count_down`, `step_on_edge`, `three_way_compare`,
  `short_circuit_jump_fold`, and `known_compare_thread`. About 2,850
  lines.
- **H. The program level.** `return_in_zero_page`,
  `row_address_tables`, `dispatch_pointer_array`, `split_tables`, and
  last `inline_functions`, whose cost estimates run the whole TAC
  optimizer on copies of the callees, so it can be checked only once B
  to G are done. Then the sequence of `translate_for_codegen` and
  `_optimized_tac` as one C++ function, so that compile.py can hand
  C++ the c99 program and the tables. About 2,100 lines. The tests
  that check a `static` function's own code turn the inliner off by
  patching `compile.inline_functions` (`tests/no_inlining.py`), so the
  request to C++ needs a way to say so.

A, B, C, D, E, F, and G can start at once. D's and F's helpers are read
across the two groups, and C's by D, E, and H, so those groups agree
first on the signatures of the helpers listed above.

### What the port has to keep

- **The order of sets.** Two compiles of the corpus with
  `PYTHONHASHSEED` 1 and 2 (3,156 compiles: the examples, the games,
  the chapters, and seeds 0-29 of the three fuzzers, each with `--tac
  -O` and `-S -O`) gave the same output for every one, so no output of
  the corpus depends on the order of a set of strings. The sets of
  block ids that SSA construction (dominance frontiers),
  `dead_loop_elimination`, `licm`, and `jump_thread` (loop bodies) walk
  iterate in the order of CPython's table, which `PyIntSet` reproduces.
- **Identity.** Five passes find an instruction's index by identity
  (`cmp_zero_jump_fold.py:218`, `narrow_byte_bitwise.py:118`,
  `narrow_byte_range.py:120`, `narrow_widened_arith.py:332`, and
  `recognize_indirect_indexed.py:359` and `:487`), which picks the
  first position of an object that sits at two. A C++ port that passes
  the index along differs only for such a shared object, which is worth
  checking for when a capture differs. The framework and
  `rewrite_uses_in` test `is` to see whether anything changed, and the
  driver's fixed point compares by `==`, so a C++ pass that rebuilds an
  equal function changes nothing.
- **Two indexes of definitions.** `DefUsePass` indexes each name by the
  instruction whose `dst` field is that Var, which takes an
  `IndirectIndexedLoad`, and `RuleSet` by `defs_in`, which leaves it
  out (`framework::dst_name` and `make_rule_env`).
- **Ints wider than 64 bits.** `value_range` multiplies and shifts
  bounds of up to 2^64 - 1, which the C++ port tests against the
  destination's largest value before forming the result. Constant
  folding multiplies 8-byte constants, and `double_bits_to_int` gives
  the exact integer of a double up to 2^1024, which its callers reduce
  modulo 2^N, except that `c99_to_tac._fp_to_integer` returns it whole
  for a `bool` target.
- **Floats.** `fp_arith` runs through numpy, and `np.float32` of a
  string parses it as a double first, so a decimal string or an integer
  converts to a `float` with two roundings:
  `1.0000001788139343261718749` gives 0x3F800002, where one rounding
  gives 0x3F800001. The C++ port converts through a double too. The
  arithmetic is the SSE unit's in both, which C++ keeps only without
  `-ffast-math` and with no contraction into fused multiply-adds.
- **Names and counters.** `c99_to_tac` numbers `%N` and its labels over
  the program, and the passes number their labels per function
  (`.<fn>@ssa_split@N`, `@scfold@N`, `@cthread@N`) and their
  temporaries by probing the symbol table. The inliner takes its next
  `@N` and `%N` from the largest in the whole table.
- **The inliner's estimate.** `_optimized_cost` runs the optimizer on a
  deep copy of the callee and a shallow copy of the symbol table, which
  shares its `Symbol` objects with the compile's table, so a pass that
  changed a symbol in place would change the compile's table during an
  estimate.

### The captures

`tools/capture_passes.py --passes tac` records each call the driver
makes on a function, and `--passes tac-stage` the calls of
`TAC_STAGE_PASSES`. A call of the driver is recorded with the
function, and with the keyword arguments `symbols` and `ssa_dsts`: the
symbols the call read before it wrote them, in the table's order, and
the SSA names. A view of the table (`_recording_table`) passes every
read and write on to the compile's table and notes the names, so that
a record holds a few symbols where the whole table would take most of
its bytes, and a call that walks the whole table (`items`, `len`), as
`fold_static_const_reads` does, holds all of it. The `mutated` line
holds the symbols the call read or added, and the SSA names, when they
changed. `c99_to_tac` changes the c99 program it is given, which its
`mutated` line holds under `input`. `c6502cpp check-captures` reads
each record of step 5 with the signatures of
`src/passes/optimization/tac_stage.cpp`, which take a c99 program, a
TAC program, or a TAC function as their input, and a TAC dump as their
result.

On 21 Nov 2026 the capture of every call of step 5 over the whole
corpus took 254 seconds with 14 processes. Every value of its 355,075
distinct records made the trip to C++ and back, from 1,110 records of
`split_tables` to 12,931 of `tac_constant_fold`, which took
`check-captures` 31 minutes in one process. The capture found that
unreachable-code elimination changed the Phis of its input in place
(Status, 21 Nov 2026).

## Step 6: the front end

Step 6 is the part of a compile before the type-checked c99 program
that `C6502_CPP_BACKEND=1` handed to C++ when step 5 was done: the
preprocessor (pcpp with the hooks of preprocessor.py), the parser, the
unroller under `-O`, the three passes that resolve names, string
lifting, and the type checker. Since group D landed, the switch hands
C++ the source text, and the C++ side runs everything from the
preprocessor to the `-S` text (the Status entry of group D). This
section lists the calls, what each reads, the pieces the groups share,
the groups, and separate plans for the parser and the preprocessor,
since each of them has to reproduce a library (Lark and pcpp) along
with the Python code around it. The
survey was made on 21 Nov 2026 from compile.py at 00d161f.

### The calls

`compile._compile_c` and `_resolved` make these calls in this order,
and the type checker runs from `_run_stage`. `unroll_program` runs only
under `-O`. Each has an entry in `tools/capture_passes.FRONT_PASSES`
under the name in the first column. The line counts are of the Python
modules each call runs.

| Capture name | Python | Lines | Reads besides its input | Returns, and what it changes |
|---|---|---|---|---|
| `preprocess` | `preprocessor.preprocess` and pcpp 1.30 | 591, and 2,519 of pcpp (preprocessor.py 1,400, evaluator.py 725, parser.py 394) with PLY's lex and yacc | `pcpp_args`, the file name, the files it includes from `-I` and include/, and pp_numbers | the text, and the `_Bool` warnings through diagnostics |
| `parse` | `parser.parse`, with c99.lark, lexer.py, and lark_cache.py | 4,884, 854, 283, 139 | the type checker, constant_expression, and designators for its folds, pp_numbers, fp_arith, builtin_functions, and Lark | a new c99 program |
| `unroll_program` | `optimization_ast.unroll` | 772 | `_common_type`, `_promote_integer`, and `_coerce_int_to_type` of the type checker | a new program, in which each copy of a body is a `copy.deepcopy` |
| `identifier_resolution` | `identifier_resolution.resolve_program` | 1,322 | `builtin_functions.is_builtin` | a new program with `@N.orig`, `@N.tag`, and `@N.compound_literal` names from one counter |
| `string_lifting` | `string_lifting.lift_program` | 564 | `designators.normalize_initializer` | a new program with a `str@N` static for each literal it lifts, in front of the declarations |
| `label_resolution` | `label_resolution.resolve_program` | 344 | | a new program with `.<fn>@<label>` labels |
| `loop_labeling` | `loop_labeling.label_program` | 422 | | a new program with `.loop@N`, `.switch@N`, `.case@N`, and `.default@N` labels from one counter |
| `type_checking` | `type_checking.check_program` | 6,206, with constant_expression 663, designators 401, and stdbit_functions 123 | | the program stamped in place, the symbol table, and the type table |

The first five passes build new programs, and none of them changed its
input on 677 programs of the corpus, though every output shares nodes
with the input. The type checker changed its input on 671 of 674, and
returns the same root. It stamps each expression's `data_type`, wraps
operands in implicit `Cast`s and `AddressOf`s, replaces initializers and
case values, swaps the operands of `3[a]`, makes a file-scope
`constexpr` object `static`, and `stdbit_functions.add_definitions`
rewrites the declaration list. Each pass raises its own error class,
which `compile._source_errors` lists, and the corpus's compiles that
fail stop in these passes: 221 calls of `parse` raise (173
`UnexpectedToken`, 11 `UnexpectedCharacters`, 36 `ParserError`, and one
`NotImplementedError`), 79 of `identifier_resolution`, 10 of
`label_resolution`, 10 of `loop_labeling`, and 313 of `type_checking`.

Besides its IR, the front end keeps this state.

- **The parser's context** (`_ParseContext`, parser.py:278-437), made
  anew by each `parse`: a stack of scopes, each with its names
  (`_Binding`: a typedef's type, an enumeration constant's value and
  type, a `constexpr` object's value, the type of an object for
  `typeof` and `auto`) and its struct, union, and enum tags; a scope per
  brace and per `for`; the stack of specifier lists; the types of the
  last two tokens; the counter of `__anonymous_struct_N` and
  `__anonymous_union_N`; the stack of the fixed types of enums; and
  every struct body parsed so far, which the folds read.
- **The type checker's tables.** `SymbolTable` and `TypeTable` are
  C++ types already (src/passes/type_checking.h, from step 4), and so
  are every attribute and initial value. An `Initial` of a `float` or a
  `double` holds the integer bits of the IEEE pattern, so no Python
  float reaches the tables. The named constants of
  `constexpr` objects (`TypeChecker._constexpr`), the stack of tag
  scopes, and the stack of resolvers in constant_expression
  (`_RESOLVERS`, `named_constants`) live only while the checker runs,
  and the parser makes a checker of its own for each fold.
- **The diagnostics.** Only the preprocessor issues a warning (for each
  `_Bool`), through `diagnostics.warn` into the innermost
  `diagnostics.collecting()` block, which compile.py opens around each
  C input.

### The pieces the groups share

These are ported and checked on the corpus (cpp/README.md, "The front
end's calls").

- `pp_numbers` (`src/pp_numbers.*`), which the lexer, the parser, and
  the preprocessor's `#if` read. A constant may have any number of
  digits, so `IntegerConstant` holds the exact bit length and the value
  when it fits in 127 bits.
- `diagnostics` (`src/diagnostics.*`), with the innermost block kept per
  thread.
- Python's `repr` of every IR value (`asdl::py_repr`, generated by
  `asdl.py --cpp` with the dump format), which 65 of the type
  checker's 201 messages and those of identifier_resolution and
  constant_expression show, as in `binary operator on non-object types:
  Structure(tag='s'), Pointer(referenced_type=Int())`.
- The type checker's type queries (`src/passes/type_checking_types.*`,
  namespace `passes::type_checking`): `_strip_quals`, the predicates,
  `_display_name`, `_c_type`, `_int_width`, `_int_bits`, `_is_signed`,
  `_rank`, `_promote_integer`, `_coerce_int_to_type`, `_common_type`, and
  `_common_bit_precise_type`. The parser, designators, and the unroller
  read some of them, and every part of the type checker reads most.

These are shared too but belong to one group, which ports them first
and says so to the others.

- `constant_expression` (`evaluate_integer_constant_expression`,
  `named_constants`, and `named_constant`), which the type checker calls
  12 times and the parser 12 times. Group T-A.
- The type checker's `_sizeof` (constant_expression has a copy of it),
  `_check_well_formed_type`, `_convert_to`, and `_decay_if_array`, which
  every part of the type checker calls. Group T-A.
- `designators.normalize_initializer`, which string_lifting and the type
  checker call, and `_size_from_initializer`, which the parser calls.
  Group T-B.
- `_check_exp`, `_check_struct_decl`, `_check_block_var`, and
  `_named_constant`, which the parser's folds call on a checker of their
  own (parser.py:2422-2663). Groups T-B, T-C, and T-D, behind a
  `TypeChecker` class whose declaration T-A writes first.

### The groups

Each group ports its modules with the conventions of cpp/README.md
("Porting a pass"), gives each capture name it ports a runner in
`src/passes/front_stage.cpp`, as `loop_labeling` has, and checks it on a
capture of the whole corpus (`--passes front`, or the group's names),
errors included. The line counts are of the Python modules.

- **P. The preprocessor**, about 3,100 lines of Python and pcpp, which
  the plan below describes. It needs only pp_numbers and diagnostics, so
  it can start at once.
- **R. The parser**, about 6,200 lines with the grammar and the lexer,
  in three parts, which the plan below describes. R1 can start at once,
  and R2 and R3 can build and check every AST shape that needs no fold
  while the type checker is ported, since the folds reach the parser
  through one interface (`_fold_integer_constant`, `_expression_type`,
  and `_fold_named_constant`).
  - R1: the lexer, the table of Lark's LALR automaton and the recognizer
    that runs it, and the syntax errors.
  - R2: declarations, declarators, specifiers, typedef names, struct,
    union, and enum, attributes, `auto`, `typeof`, and `constexpr`
    (parser.py:525-1678, 1991-2847, and the methods of 2849-3350,
    3506-3606, and 3717-4169).
  - R3: statements, expressions, constants, initializers, designators,
    and compound literals (1679-1990, 3352-3505, 3607-3716, and
    4170-4673).
- **T. The type checker and constant_expression**, about 7,300 lines in
  four parts that share the `TypeChecker` class's declaration
  (type_checking.py:2407-2537, its fields, tag scopes, and `_require_*`
  methods), which T-A writes first.
  - T-A: the type queries not yet ported, `_sizeof`,
    `_check_well_formed_type`, the null pointer constants,
    `_convert_to`, `_decay_if_array`, and all of constant_expression
    (type_checking.py:127-490 and 1200-2406), about 1,900 lines.
  - T-B: static initializers and `constexpr` (491-1199 and 3637-4438)
    with designators.py, about 1,700 lines.
  - T-C: `check_program`, file-scope and struct declarations, layouts,
    functions, calls and the builtins' checks, and statements
    (2538-3636 and 4439-4775) with stdbit_functions.py, about 1,600
    lines.
  - T-D: expressions, `_check_exp` and its cases (4776-6197), about
    1,420 lines, with 63 of the raise sites and most of the messages
    that show a repr.
- **N. The name passes.** `identifier_resolution` (1,322 lines),
  `string_lifting` (564, which needs T-B's designators), and
  `label_resolution` (344). `loop_labeling` is ported.
- **U. The unroller** (772 lines), which reads the ported type queries
  and two module globals of file-scope constant arrays that it sets
  around each call (`_CURRENT_CONST_ARRAYS`, unroll.py:98-142).
- **D. The driver.** Last, compile.py's `_compile_c` up to the type
  checker as one C++ function, so that `C6502_CPP_BACKEND=1` hands C++
  the source text and the arguments, and the request no longer carries
  the c99 program and its tables.

P, R1, T-A, N's identifier_resolution and label_resolution, and U can
start at once. T-B, T-C, and T-D start once T-A's header is agreed, and
R2 and R3 connect the folds once T is done.

### The plan for the parser

The decision is a parser written by hand, recursive descent with
precedence climbing for expressions, that gives parser.py's ASTs and
messages. parser.py builds the AST inside Lark's LALR parse, with the
transformer `_ASTBuilder` called as each rule reduces (Lark calls it
from `feed_token`, so a `ParserError` from a callback isn't wrapped in
a `VisitError`), and `_TypedefLexer` retypes an identifier to
`TYPEDEF_NAME` when the name is a typedef in scope and the parser
could shift a `TYPEDEF_NAME` there. The hand-written parser has to
reproduce these.

- **The AST shapes.** A declarator stays a Lark `Tree` that
  `_apply_declarator` walks, so the C++ parser keeps a declarator tree
  of its own. A forward declaration parses as a `var_decl` and becomes
  a `FunctionDecl` with no body when its type is a `FunType`, with
  `param_registers` of `""` for each parameter and the parameters of a
  function typed through a typedef None (the empty string in C++, as
  `cpp_backend` sends it). A struct body is a `StructDecl` in front of
  its declaration, and a typedef declaration leaves only forward
  `StructDecl`s. An enumeration constant is a `Constant(ConstInt(v &
  0xFFFF))`, or a constant of the enum's fixed type, or a `Cast` of a
  `ConstInt` for `char`, `signed char`, `short`, and `unsigned short`
  (`_enum_constant`, 2339). A character constant is a `ConstInt` of its
  byte, adjacent strings are joined as one Latin-1 string, and an
  attribute's string argument is the raw text of the literal. A typedef's
  type is deep-copied at each use, while the declarators of one
  declaration share their specifier's type object, which matters only to
  code that compares by identity.
- **The order of the actions.** The lexer runs one token ahead: token
  k is lexed after token k-1 is fed, so the action of a construct that
  ends at k-1 runs after k is lexed. That decides when scopes open and
  close (the lexer pushes a scope at each `{` that isn't a struct,
  union, or enum body, including initializer braces), when
  `init_declarator` declares its name (after its initializer, so in
  `int T = sizeof(T);` the inner `T` is still the typedef), the retyping
  of the token after a `for` statement (`retype_lookahead`), and that a
  `LexError` of token k comes before a `ParserError` of a reduction that
  k causes. A recursive-descent parser that lexes the next token
  whenever it takes one, and runs a construct's action when the
  construct returns, has the same order.
- **The typedef positions.** Lark's table has 976 states, and
  `TYPEDEF_NAME` can be shifted in 43 of them. Every one is a place
  where a type can begin: the start of a declaration or block item
  (after attributes and storage classes too), after the qualifiers of a
  specifier list, a member, a parameter after `(` or `,`, after `(` in
  an expression (a cast, a compound literal, or a parenthesized
  expression), after `sizeof (`, `typeof (`, `typeof_unqual (`, and `for
  (`, after `enum ... :`, and the start of a `type_name`. No state
  shifts one after a type specifier, which is why `int T;` declares a
  variable named T.
- **The folds.** An array size, an enumeration value, a `_BitInt`
  width, a designator's index, and a `constexpr` initializer are folded
  through a `TypeChecker` of the parser's own (`_fold_integer_constant`,
  `_checker_with_named_constants`, `_fold_named_constant`), with each
  named constant and struct body seen so far, and `typeof` and `auto`
  type an expression the same way (`_expression_type`). So the parser's
  port depends on T, and the parts of a capture that need no fold can be
  checked before it.
- **The syntax errors.** The corpus's 221 rejected programs give 194
  distinct messages, 184 of them Lark's with 37 distinct sets of
  expected terminals. A pytest plugin that recorded each exception of
  `parse` during the suite counted 1,052 of them, with 471 distinct
  messages. Together they are 509 distinct messages: 243 of Lark's,
  with 42 distinct sets of expected terminals, and 266 of the parser's
  own (`ParserError`, `LexError`, and `NotImplementedError`). An
  `UnexpectedToken` reads

  ```
  Unexpected token Token('RETURN', 'return') at line 3, column 5.
  Expected one of: 
  	* AMP
  	...
  ```

  with the terminals of Lark's `accepts`, which feeds each terminal of
  the state's action table to a copy of the parser and keeps those that
  end in a shift, or the raw set of the table when that is empty, sorted
  by `_sort_expected` (by code point, so `$END` comes first). An
  `UnexpectedCharacters` has the context line and a caret, the 63
  terminals of the lexer, and a `Previous tokens` line. A program that
  ends early gets an `UnexpectedToken` of `Token('$END', '')` at the
  last token's position. `_c23_keyword_as_name`,
  `_enum_type_without_list`, and the note about `__attribute__` (added
  when the source holds that word anywhere) read `e.expected`, the raw
  set. Lines and columns count code points, and only `WS` and
  `ATTR_OPEN` hold newlines.

The proposal for the expected sets is a table generated from c99.lark
by Lark itself: the action table and the gotos of
`_LARK.parser.parser._parse_table`, built with the four start rules
parser.py builds (`start`, `lex_only`, `declarator`, `type_name`),
since the states the start rules share carry `$END` among their raw
lookaheads, and the 715 rules' origins and lengths. That is 976 states
by 112 terminals, 52,540 of the cells holding an action, plus 2,702
gotos over 100 nonterminals, about 220 KB as a dense table of 16-bit
cells. The state numbers change from one build of the table to the
next, but the sorted sets don't. The C++ parser feeds each token, as
retyped, to a recognizer that runs this table without actions, before
the recursive descent takes the token. The recognizer then reports the
syntax error at Lark's token with Lark's `expected` and `accepts`, having
made the same reductions of merged lookaheads first. It also answers
whether `TYPEDEF_NAME` can be shifted, as `_can_shift` does. LALR
detects an error at the first token that can't extend a viable prefix,
and a correct recursive-descent parser of the same language fails at
the same token, but the expected set depends on the LALR state after
its reductions, which the descent doesn't know. The descent has to copy
the grammar's two shift-reduce resolutions, the dangling `else` and
`(void)`. A test (`tests/test_asdl_cpp.py` is the model) regenerates the
table and compares it with the checked-in one, so c99.lark and the
table can't drift apart until c99.lark stops being the source of truth.
The one place this can still differ is a `ParserError` raised by the
action of a reduction that the recognizer makes before the error, which
Python raises in place of the syntax error. A differential run of the
two parsers on the fuzzers' programs and on mutated corpus files would
find it.

The lexer is Lark's basic lexer, which takes the first terminal in
Lark's order that matches (longest regex source first, for terminals of
equal priority), whatever the length of its match, so `08` is `0` and `8`, `1.e`
is `1.` and `e`, and `1e` is `INVALID_NUMBER`. `WS` is `[ \t\n\f]+`, so
a carriage return or a vertical tab is an `UnexpectedCharacters`. The
possessive quantifiers and lookaheads of `INVALID_NUMBER` have no
`std::regex` form, so every terminal is matched by hand and checked on
the corpus's tokens and on tables of spellings.

### The plan for the preprocessor

pcpp's output has to come out byte for byte, so the C++ preprocessor
reproduces pcpp 1.30 with preprocessor.py's hooks: about 2,000 to 2,300
lines in seven parts, which one agent can port in this order.

1. The lexer, in PLY's order (the function rules in the order of their
   source lines, then the string rules by the length of their regex,
   with VERBOSE and UNICODE flags): `CPP_INTEGER` is pp_numbers'
   pp-number, `\w` is Unicode, `^=` lexes as `^` and `=`, an unclosed
   `/*` or `"` is punctuation, and a character PLY can't match is an
   error token. About 300 lines, written by hand.
2. The lines: Python's `str.splitlines()` and `rstrip()`, which split
   and strip at Unicode line breaks and spaces (`\f`, `\v`, `\x1c` to
   `\x1e`, U+0085, U+2028, and U+2029), trigraphs, and line
   continuations. About 80 lines.
3. Macros: definitions, `#undef`, the collection of arguments, and
   pcpp's expansion, which marks each token with an `expanded_from`
   list where most preprocessors use Prosser's hide sets, with its
   quirks: a token from an expansion takes the line of
   the invocation, a space is inserted after an identifier that follows
   a function-like expansion, `#` escapes every backslash and quote,
   `##` lexes the joined text again, and `, ## __VA_ARGS__` swallows the
   comma. About 550 lines.
4. Directives: the stack of conditionals, `#include` with its search
   (`-I` then include/ for `<...>`, the directories of the open files
   then the path for `"..."`, and none for a source read from stdin),
   `include_once` and the guard heuristic, pass-through of unknown
   directives, and pcpp's rewriting of paths relative to the current
   directory, which the warnings' `line N of <path>` shows. About 550
   lines.
5. `#if`: pcpp's grammar (a PLY yacc grammar of 64-bit values with an
   `unsigned` flag), with true division then `int()` for `/`, floored
   `%`, a left-associative `?:`, the error values that make `0 && 1/0`
   0, and `_NumberEvaluator`'s rewriting of each number. About 400
   lines.
6. The output, pcpp's `write`: blank lines as the difference of line
   numbers of the tokens, which drifts after an `#include`, the
   rightmost whitespace token of a run kept, a block comment as a
   space, a `//` comment as a newline, and the last line ending in a
   newline. About 120 lines.
7. preprocessor.py's hooks and arguments: `parse_args`, the three
   predefined macros and pcpp's `__DATE__`, `__TIME__`, and `__PCPP__`,
   `true` and `false` in `#if`, the `_Bool` warning (issued when pcpp
   flushes a line's tokens, so a `_Bool` before an error isn't warned
   about), the rejection of `#pragma c6502` and the blanking of the
   other pragmas, and pcpp's own errors, which print `<file>:<line>
   error: ...` to stderr and don't raise. About 300 lines.

The corpus uses little of this: object-like and function-like macros
(the latter in two games), `##` only in stdint.h's bodies, `#ifdef`,
`#ifndef`, `#if` with `defined`, `!`, and `&&`, and `#include` of
stdint.h, stdbool.h, apple2/monitor_io.h, stdbit.h, and 26 quoted
headers. All 289 pragmas are in disabled blocks, and no input has
`#elif`, `#undef`, `#error`, `#line`, a variadic macro, `#`, a line
continuation, or a carriage return. The Python tests add 2,835 sources,
function-like macros of stdbit.h, `#` and `##` in expansions, `#elif`,
`#undef`, `-D`, `-I`, `--passthru-unknown-exprs`, and the 21 calls that
raise `PreprocessorError` (14 for pragmas, 7 for numbers in `#if`).

The check is the capture of `preprocess` (the source text, the
arguments, and the file name in, the text and the warnings out, or the
error), with the files it includes read from their paths, which stay
in place while the captures are checked. A record could also hold the
current directory and each file pcpp opens with its bytes, which a
record made elsewhere would need. The corpus's 2,088 distinct calls and
the tests' 4,478 distinct inputs are the material, with a generated set
for what both leave out: macros with `#`, `##`, and `__VA_ARGS__`, `?:`,
`%`, `/` and large constants in `#if`, comments that span lines, form
feeds, carriage returns, and the drift of line numbers after an
include. pcpp's own tests and the mcpp suite it runs are in its
repository and aren't installed.

### What the port has to keep

The surveys found these places where the Python output depends on
something a straightforward C++ port could change.

- **Python's repr in messages.** The messages show the repr of strings
  (single quotes unless the string holds one and no double quote), of
  AST nodes and types with every field, of Lark's tokens, of a rule's
  alias (`got 'array_size_static'`), and of class names
  (`type(x).__name__`). `asdl::py_repr` gives the IR values', and
  `pyrepr::string` escapes the code points that a `str` of bytes holds
  as Python does up to U+00FF. Some messages are rewritten after the
  fact (`str(e).replace(repr(vd.name), repr(name))`, type_checking.py:4044).
- **The order of names.** Each resolution pass numbers its names in the
  order of its walk, with one counter per program, and the resolved
  names appear in the messages of programs that fail
  (`'@105.hswd'`), so the walk's order is part of the output even for
  an error.
- **Set order.** The parser's `_names_in` returns a set, and
  `_expression_type` (parser.py:2589) iterates it to choose which
  name's hidden tag to report when several qualify, so that message
  depends on the hash seed. Every other set of the front end is only
  tested for membership or sorted (`sorted(preserves)`).
- **Identity.** The parser compares scopes by identity
  (`remove_scope`, the `tag_scopes` of a binding), so a C++ scope must
  not reuse a dead scope's address. The other identity tests choose no
  behavior by object.
- **Ints wider than 64 bits.** Literals reach 2^64 - 1 and longer
  spellings are compared against the limits, attribute arguments and
  array sizes are unbounded, and constant_expression multiplies two
  `unsigned long long` operands to as much as 2^128 before reducing,
  which `asdl::Int` can't hold signed, so the C++ multiplies them in
  unsigned 128-bit arithmetic. One message prints the unreduced product
  (`85070591730234615847396907784232501249 does not fit in
  LongLong()`).
- **Floats.** `fp_arith` converts a decimal string to a `float` through
  a double, two roundings, as numpy does, which the C++ `fp_arith`
  already reproduces. pcpp's `/` in `#if` is Python's true division, so
  it loses precision above 2^53.
- **Encodings.** compile.py reads a source as UTF-8, and pcpp opens
  each header with the locale's encoding (cp1252 here, UTF-8 on Linux),
  with universal newlines. The shipped headers have non-ASCII only in
  comments.
- **An unnamed parameter** is None in `function_decl.params`, which the
  dump format has no form for, so the captures and `cpp_backend` send
  the empty string, and a C++ pass reads an empty name as Python's None.

### The captures

`tools/capture_passes.py --passes front` records each call of
`FRONT_PASSES` and of `FRONT_HELPERS`, the functions of pp_numbers.py,
where the lexer, the parser, and the preprocessor call them through the
module. A record of the front end can end in an `output-error` section,
the JSON object of the exception's class and message, for a call that
raised, which is then raised again, and can have a `warnings` line, the
`[where, message]` pairs the call issued. A change to the input in place
is on the `mutated` line under `input`, as for `c99_to_tac`. The source
text of `preprocess` and `parse` is the record's `input-json` line, and
`pp_number_at` is recorded with the text from its position to two
characters past the match, which is all its regular expression reads.
`c6502cpp check-captures` reads each record with the signatures of
`src/passes/front_stage.cpp` and compares a ported call's
`CompileError` with the recorded error.

On 21 Nov 2026 the capture of the whole corpus took 107 seconds with 12
processes and wrote 149 MB: 2,088 distinct calls of `preprocess`, 1,788
of `parse`, 1,562 of `unroll_program`, 1,566 of `identifier_resolution`,
1,487 each of `string_lifting` and `label_resolution`, 1,477 of
`loop_labeling`, 1,467 of `type_checking`, 5,137 of `pp_number_at`, 2,117
of `pp_parse_integer`, and one of `pp_is_floating`. No call failed to
record. The corpus never calls `pp_check`, `pp_check_floating`, or
`pp_bit_precise_width`, since the lexer checks only a number with a
separator, a binary prefix, or a `wb` suffix.

## Step 7: the assembler and the linker

Step 7 is everything compile.py, nm.py, and objdump.py do with objects
and executables, which is the assembly of `-c`, the link, and the two
listing tools, about 7,000 lines of Python in binutils/, passes/linker.py,
link_layout.py, link_script.py, runtime_library.py, the parser of the
link metadata in zp_link_metadata.py, and the helpers' 6502 code in
rtlib/helpers.py. This section lists the stages in order with what
each reads and returns, the errors each can give and how many of them
the corpus and the tests reach, where the time of a link goes, the
groups in which several agents can port the step at once, the places
where the Python output depends on something a straightforward port
could change, and the plan for the switch. The survey was made on 22
Nov 2026 from compile.py at 0551fff.

### The stages

compile.py's `main` runs these. "Raise sites" counts the `raise`
statements of each module's own error class.

| Stage | Python | Lines | Reads | Returns | Raise sites |
|---|---|---|---|---|---|
| a C input of `-c` or a link | `compile._object`, `_compile_c` | | the source, the preprocessor's arguments | the `-S` text with the link metadata block (C++ under the switch) | |
| assembly | `binutils.assembler.assemble` | 841 | the text, the file name, the linker's addresses of the names it places (`link_symbols`), `keep_source` | an `ElfFile` of `ET_REL` | 35 `AssemblerError` |
| the metadata block | `zp_link_metadata.parse_metadata` | 165 | the block's text | a `LinkMetadata` | 13 `ValueError` |
| reading and writing files | `binutils.elf.read`, `write` | 480 | bytes, a name for messages | an `ElfFile`, bytes | 8 `ElfError` |
| a linker script | `linker.read_script`, `link_script.parse_script` | 455 | the script's text, read in the locale's encoding | a `Script` | 14 `ScriptError` |
| the link | `linker.link_objects` | 1,333 | the objects with their names, the script, the origin, the `--defsym`s | an `ElfFile` of `ET_EXEC` | 34 `LinkError` |
| the runtime library | `runtime_library.needed`, `library_object` | 130, and 1,011 in rtlib/helpers.py | the objects' undefined names, the helpers' `asm_ast` functions, `asm_to_asm2`, `asm_emit`, the assembler | `runtime.o` and its metadata | |
| the shares across units | `slot_sharing.merge_shares`, `check_linked_shares` | 110 | every unit's `SlotShare`s, the call edges | the program's shares | 4 `ValueError` |
| the layout | `link_layout.lay_out`, `default_script`, `fall_through`, `tail_jump`, `startup_lines` | 488 | the script, each unit's items and sections, a lookup of the names the linker placed | each output section's parts and addresses, the section symbols | 16 `LayoutError` |
| resolving and relocating | `binutils.ld` (`global_definitions`, `Resolver`, `relax`, `relocate`, `executable`) | 343 | the objects, the placed sections, the linker's names | the executable | 10 `LinkError` |
| nm | `nm.main`, `symbol_lines` | 173 | argparse's command line, the files | the listing, the messages, the exit status | |
| objdump | `objdump.main`, `binutils.disasm.disassemble` | 226, 412 | the same | the same | |

`link_objects` itself runs these steps in order, and `elf.write` then
makes the file.

1. For each object, `parse_metadata`, `_object_items`, and
   `_without_init`, which leaves out the unit's `__zp_init`.
2. `_runtime_library`, and `global_definitions` for the absolute names.
3. `_rename_locals` and `_rename_meta`.
4. `_merge`, and `_validate_eligibility`, which runs
   `recursion_saves.hw_stack_cycles` on `_synthesize`'s program of call
   edges.
5. `_drop_unused_locals`, `_merge_zp_objects`, `startup_lines` under a
   script, `_choose_layout` (`runtime_layout.choose_layout`), and
   `_place_zp_objects`.
6. `_link_shares`.
7. `_synthesize`, `allocate_zp_slots`, `allocate_function_locals`, and
   `build_local_slot_symbols`, which are ported (step 4), and
   `_bind_extern_params`.
8. `_reassemble`, which assembles a unit again from its `.c6502.asm`
   when a name it reached in zero page lands above $FF.
9. `relax` on every section, and `_merge_identical`.
10. The assembly of the linker's own code (`_init_lines`, `__spill`, and
    `_data_lines` for a promoted object put back in the data segment).
11. `lay_out`, the `Resolver`, and `_output_symbols`.
12. `format_metadata`, which is ported (step 3), and `executable`, which
    relocates each placed section.

The captures reach these errors, which tools/binutils_errors.py groups
by the shape of their message.

- Assembly: none in the corpus, since the compiler's text always
  assembles.
- `elf_read`: the four messages of `elf.read`, from the damaged files of
  the corpus's `links` job.
- Links: 756 of the corpus's 4,537 distinct links fail, with 12 shapes
  of message from 7 raise sites: an undefined name (553, 280 of them a
  runtime helper with no 6502 code), an `IndirectCall` (72), a cycle
  (42), an `extern` zero-page object no unit defines (40), a section
  past the end of its region (32), a call of a function no unit
  defines with zp_abi (12), and an item no output section takes (5).
  21 of the suite's 104 distinct links fail, each with a message of
  its own, and 12 of those come from raise sites the corpus doesn't
  reach, so the two reach 19 of the 60 raise sites of linker.py,
  link_layout.py, and ld.py.
- Scripts and the assembler: the suite's 24 distinct scripts reach 7 of
  the 14 raise sites of `parse_script`, and its 269 distinct assemblies
  8 of the assembler's 35.
- nm and objdump: 16 and 13 shapes, each of argparse's errors the
  corpus's bad command lines give, a missing file, and each of
  `elf.read`'s messages.

No call of the corpus or the suite raises `ZpSlotAllocationError` or
`ZpLocalAllocationError` in a link, or reaches `_reassemble`'s,
`relocate`'s, or `parse_metadata`'s errors. Those, and the other raise
sites neither reaches, need cases of their own, as the earlier steps
wrote for the messages the corpus never gives.

### Where the time goes

The corpus capture timed every call: an assembly takes 8.1 ms on
average (5,491 calls in 44 s), a link 6.3 ms (4,547 in 28.6 s), a
disassembly 7.4 ms, nm 3.2 ms, and objdump 10.0 ms. The three games
link in 59 ms (Zork), 88 ms (Lode Runner), and 175 ms (Ali Baba), best
of five without a profiler, of which reading the objects takes 7 to 19
ms and writing the executable 1 to 3. In the link of Ali Baba,
`allocate_function_locals` takes 40 ms, `_merge` 21, `parse_metadata`
17, `relax` 11, `executable` 7, `global_definitions` and
`allocate_zp_slots` 7 each, `_link_shares` 6, and the layout 4, and
Zork and Lode Runner spend most of theirs in the same functions. As a process,
`compile.py -T script objects -o exe` takes 0.51 to 0.59 seconds with
`C6502_CPP_BACKEND=1` and 0.85 to 1.02 without, and with the variable
set about 0.26 seconds of it is importing passes.linker, which imports
the type checker through `passes.zp_globals` and numpy through
`fp_arith`. The suite's 106 calls of `link_objects` took 1.7 seconds
in all, and its 470 assemblies 2.9 seconds. From the speedups of the
earlier steps, a link in C++ is likely to take a few milliseconds (an
estimate, since no part of the link is ported yet), so the process's
start and the compiles of its C inputs would be most of a link's time.

### The groups

Each group ports its modules with the conventions of cpp/README.md
("Porting a pass"), into src/binutils/ for binutils/ and nm.py and
objdump.py, and src/passes/ for the linker's modules, and checks them
on a capture of the corpus and of the suite (`c6502cpp check-binutils`,
"The captures" below), errors included.

- **S. The shared pieces**, done with this survey: the ELF reader and
  writer (src/binutils/elf.h), the opcode table (opcodes.h), Python's
  argparse as nm.py and objdump.py use it (src/argparse.h, with
  `path_str` for the paths Python's messages show), the JSON writer the
  checks use, and nm (src/binutils/nm.h), which `C6502_CPP_BACKEND=1`
  runs for nm.py.
- **A. The assembler and the metadata block**: binutils/assembler.py
  (841 lines) and `parse_metadata` with its helpers
  (zp_link_metadata.py:460-625). The linker reads the metadata too, so
  A ports `parse_metadata` first and says so. It needs Python's string
  methods (`splitlines`, `strip`, `split`, `upper`, `isdigit`) and the
  regular expressions of `_TOKEN`, `_EQU`, and `_LABEL` with Python's
  Unicode classes, which src/pcpp_text.h and src/py_regex.h have, and
  unbounded integers in its expressions. It is checked on the
  `assemble` records, each object compared byte for byte, and on the
  calls the linker makes of it.
- **B. objdump and the disassembler**: objdump.py (226) and
  binutils/disasm.py (412), on S's pieces, checked on the `objdump`
  and `disassemble` records, and switched on in objdump.py as nm is.
- **C. Linker scripts and the layout**: passes/link_script.py (455) and
  link_layout.py (488), with `asm_emit`'s `instr_line`, which C makes
  public for `startup_lines` and the linker's own code. `parse_script`
  is checked on its records, whose result is the tagged JSON of the
  `Script`. The layout's inputs are the linker's `Unit`s, so the
  capture records each call of its five functions with what it reads,
  and they are checked on those records.
- **D. The linker**, on A and C, in two parts that share
  `link_objects`'s state (the objects, the metadata, and the names the
  linker placed), which D1 declares first.
  - D1: the metadata's side: `_merge`, `_validate_eligibility`,
    `_synthesize`, `_rename_locals` and `_rename_meta`, `_merge_zp_objects`,
    `_place_zp_objects`, `_choose_layout`, `_runtime_bytes`,
    `_link_shares` with `merge_shares` and `check_linked_shares`,
    `_bind_extern_params`, `_drop_unused_locals`, `_init_lines`, and
    `_data_lines` (linker.py:161-384 and 736-1333), about 900 lines.
    The allocators, `hw_stack_cycles`, `choose_layout`, and
    `format_metadata` are ported, so D1 reuses them.
  - D2: the objects' side and the driver: binutils/ld.py (343),
    `link_objects`, `_object_items`, `_without_init`, `_unit`,
    `_pointer_names`, `_reassemble`, `_output_symbols`, `_merge_identical`,
    and `_contents` (linker.py:385-735), about 750 lines. It is checked
    on the `link` records, each executable compared byte for byte.
- **E. The runtime library**: passes/runtime_library.py (130) and
  `runtime_layout.helper_footprints`. The helpers' code stays in
  rtlib/helpers.py with the simulator until step 9, so E generates a
  table of the 19 helpers' `asm_ast` functions in the dump format,
  about 20 KB, checked in under cpp/generated/ with a test that writes
  it again and compares it, as tests/test_lalr_table.py does for the
  parser's table. The C++ side then computes the footprints, and the
  backend's request stops carrying them. E needs A's assembler for
  `library_object`, and can write the table and the footprints first.
- **F. The driver and the switch**: compile.py's `-c` and link paths in
  C++, as the plan for the switch below describes, once A to E are
  done.

A, B, C, and E's table can start at once, D once A's `parse_metadata`
and C's `lay_out` have an agreed interface, and F last.

### What the port has to keep

The survey found these places where the Python output depends on
something a straightforward C++ port could change.

- **The ELF files' order.** `elf.write` writes the local symbols first
  and then the globals, each in the list's order, keeps each string
  once in its table in the order of first use (with no sharing of
  suffixes), puts the `.rela` sections after the content sections in
  their order, writes the segments' bytes first and points a section
  at a segment's bytes when its address and bytes are the same, and
  pads to 4 bytes before the section headers. `read` keeps the file's
  order. Every object and executable of the captures makes the trip
  through the C++ code (`c6502cpp check-binutils`).
- **Identity.** The linker renames and redefines symbols in place, so
  that every relocation naming one follows it (`_rename_locals`,
  `_merge_identical`), and keys the placed sections by `id()`. The C++
  `ElfFile` holds sections and symbols by shared pointers for this.
- **Python's text.** The assembler splits lines with `str.splitlines`,
  which also splits at `\x0b`, `\x0c`, `\x1c` to `\x1e`, U+0085,
  U+2028, and U+2029, strips and splits at Unicode whitespace, upper-
  cases a mnemonic with `str.upper`, and its regular expressions' `\s`
  and `\d` are Unicode's, so a number may be written in any script's
  digits. It keeps the text as `"\n".join(lines) + "\n"`, which turns
  CR LF into LF. A `DC.B` string is encoded as Latin-1.
- **Unbounded integers.** An `EQU` or an operand computes with Python's
  integers, with floor division, an arithmetic `>>` of a negative
  value, and a `<<` with no limit, and the messages print the value
  (`$-1F isn't in zero page`, Python's `X` format of a negative
  number).
- **Insertion order and sorting.** Every dict the linker walks follows
  insertion order: the zero-page objects in the order the units list
  them, the renames, the placed names, and the absolute symbols.
  `_merge` sorts the definitions, externs, and call edges by name,
  which is code-point order, the order of their UTF-8 bytes.
  `_place_zp_objects` sorts the promoted objects by the float key
  `-weight / size`, then the name. `_output_symbols` sorts each table of
  the linker's names by value, stably. No set's order reaches the
  output: each set is sorted or tested for membership.
- **Messages.** Several messages print a sorted list with Python's
  repr (`function(s) defined in multiple TUs: ['f', 'g']`), one holds
  the character U+2192 (`main → ['f']`), and the OSError of a file
  that can't be read names Python's `str(Path(path))`, with
  backslashes on Windows. `E.read_file` passes `elf.read` the path as
  the command line gives it, so an ElfError keeps a `./` and a `/`.
- **Paths and encodings.** `read_script` reads the script in the
  locale's encoding (cp1252 here, UTF-8 on Linux) with universal
  newlines. The layout matches a script's file pattern with
  `fnmatch.fnmatchcase` against the input's name after its last `/` or
  `\`. An executable's `STT_FILE` symbols are `Path(path).name`.
- **argparse.** nm.py and objdump.py wrap the usage and help to the
  terminal's width (`shutil.get_terminal_size`), which the C++ argparse
  reads the same way and `cpp_backend.run_tool` passes on as `COLUMNS`.
  The help goes out through Python's text stream, which on Windows ends
  its lines with CR LF, and the listings through `write_lf` with LF.
- **objdump's formatting.** `{sec.name:<20}` pads by code points, the
  section contents are lowercase hex in groups of four bytes, and the
  alignment is `2**(align.bit_length() - 1)`.

### The plan for the switch

nm.py shows the pattern. Under `C6502_CPP_BACKEND=1`, its `main` runs
`c6502cpp nm` with the command line (`cpp_backend.run_tool`), writes
what the C++ side writes to `sys.stdout` and `sys.stderr`, and returns
its exit status, raising SystemExit(2) for argparse's. The C++ side
writes the listing's bytes and the help in text mode, as Python does,
and its stderr with LF, which Python's stream turns into CR LF on
Windows. A test that calls `nm.main` with its streams redirected sees
the C++ output, so the suite with the variable set runs the C++ nm.
objdump.py takes over the same way once group B lands.

For compile.py, group F extends the backend's request rather than
adding a process per step.

- `-c` of a C input: the request says `assemble`, and the C++ side
  assembles the `-S` text and returns the object's bytes as a section of
  the response, which compile.py writes to `<stem>.o`. An `.asm` input
  goes to `c6502cpp assemble`, with the text and the file name.
- A link: a `c6502cpp link` request lists the inputs in order, each a
  C source (with its text and the preprocessor's arguments, compiled in
  the same process), an assembly file, or an object's path, with the
  script's path, the origin, and the `--defsym`s, and the C++ side
  writes the executable. The response holds, for each C input, what
  the preprocessor wrote to stderr and its warnings, and the first
  error with its Python class, its message, and which input or the link
  it came from, so compile.py prints `<file>: error: <message>` or
  `compile.py: link error: <message>` with exit status 1 as it does
  now. A bug the C++ side reports exits with status 3 and is raised as
  an AssertionError, as for the backend.
- The errors keep their Python classes: `AssemblerError`, `ElfError`,
  `LinkError`, `ZpSlotAllocationError`, and `ZpLocalAllocationError`
  are added to `cpp_backend._ERROR_MODULES`, and an unreadable input
  gives compile.py's message for an OSError (`missing.o: error: No such
  file or directory`).

These stay in Python until step 8.

- `--lex`, `--parse`, `--resolve`, and `--tac`.
- compile.py's own reading of its command line, which a port of the
  rest of argparse (`parse_known_args`, the mutually exclusive stage
  flags, `store_const`, and `type=`) moves to C++ in step 8.
- `ir_check` under `C6502_CHECK_IR=1`.
- tests/link_helpers.py's `linked_text`, which calls the disassembler
  directly.
- The helpers' code in rtlib/helpers.py, which group E's table copies.

The simulator (sim/linked.py, sim/lode_runner.py, and sim/ali_baba.py) reads
executables with binutils.elf and decodes with `opcodes.DECODE`, and
stays in Python until step 9. sim/assembler.py, the simulator's own
assembler of `asm_ast` programs, has opcode tables of its own for the
subset it assembles and shares no code with binutils/.

### The captures

tools/capture_binutils.py wraps `assemble`, `elf.read`, `parse_script`,
`link_objects`, `disassemble`, and the `main` functions of nm.py and
objdump.py, and records each call with its arguments and its result or
error, as tools/capture_passes.py's records are written. Each file a
call reads or writes is stored once under `blobs/`, named by its
SHA-1, which keeps a capture of the whole corpus to 80 MB of files. The
corpus is capture_passes.py's, compiled with `-c` and `-c -O`, each
object listed with eight sets of nm options and seven of objdump's,
linked alone without a script and with a script of ROM and RAM regions,
`AT>`, and `NOLOAD`, and each executable listed again. Its `links` job
links the three games with their scripts as their tests do and without
them, and reads, lists, and links damaged files with bad command
lines. As a pytest plugin it records the suite's calls. For each link
the harness links again from the objects' bytes read back, which gave
the same executable for every link of the corpus and the suite, so the
C++ linker can take its objects as bytes.

On 22 Nov 2026 the capture of the corpus took 131 seconds with 12
processes, with the compiles in C++ (`C6502_CPP_BACKEND=1`), and wrote
3,003 distinct records of `assemble`, 5,603 of `elf_read`, 4 of
`parse_script`, 4,537 of `link`, 22,225 of `disassemble`, 44,785 of
`nm`, and 39,175 of `objdump`, with 6,292 files. The suite gave
269 distinct records of `assemble`, 380 of `elf_read`, 24 of
`parse_script`, 104 of `link`, 33 of `disassemble`, 5 of `nm`, and 5 of
`objdump`, with 336 files, in a run of 408 seconds with 12 processes. `c6502cpp check-binutils` read every record of both and
wrote its JSON again with no difference, read and wrote again all
6,283 and 330 objects and executables with no difference, matched every
`elf_read` record, errors included, and matched all 44,785 `nm` records
of the corpus and the suite's 5, errors and argparse's messages
included, in the builds of MSVC 19.36 and GCC 12.2.

## Step 8: retiring the Python compiler

Step 8 removes the Python compiler, which is everything compile.py,
nm.py, and objdump.py run, and leaves the C++ port with checks of its
own. Until now every check of the port compared C++ with Python,
through the captures, tools/compare_backends.py, and the suite with the
switch on and off. None of those can run once the Python compiler is gone. Step
9 then moves the simulator, the suite, `tac_sim`, and the fuzzers to
C++, so until it lands those stay in Python and drive the C++
executables. This section lists what still runs in Python and who
imports it, classifies the suite's tests, proposes the checks that
replace the comparisons with Python, proposes the C++ command line,
and splits the step into groups. The survey was made on 23 Nov 2026
from compile.py at 0294536, and it ported `--lex` to C++ as the first
piece (the Status entry of the survey).

### What still runs in Python

The compiler's Python is 226 files of 102,029 lines, counted with
their `__init__.py` files.

| Directory | Files | Lines |
|---|---|---|
| the root's modules | 18 | 22,238 |
| passes/ | 115 | 49,366 |
| passes/optimization/ | 56 | 18,358 |
| passes/optimization/framework/ | 10 | 1,393 |
| passes/optimization_asm/ | 17 | 6,672 |
| passes/optimization_ast/ | 2 | 788 |
| binutils/ | 6 | 2,198 |
| rtlib/ | 2 | 1,016 |

The command-line programs add compile.py (1,708 lines), cpp_backend.py
(475), nm.py (173), and objdump.py (226). tools/ holds 5,885 lines,
most of them the capture harnesses and their checkers, and asdl.py
(947) generates both compilers' IR classes. The root's modules are the
four IRs' generated classes, `asm_emit`, `bit_precise`,
`builtin_functions`, `c99_to_tac`, `diagnostics`, `fp_arith`,
`ir_dump`, `lark_cache`, `lexer`, `parser`, `pp_numbers`,
`preprocessor`, `pretty`, and `tac_to_asm`.

With `C6502_CPP_BACKEND=1`, these parts of a run of compile.py still
run Python code of the compiler.

- compile.py's argparse, the reading of the inputs, and the replay of
  the response.
- `--parse`, `--resolve`, and `--tac`, which run the whole Python front
  end and print the tree with `pretty`. `--tac -O` runs
  `inline_functions` and `optimize_tac` only, without the row-address
  tables, `return_pointers_in_zero_page`, the dispatch of const-pointer
  arrays, or the split tables that `-S -O` runs.
- `ir_check` under `C6502_CHECK_IR=1`, which checks the TAC, the
  finished asm program, and the asm2 program that the response carries.
  It is 499 lines, but it imports 47 modules of 26,404 lines, because
  it reads the asm optimizer's CFG, liveness, and register allocation
  helpers and the type checker's tables.
- The exception classes. `cpp_backend._exception` imports the class
  of a rejected program's error from its Python module, so a
  `TypeCheckError` loads passes/type_checking.py and its imports
  (10,857 lines), and a syntax error loads Lark.

The table lists the modules that something other than compile.py and
the tests imports, directly or inside a function. "compile.py" is
"yes" when compile.py imports the module on any of its paths, and the
test column counts the test modules that import it.

| Module | Lines | compile.py | Test modules | Other importers |
|---|---|---|---|---|
| `asm2_ast` | 408 |  | 4 | cpp_backend |
| `asm_ast` | 447 | yes | 110 | cpp_backend; sim (assembler, harness, runtime); tools (capture_helpers, rtlib_table) |
| `binutils.assembler` | 847 | yes | 3 | tools (capture_binutils) |
| `binutils.disasm` | 412 |  | 2 | objdump; tools (capture_binutils) |
| `binutils.elf` | 487 | yes | 10 | nm; objdump; sim (ali_baba, linked, lode_runner); tools (capture_binutils) |
| `binutils.ld` | 343 |  | 1 | tools (capture_binutils) |
| `binutils.opcodes` | 106 |  | 1 | sim (ali_baba, lode_runner) |
| `bit_precise` | 187 |  | 1 | tac_sim |
| `builtin_functions` | 225 |  | 4 | tac_sim; sim (runtime) |
| `c99_ast` | 791 |  | 71 | tac_sim; fuzz (reduce); tools (capture_helpers, capture_passes) |
| `diagnostics` | 68 | yes | 2 | cpp_backend; tools (capture_passes) |
| `fp_arith` | 248 |  | 6 | tac_sim |
| `ir_dump` | 442 |  | 6 | cpp_backend; tools (capture_helpers, capture_passes, rtlib_table) |
| `lexer` | 283 | yes | 20 | tools (capture_parser_tokens, lalr_table) |
| `parser` | 4,888 | yes | 64 | tools (capture_parser_tokens, lalr_table) |
| `passes.abi_selection` | 1,165 | yes | 13 | sim (harness) |
| `passes.asm_aliasing` | 615 |  | 4 | tools (capture_helpers, capture_passes) |
| `passes.asm_dead_store` | 773 | yes | 1 | tools (capture_helpers) |
| `passes.asm_liveness` | 829 |  | 4 | tools (capture_helpers) |
| `passes.call_edges` | 68 |  | 1 | tools (capture_helpers) |
| `passes.dataclass_fields` | 45 |  |  | tools (capture_helpers, capture_passes) |
| `passes.dead_a_arith` | 374 | yes | 1 | tools (capture_helpers) |
| `passes.decimal_arith` | 164 |  |  | tools (capture_helpers) |
| `passes.inc_peephole` | 395 | yes | 3 | tools (capture_helpers) |
| `passes.ir_check` | 499 | yes | 5 | cpp_backend; fuzz (runner) |
| `passes.link_layout` | 493 |  | 1 | tools (capture_binutils) |
| `passes.link_script` | 482 |  | 1 | tools (capture_binutils) |
| `passes.linker` | 1,364 | yes | 3 | tools (capture_binutils) |
| `passes.optimization` | 30 | yes | 5 | fuzz (tac_passes) |
| `passes.optimization.cfg` | 337 |  | 1 | tools (capture_helpers) |
| `passes.optimization.framework.defuse` | 189 |  |  | tools (capture_helpers) |
| `passes.optimization.framework.driver` | 116 |  |  | fuzz (tac_passes); tools (capture_passes) |
| `passes.optimization.optimizer` | 333 |  | 1 | tools (capture_helpers, capture_passes) |
| `passes.optimization.ssa_construction` | 652 |  | 2 | tools (capture_passes) |
| `passes.optimization.ssa_destruction` | 330 |  | 1 | tools (capture_passes) |
| `passes.optimization.value_range` | 244 |  | 1 | tools (capture_helpers) |
| `passes.optimization.var_visit` | 435 |  |  | tools (capture_helpers) |
| `passes.optimization_asm.cfg` | 259 |  | 2 | tools (capture_helpers) |
| `passes.recursion_saves` | 369 | yes | 3 | tools (capture_helpers) |
| `passes.redundant_load` | 2,093 | yes | 3 | tools (capture_helpers) |
| `passes.replace_pseudoregisters` | 899 | yes | 4 | sim (harness); tools (capture_helpers) |
| `passes.runtime_layout` | 239 | yes | 5 | sim (ali_baba, harness, linked, runtime); tools (capture_helpers, capture_passes) |
| `passes.runtime_library` | 135 |  | 3 | tools (capture_binutils) |
| `passes.sections` | 94 | yes |  | tools (capture_passes) |
| `passes.slot_sharing` | 965 | yes | 2 | tools (capture_binutils, capture_helpers) |
| `passes.type_checking` | 6,205 | yes | 78 | tac_sim; sim (harness); tools (capture_helpers, capture_passes) |
| `passes.zp_globals` | 566 | yes | 3 | sim (harness) |
| `passes.zp_link_metadata` | 625 | yes | 4 | tools (capture_binutils) |
| `passes.zp_slot_naming` | 286 | yes |  | tools (capture_helpers) |
| `pp_numbers` | 324 |  | 1 | tools (capture_passes) |
| `preprocessor` | 663 | yes | 119 | sim (harness); tools (pcpp_evaluator_tables) |
| `rtlib.helpers` | 1,011 |  | 2 | sim (runtime); tools (rtlib_table) |
| `tac_ast` | 632 | yes | 59 | cpp_backend; tac_sim; fuzz (tac_passes); tools (capture_helpers) |
| `tac_to_asm` | 5,601 | yes | 7 | sim (harness) |

The other modules are imported only by compile.py and the tests. Each
is listed with its line count, the number of test modules that import
it in parentheses, and a star when compile.py reaches it only through
another module of the compiler.

- (root): asm_emit 1,501 (2), c99_to_tac 5,351 (14), lark_cache 139 (1)*, pretty 40 (1).
- passes: adc_commute 628 (1), address_taken_zp 251 (1), and_sign_bit_branch 319 (1), asm_licm 499 (1), asm_remat 912 (1), asm_to_asm2 594 (3), branch_invert 108 (1), branch_thread 135 (1), branch_through_jump 255 (1), branch_to_next_drop 185 (1), branch_value_thread 404 (1), carry_halve 163 (1), carry_in_add 173 (1), carry_inc 242 (1), carry_known 248 (1), carry_recompute 185 (1), carry_return 408 (1), carry_return_choice 239, cmp_sbc_fusion 220 (1), cmp_through_tmp_fold 462 (1), const_arith_fold 390 (1), constant_expression 663 (1), constant_store_order 287 (1), counter_to_index_reg 337 (1), cpx_cpy_peephole 186 (1), cross_jump 177 (1), dead_label_drop 91 (1), dead_pha_pla 212 (1), dec_branch_indirect_y_fold 124, dec_inc_branch_fold 163 (1), dec_peephole 422 (3), dec_register_peephole 457 (1), designators 401*, direct_index_load 162 (3), dual_index_promotion 556 (1), element_in_place 436 (1), function_local_sizing 237 (1), identifier_resolution 1,322 (34), in_place_member_update 216 (1)*, inc_low_byte_test 162 (1), index_inc_cmp_y 320 (1), join_store 244 (1), jump_table 243 (1), label_offset_fold 151 (1), label_resolution 344 (21), long_branches 169 (1), loop_constant_a 227 (1), loop_counter_to_x 955 (1), loop_invariant_a 456 (1), loop_labeling 422 (21), lower_data_load_address 167 (1), mem_const_prop 350 (1), memory_value_propagation 1,603 (1), outline_runs 244 (1), peephole_memo 186 (1), pointer_restage 184 (1)*, pointer_self_store 336 (2), prologue_synthesis 194 (1), promote_loop_carried_to_a 533, prune_unused_slots 119 (1), redundant_compare 509 (2), redundant_load_after_rmw 329 (2), redundant_store 353 (1), ret_slot_coalescing 418 (1), return_in_zero_page 123 (1), rotate_loop_test 372 (1), round_trip_load 244 (1), self_store_drop 135 (1), shared_tails 332 (1), shift_cost 56*, shift_in_a 480 (2), short_jumps 198 (1), significant_bytes 161*, slot_coalescing 629 (1), split_mem_to_mem 195 (2), stdbit_functions 123*, string_lifting 564 (10), sub1_test_zero_peephole 236 (1), tail_call 140 (1), tail_merge 267 (1), transfer_past_branch 156 (1), transfer_pm1_store 482 (1), type_check_error 15*, unreachable_drop 65 (1), via_a_store_fold 318 (1), volatile_void_read_cmp 114 (2), x_save_slot_load 374 (2), y_peephole 176 (1)*, zp_local_allocation 581 (3), zp_slot_allocation 401 (2).
- passes.optimization: and_zero_jump_fold 309 (1)*, cmp_zero_jump_fold 646 (1)*, compare_subtract 373*, constant_folding 1,203 (4)*, copy_folding 299 (1)*, copy_propagation 333 (2)*, count_down 231 (1)*, cse 182 (1)*, dead_loop_elimination 228 (1)*, dead_store_elimination 197 (2)*, direct_member_access 193*, dispatch_pointer_array 372 (1), distribute_constant 155*, divmod_hoist 326 (1)*, extend_add_chain 195*, framework 84 (17)*, inline_functions 637 (1), interference 46 (2)*, jump_thread 367 (1)*, known_compare_thread 563 (1)*, licm 119 (1)*, lnot_jump_fold 238 (1)*, loop_rotate 701 (1)*, narrow_byte_bitwise 191 (1)*, narrow_byte_range 283 (1)*, narrow_widened_arith 642 (2)*, nonnegative_compare 156*, noreturn_call 82 (1)*, phi_constant_fold 143*, phi_offset 264*, pool 60 (7), read_stored_global 167 (1)*, reassoc_const 260 (1)*, reassoc_offset 145 (1)*, recognize_indexed_load 107*, recognize_indexed_store 132 (1)*, recognize_indirect_indexed 606 (1)*, register_allocation 86 (4)*, row_address_tables 436 (1), short_circuit_jump_fold 552 (1)*, sink_and_past_branch 646 (1)*, sink_increment 405 (1)*, split_tables 538 (1), static_const_fold 149 (1)*, static_indexed_access 221*, step_on_edge 189 (1)*, strength_reduction 533 (2)*, three_way_compare 614 (1)*, truncate_extend_fold 231 (2)*, unreachable_code_elimination 246 (2)*.
- passes.optimization.framework: base 29*, operand_rewrite 66*, patterns 390*, phases 42*, rules 278*, sink 72*, window 127*.
- passes.optimization_asm: apply_coloring 518 (1)*, backward_copy_propagation 492 (1)*, byte_dce 242 (1)*, coalescing 597 (2)*, const_static_fold 440 (1)*, copy_propagation 249 (1)*, dead_static 106 (1)*, hwreg_eligibility 546 (2)*, interference 266 (2)*, liveness 275 (2)*, optimizer 729 (2), or_zero_absorb 78 (1)*, regalloc 618 (3), ssa_construction 782 (4)*, ssa_destruction 459 (1)*.
- passes.optimization_ast: unroll 774 (1).

The tools in the table are the capture harnesses
(capture_passes.py, capture_helpers.py, capture_binutils.py, and
capture_parser_tokens.py) and the generators of the C++ tables
(lalr_table.py, pcpp_evaluator_tables.py, and rtlib_table.py), which go
with the Python compiler, except rtlib_table.py, whose input stays
until step 9. sim/harness.py imports `abi_selection`,
`replace_pseudoregisters`, `zp_globals`, `type_checking`,
`preprocessor`, `tac_to_asm`, and compile.py only on its path without
the variable, and it hands the front end's passes to fuzz/reduce.py and
fuzz/tac_passes.py by name (`_PIPELINE`).

#### What step 9's Python keeps

The simulator, the suite, `tac_sim`, and the fuzzers read these modules
as data models and readers, so step 8 can't remove them. Together they
are about 5,500 lines.

- `asm_ast` and `ir_dump`. sim/assembler.py assembles an `asm_ast`
  program, and the harness already gets the finished program from
  `c6502cpp backend` as a dump. `asm2_ast` stays only while `ir_check`
  is in Python.
- `rtlib/helpers.py`, the helpers' `asm_ast` functions, which
  sim/runtime.py assembles at $E000, and passes/runtime_layout.py
  (`CLASSIC`, `RuntimeLayout`), which sim/runtime.py, linked.py, and
  ali_baba.py read, with what it imports, passes/dataclass_fields.py and
  passes/type_check_error.py. tools/rtlib_table.py keeps the C++ copy
  of the helpers equal to these.
- `binutils/elf.py`, which sim/linked.py, lode_runner.py, and
  ali_baba.py read executables with, and `binutils/opcodes.py`, whose
  `DECODE` the last two decode with.
- `builtin_functions`, whose `bin_to_bcd` sim/runtime.py calls and
  whose BCD model `tac_sim` and seven tests use, and `bit_precise`,
  `fp_arith` (the one user of numpy), `c99_ast`, and `tac_ast`, which
  `tac_sim` reads.
- From passes/type_checking.py, the classes of the tables: `Symbol`,
  `SymbolTable`, `LocalAttr`, `StaticAttr`, `FunAttr`, the initial
  values, `TypeTable`, `StructLayout`, and `MemberInfo`, which are its
  first 500 lines of 6,205. `tac_sim` reads each symbol's type and
  attributes and each structure's layout.
- `diagnostics`, through which cpp_backend.py issues the warnings, and
  the exception classes of `cpp_backend._ERROR_MODULES`, with Lark's
  hierarchy (`UnexpectedToken` and `UnexpectedCharacters` under
  `UnexpectedInput` under `LarkError`). About 65 test modules import
  one of those classes, and 24 import `lark.exceptions`.
- asdl.py, which writes the Python IR modules above and the C++ ones,
  and cpp_backend.py, compile.py, nm.py, and objdump.py as shims.

Step 8 has to give each of these what it now takes from the Python
compiler.

- The TAC interpreter (`tac_sim.Simulator`) takes a TAC program, its
  symbol table, and its type table, and today gets them from the Python
  front end and `c99_to_tac`, in `tests.test_sim_chapters._run_program`,
  which fuzz/runner.py's `tac` path and 125 tests run, and in
  fuzz/reduce.py and fuzz/tac_passes.py through the harness's
  `_PIPELINE`. The C++ side has to return the TAC's dump and the tables
  in the tagged JSON that src/tagged_json.h writes for the captures,
  and Python has to read that JSON into the classes above with the
  inverse of `tools/capture_passes.encode`, which isn't written yet.
- fuzz/tac_passes.py runs `tac_sim` on each function after each TAC
  pass, through `PhaseDriver.observer`, and fuzz/reduce.py's
  `--tac-opt` on the result of `optimize_tac`. For them the request
  takes a mode that writes each function's dump after each pass with the
  pass's name, and one that stops after `optimize_tac`.
- fuzz/reduce.py's `falls_off_end` appends a call to the body of each
  function in the parsed c99 program before the names are resolved.
  That takes the c99 program's dump and a request that accepts a c99
  program in place of the source, or else an edit of the source text
  that appends the same call.
- tests/link_helpers.py's `linked_text` calls `binutils.disasm`, which
  `c6502cpp objdump` can replace.
- cpp_backend.py finds c6502cpp with `tools.check_captures.cpp_cli`,
  which would move into cpp_backend.py when the capture tools go.

### The tests

The suite had 4,959 tests in 279 modules on 23 Nov 2026, as pytest
collects them. A script read each test module's imports and the names
that each test class uses, directly or through the module-level
functions and values it calls, including the helpers it imports from
other test modules, and sorted each name into a kind. The counts below
come from it and from a reading of the classes it couldn't sort.

| Kind | Tests | Classes | Modules |
|---|---|---|---|
| (a) end to end | 1,002 | 288 | 156 |
| (b) a Python pass or helper called directly | 3,804 | 743 | 220 |
| (c) the Python implementation itself | 118 | 31 | 11 |
| models step 9 keeps | 35 | 12 | 7 |

A module can hold classes of several kinds, and 116 of them do. A
class is (b) whenever it calls a pass directly, which 437 of the (b)
tests do alongside an end-to-end check.

**(a) End to end.** These run `compile.main`, compile.py in a
subprocess, `_run_stage("codegen")`, the simulator harness,
tests/link_helpers.py, or nm.py's and objdump.py's `main`, and they
keep running against C++ through the shims. Three edits reach them.
342 of them call Python's `preprocess` and give its output to
`_run_stage`, and they would call a helper that sends the source
text with the preprocessor on, as `cpp_backend.compile_source` does. 156
test modules skip their classes unless the pcpp command is on the
PATH, which they should test for c6502cpp instead. And 67 tests in 17
classes turn inlining off with tests/no_inlining.py, which patches
`compile.inline_functions`. `compile._inlining` already sends that
choice to C++, so the shim only has to keep a flag for it.

**(b) Direct calls.**

| What the tests call | Tests |
|---|---|
| asm passes and code generation, on asm or TAC they build | 1,402 |
| TAC passes and `c99_to_tac`, on TAC they build | 316 |
| the assembler, the linker, and their helpers | 90 |
| the front end on a source, inspecting the c99 program or the tables, 16 of them the preprocessor alone | 1,265 |
| the front end and then `c99_to_tac`, the TAC optimizer, or `tac_sim` | 466 |
| compile.py's own functions (`_resolved`, `translate_for_codegen`, `optimized_asm`, `_peephole_fixedpoint`) | 232 |
| the TAC interpreter through `tests.test_sim_chapters._run_program`, beside the simulator | 33 |

The C++ tests already cover part of them. 3,326 doctest cases existed
before this survey,
many of them the recorded calls of a Python test module (cpp/tests'
`call_case.h` and `stage_case.h`, as tests/test_inline_functions.py's
calls are in cpp/tests/test_inline_functions.cpp). Counted by module,
91 modules with 1,395 (b) tests have a C++ test file of their name, or
C++ cases that name them, with at least as many cases as the module
has (b) tests, 60 modules with 1,727 tests have fewer, and 69 modules
with 682 tests have none. A count of cases doesn't show that every
assertion is ported, so each group checks its modules one test at a
time.

Each test of (b) goes one of three ways.

- **Ported as recorded calls.** The capture wrapper records every call
  a test makes of a ported function, with its arguments and Python's
  result or error, and the distinct calls are checked in as doctest
  cases that run the C++ function, as for test_inline_functions.py.
  This is mechanical and keeps every input the test gives. An assertion
  of a property, such as "no `LDA` follows", becomes the equality of
  the whole result with Python's, which is stricter and has to be
  updated whenever the output changes on purpose. It suits the 1,808
  tests on hand-built IR and the object-file tools.
- **Ported by hand to doctest.** A test whose point is a property, or
  whose recorded calls would be very large (the front end's tests hand
  whole symbol tables to each call, as the cases of
  test_inline_functions.cpp show), is written again on the C++
  function. It suits the 1,963 front-end and compile.py tests where the
  recorded calls don't.
- **Replaced by a corpus check.** A test that only compiles a program
  and checks that it's accepted or rejected at a stage, as the
  chapters' `invalid_lex` and `invalid_parse` buckets do with
  `tokenize` and `parse`, becomes an end-to-end test of
  `compile.py --lex` or `--parse` on the same file, or a golden file
  of the corpus check below. The 16 tests of the preprocessor alone
  become tests of the `-S` output or of the preprocessor's records
  (cpp/tests/preprocessor_case.h).

No test can be dropped because a capture covers it, since the captures
aren't kept. The captures of the suite recorded every call these tests
make, though, so the recorded calls lose nothing that the comparisons
with Python checked.

These tests can't be ported as they're written.

- tests/no_inlining.py (above), which patches a function of compile.py.
- test_row_address_tables.py patches `_saving`, the estimate of the
  bytes a table saves, to force or forbid the table. The C++ pass would
  take the estimate as a parameter that its doctest cases set.
- test_byte_arith_sim.py's `test_zero_count` patches
  `tac_to_asm._shift_loop` to leave out a compare, and checks that the
  asm optimizer keeps the shift loop's test of a zero count. It would
  become a doctest of the asm optimizer on the program the patched
  lowering gives, recorded once.
- test_tac_sim_optimized.py runs fuzz/tac_passes.py's observer, which
  waits for the observing mode of the TAC request above.
- test_optimization_docs.py (6 tests) reads compile.py's and the two
  optimizer drivers' Python source with `inspect` to check the order of
  docs/optimizations/README.md. It would read the order from c6502cpp
  instead, from a command that lists the passes of each driver in the
  order they run.
- test_ir_check.py (20 tests) tests the Python `ir_check`, and goes to
  doctest with its port.
- The 125 tests that run the TAC interpreter take the TAC and the
  tables from C++ once the request above gives them.

**(c) The Python implementation.** test_asdl.py (32 tests of the
Python half of asdl.py), test_capture_passes.py (23),
test_cpp_backend.py (30, with the 6 of `--lex`), test_cpp_binutils.py
(11), test_pretty.py (7), test_peephole_memo.py (7), test_lark_cache.py
(6), test_lalr_table.py (3), test_capture_helpers.py (3), and
test_pcpp_evaluator_tables.py and test_pcpp_unicode_tables.py (1 each).
test_cpp_backend.py and test_cpp_binutils.py compare the switch with
Python, so they go with it, and the golden files below take over what
they check. The models that step 9 keeps are tested by
test_ir_dump.py (12), test_asdl_cpp.py (6), test_rtlib_table.py (5),
test_fuzz_tools.py (5), and seven tests of the BCD and `<stdbit.h>`
models in builtin_functions.py, which stay.

### The checks after Python

Three kinds of check could replace the comparisons with Python. Each
was measured on 23 Nov 2026 with the C++ port, on the jobs of
tools/compare_backends.py, which are the corpus of the examples, the games' units,
the chapters' programs, and fuzz seeds 0 to 99 of `stmts`, `calls`, and
`exprs`, in the modes `-S`, `-S -O`, `-c` and `-c -O` with their links,
and `--lex`, and the three games and the damaged files linked.

**Golden files.** The frozen Python compiler writes each command's
output once (its exit status, what it writes to stdout and stderr, and
the files it leaves), and from then on the C++ compiler's output is
compared with them, as examples/*.asm are now.

| Mode | Commands | Files | Raw | xz |
|---|---|---|---|---|
| `-S -O` | 1,788 | 1,152 | 8.3 MB | 0.50 MB |
| `-S` | 1,788 | 1,118 | 18.0 MB | 0.71 MB |
| `-c -O` and its links | 4,092 | 4,219 | 42.1 MB | 1.88 MB |
| `-c` and its links | 4,024 | 4,105 | 58.1 MB | 2.95 MB |
| `--lex` | 1,788 | 1,763 | 9.8 MB | 0.73 MB |
| the games and the damaged files | 90 | 102 | 4.9 MB | 0.22 MB |

All of them are 141 MB, 16.8 MB with zlib and 5.9 MB with xz. The C++
side ran the 8,944 jobs, 13,570 commands, in 68 seconds with 12
processes, through compile.py's `main` as compare_backends.py runs it,
so a check of every golden file takes about a minute. The objects and
executables are most of the size, and an object holds its `-S` text in
`.c6502.asm`, so a smaller form keeps the texts of `-S` and `-S -O`
(25.4 MB raw, and 1.2 MB with xz together with those modes' messages),
every command's messages (5.5 MB raw), and a SHA-256 of each of the
8,400 objects and executables (about 1 MB). git's packs use zlib, which
left the whole set 2.9 times the size xz did. The repository's pack is 7.9 MB now,
and examples/*.asm, 57 files of 1.2 MB, are the model. A golden file
catches any change to any output of the corpus, the messages of every
rejected program included, but it doesn't name the pass that made the
change, and a change that the passes after it undo leaves no trace in
it.

**Frozen captures.** The records of the captures turned into the C++
checkers' regression inputs. The capture of `split_mem_to_mem` on disk
shows the sizes. Its 7,049 distinct calls are 208 MB raw and 3.9 MB
with xz, the 1,564 calls that change the program are 69 MB and 2.0 MB,
the 200 smallest of those are 478 KB and 18 KB, and the 50 smallest
are 60 KB and 3 KB. A whole capture of every pass is far too big, about
1 GB for the examples and the games alone (cpp/README.md, "The captured
calls"). Checked in, a capture would be the 50 smallest calls that
change each function's result, and every call that raises, for each
of about 200 functions the checkers run (73 asm passes, the stage
calls of steps 4 and 5, the TAC passes, the front end's calls, and the
records of step 7). At the rate of `split_mem_to_mem` that is about 12
MB raw and under 1 MB with xz, though the stage calls carry whole
symbol tables, which could make it several times that (an estimate
from the size of the cases in cpp/tests/test_inline_functions.cpp). A
frozen capture names the pass whose result changed, and it checks each
pass on inputs the corpus gives it, also where the passes after it
would hide a change. It can't follow a change of a pass's input that a
change to an earlier pass makes, since its inputs are frozen.

**The doctest cases alone.** 3,326 cases, in 17 MB of cpp/tests, run
in a few seconds. They are the only check of the messages that neither
the corpus nor the suite reaches (every raise site has a case), and
they check one function on one input each, but they cover only the
inputs someone wrote down, which is why the corpus found the bugs it
did in steps 2 to 7.

The proposal is the golden files of the smaller form as the main
check, with a C++ command that checks them without Python
(`c6502cpp check-golden`), the frozen captures of the 50 smallest
changing calls of each function, compressed, for the passes that the
corpus changes least often, and the doctest cases as now, with a case
for each new bug. A fix that changes the output on purpose rewrites
the golden files with the C++ compiler, and the diff of the `-S` texts
is the review of the change. The fuzz seeds' programs come from
generators that stay in Python until step 9, so their golden files
hold the generated source too.

### The C++ command line

`--lex` runs in C++ since this survey. The other stages and
compile.py's own reading of its command line move as follows.

- **`--parse` and `--resolve`** print `pretty(program)`, the c99
  program as nested `Class(field=value, ...)` lines with two spaces of
  indentation per level, lists broken one item to a line, and Python's
  `repr` of every leaf, which quotes a string as Python does and
  writes `True`, `False`, `None`, and a float in its shortest
  round-trip form.
  `--resolve` runs the unroller under `-O`, the name passes, string
  lifting, and the two label passes, but no type checker. The C++ IR
  has the field names, so `asdl.py --cpp` can generate a `pretty` for
  each module, as it generates the dump's writer.
- **`--tac`** prints `pretty` of the TAC after the type checker and
  `c99_to_tac`, and under `-O` after `inline_functions` and
  `optimize_tac` only (above). Nothing reads its text but the tests
  that call `_run_stage("tac")` (9 calls) and compare it, and the 87
  tests whose classes run `--parse`, `--resolve`, or `--tac`. `tac_sim`
  and the fuzzers take Python's TAC objects, which the TAC request
  above replaces.
- **argparse.** compile.py's parser has a positional argument of one
  or more inputs, `-o`, a mutually exclusive group of six
  `store_const` stage flags (`--lex` to `-c`), `-O`, `-T`, `--origin`
  with `type=_number` (whose error names the function, `invalid
  _number value`), and `--defsym` with `append`, read with
  `parse_known_args` so that the rest goes to the preprocessor, after
  `_rewrite_ttext` has turned `-Ttext=ADDR` into `--origin`. The C++
  argparse (src/argparse.h) has `store_true`, `append`, help, and one
  positional argument of one or more values. compile.py's parser adds
  options that take a value, `store_const`, mutually exclusive groups
  with their message (`argument -S: not allowed with argument --lex`),
  `type=` with its message, and `parse_known_args`. compile.py's help
  has hyphens inside words ("comparison-against-zero",
  "byte-granular"), where textwrap breaks a line, and the C++
  HelpFormatter raises InternalError for those now, so textwrap's
  breaking at hyphens has to be ported too. pcpp's own
  argparse is in C++ already (src/preprocessor.*).
- **`ir_check`.** passes/ir_check.py checks the TAC after every TAC
  pass, the asm at the stage boundaries and after every peephole sweep,
  and the asm2 program's decimal mode. Under the switch only the checks
  after the stages run, on the dumps in the response, and the checks
  between the passes don't. Ported to C++ on the helpers the asm passes
  already share, it would run at all of Python's places when the
  request's `check_ir` is set, raise `IRCheckError` as an
  `InternalError`, and let the response stop carrying the dumps.

compile.py, nm.py, and objdump.py should stay as shims until step 9,
since the suite, the simulator, and the fuzzers call `compile.main`,
`_run_stage`, `_compile_c`, `nm.main`, and `objdump.main` in-process and
redirect their streams. compile.py would read its command line in C++
too, through a command that takes compile.py's whole argv as nm.py's
switch does (`cpp_backend.run_tool`), and keep `_run_stage` and
`_compile_c` as functions that send one request each, so that no Python
of the compiler is left behind them.

These choices are Robert's.

1. **The executable's name and its messages.** The messages and the
   usage say `compile.py:`, and the tests and the golden files hold
   them. The recommendation is to keep `c6502cpp` with a `compile`
   command that prints `compile.py` until step 9, when the tests move
   and the names can change once, to `c6502` (or `c6502cc`), `c6502-nm`,
   and `c6502-objdump`, say.
2. **The shims.** The recommendation is to keep compile.py, nm.py, and
   objdump.py as shims until step 9, as above.
3. **The format of `--parse`, `--resolve`, and `--tac`.** Python's
   `pretty` keeps the 87 tests that compare it. The dump format is
   shorter, and C++ already writes it. The recommendation is `pretty`
   in step 8, since it costs one generated function per IR, and to
   leave a change of format to step 9.
4. **How the (b) tests are ported.** The recommendation is the recorded
   calls for the tests on hand-built IR and the object-file tools, and
   a port by hand, or a corpus check, for the front-end tests whose
   recorded calls would be very large.
5. **The checks after Python.** The recommendation is the golden files
   of `-S` and `-S -O` texts, messages, and hashes, checked in raw,
   with frozen captures of the 50 smallest changing calls of each
   function, compressed with xz.
6. **`ir_check`.** The recommendation is to port it, since the fuzzers
   rely on it to catch a pass that leaves the IR inconsistent before the
   result shows it.
7. **The TAC interpreter's input.** A reader of the C++ tables in
   Python lets `tac_sim` and the fuzzers stay in Python until step 9.
   Porting `tac_sim` to C++ now would take that part of step 9 early.
   The recommendation is the reader.
8. **The variable.** Once the Python compiler is gone,
   `C6502_CPP_BACKEND=1` has nothing to switch. The recommendation is
   that compile.py ignore it from the removal on, and that the
   documents stop mentioning it.

Robert decided on 23 Nov 2026. He took the recommendations of choices
1 to 6 and 8, so the names and the `compile.py:` messages stay until
step 9, the shims stay, `--parse`, `--resolve`, and `--tac` print
`pretty`, the checks after Python are the golden files with the frozen
captures, and `ir_check` is ported. For choice 7 he chose to port
`tac_sim` to C++ in step 8, so no Python reader of the C++ tables is
written, and G4 below ports the interpreter instead. He also asked that
the groups of step 8 run one at a time, each merged, checked, and
pushed before the next one starts, and the same for the steps after it.

### The groups

Each group works in its own worktree, adds C++ tests for what it ports,
and checks its piece against Python while Python is still there, as
the earlier steps did. The groups share the request format of
`c6502cpp backend` (src/backend.h), to which each adds a mode, and
cpp_backend.py, to which each adds a function. G1 declares the order of
the modes in backend.h first.

- **G1. The stages.** `pretty` for the four IRs, generated by
  `asdl.py --cpp` into cpp/generated/, with a test that compares it
  with pretty.py on each IR of a compiled example, and the backend's
  modes for `--parse`, `--resolve`, and `--tac`, checked by
  tools/compare_backends.py's modes for them on the corpus. Files:
  asdl.py, cpp/generated/, src/backend.*, cpp_backend.py, compile.py,
  tools/compare_backends.py, tests/test_cpp_backend.py.
- **G2. compile.py's command line.** The rest of argparse in
  src/argparse.*, a `c6502cpp compile` command with compile.py's
  argparse and the dispatch of `main`, and compile.py as a shim over
  it. Its check is compare_backends.py's bad command lines, help, and
  usage at several terminal widths, as tests/test_cpp_binutils.py does
  for nm and objdump. It calls G1's modes, so it lands after G1. Files:
  src/argparse.*, src/main.cpp, a new src/compile_command.*,
  compile.py.
- **G3. `ir_check` in C++.** src/passes/ir_check.*, called from the
  peephole fixed point, the TAC optimizer's driver, and the stage
  drivers when `check_ir` is set, with test_ir_check.py's 20 tests as
  doctest cases, and checked on the fuzzers with a pass broken on
  purpose. Then cpp_backend.py stops loading the dumps. Files:
  src/passes/ir_check.*, src/passes/peephole_fixedpoint.cpp,
  src/passes/optimization/driver.cpp, src/passes/codegen_stage.cpp,
  src/backend.*, cpp_backend.py.
- **G4. What step 9 reads.** The table classes of type_checking.py in a
  module of their own, the exception classes with Lark's hierarchy in
  another, and a Python reader of the C++ tables' tagged JSON. The
  backend's modes for the TAC with its tables, after each pass with
  the observer, and after `optimize_tac`, and the edit of
  `falls_off_end`. Then `tests.test_sim_chapters._run_program`, the
  harness's `_PIPELINE`, fuzz/reduce.py, fuzz/tac_passes.py, and
  `linked_text` move onto them. Files: passes/type_checking.py, new
  modules beside it, cpp_backend.py, sim/harness.py,
  tests/test_sim_chapters.py, tests/link_helpers.py, fuzz/, src/backend.*.
- **G5. The checks after Python.** A tool that writes the golden files
  from the frozen Python compiler, `c6502cpp check-golden`, the frozen
  captures, and their place in the suite and in cpp/README.md. It can
  start at once, since it only reads the outputs. Files: tools/, a new
  directory of golden files, src/main.cpp, a new src/golden.*.
- **G6, G7, and G8. The (b) tests.** G6 takes the 1,402 tests on
  hand-built asm and the 90 of the object-file tools, G7 the 815 tests
  of the TAC passes and the TAC interpreter (the 316 on TAC they build,
  the 466 of the front end followed by TAC, and the 33 of
  `_run_program`), and G8 the 1,265 front-end tests and the 232 of
  compile.py's functions. Each checks its modules against the C++ cases one test
  at a time, records or ports what isn't covered, and removes the
  Python test once its C++ case passes. A class that mixes a direct
  call with an end-to-end check keeps the end-to-end part in Python.
  They touch disjoint test modules and add to cpp/tests/, so they run
  in parallel. G7's tests of `tac_sim` wait for G4.
- **G9. The end-to-end tests' edits.** The helper that sends the
  source text in place of `preprocess` for the 342 tests, the skip
  conditions of 156 modules, and tests/no_inlining.py's flag. It
  touches many test modules, so it lands after G6 to G8 have removed
  theirs.
- **R. The removal**, last. The root's `asm_emit`, `c99_to_tac`,
  `lark_cache`, `lexer`, `parser`, `pp_numbers`, `preprocessor`,
  `pretty`, and `tac_to_asm` go, with c99.lark, whose table
  cpp/generated/c99_lalr.cpp then becomes source. Every module of
  passes/ goes but `runtime_layout`, `dataclass_fields`,
  `type_check_error`, and the new modules of G4, and `ir_check` too
  unless G3 is done. binutils' `assembler`, `disasm`, and `ld` go, and
  so do the capture tools, tools/check_captures.py, compare_backends.py,
  binutils_errors.py, lalr_table.py, pcpp_evaluator_tables.py,
  pcpp_unicode_tables.py, and py_regex_records.py, once their outputs in
  cpp/generated/ are marked as source, and the tests of (c).
  pyproject.toml and requirements.txt
  drop `lark` and `pcpp`, and `uv lock` writes uv.lock again. numpy
  (for fp_arith.py) and py65 (for the simulator) stay until step 9. The
  Dockerfile drops `uv tool install` of pcpp and installs CMake and
  builds cpp/, since the image has no CMake today. The CLAUDE.md files
  of the root, passes/ and its three subdirectories, binutils/, tests/,
  sim/, and fuzz/ describe Python modules that are gone, and the
  passes' descriptions move to the C++ files that cpp/README.md lists,
  as do README.md's build section, docs/architecture.md's table of
  stages and modules, and the module paths of docs/optimizations/*.md.
  The Python left is about 5,500 lines of the compiler's models plus
  the shims, sim/, fuzz/, tac_sim.py, asdl.py, and the tests of (a).

G1, G3, G5, and G6 to G8 can start at once, G4 too except for the
tests of `tac_sim`, G2 after G1, G9 after G6 to G8, and R last.

#### G6's modules

The survey's classification, run again on 24 Nov 2026 on the suite as
it stood after G2, gave G6 the same 1,492 tests: 1,402 that call an asm
pass or code generation on IR they build, of which 31 also call a TAC
pass, and 90 that call the assembler, the linker, or their helpers, in
337 classes of 117 modules. Each test was run under
tools/record_test_calls.py (cpp/README.md, "The unit tests' recorded
calls"), which recorded the calls the test made of the functions that
`check-captures` runs and listed its other direct calls into the
compiler. A test was removed when its recorded calls were all its
direct calls, apart from reading `binutils/elf.py`'s objects and the
runtime layout, which step 9 keeps, building or reading the argument and
result objects of a recorded call, and switching the IR checks on, and
when each of those calls had a C++ case that passed in the MSVC and GCC
builds. The C++ cases are 827 new ones in 78 files, 539 that steps 3 to
5 had recorded from the same tests, and 4 written by hand. The table
counts the cases each module's removed tests rely on, by file, and
gives the reason each remaining test is still in Python. "End to end"
marks a test that compiles or runs a program and stays as kind (a).
"Call" names the direct calls that have no C++ runner yet, which a test
ported by hand has to replace.

| Module | (b) tests | Removed | The C++ cases of their calls | Left | Why each left is still in Python |
|---|---|---|---|---|---|
| test_adc_commute.py | 23 | 23 | test_adc_commute_calls.cpp (23) | 0 |  |
| test_address_taken_zp.py | 5 | 5 | test_address_taken_zp_calls.cpp (5) | 0 |  |
| test_ali_baba.py | 2 | 0 |  | 2 | 1 call `defined`, `read_file`; 1 call `helper_names` |
| test_and_sign_bit_branch.py | 21 | 21 | test_and_sign_bit_branch_calls.cpp (21) | 0 |  |
| test_asm_aliasing.py | 36 | 0 |  | 36 | 31 call `may_alias`; 4 call `may_alias`, `set_user_zp_globals`; 1 end to end |
| test_asm_backward_copy_propagation.py | 18 | 18 | test_asm_ssa_round_trip.cpp (18) | 0 |  |
| test_asm_byte_dce.py | 8 | 8 | test_asm_ssa_round_trip.cpp (8) | 0 |  |
| test_asm_cfg.py | 11 | 0 |  | 11 | 9 call `build_cfg`; 1 call `build_cfg`, `cfg_to_function`; 1 call `build_cfg`, `dominance_frontiers`, `immediate_dominators` |
| test_asm_copy_propagation.py | 19 | 19 | test_asm_ssa_round_trip.cpp (19) | 0 |  |
| test_asm_dead_store.py | 17 | 15 | test_asm_dead_store.cpp (15) | 2 | 2 end to end |
| test_asm_emit.py | 145 | 145 | test_asm_emit.cpp (209), test_asm_emit_calls.cpp (4); none, a Python subclass of an IR base | 0 |  |
| test_asm_licm.py | 10 | 10 | test_asm_licm_calls.cpp (10) | 0 |  |
| test_asm_liveness.py | 15 | 0 |  | 15 | 1 call `a_dead_at`; 1 call `a_dead_at`, `y_dead_at`; 3 call `all_flags_dead_at`, `flags_dead_at`; 3 call `flags_dead_at`; 3 call `kills_a`; 1 call `reads_x`, `reads_y`, `x_dead_at` and others; 2 call `sets_flags`; 1 call `x_dead_at` |
| test_asm_regalloc.py | 6 | 3 | test_asm_regalloc_calls.cpp (13) | 3 | 1 call `_instr_writes_hwregs`; 1 call `check_program`, `label_program`, `lift_program` and others; 1 end to end |
| test_asm_remat.py | 19 | 18 | test_asm_remat.cpp (22) | 1 | 1 end to end |
| test_asm_ssa_construction.py | 7 | 7 | test_asm_ssa_round_trip.cpp (7) | 0 |  |
| test_asm_ssa_destruction.py | 5 | 5 | test_asm_ssa_destruction_calls.cpp (2), test_asm_ssa_round_trip.cpp (6) | 0 |  |
| test_asm_to_asm2.py | 17 | 17 | test_asm_to_asm2.cpp (19), test_asm_to_asm2_calls.cpp (1) | 0 |  |
| test_bcd_builtins.py | 4 | 0 |  | 4 | 4 call `decimal_region_problems` |
| test_bin_to_bcd.py | 19 | 4 | test_bin_to_bcd_ir_check.cpp, by hand | 15 | 4 call `_resolved`, `check_program`, `optimize_program` and others; 1 call `bin_to_bcd`, `check_program`, `label_program` and others; 1 call `check_program`, `is_builtin`, `parse` and others; 2 call `check_program`, `parse`, `resolve_program`; 1 call `helper_footprints`; 1 call `helper_sizes`; 1 call `parse`, `resolve_program`; 4 end to end |
| test_binutils.py | 17 | 0 |  | 17 | 2 call `assemble`; 1 call `assemble`, `defined`, `relocate` and others; 1 call `assemble`, `defined`, `symbol`; 1 call `assemble`, `disassemble`, `relocate` and others; 1 call `assemble`, `link_text`, `read` and others; 3 call `assemble`, `read`, `write`; 1 call `assemble`, `relax`; 2 call `assemble`, `relax`, `relocate` and others; 2 call `assemble`, `relocate`, `size`; 1 call `assemble`, `relocate`, `size` and others; 1 call `assemble`, `size`; 1 call `read_file` |
| test_branch_invert.py | 7 | 7 | test_branch_invert_calls.cpp (15) | 0 |  |
| test_branch_thread.py | 5 | 5 | test_branch_thread_calls.cpp (5) | 0 |  |
| test_branch_through_jump.py | 6 | 6 | test_branch_through_jump_calls.cpp (6) | 0 |  |
| test_branch_to_next_drop.py | 13 | 13 | test_branch_to_next_drop_calls.cpp (22) | 0 |  |
| test_branch_value_thread.py | 4 | 4 | test_branch_value_thread_calls.cpp (4) | 0 |  |
| test_byte_arith_sim.py | 1 | 1 | test_asm_optimizer_calls.cpp, recorded in step 4 | 0 |  |
| test_carry_halve.py | 5 | 5 | test_carry_halve_calls.cpp (5) | 0 |  |
| test_carry_in_add.py | 5 | 5 | test_carry_in_add_calls.cpp (5) | 0 |  |
| test_carry_inc.py | 16 | 16 | test_carry_inc_calls.cpp (17) | 0 |  |
| test_carry_known.py | 9 | 9 | test_carry_known_calls.cpp (10) | 0 |  |
| test_carry_recompute.py | 6 | 6 | test_carry_recompute_calls.cpp (6) | 0 |  |
| test_carry_return.py | 18 | 17 | test_carry_return_calls.cpp (19) | 1 | 1 call `all_flags_dead_at`, `c_dead_at` |
| test_carry_return_choice.py | 2 | 0 |  | 2 | 1 call `_merge`; 1 call `disassemble`, `link_text`, `read` |
| test_cmp_sbc_fusion.py | 11 | 11 | test_cmp_sbc_fusion_calls.cpp (11) | 0 |  |
| test_cmp_through_tmp_fold.py | 27 | 27 | test_cmp_through_tmp_fold_calls.cpp (30) | 0 |  |
| test_coalescing.py | 16 | 16 | test_coalescing_calls.cpp (16) | 0 |  |
| test_const_arith_fold.py | 24 | 24 | test_const_arith_fold_calls.cpp (24) | 0 |  |
| test_const_static_fold.py | 1 | 1 | test_asm_optimizer_calls.cpp (1) | 0 |  |
| test_constant_store_order.py | 17 | 17 | test_constant_store_order_calls.cpp (17) | 0 |  |
| test_counter_to_index_reg.py | 15 | 15 | test_counter_to_index_reg_calls.cpp (15) | 0 |  |
| test_cpx_cpy_peephole.py | 14 | 14 | test_cpx_cpy_peephole_calls.cpp (14) | 0 |  |
| test_cross_jump.py | 5 | 5 | test_cross_jump_calls.cpp (5) | 0 |  |
| test_dead_a_arith.py | 31 | 31 | test_dead_a_arith_calls.cpp (31) | 0 |  |
| test_dead_label_drop.py | 6 | 6 | test_dead_label_drop_calls.cpp (6) | 0 |  |
| test_dead_pha_pla.py | 12 | 12 | test_dead_pha_pla_calls.cpp (16) | 0 |  |
| test_dead_static.py | 10 | 10 | test_asm_optimizer_calls.cpp (10) | 0 |  |
| test_dec_inc_branch_fold.py | 7 | 7 | test_dec_inc_branch_fold_calls.cpp (7) | 0 |  |
| test_dec_peephole.py | 7 | 7 | test_dec_peephole_calls.cpp (7) | 0 |  |
| test_direct_index_load.py | 12 | 12 | test_direct_index_load_calls.cpp (12) | 0 |  |
| test_divide_shapes.py | 2 | 0 |  | 2 | 1 call `may_alias`; 1 call `sets_flags` |
| test_dual_index_promotion.py | 16 | 16 | test_dual_index_promotion_calls.cpp (16) | 0 |  |
| test_element_rmw.py | 22 | 21 | test_element_rmw_calls.cpp (21) | 1 | 1 call `check_asm`, `enable`, `enabled` |
| test_flag_compares.py | 8 | 8 | test_flag_compares_calls.cpp (8) | 0 |  |
| test_function_local_sizing.py | 9 | 8 | test_asm_optimizer_calls.cpp (8) | 1 | 1 call `compute_local_byte_addresses` |
| test_hwreg_pinning.py | 17 | 13 | test_asm_optimizer_calls.cpp (1), test_hwreg_pinning_calls.cpp (14) | 4 | 2 call `_match_transfer_chain`; 2 call `_rewrite_redundant_transfers` |
| test_in_place_member_update.py | 2 | 0 |  | 2 | 2 call `plan_in_place_updates` |
| test_inc_low_byte_test.py | 8 | 8 | test_inc_low_byte_test_calls.cpp (8) | 0 |  |
| test_inc_peephole.py | 11 | 10 | test_inc_peephole_calls.cpp (10) | 1 | 1 end to end |
| test_index_inc_cmp_y.py | 13 | 12 | test_index_inc_cmp_y_calls.cpp (13) | 1 | 1 end to end |
| test_ir_check.py | 3 | 0 |  | 3 | 3 call `check_asm`, `enable` |
| test_join_store.py | 6 | 6 | test_join_store_calls.cpp (6) | 0 |  |
| test_jump_table.py | 5 | 5 | test_jump_table_calls.cpp (5) | 0 |  |
| test_label_offset_fold.py | 4 | 4 | test_label_offset_fold_calls.cpp (4) | 0 |  |
| test_linker.py | 2 | 0 |  | 2 | 1 call `_merge_zp_objects`; 1 call `bytes`, `disassemble`, `link_text` and others |
| test_linker_script.py | 18 | 0 |  | 18 | 4 call `assemble`, `tail_jump`; 6 call `fall_through`; 2 call `lay_out`, `parse_script`; 3 call `output_sections`, `parse_script`; 3 call `parse_script` |
| test_long_branches.py | 6 | 6 | test_long_branches_calls.cpp (6) | 0 |  |
| test_loop_constant_a.py | 11 | 11 | test_loop_constant_a_calls.cpp (12) | 0 |  |
| test_loop_counter_to_x.py | 19 | 19 | test_loop_counter_to_x_calls.cpp (19) | 0 |  |
| test_loop_invariant_a.py | 5 | 5 | test_loop_invariant_a_calls.cpp (5) | 0 |  |
| test_lower_data_load_address.py | 8 | 8 | test_frames_stage_calls.cpp (8) | 0 |  |
| test_mem_const_prop.py | 10 | 10 | test_mem_const_prop.cpp (10) | 0 |  |
| test_memory_value_propagation.py | 31 | 31 | test_memory_value_propagation.cpp (31) | 0 |  |
| test_narrow_byte_range.py | 1 | 0 |  | 1 | 1 call `_shift_loop` |
| test_or_zero_absorb.py | 6 | 6 | test_asm_ssa_round_trip.cpp (6) | 0 |  |
| test_outline_runs.py | 6 | 6 | test_outline_runs_calls.cpp (7) | 0 |  |
| test_param_registers.py | 1 | 0 |  | 1 | 1 call `_split_entry_stubs` |
| test_pointer_aware_regalloc.py | 3 | 3 | test_pointer_aware_regalloc_calls.cpp (11) | 0 |  |
| test_pointer_member_access.py | 5 | 5 | test_pointer_member_access_calls.cpp (8) | 0 |  |
| test_private_pool_callee_save.py | 4 | 0 |  | 4 | 4 call `replace_function_bare_exit` |
| test_prune_unused_slots.py | 2 | 2 | test_emission_work.cpp (2) | 0 |  |
| test_recursion_saves.py | 3 | 3 | test_abi_stage_calls.cpp (1), test_recursion_saves_calls.cpp (1), test_sharing_stage_calls.cpp (1) | 0 |  |
| test_redundant_compare.py | 18 | 18 | test_redundant_compare_calls.cpp (18) | 0 |  |
| test_redundant_load.py | 83 | 81 | test_redundant_load.cpp (82) | 2 | 1 call `_same_index_apart`; 1 end to end |
| test_redundant_load_after_rmw.py | 12 | 12 | test_redundant_load_after_rmw_calls.cpp (12) | 0 |  |
| test_redundant_store.py | 16 | 16 | test_redundant_store.cpp (16) | 0 |  |
| test_register_arguments.py | 14 | 14 | test_register_arguments_calls.cpp (14) | 0 |  |
| test_replace_pseudoregisters.py | 21 | 4 | test_frames_stage_calls.cpp (2); none, a Python subclass of an IR base | 17 | 14 call `replace_function`; 1 call `replace`; 2 end to end |
| test_replace_pseudoregisters_zp_abi.py | 6 | 1 | test_frames_stage_calls.cpp (1) | 5 | 5 call `replace_function_bare_exit` |
| test_ret_slot_coalescing.py | 5 | 3 | test_ret_slot_coalescing_calls.cpp (3) | 2 | 2 call `_instruction_operands` |
| test_rotate_loop_test.py | 11 | 11 | test_rotate_loop_test_calls.cpp (11) | 0 |  |
| test_round_trip_load.py | 8 | 8 | test_round_trip_load_calls.cpp (8) | 0 |  |
| test_runtime_layout.py | 12 | 1 | test_emission_work.cpp (3) | 11 | 1 call `bytes`; 2 call `bytes`, `choose_layout`; 2 call `choose_layout`; 1 call `defined`, `disassemble`, `is_global` and others; 1 call `defined`, `is_global`, `read_file`; 1 call `disassemble`, `link_text`, `read`; 3 end to end |
| test_runtime_library.py | 32 | 0 |  | 32 | 1 call `bin_to_bcd`, `defined`, `helper_names` and others; 1 call `closure`; 16 call `defined`, `helper_names`, `read_file`; 2 call `defined`, `helper_names`, `read_file` and others; 1 call `helper_names`; 1 call `helper_names`, `helper_sizes`; 7 call `helper_names`, `read_file`; 1 call `link_text`, `read_file`, `symbol`; 2 end to end |
| test_self_store_drop.py | 4 | 4 | test_self_store_drop_calls.cpp (4) | 0 |  |
| test_shared_tails.py | 6 | 6 | test_shared_tails_calls.cpp (6) | 0 |  |
| test_shift_in_a.py | 20 | 20 | test_shift_in_a_calls.cpp (24) | 0 |  |
| test_short_jumps.py | 9 | 9 | test_short_jumps_calls.cpp (11) | 0 |  |
| test_slot_coalescing.py | 11 | 11 | test_slot_coalescing_calls.cpp (11) | 0 |  |
| test_slot_sharing.py | 6 | 0 |  | 6 | 1 call `check_linked_shares`; 1 call `parse_metadata`; 4 end to end |
| test_split_mem_to_mem.py | 14 | 14 | test_split_mem_to_mem_calls.cpp (14) | 0 |  |
| test_strength_reduction.py | 7 | 0 |  | 7 | 1 call `enable`, `enabled`; 6 end to end |
| test_sub1_test_zero_peephole.py | 5 | 5 | test_sub1_test_zero_peephole_calls.cpp (5) | 0 |  |
| test_tac_to_asm.py | 38 | 7 | test_tac_to_asm_recorded.cpp (7) | 31 | 1 call `translate_binary`; 3 call `translate_function`; 19 call `translate_instruction`; 5 call `translate_unop_atoms`; 3 call `translate_val` |
| test_tac_to_asm_zp_abi.py | 3 | 3 | test_tac_to_asm_zp_abi_calls.cpp (3) | 0 |  |
| test_tail_call.py | 9 | 8 | test_tail_call_calls.cpp (8) | 1 | 1 end to end |
| test_tail_merge.py | 7 | 7 | test_tail_merge_calls.cpp (7) | 0 |  |
| test_transfer_past_branch.py | 6 | 6 | test_transfer_past_branch_calls.cpp (6) | 0 |  |
| test_transfer_pm1_store.py | 27 | 27 | test_transfer_pm1_store_calls.cpp (27) | 0 |  |
| test_via_a_store_fold.py | 15 | 15 | test_via_a_store_fold_calls.cpp (15) | 0 |  |
| test_volatile_named.py | 2 | 1 | test_volatile_named_calls.cpp (4) | 1 | 1 end to end |
| test_volatile_void_read_cmp.py | 4 | 4 | test_volatile_void_read_cmp_calls.cpp (6) | 0 |  |
| test_x_save_slot_load.py | 13 | 13 | test_x_save_slot_load_calls.cpp (13) | 0 |  |
| test_y_peephole.py | 10 | 10 | test_y_peephole.cpp (10) | 0 |  |
| test_zp_link_metadata.py | 11 | 3 | test_sharing_stage_calls.cpp (3) | 8 | 1 call `local_bytes`, `param_bytes`; 1 call `param_bytes`; 6 call `parse_metadata` |
| test_zp_local_allocation.py | 16 | 16 | test_sharing_stage_calls.cpp (10), test_zp_local_allocation_calls.cpp (6) | 0 |  |
| test_zp_slot_allocation.py | 8 | 8 | test_sharing_stage_calls.cpp (7), test_zp_slot_allocation_calls.cpp (1) | 0 |  |

Of the 273 tests left, 28 are end to end and stay. The other 245 are
left for a second agent, by module: test_asm_aliasing.py (35 calls of
`may_alias`), test_tac_to_asm.py (31 of `translate_instruction` and the
Translator's methods), test_runtime_library.py (30 links that read the
helpers' names from `runtime_library.helper_names`), test_linker_script.py
(18 of `parse_script`, `lay_out`, `fall_through`, and `tail_jump`),
test_binutils.py (17 of the assembler and `ld`), test_replace_pseudoregisters.py
and test_replace_pseudoregisters_zp_abi.py and test_private_pool_callee_save.py
(26 of `replace_function` and `replace_function_bare_exit`),
test_asm_liveness.py (15 of its predicates), test_bin_to_bcd.py (11 of
the front end and `optimize_program`), test_asm_cfg.py (11 of
`build_cfg` and the dominators), test_runtime_layout.py (8 of
`choose_layout`), test_zp_link_metadata.py (8 of `parse_metadata`), and
35 more in 19 modules with one to four each. The asm helpers'
predicates, `may_alias`, and the CFG are ported and checked by
`check-helpers` on the corpus, and cpp/tests/test_asm_helpers.cpp holds
a hand port of part of their tests, but no case was matched against each
Python test, so those tests stay until a recorder for the helpers'
calls, or a check of each test by hand, shows that one covers them.

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
  A bug found on the way was fixed in both compilers. When a block ended
  in a branch to the label right after it, and that label's block had
  `Phi`s, `from_ssa` gave both of each `Phi`'s arguments from it to the
  split of the taken edge, so the path that fell through skipped the
  moves. No program of the corpus has that shape.
- 20 Nov 2026: group E, the register allocation of the asm optimizer.
  `compute_liveness`, `build_interference`, `coalesce_moves`,
  `scan_function` (recorded as `hwreg_eligibility`), and `color_graph`
  are ported with the `InterferenceGraph`, `_blocked_bytes`,
  `_find_fit`, and `optimizer._all_operands_in`
  (`src/passes/optimization_asm/all_operands.h`), and each matches the
  Python function on every distinct call of the corpus: 3,591, 3,668,
  3,591, 3,601, and 6,446 calls. The harness records a `Liveness` as
  the shape of its CFG and its sets, and the graph and the coalescing's
  byte offsets sorted, since Python fills those dicts in the order of
  set iteration, which nothing reads (cpp/README.md, "The register
  allocation"). The liveness uses the CFG of step 2, so no CFG with
  blocks of their own was needed, and it reads group D's
  `decimal_arith` helpers, with `pseudo_reads` and `pseudo_writes` as
  pointers into the instruction (`pseudo_read_ptrs`,
  `pseudo_write_ptrs`).
- 20 Nov 2026: group A of step 4, the ABI and the call graph on TAC.
  `select_abi`, `fixed_address_layouts`, `pinned_slot_symbols`,
  `choose_global_carry_returns`, `choose_carry_returns`,
  `pool_eligible`, `recursive_pool_functions`, all of
  `recursion_saves`, and the TAC CFG are ported, and each matches the
  Python function on every distinct call of the corpus: 1,150 calls of
  `select_abi` and of `fixed_address_layouts`, 1,110 of each carry
  choice, 952 of each pool function and of `hw_stack_cycles`, 786 of
  `cycles`, 1,149 of `check_marked_calls`, 16 of `plan_saves`, 58 of
  `pinned_slot_symbols`, 3,382 of `live_out`, and 3,394 of `build_cfg`.
  The CFG's dominators, frontiers, and loops match on every function of
  the capture helpers' 1,110 `tables` records. Groups B and C read
  `src/passes/recursion_saves.h` (`plan_saves`, `recursion_saves::live_out`
  and `defs_in`) and `src/passes/optimization/cfg.h`.
- 20 Nov 2026: group C of step 4, `tac_to_asm`. `translate_program` is
  ported as `tac_to_asm::translate_program` (`src/tac_to_asm.h`, which
  group F includes), with the Translator's queries shared by three
  parts in files of their own (calls and memory, arithmetic, and casts
  and comparisons), and with `significant_bytes`,
  `in_place_member_update`, `pointer_restage`, `shift_cost`,
  `split_tables.plane_init`, `compare_subtract.compared_difference`, and
  the parts of `bit_precise` it reads. It matches the Python function on
  all 2,256 distinct calls of the corpus, with `-S` and with `-S -O`,
  including the symbol table's `<param>.reg` locals, in the builds of
  MSVC and GCC. A line trace found 27 lines of the lowering that the
  Python unit tests reach and the corpus doesn't, and four recorded
  calls of the unit tests reach them in the C++ tests. The unoptimized
  pipeline calls it with `fixed_address_layouts`' ABI, and
  `plan_zero_page` without one, which group F connects.
- 20 Nov 2026: group B of step 4, sharing and the zero-page allocators.
  `find_shares`, `pinned_param_copies`, `slot_aliases`,
  `changed_param_slots`, `preferred_symbols`, `allocate_zp_slots`,
  `allocate_function_locals`, `build_local_slot_symbols`,
  `allocate_zp_globals`, `plan_zero_page`, `access_weights`,
  `choose_layout`, and `build_metadata` are ported, and each stage call
  matches the Python function on every distinct call of the corpus:
  2,292 calls of `plan_zero_page`, 1,145 of `access_weights`, 1,110 each
  of `find_shares` and `pinned_param_copies`, 952 of
  `allocate_zp_slots`, 964 of `allocate_function_locals`, 778 of
  `local_slot_symbols`, 215 of `slot_aliases`, 1,107 of
  `changed_param_slots`, 216 of `preferred_symbols`, and 949 of
  `build_metadata`. `choose_layout` and `allocate_zp_globals` take a
  function, so they are checked through `plan_zero_page` and the 551
  calls recorded from the unit tests (cpp/README.md, "Sharing and the
  zero-page allocators"). `plan_zero_page` takes the HARGS measurement
  of `tac_to_asm`'s translation as a function argument, which group F
  connects to group C's port.
- 20 Nov 2026: part F2 of step 4, the end of the stage.
  `compute_address_taken_assignments` and `slot_symbols` of
  `address_taken_zp`, `local_slot_names` and `compute_local_slot_names`
  of `zp_slot_naming`, `replace_program` and `replace_program_bare_exit`,
  and `lower_data_load_address` are ported, and each stage call matches
  the Python function on every distinct call of the corpus: 1,110 calls
  of `address_taken_assignments`, 3,798 of `address_taken_slot_symbols`,
  1,110 of `replace_pseudoregisters_bare_exit`, 855 of
  `local_slot_names`, 778 of `local_slot_symbols`, 938 of
  `lower_data_load_address`, and 1,146 of `replace_pseudoregisters`.
  `local_slots` (src/passes/zp_slot_naming.h) gives the driver the
  names of the pools' bytes and group B's `build_local_slot_symbols` of
  them in one call, and `local_slot_names` is in a file of its own,
  which part F1's `apply_coloring` shares. The Python unit tests' 66
  distinct calls are recorded as C++ cases.
- 20 Nov 2026: part F1 of group F of step 4, the asm optimizer's
  program level. `optimize_program` and `_optimize_function` (as
  `optimize_function`, with `_entry_register_limits`,
  `_explicit_index_tests`, `_project_eligibility`, and
  `_check_nothing_live_across`), `apply_coloring`, `fold_const_statics`,
  `apply_dead_static_elimination`, `compute_local_bytes`, and
  `compute_address_taken_bytes` are ported, and each matches the Python
  function on every distinct call of the corpus: 2,231 calls of
  `asm_optimizer`, 7,157 of `optimize_function`, 7,016 of
  `apply_coloring`, 1,110 of `fold_const_statics`, 1,771 of
  `dead_static_elimination`, 937 of `compute_local_bytes`, and 1,110 of
  `compute_address_taken_bytes`. The stage's driver calls
  `optimize_program` from `src/passes/optimization_asm/optimizer.h` and
  the sizing from `src/passes/function_local_sizing.h`.
- 20 Nov 2026: step 4 is done. compile.py's sequence from `select_abi`
  to `synthesize_prologue`, with the pool retries, is one C++ function,
  `passes::optimized_front`, and `passes::optimized_stage` and
  `unoptimized_stage` run the whole stage from the TAC: `plan_zero_page`,
  whose HARGS measurement lowers the unoptimized TAC with the C++
  `tac_to_asm`, code generation, the link metadata, the back end's
  region, and emission (cpp/README.md, "The stage's driver"). With
  `C6502_CPP_BACKEND=1`, compile.py hands `c6502cpp backend` the TAC of
  `translate_for_codegen`, and under `-O` also the TAC after
  `_optimized_tac`, where step 5's passes end, and takes the `-S` text
  back, for `-S`, for `-c`, and for links of C inputs. The simulator
  harness takes back the finished asm program in the dump format.
  `tools/compare_backends.py` found the same output, exit status, and
  messages for all 3,576 compiles of the corpus. The suite passes with
  the variable set (4,870 tests, with `C6502_CHECK_IR=1`) and without
  it, the four fuzzers pass with it set (300 programs each of `exprs`,
  `calls`, and `stmts`, and 100 of `yarpgen`, none wrong), and every
  stage call of the corpus still matches. The first run of the suite
  found that the c99 program holds a declaration's unnamed parameter as
  None, which the dump format can't carry and the capture harness
  therefore never recorded, so `cpp_backend.py` sends it as the empty
  string, which `select_abi` reads as an unnamed parameter. With the
  variable unset and set, guards.c took 2.0 and 1.55 seconds, the Lode
  Runner units 13.5 and 11.8, and 20 `fuzz.stmts` programs 16.0 and
  7.8. A compile of guards.c spends 0.22 s of its 1.5 s in the C++
  process, and the 20 fuzz programs 2.3 s of 7.8 s. The rest of
  guards.c is Python's imports (0.5 s), the TAC optimizer, the
  inliner's estimates of cost, which run the TAC optimizer on copies of
  the callees, the parser, and the assembler of `-c`. Under the
  profiler, which about doubles the Python time, the 20 fuzz programs
  spent 7.1 s of 17.9 s in the TAC optimizer, 4.0 s of it in the IR
  checks the fuzzers turn on, 2.2 s in the parser, 1.5 s in the TAC
  interpreter, 1.4 s in the simulator's assembler, and 2.0 s in writing
  the requests and reading the programs back.
- 21 Nov 2026: the groundwork of step 5 ("Step 5" above). The survey
  splits the step into eight groups, A to H. `tools/capture_passes.py`
  records every call of the TAC optimizer's driver on a function
  (`--passes tac`) and the program-level calls from `c99_to_tac` to
  `split_tables` (`--passes tac-stage`), each driver call with only the
  symbols it read, and every value of the 355,075 distinct records of
  the corpus makes the trip to C++ and back, the c99 program and the
  tables of `c99_to_tac` among them. The helpers the groups share are
  ported: `rewrite_uses_in`, `value_range`, and the run loops of the
  pass framework (`src/passes/optimization/framework.h`), which match
  the Python helpers on 26,982 records of `capture_helpers.py
  --tac-only`, 2.44 million operands of `max_value` among them.
  `fold_truncate_extend` and `fold_extend_extend` are ported on the
  framework's def-use walk and match on all 9,273 and 9,268 of their
  distinct calls. The capture found a bug, fixed in Python, since
  neither pass is ported: unreachable-code elimination pruned a join's
  Phis in place, and dead-loop elimination retagged an exit Phi in
  place, which changed the function each pass was given, so a sweep of
  the fixed point whose only change was that looked like no change.
  351 calls of the corpus changed their input, though no output of the
  corpus changed with the fix.
- 21 Nov 2026: group B of step 5, the driver and SSA. `PhaseDriver.apply`
  with its two fixed points and their cap of 256 sweeps,
  `optimize_function`, `optimize_program`, `to_ssa`, `from_ssa`,
  `rotate_signed_countdown_loops`, and `noreturn_calls` are ported
  (cpp/README.md, "The TAC optimizer's driver"). The driver's table in
  `src/passes/optimization/optimizer.cpp` lists every pass of Python's
  `_DRIVER` in its order, with a null pointer for each pass that another
  group ports, which that group sets when it merges. Each ported call
  matches the Python function on every distinct call of the corpus:
  3,992 of `tac_to_ssa`, 3,746 of `tac_from_ssa`, 3,986 of
  `tac_rotate_signed_countdown_loops`, and 3,987 of
  `tac_noreturn_calls`. Only 6 of the rotation's calls and 29 of
  `noreturn_calls`' change their function, and no call of the corpus
  breaks a cycle of copies in `from_ssa`, so the 31 distinct calls of
  the Python unit tests are C++ cases too. The runners of
  `tac_optimize_function` and `optimize_tac` are set once the table is
  full.
- 21 Nov 2026: group F of step 5, addressing and sinking.
  `recognize_indexed_load`, `recognize_indexed_store`, and
  `recognize_indirect_indexed`, with the walk of `extend_add_chain`
  they share, `direct_member_access`, `static_indexed_access`,
  `phi_offset`, `sink_increments`, and `sink_and_past_branch` are
  ported, each in `src/passes/optimization/<module>.*` with a runner in
  tac_stage.cpp, and each matches the Python pass on every distinct
  call of the corpus in the builds of MSVC 19.36 and GCC 12.2: 9,273
  calls each of the two absolute,X recognizers, 3,942 each of
  `recognize_indirect_indexed`, `static_indexed_access`, and
  `phi_offset`, 9,275 of `direct_member_access`, 9,267 of
  `sink_increments`, and 9,243 of `sink_and_past_branch`. Few corpus
  calls of four of them change the function, so the C++ tests hold 230
  calls that the Python unit tests make, recorded with Python's results
  (cpp/README.md, "Addressing and sinking"). The helpers of
  narrow_widened_arith.py that two of the passes read (`var_unchanged`,
  `_register_temp`, `_byte_width`, `_is_unsigned`, and
  `_narrow_constant`) are in
  `src/passes/optimization/narrow_widened_arith.*`, which group D's
  port of the module's pass extends, and `split_tables._width` is
  `split_tables::width`. The one place among these passes that finds
  an instruction by identity, the access of
  `recognize_indirect_indexed`, takes the index at which the walk calls
  the rewrite, and every call gave Python's result.
- 21 Nov 2026: group C of step 5, folding. `constant_fold` (with the
  loads from constant arrays and from tables split into byte planes),
  `fold_into_constant_phi`, `fold_static_const_reads`,
  `distribute_constant`, `reduce_strength`, `reassoc_constants`, and
  `reassoc_offset_constant` are ported, with all of `fp_arith` and
  `builtin_functions`, and each pass matches the Python pass on every
  distinct call of the corpus: 12,931 calls of `tac_constant_fold`,
  12,891 each of `tac_fold_into_constant_phi` and
  `tac_distribute_constant`, 4,054 of `tac_fold_static_const_reads`,
  12,852 of `tac_reduce_strength`, 9,288 of `tac_reassoc_constants`,
  and 9,280 of `tac_reassoc_offset_constant`, in the builds of MSVC and
  GCC. The 273 calls of the Python unit tests that changed their
  function, or that the tests of the pass itself make, are C++ cases,
  and `fp_arith` and the BCD and <stdbit.h> arithmetic match the Python
  results on tables of patterns that include both zeros, subnormals,
  infinities, and quiet and signaling NaNs. `fp_arith` converts an
  integer to a `float` through a `double`, as numpy does, and returns
  the integer part of a floating value as a `TruncatedInt`, which a
  caller reduces modulo 2^N. `fold`, `is_integer_const`, and the
  helpers of strength reduction that groups D, E, and H read are in
  `src/passes/optimization/constant_folding.h` and
  `strength_reduction.h`. `c99_to_tac::tac_const_for` (src/c99_to_tac.h)
  is the piece of c99_to_tac.py these passes read, which group A's port
  of the module replaces. The entries of group C's passes and of group
  F's in the driver's table (`passes::tac_driver()`) are filled.
- 21 Nov 2026: group A of step 5, `c99_to_tac`, is ported
  (cpp/README.md, "c99_to_tac"), as `c99_to_tac::translate_program` in
  `src/c99_to_tac.*` and the three files of the lowering, on group C's
  `fp_arith` and `builtin_functions`, and its `tac_const_for` replaces
  the one group C ported for its passes. It matches the Python function on all
  1,154 distinct calls of the corpus, the 199 calls among them that
  change the c99 program in place included, and on the 2,825 distinct
  calls of the unit tests that return. 50 of the unit tests' calls,
  which reach the 253 lines of c99_to_tac.py and its helper modules
  that the corpus doesn't reach, are C++ tests. Its public header gives
  the other groups `tac_const_for`, `sizeof_c99`, and `byte_width_of`.
- 21 Nov 2026: group E of step 5, dataflow over the CFG.
  `copy_propagate`, `eliminate_dead_stores`,
  `eliminate_unreachable_code` and `eliminate_dead_loops` (the versions
  step 5a fixed, which build new Phis in Python), `thread_constant_jumps`,
  `eliminate_common_subexpressions`, `hoist_loop_invariants`,
  `fold_copies_in_fixedpoint`, `fold_copies_post_destruction`, and
  `read_stored_globals` are ported, each in
  `src/passes/optimization/<module>.*` with a runner in tac_stage.cpp,
  and each matches the Python pass on every distinct call of the corpus
  in the builds of MSVC 19.36 and GCC 12.2: 11,828 calls of
  `tac_copy_propagate`, 11,825 of `tac_eliminate_dead_stores`, 12,829
  each of `tac_eliminate_unreachable_code`, `tac_eliminate_dead_loops`,
  and `tac_thread_constant_jumps`, 11,830 of
  `tac_eliminate_common_subexpressions`, 11,825 of
  `tac_hoist_loop_invariants`, 9,573 of `tac_fold_copies_in_fixedpoint`,
  3,948 of `tac_fold_copies_post_destruction`, and 9,564 of
  `tac_read_stored_globals`. The C++ tests hold 199 calls that the
  Python unit tests make, recorded with Python's results, and the eight
  smallest corpus calls that change the function for each of the three
  passes whose corpus calls change it least often (cpp/README.md,
  "Dataflow over the CFG (group E)"). Dead-loop elimination reads group
  A's `c99_to_tac::kKeepLoopMark`. The entries of group E's passes in
  the driver's table are filled.
- 21 Nov 2026: group D of step 5, jump folds and narrowing. The rule
  set `fold_producer_jumps`, whose seven rules come from
  `cmp_zero_jump_fold`, `and_zero_jump_fold`, `lnot_jump_fold`, and
  `nonnegative_compare`, and the passes `narrow_widened_arith`,
  `narrow_byte_range`, and `narrow_byte_bitwise` are ported, and each
  matches the Python pass on every distinct call of the corpus in the
  builds of MSVC 19.36 and GCC 12.2: 12,839 calls of
  `tac_fold_producer_jumps`, 9,243 of `tac_narrow_widened_arith`, and
  9,045 each of `tac_narrow_byte_range` and `tac_narrow_byte_bitwise`.
  The 235 calls of the Python unit tests that changed their function, or
  that the tests make of a pass or a rule on its own, are C++ cases.
  `extend_source_unchanged`, which the narrowing passes read, is in
  `src/passes/optimization/cmp_zero_jump_fold.h`, and the narrowing pass
  of narrow_widened_arith.py is `apply_narrow_widened_arith`, in the
  files that hold group F's helpers of that module. Python finds an
  instruction's position by identity in four places of these passes,
  and the C++ passes take the position at which the scan reaches the
  instruction. The entries of group D's passes in the driver's table are
  filled.
- 21 Nov 2026: group G of step 5, the passes after the fixed point and
  after SSA. `hoist_subtract_to_compare` (which extends the port of
  `compare_subtract.compared_difference` that `tac_to_asm` reads),
  `hoist_divmod_pairs`, `count_loops_down`, `move_steps_onto_edges`,
  `split_three_way_compares`, `fold_short_circuit_jump` with
  `merge_equality_jumps`, and `thread_known_compares` are ported, each
  in `src/passes/optimization/<module>.*` with a runner in
  tac_stage.cpp, and each matches the Python pass on every distinct
  call of the corpus in the builds of MSVC 19.36 and GCC 12.2: 3,942
  calls each of the first five, 4,031 of `tac_fold_short_circuit_jump`,
  and 4,017 of `tac_thread_known_compares`. Few of the corpus calls
  change the function (from 1 of `split_three_way_compares` to 75 of
  `thread_known_compares`), so the C++ tests hold 197 calls, those of
  the Python unit tests that change the function or the symbols or run
  a pass on synthetic TAC and the corpus calls that change the
  function, and each of the 358 distinct calls of the unit tests
  matched. The passes needed no helper from another group. Their
  entries in the driver's table are filled, which leaves those of
  group D.
- 21 Nov 2026: group H of step 5, and step 5 is done. With every entry
  of the driver's table filled, the C++ TAC optimizer matches Python's
  on all 4,054 distinct calls of `tac_optimize_function` of the corpus,
  the inliner's estimates of cost among them, and on all 1,146 of
  `optimize_tac`, with no difference to fix. `inline_functions`,
  `return_pointers_in_zero_page`, `apply_row_address_tables`,
  `dispatch_const_pointer_arrays`, and `apply_split_tables` are ported
  (cpp/README.md, "The program level (group H)"), and each matches the
  Python function on every distinct call of the corpus in the builds of
  MSVC 19.36 and GCC 12.2: 1,150 calls of the inliner, 1,146 each of the
  next two, and 1,110 each of the last two. Few of those calls change
  the program, so the C++ tests hold 142 of the 159 distinct calls that
  the Python unit tests of the five modules make, every one of which
  matched except two made with the estimate of `row_address_tables`
  replaced. compile.py's `translate_for_codegen` and `_optimized_tac` are
  `passes::translate_for_codegen` and `optimized_tac`, and
  `passes::codegen` runs them and the stage of step 4 in one call. With
  `C6502_CPP_BACKEND=1`, compile.py hands `c6502cpp backend` the
  type-checked c99 program and its tables, after `unroll_program`, and
  takes back the `-S` text, for `-S`, `-c`, and links of C inputs, and
  the simulator harness takes back the finished asm program. The request
  says whether `-O` inlines, which a test's patch of
  `compile.inline_functions` turns off (`compile._inlining`). `--tac`
  stays in Python. The tables the stage changes stay in C++, so the
  response no longer lists the changes to the symbol table, and under
  `C6502_CHECK_IR=1` it holds the TAC that code generation lowered,
  which `ir_check.check_tac` checks once per function.
  `tools/compare_backends.py` found the same output, exit status, and
  messages for all 3,576 compiles of the corpus, and a new capture of
  step 5 (`--passes tac` and `--passes tac-stage`) matched on all
  355,422 of its distinct records. The suite passes with the variable
  set (4,891 tests, with `C6502_CHECK_IR=1`) and without
  it, and so do the four fuzzers with it set (300 programs each of
  `exprs`, `calls`, and `stmts`, and 100 of `yarpgen`, none wrong). The
  first run of the suite found that a test of the tables of row
  addresses replaced the Python estimate `_saving` and compiled through
  compile.py, which the C++ pass doesn't see, so that test now keeps the
  variable unset. With the variable unset and set, guards.c took 2.01
  and 1.13 seconds, the Lode Runner units 13.6 and 10.4, and 20
  `fuzz.stmts` programs 16.1 and 5.5, against Robert's target of 5. A
  compile of guards.c spends 0.34 s in the C++ process, and the rest is
  Python's imports (about 0.55 s) and its parser. Run in one process
  after the imports, the 20 fuzz programs took 4.98 s, 2.62 s of it in
  the 40 calls of `c6502cpp backend`. Under the profiler, which about
  doubles the Python time, the Python side of those 20 programs spent
  2.2 s in the Lark parser, which parses each program three times (once
  for the TAC interpreter and once for each of the two compiles of the
  harness), 1.7 s in reading back the asm programs and the TAC for the
  IR checks the fuzzers turn on, 1.5 s in the simulator's assembler,
  0.6 s in the preprocessor, and 0.5 s in building the simulator's
  runtime. The TAC interpreter's own run took 0.06 s.
- 21 Nov 2026: the groundwork of step 6 ("Step 6" above). The survey
  splits the front end into the groups P, R, T, N, U, and D, with plans
  for the parser and the preprocessor. `tools/capture_passes.py
  --passes front` records every call of the front end, errors and
  warnings included, and every value of its 20,177 distinct records of
  the corpus makes the trip to C++ and back. The pieces the groups share
  are ported: pp_numbers, which matches on all 7,255 distinct calls of
  the corpus and on 98 spellings and 69 positions in 19 texts written to
  reach every branch;
  diagnostics; Python's repr of every IR value, generated by `asdl.py
  --cpp`; and the type checker's type queries, which match on 2.3
  million values of 1,154 `c99types` records (`capture_helpers.py
  --front-only`), 1.77 million of them `common_type` of pairs of types.
  `loop_labeling` is ported and matches on all 1,477 distinct calls of
  the corpus, the 10 that raise `LoopLabelingError` included, and on the
  38 programs of the Python unit tests, in the builds of MSVC 19.36 and
  GCC 12.2. The survey found that compile.py printed a traceback for
  the `NotImplementedError` the parser raises for a variable-length
  array, a K&R parameter list, or `[static 3]`, and it now reports it as
  a one-line error.
- 21 Nov 2026: group T of step 6, the type checker. `check_program`
  with all of type_checking.py, constant_expression.py, designators.py,
  and stdbit_functions.py is ported in the survey's four parts, which
  share the `TypeChecker` class's declaration in
  `src/passes/type_checker.h` (cpp/README.md, "The type checker"). It
  matches the Python function on all 1,467 distinct calls of the corpus
  in both modes, the 313 that raise included, with the program stamped
  in place and both tables, and on all 3,204 distinct calls that the
  Python suite makes, 512 of which raise, in the builds of MSVC 19.36
  and GCC 12.2. 151 of the suite's calls reach the 615 lines of the four
  modules that no call of the corpus reaches, and they and the smallest
  call for each of the 149 messages the corpus never gives are C++
  cases (`tests/test_type_checking_calls.cpp`), as are the 314 distinct
  calls of `normalize_initializer` and the 40 of the constant evaluator
  that the unit tests make. The parser's folds and string_lifting will
  call `check_exp`, `check_struct_decl`, `check_block_var`,
  `named_constant`, `size_from_initializer`, and
  `designators::normalize_initializer` through the same headers.
- 21 Nov 2026: groups N and U of step 6. `identifier_resolution`,
  `label_resolution`, and the unroller are ported
  (`src/passes/identifier_resolution.*`, `label_resolution.*`, and
  `optimization_ast/unroll.*`, cpp/README.md, "The front end's calls"),
  and each matches the Python pass on every distinct call of the corpus
  in the builds of MSVC 19.36 and GCC 12.2: 1,566 calls of
  `identifier_resolution`, 79 of which raise, 1,487 of
  `label_resolution`, 10 of which raise, and 1,562 of `unroll_program`.
  A run of the Python suite with the three passes wrapped recorded
  4,236, 3,483, and 2,157 distinct calls, 128, 19, and 30 of which
  raise, and every one matched, messages included. The names of
  identifier resolution follow Python's walk, the order of the keyword
  arguments of each node it builds included, and the unroller's two
  module globals are members of an object made for each call. String
  lifting is left for after group T-B's designators. A bug found on the
  way was fixed in Python: a marked loop over a `short` or an `unsigned
  short` that comes back to a value within the cap of 256 passes, such
  as `for (short i = 0; i <= 32767; i += 256)`, raised a KeyError,
  since the unroller's table of type names for the message had neither
  type, and compile.py printed a traceback.
- 21 Nov 2026: group R1 of step 6, the lexer, the table of Lark's LALR
  automaton with the recognizer that runs it, and the syntax errors
  (cpp/README.md, "The parser's lexer and recognizer").
  `tools/lalr_table.py` writes the table, with its states numbered
  canonically so that the file doesn't depend on the hash seed, and
  `tests/test_lalr_table.py` fails when c99.lark and the file differ.
  The C++ lexer reads the terminals' patterns from the table and
  matches them with Python's semantics. `TokenStream`
  (src/parser_context.h) gives R2 and R3 the tokens as Lark's parser
  receives them, with typedef names retyped, and feeds each one taken
  to the recognizer, which reports a syntax error at Lark's token with
  Lark's message, reworded as `parse` rewords it. The check is on token
  streams that `tools/capture_parser_tokens.py` records from the corpus
  and from the Python tests. All 6,314 distinct ones matched, with
  every token's type, text, position, and scope depth and every syntax
  error's message, 277 distinct ones, and so did the 452 distinct calls
  of `tokenize`. Two things the plan left out came up. Lark makes one
  rule of each form of a `for` loop, so the end of the loop's scope is
  found by the rule's alias, and Lark's `accepts` includes `$END` where
  the input could end, since its interactive parser feeds `$END` as the
  end of the input.
- 21 Nov 2026: group P of step 6, the preprocessor. preprocessor.py and
  the parts of pcpp 1.30 and PLY it uses are ported in the plan's seven
  parts (cpp/README.md, "The preprocessor"): PLY's lexer with pcpp's
  rules in PLY's order, the lines, the macros with pcpp's lists of
  `expanded_from` shared as Python shares them, the directives and
  `#include` with pcpp's search and rewriting of paths, `#if` on PLY's
  LALR loop with the tables of the live parser and Python's arithmetic,
  `write`, and preprocessor.py's hooks and argparse's reading of the
  flags. The C++ matches Python on all 2,088 distinct `preprocess` calls
  of the corpus, on all 4,981 distinct calls the Python unit tests make,
  recorded by a pytest plugin with every file each call opened and
  checked by `c6502cpp check-preprocess`, and on 2,205 calls on
  generated inputs (macros with `#`, `##`, and `__VA_ARGS__`, every
  operator of `#if` at its limits and 600 random expressions, bad
  definitions and directives, comments, carriage returns, form feeds,
  includes and their guards, encodings, and every flag), errors,
  warnings, and stderr included, in the builds of MSVC 19.36 and GCC
  12.2. The evaluator matches on 7,349 expressions of its own. pcpp
  reports its errors on stderr without raising, so the capture of
  `preprocess` now records what a call writes there on a `stderr` line,
  which `check-captures` compares. The generated inputs found that pcpp
  expands a macro's argument with no memory of the macros around it, so
  `#define G F(G)` with `#define F(x) x` expands without end until
  Python raises RecursionError. The C++ raises it at 300 nested
  expansions, since the depth Python reaches depends on its caller's
  stack.
  Three things were left unfinished, `--debug`'s log, the matching of
  `--passthru-includes` with Python's `re`, and a `\N{name}` escape in
  an `#if` character constant, and the entry for the preprocessor's
  loose ends below records how each was finished.
- 21 Nov 2026: groups R2 and R3 of step 6, the parser's descent
  (cpp/README.md, "The parser's descent"). `parser::parse` is a
  recursive descent over R1's `TokenStream`, which runs each action
  after taking its construct's last token and before taking the next,
  so the lexer's scopes, the declarations, and the retyped typedef
  names follow Python's order, and every syntax error comes from the
  recognizer at Lark's token. An action that throws is checked against
  a copy of the recognizer, and when Lark would find the lookahead to
  be a syntax error before reducing the action's rule, the syntax error
  is thrown in its place. The skeleton, with the folds through the type
  checker, came first, and two agents ported the declarations (R2) and
  the statements and expressions (R3) on it. `parse` matches Python on
  all 1,788 distinct calls of the corpus, 221 of which raise, and on
  all 5,163 distinct calls that the Python suite makes, 693 of which
  raise, the records that R1 could check only up to an action's error
  included, in the builds of MSVC 19.36 and GCC 12.2.
  `tests/test_parser_errors.cpp` holds the shortest source of each of
  the 506 distinct messages. Two bugs found on the way were fixed in
  Python and C++: in `int * const * p` the `const` qualified the outer
  pointer instead of the inner one, and `int a[const]` raised
  TypeError, which compile.py printed as a traceback.
- 21 Nov 2026: string lifting, the last pass of group N of step 6
  (`src/passes/string_lifting.*`, cpp/README.md, "The front end's
  calls"), on T-B's `designators::normalize_initializer`. It matches
  the Python pass on all 1,487 distinct calls of the corpus in both
  modes and on all 3,495 distinct calls that the Python suite makes, 25
  of which raise TypeCheckError, messages included, in the builds of
  MSVC 19.36 and GCC 12.2. `tests/test_string_lifting.cpp` holds 32 of
  the suite's calls: those of the pass's own test module, the two that
  reach the lines no call of the corpus reaches, and one for each
  message.
- 21 Nov 2026: group D of step 6, the driver, and step 6 is done.
  `passes::front_end` (`src/passes/front_end.*`) is `_compile_c` from
  the source text through `type_check_program`, and `c6502cpp backend`
  runs it before `passes::codegen`, so with `C6502_CPP_BACKEND=1`
  compile.py hands C++ the source text of each C input of `-S`, `-c`,
  and a link, with the preprocessor's arguments and the file name, and
  the simulator harness does the same with no arguments. The request
  no longer carries a c99 program or its tables, and no caller is left
  that sends that form. The response carries what the preprocessor wrote to
  stderr, its warnings, and the class and message of an error, which
  `cpp_backend.py` writes, issues again, and raises as the Python
  exception, a subclass of the same name for Lark's syntax errors.
  `--lex`, `--parse`, `--resolve`, and `--tac` stay in Python. Run as a
  script with the variable set and a stage C++ runs, compile.py stops
  before it imports its pipeline, and the runtime helpers' footprints
  and the assembler's reading of the link metadata no longer import the
  type checker (`passes/type_check_error.py` holds `TypeCheckError`, and
  `passes.optimization` imports its optimizer on first use), so `-S` and
  `-c` import no part of the Python front end. A link still imports the
  type checker through the Python linker, which is step 7's.
  `sim.harness.compile_to_asm` returns the asm program and its slot
  symbols alone, since no caller read the tables. `tools/compare_backends.py`
  found the same output, exit status, messages, warnings, and exceptions
  for all 3,576 compiles of the corpus. The suite passes with the
  variable set (4,915 tests, with `C6502_CHECK_IR=1`) and without it,
  and the fuzzers with it set found nothing wrong in 150 programs each
  of `exprs`, `calls`, and `stmts` from seed 2000. With the variable set
  and unset, the best of three runs gave 0.61 and 2.14 seconds for
  guards.c `-c -O`, 4.6 and 14.7 for the Lode Runner units, and 4.9 and
  17.3 for 20 `fuzz.stmts` programs, against step 5's 1.13 and 10.4
  and 5.5 with the variable set and Robert's target of 5. Of the 20
  programs' 4.7 seconds after the imports, the TAC interpreter's path,
  which keeps a Python front end and `c99_to_tac` until step 9, took
  0.55, 0.05 of it in the interpreter itself.
- 21 Nov 2026: the preprocessor's loose ends, which group P left
  (cpp/README.md, "The preprocessor"). pcpp's evaluator read Python's
  `\N{name}` escape in a character constant in `#if`, so that
  `'\N{LATIN SMALL LETTER A}'` was 97, where c6502's lexer rejects the
  constant outside an `#if` as it rejects `'\q'`. That was fixed as a bug
  in Python (preprocessor.py's `_without_named_escapes`,
  tests/test_if_character_constants.py) and in C++, so the constant gets
  pcpp's report of the TypeError of `ord()`, as `'\q'` does. The
  rewriting matched pcpp's expansion without the `\N{name}` alternative
  on 200,000 random spellings, and the C++ matches Python on 34 recorded
  calls. pcpp follows Python's escapes in two other places, which were
  left alone. `\x` takes exactly two characters, and `\u` and `\U`
  take any four and eight, with the codec's error when they aren't
  hexadecimal digits.
  `--passthru-includes` compiles and matches its pattern with py_regex,
  now a port of Python 3.11's `re` parser and compiler with a matcher in
  `_sre`'s order, so an invalid pattern gets Python's exception word for
  word and a valid one Python's matches, case folding included. A
  pattern that uses what py_regex doesn't take (backreferences,
  conditionals, `\N{name}`, a class Python warns about, and a possessive
  repeat of a group, which Python 3.11.3 resumes from the wrong
  position after a failed iteration) gets a NotImplementedError that
  names it. `c6502cpp check-regex` found no difference on 27,177
  records of `tools/py_regex_records.py` (1,039,520 matches and 4,013
  errors, with 320 patterns unsupported), and the C++ matches Python on
  52 recorded calls of `preprocess` with the flag.
  The C++ writes `--debug`'s log, `pcpp_debug.log`, with pcpp's lines,
  the `repr` of each macro (which holds no memory address), Python's
  encoding and newlines, and the UnicodeEncodeError and KeyError that
  Python's log can raise. pcpp never closes the log, and preprocessor.py
  holds the preprocessor in a reference cycle, so what reached the disk
  depended on the garbage collector. The file held the whole log after
  `-S` of one C file and nothing after a link. That was fixed as a bug in Python
  (`CompilePreprocessor.run` closes the log), so each call leaves its
  whole log. The C++ matches Python's log on all 1,488 sources of the
  corpus and on 294 generated calls that reach each of pcpp's lines,
  and compile.py leaves the same bytes with and without the variable.
- 22 Nov 2026: the survey of step 7 and its groundwork ("Step 7").
  `tools/capture_binutils.py` records the Python assembler, ELF reader,
  script parser, linker, disassembler, nm.py, and objdump.py over the
  corpus and the suite, and `c6502cpp check-binutils` takes every
  record's JSON to C++ and back. The ELF reader and writer, the opcode
  table, the part of argparse nm.py and objdump.py use, and nm are
  ported (cpp/README.md, "The object-file tools"). The 6,283 objects
  and executables of the corpus's capture and the 330 of the suite's
  read and write again byte for byte, every `elf_read` record matches,
  errors included, and nm matches
  all 44,790 records, errors and argparse's messages included, in the
  builds of MSVC 19.36 and GCC 12.2. Under `C6502_CPP_BACKEND=1`,
  nm.py runs the C++ nm. The survey found four bugs, fixed in Python: a
  damaged ELF file, an input or a linker script that isn't there, a
  source that isn't UTF-8, and a `DC.B` string outside Latin-1 each gave
  a traceback, and each is now a one-line error. The C++ reader gives the
  first one's ElfError, and the others are in code that groups A and F
  port.
- 22 Nov 2026: group E of step 7, the runtime library (cpp/README.md,
  "The runtime library"). `tools/rtlib_table.py` writes the helpers'
  `asm_ast` functions and `DECIMAL_MODE_HELPERS` into
  cpp/generated/rtlib_helpers.cpp, which is compiled in, and
  tests/test_rtlib_table.py fails when the file differs from what the
  script writes. `passes::helper_footprints` gives Python's footprint
  for each of the 19 helpers, and the backend's request no longer
  carries the footprints, which took about 35 ms off a small switched
  `-S` (0.201 s to 0.167 s, best of 20) and about 32 ms off a process
  of 20 fuzz.stmts programs, which the 8 to 14 s such a run took on the
  loaded machine doesn't show. `tools/compare_backends.py` found no
  difference in 3,576 compiles. `needed`, `closure`, and
  `library_text`, the text `library_object` assembles, match Python
  on every `runtime_library` record of tools/capture_binutils.py,
  4,534 of the corpus and 101 of the suite, which take 182 distinct
  sets of helpers, in the builds of MSVC 19.36 and GCC 12.2. The
  modules have no raise site. The call of the
  assembler on the text is left to the linker's port, group D.
- 22 Nov 2026: group C of step 7, the linker scripts and the layout
  (cpp/README.md, "Linker scripts and the layout (group C)").
  `parse_script`, `read_script`, the five functions of the layout, and
  `fnmatch.fnmatchcase` are ported, on the interface group D builds on
  (937d078). The layout takes the linker's units, so
  tools/capture_binutils.py records its functions with what they read,
  and `c6502cpp check-binutils` matched all 15,141 records of the
  corpus and the 1,005 of the suite, errors included, in the builds of
  MSVC 19.36 and GCC 12.2. Tests give Python's message for each of the
  36 raise sites, the records reaching 16. `read_script` reads the
  locale's encoding as Python does, cp1252 here and UTF-8 on Linux. Five
  bugs were fixed in Python on the way, four of which ended compile.py
  with a traceback: a region's value computed every operator, so
  `0x800 + -1` raised the `<<`'s ValueError; a decimal number with a
  leading 0 raised ValueError; a region giving ORIGIN twice raised
  KeyError; a shift by a negative count in the layout raised
  ValueError; and an output section's `ALIGN(3)` gave an address no
  alignment gives.
- 22 Nov 2026: group B of step 7, the disassembler and objdump.py
  (cpp/README.md, "The object-file tools"). `c6502cpp check-binutils`
  matched all 22,225 `disassemble` records and 39,175 `objdump` records
  of the corpus, and the suite's 34 and 7, errors and argparse's
  messages included, in the builds of MSVC 19.36 and GCC 12.2. Under
  `C6502_CPP_BACKEND=1`, objdump.py runs `c6502cpp objdump`, and
  tests/test_cpp_binutils.py compares the two at four terminal widths.
  The suite passes with the variable set (4,914 tests, with
  `C6502_CHECK_IR=1`) and without it. The C++ nm named a damaged file
  by `str(Path(path))`, where Python's `E.read_file` passes the command
  line's string, so `./d.o` lost its `./`. That was fixed in nm, and
  objdump names the file the same way.
- 22 Nov 2026: group A of step 7, the parser of the link metadata and
  the assembler (cpp/README.md, "The object-file tools").
  `parse_metadata`, which the linker's groups build on, reads numbers as
  `int()` does (src/py_int.h), and tools/capture_binutils.py now records
  its calls. The assembler holds the text as code points and computes
  with checked 128-bit integers, where the widest value of the corpus
  takes 32 bits, and gives an error that names the limit past them.
  `c6502cpp assemble` assembles a request's text for group F. All 3,003
  `assemble` and 1,625 `parse_metadata` records of the corpus and the
  271 and 191 of the suite match, errors included, and each metadata
  block the assembler and the linker parse is written again by
  `format_metadata` as it was. The records reach 9 of the assembler's
  35 AssemblerError messages and 3 of `parse_metadata`'s 13 ValueError
  messages, and the C++ tests have a case for each of the others but
  the two no text reaches.
- 22 Nov 2026: group D1 of step 7, the linker's metadata side
  (cpp/README.md, "The linker's metadata side (group D1)"). The 17
  functions of passes/linker.py on the link metadata and
  `slot_sharing.merge_shares` and `check_linked_shares` are ported in
  src/passes/linker_metadata.* and slot_sharing.*, on the records,
  allocators, cycles, and layout that steps 3 and 4 ported.
  tools/capture_binutils.py records each call of each of them with what
  it reads, returns, and changes, and `c6502cpp check-binutils` matched
  all 15,491 records of the corpus and the 743 of the suite, errors
  included, in the builds of MSVC 19.36 and GCC 12.2. The records reach
  19 of the functions' 28 raise sites, and tests/test_linker_metadata.cpp
  gives Python's message for each of the 28, and the allocators'
  `ZpSlotAllocationError` and `ZpLocalAllocationError` for the
  synthetic program of a link. Two bugs were fixed in Python. A cycle
  of calls through two units, which neither unit's compile saw, failed
  the link in the slot allocator with a message that called it an
  internal error, or on an undefined `__local_*` slot, and
  `_validate_eligibility` now rejects it with a message of its own. A
  promoted zero-page object of no bytes, which only a metadata block
  written by hand lists, ended the link with a ZeroDivisionError, and
  `_merge_zp_objects` now rejects an object without bytes.
- 22 Nov 2026: group D2 of step 7, binutils/ld.py and the functions of
  passes/linker.py on the objects (cpp/README.md, "ld.py and the
  linker's objects"). `global_definitions`, the `Resolver`, `relax`,
  `relocate`, and `executable` are `binutils::ld`, and
  `_runtime_library`, which assembles group E's text with group A's
  assembler, `_object_items`, `_item_symbol`, `_without_init`, `_unit`,
  `_apply_renames`, `_pointer_names`, `_reassemble`, `_output_symbols`,
  `_merge_identical`, and `_contents` are in `passes::linker`, each with
  the Python function's inputs and result, for group D3's
  `link_objects`. The captures reach them only from inside a link,
  which changes its objects between the calls, so
  tools/capture_binutils.py records each call with its objects in a
  JSON form that keeps their symbols' order and identities, and the
  objects each call changes. `c6502cpp check-binutils` matched all
  69,428 such records of the corpus and the 2,200 of the suite, errors
  and `executable`'s bytes included, in the builds of MSVC 19.36 and
  GCC 12.2, except that GCC names the files of 68 `output_symbols`
  records of the suite, whose paths are Windows', as Python on Linux
  would. The records reach 3 of the 12 raise sites, and
  tests/test_ld.cpp and tests/test_linker_objects.cpp give Python's
  message for each of the 12. A bug was fixed in both compilers. An
  object with a relocation past the end
  of its section's bytes ended a link with an IndexError in
  `_pointer_names`, or gave the executable a section one byte longer
  than its size, and `elf.read` now rejects it with an ElfError.
- 22 Nov 2026: group D3 of step 7, the link (cpp/README.md, "The link
  (group D3)"). `link_objects` and `link_files` are
  `passes::linker::link_objects` and `link_files`, which run the
  functions of groups A, C, D1, D2, and E in Python's order on the
  objects read from their bytes, and `c6502cpp link` takes the objects'
  bytes and names, the script's path, the origin, and the `--defsym`s
  and gives the executable or the error with its Python class, for group
  F. `c6502cpp check-binutils` links every `link` record in C++, and all
  4,537 of the corpus and the 107 of the suite matched, executables byte
  for byte and errors word for word, in the build of MSVC 19.36, and in
  the build of GCC 12.2 all but 72 of the suite's, whose objects have
  Windows paths for names, which GCC names as Python on Linux would, as
  for D2's `output_symbols`. The records reach 2 of the 7 raise sites of
  `link_objects` and `link_files`, and tests/test_linker.cpp gives
  Python's message for each of the 7. The C++ link of Zork, Lode Runner,
  and Ali Baba takes 26, 34, and 75 ms, best of five, against Python's
  42, 63, and 130 ms on the same day (59, 88, and 175 ms in the survey).
  `allocate_function_locals` inverted the call graph for each function
  it placed, as Python does, which took 50 ms of Ali Baba's link in C++,
  and it now inverts it once.
- 22 Nov 2026: group F of step 7, the driver and the switch
  (cpp/README.md, "The back end through c6502cpp"). With
  `C6502_CPP_BACKEND=1`, `-c` and a link hand every input of the run to
  `c6502cpp backend` in one request (its modes `objects` and `link`,
  `cpp_backend.build`), as the plan for the switch describes, except
  that `-c` sends its inputs in one request too and a link goes through
  the backend rather than `c6502cpp link`, so that a link's C inputs
  compile in the process that links them. The C++ side compiles and
  assembles each C input, assembles each `.asm` input, reads each
  object, and returns the objects of `-c` or links them and writes the
  executable, and `compile._cpp_build` replays each input's stderr,
  warnings, and error in the Python path's order, so the messages, the
  exit status, and the files written are the same. The errors keep
  their classes (`AssemblerError`, `ElfError`, `LinkError`, and the
  allocators' errors, with `ScriptError` and `LayoutError` reaching
  compile.py as `LinkError`s as in Python), the `struct.error` of
  `elf.write`, which `c6502cpp assemble` typed "error" as `re.error`
  is, now has the type `struct.error` and leaves the file compile.py
  opened empty, and an input that can't be read gives compile.py's
  one-line message after the inputs before it. On these paths
  compile.py imports none of binutils/, `passes.linker` and its
  imports, the type checker, numpy, or rtlib, which
  tests/test_cpp_backend.py checks. `tools/compare_backends.py` now
  compiles the corpus with `-c` and `-c -O` too, links each object
  alone with and without a script, links the three games as
  tools/capture_binutils.py does and from their sources in one
  command, and links damaged objects and 21 bad command lines. It
  compared 7,156 jobs of 11,782 commands each way and found no
  difference in any status, message, exception, or file. The suite
  passes with the variable set (4,959 tests, with `C6502_CHECK_IR=1`)
  and without it, and the fuzzers with it set found nothing wrong in
  150 programs each of `exprs`, `calls`, and `stmts` from seed 3000.
  The builds of MSVC 19.36 and GCC 12.2 linked Zork from its 11 sources
  to the same bytes. With the variable unset and set, the best of three
  runs gave 0.61 and 0.13 seconds for the link of Zork from its objects,
  0.63 and 0.14 for Lode Runner, and 0.68 and 0.17 for Ali Baba (0.85 to
  1.02 and 0.51 to 0.59 in the survey); 21.0 and 4.9 seconds for `-c
  -O` of the three games' 37 units, one command per game; 0.62 and 0.11
  for the link of a one-function C program from its source with `-O`;
  and 4.25 and 0.90 for Zork's link from its sources. Of Ali Baba's
  0.17 seconds with the variable set, the c6502cpp process takes 0.07,
  Python's start takes 0.03, and the imports of argparse, json,
  subprocess, and tools/check_captures.py most of the rest; of its
  2.4 seconds of `-c -O`, Python takes about 0.1.
- 22 Nov 2026: step 7 is done. With `C6502_CPP_BACKEND=1`, no Python
  runs in `-S`, `-c`, a link, nm.py, or objdump.py but the reading of
  compile.py's command line and of its inputs, the replay of the
  response, and, under `C6502_CHECK_IR=1`, `ir_check`. These stay in
  Python until step 8: `--lex`, `--parse`, `--resolve`, and `--tac`;
  compile.py's argparse; `ir_check`; and tests/link_helpers.py's
  `linked_text`, which calls the disassembler. These stay until step
  9: the simulator, which reads executables with binutils.elf and
  decodes with `opcodes.DECODE` (sim/linked.py, sim/lode_runner.py,
  and sim/ali_baba.py), its own assembler (sim/assembler.py), the
  helpers' code in rtlib/helpers.py, which group E's table copies, and
  the TAC interpreter's path of the tests and the fuzzers.
- 23 Nov 2026: the survey of step 8 ("Step 8" above). The compiler's
  Python is 226 files of 102,029 lines, of which about 5,500 lines of
  data models and readers stay for step 9's simulator, `tac_sim`, and
  fuzzers, and the 4,959 tests of the suite are 1,002 end-to-end tests,
  3,804 that call a Python pass or helper directly, 118 of the Python
  implementation, and 35 of the models that stay. The proposed check
  after Python is a set of golden files of the corpus's outputs, which
  the C++ port gave in 68 seconds with 12 processes, 141 MB in all and
  5.9 MB with xz, kept as the `-S` and `-S -O` texts, the messages,
  and hashes of the objects and executables. `--lex` runs in C++ under
  `C6502_CPP_BACKEND=1` (3b58e2d): the backend's mode `lex` runs the
  preprocessor and `lexer::format_tokens`, and compile.py sends both
  `--lex` and `_run_stage("lex")` there. tools/compare_backends.py's
  new mode `lex` compared the 1,788 programs of the corpus, 24 of them
  rejected by the lexer, with no difference in any output, message, or
  file. The suite passes with the variable set (4,965 tests, with
  `C6502_CHECK_IR=1`) and without it (4,965 tests), and the build
  and the C++ tests pass with MSVC 19.36 and GCC 12.2.
- 23 Nov 2026: group G1 of step 8, the stages that print a tree.
  `asdl.py --cpp` generates a `pretty` for each of the four IRs, which
  writes pretty.py's text (`asdl::py_pretty`, and `c6502cpp roundtrip
  --pretty` for a dump), and it matches pretty.py on the four IRs of
  three programs, one of them with strings that need escapes
  (tests/test_cpp_backend.py, `TestPretty`). The backend's modes
  `parse`, `resolve`, and `tac` (src/passes/text_stages.h) run the
  front end to where `_run_stage` stops, and compile.py sends
  `--parse`, `--resolve`, and `--tac` and `_run_stage` for them through
  `cpp_backend.stage_source` under `C6502_CPP_BACKEND=1`, so that run
  as a script it imports none of the Python pipeline for any stage.
  tools/compare_backends.py's modes `parse`, `resolve`, `resolveO`,
  `tac`, and `tacO` ran the 1,788 programs of the corpus in each, 8,940
  commands, with no difference in any output, message, exit status, or
  file. The suite passes with the variable set (4,974 tests, with
  `C6502_CHECK_IR=1`) and without it (4,974 tests), and the build and
  the C++ tests pass with MSVC 19.36 and GCC 12.2.
- 23 Nov 2026: group G3 of step 8, `ir_check` in C++ (cpp/README.md,
  "The IR checks"). src/passes/ir_check.* has every check and message
  of passes/ir_check.py, and with the request's `check_ir` set, which
  cpp_backend.py sets from `C6502_CHECK_IR=1` or `ir_check.enable()`,
  the C++ pipeline runs each check where the Python pipeline runs it.
  A failed check throws `InternalError` with `IRCheckError`'s message,
  which reaches Python as an AssertionError. The response carries no
  dump for the checks any more, so under the switch compile.py run as
  a script imports no module of passes/ for any stage with
  `C6502_CHECK_IR=1` either. test_ir_check.py's 20 tests, the decimal
  region checks of test_bcd_builtins.py, and a case for each raise site
  are doctest cases with Python's messages (tests/ir_check_cases.h).
  With `C6502_CHECK_IR=1`, tools/compare_backends.py compared 7,156
  jobs of 11,782 commands and the 3,666 commands of its modes `tac` and
  `tacO` with no difference and no check raised. Three passes broken
  the same way in both compilers, one at a time, failed with the same
  message in both: ConstantFold dropping `x + 1` gave "after
  ConstantFold, function 'f': Ret reads %0.1, which nothing defines",
  dead_label_drop dropping the labels of `g` gave "after a peephole
  sweep, function 'g': branch to missing label .loop@0_break; jump to
  missing label .loop@0_continue" with and without `-O`, and
  `expand_decimal_arith` without its `CLD` gave "after asm_to_asm2,
  function 'add': Return at 10 runs with the decimal flag set (SED at
  2)" under `-O`. The suite passes with `C6502_CHECK_IR=1` with the
  variable set and without it (4,955 tests each, 41 skipped), the
  fuzzers with both set found nothing wrong in 150 programs each of
  `exprs`, `calls`, and `stmts` from seed 3000, and the build and the
  C++ tests pass with MSVC 19.36 and GCC 12.2. `-c -O` of Zork's 11
  units in one command, best of three, took 7.16 s in Python and 8.75
  s with the checks, and 1.33 s and 1.69 s through the C++ port.
- 23 Nov 2026: group G4 of step 8 ported tac_sim.py to C++
  (`tac_sim::Simulator`, cpp/README.md, "The TAC interpreter"), as
  Robert chose in place of a Python reader of the C++ tables. The
  backend's modes `tac-run`, `tac-observe`, and `tac-falls-off` serve
  `tests.test_sim_chapters._run_program` (with fuzz/runner.py's `tac`
  path), fuzz/reduce.py's `_tac_results` and `falls_off_end`, and
  fuzz/tac_passes.py's `first_changing_pass`, which under
  `C6502_CPP_BACKEND=1` import none of the Python front end,
  `c99_to_tac`, the TAC optimizer, or tac_sim.py. The TAC optimizer's
  C++ driver calls an observer after each step, as Python's
  `PhaseDriver.observer` is called. tools/compare_tac_sim.py ran the
  1,788 programs of the corpus through both interpreters, on the
  unoptimized TAC, on the optimized TAC, and with the observer, with
  no difference, and the observer found no pass that changes a result.
  Before two fixes it found five, all from limits of the interpreter,
  which both interpreters shared: a Phi or SSA destruction's copy of a
  variable without a value (`int c;` before a loop that assigns it)
  raised "uninitialized var", and an object that another unit defines
  (`extern struct s x;`) had no address for the stores by name that
  `direct_member_access` makes. Both are fixed in Python and C++. With
  narrow_widened_arith's check of a global broken in C++, `tac-observe`
  named the pass and the function, and fuzz/reduce.py `--tac-opt`
  reduced a program under the switch. After the merge of G1 and G3,
  the TAC modes check the IR in the C++ driver with the request's
  `check_ir`, and tools/compare_tac_sim.py with `C6502_CHECK_IR=1`
  compared the corpus again in 303 s with 14 processes, with no
  difference and no change by a pass. On an idle machine, with
  `-j 10`, the fuzzers (seeds 3000 to 3149 of each) took 6, 6, and 7 s
  with the switch and Python's TAC path (at ae37758) and 4, 5, and 6 s
  with the C++ path, none wrong. A program's TAC path took 10.7, 16.9,
  and 23.0 ms in Python (exprs, calls, stmts) and 11.8, 12.5, and
  14.0 ms in C++, of which about 7 ms is starting c6502cpp. The suite
  passes with `C6502_CHECK_IR=1` with the variable set (4,982 tests, 23
  skipped) and without it (4,983, 22 skipped), and the build and the
  C++ tests (3,352 cases) pass with MSVC 19.36 and GCC 12.2. The C++
  interpreter runs a recursion of up to 100,000 calls, where Python's
  raises RecursionError at about 330.
- 24 Nov 2026: group G5 of step 8, the checks after Python
  (cpp/README.md, "The checks after Python"). tools/write_golden.py
  writes the golden files of compare_backends.py's jobs with the
  Python compiler, in the modes `O`, `unopt`, `cO`, `c`, `lex`,
  `parse`, `resolve`, `resolveO`, `tac`, and `tacO`, the `links` jobs,
  and a mode `tacsim` of the TAC interpreter's results, which Robert
  left to G5 to decide on and which is the only check of the C++
  interpreter on the corpus once tac_sim.py has no TAC to run. They
  are 19,672 jobs of 24,298 commands in 38 MB under golden/, with the
  `-S` texts as files of their own. `c6502cpp check-golden` runs every
  command line as compile.py would under `C6502_CPP_BACKEND=1`, with
  no Python, and matched every job, in 60 s with 12 processes with
  MSVC 19.36 and in 40 s with GCC 12.2 in WSL. tools/freeze_captures.py
  froze 10,043 calls of 194 functions of tools/capture_passes.py's
  captures and 2,113 binutils records of 47 kinds, 108 MB raw and 3.2
  MB with xz, which `c6502cpp check-frozen` reads with XZ Embedded
  (vendored in cpp/third_party/xz-embedded) and checks with
  check-captures and check-binutils, matching every one in 28 s with
  MSVC and 4 s with GCC. That is more than the survey's 12 MB and 1
  MB, mostly from the calls that carry whole symbol tables, and of the
  raising calls the freezer keeps one for each form of message, which
  took the binutils archives from 3.9 MB to 0.4 MB with xz. With
  `split_mem_to_mem` and then label_resolution broken on purpose in
  C++, check-golden reported 415 and 36 jobs and check-frozen 3 and 1
  calls, each under the pass's name. tests/test_golden.py runs both.
- 24 Nov 2026: group G2 of step 8, compile.py's command line
  (cpp/README.md, "compile.py's command line"). src/argparse.* has the
  rest of what compile.py's parser uses: options that store a value
  with a `metavar` and `type=`, `store_const` options in a mutually
  exclusive group with its usage and its error, `append`, and
  `parse_known_args`, with textwrap's splitting of words at their
  hyphens in the help. `c6502cpp compile` (src/compile_command.*) reads
  compile.py's whole command line with it, after `_rewrite_ttext`, and
  does what `main` does under the switch in one process, with every
  message saying `compile.py:`. Under `C6502_CPP_BACKEND=1` compile.py's
  `main` runs it through `cpp_backend.run_tool`, in frames that give
  each write to stdout and stderr in order, ask Python for stdin, and
  end with the status, a SystemExit, or the exception `main` would
  raise. `_run_stage` and `_compile_c` still send one request each.
  `check-golden` runs every job's command lines through the command's
  code in place of G5's reader. tools/compare_backends.py's new corpus
  `cli` runs 83 command lines at six terminal widths (the help, each
  pair of stage flags, each way `--origin` is rejected, each option
  without its value, flags run together, compile.py's own checks, and
  links with each spelling of `-Ttext`), and golden/cli.txt holds them.
  The `cli` jobs found one difference, which is fixed (cd49dd2): a link
  from `-Ttext=-1` gave Python's `struct.error` and a C++ executable,
  since `check_write_fields` didn't check the addresses a link places
  from its origin. compare_backends.py compared 7,162 jobs of 12,280
  commands, and 10,728 commands of the modes `lex` to `tacO`, with no
  difference, and `check-golden` matched 19,678 jobs of 24,796 commands
  and `check-frozen` every call. The suite passes with
  `C6502_CHECK_IR=1` with the variable set (4,991 tests, 23 skipped)
  and without it (4,992, 22 skipped), and the build and the C++ tests
  (3,359 cases) pass with MSVC 19.36 and GCC 12.2. `compile.py a.c -S`
  of a one-line program under the switch took 120 ms before and 92 ms
  after (the median of 30 runs, with a build running beside them), once
  compile.py stopped importing diagnostics.py
  (and so dataclasses) and cpp_backend.py stopped importing shutil and
  tools/check_captures.py with its argparse before the early exit. Of
  the rest, Python's own start takes about 32 ms, compiling compile.py's
  source (a script has no .pyc) about 12 ms, importing subprocess (with
  locale and re) about 30 ms, and `c6502cpp compile` itself about 13
  ms.
- 24 Nov 2026: group G6 of step 8, its first part ("G6's modules", and
  cpp/README.md, "The unit tests' recorded calls"). tools/record_test_calls.py
  ran G6's 117 test modules and recorded each call that their 1,492 (b)
  tests made of an asm pass, a stage call of code generation, or
  emission. 1,219 tests are removed, 50 modules entirely, and their calls
  are 827 new doctest cases in 78 files, 539 cases that steps 3 to 5 had
  recorded from the same tests, and 4 cases written by hand for
  test_bin_to_bcd.py's IR checks. Of the 273 tests left, 28 are end to
  end and stay, and 245 in 32 modules call functions that have no C++
  runner yet, such as `may_alias`, `translate_instruction`,
  `parse_script`, and `replace_function`, and are left for a second
  agent. tests/link_helpers.py's `linked_text` reads executables with
  `c6502cpp objdump`. The suite passes with `C6502_CHECK_IR=1` with the
  variable set (3,772 tests, 23 skipped) and without it (3,773, 22
  skipped), where it ran 4,991 and 4,992 before. The C++ tests (4,190
  cases) pass in fresh builds with MSVC 19.36 and GCC 12.2, and
  `check-golden` and `check-frozen` match every job and record.
