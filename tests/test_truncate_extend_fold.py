"""Unit tests for `passes.optimization.truncate_extend_fold`.

Covers the `Truncate(Extend(x), u)` → `Copy(x, u)` / narrower-
Truncate folds, plus the soundness gate that excludes non-SSA-
renamed names (globals, statics, address-taken locals) which can
have multiple definitions.
"""

import shutil
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
            _fn(instrs), symbols=symbols, ssa_dsts={"i", "t", "u"},
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
            _fn(instrs), symbols=symbols, ssa_dsts={"i", "t", "u"},
        )
        self.assertIsInstance(out.instructions[1], tac_ast.Copy)
        self.assertEqual(out.instructions[1].src, _var("i"))

    def test_extend_then_wider_truncate_is_a_narrower_extend(self):
        # SignExtend(int8 i, int32 t); Truncate(int32 t, int16 u)
        # → SignExtend(i, u). The Truncate keeps one byte of the
        # sign replication, which a sign extension to u makes too.
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
            _fn(instrs), symbols=symbols, ssa_dsts={"i", "t", "u"},
        )
        self.assertEqual(
            out.instructions[1],
            tac_ast.SignExtend(src=_var("i"), dst=_var("u")),
        )

    def test_zero_extend_then_wider_truncate(self):
        instrs = [
            tac_ast.ZeroExtend(src=_var("i"), dst=_var("t")),
            tac_ast.Truncate(src=_var("t"), dst=_var("u")),
        ]
        symbols = _table({
            "i": _local(c99_ast.UChar()),
            "t": _local(c99_ast.ULong()),
            "u": _local(c99_ast.UInt()),
        })
        out = fold_truncate_extend(
            _fn(instrs), symbols=symbols, ssa_dsts={"i", "t", "u"},
        )
        self.assertEqual(
            out.instructions[1],
            tac_ast.ZeroExtend(src=_var("i"), dst=_var("u")),
        )

    def test_truncate_of_truncate(self):
        # Truncate(int32 x, int16 t); Truncate(int16 t, int8 u) is
        # Truncate(x, u).
        instrs = [
            tac_ast.Truncate(src=_var("x"), dst=_var("t")),
            tac_ast.Truncate(src=_var("t"), dst=_var("u")),
        ]
        symbols = _table({
            "x": _local(c99_ast.Long()),
            "t": _local(c99_ast.Int()),
            "u": _local(c99_ast.SChar()),
        })
        out = fold_truncate_extend(
            _fn(instrs), symbols=symbols, ssa_dsts={"x", "t", "u"},
        )
        self.assertEqual(
            out.instructions[1],
            tac_ast.Truncate(src=_var("x"), dst=_var("u")),
        )

    def test_extend_then_wider_truncate_of_non_ssa_source_left_alone(self):
        # Without an SSA-renamed `i`, `i` may have changed between the
        # extend and the Truncate.
        instrs = [
            tac_ast.SignExtend(src=_var("i"), dst=_var("t")),
            tac_ast.Truncate(src=_var("t"), dst=_var("u")),
        ]
        symbols = _table({
            "i": _local(c99_ast.SChar()),
            "t": _local(c99_ast.Long()),
            "u": _local(c99_ast.Int()),
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

    def test_non_ssa_source_not_folded(self):
        # `long l2 = (int)g; g += 1; f((signed char)l2);` with a
        # global `g`: reading `g` at the Truncate would see the
        # incremented value. The source must be SSA-renamed.
        instrs = [
            tac_ast.SignExtend(src=_var("g"), dst=_var("t")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("g"),
                src2=tac_ast.Constant(const=tac_ast.ConstChar(value=1)),
                dst=_var("g"),
            ),
            tac_ast.Truncate(src=_var("t"), dst=_var("u")),
        ]
        symbols = _table({
            "g": _static(c99_ast.SChar()),
            "t": _local(c99_ast.Long()),
            "u": _local(c99_ast.SChar()),
        })
        out = fold_truncate_extend(
            _fn(instrs), symbols=symbols, ssa_dsts={"t", "u"},
        )
        self.assertEqual(out.instructions[2], instrs[2])

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


def _fold_extend_extend(instrs, symbols, ssa):
    from passes.optimization.framework import PassContext
    from passes.optimization.truncate_extend_fold import FoldExtendExtend
    return FoldExtendExtend().run(
        _fn(instrs), PassContext(symbols=symbols, ssa_dsts=ssa),
    )


class TestFoldExtendExtend(unittest.TestCase):
    """An extend of a widening extend is one extend."""

    def _case(self, inner, outer):
        instrs = [
            inner(src=_var("i"), dst=_var("t")),
            outer(src=_var("t"), dst=_var("u")),
        ]
        symbols = {
            "i": _local(c99_ast.UChar()),
            "t": _local(c99_ast.Int()),
            "u": _local(c99_ast.ULong()),
        }
        return _fold_extend_extend(instrs, symbols, {"i", "t", "u"})

    def test_sign_extend_of_zero_extend_is_zero_extend(self):
        # `(unsigned long)i` for a `uint8_t i`: the promotion to `int`
        # leaves the top bit 0, so the sign extension adds zeros.
        out = self._case(tac_ast.ZeroExtend, tac_ast.SignExtend)
        self.assertEqual(
            out.instructions[1],
            tac_ast.ZeroExtend(src=_var("i"), dst=_var("u")),
        )

    def test_zero_extend_of_zero_extend(self):
        out = self._case(tac_ast.ZeroExtend, tac_ast.ZeroExtend)
        self.assertEqual(
            out.instructions[1],
            tac_ast.ZeroExtend(src=_var("i"), dst=_var("u")),
        )

    def test_sign_extend_of_sign_extend(self):
        out = self._case(tac_ast.SignExtend, tac_ast.SignExtend)
        self.assertEqual(
            out.instructions[1],
            tac_ast.SignExtend(src=_var("i"), dst=_var("u")),
        )

    def test_zero_extend_of_sign_extend_left_alone(self):
        # The middle byte copies i's sign and the top bytes are 0.
        out = self._case(tac_ast.SignExtend, tac_ast.ZeroExtend)
        self.assertEqual(out.instructions[1].src, _var("t"))

    def test_non_ssa_source_left_alone(self):
        instrs = [
            tac_ast.ZeroExtend(src=_var("i"), dst=_var("t")),
            tac_ast.SignExtend(src=_var("t"), dst=_var("u")),
        ]
        symbols = {
            "i": _local(c99_ast.UChar()),
            "t": _local(c99_ast.Int()),
            "u": _local(c99_ast.ULong()),
        }
        out = _fold_extend_extend(instrs, symbols, {"t", "u"})
        self.assertEqual(out.instructions[1].src, _var("t"))


_SIZEOF_LOOPS = (
    "#include <stdint.h>\n"
    "uint8_t a[40];\n"
    "uint8_t b[40];\n"
    "uint8_t f(void) {\n"
    "    for (uint8_t i = 0; i < sizeof a; i++)\n"
    "        a[i] = b[i] + 1;\n"
    "    for (uint8_t i = 0; i < 40U; i++)\n"
    "        b[i] = a[i];\n"
    "    return b[39];\n"
    "}\n"
    "int main(void) {\n"
    "    for (uint8_t i = 0; i < 40; i++)\n"
    "        b[i] = i;\n"
    "    return f();\n"
    "}\n"
)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestByteLoopAgainstSizeof(unittest.TestCase):
    """`for (uint8_t i = 0; i < sizeof a; i++)` compares one byte. The
    compare is at the width of `size_t`, `unsigned long`, which
    promotes `i` to `int` and then to `unsigned long`, and `i < 40U`
    converts the promoted `i` to `unsigned int` with a copy that copy
    propagation removes after the compare has become a `JumpIfCmp`
    (`cmp_zero_jump_fold.NARROW_JUMP_COMPARE_RULE`)."""

    def test_one_byte_compare(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(_SIZEOF_LOOPS), optimize=True)
        body = asm[asm.index("\nf:"):asm.index("\nmain:")]
        self.assertEqual(body.count("CPX   #$28"), 2, body)
        self.assertNotIn("SBC", body)
        self.assertNotIn("sx_done", body)

    def test_runs(self):
        from sim.harness import build_sim
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_SIZEOF_LOOPS, optimize=optimize)
                self.assertEqual(sim.run().return_int(), 40)


if __name__ == "__main__":
    unittest.main()
