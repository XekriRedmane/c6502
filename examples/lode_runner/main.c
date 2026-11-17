/*
    main.c -- starting the game, the splash screen, the wait between
    screens, and the loop that plays a game.

    The game cycles through three screens while nobody touches it. The
    splash screen waits, then the demo plays its levels, then the high
    scores show, and then the splash screen draws again. A key or a
    paddle button at any of them starts a game at level 1, Return
    shows the high scores, and Ctrl-E starts the level editor. A game
    goes on level by level until the last life is lost, and then shows
    GAME OVER and goes back to waiting.

    Departures from the original:

    - The loader's relocation code at $5F00-$5F7C (main.asm lines
      213-302), which copies the game into place from where DOS loads
      it and then jumps to MAIN, isn't translated. The linker puts this
      program where it runs.
    - MAIN's `LDX #$07; TXS` sets the 6502's stack pointer, which C
      can't do. Where the stack starts doesn't change what the game
      does. MAIN's `CLC; LDA #$01; AND #$A4; BEQ; SEC` sets a
      carry that nothing reads, and its branch goes to the next
      instruction either way, so it has no C counterpart.
    - The code at $6000, which the reset vector reaches, is cold_start.
    - The polling loop, from .poll_inputs, is poll_inputs, which
      returns what it saw in `key` instead of jumping to what acts on
      it. respond_to_input then acts on it. MAIN and LONG_DELAY each
      call both. MAIN enters the loop at CHECK_FOR_BUTTON_DOWN with
      JOYSTICK_MODE in A, and LONG_DELAY at .poll_inputs, which loads
      INPUT_MODE, and poll_inputs takes that first input mode and the
      first values of the counters in X and Y as its arguments.
    - .key_pressed reads the key from A, and key_pressed reads it from
      `key`. key_pressed is in a cycle of calls through the functions
      that never return (init_game_data calls it, and it calls
      init_game_data), and the compiler passes the arguments of a
      function in a cycle on the soft stack. For the same reason, the
      counting is done in poll_inputs, which returns and so isn't in
      the cycle, and the splash screen is drawn by draw_splash_screen,
      whose locals would otherwise be saved on the soft stack.
    - The splash screen is drawn with a local row counter where the
      original counts in GAME_ROWNUM. LONG_DELAY sets GAME_ROWNUM
      before anything reads it. HGR_PAGE, which ROW_TO_ADDR reads, is
      row_to_addr's argument.
    - INIT_GAME_DATA doesn't store RWTS_ADDR, the target of the JMP at
      $23. Every RWTS call ends at DOS's routine at $B7B5, which
      level.c calls directly.
    - The game's parts from .start_game to .game_loop, the game loop,
      and the level-cleared tally are start_level, play_level, and
      level_cleared, which init_game_data calls in a loop.
    - SCRATCH_5C, the level-cleared tally's counter, is append_level_
      cleared_note's argument.
    - GUARD_PATTERN_OFFSET + GUARD_COUNT past 8 indexes past the end of
      TIMES_3_TABLE, and GUARD_PATTERN_OFFSET past 12 past the end of
      GUARD_GOLD_TIMER_START_VALUES. The offset rises only with the
      cheat key behind INC_GUARD_PATTERN_OFFSET and when LOAD_LEVEL
      skips a level with no player. The original then reads the bytes that follow each table in its code, which
      aren't the bytes that follow them here.
    - After GAME OVER, the original's .key_pressed acts on the KBD
      value that SHOW_ANIM_LINE left in A. Here it's game_over_key,
      which wipe.c stores.

    Lode Runner, main.asm lines 303-731:

        ORG    $5F7D
    MAIN:
        LDA     ROMIN_RDROM_WRRAM2
        LDA     ROMIN_RDROM_WRRAM2
        LDA     TXTCLR
        LDA     MIXCLR
        LDA     TXTPAGE1
        LDA     HIRES

        LDX     #$07
        TXS

        CLC
        LDA     #$01
        AND     #$A4
        BEQ     .short_delay_mode
        SEC
        ; fall through to .short_delay_mode

        ORG    $5F9A

    .short_delay_mode:
        LDX     #$22            ; Number of times to check for keyboard press (34).
        LDY     #$02            ; Number of times to do X checks (2).
                                ; GAME_ROWNUM was initialized to 1, so we do 34*2*1 checks.
        LDA     KBDSTRB
        LDA     #JOYSTICK_MODE
        JMP     CHECK_FOR_BUTTON_DOWN


        ORG    $5FA6
    RELOCATE_SEGMENT_COUNT:
        HEX     04
    RELOCATE_TABLE:
        HEX     3F 47 00
        HEX     47 5F 08
        HEX     60 8E 60
        HEX     8E C0 8E
        HEX     00 00 00 00 00 00 00 00 00 00 00 00 00
        HEX     00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        HEX     00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        HEX     00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        HEX     00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

        ORG    $6000

        JSR     DETECT_LACK_OF_JOYSTICK
        LDA     #$01
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK

        ; Fallthrough to RESET_GAME

        ORG    $6008
    RESET_GAME:
        SUBROUTINE

        JSR     CLEAR_HGR1

        LDA     #$FF
        STA     .rd_table+1
        LDA     #$0E
        STA     .rd_table+2     ; RD_TABLE = 0x0EFF
        LDY     #$00
        STY     GAME_ROWNUM
        STY     GAME_MODE
        STY     DISK_LEVEL_LOC  ; GAME_ROWNUM = DISK_LEVEL_LOC = GAME_MODE = 0
        LDA     #$20
        STA     HGR_PAGE
        STA     DRAW_PAGE       ; HGR_PAGE = DRAW_PAGE = 0x20

    .draw_splash_screen_row:
        JSR     ROW_TO_ADDR     ; ROW_ADDR = ROW_TO_ADDR(Y)
        LDY     #$00

    .loop:
        INC     .rd_table+1
        BNE     .rd_table
        INC     .rd_table+2     ; RD_TABLE++

    .rd_table:
        LDA     $1A84           ; A <- *RD_TABLE ($1A84 is just a dummy value)
        BEQ     .end_of_row     ; if A == 0: break
        BPL     .is_row_offset  ; if A > 0: A -> Y, .loop
        STA     (ROW_ADDR),Y    ; *(ROW_ADDR+Y) = A

        INY                     ; Y++
        BPL     .loop           ; While Y < 0x80 (really while not 00)

    .is_row_offset:
        TAY
        BPL     .loop           ; Unconditional jump

    .end_of_row:
        INC     GAME_ROWNUM
        LDY     GAME_ROWNUM
        CPY     #192
        BCC     .draw_splash_screen_row


        STA     TXTPAGE1
        STA     HIRES
        STA     MIXCLR
        STA     TXTCLR
        JMP     LONG_DELAY

        ORG    $6056

    INIT_GAME_DATA:
        LDA     #0
        STA     SCORE
        STA     SCORE+1
        STA     SCORE+2
        STA     SCORE+3
        STA     GUARD_PATTERN_OFFSET
        STA     WIPE_MODE       ; WIPE_MODE = SCORE = $97 = 0
        STA     $53
        STA     $AB
        STA     $A8             ; $53 = $AB = $A8 = 0
        LDA     #$9B            ; 155
        STA     $A9             ; $A9 = 155
        LDA     #5
        STA     LIVES           ; LIVES = 5
        LDA     GAME_MODE
        LSR
        ; if GAME_MODE was 0 or 1 (i.e. not displaying high score screen or splash screen),
        ; play the game.
        BEQ     .put_status_and_start_game

        ; We were displaying the high score screen or splash screen
        LDA     #1
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK      ; Read hi score data
        CMP     #$00
        BNE     .set_rwts_target
        JSR     BAD_DATA_DISK
        JMP     RESET_GAME

    .set_rwts_target:
        LDA     $1FFF
        BNE     .use_dos_target
        LDA     INDIRECT_TARGET
        LDX     INDIRECT_TARGET+1
        BNE     .store_rwts_addr

    .use_dos_target:
        LDA     DISABLE_INTS_CALL_RWTS_PTR
        LDX     DISABLE_INTS_CALL_RWTS_PTR+1

    .store_rwts_addr:
        STA     RWTS_ADDR
        STX     RWTS_ADDR+1

    .put_status_and_start_game:
        JSR     PUT_STATUS
        STA     TXTPAGE1

        ORG    $609F

    .start_game:
        LDX     #$01
        JSR     LOAD_LEVEL
        LDA     #$00
        STA     KEY_COMMAND
        STA     KEY_COMMAND_LR
        LDA     GAME_MODE
        LSR
        ; if GAME_MODE was 0 or 1 (i.e. not displaying high score screen
        ; or splash screen), play the game.
        BEQ     .play_game

        ; When GAME_MODE is 2:
        JSR     WAIT_KEY
        LDA     PLAYER_COL
        STA     GAME_COLNUM
        LDA     PLAYER_ROW
        STA     GAME_ROWNUM
        LDA     #SPRITE_PLAYER
        JSR     WAIT_FOR_KEY_WITH_CURSOR_PAGE_1

    .play_game:
        LDX     #$00
        STX     DIG_DIRECTION
        STX     NOTE_INDEX

        LDA     GUARD_PATTERN_OFFSET
        CLC
        ADC     GUARD_COUNT         ; GUARD_COUNT + $97 can't be greater than 8.
        TAY
        LDX     TIMES_3_TABLE,Y     ; X = 3 * Y (goes up to Y=8)
        LDA     GUARD_PATTERNS_LIST,X
        STA     GUARD_PATTERNS
        LDA     GUARD_PATTERNS_LIST+1,X
        STA     GUARD_PATTERNS+1
        LDA     GUARD_PATTERNS_LIST+2,X
        STA     GUARD_PATTERNS+2

        LDY     GUARD_PATTERN_OFFSET
        LDA     $621D,Y
        STA     GUARD_GOLD_TIMER_START_VALUE

    .game_loop:
        JSR     MOVE_PLAYER
        LDA     ALIVE
        BEQ     .died

        JSR     PLAY_SOUND

        LDA     GOLD_COUNT
        BNE     .check_player_reached_top
        JSR     ENABLE_NEXT_LEVEL_LADDERS

    .check_player_reached_top:
        LDA     PLAYER_ROW
        BNE     .not_at_top
        LDA     PLAYER_Y_ADJ
        CMP     #$02
        BNE     .not_at_top

        ; Reached top of screen
        LDA     GOLD_COUNT
        BEQ     .level_cleared
        CMP     #$FF
        BEQ     .level_cleared      ; level cleared if GOLD_COUNT == 0 or -1.

    .not_at_top:
        JSR     HANDLE_TIMERS
        LDA     ALIVE
        BEQ     .died
        JSR     PLAY_SOUND
        JSR     MOVE_GUARDS
        LDA     ALIVE
        BEQ     .died
        BNE     .game_loop

    .level_cleared:
        INC     LEVELNUM
        INC     DISK_LEVEL_LOC
        INC     LIVES
        BNE     .lives_incremented
        DEC     LIVES               ; LIVES doesn't overflow.

    .lives_incremented:
        ; Increment score by 1500, playing an ascending tune while doing so.
        LDX     #$0F
        STX     SCRATCH_5C

    .loop2:
        LDY     #$01
        LDA     #$00
        JSR     ADD_AND_UPDATE_SCORE    ; SCORE += 100
        JSR     APPEND_LEVEL_CLEARED_NOTE
        JSR     APPEND_LEVEL_CLEARED_NOTE
        JSR     APPEND_LEVEL_CLEARED_NOTE
        DEC     SCRATCH_5C
        BNE     .loop2

    .start_game_:
        JMP     .start_game

    .died:
        DEC     LIVES
        JSR     PUT_STATUS_LIVES
        JSR     LOAD_SOUND_DATA
        HEX     02 40 02 40 03 50 03 50 04 60 04 60 05 70 05 70
        HEX     06 80 06 80 07 90 07 90 08 A0 08 A0 09 B0 09 B0
        HEX     0A C0 0A C0 0B D0 0B D0 0C E0 0C E0 0D F0 0D F0
        HEX     00

    .play_died_tune:
        JSR     PLAY_SOUND
        BCS     .play_died_tune

        LDA     GAME_MODE
        LSR
        BEQ     .restore_enable_sound     ; If GAME_MODE is 0 or 1

        LDA     LIVES
        BNE     .start_game_    ; We can still play.

        ; Game over
        JSR     RECORD_HI_SCORE_DATA_TO_DISK
        JSR     SPINNING_GAME_OVER
        BCS     .key_pressed


        ORG    $618E

    LONG_DELAY:
        JSR     WAIT_KEY
        LDX     #$FF
        LDY     #$FF
        LDA     #$03
        STA     GAME_ROWNUM

        ; fall through to .poll_inputs


        ; Game loop

        ORG    $6199

    .poll_inputs:
        LDA     INPUT_MODE

    CHECK_FOR_BUTTON_DOWN:
        CMP     #KEYBOARD_MODE
        BEQ     .poll_keyboard  ; If keyboard mode, skip check button presses.
        LDA     BUTN1
        BMI     .set_level_0_and_play_game
        LDA     BUTN0
        BMI     .set_level_0_and_play_game

        ; fall through to .poll_keyboard

        ORG    $61A9

    .poll_keyboard:
        LDA     KBD
        BMI     .key_pressed
        DEX
        BNE     .poll_inputs
        DEY
        BNE     .poll_inputs
        DEC     GAME_ROWNUM
        BNE     .poll_inputs

        ; fall through to .no_button_or_key_timeout

        ORG    $61B8

    .no_button_or_key_timeout:
        LDA     GAME_MODE
        BNE     .check_game_mode    ; If GAME_MODE != 0, .check_game_mode.

        ; When GAME_MODE = 0:
        LDX     #$01
        STX     GAME_MODE        ; Set GAME_MODE = 1
        STX     LEVELNUM
        STX     $AC
        STX     $9D                 ; LEVELNUM = $AC = $9D = 1
        LDX     ENABLE_SOUND
        STX     .restore_enable_sound+1    ; Save previous value of DNABLE_SOUND
        STA     ENABLE_SOUND
        JMP     INIT_GAME_DATA

    .restore_enable_sound:
        LDA     #$00            ; Fixed up above
        STA     ENABLE_SOUND
        LDA     KBD
        LDX     $AC
        BEQ     .key_pressed
        JMP     LONG_DELAY

        ORG    $61DE

    .check_game_mode:
        ; For game mode 1, reset and play the game.
        ; For game mode 0, display the high score screen.
        CMP     #$01
        BNE     .reset_game
        BEQ     .display_hi_score_screen        ; Unconditional jump

        ORG    $61E4

    .read_and_display_hi_score_screen:
        LDA     #$01
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK      ; read hi score table

        ; fallthrough to .display_hi_score_screen

        ORG    $61E9

    .display_hi_score_screen:
        JSR     HI_SCORE_SCREEN
        LDA     #$02
        STA     GAME_MODE            ; GAME_MODE = 2
        JMP     LONG_DELAY

        ORG    $61F3

    .reset_game:
        JMP     RESET_GAME

        ORG    $61F6

    .key_pressed:
        STA     KBDSTRB     ; Clear keyboard strobe
        CMP     #$85        ; if ctrl-E:
        BEQ     .start_level_editor
        CMP     #$8D        ; if return key:
        BEQ     .read_and_display_hi_score_screen

        ; fall through to .button_pressed

        ORG    $6201

    .set_level_0_and_play_game:
        LDX     #$00
        STX     DISK_LEVEL_LOC      ; DISK_LEVEL_LOC = 0
        INX
        STX     LEVELNUM            ; LEVELNUM = 1
        STX     $9D
        LDA     #$02
        STA     GAME_MODE
        JMP     INIT_GAME_DATA

        ORG    $6211

    .start_level_editor:
        JMP     LEVEL_EDITOR

        ORG    $6214
    TIMES_3_TABLE:
        HEX     00 03 06 09 0C 0F 12 15 18
        ORG    $621D
    GUARD_GOLD_TIMER_START_VALUES:
        HEX     26 26 2E 44 47 49 4A 4B 4C 4D 4E 4F 50
        ORG    $622A
    APPEND_LEVEL_CLEARED_NOTE:
        SUBROUTINE

        LDA     SCRATCH_5C
        ASL
        ASL
        ASL
        ASL                         ; pitch = SCRATCH_5C * 16
        LDX     #$06                ; duration
        JSR     APPEND_NOTE
        JMP     PLAY_SOUND

*/
#include "lode_runner.h"

/* Multiplies by 3, the length of an entry of GUARD_PATTERNS_LIST. */
static const uint8_t TIMES_3_TABLE[9] = {
    0x00, 0x03, 0x06, 0x09, 0x0C, 0x0F, 0x12, 0x15, 0x18,
};

/* The value a guard's gold timer starts at, by GUARD_PATTERN_OFFSET. */
static const uint8_t GUARD_GOLD_TIMER_START_VALUES[13] = {
    0x26, 0x26, 0x2E, 0x44, 0x47, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E,
    0x4F, 0x50,
};

/* The tune played when the player dies, pairs of a duration and a
 * pitch, for load_sound_data. */
static const uint8_t DIED_TUNE[] = {
    0x02, 0x40, 0x02, 0x40, 0x03, 0x50, 0x03, 0x50,
    0x04, 0x60, 0x04, 0x60, 0x05, 0x70, 0x05, 0x70,
    0x06, 0x80, 0x06, 0x80, 0x07, 0x90, 0x07, 0x90,
    0x08, 0xA0, 0x08, 0xA0, 0x09, 0xB0, 0x09, 0xB0,
    0x0A, 0xC0, 0x0A, 0xC0, 0x0B, 0xD0, 0x0B, 0xD0,
    0x0C, 0xE0, 0x0C, 0xE0, 0x0D, 0xF0, 0x0D, 0xF0,
    0x00,
};

/* ENABLE_SOUND as it was before the demo started, which the demo
 * turns off. The original keeps it in the operand of the LDA at
 * .restore_enable_sound. */
static uint8_t saved_enable_sound = 0x00;

/* What the polling loop saw: the KBD value of a key, with bit 7 set,
 * KEY_BUTTON for a paddle button, or KEY_TIMED_OUT when it gave up.
 * After the demo or GAME OVER, it's the KBD value the game read,
 * whose bit 7 may be clear. */
static uint8_t key;

enum {
    KEY_TIMED_OUT = 0,
    KEY_BUTTON = 1,
};

[[noreturn]] static void key_pressed(void);

/* Polls the paddle buttons and the keyboard until one is pressed, and
 * sets `key` to what it saw. The buttons are skipped in KEYBOARD_MODE.
 * `mode` is the input mode of the first pass, and INPUT_MODE that of
 * every later one. The passes are counted down by three counters, the
 * inner one starting at `x`, the middle one at `y`, and the outer one
 * at GAME_ROWNUM. Each counter takes 256 passes of the one inside it
 * after its first, since it goes on from 0. */
static void poll_inputs(uint8_t mode, uint8_t x, uint8_t y) {
    for (;;) {
        if (mode != KEYBOARD_MODE && ((BUTN1 & 0x80) || (BUTN0 & 0x80))) {
            key = KEY_BUTTON;
            return;
        }
        uint8_t k = KBD;
        if (k & 0x80) {
            key = k;
            return;
        }
        if (--x == 0 && --y == 0 && --GAME_ROWNUM == 0) {
            key = KEY_TIMED_OUT;
            return;
        }
        mode = INPUT_MODE;
    }
}

/* Starts a game at level 1, read from the disk. */
[[noreturn]] static void start_at_level_1(void) {
    DISK_LEVEL_LOC = 0;
    LEVELNUM = 1;
    HI_SCORE_ELIGIBLE = 1;
    GAME_MODE = GAME_MODE_PLAY_MODE;
    init_game_data();
}

/* Shows the high scores, and waits. */
[[noreturn]] static void show_hi_scores(void) {
    hi_score_screen();
    GAME_MODE = GAME_MODE_PLAY_MODE;
    long_delay();
}

/* Acts on what poll_inputs saw. A key or a button starts a game. When
 * the time runs out, the splash screen goes on to the demo, the demo
 * to the high scores, and the high scores (GAME_MODE 2) back to the
 * splash screen. */
[[noreturn]] static void respond_to_input(void) {
    if (key & 0x80)
        key_pressed();
    if (key == KEY_BUTTON)
        start_at_level_1();
    if (GAME_MODE == GAME_MODE_SPLASH_SCREEN) {
        GAME_MODE = GAME_MODE_ATTRACT_MODE;
        LEVELNUM = 1;
        DEMO_RUNNING = 1;
        HI_SCORE_ELIGIBLE = 1;
        /* The demo plays without sound. */
        saved_enable_sound = ENABLE_SOUND;
        ENABLE_SOUND = 0;
        init_game_data();
    }
    if (GAME_MODE == GAME_MODE_ATTRACT_MODE)
        show_hi_scores();
    reset_game();
}

/* Acts on the key in `key`. Ctrl-E starts the level editor, Return
 * reads and shows the high scores, and any other key starts a game. */
[[noreturn]] static void key_pressed(void) {
    KBDSTRB = key;
    if (key == 0x85)
        level_editor();
    if (key == 0x8D) {
        access_hi_score_data_from_disk(DISK_ACCESS_READ);
        show_hi_scores();
    }
    start_at_level_1();
}

/* Shows page 1 of the high-resolution graphics, and waits a short
 * time for a key or a button. The outer counter of the wait is
 * GAME_ROWNUM as the program's image holds it. */
int main(void) {
    (void)ROMIN_RDROM_WRRAM2;
    (void)ROMIN_RDROM_WRRAM2;
    (void)TXTCLR;
    (void)MIXCLR;
    (void)TXTPAGE1;
    (void)HIRES;
    (void)KBDSTRB;
    poll_inputs(JOYSTICK_MODE, 0x22, 2);
    respond_to_input();
}

/* Starts the game from the reset vector. Reads the high scores, and
 * draws the splash screen. */
[[noreturn]] void cold_start(void) {
    detect_lack_of_joystick();
    access_hi_score_data_from_disk(DISK_ACCESS_READ);
    reset_game();
}

/* Draws the splash screen on page 1 from SPLASH_SCREEN.
 *
 * Each of the screen's 192 lines is a run of bytes ending in 0. A
 * byte with bit 7 set is stored at the next place in the line, and
 * any other byte moves the place to that byte. A line also ends after
 * a store moves the place to 0x80, without reading its 0. */
static void draw_splash_screen(void) {
    const uint8_t *p = SPLASH_SCREEN;
    for (uint8_t row = 0; row < 192; row++) {
        uint8_t *line = row_to_addr(row, 0x20);
        uint8_t x = 0;
        for (uint8_t b; (b = *p++) != 0;) {
            if (b & 0x80) {
                line[x] = b;
                if (++x & 0x80)
                    break;
            } else {
                x = b;
            }
        }
    }
}

/* Draws the splash screen, shows it, and waits. */
void reset_game(void) {
    clear_hgr1();
    GAME_MODE = GAME_MODE_SPLASH_SCREEN;
    DISK_LEVEL_LOC = 0;
    DRAW_PAGE = 0x20;
    draw_splash_screen();
    TXTPAGE1 = 0;
    HIRES = 0;
    MIXCLR = 0;
    TXTCLR = 0;
    long_delay();
}

/* Waits for a key or a button for a long time, after clearing any key
 * that's waiting. */
void long_delay(void) {
    wait_key();
    GAME_ROWNUM = 3;
    poll_inputs(INPUT_MODE, 0xFF, 0xFF);
    respond_to_input();
}

/* Queues and plays a note of the level-cleared tune. The note's
 * period, `n` * 16 passes between clicks, shortens as `n` counts
 * down, so the tune rises. */
static void append_level_cleared_note(uint8_t n) {
    append_note(n << 4, 6);
    play_sound();
}

/* Loads the level, waits for a key in a game, and sets up the guards'
 * moves for the level. */
static void start_level(void) {
    load_level(true);
    KEY_COMMAND = 0;
    KEY_COMMAND_LR = 0;
    if (GAME_MODE >= GAME_MODE_PLAY_MODE) {
        /* Blink the player until a key is pressed. */
        wait_key();
        GAME_COLNUM = PLAYER_COL;
        GAME_ROWNUM = PLAYER_ROW;
        wait_for_key_with_cursor_page_1(SPRITE_PLAYER);
    }
    DIG_DIRECTION = 0;
    NOTE_INDEX = 0;
    uint8_t i = TIMES_3_TABLE[(uint8_t)(GUARD_PATTERN_OFFSET + GUARD_COUNT)];
    GUARD_PATTERNS[0] = GUARD_PATTERNS_LIST[i];
    GUARD_PATTERNS[1] = GUARD_PATTERNS_LIST[i + 1];
    GUARD_PATTERNS[2] = GUARD_PATTERNS_LIST[i + 2];
    GUARD_GOLD_TIMER_START_VALUE =
        GUARD_GOLD_TIMER_START_VALUES[GUARD_PATTERN_OFFSET];
}

/* Plays the level. Returns true when the player reaches the top row
 * after taking all the gold, and false when the player dies. */
static bool play_level(void) {
    for (;;) {
        move_player();
        if (!ALIVE)
            return false;
        play_sound();
        if (GOLD_COUNT == 0)
            enable_next_level_ladders();
        /* enable_next_level_ladders takes GOLD_COUNT to 0xFF once the
         * hidden ladders are all drawn. */
        if (PLAYER_ROW == 0 && PLAYER_Y_ADJ == 2
            && (GOLD_COUNT == 0 || GOLD_COUNT == 0xFF))
            return true;
        handle_timers();
        if (!ALIVE)
            return false;
        play_sound();
        move_guards();
        if (!ALIVE)
            return false;
    }
}

/* Goes on to the next level, gives a life up to 255, and adds 1500
 * points 100 at a time, with three notes of a rising tune for each. */
static void level_cleared(void) {
    LEVELNUM++;
    DISK_LEVEL_LOC++;
    if (LIVES != 0xFF)
        LIVES++;
    for (uint8_t n = 15; n != 0; n--) {
        add_and_update_score(0x0100);
        append_level_cleared_note(n);
        append_level_cleared_note(n);
        append_level_cleared_note(n);
    }
}

/* Starts a game or the demo with 5 lives and plays it level by level.
 * A game first checks that the disk is a data disk or the master. */
void init_game_data(void) {
    SCORE[0] = 0;
    SCORE[1] = 0;
    SCORE[2] = 0;
    SCORE[3] = 0;
    GUARD_PATTERN_OFFSET = 0;
    WIPE_MODE = 0;
    GUARD_RESURRECT_COL = 0;
    DEMO_MOVE_TURNS = 0;
    DEMO_PTR = DEMO_MOVES;
    LIVES = 5;
    if (GAME_MODE >= GAME_MODE_PLAY_MODE
        && access_hi_score_data_from_disk(DISK_ACCESS_READ) == 0) {
        bad_data_disk();
        reset_game();
    }
    put_status();
    TXTPAGE1 = 0;

    for (;;) {
        start_level();
        if (play_level()) {
            level_cleared();
            continue;
        }

        LIVES--;
        put_status_lives();
        load_sound_data(DIED_TUNE);
        while (play_sound()) {
        }

        if (GAME_MODE < GAME_MODE_PLAY_MODE) {
            /* The demo is over. A key pressed during it, which stopped
             * DEMO_RUNNING, acts as it would at the splash screen. */
            ENABLE_SOUND = saved_enable_sound;
            key = KBD;
            if (!DEMO_RUNNING)
                key_pressed();
            long_delay();
        }
        if (LIVES == 0) {
            record_hi_score_data_to_disk();
            if (spinning_game_over()) {
                key = game_over_key;
                key_pressed();
            }
            long_delay();
        }
    }
}
