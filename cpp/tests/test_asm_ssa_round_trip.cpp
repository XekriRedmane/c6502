// The calls that the Python unit tests of the asm SSA round trip make
// of its passes, each with the function the test gives the pass, the
// call's arguments, and what the Python pass returned, which the C++
// pass must return too. They were recorded by wrapping the Python
// passes while tests/test_asm_ssa_construction.py,
// test_asm_ssa_destruction.py, test_asm_copy_propagation.py,
// test_asm_backward_copy_propagation.py, test_asm_byte_dce.py, and
// test_or_zero_absorb.py ran, and, for the packed BCD chains, the calls
// on a function with a `DecimalArith` that
// test_bcd_builtins.TestCodegen's compiles make. A test that calls a
// pass several times gives a case for each distinct call.
#include "doctest.h"
#include "stage_case.h"

using c6502::testing::check_stage_case;

TEST_CASE("to_ssa: test_asm_ssa_construction.TestAsmToSsaDiamond.test_diamond_inserts_phi_at_merge (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(1) Pseudo(\"%x\" 0) false)\n"
      "  Branch(EQ \"L1\")\n"
      "  Mov(Imm(2) Pseudo(\"%x\" 0) false)\n"
      "  Jump(\"L3\" [])\n"
      "  Label(\"L1\")\n"
      "  Mov(Imm(3) Pseudo(\"%x\" 0) false)\n"
      "  Jump(\"L3\" [])\n"
      "  Label(\"L3\")\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%x.b0.v1\" 0) false)\n"
      "  Branch(EQ \"L1\")\n"
      "  Label(\".main@asm_ssa_block@1\")\n"
      "  Mov(Imm(2) Pseudo(\"%x.b0.v2\" 0) false)\n"
      "  Jump(\"L3\" [])\n"
      "  Label(\"L1\")\n"
      "  Mov(Imm(3) Pseudo(\"%x.b0.v3\" 0) false)\n"
      "  Jump(\"L3\" [])\n"
      "  Label(\"L3\")\n"
      "  Phi(Pseudo(\"%x.b0.v4\" 0) [AsmPhiArg(\".main@asm_ssa_block@1\" Pseudo(\"%x.b0.v2\" 0)) AsmPhiArg(\"L1\" Pseudo(\"%x.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"%x.b0.v4\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_asm_ssa_construction.TestAsmToSsaExclusions.test_address_taken_name_not_renamed (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(170) Pseudo(\"%p\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%p\" 0) Pseudo(\"%addr\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(170) Pseudo(\"%p\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%p\" 0) Pseudo(\"%addr\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_asm_ssa_construction.TestAsmToSsaExclusions.test_inc_target_not_renamed (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(0) Pseudo(\"%x\" 0) false)\n"
      "  Inc(Pseudo(\"%x\" 0))\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(0) Pseudo(\"%x\" 0) false)\n"
      "  Inc(Pseudo(\"%x\" 0))\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_asm_ssa_construction.TestAsmToSsaLoop.test_loop_header_gets_phi (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(0) Pseudo(\"%i\" 0) false)\n"
      "  Label(\"L\")\n"
      "  Mov(Pseudo(\"%i\" 0) Reg(A) false)\n"
      "  Mov(Imm(1) Pseudo(\"%i\" 0) false)\n"
      "  Branch(EQ \"L\")\n"
      "  Mov(Pseudo(\"%i\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(0) Pseudo(\"%i.b0.v1\" 0) false)\n"
      "  Label(\"L\")\n"
      "  Phi(Pseudo(\"%i.b0.v2\" 0) [AsmPhiArg(\".main@asm_ssa_block@0\" Pseudo(\"%i.b0.v1\" 0)) AsmPhiArg(\"L\" Pseudo(\"%i.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"%i.b0.v2\" 0) Reg(A) false)\n"
      "  Mov(Imm(1) Pseudo(\"%i.b0.v3\" 0) false)\n"
      "  Branch(EQ \"L\")\n"
      "  Label(\".main@asm_ssa_block@1\")\n"
      "  Mov(Pseudo(\"%i.b0.v3\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_asm_ssa_construction.TestAsmToSsaSingleBlock.test_multi_byte_versions_independently (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(18) Pseudo(\"%y\" 0) false)\n"
      "  Mov(Imm(52) Pseudo(\"%y\" 1) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(18) Pseudo(\"%y.b0.v1\" 0) false)\n"
      "  Mov(Imm(52) Pseudo(\"%y.b1.v1\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_asm_ssa_construction.TestAsmToSsaSingleBlock.test_single_def_renames_to_v1 (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(1) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%x.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%x.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_asm_ssa_construction.TestAsmToSsaSingleBlock.test_two_defs_get_distinct_versions (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(1) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Mov(Imm(2) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%x.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%x.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Imm(2) Pseudo(\"%x.b0.v2\" 0) false)\n"
      "  Mov(Pseudo(\"%x.b0.v2\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("order_parallel_copies: test_asm_ssa_destruction.TestFromSsaParallelCopyOrdering.test_cycle_broken_by_temp (1)") {
  check_stage_case(
      "order_parallel_copies",
      "{\"args\": [], \"kwargs\": {\"fn_name\": \"main\", \"cycle_counter\": [0]}}",
      "[{\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%b.b0.v2\\\" 0) Pseudo(\\\"%a.b0.v2\\\" 0) false)\\n\"}, {\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%a.b0.v2\\\" 0) Pseudo(\\\"%b.b0.v2\\\" 0) false)\\n\"}]",
      "[{\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%a.b0.v2\\\" 0) Pseudo(\\\".main@asm_cycle_tmp@1\\\" 0) false)\\n\"}, {\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%b.b0.v2\\\" 0) Pseudo(\\\"%a.b0.v2\\\" 0) false)\\n\"}, {\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\".main@asm_cycle_tmp@1\\\" 0) Pseudo(\\\"%b.b0.v2\\\" 0) false)\\n\"}]",
      "{\"args\": {}, \"kwargs\": {\"cycle_counter\": [1]}}");
}

TEST_CASE("from_ssa: test_asm_ssa_destruction.TestFromSsaParallelCopyOrdering.test_cycle_broken_by_temp (2)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Label(\"L_pre\")\n"
      "  Jump(\"L_merge\" [])\n"
      "  Label(\"L_merge\")\n"
      "  Phi(Pseudo(\"%a.b0.v2\" 0) [AsmPhiArg(\"L_pre\" Pseudo(\"%b.b0.v2\" 0))])\n"
      "  Phi(Pseudo(\"%b.b0.v2\" 0) [AsmPhiArg(\"L_pre\" Pseudo(\"%a.b0.v2\" 0))])\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\"L_pre\")\n"
      "  Mov(Pseudo(\"%a.b0.v2\" 0) Pseudo(\".main@asm_cycle_tmp@1\" 0) false)\n"
      "  Mov(Pseudo(\"%b.b0.v2\" 0) Pseudo(\"%a.b0.v2\" 0) false)\n"
      "  Mov(Pseudo(\".main@asm_cycle_tmp@1\" 0) Pseudo(\"%b.b0.v2\" 0) false)\n"
      "  Jump(\"L_merge\" [])\n"
      "  Label(\"L_merge\")\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("order_parallel_copies: test_asm_ssa_destruction.TestFromSsaParallelCopyOrdering.test_lost_copy_topologically_sorted (1)") {
  check_stage_case(
      "order_parallel_copies",
      "{\"args\": [], \"kwargs\": {\"fn_name\": \"main\", \"cycle_counter\": [0]}}",
      "[{\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%a.b0.v1\\\" 0) Pseudo(\\\"%b.b0.v1\\\" 0) false)\\n\"}, {\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%b.b0.v1\\\" 0) Pseudo(\\\"%c.b0.v1\\\" 0) false)\\n\"}]",
      "[{\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%b.b0.v1\\\" 0) Pseudo(\\\"%c.b0.v1\\\" 0) false)\\n\"}, {\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%a.b0.v1\\\" 0) Pseudo(\\\"%b.b0.v1\\\" 0) false)\\n\"}]",
      "");
}

TEST_CASE("from_ssa: test_asm_ssa_destruction.TestFromSsaParallelCopyOrdering.test_lost_copy_topologically_sorted (2)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Label(\"L_pre\")\n"
      "  Jump(\"L_merge\" [])\n"
      "  Label(\"L_merge\")\n"
      "  Phi(Pseudo(\"%b.b0.v1\" 0) [AsmPhiArg(\"L_pre\" Pseudo(\"%a.b0.v1\" 0))])\n"
      "  Phi(Pseudo(\"%c.b0.v1\" 0) [AsmPhiArg(\"L_pre\" Pseudo(\"%b.b0.v1\" 0))])\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\"L_pre\")\n"
      "  Mov(Pseudo(\"%b.b0.v1\" 0) Pseudo(\"%c.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%a.b0.v1\" 0) Pseudo(\"%b.b0.v1\" 0) false)\n"
      "  Jump(\"L_merge\" [])\n"
      "  Label(\"L_merge\")\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("order_parallel_copies: test_asm_ssa_destruction.TestFromSsaSimple.test_phi_lowers_to_movs_in_predecessors (1)") {
  check_stage_case(
      "order_parallel_copies",
      "{\"args\": [], \"kwargs\": {\"fn_name\": \"main\", \"cycle_counter\": [0]}}",
      "[{\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%x.b0.v2\\\" 0) Pseudo(\\\"%x.b0.v4\\\" 0) false)\\n\"}]",
      "[{\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%x.b0.v2\\\" 0) Pseudo(\\\"%x.b0.v4\\\" 0) false)\\n\"}]",
      "");
}

TEST_CASE("order_parallel_copies: test_asm_ssa_destruction.TestFromSsaSimple.test_phi_lowers_to_movs_in_predecessors (2)") {
  check_stage_case(
      "order_parallel_copies",
      "{\"args\": [], \"kwargs\": {\"fn_name\": \"main\", \"cycle_counter\": [0]}}",
      "[{\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%x.b0.v3\\\" 0) Pseudo(\\\"%x.b0.v4\\\" 0) false)\\n\"}]",
      "[{\"asdl\": \"asm_ast\", \"dump\": \"Mov(Pseudo(\\\"%x.b0.v3\\\" 0) Pseudo(\\\"%x.b0.v4\\\" 0) false)\\n\"}]",
      "");
}

TEST_CASE("from_ssa: test_asm_ssa_destruction.TestFromSsaSimple.test_phi_lowers_to_movs_in_predecessors (3)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%x.b0.v1\" 0) false)\n"
      "  Branch(EQ \"L1\")\n"
      "  Label(\".main@asm_ssa_block@1\")\n"
      "  Mov(Imm(2) Pseudo(\"%x.b0.v2\" 0) false)\n"
      "  Jump(\"L3\" [])\n"
      "  Label(\"L1\")\n"
      "  Mov(Imm(3) Pseudo(\"%x.b0.v3\" 0) false)\n"
      "  Jump(\"L3\" [])\n"
      "  Label(\"L3\")\n"
      "  Phi(Pseudo(\"%x.b0.v4\" 0) [AsmPhiArg(\".main@asm_ssa_block@1\" Pseudo(\"%x.b0.v2\" 0)) AsmPhiArg(\"L1\" Pseudo(\"%x.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"%x.b0.v4\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%x.b0.v1\" 0) false)\n"
      "  Branch(EQ \"L1\")\n"
      "  Label(\".main@asm_ssa_block@1\")\n"
      "  Mov(Imm(2) Pseudo(\"%x.b0.v2\" 0) false)\n"
      "  Mov(Pseudo(\"%x.b0.v2\" 0) Pseudo(\"%x.b0.v4\" 0) false)\n"
      "  Jump(\"L3\" [])\n"
      "  Label(\"L1\")\n"
      "  Mov(Imm(3) Pseudo(\"%x.b0.v3\" 0) false)\n"
      "  Mov(Pseudo(\"%x.b0.v3\" 0) Pseudo(\"%x.b0.v4\" 0) false)\n"
      "  Jump(\"L3\" [])\n"
      "  Label(\"L3\")\n"
      "  Mov(Pseudo(\"%x.b0.v4\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_asm_ssa_destruction.TestFromSsaSimple.test_round_trip_simple_function_preserves_shape (1)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%x.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%x.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%x.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%x.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_address_taken_excluded_as_dst (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"%x\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"%x\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_address_taken_excluded_as_src (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  LoadAddress(Pseudo(\"%at\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%at\" 0) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  LoadAddress(Pseudo(\"%at\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%at\" 0) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_byte_granular_keys (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(18) Pseudo(\"%v\" 0) false)\n"
      "  Mov(Imm(52) Pseudo(\"%v\" 1) false)\n"
      "  Mov(Pseudo(\"%v\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%v\" 1) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(18) Pseudo(\"%v\" 0) false)\n"
      "  Mov(Imm(52) Pseudo(\"%v\" 1) false)\n"
      "  Mov(Imm(18) Reg(A) false)\n"
      "  Mov(Imm(52) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_does_not_propagate_from_data_source (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Data(\"g\" 0) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Data(\"g\" 0) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_does_not_propagate_from_frame_source (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Frame(2) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Frame(2) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_does_not_propagate_from_reg_a_source (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Pseudo(\"%y\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Pseudo(\"%y\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_does_not_propagate_from_stack_source (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Stack(1) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Stack(1) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_does_not_propagate_to_def_position (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%y\" 0) Pseudo(\"%x\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%y\" 0) Pseudo(\"%x\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_dptr_staged_pointer_excluded (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Branch(EQ \".else\")\n"
      "  Mov(ImmLabelLow(\"table_a\" 0) Pseudo(\"%p\" 0) false)\n"
      "  Mov(ImmLabelHigh(\"table_a\" 0) Pseudo(\"%p\" 1) false)\n"
      "  Jump(\".merge\" [])\n"
      "  Label(\".else\")\n"
      "  Mov(ImmLabelLow(\"table_b\" 0) Pseudo(\"%p\" 0) false)\n"
      "  Mov(ImmLabelHigh(\"table_b\" 0) Pseudo(\"%p\" 1) false)\n"
      "  Label(\".merge\")\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Branch(EQ \".else\")\n"
      "  Mov(ImmLabelLow(\"table_a\" 0) Pseudo(\"%p\" 0) false)\n"
      "  Mov(ImmLabelHigh(\"table_a\" 0) Pseudo(\"%p\" 1) false)\n"
      "  Jump(\".merge\" [])\n"
      "  Label(\".else\")\n"
      "  Mov(ImmLabelLow(\"table_b\" 0) Pseudo(\"%p\" 0) false)\n"
      "  Mov(ImmLabelHigh(\"table_b\" 0) Pseudo(\"%p\" 1) false)\n"
      "  Label(\".merge\")\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_load_address_src_never_substituted (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"%target\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%target\" 0) Pseudo(\"%p\" 0))\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"%target\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%target\" 0) Pseudo(\"%p\" 0))\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_no_changes_returns_same_function (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%x\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%x\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_phi_dst_not_a_copy (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Label(\"L\")\n"
      "  Phi(Pseudo(\"%dst\" 0) [AsmPhiArg(\"L\" Pseudo(\"%s\" 0))])\n"
      "  Mov(Pseudo(\"%dst\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\"L\")\n"
      "  Phi(Pseudo(\"%dst\" 0) [AsmPhiArg(\"L\" Pseudo(\"%s\" 0))])\n"
      "  Mov(Pseudo(\"%dst\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_propagates_imm_to_pseudo_use (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Imm(5) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_propagates_into_add_src (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(3) Pseudo(\"%x\" 0) false)\n"
      "  Add(Pseudo(\"%x\" 0) Reg(A))\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(3) Pseudo(\"%x\" 0) false)\n"
      "  Add(Imm(3) Reg(A))\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_propagates_into_compare_operands (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Imm(10) Pseudo(\"%y\" 0) false)\n"
      "  Compare(Pseudo(\"%x\" 0) Pseudo(\"%y\" 0))\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Imm(10) Pseudo(\"%y\" 0) false)\n"
      "  Compare(Imm(5) Imm(10))\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_propagates_into_phi_args (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(9) Pseudo(\"%src\" 0) false)\n"
      "  Label(\"L\")\n"
      "  Phi(Pseudo(\"%dst\" 0) [AsmPhiArg(\"L\" Pseudo(\"%src\" 0))])\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(9) Pseudo(\"%src\" 0) false)\n"
      "  Label(\"L\")\n"
      "  Phi(Pseudo(\"%dst\" 0) [AsmPhiArg(\"L\" Imm(9))])\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_propagates_pseudo_to_pseudo_use (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Pseudo(\"%src\" 0) Pseudo(\"%a\" 0) false)\n"
      "  Mov(Pseudo(\"%a\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Pseudo(\"%src\" 0) Pseudo(\"%a\" 0) false)\n"
      "  Mov(Pseudo(\"%src\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_resolves_chain (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(7) Pseudo(\"%a\" 0) false)\n"
      "  Mov(Pseudo(\"%a\" 0) Pseudo(\"%b\" 0) false)\n"
      "  Mov(Pseudo(\"%b\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(7) Pseudo(\"%a\" 0) false)\n"
      "  Mov(Imm(7) Pseudo(\"%b\" 0) false)\n"
      "  Mov(Imm(7) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_asm_copy_propagation.TestCopyPropagation.test_static_dst_excluded (1)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"g\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"g\" 0) false)\n"
      "  Mov(Pseudo(\"g\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"g\" 0) false)\n"
      "  Mov(Pseudo(\"g\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_a_live_after_pair_rejects (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Data(\"OUT\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Data(\"OUT\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_address_taken_p_rejected (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  LoadAddress(Pseudo(\"%p\" 0) Pseudo(\"%addr\" 0))\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  LoadAddress(Pseudo(\"%p\" 0) Pseudo(\"%addr\" 0))\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_aliased_read_in_region_rejects (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_aliased_write_in_region_rejects (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Imm(0) Data(\"HARGS\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Imm(0) Data(\"HARGS\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_call_in_region_rejects (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Call(\"some_helper\" [] [])\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Call(\"some_helper\" [] [])\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_def_must_be_canonical_form (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(66) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(66) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_flags_live_after_pair_rejects (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Branch(EQ \"some_label\")\n"
      "  Label(\"some_label\")\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Branch(EQ \"some_label\")\n"
      "  Label(\"some_label\")\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_label_in_region_rejects (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Label(\"L\")\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Label(\"L\")\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_memory_to_memory_copy (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Data(\"__zpabi_f__p_0\" 0) false)\n"
      "  Call(\"f\" [] [])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Data(\"__zpabi_f__p_0\" 0) false)\n"
      "  Call(\"f\" [] [])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_memory_to_memory_copy_keeps_a_when_read (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Imm(3) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Data(\"g\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Imm(3) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Data(\"g\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_multi_use_p_rejects (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 1) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 1) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_no_candidates_returns_unchanged (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(0) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(0) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_not_across_allocate_stack (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  AllocateStack(1)\n"
      "  Mov(Pseudo(\"%p\" 0) Stack(1) false)\n"
      "  Call(\"f\" [] [])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  AllocateStack(1)\n"
      "  Mov(Pseudo(\"%p\" 0) Stack(1) false)\n"
      "  Call(\"f\" [] [])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_pseudo_d_rejected (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%q\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%q\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_relocation_into_stack_dst (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Stack(1) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Stack(1) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_single_byte_pattern_to_data (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"%p\" 0) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_static_p_rejected (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"g\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"g\" 0) false)\n"
      "  Mov(Pseudo(\"g\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Reg(A) Pseudo(\"g\" 0) false)\n"
      "  Mov(Pseudo(\"g\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_asm_backward_copy_propagation.TestBackwardCopyPropagation.test_user_two_byte_pattern (1)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Pseudo(\"@x\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Pseudo(\"@y\" 0) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%t.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@x\" 1) Reg(A) false)\n"
      "  Add(Pseudo(\"@y\" 1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%t.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%t.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Mov(Pseudo(\"%t.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"HARGS\" 1) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Pseudo(\"@x\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Pseudo(\"@y\" 0) Reg(A))\n"
      "  Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "  Mov(Pseudo(\"@x\" 1) Reg(A) false)\n"
      "  Add(Pseudo(\"@y\" 1) Reg(A))\n"
      "  Mov(Reg(A) Data(\"HARGS\" 1) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_asm_byte_dce.TestByteDce.test_byte_granular_dead_high_byte (1)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(18) Pseudo(\"%y\" 0) false)\n"
      "  Mov(Imm(52) Pseudo(\"%y\" 1) false)\n"
      "  Mov(Pseudo(\"%y\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(18) Pseudo(\"%y\" 0) false)\n"
      "  Mov(Pseudo(\"%y\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_asm_byte_dce.TestByteDce.test_cascading_dead_chain (1)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(1) Pseudo(\"%a\" 0) false)\n"
      "  Mov(Pseudo(\"%a\" 0) Pseudo(\"%b\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_asm_byte_dce.TestByteDce.test_drops_mov_with_unused_pseudo_dst (1)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(1) Pseudo(\"%dead\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_asm_byte_dce.TestByteDce.test_drops_phi_with_unused_dst (1)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Label(\"L\")\n"
      "  Phi(Pseudo(\"%dead\" 0) [AsmPhiArg(\"L\" Pseudo(\"%src\" 0))])\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\"L\")\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_asm_byte_dce.TestByteDce.test_keeps_mov_to_register (1)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_asm_byte_dce.TestByteDce.test_keeps_mov_with_live_pseudo_dst (1)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(1) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(1) Pseudo(\"%x\" 0) false)\n"
      "  Mov(Pseudo(\"%x\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_asm_byte_dce.TestByteDce.test_loadaddress_preserved_even_when_unused (1)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  LoadAddress(Pseudo(\"%target\" 0) Pseudo(\"%dead\" 0))\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  LoadAddress(Pseudo(\"%target\" 0) Pseudo(\"%dead\" 0))\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_asm_byte_dce.TestByteDce.test_static_write_preserved_even_when_unread_locally (1)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"g\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"g\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"g\" 0) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_or_zero_absorb.TestAbsorbZeroLoad.test_absorbs_data_source (1)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(0) Reg(A) false)\n"
      "  Or(Data(\"x\" 0) Reg(A))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Mov(Data(\"x\" 0) Reg(A) false)\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_or_zero_absorb.TestAbsorbZeroLoad.test_absorbs_pseudo_source (1)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(0) Reg(A) false)\n"
      "  Or(Pseudo(\"%t\" 0) Reg(A))\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"%t\" 0) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_or_zero_absorb.TestAbsorbZeroLoad.test_non_adjacent_not_absorbed (1)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Or(Data(\"x\" 0) Reg(A))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Or(Data(\"x\" 0) Reg(A))\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_or_zero_absorb.TestAbsorbZeroLoad.test_non_zero_immediate_not_absorbed (1)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(1) Reg(A) false)\n"
      "  Or(Data(\"x\" 0) Reg(A))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(1) Reg(A) false)\n"
      "  Or(Data(\"x\" 0) Reg(A))\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_or_zero_absorb.TestAbsorbZeroLoad.test_or_to_non_a_register_not_absorbed (1)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(0) Reg(A) false)\n"
      "  Or(Data(\"x\" 0) Reg(X))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(0) Reg(A) false)\n"
      "  Or(Data(\"x\" 0) Reg(X))\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_or_zero_absorb.TestAbsorbZeroLoad.test_volatile_mov_not_absorbed (1)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(0) Reg(A) true)\n"
      "  Or(Data(\"x\" 0) Reg(A))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(0) Reg(A) true)\n"
      "  Or(Data(\"x\" 0) Reg(A))\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_bcd_builtins.TestCodegen.test_byte_result_in_a (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"a\", \"b\", \"f\"]}}}",
      "Function(\"f\" true [] [\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Pseudo(\"%0.1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_byte_result_in_a (2)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"a\", \"b\", \"f\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_byte_result_in_a (3)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"a\", \"b\", \"f\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_byte_result_in_a (4)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"a\", \"b\", \"f\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_bcd_builtins.TestCodegen.test_byte_result_in_a (5)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_byte_result_in_a (6)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [ZP(128 0)])\n"
      "  Mov(ZP(128 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [ZP(128 0)])\n"
      "  Mov(ZP(128 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_byte_result_in_a (7)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Data(\"__local_f__0\" 0)])\n"
      "  Mov(Data(\"__local_f__0\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"a\" 0)] [Pseudo(\"b\" 0)] [Data(\"__local_f__0\" 0)])\n"
      "  Mov(Data(\"__local_f__0\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_bcd_builtins.TestCodegen.test_decimal_flag_only_around_the_chain (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"main\", \"p\", \"t\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Mov(Imm(0) Pseudo(\"@0.i.2\" 0) false)\n"
      "  Label(\".loop@0_start\")\n"
      "  Mov(Pseudo(\"@0.i.2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Mov(Pseudo(\"@0.i.2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1\" 1) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1\" 0) Pseudo(\"%9.1\" 1)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1\" 1) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1\" 1) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1\" 0) Pseudo(\"%17.1\" 1)] [Pseudo(\"%18.1\" 0) Pseudo(\"%18.1\" 1)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1\" 1) false)\n"
      "  Mov(Pseudo(\"%21.1\" 1) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Mov(Imm(0) Pseudo(\"@0.i.2.b0.v1\" 0) false)\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Pseudo(\"@0.i.2.b0.v1\" 0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_decimal_flag_only_around_the_chain (2)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"main\", \"p\", \"t\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Mov(Imm(0) Pseudo(\"@0.i.2.b0.v1\" 0) false)\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Pseudo(\"@0.i.2.b0.v1\" 0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Mov(Imm(0) Pseudo(\"@0.i.2.b0.v1\" 0) false)\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_decimal_flag_only_around_the_chain (3)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"main\", \"p\", \"t\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Mov(Imm(0) Pseudo(\"@0.i.2.b0.v1\" 0) false)\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Mov(Imm(0) Pseudo(\"@0.i.2.b0.v1\" 0) false)\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_decimal_flag_only_around_the_chain (4)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"main\", \"p\", \"t\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Mov(Imm(0) Pseudo(\"@0.i.2.b0.v1\" 0) false)\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_bcd_builtins.TestCodegen.test_decimal_flag_only_around_the_chain (5)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_decimal_flag_only_around_the_chain (6)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"main\", \"p\", \"t\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_decimal_flag_only_around_the_chain (7)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"main\", \"p\", \"t\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_decimal_flag_only_around_the_chain (8)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"main\", \"p\", \"t\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Pseudo(\"@0.i.2.b0.v2\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Pseudo(\"@0.i.2.b0.v3\" 0))])\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"%8.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%8.1.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Pseudo(\"%9.1.b0.v1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Pseudo(\"%9.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%9.1.b0.v1\" 0) Pseudo(\"%9.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Pseudo(\"@0.i.2.b0.v2\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Pseudo(\"@0.i.2.b0.v3\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Pseudo(\"%17.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Pseudo(\"%17.1.b1.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Pseudo(\"%18.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Pseudo(\"%18.1.b1.v1\" 0) false)\n"
      "  DecimalArith(true [Pseudo(\"%17.1.b0.v1\" 0) Pseudo(\"%17.1.b1.v1\" 0)] [Pseudo(\"%18.1.b0.v1\" 0) Pseudo(\"%18.1.b1.v1\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Pseudo(\"%21.1.b0.v1\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Pseudo(\"%21.1.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%21.1.b1.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"%21.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_decimal_flag_only_around_the_chain (9)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(ZP(130 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" ZP(130 0))])\n"
      "  Mov(ZP(130 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(ZP(130 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) ZP(129 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) ZP(128 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [ZP(129 0) ZP(128 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(ZP(130 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) ZP(130 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) ZP(131 0) false)\n"
      "  Mov(Data(\"p\" 2) ZP(130 0) false)\n"
      "  Mov(Data(\"t\" 4) ZP(129 0) false)\n"
      "  Mov(Data(\"t\" 5) ZP(128 0) false)\n"
      "  DecimalArith(true [ZP(131 0) ZP(130 0)] [ZP(129 0) ZP(128 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) ZP(128 0) false)\n"
      "  Mov(Data(\"t\" 3) Reg(X) false)\n"
      "  Mov(ZP(128 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Mov(Imm(0) ZP(130 0) false)\n"
      "  Label(\".loop@0_start\")\n"
      "  Mov(ZP(130 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(ZP(130 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) ZP(129 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) ZP(128 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [ZP(129 0) ZP(128 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(ZP(130 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) ZP(130 0) false)\n"
      "  Mov(ZP(130 0) ZP(130 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) ZP(131 0) false)\n"
      "  Mov(Data(\"p\" 2) ZP(130 0) false)\n"
      "  Mov(Data(\"t\" 4) ZP(129 0) false)\n"
      "  Mov(Data(\"t\" 5) ZP(128 0) false)\n"
      "  DecimalArith(true [ZP(131 0) ZP(130 0)] [ZP(129 0) ZP(128 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) ZP(128 0) false)\n"
      "  Mov(Data(\"t\" 3) Reg(X) false)\n"
      "  Mov(ZP(128 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_decimal_flag_only_around_the_chain (10)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Data(\"__local_main__i\" 0) [AsmPhiArg(\".main@ssa_block@0\" Imm(0)) AsmPhiArg(\".loop@0_continue\" Data(\"__local_main__i\" 0))])\n"
      "  Mov(Data(\"__local_main__i\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Data(\"__local_main__i\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Data(\"__local_main__1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Data(\"__local_main__0\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"__local_main__1\" 0) Data(\"__local_main__0\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Data(\"__local_main__i\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Data(\"__local_main__i\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Data(\"__local_main__2\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Data(\"__local_main__i\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Data(\"__local_main__1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Data(\"__local_main__0\" 0) false)\n"
      "  DecimalArith(true [Data(\"__local_main__2\" 0) Data(\"__local_main__i\" 0)] [Data(\"__local_main__1\" 0) Data(\"__local_main__0\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Data(\"__local_main__0\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Reg(X) false)\n"
      "  Mov(Data(\"__local_main__0\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_preheader@0\")\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Mov(Imm(0) Data(\"__local_main__i\" 0) false)\n"
      "  Label(\".loop@0_start\")\n"
      "  Mov(Data(\"__local_main__i\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(CS \".loop@0_break\")\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Data(\"__local_main__i\" 0) Reg(A) false)\n"
      "  ArithmeticShiftLeft(Reg(A))\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"t\" 0 X) Data(\"__local_main__1\" 0) false)\n"
      "  Mov(IndexedData(\"t\" 1 X) Data(\"__local_main__0\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"__local_main__1\" 0) Data(\"__local_main__0\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Mov(Data(\"__local_main__i\" 0) Reg(A) false)\n"
      "  ClearCarry\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Data(\"__local_main__i\" 0) false)\n"
      "  Mov(Data(\"__local_main__i\" 0) Data(\"__local_main__i\" 0) false)\n"
      "  Jump(\".loop@0_start\" [])\n"
      "  Label(\".loop@0_break\")\n"
      "  Mov(Data(\"p\" 1) Data(\"__local_main__2\" 0) false)\n"
      "  Mov(Data(\"p\" 2) Data(\"__local_main__i\" 0) false)\n"
      "  Mov(Data(\"t\" 4) Data(\"__local_main__1\" 0) false)\n"
      "  Mov(Data(\"t\" 5) Data(\"__local_main__0\" 0) false)\n"
      "  DecimalArith(true [Data(\"__local_main__2\" 0) Data(\"__local_main__i\" 0)] [Data(\"__local_main__1\" 0) Data(\"__local_main__0\" 0)] [Data(\"t\" 2) Data(\"t\" 3)])\n"
      "  Mov(Data(\"t\" 2) Data(\"__local_main__0\" 0) false)\n"
      "  Mov(Data(\"t\" 3) Reg(X) false)\n"
      "  Mov(Data(\"__local_main__0\" 0) Reg(A) false)\n"
      "  Return(true true false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_bcd_builtins.TestCodegen.test_member_in_place (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg\" 0) false)\n"
      "  Mov(Pseudo(\"@0.x.reg\" 0) Pseudo(\"%3.1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%3.1\" 1) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%3.1\" 0) Pseudo(\"%3.1\" 1)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.x.reg.b0.v1\" 0) Pseudo(\"%3.1.b0.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%3.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%3.1.b0.v1\" 0) Pseudo(\"%3.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_member_in_place (2)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.x.reg.b0.v1\" 0) Pseudo(\"%3.1.b0.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%3.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"%3.1.b0.v1\" 0) Pseudo(\"%3.1.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.x.reg.b0.v1\" 0) Pseudo(\"%3.1.b0.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%3.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_member_in_place (3)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.x.reg.b0.v1\" 0) Pseudo(\"%3.1.b0.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%3.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.x.reg.b0.v1\" 0) Pseudo(\"%3.1.b0.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%3.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_member_in_place (4)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.x.reg.b0.v1\" 0) Pseudo(\"%3.1.b0.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%3.1.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_bcd_builtins.TestCodegen.test_member_in_place (5)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_member_in_place (6)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_member_in_place (7)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_member_in_place (8)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.x.reg.b0.v1\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Pseudo(\"@0.x.reg.b0.v1\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_member_in_place (9)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) ZP(128 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [ZP(128 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) ZP(128 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [ZP(128 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_bcd_builtins.TestCodegen.test_member_in_place (10)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Mov(Reg(A) Pseudo(\"@1.x.reg\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@1.x.reg\" 1) false)\n"
      "  DecimalArith(false [Pseudo(\"@1.x.reg\" 0) Pseudo(\"@1.x.reg\" 1)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@1.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@1.x.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"@1.x.reg.b0.v1\" 0) Pseudo(\"@1.x.reg.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_member_in_place (11)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@1.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@1.x.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"@1.x.reg.b0.v1\" 0) Pseudo(\"@1.x.reg.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@1.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@1.x.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"@1.x.reg.b0.v1\" 0) Pseudo(\"@1.x.reg.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_member_in_place (12)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@1.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@1.x.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"@1.x.reg.b0.v1\" 0) Pseudo(\"@1.x.reg.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@1.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@1.x.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"@1.x.reg.b0.v1\" 0) Pseudo(\"@1.x.reg.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_member_in_place (13)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"add2\", \"p\"]}}}",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@1.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@1.x.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"@1.x.reg.b0.v1\" 0) Pseudo(\"@1.x.reg.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@1.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@1.x.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"@1.x.reg.b0.v1\" 0) Pseudo(\"@1.x.reg.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_bcd_builtins.TestCodegen.test_member_in_place (14)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@1.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@1.x.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"@1.x.reg.b0.v1\" 0) Pseudo(\"@1.x.reg.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@1.x.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@1.x.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"@1.x.reg.b0.v1\" 0) Pseudo(\"@1.x.reg.b1.v1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_member_in_place (15)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) ZP(129 0) false)\n"
      "  Mov(Reg(X) ZP(128 0) false)\n"
      "  DecimalArith(false [ZP(129 0) ZP(128 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) ZP(129 0) false)\n"
      "  Mov(Reg(X) ZP(128 0) false)\n"
      "  DecimalArith(false [ZP(129 0) ZP(128 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_member_in_place (16)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Data(\"__local_add__x\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"__local_add__x\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.x\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Data(\"__local_add__x\" 0) false)\n"
      "  DecimalArith(false [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"__local_add__x\" 0) Imm(0)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_member_in_place (17)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Data(\"__local_add2__x_0\" 0) false)\n"
      "  Mov(Reg(X) Data(\"__local_add2__x_1\" 0) false)\n"
      "  DecimalArith(false [Data(\"__local_add2__x_0\" 0) Data(\"__local_add2__x_1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add2\" true [\"@1.x\"] [\n"
      "  Label(\".add2@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Data(\"__local_add2__x_0\" 0) false)\n"
      "  Mov(Reg(X) Data(\"__local_add2__x_1\" 0) false)\n"
      "  DecimalArith(false [Data(\"__local_add2__x_0\" 0) Data(\"__local_add2__x_1\" 0)] [Data(\"p\" 1) Data(\"p\" 2)] [Data(\"p\" 1) Data(\"p\" 2)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_bcd_builtins.TestCodegen.test_score (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"score\"]}}}",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg\" 1) false)\n"
      "  Mov(Pseudo(\"@0.points.reg\" 0) Pseudo(\"%0.1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg\" 1) Pseudo(\"%0.1\" 1) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1\" 2) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1\" 3) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"%0.1\" 0) Pseudo(\"%0.1\" 1) Pseudo(\"%0.1\" 2) Pseudo(\"%0.1\" 3)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"%0.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b1.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b2.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b3.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) Pseudo(\"%0.1.b2.v1\" 0) Pseudo(\"%0.1.b3.v1\" 0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_score (2)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"score\"]}}}",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"%0.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b1.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b2.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b3.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) Pseudo(\"%0.1.b2.v1\" 0) Pseudo(\"%0.1.b3.v1\" 0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"%0.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b1.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b2.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b3.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_score (3)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"score\"]}}}",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"%0.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b1.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b2.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b3.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"%0.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b1.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b2.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b3.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_score (4)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"score\"]}}}",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"%0.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"@0.points.reg.b1.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b2.v1\" 0) false)\n"
      "  Mov(Imm(0) Pseudo(\"%0.1.b3.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_bcd_builtins.TestCodegen.test_score (5)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_score (6)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"score\"]}}}",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_score (7)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"score\"]}}}",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_score (8)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"add\", \"score\"]}}}",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Pseudo(\"@0.points.reg.b0.v1\" 0) false)\n"
      "  Mov(Reg(X) Pseudo(\"@0.points.reg.b1.v1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Pseudo(\"@0.points.reg.b0.v1\" 0) Pseudo(\"@0.points.reg.b1.v1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_score (9)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) ZP(129 0) false)\n"
      "  Mov(Reg(X) ZP(128 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [ZP(129 0) ZP(128 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) ZP(129 0) false)\n"
      "  Mov(Reg(X) ZP(128 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [ZP(129 0) ZP(128 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_score (10)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Data(\"__local_add__points_0\" 0) false)\n"
      "  Mov(Reg(X) Data(\"__local_add__points_1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Data(\"__local_add__points_0\" 0) Data(\"__local_add__points_1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"add\" true [\"@0.points\"] [\n"
      "  Label(\".add@asm_ssa_block@0\")\n"
      "  Mov(Reg(A) Data(\"__local_add__points_0\" 0) false)\n"
      "  Mov(Reg(X) Data(\"__local_add__points_1\" 0) false)\n"
      "  DecimalArith(false [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)] [Data(\"__local_add__points_0\" 0) Data(\"__local_add__points_1\" 0) Imm(0) Imm(0)] [Pseudo(\"score\" 0) Pseudo(\"score\" 1) Pseudo(\"score\" 2) Pseudo(\"score\" 3)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_bcd_builtins.TestCodegen.test_subtract_a_constant (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"dec\", \"w\"]}}}",
      "Function(\"dec\" true [] [\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_subtract_a_constant (2)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"dec\", \"w\"]}}}",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_subtract_a_constant (3)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"dec\", \"w\"]}}}",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_subtract_a_constant (4)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"dec\", \"w\"]}}}",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_bcd_builtins.TestCodegen.test_subtract_a_constant (5)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_subtract_a_constant (6)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"dec\" true [] [\n"
      "  Label(\".dec@asm_ssa_block@0\")\n"
      "  DecimalArith(true [Pseudo(\"w\" 0) Pseudo(\"w\" 1)] [Imm(1) Imm(0)] [Pseudo(\"w\" 0) Pseudo(\"w\" 1)])\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("to_ssa: test_bcd_builtins.TestCodegen.test_unused_top_bytes_go (1)") {
  check_stage_case(
      "to_ssa",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"low\", \"s\"]}}}",
      "Function(\"low\" true [] [\n"
      "  DecimalArith(false [Pseudo(\"s\" 0) Pseudo(\"s\" 1) Pseudo(\"s\" 2) Pseudo(\"s\" 3)] [Imm(37) Imm(0) Imm(0) Imm(0)] [Pseudo(\"%0.1\" 0) Pseudo(\"%0.1\" 1) Pseudo(\"%0.1\" 2) Pseudo(\"%0.1\" 3)])\n"
      "  Mov(Pseudo(\"%0.1\" 0) Pseudo(\"%1.1\" 0) false)\n"
      "  Mov(Pseudo(\"%1.1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0) Pseudo(\"s\" 1) Pseudo(\"s\" 2) Pseudo(\"s\" 3)] [Imm(37) Imm(0) Imm(0) Imm(0)] [Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) Pseudo(\"%0.1.b2.v1\" 0) Pseudo(\"%0.1.b3.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%1.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%1.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_unused_top_bytes_go (2)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"low\", \"s\"]}}}",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0) Pseudo(\"s\" 1) Pseudo(\"s\" 2) Pseudo(\"s\" 3)] [Imm(37) Imm(0) Imm(0) Imm(0)] [Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) Pseudo(\"%0.1.b2.v1\" 0) Pseudo(\"%0.1.b3.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%1.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%1.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0) Pseudo(\"s\" 1) Pseudo(\"s\" 2) Pseudo(\"s\" 3)] [Imm(37) Imm(0) Imm(0) Imm(0)] [Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) Pseudo(\"%0.1.b2.v1\" 0) Pseudo(\"%0.1.b3.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%1.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_unused_top_bytes_go (3)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"low\", \"s\"]}}}",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0) Pseudo(\"s\" 1) Pseudo(\"s\" 2) Pseudo(\"s\" 3)] [Imm(37) Imm(0) Imm(0) Imm(0)] [Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) Pseudo(\"%0.1.b2.v1\" 0) Pseudo(\"%0.1.b3.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%1.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0) Pseudo(\"s\" 1) Pseudo(\"s\" 2) Pseudo(\"s\" 3)] [Imm(37) Imm(0) Imm(0) Imm(0)] [Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) Pseudo(\"%0.1.b2.v1\" 0) Pseudo(\"%0.1.b3.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%1.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_unused_top_bytes_go (4)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"low\", \"s\"]}}}",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0) Pseudo(\"s\" 1) Pseudo(\"s\" 2) Pseudo(\"s\" 3)] [Imm(37) Imm(0) Imm(0) Imm(0)] [Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%0.1.b1.v1\" 0) Pseudo(\"%0.1.b2.v1\" 0) Pseudo(\"%0.1.b3.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Pseudo(\"%1.1.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("absorb_zero_load: test_bcd_builtins.TestCodegen.test_unused_top_bytes_go (5)") {
  check_stage_case(
      "absorb_zero_load",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("copy_propagate: test_bcd_builtins.TestCodegen.test_unused_top_bytes_go (6)") {
  check_stage_case(
      "copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"low\", \"s\"]}}}",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("backward_copy_propagate: test_bcd_builtins.TestCodegen.test_unused_top_bytes_go (7)") {
  check_stage_case(
      "backward_copy_propagate",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"low\", \"s\"]}}}",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("byte_dce: test_bcd_builtins.TestCodegen.test_unused_top_bytes_go (8)") {
  check_stage_case(
      "byte_dce",
      "{\"args\": [], \"kwargs\": {\"statics\": {\"set\": [\"low\", \"s\"]}}}",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Pseudo(\"%0.1.b0.v1\" 0)])\n"
      "  Mov(Pseudo(\"%0.1.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_unused_top_bytes_go (9)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [ZP(128 0)])\n"
      "  Mov(ZP(128 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [ZP(128 0)])\n"
      "  Mov(ZP(128 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: test_bcd_builtins.TestCodegen.test_unused_top_bytes_go (10)") {
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Data(\"__local_low__0\" 0)])\n"
      "  Mov(Data(\"__local_low__0\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "Function(\"low\" true [] [\n"
      "  Label(\".low@asm_ssa_block@0\")\n"
      "  DecimalArith(false [Pseudo(\"s\" 0)] [Imm(37)] [Data(\"__local_low__0\" 0)])\n"
      "  Mov(Data(\"__local_low__0\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa: both edges of a branch to the label after it get the move") {
  // `.p` ends in a branch to `.m`, the label right after it, so the Phi
  // at `.m` has an argument from `.p` for each edge. The split block of
  // each edge copies v1 into the Phi's destination, where the
  // fall-through's split block had no move. The Python test is
  // TestBranchToNextLabel in tests/test_asm_ssa_destruction.py.
  check_stage_case(
      "from_ssa",
      "{\"args\": [], \"kwargs\": {}}",
      "Function(\"f\" false [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"v.b0.v1\" 0) false)\n"
      "  Mov(Data(\"flag\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(0))\n"
      "  Branch(EQ \".other\")\n"
      "  Label(\".p\")\n"
      "  Mov(Data(\"flag\" 1) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(EQ \".m\")\n"
      "  Label(\".m\")\n"
      "  Phi(Pseudo(\"v.b0.v3\" 0) [AsmPhiArg(\".p\" Pseudo(\"v.b0.v1\" 0)) AsmPhiArg(\".p\" Pseudo(\"v.b0.v1\" 0)) AsmPhiArg(\".other\" Pseudo(\"v.b0.v2\" 0))])\n"
      "  Mov(Pseudo(\"v.b0.v3\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Return(false false false)\n"
      "  Label(\".other\")\n"
      "  Mov(Imm(2) Pseudo(\"v.b0.v2\" 0) false)\n"
      "  Jump(\".m\" [])\n"
      "])\n",
      "Function(\"f\" false [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"v.b0.v1\" 0) false)\n"
      "  Mov(Data(\"flag\" 0) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(0))\n"
      "  Branch(EQ \".other\")\n"
      "  Label(\".p\")\n"
      "  Mov(Data(\"flag\" 1) Reg(A) false)\n"
      "  Compare(Reg(A) Imm(3))\n"
      "  Branch(EQ \".f@asm_ssa_split@0\")\n"
      "  Label(\".f@asm_ssa_split@1\")\n"
      "  Mov(Pseudo(\"v.b0.v1\" 0) Pseudo(\"v.b0.v3\" 0) false)\n"
      "  Label(\".m\")\n"
      "  Mov(Pseudo(\"v.b0.v3\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Return(false false false)\n"
      "  Label(\".other\")\n"
      "  Mov(Imm(2) Pseudo(\"v.b0.v2\" 0) false)\n"
      "  Mov(Pseudo(\"v.b0.v2\" 0) Pseudo(\"v.b0.v3\" 0) false)\n"
      "  Jump(\".m\" [])\n"
      "  Label(\".f@asm_ssa_split@0\")\n"
      "  Mov(Pseudo(\"v.b0.v1\" 0) Pseudo(\"v.b0.v3\" 0) false)\n"
      "  Jump(\".m\" [])\n"
      "])\n",
      "");
}
