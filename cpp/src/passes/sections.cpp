#include "passes/sections.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

std::vector<ItemMeta> section_items(const asm_ast::Program& prog, const SectionInfos& symbols) {
  std::vector<ItemMeta> out;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = asdl::as<ir::Function>(tl)) {
      const auto it = symbols.find(fn->name);
      const SectionInfo* sym = it == symbols.end() ? nullptr : &it->second;
      const bool named = sym != nullptr && sym->section && !sym->section->empty();
      out.push_back(ItemMeta{.name = fn->name,
                             .section = named ? *sym->section : ".text",
                             .local = !fn->is_global,
                             .zero = false});
      continue;
    }
    const auto& sv = std::get<ir::StaticVariable>(tl);
    const auto it = symbols.find(sv.name);
    const SectionInfo* sym = it == symbols.end() ? nullptr : &it->second;
    bool zero = true;
    for (const auto& init : sv.init) {
      if (!asdl::isa<ir::ZeroInit>(init)) {
        zero = false;
        break;
      }
    }
    std::string section;
    if (sym != nullptr && sym->section) {
      section = *sym->section;
    } else if (sv.name.starts_with("str@") || (sym != nullptr && sym->is_const)) {
      section = ".rodata";
    } else if (zero) {
      section = ".bss";
    } else {
      section = ".data";
    }
    out.push_back(ItemMeta{.name = sv.name,
                           .section = std::move(section),
                           .local = !sv.is_global,
                           .zero = zero,
                           .merge = sym != nullptr && sym->merge});
  }
  return out;
}

}  // namespace c6502::passes
