#pragma once

#include <string>
#include <vector>

#include "c99_ast.h"
#include "passes/codegen.h"
#include "passes/type_checking.h"
#include "passes/zp_globals.h"
#include "tac_ast.h"

namespace c6502::passes {

// The tables and values of a compile that the stage reads besides the
// TAC: the type-checked c99 program, which `c99_to_tac` changes and
// `select_abi` and `fixed_address_layouts` read, the symbol table, which
// the stage changes, the type table, every name of static storage
// before the TAC optimizer ran, which `codegen` sets, and the HARGS
// footprints of the runtime helpers.
struct StageTables {
  c99_ast::Program c99;
  SymbolTable symbols;
  TypeTable types;
  NameSet statics;
  HelperFootprints footprints;
};

struct SymbolChanges {
  std::vector<std::string> reg_locals;
  std::vector<std::string> carry;
  std::vector<NewFunction> new_functions;
};

// What a run of the stage gives. With `emit`, `emitted` holds the `-S`
// text and the finished asm and asm2 programs. Without it, the stage
// stops where `optimized_asm` (or the simulator harness's unoptimized
// path) does, and `emitted.asm_prog` is the finished asm program, which
// the harness assembles with `harness_symbols`, the slot symbols and the
// unit's `EQU` bindings, or the pinned slots and the `EQU` bindings
// without -O.
struct StageResult {
  EmitResult emitted;
  OrderedSymbolAddrs harness_symbols;
  SymbolChanges changes;
  // The TAC that code generation lowered, which `codegen` sets.
  tac_ast::Program tac;
};

// What a compile asks of `codegen` besides its tables: whether `-O` is
// on, whether `-O` inlines the calls of small `static` functions
// (tests/no_inlining.py turns that off), whether the unoptimized text
// carries the link metadata `-c` assembles, and whether the stage goes
// on to the text or stops where the simulator harness takes the
// program.
struct CodegenOptions {
  bool optimize = false;
  bool inline_calls = true;
  bool link_metadata = false;
  bool emit = true;
};

// Runs code generation under `-O`: `plan_zero_page` on
// `unoptimized_tac`, the TAC from `translate_for_codegen`, then
// `optimized_front` on `tac`, the TAC after `apply_split_tables`, the
// link metadata of the TAC, the ABI, and the symbol table, and the rest
// of `optimized_asm`.
StageResult optimized_stage(const tac_ast::Program& unoptimized_tac, const tac_ast::Program& tac,
                            StageTables& tables, bool emit);

// Runs code generation without `-O` on `tac`: `plan_zero_page`,
// `fixed_address_layouts`, `tac_to_asm` with that ABI,
// `replace_pseudoregisters`, and the unoptimized tail and emission,
// with the link metadata `-c` assembles when `link_metadata` is set.
StageResult unoptimized_stage(const tac_ast::Program& tac, StageTables& tables,
                              bool link_metadata, bool emit);

// Runs code generation from the type-checked c99 program `tables.c99`:
// `translate_for_codegen`, then under `-O` `optimized_tac` and
// `optimized_stage`, which plans the zero page on the TAC from before
// those passes, and without it `unoptimized_stage`. `tables.statics` is
// set to the names of static storage after `translate_for_codegen`.
StageResult codegen(StageTables& tables, const CodegenOptions& options);

}  // namespace c6502::passes
