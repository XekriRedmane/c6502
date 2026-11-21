/* Multiplies a widened byte before adding a constant to it.

    ZeroExtend(n, %x)              # n is one byte
    Binary(Add | Subtract, %x, c, %t)
    Binary(Multiply, %t, k, %m)    # or k * %t

becomes

    ZeroExtend(n, %x)
    Binary(Multiply, %x, k, %m.dc)
    Binary(Add, %m.dc, c * k, %m)  # Subtract for a Subtract

when `%t` has no other use and `c` and `k` are integer constants of one
class. Arithmetic at a fixed width wraps, so `(x ± c) * k` and `x * k ±
c * k` are the same value. The product is then of a value with one
significant byte, which `strength_reduction` and `tac_to_asm` lower in
fewer instructions: its shifts are byte shifts in A, and a product they
can't make from shifts is an 8-by-8 loop where the other is a call of
`mul16`. The add is one add, as the subtract was.

`&MENU_DESC_PTR->options[n - 1]` with 9-byte options, in
examples/ali_baba/input.c, was `n - 1` in 16 bits and then three 16-bit
shifts and an add. After the rewrite `n * 9` is computed from the byte
`n`, and `reassoc_offset_constant` adds the `-9` after the pointer.

It runs in the fixed point before strength reduction, which would
otherwise turn the multiply into shifts first. */
#include "passes/optimization/distribute_constant.h"

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "bit_precise.h"
#include "passes/optimization/extend_add_chain.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::PassContext;
using BK = ir::Kind_binary_operator;
using CK = ir::Kind_const;

// Returns the constant of `v` when it is a Constant of one of the
// classes of 2, 4, or 8 bytes (Python's `_BITS`), or null.
const ir::Type_const* wide_constant(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c == nullptr) {
    return nullptr;
  }
  switch (ir::kind(c->const_)) {
    case CK::ConstInt:
    case CK::ConstUInt:
    case CK::ConstLong:
    case CK::ConstULong:
    case CK::ConstLongLong:
    case CK::ConstULongLong:
      return &c->const_;
    default:
      return nullptr;
  }
}

// Returns whether `x` is an SSA name defined by a `ZeroExtend` of a
// one-byte value.
bool widened_byte(const ir::Type_val& x, const framework::Instrs& instrs,
                  const std::map<std::string, std::size_t, std::less<>>& defs,
                  const PassContext& ctx) {
  const auto* var = std::get_if<ir::Var>(&x);
  if (var == nullptr) {
    return false;
  }
  const auto it = defs.find(var->name);
  if (it == defs.end()) {
    return false;
  }
  const auto* ext = std::get_if<ir::ZeroExtend>(&instrs[it->second]);
  return ext != nullptr && is_1_byte_val(ext->src, ctx.symbols);
}

// Registers `<base>.dc<k>`, for the first k no symbol has, with `base`'s
// type, as an SSA name, and returns the name.
std::string fresh(const PassContext& ctx, const std::string& base) {
  int k = 0;
  while (ctx.symbols->contains(base + ".dc" + std::to_string(k))) {
    ++k;
  }
  std::string name = base + ".dc" + std::to_string(k);
  ctx.symbols->insert_or_assign(name,
                                Symbol{.type = ctx.symbols->at(base).type, .attrs = LocalAttr{}});
  ctx.ssa_dsts->insert(name);
  return name;
}

std::uint64_t magnitude(asdl::Int v) { return static_cast<std::uint64_t>(v < 0 ? -v : v); }

}  // namespace

tac_ast::Function distribute_constant(tac_ast::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr || ctx.ssa_dsts->empty() || ctx.symbols == nullptr) {
    return fn;
  }
  const framework::Instrs& instrs = fn.instructions;
  std::map<std::string, std::size_t, std::less<>> defs;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto d = framework::dst_name(instrs[i]);
    if (d && ctx.is_ssa(*d)) {
      defs.insert_or_assign(std::string(*d), i);
    }
  }
  const auto uses = count_uses(instrs);
  std::map<std::size_t, framework::Instrs> rewrites;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto* ins = std::get_if<ir::Binary>(&instrs[i]);
    if (ins == nullptr || ir::kind(ins->op) != BK::Multiply || !asdl::isa<ir::Var>(ins->dst)) {
      continue;
    }
    const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
        {&ins->src1, &ins->src2}, {&ins->src2, &ins->src1}};
    for (const auto& [t, kv] : orders) {
      const ir::Type_const* k = wide_constant(*kv);
      const auto* tvar = std::get_if<ir::Var>(t);
      if (k == nullptr || tvar == nullptr || !ctx.is_ssa(tvar->name)) {
        continue;
      }
      const std::size_t* n = uses.find(tvar->name);
      const auto def = defs.find(tvar->name);
      if (n == nullptr || *n != 1 || def == defs.end()) {
        continue;
      }
      const auto* inner = std::get_if<ir::Binary>(&instrs[def->second]);
      if (inner == nullptr ||
          (ir::kind(inner->op) != BK::Add && ir::kind(inner->op) != BK::Subtract)) {
        continue;
      }
      const ir::Type_const* c = wide_constant(inner->src2);
      const ir::Type_val* x = &inner->src1;
      if (c == nullptr && ir::kind(inner->op) == BK::Add) {
        c = wide_constant(inner->src1);
        x = &inner->src2;
      }
      if (c == nullptr || c->index() != k->index() || !widened_byte(*x, instrs, defs, ctx)) {
        continue;
      }
      // `x * k - c * k` keeps the subtract, whose top byte `carry_inc`
      // makes `BCS skip; DEC`, where an add of the negated constant
      // would add `#$FF` to it.
      const asdl::Int cv = *bit_precise::raw_value(*c);
      const asdl::Int kv_value = *bit_precise::raw_value(*k);
      bool subtract = ir::kind(inner->op) == BK::Subtract;
      if (cv != 0 && kv_value != 0 && ((cv < 0) != (kv_value < 0))) {
        subtract = !subtract;
      }
      // The magnitude of the product, which can take 128 bits, is
      // reduced modulo 2^64 at most, so it is computed modulo 2^64.
      const std::uint64_t product = magnitude(cv) * magnitude(kv_value);
      const auto layout = *bit_precise::integer_const_layout(*k);
      const asdl::Int wrapped = bit_precise::wrap(asdl::Int(product), 8 * layout.bytes,
                                                  layout.is_signed);
      const ir::Var m{.name = fresh(ctx, std::get<ir::Var>(ins->dst).name)};
      framework::Instrs rep;
      rep.push_back(ir::Binary{.op = ir::Multiply{}, .src1 = *x, .src2 = *kv, .dst = m});
      rep.push_back(ir::Binary{
          .op = subtract ? ir::Type_binary_operator{ir::Subtract{}}
                         : ir::Type_binary_operator{ir::Add{}},
          .src1 = m,
          .src2 = ir::Constant{.const_ = bit_precise::with_value(*k, wrapped)},
          .dst = ins->dst});
      rewrites.insert_or_assign(i, std::move(rep));
      break;
    }
  }
  if (rewrites.empty()) {
    return fn;
  }
  framework::Instrs out;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto it = rewrites.find(i);
    if (it == rewrites.end()) {
      out.push_back(instrs[i]);
    } else {
      for (auto& r : it->second) {
        out.push_back(std::move(r));
      }
    }
  }
  return framework::with_instructions(fn, std::move(out));
}

}  // namespace c6502::passes
