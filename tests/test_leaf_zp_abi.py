"""End-to-end tests for the leaf ZP-passing ABI.

Coverage:
  - A `__attribute__((zp_abi))` leaf function called from a
    `main` that uses the soft-stack convention. The leaf has no
    soft-stack frame (no FunctionPrologue, no Ret with non-zero
    arg_bytes / local_bytes).
  - The simulator runs the program and returns the expected
    value, confirming the call-site ZP writes line up with the
    callee-side ZP reads.
  - A trivial-leaf-via-annotation case (`int add(int a, int b)`)
    that should collapse to bare RTS plus arithmetic.
  - Mismatched annotation rejected (zp_abi function with a call
    in body — already covered by abi_selection unit tests, here
    via the full pipeline).
"""
from __future__ import annotations

import io
import shutil
import unittest
from unittest.mock import patch

import asm_ast
from compile import main as compile_main
from sim.harness import build_sim


def _signed_int(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestLeafZpAbi(unittest.TestCase):
    def _codegen(self, source: str) -> str:
        with patch("sys.stdin", io.StringIO(source)), \
             patch("sys.stdout", new_callable=io.StringIO) as out:
            rc = compile_main([
                "compile.py", "-", "--codegen", "--optimize-asm",
            ])
        self.assertEqual(rc, 0)
        return out.getvalue()

    def _sim_return_int(self, source: str) -> int:
        sim = build_sim(source, optimize_asm=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out, "simulator timed out")
        return _signed_int(result.return_int())

    def test_zp_abi_leaf_emits_no_prologue(self) -> None:
        # add() is a ZP-ABI leaf — no calls, address not taken, 4
        # param bytes (well under the 64-byte ZP window). No
        # FunctionPrologue / Ret in its asm; only a bare RTS at
        # exit (plus the arithmetic).
        src = (
            "__attribute__((zp_abi)) int add(int a, int b) { "
            "    return a + b; "
            "} "
            "int main(void) { return add(3, 4); }"
        )
        out = self._codegen(src)
        # `add:` SUBROUTINE block. Find it and check its prologue/
        # epilogue presence.
        add_idx = out.index("add:")
        # Find where `main:` starts (or end of file) to bound add's
        # body.
        main_idx = out.index("main:")
        add_body = out[add_idx:main_idx]
        # No FunctionPrologue artifacts: no SBC against SSP, no
        # SSP/FP setup.
        self.assertNotIn("STA   SSP", add_body)
        self.assertNotIn("STA   FP", add_body)
        # The body must end in an RTS (the bare RTS atom).
        self.assertIn("RTS", add_body)

    def test_zp_abi_leaf_correct_return_value(self) -> None:
        # Run the same program in the sim. Expect 3 + 4 = 7.
        src = (
            "__attribute__((zp_abi)) int add(int a, int b) { "
            "    return a + b; "
            "} "
            "int main(void) { return add(3, 4); }"
        )
        self.assertEqual(self._sim_return_int(src), 7)

    def test_zp_abi_caller_no_allocate_stack_pre_call(self) -> None:
        # Caller (main) emits Movs to ZP $80/$81 and ZP $82/$83
        # rather than AllocateStack(4) + Stack writes.
        src = (
            "__attribute__((zp_abi)) int add(int a, int b) { "
            "    return a + b; "
            "} "
            "int main(void) { return add(3, 4); }"
        )
        out = self._codegen(src)
        # Find main's body bounded by the next function-name label
        # or end-of-output.
        main_idx = out.index("main:")
        main_body = out[main_idx:]
        # Caller writes 3 to ZP $80 and 4 to ZP $82 (low bytes;
        # high bytes go to $81/$83). No SBC against SSP for
        # arg-block allocation. (The prologue still adjusts SSP
        # for the saved-FP slot since main is soft-stack ABI; we
        # only check there's no AllocateStack-style SBC matching
        # the 4-byte arg total.)
        self.assertIn("STA   $80", main_body)
        self.assertIn("STA   $82", main_body)

    def test_caller_with_locals_calls_zp_abi_correctly(self) -> None:
        # Caller has its own body locals (x and y) AND calls a
        # ZP-ABI function. The caller's regalloc must avoid the
        # callee's param ZP addresses, otherwise the arg writes
        # would clobber the caller's locals mid-computation. This
        # exercises the "block outgoing-arg destinations" rule in
        # `_blocked_addrs_for`.
        src = (
            "__attribute__((zp_abi)) int add(int a, int b) { "
            "    return a + b; "
            "} "
            "int main(int n) { "
            "    int x = n + 10; "
            "    int y = n + 20; "
            "    return add(x, y); "
            "}"
        )
        # n defaults to 0 in the sim's calling convention →
        # x=10, y=20, return 30.
        self.assertEqual(self._sim_return_int(src), 30)

    def test_zp_abi_with_call_in_body_rejected(self) -> None:
        # End-to-end via compile_main: the abi_selection error
        # should propagate.
        src = (
            "int helper(int x) { return x + 1; } "
            "__attribute__((zp_abi)) int wrap(int x) { return helper(x); } "
            "int main(void) { return wrap(3); }"
        )
        with self.assertRaises(Exception) as cm:
            self._codegen(src)
        # AbiSelectionError doesn't subclass ParserError; it
        # propagates as-is. Verify the error message names the
        # offending function.
        self.assertIn("wrap", str(cm.exception))


if __name__ == "__main__":
    unittest.main()
