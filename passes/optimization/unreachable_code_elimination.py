"""TAC unreachable-code elimination.

Four sub-passes, run in order against the function's CFG:

1. Drop unreachable blocks. Forward-traverse from ENTRY; any block
   not visited (no instruction control-flow path can reach it) is
   removed entirely. Code after a `Ret` / `Jump` until the next
   labeled jump-target falls into this category — once we hit the
   terminator, fall-through stops, and the next block is reachable
   only if some Jump elsewhere targets its label. SSA cleanup
   piggybacks here: any `PhiArg` in a surviving block whose
   `pred_label` named a dropped block is also dropped.

2. Fold singleton Phis. After step 1 (and after the optimizer
   driver's earlier passes propagate constants into Phi args), any
   `Phi` whose remaining argument list has exactly one entry is
   semantically just `Copy(args[0].source, dst)`; rewrite it.
   Zero-arg Phis (whose every predecessor was dropped) are
   discarded — defensive, since the Phi-bearing block itself would
   then also be unreachable and step 1 should already have
   dropped it.

3. Drop useless jumps. A non-last block whose terminator's only
   successor is the source-order next block doesn't need the
   terminator at all — fall-through gets there for free. Covers
   `Jump(L)` (when L's block is the next block) and conditional
   `JumpIfTrue(c, L)` / `JumpIfFalse(c, L)` (when L is the next
   block, so taken and fall-through coincide). `Ret` is never
   dropped — its successor is EXIT, not a real block.

4. Drop useless labels. A `Label(L)` at a block's start is useless
   if no remaining `Jump` / `JumpIfTrue` / `JumpIfFalse` AND no
   `PhiArg.pred_label` targets L. Removing it doesn't affect control
   flow — the block stays reachable via fall-through. Including the
   Phi `pred_label` set in the "live targets" check is essential in
   SSA form, otherwise dropping a label referenced by a Phi would
   leave SSA-destruction unable to locate the predecessor block.

Order matters: step 1 must precede 2 (singleton-Phi folding works
on the post-cleanup arg list); step 1 must precede 3 and 4 (dropping
a dead block removes any Jump references inside it); step 3 must
precede 4 (dropping a useless Jump may make its target's label
unused).

No fixed-point iteration is needed within the pass — the optimizer
driver re-runs the whole pipeline (constant folding → UCE → copy
propagation → dead-store elimination) until structural equality, so
any opportunities a downstream pass exposes get picked up on a
later cycle.

Empty blocks (left with no instructions after steps 3/4) are not
explicitly removed: `cfg_to_function` flattens by walking
`block_order` and emits each surviving block's instructions, so an
empty block contributes zero output instructions. The CFG's stale
edge bookkeeping isn't visible outside this pass — downstream
consumers re-run `build_cfg` against the rewritten function.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.cfg import (
    CFG,
    ENTRY_ID,
    build_cfg,
    cfg_to_function,
)


_JUMP_TYPES: tuple[type, ...] = (
    tac_ast.Jump,
    tac_ast.JumpIfTrue,
    tac_ast.JumpIfFalse,
)


def eliminate_unreachable_code(fn: tac_ast.Function) -> tac_ast.Function:
    cfg = build_cfg(fn)
    _remove_unreachable_blocks(cfg)
    _fold_singleton_phis(cfg)
    _remove_useless_jumps(cfg)
    _remove_useless_labels(cfg)
    return cfg_to_function(fn, cfg)


def _remove_unreachable_blocks(cfg: CFG) -> None:
    """Forward DFS from ENTRY; any block not visited is dead. Drop
    it from the CFG, including dangling predecessor / successor
    references in surviving blocks. Also drop PhiArgs in surviving
    blocks whose `pred_label` named a dropped block."""
    reachable: set[int] = set()
    stack: list[int] = [ENTRY_ID]
    while stack:
        bid = stack.pop()
        if bid in reachable:
            continue
        reachable.add(bid)
        stack.extend(cfg.blocks[bid].successors)

    # Capture labels of about-to-be-dropped blocks so we can prune
    # any PhiArg in surviving blocks that referenced them.
    dropped_labels: set[str] = set()
    for bid, blk in cfg.blocks.items():
        if bid in reachable:
            continue
        if blk.instructions and isinstance(blk.instructions[0], tac_ast.Label):
            dropped_labels.add(blk.instructions[0].name)

    for bid in [b for b in cfg.blocks if b not in reachable]:
        del cfg.blocks[bid]
    cfg.block_order = [b for b in cfg.block_order if b in reachable]
    for blk in cfg.blocks.values():
        blk.predecessors = [p for p in blk.predecessors if p in reachable]
        blk.successors = [s for s in blk.successors if s in reachable]
        if not dropped_labels:
            continue
        for instr in blk.instructions:
            if isinstance(instr, tac_ast.Phi):
                instr.args = [
                    a for a in instr.args
                    if a.pred_label not in dropped_labels
                ]


def _fold_singleton_phis(cfg: CFG) -> None:
    """A `Phi` with exactly one remaining `PhiArg` is semantically a
    `Copy(args[0].source, dst)`; rewrite it. A zero-arg Phi is
    discarded (defensive — its block would also be unreachable in a
    well-formed CFG)."""
    for blk in cfg.blocks.values():
        new_instrs: list[tac_ast.Type_instruction] = []
        for instr in blk.instructions:
            if not isinstance(instr, tac_ast.Phi):
                new_instrs.append(instr)
                continue
            if len(instr.args) == 0:
                continue
            if len(instr.args) == 1:
                new_instrs.append(tac_ast.Copy(
                    src=instr.args[0].source, dst=instr.dst,
                ))
                continue
            new_instrs.append(instr)
        blk.instructions = new_instrs


def _remove_useless_jumps(cfg: CFG) -> None:
    """For each non-last real block, if its terminator is a Jump or
    conditional Jump whose every successor equals the source-order
    next block, drop the terminator and collapse duplicate edges."""
    for i, bid in enumerate(cfg.block_order):
        if i + 1 >= len(cfg.block_order):
            continue
        next_bid = cfg.block_order[i + 1]
        blk = cfg.blocks[bid]
        if not blk.instructions:
            continue
        last = blk.instructions[-1]
        if not isinstance(last, _JUMP_TYPES):
            continue
        if any(s != next_bid for s in blk.successors):
            continue
        blk.instructions = blk.instructions[:-1]
        # A conditional jump's two successors collapse from
        # [next, next] to [next]; an unconditional jump already had
        # one. Mirror on the next block's predecessor list.
        blk.successors = [next_bid]
        next_preds = cfg.blocks[next_bid].predecessors
        if next_preds.count(bid) > 1:
            next_preds[:] = [p for p in next_preds if p != bid] + [bid]


def _remove_useless_labels(cfg: CFG) -> None:
    """Collect every Jump target AND every `PhiArg.pred_label` across
    the function; drop any leading `Label(L)` whose `L` isn't in
    that set. Phi pred_labels count as uses — SSA destruction
    later locates the predecessor block by its leading label, so
    dropping a Phi-referenced label would break de-SSA."""
    targets: set[str] = set()
    for blk in cfg.blocks.values():
        for instr in blk.instructions:
            if isinstance(instr, _JUMP_TYPES):
                targets.add(instr.target)
            elif isinstance(instr, tac_ast.Phi):
                for arg in instr.args:
                    targets.add(arg.pred_label)
    for blk in cfg.blocks.values():
        if blk.instructions and isinstance(blk.instructions[0], tac_ast.Label):
            if blk.instructions[0].name not in targets:
                blk.instructions = blk.instructions[1:]
