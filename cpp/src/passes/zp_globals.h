// The port of passes/zp_globals.py. `allocate_zp_globals`,
// `plan_zero_page`, and `access_weights` (zp_globals_plan.cpp) decide
// where each zero-page object goes before code generation, and the work
// that reads the finished asm program (zp_globals.cpp) is
// `place_zp_globals`, which drops the storage of the zero-page objects
// and adds `__zp_init`, and `link_records`, which lists them for the
// link metadata.
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/asm_context.h"
#include "passes/runtime_layout.h"
#include "passes/type_checking.h"
#include "passes/zp_link_metadata.h"
#include "tac_ast.h"

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

// Returns where every `zeropage` object of `symbols` goes: the address
// it was declared with, or the lowest free run below $80. The fixed
// addresses are placed first, in the symbol table's order, then the
// runtime's block (`choose_layout`, which calls `hargs_need` for the
// number of HARGS bytes the program needs when the block has to move,
// and takes the classic 32 when `hargs_need` is empty), then the rest.
// An `extern` with no address and no definition goes in `external`
// instead. With `weights` (the `access_weights` of the unit), the
// globals that pay for it are then promoted into the bytes left. Also
// collects the `address(addr)` functions and objects outside zero page
// into `fixed`. Throws `CompileError("ZpGlobalError", ...)` for an
// address out of range, an overlap, or a full range, and
// `CompileError("RuntimeLayoutError", ...)` when the runtime's block
// doesn't fit.
ZpGlobals allocate_zp_globals(const SymbolTable& symbols, const TypeTable* types,
                              const HargsNeed& hargs_need,
                              const OrderedMap<std::string, asdl::Int>* weights);

// Returns each name's weighted count of references in the TAC program
// `prog`: every mention in an instruction other than `GetAddress`,
// `Label`, or a jump's target, times 8 for each loop around it, up to 3
// deep. A loop is the span from a label to a jump back to it. The names
// are in the order of their first mention, as Python's dict holds them.
OrderedMap<std::string, asdl::Int> access_weights(const tac_ast::Program& prog);

// Returns `allocate_zp_globals` for a unit whose unoptimized TAC is
// `prog`, with the weights of `access_weights` when `promote` (under
// -O). Python sizes HARGS, when the runtime's block has to move, by
// translating `prog` to asm with `tac_to_asm` and measuring it with
// `hargs_need`. `unoptimized_hargs_need` stands for that measurement,
// since `tac_to_asm` is ported by group C of step 4, and group F
// connects it to that port when it builds the stage's driver. The
// registry's runner gives it the HARGS size the record's result holds.
ZpGlobals plan_zero_page(const tac_ast::Program& prog, const SymbolTable& symbols,
                         const TypeTable* types, bool promote,
                         const HargsNeed& unoptimized_hargs_need);

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
