// The tests of `dec_register_peephole` in
// tests/test_register_arguments.py (TestDecTransferBack), ported, and a
// test of the six-instruction pattern from the module's comment.
#include <algorithm>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/dec_register_peephole.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_dec_register_peephole;

namespace {

using Instrs = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst, .is_volatile = false};
}

Instrs run(const Instrs& instrs) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = true, .instructions = instrs}}};
  prog = apply_dec_register_peephole(std::move(prog));
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

// `.top: TYA; SEC; SBC #1; TAY; BNE .top`, then `tail`.
Instrs transfer_back(const Instrs& tail) {
  Instrs out = {
      ir::Label{.name = ".top"},
      mov(kY, kA),
      ir::SetCarry{},
      ir::Sub{.src = ir::Imm{.value = 1}, .dst = kA},
      mov(kA, kY),
      ir::Branch{.cond = ir::NE{}, .target = ".top"},
  };
  out.insert(out.end(), tail.begin(), tail.end());
  return out;
}

bool has_dey(const Instrs& instrs) {
  return std::find(instrs.begin(), instrs.end(),
                   ir::Type_instruction{ir::Dec{.dst = kY}}) != instrs.end();
}

}  // namespace

TEST_CASE("dec_register_peephole makes the transfer back a DEY") {
  const Instrs out = run(transfer_back({ir::Return{}}));
  CHECK(Instrs(out.begin() + 1, out.begin() + 3) ==
        Instrs{ir::Dec{.dst = kY}, ir::Branch{.cond = ir::NE{}, .target = ".top"}});
}

TEST_CASE("dec_register_peephole keeps the transfer back when A is read after") {
  CHECK_FALSE(has_dey(run(transfer_back({ir::Return{.save_a = true}}))));
}

TEST_CASE("dec_register_peephole keeps the transfer back when C is read after") {
  CHECK_FALSE(has_dey(run(transfer_back(
      {ir::Branch{.cond = ir::CC{}, .target = ".top"}, ir::Return{}}))));
}

TEST_CASE("dec_register_peephole folds the six-instruction pattern") {
  const ir::Type_operand tmp = ir::Data{.name = "__local_f__0", .offset = 0};
  const Instrs instrs = {
      ir::Label{.name = ".top"},
      mov(kY, kA),
      ir::SetCarry{},
      ir::Sub{.src = ir::Imm{.value = 1}, .dst = kA},
      mov(kA, tmp),
      mov(tmp, kY),
      ir::Branch{.cond = ir::NE{}, .target = ".top"},
      ir::Return{},
  };
  CHECK(run(instrs) == Instrs{ir::Label{.name = ".top"}, ir::Dec{.dst = kY},
                              ir::Branch{.cond = ir::NE{}, .target = ".top"},
                              ir::Return{}});
  // A read of the temporary after the pattern keeps it.
  Instrs read_after = instrs;
  read_after.insert(read_after.end() - 1, mov(tmp, ir::Data{.name = "out", .offset = 0}));
  CHECK(run(read_after) == read_after);
}

namespace {

const ir::Type_operand kTmp = ir::Data{.name = "__local_f__0", .offset = 0};
const ir::Type_operand kOut = ir::Data{.name = "out", .offset = 0};

// `.top: <head>; TYA; SEC; SBC #1; STA tmp; LDY tmp; BNE .top; <tail>`.
Instrs through_tmp(const Instrs& head, const Instrs& tail) {
  Instrs out = {ir::Label{.name = ".top"}};
  out.insert(out.end(), head.begin(), head.end());
  const Instrs body = {
      mov(kY, kA),
      ir::SetCarry{},
      ir::Sub{.src = ir::Imm{.value = 1}, .dst = kA},
      mov(kA, kTmp),
      mov(kTmp, kY),
      ir::Branch{.cond = ir::NE{}, .target = ".top"},
  };
  out.insert(out.end(), body.begin(), body.end());
  out.insert(out.end(), tail.begin(), tail.end());
  return out;
}

}  // namespace

TEST_CASE("dec_register_peephole keeps the six-instruction pattern when C is read after") {
  const Instrs instrs =
      through_tmp({}, {ir::Branch{.cond = ir::CC{}, .target = ".top"}, ir::Return{}});
  CHECK(run(instrs) == instrs);
}

TEST_CASE("dec_register_peephole keeps the six-instruction pattern when V is read after") {
  const Instrs instrs =
      through_tmp({}, {ir::Branch{.cond = ir::VS{}, .target = ".top"}, ir::Return{}});
  CHECK(run(instrs) == instrs);
}

TEST_CASE("dec_register_peephole keeps the temporary that the loop's top reads") {
  const Instrs instrs = through_tmp({mov(kTmp, kOut)}, {ir::Return{}});
  CHECK(run(instrs) == instrs);
}

TEST_CASE("dec_register_peephole folds when the temporary is written before it is read") {
  const Instrs instrs =
      through_tmp({}, {mov(ir::Imm{.value = 5}, kTmp), mov(kTmp, kOut), ir::Return{}});
  const Instrs out = run(instrs);
  CHECK(has_dey(out));
  CHECK(out.size() == instrs.size() - 4);
}
