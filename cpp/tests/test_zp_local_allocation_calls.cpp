// The calls that the tests of tests/test_zp_local_allocation.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("allocate_function_locals: test_zp_local_allocation.py::TestIneligibility::test_caller_of_recursive_is_ineligible (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass allocate_function_locals\n"
      "source tests/test_zp_local_allocation.py::TestIneligibility::test_caller_of_recursive_is_ineligible\n"
      "args {\"args\": [{\"dict\": []}, {\"dict\": [[\"main\", 1], [\"rec\", 1]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"rec\" [] None false false)\n"
      "  ])\n"
      "  Function(\"rec\" true [] [\n"
      "    FunctionCall(\"rec\" [] None false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"dict\": []}\n");
}

TEST_CASE("allocate_function_locals: test_zp_local_allocation.py::TestIneligibility::test_indirect_call_makes_ineligible (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass allocate_function_locals\n"
      "source tests/test_zp_local_allocation.py::TestIneligibility::test_indirect_call_makes_ineligible\n"
      "args {\"args\": [{\"dict\": []}, {\"dict\": [[\"f\", 1]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    IndirectCall(Var(\"fp\") [] None)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"dict\": []}\n");
}

TEST_CASE("allocate_function_locals: test_zp_local_allocation.py::TestIneligibility::test_mutual_recursion_makes_both_ineligible (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass allocate_function_locals\n"
      "source tests/test_zp_local_allocation.py::TestIneligibility::test_mutual_recursion_makes_both_ineligible\n"
      "args {\"args\": [{\"dict\": []}, {\"dict\": [[\"a\", 1], [\"b\", 1]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"a\" true [] [\n"
      "    FunctionCall(\"b\" [] None false false)\n"
      "  ])\n"
      "  Function(\"b\" true [] [\n"
      "    FunctionCall(\"a\" [] None false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"dict\": []}\n");
}

TEST_CASE("allocate_function_locals: test_zp_local_allocation.py::TestIneligibility::test_non_zp_abi_extern_callee_disqualifies (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass allocate_function_locals\n"
      "source tests/test_zp_local_allocation.py::TestIneligibility::test_non_zp_abi_extern_callee_disqualifies\n"
      "args {\"args\": [{\"dict\": []}, {\"dict\": [[\"main\", 1]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"puts\" [] None false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"dict\": []}\n");
}

TEST_CASE("allocate_function_locals: test_zp_local_allocation.py::TestIneligibility::test_self_recursion_makes_ineligible (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass allocate_function_locals\n"
      "source tests/test_zp_local_allocation.py::TestIneligibility::test_self_recursion_makes_ineligible\n"
      "args {\"args\": [{\"dict\": []}, {\"dict\": [[\"f\", 1]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"dict\": []}\n");
}

TEST_CASE("allocate_function_locals: test_zp_local_allocation.py::TestIneligibility::test_softstack_extern_callee_disqualifies (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass allocate_function_locals\n"
      "source tests/test_zp_local_allocation.py::TestIneligibility::test_softstack_extern_callee_disqualifies\n"
      "args {\"args\": [{\"dict\": [[\"legacy\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}, {\"dict\": [[\"main\", 1]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"legacy\" [] None false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"dict\": []}\n");
}
