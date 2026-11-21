// Cases of the walk of src/passes/optimization/extend_add_chain.h, run
// through the absolute,X recognizers, which only two calls of the
// corpus make fire. Each expected function is what the Python passes
// returned for the same input and symbols.
#include <string>

#include "asdl/runtime.h"
#include "doctest.h"
#include "passes/optimization/recognize_indexed_load.h"
#include "passes/optimization/recognize_indexed_store.h"

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using c6502::passes::LocalAttr;
using c6502::passes::NameSet;
using c6502::passes::Symbol;
using c6502::passes::SymbolTable;
using c6502::passes::framework::PassContext;

ir::Function fn_of(const std::string& body) {
  return std::get<ir::Function>(c6502::asdl::load<ir::Type_top_level>(
      "Function(\"f\" true [\"i\"] [\n" + body + "])\n"));
}

Symbol local(c99::Type_data_type t) { return Symbol{.type = std::move(t), .attrs = LocalAttr{}}; }

// The symbols of the cases: the index `i` of type `index`, the extends
// `e`, `e0`, `e2`, and `w`, the addresses `a` and `a2`, and the loaded
// `d` of type `dst`.
SymbolTable symbols(c99::Type_data_type index = c99::UChar{},
                    c99::Type_data_type dst = c99::UChar{}) {
  SymbolTable s;
  s.insert_or_assign("i", local(std::move(index)));
  for (const char* n : {"e", "e0", "e2", "w"}) {
    s.insert_or_assign(n, local(c99::UInt{}));
  }
  for (const char* n : {"a", "a2"}) {
    s.insert_or_assign(n, local(c99::Pointer{.referenced_type = c99::UChar{}}));
  }
  s.insert_or_assign("d", local(std::move(dst)));
  return s;
}

// The load of `base + ZeroExtend(i)` (or the extend `ext`) into `d`.
std::string load_body(const std::string& ext, const std::string& base) {
  return "  " + ext + "(Var(\"i\") Var(\"e\"))\n" + "  Binary(Add Constant(ConstUInt(" + base +
         ")) Var(\"e\") Var(\"a\"))\n" + "  Load(Var(\"a\") Var(\"d\") false)\n" +
         "  Ret(Var(\"d\"))\n";
}

std::string load(const std::string& body, SymbolTable syms, NameSet ssa) {
  const ir::Function out = c6502::passes::recognize_indexed_load(
      fn_of(body), PassContext{.symbols = &syms, .ssa_dsts = &ssa});
  return c6502::asdl::dump(ir::Type_top_level{out});
}

const NameSet kSsa{"i", "e", "e0", "e2", "w", "a", "a2", "d"};

}  // namespace

TEST_CASE("recognize_indexed_load: a byte load at C + ZeroExtend(i) is IndexedConstLoad") {
  CHECK(load(load_body("ZeroExtend", "8192"), symbols(), kSsa) ==
        "Function(\"f\" true [\"i\"] [\n"
        "  IndexedConstLoad(8192 Var(\"i\") Var(\"d\") false)\n"
        "  Ret(Var(\"d\"))\n"
        "])\n");
  // C + 255 must stay below $10000, so 0xFF00 is the largest base.
  CHECK(load(load_body("ZeroExtend", "65280"), symbols(), kSsa) ==
        "Function(\"f\" true [\"i\"] [\n"
        "  IndexedConstLoad(65280 Var(\"i\") Var(\"d\") false)\n"
        "  Ret(Var(\"d\"))\n"
        "])\n");
}

TEST_CASE("recognize_indexed_load: the gates leave the chain alone") {
  // A negative index of a SignExtend would address C + 255.
  const std::string sign = load_body("SignExtend", "8192");
  CHECK(load(sign, symbols(c99::SChar{}), kSsa) == c6502::asdl::dump(ir::Type_top_level{fn_of(sign)}));
  const std::string high = load_body("ZeroExtend", "65281");
  CHECK(load(high, symbols(), kSsa) == c6502::asdl::dump(ir::Type_top_level{fn_of(high)}));
  const std::string plain = load_body("ZeroExtend", "8192");
  const std::string unchanged = c6502::asdl::dump(ir::Type_top_level{fn_of(plain)});
  // A load of two bytes would need two reads.
  CHECK(load(plain, symbols(c99::UChar{}, c99::UInt{}), kSsa) == unchanged);
  // An index of two bytes doesn't fit X.
  CHECK(load(plain, symbols(c99::UInt{}), kSsa) == unchanged);
  // An address SSA didn't rename may have another definition.
  CHECK(load(plain, symbols(), NameSet{"i", "e", "d"}) == unchanged);
  // Outside SSA form the walk does nothing.
  SymbolTable syms = symbols();
  const ir::Function out =
      c6502::passes::recognize_indexed_load(fn_of(plain), PassContext{.symbols = &syms});
  CHECK(c6502::asdl::dump(ir::Type_top_level{out}) == unchanged);
}

TEST_CASE("recognize_indexed_load: an extend with another reader stays") {
  CHECK(load("  ZeroExtend(Var(\"i\") Var(\"e\"))\n"
             "  Binary(Add Constant(ConstUInt(8192)) Var(\"e\") Var(\"a\"))\n"
             "  Load(Var(\"a\") Var(\"d\") false)\n"
             "  Copy(Var(\"e\") Var(\"w\"))\n"
             "  Ret(Var(\"d\"))\n",
             symbols(), kSsa) ==
        "Function(\"f\" true [\"i\"] [\n"
        "  ZeroExtend(Var(\"i\") Var(\"e\"))\n"
        "  IndexedConstLoad(8192 Var(\"i\") Var(\"d\") false)\n"
        "  Copy(Var(\"e\") Var(\"w\"))\n"
        "  Ret(Var(\"d\"))\n"
        "])\n");
}

TEST_CASE("recognize_indexed_load: the walk follows a Copy of the extend") {
  // The Copy that defines the Add's operand goes with the Add, and the
  // extend stays for dead-store elimination. The volatile bit carries
  // over.
  CHECK(load("  ZeroExtend(Var(\"i\") Var(\"e0\"))\n"
             "  Copy(Var(\"e0\") Var(\"e\"))\n"
             "  Binary(Add Var(\"e\") Constant(ConstUInt(8192)) Var(\"a\"))\n"
             "  Load(Var(\"a\") Var(\"d\") true)\n"
             "  Ret(Var(\"d\"))\n",
             symbols(), kSsa) ==
        "Function(\"f\" true [\"i\"] [\n"
        "  ZeroExtend(Var(\"i\") Var(\"e0\"))\n"
        "  IndexedConstLoad(8192 Var(\"i\") Var(\"d\") true)\n"
        "  Ret(Var(\"d\"))\n"
        "])\n");
}

TEST_CASE("recognize_indexed_store: only a 1-byte value is stored through absolute,X") {
  SymbolTable syms = symbols();
  NameSet ssa = kSsa;
  const ir::Function out = c6502::passes::recognize_indexed_store(
      fn_of("  ZeroExtend(Var(\"i\") Var(\"e\"))\n"
            "  Binary(Add Constant(ConstUInt(8192)) Var(\"e\") Var(\"a\"))\n"
            "  Store(Constant(ConstUChar(7)) Var(\"a\") false)\n"
            "  ZeroExtend(Var(\"i\") Var(\"e2\"))\n"
            "  Binary(Add Constant(ConstUInt(8192)) Var(\"e2\") Var(\"a2\"))\n"
            "  Store(Constant(ConstInt(7)) Var(\"a2\") false)\n"
            "  Ret(Var(\"d\"))\n"),
      PassContext{.symbols = &syms, .ssa_dsts = &ssa});
  CHECK(c6502::asdl::dump(ir::Type_top_level{out}) ==
        "Function(\"f\" true [\"i\"] [\n"
        "  IndexedStore(8192 Var(\"i\") Constant(ConstUChar(7)) false)\n"
        "  ZeroExtend(Var(\"i\") Var(\"e2\"))\n"
        "  Binary(Add Constant(ConstUInt(8192)) Var(\"e2\") Var(\"a2\"))\n"
        "  Store(Constant(ConstInt(7)) Var(\"a2\") false)\n"
        "  Ret(Var(\"d\"))\n"
        "])\n");
}
