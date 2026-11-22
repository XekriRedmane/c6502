// The types of binutils/ld.py that the layout returns: an input section
// placed at its address (`Placed`), and an output section of the
// executable with the input sections it holds (`OutputSection`).
// passes/link_layout.h fills them, and the port of the rest of ld.py
// (the definitions, the `Resolver`, `relax`, `relocate`, and
// `executable`) goes in this header beside them.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "binutils/elf.h"

namespace c6502::binutils::ld {

// Input section `section` of object number `unit`, running at `addr`
// (Python's `Placed`). `size` is its size in the output, which is 3
// bytes smaller than the section's when the linker drops a final `JMP`
// to the function after it. `section` is the object's own section, by
// pointer, since the linker keys the placed sections by identity
// (Python's `id(p.section)`).
struct Placed {
  int unit = 0;
  elf::SectionPtr section;
  std::int64_t addr = 0;
  std::int64_t size = 0;
};

// An output section of the executable (Python's `OutputSection`): its
// name, its run address `addr`, its load address `load_addr` (the same
// unless the script gives `AT>` or `AT(...)`), its size in bytes, the
// placed input sections in address order, whether it has no bytes in
// the file (`NOLOAD`), and whether it holds code. `executable()` sets
// `elf_section` to the section it writes for it, and the layout leaves
// it null.
struct OutputSection {
  std::string name;
  std::int64_t addr = 0;
  std::int64_t load_addr = 0;
  std::int64_t size = 0;
  std::vector<Placed> parts;
  bool noload = false;
  bool executable = false;
  elf::SectionPtr elf_section;
};

}  // namespace c6502::binutils::ld
