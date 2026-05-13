"""Behavioral tests for ZP-ABI param resolution in
`replace_pseudoregisters`.

Coverage:
  - Default (no ParamLayout): params resolve to Frame.
  - ZpLayout function: params resolve to `Data(slot_symbol, 0)`
    (which dasm resolves to zero-page addressing when the
    symbol's value is in `$00..$FF`, set by
    `passes.zp_slot_allocation`); FrameDims.arg_bytes is 0.
  - Multi-byte param under ZpLayout: each byte resolves to its
    own slot symbol.
  - Non-param locals still get Frame slots in a ZpLayout function.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.abi_selection import SoftStackLayout, ZpLayout
from passes.replace_pseudoregisters import (
    replace_function_bare_exit,
    replace_program_bare_exit,
)


def _ps(name: str, off: int = 0) -> asm_ast.Pseudo:
    return asm_ast.Pseudo(name=name, offset=off)


def _A() -> asm_ast.Reg:
    return asm_ast.Reg(reg=asm_ast.A())


def _imm(v: int) -> asm_ast.Imm:
    return asm_ast.Imm(value=v)


def _mov(src, dst) -> asm_ast.Mov:
    return asm_ast.Mov(src=src, dst=dst)


def _ret_bare(save_a: bool = True) -> asm_ast.Return:
    return asm_ast.Return(save_a=save_a)


def _fn(name: str, *instrs, params=()) -> asm_ast.Function:
    return asm_ast.Function(
        name=name, is_global=True,
        params=list(params), instructions=list(instrs),
    )


class TestZpAbiParamResolution(unittest.TestCase):
    def test_no_param_layout_uses_frame(self) -> None:
        # Default behavior: param `p` resolves to a Frame slot.
        fn = _fn(
            "f",
            _mov(_ps("p"), _A()),
            _ret_bare(),
            params=["p"],
        )
        out, dims = replace_function_bare_exit(fn)
        # Param resolved to Frame.
        movs = [i for i in out.instructions if isinstance(i, asm_ast.Mov)]
        self.assertIsInstance(movs[0].src, asm_ast.Frame)

    def test_zp_layout_param_resolves_to_zp(self) -> None:
        # ZpLayout([0x80]) — 1 byte, single param.
        fn = _fn(
            "f",
            _mov(_ps("p"), _A()),
            _ret_bare(),
            params=["p"],
        )
        out, dims = replace_function_bare_exit(
            fn, param_layout=ZpLayout(slot_symbols=["__zpabi_f_p0"], addrs=[0x80]),
        )
        # arg_bytes is 0 — no soft-stack args.
        self.assertEqual(dims.arg_bytes, 0)
        # Param resolved to Data(__zpabi_f_p0, 0); the asm-emit
        # stage prints an EQU directive that binds the symbol to
        # the layout's address.
        movs = [i for i in out.instructions if isinstance(i, asm_ast.Mov)]
        self.assertIsInstance(movs[0].src, asm_ast.Data)
        self.assertEqual(movs[0].src.name, "__zpabi_f_p0")
        self.assertEqual(movs[0].src.offset, 0)

    def test_zp_layout_multi_byte_param_resolves_byte_by_byte(self) -> None:
        # ZpLayout([0x80, 0x81]) — 2 bytes, single param. Body
        # references both bytes.
        fn = _fn(
            "f",
            _mov(_ps("p", 0), _A()),
            _mov(_ps("p", 1), _A()),
            _ret_bare(),
            params=["p"],
        )
        out, dims = replace_function_bare_exit(
            fn, param_layout=ZpLayout(
                slot_symbols=["__zpabi_f_p0", "__zpabi_f_p1"],
                addrs=[0x80, 0x81],
            ),
        )
        self.assertEqual(dims.arg_bytes, 0)
        movs = [i for i in out.instructions if isinstance(i, asm_ast.Mov)]
        # First Mov reads byte 0 → Data(__zpabi_f_p0).
        self.assertIsInstance(movs[0].src, asm_ast.Data)
        self.assertEqual(movs[0].src.name, "__zpabi_f_p0")
        # Second Mov reads byte 1 → Data(__zpabi_f_p1).
        self.assertIsInstance(movs[1].src, asm_ast.Data)
        self.assertEqual(movs[1].src.name, "__zpabi_f_p1")

    def test_zp_layout_two_params_use_consecutive_addrs(self) -> None:
        # Two 1-byte params packed at $80, $81.
        fn = _fn(
            "f",
            _mov(_ps("a"), _A()),
            _mov(_ps("b"), _A()),
            _ret_bare(),
            params=["a", "b"],
        )
        out, dims = replace_function_bare_exit(
            fn, param_layout=ZpLayout(
                slot_symbols=["__zpabi_f_p0", "__zpabi_f_p1"],
                addrs=[0x80, 0x81],
            ),
        )
        self.assertEqual(dims.arg_bytes, 0)
        movs = [i for i in out.instructions if isinstance(i, asm_ast.Mov)]
        # First Mov reads `a` byte 0 → __zpabi_f_p0.
        self.assertEqual(movs[0].src.name, "__zpabi_f_p0")
        # Second Mov reads `b` byte 0 → __zpabi_f_p1.
        self.assertEqual(movs[1].src.name, "__zpabi_f_p1")

    def test_zp_layout_local_still_gets_frame(self) -> None:
        # Param in ZP, but a non-param local still gets a Frame slot.
        fn = _fn(
            "f",
            _mov(_ps("p"), _ps("local")),
            _mov(_ps("local"), _A()),
            _ret_bare(),
            params=["p"],
        )
        out, dims = replace_function_bare_exit(
            fn, param_layout=ZpLayout(slot_symbols=["__zpabi_f_p0"], addrs=[0x80]),
        )
        # local_bytes > 0 (the local got a Frame slot).
        self.assertGreater(dims.local_bytes, 0)
        # Find the Mov writing to `local` — its dst is Frame.
        first_mov = next(
            i for i in out.instructions if isinstance(i, asm_ast.Mov)
        )
        self.assertIsInstance(first_mov.dst, asm_ast.Frame)


class TestReplaceProgramBareExitWithLayouts(unittest.TestCase):
    def test_program_layouts_threaded_per_function(self) -> None:
        # Two functions, one ZP-ABI, one default. Verify each
        # gets its own resolution.
        prog = asm_ast.Program(top_level=[
            _fn(
                "zp_fn",
                _mov(_ps("x"), _A()),
                _ret_bare(),
                params=["x"],
            ),
            _fn(
                "ss_fn",
                _mov(_ps("y"), _A()),
                _ret_bare(),
                params=["y"],
            ),
        ])
        layouts = {
            "zp_fn": ZpLayout(
                slot_symbols=["__zpabi_zp_fn_p0"], addrs=[0x80],
            ),
            "ss_fn": SoftStackLayout(),
        }
        out, dims = replace_program_bare_exit(
            prog, param_layouts=layouts,
        )
        # Each function got the right resolution.
        zp_fn = next(
            tl for tl in out.top_level
            if isinstance(tl, asm_ast.Function) and tl.name == "zp_fn"
        )
        zp_movs = [
            i for i in zp_fn.instructions if isinstance(i, asm_ast.Mov)
        ]
        self.assertIsInstance(zp_movs[0].src, asm_ast.Data)
        self.assertEqual(zp_movs[0].src.name, "__zpabi_zp_fn_p0")

        ss_fn = next(
            tl for tl in out.top_level
            if isinstance(tl, asm_ast.Function) and tl.name == "ss_fn"
        )
        ss_movs = [
            i for i in ss_fn.instructions if isinstance(i, asm_ast.Mov)
        ]
        self.assertIsInstance(ss_movs[0].src, asm_ast.Frame)

        # arg_bytes per function: 0 for ZP-ABI, > 0 for soft-stack.
        self.assertEqual(dims["zp_fn"].arg_bytes, 0)
        self.assertGreater(dims["ss_fn"].arg_bytes, 0)


if __name__ == "__main__":
    unittest.main()
