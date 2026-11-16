"""`[[noreturn]]` on a function (C23 §6.7.13.7): the parser records it
on the declaration, the type checker on the function's `FunAttr` from
any declaration, and under `--optimize` nothing runs after a call to
the function (`passes/optimization/noreturn_call.py`)."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from compile import _resolved, _run_stage, type_check_program
from parser import parse
from passes.optimization.noreturn_call import noreturn_calls
from passes.type_checking import FunAttr, Symbol, SymbolTable
from preprocessor import preprocess
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


class TestDeclarations(unittest.TestCase):

    def test_parsed_on_declarations_and_definitions(self) -> None:
        prog = parse(
            "[[noreturn]] void a(void);\n"
            "void b [[_Noreturn]] (void);\n"
            "void c(void);\n"
            "[[noreturn]] void d(void) { for (;;) {} }\n"
        )
        got = [d.function_decl.noreturn for d in prog.declaration]
        self.assertEqual(got, [True, True, False, True])

    @unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
    def test_any_declaration_makes_it_noreturn(self) -> None:
        src = (
            "void die(void);\n"
            "[[noreturn]] void die(void);\n"
            "void die(void);\n"
            "void stop(void);\n"
        )
        _prog, symbols, _types = type_check_program(
            _resolved(preprocess(src)),
        )
        self.assertTrue(symbols["die"].attrs.noreturn)
        self.assertFalse(symbols["stop"].attrs.noreturn)


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


class TestPass(unittest.TestCase):

    def _symbols(self) -> SymbolTable:
        symbols = SymbolTable()
        void = c99_ast.FunType(params=[], ret=c99_ast.Void())
        symbols["die"] = Symbol(
            type=void, attrs=FunAttr(defined=False, is_global=True,
                                     noreturn=True),
        )
        symbols["g"] = Symbol(
            type=void, attrs=FunAttr(defined=False, is_global=True),
        )
        symbols["f"] = Symbol(
            type=c99_ast.FunType(params=[], ret=c99_ast.Int()),
            attrs=FunAttr(defined=True, is_global=True),
        )
        return symbols

    def test_code_after_the_call_goes(self) -> None:
        one = tac_ast.Constant(const=tac_ast.ConstInt(value=1))
        fn = tac_ast.Function(name="f", is_global=True, params=[], instructions=[
            tac_ast.JumpIfTrue(condition=_var("x"), target=".ok"),
            tac_ast.FunctionCall(name="die", args=[], dst=None),
            tac_ast.FunctionCall(name="g", args=[], dst=None),
            tac_ast.Copy(src=one, dst=_var("y")),
            tac_ast.Label(name=".ok"),
            tac_ast.Ret(val=one),
        ])
        out = noreturn_calls(fn, self._symbols()).instructions
        # The `Ret` of an `int` function has no value, since it never
        # runs, and `tail_call` then makes the call `JMP die`.
        self.assertEqual(out, [
            fn.instructions[0], fn.instructions[1],
            tac_ast.Ret(val=None),
            *fn.instructions[4:],
        ])

    def test_other_calls_left_alone(self) -> None:
        fn = tac_ast.Function(name="f", is_global=True, params=[], instructions=[
            tac_ast.FunctionCall(name="g", args=[], dst=None),
            tac_ast.Ret(val=None),
        ])
        self.assertIs(noreturn_calls(fn, self._symbols()), fn)


# `out` is volatile, since the optimizer drops a store that no path to
# a return can read, and `stop` never returns.
_PROGRAM = """
volatile unsigned char out[3];
[[noreturn]] void stop(unsigned char code);
void stop(unsigned char code) {
    out[2] = code;
    for (;;) {
    }
}
unsigned char check(unsigned char n) {
    if (n == 0)
        stop(7);
    return 100 / n;
}
int main(void) {
    out[0] = check(4);
    out[1] = check(0);
    return 0;
}
"""


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestRuns(unittest.TestCase):

    def test_stops_in_the_call(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                sim.run(max_cycles=200_000)
                base = sim.symbols["out"]
                self.assertEqual(
                    list(sim.mpu.memory[base:base + 3]), [25, 0, 7],
                )

    def test_call_becomes_a_jump(self) -> None:
        asm = _run_stage("codegen", preprocess(_PROGRAM), optimize=True)
        check = asm.split("\ncheck:\n")[1].split("\nmain:\n")[0]
        self.assertIn("JMP   stop", check)
        self.assertNotIn("JSR   stop", check)


if __name__ == "__main__":
    unittest.main()
