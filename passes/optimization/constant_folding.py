"""TAC constant folding pass.

Replaces operations whose operands are all `Constant` with the
already-evaluated result, lowering the residual instruction to a
`Copy` of the folded constant into the original destination.

Currently a stub — returns its input unchanged. The actual folding
will land in a follow-up: per-op evaluation for `Unary` / `Binary`,
honoring the TAC `const` width set (ConstInt 1B / ConstLong 2B /
ConstLongLong 4B / ConstFloat / ConstDouble) and matching the c6502
truncation semantics used by `tac_to_asm`.
"""

from __future__ import annotations

import tac_ast


def constant_fold(fn: tac_ast.Function) -> tac_ast.Function:
    return fn
