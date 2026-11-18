/*
    display.c -- the status bar, the names printed under the map, and
    the stats screen.

    display_status_bar clears the text window and draws the status bar's
    border from its text stream, then the prompt to press the space bar
    or the button. setup_text_pos and setup_text_pos_col20 print a name
    centered in the left or right half of row 20. display_char_stats
    writes the current mob's numbers into the stats screen's template,
    prints it from the start of row 21, and waits for a key or a button
    before clear_map prints the location's name again.

    Departures from the original:

    - DISPLAY_STATUS_BAR calls TEXT_RENDERER, which calls
      DISPLAY_STATUS_BAR at a $7E in its stream. That would be recursion
      in C, so display_status_bar calls text_renderer_no_pause (text.c),
      the renderer without the $7E case. STATUS_BORDER_DATA has no $7E.
    - SETUP_TEXT_POS and SETUP_TEXT_POS_COL20 are two entries into one
      body, which is print_panel_name with the column as a parameter.
      The name, PRINT_STRING_ADDR in the original, is a parameter too.
    - PRINT_STRING_AT_ADDR, which takes the string in X and Y, is
      print_string, and the JMPs to PRINT_STRING, ROM_COUT1, and
      CLEAR_MAP are calls followed by a return.

    Several of the original's comments describe something other than
    what the code does, and the C follows the code. DISPLAY_STATUS_BAR's
    comments call the stores to TEXT_ROW and TEXT_COL WNDBTM and
    WNDLFT. SETUP_TEXT_POS's header says it sends a carriage return,
    where it sends $8C, ctrl-L, which selects lower case.
    DISPLAY_CHAR_STATS's comments put the cursor at row 0 of columns 21
    and 20, where the code sets rows 21 and 20 at column 0. They also
    name the armor, dagger, speed, and gold fields "group count",
    "ducats", "armor", and "experience", and call the gold's tens and
    ones places its hundreds and tens.

    Ali Baba, main.asm lines 12787-12828, 14500-14538, and 14740-14895
    (the ORG chunks at $6D1F, $76D2, and $7780):

        ORG     $6D1F
    DISPLAY_STATUS_BAR:
        SUBROUTINE DISPLAY_STATUS_BAR
        ; Draw the status bar border and input-prompt string.
        ;
        ; Inputs:
        ;   INPUT_MODE          — 0 = keyboard, nonzero = joystick
        ;   STATUS_BORDER_DATA  — address of border-box text data
        ;
        ; Behavior:
        ;   Sends ctrl-P ($90) to HRCG to toggle inverse video, then draws
        ;   the border box via TEXT_RENDERER.  Sets text row to 23, column
        ;   to 4, and prints S_PRESS_SPACE ("PRESS SPACE BAR") when
        ;   INPUT_MODE is 0, or S_PRESS_BUTTON ("PRESS THE BUTTON")
        ;   otherwise.
        ;
        ; Modifies:
        ;   TEXT_STREAM_PTR, PRINT_STRING_ADDR, TEXT_ROW, TEXT_COL
        ; Clobbers: A
        LDA     #$90                               ; ctrl-P with high bit (toggle inverse)
        JSR     ROM_COUT1
        STOW    STATUS_BORDER_DATA,TEXT_STREAM_PTR ; status bar border data -> $06/$07
        JSR     TEXT_RENDERER                      ; draw border box
        JSR     PRINT_CTRL_AB                      ; Ctrl-A, '0'
        STOB    #$17,TEXT_ROW                      ; WNDBTM = 23

        LDA     INPUT_MODE                         ; 0 = keyboard, nonzero = joystick
        BNE     .joystick

        ; Keyboard path:
        STOB    #<S_PRESS_SPACE,PRINT_STRING_ADDR
        LDA     #>S_PRESS_SPACE
    .store_hi_and_print:
        STA     PRINT_STRING_ADDR+1
    .set_margin:
        STOB    #$04,TEXT_COL ; WNDLFT = 4
        JMP     PRINT_STRING                       ; print string, return

    .joystick:
        STOB    #<S_PRESS_BUTTON,PRINT_STRING_ADDR
        LDA     #>S_PRESS_BUTTON
        JMP     .store_hi_and_print                ; reuse STA PRINT_STRING_ADDR+1 from keyboard path
    ...
        ORG     $76D2
        SUBROUTINE SETUP_TEXT_POS
        ; Prepare an HRCG text window at row 20 for a status panel.
        ;
        ; Behavior:
        ;   SETUP_TEXT_POS sets the starting column to 0 (left panel);
        ;   SETUP_TEXT_POS_COL20 sets it to MAP_WIDTH (right panel).
        ;   Both entry points set TEXT_ROW to 20, send HRCG ctrl-AB and a
        ;   carriage return, fill 20 columns with hi-bit spaces, compute
        ;   TEXT_COL = 10 - TEXT_COL as a centering offset, call
        ;   PRINT_CENTERED_PLUS_COL, and finalize with ctrl-K.
        ;
        ; Modifies:
        ;   TEXT_COL, TEXT_ROW
        ; Clobbers: A, X
    SETUP_TEXT_POS:
        STOB    #$00,TEXT_COL
        BEQ     .set_row
    SETUP_TEXT_POS_COL20:          ; $76D8 - original SETUP_TEXT_POS_COL20 entry
        STOB    #MAP_WIDTH,TEXT_COL
    .set_row:
        STOB    #MAP_WIDTH,TEXT_ROW
        JSR     PRINT_CTRL_AB
        LDA     #$8C
        JSR     ROM_COUT1
        LDX     #MAP_WIDTH
    .space_loop:
        LDA     #$A0
        JSR     ROM_COUT1
        DEX
        BNE     .space_loop
        STOB    #MAP_WIDTH,TEXT_ROW
        LDA     #$0A
        SEC
        SBC     TEXT_COL
        STA     TEXT_COL
        JSR     PRINT_CENTERED_PLUS_COL
        LDA     #$8B
        JMP     ROM_COUT1
    ...
        ORG     $7780
    DISPLAY_CHAR_STATS:
        SUBROUTINE DISPLAY_CHAR_STATS
        ; Format and display entity stats.
        ;
        ; Input:  ENTITY_PTR     -- mob/entity record
        ;         INPUT_MODE     -- 0 = keyboard, nonzero = joystick
        ; Output: screen updated with stats; waits for keypress
        ; Behavior:
        ;   - Reads 8 fields from entity record, converts to decimal
        ;   - Writes digits into STATS_TEMPLATE, displays it
        ;   - Shows mode-appropriate input prompt
        ;   - Polls keyboard/joystick until input received
        ;   - Exits via CLEAR_MAP
        ; Clobbers: A, X, Y

        ; --- Clear digit positions in the stats template ---
        LDA     #$A0              ; space (high-bit ASCII)
        STA     STATS_LIFE        ; life force digits
        STA     STATS_DUCATS      ; \
        STA     STATS_DUCATS+1    ;  | ducats digits
        STA     STATS_DUCATS+2    ; /
        STA     STATS_SWORD       ; sword digits
        STA     STATS_DAGGR       ; dagger digits
        STA     STATS_STR         ; strength digits
        STA     STATS_SPEED       ; speed digits
        STA     STATS_DEXT        ; dexterity digits

        ; --- Format life force: field 6, bits 5-0 ---
        LDA     #$00
        STA     NUM_VALUE+1                ; high byte = 0
        LDY     #MFIELD_HP
        LDA     (ENTITY_PTR),Y            ; field 6
        AND     #HP_MASK                   ; bits 5-0 = life force
        STA     NUM_VALUE
        STOW    STATS_LIFE_OUT,DECIMAL_OUT_PTR
        JSR     NUM_TO_DECIMAL

        ; --- Format group count: field 8, bits 3-0 ---
        LDY     #MFIELD_XP
        LDA     (ENTITY_PTR),Y            ; field 8
        AND     #LOW_NIBBLE_MASK           ; bits 3-0 = group count
        STA     NUM_VALUE
        STOW    STATS_WEAP_OUT,DECIMAL_OUT_PTR
        JSR     NUM_TO_DECIMAL

        ; --- Format weapons: field 7, bits 3-0, plus 1 ---
        LDY     #MFIELD_ATTACK
        LDA     (ENTITY_PTR),Y            ; field 7
        AND     #LOW_NIBBLE_MASK           ; bits 3-0
        CLC
        ADC     #$01                       ; +1
        STA     NUM_VALUE
        STOW    STATS_SWORD_OUT,DECIMAL_OUT_PTR
        JSR     NUM_TO_DECIMAL

        ; --- Format ducats: field 9, bits 3-0, plus 1 ---
        LDY     #MFIELD_DEFENSE
        LDA     (ENTITY_PTR),Y            ; field 9
        AND     #LOW_NIBBLE_MASK           ; bits 3-0
        CLC
        ADC     #$01                       ; +1
        STA     NUM_VALUE
        STOW    STATS_DAGGR_OUT,DECIMAL_OUT_PTR
        JSR     NUM_TO_DECIMAL

        ; --- Format strength: field 5, bits 4-0 ---
        LDY     #MFIELD_STRENGTH
        LDA     (ENTITY_PTR),Y            ; field 5
        AND     #STRENGTH_MASK             ; bits 4-0 = strength
        STA     NUM_VALUE
        STOW    STATS_STR_OUT,DECIMAL_OUT_PTR
        JSR     NUM_TO_DECIMAL

        ; --- Format armor: field 12, bits 3-0 ---
        LDY     #MFIELD_LEVEL_ENC
        LDA     (ENTITY_PTR),Y            ; field 12
        AND     #LOW_NIBBLE_MASK           ; bits 3-0
        STA     NUM_VALUE
        STOW    STATS_SPEED_OUT,DECIMAL_OUT_PTR
        JSR     NUM_TO_DECIMAL

        ; --- Format dexterity: field 11, bits 4-0 ---
        LDY     #MFIELD_LEVEL
        LDA     (ENTITY_PTR),Y            ; field 11
        AND     #LEVEL_MASK                ; bits 4-0 = dexterity
        STA     NUM_VALUE
        STOW    STATS_DEXT_OUT,DECIMAL_OUT_PTR
        JSR     NUM_TO_DECIMAL

        ; --- Format experience: fields 13-14 (16-bit) ---
        LDY     #MFIELD_GOLD
        LDA     (ENTITY_PTR),Y            ; field 13 (low byte)
        STA     NUM_VALUE
        INY
        LDA     (ENTITY_PTR),Y            ; field 14 (high byte)
        STA     NUM_VALUE+1
        STOW    STATS_DUCATS_OUT,DECIMAL_OUT_PTR
        JSR     NUM_TO_DECIMAL

        ; --- Cosmetic: if hundreds digit is blank and tens is "0", blank it ---
        LDA     STATS_DUCATS+2    ; hundreds digit
        CMP     #$A0              ; blank (leading-zero suppressed)?
        BNE     .show             ; no -> 3+ digits, show as-is
        LDA     STATS_DUCATS+3    ; tens digit
        CMP     #$B0              ; "0"?
        BNE     .show             ; no -> show as-is
        LDA     #$A0              ; replace "0" with blank
        STA     STATS_DUCATS+3

        ; --- Display the stats ---
    .show:
        JSR     SCROLL_STATUS_LINE
        JSR     PRINT_CTRL_AB              ; save cursor state
        LDA     #$15                       ; \  cursor to column 21,
        STA     $25                        ;  | row 0 (right half of
        LDA     #$00                       ;  | screen)
        STA     $24                        ; /
        LDA     #<STATS_TEMPLATE           ; \ print the stats template
        STA     PRINT_STRING_ADDR          ;  | string at [[STATS_TEMPLATE]]
        LDA     #>STATS_TEMPLATE           ;  |
        STA     PRINT_STRING_ADDR+1        ; /
        JSR     PRINT_STRING               ; display stats
        LDA     #$14                       ; \  cursor to column 20,
        STA     $25                        ;  | row 0
        LDA     #$00                       ;  |
        STA     $24                        ; /

        ; --- Show input prompt based on mode ---
        LDA     INPUT_MODE
        BEQ     .keyboard                  ; 0 = keyboard mode
        LDX     #$18                       ; \ joystick: "PUSH BUTTON
        LDY     #$7E                       ; / TO GO ON" at [[STATS_PROMPT_JOYSTICK]]
        BNE     .print_prompt
    .keyboard:
        LDX     #$01                       ; \ keyboard: "PRESS M TO
        LDY     #$7E                       ; / CONTINUE" at [[STATS_PROMPT_KEYBOARD]]
    .print_prompt:
        JSR     PRINT_STRING_AT_ADDR       ; print prompt
        JSR     ANIM_TICK_AND_WAIT         ; tick animation + wait

        ; --- Wait for input ---
        STA     $C010                      ; clear keyboard strobe
    .wait_key:
        LDA     $C000                      ; read keyboard
        BMI     .done                      ; key pressed -> exit
        LDA     INPUT_MODE
        BEQ     .wait_key                  ; keyboard mode -> loop
        LDX     #$00
        LDA     $C061,X                    ; read joystick button 0
        BMI     .done                      ; pressed -> exit
        INX
        LDA     $C061,X                    ; read joystick button 1
        BPL     .wait_key                  ; not pressed -> loop
    .done:
        JMP     CLEAR_MAP                  ; exit via CLEAR_MAP
*/
#include "ali_baba.h"

/* A space with bit 7 set, as the HRCG draws it. */
#define SPACE 0xA0

void display_status_bar(void) {
    hrcg_cout(HRCG_CTRL_P);
    TEXT_STREAM_PTR = STATUS_BORDER_DATA;
    text_renderer_no_pause();
    print_ctrl_ab();
    TEXT_ROW = 23;
    const uint8_t *prompt = INPUT_MODE != 0 ? S_PRESS_BUTTON : S_PRESS_SPACE;
    TEXT_COL = 4;
    print_string(prompt);
}

/* Clears 20 columns of row 20 from column `col`, in lower case, and
 * prints `name` centered in them, then selects upper case. The centering
 * starts from 10 minus the column the HRCG's cursor reached after the
 * spaces, which is what the original computes, and print_centered_plus_col
 * adds half the space the name leaves. SETUP_TEXT_POS and
 * SETUP_TEXT_POS_COL20. */
static void print_panel_name(uint8_t col, const uint8_t *name) {
    TEXT_COL = col;
    TEXT_ROW = 20;
    print_ctrl_ab();
    hrcg_cout(HRCG_CTRL_L);
    for (uint8_t i = MAP_WIDTH; i != 0; i--)
        hrcg_cout(SPACE);
    TEXT_ROW = 20;
    TEXT_COL = 10 - TEXT_COL;
    print_centered_plus_col(name);
    hrcg_cout(HRCG_CTRL_K);
}

void setup_text_pos(const uint8_t *name) {
    print_panel_name(0, name);
}

void setup_text_pos_col20(const uint8_t *name) {
    print_panel_name(MAP_WIDTH, name);
}

void display_char_stats(void) {
    /* num_to_decimal leaves the places of leading zeros as they are, so
     * the places that would show a stale digit are cleared first. */
    *STATS_LIFE = SPACE;
    STATS_DUCATS[0] = SPACE;
    STATS_DUCATS[1] = SPACE;
    STATS_DUCATS[2] = SPACE;
    *STATS_SWORD = SPACE;
    *STATS_DAGGR = SPACE;
    *STATS_STR = SPACE;
    *STATS_SPEED = SPACE;
    *STATS_DEXT = SPACE;

    num_to_decimal(ENTITY_PTR->life & HP_MASK, STATS_LIFE_OUT);
    num_to_decimal(ENTITY_PTR->armor & LOW_NIBBLE_MASK, STATS_WEAP_OUT);
    num_to_decimal((ENTITY_PTR->sword & LOW_NIBBLE_MASK) + 1, STATS_SWORD_OUT);
    num_to_decimal((ENTITY_PTR->dagger & LOW_NIBBLE_MASK) + 1, STATS_DAGGR_OUT);
    num_to_decimal(ENTITY_PTR->strength & STRENGTH_MASK, STATS_STR_OUT);
    num_to_decimal(ENTITY_PTR->speed & LOW_NIBBLE_MASK, STATS_SPEED_OUT);
    num_to_decimal(ENTITY_PTR->level & LEVEL_MASK, STATS_DEXT_OUT);
    num_to_decimal(ENTITY_PTR->gold, STATS_DUCATS_OUT);

    /* STATS_DUCATS[2] and [3] are the gold's tens and ones places. A
     * blank tens place and a '0' in the ones place mean no gold, and
     * the 0 is blanked, so that no gold shows as no digits. The
     * original's comments call these the hundreds and tens places. */
    if (STATS_DUCATS[2] == SPACE && STATS_DUCATS[3] == 0xB0)
        STATS_DUCATS[3] = SPACE;

    scroll_status_line();
    print_ctrl_ab();
    TEXT_ROW = 21;
    TEXT_COL = 0;
    print_string(STATS_TEMPLATE);
    TEXT_ROW = 20;
    TEXT_COL = 0;
    print_string(INPUT_MODE != 0 ? STATS_PROMPT_JOYSTICK : STATS_PROMPT_KEYBOARD);

    /* The original stores anim_tick_and_wait's A, the frame counter, to
     * clear the strobe. */
    KBDSTRB = anim_tick_and_wait();
    for (;;) {
        if (KBD & 0x80)
            break;
        if (INPUT_MODE == 0)
            continue;
        if ((BUTN[0] & 0x80) || (BUTN[1] & 0x80))
            break;
    }
    clear_map();
}
