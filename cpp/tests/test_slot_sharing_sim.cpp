// Values kept in a callee's parameter bytes across calls
// (src/passes/slot_sharing.h), parameters at a fixed zero-page address
// (`zeropage(addr)` on a parameter), and register parameters, each
// checked in the `-O` text and run in both pipelines, from
// tests/test_slot_sharing.py. Two of the programs are linked from two
// units.
//
// Under -O, a function whose declaration names no register takes its
// leading parameters in A and X, and a parameter in a register has no
// bytes to share. So each function here whose parameters are to arrive
// in slots names a register for one parameter, such as
// `[[c6502::in_a]]` on a byte, and the default is off for the rest of
// its parameters.
#include <cctype>
#include <cstdint>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

std::string codegen_of(const std::string& src, bool optimize = true, bool inline_calls = true) {
  Options options;
  options.optimize = optimize;
  options.inline_calls = inline_calls;
  return codegen(src, options);
}

std::int64_t run_of(const std::string& src, bool optimize, bool inline_calls = true) {
  SimOptions options;
  options.optimize = optimize;
  options.inline_calls = inline_calls;
  return build_sim(src, options).run().return_int();
}

// The `share` link-metadata records of the `-S` text.
std::vector<std::string> shares(const std::string& asm_text) {
  const std::string prefix = "; share ";
  std::vector<std::string> out;
  for (const std::string& line : lines(asm_text)) {
    if (line.rfind(prefix, 0) == 0) out.push_back(line.substr(prefix.size()));
  }
  return out;
}

bool any_holds_main(const std::vector<std::string>& records) {
  for (const std::string& r : records) {
    if (contains(r, "holder=main")) return true;
  }
  return false;
}

// The instruction lines of `fn`'s body, each with its runs of spaces
// made one, up to the first line that starts in the first column with
// anything but a local label.
std::vector<std::string> body(const std::string& asm_text, const std::string& fn) {
  const std::string head = "\n" + fn + ":\n";
  const std::size_t at = asm_text.find(head);
  REQUIRE(at != std::string::npos);
  std::string text = asm_text.substr(at + head.size());
  const std::size_t next = text.find(head);
  if (next != std::string::npos) text = text.substr(0, next);
  std::vector<std::string> out;
  for (const std::string& line : lines(text)) {
    if (!line.empty() && line[0] != ' ' && line[0] != '\t' && line[0] != '.') break;
    if (line.rfind("   ", 0) != 0) continue;
    std::string joined;
    std::size_t i = 0;
    while (i < line.size()) {
      while (i < line.size() && std::isspace(static_cast<unsigned char>(line[i]))) ++i;
      std::size_t j = i;
      while (j < line.size() && !std::isspace(static_cast<unsigned char>(line[j]))) ++j;
      if (j > i) joined += (joined.empty() ? "" : " ") + line.substr(i, j - i);
      i = j;
    }
    if (joined != "SUBROUTINE") out.push_back(joined);
  }
  return out;
}

// The text's `NAME<tab>EQU<tab>$HEX` lines, by name.
std::map<std::string, std::int64_t> equs(const std::string& asm_text) {
  static const std::regex re(R"(^(\S+)\tEQU\t\$([0-9A-F]+)$)");
  std::map<std::string, std::int64_t> out;
  for (const std::string& line : lines(asm_text)) {
    std::smatch m;
    if (std::regex_match(line, m, re)) out[m[1]] = std::stoll(m[2], nullptr, 16);
  }
  return out;
}

bool contains_line(const std::vector<std::string>& v, const std::string& s) {
  for (const std::string& x : v) {
    if (x == s) return true;
  }
  return false;
}

struct Linked {
  long long status = 0;
  std::string text;
  std::string err;
  std::string exe;
};

// Compiles each source with -O to an object and links them, as the
// Python test's `_link` did.
Linked link_units(const std::vector<std::string>& sources, bool inline_calls = true) {
  TempDir tmp;
  std::vector<std::string> paths;
  MainOptions options;
  options.inline_calls = inline_calls;
  for (std::size_t k = 0; k < sources.size(); ++k) {
    const std::string path = tmp.file("u" + std::to_string(k) + ".o");
    const compile_command::Outcome o = compile_main({"compile.py", "-", "-c", "-O", "-o", path}, sources[k], options);
    REQUIRE(o.status.has_value());
    REQUIRE(*o.status == 0);
    paths.push_back(path);
  }
  const std::string out = tmp.file("out");
  const compile_command::Outcome o = link(paths, out);
  Linked r;
  r.status = o.status.value_or(-1);
  r.err = o.err();
  if (r.status == 0) {
    r.text = linked_text(out);
    r.exe = read_file(out);
  }
  return r;
}

std::int64_t pages_sum() {
  std::int64_t s = 0;
  for (int k = 0; k < 512; ++k) s += (k * 7) & 0xFF;
  return s & 0xFFFF;
}

const std::string kPages = R"c(
#include <stdint.h>
uint8_t buf[600];
uint16_t sum;
static void add_page(uint8_t i [[c6502::in_a]], const uint8_t *p) {
    do { sum += p[i]; i++; } while (i != 0);
}
static void forward(uint8_t i [[c6502::in_a]], const uint8_t *p) {
    add_page(i, p);
}
int main(void) {
    for (uint16_t k = 0; k < 600; k++) buf[k] = (uint8_t)(k * 7);
    const uint8_t *p = buf;
    uint8_t pages = 2;
    do {
        forward(0, p);
        p += 0x100;
    } while (--pages != 0);
    return sum;
}
)c";
const std::string kFirst = R"c(
#include <stdint.h>
uint8_t a[2] = {10, 20}, b[2] = {3, 4};
static uint8_t first(uint8_t k [[c6502::in_a]], const uint8_t *p) {
    return p[k];
}
)c";
const std::string kUnitDef = R"c(
#include <stdint.h>
uint16_t total;
void add_page(uint8_t i [[c6502::in_a]], const uint8_t *const p) {
    do { total += p[i]; i++; } while (i != 0);
}
)c";
const std::string kUnitUse = R"c(
#include <stdint.h>
extern uint16_t total;
void add_page(uint8_t i [[c6502::in_a]], const uint8_t *const p);
uint8_t buf[512];
int main(void) {
    for (uint16_t k = 0; k < 512; k++) buf[k] = (uint8_t)(k * 7);
    const uint8_t *p = buf;
    uint8_t pages = 2;
    do {
        add_page(0, p);
        p += 0x100;
    } while (--pages != 0);
    return total;
}
)c";
// A routine at $D000 that returns the sum of the X bytes at ($E6),Y:
// LDY #0; LDA #0; loop: CLC; ADC ($E6),Y; INY; DEX; BNE loop; RTS.
const std::string kTotal = R"c(
[[c6502::zp_abi, c6502::address(0xD000)]]
unsigned char total(const unsigned char *p [[c6502::zeropage(0xE6)]],
                    register unsigned char n [[c6502::in_x]]);
)c";
const std::vector<std::uint8_t> kTotalCode = {0xA0, 0x00, 0xA9, 0x00, 0x18, 0x71,
                                              0xE6, 0xC8, 0xCA, 0xD0, 0xF9, 0x60};
// A C function that sums the n bytes at p, taking p at $30 and n in X.
const std::string kSum = R"c(
unsigned char sum(const unsigned char *p [[c6502::zeropage(0x30)]],
                  register unsigned char n [[c6502::in_x]]) {
    unsigned char s = 0, i = 0;
    do { s += p[i]; i++; } while (--n != 0);
    return s;
}
)c";
const std::string kSumDecl = R"c(
unsigned char sum(const unsigned char *p [[c6502::zeropage(0x30)]],
                  register unsigned char n [[c6502::in_x]]);
)c";
const std::string kSumMain = R"c(
unsigned char data[4] = {1, 2, 3, 4};
int main(void) { return sum(data, 4) + sum(data + 1, 2); }
)c";

// The programs of the Python module's `TestShares` and `TestAcrossUnits`
// ran with inlining off.
void check_runs_no_inlining(const std::string& src, std::int64_t want) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run_of(src, optimize, false) == want);
  }
}

}  // namespace

TEST_CASE("slot_sharing sim: a forwarded parameter and a local held in its bytes") {
  const std::string text = codegen_of(kPages, true, false);
  const std::vector<std::string> records = shares(text);
  REQUIRE(records.size() == 1);
  CHECK(contains(records[0], "member=add_page:"));
  CHECK(contains(records[0], "member=forward:"));
  CHECK(contains(records[0], "holder=main"));
  // `forward` passes its parameter on where it already is.
  CHECK(body(text, "forward") == std::vector<std::string>{"JMP add_page"});
  // The two slots are one pair of bytes.
  const auto e = equs(text);
  CHECK(e.at("__zpabi_forward__p_0") == e.at("__zpabi_add_page__p_0"));
  // `main` keeps `p` there, and its `p += 0x100` (out of SSA, a copy
  // through a temporary) updates it in place.
  const std::string marker = ".loop@2_start:";
  const std::size_t at = text.find(marker);
  REQUIRE(at != std::string::npos);
  std::string loop = text.substr(at + marker.size());
  loop = loop.substr(0, loop.find("RTS"));
  CHECK_FALSE(search(R"(__local_main__p\b)", loop));
  // The high byte of `p += 0x100` is an `INC`.
  CHECK(contains(loop, "INC   __zpabi_forward__p_1"));
  check_runs_no_inlining(kPages, pages_sum());
}

TEST_CASE("slot_sharing sim: a stepped counter kept in the bytes a callee passes on") {
  // Each `++s` is a new value, `t = s + 1` with `s` dead after, and all
  // of them, and `first`'s result, share one home: the bytes `take_next`
  // passes on to `take`. The call's result is stored there, each step is
  // an add in place, and nothing is copied into `take`'s spelling of the
  // bytes.
  const std::string src = R"c(
#include <stdint.h>
uint16_t sum;
static void take(uint8_t w [[c6502::in_a]], const uint16_t s) {
    sum += s + w;
}
static void take_next(uint8_t w [[c6502::in_a]], const uint16_t s) {
    take(w, s);
}
static uint16_t first(void) { return 40; }
int main(void) {
    uint16_t s = first();
    take_next(1, ++s);
    take_next(2, ++s);
    take(3, ++s);
    take(4, ++s);
    return sum;
}
)c";
  const std::string text = codegen_of(src, true, false);
  const std::vector<std::string> records = shares(text);
  REQUIRE(records.size() == 1);
  CHECK(contains(records[0], "member=take:"));
  CHECK(contains(records[0], "member=take_next:"));
  CHECK(contains(records[0], "holder=main"));
  CHECK(body(text, "take_next") == std::vector<std::string>{"JMP take"});
  const std::vector<std::string> main = body(text, "main");
  REQUIRE(main.size() >= 3);
  CHECK(std::vector<std::string>(main.begin(), main.begin() + 3) ==
        std::vector<std::string>{"JSR first", "STA __zpabi_take_next__s_0", "STX __zpabi_take_next__s_1"});
  for (const std::string& line : main) {
    INFO(line);
    CHECK_FALSE(contains(line, "__zpabi_take__s"));
    CHECK_FALSE(contains(line, "__local_main__"));
  }
  check_runs_no_inlining(src, 41 + 42 + 43 + 44 + 1 + 2 + 3 + 4);
}

TEST_CASE("slot_sharing sim: no share while another argument goes to the bytes") {
  // `p` is still needed after `first(0, b)`, which writes the bytes.
  const std::string src = kFirst + R"c(
        int main(void) {
            const uint8_t *p = a;
            uint8_t s = first(0, p);
            s += first(0, b);
            return s + first(0, p);
        }
        )c";
  CHECK(shares(codegen_of(src, true, false)).empty());
  check_runs_no_inlining(src, 10 + 3 + 10);
}

TEST_CASE("slot_sharing sim: no share while a call reaches a writer of the bytes") {
  // `other`, which `p` is live across, passes `first` another pointer.
  const std::string src = kFirst + R"c(
        static uint8_t other(void) { return first(0, b); }
        int main(void) {
            const uint8_t *p = a;
            uint8_t s = first(0, p);
            s += other();
            return s + first(0, p);
        }
        )c";
  CHECK(shares(codegen_of(src, true, false)).empty());
  check_runs_no_inlining(src, 10 + 3 + 10);
}

TEST_CASE("slot_sharing sim: a callee that steps a copy of its parameter") {
  // `sum` steps `p` through the bytes, which out of SSA is a copy of the
  // parameter, so the parameter's bytes stay as they were and `main` can
  // keep its pointer there.
  const std::string src = R"c(
        #include <stdint.h>
        uint8_t a[3] = {1, 2, 3};
        static uint8_t sum(const uint8_t *p, uint8_t n [[c6502::in_x]]) {
            uint8_t s = 0;
            do { s += *p++; } while (--n != 0);
            return s;
        }
        int main(void) {
            const uint8_t *p = a;
            uint8_t s = sum(p, 3);
            return s + sum(p, 2);
        }
        )c";
  CHECK(any_holds_main(shares(codegen_of(src, true, false))));
  check_runs_no_inlining(src, 6 + 3);
}

TEST_CASE("slot_sharing sim: a value held across a sibling with locals") {
  // `p` lives in `first`'s bytes across `noise`, so `noise`'s locals
  // keep off them though `noise` and `first` are siblings.
  const std::string src = kFirst + R"c(
        uint8_t out[8];
        static void noise(uint8_t k) {
            for (uint8_t i = 0; i < 8; i++) out[i] = (uint8_t)(i * k);
        }
        int main(void) {
            const uint8_t *p = a;
            uint8_t s = first(0, p);
            noise(5);
            s += first(0, p);
            noise(7);
            return s + first(0, p) + out[7];
        }
        )c";
  const std::string text = codegen_of(src, true, false);
  CHECK(any_holds_main(shares(text)));
  const auto e = equs(text);
  const std::set<std::int64_t> held = {e.at("__zpabi_first__p_0"), e.at("__zpabi_first__p_1")};
  for (const auto& [name, addr] : e) {
    if (name.rfind("__local_noise__", 0) == 0 || name.rfind("__zpabi_noise__", 0) == 0) {
      INFO(name);
      CHECK(held.count(addr) == 0);
    }
  }
  check_runs_no_inlining(src, 30 + 49);
}

TEST_CASE("slot_sharing sim: a program linked from two units runs") {
  const Linked l = link_units({kUnitDef, kUnitUse}, false);
  REQUIRE_MESSAGE(l.status == 0, l.err);
  CHECK(sim::build_linked_sim(l.exe).run().return_int() == pages_sum());
}

TEST_CASE("slot_sharing sim: a defined function reads its pinned parameter in place") {
  const std::string src = kSum + kSumMain;
  const std::string text = codegen_of(src);
  const auto e = equs(text);
  CHECK(e.at("__zpabi_sum__p_0") == 0x30);
  CHECK(e.at("__zpabi_sum__p_1") == 0x31);
  CHECK(contains_line(body(text, "sum"), "ADC (__zpabi_sum__p_0),Y"));
  CHECK(contains(text, "STA   __zpabi_sum__p_0"));
  CHECK(contains(text, "pinned=$30,$31,"));
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run_of(src, optimize) == 10 + 5);
  }
}

TEST_CASE("slot_sharing sim: a pinned parameter is copied when a callee takes one there") {
  // `first`'s callers store its argument where `both` took `q`, and
  // `both` reads `q` after calling it.
  const std::string src = R"c(
        unsigned char first(
            const unsigned char *p [[c6502::zeropage(0xE6)]]) {
            return p[0];
        }
        unsigned char both(
            const unsigned char *q [[c6502::zeropage(0xE6)]],
            const unsigned char *r) {
            unsigned char s = first(r);
            return s + q[1];
        }
        unsigned char a[2] = {10, 20}, b[2] = {3, 4};
        int main(void) { return both(a, b); }
        )c";
  const std::string text = codegen_of(src);
  const std::vector<std::string> both = body(text, "both");
  REQUIRE(both.size() >= 4);
  CHECK(std::vector<std::string>(both.begin(), both.begin() + 4) ==
        std::vector<std::string>{"LDA __zpabi_both__q_0", "STA __local_both__q", "LDA __zpabi_both__q_1",
                                 "STA __local_both__2"});
  std::vector<std::string> first_zpabi;
  for (const std::string& line : body(text, "first")) {
    if (contains(line, "__zpabi")) first_zpabi.push_back(line);
  }
  CHECK(first_zpabi == std::vector<std::string>{"LDA (__zpabi_first__p_0),Y"});
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run_of(src, optimize) == 3 + 20);
  }
}

TEST_CASE("slot_sharing sim: a pinned parameter needs zp_abi") {
  const std::string src = R"c(
        unsigned char f(const char *s [[c6502::zeropage(0xE6)]]) {
            return *s ? f(s + 1) + 1 : 0;
        }
        int main(void) { return f("abc"); }
        )c";
  Options options;
  options.optimize = true;
  const Error e = compile_error(Stage::kCodegen, src, options);
  CHECK(is_a(e.type, "AbiSelectionError"));
  CHECK_MESSAGE(search(R"(declares a `zeropage\(addr\)` parameter)", e.message), e.message);
  // Without -O, the function takes it on the soft stack, as its callers
  // pass it.
  CHECK(run_of(src, false) == 3);
}

TEST_CASE("slot_sharing sim: a pinned parameter across a link") {
  const std::string use = kSumDecl + kSumMain;
  const Linked l = link_units({kSum, use});
  REQUIRE_MESSAGE(l.status == 0, l.err);
  CHECK(equs(l.text).at("__zpabi_sum__p_0") == 0x30);
  CHECK(sim::build_linked_sim(l.exe).run().return_int() == 15);
  // A unit that puts the argument somewhere else doesn't link.
  std::string moved = use;
  for (std::size_t at = moved.find("0x30"); at != std::string::npos; at = moved.find("0x30", at + 4)) {
    moved.replace(at, 4, "0x40");
  }
  const Linked bad = link_units({kSum, moved});
  CHECK(bad.status != 0);
  CHECK_MESSAGE(contains(bad.err, "different parameters"), bad.err);
}

TEST_CASE("slot_sharing sim: arguments go to a pinned parameter's address") {
  const std::string src = kTotal + R"c(
        unsigned char data[4] = {1, 2, 3, 4};
        int main(void) { return total(data, 4) + total(data + 1, 2); }
        )c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::string text = codegen_of(src, optimize);
    const auto e = equs(text);
    CHECK(e.at("__zpabi_total__p_0") == 0xE6);
    CHECK(e.at("__zpabi_total__p_1") == 0xE7);
    CHECK(contains(text, "STA   __zpabi_total__p_0"));
    // Nothing else of the compiler's goes there.
    for (const auto& [name, addr] : e) {
      if ((addr == 0xE6 || addr == 0xE7) && !contains(name, "total")) {
        FAIL_CHECK("another name at the address: " << name);
      }
    }
    sim::Simulation s = build_sim(src, optimize);
    for (std::size_t k = 0; k < kTotalCode.size(); ++k) s.cpu.memory[0xD000 + k] = kTotalCode[k];
    CHECK(s.run().return_int() == 10 + 5);
  }
}

TEST_CASE("slot_sharing sim: a register parameter kept in its register sets no flags") {
  // From the calls fuzzer: `p1`'s local stays in X, where it arrives,
  // and its entry copy `Mov(X, X)` was taken for an `LDX`, so the `TXA`
  // before the sign test of `p1` went and the `BMI` tested `p0`, left in
  // the flags by the caller.
  const std::string src = R"c(
        unsigned char g0 = 104;
        unsigned long g2 = 132;
        int f0(register signed char p0, register signed char p1) {
            signed char l0 = ((p1 % (g2 | 1)) & (p0 % (g0 | 1)));
            return l0;
        }
        int main(void) {
            long r = (long)f0(215, 72);
            return (int)(r ^ (r >> 16));
        }
        )c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run_of(src, optimize) == 64);
  }
}

TEST_CASE("slot_sharing sim: a parameter passed on to two callees") {
  // The parameter shares both callees' bytes, so the function copies it
  // for neither, as Zork's `is_separator(c)` passes `c` to
  // `is_dict_separator` and then to `is_std_separator`. Each function
  // takes a step count in X, so that `c` arrives in its slot. 'a' and
  // 'b' are separators, 'c' and 'd' aren't, and is_ab('a') calls once,
  // the others twice.
  const std::string src = R"c(
    #include <stdint.h>
    #include <stdbool.h>
    uint8_t calls;
    static bool is_a(uint8_t step [[c6502::in_x]], uint8_t c) {
        calls += step;
        return c == 'a';
    }
    static bool is_b(uint8_t step [[c6502::in_x]], uint8_t c) {
        calls += step;
        return c == 'b';
    }
    static bool is_ab(uint8_t step [[c6502::in_x]], uint8_t c) {
        if (is_a(step, c)) return true;
        return is_b(step, c);
    }
    int main(void) {
        uint8_t n = 0;
        for (uint8_t c = 'a'; c != 'e'; c++)
            n = (uint8_t)(n << 1) | is_ab(1, c);
        return n * 16 + calls;
    }
    )c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run_of(src, optimize) == 0b1100 * 16 + 7);
  }
}

}  // namespace c6502::suite
