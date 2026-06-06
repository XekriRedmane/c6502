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
"""
from __future__ import annotations

import io
import shutil
import unittest
from unittest.mock import patch

from compile import main as compile_main
from sim.harness import build_sim


def _signed_int(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRegisterKeywordSim(unittest.TestCase):
    def _codegen(self, source: str) -> str:
        with patch("sys.stdin", io.StringIO(source)), \
             patch("sys.stdout", new_callable=io.StringIO) as out:
            rc = compile_main([
                "compile.py", "-", "--codegen", "--optimize",
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
        # `acc` instead of a ZP byte. (`n` is a plain param here — a
        # `register` param AND a `register` local together currently
        # force a soft-stack frame whose prologue clobbers the
        # incoming register arg; tracked as a follow-up.)
        src = (
            "char sum_n(char n) {"
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

    def test_caller_loads_second_arg_into_x(self) -> None:
        src = (
            "char add2(register char a, register char b) {"
            "    return (char)(a + b);"
            "}"
            "int main(void) { return add2(5, 7); }"
        )
        out = self._codegen(src)
        main_body = out[out.index("main:"):]
        # `b` (7) loads into X via the direct-into-X peephole.
        self.assertRegex(main_body, r"LDX\s+#\$07")
        # The register args don't get a slot store at the call site.
        self.assertNotRegex(main_body, r"STA\s+__zpabi_add2__b")

    # --- constraint / eligibility failures ---

    def _assert_compile_error(self, src: str, needle: str) -> None:
        with self.assertRaises(Exception) as cm:
            self._codegen(src)
        self.assertIn(needle, str(cm.exception))

    def test_pointer_register_param_rejected(self) -> None:
        src = (
            "char deref(register char *p) { return *p; }"
            "int main(void) { char c = 7; return deref(&c); }"
        )
        self._assert_compile_error(src, "pointer")

    def test_three_register_bytes_rejected(self) -> None:
        # Three 1-byte register params need three registers; only A
        # and X are available for args.
        src = (
            "char add3("
            "    register char a, register char b, register char c) {"
            "    return (char)(a + b + c);"
            "}"
            "int main(void) { return add3(1, 2, 3); }"
        )
        self._assert_compile_error(src, "register")

    def test_two_byte_register_local_rejected(self) -> None:
        # A `register` local gets only Y (one byte); a 2-byte int
        # doesn't fit.
        src = (
            "int main(void) {"
            "    register int x = 7;"
            "    return x;"
            "}"
        )
        self._assert_compile_error(src, "register")

    def test_address_of_register_param_rejected(self) -> None:
        src = (
            "char fst(register char x) {"
            "    char *p = &x;"
            "    return *p;"
            "}"
            "int main(void) { return fst(5); }"
        )
        self._assert_compile_error(src, "register")

    def test_register_on_recursive_function_rejected(self) -> None:
        # A `register` param requires zp_abi eligibility; recursion
        # makes the function ineligible -> hard error.
        src = (
            "int fib(register int n) {"
            "    if (n < 2) return n;"
            "    return fib(n - 1) + fib(n - 2);"
            "}"
            "int main(void) { return fib(5); }"
        )
        self._assert_compile_error(src, "register")


if __name__ == "__main__":
    unittest.main()
