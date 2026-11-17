"""`[[noreturn]]` on a function (C23 §6.7.13.7): the parser records it
on the declaration, the type checker on the function's `FunAttr` from
any declaration, and under `-O` nothing runs after a call to
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


# The call graph and a call that abandons its caller
# (passes/call_edges.py).


def _tac(src: str):
    """The unoptimized TAC of `src`, with its c99 AST and types."""
    from c99_to_tac import translate_program
    prog, symbols, types = type_check_program(_resolved(preprocess(src)))
    return translate_program(prog, symbols, types), prog, types


def _calls_in(tac, fn: str) -> list[tac_ast.FunctionCall]:
    for tl in tac.top_level:
        if isinstance(tl, tac_ast.Function) and tl.name == fn:
            return [i for i in tl.instructions
                    if isinstance(i, tac_ast.FunctionCall)]
    raise KeyError(fn)


def _codegen(src: str) -> str:
    return _run_stage("codegen", preprocess(src), optimize=True)


def _body(asm: str, name: str) -> str:
    """The text of function `name` in `asm`, up to the next one."""
    lines = asm.splitlines()
    start = lines.index(f"{name}:")
    end = start + 1
    while end < len(lines) and not (
        lines[end] and not lines[end][0].isspace()
        and lines[end].endswith(":") and not lines[end].startswith(".")
    ):
        end += 1
    return "\n".join(lines[start:end])


def _soft_stack(text: str) -> bool:
    """True iff `text` reads or writes through SSP or FP."""
    return any(f"({r})" in text or f"   {r}" in text or f"{r}+" in text
               for r in ("SSP", "FP"))


# A game's main loop, which `escape` abandons from two calls down.
# `get_key` takes its argument in X, so it has to be zp_abi, and
# `escape` returns its `bool` in the carry flag.
_LOOP = """
#include <stdint.h>
#include <stdbool.h>
volatile uint8_t key;
uint8_t last;
[[noreturn]] void command_loop(void);
static bool escape(uint8_t k) { return k == 0x9B; }
static uint8_t get_key(uint8_t n [[c6502::in_x]]) {
    uint8_t k = key + n;
    if (escape(k))
        command_loop();
    return k;
}
void command_loop(void) {
    for (;;)
        last = get_key(3);
}
"""


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestCallGraph(unittest.TestCase):

    def test_call_marked_noreturn(self) -> None:
        tac, _, _ = _tac(_LOOP)
        calls = {c.name: c.noreturn for c in _calls_in(tac, "get_key")}
        self.assertEqual(calls, {"escape": False, "command_loop": True})

    def test_abandons_only_without_arguments(self) -> None:
        from passes.call_edges import abandons_caller
        x = tac_ast.Var(name="x")
        self.assertTrue(abandons_caller(tac_ast.FunctionCall(
            name="r", args=[], noreturn=True)))
        self.assertFalse(abandons_caller(tac_ast.FunctionCall(
            name="r", args=[x], noreturn=True)))
        self.assertFalse(abandons_caller(tac_ast.FunctionCall(
            name="r", args=[], noreturn=False)))

    def test_cycle_through_the_call_is_zp_abi(self) -> None:
        from passes.abi_selection import ZpLayout, select_abi
        from passes.zp_local_allocation import pool_eligible
        tac, prog, types = _tac(_LOOP)
        abi = select_abi(tac, prog, types)
        for name in ("escape", "get_key", "command_loop"):
            with self.subTest(name):
                self.assertIsInstance(abi[name], ZpLayout)
        self.assertEqual(
            pool_eligible(tac, abi), {"escape", "get_key", "command_loop"},
        )

    def test_code(self) -> None:
        # Before, the `register` parameter was an error, since
        # `command_loop` was on a cycle and used the soft stack, and
        # without it `get_key` tested the carry with `LDA #0; ROL A`.
        asm = _codegen(_LOOP)
        self.assertFalse(_soft_stack(asm), asm)
        get_key = _body(asm, "get_key")
        self.assertIn("JMP   command_loop", get_key)
        self.assertIn("BCC", get_key)
        self.assertNotIn("ROL", get_key)

    def test_call_with_arguments_stays_an_edge(self) -> None:
        # `r`'s argument is written while `f`'s frame is live, so the
        # call is an ordinary call, and `f` and `r` are on a cycle.
        from passes.abi_selection import SoftStackLayout, select_abi
        tac, prog, types = _tac(
            "unsigned char out;\n"
            "void f(unsigned char x);\n"
            "[[noreturn]] void r(unsigned char a) { for (;;) f(a); }\n"
            "void f(unsigned char x) { if (x == 3) r(x); out = x; }\n"
        )
        abi = select_abi(tac, prog, types)
        self.assertIsInstance(abi["f"], SoftStackLayout)
        self.assertIsInstance(abi["r"], SoftStackLayout)

    def test_recursion_through_ordinary_calls_stays_a_cycle(self) -> None:
        # `g` can still return into `f`, which called it, so the
        # noreturn call next to that doesn't take them off the cycle.
        from passes.abi_selection import SoftStackLayout, select_abi
        tac, prog, types = _tac(
            "unsigned char n;\n"
            "[[noreturn]] void reset(void);\n"
            "void g(void);\n"
            "void f(void) { if (n) g(); }\n"
            "void g(void) { n--; if (n == 7) reset(); f(); }\n"
            "void reset(void) { for (;;) f(); }\n"
        )
        abi = select_abi(tac, prog, types)
        self.assertIsInstance(abi["f"], SoftStackLayout)
        self.assertIsInstance(abi["g"], SoftStackLayout)

    def test_pragma_on_the_call(self) -> None:
        from passes.recursion_saves import RecursionSaveError
        src = ("[[noreturn]] void r(void);\n"
               "void f(void) {\n"
               "#pragma c6502 call save(hardware_stack)\n"
               "r();\n}\n"
               "void r(void) { for (;;) f(); }\n")
        with self.assertRaises(RecursionSaveError) as cm:
            _codegen(src)
        self.assertIn("nothing has to be saved", str(cm.exception))

    def test_link_metadata(self) -> None:
        # No `call` line for the call, and nothing is on a cycle.
        asm = _codegen(_LOOP)
        meta = [l for l in asm.splitlines() if l.startswith("; call ")
                or l.startswith("; def ")]
        self.assertIn("; call get_key -> escape", meta)
        self.assertFalse(any("-> command_loop" in l for l in meta), meta)
        self.assertFalse(any("in_cycle=true" in l for l in meta), meta)


# A game that starts over from `reset_game` whenever `deep` ends a
# level, two calls below it. Each trip around leaves the return
# addresses of `middle` and `deep` on the hardware stack, so 200 trips
# wrap the stack pointer around page 1 a few times. `out` is volatile,
# since `halt` never returns.
_GAME_DEEP = """
volatile unsigned char out[3];
unsigned char steps;
[[noreturn]] void reset_game(void);
unsigned char deep(unsigned char n) {
    unsigned char v = n + 1;
    if (++steps == 3) {
        steps = 0;
        reset_game();
    }
    return v;
}
unsigned char middle(unsigned char n) {
    return deep(n) + deep(n + 1);
}
"""

_GAME_TOP = """
volatile unsigned char out[3];
unsigned char resets;
unsigned char middle(unsigned char n);
[[noreturn]] static void halt(void) {
    out[1] = 0xAA;
    for (;;) {
    }
}
[[noreturn]] void reset_game(void) {
    out[0] = ++resets;
    if (resets == 200)
        halt();
    for (;;)
        out[2] = middle(resets);
}
int main(void) {
    reset_game();
}
"""

# `out[2]` is `middle(199)`, which is 200 + 201, modulo 256.
_GAME_OUT = [200, 0xAA, (200 + 201) & 0xFF]


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestCycleRuns(unittest.TestCase):

    def test_one_unit(self) -> None:
        src = _GAME_DEEP + _GAME_TOP.replace(
            "volatile unsigned char out[3];\n", "")
        asm = _codegen(src)
        self.assertFalse(_soft_stack(asm), asm)
        self.assertIn("JMP   reset_game", _body(asm, "deep"))
        sim = build_sim(src, optimize=True)
        sim.run(max_cycles=500_000)
        base = sim.symbols["out"]
        self.assertEqual(list(sim.mpu.memory[base:base + 3]), _GAME_OUT)

    def test_two_units(self) -> None:
        # Before, the link failed with "cycle detected in zp_abi call
        # graph", since each unit's metadata had a `call` line for the
        # call to `reset_game`.
        import tempfile
        from pathlib import Path
        from sim.linked import build_linked_sim
        from tests.link_helpers import compile_object, link, linked_text
        with tempfile.TemporaryDirectory() as tmp:
            d = Path(tmp)
            self.assertEqual(compile_object(_GAME_DEEP.replace(
                "volatile unsigned char out[3];",
                "extern volatile unsigned char out[3];"), d / "deep.o"), 0)
            self.assertEqual(compile_object(_GAME_TOP, d / "top.o"), 0)
            self.assertEqual(link([d / "deep.o", d / "top.o"], d / "game"), 0)
            text = linked_text(d / "game")
            code = "\n".join(
                l for l in text.splitlines() if not l.startswith(";"))
            # `main` falls into `reset_game`, so the disassembly names
            # their one address `main`.
            self.assertRegex(code, r"JMP   (main|reset_game)\n\.if_end")
            self.assertFalse(_soft_stack(code), code)
            sim = build_linked_sim(text.exe)
            sim.run(max_cycles=500_000)
            base = sim.symbols["out"]
            self.assertEqual(list(sim.mpu.memory[base:base + 3]), _GAME_OUT)


if __name__ == "__main__":
    unittest.main()
