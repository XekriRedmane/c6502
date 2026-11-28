#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace c6502::fuzz {

// A generator's program for one seed, and what its `main` returns when
// the generator knows, or none when the TAC interpreter's result is the
// reference.
struct GeneratedProgram {
  std::string source;
  std::optional<std::int64_t> expected;
  bool operator==(const GeneratedProgram&) const = default;
};

}  // namespace c6502::fuzz
