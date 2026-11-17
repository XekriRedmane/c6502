"""Tests for passes/optimization/row_address_tables.py, which looks up
the address of a row of a 2-D array in a table of row addresses."""

from __future__ import annotations

import shutil
import unittest
from unittest import mock

import c99_ast
import tac_ast
from compile import _run_stage
from passes.optimization import row_address_tables as rat
from passes.type_checking import (
    Initial, LocalAttr, NoInitializer, StaticAttr, Symbol, Tentative,
)
from preprocessor import preprocess
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


def _v(n):
    return tac_ast.Var(name=n)


def _int(v):
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _array(rows, row, address=None):
    return Symbol(
        type=c99_ast.Array(
            element_type=c99_ast.Array(element_type=c99_ast.UChar(), size=row),
            size=rows,
        ),
        attrs=StaticAttr(
            initial_value=NoInitializer() if address else Tentative(),
            is_global=True, address=address,
        ),
    )


def _symbols(**arrays):
    out = {
        "@i": Symbol(type=c99_ast.UChar(), attrs=LocalAttr()),
        "@w": Symbol(type=c99_ast.Int(), attrs=LocalAttr()),
    }
    for t in ("%a", "%b", "%p", "%q"):
        out[t] = Symbol(
            type=c99_ast.Pointer(referenced_type=c99_ast.UChar()),
            attrs=LocalAttr(),
        )
    for t in ("%z", "%x", "%m", "%y", "%n"):
        out[t] = Symbol(type=c99_ast.Int(), attrs=LocalAttr())
    out.update(arrays)
    return out


def _row(array, k=0, row=28, index="@i", names=("%a", "%z", "%x", "%m", "%p")):
    """The TAC c99_to_tac writes for `&array[index + k]`."""
    a, z, x, m, p = names
    out = [
        tac_ast.GetAddress(operand=_v(array), dst=_v(a)),
        tac_ast.ZeroExtend(src=_v(index), dst=_v(z)),
    ]
    src = z
    if k:
        op = tac_ast.Add() if k > 0 else tac_ast.Subtract()
        out.append(tac_ast.Binary(op=op, src1=_v(z), src2=_int(abs(k)), dst=_v(x)))
        src = x
    out += [
        tac_ast.Binary(op=tac_ast.Multiply(), src1=_v(src), src2=_int(row), dst=_v(m)),
        tac_ast.Binary(op=tac_ast.Add(), src1=_v(a), src2=_v(m), dst=_v(p)),
    ]
    return out


def _prog(*instrs):
    return tac_ast.Program(top_level=[tac_ast.Function(
        name="f", is_global=True, params=["@i"],
        instructions=[*instrs, tac_ast.Ret(val=_v("%p"))],
    )])


def _function(prog):
    return next(tl for tl in prog.top_level if isinstance(tl, tac_ast.Function))


def _statics(prog):
    return {tl.name: tl for tl in prog.top_level
            if isinstance(tl, tac_ast.StaticVariable)}


_ALWAYS = mock.patch.object(rat, "_saving", lambda row: 1000)


class TestRowAddressTablesUnit(unittest.TestCase):

    def test_fixed_address_rows(self):
        symbols = _symbols(M=_array(16, 28, address=0x0800))
        with _ALWAYS:
            out = rat.apply_row_address_tables(_prog(*_row("M", k=-1)), symbols)
        self.assertEqual(_function(out).instructions, [
            tac_ast.IndexedLoad(
                name="M@rows", index=_v("@i"), dst=_v("%p"), offset=-1,
                plane=17,
            ),
            tac_ast.Ret(val=_v("%p")),
        ])
        addresses = [0x0800 + 28 * r for r in range(17)]
        table = _statics(out)["M@rows"]
        self.assertEqual(table.init, [tac_ast.StringInit(
            str="".join(chr(a & 0xFF) for a in addresses)
            + "".join(chr(a >> 8) for a in addresses),
            bytes=34,
        )])
        self.assertEqual(symbols["M@rows"].type.size, 34)

    def test_static_array_rows_are_address_bytes(self):
        symbols = _symbols(S=_array(20, 14))
        with _ALWAYS:
            out = rat.apply_row_address_tables(
                _prog(*_row("S", k=2, row=14)), symbols,
            )
        self.assertEqual(_function(out).instructions[0].offset, 2)
        init = _statics(out)["S@rows"].init
        self.assertEqual(len(init), 42)
        self.assertEqual(
            init[3], tac_ast.AddressByteInit(name="S", offset=42, high=False),
        )
        self.assertEqual(
            init[21 + 20],
            tac_ast.AddressByteInit(name="S", offset=280, high=True),
        )

    def test_arrays_share_low_bytes(self):
        # 0x200 apart: the low bytes of the row addresses are the same.
        symbols = _symbols(
            M=_array(16, 28, address=0x0800), B=_array(16, 28, address=0x0A00),
        )
        with _ALWAYS:
            out = rat.apply_row_address_tables(_prog(
                *_row("M"), *_row("B", names=("%b", "%y", "%x", "%n", "%q")),
            ), symbols)
        loads = [i for i in _function(out).instructions
                 if isinstance(i, tac_ast.IndexedLoad)]
        self.assertEqual(
            [(i.name, i.offset, i.plane) for i in loads],
            [("B@rows", 0, 34), ("B@rows", 0, 17)],
        )
        self.assertEqual(symbols["B@rows"].type.size, 51)

    def test_cost_model(self):
        # One access saves 40 bytes, and 16 rows cost 34.
        symbols = _symbols(M=_array(16, 28, address=0x0800))
        out = rat.apply_row_address_tables(_prog(*_row("M")), symbols)
        self.assertIn("M@rows", _statics(out))
        # 40 rows cost 82 bytes, which two accesses don't save.
        symbols = _symbols(M=_array(40, 28, address=0x0800))
        prog = _prog(*_row("M"), *_row("M", names=("%b", "%y", "%x", "%n", "%q")))
        self.assertEqual(rat.apply_row_address_tables(prog, symbols), prog)
        # Two arrays with one access each: neither saves its 34 bytes
        # alone, but with the low bytes shared, the pair costs 51 and
        # saves 80.
        symbols = _symbols(
            M=_array(16, 28, address=0x0800), B=_array(16, 28, address=0x0A00),
        )
        symbols.update(M=_array(16, 28, address=0x0800))
        with mock.patch.object(rat, "_saving", lambda row: 30):
            out = rat.apply_row_address_tables(_prog(
                *_row("M"), *_row("B", names=("%b", "%y", "%x", "%n", "%q")),
            ), symbols)
        self.assertIn("B@rows", _statics(out))

    def test_shapes_left_alone(self):
        cases = {
            # A power-of-two row is two shifts.
            "power of two": (_array(16, 32, address=0x0800), _row("M", row=32)),
            # The index is an int, not a byte.
            "int index": (_array(16, 28, address=0x0800),
                          _row("M", index="@w")),
            # Past the end of the table.
            "past the end": (_array(16, 28, address=0x0800), _row("M", k=17)),
        }
        for name, (array, instrs) in cases.items():
            with self.subTest(name), _ALWAYS:
                prog = _prog(*instrs)
                self.assertEqual(
                    rat.apply_row_address_tables(prog, _symbols(M=array)), prog,
                )
        with self.subTest("zero page"), _ALWAYS:
            sym = _array(4, 28)
            sym = Symbol(type=sym.type, attrs=StaticAttr(
                initial_value=Tentative(), is_global=True, zeropage=True,
            ))
            prog = _prog(*_row("M"))
            self.assertEqual(
                rat.apply_row_address_tables(prog, _symbols(M=sym)), prog,
            )
        with self.subTest("the row address is read twice"), _ALWAYS:
            instrs = _row("M")
            instrs.insert(2, tac_ast.Copy(src=_v("%z"), dst=_v("%y")))
            prog = _prog(*instrs)
            self.assertEqual(rat.apply_row_address_tables(
                prog, _symbols(M=_array(16, 28, address=0x0800)),
            ), prog)


def _codegen(src: str) -> str:
    return _run_stage("codegen", preprocess(src), optimize=True)


def _cell(r: int, c: int) -> int:
    """What `_FILL` stores in row `r`, column `c`."""
    return (r * 16 + c) & 0xFF


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestRowAddressTablesSim(unittest.TestCase):
    """Programs that index 2-D arrays through row tables, run in both
    pipelines. The tables are made whatever they cost, so that each
    access is rewritten."""

    def _check(self, src: str, expected: int, table: str) -> None:
        with _ALWAYS:
            self.assertIn(f"{table}@rows", _codegen(src))
            for optimize in (False, True):
                with self.subTest(optimize=optimize):
                    got = build_sim(src, optimize=optimize).run().return_int()
                    self.assertEqual(got, expected & 0xFFFF)

    _FILL = """
        for (uint8_t r = 0; r < ROWS; r++)
            for (uint8_t c = 0; c < COLS; c++)
                A[r][c] = (uint8_t)(r * 16 + c);
    """

    def test_fixed_address_array(self):
        src = """
        #include <stdint.h>
        #define ROWS 16
        #define COLS 28
        extern uint8_t A [[c6502::address(0x6000)]][ROWS][COLS];
        uint8_t get(uint8_t r, uint8_t c) { return A[r][c]; }
        uint8_t above(uint8_t r, uint8_t c) { return A[r - 1][c]; }
        uint8_t below(uint8_t r, uint8_t c) { return A[r + 1][c]; }
        void put(uint8_t r, uint8_t c, uint8_t v) { A[r][c] = v; }
        uint8_t *row(uint8_t r) { return A[r]; }
        uint8_t *cell(uint8_t r, uint8_t c) { return &A[r][c]; }
        uint8_t (*whole(uint8_t r))[COLS] { return &A[r]; }
        int main(void) {
            """ + self._FILL + """
            put(5, 6, 200);
            int s = get(5, 6) + get(15, 27) + above(1, 3) + below(14, 0);
            s += row(2)[4] + *cell(3, 7) + (*whole(4))[1];
            // One past the last row.
            if ((uint8_t *)whole(ROWS) != &A[0][0] + ROWS * COLS) return -1;
            if (row(ROWS) != &A[0][0] + ROWS * COLS) return -2;
            return s;
        }
        """
        self._check(
            src,
            200 + _cell(15, 27) + _cell(0, 3) + _cell(15, 0) + _cell(2, 4)
            + _cell(3, 7) + _cell(4, 1),
            "A",
        )

    def test_static_array(self):
        src = """
        #include <stdint.h>
        #define ROWS 20
        #define COLS 14
        static uint8_t A[ROWS][COLS];
        uint8_t get(uint8_t r, uint8_t c) { return A[r][c]; }
        uint8_t above(uint8_t r, uint8_t c) { return A[r - 1][c]; }
        uint8_t *end(uint8_t r) { return A[r]; }
        int main(void) {
            """ + self._FILL + """
            if (end(ROWS) != &A[0][0] + ROWS * COLS) return -1;
            return get(19, 13) + get(0, 0) + above(1, 2) + get(7, 1);
        }
        """
        self._check(
            src, _cell(19, 13) + _cell(0, 0) + _cell(0, 2) + _cell(7, 1), "A",
        )

    def test_two_byte_elements(self):
        # Rows of 7 uint16_t, 14 bytes.
        src = """
        #include <stdint.h>
        static uint16_t W[30][7];
        uint16_t get(uint8_t r, uint8_t c) { return W[r][c]; }
        void put(uint8_t r, uint8_t c, uint16_t v) { W[r][c] = v; }
        int main(void) {
            for (uint8_t r = 0; r < 30; r++)
                for (uint8_t c = 0; c < 7; c++)
                    put(r, c, (uint16_t)(r * 1000 + c));
            return get(29, 6) - get(3, 2) + get(0, 1);
        }
        """
        self._check(src, (29 * 1000 + 6) - (3 * 1000 + 2) + 1, "W")

    def test_array_of_structs(self):
        src = """
        #include <stdint.h>
        struct cell { uint8_t a, b, c; };
        static struct cell S[100];
        uint8_t b_of(uint8_t i) { return S[i].b; }
        int main(void) {
            for (uint8_t i = 0; i < 100; i++) S[i].b = i + 1;
            return b_of(99) + b_of(0) + b_of(50);
        }
        """
        self._check(src, 100 + 1 + 51, "S")

    def test_shared_low_bytes(self):
        src = """
        #include <stdint.h>
        extern uint8_t M [[c6502::address(0x6000)]][16][28];
        extern uint8_t B [[c6502::address(0x6200)]][16][28];
        uint8_t both(uint8_t r, uint8_t c) { return M[r][c] + B[r][c]; }
        int main(void) {
            M[3][4] = 10; B[3][4] = 20; M[15][27] = 1; B[15][27] = 2;
            return both(3, 4) * 10 + both(15, 27);
        }
        """
        self._check(src, 300 + 3, "B")
        with _ALWAYS:
            text = _codegen(src)
        table = text.split("\nB@rows:\n")[1]
        self.assertEqual(table.count("$"), 51)


if __name__ == "__main__":
    unittest.main()
