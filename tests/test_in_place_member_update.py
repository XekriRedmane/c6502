"""Tests for computing a member of a static object in its own bytes
(`cpp/src/passes/in_place_member_update.h`, applied by `tac_to_asm`).

The `-S` text is checked in cpp/tests/test_in_place_member_update.cpp."""

from __future__ import annotations

import unittest

from sim.harness import build_sim


def _run(src: str, optimize: bool) -> int:
    sim = build_sim(src, optimize=optimize)
    return sim.run().return_int()


_PC = """
#include <stdint.h>
typedef struct { uint16_t lo; uint8_t hi; } uint24_t;
uint24_t P;
uint16_t W;
uint8_t flag;
uint16_t t[4];
"""


class TestBehavior(unittest.TestCase):
    """Each program returns a value that a wrong rewrite changes."""

    def _check(self, body: str, expected: int) -> None:
        src = _PC + body
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), expected)

    def test_increment_carries_into_the_high_byte(self) -> None:
        self._check(
            "int main(void) { P.lo = 0x00FF; P.lo++; return P.lo; }", 0x0100,
        )

    def test_low_byte_test_after_increment(self) -> None:
        self._check(
            "uint8_t step(uint16_t v) { P.lo = v; flag = 0; P.lo++;\n"
            "    if ((uint8_t)P.lo == 0) { flag = 1;\n"
            "        if (P.lo == 0) P.hi++; }\n"
            "    return flag; }\n"
            "int main(void) { P.hi = 7; int n = 0;\n"
            "    n += step(0x00FF); n += 2 * step(0x1234);\n"
            "    n += 4 * step(0xFFFF); n += 8 * step(0x0000);\n"
            "    return n * 16 + P.hi; }",
            (1 + 4) * 16 + 8,
        )

    def test_loaded_value_read_after_the_store(self) -> None:
        # `old` is read after the store, so the member can't stand
        # for it.
        self._check(
            "uint16_t bump(void) { uint16_t old = P.lo; P.lo = old + 1;"
            " return old; }\n"
            "int main(void) { P.lo = 41; uint16_t a = bump();"
            " return a * 100 + P.lo; }",
            41 * 100 + 42,
        )

    def test_store_through_a_pointer_between_load_and_add(self) -> None:
        self._check(
            "uint16_t *q;\n"
            "void bump(void) { uint16_t v = P.lo; *q = 500; P.lo = v + 1; }\n"
            "int main(void) { q = &P.lo; P.lo = 10; bump(); return P.lo; }",
            11,
        )

    def test_value_that_reads_the_member_it_replaces(self) -> None:
        # Writing the low byte first would change the high byte's
        # source.
        self._check(
            "void swap(void) { P.lo = (P.lo >> 8) | (P.lo << 8); }\n"
            "int main(void) { P.lo = 0x1234; swap(); return P.lo; }",
            0x3412,
        )

    def test_value_that_reads_another_member(self) -> None:
        self._check(
            "void f(void) { P.lo = P.hi + 0x0102; }\n"
            "int main(void) { P.hi = 3; f(); return P.lo; }",
            0x0105,
        )

    def test_subtract_borrows(self) -> None:
        self._check(
            "int main(void) { t[1] = 0x0100; t[1] -= 1; return t[1]; }",
            0x00FF,
        )


if __name__ == "__main__":
    unittest.main()
