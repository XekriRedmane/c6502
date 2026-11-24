// The calls that the tests of tests/test_asm_regalloc.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("to_ssa: test_asm_regalloc.py::TestAsmRegalloc::test_disjoint_lifetimes_share_color (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass to_ssa\n"
      "source tests/test_asm_regalloc.py::TestAsmRegalloc::test_disjoint_lifetimes_share_color\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(1) Pseudo(\"%a\" 0) false)\n"
      "  Mov(Pseudo(\"%a\" 0) Reg(A) false)\n"
      "  Mov(Imm(2) Pseudo(\"%b\" 0) false)\n"
      "  Mov(Pseudo(\"%b\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%a.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%a.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Imm(2) Pseudo(\"%b.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%b.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n");
}

TEST_CASE("compute_liveness: test_asm_regalloc.py::TestAsmRegalloc::test_disjoint_lifetimes_share_color (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass compute_liveness\n"
      "source tests/test_asm_regalloc.py::TestAsmRegalloc::test_disjoint_lifetimes_share_color\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%a.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%a.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Imm(2) Pseudo(\"%b.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%b.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 6, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}, \"live_out\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}\n");
}

TEST_CASE("build_interference: test_asm_regalloc.py::TestAsmRegalloc::test_disjoint_lifetimes_share_color (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass build_interference\n"
      "source tests/test_asm_regalloc.py::TestAsmRegalloc::test_disjoint_lifetimes_share_color\n"
      "args {\"args\": [{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 6, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}, \"live_out\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%a.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%a.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Imm(2) Pseudo(\"%b.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%b.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"%a.b0.v1\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%a.b0.v1\", \"width\": 1, \"lives_across_call\": false}}], [\"%b.b0.v1\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%b.b0.v1\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}\n");
}

TEST_CASE("color_graph: test_asm_regalloc.py::TestAsmRegalloc::test_disjoint_lifetimes_share_color (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass color_graph\n"
      "source tests/test_asm_regalloc.py::TestAsmRegalloc::test_disjoint_lifetimes_share_color\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"%a.b0.v1\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%a.b0.v1\", \"width\": 1, \"lives_across_call\": false}}], [\"%b.b0.v1\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%b.b0.v1\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%a.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%a.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Imm(2) Pseudo(\"%b.b0.v1\" 0) false)\n"
      "  Mov(Pseudo(\"%b.b0.v1\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%b.b0.v1\", 128], [\"%a.b0.v1\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}\n");
}

TEST_CASE("to_ssa: test_asm_regalloc.py::TestAsmRegalloc::test_overlapping_lifetimes_get_different_colors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass to_ssa\n"
      "source tests/test_asm_regalloc.py::TestAsmRegalloc::test_overlapping_lifetimes_get_different_colors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(1) Pseudo(\"%a\" 0) false)\n"
      "  Mov(Imm(2) Pseudo(\"%b\" 0) false)\n"
      "  Compare(Pseudo(\"%a\" 0) Pseudo(\"%b\" 0))\n"
      "  Return(true false false)\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%a.b0.v1\" 0) false)\n"
      "  Mov(Imm(2) Pseudo(\"%b.b0.v1\" 0) false)\n"
      "  Compare(Pseudo(\"%a.b0.v1\" 0) Pseudo(\"%b.b0.v1\" 0))\n"
      "  Return(true false false)\n"
      "])\n");
}

TEST_CASE("compute_liveness: test_asm_regalloc.py::TestAsmRegalloc::test_overlapping_lifetimes_get_different_colors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass compute_liveness\n"
      "source tests/test_asm_regalloc.py::TestAsmRegalloc::test_overlapping_lifetimes_get_different_colors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%a.b0.v1\" 0) false)\n"
      "  Mov(Imm(2) Pseudo(\"%b.b0.v1\" 0) false)\n"
      "  Compare(Pseudo(\"%a.b0.v1\" 0) Pseudo(\"%b.b0.v1\" 0))\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 5, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}, \"live_out\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}\n");
}

TEST_CASE("build_interference: test_asm_regalloc.py::TestAsmRegalloc::test_overlapping_lifetimes_get_different_colors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass build_interference\n"
      "source tests/test_asm_regalloc.py::TestAsmRegalloc::test_overlapping_lifetimes_get_different_colors\n"
      "args {\"args\": [{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 5, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}, \"live_out\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%a.b0.v1\" 0) false)\n"
      "  Mov(Imm(2) Pseudo(\"%b.b0.v1\" 0) false)\n"
      "  Compare(Pseudo(\"%a.b0.v1\" 0) Pseudo(\"%b.b0.v1\" 0))\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"%a.b0.v1\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%a.b0.v1\", \"width\": 1, \"lives_across_call\": false}}], [\"%b.b0.v1\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%b.b0.v1\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"%a.b0.v1\", {\"set\": [\"%b.b0.v1\"]}], [\"%b.b0.v1\", {\"set\": [\"%a.b0.v1\"]}]]}}}\n");
}

TEST_CASE("color_graph: test_asm_regalloc.py::TestAsmRegalloc::test_overlapping_lifetimes_get_different_colors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass color_graph\n"
      "source tests/test_asm_regalloc.py::TestAsmRegalloc::test_overlapping_lifetimes_get_different_colors\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"%a.b0.v1\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%a.b0.v1\", \"width\": 1, \"lives_across_call\": false}}], [\"%b.b0.v1\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%b.b0.v1\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"%a.b0.v1\", {\"set\": [\"%b.b0.v1\"]}], [\"%b.b0.v1\", {\"set\": [\"%a.b0.v1\"]}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(1) Pseudo(\"%a.b0.v1\" 0) false)\n"
      "  Mov(Imm(2) Pseudo(\"%b.b0.v1\" 0) false)\n"
      "  Compare(Pseudo(\"%a.b0.v1\" 0) Pseudo(\"%b.b0.v1\" 0))\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%b.b0.v1\", 128], [\"%a.b0.v1\", 129]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}\n");
}

TEST_CASE("compute_liveness: test_asm_regalloc.py::TestFrameAccessWritesY::test_spilled_value_counts_as_frame_access (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass compute_liveness\n"
      "source tests/test_asm_regalloc.py::TestFrameAccessWritesY::test_spilled_value_counts_as_frame_access\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(0) Pseudo(\"i\" 0) false)\n"
      "  Mov(Pseudo(\"i\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"t\" 0 Y) Reg(A) false)\n"
      "  Mov(Imm(1) Pseudo(\"a\" 0) false)\n"
      "  Mov(Imm(2) Pseudo(\"b\" 0) false)\n"
      "  Mov(Imm(3) Pseudo(\"c\" 0) false)\n"
      "  Compare(Pseudo(\"a\" 0) Pseudo(\"b\" 0))\n"
      "  Compare(Pseudo(\"c\" 0) Imm(0))\n"
      "  Mov(Pseudo(\"i\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"t\" 0 Y) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 13, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}, \"live_out\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}\n");
}

TEST_CASE("build_interference: test_asm_regalloc.py::TestFrameAccessWritesY::test_spilled_value_counts_as_frame_access (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass build_interference\n"
      "source tests/test_asm_regalloc.py::TestFrameAccessWritesY::test_spilled_value_counts_as_frame_access\n"
      "args {\"args\": [{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 13, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}, \"live_out\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(0) Pseudo(\"i\" 0) false)\n"
      "  Mov(Pseudo(\"i\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"t\" 0 Y) Reg(A) false)\n"
      "  Mov(Imm(1) Pseudo(\"a\" 0) false)\n"
      "  Mov(Imm(2) Pseudo(\"b\" 0) false)\n"
      "  Mov(Imm(3) Pseudo(\"c\" 0) false)\n"
      "  Compare(Pseudo(\"a\" 0) Pseudo(\"b\" 0))\n"
      "  Compare(Pseudo(\"c\" 0) Imm(0))\n"
      "  Mov(Pseudo(\"i\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"t\" 0 Y) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}], [\"c\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"c\", \"width\": 1, \"lives_across_call\": false}}], [\"i\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"i\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"a\", {\"set\": [\"b\", \"c\", \"i\"]}], [\"b\", {\"set\": [\"a\", \"c\", \"i\"]}], [\"c\", {\"set\": [\"a\", \"b\", \"i\"]}], [\"i\", {\"set\": [\"a\", \"b\", \"c\"]}]]}}}\n");
}

TEST_CASE("hwreg_eligibility: test_asm_regalloc.py::TestFrameAccessWritesY::test_spilled_value_counts_as_frame_access (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_asm_regalloc.py::TestFrameAccessWritesY::test_spilled_value_counts_as_frame_access\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(0) Pseudo(\"i\" 0) false)\n"
      "  Mov(Pseudo(\"i\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"t\" 0 Y) Reg(A) false)\n"
      "  Mov(Imm(1) Pseudo(\"a\" 0) false)\n"
      "  Mov(Imm(2) Pseudo(\"b\" 0) false)\n"
      "  Mov(Imm(3) Pseudo(\"c\" 0) false)\n"
      "  Compare(Pseudo(\"a\" 0) Pseudo(\"b\" 0))\n"
      "  Compare(Pseudo(\"c\" 0) Imm(0))\n"
      "  Mov(Pseudo(\"i\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"t\" 0 Y) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": [\"c\", \"i\"]}, \"eligible_y\": {\"set\": [\"c\", \"i\"]}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": [\"i\"]}, \"use_count\": {\"dict\": [[\"i\", 3], [\"a\", 2], [\"b\", 2], [\"c\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("color_graph: test_asm_regalloc.py::TestFrameAccessWritesY::test_spilled_value_counts_as_frame_access (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass color_graph\n"
      "source tests/test_asm_regalloc.py::TestFrameAccessWritesY::test_spilled_value_counts_as_frame_access\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}], [\"c\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"c\", \"width\": 1, \"lives_across_call\": false}}], [\"i\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"i\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"a\", {\"set\": [\"b\", \"c\", \"i\"]}], [\"b\", {\"set\": [\"a\", \"c\", \"i\"]}], [\"c\", {\"set\": [\"a\", \"b\", \"i\"]}], [\"i\", {\"set\": [\"a\", \"b\", \"c\"]}]]}}}], \"kwargs\": {\"hwreg_eligibility\": {\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": [\"c\", \"i\"]}, \"eligible_y\": {\"set\": [\"c\", \"i\"]}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": [\"i\"]}, \"use_count\": {\"dict\": [[\"i\", 3], [\"a\", 2], [\"b\", 2], [\"c\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}, \"liveness\": {\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 13, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}, \"live_out\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}, \"allowed_range\": {\"range\": [128, 131, 1]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(0) Pseudo(\"i\" 0) false)\n"
      "  Mov(Pseudo(\"i\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"t\" 0 Y) Reg(A) false)\n"
      "  Mov(Imm(1) Pseudo(\"a\" 0) false)\n"
      "  Mov(Imm(2) Pseudo(\"b\" 0) false)\n"
      "  Mov(Imm(3) Pseudo(\"c\" 0) false)\n"
      "  Compare(Pseudo(\"a\" 0) Pseudo(\"b\" 0))\n"
      "  Compare(Pseudo(\"c\" 0) Imm(0))\n"
      "  Mov(Pseudo(\"i\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"t\" 0 Y) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"c\", 128], [\"b\", 129], [\"a\", 130]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": [[\"i\", \"Y\"]]}}}\n");
}

TEST_CASE("color_graph: test_asm_regalloc.py::TestFrameAccessWritesY::test_spilled_value_counts_as_frame_access (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass color_graph\n"
      "source tests/test_asm_regalloc.py::TestFrameAccessWritesY::test_spilled_value_counts_as_frame_access\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}], [\"c\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"c\", \"width\": 1, \"lives_across_call\": false}}], [\"i\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"i\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"a\", {\"set\": [\"b\", \"c\", \"i\"]}], [\"b\", {\"set\": [\"a\", \"c\", \"i\"]}], [\"c\", {\"set\": [\"a\", \"b\", \"i\"]}], [\"i\", {\"set\": [\"a\", \"b\", \"c\"]}]]}}}], \"kwargs\": {\"hwreg_eligibility\": {\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": [\"c\", \"i\"]}, \"eligible_y\": {\"set\": [\"c\", \"i\"]}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": [\"i\"]}, \"use_count\": {\"dict\": [[\"i\", 3], [\"a\", 2], [\"b\", 2], [\"c\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}, \"liveness\": {\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 13, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}, \"live_out\": {\"dict\": [[0, {\"set\": []}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}, \"allowed_range\": {\"range\": [128, 130, 1]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(0) Pseudo(\"i\" 0) false)\n"
      "  Mov(Pseudo(\"i\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"t\" 0 Y) Reg(A) false)\n"
      "  Mov(Imm(1) Pseudo(\"a\" 0) false)\n"
      "  Mov(Imm(2) Pseudo(\"b\" 0) false)\n"
      "  Mov(Imm(3) Pseudo(\"c\" 0) false)\n"
      "  Compare(Pseudo(\"a\" 0) Pseudo(\"b\" 0))\n"
      "  Compare(Pseudo(\"c\" 0) Imm(0))\n"
      "  Mov(Pseudo(\"i\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"t\" 0 Y) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"c\", 128], [\"b\", 129]]}, \"spilled\": {\"set\": [\"a\"]}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": [[\"i\", \"X\"]]}}}\n");
}
