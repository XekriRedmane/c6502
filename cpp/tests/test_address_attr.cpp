// The tests of tests/test_address_attr.py that read a program's `-S`
// text or a link's errors. They check the type checker's rules for
// `[[c6502::address(addr)]]` (no definition, every declaration
// agreeing), the EQU lines and absolute stores it gives, the calling
// conventions such a function may have, and links of units that use
// the same fixed names.
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "py_regex.h"

namespace {

namespace suite = c6502::suite;

suite::Options with_optimize(bool optimize) {
  suite::Options o;
  o.optimize = optimize;
  return o;
}

// Returns whether Python's `re.search(pattern, text)` finds a match,
// for an ASCII `text`.
bool py_search(std::string_view pattern, std::string_view text) {
  const c6502::py_regex::Regex re(pattern);
  const std::u32string t(text.begin(), text.end());
  for (std::size_t pos = 0; pos <= t.size(); ++pos) {
    if (re.match(t, pos)) return true;
  }
  return false;
}

// Returns the address that the line `name\tEQU\t$hex` gives `name`, or
// -1 when no line binds it.
long long equ(const std::string& asm_text, const std::string& name) {
  for (const std::string& line : suite::lines(asm_text)) {
    const std::string head = name + "\tEQU\t$";
    if (!line.starts_with(head)) continue;
    const std::string hex = line.substr(head.size());
    if (!hex.empty() && hex.find_first_not_of("0123456789ABCDEF") == std::string::npos) {
      return std::stoll(hex, nullptr, 16);
    }
  }
  return -1;
}

}  // namespace

TEST_CASE("address_attr: a definition of a fixed-address name is rejected") {
  const std::vector<std::pair<std::string, std::string>> bad = {
      {"tentative definition", "unsigned char x [[c6502::address(0xC000)]];\n"},
      {"initializer",
       "extern unsigned char x [[c6502::address(0xC000)]]"
       " = 1;\n"},
      {"later definition",
       "extern unsigned char x [[c6502::address(0xC000)]];\n"
       "unsigned char x = 3;\n"},
      {"function defined later",
       "[[c6502::address(0x1000)]] void f(void);\n"
       "void f(void) {}\n"},
      {"function defined earlier",
       "void f(void) {}\n"
       "[[c6502::address(0x1000)]] void f(void);\n"},
      {"conflicting function addresses",
       "[[c6502::address(0x1000)]] void f(void);\n"
       "[[c6502::address(0x2000)]] void f(void);\n"},
      {"conflicting object addresses",
       "extern unsigned char x [[c6502::address(0xC000)]];\n"
       "extern unsigned char x [[c6502::address(0xC001)]];\n"},
      {"block-scope static",
       "int main(void) { static unsigned char x "
       "[[c6502::address(0xC000)]]; return x; }\n"},
      {"automatic",
       "int main(void) { unsigned char x "
       "[[c6502::address(0xC000)]]; return x; }\n"},
  };
  for (const auto& [label, src] : bad) {
    INFO(label);
    const suite::Error e = suite::compile_error(suite::Stage::kCodegen, src, with_optimize(true));
    CHECK(suite::is_a(e.type, "TypeCheckError"));
  }
}

TEST_CASE("address_attr: a block-scope extern gets its address") {
  const std::string asm_text = suite::codegen(
      "int main(void) {\n"
      "  extern unsigned char x [[c6502::address(0xC000)]];\n"
      "  return x;\n"
      "}\n",
      true);
  CHECK(equ(asm_text, "x") == 0xC000);
}

TEST_CASE("address_attr: fixed names get EQUs, and a store to one is absolute") {
  const std::string src =
      "[[c6502::zp_abi, c6502::address(0xFDED)]]\n"
      "extern void COUT(register unsigned char c);\n"
      "extern volatile unsigned char scratch "
      "[[c6502::address(0x0779)]];\n"
      "void f(void) { COUT(0xC1); scratch = 0x91; }\n";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::string asm_text = suite::codegen(src, optimize);
    CHECK(equ(asm_text, "COUT") == 0xFDED);
    CHECK(equ(asm_text, "scratch") == 0x0779);
    CHECK(suite::contains(asm_text, "JSR   COUT"));
    CHECK(suite::contains(asm_text, "STA   scratch"));
    // No pointer is staged in DPTR for the store.
    CHECK_FALSE(py_search(R"(STA\s+DPTR|\(DPTR\))", asm_text));
    // Nothing is defined for either name.
    CHECK_FALSE(py_search("(?m)^(COUT|scratch):", asm_text));
    CHECK(suite::contains(asm_text, "COUT\tEQU\t$FDED"));
    CHECK(suite::contains(asm_text, "scratch\tEQU\t$0779"));
    // The argument is in A, so COUT's parameter slot is never written
    // and gets no EQU.
    CHECK_FALSE(suite::contains(asm_text.substr(0, asm_text.find("@zp-link")), "__zpabi_COUT"));
  }
}

TEST_CASE("address_attr: a fixed-address function without zp_abi takes the soft stack") {
  // The default zp_abi promotion under -O doesn't apply to a function
  // with a fixed address.
  const std::string src =
      "[[c6502::address(0x1000)]] void f(unsigned char c);\n"
      "void g(unsigned char c) { f(c); }\n";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::string asm_text = suite::codegen(src, optimize);
    const std::size_t call = asm_text.find("JSR   f");
    REQUIRE(call != std::string::npos);
    CHECK(suite::contains(asm_text.substr(0, call), "STA   (SSP),Y"));
    CHECK_FALSE(suite::contains(asm_text, "__zpabi_f_"));
  }
}

TEST_CASE("address_attr: declarations with a convention a fixed routine can't have") {
  const std::vector<std::pair<std::string, std::string>> bad = {
      {"register without zp_abi",
       "[[c6502::address(0x1000)]]\n"
       "void f(register unsigned char c);\n"
       "void g(void) { f(1); }\n"},
      {"parameter not in a register",
       "[[c6502::zp_abi, c6502::address(0x1000)]]\n"
       "void f(register unsigned char a, unsigned char b);\n"
       "void g(void) { f(1, 2); }\n"},
      {"4-byte result",
       "[[c6502::zp_abi, c6502::address(0x1000)]]\n"
       "unsigned long f(void);\n"
       "void g(void) { f(); }\n"},
      {"address taken",
       "[[c6502::zp_abi, c6502::address(0x1000)]]\n"
       "void f(register unsigned char c);\n"
       "void g(void) { void (*p)(unsigned char) = f; p(1); }\n"},
  };
  for (const auto& [label, src] : bad) {
    for (const bool optimize : {false, true}) {
      INFO(label << " optimize=" << optimize);
      const suite::Error e =
          suite::compile_error(suite::Stage::kCodegen, src, with_optimize(optimize));
      CHECK(suite::is_a(e.type, "AbiSelectionError"));
    }
  }
}

namespace {

const std::string kRom =
    "[[c6502::zp_abi, c6502::address(0xFDED)]]\n"
    "extern void COUT(register unsigned char c);\n"
    "extern volatile unsigned char KBD "
    "[[c6502::address(0xC000)]];\n";

}  // namespace

TEST_CASE("address_attr: a link binds each fixed name once") {
  const suite::TempDir tmp;
  const std::string a = tmp.file("a.o");
  const std::string b = tmp.file("b.o");
  const std::string out = tmp.file("out");
  REQUIRE(suite::compile_object(kRom +
                                    "void say(unsigned char c);\n"
                                    "int main(void) { say(KBD); return 0; }\n",
                                a) == 0);
  REQUIRE(suite::compile_object(kRom + "void say(unsigned char c) { COUT(c); }\n", b) == 0);
  const auto o = suite::link({a, b}, out);
  REQUIRE_MESSAGE(o.status == 0, o.err());
  const std::string text = suite::linked_text(out);
  CHECK(suite::count(text, "COUT\tEQU\t$FDED") == 1);
  CHECK(suite::count(text, "KBD\tEQU\t$C000") == 1);
}

TEST_CASE("address_attr: units that give a name different addresses don't link") {
  const suite::TempDir tmp;
  const std::string a = tmp.file("a.o");
  const std::string b = tmp.file("b.o");
  REQUIRE(suite::compile_object("[[c6502::address(0xFDED)]] void out(unsigned char c);\n"
                                "void f(void) { out(1); }\n",
                                a) == 0);
  REQUIRE(suite::compile_object("[[c6502::address(0xFDF0)]] void out(unsigned char c);\n"
                                "void g(void) { out(2); }\n",
                                b) == 0);
  const auto o = suite::link({a, b}, tmp.file("out"));
  CHECK(o.status == 1);
  CHECK(suite::contains(o.err(), "'out' is at $FDF0"));
}
