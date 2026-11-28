#include "sim/source_sim.h"

#include <utility>
#include <vector>

#include "diagnostics.h"
#include "passes/codegen_stage.h"
#include "passes/front_end.h"
#include "passes/ir_check.h"
#include "passes/zp_globals.h"

namespace c6502::sim {

Simulation build_source_sim(const std::string& source, const SourceOptions& options,
                            std::string& err) {
  const passes::ir_check::Enabled checks(options.check_ir);
  passes::FrontEndOptions front;
  front.include_dir = options.include_dir;
  front.optimize = options.optimize;
  passes::StageTables tables;
  {
    std::vector<diagnostics::CompileWarning> warnings;
    diagnostics::Collecting block(warnings);
    passes::FrontEndResult checked = passes::front_end(source, front, err);
    tables.c99 = std::move(checked.prog);
    tables.symbols = std::move(checked.symbols);
    tables.types = std::move(checked.types);
  }
  tables.footprints = passes::helper_footprints();
  passes::CodegenOptions codegen;
  codegen.optimize = options.optimize;
  codegen.inline_calls = options.inline_calls;
  codegen.emit = false;
  const passes::StageResult result = passes::codegen(tables, codegen);
  return build_sim(result.emitted.asm_prog, result.harness_symbols, options.origin);
}

}  // namespace c6502::sim
