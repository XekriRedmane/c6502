"""Regression tests for asm-level register-allocation correctness.

The asm-SSA layer versions each `(Pseudo name, byte offset)` pair
independently. Some asm instructions implicitly write more than
one byte to their dst Pseudo's storage — most notably
`LoadAddress`, which writes both the low and high byte of an
address (storage_base+0 and storage_base+1).

If the SSA layer versions only byte 0 of such a dst, byte 1's
write becomes invisible to the interference graph: regalloc can
silently allocate another value to the same physical byte 1
location, with overlapping lifetimes. The fix is to exclude such
"implicitly multi-byte" dst names from byte-granular SSA so
they keep their multi-byte coherence in a contiguous frame slot.

These tests sim a few patterns that exercise this: a loop counter
held in callee-saved ZP across an in-loop subscript through a
file-scope array. Pre-fix, regalloc gave the loop counter and
the address bytes the same callee-saved color and the loop
silently computed garbage offsets.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim, run_c_program


def _opt_asm(src: str):
    return build_sim(src, optimize=True).run()


def _no_opt(src: str):
    return run_c_program(src)


class TestLoopCounterVsLoadAddress(unittest.TestCase):
    """Each program does the same arithmetic three times — once
    unoptimized, once with `--optimize-asm`. The two paths must
    agree."""

    def test_int8_loop_uint16_array_subscript(self) -> None:
        # Minimal repro of the original bug: int8_t loop counter +
        # uint16_t array subscript inside the loop. The SignExtend
        # on `i` reads the byte right after where regalloc colored
        # the loop counter; before the fix that byte got clobbered
        # by the in-loop LoadAddress's high-byte store.
        src = (
            "#include <stdint.h>\n"
            "static uint16_t table[3] = {0xAA, 0xBB, 0xCC};\n"
            "int main(void) {\n"
            "    uint16_t sum = 0;\n"
            "    int8_t i;\n"
            "    for (i = 0; i < 3; i++) sum += table[i];\n"
            "    return sum;\n"
            "}\n"
        )
        expected = 0xAA + 0xBB + 0xCC  # 0x231
        self.assertEqual(_no_opt(src).return_int_signed(), expected)
        self.assertEqual(_opt_asm(src).return_int_signed(), expected)

    def test_int_loop_uint8_array_store(self) -> None:
        # Variant: store-through-pointer side. The LoadAddress on
        # the array decays the array name to a pointer and stores
        # to it; verifies the fix covers store as well as load.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t out[5];\n"
            "int main(void) {\n"
            "    int i;\n"
            "    for (i = 0; i < 5; i++) out[i] = (uint8_t)(i * 7);\n"
            "    return out[3];\n"
            "}\n"
        )
        expected = 21  # 3 * 7
        self.assertEqual(_no_opt(src).return_int_signed(), expected)
        self.assertEqual(_opt_asm(src).return_int_signed(), expected)

    def test_loop_with_two_loaded_addresses(self) -> None:
        # Two static arrays accessed in the same loop body — both
        # LoadAddresses' dsts must keep multi-byte coherence; their
        # bytes mustn't share a slot with each other or with the
        # loop counter.
        src = (
            "#include <stdint.h>\n"
            "static uint16_t a[3] = {1, 2, 3};\n"
            "static uint16_t b[3] = {10, 20, 30};\n"
            "int main(void) {\n"
            "    uint16_t sum = 0;\n"
            "    int8_t i;\n"
            "    for (i = 0; i < 3; i++) sum += a[i] + b[i];\n"
            "    return sum;\n"
            "}\n"
        )
        expected = (1 + 10) + (2 + 20) + (3 + 30)  # 66
        self.assertEqual(_no_opt(src).return_int_signed(), expected)
        self.assertEqual(_opt_asm(src).return_int_signed(), expected)

    def test_address_of_local_in_loop(self) -> None:
        # LoadAddress on a LOCAL (taken via &x), used as a pointer
        # inside a loop. Same shape, exercises the local-storage
        # path through `_apply_declarator` rather than `Data` for
        # the LoadAddress src.
        src = (
            "int main(void) {\n"
            "    int x = 5;\n"
            "    int *p = &x;\n"
            "    int sum = 0;\n"
            "    int i;\n"
            "    for (i = 0; i < 4; i++) sum += *p;\n"
            "    return sum;\n"
            "}\n"
        )
        expected = 5 * 4
        self.assertEqual(_no_opt(src).return_int_signed(), expected)
        self.assertEqual(_opt_asm(src).return_int_signed(), expected)


if __name__ == "__main__":
    unittest.main()
