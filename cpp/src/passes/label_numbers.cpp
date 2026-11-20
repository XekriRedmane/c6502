#include "passes/label_numbers.h"

#include <string>

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

asdl::Int next_label_number(const asm_ast::Program& prog,
                            std::initializer_list<std::string_view> prefixes) {
  asdl::Int best = -1;
  for (const auto& tl : prog.top_level) {
    const auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto& ins : fn->instructions) {
      const auto* label = asdl::as<ir::Label>(ins);
      if (label == nullptr) {
        continue;
      }
      const std::string_view name = label->name;
      for (const std::string_view prefix : prefixes) {
        if (name.size() <= prefix.size() || !name.starts_with(prefix)) {
          continue;
        }
        asdl::Int n = 0;
        bool digits = true;
        for (const char c : name.substr(prefix.size())) {
          if (c < '0' || c > '9') {
            digits = false;
            break;
          }
          n = n * 10 + (c - '0');
        }
        if (digits && n > best) {
          best = n;
        }
      }
    }
  }
  return best + 1;
}

}  // namespace c6502::passes
