"""Compares the games' C++ machines (cpp/src/sim/games.h) with the Python
machines in sim/.

Each run plays one script of one game, on the original or on the C
program. The Python machine writes the run as text, and `c6502cpp
game-run` (cpp/src/sim/game_run.h) writes the same text from the C++
machine: a line per event (a Monitor or RWTS call, for Zork) with its
PC and cycle count, which the tests leave out, and a CRC-32 of the
CPU's registers and cycle count as the event was recorded, then the
machine's state at the end, with a CRC-32 of memory and the disks, the
instructions executed, and the undefined opcodes among them. The two
texts must be equal. At the first line that differs, both sides run
again with a `step` line for each instruction after the last event that
matched, and the first step that differs is reported. The Ali Baba
script `coverage` compares the listing of the routines the scripts
reach with `c6502cpp game-run ali_baba coverage`.

    uv run python tools/compare_game_machines.py [--game G] [--script S]
        [--program original|c|readme] [--out DIR] [-j N]

The runs of the originals come first, since a C program runs until it
has made as many events as the original, and -j runs that many at once.
It needs the C++ build (`C6502_CPP` or cpp/build/bin/c6502cpp) and the
games' files, through ZORK_DSK, LODE_RUNNER_DIR, and ALI_BABA_DIR.
"""
from __future__ import annotations

import argparse
import os
import struct
import subprocess
import sys
import tempfile
import time
import zlib
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path
from typing import NamedTuple

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT))

import cpp_backend  # noqa: E402
from compile import main as compile_main  # noqa: E402

_STATE = struct.Struct("<HBBBBBQ").pack


def _digest(mpu) -> int:
    return zlib.crc32(_STATE(mpu.pc, mpu.a, mpu.x, mpu.y, mpu.sp, mpu.p,
                             mpu.processorCycles))


class _Tracer:
    def __init__(self) -> None:
        self.reset(None, 0)

    def reset(self, steps_after: int | None, steps: int) -> None:
        self.instructions = 0
        self.tracing = False
        self.digests: dict[int, int] = {}
        self.undefined: dict[int, int] = {}
        self.steps_after = steps_after
        self.steps = steps
        self.step_lines: list[str] = []
        self.events = lambda: 0
        self.mpu = None
        self._op = 0

    def event(self, index: int, mpu) -> None:
        if self.tracing:
            self.digests[index] = _digest(mpu)

    def recorder(self, mpu, label: str):
        tracer = self

        class Recorder:
            def before(self) -> None:
                tracer._op = mpu.memory[mpu.pc]

            def after(self) -> None:
                tracer.instructions += 1
                if not tracer.tracing:
                    return
                from binutils.opcodes import DECODE
                op = tracer._op
                if op not in DECODE:
                    tracer.undefined[op] = tracer.undefined.get(op, 0) + 1
                if (tracer.steps_after is not None
                        and tracer.events() > tracer.steps_after
                        and len(tracer.step_lines) < tracer.steps):
                    tracer.step_lines.append(
                        f"step {mpu.pc} {mpu.a} {mpu.x} {mpu.y} {mpu.sp} "
                        f"{mpu.p} {mpu.processorCycles}")

            def finish(self) -> None:
                pass

        return Recorder()

    def digest(self, index: int) -> str:
        d = self.digests.get(index)
        return "-" if d is None else str(d)

    def steps_text(self) -> list[str]:
        undefined = "".join(f" {op}:{n}" for op, n in sorted(self.undefined.items()))
        return self.step_lines + [f"undefined{undefined}"]


TRACER = _Tracer()


def _crc(data) -> int:
    return zlib.crc32(bytes(data))


_UNITS = {
    "zork": ["main", "decode", "objects", "numbers", "input", "pages",
             "zstring", "output", "disk", "prompts", "save"],
    "lode_runner": ["main", "level", "player", "input", "guards", "timers",
                    "screen", "hiscore", "editor", "sprites", "sound", "wipe"],
    "ali_baba": ["main", "init", "turn", "input", "combat", "ai", "events",
                 "entities", "map", "display", "util", "text", "sound", "hrcg"],
}
_SCRIPTS = {"zork": "interpreter.ld", "lode_runner": "lode_runner.ld",
            "ali_baba": "ali_baba.ld"}


def build(game: str, tmp: Path, defsyms: bool) -> Path:
    from sim.linked import defsym_args
    d = ROOT / "examples" / game
    tmp = tmp / game
    tmp.mkdir(exist_ok=True)
    objects = []
    for unit in _UNITS[game]:
        if not (d / f"{unit}.c").exists():
            continue
        obj = tmp / f"{unit}.o"
        assert compile_main(["compile.py", str(d / f"{unit}.c"), "-c", "-O",
                             "-o", str(obj)]) == 0
        objects.append(str(obj))
    exe = tmp / (game + ("" if defsyms else "-readme"))
    assert compile_main(["compile.py", "-T", str(d / _SCRIPTS[game]), *objects,
                         *(defsym_args() if defsyms else []), "-o", str(exe)]) == 0
    return exe


_ZORK = {
    "walk": ([
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
    ], []),
    "transcript": ([
        "script", "open mailbox", "take leaflet", "save", "north", "restore",
        "restore", "look", "read leaflet. read leaflet. inventory",
        "unscript", "restart", "y", "look", "quit", "y",
    ], [ord(c) for c in "362\r" "562\r" "362\r"]),
}


class _Calls(list):
    def __init__(self, mpu) -> None:
        super().__init__()
        self.mpu = mpu

    def append(self, item) -> None:
        super().append(item)
        TRACER.event(len(self) - 1, self.mpu)


def run_zork(script: str, exe: Path | None) -> tuple[list[str], int]:
    from py65.devices.mpu6502 import MPU
    from sim.apple2 import Apple2, dsk_sector
    from sim.linked import build_linked_sim
    dsk = Path(os.environ["ZORK_DSK"]).read_bytes()
    lines, keys = _ZORK[script]
    other = None
    if exe is None:
        mpu = MPU()
        binary = bytearray()
        for s in range(16, 16 + 26):
            at = dsk_sector(dsk, s // 16, s % 16)
            binary += dsk[at:at + 256]
        mpu.memory[0x0800:0x0800 + len(binary)] = binary
        mpu.pc = 0x0800
        mpu.sp = 0xFF
    else:
        sim = build_linked_sim(exe.read_bytes())
        mpu = sim.mpu
        other = dict(sim.runtime.hooks)
    a = Apple2(mpu, keys=list(keys), lines=list(lines))
    a.disks[(0x60, 1)] = bytearray(dsk)
    a.disks[(0x60, 2)] = bytearray(len(dsk))
    a.boot_screen()
    mpu.memory[0x7C] = 0x60
    mpu.memory[0x7F] = 16
    a.calls = _Calls(mpu)
    TRACER.events = lambda: len(a.calls)
    TRACER.instructions = 0
    TRACER.tracing = True
    why = a.run(other_hooks=other, recorder=TRACER.recorder)
    out = []
    for i, c in enumerate(a.calls):
        if c[0] == "RWTS":
            what = "RWTS " + " ".join(str(v) for v in c[1:])
        else:
            name, reg, ch, cv, inv = c
            what = f"{name} {'-' if reg is None else reg} {ch} {cv} {inv}"
        out.append(f"call {i} {what} state {TRACER.digest(i)}")
    for w in a.written:
        out.append("write " + " ".join(str(v) for v in w))
    out.append(f"printer {len(a.printer)} {_crc(a.printer)}")
    out.append(f"text {zlib.crc32(a.text().encode('ascii'))}")
    for (slot, drive), disk in sorted(a.disks.items()):
        out.append(f"disk {slot} {drive} {_crc(disk)}")
    out += TRACER.steps_text()
    out.append(f"end state {_digest(mpu)} memory {_crc(mpu.memory)} "
               f"instructions {TRACER.instructions} why {why}")
    return out, len(a.calls)


_LODE_RUNNER = {
    "demo": ([], (1, 2), "master", 400_000_000),
    "game": ([
        (5, ord(" ")), (8, 0x0B), (9, ord("L")),
        (300, ord("J")), (445, ord("O")),
        (692, 0x01), (775, 0x01), (858, 0x01),
        (900, ord("A")), (941, ord("B")), (982, ord("C")), (1023, 0x0D),
    ], (2, 0), "game", 200_000_000),
    "editor": ([
        (5, 0x05), (26, ord("E")), (47, 0x0D), (68, ord("K")), (89, ord("K")),
        (110, ord("K")), (131, ord("7")), (152, ord("M")), (173, ord("1")),
        (194, 0x13), (215, ord("K")), (236, ord("8")), (257, 0x11),
        (278, ord("N")), (299, ord("C")), (320, ord("0")), (341, ord("0")),
        (362, ord("2")), (383, 0x0D), (404, ord("P")), (425, ord("0")),
        (446, ord("0")), (467, ord("1")), (488, 0x0D), (521, 0x12),
    ], (3, 0), "editor", 100_000_000),
}


class _ModeChange:
    def __init__(self, before: int, after: int) -> None:
        self.before, self.after = before, after
        self.seen = False

    def __call__(self, machine) -> bool:
        mode = machine.mpu.memory[0xA7]
        if mode == self.before:
            self.seen = True
        return self.seen and mode == self.after


def _patch_record(machine_class) -> None:
    if getattr(machine_class, "_traced", False):
        return
    record = machine_class._record

    def traced(self, *args, **kwargs) -> None:
        record(self, *args, **kwargs)
        TRACER.event(len(self.events) - 1, self.mpu)

    machine_class._record = traced
    machine_class._traced = True


def run_lode_runner(script: str, exe: Path | None,
                    events: int | None) -> tuple[list[str], int]:
    from binutils import elf as E
    from sim import lode_runner as L
    from sim.apple2 import dsk_sector
    _patch_record(L.Machine)
    root = Path(os.environ["LODE_RUNNER_DIR"])
    image = (root / "main.bin").read_bytes()
    keys, (before, after), variant, max_cycles = _LODE_RUNNER[script]
    disk = bytearray((root / "lode_runner.do").read_bytes())
    at = dsk_sector(disk, 12, 15)
    if variant == "game":
        disk[at:at + 80] = bytes(80)
    if variant == "editor":
        disk[at + 255] = 0
    disk = bytes(disk)
    TRACER.instructions = 0
    if exe is None:
        m = L.original_machine(image, disk, keys)
        stop = _ModeChange(before, after)
    else:
        m = L.program_machine(image, E.read_file(exe), disk, keys,
                              recorder=TRACER.recorder)
        max_cycles = m.mpu.processorCycles + 2 * max_cycles
        n = events
        stop = lambda machine: len(machine.events) >= n  # noqa: E731
    TRACER.events = lambda: len(m.events)
    TRACER.tracing = True
    why = m.run(max_cycles, stop, recorder=TRACER.recorder)
    out = []
    ev = m.events
    for i in range(len(ev)):
        line = (f"event {i} {ev.code[i]} {ev.page1[i]} {ev.page2[i]} {ev.pc[i]} "
                f"{ev.cycles[i]} state {TRACER.digest(i)}")
        if i in ev.rwts:
            command, track, sector, buf = ev.rwts[i]
            line += f" rwts {command} {track} {sector} {_crc(buf)}"
        out.append(line)
    out += TRACER.steps_text()
    out.append(
        f"end state {_digest(m.mpu)} memory {_crc(m.mpu.memory)} disk "
        f"{_crc(m.disk)} kbd {m.kbd_reads} {m.kbd_latch} display {int(m.text)} "
        f"{int(m.mixed)} {int(m.page2)} {int(m.hires)} instructions "
        f"{TRACER.instructions} why {why}")
    return out, len(ev)


class _AliBabaScript(NamedTuple):
    keys: list[tuple[int, int]]
    end: int
    swaps: list[tuple[int, str]]
    max_cycles: int = 4_000_000_000


_ALI_BABA = {
    "attract": _AliBabaScript(keys=[], end=1500, swaps=[]),
    "game": _AliBabaScript(keys=[
        (10, 32), (12, 32), (18, 13), (49, 73), (80, 73), (141, 75),
        (202, 75), (263, 75), (284, 32), (345, 74), (406, 74), (467, 74),
        (498, 73), (529, 73), (560, 73), (591, 73), (692, 32), (723, 13),
        (754, 75), (785, 13), (846, 77), (877, 13), (908, 13), (939, 74),
        (970, 13), (1001, 13), (1032, 74), (1063, 13), (1094, 13),
        (1125, 74), (1156, 13), (1187, 73), (1218, 73), (1279, 74),
        (1340, 74), (1401, 74), (1462, 74), (1523, 74), (1584, 32),
        (1645, 32), (1706, 32),
    ], end=1800, swaps=[]),
    "save": _AliBabaScript(keys=[
        (10, 32), (12, 32), (18, 13), (49, 13), (80, 74), (111, 73),
        (142, 13), (173, 75), (204, 13), (235, 75), (266, 13), (297, 75),
        (328, 13), (359, 73), (390, 13), (421, 74), (452, 13), (483, 74),
        (514, 13), (545, 73), (576, 13), (607, 77), (638, 13), (669, 13),
        (673, 32), (677, 32), (681, 32), (685, 32), (696, 77), (707, 13),
        (738, 32), (799, 13), (830, 75), (861, 13), (922, 77),
    ], end=1000, swaps=[(691, 'scenario'), (738, 'game')]),
    "load": _AliBabaScript(keys=[
        (10, 32), (12, 32), (18, 77), (49, 13), (70, 32), (101, 32),
        (162, 13), (193, 75), (224, 13), (285, 77),
    ], end=350, swaps=[(70, 'scenario'), (101, 'game')]),
    "forest": _AliBabaScript(keys=[
        (10, 32), (12, 32), (18, 13), (49, 73), (80, 73), (111, 75),
        (142, 75), (173, 75), (204, 73), (235, 75), (266, 75), (297, 75),
        (328, 75), (359, 77), (390, 77), (421, 74), (452, 74), (483, 74),
        (514, 74), (545, 74), (576, 74), (607, 74), (638, 75), (669, 75),
        (700, 75), (731, 77), (762, 13), (793, 32), (824, 73), (855, 32),
        (886, 73), (917, 32), (948, 77), (979, 32), (1031, 74), (1062, 74),
        (1093, 74), (1124, 74), (1155, 77), (1186, 77), (1217, 77),
        (1248, 77), (1279, 77), (1310, 77), (1341, 74), (1372, 73),
        (1403, 73), (1434, 73), (1465, 75), (1496, 75), (1527, 13),
        (1558, 73), (1589, 32), (1620, 32), (1651, 73), (1682, 32),
        (1713, 77), (1744, 32), (1775, 32), (1806, 73), (1837, 73),
        (1868, 75), (1899, 32), (1930, 77), (1961, 32), (1992, 32),
        (2023, 32), (2054, 75), (2085, 32), (2116, 32), (2147, 77),
        (2178, 32), (2209, 32), (2240, 73), (2271, 32), (2302, 73),
        (2333, 32), (2364, 32), (2395, 73), (2426, 73), (2457, 75),
        (2488, 32), (2519, 73), (2550, 32), (2581, 32), (2612, 73),
        (2643, 32),
    ], end=2674, swaps=[]),
    "party": _AliBabaScript(keys=[
        (10, 32), (12, 32), (43, 77), (74, 75), (105, 32), (136, 75),
        (167, 32), (198, 73), (229, 13), (260, 73), (291, 73), (322, 75),
        (353, 75), (384, 75), (415, 13), (446, 73), (477, 32), (508, 75),
        (539, 32), (570, 13), (601, 74), (632, 73), (663, 32), (694, 74),
        (725, 32), (756, 75), (787, 32), (818, 75), (849, 32), (880, 77),
        (911, 32), (942, 77), (973, 32), (1004, 32), (1035, 73), (1066, 32),
        (1097, 13), (1128, 74), (1159, 73), (1190, 32), (1221, 75),
        (1252, 32), (1283, 77), (1314, 32), (1345, 75), (1376, 32),
        (1407, 75), (1438, 32), (1469, 75), (1500, 32), (1531, 32),
        (1562, 32), (1593, 32), (1624, 32), (1655, 77), (1686, 32),
        (1717, 73), (1748, 32), (1779, 13), (1810, 74), (1841, 73),
        (1872, 32), (1903, 75), (1934, 77), (1965, 32), (1996, 75),
        (2027, 75), (2058, 75), (2089, 75), (2120, 32), (2151, 32),
        (2182, 77),
    ], end=2213, swaps=[]),
}


_AB_DISK = "Ali Baba and the Forty Thieves (4am and san inc crack).dsk"
_AB_FILES: dict[str, bytes | None] = {}


def _ali_baba_file(name: str) -> bytes:
    if name not in _AB_FILES:
        root = Path(os.environ["ALI_BABA_DIR"])
        path = root / _AB_DISK if name == "disk" else root / "output" / "main.bin"
        _AB_FILES[name] = path.read_bytes()
    return _AB_FILES[name]


def _ali_baba_disks(script: str) -> dict[str, bytes | None]:
    from sim import ali_baba as A
    if script == "save":
        return {"scenario": None}
    if script != "load":
        return {}
    if "scenario" not in _AB_FILES:
        s = _ALI_BABA["save"]
        m = A.original_machine(_ali_baba_file("main_bin"), _ali_baba_file("disk"),
                               s.keys, s.swaps, {"scenario": None})
        assert m.run(s.max_cycles, lambda machine: machine.kbd_reads >= s.end) == "stop"
        _AB_FILES["scenario"] = bytes(m.media["scenario"].to_dsk())
    return {"scenario": _AB_FILES["scenario"]}


def _ali_baba_coverage() -> list[str]:
    from sim import ali_baba as A
    root = Path(os.environ["ALI_BABA_DIR"]) / "output"
    labels = A.original_labels((root / "main.asm").read_text(errors="replace"),
                               (root / "main.sym").read_text(errors="replace"))
    out = []
    seen = bytearray(0x10000)
    for name, s in _ALI_BABA.items():
        m = A.original_machine(_ali_baba_file("main_bin"), _ali_baba_file("disk"),
                               s.keys, s.swaps, _ali_baba_disks(name))
        assert m.run(s.max_cycles, lambda machine: machine.kbd_reads >= s.end) == "stop"
        cov = A.coverage(m.seen, labels, A.CODE_SPANS)
        out.append(f"{name}: {len(m.events)} events, {cov.summary()} "
                   "in the C program's spans")
        seen = bytearray(x | y for x, y in zip(seen, m.seen))
    cov = A.coverage(bytes(seen), labels, A.CODE_SPANS)
    out.append(f"all: {cov.summary()}; not reached:")
    out.append("  " + " ".join(cov.missed))
    out.append("end instructions 0")
    out.append("undefined")
    return out


def run_ali_baba(script: str, exe: Path | None,
                 events: int | None) -> tuple[list[str], int]:
    from binutils import elf as E
    from sim import ali_baba as A
    _patch_record(A.Machine)
    if script == "coverage":
        return _ali_baba_coverage(), 0
    s = _ALI_BABA[script]
    disks = _ali_baba_disks(script)
    main_bin, game_disk = _ali_baba_file("main_bin"), _ali_baba_file("disk")
    TRACER.instructions = 0
    if exe is None:
        m = A.original_machine(main_bin, game_disk, s.keys, s.swaps, disks)
        max_cycles = s.max_cycles
        stop = lambda machine: machine.kbd_reads >= s.end  # noqa: E731
    else:
        ld = (ROOT / "examples" / "ali_baba" / "ali_baba.ld").read_text()
        m = A.program_machine(main_bin, E.read_file(exe), game_disk,
                              s.keys, s.swaps, disks, clear=A.link_regions(ld),
                              recorder=TRACER.recorder)
        max_cycles = m.mpu.processorCycles + 2 * s.max_cycles
        n = events
        stop = lambda machine: len(machine.events) >= n  # noqa: E731
    TRACER.events = lambda: len(m.events)
    TRACER.tracing = True
    why = m.run(max_cycles, stop, recorder=TRACER.recorder)
    out = []
    ev = m.events
    for i in range(len(ev)):
        line = (f"event {i} {ev.code[i]} {ev.page1[i]} {ev.arg[i]} {ev.pc[i]} "
                f"{ev.cycles[i]} state {TRACER.digest(i)}")
        if i in ev.disk:
            entry, carry, ht, iob, buf = ev.disk[i]
            line += (f" disk {entry} {carry} {ht} {iob.hex().upper() if iob else '-'} "
                     f"{_crc(buf) if buf else '-'}")
        out.append(line)
    out += TRACER.steps_text()
    c = m.disk
    out.append(f"controller {c.selected} {int(c.motor)} {c.motor_off_at} "
               f"{int(c.q6)} {int(c.q7)} {c.latch} {c.position}")
    for name, drive in m.media.items():
        out.append(f"medium {name} dsk {_crc(drive.to_dsk())} tracks "
                   f"{_crc(b''.join(bytes(t) for t in drive.tracks))} head "
                   f"{drive.half_track} {drive.phases}")
    out.append(
        f"end state {_digest(m.mpu)} memory {_crc(m.mpu.memory)} seen "
        f"{_crc(m.seen)} kbd {m.kbd_reads} {m.kbd_latch} display {int(m.text)} "
        f"{int(m.mixed)} {int(m.page2)} {int(m.hires)} {int(m.display_set)} "
        f"instructions {TRACER.instructions} why {why}")
    return out, len(ev)


_RUNNERS = {"zork": run_zork, "lode_runner": run_lode_runner,
            "ali_baba": run_ali_baba}
_ALL = {"zork": list(_ZORK), "lode_runner": list(_LODE_RUNNER),
        "ali_baba": list(_ALI_BABA)}


def python_run(game: str, script: str, exe: Path | None, events: int | None,
               steps_after: int | None = None, steps: int = 0):
    TRACER.reset(steps_after, steps)
    if game == "zork":
        return run_zork(script, exe)
    return _RUNNERS[game](script, exe, events)


def cpp_run(game: str, script: str, exe: Path | None, events: int | None,
            steps_after: int | None = None, steps: int = 0) -> list[str]:
    cmd = [cpp_backend._require_cli(), "game-run", game, script]
    if exe is not None:
        cmd += ["--exe", str(exe)]
    if events is not None and game != "zork":
        cmd += ["--events", str(events)]
    if steps_after is not None:
        cmd += ["--steps-after", str(steps_after), "--steps", str(steps)]
    done = subprocess.run(cmd, capture_output=True, cwd=ROOT)
    if done.returncode != 0:
        raise RuntimeError(f"{' '.join(cmd)}: {done.stderr.decode(errors='replace')}")
    return done.stdout.decode("ascii").splitlines()


def _first_difference(a: list[str], b: list[str]) -> int | None:
    for i, (x, y) in enumerate(zip(a, b)):
        if x != y:
            return i
    return None if len(a) == len(b) else min(len(a), len(b))


class Comparison(NamedTuple):
    equal: bool
    events: int
    instructions: int
    report: list[str]


def compare(game: str, script: str, program: str, exe: Path | None,
            events: int | None, out_dir: Path) -> Comparison:
    t = time.time()
    py, _ = python_run(game, script, exe, events)
    t_py = time.time() - t
    t = time.time()
    cpp = cpp_run(game, script, exe, events)
    t_cpp = time.time() - t
    stem = f"{game}-{script}-{program}"
    (out_dir / f"{stem}.py.txt").write_text("\n".join(py) + "\n", encoding="utf-8", newline="\n")
    (out_dir / f"{stem}.cpp.txt").write_text("\n".join(cpp) + "\n", encoding="utf-8", newline="\n")
    n_events = sum(1 for ln in py if ln.startswith(("event ", "call ")))
    if script == "coverage":
        i = _first_difference(py[:-2], cpp)
        return Comparison(i is None, 0, 0, [
            f"{stem}: {'equal' if i is None else f'line {i} differs'}"])
    instructions = int(py[-1].split(" instructions ")[1].split()[0])
    undefined = next(ln for ln in py if ln.startswith("undefined"))
    i = _first_difference(py, cpp)
    if i is None:
        return Comparison(True, n_events, instructions, [
            f"{stem}: equal, {n_events} events, {instructions} instructions, "
            f"{undefined}, Python {t_py:.0f} s, C++ {t_cpp:.1f} s"])
    report = [f"{stem}: line {i} differs",
              f"  python: {py[i] if i < len(py) else '(none)'}",
              f"  c++:    {cpp[i] if i < len(cpp) else '(none)'}"]
    last = None
    for ln in py[:i]:
        if ln.startswith(("event ", "call ")):
            last = int(ln.split()[1])
    after = -1 if last is None else last
    py_steps, _ = python_run(game, script, exe, events, after, 200_000)
    cpp_steps = cpp_run(game, script, exe, events, after, 200_000)
    ps = [ln for ln in py_steps if ln.startswith("step ")]
    cs = [ln for ln in cpp_steps if ln.startswith("step ")]
    j = _first_difference(ps, cs)
    if j is None:
        report.append(f"  the {len(ps)} steps after event {after} are equal")
    else:
        report += [f"  step {j} after event {after} differs",
                   f"    before: {ps[j - 1] if j > 0 else '(the event)'}",
                   f"    python: {ps[j] if j < len(ps) else '(none)'}",
                   f"    c++:    {cs[j] if j < len(cs) else '(none)'}"]
    return Comparison(False, n_events, instructions, report)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--game", choices=sorted(_RUNNERS))
    parser.add_argument("--script")
    parser.add_argument("--program", choices=["original", "c", "readme"])
    parser.add_argument("--out", default=None)
    parser.add_argument("-j", type=int, default=1, help="runs at once")
    args = parser.parse_args()
    out_dir = Path(args.out or tempfile.mkdtemp(prefix="compare_games_"))
    out_dir.mkdir(parents=True, exist_ok=True)
    games = [args.game] if args.game else ["zork", "lode_runner", "ali_baba"]
    runs: list[tuple[str, str, str]] = []
    for game in games:
        for script in ([] if args.script == "coverage" else
                       [args.script] if args.script else _ALL[game]):
            programs = [args.program] if args.program else (
                ["original", "c", "readme"] if game == "ali_baba" else ["original", "c"])
            for program in programs:
                if program != "readme" or script == "game":
                    runs.append((game, script, program))
        if game == "ali_baba" and args.script in (None, "coverage") and args.program in (None, "original"):
            runs.append((game, "coverage", "original"))
    ok = True
    totals = [0, 0, 0]
    with tempfile.TemporaryDirectory() as tmp, \
            ProcessPoolExecutor(max_workers=args.j) as pool:
        exes: dict[tuple[str, str], Path] = {}
        for game, _, program in runs:
            if program != "original" and (game, program) not in exes:
                exes[(game, program)] = build(game, Path(tmp), game == "ali_baba" and program == "c")
        originals = {(g, s) for g, s, p in runs if p != "original" and g != "zork"}
        counts: dict[tuple[str, str], int] = {}
        first = [(g, s, "original") for g, s, p in runs if p == "original" or (g, s) in originals]
        first = list(dict.fromkeys(first))
        futures = {pool.submit(compare, g, s, p, None, None, out_dir): (g, s, p) for g, s, p in first}
        for future in as_completed(futures):
            g, s, p = futures[future]
            c = future.result()
            counts[(g, s)] = c.events
            if (g, s, p) in runs:
                print("\n".join(c.report), flush=True)
                ok &= c.equal
                totals = [totals[0] + 1, totals[1] + c.events, totals[2] + c.instructions]
        second = [(g, s, p) for g, s, p in runs if p != "original"]
        futures = {pool.submit(compare, g, s, p, exes[(g, p)], counts.get((g, s)), out_dir): (g, s, p)
                   for g, s, p in second}
        for future in as_completed(futures):
            c = future.result()
            print("\n".join(c.report), flush=True)
            ok &= c.equal
            totals = [totals[0] + 1, totals[1] + c.events, totals[2] + c.instructions]
    print(f"{totals[0]} runs, {totals[1]} events, {totals[2]} instructions: "
          f"{'all equal' if ok else 'DIFFERENT'} (texts in {out_dir})")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
