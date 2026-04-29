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
    """Byte width of an object type. Int / UInt = 1, Long / ULong = 2,
    Float = 4, Double = 8, Pointer = 2 (the 6502's address width).
    Used by Cast lowering to decide between SignExtend / ZeroExtend /
    Truncate / no-op (for integer types) and by various size-driven
    dispatch sites downstream."""
    if isinstance(t, (c99_ast.Int, c99_ast.UInt)):
        return 1
    if isinstance(t, (c99_ast.Long, c99_ast.ULong)):
        return 2
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
    if isinstance(t, c99_ast.UInt):
        return tac_ast.UInt()
    if isinstance(t, c99_ast.ULong):
        return tac_ast.ULong()
    if isinstance(t, c99_ast.Float):
        return tac_ast.Float()
    if isinstance(t, c99_ast.Double):
        return tac_ast.Double()
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
    if isinstance(t, c99_ast.FunType):
        return tac_ast.FunType(
            params=[_to_tac_data_type(p) for p in t.params],
            ret=_to_tac_data_type(t.ret),
        )
    raise TypeError(f"unexpected c99 data_type: {t!r}")


def _to_tac_const(c: c99_ast.Type_const) -> tac_ast.Type_const:
    """Translate a c99 const to its TAC counterpart. TAC collapses
    integer signedness onto width (the 6502 has no signedness at the
    byte level), so ConstUInt(v) becomes TAC ConstInt(v) (1 byte)
    and ConstULong(v) becomes TAC ConstLong(v) (2 bytes); the integer
    value passes through unchanged because downstream `_byte_at` masks
    each byte with `& 0xFF`. FP variants stay distinct (Float and
    Double have different IEEE 754 bit patterns), so ConstFloat /
    ConstDouble round-trip 1-to-1."""
    if isinstance(c, c99_ast.ConstInt):
        return tac_ast.ConstInt(int=c.int)
    if isinstance(c, c99_ast.ConstLong):
        return tac_ast.ConstLong(int=c.int)
    if isinstance(c, c99_ast.ConstUInt):
        return tac_ast.ConstInt(int=c.int)
    if isinstance(c, c99_ast.ConstULong):
        return tac_ast.ConstLong(int=c.int)
    if isinstance(c, c99_ast.ConstFloat):
        return tac_ast.ConstFloat(float=c.float)
    if isinstance(c, c99_ast.ConstDouble):
        return tac_ast.ConstDouble(float=c.float)
    raise TypeError(f"unexpected c99 const: {c!r}")


def _tac_const_for(t: c99_ast.Type_data_type, value: int | float) -> tac_ast.Type_const:
    """Build a TAC const matching `t`'s width (and, for FP, its
    precision). TAC collapses integer signedness onto width — UInt
    and Int both produce ConstInt; ULong and Long both produce
    ConstLong (see `_to_tac_const`) — but Float / Double remain
    distinct. Pointer collapses onto ConstLong (same 2-byte width
    as Long; addresses are unsigned-ish 2-byte values). Used by the
    synthetic-constant call sites (postfix `+1`, short-circuit 0/1,
    implicit `return 0`)."""
    if isinstance(t, (c99_ast.Int, c99_ast.UInt)):
        return tac_ast.ConstInt(int=int(value))
    if isinstance(t, (c99_ast.Long, c99_ast.ULong, c99_ast.Pointer)):
        return tac_ast.ConstLong(int=int(value))
    if isinstance(t, c99_ast.Float):
        return tac_ast.ConstFloat(float=float(value))
    if isinstance(t, c99_ast.Double):
        return tac_ast.ConstDouble(float=float(value))
    raise TypeError(
        f"cannot build a TAC const for non-object type {t!r}"
    )


def _tac_const_val(t: c99_ast.Type_data_type, value: int | float) -> tac_ast.Constant:
    """Convenience: build a TAC `Constant(const=...)` val typed by
    `t`. The result is a `Type_val` ready to drop into a TAC
    instruction's src / dst slot."""
    return tac_ast.Constant(const=_tac_const_for(t, value))


def _fold_fp_cast_constant(
    source: c99_ast.Type_data_type,
    target: c99_ast.Type_data_type,
    c: tac_ast.Type_const,
) -> tac_ast.Type_const:
    """Compile-time fold of a Cast whose operand is a Constant and
    whose source-or-target is FP. Returns a TAC const of `target`'s
    type. We have to do this work in `c99_to_tac` (rather than the
    runtime helper-call path) because TAC's `const` collapses integer
    signedness onto width — by the time `tac_to_asm` sees the constant,
    it can't tell `Int` from `UInt`. Here `source` is the c99 type
    (still distinguishing signed and unsigned), so we can pick the
    right interpretation: an unsigned source masks any negative TAC
    int back to its unsigned bit pattern (e.g. ConstInt(-1) viewed as
    UInt → 255) before converting. C99 §6.3.1.4 requires FP→integer
    truncation toward zero, which is also Python's `int(float)`
    semantics."""
    if isinstance(c, (tac_ast.ConstFloat, tac_ast.ConstDouble)):
        v: int | float = c.float
    elif isinstance(c, tac_ast.ConstInt):
        v = c.int & 0xFF if isinstance(source, c99_ast.UInt) else c.int
    elif isinstance(c, tac_ast.ConstLong):
        v = c.int & 0xFFFF if isinstance(source, c99_ast.ULong) else c.int
    else:
        raise TypeError(f"unexpected TAC const: {c!r}")
    return _tac_const_for(target, v)


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
        return tac_ast.IntInit(int=int(value))
    if isinstance(t, c99_ast.Long):
        return tac_ast.LongInit(int=int(value))
    if isinstance(t, c99_ast.UInt):
        return tac_ast.UIntInit(int=int(value))
    if isinstance(t, c99_ast.ULong):
        return tac_ast.ULongInit(int=int(value))
    if isinstance(t, c99_ast.Float):
        return tac_ast.FloatInit(float=float(value))
    if isinstance(t, c99_ast.Double):
        return tac_ast.DoubleInit(float=float(value))
    if isinstance(t, c99_ast.Pointer):
        # Pointer collapses onto Long for static-init purposes —
        # addresses are 2-byte values written as a little-endian
        # 16-bit integer (e.g. NULL = 0x0000).
        return tac_ast.LongInit(int=int(value))
    raise TypeError(
        f"static-storage object can't have non-object type {t!r}"
    )


def _zero_init_value(t: c99_ast.Type_data_type):
    """Default-zero value tree for a Tentative file-scope static. A
    scalar yields `0` (or `0.0` for FP); an array yields a tuple of
    typed zeros sized to the array (recursive for multi-dim). The
    type checker uses the same shape for `static T x;` (no init), so
    `_flat_static_init` consumes both via the same code path."""
    if isinstance(t, c99_ast.Array):
        return tuple(_zero_init_value(t.element_type) for _ in range(t.size))
    if isinstance(t, (c99_ast.Float, c99_ast.Double)):
        return 0.0
    return 0


def _flat_static_init(
    t: c99_ast.Type_data_type,
    value,
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
    return _coalesce_zero_inits(_flat_static_init_raw(t, value))


def _flat_static_init_raw(
    t: c99_ast.Type_data_type,
    value,
) -> list[tac_ast.Type_static_init]:
    if isinstance(t, c99_ast.Array):
        if not isinstance(value, tuple) or len(value) != t.size:
            raise TypeError(
                f"array static init shape mismatch: expected tuple of "
                f"size {t.size} for {t!r}, got {value!r}"
            )
        out: list[tac_ast.Type_static_init] = []
        for elem in value:
            out.extend(_flat_static_init_raw(t.element_type, elem))
        return out
    return [_tac_static_init_for(t, value)]


def _zero_byte_count(item: tac_ast.Type_static_init) -> int | None:
    """Byte count of `item` if its in-memory bytes are all zero,
    else None. Integer-zero items (Int / UInt / Long / ULong) and
    `+0.0` FP items (`-0.0` isn't representable in c6502 statics —
    the parser routes negation through `Unary`, which the constant-
    expression check rejects) qualify. AddressInit never qualifies
    — `&name` is symbolic, resolved by the assembler at link time
    to an address that may or may not be zero."""
    if isinstance(item, tac_ast.IntInit) and item.int == 0:
        return 1
    if isinstance(item, tac_ast.UIntInit) and item.int == 0:
        return 1
    if isinstance(item, tac_ast.LongInit) and item.int == 0:
        return 2
    if isinstance(item, tac_ast.ULongInit) and item.int == 0:
        return 2
    if isinstance(item, tac_ast.FloatInit) and item.float == 0.0:
        return 4
    if isinstance(item, tac_ast.DoubleInit) and item.float == 0.0:
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


def _pointee_size(t: c99_ast.Type_data_type) -> int:
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
    return _sizeof(t.referenced_type)


def _sizeof(t: c99_ast.Type_data_type) -> int:
    """Bytes occupied by a value of type `t` in c6502's storage
    model. Recursive for Array — `int[3][4]` is 12 bytes."""
    if isinstance(t, (c99_ast.Int, c99_ast.UInt)):
        return 1
    if isinstance(t, (c99_ast.Long, c99_ast.ULong, c99_ast.Pointer)):
        return 2
    if isinstance(t, c99_ast.Float):
        return 4
    if isinstance(t, c99_ast.Double):
        return 8
    if isinstance(t, c99_ast.Array):
        return _sizeof(t.element_type) * t.size
    raise TypeError(f"cannot size {t!r}")


class Translator:
    def __init__(self, symbols: SymbolTable | None = None) -> None:
        self._temp_counter = 0
        self._label_counter = 0
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
        instrs: list[tac_ast.Type_instruction] = []
        self.translate_block(fd.body, instrs)
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
            ret_type = (
                fd.data_type.ret
                if isinstance(fd.data_type, c99_ast.FunType)
                else c99_ast.Int()
            )
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
        # same names.
        return tac_ast.Function(
            name=fd.name,
            is_global=is_global,
            params=list(fd.params),
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
                init_value = _zero_init_value(sym.type)
            elif isinstance(init, NoInitializer):
                continue
            else:
                raise TypeError(f"unexpected initial value: {init!r}")
            data_type = _to_tac_data_type(sym.type)
            out.append(tac_ast.StaticVariable(
                name=name,
                is_global=sym.attrs.is_global,
                data_type=data_type,
                init=_flat_static_init(sym.type, init_value),
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
                        self._translate_array_init_list(vd, instrs)
                    else:
                        init_val = self.translate_exp(vd.init, instrs)
                        instrs.append(tac_ast.Copy(
                            src=init_val, dst=tac_ast.Var(name=vd.name),
                        ))
                return
            case c99_ast.FunctionDecl():
                return
        raise TypeError(f"unexpected declaration: {decl!r}")

    def translate_statement(
        self,
        stmt: c99_ast.Type_statement,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        match stmt:
            case c99_ast.Return(exp=exp):
                instrs.append(tac_ast.Ret(val=self.translate_exp(exp, instrs)))
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
                # dispatcher.
                if vd.init is not None:
                    if isinstance(vd.init, c99_ast.InitList):
                        self._translate_array_init_list(vd, instrs)
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
    ) -> tac_ast.Type_val:
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
                # Lower `Cast` based on the source/target c99 types.
                # The 6502 has no signedness distinction, so cross-sign
                # integer casts at the same width are no-ops; FP types
                # need explicit conversion nodes because their bit
                # patterns aren't compatible with integers (or with
                # each other across precisions):
                #   src == target                         → no-op
                #   integer same width (Int↔UInt, Long↔ULong)
                #                                         → no-op
                #   integer 1B → 2B, source signed (Int)  → SignExtend
                #   integer 1B → 2B, source unsigned      → ZeroExtend
                #   integer 2B → 1B (any signedness)      → Truncate
                #   integer → Float / Double              → IntToFloat /
                #                                           IntToDouble
                #   Float / Double → integer              → FloatToInt /
                #                                           DoubleToInt
                #   Float ↔ Double                        → FloatToDouble /
                #                                           DoubleToFloat
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
                    # avoids a runtime helper call AND sidesteps the
                    # signedness-erasure of TAC integer consts (see
                    # `_fold_fp_cast_constant` for why).
                    if isinstance(inner_val, tac_ast.Constant):
                        return tac_ast.Constant(
                            const=_fold_fp_cast_constant(
                                source, target, inner_val.const,
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
                    if isinstance(source, (c99_ast.Int, c99_ast.Long)):
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
            case c99_ast.Assignment(lval=lval, rval=rval):
                # identifier_resolution accepts two lval shapes:
                # `Var(name)` (a named storage cell) and
                # `Dereference(ptr_exp)` (a store-through-pointer).
                # Anything else gets rejected upstream; the runtime
                # check here is belt-and-braces in case a later
                # refactor lets a non-lvalue slip through.
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
                raise TypeError(
                    f"assignment lval must be Var, Dereference, or "
                    f"Subscript (identifier_resolution should have "
                    f"enforced this); got {lval!r}"
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
                #   Copy(t_val, dst)
                #   Jump(cond_end@N)
                #   Label(cond_else@N)
                #   <eval false -> f_val>
                #   Copy(f_val, dst)
                #   Label(cond_end@N)
                cond_val = self.translate_exp(cond, instrs)
                else_label = self.make_label("cond_else")
                end_label = self.make_label("cond_end")
                # The two arms have already been promoted to the
                # common type by the type checker, and the
                # Conditional's data_type is that common type.
                dst = tac_ast.Var(
                    name=self.make_temporary_variable_name(exp.data_type),
                )
                instrs.append(tac_ast.JumpIfFalse(
                    condition=cond_val, target=else_label,
                ))
                t_val = self.translate_exp(true_clause, instrs)
                instrs.append(tac_ast.Copy(src=t_val, dst=dst))
                instrs.append(tac_ast.Jump(target=end_label))
                instrs.append(tac_ast.Label(name=else_label))
                f_val = self.translate_exp(false_clause, instrs)
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
                # The temp captures the call's return value; its
                # type is the function's declared return type,
                # which the type checker has stamped on the
                # FunctionCall node's data_type.
                dst = tac_ast.Var(
                    name=self.make_temporary_variable_name(exp.data_type),
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
                # `a++` (resp. `a--`) returns the *old* value of `a`
                # while incrementing (decrementing) it. Capture the
                # old value into a temp first; only then update `a`.
                # Returning the temp means later uses of the result
                # see the old value even after `a` has been mutated.
                #
                # Same defense-in-depth lvalue check as Assignment:
                # identifier_resolution should have already rejected
                # non-Var operands.
                if not isinstance(operand, c99_ast.Var):
                    raise TypeError(
                        f"postfix operand must be Var (variable_"
                        f"resolution should have enforced this); "
                        f"got {operand!r}"
                    )
                var = tac_ast.Var(name=operand.name)
                # Operand's data_type was set by the type checker;
                # fall back to Int if absent (synthetic test AST).
                # Both temps (the captured `old` value and the
                # incremented `new` value) have the operand's type.
                op_type = operand.data_type or c99_ast.Int()
                old = tac_ast.Var(
                    name=self.make_temporary_variable_name(op_type),
                )
                instrs.append(tac_ast.Copy(src=var, dst=old))
                new = tac_ast.Var(
                    name=self.make_temporary_variable_name(op_type),
                )
                instrs.append(tac_ast.Binary(
                    op=self.translate_incdec(op),
                    src1=var,
                    src2=_tac_const_val(op_type, 1),
                    dst=new,
                ))
                instrs.append(tac_ast.Copy(src=new, dst=var))
                return old
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
                raise TypeError(
                    f"address-of operand must be Var, Dereference, "
                    f"or Subscript (identifier_resolution / array "
                    f"decay should have enforced this); got {inner!r}"
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
            size = _pointee_size(lt)
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
        size = _pointee_size(ptr_type)
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
        elem_size = _sizeof(elem_type)
        for i in range(arr_type.size):
            byte_offset = base_offset + i * elem_size
            item = init.items[i] if i < len(init.items) else None
            if isinstance(elem_type, c99_ast.Array):
                # Recurse into the sub-array. A missing item is a
                # logically-empty InitList — every leaf zeroes.
                sub = (
                    item if item is not None
                    else c99_ast.InitList(items=[], data_type=elem_type)
                )
                self._emit_init_stores(
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
) -> tac_ast.Type_program:
    """Convenience wrapper: builds a fresh Translator per call (so the
    temporary counter starts at 0 every time). The `symbols` table
    must be the one produced by `passes.type_checking.check_program`
    on the same `prog` — it's consumed both for `is_global` lookups
    on functions and for the StaticVariable enumeration at the end of
    program translation. The default of `None` is a test convenience;
    the wrapper will run `check_program` on `prog` to fill it in,
    which is what the production pipeline does anyway."""
    if symbols is None:
        from passes.type_checking import check_program
        _, symbols = check_program(prog)
    return Translator(symbols).translate_program(prog)
