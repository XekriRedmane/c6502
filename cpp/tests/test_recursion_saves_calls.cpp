// The calls that the tests of tests/test_recursion_saves.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("allocate_function_locals: test_recursion_saves.py::TestCycles::test_unmarked_cycle_gets_no_pool (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass allocate_function_locals\n"
      "source tests/test_recursion_saves.py::TestCycles::test_unmarked_cycle_gets_no_pool\n"
      "args {\"args\": [{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"f\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}, {\"dict\": [[\"main\", 1], [\"f\", 1]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "  ])\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"dict\": []}\n");
}
