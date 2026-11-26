// Tests of `section_items`, the section of each function and object
// for the link metadata's `item` lines.
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/sections.h"

namespace ir = c6502::asm_ast;
using c6502::passes::ItemMeta;
using c6502::passes::section_items;
using c6502::passes::SectionInfo;
using c6502::passes::SectionInfos;

namespace {

ir::Type_top_level function(const char* name, bool is_global = true) {
  return ir::Function{.name = name, .is_global = is_global, .params = {}, .instructions = {}};
}

ir::Type_top_level object(const char* name, std::vector<ir::Type_static_init> init) {
  return ir::StaticVariable{.name = name, .is_global = true, .init = std::move(init)};
}

}  // namespace

TEST_CASE("section_items puts a function in its named section or .text") {
  const ir::Program prog{.top_level = {function("boot"), function("f", false), function("g")}};
  const SectionInfos infos = {{"boot", SectionInfo{.section = ".text.boot"}},
                              {"f", SectionInfo{}}};
  CHECK(section_items(prog, infos) ==
        std::vector<ItemMeta>{
            {.name = "boot", .section = ".text.boot", .local = false, .zero = false},
            {.name = "f", .section = ".text", .local = true, .zero = false},
            {.name = "g", .section = ".text", .local = false, .zero = false},
        });
}

TEST_CASE("section_items puts an object in its named section, .rodata, .bss, or .data") {
  const ir::Program prog{.top_level = {
                             object("v", {ir::CharInit{.value = 1}}),
                             object("z", {ir::ZeroInit{.bytes = 4}}),
                             object("k", {ir::CharInit{.value = 2}}),
                             object("str@0", {ir::StringInit{.str = "hi", .bytes = 3}}),
                             object("m", {ir::ZeroInit{.bytes = 2}}),
                             object("t", {ir::CharInit{.value = 3}}),
                         }};
  const SectionInfos infos = {
      {"v", SectionInfo{.section = "vectors"}},
      {"z", SectionInfo{}},
      {"k", SectionInfo{.is_const = true}},
      {"m", SectionInfo{.section = "keep", .is_const = true}},
      {"t", SectionInfo{.merge = true}},
  };
  CHECK(section_items(prog, infos) ==
        std::vector<ItemMeta>{
            {.name = "v", .section = "vectors", .local = false, .zero = false},
            {.name = "z", .section = ".bss", .local = false, .zero = true},
            {.name = "k", .section = ".rodata", .local = false, .zero = false},
            {.name = "str@0", .section = ".rodata", .local = false, .zero = false},
            {.name = "m", .section = "keep", .local = false, .zero = true},
            {.name = "t", .section = ".data", .local = false, .zero = false, .merge = true},
        });
}
