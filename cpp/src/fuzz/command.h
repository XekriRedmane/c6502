// `c6502cpp fuzz`, the command line of the fuzzers, which replaces
// `python -m fuzz.<tool>`.
//
//     c6502cpp fuzz GENERATOR [--start N] [--count N] [--out DIR] [-j N] [--verbose]
//     c6502cpp fuzz programs GENERATOR [--start N] [--count N] --out DIR
//     c6502cpp fuzz run PATH...
//     c6502cpp fuzz reduce [--tac-opt] PATH
//     c6502cpp fuzz tac-passes PATH...
//
// GENERATOR is exprs, calls, stmts, or yarpgen. The first form generates
// the programs of seeds N to N + count - 1 (0 to 99 by default), runs
// each on every path, and writes each program that some path gets wrong
// to DIR (fuzz_out/<generator> by default) as <generator><seed>.c, as
// fuzz/runner.py's `fuzz_main` does. A program that only an asm path
// rejects with a backend limit, while the TAC result is right, counts as
// rejected and isn't saved. `-j N` runs the seeds in N processes, one
// seed to a process, with the output in seed order. `--verbose` also
// writes a line for each program that passed.
//
// `programs` writes the programs without running them, with a line per
// seed giving main's expected return or `None`. `run` runs the given
// files, and the `.c` files of the given directories, and checks each
// against its `// expect: N` line, or else the asm paths against the TAC
// interpreter. `reduce` writes the reduction of PATH next to it as
// <name>.min.c (fuzz/reduce.h). `tac-passes` names the first pass of the
// TAC optimizer after which main returns something else (fuzz/tac_passes.py).
#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace c6502::fuzz {

// Runs the command with `args`, the words after `fuzz`, and returns its
// exit status: 0, 1 when a program was wrong or a run failed, and 2 for
// a usage error. `self` is the path the program was started by, which
// the command starts its workers with and finds c6502's include/ from.
int command(const std::vector<std::string>& args, const std::string& self);

}  // namespace c6502::fuzz
