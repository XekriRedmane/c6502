// The calls that the tests of tests/test_linker_script.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_linker_script.py::TestSectionAttribute::test_parsed_on_declarations (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_linker_script.py::TestSectionAttribute::test_parsed_on_declarations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::section(\\\".x\\\")]] void f(void) {}\\nint g [[c6502::section(\\\".y\\\")]];\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([]) FunType([] Void) None None [] false None [] \".x\" false))\n"
      "  VarDecl(var_decl(\"g\" None Int None None None \".y\" false))\n"
      "])\n");
}
