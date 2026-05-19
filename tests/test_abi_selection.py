"""Behavioral tests for `passes.abi_selection.select_abi`.

Coverage:
  - No annotation → SoftStackLayout.
  - Annotation on a leaf function with small params → ZpLayout
    with sequential addresses from the pool's caller-saved start.
  - Annotation on a function that makes a non-recursive direct
    call → ZpLayout (the callee's params are blocked from the
    caller's locals by the regalloc, no clobbering).
  - Annotation on a function that is directly recursive → rejected.
  - Annotation on functions that are mutually recursive → rejected.
  - Annotation on a function whose body contains an indirect call
    → rejected (the callee's ABI is unknown).
  - Annotation on a function whose address is taken →
    `AbiSelectionError`.
  - Annotation on a function whose total param bytes exceed the
    pool window → `AbiSelectionError`.
  - End-to-end via `compile.py` source: parse a .c file with the
    annotation and verify the dict has the right entries.
"""
from __future__ import annotations

import unittest

import c99_ast
import tac_ast
from c99_to_tac import translate_program as translate_to_tac
from passes.abi_selection import (
    AbiSelectionError,
    SoftStackLayout,
    ZpLayout,
    select_abi,
)
from passes.identifier_resolution import resolve_program as resolve_identifiers
from passes.label_resolution import resolve_program as resolve_labels
from passes.loop_labeling import label_program as label_loops
from passes.optimization.pool import Pool
from passes.string_lifting import lift_program as lift_strings
from passes.type_checking import check_program as type_check_program
from parser import parse


def _compile_to_tac(src: str):
    """Run the front end up through TAC. Returns
    (tac_program, c99_program, types)."""
    ast0 = parse(src)
    ast1 = resolve_identifiers(ast0)
    ast2 = lift_strings(ast1)
    ast3 = resolve_labels(ast2)
    ast4 = label_loops(ast3)
    ast5, syms, types = type_check_program(ast4)
    tac = translate_to_tac(ast5, syms, types)
    return tac, ast5, types


class TestSelectAbi(unittest.TestCase):
    def test_no_annotation_eligible_defaults_to_zp_abi(self) -> None:
        # Without `__attribute__((zp_abi))`, an eligible function
        # (no indirect calls, no recursion, no address taken,
        # params fit the ZP window) defaults to zp_abi under
        # `select_abi` — this matches the `--optimize` policy of
        # "every function gets zp_abi when it can."
        tac, c99, types = _compile_to_tac(
            "int main(void) { return 0; }",
        )
        abi = select_abi(tac, c99, types)
        self.assertIsInstance(abi["main"], ZpLayout)
        # main takes no params, so the slot list is empty.
        self.assertEqual(abi["main"].addrs, [])

    def test_zp_abi_leaf_with_int_param(self) -> None:
        tac, c99, types = _compile_to_tac(
            "__attribute__((zp_abi)) int f(int x) { return x + 1; } "
            "int main(void) { return 0; }",
        )
        abi = select_abi(tac, c99, types)
        self.assertIsInstance(abi["f"], ZpLayout)
        # Int = 2 bytes; ZP window starts at $80 (default Pool).
        self.assertEqual(abi["f"].addrs, [0x80, 0x81])

    def test_zp_abi_two_int_params(self) -> None:
        tac, c99, types = _compile_to_tac(
            "__attribute__((zp_abi)) int f(int a, int b) { return a + b; } "
            "int main(void) { return 0; }",
        )
        abi = select_abi(tac, c99, types)
        self.assertEqual(abi["f"].addrs, [0x80, 0x81, 0x82, 0x83])

    def test_unannotated_eligible_helper_defaults_to_zp_abi(self) -> None:
        # `helper` has no annotation but qualifies (no recursion,
        # no indirect call, params fit) — under the default-zp_abi
        # policy it lands as ZpLayout, not SoftStack.
        tac, c99, types = _compile_to_tac(
            "int helper(int x) { return x + 1; } "
            "__attribute__((zp_abi)) int f(int x) { return helper(x); } "
            "int main(void) { return 0; }",
        )
        abi = select_abi(tac, c99, types)
        self.assertIsInstance(abi["f"], ZpLayout)
        self.assertIsInstance(abi["helper"], ZpLayout)

    def test_unannotated_recursive_silently_falls_back(self) -> None:
        # `r` is directly recursive — ineligible for zp_abi because
        # a recursive call would overwrite the outer activation's
        # param slots. Without an annotation, the default-zp_abi
        # policy silently downgrades it to SoftStack instead of
        # raising.
        tac, c99, types = _compile_to_tac(
            "int r(int x) { return x ? r(x - 1) : 0; } "
            "int main(void) { return 0; }",
        )
        abi = select_abi(tac, c99, types)
        self.assertIsInstance(abi["r"], SoftStackLayout)
        # main is eligible and gets zp_abi.
        self.assertIsInstance(abi["main"], ZpLayout)

    def test_zp_abi_direct_recursion_rejected(self) -> None:
        with self.assertRaises(AbiSelectionError) as cm:
            tac, c99, types = _compile_to_tac(
                "__attribute__((zp_abi)) int f(int x) { "
                "  return x ? f(x - 1) : 0; "
                "} "
                "int main(void) { return 0; }",
            )
            select_abi(tac, c99, types)
        self.assertIn("recursion", str(cm.exception))
        self.assertIn("`f`", str(cm.exception))

    def test_zp_abi_mutual_recursion_rejected(self) -> None:
        # f -> g -> f forms a cycle; reject f.
        with self.assertRaises(AbiSelectionError) as cm:
            tac, c99, types = _compile_to_tac(
                "int g(int x); "
                "__attribute__((zp_abi)) int f(int x) { return g(x); } "
                "int g(int x) { return f(x); } "
                "int main(void) { return 0; }",
            )
            select_abi(tac, c99, types)
        self.assertIn("recursion", str(cm.exception))
        self.assertIn("`f`", str(cm.exception))

    def test_zp_abi_indirect_call_rejected(self) -> None:
        # An indirect call inside a zp_abi body is rejected — the
        # callee's ABI is unknown at the call site.
        with self.assertRaises(AbiSelectionError) as cm:
            tac, c99, types = _compile_to_tac(
                "int helper(int x) { return x + 1; } "
                "__attribute__((zp_abi)) int f(int x) { "
                "  int (*p)(int) = &helper; "
                "  return p(x); "
                "} "
                "int main(void) { return 0; }",
            )
            select_abi(tac, c99, types)
        self.assertIn("indirect call", str(cm.exception))
        self.assertIn("`f`", str(cm.exception))

    def test_zp_abi_address_taken_rejected(self) -> None:
        # Take the address of `f` via explicit `&f`. (Implicit
        # function-name decay isn't supported by the type checker
        # yet; explicit `&f` is.)
        with self.assertRaises(AbiSelectionError) as cm:
            tac, c99, types = _compile_to_tac(
                "__attribute__((zp_abi)) int f(int x) { return x + 1; } "
                "int main(void) { return (int)&f; }",
            )
            select_abi(tac, c99, types)
        self.assertIn("address is taken", str(cm.exception))
        self.assertIn("`f`", str(cm.exception))

    def test_zp_abi_oversized_params_rejected(self) -> None:
        # Default pool is 64 caller-saved bytes ($80-$BF). 33 Long
        # params = 132 bytes, well over.
        params = ", ".join(f"long p{i}" for i in range(33))
        src = (
            f"__attribute__((zp_abi)) int f({params}) {{ return 0; }} "
            f"int main(void) {{ return 0; }}"
        )
        with self.assertRaises(AbiSelectionError) as cm:
            tac, c99, types = _compile_to_tac(src)
            select_abi(tac, c99, types)
        self.assertIn("ZP window", str(cm.exception))
        self.assertIn("`f`", str(cm.exception))

    def test_default_pool_window_used(self) -> None:
        tac, c99, types = _compile_to_tac(
            "__attribute__((zp_abi)) int f(int x) { return x; } "
            "int main(void) { return 0; }",
        )
        abi = select_abi(tac, c99, types)
        self.assertEqual(abi["f"].addrs[0], 0x80)

    def test_custom_pool_start(self) -> None:
        tac, c99, types = _compile_to_tac(
            "__attribute__((zp_abi)) int f(int x) { return x; } "
            "int main(void) { return 0; }",
        )
        abi = select_abi(tac, c99, types, pool=Pool(start=0x90))
        self.assertEqual(abi["f"].addrs, [0x90, 0x91])


if __name__ == "__main__":
    unittest.main()
