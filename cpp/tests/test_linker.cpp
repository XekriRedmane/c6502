#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "binutils/assembler.h"
#include "binutils/elf.h"
#include "binutils/ld.h"
#include "doctest.h"
#include "errors.h"
#include "passes/link_script.h"
#include "passes/linker.h"

namespace {

using c6502::CompileError;
namespace elf = c6502::binutils::elf;
namespace ld = c6502::binutils::ld;
namespace linker = c6502::passes::linker;
namespace link_script = c6502::passes::link_script;

std::string error_of(const std::function<void()>& f) {
  try {
    f();
  } catch (const CompileError& e) {
    return e.python_type() + ": " + e.what();
  }
  return "no error";
}

elf::ElfFile main_object() { return c6502::binutils::assembler::assemble("main:\n  RTS\n", "m.c"); }

// Returns an object that holds only the link metadata `lines`, as the
// Python cases build it.
elf::ElfFile meta_object(const std::vector<std::string>& lines) {
  auto sec = std::make_shared<elf::Section>();
  sec->name = std::string(elf::LINK_SECTION);
  sec->flags = 0;
  for (const std::string& line : lines) sec->data += line + "\n";
  elf::ElfFile obj;
  obj.sections.push_back(sec);
  return obj;
}

std::string link_error(ld::Objects objects,
                       const std::optional<link_script::Script>& script = std::nullopt) {
  return error_of([&] { linker::link_objects(std::move(objects), script); });
}

}  // namespace

TEST_CASE("link_objects gives Python's message at each of its raise sites") {
  CHECK(link_error({}) == "LinkError: linking needs at least one object");
  const elf::ElfFile exe = linker::link_objects({{"m.o", main_object()}}, std::nullopt);
  CHECK(link_error({{"prog", exe}}) == "LinkError: prog isn't a relocatable object");
  CHECK(link_error({{"bad.o", meta_object({"; @zp-link-meta-begin", "; bogus record",
                                           "; @zp-link-meta-end"})}}) ==
        "LinkError: bad.o: unknown record kind 'bogus': 'bogus record'");
  // An initial byte the assembler can't read, which only a metadata
  // block written by hand holds, stops the linker's own `__zp_init`.
  const auto own = [](const std::string& init) {
    return link_error({{"m.o", main_object()},
                       {"z.o", meta_object({"; @zp-link-meta-begin",
                                            "; zp g size=1 at=auto def=true init=" + init,
                                            "; @zp-link-meta-end"})}});
  };
  CHECK(own("(") ==
        "LinkError: the linker's own code: the linker:4: expected more, found the end of the "
        "line");
  CHECK(own("$1FF") == "LinkError: the linker's own code: the linker:4: 511 doesn't fit in 1 "
                       "byte(s)");
  CHECK(own("1)") == "LinkError: the linker's own code: the linker:4: unexpected ')' in '1)'");
  // The assembler takes `@@` as a name, which no object defines.
  CHECK(own("@@") == "LinkError: the linker: undefined symbol `@@`");
  const link_script::Script script = link_script::parse_script(
      "MEMORY { ROM : ORIGIN = 0x1000, LENGTH = 0 }\n"
      "SECTIONS { .text : { *(.text) } > ROM }\n");
  CHECK(link_error({{"m.o", main_object()}}, script) ==
        "LinkError: no output section in the script takes `main` (section .data, m.o)");
}

TEST_CASE("link_files gives Python's message for an object it can't read") {
  const auto files = [](const std::string& path) {
    return error_of([&] { linker::link_files({path}, "d3-out.bin", std::nullopt); });
  };
  CHECK(files("d3-missing.o") == "LinkError: d3-missing.o: No such file or directory");
  const std::filesystem::path bad = std::filesystem::temp_directory_path() / "d3-notelf.o";
  std::ofstream(bad, std::ios::binary) << "hello";
  const std::string path = bad.string();
  CHECK(files(path) == "LinkError: " + path + ": not an ELF file");
  std::filesystem::remove(bad);
}

TEST_CASE("link_objects rejects an executable as an input") {
  ld::Names defsyms;
  defsyms.insert_or_assign("X", 5);
  const elf::ElfFile exe = linker::link_objects({{"m.o", main_object()}}, std::nullopt, 0x0800,
                                                defsyms);
  CHECK(link_error({{"prog", elf::read(elf::write(exe), "prog")}}) ==
        "LinkError: prog isn't a relocatable object");
}
