"""Tests for the const-static-fold asm-level optimization.

A `static T const = <const-init>` (file-scope, internal linkage,
const-qualified, scalar) whose address is never taken is genuinely
immutable in a single-TU program. Under `--optimize`, the asm-level
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
  * Without `--optimize`, the fold is a no-op (storage and
    references both remain).
"""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestConstStaticFoldAsmShape(unittest.TestCase):
    """Source-level checks on the emitted asm."""

    def _compile_optimized(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def _compile_plain(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=False)

    def test_const_pointer_static_folded_to_immediates(self) -> None:
        # Canonical case: a `static T * const` initialized to a
        # constant address. Every reference becomes an immediate
        # of one byte of the address, and the static itself is
        # dropped.
        src = (
            "static unsigned char * const buf"
            " = (unsigned char * const)0x4123;\n"
            "void put(unsigned char x) { *buf = x; }\n"
            "int main(void) { put(0x42); return 0; }\n"
        )
        asm = self._compile_optimized(src)
        # The label is gone.
        self.assertNotIn("buf:", asm)
        # No `LDA buf` / `LDA buf+1` references.
        self.assertNotIn("LDA   buf\n", asm)
        self.assertNotIn("LDA   buf+1", asm)
        # Immediates carrying the address bytes are present (low
        # byte $23, high byte $41 from 0x4123).
        self.assertIn("#$23", asm)
        self.assertIn("#$41", asm)

    def test_unoptimized_keeps_static_storage(self) -> None:
        # Without --optimize the fold doesn't run. The storage
        # cells and the label both survive; references are
        # absolute loads, not immediates.
        src = (
            "static unsigned char * const buf"
            " = (unsigned char * const)0x4123;\n"
            "void put(unsigned char x) { *buf = x; }\n"
            "int main(void) { put(0x42); return 0; }\n"
        )
        asm = self._compile_plain(src)
        self.assertIn("buf:", asm)
        self.assertIn("LDA   buf", asm)

    def test_address_taken_keeps_static(self) -> None:
        # Taking the address of a candidate disqualifies it: the
        # symbol's address is needed at link time, so we can't
        # drop the storage. The references stay as `LDA name`,
        # not immediates.
        src = (
            "static unsigned char * const buf"
            " = (unsigned char * const)0x4000;\n"
            "unsigned char ** ptr_holder;\n"
            "int main(void) { ptr_holder = &buf; return 0; }\n"
        )
        asm = self._compile_optimized(src)
        # Static storage survived.
        self.assertIn("buf:", asm)

    def test_array_initializer_not_folded(self) -> None:
        # Multi-element initializers are out of scope — only
        # scalar inits fold. The array stays put.
        src = (
            "static const unsigned char tbl[4] = {1, 2, 3, 4};\n"
            "int main(int i) { return tbl[i]; }\n"
        )
        asm = self._compile_optimized(src)
        self.assertIn("tbl:", asm)

    def test_global_const_not_folded(self) -> None:
        # External linkage (no `static` keyword): another TU might
        # reference the symbol, so we keep it even though the
        # initializer is constant.
        src = (
            "unsigned char * const buf"
            " = (unsigned char * const)0x4000;\n"
            "void put(unsigned char x) { *buf = x; }\n"
            "int main(void) { put(0x42); return 0; }\n"
        )
        asm = self._compile_optimized(src)
        self.assertIn("buf:", asm)

    def test_non_const_static_not_folded(self) -> None:
        # No `const` qualifier — even though the initializer is
        # constant and nothing writes to the variable in this TU,
        # we don't try to prove non-mutation. The conservative
        # gate is the type-system const qualifier.
        src = (
            "static unsigned char * buf"
            " = (unsigned char *)0x4000;\n"
            "void put(unsigned char x) { *buf = x; }\n"
            "int main(void) { put(0x42); return 0; }\n"
        )
        asm = self._compile_optimized(src)
        self.assertIn("buf:", asm)

    def test_const_int_static_folded(self) -> None:
        # The fold isn't pointer-specific — any const-qualified
        # internal-linkage scalar with a constant initializer
        # qualifies. A `static const int` works the same way.
        src = (
            "static const int magic = 0x1234;\n"
            "int main(void) { return magic; }\n"
        )
        asm = self._compile_optimized(src)
        self.assertNotIn("magic:", asm)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
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


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestConstStaticFoldUnit(unittest.TestCase):
    """Direct calls to the pass on synthetic programs — exercise
    the candidate-collection / disqualification / rewrite logic
    without going through the full pipeline."""

    def test_no_symbols_is_noop(self) -> None:
        # The pass is gated on `symbols` being supplied; without
        # it (legacy / synthetic test paths) it must return the
        # program unchanged.
        from passes.optimization_asm.const_static_fold import (
            fold_const_statics,
        )
        import asm_ast
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="x", is_global=False,
                init=[asm_ast.IntInit(value=0x1234)],
            ),
        ])
        result = fold_const_statics(prog, symbols=None)
        self.assertIs(result, prog)


if __name__ == "__main__":
    unittest.main()
