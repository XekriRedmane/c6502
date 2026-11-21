/* Reads a global just stored from a temporary in place of the temporary.

What it does. `*++P` on a pointer `P` that isn't an SSA name (a global,
here one in zero page) is

    Binary(Add, P, 1, %0.1)
    Copy(%0.1, P)
    Load(%0.1, %1.1)

`%0.1` is read twice, so copy folding can't make the add write `P`
directly, and `tac_to_asm` adds into two zero-page bytes, copies them
to `P`, and loads through the copy, 14 bytes more than `P++; *P`. This
pass makes the load read `P`:

    Binary(Add, P, 1, %0.1)
    Copy(%0.1, P)
    Load(P, %1.1)

after which copy folding gives `Binary(Add, P, 1, P)`, which the INC
peephole turns into `INC P; BNE; INC P+1`, and the load is `LDA
(P),Y`.

When it applies.

- `Copy(T, G)` comes right after the `Binary` or `Unary` that defines
  `T`, an SSA name, and `G` is a variable of the same type (or both are
  pointers) that isn't an SSA name and isn't volatile. A `T` that is a
  `ZeroExtend` of a byte is left alone, since `tac_to_asm` lowers it
  with its high byte as 0 and has no such fact about `G`. Zork's `main`
  grew by 2 bytes when `disk_sector`, stored from such a `T`, was read
  in its place.
- `T` has exactly one read besides the `Copy`, in an instruction `U`
  later in the same block, with no label or jump between. `U` isn't a
  `Phi`, an extend, or a `Truncate`, and when it loads or stores
  through a pointer, `T` is that pointer and nothing else. A narrowing
  pass looks through an extend to its source, and a byte comparison is
  narrowed through a `ZeroExtend` of a global only when the global is
  unchanged up to the comparison. In Ali Baba's `handle_event_slot` a
  call came between the two.
- No instruction between the `Copy` and `U` writes `G` by name, stores
  through a pointer or at a fixed address, stores into `G` as an object
  (`IndexedSymbolStore`), or calls a function.

`G` then holds the value of `T` at `U`, so `U` reads the same value.
`T` is left with the `Copy` as its only reader, and copy folding, later
in the same sweep, removes it. A read of `G` in place of `T` costs at
most a byte more (an absolute address in place of a zero-page one), and
the store of `T` that goes with it saves two or more. */
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

// Returns whether the scan for `U` stops at `ins`: a label, a jump, a
// `Ret`, a call, or a store through a pointer or at a fixed address.
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

// Returns whether the type `t` is volatile, looking through `const`
// qualifiers to the first other one.
bool is_volatile(const c99::Type_data_type* t) {
  while (const auto* c = std::get_if<c99::Const>(t)) {
    t = &*c->referenced_type;
  }
  return std::holds_alternative<c99::Volatile>(*t);
}

// Returns `t` without its qualifiers.
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

// Returns whether `ins` reads `t` only as the pointer it loads or stores
// through. Any other instruction but a `Phi`, an extend, and a
// `Truncate` passes.
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
