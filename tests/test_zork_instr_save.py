"""The multi-unit example in examples/zork/instr_save: Zork's `save`
instruction, in three C files linked with a linker script.

`TestGoldOutputs` checks each unit's `--optimize` output and the linked
program against the checked-in `.asm` files.

The rest runs the linked program in the simulator (assembled with
dasm, so it skips without dasm). Zork's other routines, the Monitor's
RDKEY and CLREOL, and DOS's RWTS are Python hooks at the addresses they
have in Zork, which record what they're asked to do: the characters
added to the line buffer, the prompts printed in inverse, the keys
read, and each sector written, with its slot, drive, track, sector,
and 256 bytes. Two scenarios run: a save to slot 6, drive 1 (the game's
own drive) at position 3, with a wrong key and a default answer along
the way, which then asks for the game diskette back; and a save to
slot 5, drive 2 whose fifth write fails.

When the environment variable ZORK_MAIN_BIN names Zork's main.bin
(https://github.com/XekriRedmane/zorki-r15-reveng, loaded at $0800),
`test_matches_original` runs Zork's own instr_save in the same scenarios
and compares everything recorded, and the state each leaves, with the
C version's. The binary isn't part of this repository.
"""
from __future__ import annotations

import io
import os
import shutil
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from py65.devices.mpu6502 import MPU

_DIR = Path(__file__).parent.parent / "examples" / "zork" / "instr_save"
_UNITS = ["disk", "prompts", "save"]
_HAVE_PCPP = shutil.which("pcpp") is not None
_HAVE_DASM = shutil.which("dasm") is not None

# Zork's routines and the ROM's, at their addresses.
_HOME, _MORE, _LINE, _BUFFER_CHAR = 0x1CC9, 0x1C5B, 0x1BE2, 0x1B90
_COUT_STRING, _BRANCH, _NEGATED_BRANCH = 0x1D34, 0x0BAA, 0x0BB3
_RDKEY, _CLREOL, _RWTS = 0xFD0C, 0xFC9C, 0x2900
_INSTR_SAVE = 0x2064         # in main.bin

_STORY = 0x6000              # Z_HEADER_ADDR
_STATIC_PAGES = 2            # header byte $0E

# Where each version keeps the state compared at the end: the IOB's
# slot and drive, the three remembered answers, and the DCT's motor
# count.
_ORIGINAL_STATE = {
    "slot": 0x1DAD, "drive": 0x1DAE,
    "answers": (0x1E7A, 0x1E8A, 0x1E9A), "motor": 0x1DBF,
}


def _compile(unit: str, out: Path) -> None:
    from compile import main as compile_main
    assert compile_main([
        "compile.py", str(_DIR / f"{unit}.c"), "--codegen", "--optimize",
        "-o", str(out),
    ]) == 0


def _link(paths: list[Path], out: Path) -> None:
    from compile import main as compile_main
    assert compile_main([
        "compile.py", "--link", "-T", str(_DIR / "instr_save.ld"),
        *map(str, paths), "-o", str(out),
    ]) == 0


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestGoldOutputs(unittest.TestCase):
    """Each unit's output and the linked program match the checked-in
    files. To bless new ones, run the commands in the example's
    README."""

    def test_units_and_link(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            paths = []
            for unit in _UNITS:
                path = Path(tmp) / f"{unit}.asm"
                _compile(unit, path)
                paths.append(path)
                with self.subTest(unit=unit):
                    self.assertEqual(
                        path.read_text(),
                        (_DIR / f"{unit}.asm").read_text(),
                    )
            linked = Path(tmp) / "instr_save.asm"
            _link(paths, linked)
            self.assertEqual(
                linked.read_text(),
                (_DIR / "instr_save.asm").read_text(),
            )


class _Machine:
    """One MPU with the story, the interpreter state, and the hooks.
    `keys` are what RDKEY returns, in order, and the write numbered
    `fail_write` (from 1) fails."""

    def __init__(self, mpu: MPU, keys: list[int],
                 fail_write: int | None = None) -> None:
        self.mpu = mpu
        self.keys = list(keys)
        self.fail_write = fail_write
        self.events: list[tuple] = []
        m = mpu.memory
        m[0x7F] = 16                                  # SECTORS_PER_TRACK
        m[0x8A:0x8D] = bytes([0x34, 0x12, 0x00])      # Z_PC
        m[0x9A:0xB8] = bytes(range(0x40, 0x40 + 30))  # LOCAL_ZVARS
        m[0xC8:0xCE] = bytes([5, 0xE0, 0x03, 0xE8, 0x03, 2])
        m[0xBA], m[0xBB] = _STORY & 0xFF, _STORY >> 8
        m[0x24], m[0x32] = 0, 0xFF                    # CH, INVFLG
        story = bytes((7 * i + 3) & 0xFF for i in range(0x400))
        m[_STORY:_STORY + 0x400] = story
        m[_STORY] = 3                                 # version
        m[_STORY + 0x0E] = _STATIC_PAGES
        # The line buffer and the Z-machine's stack.
        m[0x0200:0x03E8] = bytes((5 * i + 1) & 0xFF for i in range(0x1E8))
        self.writes = 0
        self.hooks = {
            _HOME: lambda: self.events.append(("home",)),
            _MORE: lambda: self.events.append(("more",)),
            _LINE: lambda: self.events.append(("line",)),
            _BUFFER_CHAR: lambda: self.events.append(("char", mpu.a)),
            _COUT_STRING: self._cout_string,
            _BRANCH: lambda: self.events.append(("branch",)),
            _NEGATED_BRANCH: lambda: self.events.append(("negated",)),
            _RDKEY: self._rdkey,
            _CLREOL: lambda: self.events.append(("clreol",)),
            _RWTS: self._rwts,
        }

    def _cout_string(self) -> None:
        m = self.mpu.memory
        ptr = m[0xE6] | m[0xE7] << 8
        count = self.mpu.x or 256
        self.events.append(
            ("cout", bytes(m[ptr:ptr + count]), m[0x24], m[0x32]),
        )

    def _rdkey(self) -> None:
        key = self.keys.pop(0)
        self.events.append(("key", key))
        self.mpu.a = key

    def _rwts(self) -> None:
        m = self.mpu.memory
        iob = (self.mpu.a << 8) | self.mpu.y
        buf = m[iob + 8] | m[iob + 9] << 8
        self.writes += 1
        self.events.append((
            "rwts", m[iob + 12], m[iob + 1], m[iob + 2], m[iob + 4],
            m[iob + 5], bytes(m[buf:buf + 256]),
        ))
        if self.writes == self.fail_write:
            self.mpu.p |= 1
        else:
            self.mpu.p &= ~1

    def run(self, other_hooks=None, max_steps: int = 2_000_000) -> None:
        mpu = self.mpu
        hooks = dict(other_hooks or {})
        for k, v in self.hooks.items():
            hooks[k] = lambda _m, f=v: f()
        for _ in range(max_steps):
            if mpu.pc in hooks:
                hooks[mpu.pc](mpu.memory)
                sp = mpu.sp
                lo = mpu.memory[0x0100 + ((sp + 1) & 0xFF)]
                hi = mpu.memory[0x0100 + ((sp + 2) & 0xFF)]
                mpu.sp = (sp + 2) & 0xFF
                mpu.pc = (((hi << 8) | lo) + 1) & 0xFFFF
                continue
            if mpu.memory[mpu.pc] == 0x00:
                return
            mpu.step()
        raise AssertionError("ran too long")

    def state(self, where: dict) -> dict:
        m = self.mpu.memory
        return {
            "slot": m[where["slot"]], "drive": m[where["drive"]],
            "answers": bytes(m[a] for a in where["answers"]),
            "motor": m[where["motor"]] | m[where["motor"] + 1] << 8,
            "scratch1": m[0xE4] | m[0xE5] << 8,
            "ch": m[0x24], "invflg": m[0x32],
            "story": bytes(m[_STORY:_STORY + 0x400]),
        }


# An invalid position, then 3; RETURN for the default slot, 6; drive
# 1; a stray key, then RETURN to begin; and a stray key, then RETURN,
# when asked for the game diskette back.
_SAME_DRIVE = [0xB9, 0xB3, 0x8D, 0xB1, 0xC1, 0x8D, 0xC2, 0x8D]
# Position 0, slot 5, drive 2, RETURN to begin.
_OTHER_DRIVE = [0xB0, 0xB5, 0xB2, 0x8D]

_DRIVER = """
#include "zork.h"
int main(void) { instr_save(); return 0; }
"""


def _linked_text() -> str:
    """The example's units and a `main` that calls instr_save, linked
    by the example's script."""
    from compile import main as compile_main
    with tempfile.TemporaryDirectory() as tmp:
        paths = []
        for unit in _UNITS:
            path = Path(tmp) / f"{unit}.asm"
            _compile(unit, path)
            paths.append(path)
        driver = Path(tmp) / "driver.asm"
        with patch("sys.stdin", io.StringIO(_DRIVER)):
            assert compile_main([
                "compile.py", "-", "--codegen", "--optimize",
                "-I", str(_DIR), "-o", str(driver),
            ]) == 0
        linked = Path(tmp) / "linked.asm"
        _link(paths + [driver], linked)
        return linked.read_text()


def _run_c(text: str, keys, fail_write=None) -> tuple[_Machine, dict]:
    from sim.linked import build_linked_sim
    sim = build_linked_sim(text)
    machine = _Machine(sim.mpu, keys, fail_write)
    machine.run(sim.runtime.hooks)
    iob, dct, prompts = (sim.symbols[n] for n in ("iob", "dct", "prompts"))
    where = {
        "slot": iob + 1, "drive": iob + 2,
        "answers": (prompts + 15, prompts + 31, prompts + 47),
        "motor": dct + 2,
    }
    return machine, machine.state(where)


def _run_original(path: str, keys, fail_write=None):
    mpu = MPU()
    binary = Path(path).read_bytes()
    mpu.memory[0x0800:0x0800 + len(binary)] = binary
    # JSR instr_save ; BRK
    mpu.memory[0x05F0:0x05F4] = bytes([
        0x20, _INSTR_SAVE & 0xFF, _INSTR_SAVE >> 8, 0x00,
    ])
    mpu.pc = 0x05F0
    mpu.sp = 0xFF
    machine = _Machine(mpu, keys, fail_write)
    machine.run()
    return machine, machine.state(_ORIGINAL_STATE)


def _sectors(events) -> list[tuple]:
    return [e for e in events if e[0] == "rwts"]


@unittest.skipUnless(_HAVE_PCPP and _HAVE_DASM, "needs pcpp and dasm")
class TestRuns(unittest.TestCase):

    @classmethod
    def setUpClass(cls) -> None:
        cls.text = _linked_text()
        cls.same = _run_c(cls.text, _SAME_DRIVE)
        cls.other = _run_c(cls.text, _OTHER_DRIVE, fail_write=5)

    def test_sectors_written(self) -> None:
        machine, state = self.same
        sectors = _sectors(machine.events)
        # Three sectors of state, then dynamic memory: the pages up to
        # static memory and one more.
        self.assertEqual(len(sectors), 3 + _STATIC_PAGES + 2)
        # Position 3 starts at sector 192: track 3 + 12, sector 0.
        self.assertEqual([s[4:6] for s in sectors[:2]],
                         [(15, 0), (15, 1)])
        # Each one a write, to slot 6, drive 1.
        self.assertTrue(all(s[1:4] == (2, 0x60, 1) for s in sectors))
        first = sectors[0][6]
        self.assertEqual(first[0], 3)                    # version
        # Z_PC, last byte first.
        self.assertEqual(first[1:4], bytes([0x00, 0x12, 0x34]))
        self.assertEqual(sectors[3][6], state["story"][:256])
        self.assertEqual(machine.events[-1], ("branch",))

    def test_prompts_and_answers(self) -> None:
        machine, state = self.same
        couts = [e for e in machine.events if e[0] == "cout"]
        # The default for each prompt, in inverse at column 25.
        self.assertEqual(couts[0], ("cout", b"DEFAULT = ", 25, 0x3F))
        # Slot, drive, and position, in the order they're kept.
        self.assertEqual(state["answers"], b"613")
        self.assertEqual((state["slot"], state["drive"]), (0x60, 1))
        text = bytes(e[1] for e in machine.events if e[0] == "char")
        self.assertIn(b"PLEASE RE-INSERT GAME DISKETTE,", text)

    def test_failed_write(self) -> None:
        machine, state = self.other
        self.assertEqual(len(_sectors(machine.events)), 5)
        self.assertEqual(machine.events[-1], ("negated",))
        text = bytes(e[1] for e in machine.events if e[0] == "char")
        self.assertNotIn(b"RE-INSERT", text)
        # The IOB points at the game's drive again.
        self.assertEqual((state["slot"], state["drive"]), (0x60, 1))
        self.assertEqual(state["answers"], b"520")

    @unittest.skipUnless(os.environ.get("ZORK_MAIN_BIN"),
                         "set ZORK_MAIN_BIN to Zork's main.bin")
    def test_matches_original(self) -> None:
        path = os.environ["ZORK_MAIN_BIN"]
        for name, keys, fail, ours in (
            ("same drive", _SAME_DRIVE, None, self.same),
            ("other drive", _OTHER_DRIVE, 5, self.other),
        ):
            with self.subTest(name):
                machine, state = _run_original(path, keys, fail)
                self.assertEqual(ours[0].events, machine.events)
                self.assertEqual(ours[1], state)


if __name__ == "__main__":
    unittest.main()
