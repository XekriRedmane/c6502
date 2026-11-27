#include "sim_case.h"

#include <stdexcept>

#include "compile_case.h"
#include "errors.h"
#include "passes/codegen_stage.h"
#include "passes/front_end.h"
#include "passes/ir_check.h"
#include "passes/zp_globals.h"

namespace c6502::suite {

sim::Simulation build_sim(const std::string& source, const SimOptions& options) {
  const passes::ir_check::Enabled checks(options.check_ir);
  passes::FrontEndOptions front;
  front.include_dir = repo_path("include");
  front.optimize = options.optimize;
  passes::StageResult result;
  try {
    passes::StageTables tables;
    std::string err;
    std::vector<diagnostics::CompileWarning> warnings;
    {
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
    result = passes::codegen(tables, codegen);
  } catch (const CompileError& e) {
    throw std::runtime_error(e.python_type() + ": " + e.what());
  }
  return sim::build_sim(result.emitted.asm_prog, result.harness_symbols, options.origin);
}

sim::Simulation build_sim(const std::string& source, bool optimize) {
  SimOptions options;
  options.optimize = optimize;
  return build_sim(source, options);
}

sim::SimResult run_c_program(const std::string& source, bool optimize, std::uint64_t max_cycles) {
  return build_sim(source, optimize).run(max_cycles);
}

sim::Simulation build_linked_sim(const std::string& path) { return sim::build_linked_sim(read_file(path)); }

}  // namespace c6502::suite
