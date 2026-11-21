// Tests of the shared helpers of the TAC optimizer's passes: the
// framework's run loops, `rewrite_uses_in`, and `value_range`, on the
// cases the corpus check (tools/capture_helpers.py --tac-only) doesn't
// reach or that the C++ port handles differently from Python.
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "doctest.h"
#include "passes/optimization/framework.h"
#include "passes/optimization/value_range.h"
#include "passes/optimization/var_visit.h"

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using namespace ::c6502::passes;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::PassContext;

ir::Function fn_of(const std::string& body) {
  return std::get<ir::Function>(c6502::asdl::load<ir::Type_top_level>(
      "Function(\"f\" true [] [\n" + body + "])\n"));
}

ir::Type_instruction instr_of(const std::string& text) {
  return c6502::asdl::load<ir::Type_instruction>(text);
}

Symbol local(c99::Type_data_type t) { return Symbol{.type = std::move(t), .attrs = LocalAttr{}}; }

}  // namespace

TEST_CASE("run_def_use: a Rewrite that drops a definition dropped before is skipped") {
  // Both copies of `a` would drop its definition. The second Rewrite
  // finds it dropped already and is skipped, as Python's DefUsePass
  // skips it.
  NameSet ssa{"a", "b", "c"};
  const PassContext ctx{.ssa_dsts = &ssa};
  const auto out = framework::run_def_use(
      fn_of("  Copy(Constant(ConstInt(1)) Var(\"a\"))\n"
            "  Copy(Var(\"a\") Var(\"b\"))\n"
            "  Copy(Var(\"a\") Var(\"c\"))\n"),
      ctx, [](const ir::Type_instruction& instr, const DefUseEnv& env) -> std::optional<DefUseRewrite> {
        const auto* copy = std::get_if<ir::Copy>(&instr);
        const auto* src = copy != nullptr ? std::get_if<ir::Var>(&copy->src) : nullptr;
        if (src == nullptr || env.def_of(*src) == nullptr) {
          return std::nullopt;
        }
        const auto& def = std::get<ir::Copy>(*env.def_of(*src));
        return DefUseRewrite::dropping(ir::Copy{.src = def.src, .dst = copy->dst}, {src->name});
      });
  CHECK(c6502::asdl::dump(ir::Type_top_level{out}) ==
        "Function(\"f\" true [] [\n"
        "  Copy(Constant(ConstInt(1)) Var(\"b\"))\n"
        "  Copy(Var(\"a\") Var(\"c\"))\n"
        "])\n");
}

TEST_CASE("run_def_use: a bare list hides its definition from later rewrites") {
  // Python keeps a bare list in the view, which no isinstance test
  // matches, so the rewrite of the second instruction sees no Copy.
  NameSet ssa{"a", "b"};
  const PassContext ctx{.ssa_dsts = &ssa};
  bool saw_definition = true;
  framework::run_def_use(
      fn_of("  Copy(Constant(ConstInt(1)) Var(\"a\"))\n  Copy(Var(\"a\") Var(\"b\"))\n"), ctx,
      [&](const ir::Type_instruction& instr, const DefUseEnv& env) -> std::optional<DefUseRewrite> {
        const auto& copy = std::get<ir::Copy>(instr);
        if (std::holds_alternative<ir::Constant>(copy.src)) {
          DefUseRewrite r = DefUseRewrite::one(instr);
          r.replacement.push_back(instr);
          r.is_list = true;
          return r;
        }
        saw_definition = env.def_of(std::get<ir::Var>(copy.src)) != nullptr;
        return std::nullopt;
      });
  CHECK_FALSE(saw_definition);
}

TEST_CASE("rewrite_uses_in: the mapper sees the operands in Python's order") {
  std::vector<std::string> seen;
  const ValMapper mapper = [&](const ir::Type_val& v) -> std::optional<ir::Type_val> {
    seen.push_back(std::get<ir::Var>(v).name);
    return std::nullopt;
  };
  CHECK_FALSE(rewrite_uses_in(
      instr_of("IndirectIndexedStore(Var(\"p\") Var(\"i\") Var(\"s\") false)\n"), mapper));
  CHECK_FALSE(rewrite_uses_in(instr_of("Store(Var(\"s2\") Var(\"p2\") false)\n"), mapper));
  CHECK_FALSE(rewrite_uses_in(
      instr_of("IndirectCall(Var(\"fp\") [Var(\"a\") Var(\"b\")] Var(\"d\"))\n"), mapper));
  CHECK(seen == std::vector<std::string>{"p", "i", "s", "s2", "p2", "fp", "a", "b"});
}

TEST_CASE("rewrite_uses_in: the fields that aren't operands read stay") {
  const ValMapper rename = [](const ir::Type_val& v) -> std::optional<ir::Type_val> {
    if (const auto* var = std::get_if<ir::Var>(&v)) {
      return ir::Var{.name = var->name + "'"};
    }
    return std::nullopt;
  };
  CHECK_FALSE(rewrite_uses_in(instr_of("GetAddress(Var(\"x\") Var(\"p\"))\n"), rename));
  const auto load = rewrite_uses_in(
      instr_of("IndexedLoad(\"t\" Var(\"i\") Var(\"d\") false 3 4)\n"), rename);
  REQUIRE(load);
  CHECK(c6502::asdl::dump(*load) == "IndexedLoad(\"t\" Var(\"i'\") Var(\"d\") false 3 4)\n");
}

TEST_CASE("max_value: a product or a shift past 128 bits gives no bound") {
  // Python multiplies two bounds of 2^64 - 1 and finds the product too
  // big for the destination. The C++ port tests that without forming
  // the product, which an asdl::Int can't hold.
  SymbolTable symbols{{"a", local(c99::ULongLong{})},
                      {"b", local(c99::ULongLong{})},
                      {"p", local(c99::ULongLong{})},
                      {"s", local(c99::ULongLong{})},
                      {"c", local(c99::UChar{})},
                      {"z", local(c99::UInt{})},
                      {"q", local(c99::UInt{})}};
  NameSet ssa{"p", "s", "z", "q"};
  const auto fn = fn_of(
      "  Binary(Multiply Var(\"a\") Var(\"b\") Var(\"p\"))\n"
      "  Binary(LeftShift Var(\"a\") Constant(ConstInt(63)) Var(\"s\"))\n"
      "  ZeroExtend(Var(\"c\") Var(\"z\"))\n"
      "  Binary(Multiply Var(\"z\") Var(\"z\") Var(\"q\"))\n");
  const auto def_idx = framework::build_def_idx(fn.instructions);
  const value_range::DefOf def_of = [&](const ir::Var& v) -> const ir::Type_instruction* {
    const auto it = def_idx.find(v.name);
    return it == def_idx.end() ? nullptr : &fn.instructions[it->second];
  };
  const auto bound = [&](const char* name) {
    return value_range::max_value(ir::Var{.name = name}, def_of, &symbols, &ssa);
  };
  // `p` and `s` keep their type's bound, 2^64 - 1.
  const c6502::asdl::Int top = (c6502::asdl::Int(1) << 64) - 1;
  CHECK(bound("p") == top);
  CHECK(bound("s") == top);
  CHECK(bound("q") == c6502::asdl::Int(255 * 255));
}
