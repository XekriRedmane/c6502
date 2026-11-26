"""Behavioral tests for cpp/src/passes/zp_local_allocation.h.

Coverage:
  - Single eligible function → gets a private pool sized to
    `local_bytes`.
  - Caller-callee chain → disjoint local pools.
  - Sibling functions → may share addresses.
  - Diamond pattern → leaf disjoint from both intermediates.
  - Local pool avoids coexisting zp_abi param slots (ancestor
    AND descendant).
  - Ineligible: function with IndirectCall.
  - Ineligible: function in a cycle.
  - Ineligible: function calling a non-zp_abi extern.
  - Ineligible-callee propagation: caller of ineligible →
    ineligible.
  - zp_abi extern callee allowed (treated as bounded leaf).
  - Zero local_bytes → empty pool returned (function still
    listed as eligible).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import unittest
from tests.cpp_build import needs_c6502cpp


_FULL_POOL = """
#include <stdint.h>
uint8_t pool [[c6502::zeropage(0x80)]][128];
static uint8_t g(uint8_t x) { return x + 1; }
uint8_t f(uint8_t a, uint8_t b) {
    uint8_t s = g(a);
    uint8_t t = g(b);
    return s + t + a + b;
}
int main(void) { pool[0] = f(3, 4); return pool[0]; }
"""

# `_FULL_POOL` with a byte `d` in X before the parameters of `f` and
# `g`, so that `a`, `b`, and `x` arrive in slots. Without a register
# named in the declaration, `a` and `b` arrive in A and X, and so does
# `x`.
_FULL_POOL_SLOTS = """
#include <stdint.h>
uint8_t pool [[c6502::zeropage(0x80)]][128];
static uint8_t g(uint8_t d [[c6502::in_x]], uint8_t x) { return x + d; }
uint8_t f(uint8_t d [[c6502::in_x]], uint8_t a, uint8_t b) {
    uint8_t s = g(d, a);
    uint8_t t = g(d, b);
    return s + t + a + b;
}
int main(void) { pool[0] = f(1, 3, 4); return pool[0]; }
"""


@needs_c6502cpp
class TestUnitFallback(unittest.TestCase):
    """A unit compiled on its own takes the free bytes below the pool,
    around the runtime's block, before it spills out of zero page, as
    the linker does. The asm passes that follow values through zero page
    then see its slots."""

    def test_slots_below_the_pool(self) -> None:
        from compile import _run_source
        asm = _run_source(
            "codegen", _FULL_POOL_SLOTS, optimize=True,
        )
        equs = dict(
            (line.split("\t")[0], int(line.split("$")[1], 16))
            for line in asm.splitlines() if "\tEQU\t$" in line
        )
        runtime = set(range(equs["SSP"], equs["DPTR"] + 2))
        for name in ("__zpabi_f__a", "__zpabi_f__b", "__zpabi_g__x"):
            with self.subTest(name=name):
                self.assertLess(equs[name], 0x80)
                self.assertNotIn(equs[name], runtime)

    def test_runs(self) -> None:
        from sim.harness import build_sim
        for src in (_FULL_POOL, _FULL_POOL_SLOTS):
            with self.subTest(slots=src is _FULL_POOL_SLOTS):
                self.assertEqual(
                    build_sim(src, optimize=True).run().return_int(), 16,
                )

    def test_register_parameters_with_a_frame(self) -> None:
        # With the pool full, f's locals are on the soft stack, so its
        # prologue runs, and it does its arithmetic in A, where `a`
        # arrives. The prologue's PHA / PLA was dropped, since
        # `kills_a` didn't count the prologue, and `b` was stored with
        # `TXA; STA (FP),Y`, which no pass saw writing A. f returned 8
        # or 141.
        from sim.harness import build_sim
        src = _FULL_POOL.replace(
            "uint8_t f(uint8_t a, uint8_t b)",
            "uint8_t f(register uint8_t a, register uint8_t b)",
        )
        self.assertIn("register uint8_t b", src)
        self.assertEqual(
            build_sim(src, optimize=True).run().return_int(), 16,
        )


# Lode Runner's `guard_find_candidate_row_above`. Its preliminary
# register allocation used 6 bytes, and with a pool of 6 the final
# allocation had no two adjacent free bytes left for one of its row
# pointers, so a byte value went to a soft-stack frame.
_TIGHT_POOL = """
#include <stdint.h>
#include <stdbool.h>
extern uint8_t BG [[c6502::address(0x6000)]][16][28];
extern uint8_t LIMIT [[c6502::address(0x01)]];
bool is_floor(uint8_t sprite) { return sprite == 1 || sprite == 2; }
uint8_t f(uint8_t row, uint8_t col) {
    for (;;) {
        if (BG[row][col] != 3)
            return row;
        uint8_t ladder_row = row;
        row--;
        if (col != 0 && (is_floor(BG[ladder_row][col - 1]) ||
                         BG[row][col - 1] == 4)) {
            if (row <= LIMIT)
                return row;
        }
        if (col != 27 && (is_floor(BG[ladder_row][col + 1]) ||
                          BG[row][col + 1] == 4)) {
            if (row <= LIMIT)
                return row;
        }
        if (row == 0)
            return row;
    }
}
int main(void) {
    LIMIT = 9;
    for (uint8_t r = 0; r < 16; r++) BG[r][5] = 3;
    BG[9][4] = 4;
    return f(12, 5);
}
"""


@needs_c6502cpp
class TestPoolGrowth(unittest.TestCase):
    """A function whose final register allocation spills a value the
    preliminary one didn't gets a larger pool (`compile._pool_shortfalls`)."""

    def test_no_frame(self) -> None:
        from compile import _run_source
        asm = _run_source("codegen", _TIGHT_POOL, optimize=True)
        self.assertNotIn("prologue", asm)

    def test_runs(self) -> None:
        from sim.harness import build_sim
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(
                    build_sim(_TIGHT_POOL, optimize=optimize).run().return_int(),
                    9,
                )


if __name__ == "__main__":
    unittest.main()
