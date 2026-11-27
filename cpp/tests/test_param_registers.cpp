// The tests of tests/test_param_registers.py that compile a source:
// the errors of a parameter that names its register, as
// `[[c6502::in_y]]` or a pair such as `in_xy`, and the code of the
// callee and the caller.
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

suite::Error rejected(const std::string& src) {
  suite::Options o;
  o.optimize = true;
  return suite::compile_error(suite::Stage::kCodegen, src, o);
}

// Returns Python's `text.split(sep)[1]`, the text between the first
// `sep` and the next.
std::string piece_after(const std::string& text, const std::string& sep) {
  const std::size_t at = text.find(sep);
  REQUIRE(at != std::string::npos);
  const std::size_t start = at + sep.size();
  const std::size_t end = text.find(sep, start);
  return text.substr(start, end == std::string::npos ? std::string::npos : end - start);
}

// The paragraph after `fn`'s SUBROUTINE line, split at whitespace.
std::vector<std::string> body_words(const std::string& asm_text, const std::string& fn) {
  std::istringstream in(
      piece_after(piece_after(piece_after(asm_text, "\n" + fn + ":"), "SUBROUTINE"), "\n\n"));
  std::vector<std::string> out;
  for (std::string w; in >> w;) out.push_back(w);
  return out;
}

const std::string kSharedRegister =
    "void f(register unsigned char a [[c6502::in_y]],\n"
    "       register unsigned char b [[c6502::in_y]]) {}\n"
    "int main(void) { f(1, 2); return 0; }\n";

}  // namespace

TEST_CASE("param_registers: the ABI's errors") {
  const std::vector<std::pair<std::string, std::pair<std::string, std::string>>> cases = {
      {"wrong width",
       {"[[c6502::zp_abi, c6502::address(0xD000)]]\n"
        "void f(unsigned int a [[c6502::in_y]]);\n"
        "int main(void) { f(1); return 0; }\n",
        "is 2 bytes, but `in_y` names 1 register"}},
      {"shared register", {kSharedRegister, "both arrive in Y"}},
      {"positional overflow",
       {"void f(register unsigned char a [[c6502::in_a]],\n"
        "       register unsigned int b) {}\n"
        "int main(void) { f(1, 2); return 0; }\n",
        "the free ones here are: X"}},
      {"declarations disagree",
       {"void f(register unsigned char a [[c6502::in_y]]);\n"
        "void f(register unsigned char a) {}\n"
        "int main(void) { f(1); return 0; }\n",
        "`a in_y` against `register a`"}},
  };
  for (const auto& [label, c] : cases) {
    INFO(label);
    const suite::Error e = rejected(c.first);
    CHECK(suite::is_a(e.type, "AbiSelectionError"));
    CHECK_MESSAGE(suite::search(c.second, e.message), e.message);
  }
}

TEST_CASE("param_registers: a plain register pointer parameter is rejected") {
  // A pointer takes `in_*`.
  const suite::Error e = rejected(
      "void f(register unsigned char *p) {}\n"
      "int main(void) { f(0); return 0; }\n");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK_MESSAGE(suite::search("register parameter 'p': a pointer", e.message), e.message);
}

TEST_CASE("param_registers: the type checker's errors") {
  const std::vector<std::pair<std::string, std::pair<std::string, std::string>>> cases = {
      {"wrong width",
       {"void f(unsigned int a [[c6502::in_y]]) {}\n",
        "parameter 'a' is 2 bytes, but `in_y` names 1 register"}},
      {"pointer in one register",
       {"void f(unsigned char *p [[c6502::in_y]]) {}\n",
        "parameter 'p' is 2 bytes, but `in_y` names 1 register"}},
      {"struct",
       {"struct s { unsigned char a, b; };\n"
        "void f(struct s v [[c6502::in_xy]]) {}\n",
        "integer or a pointer"}},
  };
  for (const auto& [label, c] : cases) {
    INFO(label);
    const suite::Error e = rejected(c.first + "int main(void) { return 0; }\n");
    CHECK(suite::is_a(e.type, "TypeCheckError"));
    CHECK_MESSAGE(suite::search(c.second, e.message), e.message);
  }
}

TEST_CASE("param_registers: compile.py reports an ABI error in one line") {
  const auto o = suite::compile_main({"compile.py", "-", "-S", "-O"}, kSharedRegister);
  REQUIRE(o.status.has_value());
  CHECK(*o.status == 1);
  CHECK(o.err().rfind("<stdin>: error: ", 0) == 0);
  CHECK_FALSE(suite::contains(o.err(), "internal error"));
}

TEST_CASE("param_registers: the callee stores Y and the caller loads it") {
  const std::string asm_text = suite::codegen(
      "unsigned char out[2];\n"
      "unsigned char g;\n"
      "void put(register unsigned char hi [[c6502::in_a]],\n"
      "         register unsigned char lo [[c6502::in_y]]) {\n"
      "    out[0] = hi; out[1] = lo;\n"
      "}\n"
      "int main(void) { put(1, g); return 0; }\n",
      true);
  CHECK(body_words(asm_text, "put") ==
        std::vector<std::string>{"STA", "out", "STY", "out+1", "RTS"});
  CHECK(suite::contains(piece_after(asm_text, "\nmain:"), "LDY   g"));
}

TEST_CASE("param_registers: a pointer argument goes straight to its registers") {
  // Zork's RWTS takes the address of its block in Y and A.
  const std::string asm_text = suite::codegen(
      "struct iob { unsigned char t[17]; };\n"
      "struct iob iob;\n"
      "[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0x2900)]]\n"
      "bool RWTS(struct iob *p [[c6502::in_ya]]);\n"
      "int main(void) { return RWTS(&iob); }\n",
      true);
  CHECK(suite::contains(piece_after(asm_text, "\nmain:"),
                        "LDA   #>iob\n   LDY   #<iob\n   JSR   RWTS"));
}

TEST_CASE("param_registers: the bytes of an address go straight to their registers") {
  // The shape of Zork's RWTS call: `LDA #>iob; LDY #<iob`.
  const std::string asm_text = suite::codegen(
      "unsigned char iob[17];\n"
      "[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0x2900)]]\n"
      "bool RWTS(register unsigned char hi [[c6502::in_a]],\n"
      "                   register unsigned char lo [[c6502::in_y]]);\n"
      "int main(void) {\n"
      "    return RWTS((unsigned char)((unsigned int)iob >> 8),\n"
      "                (unsigned char)(unsigned int)iob);\n"
      "}\n",
      true);
  const std::string main_text = piece_after(asm_text, "\nmain:");
  CHECK(suite::contains(main_text, "LDA   #>iob\n   LDY   #<iob\n   JSR   RWTS"));
  CHECK_FALSE(suite::contains(main_text, "PHA"));
}
