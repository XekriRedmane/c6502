// tests/test_address_attr.py's TestPlacement, ported by hand: the
// placement that `allocate_zp_globals` gives the names declared with
// `address(addr)`, and its errors, with Python's messages. The sources
// go through the C++ front end under -O, as the Python tests' went
// through `compile._resolved(src, optimize=True)` and the type checker.
#include <map>
#include <set>
#include <string>

#include "doctest.h"
#include "errors.h"
#include "passes/front_end.h"
#include "passes/zp_globals.h"

namespace {

c6502::passes::ZpGlobals placement(const std::string& src) {
  c6502::passes::FrontEndOptions options;
  options.preprocess = false;
  options.optimize = true;
  std::string err;
  const auto r = c6502::passes::front_end(src, options, err);
  return c6502::passes::allocate_zp_globals(r.symbols, &r.types, {}, nullptr);
}

// The entries of `m`, compared without their order, as Python's dicts
// are.
std::map<std::string, c6502::asdl::Int> entries(
    const c6502::OrderedMap<std::string, c6502::asdl::Int>& m) {
  return {m.begin(), m.end()};
}

}  // namespace

TEST_CASE("address_attr: test_address_attr.py::TestPlacement::test_fixed_names") {
  const auto zp = placement(
      "[[c6502::address(0xFDED)]] void COUT(unsigned char c);\n"
      "extern unsigned int w [[c6502::address(0xC010)]];\n"
      "extern unsigned char m [[c6502::address(0x90)]];\n");
  CHECK(entries(zp.fixed) == std::map<std::string, c6502::asdl::Int>{
                                 {"COUT", 0xFDED}, {"w", 0xC010}});
  // Below $100 the object is a zero-page object: addressed, reserved
  // from the compiler's pool, and not initialized.
  CHECK(entries(zp.addrs) == std::map<std::string, c6502::asdl::Int>{{"m", 0x90}});
  CHECK(zp.reserved.count(0x90) == 1);
  CHECK(zp.owned.empty());
}

TEST_CASE("address_attr: test_address_attr.py::TestPlacement::test_placement_errors") {
  const struct {
    const char* label;
    const char* src;
    const char* message;
  } bad[] = {
      {"overlap",
       "extern unsigned int a [[c6502::address(0xC000)]];\n"
       "extern unsigned char b [[c6502::address(0xC001)]];\n",
       "object 'b' at $C001 overlaps 'a' at byte $C001"},
      {"past $FFFF", "extern unsigned int a [[c6502::address(0xFFFF)]];\n",
       "object 'a' (2 bytes at $FFFF) runs past $FFFF"},
  };
  for (const auto& b : bad) {
    CAPTURE(b.label);
    try {
      placement(b.src);
      FAIL("no error");
    } catch (const c6502::CompileError& e) {
      CHECK(e.python_type() == "ZpGlobalError");
      CHECK(std::string(e.what()) == b.message);
    }
  }
}
