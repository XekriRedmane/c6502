// The calls that the tests of tests/test_recognize_indexed_store.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_recognize_indexed_store: test_recognize_indexed_store.py::TestRecognizeIndexedStoreUnit::test_no_symbols_is_noop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indexed_store\n"
      "source tests/test_recognize_indexed_store.py::TestRecognizeIndexedStoreUnit::test_no_symbols_is_noop\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"col\") Var(\"%ext\"))\n"
      "  Binary(Add Constant(ConstUInt(16384)) Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"col\") Var(\"%ext\"))\n"
      "  Binary(Add Constant(ConstUInt(16384)) Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n");
}
