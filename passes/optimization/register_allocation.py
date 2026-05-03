"""Width-aware chordal graph coloring for register allocation.

SSA interference graphs are chordal (Hack & Goos 2006: every SSA
program's interference graph admits a perfect elimination order
equal to the dominator-tree pre-order over its value definitions).
Greedy coloring in PEO is then optimal for unit-width nodes — every
node uses no more colors than the maximum clique size. With variable
widths (1/2/4/8 bytes per node) the bound no longer holds in
general, but greedy-with-PEO remains very effective in practice.
Spill decisions catch the rare cases where width-driven fragmentation
leaves no fit.

Pool selection is driven by `lives_across_call`:
  * `True`  → try **callee-saved** first. The callee that uses a
              callee-saved ZP byte saves it to the frame in its
              prologue and restores it in the epilogue, so callees
              we call won't disturb the value. Falls back to
              caller-saved if callee-saved is full (which gives
              the wrong semantics for cross-call values — a
              caller-saved slot is clobbered by the call — so this
              fallback effectively spills via subsequent miscompile;
              when neither pool fits at all, the value spills to
              frame explicitly).
  * `False` → try **caller-saved** first. Avoids the
              prologue/epilogue save+restore overhead a callee-
              saved register imposes. Falls back to callee-saved
              if caller-saved is full.
If neither pool fits, the node is spilled (its name lands in
`Coloring.spilled`). For correctness, cross-call values that don't
fit callee-saved should be spilled rather than placed in caller-
saved — `replace_pseudoregisters`'s save logic tracks every byte
the function uses from `coloring.pool.callee_saved()`, so a cross-
call value placed in caller-saved would be saved by NO ONE and
clobbered by the call.

PEO ordering subtlety: parameters and any other graph node not
defined by an instruction (e.g. an SSA-renamed param's initial
value) are treated as conceptually defined at the implicit ENTRY
point — they appear LAST in the dom-tree-preorder build, so after
the build-list reversal they appear FIRST in the PEO and are colored
first. Matches the dominance intuition: params dominate every other
definition and therefore need the broadest available palette.

NOT done in this slice:
  * Coalescing — Phi sources and Phi dsts are NOT given equal
    colors. The existing `from_ssa` lowers each Phi to one Copy per
    PhiArg in the predecessor, regardless of coloring. A future
    slice can do move-coalescing to eliminate redundant ZP-to-ZP
    copies.
  * Spill code emission — `Coloring.spilled` is informational. A
    future slice will route spilled names through the existing
    Frame allocation in `replace_pseudoregisters`.
  * The asm IR `ZP(addr, offset)` operand and any integration with
    `tac_to_asm` / `replace_pseudoregisters`.

`color_graph` requires its input function to be in SSA form. The
PEO derivation depends on every promotable Var having a single
defining block (the chordal property). No runtime assertion guards
this — callers are expected to invoke `color_graph` between
`to_ssa` and `from_ssa` in the optimizer driver.
"""

from __future__ import annotations

from dataclasses import dataclass, field

import tac_ast
from passes.optimization.cfg import (
    ENTRY_ID,
    build_cfg,
    dominator_tree_children,
    immediate_dominators,
)
from passes.optimization.interference import InterferenceGraph
from passes.optimization.pool import Pool
from passes.optimization.var_visit import defs_in


@dataclass
class Coloring:
    """Result of `color_graph`.

    `assignments` maps each successfully-colored Var name to its ZP
    base address (the lowest byte of its allocated `width`-byte slot).
    `spilled` lists names that were in the input graph but couldn't
    fit any pool. `pool` echoes the configuration used."""
    assignments: dict[str, int] = field(default_factory=dict)
    spilled: set[str] = field(default_factory=set)
    pool: Pool = field(default_factory=Pool)


def color_graph(
    fn: tac_ast.Function,
    graph: InterferenceGraph,
    *,
    pool: Pool | None = None,
) -> Coloring:
    """Color `graph`'s nodes onto ZP byte addresses drawn from `pool`.
    Returns a `Coloring` with every graph node either in `assignments`
    or in `spilled`. Names not present in the graph (statics, function
    names, address-taken locals filtered by interference construction)
    aren't represented in either."""
    if pool is None:
        pool = Pool()
    peo = _perfect_elimination_order(fn, graph)
    assignments: dict[str, int] = {}
    spilled: set[str] = set()
    for name in peo:
        node = graph.nodes[name]
        blocked = _blocked_bytes(name, graph, assignments)
        if node.lives_across_call:
            # Cross-call values must go to callee-saved (which the
            # function's prologue/epilogue will save+restore around
            # this function's body) or spill. Caller-saved would be
            # clobbered by the call.
            base = _find_fit(pool.callee_saved(), node.width, blocked)
        else:
            # Non-cross-call: prefer caller-saved (no
            # prologue/epilogue overhead), fall back to callee-saved
            # for fit (one extra save+restore in the frame).
            base = _find_fit(pool.caller_saved(), node.width, blocked)
            if base is None:
                base = _find_fit(pool.callee_saved(), node.width, blocked)
        if base is None:
            spilled.add(name)
        else:
            assignments[name] = base
    return Coloring(assignments=assignments, spilled=spilled, pool=pool)


def _blocked_bytes(
    name: str,
    graph: InterferenceGraph,
    assignments: dict[str, int],
) -> set[int]:
    """Bytes occupied by every already-colored neighbor of `name`."""
    out: set[int] = set()
    for m in graph.neighbors(name):
        base = assignments.get(m)
        if base is None:
            # Neighbor uncolored or spilled — no constraint on us
            # (a spilled neighbor lives in the frame, not a ZP slot).
            continue
        w = graph.nodes[m].width
        out.update(range(base, base + w))
    return out


def _find_fit(
    byte_range: range, width: int, blocked: set[int],
) -> int | None:
    """Lowest base in `byte_range` such that `[base, base+width)` is
    fully inside the range and disjoint from `blocked`. Returns None
    if no such base exists."""
    lo, hi = byte_range.start, byte_range.stop
    if width <= 0 or hi - lo < width:
        return None
    for base in range(lo, hi - width + 1):
        if any(b in blocked for b in range(base, base + width)):
            continue
        return base
    return None


def _perfect_elimination_order(
    fn: tac_ast.Function, graph: InterferenceGraph,
) -> list[str]:
    """Build a PEO over `graph`'s nodes via dominator-tree pre-order
    walk of value definitions, then reverse.

    Within each block: Phi dsts first (parallel-defined at block
    entry, forming a clique), then non-Phi defs in source order.
    Names not in `graph.nodes` are skipped (the interference builder
    filtered them out — statics, function names, etc.).

    Parameters and other graph nodes never defined by an instruction
    are appended to the build list AFTER the dom-tree walk, so they
    appear FIRST in the reversed PEO and are colored first. This
    matches their conceptual position at the implicit ENTRY point,
    dominating every other definition."""
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

    # Iterative dom-tree pre-order from ENTRY.
    stack: list[int] = [ENTRY_ID]
    while stack:
        bid = stack.pop()
        blk = cfg.blocks.get(bid)
        if blk is not None:
            for instr in blk.instructions:
                if isinstance(instr, tac_ast.Phi) and isinstance(
                    instr.dst, tac_ast.Var,
                ):
                    emit(instr.dst.name)
            for instr in blk.instructions:
                if isinstance(instr, tac_ast.Phi):
                    continue
                for d in defs_in(instr):
                    emit(d.name)
        for c in reversed(children.get(bid, [])):
            stack.append(c)

    # Append any remaining nodes (params, leftover SSA names whose
    # only "definition" was the implicit function entry). Params
    # first in declared order, then alphabetic for determinism.
    leftover = [n for n in graph.nodes if n not in seen]
    param_set = set(fn.params)
    head_params = [p for p in fn.params if p in graph.nodes and p not in seen]
    head_other = sorted(n for n in leftover if n not in param_set)
    build.extend(head_params)
    build.extend(head_other)

    # PEO is the reverse of the dom-tree-preorder build: dominators
    # eliminate LAST, so they're colored FIRST when iterating PEO.
    build.reverse()
    return build
