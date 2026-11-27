"""Tests for `[[c6502::preserves_x]]` / `preserves_y`: parsing,
the rules the type checker enforces, how calls carry the set to the
asm passes, liveness, keeping a loop counter in X or Y across a call
(`loop_counter_to_x` count-up loops, `counter_to_index_reg`), a
`register` local in Y across a call (the register allocator), an
immediate kept in X across a call (`redundant_load`), and simulator
runs against stand-in routines.

The simulator runs are here, and the tests that compile a source and
run no simulator are doctest cases in cpp/tests/test_preserves.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim


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


class TestCountUpLoop(unittest.TestCase):

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
