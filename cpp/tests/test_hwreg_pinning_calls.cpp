// The calls that the tests of tests/test_hwreg_pinning.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_address_taken_peer_disqualified (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_address_taken_peer_disqualified\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  LoadAddress(Pseudo(\"E\" 0) Pseudo(\"PTR\" 0))\n"
      "  Mov(Pseudo(\"E\" 1) Pseudo(\"P\" 0) false)\n"
      "  Mov(Pseudo(\"P\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": []}, \"eligible_y\": {\"set\": []}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"E\", 2], [\"PTR\", 1], [\"P\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_colorable_pseudo_peer_stays_eligible (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_colorable_pseudo_peer_stays_eligible\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(Pseudo(\"Q\" 0) Pseudo(\"P\" 0) false)\n"
      "  Mov(Pseudo(\"P\" 0) Reg(A) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": [\"P\", \"Q\"]}, \"eligible_y\": {\"set\": [\"P\", \"Q\"]}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"Q\", 1], [\"P\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_eligible_property_is_union_of_x_and_y (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_eligible_property_is_union_of_x_and_y\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(IndexedData(\"arr\" 0 X) Pseudo(\"P\" 0) false)\n"
      "  Mov(Pseudo(\"P\" 0) Reg(A) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": []}, \"eligible_y\": {\"set\": [\"P\"]}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"P\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_load_from_indirect_memory_disqualified (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_load_from_indirect_memory_disqualified\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(Frame(1) Pseudo(\"P\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": []}, \"eligible_y\": {\"set\": []}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"P\", 1]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_load_from_indirect_memory_disqualified (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_load_from_indirect_memory_disqualified\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(Stack(1) Pseudo(\"P\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": []}, \"eligible_y\": {\"set\": []}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"P\", 1]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_load_from_indirect_memory_disqualified (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_load_from_indirect_memory_disqualified\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(Indirect(0) Pseudo(\"P\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": []}, \"eligible_y\": {\"set\": []}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"P\", 1]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_in_unsupported_op_disqualified (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_in_unsupported_op_disqualified\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Add(Pseudo(\"P\" 0) Reg(A))\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": []}, \"eligible_y\": {\"set\": []}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"P\", 1]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_loaded_from_indexed_data_x_indexed_is_y_only (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_loaded_from_indexed_data_x_indexed_is_y_only\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(IndexedData(\"arr\" 0 X) Pseudo(\"P\" 0) false)\n"
      "  Mov(Pseudo(\"P\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"arr2\" 0 X) Reg(A) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": []}, \"eligible_y\": {\"set\": [\"P\"]}, \"hints_x\": {\"set\": [\"P\"]}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"P\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_loaded_from_indexed_data_y_indexed_is_x_only (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_loaded_from_indexed_data_y_indexed_is_x_only\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(IndexedData(\"arr\" 0 Y) Pseudo(\"P\" 0) false)\n"
      "  Mov(Pseudo(\"P\" 0) Reg(A) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": [\"P\"]}, \"eligible_y\": {\"set\": []}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"P\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_stored_to_indexed_data_is_disqualified (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_stored_to_indexed_data_is_disqualified\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"P\" 0) false)\n"
      "  Mov(Pseudo(\"P\" 0) IndexedData(\"arr\" 0 X) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": []}, \"eligible_y\": {\"set\": []}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"P\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_with_indexed_data_setup_chain_gets_x_hint (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_with_indexed_data_setup_chain_gets_x_hint\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(Pseudo(\"P\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": [\"P\"]}, \"eligible_y\": {\"set\": [\"P\"]}, \"hints_x\": {\"set\": [\"P\"]}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"P\", 1]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_with_y_setup_chain_gets_y_hint (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_pseudo_with_y_setup_chain_gets_y_hint\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(Pseudo(\"P\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "  Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": [\"P\"]}, \"eligible_y\": {\"set\": [\"P\"]}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": [\"P\"]}, \"use_count\": {\"dict\": [[\"P\", 1]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_store_to_indirect_memory_disqualified (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_store_to_indirect_memory_disqualified\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(Imm(3) Pseudo(\"P\" 0) false)\n"
      "  Mov(Pseudo(\"P\" 0) Frame(1) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": []}, \"eligible_y\": {\"set\": []}, \"hints_x\": {\"set\": []}, \"hints_y\": {\"set\": []}, \"use_count\": {\"dict\": [[\"P\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}

TEST_CASE("hwreg_eligibility: test_hwreg_pinning.py::TestHwRegEligibility::test_use_count_tracks_references (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass hwreg_eligibility\n"
      "source tests/test_hwreg_pinning.py::TestHwRegEligibility::test_use_count_tracks_references\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"test\" true [] [\n"
      "  Mov(Pseudo(\"P\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Pseudo(\"P\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(Y) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.hwreg_eligibility.HwRegEligibility\", \"fields\": {\"eligible_x\": {\"set\": [\"P\"]}, \"eligible_y\": {\"set\": [\"P\"]}, \"hints_x\": {\"set\": [\"P\"]}, \"hints_y\": {\"set\": [\"P\"]}, \"use_count\": {\"dict\": [[\"P\", 2]]}, \"required_x\": {\"set\": []}, \"required_y\": {\"set\": []}, \"hints_carry\": {\"set\": []}, \"carry_pairs\": []}}\n");
}
