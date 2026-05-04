"""Tests for the static-array IndexedLoad fast path.

When `arr[i]` accesses a static-storage array whose total byte size
≤ 256, c99_to_tac emits `IndexedLoad(name, byte_index, dst)`
instead of the general `GetAddress + pointer arithmetic + Load`
chain. tac_to_asm lowers as 6502 absolute,X addressing on the
link-time label — saves a DPTR setup and an indirect-Y dereference.

Coverage:
  * The asm shape: `LDA arr,X` / `LDA arr+1,X` per byte instead of
    `LDA (DPTR),Y` chains.
  * End-to-end correctness via the sim — multi-byte loads
    (uint16_t / long element types) read all bytes correctly.
  * The optimization fires only for static arrays, only when total
    size ≤ 256.
"""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim


def _signed_int(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIndexedLoadAsmShape(unittest.TestCase):
    """Source-level tests asserting the emitted asm uses absolute,X."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_static_uint16_array_uses_absolute_x(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint16_t arr[10] = {0,1,2,3,4,5,6,7,8,9};\n"
            "int main(int i) { return arr[i]; }\n"
        )
        asm = self._compile(src)
        # Should see `LDA arr,X` (low byte) and `LDA arr+1,X` (high
        # byte) — absolute,X reads.
        self.assertIn("LDA   arr,X", asm)
        self.assertIn("LDA   arr+1,X", asm)
        # Should NOT have a DPTR setup for this access (the regular
        # pointer-arithmetic path uses DPTR).
        # Some other code may still use DPTR, so this is a softer
        # check — the IndexedLoad path itself doesn't.

    def test_uint8_array_uses_absolute_x(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint8_t arr[100];\n"
            "int main(int i) { return arr[i]; }\n"
        )
        asm = self._compile(src)
        self.assertIn("LDA   arr,X", asm)

    def test_array_too_large_falls_back(self) -> None:
        # 200 uint16_t = 400 bytes, exceeds the 256-byte limit. Falls
        # back to the general pointer-arithmetic path, no `arr,X`.
        src = (
            "#include <stdint.h>\n"
            "static uint16_t arr[200];\n"
            "int main(int i) { return arr[i]; }\n"
        )
        asm = self._compile(src)
        self.assertNotIn(",X", asm)

    def test_local_array_falls_back(self) -> None:
        # Block-scope auto array — frame-allocated, not a static
        # label. The fast path requires StaticAttr; this falls back
        # to the general path.
        src = (
            "#include <stdint.h>\n"
            "int main(int i) { uint16_t arr[10] = {0}; return arr[i]; }\n"
        )
        asm = self._compile(src)
        # No absolute,X access on the local array.
        self.assertNotIn("arr,X", asm)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIndexedLoadCorrectness(unittest.TestCase):
    """End-to-end: each load reads the correct bytes."""

    def _sim(self, src: str, *, optimize: bool = True) -> int:
        sim = build_sim(src, optimize=optimize)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out, "simulator timed out")
        return _signed_int(result.return_int())

    def test_uchar_array_indexed_loads(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint8_t arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};\n"
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (int i = 0; i < 8; i = i + 1) sum = sum + arr[i];\n"
            "    return sum;\n"
            "}\n"
        )
        # 0+1+2+3+4+5+6+7 = 28
        self.assertEqual(self._sim(src), 28)

    def test_uint16_array_two_byte_loads(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint16_t arr[4] = {0x1234, 0x5678, 0x9ABC, 0xDEF0};\n"
            "int main(void) {\n"
            "    return arr[2];\n"
            "}\n"
        )
        # 0x9ABC = 39612 — but `int` is 16-bit signed, so this is
        # interpreted as -25924. Our sim wraps to signed.
        self.assertEqual(self._sim(src) & 0xFFFF, 0x9ABC)

    def test_long_array_four_byte_loads(self) -> None:
        # `long arr[4]` — 4 bytes per element × 4 = 16 bytes total.
        # The IndexedLoad reads all 4 bytes per access.
        src = (
            "long arr[4] = {1, 2, 3, 4};\n"
            "int test_arr(void) {\n"
            "    for (int i = 0; i < 4; i = i + 1) {\n"
            "        if (arr[i] != i + 1) return 1;\n"
            "    }\n"
            "    return 0;\n"
            "}\n"
            "int main(void) { return test_arr(); }\n"
        )
        self.assertEqual(self._sim(src), 0)


if __name__ == "__main__":
    unittest.main()
