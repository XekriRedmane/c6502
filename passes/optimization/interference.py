"""Interference graph construction for register allocation.

Two `Var`s interfere when both are live at the same program point.
The graph is built by walking each block in reverse, maintaining a
"currently live" set initialized to `live_out[B]`; at each
instruction's def, edge that def against everyone currently live;
then remove the def from live; then add the instruction's uses to
live. Standard textbook algorithm.

Phi nodes are special:
  * All Phi dsts in a block are conceptually defined simultaneously
    at block entry (parallel to one another, before the first non-
    Phi instruction). So sibling Phi dsts in one block all interfere
    with each other AND with everything live just before the first
    non-Phi instruction.
  * Phi sources interfere on the *predecessor* side. This is handled
    implicitly by the liveness pass — `Liveness` already attributes
    each PhiArg.source to its matching predecessor's `live_out`. So
    when we walk a predecessor backward, the Phi sources are part of
    the initial `live` set and accumulate the right interferences
    automatically.

Each node carries:
  * `width` (1, 2, 4, or 8 bytes) — read from the symbol table via
    `passes.replace_pseudoregisters.size_of_name`. Names with no
    symbol-table entry default to 1 byte (matches `size_of_name`'s
    backstop, used by synthetic test ASTs).
  * `lives_across_call` — True iff the value is in `live` just before
    any `FunctionCall` / `IndirectCall` instruction (post-def-removed
    state). Drives the future caller/callee-saved ZP pool decision.

Nodes are dropped (NOT added to the graph) when:
  * the name resolves to a non-`LocalAttr` symbol (statics, function
    names) — register allocation never colors these; or
  * the width is 0 — defensive, no current type produces this.
"""

from __future__ import annotations

from dataclasses import dataclass, field

import tac_ast
from passes.optimization.liveness import Liveness
from passes.optimization.var_visit import defs_in, uses_in
from passes.replace_pseudoregisters import size_of_name
from passes.type_checking import LocalAttr, SymbolTable


@dataclass
class InterferenceNode:
    name: str
    width: int
    lives_across_call: bool = False


@dataclass
class InterferenceGraph:
    nodes: dict[str, InterferenceNode] = field(default_factory=dict)
    adj: dict[str, set[str]] = field(default_factory=dict)

    def neighbors(self, name: str) -> set[str]:
        return self.adj.get(name, set())

    def has_edge(self, a: str, b: str) -> bool:
        return b in self.adj.get(a, set())

    def add_edge(self, a: str, b: str) -> None:
        if a == b:
            return
        self.adj.setdefault(a, set()).add(b)
        self.adj.setdefault(b, set()).add(a)


def build_interference(
    fn: tac_ast.Function,
    liveness: Liveness,
    symbols: SymbolTable,
) -> InterferenceGraph:
    """Build the interference graph for `fn`. Width and the
    `lives_across_call` bit are set on each retained node."""
    graph = InterferenceGraph()

    def ensure_node(name: str) -> None:
        if name not in graph.nodes:
            graph.nodes[name] = InterferenceNode(
                name=name, width=0, lives_across_call=False,
            )

    for bid, blk in liveness.cfg.blocks.items():
        live: set[str] = set(liveness.live_out[bid])

        # Reverse walk through non-Phi instructions, in source-order
        # reversed.
        for i in range(len(blk.instructions) - 1, -1, -1):
            instr = blk.instructions[i]
            if isinstance(instr, tac_ast.Phi):
                # Defer Phi handling until the non-Phi walk completes.
                continue
            for d in defs_in(instr):
                ensure_node(d.name)
                for n in live:
                    if n != d.name:
                        graph.add_edge(d.name, n)
                live.discard(d.name)
            # `live` now reflects "what's live immediately before this
            # instruction's defs but before we add its uses". For a
            # call, that's the set of values that LIVE ACROSS the
            # call (every one of them is alive both after the call,
            # since they were in live_after, and before the call,
            # since the call's def — which would be the only thing
            # that could kill them across this point — has been
            # removed).
            if isinstance(instr, (tac_ast.FunctionCall, tac_ast.IndirectCall)):
                for n in live:
                    if n in graph.nodes:
                        graph.nodes[n].lives_across_call = True
            for u in uses_in(instr):
                ensure_node(u.name)
                live.add(u.name)

        # Phis at block entry: collect all dsts (parallel def at block
        # top), then edge every dst against every live name and
        # against every other dst.
        phi_dsts: list[str] = []
        for instr in blk.instructions:
            if not isinstance(instr, tac_ast.Phi):
                continue
            if isinstance(instr.dst, tac_ast.Var):
                phi_dsts.append(instr.dst.name)
                ensure_node(instr.dst.name)
        # Sibling Phi dsts interfere with each other.
        for i, a in enumerate(phi_dsts):
            for b in phi_dsts[i + 1:]:
                graph.add_edge(a, b)
        # And with everything live just before the first non-Phi
        # instruction (= current `live` after the reverse walk).
        for d in phi_dsts:
            for n in live:
                if n != d:
                    graph.add_edge(d, n)

    # Annotate widths and filter out non-LocalAttr / zero-width nodes.
    keep: dict[str, InterferenceNode] = {}
    for name, node in graph.nodes.items():
        sym = symbols.get(name) if symbols is not None else None
        if sym is not None and not isinstance(sym.attrs, LocalAttr):
            # Non-locals (statics, functions) are addressed by name,
            # not allocated to a register.
            continue
        node.width = size_of_name(name, symbols)
        if node.width <= 0:
            continue
        keep[name] = node

    # Drop edges whose endpoints were filtered out.
    for name in list(graph.adj.keys()):
        if name not in keep:
            del graph.adj[name]
            continue
        graph.adj[name] = {n for n in graph.adj[name] if n in keep}
    graph.nodes = keep

    return graph
