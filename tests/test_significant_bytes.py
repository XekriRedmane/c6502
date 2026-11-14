"""Tests for `passes.significant_bytes` and the shift of a value with
one significant byte (`tac_to_asm._inline_left_shift_byte`), and the
closed form of the loop in Zork's `please_insert_save_diskette`."""

import shutil
import unittest

from compile import _run_stage
from preprocessor import preprocess
from sim.harness import build_sim


def _shift_program(c_type: str, counts: list[int]) -> str:
    """Each `(c_type)x << k` for `k` in `counts`, checked for every
    byte `x` against adding `1 << k` (found by doubling) for each
    step of `x`. `main` returns 0, or
    1000 * (index of the count + 1) + x at the first difference. Each
    count has a function of its own, which keeps the unoptimized
    frames small."""
    lines = [
        f"static {c_type} ref({c_type} e, unsigned char k) {{",
        "    while (k--) e = e + e;",
        "    return e;",
        "}",
    ]
    for i, k in enumerate(counts):
        lines += [
            f"static {c_type} s{k}(unsigned char x) "
            f"{{ return ({c_type})x << {k}; }}",
            f"static int check{k}(void) {{",
            "    unsigned char x = 0;",
            f"    {c_type} want = 0, step = ref(1, {k});",
            "    do {",
            f"        if (s{k}(x) != want) return {1000 * (i + 1)} + x;",
            "        want = want + step;",
            "    } while (++x != 0);",
            "    return 0;",
            "}",
        ]
    lines += ["int main(void) {", "    int r;"]
    for k in counts:
        lines.append(f"    if ((r = check{k}()) != 0) return r;")
    lines += ["    return 0;", "}"]
    return "\n".join(lines)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestWidenedByteShift(unittest.TestCase):

    def _check(self, src: str):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run(
                    max_cycles=50_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), 0)

    def test_every_16_bit_count(self):
        self._check(_shift_program(
            "unsigned int", [k for k in range(1, 16) if k % 8],
        ))

    def test_every_32_bit_count(self):
        self._check(_shift_program(
            "unsigned long", [k for k in range(1, 32) if k % 8],
        ))

    def test_through_and_and_right_shift(self):
        # `w & 0x3F` and `w >> 8` have one significant byte.
        src = """
        static unsigned int ref(unsigned int e, unsigned char k) {
            while (k--) e = e + e;
            return e;
        }
        static unsigned int masked(unsigned int w) { return (w & 0x3F) << 6; }
        static unsigned int top(unsigned int w) { return (w >> 8) << 5; }
        static int sgn(signed char c) { return (int)c << 5; }
        int main(void) {
            unsigned char i = 0;
            do {
                unsigned int w = ((unsigned int)i << 8)
                    | (unsigned char)(i * 7 + 3);
                if (masked(w) != ref(w & 0x3F, 6)) return 1;
                if (top(w) != ref(w >> 8, 5)) return 2;
            } while (++i != 0);
            if (sgn(-3) != -96) return 3;
            return 0;
        }
        """
        self._check(src)

    def test_no_helper_call(self):
        src = """
        unsigned int g;
        void f(unsigned char c) { g = (unsigned int)c << 5; }
        void h(unsigned char c) { g = 64 * (c & 7); }
        """
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _run_stage("codegen", preprocess(src), optimize=optimize)
                self.assertNotIn("asl16", asm)

    def test_sign_extended_byte_shifts_every_byte(self):
        # A negative `signed char` widens to a nonzero high byte.
        src = """
        unsigned int g;
        void f(signed char c) { g = (unsigned int)c << 5; }
        """
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        self.assertIn("asl16", asm)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestScratchClosedForm(unittest.TestCase):
    """`please_insert_save_diskette` sets SCRATCH1 to one less than 64
    times the position. The loop it was written with and the closed
    form agree for every byte the answer could hold."""

    def test_formula(self):
        for answer in range(256):
            loop = 0xFFFF
            for _ in range(answer & 0x07):
                loop = (loop + 64) & 0xFFFF
            closed = ((64 * (answer & 0x07)) - 1) & 0xFFFF
            self.assertEqual(loop, closed)

    def test_compiled(self):
        src = """
        #include <stdint.h>
        uint16_t S;
        uint8_t answer;
        static void by_loop(void) {
            S = 0xFFFF;
            for (uint8_t n = answer & 0x07; n != 0; n--)
                S += 64;
        }
        static void closed(void) {
            S = (64 * (answer & 0x07)) - 1;
        }
        int main(void) {
            uint8_t a = 0;
            do {
                answer = a;
                by_loop();
                uint16_t want = S;
                S = 0x1234;
                closed();
                if (S != want) return 1000 + a;
            } while (++a != 0);
            return 0;
        }
        """
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run()
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), 0)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestTruncatedRightShift(unittest.TestCase):
    """`(uint8_t)(w >> k)` computes only the low byte, from two bytes
    of `w`, for every count that needs two (16 and 32 bits),
    against the shift by a variable count."""

    def _program(self, c_type, counts):
        lines = [
            f"static unsigned char ref({c_type} w, unsigned char k) "
            "{ return (unsigned char)(w >> k); }",
        ]
        for i, k in enumerate(counts):
            lines += [
                f"static unsigned char s{k}({c_type} w) "
                f"{{ return (unsigned char)(w >> {k}); }}",
                f"static int check{k}(void) {{",
                "    unsigned char i = 0;",
                "    do {",
                f"        {c_type} w = ({c_type})i * 0x01010101UL ^ 0x5AC3E10FUL;",
                f"        if (s{k}(w) != ref(w, {k})) return {1000 * (i + 1)} + i;",
                "    } while (++i != 0);",
                "    return 0;",
                "}",
            ]
        lines += ["int main(void) {", "    int r;"]
        lines += [f"    if ((r = check{k}()) != 0) return r;" for k in counts]
        lines += ["    return 0;", "}"]
        return "\n".join(lines)

    def _check(self, src):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run(
                    max_cycles=50_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), 0)

    def test_16_bit(self):
        self._check(self._program("unsigned int", list(range(1, 8))))

    def test_signed_16_bit(self):
        self._check(self._program("int", list(range(1, 8))))

    def test_32_bit(self):
        self._check(self._program(
            "unsigned long", [k for k in range(1, 24) if k % 8],
        ))

    def test_no_helper_call(self):
        src = """
        unsigned int z;
        unsigned char o;
        void f(void) { o = (unsigned char)(z >> 5) & 0x1F; }
        """
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        self.assertNotIn("lsr16", asm)

if __name__ == "__main__":
    unittest.main()
