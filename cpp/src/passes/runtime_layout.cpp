// On an Apple II, the Monitor keeps its text window in $20-$23 and its
// cursor in $24-$25, on top of the classic layout's HARGS and DPTR. A
// program declares the bytes that other software owns with
// `[[c6502::address(...)]]` or `zeropage(addr)`, and `choose_layout`
// moves the runtime's block out of their way.
#include "passes/runtime_layout.h"

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

#include "errors.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

using asdl::Int;

std::string ranges(const std::vector<Int>& addrs) {
  std::string out;
  std::size_t i = 0;
  while (i < addrs.size()) {
    std::size_t j = i;
    while (j + 1 < addrs.size() && addrs[j + 1] == addrs[j] + 1) {
      ++j;
    }
    if (!out.empty()) {
      out += ", ";
    }
    out += "$" + pyrepr::hex(addrs[i], 2);
    if (i != j) {
      out += "-$" + pyrepr::hex(addrs[j], 2);
    }
    i = j + 1;
  }
  return out;
}

}  // namespace

RuntimeLayout choose_layout(const std::set<Int>& reserved, const HargsNeed& hargs_need) {
  const RuntimeLayout classic;
  const std::set<Int> classic_bytes = classic.bytes();
  if (std::none_of(classic_bytes.begin(), classic_bytes.end(),
                   [&](Int b) { return reserved.contains(b); })) {
    return classic;
  }
  const Int size = std::max<Int>(hargs_need(), 1);
  std::set<Int> used = reserved;

  const auto take = [&](Int n, std::string_view what) -> Int {
    for (Int addr = 0; addr < 0x80 - n + 1; ++addr) {
      bool free = true;
      for (Int b = addr; b < addr + n; ++b) {
        if (used.contains(b)) {
          free = false;
          break;
        }
      }
      if (free) {
        for (Int b = addr; b < addr + n; ++b) {
          used.insert(b);
        }
        return addr;
      }
    }
    throw CompileError("RuntimeLayoutError",
                       "no room below $80 for the runtime's " + std::string(what) + " (" +
                           pyrepr::integer(n) +
                           " bytes) around the reserved zero-page bytes " +
                           ranges(std::vector<Int>(reserved.begin(), reserved.end())));
  };

  RuntimeLayout out;
  out.hargs = take(size, "HARGS block");
  out.ssp = take(2, "soft stack pointer SSP");
  out.fp = take(2, "frame pointer FP");
  out.dptr = take(2, "pointer scratch DPTR");
  out.hargs_size = size;
  return out;
}

}  // namespace c6502::passes
