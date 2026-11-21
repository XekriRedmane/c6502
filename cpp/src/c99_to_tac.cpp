/* The synthetic-constant helper of c99_to_tac.py, `_tac_const_for`,
which the TAC optimizer's passes call (see the header). */
#include "c99_to_tac.h"

#include "bit_precise.h"
#include "errors.h"
#include "fp_arith.h"

namespace c6502::c99_to_tac {

namespace c99 = ::c6502::c99_ast;
namespace tac = ::c6502::tac_ast;

tac::Type_const tac_const_for(const c99::Type_data_type& type, asdl::Int value) {
  const c99::Type_data_type* t = &type;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(t)) {
      t = &*v->referenced_type;
    } else {
      break;
    }
  }
  using K = c99::Kind_data_type;
  switch (c99::kind(*t)) {
    case K::BitInt: {
      // The value modulo 2^N, held as an integer of the storage width,
      // which is how the padding bits are kept (bit_precise.py).
      const auto& b = std::get<c99::BitInt>(*t);
      const int width = static_cast<int>(b.width);
      return bit_precise::make_const(static_cast<int>(bit_precise::storage_bytes(b.width)),
                                     b.is_signed, bit_precise::wrap(value, width, b.is_signed));
    }
    case K::SChar:
      return tac::ConstChar{value};
    case K::Char:
    case K::UChar:
    case K::Bool:
      return tac::ConstUChar{value};
    case K::Int:
      return tac::ConstInt{value};
    case K::UInt:
    case K::Pointer:
      return tac::ConstUInt{value};
    case K::Long:
      return tac::ConstLong{value};
    case K::ULong:
      return tac::ConstULong{value};
    case K::LongLong:
      return tac::ConstLongLong{value};
    case K::ULongLong:
      return tac::ConstULongLong{value};
    case K::Float:
      return tac::ConstFloat{fp_arith::int_to_single_bits(value)};
    case K::Double:
      return tac::ConstDouble{fp_arith::int_to_double_bits(value)};
    default:
      throw InternalError("cannot build a TAC const for non-object type");
  }
}

}  // namespace c6502::c99_to_tac
