"""Tests for the code around arguments that arrive in A and X.

- A 2-byte register parameter's high byte stays in X, the register it
  arrives in (`optimizer._entry_register_limits`, per byte).
- Two byte arguments passed on in the other order swap through A and
  a slot (`redundant_load._swap_loads_ok`).
- A byte stored from A and loaded into X or Y is transferred
  (`redundant_load._redirect_index_load_to_transfer`), and a count down
  in Y that this leaves as `TYA; SEC; SBC #1; TAY` is `DEY`
  (`dec_register_peephole._match_transfer_back`).
- A transfer made again after a run of branches goes
  (`redundant_load._repeats_transfer`).
- `LDA abs,Y; TAX` is `LDX abs,Y` (`direct_index_load`).
- `x_save_slot_load` doesn't take `STX M; TAX` for X holding M.

Each compiled shape is checked, and each program runs in the
simulator in both pipelines.
"""
from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.dec_register_peephole import apply_dec_register_peephole
from passes.direct_index_load import apply_direct_index_load
from passes.redundant_load import apply_redundant_load_elimination
from passes.x_save_slot_load import apply_x_save_slot_load
from preprocessor import preprocess
from sim.harness import build_sim

_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_Y = asm_ast.Reg(reg=asm_ast.Y())


def _prog(instrs) -> asm_ast.Program:
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=True, params=[], instructions=list(instrs),
    )])


def _instrs(prog: asm_ast.Program) -> list:
    return prog.top_level[0].instructions


def _data(name: str) -> asm_ast.Data:
    return asm_ast.Data(name=name, offset=0)


def _codegen(src: str) -> str:
    from compile import _run_stage
    return _run_stage("codegen", preprocess(src), optimize=True)


def _body(asm: str, name: str) -> list[str]:
    """The instruction and local label lines of function `name`."""
    text = asm.split(f"\n{name}:\n")[1]
    out = []
    for line in text.splitlines():
        line = line.strip()
        if not line or line.startswith(";") or line == "SUBROUTINE":
            continue
        if line.endswith(":") and not line.startswith("."):
            break
        out.append(" ".join(line.split()))
    return out


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not installed")
class TestCompiled(unittest.TestCase):
    """The code for each shape, from `-O`."""

    def test_high_byte_stays_in_x(self) -> None:
        src = ("unsigned char k;"
               "unsigned int widen(unsigned int y) { return y - k; }")
        self.assertEqual(_body(_codegen(src), "widen"), [
            "SEC", "SBC k", "BCS .carry_inc@0", "DEX", ".carry_inc@0:",
            "RTS",
        ])

    def test_arguments_swap(self) -> None:
        src = ("#include <stdint.h>\n"
               "void g(uint8_t a, uint8_t b);"
               "void f(uint8_t a, uint8_t b) { g(b, a); }")
        self.assertEqual(_body(_codegen(src), "f"), [
            "STA __local_f__a", "TXA", "LDX __local_f__a", "JMP g",
        ])

    def test_count_down_in_y(self) -> None:
        src = ("#include <stdint.h>\n"
               "void wait(uint8_t delay) {"
               "    register volatile uint8_t y = delay;"
               "    while (--y != 0) { }"
               "}")
        self.assertEqual(_body(_codegen(src), "wait"), [
            "TAY", ".loop@0_continue:", "DEY", "BNE .loop@0_continue",
            "RTS",
        ])


class TestSwapLoads(unittest.TestCase):
    """`redundant_load`'s swap of `LDX M1; LDA M2` with X holding M2."""

    def _swap(self, tail):
        return _instrs(apply_redundant_load_elimination(_prog([
            asm_ast.Mov(src=_A, dst=_data("a")),
            asm_ast.Mov(src=_X, dst=_data("b")),
            asm_ast.Mov(src=_data("a"), dst=_X),
            asm_ast.Mov(src=_data("b"), dst=_A),
            *tail,
        ])))

    def test_swaps_to_a_transfer(self) -> None:
        out = self._swap([asm_ast.Return(save_a=True, save_x=True, save_c=False)])
        self.assertEqual(out[2:4], [
            asm_ast.Mov(src=_X, dst=_A),
            asm_ast.Mov(src=_data("a"), dst=_X),
        ])

    def test_flags_read_after_keep_the_order(self) -> None:
        # N and Z after the pair are set from M2, which a swap would
        # leave set from M1.
        out = self._swap([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".z"),
            asm_ast.Return(save_a=True, save_x=True, save_c=False),
            asm_ast.Label(name=".z"),
            asm_ast.Return(save_a=True, save_x=True, save_c=False),
        ])
        self.assertEqual(out[3], asm_ast.Mov(src=_data("b"), dst=_A))


class TestTransferForLoad(unittest.TestCase):
    """`LDX M` / `LDY M` from A when A holds M."""

    def test_slot_dead_after(self) -> None:
        slot = _data("__local_f__d")
        out = _instrs(apply_redundant_load_elimination(_prog([
            asm_ast.Mov(src=_A, dst=slot),
            asm_ast.Mov(src=slot, dst=_Y),
            asm_ast.Return(save_a=False, save_x=False, save_c=False),
        ])))
        self.assertIn(asm_ast.Mov(src=_A, dst=_Y), out)

    def test_slot_read_after_stays(self) -> None:
        # A loop counter's `LDX i` stays for the promotions after the
        # first peephole fixed point.
        slot = _data("__local_f__i")
        load = asm_ast.Mov(src=slot, dst=_X)
        out = _instrs(apply_redundant_load_elimination(_prog([
            asm_ast.Mov(src=_A, dst=slot),
            load,
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.IndexedData(
                name="t", offset=0, index=asm_ast.X())),
            asm_ast.Mov(src=slot, dst=_A),
            asm_ast.Return(save_a=True, save_x=False, save_c=False),
        ])))
        self.assertIn(load, out)

    def test_named_object_still_read(self) -> None:
        out = _instrs(apply_redundant_load_elimination(_prog([
            asm_ast.Mov(src=_data("C2"), dst=_A),
            asm_ast.Mov(src=_A, dst=_data("B1")),
            asm_ast.Mov(src=_data("C2"), dst=_X),
            asm_ast.Mov(src=_data("C1"), dst=_A),
            asm_ast.Return(save_a=True, save_x=True, save_c=False),
        ])))
        self.assertEqual(out[2], asm_ast.Mov(src=_A, dst=_X))

    def test_volatile_load_needs_the_store_before(self) -> None:
        load = asm_ast.Mov(src=_data("v"), dst=_X, is_volatile=True)
        out = _instrs(apply_redundant_load_elimination(_prog([
            asm_ast.Mov(src=_data("v"), dst=_A, is_volatile=True),
            asm_ast.Mov(src=_A, dst=_data("w")),
            load,
            asm_ast.Return(save_a=False, save_x=True, save_c=False),
        ])))
        self.assertIn(load, out)


class TestRepeatedTransfer(unittest.TestCase):
    def _run(self, between):
        return _instrs(apply_redundant_load_elimination(_prog([
            asm_ast.Mov(src=_X, dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".two"),
            *between,
            asm_ast.Mov(src=_X, dst=_A),
            asm_ast.Branch(cond=asm_ast.PL(), target=".two"),
            asm_ast.Return(save_a=True, save_x=False, save_c=False),
            asm_ast.Label(name=".two"),
            asm_ast.Return(save_a=True, save_x=False, save_c=False),
        ])))

    def test_only_branches_between(self) -> None:
        out = self._run([])
        self.assertEqual(
            sum(1 for ins in out if ins == asm_ast.Mov(src=_X, dst=_A)), 1)

    def test_a_label_between_keeps_it(self) -> None:
        out = self._run([asm_ast.Label(name=".in")])
        self.assertEqual(
            sum(1 for ins in out if ins == asm_ast.Mov(src=_X, dst=_A)), 2)


class TestDecTransferBack(unittest.TestCase):
    def _run(self, tail):
        return _instrs(apply_dec_register_peephole(_prog([
            asm_ast.Label(name=".top"),
            asm_ast.Mov(src=_Y, dst=_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=_A),
            asm_ast.Mov(src=_A, dst=_Y),
            asm_ast.Branch(cond=asm_ast.NE(), target=".top"),
            *tail,
        ])))

    def test_becomes_dey(self) -> None:
        out = self._run([asm_ast.Return(save_a=False, save_x=False, save_c=False)])
        self.assertEqual(out[1:3], [
            asm_ast.Dec(dst=_Y),
            asm_ast.Branch(cond=asm_ast.NE(), target=".top"),
        ])

    def test_a_read_after_stays(self) -> None:
        out = self._run([asm_ast.Return(save_a=True, save_x=False, save_c=False)])
        self.assertNotIn(asm_ast.Dec(dst=_Y), out)

    def test_carry_read_after_stays(self) -> None:
        out = self._run([
            asm_ast.Branch(cond=asm_ast.CC(), target=".top"),
            asm_ast.Return(save_a=False, save_x=False, save_c=False),
        ])
        self.assertNotIn(asm_ast.Dec(dst=_Y), out)


class TestDirectIndexedLoad(unittest.TestCase):
    def test_other_index_folds(self) -> None:
        src = asm_ast.IndexedData(name="t", offset=0, index=asm_ast.Y())
        out = _instrs(apply_direct_index_load(_prog([
            asm_ast.Mov(src=src, dst=_A),
            asm_ast.Mov(src=_A, dst=_X),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A),
        ])))
        self.assertEqual(out[0], asm_ast.Mov(src=src, dst=_X))

    def test_same_index_stays(self) -> None:
        src = asm_ast.IndexedData(name="t", offset=0, index=asm_ast.X())
        instrs = [
            asm_ast.Mov(src=src, dst=_A),
            asm_ast.Mov(src=_A, dst=_X),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A),
        ]
        self.assertEqual(_instrs(apply_direct_index_load(_prog(instrs))),
                         instrs)


class TestXSaveSlot(unittest.TestCase):
    def test_stx_then_tax_isnt_the_init(self) -> None:
        # `STX M; TAX` leaves A's value in X, so `LDA M` reads memory.
        slot = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=_X, dst=slot),
            asm_ast.Mov(src=_A, dst=_X),
            asm_ast.Mov(src=slot, dst=_A),
            asm_ast.Return(save_a=True, save_x=True, save_c=False),
        ]
        self.assertEqual(_instrs(apply_x_save_slot_load(_prog(instrs))),
                         instrs)


_PROGRAM = """
#include <stdint.h>
unsigned char k;
unsigned int widen(unsigned int y) { return y - k; }
uint16_t pair(uint8_t a, uint8_t b) { return (uint16_t)a << 8 | b; }
uint16_t swap(uint8_t a, uint8_t b) { return pair(b, a); }
uint8_t count(uint8_t d) {
    register uint8_t y = d;
    uint8_t n = 0;
    while (--y != 0) n++;
    return n;
}
int main(void) {
    k = 5;
    if (widen(3) != 0xFFFE) return 1;
    k = 0;
    if (widen(0x1234) != 0x1234) return 2;
    k = 0xFF;
    if (widen(0x100) != 1) return 3;
    if (widen(0xFF) != 0) return 4;
    if (swap(0x12, 0x34) != 0x3412) return 5;
    if (swap(0, 0xFF) != 0xFF00) return 6;
    if (count(1) != 0) return 7;
    if (count(5) != 4) return 8;
    if (count(0) != 255) return 9;
    return 0;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not installed")
class TestSimulated(unittest.TestCase):
    def test_both_pipelines(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run()
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), 0)


if __name__ == "__main__":
    unittest.main()
