// nm.py, which lists the symbols of c6502 objects and executables as
// GNU nm does (step 7, docs/cpp_port.md).
//
// Each line is the symbol's value, a letter for its kind, and its name:
//
//     U   undefined (another file defines it, or the linker places it)
//     A   absolute
//     T   in a code section
//     R   in a read-only data section
//     D   in a writable data section
//     B   in an uninitialized (`.bss`) section
//
// The letter is lowercase for a local symbol. A value is 4 hex digits,
// since addresses on the 6502 are 16 bits, and an undefined symbol has
// blanks in its place. Like GNU nm, the list is sorted by name.
//
// Local labels (names that start with `.`), the `$c` and `$d` mapping
// symbols, and file symbols are left out unless `-a` is given, as GNU nm
// leaves out the assembler's local labels and ARM's mapping symbols.
// `U` symbols marked with `STO_MOS_ZEROPAGE` (the unit reaches them with
// zero-page addressing) are listed with a `z` after the letter when `-a`
// is given.
#pragma once

#include <functional>
#include <string>
#include <vector>

#include "binutils/elf.h"

namespace c6502::binutils::nm {

enum class Sort { kName, kNumeric, kNone };

// The options of `symbol_lines`, with Python's defaults.
struct Options {
  bool debug = false;
  bool extern_only = false;
  bool undefined_only = false;
  bool defined_only = false;
  Sort sort = Sort::kName;
  bool reverse = false;
  bool size = false;
  std::string prefix;
};

// Returns the lines `nm` prints for `elf`.
std::vector<std::string> symbol_lines(const elf::ElfFile& elf, const Options& options);

// Returns the bytes of the file at `path`, or throws `FileError` with
// the message of the OSError Python's `Path.read_bytes` raises.
using ReadFile = std::function<std::string(const std::string& path)>;

struct FileError {
  std::string message;
};

// Returns the bytes of the file at `path` on disk, as `ReadFile` does.
std::string read_path(const std::string& path);

// Runs nm.py's `main` on the command line `argv` (the program's name
// first), with the files `read_file` gives and argparse's usage and
// help wrapped for `columns` columns. Appends what it writes to stdout
// and stderr to `out` and `err`, and returns the exit status: argparse's
// 2 for a bad command line and 0 for the help, which Python raises as
// SystemExit, and otherwise 1 when a file couldn't be read and 0 when
// every one was. When `help` isn't null, it's set to whether `out` holds
// the help, which Python writes through its text stream, where it writes
// the listing's bytes (objdump.py's `write_lf`).
int main(const std::vector<std::string>& argv, const ReadFile& read_file, int columns,
         std::string& out, std::string& err, bool* help = nullptr);

}  // namespace c6502::binutils::nm
