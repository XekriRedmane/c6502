"""Writes the captures of py65 1.2.0 that cpp/tests/test_cpu.cpp checks
the C++ CPU (cpp/src/sim/cpu.h) against, in cpp/tests/frozen/cpu/.
They stay the reference once py65 is gone, so this tool runs only while
py65 is installed. Run it from the repository's root with
`uv run python tools/py65_cpu_vectors.py`.

vectors.txt starts with py65's two cycle tables, a hex digit per opcode
from $00 to $FF:

    cycletime <256 digits>
    extracycles <256 digits>

Then each line is one `MPU.step` from a random state:

    <op> <pc> <a> <x> <y> <sp> <p> <cycles> | <ram> | <reads> | <after> | <writes>

The state before is in hex, with the cycle count in 64 bits. `<ram>` is
every byte that the step read before writing it, as `addr:value`, and
the rest of memory is zero. `<reads>` is the address of every read in
py65's order, the opcode's fetch first. `<after>` is `<pc> <a> <x> <y>
<sp> <p> +<cycles taken>`, and `<writes>` is every write in order, as
`addr:value`. A step that reads a word at $FFFF, which py65's `WordAt`
reads past the end of its memory, ends `| error` after `<reads>`.

Each defined opcode gets random states and then states that force each
case of its addressing mode: a page crossed and not crossed by abs,X,
abs,Y, and (zp),Y, a zero-page index that wraps, a pointer at $FF, a
branch taken and not taken, across a page and not, the stack pointer at
each end, and the instruction at the end of memory. ADC and SBC run in
decimal mode in half of the random states. Each undefined opcode gets a
few random states.

decimal.txt has one line for each of ADC and SBC in decimal mode, each
carry, and each accumulator:

    <ADC|SBC> <carry> <a> <crc>

where `<crc>` is the CRC-32 of the 512 bytes A, P after `ADC #b` (or
`SBC #b`) for b from 0 to 255, from P = D | bit 5 | carry.
"""
from __future__ import annotations

import random
import sys
import zlib
from pathlib import Path

from py65.devices.mpu6502 import MPU

OUT = Path(__file__).resolve().parent.parent / "cpp" / "tests" / "frozen" / "cpu"

RANDOM_STEPS = 12
UNDEFINED_STEPS = 3


class _Memory(list):
    """py65's memory, whose bytes take a random value when first read.
    It records each read, each write, and the first value of each byte
    read before it was written."""

    def __init__(self, rng: random.Random) -> None:
        super().__init__([None] * 0x10000)
        self.rng = rng
        self.initial: dict[int, int] = {}
        self.reads: list[int] = []
        self.writes: list[tuple[int, int]] = []

    def preset(self, addr: int, value: int) -> None:
        addr &= 0xFFFF
        if list.__getitem__(self, addr) is None:
            list.__setitem__(self, addr, value & 0xFF)
            self.initial[addr] = value & 0xFF

    def __getitem__(self, addr):
        if not isinstance(addr, int) or not 0 <= addr < 0x10000:
            raise IndexError(addr)
        self.reads.append(addr)
        value = list.__getitem__(self, addr)
        if value is None:
            value = self.rng.randrange(256)
            list.__setitem__(self, addr, value)
            self.initial[addr] = value
        return value

    def __setitem__(self, addr, value) -> None:
        if not isinstance(addr, int) or not 0 <= addr < 0x10000:
            raise IndexError(addr)
        self.writes.append((addr, value))
        list.__setitem__(self, addr, value)


_FLAG_OF_BRANCH = {
    0x10: (MPU.NEGATIVE, False), 0x30: (MPU.NEGATIVE, True),
    0x50: (MPU.OVERFLOW, False), 0x70: (MPU.OVERFLOW, True),
    0x90: (MPU.CARRY, False), 0xB0: (MPU.CARRY, True),
    0xD0: (MPU.ZERO, False), 0xF0: (MPU.ZERO, True),
}


def _size(mode: str) -> int:
    if mode in ("imp", "acc"):
        return 1
    if mode in ("abs", "abx", "aby", "ind"):
        return 3
    return 2


class _Case:
    """The registers and memory a step starts from, before the random
    fill. A field left None is drawn at random."""

    def __init__(self, **kw) -> None:
        self.pc = kw.get("pc")
        self.a = kw.get("a")
        self.x = kw.get("x")
        self.y = kw.get("y")
        self.sp = kw.get("sp")
        self.p = kw.get("p")
        self.bytes: dict[int, int] = kw.get("bytes", {})
        # Bytes at offsets from the instruction's address.
        self.operand: list[int] = kw.get("operand", [])


def _forced_cases(op: int, name: str, mode: str, rng: random.Random) -> list[_Case]:
    r = rng.randrange
    cases: list[_Case] = []
    if mode in ("abx", "aby"):
        for cross in (True, False, True, False):
            index = r(1, 256) if cross else r(0, 128)
            low = r(256 - index, 256) if cross else r(0, 256 - index)
            reg = {"x": index} if mode == "abx" else {"y": index}
            cases.append(_Case(operand=[low, r(256)], **reg))
        # The address wraps past $FFFF.
        reg = {"x": 0xFF} if mode == "abx" else {"y": 0xFF}
        cases.append(_Case(operand=[0xF0, 0xFF], **reg))
    if mode == "iny":
        for cross in (True, False, True, False):
            index = r(1, 256) if cross else r(0, 128)
            low = r(256 - index, 256) if cross else r(0, 256 - index)
            zp = r(256)
            cases.append(_Case(operand=[zp], y=index,
                               bytes={zp: low, (zp + 1) & 0xFF: r(256)}))
        cases.append(_Case(operand=[0xFF], y=r(256)))
        cases.append(_Case(operand=[0x10], y=0xFF, bytes={0x10: 0xF0, 0x11: 0xFF}))
    if mode == "inx":
        cases.append(_Case(operand=[r(0x80, 0x100)], x=r(0x80, 0x100)))
        cases.append(_Case(operand=[0xFF], x=0))
        cases.append(_Case(operand=[0xFE], x=1))
    if mode in ("zpx", "zpy"):
        reg = "x" if mode == "zpx" else "y"
        cases.append(_Case(operand=[r(0x80, 0x100)], **{reg: r(0x80, 0x100)}))
        cases.append(_Case(operand=[0xFF], **{reg: 1}))
    if mode == "ind":
        cases.append(_Case(operand=[0xFF, r(256)]))
        cases.append(_Case(operand=[0xFF, 0xFF]))
    if mode == "rel":
        flag, when_set = _FLAG_OF_BRANCH[op]
        for taken in (True, False):
            for offset in (0x7F, 0x80, 0x00, 0x05, 0xFB):
                p = r(256)
                if taken == when_set:
                    p |= flag
                else:
                    p &= ~flag
                cases.append(_Case(operand=[offset], p=p))
        # A taken branch from the last page into the first, and back.
        p_taken = (r(256) | flag) if when_set else (r(256) & ~flag)
        cases.append(_Case(pc=0xFFF0, operand=[0x20], p=p_taken))
        cases.append(_Case(pc=0x0004, operand=[0xF0], p=p_taken))
        cases.append(_Case(pc=0xFFFE, operand=[0x01], p=p_taken))
    if name in ("PHA", "PHP", "JSR", "BRK"):
        cases.append(_Case(sp=0x00))
        cases.append(_Case(sp=0x01))
    if name in ("PLA", "PLP", "RTS", "RTI"):
        cases.append(_Case(sp=0xFF))
        cases.append(_Case(sp=0xFE))
    if name in ("ADC", "SBC"):
        for carry in (0, 1):
            cases.append(_Case(p=(r(256) | MPU.DECIMAL) & ~1 | carry))
            cases.append(_Case(p=(r(256) & ~MPU.DECIMAL) & ~1 | carry))
    if name in ("CMP", "CPX", "CPY"):
        # Equal operands.
        v = r(256)
        if mode == "imm":
            cases.append(_Case(a=v, x=v, y=v, operand=[v]))
    if name in ("TXS",):
        cases.append(_Case(x=0x00))
    # The instruction at the end of memory. A two-byte operand at $FFFF
    # makes py65 read past the end, which is the `error` case.
    size = _size(mode)
    cases.append(_Case(pc=0xFFFF))
    if size == 3:
        cases.append(_Case(pc=0xFFFE))
        cases.append(_Case(pc=0xFFFD))
    elif size == 2:
        cases.append(_Case(pc=0xFFFE))
    return cases


def _random_case(op: int, name: str, rng: random.Random, decimal: bool) -> _Case:
    p = rng.randrange(256)
    if name in ("ADC", "SBC"):
        p = p | MPU.DECIMAL if decimal else p & ~MPU.DECIMAL
    return _Case(p=p)


def _step(op: int, case: _Case, rng: random.Random) -> str:
    mpu = MPU()
    mem = _Memory(rng)
    mpu.memory = mem
    r = rng.randrange
    mpu.pc = case.pc if case.pc is not None else r(0x10000)
    mpu.a = case.a if case.a is not None else r(256)
    mpu.x = case.x if case.x is not None else r(256)
    mpu.y = case.y if case.y is not None else r(256)
    mpu.sp = case.sp if case.sp is not None else r(256)
    mpu.p = case.p if case.p is not None else r(256)
    mpu.processorCycles = r(1 << 40)
    if rng.randrange(8) == 0:
        mpu.processorCycles = (1 << 64) - 1 - r(8)
    mem.preset(mpu.pc, op)
    for i, b in enumerate(case.operand):
        mem.preset(mpu.pc + 1 + i, b)
    for addr, b in case.bytes.items():
        mem.preset(addr, b)
    before = (f"{op:02X} {mpu.pc:04X} {mpu.a:02X} {mpu.x:02X} {mpu.y:02X} "
              f"{mpu.sp:02X} {mpu.p:02X} {mpu.processorCycles:016X}")
    start = mpu.processorCycles
    error = False
    try:
        mpu.step()
    except IndexError:
        error = True
    ram = " ".join(f"{a:04X}:{v:02X}" for a, v in sorted(mem.initial.items()))
    reads = " ".join(f"{a:04X}" for a in mem.reads)
    if error:
        return f"{before} | {ram} | {reads} | error"
    delta = mpu.processorCycles - start
    if delta < 0 or delta > 15:
        raise AssertionError(f"opcode {op:02X} took {delta} cycles")
    # py65's cycle count is a Python int, which passes 2**64 where the
    # C++ CPU's count wraps, so the vector gives the cycles taken.
    after =(f"{mpu.pc:04X} {mpu.a:02X} {mpu.x:02X} {mpu.y:02X} "
             f"{mpu.sp:02X} {mpu.p:02X} +{delta:X}")
    writes = " ".join(f"{a:04X}:{v:02X}" for a, v in mem.writes)
    return f"{before} | {ram} | {reads} | {after} | {writes}".rstrip()


def vectors(rng: random.Random) -> list[str]:
    proto = MPU()
    lines = [
        "cycletime " + "".join(f"{c:X}" for c in proto.cycletime),
        "extracycles " + "".join(f"{c:X}" for c in proto.extracycles),
    ]
    for op in range(256):
        name, mode = proto.disassemble[op]
        if proto.instruct[op].__name__ == "inst_not_implemented":
            cases = [_random_case(op, name, rng, False) for _ in range(UNDEFINED_STEPS)]
            cases.append(_Case(pc=0xFFFF))
        else:
            cases = [_random_case(op, name, rng, i % 2 == 0) for i in range(RANDOM_STEPS)]
            cases += _forced_cases(op, name, mode, rng)
        for case in cases:
            lines.append(_step(op, case, rng))
    return lines


def decimal_rows() -> list[str]:
    mpu = MPU()
    lines = []
    for name, opcode in (("ADC", 0x69), ("SBC", 0xE9)):
        for carry in (0, 1):
            for a in range(256):
                out = bytearray()
                for b in range(256):
                    mpu.memory[0x200] = opcode
                    mpu.memory[0x201] = b
                    mpu.pc = 0x200
                    mpu.a = a
                    mpu.p = mpu.DECIMAL | mpu.UNUSED | carry
                    mpu.step()
                    out += bytes((mpu.a, mpu.p))
                lines.append(f"{name} {carry} {a:02X} {zlib.crc32(out):08X}")
    return lines


def main() -> int:
    import py65
    version = getattr(py65, "__version__", None)
    OUT.mkdir(parents=True, exist_ok=True)
    rng = random.Random(6502)
    vec = vectors(rng)
    (OUT / "vectors.txt").write_text("\n".join(vec) + "\n", encoding="utf-8", newline="\n")
    dec = decimal_rows()
    (OUT / "decimal.txt").write_text("\n".join(dec) + "\n", encoding="utf-8", newline="\n")
    print(f"py65 {version}: {len(vec) - 2} vectors, {len(dec)} decimal rows",
          file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
