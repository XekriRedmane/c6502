// Tests of the parts of abi_selection.h that the recorded calls of
// tests/test_abi_stage_calls.cpp and the corpus don't reach: the slot
// symbols of `zeropage(addr)` parameters, whose every call in the corpus
// and the unit tests gave an empty map.
#include <optional>
#include <string>
#include <vector>

#include "c99_ast.h"
#include "doctest.h"
#include "passes/abi_selection.h"

namespace c99 = c6502::c99_ast;
using c6502::asdl::Int;
using c6502::passes::Abi;
using c6502::passes::SoftStackLayout;
using c6502::passes::ZpLayout;

TEST_CASE("pinned_slot_symbols maps each pinned byte's slot to its address") {
  Abi abi;
  abi.insert_or_assign("soft", SoftStackLayout{});
  abi.insert_or_assign("f", ZpLayout{.slot_symbols = {"__zpabi_f__a", "__zpabi_f__b_0",
                                                      "__zpabi_f__b_1"},
                                     .pinned = {std::nullopt, Int(0x40), Int(0x41)}});
  abi.insert_or_assign("g", ZpLayout{.slot_symbols = {"__zpabi_g__x"}});
  const auto got = c6502::passes::pinned_slot_symbols(abi);
  REQUIRE(got.size() == 2);
  CHECK(got.at(std::string("__zpabi_f__b_0")) == Int(0x40));
  CHECK(got.at(std::string("__zpabi_f__b_1")) == Int(0x41));
  CHECK(got.begin()->first == "__zpabi_f__b_0");
}

TEST_CASE("pinned_param_addrs gives each byte of a zeropage(addr) parameter") {
  c99::Type_function_decl fn{.name = "f", .params = {"@1.a", "@2.b", "@3.c"}};
  fn.param_registers = {"", "@128", "auto"};
  const auto got = c6502::passes::pinned_param_addrs(fn, {Int(1), Int(2), Int(1)});
  const std::vector<std::optional<Int>> expected = {std::nullopt, Int(128), Int(129),
                                                    std::nullopt};
  CHECK(got == expected);
  fn.param_registers = {"auto"};
  CHECK(c6502::passes::pinned_param_addrs(fn, {Int(1), Int(2), Int(1)}).empty());
}
