/*
    output.c -- Zork's line buffer.

    Printed characters go through Zork's line buffer, BUFF_AREA, which
    breaks lines between words and pauses with [MORE] when the screen
    fills, and copies each line to the printer when the story asks for
    a transcript.

    Departures from the original:

    - buffer_char_set_buffer_end, a label inside buffer_char that
      dump_buffer_with_more jumps to with X = 0, is the assignment
      BUFF_END = 0.
    - cout_string's string arrives in SCRATCH2 as a parameter there
      (`zeropage(0xE6)`), so Zork's other routines can still call it.

    Zork I (r15), output/main.asm lines 3050-3227 and, for
    cout_string, 3282-3294:

    buffer_char:
        LDX      BUFF_END
        CMP      #$0D
        BNE      .not_0D
        JMP      dump_buffer_with_more
    .not_0D:
        CMP      #$20
        BCC      buffer_char_set_buffer_end
        CMP      #$60
        BCC      .store_char
        CMP      #$80
        BCS      .store_char
        SEC
        SBC      #$20              ; converts to uppercase
    .store_char:
        ORA      #$80              ; sets as normal text
        STA      BUFF_AREA,X
        CPX      WNDWDTH
        BCS      .hit_right_limit
        INX
    buffer_char_set_buffer_end:
        STX      BUFF_END
        RTS
    .hit_right_limit:
        LDA      #$A0  ; normal space
    .loop:
        CMP      BUFF_AREA,X
        BEQ      .endloop
        DEX
        BNE      .loop
        LDX      WNDWDTH
    .endloop:
        STX      BUFF_LINE_LEN
        STX      BUFF_END
        JSR      dump_buffer_with_more
    .increment_length:
        INC      BUFF_LINE_LEN
        LDX      BUFF_LINE_LEN
        CPX      WNDWDTH
        BCC      .move_last_char
        BEQ      .move_last_char
        RTS
    .move_last_char:
        LDA      BUFF_AREA,X
        LDX      BUFF_END
        STA      BUFF_AREA,X
        INC      BUFF_END
        LDX      BUFF_LINE_LEN
        JMP      .increment_length
    dump_buffer_line:
        LDY      #HEADER_FLAGS2+1
        LDA      (Z_HEADER_ADDR),Y
        AND      #$01
        BEQ      .skip_printer
        JSR      dump_buffer_to_printer
    .skip_printer:
        JSR      dump_buffer_to_screen
        RTS
    printer_card_initialized_flag:
        BYTE     00
    dump_buffer_to_printer:
        PSHW     CSW
        MOVW     PRINTER_CSW, CSW
        LDX      #$00
        LDA      printer_card_initialized_flag
        BNE      .loop
        INC      printer_card_initialized_flag
    .printer_set_80_column_output:
        LDA      #$09      ; ctrl-I
        JSR      COUT
        STOB     #$91, $0779      ; 'Q' into scratchpad RAM for slot 1.
        LDA      #$B8      ; '8'
        JSR      COUT
        LDA      #$B0      ; '0'
        JSR      COUT
        LDA      #$CE      ; 'N'
        JSR      COUT
    .loop:
        CPX      BUFF_END
        BEQ      .done
        LDA      BUFF_AREA,X
        JSR      COUT
        INX
        JMP      .loop
    .done:
        MOVW     CSW, PRINTER_CSW
        PULW     CSW
        RTS
    dump_buffer_to_screen:
        LDX      #$00
    .loop:
        CPX      BUFF_END
        BEQ      .done
        LDA      BUFF_AREA,X
        JSR      COUT1
        INX
        JMP      .loop
    .done:
        LDX      #$00
        STX      BUFF_END
        RTS
    string_more:
        DC       "[MORE]"
    dump_buffer_with_more:
        INC      CURR_LINE
        LDA      CURR_LINE
        CMP      WNDBTM
        BCC      .good_to_go    ; haven't reached bottom of screen yet
        STOW     string_more, SCRATCH2
        LDX      #6
        STOB     #$3F, INVFLG
        JSR      cout_string    ; print [MORE] in inverse text
        STOB     #$FF, INVFLG
        JSR      RDKEY          ; wait for keypress
        LDA      CH
        SEC
        SBC      #$06
        STA      CH             ; move cursor back 6
        JSR      CLREOL         ; and clear the line
        MOVB     WNDTOP, CURR_LINE
        INC      CURR_LINE      ; start at top of screen
    .good_to_go:
        LDA      BUFF_END
        PHA
        JSR      dump_buffer_line
        PLA
        CMP      WNDWDTH
        BEQ      .skip_newline
        LDA      #$8D
        JSR      COUT1
    .skip_newline:
        LDY      #HEADER_FLAGS2+1
        LDA      (Z_HEADER_ADDR),Y
        AND      #$01
        BEQ      .reset_buffer_end
        PSHW     CSW
        MOVW     PRINTER_CSW, CSW
        LDA      #$8D
        JSR      COUT
        MOVW     CSW, PRINTER_CSW
        PULW     CSW
    .reset_buffer_end:
        LDX      #$00
        JMP      buffer_char_set_buffer_end
    cout_string:
        LDY      #$00
    .loop:
        LDA      (SCRATCH2),Y
        ORA      #$80
        JSR      COUT1
        INY
        DEX
        BNE      .loop
        RTS
*/
#include "zork.h"

/* Adds `c` to the line buffer. A carriage return ends the line, and
 * other control characters are dropped. Lower case becomes upper
 * case. When the line reaches the window's width, the line is printed
 * up to its last space, and the word after it moves to the start of
 * the next line. */
void buffer_char(register uint8_t c) {
    uint8_t x = BUFF_END;
    if (c == 0x0D) {
        dump_buffer_with_more();
        return;
    }
    if (c < 0x20)
        return;
    if (c >= 0x60 && c < 0x80)
        c -= 0x20;
    BUFF_AREA[x] = c | 0x80;
    if (x < WNDWDTH) {
        BUFF_END = x + 1;
        return;
    }
    /* Find the last space. With none, break at the window's width. */
    for (;;) {
        if (BUFF_AREA[x] == 0xA0)
            break;
        if (--x == 0) {
            x = WNDWDTH;
            break;
        }
    }
    BUFF_LINE_LEN = x;
    BUFF_END = x;
    dump_buffer_with_more();
    /* Move what followed the break to the start of the buffer. */
    for (;;) {
        uint8_t from = ++BUFF_LINE_LEN;
        if (from > WNDWDTH)
            return;
        BUFF_AREA[BUFF_END++] = BUFF_AREA[from];
    }
}

/* Prints the buffered line on the screen, and on the printer too when
 * the story has asked for a transcript. */
void dump_buffer_line(void) {
    if (Z_HEADER_ADDR[HEADER_FLAGS2 + 1] & 0x01)
        dump_buffer_to_printer();
    dump_buffer_to_screen();
}

static uint8_t printer_card_initialized_flag;

void dump_buffer_to_printer(void) {
    uint16_t saved_csw = CSW;
    CSW = PRINTER_CSW;
    if (!printer_card_initialized_flag) {
        printer_card_initialized_flag = 1;
        COUT(0x09);                     /* ctrl-I */
        SLOT1_SCRATCH = 0x91;
        COUT(0xB8);                     /* '8' */
        COUT(0xB0);                     /* '0' */
        COUT(0xCE);                     /* 'N' */
    }
    for (uint8_t x = 0; x != BUFF_END; x++)
        COUT(BUFF_AREA[x]);
    PRINTER_CSW = CSW;
    CSW = saved_csw;
}

void dump_buffer_to_screen(void) {
    for (uint8_t x = 0; x != BUFF_END; x++)
        COUT1(BUFF_AREA[x]);
    BUFF_END = 0;
}

static const uint8_t string_more[6] = { '[', 'M', 'O', 'R', 'E', ']' };

/* Prints the buffered line and ends it. When the window is full, first
 * shows [MORE] in inverse and waits for a key. */
void dump_buffer_with_more(void) {
    if (++CURR_LINE >= WNDBTM) {
        INVFLG = 0x3F;
        cout_string(string_more, 6);
        INVFLG = 0xFF;
        RDKEY();
        CH -= 6;
        CLREOL();
        CURR_LINE = WNDTOP + 1;
    }
    uint8_t end = BUFF_END;
    dump_buffer_line();
    /* A full line has already wrapped. */
    if (end != WNDWDTH)
        COUT1(0x8D);
    if (Z_HEADER_ADDR[HEADER_FLAGS2 + 1] & 0x01) {
        uint16_t saved_csw = CSW;
        CSW = PRINTER_CSW;
        COUT(0x8D);
        PRINTER_CSW = CSW;
        CSW = saved_csw;
    }
    BUFF_END = 0;
}

/* Prints the `count` characters at `s` straight to the screen, with
 * their high bits set, in normal text unless INVFLG says otherwise. A
 * count of 0 prints 256. Zork's other routines pass `s` in SCRATCH2. */
void cout_string(const uint8_t *s [[c6502::zeropage(0xE6)]],
                 uint8_t count [[c6502::in_x]]) {
    uint8_t y = 0;
    do {
        COUT1(s[y] | 0x80);
        y++;
    } while (--count != 0);
}
