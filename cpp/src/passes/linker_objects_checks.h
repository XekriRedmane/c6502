// The checks of the records that tools/capture_binutils.py writes of
// binutils/ld.py (`global_definitions`, `relax`, and `executable`) and of
// the functions of passes/linker.py on the objects (`_runtime_library`,
// `_object_items`, `_item_symbol`, `_without_init`, `_unit`,
// `_apply_renames`, `_pointer_names`, `_reassemble`, `_output_symbols`,
// `_merge_identical`, and `_contents`), which `c6502cpp check-binutils`
// runs.
//
// A link changes its objects in place between these calls, so a record
// names each object by the hash of a JSON form of its sections,
// symbols, relocations, and segments, which keeps the order and the
// identities an ELF file can't (a symbol out of the table's order, a
// relocation of a symbol the table has lost). The harness's docstring
// gives the form, and `elf_json` and `elf_from_json` here read and
// write it.
#pragma once

#include <functional>
#include <string>
#include <string_view>

#include "binutils/elf.h"
#include "json.h"

namespace c6502::passes::linker_objects_checks {

// Returns the text of the JSON form of `obj`, as the harness's
// `_elf_json` writes it with `json.dumps`.
std::string elf_json(const binutils::elf::ElfFile& obj);

// Returns the object whose JSON form is `v`. Throws `InternalError` for
// JSON of another shape.
binutils::elf::ElfFile elf_from_json(const json::Value& v);

// Returns the bytes of the file a capture stores under a hash.
using BlobReader = std::function<std::string(const std::string&)>;

// Returns whether records of `kind` are checked here.
bool handles(std::string_view kind);

// Runs the C++ function of a record of `kind` on its arguments and
// input, and returns what differs from the recorded `output` (an error
// `{"type", "message"}` when `is_error`), or the empty string. `blob`
// reads the objects the record names.
std::string check(std::string_view kind, const json::Value& args, const json::Value& input,
                  const json::Value& output, bool is_error, const BlobReader& blob);

}  // namespace c6502::passes::linker_objects_checks
