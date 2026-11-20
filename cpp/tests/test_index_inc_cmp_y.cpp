// The tests of tests/test_index_inc_cmp_y.py, ported, except the one
// that runs a program in the simulator.
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "passes/index_inc_cmp_y.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_index_inc_cmp_y;
using c6502::passes::AsmContext;

namespace {

using Instrs = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};
const ir::Type_instruction kRts = ir::Return{};

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst, .is_volatile = false};
}

ir::Type_operand data(const char* name) { return ir::Data{.name = name, .offset = 0}; }

ir::Type_operand indexed_x(const char* name) {
  return ir::IndexedData{.name = name, .offset = 0, .index = ir::X{}};
}

const ir::Type_operand kTmp = data("__local_f__0");

Instrs run(const Instrs& instrs) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = true, .instructions = instrs}}};
  prog = apply_index_inc_cmp_y(std::move(prog), AsmContext{});
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

// `LDA tbl,X; STA tmp; INC tmp; LDA tmp; CMP other`.
Instrs pattern(const ir::Type_operand& tmp, const ir::Type_operand& other,
               bool dec = false) {
  ir::Type_instruction step = ir::Inc{.dst = tmp};
  if (dec) {
    step = ir::Dec{.dst = tmp};
  }
  return {mov(indexed_x("tbl"), kA), mov(kA, tmp), step, mov(tmp, kA),
          ir::Compare{.left = kA, .right = other}};
}

Instrs concat(Instrs a, const Instrs& b) {
  a.insert(a.end(), b.begin(), b.end());
  return a;
}

}  // namespace

TEST_CASE("index_inc_cmp_y folds the step done in A") {
  for (const bool dec : {false, true}) {
    CAPTURE(dec);
    Instrs instrs = {mov(indexed_x("floor_ceil"), kA)};
    if (dec) {
      instrs.push_back(ir::SetCarry{});
      instrs.push_back(ir::Sub{.src = ir::Imm{.value = 1}, .dst = kA});
    } else {
      instrs.push_back(ir::ClearCarry{});
      instrs.push_back(ir::Add{.src = ir::Imm{.value = 1}, .dst = kA});
    }
    instrs.push_back(ir::Compare{.left = kA, .right = data("beam_y")});
    instrs.push_back(kRts);
    const Instrs out = run(instrs);
    ir::Type_instruction step = ir::Inc{.dst = kY};
    if (dec) {
      step = ir::Dec{.dst = kY};
    }
    CHECK(Instrs(out.begin(), out.begin() + 3) ==
          Instrs{mov(indexed_x("floor_ceil"), kY), step,
                 ir::Compare{.left = kY, .right = data("beam_y")}});
  }
}

TEST_CASE("index_inc_cmp_y leaves the step in A when V is read after") {
  // The ADC's V is read after, and CPY doesn't set V.
  const Instrs instrs = {
      mov(indexed_x("floor_ceil"), kA),
      ir::ClearCarry{},
      ir::Add{.src = ir::Imm{.value = 1}, .dst = kA},
      ir::Compare{.left = kA, .right = data("beam_y")},
      ir::Branch{.cond = ir::VS{}, .target = ".l"},
      ir::Label{.name = ".l"},
      kRts,
  };
  CHECK(run(instrs) == instrs);
}

TEST_CASE("index_inc_cmp_y folds the five-atom shape") {
  const Instrs instrs =
      concat(pattern(kTmp, data("beam_y")),
             {ir::Branch{.cond = ir::EQ{}, .target = ".skip"}, kRts,
              ir::Label{.name = ".skip"}, kRts});
  const Instrs out = run(instrs);
  CHECK(Instrs(out.begin(), out.begin() + 3) ==
        Instrs{mov(indexed_x("tbl"), kY), ir::Inc{.dst = kY},
               ir::Compare{.left = kY, .right = data("beam_y")}});
  const Instrs dec = run(concat(pattern(kTmp, data("beam_y"), true), {kRts}));
  CHECK(dec[1] == ir::Type_instruction{ir::Dec{.dst = kY}});
}

TEST_CASE("index_inc_cmp_y skips") {
  SUBCASE("Y live") {
    // Y holds something the code after the pattern reads.
    const Instrs instrs = concat(
        concat({mov(ir::Imm{.value = 5}, kY)}, pattern(kTmp, data("beam_y"))),
        {mov(ir::IndexedData{.name = "tbl2", .offset = 0, .index = ir::Y{}}, kA), kRts});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("A live") {
    const Instrs instrs =
        concat(pattern(kTmp, data("beam_y")), {mov(kA, data("sink")), kRts});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("tmp read after") {
    const Instrs instrs =
        concat(pattern(kTmp, data("beam_y")), {mov(kTmp, data("sink")), kRts});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("the index is Y") {
    Instrs instrs = concat(pattern(kTmp, data("beam_y")), {kRts});
    instrs[0] = mov(ir::IndexedData{.name = "tbl", .offset = 0, .index = ir::Y{}}, kA);
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a compare against an indirect operand") {
    // CPY takes only an immediate, zero-page, or absolute operand.
    const Instrs instrs = concat(pattern(kTmp, ir::Indirect{.offset = 0}), {kRts});
    CHECK(run(instrs) == instrs);
  }
}

TEST_CASE("index_inc_cmp_y needs the temporary dead and the compiler's") {
  SUBCASE("a user global") {
    // `top = tbl[i]; top++; if (top == 9) ...` with a global `top` must
    // keep the store, since the caller can read `top`.
    const Instrs instrs = concat(pattern(data("top"), ir::Imm{.value = 9}), {kRts});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a read reached by the back edge") {
    // The read of `tmp` at the loop top comes before the pattern in
    // program order, and only the back edge reaches it.
    const Instrs instrs = concat(
        concat({ir::Label{.name = ".top"}, mov(kTmp, data("out"))},
               pattern(kTmp, ir::Imm{.value = 9})),
        {ir::Branch{.cond = ir::NE{}, .target = ".top"}, kRts});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a BIT after the match") {
    // The `LDY #0` ends the Y liveness walk before it reaches the `BIT`.
    const Instrs instrs = concat(pattern(kTmp, ir::Imm{.value = 9}),
                                 {mov(ir::Imm{.value = 0}, kY),
                                  ir::BitTest{.src = data("flags")}, kRts});
    const Instrs out = run(instrs);
    CHECK(std::get<ir::Mov>(out[0]).dst == kY);
  }
}
