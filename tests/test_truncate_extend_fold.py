"""Unit tests for `passes.optimization.truncate_extend_fold`.

Covers the `Truncate(Extend(x), u)` → `Copy(x, u)` / narrower-
Truncate folds, plus the soundness gate that excludes non-SSA-
renamed names (globals, statics, address-taken locals) which can
have multiple definitions.
"""

import unittest

import c99_ast
import tac_ast
from passes.optimization.truncate_extend_fold import fold_truncate_extend
from passes.type_checking import LocalAttr, StaticAttr, Symbol


def _var(name):
    return tac_ast.Var(name=name)


def _fn(instrs):
    return tac_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _table(entries):
    return entries


def _local(c99_type):
    return Symbol(type=c99_type, attrs=LocalAttr())


def _static(c99_type):
    return Symbol(
        type=c99_type,
        attrs=StaticAttr(initial_value=None, is_global=True),
    )


class TestFoldTruncateExtend(unittest.TestCase):

    def test_signext_then_truncate_same_width_becomes_copy(self):
        # SignExtend(int8 i, int16 t); Truncate(int16 t, int8 u)
        # → Copy(i, u). The round-trip is identity since the
        # Truncate reads only byte_0 (= original i) of the
        # sign-extended value.
        instrs = [
            tac_ast.SignExtend(src=_var("i"), dst=_var("t")),
            tac_ast.Truncate(src=_var("t"), dst=_var("u")),
        ]
        symbols = _table({
            "i": _local(c99_ast.SChar()),
            "t": _local(c99_ast.Int()),
            "u": _local(c99_ast.SChar()),
        })
        out = fold_truncate_extend(
            _fn(instrs), symbols=symbols, ssa_dsts={"t", "u"},
        )
        # The Truncate becomes Copy(i, u). SignExtend still
        # present; SSA-DCE drops it on the next iteration.
        self.assertIsInstance(out.instructions[1], tac_ast.Copy)
        self.assertEqual(out.instructions[1].src, _var("i"))
        self.assertEqual(out.instructions[1].dst, _var("u"))

    def test_zeroext_then_truncate_same_width_becomes_copy(self):
        instrs = [
            tac_ast.ZeroExtend(src=_var("i"), dst=_var("t")),
            tac_ast.Truncate(src=_var("t"), dst=_var("u")),
        ]
        symbols = _table({
            "i": _local(c99_ast.UChar()),
            "t": _local(c99_ast.UInt()),
            "u": _local(c99_ast.UChar()),
        })
        out = fold_truncate_extend(
            _fn(instrs), symbols=symbols, ssa_dsts={"t", "u"},
        )
        self.assertIsInstance(out.instructions[1], tac_ast.Copy)
        self.assertEqual(out.instructions[1].src, _var("i"))

    def test_extend_then_wider_truncate_left_alone(self):
        # SignExtend(int8 i, int32 t); Truncate(int32 t, int16 u)
        # → leave as is. The Truncate reads bytes that the
        # SignExtend filled with sign-replication; replacing it
        # with `Truncate(i, u)` (or Copy) would lose the high
        # bytes.
        instrs = [
            tac_ast.SignExtend(src=_var("i"), dst=_var("t")),
            tac_ast.Truncate(src=_var("t"), dst=_var("u")),
        ]
        symbols = _table({
            "i": _local(c99_ast.SChar()),     # 1 byte
            "t": _local(c99_ast.Long()),      # 4 bytes
            "u": _local(c99_ast.Int()),       # 2 bytes — wider than i
        })
        out = fold_truncate_extend(
            _fn(instrs), symbols=symbols, ssa_dsts={"t", "u"},
        )
        self.assertIsInstance(out.instructions[1], tac_ast.Truncate)
        self.assertEqual(out.instructions[1].src, _var("t"))

    def test_global_dst_not_folded(self):
        # The Extend's dst is a global (not SSA-renamed). Even
        # though the def appears to "reach" the Truncate, globals
        # can be re-defined elsewhere — the rewrite would be
        # unsound. Truncate stays as is.
        instrs = [
            # ZeroExtend writes the global `ui` here. The Truncate
            # below also reads `ui` — but in real code, the
            # Truncate's read might come BEFORE this ZeroExtend
            # (the dst dict doesn't track execution order across
            # multi-def names).
            tac_ast.ZeroExtend(src=_var("t"), dst=_var("ui")),
            tac_ast.Truncate(src=_var("ui"), dst=_var("u")),
        ]
        symbols = _table({
            "t": _local(c99_ast.UChar()),
            "ui": _static(c99_ast.UInt()),
            "u": _local(c99_ast.UChar()),
        })
        # `ui` is NOT in ssa_dsts — it's a global.
        out = fold_truncate_extend(
            _fn(instrs), symbols=symbols, ssa_dsts={"t", "u"},
        )
        self.assertIsInstance(out.instructions[1], tac_ast.Truncate)
        self.assertEqual(out.instructions[1].src, _var("ui"))

    def test_no_ssa_dsts_is_noop(self):
        # Without an ssa_dsts argument, the pass returns unchanged.
        instrs = [
            tac_ast.SignExtend(src=_var("i"), dst=_var("t")),
            tac_ast.Truncate(src=_var("t"), dst=_var("u")),
        ]
        symbols = _table({
            "i": _local(c99_ast.SChar()),
            "t": _local(c99_ast.Int()),
            "u": _local(c99_ast.SChar()),
        })
        out = fold_truncate_extend(_fn(instrs), symbols=symbols)
        self.assertEqual(out.instructions, instrs)


if __name__ == "__main__":
    unittest.main()
