"""Unit tests for `passes.optimization.truncate_extend_fold`.

Covers the `Truncate(Extend(x), u)` → `Copy(x, u)` / narrower-
Truncate folds, plus the soundness gate that excludes non-SSA-
renamed names (globals, statics, address-taken locals) which can
have multiple definitions.
"""

import unittest
from tests.cpp_build import needs_c6502cpp


_SIZEOF_LOOPS = (
    "#include <stdint.h>\n"
    "uint8_t a[40];\n"
    "uint8_t b[40];\n"
    "uint8_t f(void) {\n"
    "    for (uint8_t i = 0; i < sizeof a; i++)\n"
    "        a[i] = b[i] + 1;\n"
    "    for (uint8_t i = 0; i < 40U; i++)\n"
    "        b[i] = a[i];\n"
    "    uint8_t *p = (uint8_t *)a;\n"
    "    for (uint8_t i = 0; i < sizeof a; i++)\n"
    "        p[i] += 2;\n"
    "    return b[39];\n"
    "}\n"
    "int main(void) {\n"
    "    for (uint8_t i = 0; i < 40; i++)\n"
    "        b[i] = i;\n"
    "    return f();\n"
    "}\n"
)


@needs_c6502cpp
class TestByteLoopAgainstSizeof(unittest.TestCase):
    """`for (uint8_t i = 0; i < sizeof a; i++)` compares one byte. The
    compare is at the width of `size_t`, `unsigned int`, so `i` is
    promoted to `int` and then converted to `unsigned int` with a
    copy, as it is for `i < 40U`. Copy propagation removes the copy
    after the compare has become a `JumpIfCmp`
    (`cmp_zero_jump_fold.NARROW_JUMP_COMPARE_RULE`), or keeps it when
    the extend has other readers, as the address of `p[i]` is in the
    third loop, and the narrowing looks through it."""

    def test_one_byte_compare(self):
        from compile import _run_source
        asm = _run_source("codegen", _SIZEOF_LOOPS, optimize=True)
        body = asm[asm.index("\nf:"):asm.index("\nmain:")]
        # The third loop keeps its counter in memory, so its compare
        # is `CMP #$28`.
        self.assertEqual(
            body.count("CPX   #$28") + body.count("CMP   #$28"), 3, body,
        )
        self.assertNotIn("SBC", body)
        self.assertNotIn("sx_done", body)

    def test_runs(self):
        from sim.harness import build_sim
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_SIZEOF_LOOPS, optimize=optimize)
                self.assertEqual(sim.run().return_int(), 40)


if __name__ == "__main__":
    unittest.main()
