"""A Disk II controller and its drives, emulated at the level of the
nibbles on the disk, for programs that drive the disk themselves.

A .dsk image (DOS 3.3 sector order) becomes a track of nibbles for each
of its 35 tracks, in the format DOS 3.3's INIT writes. Each track is a
gap of sync bytes and then 16 sectors in physical order, each an
address field, a gap, a data field, and a gap.

    address field   D5 AA 96, volume, track, sector, checksum (each
                    in 4-and-4 encoding, two nibbles), DE AA EB
    data field      D5 AA AD, 342 nibbles of 6-and-2 encoded data,
                    a checksum nibble, DE AA EB

Physical sector p holds the image's sector `PHYSICAL_TO_FILE[p]` of
the track, which is how DOS 3.3's RWTS finds logical sector s at
physical sector `FILE_TO_PHYSICAL[s]`. The volume number is 254.

The 6-and-2 encoding follows DOS 3.3's PRENIB16 and POSTNB16. Byte i
of the sector gives its top six bits to the i'th of the 256 main
values and its low two bits, swapped, to bits 2k and 2k+1 of auxiliary
value i mod 86, where k is i div 86. The 86 auxiliary values go out
first, in index order, then the 256 main values, each written as the
nibble of its exclusive-or with the value before it, and the last
value's nibble is the checksum.

The controller's soft switches are at $C080 + 16 * slot. Even
addresses turn a stepper phase off and odd ones turn it on ($x0-$x7),
then motor off and on ($x8, $x9), drive 1 and 2 ($xA, $xB), Q6 off
and on ($xC, $xD), and Q7 off and on ($xE, $xF). Reading Q6 off with
Q7 off reads the data latch.

The head moves a half track each time a phase next to the one it sits
over is turned on, as the stepper does. It reads the track that
`Drive.track_at` gives for its half track, which is the whole track at
or below it unless a test says otherwise.

The disk turns one nibble under the head for each read or write of the
data latch while the motor runs, so a read of the latch always returns
the next nibble, with bit 7 set. `DiskII.position` counts the nibbles
that have passed, and the nibble under the head is that count modulo
the track's length, the same on every track. A real disk turns with
time, one nibble per 32 cycles, but the disk code polls the latch
faster than that and takes each nibble once, so it sees the same
nibbles either way. Counting accesses keeps writes in step with reads.
DOS writes its sync bytes 40 cycles apart, as 10-bit bytes, and a disk
that turned with time would put them a quarter of a nibble further
along than a reader that takes 8 bits per nibble expects them. It also
makes what the kept disk code reads independent of the cycle at which
the program calls it, which differs between two programs being
compared.

In write mode (Q7 on), a write to Q6 on loads the latch, and the next
access to Q6 off writes the latch's nibble under the head and moves
the disk on one nibble. `Drive.to_dsk` decodes the tracks back into a
.dsk image.

A position that holds no nibble, which is every position of an
unformatted track, reads as noise, as a real drive's does: a byte with
bit 7 set that's a function of the position, and never $D5 or $AA, so
that the noise never makes a field's prologue.

The motor runs for a second (1,020,484 cycles) after it's turned off,
as the controller's timer keeps it running, and the latch doesn't
change while the motor is stopped.
"""
from __future__ import annotations

from dataclasses import dataclass, field

# Physical sector p of a track holds the .dsk image's sector
# PHYSICAL_TO_FILE[p] of that track, and DOS 3.3's logical sector s is
# at physical sector FILE_TO_PHYSICAL[s].
PHYSICAL_TO_FILE = [0x0, 0x7, 0xE, 0x6, 0xD, 0x5, 0xC, 0x4,
                    0xB, 0x3, 0xA, 0x2, 0x9, 0x1, 0x8, 0xF]
FILE_TO_PHYSICAL = [PHYSICAL_TO_FILE.index(s) for s in range(16)]

TRACKS = 35
HALF_TRACKS = 2 * TRACKS
TRACK_NIBBLES = 6656
VOLUME = 254

# The cycles the motor runs after it's turned off (the controller's
# one-second timer).
SPIN_DOWN_CYCLES = 1_020_484

# The 64 nibbles that encode a 6-bit value, in value order.
WRITE_TABLE = bytes([
    0x96, 0x97, 0x9A, 0x9B, 0x9D, 0x9E, 0x9F, 0xA6,
    0xA7, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB2, 0xB3,
    0xB4, 0xB5, 0xB6, 0xB7, 0xB9, 0xBA, 0xBB, 0xBC,
    0xBD, 0xBE, 0xBF, 0xCB, 0xCD, 0xCE, 0xCF, 0xD3,
    0xD6, 0xD7, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE,
    0xDF, 0xE5, 0xE6, 0xE7, 0xE9, 0xEA, 0xEB, 0xEC,
    0xED, 0xEE, 0xEF, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6,
    0xF7, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
])
READ_TABLE = {n: v for v, n in enumerate(WRITE_TABLE)}

# The gaps of sync bytes: before the first sector, between an address
# field and its data field, and after each data field.
GAP1, GAP2, GAP3 = 48, 6, 27


def _four_and_four(value: int) -> bytes:
    return bytes([(value >> 1) | 0xAA, value | 0xAA])


def encode_sector(data: bytes) -> bytes:
    """The 343 nibbles of a data field's 342 values and checksum, for
    the 256 bytes of `data`."""
    aux = [0] * 86
    for i, b in enumerate(data):
        aux[i % 86] |= (((b >> 1) & 1) | ((b & 1) << 1)) << (2 * (i // 86))
    values = aux + [b >> 2 for b in data]
    out = bytearray()
    prev = 0
    for v in values:
        out.append(WRITE_TABLE[v ^ prev])
        prev = v
    out.append(WRITE_TABLE[prev])
    return bytes(out)


def decode_sector(nibbles: bytes) -> bytes | None:
    """The 256 bytes of a data field's 343 nibbles, or None when a
    nibble isn't a data nibble or the checksum doesn't match."""
    values = []
    prev = 0
    for n in nibbles[:342]:
        v = READ_TABLE.get(n)
        if v is None:
            return None
        prev ^= v
        values.append(prev)
    if READ_TABLE.get(nibbles[342]) != prev:
        return None
    aux, main = values[:86], values[86:]
    out = bytearray(256)
    for i in range(256):
        bits = (aux[i % 86] >> (2 * (i // 86))) & 3
        out[i] = (main[i] << 2) | ((bits & 1) << 1) | (bits >> 1)
    return bytes(out)


def encode_track(track: int, sectors: list[bytes],
                 volume: int = VOLUME) -> bytearray:
    """The nibbles of `track`, whose physical sector p holds
    `sectors[p]`."""
    out = bytearray(b"\xFF" * GAP1)
    for p, data in enumerate(sectors):
        out += b"\xD5\xAA\x96"
        out += _four_and_four(volume) + _four_and_four(track)
        out += _four_and_four(p) + _four_and_four(volume ^ track ^ p)
        out += b"\xDE\xAA\xEB" + b"\xFF" * GAP2
        out += b"\xD5\xAA\xAD" + encode_sector(data) + b"\xDE\xAA\xEB"
        out += b"\xFF" * GAP3
    out += b"\xFF" * (TRACK_NIBBLES - len(out))
    return out


def dsk_to_tracks(dsk: bytes) -> list[bytearray]:
    """The nibbles of each track of a .dsk image in DOS 3.3 order."""
    tracks = []
    for t in range(TRACKS):
        base = t * 16 * 256
        sectors = [bytes(dsk[base + s * 256:base + s * 256 + 256])
                   for s in PHYSICAL_TO_FILE]
        tracks.append(encode_track(t, sectors))
    return tracks


def _decode_4_and_4(nib: bytes, i: int) -> int:
    return ((nib[i] << 1) | 1) & nib[i + 1]


def decode_track(nibbles: bytes) -> dict[tuple[int, int], bytes]:
    """The sectors found on a track's nibbles, by (track, physical
    sector) from each address field, the track read once around from
    its start and on past the end into the start again."""
    n = len(nibbles)
    ring = bytes(nibbles) + bytes(nibbles[:400])
    found: dict[tuple[int, int], bytes] = {}
    i = 0
    while i < n:
        if ring[i:i + 3] != b"\xD5\xAA\x96":
            i += 1
            continue
        track = _decode_4_and_4(ring, i + 5)
        sector = _decode_4_and_4(ring, i + 7)
        j = i + 14
        # The data field starts within a few dozen nibbles of the
        # address field, or the sector has none.
        k = ring.find(b"\xD5\xAA\xAD", j, j + 64)
        if k >= 0:
            data = decode_sector(ring[k + 3:k + 3 + 343])
            if data is not None:
                found[(track, sector)] = data
        i = j
    return found


def _noise(pos: int) -> int:
    """The byte an unformatted position reads as."""
    n = 0x80 | ((pos * 2654435761) >> 13) & 0x7F
    return n ^ 1 if n in (0xD5, 0xAA) else n


@dataclass
class Drive:
    """One drive and the disk in it. `tracks` holds each whole track's
    nibbles. `track_at` maps each half track the head can be over to
    the index in `tracks` of the nibbles it reads there, or None for
    an unformatted track."""

    tracks: list[bytearray]
    track_at: list[int | None] = field(default_factory=lambda: [
        ht // 2 for ht in range(HALF_TRACKS)])
    half_track: int = 0
    phases: int = 0
    write_protected: bool = False
    # The tracks written since the disk was put in.
    written: set[int] = field(default_factory=set)

    @classmethod
    def from_dsk(cls, dsk: bytes) -> "Drive":
        return cls(dsk_to_tracks(dsk))

    @classmethod
    def blank(cls) -> "Drive":
        """An unformatted disk, whose tracks read as zeros."""
        return cls([bytearray(TRACK_NIBBLES) for _ in range(TRACKS)])

    def to_dsk(self, base: bytes | None = None) -> bytearray:
        """The disk as a .dsk image. Each sector found on a track under
        its own track number replaces that sector of `base` (zeros if
        None)."""
        out = bytearray(base) if base is not None else bytearray(
            TRACKS * 16 * 256)
        for t, nibbles in enumerate(self.tracks):
            for (track, p), data in decode_track(nibbles).items():
                if track == t and p < 16:
                    at = (t * 16 + PHYSICAL_TO_FILE[p]) * 256
                    out[at:at + 256] = data
        return out

    def step(self) -> None:
        """Moves the head toward a phase next to the one it's over, if
        exactly one of them is on."""
        here = self.half_track & 3
        direction = 0
        if self.phases & (1 << ((here + 1) & 3)):
            direction += 1
        if self.phases & (1 << ((here + 3) & 3)):
            direction -= 1
        if direction:
            self.half_track = max(0, min(HALF_TRACKS - 1,
                                         self.half_track + direction))

    def nibbles(self) -> bytearray | None:
        t = self.track_at[self.half_track]
        return None if t is None else self.tracks[t]


@dataclass
class DiskII:
    """A Disk II controller in `slot`, with up to two drives. A drive
    that's None is empty."""

    drives: list[Drive | None]
    slot: int = 6
    selected: int = 0
    motor: bool = False
    motor_off_at: int = -1
    q6: bool = False
    q7: bool = False
    latch: int = 0
    # The nibbles that have passed under the head.
    position: int = 0
    _loaded: bool = False

    @property
    def base(self) -> int:
        return 0xC080 + 16 * self.slot

    def spinning(self, cycles: int) -> bool:
        return self.motor or cycles < self.motor_off_at

    def drive(self) -> Drive | None:
        return self.drives[self.selected]

    def access(self, addr: int, write: bool, value: int,
               cycles: int) -> int:
        """Performs an access to soft switch `addr` at cycle `cycles`,
        with `value` the byte a write stores, and returns the byte a
        read gets."""
        sw = addr & 0x0F
        drive = self.drive()
        if sw < 8:
            if drive is not None:
                bit = 1 << (sw >> 1)
                if sw & 1:
                    drive.phases |= bit
                else:
                    drive.phases &= ~bit
                drive.step()
        elif sw == 8:
            if self.motor:
                self.motor_off_at = cycles + SPIN_DOWN_CYCLES
            self.motor = False
        elif sw == 9:
            self.motor = True
        elif sw in (0xA, 0xB):
            self.selected = sw & 1
        elif sw == 0xC:
            self.q6 = False
            if self.q7:
                self._write_latch(cycles)
        elif sw == 0xD:
            self.q6 = True
            if self.q7 and write:
                self.latch = value
                self._loaded = True
        elif sw == 0xE:
            self.q7 = False
        else:
            self.q7 = True
            if write and self.q6:
                self.latch = value
                self._loaded = True
        # Only an even address reads the latch.
        if sw & 1:
            return 0
        if not self.q7 and not self.q6:
            return self._read_latch(cycles)
        if self.q6 and not self.q7:
            # Sense write protect: bit 7 of the status.
            return 0x80 if drive is not None and drive.write_protected \
                else 0
        return 0

    def _read_latch(self, cycles: int) -> int:
        drive = self.drive()
        if drive is None or not self.spinning(cycles):
            return self.latch & 0x7F
        nibbles = drive.nibbles()
        pos = self.position
        self.position = pos + 1
        n = nibbles[pos % len(nibbles)] if nibbles is not None else 0
        if n < 0x80:
            n = _noise(pos)
        self.latch = n
        return n

    def _write_latch(self, cycles: int) -> None:
        drive = self.drive()
        if (drive is None or not self._loaded or drive.write_protected
                or not self.spinning(cycles)):
            return
        nibbles = drive.nibbles()
        if nibbles is None:
            return
        nibbles[self.position % len(nibbles)] = self.latch
        self.position += 1
        self._loaded = False
        drive.written.add(drive.track_at[drive.half_track])
