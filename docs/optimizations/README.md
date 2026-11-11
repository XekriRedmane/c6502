# Optimizations

Each file in this directory describes one optimization that c6502
performs. A file says what the optimization does and why it's cheaper
on the 6502, shows a small example before and after, and gives the
conditions under which it applies and the cases it leaves alone. For
the ideas behind the optimizer as a whole (SSA form, the fixed-point
loops, register allocation), read
[../optimization.md](../optimization.md) first.

Most listings were captured from a compile by wrapping the one pass
the file describes and printing its input and output. The rest
compare two complete compiles, or come from the pass's unit test, and
their files say so. Lines the pass doesn't touch are sometimes left
out, and `...` marks the place. Asm after pseudoregister replacement is shown as the
6502 text the compiler emits. TAC, and asm from before pseudoregister
replacement, are shown one IR instruction per line, as the
instruction's constructor with its fields in order. Operators are
written as their C symbols, except that unary minus is `neg`. A
constant carries a suffix for its type, which is `uc` for `unsigned
char`, `sc` for `signed char`, `c` for `char`, nothing for `int`, `u`
for `unsigned int`, `L` and `UL` for `long` and `unsigned long`, `LL`
and `ULL` for the `long long` types, and `f` and `d` for `float` and
`double`. A name such as `%3.1` is a
compiler temporary, and a name such as `@0.i.2` is the SSA version of
a user variable.

## Alphabetical list

- [Address of a zero-page local](lower_data_load_address.md). Splits a fixed-address LoadAddress into two immediate stores so peepholes can fold them.
- [Address-taken locals in zero page](address_taken_zp.md). Puts address-taken locals in free private-pool bytes instead of the frame.
- [AND jump fold](and_zero_jump_fold.md). Turns `if (byte & mask)` into a one-byte `JumpIfMasked` test with no temporaries.
- [Asm copy propagation](asm_copy_propagation.md). Replaces reads of a byte copied by a direct Mov with its source.
- [Asm dead-store elimination](asm_dead_store.md). Drops stores to memory that no reachable instruction reads before overwrite or return.
- [Asm loop-invariant store hoisting](asm_licm.md). Moves loop-invariant constant and copy stores in front of the loop.
- [Backward copy propagation](backward_copy_propagation.md). Makes a single-use temporary's definition store straight to the copy's destination.
- [Branch inversion](branch_invert.md). Turns a branch over a JMP into one branch on the opposite condition.
- [Branch on INC or DEC flags](dec_inc_branch_fold.md). Deletes `LDA M` between `INC`/`DEC M` and a branch on N or Z.
- [Branch through jump](branch_through_jump.md). Points a branch straight past a label that holds only a JMP.
- [Branch to next label](branch_to_next_drop.md). Deletes a branch or JMP whose target is the label right after it.
- [Branching directly on `&&` and `||`](short_circuit_jump_fold.md). Removes the 0-or-1 value of `&&` and `||` and branches straight to the target.
- [Byte-granular dead-code elimination](byte_dce.md). Drops each Mov or Phi that writes a temporary byte nothing reads.
- [Byte-granular register allocation](regalloc.md). Puts each SSA byte value in a zero-page byte, X, or Y.
- [Carry INC](carry_inc.md). Turns in-place `ADC #0` carry bytes into BCC/INC chains and `SBC #0` into BCS/DEC.
- [Carry-flag return peepholes](carry_return.md). Turns carry-flag returns into SEC/CLC and carry captures into BCC/BCS.
- [CMP and SBC fusion](cmp_sbc_fusion.md). Moves `SEC; SBC N` ahead of a branch so it replaces `CMP N`.
- [CMP for a discarded volatile read](volatile_void_read_cmp.md). Turns a discarded volatile pointer load into CMP, so A keeps its value.
- [Combining added constants](reassoc_const.md). Rewrites `(v + C2) + C1` as `v + (C1+C2)`, removing one add.
- [Common subexpression elimination](cse.md). Replaces a repeated pure computation or load with a Copy of the first.
- [Commuted ADC, AND, and ORA](adc_commute.md). Reads memory directly in ADC, AND, or ORA instead of reloading it after a spill.
- [Compare without a staging temporary](cmp_through_tmp_fold.md). Rewrites `LDA src; STA tmp; LDA other; CMP tmp` as `LDA other; CMP src`.
- [Comparison and jump fold](cmp_zero_jump_fold.md). Fuses a comparison and the jump on its result into one conditional jump.
- [Const-static fold](const_static_fold.md). Replaces reads of a static const scalar with immediates and drops its storage.
- [Constant AND and ORA fold](const_arith_fold.md). Folds `LDA #k` with `AND`/`ORA #k`, and deletes `ORA #0` and `AND #$FF`.
- [Constant folding](constant_folding.md). Evaluates operations, casts, jumps, and const-array reads whose operands are all constants.
- [Copy folding](copy_folding.md). Makes the instruction that writes a single-use temporary write the Copy's destination directly.
- [Copy propagation](copy_propagation.md). Replaces each read of an SSA Copy's destination with its source.
- [Count-down counter in X or Y](counter_to_index_reg.md). Moves a counter used only by its DEC into X or Y.
- [CPX and CPY](cpx_cpy_peephole.md). Rewrites `TXA; CMP` or `TYA; CMP` as CPX or CPY when A is dead.
- [Dead A arithmetic](dead_a_arith.md). Deletes instructions that write only A and flags when neither is read later.
- [Dead label drop](dead_label_drop.md). Deletes labels that no jump, branch, or Phi argument names.
- [Dead loop elimination](dead_loop_elimination.md). Deletes a side-effect-free loop whose values nothing reads afterward.
- [Dead register-parameter entry stub](dead_reg_entry_stub.md). Drops a register parameter's entry store when its slot is never read.
- [Dead static elimination](dead_static.md). Drops internal-linkage static objects that no instruction or initializer references.
- [Dead-store elimination](dead_store_elimination.md). Drops TAC instructions with unread SSA results, keeping calls but dropping their results.
- [DEC](dec_peephole.md). Rewrites an in-place 1-byte subtract of 1 as DEC, DEX, or DEY.
- [Default zp_abi calling convention](zp_abi.md). Passes eligible functions' arguments in fixed zero-page bytes with no frame.
- [DEX and DEY for a register counter](dec_register_peephole.md). Turns `TYA; SEC; SBC #1; STA; LDY` on an X or Y counter into DEX or DEY.
- [Direct load into X or Y](direct_index_load.md). Fuses `LDA M; TAX` or `TAY` into LDX or LDY when A is dead.
- [Direct member access](direct_member_access.md). Turns a load or store through `&obj + k` into direct byte access.
- [Dispatch on a small pointer table](dispatch_pointer_array.md). Rewrites `arr[i][j]` on a small pointer table as a compare-and-branch on `i`.
- [Indexed load from a fixed address](recognize_indexed_load.md). Turns a byte load through a constant address plus a byte index into `LDA C,X`.
- [Indexed store to a fixed address](recognize_indexed_store.md). Turns a byte store through a constant address plus a byte index into `STA C,X`.
- [Indirect-indexed access through a pointer](recognize_indirect_indexed.md). Turns a byte access through a pointer plus a byte index or constant into `(zp),Y`.
- [INY and CPY for an incremented table byte](index_inc_cmp_y.md). Does table-byte plus or minus one and its compare in Y instead of a temporary.
- [Load after read-modify-write](redundant_load_after_rmw.md). Deletes an unused load right after INC, DEC, or a shift of the same byte.
- [Logical-not jump fold](lnot_jump_fold.md). Removes a `!` used only by a jump and flips the jump's sense.
- [Loop counter in X](loop_counter_to_x.md). Moves an index loop counter into X, moving other index loads to Y.
- [Loop unrolling](unroll.md). Replaces a pragma-marked counted for loop with one body copy per iteration.
- [Loop-carried value in A](promote_loop_carried_to_a.md). Keeps an incremented register parameter in A across its loop.
- [Loop-invariant compare key in A](loop_invariant_a.md). Keeps a loop's compare key in A and swaps the compare operands.
- [Loop-invariant extends and truncates](licm.md). Moves loop-invariant extends and truncates to the loop's preheader.
- [Memory constant propagation](mem_const_prop.md). Replaces reads of bytes known to hold a constant in a block with immediates.
- [Memory value propagation](memory_value_propagation.md). Rewrites reads of staged zero-page bytes, and `(DPTR),Y`, to their original source.
- [Memory-to-memory move splitting](split_mem_to_mem.md). Splits memory-to-memory moves into LDA and STA atoms that other peepholes rewrite.
- [Move coalescing](coalescing.md). Merges copy-joined values in the interference graph so both share one byte.
- [Moving a constant out of an offset](reassoc_offset.md). Rewrites `p + (x + k)` as `(p + x) + k` so subscripts share `p + x`.
- [Multi-byte INC](inc_peephole.md). Rewrites an in-place add of 1 to memory as an INC and BNE chain.
- [Narrowing widened arithmetic](narrow_widened_arith.md). Does promoted byte arithmetic at one byte when only the low byte is used.
- [OR-with-zero absorb](or_zero_absorb.md). Rewrites `LDA #0; ORA x` as `LDA x` before move coalescing.
- [PHA and PLA removal](dead_pha_pla.md). Drops a PHA/PLA pair around instructions that leave A untouched.
- [Private local pools](zp_local_pools.md). Gives each eligible function body-local zero-page bytes that no coexisting function uses.
- [Prologue synthesis](prologue_synthesis.md). Adds a soft-stack prologue and epilogue only to functions that need a frame.
- [Reads of X's save slot](x_save_slot_load.md). Rewrites loads of X's spill byte as TXA or STX.
- [Redundant compare](redundant_compare.md). Deletes a compare reached only by a branch after an identical compare.
- [Redundant LDY setup](y_peephole.md). Drops `LDY #n`, or makes it INY or DEY, when Y's value is known.
- [Redundant load elimination](redundant_load.md). Drops loads into A, X, or Y when the register already holds the value.
- [Redundant store elimination](redundant_store.md). Drops a repeated `LDA S; STA D` copy when D still holds S.
- [Rematerialization of staged values](asm_remat.md). Reloads a staged value from its source so the staging-slot store dies.
- [Round-trip load drop](round_trip_load.md). Deletes `LDA M` after `STA M` when the flags already reflect A.
- [Second index in Y](dual_index_promotion.md). Loads a byte reloaded into X several times into Y once at entry.
- [Self-store drop](self_store_drop.md). Deletes the `STA M` of an adjacent `LDA M; STA M` pair.
- [Sign-bit test without AND](and_sign_bit_branch.md). Turns `LDA M; AND #$80; BEQ/BNE` into `LDA M; BPL/BMI`.
- [Signed countdown loop rotation](loop_rotate.md). Moves the `i >= 0` test of a signed countdown for loop to the bottom.
- [Sinking an AND past a bit test](sink_and_past_branch.md). Moves a byte AND past a bit-7 test into both successor blocks.
- [Sinking increments](sink_increment.md). Moves `Y = X ± c` past X's last read so X and Y share a register.
- [Static const reads](static_const_fold.md). Replaces reads of static const scalars, including fixed-address const pointers, with their values.
- [Store X or Y directly](via_a_store_fold.md). Turns `TXA; STA M` into `STX M` (and TYA into STY) when A is dead.
- [Strength reduction](strength_reduction.md). Rewrites multiply, divide, and modulo by constants as shifts, masks, adds, or copies.
- [Subtract-and-test fusion](sub1_test_zero_peephole.md). Drops `LDA #0; CMP i` after `SBC #1` and branches on the inverted carry.
- [Tail call](tail_call.md). Turns `JSR f; RTS` into `JMP f` in frameless functions.
- [Tail merge](tail_merge.md). Branches to one shared copy of a repeated return sequence.
- [Transfer plus or minus one](transfer_pm1_store.md). Turns `TXA; SEC; SBC #1; STA M` into `DEX; STX M`, and the +1 and Y forms.
- [Truncate of an extend](truncate_extend_fold.md). Rewrites a truncate of an extend to read the extend's source directly.
- [TXA past a branch](transfer_past_branch.md). Moves TXA or TYA after a branch when only the fall-through reads A.
- [Unreachable code drop](unreachable_drop.md). Deletes unlabeled instructions after a jump or return that no path reaches.
- [Unreachable code elimination](unreachable_code_elimination.md). Removes unreachable TAC blocks, then the stale Phi arguments, jumps, and labels.
- [Y known zero after a DEY loop](dec_branch_indirect_y_fold.md). Drops `LDY #$00` before `(DPTR),Y` after a `DEY; BNE` fall-through.

## Pipeline order

`compile.py --codegen --optimize` applies the optimizations in the
order below. Steps that aren't optimizations are listed in plain text
without a link, so that each optimization's neighbors can be seen. A
group marked *fixed point* runs every member in order, then repeats
the whole group until a sweep leaves the code unchanged.

This list is copied from `compile.optimized_asm`,
`compile._peephole_fixedpoint`, `_DRIVER` in
`passes/optimization/optimizer.py`, and `_optimize_function` in
`passes/optimization_asm/optimizer.py`. It has to be checked against
them whenever a pass is added or moved.

1. Parse. Then, on the AST:
   1. [Loop unrolling](unroll.md)
   2. Identifier resolution, string lifting, label resolution, loop
      labeling, type checking, and translation to TAC.
2. TAC optimizer, once per function:
   1. [Signed countdown loop rotation](loop_rotate.md)
   2. SSA construction.
   3. [Static const reads](static_const_fold.md)
   4. *Fixed point*:
      1. [Constant folding](constant_folding.md)
      2. [Strength reduction](strength_reduction.md)
      3. The three jump folds, run as one rule set. They are the
         [comparison and jump fold](cmp_zero_jump_fold.md), the
         [AND jump fold](and_zero_jump_fold.md), and the
         [logical-not jump fold](lnot_jump_fold.md).
      4. [Dead loop elimination](dead_loop_elimination.md)
      5. [Unreachable code elimination](unreachable_code_elimination.md)
      6. [Common subexpression elimination](cse.md)
      7. [Loop-invariant extends and truncates](licm.md)
      8. [Copy propagation](copy_propagation.md)
      9. [Dead-store elimination](dead_store_elimination.md)
      10. [Copy folding](copy_folding.md)
      11. [Combining added constants](reassoc_const.md)
      12. [Moving a constant out of an offset](reassoc_offset.md)
      13. [Direct member access](direct_member_access.md)
      14. [Indexed store to a fixed address](recognize_indexed_store.md)
      15. [Indexed load from a fixed address](recognize_indexed_load.md)
      16. [Truncate of an extend](truncate_extend_fold.md)
      17. [Sinking increments](sink_increment.md)
      18. [Sinking an AND past a bit test](sink_and_past_branch.md)
      19. [Narrowing widened arithmetic](narrow_widened_arith.md)
   5. [Indirect-indexed access through a pointer](recognize_indirect_indexed.md)
   6. SSA destruction.
   7. [Copy folding](copy_folding.md), once more.
   8. *Fixed point*: [branching directly on `&&` and
      `||`](short_circuit_jump_fold.md)
3. [Dispatch on a small pointer table](dispatch_pointer_array.md),
   over the whole program.
4. [Default zp_abi calling convention](zp_abi.md): ABI selection and
   parameter slot allocation.
5. Translation to asm.
6. Asm SSA optimizer, run once to measure each function's local
   bytes. Its output is discarded. The steps are the same as in step
   8.
7. [Private local pools](zp_local_pools.md), sized from step 6 plus
   the bytes of [address-taken locals in zero
   page](address_taken_zp.md).
8. Asm SSA optimizer:
   1. [Const-static fold](const_static_fold.md), over the whole
      program.
   2. Per function:
      1. [Subtract-and-test fusion](sub1_test_zero_peephole.md)
      2. SSA construction.
      3. *Fixed point*:
         1. [Asm copy propagation](asm_copy_propagation.md)
         2. [Backward copy propagation](backward_copy_propagation.md)
         3. [Byte-granular dead-code elimination](byte_dce.md)
         4. [OR-with-zero absorb](or_zero_absorb.md)
      4. Liveness and the interference graph.
      5. [Move coalescing](coalescing.md)
      6. [Byte-granular register allocation](regalloc.md), which
         also applies the coloring.
      7. SSA destruction.
   3. [Dead static elimination](dead_static.md), over the whole
      program.
9. Placement of [address-taken locals in zero
   page](address_taken_zp.md), then pseudoregister replacement.
10. [Address of a zero-page local](lower_data_load_address.md)
11. [Prologue synthesis](prologue_synthesis.md)
12. [Asm loop-invariant store hoisting](asm_licm.md)
13. Peephole fixed point, first run. See below for its members.
14. [Loop-carried value in A](promote_loop_carried_to_a.md)
15. [Loop counter in X](loop_counter_to_x.md)
16. [Count-down counter in X or Y](counter_to_index_reg.md)
17. [Reads of X's save slot](x_save_slot_load.md)
18. Peephole fixed point, second run.
19. [Second index in Y](dual_index_promotion.md)
20. Peephole fixed point, third run.
21. [Dead register-parameter entry stub](dead_reg_entry_stub.md)
22. [Loop-invariant compare key in A](loop_invariant_a.md)
23. Long-branch expansion, zero-page global placement, and lowering
    to `asm2_ast`.
24. Emit, followed by [redundant LDY setup](y_peephole.md) on the
    emitted text.

The peephole fixed point (`compile._peephole_fixedpoint`) runs these,
in order, as one *fixed point*. Its loop is capped at 16 sweeps, and
reaching the cap raises an error.

1. [Memory-to-memory move splitting](split_mem_to_mem.md)
2. [Multi-byte INC](inc_peephole.md)
3. [Carry INC](carry_inc.md)
4. [DEC](dec_peephole.md)
5. [Subtract-and-test fusion](sub1_test_zero_peephole.md)
6. [Direct load into X or Y](direct_index_load.md)
7. [PHA and PLA removal](dead_pha_pla.md)
8. [CPX and CPY](cpx_cpy_peephole.md)
9. [Memory value propagation](memory_value_propagation.md)
10. [Load after read-modify-write](redundant_load_after_rmw.md)
11. [Redundant load elimination](redundant_load.md)
12. [Redundant store elimination](redundant_store.md)
13. [Rematerialization of staged values](asm_remat.md)
14. [Asm dead-store elimination](asm_dead_store.md)
15. [Dead A arithmetic](dead_a_arith.md)
16. [Dead label drop](dead_label_drop.md)
17. [Unreachable code drop](unreachable_drop.md)
18. [Branch inversion](branch_invert.md)
19. [Branch to next label](branch_to_next_drop.md)
20. [Branch through jump](branch_through_jump.md)
21. [DEX and DEY for a register counter](dec_register_peephole.md)
22. [TXA past a branch](transfer_past_branch.md)
23. [INY and CPY for an incremented table byte](index_inc_cmp_y.md)
24. [CMP for a discarded volatile read](volatile_void_read_cmp.md)
25. [Y known zero after a DEY loop](dec_branch_indirect_y_fold.md)
26. [Memory constant propagation](mem_const_prop.md)
27. [Constant AND and ORA fold](const_arith_fold.md)
28. [Carry-flag return peepholes](carry_return.md), which are three
    passes run back to back.
29. [Round-trip load drop](round_trip_load.md)
30. [Sign-bit test without AND](and_sign_bit_branch.md)
31. [Self-store drop](self_store_drop.md)
32. [Redundant compare](redundant_compare.md)
33. [Compare without a staging temporary](cmp_through_tmp_fold.md)
34. [Commuted ADC, AND, and ORA](adc_commute.md)
35. [CMP and SBC fusion](cmp_sbc_fusion.md)
36. [Branch on INC or DEC flags](dec_inc_branch_fold.md)
37. [Tail call](tail_call.md)
38. [Tail merge](tail_merge.md)
39. [Store X or Y directly](via_a_store_fold.md)
40. [Transfer plus or minus one](transfer_pm1_store.md)

### Without `--optimize`

`compile.py --codegen` without `--optimize` runs none of the TAC or
asm SSA optimizations. It translates to asm, replaces
pseudoregisters, and runs the peephole fixed point above once, with
every member. It then expands long branches, places zero-page
globals, emits, and runs [redundant LDY setup](y_peephole.md) on the
text. Many peepholes find nothing to rewrite there, because every
temporary lives on the soft stack. Each peephole's file says whether
it fires in the unoptimized pipeline.
