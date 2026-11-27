// The tests of tests/test_bin_to_bcd.py that read the TAC and the `-S`
// text or run the TAC interpreter: `__c6502_bin_to_bcd` is a TAC
// `Unary(BinToBcd)` and a call of `bin16_to_bcd`, and a constant
// argument folds.
#include <string>
#include <variant>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;
namespace ir = c6502::tac_ast;

// The Unary instructions of function `fn` of the TAC of `src`.
std::vector<ir::Unary> unaries(const std::string& src, bool optimize, const std::string& fn) {
  suite::Options o;
  o.optimize = optimize;
  const suite::Tac tac = suite::tac_program(src, o);
  for (const auto& tl : tac.program.top_level) {
    if (const auto* f = std::get_if<ir::Function>(&tl); f != nullptr && f->name == fn) {
      std::vector<ir::Unary> out;
      for (const auto& ins : f->instructions) {
        if (const auto* u = std::get_if<ir::Unary>(&ins)) out.push_back(*u);
      }
      return out;
    }
  }
  FAIL("no function " << fn);
  return {};
}

}  // namespace

TEST_CASE("bin_to_bcd: the builtin lowers to a unary and a helper call") {
  const std::string src =
      "#include <stdint.h>\nuint16_t a;"
      "uint32_t f(void) { return __c6502_bin_to_bcd(a); }";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::vector<ir::Unary> ops = unaries(src, optimize, "f");
    REQUIRE(ops.size() == 1);
    CHECK(std::holds_alternative<ir::BinToBcd>(ops[0].op));
    CHECK(suite::contains(suite::codegen(src, optimize), "JSR   bin16_to_bcd"));
  }
}

TEST_CASE("bin_to_bcd: a constant argument folds in both pipelines") {
  const std::string src =
      "#include <stdint.h>\n"
      "uint32_t f(void) { return __c6502_bin_to_bcd(54321); }";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(unaries(src, optimize, "f").empty());
    const std::string asm_text = suite::codegen(src, optimize);
    CHECK_FALSE(suite::contains(asm_text, "bin16_to_bcd"));
    CHECK(suite::contains(asm_text, "#$21"));
  }
}

TEST_CASE("bin_to_bcd: a propagated constant folds") {
  const std::string src =
      "#include <stdint.h>\n"
      "uint32_t f(void) { uint16_t x = 999; x++;"
      " return __c6502_bin_to_bcd(x); }";
  CHECK(unaries(src, true, "f").empty());
  CHECK_FALSE(suite::contains(suite::codegen(src, true), "bin16_to_bcd"));
}

TEST_CASE("bin_to_bcd: an unused result goes") {
  const std::string src =
      "#include <stdint.h>\nuint16_t a;"
      "void f(void) { (void)__c6502_bin_to_bcd(a); }";
  CHECK(unaries(src, true, "f").empty());
}

TEST_CASE("bin_to_bcd: the TAC interpreter runs the builtin") {
  const std::string src = R"(
#include <stdint.h>
uint16_t v[4] = {0, 9, 65535, 31415};
uint32_t f(uint16_t x) { return __c6502_bin_to_bcd(x); }
int main(void) {
    uint32_t s = 0;
    for (uint8_t i = 0; i < 4; i++) s ^= f(v[i]) + i;
    return (int)(s ^ (s >> 16));
}
)";
  const auto value = suite::run_program(src);
  REQUIRE(value.has_value());
  // The XOR of each value's BCD plus its index, folded to 16 bits, as
  // builtin_functions.py's `bin_to_bcd` gives it.
  CHECK(static_cast<long long>(*value & 0xFFFF) == 0x4120);
}
