# What an asm operand's lowering writes

- An asm instruction with a `Frame`, `Stack`, `Indirect`, or `IndirectZp` operand is emitted with an `LDY #off` setup before it, so it writes Y and sets N/Z from the offset, even for a plain `STA`. A shift, `INC`, or `DEC` of a `Frame` or `Stack` operand also goes through A.
- A pass that tracks A, Y, or the flags across instructions has to count these writes. `asm_liveness::sets_up_y` and `asm_liveness::sets_flags` (cpp/src/passes/asm_liveness.h) do, so use them in place of reading the instruction's own opcode.
- At least six miscompiles came from missing this: `redundant_load` (fuzz.stmts seed 2391, and again for Frame shifts), `asm_liveness.sets_flags` itself, `asm_remat` and `remat` reusing a load past a Y setup, and `and_sign_bit_branch` twice, where a `BPL` tested the offset in place of A.
- `promote_loop_carried_to_a` parks a value in Y across a loop's preamble, and until 26 Nov 2026 it checked the preamble only for a `Reg(Y)` operand, so a `JSR` or a `Frame` load there overwrote the parked value. A check that a span of code leaves Y alone needs `reads_y`, `kills_y` (which counts a call that doesn't preserve Y), and `sets_up_y`.
- `for_each_operand<false>` (cpp/src/passes/asm_operands.h) skips a `DecimalArith`'s operand lists and a `Phi`'s sources. A pass that asks whether anything names a slot needs `for_each_operand<true>`, or it misses a packed BCD add of the slot.
- Check any new asm pass, and any port of one, for these writes before trusting it on the corpus.

Related: [[principles/prove-it-works]], [[principles/fix-root-causes]]
