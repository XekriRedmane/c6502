// Tests of the port of passes/prologue_synthesis.py. The Python pass has
// no unit tests of its own (tests/test_param_registers.py checks it end
// to end), so these cover each rule of its docstring.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "errors.h"
#include "passes/prologue_synthesis.h"

using c6502::passes::FrameDims;
using c6502::passes::FrameDimsByFunction;
using c6502::passes::synthesize_program;
namespace asm_text = c6502::asm_text;

namespace {

std::string run(std::string_view body, FrameDims dims) {
  FrameDimsByFunction by_fn;
  by_fn.insert_or_assign("f", std::move(dims));
  return asm_text::instructions(synthesize_program(asm_text::program(body), by_fn));
}

}  // namespace

TEST_CASE("prologue_synthesis leaves a function with no frame alone") {
  const char* body = "Mov(Imm(1) Reg(A) false) Return(true false false)";
  CHECK(run(body, FrameDims{}) == asm_text::expect(body));
}

TEST_CASE("prologue_synthesis adds the prologue and makes each Return a Ret") {
  CHECK(run("Mov(Imm(1) Data(\"x\" 0) false) Return(false true false)",
            FrameDims{.arg_bytes = 2, .local_bytes = 3, .callee_saved_addrs = {0xC0}}) ==
        asm_text::expect("FunctionPrologue(2 3 [192]) Mov(Imm(1) Data(\"x\" 0) false) "
                         "Ret(2 3 false true [192] false)"));
}

TEST_CASE("prologue_synthesis moves the register parameters' copies in front") {
  // The copy of A into its local goes first, past the label, and A is
  // read again after it, so the prologue is wrapped in PHA / PLA. Y is
  // overwritten before it is read, so it needs no save.
  CHECK(run("Label(\"top\") Mov(Reg(A) Data(\"a.reg\" 0) false) "
            "Add(Data(\"t\" 0) Reg(A)) Mov(Imm(0) Reg(Y) false) Return(true false false)",
            FrameDims{.local_bytes = 1}) ==
        asm_text::expect("Mov(Reg(A) Data(\"a.reg\" 0) false) Push(Reg(A)) "
                         "FunctionPrologue(0 1 []) Pop(Reg(A)) Label(\"top\") "
                         "Add(Data(\"t\" 0) Reg(A)) Mov(Imm(0) Reg(Y) false) "
                         "Ret(0 1 true false [] false)"));
}

TEST_CASE("prologue_synthesis saves Y around the prologue when Y is read") {
  CHECK(run("Mov(Reg(Y) Data(\"r\" 0) true) Return(false false false)",
            FrameDims{.arg_bytes = 1}) ==
        asm_text::expect("Mov(Reg(Y) Reg(A) false) Push(Reg(A)) FunctionPrologue(1 0 []) "
                         "Pop(Reg(A)) Mov(Reg(A) Reg(Y) false) "
                         "Mov(Reg(Y) Data(\"r\" 0) true) Ret(1 0 false false [] false)"));
}

TEST_CASE("prologue_synthesis rejects a function without dimensions or with a Ret") {
  CHECK_THROWS_AS(synthesize_program(asm_text::program("Return(false false false)"),
                                     FrameDimsByFunction{}),
                  c6502::InternalError);
  FrameDimsByFunction by_fn;
  by_fn.insert_or_assign("f", FrameDims{.arg_bytes = 1});
  CHECK_THROWS_AS(
      synthesize_program(asm_text::program("Ret(1 0 false false [] false)"), by_fn),
      c6502::InternalError);
}
