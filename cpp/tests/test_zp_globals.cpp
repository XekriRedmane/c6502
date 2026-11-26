// Cases of the zero-page objects' placement and `__zp_init`, the HARGS
// footprints of the runtime helpers, and the slot symbols of the
// private pools, written for the C++ functions.
#include <string>
#include <vector>

#include "doctest.h"
#include "errors.h"
#include "passes/front_end.h"
#include "passes/zp_globals.h"
#include "passes/zp_local_allocation.h"
#include "rtlib/helpers.h"

namespace {

namespace asm_ir = ::c6502::asm_ast;
namespace tac = ::c6502::tac_ast;
using c6502::asdl::Int;
using Names = std::vector<std::string>;

template <class M>
Names keys(const M& m) {
  Names out;
  for (const auto& [k, v] : m) {
    out.push_back(k);
  }
  return out;
}

asm_ir::Type_operand data(const std::string& name, Int offset) {
  return asm_ir::Data{.name = name, .offset = offset};
}

}  // namespace

TEST_CASE("zp_globals: a helper's footprint is its own extent or a callee's, whichever is larger") {
  // `hargs_need` of a program holding just the helper takes the larger
  // of the helper's own extent and the footprints of the helpers it
  // calls, which is what the footprint of a helper with callees that
  // call others in turn has to be.
  const c6502::passes::HelperFootprints& fp = c6502::passes::helper_footprints();
  for (const asm_ir::Function& fn : c6502::rtlib::all_helper_functions()) {
    CAPTURE(fn.name);
    const asm_ir::Program prog{.top_level = {fn}};
    CHECK(fp.at(fn.name) == c6502::passes::hargs_need(prog, fp));
  }
}

TEST_CASE("zp_globals: hargs_need counts the bytes of a DecimalArith") {
  const asm_ir::Function fn{
      .name = "f",
      .is_global = true,
      .params = {},
      .instructions = {asm_ir::DecimalArith{.subtract = false,
                                            .left = {data("HARGS", 0), data("HARGS", 1)},
                                            .right = {asm_ir::Imm{.value = 1}, data("x", 0)},
                                            .dst = {data("HARGS", 5), data("HARGS", 6)}},
                       asm_ir::Return{}}};
  CHECK(c6502::passes::hargs_need(asm_ir::Program{.top_level = {fn}}, {}) == 7);
  const asm_ir::Function left_only{
      .name = "g",
      .is_global = true,
      .params = {},
      .instructions = {asm_ir::DecimalArith{.subtract = true,
                                            .left = {data("HARGS", 9)},
                                            .right = {asm_ir::Imm{.value = 1}},
                                            .dst = {data("y", 0)}},
                       asm_ir::Return{}}};
  CHECK(c6502::passes::hargs_need(asm_ir::Program{.top_level = {left_only}}, {}) == 10);
}
