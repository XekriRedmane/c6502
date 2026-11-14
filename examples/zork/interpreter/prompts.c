/*
    prompts.c -- asking for the save diskette and giving the game
    diskette back.

    Departures from the original:

    - The three prompts and their defaults are one 16-byte record each,
      as in the original, but get_prompted_number_from_user takes the
      record's index in A instead of its byte offset in prompt_offset.
    - please_insert_save_diskette returns the sector before the save's
      first, where the original leaves it in SCRATCH1.

    Zork I (r15), output/main.asm lines 3467-3633:

    sPleaseInsert:
        DC      "PLEASE INSERT SAVE DISKETTE,"
    prompt_offset:
        DC      0
    sSlotPrompt:
        DC      "SLOT     (1-7):"
    save_slot:
        DC      '6
    sDrivePrompt:
        DC      "DRIVE    (1-2):"
    save_drive:
        DC      '2
    sPositionPrompt:
        DC      "POSITION (0-7):"
    save_position:
        DC      '0
    sDefault:
        DC      "DEFAULT = "
    sReturnToBegin:
        DC      "--- PRESS 'RETURN' KEY TO BEGIN ---"
    please_insert_save_diskette:
        JSR      home
        JSR      dump_buffer_with_more
        JSR      dump_buffer_with_more
        STOW     sPleaseInsert, SCRATCH2
        LDX      #28
        JSR      print_ascii_string
        JSR      dump_buffer_with_more
    .get_position_from_user:
        LDA      #(sPositionPrompt-sSlotPrompt)
        STA      prompt_offset
        JSR      get_prompted_number_from_user
        CMP      #'0
        BCC      .get_position_from_user
        CMP      #'8
        BCS      .get_position_from_user
        STA      save_position
        JSR      buffer_char
    .get_slot_from_user:
        LDA      #(sSlotPrompt - sSlotPrompt)
        STA      prompt_offset
        JSR      get_prompted_number_from_user
        CMP      #'1
        BCC      .get_slot_from_user
        CMP      #'8
        BCS      .get_slot_from_user
        TAX
        AND      #$07
        ASL
        ASL
        ASL
        ASL
        STA      iob.slot_times_16
        TXA
        STA      save_slot
        JSR      buffer_char
    .get_drive_from_user:
        LDA      #(sDrivePrompt - sSlotPrompt)
        STA      prompt_offset
        JSR      get_prompted_number_from_user
        CMP      #'1
        BCC      .get_drive_from_user
        CMP      #'3
        BCS      .get_drive_from_user
        TAX
        AND      #$03
        STA      iob.drive
        TXA
        STA      save_drive
        JSR      buffer_char
    .press_return_key_to_begin:
        JSR      dump_buffer_with_more
        STOW     sReturnToBegin, SCRATCH2
        LDX      #35
        JSR      print_ascii_string
        JSR      dump_buffer_line
        JSR      RDKEY
        CMP      #$8D
        BNE      .press_return_key_to_begin
        LDA      #$FF
        STA      SCRATCH1
        STA      SCRATCH1+1
        LDA      save_position
        AND      #$07
        BEQ      .end
        TAY
    .loop:
        ADDB     SCRATCH1, #64
        DEY
        BNE      .loop
    .end:
        JSR      dump_buffer_with_more
        RTS
    get_prompted_number_from_user:
        JSR      dump_buffer_with_more
        STOW     sSlotPrompt, SCRATCH2      ; print prompt
        ADDB     SCRATCH2, prompt_offset
        LDX      #15
        JSR      print_ascii_string
        JSR      dump_buffer_line
        LDA      #25
        STA      CH
        LDA      #$3F                       ; set inverse
        STA      INVFLG
        STOW     sDefault, SCRATCH2         ; print "DEFAULT = "
        LDX      #10
        JSR      cout_string
        STOW     save_slot, SCRATCH2        ; print default value
        ADDB     SCRATCH2, prompt_offset
        LDX      #1
        JSR      cout_string
        LDA      #$FF                       ; clear inverse
        STA      INVFLG
        JSR      RDKEY                      ; A = read key
        PHA
        LDA      #25
        STA      CH
        JSR      CLREOL                     ; clear line
        PLA
        CMP      #$8D                       ; newline?
        BNE      .end
        LDY      prompt_offset              ; store result
        LDA      save_slot,Y
    .end:
        AND      #$7F
        RTS
    sReinsertGameDiskette:
        DC      "PLEASE RE-INSERT GAME DISKETTE,"
    sPressReturnToContinue:
        DC      "--- PRESS 'RETURN' KEY TO CONTINUE ---"
    please_reinsert_game_diskette:
        LDA      iob.slot_times_16
        CMP      #$60
        BNE      .set_slot6_drive1
        LDA      iob.drive
        CMP      #$01
        BNE      .set_slot6_drive1
        JSR      dump_buffer_with_more
        STOW     sReinsertGameDiskette, SCRATCH2
        LDX      #31
        JSR      print_ascii_string
    .await_return_key:
        JSR      dump_buffer_with_more
        STOW     sPressReturnToContinue, SCRATCH2
        LDX      #38
        JSR      print_ascii_string
        JSR      dump_buffer_line
        JSR      RDKEY
        CMP      #$8D
        BNE      .await_return_key
        JSR      dump_buffer_with_more
    .set_slot6_drive1:
        STOB     #$60, iob.slot_times_16
        STOB     #$01, iob.drive
        RTS
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

static struct prompt prompts[3] = {
    { "SLOT     (1-7):", '6' },
    { "DRIVE    (1-2):", '2' },
    { "POSITION (0-7):", '0' },
};

/* Prints prompt `which` and its default in inverse, reads a key, and
 * returns it without its high bit, or the default for RETURN. */
static uint8_t get_prompted_number_from_user(register uint8_t which) {
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

/* Prompts for the save diskette's position, slot, and drive, points the
 * IOB at the drive, and waits for RETURN. Returns the sector before
 * the position's first: each position is 64 sectors. */
uint16_t please_insert_save_diskette(void) {
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
    uint16_t sector = (64 * (prompts[PROMPT_POSITION].answer & 0x07)) - 1;
    dump_buffer_with_more();
    return sector;
}

/* If the save diskette is in the game's drive (slot 6, drive 1),
 * prompts for the game diskette back and waits for RETURN. Points the IOB at
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
