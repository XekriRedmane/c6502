"""Tests for the loop-counter-to-X promotion pass.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestLoopCounterToXSim(unittest.TestCase):

    def test_pivot_after_call_keeps_loop_finite(self):
        # After `JSR g`, `out2 = spr_w[k]` loads `LDX k` and runs on
        # into `DEC i`. The optimized build used to loop forever.
        src = r'''
        #include <stdint.h>
        uint8_t ent_y[8];
        uint8_t spr_w[8];
        volatile uint8_t out1, out2;
        uint8_t calls;
        void g(void) { calls++; }
        void f(uint8_t n, uint8_t k)
        {
            uint8_t i = n;
            do {
                out1 = ent_y[i];
                g();
                out2 = spr_w[k];
                i--;
            } while ((i & 0x80) == 0);
        }
        int main(void) { f(3, 1); return calls; }
        '''
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(src, optimize=optimize).run()
                self.assertFalse(r.timed_out)
                self.assertEqual(r.return_int(), 4)


@needs_c6502cpp
class TestPointerHighByteSim(unittest.TestCase):
    """`msg = msg + msg[0] + 1` steps the pointer's high byte with one
    `INC`, after `SEC; ADC (msg),Y` (passes/carry_in_add.py). The pass
    once took that byte for a loop counter and stepped it in X, while
    the reads through `(msg),Y` still found the old high byte in
    memory, so a step across a page read the byte 256 below."""

    _SRC = r'''
    #include <stdint.h>
    uint8_t buf[1100];
    uint8_t count;
    uint8_t calls;
    uint16_t seen;
    void ext(const uint8_t *p) { seen += p[1]; }
    void ext2(void) { calls++; }
    void walk(const uint8_t *msg) {
        while (*msg != 0) {
            ext(msg);
            count++;
            msg = msg + msg[0] + 1;
            if (count > 1 || *msg == 0)
                ext2();
        }
    }
    int main(void) {
        for (uint16_t i = 0; i < 3; i++) {
            buf[OFF + i * 255] = 254;
            for (uint16_t j = 1; j < 255; j++)
                buf[OFF + i * 255 + j] = (uint8_t)(i + 1);
        }
        walk(buf + OFF);
        return calls * 256 + seen * 16 + count;
    }
    '''

    def test_steps_across_pages(self):
        # Each string is 255 bytes long, so a step from any start other
        # than a page boundary crosses a page.
        for off in (0, 5, 200):
            for optimize in (False, True):
                with self.subTest(off=off, optimize=optimize):
                    src = self._SRC.replace("OFF", str(off))
                    r = build_sim(src, optimize=optimize).run()
                    self.assertFalse(r.timed_out)
                    self.assertEqual(r.return_int(), 2 * 256 + 6 * 16 + 3)


if __name__ == "__main__":
    unittest.main()
