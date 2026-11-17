/*
    hiscore.c -- showing the high-score table, and putting a new score
    in it.

    The table is the high-score sector, which the game reads from the
    disk and writes back to it. It holds ten entries, best first. An
    entry with a level of 0 is empty, and the screen shows only its
    number.

    Departures from the original:

    - An entry is HI_SCORES[n - 1], where the original adds
      HI_SCORE_TABLE_OFFSETS[n] to HI_SCORE_DATA, so the offsets table
      isn't here.
    - The entry's number and offset in HI_SCORE_SCREEN, which the
      original keeps in HI_SCORE_INDEX and HI_SCORE_OFFSET, are a loop
      counter and a pointer.
    - RECORD_HI_SCORE_DATA_TO_DISK keeps the new entry's number in a
      local, where the original keeps it in HI_SCORE_TARGET_INDEX and
      then WIPE0. Its .rd_loc and .wr_loc instructions, whose operands
      the original sets to the new entry's address, are a pointer to
      the entry's initials.
    - The entries that move down to make room for the new one are
      copied a byte at a time from the end of entry 9 back, where the
      original copies entry 9, then 8, and so on, each from its first
      byte. The bytes that end up in the table are the same.
    - HIGH_SCORE_INITIALS_INDEX and KBD_ENTRY_INDEX are the same byte
      in the original, $824D, and here they're the one static variable
      kbd_entry_index. editor.c keeps its own copy, so a value one
      routine leaves there isn't seen by the other, but each sets it
      before reading it.

    Lode Runner, main.asm lines 4730-4864:

        ORG    $786B

    HI_SCORE_SCREEN:
        SUBROUTINE

        JSR     CLEAR_HGR2
        LDA     #$40
        STA     DRAW_PAGE
        LDA     #$00
        STA     GAME_COLNUM
        STA     GAME_ROWNUM

        ; "    LODE RUNNER HIGH SCORES\r"
        ; "\r"
        ; "\r"
        ; "    INITIALS LEVEL  SCORE\r"
        ; "    -------- ----- --------\r"
        JSR     PUT_STRING
        HEX     A0 A0 A0 A0 CC CF C4 C5 A0 D2 D5 CE CE C5 D2 A0
        HEX     C8 C9 C7 C8 A0 D3 C3 CF D2 C5 D3 8D 8D 8D A0 A0
        HEX     A0 A0 C9 CE C9 D4 C9 C1 CC D3 A0 CC C5 D6 C5 CC
        HEX     A0 A0 D3 C3 CF D2 C5 8D A0 A0 A0 A0 AD AD AD AD
        HEX     AD AD AD AD A0 AD AD AD AD AD A0 AD AD AD AD AD
        HEX     AD AD AD 8D 00

        LDA     #$01
        STA     HI_SCORE_INDEX             ; Used for row number
    .loop:
        CMP     #$0A
        BNE     .display_0_to_9
        LDA     #1
        JSR     PUT_DIGIT
        LDA     #0
        JSR     PUT_DIGIT
        JMP     .rest_of_row_number

    .display_0_to_9:
        LDA     #$A0
        JSR     PUT_CHAR        ; space
        LDA     HI_SCORE_INDEX
        JSR     PUT_DIGIT

    .rest_of_row_number:
        ; ".    "
        JSR     PUT_STRING
        HEX     AE A0 A0 A0 A0 00

        LDX     HI_SCORE_INDEX
        LDY     HI_SCORE_TABLE_OFFSETS,X
        STY     HI_SCORE_OFFSET
        LDA     HI_SCORE_DATA+3,Y
        BNE     .draw_initials
        JMP     .next_high_score_row
    .draw_initials:
        LDY     HI_SCORE_OFFSET
        LDA     HI_SCORE_DATA,Y
        JSR     PUT_CHAR
        LDY     HI_SCORE_OFFSET
        LDA     HI_SCORE_DATA+1,Y
        JSR     PUT_CHAR
        LDY     HI_SCORE_OFFSET
        LDA     HI_SCORE_DATA+2,Y
        JSR     PUT_CHAR

        ; "    "
        JSR     PUT_STRING
        HEX     A0 A0 A0 A0 00

        LDY     HI_SCORE_OFFSET
        LDA     HI_SCORE_DATA+3,Y
        JSR     TO_DECIMAL3
        LDA     HUNDREDS
        JSR     PUT_DIGIT
        LDA     TENS
        JSR     PUT_DIGIT
        LDA     UNITS
        JSR     PUT_DIGIT

        ; "  "
        JSR     PUT_STRING
        HEX     A0 A0 00

        LDY     HI_SCORE_OFFSET
        LDA     HI_SCORE_DATA+4,Y
        JSR     BCD_TO_DECIMAL2
        LDA     TENS
        JSR     PUT_DIGIT
        LDA     UNITS
        JSR     PUT_DIGIT

        LDY     HI_SCORE_OFFSET
        LDA     HI_SCORE_DATA+5,Y
        JSR     BCD_TO_DECIMAL2
        LDA     TENS
        JSR     PUT_DIGIT
        LDA     UNITS
        JSR     PUT_DIGIT

        LDY     HI_SCORE_OFFSET
        LDA     HI_SCORE_DATA+6,Y
        JSR     BCD_TO_DECIMAL2
        LDA     TENS
        JSR     PUT_DIGIT
        LDA     UNITS
        JSR     PUT_DIGIT

        LDY     HI_SCORE_OFFSET
        LDA     HI_SCORE_DATA+7,Y
        JSR     BCD_TO_DECIMAL2
        LDA     TENS
        JSR     PUT_DIGIT
        LDA     UNITS
        JSR     PUT_DIGIT

    .next_high_score_row:
        JSR     NEWLINE
        INC     HI_SCORE_INDEX
        LDA     HI_SCORE_INDEX
        CMP     #11
        BCS     .end
        JMP     .loop


    .end:
        STA     TXTPAGE2        ; Flip to page 2
        LDA     #$20
        STA     DRAW_PAGE       ; Set draw page to 1
        RTS


        ORG    $79A2
        ; Because table indices are 1-based, there's an extra 00
        ; at the beginning that never gets used.
    HI_SCORE_TABLE_OFFSETS:
        HEX     00 00 08 10 18 20 28 30 38 40 48

    Lode Runner, main.asm lines 6543-6720:

    RECORD_HI_SCORE_DATA_TO_DISK:
        SUBROUTINE

        LDA     $9D
        BEQ     .end

        LDA     SCORE
        ORA     SCORE+1
        ORA     SCORE+2
        ORA     SCORE+3
        BEQ     .end

        LDA     #$01
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK      ; read table
        ; Return value of 0 means the hi score marker wasn't present,
        ; so don't write the hi score table.
        BEQ     .end

        LDY     #$01
    .loop:
        LDX     HI_SCORE_TABLE_OFFSETS,Y
        LDA     LEVELNUM
        CMP     HI_SCORE_DATA+3,X       ; level
        BCC     .next
        BNE     .record_it

        LDA     SCORE+3
        CMP     HI_SCORE_DATA+4,X
        BCC     .next
        BNE     .record_it

        LDA     SCORE+2
        CMP     HI_SCORE_DATA+5,X
        BCC     .next
        BNE     .record_it

        LDA     SCORE+1
        CMP     HI_SCORE_DATA+6,X
        BCC     .next
        BNE     .record_it

        LDA     SCORE
        CMP     HI_SCORE_DATA+7,X
        BCC     .next
        BNE     .record_it

    .next:
        INY
        CPY     #$0B
        BCC     .loop

    .end:
        RTS

    .record_it:
        CPY     #$0A
        BEQ     .write_here
        STY     HI_SCORE_TARGET_INDEX

        ; Move the table rows to make room at index HI_SCORE_TARGET_INDEX
        LDY     #$09
    .loop2:
        LDX     HI_SCORE_TABLE_OFFSETS,Y

        ; Move 8 bytes of hi score data
        LDA     #$08
        STA     ROW_COUNT       ; temporary counter
    .loop3:
        LDA     HI_SCORE_DATA,X
        STA     HI_SCORE_DATA+8,X
        INX
        DEC     ROW_COUNT
        BNE     .loop3

        CPY     HI_SCORE_TARGET_INDEX
        BEQ     .write_here
        DEY
        BNE     .loop2

    .write_here:
        LDX     HI_SCORE_TABLE_OFFSETS,Y
        LDA     #$A0
        STA     HI_SCORE_DATA,X
        STA     HI_SCORE_DATA+1,X
        STA     HI_SCORE_DATA+2,X
        LDA     LEVELNUM
        STA     HI_SCORE_DATA+3,X
        LDA     SCORE+3
        STA     HI_SCORE_DATA+4,X
        LDA     SCORE+2
        STA     HI_SCORE_DATA+5,X
        LDA     SCORE+1
        STA     HI_SCORE_DATA+6,X
        LDA     SCORE
        STA     HI_SCORE_DATA+7,X
        STY     WIPE0               ; temporary
        LDA     HI_SCORE_TABLE_OFFSETS,Y
        STA     .rd_loc+1
        STA     .wr_loc+1
        JSR     HI_SCORE_SCREEN

        LDA     #$40
        STA     DRAW_PAGE
        LDA     WIPE0
        CLC
        ADC     #$04
        STA     GAME_ROWNUM
        LDA     #$07
        STA     GAME_COLNUM

        LDX     #$00
        STX     HIGH_SCORE_INITIALS_INDEX
    .get_initial_from_player:
        LDX     HIGH_SCORE_INITIALS_INDEX
    .rd_loc:
        LDA     HI_SCORE_DATA,X     ; fixed up to add offset from above
        JSR     CHAR_TO_SPRITE_NUM
        JSR     WAIT_FOR_KEY
        STA     KBDSTRB
        CMP     #$8D
        BEQ     .return_pressed
        CMP     #$88                ; backspace/back arrow
        BNE     .other_key_pressed

        ; backspace pressed
        LDX     KBD_ENTRY_INDEX
        BEQ     .beep       ; can't backspace/back arrow past the beginning

        DEC     HIGH_SCORE_INITIALS_INDEX
        DEC     GAME_COLNUM
        JMP     .get_initial_from_player

    .other_key_pressed:
        CMP     #$95            ; fwd arrow
        BNE     .check_for_allowed_chars
        LDX     KBD_ENTRY_INDEX
        CPX     #$02
        BEQ     .beep       ; can't fwd arrow past the end

        INC     GAME_COLNUM
        INC     KBD_ENTRY_INDEX
        JMP     .get_initial_from_player

    .check_for_allowed_chars
        CMP     #$AE        ; period allowed
        BEQ     .put_char
        CMP     #$A0        ; space allowed
        BEQ     .put_char
        CMP     #$C1
        BCC     .beep       ; can't be less than 'A'
        CMP     #$DB
        BCS     .beep       ; can't be greater than 'Z'

    .put_char
        LDY     KBD_ENTRY_INDEX
    .wr_loc:
        STA     HI_SCORE_DATA,Y     ; fixed up to add offset from above
        JSR     PUT_CHAR
        INC     KBD_ENTRY_INDEX
        LDA     KBD_ENTRY_INDEX
        CMP     #$03
        BCC     .get_initial_from_player

        DEC     KBD_ENTRY_INDEX
        DEC     GAME_COLNUM
        JMP     .get_initial_from_player

    .beep:
        JSR     BEEP
        JMP     .get_initial_from_player

    .return_pressed:
        LDA     #$20
        STA     DRAW_PAGE
        LDA     #$02
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK      ; write hi score table
        JMP     LONG_DELAY

*/
#include "lode_runner.h"

/* The initial the player is typing, 0-2. The original's byte at $824D
 * starts out as 0x60. */
static uint8_t kbd_entry_index = 0x60;

/* Draws the high-score table on page 2, shows page 2, and sets
 * DRAW_PAGE back to page 1. */
void hi_score_screen(void) {
    clear_hgr2();
    DRAW_PAGE = 0x40;
    GAME_COLNUM = 0;
    GAME_ROWNUM = 0;
    put_string("    LODE RUNNER HIGH SCORES\r\r\r"
               "    INITIALS LEVEL  SCORE\r"
               "    -------- ----- --------\r");

    for (uint8_t n = 1; n < 11; n++) {
        if (n == 10) {
            put_digit(1);
            put_digit(0);
        } else {
            put_char(0xA0);
            put_digit(n);
        }
        put_string(".    ");

        struct hi_score *entry = &HI_SCORES[n - 1];
        if (entry->level != 0) {
            put_char(entry->initials[0]);
            put_char(entry->initials[1]);
            put_char(entry->initials[2]);
            put_string("    ");

            to_decimal3(entry->level);
            put_digit(DIGITS[0]);
            put_digit(DIGITS[1]);
            put_digit(DIGITS[2]);
            put_string("  ");

            for (uint8_t i = 0; i < 4; i++) {
                bcd_to_decimal2(entry->score[i]);
                put_digit(DIGITS[1]);
                put_digit(DIGITS[2]);
            }
        }
        newline();
    }

    TXTPAGE2 = 0;
    DRAW_PAGE = 0x20;
}

/* Returns true if the game's level and score beat `entry`'s. The level
 * counts first, and then the score's digits, highest first. A tie
 * beats nothing. */
static bool beats(struct hi_score *entry) {
    if (LEVELNUM != entry->level)
        return LEVELNUM > entry->level;
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t digits = SCORE[3 - i];
        if (digits != entry->score[i])
            return digits > entry->score[i];
    }
    return false;
}

/* Puts the game's level and score in the high-score table if they beat
 * an entry, lets the player type initials for it, writes the table to
 * the disk, and goes on to long_delay, which doesn't return. Returns
 * without doing anything if the score may not go in the table, is 0,
 * doesn't beat any entry, or the disk isn't a data disk. */
void record_hi_score_data_to_disk(void) {
    if (HI_SCORE_ELIGIBLE == 0)
        return;
    if ((SCORE[0] | SCORE[1] | SCORE[2] | SCORE[3]) == 0)
        return;
    if (access_hi_score_data_from_disk(DISK_ACCESS_READ) == 0)
        return;

    uint8_t n = 1;
    while (!beats(&HI_SCORES[n - 1])) {
        if (++n == 11)
            return;
    }

    /* Move entries n-9 down one place, which drops entry 10. The
     * table is copied a byte at a time, from the last byte of entry 9
     * back to the first byte of entry n. An entry is 8 bytes, so entry
     * n starts at (n - 1) << 3. HI_SCORES[e] = HI_SCORES[e - 1] for
     * each entry took 110 bytes more, because the compiler copies each
     * entry through eight bytes of temporaries, and `i-- != first` as
     * the test took 6 bytes more than the decrement in the body. */
    uint8_t first = (uint8_t)((n - 1) << 3);
    for (uint8_t i = 9 << 3; i != first;) {
        i--;
        ((uint8_t *)HI_SCORES)[i + sizeof(struct hi_score)] =
            ((uint8_t *)HI_SCORES)[i];
    }

    struct hi_score *entry = &HI_SCORES[n - 1];
    entry->initials[0] = 0xA0;
    entry->initials[1] = 0xA0;
    entry->initials[2] = 0xA0;
    entry->level = LEVELNUM;
    entry->score[0] = SCORE[3];
    entry->score[1] = SCORE[2];
    entry->score[2] = SCORE[1];
    entry->score[3] = SCORE[0];
    uint8_t *const initials = entry->initials;
    hi_score_screen();

    /* Entry n's row on the screen is n + 4, below the heading's five
     * rows, and its initials start in column 7. */
    DRAW_PAGE = 0x40;
    GAME_ROWNUM = n + 4;
    GAME_COLNUM = 7;
    kbd_entry_index = 0;
    for (;;) {
        uint8_t key = wait_for_key(char_to_sprite_num(initials[kbd_entry_index]));
        KBDSTRB = key;
        if (key == 0x8D)                    /* Return */
            break;
        if (key == 0x88) {                  /* left arrow */
            if (kbd_entry_index == 0) {
                beep();
                continue;
            }
            kbd_entry_index--;
            GAME_COLNUM--;
            continue;
        }
        if (key == 0x95) {                  /* right arrow */
            if (kbd_entry_index == 2) {
                beep();
                continue;
            }
            GAME_COLNUM++;
            kbd_entry_index++;
            continue;
        }
        if (key != 0xAE && key != 0xA0 && (key < 0xC1 || key >= 0xDB)) {
            /* Only letters, '.', and space are allowed. */
            beep();
            continue;
        }
        initials[kbd_entry_index] = key;
        put_char(key);
        kbd_entry_index++;
        /* After the third initial the cursor stays on it. */
        if (kbd_entry_index >= 3) {
            kbd_entry_index--;
            GAME_COLNUM--;
        }
    }

    DRAW_PAGE = 0x20;
    access_hi_score_data_from_disk(DISK_ACCESS_WRITE);
    long_delay();
}
