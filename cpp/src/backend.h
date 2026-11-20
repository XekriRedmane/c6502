// The `c6502cpp backend` command, through which compile.py runs the
// back end in C++ under `C6502_CPP_BACKEND=1` (cpp_backend.py is the
// Python side).
#pragma once

#include <string>

namespace c6502::backend {

// Returns the response to the request `text`. A request is text with
// LF line endings:
//
//     c6502-backend 1
//     mode <mode>
//     context <JSON, one line>
//     args <tagged JSON, one line>
//     input
//     <the input program's dump>
//
// The context is the capture harness's (tools/capture_passes.py's
// `context_json`), and the arguments are an object of keyword
// arguments in its tagged JSON (`encode`). There are four modes.
//
// `optimized` runs the region of `optimized_asm` on the program after
// `synthesize_prologue`, with the arguments `slot_symbols`, `zp_addrs`,
// `noreturn`, `abi`, `shares`, `ret_eligible`, `zp_names`, and
// `slot_addrs`, as `passes::RegionArgs` describes. `unoptimized` runs
// the unoptimized fixed point and long branches, with `zp_addrs`. Their
// response is
//
//     coalesced <JSON [[function, [slot, ...]], ...]>
//     new_functions <JSON [[name, owner], ...]>
//     output
//     <the result's dump>
//
// where an unoptimized run leaves out the first two lines. The
// simulator harness uses these, since it runs the asm program.
//
// `optimized-text` and `unoptimized-text` run the same and then the work
// through emission (`passes::optimized_text`, `unoptimized_text`), with
// the arguments `zp`, `footprints`, `sections`, `equs`, `meta`,
// `check_ir`, and under `-O` `preferred_symbols`, as `passes::EmitArgs`
// describes. Their response is
//
//     new_functions <JSON [[name, owner], ...]>
//     asm <length>              (with check_ir)
//     <the finished asm program's dump>
//     asm2 <length>             (with check_ir)
//     <the asm2 program's dump>
//     text <length>
//     <the -S text>
//
// where each length counts the bytes after its line.
//
// Throws `std::runtime_error` for a request that can't be read, and
// passes on the exceptions of the passes.
std::string respond(const std::string& text);

}  // namespace c6502::backend
