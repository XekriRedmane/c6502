// The rules of diagnostics.py's module docstring: a warning goes to the
// innermost open block, a nested block keeps its warnings from the
// outer one, and a warning outside every block is dropped. The Python
// tests check them through the `_Bool` warning (tests/test_c23_bool.py).
#include <vector>

#include "diagnostics.h"
#include "doctest.h"

namespace D = c6502::diagnostics;

TEST_CASE("a warning outside every block is dropped") {
  D::warn("line 1", "nobody hears this");
  std::vector<D::CompileWarning> found;
  {
    D::Collecting block(found);
  }
  CHECK(found.empty());
}

TEST_CASE("warnings are collected in the order they were issued") {
  std::vector<D::CompileWarning> found;
  {
    D::Collecting block(found);
    D::warn("line 3", "first");
    D::warn("line 2 of foo.h", "second");
  }
  REQUIRE(found.size() == 2);
  CHECK(found[0].str() == "line 3: first");
  CHECK(found[1].str() == "line 2 of foo.h: second");
}

TEST_CASE("a nested block takes its warnings for itself") {
  std::vector<D::CompileWarning> outer;
  std::vector<D::CompileWarning> inner;
  {
    D::Collecting a(outer);
    D::warn("line 1", "outer");
    {
      D::Collecting b(inner);
      D::warn("line 2", "inner");
    }
    D::warn("line 3", "outer again");
  }
  CHECK(outer == std::vector<D::CompileWarning>{{"line 1", "outer"}, {"line 3", "outer again"}});
  CHECK(inner == std::vector<D::CompileWarning>{{"line 2", "inner"}});
}
