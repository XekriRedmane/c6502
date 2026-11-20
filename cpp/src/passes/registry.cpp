#include "passes/registry.h"

#include <utility>

#include "passes/dual_index_promotion.h"
#include "passes/split_mem_to_mem.h"
#include "passes/x_save_slot_load.h"

namespace c6502::passes {

namespace {

std::string run_split_mem_to_mem(asm_ast::Program input, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_split_mem_to_mem(std::move(input)));
}

std::string run_x_save_slot_load(asm_ast::Program input, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_x_save_slot_load(std::move(input)));
}

std::string run_dual_index_promotion(asm_ast::Program input, const AsmContext&,
                                     const json::Value&, const json::Value&) {
  return asdl::dump(apply_dual_index_promotion(std::move(input)));
}

}  // namespace

const std::vector<RegisteredPass>& registered_passes() {
  static const std::vector<RegisteredPass> passes = {
      {"split_mem_to_mem", run_split_mem_to_mem},
      {"x_save_slot_load", run_x_save_slot_load},
      {"dual_index_promotion", run_dual_index_promotion},
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
