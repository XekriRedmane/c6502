// The asm tests of tests/test_counter_to_index_reg.py, ported. The
// tests that compile and run C stay in Python.
#include <algorithm>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/counter_to_index_reg.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_counter_to_index_reg;
using c6502::passes::SymbolAddrs;
using Instrs = std::vector<ir::Type_instruction>;

namespace {

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kX = ir::Reg{.reg = ir::X{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};
const ir::Type_operand kN = ir::Data{.name = "n", .offset = 0};
const ir::Type_operand kCount = ir::ZP{.address = 0x84, .offset = 0};

ir::Type_operand data(const char* name, int offset = 0) {
  return ir::Data{.name = name, .offset = offset};
}

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst, .is_volatile = false};
}

ir::Type_instruction label(const char* name) { return ir::Label{.name = name}; }

Instrs run(Instrs instrs, const SymbolAddrs* addrs = nullptr) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = true, .instructions = instrs}}};
  prog = apply_counter_to_index_reg(std::move(prog), addrs);
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

Instrs loop(const Instrs& body) {
  Instrs out{mov(kN, kCount), label(".loop")};
  out.insert(out.end(), body.begin(), body.end());
  out.push_back(ir::Dec{.dst = kCount});
  out.push_back(ir::Branch{.cond = ir::NE{}, .target = ".loop"});
  out.push_back(ir::Return{.save_a = false});
  return out;
}

bool contains(const Instrs& instrs, const ir::Type_instruction& i) {
  return std::find(instrs.begin(), instrs.end(), i) != instrs.end();
}

}  // namespace

TEST_CASE("counter_to_index_reg moves a pure counter into X or Y") {
  SUBCASE("the counter moves to X") {
    const Instrs out = run(loop({ir::Inc{.dst = data("t")}}));
    CHECK(out[0] == mov(kN, kX));
    CHECK(out[out.size() - 3] == ir::Type_instruction{ir::Dec{.dst = kX}});
  }
  SUBCASE("X used in the loop falls back to Y") {
    const Instrs out = run(loop({ir::Inc{.dst = kX}}));
    CHECK(out[0] == mov(kN, kY));
    CHECK(out[out.size() - 3] == ir::Type_instruction{ir::Dec{.dst = kY}});
  }
  SUBCASE("both registers used leaves the counter in memory") {
    const Instrs instrs = loop({ir::Inc{.dst = kX}, ir::Inc{.dst = kY}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a call in the loop leaves the counter in memory") {
    const Instrs instrs = loop({ir::Call{.name = "g"}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a counter read elsewhere stays in memory") {
    const Instrs instrs = loop({mov(kCount, kA)});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a register use on an exit path is fine") {
    // The `LDX #0` returns without reaching the DEC again, so the
    // counter is dead there.
    const Instrs out = run(loop({
        ir::Branch{.cond = ir::EQ{}, .target = ".found"},
        ir::Jump{.target = ".next"},
        label(".found"),
        mov(ir::Imm{.value = 0}, kX),
        ir::Return{.save_a = true, .save_x = true},
        label(".next"),
    }));
    CHECK(out[0] == mov(kN, kX));
  }
}

TEST_CASE("counter_to_index_reg leaves a byte of a (zp),Y pointer alone") {
  // A counter that is a byte of a `(zp),Y` pointer the loop reads
  // through. The read names the pointer by its address, so the two
  // references to the byte look like a pure counter's.
  const SymbolAddrs addrs{{"__local_f__p", 0x83}};
  auto instrs = [](const ir::Type_operand& counter) {
    return Instrs{
        mov(kN, counter),
        label(".loop"),
        mov(ir::Imm{.value = 0}, kY),
        mov(ir::IndirectZpY{.address = 0x83}, kA),
        mov(kA, data("t")),
        ir::Dec{.dst = counter},
        ir::Branch{.cond = ir::NE{}, .target = ".loop"},
        ir::Return{.save_a = false},
    };
  };
  SUBCASE("a slot named by its symbol stays in memory") {
    const Instrs in = instrs(data("__local_f__p", 1));
    CHECK(run(in, &addrs) == in);
  }
  SUBCASE("a zero-page byte stays in memory") {
    const Instrs in = instrs(ir::ZP{.address = 0x84, .offset = 0});
    CHECK(run(in, &addrs) == in);
  }
  SUBCASE("the byte after the pointer moves to X") {
    const Instrs out = run(instrs(ir::ZP{.address = 0x85, .offset = 0}), &addrs);
    CHECK(out[0] == mov(kN, kX));
  }
}

TEST_CASE("counter_to_index_reg with an init from a register") {
  // An init stored from X or Y. The 6502 has no TXY or TYX.
  auto loop_from = [](const ir::Type_operand& reg, const Instrs& after) {
    Instrs out{
        mov(ir::Imm{.value = 17}, reg),
        mov(reg, kCount),
        label(".loop"),
        ir::Inc{.dst = data("t")},
        ir::Dec{.dst = kCount},
        ir::Branch{.cond = ir::NE{}, .target = ".loop"},
    };
    out.insert(out.end(), after.begin(), after.end());
    out.push_back(ir::Return{.save_a = false});
    return out;
  };
  SUBCASE("the counter stays in the register it came from") {
    CHECK(run(loop_from(kY, {})) ==
          Instrs{
              mov(ir::Imm{.value = 17}, kY),
              label(".loop"),
              ir::Inc{.dst = data("t")},
              ir::Dec{.dst = kY},
              ir::Branch{.cond = ir::NE{}, .target = ".loop"},
              ir::Return{.save_a = false},
          });
  }
  SUBCASE("the counter is never moved between X and Y") {
    // Y is read after the loop, so the counter can't count it down, and
    // X would take `Mov(Y, X)`, which is `TYA; TAX`.
    const Instrs instrs = loop_from(kY, {mov(kY, data("u"))});
    const Instrs out = run(instrs);
    CHECK_FALSE(contains(out, mov(kY, kX)));
    CHECK(out == instrs);
  }
}

TEST_CASE("counter_to_index_reg takes a loop block placed out of line") {
  // A loop with one of its blocks after the function's return, as SSA
  // destruction leaves for one side of an `if`, is still one loop.
  auto make = [](bool jump_in_from_outside) {
    const ir::Type_operand t = data("t");
    Instrs out{
        mov(ir::Imm{.value = 17}, kCount),
        label(".loop"),
        mov(t, kA),
        ir::Branch{.cond = ir::CC{}, .target = ".split"},
        ir::Inc{.dst = t},
        label(".join"),
        ir::Dec{.dst = kCount},
        ir::Branch{.cond = ir::NE{}, .target = ".loop"},
        ir::Return{.save_a = false},
        label(".split"),
        mov(ir::Imm{.value = 0}, kA),
        ir::Jump{.target = ".join"},
    };
    if (jump_in_from_outside) {
      out.insert(out.begin(), ir::Jump{.target = ".join"});
    }
    return out;
  };
  SUBCASE("the counter moves to X") {
    CHECK(contains(run(make(false)), ir::Dec{.dst = kX}));
  }
  SUBCASE("a jump into the loop from outside leaves it alone") {
    // The counter would arrive at .join without its init.
    const Instrs instrs = make(true);
    CHECK(run(instrs) == instrs);
  }
}

TEST_CASE("counter_to_index_reg on the inner loop of another loop") {
  // A counter initialized inside another loop is dead on the outer
  // loop's way back to its init, so the outer loop may use the
  // register: the inner counter of `do { for (y = n; --y;) {} } while
  // (--count);` goes in Y while the outer one is in X.
  auto nested = [](const ir::Type_operand& inner_init) {
    return Instrs{
        mov(kN, kX),
        label(".outer"),
        mov(inner_init, data("y")),
        label(".inner"),
        ir::Dec{.dst = data("y")},
        ir::Branch{.cond = ir::NE{}, .target = ".inner"},
        ir::Dec{.dst = kX},
        ir::Branch{.cond = ir::NE{}, .target = ".outer"},
        ir::Return{.save_a = false},
    };
  };
  SUBCASE("the inner counter goes in Y") {
    const Instrs out = run(nested(ir::Imm{.value = 0xB4}));
    CHECK(out[2] == mov(ir::Imm{.value = 0xB4}, kY));
    CHECK(out[4] == ir::Type_instruction{ir::Dec{.dst = kY}});
  }
  SUBCASE("the outer register used inside the inner loop keeps memory") {
    Instrs instrs = nested(ir::Imm{.value = 0xB4});
    instrs.insert(instrs.begin() + 4, ir::Inc{.dst = kY});
    // Y is used in the inner loop and X holds the outer counter.
    CHECK(contains(run(instrs), ir::Dec{.dst = data("y")}));
  }
}
