// The simulator tests of tests/test_outline_runs.py: a program whose
// repeated run of instructions src/passes/outline_runs.h moves into a
// function of its own runs in both pipelines, compiled on its own and
// linked.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

const std::string kSrc = R"c(
#include <stdint.h>
uint16_t ops[2] = { 1234, 7 };
uint16_t acc;
static uint16_t take(uint16_t a, uint16_t b) { return a * 3 + b; }
void f1(void) { acc += take(ops[0], ops[1]); acc ^= 0x55; }
void f2(void) { acc -= take(ops[0], ops[1]); acc += 9; }
void f3(void) { acc *= take(ops[0], ops[1]); }
int main(void) {
    f1(); f2(); f3(); f1();
    return (int)(acc & 0x7FFF);
}
)c";
auto expected() {
  auto acc = 0;
  auto t = (1234 * 3 + 7) & 0xFFFF;
  for (const std::string f : {"f1", "f2", "f3", "f1"}) {
    if (f == "f1") {
      acc = ((acc + t) & 0xFFFF) ^ 0x55;
    } else if (f == "f2") {
      acc = (acc - t + 9) & 0xFFFF;
    } else {
      acc = (acc * t) & 0xFFFF;
    }
  }
  return acc & 0x7FFF;
}

TEST_CASE("outline_runs: outlined and runs") {
  Options text_options;
  text_options.optimize = true;
  text_options.inline_calls = false;
  CHECK(contains(codegen(kSrc, text_options), "@run0:"));
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    SimOptions options;
    options.optimize = optimize;
    options.inline_calls = false;
    auto out = build_sim(kSrc, options).run();
    CHECK_FALSE(out.timed_out);
    CHECK(out.return_int() == expected());
  }
}

TEST_CASE("outline_runs: linked program runs") {
  TempDir tmp;
  const std::string obj = tmp.file("a.o");
  const std::string exe = tmp.file("a.out");
  MainOptions no_inlining;
  no_inlining.inline_calls = false;
  CHECK(compile_main({"compile.py", "-", "-c", "-O", "-o", obj}, kSrc, no_inlining).status == 0);
  std::vector<std::string> argv = {"compile.py", obj, "-o", exe};
  for (const std::string& d : sim_defsym_args()) argv.push_back(d);
  CHECK(compile_main(argv, "", no_inlining).status == 0);
  CHECK(build_linked_sim(exe).run().return_int() == expected());
}

}  // namespace
}  // namespace c6502::suite
