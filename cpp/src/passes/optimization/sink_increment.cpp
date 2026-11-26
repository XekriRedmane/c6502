#include "passes/optimization/sink_increment.h"

#include <set>
#include <string>
#include <string_view>

#include "bit_precise.h"
#include "passes/optimization/narrow_widened_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using asdl::Int;
using framework::Instrs;
using framework::PassContext;
using IK = ir::Kind_instruction;

bool is_block_terminator(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case IK::Label:
    case IK::Jump:
    case IK::JumpIfTrue:
    case IK::JumpIfFalse:
    case IK::JumpIfCmp:
    case IK::JumpIfMasked:
    case IK::Ret:
    case IK::FunctionCall:
    case IK::IndirectCall:
      return true;
    default:
      return false;
  }
}

bool is_store(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case IK::Store:
    case IK::IndexedStore:
    case IK::IndexedSymbolStore:
    case IK::IndirectIndexedStore:
      return true;
    default:
      return false;
  }
}

struct Step {
  const ir::Binary* binary;
  const ir::Var* x;
  const ir::Var* y;
};

std::optional<Step> as_step(const ir::Type_instruction& instr) {
  const auto* b = std::get_if<ir::Binary>(&instr);
  if (b == nullptr ||
      !(std::holds_alternative<ir::Add>(b->op) || std::holds_alternative<ir::Subtract>(b->op))) {
    return std::nullopt;
  }
  const auto* x = std::get_if<ir::Var>(&b->src1);
  const auto* y = std::get_if<ir::Var>(&b->dst);
  if (x == nullptr || y == nullptr || !std::holds_alternative<ir::Constant>(b->src2)) {
    return std::nullopt;
  }
  return Step{b, x, y};
}

bool is_sink_candidate(const ir::Type_instruction& instr) {
  const auto s = as_step(instr);
  return s && s->x->name != s->y->name;
}

std::optional<std::size_t> find_sink_offset(const Instrs& instrs, std::size_t start,
                                            std::string_view x_name, std::string_view y_name,
                                            bool stop_at_stores) {
  std::optional<std::size_t> last_x_use;
  for (std::size_t j = start + 1; j < instrs.size(); ++j) {
    const auto& instr = instrs[j];
    if (is_block_terminator(instr)) {
      break;
    }
    bool uses_x = false;
    bool touches_y = false;
    for (const auto u : uses_in(instr)) {
      uses_x = uses_x || u == x_name;
      touches_y = touches_y || u == y_name;
    }
    bool defines_x = false;
    for (const auto d : defs_in(instr)) {
      touches_y = touches_y || d == y_name;
      defines_x = defines_x || d == x_name;
    }
    if (touches_y) {
      break;
    }
    if (defines_x) {
      break;
    }
    if (stop_at_stores && is_store(instr)) {
      break;
    }
    if (uses_x && is_sink_candidate(instr)) {
      break;
    }
    if (uses_x) {
      last_x_use = j - start;
    }
  }
  return last_x_use;
}

ir::Function test_the_step(ir::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr || ctx.symbols == nullptr) {
    return fn;
  }
  Instrs instrs = fn.instructions;
  std::set<std::string, std::less<>> phi_uses;
  for (const auto& ins : instrs) {
    if (std::holds_alternative<ir::Phi>(ins)) {
      for (const auto u : uses_in(ins)) {
        phi_uses.insert(std::string(u));
      }
    }
  }
  const auto reads = [](const ir::Type_instruction& ins, std::string_view name) {
    for (const auto u : uses_in(ins)) {
      if (u == name) {
        return true;
      }
    }
    return false;
  };
  const auto is_x = [](const ir::Type_val& v, const ir::Var& x) {
    const auto* var = std::get_if<ir::Var>(&v);
    return var != nullptr && var->name == x.name;
  };
  bool changed = false;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto step = as_step(instrs[i]);
    if (!step || !ctx.ssa_dsts->contains(step->x->name) ||
        !ctx.ssa_dsts->contains(step->y->name) || step->x->name == step->y->name) {
      continue;
    }
    const auto c = bit_precise::raw_value(std::get<ir::Constant>(step->binary->src2).const_);
    if (!c) {
      continue;
    }
    const ir::Var x = *step->x;
    const ir::Var y = *step->y;
    const bool subtract = std::holds_alternative<ir::Subtract>(step->binary->op);
    const auto width = narrow_widened_arith::byte_width(y, ctx.symbols);
    if (!width || width != narrow_widened_arith::byte_width(x, ctx.symbols)) {
      continue;
    }
    const auto is_unsigned = narrow_widened_arith::is_unsigned(y, ctx.symbols);
    if (!is_unsigned || phi_uses.contains(x.name)) {
      continue;
    }
    std::size_t j = i + 1;
    while (j < instrs.size()) {
      const IK k = ir::kind(instrs[j]);
      if (k == IK::Label || k == IK::Jump || k == IK::JumpIfTrue || k == IK::JumpIfFalse ||
          k == IK::JumpIfCmp || k == IK::JumpIfMasked || k == IK::Ret) {
        break;
      }
      if (reads(instrs[j], x.name) || reads(instrs[j], y.name)) {
        break;
      }
      ++j;
    }
    if (j >= instrs.size()) {
      continue;
    }
    const auto& jump = instrs[j];
    Int k = 0;
    ir::Type_binary_operator op;
    std::string target;
    if (const auto* jt = std::get_if<ir::JumpIfTrue>(&jump)) {
      if (!is_x(jt->condition, x)) {
        continue;
      }
      op = ir::NotEqual{};
      target = jt->target;
    } else if (const auto* jf = std::get_if<ir::JumpIfFalse>(&jump)) {
      if (!is_x(jf->condition, x)) {
        continue;
      }
      op = ir::Equal{};
      target = jf->target;
    } else if (const auto* jc = std::get_if<ir::JumpIfCmp>(&jump);
               jc != nullptr && (std::holds_alternative<ir::Equal>(jc->op) ||
                                 std::holds_alternative<ir::NotEqual>(jc->op))) {
      std::optional<Int> kv;
      if (is_x(jc->src1, x) && std::holds_alternative<ir::Constant>(jc->src2)) {
        kv = bit_precise::raw_value(std::get<ir::Constant>(jc->src2).const_);
      } else if (is_x(jc->src2, x) && std::holds_alternative<ir::Constant>(jc->src1)) {
        kv = bit_precise::raw_value(std::get<ir::Constant>(jc->src1).const_);
      } else {
        continue;
      }
      if (!kv) {
        continue;
      }
      k = *kv;
      op = jc->op;
      target = jc->target;
    } else {
      continue;
    }
    bool read_later = false;
    for (std::size_t r = j + 1; r < instrs.size() && !read_later; ++r) {
      read_later = reads(instrs[r], x.name);
    }
    if (read_later) {
      continue;
    }
    const Int moved = subtract ? k - *c : k + *c;
    const auto narrowed = narrow_widened_arith::narrow_constant(
        ir::Constant{.const_ = ir::ConstInt{.value = moved}}, *width, *is_unsigned);
    if (!narrowed) {
      continue;
    }
    instrs[j] = ir::JumpIfCmp{.op = std::move(op), .src1 = y, .src2 = *narrowed, .target = target};
    changed = true;
  }
  if (!changed) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(instrs));
}

}  // namespace

ir::Function sink_increments(ir::Function fn, const PassContext& ctx) {
  ir::Function sunk = framework::run_sink(
      std::move(fn), [&](const Instrs& instrs, std::size_t i) -> std::optional<std::size_t> {
        const auto step = as_step(instrs[i]);
        if (!step || step->x->name == step->y->name) {
          return std::nullopt;
        }
        if (ctx.ssa_dsts != nullptr && !ctx.ssa_dsts->contains(step->x->name)) {
          return std::nullopt;
        }
        const auto offset = find_sink_offset(instrs, i, step->x->name, step->y->name,
                                             ctx.ssa_dsts == nullptr);
        if (!offset) {
          return std::nullopt;
        }
        return i + *offset;
      });
  return test_the_step(std::move(sunk), ctx);
}

}  // namespace c6502::passes
