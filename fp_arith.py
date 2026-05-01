"""IEEE 754 bit-pattern adapter, on top of numpy.

c6502 carries floating-point values as their IEEE 754 bit patterns —
not as Python `float`s — at every layer from the c99 AST through asm
emission. This module is the single source of truth for converting
between numbers and those bit patterns. Going through numpy avoids:

  - Python `float`'s double-precision intermediary, which can cause
    double-rounding when the eventual target is single precision.
  - Python `float()`'s brittle behavior at the IEEE 754 boundaries:
    overflow raises (vs. our wanted ±inf), and edge cases like the
    `1e30` ULP disagreement that the `ieee754` PyPI package exposed
    when we evaluated it.

numpy gives us correctly-rounded conversion at the target precision
for every IEEE 754 case (subnormals, overflow → ±inf, NaN, ±0,
denormals).

Bit-pattern shape:
  - Single precision: 32-bit pattern as a Python int in 0..2^32-1.
  - Double precision: 64-bit pattern as a Python int in 0..2^64-1.

When FP arithmetic / comparisons land for constant folding, this
module is where `single_add` / `single_compare` / etc. show up too;
keeping the surface here means consumers don't need to know about
numpy.
"""

from __future__ import annotations

import numpy as np


_SINGLE_MASK = (1 << 32) - 1
_DOUBLE_MASK = (1 << 64) - 1


def single_string_to_bits(s: str) -> int:
    """Parse a decimal string `s` as IEEE 754 single precision and
    return its 32-bit pattern. Overflow → ±inf, underflow → ±0,
    subnormals preserved."""
    # numpy emits a RuntimeWarning on overflow-to-inf; that's the
    # behavior we want, just silence the noise.
    with np.errstate(over="ignore"):
        v = np.float32(s)
    return int(v.view(np.uint32))


def double_string_to_bits(s: str) -> int:
    """Parse a decimal string `s` as IEEE 754 double precision and
    return its 64-bit pattern."""
    with np.errstate(over="ignore"):
        v = np.float64(s)
    return int(v.view(np.uint64))


def int_to_single_bits(value: int) -> int:
    """Convert a Python int (any width) to IEEE 754 single bits.
    Routes through string conversion so we never let Python's
    float() narrowing get in the way."""
    return single_string_to_bits(str(value))


def int_to_double_bits(value: int) -> int:
    """Convert a Python int (any width) to IEEE 754 double bits."""
    return double_string_to_bits(str(value))


def single_bits_to_int(bits: int) -> int:
    """Convert IEEE 754 single bits to a Python int via C99-style
    truncation toward zero. The result is unbounded — caller masks
    to the target integer type's width.

    Raises on NaN / ±inf, matching Python's `int(float)` behavior."""
    v = np.uint32(bits & _SINGLE_MASK).view(np.float32)
    return int(v)


def double_bits_to_int(bits: int) -> int:
    """Convert IEEE 754 double bits to a Python int via truncation
    toward zero. Raises on NaN / ±inf."""
    v = np.uint64(bits & _DOUBLE_MASK).view(np.float64)
    return int(v)


def single_bits_to_double_bits(bits: int) -> int:
    """Widen IEEE 754 single bits to double bits. Lossless: every
    finite single is exactly representable as a double."""
    s = np.uint32(bits & _SINGLE_MASK).view(np.float32)
    d = np.float64(s)
    return int(d.view(np.uint64))


def double_bits_to_single_bits(bits: int) -> int:
    """Narrow IEEE 754 double bits to single bits. Round-to-nearest-
    even at the single-precision boundary; overflow → ±inf;
    underflow → subnormal or ±0."""
    d = np.uint64(bits & _DOUBLE_MASK).view(np.float64)
    with np.errstate(over="ignore"):
        s = np.float32(d)
    return int(s.view(np.uint32))


# ---------------------------------------------------------------------
# Sign manipulation. Exact (no rounding); preserves NaN payloads.
# ---------------------------------------------------------------------

_SINGLE_SIGN = 1 << 31
_DOUBLE_SIGN = 1 << 63


def single_negate(bits: int) -> int:
    """Flip the sign bit of an IEEE 754 single. +0 ↔ -0, NaN payload
    preserved (only the sign flips)."""
    return (bits & _SINGLE_MASK) ^ _SINGLE_SIGN


def double_negate(bits: int) -> int:
    """Flip the sign bit of an IEEE 754 double."""
    return (bits & _DOUBLE_MASK) ^ _DOUBLE_SIGN


# ---------------------------------------------------------------------
# Arithmetic. Operations run at the operand precision via numpy, with
# correctly-rounded round-to-nearest-even, IEEE 754 special-value
# semantics (NaN propagation, inf, ±0), and overflow → ±inf. The
# `np.errstate` context silences numpy's runtime warnings — those
# behaviors (overflow, division by zero, invalid → NaN) are exactly
# what we want from the eventual runtime helpers.
# ---------------------------------------------------------------------


def _single_arith(a_bits: int, b_bits: int, op) -> int:
    a = np.uint32(a_bits & _SINGLE_MASK).view(np.float32)
    b = np.uint32(b_bits & _SINGLE_MASK).view(np.float32)
    with np.errstate(over="ignore", divide="ignore", invalid="ignore"):
        r = op(a, b)
    # Re-narrow: numpy may auto-promote some op results to float64;
    # explicit cast forces single precision.
    return int(np.float32(r).view(np.uint32))


def _double_arith(a_bits: int, b_bits: int, op) -> int:
    a = np.uint64(a_bits & _DOUBLE_MASK).view(np.float64)
    b = np.uint64(b_bits & _DOUBLE_MASK).view(np.float64)
    with np.errstate(over="ignore", divide="ignore", invalid="ignore"):
        r = op(a, b)
    return int(np.float64(r).view(np.uint64))


def single_add(a: int, b: int) -> int:
    return _single_arith(a, b, lambda x, y: x + y)


def single_sub(a: int, b: int) -> int:
    return _single_arith(a, b, lambda x, y: x - y)


def single_mul(a: int, b: int) -> int:
    return _single_arith(a, b, lambda x, y: x * y)


def single_div(a: int, b: int) -> int:
    return _single_arith(a, b, lambda x, y: x / y)


def double_add(a: int, b: int) -> int:
    return _double_arith(a, b, lambda x, y: x + y)


def double_sub(a: int, b: int) -> int:
    return _double_arith(a, b, lambda x, y: x - y)


def double_mul(a: int, b: int) -> int:
    return _double_arith(a, b, lambda x, y: x * y)


def double_div(a: int, b: int) -> int:
    return _double_arith(a, b, lambda x, y: x / y)


# ---------------------------------------------------------------------
# Comparison and predicates. The compare functions return one of
# "lt" / "eq" / "gt" / "unordered" — explicit tag instead of -1/0/1
# so callers can't accidentally collapse "unordered" with "lt" or
# "gt". For C99 §6.5.8.5: equality returns true if `+0 == -0`; all
# relational ordering is unordered when either operand is NaN.
# ---------------------------------------------------------------------


def single_compare(a_bits: int, b_bits: int) -> str:
    a = np.uint32(a_bits & _SINGLE_MASK).view(np.float32)
    b = np.uint32(b_bits & _SINGLE_MASK).view(np.float32)
    if np.isnan(a) or np.isnan(b):
        return "unordered"
    if a < b:
        return "lt"
    if a > b:
        return "gt"
    return "eq"


def double_compare(a_bits: int, b_bits: int) -> str:
    a = np.uint64(a_bits & _DOUBLE_MASK).view(np.float64)
    b = np.uint64(b_bits & _DOUBLE_MASK).view(np.float64)
    if np.isnan(a) or np.isnan(b):
        return "unordered"
    if a < b:
        return "lt"
    if a > b:
        return "gt"
    return "eq"


def single_is_zero(bits: int) -> bool:
    """True iff the single-precision value compares equal to 0
    (i.e. +0 or -0). NaN returns False."""
    a = np.uint32(bits & _SINGLE_MASK).view(np.float32)
    return bool(a == 0)


def double_is_zero(bits: int) -> bool:
    a = np.uint64(bits & _DOUBLE_MASK).view(np.float64)
    return bool(a == 0)


def single_is_truthy(bits: int) -> bool:
    """C99 §6.3.1.2 controlling-expression truthiness: the value is
    truthy iff it compares unequal to 0. NaN is truthy (NaN != 0);
    both ±0 are falsy."""
    return not single_is_zero(bits)


def double_is_truthy(bits: int) -> bool:
    return not double_is_zero(bits)
