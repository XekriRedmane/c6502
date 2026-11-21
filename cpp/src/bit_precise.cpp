#include "bit_precise.h"

#include <string>

#include "errors.h"

namespace c6502::bit_precise {

namespace tac = ::c6502::tac_ast;

std::optional<ConstLayout> odd_const_layout(const tac::Type_const& c) {
  using K = tac::Kind_const;
  switch (tac::kind(c)) {
    case K::ConstInt24:
      return ConstLayout{3, true};
    case K::ConstUInt24:
      return ConstLayout{3, false};
    case K::ConstInt40:
      return ConstLayout{5, true};
    case K::ConstUInt40:
      return ConstLayout{5, false};
    case K::ConstInt48:
      return ConstLayout{6, true};
    case K::ConstUInt48:
      return ConstLayout{6, false};
    case K::ConstInt56:
      return ConstLayout{7, true};
    case K::ConstUInt56:
      return ConstLayout{7, false};
    default:
      return std::nullopt;
  }
}

std::optional<ConstLayout> integer_const_layout(const tac::Type_const& c) {
  using K = tac::Kind_const;
  switch (tac::kind(c)) {
    case K::ConstChar:
      return ConstLayout{1, true};
    case K::ConstUChar:
      return ConstLayout{1, false};
    case K::ConstInt:
      return ConstLayout{2, true};
    case K::ConstUInt:
      return ConstLayout{2, false};
    case K::ConstLong:
      return ConstLayout{4, true};
    case K::ConstULong:
      return ConstLayout{4, false};
    case K::ConstLongLong:
      return ConstLayout{8, true};
    case K::ConstULongLong:
      return ConstLayout{8, false};
    default:
      return odd_const_layout(c);
  }
}

asdl::Int storage_bytes(asdl::Int width) { return asdl::floor_div(width + 7, 8); }

int helper_bytes(int n) {
  for (const int w : {1, 2, 4, 8}) {
    if (w >= n) {
      return w;
    }
  }
  throw InternalError("no runtime helper takes " + std::to_string(n) + "-byte operands");
}

asdl::Int wrap(asdl::Int value, int width, bool is_signed) {
  const asdl::Int modulus = asdl::Int(1) << width;
  value = asdl::floor_mod(value, modulus);
  if (is_signed && (value >> (width - 1)) != 0) {
    value -= modulus;
  }
  return value;
}

std::optional<asdl::Int> raw_value(const tac::Type_const& c) {
  using K = tac::Kind_const;
  switch (tac::kind(c)) {
    case K::ConstFloat:
    case K::ConstDouble:
      return std::nullopt;
    case K::ConstChar:
      return std::get<tac::ConstChar>(c).value;
    case K::ConstUChar:
      return std::get<tac::ConstUChar>(c).value;
    case K::ConstInt:
      return std::get<tac::ConstInt>(c).value;
    case K::ConstLong:
      return std::get<tac::ConstLong>(c).value;
    case K::ConstLongLong:
      return std::get<tac::ConstLongLong>(c).value;
    case K::ConstUInt:
      return std::get<tac::ConstUInt>(c).value;
    case K::ConstULong:
      return std::get<tac::ConstULong>(c).value;
    case K::ConstULongLong:
      return std::get<tac::ConstULongLong>(c).value;
    case K::ConstInt24:
      return std::get<tac::ConstInt24>(c).value;
    case K::ConstUInt24:
      return std::get<tac::ConstUInt24>(c).value;
    case K::ConstInt40:
      return std::get<tac::ConstInt40>(c).value;
    case K::ConstUInt40:
      return std::get<tac::ConstUInt40>(c).value;
    case K::ConstInt48:
      return std::get<tac::ConstInt48>(c).value;
    case K::ConstUInt48:
      return std::get<tac::ConstUInt48>(c).value;
    case K::ConstInt56:
      return std::get<tac::ConstInt56>(c).value;
    case K::ConstUInt56:
      return std::get<tac::ConstUInt56>(c).value;
  }
  return std::nullopt;
}

tac::Type_const with_value(const tac::Type_const& c, asdl::Int value) {
  using K = tac::Kind_const;
  switch (tac::kind(c)) {
    case K::ConstChar:
      return tac::ConstChar{value};
    case K::ConstUChar:
      return tac::ConstUChar{value};
    case K::ConstInt:
      return tac::ConstInt{value};
    case K::ConstLong:
      return tac::ConstLong{value};
    case K::ConstLongLong:
      return tac::ConstLongLong{value};
    case K::ConstUInt:
      return tac::ConstUInt{value};
    case K::ConstULong:
      return tac::ConstULong{value};
    case K::ConstULongLong:
      return tac::ConstULongLong{value};
    case K::ConstInt24:
      return tac::ConstInt24{value};
    case K::ConstUInt24:
      return tac::ConstUInt24{value};
    case K::ConstInt40:
      return tac::ConstInt40{value};
    case K::ConstUInt40:
      return tac::ConstUInt40{value};
    case K::ConstInt48:
      return tac::ConstInt48{value};
    case K::ConstUInt48:
      return tac::ConstUInt48{value};
    case K::ConstInt56:
      return tac::ConstInt56{value};
    case K::ConstUInt56:
      return tac::ConstUInt56{value};
    case K::ConstFloat:
    case K::ConstDouble:
      break;
  }
  throw InternalError("with_value of a floating constant");
}

asdl::Int const_value(const tac::Type_const& c) {
  const auto layout = integer_const_layout(c);
  if (!layout) {
    throw InternalError("const_value of a floating constant");
  }
  const asdl::Int v = *raw_value(c);
  return wrap(v, 8 * layout->bytes, layout->is_signed);
}

tac::Type_const make_const(int n, bool is_signed, asdl::Int value) {
  const asdl::Int v = asdl::floor_mod(value, asdl::Int(1) << (8 * n));
  switch (n) {
    case 1:
      return is_signed ? tac::Type_const{tac::ConstChar{v}} : tac::Type_const{tac::ConstUChar{v}};
    case 2:
      return is_signed ? tac::Type_const{tac::ConstInt{v}} : tac::Type_const{tac::ConstUInt{v}};
    case 3:
      return is_signed ? tac::Type_const{tac::ConstInt24{v}} : tac::Type_const{tac::ConstUInt24{v}};
    case 4:
      return is_signed ? tac::Type_const{tac::ConstLong{v}} : tac::Type_const{tac::ConstULong{v}};
    case 5:
      return is_signed ? tac::Type_const{tac::ConstInt40{v}} : tac::Type_const{tac::ConstUInt40{v}};
    case 6:
      return is_signed ? tac::Type_const{tac::ConstInt48{v}} : tac::Type_const{tac::ConstUInt48{v}};
    case 7:
      return is_signed ? tac::Type_const{tac::ConstInt56{v}} : tac::Type_const{tac::ConstUInt56{v}};
    case 8:
      return is_signed ? tac::Type_const{tac::ConstLongLong{v}}
                       : tac::Type_const{tac::ConstULongLong{v}};
    default:
      throw InternalError("no integer constant class of " + std::to_string(n) + " bytes");
  }
}

}  // namespace c6502::bit_precise
