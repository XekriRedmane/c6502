"""Tests for <stdbit.h>, C23's bit and byte utilities (C23 §7.18;
builtin_functions.py, docs/stdbit.md).

The functions a program reaches through their addresses are run in the
simulator, and so are all 256 values of an `unsigned char` and a sweep
of edge and random values of the wider types, for every function in its
type-generic form and its form with a suffix, in both pipelines and
against `builtin_functions.stdbit`.

cpp/tests/test_stdbit.cpp checks `stdbit` in cpp/src/builtin_functions.h
against a second model written from the standard's definitions, which
walks the bits by index, and holds the tests of the folding of constant
arguments, the header's macros and declarations, and the code a few
calls compile to.
"""
from __future__ import annotations

import random
import unittest
from pathlib import Path

import builtin_functions as bf
from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


_TYPES = {
    "uc": ("unsigned char", 1),
    "us": ("unsigned short", 2),
    "ui": ("unsigned int", 2),
    "ul": ("unsigned long", 4),
    "ull": ("unsigned long long", 8),
}


def _result_type(operation: str, value_type: str) -> str:
    if operation in ("bit_floor", "bit_ceil"):
        return value_type
    if operation == "has_single_bit":
        return "bool"
    return "unsigned int"


def _sweep(width: int) -> list[int]:
    """Edge values of a `width`-bit unsigned type and a few random
    ones: 0, 1, the all-ones value, each power of 2 and its neighbors
    for every third exponent, and the top bit's neighbors."""
    bits = 8 * width
    top = 1 << (bits - 1)
    values = {0, 1, 2, 3, (1 << bits) - 1, (1 << bits) - 2, top,
              top - 1, top + 1}
    for k in range(0, bits, 3):
        values |= {1 << k, ((1 << k) - 1), ((1 << k) + 1) & ((1 << bits) - 1)}
    rnd = random.Random(width)
    values |= {rnd.getrandbits(bits) for _ in range(6)}
    return sorted(values)


@needs_c6502cpp
class TestFunctions(unittest.TestCase):
    """The functions with a suffix as functions (C23 §7.1.4)."""

    _SRC = """
#include <stdbit.h>
typedef unsigned (*count_fn)(unsigned char);
static count_fn counts[] = { &stdc_leading_zeros_uc, stdc_count_ones_uc };
unsigned char (*floor_fn)(unsigned char) = stdc_bit_floor_uc;
int main(void) {
    unsigned r = counts[0](0x10) * 100;       /* 3 */
    r += counts[1](0xF0) * 10;               /* 4 */
    r += (stdc_trailing_zeros_ul)(0x100);    /* 8 */
    r += floor_fn(100) == 64;                /* 1 */
#undef stdc_bit_width_ui
    r += stdc_bit_width_ui(0x1000) * 1000;   /* 13 */
    return r;
}
"""

    def test_runs_in_both_pipelines(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(self._SRC, optimize=optimize)
                result = sim.run(max_cycles=2_000_000)
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), 13349)

    def test_two_units_each_have_a_copy(self) -> None:
        import tempfile
        from binutils import elf as E
        from sim.linked import build_linked_sim
        from tests.link_helpers import compile_object, link
        a = (
            "#include <stdbit.h>\n"
            "unsigned (*pa)(unsigned char) = stdc_count_ones_uc;\n"
            "unsigned a(void) { return (stdc_count_ones_uc)(7); }\n"
        )
        b = (
            "#include <stdbit.h>\nunsigned a(void);\n"
            "unsigned (*pb)(unsigned char) = stdc_count_ones_uc;\n"
            "int main(void) { return a() * 10 + (stdc_count_ones_uc)(3); }\n"
        )
        with tempfile.TemporaryDirectory() as d:
            d = Path(d)
            for name, src in (("a", a), ("b", b)):
                self.assertEqual(compile_object(src, d / f"{name}.o"), 0)
            self.assertEqual(link([d / "a.o", d / "b.o"], d / "prog"), 0)
            exe = E.read_file(d / "prog")
            self.assertEqual(build_linked_sim(exe).run().return_int(), 32)
            copies = [
                s for s in exe.symbols if s.name.startswith("stdc_count_ones_uc")
            ]
            self.assertEqual(len(copies), 2)


def _program(suffix: str | None, tname: str, values: list[int],
             operations) -> str:
    """A program that stores `stdc_<op>` (or its form with `suffix`) of
    each of `values` in an array `out_<op>`."""
    n = len(values)
    lines = ["#include <stdbit.h>"]
    lines.append(
        f"static const {tname} in[{n}] = "
        f"{{{', '.join(f'{v}ull' for v in values)}}};"
    )
    for op in operations:
        lines.append(f"{_result_type(op, tname)} out_{op}[{n}];")
    lines.append("int main(void) {")
    lines.append(f"    for (unsigned int i = 0; i < {n}; i++) {{")
    lines.append(f"        {tname} v = in[i];")
    for op in operations:
        fn = f"stdc_{op}_{suffix}" if suffix else f"stdc_{op}"
        lines.append(f"        out_{op}[i] = {fn}(v);")
    lines += ["    }", "    return 0;", "}"]
    return "\n".join(lines) + "\n"


@needs_c6502cpp
class TestSimulator(unittest.TestCase):
    """Every function on every value of an `unsigned char` and a sweep
    of each wider type, against `builtin_functions.stdbit`, in both
    pipelines. Each 8-byte program has half the functions, since the
    unoptimized pipeline's frame for all 14 would pass 253 bytes."""

    def _check_sweep(self, suffix: str) -> None:
        tname, width = _TYPES[suffix]
        values = list(range(256)) if width == 1 else _sweep(width)
        ops = bf.STDBIT_OPERATIONS
        groups = [ops[:7], ops[7:]] if width == 8 else [ops]
        for form in (None, suffix):
            for operations in groups:
                src = _program(form, tname, values, operations)
                for optimize in (False, True):
                    sim = build_sim(src, optimize=optimize)
                    result = sim.run(max_cycles=60_000_000)
                    self.assertFalse(result.timed_out)
                    mem = sim.mpu.memory
                    for op in operations:
                        size = {
                            "bit_floor": width, "bit_ceil": width,
                            "has_single_bit": 1,
                        }.get(op, 2)
                        base = sim.symbols[f"out_{op}"]
                        got = [
                            int.from_bytes(
                                bytes(mem[base + size * i:
                                          base + size * (i + 1)]),
                                "little",
                            )
                            for i in range(len(values))
                        ]
                        want = [bf.stdbit(op, v, 8 * width) for v in values]
                        with self.subTest(
                            op=op, form=form or "generic",
                            optimize=optimize,
                        ):
                            self.assertEqual(got, want)

    def test_unsigned_char(self) -> None:
        self._check_sweep("uc")

    def test_unsigned_short(self) -> None:
        self._check_sweep("us")

    def test_unsigned_int(self) -> None:
        self._check_sweep("ui")

    def test_unsigned_long(self) -> None:
        self._check_sweep("ul")

    def test_unsigned_long_long(self) -> None:
        self._check_sweep("ull")


if __name__ == "__main__":
    unittest.main()
