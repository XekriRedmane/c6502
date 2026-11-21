// Part C2 of tac_to_asm: arithmetic, the inline shifts, divisions, and
// products, and the shift and divide helpers. src/tac_to_asm.cpp opens
// with the description of the whole lowering.
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "bit_precise.h"
#include "errors.h"
#include "passes/optimization/var_visit.h"
#include "passes/recursion_saves.h"
#include "passes/shift_cost.h"
#include "tac_to_asm_translator.h"

namespace c6502::tac_to_asm {

using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

ir::Type_operand imm(asdl::Int v) { return ir::Imm{v}; }

void append(Instrs& out, Instrs more) {
  out.insert(out.end(), std::make_move_iterator(more.begin()), std::make_move_iterator(more.end()));
}

// Returns `n` copies of `instr`.
Instrs repeat(const ir::Type_instruction& instr, int n) {
  return Instrs(static_cast<std::size_t>(std::max(n, 0)), instr);
}

// Returns the inline lowering of `LeftShift(value, 1)` for any width:
// `src` copied into `dst` byte by byte, then `dst` shifted in place
// (`ASL dst.b0; ROL dst.b1; ... ROL dst.bN-1`).
//
// The shift on the dst byte uses the 6502's direct addressing modes
// when dst resolves to ZP or Data (one `ASL $XX`), and for a Frame or
// Stack dst, asm_emit writes the LDA, the shift of A, and the STA. When
// `src` and `dst` color to the same slot (a frequent result of SSA
// coalescing for compound shifts like `x <<= 1`), the leading Mov
// becomes a self-Mov, which the self-Mov peephole drops, leaving just
// the in-place shifts, the best form on the 6502.
//
// The carry goes from byte k's ASL to byte k+1's ROL even with a Mov
// between them, since STA, LDA, and LDY leave the carry flag alone. The
// result has the value's width, since a left shift doesn't depend on
// signedness, and overflow isn't detected, as the `asl*` helpers don't.
Instrs inline_left_shift_by_one(const ir::Type_operand& src_op, const ir::Type_operand& dst_op,
                                int size) {
  Instrs out;
  for (int k = 0; k < size; ++k) {
    const ir::Type_operand dst_byte = byte_at(dst_op, k);
    out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = dst_byte});
    if (k == 0) {
      out.push_back(ir::ArithmeticShiftLeft{.dst = dst_byte});
    } else {
      out.push_back(ir::RotateLeft{.dst = dst_byte});
    }
  }
  return out;
}

// Returns the inline lowering of `RightShift(value, 1)` for any width:
// `src` copied to `dst` byte by byte, then `dst` shifted in place from
// the high byte down.
//
// A signed (arithmetic) shift copies the high byte first and then reads
// it back to capture the sign:
//
//     Mov(src.high, dst.high)
//     LDA dst.high; ASL A     ; the sign bit of dst.high into carry
//     ROR(dst.high)           ; bit 7 from carry (the sign), carry from bit 0
//     Mov(src.bN-2, dst.bN-2)
//     ROR(dst.bN-2)
//     ...
//
// An unsigned (logical) shift starts with an LSR of the high byte (bit 7
// becomes 0), then a ROR of each lower byte:
//
//     Mov(src.high, dst.high)
//     LSR(dst.high)           ; bit 7 from 0, carry from bit 0
//     Mov(src.bN-2, dst.bN-2)
//     ROR(dst.bN-2)
//     ...
//
// The Mov of each byte keeps the carry the previous byte's shift made,
// since LDA, LDY, and STA leave C alone. When src and dst coalesce to
// the same slot, the self-Mov peephole drops the Movs, leaving the bare
// in-place shifts.
Instrs inline_right_shift_by_one(const ir::Type_operand& src_op, const ir::Type_operand& dst_op,
                                 int size, bool is_signed) {
  Instrs out;
  const int high = size - 1;
  const ir::Type_operand dst_high = byte_at(dst_op, high);
  out.push_back(ir::Mov{.src = byte_at(src_op, high), .dst = dst_high});
  if (is_signed) {
    // The ASL puts bit 7 into carry, and A's value is then irrelevant.
    // The ROR of dst.high puts the sign bit back into bit 7.
    out.push_back(ir::Mov{.src = dst_high, .dst = reg_a()});
    out.push_back(ir::ArithmeticShiftLeft{.dst = reg_a()});
    out.push_back(ir::RotateRight{.dst = dst_high});
  } else {
    out.push_back(ir::LogicalShiftRight{.dst = dst_high});
  }
  // The lower bytes: a copy, then a ROR with the carry from the byte
  // above.
  for (int k = high - 1; k >= 0; --k) {
    const ir::Type_operand dst_byte = byte_at(dst_op, k);
    out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = dst_byte});
    out.push_back(ir::RotateRight{.dst = dst_byte});
  }
  return out;
}

// Returns the value of `val` when it is a non-negative integer constant,
// or none. It detects a shift by a known number of bits.
std::optional<asdl::Int> shift_count_const_value(const tac::Type_val& val) {
  const auto* c = as<tac::Constant>(val);
  if (c == nullptr) {
    return std::nullopt;
  }
  const auto v = bit_precise::raw_value(c->const_);
  if (!v || *v < 0) {
    return std::nullopt;
  }
  return v;
}

// Returns a count as an `int` for the size rules, clamped, since a count
// that large is never inlined.
int small_count(asdl::Int cnt) { return cnt > 1 << 20 ? 1 << 20 : static_cast<int>(cnt); }

// Returns a left shift by `byte_count * 8` bits, where `0 < byte_count <
// size`, as byte moves: `dst.b[k] = src.b[k - byte_count]` for `k >=
// byte_count`, else 0. Signedness doesn't matter, since the bit pattern
// is the same.
//
// The moves go from the high byte down, which is correct when src and
// dst coalesce to the same slot (in-place `x <<= 8`): the high dst byte
// reads src's low byte before the low dst byte is zeroed.
Instrs inline_left_shift_byte_aligned(const ir::Type_operand& src_op,
                                      const ir::Type_operand& dst_op, int size, int byte_count) {
  Instrs out;
  for (int k = size - 1; k >= 0; --k) {
    const ir::Type_operand dst_byte = byte_at(dst_op, k);
    if (k < byte_count) {
      out.push_back(ir::Mov{.src = imm(0), .dst = dst_byte});
    } else {
      out.push_back(ir::Mov{.src = byte_at(src_op, k - byte_count), .dst = dst_byte});
    }
  }
  return out;
}

// Returns a logical right shift by `byte_count * 8` bits, where `0 <
// byte_count < size`, as byte moves: `dst.b[k] = src.b[k + byte_count]`
// for `k + byte_count < size`, else 0. The moves go from the low byte
// up, so the high src bytes are read before the low dst bytes are
// zeroed.
Instrs inline_right_shift_byte_aligned_unsigned(const ir::Type_operand& src_op,
                                                const ir::Type_operand& dst_op, int size,
                                                int byte_count) {
  Instrs out;
  for (int k = 0; k < size; ++k) {
    const ir::Type_operand dst_byte = byte_at(dst_op, k);
    const int src_idx = k + byte_count;
    if (src_idx < size) {
      out.push_back(ir::Mov{.src = byte_at(src_op, src_idx), .dst = dst_byte});
    } else {
      out.push_back(ir::Mov{.src = imm(0), .dst = dst_byte});
    }
  }
  return out;
}

// Returns `LDA src; <steps>; STA dst` for a 1-byte shift done in A.
Instrs shift_byte_in_a(const ir::Type_operand& src_op, const ir::Type_operand& dst_op,
                       const Instrs& steps) {
  Instrs out{ir::Mov{.src = byte_at(src_op, 0), .dst = reg_a()}};
  out.insert(out.end(), steps.begin(), steps.end());
  out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, 0)});
  return out;
}

// Returns a right shift by `8q + 7`, as a shift by `8(q + 1)` and then
// one bit left. Byte k of the result is `(src[k+q] >> 7) | (src[k+q+1]
// << 1)`, so one `ASL` of byte `q` puts its top bit in C, and an `LDA` /
// `ROL A` / `STA` of each byte above it carries that bit into the byte
// below. For `w >> 7` on a 16-bit `w`, that's
//
//     LDA w; ASL A; LDA w+1; ROL A; STA r; LDA #0; ROL A; STA r+1
//
// and for `w >> 15` it's `LDA w+1; ASL A; LDA #0; ROL A; STA r`, with
// `r+1` zero.
//
// The bytes above the last one carried into are zero for an unsigned
// shift. For a signed shift they're the sign fill, $FF when the value is
// negative and 0 otherwise, which the last rotate (or the `ASL` when
// there's no rotate) leaves in C. `LDA #0; ADC #$FF; EOR #$FF` turns C
// into the fill.
//
// The bytes are written low to high, and byte k is written after every
// source byte at or below k + q + 1 has been read, so this is correct
// when `src` and `dst` are the same slot.
Instrs inline_right_shift_7_mod_8(const ir::Type_operand& src_op, const ir::Type_operand& dst_op,
                                  int size, int q, bool is_signed) {
  Instrs out{ir::Mov{.src = byte_at(src_op, q), .dst = reg_a()},
             ir::ArithmeticShiftLeft{.dst = reg_a()}};
  // Bytes 0 .. size-q-2 each take bit 7 of the byte below and the low 7
  // bits of the byte above, shifted up.
  const int top = size - q - 1;
  for (int k = 0; k < top; ++k) {
    out.push_back(ir::Mov{.src = byte_at(src_op, q + 1 + k), .dst = reg_a()});
    out.push_back(ir::RotateLeft{.dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
  }
  if (is_signed) {
    out.push_back(ir::Mov{.src = imm(0), .dst = reg_a()});
    out.push_back(ir::Add{.src = imm(0xFF), .dst = reg_a()});
    out.push_back(ir::Xor{.src1 = reg_a(), .src2 = imm(0xFF), .dst = reg_a()});
    for (int k = top; k < size; ++k) {
      out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
    }
    return out;
  }
  out.push_back(ir::Mov{.src = imm(0), .dst = reg_a()});
  out.push_back(ir::RotateLeft{.dst = reg_a()});
  out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, top)});
  for (int k = top + 1; k < size; ++k) {
    out.push_back(ir::Mov{.src = imm(0), .dst = byte_at(dst_op, k)});
  }
  return out;
}

// Returns a left shift by `8q + 7`, as a shift by `8(q + 1)` and then
// one bit right. Byte k of the result, for k > q, is `(src[k-q-1] >> 1)
// | (src[k-q] << 7)`, and byte q is `src[0] << 7`. One `LSR` of byte
// `size-q-1` puts its low bit in C (the rest of that byte shifts out of
// the value), and an `LDA` / `ROR A` / `STA` of each byte below it
// carries that bit into the byte above. For `w << 7` on a 16-bit `w`,
// that's
//
//     LDA w+1; LSR A; LDA w; ROR A; STA r+1; LDA #0; ROR A; STA r
//
// The bytes are written high to low, and byte k is written after every
// source byte at or above k - q - 1 has been read, so this is correct
// when `src` and `dst` are the same slot. The bytes below q are zero.
Instrs inline_left_shift_7_mod_8(const ir::Type_operand& src_op, const ir::Type_operand& dst_op,
                                 int size, int q) {
  Instrs out{ir::Mov{.src = byte_at(src_op, size - q - 1), .dst = reg_a()},
             ir::LogicalShiftRight{.dst = reg_a()}};
  for (int k = size - 1; k > q; --k) {
    out.push_back(ir::Mov{.src = byte_at(src_op, k - q - 1), .dst = reg_a()});
    out.push_back(ir::RotateRight{.dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
  }
  out.push_back(ir::Mov{.src = imm(0), .dst = reg_a()});
  out.push_back(ir::RotateRight{.dst = reg_a()});
  out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, q)});
  for (int k = q - 1; k >= 0; --k) {
    out.push_back(ir::Mov{.src = imm(0), .dst = byte_at(dst_op, k)});
  }
  return out;
}

// Returns a left shift by the constant `count = 8q + r` (0 < count <
// size*8): the bytes placed `q` positions higher (zeroing the low `q`),
// then `dst` shifted left one bit `r` times in place, each step an `ASL`
// on byte `q` and a `ROL` on each byte above it (the bytes below `q` are
// zero and stay zero). Signedness doesn't matter.
//
// When `src` and `dst` coalesce to the same slot, the copy becomes
// self-Movs that the peepholes drop, leaving just the in-place shifts.
// `shift_cost::inline_shift_ok` decides when this is used instead of the
// `asl*` helper.
//
// A 1-byte shift runs in A instead, `LDA src; ASL A` x count; `STA dst`,
// which is as many instructions and 3 cycles a step faster than `ASL` on
// a zero-page byte.
Instrs inline_left_shift_const(const ir::Type_operand& src_op, const ir::Type_operand& dst_op,
                               int size, int count) {
  if (count % 8 == 7) {
    return inline_left_shift_7_mod_8(src_op, dst_op, size, count / 8);
  }
  if (size == 1) {
    return shift_byte_in_a(src_op, dst_op, repeat(ir::ArithmeticShiftLeft{.dst = reg_a()}, count));
  }
  const int q = count / 8;
  int r = count % 8;
  Instrs out;
  if (q != 0) {
    out = inline_left_shift_byte_aligned(src_op, dst_op, size, q);
  } else {
    // The first step copies and shifts each byte in turn.
    out = inline_left_shift_by_one(src_op, dst_op, size);
    r -= 1;
  }
  for (int step = 0; step < r; ++step) {
    for (int k = q; k < size; ++k) {
      const ir::Type_operand dst_byte = byte_at(dst_op, k);
      if (k == q) {
        out.push_back(ir::ArithmeticShiftLeft{.dst = dst_byte});
      } else {
        out.push_back(ir::RotateLeft{.dst = dst_byte});
      }
    }
  }
  return out;
}

// Returns a left shift by the constant `count = 8q + r` (0 < r, count <
// size*8) of a value whose bytes above the low one are zero
// (passes/significant_bytes.h), such as `(unsigned int)c << 5`. Only two
// bytes of the result can be nonzero: byte q, `lo = b << r`, and byte q
// + 1, `hi = b >> (8 - r)`, with `b` the source's low byte. Each is
// computed in A from `b`, by the shorter of two ways:
//
// - `lo`: `ASL A` r times, or `ROR A` 9 - r times and `AND` with the r
//   low bits clear (the rotates go round through C, so `b`'s low bits
//   arrive at the top).
// - `hi`: `LSR A` 8 - r times, or `ROL A` r + 1 times and `AND` with the
//   r low bits set.
//
// For r = 1 the carry the `ASL` shifts out is `hi`: `LDA b; ASL A; STA
// lo; LDA #0; ROL A; STA hi`, and r = 2 adds `ASL lo; ROL hi`, which
// `shift_in_a` turns into `...; LDA #0; ROL A; ASL lo; ROL A; STA hi` (2
// bytes and 2 cycles less than the two-part form). For r = 7 the `LSR`
// shifts out `lo`'s one bit: `LDA b; LSR A; STA hi; LDA #0; ROR A; STA
// lo`. The others compute `hi` and then `lo`, which leaves `lo` in A for
// an add or subtract: 13 or 14 bytes and 21 to 25 cycles with `b` in
// zero page, where `asl16` loops once per bit, and the 16-bit shift by 3
// or 4 this replaces took 15 or 18 bytes and 29 or 36 cycles. When byte
// q is the top byte there's no `hi`. The zero bytes are stored last.
// `src` and `dst` are different variables, so the regalloc keeps their
// bytes apart while `b` is still to be read.
Instrs inline_left_shift_byte(const ir::Type_operand& src_op, const ir::Type_operand& dst_op,
                              int size, int count) {
  const int q = count / 8;
  const int r = count % 8;
  const ir::Type_operand b = byte_at(src_op, 0);
  const ir::Type_operand lo = byte_at(dst_op, q);
  const bool has_hi = q + 1 < size;
  const auto lo_steps = [&]() -> Instrs {
    if (r <= 10 - r) {
      return repeat(ir::ArithmeticShiftLeft{.dst = reg_a()}, r);
    }
    Instrs s = repeat(ir::RotateRight{.dst = reg_a()}, 9 - r);
    s.push_back(ir::And{.src = imm((0xFF << r) & 0xFF), .dst = reg_a()});
    return s;
  };
  const auto hi_steps = [&]() -> Instrs {
    if (8 - r <= r + 2) {
      return repeat(ir::LogicalShiftRight{.dst = reg_a()}, 8 - r);
    }
    Instrs s = repeat(ir::RotateLeft{.dst = reg_a()}, r + 1);
    s.push_back(ir::And{.src = imm((1 << r) - 1), .dst = reg_a()});
    return s;
  };
  const ir::Type_operand zero = imm(0);
  Instrs out;
  std::set<int> nonzero;
  if (!has_hi) {
    out = shift_byte_in_a(b, lo, lo_steps());
    nonzero = {q};
  } else {
    const ir::Type_operand hi = byte_at(dst_op, q + 1);
    if (r <= 2) {
      out = shift_byte_in_a(b, lo, {ir::ArithmeticShiftLeft{.dst = reg_a()}});
      append(out, shift_byte_in_a(zero, hi, {ir::RotateLeft{.dst = reg_a()}}));
      if (r == 2) {
        out.push_back(ir::ArithmeticShiftLeft{.dst = lo});
        out.push_back(ir::RotateLeft{.dst = hi});
      }
    } else if (r == 7) {
      out = shift_byte_in_a(b, hi, {ir::LogicalShiftRight{.dst = reg_a()}});
      append(out, shift_byte_in_a(zero, lo, {ir::RotateRight{.dst = reg_a()}}));
    } else {
      // `lo` last, so it's in A for what reads it first: the low byte of
      // an add or subtract.
      out = shift_byte_in_a(b, hi, hi_steps());
      append(out, shift_byte_in_a(b, lo, lo_steps()));
    }
    nonzero = {q, q + 1};
  }
  for (int k = 0; k < size; ++k) {
    if (!nonzero.contains(k)) {
      out.push_back(ir::Mov{.src = zero, .dst = byte_at(dst_op, k)});
    }
  }
  return out;
}

// Returns the low byte of a right shift by the constant `count = 8q + r`
// (0 < r), when nothing reads the result's other bytes
// (`significant_bytes`' `low_byte_only`). It's `(b[q] >> r) | (b[q+1] <<
// (8 - r))` of the source's bytes `b`, whatever the shift's signedness,
// so it's written to `dst`'s byte 0 alone:
//
// - r = 1: `LDA b[q+1]; LSR A; LDA b[q]; ROR A`, the bit the `LSR`
//   shifts out going in at the top.
// - r = 7: `LDA b[q]; ASL A; LDA b[q+1]; ROL A`, the other way.
// - Otherwise `LDA b[q]; LSR A` r times; `STA d`; `LDA b[q+1]; ASL A` 8
//   - r times; `ORA d`, with `d` the result's byte.
//
// `(uint8_t)(zchars >> 5)` is 16 bytes and 28 cycles this way with
// `zchars` in zero page, where it called `lsr16`.
Instrs inline_right_shift_to_byte(const ir::Type_operand& src_op, const ir::Type_operand& dst_op,
                                  int count) {
  const int q = count / 8;
  const int r = count % 8;
  const ir::Type_operand lo = byte_at(src_op, q);
  const ir::Type_operand hi = byte_at(src_op, q + 1);
  const ir::Type_operand d = byte_at(dst_op, 0);
  const ir::Type_operand a = reg_a();
  if (r == 1) {
    return {ir::Mov{.src = hi, .dst = a}, ir::LogicalShiftRight{.dst = a},
            ir::Mov{.src = lo, .dst = a}, ir::RotateRight{.dst = a}, ir::Mov{.src = a, .dst = d}};
  }
  if (r == 7) {
    return {ir::Mov{.src = lo, .dst = a}, ir::ArithmeticShiftLeft{.dst = a},
            ir::Mov{.src = hi, .dst = a}, ir::RotateLeft{.dst = a}, ir::Mov{.src = a, .dst = d}};
  }
  Instrs out{ir::Mov{.src = lo, .dst = a}};
  append(out, repeat(ir::LogicalShiftRight{.dst = a}, r));
  out.push_back(ir::Mov{.src = a, .dst = d});
  out.push_back(ir::Mov{.src = hi, .dst = a});
  append(out, repeat(ir::ArithmeticShiftLeft{.dst = a}, 8 - r));
  out.push_back(ir::Or{.src = d, .dst = a});
  out.push_back(ir::Mov{.src = a, .dst = d});
  return out;
}

// Returns a right shift by the constant `count = 8q + r` (0 < count <
// size*8). Unsigned, the bytes go `q` positions lower (zeroing the high
// `q`), then `r` times the low `size - q` bytes shift right one bit,
// `LSR` on the top one and `ROR` below. Signed (callers pass q == 0),
// the value is copied, then each step captures the sign with `LDA top;
// ASL A` and shifts it back in with `ROR top`, followed by `ROR` on the
// lower bytes.
//
// `shift_cost::inline_shift_ok` decides when this is used instead of the
// `lsr*` or `asr*` helper.
//
// A 1-byte shift runs in A, as `LSR A` steps, or for a signed shift `CMP
// #$80; ROR A` steps (the compare copies bit 7 into C, which the rotate
// shifts back in).
Instrs inline_right_shift_const(const ir::Type_operand& src_op, const ir::Type_operand& dst_op,
                                int size, int count, bool is_signed) {
  if (count % 8 == 7) {
    return inline_right_shift_7_mod_8(src_op, dst_op, size, count / 8, is_signed);
  }
  if (size == 1) {
    Instrs steps;
    for (int i = 0; i < count; ++i) {
      if (is_signed) {
        steps.push_back(ir::Compare{.left = reg_a(), .right = imm(0x80)});
        steps.push_back(ir::RotateRight{.dst = reg_a()});
      } else {
        steps.push_back(ir::LogicalShiftRight{.dst = reg_a()});
      }
    }
    return shift_byte_in_a(src_op, dst_op, steps);
  }
  const int q = count / 8;
  int r = count % 8;
  Instrs out;
  if (q != 0) {
    out = inline_right_shift_byte_aligned_unsigned(src_op, dst_op, size, q);
  } else {
    // The first step copies and shifts each byte in turn.
    out = inline_right_shift_by_one(src_op, dst_op, size, is_signed);
    r -= 1;
  }
  const int top = size - 1 - q;
  for (int step = 0; step < r; ++step) {
    const ir::Type_operand top_byte = byte_at(dst_op, top);
    if (is_signed) {
      out.push_back(ir::Mov{.src = top_byte, .dst = reg_a()});
      out.push_back(ir::ArithmeticShiftLeft{.dst = reg_a()});
      out.push_back(ir::RotateRight{.dst = top_byte});
    } else {
      out.push_back(ir::LogicalShiftRight{.dst = top_byte});
    }
    for (int k = top - 1; k >= 0; --k) {
      out.push_back(ir::RotateRight{.dst = byte_at(dst_op, k)});
    }
  }
  return out;
}

enum class ShiftKind { kAsl, kLsr, kAsr };

// Returns a shift of a 1- or 2-byte value by a count in X, one bit per
// pass of a loop, where it called a shift helper. The value is copied
// into `dst`, which the loop shifts in place:
//
//         LDX n           ; or LDA n; TAX
//         CPX #0          ; these two only for a variable count
//         BEQ done
//     loop:
//         ASL dst         ; ROL dst+1          (asl)
//         LSR dst+1       ; ROR dst            (lsr)
//         LDA dst+1; ASL A; ROR dst+1; ROR dst (asr)
//         DEX
//         BNE loop
//     done:
//
// `count` is a constant count from 1 to 255, or none for the variable
// `count_op`, whose low byte is read. A count of the width in bits or
// more shifts every bit out, which leaves 0 (or every bit a copy of the
// sign for "asr"), as the `asl*`, `lsr*`, and `asr*` helpers do. The
// count goes in X because a soft-stack operand is reached through Y.
// When the count is the destination (`count_in_dst`), it's read before
// the copy.
Instrs shift_loop(const ir::Type_operand& src_op, const std::optional<ir::Type_operand>& count_op,
                  std::optional<asdl::Int> count, const ir::Type_operand& dst_op, int size,
                  ShiftKind kind, const std::string& loop, const std::string& done,
                  bool count_in_dst) {
  Instrs out;
  const auto copy = [&] {
    for (int k = 0; k < size; ++k) {
      out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = byte_at(dst_op, k)});
    }
  };
  if (!count) {
    const Instrs load{ir::Mov{.src = byte_at(*count_op, 0), .dst = reg_a()},
                      ir::Mov{.src = reg_a(), .dst = reg_x()}};
    if (count_in_dst) {
      out.insert(out.end(), load.begin(), load.end());
      copy();
    } else {
      copy();
      out.insert(out.end(), load.begin(), load.end());
    }
    // `redundant_compare` drops the CPX after an LDX of the count. The
    // branch doesn't read the flags of the TAX, since the asm optimizer
    // drops `TXA; TAX` when the count is already in X (the hazard
    // `optimization_asm.optimizer._explicit_index_tests` guards against
    // for every such branch).
    out.push_back(ir::Compare{.left = reg_x(), .right = imm(0)});
    out.push_back(ir::Branch{.cond = ir::EQ{}, .target = done});
  } else {
    copy();
    out.push_back(ir::Mov{.src = imm(*count), .dst = reg_x()});
  }
  out.push_back(ir::Label{.name = loop});
  const ir::Type_operand lo = byte_at(dst_op, 0);
  const ir::Type_operand hi = byte_at(dst_op, size - 1);
  if (kind == ShiftKind::kAsl) {
    out.push_back(ir::ArithmeticShiftLeft{.dst = lo});
    if (size == 2) {
      out.push_back(ir::RotateLeft{.dst = hi});
    }
  } else {
    if (kind == ShiftKind::kAsr) {
      out.push_back(ir::Mov{.src = hi, .dst = reg_a()});
      out.push_back(ir::ArithmeticShiftLeft{.dst = reg_a()});
      out.push_back(ir::RotateRight{.dst = hi});
    } else {
      out.push_back(ir::LogicalShiftRight{.dst = hi});
    }
    if (size == 2) {
      out.push_back(ir::RotateRight{.dst = lo});
    }
  }
  out.push_back(ir::Dec{.dst = reg_x()});
  out.push_back(ir::Branch{.cond = ir::NE{}, .target = loop});
  if (!count) {
    out.push_back(ir::Label{.name = done});
  }
  return out;
}

// Returns an unsigned byte divided by a constant from 1 to 255, by
// repeated subtraction, where it called `udivmod8`:
//
//         LDA x
//         LDX #$FF
//         SEC
//     loop:
//         INX
//         SBC #c
//         BCS loop
//         ADC #c          ; only for the remainder
//         STA rem
//         STX quot
//
// The loop runs once more than the quotient, and the last `SBC` borrows,
// so X is the quotient and A the remainder less `c`, which `ADC #c` (with
// C clear) puts back. The remainder is stored first, since a store of X
// to a soft-stack byte goes through A. It's 11 bytes before the stores
// against the call's 15, and takes about 7 cycles a unit of the quotient,
// so `x / 3` of 255 takes some 600 cycles where `udivmod8`'s eight steps
// take about 250.
Instrs inline_byte_divide(const ir::Type_operand& src_op, asdl::Int divisor,
                          const std::optional<ir::Type_operand>& quot_op,
                          const std::optional<ir::Type_operand>& rem_op, const std::string& loop) {
  const ir::Type_operand d = imm(divisor);
  Instrs out{ir::Mov{.src = byte_at(src_op, 0), .dst = reg_a()},
             ir::Mov{.src = imm(0xFF), .dst = reg_x()},
             ir::SetCarry{},
             ir::Label{.name = loop},
             ir::Inc{.dst = reg_x()},
             ir::Sub{.src = d, .dst = reg_a()},
             ir::Branch{.cond = ir::CS{}, .target = loop}};
  if (rem_op) {
    out.push_back(ir::Add{.src = d, .dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(*rem_op, 0)});
  }
  if (quot_op) {
    out.push_back(ir::Mov{.src = reg_x(), .dst = byte_at(*quot_op, 0)});
  }
  return out;
}

// Returns the 16-bit product of two bytes (the low bytes of `a_op` and
// `b_op`), by shifting and adding, where it called `mul16`:
//
//         LDA b
//         STA HARGS       ; the multiplier, and then the low byte
//         LDA #0          ; the high byte
//         LDX #8
//         LSR HARGS
//     loop:
//         BCC skip
//         CLC
//         ADC a
//     skip:
//         ROR A
//         ROR HARGS
//         DEX
//         BNE loop
//         STA dst+1
//         LDA HARGS
//         STA dst
//
// Each pass adds `a` to the high byte when the multiplier's low bit is
// set, then shifts the high byte and the multiplier right together, so
// the product's low bits fill the multiplier's byte from the top. The
// multiplier is in HARGS, whose byte is in zero page under every layout,
// because a soft-stack byte can't be rotated without going through A,
// which holds the high byte. It's 27 bytes with the stores against about
// the same for `mul16` and its four HARGS bytes, and the simulator
// counts about 170 cycles for 200 times 255 against about 580 for
// `mul16`'s sixteen 16-bit steps, each with the call of the function.
Instrs inline_byte_product(const ir::Type_operand& a_op, const ir::Type_operand& b_op,
                           const ir::Type_operand& dst_op, const std::string& loop,
                           const std::string& skip) {
  const ir::Type_operand work = hargs(0);
  return {ir::Mov{.src = byte_at(b_op, 0), .dst = reg_a()},
          ir::Mov{.src = reg_a(), .dst = work},
          ir::Mov{.src = imm(0), .dst = reg_a()},
          ir::Mov{.src = imm(8), .dst = reg_x()},
          ir::LogicalShiftRight{.dst = work},
          ir::Label{.name = loop},
          ir::Branch{.cond = ir::CC{}, .target = skip},
          ir::ClearCarry{},
          ir::Add{.src = byte_at(a_op, 0), .dst = reg_a()},
          ir::Label{.name = skip},
          ir::RotateRight{.dst = reg_a()},
          ir::RotateRight{.dst = work},
          ir::Dec{.dst = reg_x()},
          ir::Branch{.cond = ir::NE{}, .target = loop},
          ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, 1)},
          ir::Mov{.src = work, .dst = reg_a()},
          ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, 0)}};
}

// --- The queries of the rotations and pairs ---

const std::vector<const tac::Type_instruction*>& defs_of(const Translator& t,
                                                         std::string_view name) {
  static const std::vector<const tac::Type_instruction*> kNone;
  const auto it = t.tac_defs.find(name);
  return it == t.tac_defs.end() ? kNone : it->second;
}

std::size_t uses_of(const Translator& t, const std::string& name) {
  const std::size_t* n = t.tac_uses.find(name);
  return n == nullptr ? 0 : *n;
}

// Returns whether `val` is the constant whose `value` field is `v`.
bool const_value_is(const tac::Type_val& val, asdl::Int v) {
  const auto* c = as<tac::Constant>(val);
  if (c == nullptr) {
    return false;
  }
  const auto raw = bit_precise::raw_value(c->const_);
  return raw && *raw == v;
}

// Returns the instruction of kind `K` that defines the temporary `v`,
// when it's `v`'s only definition and `v` is read once, or null.
template <class K>
const K* single_def_operand(const Translator& t, const tac::Type_val& v) {
  const auto* var = as<tac::Var>(v);
  if (var == nullptr || var->name.empty() || var->name[0] != '%') {
    return nullptr;
  }
  const auto& defs = defs_of(t, var->name);
  if (defs.size() != 1 || uses_of(t, var->name) != 1) {
    return nullptr;
  }
  return as<K>(*defs[0]);
}

// Returns whether `v`'s only definition is `x >> (8n - 1)` of an
// unsigned n-byte `x`.
bool is_top_bit(const Translator& t, const tac::Type_val& v) {
  const auto* var = as<tac::Var>(v);
  if (var == nullptr) {
    return false;
  }
  const auto& defs = defs_of(t, var->name);
  if (defs.size() != 1) {
    return false;
  }
  const auto* d = as<tac::Binary>(*defs[0]);
  return d != nullptr && isa<tac::RightShift>(d->op) &&
         const_value_is(d->src2, 8 * t.size_of(d->src1) - 1) && t.is_unsigned_val(d->src1);
}

// Returns whether every definition of the byte `v` leaves it 0 or 1: the
// constant 0 or 1, a comparison, a logical not, `& 1`, the top bit of an
// unsigned value (`(uint8_t)(x >> 15)`), or a copy or extend of such a
// byte.
bool is_bit(const Translator& t, const tac::Type_val& v, int depth = 0) {
  if (const auto* c = as<tac::Constant>(v)) {
    const auto raw = bit_precise::raw_value(c->const_);
    return raw && (*raw == 0 || *raw == 1);
  }
  const auto* var = as<tac::Var>(v);
  if (var == nullptr || depth > 4) {
    return false;
  }
  const auto& defs = defs_of(t, var->name);
  if (defs.empty()) {
    return false;
  }
  for (const tac::Type_instruction* d : defs) {
    switch (tac::kind(*d)) {
      case tac::Kind_instruction::Copy:
        if (!is_bit(t, std::get<tac::Copy>(*d).src, depth + 1)) {
          return false;
        }
        break;
      case tac::Kind_instruction::ZeroExtend:
        if (!is_bit(t, std::get<tac::ZeroExtend>(*d).src, depth + 1)) {
          return false;
        }
        break;
      case tac::Kind_instruction::Binary: {
        const auto& b = std::get<tac::Binary>(*d);
        if (isa<tac::Equal, tac::NotEqual, tac::LessThan, tac::LessOrEqual, tac::GreaterThan,
                tac::GreaterOrEqual>(b.op)) {
          break;
        }
        if (isa<tac::BitwiseAnd>(b.op) && const_value_is(b.src2, 1)) {
          break;
        }
        return false;
      }
      case tac::Kind_instruction::Unary:
        if (!isa<tac::LogicalNot>(std::get<tac::Unary>(*d).op)) {
          return false;
        }
        break;
      case tac::Kind_instruction::Truncate:
        if (!is_top_bit(t, std::get<tac::Truncate>(*d).src)) {
          return false;
        }
        break;
      default:
        return false;
    }
  }
  return true;
}

// A rotate-in found at one instruction: `x`, `b`, and the shift and the
// extend it reads.
struct FoundRotation {
  tac::Type_val x;
  tac::Type_val b;
  const tac::Type_instruction* shift;
  const tac::Type_instruction* ext;
};

// Returns the rotate-in at `ins` when it's `(x << 1) | b`: an OR of a
// temporary `x << 1` and the zero extension of a byte `b` that's 0 or 1,
// each temporary read only by the OR.
std::optional<FoundRotation> rotation_at(const Translator& t, const tac::Type_instruction& ins) {
  const auto* bin = as<tac::Binary>(ins);
  if (bin == nullptr || !isa<tac::BitwiseOr>(bin->op) || !isa<tac::Var>(bin->dst)) {
    return std::nullopt;
  }
  const std::pair<const tac::Type_val*, const tac::Type_val*> orders[] = {
      {&bin->src1, &bin->src2}, {&bin->src2, &bin->src1}};
  for (const auto& [shifted, ext] : orders) {
    const auto* x = single_def_operand<tac::Binary>(t, *shifted);
    const auto* z = single_def_operand<tac::ZeroExtend>(t, *ext);
    if (x == nullptr || z == nullptr) {
      continue;
    }
    if (!(isa<tac::LeftShift>(x->op) && const_value_is(x->src2, 1) && isa<tac::Var>(x->src1))) {
      continue;
    }
    if (!(t.size_of(x->src1) == t.size_of(bin->dst) && t.size_of(z->src) == 1 &&
          is_bit(t, z->src))) {
      continue;
    }
    // The pointers of the definitions, for their indices.
    const auto* shift_def = defs_of(t, std::get<tac::Var>(*shifted).name)[0];
    const auto* ext_def = defs_of(t, std::get<tac::Var>(*ext).name)[0];
    return FoundRotation{x->src1, z->src, shift_def, ext_def};
  }
  return std::nullopt;
}

int index_of(const std::vector<tac::Type_instruction>& instrs, const tac::Type_instruction* ins) {
  return static_cast<int>(ins - instrs.data());
}

// Returns whether `b` is `(uint8_t)(y >> (8n - 1))` of the n-byte `y`,
// read before `instrs[j]` with nothing in between writing `y`.
bool top_bit_of(const Translator& t, const tac::Type_val& b, const tac::Type_val& y,
                const std::vector<tac::Type_instruction>& instrs, int j) {
  const auto* bv = as<tac::Var>(b);
  if (bv == nullptr) {
    return false;
  }
  const auto& defs = defs_of(t, bv->name);
  if (defs.size() != 1 || !isa<tac::Truncate>(*defs[0])) {
    return false;
  }
  const tac::Type_val& src = std::get<tac::Truncate>(*defs[0]).src;
  const auto* sv = as<tac::Var>(src);
  if (sv == nullptr) {
    return false;
  }
  const auto& sdefs = defs_of(t, sv->name);
  if (sdefs.size() != 1 || !is_top_bit(t, src)) {
    return false;
  }
  const auto& shift = std::get<tac::Binary>(*sdefs[0]);
  if (!(shift.src1 == y)) {
    return false;
  }
  const int k = index_of(instrs, sdefs[0]);
  if (k >= j) {
    return false;
  }
  const auto* yv = as<tac::Var>(y);
  for (int i = k + 1; i < j; ++i) {
    for (const auto d : passes::recursion_saves::defs_in(instrs[i])) {
      if (yv != nullptr && d == yv->name) {
        return false;
      }
    }
  }
  return true;
}

// Returns whether the variable `name` has a volatile type.
bool is_volatile_name(const Translator& t, const std::string& name) {
  if (t.symbols == nullptr) {
    return false;
  }
  const passes::Symbol* sym = t.symbols->find(name);
  if (sym == nullptr) {
    return false;
  }
  const c99::Type_data_type* type = &sym->type;
  while (const auto* c = as<c99::Const>(*type)) {
    type = &*c->referenced_type;
  }
  return isa<c99::Volatile>(*type);
}

// Returns the divisor of an unsigned 1-byte divide or remainder by a
// constant from 1 to 255, which `inline_byte_divide` lowers, else none.
std::optional<asdl::Int> byte_divisor(const Translator& t, const tac::Type_val& src1,
                                      const tac::Type_val& src2, int size) {
  if (size != 1 || !t.is_unsigned_val(src1)) {
    return std::nullopt;
  }
  const auto* c = as<tac::Constant>(src2);
  if (c == nullptr) {
    return std::nullopt;
  }
  const auto v = bit_precise::raw_value(c->const_);
  if (!v || !(1 <= *v && *v <= 0xFF)) {
    return std::nullopt;
  }
  return v;
}

// Returns whether only the low byte of `val` can be nonzero: a constant
// from 0 to 255, or a local whose other bytes are zero
// (passes/significant_bytes.h).
bool byte_valued(const Translator& t, const tac::Type_val& val) {
  if (const auto* c = as<tac::Constant>(val)) {
    const auto v = bit_precise::raw_value(c->const_);
    return v && 0 <= *v && *v <= 0xFF;
  }
  const auto& name = std::get<tac::Var>(val).name;
  const auto it = t.significant.find(name);
  return (it == t.significant.end() ? 2 : it->second) <= 1;
}

// Returns a 1- or 2-byte shift the inline lowerings don't take (a
// variable count, or a constant one `inline_shift_ok` rejects) as a
// loop (`shift_loop`). A constant count of 0 is a copy.
Instrs shift_loop_of(Translator& t, const tac::Type_val& src2, const ir::Type_operand& src1_op,
                     const ir::Type_operand& src2_op, const tac::Type_val& dst,
                     const ir::Type_operand& dst_op, int size, ShiftKind kind) {
  auto cnt = shift_count_const_value(src2);
  if (cnt) {
    *cnt &= 0xFF;
    if (*cnt == 0) {
      Instrs out;
      for (int k = 0; k < size; ++k) {
        out.push_back(ir::Mov{.src = byte_at(src1_op, k), .dst = byte_at(dst_op, k)});
      }
      return out;
    }
  }
  const auto* sv = as<tac::Var>(src2);
  const auto* dv = as<tac::Var>(dst);
  const bool count_in_dst = !cnt && sv != nullptr && dv != nullptr && sv->name == dv->name;
  const std::string loop = t.make_label("shift_loop");
  const std::string done = t.make_label("shift_done");
  std::optional<ir::Type_operand> count_op;
  if (!cnt) {
    count_op = src2_op;
  }
  return shift_loop(src1_op, count_op, cnt, dst_op, size, kind, loop, done, count_in_dst);
}

// Returns a multiply, divide, remainder, or shift of a 3-, 5-, 6-, or
// 7-byte integer (a bit-precise type of that size), for which the
// runtime has no helper. The operands go to the helper of the next width
// `w` (4 or 8 bytes), each extended to `w` bytes in HARGS, and the low
// `size` bytes of the result come back. A multiply's and a left shift's
// low bytes don't depend on the bytes above them, so those operands are
// zero-filled. A divide, a remainder, and a right shift extend by the
// operand's signedness, which gives the value itself at `w` bytes, so
// the result is the same as at `size` bytes and fits in them. A shift by
// a constant that `inline_shift_ok` accepts at `size` bytes is left to
// the inline lowering (none).
std::optional<Instrs> odd_width_helper_op(const Translator& t, const tac::Type_binary_operator& op,
                                          const tac::Type_val& src1, const tac::Type_val& src2,
                                          const ir::Type_operand& src1_op,
                                          const ir::Type_operand& src2_op,
                                          const ir::Type_operand& dst_op, int size) {
  const int w = bit_precise::helper_bytes(size);
  const std::string bits = w == 4 ? "32" : "64";
  const bool is_unsigned = t.is_unsigned_val(src1);
  const bool shift = isa<tac::LeftShift, tac::RightShift>(op);
  if (shift) {
    const auto cnt = shift_count_const_value(src2);
    if (cnt && passes::inline_shift_ok(size, small_count(*cnt),
                                       isa<tac::RightShift>(op) && !is_unsigned)) {
      return std::nullopt;
    }
  }
  std::string helper;
  int out_off = 0;
  bool sign_fill = false;
  switch (tac::kind(op)) {
    case tac::Kind_binary_operator::Multiply:
      helper = "mul" + bits;
      out_off = 2 * w;
      break;
    case tac::Kind_binary_operator::Divide:
    case tac::Kind_binary_operator::Modulo:
      helper = std::string(is_unsigned ? "u" : "s") + "divmod" + bits;
      out_off = (isa<tac::Divide>(op) ? 2 : 3) * w;
      sign_fill = !is_unsigned;
      break;
    case tac::Kind_binary_operator::LeftShift:
      helper = "asl" + bits;
      out_off = w + 1;
      break;
    case tac::Kind_binary_operator::RightShift:
      helper = std::string(is_unsigned ? "lsr" : "asr") + bits;
      out_off = w + 1;
      sign_fill = !is_unsigned;
      break;
    default:
      throw InternalError("odd_width_helper_op: an operator without a helper");
  }
  Instrs out;
  std::vector<ir::Type_operand> operands{src1_op};
  if (!shift) {
    operands.push_back(src2_op);
  }
  for (std::size_t j = 0; j < operands.size(); ++j) {
    const int base = static_cast<int>(j) * w;
    for (int k = 0; k < size; ++k) {
      out.push_back(ir::Mov{.src = byte_at(operands[j], k), .dst = reg_a()});
      out.push_back(ir::Mov{.src = reg_a(), .dst = hargs(base + k)});
    }
    if (sign_fill) {
      // A is the top byte. Its bit 7 goes to C, and `0 + $FF + C` then
      // `EOR #$FF` is $FF for a set bit and $00 for a clear one.
      out.push_back(ir::ArithmeticShiftLeft{.dst = reg_a()});
      out.push_back(ir::Mov{.src = imm(0), .dst = reg_a()});
      out.push_back(ir::Add{.src = imm(0xFF), .dst = reg_a()});
      out.push_back(ir::Xor{.src1 = reg_a(), .src2 = imm(0xFF), .dst = reg_a()});
    } else {
      out.push_back(ir::Mov{.src = imm(0), .dst = reg_a()});
    }
    for (int k = size; k < w; ++k) {
      out.push_back(ir::Mov{.src = reg_a(), .dst = hargs(base + k)});
    }
  }
  if (shift) {
    // The count is one byte, right after the value.
    out.push_back(ir::Mov{.src = byte_at(src2_op, 0), .dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = hargs(w)});
  }
  out.push_back(ir::Call{.name = helper});
  for (int k = 0; k < size; ++k) {
    out.push_back(ir::Mov{.src = hargs(out_off + k), .dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
  }
  return out;
}

// Returns an add or subtract by size. The carry setup runs once before
// the low byte, and each higher byte's ADC or SBC takes the carry the
// byte below made (LDA changes only N and Z).
//
// The low bytes of a constant that are zero leave the other operand's
// bytes as they are and carry (or borrow) nothing, so they're copies,
// and the carry setup goes in front of the first nonzero byte. `p +=
// 0x100` in place is then just `LDA p+1; CLC; ADC #1; STA p+1`, which
// `inc_peephole` makes `INC p+1` (and `p -= 0x100` becomes `DEC p+1`).
// For an add, the constant may be either operand. A constant that's all
// zero keeps the whole chain, so the carry still comes out set as a
// branch on it (`fused_subtract`) expects.
Instrs translate_add_sub(ir::Type_operand src1_op, ir::Type_operand src2_op,
                         const ir::Type_operand& dst_op, int size, bool add) {
  const ir::Type_operand zero = imm(0);
  Instrs out;
  int skip = 0;
  while (skip < size && byte_at(src2_op, skip) == zero) {
    ++skip;
  }
  if (skip == 0 && add && isa<ir::Imm>(src1_op)) {
    while (skip < size && byte_at(src1_op, skip) == zero) {
      ++skip;
    }
    if (skip != 0) {
      std::swap(src1_op, src2_op);
    }
  }
  if (!isa<ir::Imm>(src2_op) || skip == size) {
    skip = 0;
  }
  for (int k = 0; k < skip; ++k) {
    const ir::Type_operand src = byte_at(src1_op, k);
    const ir::Type_operand dst = byte_at(dst_op, k);
    if (!(src == dst)) {
      out.push_back(ir::Mov{.src = src, .dst = dst});
    }
  }
  for (int k = skip; k < size; ++k) {
    out.push_back(ir::Mov{.src = byte_at(src1_op, k), .dst = reg_a()});
    if (k == skip) {
      if (add) {
        out.push_back(ir::ClearCarry{});
      } else {
        out.push_back(ir::SetCarry{});
      }
    }
    if (add) {
      out.push_back(ir::Add{.src = byte_at(src2_op, k), .dst = reg_a()});
    } else {
      out.push_back(ir::Sub{.src = byte_at(src2_op, k), .dst = reg_a()});
    }
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
  }
  return out;
}

// Returns an And or an Or by size. Each byte is independent, with no
// carry or flag to thread.
Instrs translate_bitwise(const ir::Type_operand& src1_op, const ir::Type_operand& src2_op,
                         const ir::Type_operand& dst_op, int size, bool is_and) {
  Instrs out;
  for (int k = 0; k < size; ++k) {
    out.push_back(ir::Mov{.src = byte_at(src1_op, k), .dst = reg_a()});
    if (is_and) {
      out.push_back(ir::And{.src = byte_at(src2_op, k), .dst = reg_a()});
    } else {
      out.push_back(ir::Or{.src = byte_at(src2_op, k), .dst = reg_a()});
    }
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
  }
  return out;
}

// Returns an Xor by size, in the asymmetric `Xor(src1, src2, dst)` shape
// that lets the emitter choose which side carries the addressing mode.
Instrs translate_xor(const ir::Type_operand& src1_op, const ir::Type_operand& src2_op,
                     const ir::Type_operand& dst_op, int size) {
  Instrs out;
  for (int k = 0; k < size; ++k) {
    out.push_back(ir::Mov{.src = byte_at(src1_op, k), .dst = reg_a()});
    out.push_back(ir::Xor{.src1 = reg_a(), .src2 = byte_at(src2_op, k), .dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
  }
  return out;
}

// Returns the name of a `divmod` helper of `size` bytes.
std::string divmod_helper(bool is_unsigned, int size) {
  return std::string(is_unsigned ? "u" : "s") + "divmod" + std::to_string(8 * size);
}

}  // namespace

// Returns the rotate-ins of a function, `{i: (x, b, carried)}` for each
// `instrs[i]` that is `(x << 1) | b` with `b` a byte that's 0 or 1, and
// the indices of the shift and the extend it reads, which aren't lowered
// on their own. `carried` is set when C already holds `b`: `b` is the top
// bit of the value the previous rotate-in shifted, `(uint8_t)(y >> 15)`
// read before it, with only those skipped instructions in between. Only
// under `-O`, where these patterns come out of the TAC optimizer whole.
std::pair<std::map<int, Rotation>, std::set<int>> Translator::plan_rotations(
    const std::vector<tac::Type_instruction>& instrs) {
  std::map<int, Rotation> out;
  std::set<int> skip;
  if (!bare_exit) {
    return {out, skip};
  }
  std::optional<std::pair<int, tac::Type_val>> last;
  for (int i = 0; i < static_cast<int>(instrs.size()); ++i) {
    const auto found = rotation_at(*this, instrs[i]);
    if (!found) {
      continue;
    }
    const std::set<int> mine{index_of(instrs, found->shift), index_of(instrs, found->ext)};
    bool carried = false;
    if (last) {
      bool between_in_mine = true;
      for (int k = last->first + 1; k < i; ++k) {
        if (!mine.contains(k)) {
          between_in_mine = false;
          break;
        }
      }
      carried =
          between_in_mine && top_bit_of(*this, found->b, last->second, instrs, last->first);
    }
    out.insert_or_assign(i, Rotation{found->x, found->b, carried});
    skip.insert(mine.begin(), mine.end());
    last = std::make_pair(i, found->x);
  }
  return {out, skip};
}

// Returns `(x << 1) | b`, with `b` a byte that's 0 or 1, as `LDA b; LSR`
// and then a `ROL` of each byte of `x` into `dst`. The `LSR` puts `b` in
// C, and the rotates shift it into bit 0, so the result is the same, and
// C comes out as the top bit shifted out, as an `ASL` / `ROL` chain
// leaves it. That's 7 bytes in place against the shift's and the `ORA`'s
// 10. When C already holds `b`, the top bit the previous rotate-in
// shifted out, the `LDA b; LSR` goes too. Zork's `divu16`
// (examples/zork/numbers.c) shifts each step's bit into its quotient,
// and the quotient's top bit into its remainder, this way.
Instrs Translator::rotate_in(const tac::Type_val& dst, const Rotation& rotation) {
  const int size = size_of(dst);
  const ir::Type_operand src_op = translate_val(rotation.x);
  const ir::Type_operand dst_op = translate_val(dst);
  Instrs out;
  if (!rotation.carried) {
    out.push_back(ir::Mov{.src = translate_val(rotation.b), .dst = reg_a()});
    out.push_back(ir::LogicalShiftRight{.dst = reg_a()});
  }
  for (int k = 0; k < size; ++k) {
    out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = reg_a()});
    out.push_back(ir::RotateLeft{.dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
  }
  return out;
}

// Pairs a divide and a remainder of the same operands, so that one loop
// of `inline_byte_divide`, or one call of a `divmod` helper, gives both:
// `{i: (quot, rem)}` for the first of each pair, `instrs[i]`, and the
// indices of the seconds, which aren't lowered on their own. `digit =
// value / place; value %= place;` in Ali Baba's `num_to_decimal` called
// `udivmod16` twice.
//
// The second is at most 8 instructions later in the same straight run,
// with no store or call between, and nothing that writes an operand or
// reads or writes the second's destination, which is written at the
// first. The first's destination isn't an operand, which the second
// reads, and no operand is volatile, since the pair reads each once.
// Neither instruction is in `exclude`, the instructions lowered some
// other way.
std::pair<std::map<int, DivmodPair>, std::set<int>> Translator::plan_divmod_pairs(
    const std::vector<tac::Type_instruction>& instrs, const std::set<int>& exclude) {
  std::map<int, DivmodPair> pairs;
  std::set<int> skip;
  const auto is_divmod = [](const tac::Type_binary_operator& op) {
    return isa<tac::Divide, tac::Modulo>(op);
  };
  const auto is_barrier = [](const tac::Type_instruction& ins) {
    using K = tac::Kind_instruction;
    switch (tac::kind(ins)) {
      case K::Label:
      case K::Jump:
      case K::JumpIfTrue:
      case K::JumpIfFalse:
      case K::JumpIfCmp:
      case K::JumpIfMasked:
      case K::Ret:
      case K::FunctionCall:
      case K::IndirectCall:
      case K::Store:
      case K::IndexedStore:
      case K::IndexedSymbolStore:
      case K::IndirectIndexedStore:
        return true;
      default:
        return false;
    }
  };
  const int n = static_cast<int>(instrs.size());
  for (int i = 0; i < n; ++i) {
    const auto* a = as<tac::Binary>(instrs[i]);
    if (skip.contains(i) || exclude.contains(i) || a == nullptr || !is_divmod(a->op) ||
        !isa<tac::Var>(a->dst) || !isa<tac::Var>(a->src1)) {
      continue;
    }
    const int sz = size_of(a->src1);
    if (!(sz == 1 || sz == 2 || sz == 4 || sz == 8) || is_fp_val(a->src1)) {
      continue;
    }
    const std::string& x = std::get<tac::Var>(a->src1).name;
    const std::string& a_dst = std::get<tac::Var>(a->dst).name;
    passes::NameSet operands{x};
    if (const auto* v = as<tac::Var>(a->src2)) {
      operands.insert(v->name);
    }
    bool volatile_operand = false;
    for (const auto& name : operands) {
      volatile_operand = volatile_operand || is_volatile_name(*this, name);
    }
    if (operands.contains(a_dst) || volatile_operand) {
      continue;
    }
    // The second's destination is written at the first, so nothing
    // between may read or write it.
    passes::NameSet touched;
    for (int j = i + 1; j < std::min(i + 9, n); ++j) {
      const auto& b = instrs[j];
      if (const auto* bb = as<tac::Binary>(b);
          bb != nullptr && is_divmod(bb->op) && tac::kind(bb->op) != tac::kind(a->op) &&
          bb->src1 == a->src1 && bb->src2 == a->src2 && isa<tac::Var>(bb->dst)) {
        const std::string& b_dst = std::get<tac::Var>(bb->dst).name;
        if (!touched.contains(b_dst) && b_dst != a_dst && !exclude.contains(j)) {
          if (isa<tac::Divide>(a->op)) {
            pairs.insert_or_assign(i, DivmodPair{a->dst, bb->dst});
          } else {
            pairs.insert_or_assign(i, DivmodPair{bb->dst, a->dst});
          }
          skip.insert(j);
        }
        break;
      }
      if (is_barrier(b)) {
        break;
      }
      bool writes_operand = false;
      for (const auto d : passes::defs_in(b)) {
        writes_operand = writes_operand || operands.contains(d);
      }
      if (writes_operand) {
        break;
      }
      for (const auto d : passes::defs_in(b)) {
        touched.emplace(d);
      }
      for (const auto u : passes::uses_in(b)) {
        touched.emplace(u);
      }
    }
  }
  return {pairs, skip};
}

// Returns a divide and a remainder of `src1` by `src2` planned by
// `plan_divmod_pairs`, into the pair's `quot` and `rem`: one inline loop
// for a byte divided by a constant, or else one call of the `divmod`
// helper, whose quotient and remainder are both read from HARGS.
Instrs Translator::translate_divmod_pair(const tac::Type_val& src1, const tac::Type_val& src2,
                                         const DivmodPair& pair) {
  const int size = size_of(src1);
  if (const auto divisor = byte_divisor(*this, src1, src2, size)) {
    const std::string loop = make_label("div_loop");
    return inline_byte_divide(translate_val(src1), *divisor, translate_val(pair.quot),
                              translate_val(pair.rem), loop);
  }
  Instrs out = translate_helper_call({{translate_val(src1), size}, {translate_val(src2), size}},
                                     divmod_helper(is_unsigned_val(src1), size), 2 * size, size,
                                     translate_val(pair.quot));
  const ir::Type_operand rem_op = translate_val(pair.rem);
  for (int k = 0; k < size; ++k) {
    out.push_back(ir::Mov{.src = hargs(3 * size + k), .dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(rem_op, k)});
  }
  return out;
}

// Lowers a TAC Binary into asm. The operand size decides the shape of
// the sequence: one byte, or a sequence per byte with the carry threaded
// where needed.
//
// For the arithmetic, bitwise, and comparison operators the type checker
// promotes both operands to the common type, so the sizes of `src1` and
// `src2` agree. The shifts are the exception: by C99 §6.5.7.3 the right
// operand promotes on its own, so its width may differ from the left's.
// The shift helpers take only the right's low byte, so `size` is the
// size of `src1`, the left, which is the result's width. A comparison's
// result is always an `int`, and the size of the operands decides the
// shape there too.
Instrs Translator::translate_binary(const tac::Type_binary_operator& op, const tac::Type_val& src1,
                                    const tac::Type_val& src2, const tac::Type_val& dst) {
  const ir::Type_operand src1_op = translate_val(src1);
  const ir::Type_operand src2_op = translate_val(src2);
  const ir::Type_operand dst_op = translate_val(dst);
  const int size = size_of(src1);
  // The destination's size, for the zero fill of a comparison's result:
  // a comparison returns an `int`, and the inline 0 or 1 writes 1 byte,
  // so the high byte needs a zero.
  const int dst_size = size_of(dst);
  // The orderings and the right shift are chosen by signedness. The type
  // checker promoted a comparison's operands to a common type, so the
  // tests of `src1` and `src2` agree. A right shift's signedness is the
  // left's (C99 §6.5.7.5). An unsigned ordering is the BCC/BCS sequence
  // of SBCs per byte without the V correction, and a signed one the
  // V-corrected MI/PL sequence. Pointers are 16-bit unsigned addresses,
  // so they take the unsigned path, which `is_unsigned_val` gives them.
  const bool unsigned_cmp = is_unsigned_val(src1) || is_unsigned_val(src2);
  // After the C99 §6.3.1.8 promotions both operands of a floating `+`,
  // `-`, `*`, or `/` have the same type. A float (4 bytes) goes to
  // fadd, fsub, fmul, or fdiv with the result at HARGS+8..11, and a
  // double (8 bytes) to dadd, dsub, dmul, or ddiv with the result at
  // HARGS+16..23. A floating negate is a flip of the sign bit in
  // `translate_unary`, and the floating comparisons are lowered inline
  // as compares of the bit patterns (docs/sim_findings.md).
  const bool fp = is_fp_val(src1) || is_fp_val(src2);
  using K = tac::Kind_binary_operator;
  const K k = tac::kind(op);
  if (fp && (k == K::Add || k == K::Subtract || k == K::Multiply || k == K::Divide)) {
    static const char* const kFloat[] = {"fadd", "fsub", "fmul", "fdiv"};
    static const char* const kDouble[] = {"dadd", "dsub", "dmul", "ddiv"};
    const int i = k == K::Add ? 0 : k == K::Subtract ? 1 : k == K::Multiply ? 2 : 3;
    const bool single = size == 4;
    return translate_helper_call({{src1_op, size}, {src2_op, size}},
                                 single ? kFloat[i] : kDouble[i], single ? 8 : 16, size, dst_op);
  }
  if (!(size == 1 || size == 2 || size == 4 || size == 8) &&
      (k == K::Multiply || k == K::Divide || k == K::Modulo || k == K::LeftShift ||
       k == K::RightShift)) {
    if (auto widened = odd_width_helper_op(*this, op, src1, src2, src1_op, src2_op, dst_op, size)) {
      return std::move(*widened);
    }
  }
  // The output offset of a helper of 1, 2, 4, or 8 bytes, from the
  // offsets of each width.
  const auto by_size = [size](int o1, int o2, int o4, int o8) {
    return size == 1 ? o1 : size == 2 ? o2 : size == 4 ? o4 : o8;
  };
  const auto width_name = [size](const std::string& stem) {
    return stem + std::to_string(size == 1 ? 8 : size == 2 ? 16 : size == 4 ? 32 : 64);
  };
  switch (k) {
    case K::BcdAdd:
    case K::BcdSubtract: {
      // One instruction for the whole decimal-mode chain, so that no asm
      // pass can put code between its SED and CLD
      // (passes/decimal_arith.py).
      ir::DecimalArith d{.subtract = k == K::BcdSubtract};
      for (int b = 0; b < size; ++b) {
        d.left.push_back(byte_at(src1_op, b));
        d.right.push_back(byte_at(src2_op, b));
        d.dst.push_back(byte_at(dst_op, b));
      }
      return {d};
    }
    case K::Add:
      return translate_add_sub(src1_op, src2_op, dst_op, size, true);
    case K::Subtract:
      return translate_add_sub(src1_op, src2_op, dst_op, size, false);
    case K::Multiply: {
      // Each of the four mul* helpers writes a result of the operands'
      // width (1, 2, 4, or 8 bytes) in the low part of its output area.
      // The high half of A*B is dropped, since C's multiplications wrap
      // under §6.5.5.4's modular semantics. mul8 writes HARGS+2, mul16
      // HARGS+4..5, mul32 HARGS+8..11, and mul64 HARGS+16..23.
      if (size == 2 && byte_valued(*this, src1) && byte_valued(*this, src2)) {
        // Two bytes, as `narrow_byte_range` leaves a product of two
        // values from 0 to 255.
        ir::Type_operand a = src1_op;
        ir::Type_operand b = src2_op;
        if (isa<tac::Constant>(src2)) {
          std::swap(a, b);
        }
        const std::string loop = make_label("mul_loop");
        const std::string skip = make_label("mul_skip");
        return inline_byte_product(a, b, dst_op, loop, skip);
      }
      return translate_helper_call({{src1_op, size}, {src2_op, size}}, width_name("mul"),
                                   by_size(2, 4, 8, 16), size, dst_op);
    }
    case K::Divide:
    case K::Modulo: {
      // `/` and `%` go to `sdivmod*` for signed operands and `udivmod*`
      // for unsigned ones. The quotient sits at HARGS + 2 * width, right
      // after the inputs, and the remainder at HARGS + 3 * width. After
      // the C99 §6.3.1.8 promotions both operands share a type, so the
      // test of either one's signedness is enough.
      const bool divide = k == K::Divide;
      if (const auto divisor = byte_divisor(*this, src1, src2, size)) {
        const std::string loop = make_label("div_loop");
        return inline_byte_divide(src1_op, *divisor,
                                  divide ? std::optional<ir::Type_operand>(dst_op) : std::nullopt,
                                  divide ? std::nullopt : std::optional<ir::Type_operand>(dst_op),
                                  loop);
      }
      const std::string helper = width_name(is_unsigned_val(src1) ? "udivmod" : "sdivmod");
      const int out_off = divide ? by_size(2, 4, 8, 16) : by_size(3, 6, 12, 24);
      return translate_helper_call({{src1_op, size}, {src2_op, size}}, helper, out_off, size,
                                   dst_op);
    }
    case K::BitwiseAnd:
      return translate_bitwise(src1_op, src2_op, dst_op, size, true);
    case K::BitwiseOr:
      return translate_bitwise(src1_op, src2_op, dst_op, size, false);
    case K::BitwiseXor:
      return translate_xor(src1_op, src2_op, dst_op, size);
    case K::LeftShift: {
      // A constant count that `inline_shift_ok` accepts is inlined as
      // byte moves and a short ASL / ROL chain (passes/shift_cost.h),
      // which avoids the helper call and its constraint across the call
      // on the value's color. A shift by size*8 or more is undefined by
      // C99 §6.5.7.4, so the helper is fine for it.
      const auto cnt = shift_count_const_value(src2);
      // A value whose bytes above the low one are zero, such as a
      // widened byte, shifts as one byte (passes/significant_bytes.h).
      const auto* s1 = as<tac::Var>(src1);
      const auto* dv = as<tac::Var>(dst);
      if (cnt && size > 1 && 0 < *cnt && *cnt < size * 8 && (*cnt & 7) != 0 && s1 != nullptr &&
          dv != nullptr && s1->name != dv->name) {
        const auto sig = significant.find(s1->name);
        if (sig != significant.end() && sig->second == 1) {
          return inline_left_shift_byte(src1_op, dst_op, size, static_cast<int>(*cnt));
        }
      }
      if (cnt && passes::inline_shift_ok(size, small_count(*cnt))) {
        return inline_left_shift_const(src1_op, dst_op, size, static_cast<int>(*cnt));
      }
      if (size <= 2) {
        return shift_loop_of(*this, src2, src1_op, src2_op, dst, dst_op, size, ShiftKind::kAsl);
      }
      // asl8 takes a 1-byte value and count and writes HARGS+2, asl16 a
      // 2-byte value and writes HARGS+3, asl32 HARGS+5, and asl64
      // HARGS+9. By C99 §6.5.7.3 the result's type, and so the value's
      // width, is the promoted left operand's type, and the right
      // operand promotes on its own and may have another width. Only its
      // low byte is passed, since a shift by the width in bits or more
      // is undefined.
      return translate_helper_call({{src1_op, size}, {src2_op, 1}}, width_name("asl"),
                                   by_size(2, 3, 5, 9), size, dst_op);
    }
    case K::RightShift: {
      // `>>` is arithmetic on signed operands (asr*) and logical on
      // unsigned ones (lsr*) by C99 §6.5.7.5, with the signedness of the
      // operand: the const class of a constant and the symbol table's
      // type of a variable. The inputs are laid out as for a left shift.
      //
      // A constant count inlines when `inline_shift_ok` says so: an
      // unsigned shift as byte moves and an LSR / ROR chain, and a signed
      // one (with no byte moves, since the vacated high bytes would need
      // a sign fill) as a ROR chain that captures the sign.
      const auto cnt = shift_count_const_value(src2);
      bool signed_shift = !is_unsigned_val(src1);
      const auto* s1 = as<tac::Var>(src1);
      if (signed_shift && s1 != nullptr) {
        const auto sig = significant.find(s1->name);
        if ((sig == significant.end() ? size : sig->second) < size) {
          // The top byte is zero, so the value isn't negative and shifts
          // as an unsigned one.
          signed_shift = false;
        }
      }
      // Only the low byte of the result is read, and it comes from two
      // bytes of the source, whatever the signedness
      // (passes/significant_bytes.h).
      const auto* dv = as<tac::Var>(dst);
      if (cnt && (*cnt & 7) != 0 && *cnt / 8 + 1 < size && s1 != nullptr && dv != nullptr &&
          s1->name != dv->name && low_byte_only.contains(dv->name)) {
        return inline_right_shift_to_byte(src1_op, dst_op, static_cast<int>(*cnt));
      }
      if (cnt && passes::inline_shift_ok(size, small_count(*cnt), signed_shift)) {
        return inline_right_shift_const(src1_op, dst_op, size, static_cast<int>(*cnt),
                                        signed_shift);
      }
      if (size <= 2) {
        return shift_loop_of(*this, src2, src1_op, src2_op, dst, dst_op, size,
                             signed_shift ? ShiftKind::kAsr : ShiftKind::kLsr);
      }
      return translate_helper_call({{src1_op, size}, {src2_op, 1}},
                                   width_name(signed_shift ? "asr" : "lsr"), by_size(2, 3, 5, 9),
                                   size, dst_op);
    }
    case K::Equal:
    case K::NotEqual: {
      const int bytes = equality_bytes(src1, src2, size, fp);
      return translate_equality(src1_op, src2_op, dst_op, bytes,
                                k == K::Equal ? ir::Type_condition{ir::EQ{}}
                                              : ir::Type_condition{ir::NE{}},
                                dst_size, fp);
    }
    case K::LessThan:
      // src1 < src2: src1 - src2, then MI (signed) or CC (unsigned, where
      // a clear carry after the SBC means a borrow, so left < right).
      if (fp) {
        return translate_fp_ordering(src1_op, src2_op, dst_op, size, dst_size, true);
      }
      if (unsigned_cmp) {
        return translate_unsigned_ordering(src1_op, src2_op, dst_op, size, ir::CC{}, dst_size);
      }
      return translate_signed_ordering(src1_op, src2_op, dst_op, size, ir::MI{}, dst_size);
    case K::GreaterOrEqual:
      // src1 >= src2: PL (signed) or CS (unsigned, no borrow).
      if (fp) {
        // a >= b is b <= a, and both are false when either operand is a
        // NaN.
        return translate_fp_ordering(src2_op, src1_op, dst_op, size, dst_size, false);
      }
      if (unsigned_cmp) {
        return translate_unsigned_ordering(src1_op, src2_op, dst_op, size, ir::CS{}, dst_size);
      }
      return translate_signed_ordering(src1_op, src2_op, dst_op, size, ir::PL{}, dst_size);
    case K::GreaterThan:
      // src1 > src2 is src2 < src1, the operands swapped.
      if (fp) {
        return translate_fp_ordering(src2_op, src1_op, dst_op, size, dst_size, true);
      }
      if (unsigned_cmp) {
        return translate_unsigned_ordering(src2_op, src1_op, dst_op, size, ir::CC{}, dst_size);
      }
      return translate_signed_ordering(src2_op, src1_op, dst_op, size, ir::MI{}, dst_size);
    case K::LessOrEqual:
      // src1 <= src2 is src2 >= src1.
      if (fp) {
        return translate_fp_ordering(src1_op, src2_op, dst_op, size, dst_size, false);
      }
      if (unsigned_cmp) {
        return translate_unsigned_ordering(src2_op, src1_op, dst_op, size, ir::CS{}, dst_size);
      }
      return translate_signed_ordering(src2_op, src1_op, dst_op, size, ir::PL{}, dst_size);
  }
  throw InternalError("unexpected binary operator");
}

}  // namespace c6502::tac_to_asm
