/*
    prompts.c -- asking for the save diskette and giving the game
    diskette back.

    Zork I (r15), output/main.asm lines 3452-3632: print_ascii_string,
    please_insert_save_diskette, get_prompted_number_from_user, and
    please_reinsert_game_diskette, with their strings; and cout_string,
    lines 3282-3292:

    cout_string:
        LDY      #$00
    .loop:
        LDA      (SCRATCH2),Y
        ORA      #$80
        JSR      COUT1
        INY
        DEX
        BNE      .loop
        RTS

    The README in this directory quotes the rest.

    Departures from the original:

    - print_ascii_string and cout_string take the string as a
      parameter, where the original passes it in SCRATCH2. Neither
      calls the other, so the compiler gives both parameters the same
      two bytes, and every caller writes the string's address there.
      cout_string is a copy of Zork's, which stays where it is for the
      rest of Zork.

    - The three prompts and their defaults are one 16-byte record each,
      as in the original, but get_prompted_number_from_user takes the
      record's index in A instead of its byte offset in prompt_offset.
    - print_ascii_string keeps its count and index in locals where the
      original uses SCRATCH3 and SCRATCH3+1.
*/
#include "zork.h"

static const char sPleaseInsert[28] = "PLEASE INSERT SAVE DISKETTE,";
static const char sDefault[10] = "DEFAULT = ";
static const char sReturnToBegin[35] = "--- PRESS 'RETURN' KEY TO BEGIN ---";
static const char sReinsertGameDiskette[31] = "PLEASE RE-INSERT GAME DISKETTE,";
static const char sPressReturnToContinue[38] =
    "--- PRESS 'RETURN' KEY TO CONTINUE ---";

/* A prompt and its default answer, which the last answer replaces. */
struct prompt {
    char text[15];
    char answer;
};

enum { PROMPT_SLOT, PROMPT_DRIVE, PROMPT_POSITION };

struct prompt prompts[3] = {
    { "SLOT     (1-7):", '6' },
    { "DRIVE    (1-2):", '2' },
    { "POSITION (0-7):", '0' },
};

/* Adds the `count` characters at `s` to the line buffer. A count of 0
 * adds 256. */
static void print_ascii_string(
    const char *const s, uint8_t count [[c6502::in_x]]) {
    uint8_t i = 0;
    do {
        buffer_char(s[i]);
        i++;
    } while (--count != 0);
}

/* Prints the `count` characters at `s` straight to the screen, with
 * their high bits set. A count of 0 prints 256. */
static void cout_string(
    const char *const s, uint8_t count [[c6502::in_x]]) {
    uint8_t i = 0;
    do {
        COUT1(s[i] | 0x80);
        i++;
    } while (--count != 0);
}

/* Prints prompt `which` and its default in inverse, reads a key, and
 * returns it without its high bit, or the default for RETURN. */
uint8_t get_prompted_number_from_user(register uint8_t which) {
    dump_buffer_with_more();
    print_ascii_string(prompts[which].text, 15);
    dump_buffer_line();
    CH = 25;
    INVFLG = 0x3F;
    cout_string(sDefault, 10);
    cout_string(&prompts[which].answer, 1);
    INVFLG = 0xFF;
    uint8_t key = RDKEY();
    CH = 25;
    CLREOL();
    if (key == 0x8D)
        key = prompts[which].answer;
    return key & 0x7F;
}

/* Asks for the save diskette's position, slot, and drive, points the
 * IOB at the drive, and waits for RETURN. Leaves SCRATCH1 one before
 * the position's first sector: each position is 64 sectors. */
void please_insert_save_diskette(void) {
    home();
    dump_buffer_with_more();
    dump_buffer_with_more();
    print_ascii_string(sPleaseInsert, 28);
    dump_buffer_with_more();
    uint8_t c;
    do {
        c = get_prompted_number_from_user(PROMPT_POSITION);
    } while (c < '0' || c >= '8');
    prompts[PROMPT_POSITION].answer = c;
    buffer_char(c);
    do {
        c = get_prompted_number_from_user(PROMPT_SLOT);
    } while (c < '1' || c >= '8');
    iob.slot_times_16 = (uint8_t)((c & 0x07) << 4);
    prompts[PROMPT_SLOT].answer = c;
    buffer_char(c);
    do {
        c = get_prompted_number_from_user(PROMPT_DRIVE);
    } while (c < '1' || c >= '3');
    iob.drive_number = c & 0x03;
    prompts[PROMPT_DRIVE].answer = c;
    buffer_char(c);
    do {
        dump_buffer_with_more();
        print_ascii_string(sReturnToBegin, 35);
        dump_buffer_line();
    } while (RDKEY() != 0x8D);
    SCRATCH1 = (64 * (prompts[PROMPT_POSITION].answer & 0x07)) - 1;
    dump_buffer_with_more();
}

/* If the save diskette is in the game's drive (slot 6, drive 1), asks
 * for the game diskette back and waits for RETURN. Points the IOB at
 * that drive again either way. */
void please_reinsert_game_diskette(void) {
    if (iob.slot_times_16 == 0x60 && iob.drive_number == 0x01) {
        dump_buffer_with_more();
        print_ascii_string(sReinsertGameDiskette, 31);
        do {
            dump_buffer_with_more();
            print_ascii_string(sPressReturnToContinue, 38);
            dump_buffer_line();
        } while (RDKEY() != 0x8D);
        dump_buffer_with_more();
    }
    iob.slot_times_16 = 0x60;
    iob.drive_number = 0x01;
}
