// The tests of tests/test_noreturn_call.py that read a program's `-O`
// text. A call of a `[[noreturn]]` function becomes a jump, and such a
// call without arguments is no edge of the call graph
// (src/passes/call_edges.h), so a game's main loop that a function two
// calls down abandons is on no cycle.
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// The `-O` text, with inlining off as tests/no_inlining.py turns it
// off.
std::string codegen_no_inlining(const std::string& src) {
  suite::Options o;
  o.optimize = true;
  o.inline_calls = false;
  return suite::codegen(src, o);
}

// Returns the text of function `name` in `asm_text`, up to the next
// function's label.
std::string body(const std::string& asm_text, const std::string& name) {
  const std::vector<std::string> lines = suite::lines(asm_text);
  std::size_t start = 0;
  while (start < lines.size() && lines[start] != name + ":") ++start;
  REQUIRE(start < lines.size());
  std::size_t end = start + 1;
  while (end < lines.size()) {
    const std::string& l = lines[end];
    if (!l.empty() && l[0] != ' ' && l[0] != '\t' && l.ends_with(":") && l[0] != '.') break;
    ++end;
  }
  std::string out;
  for (std::size_t k = start; k < end; ++k) {
    if (k > start) out += '\n';
    out += lines[k];
  }
  return out;
}

// Returns whether `text` reads or writes through SSP or FP.
bool soft_stack(const std::string& text) {
  for (const std::string r : {"SSP", "FP"}) {
    if (suite::contains(text, "(" + r + ")") || suite::contains(text, "   " + r) ||
        suite::contains(text, r + "+")) {
      return true;
    }
  }
  return false;
}

// A game's main loop, which `escape` abandons from two calls down.
// `get_key` takes its argument in X, so it has to be zp_abi, and
// `escape` returns its `bool` in the carry flag.
const char* const kLoop = R"(
#include <stdint.h>
#include <stdbool.h>
volatile uint8_t key;
uint8_t last;
[[noreturn]] void command_loop(void);
static bool escape(uint8_t k) { return k == 0x9B; }
static uint8_t get_key(uint8_t n [[c6502::in_x]]) {
    uint8_t k = key + n;
    if (escape(k))
        command_loop();
    return k;
}
void command_loop(void) {
    for (;;)
        last = get_key(3);
}
)";

}  // namespace

TEST_CASE("noreturn_call: under -O a call of a noreturn function is a jump") {
  // `out` is volatile, since the optimizer drops a store that no path
  // to a return can read, and `stop` never returns.
  const std::string asm_text = suite::codegen(R"(
volatile unsigned char out[3];
[[noreturn]] void stop(unsigned char code);
void stop(unsigned char code) {
    out[2] = code;
    for (;;) {
    }
}
unsigned char check(unsigned char n) {
    if (n == 0)
        stop(7);
    return 100 / n;
}
int main(void) {
    out[0] = check(4);
    out[1] = check(0);
    return 0;
}
)",
                                              true);
  const std::size_t at = asm_text.find("\ncheck:\n");
  REQUIRE(at != std::string::npos);
  const std::size_t from = at + 8;
  const std::size_t end = asm_text.find("\nmain:\n", from);
  const std::string check =
      asm_text.substr(from, end == std::string::npos ? std::string::npos : end - from);
  CHECK(suite::contains(check, "JMP   stop"));
  CHECK_FALSE(suite::contains(check, "JSR   stop"));
}

TEST_CASE("noreturn_call: a main loop that a callee abandons stays off the soft stack") {
  // Before, the `register` parameter was an error, since `command_loop`
  // was on a cycle and used the soft stack, and without it `get_key`
  // tested the carry with `LDA #0; ROL A`.
  const std::string asm_text = codegen_no_inlining(kLoop);
  CHECK_FALSE_MESSAGE(soft_stack(asm_text), asm_text);
  const std::string get_key = body(asm_text, "get_key");
  CHECK(suite::contains(get_key, "JMP   command_loop"));
  CHECK(suite::contains(get_key, "BCC"));
  CHECK_FALSE(suite::contains(get_key, "ROL"));
}

TEST_CASE("noreturn_call: save_hardware_stack on a call that abandons its caller is an error") {
  const suite::Error e = suite::compile_error(suite::Stage::kCodegen,
                                              "[[noreturn]] void r(void);\n"
                                              "void f(void) {\n"
                                              "[[c6502::save_hardware_stack]] r();\n}\n"
                                              "void r(void) { for (;;) f(); }\n",
                                              [] {
                                                suite::Options o;
                                                o.optimize = true;
                                                o.inline_calls = false;
                                                return o;
                                              }());
  CHECK(suite::is_a(e.type, "RecursionSaveError"));
  CHECK(suite::contains(e.message, "nothing has to be saved"));
}

TEST_CASE("noreturn_call: the link metadata has no call line for the call and no cycle") {
  std::vector<std::string> meta;
  for (const std::string& l : suite::lines(codegen_no_inlining(kLoop))) {
    if (l.starts_with("; call ") || l.starts_with("; def ")) meta.push_back(l);
  }
  bool call_line = false;
  for (const std::string& l : meta) {
    INFO(l);
    call_line = call_line || l == "; call get_key -> escape";
    CHECK_FALSE(suite::contains(l, "-> command_loop"));
    CHECK_FALSE(suite::contains(l, "in_cycle=true"));
  }
  CHECK(call_line);
}
