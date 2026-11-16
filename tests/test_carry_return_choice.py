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
        # Another unit may call it, so this choice, made from the
        # calls, leaves it to `choose_global_carry_returns`.
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

    def test_returned_by_a_function_with_external_linkage(self):
        # `is_digit` returns in C by the rule for external linkage, so
        # `in_range`, whose result it returns, passes the carry on
        # through it.
        src = _HEAD + (
            "static bool in_range(uint8_t c) { return c - '0' < 10; }\n"
            "bool is_digit(uint8_t c) { return in_range(c); }\n"
            "int main(void) { if (is_digit(g)) g = 7; return g; }\n"
        )
        self.assertEqual(_chosen(src), {"in_range"})

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


def _global_chosen(src: str) -> set[str]:
    """The functions `choose_global_carry_returns` picks in `src`."""
    import compile as C
    from preprocessor import preprocess
    picked: set[str] = set()
    orig = C.choose_global_carry_returns

    def spy(tac, symbols, abi):
        out = orig(tac, symbols, abi)
        picked.update(out)
        return out

    C.choose_global_carry_returns = spy
    try:
        C._run_stage("codegen", preprocess(src), optimize=True)
    finally:
        C.choose_global_carry_returns = orig
    return picked


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestGlobalChoice(unittest.TestCase):
    """A `bool` function with external linkage returns in C whenever
    it's zp_abi, whatever its callers do with the result, since the
    other units decide from the declaration too."""

    def test_defined_and_declared(self):
        # `small` is defined here and `other` only declared; both are
        # chosen, even though `main` keeps one result as a value.
        src = _HEAD + (
            "bool other(uint8_t x);\n"
            "bool small(uint8_t x) { return x < 10; }\n"
            "int main(void) { g = small(g); if (other(g)) g++; return g; }\n"
        )
        self.assertEqual(_global_chosen(src), {"small", "other"})

    def test_static_left_to_the_calls(self):
        src = _HEAD + (
            "static bool small(uint8_t x) { return x < 10; }\n"
            "int main(void) { g = small(g); return g; }\n"
        )
        self.assertEqual(_global_chosen(src), set())

    def test_address_taken(self):
        # Not zp_abi, and a call through the pointer reads A.
        src = _HEAD + (
            "bool small(uint8_t x) { return x < 10; }\n"
            "bool (*fp)(uint8_t) = small;\n"
            "int main(void) { if (small(g)) g = 1; return fp(g); }\n"
        )
        self.assertEqual(_global_chosen(src), set())

    def test_fixed_address(self):
        # A routine at a fixed address has the convention its
        # declaration gives.
        src = _HEAD + (
            "[[c6502::address(0x2000)]] bool rom(void);\n"
            "int main(void) { if (rom()) g = 1; return g; }\n"
        )
        self.assertEqual(_global_chosen(src), set())

    def test_not_bool(self):
        src = _HEAD + (
            "uint8_t small(uint8_t x) { return x < 10; }\n"
            "int main(void) { if (small(g)) g = 1; return g; }\n"
        )
        self.assertEqual(_global_chosen(src), set())

    def test_values(self):
        # Each way a caller can use the result: branched on, kept as a
        # value, negated, and returned by another such function.
        src = _HEAD + (
            "uint8_t s[6] = {'a', 'x', 'b', 'y', 'b', 'a'};\n"
            "bool is_a(uint8_t c) { return c == 'a'; }\n"
            "bool is_b(uint8_t c) { return c == 'b'; }\n"
            "bool either(uint8_t c) {\n"
            "    if (is_a(c)) return true;\n"
            "    return is_b(c);\n"
            "}\n"
            "int main(void) {\n"
            "    int n = 0;\n"
            "    for (uint8_t i = 0; i != 6; i++) {\n"
            "        if (either(s[i])) n += 10;\n"
            "        if (!is_a(s[i])) n += 1;\n"
            "        n += 100 * is_b(s[i]);\n"
            "    }\n"
            "    return n;\n"
            "}\n"
        )
        # Four are 'a' or 'b', four aren't 'a', and two are 'b'.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(src, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 40 + 4 + 200)


_LIB = (
    "#include <stdint.h>\n#include <stdbool.h>\n"
    "bool is_digit(uint8_t c) { return c >= '0' && c <= '9'; }\n"
)
_MAIN = (
    "#include <stdint.h>\n#include <stdbool.h>\n"
    "bool is_digit(uint8_t c);\n"
    "uint8_t s[5] = {'1', 'x', '7', ' ', '0'};\n"
    "int main(void) {\n"
    "    int n = 0;\n"
    "    for (uint8_t i = 0; i != 5; i++) {\n"
    "        if (is_digit(s[i])) n += 10;\n"
    "        n += is_digit(s[i]);\n"
    "    }\n"
    "    return n;\n"
    "}\n"
)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestLinked(unittest.TestCase):

    def _compile(self, src: str, path) -> str:
        import io
        from unittest.mock import patch
        from compile import main as compile_main
        with patch("sys.stdin", io.StringIO(src)):
            self.assertEqual(compile_main([
                "compile.py", "-", "--codegen", "--optimize",
                "-o", str(path),
            ]), 0)
        return path.read_text()

    def test_units_agree_and_run(self):
        import tempfile
        from pathlib import Path
        from compile import main as compile_main
        with tempfile.TemporaryDirectory() as tmp:
            lib = self._compile(_LIB, Path(tmp) / "lib.asm")
            main = self._compile(_MAIN, Path(tmp) / "main.asm")
            # The defining unit and the calling unit both record it.
            self.assertRegex(lib, r"; def is_digit .*carry=true")
            self.assertRegex(main, r"; ext is_digit .*carry=true")
            out = Path(tmp) / "out.asm"
            self.assertEqual(compile_main([
                "compile.py", "--link", str(Path(tmp) / "lib.asm"),
                str(Path(tmp) / "main.asm"), "-o", str(out),
            ]), 0)
            text = out.read_text()
        if shutil.which("dasm"):
            from sim.linked import build_linked_sim
            # Three of the five are digits.
            self.assertEqual(build_linked_sim(text).run().return_int(), 33)

    def test_units_disagree(self):
        # A unit that calls the function expecting its result in A,
        # against one that defines it returning in C.
        from passes.linker import LinkError, _merge
        from passes.zp_link_metadata import (
            ExternMeta, FunctionMeta, LinkMetadata,
        )
        lib = LinkMetadata(defs=[FunctionMeta(
            name="is_digit", params=["__zpabi_is_digit__c"], locals=[],
            indirect=False, in_cycle=False, carry=True,
        )])
        main = LinkMetadata(externs=[ExternMeta(
            name="is_digit", params=["__zpabi_is_digit__c"],
        )])
        with self.assertRaisesRegex(LinkError, "carry flag"):
            _merge([lib, main])
        # The other order too.
        with self.assertRaisesRegex(LinkError, "carry flag"):
            _merge([main, lib])


if __name__ == "__main__":
    unittest.main()
