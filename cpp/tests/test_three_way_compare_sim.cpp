// The simulator tests of tests/test_three_way_compare.py: each shape of a
// 2-byte unsigned ordering compare and an equality compare of the same
// values, which src/passes/optimization/three_way_compare.h splits into
// a compare of the high bytes and then the low ones, classifies every
// pair of the values here in both pipelines.
#include <cstdint>
#include <cstdio>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"
#include "sim_helpers.h"

namespace c6502::suite {
namespace {

const std::vector<int> kValues = {0x0000, 0x0001, 0x00FF, 0x0100, 0x0101, 0x01FF, 0x1233,
                                  0x1234, 0x1235, 0x1134, 0x1334, 0x8000, 0xFFFE, 0xFFFF};

int classify(int a, int b) { return a < b ? 1 : a == b ? 2 : 3; }

// Each body returns 1 for a < b, 2 for a == b, and 3 for a > b, given
// `uint16_t a, b`.
const std::vector<std::pair<std::string, std::string>> kShapes = {
    // The ordering compare's target starts with the equality compare,
    // which jumps when they differ.
    {"less_then_equal", R"c(
        if (a < b) return 1;
        if (a == b) return 2;
        return 3;)c"},
    // The same, with the equality compare jumping when they're equal.
    {"less_then_not_equal", R"c(
        if (a < b) return 1;
        if (a != b) return 3;
        return 2;)c"},
    // `<=` at the first compare, so the operands swap.
    {"greater_then_equal", R"c(
        if (a > b) return 3;
        if (a == b) return 2;
        return 1;)c"},
    // The two compares in a row.
    {"adjacent", R"c(
        if (a < b) goto less;
        if (a != b) goto greater;
        return 2;
    less:
        return 1;
    greater:
        return 3;)c"},
    // A constant operand.
    {"constant", R"c(
        (void)b;
        if (a < 0x1234u) return 1;
        if (a == 0x1234u) return 2;
        return 3;)c"},
    // Operands built from bytes, as in Zork's match_dictionary_word.
    {"built_from_bytes", R"c(
        uint8_t ka[2], kb[2];
        ka[0] = (uint8_t)a; ka[1] = (uint8_t)(a >> 8);
        kb[0] = (uint8_t)b; kb[1] = (uint8_t)(b >> 8);
        uint16_t x = ((uint16_t)ka[1] << 8) | ka[0];
        uint16_t y = ((uint16_t)kb[1] << 8) | kb[0];
        if (x < y) return 1;
        if (x == y) return 2;
        return 3;)c"},
    // A byte widened to 2 bytes, whose high byte is 0.
    {"widened_byte", R"c(
        uint8_t c = (uint8_t)a;
        if (c < b) return 1;
        if (c == b) return 2;
        return 3;)c"},
};

int predicted(const std::string& name, int a, int b) {
  if (name == "constant") return classify(a, 0x1234);
  if (name == "widened_byte") return classify(a & 0xFF, b);
  return classify(a, b);
}

std::string program(const std::string& body) {
  std::string table;
  for (int v : kValues) {
    char hex[16];
    std::snprintf(hex, sizeof hex, "0x%04Xu", v);
    table += (table.empty() ? "" : ", ") + std::string(hex);
  }
  const std::size_t n = kValues.size();
  return cat("#include <stdint.h>\n", "uint16_t t[", n, "] = {", table, "};\n",
             "static uint8_t f(uint16_t a, uint16_t b) {", body, "\n}\n", "int main(void) {\n", "    uint16_t n = 0;\n",
             "    for (uint8_t i = 0; i < ", n, "; i++)\n", "        for (uint8_t j = 0; j < ", n, "; j++)\n",
             "            n = (uint16_t)(n * 5u + f(t[i], t[j]));\n", "    return n;\n", "}\n");
}

std::int64_t expected(const std::string& name) {
  std::int64_t n = 0;
  for (int a : kValues) {
    for (int b : kValues) n = (n * 5 + predicted(name, a, b)) & 0xFFFF;
  }
  return n;
}

TEST_CASE("three_way_compare: each shape runs") {
  std::string all;
  for (const auto& [name, body] : kShapes) all += program(body);
  // zlib's CRC-32 of the 7 programs tests/test_three_way_compare.py ran,
  // on 27 Nov 2026.
  CHECK(sim::cpu_run::crc32(all) == 0x3343559Cu);
  for (const auto& [name, body] : kShapes) {
    for (bool optimize : {false, true}) {
      INFO("shape=" << name << " optimize=" << optimize);
      SimOptions options;
      options.optimize = optimize;
      options.inline_calls = false;
      const sim::SimResult r = build_sim(program(body), options).run();
      CHECK(r.return_int() == expected(name));
    }
  }
}

}  // namespace
}  // namespace c6502::suite
