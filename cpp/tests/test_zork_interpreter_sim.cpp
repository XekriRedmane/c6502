// The cases run only with ZORK_DSK set to the path of
// `Zork I r15 (4am crack).dsk` from
// https://github.com/XekriRedmane/zorki-r15-reveng.
#include <memory>
#include <string>
#include <vector>

#include "doctest.h"
#include "game_case.h"

namespace {

namespace suite = c6502::suite;
namespace games = c6502::sim::games;
namespace apple2 = c6502::sim::apple2;

const bool kNoDisk = !games::env("ZORK_DSK").has_value();

const std::string& exe_bytes() {
  static const std::string bytes = [] {
    const suite::TempDir tmp;
    const std::string path = suite::build_example(
        tmp, "zork",
        {"main", "decode", "objects", "numbers", "input", "pages", "zstring", "output", "disk", "prompts", "save"},
        "interpreter.ld", suite::Helpers::kRuntimeLibrary);
    return suite::read_file(path);
  }();
  return bytes;
}

const games::Bytes& disk() {
  static const games::Bytes dsk = games::read_bytes(*games::env("ZORK_DSK"));
  return dsk;
}

struct Pair {
  std::unique_ptr<games::ZorkRun> original;
  std::unique_ptr<games::ZorkRun> ours;
};

Pair compare(const games::ZorkSession& session) {
  Pair p{games::zork_original(disk(), session), games::zork_program(exe_bytes(), disk(), session)};
  CHECK(p.original->run() == apple2::Stop::kHalt);
  CHECK(p.ours->run() == apple2::Stop::kHalt);
  const apple2::Apple2& a = p.original->machine;
  const apple2::Apple2& b = p.ours->machine;
  CHECK(b.calls == a.calls);
  CHECK(b.written == a.written);
  CHECK(b.printer == a.printer);
  CHECK(b.text() == a.text());
  return p;
}

}  // namespace

TEST_CASE("zork_interpreter: the walk makes Zork's own calls" * doctest::skip(kNoDisk)) {
  const Pair p = compare(games::zork_session("walk"));
  const apple2::Apple2& ours = p.ours->machine;
  const std::string screen = ours.text();
  CHECK(suite::contains(screen, "-- END OF SESSION --"));
  CHECK(suite::contains(screen, "RANK OF AMATEUR"));
  // A save writes the state, the stack, and the dynamic memory.
  CHECK(ours.written.size() > 3);
}

TEST_CASE("zork_interpreter: the transcript, a save, and a restart make Zork's own calls" * doctest::skip(kNoDisk)) {
  const Pair p = compare(games::zork_session("transcript"));
  const apple2::Apple2& ours = p.ours->machine;
  std::string printed;
  for (const apple2::Call& call : ours.calls) {
    const auto* m = std::get_if<apple2::MonitorCall>(&call);
    if (m != nullptr && m->routine == apple2::Routine::kCout1) printed += static_cast<char>(*m->a & 0x7F);
  }
  CHECK(suite::contains(printed, "[MORE]"));
  CHECK(suite::contains(printed, "POSITION (0-7):"));
  // Position 3 starts 3 * 64 sectors in, on track 15.
  REQUIRE_FALSE(ours.written.empty());
  CHECK(ours.written[0].slot == 0x60);
  CHECK(ours.written[0].drive == 2);
  CHECK(ours.written[0].track == 15);
  std::string transcript;
  for (const std::uint8_t c : ours.printer) transcript += static_cast<char>(c & 0x7F);
  CHECK(suite::contains(transcript, ">NORTH"));
  CHECK_FALSE(suite::contains(transcript, "[MORE]"));
}
