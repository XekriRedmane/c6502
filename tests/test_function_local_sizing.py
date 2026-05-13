"""Behavioral tests for `passes.function_local_sizing.compute_local_bytes`.

Coverage:
  - Empty function (no instructions) → 0.
  - Function with no ZP operands (only Pseudo / Frame / Imm) → 0.
  - Single ZP byte used → 1.
  - Multiple ZP bytes; duplicates collapse.
  - Multi-byte ZP slot (each byte counted separately).
  - Non-function top-levels skipped.
  - End-to-end: compile a small C program and verify the body's
    byte count matches the asm regalloc's coloring.
"""
from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.function_local_sizing import (
    compute_local_byte_addresses, compute_local_bytes,
)


def _zp(addr: int, off: int = 0) -> asm_ast.ZP:
    return asm_ast.ZP(address=addr, offset=off)


def _A() -> asm_ast.Reg:
    return asm_ast.Reg(reg=asm_ast.A())


def _imm(v: int) -> asm_ast.Imm:
    return asm_ast.Imm(value=v)


def _mov(src, dst) -> asm_ast.Mov:
    return asm_ast.Mov(src=src, dst=dst)


def _fn(name: str, *instrs) -> asm_ast.Function:
    return asm_ast.Function(
        name=name, is_global=True, params=[],
        instructions=list(instrs),
    )


def _prog(*tls) -> asm_ast.Program:
    return asm_ast.Program(top_level=list(tls))


class TestComputeLocalBytes(unittest.TestCase):
    def test_empty_function_is_zero(self) -> None:
        prog = _prog(_fn("f"))
        self.assertEqual(compute_local_bytes(prog), {"f": 0})

    def test_no_zp_operands_is_zero(self) -> None:
        # Pseudo / Imm / Frame / Reg shouldn't contribute.
        prog = _prog(_fn(
            "f",
            _mov(_imm(0x55), _A()),
            _mov(_A(), asm_ast.Frame(offset=1)),
            _mov(asm_ast.Pseudo(name="p", offset=0), _A()),
        ))
        self.assertEqual(compute_local_bytes(prog), {"f": 0})

    def test_single_zp_byte(self) -> None:
        prog = _prog(_fn(
            "f",
            _mov(_imm(0x42), _zp(0x80)),
        ))
        self.assertEqual(compute_local_bytes(prog), {"f": 1})

    def test_multiple_distinct_zp_bytes(self) -> None:
        prog = _prog(_fn(
            "f",
            _mov(_imm(1), _zp(0x80)),
            _mov(_imm(2), _zp(0x81)),
            _mov(_imm(3), _zp(0x82)),
        ))
        self.assertEqual(compute_local_bytes(prog), {"f": 3})

    def test_duplicate_zp_writes_collapse(self) -> None:
        # Same byte written and re-read; counts once.
        prog = _prog(_fn(
            "f",
            _mov(_imm(1), _zp(0x80)),
            _mov(_zp(0x80), _A()),
            _mov(_A(), _zp(0x80)),
        ))
        self.assertEqual(compute_local_bytes(prog), {"f": 1})

    def test_zp_with_offset_counts_each_byte(self) -> None:
        # A multi-byte value materialized as ZP(addr, offset)
        # operands shows each byte distinctly. offset is added to
        # address.
        prog = _prog(_fn(
            "f",
            _mov(_imm(1), _zp(0x90, 0)),
            _mov(_imm(2), _zp(0x90, 1)),
        ))
        self.assertEqual(compute_local_bytes(prog), {"f": 2})
        self.assertEqual(
            compute_local_byte_addresses(prog),
            {"f": frozenset({0x90, 0x91})},
        )

    def test_per_function_independent(self) -> None:
        # Two functions; each gets its own count.
        prog = _prog(
            _fn("f1", _mov(_imm(0), _zp(0x80)), _mov(_imm(0), _zp(0x81))),
            _fn("f2", _mov(_imm(0), _zp(0xC0))),
        )
        self.assertEqual(compute_local_bytes(prog), {"f1": 2, "f2": 1})

    def test_static_variable_top_level_skipped(self) -> None:
        prog = _prog(
            _fn("f", _mov(_imm(0), _zp(0x80))),
            asm_ast.StaticVariable(
                name="x", is_global=False,
                init=[asm_ast.IntInit(value=0)],
            ),
        )
        # StaticVariable not in the output.
        self.assertEqual(compute_local_bytes(prog), {"f": 1})

    def test_data_operands_ignored(self) -> None:
        # Data refs (statics, HARGS, __zpabi_* slot symbols) aren't
        # body-local ZP usage and shouldn't be counted.
        prog = _prog(_fn(
            "f",
            _mov(_imm(0x55), asm_ast.Data(name="HARGS", offset=0)),
            _mov(asm_ast.Data(name="__zpabi_f_p0", offset=0), _A()),
        ))
        self.assertEqual(compute_local_bytes(prog), {"f": 0})


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestComputeLocalBytesEndToEnd(unittest.TestCase):
    """Integration test: compile a small program through the
    optimizer and confirm the sizing helper sees the same byte
    count the regalloc colored."""

    def _compile_through_optimizer(self, src: str):
        """Return the (asm_program, colorings) pair AFTER
        `optimize_program` (so apply_coloring has rewritten
        Pseudo body locals to ZP operands) but BEFORE
        `replace_pseudoregisters_bare_exit`."""
        from c99_to_tac import translate_program as translate_to_tac
        from parser import parse
        from passes.identifier_resolution import (
            resolve_program as resolve_identifiers,
        )
        from passes.label_resolution import (
            resolve_program as resolve_labels,
        )
        from passes.loop_labeling import label_program as label_loops
        from passes.optimization import optimize_program as optimize_tac
        from passes.optimization_asm import optimizer as asm_opt
        from passes.abi_selection import select_abi
        from passes.zp_slot_allocation import allocate_zp_slots
        from passes.string_lifting import lift_program as lift_strings
        from passes.type_checking import (
            check_program as type_check_program, StaticAttr,
        )
        from preprocessor import preprocess
        from tac_to_asm import translate_program as translate_to_asm
        pp = preprocess(src)
        ast0 = parse(pp)
        ast1 = resolve_identifiers(ast0)
        ast2 = lift_strings(ast1)
        ast3 = resolve_labels(ast2)
        ast4 = label_loops(ast3)
        ast5, syms, types = type_check_program(ast4)
        tac = translate_to_tac(ast5, syms, types)
        statics = frozenset(
            n for n, s in syms.items() if isinstance(s.attrs, StaticAttr)
        )
        tac = optimize_tac(tac, syms)
        abi = select_abi(tac, ast5, types)
        abi, _ = allocate_zp_slots(tac, abi)
        asm0 = translate_to_asm(
            tac, syms, types, bare_exit=True, abi=abi,
        )
        asm1, colorings = asm_opt.optimize_program(
            asm0, extra_statics=statics, param_layouts=abi,
            symbols=syms,
        )
        return asm1, colorings

    def test_simple_function_byte_count_matches_coloring(self) -> None:
        # `int main(void) { int x = 5; int y = x + 1; return y; }`.
        # The regalloc colors x and y into ZP body locals. The exact
        # byte count depends on coalescing; we just assert sizing
        # agrees with the coloring's distinct ZP byte set.
        src = (
            "int main(void) { "
            "  int x = 5; "
            "  int y = x + 1; "
            "  return y; "
            "}"
        )
        prog, colorings = self._compile_through_optimizer(src)
        sizes = compute_local_bytes(prog)
        # main's distinct ZP byte count from the asm-walk.
        # Compare to the coloring's distinct addresses, accounting
        # for width: each coalesced rep occupies `width` bytes
        # starting at its base address.
        main_coloring = colorings["main"]
        expected_bytes: set[int] = set()
        for name, base in main_coloring.assignments.items():
            # All asm-SSA body locals are 1 byte; the IR only carries
            # multi-byte names in narrow shapes (LoadAddress.dst) that
            # don't appear here. Verify via the asm walk regardless.
            expected_bytes.add(base)
        # Sizing helper's count is a lower bound on the coloring's
        # distinct base addresses (Pseudos that came from coalescing
        # don't add new bytes), and an upper bound on bytes actually
        # used in body instructions. They should match exactly for
        # this simple no-call program.
        self.assertEqual(sizes["main"], len(expected_bytes))


if __name__ == "__main__":
    unittest.main()
