"""Tests for `passes.optimization.known_compare_thread`: a jump whose
target tests what the jump's own path decided goes past the test, a
conditional jump that the code before it decided becomes a `Jump` or
goes, and a conditional jump's target block is placed right after the
`Jump` that follows it."""

from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim
from tests.no_inlining import no_inlining


# The `if (x == 20 || x == 10) r = x == 20 ? 1 : 2;` shape the
# short-circuit fold leaves.


def _program(test: str, kind: str = "uint8_t") -> str:
    return (
        "#include <stdint.h>\n"
        f"volatile {kind} sink;\n"
        f"static uint8_t f({kind} t) {{\n"
        "    uint8_t r = 0;\n"
        f"    {test}\n"
        "    return r;\n"
        "}\n"
        "int main(void) {\n"
        "    uint16_t n = 0;\n"
        "    for (uint16_t v = 0; v < 600; v++)\n"
        f"        n = (uint16_t)(n * 3u + f(({kind})v));\n"
        "    return n;\n"
        "}\n"
    )


def _expected(test: str, kind: str) -> int:
    n = 0
    bits = 8 if kind == "uint8_t" else 16
    for v in range(600):
        t = v & ((1 << bits) - 1)
        n = (n * 3 + _model(test, t)) & 0xFFFF
    return n


_TESTS = {
    "or_then_test": (
        "if (t == 20 || t == 10) { sink = t; r = t == 20 ? 0x37 : 0x38; }",
        lambda t: (0x37 if t == 20 else 0x38) if t in (20, 10) else 0),
    "or_then_other_test": (
        "if (t == 20 || t == 10) { sink = t; r = t == 10 ? 1 : 2; }",
        lambda t: (1 if t == 10 else 2) if t in (20, 10) else 0),
    "three_way_or": (
        "if (t == 1 || t == 2 || t == 3) r = t == 2 ? 5 : t == 3 ? 6 : 7;",
        lambda t: {1: 7, 2: 5, 3: 6}.get(t, 0)),
    "and_not": (
        "if (t != 4 && t != 9) r = 1; else r = t == 4 ? 2 : 3;",
        lambda t: 1 if t not in (4, 9) else (2 if t == 4 else 3)),
    "changed_between": (
        "if (t == 20 || t == 10) { t = (uint8_t)(t + sink); "
        "r = t == 20 ? 1 : 2; }",
        None),
}


def _model(test, t):
    for body, model in _TESTS.values():
        if body == test:
            return model(t)
    raise KeyError(test)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
@no_inlining()
class TestRuns(unittest.TestCase):

    def test_values(self):
        for name, (body, model) in _TESTS.items():
            if model is None:
                continue
            for kind in ("uint8_t", "uint16_t"):
                for optimize in (False, True):
                    with self.subTest(shape=name, kind=kind,
                                      optimize=optimize):
                        r = build_sim(_program(body, kind),
                                      optimize=optimize).run()
                        self.assertEqual(r.return_int(),
                                         _expected(body, kind))

    def test_value_changed_in_between(self):
        # `sink` is 0 when f reads it here, so t keeps its value, but
        # the store and the read put the test after a write of t.
        body = _TESTS["changed_between"][0]
        src = _program(body).replace(
            "volatile uint8_t sink;", "uint8_t sink;")
        expected = 0
        for v in range(600):
            t = v & 0xFF
            r = (1 if t == 20 else 2) if t in (20, 10) else 0
            expected = (expected * 3 + r) & 0xFFFF
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(src, optimize=optimize).run()
                self.assertEqual(r.return_int(), expected)

    def test_second_compare_goes(self):
        # handle_timers in examples/lode_runner/timers.c: one compare
        # with 20 before the `if` and none in it.
        from compile import _run_source
        asm = _run_source("codegen", _program(
            "if (t == 20 || t == 10) r = t == 20 ? 0x37 : 0x38;"),
            optimize=True)
        start = asm.index("\nf:")
        body = asm[start:asm.index("\nmain:", start)]
        self.assertEqual(body.count("CMP   #$14"), 1)
        self.assertEqual(body.count("JMP"), 0)


if __name__ == "__main__":
    unittest.main()
