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
    handles a similar but slightly broader shape (also accepts
    `&staticobj`); it can delegate the integer / FP path here once
    that refactor lands. Not done in this PR.

Today's coverage
----------------
This module accepts the simplest shape that the writing-a-c-compiler-
tests corpus exercises for case labels: a single `Constant` literal,
optionally wrapped in any number of `Cast` nodes whose target types
are integer (introduced by either user-written casts or the type
checker's implicit conversion to the switch's promoted type). A
non-integer cast target — float / double / pointer — is rejected as
non-integer. Anything else (Var, Binary, Unary, Conditional, ...) is
rejected as "not a constant expression"; expanding to general
arithmetic on integer constants is a follow-up that drops in via
recursion into Unary / Binary / Conditional arms.

Integer-typed cast values are converted to the cast's target type
modulo its width — same rule as the runtime cast lowering in
c99_to_tac (`Truncate` / `SignExtend` / `ZeroExtend` are byte-level
operations that boil down to width-modular arithmetic on the source
integer value).
"""

from __future__ import annotations

import c99_ast


class ConstantExpressionError(Exception):
    """Raised when an expression required to be a constant expression
    (per C99 §6.6) doesn't satisfy the constraints of that section."""


# Width / signedness of the four integer types c6502 models. Mirrors
# `passes.type_checking._int_width` / `_is_signed` but kept local so
# this module can be imported by the type checker without creating a
# cycle.
_INT_WIDTH = {
    c99_ast.Int:   1,
    c99_ast.UInt:  1,
    c99_ast.Long:  2,
    c99_ast.ULong: 2,
}
_SIGNED = (c99_ast.Int, c99_ast.Long)


def _is_integer_type(t: c99_ast.Type_data_type) -> bool:
    return isinstance(t, (c99_ast.Int, c99_ast.Long,
                          c99_ast.UInt, c99_ast.ULong))


def _coerce_to_integer_type(value: int, t: c99_ast.Type_data_type) -> int:
    """Reduce `value` mod 2**(8*width) and re-interpret with the
    target's signedness — matches the runtime byte-level cast
    sequences in tac_to_asm (Truncate / SignExtend / ZeroExtend all
    boil down to this for compile-time-known integer values)."""
    width_bits = 8 * _INT_WIDTH[type(t)]
    mask = (1 << width_bits) - 1
    raw = value & mask
    if isinstance(t, _SIGNED) and raw & (1 << (width_bits - 1)):
        raw -= 1 << width_bits
    return raw


def _const_int_value(c: c99_ast.Type_const) -> int:
    """Pull the integer value out of a `Type_const` integer variant.
    All four variants store the bit pattern as a non-negative `int`
    field; the variant tags signedness for downstream consumers."""
    if isinstance(c, (c99_ast.ConstInt, c99_ast.ConstLong,
                      c99_ast.ConstUInt, c99_ast.ConstULong)):
        return c.int
    raise ConstantExpressionError(
        f"floating constant {c!r} is not an integer constant"
    )


def _const_type(c: c99_ast.Type_const) -> c99_ast.Type_data_type:
    """Map a `Type_const` integer variant to its c99 data_type."""
    if isinstance(c, c99_ast.ConstInt):
        return c99_ast.Int()
    if isinstance(c, c99_ast.ConstLong):
        return c99_ast.Long()
    if isinstance(c, c99_ast.ConstUInt):
        return c99_ast.UInt()
    if isinstance(c, c99_ast.ConstULong):
        return c99_ast.ULong()
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

    Today's accepted shapes:
      * `Constant(ConstInt | ConstLong | ConstUInt | ConstULong)` —
        the four integer literal variants.
      * `Cast(target_type=integer, exp=integer_constant_expression)`
        recursively — the target's signedness/width is applied
        modulo the byte width.

    `Cast(target_type=Float | Double | Pointer | ...)` is rejected
    because §6.6.6 requires integer type. A floating-point
    `Constant` is similarly rejected.
    """
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
            return _coerce_to_integer_type(inner_value, target), target
    raise ConstantExpressionError(
        f"expression is not an integer constant expression: {exp!r}"
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
