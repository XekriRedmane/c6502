// The link metadata block of passes/zp_link_metadata.py: its records,
// `build_metadata`, which collects the records that read the TAC
// program, the ABI, and the pools, and `format_metadata`, which writes
// them as the `; @zp-link-meta` comment lines. The records that read the
// finished asm program are filled in by emission (cpp/README.md,
// "Emission").
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/slot_sharing.h"
#include "tac_ast.h"

namespace c6502::passes {

// A function the unit defines (Python's `FunctionMeta`). `params` are
// its zp_abi parameter slots, one per byte, `locals` its body-local
// slots, one per byte of its pool, `param_regs` the register of each
// parameter byte ("" for a slot), `pinned` the address of each byte of
// a `zeropage(addr)` parameter, and `changes` the parameter slots it
// assigns.
struct FunctionMeta {
  std::string name;
  std::vector<std::string> params;
  std::vector<std::string> locals;
  bool indirect = false;
  bool in_cycle = false;
  std::vector<std::string> param_regs;
  std::vector<std::optional<asdl::Int>> pinned;
  std::vector<std::string> changes;
  bool carry = false;
};

// A zp_abi function the unit declares without defining it (Python's
// `ExternMeta`).
struct ExternMeta {
  std::string name;
  std::vector<std::string> params;
  std::vector<std::string> param_regs;
  std::vector<std::optional<asdl::Int>> pinned;
  bool carry = false;
};

// A function or object the unit defines, with its section (Python's
// `ItemMeta`).
struct ItemMeta {
  std::string name;
  std::string section;
  bool local = false;
  bool zero = false;
  bool merge = false;

  bool operator==(const ItemMeta&) const = default;
};

// A zero-page object of the unit (Python's `ZpObjectMeta`). `at` is its
// address when it was declared with one, `init` its initial bytes as
// `LDA #` operands when the unit defines it, and `weight` set when `-O`
// promoted it into zero page.
struct ZpObjectMeta {
  std::string name;
  asdl::Int size = 0;
  std::optional<asdl::Int> at;
  bool defined = false;
  std::vector<std::string> init;
  std::optional<asdl::Int> weight;
  bool local = false;

  bool operator==(const ZpObjectMeta&) const = default;
};

// The unit's metadata (Python's `LinkMetadata`).
struct LinkMetadata {
  std::vector<FunctionMeta> defs;
  std::vector<ExternMeta> externs;
  std::vector<std::pair<std::string, std::string>> calls;
  // The edges in `calls` whose every call saves the caller's values on
  // the hardware stack.
  std::vector<std::pair<std::string, std::string>> saved_calls;
  std::vector<ZpObjectMeta> zp_objects;
  std::optional<asdl::Int> hargs_size;
  std::optional<asdl::Int> hargs_need;
  std::vector<ItemMeta> items;
  std::vector<SlotShare> shares;
};

// Returns the metadata block as lines of the output, each a comment
// that dasm skips, from `; @zp-link-meta-begin` to
// `; @zp-link-meta-end`. A `param_regs` list is written only when one
// entry isn't empty, which keeps the block readable by the units from
// before the register parameters.
std::vector<std::string> format_metadata(const LinkMetadata& meta);

// Returns the metadata of the block in the UTF-8 text `asm_text` (a
// `.asm` file's text or an object's `.c6502.link` section), or an empty
// `LinkMetadata` when it holds no block. The lines are split as
// `str.splitlines` splits them, each record is split at Unicode spaces,
// and its numbers are read as `int()` reads them, so a block Python
// reads gives the same records. A malformed block throws
// `CompileError("ValueError", ...)` with Python's message, from one of
// the thirteen messages of `parse_metadata` or from `int()`, and a
// record without a field Python reads without a default throws
// `CompileError("KeyError", "'<field>'")`, the `str()` of Python's
// KeyError. A number of more than 128 bits throws the OverflowError of
// src/py_int.h.
LinkMetadata parse_metadata(std::string_view asm_text);

// Returns the records of `prog`'s functions, its zp_abi externs, and
// its call edges, from the TAC program and the allocators' results, run
// after `select_abi`, `allocate_zp_slots`, and
// `allocate_function_locals`. A defined function's `locals` are the
// slot symbols `slot_names_by_fn` gives its pool's bytes, which the
// linker reuses when it writes the program's `EQU` lines, or numeric
// names `__local_<fn>__<k>` for each byte of its pool when it has none.
// `changes` gives the parameter slots each function assigns, and `carry`
// names the functions that return a `bool` in the carry flag. Any of the
// three may be null, as Python's None. The defs and externs are by name,
// and the calls sorted.
LinkMetadata build_metadata(const tac_ast::Program& prog, const Abi& abi,
                            const OrderedMap<std::string, std::vector<asdl::Int>>& local_pools,
                            const OrderedMap<std::string, std::vector<std::string>>* slot_names_by_fn,
                            const OrderedMap<std::string, std::vector<std::string>>* changes,
                            const NameSet* carry);

}  // namespace c6502::passes
