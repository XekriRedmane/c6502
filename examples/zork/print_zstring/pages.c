/*
    pages.c -- reading the story file through the page cache.

    The story file is read in 256-byte pages. Pages below
    NUM_IMAGE_PAGES were loaded at startup, and the rest go through a
    cache of pages in high memory, kept as a list ordered by use. The
    string reader keeps its own copy of the program counter, Z_PC2, and
    its own page pointer, ZCODE_PAGE_ADDR2, so that reading a string
    doesn't disturb the instruction reader.

    Departures from the original:

    - find_index_of_page_table returns a slot in A and "not found" in
      the carry. It returns a struct of the value and a bool, declared
      return_bool_in_carry, which uses the same registers.
    - When a page can't be read, the original jumps to `main`, which
      restarts the interpreter. Here that's a call to restart(), which
      is `main`'s address and doesn't return.
    - set_page_first keeps the slot's neighbors in locals where the
      original uses SCRATCH2. Nothing reads SCRATCH2 after it.

    Zork I (r15), output/main.asm lines 2431-2597:

    invalidate_zcode_page2:
        STOB     #$00, ZCODE_PAGE_VALID2
        RTS
    load_packed_address:
        LDA      SCRATCH2
        ASL
        STA      Z_PC2_L
        LDA      SCRATCH2+1
        ROL
        STA      Z_PC2_H
        LDA      #$00
        ROL
        STA      Z_PC2_HH
        JMP      invalidate_zcode_page2
    get_next_code_word:
        JSR      get_next_code_byte2
        PHA
        JSR      get_next_code_byte2
        STA      SCRATCH2
        PLA
        STA      SCRATCH2+1
        RTS
    get_next_code_byte2:
        LDA      ZCODE_PAGE_VALID2
        BEQ      .zcode_page_invalid
        LDY      Z_PC2_L
        LDA      (ZCODE_PAGE_ADDR2),Y
        INY
        STY      Z_PC2_L
        BEQ      .invalidate_zcode_page
        RTS
    .invalidate_zcode_page:
        LDY      #$00
        STY      ZCODE_PAGE_VALID2
        INC      Z_PC2_H
        BNE      .end
        INC      Z_PC2_HH
    .end:
        RTS
    .zcode_page_invalid:
        LDA      Z_PC2_HH
        BNE      .find_pc_page_in_page_table
        LDA      Z_PC2_H
        CMP      NUM_IMAGE_PAGES
        BCC      .set_page_addr
    .find_pc_page_in_page_table:
        MOVW     Z_PC2_H, SCRATCH2
        JSR      find_index_of_page_table
        STA      PAGE_TABLE_INDEX2
        BCS      .not_found_in_page_table
    .set_page_first:
        JSR      set_page_first
        CLC
        LDA      PAGE_TABLE_INDEX2
        ADC      NUM_IMAGE_PAGES
    .set_page_addr:
        CLC
        ADC      Z_HEADER_ADDR+1
        STA      ZCODE_PAGE_ADDR2+1
        STOB     #$00, ZCODE_PAGE_ADDR2
        STOB     #$FF, ZCODE_PAGE_VALID2
        JMP      get_next_code_byte2
    .not_found_in_page_table:
        CMP      PAGE_TABLE_INDEX
        BNE      .read_from_disk
        STOB     #$00, ZCODE_PAGE_VALID
    .read_from_disk:
        MOVW     HIGH_MEM_ADDR, SCRATCH2
        LDA      PAGE_TABLE_INDEX2
        CLC
        ADC      SCRATCH2+1
        STA      SCRATCH2+1
        MOVW     Z_PC2_H, SCRATCH1
        JSR      read_from_sector
        BCC      .good_read
        JMP      main
    .good_read:
        LDY      PAGE_TABLE_INDEX2
        LDA      Z_PC2_H
        STA      (PAGE_L_TABLE),Y
        LDA      Z_PC2_HH
        STA      (PAGE_H_TABLE),Y
        TYA
        JMP      .set_page_first
    set_page_first:
        CMP      FIRST_Z_PAGE
        BEQ      .end
        LDX      FIRST_Z_PAGE           ; prev_first = FIRST_Z_PAGE
        STA      FIRST_Z_PAGE           ; FIRST_Z_PAGE = A
        TAY                             ; SCRATCH2L = NEXT_PAGE_TABLE[FIRST_Z_PAGE]
        LDA      (NEXT_PAGE_TABLE),Y
        STA      SCRATCH2
        TXA                             ; NEXT_PAGE_TABLE[FIRST_Z_PAGE] = prev_first
        STA      (NEXT_PAGE_TABLE),Y
        LDA      (PREV_PAGE_TABLE),Y    ; SCRATCH2H = PREV_PAGE_TABLE[FIRST_Z_PAGE]
        STA      SCRATCH2+1
        LDA      #$FF                   ; PREV_PAGE_TABLE[FIRST_Z_PAGE] = #$FF
        STA      (PREV_PAGE_TABLE),Y
        LDY      SCRATCH2+1
        LDA      SCRATCH2
        STA      (NEXT_PAGE_TABLE),Y    ; NEXT_PAGE_TABLE[SCRATCH2H] = SCRATCH2L
        TXA
        TAY
        LDA      FIRST_Z_PAGE
        STA      (PREV_PAGE_TABLE),Y    ; PREV_PAGE_TABLE[prev_first] = FIRST_Z_PAGE
        LDA      SCRATCH2
        CMP      #$FF
        BEQ      .set_last_z_page
        TAY
        LDA      SCRATCH2+1
        STA      (PREV_PAGE_TABLE),Y    ; PREV_PAGE_TABLE[SCRATCH2L] = SCRATCH2H
    .end:
        RTS
    .set_last_z_page:
        MOVB     SCRATCH2+1, LAST_Z_PAGE   ; LAST_Z_PAGE = SCRATCH2H
        RTS
    find_index_of_page_table:
        LDX      NUM_PAGE_TABLE_ENTRIES
        LDY      #$00
        LDA      SCRATCH2
    .loop:
        CMP      (PAGE_L_TABLE),Y
        BNE      .next
        LDA      SCRATCH2+1
        CMP      (PAGE_H_TABLE),Y
        BEQ      .found
        LDA      SCRATCH2
    .next:
        INY
        DEX
        BNE      .loop
        LDA      LAST_Z_PAGE
        SEC
        RTS
    .found:
        TYA
        CLC
        RTS
*/
#include "zork.h"

void invalidate_zcode_page2(void) {
    ZCODE_PAGE_VALID2 = 0;
}

/* Sets Z_PC2 to the byte address of the packed (word) address in
 * SCRATCH2, and unmaps the string reader's page. */
void load_packed_address(void) {
    Z_PC2_L = (uint8_t)(SCRATCH2 << 1);
    Z_PC2_H = (uint8_t)(SCRATCH2 >> 7);
    Z_PC2_HH = (uint8_t)(SCRATCH2 >> 15);
    invalidate_zcode_page2();
}

/* Reads the big-endian word at Z_PC2 into SCRATCH2. */
void get_next_code_word(void) {
    uint8_t hi = get_next_code_byte2();
    uint8_t lo = get_next_code_byte2();
    SCRATCH2 = ((uint16_t)hi << 8) | lo;
}

/* Returns the byte at Z_PC2 and advances Z_PC2. When Z_PC2 crosses
 * into a page that isn't mapped, maps it first, reading it from disk
 * into the least recently used cache slot if it isn't cached. */
uint8_t get_next_code_byte2(void) {
    for (;;) {
        if (ZCODE_PAGE_VALID2) {
            uint8_t b = ZCODE_PAGE_ADDR2[Z_PC2_L];
            if (++Z_PC2_L == 0) {
                ZCODE_PAGE_VALID2 = 0;
                if (++Z_PC2_H == 0)
                    Z_PC2_HH++;
            }
            return b;
        }
        uint8_t page;
        if (Z_PC2_HH == 0 && Z_PC2_H < NUM_IMAGE_PAGES) {
            page = Z_PC2_H;
        } else {
            SCRATCH2 = Z_PC2_H | ((uint16_t)Z_PC2_HH << 8);
            struct page_slot found = find_index_of_page_table();
            PAGE_TABLE_INDEX2 = found.slot;
            if (found.not_found) {
                /* The instruction reader's page is about to go. */
                if (found.slot == PAGE_TABLE_INDEX)
                    ZCODE_PAGE_VALID = 0;
                SCRATCH2 = HIGH_MEM_ADDR
                    + ((uint16_t)PAGE_TABLE_INDEX2 << 8);
                SCRATCH1 = Z_PC2_H | ((uint16_t)Z_PC2_HH << 8);
                if (read_from_sector())
                    restart();
                PAGE_L_TABLE[PAGE_TABLE_INDEX2] = Z_PC2_H;
                PAGE_H_TABLE[PAGE_TABLE_INDEX2] = Z_PC2_HH;
            }
            set_page_first(PAGE_TABLE_INDEX2);
            page = PAGE_TABLE_INDEX2 + NUM_IMAGE_PAGES;
        }
        uint8_t high = page + (uint8_t)((uint16_t)Z_HEADER_ADDR >> 8);
        ZCODE_PAGE_ADDR2 = (uint8_t *)((uint16_t)high << 8);
        ZCODE_PAGE_VALID2 = 0xFF;
    }
}

/* Moves cache slot `slot` to the front of the list ordered by use.
 * $FF ends the list in both directions. */
void set_page_first(register uint8_t slot) {
    if (slot == FIRST_Z_PAGE)
        return;
    uint8_t prev_first = FIRST_Z_PAGE;
    FIRST_Z_PAGE = slot;
    uint8_t next = NEXT_PAGE_TABLE[slot];
    NEXT_PAGE_TABLE[slot] = prev_first;
    uint8_t prev = PREV_PAGE_TABLE[slot];
    PREV_PAGE_TABLE[slot] = 0xFF;
    NEXT_PAGE_TABLE[prev] = next;
    PREV_PAGE_TABLE[prev_first] = FIRST_Z_PAGE;
    if (next == 0xFF)
        LAST_Z_PAGE = prev;
    else
        PREV_PAGE_TABLE[next] = prev;
}

/* Finds the cache slot holding story page SCRATCH2. If no slot holds
 * it, returns the least recently used slot, LAST_Z_PAGE, with
 * not_found set. */
[[c6502::return_bool_in_carry]]
struct page_slot find_index_of_page_table(void) {
    uint8_t lo = (uint8_t)SCRATCH2;
    uint8_t hi = (uint8_t)(SCRATCH2 >> 8);
    uint8_t i = 0;
    uint8_t n = NUM_PAGE_TABLE_ENTRIES;
    do {
        if (PAGE_L_TABLE[i] == lo && PAGE_H_TABLE[i] == hi)
            return (struct page_slot){ i, false };
        i++;
    } while (--n != 0);
    return (struct page_slot){ LAST_Z_PAGE, true };
}
