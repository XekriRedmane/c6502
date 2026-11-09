#include <stdint.h>

/*
    find_index_of_page_table, from the Zork I (r15) Z-machine
    interpreter for the Apple II, as reverse-engineered in
    https://github.com/XekriRedmane/zorki-r15-reveng (main.nw,
    "Find index of page table").

    The interpreter keeps up to NUM_PAGE_TABLE_ENTRIES 256-byte pages
    of the story file in RAM. Two parallel tables record which Z-page
    is in each slot: PAGE_L_TABLE holds the low byte of the Z-page
    number and PAGE_H_TABLE the high byte. The tables sit at fixed
    addresses ($2200 / $2280), set once at startup, so they're
    `static uint8_t * const` pointers here.

    The original:

        LDX  NUM_PAGE_TABLE_ENTRIES
        LDY  #$00
        LDA  SCRATCH2               ; low byte of the page sought
    .loop:
        CMP  (PAGE_L_TABLE),Y
        BNE  .next
        LDA  SCRATCH2+1
        CMP  (PAGE_H_TABLE),Y
        BEQ  .found
        LDA  SCRATCH2
    .next:
        INY
        DEX
        BNE  .loop
        LDA  LAST_Z_PAGE
        SEC
        RTS
    .found:
        TYA
        CLC
        RTS

    It reads the page from the zero-page word SCRATCH2 (here the
    zp_abi parameter `page`) and returns the slot index in A, with
    the carry clear when found and set (and A = LAST_Z_PAGE) when
    not. C can't return a flag in the carry, so this returns a
    16-bit value whose low byte (in A) is the index or last_z_page
    and whose high byte (in X) is 1 when the page wasn't found.

    The loop counts X down from NUM_PAGE_TABLE_ENTRIES and tests at
    the bottom, so with zero entries it runs 256 times. The do/while
    with a down-counter keeps exactly that behavior.

    Under --optimize this compiles to the same 18 instructions and
    loop as the original: the key's low byte stays in A for the whole
    loop (passes/loop_invariant_a.py) and the counter lives in X
    (passes/counter_to_index_reg.py). The compare is `CMP $2200,Y`
    rather than `CMP (zp),Y`, since the tables are constants here,
    so it's one cycle per entry faster.
*/

static uint8_t * const page_l_table = (uint8_t * const)0x2200;
static uint8_t * const page_h_table = (uint8_t * const)0x2280;

uint8_t num_page_table_entries;
uint8_t last_z_page;

uint16_t find_index_of_page_table(uint16_t page) {
    uint8_t lo = (uint8_t)page;
    uint8_t hi = (uint8_t)(page >> 8);
    uint8_t i = 0;
    uint8_t count = num_page_table_entries;
    do {
        if (page_l_table[i] == lo && page_h_table[i] == hi) {
            return i;
        }
        i++;
    } while (--count != 0);
    return 0x100 | last_z_page;
}

int main(void) {
    num_page_table_entries = 3;
    last_z_page = 2;
    page_l_table[0] = 0x10; page_h_table[0] = 0x00;
    page_l_table[1] = 0x20; page_h_table[1] = 0x01;
    page_l_table[2] = 0x30; page_h_table[2] = 0x00;
    /* Found at index 1, then not found (0x100 | 2). */
    return (int)(find_index_of_page_table(0x0120)
                 + find_index_of_page_table(0x0130));
}
