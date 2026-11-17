/*
    level.c -- reading, unpacking, drawing, and saving a level, the
    high-score sector, and the hidden ladders.

    A level is stored in the first 224 bytes of a sector, a nibble to a
    cell, the even column of each pair in the low nibble. Level n of a
    data disk is sector n - 1 counted from the start of track 3, 16 to
    a track. The demo's levels are in memory, in DEMO_LEVELS.

    Drawing a level does more than draw it, unless VERBATIM is 0, which
    the level editor sets. It takes the hidden ladders out of the map
    and lists them, counts the gold, lists the guards (up to 5) and the
    player (the first one found), and leaves each of them out of
    LEVEL_BG, the map of what's behind them. When a level has no
    player, the disk's first level is loaded in its place with the
    guards a step faster, and if the first level has none, the game
    starts over.

    Departures from the original:

    - The original calls RWTS through the JMP at $23, whose target is
      RWTS_ADDR, and through INDIRECT_RWTS's JMP (INDIRECT_TARGET).
      Every target ends at DOS's routine at $B7B5, so this unit calls
      RWTS directly and keeps neither RWTS_ADDR nor INDIRECT_TARGET.
      INDIRECT_RWTS isn't translated.
    - The cell being unpacked or packed is in a row and a column, which
      are locals, where the original keeps them in GAME_ROWNUM and
      GAME_COLNUM. Its byte is in a local where the original keeps it
      in LEVEL_DATA_INDEX. Which nibble it's in is the column's low
      bit, where the original counts the cells in TMP. A row has an
      even number of cells, so the two agree. Drawing a level counts in
      GAME_ROWNUM and GAME_COLNUM as the original does, since
      draw_sprite_page2 reads them.
    - The original reaches a cell through PTR1 and PTR2, which it sets
      for each row from CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS and the two
      page tables. Here a cell is LEVEL_MAP[row][col] or
      LEVEL_BG[row][col], and PTR1 and PTR2 aren't written. The
      demo's level is DEMO_LEVELS[LEVELNUM], where the original adds
      LEVELNUM to $9E for the page. The original's ROW_ADDR and ROW_ADDR2, when it
      copies page 2 to page 1, are local pointers, and its MASK0, when
      it compares the high-score sector's marker, is a local.
    - LOAD_LEVEL stores VERBATIM from X, which is $FF when it loads the
      disk's first level in place of one with no player. Here VERBATIM is the
      `bool` argument, 1. Only whether VERBATIM is zero is ever
      tested.
    - COMPRESS_AND_SAVE_LEVEL_DATA packs a cell with bit 7 set in an
      even column as if it were in an odd one, since its BPL falls
      through. The map holds only sprites 0-9, so this isn't
      translated.
    - ENABLE_NEXT_LEVEL_LADDERS counts the ladders down in a local,
      where the original keeps the count in the byte .count after its
      code. The count is set before each use.

    Lode Runner, main.asm lines 735-1210, 6767-6840:

        ORG    $6238
    LOAD_LEVEL:
        SUBROUTINE
        ; Enter routine with X set to whether the level should be
        ; loaded verbatim or not.

        STX     VERBATIM

        LDX     #$FF
        STX     PLAYER_COL
        INX
        STX     LADDER_COUNT
        STX     GOLD_COUNT
        STX     GUARD_COUNT
        STX     GUARD_NUM
        STX     DIG_ANIM_STATE
        STX     LEVEL_DATA_INDEX
        STX     TMP
        STX     GAME_ROWNUM
        TXA

        LDX     #30
    .loop1
        STA     BRICK_FILL_TIMERS,X
        DEX
        BPL     .loop1

        LDX     #$05
    .loop2
        STA     GUARD_RESURRECTION_TIMERS,X
        DEX
        BPL     .loop2


        LDA     #$01
        STA     ALIVE       ; Set player live
        ; A happens to also be DISK_ACCESS_READ.
        JSR     ACCESS_COMPRESSED_LEVEL_DATA

        LDY     GAME_ROWNUM
    .row_loop:
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDA     #$00
        STA     GAME_COLNUM

    .col_loop:
        LDA     TMP                         ; odd/even counter
        LSR
        LDY     LEVEL_DATA_INDEX
        LDA     DISK_BUFFER,Y
        BCS     .628c                       ; odd?
        AND     #$0F
        BPL     .6292                       ; unconditional jump
    .628c

        LSR
        LSR
        LSR
        LSR
        INC     LEVEL_DATA_INDEX

    .6292
        INC     TMP

        LDY     GAME_COLNUM
        CMP     #10
        BCC     .629c
        LDA     #SPRITE_EMPTY                ; sprite >= 10 -> sprite 0
    .629c:

        STA     (PTR1),Y
        STA     (PTR2),Y

        INC     GAME_COLNUM
        LDA     GAME_COLNUM
        CMP     #28
        BCC     .col_loop                   ; loop while GAME_COLNUM < 28

        INC     GAME_ROWNUM
        LDY     GAME_ROWNUM
        CPY     #16
        BCC     .row_loop                   ; loop while GAME_ROWNUM < 16


        JSR     DRAW_LEVEL_PAGE2
        BCC     .end                ; No error

        LDA     DISK_LEVEL_LOC
        BEQ     .reset_game

        LDX     #$00
        STX     DISK_LEVEL_LOC
        INC     GUARD_PATTERN_OFFSET
        DEX
        JMP     LOAD_LEVEL


    .end:
        RTS

    .reset_game:
        JMP     RESET_GAME



        ; Sound routines

        ORG    $62C7
    COMPRESS_AND_SAVE_LEVEL_DATA:
        SUBROUTINE

        LDA     #$00
        STA     LEVEL_DATA_INDEX
        STA     TMP
        STA     GAME_ROWNUM

    .loop:
        LDY     GAME_ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     #$00
        STY     GAME_COLNUM

    .loop2:
        LDA     TMP
        LSR
        LDA     (PTR1),Y
        BCS     .shift_left

        STA     SPRITE_NUM
        BPL     .next

    .shift_left:
        ASL
        ASL
        ASL
        ASL
        ORA     SPRITE_NUM
        LDY     LEVEL_DATA_INDEX
        STA     DISK_BUFFER,Y
        INC     LEVEL_DATA_INDEX

    .next:
        INC     TMP
        INC     GAME_COLNUM
        LDY     GAME_COLNUM
        CPY     #MAX_GAME_COL+1
        BCC     .loop2

        INC     GAME_ROWNUM
        LDA     GAME_ROWNUM
        CMP     #MAX_GAME_ROW+1
        BCC     .loop

        LDA     #DISK_ACCESS_WRITE
        JMP     ACCESS_COMPRESSED_LEVEL_DATA    ; tailcall
        ORG    $630E
    ACCESS_COMPRESSED_LEVEL_DATA:
        SUBROUTINE
        ; Enter routine with A set to command: 1 = read, 2 = write, 4 = format

        STA     IOB_COMMAND_CODE
        LDA     GAME_MODE
        LSR
        ; If GAME_MODE is 0 or 1, copy level data from image
        BEQ     .copy_level_data_from_image

        ; Otherwise, read/write/format level on disk
        LDA     DISK_LEVEL_LOC
        LSR
        LSR
        LSR
        LSR
        CLC
        ADC     #$03
        STA     IOB_TRACK_NUMBER            ; track 3 + (DISK_LEVEL_LOC >> 4)
        LDA     DISK_LEVEL_LOC
        AND     #$0F
        STA     IOB_SECTOR_NUMBER           ; sector DISK_LEVEL_LOC & 0x0F
        LDA     #<DISK_BUFFER
        STA     IOB_READ_WRITE_BUFFER_PTR
        LDA     #>DISK_BUFFER
        STA     IOB_READ_WRITE_BUFFER_PTR+1 ; IOB_READ_WRITE_BUFFER_PTR = 0D00
        LDA     #$00
        STA     IOB_VOLUME_NUMBER_EXPECTED  ; any volume

    ACCESS_DISK_OR_RESET_GAME:
        LDY     #<DOS_IOB
        LDA     #>DOS_IOB
        JSR     JMP_RWTS
        BCC     .end
        JMP     RESET_GAME      ; On error

    .end:
        RTS

    .copy_level_data_from_image:
        LDA     LEVELNUM        ; 1-based
        CLC
        ADC     #$9E
        STA     ROW_ADDR+1
        LDY     #$00
        STY     ROW_ADDR        ; ROW_ADDR <- 9E00 + LEVELNUM * 0x100

    .copyloop:
        LDA     (ROW_ADDR),Y
        STA     DISK_BUFFER,Y
        INY
        BNE     .copyloop
        RTS



        ORG    $6359
    ACCESS_HI_SCORE_DATA_FROM_DISK:
        SUBROUTINE

        STA     IOB_COMMAND_CODE
        LDA     #$0C
        STA     IOB_TRACK_NUMBER
        LDA     #$0F
        STA     IOB_SECTOR_NUMBER
        LDA     #<HI_SCORE_DATA
        STA     IOB_READ_WRITE_BUFFER_PTR
        LDA     #>HI_SCORE_DATA
        STA     IOB_READ_WRITE_BUFFER_PTR+1
        LDA     #$00
        STA     IOB_VOLUME_NUMBER_EXPECTED
        LDY     #<DOS_IOB
        LDA     #>DOS_IOB
        JSR     INDIRECT_RWTS
        BCC     .no_error
        JMP     RESET_GAME

    .no_error:
        LDY     #$0A
        LDA     #$00
        STA     MASK0       ; temp storage

    .loop:
        LDA     HI_SCORE_DATA+244,Y
        EOR     HI_SCORE_DATA_MARKER,Y
        ORA     MASK0
        STA     MASK0
        DEY
        BPL     .loop

        LDA     MASK0
        BEQ     .all_zero_data

        LDA     #$00
        RTS

    .all_zero_data:
        LDA     #$01
        LDX     $1FFF
        BNE     .end
        LDA     #$FF

    .end:
        RTS

        ORG    $63A5
    INDIRECT_RWTS:
        SUBROUTINE
        JMP     (INDIRECT_TARGET)

        ORG    $63A8
    HI_SCORE_DATA_MARKER:
        ; Spells out "LODE RUNNER".
        HEX     CC CF C4 C5 A0 D2 D5 CE CE C5 D2
        ORG    $63B3
    DRAW_LEVEL_PAGE2:
        SUBROUTINE
        ; Returns carry set if there was no player sprite in the level,
        ; or carry clear if there was.

        LDY     #MAX_GAME_ROW
        STY     GAME_ROWNUM

    .row_loop:
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     #MAX_GAME_COL
        STY     GAME_COLNUM

    .col_loop:
        LDA     (PTR1),Y
        LDX     VERBATIM
        BEQ     .draw_sprite1       ; This will then unconditionally jump to
                                    ; .draw_sprite2. We have to do that because of
                                    ; relative jump amount limitations.
        CMP     #SPRITE_INVISIBLE_LADDER
        BNE     .check_for_gold

        LDX     LADDER_COUNT
        CPX     #45
        BCS     .remove_sprite

        INC     LADDER_COUNT
        INX
        LDA     GAME_ROWNUM
        STA     LADDER_LOCS_ROW,X
        TYA
        STA     LADDER_LOCS_COL,X

    .remove_sprite:
        LDA     #SPRITE_EMPTY
        STA     (PTR1),Y
        STA     (PTR2),Y

    .draw_sprite1
        BEQ     .draw_sprite        ; Unconditional jump.
    .check_for_gold:
        CMP      #SPRITE_GOLD
        BNE     .check_for_guard

        INC     GOLD_COUNT
        BNE     .draw_sprite        ; This leads to a situation where if we wrap
                                    ; GOLD_COUNT around back to 0 (so 256 boxes)
                                    ; we end up falling through, which eventually
                                    ; just draws the sprite anyway. So this is kind
                                    ; of unconditional.

    .check_for_guard:
        CMP     #SPRITE_GUARD
        BNE     .check_for_player

        LDX     GUARD_COUNT
        CPX     #5
        BCS     .remove_sprite          ; If GUARD_COUNT >= 5, remove sprite.

        INC     GUARD_COUNT
        INX
        TYA
        STA     GUARD_LOCS_COL,X
        LDA     GAME_ROWNUM
        STA     GUARD_LOCS_ROW,X
        LDA     #$00
        STA     GUARD_GOLD_TIMERS,X
        STA     GUARD_ANIM_STATES,X
        LDA     #$02
        STA     GUARD_X_ADJS,X
        STA     GUARD_Y_ADJS,X

        LDA     #SPRITE_EMPTY
        STA     (PTR2),Y
        LDA     #SPRITE_GUARD
        BNE     .draw_sprite            ; Unconditional jump.

    .next_row:
        BPL     .row_loop
    .next_col:
        BPL     .col_loop
    .check_for_player:
        CMP     #SPRITE_PLAYER
        BNE     .check_for_t_thing

        LDX     PLAYER_COL
        BPL     .remove_sprite          ; If PLAYER_COL > 0, remove sprite.

        STY     PLAYER_COL
        LDX     GAME_ROWNUM
        STX     PLAYER_ROW
        LDX     #$02
        STX     PLAYER_X_ADJ
        STX     PLAYER_Y_ADJ            ; Set Player X and Y movement to 0.
        LDX     #$08
        STX     PLAYER_ANIM_STATE       ; Corresponds to sprite 9 (see SPRITE_ANIM_SEQS)

        LDA     #SPRITE_EMPTY
        STA     (PTR2),Y
        LDA     #SPRITE_PLAYER
        BNE     .draw_sprite            ; Unconditional jump.
    .check_for_t_thing:
        CMP     #SPRITE_TRAP
        BNE     .draw_sprite
        LDA     #SPRITE_BRICK

        ; fallthrough to .draw_sprite
    .draw_sprite:
        JSR     DRAW_SPRITE_PAGE2

        DEC     GAME_COLNUM
        LDY     GAME_COLNUM
        BPL     .next_col               ; Jumps to .col_loop

        DEC     GAME_ROWNUM
        LDY     GAME_ROWNUM
        BPL     .next_row               ; Jumps to .row_loop
        LDA     VERBATIM
        BEQ     .copy_page2_to_page1

        LDA     PLAYER_COL
        BPL     .reveal_screen

        SEC                             ; Oops, no player! Return error.
        RTS
    .copy_page2_to_page1:
        LDA     #$20
        STA     ROW_ADDR2+1
        LDA     #$40
        STA     ROW_ADDR+1
        LDA     #$00
        STA     ROW_ADDR2
        STA     ROW_ADDR
        TAY

    .copy_loop:
        LDA     (ROW_ADDR),Y
        STA     (ROW_ADDR2),Y
        INY
        BNE     .copy_loop

        INC     ROW_ADDR2+1
        INC     ROW_ADDR+1
        LDX     ROW_ADDR+1
        CPX     #$60
        BCC     .copy_loop

        CLC
        RTS
    .reveal_screen
        JSR     IRIS_WIPE

        LDY     #MAX_GAME_ROW
        STY     GAME_ROWNUM

    .row_loop2:
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     #MAX_GAME_COL
        STY     GAME_COLNUM

    .col_loop2:
        LDA     (PTR1),Y
        CMP     #SPRITE_PLAYER
        BEQ     .remove
        CMP     #SPRITE_GUARD
        BNE     .next

    .remove:
        LDA     #SPRITE_EMPTY
        JSR     DRAW_SPRITE_PAGE2

    .next:
        DEC     GAME_COLNUM
        LDY     GAME_COLNUM
        BPL     .col_loop2

        DEC     GAME_ROWNUM
        LDY     GAME_ROWNUM
        BPL     .row_loop2

        CLC
        RTS

        ORG    $8631
    ENABLE_NEXT_LEVEL_LADDERS:
        SUBROUTINE

        LDA     #$00
        STA     LADDER_LOCS_COL     ; LADDER_LOCS_COL = 0

        LDX     LADDER_COUNT
        STX     .count              ; .count backwards from LADDER_COUNT to 0
    .loop:
        LDX     .count
        BEQ     .dec_gold_count_if_no_ladder

        LDA     LADDER_LOCS_COL,X   ; A = LADDER_LOCS_COL[X]
        BMI     .next               ; If not present, next.

        STA     GAME_COLNUM         ; GAME_COLNUM = LADDER_LOCS_COL[X]
        LDA     LADDER_LOCS_ROW,X
        STA     GAME_ROWNUM         ; GAME_ROWNUM = LADDER_LOCS_ROW[X]
        TAY
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GAME_COLNUM
        LDA     (PTR2),Y            ; A = sprite at ladder loc
        BNE     .set_col_to_1

        LDA     #SPRITE_LADDER
        STA     (PTR2),Y            ; Set background sprite to ladder
        LDA     (PTR1),Y
        BNE     .draw_ladder        ; .draw_ladder if active sprite not blank

        LDA     #SPRITE_LADDER
        STA     (PTR1),Y            ; Set active sprite to ladder

    .draw_ladder:
        LDA     #SPRITE_LADDER
        JSR     DRAW_SPRITE_PAGE2   ; Draw ladder on background page

        LDX     GAME_COLNUM
        LDY     GAME_ROWNUM
        JSR     GET_SCREEN_COORDS_FOR
        LDA     #SPRITE_LADDER
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS ; Draw ladder on active page

        LDX     .count
        LDA     #$FF
        STA     LADDER_LOCS_COL,X       ; Remove ladder loc
        BMI     .next                   ; Unconditional

    .set_col_to_1:
        LDA     #$01
        STA     LADDER_LOCS_COL         ; LADDER_LOCS_COL = 1

    .next:
        DEC     .count
        JMP     .loop

    .dec_gold_count_if_no_ladder:
        LDA     LADDER_LOCS_COL
        BNE     .end
        DEC     GOLD_COUNT

    .end:
        RTS

    .count:
        BYTE    0

*/
#include "lode_runner.h"

const uint8_t HI_SCORE_DATA_MARKER[11] = {
    0xCC, 0xCF, 0xC4, 0xC5, 0xA0, 0xD2, 0xD5, 0xCE, 0xCE, 0xC5, 0xD2,
};

/* Draws the level from LEVEL_MAP on page 2, the bottom row first and
 * each row from the right, and then shows it on page 1. Unless
 * VERBATIM is 0, it takes the hidden ladders, the guards, and the
 * player out as it goes (see the top of this file), draws a trap as a
 * brick, and opens the iris on page 2 before taking the guards and the
 * player off it. Returns true if VERBATIM isn't 0 and the level has no
 * player. */
static bool draw_level_page2(void) {
    GAME_ROWNUM = MAX_GAME_ROW;
    do {
        GAME_COLNUM = MAX_GAME_COL;
        do {
            uint8_t col = GAME_COLNUM;
            uint8_t sprite = LEVEL_MAP[GAME_ROWNUM][col];
            if (VERBATIM) {
                uint8_t n;
                switch (sprite) {
                case SPRITE_INVISIBLE_LADDER:
                    if (LADDER_COUNT < 45) {
                        n = ++LADDER_COUNT;
                        LADDER_LOCS_ROW[n] = GAME_ROWNUM;
                        LADDER_LOCS_COL[n] = col;
                    }
                    goto remove;
                case SPRITE_GOLD:
                    GOLD_COUNT++;
                    break;
                case SPRITE_GUARD:
                    if (GUARD_COUNT >= 5)
                        goto remove;
                    n = ++GUARD_COUNT;
                    GUARD_LOCS_COL[n] = col;
                    GUARD_LOCS_ROW[n] = GAME_ROWNUM;
                    GUARD_GOLD_TIMERS[n] = 0;
                    GUARD_ANIM_STATES[n] = 0;
                    GUARD_X_ADJS[n] = 2;
                    GUARD_Y_ADJS[n] = 2;
                    LEVEL_BG[GAME_ROWNUM][col] = SPRITE_EMPTY;
                    break;
                case SPRITE_PLAYER:
                    /* PLAYER_COL is 0xFF until a player is found. */
                    if (!(PLAYER_COL & 0x80))
                        goto remove;
                    PLAYER_COL = col;
                    PLAYER_ROW = GAME_ROWNUM;
                    PLAYER_X_ADJ = 2;
                    PLAYER_Y_ADJ = 2;
                    /* SPRITE_ANIM_SEQS[8] is sprite 9, the player
                     * running right. */
                    PLAYER_ANIM_STATE = 8;
                    LEVEL_BG[GAME_ROWNUM][col] = SPRITE_EMPTY;
                    break;
                case SPRITE_TRAP:
                    sprite = SPRITE_BRICK;
                    break;
                remove:
                    sprite = SPRITE_EMPTY;
                    LEVEL_MAP[GAME_ROWNUM][col] = SPRITE_EMPTY;
                    LEVEL_BG[GAME_ROWNUM][col] = SPRITE_EMPTY;
                    break;
                }
            }
            draw_sprite_page2(sprite);
        } while ((int8_t)--GAME_COLNUM >= 0);
    } while ((int8_t)--GAME_ROWNUM >= 0);

    if (!VERBATIM) {
        /* Copy page 2 to page 1. */
        const uint8_t *src = HGR2;
        uint8_t *dst = HGR1;
        while (dst != HGR1 + 0x2000)
            *dst++ = *src++;
        return false;
    }
    if (PLAYER_COL & 0x80)
        return true;
    iris_wipe();

    /* Leave page 2 with only what's behind the guards and the player. */
    GAME_ROWNUM = MAX_GAME_ROW;
    do {
        GAME_COLNUM = MAX_GAME_COL;
        do {
            uint8_t sprite = LEVEL_MAP[GAME_ROWNUM][GAME_COLNUM];
            if (sprite == SPRITE_PLAYER || sprite == SPRITE_GUARD)
                draw_sprite_page2(SPRITE_EMPTY);
        } while ((int8_t)--GAME_COLNUM >= 0);
    } while ((int8_t)--GAME_ROWNUM >= 0);
    return false;
}

/* Reads the level (from the disk at DISK_LEVEL_LOC, or in the demo
 * level LEVELNUM from memory) into LEVEL_MAP and LEVEL_BG, clears the
 * holes and the dead guards, and draws it with draw_level_page2.
 * VERBATIM is set to `verbatim`, so that 0 draws the level as it's
 * stored. A level with no player is replaced by the disk's first
 * level, with the guards a step faster, or starts the game over if it
 * is the disk's first level. */
void load_level(bool verbatim) {
    for (;;) {
        VERBATIM = verbatim;
        PLAYER_COL = 0xFF;
        LADDER_COUNT = 0;
        GOLD_COUNT = 0;
        GUARD_COUNT = 0;
        GUARD_NUM = 0;
        DIG_ANIM_STATE = 0;
        for (uint8_t i = 0; i < 31; i++)
            BRICK_FILL_TIMERS[i] = 0;
        for (uint8_t i = 0; i < 6; i++)
            GUARD_RESURRECTION_TIMERS[i] = 0;
        ALIVE = 1;
        access_compressed_level_data(DISK_ACCESS_READ);

        uint8_t index = 0;
        for (uint8_t row = 0; row < LEVEL_ROWS; row++) {
            for (uint8_t col = 0; col < LEVEL_COLS; col++) {
                uint8_t sprite = DISK_BUFFER[index];
                if (col & 1) {
                    sprite >>= 4;
                    index++;
                } else {
                    sprite &= 0x0F;
                }
                if (sprite >= 10)
                    sprite = SPRITE_EMPTY;
                LEVEL_MAP[row][col] = sprite;
                LEVEL_BG[row][col] = sprite;
            }
        }

        if (!draw_level_page2())
            return;
        if (DISK_LEVEL_LOC == 0)
            reset_game();
        DISK_LEVEL_LOC = 0;
        GUARD_PATTERN_OFFSET++;
        verbatim = true;
    }
}

/* Packs LEVEL_MAP into DISK_BUFFER and writes it to the level's
 * sector. */
void compress_and_save_level_data(void) {
    uint8_t index = 0;
    uint8_t low = 0;
    for (uint8_t row = 0; row < LEVEL_ROWS; row++) {
        for (uint8_t col = 0; col < LEVEL_COLS; col++) {
            uint8_t sprite = LEVEL_MAP[row][col];
            if (col & 1)
                DISK_BUFFER[index++] = (uint8_t)(sprite << 4) | low;
            else
                low = sprite;
        }
    }
    access_compressed_level_data(DISK_ACCESS_WRITE);
}

void access_compressed_level_data(uint8_t command) {
    DOS_IOB.command = command;
    if (GAME_MODE < GAME_MODE_PLAY_MODE) {
        const uint8_t *level = DEMO_LEVELS[LEVELNUM];
        uint8_t i = 0;
        do {
            DISK_BUFFER[i] = level[i];
        } while (++i != 0);
        return;
    }
    DOS_IOB.track = (DISK_LEVEL_LOC >> 4) + 3;
    DOS_IOB.sector = DISK_LEVEL_LOC & 0x0F;
    DOS_IOB.buffer_addr = DISK_BUFFER;
    DOS_IOB.volume = 0;
    access_disk_or_reset_game();
}

void access_disk_or_reset_game(void) {
    if (RWTS(&DOS_IOB))
        reset_game();
}

uint8_t access_hi_score_data_from_disk(uint8_t command) {
    DOS_IOB.command = command;
    DOS_IOB.track = 12;
    DOS_IOB.sector = 15;
    DOS_IOB.buffer_addr = (uint8_t *)HI_SCORES;
    DOS_IOB.volume = 0;
    if (RWTS(&DOS_IOB))
        reset_game();

    uint8_t differ = 0;
    for (uint8_t i = 0; i < 11; i++)
        differ |= HI_SCORE_MARKER[i] ^ HI_SCORE_DATA_MARKER[i];
    if (differ)
        return 0;
    return HI_SCORE_MASTER_FLAG ? 1 : 0xFF;
}

/* Draws the hidden ladders once the gold is all picked up, on both
 * pages and in both maps. A ladder whose place in LEVEL_BG isn't empty
 * is left for a later turn, and LADDER_LOCS_COL[0] is set to say so.
 * When none are left, GOLD_COUNT goes from 0 to 0xFF. */
void enable_next_level_ladders(void) {
    LADDER_LOCS_COL[0] = 0;
    for (uint8_t n = LADDER_COUNT; n != 0; n--) {
        uint8_t col = LADDER_LOCS_COL[n];
        if (col & 0x80)
            continue;   /* already drawn */
        uint8_t row = LADDER_LOCS_ROW[n];
        GAME_COLNUM = col;
        GAME_ROWNUM = row;
        uint8_t *bg = &LEVEL_BG[row][col];
        uint8_t *map = &LEVEL_MAP[row][col];
        if (*bg != SPRITE_EMPTY) {
            LADDER_LOCS_COL[0] = 1;
            continue;
        }
        *bg = SPRITE_LADDER;
        if (*map == SPRITE_EMPTY)
            *map = SPRITE_LADDER;
        draw_sprite_page2(SPRITE_LADDER);
        draw_sprite_at_pixel_coords(SPRITE_LADDER, HALF_SCREEN_COL_TABLE[col],
                                    SCREEN_ROW_TABLE[row]);
        LADDER_LOCS_COL[n] = 0xFF;
    }
    if (LADDER_LOCS_COL[0] == 0)
        GOLD_COUNT--;
}
