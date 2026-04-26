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
  Binary(Multiply/Divide/   -> Runtime helper Calls. Today only the
    Modulo/LeftShift/         8-bit helpers (mul8/divmod8/shl8/asr8)
    RightShift)               are wired up, so a Long operand raises
                               NotImplementedError pending mul16/
                               divmod16/shl16/asr16.
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
  - Long return value: low byte in A, high byte in X (matches the
    mul8 / divmod8 helpers' existing convention).
"""

from __future__ import annotations

import asm_ast
import c99_ast
import tac_ast
from passes.type_checking import SymbolTable


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())

# Runtime helper names (8-bit). mul8 / divmod8 return both halves of
# the result in A and X. shl8 / asr8 take a value in A and a shift
# count in X and return the shifted value in A. The 16-bit helpers
# (mul16/divmod16/shl16/asr16) aren't in this repo yet; using `*`,
# `/`, `%`, `<<`, or `>>` on Long operands raises NotImplementedError.
_MUL8 = "mul8"
_DIVMOD8 = "divmod8"
_SHL8 = "shl8"
_ASR8 = "asr8"


def _to_asm_static_init(
    init: tac_ast.Type_static_init,
) -> asm_ast.Type_static_init:
    """Translate a TAC static_init to its asm counterpart. The two
    sums are 1-to-1 (`IntInit(int) | LongInit(int)`), so this is a
    pure rewrap — the variant tells the asm side whether to lay out
    a 1-byte (`IntInit`) or 2-byte (`LongInit`) cell."""
    match init:
        case tac_ast.IntInit(int=v):
            return asm_ast.IntInit(int=v)
        case tac_ast.LongInit(int=v):
            return asm_ast.LongInit(int=v)
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
        """1 for an Int-typed val, 2 for a Long-typed val. Constants
        dispatch on the const variant; Vars look up the symbol-table
        type. Unknown Vars (synthetic test AST) default to 1."""
        match val:
            case tac_ast.Constant(const=tac_ast.ConstLong()):
                return 2
            case tac_ast.Constant(const=tac_ast.ConstInt()):
                return 1
            case tac_ast.Var(name=name):
                sym = (
                    self._symbols.get(name)
                    if self._symbols is not None else None
                )
                if sym is not None and isinstance(sym.type, c99_ast.Long):
                    return 2
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
            case tac_ast.Truncate(src=src, dst=dst):
                # The k-th byte of a 2-byte memory layout is at
                # base+k (little-endian), so the source's offset-0
                # byte is the low byte. A Long → Int narrowing just
                # moves that low byte and discards the high.
                src_op = translate_val(src)
                dst_op = translate_val(dst)
                return [asm_ast.Mov(src=_byte_at(src_op, 0), dst=dst_op)]
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
        Matches the mul8 / divmod8 runtime-helper convention so
        callers can capture a Long return the same way they capture
        a multi-result helper call."""
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
        arrives with A = low byte, X = high byte (matches the
        mul8/divmod8 helper convention).
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
                _require_byte_helper(size, "*")
                return _translate_ax_call(
                    src1_op, src2_op, dst_op, _MUL8, result_in_x=False,
                )
            case tac_ast.Divide():
                _require_byte_helper(size, "/")
                return _translate_ax_call(
                    src1_op, src2_op, dst_op, _DIVMOD8, result_in_x=False,
                )
            case tac_ast.Modulo():
                _require_byte_helper(size, "%")
                return _translate_ax_call(
                    src1_op, src2_op, dst_op, _DIVMOD8, result_in_x=True,
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
                _require_byte_helper(size, "<<")
                return _translate_ax_call(
                    src1_op, src2_op, dst_op, _SHL8, result_in_x=False,
                )
            case tac_ast.RightShift():
                _require_byte_helper(size, ">>")
                return _translate_ax_call(
                    src1_op, src2_op, dst_op, _ASR8, result_in_x=False,
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


def _require_byte_helper(size: int, op_name: str) -> None:
    """The runtime helpers (`mul8`, `divmod8`, `shl8`, `asr8`) are
    8-bit only. Using `*`, `/`, `%`, `<<`, or `>>` on Long operands
    would need 16-bit equivalents (mul16/divmod16/shl16/asr16) that
    aren't in this repo yet — raise here so the failure points at
    the source-level construct rather than at a confusing emit-time
    value-out-of-range."""
    if size != 1:
        raise NotImplementedError(
            f"binary `{op_name}` on long operands is not implemented "
            f"yet (would need a 16-bit runtime helper that isn't in "
            f"this repo)"
        )


def _translate_ax_call(
    src1_op: asm_ast.Type_operand,
    src2_op: asm_ast.Type_operand,
    dst_op: asm_ast.Type_operand,
    helper: str,
    result_in_x: bool,
) -> list[asm_ast.Type_instruction]:
    """Lower a TAC op that delegates to a runtime helper taking A and X.
    src2 is staged through A (the only register the emitter can load
    from a Frame/Stack/Imm uniformly) into X, then src1 is loaded into
    A last so A holds the primary operand at the call. If the
    helper's result comes back in X (Modulo), transfer it to A before
    storing to dst."""
    out: list[asm_ast.Type_instruction] = [
        asm_ast.Mov(src=src2_op, dst=_REG_A),
        asm_ast.Mov(src=_REG_A, dst=_REG_X),
        asm_ast.Mov(src=src1_op, dst=_REG_A),
        asm_ast.Call(name=helper),
    ]
    if result_in_x:
        out.append(asm_ast.Mov(src=_REG_X, dst=_REG_A))
    out.append(asm_ast.Mov(src=_REG_A, dst=dst_op))
    return out


def translate_val(val: tac_ast.Type_val) -> asm_ast.Type_operand:
    """Translate a TAC val to its asm operand form. Constants
    become `Imm(value)` (the value is an int byte for Int constants,
    or — for Long constants — a 2-byte value the caller will split
    into bytes via `_byte_at`). Vars become `Pseudo(name, offset=0)`;
    callers that need to address higher bytes of a multi-byte value
    bump the offset via `_byte_at`."""
    match val:
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
