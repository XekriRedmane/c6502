"""TAC SSA destruction.

Converts every `Phi` instruction back to `Copy` instructions at the
end of each predecessor block. After this pass no Phi remains in
the function and the result is regular non-SSA TAC, ready for
`tac_to_asm` (which has no Phi lowering).

For each Phi in block M:
    Phi(dst, args=[(pred_label_1, src_1), ..., (pred_label_n, src_n)])
emit one `Copy(src_k, dst)` at the end of pred_k's block, immediately
before its terminator (`Ret` / `Jump` / `JumpIfTrue` / `JumpIfFalse`)
or at the end of the instruction list if there is no terminator.

The classic parallel-copy "swap" / "lost copy" problem doesn't arise
from `c99_to_tac` output run through `to_ssa`. The renaming pass
only ever pushes a name of the form `<orig>.<n>` (or `<orig>`
itself, for a parameter's initial name) onto `stack[<orig>]`, so a
Phi source for original var `v` always names a value of `v` —
never the dst of a different-original-var Phi at the same block.
That means emitting Copies in source order can't read a value
that's already been overwritten by a sibling Copy in the same
predecessor.

If a downstream pass introduces Phis that violate that invariant
(register allocation, value coalescing, ...), we'd need the standard
parallel-copy expansion (capture each Phi source into a fresh temp,
then write all temps to dsts). Not needed for Milestone 1.
"""

from __future__ import annotations

from collections import defaultdict

import tac_ast
from passes.optimization.cfg import (
    BasicBlock,
    build_cfg,
    cfg_to_function,
)


_TERMINATOR_TYPES: tuple[type, ...] = (
    tac_ast.Ret,
    tac_ast.Jump,
    tac_ast.JumpIfTrue,
    tac_ast.JumpIfFalse,
)


def from_ssa(fn: tac_ast.Function) -> tac_ast.Function:
    """Lower every Phi to Copies in predecessor blocks; remove all
    Phis from the function. Returns the rewritten Function."""
    cfg = build_cfg(fn)
    label_to_block: dict[str, BasicBlock] = {
        b.instructions[0].name: b
        for b in cfg.blocks.values()
        if b.instructions and isinstance(b.instructions[0], tac_ast.Label)
    }

    for bid in cfg.block_order:
        blk = cfg.blocks[bid]
        phis = [i for i in blk.instructions if isinstance(i, tac_ast.Phi)]
        if not phis:
            continue
        # One bucket of Copies per predecessor label.
        per_pred: dict[str, list[tac_ast.Copy]] = defaultdict(list)
        for phi in phis:
            for arg in phi.args:
                per_pred[arg.pred_label].append(
                    tac_ast.Copy(src=arg.source, dst=phi.dst),
                )
        for pred_label, copies in per_pred.items():
            pred_blk = label_to_block.get(pred_label)
            if pred_blk is None:
                # Predecessor block was removed (e.g., by an earlier
                # pass dropping unreachable code); its Phi argument
                # is dead. Skip silently.
                continue
            insert_pos = len(pred_blk.instructions)
            if (
                pred_blk.instructions
                and isinstance(pred_blk.instructions[-1], _TERMINATOR_TYPES)
            ):
                insert_pos -= 1
            pred_blk.instructions[insert_pos:insert_pos] = copies
        blk.instructions = [
            i for i in blk.instructions if not isinstance(i, tac_ast.Phi)
        ]

    return cfg_to_function(fn, cfg)
