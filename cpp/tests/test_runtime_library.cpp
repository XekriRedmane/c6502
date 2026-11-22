// Tests of the runtime library's selection of helpers and its text
// (src/passes/runtime_library.h), from tests/test_runtime_library.py.
// `c6502cpp check-binutils` compares `needed` and `library_text` with
// Python on every link of the corpus and the suite (the
// `runtime_library` records of tools/capture_binutils.py).
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "binutils/elf.h"
#include "doctest.h"
#include "passes/runtime_library.h"

namespace {

using namespace c6502;
namespace elf = binutils::elf;

// Returns an object whose symbols are `undefined`, each undefined, and
// `defined`, each defined in `.text`.
elf::ElfFile object(const std::vector<std::string>& undefined,
                    const std::vector<std::string>& defined = {}) {
  elf::ElfFile f;
  auto text = std::make_shared<elf::Section>();
  text->name = ".text";
  text->data = std::string("\x60", 1);
  f.sections.push_back(text);
  for (const auto& name : undefined) {
    auto s = std::make_shared<elf::Symbol>();
    s->name = name;
    s->bind = elf::STB_GLOBAL;
    f.symbols.push_back(s);
  }
  for (const auto& name : defined) {
    auto s = std::make_shared<elf::Symbol>();
    s->name = name;
    s->bind = elf::STB_GLOBAL;
    s->type = elf::STT_FUNC;
    s->section = text;
    f.symbols.push_back(s);
  }
  return f;
}

using Objects = std::vector<std::pair<std::string, elf::ElfFile>>;

TEST_CASE("closure adds the helpers a helper calls, in the library's order") {
  CHECK(passes::closure({"sdivmod32"}) == std::vector<std::string>{"udivmod32", "sdivmod32"});
  CHECK(passes::closure({"mul8", "fadd"}) == std::vector<std::string>{"mul8"});
  CHECK(passes::helper_names().size() == 19);
  CHECK(passes::helper_names().contains("bin16_to_bcd"));
}

TEST_CASE("needed takes the helpers the objects name and nothing defines") {
  const Objects objects{{"a.o", object({"lsr16", "sdivmod16", "putchar"})},
                        {"b.o", object({"mul8"}, {"main"})}};
  CHECK(passes::needed(objects, {"main"}) ==
        std::vector<std::string>{"mul8", "udivmod16", "sdivmod16", "lsr16"});
}

TEST_CASE("a definition or a --defsym of a callee leaves it out") {
  // sdivmod16 comes from the library, and its call of udivmod16 goes to
  // the address `defined` covers.
  const Objects objects{{"a.o", object({"sdivmod16"})}};
  CHECK(passes::needed(objects, {"udivmod16"}) == std::vector<std::string>{"sdivmod16"});
  CHECK(passes::needed(objects, {"sdivmod16"}).empty());
}

TEST_CASE("a defined helper name in an object isn't needed") {
  const Objects objects{{"a.o", object({}, {"mul16"})}};
  CHECK(passes::needed(objects, {}).empty());
}

TEST_CASE("the text has the metadata block and each helper") {
  const std::string text = passes::library_text({"udivmod16", "sdivmod16"});
  CHECK(text.find("; @zp-link-meta-begin") != std::string::npos);
  // The HARGS size and need are sdivmod16's footprint, 11 bytes.
  CHECK(text.find("; item udivmod16 section=.text local=false zero=false\n"
                  "; item sdivmod16 section=.text local=false zero=false\n"
                  "; runtime hargs_size=11 hargs_need=11\n") != std::string::npos);
  CHECK(text.find("\nudivmod16:\n") != std::string::npos);
  CHECK(text.find("\nsdivmod16:\n") > text.find("\nudivmod16:\n"));
  CHECK(text.find("\nmul8:\n") == std::string::npos);
}

}  // namespace
