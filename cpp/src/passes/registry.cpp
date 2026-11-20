#include "passes/registry.h"

#include <utility>

#include "passes/split_mem_to_mem.h"

namespace c6502::passes {

namespace {

std::string run_split_mem_to_mem(asm_ast::Program input, const json::Value&,
                                 const json::Value&) {
  return asdl::dump(apply_split_mem_to_mem(std::move(input)));
}

}  // namespace

const std::vector<RegisteredPass>& registered_passes() {
  static const std::vector<RegisteredPass> passes = {
      {"split_mem_to_mem", run_split_mem_to_mem},
  };
  return passes;
}

const RegisteredPass* find_pass(std::string_view name) {
  for (const auto& p : registered_passes()) {
    if (p.name == name) {
      return &p;
    }
  }
  return nullptr;
}

}  // namespace c6502::passes
