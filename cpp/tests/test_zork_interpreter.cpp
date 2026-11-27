// The checks of tests/test_zork_interpreter.py that run no machine, on
// the multi-unit example in examples/zork: the checked-in listing of the
// linked program, interpreter.asm, is objdump's listing of the program
// the units link to, and it calls no runtime helper. golden/O.txt and
// golden/links.txt hold the units' `-S -O` text and the executable's
// digest.
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

const char* const kUnits[] = {
    "main", "decode", "objects", "numbers", "input", "pages",
    "zstring", "output", "disk", "prompts", "save",
};

// Compiles each unit with `-c -O` and links the objects by the example's
// script, as its README does, and returns the executable's path.
std::string build(const suite::TempDir& tmp) {
  std::vector<std::string> link{"compile.py", "-T", suite::repo_path("examples/zork/interpreter.ld")};
  for (const char* unit : kUnits) {
    const std::string obj = tmp.file(std::string(unit) + ".o");
    const auto o = suite::compile_main({"compile.py",
                                        suite::repo_path("examples/zork/" + std::string(unit) + ".c"),
                                        "-c", "-O", "-o", obj});
    REQUIRE_MESSAGE(o.status == 0, unit << ": " << o.err());
    link.push_back(obj);
  }
  const std::string exe = tmp.file("interpreter");
  link.push_back("-o");
  link.push_back(exe);
  const auto o = suite::compile_main(link);
  REQUIRE_MESSAGE(o.status == 0, o.err());
  return exe;
}

}  // namespace

TEST_CASE("zork_interpreter: interpreter.asm is the listing of the linked program") {
  const suite::TempDir tmp;
  CHECK(suite::listing(build(tmp)) == suite::read_repo_file("examples/zork/interpreter.asm"));
}

TEST_CASE("zork_interpreter: the program calls no runtime helper") {
  // The program runs on Zork's own machine, which has none of the
  // runtime's helpers and nothing that sets up its soft stack, so it
  // calls neither.
  const std::string text = suite::read_repo_file("examples/zork/interpreter.asm");
  CHECK_FALSE(suite::search(R"(JSR\s+((mul|[us]divmod|asl|asr|lsr)(8|16|32|64)|icall)\b)", text));
  CHECK_FALSE(suite::search(R"(\(SSP\)|\(FP\))", text));
}
