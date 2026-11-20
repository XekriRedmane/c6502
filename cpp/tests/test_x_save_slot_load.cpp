// The tests of tests/test_x_save_slot_load.py, ported.
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/x_save_slot_load.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_x_save_slot_load;

namespace {

using Instrs = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kX = ir::Reg{.reg = ir::X{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};

ir::Type_operand data(const char* name = "M") { return ir::Data{.name = name, .offset = 0}; }

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst, .is_volatile = false};
}

Instrs run(Instrs instrs) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = true, .instructions = instrs}}};
  prog = apply_x_save_slot_load(std::move(prog));
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

}  // namespace

TEST_CASE("x_save_slot_load rewrites reads of the X-save slot") {
  SUBCASE("LDA M becomes TXA") {
    CHECK(run({mov(kX, data()), mov(data(), kA)})[1] == mov(kX, kA));
  }
  SUBCASE("a memory-to-memory move becomes STX") {
    CHECK(run({mov(kX, data()), mov(data(), data("other"))})[1] == mov(kX, data("other")));
  }
  SUBCASE("LDX M stays") {
    // `Mov(M, Reg(X))`, the LDX M restore, is left for the downstream
    // peepholes, which recognize the save-restore shape.
    CHECK(run({mov(kX, data()), mov(data(), kX)})[1] == mov(data(), kX));
  }
  SUBCASE("nothing happens without an STX M") {
    const Instrs instrs = {mov(ir::Imm{.value = 1}, data()), mov(data(), kA)};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("an INC of M disqualifies it") {
    const Instrs instrs = {mov(kX, data()), ir::Inc{.dst = data()}, mov(data(), kA)};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("an STY M disqualifies it") {
    // STY M leaves A unchanged, so a following TAX wouldn't pick up
    // M's value.
    const Instrs instrs = {mov(kX, data()), mov(kY, data()), mov(kA, kX), mov(data(), kA)};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("the init shape LDA #c; STA M; TAX is accepted") {
    const Instrs instrs = {mov(ir::Imm{.value = 1}, data()), mov(kA, kX), mov(kX, data()),
                           mov(data(), kA)};
    CHECK(run(instrs)[3] == mov(kX, kA));
  }
  SUBCASE("the loop with DEX that read a stale slot") {
    // The move into the callee's slot has to read the value after the
    // DEX, which X holds.
    const ir::Type_operand callee_slot = data("zpabi_callee_slot");
    const Instrs instrs = {
        mov(ir::Imm{.value = 1}, data()),
        mov(kA, kX),
        ir::Label{.name = ".loop"},
        mov(data(), callee_slot),
        mov(kX, data()),
        ir::Call{.name = "callee"},
        mov(data(), kX),
        ir::Dec{.dst = kX},
        ir::Branch{.cond = ir::PL{}, .target = ".loop"},
        ir::Return{.save_a = false},
    };
    CHECK(run(instrs)[3] == mov(kX, callee_slot));
  }
}

TEST_CASE("x_save_slot_load needs X to equal M wherever it rewrites") {
  SUBCASE("a call returning in X blocks the rewrite") {
    // `STX p; JSR f; LDA p`: f returns its high byte in X, so X no
    // longer holds the register parameter saved in p.
    const Instrs instrs = {mov(kX, data("p")), ir::Call{.name = "f"}, mov(data("p"), kA),
                           ir::Return{.save_a = true}};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("an LDX of another value blocks the rewrite") {
    // paint_strip_reg: `STX color; LDX x_pixel; LDA color` must read
    // memory, since X holds x_pixel.
    const Instrs instrs = {mov(kX, data("color")), mov(data("x_pixel"), kX),
                           mov(data("color"), kA), ir::Return{.save_a = true}};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a call with the restore still rewrites") {
    const Instrs instrs = {mov(kX, data()), ir::Call{.name = "f"}, mov(data(), kX),
                           ir::Dec{.dst = kX}, mov(data(), kA), ir::Return{.save_a = true}};
    CHECK(run(instrs)[4] == mov(kX, kA));
  }
  SUBCASE("a read before X is given the slot stays") {
    // `LDA p; STA out; LDX p; DEX; STX p`: at the `LDA p`, X still
    // holds whatever the caller left in it.
    const Instrs instrs = {mov(data("p"), kA), mov(kA, data("out")), mov(data("p"), kX),
                           ir::Dec{.dst = kX}, mov(kX, data("p")), ir::Return{.save_a = false}};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a read at a join with a path that never synced X stays") {
    const Instrs instrs = {
        mov(data("c"), kA),
        ir::Branch{.cond = ir::EQ{}, .target = ".join"},
        mov(data(), kX),
        ir::Dec{.dst = kX},
        mov(data(), kA),
        mov(kX, data()),
        ir::Label{.name = ".join"},
        mov(data(), kA),
        ir::Return{.save_a = true},
    };
    const Instrs out = run(instrs);
    CHECK(out[4] == mov(kX, kA));
    CHECK(out[7] == instrs[7]);
  }
}
