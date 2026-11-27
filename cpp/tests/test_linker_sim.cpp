#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "binutils/elf.h"
#include "compile_case.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "sim_case.h"
#include "sim_helpers.h"

namespace c6502::suite {
namespace {

std::map<std::string, std::int64_t> equs(const std::string& text) {
  std::map<std::string, std::int64_t> out;
  for (const std::string& line : lines(text)) {
    std::vector<std::string> parts;
    std::size_t at = 0;
    for (std::size_t tab; (tab = line.find('\t', at)) != std::string::npos; at = tab + 1) {
      parts.push_back(line.substr(at, tab - at));
    }
    parts.push_back(line.substr(at));
    if (parts.size() == 3 && parts[1] == "EQU") {
      std::string hex = parts[2];
      hex.erase(0, hex.find_first_not_of('$'));
      out[parts[0]] = std::stoll(hex, nullptr, 16);
    }
  }
  return out;
}

// Compiles `units` with `-c -O`, one object each, links them with the
// simulator's helper addresses, and returns the linked text, its EQU
// bindings, and what `main` of the program returns. Fails the case when
// a step fails.
struct Linked {
  std::string text;
  std::map<std::string, std::int64_t> equs;
  std::int64_t result = 0;
  // The size of each table half (`@lo`, `@hi`) in the executable, by
  // name.
  std::map<std::string, std::int64_t> halves;
};

Linked link_units(const std::vector<std::string>& units) {
  TempDir tmp;
  std::vector<std::string> objects;
  for (std::size_t k = 0; k < units.size(); ++k) {
    const std::string obj = tmp.file(cat("u", k, ".o"));
    REQUIRE(compile_object(units[k], obj) == 0);
    objects.push_back(obj);
  }
  const std::string out = tmp.file("linked");
  const auto o = link(objects, out);
  REQUIRE_MESSAGE(o.status == 0, o.err());
  Linked linked;
  linked.text = linked_text(out);
  linked.equs = equs(linked.text);
  linked.result = build_linked_sim(out).run().return_int();
  const binutils::elf::ElfFile elf = binutils::elf::read(read_file(out));
  for (const auto& s : elf.symbols) {
    if (s->type == binutils::elf::STT_OBJECT && (contains(s->name, "@lo") || contains(s->name, "@hi"))) {
      linked.halves[s->name] = s->size;
    }
  }
  return linked;
}

// A zp_abi parameter's slot is named after the parameter, so a
// declaration whose names differ from the definition's spells the same
// byte differently, and an unnamed one spells it by position. `put`
// takes a leading `unsigned int`, which arrives in A and X, so that the
// parameters after it arrive in their slots.
TEST_CASE("linker: an unnamed prototype and then the definition run") {
  const std::string src =
      "unsigned char put(unsigned int, unsigned char, int);\n"
      "int main(void) { return put(2, 4, 300); }\n"
      "unsigned char put(unsigned int w, unsigned char c, int n) { return w + c + n; }";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == ((2 + 4 + 300) & 0xFF));
  }
}

// A cycle whose every call is marked `[[c6502::save_hardware_stack]]`
// links and runs, as Zork's print_zstring does.
TEST_CASE("linker: a cycle whose calls save the hardware stack runs") {
  const std::string src = R"c(
    unsigned char depth;
    unsigned char out;
    void walk(unsigned char n) {
        unsigned char here = n;
        if (n == 0) return;
        [[c6502::save_hardware_stack]] walk(n - 1);
        out = out * 3 + here;
    }
    int main(void) { walk(3); return out; }
)c";
  // out = ((0 * 3 + 1) * 3 + 2) * 3 + 3
  CHECK(link_units({src}).result == 18);
}

// When the pool, $80-$FF, has no room left, slots go in the free bytes
// below it, around the runtime's block, before they spill out of zero
// page. `add3`'s leading `base` arrives in A and X, so that its other
// three parameters arrive in slots.
TEST_CASE("linker: parameters go below a full pool") {
  const std::string a =
      "#include <stdint.h>\n"
      "extern uint8_t busy [[c6502::zeropage(0x80)]][124];\n"
      "uint16_t add3(uint16_t base, uint16_t a, uint16_t b, uint16_t c);\n"
      "int main(void) { return add3(4, 1000, 200, 30) == 1234; }\n";
  const std::string b =
      "#include <stdint.h>\n"
      "uint16_t add3(uint16_t base, uint16_t a, uint16_t b, uint16_t c) {\n"
      "    return base + a + b + c;\n"
      "}\n";
  const Linked linked = link_units({a, b});
  std::map<std::string, std::int64_t> params;
  for (const auto& [name, addr] : linked.equs) {
    if (name.rfind("__zpabi_add3_", 0) == 0) params[name] = addr;
  }
  CHECK(params.size() == 6);
  const std::set<asdl::Int> runtime = passes::RuntimeLayout{}.bytes();
  for (const auto& [name, addr] : params) {
    INFO(name);
    CHECK(addr < 0x80);
    CHECK(runtime.count(addr) == 0);
  }
  CHECK(linked.result == 1);
}

// `f1` and `f2` read through their pointer parameters alike. In their
// unit, neither calls the other, so their parameter slots share
// `$80`-`$81`, and the reads are the same instructions, `LDA ($80),Y`.
// In the program, `f2` is called from `g`, whose parameter takes `$80`,
// so the linker moves `f2`'s slots and the two pointers no longer share
// bytes. Code that both functions run through the pointer has to stay
// in each of them, where `asm_emit` names the pointer by that
// function's own slot. Moved into a function of its own by
// `shared_tails` or `outline_runs`, it was named by `f1`'s slot alone,
// and `f2` read through `f1`'s bytes.
//
// Each function here takes a byte in X, `[[c6502::in_x]]`, so that the
// parameters after it arrive in their slots.
const std::string kPointerMain = R"c(#include <stdint.h>
extern uint8_t out[8];
void f1(uint8_t m [[c6502::in_x]], uint8_t *p);
void f2(uint8_t m [[c6502::in_x]], uint8_t *p);
uint8_t A[3] = {1, 2, 3};
uint8_t B[3] = {10, 20, 30};
void g(uint8_t m [[c6502::in_x]], uint8_t k) {
    f2(m, k ? B : A);
}
int main(void) {
    f1(3, A);
    g(4, 1);
    return out[0] + out[1] + out[2] + out[3] + out[7];
}
)c";

// Returns the unit that defines `f1` and `f2`, which store 1 and
// `mark2` last.
std::string readers(int mark2) {
  std::string src = "#include <stdint.h>\nuint8_t out[8];\n";
  for (const auto& [n, mark] : {std::pair{1, 1}, std::pair{2, mark2}}) {
    src += cat("void f", n, "(uint8_t m [[c6502::in_x]], uint8_t *p) {\n", "    out[3] = m;\n", "    out[0] = p[0];\n",
               "    out[1] = p[1];\n", "    out[2] = p[2];\n", "    out[7] = ", mark, ";\n", "}\n");
  }
  return src;
}

Linked link_readers(int mark2) {
  const Linked linked = link_units({readers(mark2), kPointerMain});
  CHECK(linked.equs.at("__zpabi_f1__p_0") != linked.equs.at("__zpabi_f2__p_0"));
  return linked;
}

TEST_CASE("linker: a run through a pointer stays in each function") {
  // The functions differ only in their last store, so the reads would
  // be a run that `outline_runs` moves.
  const Linked linked = link_readers(2);
  CHECK(linked.result == 10 + 20 + 30 + 4 + 2);
  CHECK_FALSE(contains(linked.text, "@run"));
}

TEST_CASE("linker: a tail through a pointer stays in each function") {
  // The functions are the same, so the reads would be in the tail that
  // `shared_tails` shares. The tail starts after them.
  const Linked linked = link_readers(1);
  CHECK(linked.result == 10 + 20 + 30 + 4 + 1);
  CHECK(contains(linked.text, "@tail"));
}

// Two units each define a `static` function `f`, and a `static`
// function `pick` that returns a pointer, which `-O` returns in a
// zero-page object of its own, `pick@ret`. Each unit spells its `f`'s
// slots `__zpabi_f__x` and `__local_f__*`, so the linker has to rename
// the slots with the function. Otherwise the merged metadata held two
// definitions of `f`, and the link failed. `main`'s `f` calls `a`, which
// calls the other unit's `f`, so the two functions' parameters must have
// different bytes, since the first `f` reads its `x` after the call.
// Each `f` takes `d` in A, `[[c6502::in_a]]`, so that `x` arrives in its
// slot.
const std::string kStaticA =
    "#include <stdint.h>\n"
    "static uint8_t T[2] = {40, 50};\n"
    "static uint8_t *pick(uint8_t k) { return &T[k & 1]; }\n"
    "static uint8_t f(uint8_t d [[c6502::in_a]], uint8_t x) {\n"
    "    uint8_t y = x + d;\n"
    "    return x + y + *pick(x);\n"
    "}\n"
    "uint8_t a(uint8_t x) { return f(1, x) + f(1, x + 1); }\n";
const std::string kStaticB =
    "#include <stdint.h>\n"
    "uint8_t a(uint8_t x);\n"
    "static uint8_t T[2] = {1, 2};\n"
    "static uint8_t *pick(uint8_t k) { return &T[k & 1]; }\n"
    "static uint8_t f(uint8_t d [[c6502::in_a]], uint8_t x) {\n"
    "    uint8_t y = a(x) + d;\n"
    "    return x + y + *pick(x);\n"
    "}\n"
    "int main(void) { return f(0, 5); }\n";

TEST_CASE("linker: two units' static functions of one name link and run") {
  const Linked linked = link_units({kStaticA, kStaticB});
  // Unit 0's `f` is called from `a`, which unit 1's `f` calls, so the
  // allocators placed them as caller and callee.
  CHECK(linked.equs.at("__zpabi_f__x.0") != linked.equs.at("__zpabi_f__x.1"));
  CHECK(linked.equs.count("__zpabi_f__x") == 0);
  CHECK(contains(linked.text, "def f.0 "));
  CHECK(contains(linked.text, "def f.1 "));
  CHECK(contains(linked.text, "call a -> f.0"));
  CHECK(contains(linked.text, "call f.1 -> a"));
  // a(5) = f.0(5) + f.0(6) = (5 + 6 + 50) + (6 + 7 + 40) = 114, and
  // f.1(5) = 5 + 114 + 2.
  CHECK(linked.result == 121);
}

TEST_CASE("linker: a static function beside an external one of its name") {
  // Unit 1's `f` has external linkage, so it keeps its name and its
  // slots, and only unit 0's is renamed.
  const std::string b =
      "#include <stdint.h>\n"
      "uint8_t a(uint8_t x);\n"
      "uint8_t f(uint8_t d [[c6502::in_a]], uint8_t x) {\n"
      "    uint8_t y = a(x) + d;\n"
      "    return x + y;\n"
      "}\n"
      "int main(void) { return f(0, 5); }\n";
  const Linked linked = link_units({kStaticA, b});
  CHECK(linked.equs.at("__zpabi_f__x.0") != linked.equs.at("__zpabi_f__x"));
  CHECK(linked.result == 119);
}

// The arrays the row-table tests read. M and B are 0x200 bytes apart, so
// the low bytes of their row addresses are the same.
const std::string kRows =
    "#include <stdint.h>\n"
    "extern uint8_t M [[c6502::address(0x6000)]][16][28];\n"
    "extern uint8_t B [[c6502::address(0x6200)]][16][28];\n";

// `-O` puts the halves of a 2-D array's table of row addresses in
// objects of their own (`A@lo`, `A@hi`), in each unit that indexes the
// array, and the linker keeps one copy of the halves with the same
// bytes. One access to a 16-row array of 28-byte rows saves more than
// its 34-byte table, so each function below makes a table.

// Returns a `main` that fills M and B, so that M[r][c] is r * 16 + c and
// B[r][c] is r + c, and returns the sum of what `reads` reads.
std::string row_main(const std::string& reads) {
  return cat("int main(void) {\n", "    for (uint8_t r = 0; r < 16; r++)\n",
             "        for (uint8_t c = 0; c < 28; c++) {\n", "            M[r][c] = (uint8_t)(r * 16 + c);\n",
             "            B[r][c] = (uint8_t)(r + c);\n", "        }\n", "    return ", reads, ";\n", "}\n");
}

TEST_CASE("linker: two units share a table of row addresses") {
  auto both = [](int k) { return cat("uint8_t both", k, "(uint8_t r, uint8_t c) { return M[r][c] + B[r][c]; }\n"); };
  const std::string u0 =
      kRows + both(0) + "uint8_t both1(uint8_t r, uint8_t c);\n" + row_main("both0(3, 4) * 10 + both1(15, 27)");
  const std::string u1 = kRows + both(1);
  const Linked linked = link_units({u0, u1});
  // Each unit made B@lo, B@hi, and M@hi, and one copy of each is left,
  // unit 0's.
  CHECK(linked.halves == std::map<std::string, std::int64_t>{{"B@lo.0", 17}, {"B@hi.0", 17}, {"M@hi.0", 17}});
  CHECK(contains(linked.text, "M@hi.0,X"));
  // M[15][27] is (240 + 27) & 0xFF.
  CHECK(linked.result == (52 + 7) * 10 + (11 + 42));
}

TEST_CASE("linker: three units read through one copy of different halves") {
  // Unit 1 indexes only M and makes M@lo and M@hi, unit 2 only B. M@lo
  // has the same bytes as unit 0's B@lo, so the program keeps unit 0's
  // three halves, and units 1 and 2 read through them.
  const std::string u0 = kRows +
                         "uint8_t m1(uint8_t r, uint8_t c);\n"
                         "uint8_t b2(uint8_t r, uint8_t c);\n"
                         "uint8_t both(uint8_t r, uint8_t c) { return M[r][c] + B[r][c]; }\n" +
                         row_main("both(1, 2) + m1(14, 5) * 2 + b2(15, 27) * 3");
  const std::string u1 = kRows + "uint8_t m1(uint8_t r, uint8_t c) { return M[r + 1][c]; }\n";
  const std::string u2 = kRows + "uint8_t b2(uint8_t r, uint8_t c) { return B[r][c]; }\n";
  const Linked linked = link_units({u0, u1, u2});
  CHECK(linked.halves == std::map<std::string, std::int64_t>{{"B@lo.0", 17}, {"B@hi.0", 17}, {"M@hi.0", 17}});
  CHECK(contains(linked.text, "M@hi.0+1,X"));
  CHECK(linked.result == (18 + 3) + ((15 * 16 + 5) & 0xFF) * 2 + 42 * 3);
}

TEST_CASE("linker: tables of row addresses that differ stay apart") {
  // T's table has 11 row addresses, M's 17, so they share no half
  // although T's rows are M's first rows. Each unit's static S is an
  // array of its own, so their tables, whose bytes are relocations
  // against S.0 and S.1, stay apart too. A global G is one array, so
  // both units' tables of it are one.
  auto s = [](int k) {
    return cat("static uint8_t S[20][14];\n", "uint8_t s", k, "(uint8_t r, uint8_t c) {\n", "    S[r][c] = ", k,
               " + 1;\n", "    return S[r][c] + S[r - 1][c] + G[r][c] + G[r - 1][c];\n", "}\n");
  };
  const std::string u0 = kRows + "uint8_t G[20][14];\n" + s(0) +
                         "uint8_t t1(uint8_t r, uint8_t c);\n"
                         "uint8_t s1(uint8_t r, uint8_t c);\n"
                         "uint8_t m(uint8_t r, uint8_t c) { return M[r][c]; }\n" +
                         row_main("(G[19][13] = 7, m(2, 3) + t1(9, 27) * 2 + s0(19, 13) * 3 + s1(19, 13) * 5)");
  // T is the first 10 rows of M's memory. A unit can't declare both.
  const std::string u1 =
      "#include <stdint.h>\n"
      "extern uint8_t T [[c6502::address(0x6000)]][10][28];\n"
      "extern uint8_t G[20][14];\n" +
      s(1) + "uint8_t t1(uint8_t r, uint8_t c) { return T[r][c]; }\n";
  const Linked linked = link_units({u0, u1});
  CHECK(linked.halves == std::map<std::string, std::int64_t>{{"B@lo", 17},
                                                             {"B@hi", 17},
                                                             {"M@hi", 17},
                                                             {"T@lo", 11},
                                                             {"T@hi", 11},
                                                             {"S@lo.0", 21},
                                                             {"S@hi.0", 21},
                                                             {"S@lo.1", 21},
                                                             {"S@hi.1", 21},
                                                             {"G@lo.0", 21},
                                                             {"G@hi.0", 21}});
  // m(2, 3) = 35, t1(9, 27) = (144 + 27) & 0xFF, s0 = 1 + 0 + 7,
  // s1 = 2 + 0 + 7.
  CHECK(linked.result == 35 + 171 * 2 + 8 * 3 + 9 * 5);
}

}  // namespace
}  // namespace c6502::suite
