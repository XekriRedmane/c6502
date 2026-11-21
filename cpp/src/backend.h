// The `c6502cpp backend` command, through which compile.py and the
// simulator harness run code generation in C++ under
// `C6502_CPP_BACKEND=1` (cpp_backend.py is the Python side).
#pragma once

#include <string>

namespace c6502::backend {

// Returns the response to the request `text`. A request is text with
// LF line endings:
//
//     c6502-backend 1
//     mode <mode>
//     args <tagged JSON, one line>
//     c99 <length>
//     <the type-checked c99 program's dump>
//     tac <length>
//     <the TAC program's dump>
//     optimized-tac <length>    (under -O)
//     <the optimized TAC program's dump>
//
// where each length counts the bytes after its line. The arguments are
// an object of the capture harness's tagged JSON
// (tools/capture_passes.py's `encode`): `symbols` and `types`, the
// compile's tables, `statics`, the names of static storage before the
// TAC optimizer ran, `footprints`, the runtime helpers' HARGS
// footprints (`runtime_layout.helper_footprints()`), `check_ir`, and,
// without -O, `link_metadata`. There are four modes, as
// src/passes/codegen_stage.h describes.
//
// `stage-optimized-text` runs `passes::optimized_stage` on the TAC
// before the TAC optimizer, which `plan_zero_page` reads, and the
// optimized TAC, and `stage-unoptimized-text` runs
// `passes::unoptimized_stage` on the one TAC. Their response is
//
//     new_functions <JSON [[name, owner], ...]>
//     reg_locals <JSON [name, ...]>
//     carry <JSON [name, ...]>
//     asm <length>              (with check_ir)
//     <the finished asm program's dump>
//     asm2 <length>             (with check_ir)
//     <the asm2 program's dump>
//     text <length>
//     <the -S text>
//
// The first three lines are the changes to the symbol table that
// cpp_backend.py enters in Python's: the functions the region made, the
// `<param>.reg` locals of `tac_to_asm`, and the functions given the
// carry-flag return.
//
// `stage-optimized` and `stage-unoptimized` stop before emission, for
// the simulator harness, which assembles the program itself. Their
// response is the same three lines, then
//
//     symbols <JSON [[name, address], ...]>
//     asm <length>
//     <the finished asm program's dump>
//
// where the symbols are the slot symbols and the unit's EQU bindings
// that the harness gives its assembler.
//
// Throws `std::runtime_error` for a request that can't be read, and
// passes on the exceptions of the passes.
std::string respond(const std::string& text);

}  // namespace c6502::backend
