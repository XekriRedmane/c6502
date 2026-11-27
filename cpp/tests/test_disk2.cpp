#include <algorithm>
#include <cstdint>
#include <vector>

#include "doctest.h"
#include "py_random.h"
#include "sim/disk2.h"

namespace {

namespace disk2 = c6502::sim::disk2;

std::vector<std::uint8_t> random_bytes(c6502::py_random::Random& rnd, std::size_t n) {
  std::vector<std::uint8_t> out;
  for (std::size_t i = 0; i < n; ++i) out.push_back(static_cast<std::uint8_t>(rnd.randrange(256)));
  return out;
}

}  // namespace

TEST_CASE("disk2: a sector's data field decodes to its bytes") {
  c6502::py_random::Random rnd(1);
  for (int k = 0; k < 50; ++k) {
    const std::vector<std::uint8_t> data = random_bytes(rnd, 256);
    const std::vector<std::uint8_t> nibbles = disk2::encode_sector(data);
    CHECK(nibbles.size() == 343);
    CHECK(std::all_of(nibbles.begin(), nibbles.end(), [](std::uint8_t n) { return n >= 0x96; }));
    CHECK(disk2::decode_sector(nibbles) == data);
  }
}

TEST_CASE("disk2: a .dsk image comes back from its tracks") {
  c6502::py_random::Random rnd(2);
  const std::vector<std::uint8_t> dsk = random_bytes(rnd, disk2::kDskBytes);
  CHECK(disk2::Drive::from_dsk(dsk).to_dsk() == dsk);
}

TEST_CASE("disk2: the protection track's sector is 342 nibbles of $AF") {
  // A sector of $36 $00 pairs, then $37 $00, then $34 $00 in its three
  // thirds (the game's track 5) is what VERIFY_DATA_FIELD checks for.
  std::vector<std::uint8_t> data;
  for (int i = 0; i < 43; ++i) data.insert(data.end(), {0x36, 0});
  for (int i = 0; i < 43; ++i) data.insert(data.end(), {0x37, 0});
  for (int i = 0; i < 42; ++i) data.insert(data.end(), {0x34, 0});
  const std::vector<std::uint8_t> nibbles = disk2::encode_sector(data);
  CHECK(std::vector<std::uint8_t>(nibbles.begin(), nibbles.begin() + 342) == std::vector<std::uint8_t>(342, 0xAF));
}
