#include "pass_order.h"

#include <algorithm>
#include <utility>

#include "passes/codegen_stage.h"
#include "passes/front_end.h"
#include "passes/zp_globals.h"

namespace c6502::pass_order {

namespace {

// The recording open on this thread, if any, and its open runs, each the
// index of its driver in `order->drivers`, or -1 for a run after a
// driver's first, whose reports aren't kept. A compile on another
// thread, as check-golden runs them, reports to its own.
struct State {
  Order* order = nullptr;
  std::vector<int> runs;
};

thread_local State state;

// The program `listing` compiles: a function with a loop, called by
// `main`, so that each driver has a function to work on.
constexpr const char* kProgram =
    "static int total;\n"
    "int sum(int n) {\n"
    "  int s = 0;\n"
    "  for (int i = 0; i < n; i++) s += i;\n"
    "  return s;\n"
    "}\n"
    "int main(void) {\n"
    "  total = sum(4);\n"
    "  return total;\n"
    "}\n";

}  // namespace

void ran(std::string_view pass) {
  if (state.order == nullptr || state.runs.empty() || state.runs.back() < 0) {
    return;
  }
  state.order->drivers[static_cast<std::size_t>(state.runs.back())].second.emplace_back(pass);
}

Run::Run(std::string_view driver) {
  if (state.order == nullptr) {
    return;
  }
  open_ = true;
  auto& drivers = state.order->drivers;
  const bool seen = std::any_of(drivers.begin(), drivers.end(),
                                [&](const auto& d) { return d.first == driver; });
  if (seen) {
    state.runs.push_back(-1);
    return;
  }
  drivers.emplace_back(std::string(driver), std::vector<std::string>{});
  state.runs.push_back(static_cast<int>(drivers.size()) - 1);
}

Run::~Run() {
  if (open_) {
    state.runs.pop_back();
  }
}

Recording::Recording(Order& order)
    : previous_(state.order), previous_runs_(std::move(state.runs)) {
  state.order = &order;
  state.runs.clear();
}

Recording::~Recording() {
  state.order = previous_;
  state.runs = std::move(previous_runs_);
}

std::string listing() {
  Order order;
  {
    Recording recording(order);
    passes::FrontEndOptions front;
    front.preprocess = false;
    front.optimize = true;
    std::string err;
    passes::FrontEndResult checked = passes::front_end(kProgram, front, err);
    passes::StageTables tables;
    tables.c99 = std::move(checked.prog);
    tables.symbols = std::move(checked.symbols);
    tables.types = std::move(checked.types);
    tables.footprints = passes::helper_footprints();
    passes::CodegenOptions options;
    options.optimize = true;
    passes::codegen(tables, options);
  }
  std::string out;
  for (const auto& [driver, passes] : order.drivers) {
    for (const auto& pass : passes) {
      out += driver + " " + pass + "\n";
    }
  }
  return out;
}

}  // namespace c6502::pass_order
