"""Backward-dataflow liveness analysis for `tac_ast.Function`.

Computes per-block `live_in` / `live_out` and exposes per-instruction
queries (`live_after` / `live_before`) for the interference-graph
builder. Operates on every `Var` in the function — not just the
SSA-promotable subset — so callers downstream of register allocation
can use the same analysis for spill insertion and de-SSA copy
placement.

Sound on both SSA and non-SSA TAC:
  - On non-SSA, a single name with multiple defs is killed by each
    def in source-order through the gen/kill computation.
  - On SSA, Phi nodes are special-cased: a Phi's `dst` is killed at
    block entry (conceptually before any non-Phi instruction), and
    its `args` are NOT block-local uses — they're attributed to the
    matching predecessor edge. Standard "Phi-source-as-pred-use"
    treatment.

`Liveness` is a snapshot. If callers mutate `fn` after computation,
the cached per-instruction information becomes stale. Compute fresh.
"""

from __future__ import annotations

from dataclasses import dataclass, field

import tac_ast
from passes.optimization.cfg import (
    CFG,
    BasicBlock,
    build_cfg,
)
from passes.optimization.var_visit import defs_in, uses_in


@dataclass
class Liveness:
    """Snapshot of the function's liveness state.

    Per-instruction queries lazily memoize block-level walks the first
    time they're called for a given block."""

    cfg: CFG
    live_in: dict[int, frozenset[str]]
    live_out: dict[int, frozenset[str]]
    # bid -> list[frozenset[str]] of length len(block.instructions),
    # each entry the live set immediately after that instruction.
    _per_instr_after: dict[int, list[frozenset[str]]] = field(
        default_factory=dict,
    )

    def live_after(self, bid: int, instr_index: int) -> frozenset[str]:
        """Vars live immediately after `instructions[instr_index]`.
        For the last instruction in a block, equals `live_out[bid]`."""
        cache = self._ensure_block_cache(bid)
        return cache[instr_index]

    def live_before(self, bid: int, instr_index: int) -> frozenset[str]:
        """Vars live immediately before `instructions[instr_index]`.
        For instr_index 0, returns the live set at the top of the
        non-Phi region of the block — i.e. `live_in[bid]` plus the
        dsts of all Phis in the block, since Phi dsts are conceptually
        defined parallel-style at block entry between live_in and the
        first ordinary instruction."""
        cache = self._ensure_block_cache(bid)
        if instr_index > 0:
            return cache[instr_index - 1]
        # Live set just before the first instruction. If the first
        # instruction is itself a Phi, "live_before" is live_in plus
        # the Phi dsts (they're defined at block entry, before any
        # non-Phi instruction). For non-Phi first instructions we
        # likewise want the post-Phi live set.
        blk = self.cfg.blocks[bid]
        phi_dsts = {
            instr.dst.name
            for instr in blk.instructions
            if isinstance(instr, tac_ast.Phi)
            and isinstance(instr.dst, tac_ast.Var)
        }
        return frozenset(self.live_in[bid] | phi_dsts)

    def _ensure_block_cache(self, bid: int) -> list[frozenset[str]]:
        cache = self._per_instr_after.get(bid)
        if cache is not None:
            return cache
        blk = self.cfg.blocks[bid]
        live: set[str] = set(self.live_out[bid])
        per_instr: list[frozenset[str]] = [frozenset()] * len(blk.instructions)
        for i in range(len(blk.instructions) - 1, -1, -1):
            per_instr[i] = frozenset(live)
            instr = blk.instructions[i]
            if isinstance(instr, tac_ast.Phi):
                # Phi defs are conceptually parallel at block entry,
                # not at this position — skip kill here. Phi sources
                # are attributed to predecessor edges, not used here.
                continue
            for d in defs_in(instr):
                live.discard(d.name)
            for u in uses_in(instr):
                live.add(u.name)
        self._per_instr_after[bid] = per_instr
        return per_instr


def compute_liveness(fn: tac_ast.Function) -> Liveness:
    """Compute per-block live-in / live-out for `fn`. Builds the CFG
    internally and returns it on the resulting `Liveness` so callers
    can chain into interference construction without a separate CFG
    build."""
    cfg = build_cfg(fn)

    # gen[B] / kill[B] over ALL Vars (not just promotable). Phi dsts
    # contribute to kill; Phi sources are NOT block-local uses.
    gen: dict[int, set[str]] = {}
    kill: dict[int, set[str]] = {}
    for bid, blk in cfg.blocks.items():
        gen_b: set[str] = set()
        kill_b: set[str] = set()
        for instr in blk.instructions:
            if isinstance(instr, tac_ast.Phi):
                # Phi dst kills at block entry; Phi sources don't
                # use here.
                if isinstance(instr.dst, tac_ast.Var):
                    kill_b.add(instr.dst.name)
                continue
            for u in uses_in(instr):
                if u.name not in kill_b:
                    gen_b.add(u.name)
            for d in defs_in(instr):
                kill_b.add(d.name)
        gen[bid] = gen_b
        kill[bid] = kill_b

    # For each block S, precompute two per-edge contributions:
    #   * `phi_src_contrib[s_bid][pred_bid]` — Vars contributed to
    #     pred_bid's live_out by S's Phi sources on the matching
    #     edge. Models "the Phi's source is read at the end of pred"
    #     (the future de-SSA Copy will read it there).
    #   * `phi_dst_contrib[s_bid]` — Vars (Phi dsts at S) contributed
    #     to EVERY predecessor's live_out, regardless of which edge.
    #     Models "the Phi's dst is written at the end of pred" (the
    #     future de-SSA Copy writes it there). Adding Phi dsts to
    #     pred's live_out is what makes them interfere with values
    #     still live across pred — without it, regalloc would
    #     happily share a slot between a Phi dst and another live
    #     value, and de-SSA's Copy would clobber the latter.
    label_to_block: dict[str, int] = {}
    for bid, blk in cfg.blocks.items():
        if blk.instructions and isinstance(blk.instructions[0], tac_ast.Label):
            label_to_block[blk.instructions[0].name] = bid

    phi_src_contrib: dict[int, dict[int, set[str]]] = {
        bid: {} for bid in cfg.blocks
    }
    phi_dst_contrib: dict[int, set[str]] = {
        bid: set() for bid in cfg.blocks
    }
    for s_bid, s_blk in cfg.blocks.items():
        for instr in s_blk.instructions:
            if not isinstance(instr, tac_ast.Phi):
                continue
            if isinstance(instr.dst, tac_ast.Var):
                phi_dst_contrib[s_bid].add(instr.dst.name)
            for arg in instr.args:
                pred_bid = label_to_block.get(arg.pred_label)
                if pred_bid is None:
                    # Phi source naming a label that no longer
                    # corresponds to any block — defensive skip;
                    # downstream UCE would normally have cleaned
                    # this up.
                    continue
                if isinstance(arg.source, tac_ast.Var):
                    phi_src_contrib[s_bid].setdefault(pred_bid, set()).add(
                        arg.source.name,
                    )

    live_in: dict[int, set[str]] = {b: set() for b in cfg.blocks}
    live_out: dict[int, set[str]] = {b: set() for b in cfg.blocks}
    changed = True
    while changed:
        changed = False
        for bid in cfg.blocks:
            new_out: set[str] = set()
            for s_bid in cfg.blocks[bid].successors:
                new_out |= live_in[s_bid]
                # Per-edge Phi-source contribution: a Phi at S whose
                # source on the (B → S) edge names V contributes V to
                # B's live_out, even though V isn't in S's live_in
                # (the Phi's dst is what's live-in at S).
                new_out |= phi_src_contrib[s_bid].get(bid, set())
                # Phi-dst contribution: every Phi dst at S is also
                # live at the end of B. Models the future de-SSA
                # Copy that will write to the Phi dst's slot at the
                # end of B; without this, regalloc could legally
                # reuse the dst's slot for another value live in B
                # and the de-SSA Copy would clobber it.
                new_out |= phi_dst_contrib[s_bid]
            new_in = gen[bid] | (new_out - kill[bid])
            if new_out != live_out[bid] or new_in != live_in[bid]:
                live_out[bid] = new_out
                live_in[bid] = new_in
                changed = True

    return Liveness(
        cfg=cfg,
        live_in={b: frozenset(s) for b, s in live_in.items()},
        live_out={b: frozenset(s) for b, s in live_out.items()},
    )
