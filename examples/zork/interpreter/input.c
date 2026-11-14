/*
    input.c -- the sread instruction: the status line, reading a line,
    and splitting it into words looked up in the dictionary.

    sread's text buffer holds its size in its first byte and the line,
    in lower case, from the second on. The parse buffer holds the
    largest number of words in its first byte and the number found in
    its second, and then four bytes for each word: the address of its
    dictionary entry (0 if it isn't there), its length, and its offset
    in the text buffer. Words are separated by spaces and the other
    characters in SEPARATORS_TABLE, which aren't words, and by the
    dictionary's own separators, which are words of one character.

    A word is looked up by its first six characters, encoded as six
    z-characters in two words, as the dictionary keeps it. The
    dictionary starts with its count of separators and the separators,
    then the length of an entry, then the number of entries, sorted.

    Departures from the original:

    - The text and parse buffers are pointers of their own, where the
      original turns OPERAND0 and OPERAND1 into them. The counts of
      input and word characters, the offset into the text buffer, and
      the parse buffer's next entry are file-scope variables, where the
      original keeps them in SCRATCH3+1, SCRATCH3, INPUT_PTR, and
      TOKEN_IDX. The dictionary's address, the entry found, and
      ascii_to_zchar's and match_dictionary_word's counts and indexes
      are arguments, return values, and locals, where the original
      keeps them in SCRATCH1, SCRATCH2, and SCRATCH3.
    - The separator tests return a bool where the original returns the
      carry.
    - GETLN1 returns the line's length in X, which C can't read. The
      line ends with the $8D that GETLN1 stores after it, so read_line
      finds the length by looking for that byte. The original stores
      $8D there again; here it's already there.
    - search_nonalpha_table looks for a character among alphabet 2's 24
      characters. The original's loop starts at index $24, so it also
      compares the 13 bytes after a2_table, which are get_alphabet's
      code, and a character equal to one of them ($03, $10, and '`'
      can be typed) encodes as a z-character above 31.

    Zork I (r15), output/main.asm lines 1741-2019, 2810-3024,
    3234-3281, 3295-3347:

    instr_sread:
        JSR      print_status_line
        ADDW     OPERAND0, Z_HEADER_ADDR, OPERAND0  ; text buffer
        ADDW     OPERAND1, Z_HEADER_ADDR, OPERAND1  ; parse buffer
        JSR      read_line      ; SCRATCH3H = read_line() (input_count)
        STA      SCRATCH3+1
        STOB     #$00, SCRATCH3 ; SCRATCH3L = 0  (char count)
        LDY      #$01
        LDA      #$00           ; store 0 in the parse buffer + 1.
        STA      (OPERAND1),Y
        STOB     #$02, TOKEN_IDX
        STOB     #$01, INPUT_PTR
    .loop_word:
        LDY      #$00           ; if parsebuf[0] == parsebuf[1] do_instruction
        LDA      (OPERAND1),Y
        INY
        CMP      (OPERAND1),Y
        BNE      .not_end1
        JMP      do_instruction
    .not_end1:
        LDA      SCRATCH3+1     ; if input_count == char_count == 0 do_instruction
        ORA      SCRATCH3
        BNE      .not_end2
        JMP      do_instruction
    .not_end2:
        LDA      SCRATCH3       ; if char_count != 6 .not_min_compress_size
        CMP      #$06
        BNE      .not_min_compress_size
        JSR      skip_separators
    .not_min_compress_size:
        LDA      SCRATCH3
        BNE      .not_separator
        LDY      #$06
        LDX      #$00
    .clear:
        LDA      #$00
        STA      ZCHAR_SCRATCH1,X
        INX
        DEY
        BNE      .clear
        LDA      INPUT_PTR          ; parsebuf[TOKEN_IDX+3] = INPUT_PTR
        LDY      TOKEN_IDX
        INY
        INY
        INY
        STA      (OPERAND1),Y
        LDY      INPUT_PTR          ; is_dict_separator(textbuf[INPUT_PTR])
        LDA      (OPERAND0),Y
        JSR      is_dict_separator
        BCS      .is_dict_separator
        LDY      INPUT_PTR          ; is_std_separator(textbuf[INPUT_PTR])
        LDA      (OPERAND0),Y
        JSR      is_std_separator
        BCC      .not_separator
        INC      INPUT_PTR          ; ++INPUT_PTR
        DEC      SCRATCH3+1         ; --input_count
        JMP      .loop_word
    .not_separator:
        LDA      SCRATCH3+1
        BEQ      .search
        LDY      INPUT_PTR          ; is_separator(textbuf[INPUT_PTR])
        LDA      (OPERAND0),Y
        JSR      is_separator
        BCS      .search
        LDY      INPUT_PTR          ; ZCHAR_SCRATCH1[char_count] = textbuf[INPUT_PTR]
        LDA      (OPERAND0),Y
        LDX      SCRATCH3
        STA      ZCHAR_SCRATCH1,X
        DEC      SCRATCH3+1         ; --input_count
        INC      SCRATCH3           ; ++char_count
        INC      INPUT_PTR          ; ++INPUT_PTR
        JMP      .loop_word
    .is_dict_separator:
        STA      ZCHAR_SCRATCH1
        INC      SCRATCH3
        DEC      SCRATCH3+1
        INC      INPUT_PTR
    .search:
        LDA      SCRATCH3
        BEQ      .loop_word
        LDA      SCRATCH3+1     ; Save input_count
        PHA
        LDY      TOKEN_IDX      ; parsebuf[TOKEN_IDX+2] = char_count
        INY
        INY
        LDA      SCRATCH3
        STA      (OPERAND1),Y
        JSR      ascii_to_zchar
        JSR      match_dictionary_word
        LDY      TOKEN_IDX              ; parsebuf[TOKEN_IDX] = entry_addr
        LDA      SCRATCH1+1
        STA      (OPERAND1),Y
        INY
        LDA      SCRATCH1
        STA      (OPERAND1),Y
        INY                             ; TOKEN_IDX += 4
        INY
        INY
        STY      TOKEN_IDX
        LDY      #$01                   ; ++parsebuf[1]
        LDA      (OPERAND1),Y
        CLC
        ADC      #$01
        STA      (OPERAND1),Y
        PLA
        STA      SCRATCH3+1
        STOB     #$00, SCRATCH3
        JMP      .loop_word
    skip_separators:
        LDA      SCRATCH3+1
        BNE      .not_end
        RTS
    .not_end:
        LDY      INPUT_PTR
        LDA      (OPERAND0),Y
        JSR      is_separator
        BCC      .not_separator
        RTS
    .not_separator:
        INC      INPUT_PTR
        DEC      SCRATCH3+1
        INC      SCRATCH3
        JMP      skip_separators
    SEPARATORS_TABLE:
        DC       #$20, #$2E, #$2C, #$3F, #$0D, #$0A, #$09, #$0C
    is_separator:
        JSR      is_dict_separator
        BCC      is_std_separator
        RTS
    is_std_separator:
        LDY      #$00
        LDX      #$08
    .loop:
        CMP      SEPARATORS_TABLE,Y
        BEQ      separator_found
        INY
        DEX
        BNE      .loop
    separator_not_found:
        CLC
        RTS
    separator_found:
        SEC
        RTS
    is_dict_separator:
        PHA
        JSR      get_dictionary_addr
        LDY      #$00
        LDA      (SCRATCH2),Y
        TAX
        PLA
    .loop:
        BEQ      separator_not_found
        INY
        CMP      (SCRATCH2),Y
        BEQ      separator_found
        DEX
        JMP      .loop
    get_dictionary_addr:
        LDY      #HEADER_DICT_ADDR
        LDA      (Z_HEADER_ADDR),Y
        STA      SCRATCH2+1
        INY
        LDA      (Z_HEADER_ADDR),Y
        STA      SCRATCH2
        ADDW     SCRATCH2, Z_HEADER_ADDR, SCRATCH2
        RTS
    match_dictionary_word:
        JSR      get_dictionary_addr
        LDY      #$00                   ; number of dict separators
        LDA      (SCRATCH2),Y
        TAY                             ; skip past and get entry length
        INY
        LDA      (SCRATCH2),Y
        ASL                             ; search_size = entry length x 16
        ASL
        ASL
        ASL
        STA      SCRATCH3
        INY                             ; entry_index = num dict entries
        LDA      (SCRATCH2),Y
        STA      SCRATCH1+1
        INY
        LDA      (SCRATCH2),Y
        STA      SCRATCH1
        INY
        TYA
        ADDA     SCRATCH2               ; entry_addr = start of dictionary entries
        LDY      #$00
        JMP      .try_match
    .loop:
        LDA      (SCRATCH2),Y
        CMP      ZCHAR_SCRATCH2+1
        BCS      .possible
    .try_match:
        ADDB2    SCRATCH2, SCRATCH3     ; entry_addr += search_size
        SEC                             ; entry_index -= 16
        LDA      SCRATCH1
        SBC      #$10
        STA      SCRATCH1
        BCS      .loop
        DEC      SCRATCH1+1
        BPL      .loop
    .possible:
        SUBB2    SCRATCH2, SCRATCH3     ; entry_addr -= search_size
        ADDB2    SCRATCH1, #$10         ; entry_index += 16
        LDA      SCRATCH3               ; search_size /= 16
        LSR
        LSR
        LSR
        LSR
        STA      SCRATCH3
    .inner_loop:
        LDY      #$00
        LDA      ZCHAR_SCRATCH2+1
        CMP      (SCRATCH2),Y
        BCC      .not_found
        BNE      .inner_next
        INY
        LDA      ZCHAR_SCRATCH2
        CMP      (SCRATCH2),Y
        BCC      .not_found
        BNE      .inner_next
        LDY      #$02
        LDA      ZCHAR_SCRATCH2+3
        CMP      (SCRATCH2),Y
        BCC      .not_found
        BNE      .inner_next
        INY
        LDA      ZCHAR_SCRATCH2+2
        CMP      (SCRATCH2),Y
        BCC      .not_found
        BEQ      .found
    .inner_next:
        ADDB2    SCRATCH2, SCRATCH3     ; entry_addr += search_size
        SUBB     SCRATCH1, #$01         ; --entry_index
        LDA      SCRATCH1
        ORA      SCRATCH1+1
        BNE      .inner_loop
    .not_found:
        LDA      #$00
        STA      SCRATCH1+1
        STA      SCRATCH1
        RTS
    .found:
        SUBW     SCRATCH2, Z_HEADER_ADDR, SCRATCH1
        RTS
    ascii_to_zchar:
        STOB     #$00, LOCKED_ALPHABET
        LDX      #$00
        LDY      #$06
    .clear:
        LDA      #$05
        STA      ZCHAR_SCRATCH2,X
        INX
        DEY
        BNE      .clear
        STOB     #$06, SCRATCH3+1   ; nchars = 6
        LDA      #$00
        STA      SCRATCH1           ; dest_index = 0
        STA      SCRATCH2           ; index = 0
    .loop:
        LDX      SCRATCH2           ; c = ZCHAR_SCRATCH1[index++]
        INC      SCRATCH2
        LDA      ZCHAR_SCRATCH1,X
        STA      SCRATCH3
        BNE      .continue
        LDA      #$05
        JMP      .store_zchar
    .continue:
        LDA      SCRATCH1           ; save dest_index
        PHA
        LDA      SCRATCH3           ; alphabet = get_alphabet_for_char(c)
        JSR      get_alphabet_for_char
        STA      SCRATCH1
        CMP      LOCKED_ALPHABET
        BEQ      .same_alphabet
        LDX      SCRATCH2
        LDA      ZCHAR_SCRATCH1,X
        JSR      get_alphabet_for_char
        CMP      SCRATCH1
        BNE      .shift_alphabet
        SEC                         ; shift_char = shift lock char (4 or 5)
        SBC      LOCKED_ALPHABET
        CLC
        ADC      #$03
        JSR      A_mod_3
        CLC
        ADC      #$03
        STA      SCRATCH1+1
        MOVB     SCRATCH1, LOCKED_ALPHABET  ; LOCKED_ALPHABET = alphabet
        PLA                         ; restore dest_index
        STA      SCRATCH1
        LDA      SCRATCH1+1         ; ZCHAR_SCRATCH2[dest_index] = shift_char
        LDX      SCRATCH1
        STA      ZCHAR_SCRATCH2,X
        INC      SCRATCH1           ; ++dest_index
        DEC      SCRATCH3+1         ; --nchars
        BNE      .add_shifted_char
        JMP      z_compress
    .add_shifted_char:
        LDA      SCRATCH1           ; save dest_index
        PHA
        JMP      .same_alphabet
    .shift_alphabet:
        LDA      SCRATCH1           ; shift_char = shift char (2 or 3)
        SEC
        SBC      LOCKED_ALPHABET
        CLC
        ADC      #$03
        JSR      A_mod_3
        TAX
        INX
        PLA
        STA      SCRATCH1           ; restore dest_index
        TXA                         ; ZCHAR_SCRATCH2[dest_index] = shift_char
        LDX      SCRATCH1
        STA      ZCHAR_SCRATCH2,X
        INC      SCRATCH1           ; ++dest_index
        DEC      SCRATCH3+1         ; --nchars
        BNE      .save_dest_index_and_same_alphabet
    stretchy_z_compress:
        JMP      z_compress
    .save_dest_index_and_same_alphabet:
        LDA      SCRATCH1           ; save dest_index
        PHA
    .same_alphabet:
        PLA
        STA      SCRATCH1           ; restore dest_index
        LDA      SCRATCH3
        JSR      get_alphabet_for_char
        SEC
        SBC      #$01               ; alphabet_minus_1 = case(c) - 1
        BPL      .not_lowercase
        LDA      SCRATCH3
        SEC
        SBC      #$5B               ; c -= 'a'-6
    .store_zchar:
        LDX      SCRATCH1           ; ZCHAR_SCRATCH2[dest_index] = c
        STA      ZCHAR_SCRATCH2,X
        INC      SCRATCH1           ; ++dest_index
        DEC      SCRATCH3+1         ; --nchars
        BEQ      .dest_full
        JMP      .loop
    .dest_full:
        JMP      z_compress
    .not_lowercase:
        BNE      .not_alphabetic
        LDA      SCRATCH3
        SEC
        SBC      #$3B               ; c -= 'A'-6
        JMP      .store_zchar
    .not_alphabetic:
        LDA      SCRATCH3
        JSR      search_nonalpha_table
        BNE      .store_zchar
        LDA      #$06               ; ZCHAR_SCRATCH2[dest_index] = 6
        LDX      SCRATCH1
        STA      ZCHAR_SCRATCH2,X
        INC      SCRATCH1           ; ++dest_index
        DEC      SCRATCH3+1         ; --nchars
        BEQ      z_compress
        LDA      SCRATCH3           ; ZCHAR_SCRATCH2[dest_index] = c >> 5
        LSR
        LSR
        LSR
        LSR
        LSR
        AND      #$03
        LDX      SCRATCH1
        STA      ZCHAR_SCRATCH2,X
        INC      SCRATCH1           ; ++dest_index
        DEC      SCRATCH3+1         ; --nchars
        BEQ      z_compress
        LDA      SCRATCH3           ; c &= 0x1F
        AND      #$1F
        JMP      .store_zchar
    search_nonalpha_table:
        LDX      #$24
    .loop:
        CMP      a2_table,X
        BEQ      .found
        DEX
        BPL      .loop
        LDY      #$00
        RTS
    .found:
        TXA
        CLC
        ADC      #$08
        RTS
    get_alphabet_for_char:
        CMP      #$61
        BCC      .check_upper
        CMP      #$7B
        BCS      .check_upper
        LDA      #$00
        RTS
    .check_upper:
        CMP      #$41
        BCC      .check_nonletter
        CMP      #$5B
        BCS      .check_nonletter
        LDA      #$01
        RTS
    .check_nonletter:
        ORA      #$00
        BEQ      .return
        BMI      .return
        LDA      #$02
    .return:
        RTS
    z_compress:
        LDA      ZCHAR_SCRATCH2+1
        ASL
        ASL
        ASL
        ASL
        ROL      ZCHAR_SCRATCH2
        ASL
        ROL      ZCHAR_SCRATCH2
        LDX      ZCHAR_SCRATCH2
        STX      ZCHAR_SCRATCH2+1
        ORA      ZCHAR_SCRATCH2+2
        STA      ZCHAR_SCRATCH2
        LDA      ZCHAR_SCRATCH2+4
        ASL
        ASL
        ASL
        ASL
        ROL      ZCHAR_SCRATCH2+3
        ASL
        ROL      ZCHAR_SCRATCH2+3
        LDX      ZCHAR_SCRATCH2+3
        STX      ZCHAR_SCRATCH2+3
        ORA      ZCHAR_SCRATCH2+5
        STA      ZCHAR_SCRATCH2+2
        LDA      ZCHAR_SCRATCH2+3
        ORA      #$80
        STA      ZCHAR_SCRATCH2+3
        RTS
    sScore:
        DC       "SCORE:"
    print_status_line:
        JSR      dump_buffer_line
        LDA      CH
        PHA
        LDA      CV
        PHA
        LDA      #$00
        STA      CH
        STA      CV
        JSR      VTAB
        STOB     #$3F, INVFLG
        JSR      CLREOL
        LDA      #VAR_CURR_ROOM
        JSR      var_get
        JSR      print_obj_in_A
        JSR      dump_buffer_to_screen
        STOB     #25, CH
        STOW     #sScore, SCRATCH2
        LDX      #$06
        JSR      cout_string
        INC      CH
        LDA      #VAR_SCORE
        JSR      var_get
        JSR      print_number
        LDA      #'/
        JSR      buffer_char
        LDA      #VAR_MAX_SCORE
        JSR      var_get
        JSR      print_number
        JSR      dump_buffer_to_screen
        STOB     #$FF, INVFLG
        PLA
        STA      CV
        PLA
        STA      CH
        JSR      VTAB
        RTS
    read_line:
        JSR      dump_buffer_line
        MOVB     WNDTOP, CURR_LINE
        JSR      GETLN1
        INC      CURR_LINE
        LDA      #$8D               ; newline
        STA      BUFF_AREA,X
        INX                         ; X = num of chars in input
        TXA
        PHA                         ; save X
        LDY      #HEADER_FLAGS2+1
        LDA      (Z_HEADER_ADDR),Y
        AND      #$01               ; Mask for transcript on
        BEQ      .continue
        TXA
        STA      BUFF_END
        JSR      dump_buffer_to_printer
        STOB     #$00, BUFF_END
    .continue
        PLA                         ; restore num of chars in input
        LDY      #$00               ; truncate to max num of chars
        CMP      (OPERAND0),Y
        BCC      .continue2
        LDA      (OPERAND0),Y
    .continue2:
        PHA                         ; save num of chars
        BEQ      .end
        TAX
    .loop:
        LDA      BUFF_AREA,Y   ; convert A-Z to lowercase
        AND      #$7F
        CMP      #$41
        BCC      .continue3
        CMP      #$5B
        BCS      .continue3
        ORA      #$20
    .continue3:
        INY
        STA      (OPERAND0),Y
        CMP      #$0D
        BEQ      .end
        DEX
        BNE      .loop
    .end:
        PLA                         ; restore num of chars
        RTS
*/
#include "zork.h"

static const char sScore[6] = "SCORE:";

/* Prints the room's name, the score, and the maximum score in inverse
 * on the screen's top line, and puts the cursor back. */
void print_status_line(void) {
    dump_buffer_line();
    uint8_t ch = CH;
    uint8_t cv = CV;
    CH = 0;
    CV = 0;
    VTAB();
    INVFLG = 0x3F;
    CLREOL();
    print_obj((uint8_t)var_get(VAR_CURR_ROOM));
    dump_buffer_to_screen();
    CH = 25;
    cout_string(sScore, 6);
    CH++;
    print_number(var_get(VAR_SCORE));
    buffer_char('/');
    print_number(var_get(VAR_MAX_SCORE));
    dump_buffer_to_screen();
    INVFLG = 0xFF;
    CV = cv;
    CH = ch;
    VTAB();
}

/* sread's text and parse buffers. */
static uint8_t *text;
static uint8_t *parse;
/* The characters of the line left to read, the characters of the
 * current word read so far, the offset of the next character in the
 * text buffer, and the offset of the parse buffer's next entry. */
static uint8_t input_count;
static uint8_t char_count;
static uint8_t input_ptr;
static uint8_t token_idx;

/* Reads a line into the text buffer, at most as many characters as
 * its first byte says, with A-Z made lower case, and returns how many
 * it read, counting the carriage return. With a transcript, the line
 * is copied to the printer. */
static uint8_t read_line(void) {
    dump_buffer_line();
    CURR_LINE = WNDTOP;
    GETLN1();
    CURR_LINE++;
    uint8_t x = 0;
    while (BUFF_AREA[x] != 0x8D)
        x++;
    x++;
    if (Z_HEADER_ADDR[HEADER_FLAGS2 + 1] & 0x01) {
        BUFF_END = x;
        dump_buffer_to_printer();
        BUFF_END = 0;
    }
    uint8_t n = x;
    if (n >= text[0])
        n = text[0];
    if (n != 0) {
        uint8_t y = 0;
        uint8_t left = n;
        do {
            uint8_t c = BUFF_AREA[y] & 0x7F;
            if (c >= 'A' && c <= 'Z')
                c |= 0x20;
            y++;
            text[y] = c;
            if (c == 0x0D)
                break;
        } while (--left != 0);
    }
    return n;
}

/* The story's dictionary. */
static uint8_t *get_dictionary_addr(void) {
    return Z_HEADER_ADDR + (((uint16_t)Z_HEADER_ADDR[HEADER_DICT_ADDR] << 8)
                            | Z_HEADER_ADDR[HEADER_DICT_ADDR + 1]);
}

static const uint8_t SEPARATORS_TABLE[8] = {
    ' ', '.', ',', '?', 0x0D, 0x0A, 0x09, 0x0C,
};

/* Whether `c` is one of the dictionary's separators, which are words
 * of their own. A count of 0 separators compares 256, as in the
 * original. */
static bool is_dict_separator(uint8_t c) {
    uint8_t *d = get_dictionary_addr();
    uint8_t n = d[0];
    if (c == 0)
        return false;
    uint8_t y = 0;
    for (;;) {
        y++;
        if (c == d[y])
            return true;
        if (--n == 0)
            return false;
    }
}

/* Whether `c` is a space or another character that only separates
 * words. */
static bool is_std_separator(uint8_t c) {
    for (uint8_t i = 0; i != 8; i++) {
        if (c == SEPARATORS_TABLE[i])
            return true;
    }
    return false;
}

static bool is_separator(uint8_t c) {
    return is_dict_separator(c) || is_std_separator(c);
}

/* Skips the rest of a word longer than six characters, counting it. */
static void skip_separators(void) {
    while (input_count != 0) {
        if (is_separator(text[input_ptr]))
            return;
        input_ptr++;
        input_count--;
        char_count++;
    }
}

/* Returns 0 for a lowercase letter, 1 for an uppercase letter, and 2
 * for any other character, except 0 and characters with the top bit
 * set, which it returns as they are. */
static uint8_t get_alphabet_for_char(uint8_t c) {
    if (c >= 'a' && c <= 'z')
        return 0;
    if (c >= 'A' && c <= 'Z')
        return 1;
    if (c == 0 || (c & 0x80))
        return c;
    return 2;
}

/* Returns the z-character for `c` in alphabet 2, or 0 if it isn't
 * there. */
static uint8_t search_nonalpha_table(uint8_t c) {
    uint8_t x = 23;
    do {
        if (c == a2_table[x])
            return x + 8;
    } while (x-- != 0);
    return 0;
}

/* Packs the six z-characters in ZCHAR_SCRATCH2 into two words, three
 * to a word, the second with its top bit set, and stores them over
 * the first four, each low byte first. */
static void z_compress(void) {
    uint8_t *z = ZCHAR_SCRATCH2;
    uint16_t w0 = ((uint16_t)z[0] << 10) | ((uint16_t)z[1] << 5) | z[2];
    uint16_t w1 = ((uint16_t)z[3] << 10) | ((uint16_t)z[4] << 5) | z[5]
        | 0x8000;
    z[0] = (uint8_t)w0;
    z[1] = (uint8_t)(w0 >> 8);
    z[2] = (uint8_t)w1;
    z[3] = (uint8_t)(w1 >> 8);
}

/* Encodes the word in ZCHAR_SCRATCH1 as six z-characters in
 * ZCHAR_SCRATCH2, padded with 5, and packs them. A letter of another
 * alphabet is preceded by a shift (2 or 3) or, when the letter after
 * it is in the same alphabet, a shift lock (4 or 5). A character in
 * no alphabet is 6 and then its code in two z-characters. */
static void ascii_to_zchar(void) {
    LOCKED_ALPHABET = 0;
    for (uint8_t i = 0; i != 6; i++)
        ZCHAR_SCRATCH2[i] = 5;
    uint8_t left = 6;
    uint8_t dest = 0;
    uint8_t index = 0;
    for (;;) {
        uint8_t c = ZCHAR_SCRATCH1[index];
        index++;
        uint8_t z;
        if (c == 0) {
            z = 5;
        } else {
            uint8_t alphabet = get_alphabet_for_char(c);
            if (alphabet != LOCKED_ALPHABET) {
                uint8_t shift = A_mod_3(alphabet - LOCKED_ALPHABET + 3);
                if (get_alphabet_for_char(ZCHAR_SCRATCH1[index]) == alphabet) {
                    shift += 3;
                    LOCKED_ALPHABET = alphabet;
                } else {
                    shift += 1;
                }
                ZCHAR_SCRATCH2[dest] = shift;
                dest++;
                if (--left == 0)
                    break;
            }
            /* Alphabet 0, as get_alphabet_for_char returns it, and a
             * character with the top bit set, less 1, are negative. */
            uint8_t a = get_alphabet_for_char(c) - 1;
            if (a & 0x80) {
                z = c - 0x5B;                   /* 6 for 'a' */
            } else if (a == 0) {
                z = c - 0x3B;                   /* 6 for 'A' */
            } else {
                z = search_nonalpha_table(c);
                if (z == 0) {
                    ZCHAR_SCRATCH2[dest] = 6;
                    dest++;
                    if (--left == 0)
                        break;
                    ZCHAR_SCRATCH2[dest] = (c >> 5) & 0x03;
                    dest++;
                    if (--left == 0)
                        break;
                    z = c & 0x1F;
                }
            }
        }
        ZCHAR_SCRATCH2[dest] = z;
        dest++;
        if (--left == 0)
            break;
    }
    z_compress();
}

/* Returns the story address of the dictionary entry for the word in
 * ZCHAR_SCRATCH2, or 0 if there isn't one. Looks at every sixteenth
 * entry's first byte until it passes the word's, and then at each
 * entry from sixteen back. */
static uint16_t match_dictionary_word(void) {
    uint8_t *d = get_dictionary_addr();
    uint8_t y = d[0] + 1;
    uint8_t len = d[y];
    uint8_t stride = (uint8_t)(len << 4);
    int16_t count = (int16_t)(((uint16_t)d[y + 1] << 8) | d[y + 2]);
    uint8_t *entry = d + (uint8_t)(y + 3);
    uint8_t key_hi = ZCHAR_SCRATCH2[1];
    for (;;) {
        entry += stride;
        count -= 16;
        if (count < 0 || entry[0] >= key_hi)
            break;
    }
    entry -= stride;
    count += 16;
    uint16_t key0 = ((uint16_t)ZCHAR_SCRATCH2[1] << 8) | ZCHAR_SCRATCH2[0];
    uint16_t key1 = ((uint16_t)ZCHAR_SCRATCH2[3] << 8) | ZCHAR_SCRATCH2[2];
    for (;;) {
        uint16_t e0 = ((uint16_t)entry[0] << 8) | entry[1];
        if (key0 < e0)
            return 0;
        if (key0 == e0) {
            uint16_t e1 = ((uint16_t)entry[2] << 8) | entry[3];
            if (key1 < e1)
                return 0;
            if (key1 == e1)
                return (uint16_t)entry - (uint16_t)Z_HEADER_ADDR;
        }
        entry += len;
        if (--count == 0)
            return 0;
    }
}

/* Shows the status line, reads a line into the text buffer at story
 * address OPERAND0, and splits it into words in the parse buffer at
 * story address OPERAND1. */
void instr_sread(void) {
    print_status_line();
    text = Z_HEADER_ADDR + OPERANDS[0];
    parse = Z_HEADER_ADDR + OPERANDS[1];
    input_count = read_line();
    char_count = 0;
    parse[1] = 0;
    token_idx = 2;
    input_ptr = 1;
    for (;;) {
        if (parse[0] == parse[1])
            return;
        if (input_count == 0 && char_count == 0)
            return;
        if (char_count == 6)
            skip_separators();
        if (char_count == 0) {
            /* The start of a word. */
            for (uint8_t i = 0; i != 6; i++)
                ZCHAR_SCRATCH1[i] = 0;
            parse[token_idx + 3] = input_ptr;
            uint8_t c = text[input_ptr];
            if (is_dict_separator(c)) {
                ZCHAR_SCRATCH1[0] = c;
                char_count++;
                input_count--;
                input_ptr++;
                goto search;
            }
            if (is_std_separator(text[input_ptr])) {
                input_ptr++;
                input_count--;
                continue;
            }
        }
        if (input_count != 0 && !is_separator(text[input_ptr])) {
            ZCHAR_SCRATCH1[char_count] = text[input_ptr];
            input_count--;
            char_count++;
            input_ptr++;
            continue;
        }
    search:
        if (char_count == 0)
            continue;
        parse[token_idx + 2] = char_count;
        ascii_to_zchar();
        uint16_t entry = match_dictionary_word();
        parse[token_idx] = (uint8_t)(entry >> 8);
        parse[token_idx + 1] = (uint8_t)entry;
        token_idx += 4;
        parse[1]++;
        char_count = 0;
    }
}
