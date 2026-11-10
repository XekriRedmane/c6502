"""Tests for `__attribute__((preserves_x))` / `preserves_y`: parsing,
the rules the type checker enforces, how calls carry the set to the
asm passes, liveness, keeping a loop counter in X or Y across a call
(`loop_counter_to_x` count-up loops, `counter_to_index_reg`), and
simulator runs against stand-in routines.
"""
from __future__ import annotations

import re
import unittest

import asm_ast
from parser import ParserError, parse
from passes.asm_liveness import call_preserves, kills_x, kills_y
from passes.type_checking import TypeCheckError
from sim.harness import build_sim


def _codegen(src: str, optimize: bool = True) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _body(asm: str, fn: str) -> str:
    """The lines of function `fn` in `asm`."""
    m = re.search(rf"(?ms)^{fn}:\n(.*?)(?=^[A-Za-z_]\w*:|\Z)", asm)
    return m.group(1) if m else ""


# A routine at $D000 that takes A, keeps X, and destroys Y.
_KEEPS_X = (
    "__attribute__((zp_abi, preserves_x, address(0xD000)))\n"
    "void put(register unsigned char c);\n"
)
# The same routine, declared without the promise.
_PLAIN = (
    "__attribute__((zp_abi, address(0xD000)))\n"
    "void put(register unsigned char c);\n"
)
# `buf` is summed into `total` by the stand-in, one byte per call.
_SUM_LOOP = (
    "unsigned char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
    "unsigned char n = 8;\n"
    "void send(void) {\n"
    "    for (unsigned char i = 0; i != n; i++)\n"
    "        put(buf[i]);\n"
    "}\n"
    "int main(void) { send(); return 0; }\n"
)
_TOTAL = 0x3100
# CLC ; ADC total ; STA total ; LDY #$FF ; RTS
_STANDIN = bytes([0x18, 0x6D, 0x00, 0x31, 0x8D, 0x00, 0x31,
                  0xA0, 0xFF, 0x60])


class TestParse(unittest.TestCase):

    def test_recorded(self) -> None:
        prog = parse(
            "__attribute__((preserves_y, address(0x1000), preserves_x))\n"
            "void f(void);\n"
        )
        self.assertEqual(prog.declaration[0].function_decl.preserves,
                         ["X", "Y"])

    def test_rejected(self) -> None:
        bad = {
            "argument": "__attribute__((preserves_x(1), address(1))) "
                        "void f(void);",
            "definition": "__attribute__((preserves_x)) void f(void) {}",
            "object": "extern unsigned char k "
                      "__attribute__((preserves_x));",
        }
        for label, src in bad.items():
            with self.subTest(label):
                with self.assertRaises(ParserError):
                    parse(src)


class TestTypeCheck(unittest.TestCase):

    def test_rejected(self) -> None:
        bad = {
            "no address":
                "__attribute__((preserves_x)) void f(void);\n",
            "int result in X":
                "__attribute__((preserves_x, address(0x1000))) "
                "int f(void);\n",
            "unsigned int result in X":
                "__attribute__((preserves_x, address(0x1000))) "
                "unsigned int f(void);\n",
        }
        for label, src in bad.items():
            with self.subTest(label):
                with self.assertRaises(TypeCheckError):
                    _codegen(src)

    def test_accepted(self) -> None:
        for label, src in {
            "Y with an int result":
                "__attribute__((preserves_y, address(0x1000))) "
                "int f(void);\n",
            "address declared first":
                "__attribute__((address(0x1000))) void f(void);\n"
                "__attribute__((preserves_x)) void f(void);\n",
        }.items():
            with self.subTest(label):
                _codegen(src + "void g(void) { f(); }\n")


class TestCalls(unittest.TestCase):

    def _call(self, src: str) -> asm_ast.Call:
        from compile import _resolved
        from c99_to_tac import translate_program as to_tac
        from passes.type_checking import check_program
        from tac_to_asm import translate_program as to_asm
        prog, symbols, types = check_program(_resolved(src, optimize=False))
        asm = to_asm(to_tac(prog, symbols, types), symbols, types)
        return next(
            i for tl in asm.top_level if getattr(tl, "name", "") == "g"
            for i in tl.instructions if isinstance(i, asm_ast.Call)
        )

    def test_call_carries_the_set(self) -> None:
        call = self._call(
            "__attribute__((preserves_y, preserves_x, address(0x1000)))\n"
            "void f(void);\nvoid g(void) { f(); }\n"
        )
        self.assertEqual(call.preserves, ["X", "Y"])
        self.assertFalse(kills_x(call))
        self.assertFalse(kills_y(call))

    def test_plain_call_clobbers(self) -> None:
        call = self._call(
            "__attribute__((address(0x1000))) void f(void);\n"
            "void g(void) { f(); }\n"
        )
        self.assertEqual(call.preserves, [])
        self.assertTrue(kills_x(call))
        self.assertFalse(call_preserves(call, "Y"))


class TestCountUpLoop(unittest.TestCase):

    def test_counter_in_x_across_the_call(self) -> None:
        body = _body(_codegen(_KEEPS_X + _SUM_LOOP), "send")
        self.assertRegex(body, r"CPX\s+n")
        self.assertRegex(body, r"LDA\s+buf,X\n\s+JSR\s+put\n\s+INX")
        self.assertNotRegex(body, r"STX|LDX\s+__local")

    def test_wrapped_without_the_promise(self) -> None:
        # The counter is still kept in X, but saved and reloaded
        # around a call that may clobber it.
        body = _body(_codegen(_PLAIN + _SUM_LOOP), "send")
        self.assertRegex(body, r"STX\s+(\S+)\n\s+JSR\s+put\n\s+LDX\s+\1")

    def test_call_before_the_loop_not_wrapped(self) -> None:
        src = (
            _PLAIN
            + "unsigned char buf[4];\nunsigned char n;\n"
            "void send(void) {\n"
            "    put(1); put(2);\n"
            "    for (unsigned char i = 0; i != n; i++) put(buf[i]);\n"
            "}\n"
        )
        body = _body(_codegen(src), "send")
        # Only the call inside the loop is wrapped.
        self.assertEqual(len(re.findall(r"STX", body)), 1)

    def test_runs(self) -> None:
        for decl in (_KEEPS_X, _PLAIN):
            for optimize in (False, True):
                with self.subTest(preserves=decl is _KEEPS_X,
                                  optimize=optimize):
                    sim = build_sim(decl + _SUM_LOOP, optimize=optimize)
                    sim.mpu.memory[0xD000:0xD000 + len(_STANDIN)] = _STANDIN
                    sim.mpu.memory[_TOTAL] = 0
                    result = sim.run()
                    self.assertEqual(result.memory[_TOTAL], 36)

    def test_the_declaration_is_trusted(self) -> None:
        # A stand-in that changes X while declared `preserves_x` breaks
        # the loop, which is what shows the compiler relies on it.
        clobbers_x = bytes([0x18, 0x6D, 0x00, 0x31, 0x8D, 0x00, 0x31,
                            0xA2, 0x07, 0x60])  # ... LDX #7 ; RTS
        sim = build_sim(_KEEPS_X + _SUM_LOOP, optimize=True)
        sim.mpu.memory[0xD000:0xD000 + len(clobbers_x)] = clobbers_x
        sim.mpu.memory[_TOTAL] = 0
        result = sim.run()
        self.assertNotEqual(result.memory[_TOTAL], 36)


class TestCountDownInY(unittest.TestCase):
    """`counter_to_index_reg` keeps a count-down counter in a register
    across a call that preserves it."""

    _SRC = (
        "unsigned char n = 5;\n"
        "void tick(void) {\n"
        "    unsigned char c = n;\n"
        "    do { put(0); } while (--c != 0);\n"
        "}\n"
    )

    def test_counter_in_y(self) -> None:
        keeps_y = (
            "__attribute__((zp_abi, preserves_y, address(0xD000)))\n"
            "void put(register unsigned char c);\n"
        )
        body = _body(_codegen(keeps_y + self._SRC), "tick")
        self.assertRegex(body, r"JSR\s+put\n\s+DEY")

    def test_counter_in_memory_without_the_promise(self) -> None:
        body = _body(_codegen(_PLAIN + self._SRC), "tick")
        self.assertNotRegex(body, r"DE[XY]")


if __name__ == "__main__":
    unittest.main()
