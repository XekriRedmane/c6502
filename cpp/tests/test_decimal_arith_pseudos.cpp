// Tests of the parts of passes/decimal_arith.py that the asm SSA round
// trip and the interference graphs read: the Pseudo reads and writes,
// the maps over the reads and the writes, and the early-write pairs.
// The Python module has no unit tests of these, so the cases follow its
// docstrings, on a three-byte chain whose destination shares a name
// with a source.
#include <string>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "doctest.h"
#include "passes/decimal_arith.h"

namespace ir = c6502::asm_ast;
namespace asdl = c6502::asdl;
namespace decimal_arith = c6502::passes::decimal_arith;

namespace {

// `d = a + d` over three bytes, with an immediate for byte 1 of `a` and
// the top byte of the result stored in `z`.
ir::DecimalArith chain() {
  return std::get<ir::DecimalArith>(asdl::load<ir::Type_instruction>(
      "DecimalArith(false [Pseudo(\"a\" 0) Imm(5) Pseudo(\"a\" 2)] "
      "[Pseudo(\"d\" 0) Pseudo(\"d\" 1) Pseudo(\"d\" 2)] "
      "[Pseudo(\"d\" 0) Pseudo(\"d\" 1) Data(\"z\" 0)])"));
}

std::vector<std::string> names(const std::vector<ir::Pseudo>& ps) {
  std::vector<std::string> out;
  for (const auto& p : ps) {
    out.push_back(p.name + "+" + asdl::int_to_string(p.offset));
  }
  return out;
}

}  // namespace

TEST_CASE("decimal_arith: the Pseudo reads and writes skip the other operands") {
  const auto instr = chain();
  CHECK(names(decimal_arith::pseudo_reads(instr)) ==
        std::vector<std::string>{"a+0", "a+2", "d+0", "d+1", "d+2"});
  CHECK(names(decimal_arith::pseudo_writes(instr)) == std::vector<std::string>{"d+0", "d+1"});
}

TEST_CASE("decimal_arith: map_reads and map_writes change one side each") {
  const auto to_imm = [](const ir::Type_operand& op) -> ir::Type_operand {
    return asdl::isa<ir::Pseudo>(op) ? ir::Type_operand{ir::Imm{.value = 0}} : op;
  };
  const auto reads = decimal_arith::map_reads(chain(), to_imm);
  CHECK(asdl::dump(ir::Type_instruction{reads}) ==
        "DecimalArith(false [Imm(0) Imm(5) Imm(0)] [Imm(0) Imm(0) Imm(0)] "
        "[Pseudo(\"d\" 0) Pseudo(\"d\" 1) Data(\"z\" 0)])\n");
  const auto writes = decimal_arith::map_writes(chain(), to_imm);
  CHECK(asdl::dump(ir::Type_instruction{writes}) ==
        "DecimalArith(false [Pseudo(\"a\" 0) Imm(5) Pseudo(\"a\" 2)] "
        "[Pseudo(\"d\" 0) Pseudo(\"d\" 1) Pseudo(\"d\" 2)] [Imm(0) Imm(0) Data(\"z\" 0)])\n");
}

TEST_CASE("decimal_arith: each destination byte conflicts with the later sources of other names") {
  // dst[0] is written before a+2 is read, and so is dst[1]. The bytes of
  // `d` itself are the same byte k on both sides, and `z` isn't a
  // Pseudo.
  const auto instr = chain();
  using Pairs = std::vector<std::pair<std::string, std::string>>;
  CHECK(decimal_arith::early_write_pairs(instr) == Pairs{{"d", "a"}, {"d", "a"}});
  CHECK(decimal_arith::later_sources(instr, 1).size() == 2);
  CHECK(decimal_arith::later_sources(instr, 2).empty());
}
