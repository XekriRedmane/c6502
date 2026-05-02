"""Translate a c99_ast tree into a tac_ast tree (three-address code).

Every C99 expression becomes a tac_ast `val` (either a Constant or a Var
holding the result of an earlier instruction). Compound expressions get
flattened: nested operators materialize their intermediate results into
fresh Var-typed temporaries and emit the corresponding TAC instruction.

The TAC program shape was widened: `tac_ast.Program(top_level*)`, where
`top_level` is `Function(name, is_global, params, instructions)` or
`StaticVariable(name, is_global, init)`. Function definitions come from
walking the c99 AST in source order; static variables come from the
symbol table after the AST walk. We deliberately *don't* emit any TAC
for the file-scope variable declarations or block-scope `extern` /
`static` variable declarations encountered during the AST walk — those
are objects with static storage duration whose initialization is
handled by `StaticVariable` entries enumerated from the symbol table.

State:
  - Translator owns the temporary-name counter (`%0`, `%1`, ...) and a
    separate label counter (`and_false@0`, `and_end@0`, ...) for the
    short-circuit lowerings.
  - The per-function instruction list is passed down explicitly as an
    argument so there's no implicit "current function" on the instance.
  - The symbol table from `passes.type_checking` is held as
    `self._symbols`. It feeds two distinct uses: (a) reading
    `FunAttr.is_global` when constructing each TAC `Function`, and
    (b) iterating every `StaticAttr` entry at the end to emit the
    corresponding TAC `StaticVariable` (or to skip the entry if its
    `initial_value` is `NoInitializer` — that's a reference to
    a definition that lives elsewhere).

Mapping:
  C99 Program(fn)             -> TAC Program(translate_function(fn))
  C99 Function(name, body)    -> TAC Function(name, <instrs built from
                                 each block_item in order>); if the
                                 body doesn't already end in a Ret,
                                 append `Ret(Constant(0))` (C99
                                 §5.1.2.2.3 for main; we apply it
                                 generally so every function
                                 terminates).
  C99 S(stmt)                 -> dispatches to translate_statement
  C99 D(decl)                 -> dispatches to translate_declaration
  C99 Declaration(name, init) -> if init is None, emit nothing; else
                                 evaluate init then
                                 Copy(init_val, Var(name)) — same TAC
                                 as the assignment `name = init`. TAC
                                 has no separate notion of a declared-
                                 but-uninitialized variable; the var
                                 name appears the first time it's used.
  C99 Return(exp)             -> emit Ret(translate_exp(exp))
  C99 Expression(exp)         -> translate_exp(exp) for side effects;
                                 the returned val is discarded.
  C99 IfStmt(cond, then,      -> evaluate cond, JumpIfFalse around
        else_clause)             the then-branch (skip directly to
                                 if_end@N when there's no else;
                                 jump-around an else-branch with a
                                 Jump+Label pair when there is). All
                                 labels come from the shared label
                                 counter (`if_end@N`, `if_else@N`).
  C99 Goto(label)             -> tac Jump(label). The label name is
                                 the unique `.<funcname>@<label>`
                                 minted by label_resolution — a
                                 dasm-style local label, scoped to
                                 the SUBROUTINE the asm emits. The
                                 `@` separator (illegal in C
                                 identifiers) keeps it disjoint
                                 from translator-minted labels
                                 (`.<prefix>_<N>`).
  C99 LabeledStmt(label, stmt) -> emit tac Label(label), then lower
                                 the inner statement. Label name is
                                 already unique (see Goto).
  C99 BreakStmt(label)        -> tac Jump(<label>_break). The
                                 incoming `label` is the base name
                                 (`.loop@<N>`) attached by
                                 loop_labeling; we derive the per-
                                 loop sub-targets by suffix.
  C99 ContinueStmt(label)     -> tac Jump(<label>_continue).
  C99 WhileStmt(cond, body,   -> Label(<continue>); <eval cond -> v>;
                label)           JumpIfFalse(v, <break>); <lower body>;
                                 Jump(<continue>); Label(<break>). The
                                 continue target is at the top of
                                 the loop (re-tests the condition);
                                 the break target sits after.
  C99 DoWhileStmt(body, cond, -> Label(<start>); <lower body>;
                  label)         Label(<continue>); <eval cond -> v>;
                                 JumpIfTrue(v, <start>); Label(<break>).
                                 The continue target sits between the
                                 body and the test, so `continue` re-
                                 runs the condition.
  C99 ForStmt(init, cond,     -> <init insns>; Label(<start>);
              post, body,        <eval cond -> v>;  -- omitted if cond
              label)             JumpIfFalse(v, <break>); -- is None
                                 <lower body>; Label(<continue>);
                                 <post insns>; -- omitted if post is None
                                 Jump(<start>); Label(<break>). The
                                 init runs once, then a test-body-
                                 post cycle. `continue` jumps to the
                                 post step (so it still runs); a
                                 missing condition is treated as
                                 unconditionally true so the test
                                 and its JumpIfFalse drop out.
  C99 InitDecl(decl)          -> same as a top-level Declaration
                                 (Copy of the initializer into the
                                 var; nothing for a bare `int x;`).
  C99 InitExp(exp)            -> evaluate `exp` for its side effects;
                                 result is discarded. Empty
                                 `InitExp(None)` lowers to nothing.
  C99 Compound(block)         -> lower each block item in order;
                                 no extra TAC structure (TAC is
                                 flat — block boundaries don't
                                 survive into the IR).
  C99 Null                    -> emit nothing
  C99 Constant(v)             -> TAC Constant(v)
  C99 Unary(op, inner)        -> emit Unary(op', translate(inner), Var(t))
                                 and return Var(t), where t is a fresh temp
  C99 Binary(op, left, right) -> emit Binary(op', translate(left),
                                 translate(right), Var(t))
                                 and return Var(t); left is translated
                                 before right so any temps it needs are
                                 numbered first.
  C99 Var(name)               -> TAC Var(name) — passthrough. The name
                                 is the unique `@N.orig` minted by
                                 identifier_resolution; it shares a
                                 namespace with TAC temps `%n` but
                                 can't collide because `@` and `%` are
                                 both illegal in C identifiers.
  C99 Assignment(Var(v), rval) -> emit translate(rval) -> rval_val,
                                 then Copy(rval_val, Var(v)); return
                                 Var(v) so chained assignments
                                 (`b = a = 5`) compose correctly. lval
                                 must be a Var (identifier_resolution
                                 enforces this; we double-check at
                                 runtime).
  C99 Postfix(op, Var(v))     -> emit Copy(Var(v), %old) to capture
                                 the operand's value before mutation,
                                 then Binary(Add/Subtract, Var(v),
                                 Constant(1), %new) to compute the
                                 updated value, then Copy(%new,
                                 Var(v)) to store it back. Returns
                                 Var(%old) so callers see the *old*
                                 value (postfix semantics) — distinct
                                 from prefix `++a`/`--a`, which the
                                 parser desugars to `a = a ± 1` and
                                 returns the *new* value via the
                                 Assignment branch.
  C99 Negate / Complement /   -> TAC Negate / Complement / LogicalNot
    LogicalNot
  C99 Add / Subtract /        -> TAC Add / Subtract / Multiply / Divide
    Multiply / Divide /          / Modulo / BitwiseAnd / BitwiseOr /
    Modulo / BitwiseAnd /        BitwiseXor / LeftShift / RightShift /
    BitwiseOr / BitwiseXor /     Equal / NotEqual / LessThan /
    LeftShift / RightShift /     GreaterThan / LessOrEqual /
    Equal / NotEqual /           GreaterOrEqual
    LessThan / GreaterThan /
    LessOrEqual / GreaterOrEqual

  C99 Conditional(cond, t, f) -> like an if/else that also produces a
                                 value: evaluate cond, JumpIfFalse to
                                 cond_else@N, evaluate t and Copy into
                                 a fresh dst temp, Jump(cond_end@N),
                                 Label(cond_else@N), evaluate f and
                                 Copy into the same dst, Label(
                                 cond_end@N). Returns dst. Labels come
                                 from the shared label counter
                                 (`cond_else@N`/`cond_end@N`), so each
                                 ternary gets globally unique numbers.

Short-circuit lowerings (no corresponding TAC binary op — the control
flow *is* the semantics):
  C99 Binary(LogicalAnd, L, R):
      <eval L -> src1>
      JumpIfFalse(src1, and_false@N)
      <eval R -> src2>
      JumpIfFalse(src2, and_false@N)
      Copy(Constant(1), result)
      Jump(and_end@N)
      Label(and_false@N)
      Copy(Constant(0), result)
      Label(and_end@N)
  C99 Binary(LogicalOr, L, R): symmetric, with JumpIfTrue / or_true@N /
      or_end@N and the 0/1 constants swapped. Each use of && or || gets
      a fresh N so nested short-circuits don't collide.
"""

from __future__ import annotations

import c99_ast
import fp_arith
import tac_ast
from passes.type_checking import (
    AddressInit,
    FunAttr,
    Initial,
    LocalAttr,
    NoInitializer,
    StaticAttr,
    Symbol,
    SymbolTable,
    Tentative,
)


# Per-loop sub-label derivation. The loop_labeling pass stamps each
# loop with a base label like `.loop@3`; the TAC lowering needs three
# distinct targets for that loop (start, continue target, break
# target), so we suffix the base. The base contains `@` (illegal in
# any C identifier), so neither it nor its suffixed forms can collide
# with a user-mangled label. They're also disjoint from every other
# translator-minted label (`.if_end@<N>`, `.cond_else@<N>`, …): those
# differ in prefix, and they end at the digit run after `@` rather
# than in a `_start`/`_continue`/`_break` suffix.
def _start_label(loop_label: str) -> str:
    return f"{loop_label}_start"


def _continue_label(loop_label: str) -> str:
    return f"{loop_label}_continue"


def _break_label(loop_label: str) -> str:
    return f"{loop_label}_break"


# ---------------------------------------------------------------------------
# c99 → TAC type / const translation
# ---------------------------------------------------------------------------
#
# The c99 and TAC ASDLs declare parallel `data_type` sums (Int /
# Long / UInt / ULong / FunType), so translating data_type is a
# one-to-one rewrap. The `const` sum is narrower in TAC (only
# ConstInt / ConstLong — the 6502 doesn't care about signedness at
# the byte level, so unsigned values pass through the signed
# variant of the matching width). The `static_init` sum, on the
# other hand, is the same shape on both sides (IntInit / LongInit /
# UIntInit / ULongInit) — codegen uses the variant to pick the dasm
# directive (DC.B / DC.W) and to track the declared type for
# debug / linker purposes.

def _byte_width_of(t: c99_ast.Type_data_type) -> int:
    """Byte width of an object type. Char / SChar / UChar / Int /
    UInt = 1, Long / ULong = 2, LongLong / ULongLong = 4, Float =
    4, Double = 8, Pointer = 2 (the 6502's address width). Used by
    Cast lowering to decide between SignExtend / ZeroExtend /
    Truncate / no-op (for integer types) and by various size-
    driven dispatch sites downstream."""
    if isinstance(t, (
        c99_ast.Int, c99_ast.UInt,
        c99_ast.Char, c99_ast.SChar, c99_ast.UChar,
    )):
        return 1
    if isinstance(t, (c99_ast.Long, c99_ast.ULong)):
        return 2
    if isinstance(t, (c99_ast.LongLong, c99_ast.ULongLong)):
        return 4
    if isinstance(t, c99_ast.Float):
        return 4
    if isinstance(t, c99_ast.Double):
        return 8
    if isinstance(t, c99_ast.Pointer):
        return 2
    raise TypeError(f"_byte_width_of: not an object type: {t!r}")


def _to_tac_data_type(t: c99_ast.Type_data_type) -> tac_ast.Type_data_type:
    """Translate a c99 data_type to its TAC counterpart. The TAC
    type sum has no Pointer variant — at the byte level, a 2-byte
    address is indistinguishable from a 2-byte integer on the 6502
    — so Pointer collapses onto `Long` (same width, same byte
    semantics). The c99 symbol table still carries Pointer for
    later passes that care (cast dispatch, dereference / address-of
    lowering when those land), but downstream TAC ops just see a
    2-byte unsigned-ish value."""
    if isinstance(t, c99_ast.Int):
        return tac_ast.Int()
    if isinstance(t, c99_ast.Long):
        return tac_ast.Long()
    if isinstance(t, c99_ast.LongLong):
        return tac_ast.LongLong()
    if isinstance(t, c99_ast.UInt):
        return tac_ast.UInt()
    if isinstance(t, c99_ast.ULong):
        return tac_ast.ULong()
    if isinstance(t, c99_ast.ULongLong):
        return tac_ast.ULongLong()
    if isinstance(t, (c99_ast.Char, c99_ast.SChar)):
        # Char / SChar collapse onto TAC Int for size purposes —
        # they share a 1-byte width and signed semantics. Codegen
        # consults the c99 symbol table directly when it needs to
        # distinguish them (e.g. integer-promotion target picks).
        return tac_ast.Int()
    if isinstance(t, c99_ast.UChar):
        # UChar collapses onto TAC UInt — 1-byte unsigned.
        return tac_ast.UInt()
    if isinstance(t, c99_ast.Float):
        return tac_ast.Float()
    if isinstance(t, c99_ast.Double):
        return tac_ast.Double()
    if isinstance(t, c99_ast.Void):
        return tac_ast.Void()
    if isinstance(t, c99_ast.Pointer):
        return tac_ast.Long()
    if isinstance(t, c99_ast.Array):
        # Arrays decay to pointers everywhere they're used as values,
        # so a TAC `Var` with array c99 type would only show up as the
        # operand of a `GetAddress` — which doesn't dispatch on its
        # operand's TAC type. Collapse to Long for consistency with
        # Pointer; the actual byte width of the storage is computed
        # by `_size_of_name` in `replace_pseudoregisters` (which reads
        # the c99 Array type directly).
        return tac_ast.Long()
    if isinstance(t, (c99_ast.Structure, c99_ast.Union)):
        # Struct / union types don't have a TAC counterpart — at the
        # byte level they're just N contiguous bytes that the asm
        # backend lays out via the symbol-table-driven
        # `_size_of`/`_size_of_name` helpers. The TAC sum's `Long`
        # is a stand-in for "address-shaped value" which is the
        # right shape for any Var-of-struct that gets address-taken
        # (the value itself isn't operated on as a TAC scalar).
        return tac_ast.Long()
    if isinstance(t, c99_ast.FunType):
        return tac_ast.FunType(
            params=[_to_tac_data_type(p) for p in t.params],
            ret=_to_tac_data_type(t.ret),
        )
    raise TypeError(f"unexpected c99 data_type: {t!r}")


def _to_tac_const(c: c99_ast.Type_const) -> tac_ast.Type_const:
    """Translate a c99 const to its TAC counterpart. TAC carries the
    full integer kind (width × signedness) on each variant, so the
    mapping is 1-to-1 on the integer side — c99 ConstUInt → TAC
    ConstUInt, etc. The 1-byte char variants (ConstChar / ConstUChar)
    collapse onto TAC's 1-byte int variants per C99 §6.3.1.1.2 (char
    types integer-promote to int / unsigned int): ConstChar onto
    ConstInt (plain `char` is signed in c6502), ConstUChar onto
    ConstUInt. FP variants ride through 1-to-1."""
    if isinstance(c, c99_ast.ConstInt):
        return tac_ast.ConstInt(value=c.value)
    if isinstance(c, c99_ast.ConstLong):
        return tac_ast.ConstLong(value=c.value)
    if isinstance(c, c99_ast.ConstLongLong):
        return tac_ast.ConstLongLong(value=c.value)
    if isinstance(c, c99_ast.ConstUInt):
        return tac_ast.ConstUInt(value=c.value)
    if isinstance(c, c99_ast.ConstULong):
        return tac_ast.ConstULong(value=c.value)
    if isinstance(c, c99_ast.ConstULongLong):
        return tac_ast.ConstULongLong(value=c.value)
    if isinstance(c, c99_ast.ConstChar):
        # Plain `char` / `signed char` → 1-byte signed int per C99
        # §6.3.1.1.2 integer promotion (char types promote to int).
        return tac_ast.ConstInt(value=c.value)
    if isinstance(c, c99_ast.ConstUChar):
        # `unsigned char` → 1-byte unsigned int via the same
        # promotion rule (§6.3.1.1.2: when int can't represent the
        # source's range, promote to unsigned int).
        return tac_ast.ConstUInt(value=c.value)
    if isinstance(c, c99_ast.ConstFloat):
        return tac_ast.ConstFloat(bits=c.bits)
    if isinstance(c, c99_ast.ConstDouble):
        return tac_ast.ConstDouble(bits=c.bits)
    raise TypeError(f"unexpected c99 const: {c!r}")


def _tac_const_for(t: c99_ast.Type_data_type, value: int | float) -> tac_ast.Type_const:
    """Build a TAC const matching `t`'s width and signedness (and,
    for FP, its precision). Pointer collapses onto ConstULong (same
    2-byte width as ULong; addresses are 16-bit unsigned). Used by
    the synthetic-constant call sites (postfix `+1`, short-circuit
    0/1, implicit `return 0`)."""
    if isinstance(t, (c99_ast.Int, c99_ast.Char, c99_ast.SChar)):
        return tac_ast.ConstInt(value=int(value))
    if isinstance(t, (c99_ast.UInt, c99_ast.UChar)):
        return tac_ast.ConstUInt(value=int(value))
    if isinstance(t, c99_ast.Long):
        return tac_ast.ConstLong(value=int(value))
    if isinstance(t, (c99_ast.ULong, c99_ast.Pointer)):
        return tac_ast.ConstULong(value=int(value))
    if isinstance(t, c99_ast.LongLong):
        return tac_ast.ConstLongLong(value=int(value))
    if isinstance(t, c99_ast.ULongLong):
        return tac_ast.ConstULongLong(value=int(value))
    if isinstance(t, c99_ast.Float):
        return tac_ast.ConstFloat(bits=fp_arith.int_to_single_bits(int(value)))
    if isinstance(t, c99_ast.Double):
        return tac_ast.ConstDouble(bits=fp_arith.int_to_double_bits(int(value)))
    raise TypeError(
        f"cannot build a TAC const for non-object type {t!r}"
    )


def _tac_const_val(t: c99_ast.Type_data_type, value: int | float) -> tac_ast.Constant:
    """Convenience: build a TAC `Constant(const=...)` val typed by
    `t`. The result is a `Type_val` ready to drop into a TAC
    instruction's src / dst slot."""
    return tac_ast.Constant(const=_tac_const_for(t, value))


def _fold_fp_cast_constant(
    target: c99_ast.Type_data_type,
    c: tac_ast.Type_const,
) -> tac_ast.Type_const:
    """Compile-time fold of a Cast whose operand is a Constant and
    whose source-or-target is FP. Returns a TAC const of `target`'s
    type. The operand's signedness is encoded in its TAC variant
    (Const{Int,Long,LongLong} → signed, Const{UInt,ULong,ULongLong}
    → unsigned), so we can pick the right interpretation locally:
    an unsigned variant masks the value back to its non-negative
    bit pattern (e.g. ConstUInt(-1) → 255) before converting; signed
    variants pass through. FP sources use their bit pattern via
    `fp_arith` to avoid Python float intermediaries. C99 §6.3.1.4
    requires FP→integer truncation toward zero, which
    `single_bits_to_int` / `double_bits_to_int` provide."""
    # FP source: bits → target via fp_arith.
    if isinstance(c, tac_ast.ConstFloat):
        if isinstance(target, c99_ast.Float):
            return tac_ast.ConstFloat(bits=c.bits)
        if isinstance(target, c99_ast.Double):
            return tac_ast.ConstDouble(
                bits=fp_arith.single_bits_to_double_bits(c.bits),
            )
        return _tac_const_for(target, fp_arith.single_bits_to_int(c.bits))
    if isinstance(c, tac_ast.ConstDouble):
        if isinstance(target, c99_ast.Double):
            return tac_ast.ConstDouble(bits=c.bits)
        if isinstance(target, c99_ast.Float):
            return tac_ast.ConstFloat(
                bits=fp_arith.double_bits_to_single_bits(c.bits),
            )
        return _tac_const_for(target, fp_arith.double_bits_to_int(c.bits))
    # Integer source: signedness rides on the variant. For unsigned
    # variants, mask to the source's bit-pattern range so a negative
    # TAC int (canonicalized at the variant's width) becomes its
    # non-negative twin before being handed to fp_arith's int → FP
    # conversion via _tac_const_for.
    if isinstance(c, tac_ast.ConstInt):
        v = c.value
    elif isinstance(c, tac_ast.ConstUInt):
        v = c.value & 0xFF
    elif isinstance(c, tac_ast.ConstLong):
        v = c.value
    elif isinstance(c, tac_ast.ConstULong):
        v = c.value & 0xFFFF
    elif isinstance(c, tac_ast.ConstLongLong):
        v = c.value
    elif isinstance(c, tac_ast.ConstULongLong):
        v = c.value & 0xFFFFFFFF
    else:
        raise TypeError(f"unexpected TAC const: {c!r}")
    return _tac_const_for(target, v)


# Width / signedness predicates for the declared-type modular
# truncation applied to static-storage integer initializers.
# The runtime cast lowering in tac_to_asm collapses to width-
# modular arithmetic for compile-time-known integer values; for
# static initializers we have to fold that here so the resulting
# byte pattern fits the cell. Mirrors
# `passes.type_checking._coerce_int_to_type` but kept local so
# c99_to_tac doesn't pull in type_checking just for this.
_TRUNC_BITS = {
    c99_ast.Int:       8,  c99_ast.UInt:      8,
    c99_ast.Char:      8,  c99_ast.SChar:     8,  c99_ast.UChar: 8,
    c99_ast.Long:     16,  c99_ast.ULong:    16,
    c99_ast.LongLong: 32,  c99_ast.ULongLong: 32,
}


def _truncate_int_for_static(t, value):
    """Reduce `value` to fit the declared integer type's width as
    a non-negative bit pattern (the form `IntInit` / `LongInit` /
    `LongLongInit` carry through asm_emit's `dc.b` / `dc.w` /
    `dc.l` directives). Negative values wrap to their two's-
    complement bit pattern at the matching width. Mirrors
    C99 §6.3.1.3 for assignment of an integer constant to a
    narrower integer type."""
    bits = _TRUNC_BITS[type(t)]
    return int(value) & ((1 << bits) - 1)


def _tac_static_init_for(
    t: c99_ast.Type_data_type,
    value: int | float | AddressInit,
) -> tac_ast.Type_static_init:
    """Build a TAC `static_init` wrapping `value`, with the variant
    matching the declared type — the integer side of the TAC
    `static_init` sum keeps signedness alongside width (unlike
    `const`, where signed and unsigned collapse), and the FP side
    keeps Float / Double distinct because their IEEE 754 byte
    patterns differ. Used both for explicit `Initial(c)`
    initializers and for tentative definitions resolved to zero of
    the declared type at end-of-TU. The value is coerced to the
    matching Python type — `int(value)` for integer variants,
    `float(value)` for FP variants — so an integer initializer for
    an FP static (e.g. `double x = 3;`) lays down `3.0` and an FP
    initializer for an integer static (after `_convert_to` wraps it
    in a Cast) lays down its truncated integer.

    AddressInit values (`&otherstatic` initializers) only make
    sense for Pointer-typed statics. The type checker has already
    validated this at the source-level construct, so an
    AddressInit here against a non-Pointer declared type is a
    bug — raise."""
    if isinstance(value, AddressInit):
        if not isinstance(t, c99_ast.Pointer):
            raise TypeError(
                f"AddressInit value can only initialize a pointer-"
                f"typed static; got declared type {t!r}"
            )
        return tac_ast.AddressInit(name=value.name, offset=value.offset)
    if isinstance(t, c99_ast.Int):
        return tac_ast.IntInit(value=_truncate_int_for_static(t, value))
    if isinstance(t, (c99_ast.Char, c99_ast.SChar)):
        # Char / SChar are 1-byte signed, same byte width and
        # signedness as Int — collapse onto IntInit so asm_emit
        # renders a single `dc.b $XX`.
        return tac_ast.IntInit(value=_truncate_int_for_static(t, value))
    if isinstance(t, c99_ast.UChar):
        # UChar is 1-byte unsigned, same byte width as UInt.
        return tac_ast.UIntInit(value=_truncate_int_for_static(t, value))
    if isinstance(t, c99_ast.Long):
        return tac_ast.LongInit(value=_truncate_int_for_static(t, value))
    if isinstance(t, c99_ast.LongLong):
        return tac_ast.LongLongInit(value=_truncate_int_for_static(t, value))
    if isinstance(t, c99_ast.UInt):
        return tac_ast.UIntInit(value=_truncate_int_for_static(t, value))
    if isinstance(t, c99_ast.ULong):
        return tac_ast.ULongInit(value=_truncate_int_for_static(t, value))
    if isinstance(t, c99_ast.ULongLong):
        return tac_ast.ULongLongInit(value=_truncate_int_for_static(t, value))
    if isinstance(t, c99_ast.Float):
        # `value` is already the IEEE 754 single bit pattern: the
        # type checker's `_const_init_value` coerces FP-typed
        # initializers to their target type's natural form, so by
        # the time we get here, an integer literal initializing
        # this static (`float x = 3;`) has already been routed
        # through `fp_arith.int_to_single_bits` upstream.
        return tac_ast.FloatInit(bits=int(value))
    if isinstance(t, c99_ast.Double):
        return tac_ast.DoubleInit(bits=int(value))
    if isinstance(t, c99_ast.Pointer):
        # Pointer collapses onto Long for static-init purposes —
        # addresses are 2-byte values written as a little-endian
        # 16-bit integer (e.g. NULL = 0x0000).
        return tac_ast.LongInit(
            value=_truncate_int_for_static(c99_ast.Long(), value),
        )
    raise TypeError(
        f"static-storage object can't have non-object type {t!r}"
    )


def _zero_init_value(t: c99_ast.Type_data_type, types=None):
    """Default-zero value tree for a Tentative file-scope static. A
    scalar yields `0` (or `0.0` for FP); an array yields a tuple of
    typed zeros sized to the array (recursive for multi-dim); a
    struct/union yields a tuple of typed zeros sized to the layout's
    member list. The type checker uses the same shape for `static T
    x;` (no init), so `_flat_static_init` consumes both via the
    same code path."""
    if isinstance(t, c99_ast.Array):
        return tuple(
            _zero_init_value(t.element_type, types) for _ in range(t.size)
        )
    if isinstance(t, (c99_ast.Structure, c99_ast.Union)):
        if types is None:
            return ()
        layout = types.get(t.tag)
        if layout is None or not layout.complete:
            return ()
        if isinstance(t, c99_ast.Union):
            if not layout.members:
                return ()
            return (_zero_init_value(layout.members[0].type, types),)
        return tuple(
            _zero_init_value(m.type, types) for m in layout.members
        )
    if isinstance(t, (c99_ast.Float, c99_ast.Double)):
        # IEEE 754 +0.0 (single or double) has all-zero bits, so
        # the same `0` is the right representation for both.
        return 0
    return 0


def _flat_static_init(
    t: c99_ast.Type_data_type,
    value,
    types=None,
) -> list[tac_ast.Type_static_init]:
    """Lay out a static-storage value tree as a flat list of TAC
    `static_init` items in source-byte order. Scalars produce a
    single-element list (`_tac_static_init_for` picks the variant
    by type); arrays recursively flatten — each element of the
    array's `value` tuple is laid out at its position, and the
    flattening is row-major for multi-dim arrays. The list shape
    mirrors the in-memory byte layout: each item describes how
    many bytes go down at the next slot. After flattening,
    consecutive zero-valued typed items are coalesced into a
    single `ZeroInit(N)` so missing-initializer zero-padding
    (C99 §6.7.8.21) and no-init statics (§6.7.8.10) lay down as
    a `DS.B N` directive instead of N separate `DC.B $00`s."""
    return _coalesce_zero_inits(_flat_static_init_raw(t, value, types))


def _flat_static_init_raw(
    t: c99_ast.Type_data_type,
    value,
    types=None,
) -> list[tac_ast.Type_static_init]:
    if isinstance(t, c99_ast.Array):
        if not isinstance(value, tuple) or len(value) != t.size:
            raise TypeError(
                f"array static init shape mismatch: expected tuple of "
                f"size {t.size} for {t!r}, got {value!r}"
            )
        # Char-element arrays (any nesting depth) collapse onto a
        # single `StringInit(str, bytes=N)` rather than per-byte
        # `IntInit` items. The two encodings are byte-identical at
        # the storage level — `dc.b $61, $62, $63` and a single
        # StringInit-rendered `dc.b $61, $62, $63` lay down the
        # same memory image — but the StringInit form is more
        # compact in the listing, especially for long strings.
        # Also handle the all-zeros case as a `ZeroInit(N)` so the
        # asm renders a single `ds.b N` (more compact than
        # `dc.b $00, $00, ...`).
        if _is_char_element(t.element_type):
            s = "".join(chr(int(b) & 0xFF) for b in value)
            if all(c == "\0" for c in s):
                return [tac_ast.ZeroInit(bytes=t.size)]
            return [tac_ast.StringInit(str=s, bytes=t.size)]
        out: list[tac_ast.Type_static_init] = []
        for elem in value:
            out.extend(_flat_static_init_raw(t.element_type, elem, types))
        return out
    if isinstance(t, (c99_ast.Structure, c99_ast.Union)):
        if types is None:
            return [tac_ast.ZeroInit(bytes=0)]
        layout = types.get(t.tag)
        if layout is None or not layout.complete:
            return [tac_ast.ZeroInit(bytes=0)]
        members = (
            layout.members[:1]
            if isinstance(t, c99_ast.Union)
            else layout.members
        )
        out: list[tac_ast.Type_static_init] = []
        for i, m in enumerate(members):
            elem_value = (
                value[i] if isinstance(value, tuple) and i < len(value)
                else _zero_init_value(m.type, types)
            )
            out.extend(_flat_static_init_raw(m.type, elem_value, types))
        # For unions, pad the layout's storage out to its full size
        # if the first member is smaller than the union (e.g. union
        # u { int a; long b; } x = {1}; — first member is 1 byte,
        # union is 2 bytes → tail-pad 1 zero byte).
        if isinstance(t, c99_ast.Union) and layout.size > 0:
            laid_down = sum(_static_init_byte_count(it) for it in out)
            if laid_down < layout.size:
                out.append(tac_ast.ZeroInit(bytes=layout.size - laid_down))
        return out
    return [_tac_static_init_for(t, value)]


def _static_init_byte_count(it) -> int:
    """How many bytes does `it` lay down in memory? Used to pad
    union-typed statics whose first-named-member width is smaller
    than the union's total size."""
    if isinstance(it, tac_ast.IntInit):
        return 1
    if isinstance(it, tac_ast.UIntInit):
        return 1
    if isinstance(it, tac_ast.LongInit):
        return 2
    if isinstance(it, tac_ast.ULongInit):
        return 2
    if isinstance(it, tac_ast.LongLongInit):
        return 4
    if isinstance(it, tac_ast.ULongLongInit):
        return 4
    if isinstance(it, tac_ast.FloatInit):
        return 4
    if isinstance(it, tac_ast.DoubleInit):
        return 8
    if isinstance(it, tac_ast.AddressInit):
        return 2
    if isinstance(it, tac_ast.ZeroInit):
        return it.bytes
    if isinstance(it, tac_ast.StringInit):
        return it.bytes
    raise TypeError(f"unknown static_init: {it!r}")


def _zero_byte_count(item: tac_ast.Type_static_init) -> int | None:
    """Byte count of `item` if its in-memory bytes are all zero,
    else None. Integer-zero items (Int / UInt / Long / ULong) and
    `+0.0` FP items (`-0.0` isn't representable in c6502 statics —
    the parser routes negation through `Unary`, which the constant-
    expression check rejects) qualify. AddressInit never qualifies
    — `&name` is symbolic, resolved by the assembler at link time
    to an address that may or may not be zero."""
    if isinstance(item, tac_ast.IntInit) and item.value == 0:
        return 1
    if isinstance(item, tac_ast.UIntInit) and item.value == 0:
        return 1
    if isinstance(item, tac_ast.LongInit) and item.value == 0:
        return 2
    if isinstance(item, tac_ast.ULongInit) and item.value == 0:
        return 2
    if isinstance(item, tac_ast.LongLongInit) and item.value == 0:
        return 4
    if isinstance(item, tac_ast.ULongLongInit) and item.value == 0:
        return 4
    if isinstance(item, tac_ast.FloatInit) and item.bits == 0:
        return 4
    if isinstance(item, tac_ast.DoubleInit) and item.bits == 0:
        return 8
    if isinstance(item, tac_ast.ZeroInit):
        return item.bytes
    return None


def _coalesce_zero_inits(
    items: list[tac_ast.Type_static_init],
) -> list[tac_ast.Type_static_init]:
    """Merge runs of zero-valued items into single `ZeroInit(N)`
    instructions. The merged byte count is the sum of each run's
    member sizes (1 for IntInit, 2 for LongInit, …)."""
    out: list[tac_ast.Type_static_init] = []
    pending = 0
    for item in items:
        b = _zero_byte_count(item)
        if b is not None:
            pending += b
            continue
        if pending > 0:
            out.append(tac_ast.ZeroInit(bytes=pending))
            pending = 0
        out.append(item)
    if pending > 0:
        out.append(tac_ast.ZeroInit(bytes=pending))
    return out


def _pointee_size(t: c99_ast.Type_data_type, types=None) -> int:
    """Bytes per element for `*ptr` where `ptr` has type `t`. Used to
    scale the integer operand in pointer arithmetic — `ptr + n`
    advances by `n * _pointee_size(ptr)` bytes per C99 §6.5.6.8.
    The widths match the rest of the c6502 type model: Int/UInt = 1,
    Long/ULong = 2, Pointer = 2 (the 6502's address width), Float = 4,
    Double = 8, Array = elem_size * count (so a `(int (*)[10]) + 1`
    advances by 10 bytes — needed once multi-dim arrays land).
    Function pointers and non-object types are rejected by the type
    checker before this point."""
    assert isinstance(t, c99_ast.Pointer), f"not a pointer: {t!r}"
    return _sizeof(t.referenced_type, types)


def _is_char_element(t: c99_ast.Type_data_type) -> bool:
    """True iff `t` is one of the three char element types (Char /
    SChar / UChar). Mirrors `passes.type_checking._is_char_element`
    but kept local so c99_to_tac doesn't pull in type_checking
    just for this predicate."""
    return isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar))


def _sizeof(t: c99_ast.Type_data_type, types=None) -> int:
    """Bytes occupied by a value of type `t` in c6502's storage
    model. Recursive for Array — `int[3][4]` is 12 bytes,
    `char[10]` is 10 bytes. For Structure / Union, looks up the
    tag's layout in `types` (the program-wide TypeTable) and reads
    its `.size`."""
    if isinstance(t, (
        c99_ast.Int, c99_ast.UInt,
        c99_ast.Char, c99_ast.SChar, c99_ast.UChar,
    )):
        return 1
    if isinstance(t, (c99_ast.Long, c99_ast.ULong, c99_ast.Pointer)):
        return 2
    if isinstance(t, (c99_ast.LongLong, c99_ast.ULongLong)):
        return 4
    if isinstance(t, c99_ast.Float):
        return 4
    if isinstance(t, c99_ast.Double):
        return 8
    if isinstance(t, c99_ast.Array):
        return _sizeof(t.element_type, types) * t.size
    if isinstance(t, (c99_ast.Structure, c99_ast.Union)):
        if types is None:
            raise TypeError(
                f"cannot size struct/union without TypeTable: {t!r}"
            )
        layout = types.get(t.tag)
        if layout is None or not layout.complete:
            raise TypeError(
                f"cannot size incomplete struct/union {t!r}"
            )
        return layout.size
    raise TypeError(f"cannot size {t!r}")


class Translator:
    def __init__(
        self,
        symbols: SymbolTable | None = None,
        types=None,
    ) -> None:
        self._temp_counter = 0
        self._label_counter = 0
        # Read-only handle to the type checker's struct/union layout
        # table. Used by `_sizeof` / `_pointee_size` to size
        # struct-typed locals, members, and pointee scaling.
        self._types = types
        # Read-only handle to the type-checker's symbol table. Used
        # twice: to set `is_global` on each TAC Function (lookup keyed
        # by source name, since FunAttr names aren't renamed by
        # identifier_resolution) and to iterate StaticAttr entries
        # at the end of `translate_program` for StaticVariable
        # emission.
        #
        # The optional default exists so that unit tests of internal
        # translation methods (`translate_exp`, `translate_statement`,
        # …) that don't construct Function nodes can build a
        # Translator without first running type-checking. Any test
        # that exercises `translate_program` or `_translate_function`
        # must pass a real symbol table — those paths read FunAttr.
        self._symbols = symbols if symbols is not None else SymbolTable()
        # Set by `_translate_function` while walking a struct-returning
        # function's body — the resolved name of the hidden sret param
        # (a `Pointer(struct)` holding the caller's return slot
        # address). Read by the Return arm to redirect `return e;` to
        # a `Store(e, sret)` + `Ret(None)`.
        self._sret_param: str | None = None

    def make_temporary_variable_name(
        self, t: c99_ast.Type_data_type | None = None,
    ) -> str:
        """Mint a fresh temporary variable name `%N` and register it
        in the symbol table as a `LocalAttr` automatic-storage
        object. Each temporary holds the result of an expression, so
        its type is the surrounding expression's `data_type` — the
        caller passes that in so codegen can size each temp's
        frame slot correctly.

        The optional default of `None` is a backstop for unit tests
        that exercise the bare counter without going through
        type-checking; the temp registers as `Int` in that case.
        Production callers — `translate_exp` for every kind of
        compound expression — always pass an explicit type.
        """
        name = f"%{self._temp_counter}"
        self._temp_counter += 1
        self._symbols[name] = Symbol(
            type=t if t is not None else c99_ast.Int(),
            attrs=LocalAttr(),
        )
        return name

    def make_label(self, prefix: str) -> str:
        # Leading `.` makes this a dasm-style local label — scoped to
        # the enclosing SUBROUTINE, so labels in different functions
        # don't collide in the global asm namespace. The `@`
        # separator (illegal in any C identifier) means a translator-
        # minted label can never be confused with anything the user
        # could write: user goto labels are mangled to
        # `.<funcname>@<orig>` where the part after `@` is a C
        # identifier; here the part after `@` is digits.
        name = f".{prefix}@{self._label_counter}"
        self._label_counter += 1
        return name

    def translate_program(self, prog: c99_ast.Type_program) -> tac_ast.Type_program:
        # Two passes assemble the TAC program's top-level list:
        # (1) Walk c99 declarations in source order. Each
        #     FunctionDecl with a body lowers to a TAC Function;
        #     every other top-level c99 declaration (forward
        #     function declarations and file-scope variable
        #     declarations) emits nothing here. The static-storage
        #     objects appear in the next pass.
        # (2) Iterate the symbol table once and emit a TAC
        #     StaticVariable for each StaticAttr entry whose
        #     initial value is concrete (Initial(c) or Tentative —
        #     the latter resolved to 0 per C99 §6.9.2.2).
        #     NoInitializer entries are pure references to a
        #     definition elsewhere; they emit nothing.
        # The two passes can't be folded because the symbol table
        # is what tells us each static-storage object's resolved
        # initial value (after merging across redeclarations) and
        # `is_global` flag — neither is locally available at any
        # one declaration site.
        match prog:
            case c99_ast.Program(declaration=decls):
                top_levels: list[tac_ast.Type_top_level] = []
                for d in decls:
                    fn = self._translate_top_level_declaration(d)
                    if fn is not None:
                        top_levels.append(fn)
                top_levels.extend(self._emit_static_variables())
                return tac_ast.Program(top_level=top_levels)
        raise TypeError(f"unexpected program: {prog!r}")

    def _translate_top_level_declaration(
        self, decl: c99_ast.Type_declaration,
    ) -> tac_ast.Type_top_level | None:
        # File-scope declarations: function definitions become TAC
        # Functions; everything else (forward function decls, all
        # variable decls) is consumed by the symbol-table pass.
        match decl:
            case c99_ast.FunctionDecl(function_decl=fd):
                if fd.body is None:
                    return None
                return self._translate_function(fd)
            case c99_ast.VarDecl():
                # File-scope variable declarations don't generate TAC
                # at the AST-walk stage. Their definitions appear via
                # the symbol-table pass below.
                return None
            case c99_ast.StructDecl():
                # Struct/union declarations are pure type info —
                # already consumed by the type checker. No TAC.
                return None
        raise TypeError(f"unexpected declaration: {decl!r}")

    def translate_function(
        self, fn: c99_ast.Type_function_definition,
    ) -> tac_ast.Function:
        """Test-convenience entry point. The c99 AST no longer holds
        function definitions in the legacy `Function(name, params,
        body)` shape — they live inside `FunctionDecl(function_decl=
        Type_function_decl(...))`. Tests still find it convenient to
        build a free-standing `Function` and translate it, so this
        method accepts the legacy shape, lifts it into a
        `Type_function_decl`, and dispatches to `_translate_function`.
        Production code reaches `_translate_function` directly via
        `translate_program`."""
        match fn:
            case c99_ast.Function(name=name, params=params, body=body):
                ftype = c99_ast.FunType(
                    params=[c99_ast.Int() for _ in params],
                    ret=c99_ast.Int(),
                )
                fd = c99_ast.Type_function_decl(
                    name=name,
                    params=list(params),
                    body=body,
                    data_type=ftype,
                    storage_class=None,
                )
                return self._translate_function(fd)
        raise TypeError(f"unexpected function: {fn!r}")

    def _translate_function(
        self, fd: c99_ast.Type_function_decl,
    ) -> tac_ast.Function:
        assert fd.body is not None
        ret_type = (
            fd.data_type.ret
            if isinstance(fd.data_type, c99_ast.FunType)
            else c99_ast.Int()
        )
        # If this function returns a struct/union, mint a hidden
        # first parameter that holds the address of the caller's
        # return slot. The body's `return e;` lowers to
        # `Store(e, sret) + Ret(None)` (see `translate_statement`
        # Return arm). The sret param goes in front of the user
        # params at the TAC level — caller-side code in the
        # `FunctionCall` arm matches by prepending the slot's
        # address as the first arg.
        sret_param: str | None = None
        if isinstance(ret_type, (c99_ast.Structure, c99_ast.Union)):
            sret_param = f".sret.{fd.name}"
            self._symbols[sret_param] = Symbol(
                type=c99_ast.Pointer(referenced_type=ret_type),
                attrs=LocalAttr(),
            )
        prior_sret = self._sret_param
        self._sret_param = sret_param
        try:
            instrs: list[tac_ast.Type_instruction] = []
            self.translate_block(fd.body, instrs)
        finally:
            self._sret_param = prior_sret
        # If the body didn't end in a Return, fall off the end
        # with an implicit `return 0`. C99 §5.1.2.2.3 specifies
        # this for `main`; we apply it generally so every TAC
        # function is guaranteed to terminate with a Ret —
        # control falling off the end of any TAC function
        # would be undefined, and the implicit zero-return
        # papers over execution paths that forgot a `return`.
        # The constant's variant matches the function's declared
        # return type so a Long-returning function gets a
        # ConstLong(0) and an Int-returning one gets ConstInt(0).
        if not instrs or not isinstance(instrs[-1], tac_ast.Ret):
            # Void return: emit Ret(val=None). The asm epilogue then
            # skips the value-into-A/X sequence and the
            # PHA/PLA-bracket. Falling off the end of a void function
            # is legal (C99 §6.9.1.12); for non-void functions it's
            # still the implicit-zero-return papering-over a missing
            # `return`. Struct-returning functions land here too —
            # the body's last `return` already wrote to *sret, so
            # falling off is also a Ret(None).
            if isinstance(ret_type, c99_ast.Void) or sret_param is not None:
                instrs.append(tac_ast.Ret(val=None))
            else:
                instrs.append(tac_ast.Ret(
                    val=_tac_const_val(ret_type, 0),
                ))
        # `is_global` rides through from the symbol table. Function
        # names aren't renamed by identifier_resolution (linkage
        # forces the source spelling), so the lookup key matches
        # `fd.name` directly. If the symbol table is empty (a unit-
        # test convenience — see `Translator.__init__`), default to
        # `is_global=True`, which matches the linkage of any function
        # without an explicit `static` specifier.
        sym = self._symbols.get(fd.name)
        if sym is not None and isinstance(sym.attrs, FunAttr):
            is_global = sym.attrs.is_global
        else:
            is_global = True
        # Parameter names ride through to TAC verbatim — they were
        # already renamed to `@<N>.<orig>` by identifier resolution,
        # and TAC `Var(@<N>.<orig>)` references in the body see the
        # same names. For struct-returning functions, the hidden
        # sret param goes first.
        params = list(fd.params)
        if sret_param is not None:
            params = [sret_param] + params
        return tac_ast.Function(
            name=fd.name,
            is_global=is_global,
            params=params,
            instructions=instrs,
        )

    def _emit_static_variables(self) -> list[tac_ast.StaticVariable]:
        # Walk the symbol table in insertion order (which matches
        # source order for file-scope decls) and emit a TAC
        # StaticVariable for every StaticAttr with a concrete initial
        # value. The initial value flattens to a list of typed
        # `IntInit(...)` / `LongInit(...)` / etc. items in source-
        # byte order. Scalar statics produce a single-element list;
        # array statics produce one entry per array slot (multi-dim
        # arrays flatten row-major).
        # NoInitializer entries are pure references — the
        # definition is somewhere else and emits its own
        # StaticVariable, or it's an external dependency the linker
        # resolves. C99 §6.9.2.2: a Tentative definition that wasn't
        # upgraded by an explicit Initial somewhere in the TU resolves
        # to a zero-initialized definition at end-of-TU; we emit that
        # zero through the same typed-zero machinery as a `static T
        # x;` with no init (handled by the type checker, which gives
        # us a pre-zeroed value tree of the right shape).
        out: list[tac_ast.StaticVariable] = []
        for name, sym in self._symbols.items():
            if not isinstance(sym.attrs, StaticAttr):
                continue
            init = sym.attrs.initial_value
            if isinstance(init, Initial):
                init_value = init.value
            elif isinstance(init, Tentative):
                init_value = _zero_init_value(sym.type, self._types)
            elif isinstance(init, NoInitializer):
                continue
            else:
                raise TypeError(f"unexpected initial value: {init!r}")
            data_type = _to_tac_data_type(sym.type)
            out.append(tac_ast.StaticVariable(
                name=name,
                is_global=sym.attrs.is_global,
                data_type=data_type,
                init=_flat_static_init(sym.type, init_value, self._types),
            ))
        return out

    def translate_block(
        self,
        block: c99_ast.Type_block,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        match block:
            case c99_ast.Block(block_item=items):
                for item in items:
                    self.translate_block_item(item, instrs)
                return
        raise TypeError(f"unexpected block: {block!r}")

    def translate_block_item(
        self,
        item: c99_ast.Type_block_item,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        match item:
            case c99_ast.S(statement=stmt):
                self.translate_statement(stmt, instrs)
                return
            case c99_ast.D(declaration=decl):
                self.translate_declaration(decl, instrs)
                return
        raise TypeError(f"unexpected block item: {item!r}")

    def translate_declaration(
        self,
        decl: c99_ast.Type_declaration,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        # TAC has no "declare" instruction — automatic-storage
        # variables are introduced by their first appearance. So a
        # bare `int x;` lowers to nothing, and `int x = e;` lowers
        # exactly like the assignment `x = e`: evaluate the
        # initializer, then Copy into the var.
        #
        # Block-scope `static int x [= e];` and `extern int x;` are
        # objects with static storage duration — their definitions
        # appear in the program's StaticVariable list assembled from
        # the symbol table. They don't run any code at the
        # declaration's source location, so we drop them here. The
        # `storage_class is not None` check is sufficient to make the
        # split: identifier_resolution / type-check have already
        # rejected any block-scope storage-class specifier other than
        # `static` / `extern`.
        #
        # A FunctionDecl is purely a name-binding artifact (consumed
        # by identifier_resolution to validate calls); it has no
        # runtime effect, so it lowers to nothing.
        match decl:
            case c99_ast.VarDecl(var_decl=vd):
                if vd.storage_class is not None:
                    return
                if vd.init is not None:
                    if isinstance(vd.init, c99_ast.InitList):
                        # Dispatch by the variable's declared type.
                        if isinstance(vd.data_type, c99_ast.Array):
                            self._translate_array_init_list(vd, instrs)
                        elif isinstance(
                            vd.data_type,
                            (c99_ast.Structure, c99_ast.Union),
                        ):
                            self._translate_struct_init_list(
                                vd, instrs,
                            )
                        else:
                            raise TypeError(
                                f"InitList for non-aggregate type "
                                f"{vd.data_type!r}"
                            )
                    elif isinstance(vd.init, c99_ast.String):
                        # `char arr[N] = "abc";` at block scope —
                        # lay each byte (plus null + zero-pad) into
                        # the array's storage via the same address-
                        # arithmetic path used for InitList.
                        self._translate_string_array_init(vd, instrs)
                    else:
                        # Plain initializer — including struct = expr
                        # copies. The Copy lowering reads the dst's
                        # symbol-table size, so a struct-typed Var on
                        # both sides fans out to N byte-copies.
                        init_val = self.translate_exp(vd.init, instrs)
                        instrs.append(tac_ast.Copy(
                            src=init_val, dst=tac_ast.Var(name=vd.name),
                        ))
                return
            case c99_ast.FunctionDecl() | c99_ast.StructDecl():
                return
        raise TypeError(f"unexpected declaration: {decl!r}")

    def translate_statement(
        self,
        stmt: c99_ast.Type_statement,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        match stmt:
            case c99_ast.Return(exp=exp):
                # `return;` (no value) lowers to Ret(val=None) — the
                # asm epilogue skips the value-into-A/X step. `return
                # e;` evaluates `e` and passes its val to Ret. Type
                # checking has already enforced that the bare form
                # only appears in void-returning functions.
                #
                # Struct/union return: the function's TAC signature
                # has a hidden `.sret.<name>` first parameter holding
                # the caller's return-slot address. We Store the
                # struct value through that pointer, then emit
                # Ret(None) — no scalar return value.
                if exp is None:
                    instrs.append(tac_ast.Ret(val=None))
                elif self._sret_param is not None:
                    val = self.translate_exp(exp, instrs)
                    instrs.append(tac_ast.Store(
                        src=val,
                        dst_ptr=tac_ast.Var(name=self._sret_param),
                    ))
                    instrs.append(tac_ast.Ret(val=None))
                else:
                    instrs.append(tac_ast.Ret(
                        val=self.translate_exp(exp, instrs),
                    ))
                return
            case c99_ast.Expression(exp=exp):
                # Translate for side effects (assignments today; calls
                # later). Whatever val the expression returns goes
                # unused — the result-temp it points at is just dead.
                self.translate_exp(exp, instrs)
                return
            case c99_ast.IfStmt(
                condition=cond, then_clause=then_stmt, else_clause=else_stmt,
            ):
                # `if (cond) then` lowers to:
                #   <eval cond -> cond_val>
                #   JumpIfFalse(cond_val, end_N)
                #   <lower then>
                #   Label(end_N)
                # With an else-branch, an extra Jump and Label split
                # the two arms:
                #   <eval cond -> cond_val>
                #   JumpIfFalse(cond_val, else_N)
                #   <lower then>
                #   Jump(end_N)
                #   Label(else_N)
                #   <lower else>
                #   Label(end_N)
                # Labels share the same counter the short-circuit
                # lowerings use, so each `if` gets globally unique
                # `if_else@N`/`if_end@N` numbers.
                cond_val = self.translate_exp(cond, instrs)
                end_label = self.make_label("if_end")
                if else_stmt is None:
                    instrs.append(tac_ast.JumpIfFalse(
                        condition=cond_val, target=end_label,
                    ))
                    self.translate_statement(then_stmt, instrs)
                    instrs.append(tac_ast.Label(name=end_label))
                else:
                    else_label = self.make_label("if_else")
                    instrs.append(tac_ast.JumpIfFalse(
                        condition=cond_val, target=else_label,
                    ))
                    self.translate_statement(then_stmt, instrs)
                    instrs.append(tac_ast.Jump(target=end_label))
                    instrs.append(tac_ast.Label(name=else_label))
                    self.translate_statement(else_stmt, instrs)
                    instrs.append(tac_ast.Label(name=end_label))
                return
            case c99_ast.Compound(block=block):
                # `{ ... }` — TAC is flat, so a compound statement
                # is just its block items lowered in order. Scope is
                # already gone by this point (identifier_resolution
                # rewrote every name to its globally-unique form), so
                # there's nothing left for `{ ... }` to mean at the
                # IR level. The grammar doesn't yet have a
                # `compound_stmt` rule, so this only fires when an
                # AST is built directly; the lowering is the same
                # either way.
                self.translate_block(block, instrs)
                return
            case c99_ast.Goto(label=label):
                # `goto label;` lowers to an unconditional Jump. The
                # target name is the unique `.<funcname>@<label>`
                # minted by label_resolution — a dasm local label
                # (leading dot scopes it to the enclosing SUBROUTINE).
                # The `@` separator (illegal in a C identifier) keeps
                # these disjoint from translator-minted labels like
                # `.if_end@N` — they share the @-marker convention,
                # but the part after `@` is a C identifier here vs.
                # a digit run there.
                instrs.append(tac_ast.Jump(target=label))
                return
            case c99_ast.LabeledStmt(label=label, statement=inner):
                # `label: stmt` lowers to a TAC Label followed by the
                # inner statement's own lowering. The label name is
                # already the unique `.<funcname>@<label>` from
                # label_resolution.
                instrs.append(tac_ast.Label(name=label))
                self.translate_statement(inner, instrs)
                return
            case c99_ast.BreakStmt(label=label):
                # `break;` lowers to an unconditional jump to the
                # break-target label of the enclosing loop. The loop
                # label is the base name (e.g. `.loop@3`) minted by
                # the loop_labeling pass; we derive the per-loop
                # break/continue/start targets from it by suffix.
                instrs.append(tac_ast.Jump(target=_break_label(label)))
                return
            case c99_ast.ContinueStmt(label=label):
                instrs.append(tac_ast.Jump(target=_continue_label(label)))
                return
            case c99_ast.WhileStmt(condition=cond, body=body, label=label):
                # while: test-then-body, with the continue target at
                # the top of the loop (re-tests the condition) and the
                # break target after the loop.
                #   Label(<continue>)
                #   <eval cond -> v>
                #   JumpIfFalse(v, <break>)
                #   <lower body>
                #   Jump(<continue>)
                #   Label(<break>)
                cont = _continue_label(label)
                brk = _break_label(label)
                instrs.append(tac_ast.Label(name=cont))
                cond_val = self.translate_exp(cond, instrs)
                instrs.append(tac_ast.JumpIfFalse(
                    condition=cond_val, target=brk,
                ))
                self.translate_statement(body, instrs)
                instrs.append(tac_ast.Jump(target=cont))
                instrs.append(tac_ast.Label(name=brk))
                return
            case c99_ast.DoWhileStmt(body=body, condition=cond, label=label):
                # do-while: body-then-test. The continue target sits
                # *between* the body and the condition test (so
                # `continue` re-runs the test), and the break target
                # sits after everything.
                #   Label(<start>)
                #   <lower body>
                #   Label(<continue>)
                #   <eval cond -> v>
                #   JumpIfTrue(v, <start>)
                #   Label(<break>)
                start = _start_label(label)
                cont = _continue_label(label)
                brk = _break_label(label)
                instrs.append(tac_ast.Label(name=start))
                self.translate_statement(body, instrs)
                instrs.append(tac_ast.Label(name=cont))
                cond_val = self.translate_exp(cond, instrs)
                instrs.append(tac_ast.JumpIfTrue(
                    condition=cond_val, target=start,
                ))
                instrs.append(tac_ast.Label(name=brk))
                return
            case c99_ast.ForStmt(
                init=init, condition=cond, post_clause=post,
                body=body, label=label,
            ):
                # for: init, then test-body-post, with the continue
                # target between the body and the post-iteration step
                # (so `continue` skips the rest of the body but still
                # runs the post step), and the break target after the
                # loop. A missing condition is treated as
                # unconditionally true — we just skip the
                # JumpIfFalse, since there's nothing to test.
                #   <init insns>
                #   Label(<start>)
                #   <eval cond -> v>          (omitted if cond is None)
                #   JumpIfFalse(v, <break>)   (omitted if cond is None)
                #   <lower body>
                #   Label(<continue>)
                #   <post insns>              (omitted if post is None)
                #   Jump(<start>)
                #   Label(<break>)
                start = _start_label(label)
                cont = _continue_label(label)
                brk = _break_label(label)
                self.translate_for_init(init, instrs)
                instrs.append(tac_ast.Label(name=start))
                if cond is not None:
                    cond_val = self.translate_exp(cond, instrs)
                    instrs.append(tac_ast.JumpIfFalse(
                        condition=cond_val, target=brk,
                    ))
                self.translate_statement(body, instrs)
                instrs.append(tac_ast.Label(name=cont))
                if post is not None:
                    # Post-clause is an expression evaluated for its
                    # side effects (the result value is discarded).
                    self.translate_exp(post, instrs)
                instrs.append(tac_ast.Jump(target=start))
                instrs.append(tac_ast.Label(name=brk))
                return
            case c99_ast.SwitchStmt(
                control=control, body=body, label=label,
                cases=cases, default_label=default_label,
                promoted_type=promoted_type,
            ):
                # Switch dispatch: evaluate the control once, then a
                # compare-and-conditional-jump per case, then an
                # unconditional jump to the default (or to the break
                # label if no default), then the body which contains
                # the case / default labels inline. C99 §6.8.4.2.4 —
                # cases fall through unless `break` is hit; the body
                # walk emits a Label for each case/default node it
                # encounters, exactly where they appear in source.
                #
                # Layout:
                #   <eval control -> t>
                #   for each (case_value, case_label):
                #     Binary(Equal, t, case_const, eq_temp)
                #     JumpIfTrue(eq_temp, case_label)
                #   Jump(default_label or <break>)
                #   <lower body>            (emits Label(case/default))
                #   Label(<break>)
                #
                # The break label uses the same `_break` suffix
                # convention as iteration statements so a `break;`
                # inside the switch body — already stamped with the
                # switch's base label by the loop-labeling pass —
                # lowers to Jump(<base>_break) via the regular
                # BreakStmt path.
                brk = _break_label(label)
                t_val = self.translate_exp(control, instrs)
                for case in cases:
                    # The type checker canonicalised every
                    # case.value to a Constant of the promoted type,
                    # so a single translate_exp gets a TAC val of
                    # the matching width.
                    case_val = self.translate_exp(case.value, instrs)
                    eq_temp = tac_ast.Var(
                        name=self.make_temporary_variable_name(
                            c99_ast.Int(),
                        ),
                    )
                    instrs.append(tac_ast.Binary(
                        op=tac_ast.Equal(),
                        src1=t_val,
                        src2=case_val,
                        dst=eq_temp,
                    ))
                    instrs.append(tac_ast.JumpIfTrue(
                        condition=eq_temp, target=case.label,
                    ))
                # No case matched: fall through to default if there
                # is one, else jump past the body.
                instrs.append(tac_ast.Jump(
                    target=default_label if default_label is not None else brk,
                ))
                self.translate_statement(body, instrs)
                instrs.append(tac_ast.Label(name=brk))
                return
            case c99_ast.CaseStmt(body=body, label=label):
                # The dispatch chain emitted at the SwitchStmt above
                # already targeted `label`; here we just plant the
                # label and recurse into the inner statement. The
                # case's `value` was already canonicalised by the
                # type checker and consumed by the dispatch chain —
                # nothing to emit at the case site itself.
                instrs.append(tac_ast.Label(name=label))
                self.translate_statement(body, instrs)
                return
            case c99_ast.DefaultStmt(body=body, label=label):
                instrs.append(tac_ast.Label(name=label))
                self.translate_statement(body, instrs)
                return
            case c99_ast.Null():
                # No-op statement. Nothing to emit.
                return
        raise TypeError(f"unexpected statement: {stmt!r}")

    def translate_for_init(
        self,
        init: c99_ast.Type_for_init,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        # For-init runs once before the loop body. A declaration
        # lowers exactly like a top-level declaration (Copy of init
        # value into the var, or nothing for a bare `int x;`); an
        # expression-init runs the expression for side effects with
        # the result thrown away. An empty `for (;;)` lowers to no
        # init instructions.
        match init:
            case c99_ast.InitDecl(var_decl=vd):
                # for-init is restricted to variable declarations
                # (C99 §6.8.5), so we lower the var_decl directly
                # rather than going through the wider declaration
                # dispatcher. Same dispatch as block-scope vars:
                # InitList for array/struct/union, String for char-
                # array, plain expression otherwise (including
                # struct-copy init).
                if vd.init is not None:
                    if isinstance(vd.init, c99_ast.InitList):
                        if isinstance(vd.data_type, c99_ast.Array):
                            self._translate_array_init_list(vd, instrs)
                        elif isinstance(
                            vd.data_type,
                            (c99_ast.Structure, c99_ast.Union),
                        ):
                            self._translate_struct_init_list(vd, instrs)
                        else:
                            raise TypeError(
                                f"InitList for non-aggregate type "
                                f"{vd.data_type!r}"
                            )
                    elif isinstance(vd.init, c99_ast.String):
                        self._translate_string_array_init(vd, instrs)
                    else:
                        init_val = self.translate_exp(vd.init, instrs)
                        instrs.append(tac_ast.Copy(
                            src=init_val, dst=tac_ast.Var(name=vd.name),
                        ))
                return
            case c99_ast.InitExp(exp=exp):
                if exp is not None:
                    self.translate_exp(exp, instrs)
                return
        raise TypeError(f"unexpected for_init: {init!r}")

    def translate_exp(
        self,
        exp: c99_ast.Type_exp,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val | None:
        match exp:
            case c99_ast.Constant(const=c):
                # The c99 and TAC const sums are 1-to-1; just rewrap
                # under the matching TAC variant. The asm backend
                # extracts the underlying int and feeds it to its
                # `Imm` operand, so a ConstLong value above the
                # 1-byte range will hit the deferred-codegen
                # boundary at asm emit's `_check_byte`.
                return tac_ast.Constant(const=_to_tac_const(c))
            case c99_ast.Cast(target_type=target, exp=inner):
                # `(void)e` (cast-to-void): evaluate `e` for its side
                # effects, return None — the value is discarded. Any
                # caller using this result would be a void value in a
                # non-void context, which the type checker already
                # forbade.
                if isinstance(target, c99_ast.Void):
                    self.translate_exp(inner, instrs)
                    return None
                # Lower `Cast` based on the source/target c99 types.
                # The 6502 has no signedness distinction, so cross-sign
                # integer casts at the same width are no-ops; FP types
                # need explicit conversion nodes because their bit
                # patterns aren't compatible with integers (or with
                # each other across precisions):
                #   src == target                         → no-op
                #   integer same width (Int↔UInt, Long↔ULong,
                #                       LongLong↔ULongLong)
                #                                         → no-op
                #   integer narrower → wider, signed src  → SignExtend
                #   integer narrower → wider, unsigned src → ZeroExtend
                #   integer wider → narrower (any signedness) → Truncate
                #   integer → Float / Double              → IntToFloat /
                #                                           IntToDouble
                #   Float / Double → integer              → FloatToInt /
                #                                           DoubleToInt
                #   Float ↔ Double                        → FloatToDouble /
                #                                           DoubleToFloat
                # SignExtend / ZeroExtend / Truncate read the source
                # and destination widths from the symbol table at
                # tac_to_asm time, so the same TAC nodes cover every
                # 1B/2B/4B widening or narrowing.
                # The source type comes from the inner node's
                # `data_type`, set by the type checker. If it's
                # None (synthetic AST that bypassed type-checking —
                # e.g. a unit test of Cast lowering on its own),
                # fall back to the no-op path so the test stays
                # focused on the structural translation.
                inner_val = self.translate_exp(inner, instrs)
                source = inner.data_type
                if source is None or source == target:
                    return inner_val
                src_fp = isinstance(
                    source, (c99_ast.Float, c99_ast.Double),
                )
                tgt_fp = isinstance(
                    target, (c99_ast.Float, c99_ast.Double),
                )
                # The temp holds the casted value — its type is the
                # cast's target.
                dst = tac_ast.Var(
                    name=self.make_temporary_variable_name(target),
                )
                if src_fp or tgt_fp:
                    # FP-involving cast. If the source is a compile-
                    # time Constant, fold it in Python here — that
                    # avoids a runtime helper call. Signedness rides
                    # on the operand's TAC variant, so the fold needs
                    # only the target type to pick the result variant.
                    if isinstance(inner_val, tac_ast.Constant):
                        return tac_ast.Constant(
                            const=_fold_fp_cast_constant(
                                target, inner_val.const,
                            ),
                        )
                    # Otherwise the source is a Var — tac_to_asm picks
                    # the right helper (signed vs. unsigned, 1B vs. 2B
                    # for integer side; Float vs. Double for FP side)
                    # by looking up src/dst types in the symbol table.
                    if src_fp and tgt_fp:
                        # Float ↔ Double cross-precision (same-precision
                        # was caught by the source == target check).
                        node_cls = (
                            tac_ast.FloatToDouble
                            if isinstance(source, c99_ast.Float)
                            else tac_ast.DoubleToFloat
                        )
                    elif src_fp:
                        node_cls = (
                            tac_ast.FloatToInt
                            if isinstance(source, c99_ast.Float)
                            else tac_ast.DoubleToInt
                        )
                    else:
                        node_cls = (
                            tac_ast.IntToFloat
                            if isinstance(target, c99_ast.Float)
                            else tac_ast.IntToDouble
                        )
                    instrs.append(node_cls(src=inner_val, dst=dst))
                    return dst
                src_w = _byte_width_of(source)
                tgt_w = _byte_width_of(target)
                if src_w == tgt_w:
                    # Same width, different signedness — bit pattern
                    # is identical, so the cast carries no codegen.
                    return inner_val
                if src_w < tgt_w:
                    if isinstance(
                        source,
                        (c99_ast.Int, c99_ast.Long, c99_ast.LongLong),
                    ):
                        instrs.append(tac_ast.SignExtend(
                            src=inner_val, dst=dst,
                        ))
                    else:
                        # Unsigned source → wider type: zero-fill
                        # the new high byte rather than replicating
                        # the sign bit.
                        instrs.append(tac_ast.ZeroExtend(
                            src=inner_val, dst=dst,
                        ))
                else:  # src_w > tgt_w
                    instrs.append(tac_ast.Truncate(
                        src=inner_val, dst=dst,
                    ))
                return dst
            case c99_ast.Unary(op=op, exp=inner):
                src = self.translate_exp(inner, instrs)
                # The temp's type is the Unary node's data_type
                # (set by the type checker — same as inner's type
                # for negate/complement, Int for logical-not).
                dst = tac_ast.Var(
                    name=self.make_temporary_variable_name(exp.data_type),
                )
                instrs.append(tac_ast.Unary(
                    op=self.translate_unop(op),
                    src=src,
                    dst=dst,
                ))
                return dst
            case c99_ast.Binary(op=c99_ast.LogicalAnd(), left=left, right=right):
                return self.translate_short_circuit(
                    left, right, instrs,
                    short_circuit_on_true=False,
                )
            case c99_ast.Binary(op=c99_ast.LogicalOr(), left=left, right=right):
                return self.translate_short_circuit(
                    left, right, instrs,
                    short_circuit_on_true=True,
                )
            case c99_ast.Binary(op=op, left=left, right=right):
                # Translate left first so its temps get the lower
                # numbers — matches a left-to-right evaluation order
                # readers will expect.
                src1 = self.translate_exp(left, instrs)
                src2 = self.translate_exp(right, instrs)
                # Pointer arithmetic (C99 §6.5.6) is the only case
                # where an integer Binary needs more than a single
                # TAC op: the integer operand has to be scaled by
                # sizeof(pointee) before the add/sub, and ptr - ptr
                # produces a byte-difference that has to be divided
                # back down to an element count. Everything else
                # falls through to the plain Binary lowering.
                lt, rt = left.data_type, right.data_type
                l_ptr = isinstance(lt, c99_ast.Pointer)
                r_ptr = isinstance(rt, c99_ast.Pointer)
                if (
                    isinstance(op, (c99_ast.Add, c99_ast.Subtract))
                    and (l_ptr or r_ptr)
                ):
                    return self.translate_pointer_arithmetic(
                        op, src1, src2, lt, rt, exp.data_type, instrs,
                    )
                # The Binary's data_type (set by the type checker)
                # is the result type after usual arithmetic
                # conversions — the common type for arithmetic /
                # bitwise / shift, Int for comparisons.
                dst = tac_ast.Var(
                    name=self.make_temporary_variable_name(exp.data_type),
                )
                instrs.append(tac_ast.Binary(
                    op=self.translate_binop(op),
                    src1=src1,
                    src2=src2,
                    dst=dst,
                ))
                return dst
            case c99_ast.Var(name=name):
                # Resolved name from identifier_resolution (e.g. `@0.x`)
                # passes straight through into TAC's Var namespace —
                # `@` and TAC's `%` are both illegal in C identifiers,
                # so user vars and translator temps can't collide.
                return tac_ast.Var(name=name)
            case c99_ast.SizeOfExp(exp=inner):
                # `sizeof e` — fold to a compile-time constant.
                # Crucially, do NOT call `translate_exp(inner, instrs)`:
                # C99 §6.5.3.4.2 says sizeof's operand is not
                # evaluated, so we mustn't emit any instructions for
                # it (no inc/dec side effects, no function calls,
                # no array writes). The type checker has already
                # stamped inner.data_type with the un-decayed
                # operand type, which is all we need to compute
                # the size. Result type is ULong (size_t in c6502).
                t = inner.data_type
                assert t is not None, (
                    "type_checker should have stamped data_type "
                    f"on sizeof's inner expression: {inner!r}"
                )
                return tac_ast.Constant(const=tac_ast.ConstULong(
                    value=_sizeof(t, self._types),
                ))
            case c99_ast.SizeOfType(target_type=t):
                # `sizeof (T)` — direct fold from the type-name.
                # No inner expression to translate.
                return tac_ast.Constant(const=tac_ast.ConstULong(
                    value=_sizeof(t, self._types),
                ))
            case c99_ast.Subscript(array=arr, index=idx):
                # `a[i]` per C99 §6.5.2.1.2 is `*(a + i)`. The type
                # checker has already decayed any array operand to a
                # pointer and widened the index to Long, so this
                # reuses the pointer-arithmetic lowering directly:
                # compute the byte address, then Load N bytes through
                # it into a fresh element-typed temp.
                addr = self._translate_subscript_address(
                    arr, idx, instrs,
                )
                dst = tac_ast.Var(
                    name=self.make_temporary_variable_name(exp.data_type),
                )
                instrs.append(tac_ast.Load(src_ptr=addr, dst=dst))
                return dst
            case c99_ast.Dot(operand=operand, member=member):
                # `e.m` — compute the byte address of the member,
                # then Load N bytes through it. The address path
                # mirrors the lvalue case (see `_translate_member_
                # address`).
                addr = self._translate_dot_address(
                    operand, member, instrs,
                )
                dst = tac_ast.Var(
                    name=self.make_temporary_variable_name(exp.data_type),
                )
                instrs.append(tac_ast.Load(src_ptr=addr, dst=dst))
                return dst
            case c99_ast.Arrow(operand=operand, member=member):
                # `p->m` — evaluate the pointer, add the member's
                # offset, Load.
                addr = self._translate_arrow_address(
                    operand, member, instrs,
                )
                dst = tac_ast.Var(
                    name=self.make_temporary_variable_name(exp.data_type),
                )
                instrs.append(tac_ast.Load(src_ptr=addr, dst=dst))
                return dst
            case c99_ast.Assignment(lval=lval, rval=rval):
                # identifier_resolution accepts five lval shapes:
                # `Var(name)` (named storage cell), `Dereference(p)`
                # (store through pointer), `Subscript(arr, idx)`
                # (array element), and `Dot` / `Arrow` (struct
                # member by value or pointer). Anything else gets
                # rejected upstream; the runtime fall-through here
                # is belt-and-braces in case a later refactor lets
                # a non-lvalue slip through.
                rval_val = self.translate_exp(rval, instrs)
                if isinstance(lval, c99_ast.Var):
                    dst = tac_ast.Var(name=lval.name)
                    instrs.append(tac_ast.Copy(src=rval_val, dst=dst))
                    # Return the lval so chained assignments compose:
                    # `b = a = 5` -> inner returns Var(@0.a), outer copies
                    # that into @1.b and returns Var(@1.b).
                    return dst
                if isinstance(lval, c99_ast.Dereference):
                    # `*p = rval` lowers to a Store: evaluate the
                    # pointer expression, then write the rval's bytes
                    # through the pointer. The result of the
                    # expression is the rval value itself (post-
                    # conversion via the type checker's _convert_to),
                    # so we return rval_val for the chained-assignment
                    # case `*q = *p = 5`.
                    ptr_val = self.translate_exp(lval.exp, instrs)
                    instrs.append(tac_ast.Store(
                        src=rval_val, dst_ptr=ptr_val,
                    ))
                    return rval_val
                if isinstance(lval, c99_ast.Subscript):
                    # `a[i] = rval` — same address computation as the
                    # rvalue Subscript, then Store instead of Load.
                    addr = self._translate_subscript_address(
                        lval.array, lval.index, instrs,
                    )
                    instrs.append(tac_ast.Store(
                        src=rval_val, dst_ptr=addr,
                    ))
                    return rval_val
                if isinstance(lval, c99_ast.Dot):
                    addr = self._translate_dot_address(
                        lval.operand, lval.member, instrs,
                    )
                    instrs.append(tac_ast.Store(
                        src=rval_val, dst_ptr=addr,
                    ))
                    return rval_val
                if isinstance(lval, c99_ast.Arrow):
                    addr = self._translate_arrow_address(
                        lval.operand, lval.member, instrs,
                    )
                    instrs.append(tac_ast.Store(
                        src=rval_val, dst_ptr=addr,
                    ))
                    return rval_val
                raise TypeError(
                    f"assignment lval must be Var, Dereference, "
                    f"Subscript, Dot, or Arrow (identifier_resolution "
                    f"should have enforced this); got {lval!r}"
                )
            case c99_ast.CompoundAssignment(
                op=op, lval=lval, rval=rval,
                intermediate_type=it,
            ):
                return self._translate_compound_assign(
                    op, lval, rval, it, instrs,
                )
            case c99_ast.Conditional(
                condition=cond,
                true_clause=true_clause,
                false_clause=false_clause,
            ):
                # `cond ? t : f` lowers like an if/else that also
                # produces a value: both arms Copy into a shared dst
                # temp so the result is a single Var the caller can
                # thread into later instructions. Labels come from the
                # same counter as `if`/short-circuit, so numbering stays
                # globally unique.
                #   <eval cond -> cond_val>
                #   JumpIfFalse(cond_val, cond_else@N)
                #   <eval true -> t_val>
                #   Copy(t_val, dst)         (skipped if void)
                #   Jump(cond_end@N)
                #   Label(cond_else@N)
                #   <eval false -> f_val>
                #   Copy(f_val, dst)         (skipped if void)
                #   Label(cond_end@N)
                # When both branches have void type (C99 §6.5.15.5),
                # the conditional has no value — skip the dst temp
                # and the per-branch Copies; just sequence the side
                # effects.
                cond_val = self.translate_exp(cond, instrs)
                else_label = self.make_label("cond_else")
                end_label = self.make_label("cond_end")
                is_void = isinstance(exp.data_type, c99_ast.Void)
                dst = (
                    None if is_void else tac_ast.Var(
                        name=self.make_temporary_variable_name(exp.data_type),
                    )
                )
                instrs.append(tac_ast.JumpIfFalse(
                    condition=cond_val, target=else_label,
                ))
                t_val = self.translate_exp(true_clause, instrs)
                if dst is not None:
                    instrs.append(tac_ast.Copy(src=t_val, dst=dst))
                instrs.append(tac_ast.Jump(target=end_label))
                instrs.append(tac_ast.Label(name=else_label))
                f_val = self.translate_exp(false_clause, instrs)
                if dst is not None:
                    instrs.append(tac_ast.Copy(src=f_val, dst=dst))
                instrs.append(tac_ast.Label(name=end_label))
                return dst
            case c99_ast.FunctionCall(name=name, args=args):
                # `f(arg1, arg2, ...)` lowers to: evaluate each arg
                # in source order (so its temporaries get the lower
                # numbers), collect the resulting TAC vals, mint a
                # fresh dst temp for the return value, and emit
                # either a direct `FunctionCall(name, args, dst)`
                # (when `name` denotes a function — type checker
                # has stamped sym.type as FunType) or an indirect
                # `IndirectCall(ptr=Var(name), args, dst)` (when
                # `name` denotes a pointer-to-function — the type
                # checker accepts both shapes for the callee). The
                # arg-evaluation and return-temp logic is identical
                # either way.
                arg_vals = [
                    self.translate_exp(a, instrs) for a in args
                ]
                # Void-returning callees produce no value — emit
                # FunctionCall with dst=None. The TAC instruction's
                # asm lowering skips the return-value capture step.
                is_void = isinstance(exp.data_type, c99_ast.Void)
                # Struct/union return: c6502's calling convention is
                # sret — the caller allocates a return slot, passes
                # its address as the (hidden) first arg, and the
                # callee writes the bytes through that pointer. The
                # FunctionCall expression's "result" is the slot
                # itself (a struct-typed Var), which downstream
                # consumers (Assignment Copy, Dot, Arrow, …) treat
                # like any other addressable struct lvalue.
                ret_t = exp.data_type
                if isinstance(ret_t, (c99_ast.Structure, c99_ast.Union)):
                    slot_name = self.make_temporary_variable_name(ret_t)
                    slot = tac_ast.Var(name=slot_name)
                    addr = tac_ast.Var(
                        name=self.make_temporary_variable_name(
                            c99_ast.Pointer(referenced_type=ret_t),
                        ),
                    )
                    instrs.append(tac_ast.GetAddress(
                        operand=slot, dst=addr,
                    ))
                    arg_vals = [addr] + arg_vals
                    sym = self._symbols.get(name)
                    if sym is not None and isinstance(sym.type, c99_ast.Pointer):
                        instrs.append(tac_ast.IndirectCall(
                            ptr=tac_ast.Var(name=name),
                            args=arg_vals, dst=None,
                        ))
                    else:
                        instrs.append(tac_ast.FunctionCall(
                            name=name, args=arg_vals, dst=None,
                        ))
                    return slot
                dst = (
                    None if is_void else tac_ast.Var(
                        name=self.make_temporary_variable_name(exp.data_type),
                    )
                )
                sym = self._symbols.get(name)
                if sym is not None and isinstance(sym.type, c99_ast.Pointer):
                    # Indirect call — the callee is a function-
                    # pointer-typed Var. Pass the pointer val (which
                    # carries the function's address at runtime) to
                    # IndirectCall; tac_to_asm stages it into DPTR
                    # before JSR-ing the icall trampoline.
                    instrs.append(tac_ast.IndirectCall(
                        ptr=tac_ast.Var(name=name),
                        args=arg_vals, dst=dst,
                    ))
                else:
                    instrs.append(tac_ast.FunctionCall(
                        name=name, args=arg_vals, dst=dst,
                    ))
                return dst
            case c99_ast.Postfix(op=op, operand=operand):
                # `a++` / `a--` returns the *old* value of the operand
                # while incrementing/decrementing it.
                return self._translate_incdec(
                    op, operand, instrs, return_old=True,
                )
            case c99_ast.Prefix(op=op, operand=operand):
                # `++a` / `--a` returns the *new* value of the operand
                # after incrementing/decrementing it.
                return self._translate_incdec(
                    op, operand, instrs, return_old=False,
                )
            case c99_ast.Dereference(exp=inner):
                # `*p` (read context) — evaluate the pointer
                # expression, then Load N bytes through the
                # resulting pointer into a fresh pointee-typed temp.
                # The dst's type comes from the type checker (the
                # Dereference node's data_type is the pointee).
                #
                # The store-through-pointer case (`*p = rval`) is
                # handled in the Assignment case above; that path
                # never reaches this one because Assignment doesn't
                # call translate_exp on its lval.
                ptr_val = self.translate_exp(inner, instrs)
                dst = tac_ast.Var(
                    name=self.make_temporary_variable_name(exp.data_type),
                )
                instrs.append(tac_ast.Load(src_ptr=ptr_val, dst=dst))
                return dst
            case c99_ast.AddressOf(exp=inner):
                # `&e` — `e` is an lvalue (validated by
                # identifier_resolution; today: Var or Dereference).
                # The result is a pointer-typed temp, type stamped
                # by the type checker.
                if isinstance(inner, c99_ast.Var):
                    # `&x` — straightforward GetAddress on the
                    # named storage cell. Works uniformly for
                    # locals, params, and statics; the asm-side
                    # LoadAddress dispatches on storage class via
                    # the symbol table.
                    dst = tac_ast.Var(
                        name=self.make_temporary_variable_name(exp.data_type),
                    )
                    instrs.append(tac_ast.GetAddress(
                        operand=tac_ast.Var(name=inner.name), dst=dst,
                    ))
                    return dst
                if isinstance(inner, c99_ast.Dereference):
                    # `&*e` ≡ `e` per C99 §6.5.3.2.3 — neither `&`
                    # nor `*` is evaluated, the result is just `e`.
                    # Translate the pointer expression directly,
                    # skipping the Load that a bare `*e` would emit.
                    return self.translate_exp(inner.exp, instrs)
                if isinstance(inner, c99_ast.Subscript):
                    # `&a[i]` ≡ `a + i` per C99 §6.5.3.2.3 — same
                    # address arithmetic as the rvalue Subscript
                    # path, but skip the trailing Load. This shape
                    # only arrives synthetically from the type
                    # checker's array-decay path: `a[i]` for a
                    # multi-dim array `a` yields an Array-typed
                    # Subscript, which `_decay_if_array` then wraps
                    # in `AddressOf` to produce a pointer-to-element
                    # for the next outer Subscript / Binary etc.
                    # User-written `&a[i]` doesn't reach here —
                    # identifier_resolution rejects AddressOf
                    # operands that aren't Var or Dereference.
                    return self._translate_subscript_address(
                        inner.array, inner.index, instrs,
                    )
                if isinstance(inner, c99_ast.Dot):
                    # `&s.m` — same address arithmetic as the rvalue
                    # Dot, but skip the trailing Load.
                    return self._translate_dot_address(
                        inner.operand, inner.member, instrs,
                    )
                if isinstance(inner, c99_ast.Arrow):
                    # `&p->m` — same address arithmetic as the
                    # rvalue Arrow, but skip the trailing Load.
                    return self._translate_arrow_address(
                        inner.operand, inner.member, instrs,
                    )
                raise TypeError(
                    f"address-of operand must be Var, Dereference, "
                    f"Subscript, Dot, or Arrow (identifier_resolution "
                    f"/ array decay should have enforced this); got "
                    f"{inner!r}"
                )
        raise TypeError(f"unexpected exp: {exp!r}")

    def translate_pointer_arithmetic(
        self,
        op: c99_ast.Type_binary_operator,
        src1: tac_ast.Type_val,
        src2: tac_ast.Type_val,
        lt: c99_ast.Type_data_type,
        rt: c99_ast.Type_data_type,
        result_type: c99_ast.Type_data_type,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        """Lower pointer arithmetic to plain TAC ops. Three shapes,
        all dispatched on the operand types the type checker stamped:
          ptr ± int     — multiply the int by sizeof(pointee), then
                          a normal Add / Subtract on two 2-byte values.
                          Skip the multiply when sizeof(pointee) == 1.
          ptr - ptr     — Subtract the two 2-byte pointers, then divide
                          the byte-difference by sizeof(pointee) to get
                          an element count. Skip the divide when
                          sizeof(pointee) == 1. Result is Long.
        The type checker's already widened any int operand to Long, so
        every value reaching this method is 2 bytes wide and the
        arithmetic happens at one width."""
        l_ptr = isinstance(lt, c99_ast.Pointer)
        r_ptr = isinstance(rt, c99_ast.Pointer)
        if l_ptr and r_ptr:
            # ptr - ptr (the only legal two-pointer additive op; ptr +
            # ptr was rejected by the type checker).
            assert isinstance(op, c99_ast.Subtract)
            size = _pointee_size(lt, self._types)
            diff = tac_ast.Var(
                name=self.make_temporary_variable_name(c99_ast.Long()),
            )
            instrs.append(tac_ast.Binary(
                op=tac_ast.Subtract(), src1=src1, src2=src2, dst=diff,
            ))
            if size == 1:
                return diff
            quot = tac_ast.Var(
                name=self.make_temporary_variable_name(c99_ast.Long()),
            )
            instrs.append(tac_ast.Binary(
                op=tac_ast.Divide(),
                src1=diff,
                src2=_tac_const_val(c99_ast.Long(), size),
                dst=quot,
            ))
            return quot
        # ptr ± int. The type checker widened the int operand to Long
        # and rejected int - ptr, so the only remaining shapes are
        # ptr_lhs ± int_rhs and int_lhs + ptr_rhs.
        if l_ptr:
            ptr_val, int_val, ptr_type = src1, src2, lt
        else:
            ptr_val, int_val, ptr_type = src2, src1, rt
        size = _pointee_size(ptr_type, self._types)
        if size != 1:
            scaled = tac_ast.Var(
                name=self.make_temporary_variable_name(c99_ast.Long()),
            )
            instrs.append(tac_ast.Binary(
                op=tac_ast.Multiply(),
                src1=int_val,
                src2=_tac_const_val(c99_ast.Long(), size),
                dst=scaled,
            ))
            int_val = scaled
        dst = tac_ast.Var(
            name=self.make_temporary_variable_name(result_type),
        )
        # For Subtract the type checker rejected int - ptr, so
        # `ptr_val` is always the lhs. For Add the operation is
        # commutative and the order doesn't matter, but we keep the
        # pointer on the lhs for consistency.
        instrs.append(tac_ast.Binary(
            op=self.translate_binop(op),
            src1=ptr_val,
            src2=int_val,
            dst=dst,
        ))
        return dst

    def _translate_array_init_list(
        self,
        vd: c99_ast.Type_var_decl,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        """Lower `T arr[N] = {e1, e2, ...};` (block-scope only) to a
        sequence of leaf Stores. GetAddress once for the array's
        base, then walk the (possibly nested) initializer tree
        recursively, accumulating a constant byte offset to each
        scalar leaf and emitting `Store(val, base + offset)` for it.
        Missing items at any level zero-pad per C99 §6.7.8.21."""
        arr_type = vd.data_type
        assert isinstance(arr_type, c99_ast.Array)
        init = vd.init
        assert isinstance(init, c99_ast.InitList)
        # Single base address for the whole initializer tree; we
        # treat it as a byte-pointer (typed Pointer(Int) is fine —
        # the size dispatch on Store comes from the value's type,
        # not the pointer's).
        base = tac_ast.Var(
            name=self.make_temporary_variable_name(
                c99_ast.Pointer(referenced_type=c99_ast.Int()),
            ),
        )
        instrs.append(tac_ast.GetAddress(
            operand=tac_ast.Var(name=vd.name), dst=base,
        ))
        self._emit_init_stores(arr_type, init, base, 0, instrs)

    def _emit_string_stores(
        self,
        s_node: c99_ast.String,
        arr_type: c99_ast.Array,
        base: tac_ast.Type_val,
        base_offset: int,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        """Lay each byte of `s_node.str` (zero-pad to `arr_type.size`)
        down at `base + base_offset + k`. Used by the nested
        char-array case of `_emit_init_stores` when an inner
        `String` initializes a `char[N]` sub-aggregate (e.g.
        `signed char a[3][4] = {{...}, "efgh", "ijk"}`)."""
        n = arr_type.size
        s = s_node.str
        elem_type = arr_type.element_type
        for i in range(n):
            byte = ord(s[i]) & 0xFF if i < len(s) else 0
            val = _tac_const_val(elem_type, byte)
            offset = base_offset + i
            if offset == 0:
                addr = base
            else:
                addr = tac_ast.Var(
                    name=self.make_temporary_variable_name(
                        c99_ast.Pointer(referenced_type=elem_type),
                    ),
                )
                instrs.append(tac_ast.Binary(
                    op=tac_ast.Add(),
                    src1=base,
                    src2=_tac_const_val(c99_ast.Long(), offset),
                    dst=addr,
                ))
            instrs.append(tac_ast.Store(src=val, dst_ptr=addr))

    def _translate_string_array_init(
        self,
        vd: c99_ast.Type_var_decl,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        """`char arr[N] = "abc";` at block scope. Per C99 §6.7.8.14
        the bytes of the literal initialize the leading elements;
        any remaining elements are zero-initialized (which
        includes the null terminator when there's room — when
        `N == len`, the terminator is elided). Lay each byte down
        via the same `GetAddress` + `Store` pattern as the InitList
        path, with the constant value coming from the string."""
        arr_type = vd.data_type
        assert isinstance(arr_type, c99_ast.Array)
        assert isinstance(vd.init, c99_ast.String)
        n = arr_type.size
        s = vd.init.str
        elem_type = arr_type.element_type
        # Single base address for the array's storage. Pointer-to-
        # Int as the temp's c99 type (size dispatch on the Store
        # itself comes from the value's TAC type, which is Int
        # 1B for every byte we lay down).
        base = tac_ast.Var(
            name=self.make_temporary_variable_name(
                c99_ast.Pointer(referenced_type=c99_ast.Int()),
            ),
        )
        instrs.append(tac_ast.GetAddress(
            operand=tac_ast.Var(name=vd.name), dst=base,
        ))
        for i in range(n):
            byte = ord(s[i]) & 0xFF if i < len(s) else 0
            val = _tac_const_val(elem_type, byte)
            if i == 0:
                addr = base
            else:
                addr = tac_ast.Var(
                    name=self.make_temporary_variable_name(
                        c99_ast.Pointer(referenced_type=elem_type),
                    ),
                )
                instrs.append(tac_ast.Binary(
                    op=tac_ast.Add(),
                    src1=base,
                    src2=_tac_const_val(c99_ast.Long(), i),
                    dst=addr,
                ))
            instrs.append(tac_ast.Store(src=val, dst_ptr=addr))

    def _emit_init_stores(
        self,
        arr_type: c99_ast.Array,
        init: c99_ast.InitList,
        base: tac_ast.Type_val,
        base_offset: int,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        """Walk an (possibly nested) initializer for an array of
        type `arr_type`, emitting Store instructions for each scalar
        leaf at `base + (base_offset + leaf_offset)` bytes. Missing
        items zero-pad: a None item with an Array element type
        recurses with an empty InitList (so all leaves of the
        sub-array zero); a None item with a scalar element type
        emits `Store(0-of-elem, addr)`."""
        elem_type = arr_type.element_type
        elem_size = _sizeof(elem_type, self._types)
        for i in range(arr_type.size):
            byte_offset = base_offset + i * elem_size
            item = init.items[i] if i < len(init.items) else None
            if isinstance(elem_type, c99_ast.Array):
                # Recurse into the sub-array. A missing item is a
                # logically-empty InitList — every leaf zeroes.
                # A `String` item at a char-array sub-element type
                # is the §6.7.8.14 string-as-char-array form; emit
                # per-byte stores at this offset.
                if (
                    isinstance(item, c99_ast.String)
                    and _is_char_element(elem_type.element_type)
                ):
                    self._emit_string_stores(
                        item, elem_type, base, byte_offset, instrs,
                    )
                    continue
                sub = (
                    item if item is not None
                    else c99_ast.InitList(items=[], data_type=elem_type)
                )
                self._emit_init_stores(
                    elem_type, sub, base, byte_offset, instrs,
                )
            elif isinstance(elem_type, (c99_ast.Structure, c99_ast.Union)):
                sub = (
                    item if item is not None
                    else c99_ast.InitList(items=[], data_type=elem_type)
                )
                self._emit_struct_init_stores(
                    elem_type, sub, base, byte_offset, instrs,
                )
            else:
                if item is None:
                    val = _tac_const_val(elem_type, 0)
                else:
                    val = self.translate_exp(item, instrs)
                if byte_offset == 0:
                    addr = base
                else:
                    addr = tac_ast.Var(
                        name=self.make_temporary_variable_name(
                            c99_ast.Pointer(referenced_type=elem_type),
                        ),
                    )
                    instrs.append(tac_ast.Binary(
                        op=tac_ast.Add(),
                        src1=base,
                        src2=_tac_const_val(c99_ast.Long(), byte_offset),
                        dst=addr,
                    ))
                instrs.append(tac_ast.Store(src=val, dst_ptr=addr))

    def _emit_struct_init_stores(
        self,
        struct_type,  # c99_ast.Structure | c99_ast.Union
        init: c99_ast.InitList,
        base: tac_ast.Type_val,
        base_offset: int,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        """Walk a struct/union initializer, emitting Store instructions
        for each scalar leaf at `base + (base_offset + member_offset)`
        bytes. Missing items zero-pad. For unions only the first named
        member is initialized per C99 §6.7.8.16."""
        if self._types is None:
            raise TypeError("struct init lowering requires TypeTable")
        layout = self._types.get(struct_type.tag)
        if layout is None or not layout.complete:
            raise TypeError(f"incomplete struct/union {struct_type!r}")
        members = (
            layout.members[:1]
            if isinstance(struct_type, c99_ast.Union)
            else layout.members
        )
        for i, m in enumerate(members):
            byte_offset = base_offset + m.byte_offset
            item = init.items[i] if i < len(init.items) else None
            mt = m.type
            if isinstance(mt, c99_ast.Array):
                if (
                    isinstance(item, c99_ast.String)
                    and _is_char_element(mt.element_type)
                ):
                    self._emit_string_stores(
                        item, mt, base, byte_offset, instrs,
                    )
                    continue
                sub = (
                    item if item is not None
                    else c99_ast.InitList(items=[], data_type=mt)
                )
                self._emit_init_stores(
                    mt, sub, base, byte_offset, instrs,
                )
            elif isinstance(mt, (c99_ast.Structure, c99_ast.Union)):
                sub = (
                    item if item is not None
                    else c99_ast.InitList(items=[], data_type=mt)
                )
                self._emit_struct_init_stores(
                    mt, sub, base, byte_offset, instrs,
                )
            else:
                if item is None:
                    val = _tac_const_val(mt, 0)
                else:
                    val = self.translate_exp(item, instrs)
                if byte_offset == 0:
                    addr = base
                else:
                    addr = tac_ast.Var(
                        name=self.make_temporary_variable_name(
                            c99_ast.Pointer(referenced_type=mt),
                        ),
                    )
                    instrs.append(tac_ast.Binary(
                        op=tac_ast.Add(),
                        src1=base,
                        src2=_tac_const_val(c99_ast.Long(), byte_offset),
                        dst=addr,
                    ))
                instrs.append(tac_ast.Store(src=val, dst_ptr=addr))

    def _translate_struct_init_list(
        self,
        vd: c99_ast.Type_var_decl,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        """Lower `struct s x = {e1, e2, ...};` (block-scope only) by
        computing the address of `x` once and emitting one Store per
        scalar leaf."""
        struct_type = vd.data_type
        assert isinstance(struct_type, (c99_ast.Structure, c99_ast.Union))
        init = vd.init
        assert isinstance(init, c99_ast.InitList)
        base = tac_ast.Var(
            name=self.make_temporary_variable_name(
                c99_ast.Pointer(referenced_type=c99_ast.Int()),
            ),
        )
        instrs.append(tac_ast.GetAddress(
            operand=tac_ast.Var(name=vd.name), dst=base,
        ))
        self._emit_struct_init_stores(
            struct_type, init, base, 0, instrs,
        )

    def _member_offset(self, struct_type, member: str) -> int:
        """Look up the byte offset of `member` in `struct_type`'s
        layout. The type checker has validated existence; this is
        a straight read."""
        layout = self._types.get(struct_type.tag)
        for m in layout.members:
            if m.name == member:
                return m.byte_offset
        raise TypeError(
            f"struct/union {struct_type.tag!r} has no member "
            f"{member!r} (type checker should have caught this)"
        )

    def _add_offset(
        self,
        base: tac_ast.Type_val,
        offset: int,
        elem_type,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        """Return `base + offset` (as a Pointer-typed temp) when
        `offset != 0`; return `base` itself when `offset == 0`."""
        if offset == 0:
            return base
        addr = tac_ast.Var(
            name=self.make_temporary_variable_name(
                c99_ast.Pointer(referenced_type=elem_type),
            ),
        )
        instrs.append(tac_ast.Binary(
            op=tac_ast.Add(),
            src1=base,
            src2=_tac_const_val(c99_ast.Long(), offset),
            dst=addr,
        ))
        return addr

    def _translate_lvalue_address(
        self,
        lval: c99_ast.Type_exp,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        """Compute the byte address of an lvalue expression. Used by
        the Dot lvalue path to get a base for the parent struct, and
        by AddressOf for member accesses. Accepts the canonical
        addressable forms (Var / Dereference / Subscript / Dot /
        Arrow) plus rvalue struct/union expressions like
        `f().x` or `(c ? a : b).x` — translating those produces a
        struct-typed temp Var (the sret slot for FunctionCall, the
        per-Conditional dst slot for Conditional), which IS
        addressable.
        """
        if isinstance(lval, c99_ast.Var):
            base = tac_ast.Var(
                name=self.make_temporary_variable_name(
                    c99_ast.Pointer(referenced_type=lval.data_type),
                ),
            )
            instrs.append(tac_ast.GetAddress(
                operand=tac_ast.Var(name=lval.name), dst=base,
            ))
            return base
        if isinstance(lval, c99_ast.Dereference):
            return self.translate_exp(lval.exp, instrs)
        if isinstance(lval, c99_ast.Subscript):
            return self._translate_subscript_address(
                lval.array, lval.index, instrs,
            )
        if isinstance(lval, c99_ast.Dot):
            return self._translate_dot_address(
                lval.operand, lval.member, instrs,
            )
        if isinstance(lval, c99_ast.Arrow):
            return self._translate_arrow_address(
                lval.operand, lval.member, instrs,
            )
        # Struct / union rvalue expressions whose result lands in a
        # temp slot — `f().m`, `(c?a:b).m`. Translate the
        # expression to materialize the slot, then GetAddress on
        # the resulting Var.
        if isinstance(
            lval.data_type, (c99_ast.Structure, c99_ast.Union),
        ):
            val = self.translate_exp(lval, instrs)
            if not isinstance(val, tac_ast.Var):
                raise TypeError(
                    f"struct rvalue translation didn't produce a "
                    f"Var: {val!r}"
                )
            base = tac_ast.Var(
                name=self.make_temporary_variable_name(
                    c99_ast.Pointer(referenced_type=lval.data_type),
                ),
            )
            instrs.append(tac_ast.GetAddress(operand=val, dst=base))
            return base
        raise TypeError(f"not an addressable lvalue: {lval!r}")

    def _translate_dot_address(
        self,
        operand: c99_ast.Type_exp,
        member: str,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        """Compute the byte address of `e.m`. The operand IS a
        struct/union lvalue; get its address, add the member's
        offset."""
        struct_type = operand.data_type
        base = self._translate_lvalue_address(operand, instrs)
        offset = self._member_offset(struct_type, member)
        m_layout = self._types.get(struct_type.tag)
        m_type = next(
            mi.type for mi in m_layout.members if mi.name == member
        )
        return self._add_offset(base, offset, m_type, instrs)

    def _translate_arrow_address(
        self,
        operand: c99_ast.Type_exp,
        member: str,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        """Compute the byte address of `p->m`. Translate the pointer
        operand, then add the member's offset."""
        ptr_type = operand.data_type
        assert isinstance(ptr_type, c99_ast.Pointer)
        struct_type = ptr_type.referenced_type
        ptr_val = self.translate_exp(operand, instrs)
        offset = self._member_offset(struct_type, member)
        m_layout = self._types.get(struct_type.tag)
        m_type = next(
            mi.type for mi in m_layout.members if mi.name == member
        )
        return self._add_offset(ptr_val, offset, m_type, instrs)

    def _translate_subscript_address(
        self,
        array: c99_ast.Type_exp,
        index: c99_ast.Type_exp,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        """Compute the byte address of `array[index]`. The type
        checker has already decayed any array operand to a pointer
        and widened the index to Long, so this is exactly a pointer
        + integer arithmetic op — `translate_pointer_arithmetic`
        scales the index by `sizeof(*array)` and emits the Add. The
        returned val is a Pointer-typed (so 2-byte, the 6502's
        address width) temp holding the byte address; both the
        rvalue path (Load through it) and the lvalue path (Store
        through it) consume it the same way."""
        arr_val = self.translate_exp(array, instrs)
        idx_val = self.translate_exp(index, instrs)
        return self.translate_pointer_arithmetic(
            op=c99_ast.Add(),
            src1=arr_val, src2=idx_val,
            lt=array.data_type, rt=index.data_type,
            result_type=array.data_type,
            instrs=instrs,
        )

    def _translate_incdec(
        self,
        op: c99_ast.Type_incdec_op,
        operand: c99_ast.Type_exp,
        instrs: list[tac_ast.Type_instruction],
        *,
        return_old: bool,
    ) -> tac_ast.Type_val:
        """Lower `++operand` / `--operand` / `operand++` / `operand--`
        as a read-modify-write on the operand's storage location.

        Returns the val the surrounding expression evaluates to: the
        OLD value (Postfix, `return_old=True`) or the NEW value
        (Prefix, `return_old=False`).

        The operand is one of the three syntactic lvalues
        identifier_resolution accepts:
          * `Var(name)` — direct read/write of the named cell.
          * `Subscript(arr, idx)` — compute the byte address ONCE via
            `_translate_subscript_address`, Load the old value,
            Binary the new value, Store back through the same
            address. Evaluating the address only once is what makes
            `++arr[--i]` correct: the `--i` side effect fires once.
          * `Dereference(ptr_exp)` — evaluate the pointer expression
            ONCE into a val, Load through it, Binary, Store through
            the same val. Same reasoning.

        For `Var`, no Load/Store is needed — TAC's Binary takes the
        Var as a val directly, and Copy writes back. The Postfix
        path additionally captures the pre-mutation value into an
        `old` temp before updating; the Prefix path skips that.

        The surrounding type-check pass already required the operand
        to be of an arithmetic / pointer object type (the same as
        Assignment); the lowering here trusts that and just sizes
        each temp by the operand's `data_type`."""
        op_type = operand.data_type or c99_ast.Int()
        if isinstance(operand, c99_ast.Var):
            var = tac_ast.Var(name=operand.name)
            old: tac_ast.Type_val | None = None
            if return_old:
                # Capture the pre-mutation value into `old` BEFORE
                # minting `new` so the temp numbering matches the
                # source-order intuition (old gets the lower number).
                old = tac_ast.Var(
                    name=self.make_temporary_variable_name(op_type),
                )
                instrs.append(tac_ast.Copy(src=var, dst=old))
            new = self._emit_incdec_step(op, var, op_type, instrs)
            instrs.append(tac_ast.Copy(src=new, dst=var))
            return old if return_old else new
        # Subscript / Dereference: compute the lvalue's byte address
        # exactly once, then Load + Binary + Store through it.
        if isinstance(operand, c99_ast.Subscript):
            addr = self._translate_subscript_address(
                operand.array, operand.index, instrs,
            )
        elif isinstance(operand, c99_ast.Dereference):
            addr = self.translate_exp(operand.exp, instrs)
        elif isinstance(operand, c99_ast.Dot):
            addr = self._translate_dot_address(
                operand.operand, operand.member, instrs,
            )
        elif isinstance(operand, c99_ast.Arrow):
            addr = self._translate_arrow_address(
                operand.operand, operand.member, instrs,
            )
        else:
            raise TypeError(
                f"increment/decrement operand must be Var, Subscript, "
                f"Dereference, Dot, or Arrow "
                f"(identifier_resolution should have enforced this); "
                f"got {operand!r}"
            )
        cur = tac_ast.Var(
            name=self.make_temporary_variable_name(op_type),
        )
        instrs.append(tac_ast.Load(src_ptr=addr, dst=cur))
        new = self._emit_incdec_step(op, cur, op_type, instrs)
        instrs.append(tac_ast.Store(src=new, dst_ptr=addr))
        return cur if return_old else new

    def _emit_incdec_step(
        self,
        op: c99_ast.Type_incdec_op,
        src: tac_ast.Type_val,
        op_type: c99_ast.Type_data_type,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        """Compute `src ± 1` (per `op`) at the operand's type.

        For pointer operands routes through
        `translate_pointer_arithmetic` so the increment scales by
        `sizeof(*ptr)` per C99 §6.5.6.8 — synthesising the same
        Cast(Long)-widened-1 the type checker would have produced
        for an explicit `ptr + 1`. For arithmetic operands emits a
        plain `Binary(Add | Subtract, src, 1)`."""
        if isinstance(op_type, c99_ast.Pointer):
            c99_op: c99_ast.Type_binary_operator = (
                c99_ast.Add() if isinstance(op, c99_ast.Increment)
                else c99_ast.Subtract()
            )
            return self.translate_pointer_arithmetic(
                op=c99_op,
                src1=src,
                src2=_tac_const_val(c99_ast.Long(), 1),
                lt=op_type,
                rt=c99_ast.Long(),
                result_type=op_type,
                instrs=instrs,
            )
        new = tac_ast.Var(
            name=self.make_temporary_variable_name(op_type),
        )
        instrs.append(tac_ast.Binary(
            op=self.translate_incdec(op),
            src1=src,
            src2=_tac_const_val(op_type, 1),
            dst=new,
        ))
        return new

    def _translate_compound_assign(
        self,
        op: c99_ast.Type_binary_operator,
        lval: c99_ast.Type_exp,
        rval: c99_ast.Type_exp,
        intermediate_type: c99_ast.Type_data_type | None,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        """Lower `lval OP= rval` as a read-modify-write on lval's
        storage location, with the binop happening at the
        intermediate type the type checker stamped on `rval`.

        For Subscript / Dereference / Dot / Arrow lvals the address
        is computed exactly ONCE so any side effects in the
        address-computing subexpressions (`arr[i++] += 1`,
        `(*p++)++`, `ptr++[idx++] *= 3`) fire only once. For Var
        lvals there's no address — the Var is read and written in
        place.

        Type rules (mirroring the Binary type-check):
          * The lval has its own type T.
          * The rval was cast by the type checker to the
            intermediate type C — so `rval.data_type` IS C.
          * The lval's loaded value must also be brought to C
            before the binop; we wrap it in a synthetic Cast
            (which lowers to SignExtend / ZeroExtend / Truncate /
            no-op as appropriate).
          * The binop result has type C (or the promoted-left for
            shifts; same thing here since shift's intermediate IS
            the promoted left).
          * The binop result is then cast back to T for the
            store.

        Pointer arithmetic (`ptr += int` / `ptr -= int`) is
        special-cased: route through `translate_pointer_arithmetic`
        so the integer rval gets scaled by sizeof(pointee) before
        the add/sub.

        The returned val is the new value of lval, at type T —
        this lets `b = (a += 5)` chain correctly."""
        lv_type = lval.data_type or c99_ast.Int()
        if intermediate_type is None:
            intermediate_type = lv_type
        is_pointer_arith = (
            isinstance(op, (c99_ast.Add, c99_ast.Subtract))
            and isinstance(lv_type, c99_ast.Pointer)
        )
        if isinstance(lval, c99_ast.Var):
            lval_var = tac_ast.Var(name=lval.name)
            new_val = self._compute_compound_step(
                op, lval_var, lv_type, rval,
                intermediate_type, is_pointer_arith, instrs,
            )
            instrs.append(tac_ast.Copy(src=new_val, dst=lval_var))
            return new_val
        # Subscript / Dereference / Dot / Arrow: compute the
        # lvalue's byte address exactly once, then Load + binop +
        # Store through it.
        if isinstance(lval, c99_ast.Subscript):
            addr = self._translate_subscript_address(
                lval.array, lval.index, instrs,
            )
        elif isinstance(lval, c99_ast.Dereference):
            addr = self.translate_exp(lval.exp, instrs)
        elif isinstance(lval, c99_ast.Dot):
            addr = self._translate_dot_address(
                lval.operand, lval.member, instrs,
            )
        elif isinstance(lval, c99_ast.Arrow):
            addr = self._translate_arrow_address(
                lval.operand, lval.member, instrs,
            )
        else:
            raise TypeError(
                f"compound-assignment lval must be Var, Subscript, "
                f"Dereference, Dot, or Arrow (identifier_resolution "
                f"should have enforced this); got {lval!r}"
            )
        cur = tac_ast.Var(
            name=self.make_temporary_variable_name(lv_type),
        )
        instrs.append(tac_ast.Load(src_ptr=addr, dst=cur))
        new_val = self._compute_compound_step(
            op, cur, lv_type, rval, intermediate_type,
            is_pointer_arith, instrs,
        )
        instrs.append(tac_ast.Store(src=new_val, dst_ptr=addr))
        return new_val

    def _compute_compound_step(
        self,
        op: c99_ast.Type_binary_operator,
        lval_val: tac_ast.Type_val,
        lv_type: c99_ast.Type_data_type,
        rval_exp: c99_ast.Type_exp,
        intermediate_type: c99_ast.Type_data_type,
        is_pointer_arith: bool,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        """Compute `(T)((C)lval_val OP rval)` and return the
        resulting val of type T (the lval's type). Used by both
        the Var and address-based branches of
        `_translate_compound_assign`."""
        rval_val = self.translate_exp(rval_exp, instrs)
        if is_pointer_arith:
            # ptr += int — route through translate_pointer_arithmetic
            # so the rval is scaled by sizeof(pointee).
            return self.translate_pointer_arithmetic(
                op=op,
                src1=lval_val,
                src2=rval_val,
                lt=lv_type,
                rt=rval_exp.data_type or c99_ast.Long(),
                result_type=lv_type,
                instrs=instrs,
            )
        # Cast loaded lval value to intermediate type if they differ.
        lval_val_at_inter = self._emit_widen_or_narrow(
            lval_val, lv_type, intermediate_type, instrs,
        )
        # Apply the binop at the intermediate type.
        binop_dst = tac_ast.Var(
            name=self.make_temporary_variable_name(intermediate_type),
        )
        instrs.append(tac_ast.Binary(
            op=self.translate_binop(op),
            src1=lval_val_at_inter,
            src2=rval_val,
            dst=binop_dst,
        ))
        # Convert binop result back to lval's type.
        return self._emit_widen_or_narrow(
            binop_dst, intermediate_type, lv_type, instrs,
        )

    def _emit_widen_or_narrow(
        self,
        src_val: tac_ast.Type_val,
        src_type: c99_ast.Type_data_type,
        dst_type: c99_ast.Type_data_type,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        """Cast `src_val` (typed `src_type`) to `dst_type`,
        emitting the appropriate TAC cast node. Returns the new
        val of type `dst_type`. Mirrors the Cast lowering in
        translate_exp's c99_ast.Cast case — covers integer↔integer
        (SignExtend / ZeroExtend / Truncate / no-op), integer↔FP
        (IntToFloat / IntToDouble / FloatToInt / DoubleToInt), and
        Float↔Double (FloatToDouble / DoubleToFloat)."""
        if src_type == dst_type:
            return src_val
        src_fp = isinstance(src_type, (c99_ast.Float, c99_ast.Double))
        dst_fp = isinstance(dst_type, (c99_ast.Float, c99_ast.Double))
        if src_fp or dst_fp:
            dst = tac_ast.Var(
                name=self.make_temporary_variable_name(dst_type),
            )
            if src_fp and dst_fp:
                node_cls = (
                    tac_ast.FloatToDouble
                    if isinstance(src_type, c99_ast.Float)
                    else tac_ast.DoubleToFloat
                )
            elif src_fp:
                node_cls = (
                    tac_ast.FloatToInt
                    if isinstance(src_type, c99_ast.Float)
                    else tac_ast.DoubleToInt
                )
            else:
                node_cls = (
                    tac_ast.IntToFloat
                    if isinstance(dst_type, c99_ast.Float)
                    else tac_ast.IntToDouble
                )
            instrs.append(node_cls(src=src_val, dst=dst))
            return dst
        src_w = _byte_width_of(src_type)
        dst_w = _byte_width_of(dst_type)
        if src_w == dst_w:
            # Same byte width, just different signedness — no codegen.
            return src_val
        dst = tac_ast.Var(
            name=self.make_temporary_variable_name(dst_type),
        )
        if src_w < dst_w:
            if isinstance(
                src_type,
                (c99_ast.Int, c99_ast.Long, c99_ast.LongLong),
            ):
                instrs.append(tac_ast.SignExtend(src=src_val, dst=dst))
            else:
                instrs.append(tac_ast.ZeroExtend(src=src_val, dst=dst))
        else:
            instrs.append(tac_ast.Truncate(src=src_val, dst=dst))
        return dst

    def translate_short_circuit(
        self,
        left: c99_ast.Type_exp,
        right: c99_ast.Type_exp,
        instrs: list[tac_ast.Type_instruction],
        short_circuit_on_true: bool,
    ) -> tac_ast.Type_val:
        # && short-circuits to 0 on the first false operand; || to 1
        # on the first true operand. Otherwise the two lowerings are
        # mirror images, so we parametrize:
        #   - which conditional-jump opcode short-circuits the chain
        #   - which constant the short-circuit branch writes (the
        #     short-circuit outcome), vs. the fallthrough branch (the
        #     opposite outcome)
        if short_circuit_on_true:
            branch_prefix, end_prefix = "or_true", "or_end"
            short_circuit_jump = tac_ast.JumpIfTrue
            short_circuit_value, fallthrough_value = 1, 0
        else:
            branch_prefix, end_prefix = "and_false", "and_end"
            short_circuit_jump = tac_ast.JumpIfFalse
            short_circuit_value, fallthrough_value = 0, 1
        branch_label = self.make_label(branch_prefix)
        end_label = self.make_label(end_prefix)
        # Short-circuit's result is always Int per C99 §6.5.13.3 /
        # §6.5.14.3, regardless of operand type.
        dst = tac_ast.Var(
            name=self.make_temporary_variable_name(c99_ast.Int()),
        )

        src1 = self.translate_exp(left, instrs)
        instrs.append(short_circuit_jump(condition=src1, target=branch_label))
        src2 = self.translate_exp(right, instrs)
        instrs.append(short_circuit_jump(condition=src2, target=branch_label))
        # Short-circuit's result is always Int (per C99 §6.5.13.3 /
        # §6.5.14.3), so the 0/1 selector constants are ConstInt.
        instrs.append(tac_ast.Copy(
            src=_tac_const_val(c99_ast.Int(), fallthrough_value),
            dst=dst,
        ))
        instrs.append(tac_ast.Jump(target=end_label))
        instrs.append(tac_ast.Label(name=branch_label))
        instrs.append(tac_ast.Copy(
            src=_tac_const_val(c99_ast.Int(), short_circuit_value),
            dst=dst,
        ))
        instrs.append(tac_ast.Label(name=end_label))
        return dst

    def translate_unop(
        self, op: c99_ast.Type_unary_operator,
    ) -> tac_ast.Type_unary_operator:
        match op:
            case c99_ast.Complement():
                return tac_ast.Complement()
            case c99_ast.Negate():
                return tac_ast.Negate()
            case c99_ast.LogicalNot():
                return tac_ast.LogicalNot()
        raise TypeError(f"unexpected unop: {op!r}")

    def translate_incdec(
        self, op: c99_ast.Type_incdec_op,
    ) -> tac_ast.Type_binary_operator:
        # Postfix ++/-- lower to a Binary(Add/Subtract, operand, 1).
        match op:
            case c99_ast.Increment():
                return tac_ast.Add()
            case c99_ast.Decrement():
                return tac_ast.Subtract()
        raise TypeError(f"unexpected incdec op: {op!r}")

    def translate_binop(
        self, op: c99_ast.Type_binary_operator,
    ) -> tac_ast.Type_binary_operator:
        match op:
            case c99_ast.Add():
                return tac_ast.Add()
            case c99_ast.Subtract():
                return tac_ast.Subtract()
            case c99_ast.Multiply():
                return tac_ast.Multiply()
            case c99_ast.Divide():
                return tac_ast.Divide()
            case c99_ast.Modulo():
                return tac_ast.Modulo()
            case c99_ast.BitwiseAnd():
                return tac_ast.BitwiseAnd()
            case c99_ast.BitwiseOr():
                return tac_ast.BitwiseOr()
            case c99_ast.BitwiseXor():
                return tac_ast.BitwiseXor()
            case c99_ast.LeftShift():
                return tac_ast.LeftShift()
            case c99_ast.RightShift():
                return tac_ast.RightShift()
            case c99_ast.Equal():
                return tac_ast.Equal()
            case c99_ast.NotEqual():
                return tac_ast.NotEqual()
            case c99_ast.LessThan():
                return tac_ast.LessThan()
            case c99_ast.GreaterThan():
                return tac_ast.GreaterThan()
            case c99_ast.LessOrEqual():
                return tac_ast.LessOrEqual()
            case c99_ast.GreaterOrEqual():
                return tac_ast.GreaterOrEqual()
        raise TypeError(f"unexpected binop: {op!r}")


def translate_program(
    prog: c99_ast.Type_program,
    symbols: SymbolTable | None = None,
    types=None,
) -> tac_ast.Type_program:
    """Convenience wrapper: builds a fresh Translator per call (so the
    temporary counter starts at 0 every time). The `symbols` and
    `types` tables must be those produced by
    `passes.type_checking.check_program` on the same `prog` — they're
    consumed for `is_global` lookups on functions, the StaticVariable
    enumeration at the end of program translation, and struct/union
    layout sizing. The default of `None` is a test convenience; the
    wrapper will run `check_program` on `prog` to fill them in, which
    is what the production pipeline does anyway."""
    if symbols is None:
        from passes.type_checking import check_program
        _, symbols, types = check_program(prog)
    return Translator(symbols, types).translate_program(prog)
