/*
    text.c -- printing strings, messages, numbers, and the scenes' text
    streams through the HRCG.

    Strings are a length byte followed by high-bit ASCII. The bottom
    three rows of the screen, 21-23, are the status lines, where
    display_message prints a message's strings centered on row 23 and
    scrolls them up. The control characters that set the HRCG's font,
    window, and video are sent by the small routines at the end of the
    file. text_renderer draws a scene's text stream in FONT_CHARSET 2's
    glyphs, two text cells square, with render_font_char.

    Departures from the original:

    - Every JSR or JMP ROM_COUT1 is a call of hrcg_cout, as ali_baba.h
      explains.
    - PRINT_STRING_ADDR, NUM_VALUE, and DECIMAL_OUT_PTR are parameters
      and locals. DISPLAY_MESSAGE leaves its pointer in
      PRINT_STRING_ADDR across DELAY_WITH_ANIMATION, which preserves it
      in the original, and in C the pointer is a local.
    - TEXT_RENDERER calls DISPLAY_STATUS_BAR at a $7E in the stream,
      and DISPLAY_STATUS_BAR calls TEXT_RENDERER. In C that would be
      recursion, which c6502 can't link. render_stream renders up to a
      $7E or the end, text_renderer handles the pause between calls of
      it, and text_renderer_no_pause, which display_status_bar calls,
      stops at a $7E. The status bar's stream has none.
    - TEXT_RENDERER jumps to MOVE_LOOP when its index passes 255 after
      a glyph, which would run a turn from inside whatever called it.
      No stream on the disk is that long, and render_stream ends the
      stream there.
    - NUM_TO_DECIMAL takes its divisors from DECIMAL_PLACES and keeps
      the value and the digit in locals while it divides. It leaves
      NUM_DIVISOR, NUM_DIGIT, and NUM_LEADING as the original does,
      and writes all of NUM_DIVISOR where the original writes only the
      low byte of 10 and 1. The entries at NUM_TO_DECIMAL+3, +$30, and
      +$3E, which only the dead ORPHANED_ENCOUNTER_HANDLER calls, aren't
      translated.
    - PRINT_STRING_AT_ADDR, which takes the string in X and Y, is
      print_string. SET_TEXT_WINDOW_UPPER_LEFT_ALL and
      SET_TEXT_WINDOW_UPPER_LEFT_LOW are set_text_window_upper_left(0)
      and (21), and SET_CURSOR_ROW21 and SET_TEXT_WINDOW_BOTTOM share
      set_cursor_row.

    Ali Baba, main.asm lines 2434-2497, 12698-12786, 14362-14469,
    14475-14499, 14539-14582, 14600-14717, and 15011-15036:

        ORG     $0DA1
    NUM_TO_DECIMAL:
        SUBROUTINE NUM_TO_DECIMAL
        ; Convert a 16-bit value to up to 5 ASCII decimal digits.
        ;
        ; Inputs:
        ;   NUM_VALUE — 16-bit unsigned value to convert
        ;   DECIMAL_OUT_PTR — pointer to output buffer
        ;
        ; Behavior:
        ;   Divides NUM_VALUE by 10000, 1000, 100, 10, and 1 via
        ;   repeated subtraction, storing high-ASCII digit characters
        ;   at DECIMAL_OUT_PTR with leading zeros suppressed.
        ;
        ; Modifies: NUM_VALUE, NUM_DIVISOR, NUM_DIGIT, NUM_LEADING
        ; Clobbers: A, X, Y

        LDY     #$00                ; output index = 0
        STY     NUM_LEADING         ; leading-zero flag = 0 (suppress)
    NUM_TO_DECIMAL_CONT:
        STOW    $2710,NUM_DIVISOR ; divisor = $2710 = 10000
        JSR     .extract_digit      ; ten-thousands digit
        STOW    $03E8,NUM_DIVISOR ; divisor = $03E8 = 1000
        JSR     .extract_digit      ; thousands digit
        STOW    $0064,NUM_DIVISOR ; divisor = $0064 = 100
        JSR     .extract_digit      ; hundreds digit
        STOB    #$0A,NUM_DIVISOR ; divisor = $000A = 10
        JSR     .extract_digit      ; tens digit
        STOB    #$01,NUM_DIVISOR ; divisor = $0001 = 1
        STA     NUM_LEADING         ; /  force output (no suppress)
        JSR     .extract_digit      ; ones digit
        RTS

    ; --- Digit extraction helper ($0DE1) ---
    ; Divides NUM_VALUE/NUM_VALUE+1 by $5A1C/$5A1D via repeated subtraction.
    ; Stores ASCII digit at (DECIMAL_OUT_PTR),Y. Suppresses leading zeros.
    .extract_digit:
        STOB    #$B0,NUM_DIGIT ; digit = '0' (high ASCII)
    .cmp_loop:
        LDX     #$01                ; start comparing high byte
    .cmp_x:
        LDA     NUM_VALUE,X         ; \
        CMP     NUM_DIVISOR,X       ;  | compare NUM_VALUE/NUM_VALUE+1 with divisor
        BCC     .output             ;  | if less, done (digit found)
        BNE     .subtract           ; /  if greater, subtract
        DEX                         ; \  high bytes equal: check low
        BNE     .subtract           ; /  (if X was 1, now 0 -> recompare)
        JMP     .cmp_x              ;    compare low byte (X=0)
    .subtract:
        SUBW    NUM_VALUE,NUM_DIVISOR,NUM_VALUE ;  NUM_VALUE/NUM_VALUE+1 -= divisor
        INC     NUM_DIGIT           ; digit++
        JMP     .cmp_loop           ; try again (reset X=1)
    .output:
        LDA     NUM_DIGIT           ; \
        BANE    #$B0,_NUM_TO_DECIMAL.store         ; and leading-zero suppress active
        LDA     NUM_LEADING         ;  |   skip output
        BANE    #$01,_NUM_TO_DECIMAL.skip
        LDA     #$B0                ; (reload '0' for final digit)
    _NUM_TO_DECIMAL.store:
        STA     (DECIMAL_OUT_PTR),Y ; store digit at output buffer
        STOB    #$01,NUM_LEADING ; clear leading-zero suppress
    _NUM_TO_DECIMAL.skip:
        INY                         ; advance output index
        RTS

        ORG     $6C8E
    TEXT_RENDERER:
        SUBROUTINE TEXT_RENDERER
        ; Render a byte-stream of text and control codes to the hi-res screen.
        ;
        ; Inputs:
        ;   TEXT_STREAM_PTR  — pointer to byte stream to render
        ;
        ; Behavior:
        ;   Initializes TEXT_LEFT_MARGIN=2, TEXT_RIGHT_MARGIN=18, FONT_ROW=1,
        ;   then reads bytes in a loop.  Bytes $00-$7C are rendered as custom
        ;   font characters (FONT_CHARSET=2), advancing the column and wrapping
        ;   at TEXT_RIGHT_MARGIN.  $7D resets to full-screen layout.  $7E pauses:
        ;   plays TUNE_ATTRACT, calls DISPLAY_STATUS_BAR, then reinitializes.
        ;   $7F ends the stream and returns.  $80-$FE are position commands:
        ;   AND #$7F gives the new row, the following byte gives the column.
        ;
        ; Modifies:
        ;   FONT_COL, FONT_ROW, FONT_CHARNUM, FONT_CHARSET,
        ;   TEXT_LEFT_MARGIN, TEXT_RIGHT_MARGIN, TEXT_STREAM_IDX
        ; Clobbers: A, X, Y
        LDY     #$00
    .init:
        STOB    #$02,TEXT_LEFT_MARGIN ; left margin = 2
        STA     FONT_COL            ; current column = 2
        STOB    #$12,TEXT_RIGHT_MARGIN ; right margin = $12 (18)
        STOB    #$01,FONT_ROW ; current row = 1

        ; --- Main byte-reading loop ---
    .fetch:
        LDA     (TEXT_STREAM_PTR),Y ; read byte from stream
        BMI     .set_pos            ; bit 7 set -> position command
        BANE    #$7F,_TEXT_RENDERER.not_end
        RTS                         ; $7F = end of stream

    _TEXT_RENDERER.not_end:
        BAEQ    #$7E,_TEXT_RENDERER.pause         ; $7E = pause
        BAEQ    #$7D,_TEXT_RENDERER.reset         ; $7D = reset to full-screen mode

        ; --- Character output ---
        STY     TEXT_STREAM_IDX     ; save stream index
        STA     FONT_CHARNUM        ; character code
        STOB    #$02,FONT_CHARSET ; character width = 2
        JSR     RENDER_FONT_CHAR    ; draw at current position
        INC     FONT_COL            ; advance column
        LDA     FONT_COL
        BALT    TEXT_RIGHT_MARGIN,_TEXT_RENDERER.next ; past right margin?
        INC     FONT_ROW            ; yes -> next row
        MOVB    TEXT_LEFT_MARGIN,FONT_COL ;   reset column to left margin
    _TEXT_RENDERER.next:
        LDY     TEXT_STREAM_IDX     ; restore stream index
        INY                         ; advance to next byte
        BNE     .fetch              ; loop (max 256 per page)
        JMP     MOVE_LOOP           ; overflow handler

        ; --- Position command ($80-$FE) ---
    .set_pos:
        AND     #$7F                ; row = byte & $7F
        STA     FONT_ROW
        INY
        LDA     (TEXT_STREAM_PTR),Y ; next byte = column
        STA     FONT_COL
        INY
        JMP     .fetch

        ; --- Pause ($7E): run tune player, show status bar ---
    _TEXT_RENDERER.pause:
        TYA                         ; \
        PHA                         ;  | save stream index
        NOP
        LDX     #TUNE_ATTRACT       ; tune 6: attract/opening
        JSR     TUNE_PLAYER
        PSHW    TEXT_STREAM_PTR     ; save stream pointer
        JSR     DISPLAY_STATUS_BAR  ; (clobbers TEXT_STREAM_PTR)
        PULW    TEXT_STREAM_PTR     ; restore stream pointer
        PLA                         ; restore stream index
        TAY
        NOP
        INY                         ; advance past $7E
        JMP     .init               ; reinit margins/position

        ; --- Reset ($7D): full-screen text mode ---
    _TEXT_RENDERER.reset:
        STOB    #$00,TEXT_LEFT_MARGIN  ; left margin = 0
        STA     FONT_COL               ; column = 0
        STA     FONT_ROW               ; row = 0
        STOB    #MAP_WIDTH,TEXT_RIGHT_MARGIN ; right margin = 20 (MAP_WIDTH)
        INY                         ; advance past $7D
        JMP     .fetch

        ORG     $761E
    DISPLAY_MESSAGE:
        SUBROUTINE DISPLAY_MESSAGE
        ; Display a game message by index from the message table.
        ;
        ; Inputs:
        ;   A — message index (0-based)
        ;
        ; Behavior:
        ;   Skips through the message table to find the indexed entry,
        ;   then prints each sub-string centered on row 23. Multi-line
        ;   messages scroll the status area between lines and pause
        ;   with animation between groups.
        ;
        ; Modifies: PRINT_STRING_ADDR
        ; Clobbers: A, X, Y

        TAX                           ; X = message index
        MOVW    MSG_TABLE_PTR,PRINT_STRING_ADDR ; load table base into PRINT_STRING_ADDR/PRINT_STRING_ADDR+1
        STOB    #$00,MSG_LINE_COUNT ; reset line counter

        JMP     .check_index          ; skip to index check

    _DISPLAY_MESSAGE.skip_entry:
        LDY     #$00
        LDA     (PRINT_STRING_ADDR),Y ; get string length
        TAY
        SEC                           ; +1 for length byte itself
        ADC     PRINT_STRING_ADDR     ; advance pointer past this entry
        STA     PRINT_STRING_ADDR
        LDA     PRINT_STRING_ADDR+1
        ADC     #$00
        STA     PRINT_STRING_ADDR+1
        BYNE    #$00,_DISPLAY_MESSAGE.skip_entry      ; no -- keep skipping within group
    .next_index:
        DEX
    .check_index:
        BXNE    #$00,_DISPLAY_MESSAGE.skip_entry      ; more entries to skip

    DISPLAY_MESSAGE_LOOP:
    .print_loop:
        LDY     #$00
        LDA     (PRINT_STRING_ADDR),Y ; get length of current string
        BNE     .has_text             ; nonzero -- print it

        ; length is zero -- message is done
        LDA     MSG_LINE_COUNT
        BAEQ    #$02,_DISPLAY_MESSAGE.final_scroll    ; printed exactly 2 lines?
        RTS                           ; otherwise just return
    _DISPLAY_MESSAGE.final_scroll:
        JMP     SCROLL_STATUS_LINE    ; scroll once more and return

    .has_text:
        LDA     MSG_LINE_COUNT
        BAMI    #$01,_DISPLAY_MESSAGE.first_line ; line_count < 1: first line
        BEQ     .second_line          ; line_count == 1: second line

        ; third+ line: scroll, then print
        JSR     SCROLL_STATUS_LINE    ; scroll status window
    .second_line:
        JSR     PRINT_BOTTOM_CENTERED ; print centered on row 23
        JMP     .advance

    _DISPLAY_MESSAGE.first_line:
        JSR     PRINT_BOTTOM_CENTERED ; print centered on row 23
        JSR     SCROLL_STATUS_LINE    ; scroll status window

    .advance:
        INC     MSG_LINE_COUNT
        LDY     #$00
        LDA     PRINT_STRING_ADDR
        SEC
        ADC     (PRINT_STRING_ADDR),Y ; add length + 1 (SEC sets carry)
        STA     PRINT_STRING_ADDR
        LDA     PRINT_STRING_ADDR+1
        ADC     #$00
        STA     PRINT_STRING_ADDR+1
        LDA     #$01
        BALT    MSG_LINE_COUNT,_DISPLAY_MESSAGE.pause ; printed <= 1 lines?
        LDY     #$00
        LDA     (PRINT_STRING_ADDR),Y ; peek at next entry
        BNE     .print_loop           ; more text -- keep printing

    _DISPLAY_MESSAGE.pause:
        JSR     DELAY_WITH_ANIMATION  ; delay
        JSR     PRINT_CTRL_AB         ; reset character set
        JMP     .print_loop           ; continue with next line
        ORG     $769B
    SCROLL_STATUS_LINE:
        SUBROUTINE SCROLL_STATUS_LINE
        ; Scroll the bottom status area up by one line.
        ;
        ; Behavior:
        ;   Switches to scroll mode, confines the window to rows 21-23,
        ;   positions cursor at row 23, sends $83 to trigger a scroll-up,
        ;   then restores full-screen wrap mode.
        ;
        ; Modifies: TEXT_ROW, TEXT_COL
        ; Clobbers: A

        ; Text display: init HRCG, set window bottom, output char $83, reset
        JSR     SET_TEXT_WINDOW_SCROLL
        JSR     SET_TEXT_WINDOW_UPPER_LEFT_LOW
        STOB    #$17,TEXT_ROW ; cursor to row 23
        LDA     #$83
        JSR     ROM_COUT1
        JSR     SET_TEXT_WINDOW_UPPER_LEFT_ALL
        JMP     SET_TEXT_WINDOW_WRAP

        ORG     $76BE
    SET_CURSOR_ROW21:
        SUBROUTINE SET_CURSOR_ROW21
        ; Position cursor at row 21 and configure status area.
        ;
        ; Behavior:
        ;   Sets TEXT_ROW to 21 and TEXT_COL to 0, then sends charset
        ;   select and Ctrl-F to ROM_COUT1 to position the cursor.
        ;
        ; Modifies: TEXT_ROW, TEXT_COL
        ; Clobbers: A

        LDA     #$15     ; WNDBTM = 21
        BNE     .shared  ; always taken (stretchy branch)

    SET_TEXT_WINDOW:
    SET_TEXT_WINDOW_BOTTOM:
        LDA     #$14     ; WNDBTM = 20

    .shared:
        STA     TEXT_ROW
        STOB    #$00,TEXT_COL ; WNDLFT = 0
        JSR     PRINT_CTRL_AB
        LDA     #$86     ; Ctrl-F with high bit
        JMP     ROM_COUT1

        ORG     $7705
    PRINT_BOTTOM_CENTERED:
        SUBROUTINE PRINT_BOTTOM_CENTERED
        ; Print a centered string at the bottom of the screen.
        ;
        ; Inputs:
        ;   PRINT_STRING_ADDR — pointer to length-prefixed string
        ;
        ; Behavior:
        ;   Sets cursor to row 23, col 0, then falls through to
        ;   PRINT_CENTERED_PLUS_COL.
        ;
        ; Modifies: TEXT_ROW, TEXT_COL
        ; Clobbers: A, X, Y

        STOB    #$17,TEXT_ROW
        STOB    #$00,TEXT_COL
        JMP     PRINT_CENTERED_PLUS_COL
        ORG     $7710
    PRINT_CENTERED_PLUS_COL:
        SUBROUTINE PRINT_CENTERED_PLUS_COL
        ; Center and print a string, offset by current column.
        ;
        ; Inputs:
        ;   PRINT_STRING_ADDR — pointer to length-prefixed string
        ;   TEXT_COL — additional column offset added to centering
        ;
        ; Behavior:
        ;   Computes (40 - string_length) / 2 + TEXT_COL as the starting
        ;   column, selects charset 0, then prints the string.
        ;
        ; Modifies: TEXT_COL
        ; Clobbers: A, X, Y

        LDA     #40
        SEC
        LDY     #$00
        SBC     (PRINT_STRING_ADDR),Y
        CLC
        LSR                             ; accumulator
        ADC     TEXT_COL
        STA     TEXT_COL
        JSR     PRINT_CTRL_AB
        JMP     PRINT_STRING

        ORG     $7730
    SET_INVERSE_VIDEO:
        SUBROUTINE SET_INVERSE_VIDEO
        ; Set inverse video mode via COUT1.
        ;
        ; Behavior:
        ;   Sends Ctrl-I ($89) to ROM_COUT1 to enable inverse video output.
        ;
        ; Clobbers: A

        LDA     #$89        ; ctrl-I
        JMP     ROM_COUT1
        ORG     $7735
    PRINT_CTRL_N:
        SUBROUTINE PRINT_CTRL_N
        ; Set normal video mode via COUT1.
        ;
        ; Behavior:
        ;   Sends Ctrl-N ($8E) to ROM_COUT1 to restore normal
        ;   (non-inverse) video output.
        ;
        ; Clobbers: A

        LDA     #$8E      ; Ctrl-N with high bit
        JMP     ROM_COUT1 ; output via COUT1
        ORG     $773A
    SET_TEXT_WINDOW_WRAP:
        SUBROUTINE SET_TEXT_WINDOW_WRAP
        ; Enable text window wrap mode.
        ;
        ; Behavior:
        ;   Sends Ctrl-O then Ctrl-W to ROM_COUT1, enabling wrap mode
        ;   so text wraps to the top of the window instead of scrolling.
        ;
        ; Clobbers: A

        LDA     #$8F ; ctrl-O
        JSR     ROM_COUT1
        LDA     #$97 ; ctrl-W
        JMP     ROM_COUT1

    SET_TEXT_WINDOW_SCROLL:
        SUBROUTINE SET_TEXT_WINDOW_SCROLL
        ; Enable text window scroll mode.
        ;
        ; Behavior:
        ;   Sends Ctrl-O then Ctrl-S to ROM_COUT1, clearing the wrap
        ;   flag and restoring normal scroll behavior.
        ;
        ; Clobbers: A

        LDA     #$8F ; ctrl-O
        JSR     ROM_COUT1
        LDA     #$93 ; ctrl-S
        JMP     ROM_COUT1

    ; PRINT_CTRL_AB (PRINT_CTRL_AB) follows at [[PRINT_CTRL_AB]], defined in its own chunk

        ORG     $774E
    PRINT_CTRL_AB:
        SUBROUTINE PRINT_CTRL_AB
        ; Select character set 0 in the HRCG.
        ;
        ; Behavior:
        ;   Sends Ctrl-A ($81) followed by '0' ($B0) to ROM_COUT1,
        ;   which selects the standard character set.
        ;
        ; Clobbers: A

        LDA     #$81 ; Ctrl-A with high bit
        JSR     ROM_COUT1
        LDA     #$B0 ; '0' with high bit
        JMP     ROM_COUT1
        ORG     $7758
    PRINT_STRING_AT_ADDR:
        SUBROUTINE PRINT_STRING_AT_ADDR
        ; Set string pointer and print a length-prefixed string.
        ;
        ; Inputs:
        ;   X — low byte of string address
        ;   Y — high byte of string address
        ;
        ; Behavior:
        ;   Stores X/Y into PRINT_STRING_ADDR, then falls through to PRINT_STRING.
        ;
        ; Clobbers: A, X, Y

        STX     PRINT_STRING_ADDR
        STY     PRINT_STRING_ADDR+1

        ; falls through to PRINT_STRING at [[PRINT_STRING]]
        ORG     $775C
    PRINT_STRING:
        SUBROUTINE PRINT_STRING
        ; Print a length-prefixed string via COUT1.
        ;
        ; Inputs:
        ;   PRINT_STRING_ADDR — pointer to length-prefixed string
        ;
        ; Behavior:
        ;   Reads the first byte as a length count, then outputs each
        ;   subsequent byte through ROM_COUT1.
        ;
        ; Clobbers: A, X, Y

        LDY     #$00
        LDA     (PRINT_STRING_ADDR),Y ; first byte = length
        TAX
    .loop:
        DEX
        BPL     .next
        RTS                           ; done

    .next:
        INY
        LDA     (PRINT_STRING_ADDR),Y ; read next char
        JSR     ROM_COUT1
        JMP     .loop

        ORG     $792D
    SET_TEXT_WINDOW_UPPER_LEFT_ALL:
        SUBROUTINE SET_TEXT_WINDOW_UPPER_LEFT_ALL
        ; Set text window origin to row 0, col 0 (full screen).
        ;
        ; Behavior:
        ;   Sets TEXT_ROW to 0 and TEXT_COL to 0, then sends Ctrl-V
        ;   to ROM_COUT1 to reposition the HRCG text window.
        ;
        ; Modifies: TEXT_ROW, TEXT_COL
        ; Clobbers: A

        LDA     #0
        JMP     SET_TEXT_WINDOW_UPPER_LEFT

    SET_TEXT_WINDOW_UPPER_LEFT_LOW:
        LDA     #21

        ; fall through

    SET_TEXT_WINDOW_UPPER_LEFT:

        STA     TEXT_ROW
        STOB    #0,TEXT_COL
        LDA     #$96        ; ctrl-V (set text window upper left)
        JMP     ROM_COUT1
*/
#include "ali_baba.h"

/* The places NUM_TO_DECIMAL divides by, one per digit. */
static const uint16_t DECIMAL_PLACES[5] = { 10000, 1000, 100, 10, 1 };

/* Leading zeros leave their bytes of `out` alone. NUM_LEADING is 1
 * from the first digit written, and always for the ones digit, so that
 * 0 is written as one '0'. */
void num_to_decimal(uint16_t value, uint8_t *out) {
    NUM_LEADING = 0;
    for (uint8_t i = 0; i < 5; i++) {
        uint16_t place = DECIMAL_PLACES[i];
        NUM_DIVISOR = place;
        if (i == 4)
            NUM_LEADING = 1;
        uint8_t digit = 0xB0;
        while (value >= place) {
            value -= place;
            digit++;
        }
        NUM_DIGIT = digit;
        if (digit != 0xB0 || NUM_LEADING == 1) {
            out[i] = digit;
            NUM_LEADING = 1;
        }
    }
}

/* What render_stream returns when it reaches the end of the stream. A
 * value below this is the index of a $7E byte. */
constexpr uint16_t STREAM_END = 0x100;

/* Renders TEXT_STREAM_PTR from index `i` with the scene's layout, until
 * a $7F, which ends the stream, or a $7E, which pauses it. Returns
 * STREAM_END or the $7E's index. Each byte of the stream is one of
 * these.
 *
 *   $00-$7C    a glyph of FONT_CHARSET 2, drawn at FONT_COL, FONT_ROW,
 *              after which the column moves right, and to the left
 *              margin of the next row at the right margin
 *   $7D        the whole map's layout: margins 0 and 20, from row 0
 *   $7E        a pause
 *   $7F        the end
 *   $80-$FF    a move to row (byte & $7F) and the column in the next
 *              byte
 *
 * The scene's layout has margins 2 and 18 and starts at column 2 of
 * row 1. */
static uint16_t render_stream(uint8_t i) {
    TEXT_LEFT_MARGIN = 2;
    FONT_COL = 2;
    TEXT_RIGHT_MARGIN = 18;
    FONT_ROW = 1;
    for (;;) {
        uint8_t b = TEXT_STREAM_PTR[i];
        if (b & 0x80) {
            FONT_ROW = b & 0x7F;
            FONT_COL = TEXT_STREAM_PTR[++i];
            i++;
        } else if (b == 0x7F) {
            return STREAM_END;
        } else if (b == 0x7E) {
            return i;
        } else if (b == 0x7D) {
            TEXT_LEFT_MARGIN = 0;
            FONT_COL = 0;
            FONT_ROW = 0;
            TEXT_RIGHT_MARGIN = MAP_WIDTH;
            i++;
        } else {
            TEXT_STREAM_IDX = i;
            FONT_CHARNUM = b;
            FONT_CHARSET = 2;
            render_font_char();
            if (++FONT_COL >= TEXT_RIGHT_MARGIN) {
                FONT_ROW++;
                FONT_COL = TEXT_LEFT_MARGIN;
            }
            /* The original jumps to MOVE_LOOP here when the index
             * passes 255, which no stream does. */
            if (++i == 0)
                return STREAM_END;
        }
    }
}

/* At a pause, plays the title tune and draws the status bar, which
 * renders a stream of its own, and then goes on with the scene's
 * layout from the byte after the $7E. */
void text_renderer(void) {
    uint16_t at = render_stream(0);
    while (at != STREAM_END) {
        tune_player(TUNE_ATTRACT, 0);
        const uint8_t *stream = TEXT_STREAM_PTR;
        display_status_bar();
        TEXT_STREAM_PTR = stream;
        at = render_stream((uint8_t)at + 1);
    }
}

/* The status bar's stream has no $7E, and display_status_bar can't call
 * text_renderer, which calls it. */
void text_renderer_no_pause(void) {
    render_stream(0);
}

/* A message is a group of strings ended by a length of 0, and the
 * table is the groups one after another. */
void display_message(uint8_t index) {
    const uint8_t *msg = ((struct location_header *)LOCATIONS)->msg_table;
    MSG_LINE_COUNT = 0;
    for (; index != 0; index--) {
        uint8_t len;
        do {
            len = *msg;
            msg += len + 1;
        } while (len != 0);
    }
    display_message_loop(msg);
}

/* Each string is printed centered on row 23, the bottom of the three
 * status lines, which scroll up between strings. The message pauses,
 * with the blink running, after every string but a first one that has
 * more after it. MSG_LINE_COUNT counts the strings, from whatever the
 * caller left in it. */
void display_message_loop(const uint8_t *msg) {
    for (;;) {
        if (*msg == 0) {
            if (MSG_LINE_COUNT == 2)
                scroll_status_line();
            return;
        }
        /* The status lines scroll after the first string is printed,
         * and before the third and each later one. The test for the
         * first is the original's CMP #1 and BMI. */
        uint8_t count = MSG_LINE_COUNT;
        bool first = (int8_t)(count - 1) < 0;
        if (!first && count != 1)
            scroll_status_line();
        print_bottom_centered(msg);
        if (first)
            scroll_status_line();
        MSG_LINE_COUNT++;
        msg += *msg + 1;
        if (MSG_LINE_COUNT <= 1 && *msg != 0)
            continue;
        delay_with_animation();
        print_ctrl_ab();
    }
}

/* Moves the text window's top left corner to column 0 of `row`. */
static void set_text_window_upper_left(uint8_t row) {
    TEXT_ROW = row;
    TEXT_COL = 0;
    hrcg_cout(HRCG_CTRL_V);
}

/* Makes a line feed at the window's bottom scroll the window. */
static void set_text_window_scroll(void) {
    hrcg_cout(HRCG_CTRL_O);
    hrcg_cout(HRCG_CTRL_S);
}

/* With the window set to the status lines, rows 21-23, a ctrl-C on
 * row 23 scrolls them. ctrl-C moves the cursor to the column the last
 * ctrl-B saved, and does a line feed when that column is at or left of
 * the cursor, which ctrl-V has just put at column 0. The saved column
 * is 0 after hrcg_init and after every ctrl-D, which ends each glyph
 * render_font_char prints. */
void scroll_status_line(void) {
    set_text_window_scroll();
    set_text_window_upper_left(21);
    TEXT_ROW = 23;
    hrcg_cout(HRCG_CTRL_C);
    set_text_window_upper_left(0);
    set_text_window_wrap();
}

static void set_cursor_row(uint8_t row) {
    TEXT_ROW = row;
    TEXT_COL = 0;
    print_ctrl_ab();
    hrcg_cout(HRCG_CTRL_F);
}

void set_cursor_row21(void) {
    set_cursor_row(21);
}

void set_text_window_bottom(void) {
    set_cursor_row(20);
}

void print_bottom_centered(const uint8_t *s) {
    TEXT_ROW = 23;
    TEXT_COL = 0;
    print_centered_plus_col(s);
}

/* The string starts (40 - length) / 2 columns from TEXT_COL, rounded
 * up, because the original's ADC takes in the bit its LSR shifts out.
 * The original's comment gives the division without the rounding. */
void print_centered_plus_col(const uint8_t *s) {
    uint8_t space = 40 - s[0];
    TEXT_COL += (space >> 1) + (space & 1);
    print_ctrl_ab();
    print_string(s);
}

void set_inverse_video(void) {
    hrcg_cout(HRCG_CTRL_I);
}

void print_ctrl_n(void) {
    hrcg_cout(HRCG_CTRL_N);
}

void set_text_window_wrap(void) {
    hrcg_cout(HRCG_CTRL_O);
    hrcg_cout(HRCG_CTRL_W);
}

/* Selects the standard font. */
void print_ctrl_ab(void) {
    hrcg_cout(HRCG_CTRL_A);
    hrcg_cout(0xB0);    /* '0' */
}

/* The original counts the length down in X and stops when it goes
 * negative, so a length above 128 prints nothing. */
void print_string(const uint8_t *s) {
    uint8_t n = *s;
    if (n == 0 || n > 128)
        return;
    do {
        hrcg_cout(*++s);
    } while (--n);
}
