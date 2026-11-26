#include "passes/via_a_store_fold.h"

#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

const ir::Type_reg* xy_to_a_transfer(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr || m->is_volatile || !is_reg_a(m->dst)) {
    return nullptr;
  }
  const auto* r = as<ir::Reg>(m->src);
  if (r == nullptr || !isa<ir::X, ir::Y>(r->reg)) {
    return nullptr;
  }
  return &r->reg;
}

// STX takes the `zp`, `zp,Y`, and `abs` addressing modes and STY takes
// `zp`, `zp,X`, and `abs`, so neither can store through indirect-Y.
bool is_a_store_to_addressable(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_reg_a(m->src) &&
         isa<ir::Data, ir::ZP>(m->dst);
}

void rewrite_function(ir::Function& fn) {
  const Instrs instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (i + 1 < instrs.size()) {
      const ir::Type_reg* src = xy_to_a_transfer(instrs[i]);
      // TXA sets N and Z from X and STX leaves them, so the flags differ
      // after the fold.
      if (src != nullptr && is_a_store_to_addressable(instrs[i + 1]) &&
          a_dead_at(instrs, labels, i + 2) && flags_dead_at(instrs, labels, i + 2)) {
        out.push_back(ir::Mov{.src = ir::Reg{.reg = *src},
                              .dst = std::get<ir::Mov>(instrs[i + 1]).dst,
                              .is_volatile = false});
        i += 2;
        changed = true;
        continue;
      }
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_via_a_store_fold(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
