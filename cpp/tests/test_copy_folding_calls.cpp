// The calls that the tests of tests/test_copy_folding.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.py::TestCopyFoldingObservableNames::test_static_intermediate_not_fused (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_copies_in_fixedpoint\n"
      "source tests/test_copy_folding.py::TestCopyFoldingObservableNames::test_static_intermediate_not_fused\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(7)) Var(\"g\"))\n"
      "  Copy(Var(\"g\") Var(\"h\"))\n"
      "  Ret(Var(\"h\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(7)) Var(\"g\"))\n"
      "  Copy(Var(\"g\") Var(\"h\"))\n"
      "  Ret(Var(\"h\"))\n"
      "])\n");
}
