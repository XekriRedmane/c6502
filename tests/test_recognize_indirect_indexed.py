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

import unittest

import c99_ast
import tac_ast
from passes.type_checking import (
    LocalAttr, Symbol, SymbolTable,
)
from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


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


def _ptr_local() -> Symbol:
    return Symbol(
        type=c99_ast.Pointer(referenced_type=c99_ast.UChar()),
        attrs=LocalAttr(),
    )


def _canonical_symbols() -> SymbolTable:
    return _table({
        "y": _uchar_local(),
        "b": _uchar_local(),
        "value": _uchar_local(),
        "p": _ptr_local(),
        "%ext": _uint_local(),
        "%addr": _ptr_local(),
    })


class TestRecognizeIndirectIndexedConstOffset(unittest.TestCase):
    """`Load(ptr + k)` / `Store(v, ptr + k)` with a constant byte
    offset `0 <= k <= 255` folds to an indirect-indexed node whose
    index is `ConstUChar(k)`, lowered as `LDY #k`."""

    def _load(self, k: int) -> list[tac_ast.Type_instruction]:
        return [
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("p"),
                src2=tac_ast.Constant(const=tac_ast.ConstInt(value=k)),
                dst=_var("%addr"),
            ),
            tac_ast.Load(
                src_ptr=_var("%addr"), dst=_var("b"), is_volatile=False,
            ),
        ]


    def _increment(self, ptr: str, between=()) -> list[tac_ast.Type_instruction]:
        """`ptr[1]++`: one address read by a Load and a Store, with
        `between` after the Load."""
        return [
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var(ptr),
                src2=tac_ast.Constant(const=tac_ast.ConstInt(value=1)),
                dst=_var("%addr"),
            ),
            tac_ast.Load(src_ptr=_var("%addr"), dst=_var("b"),
                         is_volatile=False),
            *between,
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("b"),
                src2=tac_ast.Constant(const=tac_ast.ConstUChar(value=1)),
                dst=_var("value"),
            ),
            tac_ast.Store(src=_var("value"), dst_ptr=_var("%addr"),
                          is_volatile=False),
        ]

    def _symbols_with_global(self) -> SymbolTable:
        from passes.type_checking import NoInitializer, StaticAttr
        table = _canonical_symbols()
        table["gp"] = Symbol(
            type=c99_ast.Pointer(referenced_type=c99_ast.UChar()),
            attrs=StaticAttr(initial_value=NoInitializer(), is_global=True),
        )
        return table

    def _accesses(self, instrs):
        return [i for i in instrs if isinstance(
            i, (tac_ast.Load, tac_ast.Store, tac_ast.IndirectIndexedLoad,
                tac_ast.IndirectIndexedStore),
        )]


    def test_offset_read_and_write_end_to_end(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint8_t data[4] = {1, 2, 3, 4};\n"
            "uint8_t *gp;\n"
            "void f(uint8_t *p) { p[1]++; p[2] += 3; gp[3]--; }\n"
            "int main(void) {\n"
            "    gp = data;\n"
            "    f(data);\n"
            "    return data[0] + 10 * data[1] + 100 * data[2]\n"
            "        + 1000 * data[3];\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(src, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 1 + 30 + 600 + 3000)

    def test_offset_access_end_to_end(self) -> None:
        # Loads at offsets 0 and 255, and a store of a negative
        # `signed char` constant, which must be stored as its 2's
        # complement byte.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t data[256];\n"
            "static volatile uint8_t a, b;\n"
            "void f(uint8_t *p, int8_t *q) {\n"
            "    a = p[0];\n"
            "    b = p[255];\n"
            "    q[7] = -122;\n"
            "}\n"
            "int main(void) {\n"
            "    data[0] = 0x11; data[255] = 0x99;\n"
            "    f(data, (int8_t *)data);\n"
            "    return 0;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                out = sim.run(max_cycles=5_000_000)
                self.assertFalse(out.timed_out)
                self.assertEqual(out.memory[sim.symbols["a"]], 0x11)
                self.assertEqual(out.memory[sim.symbols["b"]], 0x99)
                self.assertEqual(
                    out.memory[sim.symbols["data"] + 7], 0x100 - 122,
                )


    def test_wide_member_end_to_end(self) -> None:
        # A 2-byte member read, written, and updated in place through a
        # pointer, and through a pointer the function computes, which
        # stays in DPTR for the last byte of the store while the reads
        # go through the computed pointer's own bytes. asm_remat had
        # taken IndirectZp's offset as part of the pointer's address,
        # and dropped the store of that pointer's high byte.
        src = (
            "#include <stdint.h>\n"
            "struct rec { uint8_t a[13]; uint16_t gold; uint8_t b; };\n"
            "static struct rec r[2];\n"
            "struct rec *P;\n"
            "static uint8_t less(uint16_t price) { return P->gold < price; }\n"
            "static void clear(void) { P->gold = 0; }\n"
            "static void add(struct rec *q) { q->gold += 5; }\n"
            "static int bump(int *ptr) {\n"
            "    if ((--ptr)[3] += 4) return 4;\n"
            "    return 0;\n"
            "}\n"
            "int main(void) {\n"
            "    static int arr[4] = {-1, -2, -3, -4};\n"
            "    P = &r[1];\n"
            "    P->gold = 1000;\n"
            "    if (!less(1001) || less(1000)) return 1;\n"
            "    add(P);\n"
            "    if (r[1].gold != 1005) return 2;\n"
            "    clear();\n"
            "    if (r[1].gold != 0 || r[1].b != 0) return 3;\n"
            "    if (bump(arr + 1) != 0 || arr[3] != 0) return 4;\n"
            "    return 0;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(src, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 0)


@needs_c6502cpp
class TestByteIndexSum(unittest.TestCase):
    """`p[i + k]` with a byte `i` whose largest value plus `k` fits in
    a byte is `(p),Y` with Y = `i + k`, so a word table read through a
    pointer at an even index, `p[i]` and `p[i + 1]` with `i = j << 1`,
    is `LDY i; LDA (p),Y; INY; LDA (p),Y`."""

    _SRC = (
        "#include <stdint.h>\n"
        "static uint8_t data[300];\n"
        "uint8_t *table;\n"
        "uint16_t word(uint8_t j) {\n"
        "    uint8_t i = j << 1;\n"
        "    return ((uint16_t)table[i] << 8) | table[i + 1];\n"
        "}\n"
        "uint16_t odd(uint8_t i) {\n"
        "    return ((uint16_t)table[i] << 8) | table[i + 1];\n"
        "}\n"
        "int main(void) {\n"
        "    table = data;\n"
        "    data[254] = 0x12; data[255] = 0x34; data[256] = 0x56;\n"
        "    return (word(127) == 0x1234) + 2 * (odd(255) == 0x3456);\n"
        "}\n"
    )

    def test_even_index_reads_with_iny(self) -> None:
        from compile import _run_source
        asm = _run_source("codegen", self._SRC, optimize=True)
        word = asm[asm.index("\nword:\n"):asm.index("\nodd:\n")]
        self.assertIn("INY", word)
        self.assertNotIn("ADC", word)

    def test_runs(self) -> None:
        # `odd(255)` reads `table + 256`, past the byte Y can hold, so
        # it keeps its 16-bit add.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 3)


class TestSharedByteIndex(unittest.TestCase):
    """`p + ZeroExtend(i)` read by several 1-byte loads and stores, as
    in `p[i] &= m`, is `(p),Y` with Y = `i` in each of them, and a
    reader `(p + i) + k` with `k` outside 0-255 is `(t),Y` with `t =
    p + k`."""

    def _instrs(self, *, offset=None):
        # `y` has a definition, so that the standalone entry point,
        # which takes every dst for an SSA name, takes `y` for one.
        out = [
            tac_ast.Copy(src=_var("value"), dst=_var("y")),
            tac_ast.ZeroExtend(src=_var("y"), dst=_var("%ext")),
            tac_ast.Binary(op=tac_ast.Add(), src1=_var("p"),
                           src2=_var("%ext"), dst=_var("%addr")),
            tac_ast.Load(src_ptr=_var("%addr"), dst=_var("b")),
        ]
        if offset is not None:
            out += [
                tac_ast.Binary(
                    op=tac_ast.Add(), src1=_var("%addr"),
                    src2=tac_ast.Constant(
                        const=tac_ast.ConstInt(value=offset)),
                    dst=_var("%far"),
                ),
                tac_ast.Load(src_ptr=_var("%far"), dst=_var("c")),
            ]
        out.append(tac_ast.Store(src=_var("value"), dst_ptr=_var("%addr")))
        return out

    def _symbols(self):
        table = _canonical_symbols()
        table["c"] = _uchar_local()
        table["%far"] = _ptr_local()
        return table


    _SRC = (
        "#include <stdint.h>\n"
        "static uint8_t screen[0x2100];\n"
        "static const uint8_t mask[2] = {0xF0, 0x0F};\n"
        "uint8_t *row;\n"
        "void close(uint8_t *line, uint8_t x, uint8_t m) {\n"
        "    line[x] &= mask[m];\n"
        "}\n"
        "void open(uint8_t *line, uint8_t x, uint8_t m) {\n"
        "    const uint8_t *background = line + 0x2000;\n"
        "    line[x] |= background[x] & mask[m];\n"
        "}\n"
        "void open_global(uint8_t x) {\n"
        "    uint8_t v = row[x];\n"
        "    row = screen;\n"
        "    row[x] = v | 1;\n"
        "}\n"
        "int main(void) {\n"
        "    for (uint16_t k = 0; k < 0x100; k++) {\n"
        "        screen[k] = (uint8_t)k;\n"
        "        screen[k + 0x2000] = 0xFF;\n"
        "    }\n"
        "    close(screen + 0x10, 0xF5, 0);\n"
        "    open(screen + 0x10, 0x05, 1);\n"
        "    row = screen + 0x80;\n"
        "    open_global(0x20);\n"
        "    return screen[0x105] + screen[0x15] * 256 + screen[0x20];\n"
        "}\n"
    )

    def test_shapes(self) -> None:
        from compile import _run_source
        asm = _run_source("codegen", self._SRC, optimize=True)
        close = asm[asm.index("\nclose:\n"):asm.index("\nopen:\n")]
        self.assertNotIn("ADC", close)
        opened = asm[asm.index("\nopen:\n"):asm.index("\nopen_global:\n")]
        # One add, of 0x20 to the pointer's high byte.
        self.assertEqual(opened.count("ADC"), 1, opened)

    def test_runs(self) -> None:
        # close: 0x05 & 0xF0 = 0x00 at 0x105. open: 0x15 | (0xFF &
        # 0x0F) = 0x1F at 0x15. open_global reads row[0x20] through
        # the old row, 0xA0, and writes it with bit 0 set through the
        # new one, at 0x20.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run(
                    max_cycles=20_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 0x1F00 + 0xA1)


@needs_c6502cpp
class TestIndirectIndexedAsmShape(unittest.TestCase):
    """Source-level checks on the emitted asm."""

    def _compile(self, src: str) -> str:
        from compile import _run_source
        return _run_source("codegen", src, optimize=True)

    def test_indirect_indexed_load_emits_short_sequence(self) -> None:
        # `ptr[idx]` where ptr is a zp_abi parameter and idx is a
        # uchar local. The indirect-Y access should appear without
        # a 16-bit Add chain, and DPTR staging is bypassed by using
        # the pointer's ZP bytes as the indirect base directly. The
        # pointer arrives in A and X, and `copy` stores it into a
        # pair of its own zero-page bytes at entry.
        src = (
            "#include <stdint.h>\n"
            "static volatile uint8_t result;\n"
            "[[c6502::zp_abi]]\n"
            "void copy(uint8_t *src, uint8_t idx) {\n"
            "    result = src[idx];\n"
            "}\n"
            "int main(void) { return 0; }\n"
        )
        asm = self._compile(src)
        # Expect an `LDA (symbol),Y` indirect-Y read (the ZP base
        # is the local that holds the pointer, rendered by emit
        # using the local's symbolic name).
        self.assertIn("LDA   (__local_copy__src),Y", asm)
        # The unrecognized form would emit `CLC` for the 16-bit
        # Add inside `copy` — verify that's gone.
        body_start = asm.index("copy:")
        body_end = asm.index("\n\n", body_start)
        copy_body = asm[body_start:body_end]
        self.assertNotIn("CLC", copy_body)
        # And the DPTR staging itself has been bypassed.
        self.assertNotIn("STA   DPTR", copy_body)

    def test_global_pointer_stored_constant_on_another_path(self) -> None:
        # `P = 0` on one path made the recognizer take P for the
        # constant 0 everywhere, since the def index gives a global's
        # last store, and `P->look` was read through a computed copy
        # of P. A store doesn't make a global constant.
        src = (
            "#include <stdint.h>\n"
            "struct mob { uint8_t a, b, c, d, look; };\n"
            "extern struct mob *P [[c6502::zeropage(0xF4)]];\n"
            "void next(void);\n"
            "void walk(uint8_t n) {\n"
            "    if (n == 0) { P = 0; return; }\n"
            "    while (P->look >= 0x15) next();\n"
            "}\n"
        )
        asm = self._compile(src)
        body = asm[asm.index("walk:"):]
        self.assertIn("LDY   #$04", body)
        self.assertIn("LDA   (P),Y", body)
        self.assertNotIn("ADC", body)


@needs_c6502cpp
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
            "[[c6502::zp_abi]]\n"
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
            "[[c6502::zp_abi]]\n"
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
        # The pass only fires under -O. Verify both modes
        # produce the same result.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t data[8] = "
            "{1, 2, 3, 4, 5, 6, 7, 8};\n"
            "static volatile uint8_t result;\n"
            "[[c6502::zp_abi]]\n"
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

    def test_negative_signed_char_index(self) -> None:
        # `p[i]` with `int8_t i = -1` addresses p - 1. The fold
        # accepted the SignExtend of i, and `(p),Y` read p + 255.
        src = (
            "#include <stdint.h>\n"
            "uint8_t data[8] = {10, 11, 12, 13, 14, 15, 16, 17};\n"
            "uint8_t ld(const uint8_t *p, int8_t i) { return p[i]; }\n"
            "void st(uint8_t *p, int8_t i, uint8_t v) { p[i] = v; }\n"
            "int main(void) {\n"
            "    uint8_t r = ld(data + 4, -1);\n"
            "    st(data + 4, -2, 99);\n"
            "    return r + (data[2] == 99) * 100;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(src, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertEqual(out.return_int(), 113)


if __name__ == "__main__":
    unittest.main()
