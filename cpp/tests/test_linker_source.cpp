// The tests of tests/test_linker.py that link units compiled with
// `-c -O` and read the executable's text. They check the slot symbols
// the linker allocates for the whole program, its errors for
// definitions and cycles across units, the binding of a declaration's
// parameter names to the definition's, and a cycle whose every call
// saves the hardware stack.
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "py_regex.h"

namespace {

namespace suite = c6502::suite;
using Equs = std::map<std::string, std::int64_t>;

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

// Returns the `name EQU $addr` bindings in a linked file, from the
// lines of three tab-separated fields whose second is `EQU`.
Equs equs(const std::string& text) {
  Equs out;
  for (const std::string& line : suite::lines(text)) {
    std::vector<std::string> parts;
    std::size_t at = 0;
    for (;;) {
      const std::size_t tab = line.find('\t', at);
      parts.push_back(line.substr(at, tab == std::string::npos ? std::string::npos : tab - at));
      if (tab == std::string::npos) break;
      at = tab + 1;
    }
    if (parts.size() == 3 && parts[1] == "EQU") {
      const std::size_t digits = parts[2].find_first_not_of('$');
      out[parts[0]] = std::stoll(parts[2].substr(digits), nullptr, 16);
    }
  }
  return out;
}

// Compiles `sources` with `-c -O` to the objects `names` in a new
// directory and links them with `extra` on the command line. Returns
// the exit status, stderr, and the linked program as `linked_text`
// gives it (empty when the link fails).
std::tuple<long long, std::string, std::string> link_units(
    const std::vector<std::pair<std::string, std::string>>& units,
    const std::vector<std::string>& extra = {}) {
  const suite::TempDir tmp;
  std::vector<std::string> paths;
  for (const auto& [name, src] : units) {
    const std::string path = tmp.file(name);
    REQUIRE(suite::compile_object(src, path) == 0);
    paths.push_back(path);
  }
  const std::string out = tmp.file("linked");
  const auto o = suite::link(paths, out, std::nullopt, extra);
  const long long rc = o.status.value_or(-1);
  return {rc, o.err(), rc == 0 ? suite::linked_text(out) : ""};
}

}  // namespace

TEST_CASE("linker: two units link with one block of slot symbols and metadata") {
  // TU A defines main and calls the extern helper, which TU B defines.
  // Both are zp_abi-annotated. Linker re-allocates so the symbols agree
  // across TUs. `base` arrives in A and X, so `x` arrives in its slot.
  const auto [rc, err, text] = link_units({
      {"a.o",
       "[[c6502::zp_abi]] extern int helper(int base, int x); "
       "int main(void) { return helper(1, 7); }"},
      {"b.o",
       "[[c6502::zp_abi]] int helper(int base, int x) { "
       "  return base + x; "
       "}"},
  });
  REQUIRE_MESSAGE(rc == 0, err);
  CHECK(suite::contains(text, "__zpabi_helper__x_0\tEQU\t"));
  CHECK(suite::contains(text, "main:"));
  CHECK(suite::contains(text, "helper:"));
  // One merged metadata block, and none of the units' own.
  CHECK(suite::count(text, "@zp-link-meta-begin") == 1);
}

TEST_CASE("linker: a register parameter's assignment reaches the other unit") {
  // The linker must reconstruct the ZpLayout with the params' register
  // assignment so the per-TU asm bodies agree on the calling
  // convention. The first register arg-byte goes in A, the second in
  // X, so TU A's main passes `a` in A and `b` in X, and TU B's helper
  // definition uses the entry-stub copies from A and X. The metadata
  // block carries the param register info between them.
  const auto [rc, err, text] = link_units({
      {"a.o",
       "extern char helper(register char a, register char b);"
       "int main(void) { return helper(5, 7); }"},
      {"b.o",
       "char helper(register char a, register char b) {"
       "    return (char)(a + b);"
       "}"},
  });
  REQUIRE_MESSAGE(rc == 0, err);
  // TU A's main loads `b` into X before the JSR (the direct-into-X
  // peephole renders `LDA #imm; TAX` as one `LDX #imm`), and doesn't
  // write `__zpabi_helper__b`, the slot the linker would write had it
  // lost the register assignment.
  const std::size_t main_idx = text.find("main:");
  const std::size_t helper_idx = text.find("helper:");
  REQUIRE(main_idx != std::string::npos);
  REQUIRE(helper_idx != std::string::npos);
  const std::string main_body = main_idx < helper_idx
                                    ? text.substr(main_idx, helper_idx - main_idx)
                                    : text.substr(main_idx);
  CHECK(py_search(R"(LDX\s+#)", main_body));
  CHECK_FALSE(suite::contains(main_body, "STA\t__zpabi_helper__b"));
  CHECK(suite::contains(text, "param_regs=A,X"));
}

TEST_CASE("linker: units that declare the same extern share its symbols") {
  // `base` arrives in A and X, so `x` arrives in its slot.
  const auto [rc, err, text] = link_units({
      {"a.o",
       "[[c6502::zp_abi]] extern int helper(int base, int x); "
       "[[c6502::zp_abi]] int caller_a(int n) { "
       "  return helper(0, n); "
       "}"},
      {"b.o",
       "[[c6502::zp_abi]] extern int helper(int base, int x); "
       "[[c6502::zp_abi]] int caller_b(int n) { "
       "  return helper(0, n + 1); "
       "}"},
      {"c.o",
       "[[c6502::zp_abi]] int helper(int base, int x) { "
       "  return base + x + 100; "
       "}"
       "int main(void) { return 0; }"},
  });
  REQUIRE_MESSAGE(rc == 0, err);
  CHECK(suite::count(text, "__zpabi_helper__x_0\tEQU\t") == 1);
}

TEST_CASE("linker: a function defined in two units is a link error") {
  const auto [rc, err, text] = link_units({
      {"a.o",
       "[[c6502::zp_abi]] int helper(int x) { "
       "  return x + 1; "
       "}"},
      {"b.o",
       "[[c6502::zp_abi]] int helper(int x) { "
       "  return x + 2; "
       "}"
       "int main(void) { return helper(3); }"},
  });
  CHECK(rc == 1);
  CHECK(suite::contains(err, "multiple TUs"));
  CHECK(suite::contains(err, "helper"));
}

TEST_CASE("linker: an extern without the annotation links once --defsym gives it") {
  // Under `-O` every extern is zp_abi at the call site, and here `x`
  // arrives in A and X, since the declaration names no register. No
  // object defines the function, so the link fails on the undefined
  // symbol until `--defsym` gives its address.
  const std::vector<std::pair<std::string, std::string>> unit = {
      {"a.o",
       "extern int regular_lib_fn(int x); "
       "int main(void) { return regular_lib_fn(7); }"}};
  {
    const auto [rc, err, text] = link_units(unit);
    CHECK(rc == 1);
    CHECK(suite::contains(err, "undefined symbol `regular_lib_fn`"));
  }
  const auto [rc, err, text] = link_units(unit, {"--defsym", "regular_lib_fn=0x6000"});
  CHECK_MESSAGE(rc == 0, err);
}

TEST_CASE("linker: recursion across units is a link error") {
  // Neither unit sees the cycle between `f` and `g`. The link used to
  // fail in the slot allocator, with a message that called the cycle an
  // internal error, when the functions take parameters in slots, and on
  // the undefined `__local_f__t` when they don't.
  const std::vector<std::tuple<std::string, std::string, std::string>> cases = {
      {"slots",
       "void g(unsigned char a, unsigned char b, unsigned char c);\n"
       "void f(unsigned char a, unsigned char b, unsigned char c)"
       " { g(a, b, c); }\n",
       "void f(unsigned char a, unsigned char b, unsigned char c);\n"
       "void g(unsigned char a, unsigned char b, unsigned char c)"
       " { if (a) f(a - 1, b, c); }\n"
       "int main(void) { g(3, 4, 5); return 0; }\n"},
      {"locals",
       "extern unsigned char n;\nvoid g(void);\n"
       "void f(void) { volatile unsigned char t = n; if (t) g(); }\n",
       "unsigned char n = 3;\nvoid f(void);\n"
       "void g(void) { n--; f(); }\n"
       "int main(void) { g(); return 0; }\n"},
  };
  for (const auto& [label, tu_a, tu_b] : cases) {
    INFO(label);
    const auto [rc, err, text] = link_units({{"a.o", tu_a}, {"b.o", tu_b}});
    CHECK(rc == 1);
    CHECK(suite::contains(err,
                          "link error: function(s) on a cycle of calls between "
                          "units, which can't have private pools (recursion is "
                          "supported only within one unit, when every call in the "
                          "cycle is marked `[[c6502::save_hardware_stack]]`): "
                          "['f', 'g']"));
  }
}

TEST_CASE("linker: a link with no inputs is argparse's usage error") {
  const suite::TempDir tmp;
  const auto o = suite::link({}, tmp.file("linked"));
  CHECK(o.exited);
  REQUIRE(o.status.has_value());
  CHECK(*o.status != 0);
}

namespace {

// A zp_abi parameter's slot is named after the parameter, so a
// declaration whose names differ from the definition's spells the same
// byte differently, and an unnamed one spells it by position. Each
// `put` here takes a leading `unsigned int`, which arrives in A and X,
// so that the parameters after it arrive in their slots.
const std::string kPutDef =
    "unsigned char put(unsigned int w, unsigned char c, int n) "
    "{ return w + c + n; }";

std::tuple<long long, std::string, std::string> link_with(const std::string& decl) {
  return link_units(
      {{"a.o", kPutDef}, {"b.o", decl + " int main(void) { return put(2, 4, 300); }"}});
}

void check_bound_like_definition(const std::string& text,
                                 const std::vector<std::pair<std::string, std::string>>& names) {
  const Equs e = equs(text);
  for (const auto& [decl_sym, def_sym] : names) {
    INFO(decl_sym);
    CHECK(e.at(decl_sym) == e.at(def_sym));
  }
}

}  // namespace

TEST_CASE("linker: a declaration's renamed parameters bind to the definition's slots") {
  const auto [rc, err, text] =
      link_with("unsigned char put(unsigned int w, unsigned char x, int count);");
  REQUIRE_MESSAGE(rc == 0, err);
  check_bound_like_definition(text, {
                                        {"__zpabi_put__x", "__zpabi_put__c"},
                                        {"__zpabi_put__count_0", "__zpabi_put__n_0"},
                                        {"__zpabi_put__count_1", "__zpabi_put__n_1"},
                                    });
}

TEST_CASE("linker: a declaration's unnamed parameters bind to the definition's slots") {
  const auto [rc, err, text] = link_with("unsigned char put(unsigned int, unsigned char, int);");
  REQUIRE_MESSAGE(rc == 0, err);
  check_bound_like_definition(text, {
                                        {"__zpabi_put__1", "__zpabi_put__c"},
                                        {"__zpabi_put__2_0", "__zpabi_put__n_0"},
                                        {"__zpabi_put__2_1", "__zpabi_put__n_1"},
                                    });
}

TEST_CASE("linker: parameter names swapped between the units are a link error") {
  // `__zpabi_put__a` would be the first byte in one unit and the
  // second in the other.
  const auto [rc, err, text] = link_units({
      {"a.o",
       "unsigned char put(unsigned int w, unsigned char a, "
       "unsigned char b) { return w + a - b; }"},
      {"b.o",
       "unsigned char put(unsigned int w, unsigned char b, "
       "unsigned char a);"
       " int main(void) { return put(0, 4, 3); }"},
  });
  CHECK(rc == 1);
  CHECK(suite::contains(err, "same names"));
}

TEST_CASE("linker: swapped names of different sizes don't collide") {
  // `n` and `c` swap, but one is 1 byte and the other 2, so their
  // symbols (`__zpabi_put__n` and `__zpabi_put__n_0`) differ.
  const auto [rc, err, text] =
      link_with("unsigned char put(unsigned int w, unsigned char n, int c);");
  REQUIRE_MESSAGE(rc == 0, err);
  check_bound_like_definition(text, {
                                        {"__zpabi_put__n", "__zpabi_put__c"},
                                        {"__zpabi_put__c_0", "__zpabi_put__n_0"},
                                    });
}

TEST_CASE("linker: parameters of different sizes in the two units are a link error") {
  const auto [rc, err, text] =
      link_with("unsigned char put(unsigned int w, unsigned char c, long n);");
  CHECK(rc == 1);
  CHECK(suite::contains(err, "parameter byte"));
}

TEST_CASE("linker: a call of a function declared with an unnamed parameter compiles") {
  // It used to crash `-O`, with or without the annotation.
  for (const char* attr : {"", "[[c6502::zp_abi]] "}) {
    INFO(attr);
    const std::string src = std::string(attr) +
                            "unsigned char put(unsigned char);\n"
                            "int main(void) { return put(4); }\n";
    CHECK(suite::contains(suite::codegen(src, true), "__zpabi_put__0"));
  }
}

TEST_CASE("linker: a cycle whose every call saves the hardware stack links") {
  // As Zork's print_zstring does.
  const auto [rc, err, text] = link_units({{"a.o",
                                            "\n"
                                            "    unsigned char depth;\n"
                                            "    unsigned char out;\n"
                                            "    void walk(unsigned char n) {\n"
                                            "        unsigned char here = n;\n"
                                            "        if (n == 0) return;\n"
                                            "        [[c6502::save_hardware_stack]] walk(n - 1);\n"
                                            "        out = out * 3 + here;\n"
                                            "    }\n"
                                            "    int main(void) { walk(3); return out; }\n"}});
  REQUIRE_MESSAGE(rc == 0, err);
  CHECK(suite::contains(text, "JSR   walk"));
}

TEST_CASE("linker: a cycle without the attribute doesn't link") {
  const std::string src =
      "unsigned char out;\n"
      "void walk(unsigned char n) { if (n) { walk(n - 1); out++; } }\n"
      "int main(void) { walk(3); return out; }\n";
  const suite::TempDir tmp;
  const std::string obj = tmp.file("a.o");
  // Without the attribute the compile itself falls back to the soft
  // stack, which the linker can't re-allocate.
  if (suite::compile_object(src, obj) != 0) return;
  const auto o = suite::link({obj}, tmp.file("linked"));
  REQUIRE(o.status.has_value());
  CHECK(*o.status != 0);
}
