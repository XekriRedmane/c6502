// The `c6502cpp backend` command, through which compile.py's shim, the
// simulator harness, and the fuzzers compile a source and run the TAC
// interpreter, and compile.py's `-c` and its links (cpp_backend.py is
// the Python side).
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
// `compile._run_stage` is given it), `optimize` for `-O`, `inline`,
// false when `-O` doesn't inline the calls of small `static` functions
// (tests/no_inlining.py), `check_ir`, which turns on the IR checks of
// src/passes/ir_check.h for the request, and `link_metadata`, for the
// unoptimized text of `-c`. A failed IR check throws `InternalError`,
// which ends the process with status 3, as any other internal error
// does. The runtime helpers' HARGS footprints come
// from the copy of rtlib/helpers.py compiled into the program
// (src/rtlib/helpers.h).
//
// The modes `source` and `source-text` run the front end
// (`passes::front_end`, compile.py's `_compile_c` through
// `type_check_program`) and then `passes::codegen`
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
//     text <length>
//     <the -S text>
//
// `source` stops before emission, for the simulator harness, which
// assembles the program itself. The rest of its response is
//
//     symbols <JSON [[name, address], ...]>
//     asm <length>
//     <the finished asm program's dump>
//
// where the symbols are the slot symbols and the unit's EQU bindings
// that the harness gives its assembler. The program and its tables stay
// in C++, since Python reads nothing of them after the stage.
//
// `lex` is `compile.py --lex`, which runs only the preprocessor (with
// `preprocess`) and the lexer, and ignores `optimize`, `inline`,
// `check_ir`, and `link_metadata`. Its response has the `stderr` and
// `warnings` lines, and then the `error` line of a `PreprocessorError`
// or a `LexError`, or
//
//     text <length>
//     <one `<line>:<col>`, tab, kind, tab, value line for each token>
//
// `parse`, `resolve`, and `tac` are `compile.py --parse`, `--resolve`,
// and `--tac` (src/passes/text_stages.h), which run the preprocessor
// (with `preprocess`) and the front end to the point where compile.py's
// `_run_stage` stops, with `-O` for `resolve` and `tac`, and `inline`
// for `tac -O`. `link_metadata` doesn't apply. Their response has the
// `stderr` and `warnings` lines, and then the `error` line of the pass
// that rejected the program, or
//
//     text <length>
//     <pretty.py's text of the tree, and a newline>
//
// The modes, in the order of compile.py's stages, are `lex`, `parse`,
// `resolve`, `tac`, `source-text` (`-S`), `source` (the simulator
// harness), `objects` (`-c`), and `link`.
//
// The modes `objects` (compile.py's `-c`) and `link` take several inputs,
// so that one process serves one run of compile.py. Their request is
//
//     c6502-backend 3
//     mode objects | link
//     args <JSON, one line>
//     input <length>
//     <the input's text, UTF-8, or an object's bytes>
//     ...
//
// with one `input` section for each of the arguments' `inputs`, in their
// order. The arguments are `pcpp_args`, `include_dir`, `optimize`,
// `inline`, and `check_ir` as above, and `inputs`, a list of objects
// with `kind` ("c", "asm", or for a link "object"), `name` (the file
// name the assembler gives the object, `<stem>.c` or the `.asm` file's
// name, or the object's path, which `elf.read`'s messages name),
// `filename` (the preprocessor's file name of a C input, null for
// stdin), and for a link `link_name`, the name the object goes by in
// the link, which a script's file patterns match. A link also takes `run_link`, false when compile.py stops
// after these inputs because the next one can't be read, `script` (a
// path or null), `origin` (null for $0800), `defsyms` (`[name, value]` pairs in the
// command line's order), and `output`, the executable's path.
//
// Each input is compiled and assembled, assembled, or read in order, as
// compile.py's `_object` does, and its part of the response begins
//
//     input <k>
//
// followed, for a C input, by the `stderr` and `warnings` lines above.
// Then it has one of
//
//     error <JSON {"type": ..., "message": ...}>
//     write-error <JSON {"type": ..., "message": ...}>
//     object <length>           (objects)
//     <the object's bytes>
//     ok                        (link)
//
// where `error` is the exception of the compile, the assembler, or
// `elf.read`, and `write-error` is the `struct.error` (type
// "struct.error") that Python's `elf.write` raises after compile.py
// has opened the output file. Either ends the response. After the
// inputs of a link come one of
//
//     link-error <JSON {"type": ..., "message": ...}>
//     write-error <JSON {"type": ..., "message": ...}>
//     output-error <JSON {"errno": ..., "strerror": ...}>
//     executable <length>
//
// for an error of the script or the link, a `struct.error` of the
// executable, an output file that couldn't be opened or written, and
// the length of the executable the command wrote to `output`.
//
// The modes `tac-run`, `tac-observe`, and `tac-falls-off` run the TAC
// interpreter on a source, with the request of the mode `source`
// (src/tac_sim_modes.h describes their responses).
//
// Throws `std::runtime_error` for a request that can't be read, and
// passes on the `InternalError`s of the passes.
std::string respond(const std::string& text);

}  // namespace c6502::backend
