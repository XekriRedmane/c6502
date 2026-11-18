"""The multi-unit example in examples/zork: the whole of
Zork I's (r15) interpreter, output/main.asm of
https://github.com/XekriRedmane/zorki-r15-reveng, in eleven C files
linked with a linker script.

`TestGoldOutputs` checks each unit's `-O` output and the listing of
the linked program (`objdump.py -D`) against the checked-in `.asm`
files.

`TestSessions` plays Zork on the interpreter, and on Zork's own, and
compares the two. It runs only with the game's disk image, which isn't
part of this repository. Set ZORK_DSK to the path of `Zork I r15 (4am
crack).dsk` from the repository above. Zork's interpreter is read from
the same image (tracks 1 and 2), and the story file from track 3 on.
Each session runs on the stand-in Apple II in `sim/apple2.py`, with
the game diskette in slot 6, drive 1, and an empty save diskette in
drive 2. The two programs must make the same calls to the Monitor and
to RWTS, with the same characters and cursor, write the same sectors,
and leave the same screen and printer output.
"""
from __future__ import annotations

import os
import shutil
import tempfile
import unittest
from pathlib import Path

from py65.devices.mpu6502 import MPU

_DIR = Path(__file__).parent.parent / "examples" / "zork"
_UNITS = [
    "main", "decode", "objects", "numbers", "input", "pages", "zstring",
    "output", "disk", "prompts", "save",
]
_HAVE_PCPP = shutil.which("pcpp") is not None
_DSK = os.environ.get("ZORK_DSK")

# Zork's boot loader reads 26 sectors from track 1 into $0800.
_MAIN_FIRST_SECTOR, _MAIN_SECTORS = 16, 26

# The first session walks into the cellar, saves, fights the troll,
# restores, tries words that aren't in the dictionary and sentences the
# parser rejects, and quits. Somewhere in the fight the troll wins, and Zork
# moves the player back to the forest with random numbers.
_WALK = [
    "open mailbox", "read leaflet", "drop leaflet", "north", "north",
    "climb tree", "take egg", "down", "south", "east", "open window",
    "enter house", "take sack", "open it", "take all", "inventory",
    "west", "take lamp", "take sword", "move rug", "open trap door",
    "turn on lamp", "down", "score", "save", "south", "east",
    "take painting", "west", "north", "diagnose", "look",
    "examine troll", "kill troll with sword", "kill troll with sword",
    "kill troll with sword", "kill troll with sword",
    "kill troll with sword", "restore", "look", "xyzzy", "plugh",
    "take the frobozz", "go north, then west.", "what is a grue?",
    "brief", "verbose", "superbrief", "south", "east", "again", "wait",
    "wait", "score", "quit", "y",
]

# The second turns the transcript on, saves at position 3 by answering
# the prompts, fails to restore from the empty position 5, restores
# from 3, reads the leaflet twice in one line so that the screen fills
# and Zork stops for [MORE], turns the transcript off, and restarts.
_PRINTER = [
    "script", "open mailbox", "take leaflet", "save", "north", "restore",
    "restore", "look", "read leaflet. read leaflet. inventory",
    "unscript", "restart", "y", "look", "quit", "y",
]
_PRINTER_KEYS = [ord(c) for c in "362\r" "562\r" "362\r"]


def _compile(unit: str, out: Path) -> Path:
    """Compiles the unit as its README does: to its assembly at `out`,
    which the test compares with the checked-in file, and to an object
    beside it, which it returns for the link."""
    from compile import main as compile_main
    obj = out.with_suffix(".o")
    for stage, path in (("-S", out), ("-c", obj)):
        assert compile_main([
            "compile.py", str(_DIR / f"{unit}.c"), stage, "-O",
            "-o", str(path),
        ]) == 0
    return obj


def _link(paths: list[Path], out: Path) -> None:
    from compile import main as compile_main
    assert compile_main([
        "compile.py", "-T", str(_DIR / "interpreter.ld"),
        *map(str, paths), "-o", str(out),
    ]) == 0


def _build(tmp: str) -> tuple[list[Path], Path]:
    paths = []
    for unit in _UNITS:
        path = Path(tmp) / f"{unit}.asm"
        paths.append(_compile(unit, path))
    linked = Path(tmp) / "interpreter"
    _link(paths, linked)
    return paths, linked


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestGoldOutputs(unittest.TestCase):
    """Each unit's output and the linked program match the checked-in
    files. To bless new ones, run the commands in the example's
    README."""

    def test_units_and_link(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            paths, linked = _build(tmp)
            for unit, path in zip(_UNITS, paths):
                with self.subTest(unit=unit):
                    self.assertEqual(
                        path.with_suffix(".asm").read_text(),
                        (_DIR / f"{unit}.asm").read_text(),
                    )
            from tests.link_helpers import listing
            self.assertEqual(
                listing(linked),
                (_DIR / "interpreter.asm").read_text(),
            )

    def test_no_runtime_helpers(self) -> None:
        # The program runs on Zork's own machine, which has none of
        # the runtime's helpers and nothing that sets up its soft
        # stack, so it calls neither.
        text = (_DIR / "interpreter.asm").read_text()
        self.assertNotRegex(
            text, r"JSR\s+((mul|[us]divmod|asl|asr|lsr)(8|16|32|64)|icall)\b",
        )
        self.assertNotRegex(text, r"\(SSP\)|\(FP\)")


def _disk() -> bytes:
    return Path(_DSK).read_bytes()


def _zork_main(dsk: bytes) -> bytes:
    from sim.apple2 import dsk_sector
    out = bytearray()
    for s in range(_MAIN_FIRST_SECTOR, _MAIN_FIRST_SECTOR + _MAIN_SECTORS):
        at = dsk_sector(dsk, s // 16, s % 16)
        out += dsk[at:at + 256]
    return bytes(out)


def _machine(mpu: MPU, dsk: bytes, lines, keys):
    """The stand-in Apple II as Zork's boot loader leaves it: the
    screen set up, DEBUG_JUMP an RTS, and SECTORS_PER_TRACK 16."""
    from sim.apple2 import Apple2
    a = Apple2(mpu, keys=list(keys), lines=list(lines))
    a.disks[(0x60, 1)] = bytearray(dsk)
    a.disks[(0x60, 2)] = bytearray(len(dsk))
    a.boot_screen()
    mpu.memory[0x7C] = 0x60
    mpu.memory[0x7F] = 16
    return a


def _run_original(dsk: bytes, lines, keys=()):
    mpu = MPU()
    binary = _zork_main(dsk)
    mpu.memory[0x0800:0x0800 + len(binary)] = binary
    mpu.pc = 0x0800
    mpu.sp = 0xFF
    a = _machine(mpu, dsk, lines, keys)
    a.run()
    return a


def _run_c(text: str, dsk: bytes, lines, keys=()):
    from sim.linked import build_linked_sim
    sim = build_linked_sim(text.exe)
    a = _machine(sim.mpu, dsk, lines, keys)
    a.run(other_hooks=dict(sim.runtime.hooks))
    return a


@unittest.skipUnless(_HAVE_PCPP, "pcpp not available")
@unittest.skipUnless(_DSK, "set ZORK_DSK to Zork I r15's disk image")
class TestSessions(unittest.TestCase):

    @classmethod
    def setUpClass(cls) -> None:
        cls.dsk = _disk()
        with tempfile.TemporaryDirectory() as tmp:
            _, linked = _build(tmp)
            from tests.link_helpers import linked_text
            cls.text = linked_text(linked)

    def _compare(self, lines, keys=()):
        original = _run_original(self.dsk, lines, keys)
        ours = _run_c(self.text, self.dsk, lines, keys)
        self.assertEqual(original.stop_reason, "halt")
        self.assertEqual(ours.stop_reason, "halt")
        self.assertEqual(ours.calls, original.calls)
        self.assertEqual(ours.written, original.written)
        self.assertEqual(ours.printer, original.printer)
        self.assertEqual(ours.text(), original.text())
        return original, ours

    def test_walk(self) -> None:
        _, ours = self._compare(_WALK)
        screen = ours.text()
        self.assertIn("-- END OF SESSION --", screen)
        self.assertIn("RANK OF AMATEUR", screen)
        # A save writes the state, the stack, and the dynamic memory.
        self.assertGreater(len(ours.written), 3)

    def test_transcript_save_and_restart(self) -> None:
        _, ours = self._compare(_PRINTER, _PRINTER_KEYS)
        printed = "".join(
            chr(c & 0x7F) for k in ours.calls
            if k[0] == "COUT1" for c in [k[1]]
        )
        self.assertIn("[MORE]", printed)
        self.assertIn("POSITION (0-7):", printed)
        # Position 3 starts 3 * 64 sectors in, on track 15.
        self.assertEqual(ours.written[0][:3], (0x60, 2, 15))
        transcript = bytes(c & 0x7F for c in ours.printer)
        self.assertIn(b">NORTH", transcript)
        self.assertNotIn(b"[MORE]", transcript)


if __name__ == "__main__":
    unittest.main()
