"""The multi-unit example in examples/lode_runner: Broderbund's Lode
Runner for the Apple II, main.asm of
https://github.com/XekriRedmane/lode_runner_reveng, in twelve C files
linked with a linker script.

`TestGoldOutputs` checks each unit's `-O` output against the
checked-in `.asm` file.

`TestRuns` plays the game on the C program and on the original, and
compares the two. It runs only with the reverse-engineering project's
files, which aren't part of this repository. Set LODE_RUNNER_DIR to
the project's directory, which holds `main.bin` (the original's 48K
memory image, $0000-$BFFF), `lode_runner.do` (the game's disk, in DOS
order), and `main.asm` (for naming the original's routines when the
two differ). A run takes from about 10 seconds (the editor in the
original) to about 3 minutes (the demo in the C program), so each
script is a test method of its own, which pytest-xdist can give to a
worker of its own.

Each program runs on the stand-in Apple II in `sim/lode_runner.py`,
which records every soft-switch access and RWTS call with the state of
the two high-resolution pages. The original starts at MAIN. The C
program is loaded over the same image with the original's code
cleared and starts at `main`. The two streams of events must be the
same, up to the point where the original reaches the end of the
script's run, and the two disks must end the same.
"""
from __future__ import annotations

import os
import tempfile
import unittest
from pathlib import Path
from tests.cpp_build import needs_c6502cpp

_DIR = Path(__file__).parent.parent / "examples" / "lode_runner"
_UNITS = [
    "main", "level", "player", "input", "guards", "timers", "screen",
    "hiscore", "editor", "sprites", "sound", "wipe",
]
_LR_DIR = os.environ.get("LODE_RUNNER_DIR")

# GAME_MODE's address, which both programs keep.
_GAME_MODE = 0xA7


def _compile(unit: str, out: Path, stage: str) -> Path:
    from compile import main as compile_main
    assert compile_main([
        "compile.py", str(_DIR / f"{unit}.c"), stage, "-O",
        "-o", str(out),
    ]) == 0
    return out


def _build(tmp: str) -> Path:
    """Compiles each unit to an object and links them with the
    example's linker script. Returns the executable's path."""
    from compile import main as compile_main
    objects = [
        str(_compile(unit, Path(tmp) / f"{unit}.o", "-c"))
        for unit in _UNITS
    ]
    linked = Path(tmp) / "lode_runner"
    assert compile_main([
        "compile.py", "-T", str(_DIR / "lode_runner.ld"), *objects,
        "-o", str(linked),
    ]) == 0
    return linked


@needs_c6502cpp
class TestGoldOutputs(unittest.TestCase):
    """Each unit's output matches the checked-in file. To bless a new
    one, run `uv run python compile.py examples/lode_runner/<unit>.c -S
    -O -o examples/lode_runner/<unit>.asm`."""

    def test_units(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            for unit in _UNITS:
                with self.subTest(unit=unit):
                    path = _compile(unit, Path(tmp) / f"{unit}.asm", "-S")
                    self.assertEqual(
                        path.read_text(), (_DIR / f"{unit}.asm").read_text(),
                    )


# ---- The scripts ------------------------------------------------------------
#
# A script is the keys, as (n, key) pairs that press `key` just before
# the program's KBD read number n, and the GAME_MODE change that ends
# the run. The read numbers were found by running the original.

# The attract mode. Nothing is pressed. MAIN waits a moment and starts
# the demo, which plays its three levels until the player dies on the
# third, and then the high scores show (GAME_MODE 2).
_DEMO = ([], (1, 2))

# A game. Space at the title starts it. The first key of a game goes
# to the wait for a key with the player blinking, and the joystick
# mode the image starts in lets the missing joystick start the level
# first, so Ctrl-K (keyboard mode) at the game's first read of KBD
# comes next. In the first life the player runs right, picks up a
# piece of gold, and is caught at the ladder on the right. In the
# second he runs left and digs a hole to his right, which a guard
# falls into, and is caught later. Ctrl-A ends each of the last three
# lives at its blinking player. The score, 325, goes in the high-score
# table as ABC, after which the high scores show until the time runs
# out and the splash screen draws again (GAME_MODE 0).
_GAME = ([
    (5, ord(" ")), (8, 0x0B), (9, ord("L")),
    (300, ord("J")), (445, ord("O")),
    (692, 0x01), (775, 0x01), (858, 0x01),
    (900, ord("A")), (941, ord("B")), (982, ord("C")), (1023, 0x0D),
], (2, 0))

# The editor. Ctrl-E at the title starts it. E and Return edit level
# 1, where K K K moves the cursor right, 7 puts gold there, M moves
# down, 1 puts a brick there, and Ctrl-S saves the level. K, 8 (a
# guard), and Ctrl-Q quit without saving, so the editor prints its
# prompt to save the level, and N declines. C 0 0 2 Return clears level 2. P 0 0 1 Return plays level
# 1. The input mode is joystick again there, so the missing joystick
# starts the level, and Ctrl-R in the game (one life left, and dead)
# ends it, with no high score. The time after GAME OVER runs out, and
# the splash screen draws again (GAME_MODE 0).
_EDITOR = ([
    (5, 0x05), (26, ord("E")), (47, 0x0D), (68, ord("K")), (89, ord("K")),
    (110, ord("K")), (131, ord("7")), (152, ord("M")), (173, ord("1")),
    (194, 0x13), (215, ord("K")), (236, ord("8")), (257, 0x11),
    (278, ord("N")), (299, ord("C")), (320, ord("0")), (341, ord("0")),
    (362, ord("2")), (383, 0x0D), (404, ord("P")), (425, ord("0")),
    (446, ord("0")), (467, ord("1")), (488, 0x0D), (521, 0x12),
], (3, 0))


class _ModeChange:
    """True once GAME_MODE is `after`, having been `before`."""

    def __init__(self, before: int, after: int) -> None:
        self.before, self.after = before, after
        self.seen = False

    def __call__(self, machine) -> bool:
        mode = machine.mpu.memory[_GAME_MODE]
        if mode == self.before:
            self.seen = True
        return self.seen and mode == self.after


def _disk(path: Path, variant: str) -> bytes:
    """The game's disk. The master disk's high-score table holds
    scores from level 16 up, which no scripted game reaches, so the
    game runs on a copy with the table cleared. On the master disk the
    editor runs DONT_MANIPULATE_MASTER_DISK in place of each edit, so
    it runs on a copy that's a data disk, with the high-score sector's
    master flag cleared. Its table is kept, so that the game it plays gets no
    high score."""
    from sim.apple2 import dsk_sector
    disk = bytearray(path.read_bytes())
    at = dsk_sector(disk, 12, 15)
    if variant == "game":
        disk[at:at + 80] = bytes(80)
    if variant == "editor":
        disk[at + 255] = 0
    return bytes(disk)


def _function_names(exe) -> dict[int, str]:
    from binutils import elf as E
    return {
        s.value: s.name for s in exe.symbols
        if s.defined and s.type == E.STT_FUNC
    }


@needs_c6502cpp
@unittest.skipUnless(
    _LR_DIR, "set LODE_RUNNER_DIR to the lode_runner_reveng directory")
class TestRuns(unittest.TestCase):

    @classmethod
    def setUpClass(cls) -> None:
        from binutils import elf as E
        from sim.lode_runner import original_routines
        root = Path(_LR_DIR)
        cls.image = (root / "main.bin").read_bytes()
        cls.disk_path = root / "lode_runner.do"
        cls.routines = original_routines(
            (root / "main.asm").read_text(errors="replace"))
        with tempfile.TemporaryDirectory() as tmp:
            cls.exe = E.read_file(_build(tmp))
        cls.functions = _function_names(cls.exe)

    def _compare(self, script, variant: str, max_cycles: int):
        from sim.lode_runner import (
            first_difference, original_machine, program_machine,
            routine_at,
        )
        keys, (before, after) = script
        disk = _disk(self.disk_path, variant)
        original = original_machine(self.image, disk, keys)
        why = original.run(max_cycles, _ModeChange(before, after))
        self.assertEqual(why, "stop")
        n = len(original.events)
        ours = program_machine(self.image, self.exe, disk, keys)
        ours.run(ours.mpu.processorCycles + 2 * max_cycles,
                 lambda m: len(m.events) >= n)
        a, b = original.events, ours.events
        i = first_difference(a, b)
        if i is None and len(b) < n:
            i = len(b)
        if i is not None:
            lines = [f"the programs differ at event {i} of {n}"]
            for j in range(max(0, i - 3), i + 1):
                if j < len(a):
                    lines.append("  original: " + a.describe(j) + " in "
                                 + routine_at(self.routines, a.pc[j]))
                if j < len(b):
                    lines.append("  program:  " + b.describe(j) + " in "
                                 + routine_at(self.functions, b.pc[j]))
            self.fail("\n".join(lines))
        self.assertEqual(ours.disk, original.disk)
        return original, ours

    def test_demo(self) -> None:
        original, _ = self._compare(_DEMO, "master", 400_000_000)
        # The demo plays all three of its levels.
        self.assertEqual(original.mpu.memory[0xA6], 3)

    def test_game(self) -> None:
        original, _ = self._compare(_GAME, "game", 200_000_000)
        # The high score was written, with the initials.
        writes = [r for r in original.events.rwts.values() if r[0] == 2]
        self.assertEqual(len(writes), 1)
        self.assertEqual(writes[0][3][:3], b"\xC1\xC2\xC3")

    def test_editor(self) -> None:
        original, _ = self._compare(_EDITOR, "editor", 100_000_000)
        # The edited level 1 and the cleared level 2 were written.
        writes = [r[1:3] for r in original.events.rwts.values()
                  if r[0] == 2]
        self.assertEqual(writes, [(3, 0), (3, 1)])


if __name__ == "__main__":
    unittest.main()
