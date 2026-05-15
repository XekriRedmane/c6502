#include <stdint.h>

/* 4-slot floor-enemy state (shared with FLOOR_ENEMY_DRAW). */
uint8_t enemy_flag[4];   /* $00=off, $01=rightward, $FF=leftward      */
uint8_t enemy_col[4];    /* column value (perspective-table index)    */
uint8_t enemy_y[4];      /* screen row                                */

/* Jump-key one-shot: $FF when the player presses jump.
* Consumed/cleared by this routine. */
uint8_t jump_flag;

/* Opcode byte of MAIN_LOOP's SMC_MOVE_LEFT slot ($67DD in asm).
* $20 (JSR) means left-movement is currently active. */
extern uint8_t* const smc_move_left_op;

/* Descending speaker-click helper. */
__attribute__((zp_abi))
extern void snd_delay_down(uint8_t pitch, uint8_t clicks);

/* Spawn-row seed table indexed by player_col (54 entries).
* Indices 0..32 carry $25..$2E (rising plateaus); 33..53 are $00.
* Caller treats negative entries as "spawn suppressed" -- no entry
* in this level is negative, but the test is preserved. */
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
    /* LDX #$03 ... DEX / BPL .slot_body */
    for (uint8_t slot = 3; (slot & 0x80) == 0; slot--) {
        uint8_t flag = enemy_flag[slot];

        if (flag == 0) {
            /* try to claim this empty slot */
            if (jump_flag & 0x80) {                
                jump_flag = 0;                     /* consume immediately   */
                uint8_t sched = floor_enemy_spawn_sched[player_col];
                if ((sched & 0x80) == 0) {         /* spawn allowed  */
                    snd_delay_down(0x20, 10);      /* descending spawn click */
                    if (*smc_move_left_op == OPCODE_JSR) {
                        /* spawn leftward, entering from right edge */
                        enemy_flag[slot] = 0xFF;
                        enemy_col[slot]  = 0x3E;
                    } else {
                        /* spawn rightward, entering from left edge */
                        enemy_flag[slot] = 0x01;
                        enemy_col[slot]  = 0x4A;
                    }
                    enemy_y[slot] = (uint8_t)(player_col + 9);
                }
            }
        }
        else if (flag & 0x80) {
            /* leftward slot, col -= 5/7/9. */
            uint8_t step;
            if (move_dir == 0)        step = 7;    /* idle: medium       */
            else if (move_dir & 0x80) step = 5;    /* player left: slow  */
            else                      step = 9;    /* player right: fast */
            uint8_t new_col = (uint8_t)(enemy_col[slot] - step);
            enemy_col[slot] = new_col;
            if ((uint8_t)(new_col - 2) >= 0x8D) {
                enemy_flag[slot] = 0;              /* despawn */
            }
        }
        else {
            /* rightward slot, col += 5/7/9. */
            uint8_t step;
            if (move_dir == 0)        step = 7;    /* idle: medium       */
            else if (move_dir & 0x80) step = 9;    /* player left: fast  */
            else                      step = 5;    /* player right: slow */
            uint8_t new_col = (uint8_t)(enemy_col[slot] + step);
            enemy_col[slot] = new_col;
            if (new_col >= 0x8F) {                 
                enemy_flag[slot] = 0;              /* despawn */
            }
        }
    }
    /* Loop-end clear: ensure one-shot is cleared even if no slot was
    * empty (no spawn path consumed it). */
    jump_flag = 0;
}