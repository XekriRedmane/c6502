// Calls of `select_abi` and `fixed_address_layouts` that end in errors
// the recorded calls of tests/test_abi_stage_calls.cpp don't reach,
// written by hand.
#include <string>

#include "doctest.h"
#include "stage_case.h"

namespace {

constexpr const char* kClassicContext =
    R"~({"layout":[0,2,4,32,36],"user_zp_globals":[],"user_zp_bytes":[]})~";

constexpr const char* kTypes =
    R"~({"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}})~";

constexpr const char* kMainDecl =
    R"~(  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n)~";

// Returns the arguments of a call of a stage on a c99 program of the
// function declaration `decl` (a line of the c99 dump, with the JSON
// string's escapes) and `main`, with the keyword arguments `kwargs`.
std::string c99_args(const std::string& decl, const char* kwargs = "{}") {
  return std::string(R"~({"args": [{"asdl": "c99_ast", "dump": "Program([\n)~") + decl +
         kMainDecl + R"~(])\n"}, )~" + kTypes + R"~(], "kwargs": )~" + kwargs + "}";
}

}  // namespace

TEST_CASE("select_abi: an unnamed register parameter is named by its position") {
  c6502::testing::check_stage_call(
      "select_abi",
      c99_args(
          R"~(  FunctionDecl(function_decl(\"f\" [\"\"] Block([]) FunType([Pointer(UChar)] Void) None None [\"auto\"] false None [] None false))\n)~"),
      kClassicContext, false,
      "Program([\n"
      "  Function(\"f\" true [\"\"] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` parameter `#1` is a pointer and can't be "
      "passed in a register\n",
      "");
}

// A function with a `register` parameter that can't be zp_abi for
// another reason gets the reason after what it asks for, even when its
// name has "register" in it.
TEST_CASE("select_abi: a recursive function with a register parameter") {
  c6502::testing::check_stage_call(
      "select_abi",
      c99_args(
          R"~(  FunctionDecl(function_decl(\"register_isr\" [\"@0.x\"] Block([]) FunType([UChar] UChar) None None [\"auto\"] false None [] None false))\n)~"),
      kClassicContext, false,
      "Program([\n"
      "  Function(\"register_isr\" true [\"@0.x\"] [\n"
      "    FunctionCall(\"register_isr\" [Var(\"@0.x\")] Var(\"%0\") false false)\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `register_isr` declares a `register` parameter "
      "or local but isn't eligible for the ZP-passing ABI: function `register_isr` declared "
      "`[[c6502::zp_abi]]` but is reachable from itself through the static call graph (direct "
      "or mutual recursion); a recursive call would clobber the parameter ZP slots before the "
      "outer activation returned\n",
      "");
}

TEST_CASE("fixed_address_layouts: a fixed-address declaration without a function type") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      c99_args(
          R"~(  FunctionDecl(function_decl(\"f\" [] None Int None \"zp_abi\" [] false 53248 [] None false))\n)~",
          R"~({"pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}})~"),
      kClassicContext, false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AssertionError: the declaration of `f` has no FunType\n", "");
}
