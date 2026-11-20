// The parts of passes/slot_sharing.py ported so far: the shares of
// parameter slots between functions and the bindings of variables to
// them, which other passes read, and `check_shares`, which reads the
// finished asm program. Finding the shares (`find_shares`) reads the TAC
// and stays in Python for now.
#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Bytes several functions use as one value's home (Python's
// `SlotShare`). `members` pairs each function whose parameter slots are
// in it with those slots, low byte first, all of one width. `holders`
// are the functions that keep a local there. `live`, `forwards`, and
// `writers` are for the linker: each function with a value there paired
// with the callees it is live across, the (caller, owner) pairs whose
// every call passes the caller's own value, and the holders that assign
// theirs. Each is a tuple in Python, in the order `find_shares` made it.
struct SlotShare {
  std::vector<std::pair<std::string, std::vector<std::string>>> members;
  std::vector<std::string> holders;
  std::vector<std::pair<std::string, std::vector<std::string>>> live;
  std::vector<std::pair<std::string, std::string>> forwards;
  std::vector<std::string> writers;

  // Returns the number of bytes of the share, the width of its first
  // member's slots.
  std::size_t width() const;
  // Returns the members and the holders.
  NameSet functions() const;
  // Returns the members.
  NameSet owners() const;
  // Returns every member's slot symbols.
  NameSet slots() const;
  bool operator==(const SlotShare&) const = default;
};

// For each function, the variables it keeps in a share, each mapped to
// the slot symbols, low byte first, that its operands become (the
// second result of `find_shares`).
using Bindings = OrderedMap<std::string, OrderedMap<std::string, std::vector<std::string>>>;

// Throws `InternalError` with Python's message when the finished `prog`
// breaks what the shares rest on: a function writes one of its own
// parameter slots that is in a share. `find_shares` admits only owners
// that never assign the parameter, so such a write would be one that a
// pass after it made up.
void check_shares(const asm_ast::Program& prog, const std::vector<SlotShare>& shares);

}  // namespace c6502::passes
