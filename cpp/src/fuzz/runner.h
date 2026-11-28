// Runs a C program on the three execution paths and compares main's
// return.
//
//   * `tac`, the TAC interpreter (src/tac_sim.h) on the unoptimized TAC;
//   * `unopt`, the simulator on the program compiled as `-S` compiles it;
//   * `opt`, the simulator on the program compiled with `-O`.
//
// Each result is main's return as a signed 16-bit int, or the text
// `ERR <class>: <message>` when the path raised (a rejection by the
// compiler, an assembler error, a failed IR check, or a hook's or the
// CPU's error), with the message cut to 120 characters, or `TIMEOUT`. A
// rejection is named by the Python class it stands for, a failed IR
// check or another compiler bug is `InternalError`, and the CPU's error
// is `CpuError`. The IR checks are on in every
// path, so a pass that breaks the IR fails there even when the
// program's result comes out right.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace c6502::fuzz {

// The cycles after which a run of the simulator counts as `TIMEOUT`.
inline constexpr std::uint64_t kMaxCycles = 20'000'000;

enum class Mode { kTac, kUnopt, kOpt };
inline constexpr Mode kModes[] = {Mode::kTac, Mode::kUnopt, Mode::kOpt};
const char* mode_name(Mode m);

// main's return, or the text of the path's error or of its timeout.
using Result = std::variant<std::int64_t, std::string>;

struct Results {
  std::array<Result, 3> by_mode;
  const Result& operator[](Mode m) const { return by_mode[static_cast<std::size_t>(m)]; }
  Result& operator[](Mode m) { return by_mode[static_cast<std::size_t>(m)]; }
  bool operator==(const Results&) const = default;
};

class Runner {
 public:
  // `include_dir` is c6502's include/, which the preprocessor searches.
  explicit Runner(std::string include_dir);

  Result run_tac(const std::string& src) const;
  Result run_asm(const std::string& src, bool optimize) const;
  Results run_all(const std::string& src) const;

  const std::string& include_dir() const { return include_dir_; }

 private:
  std::string include_dir_;
};

bool is_error(const Result& r);

// Returns whether `r` is an error of a backend limit rather than a
// miscompile: the soft-stack frame is addressed through `(FP),Y`, so a
// frame over 253 bytes, or an offset past 255, can't be built.
bool is_backend_limit(const Result& r);

// Returns the paths whose result is wrong. With `expected` none, the TAC
// result is the reference and only the asm paths can be wrong, and an
// error on the TAC path makes every path count as wrong, since there is
// no reference.
std::vector<Mode> disagreements(const Results& res, std::optional<std::int64_t> expected);

// Returns `tac=<r> unopt=<r> opt=<r>`.
std::string format_result(const Results& res);

// Returns `v` as a signed 16-bit int, as main's `int` return.
std::int64_t s16(std::int64_t v);

// Returns the N of the program's first `// expect: N` line, as a signed
// 16-bit int, or none.
std::optional<std::int64_t> expected_of(const std::string& src);

}  // namespace c6502::fuzz
