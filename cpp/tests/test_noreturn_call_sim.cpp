// The simulator tests of tests/test_noreturn_call.py: under `-O` nothing
// runs after a call of a `[[noreturn]]` function (C23 §6.7.13.7,
// src/passes/optimization/noreturn_call.h), and a game that starts over
// from such a function, two calls deep, runs on the zp_abi convention in
// one unit and linked from two.
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

// `out` is volatile, since the optimizer drops a store that no path to
// a return can read, and `stop` never returns.
const std::string kProgram = R"c(
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
)c";
Bytes out_bytes(const sim::Simulation& s) {
  const std::int64_t base = s.symbol("out");
  return slice(s.cpu.memory, base, base + 3);
}

TEST_CASE("noreturn_call: a run stops in the call") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    sim::Simulation s = build_sim(kProgram, optimize);
    s.run(200'000);
    CHECK(out_bytes(s) == Bytes{25, 0, 7});
  }
}

// The call graph and a call that abandons its caller
// (cpp/src/passes/call_edges.h).

// Returns the lines of function `name` in `asm_text`, up to the next
// line that is a global label.
std::string body(const std::string& asm_text, const std::string& name) {
  const std::vector<std::string> all = lines(asm_text);
  auto at = std::find(all.begin(), all.end(), name + ":");
  REQUIRE(at != all.end());
  std::string out = *at;
  for (++at; at != all.end(); ++at) {
    const std::string& l = *at;
    if (!l.empty() && !std::isspace(static_cast<unsigned char>(l[0])) && l.back() == ':' && l[0] != '.') break;
    out += "\n" + l;
  }
  return out;
}

// Returns whether `text` reads or writes through SSP or FP.
bool soft_stack(const std::string& text) {
  for (const std::string r : {"SSP", "FP"}) {
    if (contains(text, "(" + r + ")") || contains(text, "   " + r) || contains(text, r + "+")) return true;
  }
  return false;
}

// A game that starts over from `reset_game` whenever `deep` ends a
// level, two calls below it. Each trip around leaves the return
// addresses of `middle` and `deep` on the hardware stack, so 200 trips
// wrap the stack pointer around page 1 a few times. `out` is volatile,
// since `halt` never returns.
const std::string kGameDeep = R"c(
volatile unsigned char out[3];
unsigned char steps;
[[noreturn]] void reset_game(void);
unsigned char deep(unsigned char n) {
    unsigned char v = n + 1;
    if (++steps == 3) {
        steps = 0;
        reset_game();
    }
    return v;
}
unsigned char middle(unsigned char n) {
    return deep(n) + deep(n + 1);
}
)c";
const std::string kGameTop = R"c(
volatile unsigned char out[3];
unsigned char resets;
unsigned char middle(unsigned char n);
[[noreturn]] static void halt(void) {
    out[1] = 0xAA;
    for (;;) {
    }
}
[[noreturn]] void reset_game(void) {
    out[0] = ++resets;
    if (resets == 200)
        halt();
    for (;;)
        out[2] = middle(resets);
}
int main(void) {
    reset_game();
}
)c";
// `out[2]` is `middle(199)`, which is 200 + 201, modulo 256.
const Bytes kGameOut = {200, 0xAA, (200 + 201) & 0xFF};

TEST_CASE("noreturn_call: a game that starts over runs in one unit") {
  const std::string src = kGameDeep + replace_all(kGameTop, "volatile unsigned char out[3];\n", "");
  const std::string asm_text = codegen(src, true);
  CHECK_FALSE_MESSAGE(soft_stack(asm_text), asm_text);
  CHECK(contains(body(asm_text, "deep"), "JMP   reset_game"));
  sim::Simulation s = build_sim(src, true);
  s.run(500'000);
  CHECK(out_bytes(s) == kGameOut);
}

TEST_CASE("noreturn_call: a game that starts over links from two units and runs") {
  // Before, the link failed with "cycle detected in zp_abi call
  // graph", since each unit's metadata had a `call` line for the
  // call to `reset_game`.
  TempDir d;
  CHECK(
      compile_object(replace_all(kGameDeep, "volatile unsigned char out[3];", "extern volatile unsigned char out[3];"),
                     d.file("deep.o")) == 0);
  CHECK(compile_object(kGameTop, d.file("top.o")) == 0);
  CHECK(link({d.file("deep.o"), d.file("top.o")}, d.file("game")).status == 0);
  std::string code;
  bool first = true;
  for (const std::string& l : lines(linked_text(d.file("game")))) {
    if (l.rfind(";", 0) == 0) continue;
    code += (first ? "" : "\n") + l;
    first = false;
  }
  // `main` falls into `reset_game`, so the disassembly names their one
  // address `main`.
  CHECK(search(R"(JMP   (main|reset_game)\n\.if_end)", code));
  CHECK_FALSE_MESSAGE(soft_stack(code), code);
  sim::Simulation s = build_linked_sim(d.file("game"));
  s.run(500'000);
  CHECK(out_bytes(s) == kGameOut);
}

}  // namespace
}  // namespace c6502::suite
