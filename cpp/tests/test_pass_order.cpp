// Tests of src/pass_order.h, the order of the optimizer's passes that
// `c6502cpp pass-order` lists for tests/test_optimization_docs.py.
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "doctest.h"
#include "pass_order.h"
#include "passes/optimization/optimizer.h"
#include "passes/peephole_fixedpoint.h"

namespace {

namespace passes = c6502::passes;
namespace pass_order = c6502::pass_order;

// The passes `listing` gives each driver, in order, and the drivers in
// order.
std::vector<std::pair<std::string, std::vector<std::string>>> listed() {
  std::vector<std::pair<std::string, std::vector<std::string>>> out;
  std::istringstream lines(pass_order::listing());
  std::string driver;
  std::string pass;
  while (lines >> driver >> pass) {
    if (out.empty() || out.back().first != driver) {
      out.emplace_back(driver, std::vector<std::string>{});
    }
    out.back().second.push_back(pass);
  }
  return out;
}

const std::vector<std::string>* passes_of(
    const std::vector<std::pair<std::string, std::vector<std::string>>>& drivers,
    const std::string& name) {
  for (const auto& [driver, list] : drivers) {
    if (driver == name) return &list;
  }
  return nullptr;
}

}  // namespace

TEST_CASE("pass_order: the listing holds each driver's passes in its order") {
  const auto drivers = listed();
  std::vector<std::string> names;
  for (const auto& [driver, list] : drivers) {
    names.push_back(driver);
  }
  // Each driver's lines are together, and the drivers come in the order
  // their first runs began.
  CHECK(names == std::vector<std::string>{"optimized_tac", "tac", "optimized_front",
                                          "asm_optimizer", "asm_optimizer.function",
                                          "optimized_asm", "peephole"});

  // The peephole fixed point reports its sweep's table.
  std::vector<std::string> sweep;
  for (const auto& p : passes::peephole_sweep(passes::AsmContext{}, nullptr, nullptr, {})) {
    sweep.emplace_back(p.name);
  }
  REQUIRE(passes_of(drivers, "peephole") != nullptr);
  CHECK(*passes_of(drivers, "peephole") == sweep);

  // The TAC optimizer's driver reports its six lists in order.
  const auto& d = passes::tac_driver();
  std::vector<std::string> tac;
  for (const auto* list : {&d.pre_ssa, &d.pre_fixedpoint, &d.fixedpoint, &d.post_fixedpoint,
                           &d.post_destruction, &d.post_destruction_fixedpoint}) {
    for (const auto& p : *list) {
      tac.emplace_back(p.name);
    }
  }
  REQUIRE(passes_of(drivers, "tac") != nullptr);
  CHECK(*passes_of(drivers, "tac") == tac);

  // `optimize_program` around the round trip of each function, and the
  // region's second runs of two passes.
  CHECK(*passes_of(drivers, "asm_optimizer") ==
        std::vector<std::string>{"fold_const_statics", "dead_static_elimination"});
  const auto& region = *passes_of(drivers, "optimized_asm");
  CHECK(std::count(region.begin(), region.end(), "counter_to_index_reg") == 2);
  CHECK(std::count(region.begin(), region.end(), "carry_known") == 2);
  CHECK(region.front() == "licm");
  CHECK(region.back() == "outline_runs");
}

TEST_CASE("pass_order: reports outside a recording are dropped") {
  pass_order::Order order;
  {
    const pass_order::Run run("outside");
    pass_order::ran("dropped");
  }
  {
    pass_order::Recording recording(order);
    pass_order::ran("no run is open");
    {
      const pass_order::Run run("first");
      pass_order::ran("a");
      {
        const pass_order::Run inner("inner");
        pass_order::ran("b");
      }
      pass_order::ran("c");
    }
    {
      // A second run of a driver isn't kept.
      const pass_order::Run run("first");
      pass_order::ran("d");
    }
  }
  pass_order::ran("after");
  REQUIRE(order.drivers.size() == 2);
  CHECK(order.drivers[0].first == "first");
  CHECK(order.drivers[0].second == std::vector<std::string>{"a", "c"});
  CHECK(order.drivers[1].first == "inner");
  CHECK(order.drivers[1].second == std::vector<std::string>{"b"});
}
