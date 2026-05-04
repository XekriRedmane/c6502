"""Tests for direct-memory inline shift-by-1.

`tac_to_asm`'s inline shift-by-1 lowering now emits a per-byte
`Mov(src, dst); ShiftAtom(dst)` sequence (instead of `Mov(src, A);
ShiftAtom(A); Mov(A, dst)`). After regalloc + apply_coloring +
the self-Mov peephole:

  * If src and dst coalesce to the same physical slot (typical
    for compound `<<=` / `>>=` after SSA-aware regalloc): the
    Mov drops and we get a single `ASL $XX` / `ROL $XX` etc.
    direct-memory instruction per byte.
  * Otherwise: Mov + direct shift. Same byte count as the old
    `LDA src; ASL A; STA dst` pattern, no slower.

For Frame / Stack dsts (rare — regalloc usually puts shift dsts
in ZP), asm_emit synthesizes the indirect-Y round-trip:
`LDY # / LDA (PTR),Y / ASL A / LDY # / STA (PTR),Y`.

Sim coverage: confirms each pattern produces correct results vs
the unoptimized helper-call path. Plus an asm-level smoke check
that direct-memory shifts appear in the output for in-place
compound assignments.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim, run_c_program


def _run_int(src: str, *, opt: bool = False, signed: bool = True) -> int:
    res = (
        build_sim(src, optimize=True).run() if opt
        else run_c_program(src)
    )
    return res.return_int_signed() if signed else res.return_int()


def _run_long(src: str, *, opt: bool = False, signed: bool = True) -> int:
    res = (
        build_sim(src, optimize=True).run() if opt
        else run_c_program(src)
    )
    return res.return_long_signed() if signed else res.return_long()


class TestInlineShiftCorrectness(unittest.TestCase):
    """Both paths (no-opt and --optimize-asm) produce the same
    return for every shift width × signedness."""

    def test_uint16_lsl_inplace(self):
        # `x <<= 1` on a 2-byte unsigned. Compound assignment makes
        # x both src and dst at the TAC level; after SSA regalloc
        # they typically share a ZP slot, and the inline shift's
        # leading Mov drops via the self-Mov peephole, leaving a
        # bare `ASL $XX; ROL $XX` pair.
        src = (
            "unsigned int f(unsigned int x) { x <<= 1; return x; }\n"
            "unsigned int main(void) { return f(0x1234u); }\n"
        )
        self.assertEqual(_run_int(src, opt=False, signed=False), 0x2468)
        self.assertEqual(_run_int(src, opt=True, signed=False), 0x2468)

    def test_int16_lsr_signed(self):
        src = (
            "int f(int x) { x >>= 1; return x; }\n"
            "int main(void) { return f(-5); }\n"
        )
        # -5 >> 1 = -3 (arithmetic right shift).
        self.assertEqual(_run_int(src, opt=False), -3)
        self.assertEqual(_run_int(src, opt=True), -3)

    def test_uint16_lsr_unsigned(self):
        src = (
            "unsigned int f(unsigned int x) { x >>= 1; return x; }\n"
            "unsigned int main(void) { return f(0xFFFEu); }\n"
        )
        self.assertEqual(_run_int(src, opt=False, signed=False), 0x7FFF)
        self.assertEqual(_run_int(src, opt=True, signed=False), 0x7FFF)

    def test_uint32_lsl(self):
        # 4-byte left shift exercises ASL b0; ROL b1; ROL b2; ROL b3.
        src = (
            "unsigned long f(unsigned long x) { x <<= 1; return x; }\n"
            "unsigned long main(void) { return f(0x80000001UL); }\n"
        )
        # 0x80000001 << 1 = 0x100000002 → 0x00000002 (32-bit wrap).
        self.assertEqual(_run_long(src, opt=False, signed=False), 0x2)
        self.assertEqual(_run_long(src, opt=True, signed=False), 0x2)

    def test_int32_lsr_signed(self):
        # 4-byte signed right shift: capture sign into carry, then
        # ROR through all 4 bytes high-to-low.
        src = (
            "long f(long x) { x >>= 1; return x; }\n"
            "long main(void) { return f(-100000L); }\n"
        )
        # -100000 >> 1 = -50000.
        self.assertEqual(_run_long(src, opt=False), -50000)
        self.assertEqual(_run_long(src, opt=True), -50000)

    def test_strength_reduced_multiply(self):
        # `x * 2` after strength reduction → `x << 1`, lowered via
        # the inline direct-shift pattern.
        src = (
            "unsigned int f(unsigned int x) { return x * 2; }\n"
            "unsigned int main(void) { return f(0x4321u); }\n"
        )
        self.assertEqual(_run_int(src, opt=False, signed=False), 0x8642)
        self.assertEqual(_run_int(src, opt=True, signed=False), 0x8642)


class TestInlineShiftDirectMemoryEmit(unittest.TestCase):
    """Asm-level smoke check that direct-memory shift opcodes
    appear in the output for compound assignments on ZP-coloreable
    operands."""

    def _compile_asm(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_direct_asl_appears(self):
        # `int x; x <<= 1;` should emit a direct `ASL $XX` somewhere
        # in the body (not an `ASL A` round-trip).
        src = (
            "int f(int x) { x <<= 1; return x; }\n"
            "int main(void) { return f(50); }\n"
        )
        asm = self._compile_asm(src)
        # Scan f's body for direct-mode ASL.
        body = self._extract_function(asm, "f")
        # Direct ASL has the form `ASL   $XX` (zp) or `ASL name+off`
        # (Data); accumulator form is `ASL   A`.
        direct_asl_lines = [
            line for line in body.splitlines()
            if "ASL" in line and "A" not in line.split()[-1]
        ]
        self.assertTrue(
            direct_asl_lines,
            f"expected direct-memory ASL in f's body, got:\n{body}",
        )

    @staticmethod
    def _extract_function(asm: str, name: str) -> str:
        out: list[str] = []
        in_fn = False
        for line in asm.splitlines():
            if line.startswith(f"{name}:"):
                in_fn = True
            if in_fn:
                if (
                    line and not line.startswith((" ", "\t", "."))
                    and line.endswith(":") and not line.startswith(f"{name}:")
                ):
                    break
                out.append(line)
        return "\n".join(out)


if __name__ == "__main__":
    unittest.main()
