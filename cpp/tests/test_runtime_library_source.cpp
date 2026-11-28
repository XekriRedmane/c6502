// The tests of tests/test_runtime_library.py that link a program with
// no `--defsym` and read the executable. They check which helpers a
// link takes from the runtime library, a definition that takes a
// helper's place, the error for a helper with no 6502 code, and a
// script that places no output section for the helpers.
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "binutils/elf.h"
#include "compile_case.h"
#include "doctest.h"
#include "rtlib/helpers.h"

namespace {

namespace suite = c6502::suite;
namespace elf = c6502::binutils::elf;

// The name the runtime library's object goes by in a link.
constexpr const char* kLibraryName = "runtime.o";

// The names of the helpers the runtime library has.
std::set<std::string> helper_names() {
  std::set<std::string> out;
  for (const auto& fn : c6502::rtlib::all_helper_functions()) out.insert(fn.name);
  return out;
}

// Links `paths` into `out` with no `--defsym` but those in `extra`.
c6502::compile_command::Outcome link(const std::vector<std::string>& paths,
                                     const std::string& out,
                                     const std::vector<std::string>& extra = {}) {
  std::vector<std::string> argv{"c6502"};
  argv.insert(argv.end(), paths.begin(), paths.end());
  argv.push_back("-o");
  argv.push_back(out);
  argv.insert(argv.end(), extra.begin(), extra.end());
  return suite::compile_main(argv);
}

// What a link of the program `source` gives, linked by `script` when
// one is given. That is the executable when the link succeeds, and
// stderr.
struct Built {
  std::optional<elf::ElfFile> exe;
  std::string err;
};

Built build(const std::string& source, const std::optional<std::string>& script = std::nullopt) {
  const suite::TempDir tmp;
  const std::string obj = tmp.file("a.o");
  REQUIRE(suite::compile_object(source, obj) == 0);
  std::vector<std::string> extra;
  if (script) {
    const std::string ld = tmp.file("prog.ld");
    suite::write_file(ld, *script);
    extra = {"-T", ld};
  }
  const std::string out = tmp.file("prog");
  const auto o = link({obj}, out, extra);
  Built b;
  b.err = o.err();
  if (o.status == 0) b.exe = elf::read(suite::read_file(out), out);
  return b;
}

// The helpers the executable holds in its own sections, by address.
std::map<std::string, std::int64_t> own_helpers(const elf::ElfFile& exe) {
  const std::set<std::string> names = helper_names();
  std::map<std::string, std::int64_t> out;
  for (const auto& s : exe.symbols) {
    if (names.count(s->name) && s->defined() && !s->is_absolute()) out[s->name] = s->value;
  }
  return out;
}

std::vector<std::string> file_symbols(const elf::ElfFile& exe) {
  std::vector<std::string> out;
  for (const auto& s : exe.symbols) {
    if (s->type == elf::STT_FILE) out.push_back(s->name);
  }
  return out;
}

bool has(const std::vector<std::string>& names, const std::string& name) {
  for (const auto& n : names) {
    if (n == name) return true;
  }
  return false;
}

std::set<std::string> keys(const std::map<std::string, std::int64_t>& m) {
  std::set<std::string> out;
  for (const auto& [k, v] : m) out.insert(k);
  return out;
}

const std::string kDivide = R"(
int a = -1234;
int b = 7;
int main(void) { return a / b; }
)";

}  // namespace

TEST_CASE("runtime_library: a program that calls no helper links none") {
  const Built b = build("int a = 3; int main(void) { return a + 1; }");
  REQUIRE_MESSAGE(b.exe.has_value(), b.err);
  CHECK(own_helpers(*b.exe).empty());
  CHECK_FALSE(has(file_symbols(*b.exe), kLibraryName));
}

TEST_CASE("runtime_library: a unit's definition of a helper's name takes precedence") {
  // A unit that defines a helper's name gives the program that
  // function, as a C toolchain prefers an object's definition to
  // libgcc's, and the library adds nothing.
  const suite::TempDir tmp;
  const std::string a = tmp.file("a.o");
  const std::string b = tmp.file("b.o");
  std::string quotient = kDivide;
  quotient.replace(quotient.find("int main"), 8, "int quotient");
  REQUIRE(suite::compile_object(quotient, a) == 0);
  REQUIRE(suite::compile_object("void sdivmod16(void) {}\n"
                                "void udivmod16(void) {}\n"
                                "int quotient(void);\n"
                                "int main(void) { sdivmod16(); udivmod16();"
                                " return quotient(); }\n",
                                b) == 0);
  const std::string out = tmp.file("prog");
  const auto o = link({a, b}, out);
  REQUIRE_MESSAGE(o.status == 0, o.err());
  const elf::ElfFile exe = elf::read(suite::read_file(out), out);
  CHECK_FALSE(has(file_symbols(exe), kLibraryName));
  CHECK(keys(own_helpers(exe)) == std::set<std::string>{"sdivmod16", "udivmod16"});
}

TEST_CASE("runtime_library: a helper with no 6502 code is a link error that names it") {
  const suite::TempDir tmp;
  const std::string obj = tmp.file("a.o");
  REQUIRE(suite::compile_object("float a = 1.5f; float b = 2.0f; "
                                "int main(void) { return (int)(a + b); }",
                                obj) == 0);
  const auto o = link({obj}, tmp.file("prog"));
  CHECK(o.status == 1);
  CHECK(suite::contains(o.err(), "undefined symbol `fadd`"));
  CHECK(suite::contains(o.err(), "no 6502 code in the runtime library"));
  CHECK_FALSE(suite::contains(o.err(), "isn't in the repository"));
}

TEST_CASE("runtime_library: a script that places no section for the helpers is a link error") {
  const std::string script = R"(
MEMORY
{
    ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x1000
    lib (rx) : ORIGIN = 0x4000, LENGTH = 0x0400
}
SECTIONS
{
    .text : { a.o(.text .text.*) } > ram
    .rodata : { *(.rodata .rodata.*) } > ram
    .data : { *(.data .data.*) } > ram
    .bss : { *(.bss .bss.*) } > ram

}
)";
  const Built b = build(kDivide, script);
  CHECK_FALSE(b.exe.has_value());
  CHECK(suite::contains(b.err, "`udivmod16` (section .text, runtime.o)"));
}
