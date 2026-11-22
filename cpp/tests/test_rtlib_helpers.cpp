// Tests of the runtime helpers' table (src/rtlib/helpers.h), which
// tools/rtlib_table.py copies from rtlib/helpers.py.
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "doctest.h"
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

}  // namespace
