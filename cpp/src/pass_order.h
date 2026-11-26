// The order in which the optimizer's drivers run their passes, for
// tests/test_optimization_docs.py, which checks the pipeline order of
// docs/optimizations/README.md against it (`c6502cpp pass-order`).
//
// Each run of a driver opens a `Run` with the driver's name, and reports
// each pass it runs with `ran`, in the order it runs them. The peephole
// fixed point (`run_to_fixed_point`) and the TAC optimizer's driver
// report their tables before each run of them, so that a pass is
// reported once however many sweeps the fixed point makes, and the asm
// optimizer reports the passes of its loop of copy propagation before
// the loop. The other drivers report each pass beside its call. Only the
// first run of each driver is kept, which gives its whole order, and a
// pass that a driver runs twice, as the region of `optimized_asm` runs
// `counter_to_index_reg`, is listed twice. Nothing is kept unless a
// `Recording` is open on the thread, so a compile pays one test of a
// pointer for each report.
#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace c6502::pass_order {

// The passes of the first run of each driver, in the order they ran,
// with the drivers in the order their first runs began.
struct Order {
  std::vector<std::pair<std::string, std::vector<std::string>>> drivers;
};

// Records that the innermost open `Run` of the calling thread runs
// `pass` next, when a `Recording` is open on the thread.
void ran(std::string_view pass);

// Marks one run of the driver `driver` for the lifetime of the block.
// The reports inside it, apart from those of a `Run` nested in it, are
// the driver's.
class Run {
 public:
  explicit Run(std::string_view driver);
  ~Run();
  Run(const Run&) = delete;
  Run& operator=(const Run&) = delete;

 private:
  bool open_ = false;
};

// Keeps the reports of the calling thread in `order` for the lifetime of
// the block.
class Recording {
 public:
  explicit Recording(Order& order);
  ~Recording();
  Recording(const Recording&) = delete;
  Recording& operator=(const Recording&) = delete;

 private:
  Order* previous_;
  std::vector<int> previous_runs_;
};

// Returns the lines `c6502cpp pass-order` prints: `<driver> <pass>` for
// each pass of each driver, in the order of `Order`, as an `-O` compile
// of a small built-in program runs them. Each driver runs all of its
// passes on any program with a function, so that one program reaches
// them all.
std::string listing();

}  // namespace c6502::pass_order
