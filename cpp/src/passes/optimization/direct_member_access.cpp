/* TAC pass that turns an access through `&obj + k` into a direct access
to byte `k` of `obj`.

`c99_to_tac` lowers a struct member, and an array element at a constant
index, as pointer arithmetic on the object's address:

    GetAddress(g, %p)
    Binary(Add, %p, Constant(12), %q)
    Store(c, %q)                      # g.command = c

or, after `recognize_indirect_indexed`, as an `IndirectIndexedStore(%p,
Constant(12), c)`. `tac_to_asm` then builds the address in a zero-page
pair and stores through it with `STA (ptr),Y`. This pass rewrites the
access to `IndexedSymbolStore(g, Constant(12), c)` (or `IndexedLoad` for
a read), which `tac_to_asm` lowers with a constant index to plain `STA
g+12` for an object with static storage, or to the local's own byte for
an automatic one. The `GetAddress` and the `Add` are left for dead-store
elimination, which drops them when nothing else uses the address.

When every access to a local struct or array goes this way, nothing
takes its address any more, so the asm register allocator can give its
bytes zero-page slots in place of a soft-stack frame.

The rewritten forms are these.

    Load(&obj + k, dst)                    -> IndexedLoad(obj, k, dst)
    Store(src, &obj + k)                   -> IndexedSymbolStore(obj, k, src)
    IndirectIndexedLoad(&obj + k, j, dst)  -> IndexedLoad(obj, k + j, dst)
    IndirectIndexedStore(&obj + k, j, src) -> IndexedSymbolStore(obj, k + j, src)

Here `&obj + k` is an SSA pointer defined by `GetAddress(obj)`, possibly
followed by one `Binary(Add, _, Constant(k))`, and `j` is a Constant.
The volatile bit carries over.

The conditions are these.

  * `obj` has static storage (any type), or it's an automatic struct,
    union, or array that isn't a parameter. An automatic scalar is left
    alone, since once its address isn't taken, SSA construction could
    rename it, and the rewritten access names the original. Aggregates
    are never renamed.
  * `0 <= k + j <= 255`, since the index is one byte. An access outside
    the object through `&obj + k` is undefined behavior in C, so no
    other bound is needed.
  * The value loaded or stored is a scalar. Copying a whole struct
    member that is itself a struct still goes through the pointer.

The pointers and their offsets are found in one walk over the
instructions in order, so an Add that comes before the definition of
the pointer it adds to (as one in a loop can) isn't followed. */
#include "passes/optimization/direct_member_access.h"

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "bit_precise.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;
using framework::PassContext;

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

bool is_aggregate(const c99::Type_data_type& t) {
  return std::holds_alternative<c99::Structure>(t) || std::holds_alternative<c99::Union>(t) ||
         std::holds_alternative<c99::Array>(t);
}

// Returns the value of an integer constant of any width (Python's
// `_INT_CONSTS`, which takes the odd widths of the bit-precise types
// too), or none.
std::optional<Int> const_value(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c == nullptr) {
    return std::nullopt;
  }
  return bit_precise::raw_value(c->const_);
}

// The object an SSA pointer points into, and the byte offset.
struct Base {
  std::string obj;
  Int offset = 0;
};

}  // namespace

ir::Function direct_member_access(ir::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr || ctx.ssa_dsts->empty() || ctx.symbols == nullptr) {
    return fn;
  }
  const SymbolTable& symbols = *ctx.symbols;
  const std::set<std::string, std::less<>> params(fn.params.begin(), fn.params.end());

  const auto eligible_object = [&](const std::string& name) {
    const Symbol* sym = symbols.find(name);
    if (sym == nullptr || std::holds_alternative<c99::FunType>(sym->type)) {
      return false;
    }
    if (std::holds_alternative<StaticAttr>(sym->attrs)) {
      return true;
    }
    return !params.contains(name) && is_aggregate(strip(sym->type));
  };

  const auto is_scalar = [&](const ir::Type_val& v) {
    if (std::holds_alternative<ir::Constant>(v)) {
      return true;
    }
    const Symbol* sym = symbols.find(std::get<ir::Var>(v).name);
    return sym != nullptr && !is_aggregate(strip(sym->type));
  };

  // Each SSA pointer's object and byte offset.
  std::map<std::string, Base, std::less<>> base;
  std::vector<const ir::Binary*> adds;
  for (const auto& instr : fn.instructions) {
    if (const auto* ga = std::get_if<ir::GetAddress>(&instr)) {
      const auto* operand = std::get_if<ir::Var>(&ga->operand);
      const auto* dst = std::get_if<ir::Var>(&ga->dst);
      if (operand != nullptr && dst != nullptr && ctx.ssa_dsts->contains(dst->name) &&
          eligible_object(operand->name)) {
        base.insert_or_assign(dst->name, Base{operand->name, 0});
      }
    } else if (const auto* b = std::get_if<ir::Binary>(&instr)) {
      const auto* dst = std::get_if<ir::Var>(&b->dst);
      if (std::holds_alternative<ir::Add>(b->op) && dst != nullptr &&
          ctx.ssa_dsts->contains(dst->name)) {
        adds.push_back(b);
      }
    }
  }
  for (const ir::Binary* add : adds) {
    const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
        {&add->src1, &add->src2}, {&add->src2, &add->src1}};
    for (const auto& [ptr, off] : orders) {
      const auto k = const_value(*off);
      const auto* p = std::get_if<ir::Var>(ptr);
      if (!k || p == nullptr) {
        continue;
      }
      const auto it = base.find(p->name);
      if (it == base.end()) {
        continue;
      }
      Base b{it->second.obj, it->second.offset + *k};
      base.insert_or_assign(std::get<ir::Var>(add->dst).name, std::move(b));
      break;
    }
  }

  // Returns the object and the constant index of an access through
  // `ptr` with `extra` more bytes of offset, or none.
  const auto target = [&](const ir::Type_val& ptr,
                          Int extra) -> std::optional<std::pair<std::string, ir::Type_val>> {
    const auto* p = std::get_if<ir::Var>(&ptr);
    if (p == nullptr) {
      return std::nullopt;
    }
    const auto it = base.find(p->name);
    if (it == base.end()) {
      return std::nullopt;
    }
    const Int k = it->second.offset + extra;
    if (!(0 <= k && k <= 255)) {
      return std::nullopt;
    }
    return std::pair{it->second.obj, ir::Type_val{ir::Constant{.const_ = ir::ConstUChar{.value = k}}}};
  };

  bool changed = false;
  framework::Instrs out;
  out.reserve(fn.instructions.size());
  for (const auto& instr : fn.instructions) {
    std::optional<ir::Type_instruction> repl;
    switch (ir::kind(instr)) {
      case ir::Kind_instruction::Load: {
        const auto& i = std::get<ir::Load>(instr);
        const auto t = target(i.src_ptr, 0);
        if (t && is_scalar(i.dst)) {
          repl = ir::IndexedLoad{
              .name = t->first, .index = t->second, .dst = i.dst, .is_volatile = i.is_volatile};
        }
        break;
      }
      case ir::Kind_instruction::Store: {
        const auto& i = std::get<ir::Store>(instr);
        const auto t = target(i.dst_ptr, 0);
        if (t && is_scalar(i.src)) {
          repl = ir::IndexedSymbolStore{
              .name = t->first, .index = t->second, .src = i.src, .is_volatile = i.is_volatile};
        }
        break;
      }
      case ir::Kind_instruction::IndirectIndexedLoad: {
        const auto& i = std::get<ir::IndirectIndexedLoad>(instr);
        const auto j = const_value(i.index);
        const auto t = j ? target(i.ptr, *j) : std::nullopt;
        if (t && is_scalar(i.dst)) {
          repl = ir::IndexedLoad{
              .name = t->first, .index = t->second, .dst = i.dst, .is_volatile = i.is_volatile};
        }
        break;
      }
      case ir::Kind_instruction::IndirectIndexedStore: {
        const auto& i = std::get<ir::IndirectIndexedStore>(instr);
        const auto j = const_value(i.index);
        const auto t = j ? target(i.ptr, *j) : std::nullopt;
        if (t && is_scalar(i.src)) {
          repl = ir::IndexedSymbolStore{
              .name = t->first, .index = t->second, .src = i.src, .is_volatile = i.is_volatile};
        }
        break;
      }
      default:
        break;
    }
    if (repl) {
      out.push_back(std::move(*repl));
      changed = true;
    } else {
      out.push_back(instr);
    }
  }
  if (!changed) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(out));
}

}  // namespace c6502::passes
