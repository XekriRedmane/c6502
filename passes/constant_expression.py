"""Constant-expression validator and integer folder (C99 §6.6).

C99 §6.6 defines two flavors of constant expression:

  * `constant-expression` (§6.6.3): "shall not contain assignment,
    increment, decrement, function-call, or comma operators, except
    when they are contained within a subexpression that is not
    evaluated."  Integer / FP / address constant expressions all share
    this restriction.

  * `integer constant expression` (§6.6.6): the strictest form. "shall
    have integer type and shall only have operands that are integer
    constants, enumeration constants, character constants, sizeof
    expressions whose results are integer constants, _Alignof
    expressions, and floating constants that are the immediate
    operands of casts." Used in case labels (§6.8.4.2.3), enum
    constants (§6.7.2.2.2), bit-field widths (§6.7.2.1.4), array sizes
    for non-VLA arrays (§6.7.5.2.4), and the `#if` / `#elif`
    preprocessor directives (§6.10.1.4).

The module exposes two entry points:

  * `evaluate_integer_constant_expression(exp)` — for §6.6.6 sites.
    Returns `(value, type)` where `value` is a Python `int` and
    `type` is a c99_ast integer data_type. Raises
    `ConstantExpressionError` for non-constant or non-integer-typed
    operands.

  * `validate_constant_expression(exp)` — for §6.6.3 sites that don't
    need a folded value (mostly hooks for future features; the only
    current call site that uses constant expressions is case-label
    integer folding). Walks the expression and rejects the §6.6.3
    forbidden operators.

Reuse map
---------
The validator + integer folder is reusable across:
  * case labels (today)
  * enum constants (future, if/when enums land)
  * array sizes for non-VLA arrays (future — currently the parser
    accepts only integer literals; widening to constant_exp drops in
    here)
  * bit-field widths (future, if/when struct support lands)
  * `_Alignas` / `_Static_assert` (future)
  * static initializers — `passes.type_checking._const_init_value`
    handles Constants, Casts, and `&staticobj` itself and delegates
    integer-typed operator expressions here.

Coverage
--------
The integer folder accepts integer and char literals, integer
casts, `sizeof`, the unary operators `-` / `~` / `!`, every binary
operator other than assignment, and `?:`. It relies on the type
checker's `data_type` stamps and implicit Casts, so callers type-
check the expression first. Floating operands are rejected. The
static-initializer path in `passes.type_checking._const_init_value`
handles floating constants on its own and calls this folder for
integer-typed operator expressions.

Integer-typed cast values are converted to the cast's target type
modulo its width — same rule as the runtime cast lowering in
c99_to_tac (`Truncate` / `SignExtend` / `ZeroExtend` are byte-level
operations that boil down to width-modular arithmetic on the source
integer value).

`sizeof` folds to the byte size of its operand type as an
`unsigned long` (size_t in c6502). For `sizeof (T)` the type comes
from the AST node directly; for `sizeof e` the type comes from the
inner expression's `data_type`, which the type checker must have
stamped before this evaluator runs (the case-label call site
type-checks each value first). Per C99 §6.5.3.4.2 the inner is not
evaluated, so we don't recurse into it for the §6.6.3 forbidden-
operator check either.
"""

from __future__ import annotations

import c99_ast


class ConstantExpressionError(Exception):
    """Raised when an expression required to be a constant expression
    (per C99 §6.6) doesn't satisfy the constraints of that section."""


# Width / signedness of the six integer types c6502 models. Mirrors
# `passes.type_checking._int_width` / `_is_signed` but kept local so
# this module can be imported by the type checker without creating a
# cycle.
_INT_WIDTH = {
    c99_ast.Char:      1,
    c99_ast.SChar:     1,
    c99_ast.UChar:     1,
    c99_ast.Bool:      1,
    c99_ast.Int:       2,
    c99_ast.UInt:      2,
    c99_ast.Long:      4,
    c99_ast.ULong:     4,
    c99_ast.LongLong:  8,
    c99_ast.ULongLong: 8,
}
# Plain `char` is unsigned in c6502 (C99 §6.2.5.15 leaves the choice
# to the implementation), so only `signed char` joins the signed set.
_SIGNED = (c99_ast.Int, c99_ast.Long, c99_ast.LongLong, c99_ast.SChar)


def _strip_quals(t: c99_ast.Type_data_type) -> c99_ast.Type_data_type:
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return t


def _is_integer_type(t: c99_ast.Type_data_type) -> bool:
    return type(_strip_quals(t)) in _INT_WIDTH


def _sizeof(t: c99_ast.Type_data_type) -> int:
    """Bytes occupied by a value of type `t` in c6502's storage
    model. Mirrors the helpers of the same name in
    `passes.type_checking` and `c99_to_tac`; kept local so this
    module imports nothing from either of those (which would be a
    cycle in the case of type_checking, since type_checking imports
    this module). Raises ConstantExpressionError on incomplete /
    function types — `sizeof(void)` / `sizeof(function-type)` is
    flagged at the type-check boundary too, but a defense in depth
    here keeps the const-evaluator honest if a future caller skips
    the type check."""
    t = _strip_quals(t)
    if isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar,
                      c99_ast.Bool)):
        return 1
    if isinstance(t, (c99_ast.Int, c99_ast.UInt, c99_ast.Pointer)):
        return 2
    if isinstance(t, (c99_ast.Long, c99_ast.ULong, c99_ast.Float)):
        return 4
    if isinstance(t, (c99_ast.LongLong, c99_ast.ULongLong, c99_ast.Double)):
        return 8
    if isinstance(t, c99_ast.Array):
        return _sizeof(t.element_type) * t.size
    raise ConstantExpressionError(
        f"sizeof of incomplete or function type: {t!r}"
    )


def _coerce_to_integer_type(value: int, t: c99_ast.Type_data_type) -> int:
    """Reduce `value` mod 2**(8*width) and re-interpret with the
    target's signedness — matches the runtime byte-level cast
    sequences in tac_to_asm (Truncate / SignExtend / ZeroExtend all
    boil down to this for compile-time-known integer values). A
    `_Bool` target gets 0 or 1 instead (C99 §6.3.1.2)."""
    t = _strip_quals(t)
    if isinstance(t, c99_ast.Bool):
        return int(value != 0)
    width_bits = 8 * _INT_WIDTH[type(t)]
    mask = (1 << width_bits) - 1
    raw = value & mask
    if isinstance(t, _SIGNED) and raw & (1 << (width_bits - 1)):
        raw -= 1 << width_bits
    return raw


def _const_int_value(c: c99_ast.Type_const) -> int:
    """Pull the integer value out of a `Type_const` integer variant.
    Every integer variant stores the bit pattern as a non-negative
    `int` field; the variant tags signedness for downstream consumers."""
    if isinstance(c, (c99_ast.ConstInt, c99_ast.ConstLong,
                      c99_ast.ConstLongLong,
                      c99_ast.ConstUInt, c99_ast.ConstULong,
                      c99_ast.ConstULongLong,
                      c99_ast.ConstChar, c99_ast.ConstUChar)):
        return _coerce_to_integer_type(c.value, _const_type(c))
    raise ConstantExpressionError(
        f"floating constant {c!r} is not an integer constant"
    )


def _const_type(c: c99_ast.Type_const) -> c99_ast.Type_data_type:
    """Map a `Type_const` integer variant to its c99 data_type."""
    if isinstance(c, c99_ast.ConstInt):
        return c99_ast.Int()
    if isinstance(c, c99_ast.ConstLong):
        return c99_ast.Long()
    if isinstance(c, c99_ast.ConstLongLong):
        return c99_ast.LongLong()
    if isinstance(c, c99_ast.ConstUInt):
        return c99_ast.UInt()
    if isinstance(c, c99_ast.ConstULong):
        return c99_ast.ULong()
    if isinstance(c, c99_ast.ConstULongLong):
        return c99_ast.ULongLong()
    if isinstance(c, c99_ast.ConstChar):
        return c99_ast.SChar()
    if isinstance(c, c99_ast.ConstUChar):
        return c99_ast.UChar()
    raise ConstantExpressionError(
        f"non-integer constant {c!r} has no integer type"
    )


def evaluate_integer_constant_expression(
    exp: c99_ast.Type_exp,
) -> tuple[int, c99_ast.Type_data_type]:
    """C99 §6.6.6 integer constant expression. Returns
    `(value, integer_type)` for an expression that resolves to a
    compile-time known integer value, or raises
    `ConstantExpressionError` if `exp` doesn't satisfy §6.6.6.

    Accepted shapes:
      * An integer `Constant` (any integer or char variant).
      * `Cast(target_type=integer, exp=...)`, applied modulo the
        target's width with the target's signedness.
      * `SizeOfType(T)` / `SizeOfExp(e)`, which fold to ULong.
      * `Unary` (`-`, `~`, `!`), `Binary` (arithmetic, bitwise,
        shift, comparison, `&&`, `||`), and `Conditional` over
        integer constant expressions.

    The operator arms read each node's `data_type`, so the caller
    must type-check `exp` first. By then the type checker has
    inserted the implicit Casts for integer promotion and the usual
    arithmetic conversions, so each operator computes in its own
    result type and its operands already have the right width and
    signedness. Values are Python ints in the range of their type
    (negative for a negative signed value).

    Division or remainder by zero, a shift count that is negative or
    not less than the width, a left shift of a negative value, and
    signed overflow are undefined behavior (C99 §6.5.5.5, §6.5.7.3,
    §6.5.7.4, §6.5.5) and are reported as not being constant
    expressions. `Cast(target_type=Float | Double | Pointer | ...)`
    and floating `Constant`s are rejected because §6.6.6 requires
    integer type."""
    match exp:
        case c99_ast.Constant(const=c):
            return _const_int_value(c), _const_type(c)
        case c99_ast.Cast(target_type=target, exp=inner):
            if not _is_integer_type(target):
                raise ConstantExpressionError(
                    f"integer constant expression cannot have a non-"
                    f"integer cast target ({target!r}); §6.6.6 requires "
                    f"integer type"
                )
            inner_value, _inner_type = evaluate_integer_constant_expression(
                inner,
            )
            return _coerce_to_integer_type(inner_value, target), _strip_quals(target)
        case c99_ast.SizeOfType(target_type=t):
            return _sizeof(t), c99_ast.ULong()
        case c99_ast.SizeOfExp(exp=inner):
            inner_t = inner.data_type
            if inner_t is None:
                raise ConstantExpressionError(
                    "sizeof's inner expression has no data_type — "
                    "the constant evaluator requires the operand to "
                    "be type-checked first"
                )
            return _sizeof(inner_t), c99_ast.ULong()
        case c99_ast.Unary(op=op, exp=inner):
            t = _result_type(exp)
            v, _ = evaluate_integer_constant_expression(inner)
            if isinstance(op, c99_ast.LogicalNot):
                return int(v == 0), t
            if isinstance(op, c99_ast.Negate):
                return _checked(-v, t), t
            return _coerce_to_integer_type(~v, t), t
        case c99_ast.Binary(op=op, left=lhs, right=rhs):
            return _evaluate_binary(exp, op, lhs, rhs)
        case c99_ast.Conditional(
            condition=cond, true_clause=tc, false_clause=fc,
        ):
            t = _result_type(exp)
            c, _ = evaluate_integer_constant_expression(cond)
            v, _ = evaluate_integer_constant_expression(tc if c else fc)
            return _coerce_to_integer_type(v, t), t
    raise ConstantExpressionError(
        f"expression is not an integer constant expression: {exp!r}"
    )


def _result_type(exp: c99_ast.Type_exp) -> c99_ast.Type_data_type:
    """`exp`'s type-checker-stamped integer type, or an error when
    it is missing or not an integer type."""
    t = exp.data_type
    if t is None:
        raise ConstantExpressionError(
            "operator in a constant expression has no data_type — "
            "the constant evaluator requires the expression to be "
            "type-checked first"
        )
    if not _is_integer_type(t):
        raise ConstantExpressionError(
            f"integer constant expression has non-integer type {t!r}"
        )
    return _strip_quals(t)


def _checked(value: int, t: c99_ast.Type_data_type) -> int:
    """`value` as a value of type `t`. Unsigned arithmetic wraps
    (C99 §6.2.5.9). A signed result outside `t`'s range is overflow,
    which is undefined, so it is rejected."""
    wrapped = _coerce_to_integer_type(value, t)
    if isinstance(t, _SIGNED) and wrapped != value:
        raise ConstantExpressionError(
            f"signed overflow in constant expression ({value} does "
            f"not fit in {t!r})"
        )
    return wrapped


def _evaluate_binary(
    exp: c99_ast.Type_exp, op, lhs: c99_ast.Type_exp,
    rhs: c99_ast.Type_exp,
) -> tuple[int, c99_ast.Type_data_type]:
    """Folds one `Binary` node. See
    `evaluate_integer_constant_expression` for the rules."""
    t = _result_type(exp)
    a, _ = evaluate_integer_constant_expression(lhs)
    # `&&` / `||` don't evaluate the right operand when the left one
    # decides the result (C99 §6.5.13.4 / §6.5.14.4).
    if isinstance(op, c99_ast.LogicalAnd):
        if not a:
            return 0, t
        b, _ = evaluate_integer_constant_expression(rhs)
        return int(b != 0), t
    if isinstance(op, c99_ast.LogicalOr):
        if a:
            return 1, t
        b, _ = evaluate_integer_constant_expression(rhs)
        return int(b != 0), t
    b, _ = evaluate_integer_constant_expression(rhs)
    match op:
        case c99_ast.Equal():
            return int(a == b), t
        case c99_ast.NotEqual():
            return int(a != b), t
        case c99_ast.LessThan():
            return int(a < b), t
        case c99_ast.GreaterThan():
            return int(a > b), t
        case c99_ast.LessOrEqual():
            return int(a <= b), t
        case c99_ast.GreaterOrEqual():
            return int(a >= b), t
        case c99_ast.LeftShift() | c99_ast.RightShift():
            if b < 0 or b >= 8 * _INT_WIDTH[type(t)]:
                raise ConstantExpressionError(
                    f"shift count {b} out of range in constant expression"
                )
            if isinstance(op, c99_ast.RightShift):
                # A negative left operand shifts arithmetically, which
                # is c6502's implementation-defined choice (asr*).
                return _coerce_to_integer_type(a >> b, t), t
            if isinstance(t, _SIGNED) and a < 0:
                raise ConstantExpressionError(
                    "left shift of a negative value in constant expression"
                )
            return _checked(a << b, t), t
        case c99_ast.Divide() | c99_ast.Modulo():
            if b == 0:
                raise ConstantExpressionError(
                    "division by zero in constant expression"
                )
            # C99 §6.5.5.6: the quotient truncates toward zero.
            q = abs(a) // abs(b)
            if (a < 0) != (b < 0):
                q = -q
            r = q if isinstance(op, c99_ast.Divide) else a - q * b
            return _checked(r, t), t
        case c99_ast.Add():
            return _checked(a + b, t), t
        case c99_ast.Subtract():
            return _checked(a - b, t), t
        case c99_ast.Multiply():
            return _checked(a * b, t), t
        case c99_ast.BitwiseAnd():
            return _coerce_to_integer_type(a & b, t), t
        case c99_ast.BitwiseOr():
            return _coerce_to_integer_type(a | b, t), t
        case c99_ast.BitwiseXor():
            return _coerce_to_integer_type(a ^ b, t), t
    raise ConstantExpressionError(
        f"operator {op!r} is not allowed in an integer constant expression"
    )


def validate_constant_expression(exp: c99_ast.Type_exp) -> None:
    """C99 §6.6.3 constant expression. Walks `exp` and raises
    `ConstantExpressionError` if it contains any of the operators
    forbidden by §6.6.3 (assignment / increment / decrement / function-
    call / comma — c6502 has no comma operator yet, so it's not
    listed here). Doesn't fold or return a value — call this for
    sites that need the §6.6.3 check without an integer-folding
    requirement.

    Currently unused at any call site; it exists for the upcoming
    enum / array-size / bit-field-width call sites that share §6.6.3
    semantics. The case-label path uses
    `evaluate_integer_constant_expression`, which subsumes §6.6.6
    (the strictest form)."""
    match exp:
        case c99_ast.Assignment():
            raise ConstantExpressionError(
                "assignment is not allowed in a constant expression"
            )
        case c99_ast.CompoundAssignment():
            raise ConstantExpressionError(
                "compound assignment is not allowed in a constant expression"
            )
        case c99_ast.Postfix():
            raise ConstantExpressionError(
                "postfix increment/decrement is not allowed in a "
                "constant expression"
            )
        case c99_ast.FunctionCall():
            raise ConstantExpressionError(
                "function call is not allowed in a constant expression"
            )
        case c99_ast.Constant() | c99_ast.Var():
            return
        case c99_ast.SizeOfExp() | c99_ast.SizeOfType():
            # sizeof's operand is "a subexpression that is not
            # evaluated" per C99 §6.5.3.4.2, so the §6.6.3 forbidden-
            # operator rule explicitly does NOT apply inside it
            # (§6.6.3: "except when they are contained within a
            # subexpression that is not evaluated"). Treat sizeof as
            # a leaf and don't recurse — `sizeof(i++)` is a valid
            # constant expression.
            return
        case c99_ast.Cast(exp=inner):
            validate_constant_expression(inner)
            return
        case c99_ast.Unary(exp=inner) | c99_ast.Dereference(exp=inner) | c99_ast.AddressOf(exp=inner):
            validate_constant_expression(inner)
            return
        case c99_ast.Binary(left=lhs, right=rhs):
            validate_constant_expression(lhs)
            validate_constant_expression(rhs)
            return
        case c99_ast.Conditional(
            condition=cond, true_clause=t, false_clause=f,
        ):
            validate_constant_expression(cond)
            validate_constant_expression(t)
            validate_constant_expression(f)
            return
        case c99_ast.Subscript(array=arr, index=idx):
            validate_constant_expression(arr)
            validate_constant_expression(idx)
            return
        case c99_ast.InitList(items=items):
            for it in items:
                validate_constant_expression(it)
            return
    raise ConstantExpressionError(
        f"unexpected expression in constant-expression validation: {exp!r}"
    )
