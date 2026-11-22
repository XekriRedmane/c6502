// `c6502cpp check-binutils`: the checks of the records that
// tools/capture_binutils.py writes of the Python assembler, linker,
// nm.py, and objdump.py (step 7, docs/cpp_port.md).
#pragma once

#include <string>
#include <vector>

namespace c6502::binutils {

// Checks the records under each capture directory of `args` (a
// directory with a subdirectory per kind and the `blobs` the records
// name), and with `--verbose` prints each difference in full. For
// every record, the arguments, the input, and the result read as JSON
// and written again must give the record's text. Prints a line per
// kind and returns the exit status, 1 when anything differs.
int check_binutils(const std::vector<std::string>& args);

}  // namespace c6502::binutils
