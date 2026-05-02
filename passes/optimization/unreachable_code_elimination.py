"""TAC unreachable-code elimination.

Three sub-passes, run in order against the function's CFG:

1. Drop unreachable blocks. Forward-traverse from ENTRY; any block
   not visited (no instruction control-flow path can reach it) is
   removed entirely. Code after a `Ret` / `Jump` until the next
   labeled jump-target falls into this category — once we hit the
   terminator, fall-through stops, and the next block is reachable
   only if some Jump elsewhere targets its label.

2. Drop useless jumps. A non-last block whose terminator's only
   successor is the source-order next block doesn't need the
   terminator at all — fall-through gets there for free. Covers
   `Jump(L)` (when L's block is the next block) and conditional
   `JumpIfTrue(c, L)` / `JumpIfFalse(c, L)` (when L is the next
   block, so taken and fall-through coincide). `Ret` is never
   dropped — its successor is EXIT, not a real block.

3. Drop useless labels. A `Label(L)` at a block's start is useless
   if no remaining `Jump` / `JumpIfTrue` / `JumpIfFalse` targets L.
   Removing it doesn't affect control flow — the block stays
   reachable via fall-through.

Order matters: step 1 must precede 2 and 3 (dropping a dead block
removes any Jump references inside it); step 2 must precede 3
(dropping a useless Jump may make its target's label unused).

No fixed-point iteration is needed within the pass — the optimizer
driver re-runs the whole pipeline (constant folding → UCE → copy
propagation → dead-store elimination) until structural equality, so
any opportunities a downstream pass exposes get picked up on a
later cycle.

Empty blocks (left with no instructions after steps 2/3) are not
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
    _remove_useless_jumps(cfg)
    _remove_useless_labels(cfg)
    return cfg_to_function(fn, cfg)


def _remove_unreachable_blocks(cfg: CFG) -> None:
    """Forward DFS from ENTRY; any block not visited is dead. Drop
    it from the CFG, including dangling predecessor / successor
    references in surviving blocks."""
    reachable: set[int] = set()
    stack: list[int] = [ENTRY_ID]
    while stack:
        bid = stack.pop()
        if bid in reachable:
            continue
        reachable.add(bid)
        stack.extend(cfg.blocks[bid].successors)

    for bid in [b for b in cfg.blocks if b not in reachable]:
        del cfg.blocks[bid]
    cfg.block_order = [b for b in cfg.block_order if b in reachable]
    for blk in cfg.blocks.values():
        blk.predecessors = [p for p in blk.predecessors if p in reachable]
        blk.successors = [s for s in blk.successors if s in reachable]


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
    """Collect every Jump target across the function; drop any leading
    `Label(L)` whose `L` isn't in that set."""
    targets: set[str] = set()
    for blk in cfg.blocks.values():
        for instr in blk.instructions:
            if isinstance(instr, _JUMP_TYPES):
                targets.add(instr.target)
    for blk in cfg.blocks.values():
        if blk.instructions and isinstance(blk.instructions[0], tac_ast.Label):
            if blk.instructions[0].name not in targets:
                blk.instructions = blk.instructions[1:]
