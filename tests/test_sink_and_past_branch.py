"""Tests for the TAC sinker that moves
`ZeroExtend; BitwiseAnd; Truncate` past a `JumpIfMasked`."""

import unittest

import c99_ast
import tac_ast
from passes.optimization.sink_and_past_branch import sink_and_past_branch
from passes.type_checking import LocalAttr, Symbol


def _sym(t):
    return Symbol(type=t, attrs=LocalAttr())


def _make_symbols(types: dict[str, c99_ast.Type_data_type]):
    return {name: _sym(t) for name, t in types.items()}


def _wrap(instrs, name="f"):
    return tac_ast.Function(
        name=name, is_global=False, params=[], instructions=instrs,
    )


class TestSinkAndPastBranchMatching(unittest.TestCase):
    """The four-instruction trio + JumpIfMasked pattern detection."""

    def _build_apply_bobble_shape(self):
        """The motivating TAC shape from apply_bobble.c:

            bobble:           UChar
            %x_ext = ZeroExtend(bobble)
            %x_and = BitwiseAnd(%x_ext, ConstInt(0x7F))
            %t     = Truncate(%x_and)            ; magnitude
            JumpIfMasked(bobble, 0x80, jne=False, .else)
            ; fall-through (add path): uses %t
            Copy(%t, %use_then)
            Jump(.end)
            .else:
            ; target (sub path): uses %t
            Copy(%t, %use_else)
            .end:
            Ret(None)
        """
        symbols = _make_symbols({
            "bobble": c99_ast.UChar(),
            "%x_ext": c99_ast.UInt(),
            "%x_and": c99_ast.UInt(),
            "%t": c99_ast.UChar(),
            "%use_then": c99_ast.UChar(),
            "%use_else": c99_ast.UChar(),
        })
        instrs = [
            tac_ast.ZeroExtend(
                src=tac_ast.Var(name="bobble"),
                dst=tac_ast.Var(name="%x_ext"),
            ),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=tac_ast.Var(name="%x_ext"),
                src2=tac_ast.Constant(
                    const=tac_ast.ConstInt(value=0x7F),
                ),
                dst=tac_ast.Var(name="%x_and"),
            ),
            tac_ast.Truncate(
                src=tac_ast.Var(name="%x_and"),
                dst=tac_ast.Var(name="%t"),
            ),
            tac_ast.JumpIfMasked(
                val=tac_ast.Var(name="bobble"),
                mask=0x80,
                jump_when_nonzero=False,
                target=".else",
            ),
            # Fall-through block (add path).
            tac_ast.Copy(
                src=tac_ast.Var(name="%t"),
                dst=tac_ast.Var(name="%use_then"),
            ),
            tac_ast.Jump(target=".end"),
            # Target block (sub path).
            tac_ast.Label(name=".else"),
            tac_ast.Copy(
                src=tac_ast.Var(name="%t"),
                dst=tac_ast.Var(name="%use_else"),
            ),
            # Merge.
            tac_ast.Label(name=".end"),
            tac_ast.Ret(val=None),
        ]
        return _wrap(instrs), symbols

    def test_apply_bobble_shape_sinks(self):
        fn, symbols = self._build_apply_bobble_shape()
        out = sink_and_past_branch(fn, symbols=symbols)
        # The original trio at indices 0..2 should be gone from
        # the source-order list; the JumpIfMasked stays.
        # Walk the new instructions and verify the structural
        # shape: terminator first (JumpIfMasked), then per-branch
        # trio copies.
        instrs = out.instructions
        # First three instructions should NO LONGER be the
        # original trio (the original %t etc. are gone).
        original_t_defs = [
            i for i in instrs
            if isinstance(i, tac_ast.Truncate)
            and isinstance(i.dst, tac_ast.Var)
            and i.dst.name == "%t"
        ]
        self.assertEqual(
            original_t_defs, [],
            "Original %t Truncate should have been sunk away",
        )
        # Should have two fresh `.snk_then@N` / `.snk_else@N` defs
        # for the magnitude — one per branch.
        then_t_defs = [
            i for i in instrs
            if isinstance(i, tac_ast.Truncate)
            and isinstance(i.dst, tac_ast.Var)
            and i.dst.name.startswith("%t.snk_then@")
        ]
        else_t_defs = [
            i for i in instrs
            if isinstance(i, tac_ast.Truncate)
            and isinstance(i.dst, tac_ast.Var)
            and i.dst.name.startswith("%t.snk_else@")
        ]
        self.assertEqual(len(then_t_defs), 1)
        self.assertEqual(len(else_t_defs), 1)
        # Symbol table should have entries for the fresh names.
        new_names = [
            n for n in symbols
            if n.endswith("@0") and "snk_" in n
        ]
        # Each of (%x_ext, %x_and, %t) × 2 branches = 6 fresh names.
        self.assertEqual(len(new_names), 6)
        # Uses of the original %t should be rewritten. The
        # fall-through's Copy(%t, %use_then) should now reference
        # the renamed %t.snk_then@N.
        copies = [i for i in instrs if isinstance(i, tac_ast.Copy)]
        self.assertEqual(len(copies), 2)
        copy_srcs = [c.src.name for c in copies]
        self.assertTrue(any(
            s.startswith("%t.snk_then@") for s in copy_srcs
        ))
        self.assertTrue(any(
            s.startswith("%t.snk_else@") for s in copy_srcs
        ))


class TestSinkAndPastBranchEligibility(unittest.TestCase):
    """Cases that must NOT sink — eligibility-gate verification."""

    def test_non_uchar_x_blocks(self):
        # `%x` is UInt (not 1-byte) — gating fails because the
        # asm-level narrowing would lose bits.
        symbols = _make_symbols({
            "x": c99_ast.UInt(),
            "%x_ext": c99_ast.UInt(),
            "%x_and": c99_ast.UInt(),
            "%t": c99_ast.UChar(),
            "%u": c99_ast.UChar(),
        })
        instrs = [
            tac_ast.ZeroExtend(
                src=tac_ast.Var(name="x"),
                dst=tac_ast.Var(name="%x_ext"),
            ),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=tac_ast.Var(name="%x_ext"),
                src2=tac_ast.Constant(
                    const=tac_ast.ConstInt(value=0x7F),
                ),
                dst=tac_ast.Var(name="%x_and"),
            ),
            tac_ast.Truncate(
                src=tac_ast.Var(name="%x_and"),
                dst=tac_ast.Var(name="%t"),
            ),
            tac_ast.JumpIfMasked(
                val=tac_ast.Var(name="x"),
                mask=0x80,
                jump_when_nonzero=False,
                target=".L",
            ),
            tac_ast.Copy(
                src=tac_ast.Var(name="%t"),
                dst=tac_ast.Var(name="%u"),
            ),
            tac_ast.Label(name=".L"),
            tac_ast.Ret(val=None),
        ]
        fn = _wrap(instrs)
        out = sink_and_past_branch(fn, symbols=symbols)
        self.assertEqual(out, fn, "non-uchar %x must block sinking")

    def test_out_of_range_mask_blocks(self):
        # AND constant doesn't fit in 0..0xFF.
        symbols = _make_symbols({
            "x": c99_ast.UChar(),
            "%x_ext": c99_ast.UInt(),
            "%x_and": c99_ast.UInt(),
            "%t": c99_ast.UChar(),
            "%u": c99_ast.UChar(),
        })
        instrs = [
            tac_ast.ZeroExtend(
                src=tac_ast.Var(name="x"),
                dst=tac_ast.Var(name="%x_ext"),
            ),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=tac_ast.Var(name="%x_ext"),
                src2=tac_ast.Constant(
                    const=tac_ast.ConstInt(value=0x1FF),
                ),
                dst=tac_ast.Var(name="%x_and"),
            ),
            tac_ast.Truncate(
                src=tac_ast.Var(name="%x_and"),
                dst=tac_ast.Var(name="%t"),
            ),
            tac_ast.JumpIfMasked(
                val=tac_ast.Var(name="x"),
                mask=0x80,
                jump_when_nonzero=False,
                target=".L",
            ),
            tac_ast.Copy(
                src=tac_ast.Var(name="%t"),
                dst=tac_ast.Var(name="%u"),
            ),
            tac_ast.Label(name=".L"),
            tac_ast.Ret(val=None),
        ]
        fn = _wrap(instrs)
        out = sink_and_past_branch(fn, symbols=symbols)
        self.assertEqual(out, fn)

    def test_use_past_merge_blocks(self):
        # %t is used after both branches have rejoined — would
        # require Phi insertion. The prototype declines.
        symbols = _make_symbols({
            "x": c99_ast.UChar(),
            "%x_ext": c99_ast.UInt(),
            "%x_and": c99_ast.UInt(),
            "%t": c99_ast.UChar(),
            "%u_post": c99_ast.UChar(),
        })
        instrs = [
            tac_ast.ZeroExtend(
                src=tac_ast.Var(name="x"),
                dst=tac_ast.Var(name="%x_ext"),
            ),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=tac_ast.Var(name="%x_ext"),
                src2=tac_ast.Constant(
                    const=tac_ast.ConstInt(value=0x7F),
                ),
                dst=tac_ast.Var(name="%x_and"),
            ),
            tac_ast.Truncate(
                src=tac_ast.Var(name="%x_and"),
                dst=tac_ast.Var(name="%t"),
            ),
            tac_ast.JumpIfMasked(
                val=tac_ast.Var(name="x"),
                mask=0x80,
                jump_when_nonzero=False,
                target=".else",
            ),
            # Fall-through is empty — straight to merge.
            tac_ast.Jump(target=".end"),
            tac_ast.Label(name=".else"),
            tac_ast.Jump(target=".end"),
            tac_ast.Label(name=".end"),
            # USE OF %t at the merge — would need a Phi.
            tac_ast.Copy(
                src=tac_ast.Var(name="%t"),
                dst=tac_ast.Var(name="%u_post"),
            ),
            tac_ast.Ret(val=None),
        ]
        fn = _wrap(instrs)
        out = sink_and_past_branch(fn, symbols=symbols)
        self.assertEqual(
            out, fn,
            "use of %t past the merge must block sinking",
        )

    def test_no_jumpifmasked_terminator_blocks(self):
        symbols = _make_symbols({
            "x": c99_ast.UChar(),
            "%x_ext": c99_ast.UInt(),
            "%x_and": c99_ast.UInt(),
            "%t": c99_ast.UChar(),
        })
        instrs = [
            tac_ast.ZeroExtend(
                src=tac_ast.Var(name="x"),
                dst=tac_ast.Var(name="%x_ext"),
            ),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=tac_ast.Var(name="%x_ext"),
                src2=tac_ast.Constant(
                    const=tac_ast.ConstInt(value=0x7F),
                ),
                dst=tac_ast.Var(name="%x_and"),
            ),
            tac_ast.Truncate(
                src=tac_ast.Var(name="%x_and"),
                dst=tac_ast.Var(name="%t"),
            ),
            tac_ast.Ret(val=tac_ast.Var(name="%t")),
        ]
        fn = _wrap(instrs)
        out = sink_and_past_branch(fn, symbols=symbols)
        self.assertEqual(out, fn)

    def test_jumpifmasked_on_different_var_blocks(self):
        # The JumpIfMasked tests %y, not the %x that the trio
        # operates on. Sinking would be sound only if %y == %x.
        symbols = _make_symbols({
            "x": c99_ast.UChar(),
            "y": c99_ast.UChar(),
            "%x_ext": c99_ast.UInt(),
            "%x_and": c99_ast.UInt(),
            "%t": c99_ast.UChar(),
            "%u": c99_ast.UChar(),
        })
        instrs = [
            tac_ast.ZeroExtend(
                src=tac_ast.Var(name="x"),
                dst=tac_ast.Var(name="%x_ext"),
            ),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=tac_ast.Var(name="%x_ext"),
                src2=tac_ast.Constant(
                    const=tac_ast.ConstInt(value=0x7F),
                ),
                dst=tac_ast.Var(name="%x_and"),
            ),
            tac_ast.Truncate(
                src=tac_ast.Var(name="%x_and"),
                dst=tac_ast.Var(name="%t"),
            ),
            tac_ast.JumpIfMasked(
                val=tac_ast.Var(name="y"),
                mask=0x80,
                jump_when_nonzero=False,
                target=".L",
            ),
            tac_ast.Copy(
                src=tac_ast.Var(name="%t"),
                dst=tac_ast.Var(name="%u"),
            ),
            tac_ast.Label(name=".L"),
            tac_ast.Ret(val=None),
        ]
        fn = _wrap(instrs)
        out = sink_and_past_branch(fn, symbols=symbols)
        self.assertEqual(out, fn)


class TestSinkAndPastBranchNoSymbols(unittest.TestCase):
    def test_no_symbols_passes_through(self):
        # Without a symbol table the pass can't type-check %x.
        # It must no-op (consistent with other SSA-aware passes).
        instrs = [
            tac_ast.ZeroExtend(
                src=tac_ast.Var(name="x"),
                dst=tac_ast.Var(name="%e"),
            ),
            tac_ast.JumpIfMasked(
                val=tac_ast.Var(name="x"),
                mask=0x80,
                jump_when_nonzero=False,
                target=".L",
            ),
            tac_ast.Label(name=".L"),
            tac_ast.Ret(val=None),
        ]
        fn = _wrap(instrs)
        out = sink_and_past_branch(fn, symbols=None)
        self.assertEqual(out, fn)


if __name__ == "__main__":
    unittest.main()
