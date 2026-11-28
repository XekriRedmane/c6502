// Tests of asdlgen (cpp/tools/asdlgen.h), the generator of the IR
// classes in cpp/generated/.
#include <map>
#include <string>

#include "asdlgen.h"
#include "compile_case.h"
#include "doctest.h"

namespace {

using namespace c6502;

std::map<std::string, std::string> generate(const std::string& text) {
  std::map<std::string, std::string> out;
  for (auto& [name, body] : asdlgen::generate_cpp(asdlgen::parse(text), "t.asdl", "t_ast")) {
    out[name] = body;
  }
  return out;
}

bool contains(const std::string& text, const std::string& part) {
  return text.find(part) != std::string::npos;
}

TEST_CASE("regenerating the IR classes gives the checked-in files") {
  for (const std::string stem : {"asm", "asm2", "tac", "c99"}) {
    CAPTURE(stem);
    const std::string source = stem + ".asdl";
    const auto files = asdlgen::generate_cpp(asdlgen::parse(suite::read_repo_file(source)),
                                             source, asdlgen::cpp_module_name(source));
    REQUIRE(files.size() == 2);
    CHECK(files[0].first == stem + "_ast.h");
    CHECK(files[1].first == stem + "_ast.cpp");
    for (const auto& [name, text] : files) {
      CAPTURE(name);
      CHECK(suite::read_repo_file("cpp/generated/" + name) == text);
    }
  }
}

TEST_CASE("a module's dependencies come first") {
  const std::string header = generate(
      "module T {\n"
      "    program = Program(stmt* body)\n"
      "    stmt = Assign(identifier name, expr value) | Pass\n"
      "    expr = Num(int n) | Neg(expr operand) | Paren(expr? inner)\n"
      "}\n")["t_ast.h"];
  CHECK(header.find("using Type_expr =") < header.find("using Type_stmt ="));
  CHECK(header.find("using Type_stmt =") < header.find("using Type_program ="));
  // A field in a cycle with its struct is boxed, an optional one inside
  // std::optional, and one of an earlier type is held by value.
  CHECK(contains(header, "::c6502::asdl::Box<Type_expr> operand;"));
  CHECK(contains(header, "std::optional<::c6502::asdl::Box<Type_expr>> inner;"));
  CHECK(contains(header, "  Type_expr value;"));
  CHECK(contains(header, "std::vector<Type_stmt> body;"));
  CHECK(contains(header, "enum class Kind_expr : std::uint8_t {"));
}

TEST_CASE("a field named after a C++ keyword gets an underscore") {
  const std::string header =
      generate("module T { c = Constant(const, int default) const = K }")["t_ast.h"];
  CHECK(contains(header, "Type_const const_;"));
  CHECK(contains(header, "::c6502::asdl::Int default_ = 0;"));
}

TEST_CASE("a product type is a struct") {
  auto files = generate("module T { member = (identifier name, int? size) }");
  CHECK(contains(files["t_ast.h"], "struct Type_member {"));
  CHECK(contains(files["t_ast.h"], "std::optional<::c6502::asdl::Int> size;"));
  CHECK(contains(files["t_ast.cpp"], "r.expect_name(\"member\");"));
}

// Each struct gets a `pretty`, and a constructor's fields come in the
// order of `dataclasses.fields`, which puts the sum type's attributes,
// on the base class, first.
TEST_CASE("pretty follows the Python classes") {
  auto files = generate(
      "module T {\n"
      "    e = A(int x, e* items) | B attributes (string line)\n"
      "}\n");
  const std::string& header = files["t_ast.h"];
  const std::string& source = files["t_ast.cpp"];
  for (const std::string name : {"Type_e", "A", "B"}) {
    CAPTURE(name);
    CHECK(contains(header, "void pretty(std::string& out, const " + name + "& v, int depth);"));
  }
  std::string body = source.substr(source.find("void pretty(std::string& out, const A&"));
  body = body.substr(0, body.find("\n}\n"));
  CHECK(body.find("\"line=\"") < body.find("\"x=\""));
  CHECK(body.find("\"x=\"") < body.find("\"items=\""));
  CHECK(contains(body, "out += \"[]\";"));
  CHECK(contains(body, "::c6502::pcpp::py_repr(v.line)"));
}

TEST_CASE("a field type with no C++ form is an error") {
  CHECK_THROWS_AS(generate("module T { x = X(float f) }"), asdlgen::Error);
}

TEST_CASE("a parse error gives the line and column") {
  CHECK_THROWS_WITH_AS(asdlgen::parse("module T {\r\n  x = y\r\n}"),
                       "2:8: constructor name must start with an uppercase letter: 'y'",
                       asdlgen::Error);
  CHECK_THROWS_WITH_AS(asdlgen::parse("module T { X = A }"),
                       "1:13: type name must start with a lowercase letter: 'X'",
                       asdlgen::Error);
  CHECK_THROWS_WITH_AS(asdlgen::parse("module T { x = A } y"),
                       "1:20: trailing input after module", asdlgen::Error);
  CHECK_THROWS_WITH_AS(asdlgen::parse("Foo {}"), "1:1: expected 'module'", asdlgen::Error);
  CHECK_THROWS_WITH_AS(asdlgen::parse("module F"), "1:9: expected '{'", asdlgen::Error);
  CHECK_THROWS_WITH_AS(asdlgen::parse("module F { foo = X(Int value) }"),
                       "1:23: field type must start with a lowercase letter: 'Int'",
                       asdlgen::Error);
}

TEST_CASE("an empty module and a product with no fields parse") {
  const asdlgen::Module empty = asdlgen::parse("module Foo {}");
  CHECK(empty.name == "Foo");
  CHECK(empty.types.empty());
  const asdlgen::Module unit = asdlgen::parse("module F { unit = () }");
  REQUIRE(unit.types.size() == 1);
  CHECK(unit.types[0].name == "unit");
  CHECK(unit.types[0].is_product);
  CHECK(unit.types[0].fields.empty());
}

TEST_CASE("comments may come before the module, inside it, and after it") {
  const asdlgen::Module m = asdlgen::parse(
      "-- head\nmodule F {\n  -- inside\n  foo = (int x) -- trailing\n  -- between\n"
      "  bar = Baz\n}\n-- tail\n");
  REQUIRE(m.types.size() == 2);
  CHECK(m.types[0].name == "foo");
  REQUIRE(m.types[0].fields.size() == 1);
  CHECK(m.types[0].fields[0].type == "int");
  CHECK(m.types[0].fields[0].name == "x");
  CHECK(m.types[1].name == "bar");
  REQUIRE(m.types[1].constructors.size() == 1);
  CHECK(m.types[1].constructors[0].name == "Baz");
}

TEST_CASE("the module name is the file's stem and _ast") {
  CHECK(asdlgen::cpp_module_name("asm.asdl") == "asm_ast");
  CHECK(asdlgen::cpp_module_name("C:/c6502/c99.asdl") == "c99_ast");
  CHECK(asdlgen::cpp_module_name("dir\\tac.asdl") == "tac_ast");
}

}  // namespace
