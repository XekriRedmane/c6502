// The port of passes/redundant_compare.py.
#pragma once

#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

// Drops each compare whose flags an identical compare on the only path
// into it already set, each compare with 0 whose flags the instruction
// that set the register already set, and rewrites the compares and
// branches on C that the comment at the top of redundant_compare.cpp
// describes, in every function. `ctx` gives `may_alias` the compile's
// zero-page globals.
asm_ast::Program apply_redundant_compare(asm_ast::Program prog, const AsmContext& ctx);

namespace redundant_compare {

// Python's `_A`, the operand `Reg(A)`.
extern const asm_ast::Type_operand kA;

// The Branch and Jump instructions that target each label, by the
// label's name (Python's `_label_refs`). A label named by an
// `ImmLabelLow` / `ImmLabelHigh` operand gets a -1 entry, which no
// caller accepts. `table_targets` holds the labels a `JumpTable` names,
// which Python keeps in the same dict under the key `_TABLE_TARGETS`;
// it is empty in a map from `label_refs`.
struct LabelRefs {
  std::map<std::string, std::vector<long>, std::less<>> refs;
  std::set<std::string, std::less<>> table_targets;

  // Returns the references to `name`, or null when there are none.
  const std::vector<long>* get(std::string_view name) const;
};

// Returns the references of `label_refs` without the table targets.
LabelRefs label_refs(Instrs instrs);

// Returns the references of `label_refs` with the labels each
// `JumpTable` names in `table_targets` (Python's `_refs_with_tables`).
LabelRefs refs_with_tables(Instrs instrs);

// Returns whether N and Z at `instrs[c]` were set from the current
// value of `reg`, a `Reg` operand, by the instruction that last wrote
// it, with only stores in between (Python's `_flags_from`).
//
// With `refs`, the walk also passes over conditional branches, which
// leave registers and flags alone, and follows a label to every way
// in: each branch or jump to it, and the fall-through from the
// instruction before it. Every one of them must have set the flags
// from `reg`. A label it meets twice, a label a `JumpTable` or a label
// operand names, and the function's start all count as not. In `je`,
// `LDA #$00; BEQ end` and `LDA #$01` both reach `end: STA equal`, so the
// `ORA #$00` after the store tests what the loads set.
bool flags_from(Instrs instrs, std::size_t c, const asm_ast::Type_operand& reg,
                const LabelRefs* refs = nullptr);

}  // namespace redundant_compare

}  // namespace c6502::passes
