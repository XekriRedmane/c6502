// The runners of the asm SSA round trip's stage calls. Each pass but
// `order_parallel_copies` takes one asm function, which the harness
// records as the function's dump, and returns one.
#include "passes/optimization_asm/ssa_stage.h"

#include <string>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "passes/optimization_asm/backward_copy_propagation.h"
#include "passes/optimization_asm/byte_dce.h"
#include "passes/optimization_asm/copy_propagation.h"
#include "passes/optimization_asm/or_zero_absorb.h"
#include "passes/optimization_asm/parallel_copies.h"
#include "passes/optimization_asm/ssa_construction.h"
#include "passes/optimization_asm/ssa_destruction.h"
#include "tagged_json.h"

namespace c6502::passes::ssa_stage {

namespace {

namespace ir = ::c6502::asm_ast;
using namespace ::c6502::tagged;

using InstructionC = Asdl<ir::Type_instruction, kAsmAst>;
using InstructionsC = ListOf<InstructionC>;

ir::Function input_function(const capture::Call& call) {
  auto tl = asdl::load<ir::Type_top_level>(call.input);
  auto* fn = std::get_if<ir::Function>(&tl);
  if (fn == nullptr) {
    throw std::runtime_error("the input isn't an asm Function");
  }
  return std::move(*fn);
}

std::string dump_function(ir::Function fn) {
  return asdl::dump(ir::Type_top_level{std::move(fn)});
}

StaticNames statics_of(const json::Value& kwargs) {
  const json::Value* v = kwargs.get("statics");
  return v == nullptr ? StaticNames{} : NameSetC::decode(*v);
}

}  // namespace

std::string round_trip_instructions(const json::Value& v) {
  return round_trip<InstructionsC>(v);
}

StageOutput run_to_ssa(const capture::Call& call, const AsmContext&, const json::Value&,
                       const json::Value& kwargs) {
  return {dump_function(to_ssa(input_function(call), statics_of(kwargs))), ""};
}

StageOutput run_copy_propagate(const capture::Call& call, const AsmContext&, const json::Value&,
                               const json::Value& kwargs) {
  return {dump_function(copy_propagate(input_function(call), statics_of(kwargs))), ""};
}

StageOutput run_backward_copy_propagate(const capture::Call& call, const AsmContext&,
                                        const json::Value&, const json::Value& kwargs) {
  return {dump_function(backward_copy_propagate(input_function(call), statics_of(kwargs))), ""};
}

StageOutput run_byte_dce(const capture::Call& call, const AsmContext&, const json::Value&,
                         const json::Value& kwargs) {
  return {dump_function(byte_dce(input_function(call), statics_of(kwargs))), ""};
}

StageOutput run_absorb_zero_load(const capture::Call& call, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return {dump_function(absorb_zero_load(input_function(call))), ""};
}

StageOutput run_from_ssa(const capture::Call& call, const AsmContext&, const json::Value&,
                         const json::Value&) {
  return {dump_function(from_ssa(input_function(call))), ""};
}

StageOutput run_order_parallel_copies(const capture::Call& call, const AsmContext&,
                                      const json::Value&, const json::Value& kwargs) {
  std::vector<ir::Mov> movs;
  for (auto& instr : InstructionsC::decode(json::parse(call.input))) {
    auto* mov = std::get_if<ir::Mov>(&instr);
    if (mov == nullptr) {
      throw std::runtime_error("order_parallel_copies takes only Movs");
    }
    movs.push_back(std::move(*mov));
  }
  const json::Value* name = kwargs.get("fn_name");
  const json::Value* counter = kwargs.get("cycle_counter");
  if (name == nullptr || counter == nullptr) {
    throw std::runtime_error("order_parallel_copies takes fn_name and cycle_counter");
  }
  const auto counter_list = ListOf<IntC>::decode(*counter);
  if (counter_list.size() != 1) {
    throw std::runtime_error("cycle_counter isn't a list of one int");
  }
  int count = static_cast<int>(counter_list[0]);
  const int before = count;
  std::vector<ir::Type_instruction> ordered;
  for (auto& m : order_parallel_copies(std::move(movs), Str::decode(*name), count)) {
    ordered.emplace_back(std::move(m));
  }
  StageOutput out;
  InstructionsC::encode(out.output, ordered);
  if (count != before) {
    out.mutated = "{\"args\": {}, \"kwargs\": {\"cycle_counter\": [" + std::to_string(count) + "]}}";
  }
  return out;
}

}  // namespace c6502::passes::ssa_stage
