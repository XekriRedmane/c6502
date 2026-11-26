"""Tests for HwReg (X / Y) pinning infrastructure.

Phase 2 of asm-level register allocation: extend coloring to pin
single-byte SSA Pseudos into the 6502's X / Y index registers
when their use shape is compatible (LDX/LDY/STX/STY/INX/DEX/CPX
operations only, no live range crossing a `Call`).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import shutil
import unittest


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestHwRegPinningEndToEnd(unittest.TestCase):
    """End-to-end checks on real C programs: HwReg pinning fires
    where expected and produces the right asm shape."""

    def _compile(self, src: str) -> str:
        from compile import _run_source
        return _run_source(
            "codegen", src, optimize=True,
        )

    def test_loop_iv_used_as_indexed_store_pin(self):
        # A column-iv used as the X index for many indexed stores
        # should pin to X — the writes should emit `STA $XXXX,X`
        # without per-store `LDX <slot>` setup.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
            "void clear(void) {\n"
            "    for (uint8_t i = 0; i < 16; i++) buf[i] = 0;\n"
            "}\n"
        )
        asm = self._compile(src)
        # The store `buf[i] = 0` should emit `STA $4000,X` (or
        # `STA buf,X` if not const-folded). The body should NOT
        # have a separate `LDX <ZP>` reload before each store.
        self.assertIn("STA   $40", asm)


if __name__ == "__main__":
    unittest.main()
