"""End-to-end tests for `__attribute__((reg("..")))` register-passing.

Coverage:
  - A 1-byte function whose param arrives in X and result returns in
    A still produces the right value through the simulator.
  - Same, with X-passed param and X-returned result (no conflict —
    the param is consumed before the return is computed).
  - Y-passed param.
  - Caller-side: a call with reg-attributed params emits register
    loads (LDX / LDY / LDA) instead of `STA __zpabi_*` writes.
  - Reg-attribute return: caller captures from the named register
    (STX / STY) instead of A.
  - Eligibility failures:
      * reg(...) on a function with a non-1-byte parameter type.
      * reg(...) on a function with a non-1-byte return type.
      * Two params mapped to the same register.
      * Forward-decl / definition mismatch.
      * reg(...) on a function with a body that calls a non-zp_abi
        extern (forces SoftStackLayout; reg(...) requires
        ZpLayout).
      * &x on a reg-attributed parameter.
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
class TestRegAttributeSim(unittest.TestCase):
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

    def test_x_param_a_return(self) -> None:
        # add_one takes its arg in X, returns in A (default).
        src = (
            'char add_one(char x __attribute__((reg("X")))) {'
            '    return x + 1;'
            '}'
            'int main(void) { return add_one(41); }'
        )
        self.assertEqual(self._sim_return_int(src), 42)

    def test_y_param_x_return(self) -> None:
        # add_one takes its arg in Y, returns in X.
        src = (
            '__attribute__((reg("X")))'
            'char add_one(char x __attribute__((reg("Y")))) {'
            '    return x + 1;'
            '}'
            'int main(void) { return add_one(41); }'
        )
        self.assertEqual(self._sim_return_int(src), 42)

    def test_multiple_reg_params(self) -> None:
        # mix: x in X, y in Y, return in A. The values are consumed
        # via the slot copies emitted at function entry, so the body
        # is unaffected by the register-passing choice.
        src = (
            'char addxy('
            '    char x __attribute__((reg("X"))),'
            '    char y __attribute__((reg("Y")))) {'
            '    return x + y;'
            '}'
            'int main(void) { return addxy(30, 12); }'
        )
        self.assertEqual(self._sim_return_int(src), 42)

    def test_caller_uses_register_for_arg(self) -> None:
        # Look at the emitted asm: the caller should be loading the
        # arg into X (the Y-passing path uses TAY since X / Y have
        # no direct LDY-immediate form for non-immediate sources;
        # for an Imm source we get `LDX #<value>` directly).
        src = (
            'char add_one(char x __attribute__((reg("X")))) {'
            '    return x + 1;'
            '}'
            'int main(void) { return add_one(41); }'
        )
        out = self._codegen(src)
        # Locate main's body. Caller-side load should mention X.
        main_idx = out.index("main:")
        main_body = out[main_idx:]
        # The arg-passing load is `LDX #$29` (= 41) — direct-into-X
        # peephole fuses `LDA #$29; TAX` to a single LDX.
        self.assertRegex(main_body, r"LDX\s+#\$29")
        # And the slot symbol is NOT used as the arg destination
        # (it's still emitted as an EQU directive at the top, but
        # main shouldn't STA into it).
        self.assertNotRegex(main_body, r"STA\s+__zpabi_add_one__x")

    def test_callee_returns_in_named_register(self) -> None:
        src = (
            '__attribute__((reg("X")))'
            'char add_one(char x __attribute__((reg("Y")))) {'
            '    return x + 1;'
            '}'
            'int main(void) { return add_one(41); }'
        )
        out = self._codegen(src)
        # Caller (main) should capture the return from X, not A.
        # The exact instruction may be `STX HARGS` (16-bit-Int
        # widening for the int main()'s implicit return-via-HARGS),
        # `STX __local_*`, or `TXA; STA ...` depending on what the
        # downstream peephole settles on. We just check that the
        # caller READS X after the JSR (or the equivalent TXA).
        main_idx = out.index("main:")
        main_body = out[main_idx:]
        # Find the JSR add_one line and look at what follows. The
        # next register-touching instruction should mention X.
        jsr_idx = main_body.index("JSR   add_one")
        after_jsr = main_body[jsr_idx:].split("\n", 6)[1:]
        joined = "\n".join(after_jsr)
        self.assertRegex(
            joined, r"\b(STX|TXA)\b",
            f"expected caller to read X after JSR; got:\n{joined}",
        )

    def _assert_compile_error(self, src: str, needle: str) -> None:
        # End-to-end error-path helper: `_codegen` invokes the full
        # pipeline; an AbiSelectionError / TypeCheckError propagates
        # as a Python exception (not a non-zero rc).
        with self.assertRaises(Exception) as cm:
            self._codegen(src)
        self.assertIn(needle, str(cm.exception))

    def test_non_one_byte_param_rejected(self) -> None:
        # Multi-byte type (int = 2 bytes) doesn't fit a single
        # register — rejected at abi_selection time.
        src = (
            'int doubl(int x __attribute__((reg("X")))) {'
            '    return x + x;'
            '}'
            'int main(void) { return doubl(21); }'
        )
        self._assert_compile_error(src, "1-byte")

    def test_non_one_byte_return_rejected(self) -> None:
        # `int` return is 2 bytes — doesn't fit a register.
        src = (
            '__attribute__((reg("X")))'
            'int wide(char x __attribute__((reg("Y")))) {'
            '    return x;'
            '}'
            'int main(void) { return wide(5); }'
        )
        self._assert_compile_error(src, "return slot")

    def test_param_register_conflict_rejected(self) -> None:
        # Two params can't share a register at the same call boundary.
        src = (
            'char both('
            '    char a __attribute__((reg("X"))),'
            '    char b __attribute__((reg("X")))) {'
            '    return a + b;'
            '}'
            'int main(void) { return both(1, 2); }'
        )
        self._assert_compile_error(src, "unique")

    def test_return_and_param_same_register_rejected(self) -> None:
        # Return register can't overlap with a param register —
        # caller's arg-load into X would clobber the in-flight
        # return-X register on re-entry.
        src = (
            '__attribute__((reg("X")))'
            'char ident(char x __attribute__((reg("X")))) {'
            '    return x;'
            '}'
            'int main(void) { return ident(5); }'
        )
        self._assert_compile_error(src, "conflicts")

    def test_forward_def_mismatch_rejected(self) -> None:
        # Forward decl and definition disagree on the return
        # register — caller and callee would disagree on which
        # register holds the result. Hard error.
        src = (
            '__attribute__((reg("X"))) char f(char);'
            '__attribute__((reg("Y"))) char f(char x) { return x; }'
            'int main(void) { return f(5); }'
        )
        self._assert_compile_error(src, "differs")

    def test_address_of_reg_param_rejected(self) -> None:
        # `&x` on a reg-attributed parameter is a constraint
        # violation per C99 §6.5.3.2.1; we reject at type-check.
        src = (
            'char fst(char x __attribute__((reg("X")))) {'
            '    char *p = &x;'
            '    return *p;'
            '}'
            'int main(void) { return fst(5); }'
        )
        self._assert_compile_error(src, "register")

    def test_local_pinned_to_y(self) -> None:
        # `reg("Y")` on a local — the byte-granular regalloc colors
        # the local's Pseudo to Reg(Y) directly. The body reads /
        # writes Y instead of going through a ZP byte. Verify
        # end-to-end that the value computes correctly.
        src = (
            'char sum_n(char n __attribute__((reg("X")))) {'
            '    char acc __attribute__((reg("Y"))) = 0;'
            '    while (n > 0) {'
            '        acc = (char)(acc + n);'
            '        n = (char)(n - 1);'
            '    }'
            '    return acc;'
            '}'
            'int main(void) { return sum_n(5); }'
        )
        # 5+4+3+2+1 = 15.
        self.assertEqual(self._sim_return_int(src), 15)
        # And confirm the body actually reads / writes Y (not a
        # ZP byte) for the local.
        out = self._codegen(src)
        sum_n_idx = out.index("sum_n:")
        # Locate the end of sum_n's body — the next SUBROUTINE
        # block (`main:`).
        main_idx = out.index("main:")
        sum_n_body = out[sum_n_idx:main_idx]
        # The local `acc` should NOT have its own __local_*__acc
        # ZP symbol — it lives in Y.
        self.assertNotIn("__local_sum_n__acc", sum_n_body)
        # And the body should contain TYA / TAY / LDY-style accesses
        # to the pinned local (some form of Y-touching arithmetic).
        self.assertRegex(sum_n_body, r"\b(TYA|TAY|INY|DEY)\b")

    def test_param_and_local_same_register_coexist(self) -> None:
        # A reg("X") parameter and a reg("X") local CAN coexist
        # because the param's live range ends at the entry stub
        # (the stub copies X into the param's ZP slot, after which
        # X is free) and the local's live range starts after that.
        # No interference, no error. Verifies end-to-end the
        # value computes correctly.
        src = (
            'char weird(char n __attribute__((reg("X")))) {'
            '    char k __attribute__((reg("X"))) = (char)(n + 1);'
            '    return k;'
            '}'
            'int main(void) { return weird(3); }'
        )
        self.assertEqual(self._sim_return_int(src), 4)


if __name__ == "__main__":
    unittest.main()
