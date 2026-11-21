/* The helpers of passes/optimization/narrow_widened_arith.py that
`recognize_indirect_indexed` and `sink_increment` read. The header
describes each one. The pass of the module, which narrows arithmetic on
promoted bytes, isn't ported here. */
#include "passes/optimization/narrow_widened_arith.h"

#include "bit_precise.h"
#include "errors.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using K = ir::Kind_instruction;

// Returns whether `t`, without its `const` qualifiers, is `volatile`
// (Python's `_is_volatile`).
bool is_volatile_type(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  while (const auto* c = std::get_if<c99::Const>(p)) {
    p = &*c->referenced_type;
  }
  return std::holds_alternative<c99::Volatile>(*p);
}

// Returns `t` without its `const` and `volatile` qualifiers.
const c99::Type_data_type& strip(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(p)) {
      p = &*v->referenced_type;
    } else {
      return *p;
    }
  }
}

// Returns whether `instr` may end a block or write a variable it
// doesn't name (Python's `_ENDS_BLOCK + _MAY_WRITE_MEMORY`).
bool ends_block_or_writes_memory(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Label:
    case K::Ret:
    case K::Jump:
    case K::JumpIfTrue:
    case K::JumpIfFalse:
    case K::JumpIfCmp:
    case K::JumpIfMasked:
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
}

}  // namespace

bool var_unchanged(const ir::Var& var, std::size_t start, std::size_t end,
                   const framework::DefUseEnv& env, const framework::PassContext& ctx) {
  if (ctx.ssa_dsts->contains(var.name)) {
    return true;
  }
  const Symbol* sym = ctx.symbols->find(var.name);
  if (sym == nullptr || is_volatile_type(sym->type) || start >= end) {
    return false;
  }
  for (std::size_t k = start + 1; k < end; ++k) {
    const auto& entry = env.instructions[k];
    if (!entry) {
      continue;
    }
    if (ends_block_or_writes_memory(*entry)) {
      return false;
    }
    for (const auto name : defs_in(*entry)) {
      if (name == var.name) {
        return false;
      }
    }
  }
  return true;
}

}  // namespace c6502::passes

namespace c6502::passes::narrow_widened_arith {

namespace {
namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using DK = c99::Kind_data_type;
}  // namespace

void register_temp(const framework::PassContext& ctx, std::string_view like,
                   const std::string& name) {
  if (!ctx.symbols->contains(name)) {
    const Symbol* src = ctx.symbols->find(like);
    if (src == nullptr) {
      throw InternalError("register_temp: no symbol for " + std::string(like));
    }
    c99::Type_data_type type = src->type;
    ctx.symbols->insert_or_assign(name, Symbol{.type = std::move(type), .attrs = LocalAttr{}});
  }
  ctx.ssa_dsts->insert(name);
}

std::optional<ir::Constant> narrow_constant(const ir::Constant& c, int width, bool is_unsigned) {
  const std::optional<asdl::Int> raw = bit_precise::raw_value(c.const_);
  if (!raw) {
    return std::nullopt;
  }
  if (width != 1 && width != 2 && width != 4 && width != 8) {
    return std::nullopt;
  }
  const asdl::Int modulus = asdl::Int(1) << (8 * width);
  const asdl::Int masked = asdl::floor_mod(*raw, modulus);
  const asdl::Int half = modulus >> 1;
  const asdl::Int as_signed = masked < half ? masked : masked - modulus;
  switch (width) {
    case 1:
      if (is_unsigned) {
        return ir::Constant{.const_ = ir::ConstUChar{.value = masked}};
      }
      return ir::Constant{.const_ = ir::ConstChar{.value = as_signed}};
    case 2:
      if (is_unsigned) {
        return ir::Constant{.const_ = ir::ConstUInt{.value = masked}};
      }
      return ir::Constant{.const_ = ir::ConstInt{.value = as_signed}};
    case 4:
      if (is_unsigned) {
        return ir::Constant{.const_ = ir::ConstULong{.value = masked}};
      }
      return ir::Constant{.const_ = ir::ConstLong{.value = as_signed}};
    default:
      if (is_unsigned) {
        return ir::Constant{.const_ = ir::ConstULongLong{.value = masked}};
      }
      return ir::Constant{.const_ = ir::ConstLongLong{.value = as_signed}};
  }
}

std::optional<int> byte_width(const ir::Var& v, const SymbolTable* symbols) {
  if (symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols->find(v.name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  switch (c99::kind(strip(sym->type))) {
    case DK::Char:
    case DK::SChar:
    case DK::UChar:
      return 1;
    case DK::Int:
    case DK::UInt:
    case DK::Pointer:
      return 2;
    case DK::Long:
    case DK::ULong:
    case DK::Float:
      return 4;
    case DK::LongLong:
    case DK::ULongLong:
    case DK::Double:
      return 8;
    default:
      return std::nullopt;
  }
}

std::optional<bool> is_unsigned(const ir::Var& v, const SymbolTable* symbols) {
  if (symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols->find(v.name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  switch (c99::kind(strip(sym->type))) {
    case DK::Char:
    case DK::UChar:
    case DK::UInt:
    case DK::Pointer:
    case DK::ULong:
    case DK::ULongLong:
      return true;
    case DK::SChar:
    case DK::Int:
    case DK::Long:
    case DK::LongLong:
      return false;
    default:
      return std::nullopt;
  }
}

}  // namespace c6502::passes::narrow_widened_arith
