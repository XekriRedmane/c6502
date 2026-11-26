#include "passes/transfer_pm1_store.h"

#include <cstddef>
#include <iterator>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using Instructions = std::vector<ir::Type_instruction>;

bool is_reg(const ir::Type_operand& op, bool (*test)(const ir::Type_reg&)) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && test(r->reg);
}
bool reg_is_a(const ir::Type_reg& r) { return isa<ir::A>(r); }
bool reg_is_index(const ir::Type_reg& r) { return isa<ir::X, ir::Y>(r); }

// INX, DEX, INY, and DEY set N and Z from the result and leave C and V
// alone, so the rewrites check C and V for liveness and not N and Z.
ir::Type_instruction step_of(bool increment, const ir::Type_operand& reg) {
  if (increment) {
    return ir::Inc{.dst = reg};
  }
  return ir::Dec{.dst = reg};
}

template <class T>
concept HasOperandDst =
    requires(const T& t) { t.dst; } &&
    std::is_same_v<std::remove_cvref_t<decltype(std::declval<const T&>().dst)>,
                   ir::Type_operand>;

const ir::Type_operand* dst_of(const ir::Type_instruction& ins) {
  return std::visit(
      [](const auto& i) -> const ir::Type_operand* {
        if constexpr (HasOperandDst<std::decay_t<decltype(i)>>) {
          return &i.dst;
        } else {
          return nullptr;
        }
      },
      ins);
}

struct Step {
  bool increment = false;
  int k = 0;
};

std::optional<Step> small_step(const ir::Type_instruction& carry,
                               const ir::Type_instruction& arith) {
  const ir::Type_operand* src = nullptr;
  const ir::Type_operand* dst = nullptr;
  bool increment = false;
  if (const auto* add = as<ir::Add>(arith); add && isa<ir::ClearCarry>(carry)) {
    src = &add->src;
    dst = &add->dst;
    increment = true;
  } else if (const auto* sub = as<ir::Sub>(arith); sub && isa<ir::SetCarry>(carry)) {
    src = &sub->src;
    dst = &sub->dst;
  } else {
    return std::nullopt;
  }
  const auto* imm = as<ir::Imm>(*src);
  if (imm == nullptr || !(imm->value == 1 || imm->value == 2) ||
      !is_reg(*dst, reg_is_a)) {
    return std::nullopt;
  }
  return Step{increment, static_cast<int>(imm->value)};
}

bool is_transfer(const ir::Type_instruction* ins, bool (*src_test)(const ir::Type_reg&),
                 bool (*dst_test)(const ir::Type_reg&)) {
  const auto* m = ins != nullptr ? as<ir::Mov>(*ins) : nullptr;
  return m != nullptr && !m->is_volatile && is_reg(m->src, src_test) &&
         is_reg(m->dst, dst_test);
}

bool is_indirect(const ir::Type_operand& op) {
  return isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY, ir::Frame,
             ir::Stack, ir::IndexedData>(op);
}

bool holds(const Instructions& instrs, std::size_t i, const ir::Type_operand& reg,
           const ir::Type_operand& mem, const NameSet& unaliased,
           const AsmContext& ctx) {
  const bool is_y = is_reg(reg, [](const ir::Type_reg& r) { return isa<ir::Y>(r); });
  const auto* mem_data = as<ir::Data>(mem);
  const bool is_private = mem_data != nullptr && unaliased.contains(mem_data->name);
  for (std::size_t k = i; k-- > 0;) {
    const ir::Type_instruction& ins = instrs[k];
    if (isa<ir::Label, ir::Jump, ir::Branch, ir::Return, ir::Ret, ir::Call,
            ir::FunctionPrologue, ir::AllocateStack, ir::LoadAddress,
            ir::DecimalArith>(ins)) {
      return false;
    }
    if (const auto* m = as<ir::Mov>(ins);
        m != nullptr && m->dst == reg && m->src == mem && !m->is_volatile) {
      return true;
    }
    const ir::Type_operand* dst = dst_of(ins);
    if ((is_y ? kills_y(ins) : kills_x(ins)) || (is_y && sets_up_y(ins)) ||
        (dst != nullptr && *dst == reg)) {
      return false;
    }
    if (dst == nullptr || isa<ir::Reg>(*dst)) {
      continue;
    }
    if (is_indirect(*dst)) {
      if (!is_private) {
        return false;
      }
      const auto* indexed = as<ir::IndexedData>(*dst);
      if (indexed != nullptr && indexed->name == mem_data->name) {
        return false;
      }
      continue;
    }
    if (*dst == mem || may_alias(*dst, mem, ctx)) {
      return false;
    }
  }
  return false;
}

bool rewrite_index_steps(Instructions& instrs, const NameSet& unaliased,
                         const AsmContext& ctx) {
  bool changed = false;
  std::size_t i = 0;
  while (i + 3 < instrs.size()) {
    const auto* load = as<ir::Mov>(instrs[i]);
    const auto* move = as<ir::Mov>(instrs[i + 3]);
    if (!(load != nullptr && !load->is_volatile && is_reg(load->dst, reg_is_a) &&
          isa<ir::Data, ir::ZP>(load->src) && move != nullptr &&
          is_reg(move->src, reg_is_a) && is_reg(move->dst, reg_is_index))) {
      ++i;
      continue;
    }
    const auto step = small_step(instrs[i + 1], instrs[i + 2]);
    if (!step) {
      ++i;
      continue;
    }
    const ir::Type_operand reg = move->dst;
    const ir::Type_operand mem = load->src;
    const std::size_t after = i + 4;
    if (!(holds(instrs, i, reg, mem, unaliased, ctx) && a_dead_at(instrs, after) &&
          c_dead_at(instrs, after) && v_dead_at(instrs, after))) {
      ++i;
      continue;
    }
    Instructions steps(static_cast<std::size_t>(step->k), step_of(step->increment, reg));
    instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(i),
                 instrs.begin() + static_cast<std::ptrdiff_t>(i + 4));
    instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(i), steps.begin(), steps.end());
    changed = true;
    ++i;
  }
  return changed;
}

void splice(Instructions& instrs, std::size_t begin, std::size_t end,
            Instructions replacement) {
  instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(begin),
               instrs.begin() + static_cast<std::ptrdiff_t>(end));
  instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(begin),
                std::make_move_iterator(replacement.begin()),
                std::make_move_iterator(replacement.end()));
}

bool rewrite_steps_across_transfer(Instructions& instrs) {
  bool changed = false;
  std::size_t i = 0;
  while (i + 2 < instrs.size()) {
    const ir::Type_instruction& first = instrs[i];
    const ir::Type_instruction& second = instrs[i + 1];
    const ir::Type_instruction& third = instrs[i + 2];
    std::size_t after = i + 3;
    std::optional<Instructions> rewritten;
    if (const auto found = small_step(first, second);
        found && is_transfer(&third, reg_is_a, reg_is_index) &&
        a_dead_at(instrs, after) && c_dead_at(instrs, after) &&
        v_dead_at(instrs, after)) {
      const ir::Type_operand& reg = std::get<ir::Mov>(third).dst;
      Instructions out{third};
      for (int k = 0; k < found->k; ++k) {
        out.push_back(step_of(found->increment, reg));
      }
      rewritten = std::move(out);
    }
    if (const auto found = small_step(second, third);
        !rewritten && found && is_transfer(&first, reg_is_index, reg_is_a)) {
      const ir::Type_operand reg = std::get<ir::Mov>(first).src;
      const bool is_x = isa<ir::X>(std::get<ir::Reg>(reg).reg);
      const ir::Type_instruction* back = after < instrs.size() ? &instrs[after] : nullptr;
      auto same_reg = is_x ? +[](const ir::Type_reg& r) { return isa<ir::X>(r); }
                           : +[](const ir::Type_reg& r) { return isa<ir::Y>(r); };
      Instructions steps;
      for (int k = 0; k < found->k; ++k) {
        steps.push_back(step_of(found->increment, reg));
      }
      if (is_transfer(back, reg_is_a, same_reg) && a_dead_at(instrs, after + 1) &&
          c_dead_at(instrs, after + 1) && v_dead_at(instrs, after + 1)) {
        rewritten = std::move(steps);
        after += 1;
      } else if ((is_x ? x_dead_at(instrs, after) : y_dead_at(instrs, after)) &&
                 c_dead_at(instrs, after) && v_dead_at(instrs, after)) {
        steps.push_back(first);
        rewritten = std::move(steps);
      }
    }
    if (!rewritten) {
      ++i;
      continue;
    }
    const std::size_t length = rewritten->size();
    splice(instrs, i, after, std::move(*rewritten));
    changed = true;
    i += length;
  }
  return changed;
}

bool rewrite_step_then_reload(Instructions& instrs) {
  bool changed = false;
  std::size_t i = 0;
  while (i + 4 < instrs.size()) {
    const auto found = small_step(instrs[i + 1], instrs[i + 2]);
    const auto* load = as<ir::Mov>(instrs[i]);
    const auto* reload = as<ir::Mov>(instrs[i + 4]);
    if (!(found && load != nullptr && !load->is_volatile &&
          isa<ir::Data, ir::ZP>(load->src) && is_reg(load->dst, reg_is_a) &&
          instrs[i + 3] == ir::Type_instruction{ir::Mov{.src = load->dst,
                                                        .dst = load->src}} &&
          reload != nullptr && !reload->is_volatile && reload->src == load->src &&
          is_reg(reload->dst, reg_is_index))) {
      ++i;
      continue;
    }
    const std::size_t after = i + 5;
    if (!(a_dead_at(instrs, after) && c_dead_at(instrs, after) &&
          v_dead_at(instrs, after))) {
      ++i;
      continue;
    }
    const ir::Type_operand reg = reload->dst;
    const ir::Type_operand mem = load->src;
    Instructions out{instrs[i + 4]};
    for (int k = 0; k < found->k; ++k) {
      out.push_back(step_of(found->increment, reg));
    }
    out.push_back(ir::Mov{.src = reg, .dst = mem});
    splice(instrs, i, after, std::move(out));
    changed = true;
    i += static_cast<std::size_t>(found->k) + 2;
  }
  return changed;
}

struct Match {
  ir::Type_operand reg;
  bool increment = false;
  ir::Type_operand dst;
};

std::optional<Match> match_pattern(const Instructions& instrs, std::size_t i) {
  if (i + 3 >= instrs.size()) {
    return std::nullopt;
  }
  const auto* a = as<ir::Mov>(instrs[i]);
  if (a == nullptr || a->is_volatile || !is_reg(a->src, reg_is_index) ||
      !is_reg(a->dst, reg_is_a)) {
    return std::nullopt;
  }
  const auto step = small_step(instrs[i + 1], instrs[i + 2]);
  if (!step || step->k != 1) {
    return std::nullopt;
  }
  // STX and STY have no indirect or absolute indexed form.
  const auto* d = as<ir::Mov>(instrs[i + 3]);
  if (d == nullptr || d->is_volatile || !is_reg(d->src, reg_is_a) ||
      !isa<ir::Data, ir::ZP>(d->dst)) {
    return std::nullopt;
  }
  const std::size_t after = i + 4;
  if (!a_dead_at(instrs, after)) {
    return std::nullopt;
  }
  const bool is_x = isa<ir::X>(std::get<ir::Reg>(a->src).reg);
  if (!(is_x ? x_dead_at(instrs, after) : y_dead_at(instrs, after))) {
    return std::nullopt;
  }
  if (!(c_dead_at(instrs, after) && v_dead_at(instrs, after))) {
    return std::nullopt;
  }
  return Match{a->src, step->increment, d->dst};
}

bool rewrite_pm1_stores(Instructions& instrs) {
  Instructions out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (const auto match = match_pattern(instrs, i)) {
      const ir::Type_operand reg =
          ir::Reg{.reg = std::get<ir::Reg>(match->reg).reg};
      out.push_back(step_of(match->increment, reg));
      out.push_back(ir::Mov{.src = reg, .dst = match->dst, .is_volatile = false});
      i += 4;
      changed = true;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    instrs = std::move(out);
  }
  return changed;
}

}  // namespace

asm_ast::Program apply_transfer_pm1_store(asm_ast::Program prog,
                                          const AsmContext& ctx) {
  const NameSet unaliased = unreachable_by_pointer(prog);
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_pm1_stores(fn->instructions);
      rewrite_index_steps(fn->instructions, unaliased, ctx);
      rewrite_steps_across_transfer(fn->instructions);
      rewrite_step_then_reload(fn->instructions);
    }
  }
  return prog;
}

}  // namespace c6502::passes
