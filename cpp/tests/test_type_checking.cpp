// The tests of tests/test_type_checking.py that compiled a program
// through `--tac`.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

}  // namespace

TEST_CASE("type_checking: a well-typed program goes through to the TAC") {
  const std::string out = suite::run_source(suite::Stage::kTac, "int main(void) { return 42; }");
  CHECK(suite::contains(out, "Ret("));
}

TEST_CASE("type_checking: an ill-typed program is rejected at the TAC stage") {
  const suite::Error e =
      suite::compile_error(suite::Stage::kTac, "int main(void) { int x; x(); return 0; }");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
}
