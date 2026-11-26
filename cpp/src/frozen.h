// The frozen captures: the records of the Python passes' and object-file
// tools' calls over the corpus that were kept before step 8 removed the
// Python compiler, one .xz archive per function under
// cpp/tests/frozen/<kind>/, which
// `c6502cpp check-frozen` unpacks into capture directories and checks
// with `check-captures` and `check-binutils` (cpp/README.md, "The
// frozen captures").
#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace c6502::frozen {

// Writes the files of the archive `data`, the bytes of an xz stream of
//
//     c6502-frozen 1
//     file <path> <length>
//     <the file's bytes>
//     ...
//
// under `dir`, each at its path, and returns their number. Raises
// `std::runtime_error` for an archive that can't be read.
std::size_t unpack(std::string_view data, const std::filesystem::path& dir);

}  // namespace c6502::frozen
