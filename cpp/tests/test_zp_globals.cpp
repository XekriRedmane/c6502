// Cases of the zero-page objects' placement and `__zp_init`, the HARGS
// footprints of the runtime helpers, and the slot symbols of the
// private pools, written for the C++ functions.
#include <string>
#include <utility>
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

TEST_CASE("zp_globals: __zp_init loads each initial byte once, zero first") {
  c6502::passes::ZpGlobals zp;
  zp.addrs.insert_or_assign("a", 0x30);
  zp.addrs.insert_or_assign("b", 0x33);
  zp.addrs.insert_or_assign("c", 0x35);
  zp.owned.insert("a");
  zp.owned.insert("b");
  asm_ir::Program prog{
      .top_level = {asm_ir::StaticVariable{.name = "a",
                                           .is_global = true,
                                           .init = {asm_ir::CharInit{.value = 5},
                                                    asm_ir::CharInit{.value = 0},
                                                    asm_ir::CharInit{.value = 5}}},
                    asm_ir::StaticVariable{.name = "b",
                                           .is_global = false,
                                           .init = {asm_ir::CharInit{.value = 7},
                                                    asm_ir::CharInit{.value = 0}}},
                    // `c` is another unit's, so this one stores none of
                    // its bytes.
                    asm_ir::StaticVariable{.name = "c",
                                           .is_global = true,
                                           .init = {asm_ir::CharInit{.value = 9}}},
                    asm_ir::StaticVariable{.name = "d",
                                           .is_global = true,
                                           .init = {asm_ir::CharInit{.value = 1}}}}};
  const asm_ir::Program out = c6502::passes::place_zp_globals(prog, zp, {});
  REQUIRE(out.top_level.size() == 2);
  CHECK(std::get<asm_ir::StaticVariable>(out.top_level[0]).name == "d");
  const auto& init = std::get<asm_ir::Function>(out.top_level[1]);
  CHECK(init.name == c6502::passes::kZpInitFunction);
  const asm_ir::Type_operand a = asm_ir::Reg{.reg = asm_ir::A{}};
  const auto load = [&](Int v) -> asm_ir::Type_instruction {
    return asm_ir::Mov{.src = asm_ir::Imm{.value = v}, .dst = a, .is_volatile = false};
  };
  const auto store = [&](const std::string& name, Int k) -> asm_ir::Type_instruction {
    return asm_ir::Mov{.src = a, .dst = data(name, k), .is_volatile = false};
  };
  const std::vector<asm_ir::Type_instruction> expected = {
      load(0),     store("a", 1), store("b", 1), load(5), store("a", 0),
      store("a", 2), load(7),     store("b", 0), asm_ir::Return{}};
  CHECK(init.instructions == expected);
}

TEST_CASE("zp_globals: promotion takes the most accesses per byte first, ties by name") {
  c6502::passes::FrontEndOptions options;
  options.preprocess = false;
  options.optimize = true;
  std::string err;
  const auto r = c6502::passes::front_end(
      "unsigned int a = 1;\n"
      "unsigned char b = 1;\n"
      "unsigned char c = 1;\n"
      "unsigned char d = 1;\n"
      "unsigned long e = 1;\n",
      options, err);
  c6502::OrderedMap<std::string, Int> weights;
  // 3 a byte for `a` and `b`, 8 for `c`, 2 for `d` (too few), and 7/2
  // for `e`, which comes between.
  weights.insert_or_assign("a", 6);
  weights.insert_or_assign("b", 3);
  weights.insert_or_assign("c", 8);
  weights.insert_or_assign("d", 2);
  weights.insert_or_assign("e", 14);
  const c6502::passes::ZpGlobals zp =
      c6502::passes::allocate_zp_globals(r.symbols, &r.types, {}, &weights);
  CHECK(keys(zp.promoted) == Names{"c", "e", "a", "b"});
  CHECK(keys(zp.addrs) == Names{"c", "e", "a", "b"});
}

TEST_CASE("zp_globals: access_weights doesn't count a Phi's predecessor label") {
  // Built a step at a time, since MSVC 14.36 stops with an internal
  // compiler error on the same value as one nested initializer.
  tac::Phi phi;
  phi.dst = tac::Var{.name = "x"};
  phi.args.emplace_back(tac::PhiArg{.pred_label = "g", .source = tac::Var{.name = "y"}});
  tac::Function fn;
  fn.name = "f";
  fn.is_global = true;
  fn.instructions.emplace_back(std::move(phi));
  fn.instructions.emplace_back(tac::Ret{.val = tac::Var{.name = "x"}});
  tac::Program prog;
  prog.top_level.emplace_back(std::move(fn));
  CHECK(keys(c6502::passes::access_weights(prog)) == Names{"x", "y"});
}
