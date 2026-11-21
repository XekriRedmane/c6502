// The port of passes/shift_cost.py: when a shift by a constant is cheap
// enough to inline.
#pragma once

namespace c6502::passes {

// The most one-bit shift instructions an inline shift may take.
inline constexpr int kMaxInlineShiftSteps = 8;

// Returns whether a shift of a `size`-byte value by the constant
// `count` should be lowered inline. A signed right shift is only
// inlined without a byte move (`count < 8`), because the vacated high
// bytes would need a sign fill, except for `count = 8q + 7`, whose
// lowering computes the fill.
bool inline_shift_ok(int size, int count, bool signed_right = false);

}  // namespace c6502::passes
