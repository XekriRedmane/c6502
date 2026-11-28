// The tests of tests/test_runtime_library.py that run the linked
// program. Each helper is reached from C the way the compiler reaches
// it (`*`, `/`, `%`, `<<`, and `>>` on the types that call it), linked
// with no `--defsym`, and run on the simulator with its own helpers
// removed, so that only the copies the link put in the program can run.
// The results are checked against C++ arithmetic on edge cases and on
// values drawn from CPython's `random` with the Python test's seeds.
// The other tests run programs that get only the helpers they call, or
// the simulator's copies that `--defsym` gives, or the helpers a linker
// script places, or a HARGS block the program moved.
#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "binutils/elf.h"
#include "compile_case.h"
#include "doctest.h"
#include "passes/zp_globals.h"
#include "py_random.h"
#include "rtlib/helpers.h"
#include "sim_case.h"

namespace {

namespace suite = c6502::suite;
namespace elf = c6502::binutils::elf;
namespace sim = c6502::sim;
using c6502::asdl::Int;

struct Type {
  const char* c;
  int size;
  bool is_signed;

  int bits() const { return 8 * size; }

  Int wrap(Int v) const {
    v &= (Int(1) << bits()) - 1;
    if (is_signed && (v >> (bits() - 1)) != 0) v -= Int(1) << bits();
    return v;
  }

  std::string literal(Int v) const {
    if (v == -(Int(1) << (bits() - 1))) return "(" + c6502::asdl::int_to_string(v + 1) + " - 1)";
    const std::string suffix = !is_signed && size == 4 ? "UL" : "";
    const std::string s = c6502::asdl::int_to_string(v) + suffix;
    return v < 0 ? "(" + s + ")" : s;
  }

  // Edge cases (0, 1, the largest and smallest values, and the values
  // around the sign bit and each byte boundary), then random values, `n`
  // in all.
  std::vector<Int> values(c6502::py_random::Random& rng, std::size_t n) const {
    const Int top = (Int(1) << bits()) - 1;
    std::vector<Int> raw = {0, 1, 2, 3, top, top - 1, Int(1) << (bits() - 1), (Int(1) << (bits() - 1)) - 1,
                            (Int(1) << (bits() - 1)) + 1, 0x55, 0xAA, 0x7F, 0x80, 0xFF};
    for (int k = 1; k < size; ++k) {
      raw.push_back(Int(1) << (8 * k));
      raw.push_back((Int(1) << (8 * k)) - 1);
    }
    std::vector<Int> out;
    for (const Int v : raw) {
      const Int w = wrap(v);
      if (std::find(out.begin(), out.end(), w) == out.end()) out.push_back(w);
    }
    while (out.size() < n) out.push_back(wrap(rng.getrandbits(bits())));
    out.resize(n);
    return out;
  }
};

const Type U8{"uint8_t", 1, false};
const Type COUNT = U8;
const Type B8{"_BitInt(8)", 1, true};
const Type UB8{"unsigned _BitInt(8)", 1, false};
const Type U16{"uint16_t", 2, false};
const Type S16{"int16_t", 2, true};
const Type U32{"uint32_t", 4, false};
const Type S32{"int32_t", 4, true};

Int abs_int(Int v) { return v < 0 ? -v : v; }

Int trunc_div(Int a, Int b) {
  const Int q = abs_int(a) / abs_int(b);
  return (a < 0) != (b < 0) ? -q : q;
}

std::string join(const std::vector<std::string>& items) {
  std::string out;
  for (std::size_t i = 0; i < items.size(); ++i) out += (i ? ", " : "") + items[i];
  return out;
}

// A program that applies each expression of `outs` (in `x` and `y`) to
// every pair of `xs` (of type `t`) and `ys` (of type `u`), storing the
// results in `out0`, `out1`, and so on, of type `t`.
std::string source(const Type& t, const Type& u, const std::vector<Int>& xs, const std::vector<Int>& ys,
                   const std::vector<std::string>& outs) {
  const std::string n = std::to_string(xs.size());
  std::vector<std::string> xl, yl;
  for (const Int v : xs) xl.push_back(t.literal(v));
  for (const Int v : ys) yl.push_back(u.literal(v));
  std::string text = "#include <stdint.h>\n";
  text += std::string(t.c) + " xs[" + n + "] = {" + join(xl) + "};\n";
  text += std::string(u.c) + " ys[" + n + "] = {" + join(yl) + "};\n";
  for (std::size_t k = 0; k < outs.size(); ++k) text += std::string(t.c) + " out" + std::to_string(k) + "[" + n + "];\n";
  std::string body;
  for (std::size_t k = 0; k < outs.size(); ++k) {
    body += "out" + std::to_string(k) + "[i] = (" + t.c + ")(" + outs[k] + ");";
  }
  text += "int main(void) {\n";
  text += "    for (uint8_t i = 0; i < " + n + "; i++) {\n";
  text += std::string("        ") + t.c + " x = xs[i]; " + u.c + " y = ys[i];\n";
  text += "        " + body + "\n";
  text += "    }\n    return 0;\n}\n";
  return text;
}

std::set<std::string> helper_names() {
  std::set<std::string> out;
  for (const auto& fn : c6502::rtlib::all_helper_functions()) out.insert(fn.name);
  return out;
}

struct Exe {
  std::string bytes;
  elf::ElfFile elf;
};

Exe build(const std::string& src, const std::vector<std::string>& extra = {},
          const std::optional<std::string>& script = std::nullopt) {
  const suite::TempDir tmp;
  const std::string obj = tmp.file("a.o");
  REQUIRE(suite::compile_object(src, obj) == 0);
  std::vector<std::string> argv{"c6502", obj, "-o", tmp.file("prog")};
  if (script) {
    suite::write_file(tmp.file("prog.ld"), *script);
    argv.insert(argv.begin() + 1, {"-T", tmp.file("prog.ld")});
  }
  argv.insert(argv.end(), extra.begin(), extra.end());
  const auto o = suite::compile_main(argv);
  REQUIRE_MESSAGE(o.status == 0, o.err());
  Exe exe;
  exe.bytes = suite::read_file(tmp.file("prog"));
  exe.elf = elf::read(exe.bytes, "prog");
  return exe;
}

std::map<std::string, std::int64_t> own_helpers(const elf::ElfFile& exe) {
  const std::set<std::string> names = helper_names();
  std::map<std::string, std::int64_t> out;
  for (const auto& s : exe.symbols) {
    if (names.count(s->name) && s->defined() && !s->is_absolute()) out[s->name] = s->value;
  }
  return out;
}

std::set<std::string> keys(const std::map<std::string, std::int64_t>& m) {
  std::set<std::string> out;
  for (const auto& [k, v] : m) out.insert(k);
  return out;
}

struct Ran {
  std::vector<std::uint8_t> memory;
  std::int64_t result = 0;
  std::map<std::string, std::int64_t> symbols;
};

Ran run_without_sim_helpers(const Exe& exe) {
  sim::Simulation s = sim::build_linked_sim(exe.bytes);
  std::fill_n(s.cpu.memory.begin() + s.runtime.helper_origin, s.runtime.helper_image.size(), std::uint8_t{0});
  s.runtime.hooks.clear();
  const sim::SimResult r = s.run();
  REQUIRE_FALSE(r.timed_out);
  return {r.memory, r.return_int_signed(), s.symbols};
}

std::vector<Int> read(const Ran& ran, const std::string& name, const Type& t, std::size_t n) {
  const std::int64_t base = ran.symbols.at(name);
  std::vector<Int> out;
  for (std::size_t k = 0; k < n; ++k) {
    Int v = 0;
    for (int b = 0; b < t.size; ++b) {
      v |= Int(ran.memory[static_cast<std::size_t>(base + static_cast<std::int64_t>(k) * t.size + b)]) << (8 * b);
    }
    out.push_back(t.wrap(v));
  }
  return out;
}

using Fn = std::function<Int(Int, Int)>;

constexpr std::size_t kN = 24;

std::string names_repr(const std::set<std::string>& helpers) {
  std::string out = "[";
  for (const std::string& h : helpers) out += (out.size() > 1 ? ", '" : "'") + h + "'";
  return out + "]";
}

void check(const std::set<std::string>& helpers, const Type& t, const Type& u,
           const std::vector<std::pair<std::string, Fn>>& outs, bool nonzero = false, int counts = 0,
           const std::set<std::string>& absent = {}) {
  c6502::py_random::Random rng(names_repr(helpers) + t.c);
  const std::vector<Int> xs = t.values(rng, kN);
  std::vector<Int> ys;
  if (counts != 0) {
    for (std::size_t k = 0; k < kN; ++k) ys.push_back(static_cast<Int>(k % static_cast<std::size_t>(counts)));
  } else {
    std::vector<Int> v = u.values(rng, kN + 4);
    std::reverse(v.begin(), v.end());
    for (const Int y : v) {
      if (!nonzero || y != 0) ys.push_back(y);
    }
    ys.resize(kN);
    if (t.is_signed && nonzero) {
      // The quotient of the smallest value and -1 doesn't fit the type,
      // which C leaves undefined.
      const Int low = -(Int(1) << (t.bits() - 1));
      for (std::size_t k = 0; k < kN; ++k) {
        if (xs[k] == low && ys[k] == -1) ys[k] = 2;
      }
    }
  }
  std::vector<std::string> exprs;
  for (const auto& [e, fn] : outs) exprs.push_back(e);
  const Exe exe = build(source(t, u, xs, ys, exprs));
  const std::set<std::string> own = keys(own_helpers(exe.elf));
  for (const std::string& h : helpers) CHECK_MESSAGE(own.count(h) == 1, h << " isn't linked");
  for (const std::string& h : absent) CHECK_MESSAGE(own.count(h) == 0, h << " is linked");
  const std::set<std::string> names = helper_names();
  for (const auto& s : exe.elf.symbols) CHECK_FALSE((names.count(s->name) != 0 && s->is_absolute()));
  const Ran ran = run_without_sim_helpers(exe);
  for (std::size_t k = 0; k < outs.size(); ++k) {
    const std::vector<Int> got = read(ran, "out" + std::to_string(k), t, kN);
    for (std::size_t i = 0; i < kN; ++i) {
      const Int want = t.wrap(outs[k].second(xs[i], ys[i]));
      if (got[i] != want) {
        FAIL_CHECK(outs[k].first << " on " << t.c << ": first difference at x=" << c6502::asdl::int_to_string(xs[i])
                                 << " y=" << c6502::asdl::int_to_string(ys[i]));
        break;
      }
    }
  }
}

const Fn kMul = [](Int x, Int y) { return x * y; };
const Fn kTruncMod = [](Int x, Int y) { return x - trunc_div(x, y) * y; };
const Fn kShl = [](Int x, Int y) { return x << static_cast<int>(y); };
const Fn kShr = [](Int x, Int y) { return x >> static_cast<int>(y); };

const std::string kDivide = R"(
int a = -1234;
int b = 7;
int main(void) { return a / b; }
)";

const std::string kScript = R"(
MEMORY
{
    ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x1000
    lib (rx) : ORIGIN = 0x4000, LENGTH = 0x0400
}
SECTIONS
{
    .text : { a.o(.text .text.*) } > ram
    .rodata : { *(.rodata .rodata.*) } > ram
    .data : { *(.data .data.*) } > ram
    .bss : { *(.bss .bss.*) } > ram
    .runtime : { runtime.o(.text) } > lib
}
)";

}  // namespace

// Each helper, linked from the library and run, against C++ arithmetic.
// A 1- or 2-byte shift by a variable count is an inline loop, so those
// cases check the loop's results and that the shift helpers of those
// widths aren't linked.
TEST_CASE("runtime_library sim: mul8") { check({"mul8"}, U8, U8, {{"x * y", kMul}}); }
TEST_CASE("runtime_library sim: udivmod8") {
  check({"udivmod8"}, UB8, UB8, {{"x / y", std::divides<Int>{}}, {"x % y", std::modulus<Int>{}}}, true);
}
TEST_CASE("runtime_library sim: sdivmod8") {
  check({"sdivmod8", "udivmod8"}, B8, B8, {{"x / y", trunc_div}, {"x % y", kTruncMod}}, true);
}
TEST_CASE("runtime_library sim: asl8") { check({}, U8, COUNT, {{"x << y", kShl}}, false, 8, {"asl8"}); }
TEST_CASE("runtime_library sim: lsr8") { check({}, UB8, COUNT, {{"x >> y", kShr}}, false, 8, {"lsr8"}); }
TEST_CASE("runtime_library sim: asr8") { check({}, B8, COUNT, {{"x >> y", kShr}}, false, 8, {"asr8"}); }
TEST_CASE("runtime_library sim: mul16") { check({"mul16"}, U16, U16, {{"x * y", kMul}}); }
TEST_CASE("runtime_library sim: udivmod16") {
  check({"udivmod16"}, U16, U16, {{"x / y", std::divides<Int>{}}, {"x % y", std::modulus<Int>{}}}, true);
}
TEST_CASE("runtime_library sim: sdivmod16") {
  check({"sdivmod16", "udivmod16"}, S16, S16, {{"x / y", trunc_div}, {"x % y", kTruncMod}}, true);
}
TEST_CASE("runtime_library sim: asl16") { check({}, U16, COUNT, {{"x << y", kShl}}, false, 16, {"asl16"}); }
TEST_CASE("runtime_library sim: lsr16") { check({}, U16, COUNT, {{"x >> y", kShr}}, false, 16, {"lsr16"}); }
TEST_CASE("runtime_library sim: asr16") { check({}, S16, COUNT, {{"x >> y", kShr}}, false, 16, {"asr16"}); }
TEST_CASE("runtime_library sim: mul32") { check({"mul32"}, U32, U32, {{"x * y", kMul}}); }
TEST_CASE("runtime_library sim: udivmod32") {
  check({"udivmod32"}, U32, U32, {{"x / y", std::divides<Int>{}}, {"x % y", std::modulus<Int>{}}}, true);
}
TEST_CASE("runtime_library sim: sdivmod32") {
  check({"sdivmod32", "udivmod32"}, S32, S32, {{"x / y", trunc_div}, {"x % y", kTruncMod}}, true);
}
TEST_CASE("runtime_library sim: asl32") { check({"asl32"}, U32, COUNT, {{"x << y", kShl}}, false, 32); }
TEST_CASE("runtime_library sim: lsr32") { check({"lsr32"}, U32, COUNT, {{"x >> y", kShr}}, false, 32); }
TEST_CASE("runtime_library sim: asr32") { check({"asr32"}, S32, COUNT, {{"x >> y", kShr}}, false, 32); }

TEST_CASE("runtime_library sim: bin16_to_bcd") {
  // `__c6502_bin_to_bcd` of a variable calls the helper, and a constant
  // argument folds, so a program with only constants doesn't hold it.
  c6502::py_random::Random rng("bin16_to_bcd");
  const std::vector<Int> xs = U16.values(rng, kN);
  std::vector<std::string> xl;
  for (const Int v : xs) xl.push_back(c6502::asdl::int_to_string(v));
  const std::string src = "#include <stdint.h>\nuint16_t xs[24] = {" + join(xl) +
                          "};\nuint32_t out0[24];\nint main(void) {\n"
                          "    for (uint8_t i = 0; i < 24; i++) out0[i] = __c6502_bin_to_bcd(xs[i]);\n"
                          "    return 0;\n}\n";
  const Exe exe = build(src);
  CHECK(keys(own_helpers(exe.elf)) == std::set<std::string>{"bin16_to_bcd"});
  const std::vector<Int> got = read(run_without_sim_helpers(exe), "out0", U32, kN);
  for (std::size_t i = 0; i < kN; ++i) {
    // The five decimal digits read as hexadecimal.
    char digits[8];
    std::snprintf(digits, sizeof digits, "%05u", static_cast<unsigned>(xs[i]));
    CHECK(got[i] == Int(std::stoul(digits, nullptr, 16)));
  }
  const Exe folded = build("#include <stdint.h>\nuint32_t v;\n"
                           "int main(void) { v = __c6502_bin_to_bcd(12345); return 0; }\n");
  CHECK(own_helpers(folded.elf).empty());
}

TEST_CASE("runtime_library sim: every helper is tested") {
  const std::set<std::string> tested = {"mul8",      "udivmod8",  "sdivmod8", "asl8",  "lsr8",  "asr8",  "mul16",
                                        "udivmod16", "sdivmod16", "asl16",    "lsr16", "asr16", "mul32", "udivmod32",
                                        "sdivmod32", "asl32",     "lsr32",    "asr32", "bin16_to_bcd"};
  for (const std::string& name : helper_names()) CHECK_MESSAGE(tested.count(name) == 1, name);
}

TEST_CASE("runtime_library sim: a link takes only the called helpers and their callees") {
  const Exe exe = build(kDivide);
  CHECK(keys(own_helpers(exe.elf)) == std::set<std::string>{"sdivmod16", "udivmod16"});
  CHECK(run_without_sim_helpers(exe).result == -176);
}

TEST_CASE("runtime_library sim: --defsym takes precedence over the library") {
  const Exe exe = build(kDivide, sim::defsym_args());
  CHECK(own_helpers(exe.elf).empty());
  CHECK(sim::build_linked_sim(exe.bytes).run().return_int_signed() == -176);
}

TEST_CASE("runtime_library sim: --defsym of a callee") {
  // sdivmod16 comes from the library, and its call of udivmod16 goes to
  // the address --defsym gives, the simulator's copy.
  const std::int64_t addr = sim::runtime_defsyms().at("udivmod16");
  char arg[40];
  std::snprintf(arg, sizeof arg, "--defsym=udivmod16=0x%04llX", static_cast<long long>(addr));
  const Exe exe = build(kDivide, {arg});
  CHECK(keys(own_helpers(exe.elf)) == std::set<std::string>{"sdivmod16"});
  CHECK(exe.elf.symbol("udivmod16")->value == addr);
  CHECK(sim::build_linked_sim(exe.bytes).run().return_int_signed() == -176);
}

TEST_CASE("runtime_library sim: the helpers use a moved HARGS") {
  // A fixed object in the classic block's bytes moves HARGS, and the
  // helpers read and write the program's HARGS.
  const Exe exe = build("#include <stdint.h>\n"
                        "extern uint8_t wndtop [[c6502::address(0x22)]];\n"
                        "int a = -1234; int b = 7;\n"
                        "int main(void) { wndtop = 3; return a / b; }\n");
  CHECK(exe.elf.symbol("HARGS")->value != 0x04);
  CHECK(run_without_sim_helpers(exe).result == -176);
  const std::string meta = exe.elf.link_text().value();
  const std::string after = meta.substr(meta.find("hargs_size=") + 11);
  const long long size = std::stoll(after.substr(0, after.find_first_of(" \n")));
  CHECK(size >= static_cast<long long>(c6502::passes::helper_footprints().at("sdivmod16")));
}

TEST_CASE("runtime_library sim: a linker script places the helpers by the library's name") {
  const Exe exe = build(kDivide, {}, kScript);
  const auto helpers = own_helpers(exe.elf);
  CHECK(keys(helpers) == std::set<std::string>{"sdivmod16", "udivmod16"});
  for (const auto& [name, a] : helpers) CHECK((0x4000 <= a && a < 0x4400));
  CHECK(exe.elf.symbol("main")->value < 0x4000);
  CHECK(run_without_sim_helpers(exe).result == -176);
}

TEST_CASE("runtime_library sim: a linker script places the helpers with a pattern") {
  std::string script = kScript;
  script.replace(script.find("a.o(.text .text.*)"), 18, "*(.text .text.*)");
  const Exe exe = build(kDivide, {}, script);
  for (const auto& [name, a] : own_helpers(exe.elf)) CHECK((0x0800 <= a && a < 0x1800));
  CHECK(run_without_sim_helpers(exe).result == -176);
}
