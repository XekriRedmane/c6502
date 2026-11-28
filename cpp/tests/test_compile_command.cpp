// Tests of compile.py's command line in C++ (src/compile_command.h) and
// the parts of argparse it uses (src/argparse.h): the mutually
// exclusive group in the usage and its error, `type=`, `store_const`,
// and `parse_known_args`. The expected texts are what Python 3.11's
// argparse gives, and
// tools/compare_backends.py's `cli` jobs compare many more command
// lines with compile.py's.
#include <optional>
#include <string>
#include <vector>

#include "compile_command.h"
#include "doctest.h"
#include "errors.h"

namespace {

namespace cc = c6502::compile_command;

// c6502's usage at 41 columns.
const char kUsage41[] =
    "usage: c6502 [-h] [-o OUTPUT]\n"
    "             [--lex | --parse | --resolve | --tac | -S | -c]\n"
    "             [-O] [-T SCRIPT]\n"
    "             [--origin ADDR]\n"
    "             [--defsym NAME=VALUE]\n"
    "             inputs [inputs ...]\n";

cc::Outcome run41(const std::vector<std::string>& args) {
  cc::Options options;
  options.columns = 41;
  std::vector<std::string> argv{"c6502"};
  argv.insert(argv.end(), args.begin(), args.end());
  return cc::run(argv, options);
}

}  // namespace

TEST_CASE("compile: argparse's usage errors") {
  const auto err = [](const std::vector<std::string>& args) {
    const cc::Outcome o = run41(args);
    CHECK(o.exited);
    CHECK(o.status == 2);
    CHECK(o.out().empty());
    return o.err();
  };
  const std::string usage = kUsage41;
  CHECK(err({}) == usage + "c6502: error: the following arguments are required: inputs\n");
  CHECK(err({"a.c", "-c", "-S"}) ==
        usage + "c6502: error: argument -S: not allowed with argument -c\n");
  CHECK(err({"a.c", "-Sc"}) ==
        usage + "c6502: error: argument -c: not allowed with argument -S\n");
  CHECK(err({"a.c", "--origin", "'x'"}) ==
        usage + "c6502: error: argument --origin: invalid _number value: \"'x'\"\n");
  // `_rewrite_ttext` spells `-Ttext=ADDR` as `--origin ADDR`.
  CHECK(err({"a.c", "-Ttext=0x"}) ==
        usage + "c6502: error: argument --origin: invalid _number value: '0x'\n");
  CHECK(err({"a.c", "-O2"}) ==
        usage + "c6502: error: argument -O: ignored explicit argument '2'\n");
  CHECK(err({"-o"}) == usage + "c6502: error: argument -o: expected one argument\n");
}

TEST_CASE("compile: the help") {
  const cc::Outcome o = run41({"a.c", "--he"});
  CHECK(o.exited);
  CHECK(o.status == 0);
  CHECK(o.err().empty());
  const std::string help = o.out();
  CHECK(help.rfind(kUsage41, 0) == 0);
  // The help of -O, wrapped at its hyphens.
  CHECK(help.find("                   (constant folding,\n"
                  "                   strength reduction,\n"
                  "                   comparison-against-\n"
                  "                   zero / jump fold,\n"
                  "                   UCE, copy\n"
                  "                   propagation, dead-\n"
                  "                   store elimination),\n") != std::string::npos);
}

TEST_CASE("compile: c6502's own checks, after argparse's") {
  const cc::Outcome o = run41({"-S", "a.c", "a.c", "-o", "x.s", "-DX"});
  CHECK(!o.exited);
  CHECK(o.status == 2);
  CHECK(o.err() == "c6502: -o with more than one input\n");
  CHECK(run41({"--lex", "x.o"}).err() == "c6502: x.o isn't a C source\n");
}

TEST_CASE("compile: a C source on stdin") {
  // The callback stands for Python's `sys.stdin.read()`, which a second
  // `-` reads again and finds empty.
  int reads = 0;
  cc::Options options;
  options.read_stdin = [&] {
    ++reads;
    return std::string(reads == 1 ? "_Bool b;\n" : "");
  };
  const cc::Outcome o = cc::run({"c6502", "-", "-", "--lex"}, options);
  CHECK(o.status == 0);
  CHECK(reads == 2);
  REQUIRE(o.writes.size() == 2);
  CHECK(o.writes[0].stream == cc::Stream::kErr);
  CHECK(o.writes[0].text.rfind("<stdin>: warning: line 1: ", 0) == 0);
  CHECK(o.writes[1].stream == cc::Stream::kOut);
  CHECK(o.writes[1].text == "1:1\tkeyword\t_Bool\n1:7\tidentifier\tb\n1:8\tsymbol\t;\n");
}

TEST_CASE("compile: _number reads numbers as Python's int does") {
  using c6502::asdl::Int;
  const auto n = [](const char* text) { return cc::number(text); };
  CHECK(n("0x10") == Int(16));
  CHECK(n("$10") == Int(16));
  CHECK(n("$0x10") == Int(16));
  CHECK(n(" +0x_1f ") == Int(31));
  CHECK(n("00") == Int(0));
  CHECK(n("0_0") == Int(0));
  CHECK(n("-1") == Int(-1));
  CHECK(n("$-a") == Int(-10));
  CHECK(n("0o17") == Int(15));
  CHECK(n("1_000") == Int(1000));
  CHECK(n("$ 10") == Int(16));
  CHECK(n("\x1c" "5") == Int(5));
  for (const char* bad : {"010", "1__0", "$_10", "0b2", "", "$", "0x", "12_"}) {
    CHECK_MESSAGE(!n(bad).has_value(), bad);
  }
  CHECK_THROWS_AS(n("0x1000000000000000000000000000000000"), c6502::InternalError);
}
