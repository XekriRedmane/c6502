// The tests of tests/test_preserves.py that compile a source:
// `[[c6502::preserves_x]]` and `preserves_y` on a routine at a fixed
// address, what the type checker rejects, and the code that keeps a
// loop counter, a `register` local, or an immediate in X or Y across a
// call of the routine.
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// A routine at $D000 that takes A, keeps X, and destroys Y.
const std::string kKeepsX =
    "[[c6502::zp_abi, c6502::preserves_x, c6502::address(0xD000)]]\n"
    "void put(register unsigned char c);\n";
// The same routine, declared without the promise.
const std::string kPlain =
    "[[c6502::zp_abi, c6502::address(0xD000)]]\n"
    "void put(register unsigned char c);\n";
const std::string kKeepsY =
    "[[c6502::zp_abi, c6502::preserves_y, c6502::address(0xD000)]]\n"
    "void put(register unsigned char c);\n";
const std::string kSumLoop =
    "unsigned char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
    "unsigned char n = 8;\n"
    "void send(void) {\n"
    "    for (unsigned char i = 0; i != n; i++)\n"
    "        put(buf[i]);\n"
    "}\n"
    "int main(void) { send(); return 0; }\n";
const std::string kCountDown =
    "unsigned char n = 5;\n"
    "void tick(void) {\n"
    "    unsigned char c = n;\n"
    "    do { put(0); } while (--c != 0);\n"
    "}\n";
const std::string kRegisterLocal =
    "unsigned char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
    "unsigned char n = 8;\n"
    "void send(void) {\n"
    "    register unsigned char i;\n"
    "    for (i = 0; i < n; i++) put(buf[i]);\n"
    "}\n"
    "int main(void) { send(); return 0; }\n";
const std::string kImmediateDecls =
    "[[c6502::zp_abi, c6502::preserves_x, c6502::address(0xD000)]]\n"
    "void keep(register unsigned char c [[c6502::in_x]]);\n"
    "[[c6502::zp_abi, c6502::address(0xD000)]]\n"
    "void lose(register unsigned char c [[c6502::in_x]]);\n"
    "unsigned char g = 4;\n";

bool is_label_start(const std::string& line) {
  static const std::regex label{"[A-Za-z_]\\w*:"};
  std::smatch m;
  return std::regex_search(line, m, label, std::regex_constants::match_continuous);
}

// The lines of function `fn` in `asm_text`, each with its newline, up to
// the next line that starts with a label at column 0, or "" when no line
// is `fn:`.
std::string body(const std::string& asm_text, const std::string& fn) {
  const std::string head = fn + ":\n";
  std::size_t at = 0;
  if (asm_text.rfind(head, 0) != 0) {
    at = asm_text.find("\n" + head);
    if (at == std::string::npos) return "";
    ++at;
  }
  std::size_t pos = at + head.size();
  const std::size_t start = pos;
  while (pos < asm_text.size()) {
    std::size_t end = asm_text.find('\n', pos);
    const std::string line =
        asm_text.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
    if (is_label_start(line)) break;
    if (end == std::string::npos) {
      pos = asm_text.size();
      break;
    }
    pos = end + 1;
  }
  return asm_text.substr(start, pos - start);
}

std::size_t matches(const std::string& pattern, const std::string& text) {
  const std::regex re{pattern};
  return static_cast<std::size_t>(
      std::distance(std::sregex_iterator(text.begin(), text.end(), re), std::sregex_iterator()));
}

}  // namespace

TEST_CASE("preserves: the type checker rejects a misused promise") {
  const std::vector<std::pair<std::string, std::string>> bad = {
      {"no address", "[[c6502::preserves_x]] void f(void);\n"},
      {"int result in X", "[[c6502::preserves_x, c6502::address(0x1000)]] int f(void);\n"},
      {"unsigned int result in X",
       "[[c6502::preserves_x, c6502::address(0x1000)]] unsigned int f(void);\n"},
  };
  for (const auto& [label, src] : bad) {
    INFO(label);
    const suite::Error e = suite::compile_error(suite::Stage::kCodegen, src, [] {
      suite::Options o;
      o.optimize = true;
      return o;
    }());
    CHECK(suite::is_a(e.type, "TypeCheckError"));
  }
}

TEST_CASE(
    "preserves: the type checker accepts preserves_y with an int result and a later promise") {
  const std::vector<std::pair<std::string, std::string>> good = {
      {"Y with an int result", "[[c6502::preserves_y, c6502::address(0x1000)]] int f(void);\n"},
      {"address declared first",
       "[[c6502::address(0x1000)]] void f(void);\n[[c6502::preserves_x]] void f(void);\n"},
  };
  for (const auto& [label, src] : good) {
    INFO(label);
    CHECK_NOTHROW(suite::codegen(src + "void g(void) { f(); }\n", true));
  }
}

TEST_CASE("preserves: a count-up counter stays in X across a call that preserves X") {
  const std::string b = body(suite::codegen(kKeepsX + kSumLoop, true), "send");
  CHECK(suite::search("CPX\\s+n", b));
  CHECK(suite::search("LDA\\s+buf,X\\n\\s+JSR\\s+put\\n\\s+INX", b));
  CHECK_FALSE(suite::search("STX|LDX\\s+__local", b));
}

TEST_CASE("preserves: a count-up counter in X is saved around a call without the promise") {
  // The counter is still kept in X, but saved and reloaded around a call
  // that may clobber it.
  const std::string b = body(suite::codegen(kPlain + kSumLoop, true), "send");
  CHECK(suite::search("STX\\s+(\\S+)\\n\\s+JSR\\s+put\\n\\s+LDX\\s+\\1", b));
}

TEST_CASE("preserves: a call before the loop isn't wrapped") {
  const std::string src = kPlain +
                          "unsigned char buf[4];\nunsigned char n;\n"
                          "void send(void) {\n"
                          "    put(1); put(2);\n"
                          "    for (unsigned char i = 0; i != n; i++) put(buf[i]);\n"
                          "}\n";
  const std::string b = body(suite::codegen(src, true), "send");
  // Only the call inside the loop is wrapped.
  CHECK(matches("STX", b) == 1);
}

TEST_CASE("preserves: a count-down counter stays in Y across a call that preserves Y") {
  const std::string b = body(suite::codegen(kKeepsY + kCountDown, true), "tick");
  CHECK(suite::search("JSR\\s+put\\n\\s+DEY", b));
}

TEST_CASE("preserves: a count-down counter stays in memory without the promise") {
  const std::string b = body(suite::codegen(kPlain + kCountDown, true), "tick");
  CHECK_FALSE(suite::search("DE[XY]", b));
}

TEST_CASE("preserves: a register local stays in Y across a call that preserves Y") {
  const std::string b = body(suite::codegen(kKeepsY + kRegisterLocal, true), "send");
  CHECK(suite::search("LDA\\s+buf,Y\\n\\s+JSR\\s+put\\n\\s+INY", b));
  CHECK_FALSE(suite::search("STY|__local", b));
}

TEST_CASE("preserves: a register local across a call without the promise is rejected") {
  suite::Options o;
  o.optimize = true;
  const suite::Error e = suite::compile_error(suite::Stage::kCodegen, kPlain + kRegisterLocal, o);
  CHECK(suite::is_a(e.type, "RegallocError"));
  CHECK_MESSAGE(suite::search("can't pin", e.message), e.message);
}

TEST_CASE("preserves: an immediate in X is kept across a call that preserves X") {
  const std::string b = body(
      suite::codegen(kImmediateDecls + "void f(void) { keep(5); keep(5); keep(5); }\n", true), "f");
  CHECK(matches("LDX\\s+#\\$05", b) == 1);
}

TEST_CASE("preserves: an immediate in X is reloaded after a call without the promise") {
  const std::string b =
      body(suite::codegen(kImmediateDecls + "void f(void) { lose(5); lose(5); }\n", true), "f");
  CHECK(matches("LDX\\s+#\\$05", b) == 2);
}

TEST_CASE("preserves: a value from memory is reloaded after a call that preserves X") {
  // The callee may write the memory.
  const std::string b =
      body(suite::codegen(kImmediateDecls + "void f(void) { keep(g); keep(g); }\n", true), "f");
  CHECK(matches("LDX\\s+g", b) == 2);
}
