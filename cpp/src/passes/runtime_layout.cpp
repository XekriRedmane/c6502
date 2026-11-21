// Where the runtime's zero-page block goes: SSP, FP, HARGS, and DPTR.
//
// The runtime keeps four things in zero page.
//
//     SSP    2 bytes   soft stack pointer, an `(SSP),Y` base
//     FP     2 bytes   frame pointer, an `(FP),Y` base
//     HARGS  N bytes   the runtime-helper exchange block
//     DPTR   2 bytes   the pointer scratch, an `(DPTR),Y` base
//
// The compiled code names them only symbolically, and the output binds
// each one with an `EQU` line. Their classic layout is SSP at $00, FP at
// $02, a 32-byte HARGS at $04, and DPTR at $24, which ends at $25.
//
// Code that runs alongside system software can't always use those
// bytes. On an Apple II, the Monitor keeps its text window in $20-$23
// and its cursor in $24-$25, right on top of HARGS and DPTR. A program
// says which bytes something else owns by declaring them, with
// `[[c6502::address(...)]]` or `zeropage(addr)`, and `choose_layout`
// moves the block out of their way.
//
//   * If no reserved byte is in the classic layout, the classic layout
//     is used, so a program that doesn't reserve those bytes compiles
//     exactly as before.
//   * Otherwise HARGS is sized to what the program needs (`hargs_need`),
//     and HARGS, SSP, FP, and DPTR take the lowest free runs of bytes
//     below $80, in that order. The compiler's own pools are $80-$FF,
//     so the block stays below them. When it doesn't fit, that's a
//     compile error.
//
// The layout has to be fixed before the asm passes run, since several
// of them resolve runtime names to addresses to decide aliasing.
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

// Returns `addrs`, which is sorted, as runs such as "$20-$22, $30".
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
  // HARGS gets at least one byte, even in a program that never touches
  // it, so that no other object shares its address and the emitter
  // never names a pointer `(HARGS),Y`.
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
