#include "passes/optimization/read_stored_global.h"

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::PassContext;
using K = ir::Kind_instruction;

bool ends_scan(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case K::Label:
    case K::Jump:
    case K::JumpIfTrue:
    case K::JumpIfFalse:
    case K::JumpIfCmp:
    case K::JumpIfMasked:
    case K::Ret:
    case K::FunctionCall:
    case K::IndirectCall:
    case K::Store:
    case K::IndexedStore:
    case K::IndirectIndexedStore:
      return true;
    default:
      return false;
  }
}

bool is_volatile(const c99::Type_data_type* t) {
  while (const auto* c = std::get_if<c99::Const>(t)) {
    t = &*c->referenced_type;
  }
  return std::holds_alternative<c99::Volatile>(*t);
}

const c99::Type_data_type* strip(const c99::Type_data_type* t) {
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(t)) {
      t = &*v->referenced_type;
    } else {
      return t;
    }
  }
}

bool is_var(const ir::Type_val& v, const std::string& name) {
  const auto* var = std::get_if<ir::Var>(&v);
  return var != nullptr && var->name == name;
}

bool through_pointer(const ir::Type_instruction& ins, const std::string& t) {
  const ir::Type_val* ptr = nullptr;
  std::vector<const ir::Type_val*> others;
  switch (ir::kind(ins)) {
    case K::Load:
      ptr = &std::get<ir::Load>(ins).src_ptr;
      break;
    case K::Store: {
      const auto& s = std::get<ir::Store>(ins);
      ptr = &s.dst_ptr;
      others = {&s.src};
      break;
    }
    case K::IndirectIndexedLoad: {
      const auto& l = std::get<ir::IndirectIndexedLoad>(ins);
      ptr = &l.ptr;
      others = {&l.index};
      break;
    }
    case K::IndirectIndexedStore: {
      const auto& s = std::get<ir::IndirectIndexedStore>(ins);
      ptr = &s.ptr;
      others = {&s.index, &s.src};
      break;
    }
    case K::Phi:
    case K::ZeroExtend:
    case K::SignExtend:
    case K::Truncate:
      return false;
    default:
      return true;
  }
  if (!is_var(*ptr, t)) {
    return false;
  }
  for (const ir::Type_val* v : others) {
    if (is_var(*v, t)) {
      return false;
    }
  }
  return true;
}

bool names(const std::vector<std::string_view>& vs, const std::string& name) {
  for (const std::string_view v : vs) {
    if (v == name) {
      return true;
    }
  }
  return false;
}

}  // namespace

tac_ast::Function read_stored_globals(tac_ast::Function fn, const PassContext& ctx) {
  if (ctx.symbols == nullptr || ctx.ssa_dsts == nullptr) {
    return fn;
  }
  auto& instrs = fn.instructions;
  std::map<std::string, long long, std::less<>> reads;
  for (const auto& ins : instrs) {
    for (const std::string_view u : uses_in(ins)) {
      ++reads[std::string(u)];
    }
  }
  const auto read_count = [&](const std::string& name) {
    const auto it = reads.find(name);
    return it == reads.end() ? 0LL : it->second;
  };
  for (std::size_t i = 1; i < instrs.size(); ++i) {
    const auto* copy = std::get_if<ir::Copy>(&instrs[i]);
    if (copy == nullptr) {
      continue;
    }
    const auto* src = std::get_if<ir::Var>(&copy->src);
    const auto* dst = std::get_if<ir::Var>(&copy->dst);
    if (src == nullptr || dst == nullptr) {
      continue;
    }
    const std::string t = src->name;
    const std::string g = dst->name;
    const ir::Var g_var = *dst;
    if (!ctx.is_ssa(t) || ctx.is_ssa(g) || read_count(t) != 2) {
      continue;
    }
    if (!std::holds_alternative<ir::Binary>(instrs[i - 1]) &&
        !std::holds_alternative<ir::Unary>(instrs[i - 1])) {
      continue;
    }
    if (!names(defs_in(instrs[i - 1]), t)) {
      continue;
    }
    const Symbol* t_sym = ctx.symbols->find(t);
    const Symbol* g_sym = ctx.symbols->find(g);
    if (t_sym == nullptr || g_sym == nullptr || is_volatile(&g_sym->type)) {
      continue;
    }
    const c99::Type_data_type* tt = strip(&t_sym->type);
    const c99::Type_data_type* gt = strip(&g_sym->type);
    if (*tt != *gt &&
        !(std::holds_alternative<c99::Pointer>(*tt) && std::holds_alternative<c99::Pointer>(*gt))) {
      continue;
    }
    for (std::size_t j = i + 1; j < instrs.size(); ++j) {
      const ir::Type_instruction& ins = instrs[j];
      if (names(uses_in(ins), t)) {
        if (!through_pointer(ins, t)) {
          break;
        }
        const ValMapper mapper = [&](const ir::Type_val& v) -> std::optional<ir::Type_val> {
          if (is_var(v, t)) {
            return ir::Type_val{g_var};
          }
          return std::nullopt;
        };
        if (auto rewritten = rewrite_uses_in(ins, mapper)) {
          instrs[j] = std::move(*rewritten);
        }
        --reads[t];
        break;
      }
      if (ends_scan(ins)) {
        break;
      }
      if (const auto* s = std::get_if<ir::IndexedSymbolStore>(&ins); s != nullptr && s->name == g) {
        break;
      }
      if (names(defs_in(ins), g)) {
        break;
      }
    }
  }
  return fn;
}

}  // namespace c6502::passes
