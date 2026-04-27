"""Translate a tac_ast tree into an asm_ast tree.

The asm IR is strictly 1:1 with 6502 opcodes — every node maps to
exactly one instruction (with the documented exceptions of `Ret`,
`FunctionPrologue`, and `AllocateStack`). The 6502 is an 8-bit
machine, so the IR has no width tagging: every operand is one byte.

`tac_to_asm` is therefore the home of all 16-bit lowering. For each
TAC instruction whose operands are `Long` (per the symbol table),
the translator emits a sequence of byte-level asm atoms — typically
two passes (low byte, high byte) with the 6502's carry flag
threading naturally between them for arithmetic. Multi-byte
operands are addressed via the `offset` field on `Pseudo` / `Stack`
/ `Frame` / `Data`: `Pseudo(name, offset=0)` is the low byte of
`name`, `Pseudo(name, offset=1)` the high byte; `Imm`
constants split into their low/high two's-complement bytes.

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
                               byte of a Long return), then Ret.
  Copy(src, dst)            -> 1× Mov for Int; 2× Mov (lo, hi) for Long.
  SignExtend(src, dst)      -> Mov(src, A); Mov(A, dst.lo); Branch(MI,
                               sx_neg@N); LDA #$00; Jump(sx_done@N);
                               Label(sx_neg@N); LDA #$FF; Label(sx_done@N);
                               Mov(A, dst.hi). The framing LDA sets N
                               based on the source byte's sign; STA
                               preserves flags so BMI sees the right N.
  Truncate(src, dst)        -> Mov(src.lo, dst). Memory is little-
                               endian so the source's offset-0 byte is
                               the low byte; the high byte is just
                               discarded.
  Unary(op, src, dst)       -> Mov src→A, atomic op on A, Mov A→dst
                               (Int). For Long operands the negate /
                               complement / logical-not lowerings are
                               byte-pair templates (see translate_unop).
  Binary(Add, …)            -> Int: Mov src1→A; CLC; Add(src2, A);
                               Mov A→dst.
                               Long: same pattern, twice in succession,
                               with no CLC between the low and high
                               adds — the carry from the low ADC
                               threads into the high ADC. (LDA only
                               affects N/Z, not C.)
  Binary(Subtract, …)       -> Same shape with SetCarry/Sub. The borrow
                               (in 6502 terms, an inverted-carry) also
                               threads through the high SBC.
  Binary(BitwiseAnd/Or/Xor) -> Byte op on each pair of bytes; no carry
                               threading needed (these don't touch C).
  Binary(Equal/NotEqual)    -> Int: Compare + Branch + 0/1 select.
                               Long: high CMP first; if differ, short-
                               circuit to a label that will see Z=0;
                               else fall through to low CMP (whose Z
                               is the final answer). 0/1 select after.
  Binary(LessThan/GE/GT/LE) -> Int: SBC with V-correction + Branch.
                               Long: low SBC then high SBC (carry
                               threads), V-correction on the high
                               result, branch on MI/PL. Same operand-
                               swap trick used for `>` / `<=`.
  Binary(Multiply/Divide/   -> Runtime helper Calls. Operands and
    Modulo/LeftShift/         results are exchanged through the
    RightShift)               shared zero-page slot block `HARGS`
                               (24 bytes); 8-bit operands dispatch
                               to mul8/divmod8/asl8/asr8, 16-bit
                               operands to mul16/divmod16/asl16/
                               asr16. Caller writes inputs into
                               HARGS+0..N-1, JSRs, reads the result
                               from a fixed offset later in the
                               block (see the constants section
                               for each helper's layout).
  FunctionCall(name, args,  -> AllocateStack(total_arg_bytes); write
              dst)             each arg's bytes into Stack(off..off+
                               size-1) in source order; Call name; copy
                               return value out of A (and X for Long).
  Jump/Label                -> atom-for-atom.
  JumpIfTrue/JumpIfFalse    -> Int: Mov(cond, A); Branch(NE/EQ).
                               Long: Mov(cond.lo, A); Or(cond.hi, A);
                               Branch(NE/EQ). The OR sets Z=1 iff
                               *both* bytes are zero, which is the
                               16-bit "is zero" test.
  Constant(c)               -> Imm(c.int) (1-byte values direct; for
                               Long values, callers extract bytes
                               via `_byte_at`).
  Var(name)                 -> Pseudo(name, offset=0). Subsequent
                               `_byte_at(_, k)` calls bump offset to
                               address byte k.

Calling convention (callee-side, see also `replace_pseudoregisters`):
  - Each Long arg occupies 2 stack bytes (low at the lower offset).
  - Long return value: low byte in A, high byte in X. (Runtime
    helpers use a separate ZP-slot convention — see the HARGS block
    above — so user-function and helper return paths are distinct.)
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
#   i2d      (signed 1B → double):    in HARGS+0;     out HARGS+1..8
#   u2d      (unsigned 1B → double):  in HARGS+0;     out HARGS+1..8
#   l2d      (signed 2B → double):    in HARGS+0..1;  out HARGS+2..9
#   ul2d     (unsigned 2B → double):  in HARGS+0..1;  out HARGS+2..9
#   f2i      (float → signed 1B):     in HARGS+0..3;  out HARGS+4
#   f2u      (float → unsigned 1B):   in HARGS+0..3;  out HARGS+4
#   f2l      (float → signed 2B):     in HARGS+0..3;  out HARGS+4..5
#   f2ul     (float → unsigned 2B):   in HARGS+0..3;  out HARGS+4..5
#   d2i      (double → signed 1B):    in HARGS+0..7;  out HARGS+8
#   d2u      (double → unsigned 1B):  in HARGS+0..7;  out HARGS+8
#   d2l      (double → signed 2B):    in HARGS+0..7;  out HARGS+8..9
#   d2ul     (double → unsigned 2B):  in HARGS+0..7;  out HARGS+8..9
#   f2d      (float → double):        in HARGS+0..3;  out HARGS+4..11
#   d2f      (double → float):        in HARGS+0..7;  out HARGS+8..11
_HARGS = "HARGS"
_MUL8 = "mul8"
_DIVMOD8 = "divmod8"
_ASL8 = "asl8"
_ASR8 = "asr8"
_MUL16 = "mul16"
_DIVMOD16 = "divmod16"
_ASL16 = "asl16"
_ASR16 = "asr16"
# Conversion helpers, keyed by (source-c99-type, target-c99-type) at
# the dispatch site. Signedness rides on the c99 type so we can pick
# i2f / u2f apart even though TAC's integer constants don't carry it.
_INT_TO_FLOAT = {
    c99_ast.Int: "i2f",
    c99_ast.UInt: "u2f",
    c99_ast.Long: "l2f",
    c99_ast.ULong: "ul2f",
}
_INT_TO_DOUBLE = {
    c99_ast.Int: "i2d",
    c99_ast.UInt: "u2d",
    c99_ast.Long: "l2d",
    c99_ast.ULong: "ul2d",
}
_FLOAT_TO_INT = {
    c99_ast.Int: "f2i",
    c99_ast.UInt: "f2u",
    c99_ast.Long: "f2l",
    c99_ast.ULong: "f2ul",
}
_DOUBLE_TO_INT = {
    c99_ast.Int: "d2i",
    c99_ast.UInt: "d2u",
    c99_ast.Long: "d2l",
    c99_ast.ULong: "d2ul",
}
_FLOAT_TO_DOUBLE = "f2d"
_DOUBLE_TO_FLOAT = "d2f"


def _to_asm_static_init(
    init: tac_ast.Type_static_init,
) -> asm_ast.Type_static_init:
    """Translate a TAC static_init to its asm counterpart. The asm
    integer side carries only the two width variants (`IntInit` /
    `LongInit`), so unsigned variants from TAC collapse onto the
    matching width: UIntInit → IntInit, ULongInit → LongInit. The
    integer value passes through unchanged; asm_emit's `_check_byte`
    (0..255) and `_check_word` (-32768..65535) bound the rendered
    cell. The FP side keeps Float / Double distinct because their
    IEEE 754 byte layouts differ — FloatInit is 4 bytes (single),
    DoubleInit is 8 bytes (double); the float value rides through
    1-to-1 and asm_emit packs it via `struct.pack` at emit time."""
    match init:
        case tac_ast.IntInit(int=v):
            return asm_ast.IntInit(int=v)
        case tac_ast.LongInit(int=v):
            return asm_ast.LongInit(int=v)
        case tac_ast.UIntInit(int=v):
            return asm_ast.IntInit(int=v)
        case tac_ast.ULongInit(int=v):
            return asm_ast.LongInit(int=v)
        case tac_ast.FloatInit(float=v):
            return asm_ast.FloatInit(float=v)
        case tac_ast.DoubleInit(float=v):
            return asm_ast.DoubleInit(float=v)
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

    def _size_of(self, val: tac_ast.Type_val) -> int:
        """Byte width of a TAC val. Integer types: 1 for Int / UInt,
        2 for Long / ULong. Floating types: 4 for Float, 8 for
        Double. Constants dispatch on the const variant; Vars look
        up the symbol-table type. Unknown Vars (synthetic test AST)
        default to 1."""
        match val:
            case tac_ast.Constant(const=tac_ast.ConstLong()):
                return 2
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
                if isinstance(sym.type, (c99_ast.Long, c99_ast.ULong)):
                    return 2
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
                            init=_to_asm_static_init(tl.init),
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
                # The k-th byte of a 2-byte memory layout is at
                # base+k (little-endian), so the source's offset-0
                # byte is the low byte. A Long → Int narrowing just
                # moves that low byte and discards the high.
                src_op = translate_val(src)
                dst_op = translate_val(dst)
                return [asm_ast.Mov(src=_byte_at(src_op, 0), dst=dst_op)]
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
        raise TypeError(f"unexpected instruction node: {instr!r}")

    # ------------------------------------------------------------------
    # Per-instruction lowerings
    # ------------------------------------------------------------------

    def _translate_ret(
        self, val: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """Stage the return value in registers, then Ret. Convention:
          Int return → A.
          Long return → A = low byte, X = high byte.
        Two-register Long return keeps the epilogue cheap (no extra
        memory traffic for short returns). Runtime helpers use a
        separate ZP-slot convention (see HARGS) since their multi-
        byte results don't fit in registers."""
        src_op = translate_val(val)
        size = self._size_of(val)
        if size == 1:
            return [
                asm_ast.Mov(src=src_op, dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ]
        # Long: load high byte into X via A, then low byte into A.
        # We do high first so A holds low at the call point — same
        # convention as mul8.
        return [
            asm_ast.Mov(src=_byte_at(src_op, 1), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Mov(src=_byte_at(src_op, 0), dst=_REG_A),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ]

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
        """Int → Long sign-extension. Load the source byte (sets N
        based on its sign), store it into the low byte of dst (STA
        preserves flags), then branch on the original N flag to
        write 0x00 / 0xFF to the high byte. Two minted labels per
        use; the Translator's counter keeps them globally unique."""
        src_op = translate_val(src)
        dst_op = translate_val(dst)
        neg_label = self.make_label("sx_neg")
        end_label = self.make_label("sx_done")
        return [
            asm_ast.Mov(src=src_op, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 0)),
            asm_ast.Branch(cond=asm_ast.MI(), target=neg_label),
            asm_ast.Mov(src=asm_ast.Imm(value=0x00), dst=_REG_A),
            asm_ast.Jump(target=end_label),
            asm_ast.Label(name=neg_label),
            asm_ast.Mov(src=asm_ast.Imm(value=0xFF), dst=_REG_A),
            asm_ast.Label(name=end_label),
            asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 1)),
        ]

    def _translate_zero_extend(
        self, src: tac_ast.Type_val, dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """UInt → ULong (or UInt → Long) zero-extension. Copy the
        source byte into the dst's low byte, then write a literal 0
        into the high byte. No branch needed: the new high byte is
        unconditionally zero, regardless of the source's bit pattern.
        Routes the source byte through A so memory-to-memory dst
        layouts work uniformly."""
        src_op = translate_val(src)
        dst_op = translate_val(dst)
        return [
            asm_ast.Mov(src=src_op, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 0)),
            asm_ast.Mov(src=asm_ast.Imm(value=0x00), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 1)),
        ]

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
        # Long Negate / Complement.
        if isinstance(op, tac_ast.Complement):
            # ~X = X XOR $FFFF. Independent on each byte.
            return [
                asm_ast.Mov(src=_byte_at(src_op, 0), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF),
                    dst=_REG_A,
                ),
                asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 0)),
                asm_ast.Mov(src=_byte_at(src_op, 1), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF),
                    dst=_REG_A,
                ),
                asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 1)),
            ]
        if isinstance(op, tac_ast.Negate):
            # -X = (~X) + 1, two's complement, 16-bit. Complement
            # both bytes, then a 16-bit ADC #1: low ADC adds 1 with
            # CLC; high ADC adds 0 to propagate carry.
            return [
                # Low byte: complement, then add 1.
                asm_ast.Mov(src=_byte_at(src_op, 0), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF),
                    dst=_REG_A,
                ),
                asm_ast.ClearCarry(),
                asm_ast.Add(src=asm_ast.Imm(value=0x01), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 0)),
                # High byte: complement, then add 0 with the carry
                # from the low ADC. LDA preserves C.
                asm_ast.Mov(src=_byte_at(src_op, 1), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF),
                    dst=_REG_A,
                ),
                asm_ast.Add(src=asm_ast.Imm(value=0x00), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 1)),
            ]
        raise TypeError(f"unexpected unary operator: {op!r}")

    def _translate_logical_not(
        self,
        src_op: asm_ast.Type_operand,
        dst_op: asm_ast.Type_operand,
        src_val: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """!x: 1 if x is zero, 0 otherwise. Result is always Int
        (1 byte in dst). For a Long source, OR the two bytes first
        — the result is zero iff both bytes are zero. Then the
        framing LDA's Z flag drives a Branch(EQ) into a 0/1 select."""
        true_label = self.make_label("lnot_true")
        end_label = self.make_label("lnot_end")
        size = self._size_of(src_val)
        if size == 1:
            head = [asm_ast.Mov(src=src_op, dst=_REG_A)]
        else:
            head = [
                asm_ast.Mov(src=_byte_at(src_op, 0), dst=_REG_A),
                asm_ast.Or(src=_byte_at(src_op, 1), dst=_REG_A),
            ]
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
        For Long, OR the two bytes — the result is zero iff both
        bytes are zero (i.e. the 16-bit value is zero), and Z
        reflects that."""
        cond_op = translate_val(cond)
        size = self._size_of(cond)
        if size == 1:
            return [
                asm_ast.Mov(src=cond_op, dst=_REG_A),
                asm_ast.Branch(cond=branch_cond, target=target),
            ]
        return [
            asm_ast.Mov(src=_byte_at(cond_op, 0), dst=_REG_A),
            asm_ast.Or(src=_byte_at(cond_op, 1), dst=_REG_A),
            asm_ast.Branch(cond=branch_cond, target=target),
        ]

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

        Return value: an Int return arrives in A. A Long return
        arrives with A = low byte, X = high byte. (User-function
        returns use registers; runtime helpers use the HARGS zero-
        page block instead.)
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
        # Capture return value. Int → from A; Long → from A (low)
        # and X (high), with X routed via A for the high-byte store.
        dst_op = translate_val(dst)
        dst_size = self._size_of(dst)
        if dst_size == 1:
            emitted.append(asm_ast.Mov(src=_REG_A, dst=dst_op))
        else:
            # Save the low byte first (A holds it). Then transfer X
            # to A and store the high byte. The order matters because
            # the second Mov clobbers A.
            emitted.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 0)))
            emitted.append(asm_ast.Mov(src=_REG_X, dst=_REG_A))
            emitted.append(asm_ast.Mov(src=_REG_A, dst=_byte_at(dst_op, 1)))
        return emitted

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
                # 8-bit: mul8 result low byte at HARGS+2 (the high
                # byte at HARGS+3 is discarded — int*int wraps to int
                # under C's modular semantics). 16-bit: mul16 result
                # low half at HARGS+4..5 (the high half at HARGS+6..7
                # is discarded for the same reason).
                helper, out_off = (
                    (_MUL8, 2) if size == 1 else (_MUL16, 4)
                )
                return _translate_helper_call(
                    inputs=[(src1_op, size), (src2_op, size)],
                    helper=helper,
                    output_offset=out_off, output_size=size,
                    dst_op=dst_op,
                )
            case tac_ast.Divide():
                # divmod8/divmod16 quotient sits at the byte(s)
                # immediately after the inputs; the remainder follows
                # at the next slot pair (see Modulo below).
                helper, out_off = (
                    (_DIVMOD8, 2) if size == 1 else (_DIVMOD16, 4)
                )
                return _translate_helper_call(
                    inputs=[(src1_op, size), (src2_op, size)],
                    helper=helper,
                    output_offset=out_off, output_size=size,
                    dst_op=dst_op,
                )
            case tac_ast.Modulo():
                helper, out_off = (
                    (_DIVMOD8, 3) if size == 1 else (_DIVMOD16, 6)
                )
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
                # asl8: 1B value + 1B count → 1B result at HARGS+2.
                # asl16: 2B value + 1B count → 2B result at HARGS+3.
                # The type checker promotes both shift operands to a
                # common type, so for size=2 the count arrives as 2B
                # — we pass only its low byte (shifts by ≥16 are UB
                # anyway, so the dropped high byte is irrelevant).
                if size == 1:
                    inputs = [(src1_op, 1), (src2_op, 1)]
                    helper, out_off = _ASL8, 2
                else:
                    inputs = [(src1_op, 2), (src2_op, 1)]
                    helper, out_off = _ASL16, 3
                return _translate_helper_call(
                    inputs=inputs, helper=helper,
                    output_offset=out_off, output_size=size,
                    dst_op=dst_op,
                )
            case tac_ast.RightShift():
                # `>>` always goes through the signed asr8/asr16 —
                # c6502 currently treats every integer as signed for
                # shift purposes (no logical-right-shift helper wired
                # up yet). Same input-byte layout as LeftShift.
                if size == 1:
                    inputs = [(src1_op, 1), (src2_op, 1)]
                    helper, out_off = _ASR8, 2
                else:
                    inputs = [(src1_op, 2), (src2_op, 1)]
                    helper, out_off = _ASR16, 3
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
                # src1 < src2 signed: compute src1 - src2, branch on MI.
                return self._translate_signed_ordering(
                    src1_op, src2_op, dst_op, size, asm_ast.MI(),
                )
            case tac_ast.GreaterOrEqual():
                # src1 >= src2 signed: compute src1 - src2, branch on PL.
                return self._translate_signed_ordering(
                    src1_op, src2_op, dst_op, size, asm_ast.PL(),
                )
            case tac_ast.GreaterThan():
                # src1 > src2 signed <=> src2 < src1 signed.
                return self._translate_signed_ordering(
                    src2_op, src1_op, dst_op, size, asm_ast.MI(),
                )
            case tac_ast.LessOrEqual():
                # src1 <= src2 signed <=> src2 >= src1 signed.
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
        """== / != via Compare + Branch + 0/1 select. CMP sets Z=1
        iff the bytes are equal (no overflow concern), so this is
        correct for both signed and unsigned at the byte level.

        For Long, CMP the high bytes first; if they differ, short-
        circuit to a label (via BNE) — at that label Z is 0, which
        is what we want for "not equal". Otherwise fall through to
        the low-byte CMP whose Z is the final answer.
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
            # High bytes first — if they differ the answer's "not
            # equal" without needing to look at the low bytes.
            out.extend([
                asm_ast.Mov(src=_byte_at(src1_op, 1), dst=_REG_A),
                asm_ast.Compare(
                    left=_REG_A, right=_byte_at(src2_op, 1),
                ),
                asm_ast.Branch(cond=asm_ast.NE(), target=differ_label),
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
           LDA left.lo; SEC; SBC right.lo
           [LDA left.hi;       SBC right.hi]   (Long only; carry threads)
           BVC novf; EOR #$80; novf:
           B<cond> true; LDA #0; JMP end; true: LDA #1; end: STA dst

        The V-correction handles signed overflow: when the
        subtraction overflows, the N flag from SBC is "wrong"
        relative to the mathematical sign of the difference, so
        EOR #$80 flips it. For Long, we sub the low bytes first
        (carry threads via the existing carry register) then sub
        the high bytes; the V flag from the high-byte SBC reflects
        the 16-bit overflow, and N reflects the 16-bit sign of the
        result. Caller picks operand order and branch condition to
        select among <, >=, >, <=."""
        novf_label = self.make_label("cmp_novf")
        true_label = self.make_label("cmp_true")
        end_label = self.make_label("cmp_end")
        out: list[asm_ast.Type_instruction] = [
            asm_ast.Mov(src=_byte_at(left_op, 0), dst=_REG_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=_byte_at(right_op, 0), dst=_REG_A),
        ]
        if size > 1:
            # High byte: load with LDA (preserves C from low SBC),
            # then SBC threads the borrow. Result's N/V reflect the
            # 16-bit subtraction.
            out.extend([
                asm_ast.Mov(src=_byte_at(left_op, 1), dst=_REG_A),
                asm_ast.Sub(src=_byte_at(right_op, 1), dst=_REG_A),
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
