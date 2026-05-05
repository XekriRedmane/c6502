"""Tests for the TAC-level scalar const-static fold +
const-array-subscript fold + Add reassociation.

Three composable optimizations:

  1. `passes/optimization/static_const_fold.py` replaces
     `Var(static_const_scalar)` USE positions with
     `Constant(value)` so downstream constant folding can collapse
     the resulting arithmetic.
  2. `_fold_indexed_load` in `passes/optimization/constant_folding.py`
     folds `IndexedLoad(static_const_array, Constant(byte_idx))` to
     a single Constant (the array element's value) when the index
     is element-aligned and the array's element type is const-
     qualified.
  3. `passes/optimization/reassoc_const.py` collapses
     `Constant(C1) + (Constant(C2) + V)` into `Constant(C1+C2) + V`
     so two nested 16-bit Adds become one.

Together they turn `hires_page1[interlace_p1_offsets[2] + col]`
(when both `hires_page1` and `interlace_p1_offsets` are
const-qualified statics) into a single 16-bit `Add(0x21D0, col)`
at runtime — half the address-arithmetic work, plus all the
intermediate temp slots freed up.
"""
from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.constant_folding import constant_fold
from passes.optimization.reassoc_const import reassoc_constants
from passes.optimization.static_const_fold import fold_static_const_reads
from passes.type_checking import (
    Initial, LocalAttr, NoInitializer, StaticAttr, Symbol, SymbolTable,
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
    """Build a symbol table populated from `entries`. The class
    populates by `__setitem__`, which exposes a public API."""
    tbl = SymbolTable()
    for name, sym in entries.items():
        tbl[name] = sym
    return tbl


class TestStaticConstReadsFoldUnit(unittest.TestCase):
    """Direct calls to `fold_static_const_reads` on synthetic TAC."""

    def test_const_int_static_var_folds_to_constant(self) -> None:
        # `Var(magic)` where `magic` is `static const int = 0x1234`
        # → `Constant(ConstInt(0x1234))` in USE positions.
        sym = Symbol(
            type=c99_ast.Const(referenced_type=c99_ast.Int()),
            attrs=StaticAttr(
                initial_value=Initial(value=0x1234), is_global=False,
            ),
        )
        symbols = _table({"magic": sym})
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=_var("magic"), src2=_var("y"),
                dst=_var("%t"),
            ),
        ]
        out = fold_static_const_reads(_fn(instrs), symbols)
        self.assertEqual(
            out.instructions[0].src1,
            tac_ast.Constant(const=tac_ast.ConstInt(value=0x1234)),
        )
        # `y` (no symbol entry) stays as Var.
        self.assertEqual(out.instructions[0].src2, _var("y"))

    def test_const_pointer_static_folds_via_uint(self) -> None:
        # `static T * const p = (T*)0x2000;` — the symbol's type
        # is `Const(Pointer(T))`, the initial value is the integer
        # 8192. Pointer maps to ConstUInt at TAC.
        sym = Symbol(
            type=c99_ast.Const(
                referenced_type=c99_ast.Pointer(
                    referenced_type=c99_ast.UChar(),
                ),
            ),
            attrs=StaticAttr(
                initial_value=Initial(value=0x2000), is_global=False,
            ),
        )
        symbols = _table({"p": sym})
        instrs = [
            tac_ast.Copy(src=_var("p"), dst=_var("%dst")),
        ]
        out = fold_static_const_reads(_fn(instrs), symbols)
        self.assertEqual(
            out.instructions[0].src,
            tac_ast.Constant(const=tac_ast.ConstUInt(value=0x2000)),
        )

    def test_non_const_static_does_not_fold(self) -> None:
        # No `Const(...)` wrapper — runtime modification is
        # legal even if no actual write exists. Don't fold.
        sym = Symbol(
            type=c99_ast.Int(),
            attrs=StaticAttr(
                initial_value=Initial(value=42), is_global=False,
            ),
        )
        symbols = _table({"x": sym})
        instrs = [
            tac_ast.Copy(src=_var("x"), dst=_var("%t")),
        ]
        out = fold_static_const_reads(_fn(instrs), symbols)
        self.assertEqual(out.instructions[0].src, _var("x"))

    def test_local_attr_does_not_fold(self) -> None:
        # Automatic-storage Var, even with `Const(...)` wrapper —
        # the pass only fires for static-storage objects.
        sym = Symbol(
            type=c99_ast.Const(referenced_type=c99_ast.Int()),
            attrs=LocalAttr(),
        )
        symbols = _table({"x": sym})
        instrs = [
            tac_ast.Copy(src=_var("x"), dst=_var("%t")),
        ]
        out = fold_static_const_reads(_fn(instrs), symbols)
        self.assertEqual(out.instructions[0].src, _var("x"))

    def test_no_initializer_does_not_fold(self) -> None:
        # `extern const int x;` — declared but not defined here.
        # The actual value is link-time, not foldable.
        sym = Symbol(
            type=c99_ast.Const(referenced_type=c99_ast.Int()),
            attrs=StaticAttr(
                initial_value=NoInitializer(), is_global=True,
            ),
        )
        symbols = _table({"x": sym})
        instrs = [
            tac_ast.Copy(src=_var("x"), dst=_var("%t")),
        ]
        out = fold_static_const_reads(_fn(instrs), symbols)
        self.assertEqual(out.instructions[0].src, _var("x"))

    def test_aggregate_initial_value_does_not_fold(self) -> None:
        # `static const int arr[3] = {1,2,3};` — initial value is a
        # tuple, not a scalar. Array-typed; the fold's scalar-only
        # gate skips it (the const-array-subscript fold handles
        # this case, not this pass).
        sym = Symbol(
            type=c99_ast.Array(
                element_type=c99_ast.Const(referenced_type=c99_ast.Int()),
                size=3,
            ),
            attrs=StaticAttr(
                initial_value=Initial(value=(1, 2, 3)), is_global=False,
            ),
        )
        symbols = _table({"arr": sym})
        instrs = [
            tac_ast.Copy(src=_var("arr"), dst=_var("%t")),
        ]
        out = fold_static_const_reads(_fn(instrs), symbols)
        self.assertEqual(out.instructions[0].src, _var("arr"))

    def test_def_position_var_not_substituted(self) -> None:
        # `Var(name)` in a DEF position (Copy.dst) is the storage
        # being written, not the value being read. Don't substitute.
        sym = Symbol(
            type=c99_ast.Const(referenced_type=c99_ast.Int()),
            attrs=StaticAttr(
                initial_value=Initial(value=5), is_global=False,
            ),
        )
        symbols = _table({"x": sym})
        instrs = [
            tac_ast.Copy(src=_var("y"), dst=_var("x")),
        ]
        out = fold_static_const_reads(_fn(instrs), symbols)
        # dst stays as Var(x) — it's the destination, not a value.
        self.assertEqual(out.instructions[0].dst, _var("x"))

    def test_indexed_load_name_field_preserved(self) -> None:
        # `IndexedLoad.name` is the array's symbol name (an
        # identifier string), not a Var-typed value. The fold
        # leaves the name field alone but substitutes the index.
        sym_arr = Symbol(
            type=c99_ast.Array(
                element_type=c99_ast.UChar(), size=3,
            ),
            attrs=StaticAttr(
                initial_value=Initial(value=(1, 2, 3)), is_global=False,
            ),
        )
        sym_idx = Symbol(
            type=c99_ast.Const(referenced_type=c99_ast.UChar()),
            attrs=StaticAttr(
                initial_value=Initial(value=2), is_global=False,
            ),
        )
        symbols = _table({"arr": sym_arr, "idx": sym_idx})
        instrs = [
            tac_ast.IndexedLoad(
                name="arr", index=_var("idx"), dst=_var("%t"),
            ),
        ]
        out = fold_static_const_reads(_fn(instrs), symbols)
        self.assertEqual(out.instructions[0].name, "arr")
        # The index var gets folded to Constant.
        self.assertEqual(
            out.instructions[0].index,
            tac_ast.Constant(const=tac_ast.ConstUChar(value=2)),
        )


class TestConstArraySubscriptFoldUnit(unittest.TestCase):
    """Direct calls to `constant_fold` exercising the
    `_fold_indexed_load` extension. The fold dispatches via
    `constant_fold(fn, symbols=...)`."""

    def _make_const_uint16_array_symbol(
        self, values: tuple[int, ...],
    ) -> Symbol:
        return Symbol(
            type=c99_ast.Array(
                element_type=c99_ast.Const(
                    referenced_type=c99_ast.UInt(),
                ),
                size=len(values),
            ),
            attrs=StaticAttr(
                initial_value=Initial(value=values), is_global=False,
            ),
        )

    def test_indexed_load_aligned_constant_folds(self) -> None:
        # arr[2] of uint16 — byte_idx = 4. Element value is
        # values[2]. Returns Copy(Constant, dst).
        sym_arr = self._make_const_uint16_array_symbol(
            (0x100, 0x200, 0x300),
        )
        sym_dst = Symbol(
            type=c99_ast.UInt(), attrs=LocalAttr(),
        )
        symbols = _table({"arr": sym_arr, "%t": sym_dst})
        instrs = [
            tac_ast.IndexedLoad(
                name="arr",
                index=tac_ast.Constant(
                    const=tac_ast.ConstUChar(value=4),
                ),
                dst=_var("%t"),
            ),
        ]
        out = constant_fold(_fn(instrs), symbols=symbols)
        self.assertIsInstance(out.instructions[0], tac_ast.Copy)
        self.assertEqual(
            out.instructions[0].src,
            tac_ast.Constant(const=tac_ast.ConstUInt(value=0x300)),
        )

    def test_indexed_load_with_runtime_index_does_not_fold(self) -> None:
        # Non-Constant index — can't fold.
        sym_arr = self._make_const_uint16_array_symbol(
            (0x100, 0x200, 0x300),
        )
        sym_dst = Symbol(
            type=c99_ast.UInt(), attrs=LocalAttr(),
        )
        sym_i = Symbol(type=c99_ast.UChar(), attrs=LocalAttr())
        symbols = _table({
            "arr": sym_arr, "%t": sym_dst, "%i": sym_i,
        })
        instrs = [
            tac_ast.IndexedLoad(
                name="arr", index=_var("%i"), dst=_var("%t"),
            ),
        ]
        out = constant_fold(_fn(instrs), symbols=symbols)
        # No fold — IndexedLoad survives.
        self.assertIsInstance(
            out.instructions[0], tac_ast.IndexedLoad,
        )

    def test_non_const_array_element_type_does_not_fold(self) -> None:
        # `static uint16_t arr[]` (no const on element) — the
        # array could legally be modified at runtime, so a fold
        # based on the static-init values would be unsound.
        sym_arr = Symbol(
            type=c99_ast.Array(
                element_type=c99_ast.UInt(), size=3,
            ),
            attrs=StaticAttr(
                initial_value=Initial(value=(0x100, 0x200, 0x300)),
                is_global=False,
            ),
        )
        sym_dst = Symbol(type=c99_ast.UInt(), attrs=LocalAttr())
        symbols = _table({"arr": sym_arr, "%t": sym_dst})
        instrs = [
            tac_ast.IndexedLoad(
                name="arr",
                index=tac_ast.Constant(
                    const=tac_ast.ConstUChar(value=4),
                ),
                dst=_var("%t"),
            ),
        ]
        out = constant_fold(_fn(instrs), symbols=symbols)
        self.assertIsInstance(
            out.instructions[0], tac_ast.IndexedLoad,
        )

    def test_unaligned_byte_index_does_not_fold(self) -> None:
        # byte_idx = 1 isn't aligned to a uint16 (2-byte) element.
        # Folding would need byte slicing — not implemented.
        sym_arr = self._make_const_uint16_array_symbol(
            (0x1234, 0x5678),
        )
        sym_dst = Symbol(type=c99_ast.UInt(), attrs=LocalAttr())
        symbols = _table({"arr": sym_arr, "%t": sym_dst})
        instrs = [
            tac_ast.IndexedLoad(
                name="arr",
                index=tac_ast.Constant(
                    const=tac_ast.ConstUChar(value=1),
                ),
                dst=_var("%t"),
            ),
        ]
        out = constant_fold(_fn(instrs), symbols=symbols)
        self.assertIsInstance(
            out.instructions[0], tac_ast.IndexedLoad,
        )

    def test_out_of_bounds_index_does_not_fold(self) -> None:
        sym_arr = self._make_const_uint16_array_symbol(
            (0x100, 0x200),
        )
        sym_dst = Symbol(type=c99_ast.UInt(), attrs=LocalAttr())
        symbols = _table({"arr": sym_arr, "%t": sym_dst})
        instrs = [
            tac_ast.IndexedLoad(
                name="arr",
                # byte_idx = 4 → element 2, but array has only 2
                # elements (indices 0 and 1).
                index=tac_ast.Constant(
                    const=tac_ast.ConstUChar(value=4),
                ),
                dst=_var("%t"),
            ),
        ]
        out = constant_fold(_fn(instrs), symbols=symbols)
        self.assertIsInstance(
            out.instructions[0], tac_ast.IndexedLoad,
        )


class TestReassocConstantsUnit(unittest.TestCase):
    """Direct calls to `reassoc_constants` on synthetic TAC."""

    def _const_uint(self, v: int) -> tac_ast.Constant:
        return tac_ast.Constant(const=tac_ast.ConstUInt(value=v))

    def test_combines_two_constants(self) -> None:
        # `(0x100 + V) + 0x200` → `0x300 + V`.
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=self._const_uint(0x100), src2=_var("v"),
                dst=_var("%inner"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=self._const_uint(0x200), src2=_var("%inner"),
                dst=_var("%outer"),
            ),
        ]
        out = reassoc_constants(_fn(instrs))
        # Inner is dropped; outer becomes (0x300 + v).
        self.assertEqual(len(out.instructions), 1)
        self.assertEqual(out.instructions[0].dst, _var("%outer"))
        self.assertEqual(
            out.instructions[0].src1,
            self._const_uint(0x300),
        )
        self.assertEqual(out.instructions[0].src2, _var("v"))

    def test_commutative_arrangement_combines(self) -> None:
        # `(V + 0x100) + 0x200` (Constant on the RHS of inner) →
        # `0x300 + V`.
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=_var("v"), src2=self._const_uint(0x100),
                dst=_var("%inner"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=_var("%inner"), src2=self._const_uint(0x200),
                dst=_var("%outer"),
            ),
        ]
        out = reassoc_constants(_fn(instrs))
        self.assertEqual(len(out.instructions), 1)
        # The combined constant is at src1 (the rewriter's choice);
        # the other operand is the inner's non-const.
        self.assertEqual(
            out.instructions[0].src1,
            self._const_uint(0x300),
        )
        self.assertEqual(out.instructions[0].src2, _var("v"))

    def test_multi_use_inner_does_not_fuse(self) -> None:
        # If `%inner` has another use (besides the outer Add),
        # reassoc would replicate work — bail.
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=self._const_uint(0x100), src2=_var("v"),
                dst=_var("%inner"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=self._const_uint(0x200), src2=_var("%inner"),
                dst=_var("%outer"),
            ),
            # Another use of %inner → blocks fusion.
            tac_ast.Copy(src=_var("%inner"), dst=_var("%other")),
        ]
        out = reassoc_constants(_fn(instrs))
        self.assertEqual(out.instructions, instrs)

    def test_mixed_signedness_same_width_combines(self) -> None:
        # ConstInt(100) + (ConstUInt(0x2000) + col_ext) appears in
        # pointer arithmetic where `100 + col` runs at int width
        # and `buf + ...` at pointer (uint) width. Both are
        # 16-bit; reassoc combines them and uses the outer's
        # variant for the result.
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Constant(
                    const=tac_ast.ConstInt(value=100),
                ),
                src2=_var("v"),
                dst=_var("%inner"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=self._const_uint(0x2000), src2=_var("%inner"),
                dst=_var("%outer"),
            ),
        ]
        out = reassoc_constants(_fn(instrs))
        self.assertEqual(len(out.instructions), 1)
        self.assertEqual(
            out.instructions[0].src1,
            self._const_uint(0x2000 + 100),
        )

    def test_different_widths_do_not_combine(self) -> None:
        # ConstUChar (8-bit) + ConstUInt (16-bit) — different
        # widths, the rewrite has no canonical choice for the
        # combined width. Bail.
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Constant(
                    const=tac_ast.ConstUChar(value=5),
                ),
                src2=_var("v"),
                dst=_var("%inner"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=self._const_uint(0x100), src2=_var("%inner"),
                dst=_var("%outer"),
            ),
        ]
        out = reassoc_constants(_fn(instrs))
        self.assertEqual(out.instructions, instrs)

    def test_inner_not_an_add_does_not_fuse(self) -> None:
        # Inner def is `Copy`, not `Binary(Add)` — pattern doesn't
        # match.
        instrs = [
            tac_ast.Copy(src=_var("v"), dst=_var("%inner")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=self._const_uint(0x100), src2=_var("%inner"),
                dst=_var("%outer"),
            ),
        ]
        out = reassoc_constants(_fn(instrs))
        self.assertEqual(out.instructions, instrs)

    def test_wrap_at_uint_width(self) -> None:
        # Combined value wraps modulo 2^16 for ConstUInt operands.
        # 0xFFFF + 0x0002 = 0x10001 → wraps to 0x0001.
        instrs = [
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=self._const_uint(0xFFFF), src2=_var("v"),
                dst=_var("%inner"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=self._const_uint(0x0002), src2=_var("%inner"),
                dst=_var("%outer"),
            ),
        ]
        out = reassoc_constants(_fn(instrs))
        self.assertEqual(
            out.instructions[0].src1,
            self._const_uint(0x0001),
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestStaticConstFoldAsmShape(unittest.TestCase):
    """Source-level checks on the emitted asm — confirm constants
    appear as immediates."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_scalar_const_static_collapses_to_immediate(self) -> None:
        # `static const int magic = 0x1234; magic + 1` collapses
        # to immediate 0x1235 — the static is gone, no LDA from
        # storage, no runtime add.
        src = (
            "static const int magic = 0x1234;\n"
            "int main(void) { return magic + 1; }\n"
        )
        asm = self._compile(src)
        # The static's storage is dropped (asm-level fold) and the
        # add folded at TAC level.
        self.assertNotIn("magic:", asm)
        # Result lands in HARGS as 0x1235 = 0x35, 0x12 (little-
        # endian).
        self.assertIn("#$35", asm)
        self.assertIn("#$12", asm)

    def test_const_array_subscript_with_constant_index_folds(self) -> None:
        # `arr[2]` where arr is `static const uint16_t arr[]` and
        # the array element is itself const → fold to immediate.
        # Verifies the IndexedLoad fast path matches at TAC level
        # and the asm doesn't contain a runtime `LDA arr,X` access.
        src = (
            "#include <stdint.h>\n"
            "static const uint16_t arr[3] = {0x1234, 0x5678, 0x9ABC};\n"
            "int main(void) { return arr[2]; }\n"
        )
        asm = self._compile(src)
        # The asm should NOT have an indexed load on the array
        # (the access is fully folded to the constant).
        self.assertNotIn("LDA   arr,X", asm)
        # 0x9ABC = 0xBC, 0x9A immediates.
        self.assertIn("#$BC", asm)
        self.assertIn("#$9A", asm)

    def test_add_reassoc_combines_constants(self) -> None:
        # `static const int a = 100; static const int b = 200;
        # a + b + col` should fold the two constants to 300, then
        # add `col` (runtime). The reassociation pass merges the
        # two constant Adds into one.
        src = (
            "static const int a = 100;\n"
            "static const int b = 200;\n"
            "int main(int col) { return a + b + col; }\n"
        )
        asm = self._compile(src)
        # 300 = 0x012C; expect a single addition with immediates
        # $2C (low) and $01 (high). The original two adds (a+col,
        # then +b) reassociate into one (a+b)+col = 300+col.
        self.assertIn("#$2C", asm)
        # We DON'T strictly assert the absence of multiple adds
        # because the regalloc / SSA structure can vary, but check
        # that 100 / 200 don't appear as separate immediates
        # (they got combined).
        # 100 = 0x64; 200 = 0xC8. After reassoc both are gone.
        self.assertNotIn("#$64", asm)
        self.assertNotIn("#$C8", asm)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestStaticConstFoldCorrectness(unittest.TestCase):
    """End-to-end: optimized programs compute the same answers as
    unoptimized ones."""

    def test_static_const_int_value_returned(self) -> None:
        src = (
            "static const int magic = 0x1234;\n"
            "int main(void) { return magic + 1; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int() & 0xFFFF, 0x1235)

    def test_const_array_subscript_value_returned(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static const uint16_t arr[3] = {0x1234, 0x5678, 0x9ABC};\n"
            "int main(void) { return arr[2]; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int() & 0xFFFF, 0x9ABC)

    def test_pointer_const_static_indexed_write(self) -> None:
        # The headline case: a `static T * const` initialized to a
        # raw address, indexed by a const + runtime-1-byte sum.
        # Verifies the byte lands at the right memory address.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
            "static const uint16_t offsets[3] = {0x100, 0x200, 0x300};\n"
            "int main(void) { buf[offsets[1] + 5] = 0x42; return 0; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        # Target address: 0x4000 + 0x200 + 5 = 0x4205.
        self.assertEqual(result.memory[0x4205], 0x42)

    def test_reassoc_runtime_correctness(self) -> None:
        # Verify that the reassoc rewrite preserves runtime
        # behavior — the answer must match what the unoptimized
        # arithmetic would compute.
        src = (
            "static const int a = 100;\n"
            "static const int b = 200;\n"
            "int main(int col) { return a + b + col; }\n"
        )
        sim = build_sim(src, optimize=True)
        # main is called with no args; the synthesizer puts a
        # default 0 in col, so result = 300.
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int() & 0xFFFF, 300)


if __name__ == "__main__":
    unittest.main()
