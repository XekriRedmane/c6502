// Dead static elimination, a pass over the program that drops the
// internal-linkage `StaticVariable`s nothing names. It goes with the
// folding of constants.
//
// - The TAC pass `_fold_indexed_load` rewrites `IndexedLoad(name,
//   ConstInt(idx), dst)` as `Copy(Constant(v), dst)` when `name` is a
//   const array, so every use of the array's bytes becomes an
//   immediate, and the `StaticVariable` stays.
// - `fold_const_statics` does the same for const scalar statics and
//   drops the storage when the rewrite covers every use. It leaves
//   arrays alone, whose uses are rewritten on the TAC.
// - After the unroller, the index of each array reference in a loop
//   body is a constant, so the TAC pass folds every one of them, and
//   the array's `StaticVariable` is left with nothing naming it until
//   this pass drops it.
//
// A static is dropped when all three of these are true.
//
// - Its `is_global` is false, for internal linkage. A static of
//   external linkage (anything not declared `static`) may be read by
//   another unit at link time, so it is kept.
// - No instruction of a function names it in an operand (`Pseudo`,
//   `Data`, `IndexedData`, `ImmLabelLow`, or `ImmLabelHigh`).
// - No other static's `init` names it in an `AddressInit` or an
//   `AddressByteInit`.
//
// The optimizer runs it after the SSA round trip of every function,
// since that round trip's dead code elimination can drop the last load
// of a static, which a run before it would miss.
//
// The walk over an instruction follows Python's, which visits each
// field that holds an operand and each list of operands. That includes
// the `src` of a `BitTest` and a `Phi`'s `dst`, and leaves out the
// sources of a `Phi`'s arguments, which are inside `AsmPhiArg`s. No
// `Phi` is left once the round trip has run.
#include "passes/optimization_asm/dead_static.h"

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/optimization_asm/all_operands.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using Names = std::set<std::string, std::less<>>;

void scan_operand(const ir::Type_operand& op, Names& names) {
  using K = ir::Kind_operand;
  switch (ir::kind(op)) {
    case K::Pseudo:
      names.insert(std::get<ir::Pseudo>(op).name);
      break;
    case K::Data:
      names.insert(std::get<ir::Data>(op).name);
      break;
    case K::IndexedData:
      names.insert(std::get<ir::IndexedData>(op).name);
      break;
    case K::ImmLabelLow:
      names.insert(std::get<ir::ImmLabelLow>(op).name);
      break;
    case K::ImmLabelHigh:
      names.insert(std::get<ir::ImmLabelHigh>(op).name);
      break;
    default:
      break;
  }
}

void scan_instruction(const ir::Type_instruction& instr, Names& names) {
  if (const auto* phi = as<ir::Phi>(instr)) {
    scan_operand(phi->dst, names);
    return;
  }
  if (const auto* bit = as<ir::BitTest>(instr)) {
    scan_operand(bit->src, names);
    return;
  }
  for (const ir::Type_operand* op : optimizer::all_operands_in(instr)) {
    scan_operand(*op, names);
  }
}

Names collect_referenced_names(const ir::Program& prog) {
  Names names;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      for (const auto& instr : fn->instructions) {
        scan_instruction(instr, names);
      }
    } else {
      for (const auto& init : std::get<ir::StaticVariable>(tl).init) {
        if (const auto* a = as<ir::AddressInit>(init)) {
          names.insert(a->name);
        } else if (const auto* b = as<ir::AddressByteInit>(init)) {
          names.insert(b->name);
        }
      }
    }
  }
  return names;
}

}  // namespace

asm_ast::Program apply_dead_static_elimination(asm_ast::Program prog) {
  const Names referenced = collect_referenced_names(prog);
  std::vector<ir::Type_top_level> top;
  top.reserve(prog.top_level.size());
  for (auto& tl : prog.top_level) {
    if (const auto* sv = as<ir::StaticVariable>(tl)) {
      if (!sv->is_global && !referenced.contains(sv->name)) {
        continue;
      }
    }
    top.push_back(std::move(tl));
  }
  prog.top_level = std::move(top);
  return prog;
}

}  // namespace c6502::passes
