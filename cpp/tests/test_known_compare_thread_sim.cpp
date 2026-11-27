// Each test is the shape the short-circuit fold leaves of
// `if (x == 20 || x == 10) r = x == 20 ? 1 : 2;`, run on 600 values.
#include <functional>
#include <string>
#include <vector>

#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

std::string program(const std::string& test, const std::string& kind = "uint8_t") {
  return "#include <stdint.h>\n"
         "volatile " + kind + " sink;\n"
         "static uint8_t f(" + kind + " t) {\n"
         "    uint8_t r = 0;\n"
         "    " + test + "\n"
         "    return r;\n"
         "}\n"
         "int main(void) {\n"
         "    uint16_t n = 0;\n"
         "    for (uint16_t v = 0; v < 600; v++)\n"
         "        n = (uint16_t)(n * 3u + f((" + kind + ")v));\n"
         "    return n;\n"
         "}\n";
}

// What `main` returns when `f` returns `model(t)`.
std::int64_t expected(const std::function<int(int)>& model, const std::string& kind) {
  std::int64_t n = 0;
  const int bits = kind == "uint8_t" ? 8 : 16;
  for (int v = 0; v < 600; ++v) {
    const int t = v & ((1 << bits) - 1);
    n = (n * 3 + model(t)) & 0xFFFF;
  }
  return n;
}

sim::Simulation build(const std::string& src, bool optimize) {
  SimOptions options;
  options.optimize = optimize;
  options.inline_calls = false;
  return build_sim(src, options);
}

}  // namespace

TEST_CASE("known_compare_thread sim: each shape computes its values") {
  struct Shape {
    const char* name;
    const char* body;
    std::function<int(int)> model;
  };
  const std::vector<Shape> shapes = {
      {"or_then_test", R"c(if (t == 20 || t == 10) { sink = t; r = t == 20 ? 0x37 : 0x38; })c",
       [](int t) { return t == 20 || t == 10 ? (t == 20 ? 0x37 : 0x38) : 0; }},
      {"or_then_other_test", "if (t == 20 || t == 10) { sink = t; r = t == 10 ? 1 : 2; }",
       [](int t) { return t == 20 || t == 10 ? (t == 10 ? 1 : 2) : 0; }},
      {"three_way_or", R"c(if (t == 1 || t == 2 || t == 3) r = t == 2 ? 5 : t == 3 ? 6 : 7;)c",
       [](int t) { return t == 1 ? 7 : t == 2 ? 5 : t == 3 ? 6 : 0; }},
      {"and_not", "if (t != 4 && t != 9) r = 1; else r = t == 4 ? 2 : 3;",
       [](int t) { return t != 4 && t != 9 ? 1 : (t == 4 ? 2 : 3); }},
  };
  std::string all;
  for (const Shape& shape : shapes) {
    for (const std::string kind : {"uint8_t", "uint16_t"}) {
      const std::string src = program(shape.body, kind);
      all += src;
      for (const bool optimize : {false, true}) {
        INFO("shape=" << shape.name << " kind=" << kind << " optimize=" << optimize);
        CHECK(build(src, optimize).run().return_int() == expected(shape.model, kind));
      }
    }
  }
  // The count and CRC-32 of the programs that the Python test's generator
  // wrote, computed on 27 Nov 2026.
  CHECK(shapes.size() * 2 == 8);
  CHECK(sim::cpu_run::crc32(all) == 3690397941u);
}

TEST_CASE("known_compare_thread sim: a value changed between the tests") {
  // `sink` is 0 when f reads it here, so t keeps its value, but the store
  // and the read put the test after a write of t.
  std::string src = program(R"c(if (t == 20 || t == 10) { t = (uint8_t)(t + sink); r = t == 20 ? 1 : 2; })c");
  const std::string from = "volatile uint8_t sink;";
  src.replace(src.find(from), from.size(), "uint8_t sink;");
  // The CRC-32 of the Python test's program, computed on 27 Nov 2026.
  CHECK(sim::cpu_run::crc32(src) == 1173364302u);
  const std::int64_t want =
      expected([](int t) { return t == 20 || t == 10 ? (t == 20 ? 1 : 2) : 0; }, "uint8_t");
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build(src, optimize).run().return_int() == want);
  }
}

}  // namespace c6502::suite
