/*
    zstring.c -- printing a z-string.

    The Z-machine stores text as z-strings. Each 16-bit word holds
    three 5-bit z-characters, and the top bit of the last word marks
    the end of the string. A z-character picks a letter from one of
    three alphabets, shifts to another alphabet for one character or
    until further notice, or names an abbreviation, which is itself a
    z-string, printed in place. print_string_literal prints the
    z-string that follows the current instruction, at the program
    counter Z_PC, and leaves Z_PC just past it.

    Departures from the original:

    - get_next_zchar returns its z-character in A and "end of string"
      in the carry. It returns a struct of the value and a bool,
      declared return_bool_in_carry, which uses the same registers.
    - print_zstring keeps the z-character in a local where the
      original uses SCRATCH3, and saves the decoder's state around the
      abbreviation's call with `#pragma c6502 call save(hardware_stack)`,
      which pushes it with PHA as the original does.

    Zork I (r15), output/main.asm lines 2598-2809 and, for
    print_string_literal, 908-921:

    print_zstring:
        LDA      #$00
        STA      LOCKED_ALPHABET
        STA      ZDECOMPRESS_STATE
        STOB     #$FF, SHIFT_ALPHABET
    .loop:
        JSR      get_next_zchar
        BCC      .not_end
        RTS
    .not_end:
        STA      SCRATCH3
        BEQ      .space                 ; z-char 0?
        CMP      #$01
        BEQ      .abbreviation          ; z-char 1?
        CMP      #$04
        BCC      .shift_alphabet        ; z-char 2 or 3?
        CMP      #$06
        BCC      .shift_lock_alphabet   ; z-char 4 or 5?
        JSR      get_alphabet
        ; fall through to print the z-character
        ORA      #$00
        BNE      .check_for_alphabet_A1
        LDA      #$5B
    .add_ascii_offset:
        CLC
        ADC      SCRATCH3
    .printchar:
        JSR      buffer_char
        JMP      .loop
    .check_for_alphabet_A1:
        CMP      #$01
        BNE      .map_ascii_for_A2
        LDA      #$3B
        JMP      .add_ascii_offset
    .map_ascii_for_A2:
        LDA      SCRATCH3
        SEC
        SBC      #$07
        BCC      .z10bits
        BEQ      .crlf
        TAY
        DEY
        LDA      a2_table,Y
        JMP      .printchar
    .z10bits:
        JSR      get_next_zchar
        ASL
        ASL
        ASL
        ASL
        ASL
        PHA
        JSR      get_next_zchar
        STA      SCRATCH3
        PLA
        ORA      SCRATCH3
        JMP      .printchar
    .space:
        LDA      #$20
        JMP      .printchar
    .crlf:
        LDA      #$0D
        JSR      buffer_char
        LDA      #$0A
        JMP      .printchar
    .shift_alphabet:
        JSR      get_alphabet
        CLC
        ADC      #$02
        ADC      SCRATCH3
        JSR      A_mod_3
        STA      SHIFT_ALPHABET
        JMP      .loop
    .shift_lock_alphabet:
        JSR      get_alphabet
        CLC
        ADC      SCRATCH3
        JSR      A_mod_3
        STA      LOCKED_ALPHABET
        JMP      .loop
    .abbreviation:
        JSR      get_next_zchar
        ASL
        ADC      #$01
        TAY
        LDA      (Z_ABBREV_TABLE),Y
        STA      SCRATCH2
        DEY
        LDA      (Z_ABBREV_TABLE),Y
        STA      SCRATCH2+1
        ; Save the decompress state
        LDA      LOCKED_ALPHABET
        PHA
        LDA      ZDECOMPRESS_STATE
        PHA
        LDA      ZCHARS_L
        PHA
        LDA      ZCHARS_H
        PHA
        LDA      Z_PC2_L
        PHA
        LDA      Z_PC2_H
        PHA
        LDA      Z_PC2_HH
        PHA
        JSR      load_packed_address
        JSR      print_zstring
        ; Restore the decompress state
        PLA
        STA      Z_PC2_HH
        PLA
        STA      Z_PC2_H
        PLA
        STA      Z_PC2_L
        LDA      #$00
        STA      ZCODE_PAGE_VALID2
        PLA
        STA      ZCHARS_H
        PLA
        STA      ZCHARS_L
        PLA
        STA      ZDECOMPRESS_STATE
        PLA
        STA      LOCKED_ALPHABET
        STOB     #$FF, SHIFT_ALPHABET ; Resets any temporary shift
        JMP      .loop
    A_mod_3:
        CMP      #$03
        BCC      .end
        SEC
        SBC      #$03
        JMP      A_mod_3
    .end:
        RTS
    a2_table:
        DC       "0123456789.,!?_#'"
        DC       '"
        DC       "/\-:()"
    get_alphabet:
        LDA      SHIFT_ALPHABET
        BPL      .remove_shift
        LDA      LOCKED_ALPHABET
        RTS
    .remove_shift:
        LDY      #$FF
        STY      SHIFT_ALPHABET
        RTS
    get_next_zchar:
        LDA      ZDECOMPRESS_STATE
        BPL      .check_for_char_1
        SEC
        RTS
    .check_for_char_1:
        BNE      .check_for_char_2
        INC      ZDECOMPRESS_STATE
        JSR      get_next_code_word
        MOVW     SCRATCH2, ZCHARS_L
        LDA      ZCHARS_H
        LSR
        LSR
        AND      #$1F
        CLC
        RTS
    .check_for_char_2:
        SEC
        SBC      #$01
        BNE      .check_for_last
        STOB     #$02, ZDECOMPRESS_STATE
        LDA      ZCHARS_H
        LSR
        LDA      ZCHARS_L
        ROR
        TAY
        LDA      ZCHARS_H
        LSR
        LSR
        TYA
        ROR
        LSR
        LSR
        LSR
        AND      #$1F
        CLC
        RTS
    .check_for_last:
        STOB     #$00, ZDECOMPRESS_STATE
        LDA      ZCHARS_H
        BPL      .get_char_3
        STOB     #$FF, ZDECOMPRESS_STATE
    .get_char_3:
        LDA      ZCHARS_L
        AND      #$1F
        CLC
        RTS
    print_string_literal:
        MOVB     Z_PC, Z_PC2_L
        MOVB     Z_PC+1, Z_PC2_H
        MOVB     Z_PC+2, Z_PC2_HH
        STOB     #$00, ZCODE_PAGE_VALID2
        JSR      print_zstring
        MOVB     Z_PC2_L, Z_PC
        MOVB     Z_PC2_H, Z_PC+1
        MOVB     Z_PC2_HH, Z_PC+2
        MOVB     ZCODE_PAGE_VALID2, ZCODE_PAGE_VALID
        MOVW     ZCODE_PAGE_ADDR2, ZCODE_PAGE_ADDR
        RTS
*/
#include "zork.h"

/* Alphabet 2's characters for z-characters 8 to 31. */
static const uint8_t a2_table[24] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',',
    '!', '?', '_', '#', '\'', '"', '/', '\\', '-', ':', '(', ')',
};

/* Prints the z-string at Z_PC2. An abbreviation prints another
 * z-string in the middle of this one, so this calls itself. */
void print_zstring(void) {
    LOCKED_ALPHABET = 0;
    ZDECOMPRESS_STATE = 0;
    SHIFT_ALPHABET = 0xFF;
    for (;;) {
        struct zchar c = get_next_zchar();
        if (c.end)
            return;
        uint8_t z = c.code;
        uint8_t ch;
        if (z == 0) {
            ch = ' ';
        } else if (z == 1) {
            /* An abbreviation: the table holds its packed address. */
            uint8_t i = get_next_zchar().code << 1;
            SCRATCH2 = ((uint16_t)Z_ABBREV_TABLE[i] << 8)
                | Z_ABBREV_TABLE[i + 1];
            uint8_t locked = LOCKED_ALPHABET;
            uint8_t state = ZDECOMPRESS_STATE;
            uint16_t zchars = ZCHARS;
            uint8_t pc_l = Z_PC2_L;
            uint8_t pc_h = Z_PC2_H;
            uint8_t pc_hh = Z_PC2_HH;
            load_packed_address();
            /* Abbreviations don't nest, so this goes two deep. */
#pragma c6502 call save(hardware_stack)
            print_zstring();
            Z_PC2_HH = pc_hh;
            Z_PC2_H = pc_h;
            Z_PC2_L = pc_l;
            ZCODE_PAGE_VALID2 = 0;
            ZCHARS = zchars;
            ZDECOMPRESS_STATE = state;
            LOCKED_ALPHABET = locked;
            SHIFT_ALPHABET = 0xFF;
            continue;
        } else if (z < 4) {
            SHIFT_ALPHABET = A_mod_3(get_alphabet() + 2 + z);
            continue;
        } else if (z < 6) {
            LOCKED_ALPHABET = A_mod_3(get_alphabet() + z);
            continue;
        } else {
            uint8_t alphabet = get_alphabet();
            if (alphabet == 0) {
                ch = 0x5B + z;                  /* 'a' for z = 6 */
            } else if (alphabet == 1) {
                ch = 0x3B + z;                  /* 'A' for z = 6 */
            } else if (z == 6) {
                /* A 10-bit character code in the next two z-chars. */
                uint8_t hi = get_next_zchar().code << 5;
                ch = hi | get_next_zchar().code;
            } else if (z == 7) {
                buffer_char(0x0D);
                ch = 0x0A;
            } else {
                ch = a2_table[z - 8];
            }
        }
        buffer_char(ch);
    }
}

uint8_t A_mod_3(register uint8_t a) {
    while (a >= 3)
        a -= 3;
    return a;
}

/* Returns the alphabet for the next character: the shifted one, which
 * is then used up, or else the locked one. */
uint8_t get_alphabet(void) {
    uint8_t shift = SHIFT_ALPHABET;
    if (shift & 0x80)
        return LOCKED_ALPHABET;
    SHIFT_ALPHABET = 0xFF;
    return shift;
}

/* Returns the next z-character of the string at Z_PC2, or `end` once
 * the word with the end bit has been used up. */
[[c6502::return_bool_in_carry]] struct zchar get_next_zchar(void) {
    uint8_t code;
    uint8_t state = ZDECOMPRESS_STATE;
    if (state & 0x80)
        return (struct zchar){ state, true };
    if (state == 0) {
        ZDECOMPRESS_STATE = 1;
        get_next_code_word();
        ZCHARS = SCRATCH2;
        code = (uint8_t)(ZCHARS >> 10) & 0x1F;
    } else if (state == 1) {
        ZDECOMPRESS_STATE = 2;
        code = (uint8_t)(ZCHARS >> 5) & 0x1F;
    } else {
        ZDECOMPRESS_STATE = (ZCHARS & 0x8000) ? 0xFF : 0x00;
        code = (uint8_t)ZCHARS & 0x1F;
    }
    return (struct zchar){ code, false };
}

/* Prints the z-string at Z_PC and moves Z_PC past it. The instruction
 * reader's page stays mapped as the string reader left it. Zork keeps
 * this with its instruction handlers, so it has a section of its own
 * for the linker script to place there. */
[[c6502::section(".text.literal")]]
void print_string_literal(void) {
    Z_PC2_L = Z_PC[0];
    Z_PC2_H = Z_PC[1];
    Z_PC2_HH = Z_PC[2];
    ZCODE_PAGE_VALID2 = 0;
    print_zstring();
    Z_PC[0] = Z_PC2_L;
    Z_PC[1] = Z_PC2_H;
    Z_PC[2] = Z_PC2_HH;
    ZCODE_PAGE_VALID = ZCODE_PAGE_VALID2;
    ZCODE_PAGE_ADDR = ZCODE_PAGE_ADDR2;
}
