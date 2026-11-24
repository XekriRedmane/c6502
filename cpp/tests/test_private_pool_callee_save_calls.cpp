// The calls that the tests of tests/test_private_pool_callee_save.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("replace_pseudoregisters_bare_exit: test_private_pool_callee_save.py::TestPrivatePoolCalleeSaveExclusion::test_no_private_pool_keeps_default_behavior (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass replace_pseudoregisters_bare_exit\n"
      "source tests/test_private_pool_callee_save.py::TestPrivatePoolCalleeSaveExclusion::test_no_private_pool_keeps_default_behavior\n"
      "args {\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"f\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"x\", 192]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Pseudo(\"x\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(ZP(192 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 1, \"callee_saved_addrs\": [192]}}]]}]}\n");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_private_pool_callee_save.py::TestPrivatePoolCalleeSaveExclusion::test_partial_overlap_excludes_only_private (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass replace_pseudoregisters_bare_exit\n"
      "source tests/test_private_pool_callee_save.py::TestPrivatePoolCalleeSaveExclusion::test_partial_overlap_excludes_only_private\n"
      "args {\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"f\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"x\", 192], [\"y\", 193]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}, \"local_pools\": {\"dict\": [[\"f\", [192]]]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Pseudo(\"x\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"y\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(ZP(192 0) Reg(A) false)\\n    Mov(ZP(193 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 1, \"callee_saved_addrs\": [193]}}]]}]}\n");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_private_pool_callee_save.py::TestPrivatePoolCalleeSaveExclusion::test_private_pool_excludes_callee_save (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass replace_pseudoregisters_bare_exit\n"
      "source tests/test_private_pool_callee_save.py::TestPrivatePoolCalleeSaveExclusion::test_private_pool_excludes_callee_save\n"
      "args {\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"f\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"x\", 192]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}, \"local_pools\": {\"dict\": [[\"f\", [192]]]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Pseudo(\"x\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(ZP(192 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}]]}]}\n");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_private_pool_callee_save.py::TestPrivatePoolCalleeSaveExclusion::test_private_pool_outside_callee_range_no_op (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass replace_pseudoregisters_bare_exit\n"
      "source tests/test_private_pool_callee_save.py::TestPrivatePoolCalleeSaveExclusion::test_private_pool_outside_callee_range_no_op\n"
      "args {\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"f\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"x\", 132]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}, \"local_pools\": {\"dict\": [[\"f\", [132]]]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Pseudo(\"x\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(ZP(132 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}]]}]}\n");
}
