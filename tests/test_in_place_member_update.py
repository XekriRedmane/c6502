"""Tests for computing a member of a static object in its own bytes
(`passes/in_place_member_update.py`, applied by `tac_to_asm`)."""

from __future__ import annotations

import unittest

import tac_ast
from passes.in_place_member_update import (
    InPlaceUpdate, plan_in_place_updates,
)
from sim.harness import build_sim


def _asm(src: str, fn: str) -> str:
    """The optimized asm of function `fn` in `src`, through its first
    `RTS`."""
    from compile import _run_stage
    from preprocessor import preprocess
    asm = _run_stage("codegen", preprocess(src), optimize=True)
    body = asm.split(f"\n{fn}:")[1].split("SUBROUTINE")[1]
    return body.split("RTS")[0] + "RTS"


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


class TestCodegen(unittest.TestCase):

    def test_increment_of_a_member_is_inc(self) -> None:
        body = _asm(_PC + "void f(void) { P.lo++; }", "f")
        self.assertEqual(
            body.split(), ["INC", "P", "BNE", ".inc_done@0", "INC", "P+1",
                           ".inc_done@0:", "RTS"],
        )

    def test_increment_after_an_indexed_read(self) -> None:
        body = _asm(
            _PC + "uint8_t *A;\n"
            "uint8_t f(void) { uint8_t b = A[(uint8_t)P.lo]; P.lo++;"
            " return b; }", "f",
        )
        self.assertIn("LDY   P\n", body)
        self.assertIn("INC   P\n", body)
        self.assertNotIn("STA   P", body)

    def test_byte_member_and_array_element(self) -> None:
        self.assertEqual(
            _asm(_PC + "void f(void) { P.hi++; }", "f").split(),
            ["INC", "P+2", "RTS"],
        )
        self.assertIn(
            "INC   t+4\n", _asm(_PC + "void f(void) { t[2]++; }", "f"),
        )

    def test_value_computed_into_the_member(self) -> None:
        body = _asm(_PC + "void f(void) { P.lo = W << 1; }", "f")
        self.assertEqual(
            body.split(), ["LDA", "W", "ASL", "STA", "P", "LDA", "W+1",
                           "ROL", "STA", "P+1", "RTS"],
        )


class TestPlan(unittest.TestCase):
    """`plan_in_place_updates` on TAC built by hand, for shapes C
    doesn't produce today."""

    @staticmethod
    def _plan(src1: tac_ast.Type_val) -> list:
        u = tac_ast.Var(name="u")
        zero = tac_ast.Constant(const=tac_ast.ConstUChar(value=0))
        one = tac_ast.Constant(const=tac_ast.ConstUInt(value=1))
        instrs = [
            tac_ast.Binary(op=tac_ast.Add(), src1=src1, src2=one, dst=u),
            tac_ast.IndexedSymbolStore(
                name="P", index=zero, src=u, is_volatile=False, offset=0,
            ),
        ]
        return plan_in_place_updates(
            instrs, [], lambda name: name not in ("P", "W"), lambda v: 2,
        )

    def test_value_from_elsewhere_is_computed_in_place(self) -> None:
        self.assertEqual(
            self._plan(tac_ast.Var(name="W")),
            [InPlaceUpdate(store=1, u="u", obj="P", base=0)],
        )

    def test_value_that_reads_the_object_isnt(self) -> None:
        self.assertEqual(self._plan(tac_ast.Var(name="P")), [])


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
