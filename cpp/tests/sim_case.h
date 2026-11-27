#pragma once

#include <cstdint>
#include <string>

#include "sim/harness.h"

namespace c6502::suite {

struct SimOptions {
  bool optimize = false;
  bool inline_calls = true;
  bool check_ir = true;
  std::int64_t origin = sim::kDefaultOrigin;
};

// Compiles `source` as sim/harness.py's `compile_to_asm` does, with no
// preprocessor arguments and no file name, and loads it with the
// runtime, ready to run from the boot stub. Throws `std::runtime_error`
// with the error's class and message when the program is rejected.
sim::Simulation build_sim(const std::string& source, const SimOptions& options = {});
sim::Simulation build_sim(const std::string& source, bool optimize);

sim::SimResult run_c_program(const std::string& source, bool optimize = false,
                             std::uint64_t max_cycles = sim::kDefaultMaxCycles);

sim::Simulation build_linked_sim(const std::string& path);

}  // namespace c6502::suite
