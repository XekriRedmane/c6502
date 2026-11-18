"""Tests for subscripting an object through a cast of its address.

`((uint8_t *)&S)[i]`, and `*((uint8_t *)&S + i)`, on an object `S`
with static storage read the object as an array of the pointed-to
type, so `c99_to_tac` lowers them as it lowers a subscript of a
static array: `IndexedLoad(S, i)` or `IndexedSymbolStore(S, i, v)`,
which the backend emits as `LDA S,X` / `STA S,X`, and `S+k` for a
constant index. Reading a `uint32_t` a byte at a time this way is the
access C99 §6.5.7 allows for any object. See `_object_view` and
`_try_indexed_load_subscript` in `c99_to_tac.py`.

The simulator runs check each pipeline. The ones that write a byte
and then read the whole object in the same function check that the
optimizer treats the indexed write as a write to the object, and
doesn't carry the object's earlier value past it.
"""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim


_PRELUDE = (
    "#include <stdint.h>\n"
    "extern uint32_t Z [[c6502::zeropage(0x8E)]];\n"
    "static uint32_t S;\n"
    "#define ZB ((uint8_t *)&Z)\n"
    "#define SB ((uint8_t *)&S)\n"
)


def _tac(src: str, optimize: bool = False) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("tac", preprocess(src), optimize=optimize)


def _asm(src: str, optimize: bool) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _function(asm: str, name: str) -> str:
    """The lines of function `name` in `asm`, up to the next label at
    column 0."""
    lines = asm.splitlines()
    start = lines.index(f"{name}:")
    out = []
    for line in lines[start + 1:]:
        if line and not line[0].isspace() and not line.startswith("."):
            break
        out.append(line.strip())
    return "\n".join(out)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestObjectByteViewShape(unittest.TestCase):
    """The TAC and asm use indexed access on the object's symbol."""

    def test_tac_reads_through_indexed_load(self) -> None:
        tac = _tac(_PRELUDE + (
            "uint8_t f(uint8_t i) { return ZB[3 - i]; }\n"
        ))
        self.assertIn("IndexedLoad(", tac)
        self.assertIn("name='Z'", tac)
        self.assertNotIn("GetAddress(", tac)

    def test_tac_writes_through_indexed_symbol_store(self) -> None:
        tac = _tac(_PRELUDE + (
            "void f(uint8_t i, uint8_t v) { SB[i] = v; }\n"
        ))
        self.assertIn("IndexedSymbolStore(", tac)
        self.assertNotIn("GetAddress(", tac)

    def test_tac_constant_index_is_constant_byte(self) -> None:
        tac = _tac(_PRELUDE + "uint8_t f(void) { return SB[2]; }\n")
        self.assertIn("IndexedLoad(", tac)
        self.assertIn("ConstUChar(\n              value=2", tac)

    def test_variable_index_uses_absolute_x(self) -> None:
        src = _PRELUDE + (
            "uint8_t f(uint8_t i) { return ZB[3 - i]; }\n"
            "void g(uint8_t i, uint8_t v) { ZB[i] = v; }\n"
            "uint8_t h(uint8_t i) { return *(ZB + i); }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _asm(src, optimize)
                self.assertIn("LDA   Z,X", _function(asm, "f"))
                self.assertIn("STA   Z,X", _function(asm, "g"))
                self.assertIn("LDA   Z,X", _function(asm, "h"))
                self.assertNotIn("DPTR", _function(asm, "f"))
                self.assertNotIn("DPTR", _function(asm, "g"))

    def test_optimized_read_matches_byte_array(self) -> None:
        # The shape a byte array at the same address gets.
        asm = _asm(_PRELUDE + (
            "uint8_t f(uint8_t i) { return ZB[3 - i]; }\n"
        ), optimize=True)
        self.assertEqual(
            _function(asm, "f").split(),
            "SUBROUTINE LDA #$03 SEC SBC __zpabi_f__i TAX LDA Z,X RTS"
            .split(),
        )

    def test_constant_index_is_absolute(self) -> None:
        src = _PRELUDE + (
            "uint8_t f(void) { return ZB[2]; }\n"
            "void g(uint8_t v) { *(ZB + 1) = v; }\n"
            "uint8_t h(void) { return *ZB; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _asm(src, optimize)
                self.assertIn("LDA   Z+2", _function(asm, "f"))
                self.assertIn("STA   Z+1", _function(asm, "g"))
                self.assertNotIn(",X", _function(asm, "f"))
                self.assertNotIn("DPTR", _function(asm, "g"))

    def test_constant_added_to_index_goes_in_offset(self) -> None:
        asm = _asm(_PRELUDE + (
            "uint8_t f(uint8_t i) { return SB[i + 1]; }\n"
        ), optimize=True)
        self.assertIn("LDA   S+1,X", _function(asm, "f"))

    def test_compound_assignment_is_indexed(self) -> None:
        asm = _asm(_PRELUDE + (
            "uint8_t m;\n"
            "void f(uint8_t i) { ZB[i] |= m; }\n"
            "void g(uint8_t i) { ++*(ZB + i); }\n"
        ), optimize=True)
        for fn in ("f", "g"):
            with self.subTest(fn=fn):
                body = _function(asm, fn)
                self.assertIn("LDA   Z,X", body)
                self.assertIn("STA   Z,X", body)

    def test_two_byte_elements(self) -> None:
        asm = _asm(_PRELUDE + (
            "uint16_t f(uint8_t i) { return ((uint16_t *)&S)[i]; }\n"
        ), optimize=True)
        body = _function(asm, "f")
        self.assertIn("S,X", body)
        self.assertIn("S+1,X", body)

    def test_automatic_object_keeps_pointer_access(self) -> None:
        # An automatic object has no link-time address to index from.
        tac = _tac(_PRELUDE + (
            "uint8_t f(uint8_t i) {\n"
            "    uint32_t a = S;\n"
            "    return ((uint8_t *)&a)[i];\n"
            "}\n"
        ))
        self.assertNotIn("IndexedLoad(", tac)
        self.assertIn("GetAddress(", tac)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
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
