// Part C3 of tac_to_asm: the casts (sign and zero extension and the
// conversions between integers and floating types), the unary
// operators, the conditional jumps, and the comparisons, both those
// that make a 0 or 1 and the compare-and-branch of `JumpIfCmp`.
// src/tac_to_asm.cpp describes the lowerings as a whole.
#include <optional>
#include <string>
#include <tuple>
#include <utility>

#include "asdl/runtime.h"
#include "bit_precise.h"
#include "errors.h"
#include "passes/optimization/compare_subtract.h"
#include "tac_to_asm_translator.h"

namespace c6502::tac_to_asm {

using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

void append(Instrs& out, Instrs more) {
  out.insert(out.end(), std::make_move_iterator(more.begin()), std::make_move_iterator(more.end()));
}

// Returns the number of bits of the non-negative `v`, Python's
// `int.bit_length`.
int bit_length(asdl::Int v) {
  int n = 0;
  while (v != 0) {
    v >>= 1;
    ++n;
  }
  return n;
}

// Returns the `value` of a constant operand, as Python's
// `getattr(src.const, "value", None)` gives it, or none for a variable
// or a floating constant.
std::optional<asdl::Int> constant_value(const tac::Type_val& v) {
  if (const auto* c = as<tac::Constant>(v)) {
    return bit_precise::raw_value(c->const_);
  }
  return std::nullopt;
}

// Returns the ordering with its operands swapped, `k < x` as `x > k`
// (Python's `_MIRRORED`), or none for an operator that isn't an
// ordering.
std::optional<tac::Type_binary_operator> mirrored(const tac::Type_binary_operator& op) {
  switch (tac::kind(op)) {
    case tac::Kind_binary_operator::LessThan:
      return tac::GreaterThan{};
    case tac::Kind_binary_operator::GreaterThan:
      return tac::LessThan{};
    case tac::Kind_binary_operator::LessOrEqual:
      return tac::GreaterOrEqual{};
    case tac::Kind_binary_operator::GreaterOrEqual:
      return tac::LessOrEqual{};
    default:
      return std::nullopt;
  }
}

// An ordering and its operands, as the rewrites below give them back.
struct Compare {
  tac::Type_binary_operator op;
  tac::Type_val src1;
  tac::Type_val src2;
};

// Swaps a constant on the left of an ordering to the right, mirroring
// the operator.
void constant_to_right(Compare& c) {
  if (isa<tac::Constant>(c.src1) && !isa<tac::Constant>(c.src2)) {
    if (const auto m = mirrored(c.op)) {
      c.op = *m;
      std::swap(c.src1, c.src2);
    }
  }
}

// Returns whether `c` is `x > k` or `x <= k` with only the constant `k`
// on the right.
bool greater_or_at_most_constant(const Compare& c) {
  return isa<tac::GreaterThan, tac::LessOrEqual>(c.op) && isa<tac::Constant>(c.src2) &&
         !isa<tac::Constant>(c.src1);
}

// Returns `x > k` as `x >= k + 1` and `x <= k` as `x < k + 1`.
Compare bumped(const Compare& c, asdl::Int value) {
  const auto& k = std::get<tac::Constant>(c.src2);
  const tac::Type_val one_more = tac::Constant{bit_precise::with_value(k.const_, value + 1)};
  if (isa<tac::GreaterThan>(c.op)) {
    return {tac::GreaterOrEqual{}, c.src1, one_more};
  }
  return {tac::LessThan{}, c.src1, one_more};
}

// A 1-byte ordering `JumpIfCmp` with its constant on the right, as `<`
// or `>=`. The lowering of `>` and `<=` swaps the operands and loads the
// constant into A, `LDA #k; CMP x; BCC`, and then `x` has to be loaded
// again if anything after reads it. `x > k` is `x >= k + 1` and `x <= k`
// is `x < k + 1`, which compare `x` in A with `CMP #k+1`, unless `k` is
// the type's largest value, where `k + 1` wraps. A constant on the left
// swaps first.
Compare constant_on_right(Compare c, bool is_unsigned) {
  constant_to_right(c);
  if (!greater_or_at_most_constant(c)) {
    return c;
  }
  const auto value = constant_value(c.src2);
  if (!value) {
    return c;
  }
  if (is_unsigned) {
    if ((*value & 0xFF) == 0xFF) {
      return c;
    }
  } else if (*value >= 0x7F) {
    return c;
  }
  return bumped(c, *value);
}

// An unsigned ordering `JumpIfCmp` of `size` bytes (2 or more) with a
// constant, as `x < k` or `x >= k` with the constant on the right. `x >
// k` is `x >= k + 1` and `x <= k` is `x < k + 1`, unless `k` is the
// type's largest value, where `k + 1` wraps. The lowering then subtracts
// the constant from `x`, as it would `k` from `x` after swapping the
// operands, in as many bytes, and a constant whose low byte is 0 drops
// that byte's subtract in the peepholes: `e > 0xFF` on a `uint16_t` is
// `e >= 0x100`, whose test of the high byte against 1
// `redundant_compare` makes a `BNE`.
Compare variable_first(Compare c, int size) {
  constant_to_right(c);
  if (!greater_or_at_most_constant(c)) {
    return c;
  }
  const auto value = constant_value(c.src2);
  if (!value || !(0 <= *value && *value < (asdl::Int(1) << (8 * size)) - 1)) {
    return c;
  }
  return bumped(c, *value);
}

// The 8-bit conversion helpers by the integer type on the other side,
// Python's `_INT_TO_FLOAT`, `_INT_TO_DOUBLE`, `_FLOAT_TO_INT`, and
// `_DOUBLE_TO_INT`. A `char` operand never reaches them, since
// c99_to_tac converts it to `int` or `unsigned int` first.
std::string conversion_helper(const c99::Type_data_type& t, const char* prefix,
                              const char* suffix) {
  const char* name = nullptr;
  switch (c99::kind(t)) {
    case c99::Kind_data_type::Int:
      name = "i";
      break;
    case c99::Kind_data_type::UInt:
      name = "u";
      break;
    case c99::Kind_data_type::Long:
      name = "l";
      break;
    case c99::Kind_data_type::ULong:
      name = "ul";
      break;
    case c99::Kind_data_type::LongLong:
      name = "ll";
      break;
    case c99::Kind_data_type::ULongLong:
      name = "ull";
      break;
    default:
      throw InternalError("no FP conversion helper for the type " + asdl::dump(t));
  }
  return std::string(prefix) + name + suffix;
}

// Returns the c99 integer type of a variable without its qualifiers,
// which the conversion helpers' tables are keyed by. Constants don't
// reach here, since c99_to_tac folds the casts of constants.
const c99::Type_data_type& int_type_of(const Translator& t, const tac::Type_val& val) {
  const auto* var = as<tac::Var>(val);
  if (var == nullptr) {
    throw InternalError(
        "FP conversion helper dispatch requires a Var source/dst; got " + asdl::dump(val) +
        ". c99_to_tac should fold Constant casts.");
  }
  if (t.symbols == nullptr) {
    throw InternalError("FP conversion helper dispatch requires a symbol table");
  }
  const passes::Symbol* sym = t.symbols->find(var->name);
  if (sym == nullptr) {
    throw InternalError("unknown Var in FP conversion: " + var->name);
  }
  const c99::Type_data_type* type = &sym->type;
  while (true) {
    if (const auto* c = as<c99::Const>(*type)) {
      type = &*c->referenced_type;
    } else if (const auto* v = as<c99::Volatile>(*type)) {
      type = &*v->referenced_type;
    } else {
      return *type;
    }
  }
}

// Appends the store of the 0 or 1 in A to the low byte of `dst_op`, and
// zeros in its higher bytes, so the whole result of a comparison or a
// logical not is defined.
void store_compare_result(Instrs& out, const ir::Type_operand& dst_op, int dst_size) {
  out.push_back(ir::Mov{.src = reg_a(), .dst = dst_op});
  for (int k = 1; k < dst_size; ++k) {
    out.push_back(ir::Mov{.src = ir::Imm{0}, .dst = byte_at(dst_op, k)});
  }
}

// Returns the select of 0 or 1 into A on `cond`: `B<cond> true; LDA #0;
// JMP end; true: LDA #1; end:`.
Instrs select_zero_one(const ir::Type_condition& cond, const std::string& true_label,
                       const std::string& end_label) {
  return {ir::Branch{.cond = cond, .target = true_label}, ir::Mov{.src = ir::Imm{0}, .dst = reg_a()},
          ir::Jump{.target = end_label},                  ir::Label{.name = true_label},
          ir::Mov{.src = ir::Imm{1}, .dst = reg_a()},     ir::Label{.name = end_label}};
}

// `!x`: 1 if x is zero, 0 otherwise, in `dst_size` bytes. A multi-byte
// source ORs all its bytes together first, which is zero exactly when
// every byte is. Then the Z flag of the load chooses the 0 or 1, and the
// high bytes of the result are zero.
Instrs translate_logical_not(Translator& t, const ir::Type_operand& src_op,
                             const ir::Type_operand& dst_op, const tac::Type_val& src_val,
                             int dst_size) {
  const std::string true_label = t.make_label("lnot_true");
  const std::string end_label = t.make_label("lnot_end");
  const int size = t.size_of(src_val);
  Instrs out;
  if (size == 1) {
    out.push_back(ir::Mov{.src = src_op, .dst = reg_a()});
  } else {
    out.push_back(ir::Mov{.src = byte_at(src_op, 0), .dst = reg_a()});
    for (int k = 1; k < size; ++k) {
      out.push_back(ir::Or{.src = byte_at(src_op, k), .dst = reg_a()});
    }
  }
  append(out, select_zero_one(ir::EQ{}, true_label, end_label));
  store_compare_result(out, dst_op, dst_size);
  return out;
}

// Returns the 8-bit sequence of Complement or Negate on A, which leaves
// the result in A. LogicalNot has labels and another shape, so it is
// lowered apart.
Instrs unop_atoms(const tac::Type_unary_operator& op) {
  switch (tac::kind(op)) {
    case tac::Kind_unary_operator::Complement:
      // ~A = A XOR $FF
      return {ir::Xor{.src1 = reg_a(), .src2 = ir::Imm{0xFF}, .dst = reg_a()}};
    case tac::Kind_unary_operator::Negate:
      // -A = (~A) + 1, two's complement
      return {ir::Xor{.src1 = reg_a(), .src2 = ir::Imm{0xFF}, .dst = reg_a()}, ir::ClearCarry{},
              ir::Add{.src = ir::Imm{1}, .dst = reg_a()}};
    default:
      throw InternalError("unexpected unary operator: " + asdl::dump(op));
  }
}

// `src != 0` (`cond` NE) or `src == 0` (EQ) into the 1-byte `dst`. The
// bytes ORed together leave Z set exactly when `src` is 0. For `!=`, A
// is then already the 0 result, so only a nonzero value needs `LDA #1`:
//
//     LDA src+N-1; ORA src+N-2; ...; ORA src
//     BEQ end; LDA #1; end: STA dst
//
// A floating-point `src` drops its sign bit first (`AND #$7F` on the
// high byte), so -0.0 equals 0 as IEEE 754 says.
Instrs translate_zero_test(Translator& t, const ir::Type_operand& src_op,
                           const ir::Type_operand& dst_op, int size, const ir::Type_condition& cond,
                           bool fp) {
  Instrs out{ir::Mov{.src = byte_at(src_op, size - 1), .dst = reg_a()}};
  if (fp) {
    out.push_back(ir::And{.src = ir::Imm{0x7F}, .dst = reg_a()});
  }
  for (int k = size - 2; k >= 0; --k) {
    out.push_back(ir::Or{.src = byte_at(src_op, k), .dst = reg_a()});
  }
  const std::string end_label = t.make_label("cmp_end");
  if (isa<ir::NE>(cond)) {
    out.push_back(ir::Branch{.cond = ir::EQ{}, .target = end_label});
    out.push_back(ir::Mov{.src = ir::Imm{1}, .dst = reg_a()});
    out.push_back(ir::Label{.name = end_label});
  } else {
    const std::string true_label = t.make_label("cmp_true");
    append(out, select_zero_one(ir::EQ{}, true_label, end_label));
  }
  store_compare_result(out, dst_op, 1);
  return out;
}

// `src < 0` (or `src >= 0` when `non_negative`) for a signed `src`, as a
// 0 or 1 in `dst`. The answer is bit 7 of the high byte, which `ASL`
// moves into C and `ROL` into bit 0 of a zero:
//
//     LDA src+N-1; ASL; LDA #0; ROL [; EOR #1]; STA dst
//
// The subtract and the overflow correction of `translate_signed_ordering`
// take 13 bytes or more.
Instrs translate_sign_test(const ir::Type_operand& src_op, const ir::Type_operand& dst_op,
                           int size, bool non_negative, int dst_size) {
  Instrs out = carry_to_value({ir::Mov{.src = byte_at(src_op, size - 1), .dst = reg_a()},
                               ir::ArithmeticShiftLeft{.dst = reg_a()}},
                              non_negative);
  store_compare_result(out, dst_op, dst_size);
  return out;
}

// Returns the branch for a signed `x > 0` or `x <= 0` (either operand
// order), or none. `x` is positive when its top byte's sign bit is clear
// and some byte is nonzero:
//
//     x <= 0:  LDA top; BMI target; ORA ...; BEQ target
//     x > 0:   LDA top; BMI skip; ORA ...; BNE target; skip:
//
// The subtraction from 0 with the overflow test had taken 15 bytes for a
// 2-byte `x`, and `x >= 1` on a byte 9.
std::optional<Instrs> jcmp_signed_positive(Translator& t, Compare c, int size,
                                           const std::string& target) {
  constant_to_right(c);
  if (!(isa<tac::GreaterThan, tac::LessOrEqual>(c.op) && isa<tac::Constant>(c.src2) &&
        constant_value(c.src2) == asdl::Int(0) && !isa<tac::Constant>(c.src1))) {
    return std::nullopt;
  }
  const ir::Type_operand x = translate_val(c.src1);
  Instrs out{ir::Mov{.src = byte_at(x, size - 1), .dst = reg_a()}};
  Instrs rest;
  for (int k = 0; k < size - 1; ++k) {
    rest.push_back(ir::Or{.src = byte_at(x, k), .dst = reg_a()});
  }
  if (isa<tac::LessOrEqual>(c.op)) {
    out.push_back(ir::Branch{.cond = ir::MI{}, .target = target});
    append(out, std::move(rest));
    out.push_back(ir::Branch{.cond = ir::EQ{}, .target = target});
    return out;
  }
  const std::string skip = t.make_label("jcmp_neg");
  out.push_back(ir::Branch{.cond = ir::MI{}, .target = skip});
  append(out, std::move(rest));
  out.push_back(ir::Branch{.cond = ir::NE{}, .target = target});
  out.push_back(ir::Label{.name = skip});
  return out;
}

// Returns the branch for an ordering `JumpIfCmp` right after
// `Binary(Subtract, a, b, t)` of the difference it compares
// (`compare_subtract::compared_difference`), or none. The subtract's
// last `SBC` leaves the flags the compare would set: an unsigned compare
// branches on C (`BCC` for `<`, `BCS` for `>=`), and a signed one on the
// sign of `t`'s top byte, corrected for overflow as
// `jcmp_signed_ordering` does. `t` mustn't be one of the operands, since
// the compare reads them after the subtract.
// passes/optimization/compare_subtract.py puts the subtract there.
std::optional<Instrs> fused_subtract(Translator& t, const Compare& c, const std::string& target,
                                     int size, bool is_unsigned) {
  if (!isa<tac::LessThan, tac::GreaterOrEqual, tac::GreaterThan, tac::LessOrEqual>(c.op)) {
    return std::nullopt;
  }
  const auto [a, b] = passes::compare_subtract::compared_difference(c.op, c.src1, c.src2);
  // Copies may come between, as SSA destruction puts a Phi's copies at
  // the end of the compare's block: their loads and stores leave C and V
  // as the subtract set them. None of them may write an operand, which
  // the compare reads after them, or the difference, whose top byte the
  // signed case reads.
  if (t.tac_list == nullptr) {
    return std::nullopt;
  }
  const auto& instrs = *t.tac_list;
  int k = t.tac_index - 1;
  std::vector<const tac::Copy*> between;
  while (k >= 0 && isa<tac::Copy>(instrs[k])) {
    between.push_back(&std::get<tac::Copy>(instrs[k]));
    --k;
  }
  const tac::Binary* prev = k >= 0 ? as<tac::Binary>(instrs[k]) : nullptr;
  for (const tac::Copy* copy : between) {
    if (copy->dst == *a || copy->dst == *b || (prev != nullptr && copy->dst == prev->dst)) {
      return std::nullopt;
    }
  }
  if (!(prev != nullptr && isa<tac::Subtract>(prev->op) && prev->src1 == *a && prev->src2 == *b &&
        isa<tac::Var>(prev->dst) && !(prev->dst == *a) && !(prev->dst == *b) &&
        t.size_of(prev->dst) == size)) {
    return std::nullopt;
  }
  const bool less = isa<tac::LessThan, tac::GreaterThan>(c.op);
  if (is_unsigned) {
    return Instrs{ir::Branch{.cond = less ? ir::Type_condition{ir::CC{}} : ir::Type_condition{ir::CS{}},
                             .target = target}};
  }
  // Signed: SBC sets V, and LDA leaves it, so the top byte is reloaded in
  // case anything between used A.
  const std::string novf_label = t.make_label("jcmp_novf");
  return Instrs{
      ir::Mov{.src = byte_at(translate_val(prev->dst), size - 1), .dst = reg_a()},
      ir::Branch{.cond = ir::VC{}, .target = novf_label},
      ir::Xor{.src1 = reg_a(), .src2 = ir::Imm{0x80}, .dst = reg_a()},
      ir::Label{.name = novf_label},
      ir::Branch{.cond = less ? ir::Type_condition{ir::MI{}} : ir::Type_condition{ir::PL{}},
                 .target = target}};
}

// Returns the floating ordering helper of `size`-byte operands, `flt` or
// `fle` for 4 bytes and `dlt` or `dle` for 8, and sets `out_off` to the
// HARGS offset of its 1-byte result.
std::string fp_ordering_helper(int size, bool lt, int& out_off) {
  if (size == 4) {
    out_off = 8;
    return lt ? "flt" : "fle";
  }
  out_off = 16;
  return lt ? "dlt" : "dle";
}

// FP `<` or `<=` as a `JumpIfCmp`, through the same `flt`/`fle`/`dlt`/
// `dle` helper as `translate_fp_ordering`, reading the helper's 0 or 1
// result inline and branching on `!= 0` in place of writing it to a
// destination.
//
// The bit-pattern signed compare doesn't work for IEEE 754, since
// negative floats break the encoding's order across the sign bit, and
// the infinities and NaN trip the V correction. The helper gives the
// right order over the whole floating range. The caller swaps the
// operands for `>` and `>=`, which IEEE 754 allows, since both
// directions give false on NaN.
Instrs jcmp_fp_ordering(const ir::Type_operand& left_op, const ir::Type_operand& right_op,
                        int size, const std::string& target, bool lt) {
  int out_off = 0;
  const std::string helper = fp_ordering_helper(size, lt, out_off);
  Instrs out;
  // The operands go to HARGS+0..size-1 and HARGS+size..2*size-1, as
  // `translate_helper_call` stages them. That function isn't reused
  // here because it copies the result into a destination, and this
  // branches on the result.
  int slot = 0;
  for (const ir::Type_operand* src_op : {&left_op, &right_op}) {
    for (int k = 0; k < size; ++k) {
      out.push_back(ir::Mov{.src = byte_at(*src_op, k), .dst = reg_a()});
      out.push_back(ir::Mov{.src = reg_a(), .dst = hargs(slot)});
      ++slot;
    }
  }
  out.push_back(ir::Call{.name = helper});
  // The 1-byte 0 or 1 result goes into A, and the branch is on != 0.
  out.push_back(ir::Mov{.src = hargs(out_off), .dst = reg_a()});
  out.push_back(ir::Branch{.cond = ir::NE{}, .target = target});
  return out;
}

// `==` or `!=` as a direct branch. For NotEqual the branch is on the
// first byte that differs, so any BNE goes. For Equal every byte is
// compared, and a byte that differs skips past the final BEQ to a label
// after it, so the branch isn't taken.
//
// A comparison of several bytes with 0 ORs the bytes together and
// branches once, as `translate_cond_jump` does for a truth test: `LDA
// b0; ORA b1; ...; BEQ` is 2 bytes for each byte after the first, where
// the byte-by-byte chain is 4. The chain can stop at the first nonzero
// byte, so it's a few cycles faster when a high byte isn't 0, but `if
// (score == 0)` on a uint32_t was 6 bytes longer than `if ((b0 | b1 | b2
// | b3) == 0)`.
Instrs jcmp_equality(Translator& t, const ir::Type_operand& src1_op,
                     const ir::Type_operand& src2_op, int size, const std::string& target,
                     bool branch_on_eq) {
  Instrs out;
  const bool zero = imm_zero(src2_op) || imm_zero(src1_op);
  if (size > 1 && zero) {
    const ir::Type_operand& value_op = imm_zero(src2_op) ? src1_op : src2_op;
    out.push_back(ir::Mov{.src = byte_at(value_op, 0), .dst = reg_a()});
    for (int k = 1; k < size; ++k) {
      out.push_back(ir::Or{.src = byte_at(value_op, k), .dst = reg_a()});
    }
    out.push_back(ir::Branch{
        .cond = branch_on_eq ? ir::Type_condition{ir::EQ{}} : ir::Type_condition{ir::NE{}},
        .target = target});
    return out;
  }
  if (!branch_on_eq) {
    for (int k = size - 1; k >= 0; --k) {
      out.push_back(ir::Mov{.src = byte_at(src1_op, k), .dst = reg_a()});
      out.push_back(ir::Compare{.left = reg_a(), .right = byte_at(src2_op, k)});
      out.push_back(ir::Branch{.cond = ir::NE{}, .target = target});
    }
    return out;
  }
  if (size == 1) {
    out.push_back(ir::Mov{.src = src1_op, .dst = reg_a()});
    out.push_back(ir::Compare{.left = reg_a(), .right = src2_op});
    out.push_back(ir::Branch{.cond = ir::EQ{}, .target = target});
    return out;
  }
  const std::string skip_label = t.make_label("jcmp_skip");
  // Every higher byte: an inequality skips past the final BEQ.
  for (int k = size - 1; k >= 1; --k) {
    out.push_back(ir::Mov{.src = byte_at(src1_op, k), .dst = reg_a()});
    out.push_back(ir::Compare{.left = reg_a(), .right = byte_at(src2_op, k)});
    out.push_back(ir::Branch{.cond = ir::NE{}, .target = skip_label});
  }
  // The last byte: with every higher byte equal, Z after its compare is
  // the equality of the whole value.
  out.push_back(ir::Mov{.src = byte_at(src1_op, 0), .dst = reg_a()});
  out.push_back(ir::Compare{.left = reg_a(), .right = byte_at(src2_op, 0)});
  out.push_back(ir::Branch{.cond = ir::EQ{}, .target = target});
  out.push_back(ir::Label{.name = skip_label});
  return out;
}

// Unsigned ordering compare-and-branch. The 1-byte form uses CMP, which
// needs no SEC and leaves A alone, and a wider one the SBC chain with
// the carry threading through it. `branch_cond` is CC (<) or CS (>=),
// and the caller's operand swap covers > and <=.
Instrs jcmp_unsigned_ordering(const ir::Type_operand& left_op, const ir::Type_operand& right_op,
                              int size, const ir::Type_condition& branch_cond,
                              const std::string& target) {
  Instrs out;
  if (size == 1) {
    out.push_back(ir::Mov{.src = left_op, .dst = reg_a()});
    out.push_back(ir::Compare{.left = reg_a(), .right = right_op});
    out.push_back(ir::Branch{.cond = branch_cond, .target = target});
    return out;
  }
  out.push_back(ir::Mov{.src = byte_at(left_op, 0), .dst = reg_a()});
  out.push_back(ir::SetCarry{});
  out.push_back(ir::Sub{.src = byte_at(right_op, 0), .dst = reg_a()});
  for (int k = 1; k < size; ++k) {
    out.push_back(ir::Mov{.src = byte_at(left_op, k), .dst = reg_a()});
    out.push_back(ir::Sub{.src = byte_at(right_op, k), .dst = reg_a()});
  }
  out.push_back(ir::Branch{.cond = branch_cond, .target = target});
  return out;
}

// Signed ordering compare-and-branch: the SBC chain and the `BVC; EOR
// #$80` correction of V, then a branch on MI (<) or PL (>=). The
// caller's operand swap covers > and <=.
//
// When `right_op` is `Imm(0)` (the comparison is `signed_var <op> 0`
// after any swap by the caller), the SBC chain leaves each byte as it
// is: `SBC #0` with a carry in of 1 leaves the byte unchanged and passes
// C=1 on, so each byte's load sets its own N and Z and no borrow
// propagates. The final N is bit 7 of `left_op`'s high byte, the value's
// sign bit, and V can't overflow when subtracting zero, so the
// correction changes nothing. The branch is then `LDA hi(left_op);
// B<cond> target`, 2 instructions where the SBC form takes 6 or more.
// That holds for a 1-byte `signed char >= 0` (the tail of a rotated
// countdown loop) and for any wider signed compare with zero.
Instrs jcmp_signed_ordering(Translator& t, const ir::Type_operand& left_op,
                            const ir::Type_operand& right_op, int size,
                            const ir::Type_condition& branch_cond, const std::string& target) {
  if (imm_zero(right_op)) {
    return {ir::Mov{.src = byte_at(left_op, size - 1), .dst = reg_a()},
            ir::Branch{.cond = branch_cond, .target = target}};
  }
  const std::string novf_label = t.make_label("jcmp_novf");
  Instrs out{ir::Mov{.src = byte_at(left_op, 0), .dst = reg_a()}, ir::SetCarry{},
             ir::Sub{.src = byte_at(right_op, 0), .dst = reg_a()}};
  for (int k = 1; k < size; ++k) {
    out.push_back(ir::Mov{.src = byte_at(left_op, k), .dst = reg_a()});
    out.push_back(ir::Sub{.src = byte_at(right_op, k), .dst = reg_a()});
  }
  out.push_back(ir::Branch{.cond = ir::VC{}, .target = novf_label});
  out.push_back(ir::Xor{.src1 = reg_a(), .src2 = ir::Imm{0x80}, .dst = reg_a()});
  out.push_back(ir::Label{.name = novf_label});
  out.push_back(ir::Branch{.cond = branch_cond, .target = target});
  return out;
}

}  // namespace

// A signed widening, such as `int` to `long`. Each source byte is copied
// into the matching byte of dst, and then src's high byte is loaded into
// A and its sign chooses $00 (positive) or $FF (negative) for each of
// dst's remaining high bytes. Each use makes two labels, which the
// Translator's counter keeps unique.
//
// The copies are one `Mov(src.k, dst.k)` per byte, as
// `translate_copy`'s, so forward copy propagation can substitute
// through the chain. A isn't assumed to hold src's high byte after the
// copies: if src and dst color to the same zero-page slot, the
// self-Mov peephole drops the load and leaves A undefined. The explicit
// `Mov(src.high, A)` between the copies and the sign test loads A and
// sets N from bit 7 of src's high byte, since the LDA's flags come
// last in what emission writes, even for a source that needs an LDY
// first.
Instrs Translator::translate_sign_extend(const tac::Type_val& src, const tac::Type_val& dst) {
  const ir::Type_operand src_op = translate_val(src);
  const ir::Type_operand dst_op = translate_val(dst);
  const int src_w = size_of(src);
  const int tgt_w = size_of(dst);
  const std::string neg_label = make_label("sx_neg");
  const std::string end_label = make_label("sx_done");
  Instrs out;
  for (int k = 0; k < src_w; ++k) {
    out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = byte_at(dst_op, k)});
  }
  out.push_back(ir::Mov{.src = byte_at(src_op, src_w - 1), .dst = reg_a()});
  out.push_back(ir::Branch{.cond = ir::MI{}, .target = neg_label});
  out.push_back(ir::Mov{.src = ir::Imm{0x00}, .dst = reg_a()});
  out.push_back(ir::Jump{.target = end_label});
  out.push_back(ir::Label{.name = neg_label});
  out.push_back(ir::Mov{.src = ir::Imm{0xFF}, .dst = reg_a()});
  out.push_back(ir::Label{.name = end_label});
  for (int k = src_w; k < tgt_w; ++k) {
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
  }
  return out;
}

// An unsigned widening, such as `unsigned int` to `unsigned long`. Each
// source byte is copied unchanged, and a literal 0 goes into each of
// dst's remaining high bytes, with no branch.
//
// The copies are one `Mov(src.k, dst.k)` per byte, as
// `translate_copy`'s. Emission writes the LDA and STA where the pair of
// operands needs them, but the IR shows the direct links, and
// `Mov(Imm(0), dst[k])` is a copy source that forward copy propagation
// records, so a later `Mov(dst[k], consumer)` gets `Imm(0)` and the
// intermediate pseudo is dropped as dead.
Instrs Translator::translate_zero_extend(const tac::Type_val& src, const tac::Type_val& dst) {
  const ir::Type_operand src_op = translate_val(src);
  const ir::Type_operand dst_op = translate_val(dst);
  const int src_w = size_of(src);
  const int tgt_w = size_of(dst);
  Instrs out;
  for (int k = 0; k < src_w; ++k) {
    out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = byte_at(dst_op, k)});
  }
  for (int k = src_w; k < tgt_w; ++k) {
    out.push_back(ir::Mov{.src = ir::Imm{0x00}, .dst = byte_at(dst_op, k)});
  }
  return out;
}

// An integer to `float` or `double` through a runtime helper. The
// source's width comes from its symbol's type, and its signedness
// chooses between i2f and u2f (or i2d and u2d, and so on). The output
// follows the input in HARGS (the layout table in src/tac_to_asm.cpp).
Instrs Translator::translate_int_to_fp(const tac::Type_val& src, const tac::Type_val& dst,
                                       bool target_double) {
  const std::string helper = conversion_helper(int_type_of(*this, src), "", target_double ? "2d" : "2f");
  const int in_bytes = size_of(src);
  const int out_bytes = target_double ? 8 : 4;
  return translate_helper_call({{translate_val(src), in_bytes}}, helper, in_bytes, out_bytes,
                               translate_val(dst));
}

// A `float` or `double` to an integer through a runtime helper. The
// output's width and signedness come from dst's symbol's type. The
// helpers truncate toward zero, as C99 §6.3.1.4 specifies.
Instrs Translator::translate_fp_to_int(const tac::Type_val& src, const tac::Type_val& dst,
                                       bool source_double) {
  const std::string helper =
      conversion_helper(int_type_of(*this, dst), source_double ? "d2" : "f2", "");
  const int in_bytes = source_double ? 8 : 4;
  const int out_bytes = size_of(dst);
  return translate_helper_call({{translate_val(src), in_bytes}}, helper, in_bytes, out_bytes,
                               translate_val(dst));
}

Instrs Translator::translate_unary(const tac::Type_unary_operator& op, const tac::Type_val& src,
                                   const tac::Type_val& dst) {
  const ir::Type_operand src_op = translate_val(src);
  const ir::Type_operand dst_op = translate_val(dst);
  // `!x` is always an `int` (by the type checker), even for a `long`
  // operand. The other unary operators keep the type.
  if (isa<tac::LogicalNot>(op)) {
    return translate_logical_not(*this, src_op, dst_op, src, size_of(dst));
  }
  if (isa<tac::BinToBcd>(op)) {
    // `__c6502_bin_to_bcd`: the helper writes the three low bytes of the
    // `unsigned long`, and the top byte is 0.
    Instrs out = translate_helper_call({{src_op, 2}}, "bin16_to_bcd", 2, 3, dst_op);
    out.push_back(ir::Mov{.src = ir::Imm{0}, .dst = byte_at(dst_op, 3)});
    return out;
  }
  const int size = size_of(src);
  if (size == 1) {
    Instrs out{ir::Mov{.src = src_op, .dst = reg_a()}};
    append(out, unop_atoms(op));
    out.push_back(ir::Mov{.src = reg_a(), .dst = dst_op});
    return out;
  }
  // A Negate or Complement of more than one byte.
  if (isa<tac::Complement>(op)) {
    // ~X = X XOR $FF...FF. Each byte is independent, and XOR leaves the
    // carry alone, so the order of the bytes doesn't matter.
    Instrs out;
    for (int k = 0; k < size; ++k) {
      out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = reg_a()});
      out.push_back(ir::Xor{.src1 = reg_a(), .src2 = ir::Imm{0xFF}, .dst = reg_a()});
      out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
    }
    return out;
  }
  if (isa<tac::Negate>(op)) {
    if (is_fp_val(src)) {
      // An IEEE 754 negate flips the sign bit (bit 7 of the top byte)
      // and copies every other byte unchanged, since only the sign
      // changes in `-x`. A two's-complement negate would scramble the
      // representation, which isn't a binary numeral.
      Instrs out;
      for (int k = 0; k < size - 1; ++k) {
        out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = reg_a()});
        out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
      }
      out.push_back(ir::Mov{.src = byte_at(src_op, size - 1), .dst = reg_a()});
      out.push_back(ir::Xor{.src1 = reg_a(), .src2 = ir::Imm{0x80}, .dst = reg_a()});
      out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, size - 1)});
      return out;
    }
    // An integer negate is -X = 0 - X at the operand's width: SEC, then
    // `LDA #0; SBC x_k` for each byte, the borrow threading up from the
    // low byte. For 2 bytes that is 3 bytes and 4 cycles shorter than
    // complementing each byte and adding 1 (`EOR #$FF; CLC; ADC #1`,
    // then `EOR #$FF; ADC #0`).
    Instrs out{ir::SetCarry{}};
    for (int k = 0; k < size; ++k) {
      out.push_back(ir::Mov{.src = ir::Imm{0}, .dst = reg_a()});
      out.push_back(ir::Sub{.src = byte_at(src_op, k), .dst = reg_a()});
      out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
    }
    return out;
  }
  throw InternalError("unexpected unary operator: " + asdl::dump(op));
}

// A conditional jump on whether `cond` is nonzero. For one byte, the
// load sets Z and the branch follows. A wider condition ORs every byte,
// which is zero exactly when the whole value is, and Z shows that.
Instrs Translator::translate_cond_jump(const tac::Type_val& cond, const std::string& target,
                                       const ir::Type_condition& branch_cond) {
  const ir::Type_operand cond_op = translate_val(cond);
  const int size = size_of(cond);
  if (size == 1) {
    return {ir::Mov{.src = cond_op, .dst = reg_a()},
            ir::Branch{.cond = branch_cond, .target = target}};
  }
  Instrs out{ir::Mov{.src = byte_at(cond_op, 0), .dst = reg_a()}};
  for (int k = 1; k < size; ++k) {
    out.push_back(ir::Or{.src = byte_at(cond_op, k), .dst = reg_a()});
  }
  out.push_back(ir::Branch{.cond = branch_cond, .target = target});
  return out;
}

// `(val & mask) ? jump to target : fall through` as `LDA val; AND #mask;
// B(NE|EQ) target`. `val` is always 1 byte, since the pass that makes
// this node narrows wider operands first.
//
// A mask of 0x80 tests the sign bit, and the `and_sign_bit_branch`
// peephole turns the `LDA; AND #$80; B(EQ|NE)` into `LDA val; B(PL|MI)
// target`.
Instrs Translator::translate_jump_if_masked(const tac::Type_val& val, asdl::Int mask,
                                            bool jump_when_nonzero, const std::string& target) {
  const ir::Type_operand val_op = translate_val(val);
  if (mask < 0 || mask > 0xFF) {
    throw InternalError("JumpIfMasked mask must fit in 0..255; got " + asdl::int_to_string(mask));
  }
  return {ir::Mov{.src = byte_at(val_op, 0), .dst = reg_a()},
          ir::And{.src = ir::Imm{mask}, .dst = reg_a()},
          ir::Branch{.cond = jump_when_nonzero ? ir::Type_condition{ir::NE{}}
                                               : ir::Type_condition{ir::EQ{}},
                     .target = target}};
}

// Returns how many low bytes of `src1 == src2` (or `!=`) to compare:
// those that either operand can have nonzero, and at least one, since the
// bytes above them are zero in both (passes/significant_bytes.h).
// `(int)b & 0x80` against 0 is one byte. A floating-point compare uses
// every byte.
int Translator::equality_bytes(const tac::Type_val& src1, const tac::Type_val& src2, int size,
                               bool fp) {
  if (fp) {
    return size;
  }
  const auto count = [&](const tac::Type_val& val) -> int {
    if (const auto* c = as<tac::Constant>(val)) {
      const auto v = bit_precise::raw_value(c->const_);
      if (!v || *v < 0) {
        return size;
      }
      return (bit_length(*v) + 7) / 8;
    }
    const auto it = significant.find(std::get<tac::Var>(val).name);
    return it == significant.end() ? size : it->second;
  };
  return std::max(1, std::min(size, std::max(count(src1), count(src2))));
}

// `==` and `!=` through a byte-wise compare, a branch, and a 0 or 1. CMP
// sets Z exactly when the two bytes are equal, with no overflow to
// consider, so this holds for signed and unsigned bytes alike. For N
// bytes the walk goes from the high byte (N-1) down to the low one, and
// each byte but the last branches with BNE to a "differ" label, where
// Z=0 already means unequal. The low byte's CMP, with no branch out,
// leaves Z holding the answer.
//
// A floating equality (4 or 8 bytes) compared byte by byte matches IEEE
// 754 except in two cases: a NaN compares equal to itself byte by byte,
// where C and IEEE say `NaN != NaN`, and `-0.0` and `+0.0` differ byte by
// byte, where IEEE says they're equal. c6502 has no FP runtime helpers
// yet, so this is what can be done inline, and it is correct for every
// value that isn't a NaN or a zero.
//
// A comparison with 0 whose result is one byte (the conversion of a
// scalar to `bool`, or a comparison the optimizer narrowed) goes to
// `translate_zero_test` instead.
Instrs Translator::translate_equality(const ir::Type_operand& src1_op,
                                      const ir::Type_operand& src2_op,
                                      const ir::Type_operand& dst_op, int size,
                                      const ir::Type_condition& cond, int dst_size, bool fp) {
  if (dst_size == 1 && imm_zero(src2_op)) {
    return translate_zero_test(*this, src1_op, dst_op, size, cond, fp);
  }
  const std::string true_label = make_label("cmp_true");
  const std::string end_label = make_label("cmp_end");
  Instrs out;
  if (size == 1) {
    out.push_back(ir::Mov{.src = src1_op, .dst = reg_a()});
    out.push_back(ir::Compare{.left = reg_a(), .right = src2_op});
  } else {
    const std::string differ_label = make_label("cmp_differ");
    // The high bytes first. A byte that differs branches to
    // `differ_label` without looking at the rest.
    for (int k = size - 1; k >= 1; --k) {
      out.push_back(ir::Mov{.src = byte_at(src1_op, k), .dst = reg_a()});
      out.push_back(ir::Compare{.left = reg_a(), .right = byte_at(src2_op, k)});
      out.push_back(ir::Branch{.cond = ir::NE{}, .target = differ_label});
    }
    // The last byte (offset 0) has no branch out, and Z holds the answer.
    out.push_back(ir::Mov{.src = byte_at(src1_op, 0), .dst = reg_a()});
    out.push_back(ir::Compare{.left = reg_a(), .right = byte_at(src2_op, 0)});
    out.push_back(ir::Label{.name = differ_label});
  }
  append(out, select_zero_one(cond, true_label, end_label));
  store_compare_result(out, dst_op, dst_size);
  return out;
}

// A signed ordering through SBC with the correction of V:
//
//     LDA left[0]; SEC; SBC right[0]
//     LDA left[k];      SBC right[k]   for k in 1..size-1
//     BVC novf; EOR #$80; novf:
//     ASL A; LDA #0; ROL A [; EOR #1]; STA dst
//
// When the subtraction overflows, the N flag of SBC is the wrong sign of
// the difference, and EOR #$80 flips it. For several bytes the low bytes
// are subtracted first, with the carry threading through, and the V of
// the last (high) SBC is the overflow of the whole value and N its sign.
// The caller chooses the order of the operands and the condition among
// <, >=, >, and <=.
//
// `x < 0` and `x >= 0` (the right operand 0, `cond` MI or PL) go to
// `translate_sign_test`, since the result is x's sign bit.
Instrs Translator::translate_signed_ordering(const ir::Type_operand& left_op,
                                             const ir::Type_operand& right_op,
                                             const ir::Type_operand& dst_op, int size,
                                             const ir::Type_condition& cond, int dst_size) {
  if (imm_zero(right_op) && !isa<ir::Imm>(left_op) && isa<ir::MI, ir::PL>(cond)) {
    return translate_sign_test(left_op, dst_op, size, isa<ir::PL>(cond), dst_size);
  }
  const std::string novf_label = make_label("cmp_novf");
  Instrs out{ir::Mov{.src = byte_at(left_op, 0), .dst = reg_a()}, ir::SetCarry{},
             ir::Sub{.src = byte_at(right_op, 0), .dst = reg_a()}};
  for (int k = 1; k < size; ++k) {
    // Each later byte: the LDA keeps C from the SBC before it, and the
    // SBC threads the borrow on. After the loop N and V are those of the
    // whole subtraction.
    out.push_back(ir::Mov{.src = byte_at(left_op, k), .dst = reg_a()});
    out.push_back(ir::Sub{.src = byte_at(right_op, k), .dst = reg_a()});
  }
  // Bit 7 of A is N, corrected for overflow on both paths into `novf`,
  // so `ASL` moves the sign of the difference into C and `ROL` into bit
  // 0 of a zero: 4 bytes where `BMI true; LDA #0; JMP end; true: LDA #1`
  // took 9.
  out.push_back(ir::Branch{.cond = ir::VC{}, .target = novf_label});
  out.push_back(ir::Xor{.src1 = reg_a(), .src2 = ir::Imm{0x80}, .dst = reg_a()});
  out.push_back(ir::Label{.name = novf_label});
  append(out, carry_to_value({ir::ArithmeticShiftLeft{.dst = reg_a()}}, isa<ir::PL>(cond)));
  store_compare_result(out, dst_op, dst_size);
  return out;
}

// An unsigned ordering through SBC, with no correction of V:
//
//     LDA left[0]; SEC; SBC right[0]
//     LDA left[k];      SBC right[k]   for k in 1..size-1
//     LDA #0; ROL A; STA dst
//
// After SBC, C is the unsigned ordering: C=1 means no borrow (left >=
// right), and C=0 a borrow (left < right). For several bytes the borrow
// threads through the SBCs, so the last carry is that of the whole
// subtraction. The caller chooses the order of the operands and BCC or
// BCS among <, >=, >, and <=, and `LDA #0; ROL A` makes C the 0 or 1.
//
// For BCC, `left < right`, the operands swap and the subtract starts
// with `CLC`: `right - left - 1` borrows unless `right > left`, so C is
// the answer itself, where `SEC` and an `EOR #1` after the `ROL` had
// inverted it. A carry-flag return of the result then needs no `ROL` at
// all (`carry_branch_fold` drops `LDA #0; ROL A; CMP #1`).
Instrs Translator::translate_unsigned_ordering(const ir::Type_operand& left_op,
                                               const ir::Type_operand& right_op,
                                               const ir::Type_operand& dst_op, int size,
                                               const ir::Type_condition& cond, int dst_size) {
  const ir::Type_operand* left = &left_op;
  const ir::Type_operand* right = &right_op;
  ir::Type_instruction set_c = ir::SetCarry{};
  if (isa<ir::CC>(cond)) {
    std::swap(left, right);
    set_c = ir::ClearCarry{};
  }
  Instrs out{ir::Mov{.src = byte_at(*left, 0), .dst = reg_a()}, set_c,
             ir::Sub{.src = byte_at(*right, 0), .dst = reg_a()}};
  for (int k = 1; k < size; ++k) {
    out.push_back(ir::Mov{.src = byte_at(*left, k), .dst = reg_a()});
    out.push_back(ir::Sub{.src = byte_at(*right, k), .dst = reg_a()});
  }
  append(out, carry_to_value({}, false));
  store_compare_result(out, dst_op, dst_size);
  return out;
}

// The compare-and-branch of `JumpIfCmp`. The chain of byte compares has
// the shape of `translate_equality`'s and the orderings', but one
// `Branch` on the chosen condition goes straight to `target` in place of
// making a 0 or 1. The caller (`cmp_jump_fold`) has already inverted
// `op` when the original jump was a JumpIfFalse, so this always jumps
// when `op(src1, src2)` is true.
Instrs Translator::translate_jump_if_cmp(const tac::Type_binary_operator& op_in,
                                         const tac::Type_val& src1_in,
                                         const tac::Type_val& src2_in,
                                         const std::string& target) {
  Compare c{op_in, src1_in, src2_in};
  const int size = size_of(c.src1);
  const bool unsigned_cmp = is_unsigned_val(c.src1) || is_unsigned_val(c.src2);
  const bool pointer_cmp = is_pointer_val(c.src1) || is_pointer_val(c.src2);
  const bool fp = is_fp_val(c.src1) || is_fp_val(c.src2);
  const bool as_unsigned = unsigned_cmp || pointer_cmp;
  if (as_unsigned && !fp && size > 1) {
    c = variable_first(std::move(c), size);
  }
  ir::Type_operand src1_op = translate_val(c.src1);
  ir::Type_operand src2_op = translate_val(c.src2);
  if (!fp) {
    if (auto fused = fused_subtract(*this, c, target, size, as_unsigned)) {
      return std::move(*fused);
    }
    if (!as_unsigned) {
      if (auto vs_zero = jcmp_signed_positive(*this, c, size, target)) {
        return std::move(*vs_zero);
      }
    }
    if (size == 1) {
      c = constant_on_right(std::move(c), as_unsigned);
      src1_op = translate_val(c.src1);
      src2_op = translate_val(c.src2);
    }
  }
  switch (tac::kind(c.op)) {
    case tac::Kind_binary_operator::Equal:
      return jcmp_equality(*this, src1_op, src2_op, equality_bytes(c.src1, c.src2, size, fp),
                           target, true);
    case tac::Kind_binary_operator::NotEqual:
      return jcmp_equality(*this, src1_op, src2_op, equality_bytes(c.src1, c.src2, size, fp),
                           target, false);
    case tac::Kind_binary_operator::LessThan:
      if (fp) {
        return jcmp_fp_ordering(src1_op, src2_op, size, target, true);
      }
      if (as_unsigned) {
        return jcmp_unsigned_ordering(src1_op, src2_op, size, ir::CC{}, target);
      }
      return jcmp_signed_ordering(*this, src1_op, src2_op, size, ir::MI{}, target);
    case tac::Kind_binary_operator::GreaterOrEqual:
      if (fp) {
        // a >= b is b <= a, which is safe with NaN.
        return jcmp_fp_ordering(src2_op, src1_op, size, target, false);
      }
      if (as_unsigned) {
        return jcmp_unsigned_ordering(src1_op, src2_op, size, ir::CS{}, target);
      }
      return jcmp_signed_ordering(*this, src1_op, src2_op, size, ir::PL{}, target);
    case tac::Kind_binary_operator::GreaterThan:
      // a > b is b < a, the operand swap of the Binary case.
      if (fp) {
        return jcmp_fp_ordering(src2_op, src1_op, size, target, true);
      }
      if (as_unsigned) {
        return jcmp_unsigned_ordering(src2_op, src1_op, size, ir::CC{}, target);
      }
      return jcmp_signed_ordering(*this, src2_op, src1_op, size, ir::MI{}, target);
    case tac::Kind_binary_operator::LessOrEqual:
      // a <= b is b >= a.
      if (fp) {
        return jcmp_fp_ordering(src1_op, src2_op, size, target, false);
      }
      if (as_unsigned) {
        return jcmp_unsigned_ordering(src2_op, src1_op, size, ir::CS{}, target);
      }
      return jcmp_signed_ordering(*this, src2_op, src1_op, size, ir::PL{}, target);
    default:
      throw InternalError("unsupported JumpIfCmp op: " + asdl::dump(c.op));
  }
}

// FP `<` (`lt`) or `<=` through a runtime helper. The bit-pattern
// compare doesn't follow IEEE 754's order for negative values, whose
// encoding runs the other way across the sign bit, and the signed
// integer compare goes wrong on the infinities and NaN, so an ordering
// goes through `flt`/`fle` (`float`, 4 bytes) or `dlt`/`dle` (`double`,
// 8 bytes). Each takes A at HARGS+0..N-1 and B at HARGS+N..2N-1 and
// writes one 0 or 1 byte to the FP result slot, HARGS+8 for `float` and
// HARGS+16 for `double`, as the FP arithmetic helpers do. The caller
// swaps the operands for `>` (as `<`) and `>=` (as `<=`), which IEEE 754
// allows, since both directions give false when either operand is NaN.
Instrs Translator::translate_fp_ordering(const ir::Type_operand& left_op,
                                         const ir::Type_operand& right_op,
                                         const ir::Type_operand& dst_op, int size, int dst_size,
                                         bool lt) {
  int out_off = 0;
  const std::string helper = fp_ordering_helper(size, lt, out_off);
  Instrs out = translate_helper_call({{left_op, size}, {right_op, size}}, helper, out_off, 1, dst_op);
  // The helper writes 1 byte, and the rest of the `int` result is zero.
  for (int k = 1; k < dst_size; ++k) {
    out.push_back(ir::Mov{.src = ir::Imm{0}, .dst = byte_at(dst_op, k)});
  }
  return out;
}

}  // namespace c6502::tac_to_asm
