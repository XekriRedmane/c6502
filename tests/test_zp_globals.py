"""Tests for zero-page globals (`[[c6502::zeropage]]`,
`cpp/src/passes/zp_globals.h`).

Covers simulator runs of initialization and aliasing in both
pipelines, of busy globals promoted under `-O`, and of linked units
that share zero-page objects, including a promoted object that
doesn't fit and goes back. The tests of placement, the generated
code, promotion, and the link's placement and errors are doctest
cases in cpp/tests/test_zp_globals_source.cpp.
"""
from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


# A pointer and a counter used in a loop, a constant, an array, and a
# variable read once.
_PROMOTE = (
    "#include <stdint.h>\n"
    "uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
    "uint8_t *p = buf;\n"
    "uint16_t total;\n"
    "const uint8_t step = 1;\n"
    "uint8_t once = 9;\n"
    "int main(void) {\n"
    "    for (uint8_t i = 0; i < 8; i += step) total += p[i] + buf[i];\n"
    "    return total + once;\n"
    "}\n"
)


@needs_c6502cpp
class TestPromotion(unittest.TestCase):
    """`-O` moves busy scalar globals into zero page."""

    def test_runs(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(_PROMOTE, optimize=optimize).run()
                self.assertEqual(got.return_int(), 2 * 36 + 9)


@needs_c6502cpp
class TestSim(unittest.TestCase):

    def _both(self, src: str, want: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, want)

    def test_initial_values(self) -> None:
        src = (
            "struct s { unsigned char a; unsigned int b; };\n"
            "unsigned char tab [[c6502::zeropage]][3] = {7, 8, 9};\n"
            "struct s st [[c6502::zeropage]] = {1, 300};\n"
            "unsigned long l [[c6502::zeropage]] = 70000;\n"
            "unsigned char z [[c6502::zeropage]];\n"
            "unsigned char other = 5;\n"
            "unsigned char *ptr [[c6502::zeropage]] = &other;\n"
            "int main(void) {\n"
            "    if (tab[0] + tab[1] + tab[2] != 24) return 1;\n"
            "    if (st.a != 1 || st.b != 300) return 2;\n"
            "    if (l != 70000) return 3;\n"
            "    if (z != 0) return 4;\n"
            "    if (*ptr != 5) return 5;\n"
            "    return 0;\n"
            "}\n"
        )
        self._both(src, 0)

    def test_write_through_pointer_to_zp_global(self) -> None:
        # `n` is written through a pointer, so a cached copy of it
        # can't be reused across the store.
        src = (
            "unsigned char n [[c6502::zeropage(0xA0)]] = 1;\n"
            "unsigned char *p [[c6502::zeropage]];\n"
            "int main(void) {\n"
            "    unsigned char before = n;\n"
            "    p = &n;\n"
            "    *p = 5;\n"
            "    return before * 10 + n;\n"
            "}\n"
        )
        self._both(src, 15)

    def test_pointer_global_walks_buffer(self) -> None:
        src = (
            "unsigned char buf[6] = {1, 2, 3, 4, 5, 6};\n"
            "unsigned char *p [[c6502::zeropage(0xBA)]];\n"
            "unsigned char sum [[c6502::zeropage]];\n"
            "void add(unsigned char k) { sum += p[k]; }\n"
            "int main(void) {\n"
            "    p = buf;\n"
            "    for (unsigned char i = 0; i < 6; i++) add(i);\n"
            "    p = buf + 3;\n"
            "    add(0);\n"
            "    return sum;\n"
            "}\n"
        )
        self._both(src, 21 + 4)

    def test_block_static_counter(self) -> None:
        src = (
            "unsigned char tick(void) {\n"
            "    static unsigned char c [[c6502::zeropage]] = 10;\n"
            "    return ++c;\n"
            "}\n"
            "int main(void) { tick(); tick(); return tick(); }\n"
        )
        self._both(src, 13)

    def test_global_in_pool_range_survives_calls(self) -> None:
        # The global sits where a function's locals would otherwise
        # go, and keeps its value across calls that use locals.
        src = (
            "unsigned int keep [[c6502::zeropage(0x80)]] = 0;\n"
            "unsigned int f(unsigned int a) {\n"
            "    unsigned int x = a * 5, y = a + 7;\n"
            "    return x ^ y;\n"
            "}\n"
            "int main(void) {\n"
            "    keep = 1234;\n"
            "    unsigned int r = f(3) + f(4);\n"
            "    return keep - 1234 + (r == ((15u ^ 10u) + (20u ^ 11u)));\n"
            "}\n"
        )
        self._both(src, 1)


_HEADER = (
    "#include <stdint.h>\n"
    "extern uint8_t *src [[c6502::zeropage]];\n"
    "extern uint8_t total [[c6502::zeropage]];\n"
    "extern uint16_t big [[c6502::zeropage(0x90)]];\n"
    "uint8_t sum(uint8_t n);\n"
)
# Defines `src`, `total`, and `big`, and calls `sum`.
_UNIT_A = _HEADER + (
    "uint8_t *src [[c6502::zeropage]];\n"
    "uint8_t total [[c6502::zeropage]] = 5;\n"
    "uint16_t big [[c6502::zeropage(0x90)]] = 1000;\n"
    "uint8_t buf[4] = {1, 2, 3, 4};\n"
    "int main(void) { src = buf; return sum(4) + big; }\n"
)
# Defines `count`, which it places at $26 on its own, where unit A
# puts `src`; and reads and writes A's objects.
_UNIT_B = _HEADER + (
    "uint8_t count [[c6502::zeropage]] = 2;\n"
    "uint8_t sum(uint8_t n) {\n"
    "    for (uint8_t i = 0; i < n; i++) total += src[i] * count;\n"
    "    big += total;\n"
    "    return total;\n"
    "}\n"
)


@needs_c6502cpp
class TestLink(unittest.TestCase):
    """Linking places the zero-page objects of every unit
    for the whole program and merges their `__zp_init`s."""

    def _link(self, *sources: str) -> tuple[int, str, str]:
        """Compiles each source with `-O` to an object and links them.
        Returns the exit code, the linked program as text
        (`link_helpers.linked_text`), and stderr."""
        from tests.link_helpers import (
            compile_object, link_capture_stderr, linked_text,
        )
        with tempfile.TemporaryDirectory() as tmp:
            paths = []
            for k, src in enumerate(sources):
                path = Path(tmp) / f"u{k}.o"
                self.assertEqual(compile_object(src, path), 0)
                paths.append(path)
            out = Path(tmp) / "out"
            rc, err = link_capture_stderr(paths, out)
            text = linked_text(out) if rc == 0 else ""
        return rc, text, err

    def test_linked_program_runs(self) -> None:
        from sim.linked import build_linked_sim
        rc, text, err = self._link(_UNIT_A, _UNIT_B)
        self.assertEqual(rc, 0, err)
        # total = 5 + (1+2+3+4) * 2 = 25, big = 1000 + 25.
        self.assertEqual(build_linked_sim(text.exe).run().return_int(),
                         25 + 1025)

    # Unit A leaves 4 bytes below $80; unit B promotes three busy
    # 2-byte counters, so one goes back in the data segment.
    _FULL = (
        "#include <stdint.h>\n"
        "uint8_t fill [[c6502::zeropage]][86];\n"
        "uint16_t count(void);\n"
        "int main(void) { fill[85] = 3; return count() + fill[85]; }\n"
    )
    _COUNTERS = (
        "#include <stdint.h>\n"
        "uint16_t a = 1, b = 2, c = 3;\n"
        "uint16_t count(void) {\n"
        "    for (uint8_t i = 0; i < 10; i++) { a += 1; b += a; c += b; }\n"
        "    return a + b + c;\n"
        "}\n"
    )

    def test_promoted_object_that_doesnt_fit_runs(self) -> None:
        from sim.linked import build_linked_sim
        rc, text, err = self._link(self._FULL, self._COUNTERS)
        self.assertEqual(rc, 0, err)
        a, b, c = 1, 2, 3
        for _ in range(10):
            a += 1
            b += a
            c += b
        self.assertEqual(build_linked_sim(text.exe).run().return_int(),
                         (a + b + c + 3) & 0xFFFF)


if __name__ == "__main__":
    unittest.main()
