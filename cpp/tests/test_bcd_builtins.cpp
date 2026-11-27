// The tests of tests/test_bcd_builtins.py that read the TAC and the `-S`
// text or run the TAC interpreter: `__c6502_bcd_add` and
// `__c6502_bcd_sub` of constants fold, and the code of the others is one
// chain of ADC or SBC between SED and CLD.
#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "builtin_functions.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim/cpu.h"

namespace {

namespace bf = c6502::builtin_functions;
namespace suite = c6502::suite;
namespace ir = c6502::tac_ast;

// The Binary instructions of function `fn`.
std::vector<ir::Binary> binaries(const ir::Program& prog, const std::string& fn) {
  for (const auto& tl : prog.top_level) {
    if (const auto* f = std::get_if<ir::Function>(&tl); f != nullptr && f->name == fn) {
      std::vector<ir::Binary> out;
      for (const auto& ins : f->instructions) {
        if (const auto* b = std::get_if<ir::Binary>(&ins)) out.push_back(*b);
      }
      return out;
    }
  }
  FAIL("no function " << fn);
  return {};
}

ir::Program tac(const std::string& src, bool optimize) {
  suite::Options o;
  o.optimize = optimize;
  return suite::tac_program(src, o).program;
}

std::string first_word(const std::string& line) {
  std::istringstream in(line);
  std::string w;
  in >> w;
  return w;
}

bool ends_with(const std::string& s, const std::string& suffix) {
  return s.size() >= suffix.size() &&
         s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::size_t index_of(const std::vector<std::string>& v, const std::string& item) {
  const auto it = std::find(v.begin(), v.end(), item);
  REQUIRE(it != v.end());
  return static_cast<std::size_t>(it - v.begin());
}

struct Program {
  const char* name;
  const char* src;
  long long expected;
};

// Programs whose `main` returns a 16-bit value, which the TAC
// interpreter runs. tests/test_bcd_builtins.py runs them in the
// simulator too.
const std::vector<Program>& programs() {
  static const std::vector<Program> table = {
      {"byte_add",
       "#include <stdint.h>\nuint8_t a = 0x38, b = 0x45;"
       "int main(void) { return __c6502_bcd_add(a, b); }",
       0x83},
      {"byte_wraps",
       "#include <stdint.h>\nuint8_t a = 0x99, b = 0x01;"
       "int main(void) { return __c6502_bcd_add(a, b); }",
       0x00},
      {"byte_borrows",
       "#include <stdint.h>\nuint8_t a = 0x00, b = 0x01;"
       "int main(void) { return __c6502_bcd_sub(a, b); }",
       0x99},
      {"word_carries",
       "#include <stdint.h>\nuint16_t a = 0x0999;"
       "int main(void) { return __c6502_bcd_add(a, 1); }",
       0x1000},
      {"word_borrows",
       "#include <stdint.h>\nuint16_t a = 0x1000;"
       "int main(void) { return __c6502_bcd_sub(a, 0x0001); }",
       0x0999},
      {"mixed_widths",
       "#include <stdint.h>\nuint8_t a = 0x99; uint16_t b = 0x0901;"
       "int main(void) { return __c6502_bcd_add(a, b); }",
       0x1000},
      {"long_low",
       "#include <stdint.h>\nuint32_t s = 0x00999950;"
       "int main(void) { uint32_t r = __c6502_bcd_add(s, 0x0250);"
       " return (uint16_t)r; }",
       0x0200},
      {"long_high",
       "#include <stdint.h>\nuint32_t s = 0x00999950;"
       "int main(void) { uint32_t r = __c6502_bcd_add(s, 0x0250);"
       " return (uint16_t)(r >> 16); }",
       0x0100},
      {"long_wraps",
       "#include <stdint.h>\nuint32_t s = 0x99999999;"
       "int main(void) { s = __c6502_bcd_add(s, 1);"
       " return (uint16_t)s | (uint16_t)(s >> 16); }",
       0x0000},
      {"long_borrows",
       "#include <stdint.h>\nuint32_t s = 0;"
       "int main(void) { s = __c6502_bcd_sub(s, 1);"
       " return (uint16_t)(s >> 8); }",
       0x9999},
      {"as_argument",
       "#include <stdint.h>\nuint8_t a = 0x19;"
       "int twice(uint16_t v) { return v * 2; }"
       "int main(void) { return twice(__c6502_bcd_add(a, 0x0001)); }",
       0x40},
      {"nested",
       "#include <stdint.h>\nuint16_t a = 0x0450, b = 0x0275;"
       "int main(void) { return __c6502_bcd_sub("
       "__c6502_bcd_add(a, b), __c6502_bcd_add(b, 0x0005)); }",
       0x0445},
      {"struct_and_array",
       "#include <stdint.h>\n"
       "struct s { uint8_t tag; uint16_t score; } p = {1, 0x0995};"
       "uint16_t t[3] = {0x0001, 0x0010, 0x0100};"
       "int main(void) { for (uint8_t i = 0; i < 3; i++)"
       " p.score = __c6502_bcd_add(p.score, t[i]);"
       " t[1] = __c6502_bcd_sub(p.score, t[2]);"
       " return t[1]; }",
       0x1006},
      // bcd_add(0x0F, 0x0F, 1) | bcd_sub(0x0F, 0x1A, 1) << 8, as
      // builtin_functions.py computed it.
      {"invalid_digits",
       "#include <stdint.h>\nuint8_t a = 0x0F, b = 0x0F;"
       "int main(void) { return __c6502_bcd_add(a, b)"
       " | __c6502_bcd_sub(b, 0x1A) << 8; }",
       0x9514},
      {"register_param",
       "#include <stdint.h>\n"
       "uint8_t f(register uint8_t x) { return __c6502_bcd_add(x, 0x01); }"
       "int main(void) { return f(0x09) + f(0x19); }",
       0x30},
  };
  return table;
}

const char* program(const std::string& name) {
  for (const Program& p : programs()) {
    if (p.name == name) return p.src;
  }
  FAIL("no program " << name);
  return "";
}

}  // namespace

TEST_CASE("bcd_builtins: constants fold in both pipelines") {
  const std::string src =
      "#include <stdint.h>\n"
      "uint16_t f(void) { return __c6502_bcd_add(0x0999, 0x0001); }";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(binaries(tac(src, optimize), "f").empty());
    CHECK_FALSE(suite::contains(suite::codegen(src, optimize), "SED"));
  }
}

TEST_CASE("bcd_builtins: propagated constants fold") {
  const std::string src =
      "#include <stdint.h>\n"
      "uint8_t f(void) { uint8_t x = 0x58; uint8_t y = 0x47;"
      " return __c6502_bcd_add(x, y); }";
  CHECK(binaries(tac(src, true), "f").empty());
  const std::string asm_text = suite::codegen(src, true);
  CHECK_FALSE(suite::contains(asm_text, "SED"));
  CHECK(suite::contains(asm_text, "#$05"));  // 58 + 47 = 105, wrapped to 05
}

TEST_CASE("bcd_builtins: the TAC interpreter runs the programs") {
  for (const Program& p : programs()) {
    INFO(p.name);
    const auto value = suite::run_program(p.src);
    REQUIRE(value.has_value());
    CHECK(static_cast<long long>(*value & 0xFFFF) == p.expected);
  }
}

TEST_CASE("bcd_builtins: a score added to a long") {
  const std::string src =
      "#include <stdint.h>\nuint32_t score;"
      "void add(uint16_t points) {"
      " score = __c6502_bcd_add(score, points); }";
  // `points` arrives in A and X, and `add` stores it before the chain,
  // which uses A for each byte.
  CHECK(suite::function_body(suite::codegen(src, true), "add") ==
        std::vector<std::string>{"STA __local_add__points_0", "STX __local_add__points_1", "SED",
                                 "CLC", "LDA score", "ADC __local_add__points_0", "STA score",
                                 "LDA score+1", "ADC __local_add__points_1", "STA score+1",
                                 "LDA score+2", "ADC #$00", "STA score+2", "LDA score+3",
                                 "ADC #$00", "STA score+3", "CLD", "RTS"});
}

TEST_CASE("bcd_builtins: a member is computed in place") {
  // `in_place_member_update` computes the member in its own bytes,
  // whichever operand it is. `x` arrives in A (and X), and each function
  // stores it before the chain.
  const std::string src =
      "#include <stdint.h>\n"
      "struct s { uint8_t tag; uint16_t score; } p;"
      "void add(uint8_t x) { p.score = __c6502_bcd_add(p.score, x); }"
      "void add2(uint16_t x) { p.score = __c6502_bcd_add(x, p.score); }";
  const std::string asm_text = suite::codegen(src, true);
  CHECK(suite::function_body(asm_text, "add") ==
        std::vector<std::string>{"STA __local_add__x", "SED", "CLC", "LDA p+1",
                                 "ADC __local_add__x", "STA p+1", "LDA p+2", "ADC #$00", "STA p+2",
                                 "CLD", "RTS"});
  CHECK(suite::function_body(asm_text, "add2") ==
        std::vector<std::string>{"STA __local_add2__x_0", "STX __local_add2__x_1", "SED", "CLC",
                                 "LDA __local_add2__x_0", "ADC p+1", "STA p+1",
                                 "LDA __local_add2__x_1", "ADC p+2", "STA p+2", "CLD", "RTS"});
}

TEST_CASE("bcd_builtins: the unused top bytes go") {
  const std::string src =
      "#include <stdint.h>\nuint32_t s;"
      "uint8_t low(void) { return __c6502_bcd_add(s, 0x25); }";
  const std::vector<std::string> body = suite::function_body(suite::codegen(src, true), "low");
  const std::size_t sed = index_of(body, "SED");
  const std::size_t cld = index_of(body, "CLD");
  const std::vector<std::string> chain(body.begin() + sed, body.begin() + cld + 1);
  REQUIRE(chain.size() >= 5);
  CHECK(std::vector<std::string>(chain.begin(), chain.begin() + 4) ==
        std::vector<std::string>{"SED", "CLC", "LDA s", "ADC #$25"});
  CHECK(chain.size() == 6);
}

TEST_CASE("bcd_builtins: a constant is subtracted") {
  const std::string src =
      "#include <stdint.h>\nuint16_t w;"
      "void dec(void) { w = __c6502_bcd_sub(w, 1); }";
  CHECK(suite::function_body(suite::codegen(src, true), "dec") ==
        std::vector<std::string>{"SED", "SEC", "LDA w", "SBC #$01", "STA w", "LDA w+1", "SBC #$00",
                                 "STA w+1", "CLD", "RTS"});
}

TEST_CASE("bcd_builtins: a byte result is stored from A inside the chain") {
  const std::string src =
      "#include <stdint.h>\nuint8_t a, b;"
      "uint8_t f(void) { return __c6502_bcd_add(a, b); }";
  const std::vector<std::string> body = suite::function_body(suite::codegen(src, true), "f");
  REQUIRE(body.size() >= 6);
  CHECK(std::vector<std::string>(body.begin(), body.begin() + 4) ==
        std::vector<std::string>{"SED", "CLC", "LDA a", "ADC b"});
  CHECK(body[4].rfind("STA ", 0) == 0);
  CHECK(body[5] == "CLD");
}

TEST_CASE("bcd_builtins: the unoptimized operands are soft-stack slots") {
  const std::string src =
      "#include <stdint.h>\n"
      "uint16_t f(uint16_t a, uint16_t b) {"
      " return __c6502_bcd_add(a, b); }";
  const std::vector<std::string> body = suite::function_body(suite::codegen(src, false), "f");
  const std::size_t start = index_of(body, "SED");
  const std::size_t end = index_of(body, "CLD");
  const std::vector<std::string> chain(body.begin() + start, body.begin() + end + 1);
  REQUIRE(chain.size() >= 3);
  CHECK(chain[1] == "CLC");
  // Each byte is a load, an add, and a store, each of a frame slot behind
  // its own `LDY #off`.
  std::vector<std::string> ops;
  for (std::size_t i = 2; i + 1 < chain.size(); ++i) {
    const std::string op = first_word(chain[i]);
    if (op == "LDA" || op == "ADC" || op == "STA") {
      INFO(chain[i]);
      CHECK(ends_with(chain[i], "(FP),Y"));
    }
    if (op != "LDY" && op != "INY" && op != "DEY") ops.push_back(op);
  }
  CHECK(ops == std::vector<std::string>{"LDA", "ADC", "STA", "LDA", "ADC", "STA"});
}

TEST_CASE("bcd_builtins: the decimal flag is set only around the chain") {
  const std::string src = program("struct_and_array");
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    bool inside = false;
    for (const std::string& line : suite::lines(suite::codegen(src, optimize))) {
      const std::string op = first_word(line);
      if (op == "SED") {
        CHECK_FALSE(inside);
        inside = true;
      } else if (op == "CLD") {
        CHECK(inside);
        inside = false;
      } else if (inside) {
        INFO(line);
        static const std::vector<std::string> kAllowed = {"CLC", "SEC", "LDA", "ADC", "SBC",
                                                          "STA", "LDY", "INY", "DEY"};
        CHECK(std::find(kAllowed.begin(), kAllowed.end(), op) != kAllowed.end());
      }
    }
    CHECK_FALSE(inside);
  }
}

TEST_CASE("bcd_builtins: adc_byte and sbc_byte give the CPU's A and C for every byte pair and carry") {
  using c6502::sim::Cpu;
  for (const bool subtract : {false, true}) {
    INFO((subtract ? "SBC" : "ADC"));
    std::vector<std::string> bad;
    Cpu cpu;
    for (int a = 0; a < 256; ++a) {
      for (int b = 0; b < 256; ++b) {
        for (int c = 0; c < 2; ++c) {
          cpu.memory[0x200] = subtract ? 0xE9 : 0x69;
          cpu.memory[0x201] = static_cast<std::uint8_t>(b);
          cpu.pc = 0x200;
          cpu.a = static_cast<std::uint8_t>(a);
          cpu.p = static_cast<std::uint8_t>(Cpu::kDecimal | Cpu::kUnused | c);
          cpu.step();
          const bf::ByteResult got = subtract ? bf::sbc_byte(a, b, c) : bf::adc_byte(a, b, c);
          if (got.a != cpu.a || got.carry != (cpu.p & Cpu::kCarry)) {
            bad.push_back(std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(c));
          }
        }
      }
    }
    if (bad.size() > 5) bad.resize(5);
    CHECK(bad == std::vector<std::string>{});
  }
}

TEST_CASE("bcd_builtins: bcd_add and bcd_sub wrap and borrow") {
  const auto add = [](long long x, long long y, int n) { return static_cast<long long>(bf::bcd_add(x, y, n)); };
  const auto sub = [](long long x, long long y, int n) { return static_cast<long long>(bf::bcd_sub(x, y, n)); };
  CHECK(add(0x99, 0x01, 1) == 0x00);
  CHECK(add(0x0099, 0x0001, 2) == 0x0100);
  CHECK(add(0x9999, 0x0001, 2) == 0x0000);
  CHECK(sub(0x00, 0x01, 1) == 0x99);
  CHECK(sub(0x0100, 0x0001, 2) == 0x0099);
  CHECK(add(0x00999999, 0x250, 4) == 0x01000249);
}
