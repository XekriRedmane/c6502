// Byte-granular dead-store elimination on the asm SSA form.
//
// Drops the `Mov` and `Phi` instructions whose Pseudo destination no
// instruction of the function uses. It runs to a fixed point inside the
// asm SSA round trip, since dropping one `Mov` can leave its source's
// definition unused in the next round.
//
// The TAC dead-store elimination drops a whole value that is defined
// and never read (a 4-byte `long`), but it can't see a dead byte.
// After asm SSA's versioning, byte 0 and byte 3 of one value are
// separate variables, and when a function uses only the low byte, the
// definition of byte 3 is dead on its own. A `(long)y` cast writes four
// bytes, and when `y` is later used only as `(int)y`, the two high bytes
// are dead.
//
// The pass drops only `Mov(src, Pseudo)`, `Phi(Pseudo, ...)`, and a
// packed BCD chain. The other instructions that define a Pseudo stay.
//   * `LoadAddress(src, Pseudo)`. An unused destination could go, but
//     `src` has its address taken, and without the `LoadAddress` `src`
//     might appear in no instruction, while `replace_pseudoregisters`
//     finds the operands to give frame slots in the instructions.
//   * `Pop(Pseudo)`, which moves the stack. Dropping one would take a
//     matching change of an `AllocateStack`.
//   * `Add(src, Reg(A))`, `Sub`, `And`, `Or`, `Xor`, whose destination is
//     a register, which is outside this pass even when A's value is dead.
//     That would take the liveness of the registers and the flags.
//   * `Inc`, `Dec`, `ASL`, `LSR`, `ROL`, `ROR` of a Pseudo, a
//     read-modify-write whose name SSA construction leaves unversioned.
//
// A `Mov` into A, the usual load, stays too. A is never among the uses
// the pass collects, and the next instruction reads it. The pass takes
// every register to be live.
//
// A packed BCD `DecimalArith` is pure, so it goes when nothing reads a
// byte it writes, and it loses its top bytes when only they aren't
// read, since a byte of the chain depends only on the bytes below it.
#include "passes/optimization_asm/byte_dce.h"

#include <set>
#include <vector>

#include "passes/optimization_asm/ssa_operands.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;

using UseSet = std::set<ByteVar>;

ByteVar key_of(const ir::Pseudo& p) { return {p.name, static_cast<std::int64_t>(p.offset)}; }

// Returns the (name, offset) pairs some instruction uses. Definitions
// don't count.
UseSet collect_pseudo_uses(const ir::Function& fn) {
  UseSet out;
  for (const auto& instr : fn.instructions) {
    ssa_operands::for_each_use_operand(instr, [&](const ir::Type_operand& op) {
      if (const auto* p = as<ir::Pseudo>(op)) {
        out.insert(key_of(*p));
      }
    });
  }
  return out;
}

// Returns whether `op` is a Pseudo destination whose write nothing
// reads. A static's write is seen outside the function, and every byte
// of an excluded name may be: a `LoadAddress(P, _)` lets an indirect
// read through the pointer reach any byte of P, but the uses count only
// byte 0, so without this test the high byte of `int i = -100;` would
// look unused and its store would go.
bool dead_dst(const ir::Type_operand& op, const UseSet& uses, const StaticNames& statics,
              const ExcludedNames& excluded) {
  const auto* p = as<ir::Pseudo>(op);
  return p != nullptr && !is_excluded(p->name, excluded, statics) && !uses.contains(key_of(*p));
}

// Returns whether `instr` can go: a non-volatile `Mov` or a `Phi` whose
// destination is dead, or a packed BCD chain every byte of whose
// destination is. A volatile `Mov` touches memory that is seen whether
// or not its destination is read.
bool is_dead(const ir::Type_instruction& instr, const UseSet& uses, const StaticNames& statics,
             const ExcludedNames& excluded) {
  if (const auto* mov = as<ir::Mov>(instr)) {
    return !mov->is_volatile && dead_dst(mov->dst, uses, statics, excluded);
  }
  if (const auto* da = as<ir::DecimalArith>(instr)) {
    for (const auto& d : da->dst) {
      if (!dead_dst(d, uses, statics, excluded)) {
        return false;
      }
    }
    return true;
  }
  if (const auto* phi = as<ir::Phi>(instr)) {
    return dead_dst(phi->dst, uses, statics, excluded);
  }
  return false;
}

// Drops the top bytes of the chain `instr` whose results nothing reads.
// The chain can stop after the highest byte that is read. A dead byte
// below a live one stays, since its carry is needed.
void drop_dead_top_bytes(ir::DecimalArith& instr, const UseSet& uses, const StaticNames& statics,
                         const ExcludedNames& excluded) {
  std::size_t n = instr.dst.size();
  while (n > 1 && dead_dst(instr.dst[n - 1], uses, statics, excluded)) {
    --n;
  }
  if (n == instr.dst.size()) {
    return;
  }
  instr.left.resize(std::min(n, instr.left.size()));
  instr.right.resize(std::min(n, instr.right.size()));
  instr.dst.resize(n);
}

}  // namespace

ir::Function byte_dce(ir::Function fn, const StaticNames& statics) {
  const ExcludedNames excluded = excluded_pseudo_names(fn);
  while (true) {
    const UseSet uses = collect_pseudo_uses(fn);
    bool changed = false;
    std::vector<ir::Type_instruction> out;
    out.reserve(fn.instructions.size());
    for (auto& instr : fn.instructions) {
      if (is_dead(instr, uses, statics, excluded)) {
        changed = true;
        continue;
      }
      if (auto* da = as<ir::DecimalArith>(instr)) {
        const std::size_t before = da->dst.size();
        drop_dead_top_bytes(*da, uses, statics, excluded);
        changed = changed || da->dst.size() != before;
      }
      out.push_back(std::move(instr));
    }
    fn.instructions = std::move(out);
    if (!changed) {
      return fn;
    }
  }
}

}  // namespace c6502::passes
