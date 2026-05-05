"""TAC constant folding pass.

For `Unary` / `Binary` whose every val operand is a `Constant`, for
`JumpIfTrue` / `JumpIfFalse` whose condition is a `Constant`, for
the integer-width and FP conversion casts (`SignExtend`,
`ZeroExtend`, `Truncate`, `IntToFloat`, `IntToDouble`, `FloatToInt`,
`DoubleToInt`, `FloatToDouble`, `DoubleToFloat`) whose source is a
`Constant`, and for `Copy` whose source is a `Constant` whose variant
disagrees with the dst Var's c99 type, evaluate the operation in
Python with arithmetic that matches what the 6502 lowering would
compute, then rewrite the instruction:

  - Unary / Binary / cast → Copy(Constant(result), dst)  (preserves dst)
  - Copy(Constant(c), dst) → Copy(Constant(rewrapped), dst), where
    rewrapped is `c` reinterpreted at the dst variant's signedness
    (same bit pattern; only `.value`'s number form changes)
  - JumpIfTrue(true)   /  JumpIfFalse(false)      → Jump(target)
  - JumpIfTrue(false)  /  JumpIfFalse(true)       → dropped

Integer width and signedness, matching `tac_to_asm`:

  - Integer constants carry width AND signedness via the `const`
    variant: Const{Int,UInt} are 8 bits, Const{Long,ULong} 16,
    Const{LongLong,ULongLong} 32. Signed variants store their value
    as a two's-complement signed integer in [-2^(n-1), 2^(n-1)-1];
    unsigned variants store the unsigned bit pattern in [0, 2^n - 1].
    Both representations canonicalize the same bit pattern, but the
    `.value` field's number form differs — keeping the optimizer's
    structural-equality fixed-point check well-behaved.
  - Arithmetic / bitwise Binary ops (`+`, `-`, `*`, `&`, `|`, `^`,
    `<<`): result variant matches src1 (equal to src2 post the type
    checker's usual arithmetic conversions). Same bit-pattern result
    regardless of signedness, so the operand interpretation only
    affects `.value` canonicalization.
  - Division / modulo: signed operands use truncation-toward-zero
    (C99 §6.5.5.6); unsigned operands use Python's `//` and `%`
    (which match unsigned non-negative arithmetic).
  - Comparison Binary ops always yield ConstInt (per C99 §6.5.8.6:
    `<` / `>` / etc. return int). Operand interpretation follows
    the operand variants' signedness — signed operands use signed
    comparison (matching `tac_to_asm`'s V-corrected SBC + MI/PL
    sequence), unsigned operands use unsigned (matching the BCC/BCS-
    based per-byte SBC sequence).
  - Right shifts: signed operands fold arithmetically (sign-
    preserving — matches `asr8` / `asr16` / `asr32`); unsigned
    operands fold logically (zero-fill — matches `lsr8` / `lsr16` /
    `lsr32`). Left shifts are signedness-agnostic (same bit pattern).
  - `Unary(LogicalNot)` always yields ConstInt regardless of the
    source operand's variant (per C99 §6.5.3.3.5: `!` returns int).

Floating-point semantics, via `fp_arith` (numpy-backed at the
operand precision):

  - `Negate` is a sign-bit flip (exact; preserves NaN payloads,
    swaps ±0).
  - `Add` / `Subtract` / `Multiply` / `Divide` round to nearest-
    even at the operand variant's precision; overflow → ±inf;
    invalid (e.g. `0/0`, `inf - inf`) → NaN.
  - Comparisons follow IEEE 754 §5.11: `+0 == -0`; any comparison
    against a NaN is unordered (== returns false; != returns true;
    <, >, <=, >= all return false). Result is `ConstInt`.
  - `JumpIf` truthiness follows C99 §6.3.1.2: a value compares
    truthy iff it compares unequal to 0. Both ±0 are falsy; NaN is
    truthy (NaN != 0 by definition). `LogicalNot` follows the
    same rule.

Copy folds:

  - `Copy(Constant(c), Var(name))` where `c`'s variant differs from
    the dst's c99 type's variant — but the bit widths match —
    rewraps `c` to the dst's variant. Same-width signed↔unsigned
    casts get elided in `c99_to_tac` (the bit pattern is identical),
    so a `(unsigned int)1` initializer for a `unsigned int` Var
    leaves a `Copy(Constant(ConstInt(1)), Var(uint_x))`; this fold
    canonicalizes the constant to `ConstUInt(1)` so downstream
    consumers see one unambiguous variant per Var. Width-mismatched
    Copies (which shouldn't reach this pass — c99_to_tac emits
    SignExtend / ZeroExtend / Truncate for those) are left alone.

Cast / conversion folds:

  - `SignExtend` / `ZeroExtend` / `Truncate` need the dst's TAC
    variant (width AND signedness) to canonicalize the result;
    that comes from the symbol table's c99 type for the dst Var.
    Without a symbol table, these aren't folded. Source signedness
    is encoded in the choice of node (SignExtend vs. ZeroExtend
    for widening) and in the source's const variant; truncation is
    signedness-agnostic at the bit level (just keeps low bytes).
  - `FloatToDouble` / `DoubleToFloat` need no symbol-table lookup
    — both source and target precisions are determined by the
    node class itself.
  - `IntToFloat` / `IntToDouble` read signedness off the source's
    const variant (Const{Int,Long,LongLong} → signed source,
    Const{UInt,ULong,ULongLong} → unsigned source). Both fold to
    a non-negative bit pattern via fp_arith's int-to-bits helpers.
    Compile-time-known FP casts of constants in the source program
    already get folded earlier in `c99_to_tac` (see
    `_fold_fp_cast_constant`); the only IntToFloat / IntToDouble
    nodes reaching this pass come from copy-propagating a constant
    into a Var-sourced cast.
  - `FloatToInt` / `DoubleToInt` truncate toward zero per C99
    §6.3.1.4 and store at the dst's TAC width / signedness. Bails
    on NaN / ±inf (UB per the standard; the runtime helpers'
    behavior isn't pinned yet).

Cases left unfolded:

  - `Divide` / `Modulo` (integer) with a zero divisor: undefined;
    let the runtime helper decide (or trap). FP `Divide` by zero
    DOES fold — IEEE 754 makes it well-defined (±inf or NaN).
  - Integer shifts where the count is negative or ≥ the operand's
    width: UB per C99 §6.5.7.3, and the helpers' behavior at such
    counts isn't part of the contract yet.
  - Binary ops with mismatched src1 / src2 variants: shouldn't
    happen after type checking, but bail rather than guess at the
    result width / precision.
  - `Complement` / `Modulo` / `BitwiseAnd|Or|Xor` / `LeftShift` /
    `RightShift` on FP operands: all integer-only in C; the type
    checker rejects them, but the guards stay defensive.
"""

from __future__ import annotations

import c99_ast
import fp_arith
import tac_ast


# Bit width per integer const variant — follows c6502's C99-conformant
# minimum widths (Int = 16 bits, Long = 32 bits, LongLong = 64 bits).
# FP variants are handled separately via `fp_arith` — they have
# precision, not bit width in the same sense.
_INTEGER_CONST_BITS: dict[type, int] = {
    tac_ast.ConstChar: 8,
    tac_ast.ConstUChar: 8,
    tac_ast.ConstInt: 16,
    tac_ast.ConstLong: 32,
    tac_ast.ConstLongLong: 64,
    tac_ast.ConstUInt: 16,
    tac_ast.ConstULong: 32,
    tac_ast.ConstULongLong: 64,
}

_UNSIGNED_INT_VARIANTS: tuple[type, ...] = (
    tac_ast.ConstUChar,
    tac_ast.ConstUInt, tac_ast.ConstULong, tac_ast.ConstULongLong,
)

# Inverse: TAC integer const variant for a given (bits, signed) pair.
# Width-changing folds (SignExtend / ZeroExtend / Truncate, FP→Int)
# pick the dst variant by reading both width and signedness off the
# dst's c99 type.
_INTEGER_VARIANT_FOR: dict[tuple[int, bool], type] = {
    (8, True): tac_ast.ConstChar,
    (8, False): tac_ast.ConstUChar,
    (16, True): tac_ast.ConstInt,
    (32, True): tac_ast.ConstLong,
    (64, True): tac_ast.ConstLongLong,
    (16, False): tac_ast.ConstUInt,
    (32, False): tac_ast.ConstULong,
    (64, False): tac_ast.ConstULongLong,
}


def constant_fold(
    fn: tac_ast.Function,
    *,
    symbols=None,
) -> tac_ast.Function:
    """Constant-fold each instruction. `symbols` is the type checker's
    SymbolTable (or any mapping with `.get(name) -> Symbol | None`).
    It's required to fold the integer-width casts (`SignExtend` /
    `ZeroExtend` / `Truncate`) and FP→integer conversions, since
    those need the dst Var's c99 type to pick the result variant /
    width. FP↔FP and the safe Int→FP folds work without it."""
    out: list[tac_ast.Type_instruction] = []
    for instr in fn.instructions:
        folded = _fold(instr, symbols)
        if folded is not None:
            out.append(folded)
    return tac_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=out,
    )


def _fold(
    instr: tac_ast.Type_instruction,
    symbols,
) -> tac_ast.Type_instruction | None:
    """Return the rewritten instruction, the original instruction if
    nothing folds, or None to drop the instruction (only happens for
    a JumpIf that's never taken)."""
    match instr:
        case tac_ast.Unary(
            op=op,
            src=tac_ast.Constant(const=c),
            dst=dst,
        ):
            res = _fold_unary(op, c)
            if res is None:
                return instr
            return tac_ast.Copy(src=tac_ast.Constant(const=res), dst=dst)
        case tac_ast.Binary(
            op=op,
            src1=tac_ast.Constant(const=c1),
            src2=tac_ast.Constant(const=c2),
            dst=dst,
        ):
            res = _fold_binary(op, c1, c2)
            if res is None:
                return instr
            return tac_ast.Copy(src=tac_ast.Constant(const=res), dst=dst)
        case tac_ast.Copy(
            src=tac_ast.Constant(const=c),
            dst=dst,
        ):
            res = _fold_copy(c, dst, symbols)
            if res is None:
                return instr
            return tac_ast.Copy(src=tac_ast.Constant(const=res), dst=dst)
        case tac_ast.JumpIfTrue(
            condition=tac_ast.Constant(const=c), target=t,
        ):
            tv = _truth_value(c)
            if tv is None:
                return instr
            return tac_ast.Jump(target=t) if tv else None
        case tac_ast.JumpIfFalse(
            condition=tac_ast.Constant(const=c), target=t,
        ):
            tv = _truth_value(c)
            if tv is None:
                return instr
            return None if tv else tac_ast.Jump(target=t)
        case tac_ast.IndexedLoad(
            name=name,
            index=tac_ast.Constant(const=c),
            dst=dst,
        ):
            res = _fold_indexed_load(name, c, dst, symbols)
            if res is not None:
                return res
            return instr
        case tac_ast.Phi(dst=dst, args=args) if args:
            res = _fold_phi(args, dst)
            if res is not None:
                return res
            return instr
    if isinstance(instr, _CONVERSION_NODES) and isinstance(
        instr.src, tac_ast.Constant,
    ):
        res = _fold_conversion(instr, instr.src.const, symbols)
        if res is not None:
            return tac_ast.Copy(
                src=tac_ast.Constant(const=res), dst=instr.dst,
            )
    return instr


# Cast / conversion instructions that share a (src, dst) shape and
# carry no extra fields. Listed once so `_fold` can dispatch with a
# single isinstance check.
_CONVERSION_NODES = (
    tac_ast.SignExtend, tac_ast.ZeroExtend, tac_ast.Truncate,
    tac_ast.IntToFloat, tac_ast.IntToDouble,
    tac_ast.FloatToInt, tac_ast.DoubleToInt,
    tac_ast.FloatToDouble, tac_ast.DoubleToFloat,
)


def _fold_indexed_load(
    name: str,
    idx_const: tac_ast.Type_const,
    dst: tac_ast.Type_val,
    symbols,
) -> tac_ast.Type_instruction | None:
    """Fold `IndexedLoad(name, Constant(byte_idx), dst)` into
    `Copy(Constant(value), dst)` when:
      * `name` is a static-storage object with `Initial(tuple_value)`
        (an array initialized with a constant initializer list);
      * the array's leaf element type is const-qualified (the type
        system guarantees the elements never change at runtime).
        For a multi-dim `Array(Array(... Const(scalar) ...), N)` we
        walk through every Array level looking for the Const wrapper
        at the leaf;
      * `byte_idx` is element-aligned (a fold across element
        boundaries would need byte-level slicing — deferred);
      * the addressed leaf is `int` or `float` (a Pointer-element
        array stores its addresses as ints in the init tuple, so
        Pointer leaves with link-time-numeric init values fold
        too; `AddressInit` link-time symbols still don't);
      * the dst Var's c99 type matches the leaf element type in
        width (so the fold's Constant variant matches what
        downstream consumers expect).

    Multi-dim init values are nested tuples of leaf values
    (zero-padded to the declared sizes by the type checker). We
    walk them by repeated divmod against each dimension's stride.

    Returns `Copy(Constant, dst)` on success, or None to leave the
    `IndexedLoad` alone.
    """
    if symbols is None:
        return None
    if not isinstance(dst, tac_ast.Var):
        return None
    sym = symbols.get(name)
    if sym is None:
        return None
    # Importing here to avoid a circular import at module load.
    from passes.type_checking import Initial, StaticAttr
    if not isinstance(sym.attrs, StaticAttr):
        return None
    if not isinstance(sym.attrs.initial_value, Initial):
        return None
    init_value = sym.attrs.initial_value.value
    if not isinstance(init_value, tuple):
        return None
    arr_t = sym.type
    while isinstance(arr_t, c99_ast.Const):
        arr_t = arr_t.referenced_type
    if not isinstance(arr_t, c99_ast.Array):
        return None
    # Walk through every Array nesting level, recording sizes;
    # land at the leaf element type (which must be Const-qualified
    # for the fold to be sound).
    dim_sizes: list[int] = []
    leaf_t = arr_t
    while isinstance(leaf_t, c99_ast.Array):
        dim_sizes.append(leaf_t.size)
        leaf_t = leaf_t.element_type
    if not isinstance(leaf_t, c99_ast.Const):
        return None
    leaf_t_unq = leaf_t.referenced_type
    elem_size = _scalar_size(leaf_t_unq)
    if elem_size is None:
        return None
    # `byte_idx` from the `Constant(c)` — the const variant carries
    # the value at whatever width c99_to_tac chose for the byte
    # index (typically ConstUChar for arrays ≤ 256 bytes).
    byte_idx = idx_const.value
    if byte_idx % elem_size != 0:
        return None
    elem_idx = byte_idx // elem_size
    total_elems = 1
    for d in dim_sizes:
        total_elems *= d
    if elem_idx < 0 or elem_idx >= total_elems:
        return None
    # Walk the nested init tuple by per-level stride. `remaining`
    # is the still-unspent flat element index; at each level we
    # peel off the outermost dim's contribution and descend.
    val = init_value
    remaining = elem_idx
    for level, _ in enumerate(dim_sizes):
        inner_elems = 1
        for d2 in dim_sizes[level + 1:]:
            inner_elems *= d2
        cur_idx = remaining // inner_elems
        remaining = remaining % inner_elems
        if not isinstance(val, tuple):
            return None
        if cur_idx >= len(val):
            return None
        val = val[cur_idx]
    if not isinstance(val, (int, float)):
        return None
    # Match the dst Var's c99 type — that's the result type of the
    # IndexedLoad. If dst's type doesn't match the leaf width
    # we'd need byte-level slicing; bail.
    dst_sym = symbols.get(dst.name)
    if dst_sym is None:
        return None
    dst_t = dst_sym.type
    while isinstance(dst_t, c99_ast.Const):
        dst_t = dst_t.referenced_type
    if _scalar_size(dst_t) != elem_size:
        return None
    # Build the Constant matching the dst's c99 type. Reuse the
    # c99_to_tac helper that knows the variant mapping.
    from c99_to_tac import _tac_const_for
    return tac_ast.Copy(
        src=tac_ast.Constant(const=_tac_const_for(dst_t, val)),
        dst=dst,
    )


def _scalar_size(t) -> int | None:
    """Byte width of a scalar type, or None if not a foldable
    scalar. Mirrors the size table used elsewhere in the
    optimizer; kept local to avoid a cross-module dependency."""
    if isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar)):
        return 1
    if isinstance(t, (c99_ast.Int, c99_ast.UInt, c99_ast.Pointer)):
        return 2
    if isinstance(t, (c99_ast.Long, c99_ast.ULong, c99_ast.Float)):
        return 4
    if isinstance(t, (c99_ast.LongLong, c99_ast.ULongLong, c99_ast.Double)):
        return 8
    return None


def _fold_phi(
    args: list[tac_ast.Type_phi_arg],
    dst: tac_ast.Type_val,
) -> tac_ast.Type_instruction | None:
    """If every PhiArg has the same `source` (whether all the same
    Constant or all the same Var), the Phi just copies that source
    into dst. Returns the rewritten Copy or None if the Phi's
    sources don't all agree.

    Distinct constants → leave alone (the Phi genuinely merges
    different values). The single-arg case is folded by UCE's
    `_fold_singleton_phis` to keep the rules in one place."""
    first = args[0].source
    for a in args[1:]:
        if not _vals_equal(a.source, first):
            return None
    return tac_ast.Copy(src=first, dst=dst)


def _vals_equal(a: tac_ast.Type_val, b: tac_ast.Type_val) -> bool:
    """Structural equality on Type_val: matching Constants compare by
    const variant + value; matching Vars compare by name."""
    if isinstance(a, tac_ast.Constant) and isinstance(b, tac_ast.Constant):
        return a.const == b.const
    if isinstance(a, tac_ast.Var) and isinstance(b, tac_ast.Var):
        return a.name == b.name
    return False


def _fold_conversion(
    instr: tac_ast.Type_instruction,
    c: tac_ast.Type_const,
    symbols,
) -> tac_ast.Type_const | None:
    """Try to fold a cast/conversion whose src is the Constant `c`.
    Returns the new const (the rest of `Copy(Constant(...), dst)`
    is built by the caller), or None if we can't fold."""
    match instr:
        case tac_ast.SignExtend(dst=dst):
            return _fold_widen(c, dst, symbols, sign_extend=True)
        case tac_ast.ZeroExtend(dst=dst):
            return _fold_widen(c, dst, symbols, sign_extend=False)
        case tac_ast.Truncate(dst=dst):
            return _fold_truncate(c, dst, symbols)
        case tac_ast.FloatToDouble():
            if not isinstance(c, tac_ast.ConstFloat):
                return None
            return tac_ast.ConstDouble(
                bits=fp_arith.single_bits_to_double_bits(c.bits),
            )
        case tac_ast.DoubleToFloat():
            if not isinstance(c, tac_ast.ConstDouble):
                return None
            return tac_ast.ConstFloat(
                bits=fp_arith.double_bits_to_single_bits(c.bits),
            )
        case tac_ast.IntToFloat():
            return _fold_int_to_fp(c, target_double=False)
        case tac_ast.IntToDouble():
            return _fold_int_to_fp(c, target_double=True)
        case tac_ast.FloatToInt(dst=dst):
            return _fold_fp_to_int(c, dst, symbols, source_double=False)
        case tac_ast.DoubleToInt(dst=dst):
            return _fold_fp_to_int(c, dst, symbols, source_double=True)
    return None


def _dst_int_variant(
    dst: tac_ast.Type_val, symbols,
) -> type | None:
    """Look up the dst Var's TAC integer variant (one of the six
    integer const classes), or None if the symbol table isn't
    available, the operand isn't a Var, or the c99 type isn't an
    integer kind. Width-changing integer casts and FP→integer
    conversions use this to pick the result variant."""
    if symbols is None or not isinstance(dst, tac_ast.Var):
        return None
    sym = symbols.get(dst.name)
    if sym is None:
        return None
    t = sym.type
    if isinstance(t, c99_ast.SChar):
        return tac_ast.ConstChar
    if isinstance(t, (c99_ast.Char, c99_ast.UChar)):
        # Plain `char` is unsigned in c6502.
        return tac_ast.ConstUChar
    if isinstance(t, c99_ast.Int):
        return tac_ast.ConstInt
    if isinstance(t, c99_ast.UInt):
        return tac_ast.ConstUInt
    if isinstance(t, c99_ast.Long):
        return tac_ast.ConstLong
    if isinstance(t, c99_ast.ULong):
        return tac_ast.ConstULong
    if isinstance(t, c99_ast.LongLong):
        return tac_ast.ConstLongLong
    if isinstance(t, c99_ast.ULongLong):
        return tac_ast.ConstULongLong
    return None


def _fold_widen(
    c: tac_ast.Type_const, dst: tac_ast.Type_val, symbols,
    *, sign_extend: bool,
) -> tac_ast.Type_const | None:
    """Fold SignExtend (sign_extend=True) or ZeroExtend (False) of an
    integer constant. The source's TAC variant gives us the source
    width AND signedness for canonicalization; the dst's symbol-table
    type gives us the target variant. SignExtend treats the source
    as signed at its width; ZeroExtend masks it to the source's
    unsigned bit pattern."""
    if not _is_integer_const(c):
        return None
    dst_variant = _dst_int_variant(dst, symbols)
    if dst_variant is None:
        return None
    src_bits = _INTEGER_CONST_BITS[type(c)]
    tgt_bits = _INTEGER_CONST_BITS[dst_variant]
    if tgt_bits <= src_bits:
        # Widening cast must strictly widen — c99_to_tac only emits
        # SignExtend / ZeroExtend when src_w < tgt_w. Bail rather
        # than reinterpret as a same-width or narrowing cast.
        return None
    if sign_extend:
        value = _to_signed(c.value, src_bits)
    else:
        value = c.value & ((1 << src_bits) - 1)
    return _wrap_int(dst_variant, value)


def _fold_copy(
    c: tac_ast.Type_const, dst: tac_ast.Type_val, symbols,
) -> tac_ast.Type_const | None:
    """Fold a Copy whose src is an integer Constant whose variant
    doesn't match the dst Var's c99 type. Same-width signed↔unsigned
    casts are elided in `c99_to_tac` (the bit pattern is the same),
    so the source variant can disagree with the dst's. Rewrapping
    keeps the bit pattern and recanonicalizes `.value`. Returns None
    when there's nothing to do — non-integer source, no symbol table,
    dst's c99 type isn't an integer kind we recognize, variants
    already match, or the bit widths disagree (a width-changing Copy
    shouldn't reach this pass)."""
    if not _is_integer_const(c):
        return None
    dst_variant = _dst_int_variant(dst, symbols)
    if dst_variant is None:
        return None
    if dst_variant is type(c):
        return None
    if _INTEGER_CONST_BITS[dst_variant] != _INTEGER_CONST_BITS[type(c)]:
        return None
    return _wrap_int(dst_variant, c.value)


def _fold_truncate(
    c: tac_ast.Type_const, dst: tac_ast.Type_val, symbols,
) -> tac_ast.Type_const | None:
    """Fold Truncate of an integer constant: keep the low dst-width
    bytes, canonicalize at the dst variant's signedness."""
    if not _is_integer_const(c):
        return None
    dst_variant = _dst_int_variant(dst, symbols)
    if dst_variant is None:
        return None
    src_bits = _INTEGER_CONST_BITS[type(c)]
    tgt_bits = _INTEGER_CONST_BITS[dst_variant]
    if tgt_bits >= src_bits:
        # Truncate must strictly narrow.
        return None
    return _wrap_int(dst_variant, c.value)


def _fold_int_to_fp(
    c: tac_ast.Type_const, *, target_double: bool,
) -> tac_ast.Type_const | None:
    """Fold IntToFloat / IntToDouble of an integer constant.
    Signedness rides on the operand's variant — Const{Int,Long,
    LongLong} interpret as signed, Const{UInt,ULong,ULongLong} as
    unsigned — so the conversion is unambiguous."""
    if not _is_integer_const(c):
        return None
    value = _operand_value(c)
    if target_double:
        return tac_ast.ConstDouble(bits=fp_arith.int_to_double_bits(value))
    return tac_ast.ConstFloat(bits=fp_arith.int_to_single_bits(value))


def _fold_fp_to_int(
    c: tac_ast.Type_const, dst: tac_ast.Type_val, symbols,
    *, source_double: bool,
) -> tac_ast.Type_const | None:
    """Fold FloatToInt / DoubleToInt: truncate toward zero per C99
    §6.3.1.4, store at the dst's TAC variant. Bails on NaN / ±inf —
    the C standard makes those UB and the runtime helpers' behavior
    isn't pinned."""
    if source_double:
        if not isinstance(c, tac_ast.ConstDouble):
            return None
        if not fp_arith.double_is_finite(c.bits):
            return None
        value = fp_arith.double_bits_to_int(c.bits)
    else:
        if not isinstance(c, tac_ast.ConstFloat):
            return None
        if not fp_arith.single_is_finite(c.bits):
            return None
        value = fp_arith.single_bits_to_int(c.bits)
    dst_variant = _dst_int_variant(dst, symbols)
    if dst_variant is None:
        return None
    return _wrap_int(dst_variant, value)


def _is_integer_const(c: tac_ast.Type_const) -> bool:
    return isinstance(c, tuple(_INTEGER_CONST_BITS.keys()))


def _is_unsigned_const(c: tac_ast.Type_const) -> bool:
    return isinstance(c, _UNSIGNED_INT_VARIANTS)


def _to_signed(value: int, bits: int) -> int:
    """Interpret `value` as a `bits`-wide two's-complement signed
    integer. Idempotent: a value already in the signed range is
    returned unchanged."""
    mask = (1 << bits) - 1
    value &= mask
    if value & (1 << (bits - 1)):
        return value - (1 << bits)
    return value


def _operand_value(c: tac_ast.Type_const) -> int:
    """Read the integer value of `c` interpreted at its variant's
    signedness — signed variants are signed-canonicalized, unsigned
    variants are masked to non-negative. Used by the per-op folds
    that interpret operands (comparison, FP conversion, right shift,
    truncated division/modulo)."""
    bits = _INTEGER_CONST_BITS[type(c)]
    if _is_unsigned_const(c):
        return c.value & ((1 << bits) - 1)
    return _to_signed(c.value, bits)


def _wrap_int(variant: type, value: int) -> tac_ast.Type_const:
    """Build a const of `variant` from an unbounded Python int,
    canonicalized to the variant's natural range — signed-
    canonicalized for ConstInt / ConstLong / ConstLongLong, unsigned
    bit pattern for ConstUInt / ConstULong / ConstULongLong. The
    bit pattern is identical either way; only the `.value` field's
    number form differs."""
    bits = _INTEGER_CONST_BITS[variant]
    if variant in _UNSIGNED_INT_VARIANTS:
        return variant(value=value & ((1 << bits) - 1))
    return variant(value=_to_signed(value, bits))


def _truth_value(c: tac_ast.Type_const) -> bool | None:
    """Truth value of a constant per C99 §6.3.1.2 (compares unequal
    to 0). For FP, ±0 are falsy and NaN is truthy. Returns None for
    constants we can't classify."""
    if _is_integer_const(c):
        return c.value != 0
    if isinstance(c, tac_ast.ConstFloat):
        return fp_arith.single_is_truthy(c.bits)
    if isinstance(c, tac_ast.ConstDouble):
        return fp_arith.double_is_truthy(c.bits)
    return None


def _fold_unary(
    op: tac_ast.Type_unary_operator, c: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    # LogicalNot is defined for both integer and FP operands and
    # always yields ConstInt (per C99 §6.5.3.3.5).
    if isinstance(op, tac_ast.LogicalNot):
        if _is_integer_const(c):
            return tac_ast.ConstInt(value=1 if c.value == 0 else 0)
        if isinstance(c, tac_ast.ConstFloat):
            return tac_ast.ConstInt(
                value=1 if fp_arith.single_is_zero(c.bits) else 0,
            )
        if isinstance(c, tac_ast.ConstDouble):
            return tac_ast.ConstInt(
                value=1 if fp_arith.double_is_zero(c.bits) else 0,
            )
        return None
    # Negate works on integers and FP; `Complement` (~) is
    # integer-only — the C grammar forbids `~` on FP, so we don't
    # define a meaning for FP here. Bit-pattern result is identical
    # for signed and unsigned operands (the bits negate / complement
    # the same way modulo 2^width); the operand variant only
    # determines how `.value` is canonicalized in the result.
    if isinstance(op, tac_ast.Negate):
        if _is_integer_const(c):
            variant = type(c)
            return _wrap_int(variant, -_operand_value(c))
        if isinstance(c, tac_ast.ConstFloat):
            return tac_ast.ConstFloat(bits=fp_arith.single_negate(c.bits))
        if isinstance(c, tac_ast.ConstDouble):
            return tac_ast.ConstDouble(bits=fp_arith.double_negate(c.bits))
        return None
    if isinstance(op, tac_ast.Complement):
        if not _is_integer_const(c):
            return None
        variant = type(c)
        return _wrap_int(variant, ~_operand_value(c))
    return None


def _fold_binary(
    op: tac_ast.Type_binary_operator,
    c1: tac_ast.Type_const, c2: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    # Comparisons: integer or FP, always yield ConstInt.
    if isinstance(op, (
        tac_ast.Equal, tac_ast.NotEqual,
        tac_ast.LessThan, tac_ast.GreaterThan,
        tac_ast.LessOrEqual, tac_ast.GreaterOrEqual,
    )):
        return _fold_comparison(op, c1, c2)

    # Shifts: src2 (count) may have a different width than src1.
    # Shifts are integer-only; `_fold_shift` bails on FP operands.
    if isinstance(op, (tac_ast.LeftShift, tac_ast.RightShift)):
        return _fold_shift(op, c1, c2)

    # FP arithmetic — only Add / Sub / Mul / Div are valid for FP
    # in C. The other binary ops (`%`, `& | ^`, shifts) are
    # integer-only at the C grammar level and fall through to the
    # integer path below, which will bail on FP operands.
    if isinstance(op, (tac_ast.Add, tac_ast.Subtract,
                       tac_ast.Multiply, tac_ast.Divide)):
        fp_result = _fold_fp_arith(op, c1, c2)
        if fp_result is not None:
            return fp_result

    # Integer arithmetic / bitwise: src1 and src2 share the result
    # variant. Add / Sub / Mul / And / Or / Xor produce the same
    # bit pattern for signed and unsigned operands, so we interpret
    # both as signed for the Python-level math and let `_wrap_int`
    # canonicalize per the result variant. Divide / Modulo split:
    # signed operands use truncation toward zero (C99 §6.5.5.6);
    # unsigned operands use Python's `//` and `%` (matching unsigned
    # non-negative arithmetic, which is what the future `udiv*` /
    # `umod*` helpers will compute).
    if not _is_integer_const(c1) or not _is_integer_const(c2):
        return None
    if type(c1) is not type(c2):
        return None
    variant = type(c1)
    unsigned = _is_unsigned_const(c1)
    a = _operand_value(c1)
    b = _operand_value(c2)
    match op:
        case tac_ast.Add():
            return _wrap_int(variant, a + b)
        case tac_ast.Subtract():
            return _wrap_int(variant, a - b)
        case tac_ast.Multiply():
            return _wrap_int(variant, a * b)
        case tac_ast.Divide():
            if b == 0:
                return None
            if unsigned:
                return _wrap_int(variant, a // b)
            return _wrap_int(variant, _trunc_div(a, b))
        case tac_ast.Modulo():
            if b == 0:
                return None
            if unsigned:
                return _wrap_int(variant, a % b)
            return _wrap_int(variant, _trunc_mod(a, b))
        case tac_ast.BitwiseAnd():
            return _wrap_int(variant, a & b)
        case tac_ast.BitwiseOr():
            return _wrap_int(variant, a | b)
        case tac_ast.BitwiseXor():
            return _wrap_int(variant, a ^ b)
    return None


def _fold_fp_arith(
    op: tac_ast.Type_binary_operator,
    c1: tac_ast.Type_const, c2: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    """IEEE 754 arithmetic at the operand precision. Returns None
    if either operand isn't FP, or if the variants don't match
    (mismatched precision shouldn't happen post-type-check)."""
    if isinstance(c1, tac_ast.ConstFloat) and isinstance(
        c2, tac_ast.ConstFloat,
    ):
        match op:
            case tac_ast.Add():
                return tac_ast.ConstFloat(
                    bits=fp_arith.single_add(c1.bits, c2.bits),
                )
            case tac_ast.Subtract():
                return tac_ast.ConstFloat(
                    bits=fp_arith.single_sub(c1.bits, c2.bits),
                )
            case tac_ast.Multiply():
                return tac_ast.ConstFloat(
                    bits=fp_arith.single_mul(c1.bits, c2.bits),
                )
            case tac_ast.Divide():
                # IEEE 754 division by zero is well-defined: ±inf
                # for nonzero numerator, NaN for 0/0.
                return tac_ast.ConstFloat(
                    bits=fp_arith.single_div(c1.bits, c2.bits),
                )
        return None
    if isinstance(c1, tac_ast.ConstDouble) and isinstance(
        c2, tac_ast.ConstDouble,
    ):
        match op:
            case tac_ast.Add():
                return tac_ast.ConstDouble(
                    bits=fp_arith.double_add(c1.bits, c2.bits),
                )
            case tac_ast.Subtract():
                return tac_ast.ConstDouble(
                    bits=fp_arith.double_sub(c1.bits, c2.bits),
                )
            case tac_ast.Multiply():
                return tac_ast.ConstDouble(
                    bits=fp_arith.double_mul(c1.bits, c2.bits),
                )
            case tac_ast.Divide():
                return tac_ast.ConstDouble(
                    bits=fp_arith.double_div(c1.bits, c2.bits),
                )
        return None
    return None


def _trunc_div(a: int, b: int) -> int:
    """C99 §6.5.5.6 integer division: truncate toward zero. (Python's
    `//` truncates toward negative infinity, so we can't use it
    directly when signs differ.)"""
    q = abs(a) // abs(b)
    if (a < 0) != (b < 0):
        q = -q
    return q


def _trunc_mod(a: int, b: int) -> int:
    """C99 §6.5.5.6 modulo: a - (a/b)*b, with `/` being truncation
    toward zero. Sign of the result matches the dividend."""
    return a - _trunc_div(a, b) * b


def _fold_shift(
    op: tac_ast.Type_binary_operator,
    c1: tac_ast.Type_const, c2: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    if not _is_integer_const(c1) or not _is_integer_const(c2):
        return None
    variant = type(c1)
    bits = _INTEGER_CONST_BITS[variant]
    # The count's width is its own variant's width — can differ
    # from the value's width. The count is always interpreted as a
    # non-negative integer (a negative count is UB per §6.5.7.3),
    # so an unsigned operand-value reading works for both signed
    # and unsigned count variants.
    count = _operand_value(c2)
    if count < 0 or count >= bits:
        # C99 §6.5.7.3: undefined. The c6502 helpers (asl8/16/32
        # and asr8/16/32) explicitly document this case as UB too.
        return None
    match op:
        case tac_ast.LeftShift():
            # Bit pattern is the same for signed / unsigned shifts.
            # Use the operand's signed value to keep small literal
            # printouts simple; `_wrap_int` canonicalizes per the
            # variant's signedness.
            return _wrap_int(variant, _operand_value(c1) << count)
        case tac_ast.RightShift():
            if _is_unsigned_const(c1):
                # Logical right shift — matches `lsr*`. Operand value
                # is already non-negative, so Python's `>>` zero-fills
                # naturally.
                return _wrap_int(variant, _operand_value(c1) >> count)
            # Arithmetic right shift — matches `asr*`. Python's `>>`
            # is sign-preserving on negative ints already.
            return _wrap_int(variant, _to_signed(c1.value, bits) >> count)
    return None


def _fold_comparison(
    op: tac_ast.Type_binary_operator,
    c1: tac_ast.Type_const, c2: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    if type(c1) is not type(c2):
        return None
    if isinstance(c1, tac_ast.ConstFloat):
        ord_ = fp_arith.single_compare(c1.bits, c2.bits)
        return _fp_comparison_result(op, ord_)
    if isinstance(c1, tac_ast.ConstDouble):
        ord_ = fp_arith.double_compare(c1.bits, c2.bits)
        return _fp_comparison_result(op, ord_)
    if not _is_integer_const(c1):
        return None
    # Operand interpretation follows the variant's signedness —
    # signed for Const{Int,Long,LongLong}, unsigned for
    # Const{UInt,ULong,ULongLong}. Matches `tac_to_asm`'s ordering
    # dispatch (V-corrected MI/PL for signed, BCC/BCS for unsigned).
    a = _operand_value(c1)
    b = _operand_value(c2)
    match op:
        case tac_ast.Equal():
            r = a == b
        case tac_ast.NotEqual():
            r = a != b
        case tac_ast.LessThan():
            r = a < b
        case tac_ast.GreaterThan():
            r = a > b
        case tac_ast.LessOrEqual():
            r = a <= b
        case tac_ast.GreaterOrEqual():
            r = a >= b
        case _:
            return None
    return tac_ast.ConstInt(value=1 if r else 0)


def _fp_comparison_result(
    op: tac_ast.Type_binary_operator, order: str,
) -> tac_ast.Type_const | None:
    """Map an `fp_arith.*_compare` outcome (`lt` / `eq` / `gt` /
    `unordered`) plus a TAC comparison op to a ConstInt 0/1 result.
    Per IEEE 754: any comparison against NaN is unordered; equality
    treats it as not-equal (so `==` → 0, `!=` → 1), and all four
    relational operators return false."""
    match op:
        case tac_ast.Equal():
            r = order == "eq"
        case tac_ast.NotEqual():
            r = order != "eq"
        case tac_ast.LessThan():
            r = order == "lt"
        case tac_ast.GreaterThan():
            r = order == "gt"
        case tac_ast.LessOrEqual():
            r = order in ("lt", "eq")
        case tac_ast.GreaterOrEqual():
            r = order in ("gt", "eq")
        case _:
            return None
    return tac_ast.ConstInt(value=1 if r else 0)
