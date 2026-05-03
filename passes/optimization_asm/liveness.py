"""Backward-dataflow liveness analysis for `asm_ast.Function`.

Operates on Pseudo names. Variables tracked are the byte-versioned
SSA names produced by `to_ssa` (every byte of a multi-byte original
becomes its own 1-byte variable with `offset == 0`). Names that
shouldn't be colored — statics, address-taken, params, RMW targets —
are filtered by the *caller* (the interference builder), not here;
the liveness pass itself is faithful to every Pseudo in the IR.

Computes per-block `live_in` / `live_out` and exposes per-instruction
queries (`live_after` / `live_before`) for the interference builder.

Phi handling mirrors the TAC version:
  * Phi `dst` is killed at block entry (parallel to siblings, before
    the first non-Phi instruction).
  * Phi `args[i].source` is attributed to the matching predecessor's
    `live_out` — the de-SSA Mov reads it on the predecessor edge,
    not in the merge block.

`Liveness` is a snapshot. Mutating `fn` after computation invalidates
the cached per-instruction information.
"""

from __future__ import annotations

from collections import defaultdict
from dataclasses import dataclass, field

import asm_ast
from passes.optimization_asm.cfg import (
    CFG,
    BasicBlock,
    build_cfg,
)


@dataclass
class Liveness:
    """Snapshot of the function's liveness state."""

    cfg: CFG
    live_in: dict[int, frozenset[str]]
    live_out: dict[int, frozenset[str]]
    _per_instr_after: dict[int, list[frozenset[str]]] = field(
        default_factory=dict,
    )

    def live_after(self, bid: int, instr_index: int) -> frozenset[str]:
        cache = self._ensure_block_cache(bid)
        return cache[instr_index]

    def live_before(self, bid: int, instr_index: int) -> frozenset[str]:
        cache = self._ensure_block_cache(bid)
        if instr_index > 0:
            return cache[instr_index - 1]
        blk = self.cfg.blocks[bid]
        phi_dsts = {
            instr.dst.name
            for instr in blk.instructions
            if isinstance(instr, asm_ast.Phi)
            and isinstance(instr.dst, asm_ast.Pseudo)
        }
        return frozenset(self.live_in[bid] | phi_dsts)

    def _ensure_block_cache(self, bid: int) -> list[frozenset[str]]:
        if bid in self._per_instr_after:
            return self._per_instr_after[bid]
        blk = self.cfg.blocks[bid]
        # Walk forward, accumulating per-instruction "after" sets.
        # We do this by running the same backward update we'd use for
        # block-level `live_in` — start from `live_out[bid]` and walk
        # backward — and inverting: the post-instruction set at
        # source-position k is the input to position k+1's update.
        out_sets: list[frozenset[str]] = [frozenset()] * len(blk.instructions)
        live = set(self.live_out[bid])
        for i in range(len(blk.instructions) - 1, -1, -1):
            out_sets[i] = frozenset(live)
            instr = blk.instructions[i]
            if isinstance(instr, asm_ast.Phi):
                # Phi dsts kill at block entry, sources are
                # predecessor-edge uses; neither contributes here.
                continue
            for d in _defs_in(instr):
                live.discard(d.name)
            for u in _uses_in(instr):
                live.add(u.name)
        self._per_instr_after[bid] = out_sets
        return out_sets


def compute_liveness(fn: asm_ast.Function) -> Liveness:
    """Compute liveness for `fn`. The CFG is built fresh; per-block
    `live_in` / `live_out` are computed by iterative dataflow."""
    cfg = build_cfg(fn)
    return _compute(cfg)


def _compute(cfg: CFG) -> Liveness:
    # Standard fixpoint:
    #   live_out[B] = ∪ over successors S of: live_in[S] minus Phi
    #                  dsts at S, plus the Phi-source operands at S
    #                  attributed to B (i.e., Phi(dst, args=[...,
    #                  (B_label, src), ...]) contributes `src` to
    #                  live_out[B]).
    #   live_in[B]  = uses[B] ∪ (live_out[B] ∖ defs[B])
    #
    # `defs[B]` includes Phi dsts (which are killed at block entry).
    # `uses[B]` is the standard block-local upward-exposed-use set.
    block_label = _block_labels(cfg)
    label_to_bid = {lab: bid for bid, lab in block_label.items()}

    gen: dict[int, set[str]] = {}
    kill: dict[int, set[str]] = {}
    for bid, blk in cfg.blocks.items():
        gen_b: set[str] = set()
        kill_b: set[str] = set()
        # Phi dsts kill at block entry.
        for instr in blk.instructions:
            if isinstance(instr, asm_ast.Phi) and isinstance(
                instr.dst, asm_ast.Pseudo,
            ):
                kill_b.add(instr.dst.name)
        # Walk non-Phi instructions in source order, building
        # gen / kill standardly.
        for instr in blk.instructions:
            if isinstance(instr, asm_ast.Phi):
                continue
            for u in _uses_in(instr):
                if u.name not in kill_b:
                    gen_b.add(u.name)
            for d in _defs_in(instr):
                kill_b.add(d.name)
        gen[bid] = gen_b
        kill[bid] = kill_b

    # Per-edge Phi-source contributions: for each Phi at block S,
    # each `args[i]` (pred_label, source) contributes `source.name`
    # to live_out of the predecessor block whose leading Label is
    # `pred_label`.
    phi_edge_contrib: dict[int, set[str]] = defaultdict(set)
    for bid, blk in cfg.blocks.items():
        for instr in blk.instructions:
            if not isinstance(instr, asm_ast.Phi):
                continue
            for arg in instr.args:
                if not isinstance(arg.source, asm_ast.Pseudo):
                    continue
                pred_bid = label_to_bid.get(arg.pred_label)
                if pred_bid is None:
                    continue
                phi_edge_contrib[pred_bid].add(arg.source.name)

    live_in: dict[int, set[str]] = {b: set() for b in cfg.blocks}
    live_out: dict[int, set[str]] = {b: set() for b in cfg.blocks}
    changed = True
    while changed:
        changed = False
        for bid in cfg.blocks:
            new_out: set[str] = set(phi_edge_contrib.get(bid, set()))
            for s in cfg.blocks[bid].successors:
                # Successor's live_in minus Phi dsts at successor.
                # We approximate by computing live_in[s] which
                # already excludes the Phi dsts (they're in kill[s]).
                new_out |= live_in[s]
                # Now subtract Phi dsts at successor — they're
                # killed at block entry but live_in already accounts
                # for that.
            new_in = gen[bid] | (new_out - kill[bid])
            if new_out != live_out[bid] or new_in != live_in[bid]:
                live_out[bid] = new_out
                live_in[bid] = new_in
                changed = True

    return Liveness(
        cfg=cfg,
        live_in={bid: frozenset(s) for bid, s in live_in.items()},
        live_out={bid: frozenset(s) for bid, s in live_out.items()},
    )


def _block_labels(cfg: CFG) -> dict[int, str]:
    out: dict[int, str] = {}
    for bid, blk in cfg.blocks.items():
        if blk.instructions and isinstance(blk.instructions[0], asm_ast.Label):
            out[bid] = blk.instructions[0].name
    return out


# ---------------------------------------------------------------------------
# Per-instruction defs / uses.
# ---------------------------------------------------------------------------


def _defs_in(instr: asm_ast.Type_instruction):
    """Yield Pseudos defined by `instr`. Mirrors the SSA-construction
    helper but kept here so liveness has no upward dependency on
    the SSA module."""
    match instr:
        case asm_ast.Mov(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.Pop(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.LoadAddress(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.Phi(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case (
            asm_ast.Add(dst=dst) | asm_ast.Sub(dst=dst)
            | asm_ast.And(dst=dst) | asm_ast.Or(dst=dst)
            | asm_ast.Xor(dst=dst)
        ):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case (
            asm_ast.Inc(dst=dst) | asm_ast.Dec(dst=dst)
            | asm_ast.ArithmeticShiftLeft(dst=dst)
            | asm_ast.LogicalShiftRight(dst=dst)
            | asm_ast.RotateLeft(dst=dst)
            | asm_ast.RotateRight(dst=dst)
        ):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst


def _uses_in(instr: asm_ast.Type_instruction):
    """Yield Pseudos used by `instr`."""
    match instr:
        case asm_ast.Mov(src=src):
            if isinstance(src, asm_ast.Pseudo):
                yield src
        case asm_ast.Push(src=src):
            if isinstance(src, asm_ast.Pseudo):
                yield src
        case asm_ast.Compare(left=left, right=right):
            if isinstance(left, asm_ast.Pseudo):
                yield left
            if isinstance(right, asm_ast.Pseudo):
                yield right
        case asm_ast.Add(src=src, dst=dst) | asm_ast.Sub(src=src, dst=dst) | asm_ast.And(src=src, dst=dst) | asm_ast.Or(src=src, dst=dst):
            if isinstance(src, asm_ast.Pseudo):
                yield src
            # Pseudo dst (uncommon in current tac_to_asm output) is
            # both use and def for ADC-style RMW.
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.Xor(src1=s1, src2=s2):
            if isinstance(s1, asm_ast.Pseudo):
                yield s1
            if isinstance(s2, asm_ast.Pseudo):
                yield s2
        case (
            asm_ast.Inc(dst=dst) | asm_ast.Dec(dst=dst)
            | asm_ast.ArithmeticShiftLeft(dst=dst)
            | asm_ast.LogicalShiftRight(dst=dst)
            | asm_ast.RotateLeft(dst=dst)
            | asm_ast.RotateRight(dst=dst)
        ):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.LoadAddress(src=src):
            if isinstance(src, asm_ast.Pseudo):
                yield src
        case asm_ast.Phi(args=args):
            # Phi sources are predecessor-edge uses, NOT block-local
            # uses. Liveness handles them via `phi_edge_contrib`.
            return
