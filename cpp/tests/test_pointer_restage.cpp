// tests/test_pointer_member_access.py's TestRestageRule, ported by hand,
// one case per Python test: `scalar_at` and `restage_store` on the
// Python tests' table of a struct and a union.
#include <string>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "doctest.h"
#include "passes/pointer_restage.h"
#include "passes/type_checking.h"

namespace {

namespace pr = c6502::passes::pointer_restage;
using c6502::c99_ast::Type_data_type;

Type_data_type type(const std::string& dump) {
  return c6502::asdl::load<Type_data_type>(dump);
}

// `struct s { unsigned char a[3]; unsigned g; int w[2]; unsigned char *p;
// struct s *next; }` and `union u { unsigned w; unsigned char b; }`.
c6502::passes::TypeTable types() {
  c6502::passes::TypeTable t;
  t.insert_or_assign("s", {.tag = "s",
                           .is_union = false,
                           .members = {{"a", type("Array(UChar 3)"), 0},
                                       {"g", type("UInt"), 3},
                                       {"w", type("Array(Int 2)"), 5},
                                       {"p", type("Pointer(UChar)"), 9},
                                       {"next", type("Pointer(Structure(\"s\"))"), 11}},
                           .size = 13,
                           .complete = true});
  t.insert_or_assign("u", {.tag = "u",
                           .is_union = true,
                           .members = {{"w", type("UInt"), 0}, {"b", type("UChar"), 0}},
                           .size = 2,
                           .complete = true});
  return t;
}

// The `repr` of what `scalar_at` gives, or "None".
std::string scalar(const std::string& t, c6502::asdl::Int k, c6502::asdl::Int n) {
  const auto tt = types();
  const Type_data_type object = type(t);
  const Type_data_type* got = pr::scalar_at(object, k, n, &tt);
  return got == nullptr ? "None" : c6502::asdl::py_repr(*got);
}

bool restage(const std::string& ptr, c6502::asdl::Int offset, c6502::asdl::Int n) {
  const auto tt = types();
  const Type_data_type p = type(ptr);
  return pr::restage_store(&p, offset, n, &tt);
}

const char* const kPs = "Pointer(Structure(\"s\"))";

}  // namespace

TEST_CASE("pointer_restage: TestRestageRule::test_scalar_at_member_and_element") {
  CHECK(scalar("Structure(\"s\")", 3, 2) == "UInt()");
  CHECK(scalar("Structure(\"s\")", 7, 2) == "Int()");
  CHECK(scalar("Structure(\"s\")", 4, 2) == "None");  // straddles g
  CHECK(scalar("Structure(\"s\")", 3, 1) == "None");  // half of g
  CHECK(scalar("Union(\"u\")", 0, 2) == "None");
}

TEST_CASE("pointer_restage: TestRestageRule::test_integer_and_other_pointer_members_restage") {
  CHECK(restage(kPs, 3, 2));
  CHECK(restage(kPs, 5, 2));
  CHECK(restage(kPs, 9, 2));
}

TEST_CASE("pointer_restage: TestRestageRule::test_member_of_the_pointer_s_own_type_doesnt") {
  // `P->next = q` may write P itself when P points at its own bytes,
  // and that's defined C.
  CHECK_FALSE(restage(kPs, 11, 2));
}

TEST_CASE("pointer_restage: TestRestageRule::test_bytes_and_unknown_pointees_dont") {
  CHECK_FALSE(restage(kPs, 0, 1));
  CHECK_FALSE(restage("Pointer(Void)", 0, 2));
  CHECK_FALSE(restage("UInt", 0, 2));
}
