"""Tests for passes/optimization/split_tables.py, which stores a table
of 2-byte (or wider) elements read by a byte index as its low bytes
and then its high bytes."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from compile import _run_stage
from passes.optimization.split_tables import apply_split_tables
from passes.type_checking import Initial, StaticAttr, Symbol
from preprocessor import preprocess
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


def _v(n):
    return tac_ast.Var(name=n)


def _c(v):
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=v))


def _local(t):
    from passes.type_checking import LocalAttr
    return Symbol(type=t, attrs=LocalAttr())


def _symbols(count=4, elem=c99_ast.UInt(), is_global=False):
    return {
        "T": Symbol(
            type=c99_ast.Array(
                element_type=c99_ast.Const(referenced_type=elem), size=count,
            ),
            attrs=StaticAttr(
                initial_value=Initial(value=tuple(range(count))),
                is_global=is_global,
            ),
        ),
        "@i": _local(c99_ast.UChar()),
        "%s": _local(c99_ast.UChar()),
        "%d": _local(c99_ast.UInt()),
        "%e": _local(c99_ast.UInt()),
        "%a": _local(c99_ast.Pointer(referenced_type=c99_ast.UInt())),
        "%z": _local(c99_ast.Int()),
        "%w": _local(c99_ast.Int()),
        "%p": _local(c99_ast.Pointer(referenced_type=c99_ast.UInt())),
    }


def _table(values=(0x1234, 0x5678, 0x9ABC, 0xDEF0), is_global=False):
    return tac_ast.StaticVariable(
        name="T", is_global=is_global, data_type=tac_ast.Pointer(),
        init=[tac_ast.UIntInit(value=v) for v in values],
    )


def _prog(instrs, *top):
    return tac_ast.Program(top_level=[
        tac_ast.Function(
            name="f", is_global=True, params=["@i"], instructions=instrs,
        ),
        *top,
    ])


def _shifted_read(offset=0):
    return [
        tac_ast.Binary(
            op=tac_ast.LeftShift(), src1=_v("@i"), src2=_c(1), dst=_v("%s"),
        ),
        tac_ast.IndexedLoad(
            name="T", index=_v("%s"), dst=_v("%d"), offset=offset,
        ),
        tac_ast.Ret(val=_v("%d")),
    ]


def _function(prog):
    return next(tl for tl in prog.top_level
                if isinstance(tl, tac_ast.Function))


def _static(prog, name="T"):
    return next(tl for tl in prog.top_level
                if isinstance(tl, tac_ast.StaticVariable) and tl.name == name)


class TestSplitTablesUnit(unittest.TestCase):

    def test_shifted_index_read(self):
        symbols = _symbols()
        out = apply_split_tables(_prog(_shifted_read(offset=2), _table()), symbols)
        self.assertEqual(_function(out).instructions, [
            tac_ast.IndexedLoad(
                name="T", index=_v("@i"), dst=_v("%d"), offset=1, plane=4,
            ),
            tac_ast.Ret(val=_v("%d")),
        ])
        # The low bytes, then the high bytes.
        self.assertEqual(_static(out).init, [tac_ast.StringInit(
            str="".join(map(chr, [
                0x34, 0x78, 0xBC, 0xF0, 0x12, 0x56, 0x9A, 0xDE,
            ])),
            bytes=8,
        )])
        t = symbols["T"].type
        self.assertEqual(t.size, 8)
        self.assertEqual(
            t.element_type, c99_ast.Const(referenced_type=c99_ast.UChar()),
        )

    def test_constant_index(self):
        symbols = _symbols()
        prog = _prog([
            tac_ast.IndexedLoad(name="T", index=_c(4), dst=_v("%d")),
            tac_ast.Ret(val=_v("%d")),
        ], _table())
        out = apply_split_tables(prog, symbols)
        self.assertEqual(_function(out).instructions[0], tac_ast.IndexedLoad(
            name="T", index=_c(2), dst=_v("%d"), offset=0, plane=4,
        ))

    def test_long_table_through_a_pointer(self):
        # The shape c99_to_tac writes for a table of more than 255
        # bytes: GetAddress + (ZeroExtend(i) << 1), then a Load.
        symbols = _symbols()
        prog = _prog([
            tac_ast.GetAddress(operand=_v("T"), dst=_v("%a")),
            tac_ast.ZeroExtend(src=_v("@i"), dst=_v("%z")),
            tac_ast.Binary(
                op=tac_ast.LeftShift(), src1=_v("%z"),
                src2=tac_ast.Constant(const=tac_ast.ConstInt(value=1)),
                dst=_v("%w"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_v("%a"), src2=_v("%w"), dst=_v("%p"),
            ),
            tac_ast.Load(src_ptr=_v("%p"), dst=_v("%d"), is_volatile=False),
            tac_ast.Ret(val=_v("%d")),
        ], _table())
        out = apply_split_tables(prog, symbols)
        self.assertEqual(_function(out).instructions, [
            tac_ast.IndexedLoad(
                name="T", index=_v("@i"), dst=_v("%d"), offset=0, plane=4,
            ),
            tac_ast.Ret(val=_v("%d")),
        ])

    def test_pointer_table_gets_address_bytes(self):
        symbols = _symbols(count=2, elem=c99_ast.Pointer(
            referenced_type=c99_ast.UChar(),
        ))
        table = tac_ast.StaticVariable(
            name="T", is_global=False, data_type=tac_ast.Pointer(),
            init=[tac_ast.AddressInit(name="a", offset=0),
                  tac_ast.AddressInit(name="b", offset=3)],
        )
        out = apply_split_tables(_prog(_shifted_read(), table), symbols)
        self.assertEqual(_static(out).init, [
            tac_ast.AddressByteInit(name="a", offset=0, high=False),
            tac_ast.AddressByteInit(name="b", offset=3, high=False),
            tac_ast.AddressByteInit(name="a", offset=0, high=True),
            tac_ast.AddressByteInit(name="b", offset=3, high=True),
        ])

    def test_any_other_use_keeps_the_table(self):
        address_taken = _shifted_read()[:-1] + [
            tac_ast.GetAddress(operand=_v("T"), dst=_v("%p")),
            tac_ast.Ret(val=_v("%d")),
        ]
        stored = _shifted_read()[:-1] + [
            tac_ast.IndexedSymbolStore(name="T", index=_c(0), src=_v("%d")),
            tac_ast.Ret(val=_v("%d")),
        ]
        byte_read = _shifted_read()[:-1] + [
            tac_ast.IndexedLoad(name="T", index=_c(1), dst=_v("@i")),
            tac_ast.Ret(val=_v("%d")),
        ]
        for name, instrs in (("address", address_taken),
                             ("store", stored), ("byte", byte_read)):
            with self.subTest(name):
                prog = _prog(instrs, _table())
                self.assertEqual(apply_split_tables(prog, _symbols()), prog)
        with self.subTest("initializer"):
            other = tac_ast.StaticVariable(
                name="q", is_global=False, data_type=tac_ast.Pointer(),
                init=[tac_ast.AddressInit(name="T", offset=2)],
            )
            prog = _prog(_shifted_read(), _table(), other)
            self.assertEqual(apply_split_tables(prog, _symbols()), prog)

    def test_index_changed_before_the_read(self):
        instrs = _shifted_read()
        instrs.insert(1, tac_ast.Copy(src=_c(0), dst=_v("@i")))
        prog = _prog(instrs, _table())
        self.assertEqual(apply_split_tables(prog, _symbols()), prog)

    def test_global_or_writable_table_is_left_alone(self):
        prog = _prog(_shifted_read(), _table(is_global=True))
        self.assertEqual(
            apply_split_tables(prog, _symbols(is_global=True)), prog,
        )
        symbols = _symbols()
        symbols["T"] = Symbol(
            type=c99_ast.Array(element_type=c99_ast.UInt(), size=4),
            attrs=symbols["T"].attrs,
        )
        prog = _prog(_shifted_read(), _table())
        self.assertEqual(apply_split_tables(prog, symbols), prog)


def _codegen(src: str) -> str:
    return _run_stage("codegen", preprocess(src), optimize=True)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSplitTablesSim(unittest.TestCase):
    """Programs that read split tables, run in both pipelines."""

    def _check(self, src: str, expected: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, expected & 0xFFFF)

    def test_uint16_table(self):
        src = """
        #include <stdint.h>
        static const uint16_t T[8] = {
            0x0102, 0x0304, 0x1000, 0x2000, 0x1234, 0xFF00, 7, 0x8001,
        };
        uint16_t get(uint8_t i) { return T[i]; }
        uint16_t next(uint8_t i) { return T[i + 1]; }
        int main(void) {
            uint16_t s = T[4];
            for (uint8_t i = 0; i < 8; i++) s += get(i);
            return s + next(6) + next(0);
        }
        """
        values = [0x0102, 0x0304, 0x1000, 0x2000, 0x1234, 0xFF00, 7, 0x8001]
        self._check(src, 0x1234 + sum(values) + 0x8001 + 0x0304)
        body = _codegen(src).split("\nget:\n")[1].split("RTS")[0]
        self.assertIn("T+8,X", body)
        self.assertNotIn("ASL", body)

    def test_long_table(self):
        # 200 elements, 400 bytes: the 16-bit address path.
        src = """
        #include <stdint.h>
        static const uint16_t T[200] = {11, 0x1234, 0xBEEF};
        uint16_t get(uint8_t i) { return T[i]; }
        int main(void) { return get(0) + get(1) + get(2) + get(199); }
        """
        self._check(src, 11 + 0x1234 + 0xBEEF)
        body = _codegen(src).split("\nget:\n")[1].split("RTS")[0]
        self.assertIn("T+200,X", body)

    def test_pointer_table(self):
        # Nine pointers, one more than the dispatch on a small
        # pointer table takes.
        src = """
        #include <stdint.h>
        static const uint8_t a[2] = {1, 2}, b[2] = {3, 4}, c[2] = {5, 6};
        static const uint8_t *const P[9] = {a, b, c, a + 1, b + 1, c + 1,
                                            a, b, c};
        uint8_t get(uint8_t i, uint8_t j) { return P[i][j]; }
        int main(void) {
            uint8_t s = 0;
            for (uint8_t i = 0; i < 9; i++) s += get(i, 0) * (i + 1);
            return s + get(2, 1);
        }
        """
        firsts = [1, 3, 5, 2, 4, 6, 1, 3, 5]
        self._check(src, sum(v * (k + 1) for k, v in enumerate(firsts)) + 6)
        self.assertIn("DC.B  <(a+1)", _codegen(src))

    def test_uint32_table(self):
        src = """
        #include <stdint.h>
        static const uint32_t T[3] = {0x01020304, 0x0A0B0C0D, 0xFFFFFFFF};
        uint32_t get(uint8_t i) { return T[i]; }
        int main(void) {
            return (int)(get(1) >> 16) + (int)(get(0) & 0xFFFF)
                + (int)(get(2) >> 24);
        }
        """
        self._check(src, 0x0A0B + 0x0304 + 0xFF)
        self.assertIn("T+6,X", _codegen(src))


if __name__ == "__main__":
    unittest.main()
