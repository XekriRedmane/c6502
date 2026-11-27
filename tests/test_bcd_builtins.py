"""Tests for the packed BCD builtins, `__c6502_bcd_add` and
`__c6502_bcd_sub` (builtin_functions.py, docs/builtins.md).

Both pipelines are checked here for the values the simulator computes.
The tests of the arithmetic, of the constants the optimizer folds, and
of the code the back end writes are doctest cases in
cpp/tests/test_bcd_builtins.cpp, which runs the programs below on the
TAC interpreter too.
"""
from __future__ import annotations

import random
import unittest

import builtin_functions as bf
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac
from tests.cpp_build import needs_c6502cpp


# Programs whose `main` returns a 16-bit value. Each runs in the
# simulator in both pipelines, and cpp/tests/test_bcd_builtins.cpp has
# a copy of each that it runs on the TAC interpreter.
_PROGRAMS = [
    ("byte_add",
     "#include <stdint.h>\nuint8_t a = 0x38, b = 0x45;"
     "int main(void) { return __c6502_bcd_add(a, b); }", 0x83),
    ("byte_wraps",
     "#include <stdint.h>\nuint8_t a = 0x99, b = 0x01;"
     "int main(void) { return __c6502_bcd_add(a, b); }", 0x00),
    ("byte_borrows",
     "#include <stdint.h>\nuint8_t a = 0x00, b = 0x01;"
     "int main(void) { return __c6502_bcd_sub(a, b); }", 0x99),
    ("word_carries",
     "#include <stdint.h>\nuint16_t a = 0x0999;"
     "int main(void) { return __c6502_bcd_add(a, 1); }", 0x1000),
    ("word_borrows",
     "#include <stdint.h>\nuint16_t a = 0x1000;"
     "int main(void) { return __c6502_bcd_sub(a, 0x0001); }", 0x0999),
    ("mixed_widths",
     "#include <stdint.h>\nuint8_t a = 0x99; uint16_t b = 0x0901;"
     "int main(void) { return __c6502_bcd_add(a, b); }", 0x1000),
    ("long_low",
     "#include <stdint.h>\nuint32_t s = 0x00999950;"
     "int main(void) { uint32_t r = __c6502_bcd_add(s, 0x0250);"
     " return (uint16_t)r; }", 0x0200),
    ("long_high",
     "#include <stdint.h>\nuint32_t s = 0x00999950;"
     "int main(void) { uint32_t r = __c6502_bcd_add(s, 0x0250);"
     " return (uint16_t)(r >> 16); }", 0x0100),
    ("long_wraps",
     "#include <stdint.h>\nuint32_t s = 0x99999999;"
     "int main(void) { s = __c6502_bcd_add(s, 1);"
     " return (uint16_t)s | (uint16_t)(s >> 16); }", 0x0000),
    ("long_borrows",
     "#include <stdint.h>\nuint32_t s = 0;"
     "int main(void) { s = __c6502_bcd_sub(s, 1);"
     " return (uint16_t)(s >> 8); }", 0x9999),
    ("as_argument",
     "#include <stdint.h>\nuint8_t a = 0x19;"
     "int twice(uint16_t v) { return v * 2; }"
     "int main(void) { return twice(__c6502_bcd_add(a, 0x0001)); }", 0x40),
    ("nested",
     "#include <stdint.h>\nuint16_t a = 0x0450, b = 0x0275;"
     "int main(void) { return __c6502_bcd_sub("
     "__c6502_bcd_add(a, b), __c6502_bcd_add(b, 0x0005)); }", 0x0445),
    ("struct_and_array",
     "#include <stdint.h>\n"
     "struct s { uint8_t tag; uint16_t score; } p = {1, 0x0995};"
     "uint16_t t[3] = {0x0001, 0x0010, 0x0100};"
     "int main(void) { for (uint8_t i = 0; i < 3; i++)"
     " p.score = __c6502_bcd_add(p.score, t[i]);"
     " t[1] = __c6502_bcd_sub(p.score, t[2]);"
     " return t[1]; }", 0x1006),
    ("invalid_digits",
     "#include <stdint.h>\nuint8_t a = 0x0F, b = 0x0F;"
     "int main(void) { return __c6502_bcd_add(a, b)"
     " | __c6502_bcd_sub(b, 0x1A) << 8; }",
     bf.bcd_add(0x0F, 0x0F, 1) | bf.bcd_sub(0x0F, 0x1A, 1) << 8),
    ("register_param",
     "#include <stdint.h>\n"
     "uint8_t f(register uint8_t x) { return __c6502_bcd_add(x, 0x01); }"
     "int main(void) { return f(0x09) + f(0x19); }", 0x30),
]


def _sweep_program(op: str, width: int, pairs) -> tuple[str, int]:
    """A program that applies `op` to each pair in `pairs` through
    arrays of `width`-byte operands, and returns a 16-bit hash of the
    results. Returns the source and the hash the arithmetic gives."""
    ctype = {1: "uint8_t", 2: "uint16_t", 4: "uint32_t"}[width]
    fold = bf.bcd_add if op == "add" else bf.bcd_sub
    xs = ", ".join(f"{a:#x}" for a, _ in pairs)
    ys = ", ".join(f"{b:#x}" for _, b in pairs)
    n = len(pairs)
    src = (
        "#include <stdint.h>\n"
        f"{ctype} xs[{n}] = {{{xs}}}; {ctype} ys[{n}] = {{{ys}}};"
        f"{ctype} out[{n}];"
        "int main(void) { uint16_t h = 0;"
        f" for (uint8_t i = 0; i < {n}; i++) {{"
        f" out[i] = __c6502_bcd_{op}(xs[i], ys[i]);"
        f" {ctype} r = out[i];"
        " h = (h << 3 | h >> 13) ^ (uint16_t)r ^ (uint16_t)(r >> 8);"
        " }"
        " return h; }"
    )
    h = 0
    mask = (1 << 8 * width) - 1
    for a, b in pairs:
        r = fold(a, b, width) & mask
        h = ((h << 3 | h >> 13) & 0xFFFF) ^ (r & 0xFFFF) ^ ((r >> 8) & 0xFFFF)
    return src, h


def _sweeps():
    rng = random.Random(6502)
    out = []
    for width in (1, 2, 4):
        for op in ("add", "sub"):
            pairs = [
                (rng.randrange(1 << 8 * width), rng.randrange(1 << 8 * width))
                for _ in range(24)
            ]
            out.append((f"{op}{width}", *_sweep_program(op, width, pairs)))
    return out


@needs_c6502cpp
class TestSimulator(unittest.TestCase):
    def _run(self, src: str, optimize: bool):
        sim = build_sim(src, optimize=optimize)
        result = sim.run(max_cycles=3_000_000)
        self.assertFalse(result.timed_out)
        # The program ends with the decimal flag clear.
        self.assertFalse(sim.mpu.p & sim.mpu.DECIMAL)
        return result

    def test_programs(self) -> None:
        for name, src, expected in _PROGRAMS:
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = self._run(src, optimize)
                    self.assertEqual(result.return_int() & 0xFFFF, expected)

    def test_random_operands_match_the_cpu_model(self) -> None:
        # Most of these operands have a digit above 9, so this covers
        # what the NMOS 6502 does with invalid BCD.
        for name, src, expected in _sweeps():
            with self.subTest(case=name, sim="tac"):
                self.assertEqual(_run_tac(src) & 0xFFFF, expected)
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = self._run(src, optimize)
                    self.assertEqual(result.return_int() & 0xFFFF, expected)


if __name__ == "__main__":
    unittest.main()
