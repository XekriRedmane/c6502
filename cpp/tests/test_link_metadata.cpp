// Tests of `parse_metadata` (passes/zp_link_metadata.py), with the
// result or the error Python gives for each text. `c6502cpp
// check-binutils` checks it on every block the corpus and the suite
// parse (the `parse_metadata` records of tools/capture_binutils.py),
// which reach none of its errors, so each of its thirteen ValueError
// messages has a case here, and so do the errors of the fields it reads
// with `kvs[...]` and `int()`.
#include <string>
#include <vector>

#include "doctest.h"
#include "errors.h"
#include "passes/zp_link_metadata.h"

namespace {

using c6502::CompileError;
using c6502::passes::format_metadata;
using c6502::passes::parse_metadata;

const std::string kBegin = "; @zp-link-meta-begin\n";
const std::string kEnd = "; @zp-link-meta-end\n";

std::string block(const std::string& line) { return kBegin + line + "\n" + kEnd; }

// Returns "<type>: <message>" of the error the text gives, or "" when it
// parses.
std::string error_of(const std::string& text) {
  try {
    parse_metadata(text);
  } catch (const CompileError& e) {
    return e.python_type() + ": " + e.what();
  }
  return "";
}

// Returns the lines `format_metadata` writes of what the text parses to.
std::vector<std::string> formatted(const std::string& text) {
  return format_metadata(parse_metadata(text));
}

}  // namespace

TEST_CASE("parse_metadata: the errors of the block's brackets") {
  CHECK(error_of(kBegin + kBegin) ==
        "ValueError: nested @zp-link-meta-begin without preceding @zp-link-meta-end");
  CHECK(error_of(kEnd) == "ValueError: @zp-link-meta-end without preceding @zp-link-meta-begin");
  CHECK(error_of(block("def f")) ==
        "ValueError: unexpected line inside zp-link-meta block: 'def f'");
  CHECK(error_of(kBegin) == "ValueError: @zp-link-meta-begin without matching @zp-link-meta-end");
}

TEST_CASE("parse_metadata: the errors of the records") {
  CHECK(error_of(block("; def f params= locals=")) ==
        "ValueError: malformed def record: 'def f params= locals='");
  CHECK(error_of(block("; ext f")) == "ValueError: malformed ext record: 'ext f'");
  CHECK(error_of(block("; share member=f:a bogus=1")) ==
        "ValueError: malformed share record: 'share member=f:a bogus=1'");
  CHECK(error_of(block("; share holder=g")) ==
        "ValueError: malformed share record: 'share holder=g'");
  CHECK(error_of(block("; zp x")) == "ValueError: malformed zp record: 'zp x'");
  CHECK(error_of(block("; item x")) == "ValueError: malformed item record: 'item x'");
  CHECK(error_of(block("; call f -> g later")) ==
        "ValueError: malformed call record: 'call f -> g later'");
  CHECK(error_of(block("; frob x")) == "ValueError: unknown record kind 'frob': 'frob x'");
  CHECK(error_of(block("; item x section")) == "ValueError: expected key=value, got 'section'");
}

TEST_CASE("parse_metadata: a missing field is Python's KeyError") {
  CHECK(error_of(block("; def f params= locals= indirect=false x=1")) == "KeyError: 'in_cycle'");
  CHECK(error_of(block("; runtime hargs_need=3")) == "KeyError: 'hargs_size'");
}

TEST_CASE("parse_metadata: numbers are read as int() reads them") {
  CHECK(error_of(block("; zp x size=1x at=auto def=true")) ==
        "ValueError: invalid literal for int() with base 10: '1x'");
  CHECK(error_of(block("; def f params= locals= indirect=false in_cycle=false pinned=$zz")) ==
        "ValueError: invalid literal for int() with base 16: 'zz'");
  // Arabic-Indic digits, a `0x` prefix with an underscore, and a sign.
  CHECK(formatted(block("; zp x size=١٢ at=$0x1_F def=true weight=+7")) ==
        std::vector<std::string>{"; @zp-link-meta-begin",
                                 "; zp x size=12 at=$1F def=true init= weight=7",
                                 "; @zp-link-meta-end"});
  CHECK(error_of(block("; zp x size=" + std::string(4301, '1') + " at=auto def=true")) ==
        "ValueError: Exceeds the limit (4300 digits) for integer string conversion: value has "
        "4301 digits; use sys.set_int_max_str_digits() to increase the limit");
  // Python reads 2**130, which the port's 128 bits can't hold.
  const std::string wide = "1361129467683753853853498429727072845824";
  CHECK(error_of(block("; zp x size=" + wide + " at=auto def=true")).rfind("OverflowError: ", 0) ==
        0);
}

TEST_CASE("parse_metadata: lines and tokens split at Unicode's breaks and spaces") {
  CHECK(formatted(block("; def f params=a,b locals= indirect=true in_cycle=false "
                        "pinned=,$80 carry=true")) ==
        std::vector<std::string>{
            "; @zp-link-meta-begin",
            "; def f params=a,b locals= indirect=true in_cycle=false pinned=,$80 carry=true",
            "; @zp-link-meta-end"});
  CHECK(formatted("x\u0085" + kBegin + "; call f -> g saved\u0085; item é section=.text "
                  "local=true " + kEnd) ==
        std::vector<std::string>{"; @zp-link-meta-begin", "; call f -> g saved",
                                 "; item é section=.text local=true zero=false",
                                 "; @zp-link-meta-end"});
  CHECK(formatted(block("; share member=f:a,b holder=g live=g:h forward=k>f writer=g member=m")) ==
        std::vector<std::string>{
            "; @zp-link-meta-begin",
            "; share member=f:a,b member=m: holder=g live=g:h forward=k>f writer=g",
            "; @zp-link-meta-end"});
  CHECK(parse_metadata("clear_page1:\n   RTS\n").defs.empty());
}
