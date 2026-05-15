"""End-to-end simulator test for `floor_enemy_advance` under both
the unoptimized and optimized pipelines.

The function maintains 4 floor-enemy slots, processing a one-shot
jump-spawn input plus per-slot leftward / rightward column motion.
We run a battery of scenarios through `main`, which writes the
post-call slot state into a flat checksum array; the test then
compares the byte-for-byte snapshot of the slot arrays AND the
checksum between the two pipelines.

Scenarios exercised:
  1. Empty slots + no jump input -> jump_flag must be cleared,
     slots untouched.
  2. Empty slots + jump_flag set + smc_move_left_op = $20 (JSR)
     -> slot 3 spawns leftward (flag=$FF, col=$3E),
        enemy_y[3] = player_col + 9.
  3. Empty slots + jump_flag set + smc_move_left_op != $20
     -> slot 3 spawns rightward (flag=$01, col=$4A).
  4. Leftward enemies stepping under each move_dir bucket
     (idle / player-left / player-right) including the
     col-2 >= 0x8D despawn edge.
  5. Rightward enemies stepping under each move_dir bucket
     including the col >= 0x8F despawn edge.
"""

import shutil
import unittest

import sim.runtime as rt_mod
from sim.harness import build_sim


# Source: examples/floor_enemy_advance.c, inlined here together with
# a stub `snd_delay_down`, a backing byte for `smc_move_left_op`'s
# pointee, and a `main` that exercises a sequence of scenarios and
# accumulates the resulting slot state into `result_log`.
_PROGRAM = r"""
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

__attribute__((zp_abi))
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

__attribute__((zp_abi))
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
"""


# Expected scenario state, derived by hand from the C source. Each
# entry is 16 bytes: 4 flags + 4 cols + 4 ys + (jump_flag, snd_calls,
# last_pitch, last_clicks). `snd_calls` is cumulative.
def _scenario_state() -> dict[str, list[list[int]]]:
    # 1. No jump, no enemies. snd never called.
    s1 = [
        [0, 0, 0, 0],          # flags
        [0, 0, 0, 0],          # cols
        [0, 0, 0, 0],          # ys
        [0, 0, 0, 0],          # (jump=0 after clear, snd_calls=0, _, _)
    ]
    # 2. Jump pending, smc=$20 -> slot 3 spawns left.
    #    player_col=12, sched[12]=$29 (bit7=0), so spawn fires.
    s2 = [
        [0, 0, 0, 0xFF],
        [0, 0, 0, 0x3E],
        [0, 0, 0, 12 + 9],
        [0, 1, 0x20, 10],
    ]
    # 3. Jump pending, smc=$EA -> slot 3 spawns right.
    #    player_col=5, sched[5]=$27.
    s3 = [
        [0, 0, 0, 0x01],
        [0, 0, 0, 0x4A],
        [0, 0, 0, 5 + 9],
        [0, 2, 0x20, 10],
    ]
    # 4a. Leftward, move_dir=0, step=7.
    #     col[0]=$80-7=$79 -> ($79-2)=$77 < $8D, keep.
    #     col[1]=$40-7=$39 -> ($39-2)=$37 < $8D, keep.
    #     col[2]=$10-7=$09 -> ($09-2)=$07 < $8D, keep.
    #       (note: this comment was wrong in the test design; $07 < $8D so KEEP)
    #     col[3]=$50-7=$49 -> ($49-2)=$47 < $8D, keep.
    s4a = [
        [0xFF, 0xFF, 0xFF, 0xFF],
        [0x79, 0x39, 0x09, 0x49],
        [0, 0, 0, 0],
        [0, 2, 0x20, 10],
    ]
    # 4b. Leftward, move_dir=1 (player-right), step=9.
    #     col[0]=$60-9=$57 -> ($57-2)=$55 keep.
    #     col[3]=$20-9=$17 -> ($17-2)=$15 keep.
    s4b = [
        [0xFF, 0, 0, 0xFF],
        [0x57, 0, 0, 0x17],
        [0, 0, 0, 0],
        [0, 2, 0x20, 10],
    ]
    # 4c. Leftward, move_dir=$FF (player-left), step=5.
    #     col[0]=$60-5=$5B keep.
    #     col[3]=$06-5=$01 -> ($01-2) wraps to $FF, >=$8D, DESPAWN.
    s4c = [
        [0xFF, 0, 0, 0],
        [0x5B, 0, 0, 0x01],
        [0, 0, 0, 0],
        [0, 2, 0x20, 10],
    ]
    # 5a. Rightward, move_dir=0, step=7.
    #     col[0]=$10+7=$17 keep.
    #     col[1]=$80+7=$87 keep.
    #     col[2]=$90+7=$97 >=$8F DESPAWN (flag cleared, col stays $97).
    #     col[3]=$88+7=$8F >=$8F DESPAWN.
    s5a = [
        [0x01, 0x01, 0, 0],
        [0x17, 0x87, 0x97, 0x8F],
        [0, 0, 0, 0],
        [0, 2, 0x20, 10],
    ]
    # 5b. Rightward, move_dir=$FF (player-left), step=9.
    #     col[0]=$50+9=$59 keep.
    #     col[1]=$86+9=$8F DESPAWN.
    s5b = [
        [0x01, 0, 0, 0],
        [0x59, 0x8F, 0, 0],
        [0, 0, 0, 0],
        [0, 2, 0x20, 10],
    ]
    return {
        "1_no_jump": s1,
        "2_jump_left": s2,
        "3_jump_right": s3,
        "4a_left_idle": s4a,
        "4b_left_right": s4b,
        "4c_left_left_despawn": s4c,
        "5a_right_idle": s5a,
        "5b_right_left": s5b,
    }


def _flatten(scenarios: dict[str, list[list[int]]]) -> bytes:
    out = bytearray()
    for _, rows in scenarios.items():
        for row in rows:
            out.extend(row)
    return bytes(out)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestFloorEnemyAdvanceSim(unittest.TestCase):
    """Differential opt vs unopt check on `floor_enemy_advance`.

    Both pipelines must produce the same `result_log` bytes and the
    same return value (= log_idx after the last scenario)."""

    def _run(self, optimize: bool):
        sim = build_sim(_PROGRAM, optimize=optimize)
        result = sim.run(max_cycles=2_000_000)
        self.assertFalse(
            result.timed_out,
            f"floor_enemy_advance sim timed out "
            f"(optimize={optimize})",
        )
        log_addr = sim.symbols["result_log"]
        log_bytes = bytes(result.memory[log_addr:log_addr + 16 * 8])
        return result, log_bytes

    def test_unoptimized_matches_expected(self):
        result, log = self._run(optimize=False)
        self.assertEqual(
            result.return_int() & 0xFFFF, 8 * 16,
            "log_idx should reflect 8 recorded scenarios * 16 bytes",
        )
        self.assertEqual(log, _flatten(_scenario_state()))

    def test_optimized_matches_expected(self):
        result, log = self._run(optimize=True)
        self.assertEqual(result.return_int() & 0xFFFF, 8 * 16)
        self.assertEqual(log, _flatten(_scenario_state()))

    def test_opt_and_unopt_agree(self):
        unopt_result, unopt_log = self._run(optimize=False)
        opt_result, opt_log = self._run(optimize=True)
        self.assertEqual(
            unopt_log, opt_log,
            "optimizer changed observable slot state",
        )
        # Return-value windows must agree byte for byte (Int return).
        unopt_hargs = bytes(
            unopt_result.memory[rt_mod.HARGS:rt_mod.HARGS + 2]
        )
        opt_hargs = bytes(
            opt_result.memory[rt_mod.HARGS:rt_mod.HARGS + 2]
        )
        self.assertEqual(unopt_hargs, opt_hargs)


if __name__ == "__main__":
    unittest.main()
