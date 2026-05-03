"""Asm-level register allocation.

Byte-granular: every node in the interference graph is exactly 1
byte wide (since asm-SSA has already split multi-byte values into
independent byte-versioned variables). Coloring runs PEO + greedy
fit, mirroring the TAC version's design but operating on the asm
CFG and asm SSA-renamed names.

Reuses `Coloring` and `Pool` from `passes.optimization.register_
allocation` — those types are agnostic to TAC vs asm; only the
interference-graph build and the PEO derivation differ.

Pool selection follows the same rule as the TAC version:
  * `lives_across_call=True` → callee-saved first (saved by the
    function's prologue/epilogue around any nested call).
  * `lives_across_call=False` → caller-saved first (no save/restore
    overhead; clobbered by any nested call we don't make).
Spills land in `Coloring.spilled` and `replace_pseudoregisters`
falls back to Frame allocation for them.
"""

from __future__ import annotations

import asm_ast
from passes.optimization.pool import Pool
from passes.optimization.register_allocation import (
    Coloring,
    _blocked_bytes,
    _find_fit,
)
from passes.optimization.interference import InterferenceGraph
from passes.optimization_asm.cfg import (
    ENTRY_ID,
    build_cfg,
    dominator_tree_children,
    immediate_dominators,
)
from passes.optimization_asm.liveness import _defs_in


def color_graph(
    fn: asm_ast.Function,
    graph: InterferenceGraph,
    *,
    pool: Pool | None = None,
) -> Coloring:
    """Color `graph`'s nodes onto ZP byte addresses drawn from
    `pool`. Returns a `Coloring` with every graph node either in
    `assignments` or in `spilled`."""
    if pool is None:
        pool = Pool()
    peo = _perfect_elimination_order(fn, graph)
    assignments: dict[str, int] = {}
    spilled: set[str] = set()
    for name in peo:
        node = graph.nodes[name]
        blocked = _blocked_bytes(name, graph, assignments)
        if node.lives_across_call:
            base = _find_fit(pool.callee_saved(), node.width, blocked)
        else:
            base = _find_fit(pool.caller_saved(), node.width, blocked)
            if base is None:
                base = _find_fit(pool.callee_saved(), node.width, blocked)
        if base is None:
            spilled.add(name)
        else:
            assignments[name] = base
    return Coloring(assignments=assignments, spilled=spilled, pool=pool)


def _perfect_elimination_order(
    fn: asm_ast.Function, graph: InterferenceGraph,
) -> list[str]:
    """Build a PEO over `graph`'s nodes via dom-tree pre-order walk
    of value definitions, then reverse. Identical algorithm to the
    TAC version, but operating on the asm CFG."""
    cfg = build_cfg(fn)
    idom = immediate_dominators(cfg)
    children = dominator_tree_children(idom)

    build: list[str] = []
    seen: set[str] = set()

    def emit(name: str) -> None:
        if name in seen or name not in graph.nodes:
            return
        seen.add(name)
        build.append(name)

    stack: list[int] = [ENTRY_ID]
    while stack:
        bid = stack.pop()
        blk = cfg.blocks.get(bid)
        if blk is not None:
            # Phi dsts first (parallel-defined at block entry).
            for instr in blk.instructions:
                if isinstance(instr, asm_ast.Phi) and isinstance(
                    instr.dst, asm_ast.Pseudo,
                ):
                    emit(instr.dst.name)
            # Then non-Phi defs in source order.
            for instr in blk.instructions:
                if isinstance(instr, asm_ast.Phi):
                    continue
                for d in _defs_in(instr):
                    emit(d.name)
        for c in reversed(children.get(bid, [])):
            stack.append(c)

    # Append any remaining nodes (unusual — mostly defensive). All
    # asm-SSA names have a defining instruction by construction;
    # only filter survivors land here. Sort for determinism.
    leftover = sorted(n for n in graph.nodes if n not in seen)
    build.extend(leftover)

    build.reverse()
    return build
