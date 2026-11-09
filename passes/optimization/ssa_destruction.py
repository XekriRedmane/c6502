"""TAC SSA destruction.

Converts every `Phi` instruction back to `Copy` instructions at the
end of each predecessor block. After this pass no Phi remains in
the function and the result is regular non-SSA TAC, ready for
`tac_to_asm` (which has no Phi lowering).

For each Phi in block M:
    Phi(dst, args=[(pred_label_1, src_1), ..., (pred_label_n, src_n)])
emit one `Copy(src_k, dst)` at the end of pred_k's block, immediately
before its terminator (`Ret` / `Jump` / `JumpIfTrue` / `JumpIfFalse`)
or at the end of the instruction list if there is no terminator.

**Parallel-copy ordering.** Multiple Phis at the same block produce
multiple Copies at each predecessor. Naive source-order emission can
read a slot that an earlier Copy just wrote — the classic "lost
copy" problem. Example after copy propagation:

    Phi(@2.i.2, [..., (continue, %4.1)])             ; i = %4.1
    Phi(@1.counter.1, [..., (continue, @2.i.2)])     ; counter = i

Source-order emit at end of `continue`:
    Copy(%4.1, @2.i.2)         ; @2.i.2 := %4.1   (writes @2.i.2)
    Copy(@2.i.2, @1.counter.1) ; reads @2.i.2 — but it's been
                                 ; overwritten with the new value!

The fix is **topological sort**: emit a Copy whose dst isn't read by
any other pending Copy first. The two Copies above swap order:

    Copy(@2.i.2, @1.counter.1) ; counter := old @2.i.2
    Copy(%4.1, @2.i.2)         ; now overwrite @2.i.2

**Cycles** (mutually-dependent Copies — e.g. `a, b = b, a`) need a
fresh temp to break. When `from_ssa` is called with a `symbols`
table, it mints a fresh `<funcname>.cycle_tmp@<N>` Var per cycle,
registers its type in the symbol table (matching the cycle
member's type), and rewrites the cycle as `tmp = first; first =
second; ... last = tmp`. Without `symbols` (legacy callers), cycles
fall back to source-order emission with a structural assert; the
MVP optimizer always passes symbols, so this fallback is for
backward compatibility only.

The minted cycle-temp Var has no coloring entry (regalloc has
already run), so `replace_pseudoregisters` lays it down as a Frame
slot — correct, just slower than a ZP-resident copy. Cycles are
rare in well-typed C code; the slow path is acceptable.
"""

from __future__ import annotations

import dataclasses
from collections import defaultdict

import tac_ast
from passes.optimization.cfg import (
    BasicBlock,
    build_cfg,
    cfg_to_function,
)
from passes.type_checking import LocalAttr, Symbol, SymbolTable
from passes.optimization.var_visit import defs_in, uses_in


_TERMINATOR_TYPES: tuple[type, ...] = (
    tac_ast.Ret,
    tac_ast.Jump,
    tac_ast.JumpIfTrue,
    tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp,
    tac_ast.JumpIfMasked,
)


def from_ssa(
    fn: tac_ast.Function,
    *,
    symbols: SymbolTable | None = None,
) -> tac_ast.Function:
    """Lower every Phi to Copies in predecessor blocks; remove all
    Phis from the function. Returns the rewritten Function.

    `symbols` is required for cycle-breaking — when a parallel-copy
    cycle is detected, the helper mints a fresh temp Var and
    registers its type in the table. Cycles without `symbols`
    fall back to source-order emission (and may miscompile)."""
    cfg = build_cfg(fn)
    label_to_block: dict[str, BasicBlock] = {
        b.instructions[0].name: b
        for b in cfg.blocks.values()
        if b.instructions and isinstance(b.instructions[0], tac_ast.Label)
    }
    cycle_counter = [0]  # boxed so the helper can mutate it
    live_in = _live_in(cfg)

    # One bucket of Copies per CFG edge (pred block, Phi block).
    edge_copies: dict[tuple[int, int], list[tac_ast.Copy]] = defaultdict(list)
    for bid in cfg.block_order:
        blk = cfg.blocks[bid]
        for phi in blk.instructions:
            if not isinstance(phi, tac_ast.Phi):
                continue
            for arg in phi.args:
                pred_blk = label_to_block.get(arg.pred_label)
                if pred_blk is None:
                    # Predecessor block was removed (e.g., by an earlier
                    # pass dropping unreachable code); its Phi argument
                    # is dead. Skip silently.
                    continue
                edge_copies[(pred_blk.id, bid)].append(
                    tac_ast.Copy(src=arg.source, dst=phi.dst),
                )

    split_counter = 0
    for (pid, sid), copies in edge_copies.items():
        pred_blk = cfg.blocks[pid]
        ordered = _order_parallel_copies(
            copies, fn_name=fn.name,
            cycle_counter=cycle_counter, symbols=symbols,
        )
        last = pred_blk.instructions[-1] if pred_blk.instructions else None
        if not _needs_split(pid, sid, copies, last, cfg, edge_copies, live_in):
            insert_pos = len(pred_blk.instructions)
            if isinstance(last, _TERMINATOR_TYPES):
                insert_pos -= 1
            pred_blk.instructions[insert_pos:insert_pos] = ordered
            continue
        succ_label = cfg.blocks[sid].instructions[0].name
        if last.target != succ_label:
            # The Phi block is the fall-through successor: the copies
            # go after the conditional jump, where only the
            # fall-through path runs them.
            pred_blk.instructions.extend(ordered)
            continue
        # The Phi block is the jump target: route the jump through a
        # new block that runs the copies and jumps on.
        split_counter += 1
        split_label = f".{fn.name}@ssa_split@{split_counter}"
        pred_blk.instructions[-1] = dataclasses.replace(last, target=split_label)
        new_id = max(cfg.blocks) + 1
        cfg.blocks[new_id] = BasicBlock(
            id=new_id,
            instructions=[tac_ast.Label(name=split_label), *ordered,
                          tac_ast.Jump(target=succ_label)],
        )
        cfg.block_order.append(new_id)

    for bid in cfg.block_order:
        blk = cfg.blocks[bid]
        blk.instructions = [
            i for i in blk.instructions if not isinstance(i, tac_ast.Phi)
        ]

    return cfg_to_function(fn, cfg)


_CONDITIONAL = (
    tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp, tac_ast.JumpIfMasked,
)


def _needs_split(pid, sid, copies, last, cfg, edge_copies, live_in) -> bool:
    """True iff the copies for edge pid -> sid can't simply go in
    front of pid's terminator.

    They can when pid has one successor. When it ends in a conditional
    jump (a critical edge if sid has other predecessors), copies in
    front of the jump also run on the other edge, which is only
    harmless when none of their destinations is read by the jump or
    live into the other successor, and the other edge has no copies
    of its own. Loop rotation makes this case common: the bottom test
    of a rotated loop feeds Phis at both the loop header and the
    loop exit, and the header's `i = i + 1` copy would otherwise
    clobber the `i` the exit reads."""
    if not isinstance(last, _CONDITIONAL):
        return False
    others = [t for t in cfg.blocks[pid].successors if t != sid]
    if not others:
        return False
    dsts = {c.dst.name for c in copies}
    if any(v.name in dsts for v in uses_in(last)):
        return True
    for t in others:
        if (pid, t) in edge_copies:
            return True
        if dsts & live_in.get(t, set()):
            return True
    return False


def _live_in(cfg) -> dict[int, set[str]]:
    """Var names live on entry to each block. A Phi's destination is
    defined at its block's entry, and a Phi argument counts as a use
    at the end of the predecessor it comes from."""
    label_of = {
        b.instructions[0].name: bid for bid, b in cfg.blocks.items()
        if b.instructions and isinstance(b.instructions[0], tac_ast.Label)
    }
    use: dict[int, set[str]] = {}
    defs: dict[int, set[str]] = {}
    phi_uses: dict[int, set[str]] = defaultdict(set)   # live-out of pred
    for bid, blk in cfg.blocks.items():
        u: set[str] = set()
        d: set[str] = set()
        for ins in blk.instructions:
            if isinstance(ins, tac_ast.Phi):
                d.add(ins.dst.name)
                for arg in ins.args:
                    pb = label_of.get(arg.pred_label)
                    if pb is not None and isinstance(arg.source, tac_ast.Var):
                        phi_uses[pb].add(arg.source.name)
                continue
            for v in uses_in(ins):
                if v.name not in d:
                    u.add(v.name)
            for v in defs_in(ins):
                d.add(v.name)
        use[bid], defs[bid] = u, d
    live: dict[int, set[str]] = {bid: set() for bid in cfg.blocks}
    changed = True
    while changed:
        changed = False
        for bid, blk in cfg.blocks.items():
            out = set(phi_uses[bid])
            for sid in blk.successors:
                out |= live[sid]
            new = use[bid] | (out - defs[bid])
            if new != live[bid]:
                live[bid] = new
                changed = True
    return live


def _order_parallel_copies(
    copies: list[tac_ast.Copy],
    *,
    fn_name: str,
    cycle_counter: list[int],
    symbols: SymbolTable | None,
) -> list[tac_ast.Copy]:
    """Topologically sort `copies` so that each Copy's dst is not
    read by any later Copy in the output. Equivalently: emit a Copy
    whose dst doesn't appear as the src of any other pending Copy
    first; remove it; repeat.

    When all remaining Copies form a cycle, break it by minting a
    fresh temp: pick any cycle member (src, dst), save dst's value
    to the temp, then rewrite whichever pending Copy reads dst as
    src to instead read the temp. This frees up dst as a non-cycle
    vertex; the algorithm then makes progress.

    `cycle_counter[0]` is a per-function fresh counter for temp
    names. `symbols` provides the type to register for the temp.
    Without `symbols`, falls back to source-order emission for
    cycles (may miscompile — pass `symbols` from the optimizer
    driver for soundness)."""
    if len(copies) <= 1:
        return list(copies)

    pending = list(copies)
    out: list[tac_ast.Copy] = []
    while pending:
        # Find a pending Copy whose dst is not the src of any other
        # pending Copy.
        ready_idx = None
        for i, c in enumerate(pending):
            if not isinstance(c.dst, tac_ast.Var):
                ready_idx = i
                break
            d = c.dst.name
            blocks_other = any(
                isinstance(other.src, tac_ast.Var) and other.src.name == d
                for j, other in enumerate(pending) if j != i
            )
            if not blocks_other:
                ready_idx = i
                break
        if ready_idx is not None:
            out.append(pending.pop(ready_idx))
            continue
        # All remaining Copies are in cycles. Break one by minting a
        # temp.
        if symbols is None:
            # Legacy fallback — emit in source order. Cycles will
            # miscompile but the structural shape is preserved.
            out.extend(pending)
            break
        # Pick any pending Copy. Save its dst into a fresh temp;
        # rewrite the (unique) other pending Copy that reads dst as
        # src to instead read the temp.
        chosen = pending[0]
        if not isinstance(chosen.dst, tac_ast.Var):
            # Defensive: shouldn't happen for SSA Phi-derived Copies.
            out.extend(pending)
            break
        cycle_counter[0] += 1
        tmp_name = f".{fn_name}@cycle_tmp@{cycle_counter[0]}"
        # Type the temp like its source value (the dst we're saving).
        sym = symbols.get(chosen.dst.name)
        if sym is not None:
            symbols[tmp_name] = Symbol(type=sym.type, attrs=LocalAttr())
        # Emit the save: temp = chosen.dst (the OLD value of dst).
        out.append(tac_ast.Copy(
            src=tac_ast.Var(name=chosen.dst.name),
            dst=tac_ast.Var(name=tmp_name),
        ))
        # Rewrite the (one) pending Copy whose src is chosen.dst to
        # instead read from the temp. Now chosen.dst is no longer a
        # src of any pending Copy.
        d = chosen.dst.name
        for j, other in enumerate(pending):
            if (
                j != 0
                and isinstance(other.src, tac_ast.Var)
                and other.src.name == d
            ):
                pending[j] = tac_ast.Copy(
                    src=tac_ast.Var(name=tmp_name), dst=other.dst,
                )
        # Loop continues; chosen Copy is now eligible for emission.
    return out
