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
