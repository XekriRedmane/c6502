// Tests of the runtime helpers' table (src/rtlib/helpers.h), which
// tools/rtlib_table.py copies from rtlib/helpers.py, and of the HARGS
// footprints measured on it. tests/test_rtlib_table.py compares every
// helper's footprint with Python's through `c6502cpp helper-footprints`.
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "doctest.h"
#include "passes/zp_globals.h"
#include "rtlib/helpers.h"

namespace {

using namespace c6502;

TEST_CASE("the table holds the helpers in all_helper_functions' order") {
  std::vector<std::string> names;
  for (const auto& fn : rtlib::all_helper_functions()) names.push_back(fn.name);
  const std::vector<std::string> expected{
      "mul8",      "mul16",     "mul32",  "udivmod8", "sdivmod8", "udivmod16", "sdivmod16",
      "udivmod32", "sdivmod32", "asl8",   "asr8",     "lsr8",     "asl16",     "asr16",
      "lsr16",     "asl32",     "asr32",  "lsr32",    "bin16_to_bcd"};
  CHECK(names == expected);
}

TEST_CASE("the functions dump as the table's text") {
  asm_ast::Program prog;
  for (const auto& fn : rtlib::all_helper_functions()) prog.top_level.emplace_back(fn);
  CHECK(asdl::dump(prog) == rtlib::helper_dump());
}

TEST_CASE("bin16_to_bcd is the one decimal-mode helper") {
  CHECK(rtlib::decimal_mode_helpers() == std::vector<std::string>{"bin16_to_bcd"});
}

TEST_CASE("a footprint counts the helpers a helper calls") {
  const passes::HelperFootprints& fp = passes::helper_footprints();
  CHECK(fp.size() == rtlib::all_helper_functions().size());
  // `mul32` works in HARGS+12..19.
  CHECK(fp.at("mul32") == 20);
  // `udivmod16` touches 9 bytes, and `sdivmod16`, which calls it, 11.
  CHECK(fp.at("udivmod16") == 9);
  CHECK(fp.at("sdivmod16") == 11);
  CHECK(fp.at("sdivmod32") == 22);
  CHECK(fp.at("bin16_to_bcd") == 7);
}

}  // namespace
