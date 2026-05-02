"""Pure-Python in-process assembler from `asm_ast.Program` to a memory
image.

Two-pass design:
  Pass 1 walks every top-level entry in source order, computing the byte
  size of each instruction / static-init item and recording every label
  (function name, internal `Label` node, `StaticVariable.name`) at its
  cumulative offset from `origin`.
  Pass 2 walks the same tree with the resolved symbol table, producing
  the actual byte sequence. Branch displacements (`Bxx`) are resolved
  at this point.

Pre-installed zero-page symbols: `SSP=$00/$01`, `FP=$02/$03`,
`HARGS=$04..$1B`, `DPTR=$1C/$1D` — matching the runtime header. Any
asm-level reference to one of these names resolves to the zero-page
address. Callers can pass additional `extra_symbols` (e.g. helper trap
addresses like `mul16=$E000`) before assembly so `Call(name)` /
`Jump(target)` resolve.

The assembler is a strict subset of dasm. It supports only the opcodes
and addressing modes that `tac_to_asm` / `asm_emit` actually emit:
implied / accumulator, immediate, zero-page, absolute, indirect-Y,
relative branches. JMP indirect is supported for the `IndirectCall`
trampoline (`JMP (DPTR)`), but emit doesn't use it directly today —
included so the runtime stub can.
"""

from __future__ import annotations

from dataclasses import dataclass

import asm_ast


# -------- 6502 opcode tables (just what we emit) --------

# Implied / accumulator addressing (1 byte total)
_IMPLIED = {
    "CLC": 0x18, "SEC": 0x38,
    "INX": 0xE8, "INY": 0xC8,
    "DEX": 0xCA, "DEY": 0x88,
    "PHA": 0x48, "PLA": 0x68,
    "TXA": 0x8A, "TYA": 0x98,
    "TAX": 0xAA, "TAY": 0xA8,
    "ASL_A": 0x0A, "LSR_A": 0x4A,
    "ROL_A": 0x2A, "ROR_A": 0x6A,
    "RTS": 0x60, "RTI": 0x40, "BRK": 0x00,
    "NOP": 0xEA,
}

# Immediate addressing (2 bytes: opcode + imm byte)
_IMM = {
    "LDA": 0xA9, "LDX": 0xA2, "LDY": 0xA0,
    "ADC": 0x69, "SBC": 0xE9,
    "AND": 0x29, "ORA": 0x09, "EOR": 0x49,
    "CMP": 0xC9, "CPX": 0xE0, "CPY": 0xC0,
}

# Zero-page addressing (2 bytes: opcode + zp byte)
_ZP = {
    "LDA": 0xA5, "LDX": 0xA6, "LDY": 0xA4,
    "STA": 0x85, "STX": 0x86, "STY": 0x84,
    "ADC": 0x65, "SBC": 0xE5,
    "AND": 0x25, "ORA": 0x05, "EOR": 0x45,
    "CMP": 0xC5, "CPX": 0xE4, "CPY": 0xC4,
}

# Absolute addressing (3 bytes: opcode + lo + hi)
_ABS = {
    "LDA": 0xAD, "LDX": 0xAE, "LDY": 0xAC,
    "STA": 0x8D, "STX": 0x8E, "STY": 0x8C,
    "ADC": 0x6D, "SBC": 0xED,
    "AND": 0x2D, "ORA": 0x0D, "EOR": 0x4D,
    "CMP": 0xCD, "CPX": 0xEC, "CPY": 0xCC,
    "JMP": 0x4C, "JSR": 0x20,
}

# Indirect-Y (zp),Y (2 bytes: opcode + zp byte)
_INDY = {
    "LDA": 0xB1, "STA": 0x91,
    "ADC": 0x71, "SBC": 0xF1,
    "AND": 0x31, "ORA": 0x11, "EOR": 0x51,
    "CMP": 0xD1,
}

# Indirect (3 bytes: opcode + lo + hi) — only JMP has this mode.
_IND = {"JMP": 0x6C}

# Relative branches (2 bytes: opcode + signed disp)
_BRANCH = {
    asm_ast.CC: 0x90, asm_ast.CS: 0xB0,
    asm_ast.EQ: 0xF0, asm_ast.NE: 0xD0,
    asm_ast.MI: 0x30, asm_ast.PL: 0x10,
    asm_ast.VC: 0x50, asm_ast.VS: 0x70,
}

# Reserved zero-page symbols. Match the runtime header.
DEFAULT_ZP_SYMBOLS = {
    "SSP": 0x00,    # SSP+1 = $01
    "FP": 0x02,     # FP+1  = $03
    "HARGS": 0x04,  # spans $04..$1B (24 bytes)
    "DPTR": 0x1C,   # DPTR+1 = $1D
}


# -------- assembler --------


class AssemblerError(Exception):
    """Assembly-time error: undefined symbol, branch out of range,
    operand byte overflow, etc."""


@dataclass
class AssembledProgram:
    """Result of assembling a `asm_ast.Program`.

    `image` is a 64KiB bytearray with assembled bytes laid down at
    their final addresses (everything else is 0). `symbols` maps every
    label (function names, internal labels, static variables) to its
    address; pre-installed zero-page names are also present. `code_end`
    is the address one past the last byte written by the program (so
    callers can place additional content like a runtime stub above)."""

    image: bytearray
    symbols: dict[str, int]
    origin: int
    code_end: int


def assemble(
    prog: asm_ast.Program,
    *,
    origin: int = 0x0800,
    extra_symbols: dict[str, int] | None = None,
) -> AssembledProgram:
    """Assemble `prog` into a 64KiB memory image starting at `origin`.

    `extra_symbols` is merged on top of the default zero-page names
    before assembly. Use it to pre-bind helper trap addresses (e.g.
    `mul16=$E000`) so `Call("mul16")` resolves to a `JSR $E000`."""

    symbols: dict[str, int] = dict(DEFAULT_ZP_SYMBOLS)
    if extra_symbols:
        symbols.update(extra_symbols)

    # Names whose resolved address falls in zero page. Used by Data-
    # operand size dispatch: a Data reference to a zp-resolved name
    # encodes as 2 bytes (zp addressing) instead of 3 (abs). Only
    # pre-installed names are eligible — user static variables land
    # at `origin` and above ($0800+), which is never zero-page in our
    # memory map. We freeze the set before pass 1 so size and emit
    # see the exact same answer.
    global _zp_data_names
    _zp_data_names = frozenset(
        n for n, a in symbols.items() if 0 <= a <= 0xFF
    )

    try:
        # Pass 1: collect symbols and compute total size.
        addr = origin
        for tl in prog.top_level:
            if isinstance(tl, asm_ast.Function):
                if tl.name in symbols:
                    raise AssemblerError(f"duplicate symbol {tl.name!r}")
                symbols[tl.name] = addr
                for instr in tl.instructions:
                    if isinstance(instr, asm_ast.Label):
                        if instr.name in symbols:
                            raise AssemblerError(
                                f"duplicate label {instr.name!r}"
                            )
                        symbols[instr.name] = addr
                    else:
                        addr += _instr_size(instr)
            elif isinstance(tl, asm_ast.StaticVariable):
                if tl.name in symbols:
                    raise AssemblerError(f"duplicate symbol {tl.name!r}")
                symbols[tl.name] = addr
                for item in tl.init:
                    addr += _init_size(item)
            else:
                raise TypeError(f"unexpected top-level: {tl!r}")
        code_end = addr

        # Pass 2: emit bytes.
        image = bytearray(0x10000)
        addr = origin
        for tl in prog.top_level:
            if isinstance(tl, asm_ast.Function):
                for instr in tl.instructions:
                    if isinstance(instr, asm_ast.Label):
                        continue
                    bs = _emit_instr(instr, addr, symbols)
                    expected = _instr_size(instr)
                    if len(bs) != expected:
                        raise AssemblerError(
                            f"size mismatch for {instr!r}: "
                            f"pass1={expected}, pass2={len(bs)}"
                        )
                    image[addr:addr + len(bs)] = bs
                    addr += len(bs)
            elif isinstance(tl, asm_ast.StaticVariable):
                for item in tl.init:
                    bs = _emit_init(item, symbols)
                    if len(bs) != _init_size(item):
                        raise AssemblerError(
                            f"size mismatch for static_init {item!r}"
                        )
                    image[addr:addr + len(bs)] = bs
                    addr += len(bs)
        assert addr == code_end
    finally:
        _zp_data_names = frozenset()

    return AssembledProgram(
        image=image, symbols=symbols, origin=origin, code_end=code_end,
    )


# Set by `assemble()` for the duration of one pass-pair. A Data operand
# whose name is in here resolves to a zero-page address and so encodes
# in zp mode (2 bytes); otherwise it encodes in abs mode (3 bytes).
_zp_data_names: frozenset[str] = frozenset()


# -------- public size API --------


# Default zero-page Data names for `instruction_size` — the runtime
# header's reserved names (HARGS, SSP, FP, DPTR). Their resolved
# addresses are all < 0x100, so a Data reference to one of them
# encodes in zp mode.
_DEFAULT_ZP_NAMES = frozenset(DEFAULT_ZP_SYMBOLS)


def instruction_size(
    instr: asm_ast.Type_instruction,
    *,
    zp_names: frozenset[str] | None = None,
) -> int:
    """Byte size for `instr` under the same encoding rules `assemble`
    uses. `zp_names` is the set of Data-operand names that resolve to
    zero-page addresses; if omitted, defaults to the runtime header
    names (`HARGS`, `SSP`, `FP`, `DPTR`).

    Public wrapper around the internal `_instr_size`, which threads
    its zp-set through a module-level binding because that's how
    pass 1 / pass 2 of `assemble` share state."""
    global _zp_data_names
    saved = _zp_data_names
    _zp_data_names = _DEFAULT_ZP_NAMES if zp_names is None else zp_names
    try:
        return _instr_size(instr)
    finally:
        _zp_data_names = saved


# -------- size helpers --------


def _instr_size(instr: asm_ast.Type_instruction) -> int:
    """Pass-1 size of an instruction. Must match the byte length
    returned by `_emit_instr` exactly, or pass 1 and pass 2 will
    disagree about label addresses."""
    match instr:
        case asm_ast.Mov():
            return _mov_size(instr.src, instr.dst)
        case asm_ast.Add(src=src) | asm_ast.Sub(src=src):
            return _accum_arith_size(src)
        case asm_ast.And(src=src) | asm_ast.Or(src=src):
            return _accum_arith_size(src)
        case asm_ast.Xor(src1=s1, src2=s2):
            other = s2 if _is_reg_a(s1) else s1
            return _accum_arith_size(other)
        case asm_ast.Compare(left=left, right=right):
            return _compare_size(left, right)
        case asm_ast.ClearCarry() | asm_ast.SetCarry():
            return 1
        case asm_ast.Inc() | asm_ast.Dec():
            return 1
        case asm_ast.Push() | asm_ast.Pop():
            return 1
        case (asm_ast.ArithmeticShiftLeft() | asm_ast.LogicalShiftRight()
              | asm_ast.RotateLeft() | asm_ast.RotateRight()):
            return 1
        case asm_ast.Call():
            return 3   # JSR abs
        case asm_ast.Jump():
            return 3   # JMP abs
        case asm_ast.Branch():
            return 2   # Bxx rel
        case asm_ast.Label():
            return 0
        case asm_ast.AllocateStack(bytes=n):
            return _ssp_sub_size(n)
        case asm_ast.FunctionPrologue(arg_bytes=ab, local_bytes=lb):
            return _prologue_size(ab, lb)
        case asm_ast.Ret(arg_bytes=ab, local_bytes=lb, save_a=sa):
            return _ret_size(ab, lb, sa)
        case asm_ast.LoadAddress(src=src, dst=dst):
            return _load_address_size(src, dst)
        case _:
            raise TypeError(f"unexpected instruction: {instr!r}")


def _emit_init(
    item: asm_ast.Type_static_init, syms: dict[str, int],
) -> bytes:
    """Lay down the bytes of a static_init item in source byte order
    (little-endian for the multi-byte numeric variants)."""
    match item:
        case asm_ast.IntInit(value=v):
            if not -128 <= v <= 0xFF:
                raise AssemblerError(f"IntInit {v} out of range")
            return bytes([v & 0xFF])
        case asm_ast.LongInit(value=v):
            v &= 0xFFFF
            return bytes([v & 0xFF, (v >> 8) & 0xFF])
        case asm_ast.LongLongInit(value=v) | asm_ast.FloatInit(bits=v):
            v &= 0xFFFFFFFF
            return bytes([(v >> (i * 8)) & 0xFF for i in range(4)])
        case asm_ast.DoubleInit(bits=v):
            v &= 0xFFFFFFFFFFFFFFFF
            return bytes([(v >> (i * 8)) & 0xFF for i in range(8)])
        case asm_ast.AddressInit(name=name, offset=off):
            if name not in syms:
                raise AssemblerError(f"undefined symbol {name!r}")
            addr = (syms[name] + off) & 0xFFFF
            return bytes([addr & 0xFF, (addr >> 8) & 0xFF])
        case asm_ast.StringInit(str=s, bytes=n):
            if n < len(s):
                raise AssemblerError(
                    f"StringInit bytes={n} < len(str)={len(s)}"
                )
            return bytes([ord(c) & 0xFF for c in s]) + bytes(n - len(s))
        case asm_ast.ZeroInit(bytes=n):
            if n <= 0:
                raise AssemblerError(f"ZeroInit byte count {n} not positive")
            return bytes(n)
        case _:
            raise TypeError(f"unexpected static_init: {item!r}")


def _init_size(item: asm_ast.Type_static_init) -> int:
    match item:
        case asm_ast.IntInit():
            return 1
        case asm_ast.LongInit():
            return 2
        case asm_ast.LongLongInit() | asm_ast.FloatInit():
            return 4
        case asm_ast.DoubleInit():
            return 8
        case asm_ast.AddressInit():
            return 2
        case asm_ast.StringInit(bytes=n) | asm_ast.ZeroInit(bytes=n):
            return n
        case _:
            raise TypeError(f"unexpected static_init: {item!r}")


# -------- per-instruction byte emission --------


def _emit_instr(
    instr: asm_ast.Type_instruction, addr: int, syms: dict[str, int],
) -> bytes:
    """Emit the bytes for a single instruction at address `addr`. The
    address only matters for relative branches — every other form is
    position-independent at this layer (absolute addressing is resolved
    at link-time symbol lookup, not at the branch-displacement step)."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            return _emit_mov(src, dst, syms)
        case asm_ast.Add(src=src, dst=dst):
            _check_dst_a(dst, "Add")
            return _emit_accum_arith("ADC", src, syms)
        case asm_ast.Sub(src=src, dst=dst):
            _check_dst_a(dst, "Sub")
            return _emit_accum_arith("SBC", src, syms)
        case asm_ast.And(src=src, dst=dst):
            _check_dst_a(dst, "And")
            return _emit_accum_arith("AND", src, syms)
        case asm_ast.Or(src=src, dst=dst):
            _check_dst_a(dst, "Or")
            return _emit_accum_arith("ORA", src, syms)
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            _check_dst_a(dst, "Xor")
            if _is_reg_a(s1):
                other = s2
            elif _is_reg_a(s2):
                other = s1
            else:
                raise AssemblerError(
                    f"Xor srcs must include Reg(A); got {s1!r}, {s2!r}"
                )
            return _emit_accum_arith("EOR", other, syms)
        case asm_ast.Compare(left=left, right=right):
            return _emit_compare(left, right, syms)
        case asm_ast.ClearCarry():
            return bytes([_IMPLIED["CLC"]])
        case asm_ast.SetCarry():
            return bytes([_IMPLIED["SEC"]])
        case asm_ast.Inc(dst=dst):
            return _emit_inc_dec(dst, "Inc")
        case asm_ast.Dec(dst=dst):
            return _emit_inc_dec(dst, "Dec")
        case asm_ast.Push(src=src):
            if not _is_reg_a(src):
                raise AssemblerError(f"Push src must be Reg(A); got {src!r}")
            return bytes([_IMPLIED["PHA"]])
        case asm_ast.Pop(dst=dst):
            if not _is_reg_a(dst):
                raise AssemblerError(f"Pop dst must be Reg(A); got {dst!r}")
            return bytes([_IMPLIED["PLA"]])
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            _check_dst_a(dst, "ASL")
            return bytes([_IMPLIED["ASL_A"]])
        case asm_ast.LogicalShiftRight(dst=dst):
            _check_dst_a(dst, "LSR")
            return bytes([_IMPLIED["LSR_A"]])
        case asm_ast.RotateLeft(dst=dst):
            _check_dst_a(dst, "ROL")
            return bytes([_IMPLIED["ROL_A"]])
        case asm_ast.RotateRight(dst=dst):
            _check_dst_a(dst, "ROR")
            return bytes([_IMPLIED["ROR_A"]])
        case asm_ast.Call(name=name):
            return _emit_jsr(name, syms)
        case asm_ast.Jump(target=target):
            return _emit_jmp(target, syms)
        case asm_ast.Branch(cond=cond, target=target):
            return _emit_branch(cond, target, addr, syms)
        case asm_ast.AllocateStack(bytes=n):
            return _emit_ssp_sub(n)
        case asm_ast.FunctionPrologue(arg_bytes=ab, local_bytes=lb):
            return _emit_prologue(ab, lb)
        case asm_ast.Ret(arg_bytes=ab, local_bytes=lb, save_a=sa):
            return _emit_ret(ab, lb, sa)
        case asm_ast.LoadAddress(src=src, dst=dst):
            return _emit_load_address(src, dst, syms)
        case _:
            raise TypeError(f"unexpected instruction: {instr!r}")


# -------- operand classification --------


def _is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_reg_x(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.X)


def _is_reg_y(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.Y)


def _is_indirect_y(op: asm_ast.Type_operand) -> bool:
    """True for operands lowered to 6502 (zp),Y addressing — Stack
    (via SSP), Frame (via FP), or Indirect (via DPTR)."""
    return isinstance(op, (asm_ast.Stack, asm_ast.Frame, asm_ast.Indirect))


def _is_memory(op: asm_ast.Type_operand) -> bool:
    return isinstance(
        op, (asm_ast.Stack, asm_ast.Frame, asm_ast.Data, asm_ast.Indirect)
    )


def _is_imm_label(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, (asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh))


def _check_dst_a(op: asm_ast.Type_operand, name: str) -> None:
    if not _is_reg_a(op):
        raise AssemblerError(f"{name} dst must be Reg(A); got {op!r}")


def _reject_pseudo(op: asm_ast.Type_operand) -> None:
    if isinstance(op, asm_ast.Pseudo):
        raise AssemblerError(
            f"Pseudo({op.name!r}) reached assembler; "
            "replace_pseudoregisters must run first"
        )


# -------- operand resolution --------


def _imm_byte(v: int, label: str = "immediate") -> int:
    if not 0 <= v <= 0xFF:
        raise AssemblerError(f"{label} {v} out of range 0..255")
    return v & 0xFF


def _abs_word(v: int, label: str = "address") -> tuple[int, int]:
    if not 0 <= v <= 0xFFFF:
        raise AssemblerError(f"{label} {v} out of range 0..65535")
    return (v & 0xFF, (v >> 8) & 0xFF)


def _resolve_data_addr(op: asm_ast.Data, syms: dict[str, int]) -> int:
    if op.name not in syms:
        raise AssemblerError(f"undefined symbol {op.name!r}")
    return (syms[op.name] + op.offset) & 0xFFFF


def _resolve_imm_label(
    op: asm_ast.Type_operand, syms: dict[str, int],
) -> int:
    """Resolve ImmLabelLow/High to the immediate byte value."""
    if op.name not in syms:
        raise AssemblerError(f"undefined symbol {op.name!r}")
    addr = (syms[op.name] + op.offset) & 0xFFFF
    if isinstance(op, asm_ast.ImmLabelLow):
        return addr & 0xFF
    if isinstance(op, asm_ast.ImmLabelHigh):
        return (addr >> 8) & 0xFF
    raise TypeError(f"not an ImmLabel operand: {op!r}")


def _indirect_zp(op: asm_ast.Type_operand) -> int:
    """Zero-page byte for an indirect-Y operand. Stack uses SSP, Frame
    uses FP, Indirect uses DPTR."""
    if isinstance(op, asm_ast.Stack):
        return DEFAULT_ZP_SYMBOLS["SSP"]
    if isinstance(op, asm_ast.Frame):
        return DEFAULT_ZP_SYMBOLS["FP"]
    if isinstance(op, asm_ast.Indirect):
        return DEFAULT_ZP_SYMBOLS["DPTR"]
    raise TypeError(f"not an indirect operand: {op!r}")


# -------- emit helpers --------


def _emit_ldy_imm(off: int) -> bytes:
    return bytes([_IMM["LDY"], _imm_byte(off, "frame offset")])


def _emit_lda_imm(v: int) -> bytes:
    return bytes([_IMM["LDA"], _imm_byte(v)])


def _emit_indy(opcode: str, op: asm_ast.Type_operand) -> bytes:
    """LDY #off; <op> (zp),Y — for Stack/Frame/Indirect operands."""
    return _emit_ldy_imm(op.offset) + bytes(
        [_INDY[opcode], _indirect_zp(op)]
    )


def _emit_abs(opcode: str, addr: int) -> bytes:
    lo, hi = _abs_word(addr)
    return bytes([_ABS[opcode], lo, hi])


def _emit_zp(opcode: str, zp: int) -> bytes:
    return bytes([_ZP[opcode], _imm_byte(zp, "zp address")])


def _emit_zp_or_abs(opcode: str, addr: int) -> bytes:
    """Use zero-page addressing when the address fits in a single byte
    (and the opcode supports it), otherwise absolute. Saves a byte per
    SSP/FP/HARGS reference and matches the assembler conventions
    `asm_emit` relies on (`LDA SSP` collapses to zp mode in dasm)."""
    if 0 <= addr <= 0xFF and opcode in _ZP:
        return _emit_zp(opcode, addr)
    return _emit_abs(opcode, addr)


def _emit_load_zp_or_abs_to_a(addr: int) -> bytes:
    return _emit_zp_or_abs("LDA", addr)


def _emit_store_a_zp_or_abs(addr: int) -> bytes:
    return _emit_zp_or_abs("STA", addr)


# -------- Mov --------


def _mov_size(src: asm_ast.Type_operand, dst: asm_ast.Type_operand) -> int:
    _reject_pseudo(src)
    _reject_pseudo(dst)
    # Reg<->Reg transfers: 1 byte.
    if isinstance(src, asm_ast.Reg) and isinstance(dst, asm_ast.Reg):
        return 1
    # Imm/ImmLabel -> Reg: 2 bytes (LD# imm).
    if (isinstance(src, asm_ast.Imm) or _is_imm_label(src)) and isinstance(
        dst, asm_ast.Reg
    ):
        return 2
    # Imm/ImmLabel -> memory: LDA # (2) + store.
    if isinstance(src, asm_ast.Imm) or _is_imm_label(src):
        return 2 + _store_a_to_mem_size(dst)
    # memory -> Reg(A): load.
    if _is_memory(src) and _is_reg_a(dst):
        return _load_mem_to_a_size(src)
    # Reg(A) -> memory: store.
    if _is_reg_a(src) and _is_memory(dst):
        return _store_a_to_mem_size(dst)
    # memory -> memory: load + store.
    if _is_memory(src) and _is_memory(dst):
        return _load_mem_to_a_size(src) + _store_a_to_mem_size(dst)
    raise AssemblerError(f"unsupported Mov: {src!r} -> {dst!r}")


def _load_mem_to_a_size(src: asm_ast.Type_operand) -> int:
    """LDA from a memory operand into A."""
    if isinstance(src, asm_ast.Data):
        # Absolute or zero-page (dasm picks zp when the linker resolves
        # to < 0x100 — for static-storage Data this is unreachable since
        # statics live above $0800, but the same predicate is used at
        # emit time).
        return 2 if _data_fits_zp(src) else 3
    if _is_indirect_y(src):
        return 4   # LDY # + LDA (zp),Y
    raise AssemblerError(f"can't load {src!r} into A")


def _store_a_to_mem_size(dst: asm_ast.Type_operand) -> int:
    """STA to a memory operand from A."""
    if isinstance(dst, asm_ast.Data):
        return 2 if _data_fits_zp(dst) else 3
    if _is_indirect_y(dst):
        return 4
    raise AssemblerError(f"can't store A into {dst!r}")


def _data_fits_zp(d: asm_ast.Data) -> bool:
    """True if the resolved address of `d` is in zero page. Today the
    only Data references that can fit zp are those whose name is a
    pre-installed runtime symbol (HARGS, in particular — `tac_to_asm`
    emits `Data("HARGS", k)` for the helper-call marshaling). User
    static variables live at `origin` ($0800) and above, so they're
    never zp."""
    if d.name not in _zp_data_names:
        return False
    # Offset can push us out of zero page even if the base is in.
    # `_zp_data_names` only carries the base address category; we
    # don't track ranges per name. Be conservative: only allow
    # offset 0..(0xFF - base) — but since we don't have the base
    # value here, fall back to a simple offset cap. The pre-installed
    # zp symbols today (HARGS=$04, span 24 bytes; SSP, FP, DPTR — 2
    # bytes each) all fit zp comfortably for any offset `tac_to_asm`
    # emits, so an offset bound of 0..0xFB is safe.
    return 0 <= d.offset <= 0xFB


def _emit_mov(
    src: asm_ast.Type_operand, dst: asm_ast.Type_operand,
    syms: dict[str, int],
) -> bytes:
    _reject_pseudo(src)
    _reject_pseudo(dst)
    # Reg-to-reg transfers.
    if isinstance(src, asm_ast.Reg) and isinstance(dst, asm_ast.Reg):
        sr, dr = src.reg, dst.reg
        if isinstance(sr, asm_ast.X) and isinstance(dr, asm_ast.A):
            return bytes([_IMPLIED["TXA"]])
        if isinstance(sr, asm_ast.Y) and isinstance(dr, asm_ast.A):
            return bytes([_IMPLIED["TYA"]])
        if isinstance(sr, asm_ast.A) and isinstance(dr, asm_ast.X):
            return bytes([_IMPLIED["TAX"]])
        if isinstance(sr, asm_ast.A) and isinstance(dr, asm_ast.Y):
            return bytes([_IMPLIED["TAY"]])
        raise AssemblerError(f"unsupported Mov reg->reg: {src!r} -> {dst!r}")
    # Imm -> Reg.
    if isinstance(src, asm_ast.Imm) and isinstance(dst, asm_ast.Reg):
        v = _imm_byte(src.value)
        if isinstance(dst.reg, asm_ast.A):
            return bytes([_IMM["LDA"], v])
        if isinstance(dst.reg, asm_ast.X):
            return bytes([_IMM["LDX"], v])
        if isinstance(dst.reg, asm_ast.Y):
            return bytes([_IMM["LDY"], v])
    # ImmLabel -> Reg(A) — only A is needed by LoadAddress.
    if _is_imm_label(src) and _is_reg_a(dst):
        return bytes([_IMM["LDA"], _resolve_imm_label(src, syms)])
    # Imm -> memory.
    if isinstance(src, asm_ast.Imm) and _is_memory(dst):
        return _emit_lda_imm(src.value) + _emit_store_a_to_mem(dst, syms)
    # ImmLabel -> memory.
    if _is_imm_label(src) and _is_memory(dst):
        return bytes([_IMM["LDA"], _resolve_imm_label(src, syms)]) + (
            _emit_store_a_to_mem(dst, syms)
        )
    # memory -> Reg(A).
    if _is_memory(src) and _is_reg_a(dst):
        return _emit_load_mem_to_a(src, syms)
    # Reg(A) -> memory.
    if _is_reg_a(src) and _is_memory(dst):
        return _emit_store_a_to_mem(dst, syms)
    # memory -> memory.
    if _is_memory(src) and _is_memory(dst):
        return _emit_load_mem_to_a(src, syms) + _emit_store_a_to_mem(dst, syms)
    raise AssemblerError(f"unsupported Mov: {src!r} -> {dst!r}")


def _emit_load_mem_to_a(
    op: asm_ast.Type_operand, syms: dict[str, int],
) -> bytes:
    if isinstance(op, asm_ast.Data):
        return _emit_load_zp_or_abs_to_a(_resolve_data_addr(op, syms))
    if _is_indirect_y(op):
        return _emit_indy("LDA", op)
    raise AssemblerError(f"can't load {op!r} into A")


def _emit_store_a_to_mem(
    op: asm_ast.Type_operand, syms: dict[str, int],
) -> bytes:
    if isinstance(op, asm_ast.Data):
        return _emit_store_a_zp_or_abs(_resolve_data_addr(op, syms))
    if _is_indirect_y(op):
        return _emit_indy("STA", op)
    raise AssemblerError(f"can't store A into {op!r}")


# -------- arithmetic / logic / compare --------


def _accum_arith_size(src: asm_ast.Type_operand) -> int:
    """ADC/SBC/AND/ORA/EOR (all use Reg(A) as implicit dst)."""
    if isinstance(src, asm_ast.Imm):
        return 2
    if isinstance(src, asm_ast.Data):
        return 2 if _data_fits_zp(src) else 3
    if _is_indirect_y(src):
        return 4
    raise AssemblerError(f"unsupported accum-arith src: {src!r}")


def _emit_accum_arith(
    opcode: str, src: asm_ast.Type_operand, syms: dict[str, int],
) -> bytes:
    _reject_pseudo(src)
    if isinstance(src, asm_ast.Imm):
        return bytes([_IMM[opcode], _imm_byte(src.value)])
    if isinstance(src, asm_ast.Data):
        return _emit_zp_or_abs(opcode, _resolve_data_addr(src, syms))
    if _is_indirect_y(src):
        return _emit_indy(opcode, src)
    raise AssemblerError(f"unsupported {opcode} src: {src!r}")


def _compare_size(
    left: asm_ast.Type_operand, right: asm_ast.Type_operand,
) -> int:
    if not isinstance(left, asm_ast.Reg):
        raise AssemblerError(f"Compare left must be a register; got {left!r}")
    if isinstance(right, asm_ast.Imm):
        return 2
    if isinstance(right, asm_ast.Data):
        return 2 if _data_fits_zp(right) else 3
    if _is_indirect_y(right):
        if not isinstance(left.reg, asm_ast.A):
            raise AssemblerError(
                f"Compare with left={left!r} requires Imm/Data right "
                "(no indirect-Y for CPX/CPY)"
            )
        return 4
    raise AssemblerError(f"unsupported Compare right: {right!r}")


def _emit_compare(
    left: asm_ast.Type_operand, right: asm_ast.Type_operand,
    syms: dict[str, int],
) -> bytes:
    _reject_pseudo(left)
    _reject_pseudo(right)
    if not isinstance(left, asm_ast.Reg):
        raise AssemblerError(f"Compare left must be a register; got {left!r}")
    if isinstance(left.reg, asm_ast.A):
        opcode = "CMP"
    elif isinstance(left.reg, asm_ast.X):
        opcode = "CPX"
    elif isinstance(left.reg, asm_ast.Y):
        opcode = "CPY"
    else:
        raise TypeError(f"unexpected reg: {left.reg!r}")
    if isinstance(right, asm_ast.Imm):
        return bytes([_IMM[opcode], _imm_byte(right.value)])
    if isinstance(right, asm_ast.Data):
        return _emit_zp_or_abs(opcode, _resolve_data_addr(right, syms))
    if _is_indirect_y(right):
        if opcode != "CMP":
            raise AssemblerError(
                f"Compare with left={left!r} requires Imm/Data right; "
                f"got {right!r}"
            )
        return _emit_indy(opcode, right)
    raise AssemblerError(f"unsupported Compare right: {right!r}")


# -------- inc/dec --------


def _emit_inc_dec(op: asm_ast.Type_operand, name: str) -> bytes:
    if _is_reg_x(op):
        return bytes([_IMPLIED["INX" if name == "Inc" else "DEX"]])
    if _is_reg_y(op):
        return bytes([_IMPLIED["INY" if name == "Inc" else "DEY"]])
    raise AssemblerError(f"{name} dst must be Reg(X) or Reg(Y); got {op!r}")


# -------- jumps and branches --------


def _emit_jsr(name: str, syms: dict[str, int]) -> bytes:
    if name not in syms:
        raise AssemblerError(f"undefined symbol {name!r}")
    lo, hi = _abs_word(syms[name])
    return bytes([_ABS["JSR"], lo, hi])


def _emit_jmp(target: str, syms: dict[str, int]) -> bytes:
    if target not in syms:
        raise AssemblerError(f"undefined symbol {target!r}")
    lo, hi = _abs_word(syms[target])
    return bytes([_ABS["JMP"], lo, hi])


def _emit_branch(
    cond: asm_ast.Type_condition,
    target: str, addr: int, syms: dict[str, int],
) -> bytes:
    if target not in syms:
        raise AssemblerError(f"undefined branch target {target!r}")
    op = _BRANCH[type(cond)]
    # 6502 branches: PC has already advanced past the 2-byte branch
    # when the displacement is computed, so disp = target - (addr + 2).
    disp = syms[target] - (addr + 2)
    if not -128 <= disp <= 127:
        raise AssemblerError(
            f"branch to {target!r} out of range: disp={disp}"
        )
    return bytes([op, disp & 0xFF])


# -------- compound nodes (size-dependent on args) --------


def _ssp_sub_size(amt: int) -> int:
    """`SSP -= amt`: SEC + LDA SSP + SBC #lo + STA SSP + LDA SSP+1 +
    SBC #hi + STA SSP+1. SSP and SSP+1 are zp, so 2 bytes each. Empty
    if amt == 0."""
    if amt == 0:
        return 0
    return 1 + 2 + 2 + 2 + 2 + 2 + 2  # SEC + LDA + SBC + STA + LDA + SBC + STA


def _emit_ssp_sub(amt: int) -> bytes:
    if not 0 <= amt <= 0xFFFF:
        raise AssemblerError(f"AllocateStack amt {amt} out of range")
    if amt == 0:
        return b""
    lo, hi = amt & 0xFF, (amt >> 8) & 0xFF
    ssp = DEFAULT_ZP_SYMBOLS["SSP"]
    return (
        bytes([_IMPLIED["SEC"]])
        + _emit_zp("LDA", ssp)
        + bytes([_IMM["SBC"], lo])
        + _emit_zp("STA", ssp)
        + _emit_zp("LDA", ssp + 1)
        + bytes([_IMM["SBC"], hi])
        + _emit_zp("STA", ssp + 1)
    )


# -------- prologue / epilogue --------


def _prologue_size(arg_bytes: int, local_bytes: int) -> int:
    if arg_bytes + local_bytes == 0:
        return 0
    # Allocate locals + 2-byte saved-FP slot.
    size = _ssp_sub_size(local_bytes + 2)
    # Save caller's FP into the slot at SSP+M+1 / SSP+M+2.
    # LDY #M+1; LDA FP; STA (SSP),Y; INY; LDA FP+1; STA (SSP),Y
    size += 2 + 2 + 2 + 1 + 2 + 2
    # Set FP = SSP: LDA SSP; STA FP; LDA SSP+1; STA FP+1
    size += 2 + 2 + 2 + 2
    return size


def _emit_prologue(arg_bytes: int, local_bytes: int) -> bytes:
    if arg_bytes + local_bytes == 0:
        return b""
    if not 0 <= local_bytes <= 253:
        raise AssemblerError(f"local_bytes {local_bytes} out of range 0..253")
    ssp = DEFAULT_ZP_SYMBOLS["SSP"]
    fp = DEFAULT_ZP_SYMBOLS["FP"]
    out = bytearray()
    out += _emit_ssp_sub(local_bytes + 2)
    # LDY #M+1; LDA FP; STA (SSP),Y
    out += bytes([_IMM["LDY"], local_bytes + 1])
    out += _emit_zp("LDA", fp)
    out += bytes([_INDY["STA"], ssp])
    # INY
    out += bytes([_IMPLIED["INY"]])
    # LDA FP+1; STA (SSP),Y
    out += _emit_zp("LDA", fp + 1)
    out += bytes([_INDY["STA"], ssp])
    # FP = SSP
    out += _emit_zp("LDA", ssp)
    out += _emit_zp("STA", fp)
    out += _emit_zp("LDA", ssp + 1)
    out += _emit_zp("STA", fp + 1)
    return bytes(out)


def _ret_size(arg_bytes: int, local_bytes: int, save_a: bool) -> int:
    if arg_bytes + local_bytes == 0:
        return 1   # RTS
    # SSP = FP + (N+M+2): CLC + LDA FP + ADC #lo + STA SSP + LDA FP+1
    #                     + ADC #hi + STA SSP+1 (or +0 form when amt=0)
    rewind = arg_bytes + local_bytes + 2
    if rewind == 0:
        size = 2 + 2 + 2 + 2  # LDA + STA + LDA + STA
    else:
        size = 1 + 2 + 2 + 2 + 2 + 2 + 2
    # Restore FP from slot at FP+M+1 / FP+M+2 via X scratch:
    # LDY #M+1; LDA (FP),Y; TAX; INY; LDA (FP),Y; STA FP+1; STX FP.
    # X is free to clobber because no return convention puts data
    # there (1B → A, 2B/4B/8B → HARGS).
    size += 2 + 2 + 1 + 1 + 2 + 2 + 2
    # PHA + PLA wrap if save_a; trailing RTS.
    if save_a:
        size += 1 + 1
    size += 1   # RTS
    return size


def _emit_ret(arg_bytes: int, local_bytes: int, save_a: bool) -> bytes:
    if arg_bytes + local_bytes == 0:
        return bytes([_IMPLIED["RTS"]])
    if not 0 <= local_bytes <= 253:
        raise AssemblerError(f"local_bytes {local_bytes} out of range 0..253")
    ssp = DEFAULT_ZP_SYMBOLS["SSP"]
    fp = DEFAULT_ZP_SYMBOLS["FP"]
    rewind = arg_bytes + local_bytes + 2

    out = bytearray()
    if save_a:
        out += bytes([_IMPLIED["PHA"]])
    # SSP = FP + rewind.
    if rewind == 0:
        out += _emit_zp("LDA", fp)
        out += _emit_zp("STA", ssp)
        out += _emit_zp("LDA", fp + 1)
        out += _emit_zp("STA", ssp + 1)
    else:
        lo, hi = rewind & 0xFF, (rewind >> 8) & 0xFF
        out += bytes([_IMPLIED["CLC"]])
        out += _emit_zp("LDA", fp)
        out += bytes([_IMM["ADC"], lo])
        out += _emit_zp("STA", ssp)
        out += _emit_zp("LDA", fp + 1)
        out += bytes([_IMM["ADC"], hi])
        out += _emit_zp("STA", ssp + 1)
    # Restore FP from slot. TAX/STX scratch is fine — no return
    # convention uses X.
    out += bytes([_IMM["LDY"], local_bytes + 1])
    out += bytes([_INDY["LDA"], fp])
    out += bytes([_IMPLIED["TAX"]])
    out += bytes([_IMPLIED["INY"]])
    out += bytes([_INDY["LDA"], fp])
    out += _emit_zp("STA", fp + 1)
    out += _emit_zp("STX", fp)
    if save_a:
        out += bytes([_IMPLIED["PLA"]])
    out += bytes([_IMPLIED["RTS"]])
    return bytes(out)


# -------- LoadAddress --------


def _load_address_size(
    src: asm_ast.Type_operand, dst: asm_ast.Type_operand,
) -> int:
    """Two cases: Data (immediate-resolved address; LDA #imm + store,
    twice) or Frame (16-bit add: CLC + LDA FP + ADC + STA + LDA FP+1 +
    ADC #0 + STA). `dst` is always a 2-byte memory operand; both low
    and high stores contribute one `_store_a_to_mem_size` each (offset
    doesn't change the encoding length)."""
    store_size = _store_a_to_mem_size(dst)
    if isinstance(src, asm_ast.Data):
        # LDA #lo + STA dst.lo + LDA #hi + STA dst.hi
        return 2 + store_size + 2 + store_size
    if isinstance(src, asm_ast.Frame):
        # CLC + LDA FP + ADC # + STA dst.lo + LDA FP+1 + ADC #0 + STA dst.hi
        return 1 + 2 + 2 + store_size + 2 + 2 + store_size
    raise AssemblerError(
        f"LoadAddress src must be Data or Frame; got {src!r}"
    )


def _emit_load_address(
    src: asm_ast.Type_operand, dst: asm_ast.Type_operand,
    syms: dict[str, int],
) -> bytes:
    _reject_pseudo(src)
    _reject_pseudo(dst)
    if not _is_memory(dst):
        raise AssemblerError(f"LoadAddress dst must be memory; got {dst!r}")
    fp = DEFAULT_ZP_SYMBOLS["FP"]
    if isinstance(src, asm_ast.Data):
        addr = _resolve_data_addr(src, syms)
        lo = addr & 0xFF
        hi = (addr >> 8) & 0xFF
        out = bytearray()
        out += bytes([_IMM["LDA"], lo])
        out += _emit_store_a_to_mem(dst, syms)
        out += bytes([_IMM["LDA"], hi])
        out += _emit_store_a_to_mem(_shift_offset(dst, 1), syms)
        return bytes(out)
    if isinstance(src, asm_ast.Frame):
        if not 0 <= src.offset <= 0xFF:
            raise AssemblerError(f"LoadAddress Frame offset {src.offset} oor")
        out = bytearray()
        out += bytes([_IMPLIED["CLC"]])
        out += _emit_zp("LDA", fp)
        out += bytes([_IMM["ADC"], src.offset])
        out += _emit_store_a_to_mem(dst, syms)
        out += _emit_zp("LDA", fp + 1)
        out += bytes([_IMM["ADC"], 0])
        out += _emit_store_a_to_mem(_shift_offset(dst, 1), syms)
        return bytes(out)
    raise AssemblerError(f"LoadAddress src must be Data or Frame; got {src!r}")


def _shift_offset(
    op: asm_ast.Type_operand, k: int,
) -> asm_ast.Type_operand:
    if isinstance(op, asm_ast.Frame):
        return asm_ast.Frame(offset=op.offset + k)
    if isinstance(op, asm_ast.Stack):
        return asm_ast.Stack(offset=op.offset + k)
    if isinstance(op, asm_ast.Data):
        return asm_ast.Data(name=op.name, offset=op.offset + k)
    if isinstance(op, asm_ast.Indirect):
        return asm_ast.Indirect(offset=op.offset + k)
    raise TypeError(f"can't shift offset on operand {op!r}")


