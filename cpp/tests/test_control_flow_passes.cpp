// Tests of the control-flow and whole-program passes: the branch and
// jump peepholes, tail and run sharing, jump tables, long branches,
// short jumps, slot coalescing, and copies through an index register.
// The cases come from the Python tests and docstrings of each pass, and
// each expected program is what the Python pass returns for the input.
// The passes are checked in full on the corpus by `check-captures`.
#include <string>
#include <vector>

#include "asm_ast.h"
#include "asm_text.h"
#include "doctest.h"
#include "errors.h"
#include "passes/asm_context.h"
#include "passes/branch_invert.h"
#include "passes/branch_thread.h"
#include "passes/branch_through_jump.h"
#include "passes/branch_to_next_drop.h"
#include "passes/branch_value_thread.h"
#include "passes/copy_through_index.h"
#include "passes/cross_jump.h"
#include "passes/dead_label_drop.h"
#include "passes/jump_table.h"
#include "passes/long_branches.h"
#include "passes/outline_runs.h"
#include "passes/registry_control_flow.h"
#include "passes/ret_slot_coalescing.h"
#include "passes/shared_tails.h"
#include "passes/short_jumps.h"
#include "passes/slot_coalescing.h"
#include "passes/tail_call.h"
#include "passes/tail_merge.h"
#include "passes/unreachable_drop.h"

namespace ir = c6502::asm_ast;
using namespace c6502::passes;
using asm_text::dump;
using asm_text::function;
using asm_text::program;

namespace {

const c6502::passes::AsmContext kCtx;

}  // namespace

TEST_CASE("dead_label_drop keeps only the labels something names") {
  const auto in = function(R"(
    Label(".dead") Branch(EQ ".b") Label(".b") Jump(".j" []) Label(".j")
    Phi(Data("x" 0) [AsmPhiArg(".p" Imm(0))]) Label(".p")
    Return(false false false))");
  const auto want = function(R"(
    Branch(EQ ".b") Label(".b") Jump(".j" []) Label(".j")
    Phi(Data("x" 0) [AsmPhiArg(".p" Imm(0))]) Label(".p")
    Return(false false false))");
  CHECK(dump(apply_dead_label_drop(in)) == dump(want));
}

TEST_CASE("unreachable_drop removes the code after a terminator") {
  // Dropping `JMP .b`'s block leaves `.x` with no jump to it, and the
  // pass repeats until the chain is gone.
  const auto in = function(R"(
    Jump(".a" []) Mov(Imm(1) Reg(A) false) Label(".x") Mov(Imm(2) Reg(A) false)
    Label(".a") Return(false false false) Jump(".b" []) Label(".b")
    Return(false false false))");
  const auto want = function(R"(
    Jump(".a" []) Label(".a") Return(false false false))");
  CHECK(dump(apply_unreachable_drop(in)) == dump(want));
}

TEST_CASE("branch_invert inverts a branch around a jump") {
  const char* conds[][2] = {{"EQ", "NE"}, {"NE", "EQ"}, {"CC", "CS"}, {"CS", "CC"},
                            {"MI", "PL"}, {"PL", "MI"}, {"VC", "VS"}, {"VS", "VC"}};
  for (const auto& c : conds) {
    const auto in = function(std::string("Branch(") + c[0] +
                             R"( ".L") Jump(".T" []) Label(".L") Label(".T"))");
    const auto want =
        function(std::string("Branch(") + c[1] + R"( ".T") Label(".L") Label(".T"))");
    CHECK(dump(apply_branch_invert(in)) == dump(want));
  }
  SUBCASE("a tail call stays a jump") {
    const auto in = function(R"(Branch(NE ".L") Jump("other" []) Label(".L"))");
    CHECK(dump(apply_branch_invert(in)) == dump(in));
  }
}

TEST_CASE("branch_to_next_drop drops branches and jumps to the next label") {
  SUBCASE("the label goes with its only branch") {
    const auto in = function(R"(
      Mov(Data("x" 1) Reg(A) false) Branch(PL ".d") Label(".d")
      Return(false false false))");
    const auto want = function(R"(
      Mov(Data("x" 1) Reg(A) false) Return(false false false))");
    CHECK(dump(apply_branch_to_next_drop(in)) == dump(want));
  }
  SUBCASE("a label something else names stays") {
    const auto in = function(R"(
      Jump(".d" []) Branch(PL ".d") Label(".d") Return(false false false))");
    const auto want = function(R"(
      Jump(".d" []) Label(".d") Return(false false false))");
    CHECK(dump(apply_branch_to_next_drop(in)) == dump(want));
  }
  SUBCASE("a second branch on the same condition is never taken") {
    const auto in = function(R"(
      Branch(NE ".a") Branch(NE ".b") Return(false false false)
      Label(".a") Label(".b") Return(false false false))");
    const auto want = function(R"(
      Branch(NE ".a") Return(false false false)
      Label(".a") Label(".b") Return(false false false))");
    CHECK(dump(apply_branch_to_next_drop(in)) == dump(want));
  }
  SUBCASE("a jump past a run of labels") {
    const auto in = function(R"(
      Jump(".b" []) Label(".a") Label(".b") Return(false false false))");
    const auto want = function(R"(
      Label(".a") Label(".b") Return(false false false))");
    CHECK(dump(apply_branch_to_next_drop(in)) == dump(want));
  }
}

TEST_CASE("branch_through_jump retargets through a trampoline") {
  SUBCASE("the trampoline goes when nothing falls into it") {
    const auto in = function(R"(
      Label(".top") Inc(Reg(X)) Branch(NE ".t") Return(false false false)
      Label(".t") Jump(".top" []))");
    const auto want = function(R"(
      Label(".top") Inc(Reg(X)) Branch(NE ".top") Return(false false false))");
    CHECK(dump(apply_branch_through_jump(in)) == dump(want));
  }
  SUBCASE("a cycle is left alone") {
    const auto in = function(R"(
      Branch(EQ ".a") Return(false false false)
      Label(".a") Jump(".b" []) Label(".b") Jump(".a" []))");
    CHECK(dump(apply_branch_through_jump(in)) == dump(in));
  }
}

TEST_CASE("branch_thread follows a branch on the same flag") {
  SUBCASE("the opposite condition goes past, with a new label") {
    const auto in = function(R"(
      Branch(CS ".c") Compare(Reg(X) Imm(1)) Dec(Reg(X)) Label(".c")
      Branch(CC ".out") Inc(Data("iob" 4)) Label(".out") Return(false false false))");
    const auto want = function(R"(
      Branch(CS ".thread@0") Compare(Reg(X) Imm(1)) Dec(Reg(X)) Label(".c")
      Branch(CC ".out") Label(".thread@0") Inc(Data("iob" 4)) Label(".out")
      Return(false false false))");
    CHECK(dump(apply_branch_thread(in)) == dump(want));
  }
  SUBCASE("the label number follows the largest in the program") {
    const auto in = function(R"(
      Label(".thread@7") Branch(EQ ".c") Return(false false false) Label(".c")
      Branch(NE ".x") Return(false false false) Label(".x") Return(false false false))");
    const auto out = apply_branch_thread(in);
    CHECK(dump(out).find(".thread@8") != std::string::npos);
  }
}

TEST_CASE("branch_value_thread tests a byte once") {
  const auto in = function(R"(
    Mov(Data("b" 1) Reg(A) false) Branch(NE ".L") Mov(Data("b" 0) Reg(A) false)
    Compare(Reg(A) Imm(2)) Branch(NE ".L") Return(false false false) Label(".L")
    Mov(Data("b" 1) Reg(A) false) Branch(NE ".M") Mov(Data("b" 0) Reg(A) false)
    Compare(Reg(A) Imm(4)) Branch(NE ".M") Mov(Imm(1) Reg(A) false)
    Return(true false false) Label(".M") Mov(Imm(0) Reg(A) false)
    Return(true false false))");
  const auto want = function(R"(
    Mov(Data("b" 1) Reg(A) false) Branch(NE ".M") Mov(Data("b" 0) Reg(A) false)
    Compare(Reg(A) Imm(2)) Branch(NE ".vthread@0") Return(false false false)
    Label(".L") Mov(Data("b" 1) Reg(A) false) Branch(NE ".M")
    Mov(Data("b" 0) Reg(A) false) Label(".vthread@0") Compare(Reg(A) Imm(4))
    Branch(NE ".M") Mov(Imm(1) Reg(A) false) Return(true false false) Label(".M")
    Mov(Imm(0) Reg(A) false) Return(true false false))");
  CHECK(dump(apply_branch_value_thread(in, kCtx)) == dump(want));
  SUBCASE("a store forgets what the branch decided") {
    const auto in2 = function(R"(
      Mov(Data("b" 1) Reg(A) false) Branch(NE ".L") Mov(Reg(A) Data("b" 1) false)
      Jump(".L" []) Label(".L") Mov(Data("b" 1) Reg(A) false) Branch(NE ".M")
      Return(false false false) Label(".M") Return(false false false))");
    const auto want2 = function(R"(
      Mov(Data("b" 1) Reg(A) false) Branch(NE ".M") Mov(Reg(A) Data("b" 1) false)
      Jump(".L" []) Label(".L") Mov(Data("b" 1) Reg(A) false) Branch(NE ".M")
      Return(false false false) Label(".M") Return(false false false))");
    CHECK(dump(apply_branch_value_thread(in2, kCtx)) == dump(want2));
  }
}

TEST_CASE("tail_call makes a call and a return a jump") {
  const auto in = function(R"(
    Call("g" ["A"] []) Return(true false false) Jump(".r" []) Label(".r")
    Return(false false false) Call("h" [] []) Ret(0 2 false false [] false))");
  const auto want = function(R"(
    Jump("g" ["A"]) Return(false false false) Label(".r")
    Return(false false false) Call("h" [] []) Ret(0 2 false false [] false))");
  CHECK(dump(apply_tail_call(in)) == dump(want));
}

TEST_CASE("tail_merge branches to one copy of a return tail") {
  const auto in = function(R"(
    Compare(Reg(A) Imm(1)) Branch(CS ".L") Mov(Imm(0) Reg(X) false)
    Mov(Reg(X) Reg(A) false) Return(true true false) Label(".L")
    Compare(Reg(A) Imm(2)) Branch(CS ".N") Mov(Imm(1) Reg(A) false)
    Return(true true false) Label(".N") Mov(Imm(0) Reg(X) false)
    Mov(Reg(X) Reg(A) false) Return(true true false))");
  const auto want = function(R"(
    Compare(Reg(A) Imm(1)) Branch(CC ".N") Label(".L") Compare(Reg(A) Imm(2))
    Branch(CS ".N") Mov(Imm(1) Reg(A) false) Return(true true false) Label(".N")
    Mov(Imm(0) Reg(X) false) Mov(Reg(X) Reg(A) false) Return(true true false))");
  CHECK(dump(apply_tail_merge(in)) == dump(want));
}

TEST_CASE("cross_jump shares the instructions before two jumps") {
  const auto in = function(R"(
    Branch(NE ".x4") ArithmeticShiftLeft(Data("n" 0)) RotateLeft(Data("n" 1))
    Jump(".end" []) Label(".x4") ArithmeticShiftLeft(Data("n" 0))
    RotateLeft(Data("n" 1)) ArithmeticShiftLeft(Data("n" 0)) RotateLeft(Data("n" 1))
    Jump(".end" []) Label(".end") Return(false false false))");
  const auto want = function(R"(
    Branch(NE ".x4") Label(".xjump@0") ArithmeticShiftLeft(Data("n" 0))
    RotateLeft(Data("n" 1)) Jump(".end" []) Label(".x4")
    ArithmeticShiftLeft(Data("n" 0)) RotateLeft(Data("n" 1)) Jump(".xjump@0" [])
    Label(".end") Return(false false false))");
  CHECK(dump(apply_cross_jump(in)) == dump(want));
}

TEST_CASE("jump_tables dispatches a long compare chain through a table") {
  std::string body = R"(Mov(Data("op" 0) Reg(A) false) )";
  std::string cases;
  for (int k = 1; k <= 10; ++k) {
    const std::string n = std::to_string(k);
    body += "Compare(Reg(A) Imm(" + n + ")) Branch(EQ \".c" + n + "\") ";
    cases += "Label(\".c" + n + "\") Mov(Imm(" + n +
             R"() Reg(A) false) Mov(Reg(A) Data("out" 0) false) Return(false false false) )";
  }
  body += R"(Jump(".def" []) )" + cases + R"(Label(".def") Return(false false false))";
  const auto out = apply_jump_tables(function(body));
  const auto& instrs = std::get<ir::Function>(out.top_level[0]).instructions;
  // The bounds check takes the place of the chain, and the table starts
  // at 0, since the smallest constant is 2 or less.
  CHECK(instrs[1] == ir::Type_instruction{ir::Compare{
                         .left = ir::Reg{.reg = ir::A{}}, .right = ir::Imm{.value = 11}}});
  CHECK(instrs[2] == ir::Type_instruction{ir::Branch{.cond = ir::CS{}, .target = ".def"}});
  const auto* table = std::get_if<ir::JumpTable>(&instrs[3]);
  REQUIRE(table != nullptr);
  CHECK(table->table == ".jtab@0");
  CHECK(table->index == "Y");
  CHECK(table->base == 0);
  CHECK(table->targets == std::vector<std::string>{".def", ".c1", ".c2", ".c3",
                                                   ".c4", ".c5", ".c6", ".c7",
                                                   ".c8", ".c9", ".c10"});
  SUBCASE("a chain of six is shorter than its table") {
    std::string short_body = R"(Mov(Data("op" 0) Reg(A) false) )";
    for (int k = 1; k <= 6; ++k) {
      short_body += "Compare(Reg(A) Imm(" + std::to_string(k) + ")) Branch(EQ \".def\") ";
    }
    short_body += R"(Label(".def") Return(false false false))";
    const auto in = function(short_body);
    CHECK(dump(apply_jump_tables(in)) == dump(in));
  }
}

TEST_CASE("expand_long_branches expands a branch out of reach") {
  std::string body = R"(Branch(EQ ".far") )";
  for (int k = 0; k < 50; ++k) {
    body += R"(Mov(Imm(1) Data("x" 0) false) )";
  }
  body += R"(Label(".far") Return(false false false))";
  const auto out = expand_long_branches(function(body));
  const auto& instrs = std::get<ir::Function>(out.top_level[0]).instructions;
  CHECK(instrs[0] == ir::Type_instruction{ir::Branch{.cond = ir::NE{}, .target = ".lb_skip@0"}});
  CHECK(instrs[1] == ir::Type_instruction{ir::Jump{.target = ".far"}});
  CHECK(instrs[2] == ir::Type_instruction{ir::Label{.name = ".lb_skip@0"}});
  SUBCASE("a branch out of the function is an internal error") {
    CHECK_THROWS_AS(expand_long_branches(function(R"(Branch(EQ "other"))")),
                    c6502::InternalError);
  }
}

TEST_CASE("short_jumps goes straight to a tail call and shortens jumps") {
  const auto in = function(R"(
    Compare(Reg(A) Imm(16)) Branch(CC ".s") Jump(".default" []) Label(".s")
    JumpTable(".jtab@0" "Y" 0 [".a" ".default"]) Label(".a") Mov(Imm(0) Reg(A) false)
    Jump(".end" []) Mov(Imm(1) Reg(A) false) Label(".end") Return(false false false)
    Label(".default") Jump("internal_error" []))");
  const auto want = function(R"(
    Compare(Reg(A) Imm(16)) Branch(CC ".s") Jump("internal_error" []) Label(".s")
    JumpTable(".jtab@0" "Y" 0 [".a" "internal_error"]) Label(".a")
    Mov(Imm(0) Reg(A) false) Branch(EQ ".end") Mov(Imm(1) Reg(A) false) Label(".end")
    Return(false false false))");
  CHECK(dump(apply_short_jumps(in)) == dump(want));
}

TEST_CASE("shared_tails moves a common tail into a function of its own") {
  const std::string tail = R"(
    Mov(Reg(A) Data("__zpabi_store__v_0" 0) false) Mov(Imm(0) Reg(A) false)
    Mov(Reg(A) Data("__zpabi_store__v_1" 0) false) Jump("store" []))";
  const auto in = program(R"(Function("g" true [] [Mov(Data("x" 0) Reg(A) false) )" +
                          tail + R"(]) Function("h" true [] [Mov(Data("y" 0) Reg(A) false) )" +
                          tail + "])");
  const auto want = program(R"(
    Function("g" true [] [Mov(Data("x" 0) Reg(A) false) Jump("g@tail0" ["A" "X" "Y"])])
    Function("g@tail0" false [] [)" + tail + R"(])
    Function("h" true [] [Mov(Data("y" 0) Reg(A) false) Jump("g@tail0" ["A" "X" "Y"])]))");
  std::vector<NewFunction> made;
  CHECK(dump(apply_shared_tails(in, nullptr, {}, &made)) == dump(want));
  CHECK(made == std::vector<NewFunction>{{"g@tail0", "g"}});
}

TEST_CASE("outline_runs calls a run that two functions repeat") {
  const std::string setup = R"(
    Mov(Data("ops" 2) Reg(A) false) Mov(Reg(A) Data("__zpabi_cs__b_0" 0) false)
    Mov(Data("ops" 3) Reg(A) false) Mov(Reg(A) Data("__zpabi_cs__b_1" 0) false)
    Call("cs" [] []))";
  const auto in = program(R"(Function("g" true [] [)" + setup +
                          R"( Mov(Reg(A) Data("r" 0) false) Return(false false false)])
    Function("h" true [] [)" + setup +
                          R"( Mov(Reg(A) Data("s" 0) false) Return(false false false)]))");
  const auto want = program(R"(
    Function("g" true [] [Call("g@run0" [] []) Mov(Reg(A) Data("r" 0) false)
      Return(false false false)])
    Function("g@run0" false [] [
      Mov(Data("ops" 2) Reg(A) false) Mov(Reg(A) Data("__zpabi_cs__b_0" 0) false)
      Mov(Data("ops" 3) Reg(A) false) Mov(Reg(A) Data("__zpabi_cs__b_1" 0) false)
      Jump("cs" [])])
    Function("h" true [] [Call("g@run0" [] []) Mov(Reg(A) Data("s" 0) false)
      Return(false false false)]))");
  std::vector<NewFunction> made;
  CHECK(dump(apply_outline_runs(in, nullptr, {}, &made)) == dump(want));
  CHECK(made == std::vector<NewFunction>{{"g@run0", "g"}});
}

TEST_CASE("slot_coalescing gives a copy of a parameter the parameter's slot") {
  const auto in = function(R"(
    Mov(Data("__zpabi_m__a_0" 0) Reg(A) false) Mov(Reg(A) Data("__local_m__a_0" 0) false)
    ArithmeticShiftLeft(Data("__local_m__a_0" 0)) Mov(Data("__local_m__a_0" 0) Reg(A) false)
    Return(true false false))", "m");
  const auto want = function(R"(
    Mov(Data("__zpabi_m__a_0" 0) Reg(A) false) ArithmeticShiftLeft(Data("__zpabi_m__a_0" 0))
    Mov(Data("__zpabi_m__a_0" 0) Reg(A) false) Return(true false false))", "m");
  const AbiLayouts abi = {{"m", ZpLayout{.slot_symbols = {"__zpabi_m__a_0"}}}};
  const SymbolAddrs slots = {{"__zpabi_m__a_0", 0x80}, {"__local_m__a_0", 0x81}};
  auto [out, written] = apply_slot_coalescing(in, abi, {}, slots);
  CHECK(dump(out) == dump(want));
  CHECK(written == WrittenSlots{{"m", {"__zpabi_m__a_0"}}});
  SUBCASE("a slot in a share is left alone") {
    const std::vector<SlotShare> shares = {
        SlotShare{.members = {{"m", {"__zpabi_m__a_0"}}}}};
    auto [same, none] = apply_slot_coalescing(in, abi, shares, slots);
    CHECK(dump(same) == dump(in));
    CHECK(none.empty());
  }
}

TEST_CASE("ret_slot_coalescing keeps a copy of F@ret in F@ret") {
  const auto in = program(R"(
    StaticVariable("g@ret" false [ZeroInit(2)])
    Function("g" true [] [Mov(Imm(1) Reg(A) false) Mov(Reg(A) Data("g@ret" 0) false)
      Mov(Reg(A) Data("g@ret" 1) false) Return(false false false)])
    Function("f" true [] [Call("g" [] []) Mov(Data("g@ret" 0) Reg(A) false)
      Mov(Reg(A) Data("__local_f__p_0" 0) false) Mov(Data("g@ret" 1) Reg(A) false)
      Mov(Reg(A) Data("__local_f__p_1" 0) false) Inc(Data("__local_f__p_0" 0))
      Mov(Data("__local_f__p_0" 0) Reg(A) false) Mov(Data("__local_f__p_1" 0) Reg(X) false)
      Return(true true false)]))");
  const auto want = program(R"(
    StaticVariable("g@ret" false [ZeroInit(2)])
    Function("g" true [] [Mov(Imm(1) Reg(A) false) Mov(Reg(A) Data("g@ret" 0) false)
      Mov(Reg(A) Data("g@ret" 1) false) Return(false false false)])
    Function("f" true [] [Call("g" [] []) Mov(Data("g@ret" 0) Reg(A) false)
      Mov(Reg(A) Data("g@ret" 0) false) Mov(Data("g@ret" 1) Reg(A) false)
      Mov(Reg(A) Data("g@ret" 1) false) Inc(Data("g@ret" 0))
      Mov(Data("g@ret" 0) Reg(A) false) Mov(Data("g@ret" 1) Reg(X) false)
      Return(true true false)]))");
  const SymbolAddrs slots = {{"__local_f__p_0", 0x90}, {"__local_f__p_1", 0x91}};
  const SymbolAddrs zp = {{"g@ret", 0x40}};
  CHECK(dump(apply_ret_slot_coalescing(in, slots, zp, NameSet{"f"})) == dump(want));
  CHECK(dump(apply_ret_slot_coalescing(in, slots, zp, NameSet{})) == dump(in));
}

TEST_CASE("copy_through_index copies a byte through X while A is parked") {
  const auto in = function(R"(
    Mov(Reg(A) Data("__local_f__t" 0) false) Mov(Data("m" 0) Reg(A) false)
    Mov(Reg(A) Data("d" 0) false) Mov(Data("__local_f__t" 0) Reg(A) false)
    Compare(Reg(A) Imm(3)) Branch(EQ ".x") Return(false false false) Label(".x")
    Return(false false false))");
  const auto want = function(R"(
    Mov(Data("m" 0) Reg(X) false) Mov(Reg(X) Data("d" 0) false)
    Compare(Reg(A) Imm(3)) Branch(EQ ".x") Return(false false false) Label(".x")
    Return(false false false))");
  CHECK(dump(apply_copy_through_index(in, kCtx)) == dump(want));
}

TEST_CASE("json_quote writes a string as Python's json.dumps does") {
  using c6502::passes::registry::json_quote;
  // A backslash, built apart so that no escape below reads as one.
  const std::string bs(1, '\\');
  const std::string q(1, '"');
  const std::string quoted = json_quote("a\"b\\c\n");
  CHECK(quoted == q + "a" + bs + q + "b" + bs + bs + "c" + bs + "n" + q);
  const std::string controls = json_quote("\x01\x7f");
  CHECK(controls == q + bs + "u0001" + bs + "u007f" + q);
  const std::string accent = json_quote("\xc3\xa9");
  CHECK(accent == q + bs + "u00e9" + q);
  const std::string astral = json_quote("\xf0\x9f\x98\x80");
  CHECK(astral == q + bs + "ud83d" + bs + "ude00" + q);
}
