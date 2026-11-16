"""Tests for `passes.jump_table.apply_jump_tables`: a chain of compares
of A with constants becomes a dispatch through a table of the cases'
addresses."""

import shutil
import unittest

import asm_ast
from passes.jump_table import apply_jump_tables
from sim.harness import build_sim

_A = asm_ast.Reg(reg=asm_ast.A())
_N = asm_ast.Data(name="n", offset=0)


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_jump_tables(prog).top_level[0].instructions


def _chain(keys, case_body):
    """`LDA n` and a compare chain on `keys`, a case for each that runs
    `case_body(k)` and returns, and a default that returns."""
    out = [asm_ast.Mov(src=_N, dst=_A)]
    for k in keys:
        out += [asm_ast.Compare(left=_A, right=asm_ast.Imm(value=k)),
                asm_ast.Branch(cond=asm_ast.EQ(), target=f".c{k}")]
    out += [asm_ast.Jump(target=".def")]
    for k in keys:
        out += [asm_ast.Label(name=f".c{k}"), *case_body(k),
                asm_ast.Return(save_a=False)]
    out += [asm_ast.Label(name=".def"), asm_ast.Return(save_a=False)]
    return out


def _store(k):
    return [asm_ast.Mov(src=asm_ast.Imm(value=k), dst=_N)]


class TestTable(unittest.TestCase):

    def test_dense_chain(self):
        out = _run(_chain([1, 2, 3, 4, 5, 6, 7, 8], _store))
        tables = [i for i in out if isinstance(i, asm_ast.JumpTable)]
        self.assertEqual(len(tables), 1)
        t = tables[0]
        # The smallest case is 1, so the table starts at 0, which goes
        # to the default.
        self.assertEqual(t.base, 0)
        self.assertEqual(t.targets, [".def", ".c1", ".c2", ".c3", ".c4",
                                     ".c5", ".c6", ".c7", ".c8"])
        self.assertEqual(t.index, "Y")
        k = out.index(t)
        self.assertEqual(out[k - 2:k], [
            asm_ast.Compare(left=_A, right=asm_ast.Imm(value=9)),
            asm_ast.Branch(cond=asm_ast.CS(), target=".def"),
        ])
        self.assertNotIn(asm_ast.Jump(target=".def"), out)

    def test_case_for_zero_after_ora(self):
        # A switch on a register parameter tests it with `ORA #$00`
        # and then `BEQ` for the case for 0, which joins the table,
        # and the `ORA` goes.
        instrs = _chain(list(range(1, 9)), _store)
        instrs[0:1] = [
            asm_ast.Or(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".c0"),
        ]
        instrs[-2:-2] = [asm_ast.Label(name=".c0"), *_store(0),
                         asm_ast.Return(save_a=False)]
        out = _run(instrs)
        t = next(i for i in out if isinstance(i, asm_ast.JumpTable))
        self.assertEqual(t.targets[0], ".c0")
        self.assertNotIn(asm_ast.Or(src=asm_ast.Imm(value=0), dst=_A), out)
        self.assertIsInstance(out[0], asm_ast.Compare)

    def test_lower_bound(self):
        out = _run(_chain(list(range(10, 19)), _store))
        t = next(i for i in out if isinstance(i, asm_ast.JumpTable))
        self.assertEqual(t.base, 10)
        k = out.index(t)
        self.assertEqual(out[k - 4:k - 2], [
            asm_ast.Compare(left=_A, right=asm_ast.Imm(value=10)),
            asm_ast.Branch(cond=asm_ast.CC(), target=".def"),
        ])

    def test_case_reads_a(self):
        # A case stores A, which holds the value the chain compared.
        instrs = _chain(
            [1, 2, 3, 4, 5],
            lambda k: [asm_ast.Mov(src=_A, dst=_N)],
        )
        self.assertEqual(_run(instrs), instrs)

    def test_too_few_cases(self):
        # Six compares and branches take 27 bytes with the JMP to the
        # default, and the table 29.
        instrs = _chain([1, 2, 3, 4, 5, 6], _store)
        self.assertEqual(_run(instrs), instrs)


_PROGRAM = """
#include <stdint.h>
uint8_t r[25];
static uint8_t pick(uint8_t n) {
    switch (n) {
    case 0: return 7;
    case 1: return 11;
    case 2: return 13;
    case 4: return 17;
    case 5: return 19;
    case 6: return 23;
    case 7: return 29;
    case 9: return 31;
    case 10: return 37;
    case 11: return 41;
    default: return 1;
    }
}
static uint8_t pick_high(uint8_t n) {
    switch (n) {
    case 100: return 2;
    case 101: return 3;
    case 102: return 5;
    case 103: return 7;
    case 104: return 11;
    case 105: return 13;
    case 106: return 17;
    case 107: return 19;
    case 108: return 23;
    default: return 0;
    }
}
int main(void) {
    for (uint8_t i = 0; i != 13; i++)
        r[i] = pick(i);
    r[13] = pick(200);
    for (uint8_t i = 0; i != 11; i++)
        r[14 + i] = pick_high((uint8_t)(99 + i));
    return 0;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRuns(unittest.TestCase):

    def test_values(self):
        want = [7, 11, 13, 1, 17, 19, 23, 29, 1, 31, 37, 41, 1, 1,
                0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 0]
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                sim.run()
                base = sim.symbols["r"]
                got = list(sim.mpu.memory[base:base + 25])
                self.assertEqual(got, want)

    def test_tables_made(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(_PROGRAM), optimize=True)
        # 12 entries for pick (0 to 11) and 9 for pick_high.
        self.assertEqual(asm.count("DC.W"), 21)
        self.assertEqual(asm.count("RTS\n.jtab@"), 2)


if __name__ == "__main__":
    unittest.main()
