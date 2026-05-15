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
            # A Phi.dst that apply_coloring rewrote to Reg(X) / Reg(Y)
            # may be paired with an arg whose source is still a Pseudo
            # (parameters / address-taken locals stay symbolic until
            # replace_pseudoregisters). Once those Pseudos resolve to
            # Frame / Stack / Indirect, a direct `Mov(memory, Reg(X|Y))`
            # is unassemblable — the 6502 has no `LDX (zp),Y` form.
            # Defensively route any non-Reg / non-Imm source destined
            # for X or Y through A.
            ordered = _safe_dst_xy_split(ordered)
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


def _safe_dst_xy_split(
    movs: list[asm_ast.Mov],
) -> list[asm_ast.Mov]:
    """Route any `Mov(non_reg_non_imm, Reg(X|Y))` through `Reg(A)` to
    keep the result assemblable after `replace_pseudoregisters`.

    Background: a Phi whose dst was hwreg-colored to X / Y may have
    args whose source is still a Pseudo (parameters, address-taken
    locals, statics — names that stay symbolic until
    `replace_pseudoregisters`). The 6502 has no `LDX (zp),Y` /
    `LDY (zp),Y`, so a direct `Mov(Frame/Stack/Indirect, Reg(X|Y))`
    is unassemblable. We don't know which Pseudos resolve to ZP /
    Data (assemblable) versus Frame / Stack (not), so be
    conservative: anything that isn't an `Imm` or another `Reg`
    routes through A.

    Imm and Reg sources are safe direct loads (`LDX #imm`, `TXY`
    via two transfers, etc.) and skip the split."""
    if not movs:
        return movs
    out: list[asm_ast.Mov] = []
    reg_a = asm_ast.Reg(reg=asm_ast.A())
    for m in movs:
        if (
            isinstance(m.dst, asm_ast.Reg)
            and isinstance(m.dst.reg, (asm_ast.X, asm_ast.Y))
            and not isinstance(m.src, (asm_ast.Imm, asm_ast.Reg))
        ):
            out.append(asm_ast.Mov(src=m.src, dst=reg_a))
            out.append(asm_ast.Mov(src=reg_a, dst=m.dst))
        else:
            out.append(m)
    return out


def _order_parallel_copies(
    movs: list[asm_ast.Mov],
    *,
    fn_name: str,
    cycle_counter: list[int],
) -> list[asm_ast.Mov]:
    """Topologically sort `movs` so that each Mov's dst isn't read
    by any later Mov in the output. Cycles get broken by a fresh
    temp Pseudo.

    Operand identity is checked via `_storage_key` — an opaque
    handle that distinguishes by physical location, not by SSA
    name. This catches cycles introduced by `apply_coloring`
    (where two SSA-distinct names end up at the same `ZP` address)
    that name-equality would miss."""
    if len(movs) <= 1:
        return list(movs)

    pending = list(movs)
    out: list[asm_ast.Mov] = []
    while pending:
        ready_idx = None
        for i, m in enumerate(pending):
            d_key = _storage_key(m.dst)
            if d_key is None:
                # Dst storage doesn't alias anything — trivially
                # ready (won't be read as a src by another Mov).
                ready_idx = i
                break
            blocks_other = any(
                _storage_key(other.src) == d_key
                for j, other in enumerate(pending) if j != i
            )
            if not blocks_other:
                ready_idx = i
                break
        if ready_idx is not None:
            out.append(pending.pop(ready_idx))
            continue
        # All remaining Movs form a cycle. Break by saving one
        # Mov's dst to a fresh temp Pseudo and rewriting any
        # pending Mov whose src aliases that dst to read the temp
        # instead. The temp is a fresh Pseudo that
        # `replace_pseudoregisters_bare_exit` will lay down as a
        # 1-byte Frame slot (it isn't in the coloring, so it
        # doesn't share storage with anyone).
        chosen = pending[0]
        d_key = _storage_key(chosen.dst)
        if d_key is None:
            out.extend(pending)
            break
        cycle_counter[0] += 1
        tmp_name = f".{fn_name}@asm_cycle_tmp@{cycle_counter[0]}"
        tmp = asm_ast.Pseudo(name=tmp_name, offset=0)
        # Save: tmp <- chosen.dst's CURRENT value. Read from
        # chosen.dst's storage (we need a SOURCE operand of that
        # storage; for Pseudo it's Pseudo(name, offset); for ZP
        # it's ZP(addr, 0)).
        out.append(asm_ast.Mov(src=_clone_op(chosen.dst), dst=tmp))
        for j, other in enumerate(pending):
            if (
                j != 0
                and _storage_key(other.src) == d_key
            ):
                pending[j] = asm_ast.Mov(src=tmp, dst=other.dst)
    return out


def _storage_key(op: asm_ast.Type_operand):
    """Hashable handle distinguishing operands by physical storage
    location. Two operands sharing a storage key alias each other.

    Returns `None` for operands that can't be aliased by any other
    operand (Imm, ImmLabelLow/High) — they're never SOURCES of a
    parallel-copy cycle."""
    if isinstance(op, asm_ast.Pseudo):
        return ('Pseudo', op.name, op.offset)
    if isinstance(op, asm_ast.ZP):
        return ('ZP', op.address + op.offset)
    if isinstance(op, asm_ast.Reg):
        return ('Reg', type(op.reg).__name__)
    if isinstance(op, asm_ast.Stack):
        return ('Stack', op.offset)
    if isinstance(op, asm_ast.Frame):
        return ('Frame', op.offset)
    if isinstance(op, asm_ast.Data):
        return ('Data', op.name, op.offset)
    if isinstance(op, asm_ast.Indirect):
        return ('Indirect', op.offset)
    return None


def _clone_op(op: asm_ast.Type_operand) -> asm_ast.Type_operand:
    """Build a fresh source-position operand from any operand kind.
    For most operands a structural copy works; the cycle-break
    logic uses this to read from the current value of a dst-shaped
    operand."""
    if isinstance(op, asm_ast.Pseudo):
        return asm_ast.Pseudo(name=op.name, offset=op.offset)
    if isinstance(op, asm_ast.ZP):
        return asm_ast.ZP(address=op.address, offset=op.offset)
    if isinstance(op, asm_ast.Reg):
        return asm_ast.Reg(reg=op.reg)
    if isinstance(op, asm_ast.Stack):
        return asm_ast.Stack(offset=op.offset)
    if isinstance(op, asm_ast.Frame):
        return asm_ast.Frame(offset=op.offset)
    if isinstance(op, asm_ast.Data):
        return asm_ast.Data(name=op.name, offset=op.offset)
    if isinstance(op, asm_ast.Indirect):
        return asm_ast.Indirect(offset=op.offset)
    return op
