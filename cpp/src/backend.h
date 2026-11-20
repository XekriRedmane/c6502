// The `c6502cpp backend` command, through which compile.py runs the
// back end's region in C++ under `C6502_CPP_BACKEND=1`
// (cpp_backend.py is the Python side).
#pragma once

#include <string>

namespace c6502::backend {

// Returns the response to the request `text`. A request is text with
// LF line endings:
//
//     c6502-backend 1
//     mode optimized            (or unoptimized)
//     context <JSON, one line>
//     args <tagged JSON, one line>
//     input
//     <the input program's dump>
//
// The context is the capture harness's (tools/capture_passes.py's
// `context_json`), and the arguments are an object of keyword
// arguments in its tagged JSON (`encode`). Under `optimized` they are
// `slot_symbols`, `zp_addrs`, `noreturn`, `abi`, `shares`,
// `ret_eligible`, `zp_names`, and `slot_addrs`, as
// `passes::RegionArgs` describes, and under `unoptimized` only
// `zp_addrs`. The response is
//
//     coalesced <JSON [[function, [slot, ...]], ...]>
//     new_functions <JSON [[name, owner], ...]>
//     output
//     <the result's dump>
//
// where an unoptimized run leaves out the first two lines. Throws
// `std::runtime_error` for a request that can't be read, and passes on
// the exceptions of the passes.
std::string respond(const std::string& text);

}  // namespace c6502::backend
