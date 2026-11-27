// What the cases of the three games share (test_zork_interpreter_sim.cpp,
// test_lode_runner_sim.cpp, and test_ali_baba_sim.cpp): the build of a
// multi-unit example as its README links it, and the games' machines
// and scripts of sim/games.h.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "compile_case.h"
#include "sim/games.h"

namespace c6502::suite {

// Whether the defsyms of the simulator's runtime helpers go in a link.
enum class Helpers { kSimulators, kRuntimeLibrary };

// Compiles each unit of examples/<example> that exists with `-c -O`,
// links the objects by the example's linker script `script`, with the
// simulator's helper addresses for `Helpers::kSimulators`, and returns
// the executable's path in `tmp`.
std::string build_example(const TempDir& tmp, std::string_view example, const std::vector<std::string>& units,
                          std::string_view script, Helpers helpers);

}  // namespace c6502::suite
