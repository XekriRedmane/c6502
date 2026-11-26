// Tests of `parse_metadata`. `c6502cpp check-frozen` checks it on the
// blocks the corpus and the suite parsed, which reach none of its
// errors, so each of its messages has a case here.
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

TEST_CASE("parse_metadata: a missing field") {
  CHECK(error_of(block("; def f params= locals= indirect=false x=1")) ==
        "ValueError: missing in_cycle in def record: 'def f params= locals= indirect=false x=1'");
  CHECK(error_of(block("; runtime hargs_need=3")) ==
        "ValueError: missing hargs_size in runtime record: 'runtime hargs_need=3'");
}

TEST_CASE("parse_metadata: a number is digits, and an address `$` and hex digits") {
  CHECK(formatted(block("; zp x size=12 at=$1f def=true weight=7")) ==
        std::vector<std::string>{"; @zp-link-meta-begin",
                                 "; zp x size=12 at=$1F def=true init= weight=7",
                                 "; @zp-link-meta-end"});
  CHECK(error_of(block("; zp x size=1x at=auto def=true")) ==
        "ValueError: bad number '1x' in zp record: 'zp x size=1x at=auto def=true'");
  CHECK(error_of(block("; def f params= locals= indirect=false in_cycle=false pinned=$zz")) ==
        "ValueError: bad number 'zz' in def record: "
        "'def f params= locals= indirect=false in_cycle=false pinned=$zz'");
  // Python's int() took each of these.
  for (const char* field : {"size=+7", "size=1_2", "size=", "size=١٢"}) {
    CAPTURE(field);
    CHECK(error_of(block(std::string("; zp x ") + field + " at=auto def=true"))
              .rfind("ValueError: bad number ", 0) == 0);
  }
  for (const char* at : {"at=$", "at=80", "at=$$80", "at=$0x80"}) {
    CAPTURE(at);
    CHECK(error_of(block(std::string("; zp x size=1 ") + at + " def=true"))
              .rfind("ValueError: bad number ", 0) == 0);
  }
  CHECK(error_of(block("; zp x size=" + std::string(16, '1') + " at=auto def=true")) ==
        "ValueError: bad number '1111111111111111' in zp record: 'zp x size=1111111111111111 "
        "at=auto def=true'");
  CHECK(formatted(block("; zp x size=" + std::string(15, '9') + " at=auto def=true")) ==
        std::vector<std::string>{"; @zp-link-meta-begin",
                                 "; zp x size=999999999999999 at=auto def=true init=",
                                 "; @zp-link-meta-end"});
}

TEST_CASE("parse_metadata: lines end at a newline, and words are split at spaces and tabs") {
  CHECK(formatted(block("; def f params=a,b locals= indirect=true in_cycle=false "
                        "pinned=,$80 carry=true")) ==
        std::vector<std::string>{
            "; @zp-link-meta-begin",
            "; def f params=a,b locals= indirect=true in_cycle=false pinned=,$80 carry=true",
            "; @zp-link-meta-end"});
  // A `.asm` file written with CR LF line ends, and tabs between words.
  CHECK(formatted("x\r\n; @zp-link-meta-begin\r\n; call\tf -> g  saved\r\n"
                  "; item \xC3\xA9 section=.text local=true \r\n; @zp-link-meta-end\r\n") ==
        std::vector<std::string>{"; @zp-link-meta-begin", "; call f -> g saved",
                                 "; item \xC3\xA9 section=.text local=true zero=false",
                                 "; @zp-link-meta-end"});
  // U+0085 and U+2028 aren't line ends, so a marker after one isn't a
  // line of its own.
  CHECK(error_of("x\u0085" + kBegin + kEnd) ==
        "ValueError: @zp-link-meta-end without preceding @zp-link-meta-begin");
  CHECK(error_of("x " + kEnd).empty());
  CHECK(formatted(block("; share member=f:a,b holder=g live=g:h forward=k>f writer=g member=m")) ==
        std::vector<std::string>{
            "; @zp-link-meta-begin",
            "; share member=f:a,b member=m: holder=g live=g:h forward=k>f writer=g",
            "; @zp-link-meta-end"});
  CHECK(parse_metadata("clear_page1:\n   RTS\n").defs.empty());
}
