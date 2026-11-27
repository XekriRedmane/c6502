// A Disk II controller and its drives, emulated at the level of the
// nibbles on the disk, for programs that drive the disk themselves,
// ported from sim/disk2.py.
//
// A .dsk image (DOS 3.3 sector order) becomes a track of nibbles for
// each of its 35 tracks, in the format DOS 3.3's INIT writes. Each track
// is a gap of sync bytes and then 16 sectors in physical order, each an
// address field, a gap, a data field, and a gap.
//
//     address field   D5 AA 96, volume, track, sector, checksum (each
//                     in 4-and-4 encoding, two nibbles), DE AA EB
//     data field      D5 AA AD, 342 nibbles of 6-and-2 encoded data,
//                     a checksum nibble, DE AA EB
//
// Physical sector p holds the image's sector `kPhysicalToFile[p]` of
// the track, which is how DOS 3.3's RWTS finds logical sector s. The
// volume number is 254.
//
// The 6-and-2 encoding follows DOS 3.3's PRENIB16 and POSTNB16. Byte i
// of the sector gives its top six bits to the i'th of the 256 main
// values and its low two bits, swapped, to bits 2k and 2k+1 of
// auxiliary value i mod 86, where k is i div 86. The 86 auxiliary values
// go out first, in index order, then the 256 main values, each written
// as the nibble of its exclusive-or with the value before it, and the
// last value's nibble is the checksum.
//
// The controller's soft switches are at $C080 + 16 * slot. Even
// addresses turn a stepper phase off and odd ones turn it on ($x0-$x7),
// then motor off and on ($x8, $x9), drive 1 and 2 ($xA, $xB), Q6 off
// and on ($xC, $xD), and Q7 off and on ($xE, $xF). Reading Q6 off with
// Q7 off reads the data latch.
//
// The head moves a half track each time a phase next to the one it sits
// over is turned on, as the stepper does. It reads the track that
// `Drive::track_at` gives for its half track, which is the whole track
// at or below it unless a test says otherwise.
//
// The disk turns one nibble under the head for each read or write of
// the data latch while the motor runs, so a read of the latch always
// returns the next nibble, with bit 7 set. `DiskII::position` counts the
// nibbles that have passed, and the nibble under the head is that count
// modulo the track's length, the same on every track. A real disk turns
// with time, one nibble per 32 cycles, but the disk code polls the
// latch faster than that and takes each nibble once, so it sees the
// same nibbles either way. Counting accesses keeps writes in step with
// reads. DOS writes its sync bytes 40 cycles apart, as 10-bit bytes,
// and a disk that turned with time would put them a quarter of a nibble
// further along than a reader that takes 8 bits per nibble expects
// them. It also makes what the kept disk code reads independent of the
// cycle at which the program calls it, which differs between two
// programs being compared.
//
// In write mode (Q7 on), a write to Q6 on loads the latch, and the next
// access to Q6 off writes the latch's nibble under the head and moves
// the disk on one nibble. `Drive::to_dsk` decodes the tracks back into
// a .dsk image.
//
// A position that holds no nibble, which is every position of an
// unformatted track, reads as noise, as a real drive's does: a byte with
// bit 7 set that's a function of the position, and never $D5 or $AA, so
// that the noise never makes a field's prologue.
//
// The motor runs for a second (1,020,484 cycles) after it's turned off,
// as the controller's timer keeps it running, and the latch doesn't
// change while the motor is stopped.
#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <utility>
#include <vector>

namespace c6502::sim::disk2 {

inline constexpr std::array<int, 16> kPhysicalToFile = {0x0, 0x7, 0xE, 0x6, 0xD, 0x5, 0xC, 0x4,
                                                        0xB, 0x3, 0xA, 0x2, 0x9, 0x1, 0x8, 0xF};
inline constexpr int kTracks = 35;
inline constexpr int kHalfTracks = 2 * kTracks;
inline constexpr std::size_t kTrackNibbles = 6656;
inline constexpr int kVolume = 254;
inline constexpr std::size_t kDskBytes = kTracks * 16 * 256;
inline constexpr std::uint64_t kSpinDownCycles = 1'020'484;

using Bytes = std::vector<std::uint8_t>;

// Returns the 343 nibbles of a data field's 342 values and checksum, for
// the 256 bytes of `data`.
Bytes encode_sector(std::span<const std::uint8_t> data);

// Returns the 256 bytes of a data field's 343 nibbles, or none when a
// nibble isn't a data nibble or the checksum doesn't match.
std::optional<Bytes> decode_sector(std::span<const std::uint8_t> nibbles);

// Returns the nibbles of `track`, whose physical sector p holds
// `sectors[p]`.
Bytes encode_track(int track, const std::vector<Bytes>& sectors, int volume = kVolume);

// Returns the nibbles of each track of a .dsk image in DOS 3.3 order.
std::vector<Bytes> dsk_to_tracks(std::span<const std::uint8_t> dsk);

struct SectorAddress {
  int track;
  int sector;
  auto operator<=>(const SectorAddress&) const = default;
};

// Returns the sectors found on a track's nibbles, by the track and
// physical sector of each address field, the track read once around
// from its start and on past the end into the start again.
std::map<SectorAddress, Bytes> decode_track(std::span<const std::uint8_t> nibbles);

// One drive and the disk in it. `tracks` holds each whole track's
// nibbles. `track_at` maps each half track the head can be over to the
// index in `tracks` of the nibbles it reads there, or none for an
// unformatted track.
struct Drive {
  std::vector<Bytes> tracks;
  std::vector<std::optional<int>> track_at = default_track_at();
  int half_track = 0;
  int phases = 0;
  bool write_protected = false;
  // The tracks written since the disk was put in.
  std::set<std::optional<int>> written;

  static std::vector<std::optional<int>> default_track_at();
  static Drive from_dsk(std::span<const std::uint8_t> dsk);
  // An unformatted disk, whose tracks read as zeros.
  static Drive blank();

  // Returns the disk as a .dsk image. Each sector found on a track under
  // its own track number replaces that sector of `base` (zeros if
  // null).
  Bytes to_dsk(const Bytes* base = nullptr) const;

  // Moves the head toward a phase next to the one it's over, if exactly
  // one of them is on.
  void step();

  // Returns the nibbles under the head, or null for an unformatted
  // track.
  Bytes* nibbles();
};

// A Disk II controller in `slot`, with up to two drives. A drive that's
// null is empty. The drives belong to the machine, which swaps them.
class DiskII {
 public:
  DiskII(std::array<Drive*, 2> drives_in, int slot_in = 6) : drives(drives_in), slot(slot_in) {}

  std::array<Drive*, 2> drives;
  int slot;
  int selected = 0;
  bool motor = false;
  std::int64_t motor_off_at = -1;
  bool q6 = false;
  bool q7 = false;
  std::uint8_t latch = 0;
  // The nibbles that have passed under the head.
  std::uint64_t position = 0;

  std::uint16_t base() const { return static_cast<std::uint16_t>(0xC080 + 16 * slot); }
  bool spinning(std::uint64_t cycles) const {
    return motor || static_cast<std::int64_t>(cycles) < motor_off_at;
  }
  Drive* drive() const { return drives[static_cast<std::size_t>(selected)]; }

  // Performs an access to soft switch `addr` at cycle `cycles`, with
  // `value` the byte a write stores, and returns the byte a read gets.
  std::uint8_t access(std::uint16_t addr, bool write, std::uint8_t value, std::uint64_t cycles);

 private:
  std::uint8_t read_latch(std::uint64_t cycles);
  void write_latch(std::uint64_t cycles);

  bool loaded_ = false;
};

}  // namespace c6502::sim::disk2
