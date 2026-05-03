"""Asm-level SSA destruction.

Lower every `Phi` instruction to `Mov` instructions on each
predecessor edge. After this pass no Phi remains in the function
and the result is regular non-SSA asm, ready for
`replace_pseudoregisters` (which has no Phi handling).

For each Phi in block M:
    Phi(dst, args=[(pred_label_1, src_1), ..., (pred_label_n, src_n)])
emit one `Mov(src_k, dst)` on the edge from pred_k to M.

**Critical-edge splitting.** Asm-level SSA differs from TAC SSA in
one important way: predecessors can end with a flag-sensitive
terminator (`Branch`), and a `Mov` between such a terminator and
the flag-setting instruction it depends on would clobber the flag
(LDA sets N/Z). To avoid that, every edge from a `Branch`-
terminated predecessor to a Phi-bearing merge is split by
inserting a fresh block on the edge — Movs go INTO the split
block, not into the predecessor. Edges from `Jump`-terminated
predecessors are safe to insert before the Jump (Jump doesn't read
flags), so they aren't split.

**Parallel-copy ordering.** Multiple Phis at the same block produce
multiple Movs on each edge. Naive source-order emission can read a
slot that an earlier Mov just wrote — the classic "lost copy"
problem. Mirrors the TAC `from_ssa` approach: topologically sort
each predecessor's parallel-Mov set so that a Mov whose dst isn't
read by any other pending Mov goes first.

**Cycles** (mutually-dependent Movs — e.g. `a, b = b, a`) are
broken by minting a fresh `.<funcname>@asm_cycle_tmp@<N>` Pseudo
and threading the cycle through it. The temp's size defaults to 1
byte through `replace_pseudoregisters`'s fallback (no symbol-table
entry), which is correct because asm-SSA only versions one byte at
a time — every cycle member is a single byte.
"""
from __future__ import annotations

from collections import defaultdict

import asm_ast
from passes.optimization_asm.cfg import (
    BasicBlock,
    build_cfg,
    cfg_to_function,
)


_TERMINATOR_TYPES: tuple[type, ...] = (
    asm_ast.Jump,
    asm_ast.Branch,
    asm_ast.Return,
    asm_ast.Ret,
)


def from_ssa(fn: asm_ast.Function) -> asm_ast.Function:
    """Lower every Phi to Movs on each predecessor edge; remove all
    Phis from the function. Returns the rewritten Function.

    Splits critical edges from `Branch`-terminated predecessors
    before lowering, so the inserted Movs never sit between a
    flag-setting instruction and the Branch that reads those flags."""
    fn = _split_critical_edges_for_phi_merges(fn)
    cfg = build_cfg(fn)
    label_to_block: dict[str, BasicBlock] = {
        b.instructions[0].name: b
        for b in cfg.blocks.values()
        if b.instructions and isinstance(b.instructions[0], asm_ast.Label)
    }
    cycle_counter = [0]

    for bid in cfg.block_order:
        blk = cfg.blocks[bid]
        phis = [i for i in blk.instructions if isinstance(i, asm_ast.Phi)]
        if not phis:
            continue
        per_pred: dict[str, list[asm_ast.Mov]] = defaultdict(list)
        for phi in phis:
            for arg in phi.args:
                per_pred[arg.pred_label].append(
                    asm_ast.Mov(src=arg.source, dst=phi.dst),
                )
        for pred_label, movs in per_pred.items():
            pred_blk = label_to_block.get(pred_label)
            if pred_blk is None:
                # Predecessor was dropped — Phi arg is dead.
                continue
            ordered = _order_parallel_copies(
                movs, fn_name=fn.name, cycle_counter=cycle_counter,
            )
            insert_pos = len(pred_blk.instructions)
            if (
                pred_blk.instructions
                and isinstance(pred_blk.instructions[-1], _TERMINATOR_TYPES)
            ):
                # All non-split predecessors end with Jump / Return /
                # Ret here (Branch-terminated predecessors of Phi
                # merges have been redirected through split blocks).
                # Inserting before Jump is safe — JMP doesn't read
                # flags. Return / Ret would imply a Phi at EXIT,
                # which can't happen.
                insert_pos -= 1
            pred_blk.instructions[insert_pos:insert_pos] = ordered
        blk.instructions = [
            i for i in blk.instructions if not isinstance(i, asm_ast.Phi)
        ]
    return cfg_to_function(fn, cfg)


# ---------------------------------------------------------------------------
# Critical-edge splitting.
# ---------------------------------------------------------------------------


def _split_critical_edges_for_phi_merges(
    fn: asm_ast.Function,
) -> asm_ast.Function:
    """Insert a fresh block on every edge from a `Branch`-terminated
    predecessor to a Phi-bearing merge, so the destruction Movs land
    in a flag-safe location.

    Two layout cases per critical edge:
      - `Branch(cond, M_label)` → M (the merge is the Branch's
        TAKEN target). Mint `L_split`, retarget the Branch to
        `L_split`, and append `Label(L_split), Jump(M_label)` at the
        end of the function. Movs will later be inserted between the
        Label and the Jump.
      - `Branch(cond, _)` followed in source order by `Label(M)` (M
        is the Branch's FALL-THROUGH target). Mint `L_split` and
        insert `Label(L_split)` between the Branch and the Label(M)
        in source order. The Branch keeps falling through into the
        new block, which then falls through to M. No new Jump is
        needed. Movs go between L_split and Label(M).

    In both cases each affected `Phi.args[i].pred_label` is rewritten
    from `P_label` to `L_split` so the destruction step's
    label-to-block lookup finds the new block instead of P."""
    instrs = list(fn.instructions)

    # Build a per-block scan to find Phi-bearing merge labels.
    phi_merges: set[str] = _phi_bearing_labels(instrs)
    if not phi_merges:
        return fn

    # Counter for fresh split-block labels.
    split_counter = 0

    # Two passes: first the FALL-THROUGH inserts (which mutate
    # source order — a Branch whose source-next is a Phi merge),
    # then the TAKEN-edge appends.
    fall_through_inserts: list[tuple[int, str, str]] = []
    # Tuples: (insert-position, new_label, target_phi_merge_label).
    taken_appends: list[tuple[int, str, str, str]] = []
    # Tuples: (branch_index, new_label, target_phi_merge_label,
    #          old_pred_label).

    # Walk to find the leading-Label of each block (so we can name
    # the predecessor for Phi-arg rewriting).
    pred_label_at = _block_leading_label_index(instrs)

    for i, instr in enumerate(instrs):
        if not isinstance(instr, asm_ast.Branch):
            continue
        # Identify P's leading-Label (the block that contains this
        # Branch). It's the most recent Label at-or-before index i.
        p_label = pred_label_at[i]
        if p_label is None:
            # Branch in a block with no leading label: shouldn't
            # happen after `_ensure_block_labels`, but defensive.
            continue

        # TAKEN edge: Branch(cond, M_label) where M is a Phi merge.
        if instr.target in phi_merges:
            split_label = _mint_split_label(fn.name, split_counter)
            split_counter += 1
            taken_appends.append(
                (i, split_label, instr.target, p_label),
            )
            # Retarget the Branch in place. The append step adds
            # Label(split_label) + Jump(target) to the function.
            instrs[i] = asm_ast.Branch(
                cond=instr.cond, target=split_label,
            )

        # FALL-THROUGH edge: source-next instruction past the Branch
        # is a Label of a Phi-bearing merge.
        next_idx = i + 1
        if next_idx < len(instrs) and isinstance(
            instrs[next_idx], asm_ast.Label,
        ) and instrs[next_idx].name in phi_merges:
            split_label = _mint_split_label(fn.name, split_counter)
            split_counter += 1
            fall_through_inserts.append(
                (next_idx, split_label, instrs[next_idx].name),
            )

    # Apply taken-edge appends. Add Label(split) + Jump(target) at
    # the end of the function, and rewrite Phi.args for each
    # split: pred_label changes from P's leading label to the
    # split's leading label.
    appended: list[asm_ast.Type_instruction] = []
    for _branch_idx, split_label, target_label, old_pred_label in taken_appends:
        appended.append(asm_ast.Label(name=split_label))
        appended.append(asm_ast.Jump(target=target_label))
        _rewrite_phi_pred_label(
            instrs, target_label, old_pred_label, split_label,
        )

    # Apply fall-through inserts. We need to insert in REVERSE
    # source order so earlier indices stay valid.
    for insert_pos, split_label, target_label in sorted(
        fall_through_inserts, key=lambda t: t[0], reverse=True,
    ):
        # The predecessor of the merge (via fall-through) is the
        # block that contains the Branch at insert_pos - 1. Find
        # its leading label.
        old_pred_label = pred_label_at[insert_pos - 1]
        instrs.insert(insert_pos, asm_ast.Label(name=split_label))
        if old_pred_label is not None:
            _rewrite_phi_pred_label(
                instrs, target_label, old_pred_label, split_label,
            )

    instrs.extend(appended)

    return asm_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=instrs,
    )


def _phi_bearing_labels(
    instrs: list[asm_ast.Type_instruction],
) -> set[str]:
    """Set of labels at which one or more Phi instructions sit."""
    out: set[str] = set()
    current: str | None = None
    for instr in instrs:
        if isinstance(instr, asm_ast.Label):
            current = instr.name
        elif isinstance(instr, asm_ast.Phi) and current is not None:
            out.add(current)
    return out


def _block_leading_label_index(
    instrs: list[asm_ast.Type_instruction],
) -> list[str | None]:
    """For each instruction index `i`, the name of the most recent
    `Label` at-or-before `i`. None for instructions before any Label."""
    out: list[str | None] = [None] * len(instrs)
    current: str | None = None
    for i, instr in enumerate(instrs):
        if isinstance(instr, asm_ast.Label):
            current = instr.name
        out[i] = current
    return out


def _mint_split_label(fn_name: str, n: int) -> str:
    return f".{fn_name}@asm_ssa_split@{n}"


def _rewrite_phi_pred_label(
    instrs: list[asm_ast.Type_instruction],
    merge_label: str,
    old_pred_label: str,
    new_pred_label: str,
) -> None:
    """In every Phi at the block whose leading Label is
    `merge_label`, rewrite each `args[i]` whose `pred_label ==
    old_pred_label` to use `new_pred_label`."""
    in_target_block = False
    for instr in instrs:
        if isinstance(instr, asm_ast.Label):
            in_target_block = (instr.name == merge_label)
            continue
        if not in_target_block:
            continue
        if not isinstance(instr, asm_ast.Phi):
            # Non-Phi inside the target block → done with the
            # target's Phi prefix.
            in_target_block = False
            continue
        for k, arg in enumerate(instr.args):
            if arg.pred_label == old_pred_label:
                instr.args[k] = asm_ast.AsmPhiArg(
                    pred_label=new_pred_label,
                    source=arg.source,
                )


def _order_parallel_copies(
    movs: list[asm_ast.Mov],
    *,
    fn_name: str,
    cycle_counter: list[int],
) -> list[asm_ast.Mov]:
    """Topologically sort `movs` so that each Mov's dst isn't read
    by any later Mov in the output. Cycles get broken by a fresh
    temp Pseudo."""
    if len(movs) <= 1:
        return list(movs)

    pending = list(movs)
    out: list[asm_ast.Mov] = []
    while pending:
        ready_idx = None
        for i, m in enumerate(pending):
            if not isinstance(m.dst, asm_ast.Pseudo):
                # Non-Pseudo dsts (Reg, Stack, ...) can't be read as
                # a Pseudo src by another Mov, so they're trivially
                # ready.
                ready_idx = i
                break
            d_name = m.dst.name
            d_offset = m.dst.offset
            blocks_other = any(
                isinstance(other.src, asm_ast.Pseudo)
                and other.src.name == d_name
                and other.src.offset == d_offset
                for j, other in enumerate(pending) if j != i
            )
            if not blocks_other:
                ready_idx = i
                break
        if ready_idx is not None:
            out.append(pending.pop(ready_idx))
            continue
        # All remaining Movs form a cycle. Break by saving one Mov's
        # dst to a fresh temp and rewriting any pending Mov that
        # read that dst to instead read the temp.
        chosen = pending[0]
        if not isinstance(chosen.dst, asm_ast.Pseudo):
            # Defensive — Phi-derived parallel Movs always have
            # Pseudo dsts. Fall back to source order.
            out.extend(pending)
            break
        cycle_counter[0] += 1
        tmp_name = f".{fn_name}@asm_cycle_tmp@{cycle_counter[0]}"
        tmp = asm_ast.Pseudo(name=tmp_name, offset=0)
        # Save: tmp <- chosen.dst (the OLD value of dst).
        out.append(asm_ast.Mov(
            src=asm_ast.Pseudo(
                name=chosen.dst.name, offset=chosen.dst.offset,
            ),
            dst=tmp,
        ))
        d_name = chosen.dst.name
        d_offset = chosen.dst.offset
        for j, other in enumerate(pending):
            if (
                j != 0
                and isinstance(other.src, asm_ast.Pseudo)
                and other.src.name == d_name
                and other.src.offset == d_offset
            ):
                pending[j] = asm_ast.Mov(src=tmp, dst=other.dst)
        # Loop continues; chosen Mov is now eligible for emission.
    return out
