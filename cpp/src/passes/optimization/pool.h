#pragma once

#include "asdl/runtime.h"

namespace c6502::passes {

// The addresses from `start` up to `stop`, exclusive, `step` apart,
// such as the `window` and `fallback` arguments of the zero-page
// allocators.
struct Range {
  asdl::Int start = 0;
  asdl::Int stop = 0;
  asdl::Int step = 1;

  // Returns whether `x` is one of the range's values.
  bool contains(asdl::Int x) const;
  bool operator==(const Range&) const = default;
};

// The pool of zero-page bytes from `start` to $FF. Its first half,
// `[start, mid)`, is caller-saved and the rest, `[mid, $100)`,
// callee-saved. `start` must be even and in [0, $FF], which `validate`
// checks.
struct Pool {
  asdl::Int start = 0x80;

  // Throws `std::invalid_argument` when `start` is out of range or odd.
  void validate() const;
  // Returns the first callee-saved byte, `0x80 + start / 2`.
  asdl::Int mid() const;
  // Returns the caller-saved bytes, `[start, mid)`.
  Range caller_saved() const;
  // Returns the callee-saved bytes, `[mid, 0x100)`.
  Range callee_saved() const;
  bool operator==(const Pool&) const = default;
};

}  // namespace c6502::passes
