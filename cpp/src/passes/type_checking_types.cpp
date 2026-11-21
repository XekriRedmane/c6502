#include "passes/type_checking_types.h"

#include "bit_precise.h"
#include "errors.h"

namespace c6502::passes::type_checking {

namespace ir = ::c6502::c99_ast;
using K = ir::Kind_data_type;

const Type& strip_quals(const Type& t) {
  const Type* p = &t;
  while (true) {
    if (const auto* c = std::get_if<ir::Const>(p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = std::get_if<ir::Volatile>(p)) {
      p = &*v->referenced_type;
    } else {
      return *p;
    }
  }
}

bool is_integer_type(const Type& t) {
  switch (ir::kind(strip_quals(t))) {
    case K::Int:
    case K::Long:
    case K::LongLong:
    case K::UInt:
    case K::ULong:
    case K::ULongLong:
    case K::Char:
    case K::SChar:
    case K::UChar:
    case K::Bool:
    case K::Short:
    case K::UShort:
    case K::BitInt:
      return true;
    default:
      return false;
  }
}

bool is_floating_type(const Type& t) { return asdl::isa<ir::Float, ir::Double>(strip_quals(t)); }

bool is_pointer_type(const Type& t) { return asdl::isa<ir::Pointer>(strip_quals(t)); }

bool is_struct_or_union(const Type& t) {
  return asdl::isa<ir::Structure, ir::Union>(strip_quals(t));
}

bool is_array_type(const Type& t) { return asdl::isa<ir::Array>(strip_quals(t)); }

bool is_nullptr_type(const Type& t) { return asdl::isa<ir::NullptrT>(strip_quals(t)); }

bool is_void(const Type& t) { return asdl::isa<ir::Void>(strip_quals(t)); }

bool is_void_pointer(const Type& t) {
  const auto* p = std::get_if<ir::Pointer>(&strip_quals(t));
  return p != nullptr && asdl::isa<ir::Void>(strip_quals(*p->referenced_type));
}

std::string display_name(std::string_view name) {
  if (!name.empty() && name[0] == '@') {
    const std::size_t dot = name.find('.');
    if (dot != std::string_view::npos) {
      return std::string(name.substr(dot + 1));
    }
  }
  return std::string(name);
}

std::string c_type(const Type& t) {
  switch (ir::kind(t)) {
    case K::Const:
      return "const " + c_type(*std::get<ir::Const>(t).referenced_type);
    case K::Volatile:
      return "volatile " + c_type(*std::get<ir::Volatile>(t).referenced_type);
    case K::Pointer:
      return c_type(*std::get<ir::Pointer>(t).referenced_type) + " *";
    case K::Array: {
      const auto& a = std::get<ir::Array>(t);
      return c_type(*a.element_type) + "[" + asdl::int_to_string(a.size) + "]";
    }
    case K::Structure:
      return "struct " + display_name(std::get<ir::Structure>(t).tag);
    case K::Union:
      return "union " + display_name(std::get<ir::Union>(t).tag);
    case K::FunType:
      return "a function type";
    case K::BitInt: {
      const auto& b = std::get<ir::BitInt>(t);
      return std::string(b.is_signed ? "" : "unsigned ") + "_BitInt(" +
             asdl::int_to_string(b.width) + ")";
    }
    case K::Char:
      return "char";
    case K::SChar:
      return "signed char";
    case K::UChar:
      return "unsigned char";
    case K::Bool:
      return "bool";
    case K::NullptrT:
      return "nullptr_t";
    case K::Short:
      return "short";
    case K::UShort:
      return "unsigned short";
    case K::Int:
      return "int";
    case K::UInt:
      return "unsigned int";
    case K::Long:
      return "long";
    case K::ULong:
      return "unsigned long";
    case K::LongLong:
      return "long long";
    case K::ULongLong:
      return "unsigned long long";
    case K::Float:
      return "float";
    case K::Double:
      return "double";
    case K::Void:
      return "void";
  }
  throw InternalError("c_type: unknown type");
}

int int_width(const Type& t) {
  const Type& s = strip_quals(t);
  switch (ir::kind(s)) {
    case K::BitInt:
      return static_cast<int>(bit_precise::storage_bytes(std::get<ir::BitInt>(s).width));
    case K::Char:
    case K::SChar:
    case K::UChar:
    case K::Bool:
      return 1;
    case K::Int:
    case K::UInt:
    case K::Short:
    case K::UShort:
      return 2;
    case K::Long:
    case K::ULong:
      return 4;
    case K::LongLong:
    case K::ULongLong:
      return 8;
    default:
      throw InternalError("_int_width: not an integer object type: " + asdl::py_repr(s));
  }
}

int int_bits(const Type& t) {
  const Type& s = strip_quals(t);
  if (const auto* b = std::get_if<ir::BitInt>(&s)) {
    return static_cast<int>(b->width);
  }
  return 8 * int_width(s);
}

bool is_signed(const Type& t) {
  const Type& s = strip_quals(t);
  if (const auto* b = std::get_if<ir::BitInt>(&s)) {
    return b->is_signed;
  }
  return asdl::isa<ir::Int, ir::Long, ir::LongLong, ir::SChar, ir::Short>(s);
}

std::pair<int, int> rank(const Type& t) {
  // The standard integer types left after the integer promotions, each
  // with its width in bits and its place in the order of rank `short <
  // int < long < long long` (Python's `_STANDARD_RANKS`).
  switch (ir::kind(t)) {
    case K::BitInt:
      return {static_cast<int>(std::get<ir::BitInt>(t).width), 0};
    case K::Int:
    case K::UInt:
      return {16, 3};
    case K::Long:
    case K::ULong:
      return {32, 4};
    case K::LongLong:
    case K::ULongLong:
      return {64, 5};
    default:
      throw InternalError("_rank: not a promoted integer type: " + asdl::py_repr(t));
  }
}

namespace {

// Returns the unsigned type of the same rank as the promoted integer
// type `t`.
Type unsigned_counterpart(const Type& t) {
  switch (ir::kind(t)) {
    case K::BitInt:
      return ir::BitInt{.width = std::get<ir::BitInt>(t).width, .is_signed = false};
    case K::Int:
    case K::UInt:
      return ir::UInt{};
    case K::Long:
    case K::ULong:
      return ir::ULong{};
    case K::LongLong:
    case K::ULongLong:
      return ir::ULongLong{};
    default:
      throw InternalError("_unsigned_counterpart: not a promoted integer type: " +
                          asdl::py_repr(t));
  }
}

// Returns a new value of `t`'s class, as Python's `type(t)()` makes
// one, which only a type without fields allows.
Type fresh(const Type& t) {
  switch (ir::kind(t)) {
    case K::BitInt:
    case K::FunType:
    case K::Pointer:
    case K::Array:
    case K::Structure:
    case K::Union:
    case K::Const:
    case K::Volatile:
      throw InternalError("common_type: a type with fields: " + asdl::py_repr(t));
    default:
      return t;
  }
}

}  // namespace

Type promote_integer(const Type& t) {
  const Type& s = strip_quals(t);
  if (asdl::isa<ir::Char, ir::SChar, ir::UChar, ir::Bool, ir::Short>(s)) {
    return ir::Int{};
  }
  if (asdl::isa<ir::UShort>(s)) {
    return ir::UInt{};
  }
  return s;
}

asdl::Int coerce_int_to_type(asdl::Int value, const Type& t) {
  if (asdl::isa<ir::Bool>(strip_quals(t))) {
    return value != 0 ? 1 : 0;
  }
  const int width_bits = int_bits(t);
  const asdl::Int one = 1;
  const asdl::Int mask = (one << width_bits) - 1;
  asdl::Int raw = value & mask;
  if (is_signed(t) && (raw & (one << (width_bits - 1))) != 0) {
    raw -= one << width_bits;
  }
  return raw;
}

Type common_bit_precise_type(const Type& a, const Type& b) {
  if (a == b) {
    return a;
  }
  const bool a_signed = is_signed(a);
  const bool b_signed = is_signed(b);
  if (a_signed == b_signed) {
    return rank(a) >= rank(b) ? a : b;
  }
  const Type& signed_t = a_signed ? a : b;
  const Type& unsigned_t = a_signed ? b : a;
  if (rank(unsigned_t) >= rank(signed_t)) {
    return unsigned_t;
  }
  // The signed type has the greater rank. It's the result when it holds
  // every value of the unsigned type, and otherwise the unsigned type
  // corresponding to it is.
  if (int_bits(signed_t) - 1 >= int_bits(unsigned_t)) {
    return signed_t;
  }
  return unsigned_counterpart(signed_t);
}

Type common_type(const Type& a_in, const Type& b_in) {
  const Type& a = strip_quals(a_in);
  const Type& b = strip_quals(b_in);
  if (asdl::isa<ir::Double>(a) || asdl::isa<ir::Double>(b)) {
    return ir::Double{};
  }
  if (asdl::isa<ir::Float>(a) || asdl::isa<ir::Float>(b)) {
    return ir::Float{};
  }
  if (asdl::isa<ir::BitInt>(a) || asdl::isa<ir::BitInt>(b)) {
    return common_bit_precise_type(a, b);
  }
  if (a == b) {
    return fresh(a);
  }
  const bool a_signed = is_signed(a);
  const bool b_signed = is_signed(b);
  if (a_signed == b_signed) {
    // Both signed or both unsigned, and the higher rank wins.
    return int_width(a) >= int_width(b) ? fresh(a) : fresh(b);
  }
  const Type& signed_t = a_signed ? a : b;
  const Type& unsigned_t = a_signed ? b : a;
  if (int_width(unsigned_t) >= int_width(signed_t)) {
    return fresh(unsigned_t);
  }
  // The signed type has the higher rank, and with c6502's widths its
  // range covers the unsigned type's: `long` covers `unsigned int`, and
  // `long long` covers `unsigned int` and `unsigned long`.
  return fresh(signed_t);
}

}  // namespace c6502::passes::type_checking
