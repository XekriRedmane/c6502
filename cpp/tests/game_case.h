#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "compile_case.h"
#include "sim/games.h"

namespace c6502::suite {

enum class Helpers { kSimulators, kRuntimeLibrary };

// Compiles each unit of examples/<example> that exists with `-c -O`,
// links the objects by the example's linker script `script`, with the
// simulator's helper addresses for `Helpers::kSimulators`, and returns
// the executable's path in `tmp`.
std::string build_example(const TempDir& tmp, std::string_view example, const std::vector<std::string>& units,
                          std::string_view script, Helpers helpers);

}  // namespace c6502::suite
