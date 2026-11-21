// The unit tests of tests/test_truncate_extend_fold.py, ported: the
// folds of a Truncate of an extend and of an extend of an extend, and
// the gates that leave a name alone when SSA didn't rename it.
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "doctest.h"
#include "passes/optimization/truncate_extend_fold.h"

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using c6502::passes::LocalAttr;
using c6502::passes::NameSet;
using c6502::passes::NoInitializer;
using c6502::passes::StaticAttr;
using c6502::passes::Symbol;
using c6502::passes::SymbolTable;
using c6502::passes::framework::PassContext;

ir::Function fn_of(const std::string& body) {
  return std::get<ir::Function>(c6502::asdl::load<ir::Type_top_level>(
      "Function(\"f\" true [] [\n" + body + "])\n"));
}

Symbol local(c99::Type_data_type t) { return Symbol{.type = std::move(t), .attrs = LocalAttr{}}; }

Symbol global(c99::Type_data_type t) {
  return Symbol{.type = std::move(t),
                .attrs = StaticAttr{.initial_value = NoInitializer{}, .is_global = true}};
}

// Returns the dump of instruction `k` of `fn`.
std::string at(const ir::Function& fn, std::size_t k) {
  return c6502::asdl::dump(fn.instructions.at(k));
}

ir::Function truncate_extend(const std::string& body, SymbolTable symbols, NameSet* ssa) {
  return c6502::passes::fold_truncate_extend(fn_of(body),
                                             PassContext{.symbols = &symbols, .ssa_dsts = ssa});
}

}  // namespace

TEST_CASE("fold_truncate_extend: a SignExtend and a Truncate of the same width become a Copy") {
  NameSet ssa{"i", "t", "u"};
  const auto out = truncate_extend(
      "  SignExtend(Var(\"i\") Var(\"t\"))\n  Truncate(Var(\"t\") Var(\"u\"))\n",
      {{"i", local(c99::SChar{})}, {"t", local(c99::Int{})}, {"u", local(c99::SChar{})}}, &ssa);
  CHECK(at(out, 1) == "Copy(Var(\"i\") Var(\"u\"))\n");
  // The SignExtend stays, for SSA dead-code elimination to drop.
  CHECK(at(out, 0) == "SignExtend(Var(\"i\") Var(\"t\"))\n");
}

TEST_CASE("fold_truncate_extend: a ZeroExtend and a Truncate of the same width become a Copy") {
  NameSet ssa{"i", "t", "u"};
  const auto out = truncate_extend(
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n  Truncate(Var(\"t\") Var(\"u\"))\n",
      {{"i", local(c99::UChar{})}, {"t", local(c99::UInt{})}, {"u", local(c99::UChar{})}}, &ssa);
  CHECK(at(out, 1) == "Copy(Var(\"i\") Var(\"u\"))\n");
}

TEST_CASE("fold_truncate_extend: a Truncate wider than the source is a narrower extend") {
  NameSet ssa{"i", "t", "u"};
  auto out = truncate_extend(
      "  SignExtend(Var(\"i\") Var(\"t\"))\n  Truncate(Var(\"t\") Var(\"u\"))\n",
      {{"i", local(c99::SChar{})}, {"t", local(c99::Long{})}, {"u", local(c99::Int{})}}, &ssa);
  CHECK(at(out, 1) == "SignExtend(Var(\"i\") Var(\"u\"))\n");
  out = truncate_extend(
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n  Truncate(Var(\"t\") Var(\"u\"))\n",
      {{"i", local(c99::UChar{})}, {"t", local(c99::ULong{})}, {"u", local(c99::UInt{})}}, &ssa);
  CHECK(at(out, 1) == "ZeroExtend(Var(\"i\") Var(\"u\"))\n");
}

TEST_CASE("fold_truncate_extend: a Truncate of a Truncate is one Truncate") {
  NameSet ssa{"x", "t", "u"};
  const auto out = truncate_extend(
      "  Truncate(Var(\"x\") Var(\"t\"))\n  Truncate(Var(\"t\") Var(\"u\"))\n",
      {{"x", local(c99::Long{})}, {"t", local(c99::Int{})}, {"u", local(c99::SChar{})}}, &ssa);
  CHECK(at(out, 1) == "Truncate(Var(\"x\") Var(\"u\"))\n");
}

TEST_CASE("fold_truncate_extend: a source SSA didn't rename is left alone") {
  // `i` may have changed between the extend and the Truncate.
  NameSet ssa{"t", "u"};
  auto out = truncate_extend(
      "  SignExtend(Var(\"i\") Var(\"t\"))\n  Truncate(Var(\"t\") Var(\"u\"))\n",
      {{"i", local(c99::SChar{})}, {"t", local(c99::Long{})}, {"u", local(c99::Int{})}}, &ssa);
  CHECK(at(out, 1) == "Truncate(Var(\"t\") Var(\"u\"))\n");
  // `long l2 = (int)g; g += 1; f((signed char)l2);` with a global `g`
  // would read the incremented `g` at the Truncate.
  out = truncate_extend(
      "  SignExtend(Var(\"g\") Var(\"t\"))\n"
      "  Binary(Add Var(\"g\") Constant(ConstChar(1)) Var(\"g\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n",
      {{"g", global(c99::SChar{})}, {"t", local(c99::Long{})}, {"u", local(c99::SChar{})}},
      &ssa);
  CHECK(at(out, 2) == "Truncate(Var(\"t\") Var(\"u\"))\n");
}

TEST_CASE("fold_truncate_extend: a global the extend writes isn't walked back through") {
  NameSet ssa{"t", "u"};
  const auto out = truncate_extend(
      "  ZeroExtend(Var(\"t\") Var(\"ui\"))\n  Truncate(Var(\"ui\") Var(\"u\"))\n",
      {{"t", local(c99::UChar{})}, {"ui", global(c99::UInt{})}, {"u", local(c99::UChar{})}},
      &ssa);
  CHECK(at(out, 1) == "Truncate(Var(\"ui\") Var(\"u\"))\n");
}

TEST_CASE("fold_truncate_extend: outside SSA form the function is unchanged") {
  const std::string body = "  SignExtend(Var(\"i\") Var(\"t\"))\n  Truncate(Var(\"t\") Var(\"u\"))\n";
  const auto out = truncate_extend(
      body, {{"i", local(c99::SChar{})}, {"t", local(c99::Int{})}, {"u", local(c99::SChar{})}},
      nullptr);
  CHECK(out == fn_of(body));
}

namespace {

ir::Function extend_extend(const std::string& inner, const std::string& outer, NameSet ssa) {
  SymbolTable symbols{
      {"i", local(c99::UChar{})}, {"t", local(c99::Int{})}, {"u", local(c99::ULong{})}};
  return c6502::passes::fold_extend_extend(
      fn_of("  " + inner + "(Var(\"i\") Var(\"t\"))\n  " + outer + "(Var(\"t\") Var(\"u\"))\n"),
      PassContext{.symbols = &symbols, .ssa_dsts = &ssa});
}

}  // namespace

TEST_CASE("fold_extend_extend: an extend of a widening extend is one extend") {
  // `(unsigned long)i` for a `uint8_t i`: the promotion to `int` leaves
  // the top bit 0, so the sign extension adds zeros.
  CHECK(at(extend_extend("ZeroExtend", "SignExtend", {"i", "t", "u"}), 1) ==
        "ZeroExtend(Var(\"i\") Var(\"u\"))\n");
  CHECK(at(extend_extend("ZeroExtend", "ZeroExtend", {"i", "t", "u"}), 1) ==
        "ZeroExtend(Var(\"i\") Var(\"u\"))\n");
  CHECK(at(extend_extend("SignExtend", "SignExtend", {"i", "t", "u"}), 1) ==
        "SignExtend(Var(\"i\") Var(\"u\"))\n");
}

TEST_CASE("fold_extend_extend: a zero extend of a sign extend stays") {
  // The middle byte copies i's sign and the top bytes are 0.
  CHECK(at(extend_extend("SignExtend", "ZeroExtend", {"i", "t", "u"}), 1) ==
        "ZeroExtend(Var(\"t\") Var(\"u\"))\n");
}

TEST_CASE("fold_extend_extend: a source SSA didn't rename is left alone") {
  CHECK(at(extend_extend("ZeroExtend", "SignExtend", {"t", "u"}), 1) ==
        "SignExtend(Var(\"t\") Var(\"u\"))\n");
}
