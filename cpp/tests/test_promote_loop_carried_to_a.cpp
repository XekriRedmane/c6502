// Tests of `promote_loop_carried_to_a` on the shape of snd_delay_up's
// outer loop, from the comment at the top of the pass. The Python pass
// has no tests of its own, and the corpus check covers it.
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/promote_loop_carried_to_a.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_promote_loop_carried_to_a;

namespace {

using Instrs = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kX = ir::Reg{.reg = ir::X{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};
const ir::Type_operand kPitch = ir::Data{.name = "__local_f__pitch", .offset = 0};

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst, .is_volatile = false};
}

ir::Type_operand data(const char* name, int offset = 0) {
  return ir::Data{.name = name, .offset = offset};
}

Instrs run(const Instrs& instrs) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = true, .instructions = instrs}}};
  prog = apply_promote_loop_carried_to_a(std::move(prog));
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

// The preamble that stages DPTR, then the loop with `body` after the
// `LDY pitch; INC pitch` pair.
Instrs shape(const Instrs& body) {
  Instrs out = {
      mov(kA, kPitch),
      mov(data("sfx_click_ptr"), kA),
      mov(kA, data("DPTR")),
      mov(data("sfx_click_ptr", 1), kA),
      mov(kA, data("DPTR", 1)),
      ir::Label{.name = ".loop"},
      mov(kPitch, kY),
      ir::Inc{.dst = kPitch},
  };
  out.insert(out.end(), body.begin(), body.end());
  out.push_back(ir::Branch{.cond = ir::NE{}, .target = ".loop"});
  out.push_back(ir::Return{});
  return out;
}

}  // namespace

TEST_CASE("promote_loop_carried_to_a keeps the pitch in A") {
  const Instrs body = {
      ir::Label{.name = ".inner"},
      ir::Dec{.dst = kY},
      ir::Branch{.cond = ir::NE{}, .target = ".inner"},
      ir::Compare{.left = kA, .right = ir::IndirectY{}},
      ir::Dec{.dst = kX},
  };
  Instrs expected = {
      mov(kA, kY),
      mov(data("sfx_click_ptr"), kA),
      mov(kA, data("DPTR")),
      mov(data("sfx_click_ptr", 1), kA),
      mov(kA, data("DPTR", 1)),
      mov(kY, kA),
      ir::Label{.name = ".loop"},
      mov(kA, kY),
      ir::ClearCarry{},
      ir::Add{.src = ir::Imm{.value = 1}, .dst = kA},
  };
  expected.insert(expected.end(), body.begin(), body.end());
  expected.push_back(ir::Branch{.cond = ir::NE{}, .target = ".loop"});
  expected.push_back(ir::Return{});
  CHECK(run(shape(body)) == expected);
}

TEST_CASE("promote_loop_carried_to_a leaves the slot alone") {
  SUBCASE("A written in the loop") {
    const Instrs instrs = shape({mov(data("g"), kA), ir::Dec{.dst = kX}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a compare whose flags a branch reads") {
    const Instrs instrs = shape({ir::Compare{.left = kA, .right = data("g")}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("another access of the slot") {
    const Instrs instrs = shape({mov(kPitch, kX), ir::Dec{.dst = kX}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("Y used in the preamble") {
    Instrs instrs = shape({ir::Dec{.dst = kX}});
    instrs.insert(instrs.begin() + 1, mov(ir::Imm{.value = 0}, kY));
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a packed BCD add of the slot after the loop") {
    Instrs instrs = shape({ir::Dec{.dst = kX}});
    instrs.insert(instrs.end() - 1, ir::DecimalArith{.subtract = false,
                                                     .left = {kPitch},
                                                     .right = {ir::Imm{.value = 1}},
                                                     .dst = {data("g")}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a call in the preamble, which writes Y") {
    Instrs instrs = shape({ir::Dec{.dst = kX}});
    instrs.insert(instrs.begin() + 1, ir::Call{.name = "h", .reg_args = {}, .preserves = {}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a soft-stack load in the preamble, which sets up Y") {
    Instrs instrs = shape({ir::Dec{.dst = kX}});
    instrs.insert(instrs.begin() + 1, mov(ir::Frame{.offset = 3}, kA));
    CHECK(run(instrs) == instrs);
  }
}

TEST_CASE("promote_loop_carried_to_a keeps the pitch across a call that preserves Y") {
  Instrs instrs = shape({ir::Dec{.dst = kX}});
  instrs.insert(instrs.begin() + 1, ir::Call{.name = "h", .reg_args = {}, .preserves = {"Y"}});
  const Instrs out = run(instrs);
  CHECK(out != instrs);
  CHECK(out[0] == mov(kA, kY));
}
