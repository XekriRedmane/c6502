/*
    sprites.c -- drawing and erasing sprites on the high-resolution
    screen.

    A sprite is 11 screen lines high and 10 pixels wide. A screen byte
    holds 7 pixels, so a line of a sprite covers parts of two screen
    bytes, or of three when it starts at the sixth or seventh pixel of
    a byte. COMPUTE_SHIFTED_SPRITE shifts each line into three bytes
    through the tables at PIXEL_SHIFT_TABLE, and the drawing routines
    combine the result with the screen.

    The level's cells are drawn by draw_sprite_page1 and
    draw_sprite_page2 on whole cells. The player and the guards move
    between cells, and are drawn and erased on page 1 at a screen line
    and a double-pixel column. Page 2 holds the level without them, so
    erasing a sprite copies page 2's bits back where the sprite was.

    Departures from the original:

    - The sprite and the page are parameters, where the original keeps
      them in SPRITE_NUM and HGR_PAGE. The scratch bytes ROWNUM,
      COLNUM, ROW_COUNT, MASK0, MASK1, COL_SHIFT_AMT, and
      SCREENS_DIFFER are locals, and ROW_ADDR and ROW_ADDR2 are
      pointers. draw_sprite_at_pixel_coords returns SCREENS_DIFFER.
    - ROW_TO_ADDR_FOR_BOTH_PAGES is row_to_addr (screen.c) for page 1,
      plus 0x2000 for page 2. The original ORs the high byte from
      ROW_TO_OFFSET_HI with 0x20 for page 1 and then EORs that with
      0x60 for page 2, which adds 0x20 to page 1's high byte, since
      every entry of ROW_TO_OFFSET_HI is below 0x20.
    - COMPUTE_SHIFTED_SPRITE patches the addresses its loads read, and
      here they're pointers. The original adds 1 to the low byte of a
      shifted pair's address without carrying into the high byte, and
      this code adds 1 to the address. The two differ only for a low
      byte of 0xFF, which none of PIXEL_SHIFT_TABLE's entries has.
    - The erase and pixel-coordinate draw loops handle the two and
      three byte cases in one loop each, where the original has a loop
      for each case. Both write the same bytes in the same order.
    - GET_SCREEN_COORDS_FOR, GET_BYTE_AND_SHIFT_FOR_COL, and
      GET_BYTE_AND_SHIFT_FOR_HALF_SCREEN_COL are table lookups at the
      places that need them, and GET_PTRS_TO_CURR_LEVEL_SPRITE_DATA is
      indexing into LEVEL_MAP and LEVEL_BG wherever the original calls
      it.

    Lode Runner, main.asm lines 6132-6542:

        ORG    $82AA
    DRAW_SPRITE_PAGE1:
        SUBROUTINE
        ; Enter routine with A set to sprite number to draw,
        ; GAME_ROWNUM set to the row to draw it at, and GAME_COLNUM
        ; set to the column to draw it at.

        STA     SPRITE_NUM
        LDA     #$20                ; Page number for HGR1
        BNE     DRAW_SPRITE         ; Actually unconditional jump

    DRAW_SPRITE_PAGE2:
        SUBROUTINE
        ; Enter routine with A set to sprite number to draw,
        ; GAME_ROWNUM set to the row to draw it at, and GAME_COLNUM
        ; set to the column to draw it at.

        STA     SPRITE_NUM
        LDA     #$40                ; Page number for HGR2
        ; fallthrough

    DRAW_SPRITE:
        STA     HGR_PAGE
        LDY     GAME_ROWNUM
        JSR     GET_SCREEN_COORDS_FOR
        STY     ROWNUM              ; ROWNUM = SCREEN_ROW_TABLE[GAME_ROWNUM]

        LDX     GAME_COLNUM
        JSR     GET_BYTE_AND_SHIFT_FOR_COL
        STA     COLNUM              ; COLNUM = COL_BYTE_TABLE[GAME_COLNUM]
        STX     COL_SHIFT_AMT       ; COL_SHIFT_AMT = COL_SHIFT_TABLE[GAME_COLNUM]

        LDA     PIXEL_MASK0,X 
        STA     MASK0               ; MASK0 = PIXEL_MASK0[COL_SHIFT_AMT]
        LDA     PIXEL_MASK1,X 
        STA     MASK1               ; MASK1 = PIXEL_MASK1[COL_SHIFT_AMT]

        JSR     COMPUTE_SHIFTED_SPRITE

        LDA     #$0B
        STA     ROW_COUNT
        LDX     #$00
        LDA     COL_SHIFT_AMT
        CMP     #$05
        BCS     .need_3_bytes       ; If COL_SHIFT_AMT >= 5, we need to alter three
                                    ; screen bytes, otherwise just two bytes.

    .loop1:
        LDY     ROWNUM
        JSR     ROW_TO_ADDR
        LDY     COLNUM
        LDA     (ROW_ADDR),Y 
        AND     MASK0
        ORA     BLOCK_DATA,X 
        STA     (ROW_ADDR),Y        ; screen[COLNUM] =
                                    ;   screen[COLNUM] & MASK0 | BLOCK_DATA[i]

        INX                         ; X++
        INY                         ; Y++
        LDA     (ROW_ADDR),Y 
        AND     MASK1
        ORA     BLOCK_DATA,X 
        STA     (ROW_ADDR),Y        ; screen[COLNUM+1] =
                                    ;   screen[COLNUM+1] & MASK1 | BLOCK_DATA[i+1]

        INX
        INX                         ; X += 2
        INC     ROWNUM              ; ROWNUM++
        DEC     ROW_COUNT           ; ROW_COUNT--
        BNE     .loop1              ; loop while ROW_COUNT > 0
        RTS

    .need_3_bytes
        LDY     ROWNUM
        JSR     ROW_TO_ADDR
        LDY     COLNUM
        LDA     (ROW_ADDR),Y 
        AND     MASK0
        ORA     BLOCK_DATA,X 
        STA     (ROW_ADDR),Y        ; screen[COLNUM] =
                                    ;   screen[COLNUM] & MASK0 | BLOCK_DATA[i]

        INX                         ; X++
        INY                         ; Y++
        LDA     BLOCK_DATA,X 
        STA     (ROW_ADDR),Y        ; screen[COLNUM+1] = BLOCK_DATA[i+1]

        INX                         ; X++
        INY                         ; Y++
        LDA     (ROW_ADDR),Y 
        AND     MASK1
        ORA     BLOCK_DATA,X 
        STA     (ROW_ADDR),Y        ; screen[COLNUM+2] =
                                    ;   screen[COLNUM+2] & MASK1 | BLOCK_DATA[i+2]

        INX                         ; X++
        INC     ROWNUM              ; ROWNUM++
        DEC     ROW_COUNT           ; ROW_COUNT--
        BNE     .need_3_bytes       ; loop while ROW_COUNT > 0
        RTS
        ORG    $8328
    PIXEL_MASK0:
        BYTE    %00000000
        BYTE    %00000001
        BYTE    %00000011
        BYTE    %00000111
        BYTE    %00001111
        BYTE    %00011111
        BYTE    %00111111
    PIXEL_MASK1:
        BYTE    %11111000
        BYTE    %11110000
        BYTE    %11100000
        BYTE    %11000000
        BYTE    %10000000
        BYTE    %11111110
        BYTE    %11111100
        ORG    $8336
    ERASE_SPRITE_AT_PIXEL_COORDS:
        SUBROUTINE
        ; Enter routine with A set to sprite number to draw,
        ; Y set to the screen row to erase it at, and X
        ; set to *half* the screen column to erase it at.

        STY     ROWNUM
        STA     SPRITE_NUM
        JSR     GET_BYTE_AND_SHIFT_FOR_HALF_SCREEN_COL
        STA     COLNUM
        STX     COL_SHIFT_AMT
        JSR     COMPUTE_SHIFTED_SPRITE

        LDX     #$0B
        STX     ROW_COUNT
        LDX     #$00
        LDA     COL_SHIFT_AMT
        CMP     #$05
        BCS     .need_3_bytes       ; If COL_SHIFT_AMT >= 5, we need to alter three
                                    ; screen bytes, otherwise just two bytes.

    .loop1:
        LDY     ROWNUM
        JSR     ROW_TO_ADDR_FOR_BOTH_PAGES
        LDY     COLNUM
        LDA     BLOCK_DATA,X
        EOR     #$7F
        AND     (ROW_ADDR),Y
        ORA     (ROW_ADDR2),Y
        STA     (ROW_ADDR),Y            ; screen[COLNUM] =
                                        ;   (screen[COLNUM] & (BLOCK_DATA[i] ^ 0x7F)) |
                                        ;   screen2[COLNUM]

        INX                             ; X++
        INY                             ; Y++
        LDA     BLOCK_DATA,X
        EOR     #$7F
        AND     (ROW_ADDR),Y
        ORA     (ROW_ADDR2),Y
        STA     (ROW_ADDR),Y            ; screen[COLNUM+1] =
                                        ;   (screen[COLNUM+1] & (BLOCK_DATA[i+1] ^ 0x7F)) |
                                        ;   screen2[COLNUM+1]

        INX                             ; X++
        INX                             ; X++
        INC     ROWNUM
        DEC     ROW_COUNT
        BNE     .loop1
        RTS

    .need_3_bytes:
        LDY     ROWNUM
        JSR     ROW_TO_ADDR_FOR_BOTH_PAGES
        LDY     COLNUM
        LDA     BLOCK_DATA,X
        EOR     #$7F
        AND     (ROW_ADDR),Y
        ORA     (ROW_ADDR2),Y
        STA     (ROW_ADDR),Y            ; screen[COLNUM] =
                                        ;   (screen[COLNUM] & (BLOCK_DATA[i] ^ 0x7F)) |
                                        ;   screen2[COLNUM]

        INX                             ; X++
        INY                             ; Y++
        LDA     BLOCK_DATA,X
        EOR     #$7F
        AND     (ROW_ADDR),Y
        ORA     (ROW_ADDR2),Y
        STA     (ROW_ADDR),Y            ; screen[COLNUM+1] =
                                        ;   (screen[COLNUM+1] & (BLOCK_DATA[i+1] ^ 0x7F)) |
                                        ;   screen2[COLNUM+1]

        INX                             ; X++
        INY                             ; Y++
        LDA     BLOCK_DATA,X
        EOR     #$7F
        AND     (ROW_ADDR),Y
        ORA     (ROW_ADDR2),Y
        STA     (ROW_ADDR),Y            ; screen[COLNUM+2] =
                                        ;   (screen[COLNUM+2] & (BLOCK_DATA[i+2] ^ 0x7F)) |
                                        ;   screen2[COLNUM+2]

        INX                             ; X++
        INC     ROWNUM
        DEC     ROW_COUNT
        BNE     .need_3_bytes
        RTS

        ORG    $83A7
    DRAW_SPRITE_AT_PIXEL_COORDS:
        SUBROUTINE
        ; Enter routine with A set to sprite number to draw,
        ; Y set to the screen row to draw it at, and X
        ; set to *half* the screen column to draw it at.

        STY     ROWNUM
        STA     SPRITE_NUM
        JSR     GET_BYTE_AND_SHIFT_FOR_HALF_SCREEN_COL
        STA     COLNUM
        STX     COL_SHIFT_AMT
        JSR     COMPUTE_SHIFTED_SPRITE

        LDA     #$0B
        STA     ROW_COUNT
        LDX     #$00
        STX     SCREENS_DIFFER      ; SCREENS_DIFFER = 0
        LDA     COL_SHIFT_AMT
        CMP     #$05
        BCS     .need_3_bytes       ; If COL_SHIFT_AMT >= 5, we need to alter three
                                    ; screen bytes, otherwise just two bytes.

    .loop1:
        LDY     ROWNUM
        JSR     ROW_TO_ADDR_FOR_BOTH_PAGES
        LDY     COLNUM
        LDA     (ROW_ADDR),Y
        EOR     (ROW_ADDR2),Y
        AND     BLOCK_DATA,X
        ORA     SCREENS_DIFFER
        STA     SCREENS_DIFFER          ; SCREENS_DIFFER |= 
                                        ;   ( (screen[COLNUM] ^ screen2[COLNUM]) &
                                        ;     BLOCK_DATA[i])
        LDA     BLOCK_DATA,X            
        ORA     (ROW_ADDR),Y            
        STA     (ROW_ADDR),Y            ; screen[COLNUM] |= BLOCK_DATA[i]

        INX                             ; X++
        INY                             ; Y++
        LDA     (ROW_ADDR),Y
        EOR     (ROW_ADDR2),Y
        AND     BLOCK_DATA,X
        ORA     SCREENS_DIFFER
        STA     SCREENS_DIFFER          ; SCREENS_DIFFER |= 
                                        ;   ( (screen[COLNUM+1] ^ screen2[COLNUM+1]) &
                                        ;     BLOCK_DATA[i+1])
        LDA     BLOCK_DATA,X            
        ORA     (ROW_ADDR),Y            
        STA     (ROW_ADDR),Y            ; screen[COLNUM+1] |= BLOCK_DATA[i+1]

        INX                             ; X++
        INX                             ; X++
        INC     ROWNUM
        DEC     ROW_COUNT
        BNE     .loop1
        RTS

    .need_3_bytes:
        LDY     ROWNUM
        JSR     ROW_TO_ADDR_FOR_BOTH_PAGES
        LDY     COLNUM
        LDA     (ROW_ADDR),Y
        EOR     (ROW_ADDR2),Y
        AND     BLOCK_DATA,X
        ORA     SCREENS_DIFFER
        STA     SCREENS_DIFFER          ; SCREENS_DIFFER |= 
                                        ;   ( (screen[COLNUM] ^ screen2[COLNUM]) &
                                        ;     BLOCK_DATA[i])
        LDA     BLOCK_DATA,X            
        ORA     (ROW_ADDR),Y            
        STA     (ROW_ADDR),Y            ; screen[COLNUM] |= BLOCK_DATA[i]

        INX                             ; X++
        INY                             ; Y++
        LDA     (ROW_ADDR),Y
        EOR     (ROW_ADDR2),Y
        AND     BLOCK_DATA,X
        ORA     SCREENS_DIFFER
        STA     SCREENS_DIFFER          ; SCREENS_DIFFER |= 
                                        ;   ( (screen[COLNUM+1] ^ screen2[COLNUM+1]) &
                                        ;     BLOCK_DATA[i+1])
        LDA     BLOCK_DATA,X            
        ORA     (ROW_ADDR),Y            
        STA     (ROW_ADDR),Y            ; screen[COLNUM+1] |= BLOCK_DATA[i+1]

        INX                             ; X++
        INY                             ; Y++
        LDA     (ROW_ADDR),Y
        EOR     (ROW_ADDR2),Y
        AND     BLOCK_DATA,X
        ORA     SCREENS_DIFFER
        STA     SCREENS_DIFFER          ; SCREENS_DIFFER |= 
                                        ;   ( (screen[COLNUM+2] ^ screen2[COLNUM+2]) &
                                        ;     BLOCK_DATA[i+2])
        LDA     BLOCK_DATA,X            
        ORA     (ROW_ADDR),Y            
        STA     (ROW_ADDR),Y            ; screen[COLNUM+2] |= BLOCK_DATA[i+2]

        INX                             ; X++
        INC     ROWNUM
        DEC     ROW_COUNT
        BNE     .need_3_bytes
        RTS

        ORG    $8438
    COMPUTE_SHIFTED_SPRITE:
        SUBROUTINE
        ; Enter routine with X set to pixel shift amount and
        ; SPRITE_NUM containing the sprite number to read.

    .offset_table       EQU $A000               ; Target addresses in read
    .page_table         EQU $A080               ; instructions. The only truly
    .shift_ptr_byte0    EQU $A000               ; necessary value here is the
    .shift_ptr_byte1    EQU $A000               ; 0x80 in .shift_ptr_byte0.

        LDA     #$0B                            ; 11 rows
        STA     ROW_COUNT
        LDA     #<SPRITE_DATA
        STA     TMP_PTR
        LDA     #>SPRITE_DATA
        STA     TMP_PTR+1                       ; TMP_PTR = SPRITE_DATA
        LDA     PIXEL_SHIFT_PAGES,X 
        STA     .rd_offset_table + 2
        STA     .rd_page_table + 2
        STA     .rd_offset_table2 + 2
        STA     .rd_page_table2 + 2             ; Fix up pages in lookup instructions
                                                ; based on shift amount (X).

        LDX     #$00                            ; X is the offset into BLOCK_DATA.

    .loop:                                      ; === LOOP === (over all 11 rows)
        LDY     SPRITE_NUM
        LDA     (TMP_PTR),Y 
        TAY                                     ; Get sprite pixel data.

    .rd_offset_table:
        LDA     .offset_table,Y                 ; Load offset for shift amount.
        STA     .rd_shift_ptr_byte0 + 1
        CLC
        ADC     #$01
        STA     .rd_shift_ptr_byte1 + 1         ; Fix up instruction offsets with it.
    .rd_page_table:
        LDA     .page_table,Y                   ; Load page for shift amount.
        STA     .rd_shift_ptr_byte0 + 2
        STA     .rd_shift_ptr_byte1 + 2         ; Fix up instruction page with it.

    .rd_shift_ptr_byte0:
        LDA     .shift_ptr_byte0                ; Read shifted pixel data byte 0
        STA     BLOCK_DATA,X                    ; and store in block data byte 0.
    .rd_shift_ptr_byte1:
        LDA     .shift_ptr_byte1                ; Read shifted pixel data byte 1
        STA     BLOCK_DATA+1,X                  ; and store in block data byte 1.

        LDA     TMP_PTR
        CLC
        ADC     #$68
        STA     TMP_PTR
        LDA     TMP_PTR+1
        ADC     #$00
        STA     TMP_PTR+1                       ; TMP_PTR++

        ; Now basically do the same thing with the second sprite byte

        LDY     SPRITE_NUM
        LDA     (TMP_PTR),Y 
        TAY                                     ; Get sprite pixel data.

    .rd_offset_table2:
        LDA     .offset_table,Y                 ; Load offset for shift amount.
        STA     .rd_shift_ptr2_byte0 + 1
        CLC
        ADC     #$01
        STA     .rd_shift_ptr2_byte1 + 1        ; Fix up instruction offsets with it.
    .rd_page_table2:
        LDA     .page_table,Y                   ; Load page for shift amount.
        STA     .rd_shift_ptr2_byte0 + 2
        STA     .rd_shift_ptr2_byte1 + 2        ; Fix up instruction page with it.

    .rd_shift_ptr2_byte0:
        LDA     .shift_ptr_byte0                ; Read shifted pixel data byte 0
        ORA     BLOCK_DATA+1,X                  ; OR with previous block data byte 1
        STA     BLOCK_DATA+1,X                  ; and store in block data byte 1.
    .rd_shift_ptr2_byte1:
        LDA     .shift_ptr_byte1                ; Read shifted pixel data byte 1
        STA     BLOCK_DATA+2,X                  ; and store in block data byte 2.

        LDA     TMP_PTR
        CLC
        ADC     #$68
        STA     TMP_PTR
        LDA     TMP_PTR+1
        ADC     #$00
        STA     TMP_PTR+1                       ; TMP_PTR++

        INX
        INX
        INX                                     ; X += 3
        DEC     ROW_COUNT                       ; ROW_COUNT--
        BNE     .loop                           ; loop while ROW_COUNT > 0
        RTS
        ORG    $84C1
    PIXEL_SHIFT_PAGES:
        HEX     A2 A3 A4 A5 A6 A7 A8
        ORG    $84C8

    Lode Runner, main.asm lines 7346-7439:

        ORG    $884B
    GET_PTRS_TO_CURR_LEVEL_SPRITE_DATA:
        SUBROUTINE

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        RTS

        ORG    $885D
    GET_SCREEN_COORDS_FOR:
        SUBROUTINE
        ; Enter routine with Y set to sprite row (0-16) and
        ; X set to sprite column (0-27). On return, Y will be set to
        ; screen row, and X is set to half screen column.

        LDA     SCREEN_ROW_TABLE,Y 
        PHA
        LDA     HALF_SCREEN_COL_TABLE,X 
        TAX                         ; X = HALF_SCREEN_COL_TABLE[X]
        PLA
        TAY                         ; Y = SCREEN_ROW_TABLE[Y]
        RTS
        ORG    $8868
    GET_BYTE_AND_SHIFT_FOR_COL:
        SUBROUTINE
        ; Enter routine with X set to sprite column. On
        ; return, A will be set to screen column byte number
        ; and X will be set to an additional right shift amount.

        LDA     COL_BYTE_TABLE,X 
        PHA                         ; A = COL_BYTE_TABLE[X]
        LDA     COL_SHIFT_TABLE,X 
        TAX                         ; X = COL_SHIFT_TABLE[X]
        PLA
        RTS
        ORG    $8872
    GET_BYTE_AND_SHIFT_FOR_HALF_SCREEN_COL:
        SUBROUTINE
        ; Enter routine with X set to half screen column. On
        ; return, A will be set to screen column byte number
        ; and X will be set to an additional right shift amount.

        LDA     HALF_SCREEN_COL_BYTE_TABLE,X 
        PHA                         ; A = HALF_SCREEN_COL_BYTE_TABLE[X]
        LDA     HALF_SCREEN_COL_SHIFT_TABLE,X 
        TAX                         ; X = HALF_SCREEN_COL_SHIFT_TABLE[X]
        PLA
        RTS
        ORG    $887C
    GET_SCREEN_ROW_OFFSET_IN_X_FOR:
        SUBROUTINE
        ; Enter routine with X set to offset+2 (in double-pixels) and
        ; Y set to sprite row. On return, X will retain its value and
        ; Y will be set to the screen row.

        TXA
        PHA
        JSR     GET_SCREEN_COORDS_FOR
        PLA
        TAX                                 ; Restore X
        TYA
        CLC
        ADC     ROW_OFFSET_TABLE,X
        TAY
        RTS
        ORG    $888A
    ROW_OFFSET_TABLE:
        HEX     FB FD 00 02 04
        ORG    $888F
    GET_HALF_SCREEN_COL_OFFSET_IN_Y_FOR:
        SUBROUTINE
        ; Enter routine with Y set to offset+2 (in double-pixels) and
        ; X set to sprite column. On return, Y will retain its value and
        ; X will be set to the half screen column.

        TYA
        PHA
        JSR     GET_SCREEN_COORDS_FOR
        PLA
        TAY                                 ; Restore Y
        TXA
        CLC
        ADC     COL_OFFSET_TABLE,Y
        TAX
        RTS
        ORG    $889D
    COL_OFFSET_TABLE:
        HEX     FE FF 00 01 02
*/
#include "lode_runner.h"

/* For each shift, 0-6, the bits of the first and last screen bytes
 * under a sprite's line that the sprite leaves alone. */
static const uint8_t PIXEL_MASK0[7] = {
    0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F,
};
static const uint8_t PIXEL_MASK1[7] = {
    0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0xFE, 0xFC,
};

/* The screen line and double-pixel offsets of the adjustments 0-4. */
static const int8_t ROW_OFFSET_TABLE[5] = { -5, -3, 0, 2, 4 };
static const int8_t COL_OFFSET_TABLE[5] = { -2, -1, 0, 1, 2 };

/* Returns the address of the two bytes that hold the pixels of the
 * sprite byte `b`, shifted by the shift whose page of
 * PIXEL_SHIFT_TABLE is `table`. The page holds the addresses' low
 * bytes at 0x00-0x7F and their high bytes at 0x80-0xFF. For a sprite
 * byte of 0x80 or more, the low byte comes from the high bytes' half
 * and the high byte from the next page, as in the original. */
static const uint8_t *shifted_pixels(const uint8_t *table, uint8_t b) {
    const uint8_t *const high = table + 0x80;
    return (const uint8_t *)(table[b] | (uint16_t)high[b] << 8);
}

/* Fills BLOCK_DATA with `sprite` shifted right by `shift` pixels, 11
 * lines of three screen bytes. */
static void compute_shifted_sprite(uint8_t sprite, uint8_t shift) {
    const uint8_t *table = PIXEL_SHIFT_TABLE[shift];
    /* Each line of a sprite is two rows of SPRITE_DATA, the left byte
     * and then the right. */
    const uint8_t *data = &SPRITE_DATA[0][sprite];
    for (uint8_t i = 0; i < 33; i += 3) {
        const uint8_t *left = shifted_pixels(table, data[0]);
        BLOCK_DATA[i] = left[0];
        BLOCK_DATA[i + 1] = left[1];
        const uint8_t *right = shifted_pixels(table, data[0x68]);
        BLOCK_DATA[i + 1] |= right[0];
        BLOCK_DATA[i + 2] = right[1];
        data += 2 * 0x68;
    }
}

/* Draws `sprite` over the cell at GAME_COLNUM, GAME_ROWNUM on the page
 * at `page`, 0x20 or 0x40. The sprite's bits replace the screen's in
 * the 10 pixels of each line it covers. */
static void draw_sprite(uint8_t sprite, uint8_t page) {
    uint8_t row = SCREEN_ROW_TABLE[GAME_ROWNUM];
    uint8_t col = COL_BYTE_TABLE[GAME_COLNUM];
    uint8_t shift = COL_SHIFT_TABLE[GAME_COLNUM];
    uint8_t mask0 = PIXEL_MASK0[shift];
    uint8_t mask1 = PIXEL_MASK1[shift];
    compute_shifted_sprite(sprite, shift);

    /* With a shift of 5 or 6 a line of the sprite reaches a third
     * screen byte, and the middle byte is all sprite. */
    for (uint8_t i = 0; i < 33; i += 3) {
        uint8_t *line = row_to_addr(row, page) + col;
        line[0] = (line[0] & mask0) | BLOCK_DATA[i];
        if (shift < 5) {
            line[1] = (line[1] & mask1) | BLOCK_DATA[i + 1];
        } else {
            line[1] = BLOCK_DATA[i + 1];
            line[2] = (line[2] & mask1) | BLOCK_DATA[i + 2];
        }
        row++;
    }
}

void draw_sprite_page1(uint8_t sprite) {
    draw_sprite(sprite, 0x20);
}

void draw_sprite_page2(uint8_t sprite) {
    draw_sprite(sprite, 0x40);
}

void erase_sprite_at_pixel_coords(uint8_t sprite, uint8_t half_col, uint8_t row) {
    uint8_t col = HALF_SCREEN_COL_BYTE_TABLE[half_col];
    uint8_t shift = HALF_SCREEN_COL_SHIFT_TABLE[half_col];
    compute_shifted_sprite(sprite, shift);

    /* With a shift of 5 or 6 the sprite reaches a third screen byte. */
    uint8_t width = shift < 5 ? 2 : 3;
    for (uint8_t i = 0; i < 33; i += 3) {
        uint8_t *line = row_to_addr(row, 0x20);
        const uint8_t *background = line + 0x2000;
        /* The mask is the sprite's byte with bits 0-6 inverted, so the
         * pixels the sprite sets are copied from page 2, and the
         * others are page 1's ORed with page 2's. Bit 7, the byte's
         * color, is page 1's ORed with page 2's where the sprite's
         * byte has bit 7 set, and page 2's where it doesn't. */
        for (uint8_t b = 0; b < width; b++) {
            uint8_t x = col + b;
            line[x] = (line[x] & (BLOCK_DATA[i + b] ^ 0x7F)) | background[x];
        }
        row++;
    }
}

uint8_t draw_sprite_at_pixel_coords(uint8_t sprite, uint8_t half_col, uint8_t row) {
    uint8_t col = HALF_SCREEN_COL_BYTE_TABLE[half_col];
    uint8_t shift = HALF_SCREEN_COL_SHIFT_TABLE[half_col];
    compute_shifted_sprite(sprite, shift);

    uint8_t screens_differ = 0;
    uint8_t width = shift < 5 ? 2 : 3;
    for (uint8_t i = 0; i < 33; i += 3) {
        uint8_t *line = row_to_addr(row, 0x20);
        const uint8_t *background = line + 0x2000;
        /* Page 1 differs from page 2 where another sprite is drawn, so
         * a bit set in both that and this sprite is a collision. */
        for (uint8_t b = 0; b < width; b++) {
            uint8_t x = col + b;
            screens_differ |= (line[x] ^ background[x]) & BLOCK_DATA[i + b];
            line[x] |= BLOCK_DATA[i + b];
        }
        row++;
    }
    return screens_differ;
}

uint8_t screen_row_for(uint8_t row, uint8_t y_adj) {
    return SCREEN_ROW_TABLE[row] + ROW_OFFSET_TABLE[y_adj];
}

uint8_t half_screen_col_for(uint8_t col, uint8_t x_adj) {
    return HALF_SCREEN_COL_TABLE[col] + COL_OFFSET_TABLE[x_adj];
}
