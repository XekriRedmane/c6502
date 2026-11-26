#include "passes/optimization/constant_folding.h"

#include <cstdint>
#include <string>
#include <utility>

#include "bit_precise.h"
#include "builtin_functions.h"
#include "c99_to_tac.h"
#include "errors.h"
#include "fp_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using bit_precise::ConstLayout;
using framework::PassContext;
using K = ir::Kind_instruction;
using CK = ir::Kind_const;
using BK = ir::Kind_binary_operator;
using UK = ir::Kind_unary_operator;
using TK = c99::Kind_data_type;

bool same_layout(const ConstLayout& a, const ConstLayout& b) {
  return a.bytes == b.bytes && a.is_signed == b.is_signed;
}

int bits_of(const ConstLayout& l) { return 8 * l.bytes; }

ir::Type_const make_int(const ConstLayout& l, asdl::Int value) {
  return bit_precise::with_value(bit_precise::make_const(l.bytes, l.is_signed, 0), value);
}

ir::Type_const wrap_int(const ConstLayout& l, asdl::Int value) {
  return make_int(l, bit_precise::wrap(value, bits_of(l), l.is_signed));
}

asdl::Int raw(const ir::Type_const& c) { return *bit_precise::raw_value(c); }

asdl::Int operand_value(const ir::Type_const& c) { return bit_precise::const_value(c); }

asdl::Int to_signed(asdl::Int value, int bits) { return bit_precise::wrap(value, bits, true); }

ir::Type_instruction copy_of(ir::Type_const c, const ir::Type_val& dst) {
  return ir::Copy{.src = ir::Constant{.const_ = std::move(c)}, .dst = dst};
}

const c99::Type_data_type& strip_qualifiers(const c99::Type_data_type& type) {
  const c99::Type_data_type* t = &type;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(t)) {
      t = &*v->referenced_type;
    } else {
      return *t;
    }
  }
}

std::optional<ConstLayout> dst_int_variant(const ir::Type_val& dst, const SymbolTable* symbols) {
  const auto* var = std::get_if<ir::Var>(&dst);
  if (symbols == nullptr || var == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols->find(var->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  switch (c99::kind(sym->type)) {
    case TK::SChar:
      return ConstLayout{1, true};
    case TK::Char:
    case TK::UChar:
      return ConstLayout{1, false};
    case TK::Int:
      return ConstLayout{2, true};
    case TK::UInt:
      return ConstLayout{2, false};
    case TK::Long:
      return ConstLayout{4, true};
    case TK::ULong:
      return ConstLayout{4, false};
    case TK::LongLong:
      return ConstLayout{8, true};
    case TK::ULongLong:
      return ConstLayout{8, false};
    case TK::BitInt: {
      const auto& b = std::get<c99::BitInt>(sym->type);
      return ConstLayout{static_cast<int>(bit_precise::storage_bytes(b.width)), b.is_signed};
    }
    default:
      return std::nullopt;
  }
}

std::optional<int> scalar_size(const c99::Type_data_type& t) {
  switch (c99::kind(t)) {
    case TK::Char:
    case TK::SChar:
    case TK::UChar:
      return 1;
    case TK::Int:
    case TK::UInt:
    case TK::Pointer:
      return 2;
    case TK::Long:
    case TK::ULong:
    case TK::Float:
      return 4;
    case TK::LongLong:
    case TK::ULongLong:
    case TK::Double:
      return 8;
    case TK::BitInt:
      return static_cast<int>(bit_precise::storage_bytes(std::get<c99::BitInt>(t).width));
    default:
      return std::nullopt;
  }
}

asdl::Int python_int(const ValueTree& v) {
  if (v.kind == ValueTree::Kind::kInt) {
    return v.integer;
  }
  const auto t = fp_arith::double_bits_to_int(asdl::Int(v.float_bits));
  const auto exact = t ? t->exact() : std::nullopt;
  if (!exact) {
    throw InternalError("cannot convert float NaN or infinity to integer");
  }
  return *exact;
}

const ValueTree* initial_value(std::string_view name, const SymbolTable& symbols,
                               const Symbol** sym_out) {
  const Symbol* sym = symbols.find(std::string(name));
  if (sym == nullptr) {
    return nullptr;
  }
  const auto* attrs = std::get_if<StaticAttr>(&sym->attrs);
  if (attrs == nullptr) {
    return nullptr;
  }
  const auto* initial = std::get_if<Initial>(&attrs->initial_value);
  if (initial == nullptr) {
    return nullptr;
  }
  *sym_out = sym;
  return &initial->value;
}

std::optional<ir::Type_instruction> fold_indexed_load(std::string_view name, asdl::Int byte_idx,
                                                      const ir::Type_val& dst,
                                                      const SymbolTable* symbols) {
  if (symbols == nullptr) {
    return std::nullopt;
  }
  const auto* dst_var = std::get_if<ir::Var>(&dst);
  if (dst_var == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = nullptr;
  const ValueTree* init = initial_value(name, *symbols, &sym);
  if (init == nullptr || init->kind != ValueTree::Kind::kTuple) {
    return std::nullopt;
  }
  const c99::Type_data_type& arr_t = strip_qualifiers(sym->type);
  if (!asdl::isa<c99::Array>(arr_t)) {
    return std::nullopt;
  }
  std::vector<asdl::Int> dim_sizes;
  const c99::Type_data_type* leaf = &arr_t;
  while (const auto* a = std::get_if<c99::Array>(leaf)) {
    dim_sizes.push_back(a->size);
    leaf = &*a->element_type;
  }
  bool has_const = false;
  for (;;) {
    if (asdl::isa<c99::Volatile>(*leaf)) {
      return std::nullopt;
    }
    if (const auto* c = std::get_if<c99::Const>(leaf)) {
      has_const = true;
      leaf = &*c->referenced_type;
      continue;
    }
    break;
  }
  if (!has_const) {
    return std::nullopt;
  }
  const std::optional<int> elem_size = scalar_size(*leaf);
  if (!elem_size) {
    return std::nullopt;
  }
  if (byte_idx < 0 || byte_idx % *elem_size != 0) {
    return std::nullopt;
  }
  const asdl::Int elem_idx = byte_idx / *elem_size;
  asdl::Int total = 1;
  for (const asdl::Int d : dim_sizes) {
    total *= d;
  }
  if (elem_idx >= total) {
    return std::nullopt;
  }
  const ValueTree* val = init;
  asdl::Int remaining = elem_idx;
  for (std::size_t level = 0; level < dim_sizes.size(); ++level) {
    asdl::Int inner = 1;
    for (std::size_t k = level + 1; k < dim_sizes.size(); ++k) {
      inner *= dim_sizes[k];
    }
    const asdl::Int cur = asdl::floor_div(remaining, inner);
    remaining = asdl::floor_mod(remaining, inner);
    if (val->kind != ValueTree::Kind::kTuple) {
      return std::nullopt;
    }
    if (cur >= asdl::Int(static_cast<std::uint64_t>(val->items.size()))) {
      return std::nullopt;
    }
    val = &val->items[static_cast<std::size_t>(cur)];
  }
  if (val->kind != ValueTree::Kind::kInt && val->kind != ValueTree::Kind::kFloat) {
    return std::nullopt;
  }
  const Symbol* dst_sym = symbols->find(dst_var->name);
  if (dst_sym == nullptr) {
    return std::nullopt;
  }
  const c99::Type_data_type& dst_t = strip_qualifiers(dst_sym->type);
  if (scalar_size(dst_t) != elem_size) {
    return std::nullopt;
  }
  ir::Type_const c;
  if (asdl::isa<c99::Float>(dst_t)) {
    c = ir::ConstFloat{python_int(*val)};
  } else if (asdl::isa<c99::Double>(dst_t)) {
    c = ir::ConstDouble{python_int(*val)};
  } else {
    c = c99_to_tac::tac_const_for(dst_t, python_int(*val));
  }
  return copy_of(std::move(c), dst);
}

std::optional<ir::Type_instruction> fold_plane_load(std::string_view name, asdl::Int first,
                                                    asdl::Int plane, const ir::Type_val& dst,
                                                    const SymbolTable* symbols) {
  const auto* dst_var = std::get_if<ir::Var>(&dst);
  if (symbols == nullptr || dst_var == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = nullptr;
  const ValueTree* values = initial_value(name, *symbols, &sym);
  if (values == nullptr) {
    return std::nullopt;
  }
  const auto* arr = std::get_if<c99::Array>(&sym->type);
  if (!(values->kind == ValueTree::Kind::kTuple && arr != nullptr &&
        asdl::isa<c99::Const>(*arr->element_type))) {
    return std::nullopt;
  }
  const Symbol* dst_sym = symbols->find(dst_var->name);
  if (dst_sym == nullptr) {
    return std::nullopt;
  }
  const c99::Type_data_type& dst_t = strip_qualifiers(dst_sym->type);
  const std::optional<int> width = scalar_size(dst_t);
  if (!width || asdl::isa<c99::Float, c99::Double>(dst_t)) {
    return std::nullopt;
  }
  const asdl::Int count = asdl::Int(static_cast<std::uint64_t>(values->items.size()));
  asdl::Int value = 0;
  for (int k = 0; k < *width; ++k) {
    const asdl::Int i = first + asdl::Int(k) * plane;
    if (!(i >= 0 && i < count)) {
      return std::nullopt;
    }
    const ValueTree& item = values->items[static_cast<std::size_t>(i)];
    if (item.kind != ValueTree::Kind::kInt) {
      return std::nullopt;
    }
    value |= (item.integer & 0xFF) << (8 * k);
  }
  bool is_signed = asdl::isa<c99::SChar, c99::Int, c99::Long, c99::LongLong>(dst_t);
  if (const auto* b = std::get_if<c99::BitInt>(&dst_t)) {
    is_signed = b->is_signed;
  }
  if (is_signed && value >= (asdl::Int(1) << (8 * *width - 1))) {
    value -= asdl::Int(1) << (8 * *width);
  }
  return copy_of(c99_to_tac::tac_const_for(dst_t, value), dst);
}

std::optional<ir::Type_const> fold_widen(const ir::Type_const& c, const ir::Type_val& dst,
                                         const SymbolTable* symbols, bool sign_extend) {
  const auto src = bit_precise::integer_const_layout(c);
  if (!src) {
    return std::nullopt;
  }
  const auto tgt = dst_int_variant(dst, symbols);
  if (!tgt) {
    return std::nullopt;
  }
  const int src_bits = bits_of(*src);
  if (bits_of(*tgt) <= src_bits) {
    return std::nullopt;
  }
  const asdl::Int value = sign_extend ? to_signed(raw(c), src_bits)
                                      : (raw(c) & ((asdl::Int(1) << src_bits) - 1));
  return wrap_int(*tgt, value);
}

std::optional<ir::Type_const> fold_truncate(const ir::Type_const& c, const ir::Type_val& dst,
                                            const SymbolTable* symbols) {
  const auto src = bit_precise::integer_const_layout(c);
  if (!src) {
    return std::nullopt;
  }
  const auto tgt = dst_int_variant(dst, symbols);
  if (!tgt) {
    return std::nullopt;
  }
  if (bits_of(*tgt) >= bits_of(*src)) {
    return std::nullopt;
  }
  return wrap_int(*tgt, raw(c));
}

std::optional<ir::Type_const> fold_int_to_fp(const ir::Type_const& c, bool target_double) {
  if (!bit_precise::integer_const_layout(c)) {
    return std::nullopt;
  }
  const asdl::Int value = operand_value(c);
  if (target_double) {
    return ir::ConstDouble{fp_arith::int_to_double_bits(value)};
  }
  return ir::ConstFloat{fp_arith::int_to_single_bits(value)};
}

std::optional<ir::Type_const> fold_fp_to_int(const ir::Type_const& c, const ir::Type_val& dst,
                                             const SymbolTable* symbols, bool source_double) {
  std::optional<fp_arith::TruncatedInt> value;
  if (source_double) {
    const auto* d = std::get_if<ir::ConstDouble>(&c);
    if (d == nullptr || !fp_arith::double_is_finite(d->bits)) {
      return std::nullopt;
    }
    value = fp_arith::double_bits_to_int(d->bits);
  } else {
    const auto* f = std::get_if<ir::ConstFloat>(&c);
    if (f == nullptr || !fp_arith::single_is_finite(f->bits)) {
      return std::nullopt;
    }
    value = fp_arith::single_bits_to_int(f->bits);
  }
  const auto tgt = dst_int_variant(dst, symbols);
  if (!tgt) {
    return std::nullopt;
  }
  return wrap_int(*tgt, value->wrapped(bits_of(*tgt)));
}

std::optional<ir::Type_const> fold_conversion(const ir::Type_instruction& instr,
                                              const ir::Type_const& c,
                                              const SymbolTable* symbols) {
  switch (ir::kind(instr)) {
    case K::SignExtend:
      return fold_widen(c, std::get<ir::SignExtend>(instr).dst, symbols, true);
    case K::ZeroExtend:
      return fold_widen(c, std::get<ir::ZeroExtend>(instr).dst, symbols, false);
    case K::Truncate:
      return fold_truncate(c, std::get<ir::Truncate>(instr).dst, symbols);
    case K::FloatToDouble: {
      const auto* f = std::get_if<ir::ConstFloat>(&c);
      if (f == nullptr) {
        return std::nullopt;
      }
      return ir::ConstDouble{fp_arith::single_bits_to_double_bits(f->bits)};
    }
    case K::DoubleToFloat: {
      const auto* d = std::get_if<ir::ConstDouble>(&c);
      if (d == nullptr) {
        return std::nullopt;
      }
      return ir::ConstFloat{fp_arith::double_bits_to_single_bits(d->bits)};
    }
    case K::IntToFloat:
      return fold_int_to_fp(c, false);
    case K::IntToDouble:
      return fold_int_to_fp(c, true);
    case K::FloatToInt:
      return fold_fp_to_int(c, std::get<ir::FloatToInt>(instr).dst, symbols, false);
    case K::DoubleToInt:
      return fold_fp_to_int(c, std::get<ir::DoubleToInt>(instr).dst, symbols, true);
    default:
      return std::nullopt;
  }
}

bool is_comparison(const ir::Type_binary_operator& op) {
  switch (ir::kind(op)) {
    case BK::Equal:
    case BK::NotEqual:
    case BK::LessThan:
    case BK::GreaterThan:
    case BK::LessOrEqual:
    case BK::GreaterOrEqual:
      return true;
    default:
      return false;
  }
}

ir::Type_const truth_as_dst(const ir::Type_const& c, const ir::Type_val& dst,
                            const SymbolTable* symbols) {
  const auto tgt = dst_int_variant(dst, symbols);
  const auto layout = bit_precise::integer_const_layout(c);
  if (!tgt || (layout && same_layout(*tgt, *layout))) {
    return c;
  }
  return wrap_int(*tgt, raw(c));
}

std::optional<ir::Type_const> fold_bcd(const ir::Type_binary_operator& op,
                                       const ir::Type_const& c1, const ir::Type_const& c2,
                                       const ir::Type_val& dst, const SymbolTable* symbols) {
  if (!bit_precise::integer_const_layout(c1) || !bit_precise::integer_const_layout(c2)) {
    return std::nullopt;
  }
  const auto tgt = dst_int_variant(dst, symbols);
  if (!tgt) {
    return std::nullopt;
  }
  const int nbytes = tgt->bytes;
  const asdl::Int mask = (asdl::Int(1) << (8 * nbytes)) - 1;
  const asdl::Int a = raw(c1) & mask;
  const asdl::Int b = raw(c2) & mask;
  const asdl::Int r = ir::kind(op) == BK::BcdAdd ? builtin_functions::bcd_add(a, b, nbytes)
                                                 : builtin_functions::bcd_sub(a, b, nbytes);
  return wrap_int(*tgt, r);
}

std::optional<ir::Type_const> fold_copy(const ir::Type_const& c, const ir::Type_val& dst,
                                        const SymbolTable* symbols) {
  const auto src = bit_precise::integer_const_layout(c);
  if (!src) {
    return std::nullopt;
  }
  const auto tgt = dst_int_variant(dst, symbols);
  if (!tgt || same_layout(*tgt, *src) || tgt->bytes != src->bytes) {
    return std::nullopt;
  }
  return wrap_int(*tgt, raw(c));
}

std::optional<bool> truth_value(const ir::Type_const& c) {
  if (bit_precise::integer_const_layout(c)) {
    return raw(c) != 0;
  }
  if (const auto* f = std::get_if<ir::ConstFloat>(&c)) {
    return fp_arith::single_is_truthy(f->bits);
  }
  if (const auto* d = std::get_if<ir::ConstDouble>(&c)) {
    return fp_arith::double_is_truthy(d->bits);
  }
  return std::nullopt;
}

ir::Type_const int_const(bool r) { return ir::ConstInt{r ? 1 : 0}; }

std::optional<ir::Type_const> fold_unary(const ir::Type_unary_operator& op,
                                         const ir::Type_const& c) {
  const auto layout = bit_precise::integer_const_layout(c);
  switch (ir::kind(op)) {
    case UK::BinToBcd:
      if (!layout) {
        return std::nullopt;
      }
      return ir::ConstULong{builtin_functions::bin_to_bcd(operand_value(c))};
    case UK::LogicalNot:
      if (layout) {
        return int_const(raw(c) == 0);
      }
      if (const auto* f = std::get_if<ir::ConstFloat>(&c)) {
        return int_const(fp_arith::single_is_zero(f->bits));
      }
      if (const auto* d = std::get_if<ir::ConstDouble>(&c)) {
        return int_const(fp_arith::double_is_zero(d->bits));
      }
      return std::nullopt;
    case UK::Negate:
      if (layout) {
        return wrap_int(*layout, -operand_value(c));
      }
      if (const auto* f = std::get_if<ir::ConstFloat>(&c)) {
        return ir::ConstFloat{fp_arith::single_negate(f->bits)};
      }
      if (const auto* d = std::get_if<ir::ConstDouble>(&c)) {
        return ir::ConstDouble{fp_arith::double_negate(d->bits)};
      }
      return std::nullopt;
    case UK::Complement:
      if (!layout) {
        return std::nullopt;
      }
      return wrap_int(*layout, ~operand_value(c));
  }
  return std::nullopt;
}

std::optional<ir::Type_const> fp_comparison_result(const ir::Type_binary_operator& op,
                                                   fp_arith::Order order) {
  using fp_arith::Order;
  switch (ir::kind(op)) {
    case BK::Equal:
      return int_const(order == Order::kEq);
    case BK::NotEqual:
      return int_const(order != Order::kEq);
    case BK::LessThan:
      return int_const(order == Order::kLt);
    case BK::GreaterThan:
      return int_const(order == Order::kGt);
    case BK::LessOrEqual:
      return int_const(order == Order::kLt || order == Order::kEq);
    case BK::GreaterOrEqual:
      return int_const(order == Order::kGt || order == Order::kEq);
    default:
      return std::nullopt;
  }
}

std::optional<ir::Type_const> fold_comparison(const ir::Type_binary_operator& op,
                                              const ir::Type_const& c1, const ir::Type_const& c2) {
  if (c1.index() != c2.index()) {
    return std::nullopt;
  }
  if (const auto* f = std::get_if<ir::ConstFloat>(&c1)) {
    return fp_comparison_result(op,
                                fp_arith::single_compare(f->bits, std::get<ir::ConstFloat>(c2).bits));
  }
  if (const auto* d = std::get_if<ir::ConstDouble>(&c1)) {
    return fp_comparison_result(
        op, fp_arith::double_compare(d->bits, std::get<ir::ConstDouble>(c2).bits));
  }
  if (!bit_precise::integer_const_layout(c1)) {
    return std::nullopt;
  }
  const asdl::Int a = operand_value(c1);
  const asdl::Int b = operand_value(c2);
  switch (ir::kind(op)) {
    case BK::Equal:
      return int_const(a == b);
    case BK::NotEqual:
      return int_const(a != b);
    case BK::LessThan:
      return int_const(a < b);
    case BK::GreaterThan:
      return int_const(a > b);
    case BK::LessOrEqual:
      return int_const(a <= b);
    case BK::GreaterOrEqual:
      return int_const(a >= b);
    default:
      return std::nullopt;
  }
}

std::optional<ir::Type_const> fold_shift(const ir::Type_binary_operator& op,
                                         const ir::Type_const& c1, const ir::Type_const& c2) {
  const auto layout = bit_precise::integer_const_layout(c1);
  if (!layout || !bit_precise::integer_const_layout(c2)) {
    return std::nullopt;
  }
  const int bits = bits_of(*layout);
  const asdl::Int count = operand_value(c2);
  if (count < 0 || count >= bits) {
    return std::nullopt;
  }
  const int n = static_cast<int>(count);
  if (ir::kind(op) == BK::LeftShift) {
    const std::uint64_t v = static_cast<std::uint64_t>(operand_value(c1)) << n;
    return wrap_int(*layout, asdl::Int(v));
  }
  if (!layout->is_signed) {
    return wrap_int(*layout, operand_value(c1) >> n);
  }
  return wrap_int(*layout, to_signed(raw(c1), bits) >> n);
}

std::optional<ir::Type_const> fold_fp_arith(const ir::Type_binary_operator& op,
                                            const ir::Type_const& c1, const ir::Type_const& c2) {
  const auto* f1 = std::get_if<ir::ConstFloat>(&c1);
  const auto* f2 = std::get_if<ir::ConstFloat>(&c2);
  if (f1 != nullptr && f2 != nullptr) {
    switch (ir::kind(op)) {
      case BK::Add:
        return ir::ConstFloat{fp_arith::single_add(f1->bits, f2->bits)};
      case BK::Subtract:
        return ir::ConstFloat{fp_arith::single_sub(f1->bits, f2->bits)};
      case BK::Multiply:
        return ir::ConstFloat{fp_arith::single_mul(f1->bits, f2->bits)};
      case BK::Divide:
        // IEEE 754 defines a division by zero: an infinity for a
        // nonzero numerator, and a NaN for 0/0.
        return ir::ConstFloat{fp_arith::single_div(f1->bits, f2->bits)};
      default:
        return std::nullopt;
    }
  }
  const auto* d1 = std::get_if<ir::ConstDouble>(&c1);
  const auto* d2 = std::get_if<ir::ConstDouble>(&c2);
  if (d1 != nullptr && d2 != nullptr) {
    switch (ir::kind(op)) {
      case BK::Add:
        return ir::ConstDouble{fp_arith::double_add(d1->bits, d2->bits)};
      case BK::Subtract:
        return ir::ConstDouble{fp_arith::double_sub(d1->bits, d2->bits)};
      case BK::Multiply:
        return ir::ConstDouble{fp_arith::double_mul(d1->bits, d2->bits)};
      case BK::Divide:
        return ir::ConstDouble{fp_arith::double_div(d1->bits, d2->bits)};
      default:
        return std::nullopt;
    }
  }
  return std::nullopt;
}

std::optional<ir::Type_const> fold_binary(const ir::Type_binary_operator& op,
                                          const ir::Type_const& in1, const ir::Type_const& in2) {
  if (is_comparison(op)) {
    return fold_comparison(op, in1, in2);
  }
  const BK k = ir::kind(op);
  if (k == BK::LeftShift || k == BK::RightShift) {
    return fold_shift(op, in1, in2);
  }
  if (k == BK::Add || k == BK::Subtract || k == BK::Multiply || k == BK::Divide) {
    if (auto r = fold_fp_arith(op, in1, in2)) {
      return r;
    }
  }
  auto l1 = bit_precise::integer_const_layout(in1);
  const auto l2 = bit_precise::integer_const_layout(in2);
  if (!l1 || !l2) {
    return std::nullopt;
  }
  ir::Type_const c1 = in1;
  ir::Type_const c2 = in2;
  if (in1.index() != in2.index()) {
    if (l1->bytes != l2->bytes) {
      return std::nullopt;
    }
    if (k == BK::Divide || k == BK::Modulo) {
      return std::nullopt;
    }
    const ConstLayout u{l1->bytes, false};
    const asdl::Int mask = (asdl::Int(1) << bits_of(u)) - 1;
    c1 = make_int(u, raw(in1) & mask);
    c2 = make_int(u, raw(in2) & mask);
    l1 = u;
  }
  const ConstLayout variant = *l1;
  const asdl::Int a = operand_value(c1);
  const asdl::Int b = operand_value(c2);
  switch (k) {
    case BK::Add:
      return wrap_int(variant, a + b);
    case BK::Subtract:
      return wrap_int(variant, a - b);
    case BK::Multiply: {
      const std::uint64_t p = static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b);
      return wrap_int(variant, asdl::Int(p));
    }
    case BK::Divide:
      if (b == 0) {
        return std::nullopt;
      }
      return wrap_int(variant, a / b);
    case BK::Modulo:
      if (b == 0) {
        return std::nullopt;
      }
      return wrap_int(variant, a % b);
    case BK::BitwiseAnd:
      return wrap_int(variant, a & b);
    case BK::BitwiseOr:
      return wrap_int(variant, a | b);
    case BK::BitwiseXor:
      return wrap_int(variant, a ^ b);
    default:
      return std::nullopt;
  }
}

const ir::Type_const* constant_of(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  return c != nullptr ? &c->const_ : nullptr;
}

std::optional<std::pair<const ir::Type_val*, const ir::Type_val*>> conversion_operands(
    const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
#define C6502_CONVERSION(Name)                                 \
  case K::Name: {                                              \
    const auto& i = std::get<ir::Name>(instr);                 \
    return std::make_pair(&i.src, &i.dst);                     \
  }
    C6502_CONVERSION(SignExtend)
    C6502_CONVERSION(ZeroExtend)
    C6502_CONVERSION(Truncate)
    C6502_CONVERSION(IntToFloat)
    C6502_CONVERSION(IntToDouble)
    C6502_CONVERSION(FloatToInt)
    C6502_CONVERSION(DoubleToInt)
    C6502_CONVERSION(FloatToDouble)
    C6502_CONVERSION(DoubleToFloat)
#undef C6502_CONVERSION
    default:
      return std::nullopt;
  }
}

constant_folding::FoldResult replaced(ir::Type_instruction instr) {
  return {constant_folding::FoldOutcome::kReplaced, std::move(instr)};
}

constexpr constant_folding::FoldResult kUnchanged{};
const constant_folding::FoldResult kDropped{constant_folding::FoldOutcome::kDropped,
                                            std::nullopt};

}  // namespace

namespace constant_folding {

bool is_integer_const(const ir::Type_const& c) {
  return bit_precise::integer_const_layout(c).has_value();
}

FoldResult fold(const ir::Type_instruction& instr, const SymbolTable* symbols) {
  switch (ir::kind(instr)) {
    case K::Unary: {
      const auto& u = std::get<ir::Unary>(instr);
      const ir::Type_const* c = constant_of(u.src);
      if (c == nullptr) {
        break;
      }
      auto res = fold_unary(u.op, *c);
      if (!res) {
        return kUnchanged;
      }
      if (ir::kind(u.op) == UK::LogicalNot) {
        res = truth_as_dst(*res, u.dst, symbols);
      }
      return replaced(copy_of(std::move(*res), u.dst));
    }
    case K::Binary: {
      const auto& b = std::get<ir::Binary>(instr);
      const ir::Type_const* c1 = constant_of(b.src1);
      const ir::Type_const* c2 = constant_of(b.src2);
      if (c1 == nullptr || c2 == nullptr) {
        break;
      }
      const BK k = ir::kind(b.op);
      if (k == BK::BcdAdd || k == BK::BcdSubtract) {
        auto res = fold_bcd(b.op, *c1, *c2, b.dst, symbols);
        if (!res) {
          return kUnchanged;
        }
        return replaced(copy_of(std::move(*res), b.dst));
      }
      auto res = fold_binary(b.op, *c1, *c2);
      if (!res) {
        return kUnchanged;
      }
      if (is_comparison(b.op)) {
        res = truth_as_dst(*res, b.dst, symbols);
      }
      return replaced(copy_of(std::move(*res), b.dst));
    }
    case K::Copy: {
      const auto& cp = std::get<ir::Copy>(instr);
      const ir::Type_const* c = constant_of(cp.src);
      if (c == nullptr) {
        break;
      }
      auto res = fold_copy(*c, cp.dst, symbols);
      if (!res) {
        return kUnchanged;
      }
      return replaced(copy_of(std::move(*res), cp.dst));
    }
    case K::JumpIfTrue: {
      const auto& j = std::get<ir::JumpIfTrue>(instr);
      const ir::Type_const* c = constant_of(j.condition);
      if (c == nullptr) {
        break;
      }
      const auto tv = truth_value(*c);
      if (!tv) {
        return kUnchanged;
      }
      return *tv ? replaced(ir::Jump{.target = j.target}) : kDropped;
    }
    case K::JumpIfFalse: {
      const auto& j = std::get<ir::JumpIfFalse>(instr);
      const ir::Type_const* c = constant_of(j.condition);
      if (c == nullptr) {
        break;
      }
      const auto tv = truth_value(*c);
      if (!tv) {
        return kUnchanged;
      }
      return *tv ? kDropped : replaced(ir::Jump{.target = j.target});
    }
    case K::JumpIfCmp: {
      const auto& j = std::get<ir::JumpIfCmp>(instr);
      const ir::Type_const* c1 = constant_of(j.src1);
      const ir::Type_const* c2 = constant_of(j.src2);
      if (c1 == nullptr || c2 == nullptr) {
        break;
      }
      const auto res = fold_binary(j.op, *c1, *c2);
      if (!res) {
        return kUnchanged;
      }
      const auto tv = truth_value(*res);
      if (!tv) {
        return kUnchanged;
      }
      return *tv ? replaced(ir::Jump{.target = j.target}) : kDropped;
    }
    case K::IndexedLoad: {
      const auto& load = std::get<ir::IndexedLoad>(instr);
      const ir::Type_const* c = constant_of(load.index);
      if (c == nullptr) {
        break;
      }
      if (load.is_volatile) {
        return kUnchanged;
      }
      if (!is_integer_const(*c)) {
        return kUnchanged;
      }
      std::optional<ir::Type_instruction> res;
      if (load.plane != 0) {
        res = fold_plane_load(load.name, raw(*c) + load.offset, load.plane, load.dst, symbols);
      } else {
        res = fold_indexed_load(load.name, raw(*c) + load.offset, load.dst, symbols);
      }
      return res ? replaced(std::move(*res)) : kUnchanged;
    }
    case K::Phi: {
      const auto& phi = std::get<ir::Phi>(instr);
      if (phi.args.empty()) {
        break;
      }
      const ir::Type_val& first = std::get<ir::PhiArg>(phi.args.front()).source;
      for (std::size_t k = 1; k < phi.args.size(); ++k) {
        if (!(std::get<ir::PhiArg>(phi.args[k]).source == first)) {
          return kUnchanged;
        }
      }
      return replaced(ir::Copy{.src = first, .dst = phi.dst});
    }
    default:
      break;
  }
  if (const auto ops = conversion_operands(instr)) {
    if (const ir::Type_const* c = constant_of(*ops->first)) {
      if (auto res = fold_conversion(instr, *c, symbols)) {
        return replaced(copy_of(std::move(*res), *ops->second));
      }
    }
  }
  return kUnchanged;
}

}  // namespace constant_folding

tac_ast::Function constant_fold(tac_ast::Function fn, const PassContext& ctx) {
  return framework::run_window(
      std::move(fn), 1,
      [&](const framework::Instrs& instrs, std::size_t i) -> std::optional<framework::Instrs> {
        constant_folding::FoldResult r = constant_folding::fold(instrs[i], ctx.symbols);
        switch (r.outcome) {
          case constant_folding::FoldOutcome::kUnchanged:
            return std::nullopt;
          case constant_folding::FoldOutcome::kDropped:
            return framework::Instrs{};
          case constant_folding::FoldOutcome::kReplaced:
            break;
        }
        framework::Instrs out;
        out.push_back(std::move(*r.instr));
        return out;
      });
}

}  // namespace c6502::passes
