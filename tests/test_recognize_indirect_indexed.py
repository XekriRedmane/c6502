"""Tests for the IndirectIndexedLoad/Store recognizer + lowering.

The TAC pass `recognize_indirect_indexed` detects the canonical
indirect-(zp),Y access pattern

    ZeroExtend(uchar_var, %ext)
    Binary(Add, ptr_var, %ext, %addr)   # or commutative
    Load(%addr, dst)     -- or Store(src, %addr)

with `%ext` and `%addr` single-use, `uchar_var` 1-byte typed,
and dst (or src) 1-byte typed. The pointer side is a Var
(distinguishing from the IndexedStore/Load case where it's a
Constant). The rewrite produces IndirectIndexedLoad(ptr, index,
dst) or IndirectIndexedStore(ptr, index, src).

tac_to_asm lowers IndirectIndexedLoad as:
    <stage DPTR>   ; 4 instructions
    LDA index
    TAY
    LDA (DPTR),Y
    STA dst

vs the unrecognized ~10-instruction sequence (16-bit Add then
DPTR setup then LDY #0 then LDA (DPTR),Y).

Coverage:
  * Direct unit tests on synthetic TAC: canonical pattern fires
    for both Load and Store; multi-use temps don't fold; non-uchar
    index doesn't fold; multi-byte dst/src doesn't fold; pointer
    side must be a Var (Constant goes through the IndexedLoad/
    Store path instead).
  * Asm shape: the canonical `ptr[uchar_idx]` access emits
    `LDA (DPTR),Y` after a Y setup.
  * End-to-end correctness via the sim.
"""
from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.recognize_indirect_indexed import (
    recognize_indirect_indexed,
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


def _uchar_local() -> Symbol:
    return Symbol(type=c99_ast.UChar(), attrs=LocalAttr())


def _uint_local() -> Symbol:
    return Symbol(type=c99_ast.UInt(), attrs=LocalAttr())


def _int_local() -> Symbol:
    return Symbol(type=c99_ast.Int(), attrs=LocalAttr())


def _ptr_local() -> Symbol:
    return Symbol(
        type=c99_ast.Pointer(referenced_type=c99_ast.UChar()),
        attrs=LocalAttr(),
    )


def _canonical_load() -> list[tac_ast.Type_instruction]:
    """The three-instruction template for an indirect-indexed
    LOAD: `ZeroExtend; Binary(Add, ptr_var, %ext); Load`."""
    return [
        tac_ast.ZeroExtend(src=_var("y"), dst=_var("%ext")),
        tac_ast.Binary(
            op=tac_ast.Add(),
            src1=_var("p"),
            src2=_var("%ext"),
            dst=_var("%addr"),
        ),
        tac_ast.Load(src_ptr=_var("%addr"), dst=_var("b")),
    ]


def _canonical_store() -> list[tac_ast.Type_instruction]:
    """Mirror of `_canonical_load` for STORE."""
    return [
        tac_ast.ZeroExtend(src=_var("y"), dst=_var("%ext")),
        tac_ast.Binary(
            op=tac_ast.Add(),
            src1=_var("p"),
            src2=_var("%ext"),
            dst=_var("%addr"),
        ),
        tac_ast.Store(src=_var("value"), dst_ptr=_var("%addr")),
    ]


def _canonical_symbols() -> SymbolTable:
    return _table({
        "y": _uchar_local(),
        "b": _uchar_local(),
        "value": _uchar_local(),
        "p": _ptr_local(),
        "%ext": _uint_local(),
        "%addr": _ptr_local(),
    })


class TestRecognizeIndirectIndexedLoadUnit(unittest.TestCase):
    """Unit tests for the load shape."""

    def test_canonical_load_folds(self) -> None:
        instrs = _canonical_load()
        out = recognize_indirect_indexed(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(len(out.instructions), 1)
        ins = out.instructions[0]
        self.assertIsInstance(ins, tac_ast.IndirectIndexedLoad)
        self.assertEqual(ins.ptr, _var("p"))
        self.assertEqual(ins.index, _var("y"))
        self.assertEqual(ins.dst, _var("b"))

    def test_commutative_arrangement_folds(self) -> None:
        # `ptr` on the RHS of the Binary, %ext on the LHS.
        instrs = [
            tac_ast.ZeroExtend(src=_var("y"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=_var("%ext"),
                src2=_var("p"),
                dst=_var("%addr"),
            ),
            tac_ast.Load(src_ptr=_var("%addr"), dst=_var("b")),
        ]
        out = recognize_indirect_indexed(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(
            out.instructions[0], tac_ast.IndirectIndexedLoad,
        )

    def test_no_symbols_is_noop(self) -> None:
        instrs = _canonical_load()
        out = recognize_indirect_indexed(_fn(instrs), symbols=None)
        self.assertEqual(out.instructions, instrs)

    def test_multi_use_addr_does_not_fold(self) -> None:
        instrs = _canonical_load() + [
            tac_ast.Copy(src=_var("%addr"), dst=_var("%other")),
        ]
        out = recognize_indirect_indexed(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(out.instructions, instrs)

    def test_multi_use_ext_does_not_fold(self) -> None:
        instrs = _canonical_load() + [
            tac_ast.Copy(src=_var("%ext"), dst=_var("%other")),
        ]
        out = recognize_indirect_indexed(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(out.instructions, instrs)

    def test_constant_addend_does_not_fold(self) -> None:
        # The "ptr" side is a Constant — that's the
        # IndexedConstLoad recognizer's territory, not ours.
        instrs = [
            tac_ast.ZeroExtend(src=_var("y"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Constant(
                    const=tac_ast.ConstUInt(value=0x2000),
                ),
                src2=_var("%ext"),
                dst=_var("%addr"),
            ),
            tac_ast.Load(src_ptr=_var("%addr"), dst=_var("b")),
        ]
        out = recognize_indirect_indexed(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        # Doesn't match: the indirect-indexed pass requires a Var
        # ptr.
        self.assertEqual(out.instructions, instrs)

    def test_non_zero_extend_def_does_not_fold(self) -> None:
        # %ext is a SignExtend rather than a ZeroExtend — the
        # high byte may carry a non-zero sign extension, which
        # `(zp),Y` semantics don't account for.
        instrs = [
            tac_ast.SignExtend(src=_var("y"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=_var("p"),
                src2=_var("%ext"),
                dst=_var("%addr"),
            ),
            tac_ast.Load(src_ptr=_var("%addr"), dst=_var("b")),
        ]
        out = recognize_indirect_indexed(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(out.instructions, instrs)

    def test_non_uchar_index_does_not_fold(self) -> None:
        # `y` is `int`, not uchar. The ZeroExtend's high-byte-
        # zero invariant doesn't apply to a wider source.
        instrs = _canonical_load()
        symbols = _canonical_symbols()
        symbols["y"] = _int_local()
        out = recognize_indirect_indexed(_fn(instrs), symbols=symbols)
        self.assertEqual(out.instructions, instrs)

    def test_uint_dst_does_not_fold(self) -> None:
        # Multi-byte dst: would need multiple (DPTR),Y reads with
        # carry-safe Y advancement. Deferred.
        instrs = _canonical_load()
        symbols = _canonical_symbols()
        symbols["b"] = _uint_local()
        out = recognize_indirect_indexed(_fn(instrs), symbols=symbols)
        self.assertEqual(out.instructions, instrs)


class TestRecognizeIndirectIndexedStoreUnit(unittest.TestCase):
    """Unit tests for the store shape."""

    def test_canonical_store_folds(self) -> None:
        instrs = _canonical_store()
        out = recognize_indirect_indexed(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(len(out.instructions), 1)
        ins = out.instructions[0]
        self.assertIsInstance(ins, tac_ast.IndirectIndexedStore)
        self.assertEqual(ins.ptr, _var("p"))
        self.assertEqual(ins.index, _var("y"))
        self.assertEqual(ins.src, _var("value"))

    def test_constant_byte_source_folds(self) -> None:
        # `*p_indexed = const_byte` — store of a 1-byte typed
        # Constant.
        instrs = [
            tac_ast.ZeroExtend(src=_var("y"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=_var("p"),
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
        out = recognize_indirect_indexed(
            _fn(instrs), symbols=_canonical_symbols(),
        )
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(
            out.instructions[0], tac_ast.IndirectIndexedStore,
        )

    def test_uint_src_does_not_fold(self) -> None:
        instrs = _canonical_store()
        symbols = _canonical_symbols()
        symbols["value"] = _uint_local()
        out = recognize_indirect_indexed(_fn(instrs), symbols=symbols)
        self.assertEqual(out.instructions, instrs)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIndirectIndexedAsmShape(unittest.TestCase):
    """Source-level checks on the emitted asm."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_indirect_indexed_load_emits_short_sequence(self) -> None:
        # `ptr[idx]` where ptr is a zp_abi parameter and idx is a
        # uchar local. The indirect-Y access should appear without
        # a 16-bit Add chain.
        src = (
            "#include <stdint.h>\n"
            "static volatile uint8_t result;\n"
            "__attribute__((zp_abi))\n"
            "void copy(uint8_t *src, uint8_t idx) {\n"
            "    result = src[idx];\n"
            "}\n"
            "int main(void) { return 0; }\n"
        )
        asm = self._compile(src)
        # Expect `LDA (DPTR),Y` with no preceding 16-bit ADC of idx.
        self.assertIn("LDA   (DPTR),Y", asm)
        # The unrecognized form would emit `CLC` for the 16-bit
        # Add inside `copy` — verify that's gone.
        body_start = asm.index("copy:")
        body_end = asm.index("\n\n", body_start)
        copy_body = asm[body_start:body_end]
        self.assertNotIn("CLC", copy_body)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIndirectIndexedCorrectness(unittest.TestCase):
    """End-to-end: the optimized program reads/writes the right
    addresses via the indirect-indexed path."""

    def test_load_reads_correct_byte(self) -> None:
        # Set up an 8-byte source array; have a zp_abi function
        # read byte 5 of it into a global. The optimized indirect-
        # indexed path should produce the same answer as the
        # unoptimized DPTR path.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t data[8] = "
            "{0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};\n"
            "static volatile uint8_t result;\n"
            "__attribute__((zp_abi))\n"
            "void load(uint8_t *p, uint8_t i) {\n"
            "    result = p[i];\n"
            "}\n"
            "int main(void) { load(data, 5); return 0; }\n"
        )
        sim = build_sim(src, optimize=True)
        out = sim.run(max_cycles=5_000_000)
        self.assertFalse(out.timed_out)
        result_addr = sim.symbols["result"]
        self.assertEqual(out.memory[result_addr], 0x60)

    def test_store_writes_correct_byte(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint8_t dest[8];\n"
            "__attribute__((zp_abi))\n"
            "void store(uint8_t *p, uint8_t i, uint8_t v) {\n"
            "    p[i] = v;\n"
            "}\n"
            "int main(void) { store(dest, 3, 0xAB); return 0; }\n"
        )
        sim = build_sim(src, optimize=True)
        out = sim.run(max_cycles=5_000_000)
        self.assertFalse(out.timed_out)
        dest_addr = sim.symbols["dest"]
        self.assertEqual(out.memory[dest_addr + 3], 0xAB)

    def test_unoptimized_still_works(self) -> None:
        # The pass only fires under --optimize. Verify both modes
        # produce the same result.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t data[8] = "
            "{1, 2, 3, 4, 5, 6, 7, 8};\n"
            "static volatile uint8_t result;\n"
            "__attribute__((zp_abi))\n"
            "void load(uint8_t *p, uint8_t i) {\n"
            "    result = p[i];\n"
            "}\n"
            "int main(void) { load(data, 4); return 0; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                out = sim.run(max_cycles=5_000_000)
                self.assertFalse(out.timed_out)
                result_addr = sim.symbols["result"]
                self.assertEqual(out.memory[result_addr], 5)


if __name__ == "__main__":
    unittest.main()
