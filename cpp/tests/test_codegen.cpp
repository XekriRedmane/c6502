// Tests of the helpers of passes/codegen.h.
#include "doctest.h"
#include "passes/codegen.h"

using c6502::passes::merged_symbol_addrs;
using c6502::passes::OrderedSymbolAddrs;

TEST_CASE("merged_symbol_addrs keeps the first list's order and adds the new names") {
  const OrderedSymbolAddrs first = {{"b", 1}, {"a", 2}};
  const OrderedSymbolAddrs second = {{"c", 3}, {"d", 4}};
  CHECK(merged_symbol_addrs(first, second) ==
        OrderedSymbolAddrs{{"b", 1}, {"a", 2}, {"c", 3}, {"d", 4}});
}

TEST_CASE("merged_symbol_addrs takes a shared name's address from the second list") {
  const OrderedSymbolAddrs first = {{"SSP", 0}, {"x", 0x80}, {"FP", 2}};
  const OrderedSymbolAddrs second = {{"y", 0x81}, {"x", 0x90}};
  CHECK(merged_symbol_addrs(first, second) ==
        OrderedSymbolAddrs{{"SSP", 0}, {"x", 0x90}, {"FP", 2}, {"y", 0x81}});
}

TEST_CASE("merged_symbol_addrs with an empty list") {
  const OrderedSymbolAddrs some = {{"a", 1}};
  CHECK(merged_symbol_addrs({}, some) == some);
  CHECK(merged_symbol_addrs(some, {}) == some);
  CHECK(merged_symbol_addrs({}, {}).empty());
}
