// The simulator tests of tests/test_step_pos_sim.py: `step_pos`
// decrements a slot's animation counter, adds 3 to its 16-bit world X
// with the carry between the bytes, and calls `apply_bobble`. Each
// scenario logs the slot's bytes and the last call's arguments, and both
// pipelines log the bytes computed by hand.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"
#include "sim_helpers.h"

namespace c6502::suite {
namespace {

// Source: examples/step_pos.c, inlined with an `apply_bobble` stub
// (the original declares it extern) plus a `main` driver and the
// minimum subset of the external state tables that step_pos touches.
const std::string kProgram = R"c(
#include <stdint.h>

uint8_t entity_floor_col[20];
uint8_t entity_xoff_idx[20];
uint8_t rescue_anim[20];

/* apply_bobble side-effect log. */
uint8_t bobble_calls;
uint8_t bobble_last_slot;
uint8_t bobble_last_idx;

[[c6502::zp_abi]]
void apply_bobble(uint8_t slot, uint8_t bobble_idx)
{
    bobble_calls = (uint8_t)(bobble_calls + 1);
    bobble_last_slot = slot;
    bobble_last_idx = bobble_idx;
}

[[c6502::zp_abi]]
static void step_pos(uint8_t slot, uint8_t anim_in)
{
    uint8_t new_anim = (uint8_t)(anim_in - 1);
    rescue_anim[slot] = new_anim;
    uint16_t world_x =
        ((uint16_t)entity_xoff_idx[slot] << 8) | entity_floor_col[slot];
    world_x = (uint16_t)(world_x + 3);
    entity_floor_col[slot] = (uint8_t)world_x;
    entity_xoff_idx[slot]  = (uint8_t)(world_x >> 8);
    apply_bobble(slot, new_anim);
}

/* 8 bytes per scenario:
 *   [slot, rescue_anim[slot], entity_floor_col[slot],
 *    entity_xoff_idx[slot], bobble_last_slot, bobble_last_idx,
 *    bobble_calls, 0 (pad)] */
uint8_t result_log[64];
uint8_t log_idx;

void clear_slots(void) {
    for (uint8_t i = 0; i < 20; i = (uint8_t)(i + 1)) {
        entity_floor_col[i] = 0;
        entity_xoff_idx[i] = 0;
        rescue_anim[i] = 0;
    }
}

void record(uint8_t slot) {
    uint8_t base = log_idx;
    result_log[(uint8_t)(base + 0)] = slot;
    result_log[(uint8_t)(base + 1)] = rescue_anim[slot];
    result_log[(uint8_t)(base + 2)] = entity_floor_col[slot];
    result_log[(uint8_t)(base + 3)] = entity_xoff_idx[slot];
    result_log[(uint8_t)(base + 4)] = bobble_last_slot;
    result_log[(uint8_t)(base + 5)] = bobble_last_idx;
    result_log[(uint8_t)(base + 6)] = bobble_calls;
    result_log[(uint8_t)(base + 7)] = 0;
    log_idx = (uint8_t)(base + 8);
}

int main(void) {
    bobble_calls = 0;
    log_idx = 0;

    /* 1. Slot 0, no carry: world_x = 0x0010 + 3 = 0x0013. */
    clear_slots();
    entity_xoff_idx[0]  = 0x00;
    entity_floor_col[0] = 0x10;
    step_pos(0, 8);
    record(0);

    /* 2. Slot 5, low-byte wrap: world_x = 0x02FE + 3 = 0x0301. */
    clear_slots();
    entity_xoff_idx[5]  = 0x02;
    entity_floor_col[5] = 0xFE;
    step_pos(5, 3);
    record(5);

    /* 3. Slot 19, both bytes wrap: world_x = 0x03FF + 3 = 0x0402.
     *    (High byte goes past the documented 0..3 range; the C code
     *    does not clamp, so we verify the unclamped result.) */
    clear_slots();
    entity_xoff_idx[19]  = 0x03;
    entity_floor_col[19] = 0xFF;
    step_pos(19, 1);
    record(19);

    /* 4. Slot 10, low byte +3 crosses 0x100: 0x00FF + 3 = 0x0102. */
    clear_slots();
    entity_xoff_idx[10]  = 0x00;
    entity_floor_col[10] = 0xFF;
    step_pos(10, 8);
    record(10);

    /* 5. Slot 3, anim_in = 0 -> new_anim wraps to 0xFF. */
    clear_slots();
    entity_xoff_idx[3]  = 0x00;
    entity_floor_col[3] = 0x05;
    step_pos(3, 0);
    record(3);

    return (int)log_idx;
}
)c";
// Expected post-call state, computed by hand for each scenario. Each row
// is the 8-byte layout `record` writes.
const std::vector<Bytes> kRows = {
    // 1. slot 0, anim 8->7, col 0x10+3=0x13, xoff stays 0.
    {0, 7, 0x13, 0x00, 0, 7, 1, 0},
    // 2. slot 5, anim 3->2, col 0xFE+3=0x01 wrap, xoff 0x02->0x03.
    {5, 2, 0x01, 0x03, 5, 2, 2, 0},
    // 3. slot 19, anim 1->0, col 0xFF+3=0x02 wrap, xoff 0x03->0x04.
    {19, 0, 0x02, 0x04, 19, 0, 3, 0},
    // 4. slot 10, anim 8->7, col 0xFF+3=0x02 wrap, xoff 0x00->0x01.
    {10, 7, 0x02, 0x01, 10, 7, 4, 0},
    // 5. slot 3, anim 0->0xFF, col 0x05+3=0x08, xoff stays 0.
    {3, 0xFF, 0x08, 0x00, 3, 0xFF, 5, 0},
};

LoggedRun run(Pipeline pipeline) { return run_logged(kProgram, pipeline, "result_log", 8 * 5); }

TEST_CASE("step_pos_sim: the unoptimized program logs the expected state") {
  const LoggedRun r = run(Pipeline::kUnoptimized);
  // `main` returns log_idx, 8 bytes for each of the 5 scenarios.
  CHECK((r.result.return_int() & 0xFFFF) == 8 * 5);
  CHECK(r.log == flatten(kRows));
}

TEST_CASE("step_pos_sim: the optimized program logs the expected state") {
  const LoggedRun r = run(Pipeline::kOptimized);
  CHECK((r.result.return_int() & 0xFFFF) == 8 * 5);
  CHECK(r.log == flatten(kRows));
}

TEST_CASE("step_pos_sim: the two pipelines agree") { CHECK(run(Pipeline::kUnoptimized).log == run(Pipeline::kOptimized).log); }

}  // namespace
}  // namespace c6502::suite
