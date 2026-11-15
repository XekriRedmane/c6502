/*
    main.c -- starting the interpreter, starting over, and stopping.

    Zork's boot loader reads the interpreter into memory from $0800,
    sets SECTORS_PER_TRACK, and jumps to $0800. `main` sets up the page
    cache and the stack, reads the story's dynamic and static memory
    from the game diskette into memory from __story, finds the top of
    RAM to size the cache, and then runs instructions until the story
    quits.

    Departures from the original:

    - The original starts with CLD, clears $80-$FF, and sets the 6502's
      stack pointer to $FF. C can do none of the three. The Monitor's
      reset routine clears decimal mode, and nothing after it sets it.
      `main` never returns, so a restart from inside a call leaves a
      few return addresses on the 6502's stack, which nothing reads,
      and the stack pointer wraps within its page. Instead of clearing
      zero page, `main` sets each byte of the interpreter's state that
      is read before it's written, since the compiler keeps its own
      bytes in that range too.
    - The page tables are page_tables, on the first page after the
      program's data, where the original's are at $2200-$23FF, and the
      story is loaded at __story, the first page after the tables,
      where the original loads it at $2C00.
    - The original's internal errors execute BRK, which drops into the
      Monitor. C can't execute BRK, so internal_error prints the
      message the original keeps but doesn't use, "ZORK INTERNAL
      ERROR!", and stops as instr_quit does.
    - instr_quit's and internal_error's endless loops are `for (;;)`.

    Zork I (r15), output/main.asm lines 249-403, 3025-3049, 3773-3790:

    main:
        CLD
        LDA      #$00
        LDX      #$80
    .clear:
        STA      $00,X
        INX
        BNE      .clear
        LDX      #$FF
        TXS
    .set_vars:
        ; Historical note: Setting PRINTER_CSW was originally a call to SINIT,
        ; "system-dependent initialization".
        LDA      #$C1
        STA      PRINTER_CSW+1
        LDA      #$00
        STA      PRINTER_CSW
        LDA      #$00
        STA      ZCODE_PAGE_VALID
        STA      ZCODE_PAGE_VALID2
        STOB     #$01, STACK_COUNT
        STOW     #$03E8, Z_SP
        STOB     #$FF, ZCHAR_SCRATCH1+6
        STOW     #$2200, PAGE_L_TABLE
        STOW     #$2280, PAGE_H_TABLE
        STOW     #$2300, NEXT_PAGE_TABLE
        STOW     #$2380, PREV_PAGE_TABLE
        LDY      #$00
        LDX      #$80       ; Max pages
    .loop_inc_dec_tables:
        LDA      #$00
        STA      (PAGE_L_TABLE),Y
        STA      (PAGE_H_TABLE),Y
        TYA
        CLC
        ADC      #$01
        STA      (NEXT_PAGE_TABLE),Y
        TYA
        SEC
        SBC      #$01
        STA      (PREV_PAGE_TABLE),Y
        INY
        DEX
        BNE      .loop_inc_dec_tables
        DEY
        LDA      #$FF
        STA      (NEXT_PAGE_TABLE),Y
        STOB     #$00, FIRST_Z_PAGE
        STOB     #$7F, LAST_Z_PAGE
        STOW     #$2C00, Z_HEADER_ADDR
        JSR      do_reset_window
    .read_z_image:
        MOVW     Z_HEADER_ADDR, SCRATCH2
        STOW     #$0000, SCRATCH1
        JSR      read_from_sector
        ; Historical note: The original Infocom source code did not check
        ; for an error here.
        BCC      .no_error
        JMP      main
    .no_error:
        LDY      #HEADER_HIMEM_BASE+1
        LDA      #$FF
        STA      (Z_HEADER_ADDR),Y      ; low byte of high memory base always FF.
        DEY
        LDA      (Z_HEADER_ADDR),Y      ; page
        STA      NUM_IMAGE_PAGES
        INC      NUM_IMAGE_PAGES
        LDA      #$00               ; sector = 0
    .read_another_sector:
        CLC                         ; ++sector
        ADC      #$01
        TAX
        ADC      Z_HEADER_ADDR+1    ; dest_addr = Z_HEADER_ADDR + 256*sector
        STA      SCRATCH2+1
        LDA      Z_HEADER_ADDR
        STA      SCRATCH2
        TXA
        CMP      NUM_IMAGE_PAGES
        BEQ      .check_debug_flag  ; done loading?
        PHA                         ; read_sector = sector
        STA      SCRATCH1
        LDA      #$00
        STA      SCRATCH1+1
        JSR      read_from_sector   ; read_from_sector(read_sector, dest_addr)
        ; Historical note: The original Infocom source code did not check
        ; for an error here.
        BCC      .no_error2
        JMP      main
    .no_error2:
        PLA
        JMP      .read_another_sector
    .check_debug_flag:
        LDY      #HEADER_FLAGS1
        LDA      (Z_HEADER_ADDR),Y
        AND      #$01
        EOR      #$01
        BEQ      .brk
    .store_initial_z_pc:
        LDY      #HEADER_INITIAL_ZPC+1
        LDA      (Z_HEADER_ADDR),Y
        STA      Z_PC
        DEY
        LDA      (Z_HEADER_ADDR),Y
        STA      Z_PC+1
        STOB     #$00, Z_PC+2
    .store_z_global_vars_addr:
        LDY      #HEADER_GLOBALVARS_ADDR+1
        LDA      (Z_HEADER_ADDR),Y
        STA      GLOBAL_ZVARS_ADDR
        DEY
        LDA      (Z_HEADER_ADDR),Y
        CLC
        ADC      Z_HEADER_ADDR+1
        STA      GLOBAL_ZVARS_ADDR+1
    .store_z_abbrev_table_addr:
        LDY      #HEADER_ABBREVS_ADDR+1
        LDA      (Z_HEADER_ADDR),Y
        STA      Z_ABBREV_TABLE
        DEY
        LDA      (Z_HEADER_ADDR),Y
        CLC
        ADC      Z_HEADER_ADDR+1
        STA      Z_ABBREV_TABLE+1
        STOB     #$00, HIGH_MEM_ADDR
        LDA      NUM_IMAGE_PAGES
        CLC
        ADC      Z_HEADER_ADDR+1
        STA      HIGH_MEM_ADDR+1
        JSR      locate_last_ram_page
        SEC
        SBC      HIGH_MEM_ADDR+1
        BCC      .brk
        TAY
        INY
        STY      NUM_PAGE_TABLE_ENTRIES
        TAY
        STY      LAST_Z_PAGE
        LDA      #$FF
        STA      (NEXT_PAGE_TABLE),Y
        JMP      do_instruction
    .brk:
        JSR      brk
    instr_restart:
        JSR      dump_buffer_with_more
        JMP      main
    locate_last_ram_page:
        MOVB     #$C0, SCRATCH2+1
        MOVB     #$FF, SCRATCH2
        LDY      #$00
    .loop:
        DEC      SCRATCH2+1
        LDA      (SCRATCH2),Y
        CMP      (SCRATCH2),Y
        BNE      .loop
        EOR      #$FF
        STA      (SCRATCH2),Y
        CMP      (SCRATCH2),Y
        BNE      .loop
        EOR      #$FF
        STA      (SCRATCH2),Y
        LDA      SCRATCH2+1
        RTS
    sEndOfSession:
        DC       "-- END OF SESSION --"
    instr_quit:
        JSR      dump_buffer_with_more
        STOW     sEndOfSession, SCRATCH2
        LDX      #20
        JSR      print_ascii_string
        JSR      dump_buffer_with_more
    .spinloop:
        JMP      .spinloop
    sInternalError:
        DC       "ZORK INTERNAL ERROR!"
    brk:
        BRK
*/
#include "zork.h"

/* The four tables of the page cache, PAGE_L_TABLE, PAGE_H_TABLE,
 * NEXT_PAGE_TABLE, and PREV_PAGE_TABLE, 128 bytes each. The linker
 * script starts them on a page, so that indexing one never crosses a
 * page. */
uint8_t page_tables [[c6502::section(".tables")]][512];

static const char sEndOfSession[20] = "-- END OF SESSION --";
static const char sInternalError[20] = "ZORK INTERNAL ERROR!";

/* Returns the page of the highest byte of RAM below $C000 that holds
 * what's written to it. */
static uint8_t locate_last_ram_page(void) {
    uint8_t page = 0xC0;
    for (;;) {
        page--;
        volatile uint8_t *p = (volatile uint8_t *)(((uint16_t)page << 8) | 0xFF);
        uint8_t v = *p;
        if (v != *p)
            continue;
        v ^= 0xFF;
        *p = v;
        if (v != *p)
            continue;
        *p = v ^ 0xFF;
        return page;
    }
}

[[c6502::section(".text.main")]]
int main(void) {
    PRINTER_CSW = 0xC100;
    ZCODE_PAGE_VALID = 0;
    ZCODE_PAGE_VALID2 = 0;
    PAGE_TABLE_INDEX = 0;
    PAGE_TABLE_INDEX2 = 0;
    BUFF_END = 0;
    for (uint8_t i = 0; i != 30; i++)
        LOCAL_ZVARS[i] = 0;
    FRAME_Z_SP = 0;
    FRAME_STACK_COUNT = 0;
    STACK_COUNT = 1;
    Z_SP = Z_STACK_TOP;
    ZCHAR_SCRATCH1[6] = 0xFF;

    /* The cache's slots start in order, each linked to the next, with
     * no page in them. */
    PAGE_L_TABLE = page_tables;
    PAGE_H_TABLE = page_tables + 0x80;
    NEXT_PAGE_TABLE = page_tables + 0x100;
    PREV_PAGE_TABLE = page_tables + 0x180;
    uint8_t y = 0;
    do {
        PAGE_L_TABLE[y] = 0;
        PAGE_H_TABLE[y] = 0;
        NEXT_PAGE_TABLE[y] = y + 1;
        PREV_PAGE_TABLE[y] = y - 1;
        y++;
    } while (y != 0x80);
    NEXT_PAGE_TABLE[0x7F] = 0xFF;
    FIRST_Z_PAGE = 0;
    LAST_Z_PAGE = 0x7F;
    Z_HEADER_ADDR = __story;
    reset_window();

    /* The header says how many pages to load: through the start of
     * high memory. */
    disk_sector = 0;
    disk_buffer = Z_HEADER_ADDR;
    if (read_from_sector())
        restart();
    Z_HEADER_ADDR[HEADER_HIMEM_BASE + 1] = 0xFF;
    NUM_IMAGE_PAGES = Z_HEADER_ADDR[HEADER_HIMEM_BASE] + 1;
    for (uint8_t sector = 1; sector != NUM_IMAGE_PAGES; sector++) {
        disk_sector = sector;
        disk_buffer = Z_HEADER_ADDR + ((uint16_t)sector << 8);
        if (read_from_sector())
            restart();
    }
    /* Bit 0 of the first flags byte is set only in a story the
     * interpreter can't run. */
    if (Z_HEADER_ADDR[HEADER_FLAGS1] & 0x01)
        internal_error();

    Z_PC.lo = ((uint16_t)Z_HEADER_ADDR[HEADER_INITIAL_ZPC] << 8)
        | Z_HEADER_ADDR[HEADER_INITIAL_ZPC + 1];
    Z_PC.hi = 0;
    GLOBAL_ZVARS_ADDR = Z_HEADER_ADDR
        + (((uint16_t)Z_HEADER_ADDR[HEADER_GLOBALVARS_ADDR] << 8)
           | Z_HEADER_ADDR[HEADER_GLOBALVARS_ADDR + 1]);
    Z_ABBREV_TABLE = Z_HEADER_ADDR
        + (((uint16_t)Z_HEADER_ADDR[HEADER_ABBREVS_ADDR] << 8)
           | Z_HEADER_ADDR[HEADER_ABBREVS_ADDR + 1]);

    /* The cache gets every page from the end of the loaded pages to
     * the top of RAM. */
    HIGH_MEM_ADDR = Z_HEADER_ADDR + ((uint16_t)NUM_IMAGE_PAGES << 8);
    uint8_t high_page = (uint8_t)((uint16_t)HIGH_MEM_ADDR >> 8);
    uint8_t last = locate_last_ram_page();
    if (last < high_page)
        internal_error();
    uint8_t slots = last - high_page;
    NUM_PAGE_TABLE_ENTRIES = slots + 1;
    LAST_Z_PAGE = slots;
    NEXT_PAGE_TABLE[slots] = 0xFF;

    for (;;)
        do_instruction();
}

/* Ends the line and starts over. */
void instr_restart(void) {
    dump_buffer_with_more();
    restart();
}

/* Prints "-- END OF SESSION --" on a line of its own and stops. */
void instr_quit(void) {
    dump_buffer_with_more();
    print_ascii_string(sEndOfSession, 20);
    dump_buffer_with_more();
    for (;;) {
    }
}

/* Prints "ZORK INTERNAL ERROR!" on a line of its own and stops. */
void internal_error(void) {
    dump_buffer_with_more();
    print_ascii_string(sInternalError, 20);
    dump_buffer_with_more();
    for (;;) {
    }
}
