// The calls that the tests of tests/test_address_taken_zp.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("address_taken_assignments: test_address_taken_zp.py::TestNoFrameSlotUnderY::test_falls_back_to_frame_without_y (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass address_taken_assignments\n"
      "source tests/test_address_taken_zp.py::TestNoFrameSlotUnderY::test_falls_back_to_frame_without_y\n"
      "args {\"args\": [{\"dict\": [[\"f\", [128]]]}, {\"dict\": [[\"f\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%p\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": [[\"%i\", \"X\"]]}}}]]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    LoadAddress(Pseudo(\"@1.x\" 0) Pseudo(\"%p\" 0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"dict\": [[\"f\", {\"dict\": []}]]}\n");
}

TEST_CASE("address_taken_assignments: test_address_taken_zp.py::TestNoFrameSlotUnderY::test_raises_with_a_value_in_y (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass address_taken_assignments\n"
      "source tests/test_address_taken_zp.py::TestNoFrameSlotUnderY::test_raises_with_a_value_in_y\n"
      "args {\"args\": [{\"dict\": [[\"f\", [128]]]}, {\"dict\": [[\"f\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%p\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": [[\"%i\", \"Y\"]]}}}]]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    LoadAddress(Pseudo(\"@1.x\" 0) Pseudo(\"%p\" 0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"AssertionError\", \"message\": \"function 'f': the address-taken locals ['@1.x'] didn't fit in the function's pool, and the coloring keeps a value in Y, which an access to their frame slots would overwrite\"}\n");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.py::TestSlotSymbols::test_falls_back_for_temp (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass address_taken_slot_symbols\n"
      "source tests/test_address_taken_zp.py::TestSlotSymbols::test_falls_back_for_temp\n"
      "args {\"args\": [{\"dict\": [[\"%5\", 144]]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"caller\"\n"
      "output-json\n"
      "{\"dict\": [[\"%5\", \"__local_caller__addr_%5\"]]}\n");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.py::TestSlotSymbols::test_renames_source_var (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass address_taken_slot_symbols\n"
      "source tests/test_address_taken_zp.py::TestSlotSymbols::test_renames_source_var\n"
      "args {\"args\": [{\"dict\": [[\"@9.entity_row\", 143]]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"caller\"\n"
      "output-json\n"
      "{\"dict\": [[\"@9.entity_row\", \"__local_caller__entity_row\"]]}\n");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.py::TestSlotSymbols::test_same_source_name_gets_a_suffix (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass address_taken_slot_symbols\n"
      "source tests/test_address_taken_zp.py::TestSlotSymbols::test_same_source_name_gets_a_suffix\n"
      "args {\"args\": [{\"dict\": [[\"@3.compound_literal\", 136], [\"@5.compound_literal\", 140], [\"@7.compound_literal\", 144]]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"f\"\n"
      "output-json\n"
      "{\"dict\": [[\"@3.compound_literal\", \"__local_f__compound_literal\"], [\"@5.compound_literal\", \"__local_f__compound_literal@2\"], [\"@7.compound_literal\", \"__local_f__compound_literal@3\"]]}\n");
}
