// The calls that the tests of tests/test_pointer_aware_regalloc.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("to_ssa: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_loadaddress_dst_is_width_2 (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass to_ssa\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_loadaddress_dst_is_width_2\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n");
}

TEST_CASE("compute_liveness: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_loadaddress_dst_is_width_2 (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass compute_liveness\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_loadaddress_dst_is_width_2\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 5, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": [\"%x\"]}], [1, {\"set\": []}], [2, {\"set\": [\"%x\"]}]]}, \"live_out\": {\"dict\": [[0, {\"set\": [\"%x\"]}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}\n");
}

TEST_CASE("build_interference: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_loadaddress_dst_is_width_2 (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass build_interference\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_loadaddress_dst_is_width_2\n"
      "args {\"args\": [{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 5, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": [\"%x\"]}], [1, {\"set\": []}], [2, {\"set\": [\"%x\"]}]]}, \"live_out\": {\"dict\": [[0, {\"set\": [\"%x\"]}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"%p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%p\", \"width\": 2, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}\n");
}

TEST_CASE("to_ssa: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_pointer_coexists_with_singlebyte_neighbor (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass to_ssa\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_pointer_coexists_with_singlebyte_neighbor\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(42) Pseudo(\"%v\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%v\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(42) Pseudo(\"%v.b0.v1\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%v.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n");
}

TEST_CASE("compute_liveness: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_pointer_coexists_with_singlebyte_neighbor (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass compute_liveness\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_pointer_coexists_with_singlebyte_neighbor\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(42) Pseudo(\"%v.b0.v1\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%v.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 7, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": [\"%x\"]}], [1, {\"set\": []}], [2, {\"set\": [\"%x\"]}]]}, \"live_out\": {\"dict\": [[0, {\"set\": [\"%x\"]}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}\n");
}

TEST_CASE("build_interference: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_pointer_coexists_with_singlebyte_neighbor (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass build_interference\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_pointer_coexists_with_singlebyte_neighbor\n"
      "args {\"args\": [{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 7, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": [\"%x\"]}], [1, {\"set\": []}], [2, {\"set\": [\"%x\"]}]]}, \"live_out\": {\"dict\": [[0, {\"set\": [\"%x\"]}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(42) Pseudo(\"%v.b0.v1\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%v.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"%p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%p\", \"width\": 2, \"lives_across_call\": false}}], [\"%v.b0.v1\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%v.b0.v1\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"%p\", {\"set\": [\"%v.b0.v1\"]}], [\"%v.b0.v1\", {\"set\": [\"%p\"]}]]}}}\n");
}

TEST_CASE("color_graph: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_pointer_coexists_with_singlebyte_neighbor (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass color_graph\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_pointer_coexists_with_singlebyte_neighbor\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"%p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%p\", \"width\": 2, \"lives_across_call\": false}}], [\"%v.b0.v1\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%v.b0.v1\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"%p\", {\"set\": [\"%v.b0.v1\"]}], [\"%v.b0.v1\", {\"set\": [\"%p\"]}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  Mov(Imm(42) Pseudo(\"%v.b0.v1\" 0) false)\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  Mov(Pseudo(\"%v.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%p\", 128], [\"%v.b0.v1\", 130]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}\n");
}

TEST_CASE("to_ssa: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_two_loadaddresses_get_distinct_2byte_blocks (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass to_ssa\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_two_loadaddresses_get_distinct_2byte_blocks\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  LoadAddress(Pseudo(\"%y\" 0) Pseudo(\"%q\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%q\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Mov(Pseudo(\"%q\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  LoadAddress(Pseudo(\"%y\" 0) Pseudo(\"%q\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%q\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Mov(Pseudo(\"%q\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n");
}

TEST_CASE("compute_liveness: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_two_loadaddresses_get_distinct_2byte_blocks (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass compute_liveness\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_two_loadaddresses_get_distinct_2byte_blocks\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  LoadAddress(Pseudo(\"%y\" 0) Pseudo(\"%q\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%q\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Mov(Pseudo(\"%q\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 8, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": [\"%x\", \"%y\"]}], [1, {\"set\": []}], [2, {\"set\": [\"%x\", \"%y\"]}]]}, \"live_out\": {\"dict\": [[0, {\"set\": [\"%x\", \"%y\"]}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}\n");
}

TEST_CASE("build_interference: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_two_loadaddresses_get_distinct_2byte_blocks (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass build_interference\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_two_loadaddresses_get_distinct_2byte_blocks\n"
      "args {\"args\": [{\"object\": \"passes.optimization_asm.liveness.Liveness\", \"fields\": {\"blocks\": [{\"tuple\": [0, 0, [], [2]]}, {\"tuple\": [1, 0, [2], []]}, {\"tuple\": [2, 8, [0], [1]]}], \"block_order\": [2], \"live_in\": {\"dict\": [[0, {\"set\": [\"%x\", \"%y\"]}], [1, {\"set\": []}], [2, {\"set\": [\"%x\", \"%y\"]}]]}, \"live_out\": {\"dict\": [[0, {\"set\": [\"%x\", \"%y\"]}], [1, {\"set\": []}], [2, {\"set\": []}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  LoadAddress(Pseudo(\"%y\" 0) Pseudo(\"%q\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%q\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Mov(Pseudo(\"%q\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"%p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%p\", \"width\": 2, \"lives_across_call\": false}}], [\"%q\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%q\", \"width\": 2, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"%p\", {\"set\": [\"%q\"]}], [\"%q\", {\"set\": [\"%p\"]}]]}}}\n");
}

TEST_CASE("color_graph: test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_two_loadaddresses_get_distinct_2byte_blocks (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass color_graph\n"
      "source tests/test_pointer_aware_regalloc.py::TestInterferenceMultiByteNode::test_two_loadaddresses_get_distinct_2byte_blocks\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"%p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%p\", \"width\": 2, \"lives_across_call\": false}}], [\"%q\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"%q\", \"width\": 2, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"%p\", {\"set\": [\"%q\"]}], [\"%q\", {\"set\": [\"%p\"]}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".main@asm_ssa_block@0\")\n"
      "  LoadAddress(Pseudo(\"%x\" 0) Pseudo(\"%p\" 0))\n"
      "  LoadAddress(Pseudo(\"%y\" 0) Pseudo(\"%q\" 0))\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%q\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"%p\" 1) Reg(A) false)\n"
      "  Mov(Pseudo(\"%q\" 1) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%q\", 128], [\"%p\", 130]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}\n");
}
