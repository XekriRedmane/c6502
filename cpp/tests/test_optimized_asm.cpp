// Tests of the back end's region (src/passes/optimized_asm.*). The end
// to end comparison with the Python compiler, tools/compare_backends.py,
// checks the order of the passes and their arguments on the corpus.
// These check what the region reports besides the program.
#include <string>
#include <vector>

#include "asm_text.h"
#include "doctest.h"
#include "passes/optimized_asm.h"

namespace {

namespace ir = c6502::asm_ast;
using namespace c6502::passes;
using c6502::asm_text::dump;
using c6502::asm_text::top_level;

}  // namespace

TEST_CASE("the region reports the functions shared_tails makes") {
  // Both functions end by storing the store's two parameter bytes and
  // jumping to it, which `apply_shared_tails` moves into `g@tail0`.
  const std::string tail = R"(
    Mov(Reg(A) Data("__zpabi_store__v_0" 0) false) Mov(Imm(0) Reg(A) false)
    Mov(Reg(A) Data("__zpabi_store__v_1" 0) false) Jump("store" []))";
  const auto in = top_level(R"(Function("g" true [] [Mov(Data("x" 0) Reg(A) false) )" +
                            tail + R"(]) Function("h" true [] [Mov(Data("y" 0) Reg(A) false) )" +
                            tail + "])");
  RegionArgs args;
  args.slot_symbols = {{"__zpabi_store__v_0", 0x80}, {"__zpabi_store__v_1", 0x81}};
  args.zp_names = {"__zpabi_store__v_0", "__zpabi_store__v_1"};
  args.slot_addrs = {0x80, 0x81};
  const RegionResult out = optimized_region(in, AsmContext{}, args);
  CHECK(out.new_functions == std::vector<NewFunction>{{"g@tail0", "g"}});
  REQUIRE(out.prog.top_level.size() == 3);
  CHECK(std::get<ir::Function>(out.prog.top_level[1]).name == "g@tail0");
}

TEST_CASE("the region reports the slots slot_coalescing makes a function write") {
  // `m` keeps a copy of its parameter in a local across a call, which
  // `apply_slot_coalescing` renames to the parameter's slot.
  const auto in = top_level(R"(Function("m" true [] [
    Mov(Data("__zpabi_m__a_0" 0) Reg(A) false) Mov(Reg(A) Data("__local_m__a_0" 0) false)
    Call("q" [] []) Mov(Data("__local_m__a_0" 0) Reg(A) false)
    Mov(Reg(A) Data("out" 0) false) Return(false false false)]))");
  RegionArgs args;
  args.slot_symbols = {{"__zpabi_m__a_0", 0x80}, {"__local_m__a_0", 0x81}};
  args.abi = {{"m", ZpLayout{.slot_symbols = {"__zpabi_m__a_0"}}}};
  const RegionResult out = optimized_region(in, AsmContext{}, args);
  const WrittenSlots want = {{"m", {"__zpabi_m__a_0"}}};
  INFO(dump(out.prog));
  CHECK(out.coalesced_params == want);
  CHECK(dump(out.prog).find("__local_m__a_0") == std::string::npos);
}
