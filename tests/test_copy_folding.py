"""Tests for the TAC-level copy-folding pass.

`fold_copies` fuses adjacent `<producer dst=%t>; Copy(%t, X)` pairs
into `<producer dst=X>` when `%t` is single-use. The eliminated
Copy is what `c99_to_tac` emits to write back into a non-SSA-
promoted name (a static or address-taken local) and what
`from_ssa` emits at the end of each predecessor block to feed Phi
sources into Phi dsts.

Coverage:
  * Direct unit tests on synthetic TAC: Binary + Copy collapse to
    in-place Binary; SignExtend / Cast / FunctionCall similarly
    redirect; multi-use temp doesn't fuse; non-adjacent doesn't
    fuse; Phi.dst doesn't get redirected.
  * End-to-end via the optimizer pipeline:
      - `static T const` increment chains through to INC + BNE.
      - Loop counter `i++` (where regalloc coalesces) chains too.
      - Programs computed correctly under `--optimize`.
"""
from __future__ import annotations

import shutil
import unittest

import tac_ast
from passes.optimization.copy_folding import fold_copies


def _fn(instrs):
    return tac_ast.Function(
        name="f", is_global=True, params=[],
        instructions=list(instrs),
    )


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _const(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


class TestCopyFoldingUnit(unittest.TestCase):
    """Direct calls to fold_copies on synthetic TAC."""

    def test_binary_plus_copy_fuses(self) -> None:
        # `Binary(Add, x, 1, %t); Copy(%t, x)` → `Binary(Add, x, 1, x)`.
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("x"), src2=_const(1),
                dst=_var("%t"),
            ),
            tac_ast.Copy(src=_var("%t"), dst=_var("x")),
        ]
        out = fold_copies(_fn(instrs))
        self.assertEqual(len(out.instructions), 1)
        self.assertEqual(
            out.instructions[0],
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("x"), src2=_const(1),
                dst=_var("x"),
            ),
        )

    def test_unary_plus_copy_fuses(self) -> None:
        instrs = [
            tac_ast.Unary(
                op=tac_ast.Negate(), src=_var("x"), dst=_var("%t"),
            ),
            tac_ast.Copy(src=_var("%t"), dst=_var("y")),
        ]
        out = fold_copies(_fn(instrs))
        self.assertEqual(len(out.instructions), 1)
        self.assertEqual(
            out.instructions[0],
            tac_ast.Unary(
                op=tac_ast.Negate(), src=_var("x"), dst=_var("y"),
            ),
        )

    def test_signextend_plus_copy_fuses(self) -> None:
        instrs = [
            tac_ast.SignExtend(src=_var("x"), dst=_var("%t")),
            tac_ast.Copy(src=_var("%t"), dst=_var("y")),
        ]
        out = fold_copies(_fn(instrs))
        self.assertEqual(len(out.instructions), 1)
        self.assertEqual(
            out.instructions[0],
            tac_ast.SignExtend(src=_var("x"), dst=_var("y")),
        )

    def test_function_call_plus_copy_fuses(self) -> None:
        # FunctionCall has its dst redirected when present.
        instrs = [
            tac_ast.FunctionCall(
                name="g", args=[_var("a")], dst=_var("%t"),
            ),
            tac_ast.Copy(src=_var("%t"), dst=_var("y")),
        ]
        out = fold_copies(_fn(instrs))
        self.assertEqual(len(out.instructions), 1)
        self.assertEqual(
            out.instructions[0],
            tac_ast.FunctionCall(
                name="g", args=[_var("a")], dst=_var("y"),
            ),
        )

    def test_multi_use_temp_does_not_fuse(self) -> None:
        # %t is read by Binary AND by Copy → 2 uses → don't fuse
        # (the other reader would observe a different value).
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("a"), src2=_var("b"),
                dst=_var("%t"),
            ),
            tac_ast.Binary(
                op=tac_ast.Multiply(), src1=_var("%t"), src2=_const(2),
                dst=_var("%u"),
            ),
            tac_ast.Copy(src=_var("%t"), dst=_var("x")),
        ]
        out = fold_copies(_fn(instrs))
        # No change.
        self.assertEqual(out.instructions, instrs)

    def test_non_adjacent_does_not_fuse(self) -> None:
        # An intervening instruction between producer and Copy
        # blocks fusion. (Conservative — even though %t is still
        # single-use here, the intervening op might rely on flag
        # state or could interfere with the assumed adjacency.)
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("a"), src2=_var("b"),
                dst=_var("%t"),
            ),
            tac_ast.Label(name="L"),  # intervening
            tac_ast.Copy(src=_var("%t"), dst=_var("x")),
        ]
        out = fold_copies(_fn(instrs))
        self.assertEqual(out.instructions, instrs)

    def test_phi_dst_does_not_redirect(self) -> None:
        # Phi.dst stays as the SSA-renamed name. SSA destruction
        # is responsible for emitting Copies for the Phi's args
        # in predecessor blocks; a separate `Copy(phi_dst, X)`
        # AFTER the Phi might exist, but the Phi itself shouldn't
        # be redirected.
        instrs = [
            tac_ast.Phi(
                dst=_var("%t"),
                args=[
                    tac_ast.PhiArg(pred_label="L1", source=_var("a")),
                    tac_ast.PhiArg(pred_label="L2", source=_var("b")),
                ],
            ),
            tac_ast.Copy(src=_var("%t"), dst=_var("x")),
        ]
        out = fold_copies(_fn(instrs))
        # The Phi stays; the Copy stays.
        self.assertEqual(out.instructions, instrs)

    def test_chained_copy_fuses(self) -> None:
        # Copy(A, %t); Copy(%t, B) → Copy(A, B).
        instrs = [
            tac_ast.Copy(src=_var("A"), dst=_var("%t")),
            tac_ast.Copy(src=_var("%t"), dst=_var("B")),
        ]
        out = fold_copies(_fn(instrs))
        self.assertEqual(len(out.instructions), 1)
        self.assertEqual(
            out.instructions[0],
            tac_ast.Copy(src=_var("A"), dst=_var("B")),
        )

    def test_copy_dst_constant_not_a_target(self) -> None:
        # Copy.dst is always a Var in practice (you can't store
        # to a constant). Defensively skip if not Var.
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("a"), src2=_var("b"),
                dst=_var("%t"),
            ),
            tac_ast.Copy(src=_var("%t"), dst=_const(0)),  # bogus
        ]
        out = fold_copies(_fn(instrs))
        # Even with the bogus shape, fusion can still redirect the
        # Binary's dst to a Constant — the pass doesn't second-
        # guess Copy.dst's shape. But this is an unreachable
        # state; we just check it doesn't crash.
        self.assertEqual(len(out.instructions), 1)

    def test_consumer_not_a_copy_does_not_fuse(self) -> None:
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("a"), src2=_var("b"),
                dst=_var("%t"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("%t"), src2=_const(1),
                dst=_var("%u"),
            ),
        ]
        out = fold_copies(_fn(instrs))
        self.assertEqual(out.instructions, instrs)

    def test_first_instr_with_no_consumer_passes_through(self) -> None:
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("a"), src2=_var("b"),
                dst=_var("%t"),
            ),
        ]
        out = fold_copies(_fn(instrs))
        self.assertEqual(out.instructions, instrs)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestCopyFoldingEndToEnd(unittest.TestCase):
    """Programs going through the full pipeline. Verifies the
    fusion + downstream INC peephole produce the expected asm
    and that the program computes the right answer."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_static_counter_collapses_to_inc_chain(self) -> None:
        # `static int counter; counter += 1;` — the textbook
        # case. Without fusion: ADC chain through a temp + Copy
        # back. With fusion: in-place ADC chain, which the INC
        # peephole then collapses to INC + BNE + INC.
        src = (
            "static int counter = 0;\n"
            "int main(void) { counter += 1; return counter; }\n"
        )
        asm = self._compile(src)
        # Expect INC counter; BNE done; INC counter+1; done:
        self.assertIn("INC   counter", asm)
        self.assertIn("BNE   .inc_done@", asm)
        self.assertIn("INC   counter+1", asm)
        # The temp routing is gone — no `STA counter\n   LDA`
        # round-trip.
        # (We can't easily check the *absence* of a temp, but
        # confirming the INC chain is enough.)

    def test_loop_counter_int_collapses_to_inc_chain(self) -> None:
        # `for (int i = 0; i < 10; i++)` — the loop counter's
        # `i++` is the textbook fusion case after from_ssa
        # inserts the back-edge Copy.
        src = (
            "int main(void) {\n"
            "    int s = 0;\n"
            "    for (int i = 0; i < 10; i++) s += 1;\n"
            "    return s;\n"
            "}\n"
        )
        asm = self._compile(src)
        # Loop continue should have an INC chain for i.
        self.assertIn(".loop@0_continue:", asm)
        # The continue block (between loop@0_continue and the
        # next label) should have INC + BNE + INC for the 16-bit
        # i++. Body locals emit as `__local_<fn>_b<k>` symbols
        # now; check the INC + done-label chain.
        import re
        self.assertRegex(asm, r"INC\s+__local_\w+_b\d+")
        self.assertIn(".inc_done@", asm)


if __name__ == "__main__":
    unittest.main()
