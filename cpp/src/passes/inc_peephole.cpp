// The multi-byte INC peephole, which collapses the add-with-carry chain
// of `mem += 1` into an `INC; BNE` chain.
//
// `tac_to_asm` lowers `Binary(Add, x, 1, x)`, an add of 1 in place, on
// a 2-, 4-, or 8-byte value as an ADC chain with one step per byte:
//
//     Mov(M[0], A); CLC; Add(Imm(1), A); Mov(A, M[0])
//     Mov(M[1], A); Add(Imm(0), A);      Mov(A, M[1])
//     Mov(M[2], A); Add(Imm(0), A);      Mov(A, M[2])
//     ...
//
// where `M[k]` is the k-th byte of the operand and the carry from each
// ADC goes into the next (LDA sets only N and Z and leaves C alone).
// For `Data(name, k)` operands (static storage) and `ZP(addr+k, 0)`
// operands (locals the register allocator placed), the 6502 has a much
// shorter encoding. `INC m` is a read-modify-write of 2 bytes (zero
// page) or 3 (absolute) that increments the byte in place and sets Z
// exactly when the result wrapped to zero. With it the add of 1
// becomes
//
//     INC M[0]
//     BNE done
//     INC M[1]
//     BNE done
//     ...
//     INC M[N-1]
//   done:
//
// For a 16-bit absolute value that's 8 bytes and 9 to 14 cycles
// against 17 bytes and 22 cycles for the ADC chain, and the saving
// grows with the width. A 1-byte value takes one `INC m`.
//
// Eligibility, for each byte
//
//   * The operand is `Data(name, k)`, `ZP(addr, 0)`, or an array
//     element indexed by X, `IndexedData(name, k, X)` (`INC t,X`).
//     The other kinds (`Frame`, `Stack`, `Indirect`, an element indexed
//     by Y) use addressing modes INC doesn't have, so they are skipped.
//     X can't change within the matched run, whose instructions are
//     adjacent and write only A, the flags, and the bytes, so each
//     `t+k,X` names the byte the ADC form read and wrote. For an
//     element, C and V must also be dead after the run.
//   * The load of each byte reads the byte its store writes, an update
//     in place. When they differ, which is common after SSA destruction
//     routes the value through a temporary, INC would write the wrong
//     byte or leave the temporary stale, so those are skipped too.
//
// The bytes don't have to be at consecutive addresses. The byte-
// granular asm SSA and register allocation may put the bytes of one
// value in zero-page slots that aren't adjacent, but the shape (CLC
// and ADC #1 on the first byte, ADC #0 on each later byte with no CLC
// between, and every byte updated in place) only comes from the
// lowering of a multi-byte add of 1. So wherever the bytes are, INC
// and BNE on their addresses compute the same thing.
//
// A
//
// The ADC chain leaves the top byte's new value in A, and the INC chain
// doesn't touch A. When A is read afterward (as by `return ++counter;`
// for a static `unsigned char`, whose `Return` reads A), the chain is
// followed by `LDA M[N-1]`, which is still shorter than the ADC form.
// `asm_liveness.a_dead_at` decides.
//
// The flags
//
// The ADC chain leaves C as the last ADC set it, and the INC chain
// doesn't touch C. c6502's code generation never reads C across
// separate operations (every comparison loads its own value, which
// sets N and Z again, and every SBC and ADC has its own SEC or CLC),
// so later instructions can't tell the difference. The Z flag at
// `done:` depends on the path taken in the INC chain, but the same is
// true of the ADC chain.
//
// Where it runs
//
// After `replace_pseudoregisters`, since the operands have to be
// concrete `Data` and `ZP` bytes to decide eligibility, and before
// `expand_long_branches`, so that the BNEs it adds go through that
// pass's check of branch distances. It fits both the optimized and the
// unoptimized pipeline, since its saving comes from the addressing
// modes and from no state the register allocator leaves.
#include "passes/inc_peephole.h"

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_x_indexed(const ir::Type_operand& op) {
  const auto* d = as<ir::IndexedData>(op);
  return d != nullptr && isa<ir::X>(d->index);
}

namespace inc_peephole {

bool is_inc_eligible_operand(const ir::Type_operand& op) {
  if (isa<ir::Data, ir::ZP>(op) || is_x_indexed(op)) {
    return true;
  }
  if (const auto* r = as<ir::Reg>(op)) {
    return isa<ir::X, ir::Y>(r->reg);
  }
  return false;
}

bool operands_equal(const ir::Type_operand& a, const ir::Type_operand& b) {
  if (const auto* da = as<ir::Data>(a)) {
    const auto* db = as<ir::Data>(b);
    return db != nullptr && da->name == db->name && da->offset == db->offset;
  }
  if (const auto* za = as<ir::ZP>(a)) {
    const auto* zb = as<ir::ZP>(b);
    return zb != nullptr && za->address == zb->address && za->offset == zb->offset;
  }
  if (const auto* ra = as<ir::Reg>(a)) {
    const auto* rb = as<ir::Reg>(b);
    return rb != nullptr && ra->reg.index() == rb->reg.index();
  }
  if (const auto* xa = as<ir::IndexedData>(a)) {
    const auto* xb = as<ir::IndexedData>(b);
    return xb != nullptr && xa->name == xb->name && xa->offset == xb->offset &&
           xa->index.index() == xb->index.index();
  }
  return false;
}

}  // namespace inc_peephole

namespace {

using inc_peephole::is_inc_eligible_operand;
using inc_peephole::operands_equal;

constexpr std::string_view kDonePrefix = ".inc_done@";

bool is_reg_a_op(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_imm(const ir::Type_operand& op, int value) {
  const auto* imm = as<ir::Imm>(op);
  return imm != nullptr && imm->value == value;
}

// Returns the number that the digits `s` spell, or nothing when `s` is
// empty or holds anything but ASCII digits (Python's `str.isdigit` and
// `int`).
std::optional<long long> parse_digits(std::string_view s) {
  if (s.empty()) {
    return std::nullopt;
  }
  long long n = 0;
  for (char c : s) {
    if (c < '0' || c > '9') {
      return std::nullopt;
    }
    n = n * 10 + (c - '0');
  }
  return n;
}

// Returns M when `instrs[start..start+4)` is the first byte of the
// chain, `Mov(M, A); CLC; Add(Imm(1), A); Mov(A, M)`. Neither move may
// be volatile. The original has one read and one write of the byte,
// and `INC M` would make the same accesses, but a read-modify-write of
// a byte that can change on its own is a different kind of access,
// which the compiler can't make out of a separate read and write.
const ir::Type_operand* match_first_byte(Instrs instrs, std::size_t start) {
  if (start + 4 > instrs.size()) {
    return nullptr;
  }
  const auto* i0 = as<ir::Mov>(instrs[start]);
  if (i0 == nullptr || i0->is_volatile || !is_inc_eligible_operand(i0->src) ||
      !is_reg_a_op(i0->dst)) {
    return nullptr;
  }
  if (!isa<ir::ClearCarry>(instrs[start + 1])) {
    return nullptr;
  }
  const auto* i2 = as<ir::Add>(instrs[start + 2]);
  if (i2 == nullptr || !is_imm(i2->src, 1) || !is_reg_a_op(i2->dst)) {
    return nullptr;
  }
  const auto* i3 = as<ir::Mov>(instrs[start + 3]);
  if (i3 == nullptr || i3->is_volatile || !is_reg_a_op(i3->src) ||
      !operands_equal(i3->dst, i0->src)) {
    return nullptr;
  }
  return &i0->src;
}

// Returns M when `instrs[start..start+3)` is a later byte of the chain,
// `Mov(M, A); Add(Imm(0), A); Mov(A, M)`, with no CLC, since the carry
// of the byte before goes straight in. M needn't be at the address
// after the byte before, since the byte-granular register allocation
// puts the bytes of one value in slots of their own, but it has to be
// an operand INC takes, the update has to be in place, and neither
// move may be volatile, as for the first byte.
const ir::Type_operand* match_continuation_byte(Instrs instrs, std::size_t start) {
  if (start + 3 > instrs.size()) {
    return nullptr;
  }
  const auto* i0 = as<ir::Mov>(instrs[start]);
  if (i0 == nullptr || i0->is_volatile || !is_inc_eligible_operand(i0->src) ||
      !is_reg_a_op(i0->dst)) {
    return nullptr;
  }
  const auto* i1 = as<ir::Add>(instrs[start + 1]);
  if (i1 == nullptr || !is_imm(i1->src, 0) || !is_reg_a_op(i1->dst)) {
    return nullptr;
  }
  const auto* i2 = as<ir::Mov>(instrs[start + 2]);
  if (i2 == nullptr || i2->is_volatile || !is_reg_a_op(i2->src) ||
      !operands_equal(i2->dst, i0->src)) {
    return nullptr;
  }
  return &i0->src;
}

// Returns whether an `Add` (ADC) after `idx` reads the carry that the
// folded chain would have produced, before anything resets it. Moves
// (loads, stores, and transfers) leave the carry alone, so the scan
// goes past them. An `Add` reads it, and anything else (a CLC, a SEC,
// a compare, a branch, an INC, a DEC, ...) resets it or doesn't depend
// on it. c6502 only writes an `Add` that reads the carry without a CLC
// or SEC before it for the later bytes of a multi-byte add, so this
// finds exactly a later byte that wasn't folded.
bool carry_consumed_after(Instrs instrs, std::size_t idx) {
  for (std::size_t j = idx; j < instrs.size(); ++j) {
    if (isa<ir::Mov>(instrs[j])) {
      continue;
    }
    return isa<ir::Add>(instrs[j]);
  }
  return false;
}

struct Match {
  std::size_t consumed = 0;
  std::vector<ir::Type_operand> mem_ops;
};

// Matches the chain of an add of 1 that starts at `instrs[start]`: the
// first byte's four instructions, then three for each later byte. The
// operands must be ones INC takes, and each byte's update must be in
// place.
std::optional<Match> try_match(Instrs instrs, std::size_t start) {
  if (start + 4 > instrs.size()) {
    return std::nullopt;
  }
  const ir::Type_operand* head = match_first_byte(instrs, start);
  if (head == nullptr) {
    return std::nullopt;
  }
  Match m;
  m.mem_ops.push_back(*head);
  m.consumed = 4;
  while (const ir::Type_operand* next = match_continuation_byte(instrs, start + m.consumed)) {
    m.mem_ops.push_back(*next);
    m.consumed += 3;
  }
  // The INC chain doesn't set the carry, but the ADC chain's carry out
  // goes into any later byte (`Mov(src, A); Add(Imm(0), A); ...`) that
  // wasn't folded, such as a high byte whose result goes to a register
  // (the high byte of a register return) where an update in place
  // would store it. When such an `Add` follows, the fold would drop the
  // carry it reads, so the whole match is refused.
  if (carry_consumed_after(instrs, start + m.consumed)) {
    return std::nullopt;
  }
  // An element indexed by X needs C and V dead after the run, since the
  // ADC form sets them and the INC form leaves them. The `Data` and
  // `ZP` forms rely on the code generation's convention described at
  // the top of this file. This form is newer, and the check costs
  // nothing where it passes.
  const bool any_indexed = std::any_of(m.mem_ops.begin(), m.mem_ops.end(),
                                       [](const ir::Type_operand& op) {
                                         return is_x_indexed(op);
                                       });
  if (any_indexed && !(c_dead_at(instrs, start + m.consumed) &&
                       v_dead_at(instrs, start + m.consumed))) {
    return std::nullopt;
  }
  return m;
}

// Appends the INC chain of an N-byte add of 1 to `out`. One byte takes
// one `Inc` and no branch. More take `Inc(M[k]); Branch(NE, done)` for
// each byte but the last, then `Inc(M[N-1]); Label(done)`, with a new
// `.inc_done@<counter>` label for each chain. The leading `.` makes the
// label local to dasm's SUBROUTINE, and `@<digits>` keeps it apart from
// the user's labels (`.<funcname>@<ident>`) and from the other labels
// the translator makes (`.if_end@<N>`, `.cmp_true@<N>`, `.lb_skip@<N>`).
void build_inc_chain(const std::vector<ir::Type_operand>& mem_ops, long long& counter,
                     std::vector<ir::Type_instruction>& out) {
  if (mem_ops.size() == 1) {
    out.push_back(ir::Inc{.dst = mem_ops[0]});
    return;
  }
  const std::string done = std::string(kDonePrefix) + std::to_string(counter);
  ++counter;
  for (std::size_t k = 0; k < mem_ops.size(); ++k) {
    out.push_back(ir::Inc{.dst = mem_ops[k]});
    if (k + 1 < mem_ops.size()) {
      out.push_back(ir::Branch{.cond = ir::NE{}, .target = done});
    }
  }
  out.push_back(ir::Label{.name = done});
}

// Rewrites each chain in `fn`, numbering the labels from `counter`,
// which it leaves at the next free number. At each position it tries
// the pattern, and on a match it writes the INC chain and goes past the
// matched instructions.
void rewrite_function(ir::Function& fn, long long& counter) {
  const auto& instrs = fn.instructions;
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::optional<Match> match = try_match(instrs, i);
    if (!match) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    changed = true;
    build_inc_chain(match->mem_ops, counter, out);
    if (!a_dead_at(instrs, i + match->consumed)) {
      // The ADC chain left the top byte's new value in A.
      out.push_back(ir::Mov{.src = match->mem_ops.back(),
                            .dst = ir::Reg{.reg = ir::A{}},
                            .is_volatile = false});
    }
    i += match->consumed;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_inc_peephole(asm_ast::Program prog) {
  // The labels are numbered past every `.inc_done@N` already in the
  // program. The peephole fixed point runs this pass many times, and a
  // count started at 0 each time gave a function two `.inc_done@0`
  // labels, which dasm rejects.
  long long counter = 0;
  for (const auto& tl : prog.top_level) {
    const auto* fn = as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto& ins : fn->instructions) {
      const auto* label = as<ir::Label>(ins);
      if (label == nullptr || !label->name.starts_with(kDonePrefix)) {
        continue;
      }
      if (auto n = parse_digits(std::string_view(label->name).substr(kDonePrefix.size()))) {
        counter = std::max(counter, *n + 1);
      }
    }
  }
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, counter);
    }
  }
  return prog;
}

}  // namespace c6502::passes
