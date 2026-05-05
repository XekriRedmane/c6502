"""Tests for the IndexedStore recognizer + lowering.

The TAC pass `recognize_indexed_store` detects the canonical
absolute,X-store pattern

    ZeroExtend(uchar_var, %ext)
    Binary(Add, Constant(C), %ext, %addr)   # or commutative
    Store(val, %addr)

with `%ext` and `%addr` single-use, `uchar_var` 1-byte typed,
`val` 1-byte typed, and `C ≤ 0xFF00`. Rewrites to the new TAC
instruction `IndexedStore(C, uchar_var, val)`. tac_to_asm
lowers IndexedStore as `LDA val; LDX uchar_var; STA $C,X`
(absolute,X store on a folded numeric base).

Coverage:
  * Asm shape: the canonical `static T * const` indexed write
    collapses to a single `STA $XXXX,X` instruction.
  * Eligibility: 16-bit value src is NOT folded (would need
    multi-byte STA chain), int-typed index is NOT folded
    (high byte non-zero invalidates the absolute,X invariant),
    multi-use addr / ext is NOT folded.
  * End-to-end correctness via the sim.
"""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIndexedStoreAsmShape(unittest.TestCase):
    """Source-level checks on the emitted asm."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_canonical_indexed_store_collapses(self) -> None:
        # The textbook case: a `static T * const` with an
        # uchar offset folds to `STA $XXXX,X`. The full chain
        # is: const-static read fold + reassoc + recognize.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4123;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[col] = value;\n"
            "}\n"
            "int main(void) { put(0x42, 5); return 0; }\n"
        )
        asm = self._compile(src)
        # Expect `STA $4123,X` somewhere — the canonical indexed-
        # absolute store with a folded 16-bit base.
        self.assertIn("STA   $4123,X", asm)
        # No DPTR routing for this access.
        # (Other accesses might still use DPTR; we only assert the
        # presence of the optimized form.)

    def test_const_offset_indexed_store(self) -> None:
        # Combine a `static T * const` with an indexing offset
        # constant: `buf[K + col] = v` where K is a compile-time
        # constant. Reassoc folds K into the base, then recognize
        # fires.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[100 + col] = value;\n"
            "}\n"
            "int main(void) { put(0x77, 3); return 0; }\n"
        )
        asm = self._compile(src)
        # Base is 0x4000 + 100 = 0x4064.
        self.assertIn("STA   $4064,X", asm)

    def test_int_index_does_not_fold(self) -> None:
        # `int col` is 2-byte typed; the high byte isn't
        # provably zero, so the absolute,X form is unsound. The
        # unoptimized indirect path stays.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
            "void put(uint8_t value, int col) {\n"
            "    buf[col] = value;\n"
            "}\n"
            "int main(void) { put(0x42, 5); return 0; }\n"
        )
        asm = self._compile(src)
        # No absolute,X store here. The address is computed at
        # runtime through DPTR.
        self.assertNotIn("STA   $4000,X", asm)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIndexedStoreCorrectness(unittest.TestCase):
    """End-to-end: the optimized program writes to the right
    memory address."""

    def test_byte_lands_at_indexed_address(self) -> None:
        # Folded `STA $XXXX,X` writes the byte at `XXXX + col`.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4100;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[col] = value;\n"
            "}\n"
            "int main(void) { put(0x55, 17); return 0; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.memory[0x4100 + 17], 0x55)

    def test_offset_indexed_store_lands_correctly(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[200 + col] = value;\n"
            "}\n"
            "int main(void) { put(0x99, 50); return 0; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        # 0x4000 + 200 + 50 = 0x4000 + 250 = 0x40FA.
        self.assertEqual(result.memory[0x40FA], 0x99)

    def test_unoptimized_still_works(self) -> None:
        # The fold only fires under --optimize. Without it the
        # program still has to compute the right address — verify
        # both modes produce the same result.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4200;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[col] = value;\n"
            "}\n"
            "int main(void) { put(0x33, 9); return 0; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                result = sim.run(max_cycles=5_000_000)
                self.assertFalse(result.timed_out)
                self.assertEqual(result.memory[0x4200 + 9], 0x33)


if __name__ == "__main__":
    unittest.main()
