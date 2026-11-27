"""A stand-in Apple II for Broderbund's Lode Runner, for running the
original game and examples/lode_runner on the same machine and
comparing what each does.

The game talks to the machine only through the soft switches and
DOS's RWTS, so those are what the machine records. The soft switches
are the keyboard (KBD at $C000, KBDSTRB at $C010), the speaker (SPKR
at $C030), the display switches ($C050-$C057), the paddle buttons
(BUTN0 and BUTN1 at $C061 and $C062), the paddles (PADDL0 and PADDL1
at $C064 and $C065, started by PTRIG at $C070), and the language
card's $C081. Every access to $C000-$C0FF is an event, and so is each
call of RWTS, which is a Python hook at DISABLE_INTS_CALL_RWTS ($B7B5)
working on a copy of the game's disk image (DOS order).

Each event records the address (or RWTS), whether it was a read or a
write, and CRCs of the two high-resolution pages as they were when
the access happened. Page 2's CRC covers $4000-$5EFF, since the
original's code starts at $5F00, inside the page. It also records
the program counter of the instruction and the cycle count, which
differ between the two programs and only help find where a
difference comes from.

The keyboard follows a script of (n, key) pairs. The key goes into
KBD's latch, with bit 7 set, just before the program's KBD read
number n (counting from 0), so that both programs see a key at the
same point in their polling. Bit 7 stays set until the program reads
or writes KBDSTRB, as on the Apple II, and the key's code stays in
the low seven bits after that. No paddle button is ever down, and no
joystick is attached. Each paddle reads with bit 7 set, as a paddle
whose timer never runs out does, which DETECT_LACK_OF_JOYSTICK takes
for a missing joystick.

`Machine.run` steps the MPU until a cycle limit or until its `stop`
predicate returns true for an event. The soft switches are found by
decoding each instruction before it runs. Only the absolute and
indirect addressing modes can reach $C0xx, so the check is a table
lookup for most instructions. RWTS takes no cycles.
"""
from __future__ import annotations

import zlib
from array import array
from dataclasses import dataclass, field
from typing import Callable

from py65.devices.mpu6502 import MPU

from binutils.opcodes import DECODE
from sim.apple2 import dsk_sector

KBD = 0xC000
KBDSTRB = 0xC010
SPKR = 0xC030
BUTN0, BUTN1 = 0xC061, 0xC062
PADDL0, PADDL1 = 0xC064, 0xC065
PTRIG = 0xC070

# DISABLE_INTS_CALL_RWTS, the entry every RWTS call of the game goes
# through, and DOS's IOB.
RWTS_ENTRY = 0xB7B5
DOS_IOB = 0xB7E8

# The bytes of each high-resolution page that an event's CRC covers.
# Page 2's last 256 bytes are the original's code.
_PAGES = {1: (0x2000, 0x4000), 2: (0x4000, 0x5F00)}

# The code of an RWTS event, in place of a soft-switch address.
RWTS_EVENT = 0x10000

# Where `Machine.call` makes a subroutine return to. Nothing of the
# game's is there, since it's in the Monitor's ROM.
_RETURN_TRAP = 0xFFF0

# The addressing modes that can reach a soft switch, numbered for the
# run loop. 0 is every other mode.
_ABS, _ABSX, _ABSY, _INDX, _INDY = 1, 2, 3, 4, 5
_MODES = {"abs": _ABS, "absx": _ABSX, "absy": _ABSY,
          "indx": _INDX, "indy": _INDY}
_WRITES = {"STA", "STX", "STY"}
_READ_WRITES = {"INC", "DEC", "ASL", "LSR", "ROL", "ROR"}
_NO_ACCESS = {"JMP", "JSR"}


def _mode_table() -> tuple[bytes, bytes]:
    """For each opcode, its addressing mode (one of the numbers above,
    or 0) and its access: 1 read, 2 write, 3 both."""
    modes = bytearray(256)
    access = bytearray(256)
    for op, (mnemonic, mode) in DECODE.items():
        if mode not in _MODES or mnemonic in _NO_ACCESS:
            continue
        modes[op] = _MODES[mode]
        access[op] = (2 if mnemonic in _WRITES
                      else 3 if mnemonic in _READ_WRITES else 1)
    return bytes(modes), bytes(access)


_OP_MODE, _OP_ACCESS = _mode_table()


@dataclass
class Events:
    """The event stream of one run, one entry per event in each array.
    `code` is the soft switch's address times 2, plus 1 for a write
    (an RWTS call is `RWTS_EVENT` times 2), and `page1` and `page2`
    are the CRCs of the two pages. `rwts` maps the index of each RWTS
    event to its command, track, sector, and the buffer's 256 bytes
    after the call."""

    code: array = field(default_factory=lambda: array("L"))
    page1: array = field(default_factory=lambda: array("L"))
    page2: array = field(default_factory=lambda: array("L"))
    pc: array = field(default_factory=lambda: array("H"))
    cycles: array = field(default_factory=lambda: array("Q"))
    rwts: dict[int, tuple[int, int, int, bytes]] = field(
        default_factory=dict)

    def __len__(self) -> int:
        return len(self.code)

    def describe(self, i: int) -> str:
        """Event `i` as text."""
        code = self.code[i]
        if code >> 1 == RWTS_EVENT:
            command, track, sector, _ = self.rwts[i]
            what = f"RWTS command {command} T${track:02X} S${sector:X}"
        else:
            rw = "write" if code & 1 else "read"
            what = f"{rw} ${code >> 1:04X}"
        return (f"#{i} {what} at PC ${self.pc[i]:04X}, cycle "
                f"{self.cycles[i]}, pages {self.page1[i]:08X} "
                f"{self.page2[i]:08X}")


def first_difference(a: Events, b: Events) -> int | None:
    """The index of the first event at which the two streams differ,
    in the access, the pages, or an RWTS call's arguments and buffer,
    or None when one is a prefix of the other."""
    n = min(len(a), len(b))
    for name in ("code", "page1", "page2"):
        x, y = getattr(a, name), getattr(b, name)
        if x[:n] != y[:n]:
            n = next(i for i in range(n) if x[i] != y[i])
    for i in sorted(set(a.rwts) | set(b.rwts)):
        if i < n and a.rwts.get(i) != b.rwts.get(i):
            n = i
            break
    return n if n < min(len(a), len(b)) else None


@dataclass
class Machine:
    """The MPU and its soft switches. `keys` is the keyboard script,
    (n, key) pairs with the key in ASCII. `disk` is the disk image,
    which RWTS reads and writes in place."""

    mpu: MPU
    disk: bytearray
    keys: list[tuple[int, int]] = field(default_factory=list)
    events: Events = field(default_factory=Events)
    kbd_reads: int = 0
    kbd_latch: int = 0
    # The display switches as they were last set.
    text: bool = True
    mixed: bool = False
    page2: bool = False
    hires: bool = False
    # Python routines run in place of the code at their addresses,
    # each followed by an RTS. One that returns a string ends `run`.
    hooks: dict[int, Callable[[], str | None]] = field(
        default_factory=dict)

    def __post_init__(self) -> None:
        self._keys = dict(self.keys)
        self.hooks.setdefault(RWTS_ENTRY, self._rwts)
        self._copies: dict[int, bytes | None] = {1: None, 2: None}
        self._crcs = {1: 0, 2: 0}

    # ---- Soft switches -------------------------------------------------

    def _page_crc(self, page: int) -> int:
        """The CRC of hi-res page 1 or 2. A page's CRC is computed again
        only when its bytes differ from the copy kept with the last
        one, since comparing takes about a tenth of the time."""
        mem = self.mpu.memory
        start, end = _PAGES[page]
        copy = self._copies[page]
        if copy is None or not mem.startswith(copy, start, end):
            copy = bytes(mem[start:end])
            self._copies[page] = copy
            self._crcs[page] = zlib.crc32(copy)
        return self._crcs[page]

    def _record(self, code: int, pc: int) -> None:
        ev = self.events
        p1, p2 = self._page_crc(1), self._page_crc(2)
        ev.code.append(code)
        ev.page1.append(p1)
        ev.page2.append(p2)
        ev.pc.append(pc)
        ev.cycles.append(self.mpu.processorCycles)

    def _access(self, addr: int, access: int, pc: int) -> None:
        """Records an access to soft switch `addr` by the instruction at
        `pc`, and puts the value a read returns at `addr`."""
        mem = self.mpu.memory
        if access & 1:
            self._record(addr << 1, pc)
        if access & 2:
            self._record(addr << 1 | 1, pc)
        page = addr & 0xF0
        if page == 0x00:
            if access & 1:
                key = self._keys.get(self.kbd_reads)
                if key is not None:
                    self.kbd_latch = key | 0x80
                self.kbd_reads += 1
            mem[addr] = self.kbd_latch
        elif page == 0x10:
            self.kbd_latch &= 0x7F
            mem[addr] = self.kbd_latch
        elif page == 0x50:
            switch = addr & 0x0F
            if switch < 8:
                on = bool(switch & 1)
                if switch < 2:
                    self.text = on
                elif switch < 4:
                    self.mixed = on
                elif switch < 6:
                    self.page2 = on
                else:
                    self.hires = on
            mem[addr] = 0
        elif addr in (PADDL0, PADDL1):
            mem[addr] = 0x80
        else:
            mem[addr] = 0

    # ---- DOS -----------------------------------------------------------

    def _rwts(self) -> None:
        mpu = self.mpu
        m = mpu.memory
        iob = (mpu.a << 8) | mpu.y
        track, sector = m[iob + 4], m[iob + 5]
        buf = m[iob + 8] | m[iob + 9] << 8
        command = m[iob + 12]
        ok = track < 35 and sector < 16 and command in (1, 2)
        if ok:
            at = dsk_sector(self.disk, track, sector)
            if command == 1:
                m[buf:buf + 256] = self.disk[at:at + 256]
            else:
                self.disk[at:at + 256] = bytes(m[buf:buf + 256])
        self._record(RWTS_EVENT << 1, mpu.pc)
        self.events.rwts[len(self.events) - 1] = (
            command, track, sector, bytes(m[buf:buf + 256]),
        )
        m[iob + 13] = 0 if ok else 0x40     # the return code
        if ok:
            mpu.p &= ~0x01
        else:
            mpu.p |= 0x01

    # ---- Running -------------------------------------------------------

    def run(self, max_cycles: int,
            stop: Callable[["Machine"], bool] | None = None,
            recorder: Callable | None = None) -> str:
        """Steps the MPU until `max_cycles` have run ("cycles"), or until
        `stop`, called after each event, returns true ("stop"), or the
        program halts with a `BRK` ("brk"). A hook ends the run when it
        returns a string, which `run` returns. `recorder(mpu, label)`
        returns the object whose `before` and `after` the loop calls
        around each instruction, or None for none. The default,
        `sim.lockstep.recorder`, runs the same instructions on the C++
        CPU with `C6502_SIM_LOCKSTEP=1`."""
        if recorder is None:
            from sim import lockstep
            recorder = lockstep.recorder
        rec = recorder(self.mpu, "lode_runner")
        try:
            return self._run(max_cycles, stop, rec)
        finally:
            if rec is not None:
                rec.finish()

    def _run(self, max_cycles: int,
             stop: Callable[["Machine"], bool] | None, rec) -> str:
        mpu = self.mpu
        mem = mpu.memory
        hooks = self.hooks
        instruct = mpu.instruct
        cycletime = mpu.cycletime
        extracycles = mpu.extracycles
        op_mode = _OP_MODE
        op_access = _OP_ACCESS
        events = self.events
        n_events = len(events)
        while mpu.processorCycles < max_cycles:
            pc = mpu.pc
            hook = hooks.get(pc)
            if hook is not None:
                why = hook()
                if why is not None:
                    return why
                # Return as RTS would.
                sp = mpu.sp
                lo = mem[0x0100 + ((sp + 1) & 0xFF)]
                hi = mem[0x0100 + ((sp + 2) & 0xFF)]
                mpu.sp = (sp + 2) & 0xFF
                mpu.pc = (((hi << 8) | lo) + 1) & 0xFFFF
            else:
                op = mem[pc]
                if op == 0x00:
                    return "brk"
                mode = op_mode[op]
                if mode:
                    if mode == _ABS:
                        ea = mem[pc + 1] | mem[pc + 2] << 8
                    elif mode == _ABSX:
                        ea = (mem[pc + 1] | mem[pc + 2] << 8) + mpu.x
                    elif mode == _ABSY:
                        ea = (mem[pc + 1] | mem[pc + 2] << 8) + mpu.y
                    elif mode == _INDY:
                        zp = mem[pc + 1]
                        ea = (mem[zp] | mem[(zp + 1) & 0xFF] << 8) + mpu.y
                    else:
                        zp = (mem[pc + 1] + mpu.x) & 0xFF
                        ea = mem[zp] | mem[(zp + 1) & 0xFF] << 8
                    if ea >> 8 == 0xC0:
                        self._access(ea, op_access[op], pc)
                if rec is not None:
                    rec.before()
                mpu.pc = (pc + 1) & 0xFFFF
                mpu.excycles = 0
                mpu.addcycles = extracycles[op]
                instruct[op](mpu)
                mpu.pc &= 0xFFFF
                mpu.processorCycles += cycletime[op] + mpu.excycles
                if rec is not None:
                    rec.after()
            if stop is not None and len(events) != n_events:
                n_events = len(events)
                if stop(self):
                    return "stop"
        return "cycles"

    def call(self, addr: int, max_cycles: int = 10_000_000,
             recorder: Callable | None = None) -> None:
        """Runs the subroutine at `addr` as a `JSR` would, until it
        returns, recording with `recorder` as `run` does."""
        mpu = self.mpu
        back = _RETURN_TRAP - 1
        mem = mpu.memory
        mem[0x0100 + mpu.sp] = back >> 8
        mem[0x0100 + ((mpu.sp - 1) & 0xFF)] = back & 0xFF
        mpu.sp = (mpu.sp - 2) & 0xFF
        mpu.pc = addr
        self.hooks[_RETURN_TRAP] = lambda: "returned"
        try:
            why = self.run(mpu.processorCycles + max_cycles,
                           recorder=recorder)
        finally:
            del self.hooks[_RETURN_TRAP]
        if why != "returned":
            raise RuntimeError(f"${addr:04X} didn't return ({why})")

    # ---- Looking at it -------------------------------------------------

    def screen(self, page: int = 1, width: int = 70) -> str:
        """The high-resolution page as text, a character for each block
        of pixels, `#` where at least half of them are on."""
        mem = self.mpu.memory
        base = 0x2000 if page == 1 else 0x4000
        bw = 280 // width
        bh = bw * 2
        rows = []
        for y0 in range(0, 192, bh):
            line = []
            for x0 in range(0, 280, bw):
                on = 0
                for y in range(y0, min(y0 + bh, 192)):
                    addr = (base + (y & 7) * 0x400 + ((y >> 3) & 7) * 0x80
                            + (y >> 6) * 0x28)
                    for x in range(x0, x0 + bw):
                        on += mem[addr + x // 7] >> (x % 7) & 1
                line.append("#" if on * 2 >= bw * bh else
                            "+" if on else " ")
            rows.append("".join(line).rstrip())
        return "\n".join(rows)

    def level_map(self) -> str:
        """LEVEL_MAP ($0800) as text, a character for each sprite."""
        chars = " #=H-^h$GP"
        mem = self.mpu.memory
        rows = []
        for r in range(16):
            row = mem[0x0800 + r * 28:0x0800 + r * 28 + 28]
            rows.append("".join(
                chars[c] if c < len(chars) else "?" for c in row))
        return "\n".join(rows)


# ---- Setting up the two programs ------------------------------------------

# MAIN, where the original starts, and the original's code, which the C
# program's image has cleared before it's loaded.
MAIN = 0x5F7D
CODE_START, CODE_END = 0x5F00, 0x8E53


def original_machine(image: bytes, disk: bytes,
                     keys: list[tuple[int, int]]) -> Machine:
    """The original, `image` being its 48K memory image (main.bin of
    the reverse-engineering project), ready to run from MAIN, which
    sets the stack pointer itself."""
    mpu = MPU()
    mem = bytearray(0x10000)
    mem[:len(image)] = image
    mpu.memory = mem
    mpu.pc = MAIN
    return Machine(mpu, bytearray(disk), list(keys))


def program_symbols(exe) -> dict[str, int]:
    """The defined symbols of the linked executable `exe` (an
    `ElfFile`), by name."""
    from binutils import elf as E
    out: dict[str, int] = {}
    for s in exe.symbols:
        if s.defined and s.type != E.STT_FILE:
            if s.is_global or s.name not in out:
                out[s.name] = s.value
    return out


def program_machine(image: bytes, exe, disk: bytes,
                    keys: list[tuple[int, int]],
                    recorder: Callable | None = None) -> Machine:
    """The linked C program `exe` (an `ElfFile`) loaded over the
    original's image with the original's code cleared, ready to run
    from `main`. Each segment's bytes past its file size are zeroed,
    as a loader would. The stack pointer is $07, where MAIN puts it
    with `LDX #$07; TXS`, which C can't do. `__zp_init`, if the
    program has one, has already run, recorded with `recorder` as
    `Machine.run` records."""
    mpu = MPU()
    mem = bytearray(0x10000)
    mem[:len(image)] = image
    mem[CODE_START:CODE_END] = bytes(CODE_END - CODE_START)
    for seg in exe.segments:
        mem[seg.paddr:seg.paddr + seg.memsz] = (
            seg.data + bytes(seg.memsz - len(seg.data)))
    mpu.memory = mem
    mpu.sp = 0x07
    symbols = program_symbols(exe)
    machine = Machine(mpu, bytearray(disk), list(keys))
    if "__zp_init" in symbols:
        machine.call(symbols["__zp_init"], recorder=recorder)
    mpu.pc = symbols["main"]
    return machine


_BRANCHES = {"BCC", "BCS", "BEQ", "BMI", "BNE", "BPL", "BVC", "BVS"}


def original_routines(main_asm: str) -> dict[int, str]:
    """The address of each global label of the original's main.asm (one
    that doesn't start with a dot), so that the nearest one at or below
    a program counter names the routine it's in. The addresses come
    from sizing each instruction and directive after each ORG as dasm
    would. An operand that's an EQU below $100 takes the zero-page
    form, and anything else, a label of the code included, takes the
    absolute form."""
    import re
    lines = [ln.split(";")[0].rstrip() for ln in main_asm.splitlines()]
    equs: dict[str, int] = {}
    for ln in lines:
        m = re.match(r"([A-Za-z_][A-Za-z0-9_]*)\s+EQU\s+#?\$([0-9A-Fa-f]+)",
                     ln)
        if m:
            equs[m.group(1)] = int(m.group(2), 16)

    def value(expr: str) -> int | None:
        m = re.match(r"\$([0-9A-Fa-f]+)$|([A-Za-z_][A-Za-z0-9_]*)"
                     r"(?:\+(\d+|\$[0-9A-Fa-f]+))?$", expr)
        if not m:
            return None
        if m.group(1):
            return int(m.group(1), 16)
        base = equs.get(m.group(2))
        if base is None:
            return None
        off = m.group(3) or "0"
        return base + (int(off[1:], 16) if off[0] == "$" else int(off))

    def size(mnemonic: str, operand: str) -> int:
        if mnemonic in _BRANCHES:
            return 2
        if mnemonic in ("JMP", "JSR"):
            return 3
        if not operand or operand == "A":
            return 1
        if operand.startswith("#"):
            return 2
        if operand.startswith("("):
            return 3 if mnemonic == "JMP" else 2
        v = value(operand.split(",")[0])
        return 2 if v is not None and v < 0x100 else 3

    out: dict[int, str] = {}
    pc = None
    for ln in lines:
        if not ln.strip():
            continue
        if not ln[0].isspace():
            name = ln.split()[0].rstrip(":")
            rest = ln[len(ln.split()[0]):].strip()
            if rest.startswith("EQU"):
                continue
            if pc is not None and not name.startswith("."):
                out.setdefault(pc, name)
            ln = " " + rest
            if not rest:
                continue
        parts = ln.split(None, 1)
        op = parts[0].upper()
        arg = parts[1].strip() if len(parts) > 1 else ""
        if op == "ORG":
            pc = int(arg.lstrip("$"), 16)
        elif pc is None or op in ("SUBROUTINE", "PROCESSOR", "INCLUDE"):
            continue
        elif op == "HEX":
            pc += len(arg.split())
        elif op == "BYTE":
            pc += len(arg.split(","))
        elif op == "WORD":
            pc += 2 * len(arg.split(","))
        elif op == "DS":
            pc += int(arg.lstrip("$"), 16 if arg.startswith("$") else 10)
        else:
            pc += size(op, arg.replace(" ", ""))
    return out


def routine_at(routines: dict[int, str], pc: int) -> str:
    """The name of the routine in `routines` (address to name) that
    `pc` is in, as name+offset."""
    below = [a for a in routines if a <= pc]
    if not below:
        return f"${pc:04X}"
    a = max(below)
    return f"{routines[a]}+{pc - a}" if pc != a else routines[a]
