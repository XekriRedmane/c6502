// The state of a compile that the asm passes read, which the Python
// compiler keeps in module globals: the runtime's zero-page layout
// (`passes/runtime_layout.py`'s `current()`), the user zero-page
// globals (`asm_aliasing.set_user_zp_globals`), and the names that
// `asm_aliasing.pointer_proof` installs while a pass runs. cpp/README.md
// ("The compile's context") describes how a pass receives it.
#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>

#include "asdl/runtime.h"

namespace c6502::passes {

// A set of names that is looked up by `std::string_view`.
using NameSet = std::set<std::string, std::less<>>;

// A map from symbol names to zero-page addresses, such as the
// `zp_slot_symbols` keyword argument of the peephole passes.
using SymbolAddrs = std::map<std::string, asdl::Int, std::less<>>;

// Where the runtime's zero-page block goes (Python's `RuntimeLayout`).
// The defaults are the classic layout, SSP at $00, FP at $02, a 32-byte
// HARGS at $04, and DPTR at $24.
struct RuntimeLayout {
  asdl::Int ssp = 0x00;
  asdl::Int fp = 0x02;
  asdl::Int hargs = 0x04;
  asdl::Int hargs_size = 32;
  asdl::Int dptr = 0x24;

  // Returns the address of the runtime symbol `name` (SSP, FP, HARGS,
  // or DPTR), or null for any other name, as `addrs().get(name)` does.
  const asdl::Int* addr(std::string_view name) const;

  // Returns every byte the block occupies: two each for SSP, FP, and
  // DPTR, and `hargs_size` for HARGS.
  std::set<asdl::Int> bytes() const;

  bool operator==(const RuntimeLayout&) const = default;
};

// The user globals that `[[c6502::zeropage]]` places in zero page for
// the compile, by name, and every byte they occupy (Python's
// `_USER_ZP_GLOBALS` and `_USER_ZP_BYTES`).
struct UserZpGlobals {
  SymbolAddrs addrs;
  std::set<asdl::Int> bytes;
};

// The context of an asm pass. A pass receives it as
// `const AsmContext& ctx` and passes it on to every helper that reads
// it. It is cheap to copy, since the tables it holds are shared and
// never changed, so a pass that installs a pointer proof makes a copy
// with `with_pointer_proof` for the helpers it calls.
struct AsmContext {
  RuntimeLayout layout;
  // Never null. A default context has no user zero-page globals.
  std::shared_ptr<const UserZpGlobals> user_zp =
      std::make_shared<const UserZpGlobals>();
  // The names `may_alias` takes to be out of reach of a pointer, from
  // `unreachable_by_pointer`, or null outside a pass that installs
  // them.
  std::shared_ptr<const NameSet> pointer_proof;

  // Returns whether `name` is a user zero-page global of the compile.
  bool is_user_zp_global(std::string_view name) const;
  // Returns the address of the user zero-page global `name`, or null.
  const asdl::Int* user_zp_address(std::string_view name) const;
  // Returns whether byte `addr` belongs to a user zero-page global.
  bool is_user_zp_byte(asdl::Int addr) const;
  // Returns whether the pointer proof installed holds `name`.
  bool in_pointer_proof(std::string_view name) const;

  // Returns a copy of this context with `names` installed as the
  // pointer proof, as Python's `with pointer_proof(names):` does for
  // the code inside the block.
  AsmContext with_pointer_proof(NameSet names) const;
};

}  // namespace c6502::passes
