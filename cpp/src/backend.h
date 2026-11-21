// The `c6502cpp backend` command, through which compile.py and the
// simulator harness run code generation in C++ under
// `C6502_CPP_BACKEND=1` (cpp_backend.py is the Python side).
#pragma once

#include <string>

namespace c6502::backend {

// Returns the response to the request `text`. A request is text with
// LF line endings:
//
//     c6502-backend 2
//     mode <mode>
//     args <tagged JSON, one line>
//     c99 <length>
//     <the type-checked c99 program's dump>
//
// where the length counts the bytes after its line. The arguments are
// an object of the capture harness's tagged JSON
// (tools/capture_passes.py's `encode`): `symbols` and `types`, the
// tables the type checker built, `footprints`, the runtime helpers'
// HARGS footprints (`runtime_layout.helper_footprints()`), `optimize`
// for `-O`, `inline`, false when `-O` doesn't inline the calls of small
// `static` functions (tests/no_inlining.py), `check_ir`, and
// `link_metadata`, for the unoptimized text of `-c`.
//
// Both modes run `passes::codegen` (src/passes/codegen_stage.h), which
// starts with `c99_to_tac`. `codegen-text` goes on to the `-S` text, and
// its response is
//
//     tac <length>              (with check_ir)
//     <the dump of the TAC that code generation lowered>
//     asm <length>              (with check_ir)
//     <the finished asm program's dump>
//     asm2 <length>             (with check_ir)
//     <the asm2 program's dump>
//     text <length>
//     <the -S text>
//
// `codegen` stops before emission, for the simulator harness, which
// assembles the program itself. Its response is
//
//     symbols <JSON [[name, address], ...]>
//     tac <length>              (with check_ir)
//     <the dump of the TAC that code generation lowered>
//     asm <length>
//     <the finished asm program's dump>
//
// where the symbols are the slot symbols and the unit's EQU bindings
// that the harness gives its assembler. The tables the stage changes
// stay in C++, since Python reads nothing of them after the stage.
//
// Throws `std::runtime_error` for a request that can't be read, and
// passes on the exceptions of the passes.
std::string respond(const std::string& text);

}  // namespace c6502::backend
