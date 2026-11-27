// The tests of tests/test_unroll.py that ran compile.py: under -O a
// loop marked `[[c6502::unroll]]` is unrolled, without -O it isn't, and
// the unroller's rejection is a one-line error.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

constexpr const char* kSumLoop = R"(
int main(void) {
    int s = 0;
    [[c6502::unroll]] for (int i = 0; i < 4; i++) s += i;
    return s;
}
)";

}  // namespace

TEST_CASE("unroll: a loop the unroller rejects is a one-line error") {
  const auto o = suite::compile_main({"compile.py", "-", "-S", "-O"},
                                     "unsigned char row[256];\n"
                                     "void fill(unsigned char v) {\n"
                                     "    [[c6502::unroll]] "
                                     "for (unsigned char i = 0; i <= 255; i++) row[i] = v;\n"
                                     "}\n");
  REQUIRE(o.status.has_value());
  CHECK_FALSE(o.exited);
  CHECK(*o.status != 0);
  const std::string err = o.err();
  const std::size_t first = err.find_first_not_of(" \t\r\n");
  const auto lines = suite::lines(
      first == std::string::npos
          ? std::string()
          : err.substr(first, err.find_last_not_of(" \t\r\n") - first + 1));
  REQUIRE(lines.size() == 1);
  CHECK(suite::contains(lines[0], "never terminates"));
}

TEST_CASE("unroll: -O unrolls a marked loop") {
  // A loop left in the TAC would have its continue and break labels.
  const auto o = suite::compile_main({"compile.py", "-", "--tac", "-O"}, kSumLoop);
  CHECK_FALSE(o.exited);
  CHECK(o.status == 0);
  CHECK_FALSE(suite::contains(o.out(), "_continue"));
  CHECK_FALSE(suite::contains(o.out(), "_break"));
}

TEST_CASE("unroll: without -O a marked loop stays a loop") {
  const auto o = suite::compile_main({"compile.py", "-", "--tac"}, kSumLoop);
  CHECK_FALSE(o.exited);
  CHECK(o.status == 0);
  CHECK(suite::contains(o.out(), "_continue"));
  CHECK(suite::contains(o.out(), "_break"));
}
