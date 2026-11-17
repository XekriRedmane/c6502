/*
    wipe.c -- the iris wipe between levels, and GAME OVER turning.

    The iris wipe closes the play area on page 1 in a shrinking circle
    and opens it again on page 2's picture in a growing one. Each
    circle is drawn with the midpoint algorithm, one step of x and y
    at a time, and each step clears (or copies from page 2) the half
    of a screen byte, 3 or 4 pixels, at each of the circle's eight
    points. The circle is centered on line 88 and pixel 140 of the
    176 lines and 280 pixels of the play area, and its radius goes
    from 170 down to 1 as it closes and from 1 up to 169 as it opens.

    SPINNING_GAME_OVER draws the 14 lines of GAME OVER turning about
    its vertical axis, a line at a time from 11 pictures of each line,
    and slows down as it turns. A key or a button stops it.

    Departures from the original:

    - The WIPE* bytes are statics of this unit or locals, and
      IRIS_WIPE_STEP's counter is a parameter. The original's center,
      WIPE_CENTER_Y and WIPE_CENTER_X, is constant, so it's
      CENTER_ROW and CENTER_X here. Each column, the D and R bytes of
      WIPE7-WIPE10, is a struct wipe_col.
    - The two columns at the center, WIPE8 and WIPE9, start at 140 / 7,
      which the compiler folds, where the original calls DIV_BY_7.
    - DIV_BY_7 divides a column from -30 to 310 by 7 with
      shift-and-subtract on the column plus 35, and takes 5 off the
      quotient, so a negative column gives floor division (a byte of
      0xFB to 0xFF and a pixel of 0 to 6). The original agrees for
      columns from -28 up. For -29 and -30, the columns of the two
      largest circles, it gives a byte of 0xFF and a pixel of 234 or
      233. Both are skipped while the byte is 40 or more, and stepping
      the column right reaches byte 0, pixel 0 at the same step from
      either, since the original's pixel counts up through 255 to 7
      before the byte goes to 0. So the same bytes are drawn.
    - SHOW_ANIM_LINE takes its 14 bytes as a pointer, and returns true
      when a key or button stopped it, where the original pulls the
      return address to find them and pulls one more to return to
      SPINNING_GAME_OVER's caller with the carry set. ANIM0-ANIM10 are
      arrays of those bytes, and SPIN_ORDER lists them in the order
      SPINNING_GAME_OVER calls them.
    - ADDRESS_TABLE, which holds each TABLEn less 13 for the index
      from 13 to 26, is left out. The C indexes ANIM_PIXELS from 0.
    - HGR_PAGE, which SPINNING_GAME_OVER sets to 0x20 for
      SHOW_ANIM_LINE's ROW_TO_ADDR, is the 0x20 that show_anim_line
      passes to row_to_addr.

    Lode Runner, main.asm lines 7440-8130:

        ORG    $88A2
    IRIS_WIPE:
        SUBROUTINE

        LDA     #88
        STA     WIPE_CENTER_Y
        LDA     #140
        STA     WIPE_CENTER_X

        LDA     WIPE_MODE
        BEQ     .iris_open

        LDX     #$AA
        STX     WIPE_COUNTER
        LDX     #$00
        STX     WIPE_DIR             ; Close

    .loop_close:
        JSR     IRIS_WIPE_STEP
        DEC     WIPE_COUNTER
        BNE     .loop_close

    .iris_open:
        LDA     #$01
        STA     WIPE_COUNTER
        STA     WIPE_MODE           ; So next time we will close.
        STA     WIPE_DIR            ; Open
        JSR     PUT_STATUS_LIVES
        JSR     PUT_STATUS_LEVEL

    .loop_open:
        JSR     IRIS_WIPE_STEP
        INC     WIPE_COUNTER
        LDA     WIPE_COUNTER
        CMP     #$AA
        BNE     .loop_open
        RTS

        ORG    $88D7
    IRIS_WIPE_STEP:
        SUBROUTINE

        LDA     WIPE_COUNTER
        STA     WIPE0
        LDA     #$00
        STA     WIPE0+1         ; WIPE0 = WIPE_COUNTER

        ; fallthrough with A = 0
        STA     WIPE1
        STA     WIPE1+1         ; WIPE1 = 0

        LDA     WIPE0
        ASL
        STA     WIPE2
        LDA     WIPE0+1
        ROL
        STA     WIPE2+1         ; WIPE2 = 2 * WIPE0

        LDA     #$03
        SEC
        SBC     WIPE2
        STA     WIPE2
        LDA     #$00
        SBC     WIPE2+1
        STA     WIPE2+1         ; WIPE2 = 3 - WIPE2


    ; WIPE3, WIPE4, WIPE5, and WIPE6 correspond to
    ; row numbers. WIPE3 is above the center, WIPE6
    ; is below the center, while WIPE4 and WIPE5 are on
    ; the center.

        LDA     WIPE_CENTER_Y
        SEC
        SBC     WIPE_COUNTER
        STA     WIPE3L
        LDA     #$00
        SBC     #$00
        STA     WIPE3H          ; WIPE3 = WIPE_CENTER_Y - WIPE_COUNTER

        LDA     WIPE_CENTER_Y
        STA     WIPE4L
        STA     WIPE5L
        LDA     #$00
        STA     WIPE4H
        STA     WIPE5H          ; WIPE4 = WIPE5 = WIPE_CENTER_Y

        LDA     WIPE_CENTER_Y
        CLC
        ADC     WIPE_COUNTER
        STA     WIPE6L
        LDA     #$00
        ADC     #$00
        STA     WIPE6H          ; WIPE6 = WIPE_CENTER_Y + WIPE_COUNTER


    ; WIPE7, WIPE8, WIPE9, and WIPE10 correspond to
    ; column byte numbers. Note the division by 7 pixels!
    ; WIPE7 is left of center, WIPE10 is right of center,
    ; while WIPE8 and WIPE9 are on the center.

        LDA     WIPE_CENTER_X
        SEC
        SBC     WIPE_COUNTER
        TAX
        LDA     #$00
        SBC     #$00
        JSR     DIV_BY_7
        STY     WIPE7D
        STA     WIPE7R          ; WIPE7 = (WIPE_CENTER_X - WIPE_COUNTER) / 7

        LDX     WIPE_CENTER_X
        LDA     #$00
        JSR     DIV_BY_7
        STY     WIPE8D
        STY     WIPE9D
        STA     WIPE8R
        STA     WIPE9R          ; WIPE8 = WIPE9 = WIPE_CENTER_X / 7

        LDA     WIPE_CENTER_X
        CLC
        ADC     WIPE_COUNTER
        TAX
        LDA     #$00
        ADC     #$00
        JSR     DIV_BY_7
        STY     WIPE10D
        STA     WIPE10R         ; WIPE10 = (WIPE_CENTER_X + WIPE_COUNTER) / 7

    .loop:

        LDA     WIPE1+1
        CMP     WIPE0+1
        BCC     .draw_wipe_step ; Effectively, if WIPE1 > WIPE0, jump to .draw_wipe_step.
        BEQ     .8969           ; Otherwise jump to .loop1, which...

    .loop1:
        LDA     WIPE1
        CMP     WIPE0
        BNE     .end
        LDA     WIPE1+1
        CMP     WIPE0+1
        BNE     .end            ; If WIPE0 != WIPE1, return.
        JMP     DRAW_WIPE_STEP

    .end:
        RTS

    .8969:
        LDA     WIPE1
        CMP     WIPE0
        BCS     .loop1          ; The other half of the comparison from .loop.

    .draw_wipe_step:


        JSR     DRAW_WIPE_STEP

        LDA     WIPE2+1
        BPL     .89a7

        LDA     WIPE1
        ASL
        STA     MATH_TMPL
        LDA     WIPE1+1
        ROL
        STA     MATH_TMPH       ; MATH_TMP = WIPE1 * 2

        LDA     MATH_TMPL
        ASL
        STA     MATH_TMPL
        LDA     MATH_TMPH
        ROL
        STA     MATH_TMPH       ; MATH_TMP *= 2

        LDA     WIPE2
        CLC
        ADC     MATH_TMPL
        STA     MATH_TMPL
        LDA     WIPE2+1
        ADC     MATH_TMPH
        STA     MATH_TMPH       ; MATH_TMP += WIPE2

        LDA     #$06
        CLC
        ADC     MATH_TMPL
        STA     WIPE2
        LDA     #$00
        ADC     MATH_TMPH
        STA     WIPE2+1        ; WIPE2 = MATH_TMP + 6

        JMP     .8a14

    .89a7:

        LDA     WIPE1
        SEC
        SBC     WIPE0
        STA     MATH_TMPL
        LDA     WIPE1+1
        SBC     WIPE0+1
        STA     MATH_TMPH       ; MATH_TMP = WIPE1 - WIPE0

        LDA     MATH_TMPL
        ASL
        STA     MATH_TMPL
        LDA     MATH_TMPH
        ROL
        STA     MATH_TMPH       ; MATH_TMP *= 2

        LDA     MATH_TMPL
        ASL
        STA     MATH_TMPL
        LDA     MATH_TMPH
        ROL
        STA     MATH_TMPH       ; MATH_TMP *= 2

        LDA     MATH_TMPL
        CLC
        ADC     #$10
        STA     MATH_TMPL
        LDA     MATH_TMPH
        ADC     #$00
        STA     MATH_TMPH       ; MATH_TMP += 16

        LDA     MATH_TMPL
        CLC
        ADC     WIPE2
        STA     WIPE2
        LDA     MATH_TMPH
        ADC     WIPE2+1
        STA     WIPE2+1        ; WIPE2 += MATH_TMP

        LDA     WIPE0
        PHP
        DEC     WIPE0
        PLP
        BNE     .b9ec
        DEC     WIPE0+1         ; WIPE0--
    .b9ec

        INC     WIPE3L
        BNE     .89f2
        INC     WIPE3H          ; WIPE3++
    .89f2

        DEC     WIPE10R
        BPL     .89fc
        LDA     #$06
        STA     WIPE10R
        DEC     WIPE10D
    .89fc

        INC     WIPE7R
        LDA     WIPE7R
        CMP     #$07
        BNE     .8a0a
        LDA     #$00
        STA     WIPE7R
        INC     WIPE7D
    .8a0a

        DEC     WIPE6L
        LDA     WIPE6L
        CMP     #$FF
        BNE     .8a14
        DEC     WIPE6H


    .8a14:

        INC     WIPE1
        BNE     .8a1a
        INC     WIPE1+1          ; WIPE1++
    .8a1a

        INC     WIPE9R
        LDA     WIPE9R
        CMP     #$07
        BNE     .8a28
        LDA     #$00
        STA     WIPE9R
        INC     WIPE9D
    .8a28

        DEC     WIPE4L
        LDA     WIPE4L
        CMP     #$FF
        BNE     .8a32
        DEC     WIPE4H
    .8a32

        INC     WIPE5L
        BNE     .8a38
        INC     WIPE5H          ; WIPE5++
    .8a38

        DEC     WIPE8R
        BPL     .8a42
        LDA     #$06
        STA     WIPE8R
        DEC     WIPE8D
    .8a42

        JMP     .loop

        ORG    $8A45
    DIV_BY_7:
        SUBROUTINE
        ; Enter routine with AX set to (unsigned) numerator.
        ; On exit, Y will contain the integer portion of AX/7,
        ; and A contains the remainder.

        STX     MATH_TMPL
        LDY     #$08
        SEC
        SBC     #$07

    .loop:
        PHP
        ROL     MATH_TMPH
        ASL     MATH_TMPL
        ROL
        PLP
        BCC     .adjust_up
        SBC     #$07
        JMP     .next

    .adjust_up
        ADC     #$07

    .next
        DEY
        BNE     .loop

        BCS     .no_adjust
        ADC     #$07
        CLC

    .no_adjust
        ROL     MATH_TMPH
        LDY     MATH_TMPH
        RTS
        ; Initialized "uninitialized" data

        ORG    $8A69
    DRAW_WIPE_STEP:
        SUBROUTINE

        LDY     WIPE6H
        BNE     .draw_north
        LDY     WIPE6L
        CPY     #176
        BCS     .draw_north        ; Skip if WIPE6 >= 176

        JSR     ROW_TO_ADDR_FOR_BOTH_PAGES

        ; East side
        LDY     WIPE9D
        CPY     #40
        BCS     .draw_south_west
        LDX     WIPE9R
        JSR     DRAW_WIPE_BLOCK

    .draw_south_west
        ; West side
        LDY     WIPE8D
        CPY     #40
        BCS     .draw_north
        LDX     WIPE8R
        JSR     DRAW_WIPE_BLOCK

    .draw_north:
        LDY     WIPE3H
        BNE     .draw_north2
        LDY     WIPE3L
        CPY     #176
        BCS     .draw_north2        ; Skip if WIPE3 >= 176

        JSR     ROW_TO_ADDR_FOR_BOTH_PAGES

        ; East side
        LDY     WIPE9D
        CPY     #40
        BCS     .draw_north_west
        LDX     WIPE9R
        JSR     DRAW_WIPE_BLOCK

    .draw_north_west
        ; West side
        LDY     WIPE8D
        CPY     #40
        BCS     .draw_north2
        LDX     WIPE8R
        JSR     DRAW_WIPE_BLOCK

    .draw_north2:
        LDY     WIPE5H
        BNE     .draw_south2
        LDY     WIPE5L
        CPY     #176
        BCS     .draw_south2        ; Skip if WIPE5 >= 176

        JSR     ROW_TO_ADDR_FOR_BOTH_PAGES

        ; East side
        LDY     WIPE10D
        CPY     #40
        BCS     .draw_north2_west
        LDX     WIPE10R
        JSR     DRAW_WIPE_BLOCK

    .draw_north2_west
        ; West side
        LDY     WIPE7D
        CPY     #40
        BCS     .draw_south2
        LDX     WIPE7R
        JSR     DRAW_WIPE_BLOCK

    .draw_south2:
        LDY     WIPE4H
        BNE     .end
        LDY     WIPE4L
        CPY     #176
        BCS     .end        ; Skip if WIPE4 >= 176

        JSR     ROW_TO_ADDR_FOR_BOTH_PAGES

        ; East side
        LDY     WIPE10D
        CPY     #40
        BCS     .draw_south2_west
        LDX     WIPE10R
        JSR     DRAW_WIPE_BLOCK

    .draw_south2_west
        ; West side
        LDY     WIPE7D
        CPY     #40
        BCS     .end
        LDX     WIPE7R
        JMP     DRAW_WIPE_BLOCK           ; tail call

    .end:
        RTS


        ORG    $8AF6
    DRAW_WIPE_BLOCK:
        SUBROUTINE
        ; Enter routine with X set to the column byte and Y set to
        ; the pixel number within that byte (0-6). ROW_ADDR and
        ; ROW_ADDR2 must contain the base row address for page 1
        ; and page 2, respectively.

        LDA     WIPE_DIR
        BNE     .open

        LDA     (ROW_ADDR),Y
        AND     WIPE_BLOCK_CLOSE_MASK,X
        STA     (ROW_ADDR),Y
        RTS

    .open:
        LDA     (ROW_ADDR2),Y
        AND     WIPE_BLOCK_OPEN_MASK,X
        ORA     (ROW_ADDR),Y
        STA     (ROW_ADDR),Y
        RTS

        ORG    $8B0C
    WIPE_BLOCK_CLOSE_MASK:
        BYTE     %11110000
        BYTE     %11110000
        BYTE     %11110000
        BYTE     %11110000
        BYTE     %10001111
        BYTE     %10001111
        BYTE     %10001111
    WIPE_BLOCK_OPEN_MASK:
        BYTE     %10001111
        BYTE     %10001111
        BYTE     %10001111
        BYTE     %10001111
        BYTE     %11110000
        BYTE     %11110000
        BYTE     %11110000
        ORG    $8B1A
    SPINNING_GAME_OVER:
        SUBROUTINE

        LDA     #$01
        STA     ANIM_COUNT
        LDA     #$20
        STA     HGR_PAGE

    .loop:
        JSR     ANIM5
        JSR     ANIM4
        JSR     ANIM3
        JSR     ANIM2
        JSR     ANIM1
        JSR     ANIM0
        JSR     ANIM1
        JSR     ANIM2
        JSR     ANIM3
        JSR     ANIM4
        JSR     ANIM5
        JSR     ANIM10
        JSR     ANIM9
        JSR     ANIM8
        JSR     ANIM7
        JSR     ANIM6
        JSR     ANIM7
        JSR     ANIM8
        JSR     ANIM9
        JSR     ANIM10
        LDA     ANIM_COUNT
        CMP     #100
        BCC     .loop

        JSR     ANIM5
        JSR     ANIM4
        JSR     ANIM3
        JSR     ANIM2
        JSR     ANIM1
        JSR     ANIM0
        CLC
        RTS

        ORG    $8B7A
    ANIM0:
        JSR     SHOW_ANIM_LINE
        HEX     00 01 02 03 04 05 06 07 08 09 0A 02 01 00
    ANIM1:
        JSR     SHOW_ANIM_LINE
        HEX     00 00 01 02 03 04 05 07 09 0A 02 01 00 00
    ANIM2:
        JSR     SHOW_ANIM_LINE
        HEX     00 00 00 01 02 03 04 09 0A 02 01 00 00 00
    ANIM3:
        JSR     SHOW_ANIM_LINE
        HEX     00 00 00 00 01 02 03 0A 02 01 00 00 00 00
    ANIM4:
        JSR     SHOW_ANIM_LINE
        HEX     00 00 00 00 00 01 03 0A 01 00 00 00 00 00
    ANIM5:
        JSR     SHOW_ANIM_LINE
        HEX     00 00 00 00 00 00 01 01 00 00 00 00 00 00
    ANIM6:
        JSR     SHOW_ANIM_LINE
        HEX     00 01 02 0A 09 08 07 06 05 04 03 02 01 00
    ANIM7:
        JSR     SHOW_ANIM_LINE
        HEX     00 00 01 02 0A 09 07 05 04 03 02 01 00 00
    ANIM8:
        JSR     SHOW_ANIM_LINE
        HEX     00 00 00 01 02 0A 09 04 03 02 01 00 00 00
    ANIM9:
        JSR     SHOW_ANIM_LINE
        HEX     00 00 00 00 01 02 0A 03 02 01 00 00 00 00
    ANIM10:
        JSR     SHOW_ANIM_LINE
        HEX     00 00 00 00 00 01 0A 03 01 00 00 00 00 00

        ORG    $8C35
    TABLE0:
        HEX     80 80 80 80 80 80 80 80 80 80 80 80 80 80
    TABLE1:
        HEX     C0 AA D5 AA D5 AA D5 AA D5 AA D5 AA D5 80
    TABLE2:
        HEX     90 80 80 80 80 80 80 80 80 80 80 80 80 82
    TABLE3:
        HEX     90 AA D1 A2 D5 A8 85 A8 C5 A2 D4 A2 95 82
    TABLE4:
        HEX     90 82 91 A2 C5 A8 80 88 C5 A2 94 A0 90 82
    TABLE5:
        HEX     90 82 90 A2 C4 A8 80 88 C5 A2 94 A0 90 82
    TABLE6:
        HEX     90 82 90 A2 C4 A8 81 88 C4 A2 D4 A0 95 82
    TABLE7:
        HEX     90 A2 D1 A2 C4 88 80 88 C4 A2 84 A0 85 82
    TABLE8:
        HEX     90 82 91 A2 C4 88 80 88 C4 AA 84 A0 85 82
    TABLE9:
        HEX     90 82 91 A2 C4 88 80 88 C4 8A 84 A0 91 82
    TABLE10:
        HEX     90 AA 91 A2 C4 A8 85 A8 85 82 D4 A2 91 82

        ORG    $8CCF
    ADDRESS_TABLE:
        WORD    TABLE0-13
        WORD    TABLE1-13
        WORD    TABLE2-13
        WORD    TABLE3-13
        WORD    TABLE4-13
        WORD    TABLE5-13
        WORD    TABLE6-13
        WORD    TABLE7-13
        WORD    TABLE8-13
        WORD    TABLE9-13
        WORD    TABLE10-13
        ORG    $8CE5
    SHOW_ANIM_LINE:
        SUBROUTINE

        PLA
        STA     TMP_PTR
        PLA
        STA     TMP_PTR+1           ; store "return" addr

        ; Fill 14 rows of pixel data from row 0x51 (81) through 0x5E (94).
        LDY     #$50
        STY     GAME_ROWNUM
        BNE     .next      ; unconditional

    .loop:
        JSR     ROW_TO_ADDR
        LDY     #$00
        LDA     (TMP_PTR),Y
        ASL
        TAX
        LDA     ADDRESS_TABLE,X
        STA     .loop2+1
        LDA     ADDRESS_TABLE+1,X   ; groups of 14 bytes
        STA     .loop2+2
        LDY     #$0D

        ; Copy 13 bytes of pixel data onto screen from
        ; addr+14 to addr+26
    .loop2:
        LDA     $8D08,Y             ; fixed up from above
        STA     (ROW_ADDR),Y        ; pixel data
        INY
        CPY     #$1B
        BCC     .loop2              ; Y < 27

        ; Next row
    .next:
        JSR     INCREMENT_TMP_PTR
        INC     GAME_ROWNUM
        LDY     GAME_ROWNUM
        CPY     #$5F
        BCC     .loop

        LDX     ANIM_COUNT
        LDY     #$FF
    .delay:
        DEY
        BNE     .delay
        DEX
        BNE     .delay
        INC     ANIM_COUNT

        LDA     INPUT_MODE
        CMP     #KEYBOARD_MODE
        BEQ     .check_for_keypress
        LDA     BUTN1
        BMI     .input_detected
        LDA     BUTN0
        BMI     .input_detected

    .check_for_keypress:
        LDA     KBD
        BMI     .input_detected
        RTS

        ; Skip the rest of the big animation.
    .input_detected:
        PLA
        PLA
        SEC
        LDA     KBD
        STA     KBDSTRB
        RTS

        ORG    $8D4B
    ANIM_COUNT:
        HEX     9D

        ORG    $8D4C
    INCREMENT_TMP_PTR:
        SUBROUTINE

        INC     TMP_PTR
        BNE     .end
        INC     TMP_PTR+1
    .end:
        RTS

*/
#include "lode_runner.h"

/* The circle's center, as a screen line and a pixel column. */
#define CENTER_ROW 88
#define CENTER_X 140

/* The play area is 176 lines of 40 bytes. */
#define PLAY_ROWS 176
#define PLAY_BYTES 40

/* A pixel column, as the byte of the screen line it's in and its
 * place in that byte, 0-6. */
struct wipe_col {
    uint8_t byte;
    uint8_t pixel;
};

/* 0 while the iris closes, 1 while it opens. */
static uint8_t wipe_dir;

/* The lines and columns of the circle's eight points at one step of
 * its x and y, where x counts down from the radius and y up from 0.
 * The original's WIPE3, WIPE4, WIPE5, and WIPE6 are the lines, and
 * WIPE7, WIPE8, WIPE9, and WIPE10 the columns. A line can be
 * negative. */
static int16_t row_minus_x, row_minus_y, row_plus_y, row_plus_x;
static struct wipe_col col_minus_x, col_minus_y, col_plus_y, col_plus_x;

/* A pixel's block is pixels 0-3 of its byte (the low four bits) or
 * pixels 4-6 (the next three). WIPE_BLOCK_CLOSE_MASK holds the bits
 * that closing keeps, so it clears the block. WIPE_BLOCK_OPEN_MASK
 * holds the bits that opening copies from page 2, the block and bit
 * 7, the byte's color. */
static const uint8_t WIPE_BLOCK_CLOSE_MASK[7] = {
    0xF0, 0xF0, 0xF0, 0xF0, 0x8F, 0x8F, 0x8F,
};
static const uint8_t WIPE_BLOCK_OPEN_MASK[7] = {
    0x8F, 0x8F, 0x8F, 0x8F, 0xF0, 0xF0, 0xF0,
};

/* Sets `c` to pixel column `x`, from -30 to 310, divided by 7. A
 * negative column gives the quotient rounded down, as a byte from
 * 0xFB, and a pixel from 0 to 6. */
static void div_by_7(int16_t x, struct wipe_col *c) {
    /* 5 * 7 makes the column positive, and 5 comes off the quotient.
     * The column is then less than 7 << 6, so the quotient has 7
     * bits. */
    uint16_t n = x + 35;
    uint8_t q = 0;
    for (uint16_t d = 7 << 6; d >= 7; d >>= 1) {
        q <<= 1;
        if (n >= d) {
            n -= d;
            q++;
        }
    }
    c->byte = q - 5;
    c->pixel = n;
}

static void step_right(struct wipe_col *c) {
    if (++c->pixel == 7) {
        c->pixel = 0;
        c->byte++;
    }
}

static void step_left(struct wipe_col *c) {
    if (c->pixel == 0) {
        c->pixel = 6;
        c->byte--;
    } else {
        c->pixel--;
    }
}

/* Clears the block of `c` on the page 1 line at `line`, or copies it
 * from page 2 while the iris opens. A column off the screen is
 * skipped. */
static void draw_wipe_block(uint8_t *line, struct wipe_col *c) {
    if (c->byte >= PLAY_BYTES)
        return;
    if (wipe_dir == 0)
        line[c->byte] &= WIPE_BLOCK_CLOSE_MASK[c->pixel];
    else
        line[c->byte] |= line[c->byte + 0x2000] & WIPE_BLOCK_OPEN_MASK[c->pixel];
}

/* Draws the blocks at two columns of a line, the east one first. A
 * line outside the play area is skipped. */
static void draw_wipe_row(
    int16_t row, struct wipe_col *east, struct wipe_col *west) {
    if ((uint16_t)row >= PLAY_ROWS)
        return;
    uint8_t *line = row_to_addr(row, 0x20);
    draw_wipe_block(line, east);
    draw_wipe_block(line, west);
}

/* Draws the circle's eight points at one step. */
static void draw_wipe_step(void) {
    draw_wipe_row(row_plus_x, &col_plus_y, &col_minus_y);
    draw_wipe_row(row_minus_x, &col_plus_y, &col_minus_y);
    draw_wipe_row(row_plus_y, &col_plus_x, &col_minus_x);
    draw_wipe_row(row_minus_y, &col_plus_x, &col_minus_x);
}

/* Draws the circle of radius `r`, an eighth of it at a time with
 * the midpoint algorithm, from x = r, y = 0 until y passes x. `d`,
 * the midpoint's distance from the circle, decides when x steps. */
static void iris_wipe_step(uint8_t r) {
    uint8_t x = r;
    uint8_t y = 0;
    int16_t d = 3 - (r << 1);

    row_minus_x = CENTER_ROW - r;
    row_minus_y = CENTER_ROW;
    row_plus_y = CENTER_ROW;
    row_plus_x = CENTER_ROW + r;
    div_by_7(CENTER_X - r, &col_minus_x);
    col_minus_y.byte = col_plus_y.byte = CENTER_X / 7;
    col_minus_y.pixel = col_plus_y.pixel = CENTER_X % 7;
    div_by_7(CENTER_X + r, &col_plus_x);

    for (;;) {
        if (y > x)
            return;
        draw_wipe_step();
        if (y == x)
            return;
        if (d < 0) {
            d += (y << 2) + 6;
        } else {
            /* The textbook adds 10 here. */
            d += 16 - ((x - y) << 2);
            x--;
            row_minus_x++;
            step_left(&col_plus_x);
            step_right(&col_minus_x);
            row_plus_x--;
        }
        y++;
        step_right(&col_plus_y);
        row_minus_y--;
        row_plus_y++;
        step_left(&col_minus_y);
    }
}

void iris_wipe(void) {
    uint8_t r;
    if (WIPE_MODE != 0) {
        wipe_dir = 0;
        r = 0xAA;
        do {
            iris_wipe_step(r);
        } while (--r != 0);
    }
    WIPE_MODE = 1;
    wipe_dir = 1;
    put_status_lives();
    put_status_level();
    r = 1;
    do {
        iris_wipe_step(r);
    } while (++r != 0xAA);
}

/* GAME OVER at each of 11 angles of the turn, as the picture in
 * ANIM_PIXELS that each of its 14 lines takes. ANIM0 is face on,
 * ANIM5 edge on, and ANIM6-ANIM10 the far side of the turn. */
static const uint8_t ANIM0[14] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x02, 0x01, 0x00};
static const uint8_t ANIM1[14] = {0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x09, 0x0A, 0x02, 0x01, 0x00, 0x00};
static const uint8_t ANIM2[14] = {0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x09, 0x0A, 0x02, 0x01, 0x00, 0x00, 0x00};
static const uint8_t ANIM3[14] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x0A, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00};
static const uint8_t ANIM4[14] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0A, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t ANIM5[14] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t ANIM6[14] = {0x00, 0x01, 0x02, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
static const uint8_t ANIM7[14] = {0x00, 0x00, 0x01, 0x02, 0x0A, 0x09, 0x07, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0x00};
static const uint8_t ANIM8[14] = {0x00, 0x00, 0x00, 0x01, 0x02, 0x0A, 0x09, 0x04, 0x03, 0x02, 0x01, 0x00, 0x00, 0x00};
static const uint8_t ANIM9[14] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0A, 0x03, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00};
static const uint8_t ANIM10[14] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0A, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

/* The 11 pictures of a line, the 14 bytes from byte 13 to byte 26 of
 * a screen line. */
static const uint8_t ANIM_PIXELS[11][14] = {
    {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80},
    {0xC0, 0xAA, 0xD5, 0xAA, 0xD5, 0xAA, 0xD5, 0xAA, 0xD5, 0xAA, 0xD5, 0xAA, 0xD5, 0x80},
    {0x90, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x82},
    {0x90, 0xAA, 0xD1, 0xA2, 0xD5, 0xA8, 0x85, 0xA8, 0xC5, 0xA2, 0xD4, 0xA2, 0x95, 0x82},
    {0x90, 0x82, 0x91, 0xA2, 0xC5, 0xA8, 0x80, 0x88, 0xC5, 0xA2, 0x94, 0xA0, 0x90, 0x82},
    {0x90, 0x82, 0x90, 0xA2, 0xC4, 0xA8, 0x80, 0x88, 0xC5, 0xA2, 0x94, 0xA0, 0x90, 0x82},
    {0x90, 0x82, 0x90, 0xA2, 0xC4, 0xA8, 0x81, 0x88, 0xC4, 0xA2, 0xD4, 0xA0, 0x95, 0x82},
    {0x90, 0xA2, 0xD1, 0xA2, 0xC4, 0x88, 0x80, 0x88, 0xC4, 0xA2, 0x84, 0xA0, 0x85, 0x82},
    {0x90, 0x82, 0x91, 0xA2, 0xC4, 0x88, 0x80, 0x88, 0xC4, 0xAA, 0x84, 0xA0, 0x85, 0x82},
    {0x90, 0x82, 0x91, 0xA2, 0xC4, 0x88, 0x80, 0x88, 0xC4, 0x8A, 0x84, 0xA0, 0x91, 0x82},
    {0x90, 0xAA, 0x91, 0xA2, 0xC4, 0xA8, 0x85, 0xA8, 0x85, 0x82, 0xD4, 0xA2, 0x91, 0x82},
};

/* The angles of one turn, edge on to face on, back to edge on, and
 * around the far side. The turn ends with the first 6, back to face
 * on. */
static const uint8_t *const SPIN_ORDER[20] = {
    ANIM5, ANIM4, ANIM3, ANIM2, ANIM1, ANIM0, ANIM1, ANIM2, ANIM3, ANIM4,
    ANIM5, ANIM10, ANIM9, ANIM8, ANIM7, ANIM6, ANIM7, ANIM8, ANIM9, ANIM10,
};

/* The number of delay passes after each picture, which grows by one
 * each time, so the turn slows down. */
static uint8_t anim_count = 0x9D;

/* Draws GAME OVER at the angle whose 14 line pictures are `rows`,
 * on screen lines 0x51-0x5E of page 1, and waits. Returns true, with
 * KBD's value in game_over_key and the keyboard strobe cleared, when
 * a key is waiting or, outside the
 * keyboard mode, a button is down. GAME_ROWNUM is left at 0x5F, as
 * the original leaves it. */
uint8_t game_over_key;

static bool show_anim_line(const uint8_t *rows) {
    for (GAME_ROWNUM = 0x51; GAME_ROWNUM < 0x5F; GAME_ROWNUM++) {
        uint8_t *line = row_to_addr(GAME_ROWNUM, 0x20) + 13;
        const uint8_t *pixels = ANIM_PIXELS[*rows++];
        for (uint8_t i = 0; i < 14; i++)
            line[i] = pixels[i];
    }

    /* 255 passes, then 256 for each further count. */
    uint8_t y = 0xFF;
    [[c6502::keep]] for (uint8_t x = anim_count; x != 0; x--) {
        [[c6502::keep]] for (; --y;) {
        }
    }
    anim_count++;

    if ((INPUT_MODE != KEYBOARD_MODE && ((BUTN1 & 0x80) || (BUTN0 & 0x80)))
        || (KBD & 0x80)) {
        game_over_key = KBD;
        KBDSTRB = game_over_key;
        return true;
    }
    return false;
}

bool spinning_game_over(void) {
    anim_count = 1;
    do {
        for (uint8_t i = 0; i < 20; i++) {
            if (show_anim_line(SPIN_ORDER[i]))
                return true;
        }
    } while (anim_count < 100);
    for (uint8_t i = 0; i < 6; i++) {
        if (show_anim_line(SPIN_ORDER[i]))
            return true;
    }
    return false;
}
