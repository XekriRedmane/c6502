"""Runs `examples/zork_dump_buffer.c`, Zork I's dump_buffer_to_screen
and dump_buffer_to_printer, in the simulator against stand-ins for the
Monitor's output routines.

The stand-ins sit at the ROM's addresses. COUT at $FDED is
`JMP ($0036)`, as in the ROM, so it goes wherever CSW points. COUT1 at
$FDF0 records each character it's given at $3000 onward, and a printer
routine at $D000 records its characters at $3080 onward. Each keeps
its count in a byte at $3100 or $3101.

The test buffers "HELLO", points PRINTER_CSW at the printer routine
and CSW at COUT1, as the ROM leaves it, then calls
dump_buffer_to_printer twice and dump_buffer_to_screen once.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.asm_compare import REPO

_EXAMPLE = REPO / "examples" / "zork_dump_buffer.c"

_MAIN = (
    "int main(void) {\n"
    "    dump_buffer_to_printer();\n"
    "    dump_buffer_to_printer();\n"
    "    dump_buffer_to_screen();\n"
    "    return 0;\n"
    "}\n"
)

_COUT = 0xFDED
_COUT1 = 0xFDF0
_PRINTER = 0xD000
_SCREEN_LOG, _SCREEN_COUNT = 0x3000, 0x3100
_PRINTER_LOG, _PRINTER_COUNT = 0x3080, 0x3101

_CSW, _BUFF_END, _PRINTER_CSW = 0x36, 0xEB, 0xEE
_BUFF_AREA, _SLOT1_SCRATCH = 0x0200, 0x0779

_TEXT = b"HELLO"


_Y_SAVE = 0x3102


def _recorder(log: int, count: int) -> bytes:
    """STY save ; LDY count ; STA log,Y ; INC count ; LDY save ; RTS

    It leaves X and Y as it found them, as COUT1 does, since
    <apple2/monitor_io.h> declares COUT1 `preserves_x` and
    `preserves_y` and the compiled loop keeps its index in X."""
    return bytes([
        0x8C, _Y_SAVE & 0xFF, _Y_SAVE >> 8,
        0xAC, count & 0xFF, count >> 8,
        0x99, log & 0xFF, log >> 8,
        0xEE, count & 0xFF, count >> 8,
        0xAC, _Y_SAVE & 0xFF, _Y_SAVE >> 8,
        0x60,
    ])


class TestZorkDumpBuffer(unittest.TestCase):

    def _run(self, optimize: bool):
        src = _EXAMPLE.read_text() + _MAIN
        sim = build_sim(src, optimize=optimize)
        mem = sim.mpu.memory
        mem[_COUT:_COUT + 3] = bytes([0x6C, _CSW, 0x00])  # JMP ($0036)
        rec = _recorder(_SCREEN_LOG, _SCREEN_COUNT)
        mem[_COUT1:_COUT1 + len(rec)] = rec
        rec = _recorder(_PRINTER_LOG, _PRINTER_COUNT)
        mem[_PRINTER:_PRINTER + len(rec)] = rec
        mem[_SCREEN_COUNT] = mem[_PRINTER_COUNT] = 0
        mem[_BUFF_AREA:_BUFF_AREA + len(_TEXT)] = _TEXT
        mem[_BUFF_END] = len(_TEXT)
        mem[_PRINTER_CSW], mem[_PRINTER_CSW + 1] = _PRINTER & 0xFF, _PRINTER >> 8
        mem[_CSW], mem[_CSW + 1] = _COUT1 & 0xFF, _COUT1 >> 8
        return sim.run()

    def test_output(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = self._run(optimize)
                mem = result.memory
                printed = bytes(
                    mem[_PRINTER_LOG:_PRINTER_LOG + mem[_PRINTER_COUNT]]
                )
                shown = bytes(
                    mem[_SCREEN_LOG:_SCREEN_LOG + mem[_SCREEN_COUNT]]
                )
                # The printer gets ctrl-I "80N" once, then the buffer
                # each time.
                self.assertEqual(
                    printed, bytes([0x09, 0xB8, 0xB0, 0xCE]) + _TEXT * 2,
                )
                self.assertEqual(shown, _TEXT)
                self.assertEqual(mem[_SLOT1_SCRATCH], 0x91)
                # Only dump_buffer_to_screen empties the buffer.
                self.assertEqual(mem[_BUFF_END], 0)
                # CSW is back at COUT1, and PRINTER_CSW got CSW's
                # value from inside the call.
                self.assertEqual(mem[_CSW] | mem[_CSW + 1] << 8, _COUT1)
                self.assertEqual(
                    mem[_PRINTER_CSW] | mem[_PRINTER_CSW + 1] << 8,
                    _PRINTER,
                )


if __name__ == "__main__":
    unittest.main()
