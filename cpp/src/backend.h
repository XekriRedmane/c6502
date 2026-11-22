// The `c6502cpp backend` command, through which compile.py and the
// simulator harness run code generation in C++ under
// `C6502_CPP_BACKEND=1` (cpp_backend.py is the Python side).
#pragma once

#include <string>

namespace c6502::backend {

// Returns the response to the request `text`. A request is text with
// LF line endings:
//
//     c6502-backend 3
//     mode <mode>
//     args <JSON, one line>
//     source <length>
//     <the source text>
//
// where the length counts the bytes after its line. The arguments are an
// object with `pcpp_args`, the preprocessor's arguments as a list of
// strings, `filename`, the file the source came from or null for stdin,
// `include_dir`, the directory of c6502's own headers, `preprocess`,
// false when the text is the preprocessor's output already (as
// `compile._run_stage` is given it), `footprints`, the runtime helpers'
// HARGS footprints in the capture harness's tagged JSON
// (`runtime_layout.helper_footprints()`), `optimize` for `-O`, `inline`,
// false when `-O` doesn't inline the calls of small `static` functions
// (tests/no_inlining.py), `check_ir`, and `link_metadata`, for the
// unoptimized text of `-c`.
//
// Both modes run the front end (`passes::front_end`, compile.py's
// `_compile_c` through `type_check_program`) and then `passes::codegen`
// (src/passes/codegen_stage.h). Every response begins
//
//     stderr <length>
//     <what the preprocessor wrote to stderr>
//     warnings <JSON [[where, message], ...]>
//
// with the warnings the compile issued. A compile that a pass rejects
// (a `CompileError`) ends there with
//
//     error <JSON {"type": ..., "message": ...}>
//
// the Python class of the exception and its message. `source-text` goes
// on to the `-S` text, and the rest of its response is
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
// `source` stops before emission, for the simulator harness, which
// assembles the program itself. The rest of its response is
//
//     symbols <JSON [[name, address], ...]>
//     tac <length>              (with check_ir)
//     <the dump of the TAC that code generation lowered>
//     asm <length>
//     <the finished asm program's dump>
//
// where the symbols are the slot symbols and the unit's EQU bindings
// that the harness gives its assembler. The program and its tables stay
// in C++, since Python reads nothing of them after the stage.
//
// Throws `std::runtime_error` for a request that can't be read, and
// passes on the `InternalError`s of the passes.
std::string respond(const std::string& text);

}  // namespace c6502::backend
