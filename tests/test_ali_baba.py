"""Electronic Arts' Ali Baba and the Forty Thieves for the Apple II,
main.asm of https://github.com/XekriRedmane/ali_baba_reveng, and the
C program in examples/ali_baba that translates it.

`TestGoldOutputs` compiles each unit and compares it with its
checked-in `.asm` file, which runs everywhere, and `TestDisk` checks
the Disk II emulation's conversion of a disk image. The other tests
run only with the reverse-engineering project's files, which aren't
part of this repository. Set ALI_BABA_DIR to the project's
directory, which holds `output/main.bin` (the original's memory image,
$0500-$BFFF), `output/main.asm` and `output/main.sym` (for naming the
original's routines), and the game's disk, `Ali Baba and the Forty
Thieves (4am and san inc crack).dsk`.

Each program runs on the stand-in Apple II in `sim/ali_baba.py`, which
records every soft-switch access, Monitor routine call, and call into
the kept disk code, with a CRC of hi-res page 1. `TestOriginal` plays
each script on the original and checks that it reaches what the script
is for. `TestRuns` plays each on the C program as well and compares the
two: the streams of events must be the same up to the event at which
the original's run ends, and so must the disks the scripts write.

Each script is a test method of its own, which pytest-xdist can give to
a worker of its own. On the original, the attract mode takes about 9
seconds, the game about 3, the save about 14, and the load about 18,
of which 14 go to running the save for the disk it reads.

`uv run python -m tests.test_ali_baba coverage` prints, for each script, how
many of the original's routines it executes, and the routines of the
spans the C program replaces that none of the scripts reach.
"""
from __future__ import annotations

import os
import shutil
import sys
import tempfile
import unittest
from dataclasses import dataclass, field
from pathlib import Path

_ROOT = Path(__file__).parent.parent
_DIR = _ROOT / "examples" / "ali_baba"
# The C program's units, from the plan. TestRuns compiles each that
# exists.
_UNITS = [
    "main", "init", "turn", "input", "combat", "ai", "events",
    "entities", "map", "display", "util", "text", "sound", "hrcg",
]
_HAVE_PCPP = shutil.which("pcpp") is not None
_AB_DIR = os.environ.get("ALI_BABA_DIR")
_DISK = "Ali Baba and the Forty Thieves (4am and san inc crack).dsk"


# ---- The scripts ------------------------------------------------------------
#
# A script is the keys, as (n, key) pairs that press `key` just before
# the program's KBD read number n; the disk swaps, as (n, name) pairs
# that put a disk in drive 1 just before KBD read n; and the KBD read
# at which the run ends. The read numbers were found by running the
# original, pressing each key a set number of reads after the program
# cleared the strobe on the one before. Reads by the kept disk code
# don't count. `events` is the number of events the original makes.

@dataclass(frozen=True)
class Script:
    keys: list[tuple[int, int]]
    end: int
    events: int
    swaps: list[tuple[int, str]] = field(default_factory=list)
    max_cycles: int = 4_000_000_000


def _keys(pairs: list[tuple[int, str | int]]) -> list[tuple[int, int]]:
    return [(n, ord(k) if isinstance(k, str) else k) for n, k in pairs]


# The attract mode. Nothing is pressed. The title screen draws, and its
# tune plays over and over while TUNE_ADVANCE polls the keyboard
# between notes.
_ATTRACT = Script(keys=[], end=1500, events=582113)

# A game. The two Spaces end the title tune and pick the keyboard at
# GAME_INIT's prompt (SELECT_INPUT_MODE), and Return picks BEGIN NEW
# GAME. Ali Baba walks up out of his home onto the dusty road (I I) and
# right (K K K), to beside the rune at row 5, column 15, without
# stepping on it. The J and I presses that follow leave him where he is
# (the screen at reads 497 to 691 shows him there). Return, K, and
# Return open SEE YOUR LOT, and M closes it. In the turns that the
# Returns and J presses after it pass, a hostile group attacks him
# (MOB_ATTACK, RESOLVE_ATTACK, and APPLY_DAMAGE run) and kills him, and
# the reincarnation scene, the run's one RWTS read (track $13, sector
# $C), plays. He's back in his home for the last presses.
_GAME = Script(keys=_keys([
    (10, " "), (12, " "), (18, 0x0D), (49, "I"), (80, "I"), (141, "K"),
    (202, "K"), (263, "K"), (284, " "), (345, "J"), (406, "J"),
    (467, "J"), (498, "I"), (529, "I"), (560, "I"), (591, "I"),
    (692, " "), (723, 0x0D), (754, "K"), (785, 0x0D), (846, "M"),
    (877, 0x0D), (908, 0x0D), (939, "J"), (970, 0x0D), (1001, 0x0D),
    (1032, "J"), (1063, 0x0D), (1094, 0x0D), (1125, "J"), (1156, 0x0D),
    (1187, "I"), (1218, "I"), (1279, "J"), (1340, "J"), (1401, "J"),
    (1462, "J"), (1523, "J"), (1584, " "), (1645, " "), (1706, " "),
]), end=1800, events=46528)

# A saved game. After the new game, Return opens the menu at DEFEND, J
# and I move to OTHER OPTIONS, and Return opens it; K and Return open
# OTHER OPT, the game settings. There K Return raises the game speed, K
# Return lowers it, I Return lowers the danger, J Return raises it, J
# Return turns the sound off and I Return on again (each sound toggle
# leaves MOVE selected). M and Return pick SAVE GAME, and Return saves
# in slot 1, which is on a scenario disk, so the scene that explains
# them plays (four Spaces). An unformatted disk goes in, M and Return
# pick FORMAT AND USE NEW SCENARIO DISK, and the game formats it (RWTS
# command 4), writes "ALI" to track 0 sector 0 and the game's records
# ($4000-$59FF) to tracks 1 and 2, and asks for the game disk, which
# goes back in before the Space. Return, K, Return, and M then open and
# close SEE YOUR LOT.
_SAVE = Script(keys=_keys([
    (10, " "), (12, " "), (18, 0x0D), (49, 0x0D), (80, "J"), (111, "I"),
    (142, 0x0D), (173, "K"), (204, 0x0D), (235, "K"), (266, 0x0D),
    (297, "K"), (328, 0x0D), (359, "I"), (390, 0x0D), (421, "J"),
    (452, 0x0D), (483, "J"), (514, 0x0D), (545, "I"), (576, 0x0D),
    (607, "M"), (638, 0x0D), (669, 0x0D), (673, " "), (677, " "),
    (681, " "), (685, " "), (696, "M"), (707, 0x0D), (738, " "),
    (799, 0x0D), (830, "K"), (861, 0x0D), (922, "M"),
]), swaps=[(691, "scenario"), (738, "game")], end=1000, events=17260)

# A game loaded from the scenario disk that the save script writes on
# the original. M and Return pick CONTINUE GAME NUMBER 1 at the start,
# the scenario disk goes in before the Space that answers INSERT
# SCENARIO DISK, the game reads its 26 sectors, and the game disk goes
# back in before the Space that answers INSERT ALI BABA DISK. Return,
# K, Return, and M open and close SEE YOUR LOT.
_LOAD = Script(keys=_keys([
    (10, " "), (12, " "), (18, "M"), (49, 0x0D), (70, " "), (101, " "),
    (162, 0x0D), (193, "K"), (224, 0x0D), (285, "M"),
]), swaps=[(70, "scenario"), (101, "game")], end=350, events=9414)

_SCRIPTS = {"attract": _ATTRACT, "game": _GAME, "save": _SAVE,
            "load": _LOAD}


# ---- Running them -------------------------------------------------------------

class _Files:
    """The reverse-engineering project's files, read once."""

    def __init__(self, root: Path) -> None:
        from sim.ali_baba import original_labels
        out = root / "output"
        self.main_bin = (out / "main.bin").read_bytes()
        self.disk = (root / _DISK).read_bytes()
        self.labels = original_labels(
            (out / "main.asm").read_text(errors="replace"),
            (out / "main.sym").read_text(errors="replace"))
        self.routines = {a: n for a, (n, _) in self.labels.items()}
        self._scenario: bytes | None = None

    def disks(self, script: Script) -> dict[str, bytes | None]:
        """The disks a script swaps in besides the game's: an
        unformatted one for the save, and for the load, the one the
        save script writes on the original."""
        if script is _SAVE:
            return {"scenario": None}
        if script is _LOAD:
            return {"scenario": self.scenario()}
        return {}

    def scenario(self) -> bytes:
        if self._scenario is None:
            machine = self.run_original(_SAVE)
            self._scenario = bytes(machine.media["scenario"].to_dsk())
        return self._scenario

    def run_original(self, script: Script):
        from sim.ali_baba import original_machine
        machine = original_machine(
            self.main_bin, self.disk, script.keys, script.swaps,
            self.disks(script))
        why = machine.run(script.max_cycles,
                          lambda m: m.kbd_reads >= script.end)
        if why != "stop":
            raise AssertionError(f"the original's run ended with {why}")
        return machine


_files: _Files | None = None


def _project() -> _Files:
    global _files
    if _files is None:
        _files = _Files(Path(_AB_DIR))
    return _files


def _disk_reads(machine) -> list[tuple[int, int, int]]:
    """The (command, track, sector) of each RWTS call of a run."""
    return [(iob[12], iob[4], iob[5])
            for entry, _, _, iob, _ in machine.events.disk.values()
            if iob]


@unittest.skipUnless(
    _AB_DIR, "set ALI_BABA_DIR to the ali_baba_reveng directory")
class TestOriginal(unittest.TestCase):
    """Each script on the original reaches what it's for, with the
    number of events it made when the script was written."""

    def _run(self, script: Script):
        machine = _project().run_original(script)
        self.assertEqual(len(machine.events), script.events)
        return machine

    def _reached(self, machine, *names: str) -> None:
        from sim.ali_baba import coverage
        cov = coverage(machine.seen, _project().labels)
        missed = [n for n in names if n not in cov.reached]
        self.assertEqual(missed, [])

    def test_attract(self) -> None:
        machine = self._run(_ATTRACT)
        counts = machine.events.counts()
        # The tune plays: the speaker clicks, and the only key polling
        # is the tune's.
        self.assertGreater(counts["$C030 read"], 100_000)
        self.assertEqual(counts["$C000 read"], _ATTRACT.end)
        self._reached(machine, "TUNE_ADVANCE", "PLAY_HIGH_TONE",
                      "PLAY_LOW_TONE", "HRCG_COUT")

    def test_game(self) -> None:
        machine = self._run(_GAME)
        # The reincarnation scene comes from the disk.
        self.assertIn((1, 0x13, 0xC), _disk_reads(machine))
        self._reached(machine, "GAME_TURN_LOOP", "SCENE_LOOP",
                      "TEXT_RENDERER", "DISPLAY_CHAR_STATS",
                      "HANDLE_ENCOUNTER", "NPC_MOVE_AI")

    def test_save(self) -> None:
        machine = self._run(_SAVE)
        files = _project()
        scenario = machine.media["scenario"].to_dsk()
        # The signature, and the game's records in tracks 1 and 2.
        self.assertEqual(scenario[:3], b"\xC1\xCC\xC9")
        self.assertEqual(scenario[16 * 256:16 * 256 + 256],
                         bytes(machine.mpu.memory[0x4000:0x4100]))
        writes = [r for r in _disk_reads(machine) if r[0] == 2]
        self.assertEqual(len(writes), 27)
        # The game disk is only read.
        self.assertEqual(machine.media["game"].to_dsk(files.disk),
                         files.disk)
        self._reached(machine, "FORMAT_SCENARIO_DISK", "WRITE_GAME_DATA",
                      "TOGGLE_SOUND", "SETTINGS_SPEED_UP")

    def test_load(self) -> None:
        machine = self._run(_LOAD)
        reads = [r for r in _disk_reads(machine) if r[0] == 1]
        self.assertIn((1, 1, 0), reads)
        self.assertIn((1, 2, 12), reads)
        self._reached(machine, "CONTINUE_GAME", "READ_GAME_DATA")


class TestDisk(unittest.TestCase):
    """The nibble-level disk of `sim/disk2.py` gives back the sectors it
    was made from. These need none of the project's files."""

    def test_sector_round_trip(self) -> None:
        import random
        from sim.disk2 import decode_sector, encode_sector
        rnd = random.Random(1)
        for _ in range(50):
            data = bytes(rnd.randrange(256) for _ in range(256))
            nibbles = encode_sector(data)
            self.assertEqual(len(nibbles), 343)
            self.assertTrue(all(n >= 0x96 for n in nibbles))
            self.assertEqual(decode_sector(nibbles), data)

    def test_dsk_round_trip(self) -> None:
        import random
        from sim.disk2 import Drive
        rnd = random.Random(2)
        dsk = bytes(rnd.randrange(256) for _ in range(35 * 16 * 256))
        self.assertEqual(Drive.from_dsk(dsk).to_dsk(), dsk)

    def test_protection_track_sector(self) -> None:
        """A sector of $36 $00 pairs, then $37 $00, then $34 $00 in its
        three thirds (the game's track 5) is 342 nibbles of $AF, which
        VERIFY_DATA_FIELD checks for."""
        from sim.disk2 import encode_sector
        data = bytes([0x36, 0] * 43 + [0x37, 0] * 43 + [0x34, 0] * 42)
        self.assertEqual(encode_sector(data)[:342], b"\xAF" * 342)


# ---- Comparing the C program with the original ------------------------------

def _compile(unit: str, out: Path, stage: str) -> Path:
    from compile import main as compile_main
    assert compile_main([
        "compile.py", str(_DIR / f"{unit}.c"), stage, "-O",
        "-o", str(out),
    ]) == 0
    return out


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestGoldOutputs(unittest.TestCase):
    """Each unit's output matches the checked-in file. To bless a new
    one, run `uv run python compile.py examples/ali_baba/<unit>.c -S
    -O -o examples/ali_baba/<unit>.asm`."""

    def test_units(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            for unit in _UNITS:
                with self.subTest(unit=unit):
                    path = _compile(unit, Path(tmp) / f"{unit}.asm", "-S")
                    self.assertEqual(
                        path.read_text(), (_DIR / f"{unit}.asm").read_text(),
                    )


def _build(tmp: str) -> Path:
    """Compiles each unit to an object and links them with the
    example's linker script. Returns the executable's path."""
    from compile import main as compile_main
    from sim.linked import defsym_args
    objects = [
        str(_compile(unit, Path(tmp) / f"{unit}.o", "-c"))
        for unit in _UNITS if (_DIR / f"{unit}.c").exists()
    ]
    linked = Path(tmp) / "ali_baba"
    assert compile_main([
        "compile.py", "-T", str(_DIR / "ali_baba.ld"), *objects,
        *defsym_args(), "-o", str(linked),
    ]) == 0
    return linked


def _function_names(exe) -> dict[int, str]:
    from binutils import elf as E
    return {
        s.value: s.name for s in exe.symbols
        if s.defined and s.type == E.STT_FUNC
    }


@unittest.skipUnless(_HAVE_PCPP, "pcpp not available")
@unittest.skipUnless(
    _AB_DIR, "set ALI_BABA_DIR to the ali_baba_reveng directory")
@unittest.skipUnless((_DIR / "ali_baba.ld").exists(),
                     "examples/ali_baba has no linker script yet")
class TestRuns(unittest.TestCase):
    """Each script on the C program makes the original's events, and
    leaves the disks as the original does."""

    @classmethod
    def setUpClass(cls) -> None:
        from binutils import elf as E
        with tempfile.TemporaryDirectory() as tmp:
            cls.exe = E.read_file(_build(tmp))
        cls.functions = _function_names(cls.exe)

    def _compare(self, script: Script) -> None:
        from sim.ali_baba import (
            first_difference, link_regions, program_machine, routine_at,
        )
        files = _project()
        original = files.run_original(script)
        n = len(original.events)
        ours = program_machine(
            files.main_bin, self.exe, files.disk, script.keys,
            script.swaps, files.disks(script),
            clear=link_regions((_DIR / "ali_baba.ld").read_text()))
        ours.run(ours.mpu.processorCycles + 2 * script.max_cycles,
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
                                 + routine_at(files.routines, a.pc[j]))
                if j < len(b):
                    lines.append("  program:  " + b.describe(j) + " in "
                                 + routine_at(self.functions, b.pc[j]))
            self.fail("\n".join(lines))
        for name, drive in original.media.items():
            self.assertEqual(ours.media[name].to_dsk(), drive.to_dsk(),
                             f"the {name} disk")

    def test_attract(self) -> None:
        self._compare(_ATTRACT)

    def test_game(self) -> None:
        self._compare(_GAME)

    def test_save(self) -> None:
        self._compare(_SAVE)

    def test_load(self) -> None:
        self._compare(_LOAD)


def _print_coverage() -> None:
    """Prints each script's coverage of the original, and the routines
    of the C program's spans that no script reaches."""
    from sim.ali_baba import CODE_SPANS, coverage
    files = _project()
    seen = bytearray(0x10000)
    for name, script in _SCRIPTS.items():
        machine = files.run_original(script)
        cov = coverage(machine.seen, files.labels, CODE_SPANS)
        print(f"{name}: {len(machine.events)} events, {cov.summary()} "
              "in the C program's spans")
        seen = bytearray(x | y for x, y in zip(seen, machine.seen))
    cov = coverage(bytes(seen), files.labels, CODE_SPANS)
    print(f"all: {cov.summary()}; not reached:")
    print("  " + " ".join(cov.missed))


if __name__ == "__main__":
    if sys.argv[1:] == ["coverage"]:
        _print_coverage()
    else:
        unittest.main()
