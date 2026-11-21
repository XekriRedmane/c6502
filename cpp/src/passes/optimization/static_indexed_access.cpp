/* TAC pass: a byte-indexed access to a static object, with a constant
offset, as `IndexedLoad` or `IndexedSymbolStore`.

# What it does

`prompts[which].answer` on `struct prompt prompts[3]` (records of 16
bytes, with `answer` at byte 15) lowers to

    GetAddress(prompts, %g)
    Binary(LeftShift, which, 4, %b)       ; the index scaled to bytes
    ZeroExtend(%b, %z)
    Binary(Add, %g, %z, %a)
    Binary(Add, %a, 15, %p)
    Load(%p, %key)

which, through a pointer, becomes `LDY #15; LDA (ptr),Y` with the
pointer built in two zero-page bytes first. This pass turns the access
into

    IndexedLoad(prompts, %b, %key, offset=15)

which `tac_to_asm` lowers as `LDX b; LDA prompts+15,X`. The same goes
for a `Store` (as `IndexedSymbolStore`), and for an
`IndirectIndexedLoad` or `IndirectIndexedStore` whose index is a
constant.

The rewrite is exact. `abs,X` adds the index to the full 16-bit address
`prompts+15`, so it reads `prompts + 15 + b` as the `Load` did, with no
bound on the object's size.

A pointer that's passed on and not read through,
`cout_string(&prompts[which].answer, 1)`, is `(prompts + z) + 15`. The
pass reassociates that to `(prompts + 15) + z`, so that the constant
part is one link-time address, `#<(prompts+15)`
(`passes/label_offset_fold.py` folds the add of a label's halves and a
constant), and the value takes one 16-bit add.

# Pattern

  * The address is `GetAddress(o) + ZeroExtend(b)`, optionally plus a
    constant, with `o` a static object and `b` one byte. Each
    intermediate is an SSA value.
  * Any `Load`, `Store`, `IndirectIndexedLoad`, or `IndirectIndexedStore`
    through it is rewritten. The address computation is left for
    dead-code elimination when nothing else uses it.

# Where it runs

Once, after the fixed-point loop and before `recognize_indirect_indexed`.

The new temporary of a reassociation, `<dst>.base` (or `<dst>.base<n>`
for the first `n` free), takes the destination's type and attributes,
which Python shares with the destination's symbol and C++ copies. */
#include "passes/optimization/static_indexed_access.h"

#include <map>
#include <string>
#include <vector>

#include "bit_precise.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;
using framework::Instrs;
using framework::PassContext;

// Returns the constant `v`'s value modulo 2^16 when it is a constant of
// 1, 2, or 4 bytes of one of the C types, or none.
std::optional<Int> const16(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c == nullptr) {
    return std::nullopt;
  }
  using CK = ir::Kind_const;
  switch (ir::kind(c->const_)) {
    case CK::ConstChar:
    case CK::ConstUChar:
    case CK::ConstInt:
    case CK::ConstUInt:
    case CK::ConstLong:
    case CK::ConstULong:
      return asdl::floor_mod(*bit_precise::raw_value(c->const_), 0x10000);
    default:
      return std::nullopt;
  }
}

// Returns whether `t`, without its qualifiers, is a 1-byte integer type.
bool byte_type(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(p)) {
      p = &*v->referenced_type;
    } else {
      break;
    }
  }
  return std::holds_alternative<c99::Char>(*p) || std::holds_alternative<c99::SChar>(*p) ||
         std::holds_alternative<c99::UChar>(*p);
}

// `GetAddress(o) + ZeroExtend(b)`: the static object, the byte index,
// and the Add's two operands.
struct StaticBase {
  std::string obj;
  ir::Type_val index;
  ir::Type_val g;
  ir::Type_val z;
};

// The object, the byte index, and the offset of an address.
struct Address {
  std::string obj;
  ir::Type_val index;
  Int offset = 0;
};

class Walker {
 public:
  Walker(const ir::Function& fn, const PassContext& ctx) : ctx_(ctx), symbols_(*ctx.symbols) {
    for (const auto& ins : fn.instructions) {
      if (const auto name = framework::dst_name(ins); name && ctx.ssa_dsts->contains(*name)) {
        defs_.insert_or_assign(std::string(*name), &ins);
      }
    }
  }

  // Returns the instruction that defines the SSA name `v`, or null.
  const ir::Type_instruction* def_of(const ir::Type_val& v) const {
    const auto* var = std::get_if<ir::Var>(&v);
    if (var == nullptr || !ctx_.ssa_dsts->contains(var->name)) {
      return nullptr;
    }
    const auto it = defs_.find(var->name);
    return it == defs_.end() ? nullptr : it->second;
  }

  bool one_byte(const ir::Type_val& v) const {
    if (const auto* c = std::get_if<ir::Constant>(&v)) {
      return std::holds_alternative<ir::ConstChar>(c->const_) ||
             std::holds_alternative<ir::ConstUChar>(c->const_);
    }
    const Symbol* sym = symbols_.find(std::get<ir::Var>(v).name);
    return sym != nullptr && byte_type(sym->type);
  }

  // Returns `(object, byte index, g, z)` when `v` is `GetAddress(o) +
  // ZeroExtend(b)` for a static `o` and a byte `b`.
  std::optional<StaticBase> static_base(const ir::Type_val& v) const {
    const ir::Type_instruction* d = def_of(v);
    const auto* add = d != nullptr ? std::get_if<ir::Binary>(d) : nullptr;
    if (add == nullptr || !std::holds_alternative<ir::Add>(add->op)) {
      return std::nullopt;
    }
    const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
        {&add->src1, &add->src2}, {&add->src2, &add->src1}};
    for (const auto& [g, z] : orders) {
      const ir::Type_instruction* gd = def_of(*g);
      const ir::Type_instruction* zd = def_of(*z);
      const auto* ga = gd != nullptr ? std::get_if<ir::GetAddress>(gd) : nullptr;
      const auto* ze = zd != nullptr ? std::get_if<ir::ZeroExtend>(zd) : nullptr;
      if (ga == nullptr || !std::holds_alternative<ir::Var>(ga->operand) || ze == nullptr ||
          !one_byte(ze->src)) {
        continue;
      }
      const std::string& obj = std::get<ir::Var>(ga->operand).name;
      const Symbol* sym = symbols_.find(obj);
      if (sym != nullptr && std::holds_alternative<StaticAttr>(sym->attrs)) {
        return StaticBase{obj, ze->src, *g, *z};
      }
    }
    return std::nullopt;
  }

  // Returns `(object, byte index, offset)` for the address `v`.
  std::optional<Address> address(const ir::Type_val& v) const {
    if (auto base = static_base(v)) {
      return Address{std::move(base->obj), std::move(base->index), 0};
    }
    const ir::Type_instruction* d = def_of(v);
    const auto* add = d != nullptr ? std::get_if<ir::Binary>(d) : nullptr;
    if (add != nullptr && std::holds_alternative<ir::Add>(add->op)) {
      const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
          {&add->src1, &add->src2}, {&add->src2, &add->src1}};
      for (const auto& [a, k] : orders) {
        const auto off = const16(*k);
        auto base = static_base(*a);
        if (off && base) {
          return Address{std::move(base->obj), std::move(base->index), *off};
        }
      }
    }
    return std::nullopt;
  }

  // Returns `(g + z) + k` as `(g + k) + z` for `g` the address of a
  // static object, so that `g + k` is one link-time address, or none.
  std::optional<Instrs> reassociate(const ir::Binary& ins) const {
    const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
        {&ins.src1, &ins.src2}, {&ins.src2, &ins.src1}};
    for (const auto& [a, k] : orders) {
      const auto off = const16(*k);
      if (!off || *off == 0) {
        continue;
      }
      const auto base = static_base(*a);
      if (!base) {
        continue;
      }
      const auto* d = std::get_if<ir::Var>(&ins.dst);
      const Symbol* sym = d != nullptr ? ctx_.symbols->find(d->name) : nullptr;
      if (sym == nullptr) {
        return std::nullopt;
      }
      Symbol copy = *sym;
      std::string name = d->name + ".base";
      for (int n = 1; ctx_.symbols->contains(name); ++n) {
        name = d->name + ".base" + std::to_string(n);
      }
      ctx_.symbols->insert_or_assign(name, std::move(copy));
      ctx_.ssa_dsts->insert(name);
      const ir::Var t{.name = name};
      Instrs out;
      out.push_back(ir::Binary{.op = ir::Add{}, .src1 = base->g, .src2 = *k, .dst = t});
      out.push_back(ir::Binary{.op = ir::Add{}, .src1 = t, .src2 = base->z, .dst = ins.dst});
      return out;
    }
    return std::nullopt;
  }

 private:
  const PassContext& ctx_;
  const SymbolTable& symbols_;
  std::map<std::string, const ir::Type_instruction*, std::less<>> defs_;
};

}  // namespace

ir::Function static_indexed_access(ir::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr || ctx.ssa_dsts->empty() || ctx.symbols == nullptr) {
    return fn;
  }
  const Walker w(fn, ctx);
  Instrs out;
  out.reserve(fn.instructions.size());
  bool changed = false;
  using IK = ir::Kind_instruction;
  for (const auto& ins : fn.instructions) {
    std::optional<ir::Type_instruction> repl;
    switch (ir::kind(ins)) {
      case IK::Load: {
        const auto& i = std::get<ir::Load>(ins);
        if (auto hit = w.address(i.src_ptr)) {
          repl = ir::IndexedLoad{.name = hit->obj,
                                 .index = hit->index,
                                 .dst = i.dst,
                                 .is_volatile = i.is_volatile,
                                 .offset = hit->offset};
        }
        break;
      }
      case IK::Store: {
        const auto& i = std::get<ir::Store>(ins);
        if (auto hit = w.address(i.dst_ptr)) {
          repl = ir::IndexedSymbolStore{.name = hit->obj,
                                        .index = hit->index,
                                        .src = i.src,
                                        .is_volatile = i.is_volatile,
                                        .offset = hit->offset};
        }
        break;
      }
      case IK::IndirectIndexedLoad: {
        const auto& i = std::get<ir::IndirectIndexedLoad>(ins);
        const auto j = const16(i.index);
        if (!j) {
          break;
        }
        if (auto hit = w.address(i.ptr)) {
          repl = ir::IndexedLoad{.name = hit->obj,
                                 .index = hit->index,
                                 .dst = i.dst,
                                 .is_volatile = i.is_volatile,
                                 .offset = hit->offset + *j};
        }
        break;
      }
      case IK::IndirectIndexedStore: {
        const auto& i = std::get<ir::IndirectIndexedStore>(ins);
        const auto j = const16(i.index);
        if (!j) {
          break;
        }
        if (auto hit = w.address(i.ptr)) {
          repl = ir::IndexedSymbolStore{.name = hit->obj,
                                        .index = hit->index,
                                        .src = i.src,
                                        .is_volatile = i.is_volatile,
                                        .offset = hit->offset + *j};
        }
        break;
      }
      case IK::Binary: {
        const auto& i = std::get<ir::Binary>(ins);
        if (!std::holds_alternative<ir::Add>(i.op)) {
          break;
        }
        if (auto pair = w.reassociate(i)) {
          for (auto& x : *pair) {
            out.push_back(std::move(x));
          }
          changed = true;
          continue;
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
      out.push_back(ins);
    }
  }
  if (!changed) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(out));
}

}  // namespace c6502::passes
