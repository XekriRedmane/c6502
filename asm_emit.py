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
    emits a leading `; prologue: N arg bytes, M local bytes` comment
    so the boilerplate region is easy to pick out, allocates `M+2`
    bytes (locals + saved-FP slot), writes the caller's FP into the
    slot at `SSP+M+1` (low) and `SSP+M+2` (high), sets `FP = SSP`,
    and trails with a blank line that separates the prologue from the
    body. Clobbers A and Y. For `N+M == 0` it emits nothing (no FP
    setup needed when the function has no locals or args). Bounded
    `M <= 253` so `LDY #(M+2)` fits in a byte. Args themselves were
    pushed by the caller before JSR; the prologue doesn't allocate
    them.
  - `Ret(arg_bytes=N, local_bytes=M, save_a=…)` for `N+M > 0`: leads
    with a blank line and `; epilogue` comment to mark the
    boilerplate region; if `save_a=True` (the integer-return
    convention), PHAs the return value first. Then `SSP = FP + (N +
    M + 2)` in one shot (the +2 is the saved-FP slot; the result is
    the caller's pre-arg-push SSP, so the caller needs no per-call
    cleanup). Then reads the saved FP via `(FP),Y` with `Y=M+1`
    (low) and `Y=M+2` (high), stashing the low byte in X across the
    high read so we don't corrupt the FP register's role as the
    indirect base mid-read. Closes with `PLA, RTS` (when save_a=True)
    or just `RTS` (when save_a=False — FP returns leave the result
    in HARGS, which the SSP/FP arithmetic doesn't touch, so there's
    nothing to preserve in A across the rewind). For `N+M == 0` it
    just emits `RTS` regardless of save_a.

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
  - `Xor(src1, src2, dst)` -> `EOR <src>`. dst must be `Reg(A)`; one
    of src1/src2 must be `Reg(A)`, the other an `Imm`/`Stack`/`Frame`.
    The non-A operand picks the addressing mode; Stack/Frame go through
    LDY indirect-Y like Add/Sub. Carry / sign flags are not affected.
  - `And(src, dst)` -> `AND <src>`; same operand shape as `Add`.
  - `Or(src, dst)`  -> `ORA <src>`; same operand shape as `Add`.
  - `ArithmeticShiftLeft(dst)` -> `ASL A`. dst must be `Reg(A)`.
  - `LogicalShiftRight(dst)`   -> `LSR A`. dst must be `Reg(A)`.
  - `RotateLeft(dst)`          -> `ROL A`. dst must be `Reg(A)`.
  - `RotateRight(dst)`         -> `ROR A`. dst must be `Reg(A)`.
    The 6502 has accumulator and memory addressing modes for these,
    but no indirect-Y mode — so soft-stack values can't be shifted in
    place; codegen has to load to A, shift, then store. Memory dst
    support could be added later for in-place shifts of zero-page
    locations (useful for 16-bit shift sequences with carry).
  - `Add(src, dst)` -> `ADC <src>` (src is `Imm`/`Stack`/`Frame`, dst
    `Reg(A)`). Carry must already be set up by a preceding `ClearCarry`.
    Stack/Frame sources emit an LDY pair plus the ADC (the LDY is
    addressing-mode setup, not a separate logical step).
  - `Sub(src, dst)` -> `SBC <src>` (same; preceded by `SetCarry`).
  - `Call(name)` -> `JSR <name>`.
  - `Jump(target)` -> `JMP <target>`.
  - `Branch(cond, target)` -> `B<cond> <target>` where `cond` is one of
    `CC`/`CS`/`EQ`/`MI`/`NE`/`PL`/`VC`/`VS`. The 6502's branches are
    PC-relative (signed 8-bit displacement), but the assembler resolves
    that from the target label — emit just writes the symbolic name.
  - `Label(name)` -> `<name>:` at column 1. No opcode column. Lets a
    `Jump`/`Branch` resolve to a position inside the same function.
  - `Compare(left, right)` -> `CMP`/`CPX`/`CPY` depending on whether
    `left` is `Reg(A)`/`Reg(X)`/`Reg(Y)`. `right` is `Imm`/`Stack`/`Frame`/
    `Data` for CMP (Stack/Frame go through LDY indirect-Y like Add/Sub;
    Data uses 6502 absolute addressing, no LDY needed). For CPX/CPY,
    `right` is `Imm` or `Data` (the 6502's CPX/CPY have absolute mode
    but no indirect-Y, so soft-stack operands can't be compared against
    X or Y directly — load to A and use CMP instead). Sets the same
    N/Z/C flags an `SBC left - right` would, without writing the
    result anywhere.

`Data(name, offset)` operand. References a static-storage object by
symbolic name. Lowers to 6502 absolute addressing — `LDA name`,
`STA name`, `ADC name`, `EOR name`, etc. for `offset == 0`, and
`LDA name+offset` etc. for nonzero offsets. The assembler resolves
the symbol+offset to a fixed address; no LDY indirect-Y preamble
is needed (the address is known at assembly time, not runtime).
`Data` is legal anywhere `Stack`/`Frame` is legal as a memory
operand: read sources for arithmetic / logic / compare ops, both
sides of a `Mov`. The matching `replace_pseudoregisters` pass
produces `Data` operands from any `Pseudo` whose name is a top-
level `StaticVariable`; the `offset` lets a single Pseudo address
the high byte of a 2-byte (`Long`) static via `Data(name, offset=1)`.

`StaticVariable(name, is_global, init)` top-level node. Emitted as
`<name>:` on its own line followed by `DC.B $XX` on the next, where
`XX` is the byte init value. (Mnemonics are uppercased per the
`_instr_line` convention — including the `dc.b` directive name.)
The `is_global` flag is recorded on the IR but not yet surfaced in
the asm output: dasm has no native "export" / "module-private"
distinction, and statics get unique names anyway (block-scope
statics arrive with `@<N>.<orig>` from identifier_resolution; file-
scope INTERNAL keeps the source spelling but the user wrote
`static`). When multi-TU linking lands this is where a
`.globl name` directive (or equivalent) would appear under
`is_global=True`.

(`Unary` no longer exists at the asm AST level — `tac_to_asm`
lowers TAC `Unary` directly into `Mov`/`Xor`/`ClearCarry`/`Add`
atoms. Likewise `Mul`/`Div`/`Mod`/`LeftShift`/`RightShift` are
TAC-only concepts; `tac_to_asm` lowers each to `Mov`s into the
shared `HARGS` zero-page block, a `Call` to the appropriate
runtime helper (mul8/mul16/divmod8/divmod16/asl8/asl16/asr8/asr16,
keyed off operand size), and `Mov`s reading the result back out.)
"""

from __future__ import annotations

import struct

import asm_ast


# 0-indexed column positions (column 1 = index 0).
_OPCODE_COL = 3    # "column 4"
_OPERAND_COL = 9   # "column 10"

# Symbols for the soft stack pointer and frame pointer; the runtime
# header `equ`s each to its zero-page address.
_SSP = "SSP"
_FP = "FP"
# DPTR is the dereference / scratch indirect-pointer pair, reserved
# at zero-page `$1C`/`$1D` by the runtime header. Used by the
# `Indirect(off)` operand for `(DPTR),Y` indirect-Y access — the
# addressing mode that backs Load and Store TAC ops.
_DPTR = "DPTR"


def _instr_line(opcode: str, operand: str = "") -> str:
    line = " " * _OPCODE_COL + opcode.upper()
    if operand:
        pad = max(1, _OPERAND_COL - len(line))
        line += " " * pad + operand
    return line


def _comment_line(text: str) -> str:
    """Block-level comment at opcode column. Used by the prologue and
    epilogue to mark the boilerplate regions of a function."""
    return " " * _OPCODE_COL + "; " + text


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


def _cond_suffix(c: asm_ast.Type_condition) -> str:
    """Two-letter suffix for a 6502 branch opcode (`CC` -> `BCC` etc.).
    Matches the constructor name in the asm IR exactly so adding a
    new condition is just adding a new ASDL constructor."""
    match c:
        case asm_ast.CC():
            return "CC"
        case asm_ast.CS():
            return "CS"
        case asm_ast.EQ():
            return "EQ"
        case asm_ast.MI():
            return "MI"
        case asm_ast.NE():
            return "NE"
        case asm_ast.PL():
            return "PL"
        case asm_ast.VC():
            return "VC"
        case asm_ast.VS():
            return "VS"
        case _:
            raise TypeError(f"unexpected condition: {c!r}")


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
    """ZP indirect-Y addressing string for a Stack / Frame /
    Indirect operand. Each kind picks a different ZP pointer pair
    (SSP / FP / DPTR); the `,Y` index is supplied separately by an
    LDY before the access (see _emit_load_y)."""
    if isinstance(op, asm_ast.Stack):
        return f"({_SSP}),Y"
    if isinstance(op, asm_ast.Frame):
        return f"({_FP}),Y"
    if isinstance(op, asm_ast.Indirect):
        return f"({_DPTR}),Y"
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


def _is_memory_operand(op: asm_ast.Type_operand) -> bool:
    """True iff `op` is a memory operand (Stack / Frame / Data /
    Indirect) — i.e., something that needs a load/store opcode
    rather than a transfer or immediate. Used at the dispatch
    boundary in Mov."""
    return isinstance(
        op,
        (asm_ast.Stack, asm_ast.Frame, asm_ast.Data, asm_ast.Indirect),
    )


def _data_addr(d: asm_ast.Data) -> str:
    """Absolute-addressing operand string for a Data reference. The
    `offset` field selects the byte within a multi-byte static — 0
    for the low byte (and the only byte of an Int static), 1 for the
    high byte of a Long. We render `name+offset` for nonzero offsets
    and bare `name` for the common offset-0 case."""
    if d.offset == 0:
        return d.name
    return f"{d.name}+{d.offset}"


def _emit_memop_load(
    addr_op: asm_ast.Type_operand, opcode: str = "LDA",
) -> list[str]:
    """Read the byte addressed by `addr_op` into A (or another reg
    if a different opcode is passed; the caller picks). Indirect-Y
    for Stack/Frame, absolute for Data."""
    if isinstance(addr_op, asm_ast.Data):
        return [_instr_line(opcode, _data_addr(addr_op))]
    return [
        _emit_load_y(addr_op.offset),
        _instr_line(opcode, _indirect_addr(addr_op)),
    ]


def _emit_memop_store(addr_op: asm_ast.Type_operand) -> list[str]:
    """Store A into the byte addressed by `addr_op`. Indirect-Y for
    Stack/Frame, absolute for Data."""
    if isinstance(addr_op, asm_ast.Data):
        return [_instr_line("STA", _data_addr(addr_op))]
    return [
        _emit_load_y(addr_op.offset),
        _instr_line("STA", _indirect_addr(addr_op)),
    ]


def _imm_label_text(op: asm_ast.Type_operand) -> str:
    """Render an ImmLabelLow / ImmLabelHigh operand as a dasm
    immediate. Uses dasm's `<` (low byte) and `>` (high byte) label
    operators: `#<name` for the low half of `name`'s address,
    `#>name` for the high half. A non-zero offset is parenthesised
    so dasm's parser binds the `<` / `>` to the full `name+offset`
    sum, not just to `name`."""
    if isinstance(op, asm_ast.ImmLabelLow):
        prefix = "<"
    elif isinstance(op, asm_ast.ImmLabelHigh):
        prefix = ">"
    else:
        raise TypeError(f"not an ImmLabel operand: {op!r}")
    if op.offset == 0:
        return f"#{prefix}{op.name}"
    return f"#{prefix}({op.name}+{op.offset})"


def _is_imm_label(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, (asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh))


def _emit_mov(src: asm_ast.Type_operand, dst: asm_ast.Type_operand) -> list[str]:
    _reject_pseudo(src)
    _reject_pseudo(dst)
    # Register-register and immediate-to-register cases stay as
    # special cases (different opcodes per pair); the memory-operand
    # cases (Stack/Frame/Data) are unified via `_emit_memop_*`.
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
    # ImmLabel paths. Same shape as Imm — load to A as immediate,
    # then optionally store. Restricted to A-as-destination since
    # only `LDA #<name` / `LDA #>name` are used by LoadAddress
    # today; X/Y immediate variants would compose the same way but
    # aren't needed.
    if _is_imm_label(src) and _is_reg_a(dst):
        return [_instr_line("LDA", _imm_label_text(src))]
    if _is_imm_label(src) and _is_memory_operand(dst):
        return (
            [_instr_line("LDA", _imm_label_text(src))]
            + _emit_memop_store(dst)
        )
    # Memory-operand paths. `_is_memory_operand` covers Stack, Frame,
    # Data, and Indirect — the addressing-mode difference is hidden
    # inside `_emit_memop_load` / `_emit_memop_store`.
    if isinstance(src, asm_ast.Imm) and _is_memory_operand(dst):
        _check_byte("immediate", src.value)
        return (
            [_instr_line("LDA", f"#${src.value:02X}")]
            + _emit_memop_store(dst)
        )
    if _is_memory_operand(src) and _is_reg_a(dst):
        return _emit_memop_load(src)
    if _is_reg_a(src) and _is_memory_operand(dst):
        return _emit_memop_store(dst)
    if _is_memory_operand(src) and _is_memory_operand(dst):
        return _emit_memop_load(src) + _emit_memop_store(dst)
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


def _emit_acc_arith_src(opcode: str, src: asm_ast.Type_operand) -> list[str]:
    """Common emit for ADC/SBC/AND/ORA/EOR sources: the destination is
    always Reg(A); the source can be Imm (direct), Stack/Frame
    (indirect-Y), or Data (absolute). The opcode picks the operation
    and the source picks the addressing mode."""
    match src:
        case asm_ast.Imm(value=v):
            _check_byte("immediate", v)
            return [_instr_line(opcode, f"#${v:02X}")]
        case asm_ast.Stack() | asm_ast.Frame():
            return [
                _emit_load_y(src.offset),
                _instr_line(opcode, _indirect_addr(src)),
            ]
        case asm_ast.Data():
            return [_instr_line(opcode, _data_addr(src))]
        case _:
            raise ValueError(
                f"unsupported {opcode} source: {src!r}"
            )


def _emit_add(src: asm_ast.Type_operand, dst: asm_ast.Type_operand) -> list[str]:
    """At emit, Add is the single ADC instruction (with addressing-mode
    setup for indirect-Y sources). Carry is the caller's job — a
    preceding ClearCarry."""
    _reject_pseudo(src)
    _reject_pseudo(dst)
    _check_dst_is_a(dst, "Add")
    return _emit_acc_arith_src("ADC", src)


def _emit_sub(src: asm_ast.Type_operand, dst: asm_ast.Type_operand) -> list[str]:
    """At emit, Sub is the single SBC instruction. Carry must be set
    by a preceding SetCarry (SBC subtracts an extra 1 if carry is clear)."""
    _reject_pseudo(src)
    _reject_pseudo(dst)
    _check_dst_is_a(dst, "Sub")
    return _emit_acc_arith_src("SBC", src)


def _emit_acc_logic(
    opcode: str,
    op_name: str,
    src: asm_ast.Type_operand,
    dst: asm_ast.Type_operand,
) -> list[str]:
    """Common emit for AND/ORA — both implicitly use A as one operand
    and as the destination. Same operand shape as Add/Sub but no carry
    setup is needed (these don't touch C)."""
    _reject_pseudo(src)
    _reject_pseudo(dst)
    _check_dst_is_a(dst, op_name)
    return _emit_acc_arith_src(opcode, src)


def _emit_acc_shift(
    opcode: str, op_name: str, dst: asm_ast.Type_operand,
) -> list[str]:
    """Common emit for ASL/LSR/ROL/ROR. The 6502 supports both
    accumulator and memory addressing for these, but soft-stack
    operands live behind indirect-Y which isn't a supported mode for
    the shift family — so today the only legal dst is Reg(A)."""
    _reject_pseudo(dst)
    if not _is_reg_a(dst):
        raise ValueError(f"{op_name} dst must be Reg(A), got {dst!r}")
    return [_instr_line(opcode, "A")]


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
    # 6502 EOR is "A = A XOR <imm-or-mem>". One src must be Reg(A);
    # the other carries the addressing mode (Imm direct, or Stack/
    # Frame indirect-Y). Order doesn't matter (XOR is commutative).
    if _is_reg_a(src1):
        other = src2
    elif _is_reg_a(src2):
        other = src1
    else:
        raise ValueError(
            "Xor srcs must include Reg(A); "
            f"got src1={src1!r}, src2={src2!r}"
        )
    return _emit_acc_arith_src("EOR", other)


def _emit_compare(
    left: asm_ast.Type_operand, right: asm_ast.Type_operand,
) -> list[str]:
    """Compare(left, right) -> CMP/CPX/CPY. The register on the left
    picks the opcode; the right side carries the addressing mode.
    CPX/CPY support immediate and absolute addressing (so Imm and
    Data work for any left register), but they lack indirect-Y, so
    Stack/Frame is only legal when left is A."""
    _reject_pseudo(left)
    _reject_pseudo(right)
    if not isinstance(left, asm_ast.Reg):
        raise ValueError(f"Compare left must be a register, got {left!r}")
    match left.reg:
        case asm_ast.A():
            opcode = "CMP"
        case asm_ast.X():
            opcode = "CPX"
        case asm_ast.Y():
            opcode = "CPY"
        case _:
            raise TypeError(f"unexpected reg: {left.reg!r}")
    match right:
        case asm_ast.Imm(value=v):
            _check_byte("immediate", v)
            return [_instr_line(opcode, f"#${v:02X}")]
        case asm_ast.Data():
            return [_instr_line(opcode, _data_addr(right))]
        case asm_ast.Stack() | asm_ast.Frame():
            if opcode != "CMP":
                raise ValueError(
                    f"Compare with left={left!r} requires Imm or Data "
                    "right (CPX/CPY have no indirect-Y addressing mode); "
                    f"got {right!r}"
                )
            return [
                _emit_load_y(right.offset),
                _instr_line(opcode, _indirect_addr(right)),
            ]
        case _:
            raise ValueError(
                f"cannot emit Compare(left={left!r}, right={right!r})"
            )


def _emit_function_prologue(arg_bytes: int, local_bytes: int) -> list[str]:
    if arg_bytes + local_bytes == 0:
        return []
    # Allocate locals + saved-FP slot (args were caller-pushed and
    # don't need allocation). Save the caller's FP into the slot
    # just above the locals, then capture SSP into FP. The leading
    # comment + trailing blank line mark the prologue's boilerplate
    # region so the body is easy to pick out visually.
    header = _comment_line(
        f"prologue: {arg_bytes} arg bytes, {local_bytes} local bytes"
    )
    return (
        [header]
        + _emit_ssp_sub(local_bytes + 2)
        + _emit_save_fp_into_slot(local_bytes)
        + _emit_set_fp_to_ssp()
        + [""]
    )


def _shift_offset(
    op: asm_ast.Type_operand, k: int,
) -> asm_ast.Type_operand:
    """Return a copy of `op` with its byte offset bumped by `k`. Used
    by `_emit_load_address` to reach the high byte of a 2-byte dst
    after writing the low byte."""
    if isinstance(op, asm_ast.Frame):
        return asm_ast.Frame(offset=op.offset + k)
    if isinstance(op, asm_ast.Stack):
        return asm_ast.Stack(offset=op.offset + k)
    if isinstance(op, asm_ast.Data):
        return asm_ast.Data(name=op.name, offset=op.offset + k)
    raise TypeError(f"can't shift offset on operand {op!r}")


def _emit_load_address(
    src: asm_ast.Type_operand, dst: asm_ast.Type_operand,
) -> list[str]:
    """`LoadAddress(src, dst)` — write the 2-byte address of `src`
    into `dst` (a 2-byte memory operand). Two cases, dispatched by
    `src`'s resolved kind:

      Data(name, off)   — `src` is a static-storage object; its
                          address is link-time known. Load the low /
                          high halves as immediates via dasm's
                          `<` / `>` label operators:
                              LDA  #<(name+off)
                              STA  dst.lo
                              LDA  #>(name+off)
                              STA  dst.hi

      Frame(off)        — `src` is a local or param at FP-relative
                          offset `off`. Compute `FP + off` as a
                          16-bit add. ADC's carry chains the high
                          byte automatically; the SEC isn't needed
                          because we want CLC-add semantics.
                              CLC
                              LDA  FP
                              ADC  #off
                              STA  dst.lo
                              LDA  FP+1
                              ADC  #0
                              STA  dst.hi

    `dst` must be a memory operand (Frame / Stack / Data); the
    caller should have resolved any Pseudo before reaching here.
    Stack-source LoadAddress isn't supported (no use case yet —
    address-of a callee-side stack arg would be SSP-relative)."""
    if not _is_memory_operand(dst):
        raise TypeError(f"LoadAddress dst must be a memory operand, got {dst!r}")
    dst_hi = _shift_offset(dst, 1)
    reg_a = asm_ast.Reg(reg=asm_ast.A())
    if isinstance(src, asm_ast.Data):
        return (
            _emit_mov(asm_ast.ImmLabelLow(name=src.name, offset=src.offset), reg_a)
            + _emit_memop_store(dst)
            + _emit_mov(asm_ast.ImmLabelHigh(name=src.name, offset=src.offset), reg_a)
            + _emit_memop_store(dst_hi)
        )
    if isinstance(src, asm_ast.Frame):
        _check_byte("LoadAddress offset", src.offset)
        return [
            _instr_line("CLC"),
            _instr_line("LDA", _FP),
            _instr_line("ADC", f"#${src.offset:02X}"),
        ] + _emit_memop_store(dst) + [
            _instr_line("LDA", f"{_FP}+1"),
            _instr_line("ADC", "#$00"),
        ] + _emit_memop_store(dst_hi)
    raise TypeError(
        f"LoadAddress src must be Data or Frame (resolved Pseudo); "
        f"got {src!r}"
    )


def _emit_ret(arg_bytes: int, local_bytes: int, save_a: bool) -> list[str]:
    if arg_bytes + local_bytes == 0:
        # No frame to tear down — A wasn't going to get clobbered, so
        # `save_a` is irrelevant here.
        return [_instr_line("RTS")]
    # Compute the new SSP directly from FP (one 16-bit add), restore
    # FP from its slot via (FP),Y, then RTS. Leading blank + `;
    # epilogue` comment separate the boilerplate from the body above.
    #
    # When save_a=True (Int / Long returns), bracket the SSP/FP
    # arithmetic with PHA/PLA so the return value in A survives the
    # 16-bit add. When save_a=False (FP returns), skip the PHA/PLA —
    # the return value is already in HARGS, and HARGS isn't touched
    # by the SSP/FP arithmetic, so there's nothing to preserve.
    rewind = arg_bytes + local_bytes + 2
    body = (
        _emit_set_ssp_to_fp_plus(rewind)
        + _emit_restore_fp_from_slot(local_bytes)
    )
    head = ["", _comment_line("epilogue")]
    if save_a:
        return head + [_instr_line("PHA")] + body + [
            _instr_line("PLA"), _instr_line("RTS"),
        ]
    return head + body + [_instr_line("RTS")]


def emit_instruction(instr: asm_ast.Type_instruction) -> list[str]:
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            return _emit_mov(src, dst)
        case asm_ast.FunctionPrologue(arg_bytes=ab, local_bytes=lb):
            return _emit_function_prologue(ab, lb)
        case asm_ast.Ret(arg_bytes=ab, local_bytes=lb, save_a=sa):
            return _emit_ret(ab, lb, sa)
        case asm_ast.LoadAddress(src=src, dst=dst):
            return _emit_load_address(src, dst)
        case asm_ast.AllocateStack(bytes=n):
            # Caller-side soft-stack frame allocation for a call
            # site: subtract `n` from SSP (16-bit). The same
            # `_emit_ssp_sub` helper that drives the prologue's
            # space-for-locals reservation. The caller doesn't have
            # to undo this — the callee's epilogue rewinds SSP all
            # the way back to the caller's pre-call value.
            return _emit_ssp_sub(n)
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
        case asm_ast.And(src=src, dst=dst):
            return _emit_acc_logic("AND", "And", src, dst)
        case asm_ast.Or(src=src, dst=dst):
            return _emit_acc_logic("ORA", "Or", src, dst)
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            return _emit_acc_shift("ASL", "ArithmeticShiftLeft", dst)
        case asm_ast.LogicalShiftRight(dst=dst):
            return _emit_acc_shift("LSR", "LogicalShiftRight", dst)
        case asm_ast.RotateLeft(dst=dst):
            return _emit_acc_shift("ROL", "RotateLeft", dst)
        case asm_ast.RotateRight(dst=dst):
            return _emit_acc_shift("ROR", "RotateRight", dst)
        case asm_ast.Call(name=name):
            return [_instr_line("JSR", name)]
        case asm_ast.Jump(target=target):
            return [_instr_line("JMP", target)]
        case asm_ast.Branch(cond=cond, target=target):
            return [_instr_line(f"B{_cond_suffix(cond)}", target)]
        case asm_ast.Label(name=name):
            return [f"{name}:"]
        case asm_ast.Compare(left=left, right=right):
            return _emit_compare(left, right)
        case _:
            raise TypeError(f"unexpected instruction: {instr!r}")


def emit_function(fn: asm_ast.Function) -> list[str]:
    match fn:
        case asm_ast.Function(name=name, instructions=instrs):
            # Label in col 1; SUBROUTINE directive in col 4 (same column as
            # opcodes); blank line before instructions. Consecutive blank
            # lines are collapsed — the prologue's trailing blank and
            # the epilogue's leading blank otherwise pile up when a
            # function has no body between them.
            lines = [f"{name}:", _instr_line("SUBROUTINE")]
            if instrs:
                lines.append("")
                for instr in instrs:
                    for line in emit_instruction(instr):
                        if line == "" and lines and lines[-1] == "":
                            continue
                        lines.append(line)
            return lines
        case _:
            raise TypeError(f"unexpected function: {fn!r}")


def emit_static_variable(sv: asm_ast.StaticVariable) -> list[str]:
    """Render a top-level static-storage object as a label followed
    by one assembler directive per static_init item in `sv.init`.
    The list is in source-byte order, so successive items lay
    down sequentially under the variable's label:

        # scalar, e.g. IntInit(int=N)
        <name>:
            dc.b $XX

        # array of three IntInits
        <name>:
            dc.b $X1
            dc.b $X2
            dc.b $X3

    The init's variant determines the cell width. dasm's `dc.w`
    emits 2 bytes in little-endian order, `dc.l` emits 4 bytes
    likewise, matching the rest of the soft-stack memory model
    (low byte at the symbol's address, high byte at the higher
    addresses). For DoubleInit we emit two `dc.l`s — low half then
    high half — because dasm has no native 8-byte directive. The
    float value is packed into IEEE 754 bytes here via
    `struct.pack`; the resulting little-endian bytes are repacked
    as 32-bit big-endian-displayed dwords for the dc.l constants
    (the assembler converts them back to little-endian on output,
    so the in-memory layout matches the original bytes exactly).

    Out-of-range values raise via `_check_byte` / `_check_word`.

    `is_global` rides on the IR but doesn't yet alter the emit:
    dasm has no native module-private vs. exported distinction, and
    block-scope statics already arrive with unique `@<N>.<orig>`
    names so cross-function shadowing isn't an issue. A future
    multi-TU build would emit a `.globl name` directive here under
    `is_global=True`.
    """
    if not sv.init:
        raise ValueError(f"static variable {sv.name!r} has no init")
    lines: list[str] = [f"{sv.name}:"]
    for item in sv.init:
        match item:
            case asm_ast.IntInit(int=v):
                _check_byte(f"init for {sv.name!r}", v)
                lines.append(_instr_line("dc.b", f"${v:02X}"))
            case asm_ast.LongInit(int=v):
                _check_word(f"init for {sv.name!r}", v)
                # Mask to 16 bits so signed-negative values render as
                # their two's-complement bit pattern (e.g. -1 → $FFFF).
                lines.append(_instr_line("dc.w", f"${v & 0xFFFF:04X}"))
            case asm_ast.FloatInit(float=v):
                # Pack as IEEE 754 single-precision, little-endian (`<f`).
                # Reinterpret the 4-byte sequence as a 32-bit unsigned
                # int (also little-endian) so dasm's `dc.l` lays the
                # exact same byte sequence down.
                (word,) = struct.unpack("<I", struct.pack("<f", v))
                lines.append(_instr_line("dc.l", f"${word:08X}"))
            case asm_ast.DoubleInit(float=v):
                # Pack as IEEE 754 double-precision, little-endian.
                # Reinterpret the 8-byte sequence as two little-endian
                # 32-bit unsigned ints (low then high half) and emit
                # one `dc.l` per half.
                lo, hi = struct.unpack("<II", struct.pack("<d", v))
                lines.append(_instr_line("dc.l", f"${lo:08X}"))
                lines.append(_instr_line("dc.l", f"${hi:08X}"))
            case asm_ast.AddressInit(name=target, offset=off):
                # `&otherstatic` initializer — lay down 2 little-endian
                # bytes equal to the target's address. dasm resolves
                # `target` (and the optional `+off`) to the final
                # address at link time. Same as a `LongInit` cell-wise,
                # just with a symbolic value instead of a literal.
                operand = target if off == 0 else f"{target}+{off}"
                lines.append(_instr_line("dc.w", operand))
            case asm_ast.ZeroInit(bytes=n):
                # Run of `n` zero bytes — dasm's `ds.b` reserves
                # storage initialized to zero, so we don't have to
                # spell out `n` separate `dc.b $00`s.
                if n <= 0:
                    raise ValueError(
                        f"ZeroInit byte count must be positive, got {n}"
                    )
                lines.append(_instr_line("ds.b", str(n)))
            case _:
                raise TypeError(f"unexpected static_init: {item!r}")
    return lines


def _check_word(label: str, v: int) -> None:
    """Range check for a 2-byte signed/unsigned constant. Accepts
    -32768..65535 — covers both the signed range Long literals
    target and the unsigned bit pattern that comes out of casting a
    negative Long. The 16-bit emit then masks to 0xFFFF, so a
    negative value lays down as its two's-complement byte pattern."""
    if not -32768 <= v <= 65535:
        raise ValueError(
            f"{label} {v} out of range for 16-bit (-32768..65535)"
        )


def emit_top_level(tl: asm_ast.Type_top_level) -> list[str]:
    """Dispatch on the top_level alternative."""
    if isinstance(tl, asm_ast.Function):
        return emit_function(tl)
    if isinstance(tl, asm_ast.StaticVariable):
        return emit_static_variable(tl)
    raise TypeError(f"unexpected top-level node: {tl!r}")


def emit_program(prog: asm_ast.Type_program) -> str:
    match prog:
        case asm_ast.Program(top_level=top_levels):
            # One blank line separates consecutive top-level
            # entries (function bodies, static-variable definitions)
            # so they're visually distinct in the output. Trailing
            # newline at the very end (so the file ends in a
            # newline rather than a label).
            chunks = [emit_top_level(tl) for tl in top_levels]
            joined: list[str] = []
            for i, chunk in enumerate(chunks):
                if i > 0:
                    joined.append("")
                joined.extend(chunk)
            return "\n".join(joined) + "\n"
        case _:
            raise TypeError(f"unexpected program: {prog!r}")


