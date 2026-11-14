"""The multi-unit example in examples/zork/print_zstring: Zork's
z-string printer and print_string_literal, in four C files linked with
a linker script into the addresses Zork gives them.

`TestGoldOutputs` checks each unit's `--optimize` output and the linked
program against the checked-in `.asm` files.

The rest runs the linked program in the simulator (assembled with
dasm, so it skips without dasm) on a small story file. The story has
five 256-byte pages. Pages 0 and 1 are resident at `_IMAGE`, and pages
2 to 4 are read from a disk image at `_DISK` into a two-slot cache, so
that the string crosses from a resident page into a cached one and the
third page evicts the least recently used slot. The string uses every
kind of z-character: letters in all three alphabets, both kinds of
shift, a newline, a 10-bit character, and two abbreviations. The window
is 20 columns wide and 4 lines tall, so the text wraps between words
and stops twice for [MORE].

ROM routines, RWTS, and Zork's `main` are Python hooks, run when the
program counter reaches their addresses. COUT1 and COUT record each
character, RDKEY returns a space, CLREOL records a marker, RWTS copies
a page from the disk image, and `main`, where Zork goes when a page
can't be read, stops the run. Three scenarios run: the string; the
string with a transcript, which copies each line to the printer; and
the string with every disk read failing.

When the environment variable ZORK_MAIN_BIN names Zork's main.bin
(https://github.com/XekriRedmane/zorki-r15-reveng, loaded at $0800),
`test_matches_original` runs Zork's own print_string_literal in the
same scenarios and compares what each prints and the state it leaves.
The binary isn't part of this repository.
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

_DIR = Path(__file__).parent.parent / "examples" / "zork" / "print_zstring"
_UNITS = ["disk", "pages", "output", "zstring"]
_HAVE_PCPP = shutil.which("pcpp") is not None
_HAVE_DASM = shutil.which("dasm") is not None

_RWTS, _MAIN = 0x2900, 0x0800
_COUT1, _COUT, _RDKEY, _CLREOL = 0xFDF0, 0xFDED, 0xFD0C, 0xFC9C
_PRINT_STRING_LITERAL = 0x0C5B   # in main.bin
_PRINTER = 0xC100                # PRINTER_CSW at the start

_IMAGE = 0x8000          # story pages 0 and 1, then the cache slots
_DISK = 0x9000           # story page p at _DISK + 256 * p
_TABLES = 0xA100         # PAGE_L / PAGE_H / NEXT / PREV, 16 bytes each
_ABBREVS = 0xA200
_PAGES = 5
_SPT = 16                # SECTORS_PER_TRACK

# Alphabet 2, from z-character 8 on, in the order of Zork's a2_table.
_A2 = "0123456789.,!?_#'\"/\\-:()"

_DRIVER = """
#include "zork.h"
int main(void) { print_string_literal(); return 0; }
"""


def _compile(unit: str, out: Path) -> None:
    from compile import main as compile_main
    assert compile_main([
        "compile.py", str(_DIR / f"{unit}.c"), "--codegen", "--optimize",
        "-o", str(out),
    ]) == 0


def _link(paths: list[Path], out: Path) -> None:
    from compile import main as compile_main
    assert compile_main([
        "compile.py", "--link", "-T", str(_DIR / "print_zstring.ld"),
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
            linked = Path(tmp) / "print_zstring.asm"
            _link(paths, linked)
            self.assertEqual(
                linked.read_text(),
                (_DIR / "print_zstring.asm").read_text(),
            )


def _zchars(text: str, abbrevs: dict[str, int]) -> list[int]:
    """Z-characters for `text`. `{name}` is abbreviation `abbrevs[name]`,
    `\\n` a newline, and `~X` a 10-bit character X. Uppercase letters
    use a single shift to alphabet 1, and a run of digits after `#`
    uses a shift lock to alphabet 2 and back."""
    out: list[int] = []
    i = 0
    while i < len(text):
        c = text[i]
        if c == "{":
            j = text.index("}", i)
            out += [1, abbrevs[text[i + 1:j]]]
            i = j + 1
            continue
        if c == "~":
            code = ord(text[i + 1])
            out += [3, 6, code >> 5, code & 0x1F]
            i += 2
            continue
        if c == " ":
            out.append(0)
        elif c == "\n":
            out += [3, 7]
        elif c.islower():
            out.append(ord(c) - ord("a") + 6)
        elif c.isupper():
            out += [2, ord(c) - ord("A") + 6]
        elif c == "#":
            # Lock alphabet 2 for the digits that follow, then lock
            # back to alphabet 0. From 0, z = 5 locks (0 + 5) % 3 = 2;
            # from 2, z = 4 locks (2 + 4) % 3 = 0.
            j = i + 1
            while j < len(text) and text[j].isdigit():
                j += 1
            out.append(5)
            out += [_A2.index(d) + 8 for d in text[i + 1:j]]
            out.append(4)
            i = j
            continue
        else:
            out += [3, _A2.index(c) + 8]
        i += 1
    return out


def _zstring(text: str, abbrevs: dict[str, int] | None = None) -> bytes:
    """The z-string for `text`: three z-characters to a big-endian
    word, padded with 5, and the top bit set on the last word."""
    zs = _zchars(text, abbrevs or {})
    while len(zs) % 3:
        zs.append(5)
    words = [(zs[k] << 10) | (zs[k + 1] << 5) | zs[k + 2]
             for k in range(0, len(zs), 3)]
    words[-1] |= 0x8000
    return b"".join(w.to_bytes(2, "big") for w in words)


def _story() -> tuple[bytearray, int, list[int]]:
    """The story's bytes, the address of the string to print, and the
    abbreviation table's packed addresses."""
    story = bytearray(256 * _PAGES)
    # Two abbreviations, in pages 3 and 4 (packed: byte address / 2).
    abbr_text = ["the great underground empire", "Zork"]
    abbr_at = [0x0300, 0x0400]
    for text, at in zip(abbr_text, abbr_at):
        z = _zstring(text)
        story[at:at + len(z)] = z
    text = ("West of {0} House\nYou are standing in an open field "
            "west of {1} house, with #1980 boarded doors. ~@ Hi, "
            "{0}!")
    z = _zstring(text, {"0": 0, "1": 1})
    # Start 20 bytes before the end of page 1, so the string runs on
    # into page 2, which comes from disk.
    start = 0x0200 - 20
    story[start:start + len(z)] = z
    assert start + len(z) < 0x0300
    return story, start, [a // 2 for a in abbr_at]


class _Restart(Exception):
    """Zork's `main` was reached."""


class _Machine:
    """One MPU with the story, tables, and window set up, and the
    hooks for the ROM routines, RWTS, and Zork's `main`."""

    def __init__(self, mpu: MPU, *, transcript: bool = False,
                 read_fails: bool = False) -> None:
        self.mpu = mpu
        self.read_fails = read_fails
        self.screen: list[int] = []
        self.printer: list[int] = []
        self.rdkey = 0
        self.restarted = False
        story, start, abbrevs = _story()
        if transcript:
            story[0x11] |= 0x01
        m = mpu.memory
        m[_IMAGE:_IMAGE + 0x200] = story[:0x200]
        m[_DISK:_DISK + len(story)] = story
        for k, p in enumerate(abbrevs):
            m[_ABBREVS + 2 * k] = p >> 8
            m[_ABBREVS + 2 * k + 1] = p & 0xFF
        # Two empty cache slots, slot 0 first.
        for t in range(4):
            m[_TABLES + 16 * t:_TABLES + 16 * t + 16] = [0xFF] * 16
        m[_TABLES + 32] = 1            # NEXT[0] = 1
        m[_TABLES + 48 + 1] = 0        # PREV[1] = 0
        zp = {
            0x7F: _SPT,
            0x8A: start & 0xFF, 0x8B: start >> 8, 0x8C: 0,
            0x8F: 1, 0x90: 0xFE,
            0xB8: 0x00, 0xB9: (_IMAGE >> 8) + 2,
            0xBA: 0x00, 0xBB: _IMAGE >> 8,
            0xBC: 2, 0xBD: 2, 0xBE: 0, 0xBF: 1,
            0xC0: (_TABLES) & 0xFF, 0xC1: _TABLES >> 8,
            0xC2: (_TABLES + 16) & 0xFF, 0xC3: _TABLES >> 8,
            0xC4: (_TABLES + 32) & 0xFF, 0xC5: _TABLES >> 8,
            0xC6: (_TABLES + 48) & 0xFF, 0xC7: _TABLES >> 8,
            0xE2: _ABBREVS & 0xFF, 0xE3: _ABBREVS >> 8,
            0xEB: 0, 0xED: 0,
            0xEE: _PRINTER & 0xFF, 0xEF: _PRINTER >> 8,
            0x21: 20, 0x22: 0, 0x23: 4, 0x24: 0, 0x32: 0xFF,
            0x36: 0xF0, 0x37: 0xFD,    # CSW: COUT1
        }
        for addr, v in zp.items():
            m[addr] = v
        self.hooks = {
            _COUT1: lambda: self.screen.append(self.mpu.a),
            _COUT: self._cout,
            _RDKEY: self._rdkey,
            _CLREOL: lambda: self.screen.append(-1),
            _RWTS: self._rwts,
            _MAIN: self._restart,
        }

    def _cout(self) -> None:
        # COUT goes where CSW points: the printer while a line is
        # copied there, the screen otherwise.
        m = self.mpu.memory
        csw = m[0x36] | m[0x37] << 8
        (self.printer if csw == _PRINTER else self.screen).append(
            self.mpu.a,
        )

    def _rdkey(self) -> None:
        self.rdkey += 1
        self.mpu.a = 0xA0

    def _rwts(self) -> None:
        m = self.mpu.memory
        # The parameter block's address: low byte in Y, high in A.
        iob = (self.mpu.a << 8) | self.mpu.y
        track, sector = m[iob + 4], m[iob + 5]
        buf = m[iob + 8] | (m[iob + 9] << 8)
        assert m[iob + 12] == 1, "only reads are expected"
        if self.read_fails:
            self.mpu.p |= 1
            return
        page = (track - 3) * _SPT + sector
        src = _DISK + 256 * page
        m[buf:buf + 256] = m[src:src + 256]
        self.mpu.p &= ~1               # carry clear: success

    def _restart(self) -> None:
        self.restarted = True
        raise _Restart

    def run(self, other_hooks=None, max_steps: int = 2_000_000) -> None:
        mpu = self.mpu
        hooks = dict(other_hooks or {})
        for k, v in self.hooks.items():
            hooks[k] = lambda _m, f=v: f()
        try:
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
        except _Restart:
            return
        raise AssertionError("ran too long")

    def state(self) -> dict:
        """The interpreter state print_string_literal leaves, apart from
        its scratch bytes, which this version uses less."""
        m = self.mpu.memory
        zp = {a: m[a] for a in (
            0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x91, 0x92, 0x93,
            0x94, 0x95, 0x96, 0x97, 0xBE, 0xBF, 0xCE, 0xCF, 0xD0,
            0xD1, 0xD2, 0xE4, 0xE5, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
            0x24, 0x32, 0x36, 0x37,
        )}
        return {
            "zp": zp,
            "tables": bytes(m[_TABLES:_TABLES + 64]),
            "cache": bytes(m[_IMAGE + 0x200:_IMAGE + 0x400]),
            "buffer": bytes(m[0x0200:0x0200 + m[0xEB]]),
            "screen": self.screen,
            "printer": self.printer,
            "rdkey": self.rdkey,
            "restarted": self.restarted,
        }


_SCENARIOS = {
    "plain": {},
    "transcript": {"transcript": True},
    "read fails": {"read_fails": True},
}


def _linked_text() -> str:
    """The example's units and a `main` that calls
    print_string_literal, linked by the example's script."""
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


def _run_c(text: str, **scenario) -> _Machine:
    from sim.linked import build_linked_sim
    sim = build_linked_sim(text)
    machine = _Machine(sim.mpu, **scenario)
    machine.run(sim.runtime.hooks)
    return machine


def _run_original(path: str, **scenario) -> _Machine:
    mpu = MPU()
    binary = Path(path).read_bytes()
    mpu.memory[0x0800:0x0800 + len(binary)] = binary
    # JSR print_string_literal ; BRK
    mpu.memory[0x0300:0x0304] = bytes([
        0x20, _PRINT_STRING_LITERAL & 0xFF, _PRINT_STRING_LITERAL >> 8,
        0x00,
    ])
    mpu.pc = 0x0300
    mpu.sp = 0xFF
    machine = _Machine(mpu, **scenario)
    machine.run()
    return machine


def _text(screen: list[int]) -> str:
    """The printed characters with the high bit removed, CLREOL as
    `|`, and carriage return as a newline."""
    out = []
    for c in screen:
        if c == -1:
            out.append("|")
        else:
            out.append(chr(c & 0x7F).replace("\r", "\n"))
    return "".join(out)


@unittest.skipUnless(_HAVE_PCPP and _HAVE_DASM, "needs pcpp and dasm")
class TestRuns(unittest.TestCase):

    @classmethod
    def setUpClass(cls) -> None:
        text = _linked_text()
        cls.runs = {
            name: _run_c(text, **scenario)
            for name, scenario in _SCENARIOS.items()
        }

    def test_words_in_order(self) -> None:
        machine = self.runs["plain"]
        # The last line is still in the buffer, waiting for its end.
        buffered = "".join(chr(c & 0x7F) for c in machine.state()["buffer"])
        text = _text(machine.screen) + buffered
        self.assertEqual(
            text.split(),
            ["WEST", "OF", "THE", "GREAT", "UNDERGROUND", "EMPIRE", "HOUSE",
             "[MORE]|YOU", "ARE", "STANDING", "IN", "AN", "OPEN", "FIELD",
             "WEST", "OF", "ZORK", "HOUSE,", "WITH", "[MORE]|1980",
             "BOARDED", "DOORS.", "@", "HI,", "THE", "GREAT",
             "UNDERGROUND", "EMPIRE!"],
        )
        self.assertFalse(machine.restarted)

    def test_transcript(self) -> None:
        plain, copied = self.runs["plain"], self.runs["transcript"]
        self.assertEqual(copied.screen, plain.screen)
        # The printer gets its setup, ctrl-I 80N, then each line and
        # its return, without the [MORE] prompts.
        self.assertTrue(_text(copied.printer).startswith(
            "\t80NWEST OF THE GREAT\nUNDERGROUND EMPIRE\nHOUSE\n"
        ))

    def test_read_fails(self) -> None:
        # The first abbreviation is on a page that has to be read, so
        # the run stops at Zork's `main` with the words before it
        # still in the buffer.
        machine = self.runs["read fails"]
        self.assertTrue(machine.restarted)
        self.assertEqual(machine.screen, [])
        self.assertEqual(
            bytes(c & 0x7F for c in machine.state()["buffer"]), b"WEST OF ",
        )

    @unittest.skipUnless(os.environ.get("ZORK_MAIN_BIN"),
                         "set ZORK_MAIN_BIN to Zork's main.bin")
    def test_matches_original(self) -> None:
        path = os.environ["ZORK_MAIN_BIN"]
        for name, scenario in _SCENARIOS.items():
            with self.subTest(name):
                original = _run_original(path, **scenario)
                ours = self.runs[name]
                self.assertEqual(_text(ours.screen), _text(original.screen))
                self.assertEqual(ours.state(), original.state())


if __name__ == "__main__":
    unittest.main()
