"""Tests for the IndexedStore recognizer + lowering.

The TAC pass `recognize_indexed_store` detects the canonical
absolute,X-store pattern

    ZeroExtend(uchar_var, %ext)
    Binary(Add, Constant(C), %ext, %addr)   # or commutative
    Store(val, %addr)

with `%ext` and `%addr` single-use, `uchar_var` 1-byte typed,
`val` 1-byte typed, and `C ≤ 0xFF00`. Rewrites to the new TAC
instruction `IndexedStore(C, uchar_var, val)`. tac_to_asm
lowers IndexedStore as `LDA val; LDX uchar_var; STA $C,X`
(absolute,X store on a folded numeric base).

Coverage:
  * Direct unit tests on synthetic TAC: canonical pattern fires;
    multi-use temps don't fold; non-uchar index doesn't fold;
    multi-byte src doesn't fold; out-of-range C doesn't fold.
  * Asm shape: the canonical `static T * const` indexed write
    collapses to a single `STA $XXXX,X` instruction.
  * End-to-end correctness via the sim.
"""
from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.recognize_indexed_store import (
    recognize_indexed_store,
)
from passes.type_checking import (
    LocalAttr, Symbol, SymbolTable,
)
from sim.harness import build_sim


def _fn(instrs: list[tac_ast.Type_instruction]) -> tac_ast.Function:
    return tac_ast.Function(
        name="f", is_global=True, params=[],
        instructions=list(instrs),
    )


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _table(entries: dict[str, Symbol]) -> SymbolTable:
    tbl = SymbolTable()
    for name, sym in entries.items():
        tbl[name] = sym
    return tbl


def _uchar_local(name: str) -> Symbol:
    return Symbol(type=c99_ast.UChar(), attrs=LocalAttr())


def _uint_local(name: str) -> Symbol:
    return Symbol(type=c99_ast.UInt(), attrs=LocalAttr())


def _int_local(name: str) -> Symbol:
    return Symbol(type=c99_ast.Int(), attrs=LocalAttr())


def _canonical_pattern(
    addr: int = 0x4000, addr_const_variant: type = tac_ast.ConstUInt,
) -> list[tac_ast.Type_instruction]:
    """The three-instruction template the recognizer matches:
    `ZeroExtend; Binary(Add, Constant, %ext); Store`."""
    return [
        tac_ast.ZeroExtend(src=_var("col"), dst=_var("%ext")),
        tac_ast.Binary(
            op=tac_ast.Add(),
            src1=tac_ast.Constant(const=addr_const_variant(value=addr)),
            src2=_var("%ext"),
            dst=_var("%addr"),
        ),
        tac_ast.Store(src=_var("value"), dst_ptr=_var("%addr")),
    ]


def _canonical_symbols() -> SymbolTable:
    return _table({
        "col": _uchar_local("col"),
        "value": _uchar_local("value"),
        "%ext": _uint_local("%ext"),
        "%addr": Symbol(
            type=c99_ast.Pointer(referenced_type=c99_ast.UChar()),
            attrs=LocalAttr(),
        ),
    })


class TestRecognizeIndexedStoreUnit(unittest.TestCase):
    """Direct calls to `recognize_indexed_store` on synthetic TAC."""

    def test_canonical_pattern_folds(self) -> None:
        instrs = _canonical_pattern(addr=0x4123)
        out = recognize_indexed_store(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        # Three instructions collapse to one IndexedStore.
        self.assertEqual(len(out.instructions), 1)
        ins = out.instructions[0]
        self.assertIsInstance(ins, tac_ast.IndexedStore)
        self.assertEqual(ins.address, 0x4123)
        self.assertEqual(ins.index, _var("col"))
        self.assertEqual(ins.src, _var("value"))

    def test_commutative_arrangement_folds(self) -> None:
        # Constant on src2 of the Binary instead of src1.
        instrs = [
            tac_ast.ZeroExtend(src=_var("col"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=_var("%ext"),
                src2=tac_ast.Constant(
                    const=tac_ast.ConstUInt(value=0x4000),
                ),
                dst=_var("%addr"),
            ),
            tac_ast.Store(src=_var("value"), dst_ptr=_var("%addr")),
        ]
        out = recognize_indexed_store(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(
            out.instructions[0], tac_ast.IndexedStore,
        )

    def test_no_symbols_is_noop(self) -> None:
        instrs = _canonical_pattern()
        out = recognize_indexed_store(_fn(instrs), symbols=None)
        # Without symbols, the pass can't verify operand widths;
        # leave the function unchanged.
        self.assertEqual(out.instructions, instrs)

    def test_multi_use_addr_does_not_fold(self) -> None:
        # %addr is read by both the Store AND a second consumer
        # → not single-use → don't fold.
        instrs = _canonical_pattern() + [
            tac_ast.Copy(src=_var("%addr"), dst=_var("%other")),
        ]
        out = recognize_indexed_store(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(out.instructions, instrs)

    def test_multi_use_ext_does_not_fold(self) -> None:
        # %ext has a second use → don't fold.
        instrs = _canonical_pattern() + [
            tac_ast.Copy(src=_var("%ext"), dst=_var("%other")),
        ]
        out = recognize_indexed_store(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(out.instructions, instrs)

    def test_sign_extend_def_folds(self) -> None:
        # SignExtend from a 1-byte source is now accepted under the
        # UB-permissive interpretation: the 6502's absolute,X uses
        # only the index's low byte, and negative array indices are
        # C99 §6.5.6 undefined behavior. The fold yields the same
        # absolute,X store as ZeroExtend would.
        instrs = [
            tac_ast.SignExtend(src=_var("col"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Constant(
                    const=tac_ast.ConstUInt(value=0x4000),
                ),
                src2=_var("%ext"),
                dst=_var("%addr"),
            ),
            tac_ast.Store(src=_var("value"), dst_ptr=_var("%addr")),
        ]
        symbols = _table({
            "col": Symbol(
                type=c99_ast.SChar(), attrs=LocalAttr(),
            ),
            "value": _uchar_local("value"),
            "%ext": _int_local("%ext"),
            "%addr": Symbol(
                type=c99_ast.Pointer(referenced_type=c99_ast.UChar()),
                attrs=LocalAttr(),
            ),
        })
        out = recognize_indexed_store(_fn(instrs), symbols=symbols)
        # Recognizer drops both the SignExtend and the Add, leaving
        # just the IndexedStore.
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(out.instructions[0], tac_ast.IndexedStore)
        self.assertEqual(out.instructions[0].address, 0x4000)
        self.assertEqual(out.instructions[0].index, _var("col"))

    def test_non_uchar_index_does_not_fold(self) -> None:
        # The Var underlying the ZeroExtend is `int`, not uchar.
        # The ZeroExtend's high-byte-zero invariant doesn't carry
        # for a wider source.
        instrs = _canonical_pattern()
        symbols = _table({
            "col": _int_local("col"),  # NOT uchar
            "value": _uchar_local("value"),
            "%ext": _uint_local("%ext"),
            "%addr": Symbol(
                type=c99_ast.Pointer(referenced_type=c99_ast.UChar()),
                attrs=LocalAttr(),
            ),
        })
        out = recognize_indexed_store(_fn(instrs), symbols=symbols)
        self.assertEqual(out.instructions, instrs)

    def test_uint_value_does_not_fold(self) -> None:
        # The Store's src is 16-bit (uint). The recognizer requires
        # 1-byte src — multi-byte STA $XXXX,X would need separate
        # writes and isn't handled here.
        instrs = _canonical_pattern()
        symbols = _table({
            "col": _uchar_local("col"),
            "value": _uint_local("value"),  # 16-bit
            "%ext": _uint_local("%ext"),
            "%addr": Symbol(
                type=c99_ast.Pointer(referenced_type=c99_ast.UInt()),
                attrs=LocalAttr(),
            ),
        })
        out = recognize_indexed_store(_fn(instrs), symbols=symbols)
        self.assertEqual(out.instructions, instrs)

    def test_address_above_ff00_does_not_fold(self) -> None:
        # C + 255 must fit in 16 bits. C = 0xFF80 → C + 255 =
        # 0x1007F, wraps. Skip.
        instrs = _canonical_pattern(addr=0xFF80)
        out = recognize_indexed_store(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(out.instructions, instrs)

    def test_address_at_boundary_folds(self) -> None:
        # C = 0xFF00 is the boundary: C + 255 = 0xFFFF, fits.
        instrs = _canonical_pattern(addr=0xFF00)
        out = recognize_indexed_store(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(
            out.instructions[0], tac_ast.IndexedStore,
        )
        self.assertEqual(out.instructions[0].address, 0xFF00)

    def test_address_negative_does_not_fold(self) -> None:
        # Defensive: a Constant value below 0 (theoretically
        # possible if a fold went rogue) shouldn't yield an
        # absolute,X address. Skip.
        instrs = [
            tac_ast.ZeroExtend(src=_var("col"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Constant(
                    const=tac_ast.ConstInt(value=-1),
                ),
                src2=_var("%ext"),
                dst=_var("%addr"),
            ),
            tac_ast.Store(src=_var("value"), dst_ptr=_var("%addr")),
        ]
        out = recognize_indexed_store(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(out.instructions, instrs)

    def test_constant_byte_source_folds(self) -> None:
        # Store.src can also be a Constant — a 1-byte typed
        # ConstUChar Constant is foldable.
        instrs = [
            tac_ast.ZeroExtend(src=_var("col"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Constant(
                    const=tac_ast.ConstUInt(value=0x4000),
                ),
                src2=_var("%ext"),
                dst=_var("%addr"),
            ),
            tac_ast.Store(
                src=tac_ast.Constant(
                    const=tac_ast.ConstUChar(value=0x42),
                ),
                dst_ptr=_var("%addr"),
            ),
        ]
        out = recognize_indexed_store(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(
            out.instructions[0], tac_ast.IndexedStore,
        )
        self.assertEqual(
            out.instructions[0].src,
            tac_ast.Constant(const=tac_ast.ConstUChar(value=0x42)),
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIndexedStoreAsmShape(unittest.TestCase):
    """Source-level checks on the emitted asm."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_canonical_indexed_store_collapses(self) -> None:
        # The textbook case: a `static T * const` with an
        # uchar offset folds to `STA $XXXX,X`. The full chain
        # is: const-static read fold + reassoc + recognize.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4123;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[col] = value;\n"
            "}\n"
            "int main(void) { put(0x42, 5); return 0; }\n"
        )
        asm = self._compile(src)
        # Expect `STA $4123,X` somewhere — the canonical indexed-
        # absolute store with a folded 16-bit base.
        self.assertIn("STA   $4123,X", asm)
        # No DPTR routing for this access.
        # (Other accesses might still use DPTR; we only assert the
        # presence of the optimized form.)

    def test_const_offset_indexed_store(self) -> None:
        # Combine a `static T * const` with an indexing offset
        # constant: `buf[K + col] = v` where K is a compile-time
        # constant. Reassoc folds K into the base, then recognize
        # fires.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[100 + col] = value;\n"
            "}\n"
            "int main(void) { put(0x77, 3); return 0; }\n"
        )
        asm = self._compile(src)
        # Base is 0x4000 + 100 = 0x4064.
        self.assertIn("STA   $4064,X", asm)

    def test_int_index_does_not_fold(self) -> None:
        # `int col` is 2-byte typed; the high byte isn't
        # provably zero, so the absolute,X form is unsound. The
        # unoptimized indirect path stays.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
            "void put(uint8_t value, int col) {\n"
            "    buf[col] = value;\n"
            "}\n"
            "int main(void) { put(0x42, 5); return 0; }\n"
        )
        asm = self._compile(src)
        # No absolute,X store here. The address is computed at
        # runtime through DPTR.
        self.assertNotIn("STA   $4000,X", asm)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIndexedStoreCorrectness(unittest.TestCase):
    """End-to-end: the optimized program writes to the right
    memory address."""

    def test_byte_lands_at_indexed_address(self) -> None:
        # Folded `STA $XXXX,X` writes the byte at `XXXX + col`.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4100;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[col] = value;\n"
            "}\n"
            "int main(void) { put(0x55, 17); return 0; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.memory[0x4100 + 17], 0x55)

    def test_offset_indexed_store_lands_correctly(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[200 + col] = value;\n"
            "}\n"
            "int main(void) { put(0x99, 50); return 0; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        # 0x4000 + 200 + 50 = 0x4000 + 250 = 0x40FA.
        self.assertEqual(result.memory[0x40FA], 0x99)

    def test_unoptimized_still_works(self) -> None:
        # The fold only fires under --optimize. Without it the
        # program still has to compute the right address — verify
        # both modes produce the same result.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4200;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[col] = value;\n"
            "}\n"
            "int main(void) { put(0x33, 9); return 0; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                result = sim.run(max_cycles=5_000_000)
                self.assertFalse(result.timed_out)
                self.assertEqual(result.memory[0x4200 + 9], 0x33)


if __name__ == "__main__":
    unittest.main()
