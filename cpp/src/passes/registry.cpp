#include "passes/registry.h"

#include <optional>
#include <utility>

#include "passes/asm_licm.h"
#include "passes/counter_to_index_reg.h"
#include "passes/dec_register_peephole.h"
#include "passes/dual_index_promotion.h"
#include "passes/element_in_place.h"
#include "passes/loop_constant_a.h"
#include "passes/loop_counter_to_x.h"
#include "passes/loop_invariant_a.h"
#include "passes/rotate_loop_test.h"
#include "passes/split_mem_to_mem.h"
#include "passes/x_save_slot_load.h"

namespace c6502::passes {

namespace {

// Returns the symbol map that the tagged JSON `v` holds, a
// `{"dict": [[name, address], ...]}` object, or nothing when `v` is
// null or JSON null (Python's `None`).
std::optional<SymbolAddrs> symbol_addrs_arg(const json::Value* v) {
  if (v == nullptr || v->kind == json::Value::Kind::kNull) {
    return std::nullopt;
  }
  SymbolAddrs m;
  for (const auto& pair : v->get("dict")->items) {
    m[pair.items[0].string] = pair.items[1].integer;
  }
  return m;
}

std::string run_licm(asm_ast::Program input, const AsmContext& ctx,
                     const json::Value& args, const json::Value&) {
  const auto zp_cells = symbol_addrs_arg(args.items.empty() ? nullptr : &args.items[0]);
  return asdl::dump(apply_licm(std::move(input), ctx, zp_cells ? &*zp_cells : nullptr));
}

std::string run_split_mem_to_mem(asm_ast::Program input, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_split_mem_to_mem(std::move(input)));
}

std::string run_element_in_place(asm_ast::Program input, const AsmContext& ctx,
                                 const json::Value&, const json::Value& kwargs) {
  const auto slots = symbol_addrs_arg(kwargs.get("zp_slot_symbols"));
  return asdl::dump(
      apply_element_in_place(std::move(input), ctx, slots ? &*slots : nullptr));
}

std::string run_loop_constant_a(asm_ast::Program input, const AsmContext&,
                                const json::Value&, const json::Value&) {
  return asdl::dump(apply_loop_constant_a(std::move(input)));
}

std::string run_dec_register_peephole(asm_ast::Program input, const AsmContext&,
                                      const json::Value&, const json::Value&) {
  return asdl::dump(apply_dec_register_peephole(std::move(input)));
}

std::string run_loop_counter_to_x(asm_ast::Program input, const AsmContext&,
                                  const json::Value& args, const json::Value& kwargs) {
  const auto addrs = symbol_addrs_arg(
      args.items.empty() ? kwargs.get("zp_symbol_addrs") : &args.items[0]);
  return asdl::dump(apply_loop_counter_to_x(std::move(input), addrs ? &*addrs : nullptr));
}

std::string run_counter_to_index_reg(asm_ast::Program input, const AsmContext&,
                                     const json::Value& args, const json::Value& kwargs) {
  const auto addrs = symbol_addrs_arg(
      args.items.empty() ? kwargs.get("zp_symbol_addrs") : &args.items[0]);
  return asdl::dump(
      apply_counter_to_index_reg(std::move(input), addrs ? &*addrs : nullptr));
}

std::string run_x_save_slot_load(asm_ast::Program input, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_x_save_slot_load(std::move(input)));
}

std::string run_dual_index_promotion(asm_ast::Program input, const AsmContext&,
                                     const json::Value&, const json::Value&) {
  return asdl::dump(apply_dual_index_promotion(std::move(input)));
}

std::string run_loop_invariant_a(asm_ast::Program input, const AsmContext& ctx,
                                 const json::Value&, const json::Value& kwargs) {
  const auto slots = symbol_addrs_arg(kwargs.get("zp_slot_symbols"));
  return asdl::dump(
      apply_loop_invariant_a(std::move(input), ctx, slots ? &*slots : nullptr));
}

std::string run_rotate_loop_test(asm_ast::Program input, const AsmContext& ctx,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_rotate_loop_test(std::move(input), ctx));
}

}  // namespace

const std::vector<RegisteredPass>& registered_passes() {
  static const std::vector<RegisteredPass> passes = {
      {"licm", run_licm},
      {"split_mem_to_mem", run_split_mem_to_mem},
      {"element_in_place", run_element_in_place},
      {"loop_constant_a", run_loop_constant_a},
      {"dec_register_peephole", run_dec_register_peephole},
      {"loop_counter_to_x", run_loop_counter_to_x},
      {"counter_to_index_reg", run_counter_to_index_reg},
      {"x_save_slot_load", run_x_save_slot_load},
      {"dual_index_promotion", run_dual_index_promotion},
      {"loop_invariant_a", run_loop_invariant_a},
      {"rotate_loop_test", run_rotate_loop_test},
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
