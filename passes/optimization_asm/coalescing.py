"""Move coalescing for the asm-level interference graph.

Standard SSA-era register-allocation transformation: when two
SSA-renamed Pseudo names are connected by a Mov (or by a Phi —
each PhiArg.source is move-related to the Phi.dst) and they
don't interfere in the interference graph, they CAN be given
the same color without compromising the coloring's
correctness. Doing so eliminates a copy (the Mov collapses to a
self-Mov at apply_coloring time, which the emit peephole drops).

The canonical motivating case is a loop counter:

    .preheader:
        Mov(Imm(0), %i.v0)            # init
    .loop_top:
        Phi(%i.phi, [(.preheader, %i.v0), (.continue, %i.v_post)])
        ... use %i.phi ...
    .continue:
        Mov(%i.phi, A); CLC; Add(Imm(1), A); Mov(A, %i.v_post)
        Jump(.loop_top)

After SSA destruction the Phi becomes per-edge Movs:

    .preheader:
        Mov(Imm(0), %i.v0)
        Mov(%i.v0, %i.phi)            # edge .preheader → .loop_top
        ...
    .continue:
        Mov(...) ... Mov(A, %i.v_post)
        Mov(%i.v_post, %i.phi)        # edge .continue → .loop_top

If %i.v0, %i.phi, %i.v_post all coalesce to one ZP slot, every
inserted Mov becomes a self-Mov (`Mov(ZP($X), ZP($X))`) that
asm_emit's `_emit_mov` self-Mov peephole drops. The increment
becomes a true in-place RMW on $X — which the multi-byte INC
peephole then collapses to `INC $X; BNE done; INC $X+1; done:`
when applicable.

# Algorithm

Aggressive (Chaitin-style) coalescing without the conservative
degree check. The c6502 ZP pool has 128 byte slots (default
`Pool(start=0x80)`); coalescing increases a node's neighbor
count but rarely past the pool size, so spills aren't a
concern.

Steps:

  1. Walk `fn.instructions` and collect move-related pairs:
       * `Mov(Pseudo a, Pseudo b)` — explicit copy.
       * Each `(Phi.dst, PhiArg.source)` pair where both are
         Pseudos — Phi destruction would emit a Mov for this.
  2. Use union-find to track equivalence classes.
  3. For each pair (a, b) in some order:
       * Look up class representatives a_rep, b_rep.
       * Skip if already merged (a_rep == b_rep).
       * Skip if either isn't a colorable graph node (statics,
         address-taken, params).
       * Skip if a_rep and b_rep have different widths (the
         coloring pool's slot search assumes uniform width).
       * Skip if a_rep and b_rep have an interference edge —
         coalescing would force them to share a color, which
         can't be correct.
       * Otherwise: merge b_rep into a_rep. The merged node
         inherits b_rep's neighbors (excluding a_rep itself),
         its `lives_across_call` flag is OR'd, and b_rep is
         removed from the graph.
  4. Returns a `name → representative` map covering every
     non-self-representative name. The caller projects coloring
     assignments through this map.

# Soundness

Merging two non-interfering nodes never introduces new
interference: each member of N_a was already conflicting with
A's color (or unassigned); after merge they conflict with the
merged node which has the same color. Same for N_b. The merged
node's degree is `|N_a ∪ N_b|`, which can be higher than either
input but doesn't exceed `|N_a| + |N_b|`.

Width / lives_across_call compatibility is handled by the
filter rules above. Spills are theoretically possible if
coalescing creates a node whose degree exceeds the available
pool, but practically the c6502 pool is large enough that this
doesn't happen on any program in the corpus. If it does, the
spill falls back through the existing `Coloring.spilled`
mechanism — graceful degradation.
"""

from __future__ import annotations

from dataclasses import dataclass, field

import asm_ast
from passes.optimization.interference import InterferenceGraph


@dataclass
class CoalesceResult:
    """Outcome of `coalesce_moves`. The interference graph has been
    mutated in place — coalesced non-representative nodes have been
    removed and their edges redirected to the representative.

    `representative` maps every coalesced (non-self-rep) name to its
    rep. Names not in the map are graph nodes unchanged by
    coalescing. Use `resolve(name)` to get the final coloring slot
    for any name (it walks the chain to the rep)."""
    representative: dict[str, str] = field(default_factory=dict)

    def resolve(self, name: str) -> str:
        """Return `name`'s representative (transitively). Returns
        `name` itself if it isn't coalesced."""
        cur = name
        while cur in self.representative:
            cur = self.representative[cur]
        return cur


def coalesce_moves(
    fn: asm_ast.Function,
    graph: InterferenceGraph,
) -> CoalesceResult:
    """Coalesce move-related Pseudo pairs in `graph` (mutates the
    graph in place). Returns a `CoalesceResult` whose
    `representative` map projects every coalesced non-rep name to
    its rep — the caller uses this to expand the post-coloring
    assignments back over every original SSA name."""
    pairs = list(_move_related_pairs(fn))
    rep_map: dict[str, str] = {}

    def find(name: str) -> str:
        # Path compression via the rep_map.
        path = []
        while name in rep_map:
            path.append(name)
            name = rep_map[name]
        for p in path:
            rep_map[p] = name
        return name

    for a, b in pairs:
        a_rep = find(a)
        b_rep = find(b)
        if a_rep == b_rep:
            continue
        if a_rep not in graph.nodes or b_rep not in graph.nodes:
            # One or both excluded from the graph (statics, address-
            # taken, params). Coalescing isn't representable.
            continue
        if graph.nodes[a_rep].width != graph.nodes[b_rep].width:
            continue
        if graph.has_edge(a_rep, b_rep):
            continue
        # Merge b_rep into a_rep.
        _merge(graph, dst=a_rep, src=b_rep)
        rep_map[b_rep] = a_rep
    return CoalesceResult(representative=rep_map)


def _move_related_pairs(
    fn: asm_ast.Function,
):
    """Yield `(name_a, name_b)` pairs where the two names are
    connected by a move relation. Sources:

      * `Mov(Pseudo, Pseudo)` — explicit Pseudo-to-Pseudo copy.
      * Each `(Phi.dst, PhiArg.source)` pair where both are
        Pseudos — SSA destruction would emit a Mov for this.

    Pairs with `offset != 0` Pseudos are skipped: those reference
    an unrenamed multi-byte name (typically the unsplit param
    bytes that asm-SSA leaves alone), and the coloring layer
    doesn't byte-rewrite them."""
    for instr in fn.instructions:
        if isinstance(instr, asm_ast.Mov):
            src, dst = instr.src, instr.dst
            if (
                isinstance(src, asm_ast.Pseudo)
                and isinstance(dst, asm_ast.Pseudo)
                and src.offset == 0 and dst.offset == 0
                and src.name != dst.name
            ):
                yield (src.name, dst.name)
        elif isinstance(instr, asm_ast.Phi):
            dst = instr.dst
            if not (
                isinstance(dst, asm_ast.Pseudo) and dst.offset == 0
            ):
                continue
            for arg in instr.args:
                src = arg.source
                if (
                    isinstance(src, asm_ast.Pseudo)
                    and src.offset == 0
                    and src.name != dst.name
                ):
                    yield (dst.name, src.name)


def _merge(
    graph: InterferenceGraph, *, dst: str, src: str,
) -> None:
    """Merge `src` into `dst` in place. `src`'s neighbors become
    `dst`'s neighbors; `src` is removed from `graph.nodes` and
    `graph.adj`. The merged node's `lives_across_call` flag is the
    OR of the two inputs."""
    src_neighbors = graph.adj.get(src, set())
    for n in src_neighbors:
        # Drop the (n, src) back-edge.
        if n in graph.adj:
            graph.adj[n].discard(src)
        # Add (dst, n) and (n, dst), unless the merge would create
        # a self-loop (n == dst) — the existing has_edge check above
        # rules out this case for the coalescing root, but a chain
        # of merges could produce one transitively.
        if n != dst:
            graph.adj.setdefault(dst, set()).add(n)
            graph.adj.setdefault(n, set()).add(dst)
    graph.adj.pop(src, None)
    if graph.nodes[src].lives_across_call:
        graph.nodes[dst].lives_across_call = True
    graph.nodes.pop(src, None)
