// The TAC interpreter (src/tac_sim.h) on the TAC optimizer's output,
// which holds the instructions the optimizer makes and c99_to_tac
// doesn't, and on a function in SSA form. Each program is compiled
// without the preprocessor, which none of them needs.
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "c99_to_tac.h"
#include "doctest.h"
#include "parser.h"
#include "passes/identifier_resolution.h"
#include "passes/label_resolution.h"
#include "passes/loop_labeling.h"
#include "passes/optimization/optimizer.h"
#include "passes/optimization/ssa_construction.h"
#include "passes/string_lifting.h"
#include "passes/type_checker.h"
#include "tac_sim.h"

namespace {

namespace ir = c6502::tac_ast;
namespace c99 = c6502::c99_ast;
namespace passes = c6502::passes;
using c6502::asdl::Int;
using c6502::tac_sim::Simulator;

struct Compiled {
  ir::Program tac;
  passes::SymbolTable symbols;
  passes::TypeTable types;
};

Compiled compile_to_tac(const std::string& source) {
  c99::Program prog = c6502::parser::parse(source);
  prog = passes::label_program(passes::resolve_labels(
      passes::lift_strings(passes::resolve_identifiers(std::move(prog)))));
  auto tables = passes::type_checking::check_program(prog);
  Compiled out{.symbols = std::move(tables.symbols), .types = std::move(tables.types)};
  out.tac = c6502::c99_to_tac::translate_program(prog, out.symbols, &out.types);
  return out;
}

// Returns whether a function of `prog` has an instruction of the
// alternative `T`.
template <typename T>
bool has(const ir::Program& prog) {
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<ir::Function>(&tl)) {
      for (const auto& ins : fn->instructions) {
        if (std::holds_alternative<T>(ins)) {
          return true;
        }
      }
    }
  }
  return false;
}

// Checks that the optimized TAC of `source` has an instruction `T` and
// that `main` returns on it what it returns on the unoptimized TAC.
template <typename T>
void check_optimized(const std::string& source) {
  Compiled c = compile_to_tac(source);
  std::optional<Int> expected;
  {
    Simulator sim(c.tac, c.symbols, &c.types);
    expected = sim.call("main", {});
  }
  const ir::Program opt = passes::optimize_program(c.tac, &c.symbols);
  CHECK(has<T>(opt));
  Simulator sim(opt, c.symbols, &c.types);
  CHECK(sim.call("main", {}) == expected);
}

}  // namespace

TEST_CASE("tac_sim_optimized: the optimizer's instructions") {
  SUBCASE("JumpIfCmp") {
    check_optimized<ir::JumpIfCmp>(
        "int main(void) {\n"
        "  int s = 0;\n"
        "  for (unsigned char i = 0; i < 10; i++) s += i;\n"
        "  return s;\n"
        "}\n");
  }
  SUBCASE("JumpIfMasked") {
    check_optimized<ir::JumpIfMasked>(
        "unsigned char c = 0x93;\n"
        "int main(void) {\n"
        "  int r = 0;\n"
        "  if (c & 0x80) r += 1;\n"
        "  if (c & 0x04) r += 2;\n"
        "  if (c & 0x10) r += 4;\n"
        "  return r;\n"
        "}\n");
  }
  SUBCASE("IndexedStore") {
    check_optimized<ir::IndexedStore>(
        "static unsigned char * const buf = (unsigned char * const)0x2000;\n"
        "unsigned char col = 3;\n"
        "int main(void) {\n"
        "  buf[100 + col] = 42;\n"
        "  buf[101 + col] = 7;\n"
        "  return buf[100 + col] * 10 + buf[101 + col];\n"
        "}\n");
  }
  SUBCASE("IndexedConstLoad") {
    check_optimized<ir::IndexedConstLoad>(
        "static unsigned char * const buf = (unsigned char * const)0x2000;\n"
        "unsigned char col = 3;\n"
        "int main(void) {\n"
        "  buf[100 + col] = 42;\n"
        "  return buf[100 + col];\n"
        "}\n");
  }
  SUBCASE("IndirectIndexedLoad") {
    check_optimized<ir::IndirectIndexedLoad>(
        "unsigned char data[8] = {5, 6, 7, 8, 9, 10, 11, 12};\n"
        "int sum(unsigned char *p, unsigned char n) {\n"
        "  int s = 0;\n"
        "  for (unsigned char i = 0; i < n; i++) s += p[i];\n"
        "  return s;\n"
        "}\n"
        "int main(void) { return sum(data, 8); }\n");
  }
  SUBCASE("IndirectIndexedStore") {
    check_optimized<ir::IndirectIndexedStore>(
        "unsigned char data[8];\n"
        "void fill(unsigned char *p, unsigned char n) {\n"
        "  for (unsigned char i = 0; i < n; i++) p[i] = i * 3;\n"
        "}\n"
        "int main(void) { fill(data, 8); return data[7] + data[2]; }\n");
  }
}

TEST_CASE("tac_sim_optimized: the SSA form") {
  // A loop's SSA form has Phis at its header, and the interpreter runs
  // it to the result of the TAC before SSA.
  Compiled c = compile_to_tac(
      "int main(void) {\n"
      "  int a = 1, b = 0;\n"
      "  for (int i = 0; i < 6; i++) { int t = a; a = a + b; b = t; }\n"
      "  return a * 100 + b;\n"
      "}\n");
  std::optional<Int> expected;
  {
    Simulator sim(c.tac, c.symbols, &c.types);
    expected = sim.call("main", {});
  }
  REQUIRE(expected.has_value());
  CHECK(*expected == Int(1308));
  auto main_fn = std::get<ir::Function>(c.tac.top_level[0]);
  auto [ssa_fn, dsts] = passes::to_ssa(std::move(main_fn), c.symbols);
  ir::Program ssa;
  ssa.top_level.emplace_back(ssa_fn);
  CHECK(has<ir::Phi>(ssa));
  Simulator sim(ssa, c.symbols, &c.types);
  CHECK(sim.call("main", {}) == expected);
}
