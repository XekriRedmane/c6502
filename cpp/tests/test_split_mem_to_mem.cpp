// The tests of tests/test_split_mem_to_mem.py, ported.
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/split_mem_to_mem.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_split_mem_to_mem;

namespace {

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kX = ir::Reg{.reg = ir::X{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};

ir::Type_operand data(const char* name) {
  return ir::Data{.name = name, .offset = 0};
}

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst,
                         bool is_volatile = false) {
  return ir::Mov{.src = src, .dst = dst, .is_volatile = is_volatile};
}

std::vector<ir::Type_instruction> run(std::vector<ir::Type_instruction> instrs) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = true, .instructions = instrs}}};
  prog = apply_split_mem_to_mem(std::move(prog));
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

}  // namespace

TEST_CASE("split_mem_to_mem splits memory-to-memory moves through A") {
  SUBCASE("Data to Data") {
    CHECK(run({mov(data("src"), data("dst"))}) ==
          std::vector<ir::Type_instruction>{mov(data("src"), kA),
                                            mov(kA, data("dst"))});
  }
  SUBCASE("ZP to Data") {
    const ir::Type_operand zp = ir::ZP{.address = 0x80, .offset = 0};
    CHECK(run({mov(zp, data("dst"))}) ==
          std::vector<ir::Type_instruction>{mov(zp, kA), mov(kA, data("dst"))});
  }
  SUBCASE("IndexedData to Data") {
    // asm_emit writes Mov(IndexedData, mem) as LDA name,X; STA dst, the
    // same shape as any other move from memory to memory.
    const ir::Type_operand src =
        ir::IndexedData{.name = "floor_ceil", .offset = 0, .index = ir::X{}};
    CHECK(run({mov(src, data("dst"))}) ==
          std::vector<ir::Type_instruction>{mov(src, kA), mov(kA, data("dst"))});
  }
  SUBCASE("a volatile move stays one atom") {
    const auto instr = mov(data("src"), data("dst"), true);
    CHECK(run({instr}) == std::vector<ir::Type_instruction>{instr});
  }
  SUBCASE("three moves from one source") {
    CHECK(run({mov(data("src"), data("d1")), mov(data("src"), data("d2")),
               mov(data("src"), data("d3"))}) ==
          std::vector<ir::Type_instruction>{
              mov(data("src"), kA), mov(kA, data("d1")), mov(data("src"), kA),
              mov(kA, data("d2")), mov(data("src"), kA), mov(kA, data("d3"))});
  }
}

TEST_CASE("split_mem_to_mem drops moves of an operand to itself") {
  CHECK(run({mov(data("m"), data("m"))}).empty());
  const ir::Type_operand zp = ir::ZP{.address = 0x85, .offset = 0};
  CHECK(run({mov(zp, zp)}).empty());
  CHECK(run({mov(kX, kX)}).empty());
}

TEST_CASE("split_mem_to_mem leaves other moves alone") {
  const std::vector<ir::Type_instruction> instrs = {
      mov(ir::Imm{.value = 5}, data("dst")),
      mov(kA, data("dst")),
      mov(data("src"), kA),
      mov(kX, kA),
      mov(kA, kX),
  };
  CHECK(run(instrs) == instrs);
}

TEST_CASE("split_mem_to_mem splits a move between X and Y through A") {
  CHECK(run({mov(kX, kY)}) ==
        std::vector<ir::Type_instruction>{mov(kX, kA), mov(kA, kY)});
  CHECK(run({mov(kY, kX)}) ==
        std::vector<ir::Type_instruction>{mov(kY, kA), mov(kA, kX)});
}
