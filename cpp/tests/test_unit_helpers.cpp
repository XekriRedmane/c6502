// The Python unit tests of helpers that the asm passes and code
// generation keep to themselves, ported by hand, one case per Python
// test with each of its assertions: apply_coloring's chains of
// transfers (tests/test_hwreg_pinning.py), redundant_load's
// `same_index_apart` (test_redundant_load.py), prologue synthesis's
// entry stubs (test_param_registers.py), the registers an instruction
// writes for the register allocation (test_asm_regalloc.py), the plan
// of members computed in place (test_in_place_member_update.py), and
// tac_to_asm's shift loop (test_narrow_byte_range.py), unary atoms, and
// labels (test_tac_to_asm.py). Each C++ helper is declared in its
// module's header for these cases.
#include <set>
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "passes/in_place_member_update.h"
#include "passes/optimization_asm/apply_coloring.h"
#include "passes/optimization_asm/regalloc.h"
#include "passes/prologue_synthesis.h"
#include "passes/redundant_load.h"
#include "tac_ast.h"
#include "tac_to_asm.h"
#include "tac_to_asm_translator.h"

namespace ir = c6502::asm_ast;
namespace tac = c6502::tac_ast;
using List = std::vector<ir::Type_instruction>;

namespace {

ir::Type_operand reg(ir::Type_reg r) { return ir::Reg{.reg = r}; }
const ir::Type_operand kA = reg(ir::A{});
const ir::Type_operand kX = reg(ir::X{});
const ir::Type_operand kY = reg(ir::Y{});

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst};
}
ir::Type_operand data(const char* name, int off = 0) {
  return ir::Data{.name = name, .offset = off};
}

}  // namespace

// --- test_hwreg_pinning.py's TestApplyColoringHwReg ---

TEST_CASE("hwreg_pinning: TestApplyColoringHwReg::test_self_transfer_chain_dropped") {
  // `TXA; TAX` does nothing to X, so both moves go and the LDA stays.
  const List result = c6502::passes::rewrite_redundant_transfers(
      {mov(kX, kA), mov(kA, kX), mov(ir::ZP{.address = 0x80, .offset = 0}, kA)});
  REQUIRE(result.size() == 1);
  CHECK(std::get<ir::Mov>(result[0]).src == ir::Type_operand{ir::ZP{.address = 0x80}});
}

TEST_CASE("hwreg_pinning: TestApplyColoringHwReg::test_cross_transfer_rewrites_indexed_data") {
  // `TYA; TAX; LDA arr,X` becomes `LDA arr,Y`.
  const List result = c6502::passes::rewrite_redundant_transfers(
      {mov(kY, kA), mov(kA, kX),
       mov(ir::IndexedData{.name = "arr", .offset = 0, .index = ir::X{}}, kA)});
  REQUIRE(result.size() == 1);
  CHECK(std::get<ir::Mov>(result[0]).src ==
        ir::Type_operand{ir::IndexedData{.name = "arr", .offset = 0, .index = ir::Y{}}});
}

TEST_CASE("hwreg_pinning: TestApplyColoringHwReg::test_match_transfer_chain_returns_letters") {
  // Python returns ("Y", "X", 2), the chain's two letters and its
  // length, which is always 2.
  const auto chain = c6502::passes::match_transfer_chain({mov(kY, kA), mov(kA, kX)}, 0);
  REQUIRE(chain.has_value());
  CHECK(chain->src == 'Y');
  CHECK(chain->dst == 'X');
}

TEST_CASE("hwreg_pinning: TestApplyColoringHwReg::test_match_returns_none_for_non_chain") {
  CHECK_FALSE(c6502::passes::match_transfer_chain({mov(kY, kA), ir::ClearCarry{}}, 0));
}

// --- test_redundant_load.py ---

namespace {

// test_redundant_load.py's `TestSwapPairs._indexed_join`: the join
// storing a slot and then `m` (or the slot) to two bytes of `z` indexed
// by `index`.
List indexed_join(ir::Type_reg index, const ir::Type_operand* m) {
  const auto z = [&](int k) {
    return ir::Type_operand{ir::IndexedData{.name = "z", .offset = k, .index = index}};
  };
  const ir::Type_operand t = data("__local_f__t");
  return {mov(data("hi"), kA),
          mov(kA, t),
          mov(data("__local_f__lo"), kA),
          mov(kA, z(0)),
          mov(m != nullptr ? *m : t, kA),
          mov(kA, z(1)),
          mov(ir::Imm{.value = 1}, kA),
          ir::Return{}};
}

}  // namespace

TEST_CASE("redundant_load: TestSwapPairs::test_indexed_bytes_through_y_setup") {
  // The `LDY #off` of a soft-stack load between the stores changes Y,
  // so `z,Y` and `z+1,Y` may be one byte.
  const auto z = [](int k) {
    return ir::Type_operand{ir::IndexedData{.name = "z", .offset = k, .index = ir::Y{}}};
  };
  const List plain = indexed_join(ir::Y{}, nullptr);
  CHECK(c6502::passes::same_index_apart(z(0), z(1),
                                        std::span(plain).subspan(2, 4)));
  const ir::Type_operand frame = ir::Frame{.offset = 3};
  const List setup = indexed_join(ir::Y{}, &frame);
  CHECK_FALSE(c6502::passes::same_index_apart(z(0), z(1),
                                              std::span(setup).subspan(2, 4)));
}

// --- test_param_registers.py ---

TEST_CASE("param_registers: TestPrologue::test_y_stub_goes_before_the_prologue") {
  // The soft-stack prologue writes Y, so the store of a parameter that
  // arrives in Y has to come first.
  const ir::Type_instruction stub = mov(kY, data("__zpabi_f__lo"));
  const List rest = {ir::Return{}};
  List all = {stub};
  all.insert(all.end(), rest.begin(), rest.end());
  const auto [stubs, body] = c6502::passes::split_entry_stubs(all);
  CHECK(stubs == List{stub});
  CHECK(body == rest);
}

// --- test_asm_regalloc.py ---

TEST_CASE("asm_regalloc: TestFrameAccessWritesY::test_frame_name_counts_as_y_write") {
  // A pseudo that `replace_pseudoregisters` puts in the frame is read
  // with `LDY #off; LDA (FP),Y`.
  const ir::Type_instruction read = mov(ir::Pseudo{.name = "@1.p0"}, kA);
  CHECK(c6502::passes::instr_writes_hwregs(read, nullptr).empty());
  const c6502::passes::NameSet frame_names = {"@1.p0"};
  CHECK(c6502::passes::instr_writes_hwregs(read, &frame_names) == std::vector<char>{'Y'});
}

// --- test_narrow_byte_range.py ---

TEST_CASE("narrow_byte_range: TestShiftLoop::test_count_in_destination_is_read_first") {
  // The count that's also the destination is read before the copy into
  // the destination overwrites it.
  const ir::Type_operand x = ir::Pseudo{.name = "x"};
  const ir::Type_operand n = ir::Pseudo{.name = "n"};
  const List out = c6502::tac_to_asm::shift_loop(x, n, std::nullopt, n, 1,
                                                 c6502::tac_to_asm::ShiftKind::kAsl, ".l", ".d",
                                                 true);
  REQUIRE(out.size() >= 3);
  CHECK(List(out.begin(), out.begin() + 3) == List{mov(n, kA), mov(kA, kX), mov(x, n)});
}

// --- test_in_place_member_update.py's TestPlan ---

namespace {

// `u = src1 + 1; P[0] = u`, planned with every name but P and W a local
// and every value 2 bytes.
std::vector<c6502::passes::InPlaceUpdate> plan(const char* src1) {
  const std::vector<tac::Type_instruction> instrs = {
      c6502::asdl::load<tac::Type_instruction>(std::string("Binary(Add Var(\"") + src1 +
                                               "\") Constant(ConstUInt(1)) Var(\"u\"))"),
      c6502::asdl::load<tac::Type_instruction>(
          "IndexedSymbolStore(\"P\" Constant(ConstUChar(0)) Var(\"u\") false 0)"),
  };
  return c6502::passes::plan_in_place_updates(
      instrs, {}, [](std::string_view name) { return name != "P" && name != "W"; },
      [](const tac::Type_val&) { return 2; });
}

}  // namespace

TEST_CASE("in_place_member_update: TestPlan::test_value_from_elsewhere_is_computed_in_place") {
  const auto out = plan("W");
  REQUIRE(out.size() == 1);
  CHECK(out[0].store == 1);
  CHECK(out[0].u == "u");
  CHECK(out[0].obj == "P");
  CHECK(out[0].base == 0);
  CHECK_FALSE(out[0].load.has_value());
  CHECK_FALSE(out[0].t.has_value());
}

TEST_CASE("in_place_member_update: TestPlan::test_value_that_reads_the_object_isnt") {
  CHECK(plan("P").empty());
}

// --- test_tac_to_asm.py ---

TEST_CASE("tac_to_asm: TestTranslateUnopAtoms::test_complement_emits_xor_with_ff") {
  CHECK(c6502::tac_to_asm::unop_atoms(tac::Complement{}) ==
        List{ir::Xor{.src1 = kA, .src2 = ir::Imm{.value = 0xFF}, .dst = kA}});
}

TEST_CASE("tac_to_asm: TestTranslateUnopAtoms::test_negate_emits_xor_clearcarry_add_one") {
  CHECK(c6502::tac_to_asm::unop_atoms(tac::Negate{}) ==
        List{ir::Xor{.src1 = kA, .src2 = ir::Imm{.value = 0xFF}, .dst = kA}, ir::ClearCarry{},
             ir::Add{.src = ir::Imm{.value = 1}, .dst = kA}});
}

TEST_CASE("tac_to_asm: TestTranslateComparisons::test_labels_are_unique_across_compares_in_one_translator") {
  // A Translator used again, as within a program, keeps counting its
  // labels, so two compares get disjoint ones.
  const c6502::passes::AsmContext ctx;
  const c6502::tac_to_asm::Options options;
  c6502::tac_to_asm::Translator t(nullptr, nullptr, ctx, options);
  const tac::Type_val src1 = tac::Var{.name = "%0"};
  const tac::Type_val src2 = tac::Constant{.const_ = tac::ConstInt{.value = 5}};
  const tac::Type_val dst = tac::Var{.name = "%1"};
  const auto labels = [](const List& instrs) {
    std::set<std::string> out;
    for (const auto& i : instrs) {
      if (const auto* l = std::get_if<ir::Label>(&i)) out.insert(l->name);
    }
    return out;
  };
  const auto first = labels(t.translate_binary(tac::Equal{}, src1, src2, dst));
  const auto second = labels(t.translate_binary(tac::Equal{}, src1, src2, dst));
  CHECK_FALSE(first.empty());
  for (const auto& name : first) {
    CHECK(second.count(name) == 0);
  }
}
