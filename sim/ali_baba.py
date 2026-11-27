"""A stand-in Apple II for Electronic Arts' Ali Baba and the Forty
Thieves, for running the original game and examples/ali_baba on the
same machine and comparing what each does.

The machine has 48K of RAM, a keyboard, a speaker, the display
switches, the paddle buttons, a Disk II controller in slot 6
(`sim/disk2.py`), and Python stand-ins for the Monitor ROM routines the
game calls. The disk is emulated at the level of its nibbles, so that
the game's own disk code runs unchanged: DOS 3.3's RWTS at $B600, the
VM-driven reader at $B300, and the resident routines at $0500.

Every access to $C000-$C0FF is an event, and so is every call of a
Monitor routine, except those the kept disk code makes. That code, at
$0500-$07FF and $B300-$BFFF, is the original's in both programs, so the
accesses it makes to the disk switches, the keyboard (VERIFY_DATA_FIELD
polls KBD once per nibble), and the Monitor (the VMs call WAIT) aren't
events, and its reads of KBD don't count toward the keyboard script.
In their place, each call of DISK_READ_FIELD ($B300) or RWTS_ENTRY
($B7B5) from outside that code is one event, recorded when it returns,
with its result: the carry, and for RWTS the IOB and the 256 bytes of
its buffer. Each disk read thus stays at its place in the stream, and
the thousands of nibble reads inside it aren't compared. The motor
switch that LOAD_SCENE_DATA touches itself is an event like any
other.

Each event records the address (or the Monitor routine's, or the disk
entry's), whether it was a read or a write, a CRC of hi-res page 1
($2000-$3FFF) as it was when the access happened, and an argument:
the register a Monitor routine reads (A for WAIT, COUT, and VTABZ, X
for PREAD), and 0 otherwise, since the A a caller happens to leave for
a routine that ignores it, such as SETPWRC, differs between the two
programs. Page 2 is the game's data, so it isn't covered. Until the
program first touches a display switch, page 1 holds the code
GAME_INIT relocates, so the events before then (one, SETPWRC's)
record 0 for its CRC. GAME_INIT reads TXTCLR ($C050) just after the
relocation has cleared page 1, and the C program's game_init clears
it too. It also records the program counter of the instruction and the
cycle count, which differ between the two programs and only help find
where a difference comes from.

The Monitor routines are hooks at their ROM addresses. COUT ($FDED)
jumps through CSW as the ROM does, which after INIT_HRCG is the HRCG's
HRCG_COUT, so the character output is the game's own code. COUT isn't
an event, since the C program calls its HRCG directly, and what the
HRCG draws and the Monitor routines it calls are. WAIT
($FCA8) takes the ROM's time, 7 + 11A + 5A(A+1)/2 cycles after the
JSR for A from 1 to 255, and for A = 0 what the ROM's loop takes
(`_WAIT_CYCLES`). PREAD ($FB1E) finds no paddle and returns
Y = $FF after the ROM's 2833 cycles. SETPWRC ($FB6F) sets PWREDUP from
the reset vector. The text routines the HRCG's control characters end
with (VTAB, VTABZ, CLREOP, HOME, SCROLL, and CLREOL) write text page 1
at $0400-$07FF, as the ROM does, which is where the resident routines
are. Each follows the ROM's instructions, and their memory, registers,
and flags matched the Apple II Plus ROM's on 3,000 random windows,
cursors, and registers. COUT1 ($FDF0) has no stand-in, since the game
calls COUT only after INIT_HRCG has pointed CSW at HRCG_COUT, so a run
that reaches it ends with a message.

The keyboard follows a script of (n, key) pairs. The key goes into
KBD's latch, with bit 7 set, just before the program's KBD read number
n (counting from 0), so that both programs see a key at the same point
in their polling. The disks follow a script of (n, name) pairs, which
put the disk `name` in drive 1 just before KBD read number n. Bit 7 stays set until the program reads or writes
KBDSTRB, as on the Apple II. No paddle button is ever down, and no
joystick is attached. GAME_INIT's SELECT_INPUT_MODE waits for Space
(keyboard) or a change of button 0 (joystick), so every script starts
with a Space.

The machine starts as EALDR leaves it when it jumps to $0800, which a
boot of the disk on Apple II Plus and Disk II ROMs through
`sim/disk2.py` showed (`ENTRY_ZERO_PAGE` and `entry_image`). The
memory is main.bin with GAME_INIT's relocation undone, since GAME_INIT
copies $2000-$32FF to $9600-$A8FF and $3300-$3FFF to $B300-$BFFF and
then clears $2000-$3FFF. The registers are A=$08, X=$08, Y=$0C,
SP=$FF, and P=$30, the display is full-screen hi-res page 1, and the
drive's head is over track 5 with phase 2 on and the motor stopped.
EALDR keeps no record of the head's track where DOS's RWTS keeps it
(the text page's screen hole at $047E for slot 6), so the game's RWTS
starts out taking the head to be over track 0.

The disk is the game's .dsk image. Its track 5 is the copy protection
track: every sector's data field is 342 nibbles of $AF, which is what
VERIFY_DATA_FIELD checks for. The check (DISK_READ_FIELD, called by
LOAD_SCENE_DATA) reads at track 5, half track 5.5, and track 6, so the
original disk's protection track was evidently a wide one, readable
from half track 10 to 12. The image has only whole tracks, and its
track 6 is empty, so on a drive that reads half track 11 as track 5
and half track 12 as track 6, each of the check's passes would find 4
bad sectors, the check would fail, and LOAD_SCENE_DATA would write a
random byte into the game's messages at $9AA6-$9BA5. `WIDE_TRACK` (the
default) makes half tracks 11 and 12 read track 5, and nothing on the
image is on track 6, so the check passes. The check runs after RWTS has sought track 5 with command 0,
which doesn't read an address field. When it's the game's first RWTS
call, RWTS steps ten half tracks from track 0, where it takes the head
to be, the head ends over half tracks 18 to 20 (tracks 9 and 10), and
that check fails all the same. Once a read has found the track the
head is over, the check passes: after the game script
(cpp/src/sim/games.h), three calls of LOAD_SCENE_DATA each got the
carry clear from DISK_READ_FIELD, and without `WIDE_TRACK` each got it
set.

`Machine.run` steps the MPU until a cycle limit or until its `stop`
predicate returns true for an event. The soft switches are found by
decoding each instruction before it runs. Only the absolute and
indirect addressing modes can reach $C0xx, so the check is a table
lookup for most instructions. `Machine.seen` marks every address an
instruction was fetched from, for `coverage`.

Most of the game's time goes to the delay loops of its sound routines,
so `run` takes three kinds of loop in one step: DEX; BNE back to the
DEX, the same with DEY, and ROL zp; DEX; BNE back to the ROL (in
PLAY_LOW_TONE). Each leaves the registers, flags, memory, and cycle
count as the loop would, so a run's events, memory, and registers are
the same with or without them, and the title tune runs nine times
faster.
"""
from __future__ import annotations

import re
import zlib
from array import array
from dataclasses import dataclass, field
from typing import Callable

from py65.devices.mpu6502 import MPU

from binutils.opcodes import DECODE
from sim.disk2 import DiskII, Drive

# ---- The machine's addresses ------------------------------------------------

KBD = 0xC000
KBDSTRB = 0xC010
SPKR = 0xC030
BUTN0, BUTN1, BUTN2 = 0xC061, 0xC062, 0xC063
PADDL0 = 0xC064
PTRIG = 0xC070
DISK_SLOT = 6
DISK_SWITCHES = 0xC080 + 16 * DISK_SLOT

# The Monitor routines the game calls, and those the HRCG's control
# characters jump to.
PREAD = 0xFB1E
SETPWRC = 0xFB6F
VTAB = 0xFC22
VTABZ = 0xFC24
CLREOP = 0xFC42
HOME = 0xFC58
SCROLL = 0xFC70
CLREOL = 0xFC9C
WAIT = 0xFCA8
COUT = 0xFDED
COUT1 = 0xFDF0

# The Monitor routines whose result depends on A, the one argument an
# event of theirs records.
_READS_A = frozenset({WAIT, COUT, COUT1, VTABZ})

# The Monitor's zero page.
WNDLFT, WNDWDTH, WNDTOP, WNDBTM = 0x20, 0x21, 0x22, 0x23
CH, CV, BASL, BASH, BAS2L, BAS2H = 0x24, 0x25, 0x28, 0x29, 0x2A, 0x2B
INVFLG, YSAV1, CSW = 0x32, 0x35, 0x36
SOFTEV, PWREDUP = 0x03F2, 0x03F4

# The disk code that both programs keep, and its two entries.
KEPT_DISK_CODE = ((0x0500, 0x0800), (0xB300, 0xC000))
DISK_READ_FIELD = 0xB300
RWTS_ENTRY = 0xB7B5

# The bytes of the hi-res page an event's CRC covers.
PAGE1 = (0x2000, 0x4000)

# Event codes, in place of a soft switch's address: a Monitor routine
# is ROM_EVENT + its address, and a call into the disk code is
# DISK_EVENT + its entry.
ROM_EVENT = 0x10000
DISK_EVENT = 0x20000

# Where `Machine.call` makes a subroutine return to, an address in the
# ROM that the game never calls.
_RETURN_TRAP = 0xFFF0

# The addressing modes that can reach a soft switch, numbered for the
# run loop. 0 is every other mode.
_ABS, _ABSX, _ABSY, _INDX, _INDY = 1, 2, 3, 4, 5
_MODES = {"abs": _ABS, "absx": _ABSX, "absy": _ABSY,
          "indx": _INDX, "indy": _INDY}
_WRITES = {"STA": "a", "STX": "x", "STY": "y"}
_READ_WRITES = {"INC", "DEC", "ASL", "LSR", "ROL", "ROR"}
_NO_ACCESS = {"JMP", "JSR"}


def _mode_table() -> tuple[bytes, bytes, bytes]:
    """For each opcode, its addressing mode (one of the numbers above,
    or 0), its access (1 read, 2 write, 3 both), and for a store, the
    register it stores (1 A, 2 X, 3 Y)."""
    modes = bytearray(256)
    access = bytearray(256)
    reg = bytearray(256)
    for op, (mnemonic, mode) in DECODE.items():
        if mode not in _MODES or mnemonic in _NO_ACCESS:
            continue
        modes[op] = _MODES[mode]
        access[op] = (2 if mnemonic in _WRITES
                      else 3 if mnemonic in _READ_WRITES else 1)
        reg[op] = " axy".index(_WRITES.get(mnemonic, "a"))
    return bytes(modes), bytes(access), bytes(reg)


_OP_MODE, _OP_ACCESS, _OP_REG = _mode_table()

# The first opcodes of the delay loops `Machine.run` takes in one step:
# DEX, DEY, and ROL zp.
_LOOP_HEADS = bytes(1 if op in (0xCA, 0x88, 0x26) else 0
                    for op in range(256))


def _kept(pc: int) -> bool:
    return 0x0500 <= pc < 0x0800 or 0xB300 <= pc < 0xC000


# ---- The machine as EALDR leaves it ----------------------------------------

# The zero page when EALDR jumps to $0800, from a boot of the disk on
# the Apple II Plus ROM: the Monitor's window, INVFLG, CSW and KSW
# after RESET, and what the boot ROM, BOOT1, and EALDR left. Every
# other byte is 0.
ENTRY_ZERO_PAGE = {
    0x01: 0xC6, 0x21: 0x28, 0x23: 0x18, 0x29: 0x04, 0x2B: 0x60,
    0x2C: 0xF5, 0x2D: 0x0A, 0x2E: 0x01, 0x2F: 0xFE, 0x32: 0xFF,
    0x35: 0x1B, 0x36: 0xF0, 0x37: 0xFD, 0x38: 0xE8, 0x39: 0x06,
    0x3A: 0x10, 0x3B: 0xFE, 0x3C: 0xFF, 0x3D: 0x05, 0x3E: 0x30,
    0x40: 0x50, 0x43: 0x08, 0x45: 0x33, 0x46: 0x3C, 0x47: 0xA9,
    0x4A: 0x0A, 0x4B: 0x0A, 0x4D: 0xA6, 0x4F: 0x08,
}
# $03F0-$03F4 then: the BRK vector, the reset vector EALDR set, and
# PWREDUP.
ENTRY_VECTORS = bytes([0x59, 0xFA, 0x48, 0xA9, 0x0C])
ENTRY_REGISTERS = {"a": 0x08, "x": 0x08, "y": 0x0C, "sp": 0xFF,
                   "p": 0x30}
ENTRY = 0x0800

# The relocation GAME_INIT does (RELOCATE_HIRES_STAGING): each span of
# the staging area in hi-res page 1, and where it goes.
RELOCATIONS = ((0x2000, 0x9600, 0x1300), (0x3300, 0xB300, 0x0D00))

# The half tracks that read track 5, the protection track, besides
# its own (10). See the module docstring.
WIDE_TRACK = {11: 5, 12: 5}


def entry_image(main_bin: bytes) -> bytearray:
    """The 64K of memory as EALDR leaves it, from main.bin (the image
    of $0500-$BFFF after GAME_INIT's relocation): the zero page and
    vectors above, and the relocated code put back in the staging area
    of hi-res page 1. The relocated addresses keep main.bin's bytes,
    which GAME_INIT's copy overwrites, and $A900-$B2FF holds main.bin's
    zeros where EALDR's code was."""
    mem = bytearray(0x10000)
    mem[0x0500:0x0500 + len(main_bin)] = main_bin
    for staged, home, size in RELOCATIONS:
        mem[staged:staged + size] = mem[home:home + size]
    for addr, value in ENTRY_ZERO_PAGE.items():
        mem[addr] = value
    mem[0x03F0:0x03F5] = ENTRY_VECTORS
    return mem


def entry_drive(disk: bytes, wide_track: bool = True) -> Drive:
    """The game's disk in drive 1, with the head where EALDR leaves
    it."""
    drive = Drive.from_dsk(disk)
    if wide_track:
        for ht, t in WIDE_TRACK.items():
            drive.track_at[ht] = t
    drive.half_track = 10
    drive.phases = 0b0100
    return drive


# ---- Events -----------------------------------------------------------------

@dataclass
class Events:
    """The event stream of one run, one entry per event in each array.
    `code` is the soft switch's address times 2, plus 1 for a write (a
    Monitor routine is `ROM_EVENT` plus its address, times 2, and a
    disk call `DISK_EVENT` plus its entry, times 2), `page1` is the
    CRC of hi-res page 1, and `arg` is the register a Monitor routine
    reads, or 0. `disk`
    maps the index of each disk event to its entry, the carry it
    returned, the half track the head ended over, and for RWTS the 17
    bytes of the IOB and the 256 of the buffer after the call."""

    code: array = field(default_factory=lambda: array("L"))
    page1: array = field(default_factory=lambda: array("L"))
    arg: array = field(default_factory=lambda: array("H"))
    pc: array = field(default_factory=lambda: array("H"))
    cycles: array = field(default_factory=lambda: array("Q"))
    disk: dict[int, tuple[int, int, int, bytes, bytes]] = field(
        default_factory=dict)

    def __len__(self) -> int:
        return len(self.code)

    def describe(self, i: int) -> str:
        """Event `i` as text."""
        code = self.code[i] >> 1
        if code >= DISK_EVENT:
            entry, carry, ht, iob, _ = self.disk[i]
            what = f"disk call ${entry:04X} returning C={carry}"
            if iob:
                what += (f" (RWTS command {iob[12]} T${iob[4]:02X} "
                         f"S${iob[5]:X} buffer ${iob[9]:02X}{iob[8]:02X})")
        elif code >= ROM_EVENT:
            name = ROM_NAMES.get(code - ROM_EVENT,
                                 f"${code - ROM_EVENT:04X}")
            what = f"{name} with A=${self.arg[i]:02X}"
        else:
            rw = "write" if self.code[i] & 1 else "read"
            what = f"{rw} ${code:04X}"
        return (f"#{i} {what} at PC ${self.pc[i]:04X}, cycle "
                f"{self.cycles[i]}, page 1 {self.page1[i]:08X}")

    def counts(self) -> dict[str, int]:
        """The number of events of each kind, by soft switch, Monitor
        routine, or disk entry."""
        out: dict[str, int] = {}
        for c in self.code:
            code = c >> 1
            if code >= DISK_EVENT:
                key = f"disk ${code - DISK_EVENT:04X}"
            elif code >= ROM_EVENT:
                key = ROM_NAMES.get(code - ROM_EVENT, hex(code))
            else:
                key = f"${code:04X} {'write' if c & 1 else 'read'}"
            out[key] = out.get(key, 0) + 1
        return out


def first_difference(a: Events, b: Events) -> int | None:
    """The index of the first event at which the two streams differ,
    in the access, the page, the argument, or a disk call's result, or
    None when one is a prefix of the other."""
    n = min(len(a), len(b))
    for name in ("code", "page1", "arg"):
        x, y = getattr(a, name), getattr(b, name)
        if x[:n] != y[:n]:
            n = next(i for i in range(n) if x[i] != y[i])
    for i in sorted(set(a.disk) | set(b.disk)):
        if i < n and a.disk.get(i) != b.disk.get(i):
            n = i
            break
    return n if n < min(len(a), len(b)) else None


# ---- The machine ------------------------------------------------------------

@dataclass
class Machine:
    """The MPU and its devices. `keys` is the keyboard script, (n, key)
    pairs with the key in ASCII. `disk` is the controller in slot 6.
    `media` holds the disks by name, "game" being the one in drive 1 at
    the start, and `swaps` is the disk script, (n, name) pairs that put
    disk `name` in drive 1 just before the program's KBD read number n,
    as the keys go in."""

    mpu: MPU
    disk: DiskII
    keys: list[tuple[int, int]] = field(default_factory=list)
    media: dict[str, Drive] = field(default_factory=dict)
    swaps: list[tuple[int, str]] = field(default_factory=list)
    events: Events = field(default_factory=Events)
    kbd_reads: int = 0
    kbd_latch: int = 0
    # The display switches as they were last set.
    text: bool = False
    mixed: bool = False
    page2: bool = False
    hires: bool = True
    # Python routines run in place of the code at their addresses. A
    # hook that leaves the program counter where it was lets the
    # instruction there run after it, and one that returns a string
    # ends `run`.
    hooks: dict[int, Callable[[], str | None]] = field(
        default_factory=dict)
    # Whether `run` takes the delay loops in one step.
    fast_loops: bool = True
    # Whether the program has touched a display switch ($C050-$C05F)
    # yet. Until then, events record 0 for page 1's CRC.
    display_set: bool = False

    def __post_init__(self) -> None:
        self._keys = dict(self.keys)
        self._swaps = dict(self.swaps)
        self.seen = bytearray(0x10000)
        self._copy: bytes | None = None
        self._crc = 0
        rom = {
            WAIT: self._wait, COUT: self._cout, PREAD: self._pread,
            SETPWRC: self._setpwrc, COUT1: self._cout1, VTAB: self._vtab,
            VTABZ: self._vtabz, CLREOP: self._clreop, HOME: self._home,
            SCROLL: self._scroll, CLREOL: self._clreol,
        }
        for addr, fn in rom.items():
            self.hooks.setdefault(addr, self._rom_hook(addr, fn))
        for entry in (DISK_READ_FIELD, RWTS_ENTRY):
            self.hooks.setdefault(entry, self._disk_entry(entry))

    # ---- Events --------------------------------------------------------

    def _page_crc(self) -> int:
        """The CRC of hi-res page 1. It's computed again only when the
        page's bytes differ from the copy kept with the last one, since
        comparing takes about a tenth of the time."""
        mem = self.mpu.memory
        start, end = PAGE1
        copy = self._copy
        if copy is None or not mem.startswith(copy, start, end):
            copy = bytes(mem[start:end])
            self._copy = copy
            self._crc = zlib.crc32(copy)
        return self._crc

    def _record(self, code: int, pc: int, arg: int = 0) -> None:
        ev = self.events
        ev.code.append(code)
        ev.page1.append(self._page_crc() if self.display_set else 0)
        ev.arg.append(arg)
        ev.pc.append(pc)
        ev.cycles.append(self.mpu.processorCycles)

    # ---- Soft switches -------------------------------------------------

    def _access(self, addr: int, access: int, value: int, pc: int) -> None:
        """Records an access to soft switch `addr` by the instruction at
        `pc`, which writes `value` if it writes, and puts the value a
        read returns at `addr`."""
        mem = self.mpu.memory
        kept = _kept(pc)
        if addr & 0xF0 == 0x50:
            self.display_set = True
        if not kept:
            if access & 1:
                self._record(addr << 1, pc)
            if access & 2:
                self._record(addr << 1 | 1, pc)
        page = addr & 0xF0
        if page == 0x00:
            if access & 1 and not kept:
                key = self._keys.get(self.kbd_reads)
                if key is not None:
                    self.kbd_latch = key | 0x80
                name = self._swaps.get(self.kbd_reads)
                if name is not None:
                    self.insert(name)
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
        elif DISK_SWITCHES <= addr < DISK_SWITCHES + 16:
            mem[addr] = self.disk.access(
                addr, bool(access & 2), value, self.mpu.processorCycles)
        elif PADDL0 <= addr < PADDL0 + 4:
            mem[addr] = 0x80
        else:
            mem[addr] = 0

    def insert(self, name: str) -> None:
        """Puts disk `name` of `media` in drive 1, under the head where
        it is."""
        old = self.disk.drives[0]
        new = self.media[name]
        if old is not None:
            new.half_track, new.phases = old.half_track, old.phases
        self.disk.drives[0] = new

    # ---- Monitor routines ----------------------------------------------

    def _caller(self) -> int:
        """The address of the JSR that called the routine at the
        program counter, from the return address on the stack."""
        mem = self.mpu.memory
        sp = self.mpu.sp
        ret = mem[0x100 + ((sp + 1) & 0xFF)] | mem[0x100 + ((sp + 2) & 0xFF)] << 8
        return (ret - 2) & 0xFFFF

    def _rom_hook(self, addr: int,
                  fn: Callable[[], bool | str | None],
                  ) -> Callable[[], str | None]:
        """The hook for Monitor routine `addr`, which records the event
        unless the kept disk code called it, runs `fn`, and returns as
        RTS would unless `fn` returns True (a jump elsewhere)."""
        def hook() -> str | None:
            mpu = self.mpu
            caller = self._caller()
            # COUT only jumps through CSW to the HRCG, which the C
            # program calls directly, so it isn't an event. What the
            # HRCG draws, and the Monitor routines it calls, are.
            if not _kept(caller) and addr != COUT:
                # The argument is the register the routine reads: A for
                # WAIT, COUT, and VTABZ, X for PREAD, and nothing for the
                # rest, whose A is whatever the caller last loaded.
                arg = (mpu.a if addr in _READS_A
                       else mpu.x if addr == PREAD else 0)
                self._record((ROM_EVENT + addr) << 1, caller, arg)
            done = fn()
            if isinstance(done, str):
                return done
            if not done:
                self._rts()
            return None
        return hook

    def _rts(self) -> None:
        mpu = self.mpu
        mem = mpu.memory
        sp = mpu.sp
        lo = mem[0x0100 + ((sp + 1) & 0xFF)]
        hi = mem[0x0100 + ((sp + 2) & 0xFF)]
        mpu.sp = (sp + 2) & 0xFF
        mpu.pc = (((hi << 8) | lo) + 1) & 0xFFFF
        mpu.processorCycles += 6

    def _set_nz(self, value: int) -> None:
        mpu = self.mpu
        mpu.p = (mpu.p & ~0x82) | (value & 0x80) | (0 if value else 0x02)

    def _wait(self) -> None:
        """WAIT: A=0, with N and V clear and Z and C set, after the
        ROM's time."""
        mpu = self.mpu
        mpu.processorCycles += _WAIT_CYCLES[mpu.a]
        mpu.a = 0
        mpu.p = (mpu.p & ~0xC0) | 0x03

    def _cout(self) -> bool:
        """COUT: JMP (CSW)."""
        mem = self.mpu.memory
        self.mpu.pc = mem[CSW] | mem[CSW + 1] << 8
        self.mpu.processorCycles += 5
        return True

    def _pread(self) -> None:
        """PREAD: no paddle's timer runs out, so the count reaches 255.
        The ROM's last read of the paddle leaves $80 in A."""
        mpu = self.mpu
        mpu.processorCycles += 2827
        mpu.a = 0x80
        mpu.y = 0xFF
        mpu.p = (mpu.p & ~0x02) | 0x80

    def _setpwrc(self) -> None:
        """SETPWRC: PWREDUP = high byte of the reset vector XOR $A5."""
        mpu = self.mpu
        mem = mpu.memory
        mpu.a = mem[SOFTEV + 1] ^ 0xA5
        mem[PWREDUP] = mpu.a
        self._set_nz(mpu.a)
        mpu.processorCycles += 10

    # The Monitor's text routines, each step as the ROM's instructions
    # do it, so that the registers and flags come back as the ROM leaves
    # them. The pushes and return addresses the ROM leaves below the
    # stack pointer aren't written. Each takes about the ROM's time.

    def _nz(self, v: int) -> int:
        mpu = self.mpu
        mpu.p = (mpu.p & ~0x82) | (v & 0x80) | (0 if v else 0x02)
        return v

    def _adc(self, v: int) -> None:
        """ADC in binary mode."""
        mpu = self.mpu
        a = mpu.a
        r = a + v + (mpu.p & 1)
        p = mpu.p & ~0xC3
        if r > 0xFF:
            p |= 0x01
        if ~(a ^ v) & (a ^ r) & 0x80:
            p |= 0x40
        r &= 0xFF
        mpu.p = p | (r & 0x80) | (0 if r else 0x02)
        mpu.a = r

    def _cmp(self, reg: int, v: int) -> None:
        mpu = self.mpu
        r = (reg - v) & 0xFF
        mpu.p = ((mpu.p & ~0x83) | (0x01 if reg >= v else 0)
                 | (r & 0x80) | (0 if r else 0x02))

    def _asl(self) -> None:
        mpu = self.mpu
        mpu.p = (mpu.p & ~0x01) | (mpu.a >> 7)
        mpu.a = self._nz((mpu.a << 1) & 0xFF)

    def _bascalc(self) -> None:
        """BASCALC ($FBC1): BASH/BASL = the text line in A."""
        mpu = self.mpu
        mem = mpu.memory
        row = mpu.a
        mpu.p = (mpu.p & ~0x01) | (row & 1)
        mem[BASH] = self._nz(((row >> 1) & 3) | 4)
        mpu.a = self._nz(row & 0x18)
        if row & 1:
            self._adc(0x7F)
        mem[BASL] = mpu.a
        self._asl()
        self._asl()
        mpu.a = self._nz(mpu.a | mem[BASL])
        mem[BASL] = mpu.a
        mpu.processorCycles += 40

    def _vtabz_steps(self) -> None:
        """VTABZ: BASCALC, then ADC WNDLFT into BASL."""
        mem = self.mpu.memory
        self._bascalc()
        self._adc(mem[WNDLFT])
        mem[BASL] = self.mpu.a
        self.mpu.processorCycles += 15

    def _vtab_steps(self) -> None:
        self.mpu.a = self._nz(self.mpu.memory[CV])
        self.mpu.processorCycles += 3
        self._vtabz_steps()

    def _cleolz_steps(self) -> None:
        """CLEOLZ ($FC9E): spaces from column Y to the window's right
        edge, at least one."""
        mpu = self.mpu
        mem = mpu.memory
        mpu.a = self._nz(0xA0)
        base = mem[BASL] | mem[BASH] << 8
        width = mem[WNDWDTH]
        while True:
            mem[(base + mpu.y) & 0xFFFF] = 0xA0
            mpu.y = self._nz((mpu.y + 1) & 0xFF)
            self._cmp(mpu.y, width)
            mpu.processorCycles += 13
            if not mpu.p & 1:
                continue
            break
        mpu.processorCycles += 8

    def _cleop1_steps(self) -> None:
        """CLEOP1 ($FC46): clears from column Y of line A to the end of
        the window, then VTAB."""
        mpu = self.mpu
        mem = mpu.memory
        while True:
            row = mpu.a
            self._vtabz_steps()
            self._cleolz_steps()
            mpu.y = self._nz(0)
            mpu.a = self._nz(row)
            self._adc(0)
            self._cmp(mpu.a, mem[WNDBTM])
            mpu.processorCycles += 25
            if not mpu.p & 1:
                continue
            break
        self._vtab_steps()

    def _vtab(self) -> None:
        self._vtab_steps()

    def _vtabz(self) -> None:
        self._vtabz_steps()

    def _clreol(self) -> None:
        mpu = self.mpu
        mpu.y = self._nz(mpu.memory[CH])
        mpu.processorCycles += 3
        self._cleolz_steps()

    def _clreop(self) -> None:
        mpu = self.mpu
        mem = mpu.memory
        mpu.y = self._nz(mem[CH])
        mpu.a = self._nz(mem[CV])
        mpu.processorCycles += 6
        self._cleop1_steps()

    def _home(self) -> None:
        mpu = self.mpu
        mem = mpu.memory
        mpu.a = self._nz(mem[WNDTOP])
        mem[CV] = mpu.a
        mpu.y = self._nz(0)
        mem[CH] = 0
        mpu.processorCycles += 13
        self._cleop1_steps()

    def _scroll(self) -> None:
        """SCROLL ($FC70): moves the window's lines up one and clears
        its bottom line."""
        mpu = self.mpu
        mem = mpu.memory
        mpu.a = self._nz(mem[WNDTOP])
        row = mpu.a
        self._vtabz_steps()
        while True:
            mpu.a = self._nz(mem[BASL])
            mem[BAS2L] = mpu.a
            mpu.a = self._nz(mem[BASH])
            mem[BAS2H] = mpu.a
            mpu.y = self._nz((mem[WNDWDTH] - 1) & 0xFF)
            mpu.a = self._nz(row)
            self._adc(1)
            self._cmp(mpu.a, mem[WNDBTM])
            mpu.processorCycles += 30
            if mpu.p & 1:
                break
            row = mpu.a
            self._vtabz_steps()
            src = mem[BASL] | mem[BASH] << 8
            dst = mem[BAS2L] | mem[BAS2H] << 8
            while True:
                mpu.a = self._nz(mem[(src + mpu.y) & 0xFFFF])
                mem[(dst + mpu.y) & 0xFFFF] = mpu.a
                mpu.y = self._nz((mpu.y - 1) & 0xFF)
                mpu.processorCycles += 16
                if not mpu.y & 0x80:
                    continue
                break
        mpu.y = self._nz(0)
        self._cleolz_steps()
        self._vtab_steps()

    def _cout1(self) -> str:
        return "unimplemented Monitor routine COUT1"

    # ---- Disk calls ----------------------------------------------------

    def _disk_entry(self, entry: int) -> Callable[[], None]:
        """The hook at a disk entry. On a call from outside the kept
        code, it notes the stack pointer and sets a hook at the return
        address that records the event when the call returns there. The
        entry's own code then runs."""
        def hook() -> None:
            mpu = self.mpu
            caller = self._caller()
            if _kept(caller):
                return
            mem = mpu.memory
            sp = mpu.sp
            ret = (mem[0x100 + ((sp + 1) & 0xFF)]
                   | mem[0x100 + ((sp + 2) & 0xFF)] << 8) + 1
            iob = (mpu.a << 8) | mpu.y if entry == RWTS_ENTRY else None
            back = (sp + 2) & 0xFF
            previous = self.hooks.get(ret)

            def returned() -> str | None:
                if mpu.sp != back:
                    return previous() if previous else None
                if previous is None:
                    del self.hooks[ret]
                else:
                    self.hooks[ret] = previous
                self._disk_returned(entry, caller, iob)
                return previous() if previous else None

            self.hooks[ret] = returned
        return hook

    def _disk_returned(self, entry: int, caller: int,
                       iob: int | None) -> None:
        mpu = self.mpu
        mem = mpu.memory
        iob_bytes = buffer = b""
        if iob is not None:
            iob_bytes = bytes(mem[iob:iob + 17])
            buf = iob_bytes[8] | iob_bytes[9] << 8
            if iob_bytes[12] in (1, 2):
                buffer = bytes(mem[buf:buf + 256])
        drive = self.disk.drive()
        self._record((DISK_EVENT + entry) << 1, caller)
        self.events.disk[len(self.events) - 1] = (
            entry, mpu.p & 1, drive.half_track if drive else -1,
            iob_bytes, buffer,
        )

    # ---- Running -------------------------------------------------------

    def run(self, max_cycles: int,
            stop: Callable[["Machine"], bool] | None = None) -> str:
        """Steps the MPU until `max_cycles` have run ("cycles"), or until
        `stop`, called after each event, returns true ("stop"), or the
        program reaches a `BRK` ("brk", which is also what a call of a
        ROM routine without a hook runs into). A hook ends the run when
        it returns a string, which `run` returns. With
        `C6502_SIM_LOCKSTEP=1`, the C++ CPU runs the same instructions
        (`sim.lockstep.MachineRecorder`)."""
        from sim import lockstep
        rec = (lockstep.MachineRecorder(self.mpu, "ali_baba")
               if lockstep.enabled() else None)
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
        seen = self.seen
        instruct = mpu.instruct
        cycletime = mpu.cycletime
        extracycles = mpu.extracycles
        op_mode = _OP_MODE
        op_access = _OP_ACCESS
        op_reg = _OP_REG
        loop_heads = _LOOP_HEADS
        fast = self.fast_loops
        events = self.events
        n_events = len(events)
        while mpu.processorCycles < max_cycles:
            pc = mpu.pc
            hook = hooks.get(pc)
            if hook is not None:
                why = hook()
                if why is not None:
                    return why
                if mpu.pc != pc:
                    if stop is not None and len(events) != n_events:
                        n_events = len(events)
                        if stop(self):
                            return "stop"
                    continue
            seen[pc] = 1
            op = mem[pc]
            if op == 0x00:
                return "brk"
            if loop_heads[op] and fast and self._delay_loop(pc, op):
                continue
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
                    reg = op_reg[op]
                    value = (mpu.a if reg == 1 else mpu.x if reg == 2
                             else mpu.y)
                    self._access(ea, op_access[op], value, pc)
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

    def _delay_loop(self, pc: int, op: int) -> bool:
        """Runs the delay loop at `pc` to its end in one step, if the
        code there is one (see the module docstring) and no hook is
        inside it. Returns whether it did."""
        mpu = self.mpu
        mem = mpu.memory
        if op == 0x26:
            if mem[pc + 2:pc + 5] != b"\xCA\xD0\xFB":
                return False
            size, per_pass = 5, 7
        elif mem[pc + 1] == 0xD0 and mem[pc + 2] == 0xFD:
            size, per_pass = 3, 2
        else:
            return False
        hooks = self.hooks
        for a in range(pc + 1, pc + size):
            if a in hooks:
                return False
        end = pc + size
        taken = 4 if (end ^ pc) & 0xFF00 else 3
        if op == 0x88:
            passes = mpu.y or 256
            mpu.y = 0
        else:
            passes = mpu.x or 256
            mpu.x = 0
        if op == 0x26:
            zp = mem[pc + 1]
            nine = (mpu.p & 1) << 8 | mem[zp]
            k = passes % 9
            nine = ((nine << k) | (nine >> (9 - k))) & 0x1FF
            mem[zp] = nine & 0xFF
            mpu.p = (mpu.p & ~0x01) | (nine >> 8)
        mpu.p = (mpu.p & ~0x80) | 0x02
        mpu.processorCycles += passes * (per_pass + taken) - taken + 2
        mpu.pc = end
        seen = self.seen
        seen[pc] = seen[end - 2] = 1
        if op == 0x26:
            seen[pc + 2] = 1
        return True

    def call(self, addr: int, max_cycles: int = 10_000_000) -> None:
        """Runs the subroutine at `addr` as a `JSR` would, until it
        returns."""
        mpu = self.mpu
        back = _RETURN_TRAP - 1
        mem = mpu.memory
        mem[0x0100 + mpu.sp] = back >> 8
        mem[0x0100 + ((mpu.sp - 1) & 0xFF)] = back & 0xFF
        mpu.sp = (mpu.sp - 2) & 0xFF
        pc = mpu.pc
        mpu.pc = addr
        self.hooks[_RETURN_TRAP] = lambda: "returned"
        try:
            why = self.run(mpu.processorCycles + max_cycles)
        finally:
            del self.hooks[_RETURN_TRAP]
        if why != "returned":
            raise RuntimeError(f"${addr:04X} didn't return ({why})")
        mpu.sp = (mpu.sp + 2) & 0xFF
        mpu.pc = pc

    # ---- Looking at it -------------------------------------------------

    def screen(self, width: int = 70) -> str:
        """Hi-res page 1 as text, a character for each block of pixels,
        `#` where at least half of them are on."""
        mem = self.mpu.memory
        bw = 280 // width
        bh = bw * 2
        rows = []
        for y0 in range(0, 192, bh):
            line = []
            for x0 in range(0, 280, bw):
                on = 0
                for y in range(y0, min(y0 + bh, 192)):
                    addr = (0x2000 + (y & 7) * 0x400
                            + ((y >> 3) & 7) * 0x80 + (y >> 6) * 0x28)
                    for x in range(x0, x0 + bw):
                        on += mem[addr + x // 7] >> (x % 7) & 1
                line.append("#" if on * 2 >= bw * bh else
                            "+" if on else " ")
            rows.append("".join(line).rstrip())
        return "\n".join(rows)


def _wait_cycles() -> list[int]:
    """The cycles WAIT takes after the JSR, for each A, by running the
    ROM's loop (SEC; PHA; SBC #1 until zero; PLA; SBC #1 and again
    until zero; RTS, less the RTS, which `_rts` counts)."""
    out = []
    for a0 in range(256):
        a, c, cycles = a0, 1, 2
        while True:
            cycles += 3
            outer = a
            while True:
                r = a - 1 - (1 - c)
                c = 1 if r >= 0 else 0
                a = r & 0xFF
                cycles += 2
                if a == 0:
                    cycles += 2
                    break
                cycles += 3
            a = outer
            cycles += 4
            r = a - 1 - (1 - c)
            c = 1 if r >= 0 else 0
            a = r & 0xFF
            cycles += 2
            if a == 0:
                cycles += 2
                break
            cycles += 3
        out.append(cycles)
    return out


_WAIT_CYCLES = _wait_cycles()

ROM_NAMES = {
    PREAD: "PREAD", SETPWRC: "SETPWRC", VTAB: "VTAB", VTABZ: "VTABZ",
    CLREOP: "CLREOP", HOME: "HOME", SCROLL: "SCROLL", CLREOL: "CLREOL",
    WAIT: "WAIT", COUT: "COUT", COUT1: "COUT1",
}


# ---- Setting up the two programs ------------------------------------------

def _machine(mem: bytearray, disk: bytes, keys: list[tuple[int, int]],
             swaps: list[tuple[int, str]],
             disks: dict[str, bytes | None] | None,
             wide_track: bool) -> Machine:
    mpu = MPU()
    mpu.memory = mem
    for reg, value in ENTRY_REGISTERS.items():
        setattr(mpu, reg, value)
    mpu.pc = ENTRY
    media = {"game": entry_drive(disk, wide_track)}
    for name, image in (disks or {}).items():
        media[name] = Drive.blank() if image is None else Drive.from_dsk(image)
    controller = DiskII([media["game"], None], slot=DISK_SLOT)
    return Machine(mpu, controller, list(keys), media, list(swaps))


def original_machine(main_bin: bytes, disk: bytes,
                     keys: list[tuple[int, int]],
                     swaps: list[tuple[int, str]] = (),
                     disks: dict[str, bytes | None] | None = None,
                     wide_track: bool = True) -> Machine:
    """The original, `main_bin` being main.bin of the
    reverse-engineering project, ready to run from $0800 as EALDR
    leaves the machine. `disk` is the game's .dsk image, and `disks`
    the other disks the disk script `swaps` puts in, each a .dsk image
    or None for an unformatted disk."""
    return _machine(entry_image(main_bin), disk, keys, swaps, disks,
                    wide_track)


# The spans of the original's memory, at their addresses after
# GAME_INIT's relocation, whose code and tables the C program
# translates (from the plan for examples/ali_baba), for reporting the
# coverage of the code the comparison checks.
CODE_SPANS = (
    (0x0800, 0x1F19), (0x5AA9, 0x776E), (0x7780, 0x7D83),
    (0x92A5, 0x9AA5), (0xA409, 0xA5BC),
)


def _store(mem: bytearray, addr: int, data: bytes) -> None:
    """Stores `data` at `addr`."""
    mem[addr:addr + len(data)] = data


def _put(mem: bytearray, addr: int, data: bytes) -> None:
    """Stores `data` at `addr`, and the bytes of it that GAME_INIT's
    relocation copies from hi-res page 1 at their staging addresses
    too, so that they're in place both before and after the copy."""
    mem[addr:addr + len(data)] = data
    for staged, home, size in RELOCATIONS:
        lo, hi = max(addr, home), min(addr + len(data), home + size)
        if lo < hi:
            at = staged + lo - home
            mem[at:at + hi - lo] = data[lo - addr:hi - addr]


def link_regions(script: str) -> tuple[tuple[int, int], ...]:
    """The (start, end) of each region in the MEMORY block of a linker
    script, whose ORIGIN and LENGTH are numbers or sums and differences
    of them."""
    def value(expr: str) -> int:
        total, sign = 0, 1
        for token in re.findall(r"0[xX][0-9a-fA-F]+|\d+|[-+]", expr):
            if token in "+-":
                sign = 1 if token == "+" else -1
            else:
                total += sign * int(token, 0)
        return total
    block = re.search(r"MEMORY\s*\{(.*?)\}", script, re.S)
    out = []
    for m in re.finditer(r"ORIGIN\s*=\s*([^,]+),\s*LENGTH\s*=\s*([^\n]+)",
                         block.group(1) if block else ""):
        start = value(m.group(1))
        out.append((start, start + value(m.group(2))))
    return tuple(out)


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


def program_machine(main_bin: bytes, exe, disk: bytes,
                    keys: list[tuple[int, int]],
                    swaps: list[tuple[int, str]] = (),
                    disks: dict[str, bytes | None] | None = None,
                    entry: str | int = "main",
                    clear: tuple[tuple[int, int], ...] = (),
                    ssp: int | None = None,
                    wide_track: bool = True,
                    staged: bool = False) -> Machine:
    """The linked C program `exe` (an `ElfFile`) loaded over the
    original's image as EALDR leaves it, ready to run from `entry` (a
    symbol or an address). The spans in `clear`, which should be the
    linker script's regions (`link_regions`), are zeroed first, so that
    a jump into the original's code that the program replaces runs into
    a BRK. Each
    segment's bytes past its file size are zeroed, as a loader would.
    The program's game_init doesn't relocate, so hi-res page 1 keeps
    the original's staged bytes, which the original shows on the
    screen until it draws over them, and the program's segments are
    put only at their own addresses. With `staged`, a segment at an
    address GAME_INIT relocates to is also put at its staging address
    (`_put`), for a program that relocates as the original does.

    When the program calls the simulator's runtime helpers (it was
    linked with `sim.linked.defsym_args()`), they're installed as
    `sim/linked.py` installs them, at $E000 in the ROM's space, which
    the game never uses. A program linked without them holds the
    helpers it calls from the runtime library, in its own segments.
    `ssp`, if given, is stored in SSP, for a program whose functions
    use the soft stack.

    `__zp_init`, if the program has one, has already run. The
    registers are then put back as EALDR leaves them."""
    mem = entry_image(main_bin)
    put = _put if staged else _store
    for start, end in clear:
        put(mem, start, bytes(end - start))
    for seg in exe.segments:
        put(mem, seg.paddr, seg.data + bytes(seg.memsz - len(seg.data)))
    machine = _machine(mem, disk, keys, swaps, disks, wide_track)
    symbols = program_symbols(exe)
    _install_helpers(machine, symbols, exe)
    if ssp is not None:
        from passes.runtime_layout import CLASSIC
        at = symbols.get("SSP", CLASSIC.ssp)
        mem[at], mem[at + 1] = ssp & 0xFF, ssp >> 8
    if "__zp_init" in symbols:
        machine.call(symbols["__zp_init"])
        for reg, value in ENTRY_REGISTERS.items():
            setattr(machine.mpu, reg, value)
    machine.mpu.pc = entry if isinstance(entry, int) else symbols[entry]
    return machine


def _install_helpers(machine: Machine, symbols: dict[str, int],
                     exe) -> None:
    """Installs the simulator's runtime helpers that the program `exe`
    refers to and doesn't hold itself, checking that it was linked with
    their addresses."""
    import sim.runtime as rt_mod
    from binutils import elf as E
    from passes.runtime_layout import CLASSIC, RuntimeLayout
    own = {
        s.name for s in exe.symbols
        if s.defined and s.type != E.STT_FILE and s.section != E.SHN_ABS
    }
    layout = RuntimeLayout(
        ssp=symbols.get("SSP", CLASSIC.ssp),
        fp=symbols.get("FP", CLASSIC.fp),
        hargs=symbols.get("HARGS", CLASSIC.hargs),
        dptr=symbols.get("DPTR", CLASSIC.dptr),
    )
    runtime = rt_mod.build_runtime(layout)
    used = [n for n in runtime.symbols if n in symbols and n not in own]
    if not used:
        return
    for name in used:
        if symbols[name] != runtime.symbols[name]:
            raise RuntimeError(
                f"the executable has `{name}` at ${symbols[name]:04X}, "
                f"and the runtime has it at ${runtime.symbols[name]:04X}")
    mem = machine.mpu.memory
    if runtime.helper_image:
        at = runtime.helper_origin
        mem[at:at + len(runtime.helper_image)] = runtime.helper_image
    for addr, fn in runtime.hooks.items():
        def hook(fn=fn) -> None:
            fn(machine.mpu.memory)
            machine._rts()
        machine.hooks[addr] = hook


# ---- The original's routines ------------------------------------------------

# The directives and macros of main.asm that lay down data: the
# assembler's own, the event-list macros (EV...), and the bytecode of
# the disk VMs (VM... and DVM...).
_DATA_OPS = ("DC", "HEX", "DS", "INCLUDE", "BYTE", "WORD", "APSTR")
_DATA_PREFIXES = ("EV", "VM", "DVM")


def original_labels(main_asm: str, main_sym: str) -> dict[int, tuple[str, bool]]:
    """Each global label of the original's main.asm (one without a dot,
    and not an EQU), by address from main.sym, with whether the first
    statement after it is an instruction (True) or data (False). When
    several labels share an address, the first in the source names
    it."""
    addrs: dict[str, int] = {}
    for line in main_sym.splitlines():
        parts = line.split()
        if len(parts) >= 2 and re.fullmatch(r"[0-9a-fA-F]{4}", parts[1]):
            addrs[parts[0]] = int(parts[1], 16)
    out: dict[int, tuple[str, bool]] = {}
    pending: list[str] = []
    for raw in main_asm.splitlines():
        line = raw.split(";")[0].rstrip()
        if not line.strip():
            continue
        if not line[0].isspace():
            head = line.split()[0].rstrip(":")
            rest = line[len(line.split()[0]):].split()
            if rest and (rest[0].upper() == "EQU" or rest[0] == "="):
                continue
            if "." not in head and head in addrs:
                pending.append(head)
            if not rest:
                continue
            op = rest[0].upper()
        else:
            op = line.split()[0].upper()
        if op in ("SUBROUTINE", "ORG", "PROCESSOR", "MACRO", "ENDM"):
            if op == "ORG":
                pending = []
            continue
        if pending:
            code = not (op.split(".")[0] in _DATA_OPS
                        or op.startswith(_DATA_PREFIXES))
            for name in pending:
                out.setdefault(addrs[name], (name, code))
            pending = []
    return out


def original_routines(main_asm: str, main_sym: str) -> dict[int, str]:
    """The address of each global label of the original, so that the
    nearest one at or below a program counter names the routine it's
    in."""
    return {a: name for a, (name, _) in
            original_labels(main_asm, main_sym).items()}


def routine_at(routines: dict[int, str], pc: int) -> str:
    """The name of the routine in `routines` (address to name) that
    `pc` is in, as name+offset."""
    below = [a for a in routines if a <= pc]
    if not below:
        return f"${pc:04X}"
    a = max(below)
    return f"{routines[a]}+{pc - a}" if pc != a else routines[a]


@dataclass
class Coverage:
    """Which of the original's routines a run executed. `routines` maps
    each code label's name to its address, the end of its code (the
    next label of either kind), and the number of its instructions the
    run executed."""

    routines: dict[str, tuple[int, int, int]]

    @property
    def reached(self) -> list[str]:
        return [n for n, (_, _, k) in self.routines.items() if k]

    @property
    def missed(self) -> list[str]:
        return [n for n, (_, _, k) in self.routines.items() if not k]

    def summary(self) -> str:
        return (f"{len(self.reached)} of {len(self.routines)} routines "
                "executed")


def coverage(seen: bytes, labels: dict[int, tuple[str, bool]],
             spans: tuple[tuple[int, int], ...] | None = None) -> Coverage:
    """The coverage of the code labels in `labels` (from
    `original_labels`) by a run whose `Machine.seen` is `seen`, counting
    only the labels inside `spans` (start, end) when it's given."""
    addrs = sorted(labels)
    out: dict[str, tuple[int, int, int]] = {}
    for i, a in enumerate(addrs):
        name, code = labels[a]
        if not code:
            continue
        if spans is not None and not any(s <= a < e for s, e in spans):
            continue
        end = addrs[i + 1] if i + 1 < len(addrs) else 0xC000
        out[name] = (a, end, sum(seen[a:end]))
    return Coverage(out)
