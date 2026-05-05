"""AST-level optimization passes.

Distinct from `passes.optimization` (which operates on TAC) and
`passes.optimization_asm` (which operates on asm IR with Pseudos):
this package transforms the c99 AST itself.

Currently houses a single pass: `unroll`, which fully unrolls
`for` loops carrying `#pragma c6502 loop unroll(enable)`. AST-
level unrolling sidesteps the per-iteration label / induction-
variable renaming that a TAC-level fold would need to redo —
identifier_resolution and loop_labeling run AFTER unroll, so
each cloned body picks up its own per-iteration names from those
passes naturally.
"""
