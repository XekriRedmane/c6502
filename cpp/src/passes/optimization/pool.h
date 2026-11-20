// The port of passes/optimization/pool.py, the zero-page pool that
// register allocation draws from, and the Python `range` values that
// the allocators take and return.
#pragma once

#include "asdl/runtime.h"

namespace c6502::passes {

// A Python `range(start, stop, step)`, such as the `window` and
// `fallback` arguments of the zero-page allocators.
struct Range {
  asdl::Int start = 0;
  asdl::Int stop = 0;
  asdl::Int step = 1;

  // Returns whether `x` is one of the range's values, as `x in r`
  // does. A step of 0 is never made, since Python rejects it.
  bool contains(asdl::Int x) const;
  bool operator==(const Range&) const = default;
};

// The pool of zero-page bytes from `start` to $FF. Its first half,
// `[start, mid)`, is caller-saved and the rest, `[mid, $100)`,
// callee-saved. `start` must be even and in [0, $FF], which Python's
// constructor checks and `validate` checks here.
struct Pool {
  asdl::Int start = 0x80;

  // Throws `std::invalid_argument` with the message of Python's
  // `ValueError` when `start` is out of range or odd.
  void validate() const;
  // Returns the first callee-saved byte, `0x80 + start // 2`.
  asdl::Int mid() const;
  // Returns the caller-saved bytes, `[start, mid)`.
  Range caller_saved() const;
  // Returns the callee-saved bytes, `[mid, 0x100)`.
  Range callee_saved() const;
  bool operator==(const Pool&) const = default;
};

}  // namespace c6502::passes
