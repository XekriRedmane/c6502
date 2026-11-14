/*
    save.c -- the Z-machine's `save` instruction.

    Zork I (r15), output/main.asm lines 3634-3699:

    instr_save:
        JSR      please_insert_save_diskette
        LDX      #$00
        LDY      #HEADER_VERSION
        LDA      (Z_HEADER_ADDR),Y
        STA      BUFF_AREA,X
        INX
        STOW     #Z_PC, SCRATCH2
        LDY      #$03
        JSR      copy_data_to_buff
        STOW     #LOCAL_ZVARS, SCRATCH2
        LDY      #30
        JSR      copy_data_to_buff
        STOW     #STACK_COUNT, SCRATCH2
        LDY      #6
        JSR      copy_data_to_buff
        JSR      write_next_sector
        BCS      .fail
        LDX      #$00
        STOW     #$0280, SCRATCH2
        LDY      #$00
        JSR      copy_data_to_buff
        JSR      write_next_sector
        BCS      .fail
        LDX      #$00
        STOW     #$0380, SCRATCH2
        LDY      #$68
        JSR      copy_data_to_buff
        JSR      write_next_sector
        BCS      .fail
        MOVW     Z_HEADER_ADDR, SCRATCH2
        LDY      #HEADER_STATIC_MEM_BASE
        LDA      (Z_HEADER_ADDR),Y
        STA      SCRATCH3                   ; big-endian!
        INC      SCRATCH3
    .loop:
        JSR      inc_sector_and_write
        BCS      .fail
        INC      SCRATCH2+1
        DEC      SCRATCH3
        BNE      .loop
        JSR      inc_sector_and_write
        BCS      .fail
        JSR      please_reinsert_game_diskette
        JMP      branch
    .fail:
        JSR      please_reinsert_game_diskette
        JMP      negated_branch
    copy_data_to_buff:
        DEY
        LDA      (SCRATCH2),Y
        STA      BUFF_AREA,X
        INX
        CPY      #$00
        BNE      copy_data_to_buff
        RTS

    A save is three sectors of interpreter state followed by the story's
    dynamic memory, a page to a sector. The first sector holds the
    story's version, the program counter, the local variables, and the
    stack's bookkeeping; the next two hold the Z-machine's stack. Each
    group of bytes goes into the sector last byte first.

    Departures from the original:

    - The bytes to copy or write are a parameter, where the original
      passes them in SCRATCH2. The compiler keeps the page pointer of
      the loop in inc_sector_and_write's parameter, which that routine
      leaves alone, as the original keeps it in SCRATCH2.
    - copy_data_to_buff returns the next index into BUFF_AREA in A,
      where the original leaves it in X.
    - The page count is a local where the original uses SCRATCH3.
*/
#include "zork.h"

/* Copies the `count` bytes at `src` into BUFF_AREA from index `x` on,
 * last byte first, and returns the index after them. A count of 0
 * copies 256. */
static uint8_t copy_data_to_buff(
    const uint8_t *const src, uint8_t x __attribute__((in_x)),
    uint8_t count __attribute__((in_y))) {
    do {
        count--;
        BUFF_AREA[x] = src[count];
        x++;
    } while (count != 0);
    return x;
}

void instr_save(void) {
    please_insert_save_diskette();
    uint8_t x = 0;
    BUFF_AREA[x] = Z_HEADER_ADDR[HEADER_VERSION];
    x++;
    x = copy_data_to_buff(Z_PC, x, 3);
    x = copy_data_to_buff(LOCAL_ZVARS, x, 30);
    copy_data_to_buff(STACK_STATE, x, 6);
    if (write_next_sector())
        goto fail;
    copy_data_to_buff(Z_STACK, 0, 0);
    if (write_next_sector())
        goto fail;
    copy_data_to_buff(Z_STACK + 0x100, 0, 0x68);
    if (write_next_sector())
        goto fail;
    uint8_t *page = Z_HEADER_ADDR;
    uint8_t pages = Z_HEADER_ADDR[HEADER_STATIC_MEM_BASE] + 1;
    do {
        if (inc_sector_and_write(page))
            goto fail;
        page += 0x100;
    } while (--pages != 0);
    if (inc_sector_and_write(page))
        goto fail;
    please_reinsert_game_diskette();
    branch();
    return;
fail:
    please_reinsert_game_diskette();
    negated_branch();
}
