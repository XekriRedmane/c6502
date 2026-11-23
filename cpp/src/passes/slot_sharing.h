// The port of passes/slot_sharing.py: the shares of parameter slots
// between functions and the bindings of variables to them
// (`find_shares`), what the allocators, `tac_to_asm`, the link metadata,
// and emission read of them, and `check_shares`, which reads the
// finished asm program, and the linker's half, `merge_shares` and
// `check_linked_shares`.
#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

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

// Each function a program defines, in the program's order, with every
// function it can call, directly or not.
using ReachSets = OrderedMap<std::string, NameSet>;

// Returns each function defined in `prog` mapped to every function it
// can call, directly or not. A function without a body here is a leaf.
// A call that abandons its caller isn't followed (call_edges.h), since
// the caller and the functions it was called from are never on the call
// stack with the callee again.
ReachSets reach_sets(const tac_ast::Program& prog);

// Returns whether `a` and `b` can be on the call stack together: they
// are the same function, or one can call the other.
bool coexist(std::string_view a, std::string_view b, const ReachSets& reach);

// Returns the indices in `shares` of the shares whose bytes `fn`'s own
// bytes must avoid, those with a function that can be on the call stack
// with `fn`, in order. Python returns the shares themselves.
std::vector<std::size_t> share_conflicts(std::string_view fn, const std::vector<SlotShare>& shares,
                                         const ReachSets& reach);

// Returns the shares for `prog` (see the top of slot_sharing.cpp), and
// for each function the variables it keeps in a share, each mapped to
// the slot symbols, low byte first, that its operands become.
// `eligible` is the functions with private pools, `fixed` the routines
// with fixed addresses, and `recursion` the functions on a cycle.
// `types` may be null, as Python's default None.
std::pair<std::vector<SlotShare>, Bindings> find_shares(const tac_ast::Program& prog,
                                                        const Abi& abi,
                                                        const SymbolTable& symbols,
                                                        const TypeTable* types,
                                                        const NameSet& eligible,
                                                        const NameSet& fixed,
                                                        const NameSet& recursion);

// Returns each slot symbol in one of `shares` mapped to one spelling for
// its byte, the first member's. The allocators and the linker give a
// share's slots one address, so a store from one of them to another of
// the same byte is a store of the byte to itself.
OrderedMap<std::string, std::string> slot_aliases(const std::vector<SlotShare>& shares);

// Returns, for each zp_abi function defined in `prog`, the slot symbols
// of the parameters it assigns, takes the address of, or indexes by
// name, for the link metadata: a unit that declares one of them `const`
// keeps a value there.
OrderedMap<std::string, std::vector<std::string>> changed_param_slots(const tac_ast::Program& prog,
                                                                      const Abi& abi,
                                                                      const SymbolTable& symbols,
                                                                      const TypeTable* types);

// Returns the `zeropage(addr)` parameters of the zp_abi functions
// defined in `prog` that their function copies into a local of its own
// as it starts (`tac_to_asm._pinned_param_locals`), by function. A
// function keeps such a parameter in its bytes unless something it
// calls might write them while it runs: a function with a
// `zeropage(addr)` parameter on any of the same bytes, since its callers
// store the argument there, or a function this unit doesn't define,
// other than a routine with an `address` (`addressed`), since it might
// call one.
OrderedMap<std::string, NameSet> pinned_param_copies(const tac_ast::Program& prog, const Abi& abi,
                                                     const SymbolTable& symbols,
                                                     const TypeTable* types,
                                                     const NameSet& addressed);

// Returns, for each function, the slot symbols its variables live in,
// which `asm_emit` names a `(zp),Y` pointer by in that function's body,
// so the linker, which binds each symbol again, keeps the pointer where
// the function keeps the value.
OrderedMap<std::string, NameSet> preferred_symbols(const Bindings& bindings);

// Returns the shares of every unit as one program's (the linker's
// `merge_shares`): each slot symbol renamed by `canonical` (a
// declaration's spelling of a parameter's slot to the definition's), and
// shares with a slot in common joined, each with its members, holders,
// live callees, forwards, and writers sorted. Throws
// `CompileError("ValueError", ...)` with Python's message when a
// function would be in a share twice.
std::vector<SlotShare> merge_shares(const std::vector<SlotShare>& shares,
                                    const OrderedMap<std::string, std::string>& canonical);

// Throws `CompileError("ValueError", ...)` with Python's message unless
// rule 2 of slot_sharing.cpp's comment is met for the whole program and
// every member's owner leaves its parameter alone. `calls` are the
// program's call edges, `defined` the functions some unit defines, and
// `changed` each one's assigned parameter slots.
void check_linked_shares(const std::vector<SlotShare>& shares,
                         const std::vector<std::pair<std::string, std::string>>& calls,
                         const NameSet& defined, const OrderedMap<std::string, NameSet>& changed);

}  // namespace c6502::passes
