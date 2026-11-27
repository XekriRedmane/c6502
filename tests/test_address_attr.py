"""Tests for `[[c6502::address(addr)]]` on functions and
`extern` objects (`cpp/src/passes/zp_globals.h`, "Fixed addresses outside
zero page").

Covers simulator runs against a stand-in routine and stand-in memory.
The tests of the type checker's rules, the generated `EQU` lines, the
conventions such a function may have, and links of units that use the
same fixed names are doctest cases in cpp/tests/test_address_attr.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim


class TestSim(unittest.TestCase):

    # A stand-in for a ROM routine, placed at $D000 before the run:
    #   CLC ; ADC #1 ; RTS
    _INC_ROUTINE = bytes([0x18, 0x69, 0x01, 0x60])

    # Swaps the bytes of the 2-byte argument in A (low) and X (high)
    # and returns the result the same way. $D0F0 is scratch.
    #   STA $D0F0 ; TXA ; LDX $D0F0 ; RTS
    _SWAP_ROUTINE = bytes([0x8D, 0xF0, 0xD0, 0x8A, 0xAE, 0xF0, 0xD0, 0x60])

    def test_call_fixed_routine(self) -> None:
        src = (
            "[[c6502::zp_abi, c6502::address(0xD000)]]\n"
            "extern unsigned char inc(register unsigned char c);\n"
            "int main(void) {\n"
            "  unsigned char t = 0;\n"
            "  for (unsigned char i = 0; i < 3; i++) t += inc(i);\n"
            "  return t;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                sim.mpu.memory[0xD000:0xD004] = self._INC_ROUTINE
                self.assertEqual(sim.run().return_int(), 1 + 2 + 3)

    def test_carry_flag_result(self) -> None:
        # A routine that reports its result in the carry flag:
        #   CMP #5 ; RTS        C set iff A >= 5
        src = (
            "[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0xD000)]]\n"
            "extern bool at_least_5(register unsigned char a);\n"
            "int main(void) {\n"
            "  int n = 0;\n"
            "  for (unsigned char i = 0; i < 9; i++)\n"
            "    if (at_least_5(i)) n += 10; else n += 1;\n"
            "  return n;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                sim.mpu.memory[0xD000:0xD003] = bytes([0xC9, 0x05, 0x60])
                self.assertEqual(sim.run().return_int(), 4 * 10 + 5)

    def test_two_byte_argument_and_result(self) -> None:
        src = (
            "[[c6502::zp_abi, c6502::address(0xD000)]]\n"
            "extern unsigned int swap(register unsigned int v);\n"
            "unsigned int k = 0x1234;\n"
            "int main(void) { return swap(k) + 1; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                sim.mpu.memory[0xD000:0xD008] = self._SWAP_ROUTINE
                self.assertEqual(sim.run().return_int(), 0x3412 + 1)

    def test_pointer_result(self) -> None:
        # A pointer comes back as an `unsigned int` does, low byte in
        # A and high byte in X:
        #   LDA #$34 ; LDX #$12 ; RTS
        src = (
            "[[c6502::zp_abi, c6502::address(0xD000)]]\n"
            "extern unsigned char *where(void);\n"
            "int main(void) { return (int)(unsigned int)where(); }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                sim.mpu.memory[0xD000:0xD005] = bytes(
                    [0xA9, 0x34, 0xA2, 0x12, 0x60],
                )
                self.assertEqual(sim.run().return_int(), 0x1234)

    def test_fixed_objects(self) -> None:
        src = (
            "extern volatile unsigned char port "
            "[[c6502::address(0xC800)]];\n"
            "extern unsigned int word [[c6502::address(0xC810)]];\n"
            "extern unsigned char tbl [[c6502::address(0xC820)]][4];\n"
            "int main(void) {\n"
            "  port = 5;\n"
            "  unsigned int t = word;\n"
            "  for (unsigned char i = 0; i < 4; i++) t += tbl[i];\n"
            "  tbl[0] = port + 1;\n"
            "  return t;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                mem = sim.mpu.memory
                mem[0xC810], mem[0xC811] = 0x34, 0x12
                mem[0xC820:0xC824] = bytes([1, 2, 3, 4])
                result = sim.run()
                self.assertEqual(result.return_int(), 0x1234 + 10)
                self.assertEqual(result.memory[0xC800], 5)
                self.assertEqual(result.memory[0xC820], 6)

    def test_poll_key(self) -> None:
        # The example in docs/attributes.md. Clearing the strobe on
        # one path used to leave A = 0 at the return under
        # -O, because two passes lost track of the `LDA #0`
        # inside a volatile `STA`.
        src = (
            "extern volatile unsigned char KBD "
            "[[c6502::address(0xC000)]];\n"
            "extern volatile unsigned char KBDSTRB "
            "[[c6502::address(0xC010)]];\n"
            "unsigned char poll_key(void) {\n"
            "  unsigned char k = KBD;\n"
            "  if (k & 0x80) KBDSTRB = 0;\n"
            "  return k;\n"
            "}\n"
            "int main(void) { return poll_key(); }\n"
        )
        for optimize in (False, True):
            for key in (0xC1, 0x41):
                with self.subTest(optimize=optimize, key=key):
                    sim = build_sim(src, optimize=optimize)
                    sim.mpu.memory[0xC000] = key
                    sim.mpu.memory[0xC010] = 0x55
                    result = sim.run()
                    self.assertEqual(result.return_int(), key)
                    self.assertEqual(
                        result.memory[0xC010],
                        0 if key & 0x80 else 0x55,
                    )

    def test_wait_through_pointer(self) -> None:
        # With no key in KBD the loop has to keep waiting, so the run
        # reaches the cycle cap. It used to test a stale A and exit.
        src = (
            "int main(void) {\n"
            "  while (!(*(volatile unsigned char *)0xC000 & 0x80)) ;\n"
            "  return 7;\n"
            "}\n"
        )
        for optimize in (False, True):
            for key, waits in ((0x00, True), (0xC1, False)):
                with self.subTest(optimize=optimize, key=key):
                    sim = build_sim(src, optimize=optimize)
                    sim.mpu.memory[0xC000] = key
                    result = sim.run(max_cycles=20_000)
                    self.assertEqual(result.timed_out, waits)
                    if not waits:
                        self.assertEqual(result.return_int(), 7)

    def test_address_of_fixed_object(self) -> None:
        src = (
            "extern unsigned char port [[c6502::address(0xC800)]];\n"
            "int main(void) { return (int)&port; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got & 0xFFFF, 0xC800)


if __name__ == "__main__":
    unittest.main()
