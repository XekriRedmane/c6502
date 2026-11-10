"""Runs examples/zork_print_string_literal.c in the simulator on a small
story file, and compares what it prints, and the interpreter state it
leaves, with Zork's own code.

The story has five 256-byte pages. Pages 0 and 1 are resident at
`_IMAGE`, and pages 2 to 4 are read from a disk image at `_DISK` into
a two-slot cache, so that the string crosses from a resident page into
a cached one and the third page evicts the least recently used slot.
The string uses every kind of z-character: letters in all three
alphabets, both kinds of shift, a newline, a 10-bit character, and two
abbreviations. The window is 20 columns wide and 4 lines tall, so the
text wraps between words and stops twice for [MORE].

ROM routines and RWTS are Python hooks, run when the program counter
reaches their addresses. COUT1 records each character, RDKEY returns
a space, CLREOL records a marker, and RWTS copies a page from the disk
image.

`test_matches_original` runs Zork's code from its binary (main.bin
from https://github.com/XekriRedmane/zorki-r15-reveng, loaded at
$0800) when the environment variable ZORK_MAIN_BIN names that file,
and skips otherwise. The binary isn't part of this repository.
"""
from __future__ import annotations

import os
import shutil
import unittest
from pathlib import Path

from py65.devices.mpu6502 import MPU

from sim.harness import build_sim

_EXAMPLE = Path(__file__).parent.parent / "examples" / "zork_print_string_literal.c"

# The test's harness: the routine the example leaves outside, and a
# main that prints one string literal.
_DRIVER = """
uint8_t restart_called;
void restart(void) { restart_called = 1; }
int main(void) {
    print_string_literal();
    return 0;
}
"""

# Both programs call RWTS at $2900, where Zork's DOS has it. Zork's
# code is at $0800-$21FF. The unoptimized C program is long enough to
# reach $2900 from the sim's usual $0800, so it's loaded at $2A00, and
# the story and tables go above the soft stack, which grows down from
# $7FFF.
_RWTS = 0x2900
_C_ORIGIN = 0x2A00
_IMAGE = 0x8000          # story pages 0 and 1, then the cache slots
_DISK = 0x9000           # story page p at _DISK + 256 * p
_TABLES = 0xA100         # PAGE_L / PAGE_H / NEXT / PREV, 16 bytes each
_ABBREVS = 0xA200
_PAGES = 5
_SPT = 16                # SECTORS_PER_TRACK

_COUT1, _COUT, _RDKEY, _CLREOL = 0xFDF0, 0xFDED, 0xFD0C, 0xFC9C

# Alphabet 2, from z-character 8 on, in the order of Zork's a2_table.
_A2 = "0123456789.,!?_#'\"/\\-:()"


def _zchars(text: str, abbrevs: dict[str, int]) -> list[int]:
    """Z-characters for `text`. `{name}` is abbreviation `abbrevs[name]`,
    `\\n` a newline, and `~X` a 10-bit character X. Uppercase letters
    use a single shift to alphabet 1, and the run "DIGITS" uses a
    shift lock to alphabet 2 and back."""
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


class _Machine:
    """One MPU with the story, tables, and window set up, and the
    hooks for the ROM routines and RWTS."""

    def __init__(self, mpu: MPU) -> None:
        self.mpu = mpu
        self.screen: list[int] = []
        self.printer: list[int] = []
        self.clreol = 0
        self.rdkey = 0
        story, start, abbrevs = _story()
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
            0x21: 20, 0x22: 0, 0x23: 4, 0x24: 0, 0x32: 0xFF,
        }
        for addr, v in zp.items():
            m[addr] = v
        self.hooks = {
            _COUT1: lambda: self.screen.append(self.mpu.a),
            _COUT: lambda: self.printer.append(self.mpu.a),
            _RDKEY: self._rdkey,
            _CLREOL: self._clreol,
            _RWTS: self._rwts,
        }

    def _rdkey(self) -> None:
        self.rdkey += 1
        self.mpu.a = 0xA0

    def _clreol(self) -> None:
        self.clreol += 1
        self.screen.append(-1)

    def _rwts(self) -> None:
        m = self.mpu.memory
        # The parameter block's address: high byte in A, low in Y.
        iob = (self.mpu.a << 8) | self.mpu.y
        track, sector = m[iob + 4], m[iob + 5]
        buf = m[iob + 8] | (m[iob + 9] << 8)
        assert m[iob + 12] == 1, "only reads are expected"
        page = (track - 3) * _SPT + sector
        src = _DISK + 256 * page
        m[buf:buf + 256] = m[src:src + 256]
        self.mpu.p &= ~1               # carry clear: success

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

    def state(self) -> dict:
        """The interpreter state print_string_literal leaves, apart from
        its scratch bytes, which this version uses less."""
        m = self.mpu.memory
        zp = {a: m[a] for a in (
            0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x91, 0x92, 0x93,
            0x94, 0x95, 0x96, 0x97, 0xBE, 0xBF, 0xCE, 0xCF, 0xD0,
            0xD1, 0xD2, 0xE4, 0xE5, 0xEB, 0xEC, 0xED, 0x24, 0x32,
        )}
        return {
            "zp": zp,
            "tables": bytes(m[_TABLES:_TABLES + 64]),
            "cache": bytes(m[_IMAGE + 0x200:_IMAGE + 0x400]),
            "buffer": bytes(m[0x0200:0x0200 + m[0xEB]]),
            "screen": self.screen,
            "printer": self.printer,
            "rdkey": self.rdkey,
        }


def _run_c(optimize: bool) -> _Machine:
    src = _EXAMPLE.read_text() + _DRIVER
    sim = build_sim(src, optimize=optimize, origin=_C_ORIGIN)
    assert sim.code_end <= 0x7000, "the program reaches the soft stack"
    machine = _Machine(sim.mpu)
    machine.run(sim.runtime.hooks)
    assert sim.mpu.memory[sim.symbols["restart_called"]] == 0
    return machine


def _run_original(path: str) -> _Machine:
    mpu = MPU()
    binary = Path(path).read_bytes()
    mpu.memory[0x0800:0x0800 + len(binary)] = binary
    # JSR print_string_literal ; BRK
    mpu.memory[0x0300:0x0304] = bytes([0x20, 0x5B, 0x0C, 0x00])
    mpu.pc = 0x0300
    mpu.sp = 0xFF
    machine = _Machine(mpu)
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


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestZorkPrintStringLiteral(unittest.TestCase):

    @classmethod
    def setUpClass(cls) -> None:
        cls.unopt = _run_c(False)
        cls.opt = _run_c(True)

    def test_pipelines_agree(self) -> None:
        self.assertEqual(self.unopt.state(), self.opt.state())

    def test_words_in_order(self) -> None:
        # The last line is still in the buffer, waiting for its end.
        buffered = "".join(chr(c & 0x7F) for c in self.opt.state()["buffer"])
        text = _text(self.opt.screen) + buffered
        self.assertEqual(
            text.split(),
            ["WEST", "OF", "THE", "GREAT", "UNDERGROUND", "EMPIRE", "HOUSE",
             "[MORE]|YOU", "ARE", "STANDING", "IN", "AN", "OPEN", "FIELD",
             "WEST", "OF", "ZORK", "HOUSE,", "WITH", "[MORE]|1980",
             "BOARDED", "DOORS.", "@", "HI,", "THE", "GREAT",
             "UNDERGROUND", "EMPIRE!"],
        )

    @unittest.skipUnless(os.environ.get("ZORK_MAIN_BIN"),
                         "set ZORK_MAIN_BIN to Zork's main.bin")
    def test_matches_original(self) -> None:
        original = _run_original(os.environ["ZORK_MAIN_BIN"])
        self.assertEqual(_text(self.opt.screen), _text(original.screen))
        self.assertEqual(self.opt.state(), original.state())


if __name__ == "__main__":
    unittest.main()
