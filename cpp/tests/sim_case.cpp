#include "sim_case.h"

#include <stdexcept>

#include "compile_case.h"
#include "errors.h"
#include "sim/source_sim.h"

namespace c6502::suite {

sim::Simulation build_sim(const std::string& source, const SimOptions& options) {
  sim::SourceOptions o;
  o.include_dir = repo_path("include");
  o.optimize = options.optimize;
  o.inline_calls = options.inline_calls;
  o.check_ir = options.check_ir;
  o.origin = options.origin;
  std::string err;
  try {
    return sim::build_source_sim(source, o, err);
  } catch (const CompileError& e) {
    throw std::runtime_error(e.python_type() + ": " + e.what());
  }
}

sim::Simulation build_sim(const std::string& source, bool optimize) {
  SimOptions options;
  options.optimize = optimize;
  return build_sim(source, options);
}

sim::Simulation build_sim(const std::string& source, Pipeline pipeline) {
  return build_sim(source, optimized(pipeline));
}

PipelineReturns both_paths(const std::string& source) {
  return {build_sim(source, Pipeline::kUnoptimized).run().return_int_signed(),
          build_sim(source, Pipeline::kOptimized).run().return_int_signed()};
}

sim::SimResult run_c_program(const std::string& source, bool optimize, std::uint64_t max_cycles) {
  return build_sim(source, optimize).run(max_cycles);
}

sim::Simulation build_linked_sim(const std::string& path) { return sim::build_linked_sim(read_file(path)); }

}  // namespace c6502::suite
