"""Apply a `Coloring` to an asm-SSA function: substitute every
`Pseudo(name, offset)` whose name is in `coloring.assignments` with
the corresponding `ZP(address, offset)` operand, OR substitute names
in `coloring.hwreg_assignments` with `Reg(X)` / `Reg(Y)`.

Runs between `byte_dce` and `from_ssa`. By the time `from_ssa`
sees the function, colored values have been lowered to ZP / HwReg,
so the parallel-copy ordering can spot cross-Mov cycles at the
PHYSICAL slot level (which is the actual hazard) instead of just
the SSA name level. Cycles like:

    Phi(X, [(P, Y)])  ; X := Y
    Phi(Y, [(P, X)])  ; Y := X

where X and Y get DIFFERENT colors $A and $B form a 2-cycle at
the predecessor edge (Mov $B → $A clobbers $A before Mov $A → $B
reads it). With pre-applied coloring, those Movs become
`Mov(ZP($B), ZP($A))` and `Mov(ZP($A), ZP($B))`, and the cycle
detector sees the `ZP($A)` repetition.

# HwReg substitution

When a Pseudo is HwReg-colored to X / Y, every reference to it
becomes `Reg(X)` / `Reg(Y)`. The substitution alone produces
correct but suboptimal code: an IndexedData index-setup chain

    Mov(P, Reg(A)); Mov(Reg(A), Reg(X)); ... IndexedData(..., X)

with P colored to Y becomes

    Mov(Reg(Y), Reg(A)); Mov(Reg(A), Reg(X)); ... IndexedData(..., X)

— TYA + TAX before each LDA name,X. The two transfers are
redundant since X already gets Y's value and we could read
IndexedData with `index=Y` directly. So a follow-up peephole
recognizes the redundant transfer chain and drops it, rewriting
the following IndexedData operands' index from X to Y (or Y to X
in the symmetric case). See `_rewrite_redundant_transfers`.

Pseudos NOT in either coloring (params, address-taken, statics,
spilled) pass through unchanged — they're handled later by
`replace_pseudoregisters_bare_exit`.
"""
from __future__ import annotations

import asm_ast
from passes.optimization.register_allocation import Coloring


def apply_coloring(
    fn: asm_ast.Function, coloring: Coloring,
    *,
    local_pool: list[int] | None = None,
) -> asm_ast.Function:
    """Return `fn` with every colored Pseudo lowered to ZP / Reg.

    When `local_pool` is provided, regalloc-colored Pseudos whose
    address falls in the pool are emitted as
    `Data(__local_<fn>_b<k>, 0)` operands instead of numeric
    `ZP(addr, 0)` — where `k` is the byte's position in
    `local_pool`. The asm-emit stage prints
    `__local_<fn>_b<k> EQU $<addr>` directives alongside the
    `__zpabi_*` block, and dasm picks zp vs. absolute addressing
    from the resolved value (so a pool that spilled above `$FF`
    is transparent to the body). The symbolic form is what makes
    the multi-TU linker (`compile.py --link`) able to re-allocate
    body locals globally: the per-TU IR doesn't bake in addresses,
    only stable slot indices.

    Functions WITHOUT a private pool (ineligible per
    `zp_local_allocation`) fall back to the numeric `ZP(addr, 0)`
    form. Same for any colored byte that — defensively — lands
    outside the supplied pool (shouldn't happen; the regalloc was
    given the pool as its `allowed_range`)."""
    if not coloring.assignments and not coloring.hwreg_assignments:
        return fn
    addr_to_slot: dict[int, int] = {}
    if local_pool:
        addr_to_slot = {addr: k for k, addr in enumerate(local_pool)}
    # Phase 1: substitute Pseudo references.
    new_instrs = [
        _apply_to_instruction(instr, coloring, fn.name, addr_to_slot)
        for instr in fn.instructions
    ]
    # Phase 2: drop the redundant TR'A + TAR transfer chain that
    # arises when a HwReg-colored Pseudo feeds an IndexedData index
    # setup. Rewrites the following IndexedData operands' index to
    # use the source HwReg directly. Iterated to fixpoint because a
    # cross-transfer's rewritten run may include adjacent self-
    # transfer chains that the next pass picks up.
    if coloring.hwreg_assignments:
        while True:
            prev_len = len(new_instrs)
            new_instrs = _rewrite_redundant_transfers(new_instrs)
            if len(new_instrs) == prev_len:
                break
    return asm_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=new_instrs,
    )


def _apply_to_op(
    op: asm_ast.Type_operand, coloring: Coloring,
    fn_name: str, addr_to_slot: dict[int, int],
) -> asm_ast.Type_operand:
    if isinstance(op, asm_ast.Pseudo):
        if op.name in coloring.hwreg_assignments:
            # HwReg coloring is single-byte only, so offset must be 0.
            # (Eligibility scan + regalloc together guarantee this; if
            # offset != 0 sneaks through, treat it as a regalloc bug
            # rather than silently masking with `& 0`.)
            assert op.offset == 0, (
                f"HwReg-colored Pseudo {op.name} has nonzero offset "
                f"{op.offset}; eligibility scan should have rejected it"
            )
            return _hwreg_letter_to_op(coloring.hwreg_assignments[op.name])
        if op.name in coloring.assignments:
            addr = coloring.assignments[op.name] + op.offset
            if addr in addr_to_slot:
                return asm_ast.Data(
                    name=f"__local_{fn_name}_b{addr_to_slot[addr]}",
                    offset=0,
                )
            return asm_ast.ZP(address=addr, offset=0)
    return op


def _hwreg_letter_to_op(letter: str) -> asm_ast.Reg:
    if letter == "X":
        return asm_ast.Reg(reg=asm_ast.X())
    if letter == "Y":
        return asm_ast.Reg(reg=asm_ast.Y())
    raise ValueError(f"unknown HwReg letter: {letter!r}")


def _apply_to_instruction(
    instr: asm_ast.Type_instruction, coloring: Coloring,
    fn_name: str, addr_to_slot: dict[int, int],
) -> asm_ast.Type_instruction:
    apply = lambda op: _apply_to_op(op, coloring, fn_name, addr_to_slot)
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            return asm_ast.Mov(src=apply(src), dst=apply(dst))
        case asm_ast.Add(src=src, dst=dst):
            return asm_ast.Add(src=apply(src), dst=apply(dst))
        case asm_ast.Sub(src=src, dst=dst):
            return asm_ast.Sub(src=apply(src), dst=apply(dst))
        case asm_ast.And(src=src, dst=dst):
            return asm_ast.And(src=apply(src), dst=apply(dst))
        case asm_ast.Or(src=src, dst=dst):
            return asm_ast.Or(src=apply(src), dst=apply(dst))
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            return asm_ast.Xor(
                src1=apply(s1), src2=apply(s2), dst=apply(dst),
            )
        case asm_ast.Inc(dst=dst):
            return asm_ast.Inc(dst=apply(dst))
        case asm_ast.Dec(dst=dst):
            return asm_ast.Dec(dst=apply(dst))
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            return asm_ast.ArithmeticShiftLeft(dst=apply(dst))
        case asm_ast.LogicalShiftRight(dst=dst):
            return asm_ast.LogicalShiftRight(dst=apply(dst))
        case asm_ast.RotateLeft(dst=dst):
            return asm_ast.RotateLeft(dst=apply(dst))
        case asm_ast.RotateRight(dst=dst):
            return asm_ast.RotateRight(dst=apply(dst))
        case asm_ast.Push(src=src):
            return asm_ast.Push(src=apply(src))
        case asm_ast.Pop(dst=dst):
            return asm_ast.Pop(dst=apply(dst))
        case asm_ast.Compare(left=left, right=right):
            return asm_ast.Compare(
                left=apply(left), right=apply(right),
            )
        case asm_ast.LoadAddress(src=src, dst=dst):
            # `src` is by construction excluded from coloring
            # (address-taken), so apply() is a no-op for it. Apply
            # to dst for completeness.
            return asm_ast.LoadAddress(
                src=apply(src), dst=apply(dst),
            )
        case asm_ast.Phi(dst=dst, args=args):
            return asm_ast.Phi(
                dst=apply(dst),
                args=[
                    asm_ast.AsmPhiArg(
                        pred_label=a.pred_label,
                        source=apply(a.source),
                    )
                    for a in args
                ],
            )
        case _:
            return instr


# ---------------------------------------------------------------------------
# Phase 2: redundant transfer-chain elimination
# ---------------------------------------------------------------------------


def _rewrite_redundant_transfers(
    instrs: list[asm_ast.Type_instruction],
) -> list[asm_ast.Type_instruction]:
    """Eliminate redundant `Mov(Reg(R'), Reg(A)); Mov(Reg(A),
    Reg(R))` chains that arise after HwReg-coloring substitutes a
    Pseudo→Reg(R') in an IndexedData index-setup chain.

    Two cases:

    **Self-transfer** (R == R'). The chain is `TR'A; TAR'` —
    transfers R' through A and back to R'. Pure no-op for R'  (its
    value is unchanged); only A is clobbered. By construction
    (every emitter of this shape is the IndexedData index setup,
    where the next instruction redefines A via an LDA), dropping
    is unconditionally sound. Drop both Movs; emit nothing.

    **Cross-transfer** (R != R'). The chain transfers R' to R;
    the value the user wanted in R is already in R'. If the chain
    is followed by IndexedData accesses with `index=R` and no
    intervening write to R / R' / A or control-flow boundary, we
    can rewrite those accesses to `index=R'` and drop the chain.
    If no IndexedData rewrites would fire (because the chain is
    setting up R for some non-IndexedData use), leave the chain
    in place — dropping would leave R undefined for that use.
    """
    out: list[asm_ast.Type_instruction] = []
    i = 0
    n = len(instrs)
    while i < n:
        chain = _match_transfer_chain(instrs, i)
        if chain is None:
            out.append(instrs[i])
            i += 1
            continue
        src_reg, dst_reg, chain_len = chain
        if src_reg == dst_reg:
            # Self-transfer: drop the chain unconditionally.
            i += chain_len
            continue
        # Cross-transfer: walk forward from i+chain_len, rewriting
        # IndexedData operands until the run boundary.
        #
        # When a rewrite would produce an unencodable Mov shape
        # (same-index `LDX abs,X` / `LDY abs,Y`), we split the
        # consolidated load (`Mov(IndexedData(...,X), Reg(Y))` =
        # `LDY abs,X`) into `Mov(IndexedData(...,Y), Reg(A))` +
        # `Mov(Reg(A), Reg(Y))` — i.e. `LDA abs,Y; TAY`. The
        # post-rewrite shape (4 bytes) is one byte longer than the
        # pre-rewrite `LDY abs,X` (3 bytes), but dropping the
        # preceding 2-byte transfer chain still nets one byte
        # saved AND keeps Reg(X) free for downstream opportunities
        # (e.g. loop_counter_to_x). Stores can't be split this
        # way — `STX/STY abs,X|Y` don't exist — so a Mov with the
        # IndexedData on the dst side aborts the whole rewrite.
        j = i + chain_len
        rewritten_run: list[asm_ast.Type_instruction] = []
        any_rewritten = False
        aborted = False
        while j < n:
            instr = instrs[j]
            new_instr = _rewrite_indexed_data_index_in_instr(
                instr, from_letter=dst_reg, to_letter=src_reg,
            )
            if new_instr is not instr:
                any_rewritten = True
                splitted = _split_unencodable_indexed_load(new_instr)
                if splitted is not None:
                    rewritten_run.extend(splitted)
                    j += 1
                    # The split's tail is `Mov(Reg(A), Reg(dst))`
                    # which writes dst_reg; that's also a chain
                    # boundary, so stop here.
                    break
                if _is_unencodable_indexed_mov(new_instr):
                    aborted = True
                    break
            should_stop = _instr_breaks_transfer_chain(
                new_instr, dst_reg=dst_reg, src_reg=src_reg,
            )
            rewritten_run.append(new_instr)
            j += 1
            if should_stop:
                break
        if any_rewritten and not aborted:
            # Drop the chain, splice the rewritten run.
            out.extend(rewritten_run)
            i = j
        else:
            out.append(instrs[i])
            i += 1
    return out


def _is_unencodable_indexed_mov(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` is a Mov whose IndexedData operand uses the
    same index register as the other operand's HwReg. The 6502
    has no `LDX abs,X` / `LDY abs,Y` / `STX abs,X` / `STY abs,Y`
    encodings; emit and the in-process assembler both reject these.
    Used by the cross-transfer rewrite to detect when an X→Y (or
    Y→X) IndexedData index rewrite would land in the unencodable
    region."""
    if not isinstance(instr, asm_ast.Mov):
        return False
    src, dst = instr.src, instr.dst
    if (isinstance(src, asm_ast.IndexedData)
        and isinstance(dst, asm_ast.Reg)
        and type(src.index) is type(dst.reg)):
        return True
    if (isinstance(dst, asm_ast.IndexedData)
        and isinstance(src, asm_ast.Reg)
        and type(dst.index) is type(src.reg)):
        return True
    return False


def _split_unencodable_indexed_load(
    instr: asm_ast.Type_instruction,
) -> list[asm_ast.Type_instruction] | None:
    """If `instr` is the same-index `Mov(IndexedData(...,R), Reg(R))`
    load form, split it into `Mov(IndexedData(...,R), Reg(A))` +
    `Mov(Reg(A), Reg(R))` (= `LDA abs,R; T{R}A` then `TAR`). The
    consolidated `LD{R} abs,R` doesn't exist, but the split is
    value-equivalent and 1 byte longer than the consolidated form
    would have been. Returns None for stores (no split is
    value-equivalent) and for instructions that aren't this exact
    shape."""
    if not isinstance(instr, asm_ast.Mov):
        return None
    src, dst = instr.src, instr.dst
    if not (
        isinstance(src, asm_ast.IndexedData)
        and isinstance(dst, asm_ast.Reg)
        and type(src.index) is type(dst.reg)
    ):
        return None
    reg_a = asm_ast.Reg(reg=asm_ast.A())
    return [
        asm_ast.Mov(src=src, dst=reg_a),
        asm_ast.Mov(src=reg_a, dst=dst),
    ]


def _match_transfer_chain(
    instrs: list[asm_ast.Type_instruction], start: int,
) -> tuple[str, str, int] | None:
    """Match a 2-instruction transfer chain `Mov(Reg(R'), A); Mov(A,
    Reg(R))` at `instrs[start]`. Returns (src_letter, dst_letter, 2)
    on success, or None on failure. R and R' are HwRegs (X or Y);
    the same-letter case (R == R') represents a self-transfer
    through A — degenerate but still detected, and the caller
    decides what to do with it."""
    if start + 2 > len(instrs):
        return None
    i0 = instrs[start]
    i1 = instrs[start + 1]
    if not (isinstance(i0, asm_ast.Mov) and isinstance(i1, asm_ast.Mov)):
        return None
    src = i0.src
    dst1 = i0.dst
    src1 = i1.src
    dst2 = i1.dst
    # i0: Mov(Reg(R'), Reg(A))
    if not (
        isinstance(src, asm_ast.Reg)
        and isinstance(src.reg, (asm_ast.X, asm_ast.Y))
        and isinstance(dst1, asm_ast.Reg)
        and isinstance(dst1.reg, asm_ast.A)
    ):
        return None
    # i1: Mov(Reg(A), Reg(R))
    if not (
        isinstance(src1, asm_ast.Reg)
        and isinstance(src1.reg, asm_ast.A)
        and isinstance(dst2, asm_ast.Reg)
        and isinstance(dst2.reg, (asm_ast.X, asm_ast.Y))
    ):
        return None
    src_letter = "X" if isinstance(src.reg, asm_ast.X) else "Y"
    dst_letter = "X" if isinstance(dst2.reg, asm_ast.X) else "Y"
    return (src_letter, dst_letter, 2)


def _rewrite_indexed_data_index_in_instr(
    instr: asm_ast.Type_instruction,
    *, from_letter: str, to_letter: str,
) -> asm_ast.Type_instruction:
    """If `instr` is a Mov whose src or dst is an IndexedData with
    index matching `from_letter`, return a copy with the index
    rewritten to `to_letter`. Otherwise return `instr` unchanged
    (same identity if no rewrite — caller's heuristic uses
    `is`-comparison)."""
    if not isinstance(instr, asm_ast.Mov):
        return instr
    new_src = _rewrite_indexed_data_index_in_op(
        instr.src, from_letter=from_letter, to_letter=to_letter,
    )
    new_dst = _rewrite_indexed_data_index_in_op(
        instr.dst, from_letter=from_letter, to_letter=to_letter,
    )
    if new_src is instr.src and new_dst is instr.dst:
        return instr
    return asm_ast.Mov(src=new_src, dst=new_dst)


def _rewrite_indexed_data_index_in_op(
    op: asm_ast.Type_operand, *, from_letter: str, to_letter: str,
) -> asm_ast.Type_operand:
    if not isinstance(op, asm_ast.IndexedData):
        return op
    cur_letter = "X" if isinstance(op.index, asm_ast.X) else "Y"
    if cur_letter != from_letter:
        return op
    new_reg = (
        asm_ast.X() if to_letter == "X" else asm_ast.Y()
    )
    return asm_ast.IndexedData(
        name=op.name, offset=op.offset, index=new_reg,
    )


def _instr_breaks_transfer_chain(
    instr: asm_ast.Type_instruction,
    *, dst_reg: str, src_reg: str,
) -> bool:
    """True iff `instr` ends the rewriteable run after the transfer
    chain. The run continues only as long as:
      * `instr` doesn't write to Reg(dst_reg) (would re-define the
        register we're rewriting), to Reg(src_reg) (would change the
        source value), or to Reg(A).
      * `instr` isn't a Call, Label, Branch, or Jump (control flow
        boundary).

    Note: writes to Reg(A) are common (every IndexedData read goes
    through A), so the chain typically stops after one or a few
    rewritten Movs in practice. That's correct — once A is
    clobbered, we can't be sure what comes next. Index-register
    rewrites are still sound up to and including the A-clobbering
    instruction (the IndexedData read itself uses A as destination
    but reads only the index register, which we haven't touched
    yet at that point). So we treat "ending after this Mov" rather
    than "ending before this Mov"."""
    # Control-flow boundaries.
    if isinstance(instr, (asm_ast.Call, asm_ast.Label, asm_ast.Jump,
                          asm_ast.Branch, asm_ast.Return, asm_ast.Ret,
                          asm_ast.FunctionPrologue,
                          asm_ast.AllocateStack)):
        return True
    # Writes to dst_reg / src_reg via Mov / Inc / Dec / etc.
    dst_op = _instr_destination(instr)
    if dst_op is not None and isinstance(dst_op, asm_ast.Reg):
        cur_letter = _reg_letter(dst_op)
        if cur_letter in (dst_reg, src_reg):
            return True
    return False


def _instr_destination(
    instr: asm_ast.Type_instruction,
) -> asm_ast.Type_operand | None:
    """The single-destination operand of `instr`, if any. (Many
    instructions write A; this function returns the explicit dst
    field. Compare / Push / ClearCarry / SetCarry have no dst.)"""
    match instr:
        case asm_ast.Mov(dst=dst):
            return dst
        case asm_ast.Add(dst=dst) | asm_ast.Sub(dst=dst):
            return dst
        case asm_ast.And(dst=dst) | asm_ast.Or(dst=dst):
            return dst
        case asm_ast.Xor(dst=dst):
            return dst
        case asm_ast.Inc(dst=dst) | asm_ast.Dec(dst=dst):
            return dst
        case (
            asm_ast.ArithmeticShiftLeft(dst=dst)
            | asm_ast.LogicalShiftRight(dst=dst)
            | asm_ast.RotateLeft(dst=dst)
            | asm_ast.RotateRight(dst=dst)
        ):
            return dst
        case asm_ast.Pop(dst=dst):
            return dst
        case asm_ast.LoadAddress(dst=dst):
            return dst
    return None


def _reg_letter(op: asm_ast.Type_operand) -> str:
    """Return 'A' / 'X' / 'Y' if `op` is a Reg, else ''. Used by the
    chain-breaking check; non-Reg destinations don't affect index
    register state."""
    if not isinstance(op, asm_ast.Reg):
        return ""
    if isinstance(op.reg, asm_ast.A):
        return "A"
    if isinstance(op.reg, asm_ast.X):
        return "X"
    if isinstance(op.reg, asm_ast.Y):
        return "Y"
    return ""
