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

// Source: examples/floor_enemy_advance.c, inlined here together with
// a stub `snd_delay_down`, a backing byte for `smc_move_left_op`'s
// pointee, and a `main` that exercises a sequence of scenarios and
// accumulates the resulting slot state into `result_log`.
const std::string kProgram = R"c(
#include <stdint.h>

uint8_t enemy_flag[4];
uint8_t enemy_col[4];
uint8_t enemy_y[4];
uint8_t jump_flag;

/* Backing byte for what smc_move_left_op points to. Test toggles
 * this between $20 (JSR; means leftward-spawn) and $EA (NOP; means
 * rightward-spawn). */
uint8_t smc_target;
uint8_t * const smc_move_left_op = &smc_target;

/* Spawn-side-effect counters so the test can assert the call
 * happened. */
uint8_t snd_calls;
uint8_t snd_last_pitch;
uint8_t snd_last_clicks;

[[c6502::zp_abi]]
void snd_delay_down(uint8_t pitch, uint8_t clicks) {
    snd_calls = (uint8_t)(snd_calls + 1);
    snd_last_pitch = pitch;
    snd_last_clicks = clicks;
}

static const uint8_t floor_enemy_spawn_sched[54] = {
    0x25, 0x26, 0x26, 0x26, 0x26, 0x27, 0x27, 0x27,
    0x28, 0x28, 0x28, 0x28, 0x29, 0x29, 0x29, 0x2A,
    0x2A, 0x2A, 0x2A, 0x2B, 0x2B, 0x2B, 0x2C, 0x2C,
    0x2C, 0x2C, 0x2D, 0x2D, 0x2D, 0x2E, 0x2E, 0x2E,
    0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define OPCODE_JSR 0x20

[[c6502::zp_abi]]
void floor_enemy_advance(uint8_t move_dir, uint8_t player_col)
{
    for (uint8_t slot = 3; (slot & 0x80) == 0; slot--) {
        uint8_t flag = enemy_flag[slot];

        if (flag == 0) {
            if (jump_flag & 0x80) {
                jump_flag = 0;
                uint8_t sched = floor_enemy_spawn_sched[player_col];
                if ((sched & 0x80) == 0) {
                    snd_delay_down(0x20, 10);
                    if (*smc_move_left_op == OPCODE_JSR) {
                        enemy_flag[slot] = 0xFF;
                        enemy_col[slot]  = 0x3E;
                    } else {
                        enemy_flag[slot] = 0x01;
                        enemy_col[slot]  = 0x4A;
                    }
                    enemy_y[slot] = (uint8_t)(player_col + 9);
                }
            }
        }
        else if (flag & 0x80) {
            uint8_t step;
            if (move_dir == 0)        step = 7;
            else if (move_dir & 0x80) step = 5;
            else                      step = 9;
            uint8_t new_col = (uint8_t)(enemy_col[slot] - step);
            enemy_col[slot] = new_col;
            if ((uint8_t)(new_col - 2) >= 0x8D) {
                enemy_flag[slot] = 0;
            }
        }
        else {
            uint8_t step;
            if (move_dir == 0)        step = 7;
            else if (move_dir & 0x80) step = 9;
            else                      step = 5;
            uint8_t new_col = (uint8_t)(enemy_col[slot] + step);
            enemy_col[slot] = new_col;
            if (new_col >= 0x8F) {
                enemy_flag[slot] = 0;
            }
        }
    }
    jump_flag = 0;
}

/* Recorded post-call snapshots. Each scenario writes 16 bytes:
 *   [flag[0..3], col[0..3], y[0..3], jump_flag, snd_calls,
 *    snd_last_pitch, snd_last_clicks].
 */
uint8_t result_log[128];
uint8_t log_idx;

void clear_slots(void) {
    for (uint8_t i = 0; i < 4; i = (uint8_t)(i + 1)) {
        enemy_flag[i] = 0;
        enemy_col[i] = 0;
        enemy_y[i] = 0;
    }
}

void record(void) {
    uint8_t base = log_idx;
    result_log[(uint8_t)(base + 0)]  = enemy_flag[0];
    result_log[(uint8_t)(base + 1)]  = enemy_flag[1];
    result_log[(uint8_t)(base + 2)]  = enemy_flag[2];
    result_log[(uint8_t)(base + 3)]  = enemy_flag[3];
    result_log[(uint8_t)(base + 4)]  = enemy_col[0];
    result_log[(uint8_t)(base + 5)]  = enemy_col[1];
    result_log[(uint8_t)(base + 6)]  = enemy_col[2];
    result_log[(uint8_t)(base + 7)]  = enemy_col[3];
    result_log[(uint8_t)(base + 8)]  = enemy_y[0];
    result_log[(uint8_t)(base + 9)]  = enemy_y[1];
    result_log[(uint8_t)(base + 10)] = enemy_y[2];
    result_log[(uint8_t)(base + 11)] = enemy_y[3];
    result_log[(uint8_t)(base + 12)] = jump_flag;
    result_log[(uint8_t)(base + 13)] = snd_calls;
    result_log[(uint8_t)(base + 14)] = snd_last_pitch;
    result_log[(uint8_t)(base + 15)] = snd_last_clicks;
    log_idx = (uint8_t)(base + 16);
}

int main(void) {
    snd_calls = 0;
    log_idx = 0;

    /* 1. All slots empty, no jump input -> jump_flag must clear. */
    clear_slots();
    jump_flag = 0;
    smc_target = 0x20;
    floor_enemy_advance(0, 12);
    record();

    /* 2. Empty + jump pending + smc = JSR -> slot 3 spawns left. */
    clear_slots();
    jump_flag = 0xFF;
    smc_target = 0x20;
    floor_enemy_advance(0, 12);
    record();

    /* 3. Empty + jump pending + smc != JSR -> slot 3 spawns right. */
    clear_slots();
    jump_flag = 0xFF;
    smc_target = 0xEA;
    floor_enemy_advance(0, 5);
    record();

    /* 4a. Leftward enemies, idle move_dir -> step=7 each. */
    clear_slots();
    enemy_flag[0] = 0xFF; enemy_col[0] = 0x80;
    enemy_flag[1] = 0xFF; enemy_col[1] = 0x40;
    enemy_flag[2] = 0xFF; enemy_col[2] = 0x10;  /* will hit despawn */
    enemy_flag[3] = 0xFF; enemy_col[3] = 0x50;
    jump_flag = 0;
    floor_enemy_advance(0, 0);
    record();

    /* 4b. Leftward enemies, player-right move_dir (e.g. 1)
     *     -> step=9 each (fast). */
    clear_slots();
    enemy_flag[0] = 0xFF; enemy_col[0] = 0x60;
    enemy_flag[3] = 0xFF; enemy_col[3] = 0x20;  /* will despawn */
    jump_flag = 0;
    floor_enemy_advance(1, 0);
    record();

    /* 4c. Leftward enemies, player-left move_dir ($FF)
     *     -> step=5 each (slow). */
    clear_slots();
    enemy_flag[0] = 0xFF; enemy_col[0] = 0x60;
    enemy_flag[3] = 0xFF; enemy_col[3] = 0x06;  /* despawn */
    jump_flag = 0;
    floor_enemy_advance(0xFF, 0);
    record();

    /* 5a. Rightward enemies, idle move_dir -> step=7 each. */
    clear_slots();
    enemy_flag[0] = 0x01; enemy_col[0] = 0x10;
    enemy_flag[1] = 0x01; enemy_col[1] = 0x80;
    enemy_flag[2] = 0x01; enemy_col[2] = 0x90;  /* already >= $8F */
    enemy_flag[3] = 0x01; enemy_col[3] = 0x88;  /* lands at $8F: despawn */
    jump_flag = 0;
    floor_enemy_advance(0, 0);
    record();

    /* 5b. Rightward enemies, player-left ($FF) -> step=9 (fast). */
    clear_slots();
    enemy_flag[0] = 0x01; enemy_col[0] = 0x50;
    enemy_flag[1] = 0x01; enemy_col[1] = 0x86;  /* lands at $8F: despawn */
    jump_flag = 0;
    floor_enemy_advance(0xFF, 0);
    record();

    return (int)log_idx;
}
)c";
// Expected scenario state, derived by hand from the C source. Each
// entry is 16 bytes: 4 flags + 4 cols + 4 ys + (jump_flag, snd_calls,
// last_pitch, last_clicks). `snd_calls` is cumulative.
Bytes scenario_state() {
  // 1. No jump, no enemies. snd never called.
  const std::vector<Bytes> s1 = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  // 2. Jump pending, smc=$20 -> slot 3 spawns left.
  //    player_col=12, sched[12]=$29 (bit7=0), so spawn fires.
  const std::vector<Bytes> s2 = {{0, 0, 0, 0xFF}, {0, 0, 0, 0x3E}, {0, 0, 0, 12 + 9}, {0, 1, 0x20, 10}};
  // 3. Jump pending, smc=$EA -> slot 3 spawns right.
  //    player_col=5, sched[5]=$27.
  const std::vector<Bytes> s3 = {{0, 0, 0, 0x01}, {0, 0, 0, 0x4A}, {0, 0, 0, 5 + 9}, {0, 2, 0x20, 10}};
  // 4a. Leftward, move_dir=0, step=7.
  //     col[0]=$80-7=$79 -> ($79-2)=$77 < $8D, keep.
  //     col[1]=$40-7=$39 -> ($39-2)=$37 < $8D, keep.
  //     col[2]=$10-7=$09 -> ($09-2)=$07 < $8D, keep.
  //       (note: this comment was wrong in the test design; $07 < $8D so KEEP)
  const std::vector<Bytes> s4a = {{0xFF, 0xFF, 0xFF, 0xFF}, {0x79, 0x39, 0x09, 0x49}, {0, 0, 0, 0}, {0, 2, 0x20, 10}};
  // 4b. Leftward, move_dir=1 (player-right), step=9.
  //     col[0]=$60-9=$57 -> ($57-2)=$55 keep.
  //     col[3]=$20-9=$17 -> ($17-2)=$15 keep.
  const std::vector<Bytes> s4b = {{0xFF, 0, 0, 0xFF}, {0x57, 0, 0, 0x17}, {0, 0, 0, 0}, {0, 2, 0x20, 10}};
  // 4c. Leftward, move_dir=$FF (player-left), step=5.
  //     col[0]=$60-5=$5B keep.
  //     col[3]=$06-5=$01 -> ($01-2) wraps to $FF, >=$8D, DESPAWN.
  const std::vector<Bytes> s4c = {{0xFF, 0, 0, 0}, {0x5B, 0, 0, 0x01}, {0, 0, 0, 0}, {0, 2, 0x20, 10}};
  // 5a. Rightward, move_dir=0, step=7.
  //     col[0]=$10+7=$17 keep.
  //     col[1]=$80+7=$87 keep.
  //     col[2]=$90+7=$97 >=$8F DESPAWN (flag cleared, col stays $97).
  //     col[3]=$88+7=$8F >=$8F DESPAWN.
  const std::vector<Bytes> s5a = {{0x01, 0x01, 0, 0}, {0x17, 0x87, 0x97, 0x8F}, {0, 0, 0, 0}, {0, 2, 0x20, 10}};
  // 5b. Rightward, move_dir=$FF (player-left), step=9.
  //     col[0]=$50+9=$59 keep.
  //     col[1]=$86+9=$8F DESPAWN.
  const std::vector<Bytes> s5b = {{0x01, 0, 0, 0}, {0x59, 0x8F, 0, 0}, {0, 0, 0, 0}, {0, 2, 0x20, 10}};
  std::vector<Bytes> out;
  for (const auto* rows : {&s1, &s2, &s3, &s4a, &s4b, &s4c, &s5a, &s5b})
    out.insert(out.end(), rows->begin(), rows->end());
  return flatten(out);
}

LoggedRun run(Pipeline pipeline) { return run_logged(kProgram, pipeline, "result_log", 16 * 8); }

TEST_CASE("floor_enemy_advance_sim: the unoptimized program logs the expected state") {
  const LoggedRun r = run(Pipeline::kUnoptimized);
  // `main` returns log_idx, 16 bytes for each of the 8 scenarios.
  CHECK((r.result.return_int() & 0xFFFF) == 8 * 16);
  CHECK(r.log == scenario_state());
}

TEST_CASE("floor_enemy_advance_sim: the optimized program logs the expected state") {
  const LoggedRun r = run(Pipeline::kOptimized);
  CHECK((r.result.return_int() & 0xFFFF) == 8 * 16);
  CHECK(r.log == scenario_state());
}

TEST_CASE("floor_enemy_advance_sim: the two pipelines agree") {
  const LoggedRun unopt = run(Pipeline::kUnoptimized);
  const LoggedRun opt = run(Pipeline::kOptimized);
  CHECK(unopt.log == opt.log);
  CHECK(hargs_head(unopt.result) == hargs_head(opt.result));
}

}  // namespace
}  // namespace c6502::suite
