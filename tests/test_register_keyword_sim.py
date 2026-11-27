"""End-to-end tests for the `register` keyword's register passing.

Register placement is POSITIONAL: the first register arg-byte goes in
A, the second in X; a `register` local uses Y. The return value rides
in A (low) / X (high) for any non-pointer return of <=2 bytes,
independent of the keyword. Pointers are never placed in registers.

Coverage:
  - Two 1-byte register params (a -> A, b -> X) compute correctly.
  - A 2-byte register param spreads low -> A, high -> X.
  - A 1-byte register return (A) and a 2-byte register return (A/X).
  - A `register` local pinned to Y.
  - Caller emits `LDX #..` for the second register arg-byte.
  - Eligibility / constraint failures:
      * a pointer `register` parameter,
      * a third register arg-byte (over the 2-byte A/X budget),
      * a 2-byte `register` local (a local gets only Y),
      * `&x` on a `register` parameter,
      * `register` params on a recursive (zp_abi-ineligible) function.

The tests that only read compile.py's output or errors are doctest
cases in cpp/tests/test_register_keyword_sim.cpp.
"""
from __future__ import annotations

import io
import unittest
from unittest.mock import patch

from compile import main as compile_main
from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


def _signed_int(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


@needs_c6502cpp
class TestRegisterKeywordSim(unittest.TestCase):
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

    def test_two_register_params_a_x(self) -> None:
        # a -> A, b -> X; result returns in A.
        src = (
            "char add2(register char a, register char b) {"
            "    return (char)(a + b);"
            "}"
            "int main(void) { return add2(30, 12); }"
        )
        self.assertEqual(self._sim_return_int(src), 42)

    def test_two_byte_register_param_spreads_a_x(self) -> None:
        # A 2-byte `register` param: low byte -> A, high byte -> X.
        src = (
            "unsigned int echo(register unsigned int a) { return a; }"
            "int main(void) { return (int)echo(0x1234); }"
        )
        self.assertEqual(self._sim_return_int(src) & 0xFFFF, 0x1234)

    def test_one_byte_register_return_in_a(self) -> None:
        src = (
            "char add_one(register char x) { return (char)(x + 1); }"
            "int main(void) { return add_one(41); }"
        )
        self.assertEqual(self._sim_return_int(src), 42)

    def test_two_byte_return_in_a_x(self) -> None:
        # A 2-byte int return rides in A (low) / X (high).
        src = (
            "unsigned int widen(register char x) {"
            "    return (unsigned int)((unsigned int)x << 8 | 0x07);"
            "}"
            "int main(void) { return (int)widen(0x12); }"
        )
        self.assertEqual(self._sim_return_int(src) & 0xFFFF, 0x1207)

    def test_local_pinned_to_y(self) -> None:
        # A `register` local pins to Y; the body reads / writes Y for
        # `acc` instead of a ZP byte. Exercised alongside a `register`
        # param (`n` -> A) — both together must stay bare-exit (no
        # soft-stack frame whose prologue would clobber the incoming
        # register arg).
        src = (
            "char sum_n(register char n) {"
            "    register char acc = 0;"
            "    while (n > 0) {"
            "        acc = (char)(acc + n);"
            "        n = (char)(n - 1);"
            "    }"
            "    return acc;"
            "}"
            "int main(void) { return sum_n(5); }"
        )
        self.assertEqual(self._sim_return_int(src), 15)  # 5+4+3+2+1
        out = self._codegen(src)
        sum_n_idx = out.index("sum_n:")
        main_idx = out.index("main:")
        sum_n_body = out[sum_n_idx:main_idx]
        self.assertNotIn("__local_sum_n__acc", sum_n_body)
        self.assertRegex(sum_n_body, r"\b(TYA|TAY|INY|DEY)\b")

    # --- constraint / eligibility failures ---

if __name__ == "__main__":
    unittest.main()
