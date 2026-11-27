"""Electronic Arts' Ali Baba and the Forty Thieves for the Apple II,
main.asm of https://github.com/XekriRedmane/ali_baba_reveng, and the
C program in examples/ali_baba that translates it.

golden/O.txt compiles each unit with `-S -O`. `TestDisk` checks the
Disk II emulation's conversion of a disk image, which runs everywhere.
The other tests
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
`TestRuns` links the program with the simulator's runtime helpers
(`sim.linked.defsym_args`), and `TestReadmeLink` links it by the
README's command, which takes the helpers from the runtime library,
and plays the game on it.

Each script is a test method of its own, which pytest-xdist can give to
a worker of its own. On the original, the attract mode takes about 9
seconds, the game about 3, the save about 14, the load about 18, of
which 14 go to running the save for the disk it reads, and the forest
and the party about 5 each.

`uv run python -m tests.test_ali_baba coverage` prints, for each script, how
many of the original's routines it executes, and the routines of the
spans the C program replaces that none of the scripts reach.
"""
from __future__ import annotations

import os
import sys
import tempfile
import unittest
from dataclasses import dataclass, field
from pathlib import Path
from tests.cpp_build import needs_c6502cpp

_ROOT = Path(__file__).parent.parent
_DIR = _ROOT / "examples" / "ali_baba"
# The C program's units, from the plan. TestRuns compiles each that
# exists.
_UNITS = [
    "main", "init", "turn", "input", "combat", "ai", "events",
    "entities", "map", "display", "util", "text", "sound", "hrcg",
]
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

# Shopping in the forest. After the new game, Ali Baba walks onto the
# dusty road (I I), right (K K K), up (I), right past the rune (K K K
# K), and down (M M) through the door at row 6, column 18 into the
# forest, at row 1, column 10. Seven J presses take him left to the
# chest of 3 ducats at row 1, column 3 (PICKUP_TREASURE). K K K and M
# take him to the weapon shop at row 2, column 6, whose scene Return
# closes. The shop's menu shows AN IRON SWORD first. Space picks BUY
# SOMETHING ELSE, which shows AN OAK CLUB, and I and Space buy it for
# the 3 ducats (SHOP_BUY_EFFECT_2), after which the menu shows the
# iron sword again. I and Space try to buy it with nothing left, which
# moves on to the club, and M and Space pick NO SALE. J J J J, six M
# presses, and J take him to the chest of 47 ducats at row 8, column
# 1, and I I I and K K to the armor shop at row 5, column 3. The first
# J waits 52 reads, since with the usual 31 a tiger that roams the
# forest catches him on the way. Return closes the scene. I and Space
# try to buy the RITHMIL PLATE ARMOR, which he can't afford, Space
# skips the RITHMIL CHAIN MAIL, and I and Space buy the LEATHER ARMOR
# for 8 ducats, which gives him experience to come (XP_AWARD_4,
# AWARD_XP). M and Space pick NO SALE. Space, I I K, and Space open
# OTHER OPTIONS, M and Space pick DROP GOLD, Space Space press #
# twice, K and Space Space press @ twice, and M and Space drop 32 of
# his 39 ducats (DROP_GOLD_EXEC). The tiger is in the cell above him.
# Space, I, and Space pick ATTACK, and I and Space attack ABOVE
# (ATTACK_UP). By his next turn the tiger has wounded him and moved
# into his cell. Space, I I K, Space, I, and Space pick DROP ARMOR,
# which turns the leather armor's experience into a level
# (FLUSH_PENDING_XP). Space, I, and Space pick ATTACK again, which
# with the tiger in his cell goes straight to it (NEXT_COLOCATED_MOB).
# The run ends before the tiger's turn.
_FOREST = Script(keys=_keys([
    (10, " "), (12, " "), (18, 0x0D), (49, "I"), (80, "I"), (111, "K"),
    (142, "K"), (173, "K"), (204, "I"), (235, "K"), (266, "K"), (297, "K"),
    (328, "K"), (359, "M"), (390, "M"), (421, "J"), (452, "J"), (483, "J"),
    (514, "J"), (545, "J"), (576, "J"), (607, "J"), (638, "K"), (669, "K"),
    (700, "K"), (731, "M"), (762, 0x0D), (793, " "), (824, "I"),
    (855, " "), (886, "I"), (917, " "), (948, "M"), (979, " "),
    (1031, "J"), (1062, "J"), (1093, "J"), (1124, "J"), (1155, "M"),
    (1186, "M"), (1217, "M"), (1248, "M"), (1279, "M"), (1310, "M"),
    (1341, "J"), (1372, "I"), (1403, "I"), (1434, "I"), (1465, "K"),
    (1496, "K"), (1527, 0x0D), (1558, "I"), (1589, " "), (1620, " "),
    (1651, "I"), (1682, " "), (1713, "M"), (1744, " "), (1775, " "),
    (1806, "I"), (1837, "I"), (1868, "K"), (1899, " "), (1930, "M"),
    (1961, " "), (1992, " "), (2023, " "), (2054, "K"), (2085, " "),
    (2116, " "), (2147, "M"), (2178, " "), (2209, " "), (2240, "I"),
    (2271, " "), (2302, "I"), (2333, " "), (2364, " "), (2395, "I"),
    (2426, "I"), (2457, "K"), (2488, " "), (2519, "I"), (2550, " "),
    (2581, " "), (2612, "I"), (2643, " "),
]), end=2674, events=100842)

# A second character. At the start, M and K move to #, and Space
# raises the slot number to 2 (SLOT_INC_ENTER). K and Space press @,
# which lowers it to 1 again, and I and Return pick BEGIN NEW GAME.
# Ali Baba walks onto the dusty road (I I) and right (K K K) to beside
# the rune at row 5, column 15. Return, I, and Space pick ATTACK, and
# K and Space attack the cell to his RIGHT. The rune is an entry of
# the location's event list, so PLAYER_ATTACK runs and takes its
# branch for a target that can't be hurt. Return, J I, and Space open
# OTHER OPTIONS, and J and Space pick ADD SOMEONE, whose menu shows
# ABOU HASSAN (CHAR_SELECT_INIT). K and Space press #, which shows
# LUTHIEN, and K and Space press @, which shows ABOU HASSAN again. M
# and Space pick TO THIS ROOM, M and Space TO ALI BABA'S HOME, and
# Space ADD, and ABOU HASSAN joins (INIT_CHAR_SELECT). I and Space
# pick MOVE. Return, J I, Space, K, and Space open OTHER OPT, the game
# settings, and M and Space pick SAVE GAME. K and Space press #, K and
# Space press @ (SLOT_INC_REENTER, SLOT_DEC_REENTER), and K and Space
# save in slot 1. It's on a scenario disk, so the scene that explains
# them plays (four Spaces). M and Space pick FORMAT AND USE NEW
# SCENARIO DISK with the game disk still in the drive, so the game
# prints INSERT SCENARIO DISK and shows the choices again
# (SCENARIO_INSERT_PROMPT), and I and Space pick CANCEL REQUEST.
# Return, J I, Space, K M, and Space pick RETIRE, and Ali Baba retires
# (RETIRE_CHARACTER). The first K ends the scene, and Abou Hassan
# walks right (K K K) onto the rune, whose scene plays (two Spaces)
# and which hurts him (TRIGGER_SCENE_EVENT). M walks him into the wall
# below, which stops him (SHOW_LOCKED_MSG).
_PARTY = Script(keys=_keys([
    (10, " "), (12, " "), (43, "M"), (74, "K"), (105, " "), (136, "K"),
    (167, " "), (198, "I"), (229, 0x0D), (260, "I"), (291, "I"),
    (322, "K"), (353, "K"), (384, "K"), (415, 0x0D), (446, "I"),
    (477, " "), (508, "K"), (539, " "), (570, 0x0D), (601, "J"),
    (632, "I"), (663, " "), (694, "J"), (725, " "), (756, "K"), (787, " "),
    (818, "K"), (849, " "), (880, "M"), (911, " "), (942, "M"), (973, " "),
    (1004, " "), (1035, "I"), (1066, " "), (1097, 0x0D), (1128, "J"),
    (1159, "I"), (1190, " "), (1221, "K"), (1252, " "), (1283, "M"),
    (1314, " "), (1345, "K"), (1376, " "), (1407, "K"), (1438, " "),
    (1469, "K"), (1500, " "), (1531, " "), (1562, " "), (1593, " "),
    (1624, " "), (1655, "M"), (1686, " "), (1717, "I"), (1748, " "),
    (1779, 0x0D), (1810, "J"), (1841, "I"), (1872, " "), (1903, "K"),
    (1934, "M"), (1965, " "), (1996, "K"), (2027, "K"), (2058, "K"),
    (2089, "K"), (2120, " "), (2151, " "), (2182, "M"),
]), end=2213, events=103140)

_SCRIPTS = {"attract": _ATTRACT, "game": _GAME, "save": _SAVE,
            "load": _LOAD, "forest": _FOREST, "party": _PARTY}


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

    def test_forest(self) -> None:
        machine = self._run(_FOREST)
        self._reached(machine, "PICKUP_TREASURE", "DISPLAY_SHOP",
                      "SHOP_BUY_HANDLER", "SHOP_BUY_EFFECT_2",
                      "XP_AWARD_4", "AWARD_XP", "DROP_GOLD_EXEC",
                      "FLUSH_PENDING_XP", "ATTACK_UP", "FIND_ATTACK_TARGET",
                      "NEXT_COLOCATED_MOB")

    def test_party(self) -> None:
        machine = self._run(_PARTY)
        # The save is cancelled, so nothing is written.
        self.assertEqual([r for r in _disk_reads(machine) if r[0] == 2],
                         [])
        self._reached(machine, "SLOT_INC_ENTER", "SLOT_DEC_ENTER",
                      "PLAYER_ATTACK", "CHAR_SELECT_INIT", "CHAR_SELECT_NEXT",
                      "CHAR_SELECT_PREV", "CHAR_SELECT_HERE",
                      "CHAR_SELECT_HOME", "INIT_CHAR_SELECT",
                      "SLOT_INC_REENTER", "SLOT_DEC_REENTER",
                      "SCENARIO_INSERT_PROMPT", "CANCEL_SCENARIO",
                      "RETIRE_CHARACTER", "TRIGGER_SCENE_EVENT",
                      "SHOW_LOCKED_MSG")


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


def _build(tmp: str, defsyms: bool = True) -> Path:
    """Compiles each unit to an object and links them with the
    example's linker script. Returns the executable's path. With
    `defsyms`, the link gives the simulator's runtime helpers'
    addresses (`sim.linked.defsym_args`). Without it, the link is the
    README's, and the program holds the helpers it calls from the
    runtime library."""
    from compile import main as compile_main
    from sim.linked import defsym_args
    objects = [
        str(_compile(unit, Path(tmp) / f"{unit}.o", "-c"))
        for unit in _UNITS if (_DIR / f"{unit}.c").exists()
    ]
    linked = Path(tmp) / "ali_baba"
    assert compile_main([
        "compile.py", "-T", str(_DIR / "ali_baba.ld"), *objects,
        *(defsym_args() if defsyms else []), "-o", str(linked),
    ]) == 0
    return linked


def _function_names(exe) -> dict[int, str]:
    from binutils import elf as E
    return {
        s.value: s.name for s in exe.symbols
        if s.defined and s.type == E.STT_FUNC
    }


@needs_c6502cpp
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

    def test_forest(self) -> None:
        self._compare(_FOREST)

    def test_party(self) -> None:
        self._compare(_PARTY)


@needs_c6502cpp
@unittest.skipUnless(
    _AB_DIR, "set ALI_BABA_DIR to the ali_baba_reveng directory")
@unittest.skipUnless((_DIR / "ali_baba.ld").exists(),
                     "examples/ali_baba has no linker script yet")
class TestReadmeLink(unittest.TestCase):
    """The program linked by the README's command, with no `--defsym`,
    so that it holds every runtime helper it calls from the runtime
    library, fits the script's regions and plays a game. The link
    fails when a region overflows, so the build checks the fit."""

    @classmethod
    def setUpClass(cls) -> None:
        from binutils import elf as E
        with tempfile.TemporaryDirectory() as tmp:
            cls.exe = E.read_file(_build(tmp, defsyms=False))
        cls.functions = _function_names(cls.exe)

    def test_helpers_are_its_own(self) -> None:
        from binutils import elf as E
        from cpp_backend import helper_names
        helpers = helper_names()
        absolute = [
            s.name for s in self.exe.symbols
            if s.name in helpers and s.section == E.SHN_ABS
        ]
        self.assertEqual(absolute, [])

    def test_game(self) -> None:
        TestRuns._compare(self, _GAME)


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
