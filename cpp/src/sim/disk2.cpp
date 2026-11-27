#include "sim/disk2.h"

#include <algorithm>
#include <stdexcept>

namespace c6502::sim::disk2 {

namespace {

// The 64 nibbles that encode a 6-bit value, in value order.
constexpr std::array<std::uint8_t, 64> kWriteTable = {
    0x96, 0x97, 0x9A, 0x9B, 0x9D, 0x9E, 0x9F, 0xA6, 0xA7, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB2, 0xB3,
    0xB4, 0xB5, 0xB6, 0xB7, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xCB, 0xCD, 0xCE, 0xCF, 0xD3,
    0xD6, 0xD7, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE5, 0xE6, 0xE7, 0xE9, 0xEA, 0xEB, 0xEC,
    0xED, 0xEE, 0xEF, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

// The value of each data nibble, or -1 for a byte that isn't one.
const std::array<int, 256>& read_table() {
  static const std::array<int, 256> table = [] {
    std::array<int, 256> t{};
    t.fill(-1);
    for (int v = 0; v < 64; ++v) t[kWriteTable[static_cast<std::size_t>(v)]] = v;
    return t;
  }();
  return table;
}

// The gaps of sync bytes: before the first sector, between an address
// field and its data field, and after each data field.
constexpr std::size_t kGap1 = 48;
constexpr std::size_t kGap2 = 6;
constexpr std::size_t kGap3 = 27;

void append(Bytes& out, std::initializer_list<std::uint8_t> bytes) { out.insert(out.end(), bytes); }

void four_and_four(Bytes& out, int value) {
  out.push_back(static_cast<std::uint8_t>((value >> 1) | 0xAA));
  out.push_back(static_cast<std::uint8_t>(value | 0xAA));
}

int decode_4_and_4(std::span<const std::uint8_t> nib, std::size_t i) {
  return ((nib[i] << 1) | 1) & nib[i + 1];
}

// The byte an unformatted position reads as.
std::uint8_t noise(std::uint64_t pos) {
  const auto n = static_cast<std::uint8_t>(0x80 | (((pos * 2654435761ULL) >> 13) & 0x7F));
  return n == 0xD5 || n == 0xAA ? n ^ 1 : n;
}

}  // namespace

Bytes encode_sector(std::span<const std::uint8_t> data) {
  std::array<int, 86> aux{};
  for (std::size_t i = 0; i < data.size(); ++i) {
    const int b = data[i];
    aux[i % 86] |= (((b >> 1) & 1) | ((b & 1) << 1)) << (2 * (i / 86));
  }
  std::vector<int> values(aux.begin(), aux.end());
  for (const std::uint8_t b : data) values.push_back(b >> 2);
  Bytes out;
  int prev = 0;
  for (const int v : values) {
    out.push_back(kWriteTable.at(static_cast<std::size_t>(v ^ prev)));
    prev = v;
  }
  out.push_back(kWriteTable.at(static_cast<std::size_t>(prev)));
  return out;
}

std::optional<Bytes> decode_sector(std::span<const std::uint8_t> nibbles) {
  std::vector<int> values;
  int prev = 0;
  for (std::size_t i = 0; i < std::min<std::size_t>(342, nibbles.size()); ++i) {
    const int v = read_table()[nibbles[i]];
    if (v < 0) return std::nullopt;
    prev ^= v;
    values.push_back(prev);
  }
  if (nibbles.size() < 343) throw std::out_of_range("a data field has fewer than 343 nibbles");
  if (read_table()[nibbles[342]] != prev) return std::nullopt;
  Bytes out(256);
  for (std::size_t i = 0; i < 256; ++i) {
    const int bits = (values[i % 86] >> (2 * (i / 86))) & 3;
    out[i] = static_cast<std::uint8_t>((values[86 + i] << 2) | ((bits & 1) << 1) | (bits >> 1));
  }
  return out;
}

Bytes encode_track(int track, const std::vector<Bytes>& sectors, int volume) {
  Bytes out(kGap1, 0xFF);
  for (std::size_t p = 0; p < sectors.size(); ++p) {
    const int ps = static_cast<int>(p);
    append(out, {0xD5, 0xAA, 0x96});
    four_and_four(out, volume);
    four_and_four(out, track);
    four_and_four(out, ps);
    four_and_four(out, volume ^ track ^ ps);
    append(out, {0xDE, 0xAA, 0xEB});
    out.insert(out.end(), kGap2, 0xFF);
    append(out, {0xD5, 0xAA, 0xAD});
    const Bytes field = encode_sector(sectors[p]);
    out.insert(out.end(), field.begin(), field.end());
    append(out, {0xDE, 0xAA, 0xEB});
    out.insert(out.end(), kGap3, 0xFF);
  }
  if (out.size() < kTrackNibbles) out.insert(out.end(), kTrackNibbles - out.size(), 0xFF);
  return out;
}

std::vector<Bytes> dsk_to_tracks(std::span<const std::uint8_t> dsk) {
  std::vector<Bytes> tracks;
  for (int t = 0; t < kTracks; ++t) {
    const std::size_t base = static_cast<std::size_t>(t) * 16 * 256;
    std::vector<Bytes> sectors;
    for (const int s : kPhysicalToFile) {
      const std::size_t at = std::min(base + static_cast<std::size_t>(s) * 256, dsk.size());
      const std::size_t end = std::min(at + 256, dsk.size());
      sectors.emplace_back(dsk.begin() + static_cast<std::ptrdiff_t>(at), dsk.begin() + static_cast<std::ptrdiff_t>(end));
    }
    tracks.push_back(encode_track(t, sectors));
  }
  return tracks;
}

std::map<SectorAddress, Bytes> decode_track(std::span<const std::uint8_t> nibbles) {
  const std::size_t n = nibbles.size();
  Bytes ring(nibbles.begin(), nibbles.end());
  ring.insert(ring.end(), nibbles.begin(), nibbles.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(400, n)));
  const std::span<const std::uint8_t> r(ring);
  const auto matches = [&r](std::size_t i, std::uint8_t a, std::uint8_t b, std::uint8_t c) {
    return i + 3 <= r.size() && r[i] == a && r[i + 1] == b && r[i + 2] == c;
  };
  std::map<SectorAddress, Bytes> found;
  std::size_t i = 0;
  while (i < n) {
    if (!matches(i, 0xD5, 0xAA, 0x96)) {
      ++i;
      continue;
    }
    const int track = decode_4_and_4(r, i + 5);
    const int sector = decode_4_and_4(r, i + 7);
    const std::size_t j = i + 14;
    // The data field starts within a few dozen nibbles of the address
    // field, or the sector has none.
    for (std::size_t k = j; k + 3 <= std::min(j + 64, r.size()); ++k) {
      if (!matches(k, 0xD5, 0xAA, 0xAD)) continue;
      const std::size_t start = std::min(k + 3, r.size());
      const std::size_t end = std::min(k + 3 + 343, r.size());
      if (std::optional<Bytes> data = decode_sector(r.subspan(start, end - start))) {
        found[SectorAddress{track, sector}] = std::move(*data);
      }
      break;
    }
    i = j;
  }
  return found;
}

std::vector<std::optional<int>> Drive::default_track_at() {
  std::vector<std::optional<int>> out;
  for (int ht = 0; ht < kHalfTracks; ++ht) out.emplace_back(ht / 2);
  return out;
}

Drive Drive::from_dsk(std::span<const std::uint8_t> dsk) {
  Drive d;
  d.tracks = dsk_to_tracks(dsk);
  return d;
}

Drive Drive::blank() {
  Drive d;
  d.tracks.assign(kTracks, Bytes(kTrackNibbles, 0));
  return d;
}

Bytes Drive::to_dsk(const Bytes* base) const {
  Bytes out = base != nullptr ? *base : Bytes(kDskBytes, 0);
  for (std::size_t t = 0; t < tracks.size(); ++t) {
    for (const auto& [where, data] : decode_track(tracks[t])) {
      if (where.track == static_cast<int>(t) && where.sector < 16) {
        const std::size_t at = (t * 16 + static_cast<std::size_t>(kPhysicalToFile[static_cast<std::size_t>(where.sector)])) * 256;
        std::copy(data.begin(), data.end(), out.begin() + static_cast<std::ptrdiff_t>(at));
      }
    }
  }
  return out;
}

void Drive::step() {
  const int here = half_track & 3;
  int direction = 0;
  if ((phases & (1 << ((here + 1) & 3))) != 0) ++direction;
  if ((phases & (1 << ((here + 3) & 3))) != 0) --direction;
  if (direction != 0) half_track = std::clamp(half_track + direction, 0, kHalfTracks - 1);
}

Bytes* Drive::nibbles() {
  const std::optional<int> t = track_at.at(static_cast<std::size_t>(half_track));
  return t ? &tracks.at(static_cast<std::size_t>(*t)) : nullptr;
}

std::uint8_t DiskII::access(std::uint16_t addr, bool write, std::uint8_t value, std::uint64_t cycles) {
  const unsigned sw = addr & 0x0F;
  Drive* d = drive();
  if (sw < 8) {
    if (d != nullptr) {
      const int bit = 1 << (sw >> 1);
      if ((sw & 1) != 0) {
        d->phases |= bit;
      } else {
        d->phases &= ~bit;
      }
      d->step();
    }
  } else if (sw == 8) {
    if (motor) motor_off_at = static_cast<std::int64_t>(cycles + kSpinDownCycles);
    motor = false;
  } else if (sw == 9) {
    motor = true;
  } else if (sw == 0xA || sw == 0xB) {
    selected = static_cast<int>(sw & 1);
  } else if (sw == 0xC) {
    q6 = false;
    if (q7) write_latch(cycles);
  } else if (sw == 0xD) {
    q6 = true;
    if (q7 && write) {
      latch = value;
      loaded_ = true;
    }
  } else if (sw == 0xE) {
    q7 = false;
  } else {
    q7 = true;
    if (write && q6) {
      latch = value;
      loaded_ = true;
    }
  }
  // Only an even address reads the latch.
  if ((sw & 1) != 0) return 0;
  if (!q7 && !q6) return read_latch(cycles);
  // Sense write protect: bit 7 of the status.
  if (q6 && !q7) return d != nullptr && d->write_protected ? 0x80 : 0;
  return 0;
}

std::uint8_t DiskII::read_latch(std::uint64_t cycles) {
  Drive* d = drive();
  if (d == nullptr || !spinning(cycles)) return latch & 0x7F;
  Bytes* nib = d->nibbles();
  const std::uint64_t pos = position;
  position = pos + 1;
  std::uint8_t n = nib != nullptr ? (*nib)[pos % nib->size()] : 0;
  if (n < 0x80) n = noise(pos);
  latch = n;
  return n;
}

void DiskII::write_latch(std::uint64_t cycles) {
  Drive* d = drive();
  if (d == nullptr || !loaded_ || d->write_protected || !spinning(cycles)) return;
  Bytes* nib = d->nibbles();
  if (nib == nullptr) return;
  (*nib)[position % nib->size()] = latch;
  ++position;
  loaded_ = false;
  d->written.insert(d->track_at.at(static_cast<std::size_t>(d->half_track)));
}

}  // namespace c6502::sim::disk2
