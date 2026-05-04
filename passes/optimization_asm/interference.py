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
    reference downstream).

    Most nodes are 1 byte wide (asm-SSA has split each multi-byte
    original into per-byte-versioned variables). The exception is
    `LoadAddress.dst`: a 2-byte pointer whose high byte is implicitly
    written at storage_base+1 (see emit's `_shift_offset(dst, 1)`),
    so the SSA layer doesn't byte-version it. It still needs ZP
    coloring eligibility — we add it to the graph as a `width=2`
    node so the allocator finds 2 contiguous bytes for it."""
    excluded, multi_byte_widths = _categorize_names(fn, statics)
    excluded |= set(fn.params)

    graph = InterferenceGraph()

    def colorable(name: str) -> bool:
        return name not in excluded

    def width_of(name: str) -> int:
        return multi_byte_widths.get(name, 1)

    def ensure_node(name: str) -> None:
        if name not in graph.nodes:
            graph.nodes[name] = InterferenceNode(
                name=name, width=width_of(name), lives_across_call=False,
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


def _categorize_names(
    fn: asm_ast.Function,
    statics: frozenset[str],
) -> tuple[set[str], dict[str, int]]:
    """Classify every Pseudo name in `fn` into one of three kinds:

      * **excluded** (returned in the first set): not eligible for
        ZP coloring. Includes statics and address-taken sources
        (`LoadAddress.src`).
      * **multi-byte coloring candidate** (returned as
        `{name: width}`): names that need a contiguous N-byte ZP
        block. Two sources:
          - `LoadAddress.dst` names (always 2 bytes — addresses
            are 16-bit on the 6502; the LoadAddress instruction
            implicitly writes both bytes of its dst).
          - Any Pseudo name with references at multiple byte
            offsets that isn't otherwise excluded — typically the
            in-place inline-shift dst from `tac_to_asm`'s shift-
            by-1 lowering. Width is `max_offset + 1`.
        The interference graph adds them with `width=N` so the
        allocator finds N consecutive free bytes.
      * **single-byte (default)**: every other Pseudo. Width=1.
        Includes RMW-target names (`Inc / Dec / ASL / LSR / ROL /
        ROR.dst`) when their references are single-offset — those
        are treated as ordinary 1-byte values whose def site
        happens to also be a use site.

    `statics` is the set of static-storage names (file-scope
    globals, externs); they're always excluded."""
    excluded: set[str] = set(statics)
    multi_byte_widths: dict[str, int] = {}
    referenced: dict[str, set[int]] = defaultdict(set)
    for instr in fn.instructions:
        if isinstance(instr, asm_ast.LoadAddress):
            if isinstance(instr.src, asm_ast.Pseudo):
                excluded.add(instr.src.name)
            # LoadAddress.dst: 2-byte address. We DON'T exclude
            # it; instead we color it with width=2 so regalloc
            # finds 2 contiguous ZP bytes.
            if isinstance(instr.dst, asm_ast.Pseudo):
                multi_byte_widths.setdefault(instr.dst.name, 2)
        for op in _all_pseudos_in(instr):
            referenced[op.name].add(op.offset)
    # Names referenced at multiple byte offsets that aren't already
    # multi-byte coloring candidates: promote to multi-byte coloring
    # with `width = max_offset + 1`. This catches the RMW-shift
    # output from `tac_to_asm`'s inline shift-by-1, where each
    # byte of a multi-byte temp is shifted in place via a Pseudo
    # operand on the shift atom.
    for name, offsets in referenced.items():
        if max(offsets) > 0 and name not in multi_byte_widths:
            multi_byte_widths[name] = max(offsets) + 1
    # If a name slipped into both the multi-byte set AND was
    # individually excluded, the exclusion wins.
    for name in list(multi_byte_widths.keys()):
        if name in excluded:
            del multi_byte_widths[name]
    return excluded, multi_byte_widths


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
