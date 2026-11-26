#include "passes/adc_commute.h"

#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "errors.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using Instructions = std::vector<ir::Type_instruction>;

// Returns whether `instr` is `Mov(Reg(A), <ZP|Data>)` and isn't
// volatile.
bool is_sta(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_reg_a(m->src) &&
         isa<ir::ZP, ir::Data>(m->dst);
}

// Returns whether `op` is one of the operands `mem` may be: a byte with
// a fixed address, an indexed byte, or a byte through a zero-page
// pointer (`(zp),Y`), which `ADC`, `AND`, `ORA`, and `STA` all take.
bool is_mem_kind(const ir::Type_operand& op) {
  return isa<ir::ZP, ir::Data, ir::IndexedData, ir::IndirectZp, ir::IndirectZpY>(op);
}

// Returns whether `instr` is `Mov(<mem>, Reg(A))` for one of those
// operands and isn't volatile. ADC takes the same addressing modes, so
// the rewrite stays encodable.
bool is_lda_compat(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_reg_a(m->dst) && is_mem_kind(m->src);
}

// Returns whether `instr` is `Mov(Reg(A), <mem>)`, the store that
// matches `is_lda_compat`, which STA takes in the same addressing
// modes, and isn't volatile.
bool is_sta_compat(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_reg_a(m->src) && is_mem_kind(m->dst);
}

bool is_clc_or_sec(const ir::Type_instruction& instr) {
  return isa<ir::ClearCarry, ir::SetCarry>(instr);
}

// Returns whether `a` and `b` are the same operand, field for field,
// for the kinds this peephole matches, and false when the kinds differ.
bool operands_equal(const ir::Type_operand& a, const ir::Type_operand& b) {
  return isa<ir::Data, ir::ZP, ir::IndexedData, ir::IndirectZp, ir::IndirectZpY>(a) && a == b;
}

// Returns the source of `instr` when it is an `Add`, `And`, or `Or`
// into `Reg(A)`, or null.
const ir::Type_operand* comm_op_src(const ir::Type_instruction& instr) {
  if (const auto* i = as<ir::Add>(instr)) {
    return is_reg_a(i->dst) ? &i->src : nullptr;
  }
  if (const auto* i = as<ir::And>(instr)) {
    return is_reg_a(i->dst) ? &i->src : nullptr;
  }
  if (const auto* i = as<ir::Or>(instr)) {
    return is_reg_a(i->dst) ? &i->src : nullptr;
  }
  return nullptr;
}

// Returns whether `instr` is an `Add`, `And`, or `Or` into `Reg(A)`
// whose source is `temp`.
bool is_comm_op_with_temp(const ir::Type_instruction& instr,
                          const ir::Type_operand& temp) {
  const ir::Type_operand* src = comm_op_src(instr);
  return src != nullptr && operands_equal(*src, temp);
}

bool is_memory_kind(const ir::Type_operand& op) {
  return isa<ir::ZP, ir::Data, ir::IndexedData, ir::Stack, ir::Frame, ir::Indirect,
             ir::IndirectY, ir::IndirectZp, ir::IndirectZpY>(op);
}

bool maybe_aliases(const ir::Type_operand& op, const ir::Type_operand& temp,
                   const AsmContext& ctx) {
  return is_memory_kind(op) && may_alias(op, temp, ctx);
}

bool accesses_alias(const ir::Type_instruction& instr, const ir::Type_operand& temp,
                    const AsmContext& ctx) {
  if (const auto* m = as<ir::Mov>(instr)) {
    return maybe_aliases(m->src, temp, ctx) || maybe_aliases(m->dst, temp, ctx);
  }
  if (const auto* i = as<ir::Inc>(instr)) {
    return maybe_aliases(i->dst, temp, ctx);
  }
  if (const auto* d = as<ir::Dec>(instr)) {
    return maybe_aliases(d->dst, temp, ctx);
  }
  if (const auto* c = as<ir::Compare>(instr)) {
    return maybe_aliases(c->left, temp, ctx) || maybe_aliases(c->right, temp, ctx);
  }
  if (const auto* b = as<ir::BitTest>(instr)) {
    return maybe_aliases(b->src, temp, ctx);
  }
  return false;
}

// Returns whether `instr` may write `Reg(A)` as it's emitted. That's a
// `Mov` into A, and also a `Mov` into memory from anything other than a
// register, which is emitted as `LDA src; STA dst`, and a transfer
// between X and Y, which goes through A (`TXA; TAY`). An operand on the
// soft stack or behind a pointer counts too, since its expansion can go
// through A and loads Y.
bool writes_a(const ir::Type_instruction& instr) {
  bool expanding = false;
  for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
    if (isa<ir::Frame, ir::Stack, ir::Indirect, ir::IndirectY, ir::IndirectZp,
            ir::IndirectZpY>(op)) {
      expanding = true;
    }
  });
  if (expanding) {
    return true;
  }
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr) {
    return false;
  }
  if (is_reg_a(m->dst)) {
    return true;
  }
  if (!isa<ir::Reg>(m->src)) {
    return !isa<ir::Reg>(m->dst);
  }
  return isa<ir::Reg>(m->dst);
}

// Returns whether `instr` may sit in the band between the spill and the
// load: it is one of the kinds named (Mov, Inc, Dec, ClearCarry,
// SetCarry, Compare, BitTest), doesn't write Reg(A), and doesn't read
// or write `temp`.
bool is_preserving(const ir::Type_instruction& instr, const ir::Type_operand& temp,
                   const AsmContext& ctx) {
  if (!isa<ir::Mov, ir::Inc, ir::Dec, ir::ClearCarry, ir::SetCarry, ir::Compare,
           ir::BitTest>(instr)) {
    return false;
  }
  return !writes_a(instr) && !accesses_alias(instr, temp, ctx);
}

// A match of the window: the LDA to drop, the operation to rewrite,
// the last instruction of the window, and the operand `mem`.
struct Match {
  std::size_t lda_idx = 0;
  std::size_t op_idx = 0;
  std::size_t sta_mem_idx = 0;
  ir::Type_operand mem;
};

// Returns the match of the pattern starting at `instrs[i]`, with the
// spill at `i`.
std::optional<Match> try_match(const Instructions& instrs, std::size_t i,
                               const AsmContext& ctx) {
  const std::size_t n = instrs.size();
  if (!is_sta(instrs[i])) {
    return std::nullopt;
  }
  const ir::Type_operand& temp = std::get<ir::Mov>(instrs[i]).dst;
  std::size_t j = i + 1;
  while (true) {
    if (j >= n) {
      return std::nullopt;
    }
    if (is_lda_compat(instrs[j])) {
      break;
    }
    if (!is_preserving(instrs[j], temp, ctx)) {
      return std::nullopt;
    }
    ++j;
  }
  const ir::Type_operand& mem = std::get<ir::Mov>(instrs[j]).src;
  std::size_t k = j + 1;
  if (k >= n) {
    return std::nullopt;
  }
  if (is_clc_or_sec(instrs[k])) {
    ++k;
    if (k >= n) {
      return std::nullopt;
    }
  }
  if (!is_comm_op_with_temp(instrs[k], temp)) {
    return std::nullopt;
  }
  std::size_t sta_mem_idx = k;
  if (k + 1 < n && is_sta_compat(instrs[k + 1]) &&
      operands_equal(std::get<ir::Mov>(instrs[k + 1]).dst, mem)) {
    sta_mem_idx = k + 1;
  }
  return Match{j, k, sta_mem_idx, mem};
}

ir::Type_instruction replace_src(const ir::Type_instruction& op,
                                 const ir::Type_operand& new_src) {
  if (const auto* i = as<ir::Add>(op)) {
    return ir::Add{.src = new_src, .dst = i->dst};
  }
  if (const auto* i = as<ir::And>(op)) {
    return ir::And{.src = new_src, .dst = i->dst};
  }
  if (const auto* i = as<ir::Or>(op)) {
    return ir::Or{.src = new_src, .dst = i->dst};
  }
  throw InternalError("unexpected op class");
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx) {
  const Instructions& instrs = fn.instructions;
  const std::size_t n = instrs.size();
  std::set<std::size_t> drop;
  std::map<std::size_t, ir::Type_instruction> rewrite;
  std::size_t i = 0;
  while (i < n) {
    if (drop.contains(i) || rewrite.contains(i)) {
      ++i;
      continue;
    }
    const auto match = try_match(instrs, i, ctx);
    if (!match) {
      ++i;
      continue;
    }
    drop.insert(match->lda_idx);
    rewrite[match->op_idx] = replace_src(instrs[match->op_idx], match->mem);
    i = match->sta_mem_idx + 1;
  }
  if (drop.empty() && rewrite.empty()) {
    return;
  }
  Instructions out;
  out.reserve(n);
  for (std::size_t k = 0; k < n; ++k) {
    if (drop.contains(k)) {
      continue;
    }
    const auto it = rewrite.find(k);
    out.push_back(it != rewrite.end() ? it->second : instrs[k]);
  }
  fn.instructions = std::move(out);
}

// Returns 'X' or 'Y' when `op` is that register, and 0 otherwise.
char index_reg(const ir::Type_operand& op) {
  if (const auto* r = as<ir::Reg>(op)) {
    if (isa<ir::X>(r->reg)) {
      return 'X';
    }
    if (isa<ir::Y>(r->reg)) {
      return 'Y';
    }
  }
  return 0;
}

// Returns whether `instr`, as `asm_emit` writes it, may change `reg`
// ('X' or 'Y').
bool writes_reg(const ir::Type_instruction& instr, char reg) {
  if (reg == 'Y' && sets_up_y(instr)) {
    return true;
  }
  if (const auto* m = as<ir::Mov>(instr)) {
    return index_reg(m->dst) == reg;
  }
  if (const auto* i = as<ir::Inc>(instr)) {
    return index_reg(i->dst) == reg;
  }
  if (const auto* d = as<ir::Dec>(instr)) {
    return index_reg(d->dst) == reg;
  }
  return false;
}

bool writes_memory_alias(const ir::Type_instruction& instr, const ir::Type_operand& mem,
                         const AsmContext& ctx) {
  const ir::Type_operand* dst = nullptr;
  if (const auto* m = as<ir::Mov>(instr)) {
    dst = &m->dst;
  } else if (const auto* i = as<ir::Inc>(instr)) {
    dst = &i->dst;
  } else if (const auto* d = as<ir::Dec>(instr)) {
    dst = &d->dst;
  } else if (const auto* asl = as<ir::ArithmeticShiftLeft>(instr)) {
    dst = &asl->dst;
  } else if (const auto* lsr = as<ir::LogicalShiftRight>(instr)) {
    dst = &lsr->dst;
  } else if (const auto* rol = as<ir::RotateLeft>(instr)) {
    dst = &rol->dst;
  } else if (const auto* ror = as<ir::RotateRight>(instr)) {
    dst = &ror->dst;
  }
  return dst != nullptr && maybe_aliases(*dst, mem, ctx);
}

bool is_straight(const ir::Type_instruction& instr) {
  return isa<ir::Mov, ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor, ir::Compare,
             ir::BitTest, ir::ClearCarry, ir::SetCarry, ir::Inc, ir::Dec,
             ir::ArithmeticShiftLeft, ir::LogicalShiftRight, ir::RotateLeft,
             ir::RotateRight, ir::Branch>(instr);
}

// Returns X or Y when the straight-line code before `instrs[i]` last set
// it with `STX mem` / `LDX mem` (or Y's), not as a volatile access, and
// nothing since has written that register or may have written `mem`.
std::optional<ir::Type_operand> register_holding(const Instructions& instrs,
                                                 std::size_t i,
                                                 const ir::Type_operand& mem,
                                                 const AsmContext& ctx) {
  for (std::size_t k = i; k-- > 0;) {
    const ir::Type_instruction& ins = instrs[k];
    if (!is_straight(ins)) {
      return std::nullopt;
    }
    if (const auto* m = as<ir::Mov>(ins); m != nullptr && !m->is_volatile) {
      const ir::Type_operand* reg_op = nullptr;
      if (operands_equal(m->dst, mem) && index_reg(m->src) != 0) {
        reg_op = &m->src;
      } else if (operands_equal(m->src, mem) && index_reg(m->dst) != 0) {
        reg_op = &m->dst;
      }
      if (reg_op != nullptr) {
        const char reg = index_reg(*reg_op);
        for (std::size_t q = k + 1; q < i; ++q) {
          if (writes_reg(instrs[q], reg)) {
            return std::nullopt;
          }
        }
        return *reg_op;
      }
    }
    if (writes_memory_alias(ins, mem, ctx)) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

// Applies `apply_register_copy_commute`'s rewrite to one function, with
// the names no pointer reaches installed as the pointer proof of `ctx`.
void register_copy_function(ir::Function& fn, const AsmContext& ctx) {
  Instructions& instrs = fn.instructions;
  for (std::size_t i = 0; i + 1 < instrs.size(); ++i) {
    const auto* load = as<ir::Mov>(instrs[i]);
    if (!(load != nullptr && isa<ir::Imm, ir::Data, ir::ZP>(load->src) &&
          !load->is_volatile && is_reg_a(load->dst))) {
      continue;
    }
    std::size_t k = i + 1;
    if (is_clc_or_sec(instrs[k])) {
      ++k;
    }
    if (k >= instrs.size()) {
      continue;
    }
    const ir::Type_instruction& op = instrs[k];
    const ir::Type_operand* op_src = comm_op_src(op);
    if (op_src == nullptr || !isa<ir::Data, ir::ZP>(*op_src)) {
      continue;
    }
    const auto reg = register_holding(instrs, i, *op_src, ctx);
    if (!reg) {
      continue;
    }
    const ir::Type_operand load_src = load->src;
    const ir::Type_operand load_dst = load->dst;
    instrs[k] = replace_src(op, load_src);
    instrs[i] = ir::Mov{.src = *reg, .dst = load_dst, .is_volatile = false};
  }
}

}  // namespace

asm_ast::Program apply_adc_commute(asm_ast::Program prog, const AsmContext& ctx) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx);
    }
  }
  return apply_register_copy_commute(std::move(prog), ctx);
}

asm_ast::Program apply_register_copy_commute(asm_ast::Program prog,
                                             const AsmContext& ctx) {
  const AsmContext inner = ctx.with_pointer_proof(unreachable_by_pointer(prog));
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      register_copy_function(*fn, inner);
    }
  }
  return prog;
}

}  // namespace c6502::passes
