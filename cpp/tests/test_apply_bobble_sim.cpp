// The scenarios include a magnitude of 0 on each branch, the magnitude
// $7F, and 8-bit wrap both ways, and the expected bytes were computed by
// hand.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

// Source: examples/apply_bobble.c, inlined together with concrete
// definitions for `rescue_bobble` and `entity_floor_pos` (the only
// externs the function references), plus a `main` driver.
const std::string kProgram = R"c(
#include <stdint.h>

uint8_t entity_floor_pos[20];

/* 7-entry signed-magnitude delta table: bit 7 set = descend (row +=
 * magnitude), bit 7 clear = ascend (row -= magnitude). */
const uint8_t rescue_bobble[7] = {
    0x02,  /* idx 0: ascend by 2 */
    0x82,  /* idx 1: descend by 2 */
    0x01,  /* idx 2: ascend by 1 */
    0x81,  /* idx 3: descend by 1 */
    0x00,  /* idx 4: ascend by 0  (no-op) */
    0x80,  /* idx 5: descend by 0 (no-op) */
    0x7F,  /* idx 6: ascend by 127 */
};

[[c6502::zp_abi]]
static void apply_bobble(uint8_t slot, uint8_t bobble_idx)
{
    uint8_t bobble    = rescue_bobble[bobble_idx];
    uint8_t magnitude = bobble & 0x7F;
    if (bobble & 0x80) {
        entity_floor_pos[slot] = (uint8_t)(entity_floor_pos[slot] + magnitude);
    } else {
        entity_floor_pos[slot] = (uint8_t)(entity_floor_pos[slot] - magnitude);
    }
}

/* Each scenario writes 2 bytes: [pre-call row, post-call row]. */
uint8_t result_log[64];
uint8_t log_idx;

void scenario(uint8_t slot, uint8_t bobble_idx, uint8_t initial) {
    entity_floor_pos[slot] = initial;
    uint8_t base = log_idx;
    result_log[(uint8_t)(base + 0)] = entity_floor_pos[slot];
    apply_bobble(slot, bobble_idx);
    result_log[(uint8_t)(base + 1)] = entity_floor_pos[slot];
    log_idx = (uint8_t)(base + 2);
}

int main(void) {
    log_idx = 0;

    /* 1. Ascend by 2: 100 -> 98. */
    scenario(0, 0, 100);
    /* 2. Descend by 2: 100 -> 102. */
    scenario(1, 1, 100);
    /* 3. Ascend by 0: 50 -> 50 (no-op). */
    scenario(2, 4, 50);
    /* 4. Descend by 0: 50 -> 50 (no-op). */
    scenario(3, 5, 50);
    /* 5. Ascend by 127: 200 -> 73. */
    scenario(4, 6, 200);
    /* 6. Ascend by 1 on the last slot: 10 -> 9. */
    scenario(19, 2, 10);
    /* 7. Descend by 1 with wrap: 255 -> 0. */
    scenario(10, 3, 255);
    /* 8. Ascend by 2 with wrap: 0 -> 254. */
    scenario(5, 0, 0);

    return (int)log_idx;
}
)c";
// Each row is the pre-call and post-call byte at the scenario's slot.
const Bytes kExpected = {100, 98, 100, 102, 50, 50, 50, 50, 200, 73, 10, 9, 255, 0, 0, 254};

LoggedRun run(bool optimize) { return run_logged(kProgram, optimize, "result_log", 2 * 8); }

TEST_CASE("apply_bobble_sim: the unoptimized program logs the expected state") {
  const LoggedRun r = run(false);
  // `main` returns log_idx, 2 bytes for each of the 8 scenarios.
  CHECK((r.result.return_int() & 0xFFFF) == 2 * 8);
  CHECK(r.log == kExpected);
}

TEST_CASE("apply_bobble_sim: the optimized program logs the expected state") {
  const LoggedRun r = run(true);
  CHECK((r.result.return_int() & 0xFFFF) == 2 * 8);
  CHECK(r.log == kExpected);
}

TEST_CASE("apply_bobble_sim: the two pipelines agree") {
  const LoggedRun unopt = run(false);
  const LoggedRun opt = run(true);
  CHECK(unopt.log == opt.log);
  CHECK(hargs_head(unopt.result) == hargs_head(opt.result));
}

}  // namespace
}  // namespace c6502::suite
