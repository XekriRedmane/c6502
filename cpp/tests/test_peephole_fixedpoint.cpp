// The tests of tests/test_peephole_fixedpoint.py, ported, and a check
// that the sweep holds the passes of compile.py's
// `_peephole_fixedpoint` in its order.
#include <string>
#include <string_view>
#include <vector>

#include "asm_text.h"
#include "doctest.h"
#include "errors.h"
#include "passes/peephole_fixedpoint.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;
namespace ir = c6502::asm_ast;
namespace passes = c6502::passes;

ir::Program fixed_point(std::string_view instrs) {
  return passes::peephole_fixedpoint(program(instrs), passes::AsmContext{});
}

}  // namespace

TEST_CASE("peephole_fixedpoint converges") {
  SUBCASE("an empty function is unchanged") {
    const ir::Program prog = program("Return(false false false)");
    CHECK(passes::peephole_fixedpoint(prog, passes::AsmContext{}) == prog);
  }
  SUBCASE("a program with nothing to rewrite is unchanged") {
    // The store goes to a `Data` operand, a global the linker places,
    // so `asm_dead_store` keeps it live across the function's exit.
    const ir::Program prog = program(
        R"(Mov(ZP(128 0) Reg(A) false) Mov(Reg(A) Data("sink" 0) false)
           Return(false false false))");
    CHECK(passes::peephole_fixedpoint(prog, passes::AsmContext{}) == prog);
  }
  SUBCASE("a one-byte add of 1 becomes INC") {
    CHECK(instructions(fixed_point(
              "Mov(ZP(144 0) Reg(A) false) ClearCarry Add(Imm(1) Reg(A)) "
              "Mov(Reg(A) ZP(144 0) false) Return(false false false)")) ==
          expect("Inc(ZP(144 0)) Return(false false false)"));
  }
  SUBCASE("an INC and the load it makes redundant go in one sweep") {
    // Once `inc_peephole` makes the chain on $90 one INC, A still holds
    // $80 at the second `LDA $80`, since the INC writes another byte,
    // and `redundant_load` drops the load. The store goes to a `Data`
    // sink, which `asm_dead_store` keeps.
    CHECK(instructions(fixed_point(
              R"(Mov(ZP(128 0) Reg(A) false)
                 Mov(ZP(144 0) Reg(A) false) ClearCarry Add(Imm(1) Reg(A))
                 Mov(Reg(A) ZP(144 0) false)
                 Mov(ZP(128 0) Reg(A) false) Mov(Reg(A) Data("sink" 0) false)
                 Return(false false false))")) ==
          expect(R"(Mov(ZP(128 0) Reg(A) false) Inc(ZP(144 0))
                    Mov(Reg(A) Data("sink" 0) false) Return(false false false))"));
  }
}

TEST_CASE("peephole_fixedpoint stops at the cap") {
  // A pass that renames the function on every call makes every sweep
  // change the program, so the fixed point gives up after the cap.
  int calls = 0;
  std::vector<passes::SweepPass> sweep = {
      {"always_change", [&calls](ir::Program p) {
         ++calls;
         auto& fn = std::get<ir::Function>(p.top_level[0]);
         fn.name += "_x";
         return p;
       }}};
  try {
    passes::run_to_fixed_point(program("Return(false false false)"), sweep);
    FAIL("the fixed point converged");
  } catch (const c6502::InternalError& e) {
    CHECK(std::string(e.what()).find("didn't converge") != std::string::npos);
  }
  CHECK(calls == passes::kPeepholeFixedpointCap);
}

TEST_CASE("the sweep holds c6502's passes in its order") {
  const passes::SymbolAddrs none;
  std::vector<std::string> names;
  for (const auto& p : passes::peephole_sweep(passes::AsmContext{}, nullptr, &none, {})) {
    names.emplace_back(p.name);
  }
  const std::vector<std::string> expected = {
      "split_mem_to_mem", "carry_halve", "carry_in_add", "inc_peephole",
      "inc_low_byte_test", "carry_inc", "dec_peephole", "sub1_test_zero",
      "element_in_place", "direct_index_load", "dead_pha_pla", "cpx_cpy",
      "memory_value_propagation", "shift_in_a", "redundant_load_after_rmw",
      "redundant_load", "redundant_store", "constant_store_order", "join_store",
      "remat", "asm_dead_store", "loop_constant_a", "dead_a_arith",
      "dead_label_drop", "unreachable_drop", "branch_invert",
      "branch_to_next_drop", "branch_through_jump", "branch_thread",
      "branch_value_thread", "dec_register_peephole", "transfer_past_branch",
      "index_inc_cmp_y", "volatile_void_read_cmp", "dec_branch_indirect_y_fold",
      "mem_const_prop", "const_arith_fold", "label_offset_fold",
      "carry_recompute_drop", "carry_return_tails", "carry_const_fold",
      "carry_branch_fold", "round_trip_load_drop", "and_sign_bit_branch",
      "self_store_drop", "pointer_self_store_drop", "redundant_compare",
      "cmp_through_tmp_fold", "adc_commute", "cmp_sbc_fusion",
      "dec_inc_branch_fold", "tail_call", "tail_merge", "cross_jump",
      "via_a_store_fold", "transfer_pm1_store",
  };
  CHECK(names == expected);
}
