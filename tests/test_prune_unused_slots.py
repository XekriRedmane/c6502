"""Tests for `passes.prune_unused_slots.prune_unused_locals`."""

from __future__ import annotations

import re
import shutil
import unittest

import asm_ast
from passes.prune_unused_slots import prune_unused_locals


def _prog(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="main", is_global=True, params=[], instructions=instrs,
    )])


class TestPruneUnusedLocals(unittest.TestCase):

    def test_unreferenced_local_dropped(self):
        prog = _prog([asm_ast.Return(save_a=False)])
        self.assertEqual(
            prune_unused_locals(prog, {"__local_main__0": 0x80}), {},
        )

    def test_address_halves_keep_local(self):
        # An address-taken local referenced only as `#<sym` / `#>sym`
        # needs its EQU, or the emitted file has an undefined symbol.
        a = asm_ast.Reg(reg=asm_ast.A())
        prog = _prog([
            asm_ast.Mov(
                src=asm_ast.ImmLabelLow(name="__local_main__a", offset=0),
                dst=a, is_volatile=False,
            ),
            asm_ast.Mov(
                src=asm_ast.ImmLabelHigh(name="__local_main__a", offset=0),
                dst=a, is_volatile=False,
            ),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(
            prune_unused_locals(prog, {"__local_main__a": 0x86}),
            {"__local_main__a": 0x86},
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestEmittedLocalsDefined(unittest.TestCase):

    def test_every_referenced_local_has_an_equ(self):
        from compile import _run_stage
        src = """
            int main(void) {
                int a[4] = {1, 2, 3, 4}; int i = 0;
                a[i++] += 10; a[i++] *= 3; a[--i] <<= 1;
                return a[0] + a[1] + i + a[2] - 1;
            }
        """
        out = _run_stage("codegen", src, optimize=True)
        body = "\n".join(
            ln for ln in out.splitlines() if not ln.startswith(";")
        )
        defined = set(re.findall(r"^(__local_\w+)\s+EQU", body, re.M))
        used = set(re.findall(r"\b(__local_\w+?)(?:\+\d+)?\b", body))
        self.assertTrue(used)
        self.assertEqual(used - defined, set())


if __name__ == "__main__":
    unittest.main()
