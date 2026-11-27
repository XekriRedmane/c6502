// The branch on a signed `x > 0` or `x <= 0` that tests the sign bit
// and ORs the bytes, run in both pipelines on values of three types,
// from tests/test_signed_positive_branch.py.
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

const std::vector<std::pair<std::string, std::vector<std::int64_t>>> kTypes = {
    {"signed char", {-128, -1, 0, 1, 127}},
    {"int", {-32768, -256, -1, 0, 1, 255, 256, 32767}},
    {"long", {-2147483647 - 1, -65536, -1, 0, 1, 256, 65536, 2147483647}},
};

// Each value is read from a volatile array, so no comparison folds. Bit
// k of a test's result says whether the condition held for the kth
// value. The four tests are the two operators in each operand order.
std::string program(const std::string& type, const std::vector<std::int64_t>& values) {
  const std::string n = std::to_string(values.size());
  std::string init;
  for (std::size_t k = 0; k < values.size(); ++k) {
    if (k > 0) init += ", ";
    init += "(" + type + ")(" + std::to_string(values[k]) + ")";
  }
  std::string out = "volatile " + type + " v[" + n + "] = {" + init + "};\n";
  const auto test = [&](const char* name, const char* cond) {
    out += std::string("unsigned int ") + name + "(void) { unsigned int r = 0;\n";
    out += "  for (unsigned char k = 0; k < " + n + "; k++) {\n";
    out += "    " + type + " x = v[k];\n";
    out += std::string("    if (") + cond + ") r |= 1u << k; }\n";
    out += "  return r; }\n";
  };
  test("pos", "x > 0");
  test("nonpos", "x <= 0");
  test("pos_rev", "0 < x");
  test("nonpos_rev", "0 >= x");
  out +=
      "unsigned int which;\n"
      "int main(void) {\n"
      "  if (which == 0) return pos();\n"
      "  if (which == 1) return nonpos();\n"
      "  if (which == 2) return pos_rev();\n"
      "  return nonpos_rev(); }\n";
  return out;
}

}  // namespace

TEST_CASE("signed_positive_branch sim: each comparison with 0 gives the sign test's result") {
  // The CRC-32 of tests/test_signed_positive_branch.py's `_program` for
  // the three types, concatenated, computed on 27 Nov 2026, shows that
  // the generator writes the programs the Python module wrote.
  std::string all;
  for (const auto& [type, values] : kTypes) all += program(type, values);
  CHECK(sim::cpu_run::crc32(all) == 2380030858);
  for (const auto& [type, values] : kTypes) {
    std::uint64_t positive = 0;
    for (std::size_t k = 0; k < values.size(); ++k) {
      if (values[k] > 0) positive |= std::uint64_t{1} << k;
    }
    const std::uint64_t full = (std::uint64_t{1} << values.size()) - 1;
    const std::uint64_t expected[] = {positive, full ^ positive, positive, full ^ positive};
    for (int which = 0; which < 4; ++which) {
      std::string src = program(type, values);
      src.replace(src.find("unsigned int which;"), 19, "unsigned int which = " + std::to_string(which) + ";");
      for (const bool optimize : {false, true}) {
        INFO("type=" << type << " which=" << which << " optimize=" << optimize);
        CHECK(static_cast<std::uint64_t>(build_sim(src, optimize).run().return_int() & 0xFFFF) ==
              expected[which]);
      }
    }
  }
}

}  // namespace c6502::suite
