// The unit tests of tests/test_constant_store_order.py, ported. The
// simulator's runs are in test_constant_store_order_sim.cpp.
#include <string>
#include <vector>

#include "asm_text.h"
#include "doctest.h"
#include "passes/constant_store_order.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string join(const std::vector<std::string>& parts) {
  std::string out;
  for (const auto& p : parts) {
    out += p;
    out += ' ';
  }
  return out;
}

// `Mov(#value, name)`, which `asm_emit` writes as `LDA; STA`.
std::string konst(int value, const std::string& name, bool is_volatile = false) {
  return "Mov(Imm(" + std::to_string(value) + ") Data(\"" + name + "\" 0) " +
         (is_volatile ? "true" : "false") + ")";
}

std::string load(int value) { return "Mov(Imm(" + std::to_string(value) + ") Reg(A) false)"; }

std::string store(const std::string& name, int offset = 0) {
  return "Mov(Reg(A) Data(\"" + name + "\" " + std::to_string(offset) + ") false)";
}

std::string mem_load(const std::string& src, bool is_volatile = false) {
  return "Mov(" + src + " Reg(A) " + (is_volatile ? "true" : "false") + ")";
}

std::string data(const std::string& name) { return "Data(\"" + name + "\" 0)"; }

std::string indexed(const std::string& name, const char* index = "X") {
  return "IndexedData(\"" + name + "\" 0 " + index + ")";
}

// Runs the pass on `instrs` followed by a return, and gives the result
// with the return taken off again.
std::string run(const std::vector<std::string>& instrs) {
  return instructions(c6502::passes::apply_constant_store_order(
      program(join(instrs) + "Return(false false false)")));
}

std::string want(const std::vector<std::string>& instrs) {
  return expect(join(instrs) + "Return(false false false)");
}

void check_kept(const std::vector<std::string>& instrs) { CHECK(run(instrs) == want(instrs)); }

const std::string kRows = indexed("ROWS");
const std::string kCols = indexed("COLS");

}  // namespace

TEST_CASE("constant_store_order groups stores of constants") {
  SUBCASE("a repeated value is loaded once") {
    // `csw = 0xC100; valid = 0;` stores 0, $C1, 0.
    CHECK(run({konst(0, "lo"), konst(0xC1, "hi"), konst(0, "valid")}) ==
          want({load(0xC1), store("hi"), load(0), store("lo"), store("valid")}));
  }
  SUBCASE("the last value stays last") {
    // A and N/Z hold 7 after the run, as before.
    CHECK(run({konst(7, "a"), konst(1, "b"), konst(7, "c"), konst(1, "d")}) ==
          want({load(7), store("a"), store("c"), load(1), store("b"), store("d")}));
  }
  SUBCASE("label halves count as constants") {
    const std::string lo = R"(Mov(ImmLabelLow("t" 0) Data("p" 0) false))";
    const std::string hi = R"(Mov(ImmLabelHigh("t" 0) Data("p" 1) false))";
    const std::string lo2 = R"(Mov(ImmLabelLow("t" 0) Data("q" 0) false))";
    CHECK(run({lo, hi, lo2}) ==
          want({R"(Mov(ImmLabelHigh("t" 0) Reg(A) false))", store("p", 1),
                R"(Mov(ImmLabelLow("t" 0) Reg(A) false))", store("p"), store("q")}));
  }
}

TEST_CASE("constant_store_order leaves a run of constants alone") {
  SUBCASE("no value repeats") { check_kept({konst(1, "a"), konst(2, "b")}); }
  SUBCASE("the same byte twice") {
    // The second store of `a` has to come after the first.
    check_kept({konst(0, "a"), konst(1, "a"), konst(0, "b")});
  }
  SUBCASE("a volatile store ends the run") {
    check_kept({konst(0, "a"), konst(1, "io", true), konst(0, "b")});
  }
  SUBCASE("the slots of two functions") {
    // The linker may give these two slots one byte.
    check_kept({konst(0, "__zpabi_g__p"), konst(1, "__local_h__t"), konst(0, "x")});
  }
}

TEST_CASE("constant_store_order groups values loaded from memory") {
  // Runs like Lode Runner's `fill_hole`.
  SUBCASE("indexed values are loaded once") {
    CHECK(run({mem_load(kRows), store("row"), mem_load(kCols), store("col"), mem_load(kRows),
               store("GAME_ROWNUM"), mem_load(kCols), store("GAME_COLNUM")}) ==
          want({mem_load(kRows), store("row"), store("GAME_ROWNUM"), mem_load(kCols),
                store("col"), store("GAME_COLNUM")}));
  }
  SUBCASE("a Data value is loaded once") {
    const std::string g = data("g");
    CHECK(run({mem_load(g), store("a"), load(1), store("b"), mem_load(g), store("c"), load(1),
               store("d")}) ==
          want({mem_load(g), store("a"), store("c"), load(1), store("b"), store("d")}));
  }
  SUBCASE("a load of a byte the run stored goes with its value") {
    // After `STA g`, `LDA g` reads the 1 just stored there, so `STA c`
    // joins the stores of 1. The first `LDA g` reads `g` before the run
    // writes it, and stays first.
    const std::string g = data("g");
    CHECK(run({mem_load(g), store("a"), load(1), store("g"), mem_load(g), store("c")}) ==
          want({mem_load(g), store("a"), load(1), store("g"), store("c")}));
  }
  SUBCASE("the first load of a byte the run stores stays before the store") {
    // The run from the first `LDA g` stores into `g`, which that load
    // reads, so it's left alone. The run from `LDA #1` reloads the 1 it
    // stored into `g`, so `STA c` joins the stores of 1, and the first
    // load of `g` still reads it before `STA g`.
    const std::string g = data("g");
    CHECK(run({mem_load(g), store("a"), load(1), store("b"), load(1), store("g"), mem_load(g),
               store("c")}) ==
          want({mem_load(g), store("a"), load(1), store("b"), store("g"), store("c")}));
  }
}

TEST_CASE("constant_store_order leaves a run of loaded values alone") {
  SUBCASE("a volatile load") {
    // A volatile byte may change between the two reads.
    check_kept({mem_load(data("io"), true), store("a"), load(1), store("b"),
                mem_load(data("io"), true), store("c")});
  }
  SUBCASE("a store into the object an indexed load reads") {
    // `STA ROWS+3` may write the byte `ROWS,X` reads.
    check_kept({mem_load(kRows), store("a"), load(1), store("ROWS", 3), mem_load(kRows),
                store("c")});
  }
  SUBCASE("a change of the index register") {
    // After the `LDX`, `ROWS,X` names another byte.
    check_kept({mem_load(kRows), store("a"), mem_load(kCols), store("b"),
                R"(Mov(Data("j" 0) Reg(X) false))", mem_load(kRows), store("c")});
  }
  SUBCASE("the other index register") {
    check_kept({mem_load(kRows), store("a"), mem_load(kCols), store("b"),
                mem_load(indexed("ROWS", "Y")), store("c")});
  }
  SUBCASE("a load from another function's slot") {
    // The linker may give `g`'s and `h`'s slots one byte, so the store to
    // `h`'s slot may change what `g`'s slot holds.
    const std::string g = data("__zpabi_g__p");
    check_kept({mem_load(g), store("a"), load(1), store("__zpabi_h__q"), mem_load(g),
                store("c")});
  }
  SUBCASE("an indexed load of a slot") {
    // Other slot names may cover the slot's bytes.
    const std::string slot = indexed("__local_f__buf", "Y");
    check_kept({mem_load(slot), store("a"), load(1), store("b"), mem_load(slot), store("c")});
  }
}
