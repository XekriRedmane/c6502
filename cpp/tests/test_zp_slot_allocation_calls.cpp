// The calls that the tests of tests/test_zp_slot_allocation.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("allocate_zp_slots: test_zp_slot_allocation.py::TestZpSlotAllocationErrors::test_cycle_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass allocate_zp_slots\n"
      "source tests/test_zp_slot_allocation.py::TestZpSlotAllocationErrors::test_cycle_raises\n"
      "args {\"args\": [{\"dict\": [[\"a\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_a_p0\"], \"addrs\": [128], \"param_registers\": [], \"pinned\": []}}], [\"b\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_b_p0\"], \"addrs\": [128], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}\n"
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
      "output-error\n"
      "{\"type\": \"ZpSlotAllocationError\", \"message\": \"cycle detected in zp_abi call graph involving ['a', 'b'] \\u2014 select_abi should have rejected this; internal error\"}\n");
}
