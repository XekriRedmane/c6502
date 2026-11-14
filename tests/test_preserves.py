"""Tests for `[[c6502::preserves_x]]` / `preserves_y`: parsing,
the rules the type checker enforces, how calls carry the set to the
asm passes, liveness, keeping a loop counter in X or Y across a call
(`loop_counter_to_x` count-up loops, `counter_to_index_reg`), a
`register` local in Y across a call (the register allocator), an
immediate kept in X across a call (`redundant_load`), and simulator
runs against stand-in routines.
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
    "[[c6502::zp_abi, c6502::preserves_x, c6502::address(0xD000)]]\n"
    "void put(register unsigned char c);\n"
)
# The same routine, declared without the promise.
_PLAIN = (
    "[[c6502::zp_abi, c6502::address(0xD000)]]\n"
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
            "[[c6502::preserves_y, c6502::address(0x1000), c6502::preserves_x]]\n"
            "void f(void);\n"
        )
        self.assertEqual(prog.declaration[0].function_decl.preserves,
                         ["X", "Y"])

    def test_rejected(self) -> None:
        bad = {
            "argument": "[[c6502::preserves_x(1), c6502::address(1)]] "
                        "void f(void);",
            "definition": "[[c6502::preserves_x]] void f(void) {}",
            "object": "extern unsigned char k "
                      "[[c6502::preserves_x]];",
        }
        for label, src in bad.items():
            with self.subTest(label):
                with self.assertRaises(ParserError):
                    parse(src)


class TestTypeCheck(unittest.TestCase):

    def test_rejected(self) -> None:
        bad = {
            "no address":
                "[[c6502::preserves_x]] void f(void);\n",
            "int result in X":
                "[[c6502::preserves_x, c6502::address(0x1000)]] "
                "int f(void);\n",
            "unsigned int result in X":
                "[[c6502::preserves_x, c6502::address(0x1000)]] "
                "unsigned int f(void);\n",
        }
        for label, src in bad.items():
            with self.subTest(label):
                with self.assertRaises(TypeCheckError):
                    _codegen(src)

    def test_accepted(self) -> None:
        for label, src in {
            "Y with an int result":
                "[[c6502::preserves_y, c6502::address(0x1000)]] "
                "int f(void);\n",
            "address declared first":
                "[[c6502::address(0x1000)]] void f(void);\n"
                "[[c6502::preserves_x]] void f(void);\n",
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
            "[[c6502::preserves_y, c6502::preserves_x, c6502::address(0x1000)]]\n"
            "void f(void);\nvoid g(void) { f(); }\n"
        )
        self.assertEqual(call.preserves, ["X", "Y"])
        self.assertFalse(kills_x(call))
        self.assertFalse(kills_y(call))

    def test_plain_call_clobbers(self) -> None:
        call = self._call(
            "[[c6502::address(0x1000)]] void f(void);\n"
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
            "[[c6502::zp_abi, c6502::preserves_y, c6502::address(0xD000)]]\n"
            "void put(register unsigned char c);\n"
        )
        body = _body(_codegen(keeps_y + self._SRC), "tick")
        self.assertRegex(body, r"JSR\s+put\n\s+DEY")

    def test_counter_in_memory_without_the_promise(self) -> None:
        body = _body(_codegen(_PLAIN + self._SRC), "tick")
        self.assertNotRegex(body, r"DE[XY]")


def _run_total(src: str, standin: bytes, optimize: bool) -> int:
    """Run `src` with `standin` at $D000 and return the byte at
    `_TOTAL` it accumulates."""
    sim = build_sim(src, optimize=optimize)
    sim.mpu.memory[0xD000:0xD000 + len(standin)] = standin
    sim.mpu.memory[_TOTAL] = 0
    return sim.run().memory[_TOTAL]


class TestRegisterLocalInY(unittest.TestCase):
    """The register allocator keeps a value, here a `register` local,
    in Y across a call that preserves Y."""

    _KEEPS_Y = (
        "[[c6502::zp_abi, c6502::preserves_y, c6502::address(0xD000)]]\n"
        "void put(register unsigned char c);\n"
    )
    _SRC = (
        "unsigned char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
        "unsigned char n = 8;\n"
        "void send(void) {\n"
        "    register unsigned char i;\n"
        "    for (i = 0; i < n; i++) put(buf[i]);\n"
        "}\n"
        "int main(void) { send(); return 0; }\n"
    )
    # CLC ; ADC total ; STA total ; LDX #$FF ; RTS (keeps Y)
    _STANDIN = bytes([0x18, 0x6D, 0x00, 0x31, 0x8D, 0x00, 0x31,
                      0xA2, 0xFF, 0x60])

    def test_in_y_across_the_call(self) -> None:
        body = _body(_codegen(self._KEEPS_Y + self._SRC), "send")
        self.assertRegex(body, r"LDA\s+buf,Y\n\s+JSR\s+put\n\s+INY")
        self.assertNotRegex(body, r"STY|__local")

    def test_rejected_without_the_promise(self) -> None:
        from passes.optimization_asm.regalloc import RegallocError
        with self.assertRaisesRegex(RegallocError, "can't pin"):
            _codegen(_PLAIN + self._SRC)

    def test_runs(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run_total(
                    self._KEEPS_Y + self._SRC, self._STANDIN, optimize,
                ), 36)


class TestImmediateInX(unittest.TestCase):
    """`redundant_load` keeps an immediate it knows X holds across a
    call that preserves X, but not one that doesn't, and never a
    memory equivalence, since the callee may write the memory."""

    _DECLS = (
        "[[c6502::zp_abi, c6502::preserves_x, c6502::address(0xD000)]]\n"
        "void keep(register unsigned char c [[c6502::in_x]]);\n"
        "[[c6502::zp_abi, c6502::address(0xD000)]]\n"
        "void lose(register unsigned char c [[c6502::in_x]]);\n"
        "unsigned char g = 4;\n"
    )
    # TXA ; CLC ; ADC total ; STA total ; LDY #$FF ; RTS (keeps X)
    _STANDIN = bytes([0x8A, 0x18, 0x6D, 0x00, 0x31, 0x8D, 0x00, 0x31,
                      0xA0, 0xFF, 0x60])

    def test_kept_across_a_preserving_call(self) -> None:
        body = _body(_codegen(
            self._DECLS + "void f(void) { keep(5); keep(5); keep(5); }\n"
        ), "f")
        self.assertEqual(len(re.findall(r"LDX\s+#\$05", body)), 1)

    def test_reloaded_after_a_plain_call(self) -> None:
        body = _body(_codegen(
            self._DECLS + "void f(void) { lose(5); lose(5); }\n"
        ), "f")
        self.assertEqual(len(re.findall(r"LDX\s+#\$05", body)), 2)

    def test_memory_reloaded(self) -> None:
        body = _body(_codegen(
            self._DECLS + "void f(void) { keep(g); keep(g); }\n"
        ), "f")
        self.assertEqual(len(re.findall(r"LDX\s+g", body)), 2)

    def test_runs(self) -> None:
        src = self._DECLS + (
            "int main(void) {\n"
            "    keep(5); keep(5); keep(7); keep(g); keep(g);\n"
            "    return 0;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(
                    _run_total(src, self._STANDIN, optimize), 25,
                )


if __name__ == "__main__":
    unittest.main()
