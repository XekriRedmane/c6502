// `link_objects` and `link_files` link c6502's relocatable objects into
// an executable, in the order the comment at the top of linker.cpp gives.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "binutils/elf.h"
#include "binutils/ld.h"
#include "passes/link_script.h"

namespace c6502::passes::linker {

// Where a link without a script puts `.text`.
inline constexpr std::int64_t kDefaultOrigin = 0x0800;

// Returns the executable that links `inputs`, each an object and the
// name it goes by, which the script's file patterns match without its
// directory. `script` is a parsed linker script, or none for the
// default layout from `origin`, which must be in $0000-$FFFF.
// `defsyms` gives addresses for names no object defines, and a runtime
// helper it names isn't taken from the runtime library. The objects are
// the caller's, and the link renames and redefines their symbols in
// place. Throws `CompileError` of class "LinkError" for a problem with
// the program, and "ZpSlotAllocationError" or "ZpLocalAllocationError"
// when the merged program's slots don't fit.
binutils::elf::ElfFile link_objects(binutils::ld::Objects inputs,
                                    const std::optional<link_script::Script>& script,
                                    asdl::Int origin = kDefaultOrigin,
                                    const binutils::ld::Names& defsyms = {});

// Links the objects at `input_paths` into the executable at
// `output_path`, laid out by the linker script at `script_path` if one
// is given. Throws `CompileError("LinkError",
// ...)` for an object that can't be read, with `ElfError`'s message or
// `<path>: <the OSError's strerror>`, and `link_objects`'s errors.
void link_files(const std::vector<std::string>& input_paths, const std::string& output_path,
                const std::optional<std::string>& script_path,
                std::int64_t origin = kDefaultOrigin, const binutils::ld::Names& defsyms = {});

}  // namespace c6502::passes::linker
