// The `compile.py -S -O` text of tests/test_pointer_member_access.py's
// accesses of struct members through pointers. The simulator's runs are
// in test_pointer_member_access_sim.cpp.
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

constexpr const char* kStruct = R"(
#include <stdint.h>
struct s { uint8_t a[13]; uint16_t g; uint8_t *buf; };
struct node { struct node *next; uint16_t v; };
extern struct s *P [[c6502::zeropage(0xF4)]];
extern struct node *N [[c6502::zeropage(0xF6)]];
)";

// Returns `compile.py - -S -O -o -` of `source`.
std::string asm_of(const std::string& source) {
  const auto o = suite::compile_main({"compile.py", "-", "-S", "-O", "-o", "-"}, source);
  REQUIRE(o.status.has_value());
  REQUIRE(*o.status == 0);
  return o.out();
}

// Returns the lines of function `name`, from its label to the next
// line that labels a function.
std::string function_text(const std::string& text, const std::string& name) {
  const std::vector<std::string> lines = suite::lines(text);
  std::size_t start = 0;
  while (start < lines.size() && lines[start] != name + ":") ++start;
  if (start == lines.size()) throw std::runtime_error("no line " + name + ":");
  std::size_t end = start + 1;
  while (end < lines.size() &&
         !(!lines[end].empty() && !std::isspace(static_cast<unsigned char>(lines[end][0])) &&
           lines[end].back() == ':' && lines[end][0] != '.')) {
    ++end;
  }
  std::string out;
  for (std::size_t k = start; k < end; ++k) out += (k == start ? "" : "\n") + lines[k];
  return out;
}

std::string first_word(const std::string& line) {
  std::istringstream in(line);
  std::string w;
  in >> w;
  return w;
}

}  // namespace

TEST_CASE("pointer_member_access: a member store through a zero-page pointer goes through it") {
  const std::string text =
      function_text(asm_of(std::string(kStruct) + "void f(uint16_t v) { P->g = v; }"), "f");
  CHECK_FALSE(suite::contains(text, "DPTR"));
  CHECK(suite::count(text, "STA   (P),Y") == 2);
}

TEST_CASE("pointer_member_access: a member through a pointer parameter needs no DPTR") {
  const std::string text = asm_of(std::string(kStruct) + R"(
            void put(struct s *r) { r->g = 0x1234; }
            uint16_t get(struct s *r) { return r->g; })");
  CHECK_FALSE(suite::contains(function_text(text, "put"), "DPTR"));
  CHECK_FALSE(suite::contains(function_text(text, "get"), "DPTR"));
}

TEST_CASE("pointer_member_access: a store of a node's own pointer member keeps the copy") {
  const std::string text =
      function_text(asm_of(std::string(kStruct) + "void f(struct node *q) { N->next = q; }"), "f");
  CHECK(suite::contains(text, "STA   (DPTR),Y"));
}

TEST_CASE("pointer_member_access: an add to a pointer member reads and writes its high byte") {
  const std::string text =
      function_text(asm_of(std::string(kStruct) + "void f(void) { P->buf += 0x100; }"), "f");
  CHECK(suite::count(text, "LDA   (P),Y") == 1);
  CHECK(suite::count(text, "STA   (P),Y") == 1);
  CHECK(suite::contains(text, "LDY   #$10"));
}

TEST_CASE("pointer_member_access: a compound assignment with a call calls first") {
  const std::string text = function_text(asm_of(R"(
            #include <stdint.h>
            struct mob { uint8_t pad[11]; uint8_t level; };
            extern struct mob *E [[c6502::zeropage(0xF8)]];
            uint8_t h(uint8_t x);
            void f(uint8_t x) { E->level |= h(x); })"),
                                         "f");
  const std::vector<std::string> lines = suite::lines(text);
  std::vector<std::string> ops;
  for (std::size_t k = 2; k < lines.size(); ++k) {
    if (!lines[k].empty()) ops.push_back(first_word(lines[k]));
  }
  CHECK(ops == std::vector<std::string>{"JSR", "LDY", "ORA", "STA", "RTS"});
}

TEST_CASE("pointer_member_access: a choice of member is a choice of offset") {
  const std::string text = function_text(asm_of(R"(
            #include <stdint.h>
            struct mob { uint8_t pad[7]; uint8_t sword, armor, dagger; };
            extern struct mob *E [[c6502::zeropage(0xF8)]];
            void f(uint8_t n, uint8_t v) {
                uint8_t *w = n < 3 ? &E->sword : &E->dagger;
                *w = v;
            })"),
                                         "f");
  CHECK(suite::contains(text, "LDY   #$07"));
  CHECK(suite::contains(text, "LDY   #$09"));
  CHECK(suite::contains(text, "STA   (E),Y"));
  CHECK_FALSE(suite::contains(text, "ADC"));
}

TEST_CASE("pointer_member_access: an index minus one multiplies the byte") {
  const std::string text = function_text(asm_of(R"(
            #include <stdint.h>
            struct o { uint8_t b[9]; };
            struct o *option(struct o *p, uint8_t n) { return &p[n - 1]; }
            )"),
                                         "option");
  CHECK_FALSE(suite::contains(text, "SBC   #$01"));
  CHECK(suite::contains(text, "SBC   #$09"));
}

TEST_CASE("pointer_member_access: a compare with a member reads it through the pointer") {
  const std::string text = function_text(asm_of(R"(
            #include <stdint.h>
            struct mob { uint8_t b[13]; uint16_t gold; };
            extern struct mob *E [[c6502::zeropage(0xF8)]];
            extern uint16_t W [[c6502::address(0x5A02)]];
            uint8_t c(void) { return W <= E->gold; })"),
                                         "c");
  CHECK_FALSE(suite::contains(text, "STA"));
  CHECK(suite::count(text, "LDA   (E),Y") == 2);
}
