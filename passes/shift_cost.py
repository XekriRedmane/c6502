"""When a shift by a constant is cheap enough to inline.

`tac_to_asm` lowers `x << k` / `x >> k` with a constant `k` inline
when this says so, and calls the `asl*` / `lsr*` / `asr*` runtime
helper otherwise. `strength_reduction` uses the same rule to decide
whether rewriting a multiply by a constant into shifts and an add is
worth it, so it never trades one helper call (`mul16`) for two
(`asl16`).

A shift by `k = 8q + r` bits of an `n`-byte value is `q` byte moves
followed by `r` one-bit steps. Each step shifts the `n - q` bytes
that aren't known to be zero, with one `ASL` / `ROL`, or `LSR` /
`ROR`, per byte. A signed right step needs two more instructions
(`LDA top; ASL A`) to copy the sign bit into carry before the `ROR`.
Inlining is used while those step instructions number at most 8, so
a 16-bit shift by up to 4 bits, an 8-bit shift by up to 7, or a
32-bit shift by up to 2 (or by 8q + 2 with the moved bytes).

A shift by exactly 1, or by a multiple of 8, is always inlined,
whatever the width, as it was before this rule existed.

The helper call costs about 8 instructions of argument and result
staging plus the JSR, and the helper loops once per bit, so up to 8
inline shift instructions is about the same size and always faster.
"""

from __future__ import annotations

MAX_INLINE_SHIFT_STEPS = 8


def inline_shift_ok(size: int, count: int, *, signed_right: bool = False) -> bool:
    """True iff a shift of a `size`-byte value by the constant
    `count` should be lowered inline. A signed right shift is only
    inlined without a byte move (`count < 8`), because the vacated
    high bytes would need a sign fill."""
    if not 0 < count < size * 8:
        return False
    q, r = divmod(count, 8)
    if signed_right and q:
        return False
    if count == 1 or r == 0:
        return True
    per_step = size - q + (2 if signed_right else 0)
    return r * per_step <= MAX_INLINE_SHIFT_STEPS
