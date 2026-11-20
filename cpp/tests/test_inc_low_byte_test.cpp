// The tests of tests/test_inc_low_byte_test.py, ported.
#include <string>
#include <string_view>

#include "asm_text.h"
#include "doctest.h"
#include "passes/inc_low_byte_test.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_inc_low_byte_test(program(instrs)));
}

const std::string kM0 = R"(Data("P" 0))";
const std::string kLdaM0 = R"(Mov(Data("P" 0) Reg(A) false) )";
const std::string kRet = "Return(false false false) ";
const std::string kBody = R"(Mov(Imm(0) Data("V" 0) false) )";
// The two-byte chain, `INC P; BNE .d; INC P+1; .d:`.
const std::string kChain2 =
    R"(Inc(Data("P" 0)) Branch(NE ".d") Inc(Data("P" 1)) Label(".d") )";

}  // namespace

TEST_CASE("inc_low_byte_test moves a BNE onto the chain") {
  SUBCASE("two bytes") {
    CHECK(run(kChain2 + kLdaM0 + R"(Branch(NE ".t") )" + kBody + R"(Label(".t") )" + kRet) ==
          expect(R"(Inc(Data("P" 0)) Branch(NE ".t") Inc(Data("P" 1)) )" + kLdaM0 + kBody +
                 R"(Label(".t") )" + kRet));
  }
  SUBCASE("three bytes keep the label for the later branch") {
    const std::string chain3 =
        R"(Inc(Data("P" 0)) Branch(NE ".d") Inc(Data("P" 1)) Branch(NE ".d")
           Inc(Data("P" 2)) Label(".d") )";
    CHECK(run(chain3 + kLdaM0 + R"(Branch(NE ".t") )" + kBody + R"(Label(".t") )" + kRet) ==
          expect(R"(Inc(Data("P" 0)) Branch(NE ".t") Inc(Data("P" 1)) Branch(NE ".d")
                    Inc(Data("P" 2)) Label(".d") )" +
                 kLdaM0 + kBody + R"(Label(".t") )" + kRet));
  }
}

TEST_CASE("inc_low_byte_test turns a BEQ into a jump") {
  CHECK(run(kChain2 + kLdaM0 + R"(Branch(EQ ".t") )" + kBody + R"(Label(".t") )" + kRet) ==
        expect(R"(Inc(Data("P" 0)) Branch(NE ".inc_test@0") Inc(Data("P" 1)) )" + kLdaM0 +
               R"(Jump(".t" []) Label(".inc_test@0") )" + kBody + R"(Label(".t") )" + kRet));
}

TEST_CASE("inc_low_byte_test leaves other shapes alone") {
  auto unchanged = [](const std::string& text) { CHECK(run(text) == expect(text)); };
  SUBCASE("A read where the branch would go") {
    // The moved branch would skip the load.
    unchanged(kChain2 + kLdaM0 + R"(Branch(NE ".t") )" + kBody +
              R"(Label(".t") Return(true false false))");
  }
  SUBCASE("the label reached from elsewhere") {
    unchanged(R"(Branch(NE ".d") )" + kChain2 + kLdaM0 + R"(Branch(NE ".t") )" + kBody +
              R"(Label(".t") )" + kRet);
  }
  SUBCASE("a test of another byte") {
    unchanged(kChain2 + R"(Mov(Data("P" 1) Reg(A) false) Branch(NE ".t") )" + kBody +
              R"(Label(".t") )" + kRet);
  }
  SUBCASE("a branch on the sign") {
    unchanged(kChain2 + kLdaM0 + R"(Branch(MI ".t") )" + kBody + R"(Label(".t") )" + kRet);
  }
  SUBCASE("a volatile load") {
    unchanged(kChain2 + R"(Mov(Data("P" 0) Reg(A) true) Branch(NE ".t") )" + kBody +
              R"(Label(".t") )" + kRet);
  }
}
