// The tests of tests/test_recursion_saves.py that compile a source:
// `[[c6502::save_hardware_stack]]` on a call that can't recurse or goes
// through a pointer, and the code of a marked call, which saves the
// caller's live values with PHA and PLA.
#include <cctype>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// The attribute, before a statement or declaration. In the programs
// below, `SAVE ` stands where it goes.
const std::string kSave = "[[c6502::save_hardware_stack]] ";

std::string with_saves(std::string src, const std::string& save = kSave) {
  for (std::size_t at = src.find("SAVE "); at != std::string::npos;
       at = src.find("SAVE ", at + save.size())) {
    src.replace(at, 5, save);
  }
  return src;
}

// The lines of function `name` in `asm_text`, joined by newlines, up to
// the next line that is a label at column 0 other than a local one.
std::string function(const std::string& asm_text, const std::string& name) {
  const std::vector<std::string> lines = suite::lines(asm_text);
  std::size_t start = 0;
  while (start < lines.size() && lines[start] != name + ":") ++start;
  REQUIRE(start < lines.size());
  std::size_t end = start + 1;
  while (end < lines.size()) {
    const std::string& l = lines[end];
    if (!l.empty() && !std::isspace(static_cast<unsigned char>(l[0])) && l.back() == ':' &&
        l[0] != '.') {
      break;
    }
    ++end;
  }
  std::string out;
  for (std::size_t i = start; i < end; ++i) out += (i == start ? "" : "\n") + lines[i];
  return out;
}

suite::Error rejected(const std::string& src, bool optimize) {
  suite::Options o;
  o.optimize = optimize;
  return suite::compile_error(suite::Stage::kCodegen, src, o);
}

const std::string kDescend = R"(
unsigned char level;
unsigned char deepest;
unsigned char remaining;

void descend(void) {
    unsigned char here = level;
    if (here > deepest) deepest = here;
    if (remaining != 0) {
        remaining = remaining - 1;
        level = here + 1;
        SAVE descend();
        level = here;
    }
}

int main(void) {
    remaining = 5;
    descend();
    return deepest * 10 + level;
}
)";

}  // namespace

TEST_CASE("recursion_saves: a marked call that can't recurse is rejected") {
  const std::string src =
      "int g(int x) { return x; }\nint f(int x) {\n" + kSave + "return g(x);\n}\n";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const suite::Error e = rejected(src, optimize);
    CHECK(suite::is_a(e.type, "RecursionSaveError"));
    CHECK(suite::contains(e.message, "'g' can't call 'f' again"));
  }
}

TEST_CASE("recursion_saves: a marked call through a pointer is rejected") {
  const std::string src = "int f(int x) {\nint (*p)(int) = f;\nif (x == 0) return 0;\n" + kSave +
                          "return p(x - 1);\n}\n";
  const suite::Error e = rejected(src, true);
  CHECK(suite::is_a(e.type, "RecursionSaveError"));
  CHECK(suite::contains(e.message, "function pointer"));
}

TEST_CASE("recursion_saves: the live values are saved around the marked call") {
  const std::string body = function(suite::codegen(with_saves(kDescend), true), "descend");
  CHECK_FALSE(suite::contains(body, "prologue"));
  CHECK(suite::contains(body,
                        "   LDA   __local_descend__here\n   PHA\n   JSR   descend\n"
                        "   PLA\n   STA   level\n"));
}

TEST_CASE("recursion_saves: without the attribute the function saves callee-saved bytes") {
  const std::string body = function(suite::codegen(with_saves(kDescend, ""), true), "descend");
  CHECK(suite::contains(body, "callee-saved"));
  CHECK_FALSE(suite::contains(body, "PHA"));
}

TEST_CASE("recursion_saves: the saves push from and pull into globals") {
  const std::string src = with_saves(R"(
unsigned char a, b, depth;
void load(void) { depth++; }
void f(void) {
    if (depth < 2) {
        unsigned char sa = a;
        unsigned char sb = b;
        load();
        SAVE f();
        b = sb;
        a = sa;
    }
}
int main(void) { f(); return depth; }
)");
  const std::string body = function(suite::codegen(src, true), "f");
  CHECK(suite::contains(body,
                        "   LDA   a\n   PHA\n   LDA   b\n   PHA\n   JSR   load\n"
                        "   JSR   f\n   PLA\n   STA   b\n   PLA\n   STA   a\n"));
}

TEST_CASE("recursion_saves: a caller of the cycle keeps zp_abi") {
  // rec has a pool, so f0 can call it and read its own parameter slot
  // afterwards. f0 takes `d` in A, so `p0` arrives in its slot.
  const std::string src = with_saves(R"(
int rec(int d) {
    int k = d * 3;
    if (d <= 0) return 0;
    SAVE int r = rec(d - 1);
    return r + 1 + (k & 1);
}
int f0(unsigned char d [[c6502::in_a]], int p0) {
    int l = rec(3);
    return p0 + l + d;
}
int main(void) { return f0(2, 4); }
)");
  const std::string body = function(suite::codegen(src, true), "f0");
  CHECK_FALSE(suite::contains(body, "prologue"));
  const std::size_t call = body.find("JSR   rec");
  REQUIRE(call != std::string::npos);
  std::string after = body.substr(call + 9);
  after = after.substr(0, after.find("JSR   rec"));
  CHECK(suite::contains(after, "__zpabi_f0__p0_0"));
}

TEST_CASE("recursion_saves: an unmarked call on the cycle keeps the callee saves") {
  const std::string src = with_saves(R"(
int pong(int n);
int ping(int n) {
    int a = n * 2;
    if (n <= 0) return 0;
    SAVE int r = pong(n - 1);
    return r + a;
}
int pong(int n) {
    int b = n + 5;
    if (n <= 0) return 1;
    int r = ping(n - 1);
    return r * 2 + b;
}
int main(void) { return ping(5); }
)");
  const std::string asm_text = suite::codegen(src, true);
  CHECK(suite::contains(function(asm_text, "ping"), "callee-saved"));
  CHECK_FALSE(suite::contains(function(asm_text, "ping"), "PHA\n   JSR"));
}
