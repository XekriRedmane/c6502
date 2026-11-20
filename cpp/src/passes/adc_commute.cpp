// Drops the spill of `STA temp; LDA mem; ADC temp; STA mem` when the
// commutativity of ADC lets the addition read mem directly.
//
// The shape
//
// A compound assignment `mem += V` where `V` was just computed into A.
// The plain lowering spills A:
//
//     Mov(Reg(A), temp)            ; STA temp, the spill of V
//     [operations that keep A and temp]
//     Mov(mem, Reg(A))             ; LDA mem
//     ClearCarry                   ; CLC (optional, for ADC)
//     Add(temp, Reg(A))            ; ADC temp, so A = mem + V
//     Mov(Reg(A), mem)             ; STA mem
//
// ADC is commutative, `mem + V == V + mem`, so if A still holds V at
// the ADC, with nothing between that writes A, the spill folds out:
//
//     [operations between]
//     ClearCarry
//     Add(mem, Reg(A))             ; ADC mem, so A = V + mem
//     Mov(Reg(A), mem)             ; STA mem
//
// The same shape works for `And` and `Or`, which are commutative and
// set the flags the same way. `Sub` isn't commutative, and `Xor` has
// the IR's form with three operands, a different shape.
//
// The final `STA mem` is optional. Without it the result in A is the
// same, and the rewrite still drops the load. `print_zstring`'s
// `get_alphabet() + z` (examples/zork/zstring.c) was `STA t; LDA z;
// CLC; ADC t` and becomes `CLC; ADC z`.
//
// What the rewrite does
//
//   - It drops the `LDA mem`, since A already holds V.
//   - It rewrites the `ADC temp` (or `AND temp` / `ORA temp`) to read
//     `mem`.
//   - It leaves the `STA temp` in place. If `temp` is dead afterward,
//     `asm_dead_store` removes it in the next sweep, and if `temp` is
//     still read for some other reason, the STA stays sound.
//
// So the leftover `STA temp` is no obligation of this pass's
// soundness. It's work for the dead store pass, which keeps the
// matching simple, with no analysis of what is dead across blocks,
// while it still takes two instructions off the hot path at each
// occurrence.
//
// The conditions
//
// A window of five positions with a flexible band between:
//
//   [i]      `Mov(Reg(A), temp)` with temp a ZP or Data byte, not
//            volatile.
//   [i+1..j-1]  operations between, each of which
//              - is a Mov, Inc, Dec, ClearCarry, SetCarry, Compare, or
//                BitTest,
//              - doesn't write `Reg(A)`, and
//              - doesn't read or write any byte that may be `temp`.
//   [j]      `Mov(mem, Reg(A))` with mem a ZP, Data, IndexedData,
//            IndirectZp, or IndirectZpY operand, not volatile.
//   [j+1]    an optional `ClearCarry` / `SetCarry`.
//   [k]      `Add(temp, Reg(A))`, `And(temp, Reg(A))`, or `Or(temp,
//            Reg(A))`, whose operand is the STA's destination, field
//            for field.
//   [k+1]    an optional `Mov(Reg(A), mem)` whose operand is the LDA's
//            source, field for field, not volatile.
//
// The band between is what relaxes a peephole of adjacent
// instructions. It takes the LDX/LDY of an index register that
// typically sits between a spill of A and the matching load of mem.
// Every kind in the band is named, and any other kind ends the match.
//
// Soundness
//
// Let V be A's value entering [i]. The original leaves A = V and
// temp = V through the band. [j] sets A to mem's value and N/Z from
// it, the CLC/SEC sets C, [k] sets A to op(mem, V, C) and N/Z/C/V from
// the result, and [k+1] stores A in mem. The rewrite leaves A = V
// through the band and past the dropped [j], with the flags the band
// left, the CLC/SEC sets C, the rewritten [k] sets A to op(V, mem, C)
// and N/Z/C/V from the result, and [k+1] stores A in mem.
//
// Both leave the same A after [k+1], since op is commutative in V and
// mem's value, and the same flags, since op overwrites N/Z/C/V. No
// instruction between [j] and [k+1] reads N/Z, so the flags of the
// dropped [j] aren't observed. C comes from the optional CLC/SEC and
// never from [j]. The index register an IndexedData mem uses is set
// before [j] and unchanged up to [k+1], so the rewritten `ADC mem` and
// the `STA mem` kept address the same byte.
//
// Where it runs
//
// Inside the peephole fixed point, with the other simplifications of
// read-modify-writes. It composes with the TAC-level sinkers that
// expose the STA-then-CLC-ADC pattern of a `+= V` on memory, and on its
// own it fires wherever the compiler emits the spill, reload, and add
// for a read-modify-write of one byte.
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
// volatile. Keeping the spill's destination a ZP or Data byte (no
// IndexedData) keeps the spill slot one fixed byte, the only shape the
// aliasing check handles cleanly.
bool is_sta(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_reg_a(m->src) &&
         isa<ir::ZP, ir::Data>(m->dst);
}

// Returns whether `op` is one of the operands `mem` may be: a byte with
// a fixed address, an indexed byte, or a byte through a zero-page
// pointer (`(zp),Y`), which `ADC`, `AND`, `ORA`, and `STA` all take. The
// `LDY #k` an `IndirectZp` operand begins with moves from the dropped
// load to the operation, with only a `CLC` or `SEC` between, so Y ends
// the same.
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
  if (const auto* da = as<ir::Data>(a)) {
    const auto* db = as<ir::Data>(b);
    return db != nullptr && da->name == db->name && da->offset == db->offset;
  }
  if (const auto* za = as<ir::ZP>(a)) {
    const auto* zb = as<ir::ZP>(b);
    return zb != nullptr && za->address == zb->address && za->offset == zb->offset;
  }
  if (const auto* xa = as<ir::IndexedData>(a)) {
    const auto* xb = as<ir::IndexedData>(b);
    return xb != nullptr && xa->name == xb->name && xa->offset == xb->offset &&
           xa->index.index() == xb->index.index();
  }
  if (const auto* pa = as<ir::IndirectZp>(a)) {
    const auto* pb = as<ir::IndirectZp>(b);
    return pb != nullptr && pa->address == pb->address && pa->offset == pb->offset;
  }
  if (const auto* pa = as<ir::IndirectZpY>(a)) {
    const auto* pb = as<ir::IndirectZpY>(b);
    return pb != nullptr && pa->address == pb->address;
  }
  return false;
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

// Returns whether `op` names memory. `may_alias` is defined only for
// these kinds, and a register or an immediate would fall through to
// its answer of true, which would reject an LDX or LDY in the band.
bool is_memory_kind(const ir::Type_operand& op) {
  return isa<ir::ZP, ir::Data, ir::IndexedData, ir::Stack, ir::Frame, ir::Indirect,
             ir::IndirectY, ir::IndirectZp, ir::IndirectZpY>(op);
}

// Returns whether `op` names memory that may be `temp`. Register,
// immediate, and label operands touch no memory.
bool maybe_aliases(const ir::Type_operand& op, const ir::Type_operand& temp,
                   const AsmContext& ctx) {
  return is_memory_kind(op) && may_alias(op, temp, ctx);
}

// Returns whether any operand of `instr` that names memory may be
// `temp`.
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
// through A and loads Y. Without the case of a destination in memory,
// `other = 7;` between the spill and the add made the add use 7 in
// place of the spilled value.
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
//
// A `Mov` covers LDX, LDY, STX, STY, TXA, TYA, TAX, TAY, and so on, and
// `writes_a` rejects those that write A. An `Inc` or `Dec` changes
// memory or an index register, and `accesses_alias` rejects one that
// touches `temp`. `ClearCarry` and `SetCarry` change only flags, which
// nothing between the spill and the dropped load reads. `Compare` reads
// its operands and sets flags, and `BitTest` reads memory and sets
// N/V/Z, and neither writes A. Push, Pop, Call, FunctionPrologue,
// AllocateStack, Return, Ret, LoadAddress, the read-modify-writes of
// memory that could write A through the indirect-Y expansion, and all
// control flow are left out.
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
  // Walks forward until a load that fits or an instruction the band
  // can't hold.
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
  // An optional ClearCarry / SetCarry between the LDA and the operation.
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
  // The commutative operation reading the spill into A.
  if (!is_comm_op_with_temp(instrs[k], temp)) {
    return std::nullopt;
  }
  // A store of the result back to `mem`, as `mem += v` makes, ends the
  // window when it's there. The result is the same without it, as in
  // `print_zstring`'s `get_alphabet() + z`.
  std::size_t sta_mem_idx = k;
  if (k + 1 < n && is_sta_compat(instrs[k + 1]) &&
      operands_equal(std::get<ir::Mov>(instrs[k + 1]).dst, mem)) {
    sta_mem_idx = k + 1;
  }
  return Match{j, k, sta_mem_idx, mem};
}

// Returns the operation `op` with its source replaced by `new_src`.
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
    // Skips past the window matched. The `STA mem` at its end is kept
    // and could be the spill of a later match, but that structure would
    // have to come from new code, not from this rewrite, and moving on
    // keeps the pass from matching its own rewrite again.
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

// The second stage: a constant plus a byte that X or Y still holds.

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

// Returns whether `instr` may write a byte that `mem` names.
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
