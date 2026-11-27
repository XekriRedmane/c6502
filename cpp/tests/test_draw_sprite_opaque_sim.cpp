// examples/draw_sprite_opaque.c run in both pipelines, from
// tests/test_draw_sprite_opaque_sim.py: `page_flag & 0x80` switches the
// row base between `screen_row_addr_hi` (page 1) and
// `screen_row_addr_hi2` (page 2). The optimizer once dropped the switch
// and always used `screen_row_addr_hi`.
//
// For sprite_y = 0, screen_row_addr_hi[0] is $20 and
// screen_row_addr_hi2[0] is $40, so the sprite's byte $A5 lands at $2000
// for page_flag $00 and at $4000 for page_flag $80.
#include <string>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

// The example's source, read as Python's text mode reads it, followed
// by a `main` that draws on both pages and returns the byte found on
// page 1 in its low byte and the one on page 2 in its high byte.
std::string source() {
  std::string body = read_repo_file("examples/draw_sprite_opaque.c");
  std::string lf;
  for (const char c : body) {
    if (c != '\r') lf += c;
  }
  return lf + R"c(
uint8_t tile_data[1] = { 0xA5 };

int main(void) {
    /* Clear both candidate target bytes so we can tell which was
       written. */
    *(volatile uint8_t *)0x2000 = 0x00;
    *(volatile uint8_t *)0x4000 = 0x00;
    /* page_flag=0x00 -> should write to $2000 (page 1). */
    draw_sprite_opaque(1, 1, 0, 0, tile_data, 0x00);
    uint8_t page1 = *(volatile uint8_t *)0x2000;
    /* Reset and try with page_flag=0x80 -> should write to $4000
       (page 2). */
    *(volatile uint8_t *)0x2000 = 0x00;
    *(volatile uint8_t *)0x4000 = 0x00;
    draw_sprite_opaque(1, 1, 0, 0, tile_data, 0x80);
    uint8_t page2 = *(volatile uint8_t *)0x4000;
    /* Pack into the return value: low byte = page1, high byte =
       page2. The test expects 0xA5A5 (both wrote correctly). */
    return ((int)page2 << 8) | page1;
}
)c";
}

std::int64_t run(bool optimize) {
  const sim::SimResult result = build_sim(source(), optimize).run(2'000'000);
  CHECK_FALSE(result.timed_out);
  return result.return_int() & 0xFFFF;
}

}  // namespace

TEST_CASE("draw_sprite_opaque_sim: the unoptimized program writes on the page the flag selects") {
  CHECK(run(false) == 0xA5A5);
}

TEST_CASE("draw_sprite_opaque_sim: the optimized program writes on the page the flag selects") {
  CHECK(run(true) == 0xA5A5);
}

}  // namespace c6502::suite
