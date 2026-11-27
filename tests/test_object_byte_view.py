"""Tests for subscripting an object through a cast of its address.

`((uint8_t *)&S)[i]`, and `*((uint8_t *)&S + i)`, on an object `S`
with static storage read the object as an array of the pointed-to
type, so `c99_to_tac` lowers them as it lowers a subscript of a
static array: `IndexedLoad(S, i)` or `IndexedSymbolStore(S, i, v)`,
which the backend emits as `LDA S,X` / `STA S,X`, and `S+k` for a
constant index. Reading a `uint32_t` a byte at a time this way is the
access C99 §6.5.7 allows for any object. See `_object_view` and
`try_indexed_load_subscript` in cpp/src/c99_to_tac_exp.cpp.

The tests of the TAC and the asm are doctest cases in
cpp/tests/test_object_byte_view.cpp. The simulator runs check each
pipeline. The ones that write a byte and then read the whole object in
the same function check that the optimizer treats the indexed write as
a write to the object, and doesn't carry the object's earlier value
past it.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


_PRELUDE = (
    "#include <stdint.h>\n"
    "extern uint32_t Z [[c6502::zeropage(0x8E)]];\n"
    "static uint32_t S;\n"
    "#define ZB ((uint8_t *)&Z)\n"
    "#define SB ((uint8_t *)&S)\n"
)


@needs_c6502cpp
class TestObjectByteViewSim(unittest.TestCase):
    """Each check sets a bit of the result when it fails, so a result
    of 0 means every check passed."""

    def _check(self, body: str) -> None:
        src = _PRELUDE + body
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), 0)

    def test_reads_each_byte(self) -> None:
        self._check(
            "uint8_t zrd(uint8_t i) { return ZB[i]; }\n"
            "uint8_t zrev(uint8_t i) { return ZB[3 - i]; }\n"
            "uint8_t srd(uint8_t i) { return *(SB + i); }\n"
            "int main(void) {\n"
            "    int bad = 0;\n"
            "    Z = 0x44332211UL;\n"
            "    S = 0x88776655UL;\n"
            "    for (uint8_t i = 0; i < 4; i++) {\n"
            "        if (zrd(i) != 0x11 * (i + 1)) bad |= 1;\n"
            "        if (zrev(i) != 0x11 * (4 - i)) bad |= 2;\n"
            "        if (srd(i) != 0x11 * (i + 5)) bad |= 4;\n"
            "    }\n"
            "    if (ZB[0] != 0x11 || ZB[1] != 0x22) bad |= 8;\n"
            "    if (ZB[2] != 0x33 || ZB[3] != 0x44) bad |= 16;\n"
            "    if (*SB != 0x55 || *(SB + 3) != 0x88) bad |= 32;\n"
            "    if (((uint16_t *)&S)[1] != 0x8877) bad |= 64;\n"
            "    return bad;\n"
            "}\n"
        )

    def test_byte_writes_reach_whole_object(self) -> None:
        self._check(
            "void zwr(uint8_t i, uint8_t v) { ZB[i] = v; }\n"
            "void swr(uint8_t i, uint8_t v) { *(SB + i) = v; }\n"
            "int main(void) {\n"
            "    int bad = 0;\n"
            "    Z = 0;\n"
            "    S = 0;\n"
            "    for (uint8_t i = 0; i < 4; i++) {\n"
            "        zwr(i, 0x10 + i);\n"
            "        swr(3 - i, 0xA0 + i);\n"
            "    }\n"
            "    if (Z != 0x13121110UL) bad |= 1;\n"
            "    if (S != 0xA0A1A2A3UL) bad |= 2;\n"
            "    ZB[1] = 0x55;\n"
            "    *(SB + 2) = 0x66;\n"
            "    if (Z != 0x13125510UL) bad |= 4;\n"
            "    if (S != 0xA066A2A3UL) bad |= 8;\n"
            "    ((uint16_t *)&Z)[1] = 0xBEEF;\n"
            "    if (Z != 0xBEEF5510UL) bad |= 16;\n"
            "    return bad;\n"
            "}\n"
        )

    def test_write_after_whole_store_in_one_function(self) -> None:
        # The constant stored to the object must not reach the read
        # after the byte write.
        self._check(
            "uint32_t g(uint8_t i, uint8_t v) {\n"
            "    S = 0x01020304UL;\n"
            "    SB[i] = v;\n"
            "    return S;\n"
            "}\n"
            "uint32_t h(uint8_t v) {\n"
            "    Z = 0x01020304UL;\n"
            "    ZB[2] = v;\n"
            "    return Z;\n"
            "}\n"
            "uint8_t k(uint8_t i) {\n"
            "    uint8_t before = SB[i];\n"
            "    S = 0;\n"
            "    return before + SB[i];\n"
            "}\n"
            "int main(void) {\n"
            "    int bad = 0;\n"
            "    if (g(1, 0xEE) != 0x0102EE04UL) bad |= 1;\n"
            "    if (S != 0x0102EE04UL) bad |= 2;\n"
            "    if (h(0x77) != 0x01770304UL) bad |= 4;\n"
            "    S = 0x00000500UL;\n"
            "    if (k(1) != 5) bad |= 8;\n"
            "    return bad;\n"
            "}\n"
        )

    def test_byte_write_from_a_register_parameter(self) -> None:
        # `v` arrives in A, and backward copy propagation moved its
        # store to `ZB[2]` up to the entry, in front of the stores of
        # `Z`, since it took `Pseudo(Z, 2)` and `Data(Z, 2)` for
        # different bytes.
        self._check(
            "uint32_t h(register uint8_t v) {\n"
            "    Z = 0x01020304UL;\n"
            "    ZB[2] = v;\n"
            "    return Z;\n"
            "}\n"
            "int main(void) { return h(0x77) != 0x01770304UL; }\n"
        )

    def test_compound_assignment_and_increment(self) -> None:
        self._check(
            "uint8_t m;\n"
            "void orb(uint8_t i) { ZB[i] |= m; }\n"
            "void inc(uint8_t i) { ++*(SB + i); }\n"
            "void add(uint8_t i) { SB[i] += 0x10; }\n"
            "int main(void) {\n"
            "    int bad = 0;\n"
            "    Z = 0x01010101UL;\n"
            "    S = 0x000000FFUL;\n"
            "    m = 0x80;\n"
            "    orb(0);\n"
            "    orb(3);\n"
            "    if (Z != 0x81010181UL) bad |= 1;\n"
            "    inc(0);\n"
            "    inc(2);\n"
            "    if (S != 0x00010000UL) bad |= 2;\n"
            "    add(2);\n"
            "    if (S != 0x00110000UL) bad |= 4;\n"
            "    ZB[1] ^= 0x03;\n"
            "    (*(ZB + 2))--;\n"
            "    if (Z != 0x81000281UL) bad |= 8;\n"
            "    return bad;\n"
            "}\n"
        )


if __name__ == "__main__":
    unittest.main()
