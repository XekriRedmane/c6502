// The parts of passes/zp_globals.py that read the finished asm program:
// `place_zp_globals`, which drops the storage of the zero-page objects
// and adds `__zp_init`, and `link_records`, which lists them for the
// link metadata. Where each object goes is decided in Python
// (`allocate_zp_globals`) before the back end runs.
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/asm_context.h"
#include "passes/zp_link_metadata.h"

namespace c6502::passes {

// The name of the function that stores the initial bytes of the
// unit's zero-page objects, which the boot code calls before `main`.
inline constexpr const char* kZpInitFunction = "__zp_init";

// Each 6502 helper's HARGS footprint in bytes, counting the helpers it
// calls (`runtime_layout.helper_footprints`, which Python measures from
// rtlib/helpers.py).
using HelperFootprints = std::map<std::string, asdl::Int, std::less<>>;

// The zero-page objects of a unit (Python's `ZpGlobals`, whose
// docstring gives each field's meaning), with its fields in Python's
// order. The maps keep the order of Python's dicts, which the tagged
// JSON of a capture writes, and the functions that list the objects
// sort them as Python does.
struct ZpGlobals {
  // Each zero-page object's address.
  OrderedMap<std::string, asdl::Int> addrs;
  // Every byte the objects occupy, which the allocators avoid.
  std::set<asdl::Int> reserved;
  // The objects the unit defines, whose initial values `__zp_init`
  // stores.
  NameSet owned;
  // Each function and object placed with `address(addr)` at $100 or
  // above, with its address, which gets only an `EQU` line.
  OrderedMap<std::string, asdl::Int> fixed;
  // The runtime's zero-page layout, whose HARGS size `check_hargs`
  // checks the program against.
  RuntimeLayout layout;
  // Each object's size in bytes.
  OrderedMap<std::string, asdl::Int> sizes;
  // The objects declared with an address.
  NameSet pinned;
  // Each `extern` object that another unit defines and the linker
  // places, with its size.
  OrderedMap<std::string, asdl::Int> external;
  // Each object `-O` moved into zero page, with its weight.
  OrderedMap<std::string, asdl::Int> promoted;
  // The bytes of the `zeropage(addr)` parameters, which are in
  // `reserved` too.
  std::set<asdl::Int> param_bytes;

  bool operator==(const ZpGlobals&) const = default;
};

// Returns `prog` without the `StaticVariable` of any zero-page object,
// and with a function `__zp_init` that stores the initial bytes of
// those the unit defines. Stores of one value share one `LDA`, zero
// first. A program with no zero-page object comes back unchanged, and
// an object whose `StaticVariable` an earlier pass dropped (since
// nothing read it) isn't initialized. First checks that `prog` fits the
// HARGS size of the layout (`check_hargs`).
asm_ast::Program place_zp_globals(asm_ast::Program prog, const ZpGlobals& zp,
                                  const HelperFootprints& footprints);

// Returns the link metadata's `zp` records of the zero-page objects of
// `prog`, the program before `place_zp_globals`: the unit's objects in
// order of address and then name, then the external ones by name. A
// defined object's initial bytes are written as `asm_emit` writes
// immediates (`$05`, `<buf`, `>(buf+2)`), so the linker can store them
// without knowing the kinds of initializers.
std::vector<ZpObjectMeta> link_records(const asm_ast::Program& prog, const ZpGlobals& zp);

// Returns the number of HARGS bytes `prog` needs: its own highest
// offset into HARGS, and the footprint of each helper it calls
// (`runtime_layout.hargs_need`).
asdl::Int hargs_need(const asm_ast::Program& prog, const HelperFootprints& footprints);

// Throws `CompileError("RuntimeLayoutError", ...)` when the finished
// `prog` uses more of HARGS than `layout` gave it
// (`runtime_layout.check_hargs`). The size is measured on the
// unoptimized code, so that would mean an optimizer pass called a
// helper with a larger footprint than any the unoptimized code called.
void check_hargs(const asm_ast::Program& prog, const RuntimeLayout& layout,
                 const HelperFootprints& footprints);

}  // namespace c6502::passes
