// Each case is one small function that applies one operator to its
// parameters, so that neither pipeline can fold it, and `main` calls it
// twice with different operands and stores each result in a global.
// The operators are `+`, `-`, `*`, `/`, `%`, `&`, `|`, `^`, `<<` and
// `>>` by a parameter and by a constant, unary `-` and `~`, `<`, `<=`,
// `==`, conversions to and from `long long`, `unsigned long long`,
// `int`, and `unsigned char`, and conversions to and from `double` and
// `float`. A second grid applies `++`, postfix `--`, `+=`, `*=`, `<<=`,
// and `>>=` to a bit-precise object in each storage class of
// test_op_matrix_sim.cpp, for a set of widths that covers each storage
// size and padding.
//
// A result of a bit-precise type is compared byte for byte, padding
// bits included, so the test also checks that every operation leaves
// them as the sign or zero extension of the value. The operands sit on
// the edges of each type's range as well as inside it, so `+`, `-`,
// `*`, and `<<` wrap at N bits, and signed overflow wraps too, which is
// what c6502 does with it. Division by zero and the minimum divided by
// -1 are left out, since C leaves them undefined, and shift counts are
// less than N.
//
// The programs are written here with the Python test's draws from
// CPython's `random`, and the expected bytes of each result are the
// Python model's (bit_precise.py), frozen in
// cpp/tests/frozen/sim/bitint_sim.txt, since the compiler folds
// constants with the C++ form of that model.
#include <cstdio>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "compile_case.h"
#include "doctest.h"
#include "py_random.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

using asdl::Int;

constexpr std::size_t kChunk = 40;
// Bytes of call results per program. Without -O, main holds each call's
// result in a frame temp, and a frame is at most 253 bytes.
constexpr int kResultBudget = 150;
constexpr int kZpBudget = 60;

int min_width(bool is_signed) { return is_signed ? 2 : 1; }
int storage_bytes(int width) { return (width + 7) / 8; }

std::pair<Int, Int> value_range(int width, bool is_signed) {
  if (is_signed) return {-(Int(1) << (width - 1)), (Int(1) << (width - 1)) - 1};
  return {0, (Int(1) << width) - 1};
}

std::string ctype(int width, bool is_signed) {
  return std::string(is_signed ? "" : "unsigned ") + "_BitInt(" + std::to_string(width) + ")";
}

std::string tag_of(int width, bool is_signed) { return (is_signed ? "s" : "u") + std::to_string(width); }

// `v` modulo 2^(8 * size) as an unsigned constant of `size` bytes (2 or
// 8). A cast of it to a type of that size or to a bit-precise type
// converts it to `v` (modulo 2^N, which wraps a signed type as c6502
// does), and c99_to_tac folds the cast, so no call's operand takes a
// temporary in main's frame.
std::string literal(Int v, int size = 8) {
  const Int m = v & ((Int(1) << (8 * size)) - 1);
  char buf[24];
  std::snprintf(buf, sizeof buf, "0x%llX", static_cast<unsigned long long>(m));
  return std::string(buf) + (size == 8 ? "ULL" : "U");
}

struct Case {
  std::string name;
  std::string text;
  std::vector<std::string> calls;
  std::string rtype;
  int size;
  std::string what;
};

// Operands of the type: its edges and two values inside it.
std::vector<Int> values(int width, bool is_signed, py_random::Random& rng) {
  const auto [lo, hi] = value_range(width, is_signed);
  std::vector<Int> vals{lo, hi, 0, 1, hi / 2 + 1};
  if (is_signed) vals.push_back(-1);
  vals.push_back(rng.randint(lo, hi));
  vals.push_back(rng.randint(lo, hi));
  return vals;
}

// Python's `repr` of the double `a + 0.5` (or `a - 0.5` for a negative
// `a`), or of `float(a)` for a whole `a` with `whole`. Every such value
// is below 2^53 in magnitude, so its shortest digits are its exact
// decimal, in fixed notation.
std::string float_repr(Int a, bool whole) { return asdl::int_to_string(a) + (whole ? ".0" : ".5"); }

std::vector<Case> operator_cases() {
  py_random::Random rng(23);
  std::vector<Case> out;
  const std::pair<const char*, const char*> binary[] = {
      {"add", "+"}, {"sub", "-"}, {"mul", "*"}, {"div", "/"},  {"mod", "%"}, {"and", "&"},
      {"or", "|"},  {"xor", "^"}, {"lt", "<"},  {"le", "<="}, {"eq", "=="},
  };
  for (int width = 1; width <= 64; ++width) {
    for (const bool is_signed : {false, true}) {
      if (width < min_width(is_signed)) continue;
      const std::string t = ctype(width, is_signed);
      const int size = storage_bytes(width);
      const std::vector<Int> vals = values(width, is_signed, rng);
      const std::string tag = tag_of(width, is_signed);
      const auto add = [&](const std::string& op, const std::string& text, std::vector<std::string> calls,
                           const std::string& rtype, int rsize, const std::string& what) {
        out.push_back({tag + "_" + op, text, std::move(calls), rtype, rsize, what + " on " + t});
      };
      const auto cast = [&](Int v) { return "(" + t + ")" + literal(v); };
      for (const auto& [op, sym] : binary) {
        std::vector<std::pair<Int, Int>> pairs;
        const std::string o = op;
        for (int i = 0; i < 40; ++i) {
          const Int a = rng.choice(vals);
          const Int b = rng.choice(vals);
          if (o == "div" || o == "mod") {
            if (b == 0 || (is_signed && b == -1 && a == value_range(width, is_signed).first)) continue;
          }
          pairs.emplace_back(a, b);
          if (pairs.size() == 2) break;
        }
        const bool compare = o == "lt" || o == "le" || o == "eq";
        const std::string rtype = compare ? "int" : t;
        std::vector<std::string> calls;
        for (const auto& [a, b] : pairs) calls.push_back(cast(a) + ", " + cast(b));
        add(o, rtype + " f_" + tag + "_" + o + "(" + t + " a, " + t + " b) { return a " + sym + " b; }\n", calls,
            rtype, compare ? 2 : size, o);
      }
      for (const auto& [op, sym] : {std::pair<std::string, std::string>{"shl", "<<"}, {"shr", ">>"}}) {
        const Int a0 = rng.choice(vals);
        const Int a1 = rng.choice(vals);
        const Int n1 = rng.randrange(width);
        add(op, t + " f_" + tag + "_" + op + "(" + t + " a, int n) { return a " + sym + " n; }\n",
            {cast(a0) + ", " + std::to_string(width - 1), cast(a1) + ", " + asdl::int_to_string(n1)}, t, size, op);
        if (width > 1) {
          const int k = std::max(1, width / 3);
          const Int c0 = rng.choice(vals);
          const Int c1 = rng.choice(vals);
          add(op + "c", t + " f_" + tag + "_" + op + "c(" + t + " a) { return a " + sym + " " + std::to_string(k) +
                            "; }\n",
              {cast(c0), cast(c1)}, t, size, op + " by " + std::to_string(k));
        }
      }
      for (const auto& [op, sym] : {std::pair<std::string, std::string>{"neg", "-"}, {"not", "~"}}) {
        const Int c0 = rng.choice(vals);
        const Int c1 = rng.choice(vals);
        add(op, t + " f_" + tag + "_" + op + "(" + t + " a) { return " + sym + "a; }\n", {cast(c0), cast(c1)}, t,
            size, op);
      }
      struct Other {
        const char* c;
        int size;
        bool is_signed;
      };
      for (const Other& other : {Other{"long long", 8, true}, Other{"unsigned long long", 8, false},
                                 Other{"int", 2, true}, Other{"unsigned char", 1, false}}) {
        std::string otag = other.c;
        for (char& ch : otag) {
          if (ch == ' ') ch = '_';
        }
        const Int c0 = rng.choice(vals);
        const Int c1 = rng.choice(vals);
        add("to_" + otag, std::string(other.c) + " f_" + tag + "_to_" + otag + "(" + t + " a) { return a; }\n",
            {cast(c0), cast(c1)}, other.c, other.size, std::string("conversion to ") + other.c);
        const auto [olo, ohi] = value_range(8 * other.size, other.is_signed);
        const Int x0 = rng.randint(olo, ohi);
        const std::vector<Int> ends{olo, ohi};
        const Int x1 = rng.choice(ends);
        const int lsize = other.size == 2 ? 2 : 8;
        add("from_" + otag, t + " f_" + tag + "_from_" + otag + "(" + other.c + " x) { return x; }\n",
            {"(" + std::string(other.c) + ")" + literal(x0, lsize), "(" + std::string(other.c) + ")" + literal(x1, lsize)},
            t, size, std::string("conversion from ") + other.c);
      }
      const auto [lo, hi] = value_range(width, is_signed);
      struct Fp {
        const char* c;
        int size;
        Int limit;
      };
      for (const Fp& fp : {Fp{"double", 8, Int(1) << 53}, Fp{"float", 4, Int(1) << 24}}) {
        const Int flo = std::max(lo, -fp.limit + 1);
        const Int fhi = std::min(hi, fp.limit - 1);
        const std::string f = fp.c;
        const Int a0 = rng.randint(flo, fhi);
        add("to_" + f, f + " f_" + tag + "_to_" + f + "(" + t + " a) { return a; }\n", {cast(a0), cast(fhi)}, f,
            fp.size, "conversion to " + f);
        // A value with a fraction, which the conversion drops, from
        // inside the range where the floating type is exact.
        const Int b0 = rng.randint(flo, fhi);
        std::vector<std::string> calls;
        for (const Int a : {b0, flo}) {
          const Int mag = a < 0 ? -a : a;
          calls.push_back(float_repr(a, !(mag < fp.limit / 2)) + (f == "float" ? "f" : ""));
        }
        add("from_" + f, t + " f_" + tag + "_from_" + f + "(" + f + " d) { return d; }\n", calls, t, size,
            "conversion from " + f);
      }
    }
  }
  return out;
}

// The storage grid's widths: each storage size from 1 to 8 bytes, with
// and without padding.
const int kGridWidths[] = {1, 5, 8, 12, 16, 17, 24, 31, 40, 48, 55, 64};
const char* const kStorage[] = {"local", "static", "global", "zp", "ptr", "array", "member"};
const std::pair<const char*, const char*> kGridOps[] = {
    {"preinc", "++X"}, {"postdec", "X--"}, {"add", "X += 3"}, {"mul", "X *= 3"}, {"shl", "X <<= 2"}, {"shr", "X >>= 1"},
};

Int wrap(Int v, int width, bool is_signed) {
  v &= (Int(1) << width) - 1;
  if (is_signed && (v >> (width - 1)) != 0) v -= Int(1) << width;
  return v;
}

// The expression's value. A compound assignment with an `int` operand
// computes in the common type, which is `int` for a bit-precise type of
// at most 16 bits (C23 §6.3.1.8), and the result converts back, so
// either way the object wraps at N bits. Only the source's text needs
// it, since the expected bytes are frozen.
Int grid_value(const std::string& op, Int x, int width, bool is_signed) {
  if (op == "preinc") return wrap(x + 1, width, is_signed);
  if (op == "postdec") return x;
  if (op == "add") return wrap(x + 3, width, is_signed);
  if (op == "mul") return wrap(x * 3, width, is_signed);
  if (op == "shl") return wrap(x << 2, width, is_signed);
  return wrap(x >> 1, width, is_signed);
}

std::string replace_x(const std::string& text, const std::string& obj) {
  std::string out;
  for (const char ch : text) out += ch == 'X' ? obj : std::string(1, ch);
  return out;
}

std::vector<Case> grid_cases(std::map<std::string, std::vector<std::string>>& decls) {
  py_random::Random rng(64);
  std::vector<Case> out;
  for (const int width : kGridWidths) {
    for (const bool is_signed : {false, true}) {
      if (width < min_width(is_signed)) continue;
      const std::string t = ctype(width, is_signed);
      const int size = storage_bytes(width);
      const auto [lo, hi] = value_range(width, is_signed);
      const std::string tag = tag_of(width, is_signed);
      for (const char* storage : kStorage) {
        const std::string st = storage;
        for (const auto& [op_c, text] : kGridOps) {
          const std::string op = op_c;
          // A shift count must be less than the width.
          if ((op == "shl" && width <= 2) || (op == "shr" && width == 1)) continue;
          const std::vector<Int> choices{lo, hi, rng.randint(lo, hi)};
          const Int start = rng.choice(choices);
          const Int value = grid_value(op, start, width, is_signed);
          const std::string name = "g_" + tag + "_" + st + "_" + op;
          const std::string init = "(" + t + ")" + literal(start);
          std::vector<std::string> d;
          std::string pre, obj;
          if (st == "local") {
            pre = "    " + t + " x = " + init + ";\n";
            obj = "x";
          } else if (st == "static") {
            pre = "    static " + t + " x = " + init + ";\n";
            obj = "x";
          } else if (st == "global") {
            d.push_back(t + " v_" + name + " = " + init + ";");
            obj = "v_" + name;
          } else if (st == "zp") {
            d.push_back(t + " v_" + name + " [[c6502::zeropage]] = " + init + ";");
            obj = "v_" + name;
          } else if (st == "ptr") {
            d.push_back(t + " v_" + name + " = " + init + ";");
            pre = "    " + t + " *p = &v_" + name + ";\n";
            obj = "(*p)";
          } else if (st == "array") {
            d.push_back(t + " v_" + name + "[3] = {0, " + init + ", 0};");
            obj = "v_" + name + "[one]";
          } else {
            d.push_back("struct S_" + tag + " v_" + name + " = {0, " + init + "};");
            obj = "v_" + name + ".m";
          }
          const std::string expr = "(" + replace_x(text, obj) + ")";
          const std::string body = "    " + t + " r = " + expr + ";\n    return r == (" + t + ")" + literal(value) +
                                   " ? " + obj + " : (" + t + ")0;\n";
          out.push_back({name, t + " f_" + name + "(void) {\n" + pre + body + "}\n", {""}, t, size,
                         op + " on " + st + " " + t + " (start " + asdl::int_to_string(start) + ")"});
          decls[name] = d;
        }
      }
    }
  }
  return out;
}

std::string join_lines(const std::vector<std::string>& lines) {
  std::string out;
  for (std::size_t i = 0; i < lines.size(); ++i) out += (i ? "\n" : "") + lines[i];
  return out;
}

struct Program {
  std::string source;
  std::vector<Case> cases;
};

std::vector<Program> programs(const std::vector<Case>& cases,
                              const std::map<std::string, std::vector<std::string>>& decls) {
  std::vector<Program> out;
  const auto emit = [&](const std::vector<Case>& chunk) {
    std::vector<std::string> head{"unsigned char one = 1;"};
    for (const int width : kGridWidths) {
      for (const bool is_signed : {false, true}) {
        if (width >= min_width(is_signed)) {
          head.push_back("struct S_" + tag_of(width, is_signed) + " { unsigned char pad; " + ctype(width, is_signed) +
                         " m; };");
        }
      }
    }
    std::vector<std::string> results, calls;
    std::string body;
    for (const Case& c : chunk) {
      const auto it = decls.find(c.name);
      if (it != decls.end()) head.insert(head.end(), it->second.begin(), it->second.end());
      body += c.text;
      for (std::size_t j = 0; j < c.calls.size(); ++j) {
        const std::string r = "r_" + c.name + "_" + std::to_string(j);
        results.push_back(c.rtype + " " + r + ";");
        calls.push_back("    " + r + " = f_" + c.name + "(" + c.calls[j] + ");");
      }
    }
    std::vector<std::string> all = head;
    all.insert(all.end(), results.begin(), results.end());
    std::string src = join_lines(all) + "\n" + body;
    src += "int main(void) {\n" + join_lines(calls) + "\n    return 0;\n}\n";
    out.push_back({src, chunk});
  };
  std::vector<Case> chunk;
  int budget = 0, zp = 0;
  for (const Case& c : cases) {
    const int need = c.size * static_cast<int>(c.calls.size());
    const int extra = c.name.find("_zp_") != std::string::npos ? c.size : 0;
    if (!chunk.empty() && (chunk.size() >= kChunk || budget + need > kResultBudget || zp + extra > kZpBudget)) {
      emit(chunk);
      chunk.clear();
      budget = zp = 0;
    }
    chunk.push_back(c);
    budget += need;
    zp += extra;
  }
  if (!chunk.empty()) emit(chunk);
  return out;
}

struct Frozen {
  // The number of programs and the CRC-32 of their sources, by grid.
  std::map<std::string, std::pair<std::size_t, std::uint32_t>> grids;
  // Each result's bytes, high byte first, in hex.
  std::map<std::string, std::string> want;
};

const Frozen& frozen() {
  static const Frozen f = [] {
    Frozen out;
    std::istringstream in(read_repo_file("cpp/tests/frozen/sim/bitint_sim.txt"));
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty() || line[0] == '#') continue;
      std::istringstream words(line);
      std::string first;
      words >> first;
      if (first == "programs") {
        std::string kind, crc;
        std::size_t n = 0;
        words >> kind >> n >> crc;
        out.grids[kind] = {n, static_cast<std::uint32_t>(std::stoul(crc, nullptr, 16))};
      } else {
        words >> out.want[first];
      }
    }
    return out;
  }();
  return f;
}

const std::vector<Program>& operator_programs() {
  static const std::vector<Program> ps = programs(operator_cases(), {});
  return ps;
}

const std::vector<Program>& grid_programs() {
  static const std::vector<Program> ps = [] {
    std::map<std::string, std::vector<std::string>> decls;
    const std::vector<Case> cases = grid_cases(decls);
    return programs(cases, decls);
  }();
  return ps;
}

std::string hex_high_first(const std::vector<std::uint8_t>& memory, std::int64_t addr, int size) {
  std::string out;
  char buf[4];
  for (int i = size - 1; i >= 0; --i) {
    std::snprintf(buf, sizeof buf, "%02x", memory[static_cast<std::size_t>(addr + i)]);
    out += buf;
  }
  return out;
}

void check(const std::vector<Program>& ps, bool optimize, int shard, int shards) {
  std::vector<std::string> failures;
  for (std::size_t k = 0; k < ps.size(); ++k) {
    if (static_cast<int>(k % static_cast<std::size_t>(shards)) != shard) continue;
    const Program& p = ps[k];
    sim::Simulation s;
    try {
      s = build_sim(p.source, optimize);
      s.run();
    } catch (const std::exception& e) {
      failures.push_back(p.cases[0].name + "..: " + e.what());
      continue;
    }
    for (const Case& c : p.cases) {
      for (std::size_t j = 0; j < c.calls.size(); ++j) {
        const std::string r = "r_" + c.name + "_" + std::to_string(j);
        const std::string want = frozen().want.at(r);
        const std::string got = hex_high_first(s.cpu.memory, s.symbol(r), c.size);
        if (got != want) {
          failures.push_back(c.name + ": " + c.what + ", call '" + c.calls[j] + "': got " + got + ", want " + want);
        }
      }
    }
  }
  std::string shown;
  for (std::size_t i = 0; i < failures.size() && i < 40; ++i) shown += failures[i] + "\n";
  CHECK_MESSAGE(failures.empty(), shown);
}

std::uint32_t crc_of(const std::vector<Program>& ps) {
  std::string all;
  for (const Program& p : ps) all += p.source;
  return sim::cpu_run::crc32(all);
}

}  // namespace

TEST_CASE("bitint sim: the programs are those test_bitint_sim.py wrote") {
  const auto& grids = frozen().grids;
  CHECK(operator_programs().size() == grids.at("operators").first);
  CHECK(crc_of(operator_programs()) == grids.at("operators").second);
  CHECK(grid_programs().size() == grids.at("storage").first);
  CHECK(crc_of(grid_programs()) == grids.at("storage").second);
  std::size_t results = 0;
  for (const auto* ps : {&operator_programs(), &grid_programs()}) {
    for (const Program& p : *ps) {
      for (const Case& c : p.cases) results += c.calls.size();
    }
  }
  CHECK(results == frozen().want.size());
}

TEST_CASE("shard: bitint operators unoptimized 0") { check(operator_programs(), false, 0, 16); }
TEST_CASE("shard: bitint operators optimized 0") { check(operator_programs(), true, 0, 16); }
TEST_CASE("shard: bitint operators unoptimized 1") { check(operator_programs(), false, 1, 16); }
TEST_CASE("shard: bitint operators optimized 1") { check(operator_programs(), true, 1, 16); }
TEST_CASE("shard: bitint operators unoptimized 2") { check(operator_programs(), false, 2, 16); }
TEST_CASE("shard: bitint operators optimized 2") { check(operator_programs(), true, 2, 16); }
TEST_CASE("shard: bitint operators unoptimized 3") { check(operator_programs(), false, 3, 16); }
TEST_CASE("shard: bitint operators optimized 3") { check(operator_programs(), true, 3, 16); }
TEST_CASE("shard: bitint operators unoptimized 4") { check(operator_programs(), false, 4, 16); }
TEST_CASE("shard: bitint operators optimized 4") { check(operator_programs(), true, 4, 16); }
TEST_CASE("shard: bitint operators unoptimized 5") { check(operator_programs(), false, 5, 16); }
TEST_CASE("shard: bitint operators optimized 5") { check(operator_programs(), true, 5, 16); }
TEST_CASE("shard: bitint operators unoptimized 6") { check(operator_programs(), false, 6, 16); }
TEST_CASE("shard: bitint operators optimized 6") { check(operator_programs(), true, 6, 16); }
TEST_CASE("shard: bitint operators unoptimized 7") { check(operator_programs(), false, 7, 16); }
TEST_CASE("shard: bitint operators optimized 7") { check(operator_programs(), true, 7, 16); }
TEST_CASE("shard: bitint operators unoptimized 8") { check(operator_programs(), false, 8, 16); }
TEST_CASE("shard: bitint operators optimized 8") { check(operator_programs(), true, 8, 16); }
TEST_CASE("shard: bitint operators unoptimized 9") { check(operator_programs(), false, 9, 16); }
TEST_CASE("shard: bitint operators optimized 9") { check(operator_programs(), true, 9, 16); }
TEST_CASE("shard: bitint operators unoptimized 10") { check(operator_programs(), false, 10, 16); }
TEST_CASE("shard: bitint operators optimized 10") { check(operator_programs(), true, 10, 16); }
TEST_CASE("shard: bitint operators unoptimized 11") { check(operator_programs(), false, 11, 16); }
TEST_CASE("shard: bitint operators optimized 11") { check(operator_programs(), true, 11, 16); }
TEST_CASE("shard: bitint operators unoptimized 12") { check(operator_programs(), false, 12, 16); }
TEST_CASE("shard: bitint operators optimized 12") { check(operator_programs(), true, 12, 16); }
TEST_CASE("shard: bitint operators unoptimized 13") { check(operator_programs(), false, 13, 16); }
TEST_CASE("shard: bitint operators optimized 13") { check(operator_programs(), true, 13, 16); }
TEST_CASE("shard: bitint operators unoptimized 14") { check(operator_programs(), false, 14, 16); }
TEST_CASE("shard: bitint operators optimized 14") { check(operator_programs(), true, 14, 16); }
TEST_CASE("shard: bitint operators unoptimized 15") { check(operator_programs(), false, 15, 16); }
TEST_CASE("shard: bitint operators optimized 15") { check(operator_programs(), true, 15, 16); }

TEST_CASE("shard: bitint storage unoptimized 0") { check(grid_programs(), false, 0, 4); }
TEST_CASE("shard: bitint storage optimized 0") { check(grid_programs(), true, 0, 4); }
TEST_CASE("shard: bitint storage unoptimized 1") { check(grid_programs(), false, 1, 4); }
TEST_CASE("shard: bitint storage optimized 1") { check(grid_programs(), true, 1, 4); }
TEST_CASE("shard: bitint storage unoptimized 2") { check(grid_programs(), false, 2, 4); }
TEST_CASE("shard: bitint storage optimized 2") { check(grid_programs(), true, 2, 4); }
TEST_CASE("shard: bitint storage unoptimized 3") { check(grid_programs(), false, 3, 4); }
TEST_CASE("shard: bitint storage optimized 3") { check(grid_programs(), true, 3, 4); }

}  // namespace c6502::suite
