// Random programs from YARPGen (https://github.com/intel/yarpgen), run
// on every c6502 path, ported from fuzz/yarpgen.py.
//
// YARPGen generates loop nests over arrays and scalars of every integer
// type, with values it has checked for undefined behavior under its
// model of C. fuzz/build_yarpgen.sh builds it with that model's `int`
// 16 bits wide, as c6502's is, so a program it accepts is well-defined
// for c6502 too. Each seed's test is three files, which `merge` turns
// into one program:
//   * `init.h` (declarations), `func.c` (`test()`), and `driver.c`
//     (definitions, `init()`, `checksum()`, and `main`), in that order;
//   * `main` returns the checksum, folded to 15 bits, instead of
//     printing it, since c6502 has no `printf`, and the TAC interpreter's
//     result is the reference;
//   * `size_t` is `unsigned int`.
//
// Inputs are globals (`--inp-as-args=none`), since a `test()` taking
// them as parameters needs more than the 253 bytes a soft-stack frame
// can hold. Arrays have one dimension, loops at most 6 iterations, and a
// test at most 25 statements, which keeps a program to a few seconds in
// the simulator. A large `test()` still needs more than 253 bytes of
// temporaries without -O, which the runner counts as rejected.
#pragma once

#include <filesystem>
#include <string>

#include "asdl/runtime.h"
#include "fuzz/program.h"

namespace c6502::fuzz::yarpgen {

// Returns the path of the YARPGen that fuzz/build_yarpgen.sh builds in
// the checkout at `root`.
std::filesystem::path binary(const std::filesystem::path& root);

// Returns one c6502 program from a YARPGen test's three files. Throws
// `std::runtime_error` when `driver` doesn't print the checksum.
std::string merge(std::string init, std::string func, std::string driver);

// Returns the program for `seed`, from the YARPGen at `exe`. YARPGen
// reserves seed 0 for a random one, so it gets `seed + 1`. Throws
// `std::runtime_error` when YARPGen fails.
GeneratedProgram make_program(asdl::Int seed, const std::filesystem::path& exe);

}  // namespace c6502::fuzz::yarpgen
