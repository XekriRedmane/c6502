// The tests of tests/test_cmp_zero_jump_fold.py that read the `-S -O`
// text. The simulator runs stay in Python.
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.rfind(prefix, 0) == 0;
}

bool ends_with(const std::string& s, const std::string& suffix) {
  return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// Returns `text[text.index(from):text.index(to)]`.
std::string between(const std::string& text, const std::string& from, const std::string& to) {
  const std::size_t start = text.find(from);
  const std::size_t end = text.find(to);
  if (start == std::string::npos || end == std::string::npos) {
    throw std::runtime_error("substring not found");
  }
  return end > start ? text.substr(start, end - start) : std::string();
}

// Returns the lines of function `name` from its label to the next
// function's, without the prologue's and epilogue's blocks (from their
// `; prologue:` or `; epilogue` comment to the next blank line), so that
// the SSP arithmetic of the frame doesn't count as the body's.
std::string body_without_frame(const std::string& text, const std::string& name) {
  std::string out;
  bool in_fn = false;
  bool in_frame_block = false;
  bool first = true;
  for (const std::string& line : suite::lines(text)) {
    if (starts_with(line, name + ":")) in_fn = true;
    if (!in_fn) continue;
    if (!line.empty() && line[0] != ' ' && line[0] != '\t' && line[0] != '.' &&
        ends_with(line, ":") && !starts_with(line, name + ":")) {
      break;
    }
    if (suite::contains(line, "; prologue:") || suite::contains(line, "; epilogue")) {
      in_frame_block = true;
      continue;
    }
    if (in_frame_block) {
      if (line.empty()) in_frame_block = false;
      continue;
    }
    out += (first ? "" : "\n") + line;
    first = false;
  }
  return out;
}

}  // namespace

TEST_CASE("cmp_zero_jump_fold: a byte compared with 0 is a bare branch on the load") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "int f(uint8_t a) { if (a == 0) return 1; return 2; }\n"
      "int main(void) { return f(0); }\n",
      true);
  const std::string body = body_without_frame(text, "f");
  CHECK_FALSE(suite::contains(body, "BVC"));
  CHECK_FALSE(suite::contains(body, "CMP"));
  CHECK(suite::contains(body, "BNE"));
}

TEST_CASE("cmp_zero_jump_fold: a byte loop's test is a 1-byte compare and BCC") {
  // The loop test is at the bottom of the loop
  // (cpp/src/passes/rotate_loop_test.h). The body reads `i`, or the loop
  // would count down to 0 (cpp/src/passes/optimization/count_down.h)
  // and have no compare.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "int main(void) {\n"
      "    uint8_t i;\n"
      "    int sum = 0;\n"
      "    for (i = 0; i < 105; i = i + 1) sum = sum + i;\n"
      "    return sum;\n"
      "}\n",
      true);
  CHECK_FALSE(suite::contains(text, "BVC   .jcmp_novf"));
  CHECK(suite::contains(text, "CMP   #$69\n   BCC   .main@rot@"));
}

TEST_CASE("cmp_zero_jump_fold: a signed byte >= 0 is a branch on the sign") {
  // `redundant_load_after_rmw` drops the load too, so the loop's tail
  // is `DEC; BPL`.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "int sum;\n"
      "int main(void) {\n"
      "    for (int8_t x = 15; x >= 0; x--) sum += 1;\n"
      "    return sum;\n"
      "}\n",
      true);
  CHECK_FALSE(suite::contains(text, "BVC   .jcmp_novf"));
  CHECK(suite::contains(text, "BPL"));
}

TEST_CASE("cmp_zero_jump_fold: a switch on a byte compares bytes") {
  // Each compare of the SSA-renamed extended value narrows to the byte.
  const std::string text =
      suite::codegen(R"(#include <stdint.h>
uint8_t pick(uint8_t n) {
    switch (n) {
    case 0: return 10;
    case 1: return 11;
    case 2: return 12;
    case 7: return 17;
    case 200: return 99;
    case 300: return 1;
    default: return 50;
    }
}
int main(void) {
    return pick(0) + pick(1) + pick(2) + pick(7) + pick(200)
        + pick(44);
}
)", true);
  const std::string body = between(text, "pick:", "main:");
  CHECK_FALSE(suite::contains(body, "CMP   #$00"));
  CHECK(suite::contains(body, "CMP   #$C8"));
}

TEST_CASE("cmp_zero_jump_fold: a compare kept as a value subtracts one byte") {
  suite::Options options;
  options.optimize = true;
  options.inline_calls = false;
  const std::string text = suite::codegen(
      R"(#include <stdint.h>
uint8_t g[8] = {0, 9, 10, 11, 127, 128, 200, 255};
static uint8_t test(uint8_t x) { return x < 10; }
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 8; i++)
        n = (uint16_t)(n << 1) | test(g[i]);
    return n;
}
)",
      options);
  const std::string body = between(text, "\ntest:", "\nmain:");
  CHECK_FALSE(suite::contains(body, "BVC"));
  CHECK_FALSE(suite::contains(body, "SBC   #$00"));
  CHECK(suite::contains(body, "#$0A"));
}

TEST_CASE("cmp_zero_jump_fold: a global byte compared with a constexpr byte is a byte compare") {
  // The constant's read is folded only after the compare reaches its
  // jump, where the global's extend is found in the same straight run.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "constexpr uint8_t M = 0x63;\n"
      "extern uint8_t T [[c6502::address(0x4239)]];\n"
      "void f(void) { if (T < M) T++; }\n",
      true);
  const std::size_t at = text.find("\nf:");
  REQUIRE(at != std::string::npos);
  const std::string body = text.substr(at);
  CHECK(suite::contains(body, "CMP   #$63"));
  CHECK_FALSE(suite::contains(body, "BVC"));
}

TEST_CASE("cmp_zero_jump_fold: a global isn't narrowed through a copy the loop after it makes") {
  const std::string src =
      "unsigned int z;\n"
      "void f(int p, unsigned char k) {\n"
      "  if (z < k) {\n"
      "    for (unsigned char i = 0; i < 4; i++) z = (p ? i : i);\n"
      "  }\n"
      "}\n"
      "int main(void) { z = 10; f(3, 5); return z; }\n";
  suite::Options o;
  o.optimize = true;
  const suite::Tac tac = suite::tac_program(src, o);
  for (const auto& tl : tac.program.top_level) {
    const auto* fn = std::get_if<c6502::tac_ast::Function>(&tl);
    if (fn == nullptr || fn->name != "f") continue;
    const c6502::tac_ast::JumpIfCmp* jump = nullptr;
    for (const auto& ins : fn->instructions) {
      jump = std::get_if<c6502::tac_ast::JumpIfCmp>(&ins);
      if (jump != nullptr) break;
    }
    REQUIRE(jump != nullptr);
    const auto* left = std::get_if<c6502::tac_ast::Var>(&jump->src1);
    const auto* right = std::get_if<c6502::tac_ast::Var>(&jump->src2);
    CHECK(((left != nullptr && left->name == "z") || (right != nullptr && right->name == "z")));
  }
}
