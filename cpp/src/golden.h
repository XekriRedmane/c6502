// `c6502cpp check-golden`, the check of the C++ compiler against the
// golden files that tools/write_golden.py wrote with the frozen Python
// compiler (docs/cpp_port.md, "The checks after Python").
//
// The golden files are under `golden/` at the repository's root:
// `golden/<manifest>.txt`, one manifest for each mode of the corpus's
// programs and `links.txt` for the games and the bad command lines,
// and beside them the `-S` texts, the fuzz programs' sources, and the
// blobs the jobs write. tools/write_golden.py's docstring gives the
// format. For each job of each manifest, the check makes an empty
// directory, takes the job's steps in it (its directories, its files,
// changes of the current directory, the terminal's width, and its
// command lines), and runs each command line through `c6502cpp
// compile`'s code (src/compile_command.h), which reads it with
// compile.py's argparse and does what compile.py's `main` does under
// `C6502_CPP_BACKEND=1`, with the C++ compiler, assembler, and linker
// in the same process and no Python. It writes the manifest's lines
// for what each command gave and for the files the job left, and
// compares them with the manifest's own, and each text the manifest
// keeps with the file's bytes.
//
// The jobs run in worker processes, each a `c6502cpp check-golden
// --worker` of its share of the jobs, since a job changes the current
// directory. With `--update`, the check writes the manifests and the
// texts again from what the C++ compiler gave, for a change made on
// purpose, and removes the texts no manifest names any longer.
#pragma once

#include <string>
#include <vector>

namespace c6502::golden {

// Runs `c6502cpp check-golden` with the arguments after the command:
//
//     check-golden [--root <dir>] [-j <n>] [--manifest <name>,...]
//                  [--label <label>] [--update] [--verbose]
//
// `--root` is the repository's root (by default the first directory
// from the current one upward that holds `golden/`), `-j` the number of
// worker processes, `--manifest` the manifests to check (all of them
// by default), and `--label` one job's label. Prints a `DIFFER` line
// for each job whose lines differ, a line per manifest with its counts,
// and returns 0 when no job differs. `self` is the path the program
// was started by, which the workers are started with.
int check_golden(const std::vector<std::string>& args, const std::string& self);

}  // namespace c6502::golden
