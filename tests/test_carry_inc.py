"""Tests for the carry-INC peephole (`passes/carry_inc.py`) and the
carry-byte pairs in move coalescing that set it up.

The peephole rewrites an in-place carry byte `LDA M; ADC #0; STA M`
to `BCC skip; INC M; skip:`, and a borrow byte `LDA M; SBC #0; STA
M` to `BCS skip; DEC M; skip:`, when A and every flag are dead after
the store. Coalescing puts both ends of such a byte in one slot,
including when the source is one byte of a multi-byte shift temp.

The sim tests run each add and subtract across the values where the
carry or borrow does and doesn't happen, since the two branches of
the rewrite are different code paths.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestCarryIncSim(unittest.TestCase):

    _BYTES = [0x00, 0x01, 0x7F, 0x80, 0xFE, 0xFF]
    _WORDS = [0x0000, 0x00FF, 0x0100, 0x7FFF, 0x80F0, 0xFF01, 0xFFFF]

    def _check(self, ctype, size, op, words, optimize) -> None:
        mask = (1 << (size * 8)) - 1
        cases = [(w, b) for w in words for b in self._BYTES]
        n = len(cases)
        ws = ", ".join(f"{w}UL" for w, _ in cases)
        bs = ", ".join(str(b) for _, b in cases)
        src = (
            f"{ctype} g;\n"
            f"{ctype} r[{n}];\n"
            f"{ctype} q[{n}];\n"
            f"{ctype} ws[{n}] = {{{ws}}};\n"
            f"unsigned char bs[{n}] = {{{bs}}};\n"
            f"void f(unsigned char c) {{ g {op}= c; }}\n"
            f"{ctype} h({ctype} t, unsigned char c) {{ return t {op} c; }}\n"
            "int main(void) {\n"
            f"    for (int i = 0; i < {n}; i++) {{\n"
            "        g = ws[i]; f(bs[i]); r[i] = g;\n"
            "        q[i] = h(ws[i], bs[i]);\n"
            "    }\n"
            "    return 0;\n}\n"
        )
        sim = build_sim(src, optimize=optimize)
        sim.run()
        mem = sim.mpu.memory
        for name in ("r", "q"):
            base = sim.symbols[name]
            for i, (w, b) in enumerate(cases):
                want = (w + b if op == "+" else w - b) & mask
                at = base + i * size
                got = int.from_bytes(bytes(mem[at:at + size]), "little")
                self.assertEqual(
                    got, want,
                    msg=f"{name}: {w:#x} {op} {b:#x}: got {got:#x}",
                )

    def test_uint_add_and_subtract(self) -> None:
        for optimize in (False, True):
            for op in "+-":
                with self.subTest(optimize=optimize, op=op):
                    self._check("unsigned int", 2, op, self._WORDS, optimize)

    def test_ulong_add_and_subtract(self) -> None:
        # The carry runs through bytes 1 and 2, which are followed by
        # another ADC and keep the ADC form, before the top byte.
        longs = [0x00000000, 0x00FFFFFF, 0x0000FFFF, 0xFFFFFFFF,
                 0x12345678, 0x01000000]
        for optimize in (False, True):
            for op in "+-":
                with self.subTest(optimize=optimize, op=op):
                    self._check("unsigned long", 4, op, longs, optimize)

    def test_times_nine(self) -> None:
        # `(x << 3) + x` puts the sum's high byte in the shift temp's
        # high byte, which the BCC / INC then updates.
        src = (
            "unsigned int r[256];\n"
            "unsigned int f(unsigned char x) { return x * 9; }\n"
            "int main(void) {\n"
            "    for (unsigned int i = 0; i < 256; i++) r[i] = f(i);\n"
            "    return 0;\n}\n"
        )
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        body = asm.split("\nf:")[1].split("RTS")[0]
        self.assertIn("BCC   .carry_inc@", body)
        sim = build_sim(src, optimize=True)
        sim.run()
        mem = sim.mpu.memory
        base = sim.symbols["r"]
        for i in range(256):
            got = mem[base + 2 * i] | (mem[base + 2 * i + 1] << 8)
            self.assertEqual(got, i * 9, msg=f"x={i}")


if __name__ == "__main__":
    unittest.main()
