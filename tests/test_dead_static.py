"""Tests for the asm-level dead static elimination pass.

`apply_dead_static_elimination` drops internal-linkage
`StaticVariable` top-levels nothing references.

Coverage:
  * Internal-linkage static with no references is dropped.
  * Internal-linkage static referenced via Pseudo / Data /
    IndexedData / ImmLabelLow / ImmLabelHigh is kept.
  * External-linkage (`is_global=True`) static is kept even
    when unreferenced (linker may resolve another TU's reference).
  * AddressInit chain: a static referenced only by another
    static's init is kept.
  * Functions pass through unchanged.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.optimization_asm.dead_static import (
    apply_dead_static_elimination,
)


_REG_A = asm_ast.Reg(reg=asm_ast.A())


def _fn(
    name: str, instrs: list[asm_ast.Type_instruction],
) -> asm_ast.Function:
    return asm_ast.Function(
        name=name, is_global=True, params=[], instructions=instrs,
    )


def _static(
    name: str, *, is_global: bool = False,
    init: list[asm_ast.Type_static_init] | None = None,
) -> asm_ast.StaticVariable:
    return asm_ast.StaticVariable(
        name=name, is_global=is_global,
        init=init if init is not None else [asm_ast.IntInit(value=0)],
    )


def _names(prog: asm_ast.Program) -> set[str]:
    return {
        tl.name for tl in prog.top_level
        if isinstance(tl, asm_ast.StaticVariable)
    }


class TestDeadStaticElimination(unittest.TestCase):
    def test_drops_unreferenced_internal_static(self) -> None:
        prog = asm_ast.Program(top_level=[
            _fn("main", [asm_ast.Return(save_a=False)]),
            _static("dead", is_global=False),
        ])
        out = apply_dead_static_elimination(prog)
        self.assertEqual(_names(out), set())

    def test_keeps_external_static_even_unreferenced(self) -> None:
        prog = asm_ast.Program(top_level=[
            _fn("main", [asm_ast.Return(save_a=False)]),
            _static("global_x", is_global=True),
        ])
        out = apply_dead_static_elimination(prog)
        self.assertEqual(_names(out), {"global_x"})

    def test_keeps_static_referenced_by_data_operand(self) -> None:
        prog = asm_ast.Program(top_level=[
            _fn("main", [
                asm_ast.Mov(
                    src=asm_ast.Data(name="kept", offset=0), dst=_REG_A,
                ),
                asm_ast.Return(save_a=False),
            ]),
            _static("kept"),
            _static("dead"),
        ])
        out = apply_dead_static_elimination(prog)
        self.assertEqual(_names(out), {"kept"})

    def test_keeps_static_referenced_by_indexed_data(self) -> None:
        prog = asm_ast.Program(top_level=[
            _fn("main", [
                asm_ast.Mov(
                    src=asm_ast.IndexedData(
                        name="kept", offset=0, index=asm_ast.X(),
                    ),
                    dst=_REG_A,
                ),
                asm_ast.Return(save_a=False),
            ]),
            _static("kept"),
        ])
        out = apply_dead_static_elimination(prog)
        self.assertEqual(_names(out), {"kept"})

    def test_keeps_static_referenced_by_pseudo(self) -> None:
        # Before replace_pseudoregisters, statics are referenced
        # via Pseudo. The pass should still see those.
        prog = asm_ast.Program(top_level=[
            _fn("main", [
                asm_ast.Mov(
                    src=asm_ast.Pseudo(name="kept", offset=0),
                    dst=_REG_A,
                ),
                asm_ast.Return(save_a=False),
            ]),
            _static("kept"),
        ])
        out = apply_dead_static_elimination(prog)
        self.assertEqual(_names(out), {"kept"})

    def test_keeps_static_referenced_by_immlabel(self) -> None:
        # LoadAddress lowers via ImmLabelLow / ImmLabelHigh; if
        # these survive into our pass's input, they must count
        # as references.
        prog = asm_ast.Program(top_level=[
            _fn("main", [
                asm_ast.Mov(
                    src=asm_ast.ImmLabelLow(name="kept", offset=0),
                    dst=_REG_A,
                ),
                asm_ast.Return(save_a=False),
            ]),
            _static("kept"),
        ])
        out = apply_dead_static_elimination(prog)
        self.assertEqual(_names(out), {"kept"})

    def test_keeps_static_referenced_by_loadaddress(self) -> None:
        prog = asm_ast.Program(top_level=[
            _fn("main", [
                asm_ast.LoadAddress(
                    src=asm_ast.Data(name="kept", offset=0),
                    dst=asm_ast.Pseudo(name="ptr", offset=0),
                ),
                asm_ast.Return(save_a=False),
            ]),
            _static("kept"),
            _static("ptr"),  # also kept (referenced as LA's dst)
        ])
        out = apply_dead_static_elimination(prog)
        self.assertEqual(_names(out), {"kept", "ptr"})

    def test_address_init_chain_keeps_target(self) -> None:
        # `a` is referenced only by `b`'s AddressInit; `b` is
        # referenced by main. Both should be kept.
        prog = asm_ast.Program(top_level=[
            _fn("main", [
                asm_ast.Mov(
                    src=asm_ast.Data(name="b", offset=0), dst=_REG_A,
                ),
                asm_ast.Return(save_a=False),
            ]),
            _static(
                "a",
                init=[asm_ast.IntInit(value=42)],
            ),
            _static(
                "b",
                init=[asm_ast.AddressInit(name="a", offset=0)],
            ),
        ])
        out = apply_dead_static_elimination(prog)
        self.assertEqual(_names(out), {"a", "b"})

    def test_drops_array_static_after_const_fold(self) -> None:
        # Simulates the post-const-array-fold state: the array's
        # bytes have all been replaced by Imms inline, so no
        # function instruction names the array.
        prog = asm_ast.Program(top_level=[
            _fn("main", [
                asm_ast.Mov(src=asm_ast.Imm(value=0xA8), dst=_REG_A),
                asm_ast.Mov(
                    src=_REG_A,
                    dst=asm_ast.IndexedData(
                        name="", offset=0x20A8, index=asm_ast.X(),
                    ),
                ),
                asm_ast.Return(save_a=False),
            ]),
            _static(
                "interlace_p1_offsets",
                init=[asm_ast.LongInit(value=0x00A8)],
            ),
        ])
        out = apply_dead_static_elimination(prog)
        self.assertEqual(_names(out), set())

    def test_functions_pass_through_unchanged(self) -> None:
        f = _fn("g", [asm_ast.Return(save_a=False)])
        prog = asm_ast.Program(top_level=[f])
        out = apply_dead_static_elimination(prog)
        self.assertEqual(out.top_level, [f])


if __name__ == "__main__":
    unittest.main()
