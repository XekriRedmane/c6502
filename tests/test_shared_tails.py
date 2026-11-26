"""cpp/src/passes/shared_tails.h: code that two functions of a unit end with
alike goes in a function of its own, which each jumps to.

`TestPass` runs the pass on hand-written asm. `TestPrograms` compiles
programs whose functions end alike, checks that a tail was shared, and
runs them in the simulator, compiled on their own and linked, where
the linker puts one function right before the tail and drops its
jump.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp



# `store` takes its multiplier in X, so that `v` arrives in its slot
# and each caller ends in the same stores into it, `STA v_0; LDA #0;
# STA v_1; LDX #3; JMP store`. Passed in A and X, `v` left only `LDX
# #0; JMP store`, which is too short to share.
_SRC = """
#include <stdint.h>
uint16_t acc;
uint8_t tab[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
void store(uint8_t m [[c6502::in_x]], uint16_t v) { acc = acc * m + v; }
void f1(uint8_t i) { store(3, tab[i]); }
void f2(uint8_t i) { store(3, (uint8_t)(tab[i] + 1)); }
void f3(uint8_t i) { store(3, (uint8_t)(tab[i] << 1)); }
int main(void) {
    f1(2); f2(5); f3(7); f1(0);
    return acc;
}
"""
_EXPECTED = (((3 * 3 + 7) * 3 + 16) * 3 + 1)


@needs_c6502cpp
class TestPrograms(unittest.TestCase):

    def test_shared_and_runs(self):
        from compile import _run_source
        asm = _run_source("codegen", _SRC, optimize=True)
        self.assertIn("@tail0:", asm)
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_SRC, optimize=optimize).run()
                self.assertEqual(result.return_int(), _EXPECTED)

    def test_linked_program_runs(self):
        from sim.linked import build_linked_sim
        from tests.link_helpers import compile_object, link, symbols
        from binutils import elf as E
        with tempfile.TemporaryDirectory() as tmp:
            obj = Path(tmp) / "a.o"
            exe = Path(tmp) / "a.out"
            self.assertEqual(compile_object(_SRC, obj), 0)
            self.assertEqual(link([obj], exe), 0)
            syms = symbols(exe)
            tails = [n for n in syms if "@tail" in n]
            self.assertTrue(tails)
            result = build_linked_sim(E.read_file(exe)).run()
            self.assertEqual(result.return_int(), _EXPECTED)


if __name__ == "__main__":
    unittest.main()
