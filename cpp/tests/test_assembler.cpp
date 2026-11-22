// Tests of the assembler (binutils/assembler.py), with the result
// Python gives for each text: its error, or the bytes of the object's
// first section. `c6502cpp check-binutils` checks it on every assembly
// of the corpus and the suite, which reach 8 of its 35 AssemblerError
// messages, so each message has a case here, except the two no text can
// reach: an operator the parser doesn't make, and a statement whose
// bytes differ from the count its sizing gave. The rest are cases of
// Python's text and integers that the port has to keep.
#include <optional>
#include <string>
#include <vector>

#include "binutils/assembler.h"
#include "binutils/elf.h"
#include "doctest.h"
#include "errors.h"

namespace {

using namespace c6502::binutils;
using c6502::CompileError;

struct Case {
  const char* text;
  const char* expected;
};

// From Python's `assemble(text, "t.s")`.
const Case kCases[] = {
    {"f:\n LDA #$\n", "error AssemblerError: t.s:2: can't read '$'"},
    {"f:\n LDA #(1\n", "error AssemblerError: t.s:2: expected ), found the end of the line"},
    {"f:\n LDA #\n", "error AssemblerError: t.s:2: expected more, found the end of the line"},
    {"f:\n LDA #)\n", "error AssemblerError: t.s:2: unexpected ')'"},
    {"f:\n LDA #1 2\n", "error AssemblerError: t.s:2: unexpected '2' in '1 2'"},
    {"f:\n LDA #1/0\n", "error AssemblerError: t.s:2: division by zero"},
    {"f:\n LDA #x*2\n",
     "error AssemblerError: t.s:2: `x` can't be used in `*`, since its address is only known "
     "when linking"},
    {"f:\n LDA #< <x\n", "error AssemblerError: t.s:2: `<` of a byte"},
    {"f: LDA\n", "error AssemblerError: t.s:1: can't read the label 'f: LDA'"},
    {".l:\n", "error AssemblerError: t.s:1: local label .l before any item"},
    {"f:\n.l:\n.l:\n", "error AssemblerError: t.s:3: .l is defined twice in f"},
    {"f:\nf:\n", "error AssemblerError: t.s:2: f is defined twice"},
    {" lda #1\n", "error AssemblerError: t.s:1: LDA comes before any label"},
    // `str.upper` maps "ß" to "SS".
    {" \u00df\n", "error AssemblerError: t.s:1: SS comes before any label"},
    {"f:\n frob\n", "error AssemblerError: t.s:2: unknown instruction or directive frob"},
    {"X EQU y\n", "error AssemblerError: t.s:1: X has to be a number"},
    {"X EQU 1\nX EQU 2\n", "error AssemblerError: t.s:2: X is given two values"},
    {"f:\n DS.B\n", "error AssemblerError: t.s:2: DS.B takes a count"},
    {"f:\n DS.B x\n", "error AssemblerError: t.s:2: DS.B needs a constant count"},
    {"f:\n DS.B 1,x\n", "error AssemblerError: t.s:2: DS.B needs a constant fill"},
    {"f:\n BNE x,X\n", "error AssemblerError: t.s:2: BNE takes a label"},
    {"f:\n LDX x,X\n", "error AssemblerError: t.s:2: LDX has no absx addressing mode"},
    {"f:\n BNE (x),Y\n", "error AssemblerError: t.s:2: BNE has no indy addressing mode"},
    {"; @zp-link-meta-begin\n; item f section=.bss local=false zero=true\n; @zp-link-meta-end\n"
     "f:\n DC.B 1\n",
     "error AssemblerError: t.s:4: the link metadata says f is all zero bytes, and it has data"},
    {"f EQU 1\nf:\n RTS\n", "error AssemblerError: t.s: f is both a label and an EQU"},
    {"f:\n JMP .x\n", "error AssemblerError: t.s:2: .x isn't a label in this function"},
    {"f:\n DC.B \"ab\"c\n", "error AssemblerError: t.s:2: bad string \"ab\"c"},
    {"f:\n DC.B \"\u0100\"\n",
     "error AssemblerError: t.s:2: \"\u0100\" holds a character outside Latin-1, which DC.B "
     "can't store in a byte"},
    {"f:\n BNE <x\n", "error AssemblerError: t.s:2: a branch takes a label"},
    {"f:\n.l:\n DS.B 200\n BNE .l\n",
     "error AssemblerError: t.s:4: branch to .l is -202 bytes away, out of reach"},
    {"f:\n BNE 5\n", "error AssemblerError: t.s:2: a branch takes a label"},
    {"f:\n LDA (-31),Y\n", "error AssemblerError: t.s:2: $-1F isn't in zero page"},
    {"f:\n LDA (x),Y\n",
     "error AssemblerError: t.s:2: LDA (indy) needs a zero-page address, and x isn't known to "
     "be in zero page"},
    {"f:\n DC.B 256\n", "error AssemblerError: t.s:2: 256 doesn't fit in 1 byte(s)"},
    {"f:\n DC.W <x\n",
     "error AssemblerError: t.s:2: `<` or `>` gives one byte, in a 2-byte field"},
    {"f:\n DC.L x\n", "error AssemblerError: t.s:2: a 4-byte field can't hold the address of x"},
    // Arabic-Indic digits.
    {"f:\n LDA #\u0661\u0662\n", "data a90c"},
    // `str.upper` maps the long s to "S" and the ligature "ﬆ" to "ST".
    {"f:\n \u017fta $10\n \ufb06a $11\n", "data 85108511"},
    // Latin-1, floor division, an arithmetic `>>`, a value wider than 64
    // bits on the way, and two's complement `&` and `~`.
    {"f:\n DC.B \"\u00e9\", -7/2, -1>>4, (1<<100)>>100, 3&-2, ~0\n", "data e9fcff0102ff"},
    {"f:\n DC.B 1<<-1\n", "error ValueError: negative shift count"},
    // U+0085 and U+2028 end lines, U+2003 starts an instruction, and a
    // `';'` opens no string, so the `;` starts a comment.
    {"f:\xC2\x85 DC.B 1\u2028\u2003DC.B 2\r\n DC.B ';'\n",
     "error AssemblerError: t.s:4: can't read \"'\""},
    {"f:\n DC.B 'a', '\"', \"a;b\" ; c\n", "error AssemblerError: t.s:2: can't read ' \"a'"},
    // An `EQU`'s symbol stays a symbol in an operand, so `X,X` is a
    // relocation of the absolute X.
    {"X = $80\nf:\n SUBROUTINE\n LDA X,X\n STA X+1\n LDA (X),Y\n JMP (X)\n BCC .l\n.l:\n ASL a\n"
     " DC.B <g, >g\n LDA #>(g+2)\n RTS\n",
     "data b5008500b1006c000090000a0000a90060"},
    {"f:\n LDA [1+2]*3\n DS.B 2, $1FF\n DC.W -1, g+1\n BEQ g\n", "data a509ffffffff0000f000"},
};

std::string hex(const std::string& data) {
  static const char digits[] = "0123456789abcdef";
  std::string out;
  for (const unsigned char c : data) {
    out += digits[c >> 4];
    out += digits[c & 15];
  }
  return out;
}

std::string result_of(const std::string& text, const assembler::LinkSymbols* links = nullptr) {
  try {
    const elf::ElfFile obj = assembler::assemble(text, "t.s", links);
    return "data " + hex(obj.sections.at(0)->data);
  } catch (const CompileError& e) {
    return "error " + e.python_type() + ": " + e.what();
  }
}

}  // namespace

TEST_CASE("assembler: Python's result for each case") {
  for (const Case& c : kCases) {
    CAPTURE(c.text);
    CHECK(result_of(c.text) == c.expected);
  }
}

TEST_CASE("assembler: a decimal number of more than 4,300 digits is Python's ValueError") {
  CHECK(result_of("f:\n DC.B " + std::string(4301, '1') + "\n") ==
        "error ValueError: Exceeds the limit (4300 digits) for integer string conversion: value "
        "has 4301 digits; use sys.set_int_max_str_digits() to increase the limit");
}

TEST_CASE("assembler: what the port takes less of than Python gives an error that says so") {
  // Python computes 2**200 // 2**199; the port's integers have 128 bits.
  CHECK(result_of("f:\n DC.B (1<<200)>>199\n").rfind("error OverflowError: t.s:2: ", 0) == 0);
  // 60 parentheses are 488 of Python's calls, which Python takes and the
  // port doesn't, and 40 are 328.
  CHECK(result_of("f:\n DC.B " + std::string(60, '(') + "1" + std::string(60, ')') + "\n")
            .rfind("error RecursionError: t.s:2: ", 0) == 0);
  CHECK(result_of("f:\n DC.B " + std::string(40, '(') + "1" + std::string(40, ')') + "\n") ==
        "data 01");
}

TEST_CASE("assembler: the linker's addresses choose the addressing mode") {
  const std::string text = "DPTR EQU $24\n__local_f__0 EQU $90\nf:\n LDA DPTR\n STA __local_f__0\n";
  CHECK(result_of(text) == "data a5008500");
  const assembler::LinkSymbols outside{{"DPTR", std::nullopt}, {"__local_f__0", 0x300}};
  CHECK(result_of(text, &outside) == "data ad00008d0000");
  const elf::ElfFile obj = assembler::assemble(text, "dir/t.s");
  // The names the linker places stay undefined, marked when the unit
  // reaches them in zero page.
  CHECK(obj.symbols.at(0)->name == "t.s");
  const auto slot = obj.symbol("__local_f__0");
  REQUIRE(static_cast<bool>(slot));
  CHECK(!slot->defined());
  CHECK(slot->other == elf::STO_MOS_ZEROPAGE);
}

TEST_CASE("assembler: the object keeps the text with its lines joined by LF") {
  const elf::ElfFile obj = assembler::assemble("f:\r\n RTS\x0c", "t.s");
  CHECK(obj.source_text().value_or("none") == "f:\n RTS\n");
  CHECK(!assembler::assemble("f:\n RTS\n", "t.s", nullptr, false).source_text().has_value());
}
