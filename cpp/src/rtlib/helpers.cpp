#include "rtlib/helpers.h"

#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <utility>

#include "asdl/runtime.h"
#include "errors.h"

namespace c6502::rtlib {

namespace {

namespace ir = ::c6502::asm_ast;
using Instrs = std::vector<ir::Type_instruction>;

ir::Type_operand reg_a() { return ir::Reg{.reg = ir::A{}}; }
ir::Type_operand reg_x() { return ir::Reg{.reg = ir::X{}}; }
ir::Type_operand hargs(int k) { return ir::Data{.name = "HARGS", .offset = k}; }
ir::Type_operand imm(int v) { return ir::Imm{.value = v}; }

ir::Mov mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = std::move(src), .dst = std::move(dst), .is_volatile = false};
}
ir::Label label(const std::string& name) { return ir::Label{.name = name}; }
ir::Branch branch(ir::Type_condition cond, const std::string& target) {
  return ir::Branch{.cond = cond, .target = target};
}
ir::Ret ret() { return ir::Ret{}; }

void append(Instrs& out, const Instrs& more) { out.insert(out.end(), more.begin(), more.end()); }

ir::Function helper(const std::string& name, Instrs instructions) {
  return ir::Function{
      .name = name, .is_global = true, .params = {}, .instructions = std::move(instructions)};
}

// A = -A: EOR #$FF gives the one's complement, and CLC; ADC #1 the
// two's.
Instrs negate_a() {
  return {ir::Xor{.src1 = reg_a(), .src2 = imm(0xFF), .dst = reg_a()}, ir::ClearCarry{},
          ir::Add{.src = imm(1), .dst = reg_a()}};
}

// Copies the `n` bytes at HARGS+src.. to HARGS+dst.., through A.
Instrs copy_n_via_a(int src, int dst, int n) {
  Instrs out;
  for (int k = 0; k < n; ++k) {
    out.push_back(mov(hargs(src + k), reg_a()));
    out.push_back(mov(reg_a(), hargs(dst + k)));
  }
  return out;
}

// Negates the `n`-byte value at HARGS+off.. in place: each byte is
// inverted, 1 is added to the low byte with CLC; ADC #1, and the carry
// goes up through the others with ADC #0. EOR leaves C alone, so the
// carry survives the LDA and EOR between the adds.
Instrs negate_n_inplace(int off, int n) {
  Instrs out{mov(hargs(off), reg_a()),
             ir::Xor{.src1 = reg_a(), .src2 = imm(0xFF), .dst = reg_a()},
             ir::ClearCarry{},
             ir::Add{.src = imm(1), .dst = reg_a()},
             mov(reg_a(), hargs(off))};
  for (int k = 1; k < n; ++k) {
    out.push_back(mov(hargs(off + k), reg_a()));
    out.push_back(ir::Xor{.src1 = reg_a(), .src2 = imm(0xFF), .dst = reg_a()});
    out.push_back(ir::Add{.src = imm(0), .dst = reg_a()});
    out.push_back(mov(reg_a(), hargs(off + k)));
  }
  return out;
}

// The low byte of A * B, for A at HARGS+0 and B at HARGS+1, at HARGS+2,
// by shift and add over eight bits:
//
//     A_work := A; B_work := B; result := 0
//     for _ in 8:
//         if low bit of A_work: result += B_work
//         A_work >>= 1
//         B_work <<= 1
//
// The carry out of each add leaves the result's byte, so a product
// past 255 wraps, as C's unsigned arithmetic does. A_work and B_work
// are copies in HARGS+3 and HARGS+4, so the inputs survive.
ir::Function mul8() {
  const std::string loop = ".mul8_loop";
  const std::string skip = ".mul8_skip";
  return helper("mul8", {
                            mov(hargs(0), reg_a()),
                            mov(reg_a(), hargs(3)),
                            mov(hargs(1), reg_a()),
                            mov(reg_a(), hargs(4)),
                            mov(imm(0), reg_a()),
                            mov(reg_a(), hargs(2)),
                            mov(imm(8), reg_x()),
                            label(loop),
                            ir::LogicalShiftRight{.dst = hargs(3)},
                            branch(ir::CC{}, skip),
                            mov(hargs(2), reg_a()),
                            ir::ClearCarry{},
                            ir::Add{.src = hargs(4), .dst = reg_a()},
                            mov(reg_a(), hargs(2)),
                            label(skip),
                            ir::ArithmeticShiftLeft{.dst = hargs(4)},
                            ir::Dec{.dst = reg_x()},
                            branch(ir::NE{}, loop),
                            ret(),
                        });
}

// The low 2 bytes of A * B, for A at HARGS+0..1 and B at HARGS+2..3, at
// HARGS+4..5, by mul8's shift and add over sixteen bits, with A_work at
// HARGS+6..7 and B_work at HARGS+8..9.
ir::Function mul16() {
  const std::string loop = ".mul16_loop";
  const std::string skip = ".mul16_skip";
  return helper("mul16", {
                             mov(hargs(0), reg_a()),
                             mov(reg_a(), hargs(6)),
                             mov(hargs(1), reg_a()),
                             mov(reg_a(), hargs(7)),
                             mov(hargs(2), reg_a()),
                             mov(reg_a(), hargs(8)),
                             mov(hargs(3), reg_a()),
                             mov(reg_a(), hargs(9)),
                             mov(imm(0), reg_a()),
                             mov(reg_a(), hargs(4)),
                             mov(reg_a(), hargs(5)),
                             mov(imm(16), reg_x()),
                             label(loop),
                             // The high byte first, so its bit 0 goes
                             // through C into the low byte's bit 7, and
                             // A_work's low bit ends in C.
                             ir::LogicalShiftRight{.dst = hargs(7)},
                             ir::RotateRight{.dst = hargs(6)},
                             branch(ir::CC{}, skip),
                             ir::ClearCarry{},
                             mov(hargs(4), reg_a()),
                             ir::Add{.src = hargs(8), .dst = reg_a()},
                             mov(reg_a(), hargs(4)),
                             mov(hargs(5), reg_a()),
                             ir::Add{.src = hargs(9), .dst = reg_a()},
                             mov(reg_a(), hargs(5)),
                             label(skip),
                             ir::ArithmeticShiftLeft{.dst = hargs(8)},
                             ir::RotateLeft{.dst = hargs(9)},
                             ir::Dec{.dst = reg_x()},
                             branch(ir::NE{}, loop),
                             ret(),
                         });
}

// The low 4 bytes of A * B, for A at HARGS+0..3 and B at HARGS+4..7, at
// HARGS+8..11, by shift and add over 32 bits, with A_work at
// HARGS+12..15 and B_work at HARGS+16..19.
ir::Function mul32() {
  const std::string loop = ".mul32_loop";
  const std::string skip = ".mul32_skip";
  Instrs body = copy_n_via_a(0, 12, 4);
  append(body, copy_n_via_a(4, 16, 4));
  body.push_back(mov(imm(0), reg_a()));
  for (int k = 0; k < 4; ++k) body.push_back(mov(reg_a(), hargs(8 + k)));
  body.push_back(mov(imm(32), reg_x()));
  body.push_back(label(loop));
  body.push_back(ir::LogicalShiftRight{.dst = hargs(15)});
  body.push_back(ir::RotateRight{.dst = hargs(14)});
  body.push_back(ir::RotateRight{.dst = hargs(13)});
  body.push_back(ir::RotateRight{.dst = hargs(12)});
  body.push_back(branch(ir::CC{}, skip));
  body.push_back(ir::ClearCarry{});
  for (int k = 0; k < 4; ++k) {
    body.push_back(mov(hargs(8 + k), reg_a()));
    body.push_back(ir::Add{.src = hargs(16 + k), .dst = reg_a()});
    body.push_back(mov(reg_a(), hargs(8 + k)));
  }
  body.push_back(label(skip));
  body.push_back(ir::ArithmeticShiftLeft{.dst = hargs(16)});
  body.push_back(ir::RotateLeft{.dst = hargs(17)});
  body.push_back(ir::RotateLeft{.dst = hargs(18)});
  body.push_back(ir::RotateLeft{.dst = hargs(19)});
  body.push_back(ir::Dec{.dst = reg_x()});
  body.push_back(branch(ir::NE{}, loop));
  body.push_back(ret());
  return helper("mul32", std::move(body));
}

// The 8-bit shifts: the value at HARGS+0 shifted by the count at
// HARGS+1, at HARGS+2, by a loop of one-bit shifts of A. A count of 8
// or more, which C leaves undefined, gives 0 for `asl8` and `lsr8`, and
// the sign in every bit for `asr8`.
ir::Function shift8(const std::string& name, Instrs step) {
  const std::string loop = "." + name + "_loop";
  const std::string done = "." + name + "_done";
  Instrs body{mov(hargs(0), reg_a()), mov(hargs(1), reg_x()), branch(ir::EQ{}, done),
              label(loop)};
  append(body, step);
  body.push_back(ir::Dec{.dst = reg_x()});
  body.push_back(branch(ir::NE{}, loop));
  body.push_back(label(done));
  body.push_back(mov(reg_a(), hargs(2)));
  body.push_back(ret());
  return helper(name, std::move(body));
}

ir::Function asl8() { return shift8("asl8", {ir::ArithmeticShiftLeft{.dst = reg_a()}}); }

ir::Function lsr8() { return shift8("lsr8", {ir::LogicalShiftRight{.dst = reg_a()}}); }

// `CMP #$80` sets C to A's sign bit, and `ROR A` shifts it back in.
ir::Function asr8() {
  return shift8("asr8", {ir::Compare{.left = reg_a(), .right = imm(0x80)},
                         ir::RotateRight{.dst = reg_a()}});
}

// The 16- and 32-bit shifts: the `bytes`-byte value at HARGS+0.. shifted
// by the count at HARGS+bytes, at HARGS+bytes+1.., by a loop of `step`
// on the result, which starts as a copy of the value.
ir::Function shift_wide(const std::string& name, int bytes, Instrs step) {
  const std::string loop = "." + name + "_loop";
  const std::string done = "." + name + "_done";
  Instrs body = copy_n_via_a(0, bytes + 1, bytes);
  body.push_back(mov(hargs(bytes), reg_x()));
  body.push_back(branch(ir::EQ{}, done));
  body.push_back(label(loop));
  append(body, step);
  body.push_back(ir::Dec{.dst = reg_x()});
  body.push_back(branch(ir::NE{}, loop));
  body.push_back(label(done));
  body.push_back(ret());
  return helper(name, std::move(body));
}

// A shift left goes from the low byte up, ASL and then ROL through C.
Instrs shift_left_step(int low, int bytes) {
  Instrs out{ir::ArithmeticShiftLeft{.dst = hargs(low)}};
  for (int k = 1; k < bytes; ++k) out.push_back(ir::RotateLeft{.dst = hargs(low + k)});
  return out;
}

// A shift right goes from the top byte down, `top` and then ROR through
// C.
Instrs shift_right_step(ir::Type_instruction top, int low, int bytes) {
  Instrs out{std::move(top)};
  for (int k = bytes - 2; k >= 0; --k) out.push_back(ir::RotateRight{.dst = hargs(low + k)});
  return out;
}

ir::Function asl_wide(const std::string& name, int bytes) {
  return shift_wide(name, bytes, shift_left_step(bytes + 1, bytes));
}

ir::Function lsr_wide(const std::string& name, int bytes) {
  const int top = 2 * bytes;
  return shift_wide(name, bytes,
                    shift_right_step(ir::LogicalShiftRight{.dst = hargs(top)}, bytes + 1, bytes));
}

// `LDA top; ASL A` puts the sign in C, and the RORs shift it in from
// the top.
ir::Function asr_wide(const std::string& name, int bytes) {
  const int top = 2 * bytes;
  Instrs step{mov(hargs(top), reg_a()), ir::ArithmeticShiftLeft{.dst = reg_a()}};
  append(step, shift_right_step(ir::RotateRight{.dst = hargs(top)}, bytes + 1, bytes));
  return shift_wide(name, bytes, std::move(step));
}

// The quotient and remainder of N / D, for N at HARGS+0 and D at
// HARGS+1, at HARGS+2 and HARGS+3, by long division over eight bits:
//
//     quot := N; rem := 0
//     for _ in 8:
//         quot := quot << 1        ; its top bit into C
//         rem := (rem << 1) | C
//         if rem >= D:
//             rem -= D
//             quot bit 0 := 1
//
// The ASL of quot leaves bit 0 clear, so an INC sets it without a
// carry. A division by 0 gives a quotient of $FF and N as the
// remainder.
ir::Function udivmod8() {
  const std::string loop = ".udivmod8_loop";
  const std::string skip = ".udivmod8_skip";
  return helper("udivmod8", {
                                mov(hargs(0), reg_a()),
                                mov(reg_a(), hargs(2)),
                                mov(imm(0), reg_a()),
                                mov(reg_a(), hargs(3)),
                                mov(imm(8), reg_x()),
                                label(loop),
                                ir::ArithmeticShiftLeft{.dst = hargs(2)},
                                ir::RotateLeft{.dst = hargs(3)},
                                mov(hargs(3), reg_a()),
                                ir::Compare{.left = reg_a(), .right = hargs(1)},
                                branch(ir::CC{}, skip),
                                // The CMP left C set for the SBC.
                                ir::Sub{.src = hargs(1), .dst = reg_a()},
                                mov(reg_a(), hargs(3)),
                                ir::Inc{.dst = hargs(2)},
                                label(skip),
                                ir::Dec{.dst = reg_x()},
                                branch(ir::NE{}, loop),
                                ret(),
                            });
}

// The signed quotient and remainder, truncated toward zero as C99
// §6.5.5 gives them, by `udivmod8` on |N| and |D|. The quotient is
// negated when the signs differ, and the remainder takes N's sign. N
// and D are kept at HARGS+4 and HARGS+5 and put back at the end, since
// the inputs survive the call. For N = -128, -N is $80 again, which
// `udivmod8` reads as 128, so the result is right for every D but 0.
ir::Function sdivmod8() {
  const std::string n_pos = ".sdivmod8_n_pos";
  const std::string d_pos = ".sdivmod8_d_pos";
  const std::string quot_pos = ".sdivmod8_quot_pos";
  const std::string rem_pos = ".sdivmod8_rem_pos";
  Instrs body = copy_n_via_a(0, 4, 2);
  body.push_back(mov(hargs(0), reg_a()));
  body.push_back(branch(ir::PL{}, n_pos));
  append(body, negate_a());
  body.push_back(mov(reg_a(), hargs(0)));
  body.push_back(label(n_pos));
  body.push_back(mov(hargs(1), reg_a()));
  body.push_back(branch(ir::PL{}, d_pos));
  append(body, negate_a());
  body.push_back(mov(reg_a(), hargs(1)));
  body.push_back(label(d_pos));
  body.push_back(ir::Call{.name = "udivmod8"});
  body.push_back(mov(hargs(4), reg_a()));
  body.push_back(ir::Xor{.src1 = reg_a(), .src2 = hargs(5), .dst = reg_a()});
  body.push_back(branch(ir::PL{}, quot_pos));
  body.push_back(mov(hargs(2), reg_a()));
  append(body, negate_a());
  body.push_back(mov(reg_a(), hargs(2)));
  body.push_back(label(quot_pos));
  body.push_back(mov(hargs(4), reg_a()));
  body.push_back(branch(ir::PL{}, rem_pos));
  body.push_back(mov(hargs(3), reg_a()));
  append(body, negate_a());
  body.push_back(mov(reg_a(), hargs(3)));
  body.push_back(label(rem_pos));
  append(body, copy_n_via_a(4, 0, 2));
  body.push_back(ret());
  return helper("sdivmod8", std::move(body));
}

// The quotient and remainder of N / D, for N at HARGS+0..1 and D at
// HARGS+2..3, at HARGS+4..5 and HARGS+6..7. Each of sixteen steps
// shifts the 32-bit (rem:quot) left one bit and, when rem >= D,
// subtracts D from rem and sets quot's bit 0. HARGS+8 holds the
// tentative low byte of rem - D, since the SBC of the high byte needs
// A.
ir::Function udivmod16() {
  const std::string loop = ".udivmod16_loop";
  const std::string skip = ".udivmod16_skip";
  Instrs body = copy_n_via_a(0, 4, 2);
  append(body, {
                   mov(imm(0), reg_a()),
                   mov(reg_a(), hargs(6)),
                   mov(reg_a(), hargs(7)),
                   mov(imm(16), reg_x()),
                   label(loop),
                   ir::ArithmeticShiftLeft{.dst = hargs(4)},
                   ir::RotateLeft{.dst = hargs(5)},
                   ir::RotateLeft{.dst = hargs(6)},
                   ir::RotateLeft{.dst = hargs(7)},
                   ir::SetCarry{},
                   mov(hargs(6), reg_a()),
                   ir::Sub{.src = hargs(2), .dst = reg_a()},
                   mov(reg_a(), hargs(8)),
                   mov(hargs(7), reg_a()),
                   ir::Sub{.src = hargs(3), .dst = reg_a()},
                   branch(ir::CC{}, skip),
                   mov(reg_a(), hargs(7)),
                   mov(hargs(8), reg_a()),
                   mov(reg_a(), hargs(6)),
                   ir::Inc{.dst = hargs(4)},
                   label(skip),
                   ir::Dec{.dst = reg_x()},
                   branch(ir::NE{}, loop),
                   ret(),
               });
  return helper("udivmod16", std::move(body));
}

// The signed division of `bytes`-byte values, as `sdivmod8` but with
// the inputs' top bytes kept at HARGS+sign_n and HARGS+sign_d, outside
// the unsigned helper's footprint. The inputs are made positive in
// place and negated again at the end, since negation is its own inverse
// modulo 2^(8 * bytes).
ir::Function sdivmod_wide(int bytes, int sign_n, int sign_d) {
  const std::string name = "sdivmod" + std::to_string(8 * bytes);
  const std::string n_pos = "." + name + "_n_pos";
  const std::string d_pos = "." + name + "_d_pos";
  const std::string quot_pos = "." + name + "_quot_pos";
  const std::string rem_pos = "." + name + "_rem_pos";
  const std::string n_done = "." + name + "_n_done";
  const std::string d_done = "." + name + "_d_done";
  const int d = bytes;
  const int quot = 2 * bytes;
  const int rem = 3 * bytes;
  Instrs body{mov(hargs(bytes - 1), reg_a()), mov(reg_a(), hargs(sign_n)),
              branch(ir::PL{}, n_pos)};
  append(body, negate_n_inplace(0, bytes));
  append(body, {label(n_pos), mov(hargs(d + bytes - 1), reg_a()), mov(reg_a(), hargs(sign_d)),
                branch(ir::PL{}, d_pos)});
  append(body, negate_n_inplace(d, bytes));
  append(body, {label(d_pos), ir::Call{.name = "udivmod" + std::to_string(8 * bytes)},
                mov(hargs(sign_n), reg_a()),
                ir::Xor{.src1 = reg_a(), .src2 = hargs(sign_d), .dst = reg_a()},
                branch(ir::PL{}, quot_pos)});
  append(body, negate_n_inplace(quot, bytes));
  append(body, {label(quot_pos), mov(hargs(sign_n), reg_a()), branch(ir::PL{}, rem_pos)});
  append(body, negate_n_inplace(rem, bytes));
  append(body, {label(rem_pos), mov(hargs(sign_n), reg_a()), branch(ir::PL{}, n_done)});
  append(body, negate_n_inplace(0, bytes));
  append(body, {label(n_done), mov(hargs(sign_d), reg_a()), branch(ir::PL{}, d_done)});
  append(body, negate_n_inplace(d, bytes));
  append(body, {label(d_done), ret()});
  return helper(name, std::move(body));
}

// `udivmod16` over 32 bits: N at HARGS+0..3 and D at HARGS+4..7, the
// quotient at HARGS+8..11 and the remainder at HARGS+12..15, and the
// tentative rem - D at HARGS+16..19.
ir::Function udivmod32() {
  const std::string loop = ".udivmod32_loop";
  const std::string skip = ".udivmod32_skip";
  Instrs body = copy_n_via_a(0, 8, 4);
  body.push_back(mov(imm(0), reg_a()));
  for (int k = 0; k < 4; ++k) body.push_back(mov(reg_a(), hargs(12 + k)));
  body.push_back(mov(imm(32), reg_x()));
  body.push_back(label(loop));
  append(body, shift_left_step(8, 8));
  body.push_back(ir::SetCarry{});
  for (int k = 0; k < 4; ++k) {
    body.push_back(mov(hargs(12 + k), reg_a()));
    body.push_back(ir::Sub{.src = hargs(4 + k), .dst = reg_a()});
    body.push_back(mov(reg_a(), hargs(16 + k)));
  }
  body.push_back(branch(ir::CC{}, skip));
  append(body, copy_n_via_a(16, 12, 4));
  body.push_back(ir::Inc{.dst = hargs(8)});
  body.push_back(label(skip));
  body.push_back(ir::Dec{.dst = reg_x()});
  body.push_back(branch(ir::NE{}, loop));
  body.push_back(ret());
  return helper("udivmod32", std::move(body));
}

// Converts the 16-bit value at HARGS+0..1 to five packed BCD digits at
// HARGS+2..4: the ones and tens in HARGS+2, the hundreds and thousands
// in HARGS+3, and the ten-thousands in the low nibble of HARGS+4, whose
// high nibble is 0. The input survives, since the loop shifts a copy of
// it in HARGS+5..6.
//
// It doubles the BCD result in decimal mode once for each bit of the
// input, from the most significant bit down, with that bit as the carry
// into the doubling. That is, bcd(N) = 2 * bcd(N-1) + bit(16 - N),
// computed as an ADC of each BCD byte to itself. The input is at most
// 65535, so HARGS+4 is at most $06 before its last doubling, and no
// carry leaves it.
//
// The decimal flag is set from the SED before the loop to the CLD before
// the RTS. The ASL and ROL in the loop don't depend on it. An interrupt
// handler that runs during the loop runs with the flag set
// (docs/builtins.md, "Interrupts"). It takes 46 bytes, and a call takes
// 712 cycles with its JSR and RTS for every input, or 727 when the
// loop's BNE crosses a page.
ir::Function bin16_to_bcd() {
  const std::string loop = ".bin16_to_bcd_loop";
  Instrs body = copy_n_via_a(0, 5, 2);
  append(body, {
                   ir::SetDecimal{},
                   mov(imm(0), reg_a()),
                   mov(reg_a(), hargs(2)),
                   mov(reg_a(), hargs(3)),
                   mov(reg_a(), hargs(4)),
                   mov(imm(16), reg_x()),
                   label(loop),
                   ir::ArithmeticShiftLeft{.dst = hargs(5)},
                   ir::RotateLeft{.dst = hargs(6)},
               });
  for (int k = 2; k <= 4; ++k) {
    body.push_back(mov(hargs(k), reg_a()));
    body.push_back(ir::Add{.src = hargs(k), .dst = reg_a()});
    body.push_back(mov(reg_a(), hargs(k)));
  }
  append(body, {ir::Dec{.dst = reg_x()}, branch(ir::NE{}, loop), ir::ClearDecimal{}, ret()});
  return helper("bin16_to_bcd", std::move(body));
}

std::vector<ir::Function> build_helpers() {
  return {
      mul8(),
      mul16(),
      mul32(),
      udivmod8(),
      sdivmod8(),
      udivmod16(),
      sdivmod_wide(2, 9, 10),
      udivmod32(),
      sdivmod_wide(4, 20, 21),
      asl8(),
      asr8(),
      lsr8(),
      asl_wide("asl16", 2),
      asr_wide("asr16", 2),
      lsr_wide("lsr16", 2),
      asl_wide("asl32", 4),
      asr_wide("asr32", 4),
      lsr_wide("lsr32", 4),
      bin16_to_bcd(),
  };
}

}  // namespace

void check_acyclic(const std::vector<asm_ast::Function>& fns) {
  std::set<std::string, std::less<>> names;
  for (const ir::Function& fn : fns) names.insert(fn.name);
  std::map<std::string, std::set<std::string>, std::less<>> edges;
  for (const ir::Function& fn : fns) {
    std::set<std::string>& out = edges[fn.name];
    for (const ir::Type_instruction& instr : fn.instructions) {
      if (const auto* call = asdl::as<ir::Call>(instr); call && names.contains(call->name)) {
        out.insert(call->name);
      } else if (const auto* jump = asdl::as<ir::Jump>(instr);
                 jump && names.contains(jump->target)) {
        out.insert(jump->target);
      }
    }
  }
  std::set<std::string, std::less<>> done;
  std::vector<std::string> path;
  const std::function<void(const std::string&)> visit = [&](const std::string& name) {
    if (const auto it = std::find(path.begin(), path.end(), name); it != path.end()) {
      std::string cycle;
      for (auto k = it; k != path.end(); ++k) cycle += *k + " -> ";
      throw InternalError("the runtime helpers call each other in a cycle: " + cycle + name);
    }
    if (done.contains(name)) {
      return;
    }
    path.push_back(name);
    for (const std::string& callee : edges.at(name)) visit(callee);
    path.pop_back();
    done.insert(name);
  };
  for (const ir::Function& fn : fns) visit(fn.name);
}

const std::vector<asm_ast::Function>& all_helper_functions() {
  static const std::vector<asm_ast::Function> functions = [] {
    std::vector<ir::Function> fns = build_helpers();
    check_acyclic(fns);
    return fns;
  }();
  return functions;
}

std::string helper_dump() {
  ir::Program prog;
  for (const ir::Function& fn : all_helper_functions()) prog.top_level.emplace_back(fn);
  return asdl::dump(prog);
}

const std::vector<std::string>& decimal_mode_helpers() {
  static const std::vector<std::string> names{"bin16_to_bcd"};
  return names;
}

}  // namespace c6502::rtlib
