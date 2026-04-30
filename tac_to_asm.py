"""Translate a tac_ast tree into an asm_ast tree.

The asm IR is strictly 1:1 with 6502 opcodes — every node maps to
exactly one instruction (with the documented exceptions of `Ret`,
`FunctionPrologue`, and `AllocateStack`). The 6502 is an 8-bit
machine, so the IR has no width tagging: every operand is one byte.

`tac_to_asm` is therefore the home of all multi-byte lowering. For
each TAC instruction whose operands are wider than 1 byte (Long /
ULong = 2, LongLong / ULongLong = 4, Float = 4, Double = 8 — per
the symbol table), the translator emits a sequence of byte-level
asm atoms — typically one pass per byte with the 6502's carry flag
threading naturally between them for arithmetic. Multi-byte
operands are addressed via the `offset` field on `Pseudo` / `Stack`
/ `Frame` / `Data`: `Pseudo(name, offset=0)` is the low byte of
`name`, `Pseudo(name, offset=k)` the (k+1)-th byte; `Imm`
constants split into their bytes via shift-and-mask.

The Translator class holds a label counter so inline lowerings
(comparisons, `!`, sign-extension) get unique labels across the
whole program. Module-level `translate_*` functions construct a
fresh Translator per call; use the class directly when you want
the counter to persist across calls.

Mapping highlights (full per-op detail in `translate_binary` /
`translate_instruction`):
  Program(top_level)        -> Program(top_level)
  Function(name, …)         -> Function(name, …, flat-mapped instrs)
  StaticVariable(name, …)   -> StaticVariable(name, …, init-rewrapped)
  Ret(val)                  -> stage `val` in A (and X for the high
                               byte of a Long return), or in HARGS
                               for wider returns; then Ret.
  Copy(src, dst)            -> N× Mov per byte for an N-byte type.
  SignExtend(src, dst)      -> Copy each source byte to the matching
                               dst byte; the last LDA's N flag is the
                               sign byte's. BMI dispatches a write of
                               $00 / $FF into each remaining (high)
                               dst byte.
  ZeroExtend(src, dst)      -> Copy each source byte to the matching
                               dst byte; LDA #$00; STA into each
                               remaining (high) dst byte.
  Truncate(src, dst)        -> Copy `_size_of(dst)` low bytes of src
                               into dst. Memory is little-endian so
                               offset 0 is the low byte; the source's
                               high bytes are just discarded.
  Unary(op, src, dst)       -> Mov src→A, atomic op on A, Mov A→dst
                               (Int). For multi-byte operands the
                               negate / complement / logical-not
                               lowerings fan out per byte (see
                               translate_unop).
  Binary(Add, …)            -> Int: Mov src1→A; CLC; Add(src2, A);
                               Mov A→dst. Multi-byte: same pattern,
                               once per byte, with no CLC after the
                               low byte — the carry from each ADC
                               threads into the next. (LDA only
                               affects N/Z, not C.)
  Binary(Subtract, …)       -> Same shape with SetCarry/Sub. The borrow
                               (in 6502 terms, an inverted-carry) also
                               threads through each successive SBC.
  Binary(BitwiseAnd/Or/Xor) -> Byte op on each pair of bytes; no carry
                               threading needed (these don't touch C).
  Binary(Equal/NotEqual)    -> Int: Compare + Branch + 0/1 select.
                               Multi-byte: walk bytes high→low, BNE
                               short-circuit on each except the
                               lowest; the final low-byte CMP's Z is
                               the answer. 0/1 select after.
  Binary(LessThan/GE/GT/LE) -> Int: SBC with V-correction + Branch.
                               Multi-byte: chained SBCs (carry threads
                               low→high), V-correction on the final
                               high-byte result, branch on MI/PL. Same
                               operand-swap trick used for `>` / `<=`.
  Binary(Multiply/Divide/   -> Runtime helper Calls. Operands and
    Modulo/LeftShift/         results are exchanged through the
    RightShift)               shared zero-page slot block `HARGS`
                               (24 bytes). Width-driven helper choice:
                               8-bit operands dispatch to
                               mul8/divmod8/asl8/asr8; 16-bit to
                               mul16/divmod16/asl16/asr16; 32-bit to
                               mul32/divmod32/asl32/asr32. Caller
                               writes inputs into HARGS+0..N-1, JSRs,
                               reads the result from a fixed offset
                               later in the block (see the constants
                               section for each helper's layout).
  FunctionCall(name, args,  -> AllocateStack(total_arg_bytes); write
              dst)             each arg's bytes into Stack(off..off+
                               size-1) in source order; Call name;
                               copy return value out (per the
                               return-value convention below).
  Jump/Label                -> atom-for-atom.
  JumpIfTrue/JumpIfFalse    -> Int: Mov(cond, A); Branch(NE/EQ).
                               Multi-byte: Mov(cond[0], A); chain
                               Or(cond[k], A) for k=1..size-1;
                               Branch(NE/EQ). The OR sets Z=1 iff
                               every byte is zero, which is the
                               whole-value "is zero" test.
  Constant(c)               -> Imm(c.int) (1-byte values direct; for
                               wider values, callers extract bytes
                               via `_byte_at`).
  Var(name)                 -> Pseudo(name, offset=0). Subsequent
                               `_byte_at(_, k)` calls bump offset to
                               address byte k.

Calling convention (callee-side, see also `replace_pseudoregisters`):
  - Each arg occupies size_of(arg_type) consecutive stack bytes (low
    at the lower offset for multi-byte args).
  - Return value rides in registers when small enough to fit, in the
    HARGS zero-page block when not:
      Int (1B)              → A.
      Long (2B)             → A = low byte, X = high byte.
      LongLong / Float (4B) → HARGS+8..11.
      Double (8B)           → HARGS+16..23.
    The FP slots are deliberately the same as the FP arithmetic
    helpers' output slots, so a function ending in `return a+b;` for
    FP operands needs no epilogue copy. FP returns also skip the
    epilogue's PHA/PLA pair (Ret(save_a=False)) — the SSP/FP
    arithmetic doesn't touch HARGS, so there's nothing to preserve.
"""

from __future__ import annotations

import struct

import asm_ast
import c99_ast
import tac_ast
from passes.type_checking import SymbolTable


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())

# Runtime helpers exchange operands through a shared 24-byte block
# in zero page (`$04`–`$1B`). The asm symbol `HARGS` names the
# block's base address (defined to $04 by the runtime header); each
# helper documents its own byte layout within HARGS+0..HARGS+23.
# Inputs sit at the low offsets, outputs at the high offsets, and
# inputs survive the call. The block is sized for the largest helper
# (a double-precision FP op needs 16 bytes of inputs + 8 bytes of
# output); integer helpers use only the low 8 bytes.
#
# 8-bit helpers:
#   mul8     in:  A=HARGS+0, B=HARGS+1   out: result.lo=HARGS+2,
#                                              result.hi=HARGS+3
#   divmod8  in:  num=HARGS+0, den=HARGS+1
#                                       out: quot=HARGS+2, rem=HARGS+3
#   asl8     in:  val=HARGS+0, count=HARGS+1
#                                       out: result=HARGS+2
#   asr8     same shape as asl8 (signed arithmetic right shift)
#
# 16-bit helpers:
#   mul16    in:  A=HARGS+0..1, B=HARGS+2..3
#                                       out: result=HARGS+4..7 (32-bit)
#   divmod16 in:  num=HARGS+0..1, den=HARGS+2..3
#                                       out: quot=HARGS+4..5,
#                                            rem=HARGS+6..7
#   asl16    in:  val=HARGS+0..1, count=HARGS+2 (1 byte)
#                                       out: result=HARGS+3..4
#   asr16    same shape as asl16
#
# FP arithmetic helpers (not implemented yet; layout reserved):
#   fadd/fsub/fmul/fdiv  in:  A=HARGS+0..3, B=HARGS+4..7
#                              out: result=HARGS+8..11
#   dadd/dsub/dmul/ddiv  in:  A=HARGS+0..7, B=HARGS+8..15
#                              out: result=HARGS+16..23
#
# FP/integer conversion helpers (not implemented yet; layout
# reserved). Inputs at low offsets, output immediately after
# (matching the existing helper convention). The signed/unsigned and
# narrow/wide variants are separate helpers because the runtime
# implementations differ — tac_to_asm picks the right name from the
# operand's symbol-table type (Int / UInt / Long / ULong vs. Float /
# Double):
#   i2f      (signed 1B → float):     in HARGS+0;     out HARGS+1..4
#   u2f      (unsigned 1B → float):   in HARGS+0;     out HARGS+1..4
#   l2f      (signed 2B → float):     in HARGS+0..1;  out HARGS+2..5
#   ul2f     (unsigned 2B → float):   in HARGS+0..1;  out HARGS+2..5
#   ll2f     (signed 4B → float):     in HARGS+0..3;  out HARGS+4..7
#   ull2f    (unsigned 4B → float):   in HARGS+0..3;  out HARGS+4..7
#   i2d      (signed 1B → double):    in HARGS+0;     out HARGS+1..8
#   u2d      (unsigned 1B → double):  in HARGS+0;     out HARGS+1..8
#   l2d      (signed 2B → double):    in HARGS+0..1;  out HARGS+2..9
#   ul2d     (unsigned 2B → double):  in HARGS+0..1;  out HARGS+2..9
#   ll2d     (signed 4B → double):    in HARGS+0..3;  out HARGS+4..11
#   ull2d    (unsigned 4B → double):  in HARGS+0..3;  out HARGS+4..11
#   f2i      (float → signed 1B):     in HARGS+0..3;  out HARGS+4
#   f2u      (float → unsigned 1B):   in HARGS+0..3;  out HARGS+4
#   f2l      (float → signed 2B):     in HARGS+0..3;  out HARGS+4..5
#   f2ul     (float → unsigned 2B):   in HARGS+0..3;  out HARGS+4..5
#   f2ll     (float → signed 4B):     in HARGS+0..3;  out HARGS+4..7
#   f2ull    (float → unsigned 4B):   in HARGS+0..3;  out HARGS+4..7
#   d2i      (double → signed 1B):    in HARGS+0..7;  out HARGS+8
#   d2u      (double → unsigned 1B):  in HARGS+0..7;  out HARGS+8
#   d2l      (double → signed 2B):    in HARGS+0..7;  out HARGS+8..9
#   d2ul     (double → unsigned 2B):  in HARGS+0..7;  out HARGS+8..9
#   d2ll     (double → signed 4B):    in HARGS+0..7;  out HARGS+8..11
#   d2ull    (double → unsigned 4B):  in HARGS+0..7;  out HARGS+8..11
#   f2d      (float → double):        in HARGS+0..3;  out HARGS+4..11
#   d2f      (double → float):        in HARGS+0..7;  out HARGS+8..11
_HARGS = "HARGS"
# DPTR is the dereference / scratch indirect-pointer pair. Reserved
# at zero-page `$1C`/`$1D` (right after HARGS). Used by Load /
# Store TAC ops: caller writes the 2-byte target address into
# `DPTR` / `DPTR+1`, then reads or writes through `(DPTR),Y` with Y
# = byte offset. Caller-saved like HARGS — any helper call may
# clobber it, so the byte sequence is always "stage to DPTR, then
# access" with no expectation that DPTR survives across other ops.
_DPTR = "DPTR"
# `icall` is the runtime trampoline for indirect function calls:
# its single instruction is `JMP (DPTR)`, which reads a 2-byte
# address from the DPTR zero-page slot and jumps there. The caller
# stages the function pointer's bytes into DPTR, then `JSR icall`
# — the JSR pushes the return address as usual, the trampoline
# JMP transfers control to the target, and the target's RTS pops
# back to the caller. Lives in the runtime header alongside the
# arithmetic helpers.
_ICALL = "icall"
_MUL8 = "mul8"
_DIVMOD8 = "divmod8"
_ASL8 = "asl8"
_ASR8 = "asr8"
_MUL16 = "mul16"
_DIVMOD16 = "divmod16"
_ASL16 = "asl16"
_ASR16 = "asr16"
# 32-bit integer helpers. Same "output slots immediately after
# inputs" convention as the 8/16-bit forms:
#   mul32    in:  A=HARGS+0..3, B=HARGS+4..7
#                                       out: result=HARGS+8..15 (64-bit)
#   divmod32 in:  num=HARGS+0..3, den=HARGS+4..7
#                                       out: quot=HARGS+8..11,
#                                            rem=HARGS+12..15
#   asl32    in:  val=HARGS+0..3, count=HARGS+4 (1 byte)
#                                       out: result=HARGS+5..8
#   asr32    same shape as asl32 (signed arithmetic right shift)
_MUL32 = "mul32"
_DIVMOD32 = "divmod32"
_ASL32 = "asl32"
_ASR32 = "asr32"
# Conversion helpers, keyed by (source-c99-type, target-c99-type) at
# the dispatch site. Signedness rides on the c99 type so we can pick
# i2f / u2f apart even though TAC's integer constants don't carry it.
_INT_TO_FLOAT = {
    c99_ast.Int:       "i2f",
    c99_ast.UInt:      "u2f",
    c99_ast.Long:      "l2f",
    c99_ast.ULong:     "ul2f",
    c99_ast.LongLong:  "ll2f",
    c99_ast.ULongLong: "ull2f",
    # Char types share width and signedness with Int / UInt, so
    # they re-use the same helpers. Integer-promotion in
    # type_checking lifts char operands to Int / UInt before
    # arithmetic, but a direct `(double)c` cast bypasses
    # promotion and hits this dispatch with a Char source.
    c99_ast.Char:      "i2f",
    c99_ast.SChar:     "i2f",
    c99_ast.UChar:     "u2f",
}
_INT_TO_DOUBLE = {
    c99_ast.Int:       "i2d",
    c99_ast.UInt:      "u2d",
    c99_ast.Long:      "l2d",
    c99_ast.ULong:     "ul2d",
    c99_ast.LongLong:  "ll2d",
    c99_ast.ULongLong: "ull2d",
    c99_ast.Char:      "i2d",
    c99_ast.SChar:     "i2d",
    c99_ast.UChar:     "u2d",
}
_FLOAT_TO_INT = {
    c99_ast.Int:       "f2i",
    c99_ast.UInt:      "f2u",
    c99_ast.Long:      "f2l",
    c99_ast.ULong:     "f2ul",
    c99_ast.LongLong:  "f2ll",
    c99_ast.ULongLong: "f2ull",
    c99_ast.Char:      "f2i",
    c99_ast.SChar:     "f2i",
    c99_ast.UChar:     "f2u",
}
_DOUBLE_TO_INT = {
    c99_ast.Int:       "d2i",
    c99_ast.UInt:      "d2u",
    c99_ast.Long:      "d2l",
    c99_ast.ULong:     "d2ul",
    c99_ast.LongLong:  "d2ll",
    c99_ast.ULongLong: "d2ull",
    c99_ast.Char:      "d2i",
    c99_ast.SChar:     "d2i",
    c99_ast.UChar:     "d2u",
}
_FLOAT_TO_DOUBLE = "f2d"
_DOUBLE_TO_FLOAT = "d2f"


def _to_asm_static_init(
    init: tac_ast.Type_static_init,
) -> asm_ast.Type_static_init:
    """Translate a TAC static_init to its asm counterpart. The asm
    integer side carries only the three width variants (`IntInit` /
    `LongInit` / `LongLongInit`), so unsigned variants from TAC
    collapse onto the matching width: UIntInit → IntInit,
    ULongInit → LongInit, ULongLongInit → LongLongInit. The
    integer value passes through unchanged; asm_emit's `_check_byte`
    (0..255), `_check_word` (-32768..65535), and `_check_dword`
    (-2^31..2^32-1) bound the rendered cell. The FP side keeps
    Float / Double distinct because their IEEE 754 byte layouts
    differ — FloatInit is 4 bytes (single), DoubleInit is 8 bytes
    (double); the float value rides through 1-to-1 and asm_emit
    packs it via `struct.pack` at emit time."""
    match init:
        case tac_ast.IntInit(int=v):
            return asm_ast.IntInit(int=v)
        case tac_ast.LongInit(int=v):
            return asm_ast.LongInit(int=v)
        case tac_ast.LongLongInit(int=v):
            return asm_ast.LongLongInit(int=v)
        case tac_ast.UIntInit(int=v):
            return asm_ast.IntInit(int=v)
        case tac_ast.ULongInit(int=v):
            return asm_ast.LongInit(int=v)
        case tac_ast.ULongLongInit(int=v):
            return asm_ast.LongLongInit(int=v)
        case tac_ast.FloatInit(float=v):
            return asm_ast.FloatInit(float=v)
        case tac_ast.DoubleInit(float=v):
            return asm_ast.DoubleInit(float=v)
        case tac_ast.AddressInit(name=n, offset=off):
            return asm_ast.AddressInit(name=n, offset=off)
        case tac_ast.StringInit(str=s, bytes=b):
            return asm_ast.StringInit(str=s, bytes=b)
        case tac_ast.ZeroInit(bytes=b):
            return asm_ast.ZeroInit(bytes=b)
    raise TypeError(f"unexpected static_init: {init!r}")


def _byte_at(op: asm_ast.Type_operand, k: int) -> asm_ast.Type_operand:
    """Address the k-th byte (0 = low, 1 = high) of a multi-byte
    operand. For `Imm`, extract that byte of the constant — Python's
    `>>` on a negative int is arithmetic, so a negative Long folds
    to its two's-complement bytes (e.g. `-1 → low=$FF, high=$FF`).
    For memory-shaped operands (`Pseudo`/`Stack`/`Frame`/`Data`),
    bump the operand's `offset` by k. Registers don't have an offset
    concept (they're 1-byte), so they're rejected here — callers
    should never reach this with a `Reg`."""
    match op:
        case asm_ast.Imm(value=v):
            return asm_ast.Imm(value=(v >> (8 * k)) & 0xFF)
        case asm_ast.Pseudo(name=n, offset=base):
            return asm_ast.Pseudo(name=n, offset=base + k)
        case asm_ast.Stack(offset=base):
            return asm_ast.Stack(offset=base + k)
        case asm_ast.Frame(offset=base):
            return asm_ast.Frame(offset=base + k)
        case asm_ast.Data(name=n, offset=base):
            return asm_ast.Data(name=n, offset=base + k)
    raise TypeError(f"can't address byte {k} of {op!r}")


class Translator:
    """One Translator per program (so the `make_label` counter is
    program-global). Holds the type-checker's symbol table so it can
    look up TAC `Var` types and pick the right operand-size dispatch
    for each instruction."""

    def __init__(self, symbols: SymbolTable | None = None) -> None:
        self._label_counter = 0
        # Optional — synthetic-AST tests can build a Translator
        # without a symbol table. In that case `_size_of` falls back
        # to `1` (Byte) for any Var whose name isn't found, which
        # matches the Int-only world the existing tests assume.
        self._symbols = symbols

    def _is_pointer_val(self, val: tac_ast.Type_val) -> bool:
        """True iff `val` is a Var whose c99 symbol type is Pointer.
        Constants never qualify — TAC's `const` sum has no pointer
        variant (Pointer collapses onto Long for sizing). Used by
        the ordering-op dispatch to pick unsigned vs. signed
        comparison: pointers are 16-bit unsigned addresses, so
        unsigned ordering is the only sensible interpretation."""
        if not isinstance(val, tac_ast.Var):
            return False
        if self._symbols is None:
            return False
        sym = self._symbols.get(val.name)
        if sym is None:
            return False
        return isinstance(sym.type, c99_ast.Pointer)

    def _size_of(self, val: tac_ast.Type_val) -> int:
        """Byte width of a TAC val. Integer types: 1 for Int / UInt
        / Char / SChar / UChar, 2 for Long / ULong, 4 for LongLong
        / ULongLong. Floating types: 4 for Float, 8 for Double.
        Pointer: 2 (the 6502's address width). Constants dispatch
        on the const variant; Vars look up the symbol-table c99
        type. Unknown Vars (synthetic test AST) default to 1."""
        match val:
            case tac_ast.Constant(const=tac_ast.ConstLong()):
                return 2
            case tac_ast.Constant(const=tac_ast.ConstLongLong()):
                return 4
            case tac_ast.Constant(const=tac_ast.ConstInt()):
                return 1
            case tac_ast.Constant(const=tac_ast.ConstFloat()):
                return 4
            case tac_ast.Constant(const=tac_ast.ConstDouble()):
                return 8
            case tac_ast.Var(name=name):
                sym = (
                    self._symbols.get(name)
                    if self._symbols is not None else None
                )
                if sym is None:
                    return 1
                if isinstance(
                    sym.type,
                    (c99_ast.Long, c99_ast.ULong, c99_ast.Pointer),
                ):
                    return 2
                if isinstance(
                    sym.type, (c99_ast.LongLong, c99_ast.ULongLong),
                ):
                    return 4
                if isinstance(sym.type, c99_ast.Float):
                    return 4
                if isinstance(sym.type, c99_ast.Double):
                    return 8
                return 1
        raise TypeError(f"unexpected val: {val!r}")

    def make_label(self, prefix: str) -> str:
        # Leading `.` makes this a dasm-style local label, scoped to
        # the enclosing SUBROUTINE. The `@` separator (illegal in any
        # C identifier) keeps these disjoint from any user-written
        # name.
        name = f".{prefix}@{self._label_counter}"
        self._label_counter += 1
        return name

    def translate_program(
        self, prog: tac_ast.Type_program,
    ) -> asm_ast.Type_program:
        # Each TAC `Function` lowers to an asm `Function`; each TAC
        # `StaticVariable` rides through to an asm `StaticVariable`
        # unchanged, with the typed init (`IntInit | LongInit`)
        # rewrapped 1-to-1. The variant of the init alone determines
        # the cell size at emit (DC.B for IntInit, DC.W for LongInit);
        # the asm side has no separate `data_type` field.
        match prog:
            case tac_ast.Program(top_level=top_levels):
                out: list[asm_ast.Type_top_level] = []
                for tl in top_levels:
                    if isinstance(tl, tac_ast.StaticVariable):
                        out.append(asm_ast.StaticVariable(
                            name=tl.name,
                            is_global=tl.is_global,
                            init=[_to_asm_static_init(i) for i in tl.init],
                        ))
                    else:
                        out.append(self.translate_function(tl))
                return asm_ast.Program(top_level=out)
        raise TypeError(f"unexpected program node: {prog!r}")

    def translate_function(
        self, fn: tac_ast.Type_top_level,
    ) -> asm_ast.Function:
        match fn:
            case tac_ast.Function(
                name=name, is_global=is_global,
                params=params, instructions=instrs,
            ):
                # Parameter names ride through to the asm Function
                # so the frame-layout pass can place them at the
                # right Frame offsets. References to params inside
                # the body are TAC `Var(@<N>.<orig>)` and lower to
                # `Pseudo(@<N>.<orig>, offset=0)` like any other TAC
                # variable; replace_pseudoregisters uses the symbol
                # table to size each pseudo (1 byte for Int, 2 for
                # Long) and resolves Pseudo(name, offset=k) to the
                # k-th byte of its allocated slot. References to
                # static-storage objects also pass through as Pseudo
                # here; replace_pseudoregisters distinguishes them
                # by name and rewrites them as `Data(name, offset=k)`
                # for absolute-addressed access.
                out: list[asm_ast.Type_instruction] = []
                for instr in instrs:
                    out.extend(self.translate_instruction(instr))
                return asm_ast.Function(
                    name=name,
                    is_global=is_global,
                    params=list(params),
                    instructions=out,
                )
        raise TypeError(f"unexpected function node: {fn!r}")

    def translate_instruction(
        self, instr: tac_ast.Type_instruction,
    ) -> list[asm_ast.Type_instruction]:
        match instr:
            case tac_ast.Ret(val=val):
                return self._translate_ret(val)
            case tac_ast.SignExtend(src=src, dst=dst):
                return self._translate_sign_extend(src, dst)
            case tac_ast.ZeroExtend(src=src, dst=dst):
                return self._translate_zero_extend(src, dst)
            case tac_ast.Truncate(src=src, dst=dst):
                # Memory is little-endian, so byte k of a multi-byte
                # value sits at offset+k. A wider→narrower truncation
                # copies the dst-width worth of low bytes from src and
                # discards the source's higher bytes.
                src_op = translate_val(src)
                dst_op = translate_val(dst)
                tgt_w = self._size_of(dst)
                return [
                    asm_ast.Mov(
                        src=_byte_at(src_op, k),
                        dst=_byte_at(dst_op, k),
                    )
                    for k in range(tgt_w)
                ]
            case tac_ast.IntToFloat(src=src, dst=dst):
                return self._translate_int_to_fp(src, dst, target_double=False)
            case tac_ast.IntToDouble(src=src, dst=dst):
                return self._translate_int_to_fp(src, dst, target_double=True)
            case tac_ast.FloatToInt(src=src, dst=dst):
                return self._translate_fp_to_int(src, dst, source_double=False)
            case tac_ast.DoubleToInt(src=src, dst=dst):
                return self._translate_fp_to_int(src, dst, source_double=True)
            case tac_ast.FloatToDouble(src=src, dst=dst):
                # Float → Double widens IEEE 754 single (4B) to double
                # (8B). Inputs and outputs go through HARGS — see the
                # `f2d` / `d2f` layout in the constants section.
                return _translate_helper_call(
                    inputs=[(translate_val(src), 4)],
                    helper=_FLOAT_TO_DOUBLE,
                    output_offset=4, output_size=8,
                    dst_op=translate_val(dst),
                )
            case tac_ast.DoubleToFloat(src=src, dst=dst):
                return _translate_helper_call(
                    inputs=[(translate_val(src), 8)],
                    helper=_DOUBLE_TO_FLOAT,
                    output_offset=8, output_size=4,
                    dst_op=translate_val(dst),
                )
            case tac_ast.Unary(op=op, src=src, dst=dst):
                return self._translate_unary(op, src, dst)
            case tac_ast.Binary(op=op, src1=src1, src2=src2, dst=dst):
                return self.translate_binary(op, src1, src2, dst)
            case tac_ast.Copy(src=src, dst=dst):
                return self._translate_copy(src, dst)
            case tac_ast.Jump(target=target):
                return [asm_ast.Jump(target=target)]
            case tac_ast.Label(name=name):
                return [asm_ast.Label(name=name)]
            case tac_ast.JumpIfTrue(condition=cond, target=target):
                return self._translate_cond_jump(
                    cond, target, asm_ast.NE(),
                )
            case tac_ast.JumpIfFalse(condition=cond, target=target):
                return self._translate_cond_jump(
                    cond, target, asm_ast.EQ(),
                )
            case tac_ast.FunctionCall(name=name, args=args, dst=dst):
                return self._translate_function_call(name, args, dst)
            case tac_ast.IndirectCall(ptr=ptr, args=args, dst=dst):
                return self._translate_indirect_call(ptr, args, dst)
            case tac_ast.GetAddress(operand=operand, dst=dst):
                return self._translate_get_address(operand, dst)
            case tac_ast.Load(src_ptr=src_ptr, dst=dst):
                return self._translate_load(src_ptr, dst)
            case tac_ast.Store(src=src, dst_ptr=dst_ptr):
                return self._translate_store(src, dst_ptr)
        raise TypeError(f"unexpected instruction node: {instr!r}")

    # ------------------------------------------------------------------
    # Per-instruction lowerings
    # ------------------------------------------------------------------

    def _translate_ret(
        self, val: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """Stage the return value, then Ret. Convention by width:
          Int (1B)              → A.
          Long (2B)             → A = low byte, X = high byte.
          LongLong / Float (4B) → HARGS+8..11.
          Double (8B)           → HARGS+16..23.
        1- and 2-byte integer returns ride in registers because the
        epilogue's PHA/PLA can preserve A across the SSP/FP
        arithmetic cheaply (and X isn't touched by that arithmetic).
        Wider returns ride through HARGS — for FP these are the
        same offsets the arithmetic helpers write to (`fadd` /
        `fsub` / `fmul` / `fdiv` → HARGS+8..11; `dadd` / `dsub` /
        `dmul` / `ddiv` → HARGS+16..23), so `return a OP b;` for FP
        operands leaves the result in the right slot already — no
        epilogue copy. LongLong (4B integer) reuses the Float
        return slot HARGS+8..11; types are exclusive per call so
        the overlap is fine, and `mul32` / `divmod32` write to that
        same offset for the same no-copy-when-possible reason. The
        `save_a=False` Ret skips the PHA/PLA pair the
        register-path uses; HARGS isn't touched by SSP/FP
        arithmetic. arg_bytes/local_bytes are placeholders patched
        by replace_pseudoregisters."""
        src_op = translate_val(val)
        size = self._size_of(val)
        if size == 1:
            return [
                asm_ast.Mov(src=src_op, dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ]
        if size == 2:
            # Long: load high byte into X via A, then low byte into
            # A. We do high first so A holds low at the call point
            # — same convention as mul8.
            return [
                asm_ast.Mov(src=_byte_at(src_op, 1), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=_byte_at(src_op, 0), dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ]
        # LongLong / Float (4B) → HARGS+8..11; Double (8B) →
        # HARGS+16..23. Write the src bytes into HARGS, then Ret
        # with save_a=False.
        out_off = 8 if size == 4 else 16
        seq: list[asm_ast.Type_instruction] = []
        for k in range(size):
            seq.append(asm_ast.Mov(src=_byte_at(src_op, k), dst=_REG_A))
            seq.append(asm_ast.Mov(src=_REG_A, dst=_hargs(out_off + k)))
        seq.append(asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=False))
        return seq

    def _translate_copy(
        self, src: tac_ast.Type_val, dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        src_op = translate_val(src)
        dst_op = translate_val(dst)
        size = self._size_of(src)
        # Per-byte Mov; the emitter handles every legal addressing
        # pair (Imm→Reg, Reg→Mem, Mem→Mem, etc.) and routes through A
        # for memory-to-memory shapes.
        return [
            asm_ast.Mov(src=_byte_at(src_op, k), dst=_byte_at(dst_op, k))
            for k in range(size)
        ]

    def _translate_sign_extend(
        self, src: tac_ast.Type_val, dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """Signed narrower→wider widening (Int → Long, Int → LongLong,
        Long → LongLong, etc.). Copy each source byte into the
        matching dst byte, then dispatch on the source's high-byte
        sign to write $00 (positive) or $FF (negative) to each of
        dst's remaining (high) bytes. The last LDA in the byte-copy
        loop is of the source's high byte, which sets N from its
        sign; the intervening STAs preserve flags so the BMI below
        sees the right N. Two minted labels per use; the Translator's
        counter keeps them globally unique."""
        src_op = translate_val(src)
        dst_op = translate_val(dst)
        src_w = self._size_of(src)
        tgt_w = self._size_of(dst)
        neg_label = self.make_label("sx_neg")
        end_label = self.make_label("sx_done")
        out: list[asm_ast.Type_instruction] = []
        for k in range(src_w):
            out.append(asm_ast.Mov(src=_byte_at(src_op, k), dst=_REG_A))
            out.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
        out.extend([
            asm_ast.Branch(cond=asm_ast.MI(), target=neg_label),
            asm_ast.Mov(src=asm_ast.Imm(value=0x00), dst=_REG_A),
            asm_ast.Jump(target=end_label),
            asm_ast.Label(name=neg_label),
            asm_ast.Mov(src=asm_ast.Imm(value=0xFF), dst=_REG_A),
            asm_ast.Label(name=end_label),
        ])
        for k in range(src_w, tgt_w):
            out.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
        return out

    def _translate_zero_extend(
        self, src: tac_ast.Type_val, dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """Unsigned narrower→wider widening (UInt → ULong, UInt →
        ULongLong, ULong → ULongLong, etc.). Copy each source byte
        unchanged, then write a literal 0 into each of dst's
        remaining (high) bytes. No branch needed — the new high
        bytes are unconditionally zero. Routes each byte through A
        so memory-to-memory dst layouts work uniformly."""
        src_op = translate_val(src)
        dst_op = translate_val(dst)
        src_w = self._size_of(src)
        tgt_w = self._size_of(dst)
        out: list[asm_ast.Type_instruction] = []
        for k in range(src_w):
            out.append(asm_ast.Mov(src=_byte_at(src_op, k), dst=_REG_A))
            out.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
        if src_w < tgt_w:
            out.append(asm_ast.Mov(
                src=asm_ast.Imm(value=0x00), dst=_REG_A,
            ))
            for k in range(src_w, tgt_w):
                out.append(asm_ast.Mov(
                    src=_REG_A, dst=_byte_at(dst_op, k),
                ))
        return out

    def _int_type_of(
        self, val: tac_ast.Type_val,
    ) -> type[c99_ast.Type_data_type]:
        """Look up the c99 integer type class (Int / UInt / Long /
        ULong) of a Var. The conversion-helper dispatch tables key on
        this class to pick i2f vs. u2f vs. l2f vs. ul2f (and similarly
        for the other directions). Constants don't reach here —
        c99_to_tac folds compile-time constant FP casts in Python
        because the TAC `const` sum doesn't carry signedness."""
        if not isinstance(val, tac_ast.Var):
            raise TypeError(
                "FP conversion helper dispatch requires a Var source/dst; "
                f"got {val!r}. c99_to_tac should fold Constant casts."
            )
        if self._symbols is None:
            raise TypeError(
                "FP conversion helper dispatch requires a symbol table"
            )
        sym = self._symbols.get(val.name)
        if sym is None:
            raise TypeError(f"unknown Var in FP conversion: {val.name}")
        return type(sym.type)

    def _translate_int_to_fp(
        self,
        src: tac_ast.Type_val, dst: tac_ast.Type_val,
        *, target_double: bool,
    ) -> list[asm_ast.Type_instruction]:
        """Int / UInt / Long / ULong → Float or Double via runtime
        helper. Source byte width comes from the symbol-table type;
        signedness picks i2f vs. u2f (or i2d vs. u2d, etc.). Output
        sits at HARGS+(input_bytes); see the helper layout table at
        the top of the file."""
        src_type = self._int_type_of(src)
        table = _INT_TO_DOUBLE if target_double else _INT_TO_FLOAT
        helper = table[src_type]
        in_bytes = self._size_of(src)
        out_bytes = 8 if target_double else 4
        return _translate_helper_call(
            inputs=[(translate_val(src), in_bytes)],
            helper=helper,
            output_offset=in_bytes, output_size=out_bytes,
            dst_op=translate_val(dst),
        )

    def _translate_fp_to_int(
        self,
        src: tac_ast.Type_val, dst: tac_ast.Type_val,
        *, source_double: bool,
    ) -> list[asm_ast.Type_instruction]:
        """Float or Double → Int / UInt / Long / ULong via runtime
        helper. Output byte width and signedness come from the dst's
        symbol-table type. The helpers truncate toward zero per
        C99 §6.3.1.4."""
        dst_type = self._int_type_of(dst)
        table = _DOUBLE_TO_INT if source_double else _FLOAT_TO_INT
        helper = table[dst_type]
        in_bytes = 8 if source_double else 4
        out_bytes = self._size_of(dst)
        return _translate_helper_call(
            inputs=[(translate_val(src), in_bytes)],
            helper=helper,
            output_offset=in_bytes, output_size=out_bytes,
            dst_op=translate_val(dst),
        )

    def _translate_unary(
        self,
        op: tac_ast.Type_unary_operator,
        src: tac_ast.Type_val,
        dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        src_op = translate_val(src)
        dst_op = translate_val(dst)
        # `!x` always yields Int (per the type checker), even for a
        # Long operand. Other unary ops preserve type.
        if isinstance(op, tac_ast.LogicalNot):
            return self._translate_logical_not(src_op, dst_op, src)
        size = self._size_of(src)
        if size == 1:
            return (
                [asm_ast.Mov(src=src_op, dst=_REG_A)]
                + self._unop_atoms(op)
                + [asm_ast.Mov(src=_REG_A, dst=dst_op)]
            )
        # Multi-byte Negate / Complement (Long, ULong, LongLong,
        # ULongLong — any size > 1).
        if isinstance(op, tac_ast.Complement):
            # ~X = X XOR $FF...FF. Each byte is independent — XOR
            # doesn't touch the carry flag, so byte order doesn't
            # matter and the loop fans out cleanly.
            out: list[asm_ast.Type_instruction] = []
            for k in range(size):
                out.append(asm_ast.Mov(src=_byte_at(src_op, k), dst=_REG_A))
                out.append(asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF),
                    dst=_REG_A,
                ))
                out.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
            return out
        if isinstance(op, tac_ast.Negate):
            # -X = (~X) + 1, two's complement at the operand width.
            # Complement each byte; for the low byte, CLC + ADC #1;
            # for each higher byte, ADC #0 (propagates the carry
            # from the previous ADC). LDA / EOR preserve C, so the
            # carry threads naturally across bytes.
            out2: list[asm_ast.Type_instruction] = []
            for k in range(size):
                out2.append(asm_ast.Mov(src=_byte_at(src_op, k), dst=_REG_A))
                out2.append(asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF),
                    dst=_REG_A,
                ))
                if k == 0:
                    out2.append(asm_ast.ClearCarry())
                    out2.append(asm_ast.Add(
                        src=asm_ast.Imm(value=0x01), dst=_REG_A,
                    ))
                else:
                    out2.append(asm_ast.Add(
                        src=asm_ast.Imm(value=0x00), dst=_REG_A,
                    ))
                out2.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
            return out2
        raise TypeError(f"unexpected unary operator: {op!r}")

    def _translate_logical_not(
        self,
        src_op: asm_ast.Type_operand,
        dst_op: asm_ast.Type_operand,
        src_val: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """!x: 1 if x is zero, 0 otherwise. Result is always Int
        (1 byte in dst). For a multi-byte source, OR all the bytes
        together first — the OR result is zero iff every byte is
        zero. Then the framing LDA's Z flag drives a Branch(EQ)
        into a 0/1 select."""
        true_label = self.make_label("lnot_true")
        end_label = self.make_label("lnot_end")
        size = self._size_of(src_val)
        if size == 1:
            head: list[asm_ast.Type_instruction] = [
                asm_ast.Mov(src=src_op, dst=_REG_A),
            ]
        else:
            head = [asm_ast.Mov(src=_byte_at(src_op, 0), dst=_REG_A)]
            for k in range(1, size):
                head.append(asm_ast.Or(src=_byte_at(src_op, k), dst=_REG_A))
        return head + [
            asm_ast.Branch(cond=asm_ast.EQ(), target=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target=end_label),
            asm_ast.Label(name=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Label(name=end_label),
            asm_ast.Mov(src=_REG_A, dst=dst_op),
        ]

    def _unop_atoms(
        self, op: tac_ast.Type_unary_operator,
    ) -> list[asm_ast.Type_instruction]:
        """Atomic 8-bit asm sequences implementing Complement /
        Negate on A. Result stays in A. LogicalNot is handled
        separately since it needs labels and a different shape."""
        match op:
            case tac_ast.Complement():
                # ~A = A XOR $FF
                return [asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF),
                    dst=_REG_A,
                )]
            case tac_ast.Negate():
                # -A = (~A) + 1, two's complement
                return [
                    asm_ast.Xor(
                        src1=_REG_A, src2=asm_ast.Imm(value=0xFF),
                        dst=_REG_A,
                    ),
                    asm_ast.ClearCarry(),
                    asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
                ]
        raise TypeError(f"unexpected unary operator: {op!r}")

    def _translate_cond_jump(
        self,
        cond: tac_ast.Type_val,
        target: str,
        branch_cond: asm_ast.Type_condition,
    ) -> list[asm_ast.Type_instruction]:
        """Conditional jump on the truthiness of `cond`. For Int,
        a single Mov sets Z based on the loaded byte and we Branch.
        For multi-byte conditions (Long, LongLong, …), OR every
        byte — the result is zero iff every byte is zero (i.e. the
        whole value is zero), and Z reflects that."""
        cond_op = translate_val(cond)
        size = self._size_of(cond)
        if size == 1:
            return [
                asm_ast.Mov(src=cond_op, dst=_REG_A),
                asm_ast.Branch(cond=branch_cond, target=target),
            ]
        out: list[asm_ast.Type_instruction] = [
            asm_ast.Mov(src=_byte_at(cond_op, 0), dst=_REG_A),
        ]
        for k in range(1, size):
            out.append(asm_ast.Or(src=_byte_at(cond_op, k), dst=_REG_A))
        out.append(asm_ast.Branch(cond=branch_cond, target=target))
        return out

    def _translate_function_call(
        self,
        name: str,
        args: list[tac_ast.Type_val],
        dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """Caller side of the soft-stack calling convention. Each arg
        occupies size_of(arg_type) consecutive stack bytes (1 for
        Int, 2 for Long), packed in source order starting at Stack(1).
        The callee's prologue saves FP, captures its own FP, and the
        epilogue rewinds SSP all the way back to the caller's pre-
        call value — no per-call cleanup at the call site.

        Return value, by dst width (read directly after the JSR,
        before any other helper call clobbers HARGS):
          Int (1B)              ← A.
          Long (2B)             ← A = low byte, X = high byte.
          LongLong / Float (4B) ← HARGS+8..11.
          Double (8B)           ← HARGS+16..23.
        See `_translate_ret` for the matching callee-side write.
        """
        # Compute total arg-stack bytes and per-arg base offsets.
        arg_sizes = [self._size_of(a) for a in args]
        total = sum(arg_sizes)
        emitted: list[asm_ast.Type_instruction] = []
        if total > 0:
            emitted.append(asm_ast.AllocateStack(bytes=total))
            off = 1
            for arg, sz in zip(args, arg_sizes):
                arg_op = translate_val(arg)
                for k in range(sz):
                    emitted.append(asm_ast.Mov(
                        src=_byte_at(arg_op, k),
                        dst=asm_ast.Stack(offset=off + k),
                    ))
                off += sz
        emitted.append(asm_ast.Call(name=name))
        dst_op = translate_val(dst)
        dst_size = self._size_of(dst)
        if dst_size == 1:
            # Int: from A directly.
            emitted.append(asm_ast.Mov(src=_REG_A, dst=dst_op))
        elif dst_size == 2:
            # Long: A = low (save first, before TXA clobbers it),
            # then X → A → high.
            emitted.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 0)))
            emitted.append(asm_ast.Mov(src=_REG_X, dst=_REG_A))
            emitted.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 1)))
        else:
            # LongLong / Float (4B) → HARGS+8..11;
            # Double (8B) → HARGS+16..23. Read it back byte-by-byte
            # through A.
            in_off = 8 if dst_size == 4 else 16
            for k in range(dst_size):
                emitted.append(asm_ast.Mov(src=_hargs(in_off + k), dst=_REG_A))
                emitted.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
        return emitted

    def _translate_indirect_call(
        self,
        ptr: tac_ast.Type_val,
        args: list[tac_ast.Type_val],
        dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """Indirect call through a function pointer. Same shape as
        the direct path — caller pushes args onto the soft stack,
        callee's epilogue rewinds SSP — but the call site stages
        the function pointer's two bytes into DPTR and JSRs the
        runtime `icall` trampoline (`icall: JMP (DPTR)`). The
        trampoline JSR pushes the return address as usual, the
        JMP indirect transfers control to the target function, and
        the function's RTS returns past the JSR icall. Return-value
        capture is identical to the direct path.

        DPTR is caller-saved like HARGS — the callee may clobber
        it freely. We just need it intact at the moment of the
        JSR icall, which the staging sequence guarantees."""
        # Push args onto the soft stack (same as direct).
        arg_sizes = [self._size_of(a) for a in args]
        total = sum(arg_sizes)
        emitted: list[asm_ast.Type_instruction] = []
        if total > 0:
            emitted.append(asm_ast.AllocateStack(bytes=total))
            off = 1
            for arg, sz in zip(args, arg_sizes):
                arg_op = translate_val(arg)
                for k in range(sz):
                    emitted.append(asm_ast.Mov(
                        src=_byte_at(arg_op, k),
                        dst=asm_ast.Stack(offset=off + k),
                    ))
                off += sz
        # Stage the function-pointer's two bytes into DPTR, then
        # JSR the trampoline.
        emitted.extend(self._stage_dptr(translate_val(ptr)))
        emitted.append(asm_ast.Call(name=_ICALL))
        # Capture return value — same byte plan as the direct path.
        dst_op = translate_val(dst)
        dst_size = self._size_of(dst)
        if dst_size == 1:
            emitted.append(asm_ast.Mov(src=_REG_A, dst=dst_op))
        elif dst_size == 2:
            emitted.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 0)))
            emitted.append(asm_ast.Mov(src=_REG_X, dst=_REG_A))
            emitted.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 1)))
        else:
            in_off = 8 if dst_size == 4 else 16
            for k in range(dst_size):
                emitted.append(asm_ast.Mov(src=_hargs(in_off + k), dst=_REG_A))
                emitted.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
        return emitted

    # ------------------------------------------------------------------
    # Pointer ops
    # ------------------------------------------------------------------

    def _translate_get_address(
        self, operand: tac_ast.Type_val, dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """`&x` — produce a `LoadAddress` compound asm node. The
        operand at TAC time is a Var naming the lvalue; we hand it
        through as a Pseudo so replace_pseudoregisters can dispatch
        on its storage class (local → FP-relative add; static →
        immediate label-half loads). dst is the 2-byte temp that
        holds the resulting address."""
        if not isinstance(operand, tac_ast.Var):
            raise TypeError(
                f"GetAddress operand must be a Var (an lvalue name); "
                f"got {operand!r}"
            )
        return [asm_ast.LoadAddress(
            src=asm_ast.Pseudo(name=operand.name, offset=0),
            dst=translate_val(dst),
        )]

    def _translate_load(
        self, src_ptr: tac_ast.Type_val, dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """`*p` (read) — stage the 2-byte address `p` into the DPTR
        zero-page pair, then read N bytes through `(DPTR),Y` into
        `dst`. N is dst's width per the symbol table."""
        ptr_op = translate_val(src_ptr)
        dst_op = translate_val(dst)
        n = self._size_of(dst)
        return [
            *self._stage_dptr(ptr_op),
            *_indirect_read(dst_op, n),
        ]

    def _translate_store(
        self, src: tac_ast.Type_val, dst_ptr: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """`*p = v` — stage the 2-byte address `p` into DPTR, then
        write `src`'s N bytes through `(DPTR),Y`. N is src's width."""
        ptr_op = translate_val(dst_ptr)
        src_op = translate_val(src)
        n = self._size_of(src)
        return [
            *self._stage_dptr(ptr_op),
            *_indirect_write(src_op, n),
        ]

    @staticmethod
    def _stage_dptr(
        ptr_op: asm_ast.Type_operand,
    ) -> list[asm_ast.Type_instruction]:
        """Copy the two address bytes from `ptr_op` into the DPTR
        zero-page pair. Routes each byte through A because A is the
        only register that loads uniformly from any source-operand
        kind (Imm, Pseudo, Frame, Data, Indirect)."""
        return [
            asm_ast.Mov(src=_byte_at(ptr_op, 0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name=_DPTR, offset=0)),
            asm_ast.Mov(src=_byte_at(ptr_op, 1), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name=_DPTR, offset=1)),
        ]

    # ------------------------------------------------------------------
    # Binary ops
    # ------------------------------------------------------------------

    def translate_binary(
        self,
        op: tac_ast.Type_binary_operator,
        src1: tac_ast.Type_val,
        src2: tac_ast.Type_val,
        dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """Lower a TAC Binary into asm. Operand size dispatches the
        sequence shape: Int → today's single-byte sequences; Long →
        per-byte sequences with carry threading where needed.

        The type checker promotes both operands to the common type
        before this point, so `_size_of(src1) == _size_of(src2)` is
        an invariant. The result type matches the operand type for
        arithmetic / bitwise / shift, and is always Int for
        comparisons (the size dispatch keys off the operand size,
        not the result size, since that's what determines the
        sequence shape)."""
        src1_op = translate_val(src1)
        src2_op = translate_val(src2)
        dst_op = translate_val(dst)
        size = self._size_of(src1)
        # Ordering ops on pointer operands use unsigned comparison
        # — pointers are 16-bit unsigned addresses, so the V-corrected
        # signed sequence would mis-rank addresses ≥ $8000 as
        # "negative". Type-checking forces both operands to be
        # pointers when one is, so checking either side suffices.
        pointer_cmp = (
            self._is_pointer_val(src1) or self._is_pointer_val(src2)
        )
        match op:
            case tac_ast.Add():
                return self._translate_add_sub(
                    src1_op, src2_op, dst_op, size,
                    setup=asm_ast.ClearCarry(), op_cls=asm_ast.Add,
                )
            case tac_ast.Subtract():
                return self._translate_add_sub(
                    src1_op, src2_op, dst_op, size,
                    setup=asm_ast.SetCarry(), op_cls=asm_ast.Sub,
                )
            case tac_ast.Multiply():
                # 8-bit:  mul8   result low byte at HARGS+2 (the
                #                high byte at HARGS+3 is discarded —
                #                int*int wraps to int under C's
                #                modular semantics).
                # 16-bit: mul16  result low half at HARGS+4..5 (the
                #                high half at HARGS+6..7 is discarded
                #                for the same reason).
                # 32-bit: mul32  result low 4 bytes at HARGS+8..11
                #                (the high 4 bytes at HARGS+12..15
                #                are discarded for the same reason).
                if size == 1:
                    helper, out_off = _MUL8, 2
                elif size == 2:
                    helper, out_off = _MUL16, 4
                else:
                    helper, out_off = _MUL32, 8
                return _translate_helper_call(
                    inputs=[(src1_op, size), (src2_op, size)],
                    helper=helper,
                    output_offset=out_off, output_size=size,
                    dst_op=dst_op,
                )
            case tac_ast.Divide():
                # divmod8/16/32 quotient sits at the byte(s)
                # immediately after the inputs; the remainder follows
                # at the next slot run (see Modulo below).
                if size == 1:
                    helper, out_off = _DIVMOD8, 2
                elif size == 2:
                    helper, out_off = _DIVMOD16, 4
                else:
                    helper, out_off = _DIVMOD32, 8
                return _translate_helper_call(
                    inputs=[(src1_op, size), (src2_op, size)],
                    helper=helper,
                    output_offset=out_off, output_size=size,
                    dst_op=dst_op,
                )
            case tac_ast.Modulo():
                # Remainder follows the quotient: divmod8 rem at
                # HARGS+3, divmod16 rem at HARGS+6..7, divmod32 rem
                # at HARGS+12..15.
                if size == 1:
                    helper, out_off = _DIVMOD8, 3
                elif size == 2:
                    helper, out_off = _DIVMOD16, 6
                else:
                    helper, out_off = _DIVMOD32, 12
                return _translate_helper_call(
                    inputs=[(src1_op, size), (src2_op, size)],
                    helper=helper,
                    output_offset=out_off, output_size=size,
                    dst_op=dst_op,
                )
            case tac_ast.BitwiseAnd():
                return self._translate_bitwise(
                    src1_op, src2_op, dst_op, size, asm_ast.And,
                )
            case tac_ast.BitwiseOr():
                return self._translate_bitwise(
                    src1_op, src2_op, dst_op, size, asm_ast.Or,
                )
            case tac_ast.BitwiseXor():
                return self._translate_xor(
                    src1_op, src2_op, dst_op, size,
                )
            case tac_ast.LeftShift():
                # asl8:  1B value + 1B count → 1B result at HARGS+2.
                # asl16: 2B value + 1B count → 2B result at HARGS+3.
                # asl32: 4B value + 1B count → 4B result at HARGS+5.
                # The type checker promotes both shift operands to a
                # common type, so for size>1 the count arrives wider
                # than 1 byte — we pass only its low byte (shifts by
                # ≥ width-in-bits are UB anyway, so the dropped high
                # bytes are irrelevant).
                if size == 1:
                    inputs = [(src1_op, 1), (src2_op, 1)]
                    helper, out_off = _ASL8, 2
                elif size == 2:
                    inputs = [(src1_op, 2), (src2_op, 1)]
                    helper, out_off = _ASL16, 3
                else:
                    inputs = [(src1_op, 4), (src2_op, 1)]
                    helper, out_off = _ASL32, 5
                return _translate_helper_call(
                    inputs=inputs, helper=helper,
                    output_offset=out_off, output_size=size,
                    dst_op=dst_op,
                )
            case tac_ast.RightShift():
                # `>>` always goes through the signed asr8/16/32 —
                # c6502 currently treats every integer as signed for
                # shift purposes (no logical-right-shift helper wired
                # up yet). Same input-byte layout as LeftShift.
                if size == 1:
                    inputs = [(src1_op, 1), (src2_op, 1)]
                    helper, out_off = _ASR8, 2
                elif size == 2:
                    inputs = [(src1_op, 2), (src2_op, 1)]
                    helper, out_off = _ASR16, 3
                else:
                    inputs = [(src1_op, 4), (src2_op, 1)]
                    helper, out_off = _ASR32, 5
                return _translate_helper_call(
                    inputs=inputs, helper=helper,
                    output_offset=out_off, output_size=size,
                    dst_op=dst_op,
                )
            case tac_ast.Equal():
                return self._translate_equality(
                    src1_op, src2_op, dst_op, size, asm_ast.EQ(),
                )
            case tac_ast.NotEqual():
                return self._translate_equality(
                    src1_op, src2_op, dst_op, size, asm_ast.NE(),
                )
            case tac_ast.LessThan():
                # src1 < src2: compute src1 - src2, branch on MI
                # (signed) or CC (unsigned, i.e. carry clear after
                # SBC means a borrow occurred = left < right).
                if pointer_cmp:
                    return self._translate_unsigned_ordering(
                        src1_op, src2_op, dst_op, size, asm_ast.CC(),
                    )
                return self._translate_signed_ordering(
                    src1_op, src2_op, dst_op, size, asm_ast.MI(),
                )
            case tac_ast.GreaterOrEqual():
                # src1 >= src2: branch on PL (signed) or CS
                # (unsigned, no borrow).
                if pointer_cmp:
                    return self._translate_unsigned_ordering(
                        src1_op, src2_op, dst_op, size, asm_ast.CS(),
                    )
                return self._translate_signed_ordering(
                    src1_op, src2_op, dst_op, size, asm_ast.PL(),
                )
            case tac_ast.GreaterThan():
                # src1 > src2 <=> src2 < src1. Same operand-swap
                # trick as the signed form.
                if pointer_cmp:
                    return self._translate_unsigned_ordering(
                        src2_op, src1_op, dst_op, size, asm_ast.CC(),
                    )
                return self._translate_signed_ordering(
                    src2_op, src1_op, dst_op, size, asm_ast.MI(),
                )
            case tac_ast.LessOrEqual():
                # src1 <= src2 <=> src2 >= src1.
                if pointer_cmp:
                    return self._translate_unsigned_ordering(
                        src2_op, src1_op, dst_op, size, asm_ast.CS(),
                    )
                return self._translate_signed_ordering(
                    src2_op, src1_op, dst_op, size, asm_ast.PL(),
                )
        raise TypeError(f"unexpected binary operator: {op!r}")

    def _translate_add_sub(
        self,
        src1_op: asm_ast.Type_operand,
        src2_op: asm_ast.Type_operand,
        dst_op: asm_ast.Type_operand,
        size: int,
        *,
        setup: asm_ast.Type_instruction,
        op_cls,
    ) -> list[asm_ast.Type_instruction]:
        """Add/Sub templated by size. Carry setup runs once before
        the low byte; the high byte's ADC/SBC reuses the carry
        produced by the low op (LDA only affects N/Z)."""
        out: list[asm_ast.Type_instruction] = []
        for k in range(size):
            out.append(asm_ast.Mov(src=_byte_at(src1_op, k), dst=_REG_A))
            if k == 0:
                out.append(setup)
            out.append(op_cls(src=_byte_at(src2_op, k), dst=_REG_A))
            out.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
        return out

    def _translate_bitwise(
        self,
        src1_op: asm_ast.Type_operand,
        src2_op: asm_ast.Type_operand,
        dst_op: asm_ast.Type_operand,
        size: int,
        op_cls,
    ) -> list[asm_ast.Type_instruction]:
        """And/Or templated by size. Each byte is independent (no
        carry / flag threading needed)."""
        out: list[asm_ast.Type_instruction] = []
        for k in range(size):
            out.append(asm_ast.Mov(src=_byte_at(src1_op, k), dst=_REG_A))
            out.append(op_cls(src=_byte_at(src2_op, k), dst=_REG_A))
            out.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
        return out

    def _translate_xor(
        self,
        src1_op: asm_ast.Type_operand,
        src2_op: asm_ast.Type_operand,
        dst_op: asm_ast.Type_operand,
        size: int,
    ) -> list[asm_ast.Type_instruction]:
        """Xor uses the asymmetric `Xor(src1, src2, dst)` shape so
        the emitter can pick which side carries the addressing
        mode. Same per-byte expansion as And/Or."""
        out: list[asm_ast.Type_instruction] = []
        for k in range(size):
            out.append(asm_ast.Mov(src=_byte_at(src1_op, k), dst=_REG_A))
            out.append(asm_ast.Xor(
                src1=_REG_A, src2=_byte_at(src2_op, k), dst=_REG_A,
            ))
            out.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
        return out

    def _translate_equality(
        self,
        src1_op: asm_ast.Type_operand,
        src2_op: asm_ast.Type_operand,
        dst_op: asm_ast.Type_operand,
        size: int,
        cond: asm_ast.Type_condition,
    ) -> list[asm_ast.Type_instruction]:
        """== / != via byte-wise Compare + Branch + 0/1 select. CMP
        sets Z=1 iff the two bytes are equal (no overflow concern),
        so this is correct for both signed and unsigned at the byte
        level. Generalizes to any operand size N (1, 2, 4, 8): walk
        the bytes from high (N-1) to low (0); on each byte except
        the last, BNE short-circuits to a "differ" label where
        Z=0 already encodes "not equal". The final low-byte CMP
        (no early-exit needed) leaves Z holding the answer.

        FP equality (size 4 / 8): byte-wise comparison matches IEEE
        754 equality EXCEPT for two known edge cases — NaN compares
        equal to itself byte-wise but C / IEEE say `NaN != NaN`,
        and `-0.0` and `+0.0` compare unequal byte-wise but IEEE
        says they're equal. c6502 has no FP runtime helpers yet, so
        this is the best we can do inline; correct results for
        every non-NaN, non-zero FP value the test corpus exercises.
        """
        true_label = self.make_label("cmp_true")
        end_label = self.make_label("cmp_end")
        out: list[asm_ast.Type_instruction] = []
        if size == 1:
            out.extend([
                asm_ast.Mov(src=src1_op, dst=_REG_A),
                asm_ast.Compare(left=_REG_A, right=src2_op),
            ])
        else:
            differ_label = self.make_label("cmp_differ")
            # High bytes first — if any byte differs, BNE early-
            # exits to differ_label without looking at the rest.
            for k in range(size - 1, 0, -1):
                out.extend([
                    asm_ast.Mov(src=_byte_at(src1_op, k), dst=_REG_A),
                    asm_ast.Compare(
                        left=_REG_A, right=_byte_at(src2_op, k),
                    ),
                    asm_ast.Branch(cond=asm_ast.NE(), target=differ_label),
                ])
            # Last byte (offset 0): no early-exit; Z holds the answer.
            out.extend([
                asm_ast.Mov(src=_byte_at(src1_op, 0), dst=_REG_A),
                asm_ast.Compare(
                    left=_REG_A, right=_byte_at(src2_op, 0),
                ),
                asm_ast.Label(name=differ_label),
            ])
        out.extend([
            asm_ast.Branch(cond=cond, target=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target=end_label),
            asm_ast.Label(name=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Label(name=end_label),
            asm_ast.Mov(src=_REG_A, dst=dst_op),
        ])
        return out

    def _translate_signed_ordering(
        self,
        left_op: asm_ast.Type_operand,
        right_op: asm_ast.Type_operand,
        dst_op: asm_ast.Type_operand,
        size: int,
        cond: asm_ast.Type_condition,
    ) -> list[asm_ast.Type_instruction]:
        """Signed ordering compare via SBC with V-correction:
           LDA left[0]; SEC; SBC right[0]
           LDA left[k];      SBC right[k]   for k in 1..size-1
           BVC novf; EOR #$80; novf:
           B<cond> true; LDA #0; JMP end; true: LDA #1; end: STA dst

        The V-correction handles signed overflow: when the
        subtraction overflows, the N flag from SBC is "wrong"
        relative to the mathematical sign of the difference, so
        EOR #$80 flips it. For multi-byte operands, we sub the low
        bytes first (carry threads via the existing carry register)
        then sub each higher byte in turn; the V flag from the
        FINAL (high-byte) SBC reflects the whole value's overflow,
        and N reflects the whole value's sign. Caller picks operand
        order and branch condition to select among <, >=, >, <=."""
        novf_label = self.make_label("cmp_novf")
        true_label = self.make_label("cmp_true")
        end_label = self.make_label("cmp_end")
        out: list[asm_ast.Type_instruction] = [
            asm_ast.Mov(src=_byte_at(left_op, 0), dst=_REG_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=_byte_at(right_op, 0), dst=_REG_A),
        ]
        for k in range(1, size):
            # Each subsequent byte: LDA (preserves C from previous
            # SBC), then SBC threads the borrow. After the loop the
            # N/V flags reflect the multi-byte subtraction.
            out.extend([
                asm_ast.Mov(src=_byte_at(left_op, k), dst=_REG_A),
                asm_ast.Sub(src=_byte_at(right_op, k), dst=_REG_A),
            ])
        out.extend([
            asm_ast.Branch(cond=asm_ast.VC(), target=novf_label),
            asm_ast.Xor(
                src1=_REG_A, src2=asm_ast.Imm(value=0x80), dst=_REG_A,
            ),
            asm_ast.Label(name=novf_label),
            asm_ast.Branch(cond=cond, target=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target=end_label),
            asm_ast.Label(name=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Label(name=end_label),
            asm_ast.Mov(src=_REG_A, dst=dst_op),
        ])
        return out

    def _translate_unsigned_ordering(
        self,
        left_op: asm_ast.Type_operand,
        right_op: asm_ast.Type_operand,
        dst_op: asm_ast.Type_operand,
        size: int,
        cond: asm_ast.Type_condition,
    ) -> list[asm_ast.Type_instruction]:
        """Unsigned ordering compare via SBC, no V-correction:
           LDA left[0]; SEC; SBC right[0]
           LDA left[k];      SBC right[k]   for k in 1..size-1
           B<cond> true; LDA #0; JMP end; true: LDA #1; end: STA dst

        After SBC, the carry flag is the unsigned ordering result:
        C=1 means no borrow (left >= right unsigned), C=0 means
        borrow (left < right unsigned). For multi-byte operands the
        borrow threads through the per-byte SBCs, so the final
        carry reflects the whole-value unsigned subtraction. Caller
        picks operand order and BCC/BCS to select among <, >=, >,
        <=.
        """
        true_label = self.make_label("cmp_true")
        end_label = self.make_label("cmp_end")
        out: list[asm_ast.Type_instruction] = [
            asm_ast.Mov(src=_byte_at(left_op, 0), dst=_REG_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=_byte_at(right_op, 0), dst=_REG_A),
        ]
        for k in range(1, size):
            out.extend([
                asm_ast.Mov(src=_byte_at(left_op, k), dst=_REG_A),
                asm_ast.Sub(src=_byte_at(right_op, k), dst=_REG_A),
            ])
        out.extend([
            asm_ast.Branch(cond=cond, target=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target=end_label),
            asm_ast.Label(name=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Label(name=end_label),
            asm_ast.Mov(src=_REG_A, dst=dst_op),
        ])
        return out

    def translate_unop_atoms(
        self, op: tac_ast.Type_unary_operator,
    ) -> list[asm_ast.Type_instruction]:
        """Backwards-compat shim for the old standalone-unop entry
        point. Returns the 8-bit atomic sequence on A; LogicalNot
        uses the inline labelled form. Tests that exercised the
        old entry point still work; production callers go through
        `translate_instruction(Unary(...))` which handles the
        framing Mov / Mov pair."""
        if isinstance(op, tac_ast.LogicalNot):
            true_label = self.make_label("lnot_true")
            end_label = self.make_label("lnot_end")
            return [
                asm_ast.Branch(cond=asm_ast.EQ(), target=true_label),
                asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
                asm_ast.Jump(target=end_label),
                asm_ast.Label(name=true_label),
                asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Label(name=end_label),
            ]
        return self._unop_atoms(op)


def _hargs(k: int) -> asm_ast.Type_operand:
    """Reference the k-th byte of the shared zero-page slot block,
    addressed absolutely as `HARGS+k` (collapses to `HARGS` when k=0).
    The runtime header pins `HARGS` at $04, so dasm picks zero-page
    addressing automatically."""
    return asm_ast.Data(name=_HARGS, offset=k)


def _indirect_read(
    dst: asm_ast.Type_operand, n: int,
) -> list[asm_ast.Type_instruction]:
    """Read `n` bytes through DPTR (already staged with the source
    address) into `dst`. Each byte routes through A: `LDY #k; LDA
    (DPTR),Y; STA dst+k`. Used by the Load TAC op."""
    out: list[asm_ast.Type_instruction] = []
    for k in range(n):
        out.append(asm_ast.Mov(src=asm_ast.Indirect(offset=k), dst=_REG_A))
        out.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst, k)))
    return out


def _indirect_write(
    src: asm_ast.Type_operand, n: int,
) -> list[asm_ast.Type_instruction]:
    """Write `n` bytes from `src` through DPTR (already staged with
    the destination address). Each byte routes through A: `LDY #k;
    LDA src+k; STA (DPTR),Y`. Used by the Store TAC op."""
    out: list[asm_ast.Type_instruction] = []
    for k in range(n):
        out.append(asm_ast.Mov(src=_byte_at(src, k), dst=_REG_A))
        out.append(asm_ast.Mov(src=_REG_A, dst=asm_ast.Indirect(offset=k)))
    return out


def _translate_helper_call(
    inputs: list[tuple[asm_ast.Type_operand, int]],
    helper: str,
    output_offset: int,
    output_size: int,
    dst_op: asm_ast.Type_operand,
) -> list[asm_ast.Type_instruction]:
    """Lower a TAC op that delegates to a runtime helper using the
    shared zero-page slot block. `inputs` is a list of
    `(source-operand, num-bytes)` pairs packed sequentially from
    `HARGS+0` upward; the helper reads them in place. After the
    call, copy `output_size` bytes from `HARGS+output_offset` into
    `dst_op`.

    Each byte routes through A — A is the only register that can
    load uniformly from any source-operand kind, and the only one
    that can store to a `Data` destination. Inputs survive the call
    so caller-side dst can safely overlap an input at the TAC level
    (matters for `x = x * y` etc. once value numbering elides
    redundant Copies)."""
    out: list[asm_ast.Type_instruction] = []
    slot = 0
    for src_op, sz in inputs:
        for k in range(sz):
            out.append(asm_ast.Mov(src=_byte_at(src_op, k), dst=_REG_A))
            out.append(asm_ast.Mov(src=_REG_A, dst=_hargs(slot)))
            slot += 1
    out.append(asm_ast.Call(name=helper))
    for k in range(output_size):
        out.append(asm_ast.Mov(
            src=_hargs(output_offset + k), dst=_REG_A,
        ))
        out.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, k)))
    return out


def translate_val(val: tac_ast.Type_val) -> asm_ast.Type_operand:
    """Translate a TAC val to its asm operand form. Constants
    become `Imm(value)`, where `value` is the bit-pattern integer
    of any width (1 byte for ConstInt, 2 for ConstLong, 4 for
    ConstFloat's IEEE 754 single, 8 for ConstDouble's IEEE 754
    double); the caller splits multi-byte values into bytes via
    `_byte_at`. FP constants pack to a non-negative bit pattern
    here (via `struct.pack`) so the same shift-and-mask byte
    extraction in `_byte_at` works without special-casing FP.
    Vars become `Pseudo(name, offset=0)`; callers that need to
    address higher bytes bump the offset via `_byte_at`."""
    match val:
        case tac_ast.Constant(const=tac_ast.ConstFloat(float=v)):
            (bits,) = struct.unpack("<I", struct.pack("<f", v))
            return asm_ast.Imm(value=bits)
        case tac_ast.Constant(const=tac_ast.ConstDouble(float=v)):
            (bits,) = struct.unpack("<Q", struct.pack("<d", v))
            return asm_ast.Imm(value=bits)
        case tac_ast.Constant(const=c):
            return asm_ast.Imm(value=c.int)
        case tac_ast.Var(name=n):
            return asm_ast.Pseudo(name=n, offset=0)
    raise TypeError(f"unexpected val node: {val!r}")


def translate_unop_atoms(
    op: tac_ast.Type_unary_operator,
) -> list[asm_ast.Type_instruction]:
    return Translator().translate_unop_atoms(op)


# Module-level wrappers: each call builds a fresh Translator (so the
# label counter restarts at 0). Use the Translator class directly when
# you need the counter to persist across calls.
def translate_program(
    prog: tac_ast.Type_program,
    symbols: SymbolTable | None = None,
) -> asm_ast.Type_program:
    """Convenience wrapper. The optional `symbols` table is the one
    `c99_to_tac` produced — the Translator consults it via
    `_size_of` to pick the right size dispatch (1-byte vs 2-byte
    sequence) for each instruction. Unit-test callers that only
    exercise Int paths can omit it; the Translator falls back to
    1-byte (Int) for any name not in the table."""
    return Translator(symbols).translate_program(prog)


def translate_function(
    fn: tac_ast.Type_top_level,
) -> asm_ast.Function:
    return Translator().translate_function(fn)


def translate_instruction(
    instr: tac_ast.Type_instruction,
) -> list[asm_ast.Type_instruction]:
    return Translator().translate_instruction(instr)


def translate_binary(
    op: tac_ast.Type_binary_operator,
    src1: tac_ast.Type_val,
    src2: tac_ast.Type_val,
    dst: tac_ast.Type_val,
) -> list[asm_ast.Type_instruction]:
    return Translator().translate_binary(op, src1, src2, dst)
