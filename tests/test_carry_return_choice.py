"""Tests for `passes.carry_return_choice`, which returns a `static`
function's `bool` in the carry flag, without the annotation, when
every call to it suits the carry."""

import shutil
import unittest

from sim.harness import build_sim


def _chosen(src: str) -> set[str]:
    """The functions `choose_carry_returns` picks in `src`."""
    import compile as C
    from preprocessor import preprocess
    picked: set[str] = set()
    orig = C.choose_carry_returns

    def spy(tac, symbols):
        out = orig(tac, symbols)
        picked.update(out)
        return out

    C.choose_carry_returns = spy
    try:
        C._run_stage("codegen", preprocess(src), optimize=True)
    finally:
        C.choose_carry_returns = orig
    return picked


_HEAD = "#include <stdint.h>\n#include <stdbool.h>\nuint8_t g;\n"


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestChoice(unittest.TestCase):

    def test_branched_on(self):
        src = _HEAD + (
            "static bool small(uint8_t x) { return x < 10; }\n"
            "int main(void) { if (small(g)) g = 1; return g; }\n"
        )
        self.assertEqual(_chosen(src), {"small"})

    def test_result_kept_as_a_value(self):
        # `g = small(g)` would need `LDA #0; ROL A` after the call.
        src = _HEAD + (
            "static bool small(uint8_t x) { return x < 10; }\n"
            "int main(void) { if (small(g)) g++; g = small(g); return g; }\n"
        )
        self.assertEqual(_chosen(src), set())

    def test_external_linkage(self):
        # Another unit may call it, and it would see the result in A.
        src = _HEAD + (
            "bool small(uint8_t x) { return x < 10; }\n"
            "int main(void) { if (small(g)) g = 1; return g; }\n"
        )
        self.assertEqual(_chosen(src), set())

    def test_address_taken(self):
        src = _HEAD + (
            "static bool small(uint8_t x) { return x < 10; }\n"
            "bool (*fp)(uint8_t) = small;\n"
            "int main(void) { if (small(g)) g = 1; return fp(g); }\n"
        )
        self.assertEqual(_chosen(src), set())

    def test_returned_by_a_chosen_function(self):
        # `either` returns `is_b`'s result, and is branched on, so both
        # pass the carry on.
        src = _HEAD + (
            "static bool is_a(uint8_t c) { return c == 'a'; }\n"
            "static bool is_b(uint8_t c) { return c == 'b'; }\n"
            "static bool either(uint8_t c) {\n"
            "    if (is_a(c)) return true;\n"
            "    return is_b(c);\n"
            "}\n"
            "int main(void) { if (either(g)) g = 7; return g; }\n"
        )
        self.assertEqual(_chosen(src), {"is_a", "is_b", "either"})

    def test_values(self):
        src = _HEAD + (
            "uint8_t s[6] = {'a', 'x', 'b', 'y', 'b', 'a'};\n"
            "static bool is_a(uint8_t c) { return c == 'a'; }\n"
            "static bool is_b(uint8_t c) { return c == 'b'; }\n"
            "static bool either(uint8_t c) {\n"
            "    if (is_a(c)) return true;\n"
            "    return is_b(c);\n"
            "}\n"
            "int main(void) {\n"
            "    int n = 0;\n"
            "    for (uint8_t i = 0; i != 6; i++) {\n"
            "        if (either(s[i])) n += 10;\n"
            "        if (!is_a(s[i])) n += 1;\n"
            "    }\n"
            "    return n;\n"
            "}\n"
        )
        # Four of the six are 'a' or 'b', and four aren't 'a'.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(src, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 40 + 4)


if __name__ == "__main__":
    unittest.main()
