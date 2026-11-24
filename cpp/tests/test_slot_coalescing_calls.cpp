// The calls that the tests of tests/test_slot_coalescing.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestSlotCoalescing::test_copy_after_a_branch_goes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestSlotCoalescing::test_copy_after_a_branch_goes\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}]]}, [], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__zpabi_f__p\" 0) Reg(A) false)\n"
      "    Branch(PL \".split\")\n"
      "    SetCarry\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Sub(Data(\"__zpabi_f__p\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) false)\n"
      "    Label(\".join\")\n"
      "    Mov(Data(\"__local_f__l\" 0) Reg(X) false)\n"
      "    Return(true false false)\n"
      "    Label(\".split\")\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) false)\n"
      "    Jump(\".join\" [])\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Data(\\\"__zpabi_f__p\\\" 0) Reg(A) false)\\n    Branch(PL \\\".split\\\")\\n    SetCarry\\n    Mov(Imm(0) Reg(A) false)\\n    Sub(Data(\\\"__zpabi_f__p\\\" 0) Reg(A))\\n    Mov(Reg(A) Data(\\\"__zpabi_f__p\\\" 0) false)\\n    Label(\\\".join\\\")\\n    Mov(Data(\\\"__zpabi_f__p\\\" 0) Reg(X) false)\\n    Return(true false false)\\n    Label(\\\".split\\\")\\n    Jump(\\\".join\\\" [])\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", [\"__zpabi_f__p\"]]]}]}\n");
}

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestSlotCoalescing::test_local_takes_the_parameter_slot (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestSlotCoalescing::test_local_takes_the_parameter_slot\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}]]}, [], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__zpabi_f__p\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) false)\n"
      "    ArithmeticShiftLeft(Data(\"__local_f__l\" 0))\n"
      "    Mov(Data(\"__local_f__l\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Data(\\\"__zpabi_f__p\\\" 0) Reg(A) false)\\n    ArithmeticShiftLeft(Data(\\\"__zpabi_f__p\\\" 0))\\n    Mov(Data(\\\"__zpabi_f__p\\\" 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", [\"__zpabi_f__p\"]]]}]}\n");
}

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestSlotCoalescing::test_local_used_as_a_pointer_left_alone (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestSlotCoalescing::test_local_used_as_a_pointer_left_alone\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}]]}, [], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__zpabi_f__p\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) false)\n"
      "    Mov(Imm(0) Reg(Y) false)\n"
      "    Mov(IndirectZpY(129) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Data(\\\"__zpabi_f__p\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__local_f__l\\\" 0) false)\\n    Mov(Imm(0) Reg(Y) false)\\n    Mov(IndirectZpY(129) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": []}]}\n");
}

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestSlotCoalescing::test_parameter_read_after_the_local_changes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestSlotCoalescing::test_parameter_read_after_the_local_changes\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}]]}, [], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__zpabi_f__p\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) false)\n"
      "    Inc(Data(\"__local_f__l\" 0))\n"
      "    Mov(Data(\"__local_f__l\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__zpabi_f__p\" 0) Reg(A))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Data(\\\"__zpabi_f__p\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__local_f__l\\\" 0) false)\\n    Inc(Data(\\\"__local_f__l\\\" 0))\\n    Mov(Data(\\\"__local_f__l\\\" 0) Reg(A) false)\\n    ClearCarry\\n    Add(Data(\\\"__zpabi_f__p\\\" 0) Reg(A))\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": []}]}\n");
}

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestSlotCoalescing::test_shared_slot_left_alone (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestSlotCoalescing::test_shared_slot_left_alone\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}]]}, [{\"object\": \"passes.slot_sharing.SlotShare\", \"fields\": {\"members\": {\"tuple\": [{\"tuple\": [\"f\", {\"tuple\": [\"__zpabi_f__p\"]}]}, {\"tuple\": [\"g\", {\"tuple\": [\"__zpabi_g__q\"]}]}]}, \"holders\": {\"tuple\": []}, \"live\": {\"tuple\": []}, \"forwards\": {\"tuple\": []}, \"writers\": {\"tuple\": []}}}], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__zpabi_f__p\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) false)\n"
      "    ArithmeticShiftLeft(Data(\"__local_f__l\" 0))\n"
      "    Mov(Data(\"__local_f__l\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Data(\\\"__zpabi_f__p\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__local_f__l\\\" 0) false)\\n    ArithmeticShiftLeft(Data(\\\"__local_f__l\\\" 0))\\n    Mov(Data(\\\"__local_f__l\\\" 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": []}]}\n");
}

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestSlotCoalescing::test_unrelated_local_left_alone (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestSlotCoalescing::test_unrelated_local_left_alone\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}]]}, [], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(3) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__m\" 0) false)\n"
      "    Inc(Data(\"__local_f__m\" 0))\n"
      "    Mov(Data(\"__local_f__m\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Imm(3) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__local_f__m\\\" 0) false)\\n    Inc(Data(\\\"__local_f__m\\\" 0))\\n    Mov(Data(\\\"__local_f__m\\\" 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": []}]}\n");
}

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestSlotCoalescing::test_volatile_local_left_alone (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestSlotCoalescing::test_volatile_local_left_alone\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}]]}, [], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__zpabi_f__p\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) true)\n"
      "    Dec(Data(\"__local_f__l\" 0))\n"
      "    Mov(Data(\"__local_f__l\" 0) Reg(A) true)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Data(\\\"__zpabi_f__p\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__local_f__l\\\" 0) true)\\n    Dec(Data(\\\"__local_f__l\\\" 0))\\n    Mov(Data(\\\"__local_f__l\\\" 0) Reg(A) true)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": []}]}\n");
}

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestCalleeSlot::test_local_computed_in_the_callee_slot (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestCalleeSlot::test_local_computed_in_the_callee_slot\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}], [\"g\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_g__q\"], \"addrs\": [144], \"param_registers\": [null], \"pinned\": [null]}}], [\"h\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_h__r\"], \"addrs\": [144], \"param_registers\": [null], \"pinned\": [null]}}]]}, [], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130], [\"__zpabi_g__q\", 144], [\"__zpabi_h__r\", 144]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__local_f__m\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Imm(1) Reg(A))\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) false)\n"
      "    Mov(Data(\"__local_f__l\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__zpabi_g__q\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"h\" false [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Data(\\\"__local_f__m\\\" 0) Reg(A) false)\\n    ClearCarry\\n    Add(Imm(1) Reg(A))\\n    Mov(Reg(A) Data(\\\"__zpabi_g__q\\\" 0) false)\\n    Mov(Data(\\\"__zpabi_g__q\\\" 0) Reg(A) false)\\n    Call(\\\"g\\\" [] [])\\n    Return(false false false)\\n  ])\\n  Function(\\\"g\\\" false [] [\\n    Return(false false false)\\n  ])\\n  Function(\\\"h\\\" false [] [\\n    Return(false false false)\\n  ])\\n])\\n\"}, {\"dict\": []}]}\n");
}

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestCalleeSlot::test_not_live_across_a_call (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestCalleeSlot::test_not_live_across_a_call\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}], [\"g\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_g__q\"], \"addrs\": [144], \"param_registers\": [null], \"pinned\": [null]}}], [\"h\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_h__r\"], \"addrs\": [144], \"param_registers\": [null], \"pinned\": [null]}}]]}, [], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130], [\"__zpabi_g__q\", 144], [\"__zpabi_h__r\", 144]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__local_f__m\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) false)\n"
      "    Call(\"h\" [] [])\n"
      "    Mov(Data(\"__local_f__l\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__zpabi_g__q\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"h\" false [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Data(\\\"__local_f__m\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__local_f__l\\\" 0) false)\\n    Call(\\\"h\\\" [] [])\\n    Mov(Data(\\\"__local_f__l\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__zpabi_g__q\\\" 0) false)\\n    Call(\\\"g\\\" [] [])\\n    Return(false false false)\\n  ])\\n  Function(\\\"g\\\" false [] [\\n    Return(false false false)\\n  ])\\n  Function(\\\"h\\\" false [] [\\n    Return(false false false)\\n  ])\\n])\\n\"}, {\"dict\": []}]}\n");
}

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestCalleeSlot::test_not_live_across_another_callees_argument (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestCalleeSlot::test_not_live_across_another_callees_argument\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}], [\"g\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_g__q\"], \"addrs\": [144], \"param_registers\": [null], \"pinned\": [null]}}], [\"h\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_h__r\"], \"addrs\": [144], \"param_registers\": [null], \"pinned\": [null]}}]]}, [], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130], [\"__zpabi_g__q\", 144], [\"__zpabi_h__r\", 144]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__local_f__m\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) false)\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__zpabi_h__r\" 0) false)\n"
      "    Call(\"h\" [] [])\n"
      "    Mov(Data(\"__local_f__l\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__zpabi_g__q\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"h\" false [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Data(\\\"__local_f__m\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__local_f__l\\\" 0) false)\\n    Mov(Imm(5) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__zpabi_h__r\\\" 0) false)\\n    Call(\\\"h\\\" [] [])\\n    Mov(Data(\\\"__local_f__l\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__zpabi_g__q\\\" 0) false)\\n    Call(\\\"g\\\" [] [])\\n    Return(false false false)\\n  ])\\n  Function(\\\"g\\\" false [] [\\n    Return(false false false)\\n  ])\\n  Function(\\\"h\\\" false [] [\\n    Return(false false false)\\n  ])\\n])\\n\"}, {\"dict\": []}]}\n");
}

TEST_CASE("slot_coalescing: test_slot_coalescing.py::TestCalleeSlot::test_tail_call_reads_the_slot (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass slot_coalescing\n"
      "source tests/test_slot_coalescing.py::TestCalleeSlot::test_tail_call_reads_the_slot\n"
      "args {\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p\"], \"addrs\": [128], \"param_registers\": [null], \"pinned\": [null]}}], [\"g\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_g__q\"], \"addrs\": [144], \"param_registers\": [null], \"pinned\": [null]}}], [\"h\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_h__r\"], \"addrs\": [144], \"param_registers\": [null], \"pinned\": [null]}}]]}, [], {\"dict\": [[\"__zpabi_f__p\", 128], [\"__local_f__l\", 129], [\"__local_f__m\", 130], [\"__zpabi_g__q\", 144], [\"__zpabi_h__r\", 144]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__local_f__m\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__l\" 0) false)\n"
      "    Mov(Data(\"__local_f__l\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__zpabi_g__q\" 0) false)\n"
      "    Jump(\"g\" [])\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"h\" false [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [] [\\n    Mov(Data(\\\"__local_f__m\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"__zpabi_g__q\\\" 0) false)\\n    Mov(Data(\\\"__zpabi_g__q\\\" 0) Reg(A) false)\\n    Jump(\\\"g\\\" [])\\n  ])\\n  Function(\\\"g\\\" false [] [\\n    Return(false false false)\\n  ])\\n  Function(\\\"h\\\" false [] [\\n    Return(false false false)\\n  ])\\n])\\n\"}, {\"dict\": []}]}\n");
}
