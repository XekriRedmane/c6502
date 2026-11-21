// The port of passes/optimization/extend_add_chain.py, the walk back
// from a Load's or a Store's address through `Binary(Add, base, %ext)`
// and `%ext := ZeroExtend(idx)` that the three addressing-mode
// recognizers share, and the width predicates `is_1_byte_var` and
// `is_1_byte_val`, which constant folding's `distribute_constant` reads
// too.
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "ordered_map.h"
#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// The use counts of a function's names (`count_uses`), which a
// recognizer computes once per run, as Python's `prepare_extra` does,
// and reads as `env.extra`. A name not in it is read nowhere.
using UseCounts = OrderedMap<std::string, std::size_t>;

// Returns how many times `uses` counts `name`, 0 when it isn't there.
std::size_t use_count(const UseCounts& uses, std::string_view name);

// A recognized `extend -> add -> use` chain.
struct ExtendAddChain {
  // The Add's operand other than `%ext`: a Constant for the absolute,X
  // recognizers, a pointer Var for the indirect one.
  tac_ast::Type_val base;
  // The 1-byte index, the extend's source.
  tac_ast::Var idx_var;
  // The Add's destination, which is read once and can be dropped.
  tac_ast::Var addr_var;
  // The extend's destination.
  tac_ast::Var ext_var;
  // Whether the Add is `ext_var`'s only reader, so that its definition
  // can go too. Common-subexpression elimination shares one extend
  // among several subscripts with the same index, and then each chain
  // is rewritten but the extend stays for the others, which dead-store
  // elimination drops once the last one is gone.
  bool ext_droppable = true;

  // Returns the names of the definitions a recognizer drops with its
  // rewrite: the Add's, and the extend's when it is droppable.
  std::vector<std::string> drop_defs() const;
};

// Walks back from `addr_var`, a Load's or a Store's address, through
// `Binary(Add, base, %ext)` and `%ext := ZeroExtend(idx)`, with the
// gates the three recognizers share: a symbol table, `addr_var` and
// `%ext` SSA-renamed, `addr_var` read once (unless `single_use` is
// false, when the caller checks its other readers), the extend a
// ZeroExtend, possibly through up to four Copies, and `idx` a 1-byte
// Var. Returns the chain, or none when a gate fails. The caller checks
// the kind of `base` and emits its own instruction.
std::optional<ExtendAddChain> recognize_extend_add_chain(const tac_ast::Var& addr_var,
                                                         const framework::DefUseEnv& env,
                                                         const framework::PassContext& ctx,
                                                         const UseCounts& uses,
                                                         bool single_use = true);

// Returns whether `v`'s c99 type, without its qualifiers, is a 1-byte
// scalar (`char`, `signed char`, or `unsigned char`). A null `symbols`
// or a Var with no symbol gives false.
bool is_1_byte_var(const tac_ast::Var& v, const SymbolTable* symbols);

// Returns whether `v` is a 1-byte value: a Var whose type
// `is_1_byte_var` accepts, or a `ConstChar` or `ConstUChar` constant.
bool is_1_byte_val(const tac_ast::Type_val& v, const SymbolTable* symbols);

// Returns the name of every Var that an instruction of `fn` writes in
// its `dst` field, which the recognizers' entry points for the unit
// tests take as the SSA names.
NameSet all_dsts(const tac_ast::Function& fn);

}  // namespace c6502::passes
