/*
    input.c -- the keyboard, the joystick, and the demo's moves.

    During play, check_for_input sets KEY_COMMAND (up and down, and the
    dig keys) and KEY_COMMAND_LR (left and right) from the keyboard or
    the joystick, and in the demo from DEMO_MOVES. A control key runs
    one of the cheat or setting commands instead. Most of those go
    back to reading a key, and the cheats that end the player's life
    return.

    The joystick is read as two paddles. Each paddle has two
    thresholds, and a reading past one of them is a move. Ctrl-X and
    ctrl-Y swap a paddle's thresholds, which reverses its direction.

    Departures from the original:

    - The control keys' handlers are the cases of a switch in
      check_for_input, and VALID_CTRL_KEYS and CTRL_KEY_HANDLERS are
      its case labels. A handler that ends with JMP CHECK_FOR_INPUT
      continues check_for_input's loop, and one that ends with RTS
      returns from check_for_input. RIGHT_ARROW_HANDLER's branch to
      LEFT_ARROW_HANDLER's last JMP and CTRL_R_HANDLER's fall into
      CTRL_A_HANDLER are written out in their cases.
    - INC_GUARD_PATTERN_OFFSET isn't in CTRL_KEY_HANDLERS, and nothing
      in main.asm calls it or jumps to it, so it's left out.
    - The key or joystick reading that ends a wait is kept in
      wait_result while the cursor is redrawn, where the original
      pushes A. The waits' counts of polls, which the original keeps
      in SCRATCH_A1, are a local of poll_for_key.
    - In keyboard mode, CHECK_JOYSTICK_OR_DELAY's delay counts Y down
      from whatever its caller left. That's 0 on every call of a wait
      but the first, since the delay leaves Y at 0, and on the first
      it's whatever DRAW_SPRITE_PAGE1 or DRAW_SPRITE_PAGE2 left. Y only
      sets the delay's length, so check_joystick_or_delay always
      counts from 0, 512 passes in all.
    - DETECT_LACK_OF_JOYSTICK polls the paddles 4096 times here. The
      original's first 256 polls are counted in Y, which is whatever
      the boot code left at $6000, so its count is between 3841 and
      4096. Each poll reads PADDL[0] and PADDL[1], so with no joystick
      the number of reads can differ from the original's by up to 255
      of each.
    - wait_key, return_handler, and check_for_input write 0 to
      KBDSTRB and TXTPAGE1 where the original writes whatever is in A
      (check_for_input writes the key, as the original does). Only
      the access matters to the hardware.
    - The demo's moves index VALID_KEY_COMMANDS with each nibble of a
      move. The table has seven entries, and in the original a nibble
      above 6 would read the code after it. The demo's moves are
      assumed to use only 0-6. This wasn't checked against DEMO_MOVES.

    Lode Runner, main.asm lines 2023-2422, 4600-4729, 6721-6766, 6841-6858, 6931-6984, 7120-7221:

        ORG    $69B8
    CHECK_FOR_MODE_1_INPUT:
        SUBROUTINE

        LDA     KBD
        BMI     .key_pressed

        LDA     INPUT_MODE
        CMP     #KEYBOARD_MODE
        BEQ     .nothing_pressed

        ; Check joystick buttons also
        LDA     BUTN1
        BMI     .key_pressed
        LDA     BUTN0
        BPL     .nothing_pressed

    .key_pressed:
        ; Simulate killing the attact-mode player.
        LSR     $AC
        LSR     ALIVE
        LDA     #$01
        STA     LIVES
        RTS

    .nothing_pressed:
        LDA     $AB
        BNE     .sim_keypress

        LDY     #$00
        LDA     ($A8),Y
        STA     $AA
        INY
        LDA     ($A8),Y
        STA     $AB

        ; {$A8,$A9} += 2
        LDA     $A8
        CLC
        ADC     #$02
        STA     $A8
        LDA     $A9
        ADC     #$00
        STA     $A9

    .sim_keypress:
        LDA     $AA
        AND     #$0F
        TAX
        LDA     VALID_KEY_COMMANDS,X
        STA     KEY_COMMAND
        LDA     $AA
        LSR
        LSR
        LSR
        LSR
        TAX
        LDA     VALID_KEY_COMMANDS,X
        STA     KEY_COMMAND_LR
        DEC     $AB
        RTS


        ; Guard AI routines

        ORG    $6A0B
    VALID_KEY_COMMANDS:
        HEX     C9      ; 'I'
        HEX     CA      ; 'J'
        HEX     CB      ; 'K'
        HEX     CC      ; 'L'
        HEX     CF      ; 'O'
        HEX     D5      ; 'U'
        HEX     A0      ; space
        ORG    $6A12
    CHECK_FOR_INPUT:
        SUBROUTINE

        LDA     GAME_MODE
        CMP     #$01
        BEQ     CHECK_FOR_MODE_1_INPUT

        LDX     KBD
        STX     KBDSTRB
        STX     SPRITE_NUM
        BMI     .key_pressed

        LDA     INPUT_MODE
        CMP     #KEYBOARD_MODE
        BEQ     .end                    ; If keyboard mode, end.

    .check_buttons_:
        JMP     READ_JOYSTICK_FOR_COMMAND

    .key_pressed:
        CPX     #$A0
        BCS     .non_ctrl_key_pressed
        ; ctrl key pressed
        STX     SPRITE_NUM
        LDY     #$FF

    .loop:
        INY
        LDA     VALID_CTRL_KEYS,Y
        BEQ     .non_ctrl_key_pressed

        CMP     SPRITE_NUM
        BNE     .loop

        TYA
        ASL
        TAY
        LDA     CTRL_KEY_HANDLERS+1,Y
        PHA
        LDA     CTRL_KEY_HANDLERS,Y
        PHA
        RTS                             ; JSR to CTRL_KEY_HANDLERS[Y], then return.

    .non_ctrl_key_pressed:
        LDA     INPUT_MODE
        CMP     #JOYSTICK_MODE
        BEQ     .check_buttons_         ; If joystick mode, check buttons.

        LDX     SPRITE_NUM
        STX     KEY_COMMAND
        STX     KEY_COMMAND_LR

    .end:
        RTS

        ORG    $6A56
    CTRL_CARET_HANDLER:
        SUBROUTINE

        INC     LIVES
        INC     LEVELNUM
        INC     DISK_LEVEL_LOC
        LSR     ALIVE       ; set player dead
        LSR     $9D
        RTS
        ORG    $6A61
    CTRL_AT_HANDLER:
        SUBROUTINE

        INC     LIVES
        BNE     .have_lives
        DEC     LIVES           ; LIVES = 255
    .have_lives:
        JSR     PUT_STATUS_LIVES
        LSR     $9D
        JMP     CHECK_FOR_INPUT
        ORG    $6A6F
    INC_GUARD_PATTERN_OFFSET:
        SUBROUTINE

        INC     GUARD_PATTERN_OFFSET
        INC     LIVES
        LSR     ALIVE
        RTS
        ORG    $6A76
    ESC_HANDLER:
        SUBROUTINE

        JSR     WAIT_KEY_QUEUED
        CMP     #$9B            ; key pressed is ESC?
        BNE     ESC_HANDLER
        JMP     CHECK_FOR_INPUT
        ORG    $6A80
    CTRL_R_HANDLER:
        SUBROUTINE

        LDA     #$01
        STA     LIVES

    CTRL_A_HANDLER:
        LSR     ALIVE           ; Set player to dead
        RTS
        ORG    $6A87
    CTRL_S_HANDLER:
        SUBROUTINE

        LDA     ENABLE_SOUND
        EOR     #$FF
        STA     ENABLE_SOUND
        JMP     CHECK_FOR_INPUT
        ORG    $6A90
    DOWN_ARROW_HANDLER:
        SUBROUTINE

        LDA     #JOYSTICK_MODE
        STA     INPUT_MODE
        JMP     CHECK_FOR_INPUT

        ORG    $6A97
    UP_ARROW_HANDLER:
        SUBROUTINE

        LDA     #KEYBOARD_MODE
        STA     INPUT_MODE
        JMP     CHECK_FOR_INPUT
        ORG    $6A9E
    CTRL_X_HANDLER:
        SUBROUTINE

        LDA     PADDLE0_THRESH1
        LDX     PADDLE0_THRESH2
        STA     PADDLE0_THRESH2
        STX     PADDLE0_THRESH1
        JMP     CHECK_FOR_INPUT

        ORG    $6AAD
    CTRL_Y_HANDLER:
        SUBROUTINE

        LDA     PADDLE1_THRESH1
        LDX     PADDLE1_THRESH2
        STA     PADDLE1_THRESH2
        STX     PADDLE1_THRESH1
        JMP     CHECK_FOR_INPUT

        ORG    $6ABC
    RIGHT_ARROW_HANDLER:
        SUBROUTINE

        LDA     FRAME_PERIOD
        BEQ     LEFT_ARROW_HANDLER_end
        DEC     FRAME_PERIOD
        JMP     CHECK_FOR_INPUT

        ORG    $6AC5
    LEFT_ARROW_HANDLER:
        SUBROUTINE

        LDA     FRAME_PERIOD
        CMP     #$0F
        BEQ     LEFT_ARROW_HANDLER_end
        INC     FRAME_PERIOD

    LEFT_ARROW_HANDLER_end:
        JMP     CHECK_FOR_INPUT

        ORG    $6AD0
    READ_JOYSTICK_FOR_COMMAND:
        SUBROUTINE

        LDA     BUTN1
        BPL     .check_butn0
        LDA     #$D5                  ; 'U' (dig)
        BNE     .store_key_command    ; unconditional

    .check_butn0:
        LDA     BUTN0
        BPL     .read_paddles
        LDA     #$CF                  ; 'O' (dig)

    .store_key_command
        STA     KEY_COMMAND
        STA     KEY_COMMAND_LR
        RTS

    .read_paddles:
        JSR     READ_PADDLES
        LDY     PADDLE0_VALUE

        LDA     PADDLE0_THRESH2
        CMP     #$2E
        BEQ     .6afa
        ; PADDLE0_THRESH2 != 46

        CPY     PADDLE0_THRESH2
        BCS     .6b03
        ; PADDLE0_VALUE < PADDLE0_THRESH2

        LDA     #$CC                  ; 'L' (right)
        BNE     .check_paddle_1       ; unconditional

    .6afa:
        CPY     PADDLE0_THRESH2
        BCC     .6b03
        ; PADDLE0_VALUE >= PADDLE0_THRESH2

        LDA     #$CC                  ; 'L' (right)
        BNE     .check_paddle_1       ; unconditional

    .6b03:
        LDA     PADDLE0_THRESH1
        CMP     #$2E
        BEQ     .6b13
        ; PADDLE0_THRESH1 != 46

        CPY     PADDLE0_THRESH1
        BCS     .6b1c
        ; PADDLE0_VALUE < PADDLE0_THRESH1
        LDA     #$CA                  ; 'J' (left)
        BNE     .check_paddle_1       ; unconditional

    .6b13:
        CPY     PADDLE0_THRESH1
        BCC     .6b1c
        ; PADDLE0_VALUE >= PADDLE0_THRESH1
        LDA     #$CA                  ; 'J' (left)
        BNE     .check_paddle_1       ; unconditional

    .6b1c:
        LDA     #$C0                   ; '@'

    .check_paddle_1:
        STA     KEY_COMMAND_LR

        LDY     PADDLE1_VALUE

        LDA     PADDLE1_THRESH1
        CMP     #$2E
        BEQ     .6b32
        ; PADDLE1_THRESH1 != 46

        CPY     PADDLE1_THRESH1
        BCS     .6b3b
        ; PADDLE1_VALUE >= PADDLE1_THRESH1

        LDA     #$C9                         ; 'I' (up)
        BNE     .store_key_command_end       ; unconditional

    .6b32:
        CPY     PADDLE1_THRESH1
        BCC     .6b3b
        ; PADDLE1_VALUE < PADDLE1_THRESH1

        LDA     #$C9                         ; 'I' (up)
        BNE     .store_key_command_end       ; unconditional

    .6b3b:
        LDA     PADDLE1_THRESH2
        CMP     #$2E
        BEQ     .6b4b
        ; PADDLE1_THRESH2 != 46

        CPY     PADDLE1_THRESH2
        BCS     .6b54
        ; PADDLE1_VALUE >= PADDLE1_THRESH2

        LDA     #$CB                         ; 'K' (down)
        BNE     .store_key_command_end       ; unconditional

    .6b4b:
        CPY     PADDLE1_THRESH2
        BCC     .6b54
        ; PADDLE1_VALUE < PADDLE1_THRESH2

        LDA     #$CB                         ; 'K' (down)
        BNE     .store_key_command_end       ; unconditional

    .6b54:
        LDA     #$C0                         ; '@'

    .store_key_command_end:
        STA     KEY_COMMAND
        RTS


        ORG    $6B59
    VALID_CTRL_KEYS:
        ; ctrl-
        ; ^ @ [ R A S J K H U X Y M
        ; Esc:         ctrl-[
        ; Down arrow:  ctrl-J
        ; Up arrow:    ctrl-K
        ; Right arrow: ctrl-U
        ; Left arrow:  ctrl-H
        ; Return:      ctrl-M
        HEX     9E 80 9B 92 81 93 8A 8B 88 95 98 99 8D 00

        ORG    $6B67
    CTRL_KEY_HANDLERS:
        ; These get pushed onto the stack, then an RTS is issued.
        ; Remember that the 6502's return stack contains the address
        ; to return to *minus 1*, so these values are actually one less
        ; than the function to jump to.
        WORD    CTRL_CARET_HANDLER-1
        WORD    CTRL_AT_HANDLER-1
        WORD    ESC_HANDLER-1
        WORD    CTRL_R_HANDLER-1
        WORD    CTRL_A_HANDLER-1
        WORD    CTRL_S_HANDLER-1
        WORD    DOWN_ARROW_HANDLER-1
        WORD    UP_ARROW_HANDLER-1
        WORD    LEFT_ARROW_HANDLER-1
        WORD    RIGHT_ARROW_HANDLER-1
        WORD    CTRL_X_HANDLER-1
        WORD    CTRL_Y_HANDLER-1
        WORD    RETURN_HANDLER-1
        ORG    $6B81
    PADDLE0_THRESH1:
        HEX     12
    PADDLE0_THRESH2:
        HEX     2E
    PADDLE1_THRESH1:
        HEX     12
    PADDLE1_THRESH2:
        HEX     2E
    ...

        ORG    $77AC
    RETURN_HANDLER:
        SUBROUTINE

        JSR     HI_SCORE_SCREEN     ; show high score screen
        LDX     #$FF
        LDY     #$FF
        LDA     #$04
        STA     SCRATCH_A1          ; loop 256x256x4 times

    .loop:
        LDA     INPUT_MODE
        CMP     #KEYBOARD_MODE                ; Keyboard mode
        BEQ     .check_keyboard

        LDA     BUTN1
        BMI     .button_pressed
        LDA     BUTN0
        BMI     .button_pressed

    .check_keyboard:
        LDA     KBD
        BMI     .button_pressed

        DEX
        BNE     .loop
        DEY
        BNE     .loop
        DEC     SCRATCH_A1
        BNE     .loop

    .button_pressed:
        STA     KBDSTRB
        STA     TXTPAGE1
        JSR     CLEAR_HGR2
        LDY     #MAX_GAME_ROW
        STY     GAME_ROWNUM

    .loop2:
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     #MAX_GAME_COL
        STY     GAME_COLNUM

    .loop3:
        LDA     (PTR2),Y
        CMP     #SPRITE_TRAP
        BNE     .draw_sprite
        LDA     #SPRITE_BRICK
    .draw_sprite:
        JSR     DRAW_SPRITE_PAGE2

        DEC     GAME_COLNUM
        LDY     GAME_COLNUM
        BPL     .loop3

        DEC     GAME_ROWNUM
        LDY     GAME_ROWNUM
        BPL     .loop2

        LDX     #$1E
    .loop4:
        STX     TMP_LOOP_CTR
        LDA     BRICK_FILL_TIMERS,X
        BEQ     .next4

        LDY     BRICK_DIG_ROWS,X
        STY     GAME_ROWNUM
        LDY     BRICK_DIG_COLS,X
        STY     GAME_COLNUM
        CMP     #$15
        BCC     .check_b

        LDA     #SPRITE_EMPTY
        JSR     DRAW_SPRITE_PAGE2
        JMP     .next4

    .check_b:
        CMP     #$0B
        BCC     .draw_sprite_56
        LDA     #$37
        JSR     DRAW_SPRITE_PAGE2
        JMP     .next4

    .draw_sprite_56:
        LDA     #$38
        JSR     DRAW_SPRITE_PAGE2

    .next4:
        LDX     TMP_LOOP_CTR
        DEX
        BPL     .loop4

        LDX     GUARD_COUNT
        BEQ     .check_for_input

    .loop5:
        LDA     GUARD_RESURRECTION_TIMERS,X
        STX     TMP_LOOP_CTR
        BEQ     .next5

        LDY     GUARD_LOCS_COL,X
        STY     GAME_COLNUM
        LDY     GUARD_LOCS_ROW,X
        STY     GAME_ROWNUM
        CMP     #$14
        BCS     .next5

        CMP     #$0B
        BCC     .draw_sprite_58
        LDA     #$39                ; sprite 57
        BNE     .draw_sprite2       ; unconditional

    .draw_sprite_58:
        LDA     #$3A

    .draw_sprite2:
        JSR     DRAW_SPRITE_PAGE2

    .next5:
        LDX     TMP_LOOP_CTR
        DEX
        BNE     .loop5

    .check_for_input:
        JMP     CHECK_FOR_INPUT

    ...

        ORG    $85F3
    WAIT_FOR_KEY:
        SUBROUTINE
        ; Enter routine with A set to cursor sprite. If zero, sprite 10 (all white)
        ; will be used.

        STA     CURSOR_SPRITE

    .loop:
        LDA     #$68
        STA     SCRATCH_A1
        LDA     CURSOR_SPRITE
        BNE     .draw_sprite
        LDA     #SPRITE_ALLWHITE
    .draw_sprite:
        JSR     DRAW_SPRITE_PAGE2

    .loop2:
        LDA     KBD
        BMI     .end            ; on keypress, end

        JSR     CHECK_JOYSTICK_OR_DELAY
        DEC     SCRATCH_A1
        BNE     .loop2

        ; Draw a blank
        LDA     #$00
        JSR     DRAW_SPRITE_PAGE2
        LDA     #$68
        STA     SCRATCH_A1

    .loop3:
        LDA     KBD
        BMI     .end
        JSR     CHECK_JOYSTICK_OR_DELAY
        DEC     SCRATCH_A1
        BNE     .loop3
        JMP     .loop

    .end:
        PHA
        LDA     CURSOR_SPRITE
        JSR     DRAW_SPRITE_PAGE2
        PLA
        RTS

    ...

        ORG    $869F
    WAIT_KEY:
        SUBROUTINE

        STA     KBDSTRB
        LDA     KBD
        BMI     WAIT_KEY
        RTS

        ORG    $86A8
    WAIT_KEY_QUEUED:
        SUBROUTINE

        LDA     KBD
        BPL     WAIT_KEY_QUEUED
        STA     KBDSTRB
        RTS

    ...

        ORG    $8700
    WAIT_FOR_KEY_WITH_CURSOR_PAGE_1:
        SUBROUTINE
        ; Enter routine with A set to cursor sprite. If zero, sprite 10 (all white)
        ; will be used.

        STA     CURSOR_SPRITE

    .loop:
        LDA     #$68
        STA     SCRATCH_A1
        LDA     #$00
        LDX     CURSOR_SPRITE
        BNE     .draw_sprite
        LDA     #SPRITE_ALLWHITE
    .draw_sprite:
        JSR     DRAW_SPRITE_PAGE1

    .loop2:
        LDA     KBD
        BMI     .end            ; on keypress, end

        JSR     CHECK_JOYSTICK_OR_DELAY
        BCS     .end

        DEC     SCRATCH_A1
        BNE     .loop2

        LDA     CURSOR_SPRITE
        JSR     DRAW_SPRITE_PAGE1
        LDA     #$68
        STA     SCRATCH_A1

    .loop3:
        LDA     KBD
        BMI     .end

        JSR     CHECK_JOYSTICK_OR_DELAY
        BCS     .end

        DEC     SCRATCH_A1
        BNE     .loop3
        JMP     .loop

    .end:
        PHA
        LDA     CURSOR_SPRITE
        JSR     DRAW_SPRITE_PAGE1
        PLA
        RTS

        ORG    $8745
    CURSOR_SPRITE:
        HEX     06
    ...

        ORG    $8746
    READ_PADDLES:
        SUBROUTINE

        LDA     #$00
        STA     PADDLE0_VALUE
        STA     PADDLE1_VALUE       ; Zero out values
        LDA     PTRIG

    .loop:
        LDX     #$01                ; Start with paddle 1

    .check_paddle:
        LDA     PADDL0,X
        BPL     .threshold_reached
        INC     PADDLE0_VALUE,X
    .check_next_paddle
        DEX
        BPL     .check_paddle

        ; Checked both paddles
        LDA     PADDL0
        ORA     PADDL1
        BPL     .end                ; Both paddles triggered, then end.
        LDA     PADDLE0_VALUE
        ORA     PADDLE1_VALUE
        BPL     .loop               ; Unconditional

    .threshold_reached:
        NOP
        BPL     .check_next_paddle      ; Unconditional

    .end:
        RTS

        ORG    $876D
    CHECK_JOYSTICK_OR_DELAY:
        SUBROUTINE

        LDA     INPUT_MODE
        CMP     #KEYBOARD_MODE
        BEQ     .delay_and_return       ; Keyboard mode, so just delay and return

        JSR     READ_PADDLES

        LDA     PADDLE0_VALUE
        CMP     #$12
        BCC     .have_joystick_input           ; PADDLE0_VALUE < 0x12
        CMP     #$3B
        BCS     .have_joystick_input           ; PADDLE0_VALUE >= 0x3B

        LDA     PADDLE1_VALUE
        CMP     #$12
        BCC     .have_joystick_input
        CMP     #$3B
        BCS     .have_joystick_input

        LDA     BUTN1
        BMI     .have_joystick_input
        LDA     BUTN0
        BMI     .have_joystick_input

        CLC
        RTS

    .have_joystick_input:
        SEC
        RTS

    .delay_and_return:
        LDX     #$02
    .loop:
        DEY
        BNE     .loop
        DEX
        BNE     .loop
        CLC
        RTS

        ORG    $87A2
    DETECT_LACK_OF_JOYSTICK:
        SUBROUTINE

        LDA     PTRIG
        LDX     #$10

    .loop:
        LDA     PADDL0
        ORA     PADDL1
        BPL     .return

        DEY
        BNE     .loop
        DEX
        BNE     .loop

        LDA     #KEYBOARD_MODE
        STA     INPUT_MODE

    .return:
        RTS
*/
#include "lode_runner.h"

static void return_handler(void);

/* The key commands a demo move's nibbles stand for: 'I', 'J', 'K',
 * 'L', 'O', 'U', and a space for no move, with bit 7 set. */
static const uint8_t VALID_KEY_COMMANDS[7] = {
    0xC9, 0xCA, 0xCB, 0xCC, 0xCF, 0xD5, 0xA0,
};

/* Each paddle's two thresholds, THRESH1 then THRESH2. A reading
 * passes a threshold of 0x2E by reaching it, and any other threshold
 * by going below it (see past_threshold). Ctrl-X and ctrl-Y swap a
 * pair. */
static uint8_t paddle_thresholds[2][2] = {
    { 0x12, 0x2E },
    { 0x12, 0x2E },
};

/* The cursor sprite of the last wait_for_key or
 * wait_for_key_with_cursor_page_1, 0 for SPRITE_ALLWHITE. */
static uint8_t cursor_sprite = 0x06;

/* The key, or the joystick reading, that ended the last wait. */
static uint8_t wait_result;

/* Returns true while either paddle's timer is still running. PADDL[0]
 * is read first, as in the original. */
static bool paddle_timer_running(void) {
    uint8_t timers = PADDL[0];
    timers |= PADDL[1];
    return timers & 0x80;
}

/* Counts the passes until each paddle's timer runs out into
 * PADDLE_VALUES. Counting stops when both timers have run out, or
 * when either count reaches 128. */
static void read_paddles(void) {
    PADDLE_VALUES[0] = 0;
    PADDLE_VALUES[1] = 0;
    (void)PTRIG;
    do {
        if (PADDL[1] & 0x80)
            PADDLE_VALUES[1]++;
        if (PADDL[0] & 0x80)
            PADDLE_VALUES[0]++;
        if (!paddle_timer_running())
            return;
    } while (!((PADDLE_VALUES[0] | PADDLE_VALUES[1]) & 0x80));
}

/* Returns true when a paddle reading is past a threshold. The
 * threshold 0x2E is the high end of the paddle's range, so a reading
 * at or above it is past it. Any other threshold is the low end. */
static bool past_threshold(uint8_t reading, uint8_t threshold) {
    if (threshold == 0x2E)
        return reading >= threshold;
    return reading < threshold;
}

/* Returns true when a joystick button is down in joystick mode. */
static bool button_pressed(void) {
    return INPUT_MODE != KEYBOARD_MODE && ((BUTN1 & 0x80) || (BUTN0 & 0x80));
}

/* Sets the key commands from the joystick. A button digs, and
 * otherwise paddle 0 moves left or right and paddle 1 up or down. '@'
 * is no move. */
static void read_joystick_for_command(void) {
    uint8_t dig;
    if (BUTN1 & 0x80) {
        dig = 0xD5;     /* 'U' */
    } else if (BUTN0 & 0x80) {
        dig = 0xCF;     /* 'O' */
    } else {
        read_paddles();
        uint8_t x = PADDLE_VALUES[0];
        if (past_threshold(x, paddle_thresholds[0][1]))
            KEY_COMMAND_LR = 0xCC;      /* 'L' */
        else if (past_threshold(x, paddle_thresholds[0][0]))
            KEY_COMMAND_LR = 0xCA;      /* 'J' */
        else
            KEY_COMMAND_LR = 0xC0;      /* '@' */

        uint8_t y = PADDLE_VALUES[1];
        if (past_threshold(y, paddle_thresholds[1][0]))
            KEY_COMMAND = 0xC9;         /* 'I' */
        else if (past_threshold(y, paddle_thresholds[1][1]))
            KEY_COMMAND = 0xCB;         /* 'K' */
        else
            KEY_COMMAND = 0xC0;         /* '@' */
        return;
    }
    KEY_COMMAND = dig;
    KEY_COMMAND_LR = dig;
}

/* Ends the demo if a key or button is pressed, by killing the demo's
 * player on its last life. Otherwise sets the key commands from the
 * demo's current move, and steps to the next move when the current
 * one's turns are used up. A move is two bytes, the move and its
 * number of turns. The move's low nibble chooses KEY_COMMAND and its
 * high nibble KEY_COMMAND_LR, from VALID_KEY_COMMANDS. */
static void check_for_mode_1_input(void) {
    if ((KBD & 0x80) || button_pressed()) {
        DEMO_RUNNING >>= 1;
        ALIVE >>= 1;
        LIVES = 1;
        return;
    }
    if (DEMO_MOVE_TURNS == 0) {
        DEMO_MOVE = DEMO_PTR[0];
        DEMO_MOVE_TURNS = DEMO_PTR[1];
        DEMO_PTR += 2;
    }
    KEY_COMMAND = VALID_KEY_COMMANDS[DEMO_MOVE & 0x0F];
    KEY_COMMAND_LR = VALID_KEY_COMMANDS[DEMO_MOVE >> 4];
    DEMO_MOVE_TURNS--;
}

/* Waits for a key and clears the keyboard strobe. Returns the key,
 * with bit 7 set. */
static uint8_t wait_key_queued(void) {
    uint8_t key;
    do {
        key = KBD;
    } while (!(key & 0x80));
    KBDSTRB = key;
    return key;
}

void check_for_input(void) {
    for (;;) {
        if (GAME_MODE == GAME_MODE_ATTRACT_MODE) {
            check_for_mode_1_input();
            return;
        }

        uint8_t key = KBD;
        KBDSTRB = key;
        if (!(key & 0x80)) {
            if (INPUT_MODE != KEYBOARD_MODE)
                read_joystick_for_command();
            return;
        }

        switch (key) {
        case 0x9E:      /* ctrl-^: skip to the next level */
            LIVES++;
            LEVELNUM++;
            DISK_LEVEL_LOC++;
            ALIVE >>= 1;
            HI_SCORE_ELIGIBLE >>= 1;
            return;
        case 0x80:      /* ctrl-@: one more life, up to 255 */
            if (LIVES != 0xFF)
                LIVES++;
            put_status_lives();
            HI_SCORE_ELIGIBLE >>= 1;
            continue;
        case 0x9B:      /* ESC: pause until ESC is pressed again */
            while (wait_key_queued() != 0x9B) {
            }
            continue;
        case 0x92:      /* ctrl-R: end the game */
            LIVES = 1;
            ALIVE >>= 1;
            return;
        case 0x81:      /* ctrl-A: lose a life */
            ALIVE >>= 1;
            return;
        case 0x93:      /* ctrl-S: sound on or off */
            ENABLE_SOUND ^= 0xFF;
            continue;
        case 0x8A:      /* down arrow, ctrl-J */
            INPUT_MODE = JOYSTICK_MODE;
            continue;
        case 0x8B:      /* up arrow, ctrl-K */
            INPUT_MODE = KEYBOARD_MODE;
            continue;
        case 0x88:      /* left arrow, ctrl-H: slower */
            if (FRAME_PERIOD != 0x0F)
                FRAME_PERIOD++;
            continue;
        case 0x95:      /* right arrow, ctrl-U: faster */
            if (FRAME_PERIOD != 0)
                FRAME_PERIOD--;
            continue;
        case 0x98: {    /* ctrl-X: reverse paddle 0 */
            uint8_t t = paddle_thresholds[0][0];
            paddle_thresholds[0][0] = paddle_thresholds[0][1];
            paddle_thresholds[0][1] = t;
            continue;
        }
        case 0x99: {    /* ctrl-Y: reverse paddle 1 */
            uint8_t t = paddle_thresholds[1][0];
            paddle_thresholds[1][0] = paddle_thresholds[1][1];
            paddle_thresholds[1][1] = t;
            continue;
        }
        case 0x8D:      /* RETURN: show the high scores */
            return_handler();
            continue;
        }

        /* Any other key is a key command, but only in keyboard mode. */
        if (INPUT_MODE == JOYSTICK_MODE) {
            read_joystick_for_command();
        } else {
            KEY_COMMAND = key;
            KEY_COMMAND_LR = key;
        }
        return;
    }
}

/* Shows the high scores until a key or button is pressed, or for
 * 261,887 polls, and then draws the level's background on page 2 as
 * it now is, with the holes and the guards being reborn, and shows
 * page 1. */
static void return_handler(void) {
    hi_score_screen();

    /* The original counts the polls in X, Y, and SCRATCH_A1. */
    uint8_t x = 0xFF, y = 0xFF, z = 4;
    do {
        if (button_pressed() || (KBD & 0x80))
            break;
    } while (--x || --y || --z);

    KBDSTRB = 0;
    TXTPAGE1 = 0;
    clear_hgr2();

    /* The cells are drawn from the last to the first, so `cell` steps
     * back through LEVEL_BG one cell at a time. A trap is drawn as the
     * brick it looks like. */
    const uint8_t *cell = &LEVEL_BG[MAX_GAME_ROW][MAX_GAME_COL];
    for (GAME_ROWNUM = MAX_GAME_ROW; (int8_t)GAME_ROWNUM >= 0; GAME_ROWNUM--) {
        for (GAME_COLNUM = MAX_GAME_COL; (int8_t)GAME_COLNUM >= 0; GAME_COLNUM--) {
            uint8_t sprite = *cell--;
            if (sprite == SPRITE_TRAP)
                sprite = SPRITE_BRICK;
            draw_sprite_page2(sprite);
        }
    }

    for (uint8_t i = 0x1E; (int8_t)i >= 0; i--) {
        uint8_t timer = BRICK_FILL_TIMERS[i];
        if (timer == 0)
            continue;
        GAME_ROWNUM = BRICK_DIG_ROWS[i];
        GAME_COLNUM = BRICK_DIG_COLS[i];
        if (timer >= 0x15)
            draw_sprite_page2(SPRITE_EMPTY);
        else if (timer >= 0x0B)
            draw_sprite_page2(SPRITE_BRICK_FILL0);
        else
            draw_sprite_page2(SPRITE_BRICK_FILL1);
    }

    /* A guard's cell is set even when its timer is too high to draw
     * it, as in the original. */
    for (uint8_t g = GUARD_COUNT; g != 0; g--) {
        uint8_t timer = GUARD_RESURRECTION_TIMERS[g];
        if (timer == 0)
            continue;
        GAME_COLNUM = GUARD_LOCS_COL[g];
        GAME_ROWNUM = GUARD_LOCS_ROW[g];
        if (timer >= 0x14)
            continue;
        draw_sprite_page2(timer >= 0x0B ? SPRITE_GUARD_EGG0 : SPRITE_GUARD_EGG1);
    }
}

/* In joystick mode, reads the joystick and returns true if it's in
 * use, a paddle reading below 0x12 or at or above 0x3B or a button
 * down. It leaves the reading or the button's value in wait_result.
 * In keyboard mode, waits 512 passes and returns false. */
static bool check_joystick_or_delay(void) {
    if (INPUT_MODE == KEYBOARD_MODE) {
        for (uint8_t x = 2; x != 0; x--) {
            [[c6502::keep]] for (uint8_t y = 0; --y;) {
            }
        }
        return false;
    }

    read_paddles();
    for (uint8_t p = 0; p < 2; p++) {
        uint8_t reading = PADDLE_VALUES[p];
        if (reading < 0x12 || reading >= 0x3B) {
            wait_result = reading;
            return true;
        }
    }
    uint8_t button = BUTN1;
    if (!(button & 0x80))
        button = BUTN0;
    if (button & 0x80) {
        wait_result = button;
        return true;
    }
    return false;
}

/* Polls the keyboard 0x68 times, with check_joystick_or_delay between
 * polls. Returns true when a key is pressed, leaving it in
 * wait_result, or when `joystick_ends` is true and the joystick is in
 * use. */
static bool poll_for_key(bool joystick_ends) {
    for (uint8_t n = 0x68; n != 0; n--) {
        uint8_t key = KBD;
        if (key & 0x80) {
            wait_result = key;
            return true;
        }
        if (check_joystick_or_delay() && joystick_ends)
            return true;
    }
    return false;
}

/* Blinks between the cursor and SPRITE_EMPTY. In joystick mode the
 * joystick is read between polls of the keyboard, but only a key ends
 * this wait. */
uint8_t wait_for_key(uint8_t cursor) {
    cursor_sprite = cursor;
    do {
        draw_sprite_page2(cursor_sprite ? cursor_sprite : SPRITE_ALLWHITE);
        if (poll_for_key(false))
            break;
        draw_sprite_page2(SPRITE_EMPTY);
    } while (!poll_for_key(false));
    draw_sprite_page2(cursor_sprite);
    return wait_result;
}

/* Blinks between SPRITE_EMPTY and the cursor, or between
 * SPRITE_ALLWHITE and SPRITE_EMPTY for a cursor of 0. The joystick
 * also ends this wait, and then the returned "key" is the paddle
 * reading or the button's value that showed the joystick in use. */
uint8_t wait_for_key_with_cursor_page_1(uint8_t cursor) {
    cursor_sprite = cursor;
    do {
        draw_sprite_page1(cursor_sprite ? SPRITE_EMPTY : SPRITE_ALLWHITE);
        if (poll_for_key(true))
            break;
        draw_sprite_page1(cursor_sprite);
    } while (!poll_for_key(true));
    draw_sprite_page1(cursor_sprite);
    return wait_result;
}

void wait_key(void) {
    do {
        KBDSTRB = 0;
    } while (KBD & 0x80);
}

/* Switches to keyboard mode if the paddles' timers are still running
 * after 4096 polls, which is what an unplugged joystick does. */
void detect_lack_of_joystick(void) {
    (void)PTRIG;
    for (uint16_t n = 0x1000; n != 0; n--) {
        if (!paddle_timer_running())
            return;
    }
    INPUT_MODE = KEYBOARD_MODE;
}
