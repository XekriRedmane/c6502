/*
    screen.c -- the status line, text, and numbers on the
    high-resolution screen.

    Text is drawn a sprite to a character, at the cell GAME_COLNUM,
    GAME_ROWNUM of the level's grid, on the page DRAW_PAGE names. Row 16
    is the status line under the level, which shows the score, the
    lives ("MEN"), and the level number.

    Departures from the original:

    - row_to_addr takes the page as a parameter, where the original
      reads it from HGR_PAGE, and returns the address, where the
      original leaves it in ROW_ADDR. Nothing in this unit uses
      ROW_TO_ADDR_FOR_BOTH_PAGES, and sprites.c and wipe.c, whose
      routines call it, compute both pages' addresses from
      row_to_addr's, so it isn't here.
    - CLEAR_PAGE takes a pointer to the screen page and clears 0x2000
      bytes from it, where the original takes the first and the end
      page numbers in A and X.
    - PUT_STATUS_BYTE takes the number and the column as parameters.
    - add_and_update_score takes the points as a parameter, where the
      original takes their low byte in A and their high byte in Y. It
      adds them to SCORE with __c6502_bcd_add, which sets the 6502's
      decimal mode and adds a byte at a time as the original does,
      through a uint32_t pointer to SCORE's four bytes.
    - put_digit and put_char share the drawing of a sprite at the
      cursor, draw_at_cursor, which the original repeats in each.
    - char_to_sprite_num finds the punctuation's sprites by searching
      the table PUNCTUATION, where the original compares with each
      character in turn.
    - put_string takes a pointer to a plain ASCII string and sets bit
      7 of each character, where the original reads high-bit ASCII
      bytes placed after its JSR and returns past them.

    Lode Runner, main.asm lines 4865-5214:

        ORG    $79AD
    PUT_STATUS:
        SUBROUTINE

        JSR     CLEAR_HGR1
        JSR     CLEAR_HGR2

    PUT_STATUS_DRAW:
        LDY     #$27
        LDA     DRAW_PAGE
        CMP     #$40
        BEQ     .draw_line_on_page_2

    .draw_line_on_page_1:
        LDA     #$AA
        STA     $2350,Y
        STA     $2750,Y
        STA     $2B50,Y
        STA     $2F50,Y
        DEY
        LDA     #$D5
        STA     $2350,Y
        STA     $2750,Y
        STA     $2B50,Y
        STA     $2F50,Y
        DEY
        BPL     .draw_line_on_page_1
        BMI     .end        ; Unconditional

    .draw_line_on_page_2:
        LDA     #$AA
        STA     $4350,Y
        STA     $4750,Y
        STA     $4B50,Y
        STA     $4F50,Y
        DEY
        LDA     #$D5
        STA     $4350,Y
        STA     $4750,Y
        STA     $4B50,Y
        STA     $4F50,Y
        DEY
        BPL     .draw_line_on_page_2

    .end:
        LDA     #$10
        STA     GAME_ROWNUM
        LDA     #$00
        STA     GAME_COLNUM

        ; "SCORE        MEN    LEVEL   "
        JSR     PUT_STRING
        HEX     D3 C3 CF D2 C5 A0 A0 A0 A0 A0 A0 A0 A0 CD C5 CE
        HEX     A0 A0 A0 A0 CC C5 D6 C5 CC A0 A0 A0 00

        JSR     PUT_STATUS_LIVES
        JSR     PUT_STATUS_LEVEL
        LDA     #$00
        TAY
        JMP     ADD_AND_UPDATE_SCORE        ; tailcall


        ORG    $7A31
    ROW_TO_ADDR:
        SUBROUTINE
        ; Enter routine with Y set to row. Base address
        ; (for column 0) will be placed in ROW_ADDR.

        LDA     ROW_TO_OFFSET_LO,Y 
        STA     ROW_ADDR
        LDA     ROW_TO_OFFSET_HI,Y 
        ORA     HGR_PAGE
        STA     ROW_ADDR+1
        RTS
        ORG    $7A3E
    ROW_TO_ADDR_FOR_BOTH_PAGES:
        SUBROUTINE
        ; Enter routine with Y set to row. Base address
        ; (for column 0) will be placed in ROW_ADDR (for page 1)
        ; and ROW_ADDR2 (for page 2).

        LDA     ROW_TO_OFFSET_LO,Y 
        STA     ROW_ADDR
        STA     ROW_ADDR2
        LDA     ROW_TO_OFFSET_HI,Y 
        ORA     #$20
        STA     ROW_ADDR+1
        EOR     #$60
        STA     ROW_ADDR2+1
        RTS
        ORG    $7A51
    CLEAR_HGR1:
        SUBROUTINE

        LDA     #$20                ; Start at $2000
        LDX     #$40                ; End at $4000 (but not including)
        BNE     CLEAR_PAGE          ; Unconditional jump

    CLEAR_HGR2:
        SUBROUTINE

        LDA     #$40                ; Start at $4000
        LDX     #$60                ; End at $6000 (but not including)
        ; fallthrough

    CLEAR_PAGE:
        STA     TMP_PTR+1           ; Start with the page in A.
        LDA     #$00
        STA     TMP_PTR
        TAY
        LDA     #$80                ; fill byte = 0x80

    .loop:
        STA     (TMP_PTR),Y
        INY
        BNE     .loop
        INC     TMP_PTR+1
        CPX     TMP_PTR+1
        BNE     .loop               ; while TMP_PTR != X * 0x100
        RTS
        ORG    $7A70
    PUT_STATUS_LIVES:
        SUBROUTINE

        LDA     LIVES
        LDX     #16
        ; fallthrough

    PUT_STATUS_BYTE:
        SUBROUTINE
        ; Puts the number in A as a three-digit decimal on the screen
        ; at row 16, column X.

        STX     GAME_COLNUM
        JSR     TO_DECIMAL3
        LDA     #16
        STA     GAME_ROWNUM
        LDA     HUNDREDS
        JSR     PUT_DIGIT
        LDA     TENS
        JSR     PUT_DIGIT
        LDA     UNITS
        JMP     PUT_DIGIT           ; tail call

    PUT_STATUS_LEVEL:
        SUBROUTINE

        LDA     LEVELNUM
        LDX     #25
        BNE     PUT_STATUS_BYTE     ; Unconditional jump

        ORG    $7A92
    ADD_AND_UPDATE_SCORE:
        SUBROUTINE
        ; Enter routine with A set to BCD tens/units and
        ; Y set to BCD thousands/hundreds.

        CLC
        SED                         ; Turn on BCD addition mode.
        ADC     SCORE
        STA     SCORE
        TYA
        ADC     SCORE+1
        STA     SCORE+1
        LDA     #$00
        ADC     SCORE+2
        STA     SCORE+2
        LDA     #$00
        ADC     SCORE+3
        STA     SCORE+3             ; SCORE += param
        CLD                         ; Turn off BCD addition mode.

        LDA     #5
        STA     GAME_COLNUM
        LDA     #16
        STA     GAME_ROWNUM

        LDA     SCORE+3
        JSR     BCD_TO_DECIMAL2
        LDA     UNITS               ; Note we skipped TENS.
        JSR     PUT_DIGIT

        LDA     SCORE+2
        JSR     BCD_TO_DECIMAL2
        LDA     TENS
        JSR     PUT_DIGIT
        LDA     UNITS
        JSR     PUT_DIGIT

        LDA     SCORE+1
        JSR     BCD_TO_DECIMAL2
        LDA     TENS
        JSR     PUT_DIGIT
        LDA     UNITS
        JSR     PUT_DIGIT

        LDA     SCORE
        JSR     BCD_TO_DECIMAL2
        LDA     TENS
        JSR     PUT_DIGIT
        LDA     UNITS
        JMP     PUT_DIGIT           ; tail call


        ORG    $7AE9
    BCD_TO_DECIMAL2:
        SUBROUTINE
        ; Enter routine with A set to the BCD number to convert.

        STA     TENS
        AND     #$0F
        STA     UNITS
        LDA     TENS
        LSR
        LSR
        LSR
        LSR
        STA     TENS
        RTS


        ; Screen and level routines

        ORG    $7AF8
    TO_DECIMAL3:
        SUBROUTINE
        ; Enter routine with A set to the number to convert.

        LDX     #$00
        STX     TENS
        STX     HUNDREDS

    .loop1:
        CMP     #100
        BCC     .loop2
        INC     HUNDREDS
        SBC     #100
        BNE     .loop1

    .loop2:
        CMP     #10
        BCC     .end
        INC     TENS
        SBC     #10
        BNE     .loop2

    .end:
        STA     UNITS
        RTS

        ORG    $7B15
    PUT_DIGIT:
        SUBROUTINE
        ; Enter routine with A set to the digit to put on the screen.

        CLC
        ADC     #$3B                    ; '0' -> sprite 59, '9' -> sprite 68.
        LDX     DRAW_PAGE
        CPX     #$40
        BEQ     .draw_to_page2
        JSR     DRAW_SPRITE_PAGE1
        INC     GAME_COLNUM
        RTS

    .draw_to_page2:
        JSR     DRAW_SPRITE_PAGE2
        INC     GAME_COLNUM
        RTS

        ORG    $7B2A
    CHAR_TO_SPRITE_NUM:
        SUBROUTINE
        ; Enter routine with A set to the ASCII code of the
        ; character to convert to sprite number, with the high bit set.
        ; The sprite number is returned in A.

        CMP     #$C1                    ; 'A' -> sprite 69
        BCC     .not_letter
        CMP     #$DB                    ; 'Z' -> sprite 94
        BCC     .letter

    .not_letter:
        ; On return, we will subtract 0x7C from X to
        ; get the actual sprite. This is to make A-Z
        ; easier to handle.
        LDX     #$7C
        CMP     #$A0                    ; ' ' -> sprite 0
        BEQ     .end
        LDX     #$DB
        CMP     #$BE                    ; '>' -> sprite 95
        BEQ     .end
        INX
        CMP     #$AE                    ; '.' -> sprite 96
        BEQ     .end
        INX
        CMP     #$A8                    ; '(' -> sprite 97
        BEQ     .end
        INX
        CMP     #$A9                    ; ')' -> sprite 98
        BEQ     .end
        INX
        CMP     #$AF                    ; '/' -> sprite 99
        BEQ     .end
        INX
        CMP     #$AD                    ; '-' -> sprite 100
        BEQ     .end
        INX
        CMP     #$BC                    ; '<' -> sprite 101
        BEQ     .end
        LDA     #$10                    ; sprite 16: just one of the man sprites
        RTS

    .end:
        TXA

    .letter:
        SEC
        SBC     #$7C                    
        RTS


        ORG    $7B64
    PUT_CHAR:
        SUBROUTINE
        ; Enter routine with A set to the ASCII code of the
        ; character to put on the screen, with the high bit set.

        CMP     #$8D
        BEQ     NEWLINE                 ; If newline, do NEWLINE instead.
        JSR     CHAR_TO_SPRITE_NUM
        LDX     DRAW_PAGE
        CPX     #$40
        BEQ     .draw_to_page2

        JSR     DRAW_SPRITE_PAGE1
        INC     GAME_COLNUM
        RTS

    .draw_to_page2
        JSR     DRAW_SPRITE_PAGE2
        INC     GAME_COLNUM
        RTS

    NEWLINE:
        SUBROUTINE
        INC     GAME_ROWNUM
        LDA     #$00
        STA     GAME_COLNUM
        RTS


    Lode Runner, main.asm lines 6902-6930:

        ORG    $86E0
    PUT_STRING:
        SUBROUTINE

        PLA
        STA     SAVED_RET_ADDR
        PLA
        STA     SAVED_RET_ADDR+1
        BNE     .next

    .loop:
        LDY     #$00
        LDA     (SAVED_RET_ADDR),Y
        BEQ     .end
        JSR     PUT_CHAR

    .next:
        INC     SAVED_RET_ADDR
        BNE     .loop
        INC     SAVED_RET_ADDR+1
        BNE     .loop

    .end:
        LDA     SAVED_RET_ADDR+1
        PHA
        LDA     SAVED_RET_ADDR
        PHA
        RTS

*/
#include "lode_runner.h"

/* Clears the page of 0x2000 bytes at `page` to 0x80, which is black
 * with the high bit set. */
static void clear_page(uint8_t *page) {
    for (uint8_t pages = 0x20; pages != 0; pages--) {
        uint8_t i = 0;
        do {
            page[i] = 0x80;
        } while (++i != 0);
        page += 0x100;
    }
}

void clear_hgr1(void) {
    clear_page(HGR1);
}

void clear_hgr2(void) {
    clear_page(HGR2);
}

/* Clears both pages, then draws the status line on DRAW_PAGE. */
void put_status(void) {
    clear_hgr1();
    clear_hgr2();
    put_status_draw();
}

/* Draws the band across screen lines 176-179 that separates the level
 * from the status line, right to left, then the status line's labels,
 * lives, level, and score. */
void put_status_draw(void) {
    uint8_t *const page = DRAW_PAGE == 0x40 ? HGR2 : HGR1;
    uint8_t *const line176 = page + 0x0350;
    uint8_t *const line177 = page + 0x0750;
    uint8_t *const line178 = page + 0x0B50;
    uint8_t *const line179 = page + 0x0F50;
    uint8_t x = 0x27;
    do {
        line176[x] = 0xAA;
        line177[x] = 0xAA;
        line178[x] = 0xAA;
        line179[x] = 0xAA;
        x--;
        line176[x] = 0xD5;
        line177[x] = 0xD5;
        line178[x] = 0xD5;
        line179[x] = 0xD5;
        x--;
    } while ((int8_t)x >= 0);

    GAME_ROWNUM = 16;
    GAME_COLNUM = 0;
    put_string("SCORE        MEN    LEVEL   ");
    put_status_lives();
    put_status_level();
    add_and_update_score(0);
}

/* Returns the address of screen line `row` on the page whose first
 * byte's high byte is `page`, 0x20 or 0x40. */
uint8_t *row_to_addr(uint8_t row, uint8_t page) {
    return (uint8_t *)(ROW_TO_OFFSET_LO[row]
                       | (uint16_t)(ROW_TO_OFFSET_HI[row] | page) << 8);
}

/* Puts `n` as three decimal digits in the status line, from column
 * `col` on. */
static void put_status_byte(uint8_t n, uint8_t col) {
    GAME_COLNUM = col;
    to_decimal3(n);
    GAME_ROWNUM = 16;
    put_digit(DIGITS[0]);
    put_digit(DIGITS[1]);
    put_digit(DIGITS[2]);
}

void put_status_lives(void) {
    put_status_byte(LIVES, 16);
}

void put_status_level(void) {
    put_status_byte(LEVELNUM, 25);
}

void add_and_update_score(uint16_t bcd_points) {
    SCORE = __c6502_bcd_add(SCORE, bcd_points);

    GAME_COLNUM = 5;
    GAME_ROWNUM = 16;
    /* The status line shows seven digits, so the score's highest digit
     * isn't drawn. */
    bcd_to_decimal2(SCORE_BYTES[3]);
    put_digit(DIGITS[2]);
    for (int8_t i = 2; i >= 0; i--) {
        bcd_to_decimal2(SCORE_BYTES[i]);
        put_digit(DIGITS[1]);
        put_digit(DIGITS[2]);
    }
}

/* Sets DIGITS[1] and DIGITS[2] to the tens and units of the BCD byte
 * `bcd`. */
void bcd_to_decimal2(uint8_t bcd) {
    DIGITS[1] = bcd >> 4;
    DIGITS[2] = bcd & 0x0F;
}

/* Sets DIGITS to the hundreds, tens, and units of `n`. */
void to_decimal3(uint8_t n) {
    DIGITS[1] = 0;
    DIGITS[0] = 0;
    while (n >= 100) {
        DIGITS[0]++;
        n -= 100;
    }
    while (n >= 10) {
        DIGITS[1]++;
        n -= 10;
    }
    DIGITS[2] = n;
}

/* Draws `sprite` at the cursor on DRAW_PAGE and moves the cursor
 * right. */
static void draw_at_cursor(uint8_t sprite) {
    if (DRAW_PAGE == 0x40)
        draw_sprite_page2(sprite);
    else
        draw_sprite_page1(sprite);
    GAME_COLNUM++;
}

/* Draws the decimal digit `digit` at the cursor and moves the cursor
 * right. */
void put_digit(uint8_t digit) {
    draw_at_cursor(digit + SPRITE_DIGIT0);
}

/* The punctuation that has sprites, in high-bit ASCII. Sprite 95 is
 * '>', and the others follow in order. */
static const uint8_t PUNCTUATION[7] = {
    0xBE, 0xAE, 0xA8, 0xA9, 0xAF, 0xAD, 0xBC,   /* ">.()/-<" */
};

/* Returns the sprite that draws the high-bit ASCII character `c`. The
 * letters A-Z are sprites 69-94, space is sprite 0, and ">.()/-<" are
 * sprites 95-101. Every other character, the digits included, is drawn
 * as sprite 0x10, one of the player's sprites. */
uint8_t char_to_sprite_num(uint8_t c) {
    if (c >= 0xC1 && c < 0xDB)
        return c - 0x7C;
    if (c == 0xA0)
        return 0;
    for (uint8_t i = 0; i < 7; i++) {
        if (c == PUNCTUATION[i])
            return 95 + i;
    }
    return 0x10;
}

void put_char(uint8_t c) {
    if (c == 0x8D) {
        newline();
        return;
    }
    draw_at_cursor(char_to_sprite_num(c));
}

/* Moves the cursor to the start of the next row. */
void newline(void) {
    GAME_ROWNUM++;
    GAME_COLNUM = 0;
}

void put_string(const char *s) {
    for (; *s != 0; s++)
        put_char(*s | 0x80);
}
