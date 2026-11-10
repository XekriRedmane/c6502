#include <stdint.h>

/*
    match_dictionary_word, from the Zork I (r15) Z-machine interpreter
    for the Apple II, as reverse-engineered in
    https://github.com/XekriRedmane/zorki-r15-reveng (main.nw,
    "Searching the dictionary").

    The story file's header holds the dictionary's offset, big-endian,
    at bytes $08-$09. The dictionary starts with a count of word-
    separator characters and the separators themselves, then the
    length of an entry, then the number of entries (big-endian), then
    the entries, sorted by their first four bytes. Those four bytes are
    a word's encoded z-characters, two big-endian words.

    The word to look up is in ZCHAR_SCRATCH2 as two little-endian
    words, so its bytes in dictionary order are [1], [0], [3], [2]. The
    routine skips ahead 16 entries at a time while an entry's first
    byte is below the word's, backs up 16, and then compares entries
    one at a time. It returns the matching entry's z-address (its
    offset from the header), or 0 when the word isn't there.

    The original, with its ADDA / ADDB2 / SUBB2 / SUBB / SUBW macros
    expanded (SCRATCH1 $E4, SCRATCH2 $E6, SCRATCH3 $E8, ZCHAR_SCRATCH2
    $DA, Z_HEADER_ADDR $BA):

    get_dictionary_addr:
        LDY  #HEADER_DICT_ADDR
        LDA  (Z_HEADER_ADDR),Y
        STA  SCRATCH2+1
        INY
        LDA  (Z_HEADER_ADDR),Y
        STA  SCRATCH2
        CLC                         ; ADDW SCRATCH2, Z_HEADER_ADDR, SCRATCH2
        LDA  SCRATCH2
        ADC  Z_HEADER_ADDR
        STA  SCRATCH2
        LDA  SCRATCH2+1
        ADC  Z_HEADER_ADDR+1
        STA  SCRATCH2+1
        RTS

    match_dictionary_word:
        JSR  get_dictionary_addr
        LDY  #$00                   ; number of dict separators
        LDA  (SCRATCH2),Y
        TAY                         ; skip past and get entry length
        INY
        LDA  (SCRATCH2),Y
        ASL                         ; search_size = entry length x 16
        ASL
        ASL
        ASL
        STA  SCRATCH3
        INY                         ; entry_index = num dict entries
        LDA  (SCRATCH2),Y
        STA  SCRATCH1+1
        INY
        LDA  (SCRATCH2),Y
        STA  SCRATCH1
        INY
        TYA
        CLC                         ; ADDA SCRATCH2
        ADC  SCRATCH2
        STA  SCRATCH2
        BCC  .c1
        INC  SCRATCH2+1
    .c1:
        LDY  #$00
        JMP  .try_match
    .loop:
        LDA  (SCRATCH2),Y
        CMP  ZCHAR_SCRATCH2+1
        BCS  .possible
    .try_match:
        CLC                         ; ADDB2 SCRATCH2, SCRATCH3
        LDA  SCRATCH2
        ADC  SCRATCH3
        STA  SCRATCH2
        BCC  .c2
        INC  SCRATCH2+1
    .c2:
        SEC                         ; entry_index -= 16
        LDA  SCRATCH1
        SBC  #$10
        STA  SCRATCH1
        BCS  .loop
        DEC  SCRATCH1+1
        BPL  .loop
    .possible:
        SEC                         ; SUBB2 SCRATCH2, SCRATCH3
        LDA  SCRATCH2
        SBC  SCRATCH3
        STA  SCRATCH2
        BCS  .c3
        DEC  SCRATCH2+1
    .c3:
        CLC                         ; ADDB2 SCRATCH1, #$10
        LDA  SCRATCH1
        ADC  #$10
        STA  SCRATCH1
        BCC  .c4
        INC  SCRATCH1+1
    .c4:
        LDA  SCRATCH3               ; search_size /= 16
        LSR
        LSR
        LSR
        LSR
        STA  SCRATCH3
    .inner_loop:
        LDY  #$00
        LDA  ZCHAR_SCRATCH2+1
        CMP  (SCRATCH2),Y
        BCC  .not_found
        BNE  .inner_next
        INY
        LDA  ZCHAR_SCRATCH2
        CMP  (SCRATCH2),Y
        BCC  .not_found
        BNE  .inner_next
        LDY  #$02
        LDA  ZCHAR_SCRATCH2+3
        CMP  (SCRATCH2),Y
        BCC  .not_found
        BNE  .inner_next
        INY
        LDA  ZCHAR_SCRATCH2+2
        CMP  (SCRATCH2),Y
        BCC  .not_found
        BEQ  .found
    .inner_next:
        CLC                         ; ADDB2 SCRATCH2, SCRATCH3
        LDA  SCRATCH2
        ADC  SCRATCH3
        STA  SCRATCH2
        BCC  .c5
        INC  SCRATCH2+1
    .c5:
        LDA  SCRATCH1               ; SUBB SCRATCH1, #$01
        SEC
        SBC  #$01
        STA  SCRATCH1
        BCS  .c6
        DEC  SCRATCH1+1
    .c6:
        LDA  SCRATCH1
        ORA  SCRATCH1+1
        BNE  .inner_loop
    .not_found:
        LDA  #$00
        STA  SCRATCH1+1
        STA  SCRATCH1
        RTS
    .found:
        SEC                         ; SUBW SCRATCH2, Z_HEADER_ADDR, SCRATCH1
        LDA  SCRATCH2
        SBC  Z_HEADER_ADDR
        STA  SCRATCH1
        LDA  SCRATCH2+1
        SBC  Z_HEADER_ADDR+1
        STA  SCRATCH1+1
        RTS

    It returns the z-address in SCRATCH1. Here it's the return value,
    which comes back in A (low) and X (high). The original keeps the
    header pointer and the word in zero page, at $BA and $DA, and
    `z_header_addr` and `zchar_scratch2` are placed at the same
    addresses.

    Like the original, the linear scan compares an entry before it
    counts down, so a dictionary with no entries isn't handled.
*/

#define HEADER_DICT_ADDR 0x08

uint8_t *z_header_addr __attribute__((zeropage(0xBA)));
uint8_t zchar_scratch2[6] __attribute__((zeropage(0xDA)));

uint8_t *get_dictionary_addr(void) {
    uint16_t offset = ((uint16_t)z_header_addr[HEADER_DICT_ADDR] << 8)
                      | z_header_addr[HEADER_DICT_ADDR + 1];
    return z_header_addr + offset;
}

uint16_t match_dictionary_word(void) {
    uint8_t *dict = get_dictionary_addr();
    uint8_t n_separators = dict[0];
    uint8_t entry_len = dict[n_separators + 1];
    uint8_t search_size = entry_len << 4;
    int16_t entry_index = (int16_t)(((uint16_t)dict[n_separators + 2] << 8)
                                    | dict[n_separators + 3]);
    uint8_t *entry = dict + n_separators + 4;

    /* Skip 16 entries at a time while the entry's first byte is below
       the word's. */
    for (;;) {
        entry += search_size;
        entry_index -= 16;
        if (entry_index < 0 || entry[0] >= zchar_scratch2[1])
            break;
    }
    entry -= search_size;
    entry_index += 16;

    /* Compare entries one at a time. The dictionary is sorted, so an
       entry above the word means the word isn't there. */
    do {
        if (zchar_scratch2[1] < entry[0]) return 0;
        if (zchar_scratch2[1] == entry[0]) {
            if (zchar_scratch2[0] < entry[1]) return 0;
            if (zchar_scratch2[0] == entry[1]) {
                if (zchar_scratch2[3] < entry[2]) return 0;
                if (zchar_scratch2[3] == entry[2]) {
                    if (zchar_scratch2[2] < entry[3]) return 0;
                    if (zchar_scratch2[2] == entry[3])
                        return (uint16_t)(entry - z_header_addr);
                }
            }
        }
        entry += entry_len;
    } while (--entry_index != 0);
    return 0;
}

static uint8_t story[0x200];

int main(void) {
    /* A header at story[0] with the dictionary at offset $0040: three
       separators, 7-byte entries, and 20 entries whose first four
       bytes are 00 00 00 05, 00 00 00 0A, and so on. */
    uint8_t *d = story + 0x40;
    uint8_t i;
    z_header_addr = story;
    story[HEADER_DICT_ADDR] = 0x00;
    story[HEADER_DICT_ADDR + 1] = 0x40;
    d[0] = 3;
    d[1] = '.';
    d[2] = ',';
    d[3] = '"';
    d[4] = 7;
    d[5] = 0;
    d[6] = 20;
    for (i = 0; i < 20; i++)
        d[7 + 7 * i + 3] = (uint8_t)(5 * (i + 1));
    /* Look up entry 17's word, 00 00 00 55. */
    zchar_scratch2[0] = 0x00;
    zchar_scratch2[1] = 0x00;
    zchar_scratch2[2] = 0x55;
    zchar_scratch2[3] = 0x00;
    /* Its z-address is $0040 + 7 + 7 * 16. */
    return (int)match_dictionary_word();
}
