// <stdbit.h> run in the simulator, from tests/test_stdbit.py: the
// functions a program reaches through their addresses, one unit's and
// two linked units', and every function in its type-generic form and its
// form with a suffix on all 256 values of an `unsigned char` and a sweep
// of edge and random values of the wider types, in both pipelines.
#include <cstdint>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"
#include "binutils/elf.h"
#include "builtin_functions.h"
#include "compile_case.h"
#include "doctest.h"
#include "py_random.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("stdbit sim: the functions with a suffix run as functions in both pipelines") {
  // C23 §7.1.4 makes each of them a function too.
  const char* src = R"c(
#include <stdbit.h>
typedef unsigned (*count_fn)(unsigned char);
static count_fn counts[] = { &stdc_leading_zeros_uc, stdc_count_ones_uc };
unsigned char (*floor_fn)(unsigned char) = stdc_bit_floor_uc;
int main(void) {
    unsigned r = counts[0](0x10) * 100;       /* 3 */
    r += counts[1](0xF0) * 10;               /* 4 */
    r += (stdc_trailing_zeros_ul)(0x100);    /* 8 */
    r += floor_fn(100) == 64;                /* 1 */
#undef stdc_bit_width_ui
    r += stdc_bit_width_ui(0x1000) * 1000;   /* 13 */
    return r;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(2'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK(r.return_int() == 13349);
  }
}

TEST_CASE("stdbit sim: two units that take a function's address each have a copy") {
  const std::string a = R"c(#include <stdbit.h>
unsigned (*pa)(unsigned char) = stdc_count_ones_uc;
unsigned a(void) { return (stdc_count_ones_uc)(7); }
)c";
  const std::string b = R"c(#include <stdbit.h>
unsigned a(void);
unsigned (*pb)(unsigned char) = stdc_count_ones_uc;
int main(void) { return a() * 10 + (stdc_count_ones_uc)(3); }
)c";
  const TempDir d;
  CHECK(compile_object(a, d.file("a.o")) == 0);
  CHECK(compile_object(b, d.file("b.o")) == 0);
  const auto o = link({d.file("a.o"), d.file("b.o")}, d.file("prog"));
  REQUIRE(o.status.value_or(-1) == 0);
  const std::string exe = read_file(d.file("prog"));
  CHECK(sim::build_linked_sim(exe).run().return_int() == 32);
  const binutils::elf::ElfFile file = binutils::elf::read(exe, "prog");
  int copies = 0;
  for (const auto& s : file.symbols) {
    if (std::string_view(s->name).substr(0, 18) == "stdc_count_ones_uc") ++copies;
  }
  CHECK(copies == 2);
}

namespace {

struct ValueType {
  const char* suffix;
  const char* name;
  int width;
};

std::string result_type(std::string_view op, const std::string& value_type) {
  if (op == "bit_floor" || op == "bit_ceil") return value_type;
  if (op == "has_single_bit") return "bool";
  return "unsigned int";
}

// Edge values of a `width`-byte unsigned type and a few random ones:
// 0, 1, the all-ones value, each power of 2 and its neighbors for every
// third exponent, and the top bit's neighbors, sorted.
std::vector<asdl::Int> sweep(int width) {
  const int bits = 8 * width;
  const asdl::Int one = 1;
  const asdl::Int top = one << (bits - 1);
  const asdl::Int all = (one << bits) - 1;
  std::set<asdl::Int> values = {0, 1, 2, 3, all, all - 1, top, top - 1, top + 1};
  for (int k = 0; k < bits; k += 3) {
    values.insert(one << k);
    values.insert((one << k) - 1);
    values.insert(((one << k) + 1) & all);
  }
  py_random::Random rnd(width);
  for (int i = 0; i < 6; ++i) values.insert(rnd.getrandbits(bits));
  return {values.begin(), values.end()};
}

// A program that stores `stdc_<op>` (or its form with `suffix`) of each
// of `values` in an array `out_<op>`.
std::string program(const char* suffix, const std::string& tname, const std::vector<asdl::Int>& values,
                    const std::vector<std::string_view>& operations) {
  const std::string n = std::to_string(values.size());
  std::string list;
  for (std::size_t i = 0; i < values.size(); ++i) {
    list += (i ? ", " : "") + asdl::int_to_string(values[i]) + "ull";
  }
  std::vector<std::string> lines = {"#include <stdbit.h>"};
  lines.push_back("static const " + tname + " in[" + n + "] = {" + list + "};");
  for (const std::string_view op : operations) {
    lines.push_back(result_type(op, tname) + " out_" + std::string(op) + "[" + n + "];");
  }
  lines.push_back("int main(void) {");
  lines.push_back("    for (unsigned int i = 0; i < " + n + "; i++) {");
  lines.push_back("        " + tname + " v = in[i];");
  for (const std::string_view op : operations) {
    const std::string fn = suffix ? "stdc_" + std::string(op) + "_" + suffix : "stdc_" + std::string(op);
    lines.push_back("        out_" + std::string(op) + "[i] = " + fn + "(v);");
  }
  for (const char* l : {"    }", "    return 0;", "}"}) lines.push_back(l);
  std::string out;
  for (std::size_t i = 0; i < lines.size(); ++i) out += (i ? "\n" : "") + lines[i];
  return out + "\n";
}

// What tests/test_stdbit.py's generator gave on 27 Nov 2026 for each
// type: the number of values, the number of programs, and the CRC-32s
// of the programs' sources concatenated, of the values joined by
// commas, and of the results of `builtin_functions.stdbit` for each
// operation in order, each result followed by a comma and each
// operation's by a newline.
struct Frozen {
  std::size_t values;
  std::size_t programs;
  std::uint32_t sources_crc;
  std::uint32_t values_crc;
  std::uint32_t results_crc;
};

// Each 8-byte program has half the functions, since the unoptimized
// pipeline's frame for all 14 would pass 253 bytes.
void check_sweep(const ValueType& type, const Frozen& frozen) {
  std::vector<asdl::Int> values;
  if (type.width == 1) {
    for (int v = 0; v < 256; ++v) values.push_back(v);
  } else {
    values = sweep(type.width);
  }
  const int bits = 8 * type.width;
  const std::vector<std::string_view>& ops = builtin_functions::stdbit_operations();
  std::vector<std::vector<std::string_view>> groups;
  if (type.width == 8) {
    groups.emplace_back(ops.begin(), ops.begin() + 7);
    groups.emplace_back(ops.begin() + 7, ops.end());
  } else {
    groups.push_back(ops);
  }

  std::string joined_values, results, sources;
  for (std::size_t i = 0; i < values.size(); ++i) joined_values += (i ? "," : "") + asdl::int_to_string(values[i]);
  for (const std::string_view op : ops) {
    for (const asdl::Int v : values) results += asdl::int_to_string(builtin_functions::stdbit(op, v, bits)) + ",";
    results += "\n";
  }
  std::size_t programs = 0;
  for (const char* form : {static_cast<const char*>(nullptr), type.suffix}) {
    for (const auto& operations : groups) {
      const std::string src = program(form, type.name, values, operations);
      sources += src;
      ++programs;
      for (const bool optimize : {false, true}) {
        sim::Simulation s = build_sim(src, optimize);
        const sim::SimResult r = s.run(60'000'000);
        CHECK_FALSE(r.timed_out);
        for (const std::string_view op : operations) {
          const int size = op == "bit_floor" || op == "bit_ceil" ? type.width : op == "has_single_bit" ? 1 : 2;
          const std::int64_t base = s.symbol("out_" + std::string(op));
          std::vector<std::uint64_t> got, want;
          for (std::size_t i = 0; i < values.size(); ++i) {
            got.push_back(r.word(base + size * static_cast<std::int64_t>(i), size));
            want.push_back(static_cast<std::uint64_t>(builtin_functions::stdbit(op, values[i], bits)));
          }
          INFO("op=" << op << " form=" << (form ? form : "generic") << " optimize=" << optimize);
          CHECK(got == want);
        }
      }
    }
  }
  CHECK(values.size() == frozen.values);
  CHECK(programs == frozen.programs);
  CHECK(sim::cpu_run::crc32(sources) == frozen.sources_crc);
  CHECK(sim::cpu_run::crc32(joined_values) == frozen.values_crc);
  CHECK(sim::cpu_run::crc32(results) == frozen.results_crc);
}

}  // namespace

TEST_CASE("stdbit sim: every function on every unsigned char") {
  check_sweep({"uc", "unsigned char", 1}, {256, 2, 0xad2565f6u, 0x18b18742u, 0x52a90b8bu});
}

TEST_CASE("stdbit sim: every function on a sweep of unsigned short") {
  check_sweep({"us", "unsigned short", 2}, {27, 2, 0x40da7b61u, 0x9a6183bbu, 0x856068e4u});
}

TEST_CASE("stdbit sim: every function on a sweep of unsigned int") {
  check_sweep({"ui", "unsigned int", 2}, {27, 2, 0xcf1e10c5u, 0x9a6183bbu, 0x856068e4u});
}

TEST_CASE("stdbit sim: every function on a sweep of unsigned long") {
  check_sweep({"ul", "unsigned long", 4}, {45, 2, 0x9fb48747u, 0xcb812a38u, 0x66414303u});
}

TEST_CASE("stdbit sim: every function on a sweep of unsigned long long") {
  check_sweep({"ull", "unsigned long long", 8}, {75, 4, 0x0aea16e0u, 0xe4966c3eu, 0x8bc80862u});
}

}  // namespace c6502::suite
