// The tests of tests/test_runtime_layout.py that read a program's `-S`
// text or a link's symbols. They check where SSP, FP, HARGS, and DPTR
// go when the program reserves bytes of the classic layout, how HARGS
// is sized, the EQU lines that bind them, and the layout a link
// chooses for the whole program.
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "passes/asm_context.h"

namespace {

namespace suite = c6502::suite;
using c6502::passes::RuntimeLayout;
using Equs = std::map<std::string, std::int64_t>;

std::string codegen(const std::string& src, bool optimize = true) {
  return suite::codegen(src, optimize);
}

// Returns the bindings of the lines that `^(\S+)\tEQU\t\$([0-9A-F]+)$`
// matches.
Equs equ(const std::string& asm_text) {
  Equs out;
  for (const std::string& line : suite::lines(asm_text)) {
    const std::size_t tab = line.find("\tEQU\t$");
    if (tab == std::string::npos || tab == 0) continue;
    const std::string name = line.substr(0, tab);
    const std::string hex = line.substr(tab + 6);
    if (name.find_first_of(" \t") != std::string::npos || hex.empty() ||
        hex.find_first_not_of("0123456789ABCDEF") != std::string::npos) {
      continue;
    }
    out[name] = std::stoll(hex, nullptr, 16);
  }
  return out;
}

// The layout the EQU lines give, with HARGS 32 bytes long, as Python's
// `RuntimeLayout(ssp=..., fp=..., hargs=..., dptr=...)` makes it.
RuntimeLayout layout_of(const std::string& asm_text) {
  const Equs e = equ(asm_text);
  RuntimeLayout l;
  l.ssp = e.at("SSP");
  l.fp = e.at("FP");
  l.hargs = e.at("HARGS");
  l.dptr = e.at("DPTR");
  return l;
}

// Returns the number after the first `key` in `text`, which
// `re.search(key + r"(\d+)", text)` gives.
std::int64_t number_after(const std::string& text, const std::string& key) {
  std::size_t at = 0;
  for (;;) {
    at = text.find(key, at);
    REQUIRE(at != std::string::npos);
    const std::size_t digits = at + key.size();
    const std::size_t end = text.find_first_not_of("0123456789", digits);
    if (end != digits) return std::stoll(text.substr(digits, end - digits));
    at = digits;
  }
}

// The Monitor's zero-page bytes in the unenhanced Apple IIe F8 ROM,
// $1F through $49 (one array, so they're reserved as one object).
const std::string kMonitor =
    "extern volatile unsigned char monitor_zp "
    "[[c6502::address(0x1F)]][0x2B];\n";

// A program that uses SSP and FP (recursion), DPTR (pointers), and
// HARGS (a 32-bit multiply and divide and an int / float round trip).
const std::string kSrc =
    "unsigned char buf[8];\n"
    "unsigned long lm(unsigned long a, unsigned long b) "
    "{ return a * b / 7; }\n"
    "int sum(unsigned char *p, unsigned char n) {\n"
    "  int s = 0;\n"
    "  while (n--) s += *p++;\n"
    "  return s;\n"
    "}\n"
    "int rec(int n) { return n ? n + rec(n - 1) : 0; }\n"
    "int main(void) {\n"
    "  for (unsigned char i = 0; i < 8; i++) buf[i] = i * 3;\n"
    "  float f = (float)sum(buf, 8);\n"
    "  int g = (int)f;\n"
    "  return (int)(lm(100000, 3) & 0x7FFF) + g + rec(10);\n"
    "}\n";

}  // namespace

TEST_CASE("runtime_layout: a relocated HARGS is the size the program needs") {
  const std::vector<std::tuple<std::string, std::string, std::int64_t>> cases = {
      {"no helper", "int f(int a) { return a + 1; }\n", 1},
      // An `unsigned char` product is promoted to `int` and done by an
      // inline loop, so the one-byte `mul8` comes from an `unsigned
      // _BitInt(8)`.
      {"8-bit multiply",
       "unsigned _BitInt(8) f(unsigned _BitInt(8) a, "
       "unsigned _BitInt(8) b) { return a * b; }\n",
       5},
      {"32-bit divide", "long f(long a, long b) { return a / b; }\n", 22},
  };
  for (const auto& [label, src, size] : cases) {
    INFO(label);
    RuntimeLayout l = layout_of(codegen(kMonitor + src));
    // HARGS is the program's need, placed clear of the Monitor's bytes
    // and of the other three.
    l.hargs_size = size;
    const std::set<c6502::asdl::Int> others = l.bytes();
    for (c6502::asdl::Int a = 0x1F; a < 0x4A; ++a) {
      CHECK(others.count(a) == 0);
    }
    CHECK(others.size() == static_cast<std::size_t>(6 + size));
  }
}

TEST_CASE("runtime_layout: a program that reserves nothing gets the classic EQUs") {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::string asm_text = codegen("int main(void) { return 0; }\n", optimize);
    CHECK(layout_of(asm_text) == RuntimeLayout{});
    // The four runtime lines start the output.
    CHECK(asm_text.starts_with("SSP\tEQU\t$00\nFP\tEQU\t$02\nHARGS\tEQU\t$04\n"
                               "DPTR\tEQU\t$24\n"));
  }
}

TEST_CASE("runtime_layout: declaring the Monitor's cursor moves DPTR") {
  // A zero-page global goes in the bytes left over.
  const Equs e = equ(codegen(
      "extern unsigned char CH [[c6502::address(0x24)]];\n"
      "extern unsigned char CV [[c6502::address(0x25)]];\n"
      "unsigned char g [[c6502::zeropage]];\n"
      "unsigned char peek(unsigned char *p) { return *p + CH + g; }\n",
      false));
  CHECK(e.at("CH") == 0x24);
  CHECK(e.at("CV") == 0x25);
  // No helper is called, so HARGS gets one byte. HARGS, SSP, FP, and
  // DPTR take the lowest free runs, and g the next byte.
  CHECK(std::vector<std::int64_t>{e.at("HARGS"), e.at("SSP"), e.at("FP"), e.at("DPTR"),
                                  e.at("g")} == std::vector<std::int64_t>{0x00, 0x01, 0x03, 0x05,
                                                                          0x07});
}

namespace {

// The bytes of the Monitor routines a program calls, which are text,
// keyboard, line input, lo-res graphics, the IRQ entry, and INIT.
std::vector<std::int64_t> routine_bytes() {
  std::vector<std::int64_t> out;
  for (std::int64_t a = 0x1F; a < 0x2F; ++a) out.push_back(a);
  for (std::int64_t a : {0x30, 0x32, 0x33}) out.push_back(a);
  for (std::int64_t a = 0x35; a < 0x3A; ++a) out.push_back(a);
  for (std::int64_t a : {0x45, 0x48}) out.push_back(a);
  return out;
}

// Every byte the Monitor uses.
std::vector<std::int64_t> all_bytes() {
  std::vector<std::int64_t> out{0x00, 0x01};
  for (std::int64_t a = 0x1F; a < 0x4A; ++a) out.push_back(a);
  return out;
}

}  // namespace

TEST_CASE("runtime_layout: the Monitor headers keep the layout off their bytes") {
  const std::vector<std::tuple<std::string, std::string, std::vector<std::int64_t>>> headers = {
      {"monitor_io.h", "#include <apple2/monitor_io.h>\n", routine_bytes()},
      {"interactive_monitor.h", "#include <apple2/interactive_monitor.h>\n", all_bytes()},
  };
  for (const auto& [header, include, reserved] : headers) {
    const std::set<std::int64_t> taken(reserved.begin(), reserved.end());
    for (const bool optimize : {false, true}) {
      INFO(header << " optimize=" << optimize);
      const Equs e = equ(codegen(include + kSrc, optimize));
      CHECK(e.at("CH") == 0x24);
      CHECK(e.at("CSW") == 0x36);
      for (const char* name : {"SSP", "FP", "DPTR"}) {
        INFO(name);
        CHECK(taken.count(e.at(name)) == 0);
        CHECK(taken.count(e.at(name) + 1) == 0);
      }
    }
  }
}

TEST_CASE("runtime_layout: monitor_io.h leaves the interactive Monitor's bytes free") {
  // With only monitor_io.h, a zero-page global can go in a byte only
  // the interactive Monitor uses, such as $00 or $2F.
  const Equs e = equ(codegen(
      "#include <apple2/monitor_io.h>\n"
      "unsigned char g [[c6502::zeropage(0x2F)]];\n"
      "unsigned char h [[c6502::zeropage(0x00)]];\n"
      "int main(void) { return g + h; }\n"));
  CHECK(e.at("g") == 0x2F);
  CHECK(e.at("h") == 0x00);
  suite::Options o;
  o.optimize = true;
  const suite::Error error = suite::compile_error(suite::Stage::kCodegen,
                                                  "#include <apple2/interactive_monitor.h>\n"
                                                  "unsigned char g [[c6502::zeropage(0x2F)]];\n"
                                                  "int main(void) { return g; }\n",
                                                  o);
  CHECK(suite::is_a(error.type, "TypeCheckError"));
}

namespace {

// Compiles `src` from stdin with `-S -O` to `path`.
void compile_s(const std::string& src, const std::string& path) {
  const auto o = suite::compile_main({"c6502", "-", "-S", "-O", "-o", path}, src);
  REQUIRE(o.status == 0);
}

// Returns `re.sub(r" hargs_need=\d+", "", text)`.
std::string drop_hargs_need(const std::string& text) {
  const std::string key = " hargs_need=";
  std::string out;
  std::size_t at = 0;
  for (;;) {
    const std::size_t found = text.find(key, at);
    if (found == std::string::npos) return out + text.substr(at);
    std::size_t end = found + key.size();
    while (end < text.size() && text[end] >= '0' && text[end] <= '9') ++end;
    out += text.substr(at, (end == found + key.size() ? end : found) - at);
    at = end;
  }
}

}  // namespace

TEST_CASE("runtime_layout: a link binds the runtime's names once") {
  const suite::TempDir tmp;
  const std::string a = tmp.file("a.asm");
  const std::string b = tmp.file("b.asm");
  const std::string out = tmp.file("o");
  compile_s("int g(int x);\nint main(void) { return g(2); }\n", a);
  compile_s("int g(int x) { return x + 1; }\n", b);
  const auto o = suite::link({a, b}, out);
  REQUIRE_MESSAGE(o.status == 0, o.err());
  const std::string text = suite::linked_text(out);
  CHECK(suite::count(text, "DPTR\tEQU\t$24") == 1);
  CHECK(suite::count(text, "SSP\tEQU\t$00") == 1);
}

TEST_CASE("runtime_layout: units that don't give their HARGS need get the classic layout") {
  // A unit that doesn't say how much of HARGS it uses is taken to use
  // the classic block's, and its own EQUs for the runtime's names don't
  // bind them, since the linker chooses the layout.
  const suite::TempDir tmp;
  const std::string a = tmp.file("a.asm");
  const std::string b = tmp.file("b.asm");
  const std::string out = tmp.file("o");
  compile_s("int main(void) { return 0; }\n", a);
  compile_s("int g(int x) { return x + 1; }\n", b);
  for (const std::string& path : {a, b}) {
    suite::write_file(path, drop_hargs_need(suite::read_file(path)));
  }
  std::string b_text = suite::read_file(b);
  const std::size_t at = b_text.find("DPTR\tEQU\t$24");
  REQUIRE(at != std::string::npos);
  b_text.replace(at, 12, "DPTR\tEQU\t$30");
  suite::write_file(b, b_text);
  const auto o = suite::link({a, b}, out);
  REQUIRE_MESSAGE(o.status == 0, o.err());
  CHECK(suite::symbols(out).at("DPTR") == RuntimeLayout{}.dptr);
}

TEST_CASE("runtime_layout: a link chooses one layout for the whole program") {
  // One unit uses the Monitor's zero page, which moves the block. The
  // other doesn't, and needs more of HARGS (a 16-bit multiply).
  const suite::TempDir tmp;
  const std::string a = tmp.file("a.asm");
  const std::string b = tmp.file("b.asm");
  const std::string out = tmp.file("o");
  compile_s(
      "#include <apple2/monitor_io.h>\n"
      "int mul(int x, int y);\n"
      "int main(void) { CH = 3; return mul(6, 7); }\n",
      a);
  compile_s("int mul(int x, int y) { return x * y; }\n", b);
  const auto o = suite::link({a, b}, out);
  REQUIRE_MESSAGE(o.status == 0, o.err());
  const auto sym = suite::symbols(out);
  for (const char* name : {"SSP", "FP", "HARGS", "DPTR"}) {
    INFO(name);
    CHECK(sym.at(name) != 0x24);
  }
  const std::int64_t need = number_after(suite::read_file(b), "hargs_need=");
  CHECK(need > 1);
  const std::int64_t size = number_after(suite::linked_text(out), "; runtime hargs_size=");
  CHECK(size >= need);
}
