"""Interference graph construction for asm-level register allocation.

Two Pseudo names interfere when both are live at the same program
point. Each colorable name is treated as 1 byte wide — that's the
whole point of byte-granular SSA: every byte of every multi-byte
original has been split into its own variable upstream.

Filters applied here (names NOT added to the graph):
  * `statics` — file-scope globals, block-scope statics, externs.
    Their addresses are link-time fixed.
  * `address_taken` — names appearing as `LoadAddress.src`.
    Multi-byte coherence required.
  * `params` — function parameters. The calling convention
    delivers their bytes via Frame addressing on entry.
  * `rmw_targets` — names appearing as `Inc/Dec/ASL/LSR/ROL/ROR.dst`.
    Defensive (not produced by today's `tac_to_asm`).
  * any name whose Pseudo references include a non-zero `offset` —
    that's an unversioned multi-byte name (typically the initial
    bytes of a param being read pre-SSA-rename), not eligible for
    1-byte coloring.

`lives_across_call` is set on a node iff the value is live just
before any `Call` instruction (post-def-removed state). The asm-
level coloring driver uses this to prefer caller- vs callee-saved
ZP slots, mirroring the TAC version.
"""

from __future__ import annotations

from collections import defaultdict
from dataclasses import dataclass, field

import asm_ast
from passes.optimization.interference import (
    InterferenceGraph,
    InterferenceNode,
)
from passes.optimization_asm.liveness import Liveness, _defs_in, _uses_in


def build_interference(
    fn: asm_ast.Function,
    liveness: Liveness,
    *,
    statics: frozenset[str] = frozenset(),
) -> InterferenceGraph:
    """Build the interference graph for `fn` from its `liveness`
    snapshot. Returns the same `InterferenceGraph` type the TAC
    pipeline uses (its name is the only field colored values
    reference downstream)."""
    excluded = _excluded_names(fn) | statics
    excluded |= set(fn.params)

    graph = InterferenceGraph()

    def colorable(name: str) -> bool:
        return name not in excluded

    def ensure_node(name: str) -> None:
        if name not in graph.nodes:
            graph.nodes[name] = InterferenceNode(
                name=name, width=1, lives_across_call=False,
            )

    for bid, blk in liveness.cfg.blocks.items():
        live: set[str] = {n for n in liveness.live_out[bid] if colorable(n)}

        # Reverse walk over non-Phi instructions.
        for i in range(len(blk.instructions) - 1, -1, -1):
            instr = blk.instructions[i]
            if isinstance(instr, asm_ast.Phi):
                continue
            for d in _defs_in(instr):
                if not colorable(d.name):
                    continue
                ensure_node(d.name)
                for n in live:
                    if n != d.name:
                        graph.add_edge(d.name, n)
                live.discard(d.name)
            # Cross-call detection.
            if isinstance(instr, asm_ast.Call):
                for n in live:
                    if n in graph.nodes:
                        graph.nodes[n].lives_across_call = True
            for u in _uses_in(instr):
                if not colorable(u.name):
                    continue
                ensure_node(u.name)
                live.add(u.name)

        # Now handle Phi defs (which are conceptually defined at
        # block entry, parallel-style). All Phi dsts in this block
        # interfere with each other and with everything live
        # immediately before the first non-Phi instruction (which
        # is `live` after the reverse walk).
        phi_dsts = [
            instr.dst.name
            for instr in blk.instructions
            if isinstance(instr, asm_ast.Phi)
            and isinstance(instr.dst, asm_ast.Pseudo)
            and colorable(instr.dst.name)
        ]
        for k, dst in enumerate(phi_dsts):
            ensure_node(dst)
            for other in phi_dsts[k + 1:]:
                graph.add_edge(dst, other)
            for n in live:
                if n != dst:
                    graph.add_edge(dst, n)

    return graph


# ---------------------------------------------------------------------------
# Excluded-name discovery.
# ---------------------------------------------------------------------------


def _excluded_names(fn: asm_ast.Function) -> set[str]:
    """Names that can't be byte-granular-colored. Includes
    address-taken, RMW targets, and any name that has a Pseudo
    reference at non-zero offset (= multi-byte unversioned form)."""
    excluded: set[str] = set()
    multi_byte: set[str] = set()
    referenced: dict[str, set[int]] = defaultdict(set)
    for instr in fn.instructions:
        match instr:
            case asm_ast.LoadAddress(src=src, dst=dst):
                # Both src and dst exclude. src is address-taken;
                # dst holds a 2-byte address whose high byte is
                # implicitly stored at storage_base+1 (see emit's
                # `_shift_offset(dst, 1)`) — versioning byte 0 alone
                # would let regalloc place another value at byte 1's
                # implicit storage location.
                if isinstance(src, asm_ast.Pseudo):
                    excluded.add(src.name)
                if isinstance(dst, asm_ast.Pseudo):
                    excluded.add(dst.name)
            case (
                asm_ast.Inc(dst=dst)
                | asm_ast.Dec(dst=dst)
                | asm_ast.ArithmeticShiftLeft(dst=dst)
                | asm_ast.LogicalShiftRight(dst=dst)
                | asm_ast.RotateLeft(dst=dst)
                | asm_ast.RotateRight(dst=dst)
            ):
                if isinstance(dst, asm_ast.Pseudo):
                    excluded.add(dst.name)
        for op in _all_pseudos_in(instr):
            referenced[op.name].add(op.offset)
    for name, offsets in referenced.items():
        if any(o != 0 for o in offsets):
            multi_byte.add(name)
    return excluded | multi_byte


def _all_pseudos_in(
    instr: asm_ast.Type_instruction,
):
    """Yield every Pseudo operand appearing anywhere in `instr`."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            for op in (src, dst):
                if isinstance(op, asm_ast.Pseudo):
                    yield op
        case asm_ast.Add(src=src, dst=dst) | asm_ast.Sub(src=src, dst=dst) | asm_ast.And(src=src, dst=dst) | asm_ast.Or(src=src, dst=dst):
            for op in (src, dst):
                if isinstance(op, asm_ast.Pseudo):
                    yield op
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            for op in (s1, s2, dst):
                if isinstance(op, asm_ast.Pseudo):
                    yield op
        case asm_ast.Inc(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.Dec(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.LogicalShiftRight(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.RotateLeft(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.RotateRight(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.Push(src=src):
            if isinstance(src, asm_ast.Pseudo):
                yield src
        case asm_ast.Pop(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.Compare(left=left, right=right):
            for op in (left, right):
                if isinstance(op, asm_ast.Pseudo):
                    yield op
        case asm_ast.LoadAddress(src=src, dst=dst):
            for op in (src, dst):
                if isinstance(op, asm_ast.Pseudo):
                    yield op
        case asm_ast.Phi(dst=dst, args=args):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
            for a in args:
                if isinstance(a.source, asm_ast.Pseudo):
                    yield a.source
