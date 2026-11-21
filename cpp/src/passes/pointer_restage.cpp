// Reads the pointer again for each byte of a wide access through it.
//
// `tac_to_asm` lowers a load or store of a value wider than a byte, at a
// constant offset through a pointer, by copying the pointer into DPTR
// once and then reading or writing each byte through `(DPTR),Y`. When the
// pointer is in zero page, `memory_value_propagation` turns each
// `(DPTR),Y` into `(p),Y` while it can show that DPTR still holds a copy
// of `p`, and the copy into DPTR goes. Two things stopped that for the
// second byte.
//
// - A store through a pointer ends every fact the pass holds, since the
//   byte it writes could be one of `p`'s. The first byte of `P->gold = 0`
//   ended the fact that DPTR copies `P`, so the second byte was stored
//   through DPTR and the copy stayed (8 bytes).
// - The register allocator gave the pointer's bytes to another value
//   once the copy into DPTR had read them. The low byte of `r->gold`,
//   read through a pointer parameter `r`, went into `r`'s own byte, so
//   the high byte could only be read through DPTR.
//
// `tac_to_asm` copies the pointer into DPTR again before each byte after
// the first when this module allows it. The second copy reads the
// pointer's bytes after the earlier bytes are read or written, which
// keeps the pointer alive to the last byte, and each copy gives
// `memory_value_propagation` a fact of its own, so every byte is read or
// written through `(p),Y` and every copy into DPTR goes.
//
// Copying again is the same as reading the pointer again. For a load
// nothing is written between the copies, so the pointer has the value it
// had. For a store, the earlier bytes of the same store are written
// between the copies, and they could only change the pointer if the
// store's bytes overlapped the object that holds it. C's effective-type
// rule (C23 §6.5p7) makes that overlap undefined in the cases
// `restage_store` accepts:
//
// - The pointer is a TAC variable of pointer type, so the bytes it is
//   copied from are an object of that type, or a temporary that no C
//   lvalue can reach.
// - The store writes the scalar that the pointer's referenced type has
//   at exactly those bytes (`scalar_at`), a member, an element, or the
//   whole referenced object. A union and a byte that only part of a
//   scalar covers give no type, and the store is left alone.
// - That scalar is no character type, so the store doesn't have the
//   character type's licence to write any object, and its type isn't
//   compatible with the pointer's own type. An integer or floating type
//   is never compatible with a pointer type. A pointer type is compared
//   with the pointer's type with every qualifier removed, and a pointer
//   to an array or a function is taken to be compatible, which leaves
//   `P->next = q` alone for a `struct node *P`, since a node that holds
//   its own address is defined C.
//
// A store through a pointer whose referenced type the program casts
// (`((struct t *)p)->m`) is judged by the type TAC gives the pointer,
// which copy propagation may make `p`'s own type. Writing `p`'s bytes
// through a member of a structure that isn't there is outside what the
// rule above covers, and the lowering assumes no program does it.
//
// The copy pays only when `memory_value_propagation` can remove it,
// which is possible only for a pointer in zero page: a TAC temporary or
// local, which the register allocator puts in zero page under `-O`
// unless it spills, a parameter's or local's zero-page slot, or a
// zero-page object (`AsmContext::is_user_zp_global`).
// `pointer_in_zero_page` checks that, and the unoptimized pipeline never
// copies again.
#include "passes/pointer_restage.h"

#include <string_view>

namespace c6502::passes::pointer_restage {

namespace c99 = ::c6502::c99_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

const c99::Type_data_type& unqualified(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  while (true) {
    if (const auto* c = as<c99::Const>(*p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = as<c99::Volatile>(*p)) {
      p = &*v->referenced_type;
    } else {
      return *p;
    }
  }
}

// Returns `t` with every `const` and `volatile` removed, through
// pointers and arrays.
c99::Type_data_type deep_unqualified(const c99::Type_data_type& t) {
  const c99::Type_data_type& u = unqualified(t);
  if (const auto* p = as<c99::Pointer>(u)) {
    return c99::Pointer{.referenced_type = deep_unqualified(*p->referenced_type)};
  }
  if (const auto* a = as<c99::Array>(u)) {
    return c99::Array{.element_type = deep_unqualified(*a->element_type), .size = a->size};
  }
  return u;
}

bool is_character(const c99::Type_data_type& t) {
  return isa<c99::Char, c99::SChar, c99::UChar, c99::Bool>(t);
}

// Returns the size of `t` in bytes, or none when it isn't known.
std::optional<asdl::Int> size_of(const c99::Type_data_type& type, const TypeTable* types) {
  const c99::Type_data_type& t = unqualified(type);
  if (is_character(t)) {
    return 1;
  }
  if (isa<c99::Int, c99::UInt, c99::Short, c99::UShort, c99::Pointer>(t)) {
    return 2;
  }
  if (isa<c99::Long, c99::ULong, c99::Float>(t)) {
    return 4;
  }
  if (isa<c99::LongLong, c99::ULongLong, c99::Double>(t)) {
    return 8;
  }
  if (const auto* b = as<c99::BitInt>(t)) {
    return asdl::floor_div(b->width + 7, 8);
  }
  if (const auto* a = as<c99::Array>(t)) {
    const auto e = size_of(*a->element_type, types);
    if (!e) {
      return std::nullopt;
    }
    return *e * a->size;
  }
  if (types != nullptr && !types->empty()) {
    const std::string* tag = nullptr;
    if (const auto* s = as<c99::Structure>(t)) {
      tag = &s->tag;
    } else if (const auto* u = as<c99::Union>(t)) {
      tag = &u->tag;
    }
    if (tag != nullptr) {
      const StructLayout* layout = types->find(*tag);
      if (layout != nullptr && layout->complete) {
        return layout->size;
      }
    }
  }
  return std::nullopt;
}

}  // namespace

const c99::Type_data_type* scalar_at(const c99::Type_data_type& type, asdl::Int k, asdl::Int n,
                                     const TypeTable* types) {
  const c99::Type_data_type& t = unqualified(type);
  if (const auto* a = as<c99::Array>(t)) {
    const auto e = size_of(*a->element_type, types);
    if (!e || *e == 0 || k < 0 || k >= *e * a->size) {
      return nullptr;
    }
    return scalar_at(*a->element_type, asdl::floor_mod(k, *e), n, types);
  }
  if (const auto* s = as<c99::Structure>(t)) {
    const StructLayout* layout =
        types != nullptr && !types->empty() ? types->find(s->tag) : nullptr;
    if (layout == nullptr || !layout->complete) {
      return nullptr;
    }
    for (const auto& m : layout->members) {
      const auto size = size_of(m.type, types);
      if (size && m.byte_offset <= k && k < m.byte_offset + *size) {
        return scalar_at(m.type, k - m.byte_offset, n, types);
      }
    }
    return nullptr;
  }
  if (isa<c99::Union, c99::Void, c99::FunType>(t)) {
    return nullptr;
  }
  if (k == 0) {
    const auto size = size_of(t, types);
    if (size && *size == n) {
      return &t;
    }
  }
  return nullptr;
}

bool store_keeps_pointer(const c99::Type_data_type& ptr_type, const c99::Type_data_type& stored) {
  const c99::Type_data_type& s = unqualified(stored);
  if (is_character(s)) {
    return false;
  }
  if (!isa<c99::Pointer>(s)) {
    return true;
  }
  const c99::Type_data_type da = deep_unqualified(s);
  const c99::Type_data_type db = deep_unqualified(ptr_type);
  const c99::Type_data_type& a = *std::get<c99::Pointer>(da).referenced_type;
  const c99::Type_data_type& b = *std::get<c99::Pointer>(db).referenced_type;
  if (isa<c99::Array, c99::FunType>(a) || isa<c99::Array, c99::FunType>(b)) {
    return false;
  }
  return !(a == b);
}

bool pointer_in_zero_page(const asm_ast::Type_operand& op, const AsmContext& ctx) {
  if (isa<asm_ast::Pseudo>(op)) {
    return true;
  }
  if (const auto* d = as<asm_ast::Data>(op)) {
    const std::string_view name = d->name;
    return name.starts_with("__zpabi_") || name.starts_with("__local_") ||
           ctx.is_user_zp_global(name);
  }
  return false;
}

bool restage_store(const c99::Type_data_type* ptr_type, asdl::Int offset, asdl::Int n,
                   const TypeTable* types) {
  if (n < 2 || ptr_type == nullptr) {
    return false;
  }
  const c99::Type_data_type& p = unqualified(*ptr_type);
  const auto* ptr = as<c99::Pointer>(p);
  if (ptr == nullptr) {
    return false;
  }
  const c99::Type_data_type* stored = scalar_at(*ptr->referenced_type, offset, n, types);
  if (stored == nullptr) {
    return false;
  }
  return store_keeps_pointer(p, *stored);
}

}  // namespace c6502::passes::pointer_restage
