"""Tests for the code around arguments that arrive in A and X.

- A 2-byte register parameter's high byte stays in X, the register it
  arrives in (`optimizer._entry_register_limits`, per byte).
- Two byte arguments passed on in the other order swap through A and
  a slot (`redundant_load._swap_loads_ok`).
- A byte stored from A and loaded into X or Y is transferred
  (`redundant_load._redirect_index_load_to_transfer`), and a count down
  in Y that this leaves as `TYA; SEC; SBC #1; TAY` is `DEY`
  (`dec_register_peephole._match_transfer_back`).
- A transfer made again after a run of branches goes
  (`redundant_load._repeats_transfer`).
- `LDA abs,Y; TAX` is `LDX abs,Y` (`direct_index_load`).
- `x_save_slot_load` doesn't take `STX M; TAX` for X holding M.

Each compiled shape is checked, and each program runs in the
simulator in both pipelines.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


def _codegen(src: str) -> str:
    from compile import _run_source
    return _run_source("codegen", src, optimize=True)


def _body(asm: str, name: str) -> list[str]:
    """The instruction and local label lines of function `name`."""
    text = asm.split(f"\n{name}:\n")[1]
    out = []
    for line in text.splitlines():
        line = line.strip()
        if not line or line.startswith(";") or line == "SUBROUTINE":
            continue
        if line.endswith(":") and not line.startswith("."):
            break
        out.append(" ".join(line.split()))
    return out


@needs_c6502cpp
class TestCompiled(unittest.TestCase):
    """The code for each shape, from `-O`."""

    def test_high_byte_stays_in_x(self) -> None:
        src = ("unsigned char k;"
               "unsigned int widen(unsigned int y) { return y - k; }")
        self.assertEqual(_body(_codegen(src), "widen"), [
            "SEC", "SBC k", "BCS .carry_inc@0", "DEX", ".carry_inc@0:",
            "RTS",
        ])

    def test_arguments_swap(self) -> None:
        src = ("#include <stdint.h>\n"
               "void g(uint8_t a, uint8_t b);"
               "void f(uint8_t a, uint8_t b) { g(b, a); }")
        self.assertEqual(_body(_codegen(src), "f"), [
            "STA __local_f__a", "TXA", "LDX __local_f__a", "JMP g",
        ])

    def test_count_down_in_y(self) -> None:
        src = ("#include <stdint.h>\n"
               "void wait(uint8_t delay) {"
               "    register volatile uint8_t y = delay;"
               "    while (--y != 0) { }"
               "}")
        self.assertEqual(_body(_codegen(src), "wait"), [
            "TAY", ".loop@0_continue:", "DEY", "BNE .loop@0_continue",
            "RTS",
        ])


_PROGRAM = """
#include <stdint.h>
unsigned char k;
unsigned int widen(unsigned int y) { return y - k; }
uint16_t pair(uint8_t a, uint8_t b) { return (uint16_t)a << 8 | b; }
uint16_t swap(uint8_t a, uint8_t b) { return pair(b, a); }
uint8_t count(uint8_t d) {
    register uint8_t y = d;
    uint8_t n = 0;
    while (--y != 0) n++;
    return n;
}
int main(void) {
    k = 5;
    if (widen(3) != 0xFFFE) return 1;
    k = 0;
    if (widen(0x1234) != 0x1234) return 2;
    k = 0xFF;
    if (widen(0x100) != 1) return 3;
    if (widen(0xFF) != 0) return 4;
    if (swap(0x12, 0x34) != 0x3412) return 5;
    if (swap(0, 0xFF) != 0xFF00) return 6;
    if (count(1) != 0) return 7;
    if (count(5) != 4) return 8;
    if (count(0) != 255) return 9;
    return 0;
}
"""


@needs_c6502cpp
class TestSimulated(unittest.TestCase):
    def test_both_pipelines(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run()
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), 0)


if __name__ == "__main__":
    unittest.main()
