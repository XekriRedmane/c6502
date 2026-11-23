// The `c6502cpp link` command, through which compile.py can run the
// link of its objects in C++ (group F of step 7, docs/cpp_port.md,
// connects it).
#pragma once

#include <string>

namespace c6502::binutils {

// Returns the response to the request `text`. A request is text with LF
// line endings:
//
//     c6502-link 1
//     args <JSON, one line>
//     object <length>
//     <the object's bytes>
//     ...
//
// with one `object` section for each name the arguments list, in the
// order of the link, each length counting the bytes after its line. The
// arguments are an object with `objects`, the names the objects go by
// (compile.py's object path, or `<stem>.o` for a C or `.asm` input),
// `script`, the path of the linker script or null, `origin`, and
// `defsyms`, a list of `[name, value]` pairs in the order of the
// command line. compile.py reads the objects first and then the script,
// so the command reads them in that order. The response is
//
//     executable <length>
//     <the executable's bytes>
//
// or, when the link fails,
//
//     error <JSON {"type": ..., "message": ..., "input": ...}>
//
// with the Python class of the exception and its message. `input` is the
// position of the object `elf.read` rejects, for which compile.py
// reports `<name>: error: <message>`, and null for an error of the
// script or of the link, which it reports as `compile.py: link error:
// <message>`.
//
// With `repeat` above 1 the link runs that many times on the request's
// bytes, from the reading of the objects to the writing of the
// executable, and `best_ms`, when not null, is set to the shortest run
// in milliseconds. Throws `std::runtime_error` for a request that can't
// be read, and passes on an `InternalError`.
std::string link_response(const std::string& text, int repeat = 1, double* best_ms = nullptr);

}  // namespace c6502::binutils
