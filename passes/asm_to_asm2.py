"""Lower `asm_ast.Program` to `asm2_ast.Program`.

`asm2_ast` is the strictly-atomic-IR sibling of `asm_ast`: every
node is one logical 6502 instruction. The three compound nodes
that asm_emit used to expand at emit time —
`AllocateStack`, `FunctionPrologue`, and `Ret` — are gone in
asm2; this pass rewrites each into its component atoms (the same
atoms `asm_emit` emits today, just laid out in the IR rather
than spelled out in text). Every other instruction passes
through unchanged at the asm2 type. `LoadAddress` stays a
single asm2 node (its expansion is short enough to remain "one
logical compute-an-address-into-two-bytes step" — same status
it has in asm_ast).

Operand / static_init / reg / condition payloads also have to
re-tag at the asm2_ast type (dataclasses don't unify across
modules, so e.g. `asm_ast.Stack(0)` and `asm2_ast.Stack(0)` are
distinct classes). The walk is mechanical.

Naive expansion. The compound-node lowerings here drop the INY /
TAX / STX byte-saving tricks that the previous in-emit
expansions used: each `Mov(Reg(A), Stack(off))` re-loads Y, the
restore-FP path moves the low byte through X via `TXA; STA` (no
direct `STX`), etc. The result is +1 byte per `FunctionPrologue`
and +2 bytes per non-trivial `Ret` versus the old emit; in
return the asm2 IR has a much smaller atom set and the same
shape works for every operand. `sim.assembler._prologue_size` /
`_ret_size` / `_emit_prologue` / `_emit_ret` mirror the same
lowering so `instruction_size` and `assemble` stay byte-aligned
with what `asm_emit` will produce.
"""

from __future__ import annotations

import asm_ast
import asm2_ast


# Reserved zero-page symbol names that the runtime header
# `equ`s to fixed addresses. Used by the prologue / epilogue
# expansions for SSP / FP arithmetic.
_SSP = "SSP"
_FP = "FP"


def translate_program(prog: asm_ast.Program) -> asm2_ast.Program:
    """Translate an asm_ast program to its asm2_ast equivalent."""
    return asm2_ast.Program(
        top_level=[_xlate_top_level(tl) for tl in prog.top_level],
    )


def _xlate_top_level(tl: asm_ast.Type_top_level) -> asm2_ast.Type_top_level:
    if isinstance(tl, asm_ast.Function):
        instructions: list[asm2_ast.Type_instruction] = []
        for instr in tl.instructions:
            instructions.extend(_xlate_instruction(instr))
        return asm2_ast.Function(
            name=tl.name,
            is_global=tl.is_global,
            params=list(tl.params),
            instructions=instructions,
        )
    if isinstance(tl, asm_ast.StaticVariable):
        return asm2_ast.StaticVariable(
            name=tl.name,
            is_global=tl.is_global,
            init=[_xlate_static_init(it) for it in tl.init],
        )
    raise TypeError(f"unexpected top-level: {tl!r}")


# -------- payload retagging --------


def _xlate_static_init(it: asm_ast.Type_static_init) -> asm2_ast.Type_static_init:
    match it:
        case asm_ast.CharInit(value=v):
            return asm2_ast.CharInit(value=v)
        case asm_ast.IntInit(value=v):
            return asm2_ast.IntInit(value=v)
        case asm_ast.LongInit(value=v):
            return asm2_ast.LongInit(value=v)
        case asm_ast.LongLongInit(value=v):
            return asm2_ast.LongLongInit(value=v)
        case asm_ast.FloatInit(bits=b):
            return asm2_ast.FloatInit(bits=b)
        case asm_ast.DoubleInit(bits=b):
            return asm2_ast.DoubleInit(bits=b)
        case asm_ast.AddressInit(name=n, offset=o):
            return asm2_ast.AddressInit(name=n, offset=o)
        case asm_ast.StringInit(str=s, bytes=n):
            return asm2_ast.StringInit(str=s, bytes=n)
        case asm_ast.ZeroInit(bytes=n):
            return asm2_ast.ZeroInit(bytes=n)
        case _:
            raise TypeError(f"unexpected static_init: {it!r}")


def _xlate_reg(r: asm_ast.Type_reg) -> asm2_ast.Type_reg:
    if isinstance(r, asm_ast.A):
        return asm2_ast.A()
    if isinstance(r, asm_ast.X):
        return asm2_ast.X()
    if isinstance(r, asm_ast.Y):
        return asm2_ast.Y()
    raise TypeError(f"unexpected reg: {r!r}")


_COND_MAP = {
    asm_ast.CC: asm2_ast.CC,
    asm_ast.CS: asm2_ast.CS,
    asm_ast.EQ: asm2_ast.EQ,
    asm_ast.MI: asm2_ast.MI,
    asm_ast.NE: asm2_ast.NE,
    asm_ast.PL: asm2_ast.PL,
    asm_ast.VC: asm2_ast.VC,
    asm_ast.VS: asm2_ast.VS,
}


def _xlate_cond(c: asm_ast.Type_condition) -> asm2_ast.Type_condition:
    cls = _COND_MAP.get(type(c))
    if cls is None:
        raise TypeError(f"unexpected condition: {c!r}")
    return cls()


def _xlate_op(op: asm_ast.Type_operand) -> asm2_ast.Type_operand:
    match op:
        case asm_ast.Imm(value=v):
            return asm2_ast.Imm(value=v)
        case asm_ast.Reg(reg=r):
            return asm2_ast.Reg(reg=_xlate_reg(r))
        case asm_ast.Stack(offset=o):
            return asm2_ast.Stack(offset=o)
        case asm_ast.Frame(offset=o):
            return asm2_ast.Frame(offset=o)
        case asm_ast.Data(name=n, offset=o):
            return asm2_ast.Data(name=n, offset=o)
        case asm_ast.Indirect(offset=o):
            return asm2_ast.Indirect(offset=o)
        case asm_ast.ZP(address=a, offset=o):
            return asm2_ast.ZP(address=a, offset=o)
        case asm_ast.ImmLabelLow(name=n, offset=o):
            return asm2_ast.ImmLabelLow(name=n, offset=o)
        case asm_ast.ImmLabelHigh(name=n, offset=o):
            return asm2_ast.ImmLabelHigh(name=n, offset=o)
        case asm_ast.IndexedData(name=n, offset=o, index=ix):
            return asm2_ast.IndexedData(
                name=n, offset=o, index=_xlate_reg(ix),
            )
        case asm_ast.Pseudo():
            raise ValueError(
                f"Pseudo({op.name!r}) reached asm_to_asm2; "
                "replace_pseudoregisters must run first"
            )
        case _:
            raise TypeError(f"unexpected operand: {op!r}")


# -------- compound-node expansions --------
#
# Each helper returns a list of asm2 atoms. The shapes mirror
# what `asm_emit._emit_function_prologue` / `_emit_ret` /
# `_emit_ssp_sub` etc. used to produce, except for the INY / TAX
# / STX byte-saving tricks: those are dropped in favor of two
# self-contained Mov atoms per word.


def _reg_a() -> asm2_ast.Reg:
    return asm2_ast.Reg(reg=asm2_ast.A())


def _reg_x() -> asm2_ast.Reg:
    return asm2_ast.Reg(reg=asm2_ast.X())


def _data(name: str, offset: int) -> asm2_ast.Data:
    return asm2_ast.Data(name=name, offset=offset)


def _ssp_sub(amt: int) -> list[asm2_ast.Type_instruction]:
    """`SSP -= amt` (16-bit). 0 if amt==0; otherwise a 7-atom
    SEC / LDA-SBC-STA / LDA-SBC-STA byte pair."""
    if amt == 0:
        return []
    if not 0 <= amt <= 0xFFFF:
        raise ValueError(f"AllocateStack amt {amt} out of range 0..65535")
    lo, hi = amt & 0xFF, (amt >> 8) & 0xFF
    A = _reg_a()
    return [
        asm2_ast.SetCarry(),
        asm2_ast.Mov(src=_data(_SSP, 0), dst=A),
        asm2_ast.Sub(src=asm2_ast.Imm(value=lo), dst=A),
        asm2_ast.Mov(src=A, dst=_data(_SSP, 0)),
        asm2_ast.Mov(src=_data(_SSP, 1), dst=A),
        asm2_ast.Sub(src=asm2_ast.Imm(value=hi), dst=A),
        asm2_ast.Mov(src=A, dst=_data(_SSP, 1)),
    ]


def _set_fp_to_ssp() -> list[asm2_ast.Type_instruction]:
    """`FP = SSP` (16-bit)."""
    A = _reg_a()
    return [
        asm2_ast.Mov(src=_data(_SSP, 0), dst=A),
        asm2_ast.Mov(src=A, dst=_data(_FP, 0)),
        asm2_ast.Mov(src=_data(_SSP, 1), dst=A),
        asm2_ast.Mov(src=A, dst=_data(_FP, 1)),
    ]


def _set_ssp_to_fp_plus(amt: int) -> list[asm2_ast.Type_instruction]:
    """`SSP = FP + amt` (16-bit)."""
    if not 0 <= amt <= 0xFFFF:
        raise ValueError(f"epilogue rewind {amt} out of range 0..65535")
    A = _reg_a()
    if amt == 0:
        # Same shape as `FP = SSP` but reversed source/destination.
        return [
            asm2_ast.Mov(src=_data(_FP, 0), dst=A),
            asm2_ast.Mov(src=A, dst=_data(_SSP, 0)),
            asm2_ast.Mov(src=_data(_FP, 1), dst=A),
            asm2_ast.Mov(src=A, dst=_data(_SSP, 1)),
        ]
    lo, hi = amt & 0xFF, (amt >> 8) & 0xFF
    return [
        asm2_ast.ClearCarry(),
        asm2_ast.Mov(src=_data(_FP, 0), dst=A),
        asm2_ast.Add(src=asm2_ast.Imm(value=lo), dst=A),
        asm2_ast.Mov(src=A, dst=_data(_SSP, 0)),
        asm2_ast.Mov(src=_data(_FP, 1), dst=A),
        asm2_ast.Add(src=asm2_ast.Imm(value=hi), dst=A),
        asm2_ast.Mov(src=A, dst=_data(_SSP, 1)),
    ]


def _save_fp_into_slot(m: int) -> list[asm2_ast.Type_instruction]:
    """Write the current FP into Stack(m+1) / Stack(m+2). Each
    Mov(Reg(A), Stack(off)) re-loads Y; that's +1 byte vs. the
    INY-trick form `_emit_function_prologue` used to emit, but
    keeps every atom self-contained."""
    _check_local_bytes(m)
    A = _reg_a()
    return [
        asm2_ast.Mov(src=_data(_FP, 0), dst=A),
        asm2_ast.Mov(src=A, dst=asm2_ast.Stack(offset=m + 1)),
        asm2_ast.Mov(src=_data(_FP, 1), dst=A),
        asm2_ast.Mov(src=A, dst=asm2_ast.Stack(offset=m + 2)),
    ]


def _restore_fp_from_slot(m: int) -> list[asm2_ast.Type_instruction]:
    """Restore FP from Frame(m+1) / Frame(m+2). The low byte goes
    through X (TAX / TXA) so it survives across the high read /
    high store; X is free to clobber because no return convention
    holds data there. +2 bytes vs. the INY+STX trick the old
    `_emit_ret` used."""
    _check_local_bytes(m)
    A = _reg_a()
    X = _reg_x()
    return [
        asm2_ast.Mov(src=asm2_ast.Frame(offset=m + 1), dst=A),
        asm2_ast.Mov(src=A, dst=X),
        asm2_ast.Mov(src=asm2_ast.Frame(offset=m + 2), dst=A),
        asm2_ast.Mov(src=A, dst=_data(_FP, 1)),
        asm2_ast.Mov(src=X, dst=A),
        asm2_ast.Mov(src=A, dst=_data(_FP, 0)),
    ]


def _save_zp_byte_to_frame(
    zp_addr: int, frame_offset: int,
) -> list[asm2_ast.Type_instruction]:
    """Save the byte at `$zp_addr` into the frame slot at
    `FP+frame_offset`. Used by the prologue's callee-save
    sequence."""
    _check_byte("callee-saved address", zp_addr)
    A = _reg_a()
    return [
        asm2_ast.Mov(
            src=asm2_ast.ZP(address=zp_addr, offset=0), dst=A,
        ),
        asm2_ast.Mov(
            src=A, dst=asm2_ast.Frame(offset=frame_offset),
        ),
    ]


def _restore_zp_byte_from_frame(
    zp_addr: int, frame_offset: int,
) -> list[asm2_ast.Type_instruction]:
    """Restore the byte at `$zp_addr` from `FP+frame_offset`. Used
    by the epilogue's callee-restore sequence."""
    _check_byte("callee-saved address", zp_addr)
    A = _reg_a()
    return [
        asm2_ast.Mov(
            src=asm2_ast.Frame(offset=frame_offset), dst=A,
        ),
        asm2_ast.Mov(
            src=A, dst=asm2_ast.ZP(address=zp_addr, offset=0),
        ),
    ]


def _function_prologue(
    arg_bytes: int, local_bytes: int, callee_saved_addrs: list[int],
) -> list[asm2_ast.Type_instruction]:
    if arg_bytes + local_bytes == 0:
        # No frame to set up — no args were pushed and no locals
        # need allocation, so the prologue is empty. (Callee-saves
        # without a frame would have nowhere to land, so we forbid
        # that case implicitly: the prologue's Comment / saves
        # block is gated on the frame existing.)
        return []
    saves_note = (
        f", {len(callee_saved_addrs)} callee-saved bytes"
        if callee_saved_addrs else ""
    )
    out: list[asm2_ast.Type_instruction] = [
        asm2_ast.Comment(
            text=f"prologue: {arg_bytes} arg bytes, "
                 f"{local_bytes} local bytes{saves_note}"
        )
    ]
    out += _ssp_sub(local_bytes + 2)
    out += _save_fp_into_slot(local_bytes)
    out += _set_fp_to_ssp()
    for i, addr in enumerate(callee_saved_addrs):
        out += _save_zp_byte_to_frame(addr, i + 1)
    out.append(asm2_ast.Blank())
    return out


def _ret(
    arg_bytes: int, local_bytes: int, save_a: bool,
    callee_saved_addrs: list[int],
) -> list[asm2_ast.Type_instruction]:
    if arg_bytes + local_bytes == 0:
        # No frame to tear down — `Return` (RTS) by itself.
        return [asm2_ast.Return()]
    rewind = arg_bytes + local_bytes + 2
    out: list[asm2_ast.Type_instruction] = [
        asm2_ast.Blank(),
        asm2_ast.Comment(text="epilogue"),
    ]
    # Restore callee-saved ZP bytes BEFORE the SSP/FP teardown so
    # FP is still valid for the indirect-Y reads.
    for i, addr in enumerate(callee_saved_addrs):
        out += _restore_zp_byte_from_frame(addr, i + 1)
    A = _reg_a()
    if save_a:
        out.append(asm2_ast.Push(src=A))
    out += _set_ssp_to_fp_plus(rewind)
    out += _restore_fp_from_slot(local_bytes)
    if save_a:
        out.append(asm2_ast.Pop(dst=A))
    out.append(asm2_ast.Return())
    return out


def _check_local_bytes(m: int) -> None:
    # Largest workable LDY immediate for `Stack(m+2)` / `Frame(m+2)`
    # access (M+2 must fit in a byte).
    if not 0 <= m <= 253:
        raise ValueError(
            f"local_bytes {m} out of range 0..253 "
            "(limited by LDY immediate for FP-slot addressing)"
        )


def _check_byte(label: str, v: int) -> None:
    if not 0 <= v <= 0xFF:
        raise ValueError(f"{label} {v} out of range 0..255")


# -------- per-instruction dispatch --------


def _xlate_instruction(
    instr: asm_ast.Type_instruction,
) -> list[asm2_ast.Type_instruction]:
    """Translate one asm_ast instruction into a list of asm2_ast
    atoms. Most instructions translate to a single atom; the three
    compound nodes (AllocateStack, FunctionPrologue, Ret) expand
    into multi-atom sequences."""
    match instr:
        case asm_ast.Mov(src=s, dst=d):
            return [asm2_ast.Mov(src=_xlate_op(s), dst=_xlate_op(d))]
        case asm_ast.Add(src=s, dst=d):
            return [asm2_ast.Add(src=_xlate_op(s), dst=_xlate_op(d))]
        case asm_ast.Sub(src=s, dst=d):
            return [asm2_ast.Sub(src=_xlate_op(s), dst=_xlate_op(d))]
        case asm_ast.Call(name=n):
            return [asm2_ast.Call(name=n)]
        case asm_ast.ClearCarry():
            return [asm2_ast.ClearCarry()]
        case asm_ast.SetCarry():
            return [asm2_ast.SetCarry()]
        case asm_ast.Inc(dst=d):
            return [asm2_ast.Inc(dst=_xlate_op(d))]
        case asm_ast.Dec(dst=d):
            return [asm2_ast.Dec(dst=_xlate_op(d))]
        case asm_ast.Push(src=s):
            return [asm2_ast.Push(src=_xlate_op(s))]
        case asm_ast.Pop(dst=d):
            return [asm2_ast.Pop(dst=_xlate_op(d))]
        case asm_ast.Xor(src1=s1, src2=s2, dst=d):
            return [asm2_ast.Xor(
                src1=_xlate_op(s1),
                src2=_xlate_op(s2),
                dst=_xlate_op(d),
            )]
        case asm_ast.And(src=s, dst=d):
            return [asm2_ast.And(src=_xlate_op(s), dst=_xlate_op(d))]
        case asm_ast.Or(src=s, dst=d):
            return [asm2_ast.Or(src=_xlate_op(s), dst=_xlate_op(d))]
        case asm_ast.ArithmeticShiftLeft(dst=d):
            return [asm2_ast.ArithmeticShiftLeft(dst=_xlate_op(d))]
        case asm_ast.LogicalShiftRight(dst=d):
            return [asm2_ast.LogicalShiftRight(dst=_xlate_op(d))]
        case asm_ast.RotateLeft(dst=d):
            return [asm2_ast.RotateLeft(dst=_xlate_op(d))]
        case asm_ast.RotateRight(dst=d):
            return [asm2_ast.RotateRight(dst=_xlate_op(d))]
        case asm_ast.Label(name=n):
            return [asm2_ast.Label(name=n)]
        case asm_ast.Jump(target=t):
            return [asm2_ast.Jump(target=t)]
        case asm_ast.Branch(cond=c, target=t):
            return [asm2_ast.Branch(cond=_xlate_cond(c), target=t)]
        case asm_ast.Compare(left=lt, right=rt):
            return [asm2_ast.Compare(
                left=_xlate_op(lt), right=_xlate_op(rt),
            )]
        case asm_ast.LoadAddress(src=s, dst=d):
            return [asm2_ast.LoadAddress(
                src=_xlate_op(s), dst=_xlate_op(d),
            )]
        case asm_ast.AllocateStack(bytes=n):
            return _ssp_sub(n)
        case asm_ast.FunctionPrologue(
            arg_bytes=ab, local_bytes=lb, callee_saved_addrs=csa,
        ):
            return _function_prologue(ab, lb, list(csa))
        case asm_ast.Ret(
            arg_bytes=ab, local_bytes=lb, save_a=sa,
            callee_saved_addrs=csa,
        ):
            return _ret(ab, lb, sa, list(csa))
        case asm_ast.Return():
            # Bare exit — just RTS, no SSP/FP teardown. Emitted on
            # the `--optimize-asm` path between phase 9 and the
            # synthesis pass; if synthesis decides the function
            # needs no frame, this passes straight through to the
            # corresponding asm2 atom. `save_a` is carried on the
            # asm_ast node but discarded here — by the time we're
            # in asm2 the synthesis pass has already chosen between
            # bare RTS (this branch) and a full Ret-shaped epilogue
            # (which lowers via `_ret`).
            return [asm2_ast.Return()]
        case asm_ast.Phi():
            # Phis are transient SSA-form nodes; they should have
            # been lowered to Copies by `from_ssa` long before
            # asm_to_asm2 sees the program.
            raise TypeError(
                "asm_to_asm2: Phi node leaked past SSA destruction",
            )
        case _:
            raise TypeError(f"unexpected instruction: {instr!r}")
