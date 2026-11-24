// The Python unit tests of the runtime's layout and of the runtime
// library's helpers that called the Python functions directly, ported
// by hand, one case per test with each of its assertions:
// tests/test_runtime_layout.py's `choose_layout` and `helper_footprints`,
// test_runtime_library.py's `closure` and `helper_sizes`, and
// test_bin_to_bcd.py's size and footprint of `bin16_to_bcd`. Python's
// `helper_sizes` is the size of each helper's section in the library's
// object for that helper alone, which `helper_size` assembles here.
#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "binutils/assembler.h"
#include "binutils/elf.h"
#include "doctest.h"
#include "errors.h"
#include "passes/asm_context.h"
#include "passes/runtime_layout.h"
#include "passes/runtime_library.h"
#include "passes/zp_globals.h"

namespace {

using c6502::asdl::Int;
using c6502::passes::choose_layout;
using c6502::passes::RuntimeLayout;
using Bytes = std::set<Int>;

Bytes range(Int lo, Int hi) {
  Bytes out;
  for (Int b = lo; b < hi; ++b) out.insert(b);
  return out;
}

// Returns the bytes `name` takes in a program, alone, without the
// helpers it calls (Python's `runtime_library.helper_sizes()[name]`).
std::int64_t helper_size(const std::string& name) {
  const auto obj = c6502::binutils::assembler::assemble(
      c6502::passes::library_text({name}), c6502::passes::kLibraryName);
  const auto sym = obj.symbol(name);
  REQUIRE(sym.get() != nullptr);
  REQUIRE(sym->section.get() != nullptr);
  return sym->section->size();
}

}  // namespace

TEST_CASE("runtime_layout: TestChooseLayout::test_classic_when_nothing_collides") {
  int calls = 0;
  const RuntimeLayout layout = choose_layout({0x26, 0x40, 0x90}, [&] {
    ++calls;
    return Int{5};
  });
  CHECK(layout == RuntimeLayout{});
  // HARGS isn't measured when the block stays put.
  CHECK(calls == 0);
}

TEST_CASE("runtime_layout: TestChooseLayout::test_moves_around_reserved_bytes") {
  const Bytes reserved = range(0x20, 0x26);
  const RuntimeLayout layout = choose_layout(reserved, [] { return Int{6}; });
  CHECK(layout.hargs_size == 6);
  const Bytes used = layout.bytes();
  CHECK(std::none_of(used.begin(), used.end(), [&](Int b) { return reserved.contains(b); }));
  // HARGS first, then SSP, FP, DPTR, each in the lowest free run.
  CHECK(layout.hargs == 0x00);
  CHECK(layout.ssp == 0x06);
  CHECK(layout.fp == 0x08);
  CHECK(layout.dptr == 0x0A);
}

TEST_CASE("runtime_layout: TestChooseLayout::test_unused_hargs_still_gets_a_byte") {
  const RuntimeLayout layout = choose_layout({0x24}, [] { return Int{0}; });
  CHECK(layout.hargs_size == 1);
  CHECK(layout.bytes().size() == 7);
}

TEST_CASE("runtime_layout: TestChooseLayout::test_no_room") {
  std::string message;
  try {
    choose_layout(range(0x02, 0x80), [] { return Int{4}; });
  } catch (const c6502::CompileError& e) {
    message = e.python_type() + ": " + e.what();
  }
  CHECK(message ==
        "RuntimeLayoutError: no room below $80 for the runtime's HARGS block (4 bytes) around "
        "the reserved zero-page bytes $02-$7F");
}

TEST_CASE("runtime_layout: TestHargsSize::test_helper_footprints") {
  const auto& fp = c6502::passes::helper_footprints();
  // mul32 works in HARGS+12..19, sdivmod32 stashes signs at +20 / +21,
  // and sdivmod8 calls udivmod8.
  CHECK(fp.at("mul32") == 20);
  CHECK(fp.at("sdivmod32") == 22);
  CHECK(fp.at("sdivmod8") == 6);
  CHECK(fp.at("sdivmod8") >= fp.at("udivmod8"));
}

TEST_CASE("runtime_library: test_runtime_library.py::TestSelection::test_closure") {
  using Names = std::vector<std::string>;
  CHECK(c6502::passes::closure({"sdivmod32"}) == Names{"udivmod32", "sdivmod32"});
  CHECK(c6502::passes::closure({"mul8", "fadd"}) == Names{"mul8"});
}

TEST_CASE("runtime_library: test_runtime_library.py::TestSelection::test_sizes") {
  // Every helper of the library has a size, and each takes some bytes.
  for (const std::string& name : c6502::passes::helper_names()) {
    CAPTURE(name);
    CHECK(helper_size(name) > 0);
  }
}

TEST_CASE("runtime_library: test_bin_to_bcd.py::TestHelper::test_size_and_cycles") {
  // docs/builtins.md gives the size. The cycles are measured on the
  // simulator by the Python test, which stays.
  CHECK(helper_size("bin16_to_bcd") == 46);
}

TEST_CASE("runtime_library: test_bin_to_bcd.py::TestHelper::test_footprint") {
  CHECK(c6502::passes::helper_footprints().at("bin16_to_bcd") == 7);
}
