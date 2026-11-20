// Asm-level copy propagation on the SSA form.
//
// This is the asm counterpart of `passes.optimization.copy_propagation`
// (TAC), with byte-granular (name, offset) keys and the asm
// instructions.
//
// A copy is a `Mov(src, Pseudo dst)` where
//   * `dst` is a Pseudo that `to_ssa` versioned, or a parameter byte
//     that is still promotable, which is to say that its name is neither
//     in `statics` nor among the names whose address is taken or that a
//     read-modify-write targets. Such a Pseudo has one SSA definition,
//     so `dst` equals `src` at every use of `dst`.
//   * `src` is an `Imm`, `ImmLabelLow`, or `ImmLabelHigh`, which never
//     change (literal values and link-time addresses), or a Pseudo whose
//     name passes the same test, whose value SSA keeps stable from the
//     definition of `dst` to any use. Any other source (`Reg`, `Stack`,
//     `Frame`, `Data`, `ZP`, `Indirect`) is a mutable cell that other
//     instructions can write between the definition and the use, so
//     propagating its value would read a stale one.
//
// A renamed Pseudo always has offset 0, since the byte's position is in
// its name (`<orig>.b<k>.vN`), and the parameters, statics, and
// address-taken Pseudos that keep their names keep their offsets. Both
// are keyed by (name, offset).
//
// The pass handles only a direct `Mov(src, dst)`. It doesn't take the
// two moves `Mov src->A; Mov A->Pseudo` for a copy, since following A
// across the instructions that write it is outside its scope.
// `tac_to_asm` sends every write of a Pseudo through A, so direct
// moves between Pseudos are rare in its output, but SSA destruction
// writes them on the edges, and the pass serves any pass that writes
// them earlier.
//
// A `Phi` merges several values, so it is no copy, and its destination
// never enters the map.
//
// The algorithm is the TAC pass's.
//   1. Record `copy_src[(dst.name, dst.offset)] = src` for every copy.
//   2. Follow each entry whose source is another copy's destination, to
//      a base value: an immediate, a label's half, or a Pseudo that isn't
//      in the map. SSA has no cycles.
//   3. Replace each use of a Pseudo in the map with its base value, in
//      every instruction, `Phi` arguments included. Definitions stay,
//      since they are the SSA names the values come from.
//
// After that the copy's `Mov` is a dead store, since nothing reads its
// destination, and `byte_dce` drops it in the next round of the fixed
// point.
//
// The pass is sound only on SSA form, since outside it a later `Mov` could
// define the Pseudo again. The optimizer runs `to_ssa`, then this pass
// and `byte_dce` and the others, and then `from_ssa`, so its input is
// in SSA form.
#include "passes/optimization_asm/copy_propagation.h"

#include <map>
#include <set>
#include <vector>

#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

ByteVar key_of(const ir::Pseudo& p) { return {p.name, static_cast<std::int64_t>(p.offset)}; }

using CopyMap = std::map<ByteVar, ir::Type_operand>;

// Returns the copies' sources by destination. Statics are left out as
// both destinations and sources: a write of one is seen outside the
// function, and a call between the copy and the use can change one.
CopyMap collect_copy_sources(const ir::Function& fn, const ExcludedNames& excluded,
                             const StaticNames& statics) {
  CopyMap out;
  for (const auto& instr : fn.instructions) {
    const auto* mov = as<ir::Mov>(instr);
    if (mov == nullptr) {
      continue;
    }
    const auto* dst = as<ir::Pseudo>(mov->dst);
    if (dst == nullptr || is_excluded(dst->name, excluded, statics)) {
      continue;
    }
    if (isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(mov->src)) {
      out.insert_or_assign(key_of(*dst), mov->src);
      continue;
    }
    if (const auto* src = as<ir::Pseudo>(mov->src);
        src != nullptr && !is_excluded(src->name, excluded, statics)) {
      out.insert_or_assign(key_of(*dst), mov->src);
    }
  }
  return out;
}

// Follows each entry whose source is another copy's destination to a
// base value. SSA has no cycles, but the walk stops at one rather than
// loop if a malformed input has one.
CopyMap resolve_chains(const CopyMap& copy_src) {
  CopyMap resolved;
  for (const auto& [key, first] : copy_src) {
    std::set<ByteVar> seen;
    const ir::Type_operand* cur = &first;
    while (const auto* p = as<ir::Pseudo>(*cur)) {
      const ByteVar cur_key = key_of(*p);
      const auto it = copy_src.find(cur_key);
      if (it == copy_src.end() || seen.contains(cur_key)) {
        break;
      }
      seen.insert(cur_key);
      cur = &it->second;
    }
    resolved.emplace(key, *cur);
  }
  return resolved;
}

// Rewrites each use of a Pseudo in `instr` to its base value. The
// definitions stay. `LoadAddress.src` names the storage cell whose
// address the instruction takes, and propagating a literal into it would
// lose the address, so it stays too.
void rewrite(ir::Type_instruction& instr, const CopyMap& resolved) {
  const auto sub = [&](ir::Type_operand& op) {
    if (const auto* p = as<ir::Pseudo>(op)) {
      if (const auto it = resolved.find(key_of(*p)); it != resolved.end()) {
        op = it->second;
      }
    }
  };
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith: {
      // The propagated values, constants and SSA Pseudos, are all
      // operand forms the chain takes.
      auto& i = std::get<ir::DecimalArith>(instr);
      for (auto& op : i.left) {
        sub(op);
      }
      for (auto& op : i.right) {
        sub(op);
      }
      break;
    }
    case ir::Kind_instruction::Mov:
      sub(std::get<ir::Mov>(instr).src);
      break;
    // The destination of an ALU instruction is read and written. It is
    // A in what `tac_to_asm` writes, so substituting there does nothing,
    // but the IR allows a Pseudo.
    case ir::Kind_instruction::Add: {
      auto& i = std::get<ir::Add>(instr);
      sub(i.src);
      sub(i.dst);
      break;
    }
    case ir::Kind_instruction::Sub: {
      auto& i = std::get<ir::Sub>(instr);
      sub(i.src);
      sub(i.dst);
      break;
    }
    case ir::Kind_instruction::And: {
      auto& i = std::get<ir::And>(instr);
      sub(i.src);
      sub(i.dst);
      break;
    }
    case ir::Kind_instruction::Or: {
      auto& i = std::get<ir::Or>(instr);
      sub(i.src);
      sub(i.dst);
      break;
    }
    case ir::Kind_instruction::Xor: {
      auto& i = std::get<ir::Xor>(instr);
      sub(i.src1);
      sub(i.src2);
      break;
    }
    case ir::Kind_instruction::Push:
      sub(std::get<ir::Push>(instr).src);
      break;
    case ir::Kind_instruction::Compare: {
      auto& i = std::get<ir::Compare>(instr);
      sub(i.left);
      sub(i.right);
      break;
    }
    case ir::Kind_instruction::Inc:
      sub(std::get<ir::Inc>(instr).dst);
      break;
    case ir::Kind_instruction::Dec:
      sub(std::get<ir::Dec>(instr).dst);
      break;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      sub(std::get<ir::ArithmeticShiftLeft>(instr).dst);
      break;
    case ir::Kind_instruction::LogicalShiftRight:
      sub(std::get<ir::LogicalShiftRight>(instr).dst);
      break;
    case ir::Kind_instruction::RotateLeft:
      sub(std::get<ir::RotateLeft>(instr).dst);
      break;
    case ir::Kind_instruction::RotateRight:
      sub(std::get<ir::RotateRight>(instr).dst);
      break;
    case ir::Kind_instruction::Phi:
      for (auto& a : std::get<ir::Phi>(instr).args) {
        sub(std::get<ir::AsmPhiArg>(a).source);
      }
      break;
    default:
      // `Pop`, `Call`, the jumps, labels, and returns, the carry
      // instructions, `AllocateStack`, and `FunctionPrologue` use no
      // Pseudo.
      break;
  }
}

}  // namespace

ir::Function copy_propagate(ir::Function fn, const StaticNames& statics) {
  const ExcludedNames excluded = excluded_pseudo_names(fn);
  const CopyMap copy_src = collect_copy_sources(fn, excluded, statics);
  if (copy_src.empty()) {
    return fn;
  }
  const CopyMap resolved = resolve_chains(copy_src);
  for (auto& instr : fn.instructions) {
    rewrite(instr, resolved);
  }
  return fn;
}

}  // namespace c6502::passes
