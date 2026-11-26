// The calls that the unit tests of the Python modules of part F2 make of
// `replace_program`, `replace_program_bare_exit`, their per-function
// forms, `compute_address_taken_assignments`,
// `address_taken_zp.slot_symbols`, `lower_data_load_address`,
// `compute_local_slot_names`, `local_slot_names`, and
// `build_local_slot_symbols`, each with what the Python function
// returned. They were recorded by wrapping the Python functions while
// tests/test_replace_pseudoregisters.py,
// test_replace_pseudoregisters_zp_abi.py, test_address_taken_zp.py, and
// test_lower_data_load_address.py ran. A call of `replace_function` or
// `replace_function_bare_exit` is recorded as the program-level call on
// a program of that one function, with the function's coloring, layout,
// pool, and address-taken locals under its name, and its result is what
// Python's program-level function returned on that program. A call of
// `local_slot_names` is recorded as `compute_local_slot_names` of the
// one pool. The calls whose arguments the harness can't encode (the fake
// symbols and colorings of test_address_taken_zp.py, and the stub
// program of a test of the TypeError) are left out, and so is each
// distinct call after its first. The last four cases are written by
// hand, for the two errors of `replace_pseudoregisters`, for the
// fallbacks of `local_slot_names`, and for an address-taken local
// without room while Y holds a value, whose results came from Python.
#include <exception>
#include <string>
#include <string_view>

#include "capture.h"
#include "doctest.h"
#include "json.h"
#include "passes/stage_registry.h"

namespace {

// Runs the stage call `name` on `input` with the call's arguments
// (`args_json`) in the classic layout and checks that it returns
// `expected`, a dump or tagged JSON. An `expected` of the form
// `exception: <Python type>: <message>` checks that the call throws an
// exception whose message holds `<message>`.
void check_frames_case(std::string_view name, std::string_view args_json, std::string_view input,
                       std::string_view expected) {
  const c6502::passes::StageSignature* sig = c6502::passes::find_stage(name);
  REQUIRE(sig != nullptr);
  REQUIRE(sig->run != nullptr);
  c6502::capture::Call call;
  call.pass = std::string(name);
  call.args_json = std::string(args_json);
  call.input = std::string(input);
  call.input_is_json = sig->input_json != nullptr;
  const c6502::json::Value args = c6502::json::parse(args_json);
  constexpr std::string_view kException = "exception: ";
  if (expected.substr(0, kException.size()) == kException) {
    const std::string_view rest = expected.substr(kException.size());
    const std::string_view message = rest.substr(rest.find(": ") + 2);
    std::string what;
    try {
      sig->run(call, c6502::passes::AsmContext{}, *args.get("args"), *args.get("kwargs"));
    } catch (const std::exception& e) {
      what = e.what();
    }
    CHECK(what.find(message) != std::string::npos);
    return;
  }
  const c6502::passes::StageOutput got =
      sig->run(call, c6502::passes::AsmContext{}, *args.get("args"), *args.get("kwargs"));
  CHECK(got.output == expected);
  CHECK(got.mutated.empty());
}

}  // namespace

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestColoringIntegration.test_callee_saved_addrs_collected_and_reserved (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"hot\", 192]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"hot\" 0) false)\n"
      "    Mov(Imm(2) Pseudo(\"cold\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 2 [192])\n"
      "    Mov(Imm(1) ZP(192 0) false)\n"
      "    Mov(Imm(2) Frame(2) false)\n"
      "    Ret(0 2 true false [192] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestColoringIntegration.test_colored_local_high_byte (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"x\", 130]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Pseudo(\"x\" 1) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 0 [])\n"
      "    Mov(ZP(130 1) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestColoringIntegration.test_colored_local_lowers_to_zp (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"x\", 130]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"x\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 0 [])\n"
      "    Mov(Imm(1) ZP(130 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestColoringIntegration.test_colored_local_skips_frame_allocation (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"colored\", 130]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"colored\" 0) false)\n"
      "    Mov(Imm(2) Pseudo(\"uncolored\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Mov(Imm(1) ZP(130 0) false)\n"
      "    Mov(Imm(2) Frame(1) false)\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestColoringIntegration.test_multibyte_callee_saved_enumerates_all_bytes (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"wide\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"colorings\": {\"dict\": [[\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"wide\", 192]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"wide\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 4 [192 193 194 195])\n"
      "    Mov(Imm(1) ZP(192 0) false)\n"
      "    Ret(0 4 true false [192 193 194 195] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestColoringIntegration.test_no_coloring_arg_unchanged (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"x\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Mov(Imm(1) Frame(1) false)\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestColoringIntegration.test_param_ignores_coloring (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"p\", 130]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}}}",
      "Program([\n"
      "  Function(\"main\" true [\"p\"] [\n"
      "    Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [\"p\"] [\n"
      "    FunctionPrologue(1 0 [])\n"
      "    Mov(Frame(3) Reg(A) false)\n"
      "    Ret(1 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestColoringIntegration.test_replace_program_threads_per_function_colorings (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"foo\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"x\", 130]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}}}",
      "Program([\n"
      "  Function(\"foo\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"x\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "  Function(\"bar\" true [] [\n"
      "    Mov(Imm(2) Pseudo(\"y\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"foo\" true [] [\n"
      "    FunctionPrologue(0 0 [])\n"
      "    Mov(Imm(1) ZP(130 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "  Function(\"bar\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Mov(Imm(2) Frame(1) false)\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestColoringIntegration.test_spilled_local_falls_through_to_frame (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {\"colorings\": {\"dict\": [[\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"x\", 130]]}, \"spilled\": {\"set\": [\"y\"]}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"x\" 0) false)\n"
      "    Mov(Imm(2) Pseudo(\"y\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Mov(Imm(1) ZP(130 0) false)\n"
      "    Mov(Imm(2) Frame(1) false)\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestLocalLayout.test_arith_instructions_rewrite_pseudo_operands (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Add(Pseudo(\"t\" 0) Reg(A))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Add(Frame(1) Reg(A))\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestLocalLayout.test_arith_instructions_rewrite_pseudo_operands (call 2)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Sub(Pseudo(\"t\" 0) Reg(A))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Sub(Frame(1) Reg(A))\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestLocalLayout.test_arith_instructions_rewrite_pseudo_operands (call 3)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    And(Pseudo(\"t\" 0) Reg(A))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    And(Frame(1) Reg(A))\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestLocalLayout.test_arith_instructions_rewrite_pseudo_operands (call 4)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Or(Pseudo(\"t\" 0) Reg(A))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Or(Frame(1) Reg(A))\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestLocalLayout.test_arith_instructions_rewrite_pseudo_operands (call 5)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Xor(Reg(A) Pseudo(\"t\" 0) Reg(A))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Xor(Reg(A) Frame(1) Reg(A))\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestLocalLayout.test_distinct_pseudos_get_sequential_offsets (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"a\" 0) false)\n"
      "    Mov(Imm(2) Pseudo(\"b\" 0) false)\n"
      "    Mov(Pseudo(\"a\" 0) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 2 [])\n"
      "    Mov(Imm(1) Frame(1) false)\n"
      "    Mov(Imm(2) Frame(2) false)\n"
      "    Mov(Frame(1) Reg(A) false)\n"
      "    Ret(0 2 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestLocalLayout.test_imm_src_only_pseudo_dst (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(7) Pseudo(\"t\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Mov(Imm(7) Frame(1) false)\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestLocalLayout.test_same_pseudo_on_both_sides_uses_one_offset (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Pseudo(\"x\" 0) Pseudo(\"x\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Mov(Frame(1) Frame(1) false)\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestNoPseudosNoLocals.test_empty_function (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 0 [])\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestNoPseudosNoLocals.test_function_with_no_pseudos (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(42) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 0 [])\n"
      "    Mov(Imm(42) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestParamLayout.test_one_param_no_locals_lands_at_offset_3 (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [\"a\"] [\n"
      "    Mov(Pseudo(\"a\" 0) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [\"a\"] [\n"
      "    FunctionPrologue(1 0 [])\n"
      "    Mov(Frame(3) Reg(A) false)\n"
      "    Ret(1 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestParamLayout.test_param_order_independent_of_use_order (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [\"a\" \"b\"] [\n"
      "    Mov(Pseudo(\"b\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"a\" 0) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [\"a\" \"b\"] [\n"
      "    FunctionPrologue(2 0 [])\n"
      "    Mov(Frame(4) Reg(A) false)\n"
      "    Mov(Frame(3) Reg(A) false)\n"
      "    Ret(2 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestParamLayout.test_param_used_as_dst (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [\"a\"] [\n"
      "    Mov(Imm(5) Pseudo(\"a\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [\"a\"] [\n"
      "    FunctionPrologue(1 0 [])\n"
      "    Mov(Imm(5) Frame(3) false)\n"
      "    Ret(1 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestParamLayout.test_save_a_false_carries_through_ret_patch (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(5) Pseudo(\"t\" 0) false)\n"
      "    Ret(0 0 false false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Mov(Imm(5) Frame(1) false)\n"
      "    Ret(0 1 false false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestParamLayout.test_two_params_two_locals (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [\"a\" \"b\"] [\n"
      "    Mov(Imm(1) Pseudo(\"t1\" 0) false)\n"
      "    Mov(Pseudo(\"a\" 0) Reg(A) false)\n"
      "    Mov(Imm(2) Pseudo(\"t2\" 0) false)\n"
      "    Mov(Pseudo(\"b\" 0) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [\"a\" \"b\"] [\n"
      "    FunctionPrologue(2 2 [])\n"
      "    Mov(Imm(1) Frame(1) false)\n"
      "    Mov(Frame(5) Reg(A) false)\n"
      "    Mov(Imm(2) Frame(2) false)\n"
      "    Mov(Frame(6) Reg(A) false)\n"
      "    Ret(2 2 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestParamLayout.test_unused_param_does_not_appear_in_body (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [\"a\" \"b\"] [\n"
      "    Mov(Pseudo(\"a\" 0) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [\"a\" \"b\"] [\n"
      "    FunctionPrologue(2 0 [])\n"
      "    Mov(Frame(3) Reg(A) false)\n"
      "    Ret(2 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestPassThrough.test_call_jump_branch_label_pass_through (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Call(\"mul8\" [] [])\n"
      "    Jump(\"end\" [])\n"
      "    Branch(EQ \"end\")\n"
      "    Label(\"end\")\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 0 [])\n"
      "    Call(\"mul8\" [] [])\n"
      "    Jump(\"end\" [])\n"
      "    Branch(EQ \"end\")\n"
      "    Label(\"end\")\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestPassThrough.test_non_pseudo_operands_pass_through (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Frame(7) Reg(A) false)\n"
      "    Mov(Stack(3) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 0 [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Frame(7) Reg(A) false)\n"
      "    Mov(Stack(3) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestReplaceProgram.test_function_with_params_and_locals_via_program (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"foo\" true [\"a\"] [\n"
      "    Mov(Imm(5) Pseudo(\"t\" 0) false)\n"
      "    Mov(Pseudo(\"a\" 0) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"foo\" true [\"a\"] [\n"
      "    FunctionPrologue(1 1 [])\n"
      "    Mov(Imm(5) Frame(1) false)\n"
      "    Mov(Frame(4) Reg(A) false)\n"
      "    Ret(1 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.TestReplaceProgram.test_two_functions_have_independent_local_counters (call 1)") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"foo\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"t\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "  Function(\"bar\" true [] [\n"
      "    Mov(Imm(2) Pseudo(\"t\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"foo\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Mov(Imm(1) Frame(1) false)\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "  Function(\"bar\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Mov(Imm(2) Frame(1) false)\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_replace_pseudoregisters_zp_abi.TestReplaceProgramBareExitWithLayouts.test_program_layouts_threaded_per_function (call 1)") {
  check_frames_case(
      "replace_pseudoregisters_bare_exit",
      "{\"args\": [], \"kwargs\": {\"param_layouts\": {\"dict\": [[\"zp_fn\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_zp_fn_p0\"], \"addrs\": [128], \"param_registers\": [], \"pinned\": []}}], [\"ss_fn\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}}}",
      "Program([\n"
      "  Function(\"zp_fn\" true [\"x\"] [\n"
      "    Mov(Pseudo(\"x\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "  Function(\"ss_fn\" true [\"y\"] [\n"
      "    Mov(Pseudo(\"y\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"zp_fn\\\" true [\\\"x\\\"] [\\n    Mov(Data(\\\"__zpabi_zp_fn_p0\\\" 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n  Function(\\\"ss_fn\\\" true [\\\"y\\\"] [\\n    Mov(Frame(3) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"zp_fn\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}], [\"ss_fn\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 1, \"local_bytes\": 0, \"callee_saved_addrs\": []}}]]}]}");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_replace_pseudoregisters_zp_abi.TestZpAbiParamResolution.test_no_param_layout_uses_frame (call 1)") {
  check_frames_case(
      "replace_pseudoregisters_bare_exit",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [\"p\"] [\n"
      "    Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [\\\"p\\\"] [\\n    Mov(Frame(3) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 1, \"local_bytes\": 0, \"callee_saved_addrs\": []}}]]}]}");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_replace_pseudoregisters_zp_abi.TestZpAbiParamResolution.test_zp_layout_local_still_gets_frame (call 1)") {
  check_frames_case(
      "replace_pseudoregisters_bare_exit",
      "{\"args\": [], \"kwargs\": {\"param_layouts\": {\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f_p0\"], \"addrs\": [128], \"param_registers\": [], \"pinned\": []}}]]}}}",
      "Program([\n"
      "  Function(\"f\" true [\"p\"] [\n"
      "    Mov(Pseudo(\"p\" 0) Pseudo(\"local\" 0) false)\n"
      "    Mov(Pseudo(\"local\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [\\\"p\\\"] [\\n    Mov(Data(\\\"__zpabi_f_p0\\\" 0) Frame(1) false)\\n    Mov(Frame(1) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 1, \"callee_saved_addrs\": []}}]]}]}");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_replace_pseudoregisters_zp_abi.TestZpAbiParamResolution.test_zp_layout_multi_byte_param_resolves_byte_by_byte (call 1)") {
  check_frames_case(
      "replace_pseudoregisters_bare_exit",
      "{\"args\": [], \"kwargs\": {\"param_layouts\": {\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f_p0\", \"__zpabi_f_p1\"], \"addrs\": [128, 129], \"param_registers\": [], \"pinned\": []}}]]}}}",
      "Program([\n"
      "  Function(\"f\" true [\"p\"] [\n"
      "    Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"p\" 1) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [\\\"p\\\"] [\\n    Mov(Data(\\\"__zpabi_f_p0\\\" 0) Reg(A) false)\\n    Mov(Data(\\\"__zpabi_f_p1\\\" 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}]]}]}");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_replace_pseudoregisters_zp_abi.TestZpAbiParamResolution.test_zp_layout_param_resolves_to_zp (call 1)") {
  check_frames_case(
      "replace_pseudoregisters_bare_exit",
      "{\"args\": [], \"kwargs\": {\"param_layouts\": {\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f_p0\"], \"addrs\": [128], \"param_registers\": [], \"pinned\": []}}]]}}}",
      "Program([\n"
      "  Function(\"f\" true [\"p\"] [\n"
      "    Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [\\\"p\\\"] [\\n    Mov(Data(\\\"__zpabi_f_p0\\\" 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}]]}]}");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_replace_pseudoregisters_zp_abi.TestZpAbiParamResolution.test_zp_layout_two_params_use_consecutive_addrs (call 1)") {
  check_frames_case(
      "replace_pseudoregisters_bare_exit",
      "{\"args\": [], \"kwargs\": {\"param_layouts\": {\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f_p0\", \"__zpabi_f_p1\"], \"addrs\": [128, 129], \"param_registers\": [], \"pinned\": []}}]]}}}",
      "Program([\n"
      "  Function(\"f\" true [\"a\" \"b\"] [\n"
      "    Mov(Pseudo(\"a\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"b\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"f\\\" true [\\\"a\\\" \\\"b\\\"] [\\n    Mov(Data(\\\"__zpabi_f_p0\\\" 0) Reg(A) false)\\n    Mov(Data(\\\"__zpabi_f_p1\\\" 0) Reg(A) false)\\n    Return(true false false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"f\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}]]}]}");
}

TEST_CASE("local_slot_names: test_address_taken_zp.TestAddressTakenZp.test_address_taken_in_loop_body (call 1)") {
  check_frames_case(
      "local_slot_names",
      "{\"args\": [{\"dict\": [[\"finder\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"@0.out.reg\", 131]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}], \"kwargs\": {}}",
      "{\"dict\": [[\"finder\", [131, 132]]]}",
      "{\"dict\": [[\"finder\", [\"__local_finder__out\", \"__local_finder__0\"]]]}");
}

TEST_CASE("local_slot_names: test_address_taken_zp.TestAddressTakenZp.test_address_taken_in_loop_body (call 2)") {
  check_frames_case(
      "local_slot_names",
      "{\"args\": [{\"dict\": [[\"caller\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%1.1.b0.v1\", 128], [\"%0.1\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}], \"kwargs\": {}}",
      "{\"dict\": [[\"caller\", [128, 129, 130]]]}",
      "{\"dict\": [[\"caller\", [\"__local_caller__0\", \"__local_caller__1\", \"__local_caller__2\"]]]}");
}

TEST_CASE("address_taken_assignments: test_address_taken_zp.TestAddressTakenZp.test_address_taken_in_loop_body (call 1)") {
  check_frames_case(
      "address_taken_assignments",
      "{\"args\": [{\"dict\": [[\"main\", []], [\"caller\", [128, 129, 130]], [\"finder\", [131, 132]]]}, {\"dict\": [[\"finder\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"@0.out.reg\", 131]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"caller\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%1.1.b0.v1\", 128], [\"%0.1\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"target\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"finder\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(UChar)] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"caller\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.row\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.out.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"finder\" true [\"@0.out\"] [\n"
      "    Label(\".finder@asm_ssa_block@0\")\n"
      "    Mov(Reg(A) Data(\"__local_finder__out\" 0) false)\n"
      "    Mov(Reg(X) Data(\"__local_finder__0\" 0) false)\n"
      "    Mov(Data(\"__local_finder__out\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Data(\"__local_finder__0\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Pseudo(\"target\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(1))\n"
      "    Return(false false true)\n"
      "  ])\n"
      "  Function(\"caller\" true [] [\n"
      "    Label(\".caller@asm_ssa_block@0\")\n"
      "    LoadAddress(Pseudo(\"@1.row\" 0) Data(\"__local_caller__0\" 0))\n"
      "    Mov(Data(\"__local_caller__1\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Call(\"finder\" [\"A\" \"X\"] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    RotateLeft(Reg(A))\n"
      "    Mov(Reg(A) Data(\"__local_caller__0\" 0) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Branch(NE \".if_end@0\")\n"
      "    Label(\".caller@asm_ssa_block@1\")\n"
      "    Return(false false false)\n"
      "    Label(\".if_end@0\")\n"
      "    Mov(Pseudo(\"@1.row\" 0) Pseudo(\"target\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@asm_ssa_block@0\")\n"
      "    Call(\"caller\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "  StaticVariable(\"target\" true [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"finder\", {\"dict\": []}], [\"caller\", {\"dict\": [[\"@1.row\", 130]]}], [\"main\", {\"dict\": []}]]}");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.TestAddressTakenZp.test_address_taken_in_loop_body (call 1)") {
  check_frames_case(
      "address_taken_slot_symbols",
      "{\"args\": [{\"dict\": []}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"target\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"finder\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(UChar)] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"caller\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.row\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.out.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "\"finder\"",
      "{\"dict\": []}");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.TestAddressTakenZp.test_address_taken_in_loop_body (call 2)") {
  check_frames_case(
      "address_taken_slot_symbols",
      "{\"args\": [{\"dict\": [[\"@1.row\", 130]]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"target\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"finder\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(UChar)] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"caller\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.row\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.out.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "\"caller\"",
      "{\"dict\": [[\"@1.row\", \"__local_caller__row\"]]}");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.TestAddressTakenZp.test_address_taken_in_loop_body (call 3)") {
  check_frames_case(
      "address_taken_slot_symbols",
      "{\"args\": [{\"dict\": []}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"target\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"finder\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(UChar)] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"caller\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.row\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.out.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "\"main\"",
      "{\"dict\": []}");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_address_taken_zp.TestAddressTakenZp.test_address_taken_in_loop_body (call 1)") {
  check_frames_case(
      "replace_pseudoregisters_bare_exit",
      "{\"args\": [], \"kwargs\": {\"extra_statics\": {\"set\": [\"target\"]}, \"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"target\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"finder\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(UChar)] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"caller\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.row\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.out.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"types\": {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"colorings\": {\"dict\": [[\"finder\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"@0.out.reg\", 131]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"caller\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%1.1.b0.v1\", 128], [\"%0.1\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}, \"param_layouts\": {\"dict\": [[\"finder\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_finder__out_0\", \"__zpabi_finder__out_1\"], \"addrs\": [null, null], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"caller\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}, \"local_pools\": {\"dict\": [[\"main\", []], [\"caller\", [128, 129, 130]], [\"finder\", [131, 132]]]}, \"address_taken_assignments\": {\"dict\": [[\"finder\", {\"dict\": []}], [\"caller\", {\"dict\": [[\"@1.row\", 130]]}], [\"main\", {\"dict\": []}]]}, \"address_taken_symbols\": {\"dict\": [[\"finder\", {\"dict\": []}], [\"caller\", {\"dict\": [[\"@1.row\", \"__local_caller__row\"]]}], [\"main\", {\"dict\": []}]]}}}",
      "Program([\n"
      "  Function(\"finder\" true [\"@0.out\"] [\n"
      "    Label(\".finder@asm_ssa_block@0\")\n"
      "    Mov(Reg(A) Data(\"__local_finder__out\" 0) false)\n"
      "    Mov(Reg(X) Data(\"__local_finder__0\" 0) false)\n"
      "    Mov(Data(\"__local_finder__out\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Data(\"__local_finder__0\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Pseudo(\"target\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(1))\n"
      "    Return(false false true)\n"
      "  ])\n"
      "  Function(\"caller\" true [] [\n"
      "    Label(\".caller@asm_ssa_block@0\")\n"
      "    LoadAddress(Pseudo(\"@1.row\" 0) Data(\"__local_caller__0\" 0))\n"
      "    Mov(Data(\"__local_caller__1\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Call(\"finder\" [\"A\" \"X\"] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    RotateLeft(Reg(A))\n"
      "    Mov(Reg(A) Data(\"__local_caller__0\" 0) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Branch(NE \".if_end@0\")\n"
      "    Label(\".caller@asm_ssa_block@1\")\n"
      "    Return(false false false)\n"
      "    Label(\".if_end@0\")\n"
      "    Mov(Pseudo(\"@1.row\" 0) Pseudo(\"target\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@asm_ssa_block@0\")\n"
      "    Call(\"caller\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "  StaticVariable(\"target\" true [ZeroInit(1)])\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"finder\\\" true [\\\"@0.out\\\"] [\\n    Label(\\\".finder@asm_ssa_block@0\\\")\\n    Mov(Reg(A) Data(\\\"__local_finder__out\\\" 0) false)\\n    Mov(Reg(X) Data(\\\"__local_finder__0\\\" 0) false)\\n    Mov(Data(\\\"__local_finder__out\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"DPTR\\\" 0) false)\\n    Mov(Data(\\\"__local_finder__0\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"DPTR\\\" 1) false)\\n    Mov(Data(\\\"target\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Indirect(0) false)\\n    Mov(Imm(1) Reg(A) false)\\n    Compare(Reg(A) Imm(1))\\n    Return(false false true)\\n  ])\\n  Function(\\\"caller\\\" true [] [\\n    Label(\\\".caller@asm_ssa_block@0\\\")\\n    LoadAddress(Data(\\\"__local_caller__row\\\" 0) Data(\\\"__local_caller__0\\\" 0))\\n    Mov(Data(\\\"__local_caller__1\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Reg(X) false)\\n    Mov(Data(\\\"__local_caller__0\\\" 0) Reg(A) false)\\n    Call(\\\"finder\\\" [\\\"A\\\" \\\"X\\\"] [])\\n    Mov(Imm(0) Reg(A) false)\\n    RotateLeft(Reg(A))\\n    Mov(Reg(A) Data(\\\"__local_caller__0\\\" 0) false)\\n    Mov(Data(\\\"__local_caller__0\\\" 0) Reg(A) false)\\n    Branch(NE \\\".if_end@0\\\")\\n    Label(\\\".caller@asm_ssa_block@1\\\")\\n    Return(false false false)\\n    Label(\\\".if_end@0\\\")\\n    Mov(Data(\\\"__local_caller__row\\\" 0) Data(\\\"target\\\" 0) false)\\n    Return(false false false)\\n  ])\\n  Function(\\\"main\\\" true [] [\\n    Label(\\\".main@asm_ssa_block@0\\\")\\n    Call(\\\"caller\\\" [] [])\\n    Mov(Imm(0) Reg(A) false)\\n    Mov(Reg(A) Reg(X) false)\\n    Mov(Imm(0) Reg(A) false)\\n    Return(true true false)\\n  ])\\n  StaticVariable(\\\"target\\\" true [ZeroInit(1)])\\n])\\n\"}, {\"dict\": [[\"finder\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}], [\"caller\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}], [\"main\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}]]}]}");
}

TEST_CASE("local_slot_names: test_address_taken_zp.TestAddressTakenZp.test_address_taken_in_loop_body (call 3)") {
  check_frames_case(
      "local_slot_names",
      "{\"args\": [{\"dict\": [[\"finder\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"@0.out.reg\", 131]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"caller\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%1.1.b0.v1\", 128], [\"%0.1\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}], \"kwargs\": {\"address_taken_assignments\": {\"dict\": [[\"finder\", {\"dict\": []}], [\"caller\", {\"dict\": [[\"@1.row\", 130]]}], [\"main\", {\"dict\": []}]]}, \"address_taken_symbols\": {\"dict\": [[\"finder\", {\"dict\": []}], [\"caller\", {\"dict\": [[\"@1.row\", \"__local_caller__row\"]]}], [\"main\", {\"dict\": []}]]}}}",
      "{\"dict\": [[\"main\", []], [\"caller\", [128, 129, 130]], [\"finder\", [131, 132]]]}",
      "{\"dict\": [[\"main\", []], [\"caller\", [\"__local_caller__0\", \"__local_caller__1\", \"__local_caller__row\"]], [\"finder\", [\"__local_finder__out\", \"__local_finder__0\"]]]}");
}

TEST_CASE("local_slot_symbols: test_address_taken_zp.TestAddressTakenZp.test_address_taken_in_loop_body (call 1)") {
  check_frames_case(
      "local_slot_symbols",
      "{\"args\": [], \"kwargs\": {\"slot_names_by_fn\": {\"dict\": [[\"main\", []], [\"caller\", [\"__local_caller__0\", \"__local_caller__1\", \"__local_caller__row\"]], [\"finder\", [\"__local_finder__out\", \"__local_finder__0\"]]]}}}",
      "{\"dict\": [[\"main\", []], [\"caller\", [128, 129, 130]], [\"finder\", [131, 132]]]}",
      "{\"dict\": [[\"__local_caller__0\", 128], [\"__local_caller__1\", 129], [\"__local_caller__row\", 130], [\"__local_finder__out\", 131], [\"__local_finder__0\", 132]]}");
}

TEST_CASE("lower_data_load_address: test_address_taken_zp.TestAddressTakenZp.test_address_taken_in_loop_body (call 1)") {
  check_frames_case(
      "lower_data_load_address",
      "{\"args\": [], \"kwargs\": {\"slot_symbols\": {\"dict\": [[\"__local_caller__0\", 128], [\"__local_caller__1\", 129], [\"__local_caller__row\", 130], [\"__local_finder__out\", 131], [\"__local_finder__0\", 132]]}, \"byte_names\": {\"dict\": [[\"main\", {\"dict\": []}], [\"caller\", {\"dict\": [[128, \"__local_caller__0\"], [129, \"__local_caller__1\"], [130, \"__local_caller__row\"]]}], [\"finder\", {\"dict\": [[131, \"__local_finder__out\"], [132, \"__local_finder__0\"]]}]]}}}",
      "Program([\n"
      "  Function(\"finder\" true [\"@0.out\"] [\n"
      "    Label(\".finder@asm_ssa_block@0\")\n"
      "    Mov(Reg(A) Data(\"__local_finder__out\" 0) false)\n"
      "    Mov(Reg(X) Data(\"__local_finder__0\" 0) false)\n"
      "    Mov(Data(\"__local_finder__out\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Data(\"__local_finder__0\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Data(\"target\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(1))\n"
      "    Return(false false true)\n"
      "  ])\n"
      "  Function(\"caller\" true [] [\n"
      "    Label(\".caller@asm_ssa_block@0\")\n"
      "    LoadAddress(Data(\"__local_caller__row\" 0) Data(\"__local_caller__0\" 0))\n"
      "    Mov(Data(\"__local_caller__1\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Call(\"finder\" [\"A\" \"X\"] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    RotateLeft(Reg(A))\n"
      "    Mov(Reg(A) Data(\"__local_caller__0\" 0) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Branch(NE \".if_end@0\")\n"
      "    Label(\".caller@asm_ssa_block@1\")\n"
      "    Return(false false false)\n"
      "    Label(\".if_end@0\")\n"
      "    Mov(Data(\"__local_caller__row\" 0) Data(\"target\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@asm_ssa_block@0\")\n"
      "    Call(\"caller\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "  StaticVariable(\"target\" true [ZeroInit(1)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"finder\" true [\"@0.out\"] [\n"
      "    Label(\".finder@asm_ssa_block@0\")\n"
      "    Mov(Reg(A) Data(\"__local_finder__out\" 0) false)\n"
      "    Mov(Reg(X) Data(\"__local_finder__0\" 0) false)\n"
      "    Mov(Data(\"__local_finder__out\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Data(\"__local_finder__0\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Data(\"target\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(1))\n"
      "    Return(false false true)\n"
      "  ])\n"
      "  Function(\"caller\" true [] [\n"
      "    Label(\".caller@asm_ssa_block@0\")\n"
      "    Mov(ImmLabelLow(\"__local_caller__row\" 0) Data(\"__local_caller__0\" 0) false)\n"
      "    Mov(ImmLabelHigh(\"__local_caller__row\" 0) Data(\"__local_caller__1\" 0) false)\n"
      "    Mov(Data(\"__local_caller__1\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Call(\"finder\" [\"A\" \"X\"] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    RotateLeft(Reg(A))\n"
      "    Mov(Reg(A) Data(\"__local_caller__0\" 0) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Branch(NE \".if_end@0\")\n"
      "    Label(\".caller@asm_ssa_block@1\")\n"
      "    Return(false false false)\n"
      "    Label(\".if_end@0\")\n"
      "    Mov(Data(\"__local_caller__row\" 0) Data(\"target\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@asm_ssa_block@0\")\n"
      "    Call(\"caller\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "  StaticVariable(\"target\" true [ZeroInit(1)])\n"
      "])\n");
}

TEST_CASE("local_slot_names: test_address_taken_zp.TestAddressTakenZp.test_uchar_address_taken_lands_in_zp (call 1)") {
  check_frames_case(
      "local_slot_names",
      "{\"args\": [{\"dict\": [[\"writer\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"@0.out.reg\", 131]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}], \"kwargs\": {}}",
      "{\"dict\": [[\"writer\", [131, 132]]]}",
      "{\"dict\": [[\"writer\", [\"__local_writer__out\", \"__local_writer__0\"]]]}");
}

TEST_CASE("local_slot_names: test_address_taken_zp.TestAddressTakenZp.test_uchar_address_taken_lands_in_zp (call 2)") {
  check_frames_case(
      "local_slot_names",
      "{\"args\": [{\"dict\": [[\"caller\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%1.1\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}], \"kwargs\": {}}",
      "{\"dict\": [[\"caller\", [128, 129, 130]]]}",
      "{\"dict\": [[\"caller\", [\"__local_caller__0\", \"__local_caller__1\", \"__local_caller__2\"]]]}");
}

TEST_CASE("address_taken_assignments: test_address_taken_zp.TestAddressTakenZp.test_uchar_address_taken_lands_in_zp (call 1)") {
  check_frames_case(
      "address_taken_assignments",
      "{\"args\": [{\"dict\": [[\"main\", []], [\"caller\", [128, 129, 130]], [\"writer\", [131, 132]]]}, {\"dict\": [[\"writer\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"@0.out.reg\", 131]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"caller\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%1.1\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"writer\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(UChar)] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"caller\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.out.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"writer\" true [\"@0.out\"] [\n"
      "    Label(\".writer@asm_ssa_block@0\")\n"
      "    Mov(Reg(A) Data(\"__local_writer__out\" 0) false)\n"
      "    Mov(Reg(X) Data(\"__local_writer__0\" 0) false)\n"
      "    Mov(Data(\"__local_writer__out\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Data(\"__local_writer__0\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(66) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"caller\" true [] [\n"
      "    Label(\".caller@asm_ssa_block@0\")\n"
      "    LoadAddress(Pseudo(\"@1.x\" 0) Data(\"__local_caller__0\" 0))\n"
      "    Mov(Data(\"__local_caller__1\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Call(\"writer\" [\"A\" \"X\"] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@asm_ssa_block@0\")\n"
      "    Call(\"caller\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"writer\", {\"dict\": []}], [\"caller\", {\"dict\": [[\"@1.x\", 130]]}], [\"main\", {\"dict\": []}]]}");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.TestAddressTakenZp.test_uchar_address_taken_lands_in_zp (call 1)") {
  check_frames_case(
      "address_taken_slot_symbols",
      "{\"args\": [{\"dict\": []}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"writer\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(UChar)] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"caller\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.out.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "\"writer\"",
      "{\"dict\": []}");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.TestAddressTakenZp.test_uchar_address_taken_lands_in_zp (call 2)") {
  check_frames_case(
      "address_taken_slot_symbols",
      "{\"args\": [{\"dict\": [[\"@1.x\", 130]]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"writer\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(UChar)] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"caller\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.out.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "\"caller\"",
      "{\"dict\": [[\"@1.x\", \"__local_caller__x\"]]}");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.TestAddressTakenZp.test_uchar_address_taken_lands_in_zp (call 3)") {
  check_frames_case(
      "address_taken_slot_symbols",
      "{\"args\": [{\"dict\": []}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"writer\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(UChar)] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"caller\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.out.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "\"main\"",
      "{\"dict\": []}");
}

TEST_CASE("replace_pseudoregisters_bare_exit: test_address_taken_zp.TestAddressTakenZp.test_uchar_address_taken_lands_in_zp (call 1)") {
  check_frames_case(
      "replace_pseudoregisters_bare_exit",
      "{\"args\": [], \"kwargs\": {\"extra_statics\": {\"set\": []}, \"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"writer\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(UChar)] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"caller\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.out.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"types\": {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"colorings\": {\"dict\": [[\"writer\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"@0.out.reg\", 131]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"caller\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%1.1\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}, \"param_layouts\": {\"dict\": [[\"writer\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_writer__out_0\", \"__zpabi_writer__out_1\"], \"addrs\": [null, null], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"caller\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}, \"local_pools\": {\"dict\": [[\"main\", []], [\"caller\", [128, 129, 130]], [\"writer\", [131, 132]]]}, \"address_taken_assignments\": {\"dict\": [[\"writer\", {\"dict\": []}], [\"caller\", {\"dict\": [[\"@1.x\", 130]]}], [\"main\", {\"dict\": []}]]}, \"address_taken_symbols\": {\"dict\": [[\"writer\", {\"dict\": []}], [\"caller\", {\"dict\": [[\"@1.x\", \"__local_caller__x\"]]}], [\"main\", {\"dict\": []}]]}}}",
      "Program([\n"
      "  Function(\"writer\" true [\"@0.out\"] [\n"
      "    Label(\".writer@asm_ssa_block@0\")\n"
      "    Mov(Reg(A) Data(\"__local_writer__out\" 0) false)\n"
      "    Mov(Reg(X) Data(\"__local_writer__0\" 0) false)\n"
      "    Mov(Data(\"__local_writer__out\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Data(\"__local_writer__0\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(66) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"caller\" true [] [\n"
      "    Label(\".caller@asm_ssa_block@0\")\n"
      "    LoadAddress(Pseudo(\"@1.x\" 0) Data(\"__local_caller__0\" 0))\n"
      "    Mov(Data(\"__local_caller__1\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Call(\"writer\" [\"A\" \"X\"] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@asm_ssa_block@0\")\n"
      "    Call(\"caller\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"writer\\\" true [\\\"@0.out\\\"] [\\n    Label(\\\".writer@asm_ssa_block@0\\\")\\n    Mov(Reg(A) Data(\\\"__local_writer__out\\\" 0) false)\\n    Mov(Reg(X) Data(\\\"__local_writer__0\\\" 0) false)\\n    Mov(Data(\\\"__local_writer__out\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"DPTR\\\" 0) false)\\n    Mov(Data(\\\"__local_writer__0\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Data(\\\"DPTR\\\" 1) false)\\n    Mov(Imm(66) Reg(A) false)\\n    Mov(Reg(A) Indirect(0) false)\\n    Return(false false false)\\n  ])\\n  Function(\\\"caller\\\" true [] [\\n    Label(\\\".caller@asm_ssa_block@0\\\")\\n    LoadAddress(Data(\\\"__local_caller__x\\\" 0) Data(\\\"__local_caller__0\\\" 0))\\n    Mov(Data(\\\"__local_caller__1\\\" 0) Reg(A) false)\\n    Mov(Reg(A) Reg(X) false)\\n    Mov(Data(\\\"__local_caller__0\\\" 0) Reg(A) false)\\n    Call(\\\"writer\\\" [\\\"A\\\" \\\"X\\\"] [])\\n    Return(false false false)\\n  ])\\n  Function(\\\"main\\\" true [] [\\n    Label(\\\".main@asm_ssa_block@0\\\")\\n    Call(\\\"caller\\\" [] [])\\n    Mov(Imm(0) Reg(A) false)\\n    Mov(Reg(A) Reg(X) false)\\n    Mov(Imm(0) Reg(A) false)\\n    Return(true true false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"writer\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}], [\"caller\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}], [\"main\", {\"object\": \"passes.replace_pseudoregisters.FrameDims\", \"fields\": {\"arg_bytes\": 0, \"local_bytes\": 0, \"callee_saved_addrs\": []}}]]}]}");
}

TEST_CASE("local_slot_names: test_address_taken_zp.TestAddressTakenZp.test_uchar_address_taken_lands_in_zp (call 3)") {
  check_frames_case(
      "local_slot_names",
      "{\"args\": [{\"dict\": [[\"writer\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"@0.out.reg\", 131]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"caller\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%1.1\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], [\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}], \"kwargs\": {\"address_taken_assignments\": {\"dict\": [[\"writer\", {\"dict\": []}], [\"caller\", {\"dict\": [[\"@1.x\", 130]]}], [\"main\", {\"dict\": []}]]}, \"address_taken_symbols\": {\"dict\": [[\"writer\", {\"dict\": []}], [\"caller\", {\"dict\": [[\"@1.x\", \"__local_caller__x\"]]}], [\"main\", {\"dict\": []}]]}}}",
      "{\"dict\": [[\"main\", []], [\"caller\", [128, 129, 130]], [\"writer\", [131, 132]]]}",
      "{\"dict\": [[\"main\", []], [\"caller\", [\"__local_caller__0\", \"__local_caller__1\", \"__local_caller__x\"]], [\"writer\", [\"__local_writer__out\", \"__local_writer__0\"]]]}");
}

TEST_CASE("local_slot_symbols: test_address_taken_zp.TestAddressTakenZp.test_uchar_address_taken_lands_in_zp (call 1)") {
  check_frames_case(
      "local_slot_symbols",
      "{\"args\": [], \"kwargs\": {\"slot_names_by_fn\": {\"dict\": [[\"main\", []], [\"caller\", [\"__local_caller__0\", \"__local_caller__1\", \"__local_caller__x\"]], [\"writer\", [\"__local_writer__out\", \"__local_writer__0\"]]]}}}",
      "{\"dict\": [[\"main\", []], [\"caller\", [128, 129, 130]], [\"writer\", [131, 132]]]}",
      "{\"dict\": [[\"__local_caller__0\", 128], [\"__local_caller__1\", 129], [\"__local_caller__x\", 130], [\"__local_writer__out\", 131], [\"__local_writer__0\", 132]]}");
}

TEST_CASE("lower_data_load_address: test_address_taken_zp.TestAddressTakenZp.test_uchar_address_taken_lands_in_zp (call 1)") {
  check_frames_case(
      "lower_data_load_address",
      "{\"args\": [], \"kwargs\": {\"slot_symbols\": {\"dict\": [[\"__local_caller__0\", 128], [\"__local_caller__1\", 129], [\"__local_caller__x\", 130], [\"__local_writer__out\", 131], [\"__local_writer__0\", 132]]}, \"byte_names\": {\"dict\": [[\"main\", {\"dict\": []}], [\"caller\", {\"dict\": [[128, \"__local_caller__0\"], [129, \"__local_caller__1\"], [130, \"__local_caller__x\"]]}], [\"writer\", {\"dict\": [[131, \"__local_writer__out\"], [132, \"__local_writer__0\"]]}]]}}}",
      "Program([\n"
      "  Function(\"writer\" true [\"@0.out\"] [\n"
      "    Label(\".writer@asm_ssa_block@0\")\n"
      "    Mov(Reg(A) Data(\"__local_writer__out\" 0) false)\n"
      "    Mov(Reg(X) Data(\"__local_writer__0\" 0) false)\n"
      "    Mov(Data(\"__local_writer__out\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Data(\"__local_writer__0\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(66) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"caller\" true [] [\n"
      "    Label(\".caller@asm_ssa_block@0\")\n"
      "    LoadAddress(Data(\"__local_caller__x\" 0) Data(\"__local_caller__0\" 0))\n"
      "    Mov(Data(\"__local_caller__1\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Call(\"writer\" [\"A\" \"X\"] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@asm_ssa_block@0\")\n"
      "    Call(\"caller\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"writer\" true [\"@0.out\"] [\n"
      "    Label(\".writer@asm_ssa_block@0\")\n"
      "    Mov(Reg(A) Data(\"__local_writer__out\" 0) false)\n"
      "    Mov(Reg(X) Data(\"__local_writer__0\" 0) false)\n"
      "    Mov(Data(\"__local_writer__out\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Data(\"__local_writer__0\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(66) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"caller\" true [] [\n"
      "    Label(\".caller@asm_ssa_block@0\")\n"
      "    Mov(ImmLabelLow(\"__local_caller__x\" 0) Data(\"__local_caller__0\" 0) false)\n"
      "    Mov(ImmLabelHigh(\"__local_caller__x\" 0) Data(\"__local_caller__1\" 0) false)\n"
      "    Mov(Data(\"__local_caller__1\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Data(\"__local_caller__0\" 0) Reg(A) false)\n"
      "    Call(\"writer\" [\"A\" \"X\"] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@asm_ssa_block@0\")\n"
      "    Call(\"caller\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.TestSlotSymbols.test_falls_back_for_temp (call 1)") {
  check_frames_case(
      "address_taken_slot_symbols",
      "{\"args\": [{\"dict\": [[\"%5\", 144]]}, null, null], \"kwargs\": {}}",
      "\"caller\"",
      "{\"dict\": [[\"%5\", \"__local_caller__addr_%5\"]]}");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.TestSlotSymbols.test_renames_source_var (call 1)") {
  check_frames_case(
      "address_taken_slot_symbols",
      "{\"args\": [{\"dict\": [[\"@9.entity_row\", 143]]}, null, null], \"kwargs\": {}}",
      "\"caller\"",
      "{\"dict\": [[\"@9.entity_row\", \"__local_caller__entity_row\"]]}");
}

TEST_CASE("address_taken_slot_symbols: test_address_taken_zp.TestSlotSymbols.test_same_source_name_gets_a_suffix (call 1)") {
  check_frames_case(
      "address_taken_slot_symbols",
      "{\"args\": [{\"dict\": [[\"@3.compound_literal\", 136], [\"@5.compound_literal\", 140], [\"@7.compound_literal\", 144]]}, null, null], \"kwargs\": {}}",
      "\"f\"",
      "{\"dict\": [[\"@3.compound_literal\", \"__local_f__compound_literal\"], [\"@5.compound_literal\", \"__local_f__compound_literal@2\"], [\"@7.compound_literal\", \"__local_f__compound_literal@3\"]]}");
}

TEST_CASE("lower_data_load_address: test_lower_data_load_address.TestFrameSrcUntouched.test_frame_src_passes_through (call 1)") {
  check_frames_case(
      "lower_data_load_address",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Frame(4) ZP(128 0))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Frame(4) ZP(128 0))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_data_load_address: test_lower_data_load_address.TestMixedInstructionStream.test_split_in_middle (call 1)") {
  check_frames_case(
      "lower_data_load_address",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(7) ZP(130 0) false)\n"
      "    LoadAddress(Data(\"target\" 0) ZP(128 0))\n"
      "    Mov(Imm(9) ZP(131 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(7) ZP(130 0) false)\n"
      "    Mov(ImmLabelLow(\"target\" 0) ZP(128 0) false)\n"
      "    Mov(ImmLabelHigh(\"target\" 0) ZP(128 1) false)\n"
      "    Mov(Imm(9) ZP(131 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_data_load_address: test_lower_data_load_address.TestNoRewriteIdentity.test_function_without_load_address (call 1)") {
  check_frames_case(
      "lower_data_load_address",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(5) ZP(128 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(5) ZP(128 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_data_load_address: test_lower_data_load_address.TestNonFunctionTopLevels.test_static_variable_preserved (call 1)") {
  check_frames_case(
      "lower_data_load_address",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  StaticVariable(\"g\" false [IntInit(5)])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  StaticVariable(\"g\" false [IntInit(5)])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_data_load_address: test_lower_data_load_address.TestSplitDataSrc.test_data_src_data_dst (call 1)") {
  check_frames_case(
      "lower_data_load_address",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Data(\"entity_row_slot\" 0) Data(\"ptr_temp\" 0))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(ImmLabelLow(\"entity_row_slot\" 0) Data(\"ptr_temp\" 0) false)\n"
      "    Mov(ImmLabelHigh(\"entity_row_slot\" 0) Data(\"ptr_temp\" 1) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_data_load_address: test_lower_data_load_address.TestSplitDataSrc.test_data_src_dst_with_offset (call 1)") {
  check_frames_case(
      "lower_data_load_address",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Data(\"target\" 0) Data(\"multi\" 2))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(ImmLabelLow(\"target\" 0) Data(\"multi\" 2) false)\n"
      "    Mov(ImmLabelHigh(\"target\" 0) Data(\"multi\" 3) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_data_load_address: test_lower_data_load_address.TestSplitDataSrc.test_data_src_nonzero_offset_preserved (call 1)") {
  check_frames_case(
      "lower_data_load_address",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Data(\"arr\" 3) ZP(128 0))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(ImmLabelLow(\"arr\" 3) ZP(128 0) false)\n"
      "    Mov(ImmLabelHigh(\"arr\" 3) ZP(128 1) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_data_load_address: test_lower_data_load_address.TestSplitDataSrc.test_data_src_zp_dst (call 1)") {
  check_frames_case(
      "lower_data_load_address",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Data(\"target\" 0) ZP(128 0))\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(ImmLabelLow(\"target\" 0) ZP(128 0) false)\n"
      "    Mov(ImmLabelHigh(\"target\" 0) ZP(128 1) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: a LoadAddress destination seen nowhere else") {
  // The first walk reads a `LoadAddress`'s `src` and not its `dst`, so a
  // destination pseudo that no other instruction names has no slot.
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    LoadAddress(Pseudo(\"@1.a\" 0) Pseudo(\"%2\" 0))\n"
      "  ])\n"
      "])\n",
      "exception: ValueError: Pseudo('%2') is neither a static, a colored local, an ordinary "
      "local, nor a declared parameter; check tac_to_asm output");
}

TEST_CASE("replace_pseudoregisters: a Phi left after SSA destruction") {
  check_frames_case(
      "replace_pseudoregisters",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Phi(Pseudo(\"x\" 0) [])\n"
      "  ])\n"
      "])\n",
      "exception: TypeError: replace_pseudoregisters: Phi node leaked past SSA destruction");
}

TEST_CASE("local_slot_names: a source name taken twice, a wide value, and an address-taken local") {
  // Two TAC SSA versions of `x` on two bytes take one name, so the
  // second byte falls back to a number. `w` is two bytes wide, so its
  // bytes take `_0` and `_1`, and `%9` on the byte of `x` gives way to
  // the name from the source. The address-taken `buf` takes its own
  // symbol. The result is what Python's `compute_local_slot_names`
  // returned for these arguments.
  check_frames_case(
      "local_slot_names",
      "{\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"@1.x.0\", 128], [\"@1.x.1\", 129], [\"%3\", 130], [\"@2.w.b0.v1\", 131], [\"@2.w.b1.v0\", 132], [\"%9.b0.v0\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}], \"kwargs\": {\"address_taken_assignments\": {\"dict\": [[\"f\", {\"dict\": [[\"@4.buf\", 133]]}]]}, \"address_taken_symbols\": {\"dict\": [[\"f\", {\"dict\": [[\"@4.buf\", \"__local_f__buf\"]]}]]}}}",
      "{\"dict\": [[\"f\", [132, 128, 129, 130, 131, 133]]]}",
      "{\"dict\": [[\"f\", [\"__local_f__w_1\", \"__local_f__x\", \"__local_f__0\", \"__local_f__1\", \"__local_f__w_0\", \"__local_f__buf\"]]]}");
}

namespace {

// `f`'s one pool byte holds `%p`, so the address-taken `@1.x` doesn't
// fit and gets a frame slot. The coloring counted `@1.x` as a zero-page
// byte when it kept values in Y, so the case is an error when one is
// there (tests/test_address_taken_zp.py's `TestNoFrameSlotUnderY`).
constexpr const char* kNoRoomForTaken =
    "Program([\n"
    "  Function(\"f\" true [] [\n"
    "    LoadAddress(Pseudo(\"@1.x\" 0) Pseudo(\"%p\" 0))\n"
    "    Return(false false false)\n"
    "  ])\n"
    "])\n";

std::string no_room_args(const char* letter) {
  return std::string(
             "{\"args\": [{\"dict\": [[\"f\", [128]]]}, {\"dict\": [[\"f\", {\"object\": "
             "\"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": "
             "{\"dict\": [[\"%p\", 128]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": "
             "\"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, "
             "\"hwreg_assignments\": {\"dict\": [[\"%i\", \"") +
         letter + "\"]]}}}]]}, null, null], \"kwargs\": {}}";
}

}  // namespace

TEST_CASE("address_taken_assignments: a local that doesn't fit while Y holds a value") {
  check_frames_case("address_taken_assignments", no_room_args("X"), kNoRoomForTaken,
                    "{\"dict\": [[\"f\", {\"dict\": []}]]}");
  check_frames_case("address_taken_assignments", no_room_args("Y"), kNoRoomForTaken,
                    "exception: AssertionError: function 'f': the address-taken locals '@1.x' "
                    "didn't fit in the function's pool, and the coloring keeps a value in Y, "
                    "which an access to their frame slots would overwrite");
}

TEST_CASE("address_taken_assignments: two locals that don't fit while Y holds a value") {
  check_frames_case("address_taken_assignments", no_room_args("Y"),
                    "Program([\n"
                    "  Function(\"f\" true [] [\n"
                    "    LoadAddress(Pseudo(\"@1.x\" 0) Pseudo(\"%p\" 0))\n"
                    "    LoadAddress(Pseudo(\"@2.y\" 0) Pseudo(\"%p\" 0))\n"
                    "    Return(false false false)\n"
                    "  ])\n"
                    "])\n",
                    "exception: AssertionError: function 'f': the address-taken locals '@1.x', "
                    "'@2.y' didn't fit in the function's pool");
}

// No C object has no bytes, so a local of size 0 is a bug in the
// compiler, which the allocation reports rather than giving it the
// first free byte.
TEST_CASE("address_taken_assignments: a local of no bytes") {
  check_frames_case(
      "address_taken_assignments",
      R"~({"args": [{"dict": [["f", [128]]]}, {"dict": []}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["@1.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(UChar 0)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}], "kwargs": {}})~",
      kNoRoomForTaken,
      "exception: AssertionError: address_taken_zp: the address-taken local '@1.x' has no "
      "bytes");
}
