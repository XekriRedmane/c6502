// The code generation stage of compile.py from the TAC to the `-S` text
// in one call: `plan_zero_page`, the `-O` pipeline of `_optimized_front`
// after the TAC optimizer (or the unoptimized pipeline's code
// generation), the back end's region, and emission, which
// `c6502cpp backend`'s stage modes run for compile.py.
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
// TAC: the type-checked c99 program, which `select_abi` and
// `fixed_address_layouts` read, the symbol table, which the stage
// changes as Python's passes do, the type table, every name of static
// storage before the TAC optimizer ran (`_codegen`'s `statics`), and the
// HARGS footprints of the runtime helpers, which Python measures from
// rtlib/helpers.py.
struct StageTables {
  c99_ast::Program c99;
  SymbolTable symbols;
  TypeTable types;
  NameSet statics;
  HelperFootprints footprints;
};

// What the stage changed in the symbol table that Python enters in its
// own: the `<param>.reg` locals `tac_to_asm` added, in order, the
// functions the carry-return choices gave the carry-flag return, and the
// functions the region made, each with the function whose symbol it
// copies.
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
// unit's `EQU` bindings (`{**opt.slot_symbols, **zp.equs()}`, or the
// pinned slots and `zp.equs()` without -O).
struct StageResult {
  EmitResult emitted;
  OrderedSymbolAddrs harness_symbols;
  SymbolChanges changes;
};

// Runs compile.py's code generation under `-O`: `plan_zero_page` on
// `unoptimized_tac`, the TAC from `translate_for_codegen`, then
// `_optimized_front` after the TAC optimizer on `tac`, the TAC after
// `apply_split_tables`, the link metadata of the TAC, the ABI, and the
// symbol table, and the rest of `optimized_asm` and `_codegen`.
StageResult optimized_stage(const tac_ast::Program& unoptimized_tac, const tac_ast::Program& tac,
                            StageTables& tables, bool emit);

// Runs compile.py's code generation without `-O` on `tac`:
// `plan_zero_page`, `fixed_address_layouts`, `tac_to_asm` with that ABI,
// `replace_pseudoregisters`, and the unoptimized tail and emission, with
// the link metadata `-c` assembles when `link_metadata` is set.
StageResult unoptimized_stage(const tac_ast::Program& tac, StageTables& tables,
                              bool link_metadata, bool emit);

}  // namespace c6502::passes
