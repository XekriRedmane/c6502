// The calls that the tests of tests/test_dispatch_pointer_array.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("dispatch_const_pointer_arrays: test_dispatch_pointer_array.py::TestDispatchPointerArray::test_no_symbols_skipped (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass dispatch_const_pointer_arrays\n"
      "source tests/test_dispatch_pointer_array.py::TestDispatchPointerArray::test_no_symbols_skipped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"arr\" false Pointer [AddressInit(\"sub_a\" 0) AddressInit(\"sub_b\" 0) AddressInit(\"sub_c\" 0)])\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(LeftShift Var(\"%i\") Constant(ConstInt(1)) Var(\"%scaled\"))\n"
      "    IndexedLoad(\"arr\" Var(\"%scaled\") Var(\"%ptr\") false 0 0)\n"
      "    IndirectIndexedLoad(Var(\"%ptr\") Var(\"%j\") Var(\"%val\") false)\n"
      "    Ret(Var(\"%val\"))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"arr\" false Pointer [AddressInit(\"sub_a\" 0) AddressInit(\"sub_b\" 0) AddressInit(\"sub_c\" 0)])\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(LeftShift Var(\"%i\") Constant(ConstInt(1)) Var(\"%scaled\"))\n"
      "    IndexedLoad(\"arr\" Var(\"%scaled\") Var(\"%ptr\") false 0 0)\n"
      "    IndirectIndexedLoad(Var(\"%ptr\") Var(\"%j\") Var(\"%val\") false)\n"
      "    Ret(Var(\"%val\"))\n"
      "  ])\n"
      "])\n");
}
