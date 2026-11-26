#include "passes/carry_in_add.h"

#include <cstddef>
#include <iterator>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kZero = ir::Imm{.value = 0};
const ir::Type_operand kOne = ir::Imm{.value = 1};

const ir::Mov* mov(const ir::Type_instruction& ins, const ir::Type_operand* src = nullptr,
                   const ir::Type_operand* dst = nullptr) {
  const auto* m = as<ir::Mov>(ins);
  if (m == nullptr || m->is_volatile || (src != nullptr && m->src != *src) ||
      (dst != nullptr && m->dst != *dst)) {
    return nullptr;
  }
  return m;
}

const ir::Add* add(const ir::Type_instruction& ins, const ir::Type_operand* src = nullptr) {
  const auto* a = as<ir::Add>(ins);
  if (a == nullptr || a->dst != kA || (src != nullptr && a->src != *src)) {
    return nullptr;
  }
  return a;
}

bool is_slot(const ir::Type_operand& op, const std::string& prefix) {
  const auto* d = as<ir::Data>(op);
  return d != nullptr && d->name.starts_with(prefix);
}

struct Match {
  ir::Type_operand m, h, t0, t1, r0, r1;
};

std::optional<Match> match(const ir::Type_instruction* w, const std::string& prefix) {
  if (!(isa<ir::ClearCarry>(w[0]) && add(w[1]) && mov(w[2], &kA) && mov(w[3], nullptr, &kA) &&
        add(w[4], &kZero) && mov(w[5], &kA) && mov(w[6], nullptr, &kA) &&
        isa<ir::ClearCarry>(w[7]) && add(w[8], &kOne) && mov(w[9], &kA) &&
        mov(w[10], nullptr, &kA) && add(w[11], &kZero) && mov(w[12], &kA))) {
    return std::nullopt;
  }
  Match r{
      .m = std::get<ir::Add>(w[1]).src,
      .h = std::get<ir::Mov>(w[3]).src,
      .t0 = std::get<ir::Mov>(w[2]).dst,
      .t1 = std::get<ir::Mov>(w[5]).dst,
      .r0 = std::get<ir::Mov>(w[9]).dst,
      .r1 = std::get<ir::Mov>(w[12]).dst,
  };
  if (!(std::get<ir::Mov>(w[6]).src == r.t0 && std::get<ir::Mov>(w[10]).src == r.t1)) {
    return std::nullopt;
  }
  if (!is_slot(r.t0, prefix) || !is_slot(r.t1, prefix)) {
    return std::nullopt;
  }
  if (r.t0 == r.t1 || r.h == r.t0 || r.r0 == r.t1) {
    return std::nullopt;
  }
  if (!isa<ir::Data, ir::ZP>(r.r0)) {
    return std::nullopt;
  }
  if (isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(r.h)) {
    return r;
  }
  if (is_slot(r.h, prefix) && r.h != r.r0) {
    return r;
  }
  return std::nullopt;
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx,
                      const SymbolAddrs* zp_slot_symbols) {
  auto& instrs = fn.instructions;
  const std::string prefix = "__local_" + fn.name + "__";
  std::size_t i = 0;
  while (i + 13 <= instrs.size()) {
    const auto found = match(&instrs[i], prefix);
    if (!found) {
      ++i;
      continue;
    }
    const std::size_t end = i + 13;
    if (!(c_dead_at(instrs, end) && v_dead_at(instrs, end) &&
          asm_dead_store::mem_dead_after(instrs, i + 6, found->t0, ctx, fn.name,
                                         zp_slot_symbols) &&
          asm_dead_store::mem_dead_after(instrs, i + 10, found->t1, ctx, fn.name,
                                         zp_slot_symbols))) {
      ++i;
      continue;
    }
    std::vector<ir::Type_instruction> repl = {
        ir::SetCarry{},
        ir::Add{.src = found->m, .dst = kA},
        ir::Mov{.src = kA, .dst = found->r0, .is_volatile = false},
        ir::Mov{.src = found->h, .dst = kA, .is_volatile = false},
        ir::Add{.src = kZero, .dst = kA},
        ir::Mov{.src = kA, .dst = found->r1, .is_volatile = false},
    };
    const auto first = instrs.begin() + static_cast<std::ptrdiff_t>(i);
    instrs.erase(first, first + 13);
    instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(i),
                  std::make_move_iterator(repl.begin()), std::make_move_iterator(repl.end()));
    i += 6;
  }
}

}  // namespace

asm_ast::Program apply_carry_in_add(asm_ast::Program prog, const AsmContext& ctx,
                                    const SymbolAddrs* zp_slot_symbols) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx, zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
