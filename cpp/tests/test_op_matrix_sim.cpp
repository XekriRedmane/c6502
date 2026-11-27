// The assignment operators over every integer type, storage class, and
// way of using the result, from test_op_matrix_sim.py.
//
// Each case is one small function that applies one operator to one
// object and uses the result one way:
//
//   * types: the eight integer types, `unsigned char` to `long long`;
//   * storage: an automatic local, a block-scope `static`, a global, a
//     zero-page global, `*p` for a pointer to a global, `a[i]` with a
//     runtime index, and a struct member;
//   * operators: `++` and `--` before and after, `+=`, `-=`, `*=`,
//     `&=`, `|=`, `^=`, `<<=`, and `>>=` (on a value with the sign bit
//     set as well as without);
//   * uses: the expression's value returned, the object read back
//     after the operator, the value compared in an `if`, and the value
//     passed through a call.
//
// The starting values sit on a carry or borrow boundary (`0x00FF`
// before `++`, `0x0100` before `--`, and their wider forms), so a
// multi-byte operator has to carry into every byte. Each expected
// result is computed under C99's rules with c6502's widths. Every
// program runs with and without -O.
//
// This grid is what found `return ++c;` for a static `unsigned char`
// losing A: the INC peephole left A without the new value, and neither
// the chapter corpus nor the fuzzers had a 1-byte object incremented in
// place and then returned.
//
// Cases are split into programs of at most `kChunk` functions, at most
// `kZpBudget` bytes of zero-page globals, and at most `kResultBudget`
// bytes of results, so that the unoptimized code stays clear of the soft
// stack, its `main` frame stays under 253 bytes, and the automatic
// zero-page range doesn't fill.
#include <cstdio>
#include <set>
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

using asdl::Int;

struct CType {
  const char* c;
  int size;
  bool is_signed;
};

const CType kTypes[] = {
    {"unsigned char", 1, false}, {"signed char", 1, true}, {"unsigned int", 2, false},       {"int", 2, true},
    {"unsigned long", 4, false}, {"long", 4, true},        {"unsigned long long", 8, false}, {"long long", 8, true},
};

const char* const kStorage[] = {"local", "static", "global", "zp", "ptr", "array", "member"};

struct Op {
  const char* name;
  const char* text;
  const char* kind;
};

const Op kOps[] = {
    {"preinc", "++X", "carry"},        {"predec", "--X", "borrow"},    {"postinc", "X++", "carry"},
    {"postdec", "X--", "borrow"},      {"add", "X += 3", "carry"},     {"sub", "X -= 5", "borrow"},
    {"mul", "X *= 3", "small"},        {"and", "X &= 0x5A", "pattern"}, {"or", "X |= 0x81", "pattern"},
    {"xor", "X ^= 0xFF", "pattern"},   {"shl", "X <<= 1", "pattern"},  {"shr", "X >>= 1", "pattern"},
    {"shr_neg", "X >>= 1", "negative"},
};

const char* const kUses[] = {"ret", "after", "if", "arg"};

constexpr std::size_t kChunk = 40;
constexpr int kZpBudget = 80;
// Without -O, main holds each call's result in a frame temp, and a frame
// is at most 253 bytes.
constexpr int kResultBudget = 160;
constexpr int kShards = 8;
// The number of programs and a CRC-32 of their sources concatenated, as
// test_op_matrix_sim.py's generator gave them on 27 Nov 2026.
constexpr std::size_t kOpMatrixPrograms = 88;
constexpr std::uint32_t kOpMatrixCrc = 0x9854DDA3;

Int wrap(Int v, int size, bool is_signed) {
  const int bits = size * 8;
  v &= (Int(1) << bits) - 1;
  if (is_signed && (v >> (bits - 1)) != 0) v -= Int(1) << bits;
  return v;
}

Int hex_repeat(const std::string& head, const std::string& unit, int n) {
  std::string s = head;
  for (int i = 0; i < n; ++i) s += unit;
  Int v = 0;
  for (const char ch : s) v = v * 16 + (ch <= '9' ? ch - '0' : ch - 'A' + 10);
  return v;
}

Int start_value(const std::string& kind, int size) {
  const int bits = size * 8;
  if (kind == "carry") return size > 1 ? (Int(1) << (bits - 8)) - 1 : 0xFF;
  if (kind == "borrow") return size > 1 ? Int(1) << (bits - 8) : 0x00;
  if (kind == "small") return size > 1 ? hex_repeat("12", "34", size - 1) : 0x5A;
  if (kind == "pattern") return hex_repeat("", "5A", size);
  return hex_repeat("", "A5", size);
}

struct Effect {
  Int value;
  Int object_after;
};

Effect apply(const std::string& op, Int x, int size, bool is_signed) {
  const Int p = size == 1 ? wrap(x, 2, true) : wrap(x, size, is_signed);
  const auto w = [&](Int v) { return wrap(v, size, is_signed); };
  if (op == "preinc") return {w(p + 1), w(p + 1)};
  if (op == "predec") return {w(p - 1), w(p - 1)};
  if (op == "postinc") return {x, w(p + 1)};
  if (op == "postdec") return {x, w(p - 1)};
  Int n = 0;
  if (op == "add") n = w(p + 3);
  if (op == "sub") n = w(p - 5);
  if (op == "mul") n = w(p * 3);
  if (op == "and") n = w(p & 0x5A);
  if (op == "or") n = w(p | 0x81);
  if (op == "xor") n = w(p ^ 0xFF);
  if (op == "shl") n = w(p << 1);
  // Arithmetic for a negative signed value, as c6502 does.
  if (op == "shr" || op == "shr_neg") n = w(p >> 1);
  return {n, n};
}

std::string literal(Int v, bool is_signed) { return asdl::int_to_string(v) + (is_signed ? "LL" : "ULL"); }

struct Case {
  std::string name;
  CType type;
  std::string storage;
  const Op* op;
  std::string use;
  Int start;
  Int want;
  int ret_size;

  std::string describe() const {
    char buf[40];
    std::snprintf(buf, sizeof buf, "%#llx", static_cast<unsigned long long>(start));
    return std::string(op->name) + " on " + storage + " " + type.c + " (start " +
           (start == 0 ? std::string("0x0") : std::string(buf)) + "), use " + use;
  }
};

std::vector<Case> cases() {
  std::vector<Case> out;
  int n = 0;
  for (const CType& t : kTypes) {
    for (const char* storage : kStorage) {
      for (const Op& op : kOps) {
        if (std::string(op.name) == "shr_neg" && !t.is_signed) continue;
        const Int start = wrap(start_value(op.kind, t.size), t.size, t.is_signed);
        const auto [value, after] = apply(op.name, start, t.size, t.is_signed);
        for (const char* use : kUses) {
          Int want = value;
          int ret_size = t.size;
          if (std::string(use) == "if") {
            want = 1;
            ret_size = 2;
          } else if (std::string(use) == "after") {
            want = after;
          }
          out.push_back({"c" + std::to_string(n), t, storage, &op, use, start, want, ret_size});
          ++n;
        }
      }
    }
  }
  return out;
}

std::string tag(const std::string& ctype) {
  std::string out = ctype;
  for (char& ch : out) {
    if (ch == ' ') ch = '_';
  }
  return out;
}

std::string replace_x(const std::string& text, const std::string& obj) {
  std::string out;
  for (const char ch : text) out += ch == 'X' ? obj : std::string(1, ch);
  return out;
}

std::pair<std::vector<std::string>, std::string> function(const Case& c) {
  const std::string t = c.type.c;
  const std::string init = literal(c.start, c.type.is_signed);
  std::vector<std::string> decls;
  std::string pre, obj;
  if (c.storage == "local") {
    pre = "    " + t + " x = (" + t + ")(" + init + ");\n";
    obj = "x";
  } else if (c.storage == "static") {
    pre = "    static " + t + " x = (" + t + ")(" + init + ");\n";
    obj = "x";
  } else if (c.storage == "global") {
    decls.push_back(t + " g_" + c.name + " = (" + t + ")(" + init + ");");
    obj = "g_" + c.name;
  } else if (c.storage == "zp") {
    decls.push_back(t + " z_" + c.name + " [[c6502::zeropage]] = (" + t + ")(" + init + ");");
    obj = "z_" + c.name;
  } else if (c.storage == "ptr") {
    decls.push_back(t + " t_" + c.name + " = (" + t + ")(" + init + ");");
    pre = "    " + t + " *p = &t_" + c.name + ";\n";
    obj = "(*p)";
  } else if (c.storage == "array") {
    decls.push_back(t + " a_" + c.name + "[3] = {0, (" + t + ")(" + init + "), 0};");
    obj = "a_" + c.name + "[one]";
  } else {
    decls.push_back("struct S_" + tag(t) + " s_" + c.name + " = {0, (" + t + ")(" + init + ")};");
    obj = "s_" + c.name + ".m";
  }
  const std::string expr = "(" + replace_x(c.op->text, obj) + ")";
  const std::string rtype = c.use == "if" ? "int" : t;
  std::string body;
  if (c.use == "ret") {
    body = "    return " + expr + ";\n";
  } else if (c.use == "after") {
    body = "    " + expr + ";\n    return " + obj + ";\n";
  } else if (c.use == "if") {
    const Int value = apply(c.op->name, c.start, c.type.size, c.type.is_signed).value;
    body = "    if (" + expr + " == (" + t + ")(" + literal(value, c.type.is_signed) + ")) return 1;\n    return 0;\n";
  } else {
    body = "    return id_" + tag(t) + "(" + expr + ");\n";
  }
  return {decls, rtype + " f_" + c.name + "(void) {\n" + pre + body + "}\n"};
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

Program emit(const std::vector<Case>& chunk) {
  std::set<std::string> types;
  for (const Case& c : chunk) types.insert(c.type.c);
  std::vector<std::string> head{"unsigned char one = 1;"};
  for (const std::string& t : types) {
    head.push_back("struct S_" + tag(t) + " { unsigned char pad; " + t + " m; };");
    head.push_back(t + " id_" + tag(t) + "(" + t + " v) { return v; }");
  }
  std::vector<std::string> decls, results, calls;
  std::string fns;
  for (const Case& c : chunk) {
    auto [d, f] = function(c);
    decls.insert(decls.end(), d.begin(), d.end());
    fns += f;
    results.push_back((c.use == "if" ? std::string("int") : std::string(c.type.c)) + " r_" + c.name + ";");
    calls.push_back("    r_" + c.name + " = f_" + c.name + "();");
  }
  std::vector<std::string> all = head;
  all.insert(all.end(), decls.begin(), decls.end());
  all.insert(all.end(), results.begin(), results.end());
  std::string src = join_lines(all) + "\n" + fns;
  src += "int main(void) {\n" + join_lines(calls) + "\n    return 0;\n}\n";
  return {src, chunk};
}

const std::vector<Program>& programs() {
  static const std::vector<Program> out = [] {
    std::vector<Program> ps;
    std::vector<Case> chunk;
    int zp_bytes = 0, result_bytes = 0;
    for (const Case& c : cases()) {
      const int extra = c.storage == "zp" ? c.type.size : 0;
      if (!chunk.empty() && (chunk.size() >= kChunk || zp_bytes + extra > kZpBudget ||
                             result_bytes + c.ret_size > kResultBudget)) {
        ps.push_back(emit(chunk));
        chunk.clear();
        zp_bytes = result_bytes = 0;
      }
      chunk.push_back(c);
      zp_bytes += extra;
      result_bytes += c.ret_size;
    }
    if (!chunk.empty()) ps.push_back(emit(chunk));
    return ps;
  }();
  return out;
}

void check(Pipeline pipeline, int shard) {
  std::vector<std::string> failures;
  const std::vector<Program>& ps = programs();
  for (std::size_t k = 0; k < ps.size(); ++k) {
    if (static_cast<int>(k % kShards) != shard) continue;
    const Program& p = ps[k];
    sim::Simulation s;
    try {
      s = build_sim(p.source, pipeline);
      s.run();
    } catch (const std::exception& e) {
      failures.push_back(p.cases[0].name + "..: " + e.what());
      continue;
    }
    for (const Case& c : p.cases) {
      const std::int64_t addr = s.symbol("r_" + c.name);
      Int raw = 0;
      for (int b = 0; b < c.ret_size; ++b) raw |= Int(s.cpu.memory[static_cast<std::size_t>(addr + b)]) << (8 * b);
      const Int want = c.want & ((Int(1) << (8 * c.ret_size)) - 1);
      if (raw != want) {
        failures.push_back(c.name + ": " + c.describe() + ": got " + asdl::int_to_string(raw) + ", want " +
                           asdl::int_to_string(want));
      }
    }
  }
  std::string shown;
  for (std::size_t i = 0; i < failures.size() && i < 40; ++i) shown += failures[i] + "\n";
  CHECK_MESSAGE(failures.empty(), shown);
}

}  // namespace

TEST_CASE("op matrix sim: the programs are those test_op_matrix_sim.py wrote") {
  std::string all;
  for (const Program& p : programs()) all += p.source;
  CHECK(programs().size() == kOpMatrixPrograms);
  CHECK(sim::cpu_run::crc32(all) == kOpMatrixCrc);
}

TEST_CASE("shard: op matrix unoptimized 0") { check(Pipeline::kUnoptimized, 0); }
TEST_CASE("shard: op matrix unoptimized 1") { check(Pipeline::kUnoptimized, 1); }
TEST_CASE("shard: op matrix unoptimized 2") { check(Pipeline::kUnoptimized, 2); }
TEST_CASE("shard: op matrix unoptimized 3") { check(Pipeline::kUnoptimized, 3); }
TEST_CASE("shard: op matrix unoptimized 4") { check(Pipeline::kUnoptimized, 4); }
TEST_CASE("shard: op matrix unoptimized 5") { check(Pipeline::kUnoptimized, 5); }
TEST_CASE("shard: op matrix unoptimized 6") { check(Pipeline::kUnoptimized, 6); }
TEST_CASE("shard: op matrix unoptimized 7") { check(Pipeline::kUnoptimized, 7); }
TEST_CASE("shard: op matrix optimized 0") { check(Pipeline::kOptimized, 0); }
TEST_CASE("shard: op matrix optimized 1") { check(Pipeline::kOptimized, 1); }
TEST_CASE("shard: op matrix optimized 2") { check(Pipeline::kOptimized, 2); }
TEST_CASE("shard: op matrix optimized 3") { check(Pipeline::kOptimized, 3); }
TEST_CASE("shard: op matrix optimized 4") { check(Pipeline::kOptimized, 4); }
TEST_CASE("shard: op matrix optimized 5") { check(Pipeline::kOptimized, 5); }
TEST_CASE("shard: op matrix optimized 6") { check(Pipeline::kOptimized, 6); }
TEST_CASE("shard: op matrix optimized 7") { check(Pipeline::kOptimized, 7); }

}  // namespace c6502::suite
