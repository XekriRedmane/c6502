/* TAC pass: compute an operation of a Phi of constants in the Phi's
arguments.

`?:` with constant arms joins them in a Phi, and the arithmetic after
the join runs on whichever arrived:

    Phi(%c, [3 from .then, 2 from .else])
    Truncate(%c, %c.lo)
    Binary(+, %c.lo, 1uc, B)

C's `B = (flag ? 3 : 2) + 1;` lowered that way loads 3 or 2 and then
adds 1. Every argument is a constant, so the operation's result on each
path is a constant too, and the Phi can join those instead:

    Phi(%c.lo, [3uc from .then, 2uc from .else])
    Binary(+, %c.lo, 1uc, B)

and then

    Phi(%c.lo.p0, [4uc from .then, 3uc from .else])
    Copy(%c.lo.p0, B)

which `tac_to_asm` lowers as a load of 4 or 3 and a store.

The rule. A `Phi` whose arguments are all integer constants, and whose
destination is an SSA name read by exactly one instruction `U`, takes
`U`'s operation when `U` is a `Unary`, a `Binary` whose other operand
is a constant, or a `Truncate`, and constant folding
(`constant_folding::fold`) folds `U` for every argument. The Phi's
destination becomes `U`'s when that is an SSA name, and `U` goes.
Otherwise the Phi gets a new temporary of the type of `U`'s
destination, and `U` becomes a copy of it into the destination, which
keeps the store where it was.

`U` reads the Phi's destination, so the Phi's block dominates `U`, and
every reader of `U`'s destination comes after `U`, so the Phi still
defines that value before every read. The Phi computes the value on
every path into its block, where `U` may have run on fewer, but the
value is a constant on each path and computing it has no effect, so
that is harmless. An operation that doesn't fold for some argument,
such as a divide by zero, leaves the Phi alone.

An extend isn't among the operations taken. It would only widen the
Phi, and a Phi of a byte's constants is what other rules recognize,
such as the shift of a 0 or 1 through the carry (`tac_to_asm`'s
`rotate_in`). */
#include "passes/optimization/phi_constant_fold.h"

#include <string>
#include <utility>
#include <vector>

#include "passes/optimization/constant_folding.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::PassContext;
using K = ir::Kind_instruction;

bool int_const(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  return c != nullptr && constant_folding::is_integer_const(c->const_);
}

// Returns the destination of a `Unary`, `Binary`, or `Truncate`, the
// operations a Phi takes, or null for any other instruction.
const ir::Type_val* foldable_dst(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Unary:
      return &std::get<ir::Unary>(instr).dst;
    case K::Binary:
      return &std::get<ir::Binary>(instr).dst;
    case K::Truncate:
      return &std::get<ir::Truncate>(instr).dst;
    default:
      return nullptr;
  }
}

// Returns a new SSA local of `like`'s type without its qualifiers,
// named `<like>.p<k>` for the first k no symbol has.
ir::Var make_temp(const PassContext& ctx, const std::string& like) {
  int k = 0;
  while (ctx.symbols->contains(like + ".p" + std::to_string(k))) {
    ++k;
  }
  const std::string name = like + ".p" + std::to_string(k);
  const c99::Type_data_type* t = &ctx.symbols->at(like).type;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(t)) {
      t = &*v->referenced_type;
    } else {
      break;
    }
  }
  ctx.symbols->insert_or_assign(name, Symbol{.type = *t, .attrs = LocalAttr{}});
  ctx.ssa_dsts->insert(name);
  return ir::Var{.name = name};
}

}  // namespace

tac_ast::Function fold_into_constant_phis(tac_ast::Function fn, const PassContext& ctx) {
  if (ctx.symbols == nullptr || ctx.ssa_dsts == nullptr) {
    return fn;
  }
  const auto& instrs = fn.instructions;
  const auto counts = count_uses(instrs);
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto* phi = std::get_if<ir::Phi>(&instrs[i]);
    if (phi == nullptr || phi->args.empty()) {
      continue;
    }
    const auto* phi_dst = std::get_if<ir::Var>(&phi->dst);
    if (phi_dst == nullptr || !ctx.is_ssa(phi_dst->name)) {
      continue;
    }
    const std::size_t* count = counts.find(phi_dst->name);
    if (count == nullptr || *count != 1) {
      continue;
    }
    bool all_constant = true;
    for (const auto& a : phi->args) {
      all_constant = all_constant && int_const(std::get<ir::PhiArg>(a).source);
    }
    if (!all_constant) {
      continue;
    }
    // The one instruction that reads the Phi's destination.
    std::optional<std::size_t> j;
    for (std::size_t k = 0; k < instrs.size() && !j; ++k) {
      for (const std::string_view u : uses_in(instrs[k])) {
        if (u == phi_dst->name) {
          j = k;
          break;
        }
      }
    }
    if (!j) {
      continue;
    }
    const ir::Type_instruction& user = instrs[*j];
    const ir::Type_val* user_dst_val = foldable_dst(user);
    const auto* user_dst = user_dst_val != nullptr ? std::get_if<ir::Var>(user_dst_val) : nullptr;
    if (user_dst == nullptr) {
      continue;
    }
    if (const auto* b = std::get_if<ir::Binary>(&user)) {
      if (!int_const(b->src1) && !int_const(b->src2)) {
        continue;
      }
    }
    std::vector<ir::Type_phi_arg> args;
    bool folded_all = true;
    for (const auto& a : phi->args) {
      const ValMapper mapper =
          [&](const ir::Type_val& v) -> std::optional<ir::Type_val> {
        if (v == phi->dst) {
          return std::get<ir::PhiArg>(a).source;
        }
        return std::nullopt;
      };
      const std::optional<ir::Type_instruction> rewritten = rewrite_uses_in(user, mapper);
      const constant_folding::FoldResult folded =
          constant_folding::fold(rewritten ? *rewritten : user, ctx.symbols);
      const ir::Copy* copy = folded.outcome == constant_folding::FoldOutcome::kReplaced
                                 ? std::get_if<ir::Copy>(&*folded.instr)
                                 : nullptr;
      if (copy == nullptr || !int_const(copy->src)) {
        folded_all = false;
        break;
      }
      args.push_back(ir::PhiArg{.pred_label = std::get<ir::PhiArg>(a).pred_label, .source = copy->src});
    }
    if (!folded_all) {
      continue;
    }
    framework::Instrs out(instrs.begin(), instrs.end());
    if (ctx.is_ssa(user_dst->name)) {
      out[i] = ir::Phi{.dst = *user_dst, .args = std::move(args)};
      out.erase(out.begin() + static_cast<std::ptrdiff_t>(*j));
    } else {
      const ir::Var tmp = make_temp(ctx, user_dst->name);
      out[i] = ir::Phi{.dst = tmp, .args = std::move(args)};
      out[*j] = ir::Copy{.src = tmp, .dst = *user_dst};
    }
    return framework::with_instructions(fn, std::move(out));
  }
  return fn;
}

}  // namespace c6502::passes
