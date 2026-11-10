"""Compares `examples/zork_get_object_addr.c` under --optimize with the
hand-written routine it translates, from the Zork I (r15) Z-machine
interpreter (https://github.com/XekriRedmane/zorki-r15-reveng).

The original computes `header + table_offset + 53 + 9 * obj` with the
multiply as `8x + x` (three ASL / ROL pairs and an add). It's run with
py65 on the same header and object numbers as the compiled function,
and both are timed from entry to RTS.

The compiled function is 44 instructions and 842 cycles over the six
calls, against the original's 30 and 580, so the 5% comparisons are
expected failures. The multiply is now `(x << 3) + x` inline, as in
the original, and its high byte takes the carry with `BCC` / `INC`
(`passes/carry_inc.py`). `z_header_addr` is placed in zero page at the
original's $BA (`__attribute__((zeropage(0xBA)))`), so it's read as the
`(zp),Y` base directly. Most of what remains comes from two things.

- The original relies on the header being page-aligned and adds only
  the pointer's high byte. The C adds the whole pointer.
- The original adds `+ 53` and the table offset into SCRATCH2 in
  place, carrying with `BCC` / `INC` and keeping the low byte in A
  between adds. c6502 adds the table offset and the product as full
  16-bit adds, and the `+ 53` reads the sum from a temp and writes
  it to HARGS, so its high byte isn't in place and keeps the `ADC
  #0` form. Putting that sum in HARGS from the start would need the
  return value's bytes assigned to HARGS before register
  allocation.

`test_no_regression` holds the current numbers as ceilings."""

from __future__ import annotations

import shutil
import unittest

from py65.devices.mpu6502 import MPU

import sim.harness as H
from tests.asm_compare import (
    REPO, assemble, call, count_function_instructions,
)

_EXAMPLE = REPO / "examples" / "zork_get_object_addr.c"
_HEADER_PAGE = 0x40          # the header at $4000
_TABLE_OFFSET = 0x02A6       # big-endian at header+$0A
_OBJECTS = [1, 2, 9, 100, 200, 255]

# Zero page as documented: Z_HEADER_ADDR $BA, SCRATCH2 $E6.
_ORIGINAL = [
    "STA $E6", "LDA #$00", "STA $E7", "LDA $E6",
    "ASL $E6", "ROL $E7", "ASL $E6", "ROL $E7", "ASL $E6", "ROL $E7",
    "CLC", "ADC $E6", "BCC continue", "INC $E7", "CLC",
    "continue: ADC #$35", "STA $E6", "BCC continue2", "INC $E7",
    "continue2: LDY #$0B", "LDA ($BA),Y", "CLC", "ADC $E6", "STA $E6",
    "DEY", "LDA ($BA),Y", "ADC $E7", "ADC $BB", "STA $E7", "RTS",
]
_ORIGIN = 0x1000


def _expected(obj: int) -> int:
    return (_HEADER_PAGE << 8) + _TABLE_OFFSET + 53 + 9 * obj


def _header(mem) -> None:
    base = _HEADER_PAGE << 8
    mem[base + 0x0A] = _TABLE_OFFSET >> 8
    mem[base + 0x0B] = _TABLE_OFFSET & 0xFF


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestGetObjectAddr(unittest.TestCase):

    @classmethod
    def setUpClass(cls) -> None:
        src = _EXAMPLE.read_text()
        cls.compiled_count = count_function_instructions(_EXAMPLE, "get_object_addr")
        cls.sim = H.build_sim(src, optimize=True)
        _, _, _, cls.zp_symbols = H.compile_to_asm(src, optimize=True)

    def _run_original(self, obj: int) -> tuple[int, int]:
        mpu = MPU()
        assemble(mpu, _ORIGINAL, _ORIGIN)
        _header(mpu.memory)
        mpu.memory[0xBA], mpu.memory[0xBB] = 0x00, _HEADER_PAGE
        mpu.a = obj
        cycles = call(mpu, _ORIGIN)
        return cycles, mpu.memory[0xE6] | (mpu.memory[0xE7] << 8)

    def _run_compiled(self, obj: int) -> tuple[int, int]:
        mpu, sym = self.sim.mpu, self.sim.symbols
        mem = mpu.memory
        _header(mem)
        mem[sym["z_header_addr"]] = 0x00
        mem[sym["z_header_addr"] + 1] = _HEADER_PAGE
        mem[self.zp_symbols["__zpabi_get_object_addr__obj"]] = obj
        cycles = call(mpu, sym["get_object_addr"])
        hargs = self.sim.runtime.symbols.get("HARGS", 0x04)
        return cycles, mem[hargs] | (mem[hargs + 1] << 8)

    def test_results_match_original(self) -> None:
        for obj in _OBJECTS:
            with self.subTest(obj=obj):
                self.assertEqual(self._run_original(obj)[1], _expected(obj))
                self.assertEqual(self._run_compiled(obj)[1], _expected(obj))

    @unittest.expectedFailure
    def test_size_within_five_percent(self) -> None:
        self.assertLessEqual(
            abs(self.compiled_count - len(_ORIGINAL)), 0.05 * len(_ORIGINAL),
            msg=f"compiled {self.compiled_count}, original {len(_ORIGINAL)}",
        )

    @unittest.expectedFailure
    def test_cycles_no_more_than_five_percent_over(self) -> None:
        original = sum(self._run_original(o)[0] for o in _OBJECTS)
        compiled = sum(self._run_compiled(o)[0] for o in _OBJECTS)
        self.assertLessEqual(
            compiled, 1.05 * original,
            msg=f"compiled {compiled}, original {original}",
        )

    def test_no_regression(self) -> None:
        compiled = sum(self._run_compiled(o)[0] for o in _OBJECTS)
        self.assertLessEqual(self.compiled_count, 44)
        self.assertLessEqual(compiled, 842)


if __name__ == "__main__":
    unittest.main()
