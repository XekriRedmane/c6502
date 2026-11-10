"""Compares `examples/zork_find_index_of_page_table.c` under --optimize
with the hand-written routine it translates, from the Zork I (r15)
Z-machine interpreter
(https://github.com/XekriRedmane/zorki-r15-reveng).

The original is assembled with py65's line assembler and run on the
same table contents and searches as the compiled function. Both are
timed from entry to RTS. The compiled code must return the same
results, be within 5% of the original's instruction count, and take
no more than 105% of its cycles. Today it matches the instruction
count and is about 6% faster, because the tables are constants in the
C (`CMP abs,Y`, 4 cycles) where the original reads them through
zero-page pointers (`CMP (zp),Y`, 5 cycles).

The passes this depends on are `passes/loop_invariant_a.py` (the key
stays in A across the loop) and `passes/counter_to_index_reg.py` (the
count-down counter lives in X)."""

from __future__ import annotations

import re
import shutil
import subprocess
import sys
import unittest
from pathlib import Path

from py65.assembler import Assembler
from py65.devices.mpu6502 import MPU

import sim.harness as H

_EXAMPLE = (
    Path(__file__).parent.parent / "examples"
    / "zork_find_index_of_page_table.c"
)
_N = 0x4C            # NUM_PAGE_TABLE_ENTRIES in Zork I
_LAST = 0x4B         # LAST_Z_PAGE
_PAGES = [0x0100 + 7 * k for k in range(_N)]
_SEARCHES = [
    _PAGES[0], _PAGES[10], _PAGES[37], _PAGES[75], 0x7777,
    # Low byte matches entry 20, high byte doesn't: exercises the
    # second compare and the reload of A on that path.
    (_PAGES[20] & 0xFF) | 0x3300,
]

# The original, with its zero-page variables at their documented
# addresses: SCRATCH2 $58, PAGE_L_TABLE $5A, PAGE_H_TABLE $5C,
# NUM_PAGE_TABLE_ENTRIES $BD, LAST_Z_PAGE $BE.
_ORIGINAL = [
    "LDX $BD", "LDY #$00", "LDA $58",
    "loop: CMP ($5A),Y", "BNE next", "LDA $59", "CMP ($5C),Y",
    "BEQ found", "LDA $58",
    "next: INY", "DEX", "BNE loop", "LDA $BE", "SEC", "RTS",
    "found: TYA", "CLC", "RTS",
]
_ORIGIN = 0x1000
_RETURN = 0x0F00     # a BRK the routine returns to


def _assemble_original(mpu: MPU) -> None:
    asm = Assembler(mpu)
    labels: dict[str, int] = {}
    for pass_ in range(2):
        pc = _ORIGIN
        for line in _ORIGINAL:
            if ":" in line:
                label, line = (part.strip() for part in line.split(":", 1))
                labels[label] = pc
            for label in ("loop", "next", "found"):
                line = re.sub(rf"\b{label}\b", f"${labels.get(label, pc):04X}", line)
            code = asm.assemble(line, pc)
            if pass_ == 1:
                mpu.memory[pc:pc + len(code)] = code
            pc += len(code)


def _call(mpu: MPU, entry: int) -> int:
    """Runs the subroutine at `entry` to its RTS; returns cycles."""
    mpu.memory[_RETURN] = 0x00
    mpu.memory[0x01FF] = ((_RETURN - 1) >> 8) & 0xFF
    mpu.memory[0x01FE] = (_RETURN - 1) & 0xFF
    mpu.sp = 0xFD
    mpu.pc = entry
    start = mpu.processorCycles
    while mpu.pc != _RETURN:
        mpu.step()
    return mpu.processorCycles - start


def _fill_tables(mem) -> None:
    for k, p in enumerate(_PAGES):
        mem[0x2200 + k] = p & 0xFF
        mem[0x2280 + k] = p >> 8


def _expected(page: int) -> tuple[int, bool]:
    if page in _PAGES:
        return _PAGES.index(page), True
    return _LAST, False


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestFindIndexOfPageTable(unittest.TestCase):

    @classmethod
    def setUpClass(cls) -> None:
        src = _EXAMPLE.read_text()
        asm_text = subprocess.run(
            [sys.executable, "compile.py", str(_EXAMPLE), "--codegen", "--optimize"],
            capture_output=True, text=True, check=True,
            cwd=Path(__file__).parent.parent,
        ).stdout
        body = asm_text.split("find_index_of_page_table:")[1].split("\nmain:")[0]
        cls.compiled_count = sum(
            1 for line in body.splitlines()
            if re.match(r"^\s+[A-Z]{3}\b", line)
        )
        cls.sim = H.build_sim(src, optimize=True)
        _, _, _, cls.zp_symbols = H.compile_to_asm(src, optimize=True)

    def _run_original(self, page: int) -> tuple[int, int, bool]:
        mpu = MPU()
        _assemble_original(mpu)
        mem = mpu.memory
        _fill_tables(mem)
        mem[0xBD], mem[0xBE] = _N, _LAST
        mem[0x58], mem[0x59] = page & 0xFF, page >> 8
        mem[0x5A], mem[0x5B] = 0x00, 0x22
        mem[0x5C], mem[0x5D] = 0x80, 0x22
        cycles = _call(mpu, _ORIGIN)
        return cycles, mpu.a, not (mpu.p & 1)

    def _run_compiled(self, page: int) -> tuple[int, int, bool]:
        mpu, sym = self.sim.mpu, self.sim.symbols
        mem = mpu.memory
        _fill_tables(mem)
        mem[sym["num_page_table_entries"]] = _N
        mem[sym["last_z_page"]] = _LAST
        mem[self.zp_symbols["__zpabi_find_index_of_page_table__page_0"]] = page & 0xFF
        mem[self.zp_symbols["__zpabi_find_index_of_page_table__page_1"]] = page >> 8
        cycles = _call(mpu, sym["find_index_of_page_table"])
        return cycles, mpu.a, mpu.x == 0

    def test_results_match_original(self) -> None:
        for page in _SEARCHES:
            with self.subTest(page=hex(page)):
                _, oa, ofound = self._run_original(page)
                _, ca, cfound = self._run_compiled(page)
                self.assertEqual((oa, ofound), _expected(page))
                self.assertEqual((ca, cfound), _expected(page))

    def test_size_within_five_percent(self) -> None:
        self.assertLessEqual(
            abs(self.compiled_count - len(_ORIGINAL)), 0.05 * len(_ORIGINAL),
        )

    def test_cycles_no_more_than_five_percent_over(self) -> None:
        original = sum(self._run_original(p)[0] for p in _SEARCHES)
        compiled = sum(self._run_compiled(p)[0] for p in _SEARCHES)
        self.assertLessEqual(compiled, 1.05 * original)


if __name__ == "__main__":
    unittest.main()
