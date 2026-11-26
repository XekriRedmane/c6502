// Cases of the type checker's rules that the recorded calls of
// test_type_checking_calls.cpp don't reach, run on C source through the
// front end without the preprocessor.
#include <string>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "doctest.h"
#include "errors.h"
#include "passes/front_end.h"
#include "passes/type_checker.h"
#include "passes/type_checking_types.h"

namespace {

namespace ir = c6502::c99_ast;
namespace tc = c6502::passes::type_checking;

// Returns "<Python class>: <message>" of the error the front end gives
// for `src`, or "no error".
std::string error_of(const std::string& src) {
  c6502::passes::FrontEndOptions options;
  options.preprocess = false;
  std::string err;
  try {
    c6502::passes::front_end(src, options, err);
  } catch (const c6502::CompileError& e) {
    return e.python_type() + ": " + e.what();
  }
  return "no error";
}

}  // namespace

TEST_CASE("type checker: &* of an expression is the operand's decayed pointer") {
  SUBCASE("an element of a two-dimensional array") {
    CHECK(error_of("int h(void) { int a[2][3]; int *q = &*a[1]; return *q; }") == "no error");
  }
  SUBCASE("an array") {
    CHECK(error_of("int h(void) { int a[3]; int *q = &*a; return *q; }") == "no error");
  }
  SUBCASE("a compound literal of array type") {
    CHECK(error_of("int g(void) { int *p = &*(int[]){1, 2}; return *p; }") == "no error");
  }
  SUBCASE("a function") {
    CHECK(error_of("int f(void); int g(void) { int (*p)(void) = &*f; return p(); }") ==
          "no error");
  }
  SUBCASE("a pointer") {
    CHECK(error_of("int g(int *p) { int *q = &*p; return *q; }") == "no error");
  }
  SUBCASE("an integer") {
    CHECK(error_of("int g(void) { int x = 0; int *p = &*x; return *p; }") ==
          "TypeCheckError: unary '*' requires a pointer operand, got Int");
  }
}

TEST_CASE("type checker: a constant initializer can't convert an infinity or a NaN to an integer") {
  CHECK(error_of("static int x = 1e309;") ==
        "TypeCheckError: a constant initializer converts an infinity to the integer type int, "
        "which can't represent it (C99 §6.3.1.4p1, §6.6p4)");
  CHECK(error_of("static unsigned char x = -1e309;") ==
        "TypeCheckError: a constant initializer converts an infinity to the integer type "
        "unsigned char, which can't represent it (C99 §6.3.1.4p1, §6.6p4)");
  CHECK(error_of("static long x = (long)(0.0 / 0.0);") ==
        "TypeCheckError: a constant initializer converts a NaN to the integer type long, which "
        "can't represent it (C99 §6.3.1.4p1, §6.6p4)");
  CHECK(error_of("static int x = 1.5e4;") == "no error");
}

TEST_CASE("type checker: a ConstChar initializes as a signed char") {
  const ir::Type_const minus_one = ir::ConstChar{.value = -1};
  CHECK(static_cast<long long>(tc::coerce_init_const_value(minus_one, ir::Int{})) == -1);
  CHECK(static_cast<long long>(tc::coerce_init_const_value(minus_one, ir::Long{})) == -1);
  CHECK(static_cast<long long>(tc::coerce_init_const_value(minus_one, ir::UInt{})) == 0xFFFF);
  CHECK(static_cast<long long>(tc::coerce_init_const_value(minus_one, ir::Float{})) == 0xBF800000);
  CHECK(static_cast<long long>(tc::coerce_init_const_value(minus_one, ir::Bool{})) == 1);
  CHECK(tc::constant_type(minus_one) == ir::Type_data_type(ir::SChar{}));
}

TEST_CASE("type checker: the usual arithmetic conversions of two integer types") {
  using T = ir::Type_data_type;
  auto common = [](const T& a, const T& b) { return c6502::asdl::py_repr(tc::common_type(a, b)); };
  CHECK(common(ir::Int{}, ir::Int{}) == "Int()");
  CHECK(common(ir::Int{}, ir::UInt{}) == "UInt()");
  CHECK(common(ir::UInt{}, ir::Int{}) == "UInt()");
  CHECK(common(ir::UInt{}, ir::Long{}) == "Long()");
  CHECK(common(ir::Long{}, ir::ULong{}) == "ULong()");
  CHECK(common(ir::ULong{}, ir::LongLong{}) == "LongLong()");
  CHECK(common(ir::LongLong{}, ir::ULongLong{}) == "ULongLong()");
  CHECK(common(ir::Int{}, ir::ULongLong{}) == "ULongLong()");
  CHECK(common(ir::UInt{}, ir::LongLong{}) == "LongLong()");
  CHECK(common(ir::Const{.referenced_type = T(ir::Long{})}, ir::UInt{}) == "Long()");
  CHECK(common(ir::BitInt{.width = 24, .is_signed = true}, ir::UInt{}) ==
        "BitInt(width=24, is_signed=True)");
  CHECK(common(ir::BitInt{.width = 16, .is_signed = true}, ir::UInt{}) == "UInt()");
  CHECK(common(ir::BitInt{.width = 16, .is_signed = false}, ir::Int{}) == "UInt()");
  CHECK(common(ir::Int{}, ir::Double{}) == "Double()");
  CHECK(common(ir::Float{}, ir::ULongLong{}) == "Float()");
}
