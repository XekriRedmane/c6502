"""TAC dead pure-loop elimination.

If a natural loop has no observable side effects (no Call, no Store,
no Ret) and no value defined inside it is read outside, the entire
loop is dead — its only purpose is to spin computing values nobody
reads. We rewrite the loop's header block's contents to a `Jump` at
the loop's exit, leaving every other body block unreachable for the
next UCE iteration to prune.

Runs inside the optimizer's TAC fixed-point loop, so the input is in
SSA form. The SSA single-def invariant turns the live-out check into
a simple name scan: a name defined inside the loop is "live-out"
exactly when it appears as a use in some instruction outside the
body. Non-SSA defs (statics, address-taken locals, aggregates) inside
a loop body conservatively block elimination — proving them
not-live-out would require non-SSA liveness analysis, and the common
shape that motivates this pass (empty `do { } while (--counter)` or
`while (--counter)`) only writes SSA-renamed names anyway.

Conservative gates beyond purity:

  - Exactly one exit edge — a single `(body_block → exit_block)`
    edge leaves the loop. Multi-exit loops are skipped because
    consolidating multiple exit-point Phi args into one is only
    safe when their sources agree, and we don't want to check
    that here.
  - The exit block has a leading `Label`. We jump to it by name;
    a labelless block can't be the target of a Jump.
  - The header block has a leading `Label`. We need to preserve
    it (other code may still jump to it from outside the loop).

If the rewrite fires, the header's instructions become
`[Label(header_name), Jump(exit_label)]`. Any Phi at the exit
whose `pred_label` named the (old) exit-edge source gets retagged
to the header — the predecessor of the exit from the loop side is
now the header, not the body block. We've already checked no Phi
arg's *source* is a loop_def, so the retag is just a structural
predecessor-label update; the value semantics are unchanged.

Requires SSA form. A `ssa_dsts=None` caller is a no-op (mirrors
`eliminate_dead_stores`'s behavior in that situation).
"""

from __future__ import annotations

import tac_ast
from passes.optimization.cfg import (
    CFG,
    build_cfg,
    cfg_to_function,
    natural_loops,
)
from passes.optimization.var_visit import defs_in, uses_in


# A loop containing any of these can't be safely deleted.
#   FunctionCall / IndirectCall — callees may have arbitrary effects.
#   Store / IndexedStore / IndirectIndexedStore — writes to (possibly
#     aliased) memory observable beyond the loop.
#   Ret — exiting via Ret is observably distinct from falling out
#     of the loop normally; collapsing them would change function
#     return behavior.
_SIDE_EFFECTING_TYPES: tuple[type, ...] = (
    tac_ast.FunctionCall,
    tac_ast.IndirectCall,
    tac_ast.Store,
    tac_ast.IndexedStore,
    tac_ast.IndirectIndexedStore,
    tac_ast.Ret,
)


def eliminate_dead_loops(
    fn: tac_ast.Function,
    *,
    ssa_dsts: set[str] | None = None,
) -> tac_ast.Function:
    """Drop natural loops that compute only loop-local values.

    Returns a new function with any eligible loop's header rewritten
    to skip the loop; UCE on a subsequent iteration prunes the
    now-unreachable body. If no loop qualifies, returns `fn`
    unchanged."""
    if ssa_dsts is None:
        return fn
    cfg = build_cfg(fn)
    loops = natural_loops(cfg)
    changed = False
    for header, body in loops:
        if _try_eliminate(cfg, header, body, ssa_dsts):
            changed = True
    if not changed:
        return fn
    return cfg_to_function(fn, cfg)


def _try_eliminate(
    cfg: CFG,
    header: int,
    body: set[int],
    ssa_dsts: set[str],
) -> bool:
    """Verify the loop is pure and loop-local, then rewrite the
    header to jump past the body. Mutates `cfg` in place if the
    rewrite fires; returns True iff it did."""
    # Purity: every body instruction must be pure. Phi / Copy /
    # Binary / Unary / cast / Load / GetAddress / IndexedLoad /
    # IndexedConstLoad / IndirectIndexedLoad / Label / Jump /
    # JumpIfTrue / JumpIfFalse / JumpIfCmp / JumpIfMasked all pass.
    for bid in body:
        for instr in cfg.blocks[bid].instructions:
            if isinstance(instr, _SIDE_EFFECTING_TYPES):
                return False

    # All defs in the loop body must be SSA-renamed names. Non-SSA
    # defs would require full liveness to prove not-live-out.
    loop_defs: set[str] = set()
    for bid in body:
        for instr in cfg.blocks[bid].instructions:
            for d in defs_in(instr):
                if d.name not in ssa_dsts:
                    return False
                loop_defs.add(d.name)

    # Live-out check: no use OUTSIDE the loop references any
    # loop_def. `uses_in` returns Phi sources as uses, so a Phi
    # in the exit block whose arg sources a loop_def is caught here.
    for bid, blk in cfg.blocks.items():
        if bid in body:
            continue
        for instr in blk.instructions:
            for u in uses_in(instr):
                if u.name in loop_defs:
                    return False

    # Exit edge: must be exactly one (body_block → non_body_block).
    exit_edges: list[tuple[int, int]] = []
    for bid in body:
        for succ in cfg.blocks[bid].successors:
            if succ not in body:
                exit_edges.append((bid, succ))
    if len(exit_edges) != 1:
        return False
    src_id, exit_block_id = exit_edges[0]

    # Exit block must have a leading label so we can name a Jump
    # target.
    exit_blk = cfg.blocks[exit_block_id]
    if not exit_blk.instructions or not isinstance(
        exit_blk.instructions[0], tac_ast.Label,
    ):
        return False
    exit_label = exit_blk.instructions[0].name

    # Header must have a leading label so we can preserve any
    # external jumps to it.
    header_blk = cfg.blocks[header]
    if not header_blk.instructions or not isinstance(
        header_blk.instructions[0], tac_ast.Label,
    ):
        return False
    header_label = header_blk.instructions[0].name

    # Patch up Phis at the exit: the old loop-side predecessor was
    # `src_id`; the new one is `header`. Retag every PhiArg whose
    # `pred_label` named the old src.
    src_blk = cfg.blocks[src_id]
    src_label: str | None = None
    if src_blk.instructions and isinstance(
        src_blk.instructions[0], tac_ast.Label,
    ):
        src_label = src_blk.instructions[0].name
    if src_label is not None and src_label != header_label:
        for instr in exit_blk.instructions:
            if isinstance(instr, tac_ast.Phi):
                instr.args = [
                    tac_ast.PhiArg(
                        source=a.source,
                        pred_label=(
                            header_label
                            if a.pred_label == src_label
                            else a.pred_label
                        ),
                    )
                    for a in instr.args
                ]

    # Rewrite the header: keep its label so anything still jumping
    # in lands somewhere valid, then unconditionally jump past the
    # body. UCE on the next fixed-point sweep prunes the rest of
    # the body (now unreachable from ENTRY).
    header_blk.instructions = [
        tac_ast.Label(name=header_label),
        tac_ast.Jump(target=exit_label),
    ]
    return True
