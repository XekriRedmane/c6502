#include "passes/volatile_void_read_cmp.h"

#include <cstddef>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_candidate(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && m->is_volatile &&
         isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY>(m->src) &&
         is_reg_a(m->dst);
}

void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  bool any = false;
  for (const auto& instr : instrs) {
    any = any || is_candidate(instr);
  }
  if (!any) {
    return;
  }
  std::vector<ir::Type_instruction> out = instrs;
  const LabelMap labels(instrs);
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (!is_candidate(instrs[i])) {
      continue;
    }
    // CMP performs the same read cycle as LDA, but it sets N, Z, and C
    // from `A - byte` where LDA sets N and Z from the byte and leaves C.
    if (!(a_dead_at(instrs, labels, i + 1) && flags_dead_at(instrs, labels, i + 1) &&
          c_dead_at(instrs, labels, i + 1))) {
      continue;
    }
    const auto& m = std::get<ir::Mov>(instrs[i]);
    out[i] = ir::Compare{.left = m.dst, .right = m.src};
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_volatile_void_read_cmp(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
