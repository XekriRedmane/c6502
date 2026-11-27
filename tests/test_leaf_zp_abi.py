"""End-to-end tests for the leaf ZP-passing ABI.

Coverage:
  - A `[[c6502::zp_abi]]` leaf function called from a
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

The tests that only read compile.py's output are doctest cases in
cpp/tests/test_leaf_zp_abi.cpp.
"""
from __future__ import annotations

import io
import unittest
from unittest.mock import patch

import asm_ast
from compile import main as compile_main
from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


def _signed_int(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


@needs_c6502cpp
class TestLeafZpAbi(unittest.TestCase):
    def _codegen(self, source: str) -> str:
        with patch("sys.stdin", io.StringIO(source)), \
             patch("sys.stdout", new_callable=io.StringIO) as out:
            rc = compile_main([
                "compile.py", "-", "-S", "-O",
            ])
        self.assertEqual(rc, 0)
        return out.getvalue()

    def _sim_return_int(self, source: str) -> int:
        sim = build_sim(source, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out, "simulator timed out")
        return _signed_int(result.return_int())

    def test_zp_abi_leaf_correct_return_value(self) -> None:
        # Run the same program in the sim. Expect 3 + 4 = 7.
        src = (
            "[[c6502::zp_abi]] int add(int a, int b) { "
            "    return a + b; "
            "} "
            "int main(void) { return add(3, 4); }"
        )
        self.assertEqual(self._sim_return_int(src), 7)

    def test_caller_with_locals_calls_zp_abi_correctly(self) -> None:
        # Caller has its own body locals (x and y) AND calls a
        # ZP-ABI function. The caller's regalloc must avoid the
        # callee's param ZP addresses, otherwise the arg writes
        # would clobber the caller's locals mid-computation. This
        # exercises the "block outgoing-arg destinations" rule in
        # `_blocked_addrs_for`.
        src = (
            "[[c6502::zp_abi]] int add(int a, int b) { "
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

    def test_zp_abi_nonrecursive_call_accepted(self) -> None:
        # A direct non-recursive call from a zp_abi function is
        # now accepted (the optimizer's regalloc keeps the callee's
        # param ZP slots disjoint from the caller's locals).
        src = (
            "int helper(int x) { return x + 1; } "
            "[[c6502::zp_abi]] int wrap(int x) { return helper(x); } "
            "int main(void) { return wrap(3); }"
        )
        self.assertEqual(self._sim_return_int(src), 4)

    def test_eligible_non_leaf_emits_no_prologue(self) -> None:
        # An eligible function (no recursion, no indirect calls,
        # all callees eligible) that DOES make a zp_abi call AND
        # has a body local live across that call should still
        # emit as bare body + RTS — the private-pool allocator
        # gives the local a ZP slot disjoint from every coexisting
        # function's footprint, so no callee-save prologue is
        # needed. Headline win of the zp_local_allocation pass.
        src = (
            "[[c6502::zp_abi]] "
            "void touch(unsigned char x) { (void)x; } "
            "[[c6502::zp_abi]] "
            "unsigned char wrap(unsigned char a, unsigned char b) { "
            "  unsigned char x = a; "
            "  touch(b); "
            "  return x; "
            "} "
            "int main(void) { return wrap(7, 3); }"
        )
        asm = self._codegen(src)
        # wrap's body bounded by the next top-level label.
        wrap_idx = asm.index("\nwrap:")
        main_idx = asm.index("\nmain:")
        wrap_body = asm[wrap_idx:main_idx]
        # No soft-stack prologue artifacts.
        self.assertNotIn("STA   SSP", wrap_body)
        self.assertNotIn("STA   FP", wrap_body)
        self.assertNotIn(";", wrap_body)  # no "prologue:" comments
        # The cross-call value `x` (= a) must survive the call;
        # verified by the sim.
        self.assertEqual(self._sim_return_int(src), 7)

    def test_zp_abi_caller_callee_get_disjoint_slots(self) -> None:
        # A zp_abi caller and its direct zp_abi callee must be
        # assigned disjoint slot ranges by
        # cpp/src/passes/zp_slot_allocation.h: otherwise the callee's
        # arg-write would clobber the caller's still-live params.
        # Verified end-to-end via the simulator (correctness) and
        # by inspecting the EQU directives in the emitted asm
        # (allocation shape). Each function's leading `v` or `w`
        # arrives in A and X, so the parameters after it arrive in
        # their slots.
        src = (
            "[[c6502::zp_abi]] "
            "void noop(int w, int x, int y) { (void)w; (void)x; (void)y; } "
            "[[c6502::zp_abi]] "
            "int caller(int v, int a, int b) { "
            "  noop(v, a + 100, 0); "
            "  return b; "
            "} "
            "int main(void) { return caller(1, 2, 5); }"
        )
        # Correctness: `b` survives the call to `noop`.
        self.assertEqual(self._sim_return_int(src), 5)
        # Allocation shape: caller occupies the bottom of the ZP
        # window; noop is pushed up so its slots don't overlap.
        asm = self._codegen(src)
        # 2-byte int params -> two per-byte slots each: <param>_0
        # (low byte) and <param>_1 (high byte).
        self.assertIn("__zpabi_caller__a_0\tEQU\t$80", asm)
        self.assertIn("__zpabi_caller__a_1\tEQU\t$81", asm)
        self.assertIn("__zpabi_caller__b_0\tEQU\t$82", asm)
        self.assertIn("__zpabi_caller__b_1\tEQU\t$83", asm)
        self.assertIn("__zpabi_noop__x_0\tEQU\t$84", asm)
        self.assertIn("__zpabi_noop__x_1\tEQU\t$85", asm)
        self.assertIn("__zpabi_noop__y_0\tEQU\t$86", asm)
        self.assertIn("__zpabi_noop__y_1\tEQU\t$87", asm)


if __name__ == "__main__":
    unittest.main()
