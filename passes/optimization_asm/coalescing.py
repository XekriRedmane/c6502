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

# Carry bytes

After the move pairs, a second round looks at each carry-propagating
byte `Mov(P_a, A); Add(Imm(0), A); Mov(A, P_b)`. That's the high
byte of an add whose other operand's high byte is zero, such as
`t + (uint16_t)c` or `t + 53`, or the same with `Sub(Imm(0), A)`
for a subtract's borrow. When P_a and P_b share a byte, the peephole
in `passes/carry_inc.py` rewrites the three instructions to `BCC
skip; INC P; skip:` (or `BCS skip; DEC P; skip:`), the way hand-
written 6502 code adds a byte to a 16-bit value. These pairs come after the move pairs, so they
never take a slot away from a copy the move pairs would remove.

The same round also takes each byte copy that the move pairs skip
because one side is a byte of a multi-byte name, such as a pointer
used as a `(zp),Y` base being advanced through 1-byte SSA temps at
the bottom of a loop.

P_a is often byte k of a multi-byte name. A constant shift is
lowered to ASL / ROL on its destination in place, and asm-SSA keeps
any name that's the target of such a read-modify-write as one
contiguous block. P_b, a 1-byte SSA name, then merges into that
block's node at byte k, and `CoalesceResult.byte_offset` records k
so the coloring projection gives P_b the block's address plus k.
That's sound for the same reason as an ordinary merge, since there
is no interference edge between P_b and any byte of the block, and
P_b's neighbors become neighbors of the whole block.

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
    # Byte position of a name inside its parent's storage, for a
    # 1-byte name merged into one byte of a multi-byte node (see
    # "Carry bytes" in the module docstring). A name missing here
    # sits at offset 0 of its parent.
    byte_offset: dict[str, int] = field(default_factory=dict)

    def resolve(self, name: str) -> str:
        """Return `name`'s representative (transitively). Returns
        `name` itself if it isn't coalesced."""
        cur = name
        while cur in self.representative:
            cur = self.representative[cur]
        return cur

    def offset_of(self, name: str) -> int:
        """Returns the byte position of `name` inside its
        representative's storage, summed along the chain."""
        total = 0
        cur = name
        while cur in self.representative:
            total += self.byte_offset.get(cur, 0)
            cur = self.representative[cur]
        return total


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
    # Every graph name's width, taken before any merge removes names
    # from the graph. The byte-level check has to see every member of
    # a coalesced class, including those merged by the move pairs.
    widths = {n: node.width for n, node in graph.nodes.items()}
    rep_map: dict[str, str] = {}
    offsets: dict[str, int] = {}

    def find(name: str) -> str:
        # Path compression via the rep_map. Each compressed link
        # keeps the byte offset summed along the path it replaces.
        path = []
        while name in rep_map:
            path.append(name)
            name = rep_map[name]
        total = 0
        for p in reversed(path):
            total += offsets.get(p, 0)
            rep_map[p] = name
            if total:
                offsets[p] = total
            else:
                offsets.pop(p, None)
        return name

    def offset(name: str) -> int:
        find(name)
        return offsets.get(name, 0)

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
    merger = _ByteMerger(fn, graph, find, offset, rep_map, offsets, widths)
    for src, dst in _byte_copy_pairs(fn):
        merger.coalesce(src, dst)
    for src, dst in _carry_byte_pairs(fn):
        merger.coalesce(src, dst)
    return CoalesceResult(
        representative=rep_map, byte_offset=dict(offsets),
    )


class _ByteMerger:
    """Merges byte-copy and carry-byte pairs (see "Carry bytes" in the
    module docstring), tracking which byte of its representative each
    coalesced name occupies.

    The interference graph is per name, and a multi-byte name is live
    while any of its bytes is. So a 1-byte name defined while the
    other byte of a pointer is still to be read has an edge to the
    whole pointer, even when the byte it would share is already dead.
    When such an edge blocks a merge into a multi-byte node,
    `_byte_edges` decides it at byte granularity instead: every byte of a multi-byte name is renamed to
    its own name, liveness is recomputed, and the merge goes ahead
    when no two names that would share a byte are live at once."""

    def __init__(self, fn, graph, find, offset, rep_map, offsets, widths):
        self.fn = fn
        self.graph = graph
        self.find = find
        self.offset = offset
        self.rep_map = rep_map
        self.offsets = offsets
        self._edges: set[frozenset[str]] | None = None
        # Widths of every name that was in the graph before the move
        # pairs merged any. A class's members are found among these.
        self._widths = dict(widths)

    def coalesce(self, src: asm_ast.Pseudo, dst: asm_ast.Pseudo) -> None:
        """Puts the bytes `src` and `dst` in one byte of storage when
        that's sound, by merging one node into the other at the offset
        that lines the two bytes up. The merged node has to fit inside
        the one it joins."""
        graph = self.graph
        s_rep, d_rep = self.find(src.name), self.find(dst.name)
        if s_rep == d_rep:
            return
        if s_rep not in graph.nodes or d_rep not in graph.nodes:
            return
        s_width = graph.nodes[s_rep].width
        d_width = graph.nodes[d_rep].width
        # Byte position of each operand inside its representative.
        s_pos = self.offset(src.name) + src.offset
        d_pos = self.offset(dst.name) + dst.offset
        # Merging `small` into `big` puts small's byte j at big's byte
        # j + pos.
        if 0 <= s_pos - d_pos and s_pos - d_pos + d_width <= s_width:
            big, small, pos = s_rep, d_rep, s_pos - d_pos
        elif 0 <= d_pos - s_pos and d_pos - s_pos + s_width <= d_width:
            big, small, pos = d_rep, s_rep, d_pos - s_pos
        else:
            return
        if graph.has_edge(s_rep, d_rep):
            # A name-level edge is imprecise only through a multi-byte
            # name, which is live while any byte is.
            if graph.nodes[big].width == 1:
                return
            if self._conflicts(big, small, pos):
                return
        _merge(graph, dst=big, src=small)
        self.rep_map[small] = big
        if pos:
            self.offsets[small] = pos

    def _members(self, rep: str) -> list[tuple[str, int]]:
        """The original graph names coalesced into `rep`, each with its
        byte position in rep's storage."""
        return [
            (n, self.offset(n)) for n in self._widths
            if self.find(n) == rep
        ]

    def _bytes(self, rep: str, shift: int) -> dict[int, list[str]]:
        """Maps each byte position of `rep`'s storage, plus `shift`, to
        the byte-level names that occupy it."""
        out: dict[int, list[str]] = {}
        for n, p in self._members(rep):
            w = self._widths[n]
            for j in range(w):
                out.setdefault(p + j + shift, []).append(
                    n if w == 1 else f"{n}#{j}",
                )
        return out

    def _conflicts(self, big: str, small: str, pos: int) -> bool:
        """True iff a name in `small`'s class and a name in `big`'s
        class that would share a byte are live at the same time."""
        if self._edges is None:
            multi = {n for n, w in self._widths.items() if w > 1}
            self._edges = _byte_edges(self.fn, multi)
        big_bytes = self._bytes(big, 0)
        for k, names in self._bytes(small, pos).items():
            for a in names:
                for b in big_bytes.get(k, []):
                    if frozenset((a, b)) in self._edges:
                        return True
        return False


def _byte_edges(
    fn: asm_ast.Function, multi: set[str],
) -> set[frozenset[str]]:
    """Interference edges with every byte of each name in `multi`
    renamed to `<name>#<j>`, over every Pseudo in `fn`. A def the
    rename can't see, such as the implicit high-byte write of a
    `LoadAddress`, only makes that byte look live for longer, which
    adds edges and never removes one."""
    import dataclasses
    from passes.optimization_asm.liveness import (
        _defs_in, _uses_in, compute_liveness,
    )

    def rename(x):
        if isinstance(x, asm_ast.Pseudo):
            if x.name in multi:
                return asm_ast.Pseudo(name=f"{x.name}#{x.offset}", offset=0)
            return x
        if isinstance(x, list):
            return [rename(v) for v in x]
        if dataclasses.is_dataclass(x) and not isinstance(x, type):
            return dataclasses.replace(x, **{
                f.name: rename(getattr(x, f.name))
                for f in dataclasses.fields(x)
            })
        return x

    renamed = asm_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=[rename(i) for i in fn.instructions],
    )
    liveness = compute_liveness(renamed)
    edges: set[frozenset[str]] = set()
    for bid, blk in liveness.cfg.blocks.items():
        live = set(liveness.live_out[bid])
        for instr in reversed(blk.instructions):
            if isinstance(instr, asm_ast.Phi):
                continue
            for dv in _defs_in(instr):
                for n in live:
                    if n != dv.name:
                        edges.add(frozenset((dv.name, n)))
                live.discard(dv.name)
            for u in _uses_in(instr):
                live.add(u.name)
        phi_dsts = [
            i.dst.name for i in blk.instructions
            if isinstance(i, asm_ast.Phi)
            and isinstance(i.dst, asm_ast.Pseudo)
        ]
        for k, dst in enumerate(phi_dsts):
            for other in phi_dsts[k + 1:]:
                edges.add(frozenset((dst, other)))
            for n in live:
                if n != dst:
                    edges.add(frozenset((dst, n)))
    return edges


def _move_related_pairs(
    fn: asm_ast.Function,
):
    """Yield `(name_a, name_b)` pairs where the two names are
    connected by a move relation. Sources:

      * `Mov(Pseudo, Pseudo)` — explicit Pseudo-to-Pseudo copy.
      * Each `(Phi.dst, PhiArg.source)` pair where both are
        Pseudos — SSA destruction would emit a Mov for this.
      * Adjacent `Mov(Pseudo a, Reg(A)); Mov(Reg(A), Pseudo b)` —
        an A-routed copy from a to b. `tac_to_asm` always routes
        Pseudo↔Pseudo transfers through A (the 6502 has no direct
        mem-to-mem move at the IR level), so the indirect form is
        the typical shape after copy-prop / DCE settles. Coalescing
        the two ends lets the resulting `Mov(P, A); Mov(A, P)` (with
        P the merged color) collapse to a self-Mov dropped at emit.

    Pairs with `offset != 0` Pseudos are skipped: those reference
    an unrenamed multi-byte name (typically the unsplit param
    bytes that asm-SSA leaves alone), and the coloring layer
    doesn't byte-rewrite them.

    Volatile-flagged Movs aren't coalesce candidates — coalescing
    would merge the volatile Pseudo with a non-volatile temp under
    one name, and the resulting single coloring would let
    non-volatile uses share the volatile cell's storage. Whether
    that's incorrect depends on later passes' assumptions, but
    keeping them distinct is the safe call."""
    instrs = fn.instructions
    for instr in instrs:
        if isinstance(instr, asm_ast.Mov):
            if instr.is_volatile:
                continue
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
    # A-routed Pseudo↔Pseudo: adjacent `Mov(P_a, A); Mov(A, P_b)`.
    # The 6502 has no direct memory-to-memory move, so a logical
    # copy from one Pseudo to another lowers to a load-via-A then
    # a store-from-A. Coalescing these merges the two ends; after
    # apply_coloring the pair becomes a self-Mov dropped at emit.
    for i in range(len(instrs) - 1):
        a, b = instrs[i], instrs[i + 1]
        if not (isinstance(a, asm_ast.Mov)
                and isinstance(b, asm_ast.Mov)):
            continue
        if a.is_volatile or b.is_volatile:
            continue
        if not (isinstance(a.src, asm_ast.Pseudo)
                and a.src.offset == 0
                and isinstance(a.dst, asm_ast.Reg)
                and isinstance(a.dst.reg, asm_ast.A)):
            continue
        if not (isinstance(b.src, asm_ast.Reg)
                and isinstance(b.src.reg, asm_ast.A)
                and isinstance(b.dst, asm_ast.Pseudo)
                and b.dst.offset == 0):
            continue
        if a.src.name == b.dst.name:
            continue
        yield (a.src.name, b.dst.name)


def _byte_copy_pairs(fn: asm_ast.Function):
    """Yields `(src, dst)` Pseudo operands for each byte copy, a
    `Mov(Pseudo, Pseudo)` or an A-routed `Mov(Pseudo, A); Mov(A,
    Pseudo)`. The move pairs already took the copies between two
    1-byte names; what's left is typically a multi-byte pointer that
    asm-SSA leaves whole (it's the base of a `(zp),Y` access) being
    copied a byte at a time to or from 1-byte SSA names, as at the
    bottom of a loop that advances the pointer."""
    instrs = fn.instructions

    def wanted(a, b) -> bool:
        return (
            isinstance(a, asm_ast.Pseudo) and isinstance(b, asm_ast.Pseudo)
            and a.name != b.name
        )

    for instr in instrs:
        if (isinstance(instr, asm_ast.Mov) and not instr.is_volatile
                and wanted(instr.src, instr.dst)):
            yield (instr.src, instr.dst)
    for a, b in zip(instrs, instrs[1:]):
        if not (isinstance(a, asm_ast.Mov) and isinstance(b, asm_ast.Mov)):
            continue
        if a.is_volatile or b.is_volatile:
            continue
        if not (isinstance(a.dst, asm_ast.Reg)
                and isinstance(a.dst.reg, asm_ast.A)
                and isinstance(b.src, asm_ast.Reg)
                and isinstance(b.src.reg, asm_ast.A)):
            continue
        if wanted(a.src, b.dst):
            yield (a.src, b.dst)


def _carry_byte_pairs(fn: asm_ast.Function):
    """Yields `(src, dst)` Pseudo operands for each carry-propagating
    byte `Mov(src, A); Add(Imm(0), A); Mov(A, dst)`, the high byte of
    an add whose other operand's high byte is zero, and for the same
    shape with `Sub` (a subtract's borrow). Unlike the Mov
    pairs, `src` may be a byte of a multi-byte name (a shift temp
    that asm-SSA leaves whole because ASL / ROL rewrite it in
    place)."""
    instrs = fn.instructions
    for i in range(len(instrs) - 2):
        a, op, b = instrs[i], instrs[i + 1], instrs[i + 2]
        if not (isinstance(a, asm_ast.Mov)
                and isinstance(op, (asm_ast.Add, asm_ast.Sub))
                and isinstance(b, asm_ast.Mov)):
            continue
        if a.is_volatile or b.is_volatile:
            continue
        if not (op.src == asm_ast.Imm(value=0)
                and isinstance(op.dst, asm_ast.Reg)
                and isinstance(op.dst.reg, asm_ast.A)):
            continue
        if not (isinstance(a.src, asm_ast.Pseudo)
                and isinstance(a.dst, asm_ast.Reg)
                and isinstance(a.dst.reg, asm_ast.A)):
            continue
        if not (isinstance(b.src, asm_ast.Reg)
                and isinstance(b.src.reg, asm_ast.A)
                and isinstance(b.dst, asm_ast.Pseudo)):
            continue
        if a.src.name == b.dst.name:
            continue
        yield (a.src, b.dst)


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
