// The calls that the tests of tests/test_coalescing.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_a_routed_pair_no_interference_coalesces (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_a_routed_pair_no_interference_coalesces\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"a\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"b\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": [[\"b\", \"a\"]]}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_a_routed_volatile_not_coalesced (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_a_routed_volatile_not_coalesced\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"a\" 0) Reg(A) true)\n"
      "  Mov(Reg(A) Pseudo(\"b\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": []}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_byte_level_check_allows_a_copy_of_a_live_byte (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_byte_level_check_allows_a_copy_of_a_live_byte\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"p\", \"width\": 2, \"lives_across_call\": false}}], [\"s\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"s\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"p\", {\"set\": [\"s\"]}], [\"s\", {\"set\": [\"p\"]}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"p\", \"width\": 2, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"p\", {\"set\": []}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(7) Pseudo(\"s\" 0) false)\n"
      "  Mov(Imm(1) Pseudo(\"p\" 1) false)\n"
      "  Mov(Pseudo(\"s\" 0) Pseudo(\"p\" 0) false)\n"
      "  Mov(Pseudo(\"p\" 1) Reg(A) false)\n"
      "  Mov(Pseudo(\"s\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": [[\"s\", \"p\"]]}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_byte_level_check_allows_dead_byte (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_byte_level_check_allows_dead_byte\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"p\", \"width\": 2, \"lives_across_call\": false}}], [\"s\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"s\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"p\", {\"set\": [\"s\"]}], [\"s\", {\"set\": [\"p\"]}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"p\", \"width\": 2, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"p\", {\"set\": []}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "  Mov(Imm(7) Pseudo(\"s\" 0) false)\n"
      "  Mov(Pseudo(\"p\" 1) Reg(A) false)\n"
      "  Mov(Pseudo(\"s\" 0) Pseudo(\"p\" 0) false)\n"
      "  Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": [[\"s\", \"p\"]]}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_byte_level_check_rejects_a_later_write (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_byte_level_check_rejects_a_later_write\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"p\", \"width\": 2, \"lives_across_call\": false}}], [\"s\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"s\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"p\", {\"set\": [\"s\"]}], [\"s\", {\"set\": [\"p\"]}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(7) Pseudo(\"s\" 0) false)\n"
      "  Mov(Imm(1) Pseudo(\"p\" 1) false)\n"
      "  Mov(Pseudo(\"s\" 0) Pseudo(\"p\" 0) false)\n"
      "  Mov(Imm(9) Pseudo(\"p\" 0) false)\n"
      "  Mov(Pseudo(\"p\" 1) Reg(A) false)\n"
      "  Mov(Pseudo(\"s\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": []}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_byte_level_check_rejects_live_byte (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_byte_level_check_rejects_live_byte\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"p\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"p\", \"width\": 2, \"lives_across_call\": false}}], [\"s\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"s\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"p\", {\"set\": [\"s\"]}], [\"s\", {\"set\": [\"p\"]}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Imm(7) Pseudo(\"s\" 0) false)\n"
      "  Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "  Mov(Pseudo(\"s\" 0) Pseudo(\"p\" 0) false)\n"
      "  Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "  Return(true false false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": []}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_chained_merges_resolve_transitively (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_chained_merges_resolve_transitively\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}], [\"c\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"c\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"a\" 0) Pseudo(\"b\" 0) false)\n"
      "  Mov(Pseudo(\"b\" 0) Pseudo(\"c\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": [[\"b\", \"a\"], [\"c\", \"a\"]]}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_different_widths_coalesce_at_byte (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_different_widths_coalesce_at_byte\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 2, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 2, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"a\" 0) Pseudo(\"b\" 1) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": [[\"a\", \"b\"]]}, \"byte_offset\": {\"dict\": [[\"a\", 1]]}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_excluded_name_not_coalesced (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_excluded_name_not_coalesced\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"static_name\" 0) Pseudo(\"a\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": []}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_interfering_pair_not_coalesced (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_interfering_pair_not_coalesced\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"a\", {\"set\": [\"b\"]}], [\"b\", {\"set\": [\"a\"]}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"a\" 0) Pseudo(\"b\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": []}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_lives_across_call_ored (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_lives_across_call_ored\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": true}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": true}}]]}, \"adj\": {\"dict\": []}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"a\" 0) Pseudo(\"b\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": [[\"b\", \"a\"]]}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_mov_pair_no_interference_coalesces (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_mov_pair_no_interference_coalesces\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"a\" 0) Pseudo(\"b\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": [[\"b\", \"a\"]]}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_neighbor_edges_redirected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_neighbor_edges_redirected\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}], [\"c\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"c\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"b\", {\"set\": [\"c\"]}], [\"c\", {\"set\": [\"b\"]}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"c\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"c\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": [[\"a\", {\"set\": [\"c\"]}], [\"c\", {\"set\": [\"a\"]}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"a\" 0) Pseudo(\"b\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": [[\"b\", \"a\"]]}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_offset_nonzero_pseudo_not_coalesced (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_offset_nonzero_pseudo_not_coalesced\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"a\" 0) Pseudo(\"b\" 1) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": []}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_phi_args_coalesce_with_dst (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_phi_args_coalesce_with_dst\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}], [\"phi\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"phi\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"phi\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"phi\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Phi(Pseudo(\"phi\" 0) [AsmPhiArg(\"L1\" Pseudo(\"a\" 0)) AsmPhiArg(\"L2\" Pseudo(\"b\" 0))])\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": [[\"a\", \"phi\"], [\"b\", \"phi\"]]}, \"byte_offset\": {\"dict\": []}}}\n");
}

TEST_CASE("coalesce_moves: test_coalescing.py::TestCoalescingUnit::test_self_loop_avoided_via_chain (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass coalesce_moves\n"
      "source tests/test_coalescing.py::TestCoalescingUnit::test_self_loop_avoided_via_chain\n"
      "args {\"args\": [{\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}], [\"b\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"b\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.optimization.interference.InterferenceGraph\", \"fields\": {\"nodes\": {\"dict\": [[\"a\", {\"object\": \"passes.optimization.interference.InterferenceNode\", \"fields\": {\"name\": \"a\", \"width\": 1, \"lives_across_call\": false}}]]}, \"adj\": {\"dict\": []}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"a\" 0) Pseudo(\"b\" 0) false)\n"
      "  Mov(Pseudo(\"b\" 0) Pseudo(\"a\" 0) false)\n"
      "])\n"
      "output-json\n"
      "{\"object\": \"passes.optimization_asm.coalescing.CoalesceResult\", \"fields\": {\"representative\": {\"dict\": [[\"b\", \"a\"]]}, \"byte_offset\": {\"dict\": []}}}\n");
}
