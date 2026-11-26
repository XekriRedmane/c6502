#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>

#include "asdl/runtime.h"

namespace c6502::passes {

using NameSet = std::set<std::string, std::less<>>;

using SymbolAddrs = std::map<std::string, asdl::Int, std::less<>>;

// Where the runtime's zero-page block goes. The defaults are the
// classic layout, SSP at $00, FP at $02, a 32-byte HARGS at $04, and
// DPTR at $24.
struct RuntimeLayout {
  asdl::Int ssp = 0x00;
  asdl::Int fp = 0x02;
  asdl::Int hargs = 0x04;
  asdl::Int hargs_size = 32;
  asdl::Int dptr = 0x24;

  // Returns the address of the runtime symbol `name` (SSP, FP, HARGS,
  // or DPTR), or null for any other name.
  const asdl::Int* addr(std::string_view name) const;

  // Returns every byte the block occupies: two each for SSP, FP, and
  // DPTR, and `hargs_size` for HARGS.
  std::set<asdl::Int> bytes() const;

  bool operator==(const RuntimeLayout&) const = default;
};

// The user globals that `[[c6502::zeropage]]` places in zero page for
// the compile, by name, and every byte they occupy.
struct UserZpGlobals {
  SymbolAddrs addrs;
  std::set<asdl::Int> bytes;
};

// The context of an asm pass. The tables it holds are shared and never
// changed, so a copy is cheap.
struct AsmContext {
  RuntimeLayout layout;
  // Never null. A default context has no user zero-page globals.
  std::shared_ptr<const UserZpGlobals> user_zp =
      std::make_shared<const UserZpGlobals>();
  // The names `may_alias` takes to be out of reach of a pointer, from
  // `unreachable_by_pointer`, or null outside a pass that installs
  // them.
  std::shared_ptr<const NameSet> pointer_proof;

  bool is_user_zp_global(std::string_view name) const;
  // Returns the address of the user zero-page global `name`, or null.
  const asdl::Int* user_zp_address(std::string_view name) const;
  bool is_user_zp_byte(asdl::Int addr) const;
  bool in_pointer_proof(std::string_view name) const;

  // Returns a copy of this context with `names` installed as the
  // pointer proof.
  AsmContext with_pointer_proof(NameSet names) const;
};

}  // namespace c6502::passes
