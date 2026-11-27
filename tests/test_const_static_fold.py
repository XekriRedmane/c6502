"""Tests for the const-static-fold asm-level optimization.

A `static T const = <const-init>` (file-scope, internal linkage,
const-qualified, scalar) whose address is never taken is genuinely
immutable in a single-TU program. Under `-O`, the asm-level
optimizer pipeline replaces every reference to its bytes with `Imm`
operands carrying the corresponding initializer byte, and drops
the `StaticVariable` top-level. The win: `LDA name` (3 bytes) →
`LDA #imm` (2 bytes) at every use, plus the storage cells freed.

Coverage:
  * Asm shape: a `static T * const` initialized to a constant
    address gets folded — `LDA name` / `LDA name+1` are gone, the
    label itself disappears, and immediate loads of the address
    bytes appear in their place.
  * Disqualifications: address-taken (`&p`), externally linked
    (`is_global=True`), array initializer, non-const, missing
    `const` qualifier — every one of these keeps the static.
  * End-to-end correctness via the sim — folded code computes
    the same result as unfolded code.
  * Without `-O`, the fold is a no-op (storage and
    references both remain).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).

The `-S` text is checked in cpp/tests/test_const_static_fold.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestConstStaticFoldCorrectness(unittest.TestCase):
    """End-to-end: optimized programs compute the same answers as
    unoptimized ones."""

    def test_writes_through_folded_pointer(self) -> None:
        # The folded pointer-load lands on the same address the
        # unfolded version computes. We pick an address well above
        # the code region, write a known byte through `*buf`, and
        # check it landed at the expected memory cell.
        src = (
            "static unsigned char * const buf"
            " = (unsigned char * const)0x4000;\n"
            "int main(void) { *buf = 0x77; return 0; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                result = sim.run(max_cycles=5_000_000)
                self.assertFalse(result.timed_out)
                self.assertEqual(result.memory[0x4000], 0x77)

    def test_indexed_writes_through_folded_pointer(self) -> None:
        # The 16-bit add of a folded pointer + integer must still
        # produce the correct address. Combines this fold with the
        # pointer-arithmetic lowering — the pointer's bytes are
        # `Imm` instead of `Data`, but the per-byte add chain is
        # otherwise identical.
        src = (
            "static unsigned char * const buf"
            " = (unsigned char * const)0x4000;\n"
            "int main(void) {\n"
            "    buf[300] = 0x42;\n"
            "    return 0;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                result = sim.run(max_cycles=5_000_000)
                self.assertFalse(result.timed_out)
                self.assertEqual(result.memory[0x4000 + 300], 0x42)

    def test_const_int_value_returned(self) -> None:
        # A `static const int` whose value is the program's return
        # — round-trips through the fold path.
        src = (
            "static const int magic = 0x1234;\n"
            "int main(void) { return magic; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        # 16-bit return lands in HARGS+0..1 (Int).
        self.assertEqual(result.return_int() & 0xFFFF, 0x1234)


@needs_c6502cpp
class TestConstPointer(unittest.TestCase):
    """A `static const` pointer initialized with an address is read as
    the address's halves, and an access through it as one through the
    address, where both had read the pointer from memory."""

    _SRC = (
        "#include <stdint.h>\n"
        "uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
        "static const uint8_t *const B = buf + 3;\n"
        "uint8_t at(uint8_t i) { return B[i]; }\n"
        "const uint8_t *ptr(void) { return B; }\n"
        "int main(void) {\n"
        "    int bad = 0;\n"
        "    if (at(0) != 4 || at(4) != 8) bad |= 1;\n"
        "    if (ptr() != &buf[3]) bad |= 2;\n"
        "    return bad;\n"
        "}\n"
    )

    def test_runs(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(out.return_int(), 0)

if __name__ == "__main__":
    unittest.main()
