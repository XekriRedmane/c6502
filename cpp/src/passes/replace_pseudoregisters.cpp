#include "passes/replace_pseudoregisters.h"

#include <variant>

namespace c6502::passes {

namespace c99 = ::c6502::c99_ast;

asdl::Int sizeof_type(const c99::Type_data_type& t, const TypeTable* types) {
  using K = c99::Kind_data_type;
  switch (c99::kind(t)) {
    case K::Const:
      return sizeof_type(*std::get<c99::Const>(t).referenced_type, types);
    case K::Volatile:
      return sizeof_type(*std::get<c99::Volatile>(t).referenced_type, types);
    case K::Char:
    case K::SChar:
    case K::UChar:
      return 1;
    case K::Int:
    case K::UInt:
    case K::Pointer:
      return 2;
    case K::Long:
    case K::ULong:
    case K::Float:
      return 4;
    case K::LongLong:
    case K::ULongLong:
    case K::Double:
      return 8;
    case K::BitInt:
      // ceil(N/8). After c99_to_tac only 3, 5, 6, and 7 bytes are left
      // of these.
      return asdl::floor_div(std::get<c99::BitInt>(t).width + 7, 8);
    case K::Array: {
      const auto& a = std::get<c99::Array>(t);
      return sizeof_type(*a.element_type, types) * a.size;
    }
    case K::Structure:
    case K::Union: {
      if (types == nullptr) {
        return 1;
      }
      const std::string& tag = c99::kind(t) == K::Structure ? std::get<c99::Structure>(t).tag
                                                            : std::get<c99::Union>(t).tag;
      const StructLayout* layout = types->find(tag);
      if (layout == nullptr || !layout->complete) {
        return 1;
      }
      return layout->size;
    }
    default:
      return 1;
  }
}

asdl::Int size_of_name(std::string_view name, const SymbolTable* symbols, const TypeTable* types) {
  if (symbols == nullptr) {
    return 1;
  }
  const Symbol* sym = symbols->find(name);
  if (sym == nullptr) {
    return 1;
  }
  return sizeof_type(sym->type, types);
}

}  // namespace c6502::passes
