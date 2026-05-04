"""Interference graph data types — shared by the asm-level register
allocator (`passes/optimization_asm/regalloc.py`).

Two `Var`s interfere when both are live at the same program point.
The graph is a per-name mapping plus an undirected adjacency dict.
Nodes carry a `width` (1, 2, 4, or 8 bytes) and a
`lives_across_call` flag — the asm-level allocator uses both to
decide between caller-saved and callee-saved ZP pools and to size
the contiguous slot for multi-byte values.

The construction logic (which walks a function and produces an
`InterferenceGraph`) lives in
`passes/optimization_asm/interference.py` — that pass is the only
consumer now that the TAC-level register allocator has been
removed. This file just owns the data classes so both layers can
agree on the type vocabulary.
"""

from __future__ import annotations

from dataclasses import dataclass, field


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
