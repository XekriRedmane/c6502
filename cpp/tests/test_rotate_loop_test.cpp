// The asm tests of tests/test_rotate_loop_test.py, ported. The Python
// tests that run compiled programs in the simulator stay in Python.
#include <algorithm>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "passes/rotate_loop_test.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_rotate_loop_test;
using c6502::passes::AsmContext;

namespace {

using Instrs = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kX = ir::Reg{.reg = ir::X{}};

ir::Type_operand imm(int v) { return ir::Imm{.value = v}; }
ir::Type_operand data(const char* name, int off = 0) {
  return ir::Data{.name = name, .offset = off};
}
ir::Type_instruction label(const char* name) { return ir::Label{.name = name}; }
ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst};
}

Instrs run(const Instrs& instrs) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = true, .instructions = instrs}}};
  prog = apply_rotate_loop_test(std::move(prog), AsmContext{});
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

// Returns `<prefix>; .top: <test>; BEQ .exit; <body>; JMP .top; .exit:
// RTS`.
Instrs loop(const Instrs& prefix, const Instrs& test, Instrs body = {}) {
  if (body.empty()) {
    body = {ir::Inc{.dst = kX}};
  }
  Instrs out = prefix;
  out.push_back(label(".top"));
  out.insert(out.end(), test.begin(), test.end());
  out.push_back(ir::Branch{.cond = ir::EQ{}, .target = ".exit"});
  out.insert(out.end(), body.begin(), body.end());
  out.push_back(ir::Jump{.target = ".top"});
  out.push_back(label(".exit"));
  out.push_back(ir::Return{});
  return out;
}

const Instrs kCpx6 = {ir::Compare{.left = kX, .right = imm(6)}};

bool contains(const Instrs& v, const ir::Type_instruction& i) {
  return std::find(v.begin(), v.end(), i) != v.end();
}

}  // namespace

TEST_CASE("rotate_loop_test enters a loop at its body when the test passes") {
  // X is 0 on entry and the test branches only for 6.
  CHECK(run(loop({mov(imm(0), kX)}, kCpx6)) ==
        Instrs{
            mov(imm(0), kX),
            label(".f@rot@0"),
            ir::Inc{.dst = kX},
            ir::Compare{.left = kX, .right = imm(6)},
            ir::Branch{.cond = ir::NE{}, .target = ".f@rot@0"},
            label(".exit"),
            ir::Return{},
        });
}

TEST_CASE("rotate_loop_test jumps to the test when the entry is unknown") {
  CHECK(run(loop({mov(data("n"), kX)}, kCpx6)) ==
        Instrs{
            mov(data("n"), kX),
            ir::Jump{.target = ".top"},
            label(".f@rot@0"),
            ir::Inc{.dst = kX},
            label(".top"),
            ir::Compare{.left = kX, .right = imm(6)},
            ir::Branch{.cond = ir::NE{}, .target = ".f@rot@0"},
            label(".exit"),
            ir::Return{},
        });
}

TEST_CASE("rotate_loop_test jumps to the test of a loop at the function's start") {
  // The function is entered at `.top`, with X unknown. Without the
  // entry JMP the first pass would skip the test (Zork's A_mod_3
  // printed words in the wrong places that way).
  const Instrs out = run(loop({}, kCpx6));
  CHECK(out[0] == ir::Type_instruction{ir::Jump{.target = ".top"}});
}

TEST_CASE("rotate_loop_test reads the entry value from memory") {
  // `LDA #$11; STA i; .top: LDA i; BEQ` passes on entry.
  const Instrs out = run(loop({mov(imm(0x11), kA), mov(kA, data("i"))},
                              {mov(data("i"), kA)}, {ir::Dec{.dst = data("i")}}));
  CHECK_FALSE(contains(out, ir::Jump{.target = ".top"}));
}

TEST_CASE("rotate_loop_test forgets the entry value at a call") {
  const Instrs out = run(loop({mov(imm(0), kX), ir::Call{.name = "g"}}, kCpx6));
  CHECK(out[2] == ir::Type_instruction{ir::Jump{.target = ".top"}});
}

TEST_CASE("rotate_loop_test keeps the jump when the first test exits") {
  // X is 6, so the first test exits.
  const Instrs out = run(loop({mov(imm(6), kX)}, kCpx6));
  CHECK(out[1] == ir::Type_instruction{ir::Jump{.target = ".top"}});
}

TEST_CASE("rotate_loop_test keeps the jump for a test that writes memory") {
  // `for (uint8_t i = 0x1F; i-- != 0;)` tests with `DEC i; LDA i; CMP
  // #$FF; BEQ`. The test passes on entry, but entered at the body, the
  // first pass would skip the DEC and index with 0x1F.
  const Instrs out = run(loop({mov(imm(0x1F), kA), mov(kA, data("i"))},
                              {ir::Dec{.dst = data("i")}, mov(data("i"), kA),
                               ir::Compare{.left = kA, .right = imm(0xFF)}},
                              {mov(data("i"), kX)}));
  CHECK(out[2] == ir::Type_instruction{ir::Jump{.target = ".top"}});
}

TEST_CASE("rotate_loop_test keeps the jump for a test that stores") {
  // `(n = i) != 4` stores `n` in the test.
  const Instrs out = run(loop({mov(imm(0), kA), mov(kA, data("i"))},
                              {mov(data("i"), kA), mov(kA, data("n")),
                               ir::Compare{.left = kA, .right = imm(4)}},
                              {ir::Inc{.dst = data("i")}}));
  CHECK(out[2] == ir::Type_instruction{ir::Jump{.target = ".top"}});
}

TEST_CASE("rotate_loop_test keeps the label a continue jumps to") {
  // A `continue` branches to `.top`, which moves with the test.
  const Instrs out = run(loop({mov(imm(0), kX)}, kCpx6,
                              {ir::Inc{.dst = kX},
                               ir::Branch{.cond = ir::MI{}, .target = ".top"},
                               ir::Inc{.dst = kX}}));
  const auto top = std::find(out.begin(), out.end(), label(".top"));
  REQUIRE(top != out.end());
  CHECK(*(top + 1) == ir::Type_instruction{ir::Compare{.left = kX, .right = imm(6)}});
}

TEST_CASE("rotate_loop_test leaves a test that exits elsewhere") {
  // The test branches to `.out`, which isn't after the JMP back.
  const Instrs instrs = {
      label(".top"),
      kCpx6[0],
      ir::Branch{.cond = ir::EQ{}, .target = ".out"},
      ir::Inc{.dst = kX},
      ir::Jump{.target = ".top"},
      label(".exit"),
      ir::Return{},
      label(".out"),
      ir::Return{},
  };
  CHECK(run(instrs) == instrs);
}

TEST_CASE("rotate_loop_test leaves a long body") {
  Instrs body;
  for (int k = 0; k < 40; ++k) {
    body.push_back(mov(kA, data("t", k)));
  }
  const Instrs instrs = loop({mov(imm(0), kX)}, kCpx6, body);
  CHECK(run(instrs) == instrs);
}
