// The functions that the unit tests of tests/test_cfg.py build a TAC
// CFG of, each with the values of the CFG and its analyses that the
// Python functions gave, as tools/capture_helpers.py's `_cfg_values`
// writes them. They were recorded by wrapping `build_cfg` while the
// tests ran.
#include "doctest.h"
#include "tac_cfg_case.h"

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_backward_jump_creates_loop (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Label(\"top\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  JumpIfTrue(Var(\"c\") \"top\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/3/;2/3/0.2/2.3;3/1/2/1|2.3\n"
      "reverse_postorder 0.2.3.1\n"
      "immediate_dominators 0:0;2:0;3:2;1:3\n"
      "dominator_tree_children 0:2;2:3;3:1;1:\n"
      "dominance_frontiers 0:;2:2;3:;1:\n"
      "natural_loops 2:2\n");
}

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_consecutive_labels_each_start_new_block (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Label(\"A\")\n"
      "  Label(\"B\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/3/;2/1/0/3;3/2/2/1|2.3\n"
      "reverse_postorder 0.2.3.1\n"
      "immediate_dominators 0:0;2:0;3:2;1:3\n"
      "dominator_tree_children 0:2;2:3;3:1;1:\n"
      "dominance_frontiers 0:;2:;3:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_empty_function_entry_to_exit (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [])\n",
      "tac_cfg 0/0//1;1/0/0/|\n"
      "reverse_postorder 0.1\n"
      "immediate_dominators 0:0;1:0\n"
      "dominator_tree_children 0:1;1:\n"
      "dominance_frontiers 0:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_forward_jump_skips_block (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Jump(\"L1\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Label(\"L1\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/4/;2/2/0/4;3/1//4;4/2/2.3/1|2.3.4\n"
      "reverse_postorder 0.2.4.1\n"
      "immediate_dominators 0:0;2:0;4:2;1:4\n"
      "dominator_tree_children 0:2;2:4;4:1;1:\n"
      "dominance_frontiers 0:;2:;4:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_jump_if_false_two_successors (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Var(\"c\") \"end\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Label(\"end\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/4/;2/1/0/4.3;3/1/2/4;4/2/2.3/1|2.3.4\n"
      "reverse_postorder 0.2.3.4.1\n"
      "immediate_dominators 0:0;2:0;3:2;4:2;1:4\n"
      "dominator_tree_children 0:2;2:3.4;3:;4:1;1:\n"
      "dominance_frontiers 0:;2:;3:4;4:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_jump_if_true_two_successors (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Var(\"c\") \"L1\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Jump(\"L2\")\n"
      "  Label(\"L1\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Label(\"L2\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/5/;2/1/0/4.3;3/2/2/5;4/2/2/5;5/2/3.4/1|2.3.4.5\n"
      "reverse_postorder 0.2.3.4.5.1\n"
      "immediate_dominators 0:0;2:0;3:2;4:2;5:2;1:5\n"
      "dominator_tree_children 0:2;2:3.4.5;3:;4:;5:1;1:\n"
      "dominance_frontiers 0:;2:;3:5;4:5;5:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_label_starts_new_block (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Label(\"L1\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/3/;2/1/0/3;3/3/2/1|2.3\n"
      "reverse_postorder 0.2.3.1\n"
      "immediate_dominators 0:0;2:0;3:2;1:3\n"
      "dominator_tree_children 0:2;2:3;3:1;1:\n"
      "dominance_frontiers 0:;2:;3:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_mid_function_ret_starts_new_block (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/2.3/;2/1/0/1;3/2//1|2.3\n"
      "reverse_postorder 0.2.1\n"
      "immediate_dominators 0:0;2:0;1:2\n"
      "dominator_tree_children 0:2;2:1;1:\n"
      "dominance_frontiers 0:;2:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_predecessors_mirror_successors (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  JumpIfFalse(Var(\"c\") \"L2\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Jump(\"L3\")\n"
      "  Label(\"L2\")\n"
      "  Copy(Constant(ConstInt(3)) Var(\"y\"))\n"
      "  Label(\"L3\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/5/;2/2/0/4.3;3/2/2/5;4/2/2/5;5/2/3.4/1|2.3.4.5\n"
      "reverse_postorder 0.2.3.4.5.1\n"
      "immediate_dominators 0:0;2:0;3:2;4:2;5:2;1:5\n"
      "dominator_tree_children 0:2;2:3.4.5;3:;4:;5:1;1:\n"
      "dominance_frontiers 0:;2:;3:5;4:5;5:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_single_block_returns (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/2/;2/2/0/1|2\n"
      "reverse_postorder 0.2.1\n"
      "immediate_dominators 0:0;2:0;1:2\n"
      "dominator_tree_children 0:2;2:1;1:\n"
      "dominance_frontiers 0:;2:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestBuildCFG.test_trailing_block_without_terminator_falls_to_exit (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/2/;2/1/0/1|2\n"
      "reverse_postorder 0.2.1\n"
      "immediate_dominators 0:0;2:0;1:2\n"
      "dominator_tree_children 0:2;2:1;1:\n"
      "dominance_frontiers 0:;2:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestCFGToFunction.test_dropping_block_removes_its_instructions (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Jump(\"L1\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Label(\"L1\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/4/;2/2/0/4;3/1//4;4/2/2.3/1|2.3.4\n"
      "reverse_postorder 0.2.4.1\n"
      "immediate_dominators 0:0;2:0;4:2;1:4\n"
      "dominator_tree_children 0:2;2:4;4:1;1:\n"
      "dominance_frontiers 0:;2:;4:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestCFGToFunction.test_round_trip (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"foo\" true [\"p\"] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  JumpIfTrue(Var(\"c\") \"end\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Label(\"end\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/4/;2/2/0/4.3;3/1/2/4;4/2/2.3/1|2.3.4\n"
      "reverse_postorder 0.2.3.4.1\n"
      "immediate_dominators 0:0;2:0;3:2;4:2;1:4\n"
      "dominator_tree_children 0:2;2:3.4;3:;4:1;1:\n"
      "dominance_frontiers 0:;2:;3:4;4:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestDominance.test_diamond_join_in_df (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Var(\"c\") \"L_else\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"y\"))\n"
      "  Jump(\"L_join\")\n"
      "  Label(\"L_else\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Label(\"L_join\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/5/;2/1/0/4.3;3/2/2/5;4/2/2/5;5/2/3.4/1|2.3.4.5\n"
      "reverse_postorder 0.2.3.4.5.1\n"
      "immediate_dominators 0:0;2:0;3:2;4:2;5:2;1:5\n"
      "dominator_tree_children 0:2;2:3.4.5;3:;4:;5:1;1:\n"
      "dominance_frontiers 0:;2:;3:5;4:5;5:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestDominance.test_dominator_tree_children_inverts_idom (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Var(\"c\") \"L_else\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"y\"))\n"
      "  Jump(\"L_join\")\n"
      "  Label(\"L_else\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Label(\"L_join\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/5/;2/1/0/4.3;3/2/2/5;4/2/2/5;5/2/3.4/1|2.3.4.5\n"
      "reverse_postorder 0.2.3.4.5.1\n"
      "immediate_dominators 0:0;2:0;3:2;4:2;5:2;1:5\n"
      "dominator_tree_children 0:2;2:3.4.5;3:;4:;5:1;1:\n"
      "dominance_frontiers 0:;2:;3:5;4:5;5:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestDominance.test_loop_back_edge_in_df (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Label(\"head\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  JumpIfTrue(Var(\"c\") \"head\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/3/;2/3/0.2/2.3;3/1/2/1|2.3\n"
      "reverse_postorder 0.2.3.1\n"
      "immediate_dominators 0:0;2:0;3:2;1:3\n"
      "dominator_tree_children 0:2;2:3;3:1;1:\n"
      "dominance_frontiers 0:;2:2;3:;1:\n"
      "natural_loops 2:2\n");
}

TEST_CASE("tac_cfg: test_cfg.TestDominance.test_reverse_postorder_starts_at_entry (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/2/;2/2/0/1|2\n"
      "reverse_postorder 0.2.1\n"
      "immediate_dominators 0:0;2:0;1:2\n"
      "dominator_tree_children 0:2;2:1;1:\n"
      "dominance_frontiers 0:;2:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestDominance.test_straight_line_each_dominates_next (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Label(\"L1\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Label(\"L2\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/4/;2/1/0/3;3/2/2/4;4/2/3/1|2.3.4\n"
      "reverse_postorder 0.2.3.4.1\n"
      "immediate_dominators 0:0;2:0;3:2;4:3;1:4\n"
      "dominator_tree_children 0:2;2:3;3:4;4:1;1:\n"
      "dominance_frontiers 0:;2:;3:;4:;1:\n"
      "natural_loops -\n");
}

TEST_CASE("tac_cfg: test_cfg.TestDominance.test_unreachable_block_excluded_from_dominance (call 1)") {
  c6502::testing::check_tac_cfg_case(
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "])\n",
      "tac_cfg 0/0//2;1/0/2.3/;2/1/0/1;3/2//1|2.3\n"
      "reverse_postorder 0.2.1\n"
      "immediate_dominators 0:0;2:0;1:2\n"
      "dominator_tree_children 0:2;2:1;1:\n"
      "dominance_frontiers 0:;2:;1:\n"
      "natural_loops -\n");
}
