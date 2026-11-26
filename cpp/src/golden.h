// `c6502cpp check-golden`, the check of the C++ compiler against the
// golden files of the corpus (docs/cpp_port.md, "The checks after
// Python"). The frozen Python compiler wrote them in step 8 of the
// port, before it was removed, and from then on `--update` writes them
// from the C++ compiler, for a change to the output made on purpose,
// whose diff of the `-S` texts is the review of the change.
//
// The jobs are each program of the corpus (the examples, the games'
// units, the chapters' programs, and seeds 0 to 99 of `fuzz.stmts`,
// `fuzz.calls`, and `fuzz.exprs`) in each mode (`O`, `unopt`, `cO`,
// `c`, `lex`, `parse`, `resolve`, `resolveO`, `tac`, `tacO`, and
// `tacsim`), the `links` jobs, which compile and link the three games
// and link the damaged files and the bad command lines, and the `cli`
// jobs, which run compile.py's help, its usage errors, and its
// spellings of `-Ttext` at six terminal widths. For each job of each
// manifest, the check makes an empty directory, takes the job's steps
// in it (its directories, its files, changes of the current directory,
// the terminal's width, and its command lines), and runs each command
// line through `c6502cpp compile`'s code (src/compile_command.h), which
// reads it with compile.py's argparse and does what compile.py's
// `main` does, with the C++ compiler, assembler, and linker in the same
// process. It writes the manifest's lines for what each command gave
// and for the files the job left, and compares them with the
// manifest's own, and each text the manifest keeps with the file's
// bytes.
//
// The files are under `golden/` at the repository's root, with LF line
// endings:
//
// - `golden/<mode>.txt`, the manifest of each mode's jobs,
//   `golden/links.txt`, the manifest of the `links` jobs, and
//   `golden/cli.txt`, the manifest of the `cli` jobs.
// - `golden/<mode>/<source>.s`, the `-S` text of each program that
//   compiled, named for its source without `.c`, so that
//   `examples/sfx_tone.c` gives `golden/O/examples/sfx_tone.s`. A fuzz
//   program's text is `golden/<mode>/fuzz/<tool><seed>.s`.
// - `golden/fuzz/<tool><seed>.c`, the source of each fuzz program,
//   which the generators of fuzz/ gave and which the commands compile.
// - `golden/blobs/<sha256>`, the bytes of each file over 120 bytes that
//   a job writes before a command, such as the linker script of the
//   modes `c` and `cO` and the damaged objects of the `links` jobs.
//
// A manifest is text, one line per item:
//
//     c6502-golden 1
//     define <name> <JSON list>
//     text <file name>
//     job <label>
//     mkdir <path>
//     cd <path>
//     columns <n>
//     write <path> text <JSON string>
//     write <path> blob <sha256>
//     run <JSON list, the command line>
//     tacsim <JSON list, a C source and its preprocessor's arguments>
//     status <exit status, or "none" when the command raised>
//     stdout <JSON string>
//     stderr <JSON string>
//     raised <JSON [type, message]>
//     file <path> text <path under golden/>
//     file <path> sha256 <hex digest> <size>
//     end
//
// The `define` lines at the top name lists of arguments that many
// commands share, and a command line holds the name in place of the
// list. The `text` lines name the files of the mode's jobs that are
// kept as text, which are the `-S` outputs, and every other file a job
// leaves is kept as its SHA-256. Each job begins with `job` and ends
// with `end`. `mkdir`, `cd`, `write`, and `run` are its steps, in the
// order the job took them, and the `status` line and the `stdout`,
// `stderr`, and `raised` lines that aren't empty follow each `run`. A
// `columns` step sets the terminal's width (`COLUMNS`) that argparse
// wraps the usage and help to for the commands after it, which is 80
// until a job's first `columns`. The paths of `mkdir`, `cd`, `write`,
// and `file` are relative to the job's directory, and `cd` names the
// directory relative to that one (`.` for itself). After the last step
// come the files the job left, sorted by path.
//
// A job of the mode `tacsim` has one step, `tacsim`, which runs `main`
// of the program on its TAC with the TAC interpreter, and after it the
// line `front <JSON {"type": ..., "message": ...}>` for the front end's
// error, or the line `result <JSON>` of `main`'s result on the
// unoptimized TAC, the line `optimized <JSON>` of its result on the TAC
// after the TAC optimizer when the first was a value, and the line
// `found <JSON>` of fuzz/tac_passes.py's `first_changing_pass`, each in
// the form of the interpreter's response (src/tac_sim_modes.h).
//
// A command names each file of the repository by its path relative to
// the repository's root, such as `examples/zork/main.c`, and the files
// the job writes by their paths in the job's directory. A command's
// argument is such a path if and only if it begins with one of
// `kRootDirs`. The check runs the command with the repository's files
// at `<root>/<path>` and replaces `<root>/` with nothing in what the
// command writes and raises, so the messages name the files as the
// command line does and the manifests are the same on every machine.
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
