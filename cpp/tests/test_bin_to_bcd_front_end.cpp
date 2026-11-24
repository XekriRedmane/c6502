// tests/test_bin_to_bcd.py's TestFrontEnd, ported by hand to the C++
// front end, one case per Python test. The sources declare the
// <stdint.h> types they use with the header's typedefs, since the
// cases don't preprocess. Where the Python test checked the types of
// the builtin's call and of its argument, the case checks the dump of
// the whole call, which Python's front end gives for the same source.
#include <cstddef>
#include <string>
#include <variant>
#include <vector>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "doctest.h"
#include "errors.h"
#include "passes/front_end.h"
#include "passes/type_checking.h"

namespace {

const char* const kPrelude =
    "typedef unsigned char uint8_t;\n"
    "typedef int int16_t;\n"
    "typedef unsigned int uint16_t;\n"
    "typedef unsigned long uint32_t;\n";

// Runs the front end on `src` after the prelude, without the
// preprocessor.
c6502::passes::FrontEndResult check(const std::string& src) {
  c6502::passes::FrontEndOptions options;
  options.preprocess = false;
  std::string err;
  return c6502::passes::front_end(kPrelude + src, options, err);
}

// Returns the dump of the first call of `__c6502_bin_to_bcd` in the
// dump `text`, from its constructor to its closing parenthesis.
std::string first_call(const std::string& text) {
  const std::size_t start = text.find("FunctionCall(\"__c6502_bin_to_bcd\"");
  REQUIRE(start != std::string::npos);
  int depth = 0;
  bool quoted = false;
  for (std::size_t i = start; i < text.size(); ++i) {
    const char c = text[i];
    if (quoted) {
      if (c == '\\') ++i;
      else if (c == '"') quoted = false;
    } else if (c == '"') {
      quoted = true;
    } else if (c == '(') {
      ++depth;
    } else if (c == ')' && --depth == 0) {
      return text.substr(start, i + 1 - start);
    }
  }
  FAIL("unbalanced dump");
  return "";
}

// Returns "<Python class>: <message>" of the error the front end
// gives for `src`, or "no error".
std::string error_of(const std::string& src) {
  try {
    check(src);
  } catch (const c6502::CompileError& e) {
    return e.python_type() + ": " + e.what();
  }
  return "no error";
}

}  // namespace

TEST_CASE("bin_to_bcd: test_bin_to_bcd.py::TestFrontEnd::test_result_type") {
  // The call is an `unsigned long`, and its argument is converted to
  // `unsigned int`.
  {
    CAPTURE("uint8_t a;");
    const auto r = check("uint8_t a; unsigned long g(void) { return __c6502_bin_to_bcd(a); }");
    CHECK(first_call(c6502::asdl::dump(r.prog)) ==
          "FunctionCall(\"__c6502_bin_to_bcd\" [Cast(UInt Var(\"a\" UChar) UInt)] ULong false)");
  }
  {
    CAPTURE("int16_t a;");
    const auto r = check("int16_t a; unsigned long g(void) { return __c6502_bin_to_bcd(a); }");
    CHECK(first_call(c6502::asdl::dump(r.prog)) ==
          "FunctionCall(\"__c6502_bin_to_bcd\" [Cast(UInt Var(\"a\" Int) UInt)] ULong false)");
  }
  {
    CAPTURE("uint32_t a;");
    const auto r = check("uint32_t a; unsigned long g(void) { return __c6502_bin_to_bcd(a); }");
    CHECK(first_call(c6502::asdl::dump(r.prog)) ==
          "FunctionCall(\"__c6502_bin_to_bcd\" [Cast(UInt Var(\"a\" ULong) UInt)] ULong false)");
  }
  {
    CAPTURE("bool a;");
    const auto r = check("bool a; unsigned long g(void) { return __c6502_bin_to_bcd(a); }");
    CHECK(first_call(c6502::asdl::dump(r.prog)) ==
          "FunctionCall(\"__c6502_bin_to_bcd\" [Cast(UInt Var(\"a\" Bool) UInt)] ULong false)");
  }
  {
    CAPTURE("char a;");
    const auto r = check("char a; unsigned long g(void) { return __c6502_bin_to_bcd(a); }");
    CHECK(first_call(c6502::asdl::dump(r.prog)) ==
          "FunctionCall(\"__c6502_bin_to_bcd\" [Cast(UInt Var(\"a\" Char) UInt)] ULong false)");
  }
  {
    CAPTURE("_BitInt(12) a;");
    const auto r = check("_BitInt(12) a; unsigned long g(void) { return __c6502_bin_to_bcd(a); }");
    CHECK(first_call(c6502::asdl::dump(r.prog)) ==
          "FunctionCall(\"__c6502_bin_to_bcd\" [Cast(UInt Var(\"a\" BitInt(12 true)) UInt)] ULong false)");
  }
  {
    CAPTURE("enum { A } a;");
    const auto r = check("enum { A } a; unsigned long g(void) { return __c6502_bin_to_bcd(a); }");
    CHECK(first_call(c6502::asdl::dump(r.prog)) ==
          "FunctionCall(\"__c6502_bin_to_bcd\" [Cast(UInt Var(\"a\" Int) UInt)] ULong false)");
  }
}

TEST_CASE("bin_to_bcd: test_bin_to_bcd.py::TestFrontEnd::test_type_errors") {
  CHECK(error_of("double d; void f(void) { __c6502_bin_to_bcd(d); }") ==
        "TypeCheckError: the argument of __c6502_bin_to_bcd has type double; it converts an integer to packed BCD, and takes an argument of integer type");
  CHECK(error_of("uint8_t *p; void f(void) { __c6502_bin_to_bcd(p); }") ==
        "TypeCheckError: the argument of __c6502_bin_to_bcd has type unsigned char *; it converts an integer to packed BCD, and takes an argument of integer type");
  CHECK(error_of("uint8_t t[2]; void f(void) { __c6502_bin_to_bcd(t); }") ==
        "TypeCheckError: the argument of __c6502_bin_to_bcd has type unsigned char *; it converts an integer to packed BCD, and takes an argument of integer type");
  CHECK(error_of("struct s { int x; } v; void f(void) { __c6502_bin_to_bcd(v); }") ==
        "TypeCheckError: the argument of __c6502_bin_to_bcd has type struct s; it converts an integer to packed BCD, and takes an argument of integer type");
  CHECK(error_of(" void f(void) { __c6502_bin_to_bcd(); }") ==
        "TypeCheckError: __c6502_bin_to_bcd takes 1 arguments, got 0");
  CHECK(error_of(" void f(void) { __c6502_bin_to_bcd(1, 2); }") ==
        "TypeCheckError: __c6502_bin_to_bcd takes 1 arguments, got 2");
}

TEST_CASE("bin_to_bcd: test_bin_to_bcd.py::TestFrontEnd::test_names_are_reserved") {
  CHECK(error_of("unsigned long __c6502_bin_to_bcd(unsigned x);") ==
        "IdentifierResolutionError: __c6502_bin_to_bcd is a builtin and can't be declared");
  CHECK(error_of("void f(void) { int __c6502_bin_to_bcd = 1; }") ==
        "IdentifierResolutionError: __c6502_bin_to_bcd is a builtin and can't be declared");
  CHECK(error_of("void *p = __c6502_bin_to_bcd;") ==
        "IdentifierResolutionError: __c6502_bin_to_bcd is a builtin, which can only be called");
}

TEST_CASE("bin_to_bcd: test_bin_to_bcd.py::TestFrontEnd::test_constant_argument_makes_a_constant_expression") {
  const auto r = check("static uint32_t s = __c6502_bin_to_bcd(12345);uint32_t t = __c6502_bin_to_bcd(-1);uint32_t u = __c6502_bin_to_bcd(70000L);int a[__c6502_bin_to_bcd(10)];int f(uint8_t k) { switch (k) { case __c6502_bin_to_bcd(42): return 1; } return sizeof a; }");
  const auto value = [&](const char* name) {
    const auto& attrs = std::get<c6502::passes::StaticAttr>(r.symbols.at(name).attrs);
    return std::get<c6502::passes::Initial>(attrs.initial_value).value.integer;
  };
  CHECK(value("s") == 0x12345);
  CHECK(value("t") == 0x65535);
  CHECK(value("u") == 0x04464);
  CHECK(std::get<c6502::c99_ast::Array>(r.symbols.at("a").type).size == 0x10);
}
