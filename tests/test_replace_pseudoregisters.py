"""Tests for `passes.replace_pseudoregisters`.

The tests of the frame layout that called `replace_function`,
`Replacer.replace`, or `replace_program` directly are doctest cases of
the C++ port now, the program-level calls of
cpp/tests/test_frames_stage_calls.cpp and
test_replace_pseudoregisters_calls.cpp (docs/cpp_port.md, step 8,
group G6).
"""
import unittest

import asm_ast
from passes.replace_pseudoregisters import replace_function, replace_program


def _reg_a():
    return asm_ast.Reg(reg=asm_ast.A())


def _fn(*instrs, name="main", params=(), is_global=True):
    return asm_ast.Function(
        name=name,
        is_global=is_global,
        params=list(params),
        instructions=list(instrs),
    )


class TestColoringIntegration(unittest.TestCase):
    """Pseudos whose names appear in `coloring.assignments` lower to
    `ZP(address, offset)` and skip frame allocation. Spilled, never-
    colored, and address-taken locals fall through to the existing
    Frame path. Params always go to Frame regardless of coloring."""

    def _coloring(self, **assignments):
        # Lazy local import to avoid coupling test file load order to
        # passes.optimization being importable when these tests run.
        from passes.optimization.register_allocation import Coloring
        from passes.optimization.pool import Pool
        return Coloring(
            assignments=dict(assignments),
            spilled=set(),
            pool=Pool(),
        )

    def test_colored_local_lowers_to_zp(self):
        out = replace_function(
            _fn(
                asm_ast.Mov(
                    src=asm_ast.Imm(value=1),
                    dst=asm_ast.Pseudo(name="x", offset=0),
                ),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ),
            coloring=self._coloring(x=0x82),
        )
        self.assertEqual(out.instructions[1], asm_ast.Mov(
            src=asm_ast.Imm(value=1),
            dst=asm_ast.ZP(address=0x82, offset=0),
        ))

    def test_colored_local_high_byte(self):
        # A Pseudo with a non-zero offset preserves it in the ZP
        # operand — same offset semantics as Data / Frame.
        out = replace_function(
            _fn(
                asm_ast.Mov(
                    src=asm_ast.Pseudo(name="x", offset=1), dst=_reg_a(),
                ),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ),
            coloring=self._coloring(x=0x82),
        )
        self.assertEqual(out.instructions[1], asm_ast.Mov(
            src=asm_ast.ZP(address=0x82, offset=1), dst=_reg_a(),
        ))

    def test_colored_local_skips_frame_allocation(self):
        # One colored local + one uncolored local. The colored one
        # must NOT contribute to local_bytes; only the uncolored one
        # gets a Frame slot.
        out = replace_function(
            _fn(
                asm_ast.Mov(
                    src=asm_ast.Imm(value=1),
                    dst=asm_ast.Pseudo(name="colored", offset=0),
                ),
                asm_ast.Mov(
                    src=asm_ast.Imm(value=2),
                    dst=asm_ast.Pseudo(name="uncolored", offset=0),
                ),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ),
            coloring=self._coloring(colored=0x82),
        )
        # Prologue carries local_bytes=2 (uncolored is 1 byte by
        # default since no symbol table; the colored one contributes
        # zero).
        self.assertEqual(out.instructions[0], asm_ast.FunctionPrologue(
            arg_bytes=0, local_bytes=1,
        ))
        self.assertEqual(out.instructions[1], asm_ast.Mov(
            src=asm_ast.Imm(value=1),
            dst=asm_ast.ZP(address=0x82, offset=0),
        ))
        self.assertEqual(out.instructions[2], asm_ast.Mov(
            src=asm_ast.Imm(value=2),
            dst=asm_ast.Frame(offset=1),
        ))

    def test_spilled_local_falls_through_to_frame(self):
        # Spilled names are absent from `assignments` (and present in
        # `spilled`); they should reach the Frame path unchanged.
        from passes.optimization.register_allocation import Coloring
        from passes.optimization.pool import Pool
        coloring = Coloring(
            assignments={"x": 0x82},
            spilled={"y"},
            pool=Pool(),
        )
        out = replace_function(
            _fn(
                asm_ast.Mov(
                    src=asm_ast.Imm(value=1),
                    dst=asm_ast.Pseudo(name="x", offset=0),
                ),
                asm_ast.Mov(
                    src=asm_ast.Imm(value=2),
                    dst=asm_ast.Pseudo(name="y", offset=0),
                ),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ),
            coloring=coloring,
        )
        # x → ZP, y → Frame (spilled).
        self.assertEqual(out.instructions[1].dst, asm_ast.ZP(address=0x82, offset=0))
        self.assertEqual(out.instructions[2].dst, asm_ast.Frame(offset=1))

    def test_param_ignores_coloring(self):
        # Even if a param's name is colored, it must lower to Frame
        # (the calling convention dictates Frame offsets on entry).
        out = replace_function(
            _fn(
                asm_ast.Mov(
                    src=asm_ast.Pseudo(name="p", offset=0), dst=_reg_a(),
                ),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
                params=("p",),
            ),
            coloring=self._coloring(p=0x82),
        )
        # M=0 (no locals). Param p → Frame(M+3) = Frame(3).
        self.assertIsInstance(out.instructions[1].src, asm_ast.Frame)
        self.assertEqual(out.instructions[1].src.offset, 3)

    def test_no_coloring_arg_unchanged(self):
        # Regression backstop: replace_function with coloring=None
        # produces identical output to a pre-regalloc call.
        instrs_before = [
            asm_ast.Mov(
                src=asm_ast.Imm(value=1),
                dst=asm_ast.Pseudo(name="x", offset=0),
            ),
            asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
        ]
        without = replace_function(_fn(*instrs_before))
        with_none = replace_function(_fn(*instrs_before), coloring=None)
        self.assertEqual(without, with_none)

    def test_replace_program_threads_per_function_colorings(self):
        # Two functions, each with its own coloring entry in the
        # `colorings` dict. Verify each function picks up the matching
        # entry (or falls back to no-coloring if missing).
        from passes.optimization.register_allocation import Coloring
        from passes.optimization.pool import Pool
        prog = asm_ast.Program(top_level=[
            asm_ast.Function(
                name="foo", is_global=True, params=[],
                instructions=[
                    asm_ast.Mov(
                        src=asm_ast.Imm(value=1),
                        dst=asm_ast.Pseudo(name="x", offset=0),
                    ),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
                ],
            ),
            asm_ast.Function(
                name="bar", is_global=True, params=[],
                instructions=[
                    asm_ast.Mov(
                        src=asm_ast.Imm(value=2),
                        dst=asm_ast.Pseudo(name="y", offset=0),
                    ),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
                ],
            ),
        ])
        colorings = {
            "foo": Coloring(assignments={"x": 0x82}, spilled=set(), pool=Pool()),
            # bar: no entry → falls through to all-Frame.
        }
        out = replace_program(prog, colorings=colorings)
        # foo's x → ZP; bar's y → Frame.
        self.assertEqual(
            out.top_level[0].instructions[1].dst,
            asm_ast.ZP(address=0x82, offset=0),
        )
        self.assertEqual(
            out.top_level[1].instructions[1].dst,
            asm_ast.Frame(offset=1),
        )

    def test_callee_saved_addrs_collected_and_reserved(self):
        # A coloring with a callee-saved assignment ($C0, in the
        # default Pool's callee_saved range): the function's
        # FunctionPrologue and Ret should carry the address in
        # `callee_saved_addrs`, and locals should shift up by S
        # (the save area's size).
        from passes.optimization.register_allocation import Coloring
        from passes.optimization.pool import Pool
        coloring = Coloring(
            assignments={"hot": 0xC0},  # 1 byte in callee-saved
            spilled=set(),
            pool=Pool(),
        )
        out = replace_function(
            _fn(
                asm_ast.Mov(
                    src=asm_ast.Imm(value=1),
                    dst=asm_ast.Pseudo(name="hot", offset=0),
                ),
                # An uncolored local — should land at FP+S+1 = FP+2.
                asm_ast.Mov(
                    src=asm_ast.Imm(value=2),
                    dst=asm_ast.Pseudo(name="cold", offset=0),
                ),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ),
            coloring=coloring,
        )
        prologue = out.instructions[0]
        self.assertIsInstance(prologue, asm_ast.FunctionPrologue)
        self.assertEqual(prologue.callee_saved_addrs, [0xC0])
        # Save area = 1 byte, plus 1-byte uncolored local → local_bytes=2.
        self.assertEqual(prologue.local_bytes, 2)
        # The uncolored local lands at FP+(1+1) = FP+2.
        self.assertEqual(out.instructions[2].dst, asm_ast.Frame(offset=2))
        # Ret carries the same callee-saved-addrs list.
        ret = out.instructions[-1]
        self.assertIsInstance(ret, asm_ast.Ret)
        self.assertEqual(ret.callee_saved_addrs, [0xC0])

    def test_multibyte_callee_saved_enumerates_all_bytes(self):
        # A 4-byte value (Long) at $C0 occupies $C0..$C3, all in
        # callee-saved. All 4 bytes should appear in the prologue's
        # callee_saved_addrs.
        from passes.optimization.register_allocation import Coloring
        from passes.optimization.pool import Pool
        from passes.type_checking import LocalAttr, Symbol, SymbolTable
        import c99_ast
        st = SymbolTable()
        st["wide"] = Symbol(type=c99_ast.Long(), attrs=LocalAttr())
        coloring = Coloring(
            assignments={"wide": 0xC0},
            spilled=set(),
            pool=Pool(),
        )
        out = replace_function(
            _fn(
                asm_ast.Mov(
                    src=asm_ast.Imm(value=1),
                    dst=asm_ast.Pseudo(name="wide", offset=0),
                ),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ),
            symbols=st,
            coloring=coloring,
        )
        prologue = out.instructions[0]
        self.assertEqual(prologue.callee_saved_addrs, [0xC0, 0xC1, 0xC2, 0xC3])
        # Save area = 4 bytes, no other locals.
        self.assertEqual(prologue.local_bytes, 4)


if __name__ == "__main__":
    unittest.main()
