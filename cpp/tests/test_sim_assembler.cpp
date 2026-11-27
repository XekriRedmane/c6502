// Tests of the simulator's assembler (src/sim/assembler.h). The
// messages are sim/assembler.py's for the same programs.
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "sim/assembler.h"

namespace {

namespace ir = c6502::asm_ast;
using c6502::sim::AssembledProgram;
using c6502::sim::assemble;
using c6502::sim::AssemblerError;

const ir::Type_instruction kRts = ir::Return{};

ir::Type_top_level function(const std::string& name, std::vector<ir::Type_instruction> instrs) {
  return ir::Function{.name = name, .is_global = true, .params = {}, .instructions = std::move(instrs)};
}

ir::Type_instruction lda_imm(int v) {
  return ir::Mov{.src = ir::Imm{.value = v}, .dst = ir::Reg{.reg = ir::A{}}};
}

std::vector<ir::Type_instruction> filler(int n) { return std::vector<ir::Type_instruction>(n, lda_imm(1)); }

std::string error_of(const std::vector<ir::Type_top_level>& top) {
  try {
    assemble(ir::Program{.top_level = top});
  } catch (const AssemblerError& e) {
    return e.what();
  }
  return "no error";
}

}  // namespace

TEST_CASE("sim assembler: test_element_rmw.py::TestEmit::test_sim_assembler_encodings") {
  // INC, DEC, and the shifts of an element indexed by X are abs,X.
  const ir::Type_operand e = ir::IndexedData{.name = "tab", .offset = 0, .index = ir::X{}};
  std::vector<ir::Type_instruction> instrs = {
      ir::Inc{.dst = e},
      ir::Dec{.dst = e},
      ir::ArithmeticShiftLeft{.dst = e},
      ir::LogicalShiftRight{.dst = e},
      ir::RotateLeft{.dst = e},
      ir::RotateRight{.dst = e},
      kRts,
  };
  const ir::Program prog{.top_level = {
                             function("f", instrs),
                             ir::StaticVariable{.name = "tab", .is_global = false,
                                                .init = {ir::ZeroInit{.bytes = 4}}},
                         }};
  const AssembledProgram out = assemble(prog, 0x0800);
  const std::int64_t tab = out.symbols.at("tab");
  std::vector<std::uint8_t> want;
  for (const int op : {0xFE, 0xDE, 0x1E, 0x5E, 0x3E, 0x7E}) {
    want.push_back(static_cast<std::uint8_t>(op));
    want.push_back(static_cast<std::uint8_t>(tab & 0xFF));
    want.push_back(static_cast<std::uint8_t>(tab >> 8));
  }
  want.push_back(0x60);
  const std::vector<std::uint8_t> code(out.image.begin() + 0x0800, out.image.begin() + 0x0800 + 19);
  CHECK(code == want);
  CHECK(c6502::sim::instruction_size(ir::Inc{.dst = e}) == 3);
}

TEST_CASE("sim assembler: the instructions the corpus doesn't reach") {
  // BIT, PHA, PLA, a jump table, packed BCD arithmetic, and SED / CLD,
  // with the runtime's FP and SSP moved, and a `float` static. The bytes
  // are sim/assembler.py's.
  const ir::Type_operand a = ir::Reg{.reg = ir::A{}};
  const std::vector<ir::Type_instruction> instrs = {
      ir::BitTest{.src = ir::Data{.name = "HARGS", .offset = 2}},
      ir::BitTest{.src = ir::Data{.name = "t", .offset = 1}},
      ir::BitTest{.src = ir::ZP{.address = 0x80, .offset = 1}},
      ir::Push{.src = a},
      ir::Pop{.dst = a},
      ir::JumpTable{.table = "jt", .index = "Y", .base = 1, .targets = {"l1", "l2"}},
      ir::Label{.name = "l1"},
      ir::DecimalArith{.subtract = false,
                       .left = {ir::Data{.name = "t"}, ir::Frame{.offset = 3}},
                       .right = {ir::Imm{.value = 0x15}, ir::IndirectZp{.address = 0x90, .offset = 2}},
                       .dst = {ir::Data{.name = "t"}, ir::Frame{.offset = 4}}},
      ir::Label{.name = "l2"},
      ir::DecimalArith{.subtract = true,
                       .left = {ir::ZP{.address = 0x82}},
                       .right = {ir::Data{.name = "HARGS", .offset = 1}},
                       .dst = {ir::Stack{.offset = 1}}},
      ir::SetDecimal{},
      ir::ClearDecimal{},
      kRts,
  };
  const ir::Program prog{
      .top_level = {function("f", instrs),
                    ir::StaticVariable{.name = "t", .is_global = false,
                                       .init = {ir::FloatInit{.bits = 0x3FC00000},
                                                ir::CharInit{.value = -1}}}}};
  const AssembledProgram out = assemble(prog, 0x0800, {{"FP", 0x10}, {"SSP", 0x12}});
  const std::vector<std::uint8_t> want = {
      0x24, 0x06, 0x2C, 0x3E, 0x08, 0x24, 0x81, 0x48, 0x68, 0x0A, 0xA8, 0xB9, 0x13, 0x08,
      0x48, 0xB9, 0x12, 0x08, 0x48, 0x60, 0x17, 0x08, 0x2E, 0x08, 0xF8, 0x18, 0xAD, 0x3D,
      0x08, 0x69, 0x15, 0x8D, 0x3D, 0x08, 0xA0, 0x03, 0xB1, 0x10, 0xA0, 0x02, 0x71, 0x90,
      0xA0, 0x04, 0x91, 0x10, 0xD8, 0xF8, 0x38, 0xA5, 0x82, 0xE5, 0x05, 0xA0, 0x01, 0x91,
      0x12, 0xD8, 0xF8, 0xD8, 0x60, 0x00, 0x00, 0xC0, 0x3F, 0xFF};
  CHECK(out.code_end == 0x842);
  CHECK(std::vector<std::uint8_t>(out.image.begin() + 0x0800, out.image.begin() + 0x842) == want);
  const std::map<std::string, std::int64_t> symbols = {
      {"SSP", 0x12}, {"FP", 0x10}, {"HARGS", 0x04}, {"DPTR", 0x24}, {"f", 0x800},
      {"l1", 0x818}, {"l2", 0x82F}, {"t", 0x83D}};
  CHECK(out.symbols == symbols);
}

TEST_CASE("sim assembler: an undefined symbol") {
  CHECK(error_of({function("f", {ir::Call{.name = "nowhere"}, kRts})}) ==
        "undefined symbol 'nowhere'");
  CHECK(error_of({function("f", {ir::Branch{.cond = ir::EQ{}, .target = "nowhere"}, kRts})}) ==
        "undefined branch target 'nowhere'");
}

TEST_CASE("sim assembler: a branch out of range") {
  // A displacement of 127 forward and of -128 back reaches.
  std::vector<ir::Type_instruction> forward = {ir::Branch{.cond = ir::EQ{}, .target = "far"}};
  for (const auto& i : filler(63)) forward.push_back(i);
  forward.push_back(ir::ClearCarry{});
  forward.push_back(ir::Label{.name = "far"});
  forward.push_back(kRts);
  CHECK(error_of({function("f", forward)}) == "no error");
  forward.insert(forward.begin() + 1, ir::ClearCarry{});
  CHECK(error_of({function("f", forward)}) == "branch to 'far' out of range: disp=128");

  std::vector<ir::Type_instruction> back = {ir::Label{.name = "top"}};
  for (const auto& i : filler(63)) back.push_back(i);
  back.push_back(ir::Branch{.cond = ir::NE{}, .target = "top"});
  back.push_back(kRts);
  CHECK(error_of({function("f", back)}) == "no error");
  back.insert(back.begin() + 1, lda_imm(1));
  CHECK(error_of({function("f", back)}) == "branch to 'top' out of range: disp=-130");
}

TEST_CASE("sim assembler: a duplicate label or symbol") {
  CHECK(error_of({function("f", {ir::Label{.name = "x"}, ir::Label{.name = "x"}, kRts})}) ==
        "duplicate label 'x'");
  CHECK(error_of({function("f", {kRts}),
                  ir::StaticVariable{.name = "f", .is_global = false,
                                     .init = {ir::ZeroInit{.bytes = 1}}}}) ==
        "duplicate symbol 'f'");
  // The zero-page symbols are defined before the program's.
  CHECK(error_of({function("SSP", {kRts})}) == "duplicate symbol 'SSP'");
}

TEST_CASE("sim assembler: a program past $FFFF") {
  // tests/chapter_15/valid/initialization/static_nested.c is a program
  // like this one.
  const ir::Type_top_level big = ir::StaticVariable{
      .name = "big", .is_global = false, .init = {ir::ZeroInit{.bytes = 0xF800}, ir::CharInit{.value = 1}}};
  CHECK(error_of({big}) == "the program's bytes end at $10001, past $FFFF");
  const ir::Type_top_level fits = ir::StaticVariable{
      .name = "big", .is_global = false, .init = {ir::ZeroInit{.bytes = 0xF7FF}, ir::CharInit{.value = 1}}};
  CHECK(error_of({fits}) == "no error");
}

TEST_CASE("sim assembler: an origin in zero page") {
  // A static below $100 would be sized with absolute addressing in pass
  // 1, before its address is known, and written with zero-page
  // addressing in pass 2.
  const ir::Program prog{.top_level = {function("f", {kRts})}};
  CHECK_THROWS_WITH_AS(assemble(prog, 0xFF), "origin 0xff is below 0x100, and only the zero-page symbols "
                       "assemble starts from may lie there", AssemblerError);
  CHECK_THROWS_AS(assemble(prog, -1), AssemblerError);
  CHECK(assemble(prog, 0x100).symbols.at("f") == 0x100);
}

TEST_CASE("sim assembler: an operand that doesn't fit") {
  CHECK(error_of({function("f", {lda_imm(256)})}) == "immediate 256 out of range 0..255");
  CHECK(error_of({function("f", {ir::Mov{.src = ir::Imm{.value = 1},
                                         .dst = ir::IndexedData{.name = "t", .index = ir::X{}}}})}) ==
        "can't store A into IndexedData(name='t', offset=0, index=X())");
}
