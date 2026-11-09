"""Compares `examples/match_dictionary_word.c` under --optimize with the
hand-written routine it translates, from the Zork I (r15) Z-machine
interpreter (https://github.com/XekriRedmane/zorki-r15-reveng).

Both look words up in the same 40-entry dictionary with py65, and
each is timed from entry to RTS, including the call to
`get_dictionary_addr`. The lookups cover entries on both sides of each
16-entry stride of the coarse search, the first and last entries, and
words that aren't there: below the first entry, between two entries,
and past the last.

The compiled function is 112 instructions against the original's 107,
and takes 4.0% more cycles over these lookups. Most of what's left:

- `get_dictionary_addr` returns the pointer in HARGS, and the caller
  copies it to a zero-page pair before using it as a `(zp),Y` base.
  The original leaves it in SCRATCH2.
- The original reads the entry length and count with Y = n + 1 ..
  n + 3, where the C adds n to the pointer once and uses `LDY #1` ..
  `LDY #3`.
- The high byte of the entry count goes through a temp.
- The result comes back in A and X, so the found path subtracts into
  a temp and loads it back. The original leaves it in SCRATCH1.

The original also keeps the header pointer and the word in zero page,
which c6502 can't do with a global yet (see TODO.md)."""

from __future__ import annotations

import shutil
import unittest

from py65.devices.mpu6502 import MPU

import sim.harness as H
from tests.asm_compare import (
    REPO, assemble, call, count_function_instructions,
)

_EXAMPLE = REPO / "examples" / "match_dictionary_word.c"
_HEADER_PAGE = 0x40          # the header at $4000
_DICT_OFFSET = 0x0100        # big-endian at header+$08
_N_ENTRIES = 40
_ENTRY_LEN = 7
_SEPARATORS = [0x2E, 0x2C, 0x22]
# The entries start after the separator count, the separators, the
# entry length, and the two-byte entry count.
_FIRST_ENTRY = _DICT_OFFSET + 1 + len(_SEPARATORS) + 3


def _key(i: int) -> int:
    """Entry i's first four bytes as a 32-bit big-endian value. Pairs
    of entries share a first byte, so the comparison of the later
    bytes is reached."""
    return ((i // 2) << 24) | ((i % 2) * 0x030000) | (i << 8) | 0x40


_LOOKUPS = (
    [_key(i) for i in (0, 1, 2, 15, 16, 17, 31, 32, 33, 39)]
    + [_key(0) - 1, _key(20) + 1, _key(39) + 1]
)

# Zero page as documented: Z_HEADER_ADDR $BA, ZCHAR_SCRATCH2 $DA,
# SCRATCH1 $E4, SCRATCH2 $E6, SCRATCH3 $E8. The macros are expanded,
# with numbered labels for their local `.continue`s.
_ORIGINAL = [
    "JSR get_dictionary_addr",
    "LDY #$00", "LDA ($E6),Y", "TAY", "INY", "LDA ($E6),Y",
    "ASL A", "ASL A", "ASL A", "ASL A", "STA $E8",
    "INY", "LDA ($E6),Y", "STA $E5", "INY", "LDA ($E6),Y", "STA $E4",
    "INY", "TYA",
    "CLC", "ADC $E6", "STA $E6", "BCC c1", "INC $E7",
    "c1: LDY #$00", "JMP try_match",
    "loop: LDA ($E6),Y", "CMP $DB", "BCS possible",
    "try_match: CLC", "LDA $E6", "ADC $E8", "STA $E6", "BCC c2", "INC $E7",
    "c2: SEC", "LDA $E4", "SBC #$10", "STA $E4", "BCS loop",
    "DEC $E5", "BPL loop",
    "possible: SEC", "LDA $E6", "SBC $E8", "STA $E6", "BCS c3", "DEC $E7",
    "c3: CLC", "LDA $E4", "ADC #$10", "STA $E4", "BCC c4", "INC $E5",
    "c4: LDA $E8", "LSR A", "LSR A", "LSR A", "LSR A", "STA $E8",
    "inner_loop: LDY #$00", "LDA $DB", "CMP ($E6),Y",
    "BCC not_found", "BNE inner_next",
    "INY", "LDA $DA", "CMP ($E6),Y", "BCC not_found", "BNE inner_next",
    "LDY #$02", "LDA $DD", "CMP ($E6),Y", "BCC not_found",
    "BNE inner_next",
    "INY", "LDA $DC", "CMP ($E6),Y", "BCC not_found", "BEQ found",
    "inner_next: CLC", "LDA $E6", "ADC $E8", "STA $E6", "BCC c5",
    "INC $E7",
    "c5: LDA $E4", "SEC", "SBC #$01", "STA $E4", "BCS c6", "DEC $E5",
    "c6: LDA $E4", "ORA $E5", "BNE inner_loop",
    "not_found: LDA #$00", "STA $E5", "STA $E4", "RTS",
    "found: SEC", "LDA $E6", "SBC $BA", "STA $E4", "LDA $E7", "SBC $BB",
    "STA $E5", "RTS",
    "get_dictionary_addr: LDY #$08", "LDA ($BA),Y", "STA $E7", "INY",
    "LDA ($BA),Y", "STA $E6",
    "CLC", "LDA $E6", "ADC $BA", "STA $E6", "LDA $E7", "ADC $BB",
    "STA $E7", "RTS",
]
# The routine proper, up to get_dictionary_addr.
_ORIGINAL_COUNT = _ORIGINAL.index("get_dictionary_addr: LDY #$08")
_ORIGIN = 0x1000


def _expected(key: int) -> int:
    for i in range(_N_ENTRIES):
        if _key(i) == key:
            return _FIRST_ENTRY + _ENTRY_LEN * i
    return 0


def _header(mem) -> None:
    base = _HEADER_PAGE << 8
    mem[base + 0x08] = _DICT_OFFSET >> 8
    mem[base + 0x09] = _DICT_OFFSET & 0xFF
    d = base + _DICT_OFFSET
    mem[d] = len(_SEPARATORS)
    for k, s in enumerate(_SEPARATORS):
        mem[d + 1 + k] = s
    p = d + 1 + len(_SEPARATORS)
    mem[p] = _ENTRY_LEN
    mem[p + 1] = _N_ENTRIES >> 8
    mem[p + 2] = _N_ENTRIES & 0xFF
    for i in range(_N_ENTRIES):
        e = base + _FIRST_ENTRY + _ENTRY_LEN * i
        mem[e:e + 4] = list(_key(i).to_bytes(4, "big"))
        mem[e + 4:e + _ENTRY_LEN] = [0xA5] * (_ENTRY_LEN - 4)


def _word_bytes(key: int) -> list[int]:
    """ZCHAR_SCRATCH2's first four bytes: the two big-endian words of
    the key, each stored little-endian."""
    b = key.to_bytes(4, "big")
    return [b[1], b[0], b[3], b[2]]


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestMatchDictionaryWord(unittest.TestCase):

    @classmethod
    def setUpClass(cls) -> None:
        src = _EXAMPLE.read_text()
        cls.compiled_count = count_function_instructions(
            _EXAMPLE, "match_dictionary_word",
        )
        cls.sim = H.build_sim(src, optimize=True)

    def _run_original(self, key: int) -> tuple[int, int]:
        mpu = MPU()
        assemble(mpu, _ORIGINAL, _ORIGIN)
        _header(mpu.memory)
        mpu.memory[0xBA], mpu.memory[0xBB] = 0x00, _HEADER_PAGE
        mpu.memory[0xDA:0xDE] = _word_bytes(key)
        cycles = call(mpu, _ORIGIN)
        return cycles, mpu.memory[0xE4] | (mpu.memory[0xE5] << 8)

    def _run_compiled(self, key: int) -> tuple[int, int]:
        mpu, sym = self.sim.mpu, self.sim.symbols
        mem = mpu.memory
        _header(mem)
        mem[sym["z_header_addr"]] = 0x00
        mem[sym["z_header_addr"] + 1] = _HEADER_PAGE
        w = sym["zchar_scratch2"]
        mem[w:w + 4] = _word_bytes(key)
        cycles = call(mpu, sym["match_dictionary_word"])
        return cycles, mpu.a | (mpu.x << 8)

    def test_results_match_original(self) -> None:
        for key in _LOOKUPS:
            with self.subTest(key=hex(key)):
                self.assertEqual(self._run_original(key)[1], _expected(key))
                self.assertEqual(self._run_compiled(key)[1], _expected(key))

    def test_size_within_five_percent(self) -> None:
        self.assertLessEqual(
            abs(self.compiled_count - _ORIGINAL_COUNT),
            0.05 * _ORIGINAL_COUNT,
            msg=f"compiled {self.compiled_count}, original {_ORIGINAL_COUNT}",
        )

    def test_cycles_no_more_than_five_percent_over(self) -> None:
        original = sum(self._run_original(k)[0] for k in _LOOKUPS)
        compiled = sum(self._run_compiled(k)[0] for k in _LOOKUPS)
        self.assertLessEqual(
            compiled, 1.05 * original,
            msg=f"compiled {compiled}, original {original}",
        )


if __name__ == "__main__":
    unittest.main()
