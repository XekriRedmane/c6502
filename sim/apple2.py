"""A stand-in Apple II for running a whole program in the simulator.
It has the Monitor's text routines, its keyboard and line input, a
printer, and DOS 3.3's RWTS, each a Python hook at the routine's
address.

The hooks keep a 40-by-24 text screen as the ROM would. COUT1 writes
a character at the cursor, inside the text window that WNDLFT,
WNDWDTH, WNDTOP, and WNDBTM set, and scrolls the window when the
cursor passes its bottom line. HOME and CLREOL clear. VTAB does
nothing, since the screen is kept by line and column. RDKEY and
GETLN1 take keys and lines from a script, and COUT goes to the screen
or to the printer, depending on CSW. RWTS reads and writes 256-byte
sectors of the disk images in `disks`, keyed by slot times 16 and
drive.

Every hook call is appended to `calls`, with the character it was
given, if any, and the cursor and INVFLG it saw, so that two programs
meant to behave the same can be compared call for call. The hooks
take no cycles.

`Apple2.run` steps the MPU until the program ends, at a `BRK`, a
`JMP` to itself (a program that has stopped), or a line input with
the script used up.
"""
from __future__ import annotations

from dataclasses import dataclass, field

from py65.devices.mpu6502 import MPU

HOME = 0xFC58
CLREOL = 0xFC9C
VTAB = 0xFC22
RDKEY = 0xFD0C
GETLN1 = 0xFD6F
COUT = 0xFDED
COUT1 = 0xFDF0
RWTS = 0x2900

# The printer's output hook. Zork puts it in CSW while it prints to the
# printer, and a card in slot 1 has its entry point there.
PRINTER = 0xC100

WNDLFT, WNDWDTH, WNDTOP, WNDBTM = 0x20, 0x21, 0x22, 0x23
CH, CV, INVFLG, CSW = 0x24, 0x25, 0x32, 0x36
RNDL = 0x4E

SPACE = 0xA0

# DOS 3.3's sector interleave: logical sector s of a track is physical
# sector SKEW[s], the order a .dsk image keeps them in.
SKEW = [0x0, 0xD, 0xB, 0x9, 0x7, 0x5, 0x3, 0x1,
        0xE, 0xC, 0xA, 0x8, 0x6, 0x4, 0x2, 0xF]


def dsk_sector(image: bytes | bytearray, track: int, sector: int) -> int:
    """The offset in a DOS-order .dsk image of logical `sector` of
    `track`."""
    return (track * 16 + SKEW[sector]) * 256


class Stop(Exception):
    """The program has ended."""

    def __init__(self, reason: str) -> None:
        super().__init__(reason)
        self.reason = reason


@dataclass
class Apple2:
    """One MPU with the stand-ins installed. `keys` are what RDKEY
    returns, without the high bit; when they run out it returns
    RETURN. `lines` are what GETLN1 reads, one per call."""

    mpu: MPU
    keys: list[int] = field(default_factory=list)
    lines: list[str] = field(default_factory=list)
    disks: dict[tuple[int, int], bytearray] = field(default_factory=dict)
    screen: list[bytearray] = field(
        default_factory=lambda: [bytearray([SPACE] * 40) for _ in range(24)],
    )
    printer: list[int] = field(default_factory=list)
    calls: list[tuple] = field(default_factory=list)
    # Bytes written to each disk, as (slot, drive, track, sector).
    written: list[tuple[int, int, int, int]] = field(default_factory=list)
    stop_reason: str = ""
    _lcg: int = 1

    def __post_init__(self) -> None:
        self.hooks = {
            HOME: self._home, CLREOL: self._clreol, VTAB: self._vtab,
            RDKEY: self._rdkey, GETLN1: self._getln1, COUT: self._cout,
            COUT1: self._cout1, RWTS: self._rwts,
        }

    # ---- Machine state ------------------------------------------------

    def boot_screen(self) -> None:
        """Sets the Monitor's zero page as INIT, SETVID, and SETKBD
        leave it: a full-screen window, normal text, and CSW at
        COUT1."""
        m = self.mpu.memory
        m[WNDLFT], m[WNDWDTH], m[WNDTOP], m[WNDBTM] = 0, 40, 0, 24
        m[CH], m[CV], m[INVFLG] = 0, 0, 0xFF
        m[CSW], m[CSW + 1] = COUT1 & 0xFF, COUT1 >> 8

    def text(self) -> str:
        """The screen as text, one line per row, inverse characters in
        lower case."""
        rows = []
        for row in self.screen:
            out = []
            for c in row:
                ch = c & 0x3F
                ch = ch + 0x40 if ch < 0x20 else ch
                s = chr(ch)
                out.append(s if c >= 0x80 else s.lower())
            rows.append("".join(out).rstrip())
        return "\n".join(rows)

    def _m(self, addr: int) -> int:
        return self.mpu.memory[addr]

    def _log(self, name: str) -> None:
        # A is recorded only for the routines that read it.
        a = self.mpu.a if name in ("COUT", "COUT1") else None
        self.calls.append((
            name, a, self._m(CH), self._m(CV), self._m(INVFLG),
        ))

    # ---- The Monitor ---------------------------------------------------

    def _put(self, c: int) -> None:
        """Puts screen code `c` at the cursor and advances it."""
        m = self.mpu.memory
        col = m[WNDLFT] + m[CH]
        if 0 <= m[CV] < 24 and col < 40:
            self.screen[m[CV]][col] = c
        m[CH] += 1
        if m[CH] >= m[WNDWDTH]:
            self._cr()

    def _cr(self) -> None:
        m = self.mpu.memory
        m[CH] = 0
        m[CV] += 1
        if m[CV] >= m[WNDBTM]:
            m[CV] = m[WNDBTM] - 1
            left, width = m[WNDLFT], m[WNDWDTH]
            for row in range(m[WNDTOP], m[WNDBTM] - 1):
                self.screen[row][left:left + width] = (
                    self.screen[row + 1][left:left + width]
                )
            self.screen[m[WNDBTM] - 1][left:left + width] = (
                bytes([SPACE] * width)
            )

    def _screen_out(self, a: int) -> None:
        m = self.mpu.memory
        if a >= 0xA0:
            self._put(a & m[INVFLG])
        elif a == 0x8D:
            self._cr()
        elif a == 0x88:
            if m[CH] > 0:
                m[CH] -= 1

    def _cout1(self) -> None:
        self._log("COUT1")
        self._screen_out(self.mpu.a)

    def _cout(self) -> None:
        self._log("COUT")
        csw = self._m(CSW) | self._m(CSW + 1) << 8
        if csw == PRINTER:
            self.printer.append(self.mpu.a)
        else:
            self._screen_out(self.mpu.a)

    def _home(self) -> None:
        self._log("HOME")
        m = self.mpu.memory
        left, width = m[WNDLFT], m[WNDWDTH]
        for row in range(m[WNDTOP], m[WNDBTM]):
            self.screen[row][left:left + width] = bytes([SPACE] * width)
        m[CH], m[CV] = 0, m[WNDTOP]

    def _clreol(self) -> None:
        self._log("CLREOL")
        m = self.mpu.memory
        row = self.screen[m[CV]]
        for col in range(m[WNDLFT] + m[CH], m[WNDLFT] + m[WNDWDTH]):
            if col < 40:
                row[col] = SPACE & m[INVFLG]

    def _vtab(self) -> None:
        self._log("VTAB")

    def _step_random(self) -> None:
        # KEYIN counts RNDL/RNDH up once per pass of its loop while it
        # waits for a key, so the count depends on how long the key
        # took. Here each wait is a number of passes from a linear
        # congruential generator, so that runs repeat. A program that
        # calls its random routine many times between keys, as Zork
        # does when the player dies, can loop forever on an unlucky
        # seed, since Zork's routine only shifts the high byte between
        # keys.
        self._lcg = (self._lcg * 1103515245 + 12345) & 0x7FFFFFFF
        m = self.mpu.memory
        rnd = (m[RNDL] | m[RNDL + 1] << 8) + 1 + (self._lcg >> 16)
        m[RNDL], m[RNDL + 1] = rnd & 0xFF, (rnd >> 8) & 0xFF

    def _rdkey(self) -> None:
        self._log("RDKEY")
        self._step_random()
        key = self.keys.pop(0) if self.keys else 0x0D
        self.mpu.a = key | 0x80

    def _getln1(self) -> None:
        self._log("GETLN1")
        if not self.lines:
            raise Stop("input used up")
        line = self.lines.pop(0).upper()
        m = self.mpu.memory
        for x, ch in enumerate(line):
            self._step_random()
            code = ord(ch) | 0x80
            m[0x0200 + x] = code
            self._screen_out(code)
        m[0x0200 + len(line)] = 0x8D
        self._clreol()
        self._screen_out(0x8D)
        self.mpu.x = len(line)
        self.mpu.a = 0x8D

    # ---- DOS -----------------------------------------------------------

    def _rwts(self) -> None:
        mpu = self.mpu
        m = mpu.memory
        iob = (mpu.a << 8) | mpu.y
        slot, drive = m[iob + 1], m[iob + 2]
        track, sector = m[iob + 4], m[iob + 5]
        buf = m[iob + 8] | m[iob + 9] << 8
        command = m[iob + 12]
        self.calls.append(("RWTS", slot, drive, track, sector, command))
        disk = self.disks.get((slot, drive))
        if disk is None or track >= 35 or sector >= 16:
            mpu.p |= 0x01                   # carry set: failed
            return
        at = dsk_sector(disk, track, sector)
        if command == 1:
            m[buf:buf + 256] = disk[at:at + 256]
        elif command == 2:
            disk[at:at + 256] = bytes(m[buf:buf + 256])
            self.written.append((slot, drive, track, sector))
        mpu.p &= ~0x01

    # ---- Running -------------------------------------------------------

    def run(self, max_cycles: int = 2_000_000_000,
            other_hooks: dict | None = None) -> str:
        """Steps the MPU until the program ends, and returns why: "brk",
        "halt" (a jump to itself), "input used up", or raises if it runs
        past `max_cycles`. With `C6502_SIM_LOCKSTEP=1`, the C++ CPU runs
        the same instructions (`sim.lockstep.MachineRecorder`)."""
        from sim import lockstep
        mpu = self.mpu
        rec = lockstep.MachineRecorder(mpu, "apple2") if lockstep.enabled() else None
        try:
            return self._run(max_cycles, other_hooks, rec)
        finally:
            if rec is not None:
                rec.finish()

    def _run(self, max_cycles: int, other_hooks: dict | None, rec) -> str:
        mpu = self.mpu
        mem = mpu.memory
        hooks = dict(other_hooks or {})
        hooks.update(self.hooks)
        try:
            while mpu.processorCycles < max_cycles:
                pc = mpu.pc
                hook = hooks.get(pc)
                if hook is not None:
                    hook() if pc in self.hooks else hook(mem)
                    # Return as RTS would.
                    sp = mpu.sp
                    lo = mem[0x0100 + ((sp + 1) & 0xFF)]
                    hi = mem[0x0100 + ((sp + 2) & 0xFF)]
                    mpu.sp = (sp + 2) & 0xFF
                    mpu.pc = (((hi << 8) | lo) + 1) & 0xFFFF
                    continue
                op = mem[pc]
                if op == 0x00:
                    raise Stop("brk")
                if op == 0x4C and mem[pc + 1] | mem[pc + 2] << 8 == pc:
                    raise Stop("halt")
                if rec is not None:
                    rec.before()
                mpu.step()
                if rec is not None:
                    rec.after()
        except Stop as stop:
            self.stop_reason = stop.reason
            return stop.reason
        raise AssertionError(f"ran past {max_cycles} cycles")
