"""Simulator runs of programs that reserve the bytes the classic
layout of the runtime's zero page uses, which check that the moved
block leaves those bytes alone.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6), and the tests of where SSP, FP,
HARGS, and DPTR go, how HARGS is sized, the EQU lines that bind them,
and linking are in cpp/tests/test_runtime_layout_source.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim


# The Monitor's zero-page bytes in the unenhanced Apple IIe F8 ROM,
# $1F through $49 (one array, so they're reserved as one object).
_MONITOR = (
    "extern volatile unsigned char monitor_zp "
    "[[c6502::address(0x1F)]][0x2B];\n"
)


class TestSim(unittest.TestCase):

    _SRC = (
        "unsigned char buf[8];\n"
        "unsigned long lm(unsigned long a, unsigned long b) "
        "{ return a * b / 7; }\n"
        "int sum(unsigned char *p, unsigned char n) {\n"
        "  int s = 0;\n"
        "  while (n--) s += *p++;\n"
        "  return s;\n"
        "}\n"
        "int rec(int n) { return n ? n + rec(n - 1) : 0; }\n"
        "int main(void) {\n"
        "  for (unsigned char i = 0; i < 8; i++) buf[i] = i * 3;\n"
        "  float f = (float)sum(buf, 8);\n"
        "  int g = (int)f;\n"
        "  return (int)(lm(100000, 3) & 0x7FFF) + g + rec(10);\n"
        "}\n"
    )
    _WANT = ((100000 * 3 // 7) & 0x7FFF) + 84 + 55

    def _run(self, src: str, optimize: bool):
        sim = build_sim(src, optimize=optimize)
        pattern = {a: (0xA5 ^ a) & 0xFF for a in range(0x1F, 0x4A)}
        for a, v in pattern.items():
            sim.mpu.memory[a] = v
        result = sim.run()
        changed = [a for a, v in pattern.items() if result.memory[a] != v]
        return result.return_int(), changed

    def test_monitor_bytes_untouched(self) -> None:
        # Pointer accesses (DPTR), recursion (SSP, FP), a 32-bit
        # multiply and divide (6502 helpers with HARGS scratch), and an
        # int / float round trip (Python hooks) all run with the block
        # moved clear of $1F-$49.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got, changed = self._run(_MONITOR + self._SRC, optimize)
                self.assertEqual(got, self._WANT)
                self.assertEqual(changed, [])

    def test_classic_layout_writes_those_bytes(self) -> None:
        # Without the reservation, the unoptimized build writes into
        # $1F-$49 (the top of HARGS and DPTR are there), which is what
        # the test above checks doesn't happen. The optimized build
        # keeps its pointers in zero-page locals instead of DPTR, and
        # its helpers stay below $1F, so it doesn't.
        got, changed = self._run(self._SRC, optimize=False)
        self.assertEqual(got, self._WANT)
        self.assertNotEqual(changed, [])


class TestMonitorHeaders(unittest.TestCase):
    """`<apple2/monitor_io.h>` reserves the zero page of the Monitor
    routines a program calls, and `<apple2/interactive_monitor.h>` adds the
    bytes only the Monitor's interactive part uses."""

    # The bytes of the routines a program calls: text, keyboard, line
    # input, lo-res graphics, the IRQ entry, and INIT.
    _ROUTINE_BYTES = (
        list(range(0x1F, 0x2F)) + [0x30, 0x32, 0x33]
        + list(range(0x35, 0x3A)) + [0x45, 0x48]
    )
    # Every byte the Monitor uses.
    _ALL_BYTES = [0x00, 0x01] + list(range(0x1F, 0x4A))

    _HEADERS = {
        "monitor_io.h": ("#include <apple2/monitor_io.h>\n", _ROUTINE_BYTES),
        "interactive_monitor.h":
            ("#include <apple2/interactive_monitor.h>\n", _ALL_BYTES),
    }

    def test_reserved_bytes_untouched(self) -> None:
        for header, (include, reserved) in self._HEADERS.items():
            for optimize in (False, True):
                with self.subTest(header, optimize=optimize):
                    sim = build_sim(
                        include + TestSim._SRC, optimize=optimize,
                    )
                    pattern = {a: (0x5A ^ a) & 0xFF for a in reserved}
                    for a, v in pattern.items():
                        sim.mpu.memory[a] = v
                    result = sim.run()
                    self.assertEqual(result.return_int(), TestSim._WANT)
                    self.assertEqual(
                        [a for a, v in pattern.items()
                         if result.memory[a] != v],
                        [],
                    )


if __name__ == "__main__":
    unittest.main()
