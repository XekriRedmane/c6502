// When a shift by a constant is cheap enough to inline.
//
// `tac_to_asm` lowers `x << k` and `x >> k` with a constant `k` inline
// when this says so, and calls the `asl*`, `lsr*`, or `asr*` runtime
// helper otherwise. `strength_reduction` uses the same rule to decide
// whether rewriting a multiply by a constant into shifts and an add is
// worth it, so it never trades one helper call (`mul16`) for two
// (`asl16`).
//
// A shift by `k = 8q + r` bits of an `n`-byte value is `q` byte moves
// followed by `r` one-bit steps. Each step shifts the `n - q` bytes that
// aren't known to be zero, with one `ASL` / `ROL`, or `LSR` / `ROR`, per
// byte. A signed right step needs two more instructions (`LDA top; ASL
// A`) to copy the sign bit into carry before the `ROR`. Inlining is used
// while those step instructions number at most 8, so a 16-bit shift by
// up to 4 bits, an 8-bit shift by up to 7, or a 32-bit shift by up to 2
// (or by 8q + 2 with the moved bytes).
//
// A shift by exactly 1, or by a multiple of 8, is always inlined,
// whatever the width, as it was before this rule existed. So is a shift
// by `8q + 7`, which is lowered as a shift by `8(q + 1)` and one bit back
// the other way: an `ASL` (or `LSR`) of one byte followed by an `LDA` /
// `ROL A` / `STA` (or `ROR A`) of each remaining byte. For a 16-bit `w
// >> 7` that's 8 instructions.
//
// A left shift of a value with one significant byte, such as a widened
// byte, doesn't come here. `tac_to_asm` always inlines it, as two byte
// shifts (passes/significant_bytes.h).
//
// The helper call costs about 8 instructions of argument and result
// staging plus the JSR, and the helper loops once per bit, so up to 8
// inline shift instructions is about the same size and always faster.
#include "passes/shift_cost.h"

namespace c6502::passes {

bool inline_shift_ok(int size, int count, bool signed_right) {
  if (!(0 < count && count < size * 8)) {
    return false;
  }
  const int q = count / 8;
  const int r = count % 8;
  if (r == 7) {
    return true;
  }
  if (signed_right && q != 0) {
    return false;
  }
  if (count == 1 || r == 0) {
    return true;
  }
  const int per_step = size - q + (signed_right ? 2 : 0);
  return r * per_step <= kMaxInlineShiftSteps;
}

}  // namespace c6502::passes
