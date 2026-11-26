#pragma once

#include <string>
#include <vector>

namespace c6502::binutils {

// Checks the records under each capture directory of `args` (a
// directory with a subdirectory per kind and the `blobs` the records
// name), and with `--verbose` prints each difference in full. For
// every record, the arguments, the input, and the result read as JSON
// and written again must give the record's text. Every object and
// executable a record holds (an assembly's object, a link's objects
// and executable, a disassembled file) read and written again by the C++ ELF
// code must give its bytes, and every `elf_read` record's file read in
// C++ must give the record's rewrite or its error. Each ported tool
// runs on its records and must give the recorded result. Prints a line
// per kind and returns the exit status, 1 when anything differs.
int check_binutils(const std::vector<std::string>& args);

}  // namespace c6502::binutils
