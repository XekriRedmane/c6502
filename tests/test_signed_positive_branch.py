"""Tests for `tac_to_asm._jcmp_signed_positive`, the branch on a signed
`x > 0` or `x <= 0` that tests the sign bit and ORs the bytes.

The `-S` text is checked in cpp/tests/test_signed_positive_branch.cpp."""

from __future__ import annotations

import unittest
from tests.cpp_build import needs_c6502cpp


_TYPES = {
    "signed char": (-128, -1, 0, 1, 127),
    "int": (-32768, -256, -1, 0, 1, 255, 256, 32767),
    "long": (-2147483647 - 1, -65536, -1, 0, 1, 256, 65536, 2147483647),
}


def _program(type_name: str, values) -> str:
    # Each value is read from a volatile array, so no comparison folds.
    # Bit k of a test's result says whether the condition held for the
    # kth value. The four tests are the two operators in each operand
    # order.
    lines = [
        f"volatile {type_name} v[{len(values)}] = "
        f"{{{', '.join(f'({type_name})({x})' for x in values)}}};",
        "unsigned int pos(void) { unsigned int r = 0;",
        f"  for (unsigned char k = 0; k < {len(values)}; k++) {{",
        f"    {type_name} x = v[k];",
        "    if (x > 0) r |= 1u << k; }",
        "  return r; }",
        "unsigned int nonpos(void) { unsigned int r = 0;",
        f"  for (unsigned char k = 0; k < {len(values)}; k++) {{",
        f"    {type_name} x = v[k];",
        "    if (x <= 0) r |= 1u << k; }",
        "  return r; }",
        "unsigned int pos_rev(void) { unsigned int r = 0;",
        f"  for (unsigned char k = 0; k < {len(values)}; k++) {{",
        f"    {type_name} x = v[k];",
        "    if (0 < x) r |= 1u << k; }",
        "  return r; }",
        "unsigned int nonpos_rev(void) { unsigned int r = 0;",
        f"  for (unsigned char k = 0; k < {len(values)}; k++) {{",
        f"    {type_name} x = v[k];",
        "    if (0 >= x) r |= 1u << k; }",
        "  return r; }",
        "unsigned int which;",
        "int main(void) {",
        "  if (which == 0) return pos();",
        "  if (which == 1) return nonpos();",
        "  if (which == 2) return pos_rev();",
        "  return nonpos_rev(); }",
    ]
    return "\n".join(lines) + "\n"


@needs_c6502cpp
class TestSignedPositiveBranch(unittest.TestCase):

    def test_runs(self):
        from sim.harness import build_sim
        for type_name, values in _TYPES.items():
            positive = sum(1 << k for k, x in enumerate(values) if x > 0)
            full = (1 << len(values)) - 1
            expected = (positive, full ^ positive, positive,
                        full ^ positive)
            for which, want in enumerate(expected):
                src = _program(type_name, values).replace(
                    "unsigned int which;", f"unsigned int which = {which};")
                for optimize in (False, True):
                    with self.subTest(type=type_name, which=which,
                                      optimize=optimize):
                        out = build_sim(src, optimize=optimize).run()
                        self.assertEqual(out.return_int() & 0xFFFF, want)


if __name__ == "__main__":
    unittest.main()
