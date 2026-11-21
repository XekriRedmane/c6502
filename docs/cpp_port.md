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
- The TAC CFG's dominance frontiers and natural loop bodies, and the set
  of reachable blocks `natural_loops` walks, are Python sets of block
  ids, and SSA construction and the loop passes walk them. Their order
  is the order of CPython's hash table, which `PyIntSet`
  (`cpp/src/py_int_set.h`) reproduces.

## Step 5: the TAC optimizer, c99_to_tac, and the inliner

Step 5 is the part of a compile between the type checker and the TAC
that `C6502_CPP_BACKEND=1` hands to C++: `c99_to_tac`, the passes of
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
