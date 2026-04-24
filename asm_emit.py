"""Emit 6502 assembly from an asm_ast program.

Formatting rules:
  - labels start in column 1
  - opcodes (uppercase) start in column 4
  - operands start in column 10

Soft-stack convention (see README "Function stack frame layout"):
  - the soft stack pointer is the symbol `SSP`, a 16-bit ZP value
    (low byte at `SSP`, high byte at `SSP+1`)
  - the frame pointer is the symbol `FP`, also a 16-bit ZP value
    (low byte at `FP`, high byte at `FP+1`); FP is captured once at
    function entry and stays put even when SSP moves during the body
  - `Stack(off)` operands are the byte at `SSP+off` (SSP-relative);
    `Frame(off)` operands are the byte at `FP+off` (FP-relative).
    Both emit as `LDY #off` then `LDA (PTR),Y` / `STA (PTR),Y`
  - any indirect access clobbers Y
  - `FunctionPrologue(arg_bytes=N, local_bytes=M)` for `N+M > 0`:
    allocates `M+2` bytes (locals + saved-FP slot), writes the caller's
    FP into the slot at `SSP+M+1` (low) and `SSP+M+2` (high), then
    sets `FP = SSP`. Clobbers A and Y. For `N+M == 0` it emits nothing
    (no FP setup needed when the function has no locals or args).
    Bounded `M <= 253` so `LDY #(M+2)` fits in a byte. Args
    themselves were pushed by the caller before JSR; the prologue
    doesn't allocate them.
  - `Ret(arg_bytes=N, local_bytes=M)` for `N+M > 0`: PHA, then
    `SSP = FP + (N + M + 2)` in one shot (the +2 is the saved-FP
    slot; the result is the caller's pre-arg-push SSP, so the caller
    needs no per-call cleanup). Then read the saved FP via `(FP),Y`
    with `Y=M+1` (low) and `Y=M+2` (high), stashing the low byte in
    X across the high read so we don't corrupt the FP register's
    role as the indirect base mid-read. Then PLA, RTS. For `N+M ==
    0` it just emits `RTS`.

One-instruction-per-node rule. With the exceptions of `Ret` and
`FunctionPrologue` (the multi-step compound nodes documented above),
every emit-stage instruction maps to exactly one 6502 opcode. Any
higher-level operation (e.g. `Unary(Not, A)` -> EOR/CLC/ADC sequence)
must be lowered into atoms by an earlier pass before reaching emit.

Atomic arithmetic / flag instructions:
  - `ClearCarry` -> `CLC`; `SetCarry` -> `SEC`.
  - `Inc(dst)` / `Dec(dst)`: `dst` must be `Reg(X)` or `Reg(Y)` ->
    `INX`/`INY`/`DEX`/`DEY`. (Plain 6502 has no INA/DEA.)
  - `Push(src)` -> `PHA` (src must be `Reg(A)`).
  - `Pop(dst)`  -> `PLA` (dst must be `Reg(A)`).
  - `Xor(src1, src2, dst)` -> `EOR #imm`. dst must be `Reg(A)`; one
    of src1/src2 must be `Reg(A)`, the other an `Imm`.
  - `Add(src, dst)` -> `ADC <src>` (src is `Imm`/`Stack`/`Frame`, dst
    `Reg(A)`). Carry must already be set up by a preceding `ClearCarry`.
    Stack/Frame sources emit an LDY pair plus the ADC (the LDY is
    addressing-mode setup, not a separate logical step).
  - `Sub(src, dst)` -> `SBC <src>` (same; preceded by `SetCarry`).

Compound instructions invalid at emit (must be lowered earlier):
  - `Mul`, `Div`, `Mod` — will be lowered to `Call mul8` / `Call
    div8` once `Call` exists.

(`Unary` no longer exists at the asm AST level — `tac_to_asm`
lowers TAC `Unary` directly into `Mov`/`Xor`/`ClearCarry`/`Add`
atoms.)
"""

from __future__ import annotations

import asm_ast


# 0-indexed column positions (column 1 = index 0).
_OPCODE_COL = 3    # "column 4"
_OPERAND_COL = 9   # "column 10"

# Symbols for the soft stack pointer and frame pointer; the runtime
# header `equ`s each to its zero-page address.
_SSP = "SSP"
_FP = "FP"


def _instr_line(opcode: str, operand: str = "") -> str:
    line = " " * _OPCODE_COL + opcode.upper()
    if operand:
        pad = max(1, _OPERAND_COL - len(line))
        line += " " * pad + operand
    return line


def _check_byte(label: str, v: int) -> None:
    if not 0 <= v <= 255:
        raise ValueError(f"{label} {v} out of range for 6502 (expected 0..255)")


def _check_amt(amt: int) -> None:
    if not 0 <= amt <= 0xFFFF:
        raise ValueError(f"stack adjust {amt} out of range (expected 0..65535)")


def _reject_pseudo(op: asm_ast.Type_operand) -> None:
    """Pseudo operands must be eliminated before emit; the pseudo->stack
    replacement pass owns that. Reaching emit with one is a contract
    violation in an earlier pass, not a user-facing condition."""
    if isinstance(op, asm_ast.Pseudo):
        raise ValueError(
            f"Pseudo({op.name!r}) reached asm_emit; "
            "the pseudo->stack replacement pass must run first"
        )


def _reg_letter(r: asm_ast.Type_reg) -> str:
    match r:
        case asm_ast.A():
            return "A"
        case asm_ast.X():
            return "X"
        case asm_ast.Y():
            return "Y"
        case _:
            raise TypeError(f"unexpected reg: {r!r}")


def _emit_ssp_sub(amt: int) -> list[str]:
    """16-bit `SSP -= amt`. Clobbers A. Empty if amt == 0."""
    _check_amt(amt)
    if amt == 0:
        return []
    lo, hi = amt & 0xFF, (amt >> 8) & 0xFF
    return [
        _instr_line("SEC"),
        _instr_line("LDA", _SSP),
        _instr_line("SBC", f"#${lo:02X}"),
        _instr_line("STA", _SSP),
        _instr_line("LDA", f"{_SSP}+1"),
        _instr_line("SBC", f"#${hi:02X}"),
        _instr_line("STA", f"{_SSP}+1"),
    ]


def _emit_ssp_add(amt: int) -> list[str]:
    """16-bit `SSP += amt`. Clobbers A. Empty if amt == 0."""
    _check_amt(amt)
    if amt == 0:
        return []
    lo, hi = amt & 0xFF, (amt >> 8) & 0xFF
    return [
        _instr_line("CLC"),
        _instr_line("LDA", _SSP),
        _instr_line("ADC", f"#${lo:02X}"),
        _instr_line("STA", _SSP),
        _instr_line("LDA", f"{_SSP}+1"),
        _instr_line("ADC", f"#${hi:02X}"),
        _instr_line("STA", f"{_SSP}+1"),
    ]


def _indirect_addr(op: asm_ast.Type_operand) -> str:
    """ZP indirect-Y addressing string for a Stack or Frame operand."""
    if isinstance(op, asm_ast.Stack):
        return f"({_SSP}),Y"
    if isinstance(op, asm_ast.Frame):
        return f"({_FP}),Y"
    raise TypeError(f"not an indirect operand: {op!r}")


def _emit_load_y(off: int) -> str:
    _check_byte("offset", off)
    return _instr_line("LDY", f"#${off:02X}")


def _emit_indirect_load(off: int, addr_op: asm_ast.Type_operand) -> list[str]:
    """Read the byte at the indirect Stack/Frame position into A."""
    return [
        _emit_load_y(off),
        _instr_line("LDA", _indirect_addr(addr_op)),
    ]


def _emit_indirect_store(off: int, addr_op: asm_ast.Type_operand) -> list[str]:
    """Store A into the byte at the indirect Stack/Frame position."""
    return [
        _emit_load_y(off),
        _instr_line("STA", _indirect_addr(addr_op)),
    ]


def _emit_mov(src: asm_ast.Type_operand, dst: asm_ast.Type_operand) -> list[str]:
    _reject_pseudo(src)
    _reject_pseudo(dst)
    match src, dst:
        case asm_ast.Imm(value=v), asm_ast.Reg(reg=r):
            _check_byte("immediate", v)
            return [_instr_line(f"LD{_reg_letter(r)}", f"#${v:02X}")]
        case asm_ast.Reg(reg=asm_ast.X()), asm_ast.Reg(reg=asm_ast.A()):
            return [_instr_line("TXA")]
        case asm_ast.Reg(reg=asm_ast.Y()), asm_ast.Reg(reg=asm_ast.A()):
            return [_instr_line("TYA")]
        case asm_ast.Reg(reg=asm_ast.A()), asm_ast.Reg(reg=asm_ast.X()):
            return [_instr_line("TAX")]
        case asm_ast.Reg(reg=asm_ast.A()), asm_ast.Reg(reg=asm_ast.Y()):
            return [_instr_line("TAY")]
        case asm_ast.Imm(value=v), (
            asm_ast.Stack(offset=off) | asm_ast.Frame(offset=off)
        ) as dst_addr:
            _check_byte("immediate", v)
            return (
                [_instr_line("LDA", f"#${v:02X}")]
                + _emit_indirect_store(off, dst_addr)
            )
        case (
            asm_ast.Stack(offset=off) | asm_ast.Frame(offset=off)
        ) as src_addr, asm_ast.Reg(reg=asm_ast.A()):
            return _emit_indirect_load(off, src_addr)
        case asm_ast.Reg(reg=asm_ast.A()), (
            asm_ast.Stack(offset=off) | asm_ast.Frame(offset=off)
        ) as dst_addr:
            return _emit_indirect_store(off, dst_addr)
        case (
            asm_ast.Stack(offset=src_off) | asm_ast.Frame(offset=src_off)
        ) as src_addr, (
            asm_ast.Stack(offset=dst_off) | asm_ast.Frame(offset=dst_off)
        ) as dst_addr:
            return (
                _emit_indirect_load(src_off, src_addr)
                + _emit_indirect_store(dst_off, dst_addr)
            )
        case _:
            raise ValueError(f"cannot emit Mov(src={src!r}, dst={dst!r})")


def _check_local_bytes(m: int) -> None:
    if not 0 <= m <= 253:
        raise ValueError(
            f"local_bytes {m} out of range (expected 0..253; "
            "limited by LDY immediate for FP-slot addressing)"
        )


def _emit_set_fp_to_ssp() -> list[str]:
    """`FP = SSP`. Clobbers A."""
    return [
        _instr_line("LDA", _SSP),
        _instr_line("STA", _FP),
        _instr_line("LDA", f"{_SSP}+1"),
        _instr_line("STA", f"{_FP}+1"),
    ]


def _emit_set_ssp_to_fp_plus(amt: int) -> list[str]:
    """`SSP = FP + amt`. Clobbers A."""
    _check_amt(amt)
    if amt == 0:
        return [
            _instr_line("LDA", _FP),
            _instr_line("STA", _SSP),
            _instr_line("LDA", f"{_FP}+1"),
            _instr_line("STA", f"{_SSP}+1"),
        ]
    lo, hi = amt & 0xFF, (amt >> 8) & 0xFF
    return [
        _instr_line("CLC"),
        _instr_line("LDA", _FP),
        _instr_line("ADC", f"#${lo:02X}"),
        _instr_line("STA", _SSP),
        _instr_line("LDA", f"{_FP}+1"),
        _instr_line("ADC", f"#${hi:02X}"),
        _instr_line("STA", f"{_SSP}+1"),
    ]


def _emit_save_fp_into_slot(m: int) -> list[str]:
    """Write the current FP into the slot at `SSP+M+1` (low) /
    `SSP+M+2` (high). Clobbers A and Y. Requires `M <= 253`."""
    _check_local_bytes(m)
    return [
        _instr_line("LDY", f"#${m + 1:02X}"),
        _instr_line("LDA", _FP),
        _instr_line("STA", f"({_SSP}),Y"),
        _instr_line("INY"),
        _instr_line("LDA", f"{_FP}+1"),
        _instr_line("STA", f"({_SSP}),Y"),
    ]


def _emit_restore_fp_from_slot(m: int) -> list[str]:
    """Read the 2 bytes at `FP+M+1` / `FP+M+2` back into the FP
    register. Uses X as a 1-byte scratch for the low byte: we can't
    write to FP between the two reads because `(FP),Y` uses both
    bytes of FP as the indirect base. Clobbers A, X, Y. Requires
    `M <= 253`."""
    _check_local_bytes(m)
    return [
        _instr_line("LDY", f"#${m + 1:02X}"),
        _instr_line("LDA", f"({_FP}),Y"),
        _instr_line("TAX"),
        _instr_line("INY"),
        _instr_line("LDA", f"({_FP}),Y"),
        _instr_line("STA", f"{_FP}+1"),
        _instr_line("STX", _FP),
    ]


def _check_dst_is_a(dst: asm_ast.Type_operand, op_name: str) -> None:
    """Many ops can only land their result in the accumulator."""
    if not (isinstance(dst, asm_ast.Reg) and isinstance(dst.reg, asm_ast.A)):
        raise ValueError(f"{op_name} dst must be Reg(A), got {dst!r}")


def _is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _emit_add(src: asm_ast.Type_operand, dst: asm_ast.Type_operand) -> list[str]:
    """At emit, Add is the single ADC instruction (with addressing-mode
    setup for indirect-Y sources). Carry is the caller's job — a
    preceding ClearCarry."""
    _reject_pseudo(src)
    _reject_pseudo(dst)
    _check_dst_is_a(dst, "Add")
    match src:
        case asm_ast.Imm(value=v):
            _check_byte("immediate", v)
            return [_instr_line("ADC", f"#${v:02X}")]
        case asm_ast.Stack() | asm_ast.Frame():
            return [
                _emit_load_y(src.offset),
                _instr_line("ADC", _indirect_addr(src)),
            ]
        case _:
            raise ValueError(f"cannot emit Add(src={src!r}, dst={dst!r})")


def _emit_sub(src: asm_ast.Type_operand, dst: asm_ast.Type_operand) -> list[str]:
    """At emit, Sub is the single SBC instruction. Carry must be set
    by a preceding SetCarry (SBC subtracts an extra 1 if carry is clear)."""
    _reject_pseudo(src)
    _reject_pseudo(dst)
    _check_dst_is_a(dst, "Sub")
    match src:
        case asm_ast.Imm(value=v):
            _check_byte("immediate", v)
            return [_instr_line("SBC", f"#${v:02X}")]
        case asm_ast.Stack() | asm_ast.Frame():
            return [
                _emit_load_y(src.offset),
                _instr_line("SBC", _indirect_addr(src)),
            ]
        case _:
            raise ValueError(f"cannot emit Sub(src={src!r}, dst={dst!r})")


def _emit_inc(dst: asm_ast.Type_operand) -> list[str]:
    _reject_pseudo(dst)
    match dst:
        case asm_ast.Reg(reg=asm_ast.X()):
            return [_instr_line("INX")]
        case asm_ast.Reg(reg=asm_ast.Y()):
            return [_instr_line("INY")]
        case _:
            raise ValueError(
                f"Inc dst must be Reg(X) or Reg(Y), got {dst!r}"
            )


def _emit_dec(dst: asm_ast.Type_operand) -> list[str]:
    _reject_pseudo(dst)
    match dst:
        case asm_ast.Reg(reg=asm_ast.X()):
            return [_instr_line("DEX")]
        case asm_ast.Reg(reg=asm_ast.Y()):
            return [_instr_line("DEY")]
        case _:
            raise ValueError(
                f"Dec dst must be Reg(X) or Reg(Y), got {dst!r}"
            )


def _emit_push(src: asm_ast.Type_operand) -> list[str]:
    _reject_pseudo(src)
    if not _is_reg_a(src):
        raise ValueError(f"Push src must be Reg(A), got {src!r}")
    return [_instr_line("PHA")]


def _emit_pop(dst: asm_ast.Type_operand) -> list[str]:
    _reject_pseudo(dst)
    if not _is_reg_a(dst):
        raise ValueError(f"Pop dst must be Reg(A), got {dst!r}")
    return [_instr_line("PLA")]


def _emit_xor(
    src1: asm_ast.Type_operand,
    src2: asm_ast.Type_operand,
    dst: asm_ast.Type_operand,
) -> list[str]:
    _reject_pseudo(src1)
    _reject_pseudo(src2)
    _reject_pseudo(dst)
    _check_dst_is_a(dst, "Xor")
    # 6502 EOR is "A = A XOR <imm-or-mem>"; at emit we accept only
    # the immediate form, which means one src must be Reg(A) and the
    # other an Imm. Order doesn't matter (XOR is commutative).
    if _is_reg_a(src1) and isinstance(src2, asm_ast.Imm):
        v = src2.value
    elif _is_reg_a(src2) and isinstance(src1, asm_ast.Imm):
        v = src1.value
    else:
        raise ValueError(
            "Xor srcs must be Reg(A) and Imm in some order, "
            f"got src1={src1!r}, src2={src2!r}"
        )
    _check_byte("immediate", v)
    return [_instr_line("EOR", f"#${v:02X}")]


def _emit_function_prologue(arg_bytes: int, local_bytes: int) -> list[str]:
    if arg_bytes + local_bytes == 0:
        return []
    # Allocate locals + saved-FP slot (args were caller-pushed and
    # don't need allocation). Save the caller's FP into the slot
    # just above the locals, then capture SSP into FP.
    return (
        _emit_ssp_sub(local_bytes + 2)
        + _emit_save_fp_into_slot(local_bytes)
        + _emit_set_fp_to_ssp()
    )


def _emit_ret(arg_bytes: int, local_bytes: int) -> list[str]:
    if arg_bytes + local_bytes == 0:
        return [_instr_line("RTS")]
    # Compute the new SSP directly from FP (one 16-bit add), restore
    # FP from its slot via (FP),Y, then RTS. The whole thing is
    # PHA/PLA-wrapped so the return value in A survives.
    rewind = arg_bytes + local_bytes + 2
    return (
        [_instr_line("PHA")]
        + _emit_set_ssp_to_fp_plus(rewind)
        + _emit_restore_fp_from_slot(local_bytes)
        + [_instr_line("PLA"), _instr_line("RTS")]
    )


def emit_instruction(instr: asm_ast.Type_instruction) -> list[str]:
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            return _emit_mov(src, dst)
        case asm_ast.FunctionPrologue(arg_bytes=ab, local_bytes=lb):
            return _emit_function_prologue(ab, lb)
        case asm_ast.Ret(arg_bytes=ab, local_bytes=lb):
            return _emit_ret(ab, lb)
        case asm_ast.Add(src=src, dst=dst):
            return _emit_add(src, dst)
        case asm_ast.Sub(src=src, dst=dst):
            return _emit_sub(src, dst)
        case asm_ast.ClearCarry():
            return [_instr_line("CLC")]
        case asm_ast.SetCarry():
            return [_instr_line("SEC")]
        case asm_ast.Inc(dst=dst):
            return _emit_inc(dst)
        case asm_ast.Dec(dst=dst):
            return _emit_dec(dst)
        case asm_ast.Push(src=src):
            return _emit_push(src)
        case asm_ast.Pop(dst=dst):
            return _emit_pop(dst)
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            return _emit_xor(s1, s2, dst)
        case asm_ast.Mul() | asm_ast.Div() | asm_ast.Mod():
            raise ValueError(
                "Mul/Div/Mod are not valid at emit; will be lowered to "
                f"a Call (mul8/div8) by an earlier pass. Got {instr!r}"
            )
        case _:
            raise TypeError(f"unexpected instruction: {instr!r}")


def emit_function(fn: asm_ast.Type_function_definition) -> list[str]:
    match fn:
        case asm_ast.Function(name=name, instructions=instrs):
            # Label in col 1; SUBROUTINE directive in col 4 (same column as
            # opcodes); blank line before instructions.
            lines = [f"{name}:", _instr_line("SUBROUTINE")]
            if instrs:
                lines.append("")
                for instr in instrs:
                    lines.extend(emit_instruction(instr))
            return lines
        case _:
            raise TypeError(f"unexpected function: {fn!r}")


def emit_program(prog: asm_ast.Type_program) -> str:
    match prog:
        case asm_ast.Program(function_definition=fn):
            return "\n".join(emit_function(fn)) + "\n"
        case _:
            raise TypeError(f"unexpected program: {prog!r}")


