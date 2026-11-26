#include "passes/optimization/static_const_fold.h"

#include <map>
#include <string>

#include "c99_to_tac.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using TK = c99::Kind_data_type;

const c99::Type_data_type* scalar_type(const c99::Type_data_type& type) {
  bool has_const = false;
  const c99::Type_data_type* inner = &type;
  for (;;) {
    if (asdl::isa<c99::Volatile>(*inner)) {
      return nullptr;
    }
    if (const auto* c = std::get_if<c99::Const>(inner)) {
      has_const = true;
      inner = &*c->referenced_type;
      continue;
    }
    break;
  }
  if (!has_const) {
    return nullptr;
  }
  switch (c99::kind(*inner)) {
    case TK::Char:
    case TK::SChar:
    case TK::UChar:
    case TK::Int:
    case TK::UInt:
    case TK::Long:
    case TK::ULong:
    case TK::LongLong:
    case TK::ULongLong:
    case TK::Float:
    case TK::Double:
    case TK::Pointer:
    case TK::BitInt:
      return inner;
    default:
      return nullptr;
  }
}

std::map<std::string, ir::Constant, std::less<>> build_cache(const SymbolTable& symbols) {
  std::map<std::string, ir::Constant, std::less<>> cache;
  for (const auto& [name, sym] : symbols) {
    const auto* attrs = std::get_if<StaticAttr>(&sym.attrs);
    if (attrs == nullptr) {
      continue;
    }
    const auto* initial = std::get_if<Initial>(&attrs->initial_value);
    if (initial == nullptr) {
      continue;
    }
    const ValueTree& v = initial->value;
    if (v.kind != ValueTree::Kind::kInt) {
      continue;
    }
    const c99::Type_data_type* t = scalar_type(sym.type);
    if (t == nullptr) {
      continue;
    }
    ir::Type_const c;
    if (asdl::isa<c99::Float>(*t)) {
      c = ir::ConstFloat{v.integer};
    } else if (asdl::isa<c99::Double>(*t)) {
      c = ir::ConstDouble{v.integer};
    } else {
      c = c99_to_tac::tac_const_for(*t, v.integer);
    }
    cache.insert_or_assign(name, ir::Constant{.const_ = std::move(c)});
  }
  return cache;
}

}  // namespace

tac_ast::Function fold_static_const_reads(tac_ast::Function fn,
                                          const framework::PassContext& ctx) {
  if (ctx.symbols == nullptr) {
    return fn;
  }
  const auto cache = build_cache(*ctx.symbols);
  return framework::run_operand_rewrite(
      std::move(fn), [&](const ir::Type_val& v) -> std::optional<ir::Type_val> {
        const auto* var = std::get_if<ir::Var>(&v);
        if (var == nullptr) {
          return std::nullopt;
        }
        const auto it = cache.find(var->name);
        if (it == cache.end()) {
          return std::nullopt;
        }
        return it->second;
      });
}

}  // namespace c6502::passes
