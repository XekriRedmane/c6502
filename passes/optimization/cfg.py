"""Control-flow graph for a TAC function.

A `CFG` is a directed graph of `BasicBlock`s plus two distinguished
nodes — ENTRY (where control begins) and EXIT (where it leaves the
function). Every block in the dict is identified by an int id;
`ENTRY_ID = 0` and `EXIT_ID = 1` are reserved for the two sentinels,
real blocks number from 2 onward.

Basic-block partitioning rules:
  - A new block starts at the function's first instruction, at every
    `Label` instruction, and at the instruction immediately following
    a terminator (`Ret` / `Jump` / `JumpIfTrue` / `JumpIfFalse`).
  - A block ends after its terminator (if it has one) or at the
    instruction before the next block's start (the fall-through case).
  - Consequently `Label` only ever appears as an instruction's first;
    `Ret` / `Jump` / `JumpIfTrue` / `JumpIfFalse` only ever appear as
    its last.

Successor edges from each real block:
  - `Ret`              → EXIT.
  - `Jump(L)`          → the block whose first instruction is `Label(L)`.
  - `JumpIfTrue(L)` /
    `JumpIfFalse(L)`   → both the labeled block (taken) and the
                          source-order next block (fall-through).
  - any other          → the source-order next block (fall-through).
  - the last block in
    source order with
    no terminator      → EXIT (defensive — `c99_to_tac` always
                          appends an implicit `Ret`, so this only
                          arises if a downstream pass strips it).
  - ENTRY              → the first real block, or EXIT for an empty
                          function body.

Unreachable blocks (no path from ENTRY) keep their instructions and
their outgoing edges; consumers like unreachable-code elimination
identify them by traversing forward from ENTRY and dropping anything
not visited.

`block_order` lists real-block ids in source order. Flattening a CFG
back to a `Function` walks `block_order` and emits each surviving
block's instructions in turn — see `cfg_to_function`.
"""

from __future__ import annotations

from dataclasses import dataclass, field

import tac_ast


ENTRY_ID = 0
EXIT_ID = 1


@dataclass
class BasicBlock:
    id: int
    instructions: list[tac_ast.Type_instruction] = field(default_factory=list)
    predecessors: list[int] = field(default_factory=list)
    successors: list[int] = field(default_factory=list)


@dataclass
class CFG:
    blocks: dict[int, BasicBlock]
    block_order: list[int]


_TERMINATOR_TYPES: tuple[type, ...] = (
    tac_ast.Ret,
    tac_ast.Jump,
    tac_ast.JumpIfTrue,
    tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp,
    tac_ast.JumpIfMasked,
)


def build_cfg(fn: tac_ast.Function) -> CFG:
    """Partition `fn`'s instructions into basic blocks and wire entry,
    exit, and inter-block edges. The returned CFG has two sentinel
    blocks at `ENTRY_ID` / `EXIT_ID` plus one block per partition,
    each numbered uniquely from 2 upward in source order."""
    blocks: dict[int, BasicBlock] = {
        ENTRY_ID: BasicBlock(id=ENTRY_ID),
        EXIT_ID: BasicBlock(id=EXIT_ID),
    }
    block_order: list[int] = []
    next_id = 2
    current: list[tac_ast.Type_instruction] = []

    def finalize() -> None:
        nonlocal next_id
        if not current:
            return
        bid = next_id
        next_id += 1
        blocks[bid] = BasicBlock(id=bid, instructions=list(current))
        block_order.append(bid)
        current.clear()

    for instr in fn.instructions:
        if isinstance(instr, tac_ast.Label):
            finalize()
            current.append(instr)
        elif isinstance(instr, _TERMINATOR_TYPES):
            current.append(instr)
            finalize()
        else:
            current.append(instr)
    finalize()

    label_to_block: dict[str, int] = {}
    for bid in block_order:
        first = blocks[bid].instructions[0]
        if isinstance(first, tac_ast.Label):
            label_to_block[first.name] = bid

    def add_edge(src: int, dst: int) -> None:
        blocks[src].successors.append(dst)
        blocks[dst].predecessors.append(src)

    if block_order:
        add_edge(ENTRY_ID, block_order[0])
    else:
        add_edge(ENTRY_ID, EXIT_ID)

    for i, bid in enumerate(block_order):
        last = blocks[bid].instructions[-1]
        next_bid = block_order[i + 1] if i + 1 < len(block_order) else EXIT_ID
        if isinstance(last, tac_ast.Ret):
            add_edge(bid, EXIT_ID)
        elif isinstance(last, tac_ast.Jump):
            add_edge(bid, label_to_block[last.target])
        elif isinstance(
            last,
            (
                tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
                tac_ast.JumpIfCmp, tac_ast.JumpIfMasked,
            ),
        ):
            add_edge(bid, label_to_block[last.target])
            add_edge(bid, next_bid)
        else:
            add_edge(bid, next_bid)

    return CFG(blocks=blocks, block_order=block_order)


def cfg_to_function(fn: tac_ast.Function, cfg: CFG) -> tac_ast.Function:
    """Flatten `cfg` back into a `tac_ast.Function`, preserving the
    function's name / linkage / parameters and emitting each real
    block's instructions in `block_order` order. Removing a block from
    `cfg.blocks` (or its id from `block_order`) drops its instructions
    on flatten — that's how unreachable-code elimination produces its
    output."""
    out: list[tac_ast.Type_instruction] = []
    for bid in cfg.block_order:
        if bid in cfg.blocks:
            out.extend(cfg.blocks[bid].instructions)
    return tac_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=out,
    )


# ---------------------------------------------------------------------------
# Dominance analysis
# ---------------------------------------------------------------------------
#
# Cooper, Harvey, Kennedy, "A Simple, Fast Dominance Algorithm" (2006).
# An iterative dataflow algorithm — converges in two passes over a
# reverse-postorder traversal for typical reducible CFGs and never
# more than O(N^2) for pathological cases. Plenty fast for c6502's
# function sizes.
#
# Definitions:
#   `B dominates A` iff every path from ENTRY to A passes through B.
#   `B = idom(A)` iff B dominates A and no other dominator of A
#                  (other than A itself) is dominated by B.
#   `DF(B)` (dominance frontier) is the set of blocks `X` such that
#                  B dominates a predecessor of X but does not strictly
#                  dominate X — i.e. the "exits" of B's dominance
#                  region. Phi nodes for a variable defined in B go at
#                  the iterated DF of B's definition blocks (Cytron
#                  et al. 1991).
#
# All functions exclude unreachable blocks from their result — only
# blocks reachable from ENTRY appear in the returned dicts.


def reverse_postorder(cfg: CFG) -> list[int]:
    """DFS-from-ENTRY postorder, reversed. The order each block
    appears at puts every dominator of a block before it, which is
    what makes Cooper's algorithm converge in one iteration on
    reducible flow graphs."""
    visited: set[int] = {ENTRY_ID}
    postorder: list[int] = []
    stack: list[tuple[int, list[int], int]] = [
        (ENTRY_ID, list(cfg.blocks[ENTRY_ID].successors), 0),
    ]
    while stack:
        bid, succs, i = stack[-1]
        if i >= len(succs):
            postorder.append(bid)
            stack.pop()
            continue
        stack[-1] = (bid, succs, i + 1)
        nxt = succs[i]
        if nxt not in visited:
            visited.add(nxt)
            stack.append((nxt, list(cfg.blocks[nxt].successors), 0))
    postorder.reverse()
    return postorder


def immediate_dominators(cfg: CFG) -> dict[int, int]:
    """Map each reachable block to its immediate dominator.
    `idom[ENTRY_ID] == ENTRY_ID` is a sentinel — ENTRY has no
    dominator outside itself."""
    rpo = reverse_postorder(cfg)
    rpo_index = {b: i for i, b in enumerate(rpo)}
    idom: dict[int, int] = {ENTRY_ID: ENTRY_ID}

    def intersect(a: int, b: int) -> int:
        finger1, finger2 = a, b
        while finger1 != finger2:
            while rpo_index[finger1] > rpo_index[finger2]:
                finger1 = idom[finger1]
            while rpo_index[finger2] > rpo_index[finger1]:
                finger2 = idom[finger2]
        return finger1

    changed = True
    while changed:
        changed = False
        for b in rpo:
            if b == ENTRY_ID:
                continue
            preds = [
                p for p in cfg.blocks[b].predecessors if p in rpo_index
            ]
            processed = [p for p in preds if p in idom]
            if not processed:
                continue
            new_idom = processed[0]
            for p in processed[1:]:
                new_idom = intersect(p, new_idom)
            if idom.get(b) != new_idom:
                idom[b] = new_idom
                changed = True
    return idom


def dominator_tree_children(idom: dict[int, int]) -> dict[int, list[int]]:
    """Invert `idom` to a parent → children map. ENTRY's self-edge
    is filtered out so a tree walker doesn't recurse forever."""
    children: dict[int, list[int]] = {b: [] for b in idom}
    for b, p in idom.items():
        if b == p:
            continue
        children[p].append(b)
    return children


def dominance_frontiers(cfg: CFG) -> dict[int, set[int]]:
    """Compute DF[B] for every reachable block B, via the standard
    Cytron walk: for each block X with multiple predecessors, every
    predecessor P contributes X to DF[runner] for runner = P,
    idom[P], idom[idom[P]], ... up to (but not including) idom[X]."""
    idom = immediate_dominators(cfg)
    df: dict[int, set[int]] = {b: set() for b in idom}
    for b in idom:
        if b == ENTRY_ID:
            continue
        preds = [p for p in cfg.blocks[b].predecessors if p in idom]
        if len(preds) < 2:
            continue
        for p in preds:
            runner = p
            while runner != idom[b]:
                df[runner].add(b)
                runner = idom[runner]
    return df
