// The tests of tests/test_loop_constant_a.py, ported.
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/loop_constant_a.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_loop_constant_a;

namespace {

using Instrs = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kX = ir::Reg{.reg = ir::X{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst};
}

ir::Type_operand imm(int v) { return ir::Imm{.value = v}; }

Instrs run(const Instrs& instrs) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = false, .instructions = instrs}}};
  prog = apply_loop_constant_a(std::move(prog));
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

// The loop `c99_to_tac._emit_zero_bytes` leaves under -O:
//
//     LDY #20
// .clear:
//     DEY
//     LDA #0
//     STA v,Y
//     TYA
//     BNE .clear
//     <after>
//     RTS
Instrs clear_loop(const Instrs& after = {}) {
  Instrs out = {
      mov(imm(20), kY),
      ir::Label{.name = ".clear"},
      ir::Dec{.dst = kY},
      mov(imm(0), kA),
      mov(kA, ir::IndexedData{.name = "v", .offset = 0, .index = ir::Y{}}),
      mov(kY, kA),
      ir::Branch{.cond = ir::NE{}, .target = ".clear"},
  };
  out.insert(out.end(), after.begin(), after.end());
  out.push_back(ir::Return{});
  return out;
}

void insert_at(Instrs& v, std::size_t at, const Instrs& items) {
  v.insert(v.begin() + static_cast<std::ptrdiff_t>(at), items.begin(), items.end());
}

}  // namespace

TEST_CASE("loop_constant_a moves the clearing loop's load in front of it") {
  // The load goes in front of the header, and the TYA goes, since the
  // BNE reads the flags DEY set.
  CHECK(run(clear_loop()) ==
        Instrs{
            mov(imm(20), kY),
            mov(imm(0), kA),
            ir::Label{.name = ".clear"},
            ir::Dec{.dst = kY},
            mov(kA, ir::IndexedData{.name = "v", .offset = 0, .index = ir::Y{}}),
            ir::Branch{.cond = ir::NE{}, .target = ".clear"},
            ir::Return{},
        });
}

TEST_CASE("loop_constant_a leaves a loop whose transfer's A is read") {
  // A store between the TYA and the branch reads the TYA's A.
  Instrs loop = clear_loop();
  insert_at(loop, 6, {mov(kA, ir::Data{.name = "out"})});
  CHECK(run(loop) == loop);
  // With the store after the loop, the TYA's A is read there.
  loop = clear_loop({mov(kA, ir::Data{.name = "out"})});
  CHECK(run(loop) == loop);
}

TEST_CASE("loop_constant_a leaves a loop with two constants") {
  // Loads of 0 and of $FF in one loop leave A changing.
  Instrs loop = clear_loop();
  insert_at(loop, 4, {mov(imm(0xFF), kA), mov(kA, ir::Data{.name = "w"})});
  CHECK(run(loop) == loop);
}

TEST_CASE("loop_constant_a leaves a loop with another write of A") {
  // An LDA of a byte in the loop is another value in A.
  Instrs loop = clear_loop();
  insert_at(loop, 2,
            {mov(ir::Data{.name = "m"}, kA), mov(kA, ir::Data{.name = "w"})});
  CHECK(run(loop) == loop);
}

TEST_CASE("loop_constant_a leaves a loop with a call") {
  Instrs loop = clear_loop();
  insert_at(loop, 2, {ir::Call{.name = "g"}});
  CHECK(run(loop) == loop);
}

TEST_CASE("loop_constant_a leaves a loop with A live at the header") {
  // A read before the load in the loop reads the value A had before the
  // loop on the first pass.
  Instrs loop = clear_loop();
  insert_at(loop, 2, {mov(kA, ir::Data{.name = "w"})});
  CHECK(run(loop) == loop);
}

TEST_CASE("loop_constant_a keeps a load whose flags are read") {
  Instrs loop = clear_loop();
  insert_at(loop, 4, {ir::Branch{.cond = ir::EQ{}, .target = ".clear"}});
  CHECK(run(loop) == loop);
}

TEST_CASE("loop_constant_a leaves a loop entered by a jump") {
  // A jump to the header from outside the loop would skip the new load.
  Instrs loop = {ir::Jump{.target = ".clear"}, ir::Label{.name = ".other"}};
  const Instrs tail = clear_loop();
  loop.insert(loop.end(), tail.begin(), tail.end());
  CHECK(run(loop) == loop);
}

TEST_CASE("loop_constant_a leaves a header after a terminator") {
  // Code before the header that ends in RTS doesn't fall into the loop,
  // so nothing would run the new load.
  Instrs loop = clear_loop();
  insert_at(loop, 1, {ir::Return{}});
  CHECK(run(loop) == loop);
}

TEST_CASE("loop_constant_a leaves an INC indexed by Y") {
  // There's no INC abs,Y, so `asm_emit` goes through A for it.
  Instrs loop = clear_loop();
  insert_at(loop, 2,
            {ir::Inc{.dst = ir::IndexedData{.name = "w", .offset = 0, .index = ir::Y{}}}});
  CHECK(run(loop) == loop);
}

TEST_CASE("loop_constant_a moves the load of a loop with no transfer") {
  // LDX #0; .l: LDA #0; STA v,X; INX; BNE .l moves the load only.
  const Instrs loop = {
      mov(imm(0), kX),
      ir::Label{.name = ".l"},
      mov(imm(0), kA),
      mov(kA, ir::IndexedData{.name = "v", .offset = 0, .index = ir::X{}}),
      ir::Inc{.dst = kX},
      ir::Branch{.cond = ir::NE{}, .target = ".l"},
      ir::Return{},
  };
  CHECK(run(loop) ==
        Instrs{loop[0], loop[2], loop[1], loop[3], loop[4], loop[5], loop[6]});
}
