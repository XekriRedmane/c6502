"""Tests for `passes.optimization.three_way_compare`: a 2-byte unsigned
ordering compare and an equality compare of the same two values
split into a compare of the high bytes and then the low ones."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.framework import PassContext
from passes.optimization.three_way_compare import split_three_way_compares
from passes.type_checking import LocalAttr, Symbol, SymbolTable
from sim.harness import build_sim
from tests.no_inlining import no_inlining

# Values on both sides of each byte's edges, so every outcome of each
# byte's compare comes up.
_VALUES = (0x0000, 0x0001, 0x00FF, 0x0100, 0x0101, 0x01FF, 0x1233, 0x1234,
           0x1235, 0x1134, 0x1334, 0x8000, 0xFFFE, 0xFFFF)


def _classify(a: int, b: int) -> int:
    return 1 if a < b else 2 if a == b else 3


# Each body returns 1 for a < b, 2 for a == b, and 3 for a > b, given
# `uint16_t a, b`.
_SHAPES = {
    # The ordering compare's target starts with the equality compare,
    # which jumps when they differ.
    "less_then_equal": """
        if (a < b) return 1;
        if (a == b) return 2;
        return 3;""",
    # The same, with the equality compare jumping when they're equal.
    "less_then_not_equal": """
        if (a < b) return 1;
        if (a != b) return 3;
        return 2;""",
    # `<=` at the first compare, so the operands swap.
    "greater_then_equal": """
        if (a > b) return 3;
        if (a == b) return 2;
        return 1;""",
    # The two compares in a row.
    "adjacent": """
        if (a < b) goto less;
        if (a != b) goto greater;
        return 2;
    less:
        return 1;
    greater:
        return 3;""",
    # A constant operand.
    "constant": """
        (void)b;
        if (a < 0x1234u) return 1;
        if (a == 0x1234u) return 2;
        return 3;""",
    # Operands built from bytes, as in Zork's match_dictionary_word.
    "built_from_bytes": """
        uint8_t ka[2], kb[2];
        ka[0] = (uint8_t)a; ka[1] = (uint8_t)(a >> 8);
        kb[0] = (uint8_t)b; kb[1] = (uint8_t)(b >> 8);
        uint16_t x = ((uint16_t)ka[1] << 8) | ka[0];
        uint16_t y = ((uint16_t)kb[1] << 8) | kb[0];
        if (x < y) return 1;
        if (x == y) return 2;
        return 3;""",
    # A byte widened to 2 bytes, whose high byte is 0.
    "widened_byte": """
        uint8_t c = (uint8_t)a;
        if (c < b) return 1;
        if (c == b) return 2;
        return 3;""",
}

_EXPECTED = {
    "constant": lambda a, b: _classify(a, 0x1234),
    "widened_byte": lambda a, b: _classify(a & 0xFF, b),
}


def _program(body: str) -> str:
    table = ", ".join(f"0x{v:04X}u" for v in _VALUES)
    n = len(_VALUES)
    return (
        "#include <stdint.h>\n"
        f"uint16_t t[{n}] = {{{table}}};\n"
        "static uint8_t f(uint16_t a, uint16_t b) {" + body + "\n}\n"
        "int main(void) {\n"
        "    uint16_t n = 0;\n"
        f"    for (uint8_t i = 0; i < {n}; i++)\n"
        f"        for (uint8_t j = 0; j < {n}; j++)\n"
        "            n = (uint16_t)(n * 5u + f(t[i], t[j]));\n"
        "    return n;\n"
        "}\n"
    )


def _expected(name: str) -> int:
    pred = _EXPECTED.get(name, _classify)
    n = 0
    for a in _VALUES:
        for b in _VALUES:
            n = (n * 5 + pred(a, b)) & 0xFFFF
    return n


def _function_asm(src: str, name: str) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    asm = _run_stage("codegen", preprocess(src), optimize=True)
    start = asm.index(f"\n{name}:")
    return asm[start:asm.index("\nmain:", start)]


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
@no_inlining()
class TestRuns(unittest.TestCase):

    def test_shapes(self):
        for name, body in _SHAPES.items():
            for optimize in (False, True):
                with self.subTest(shape=name, optimize=optimize):
                    r = build_sim(_program(body), optimize=optimize).run()
                    self.assertEqual(r.return_int(), _expected(name))

    def test_no_two_byte_subtract(self):
        # A 2-byte ordering compare lowers to `SBC`, and a split one
        # to a `CMP` of each byte.
        for name, body in _SHAPES.items():
            with self.subTest(shape=name):
                self.assertNotIn("SBC", _function_asm(_program(body), "f"))

    def test_signed_compare_left_alone(self):
        body = _SHAPES["less_then_equal"].replace("a < b", "(int)a < (int)b")
        self.assertIn("SBC", _function_asm(_program(body), "f"))


def _var(name):
    return tac_ast.Var(name=name)


def _jcmp(op, a, b, target):
    return tac_ast.JumpIfCmp(op=op(), src1=_var(a), src2=_var(b),
                             target=target)


def _zero():
    return tac_ast.Constant(const=tac_ast.ConstUInt(value=0))


def _run(instrs, types):
    syms = SymbolTable()
    for name, t in types.items():
        syms[name] = Symbol(type=t, attrs=LocalAttr())
    fn = tac_ast.Function(name="f", is_global=True, params=list(types),
                          instructions=instrs)
    return split_three_way_compares(fn, PassContext(symbols=syms))


class TestPattern(unittest.TestCase):
    """The two compares of `if (a < b) return 0; if (a == b) ...`."""

    _UINT = {"a": c99_ast.UInt(), "b": c99_ast.UInt()}

    def _pair(self, j2_label_refs=0, x=None):
        instrs = [
            _jcmp(tac_ast.GreaterOrEqual, "a", "b", ".t1"),
            *(x if x is not None else [tac_ast.Ret(val=_zero())]),
            tac_ast.Label(name=".t1"),
            _jcmp(tac_ast.NotEqual, "a", "b", ".t2"),
            tac_ast.Ret(val=_var("a")),
            tac_ast.Label(name=".t2"),
            tac_ast.Ret(val=_var("b")),
        ]
        for _ in range(j2_label_refs):
            instrs = [tac_ast.Jump(target=".t1")] + instrs
        return instrs

    def test_splits(self):
        out = _run(self._pair(), self._UINT).instructions
        ops = [type(i.op).__name__ for i in out
               if isinstance(i, tac_ast.JumpIfCmp)]
        self.assertEqual(
            ops, ["LessThan", "NotEqual", "GreaterOrEqual", "NotEqual"],
        )
        # The second compare goes, since only the first jumped to it.
        self.assertNotIn(tac_ast.Label(name=".t1"), out)

    def test_keeps_a_second_compare_reached_otherwise(self):
        # Another jump still reaches the 2-byte compare at .t1, so it
        # stays.
        out = _run(self._pair(j2_label_refs=1), self._UINT).instructions
        self.assertIn(tac_ast.Label(name=".t1"), out)
        self.assertIn(_jcmp(tac_ast.NotEqual, "a", "b", ".t2"), out)

    def test_signed_left_alone(self):
        instrs = self._pair()
        out = _run(instrs, {"a": c99_ast.Int(), "b": c99_ast.Int()})
        self.assertEqual(out.instructions, instrs)

    def test_less_outcome_must_end_in_a_terminator(self):
        # The block for the low bytes' second compare goes in front of
        # .t1, so nothing may fall into it.
        g = tac_ast.Var(name="g")
        instrs = self._pair(x=[tac_ast.Copy(src=_zero(), dst=g)])
        types = dict(self._UINT, g=c99_ast.UInt())
        self.assertEqual(_run(instrs, types).instructions, instrs)

    def test_after_a_subtract_left_alone(self):
        # tac_to_asm branches on the subtract's flags instead.
        d = tac_ast.Var(name="d")
        instrs = [tac_ast.Binary(op=tac_ast.Subtract(), src1=_var("a"),
                                 src2=_var("b"), dst=d)] + self._pair()
        types = dict(self._UINT, d=c99_ast.UInt())
        self.assertEqual(_run(instrs, types).instructions, instrs)


if __name__ == "__main__":
    unittest.main()
