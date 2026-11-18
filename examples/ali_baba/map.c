/*
    map.c -- the location on the screen, the scenes read from the disk,
    and the RWTS calls the other units read and write the disk with.

    A location's map is drawn in 2 x 2 cells of the HRCG's fonts. When
    the game moves to another location, enter_location fills the map
    with the style's tile if the style changed, draws each cell between
    the location's corners, and the first time the location is drawn
    shows its treasures and its mobs one at a time, each named on the
    bottom line and blinking. A scene is a sector of text that
    scene_loop reads from the disk and draws over the map, followed by
    the title tune, until the scene's last byte names no next scene.
    Then the location is drawn again.

    Departures from the original:

    - LOAD_MAP_TILES reads DEFAULT_CHAR and BLINK_ALT_CHAR from
      TILE_DEFAULT_TAB and TILE_BLINK_TAB, which hold the same byte for
      every style, and load_map_tiles stores those bytes as constants.
      It stores them in the original's order.
    - DRAW_MAP_COL computes the cell's position with a loop that stops
      adding when the sum passes 255, and draw_map_col uses
      colrow_to_pos. No location's corner is below row 9, so the sums
      never pass 255.
    - CHECK_AT_BOUNDARY reads the position from LOCATION_ID, which
      FIND_ENTITY_AT_POS stored there, and check_at_boundary takes it as
      a parameter. It returns ENTITY_EDGE or ENTITY_NONE where the
      original stores 1 or 0 in $BE/$BF.
    - SCENE_LOOP calls SAVE_ZP_CONTEXT and RESTORE_ZP_CONTEXT, which
      copy $BC, ENTITY_PTR, $BE, and MOB_PTR to $5A9A-$5AA1 and back.
      scene_loop copies ENTITY_PTR and MOB_PTR to SAVED_ENTITY_PTR and
      SAVED_MOB_PTR and back, and the other two are locals and
      parameters in C.
    - LOAD_SCENE_FROM_DISK calls DISK_RTS, an RTS, three times, and
      reads through DISK_READ_JUMP, a JMP to DISK_READ.
      load_scene_from_disk calls disk_read. It takes the scene as a
      parameter where the original reads SCENE_NUMBER, and keeps the
      track and the count of tries in locals where the original uses
      $BE.
    - INTEGRITY_FAIL_ADDR is the error tail of LOAD_SCENE_DATA, which
      VERIFY_INTEGRITY jumps to. integrity_fail is a function that
      load_scene_data and verify_integrity call.
    - RWTS_DISPATCH, DISK_READ, and DISK_WRITE return the carry, and
      their C versions return true on an error.
    - TUNE_PLAYER takes the tune number times 2 in X, and tune_player
      the number itself. The original's Y is whatever the routine
      before left, which matters only for tunes 1 and 2, so the C
      passes 0.
    - CLEAR_STRENGTH_FLAGS, LOAD_SCENE_DISK_ENTRY, and
      SETUP_AND_READ_DISK have no callers that run and aren't
      translated. TUNE_END_MARKER, the byte at $A440, stays with the
      tunes.

    Ali Baba, main.asm lines 1432-1454, 1664-1723, 2035-2056,
    12597-12629, 12648-12697, 12903-12932, 13827-14076, 14470-14474,
    the variables at lines 15312-15385, and lines 16987-17000 and
    17040-17105, with the lines that hold only a comment left out:

        ORG     $0901
    SETUP_SCENE_DISPLAY:
        SUBROUTINE SETUP_SCENE_DISPLAY
        STOB    #$00,AI_WAIT_FLAG       ; assume player: no wait
        BANE    CHAR_AI_MODE,_SETUP_SCENE_DISPLAY.not_ai    ; branch if not AI
        STOB    #$0A,AI_WAIT_FLAG       ; AI: set 10-tick delay
        LDX     #$10                    ; tune 8: AI turn effect
        JSR     TUNE_PLAYER
    _SETUP_SCENE_DISPLAY.not_ai:
        JSR     SETUP_CHAR_SPRITE       ; draw character sprite
        JSR     LOAD_CHAR_NAME_PTR      ; load character name pointer
        LDA     AI_WAIT_FLAG            ; 0 or 10
        ROL                             ; shift left: 0->0, 10->20
        STA     WAIT_LOOP_COUNT         ; store final wait count
        JMP     TIMED_WAIT              ; wait and return
    ...

        ORG     $0A17
    SCENE_SETUP:
        SUBROUTINE SCENE_SETUP
        LDA     ACTIVE_CHAR                ; scene/character index
        JSR     GET_CHAR_RECORD            ; -> CHAR_REC = 9-byte record
        LDY     #LFIELD_MIN_POS            ; byte 3: top-left bounding position
        LDA     (CHAR_REC),Y
        JSR     POS_TO_COLROW              ; -> A = min_col, Y = min_row
        STA     BOUNDS_MIN_COL
        STY     BOUNDS_MIN_ROW
        LDY     #LFIELD_MAX_POS            ; byte 4: bottom-right bounding position
        LDA     (CHAR_REC),Y
        JSR     POS_TO_COLROW              ; -> A = max_col, Y = max_row
        STA     BOUNDS_MAX_COL
        STY     BOUNDS_MAX_ROW
        INC     BOUNDS_MIN_COL             ; exclusive border: min_col++
        INC     BOUNDS_MIN_ROW             ; exclusive border: min_row++
        CLC
        LDA     BOUNDS_MAX_COL             ; \
        SBC     BOUNDS_MIN_COL             ;  | col_range = max - min
        JSR     RANDOM_IN_RANGE            ;  | random offset in [0, range)
        CLC                                ;  |
        ADC     BOUNDS_MIN_COL             ;  | col = min + offset
        STA     FONT_COL                   ; / save for COLROW_TO_POS
        CLC
        LDA     BOUNDS_MAX_ROW             ; \
        SBC     BOUNDS_MIN_ROW             ;  | row_range = max - min
        JSR     RANDOM_IN_RANGE            ;  | random offset
        CLC                                ;  |
        ADC     BOUNDS_MIN_ROW             ; / row = min + offset
        TAY                                ; Y = row
        LDA     FONT_COL                   ; A = col
        JSR     COLROW_TO_POS              ; -> A = linear map position
        LDY     #MFIELD_POSITION           ; byte 3: mob map position
        STA     (MOB_PTR),Y                ; store new position
        MOVW    MOB_PTR,TARGET_REC         ; TARGET_REC = MOB_PTR
        STOB    #$00,SOURCE_CHAR           ; no source character
        JSR     REORDER_CHAR               ; splice into linked list
        LDY     #MFIELD_GOLD            ; byte 13: gold low byte
        LDA     #$00                       ; \
        STA     (MOB_PTR),Y                ;  | clear gold counter
        INY                                ;  | Y = MFIELD_GOLD+1
        STA     (MOB_PTR),Y                ; /
        JSR     CLAMP_CHAR_FIELD           ; enforce byte 6 >= byte 5
        LDA     CURRENT_PLAYER             ; current player index
        BANE    ACTIVE_CHAR,_SCENE_SETUP.done          ; not current player -> skip draw
        LDY     #MFIELD_POSITION           ; byte 3: mob map position
        LDA     (MOB_PTR),Y                ; reload position
        JSR     DRAW_CHAR_AT_POS           ; render character at position
    _SCENE_SETUP.done:
        RTS
    ...

        ORG     $0BFA
    CHECK_AT_BOUNDARY:
        SUBROUTINE CHECK_AT_BOUNDARY
        LDA     LOCATION_ID            ; current map position
        JSR     POS_TO_COLROW          ; A = col, Y = row
        BAEQ    VIEW_MIN_COL,_CHECK_AT_BOUNDARY.at_edge  ; on left edge?
        BAEQ    VIEW_MAX_COL,_CHECK_AT_BOUNDARY.at_edge  ; on right edge?
        TYA                            ; row -> A
        BAEQ    VIEW_MIN_ROW,_CHECK_AT_BOUNDARY.at_edge  ; on top edge?
        BAEQ    VIEW_MAX_ROW,_CHECK_AT_BOUNDARY.at_edge  ; on bottom edge?
        STOB    #$00,DATA_PTR          ; interior: result = 0
        STA     DATA_PTR+1
        RTS
    _CHECK_AT_BOUNDARY.at_edge:
        STOW2    $0001,DATA_PTR        ; boundary: result = 1
        RTS
    ...

        ORG     $6C12
    DISK_READ:
        LDA     #$01               ; command = read
        BNE     RWTS_DISPATCH      ; always taken
    DISK_WRITE:
        LDA     #$02               ; command = write
    RWTS_DISPATCH:
        SUBROUTINE RWTS_DISPATCH
        LDY     #$0C
        STA     ($08),Y            ; IOB+$0C = command (A)
        LDA     #$00
        LDY     #$03
        STA     ($08),Y            ; IOB+$03 = volume (0 = any)
        LDA     $09                ; \ A/Y = IOB pointer
        LDY     $08                ; /
        JSR     RWTS_ENTRY         ; execute RWTS command
        STOB    #$00,$48           ; clear disk status
        RTS
    ...

        ORG     $6C37
    SCENE_LOOP:
        SUBROUTINE SCENE_LOOP
        STA     SCENE_NUMBER               ; store incoming scene number
        STOB    #MAP_WIDTH,BLINK_COL       ; disable blink cursor (sentinel = 20)
        JSR     SAVE_ZP_CONTEXT           ; save caller's ZP context
        JSR     STEP_PRNG                  ; advance PRNG
        LDA     PRNG_OUTPUT                ; fresh random byte
        AND     #PRNG_SCENE_SKIP_MASK      ; test bits 1 and 3
        BNE     .display                   ; ~75%: skip disk reload
        JSR     LOAD_SCENE_DATA            ; reload scene text from disk
    .display:
        JSR     LOAD_SCENE_FROM_DISK       ; load scene text from disk
        BCS     .exit                      ; carry set = disk error
        JSR     DISPLAY_STATUS_BAR         ; refresh status bar
        STOW    SCENE_TEXT_DATA,TEXT_STREAM_PTR ; point at scene data
        JSR     TEXT_RENDERER              ; render scene text
        LDX     #TUNE_ATTRACT              ; tune 6: attract/opening music
        JSR     TUNE_PLAYER                ; advance tune playback
        LDA     SCENE_TRANSITION           ; scene transition flag
        BEQ     .done                      ; zero = no transition
        STA     SCENE_NUMBER               ; nonzero = new scene number
        JMP     .display                   ; restart with new scene
    .done:
        JSR     SET_TEXT_WINDOW_BOTTOM     ; restore text window
        LDA     #$00
        STA     LOCATION_POS               ; clear location flags
        STA     LOCATION_STYLE
        JSR     ENTER_LOCATION             ; redraw map if location changed
        JMP     RESTORE_ZP_CONTEXT        ; restore ZP and return
    .exit:
        JSR     SET_TEXT_WINDOW_BOTTOM
        LDA     #MSG_BREAKS_FREE
        JSR     DISPLAY_MESSAGE            ; "breaks free" message
        JMP     .done
    ...

        ORG     $6DD8
    SCENE_LOADER_START:
    LOAD_SCENE_DATA:
        SUBROUTINE LOAD_SCENE_DATA
        BIT     $C0E9                   ; assert disk motor on
        LDA     #$05                    ; \
        LDY     #$04                    ;  | IOB byte 4 = track 5
        STA     ($08),Y                 ; /
        LDA     #$00
        JSR     RWTS_DISPATCH           ; execute RWTS command
        CLC
        JSR     DISK_READ_FIELD         ; read sector data
        BCS     .error
        RTS                             ; success
    INTEGRITY_FAIL_ADDR:
    .error:
        LDY     PRNG_OUTPUT             ; use PRNG as index
        JSR     STEP_PRNG               ; advance PRNG
        LDA     PRNG_OUTPUT             ; load new PRNG byte
        STA     $9AA6,Y                 ; fill into message area
        RTS
    ...

        ORG     $72A7
    ENTER_LOCATION:
        SUBROUTINE ENTER_LOCATION
        LDA     CHAR_PTR               ; current location (low byte)
        BANE    LOCATION_POS,_ENTER_LOCATION.changed  ; changed since last render?
        RTS
    _ENTER_LOCATION.changed:
        STA     LOCATION_POS           ; record new position
        JSR     ANIM_TICK_AND_WAIT     ; advance animation frame
        JSR     SET_TEXT_WINDOW_BOTTOM ; configure text window
        JSR     CLEAR_MAP              ; clear map display
        STOW    CANDIDATE_POS,ENTITY_PTR ; ENTITY_PTR -> $5A15
        JSR     FIND_NEAREST_EVENT     ; find closest event
        STA     EVENT_POS              ; save event position
        LDY     #CFIELD_STYLE          ; byte 5: style | visited flag
        LDA     (CHAR_PTR),Y
        AND     #STYLE_ID_MASK         ; strip visited flag (bit 7)
        BAEQ    LOCATION_STYLE,_ENTER_LOCATION.same_style ; style unchanged?
        STA     LOCATION_STYLE         ; update current style
        JSR     LOAD_MAP_TILES         ; reload tile set
        JSR     FILL_MAP               ; fill map with new tiles
    _ENTER_LOCATION.same_style:
        LDY     #LFIELD_MIN_POS        ; byte 3: min viewport position
        LDA     (CHAR_PTR),Y
        JSR     POS_TO_COLROW          ; A = col, Y = row
        STY     VIEW_MIN_ROW           ; set top edge
        STY     FONT_ROW               ; row iterator starts at top
        STA     VIEW_MIN_COL           ; set left edge
        LDY     #LFIELD_MAX_POS        ; byte 4: max viewport position
        LDA     (CHAR_PTR),Y
        JSR     POS_TO_COLROW          ; A = col, Y = row
        STY     VIEW_MAX_ROW           ; set bottom edge
        STA     VIEW_MAX_COL           ; set right edge
    _ENTER_LOCATION.row_loop:
        MOVB    VIEW_MIN_COL,FONT_COL  ; reset column to left edge
    _ENTER_LOCATION.col_loop:
        JSR     DRAW_MAP_COL           ; draw tile at (FONT_COL, FONT_ROW)
        INC     FONT_COL
        LDA     VIEW_MAX_COL
        BAPL    FONT_COL,_ENTER_LOCATION.col_loop     ; next column
        INC     FONT_ROW
        LDA     VIEW_MAX_ROW
        BAPL    FONT_ROW,_ENTER_LOCATION.row_loop     ; next row
        LDY     #CFIELD_STYLE          ; byte 5: check visited flag
        LDA     (CHAR_PTR),Y
        BMI     .skip_entities         ; bit 7 set = visited, skip
        JSR     DRAW_MAP_ENTITIES      ; overlay entity icons
    .skip_entities:
        RTS
        ORG     $7323
    DRAW_MAP_COL:
        SUBROUTINE DRAW_MAP_COL
        LDA     FONT_COL               ; A = column
        CLC
        LDY     FONT_ROW               ; Y = row counter
    .mult_loop:
        DEY                            ; row--
        BMI     .mult_done             ; Y < 0 -> done
        ADC     #MAP_WIDTH             ; A += 20 (one row)
        BCC     .mult_loop
    .mult_done:
        JSR     FIND_ENTITY_AT_POS     ; find entity at position
        JSR     GET_ENTITY_FONTCHAR    ; resolve font character
        JSR     RENDER_FONT_CHAR       ; draw it
        RTS
        ORG     $733B
    DRAW_MAP_ENTITIES:
        SUBROUTINE DRAW_MAP_ENTITIES
        LDY     #CFIELD_STYLE          ; byte 5: style + visited flag
        LDA     (CHAR_PTR),Y
        ORA     #CFIELD_DRAWN_FLAG     ; set bit 7 = entities drawn
        STA     (CHAR_PTR),Y
        LDA     MAP_REDRAW_FLAG        ; full redraw pending?
        BEQ     .no_redraw
        JMP     .draw_mobs             ; skip grid scan
    .no_redraw:
        LDA     LOCATION_STYLE
        AND     #STYLE_SCROLL_MASK     ; bits 6-5: scroll trigger
        BEQ     .scan_grid             ; 0 = normal scan
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_ROCK_FALLS
        JSR     DISPLAY_MESSAGE
        STOB    #$FF,MAP_REDRAW_FLAG   ; schedule full redraw
        BNE     .draw_mobs             ; always taken
    .scan_grid:
        MOVB    VIEW_MIN_COL,FONT_COL  ; start at top-left of view
        MOVB    VIEW_MIN_ROW,FONT_ROW
        LDY     #CFIELD_NEXT           ; byte 2: mob head link
        LDA     (CHAR_PTR),Y
        PHA                            ; save mob head
        LDA     #$00
        STA     (CHAR_PTR),Y           ; clear during scan
    _DRAW_MAP_ENTITIES.cell_check:
        LDA     FONT_COL
        LDY     FONT_ROW
        JSR     COLROW_TO_POS          ; A = packed position
        JSR     FIND_ENTITY_AT_POS     ; -> ENTITY_REC
        LDA     ENTITY_REC+1           ; high byte of entity pointer
        BEQ     _DRAW_MAP_ENTITIES.next_col              ; 0 = no entity here
        JSR     GET_ENTITY_FONTCHAR    ; resolve font character
        LDY     #EVFIELD_TYPE          ; byte 1: event type
        LDA     (ENTITY_REC),Y
        BAEQ    #EVENT_EMPTY,_DRAW_MAP_ENTITIES.next_col ; $FE = empty slot, skip
        BALT    #$80,_DRAW_MAP_ENTITIES.draw_it          ; $00-$7F = drawable
        BALT    #EVENT_ACTIVATED,_DRAW_MAP_ENTITIES.next_col ; $80-$BF = not activated, skip
    _DRAW_MAP_ENTITIES.draw_it:                          ; $C0-$FF = activated, draw
        JSR     SET_CURSOR_ROW21
        JSR     CLASSIFY_ENTITY_TYPE
        JSR     PRINT_BOTTOM_CENTERED
        JSR     DRAW_MAP_ICON_B
    _DRAW_MAP_ENTITIES.next_col:
        INC     FONT_COL
        LDA     VIEW_MAX_COL
        BAPL    FONT_COL,_DRAW_MAP_ENTITIES.cell_check   ; next column
        MOVB    VIEW_MIN_COL,FONT_COL  ; wrap column, advance row
        INC     FONT_ROW
        LDA     VIEW_MAX_ROW
        BAPL    FONT_ROW,_DRAW_MAP_ENTITIES.cell_check   ; next row
        LDY     #CFIELD_NEXT           ; restore mob head link
        PLA
        STA     (CHAR_PTR),Y
        BNE     .draw_mobs             ; non-zero -> draw mobs too
        RTS
    .draw_mobs:
        JSR     FIRST_GROUP_MEMBER     ; MOB_PTR -> first mob
    .mob_loop:
        LDY     #MFIELD_POSITION       ; byte 3: packed map position
        LDA     (MOB_PTR),Y
        JSR     POS_TO_COLROW          ; A = col, Y = row
        STY     FONT_ROW
        STA     FONT_COL
        STOB    #$01,FONT_CHARSET      ; custom font
        LDY     #MFIELD_APPEARANCE     ; byte 4: appearance index
        LDA     (MOB_PTR),Y
        JSR     APPEARANCE_TO_FONTCHAR ; -> font char number
        STA     FONT_CHARNUM
        BYEQ    #$00,_DRAW_MAP_ENTITIES.mob_blink        ; char 0 -> always blink
        LDY     #MFIELD_LEVEL_ENC      ; byte 12: level encoding
        LDA     (MOB_PTR),Y
        AND     #LOW_NIBBLE_MASK       ; low nibble = base level
        BEQ     .mob_anim              ; 0 -> static render only
    _DRAW_MAP_ENTITIES.mob_blink:
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MEMBER_NAME
        JSR     DRAW_MAP_ICON_B        ; draw with blink
        JMP     .mob_done
    .mob_anim:
        JSR     RENDER_FONT_CHAR       ; draw without blink
    .mob_done:
        JSR     NEXT_GROUP_MEMBER      ; advance to next mob
        LDA     MOB_PTR+1              ; 0 = end of group
        BNE     .mob_loop
        RTS
        ORG     $740B
    DRAW_MAP_ICON_B:
        SUBROUTINE DRAW_MAP_ICON_B
        JSR     SCROLL_STATUS_LINE     ; update status line
        JSR     RENDER_FONT_CHAR       ; draw glyph
        MOVB    FONT_COL,BLINK_COL     ; arm blink cursor: column
        MOVB    FONT_ROW,BLINK_ROW     ;                   row
        MOVB    FONT_CHARNUM,BLINK_CHAR;                   character
        JSR     DRAW_BLINK_ALT         ; draw alternate blink frame
        MOVB    BLINK_DELAY,WAIT_LOOP_COUNT ; load blink duration
        JSR     TIMED_WAIT             ; pause for blink interval
        JSR     DRAW_BLINK_NORMAL      ; restore normal frame
        JSR     ANIM_TICK_AND_WAIT     ; advance animation clock
        STOB    #MAP_WIDTH,BLINK_COL   ; disable blink cursor (sentinel = 20)
        RTS
        ORG     $743B
    FILL_MAP:
        SUBROUTINE FILL_MAP
        STOB    #$13,FONT_COL           ; start at column 19 (rightmost)
        STOB    #$09,FONT_ROW           ; start at row 9 (bottom)
        MOVB    MAP_FILL_CHAR,FONT_CHARNUM ; fill character
        STOB    #$01,FONT_CHARSET       ; charset 1 = custom font
        JSR     RENDER_FONT_CHAR        ; build HRCG control string once
    .loop:
        DEC     FONT_COL                ; move one column left
        BPL     .draw                   ; col >= 0 -> draw
        STOB    #$13,FONT_COL           ; wrap to column 19
        DEC     FONT_ROW                ; move one row up
        BPL     .draw                   ; row >= 0 -> draw
        RTS                             ; all 200 positions filled
    .draw:
        JSR     FONT_POS_TO_TEXT_POS    ; set HRCG cursor
        JSR     PRINT_STRING            ; stamp pre-built glyph
        JMP     .loop
        ORG     $746C
    LOAD_MAP_TILES:
        SUBROUTINE LOAD_MAP_TILES
        LDA     LOCATION_STYLE          ; location style byte
        ROL2                            ; rotate bits 7-6 down to 1-0
        ROL2
        AND     #STYLE_INDEX_MASK       ; mask to 2-bit style index
        TAX
        LDA     TILE_BLINK_TAB,X        ; blink alternate char for this style
        STA     BLINK_ALT_CHAR
        LDA     TILE_FILL_TAB,X         ; map fill char for this style
        STA     MAP_FILL_CHAR
        LDA     TILE_DEFAULT_TAB,X      ; default char for this style
        STA     DEFAULT_CHAR
        RTS
    ...

        ORG     $76B0
    CLEAR_MAP:
        LDWIY    CHAR_PTR,#$00,PRINT_STRING_ADDR ; bytes 0-1: name ptr -> PRINT_STRING_ADDR
        JMP     SETUP_TEXT_POS
    ...

        ORG     $7AAC
    BLINK_DELAY:
        DC.B    $2D                     ; blink delay (loaded into WAIT_LOOP_COUNT)
    ...

        ORG     $7AC1
    MAP_FILL_CHAR:
        DC.B    $25                     ; map grid fill character
    DEFAULT_CHAR:
        DC.B    $2C                     ; default entity character
    BLINK_ALT_CHAR:
        DC.B    $00                     ; alternate blink character
        ORG     $7AC4
    MAP_REDRAW_FLAG:
        DC.B    $00                     ; non-zero = full map redraw needed
    ...

        ORG     $7AD0
    TILE_DEFAULT_TAB:
        HEX     2C 2C 2C 2C            ; default char per style
    TILE_FILL_TAB:
        HEX     25 2A 20 29            ; fill char per style
    TILE_BLINK_TAB:
        HEX     00 00 00 00            ; blink alt char per style
    ...

        ORG     $A440
    TUNE_END_MARKER:
        DC.B    $24               ; tune END byte (shared with TUNE_11)
    LOAD_SCENE_DISK_ENTRY:
        LDX     #$0D               ; track $0D
        LDA     #$0A               ; sector $0A
        JMP     SETUP_AND_READ_DISK
    DISK_READ_JUMP:
        JMP     DISK_READ
    DISK_RTS:
        RTS
    ...

        ORG     $A478
    LOAD_SCENE_FROM_DISK:
        SUBROUTINE LOAD_SCENE_FROM_DISK
        JSR     DISK_RTS           ; no-op (cracked-out hook)
        LDA     #$00
        LDY     #$08
        STA     (RWTS_IOB_PTR),Y   ; IOB+8 = buffer lo ($00)
        INY
        LDA     #$5C
        STA     (RWTS_IOB_PTR),Y   ; IOB+9 = buffer hi ($5C)
        LDA     GAME_SIG_TRACK     ; track = base track ($11)
        STA     $BE                ; $BE = working track number
        LDA     GAME_SIG_SECTOR    ; A = base sector ($00)
        CLC
        ADC     SCENE_NUMBER       ; A = base_sector + scene_number
    .mod13:
        CMP     #$0D               ; A >= 13?
        BCC     .set_iob           ; no -> A = final sector
        INC     $BE                ; yes -> track++
        SEC
        SBC     #$0D               ; A -= 13
        JMP     .mod13             ; repeat until A < 13
    .set_iob:
        LDY     #$05
        STA     (RWTS_IOB_PTR),Y   ; IOB+5 = sector
        DEY
        LDA     $BE
        STA     (RWTS_IOB_PTR),Y   ; IOB+4 = track
        LDA     #$02               ; 2 retries on failure
        STA     $BE
    .retry:
        JSR     DISK_READ_JUMP     ; RWTS read
        BCS     .error             ; C set = read failed
        JSR     DISK_RTS           ; no-op (cracked-out hook)
        CLC                        ; C clear = success
        RTS
    .error:
        DEC     $BE                ; retries--
        BNE     .retry             ; try again if retries remain
        JSR     DISK_RTS           ; no-op (cracked-out hook)
        SEC                        ; C set = all retries exhausted
        RTS
*/
#include "ali_baba.h"

/* ---- The map's glyphs ------------------------------------------------------ */

/* The glyph FILL_MAP stamps over the whole map, which load_map_tiles
 * picks for the location's style. GAME_INIT fills the map before any
 * location has set it, with the first style's $25. */
static uint8_t MAP_FILL_CHAR = 0x25;

/* The glyph GET_ENTITY_FONTCHAR draws for an edge or an empty cell, and
 * the one the blinking cell shows between the sprite's turns. */
uint8_t DEFAULT_CHAR = 0x2C;
uint8_t BLINK_ALT_CHAR = 0x00;

/* MAP_FILL_CHAR for each of the four styles, bits 6-5 of the location's
 * style byte. The original also has a table of DEFAULT_CHAR and one of
 * BLINK_ALT_CHAR for each style, which hold $2C and $00 for every
 * style. */
static const uint8_t TILE_FILL_TAB[4] = { 0x25, 0x2A, 0x20, 0x29 };
#define TILE_DEFAULT_CHAR 0x2C
#define TILE_BLINK_CHAR 0x00

/* Bits 6-5 of a location's style select its tiles, and a location with
 * either set gets the rock-fall message in draw_map_entities. Bit 7 is
 * set once the location's entities have been drawn. */
#define STYLE_TILES_MASK 0x60
#define STYLE_ID_MASK 0x7F
#define STYLE_DRAWN_BIT 0x80

/* Nonzero once draw_map_entities has printed the rock-fall message,
 * after which it draws only the mobs of every location. Nothing clears
 * it. */
static uint8_t MAP_REDRAW_FLAG = 0;

/* The WAIT calls draw_map_icon_b spends with the location's tile shown
 * in a cell. */
#define BLINK_DELAY 0x2D

/* Sets the tiles for LOCATION_STYLE. The original's index is LOCATION_STYLE
 * rotated left four times through the carry and masked with 3, which is
 * bits 6-5 whatever the carry was. */
static void load_map_tiles(void) {
    uint8_t style = (LOCATION_STYLE >> 5) & 3;
    BLINK_ALT_CHAR = TILE_BLINK_CHAR;
    MAP_FILL_CHAR = TILE_FILL_TAB[style];
    DEFAULT_CHAR = TILE_DEFAULT_CHAR;
}

/* Draws the map's glyph once at column 19 of row 9, and then prints the
 * same string, s_PRINT_FONT_CHAR, at each other cell from right to left
 * and from the bottom up. */
void fill_map(void) {
    FONT_COL = MAP_WIDTH - 1;
    FONT_ROW = MAP_HEIGHT - 1;
    FONT_CHARNUM = MAP_FILL_CHAR;
    FONT_CHARSET = 1;
    render_font_char();
    for (;;) {
        if ((int8_t)--FONT_COL < 0) {
            FONT_COL = MAP_WIDTH - 1;
            if ((int8_t)--FONT_ROW < 0)
                return;
        }
        font_pos_to_text_pos();
        print_string(s_PRINT_FONT_CHAR);
    }
}

void clear_map(void) {
    setup_text_pos(CHAR_PTR->name);
}

/* ---- Drawing a location ------------------------------------------------------ */

/* Draws what is at FONT_COL, FONT_ROW. The original adds 20 once per
 * row and stops adding if the sum passes 255. colrow_to_pos goes on
 * adding, which gives the same position for every location, since no
 * location's corner is below row 9. */
static void draw_map_col(void) {
    get_entity_fontchar(find_entity_at_pos(colrow_to_pos(FONT_COL, FONT_ROW)));
    render_font_char();
}

/* Shows FONT_CHARNUM in the cell at FONT_COL, FONT_ROW, then shows the
 * location's tile there for BLINK_DELAY WAIT calls, then the glyph
 * again for a tick. The blink routines draw the cell through FONT_COL,
 * FONT_ROW, and FONT_CHARNUM, which they set from BLINK_COL, BLINK_ROW,
 * and BLINK_CHAR, so those are left as they were. */
static void draw_map_icon_b(void) {
    scroll_status_line();
    render_font_char();
    BLINK_COL = FONT_COL;
    BLINK_ROW = FONT_ROW;
    BLINK_CHAR = FONT_CHARNUM;
    draw_blink_alt();
    WAIT_LOOP_COUNT = BLINK_DELAY;
    timed_wait();
    draw_blink_normal();
    anim_tick_and_wait();
    BLINK_COL = MAP_WIDTH;
}

/*
    Shows the location's treasures and mobs the first time it is drawn.
    Each treasure and each mob is named on the bottom line and blinks.

    The scan of the cells for treasures empties the location's mob list
    while it runs, so that find_entity_at_pos finds only event entries.
    The entries it shows are the encounters ($00-$7F) and the activated
    ones ($C0-$FD). Then each mob is drawn, and a player character, or
    another mob whose speed's low nibble isn't 0, blinks.

    A location with either of the style bits 6-5 set gets the rock-fall
    message in place of the scan, and the message sets MAP_REDRAW_FLAG,
    after which no location is scanned. The mobs are drawn then even
    when the list is empty, and first_group_member then goes on from the
    old MOB_PTR, as in the original, and the loop draws at least one
    mob, reading MOB_PTR = 0's
    $03, $04, and $0C when the list ends there.
*/
static void draw_map_entities(void) {
    CHAR_PTR->style |= STYLE_DRAWN_BIT;
    if (MAP_REDRAW_FLAG == 0) {
        if ((LOCATION_STYLE & STYLE_TILES_MASK) != 0) {
            set_cursor_row21();
            display_message(MSG_ROCK_FALLS);
            MAP_REDRAW_FLAG = 0xFF;
        } else {
            FONT_COL = VIEW_MIN_COL;
            FONT_ROW = VIEW_MIN_ROW;
            uint8_t head = CHAR_PTR->mob_head;
            CHAR_PTR->mob_head = 0;
            do {
                entity_ref e = find_entity_at_pos(colrow_to_pos(FONT_COL, FONT_ROW));
                if ((e >> 8) != 0) {
                    get_entity_fontchar(e);
                    uint8_t type = ENTITY_EVENT(e)->type;
                    if (type != EVENT_TYPE_OBJECT
                        && (type < 0x80 || type >= EVENT_ACTIVATED)) {
                        set_cursor_row21();
                        print_bottom_centered(classify_entity_type(e));
                        draw_map_icon_b();
                    }
                }
                FONT_COL++;
                if (FONT_COL > VIEW_MAX_COL) {
                    FONT_COL = VIEW_MIN_COL;
                    FONT_ROW++;
                }
            } while (FONT_ROW <= VIEW_MAX_ROW);
            CHAR_PTR->mob_head = head;
            if (head == 0)
                return;
        }
    }

    first_group_member();
    do {
        uint16_t colrow = pos_to_colrow(MOB_PTR->pos);
        FONT_ROW = colrow >> 8;
        FONT_COL = (uint8_t)colrow;
        FONT_CHARSET = 1;
        uint16_t glyph = appearance_to_fontchar(MOB_PTR->appearance);
        FONT_CHARNUM = (uint8_t)glyph;
        if ((glyph >> 8) == 0 || (MOB_PTR->speed & LOW_NIBBLE_MASK) != 0) {
            set_cursor_row21();
            print_member_name();
            draw_map_icon_b();
        } else {
            render_font_char();
        }
        next_group_member();
    } while (((uint16_t)MOB_PTR >> 8) != 0);
}

/*
    Draws the location at CHAR_PTR when it isn't the one drawn last,
    which LOCATION_POS records by the low byte of its record's address.
    It names the location, refills the map when the style changed,
    draws every cell from VIEW_MIN to VIEW_MAX, and then the
    treasures and mobs, unless they were drawn before.

    FIND_NEAREST_EVENT measures from ENTITY_PTR->pos, and ENTITY_PTR is
    pointed at CANDIDATE_POS ($5A15), whose record's pos byte is $5A18,
    PRNG_OUTPUT. ENTITY_PTR is left there.

    The loops compare with CMP and BPL, a signed test, which is the
    same as the unsigned one for columns 0-20 and rows 0-13.
*/
void enter_location(void) {
    uint8_t low = (uint8_t)(uint16_t)CHAR_PTR;
    if (low == LOCATION_POS)
        return;
    LOCATION_POS = low;
    anim_tick_and_wait();
    set_text_window_bottom();
    clear_map();
    ENTITY_PTR = (struct mob *)&CANDIDATE_POS;
    EVENT_POS = find_nearest_event();

    uint8_t style = CHAR_PTR->style & STYLE_ID_MASK;
    if (style != LOCATION_STYLE) {
        LOCATION_STYLE = style;
        load_map_tiles();
        fill_map();
    }

    uint16_t colrow = pos_to_colrow(CHAR_PTR->min_pos);
    VIEW_MIN_ROW = colrow >> 8;
    FONT_ROW = colrow >> 8;
    VIEW_MIN_COL = (uint8_t)colrow;
    colrow = pos_to_colrow(CHAR_PTR->max_pos);
    VIEW_MAX_ROW = colrow >> 8;
    VIEW_MAX_COL = (uint8_t)colrow;
    do {
        FONT_COL = VIEW_MIN_COL;
        do {
            draw_map_col();
            FONT_COL++;
        } while (FONT_COL <= VIEW_MAX_COL);
        FONT_ROW++;
    } while (FONT_ROW <= VIEW_MAX_ROW);

    if ((CHAR_PTR->style & STYLE_DRAWN_BIT) == 0)
        draw_map_entities();
}

/* ENTITY_EDGE when `pos` is in the first or last column or row of the
 * location drawn, which is where its walls are. */
entity_ref check_at_boundary(uint8_t pos) {
    uint16_t colrow = pos_to_colrow(pos);
    uint8_t col = (uint8_t)colrow;
    uint8_t row = colrow >> 8;
    if (col == VIEW_MIN_COL || col == VIEW_MAX_COL
        || row == VIEW_MIN_ROW || row == VIEW_MAX_ROW)
        return ENTITY_EDGE;
    return ENTITY_NONE;
}

/* ---- Turns and placing mobs ------------------------------------------------ */

/* Makes the mob whose turn it is the blinking cell and names it. The
 * original stores 0 in AI_WAIT_FLAG and compares CHAR_AI_MODE with that
 * 0, and branches past the tune when they differ, so the player's turn
 * (CHAR_AI_MODE 0) gets TUNE_AI_TURN and 20 WAIT calls, and an AI's
 * turn gets neither, although its comments say the opposite.
 *
 * The original doubles AI_WAIT_FLAG with ROL, whose carry comes from
 * the end of HRCG_COUT, reached from LOAD_CHAR_NAME_PTR's last
 * character, ctrl-K. HRCG_CTRL_K leaves the carry set only when the
 * character before it was ctrl-O, and the character before it is the
 * last of the mob's name, so the carry is clear. */
void setup_scene_display(void) {
    AI_WAIT_FLAG = 0;
    if (CHAR_AI_MODE == 0) {
        AI_WAIT_FLAG = 10;
        tune_player(TUNE_AI_TURN, 0);
    }
    setup_char_sprite();
    load_char_name_ptr();
    WAIT_LOOP_COUNT = (uint8_t)(AI_WAIT_FLAG << 1);
    timed_wait();
}

/* Moves the mob at MOB_PTR into location ACTIVE_CHAR at a random cell
 * inside its walls, from SOURCE_CHAR 0, the list of mobs at no
 * location. The walls are the corners' columns and rows, so the cell
 * is between them. */
void scene_setup(void) {
    struct location *loc = get_char_record(ACTIVE_CHAR);
    uint16_t colrow = pos_to_colrow(loc->min_pos);
    BOUNDS_MIN_COL = (uint8_t)colrow;
    BOUNDS_MIN_ROW = colrow >> 8;
    colrow = pos_to_colrow(loc->max_pos);
    BOUNDS_MAX_COL = (uint8_t)colrow;
    BOUNDS_MAX_ROW = colrow >> 8;
    BOUNDS_MIN_COL++;
    BOUNDS_MIN_ROW++;

    FONT_COL = random_in_range(BOUNDS_MAX_COL - BOUNDS_MIN_COL - 1) + BOUNDS_MIN_COL;
    uint8_t row = random_in_range(BOUNDS_MAX_ROW - BOUNDS_MIN_ROW - 1) + BOUNDS_MIN_ROW;
    MOB_PTR->pos = colrow_to_pos(FONT_COL, row);

    SOURCE_CHAR = 0;
    reorder_char(MOB_PTR);
    MOB_PTR->gold = 0;
    clamp_char_field();
    if (CURRENT_PLAYER == ACTIVE_CHAR)
        draw_char_at_pos(MOB_PTR->pos);
}

/* ---- The disk ------------------------------------------------------------- */

/* RWTS_ENTRY sets the carry on an error, and storing 0 at $48 doesn't
 * change it. */
bool rwts_dispatch(uint8_t command) {
    struct rwts_iob *iob = RWTS_IOB_PTR;
    iob->command = command;
    iob->volume = 0;
    bool error = RWTS_ENTRY(iob);
    DOS_ZP[0x48 - 0x2A] = 0;
    return error;
}

bool disk_read(void) {
    return rwts_dispatch(RWTS_READ);
}

bool disk_write(void) {
    return rwts_dispatch(RWTS_WRITE);
}

/* Steps the PRNG and writes its new output into the messages, at the
 * old output's offset from $9AA6. */
void integrity_fail(void) {
    uint8_t offset = PRNG_OUTPUT;
    step_prng();
    (GAME_MESSAGE_STRINGS + 1)[offset] = PRNG_OUTPUT;
}

/* The copy protection's check. Seeks to track 5 and has the disk
 * reader check its data field there, and on a failure writes a random
 * byte into the messages. The original's VERIFY_DISK_IO checksums this
 * routine's code. */
static void load_scene_data(void) {
    (void)DISK_MOTOR_ON;
    RWTS_IOB_PTR->track = 5;
    rwts_dispatch(RWTS_SEEK);
    if (DISK_READ_FIELD())
        integrity_fail();
}

/* The scenes are sectors of 13-sector tracks from GAME_SIG_TRACK and
 * GAME_SIG_SECTOR on, one scene to a sector. Reads scene `scene` into
 * SCENE_TEXT_DATA, trying twice, and returns true when both reads
 * failed. */
[[c6502::section(".text.scene_disk")]]
static bool load_scene_from_disk(uint8_t scene) {
    struct rwts_iob *iob = RWTS_IOB_PTR;
    iob->buffer_addr = SCENE_TEXT_DATA;
    uint8_t track = GAME_SIG_TRACK;
    uint8_t sector = GAME_SIG_SECTOR + scene;
    while (sector >= 13) {
        track++;
        sector -= 13;
    }
    iob->sector = sector;
    iob->track = track;
    uint8_t tries = 2;
    while (disk_read()) {
        if (--tries == 0)
            return true;
    }
    return false;
}

/* ---- Scenes ---------------------------------------------------------------- */

/* The bits of a new PRNG_OUTPUT that skip the copy protection's check
 * when either is set, which is three times in four. */
#define PRNG_SCENE_SKIP_MASK 0x0A

/* Plays scene `scene`, and each scene the last byte of its sector names
 * after it, with the title tune after each, and then redraws the
 * location. A failed read prints "breaks free" and ends the scenes, and
 * then sets the text window twice, as the original does. */
void scene_loop(uint8_t scene) {
    SCENE_NUMBER = scene;
    BLINK_COL = MAP_WIDTH;
    SAVED_ENTITY_PTR = ENTITY_PTR;
    SAVED_MOB_PTR = MOB_PTR;

    if ((step_prng() & PRNG_SCENE_SKIP_MASK) == 0)
        load_scene_data();

    for (;;) {
        if (load_scene_from_disk(SCENE_NUMBER)) {
            set_text_window_bottom();
            display_message(MSG_BREAKS_FREE);
            break;
        }
        display_status_bar();
        TEXT_STREAM_PTR = SCENE_TEXT_DATA;
        text_renderer();
        tune_player(TUNE_ATTRACT, 0);
        if (SCENE_TRANSITION == 0)
            break;
        SCENE_NUMBER = SCENE_TRANSITION;
    }

    set_text_window_bottom();
    LOCATION_POS = 0;
    LOCATION_STYLE = 0;
    enter_location();
    ENTITY_PTR = SAVED_ENTITY_PTR;
    MOB_PTR = SAVED_MOB_PTR;
}
