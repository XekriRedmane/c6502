// Calls of the zero-page allocators that no Python unit test makes: the
// errors for a cycle among zp_abi functions and for slots and a pool
// that fit nowhere, a pool in the fallback range, and a caller's and its
// callee's pools, each with what the Python function returned or the
// exception it raised.
#include "doctest.h"
#include "sharing_case.h"

TEST_CASE("allocate_zp_slots: an allocation error or fallback (case 1)") {
  c6502::testing::check_sharing_call(
      "allocate_zp_slots",
      "{\"args\": [{\"dict\": [[\"a\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_a__p_0\"], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"b\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_b__p_0\"], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"a\" true [] [\n"
      "    FunctionCall(\"b\" [] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"b\" true [] [\n"
      "    FunctionCall(\"a\" [] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: ZpSlotAllocationError: cycle detected in zp_abi call graph involving ['a', 'b'] — select_abi should have rejected this; internal error\n");
}

TEST_CASE("allocate_zp_slots: an allocation error or fallback (case 2)") {
  c6502::testing::check_sharing_call(
      "allocate_zp_slots",
      "{\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p_0\", \"__zpabi_f__p_1\", \"__zpabi_f__p_2\", \"__zpabi_f__p_3\", \"__zpabi_f__p_4\", \"__zpabi_f__p_5\", \"__zpabi_f__p_6\", \"__zpabi_f__p_7\"], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {\"reserved\": {\"set\": [128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191]}, \"spill_start\": 512, \"spill_end\": 516}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: ZpSlotAllocationError: can't allocate 8-byte slot range for function `f`: no contiguous gap in either the ZP window ($80-$BF) or the spill region ($0200-$0203) avoids every already-allocated caller's range\n");
}

TEST_CASE("allocate_function_locals: an allocation error or fallback (case 1)") {
  c6502::testing::check_sharing_call(
      "allocate_function_locals",
      "{\"args\": [{\"dict\": []}, {\"dict\": [[\"f\", 300]]}], \"kwargs\": {\"spill_start\": 512, \"spill_end\": 528}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: ZpLocalAllocationError: can't allocate 300-byte local pool for function `f`: no contiguous gap in either ZP ($80-$BF) or spill region ($0200-$020F) avoids the 0 forbidden bytes\n");
}

TEST_CASE("allocate_function_locals: an allocation error or fallback (case 2)") {
  c6502::testing::check_sharing_call(
      "allocate_function_locals",
      "{\"args\": [{\"dict\": []}, {\"dict\": [[\"f\", 70]]}], \"kwargs\": {\"fallback\": {\"range\": [16, 96, 1]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", [16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85]]]}\n");
}

TEST_CASE("allocate_function_locals: an allocation error or fallback (case 3)") {
  c6502::testing::check_sharing_call(
      "allocate_function_locals",
      "{\"args\": [{\"dict\": []}, {\"dict\": [[\"main\", 3], [\"g\", 2]]}], \"kwargs\": {\"reserved\": {\"set\": [129]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"g\" [] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"g\" true [] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", [130, 131, 132]], [\"g\", [133, 134]]]}\n");
}
