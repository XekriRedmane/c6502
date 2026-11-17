/*
    timers.c -- the holes filling back in and the dead guards coming
    back.

    Each turn, HANDLE_TIMERS counts down the timer of every hole being
    filled. The hole shows two stages of brick on the way, and when the
    timer runs out the brick is back. A player in the hole then dies,
    and a guard in it is killed. A killed guard is moved to the first
    empty cell from row 1 on, starting at GUARD_RESURRECT_COL, which
    steps every turn, and GUARD_RESURRECTIONS hatches it there from an
    egg over 20 turns.

    Departures from the original:

    - TMP_LOOP_CTR, the hole's index, is a local of handle_timers.
    - GUARD_RESURRECTIONS keeps GUARD_NUM in a local where the original
      pushes it on the stack.
    - When no empty cell is left in rows 1-15, the search for a place
      to put a killed guard goes on into row 16. The original's row 16
      pointer is $0008, in zero page, and this program's is the bytes
      after LEVEL_BG. Only a level whose rows 1-15 are full has no
      empty cell, so the two can't differ in a game.

    Lode Runner, main.asm lines 4318-4599:

        ORG    $75F4
    HANDLE_TIMERS:
        SUBROUTINE

        JSR     GUARD_RESURRECTIONS

        ; Increment GUARD_RESURRECT_COL mod 29

        INC     GUARD_RESURRECT_COL
        LDA     GUARD_RESURRECT_COL
        CMP     #MAX_GAME_COL+1
        BCC     .guard_col_incremented

        LDA     #$00
        STA     GUARD_RESURRECT_COL

    .guard_col_incremented:
        LDX     #$1E        ; 30

    .loop:
        LDA     BRICK_FILL_TIMERS,X
        STX     TMP_LOOP_CTR
        BNE     .table_ce0_nonzero
        JMP     .next

    .table_ce0_nonzero:
        DEC     BRICK_FILL_TIMERS,X
        BEQ     .brick_fill_timer_expired

        LDA     BRICK_DIG_COLS,X
        STA     GAME_COLNUM
        LDA     BRICK_DIG_ROWS,X
        STA     GAME_ROWNUM

        LDA     BRICK_FILL_TIMERS,X
        CMP     #$14            ; 20
        BNE     .check_for_10

        LDA     #SPRITE_BRICK_FILL0

    .draw_sprite:
        JSR     DRAW_SPRITE_PAGE2
        LDX     GAME_COLNUM
        LDY     GAME_ROWNUM
        JSR     GET_SCREEN_COORDS_FOR
        LDA     #SPRITE_EMPTY
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS

    .next_:
        JMP     .next

    .check_for_10:
        CMP     #$0A            ; 10
        BNE     .next_

        LDA     #SPRITE_BRICK_FILL1
        BNE     .draw_sprite            ; Unconditional

    .brick_fill_timer_expired:
        LDX     TMP_LOOP_CTR
        LDY     BRICK_DIG_ROWS,X
        STY     GAME_ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     BRICK_DIG_COLS,X
        STY     GAME_COLNUM
        LDA     (PTR1),Y
        CMP     #SPRITE_EMPTY
        BNE     .check_for_brick_fill_player_kill
        JMP     .draw_brick

    .check_for_brick_fill_player_kill:
        CMP     #SPRITE_PLAYER
        BNE     .check_for_brick_fill_guard_kill
        LSR     ALIVE

    .check_for_brick_fill_guard_kill:
        CMP     #SPRITE_GUARD
        BEQ     .kill_guard

        CMP     #SPRITE_GOLD
        BNE     .draw_brick_
        DEC     GOLD_COUNT

    .draw_brick_:
        JMP     .draw_brick

    .kill_guard:
        LDA     #SPRITE_BRICK
        STA     (PTR1),Y
        STA     (PTR2),Y
        JSR     DRAW_SPRITE_PAGE1
        LDA     #SPRITE_BRICK
        JSR     DRAW_SPRITE_PAGE2
        LDX     GUARD_COUNT

    .find_killed_guard:
        LDA     GUARD_LOCS_COL,X
        CMP     GAME_COLNUM
        BNE     .next_guard
        LDA     GUARD_LOCS_ROW,X
        CMP     GAME_ROWNUM
        BNE     .next_guard

        LDA     GUARD_GOLD_TIMERS,X
        BPL     .reset_guard_gold_timer
        DEC     GOLD_COUNT

    .reset_guard_gold_timer:
        LDA     #$7F
        STA     GUARD_GOLD_TIMERS,X
        STX     GUARD_NUM
        JSR     LOAD_GUARD_DATA
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        LDX     GUARD_NUM
        LDY     #$01
        STY     GAME_ROWNUM

    .row_loop:
        LDY     GAME_ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GUARD_RESURRECT_COL

    .col_loop:
        LDA     (PTR2),Y
        CMP     #$00
        BEQ     .found_good_resurrect_loc

        INC     GUARD_RESURRECT_COL
        LDY     GUARD_RESURRECT_COL
        CPY     #MAX_GAME_COL+1
        BCC     .col_loop

        INC     GAME_ROWNUM
        LDA     #$00
        STA     GUARD_RESURRECT_COL
        BEQ     .row_loop               ; unconditional

    .found_good_resurrect_loc:
        TYA
        STA     GUARD_LOCS_COL,X
        LDA     GAME_ROWNUM
        STA     GUARD_LOCS_ROW,X
        LDA     #$14            ; 20
        STA     GUARD_RESURRECTION_TIMERS,X
        LDA     #$02
        STA     GUARD_Y_ADJS,X
        STA     GUARD_X_ADJS,X
        LDA     #$00
        STA     GUARD_ANIM_STATES,X
        LDY     #$00
        LDA     #$75
        JSR     ADD_AND_UPDATE_SCORE        ; SCORE += 75
        JMP     .next

    .next_guard:
        DEX
        BNE     .find_killed_guard

        ; This should never fall through

    .draw_brick:
        LDA     #SPRITE_BRICK
        STA     (PTR1),Y
        JSR     DRAW_SPRITE_PAGE1
        LDA     #SPRITE_BRICK
        JSR     DRAW_SPRITE_PAGE2

    .next:
        LDX     TMP_LOOP_CTR
        DEX
        BMI     HANDLE_TIMERS_COMMON_RETURN
        JMP     .loop

    HANDLE_TIMERS_COMMON_RETURN:
        RTS

        ORG    $7716
    GUARD_RESURRECTIONS:
        SUBROUTINE

        LDX     GUARD_COUNT
        BEQ     HANDLE_TIMERS_COMMON_RETURN

        LDA     GUARD_NUM
        PHA

    .loop:
        LDA     GUARD_RESURRECTION_TIMERS,X
        BEQ     .next

        STX     GUARD_NUM
        JSR     LOAD_GUARD_DATA
        LDA     #$7F
        STA     GUARD_GOLD_TIMERS,X
        LDA     GUARD_LOCS_COL,X
        STA     GAME_COLNUM
        LDA     GUARD_LOCS_ROW,X
        STA     GAME_ROWNUM

        DEC     GUARD_RESURRECTION_TIMERS,X
        BEQ     .resurrect

        LDA     GUARD_RESURRECTION_TIMERS,X
        CMP     #$13                ; 19
        BNE     .check_guard_flag_5_is_10

        ; GUARD_RESURRECTION_TIMER is 19

        LDA     #SPRITE_GUARD_EGG0
        JSR     DRAW_SPRITE_PAGE2
        JSR     GET_GUARD_SPRITE_AND_COORDS
        LDA     #SPRITE_GUARD_EGG0
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS
        JMP     .next2

    .check_guard_flag_5_is_10:
        CMP     #$0A                ; 10
        BNE     .next

        ; GUARD_RESURRECTION_TIMER is 10
        LDA     #SPRITE_GUARD_EGG1
        JSR     DRAW_SPRITE_PAGE2
        JSR     GET_GUARD_SPRITE_AND_COORDS
        LDA     #SPRITE_GUARD_EGG1
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS

    .next2:
        ; Restores the counter
        LDX     GUARD_NUM

    .next:
        DEX
        BNE     .loop

        PLA
        STA     GUARD_NUM
        RTS

    .resurrect:
        LDY     GAME_ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDX     GUARD_NUM
        INC     GUARD_RESURRECTION_TIMERS,X
        LDY     GAME_COLNUM
        LDA     (PTR1),Y
        BNE     .next

        ; empty
        LDA     #SPRITE_GUARD
        STA     (PTR1),Y
        LDA     #SPRITE_EMPTY
        JSR     DRAW_SPRITE_PAGE2
        LDA     #$00
        LDX     GUARD_NUM
        STA     GUARD_GOLD_TIMERS,X
        STA     GUARD_RESURRECTION_TIMERS,X
        LDA     #SPRITE_GUARD
        JSR     DRAW_SPRITE_PAGE1

        ; Play the "guard resurrection" sound
        JSR     LOAD_SOUND_DATA
        HEX     02 7C 03 78 04 74 05 70 00

        LDX     GUARD_NUM
        JMP     .next

*/
#include "lode_runner.h"

/* Played when a guard hatches. Each pair is a duration and a pitch. */
static const uint8_t guard_hatched_tune[] = {
    0x02, 0x7C, 0x03, 0x78, 0x04, 0x74, 0x05, 0x70, 0x00,
};

/* Counts down each egg's GUARD_RESURRECTION_TIMERS, draws the egg's
 * two stages at 19 and 10, and at 0 hatches the guard if its cell is
 * empty. Otherwise the timer goes back to 1, and the guard tries again
 * next turn. The guard's gold timer is 0x7F while it's an egg, which
 * MOVE_GUARD treats as stuck in a hole. */
static void guard_resurrections(void) {
    uint8_t g = GUARD_COUNT;
    if (g == 0)
        return;
    uint8_t saved_guard_num = GUARD_NUM;
    do {
        if (GUARD_RESURRECTION_TIMERS[g] == 0)
            continue;
        GUARD_NUM = g;
        load_guard_data();
        GUARD_GOLD_TIMERS[g] = 0x7F;
        GAME_COLNUM = GUARD_LOCS_COL[g];
        GAME_ROWNUM = GUARD_LOCS_ROW[g];
        uint8_t timer = --GUARD_RESURRECTION_TIMERS[g];
        if (timer == 0) {
            GUARD_RESURRECTION_TIMERS[g] = 1;
            if (LEVEL_MAP[GAME_ROWNUM][GAME_COLNUM] == SPRITE_EMPTY) {
                LEVEL_MAP[GAME_ROWNUM][GAME_COLNUM] = SPRITE_GUARD;
                draw_sprite_page2(SPRITE_EMPTY);
                GUARD_GOLD_TIMERS[g] = 0;
                GUARD_RESURRECTION_TIMERS[g] = 0;
                draw_sprite_page1(SPRITE_GUARD);
                load_sound_data(guard_hatched_tune);
            }
        } else if (timer == 19) {
            draw_sprite_page2(SPRITE_GUARD_EGG0);
            draw_guard_sprite(SPRITE_GUARD_EGG0);
        } else if (timer == 10) {
            draw_sprite_page2(SPRITE_GUARD_EGG1);
            draw_guard_sprite(SPRITE_GUARD_EGG1);
        }
    } while (--g != 0);
    GUARD_NUM = saved_guard_num;
}

/* Kills the guard in the hole at GAME_COLNUM, GAME_ROWNUM, taking any
 * gold it carries out of the level, and turns it into an egg at the
 * first empty cell from row 1 on, starting at GUARD_RESURRECT_COL. The
 * player scores 75 points. Returns false when no guard is at the hole.
 * The guard search runs from GUARD_COUNT down to 1, and when
 * GUARD_COUNT is 0 it wraps around through 255 as the original's
 * does. */
static bool kill_guard_in_hole(void) {
    uint8_t g = GUARD_COUNT;
    do {
        if (GUARD_LOCS_COL[g] == GAME_COLNUM && GUARD_LOCS_ROW[g] == GAME_ROWNUM) {
            if ((int8_t)GUARD_GOLD_TIMERS[g] < 0)
                GOLD_COUNT--;
            GUARD_GOLD_TIMERS[g] = 0x7F;
            GUARD_NUM = g;
            load_guard_data();
            erase_guard();
            GAME_ROWNUM = 1;
            while (LEVEL_BG[GAME_ROWNUM][GUARD_RESURRECT_COL] != SPRITE_EMPTY) {
                if (++GUARD_RESURRECT_COL > MAX_GAME_COL) {
                    GAME_ROWNUM++;
                    GUARD_RESURRECT_COL = 0;
                }
            }
            GUARD_LOCS_COL[g] = GUARD_RESURRECT_COL;
            GUARD_LOCS_ROW[g] = GAME_ROWNUM;
            GUARD_RESURRECTION_TIMERS[g] = 20;
            GUARD_Y_ADJS[g] = 2;
            GUARD_X_ADJS[g] = 2;
            GUARD_ANIM_STATES[g] = 0;
            add_and_update_score(0x0075);
            return true;
        }
    } while (--g != 0);
    return false;
}

/* Puts the brick back in the cell at row, col on both pages. */
static void draw_brick(uint8_t row, uint8_t col) {
    LEVEL_MAP[row][col] = SPRITE_BRICK;
    draw_sprite_page1(SPRITE_BRICK);
    draw_sprite_page2(SPRITE_BRICK);
}

/* Fills in hole i, whose timer has run out, killing whatever is in it.
 * A filled hole holding gold takes it out of the level. */
static void fill_hole(uint8_t i) {
    uint8_t row = BRICK_DIG_ROWS[i];
    uint8_t col = BRICK_DIG_COLS[i];
    GAME_ROWNUM = row;
    GAME_COLNUM = col;
    uint8_t sprite = LEVEL_MAP[row][col];
    if (sprite == SPRITE_PLAYER) {
        ALIVE >>= 1;
    } else if (sprite == SPRITE_GUARD) {
        /* If no guard is found at the hole, the brick is drawn a second
         * time below, as in the original. */
        LEVEL_BG[row][col] = SPRITE_BRICK;
        draw_brick(row, col);
        if (kill_guard_in_hole())
            return;
    } else if (sprite == SPRITE_GOLD) {
        GOLD_COUNT--;
    }
    draw_brick(row, col);
}

void handle_timers(void) {
    guard_resurrections();
    if (++GUARD_RESURRECT_COL > MAX_GAME_COL)
        GUARD_RESURRECT_COL = 0;
    for (uint8_t i = 30; (int8_t)i >= 0; i--) {
        if (BRICK_FILL_TIMERS[i] == 0)
            continue;
        uint8_t timer = --BRICK_FILL_TIMERS[i];
        if (timer == 0) {
            fill_hole(i);
            continue;
        }
        GAME_COLNUM = BRICK_DIG_COLS[i];
        GAME_ROWNUM = BRICK_DIG_ROWS[i];
        if (timer == 20 || timer == 10) {
            /* The brick shows on page 2, the background, and erasing an
             * empty sprite from page 1 lets it through. */
            draw_sprite_page2(timer == 20 ? SPRITE_BRICK_FILL0 : SPRITE_BRICK_FILL1);
            erase_sprite_at_pixel_coords(
                SPRITE_EMPTY, HALF_SCREEN_COL_TABLE[GAME_COLNUM],
                SCREEN_ROW_TABLE[GAME_ROWNUM]);
        }
    }
}
