// The tests of tests/test_stdbit.py that run no simulator: the model of
// <stdbit.h> in builtin_functions.h against one written from C23
// §7.18's bit-by-bit definitions, the folding of constant arguments,
// the header's names, and the code a few calls compile to.
#include <algorithm>
#include <optional>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "builtin_functions.h"
#include "compile_case.h"
#include "doctest.h"
#include "py_random.h"

namespace {

namespace suite = c6502::suite;
namespace bf = c6502::builtin_functions;
namespace ir = c6502::tac_ast;
using c6502::asdl::Int;

struct TypeInfo {
  std::string suffix;
  std::string name;
  int bytes;
};

const std::vector<TypeInfo>& types() {
  static const std::vector<TypeInfo> table = {
      {"uc", "unsigned char", 1}, {"us", "unsigned short", 2},      {"ui", "unsigned int", 2},
      {"ul", "unsigned long", 4}, {"ull", "unsigned long long", 8},
  };
  return table;
}

std::string result_type(std::string_view operation, const std::string& value_type) {
  if (operation == "bit_floor" || operation == "bit_ceil") return value_type;
  if (operation == "has_single_bit") return "bool";
  return "unsigned int";
}

// `operation` computed from C23 §7.18's wording, one bit at a time.
// `msi(k)` is the bit at most significant index k (§7.18.1p3) and
// `lsi(k)` the bit at least significant index k (§7.18.1p4).
Int by_index(std::string_view operation, Int value, int width) {
  const auto msi = [&](int k) { return static_cast<int>((value >> (width - 1 - k)) & 1); };
  const auto lsi = [&](int k) { return static_cast<int>((value >> k) & 1); };
  const auto run = [&](auto bit, int want) {
    int n = 0;
    while (n < width && bit(n) == want) ++n;
    return n;
  };
  const auto first = [&](auto bit, int want) {
    for (int k = 0; k < width; ++k) {
      if (bit(k) == want) return k + 1;
    }
    return 0;
  };
  int ones = 0;
  for (int k = 0; k < width; ++k) ones += lsi(k);
  if (operation == "leading_zeros") return run(msi, 0);
  if (operation == "leading_ones") return run(msi, 1);
  if (operation == "trailing_zeros") return run(lsi, 0);
  if (operation == "trailing_ones") return run(lsi, 1);
  if (operation == "first_leading_zero") return first(msi, 0);
  if (operation == "first_leading_one") return first(msi, 1);
  if (operation == "first_trailing_zero") return first(lsi, 0);
  if (operation == "first_trailing_one") return first(lsi, 1);
  if (operation == "count_zeros") return width - ones;
  if (operation == "count_ones") return ones;
  if (operation == "has_single_bit") return ones == 1 ? 1 : 0;
  if (operation == "bit_width") {
    // "the smallest number of bits needed to store value"
    for (int n = 0; n <= width; ++n) {
      if (value < (Int(1) << n)) return n;
    }
  }
  if (operation == "bit_floor") {
    Int best = 0;
    for (int k = 0; k < width; ++k) {
      if ((Int(1) << k) <= value) best = Int(1) << k;
    }
    return best;
  }
  if (operation == "bit_ceil") {
    for (int k = 0; k < width; ++k) {
      if ((Int(1) << k) >= value) return Int(1) << k;
    }
    return 0;
  }
  FAIL("unknown operation " << operation);
  return 0;
}

// Edge values of a `bytes`-byte unsigned type and a few random ones: 0,
// 1, the all-ones value, each power of 2 and its neighbors for every
// third exponent, and the top bit's neighbors, in increasing order.
std::vector<Int> sweep(int bytes) {
  const int bits = 8 * bytes;
  const Int mask = (Int(1) << bits) - 1;
  const Int top = Int(1) << (bits - 1);
  std::set<Int> values = {0, 1, 2, 3, mask, mask - 1, top, top - 1, top + 1};
  for (int k = 0; k < bits; k += 3) {
    values.insert(Int(1) << k);
    values.insert((Int(1) << k) - 1);
    values.insert(((Int(1) << k) + 1) & mask);
  }
  c6502::py_random::Random rnd{Int(bytes)};
  for (int i = 0; i < 6; ++i) values.insert(rnd.getrandbits(bits));
  return {values.begin(), values.end()};
}

unsigned long long u64(Int v) { return static_cast<unsigned long long>(v); }

// The value of the constant `v`, or none for a floating one.
std::optional<Int> constant_value(const ir::Type_const& v) {
  return std::visit(
      [](const auto& c) -> std::optional<Int> {
        using C = std::decay_t<decltype(c)>;
        if constexpr (std::is_same_v<C, ir::ConstFloat> || std::is_same_v<C, ir::ConstDouble>) {
          return std::nullopt;
        } else {
          return c.value;
        }
      },
      v);
}

std::vector<std::string> f_body(const std::string& call, const std::string& decls) {
  return suite::function_body(
      suite::codegen("#include <stdbit.h>\n" + decls + "\nvoid f(void) { r = " + call + "; }\n",
                     true),
      "f");
}

std::vector<std::string> findall(const std::string& pattern, const std::string& text,
                                 int group = 1) {
  const std::regex re{pattern};
  std::vector<std::string> out;
  for (auto it = std::sregex_iterator(text.begin(), text.end(), re); it != std::sregex_iterator();
       ++it) {
    out.push_back((*it)[group].str());
  }
  return out;
}

}  // namespace

TEST_CASE("stdbit: the model matches the standard's definitions by index") {
  const auto check = [](int width, const std::vector<Int>& values) {
    for (const Int value : values) {
      for (const std::string_view op : bf::stdbit_operations()) {
        INFO(op << " width=" << width << " value=" << u64(value));
        CHECK(u64(bf::stdbit(op, value, width)) == u64(by_index(op, value, width)));
      }
    }
  };
  std::vector<Int> bytes;
  for (int v = 0; v < 256; ++v) bytes.push_back(v);
  check(8, bytes);
  check(16, sweep(2));
  check(32, sweep(4));
  check(64, sweep(8));
}

TEST_CASE("stdbit: the model's results for 0 and the all-ones value") {
  // Every operation defines them (none of §7.18 is undefined).
  for (const int width : {8, 16, 32, 64}) {
    const Int ones = (Int(1) << width) - 1;
    struct Edge {
      std::string_view op;
      Int zero;
      Int all;
    };
    const std::vector<Edge> want = {
        {"leading_zeros", width, 0},
        {"leading_ones", 0, width},
        {"trailing_zeros", width, 0},
        {"trailing_ones", 0, width},
        {"first_leading_zero", 1, 0},
        {"first_leading_one", 0, 1},
        {"first_trailing_zero", 1, 0},
        {"first_trailing_one", 0, 1},
        {"count_zeros", width, 0},
        {"count_ones", 0, width},
        {"has_single_bit", 0, 0},
        {"bit_width", 0, width},
        {"bit_floor", 0, Int(1) << (width - 1)},
        // The power of 2 doesn't fit (§7.18.16p2).
        {"bit_ceil", 1, 0},
    };
    REQUIRE(want.size() == bf::stdbit_operations().size());
    for (const auto& [op, zero, all] : want) {
      INFO(op << " width=" << width);
      CHECK(std::find(bf::stdbit_operations().begin(), bf::stdbit_operations().end(), op) !=
            bf::stdbit_operations().end());
      CHECK(u64(bf::stdbit(op, 0, width)) == u64(zero));
      CHECK(u64(bf::stdbit(op, ones, width)) == u64(all));
    }
  }
}

TEST_CASE("stdbit: the registry of operations and builtins") {
  CHECK(bf::stdbit_operations().size() == 14);
  CHECK(bf::stdbit_builtins().size() == 14 * 6);
  std::size_t with_suffix = 0;
  for (const bf::StdbitBuiltin& b : bf::stdbit_builtins()) {
    INFO(b.name);
    CHECK(b.name.rfind("__c6502_stdc_", 0) == 0);
    CHECK(bf::builtin_arity(b.name) == std::optional<int>(1));
    if (b.suffix) ++with_suffix;
  }
  CHECK(with_suffix == 14 * 5);
}

TEST_CASE("stdbit: constant arguments fold in both pipelines") {
  for (const TypeInfo& t : types()) {
    const Int value = t.bytes == 1 ? Int(0x50) : Int(0x0F00) << (8 * (t.bytes - 2));
    const std::string text = std::to_string(u64(value));
    for (const std::string_view op_view : bf::stdbit_operations()) {
      const std::string op(op_view);
      const std::string src = "#include <stdbit.h>\n" + result_type(op, t.name) +
                              " r;\nvoid f(void) { r = stdc_" + op + "((" + t.name + ")" + text +
                              "); r = stdc_" + op + "_" + t.suffix + "(" + text + "); }\n";
      const Int want = by_index(op, value, 8 * t.bytes);
      for (const bool optimize : {false, true}) {
        INFO(op << " type=" << t.name << " optimize=" << optimize);
        suite::Options o;
        o.optimize = optimize;
        const suite::Tac tac = suite::tac_program(src, o);
        const ir::Function* fn = nullptr;
        for (const auto& tl : tac.program.top_level) {
          if ((fn = std::get_if<ir::Function>(&tl)) != nullptr) break;
        }
        REQUIRE(fn != nullptr);
        bool label = false;
        bool found = false;
        for (const auto& ins : fn->instructions) {
          if (std::holds_alternative<ir::Label>(ins)) label = true;
          if (const auto* copy = std::get_if<ir::Copy>(&ins)) {
            if (const auto* c = std::get_if<ir::Constant>(&copy->src)) {
              if (constant_value(c->const_) == std::optional<Int>(want)) found = true;
            }
          }
        }
        CHECK_FALSE(label);
        CHECK(found);
      }
    }
  }
}

TEST_CASE("stdbit: the header's names are in its namespace") {
  // Every name the header declares or defines is one C23 §7.18 lists,
  // `size_t` or an integer type from §7.18.1p1, a name in the
  // implementation's namespace, or a `stdc_` name (§7.33.12).
  std::string text = suite::read_repo_file("include/stdbit.h");
  text = std::regex_replace(text, std::regex{"/\\*[\\s\\S]*?\\*/"}, "");
  std::set<std::string> names;
  for (std::string& n : findall("#define\\s+(\\w+)", text)) names.insert(std::move(n));
  for (std::string& n : findall("typedef\\s+[\\w\\s]+?\\s(\\w+);", text))
    names.insert(std::move(n));
  {
    std::smatch m;
    const std::regex decl{"(\\w+)\\(\\w[\\w\\s]*\\s(\\w+)\\);"};
    REQUIRE(std::regex_search(text, m, decl));
    names.insert(m[1].str());
    names.insert(m[2].str());
  }
  for (std::string& n : findall("\\s(\\w+)\\(", text)) names.insert(std::move(n));
  const std::regex allowed{"_[A-Z_]\\w*|__\\w*|stdc_\\w+|size_t|u?int(_least)?(8|16|32|64)_t"};
  std::vector<std::string> bad;
  for (const std::string& n : names) {
    if (!std::regex_match(n, allowed)) bad.push_back(n);
  }
  CHECK(bad == std::vector<std::string>{});
}

TEST_CASE("stdbit: every function is declared and defined as a macro") {
  const std::string text = suite::read_repo_file("include/stdbit.h");
  for (const std::string_view op_view : bf::stdbit_operations()) {
    const std::string op(op_view);
    INFO(op);
    CHECK(suite::contains(text, "#define stdc_" + op + "(x) __c6502_stdc_" + op + "(x)"));
    for (const TypeInfo& t : types()) {
      INFO(t.name);
      CHECK(suite::contains(text, result_type(op, t.name) + " stdc_" + op + "_" + t.suffix + "(" +
                                      t.name + " __value);"));
      CHECK(suite::contains(text, "#define stdc_" + op + "_" + t.suffix + "(x) __c6502_stdc_" + op +
                                      "_" + t.suffix + "(x)"));
    }
  }
}

TEST_CASE("stdbit: unused declarations leave nothing in the output") {
  std::string asm_text = suite::codegen(
      "#include <stdbit.h>\nunsigned char c; unsigned r;\n"
      "void f(void) { r = stdc_leading_zeros(c); }\n",
      true);
  for (std::size_t at = asm_text.find(".stdbit_"); at != std::string::npos;
       at = asm_text.find(".stdbit_", at)) {
    asm_text.erase(at, 8);
  }
  CHECK_FALSE(suite::contains(asm_text, "stdc_"));
}

TEST_CASE("stdbit: the leading zeros of a byte") {
  // Shift a copy right until it's zero, counting down from 8.
  CHECK(f_body("stdc_leading_zeros(c)", "unsigned char c; unsigned r;") ==
        std::vector<std::string>{"LDA c", "STA __local_f__1", "LDA #$08", "STA __local_f__0",
                                 "JMP .stdbit_test@2", ".stdbit_body@1:", "LSR __local_f__1",
                                 "DEC __local_f__0", ".stdbit_test@2:", "LDA __local_f__1",
                                 "BNE .stdbit_body@1", "LDA __local_f__0", "STA r", "LDA #$00",
                                 "STA r+1", "RTS"});
}

TEST_CASE("stdbit: the count of ones of an int") {
  // The low bit is tested with `LSR` of a copy in A, and then the two
  // bytes shift right in place.
  CHECK(f_body("stdc_count_ones(u)", "unsigned u; unsigned r;") ==
        std::vector<std::string>{"LDA u",
                                 "STA __local_f__2",
                                 "LDA u+1",
                                 "STA __local_f__3",
                                 "LDA #$00",
                                 "STA __local_f__0",
                                 "JMP .stdbit_test@2",
                                 ".stdbit_body@1:",
                                 "LDA __local_f__2",
                                 "LSR",
                                 "BCC .stdbit_skip@3",
                                 "INC __local_f__0",
                                 ".stdbit_skip@3:",
                                 "LSR __local_f__3",
                                 "ROR __local_f__2",
                                 ".stdbit_test@2:",
                                 "LDA __local_f__2",
                                 "ORA __local_f__3",
                                 "BNE .stdbit_body@1",
                                 "LDA __local_f__0",
                                 "STA r",
                                 "LDA #$00",
                                 "STA r+1",
                                 "RTS"});
}

TEST_CASE("stdbit: has_single_bit of a byte") {
  const std::vector<std::string> body =
      f_body("stdc_has_single_bit(c)", "unsigned char c; bool r;");
  CHECK(std::find(body.begin(), body.end(), "SBC #$01") != body.end());
  CHECK(std::find(body.begin(), body.end(), "AND c") != body.end());
}

TEST_CASE("stdbit: no function calls a helper") {
  // Every function is written in place, with no call of a runtime helper.
  for (const std::string_view op_view : bf::stdbit_operations()) {
    const std::string op(op_view);
    for (const TypeInfo& t : types()) {
      for (const bool optimize : {false, true}) {
        INFO(op << " type=" << t.name << " optimize=" << optimize);
        const std::string asm_text =
            suite::codegen("#include <stdbit.h>\n" + t.name + " g; " + result_type(op, t.name) +
                               " r;\nvoid f(void) { r = stdc_" + op + "(g); }\n",
                           optimize);
        std::string joined;
        for (const std::string& line : suite::function_body(asm_text, "f")) joined += line + " ";
        CHECK_FALSE(suite::contains(joined, "JSR"));
      }
    }
  }
}
