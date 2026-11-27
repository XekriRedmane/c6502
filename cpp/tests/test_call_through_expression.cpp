// The tests of tests/test_call_through_expression.py that compile a
// source: what the type checker rejects in a call through an expression,
// and `[[c6502::save_hardware_stack]]` on a call through a table.
#include <string>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

suite::Error rejected(const std::string& src) {
  suite::Options o;
  o.optimize = true;
  return suite::compile_error(suite::Stage::kCodegen, src, o);
}

}  // namespace

TEST_CASE("call_through_expression: a callee that isn't a function is rejected") {
  const std::vector<std::pair<std::string, std::string>> cases = {
      {"int x;", "(*&x)()"},
      // `&fp` is a pointer to the pointer, which isn't a function.
      {"int (*fp)(void);", "(&fp)()"},
      {"int a[2];", "a[0]()"},
  };
  for (const auto& [decls, call] : cases) {
    INFO(call);
    const suite::Error e = rejected(decls + " int main(void) { return " + call + "; }");
    CHECK(suite::is_a(e.type, "TypeCheckError"));
    CHECK_MESSAGE(suite::search("not a function", e.message), e.message);
  }
}

TEST_CASE("call_through_expression: the number of arguments is checked") {
  const suite::Error e = rejected("int (*t[1])(int); int main(void) { return t[0](1, 2); }");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK_MESSAGE(suite::search("called with 2", e.message), e.message);
}

TEST_CASE("call_through_expression: save_hardware_stack on a call through a table is rejected") {
  const suite::Error e = rejected(R"(
            int (*t[1])(int);
            int main(void) {
                [[c6502::save_hardware_stack]] return t[0](1);
            }
        )");
  CHECK(suite::is_a(e.type, "RecursionSaveError"));
  CHECK_MESSAGE(suite::search("function pointer", e.message), e.message);
}
