"""Asm-level optimization passes.

Mirror of `passes.optimization` for the `--optimize-asm` pipeline:
SSA construction → fixed-point opts → byte-granular regalloc →
SSA destruction. Operates on `asm_ast.Program` with `Pseudo` operands
(so it runs BEFORE `replace_pseudoregisters`, while operand
locations are still virtual).

Why a separate package: TAC SSA renames whole multi-byte values
(an Int / Long / Pointer is one variable). Asm-level SSA renames
each (Pseudo name, byte offset) pair INDEPENDENTLY, which is what
exposes byte-granular optimizations like high-byte DCE on values
that constant-folded to fit in a byte. The two SSA layers also
sit on different IRs, so reusing the TAC modules would mean
parameterizing every node-typed match arm, a worse split.
"""
