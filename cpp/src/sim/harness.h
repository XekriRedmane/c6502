// The simulator harness, ported from sim/harness.py and sim/linked.py:
// a program assembled with the runtime (sim/runtime.h), or a linked
// executable loaded at its segments' addresses, run on the CPU
// (sim/cpu.h) from the boot stub until the boot stub's BRK or a limit
// on the cycles, with the hooks run at their trap addresses.
#pragma once

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "asm_ast.h"
#include "passes/prune_unused_slots.h"
#include "sim/cpu.h"
#include "sim/runtime.h"

namespace c6502::sim {

inline constexpr std::int64_t kDefaultOrigin = 0x0800;
inline constexpr std::uint64_t kDefaultMaxCycles = 10'000'000;

// The state when a run stopped at the boot stub's BRK, or at the limit
// on the cycles, which sets `timed_out`.
struct SimResult {
  std::uint8_t a = 0;
  std::uint8_t x = 0;
  std::uint8_t y = 0;
  std::uint16_t pc = 0;
  std::uint8_t sp = 0;
  std::uint64_t cycles = 0;
  std::vector<std::uint8_t> memory;
  bool timed_out = false;
  std::uint16_t hargs = 0x04;

  // A return of one byte is in A, and one of two bytes or a pointer in
  // A and X, low byte first.
  std::int64_t return_char() const { return a; }
  std::int64_t return_char_signed() const { return static_cast<std::int8_t>(a); }
  std::int64_t return_int() const { return a | (x << 8); }
  std::int64_t return_int_signed() const { return static_cast<std::int16_t>(return_int()); }
  std::int64_t return_pointer() const { return return_int(); }
  // A return of 4 bytes is at HARGS+8 and one of 8 bytes at HARGS+16.
  std::uint64_t return_long() const;
  std::int64_t return_long_signed() const { return static_cast<std::int32_t>(return_long()); }
  std::uint64_t return_longlong() const;
  std::int64_t return_longlong_signed() const { return static_cast<std::int64_t>(return_longlong()); }

  // Returns the `size` bytes at `addr`, low byte first.
  std::uint64_t word(std::int64_t addr, int size = 2) const;
};

// A program loaded with its runtime, ready to run from the boot stub.
class Simulation {
 public:
  Cpu cpu;
  std::map<std::string, std::int64_t> symbols;
  Runtime runtime;
  std::int64_t origin = kDefaultOrigin;
  std::int64_t code_end = kDefaultOrigin;
  std::set<std::string> function_names;

  // Takes one step: runs the hook at PC and returns as RTS does, or
  // executes the instruction at PC. Returns false, and does nothing,
  // when the byte at PC is $00.
  bool step();

  // Steps until the byte at PC is $00 or the cycles reach `max_cycles`,
  // with the hooks of `runtime.hooks` as they are when it starts.
  // Throws the `HookError` of a hook that fails and the `CpuError` of a
  // step py65 can't take.
  SimResult run(std::uint64_t max_cycles = kDefaultMaxCycles);

  // Returns the address of the symbol `name`, and throws
  // `std::out_of_range` naming it when the program has none.
  std::int64_t symbol(std::string_view name) const;
};

// Assembles `program` at `origin` with the runtime for the layout that
// `symbols` give (their SSP, FP, HARGS, and DPTR, or the classic ones),
// with the runtime's helpers and `symbols` as extra symbols, and loads
// it, as sim/harness.py's `build_sim` did with the program and the
// symbols the code generation stage returns. Throws
// `std::invalid_argument` when the program has no `main`.
Simulation build_sim(const asm_ast::Program& program, const passes::OrderedSymbolAddrs& symbols,
                     std::int64_t origin = kDefaultOrigin);

// Loads the ELF executable `exe` at its segments' load addresses with
// the runtime for its layout, as sim/linked.py's `build_linked_sim`
// does. Its symbols are the executable's defined ones, a global one
// where a name is both, and the runtime's helpers it doesn't define
// itself. Throws `std::runtime_error` when it isn't an executable, has
// no `main`, or gives a helper an address other than the runtime's.
Simulation build_linked_sim(std::string_view exe, std::int64_t origin = kDefaultOrigin);

// The address of every runtime helper, the same for every layout, for
// linking a program the simulator runs (`--defsym`), as sim/linked.py's
// `runtime_defsyms` gives them.
std::map<std::string, std::int64_t> runtime_defsyms();
// `runtime_defsyms` as `--defsym=NAME=0xADDR` arguments, in name order.
std::vector<std::string> defsym_args();

}  // namespace c6502::sim
