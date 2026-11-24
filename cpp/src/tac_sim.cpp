#include "tac_sim.h"

#include <algorithm>
#include <cstdio>
#include <memory>
#include <utility>

#include "bit_precise.h"
#include "builtin_functions.h"
#include "errors.h"
#include "fp_arith.h"
#include "pyrepr.h"

namespace c6502::tac_sim {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;

// The addresses held in `Memory::low_`.
constexpr std::int64_t kLowLimit = std::int64_t{1} << 24;

[[noreturn]] void fail(const std::string& message) { throw SimError("TacSimError", message); }

// Raises the `KeyError` of a dict lookup of the missing key `key`.
[[noreturn]] void key_error(const std::string& key) {
  throw SimError("KeyError", pyrepr::string(key));
}

// Returns the mask of `width` bytes, for a width from 0 to 8.
std::uint64_t mask(int width) {
  return width >= 8 ? ~std::uint64_t{0} : (std::uint64_t{1} << (8 * width)) - 1;
}

// Returns `u`, `width` bytes wide, as a signed value (Python's
// `_to_signed`): `u - 2^(8 * width)` when its top bit is set. `u` may
// be wider than `width`, as an operand of another width is, and then
// the result is what Python's unbounded arithmetic gives.
Int to_signed(std::uint64_t u, int width) {
  if (width >= 8) {
    return Int(static_cast<std::int64_t>(u));
  }
  const std::uint64_t half = std::uint64_t{1} << (8 * width - 1);
  return u >= half ? Int(u) - Int(2 * half) : Int(u);
}

// Returns `v` modulo 2^64, as the bytes of a Python int's two's
// complement give it.
std::uint64_t low64(Int v) { return static_cast<std::uint64_t>(v); }

// Returns `v` as an address, which a Python int is without a bound.
std::int64_t address(Int v) {
  if (v < Int(INT64_MIN) || v > Int(INT64_MAX)) {
    throw InternalError("the C++ TAC interpreter holds an address in 64 bits, and " +
                        asdl::int_to_string(v) + " is wider");
  }
  return static_cast<std::int64_t>(v);
}

const c99::Type_data_type& strip_qualifiers(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  for (;;) {
    if (const auto* c = asdl::as<c99::Const>(*p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = asdl::as<c99::Volatile>(*p)) {
      p = &*v->referenced_type;
    } else {
      return *p;
    }
  }
}

// Returns the Python class name of the c99 type `t`.
const char* c99_type_name(const c99::Type_data_type& t) {
  using K = c99::Kind_data_type;
  switch (c99::kind(t)) {
    case K::Int: return "Int";
    case K::Long: return "Long";
    case K::LongLong: return "LongLong";
    case K::UInt: return "UInt";
    case K::ULong: return "ULong";
    case K::ULongLong: return "ULongLong";
    case K::Float: return "Float";
    case K::Double: return "Double";
    case K::Char: return "Char";
    case K::SChar: return "SChar";
    case K::UChar: return "UChar";
    case K::Short: return "Short";
    case K::UShort: return "UShort";
    case K::Bool: return "Bool";
    case K::NullptrT: return "NullptrT";
    case K::BitInt: return "BitInt";
    case K::Void: return "Void";
    case K::FunType: return "FunType";
    case K::Pointer: return "Pointer";
    case K::Array: return "Array";
    case K::Structure: return "Structure";
    case K::Union: return "Union";
    case K::Const: return "Const";
    case K::Volatile: return "Volatile";
  }
  return "?";
}

const char* unary_name(const ir::Type_unary_operator& op) {
  using K = ir::Kind_unary_operator;
  switch (ir::kind(op)) {
    case K::Complement: return "Complement";
    case K::Negate: return "Negate";
    case K::LogicalNot: return "LogicalNot";
    case K::BinToBcd: return "BinToBcd";
  }
  return "?";
}

const char* binary_name(const ir::Type_binary_operator& op) {
  using K = ir::Kind_binary_operator;
  switch (ir::kind(op)) {
    case K::Add: return "Add";
    case K::Subtract: return "Subtract";
    case K::Multiply: return "Multiply";
    case K::Divide: return "Divide";
    case K::Modulo: return "Modulo";
    case K::BitwiseAnd: return "BitwiseAnd";
    case K::BitwiseOr: return "BitwiseOr";
    case K::BitwiseXor: return "BitwiseXor";
    case K::LeftShift: return "LeftShift";
    case K::RightShift: return "RightShift";
    case K::Equal: return "Equal";
    case K::NotEqual: return "NotEqual";
    case K::LessThan: return "LessThan";
    case K::GreaterThan: return "GreaterThan";
    case K::LessOrEqual: return "LessOrEqual";
    case K::GreaterOrEqual: return "GreaterOrEqual";
    case K::BcdAdd: return "BcdAdd";
    case K::BcdSubtract: return "BcdSubtract";
  }
  return "?";
}

// Returns the class name and the field of a TAC constant, for the
// `repr` of one and for a message about its missing `value`.
std::pair<const char*, const char*> const_name(const ir::Type_const& c) {
  using K = ir::Kind_const;
  switch (ir::kind(c)) {
    case K::ConstChar: return {"ConstChar", "value"};
    case K::ConstUChar: return {"ConstUChar", "value"};
    case K::ConstInt: return {"ConstInt", "value"};
    case K::ConstLong: return {"ConstLong", "value"};
    case K::ConstLongLong: return {"ConstLongLong", "value"};
    case K::ConstUInt: return {"ConstUInt", "value"};
    case K::ConstULong: return {"ConstULong", "value"};
    case K::ConstULongLong: return {"ConstULongLong", "value"};
    case K::ConstFloat: return {"ConstFloat", "bits"};
    case K::ConstDouble: return {"ConstDouble", "bits"};
    case K::ConstInt24: return {"ConstInt24", "value"};
    case K::ConstUInt24: return {"ConstUInt24", "value"};
    case K::ConstInt40: return {"ConstInt40", "value"};
    case K::ConstUInt40: return {"ConstUInt40", "value"};
    case K::ConstInt48: return {"ConstInt48", "value"};
    case K::ConstUInt48: return {"ConstUInt48", "value"};
    case K::ConstInt56: return {"ConstInt56", "value"};
    case K::ConstUInt56: return {"ConstUInt56", "value"};
  }
  return {"?", "value"};
}

// Returns the integer a constant holds, its `value` or its `bits`.
Int const_payload(const ir::Type_const& c) {
  using K = ir::Kind_const;
  switch (ir::kind(c)) {
    case K::ConstChar: return std::get<ir::ConstChar>(c).value;
    case K::ConstUChar: return std::get<ir::ConstUChar>(c).value;
    case K::ConstInt: return std::get<ir::ConstInt>(c).value;
    case K::ConstLong: return std::get<ir::ConstLong>(c).value;
    case K::ConstLongLong: return std::get<ir::ConstLongLong>(c).value;
    case K::ConstUInt: return std::get<ir::ConstUInt>(c).value;
    case K::ConstULong: return std::get<ir::ConstULong>(c).value;
    case K::ConstULongLong: return std::get<ir::ConstULongLong>(c).value;
    case K::ConstFloat: return std::get<ir::ConstFloat>(c).bits;
    case K::ConstDouble: return std::get<ir::ConstDouble>(c).bits;
    case K::ConstInt24: return std::get<ir::ConstInt24>(c).value;
    case K::ConstUInt24: return std::get<ir::ConstUInt24>(c).value;
    case K::ConstInt40: return std::get<ir::ConstInt40>(c).value;
    case K::ConstUInt40: return std::get<ir::ConstUInt40>(c).value;
    case K::ConstInt48: return std::get<ir::ConstInt48>(c).value;
    case K::ConstUInt48: return std::get<ir::ConstUInt48>(c).value;
    case K::ConstInt56: return std::get<ir::ConstInt56>(c).value;
    case K::ConstUInt56: return std::get<ir::ConstUInt56>(c).value;
  }
  return 0;
}

// Returns the integer a scalar static initializer holds, its `value` or
// its `bits`.
Int init_payload(const ir::Type_static_init& init) {
  using K = ir::Kind_static_init;
  switch (ir::kind(init)) {
    case K::CharInit: return std::get<ir::CharInit>(init).value;
    case K::UCharInit: return std::get<ir::UCharInit>(init).value;
    case K::IntInit: return std::get<ir::IntInit>(init).value;
    case K::UIntInit: return std::get<ir::UIntInit>(init).value;
    case K::LongInit: return std::get<ir::LongInit>(init).value;
    case K::ULongInit: return std::get<ir::ULongInit>(init).value;
    case K::LongLongInit: return std::get<ir::LongLongInit>(init).value;
    case K::ULongLongInit: return std::get<ir::ULongLongInit>(init).value;
    case K::FloatInit: return std::get<ir::FloatInit>(init).bits;
    case K::DoubleInit: return std::get<ir::DoubleInit>(init).bits;
    default: return 0;
  }
}

const char* val_name(const ir::Type_val& v) {
  return asdl::isa<ir::Var>(v) ? "Var" : "Constant";
}

// Returns the `TypeInfo` of the scalar type `t` (Python's `_type_info`).
// `const` and `volatile` don't change a value's representation, and a
// bit-precise type is its whole bytes (src/bit_precise.h).
TypeInfo type_info(const c99::Type_data_type& type) {
  const c99::Type_data_type& t = strip_qualifiers(type);
  using K = c99::Kind_data_type;
  switch (c99::kind(t)) {
    case K::Char: return {false, 1, false};
    case K::SChar: return {true, 1, false};
    case K::UChar: return {false, 1, false};
    case K::Int: return {true, 2, false};
    case K::UInt: return {false, 2, false};
    case K::Pointer: return {false, 2, false};
    case K::Long: return {true, 4, false};
    case K::ULong: return {false, 4, false};
    case K::Float: return {false, 4, true};
    case K::LongLong: return {true, 8, false};
    case K::ULongLong: return {false, 8, false};
    case K::Double: return {false, 8, true};
    case K::BitInt: {
      const auto& b = std::get<c99::BitInt>(t);
      const Int bytes = bit_precise::storage_bytes(b.width);
      if (bytes > 8) {
        throw InternalError("the C++ TAC interpreter holds a value in at most 8 bytes, and _BitInt(" +
                            asdl::int_to_string(b.width) + ") is wider");
      }
      return {b.is_signed, static_cast<int>(bytes), false};
    }
    default:
      fail(std::string("unsupported type in simulator: ") + c99_type_name(t));
  }
}

// Returns whether `t` is a struct or a union. Python's `_is_aggregate`
// doesn't look through `const` or `volatile`, so neither does this.
bool is_aggregate(const c99::Type_data_type& t) {
  return asdl::isa<c99::Structure, c99::Union>(t);
}

// Returns the `TypeInfo` and the unsigned value of a TAC constant (a
// floating constant's value is its IEEE 754 bit pattern).
std::pair<TypeInfo, std::uint64_t> const_info(const ir::Type_const& c) {
  using K = ir::Kind_const;
  const Int v = const_payload(c);
  switch (ir::kind(c)) {
    case K::ConstChar: return {{true, 1, false}, low64(v) & 0xFF};
    case K::ConstUChar: return {{false, 1, false}, low64(v) & 0xFF};
    case K::ConstInt: return {{true, 2, false}, low64(v) & 0xFFFF};
    case K::ConstUInt: return {{false, 2, false}, low64(v) & 0xFFFF};
    case K::ConstLong: return {{true, 4, false}, low64(v) & 0xFFFFFFFF};
    case K::ConstULong: return {{false, 4, false}, low64(v) & 0xFFFFFFFF};
    case K::ConstLongLong: return {{true, 8, false}, low64(v)};
    case K::ConstULongLong: return {{false, 8, false}, low64(v)};
    case K::ConstFloat: return {{false, 4, true}, low64(v) & 0xFFFFFFFF};
    case K::ConstDouble: return {{false, 8, true}, low64(v)};
    default: {
      const auto layout = bit_precise::odd_const_layout(c);
      return {{layout->is_signed, layout->bytes, false}, low64(v) & mask(layout->bytes)};
    }
  }
}

// Returns the bytes a static initializer item lays down.
std::int64_t init_size(const ir::Type_static_init& init) {
  using K = ir::Kind_static_init;
  switch (ir::kind(init)) {
    case K::CharInit:
    case K::UCharInit:
    case K::AddressByteInit: return 1;
    case K::IntInit:
    case K::UIntInit:
    case K::AddressInit: return 2;
    case K::LongInit:
    case K::ULongInit:
    case K::FloatInit: return 4;
    case K::LongLongInit:
    case K::ULongLongInit:
    case K::DoubleInit: return 8;
    case K::StringInit: return address(std::get<ir::StringInit>(init).bytes);
    case K::ZeroInit: return address(std::get<ir::ZeroInit>(init).bytes);
  }
  return 0;
}

// Returns Python's `repr` of a character in a `UnicodeEncodeError`.
std::string char_escape(std::uint32_t cp) {
  char buf[16];
  if (cp <= 0xFFFF) {
    std::snprintf(buf, sizeof buf, "\\u%04x", static_cast<unsigned>(cp));
  } else {
    std::snprintf(buf, sizeof buf, "\\U%08x", static_cast<unsigned>(cp));
  }
  return buf;
}

// Returns the bytes of `s.encode("latin-1")`, or raises Python's
// `UnicodeEncodeError` for a code point above 255, which names the
// run of such code points that starts there.
std::vector<std::uint8_t> latin1(const std::string& s) {
  const std::vector<std::uint32_t> cps = pyrepr::code_points(s);
  std::vector<std::uint8_t> out;
  out.reserve(cps.size());
  for (std::size_t i = 0; i < cps.size(); ++i) {
    if (cps[i] > 0xFF) {
      std::size_t end = i + 1;
      while (end < cps.size() && cps[end] > 0xFF) ++end;
      std::string what = end - i == 1
                             ? "character '" + char_escape(cps[i]) + "' in position " +
                                   std::to_string(i)
                             : "characters in position " + std::to_string(i) + "-" +
                                   std::to_string(end - 1);
      throw SimError("UnicodeEncodeError",
                     "'latin-1' codec can't encode " + what + ": ordinal not in range(256)");
    }
    out.push_back(static_cast<std::uint8_t>(cps[i]));
  }
  return out;
}

bool is_truthy(std::uint64_t value, const TypeInfo& ti) {
  if (ti.is_fp) {
    return ti.width == 8 ? fp_arith::double_is_truthy(Int(value))
                         : fp_arith::single_is_truthy(Int(value));
  }
  return value != 0;
}

std::uint64_t eval_unary(const ir::Type_unary_operator& op, std::uint64_t v, const TypeInfo& ti) {
  using K = ir::Kind_unary_operator;
  const std::uint64_t m = mask(ti.width);
  if (ti.is_fp) {
    switch (ir::kind(op)) {
      case K::Negate:
        return low64(ti.width == 8 ? fp_arith::double_negate(Int(v))
                                   : fp_arith::single_negate(Int(v)));
      case K::LogicalNot:
        return is_truthy(v, ti) ? 0 : 1;
      case K::Complement:
        fail("Complement is not defined for FP types");
      default:
        fail(std::string("unsupported FP unary op: ") + unary_name(op));
    }
  }
  switch (ir::kind(op)) {
    case K::Negate: return low64(-to_signed(v, ti.width)) & m;
    case K::Complement: return ~v & m;
    case K::LogicalNot: return v == 0 ? 1 : 0;
    case K::BinToBcd: return low64(builtin_functions::bin_to_bcd(Int(v)));
  }
  fail(std::string("unsupported unary op: ") + unary_name(op));
}

// The floating arithmetic and comparisons of `_eval_binary_fp`, at the
// precision `w` chooses (4 for single, 8 for double). A comparison gives
// 0 or 1. `==` is true when the operands are equal and false when they
// are unordered, the ordering relations are false when they are
// unordered, and `!=` is the negation of `==`, so it is true then (C99
// §6.5.8p5).
std::uint64_t eval_binary_fp(const ir::Type_binary_operator& op, std::uint64_t a, std::uint64_t b,
                             int w) {
  using K = ir::Kind_binary_operator;
  using O = fp_arith::Order;
  const Int x(a), y(b);
  switch (ir::kind(op)) {
    case K::Equal:
    case K::NotEqual:
    case K::LessThan:
    case K::GreaterThan:
    case K::LessOrEqual:
    case K::GreaterOrEqual: {
      const O tag = w == 8 ? fp_arith::double_compare(x, y) : fp_arith::single_compare(x, y);
      switch (ir::kind(op)) {
        case K::Equal: return tag == O::kEq ? 1 : 0;
        case K::NotEqual: return tag == O::kEq ? 0 : 1;
        case K::LessThan: return tag == O::kLt ? 1 : 0;
        case K::GreaterThan: return tag == O::kGt ? 1 : 0;
        case K::LessOrEqual: return tag == O::kLt || tag == O::kEq ? 1 : 0;
        default: return tag == O::kGt || tag == O::kEq ? 1 : 0;
      }
    }
    case K::Add: return low64(w == 8 ? fp_arith::double_add(x, y) : fp_arith::single_add(x, y));
    case K::Subtract:
      return low64(w == 8 ? fp_arith::double_sub(x, y) : fp_arith::single_sub(x, y));
    case K::Multiply:
      return low64(w == 8 ? fp_arith::double_mul(x, y) : fp_arith::single_mul(x, y));
    case K::Divide:
      return low64(w == 8 ? fp_arith::double_div(x, y) : fp_arith::single_div(x, y));
    default:
      fail(std::string("unsupported FP binary op: ") + binary_name(op));
  }
}

// Returns Python's `a // b` of two magnitudes.
Int abs_int(Int v) { return v < 0 ? -v : v; }

// Evaluates a binary operation on `a` and `b` with the signedness,
// width, and floating flag `ti` of the left operand. c99_to_tac turns a
// conversion between signednesses of one width into a `Copy` to a
// temporary of the new type, so the usual arithmetic conversions leave
// both operands of `/`, `%`, and the ordering comparisons with one
// signedness. The right operand of `>>` keeps its own promoted type
// (C99 §6.5.7p3), which must not choose the kind of shift.
std::uint64_t eval_binary(const ir::Type_binary_operator& op, std::uint64_t a, std::uint64_t b,
                          const TypeInfo& ti) {
  if (ti.is_fp) {
    return eval_binary_fp(op, a, b, ti.width);
  }
  using K = ir::Kind_binary_operator;
  const int w = ti.width;
  const std::uint64_t m = mask(w);
  switch (ir::kind(op)) {
    case K::Equal: return a == b ? 1 : 0;
    case K::NotEqual: return a != b ? 1 : 0;
    case K::LessThan:
    case K::GreaterThan:
    case K::LessOrEqual:
    case K::GreaterOrEqual: {
      const Int x = ti.is_signed ? to_signed(a, w) : Int(a);
      const Int y = ti.is_signed ? to_signed(b, w) : Int(b);
      bool r = false;
      switch (ir::kind(op)) {
        case K::LessThan: r = x < y; break;
        case K::GreaterThan: r = x > y; break;
        case K::LessOrEqual: r = x <= y; break;
        default: r = x >= y; break;
      }
      return r ? 1 : 0;
    }
    // Packed BCD, byte by byte as the 6502's decimal mode computes it.
    case K::BcdAdd: return low64(builtin_functions::bcd_add(Int(a), Int(b), w));
    case K::BcdSubtract: return low64(builtin_functions::bcd_sub(Int(a), Int(b), w));
    case K::Add: return (a + b) & m;
    case K::Subtract: return (a - b) & m;
    case K::Multiply: return (a * b) & m;
    case K::BitwiseAnd: return a & b;
    case K::BitwiseOr: return a | b;
    case K::BitwiseXor: return a ^ b;
    case K::LeftShift: {
      const std::uint64_t count = b & 0xFF;
      return count >= 64 ? 0 : (a << count) & m;
    }
    case K::RightShift: {
      const std::uint64_t count = b & 0xFF;
      if (ti.is_signed) {
        // `a` is `w` bytes, so its signed value fits in 64 bits.
        const auto s = static_cast<std::int64_t>(to_signed(a, w));
        return static_cast<std::uint64_t>(s >> std::min<std::uint64_t>(count, 63)) & m;
      }
      return count >= 64 ? 0 : a >> count;
    }
    case K::Divide: {
      if (b == 0) fail("division by zero");
      if (ti.is_signed) {
        const Int x = to_signed(a, w), y = to_signed(b, w);
        Int q = abs_int(x) / abs_int(y);
        if ((x < 0) != (y < 0)) q = -q;
        return low64(q) & m;
      }
      return (a / b) & m;
    }
    case K::Modulo: {
      if (b == 0) fail("modulo by zero");
      if (ti.is_signed) {
        const Int x = to_signed(a, w), y = to_signed(b, w);
        Int r = abs_int(x) % abs_int(y);
        if (x < 0) r = -r;
        return low64(r) & m;
      }
      return (a % b) & m;
    }
  }
  fail(std::string("unsupported binary op: ") + binary_name(op));
}

std::string hex_address(std::uint64_t v) {
  char buf[24];
  std::snprintf(buf, sizeof buf, "0x%llx", static_cast<unsigned long long>(v));
  return buf;
}

}  // namespace

// --- Memory ---

std::int64_t Memory::allocate(std::int64_t size) {
  const std::int64_t addr = next_addr_;
  next_addr_ += size;
  return addr;
}

std::uint8_t Memory::byte(std::int64_t addr) const {
  if (addr >= 0 && addr < kLowLimit) {
    return static_cast<std::size_t>(addr) < low_.size() ? low_[static_cast<std::size_t>(addr)] : 0;
  }
  const auto it = high_.find(addr);
  return it == high_.end() ? 0 : it->second;
}

void Memory::set_byte(std::int64_t addr, std::uint8_t b) {
  if (addr >= 0 && addr < kLowLimit) {
    const auto i = static_cast<std::size_t>(addr);
    if (i >= low_.size()) {
      low_.resize(std::min<std::size_t>(std::max<std::size_t>(i + 1, 2 * low_.size()),
                                        static_cast<std::size_t>(kLowLimit)));
    }
    low_[i] = b;
    return;
  }
  high_[addr] = b;
}

void Memory::store(std::int64_t addr, std::uint64_t value, std::int64_t width) {
  for (std::int64_t i = 0; i < width; ++i) {
    set_byte(addr + i, i < 8 ? static_cast<std::uint8_t>(value >> (8 * i)) : 0);
  }
}

void Memory::store_int(std::int64_t addr, Int value, std::int64_t width) {
  for (std::int64_t i = 0; i < width; ++i) {
    std::uint8_t b = 0;
    if (i < 16) {
      b = static_cast<std::uint8_t>(static_cast<std::uint64_t>(value >> static_cast<int>(8 * i)) & 0xFF);
    } else {
      b = value < 0 ? 0xFF : 0;
    }
    set_byte(addr + i, b);
  }
}

std::uint64_t Memory::load(std::int64_t addr, std::int64_t width) const {
  std::uint64_t v = 0;
  for (std::int64_t i = 0; i < width && i < 8; ++i) {
    v |= static_cast<std::uint64_t>(byte(addr + i)) << (8 * i);
  }
  return v;
}

Int Memory::load_int(std::int64_t addr, std::int64_t width) const {
  if (width > 15) {
    throw InternalError("the C++ TAC interpreter reads at most 15 bytes as one value, and " +
                        std::to_string(width) + " were asked for");
  }
  Int v = 0;
  for (std::int64_t i = 0; i < width; ++i) {
    v |= Int(byte(addr + i)) << static_cast<int>(8 * i);
  }
  return v;
}

// --- Simulator ---

struct Simulator::FunctionInfo {
  const ir::Function* function = nullptr;
  // The index of each label, the last one when a name repeats, as
  // Python's dict comprehension keeps it.
  std::unordered_map<std::string, std::size_t> labels;
  // The parameters and locals that some `GetAddress` takes the address
  // of.
  std::unordered_map<std::string, bool> address_taken;
};

struct Simulator::Frame {
  const FunctionInfo* info = nullptr;
  std::unordered_map<std::string, std::uint64_t> env;
  std::unordered_map<std::string, std::int64_t> local_addr;
  std::size_t pc = 0;
  // The labels of the block being run and of the one before it, for
  // choosing a Phi's argument, or null for none.
  const std::string* block_label = nullptr;
  const std::string* prev_label = nullptr;
  // While a callee runs, the destination of the call's result.
  const std::optional<ir::Type_val>* call_dst = nullptr;
};

// What the interpreter reads from a symbol's row, computed once per
// name: the row (null when there is none), whether its type is an
// aggregate, its `TypeInfo` or the message `_type_info` raises for it,
// and whether it has static storage.
struct Simulator::SymInfo {
  const passes::Symbol* sym = nullptr;
  bool aggregate = false;
  std::optional<TypeInfo> ti;
  std::string ti_error;
  bool is_internal_error = false;
  bool is_static = false;

  const TypeInfo& info() const {
    if (!ti) {
      if (is_internal_error) throw InternalError(ti_error);
      fail(ti_error);
    }
    return *ti;
  }
};

Simulator::Simulator(const ir::Program& program, const passes::SymbolTable& symbols,
                     const passes::TypeTable* types)
    : symbols_(symbols), types_(types) {
  std::vector<std::string> function_order;
  for (const auto& t : program.top_level) {
    const auto* fn = asdl::as<ir::Function>(t);
    if (fn == nullptr) continue;
    auto info = std::make_unique<FunctionInfo>();
    info->function = fn;
    for (std::size_t i = 0; i < fn->instructions.size(); ++i) {
      const auto& ins = fn->instructions[i];
      if (const auto* label = asdl::as<ir::Label>(ins)) {
        info->labels[label->name] = i;
      } else if (const auto* ga = asdl::as<ir::GetAddress>(ins)) {
        if (const auto* var = asdl::as<ir::Var>(ga->operand)) {
          info->address_taken[var->name] = true;
        }
      }
    }
    if (functions_.find(fn->name) == functions_.end()) {
      function_order.push_back(fn->name);
    }
    functions_[fn->name] = std::move(info);
  }
  for (const auto& t : program.top_level) {
    if (const auto* sv = asdl::as<ir::StaticVariable>(t)) {
      std::int64_t size = 0;
      for (const auto& item : sv->init) size += init_size(item);
      static_addr_[sv->name] = memory_.allocate(size);
    }
  }
  for (const auto& name : function_order) {
    const std::int64_t addr = memory_.allocate(2);
    function_addr_[name] = addr;
    addr_to_function_[addr] = name;
  }
  for (const auto& t : program.top_level) {
    if (const auto* sv = asdl::as<ir::StaticVariable>(t)) {
      lay_down_static(*sv);
    }
  }
}

Simulator::~Simulator() = default;

void Simulator::lay_down_static(const ir::StaticVariable& sv) {
  std::int64_t addr = static_addr_.at(sv.name);
  for (const auto& item : sv.init) {
    using K = ir::Kind_static_init;
    switch (ir::kind(item)) {
      case K::CharInit:
      case K::UCharInit:
      case K::IntInit:
      case K::UIntInit:
      case K::LongInit:
      case K::ULongInit:
      case K::LongLongInit:
      case K::ULongLongInit:
      case K::FloatInit:
      case K::DoubleInit: {
        const Int v = init_payload(item);
        const std::int64_t n = init_size(item);
        memory_.store_int(addr, v, n);
        addr += n;
        break;
      }
      case K::AddressInit:
      case K::AddressByteInit: {
        const bool byte = ir::kind(item) == K::AddressByteInit;
        const std::string& n = byte ? std::get<ir::AddressByteInit>(item).name
                                    : std::get<ir::AddressInit>(item).name;
        const Int off = byte ? std::get<ir::AddressByteInit>(item).offset
                             : std::get<ir::AddressInit>(item).offset;
        std::optional<std::int64_t> target;
        if (const auto s = static_address(n)) {
          target = *s;
        } else if (auto f = function_addr_.find(n); f != function_addr_.end()) {
          target = f->second;
        }
        if (!target) {
          fail(std::string(byte ? "AddressByteInit" : "AddressInit") + " references unknown " +
               pyrepr::string(n));
        }
        if (!byte) {
          memory_.store_int(addr, Int(*target) + off, 2);
          addr += 2;
        } else {
          const std::uint64_t value = low64(Int(*target) + off) & 0xFFFF;
          memory_.store(addr, std::get<ir::AddressByteInit>(item).high ? value >> 8 : value & 0xFF,
                        1);
          addr += 1;
        }
        break;
      }
      case K::StringInit: {
        const auto& s = std::get<ir::StringInit>(item);
        const std::vector<std::uint8_t> raw = latin1(s.str);
        const std::int64_t n = address(s.bytes);
        for (std::size_t i = 0; i < raw.size(); ++i) {
          memory_.set_byte(addr + static_cast<std::int64_t>(i), raw[i]);
        }
        // The rest is zero.
        for (auto i = static_cast<std::int64_t>(raw.size()); i < n; ++i) {
          memory_.set_byte(addr + i, 0);
        }
        addr += n;
        break;
      }
      case K::ZeroInit: {
        const std::int64_t n = address(std::get<ir::ZeroInit>(item).bytes);
        for (std::int64_t i = 0; i < n; ++i) memory_.set_byte(addr + i, 0);
        addr += n;
        break;
      }
    }
  }
}

std::optional<std::int64_t> Simulator::address_of_name(const std::string& name) const {
  if (auto it = static_addr_.find(name); it != static_addr_.end()) return it->second;
  if (auto it = function_addr_.find(name); it != function_addr_.end()) return it->second;
  return std::nullopt;
}

const Simulator::SymInfo& Simulator::sym_info(const std::string& name) const {
  if (auto it = sym_cache_.find(name); it != sym_cache_.end()) return *it->second;
  SymInfo info;
  info.sym = symbols_.find(name);
  if (info.sym != nullptr) {
    info.aggregate = is_aggregate(info.sym->type);
    info.is_static = asdl::isa<passes::StaticAttr>(info.sym->attrs);
    try {
      info.ti = type_info(info.sym->type);
    } catch (const SimError& e) {
      info.ti_error = e.what();
    } catch (const InternalError& e) {
      info.ti_error = e.what();
      info.is_internal_error = true;
    }
  }
  return *sym_cache_.emplace(name, std::make_unique<SymInfo>(std::move(info))).first->second;
}

const passes::Symbol& Simulator::symbol(const std::string& name) const {
  const SymInfo& info = sym_info(name);
  if (info.sym == nullptr) key_error(name);
  return *info.sym;
}

std::int64_t Simulator::size_of(const c99::Type_data_type& type) const {
  const c99::Type_data_type& t = strip_qualifiers(type);
  if (const auto* a = asdl::as<c99::Array>(t)) {
    return size_of(*a->element_type) * address(a->size);
  }
  if (is_aggregate(t)) {
    const std::string& tag = asdl::isa<c99::Structure>(t) ? std::get<c99::Structure>(t).tag
                                                          : std::get<c99::Union>(t).tag;
    if (types_ == nullptr) {
      fail("cannot size aggregate type without TypeTable: " + asdl::py_repr(t));
    }
    const passes::StructLayout* layout = types_->find(tag);
    if (layout == nullptr || !layout->complete) {
      fail("incomplete aggregate type: " + pyrepr::string(tag));
    }
    return address(layout->size);
  }
  return type_info(t).width;
}

std::optional<Int> Simulator::call(const std::string& name, const std::vector<Int>& args) {
  const std::size_t base = frames_.size();
  try {
    push_frame(name, args);
    for (;;) {
      Frame& frame = *frames_.back();
      const ir::Function& fn = *frame.info->function;
      if (frame.pc >= fn.instructions.size()) {
        fail(fn.name + ": ran past end without Ret");
      }
      const ir::Type_instruction& ins = fn.instructions[frame.pc];
      frame.pc += 1;
      std::optional<Return> ret = step(frame, ins);
      if (!ret) continue;
      std::optional<Int> rv = finish_call(fn.name, *ret);
      frames_.pop_back();
      if (frames_.size() == base) return rv;
      Frame& caller = *frames_.back();
      const std::optional<ir::Type_val>* dst = caller.call_dst;
      caller.call_dst = nullptr;
      if (dst->has_value()) capture_return(caller, **dst, rv);
    }
  } catch (...) {
    frames_.resize(base);
    throw;
  }
}

void Simulator::push_frame(const std::string& name, const std::vector<Int>& args) {
  const auto it = functions_.find(name);
  if (it == functions_.end()) {
    fail("unknown function: " + pyrepr::string(name));
  }
  const FunctionInfo& info = *it->second;
  const ir::Function& fn = *info.function;
  if (args.size() != fn.params.size()) {
    fail(name + ": expected " + std::to_string(fn.params.size()) + " args, got " +
         std::to_string(args.size()));
  }
  if (frames_.size() >= kMaxCallDepth) {
    throw SimError("RecursionError", "maximum recursion depth exceeded");
  }
  Frame frame;
  frame.info = &info;
  // An address-taken parameter gets memory now, with its argument's
  // bytes, and an address-taken local gets it at its first `GetAddress`.
  // A struct or union parameter always lives in memory: its argument is
  // the address of the caller's bytes, which are copied into a new
  // region for the callee.
  for (std::size_t i = 0; i < fn.params.size(); ++i) {
    const std::string& pname = fn.params[i];
    const Int raw = args[i];
    const SymInfo& sym = sym_info(pname);
    if (sym.sym == nullptr) key_error(pname);
    if (sym.aggregate) {
      const std::int64_t size = size_of(sym.sym->type);
      const std::int64_t addr = memory_.allocate(size);
      frame.local_addr[pname] = addr;
      memcpy(addr, address(raw), size);
      continue;
    }
    const int w = sym.info().width;
    const std::uint64_t masked = low64(raw) & mask(w);
    if (info.address_taken.count(pname) != 0) {
      const std::int64_t addr = memory_.allocate(w);
      frame.local_addr[pname] = addr;
      memory_.store(addr, masked, w);
    } else {
      frame.env[pname] = masked;
    }
  }
  frames_.push_back(std::make_unique<Frame>(std::move(frame)));
}

std::optional<Int> Simulator::finish_call(const std::string& name, const Return& ret) {
  if (!ret.value) return std::nullopt;
  const passes::Symbol& sym = symbol(name);
  const auto* fun = asdl::as<c99::FunType>(sym.type);
  if (fun == nullptr) {
    throw SimError("AttributeError", std::string("'") + c99_type_name(sym.type) +
                                         "' object has no attribute 'ret'");
  }
  const c99::Type_data_type& ret_type = *fun->ret;
  const Int value = *ret.value;
  if (is_aggregate(ret_type)) {
    // A `return_bool_in_carry` struct: the flag, its last byte, comes
    // back in the carry, so the caller sees 0 or 1.
    const int top = static_cast<int>(8 * (ret.width - 1));
    const Int flag = (value >> top) & Int(0xFF);
    return (value & ((Int(1) << top) - 1)) | (Int(flag != 0 ? 1 : 0) << top);
  }
  const TypeInfo ti = type_info(ret_type);
  if (ti.is_fp) {
    // The IEEE 754 bit pattern.
    return value;
  }
  return ti.is_signed ? to_signed(low64(value), static_cast<int>(ret.width)) : value;
}

asdl::Int Simulator::read_static(const std::string& name) {
  const SymInfo& info = sym_info(name);
  if (info.sym == nullptr || !info.is_static) {
    fail("not a static variable: " + pyrepr::string(name));
  }
  const std::int64_t addr = static_slot(name);
  const TypeInfo& ti = info.info();
  const std::uint64_t raw = memory_.load(addr, ti.width);
  if (ti.is_fp) return Int(raw);
  return ti.is_signed ? to_signed(raw, ti.width) : Int(raw);
}

std::int64_t Simulator::static_slot(const std::string& name) {
  const std::optional<std::int64_t> addr = static_address(name);
  if (!addr) key_error(name);
  return *addr;
}

std::optional<std::int64_t> Simulator::static_address(const std::string& name) {
  if (auto it = static_addr_.find(name); it != static_addr_.end()) return it->second;
  // An object that another unit defines, such as `extern struct s x;`,
  // has no StaticVariable, and gets zeroed memory of its own the first
  // time the program uses it, which every frame then shares. An object
  // at a fixed address (`[[c6502::address(addr)]]`) is an I/O register
  // or the memory of a program the interpreter doesn't run, so it gets
  // none, and a use of it raises as before, where a loop that polls a
  // zeroed register would never end.
  const SymInfo& info = sym_info(name);
  if (info.sym == nullptr || !info.is_static ||
      std::get<passes::StaticAttr>(info.sym->attrs).address) {
    return std::nullopt;
  }
  const std::int64_t addr = memory_.allocate(size_of(info.sym->type));
  static_addr_[name] = addr;
  return addr;
}

void Simulator::jump(Frame& frame, const std::string& target) {
  const auto it = frame.info->labels.find(target);
  if (it == frame.info->labels.end()) key_error(target);
  frame.pc = it->second;
}

void Simulator::memcpy(std::int64_t dst, std::int64_t src, std::int64_t size) {
  // Byte by byte from the front, as Python's loop copies.
  for (std::int64_t i = 0; i < size; ++i) {
    memory_.set_byte(dst + i, memory_.byte(src + i));
  }
}

bool Simulator::is_aggregate_var(const ir::Type_val& v) const {
  const auto* var = asdl::as<ir::Var>(v);
  if (var == nullptr) return false;
  const SymInfo& info = sym_info(var->name);
  if (info.sym == nullptr) key_error(var->name);
  return info.aggregate;
}

bool Simulator::is_local_aggregate(const std::string& name) const {
  const SymInfo& info = sym_info(name);
  if (info.sym == nullptr || info.is_static) return false;
  return asdl::isa<c99::Structure, c99::Union, c99::Array>(strip_qualifiers(info.sym->type));
}

bool Simulator::is_local_access(const std::string& name, const ir::Type_val& index) const {
  if (sym_info(name).sym == nullptr) return false;
  const auto* c = asdl::as<ir::Constant>(index);
  if (c == nullptr) return false;
  if (asdl::isa<ir::ConstFloat, ir::ConstDouble>(c->const_)) {
    throw SimError("AttributeError", std::string("'") + const_name(c->const_).first +
                                         "' object has no attribute 'value'");
  }
  return const_payload(c->const_) == 0;
}

std::int64_t Simulator::address_of(Frame& frame, const ir::Type_val& v) {
  const auto* var = asdl::as<ir::Var>(v);
  if (var == nullptr) fail("GetAddress operand must be a Var, got " + asdl::py_repr(v));
  return address_of_var(frame, var->name);
}

std::int64_t Simulator::address_of_var(Frame& frame, const std::string& name) {
  // A function's address is in a map of its own, from which a call
  // through a pointer finds the function again.
  if (auto it = function_addr_.find(name); it != function_addr_.end()) return it->second;
  if (const auto s = static_address(name)) return *s;
  if (auto it = frame.local_addr.find(name); it != frame.local_addr.end()) return it->second;
  // An address-taken local gets its memory at its first `GetAddress`.
  // If it has a value already, the value moves into the memory.
  const SymInfo& info = sym_info(name);
  if (info.sym == nullptr) fail("unknown var: " + pyrepr::string(name));
  const std::int64_t size = size_of(info.sym->type);
  const std::int64_t addr = memory_.allocate(size);
  frame.local_addr[name] = addr;
  if (auto it = frame.env.find(name); it != frame.env.end()) {
    const int w = info.info().width;
    memory_.store(addr, it->second, w);
    frame.env.erase(it);
  }
  return addr;
}

Simulator::Read Simulator::read(Frame& frame, const ir::Type_val& v) {
  if (const auto* c = asdl::as<ir::Constant>(v)) {
    const auto [ti, value] = const_info(c->const_);
    return {value, ti};
  }
  const std::string& n = std::get<ir::Var>(v).name;
  const SymInfo& info = sym_info(n);
  if (info.sym == nullptr) key_error(n);
  const TypeInfo& ti = info.info();
  if (info.is_static) {
    return {memory_.load(static_slot(n), ti.width), ti};
  }
  if (auto it = frame.local_addr.find(n); it != frame.local_addr.end()) {
    return {memory_.load(it->second, ti.width), ti};
  }
  const auto it = frame.env.find(n);
  if (it == frame.env.end()) fail("uninitialized var: " + n);
  return {it->second, ti};
}

void Simulator::write(Frame& frame, const ir::Type_val& v, std::uint64_t raw) {
  const auto* var = asdl::as<ir::Var>(v);
  if (var == nullptr) fail(std::string("can only write to Var, got ") + val_name(v));
  write_var(frame, var->name, raw);
}

void Simulator::write_var(Frame& frame, const std::string& name, std::uint64_t raw) {
  const SymInfo& info = sym_info(name);
  if (info.sym == nullptr) key_error(name);
  const int w = info.info().width;
  const std::uint64_t masked = raw & mask(w);
  if (info.is_static) {
    memory_.store(static_slot(name), masked, w);
  } else if (auto it = frame.local_addr.find(name); it != frame.local_addr.end()) {
    memory_.store(it->second, masked, w);
  } else {
    frame.env[name] = masked;
  }
}

int Simulator::dst_width(const ir::Type_val& v) const {
  const auto* var = asdl::as<ir::Var>(v);
  if (var == nullptr) fail(std::string("expected Var as dst, got ") + val_name(v));
  const SymInfo& info = sym_info(var->name);
  if (info.sym == nullptr) key_error(var->name);
  return info.info().width;
}

std::vector<Int> Simulator::call_args(Frame& frame, const std::vector<ir::Type_val>& args) {
  // A struct or union argument passes its address, and the callee
  // copies the bytes from there into its own region.
  std::vector<Int> out;
  out.reserve(args.size());
  for (const auto& a : args) {
    if (is_aggregate_var(a)) {
      out.push_back(Int(address_of(frame, a)));
    } else {
      out.push_back(Int(read(frame, a).value));
    }
  }
  return out;
}

void Simulator::capture_return(Frame& frame, const ir::Type_val& dst,
                               const std::optional<Int>& rv) {
  // Nothing is written for a function that returns no value.
  if (!rv) return;
  if (is_aggregate_var(dst)) {
    const std::int64_t size = size_of(symbol(std::get<ir::Var>(dst).name).type);
    memory_.store_int(address_of(frame, dst), *rv, size);
    return;
  }
  write(frame, dst, low64(*rv) & mask(dst_width(dst)));
}

void Simulator::run_phis(Frame& frame) {
  // `step` has stepped past the first Phi. Each Phi reads its argument
  // for the predecessor before any of them writes its destination.
  const auto& ins = frame.info->function->instructions;
  std::vector<const ir::Phi*> phis{&std::get<ir::Phi>(ins[frame.pc - 1])};
  while (frame.pc < ins.size() && asdl::isa<ir::Phi>(ins[frame.pc])) {
    phis.push_back(&std::get<ir::Phi>(ins[frame.pc]));
    frame.pc += 1;
  }
  std::vector<std::optional<std::uint64_t>> values;
  values.reserve(phis.size());
  for (const ir::Phi* phi : phis) {
    const ir::Type_val* source = nullptr;
    for (const auto& a : phi->args) {
      const auto& arg = std::get<ir::PhiArg>(a);
      if (frame.prev_label != nullptr && arg.pred_label == *frame.prev_label) {
        source = &arg.source;
        break;
      }
    }
    if (source == nullptr) {
      const auto* dst = asdl::as<ir::Var>(phi->dst);
      if (dst == nullptr) {
        throw SimError("AttributeError", "'Constant' object has no attribute 'name'");
      }
      fail("Phi for " + dst->name + " has no argument from " +
           (frame.prev_label == nullptr ? std::string("None") : pyrepr::string(*frame.prev_label)));
    }
    if (is_unset(frame, *source)) {
      // SSA construction gives a Phi the variable's own name for a path
      // on which the variable has no value yet, as for `int c;` before a
      // loop that assigns it. The Phi's destination then has no value
      // either, and only a read of it raises.
      values.emplace_back();
      continue;
    }
    values.emplace_back(read(frame, *source).value);
  }
  for (std::size_t i = 0; i < phis.size(); ++i) {
    if (values[i]) {
      write(frame, phis[i]->dst, *values[i]);
    } else {
      unset(frame, phis[i]->dst);
    }
  }
}

bool Simulator::is_unset(const Frame& frame, const ir::Type_val& v) const {
  // An automatic scalar with no value yet, which `read` raises
  // "uninitialized var" for.
  const auto* var = asdl::as<ir::Var>(v);
  if (var == nullptr) return false;
  const SymInfo& info = sym_info(var->name);
  return info.sym != nullptr && !info.is_static && frame.local_addr.count(var->name) == 0 &&
         frame.env.count(var->name) == 0;
}

void Simulator::unset(Frame& frame, const ir::Type_val& v) {
  // Takes an automatic scalar's value away, so that a read of it raises
  // "uninitialized var". An object in memory keeps its bytes.
  if (const auto* var = asdl::as<ir::Var>(v)) frame.env.erase(var->name);
}

std::optional<Simulator::Return> Simulator::step(Frame& frame, const ir::Type_instruction& ins) {
  using K = ir::Kind_instruction;
  switch (ir::kind(ins)) {
    case K::Label: {
      frame.prev_label = frame.block_label;
      frame.block_label = &std::get<ir::Label>(ins).name;
      return std::nullopt;
    }
    case K::Phi:
      run_phis(frame);
      return std::nullopt;
    case K::JumpIfCmp: {
      const auto& j = std::get<ir::JumpIfCmp>(ins);
      const Read a = read(frame, j.src1);
      const Read b = read(frame, j.src2);
      const TypeInfo ti{a.ti.is_signed && b.ti.is_signed, a.ti.width, a.ti.is_fp || b.ti.is_fp};
      if (eval_binary(j.op, a.value, b.value & mask(a.ti.width), ti) != 0) jump(frame, j.target);
      return std::nullopt;
    }
    case K::JumpIfMasked: {
      const auto& j = std::get<ir::JumpIfMasked>(ins);
      const Read v = read(frame, j.val);
      if (((Int(v.value) & j.mask) != 0) == j.jump_when_nonzero) jump(frame, j.target);
      return std::nullopt;
    }
    case K::Jump:
      jump(frame, std::get<ir::Jump>(ins).target);
      return std::nullopt;
    case K::JumpIfTrue: {
      const auto& j = std::get<ir::JumpIfTrue>(ins);
      const Read c = read(frame, j.condition);
      if (is_truthy(c.value, c.ti)) jump(frame, j.target);
      return std::nullopt;
    }
    case K::JumpIfFalse: {
      const auto& j = std::get<ir::JumpIfFalse>(ins);
      const Read c = read(frame, j.condition);
      if (!is_truthy(c.value, c.ti)) jump(frame, j.target);
      return std::nullopt;
    }
    case K::Copy: {
      const auto& c = std::get<ir::Copy>(ins);
      if (is_aggregate_var(c.dst)) {
        const std::int64_t src = address_of(frame, c.src);
        const std::int64_t dst = address_of(frame, c.dst);
        memcpy(dst, src, size_of(symbol(std::get<ir::Var>(c.dst).name).type));
      } else if (is_unset(frame, c.src)) {
        // SSA destruction's copy of a Phi argument that has no value
        // (`run_phis`).
        unset(frame, c.dst);
      } else {
        write(frame, c.dst, read(frame, c.src).value);
      }
      return std::nullopt;
    }
    case K::Unary: {
      const auto& u = std::get<ir::Unary>(ins);
      const Read v = read(frame, u.src);
      write(frame, u.dst, eval_unary(u.op, v.value, v.ti));
      return std::nullopt;
    }
    case K::Binary: {
      const auto& b = std::get<ir::Binary>(ins);
      const Read x = read(frame, b.src1);
      const Read y = read(frame, b.src2);
      write(frame, b.dst, eval_binary(b.op, x.value, y.value, x.ti));
      return std::nullopt;
    }
    case K::SignExtend: {
      const auto& s = std::get<ir::SignExtend>(ins);
      const Read v = read(frame, s.src);
      const Int vs = to_signed(v.value, v.ti.width);
      write(frame, s.dst, low64(vs) & mask(dst_width(s.dst)));
      return std::nullopt;
    }
    case K::ZeroExtend:
    case K::Truncate: {
      const auto& src = ir::kind(ins) == K::ZeroExtend ? std::get<ir::ZeroExtend>(ins).src
                                                       : std::get<ir::Truncate>(ins).src;
      const auto& dst = ir::kind(ins) == K::ZeroExtend ? std::get<ir::ZeroExtend>(ins).dst
                                                       : std::get<ir::Truncate>(ins).dst;
      const Read v = read(frame, src);
      write(frame, dst, v.value & mask(dst_width(dst)));
      return std::nullopt;
    }
    case K::IntToFloat:
    case K::IntToDouble: {
      const bool single = ir::kind(ins) == K::IntToFloat;
      const auto& src = single ? std::get<ir::IntToFloat>(ins).src : std::get<ir::IntToDouble>(ins).src;
      const auto& dst = single ? std::get<ir::IntToFloat>(ins).dst : std::get<ir::IntToDouble>(ins).dst;
      const Read v = read(frame, src);
      const Int n = v.ti.is_signed ? to_signed(v.value, v.ti.width) : Int(v.value);
      write(frame, dst, low64(single ? fp_arith::int_to_single_bits(n) : fp_arith::int_to_double_bits(n)));
      return std::nullopt;
    }
    case K::FloatToInt:
    case K::DoubleToInt: {
      const bool single = ir::kind(ins) == K::FloatToInt;
      const auto& src = single ? std::get<ir::FloatToInt>(ins).src : std::get<ir::DoubleToInt>(ins).src;
      const auto& dst = single ? std::get<ir::FloatToInt>(ins).dst : std::get<ir::DoubleToInt>(ins).dst;
      const Read v = read(frame, src);
      const std::optional<fp_arith::TruncatedInt> n =
          single ? fp_arith::single_bits_to_int(Int(v.value)) : fp_arith::double_bits_to_int(Int(v.value));
      if (!n) {
        // Python's `int` of a NaN or an infinity.
        const bool nan = single ? (v.value & 0x7FFFFF) != 0 : (v.value & 0xFFFFFFFFFFFFFull) != 0;
        if (nan) throw SimError("ValueError", "cannot convert float NaN to integer");
        throw SimError("OverflowError", "cannot convert float infinity to integer");
      }
      write(frame, dst, low64(n->wrapped(8 * dst_width(dst))));
      return std::nullopt;
    }
    case K::FloatToDouble: {
      const auto& c = std::get<ir::FloatToDouble>(ins);
      const Read v = read(frame, c.src);
      write(frame, c.dst, low64(fp_arith::single_bits_to_double_bits(Int(v.value))));
      return std::nullopt;
    }
    case K::DoubleToFloat: {
      const auto& c = std::get<ir::DoubleToFloat>(ins);
      const Read v = read(frame, c.src);
      write(frame, c.dst, low64(fp_arith::double_bits_to_single_bits(Int(v.value))));
      return std::nullopt;
    }
    case K::GetAddress: {
      const auto& g = std::get<ir::GetAddress>(ins);
      const std::int64_t addr = address_of(frame, g.operand);
      write(frame, g.dst, static_cast<std::uint64_t>(addr));
      return std::nullopt;
    }
    case K::Load: {
      const auto& l = std::get<ir::Load>(ins);
      const Read p = read(frame, l.src_ptr);
      const int w = dst_width(l.dst);
      write(frame, l.dst, memory_.load(static_cast<std::int64_t>(p.value), w));
      return std::nullopt;
    }
    case K::Store: {
      const auto& s = std::get<ir::Store>(ins);
      if (is_aggregate_var(s.src)) {
        const std::int64_t src = address_of(frame, s.src);
        const Read p = read(frame, s.dst_ptr);
        memcpy(static_cast<std::int64_t>(p.value), src,
               size_of(symbol(std::get<ir::Var>(s.src).name).type));
      } else {
        const Read v = read(frame, s.src);
        const Read p = read(frame, s.dst_ptr);
        memory_.store(static_cast<std::int64_t>(p.value), v.value, v.ti.width);
      }
      return std::nullopt;
    }
    case K::IndexedLoad: {
      // A load from a static array: the name's address plus the byte
      // index.
      const auto& l = std::get<ir::IndexedLoad>(ins);
      std::optional<std::int64_t> base;
      base = static_address(l.name);
      if (!base && is_local_aggregate(l.name)) {
        // A member of an automatic struct, union, or array, read in
        // place (direct_member_access).
        base = address_of_var(frame, l.name);
      }
      if (!base && is_local_access(l.name, l.index)) {
        // A volatile automatic scalar, which c99_to_tac reads with an
        // IndexedLoad at index 0.
        const Read v = read(frame, ir::Var{.name = l.name});
        write(frame, l.dst, v.value);
        return std::nullopt;
      }
      if (!base) fail("IndexedLoad on unknown static " + pyrepr::string(l.name));
      const Read idx = read(frame, l.index);
      const int w = dst_width(l.dst);
      const std::int64_t at = *base + address(l.offset) + static_cast<std::int64_t>(idx.value & 0xFF);
      if (l.plane != 0) {
        // A table split into planes of bytes.
        const std::int64_t plane = address(l.plane);
        std::uint64_t v = 0;
        for (int k = 0; k < w; ++k) {
          v |= memory_.load(at + k * plane, 1) << (8 * k);
        }
        write(frame, l.dst, v);
        return std::nullopt;
      }
      write(frame, l.dst, memory_.load(at, w));
      return std::nullopt;
    }
    case K::IndexedSymbolStore: {
      const auto& s = std::get<ir::IndexedSymbolStore>(ins);
      std::optional<std::int64_t> base;
      base = static_address(s.name);
      if (!base && is_local_aggregate(s.name)) base = address_of_var(frame, s.name);
      if (!base && is_local_access(s.name, s.index)) {
        const Read v = read(frame, s.src);
        write_var(frame, s.name, v.value);
        return std::nullopt;
      }
      if (!base) fail("IndexedSymbolStore on unknown static " + pyrepr::string(s.name));
      const Read idx = read(frame, s.index);
      const Read v = read(frame, s.src);
      memory_.store(*base + address(s.offset) + static_cast<std::int64_t>(idx.value & 0xFF), v.value,
                    v.ti.width);
      return std::nullopt;
    }
    case K::IndexedStore: {
      const auto& s = std::get<ir::IndexedStore>(ins);
      const Read idx = read(frame, s.index);
      const Read v = read(frame, s.src);
      memory_.store(static_cast<std::int64_t>(low64(s.address + Int(idx.value & 0xFF)) & 0xFFFF),
                    v.value, v.ti.width);
      return std::nullopt;
    }
    case K::IndexedConstLoad: {
      const auto& l = std::get<ir::IndexedConstLoad>(ins);
      const Read idx = read(frame, l.index);
      const std::int64_t at = static_cast<std::int64_t>(low64(l.address + Int(idx.value & 0xFF)) & 0xFFFF);
      write(frame, l.dst, memory_.load(at, dst_width(l.dst)));
      return std::nullopt;
    }
    case K::IndirectIndexedLoad: {
      const auto& l = std::get<ir::IndirectIndexedLoad>(ins);
      const Read p = read(frame, l.ptr);
      const Read idx = read(frame, l.index);
      const auto at = static_cast<std::int64_t>((p.value + (idx.value & 0xFF)) & 0xFFFF);
      write(frame, l.dst, memory_.load(at, dst_width(l.dst)));
      return std::nullopt;
    }
    case K::IndirectIndexedStore: {
      const auto& s = std::get<ir::IndirectIndexedStore>(ins);
      const Read p = read(frame, s.ptr);
      const Read idx = read(frame, s.index);
      const Read v = read(frame, s.src);
      memory_.store(static_cast<std::int64_t>((p.value + (idx.value & 0xFF)) & 0xFFFF), v.value,
                    v.ti.width);
      return std::nullopt;
    }
    case K::Ret: {
      const auto& r = std::get<ir::Ret>(ins);
      if (!r.val) return Return{std::nullopt, 0};
      if (is_aggregate_var(*r.val)) {
        // A `return_bool_in_carry` struct, the only struct a Ret carries,
        // since the others go through the sret pointer. Its bytes travel
        // as one little-endian integer.
        const std::int64_t size = size_of(symbol(std::get<ir::Var>(*r.val).name).type);
        const std::int64_t addr = address_of(frame, *r.val);
        return Return{memory_.load_int(addr, size), size};
      }
      const Read v = read(frame, *r.val);
      return Return{Int(v.value), v.ti.width};
    }
    case K::FunctionCall: {
      const auto& c = std::get<ir::FunctionCall>(ins);
      std::vector<Int> args = call_args(frame, c.args);
      frame.call_dst = &c.dst;
      push_frame(c.name, args);
      return std::nullopt;
    }
    case K::IndirectCall: {
      const auto& c = std::get<ir::IndirectCall>(ins);
      const Read p = read(frame, c.ptr);
      const auto it = addr_to_function_.find(static_cast<std::int64_t>(p.value));
      if (it == addr_to_function_.end()) {
        fail("IndirectCall to unknown address " + hex_address(p.value));
      }
      std::vector<Int> args = call_args(frame, c.args);
      frame.call_dst = &c.dst;
      push_frame(it->second, args);
      return std::nullopt;
    }
  }
  return std::nullopt;
}

Outcome run_main(const ir::Program& program, const passes::SymbolTable& symbols,
                 const passes::TypeTable* types) {
  Outcome out;
  try {
    Simulator sim(program, symbols, types);
    out.value = sim.call("main", {});
  } catch (const SimError& e) {
    out.error_type = e.python_type();
    out.error_message = e.what();
  }
  return out;
}

}  // namespace c6502::tac_sim
