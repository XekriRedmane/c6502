/*
    init.c -- the start of the game: setting up the machine, the title
    screen with its border animation and title tune, the choice of the
    keyboard or the joystick, the save/load menu, and the count of the
    player characters.

    game_init sets the reset vector, shows the high-resolution page 1,
    sets up the HRCG and the disk IOB, and draws the title screen. The
    title tune then plays until a key is pressed, and after each of its
    notes the tune player calls border_anim_step, which moves icons
    around the border of the picture. Then the player picks the
    keyboard or the joystick, the save/load menu runs as a chain from
    CHAIN_INIT_WORLD, and the player characters outside location 0 are
    counted into TOTAL_MOB_COUNT.

    Departures from the original:

    - GAME_INIT's RELOCATE_HIRES_STAGING copies $2000-$32FF to $9600
      and $3300-$3FFF to $B300 and clears each byte it copies. The C
      program is loaded with its HRCG at $92A5 and the disk reader
      already at $B300, so there's nothing to copy, and running the
      copy would erase them. game_init only clears hi-res page 1
      (`clear_hires_page1`), which still holds what the loader staged
      there, as the original's copy leaves it.
    - INIT_STUB and INIT_STUB2 are bare RTSes, and their calls are
      dropped.
    - INIT_HRCG patches the HRCG's code ($94B6 to RTS and $92D5-$92D7
      to NOPs) and jumps to HRCG_INIT. hrcg.c translates the HRCG as
      patched, so game_init calls hrcg_init directly.
    - The original reaches GAME_ACTION_HANDLER and its three handlers
      through the tune player's JMP (DISPATCH_VEC) at $7A49, and each
      handler goes back to the tune with JMP TUNE_ADVANCE after storing
      the next handler's address in DISPATCH_VEC. Here tune_player
      calls border_anim_step after each note while BORDER_ANIM_ON is
      true, border_anim_step returns where the original jumps to
      TUNE_ADVANCE, and the next handler is a number in border_next.
      INIT_GAME_STATE ends the animation by writing JMP TUNE_ADVANCE
      over the JMP (DISPATCH_VEC), and init_game_state clears
      BORDER_ANIM_ON in its place.
    - The border's 3-byte records are three arrays, border_glyph,
      border_row, and border_col, indexed by cell, and the handlers name
      runs of cells where the original passes table pointers and byte
      offsets. The label positions are two arrays of columns and rows.
      The glyphs that DRAW_BORDER_CHAR and DRAW_BORDER_ICONS draw with
      immediate operands are a table that plot_title_glyphs walks, and
      DRAW_BORDER_CHAR and PLOT_CHAR are its body.
    - The bytes the border code uses as variables are statics and
      locals: BORDER_ANIM_INDEX is border_anim_index, BORDER_ANIM_SAVE
      is border_pass, BORDER_LOOP_CTR is border_handler (half its
      value) in the animation and the loop index in draw_border_strings,
      BORDER_SWAP_FLAG is move_border_icons's `carried`,
      BORDER_INITIAL_CHAR is anim_border_icons's `from`, and
      BORDER_ICON_OFFSET and ICON_TABLE_PTR are the cell index.
      BORDER_HANDLER_TABLE is the switch in border_anim_step.
    - RENDER_FONT_CHAR_DIRECT takes a font selector in X, which it uses
      only for glyphs of 24 and up. The border holds only glyphs below
      24, so draw_border_cell passes 0 where the original passes the
      high byte of a table pointer, a table index, or whatever X held
      after the tune player's note.
    - GAME_INIT stores $A56F in SOFTEV as two bytes, and so does
      game_init. cold_restart is at that address, and taking its
      address would make it ineligible for the ZP-passing ABI.
    - The code from $5C00 on is overwritten by the first scene the game
      loads into SCENE_TEXT_DATA, but it runs only before that, so the
      C, which keeps it elsewhere, behaves the same.

    Ali Baba and the Forty Thieves, main.asm lines 1288-1328,
    2125-2157, 9703-10243, 12485-12487, 12630-12645, 12864-12902, and
    14948-15010:

        ORG     $084E
    SELECT_INPUT_MODE:
        SUBROUTINE SELECT_INPUT_MODE
        ; Prompt the player to choose keyboard or joystick input.
        ;
        ; Behavior:
        ;   1. Displays the input mode prompt message.
        ;   2. Polls keyboard and paddle button 1 in a tight loop.
        ;   3. Space bar selects keyboard (INPUT_MODE=0); paddle button
        ;      state change selects joystick (INPUT_MODE=1).
        ;
        ; Outputs:
        ;   INPUT_MODE — 0 for keyboard, 1 for joystick
        ; Modifies: INPUT_MODE
        ; Clobbers: A, X, Y

        LDA     #MSG_INPUT_PROMPT           ; message $38: input mode prompt
        JSR     DISPLAY_MESSAGE
        LDA     $C061          ; read paddle button 1
        AND     #$80           ; isolate bit 7 (pressed state)
        STA     $BA            ; save initial button state
    .clear:
        STA     $C010          ; clear keyboard strobe
    .poll:
        LDX     $C000          ; read keyboard
        BMI     .key           ; key pressed -> check it
        LDA     $C061          ; re-read paddle button
        EOR     $BA            ; compare with initial state
        BMI     .joystick      ; button state changed -> joystick
        BPL     .poll          ; keep polling
    .key:
        BXEQ    #$A0,_SELECT_INPUT_MODE.keyboard ; yes -> keyboard mode
        BNE     .clear         ; other key -> ignore, clear strobe
    _SELECT_INPUT_MODE.keyboard:
        STOB    #$00,INPUT_MODE ; 0 = keyboard
        RTS
    .joystick:
        STOB    #$01,INPUT_MODE ; 1 = joystick
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_JOYSTICK_MODE           ; message $2F: joystick confirmation
        JMP     DISPLAY_MESSAGE
    ...

        ORG     $0C51
    INIT_MOB_COUNT:
        SUBROUTINE INIT_MOB_COUNT
        ; Sum mob counts from all character group records into TOTAL_MOB_COUNT.
        ;
        ; Behavior:
        ;   Starts at character record 1, walks 9-byte records summing
        ;   byte 8 (group member count) until end sentinel (byte 1 = $FF).
        ;
        ; Outputs:
        ;   TOTAL_MOB_COUNT — total mobs across groups 1+
        ; Modifies: TOTAL_MOB_COUNT, CHAR_PTR
        ; Clobbers: A, Y

        STOB    #$00,TOTAL_MOB_COUNT ; clear total
        STOW    $4009,CHAR_PTR ; $FA/$FB = $4009 (record 1)
    .loop:
        LDY     #$01
        LDA     (CHAR_PTR),Y    ; byte 1 of record
        BANE    #$FF,_INIT_MOB_COUNT.count     ; $FF = end sentinel
        RTS
    _INIT_MOB_COUNT.count:
        LDY     #$08
        LDA     (CHAR_PTR),Y    ; byte 8 = group member count
        CLC
        ADC     TOTAL_MOB_COUNT ; accumulate
        STA     TOTAL_MOB_COUNT
        LDA     #$09            ; \ advance $FA by 9 (next record)
        ADC     CHAR_PTR        ;  |
        STA     CHAR_PTR        ;  |
        BCC     .loop           ;  |
        INC     CHAR_PTR+1      ;  |
        BCS     .loop           ; / always taken
    ...

        ORG     $5B00
    INIT_GAME_STATE:
        SUBROUTINE INIT_GAME_STATE
        ; Set up game display, dispatch vector, and play opening tune.
        ;
        ; Behavior:
        ;   1. Calls INIT_GAME_DISPLAY to draw the main screen layout.
        ;   2. Sets DISPATCH_VEC to GAME_ACTION_HANDLER.
        ;   3. Patches character renderer at [[CHAR_CHARSET]] and jump at $7A49-$7A4B.
        ;   4. Plays tune 6 (attract/opening music).
        ;
        ; Modifies: DISPATCH_VEC, INPUT_MODE, $80A6, $7A49-$7A4B
        ; Clobbers: A, X, Y

        JSR     INIT_GAME_DISPLAY     ; set up game display / UI
        STOW    GAME_ACTION_HANDLER,DISPATCH_VEC ; $F6/DISPATCH_VEC+1 = game action handler
        STOB    #$00,$5A00 ; clear game state flag
        LDX     #$B1                  ; LDA (zp),Y opcode
        STX     $80A6                 ; patch character renderer
        LDX     #$06                  ; tune 3: attract/opening
        JSR     TUNE_PLAYER
        STOW    $4F4C,$7A49 ; patch $7A49 to JMP TUNE_ADVANCE
    INPUT_SCAN_OFFSET:                     ; $5B24: also scan position index (runtime storage)
        LDA     #$7A                   ; (also UNDER_LEVEL at [[UNDER_LEVEL]])
    SAFE_TO_REST:                          ; $5B26: also 1 if no adjacent threats (runtime storage)
        STA     $7A4B                  ; (also TURN_ACTIVE at [[TURN_ACTIVE]])
        RTS

    UNDER_LEVEL     EQU INPUT_SCAN_OFFSET+1 ; $5B25: 1 if char level < steps taken (runtime storage)
    TURN_ACTIVE     EQU SAFE_TO_REST+1      ; $5B27: 1 while turn is in progress (runtime storage)
        ORG     $5B2A
    GAME_ACTION_HANDLER:
        SUBROUTINE GAME_ACTION_HANDLER
        ; Border animation handler invoked via DISPATCH_VEC during idle loop.
        ;
        ; NOTE: Code from $5C00 onward (TICK_BORDER_ANIM tail through
        ; DRAW_BORDER_LABELS) is overwritten when LOAD_SCENE_FROM_DISK
        ; loads scene text into SCENE_TEXT_DATA ($5C00).  This permanently
        ; disables the idle border animation after the first scene load.
        ; See the LOAD_SCENE_FROM_DISK documentation for the full analysis.
        ;
        ; Behavior:
        ;   1. Runs a 3-pass loop: each pass dispatches three border handlers
        ;      (A, B, C) that swap/draw 2x2 icons along the border.
        ;   2. Between passes, randomly places icons via ANIM_BORDER_ICONS_A/B.
        ;   3. Each pass advances the animation index via TICK_BORDER_ANIM
        ;      and plays tune notes via TUNE_ADVANCE between handler calls.
        ;
        ; Modifies: BORDER_ANIM_SAVE, BORDER_ANIM_INDEX, FONT_CHARNUM,
        ;   DISPATCH_VEC, border position tables
        ; Clobbers: A, X, Y

        ; --- Main entry: 3-pass border animation loop ---
    .restart:
        LDA     #$03
        STA     BORDER_ANIM_SAVE  ; 3 passes
        JSR     TICK_BORDER_ANIM  ; advance one position
    .new_pass:
        LDX     #$04              ; start at handler pair 2
        STX     BORDER_LOOP_CTR   ; handler index
    .handler_loop:
        LDX     BORDER_LOOP_CTR
        LDA     BORDER_HANDLER_TABLE,X ; \ load handler pointer
        STA     DISPATCH_VEC      ;  | into DISPATCH_VEC
        INX                       ;  |
        LDA     BORDER_HANDLER_TABLE,X ;  |
        STA     DISPATCH_VEC+1    ; /
        DEC     BORDER_LOOP_CTR   ; \ next handler pair
        DEC     BORDER_LOOP_CTR   ; /
        JMP     TUNE_ADVANCE      ; play next note, returns via DISPATCH_VEC
    .check_pass:
        LDA     BORDER_LOOP_CTR
        BPL     .handler_loop     ; more handlers -> continue
        DEC     BORDER_ANIM_SAVE  ; next pass
        BMI     .restart          ; all passes done -> restart from top
        LDA     BORDER_ANIM_SAVE
        CMP     #$01
        BEQ     .pass_0
        BCS     .new_pass         ; pass 2 -> normal handlers
        JSR     ANIM_BORDER_ICONS_A ; pass 1 -> random icon placement
        JMP     .new_pass
    .pass_0:
        JSR     ANIM_BORDER_ICONS_B ; pass 0 -> alternate icons
        JMP     .new_pass

        ; --- Handler A ($5B6D): draw from $5DE9/$5E04 tables ---
    BORDER_HANDLER_A:
        LDA     #$E9
        LDX     #$5D
        JSR     DRAW_BORDER_ICON_LIST
        LDA     #$04
        LDX     #$5E
        JSR     DRAW_BORDER_ICON_LIST
        JMP     .check_pass

        ; --- Handler C ($5B7E): draw from $5E1F/$5E3A tables ---
    BORDER_HANDLER_C:
        LDA     #$1F
        LDX     #$5E
        JSR     DRAW_BORDER_ICON_LIST
        LDA     #$3A
        LDX     #$5E
        JSR     DRAW_BORDER_ICON_LIST
        JMP     .check_pass

        ; --- Handler B ($5B8F): draw from $5DCB table (Y=$1B start) ---
    BORDER_HANDLER_B:
        LDA     #$CB
        LDX     #$5D
        LDY     #$1B
        JSR     DRAW_BORDER_ICON_ENTRY
        JMP     .check_pass

        ; --- Draw a list of border icons from (A/X) pointer ---
    DRAW_BORDER_ICON_LIST:
        LDY     #$18              ; default: start at offset $18
    DRAW_BORDER_ICON_ENTRY:
        STA     ICON_TABLE_PTR    ; \ set pointer
        STX     ICON_TABLE_PTR+1  ; /
        STY     BORDER_ICON_OFFSET ; save offset
        LDA     #$00
        STA     BORDER_SWAP_FLAG  ; clear swap flag
    .icon_loop:
        LDY     BORDER_ICON_OFFSET ; current offset
        LDA     BORDER_SWAP_FLAG  ; swap flag
        BNE     .swap_restore     ; nonzero -> restore saved char
        LDA     (ICON_TABLE_PTR),Y ; read icon from table
        STA     BORDER_SWAP_FLAG  ; save it
        BEQ     .skip             ; zero -> skip (empty)
        LDA     #$00
        STA     (ICON_TABLE_PTR),Y ; clear table entry
        JSR     DRAW_BORDER_CHAR_AT ; draw the icon
        JMP     .skip
    .swap_restore:
        STA     (ICON_TABLE_PTR),Y ; restore saved icon
        JSR     DRAW_BORDER_CHAR_AT ; draw it
        LDA     #$00
        STA     BORDER_SWAP_FLAG  ; clear swap flag
    .skip:
        DEC     BORDER_ICON_OFFSET ; \ next entry (3 bytes per record)
        DEC     BORDER_ICON_OFFSET ;  |
        DEC     BORDER_ICON_OFFSET ; /
        BPL     .icon_loop
        RTS

        ; --- Draw a single border character at position from table entry ---
    DRAW_BORDER_CHAR_AT:
        STA     FONT_CHARNUM      ; set character to draw
        INY
        LDA     (ICON_TABLE_PTR),Y ; column
        STA     TEXT_ROW          ; -> cursor column
        INY
        LDA     (ICON_TABLE_PTR),Y ; row
        STA     TEXT_COL          ; -> cursor row
        JMP     RENDER_FONT_CHAR_DIRECT ; render font char

        ; --- Tick border animation: advance index, draw next position ---
    TICK_BORDER_ANIM:
        LDA     BORDER_ANIM_INDEX ; current position index
        STA     BORDER_POS_TABLE+27 ; save to restore later ($5DE6)
        LDY     BORDER_POS_TABLE+28 ; \ get col/row from next position
        STY     TEXT_ROW          ;  |
        LDY     BORDER_POS_TABLE+29 ;  |
        STY     TEXT_COL          ; /
        STA     FONT_CHARNUM      ; draw border char
        JSR     RENDER_FONT_CHAR_DIRECT
        INC     BORDER_ANIM_INDEX ; advance index
        LDA     BORDER_ANIM_INDEX
        CMP     #$16              ; past end?
        BCC     .tick_done
        LDA     #$05              ; wrap to start
        STA     BORDER_ANIM_INDEX
    .tick_done:
        RTS

        ; --- Random icon placement A (from $5E1F/$5E3A tables) ---
    ANIM_BORDER_ICONS_A:
        LDA     #$02              ; \ random(2): pick table A or B
        PHA                       ; /
        LDX     #$06
        LDA     #$1F
        LDY     #$5E
        JMP     .anim_shared

        ; --- Random icon placement B (from $5DE9/$5DCB tables) ---
    ANIM_BORDER_ICONS_B:
        LDA     #$04              ; \ random(4): pick position
        PHA                       ; /
        LDX     #$15
        LDA     #$E9
        LDY     #$5D
    .anim_shared:
        STX     BORDER_INITIAL_CHAR ; save max index
        STA     ICON_TABLE_PTR    ; \ set pointer
        STY     ICON_TABLE_PTR+1  ; /
        PLA
        JSR     RANDOM_IN_RANGE
        CMP     #$01
        BEQ     .random_ok
        BCC     .random_small
        RTS                       ; > 1 -> skip (rare)
    .random_ok:
        LDY     #$18
        BNE     .do_place
    .random_small:
        LDY     #$33
    .do_place:
        LDX     BORDER_INITIAL_CHAR
        LDA     BORDER_POS_TABLE,X ; read icon from border table
        STA     FONT_CHARNUM
        STA     (ICON_TABLE_PTR),Y ; store in target table
        LDA     #$00
        STA     BORDER_POS_TABLE,X ; clear source entry
        INY
        LDA     (ICON_TABLE_PTR),Y ; column
        STA     TEXT_ROW
        INY
        LDA     (ICON_TABLE_PTR),Y ; row
        STA     TEXT_COL
        JSR     RENDER_FONT_CHAR_DIRECT ; render
        LDA     #$00
        STA     FONT_CHARNUM
        LDX     BORDER_INITIAL_CHAR
        LDA     BORDER_POS_TABLE+1,X ; \ restore cursor from
        STA     TEXT_ROW          ;  | adjacent table entry
        LDA     BORDER_POS_TABLE+2,X ;  |
        STA     TEXT_COL          ; /
        JMP     RENDER_FONT_CHAR_DIRECT ; render and return
        ORG     $5C6D
    INIT_GAME_DISPLAY:
        SUBROUTINE INIT_GAME_DISPLAY
        ; Draw the game's main screen layout with borders and decorations.
        ;
        ; Behavior:
        ;   1. Clears the hi-res screen, renders UI background from UI_TEXT_STREAM.
        ;   2. Draws 2x2 border characters at fixed positions and border labels/icons.
        ;   3. Prints copyright string (row 21) and press-space prompt (row 23).
        ;   4. Switches to charset 5 and draws decorative strings on the right panel.
        ;
        ; Modifies: TEXT_ROW, TEXT_COL, TEXT_STREAM_PTR, FONT_CHARNUM, FONT_CHARSET
        ; Clobbers: A, X, Y

        LDA     #$90                           ; HRCG control: clear hi-res screen
        JSR     ROM_COUT1
        stow    UI_TEXT_STREAM,TEXT_STREAM_PTR ; point text stream at UI data
        JSR     TEXT_RENDERER                  ; render game UI background

        ; Draw three 2x2 border characters across top (charset 2, row 3)
        LDA     #$01                           ; char 1
        LDX     #$10                           ; col 16
        JSR     DRAW_BORDER_CHAR
        LDA     #$0E                           ; char 14
        LDX     #$12                           ; col 18
        JSR     DRAW_BORDER_CHAR
        LDA     #$04                           ; char 4
        LDX     #$14                           ; col 20
        JSR     DRAW_BORDER_CHAR

        JSR     DRAW_BORDER_LABELS             ; draw border text labels
        JSR     DRAW_BORDER_ICONS              ; draw corner decorations

        JSR     PRINT_CTRL_AB                  ; reset HRCG block mode

        ; Print copyright at row 21, col 0
        STOB    #$15,TEXT_ROW
        STOB    #$00,TEXT_COL
        LDX     #<S_COPYRIGHT
        LDY     #>S_COPYRIGHT
        JSR     PRINT_STRING_AT_ADDR

        ; Print "PRESS SPACE BAR TO CONTINUE" at row 23, col 0
        STOB    #$17,TEXT_ROW
        STOB    #$00,TEXT_COL
        LDX     #<S_PRESS_SPACE_BOOT
        LDY     #>S_PRESS_SPACE_BOOT
        JSR     PRINT_STRING_AT_ADDR

        ; Switch to HRCG charset 5 for decorative chars
        LDA     #$81                           ; ctrl-A (select charset)
        JSR     ROM_COUT1
        LDA     #$B5                           ; '5' = charset 5
        JSR     ROM_COUT1

        ; Print decorative strings at col 30, rows 21-23
        STOB    #$15,TEXT_ROW
        STOB    #$1E,TEXT_COL
        LDX     #<S_DECOR_ROW1
        LDY     #>S_DECOR_ROW1
        JSR     PRINT_STRING_AT_ADDR

        INC     TEXT_ROW
        STOB    #$1E,TEXT_COL
        LDX     #<S_DECOR_ROW2
        LDY     #>S_DECOR_ROW2
        JSR     PRINT_STRING_AT_ADDR

        INC     TEXT_ROW
        STOB    #$1E,TEXT_COL
        LDX     #<S_DECOR_ROW3
        LDY     #>S_DECOR_ROW3
        JMP     PRINT_STRING_AT_ADDR
        ORG     $5CEC
    DRAW_BORDER_CHAR:

        STX     TEXT_COL
        STA     FONT_CHARNUM
        LDY     #$03
        STY     TEXT_ROW
        STOB    #$02,FONT_CHARSET
        JMP     PRINT_FONTCHAR_AT_TEXT_POS
        ORG     $5CFD
    DRAW_BORDER_LABELS:
        SUBROUTINE DRAW_BORDER_LABELS
        ; Draw border label strings at predefined positions in three loops.
        ;
        ; Behavior:
        ;   1. Selects HRCG charset 2.
        ;   2. Loop 1: draws horizontal border strings at 6 positions.
        ;   3. Loop 2: draws left vertical border strings at 10 positions.
        ;   4. Loop 3: draws right vertical border strings at 10 positions.
        ;
        ; Modifies: TEXT_COL, TEXT_ROW, PRINT_STRING_ADDR, BORDER_LOOP_CTR
        ; Clobbers: A, X, Y

        ; Select HRCG charset 2
        LDA     #$81 ; ctrl-A
        JSR     ROM_COUT1
        LDA     #$B2 ; '2' = charset 2
        JSR     ROM_COUT1

        ; Loop 1: horizontal borders (6 positions from BORDER_POS_HORIZ)
        LDX     #$0A ; index 10 (5 pairs, counting down by 2)
        STX     BORDER_LOOP_CTR
        stow    BORDER_STR_HORIZ,PRINT_STRING_ADDR
    .loop1:
        LDA     BORDER_POS_HORIZ,X
        STA     TEXT_COL
        INX
        LDA     BORDER_POS_HORIZ,X
        STA     TEXT_ROW
        JSR     PRINT_STRING
        DEC     BORDER_LOOP_CTR
        DEC     BORDER_LOOP_CTR
        LDX     BORDER_LOOP_CTR
        BPL     .loop1

        ; Loop 2: vertical borders left (10 positions from BORDER_POS_VERT_L)
        LDX     #$12 ; index 18 (9 pairs, counting down by 2)
        STX     BORDER_LOOP_CTR
        stow    BORDER_STR_VERT_L,PRINT_STRING_ADDR
    .loop2:
        LDA     BORDER_POS_VERT_L,X
        STA     TEXT_COL
        INX
        LDA     BORDER_POS_VERT_L,X
        STA     TEXT_ROW
        JSR     PRINT_STRING
        DEC     BORDER_LOOP_CTR
        DEC     BORDER_LOOP_CTR
        LDX     BORDER_LOOP_CTR
        BPL     .loop2

        ; Loop 3: vertical borders right (10 positions from BORDER_POS_VERT_R)
        LDX     #$12
        STX     BORDER_LOOP_CTR
        stow    BORDER_STR_VERT_R,PRINT_STRING_ADDR
    .loop3:
        LDA     BORDER_POS_VERT_R,X
        STA     TEXT_COL
        INX
        LDA     BORDER_POS_VERT_R,X
        STA     TEXT_ROW
        JSR     PRINT_STRING
        DEC     BORDER_LOOP_CTR
        DEC     BORDER_LOOP_CTR
        LDX     BORDER_LOOP_CTR
        BPL     .loop3
        RTS
        ORG     $5D7A
    DRAW_BORDER_ICONS:
        ; Draw six decorative 2x2 font icons at fixed border positions.
        ;
        ; Behavior:
        ;   Calls PLOT_CHAR six times with hardcoded (char, col, row)
        ;   triples: four corner ornaments (chars 1-4) and two midpoint
        ;   icons (char $1C).
        ;
        ; Clobbers: A, X, Y

        LDA     #$02               ; \ char 2: left col 4, row 11
        LDX     #$04               ;  |
        LDY     #$0B               ;  |
        JSR     PLOT_CHAR          ; /

        LDA     #$01               ; \ char 1: left col 4, row 16
        LDX     #$04               ;  |
        LDY     #$10               ;  |
        JSR     PLOT_CHAR          ; /

        LDA     #$03               ; \ char 3: right col 34, row 16
        LDX     #$22               ;  |
        LDY     #$10               ;  |
        JSR     PLOT_CHAR          ; /

        LDA     #$04               ; \ char 4: right col 34, row 11
        LDX     #$22               ;  |
        LDY     #$0B               ;  |
        JSR     PLOT_CHAR          ; /

        LDA     #$1C               ; \ char 28: mid col 16, row 16
        LDX     #$10               ;  |
        LDY     #$10               ;  |
        JSR     PLOT_CHAR          ; /

        LDA     #$1C               ; \ char 28: mid col 22, row 11
        LDX     #$16               ;  |
        LDY     #$0B               ;  |
        JMP     PLOT_CHAR          ; / (tail-call)
        ORG     $5DB0
    PLOT_CHAR:
        SUBROUTINE PLOT_CHAR
        ; Inputs:  A = character number, X = text column, Y = text row
        ; Outputs: glyph rendered to hi-res page
        ; Clobbers: A, X, Y

        STA     FONT_CHARNUM            ; save glyph index
        STX     TEXT_COL                ; save target column
        STY     TEXT_ROW                ; save target row
        STOB    #$01,FONT_CHARSET       ; force custom font charset
        JMP     PRINT_FONTCHAR_AT_TEXT_POS ; render and return
        ORG     $5DBF
    BORDER_INITIAL_CHAR:
        DC.B    $A0               ; initial border character (space)
    BORDER_ANIM_INDEX:
        DC.B    $06               ; current animation index (cycles $05-$15)
    BORDER_ANIM_SAVE:
        DC.B    $A0               ; saved character at animated position
    BORDER_ICON_CHARS:
        DC.B    $C4,$D3,$E9       ; icon character codes (D, S, i)
    BORDER_HANDLER_TABLE:
        DC.W    BORDER_HANDLER_A  ; handler pointer 0
        DC.W    BORDER_HANDLER_B  ; handler pointer 1
        DC.W    BORDER_HANDLER_C  ; handler pointer 2

        ; --- Border position table: 46 entries of (icon, col, row) ---
        ; Bottom border (row $13, columns $09-$12)
    BORDER_POS_TABLE:
        DC.B    $00,$09,$13       ; \
        DC.B    $00,$0A,$13       ;  |
        DC.B    $00,$0B,$13       ;  |
        DC.B    $00,$0C,$13       ;  | bottom border
        DC.B    $00,$0D,$13       ;  |
        DC.B    $0B,$0E,$13       ;  | ($0B = special icon)
        DC.B    $00,$0F,$13       ;  |
        DC.B    $00,$10,$13       ;  |
        DC.B    $00,$11,$13       ;  |
        DC.B    $00,$12,$13       ; /
        ; Right border (column $10, rows $06-$0E top, $18-$20 bottom)
        DC.B    $00,$10,$06       ; \
        DC.B    $00,$10,$07       ;  |
        DC.B    $00,$10,$08       ;  |
        DC.B    $00,$10,$09       ;  |
        DC.B    $00,$10,$0A       ;  | right border (top)
        DC.B    $00,$10,$0B       ;  |
        DC.B    $0F,$10,$0C       ;  | ($0F = special icon)
        DC.B    $00,$10,$0D       ;  |
        DC.B    $00,$10,$0E       ; /
        DC.B    $00,$10,$20       ; \
        DC.B    $00,$10,$1F       ;  |
        DC.B    $00,$10,$1E       ;  |
        DC.B    $00,$10,$1D       ;  |
        DC.B    $00,$10,$1C       ;  | right border (bottom)
        DC.B    $05,$10,$1B       ;  | ($05 = special icon)
        DC.B    $00,$10,$1A       ;  |
        DC.B    $00,$10,$19       ;  |
        DC.B    $00,$10,$18       ; /
        ; Left border (column $0B, rows $06-$0E top, $18-$20 bottom)
        DC.B    $00,$0B,$06       ; \
        DC.B    $00,$0B,$07       ;  |
        DC.B    $00,$0B,$08       ;  |
        DC.B    $00,$0B,$09       ;  | left border (top)
        DC.B    $14,$0B,$0A       ;  | ($14 = special icon)
        DC.B    $00,$0B,$0B       ;  |
        DC.B    $00,$0B,$0C       ;  |
        DC.B    $00,$0B,$0D       ;  |
        DC.B    $00,$0B,$0E       ; /
        DC.B    $00,$0B,$20       ; \
        DC.B    $00,$0B,$1F       ;  |
        DC.B    $00,$0B,$1E       ;  |
        DC.B    $00,$0B,$1D       ;  |
        DC.B    $0E,$0B,$1C       ;  | left border (bottom)
        DC.B    $00,$0B,$1B       ;  | ($0E = special icon)
        DC.B    $00,$0B,$1A       ;  |
        DC.B    $00,$0B,$19       ;  |
        DC.B    $00,$0B,$18       ; /
        ORG     $5E55
    UI_TEXT_STREAM:
        DC.B    $7D               ; HRCG reset to full screen
        DC.B    $80,$02           ; mode control
        ; Map viewport bounds (2 sets)
        DC.B    $2F,$00,$00,$01,$0C,$09,$00,$00,$02,$01,$02,$01,$00,$00
        DC.B    $2F,$83,$01,$14,$08,$05,$00,$06,$0F,$12,$14,$19,$00,$14
        DC.B    $08,$09,$05,$16,$05,$13
        DC.B    $7F
        ORG     $5E7B
    BORDER_POS_HORIZ:                     ; 6 col/row pairs for horizontal labels
        DC.B    $01,$09,$01,$0E,$01,$13
        DC.B    $16,$09,$16,$0E,$16,$13
    BORDER_POS_VERT_L:                    ; 10 col/row pairs for vertical labels (left)
        DC.B    $01,$0A,$01,$0B,$01,$0C,$01,$0D
        DC.B    $01,$0F,$01,$12
        DC.B    $16,$0A,$16,$0D,$16,$0F,$16,$12
    BORDER_POS_VERT_R:                    ; 10 col/row pairs for vertical labels (right)
        DC.B    $10,$0A,$10,$0D,$10,$0F,$10,$12
        DC.B    $25,$0A,$25,$0D,$25,$0F
        DC.B    $25,$10,$25,$11,$25,$12
        ORG     $5EAF
    BORDER_STR_HORIZ:
        DC.B    $11               ; length = 17
        HEX     F0 F1 F0 F1 F0 F1 F0 F1
        HEX     F0 F1 F0 F1 F0 F1 F0 F1
        DC.B    $F0               ; last byte of horiz pattern
    BORDER_STR_VERT_L:
        DC.B    $02,$F0,$F1       ; vertical border pattern (left): len=2
    BORDER_STR_VERT_R:
        DC.B    $02,$F1,$F0       ; vertical border pattern (right): len=2
        ORG     $5EC7
    S_COPYRIGHT:
        HEX 1B C3 CF D0 D9 D2 C9 C7 C8 D4 A0 B1 B9 B8 B2 A0 D3 D4 D5 C1 D2 D4 A0 D3 CD C9 D4 C8
        ORG     $5EE3
    S_PRESS_SPACE_BOOT:
        HEX 1B D0 D2 C5 D3 D3 A0 D3 D0 C1 C3 C5 A0 C2 C1 D2 A0 D4 CF A0 C3 CF CE D4 C9 CE D5 C5
    S_DECOR_ROW1:
        DC.B    $07
        HEX     E8 E9 EA EB EC ED EE
    S_DECOR_ROW2:
        DC.B    $08
        HEX     EF F0 F1 F2 F3 F4 F5 F6
    S_DECOR_ROW3:
        DC.B    $08
        HEX     F7 F8 F9 FA FB FC FD FE
    ...

        ORG     $6B94
    INIT_STUB2:
        RTS
    ...

        ORG     $6C2E
    INIT_DISK_IOB:
        SUBROUTINE INIT_DISK_IOB
        ; Set the zero-page RWTS IOB pointer to the IOB address.
        ;
        ; Behavior:
        ;   Stores the address of RWTS_IOB ($B7E8) into RWTS_IOB_PTR ($08/$09).
        ;
        ; Modifies: RWTS_IOB_PTR
        ; Clobbers: A, Y

        LDA     #>RWTS_IOB     ; \ set RWTS IOB pointer
        LDY     #<RWTS_IOB     ;  | $08/$09 = $B7E8
        STA     RWTS_IOB_PTR+1 ;  |
        STY     RWTS_IOB_PTR   ; /
        RTS
    ...

        ORG     $6DA5
    RELOCATE_HIRES_STAGING:
        SUBROUTINE RELOCATE_HIRES_STAGING
        ; Block-copy staged data from hi-res page to runtime addresses.
        ;
        ; Behavior:
        ;   1. Copies $2000-$32FF to $9600-$A8FF (19 pages: HRCG + font).
        ;   2. Copies $3300-$3FFF to $B300-$BFFF (13 pages: disk I/O).
        ;   3. Zeroes each source byte after copying, clearing the hi-res page.
        ;
        ; Modifies: COPY_SRC, COPY_DEST, $2000-$3FFF (zeroed), $9600-$A8FF, $B300-$BFFF
        ; Clobbers: A, X, Y

        STOB    #$00,COPY_DEST ; dest = $9600
        STA     COPY_SRC      ; \  src  = $2000
        STOB    #$20,COPY_SRC+1 ; src hi
        STOB    #$96,COPY_DEST+1 ; dest hi
        LDY     #$00
        LDX     #$13          ; 19 pages: $2000-$32FF -> $9600-$A8FF
        JSR     .copy
        LDX     #$0D          ; 13 pages: $3300-$3FFF -> $B300-$BFFF
        STOB    #$B3,COPY_DEST+1 ; dest hi = $B3
        INC     COPY_SRC+1    ; src hi = $33 (was $32 after first pass)

    .copy:
        LDA     (COPY_SRC),Y  ; load from source
        STA     (COPY_DEST),Y ; store to dest
        LDA     #$00
        STA     (COPY_SRC),Y  ; clear source byte
        DEY
        BNE     .copy         ; inner loop (256 bytes)
        DEX
        BNE     .next_page    ; more pages?
        RTS

    .next_page:
        INC     COPY_SRC+1    ; next source page
        INC     COPY_DEST+1   ; next dest page
        JMP     .copy
    ...

        ORG     $78C7
    GAME_INIT:
        SUBROUTINE GAME_INIT
        ; Initialize the game from cold start.
        ;
        ; Behavior:
        ;   1. Sets reset vector to $A56F and disables blink cursor.
        ;   2. Relocates hi-res staging data to runtime addresses ($9600+, $B300+).
        ;   3. Switches to hi-res graphics mode and initializes HRCG, disk IOB,
        ;      text window, game display, map, and input mode.
        ;   4. Clears location/entity state, inits world and mob count.
        ;
        ; Modifies: reset vector ($03F2), BLINK_COL, LOCATION_POS, LOCATION_STYLE,
        ;   ENTITY_PTR+1, CURRENT_PLAYER, DISPATCH_VEC, display softswitches
        ; Clobbers: A, X, Y

        LDX     #$6F                   ; \
        LDY     #$A5                   ;  | reset vector = $A56F
        STX     $03F2                  ;  |
        STY     $03F3                  ; /
        JSR     $FB6F                  ; ROM: set reset vector
        STOB    #$14,BLINK_COL ; disable blink cursor
        JSR     RELOCATE_HIRES_STAGING ; copy $2000+ to $9600+/$B300+
        LDA     $C050                  ; graphics mode
        LDA     $C052                  ; full screen (no text split)
        LDA     $C054                  ; page 1
        LDA     $C057                  ; hi-res
        JSR     INIT_DISK_IOB
        JSR     INIT_HRCG
        JSR     SET_TEXT_WINDOW_WRAP
        JSR     INIT_STUB
        JSR     INIT_GAME_STATE
        JSR     FILL_MAP
        JSR     SET_TEXT_WINDOW_BOTTOM
        JSR     INIT_STUB2
        STOB    #$00,LOCATION_POS ;  clear state variables
        STA     LOCATION_STYLE         ;  |
        STA     ENTITY_PTR+1           ; /
        JSR     SELECT_INPUT_MODE
        JSR     INIT_WORLD
        JSR     INIT_MOB_COUNT
        STOB    #$01,CURRENT_PLAYER
        RTS
        ORG     $7919
    INIT_STUB:
        RTS
        ORG     $791A
    INIT_HRCG:
        SUBROUTINE INIT_HRCG
        ; Patch and initialize the HRCG character generator library.
        ;
        ; Behavior:
        ;   1. Patches $94B6 to RTS and $92D5-$92D7 to NOPs.
        ;   2. Tail-calls HRCG_INIT to complete initialization.
        ;
        ; Modifies: $94B6, $92D5-$92D7
        ; Clobbers: A

        STOB    #$60,$94B6 ; RTS opcode
        STOB    #$EA,$92D5 ; NOP opcode
        STA     $92D6     ;  | (NOP out DOS reconnect)
        STA     $92D7     ; /
        JMP     HRCG_INIT ; tail call: enter HRCG initialization
*/
#include "ali_baba.h"

/* ---- The title screen's border ------------------------------------------- */

/*
    The cells of the title screen's border that the animation moves its
    icons through, 46 of them. Each has a glyph of font '1' (0 for an
    empty cell), and a text row and column for the glyph's top left
    corner. The original's BORDER_POS_TABLE holds them as 3-byte records
    of glyph, row, and column. Only the glyphs change.

    The animation uses five runs of cells, each drawn as one list:

      0-9     handler B, ending at the cell start_border_passes draws
      10-18   handler A's first list
      19-27   handler A's second list
      28-36   handler C's first list
      37-45   handler C's second list

    The original's handlers point at the runs' byte offsets, 0, $1E,
    $39, $54, and $6F.
*/
#define BORDER_CELLS 46
#define BORDER_TICK_CELL 9

static uint8_t border_glyph[BORDER_CELLS] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00,
};

static const uint8_t border_row[BORDER_CELLS] = {
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
    0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
};

static const uint8_t border_col[BORDER_CELLS] = {
    0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
    0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x20, 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18,
    0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x20, 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18,
};

/* The glyph start_border_passes draws next, 5-$15. BORDER_ANIM_INDEX. */
static uint8_t border_anim_index = 0x06;

/* The handlers, numbered by their places in the original's
 * BORDER_HANDLER_TABLE, and the state that starts the animation over. */
enum {
    BORDER_HANDLER_A = 0,
    BORDER_HANDLER_B = 1,
    BORDER_HANDLER_C = 2,
    BORDER_RESTART = 3,
};

/* The step border_anim_step runs next, a BORDER_ value. The original
 * keeps the routine's address in DISPATCH_VEC ($F6/$F7). */
static uint8_t border_next;
/* The handler the pass chooses after border_next, counting down from
 * BORDER_HANDLER_C, and -1 once the pass has chosen all three. The
 * original's BORDER_LOOP_CTR ($5DC3) is twice this. */
static int8_t border_handler;
/* The passes left after this one, 3 down to 0. BORDER_ANIM_SAVE. */
static uint8_t border_pass;

bool BORDER_ANIM_ON = true;

/*
    Draws `glyph` at border cell `cell`. RENDER_FONT_CHAR_DIRECT reads
    its font selector only for a glyph of 24 or more, and every glyph
    the border holds is below 24: the cells start with $05-$14, and
    start_border_passes stores 5-$15. So the selector passed here is
    unused, and the glyph is drawn with the selector that
    init_game_state left in CHAR_CHARSET. The original passes the high
    byte of its table pointer, or a table index.
*/
static void draw_border_cell(uint8_t cell, uint8_t glyph)
{
    FONT_CHARNUM = glyph;
    TEXT_ROW = border_row[cell];
    TEXT_COL = border_col[cell];
    render_font_char_direct(0);
}

/*
    Moves each icon in cells `first` to `last` one cell toward `first`,
    working from `last` down. An icon is erased from its cell and drawn
    in the next one, and an icon in `first` is erased and lost.
    DRAW_BORDER_ICON_ENTRY, whose BORDER_SWAP_FLAG is `carried`.
*/
static void move_border_icons(uint8_t first, uint8_t last)
{
    uint8_t cell = last;
    uint8_t carried = 0;

    for (;;) {
        if (carried != 0) {
            border_glyph[cell] = carried;
            draw_border_cell(cell, carried);
            carried = 0;
        } else {
            carried = border_glyph[cell];
            if (carried != 0) {
                border_glyph[cell] = 0;
                draw_border_cell(cell, 0);
            }
        }
        if (cell == first)
            break;
        cell--;
    }
}

/* Moves the icons of the nine cells from `first` on.
 * DRAW_BORDER_ICON_LIST, which falls into DRAW_BORDER_ICON_ENTRY with
 * the offset of the ninth record. */
static void move_border_icon_list(uint8_t first)
{
    move_border_icons(first, first + 8);
}

/*
    Starts the passes over: three more after this one, and the next
    glyph of the cycle 5-$15 drawn in cell 9, the last cell that handler
    B moves. The .restart of GAME_ACTION_HANDLER, and TICK_BORDER_ANIM.
*/
static void start_border_passes(void)
{
    uint8_t glyph = border_anim_index;

    border_pass = 3;
    border_glyph[BORDER_TICK_CELL] = glyph;
    draw_border_cell(BORDER_TICK_CELL, glyph);
    if (++border_anim_index >= 0x16)
        border_anim_index = 5;
    border_handler = BORDER_HANDLER_C;
}

/*
    Picks random_in_range(limit). For 1 it moves the icon in cell `from`
    to cell `to`, for 0 to cell `to` + 9, and for anything larger it
    does nothing. ANIM_BORDER_ICONS_A (limit 2, from 2, to 36) and
    ANIM_BORDER_ICONS_B (limit 4, from 7, to 18). The original keeps
    `from` in BORDER_INITIAL_CHAR.
*/
static void anim_border_icons(uint8_t limit, uint8_t from, uint8_t to)
{
    uint8_t r = random_in_range(limit);
    uint8_t glyph;

    if (r > 1)
        return;
    if (r == 0)
        to += 9;
    glyph = border_glyph[from];
    border_glyph[to] = glyph;
    border_glyph[from] = 0;
    draw_border_cell(to, glyph);
    draw_border_cell(from, 0);
}

/*
    The original's GAME_ACTION_HANDLER runs the animation as a loop of
    passes, each of which runs handlers C, B, and A, one per note of the
    title tune. It gets to the next note by storing the next handler's
    address in DISPATCH_VEC and jumping to TUNE_ADVANCE, and the tune
    player jumps through DISPATCH_VEC after the note. So each call here
    runs the handler the last call chose, then what the original's
    handler does after it (.check_pass and, at the end of a pass, the
    pass's random move), and chooses the next handler.

    The first call, and the call that ends the fourth pass, start the
    passes over. A pass ends after handler A. The end of the second pass
    runs ANIM_BORDER_ICONS_B, and the end of the third runs
    ANIM_BORDER_ICONS_A.
*/
void border_anim_step(void)
{
    switch (border_next) {
    case BORDER_HANDLER_A:
        move_border_icon_list(10);
        move_border_icon_list(19);
        break;
    case BORDER_HANDLER_B:
        move_border_icons(0, 9);
        break;
    case BORDER_HANDLER_C:
        move_border_icon_list(28);
        move_border_icon_list(37);
        break;
    default:    /* BORDER_RESTART */
        start_border_passes();
        break;
    }
    if (border_handler < 0) {
        if (border_pass == 0) {
            start_border_passes();
        } else {
            border_pass--;
            if (border_pass == 1)
                anim_border_icons(4, 7, 18);
            else if (border_pass == 0)
                anim_border_icons(2, 2, 36);
            border_handler = BORDER_HANDLER_C;
        }
    }
    border_next = (uint8_t)border_handler;
    border_handler--;
}

/* ---- The title screen ----------------------------------------------------- */

/* The title screen's text stream for text_renderer: the HRCG reset to
 * the full screen, the mode, and the two map viewports' bounds. */
static const uint8_t UI_TEXT_STREAM[38] = {
    0x7D,
    0x80, 0x02,
    0x2F, 0x00, 0x00, 0x01, 0x0C, 0x09, 0x00, 0x00, 0x02, 0x01, 0x02, 0x01, 0x00, 0x00,
    0x2F, 0x83, 0x01, 0x14, 0x08, 0x05, 0x00, 0x06, 0x0F, 0x12, 0x14, 0x19, 0x00, 0x14,
    0x08, 0x09, 0x05, 0x16, 0x05, 0x13,
    0x7F,
};

/*
    Where draw_border_labels prints the strings of the border frame,
    as a text column and row for each. Positions 0-5 take the
    horizontal string, 6-15 the left vertical string, and 16-25 the
    right vertical string. The original's BORDER_POS_HORIZ,
    BORDER_POS_VERT_L, and BORDER_POS_VERT_R hold them as pairs of
    column and row.
*/
static const uint8_t BORDER_LABEL_COL[26] = {
    0x01, 0x01, 0x01, 0x16, 0x16, 0x16,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x16, 0x16, 0x16, 0x16,
    0x10, 0x10, 0x10, 0x10, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25,
};
static const uint8_t BORDER_LABEL_ROW[26] = {
    0x09, 0x0E, 0x13, 0x09, 0x0E, 0x13,
    0x0A, 0x0B, 0x0C, 0x0D, 0x0F, 0x12, 0x0A, 0x0D, 0x0F, 0x12,
    0x0A, 0x0D, 0x0F, 0x12, 0x0A, 0x0D, 0x0F, 0x10, 0x11, 0x12,
};

/* The border frame's strings, glyphs $F0 and $F1 of font '2'. Each
 * string is a length byte and then its characters. */
static const uint8_t BORDER_STR_HORIZ[18] = {
    0x11,
    0xF0, 0xF1, 0xF0, 0xF1, 0xF0, 0xF1, 0xF0, 0xF1,
    0xF0, 0xF1, 0xF0, 0xF1, 0xF0, 0xF1, 0xF0, 0xF1,
    0xF0,
};
static const uint8_t BORDER_STR_VERT_L[3] = { 0x02, 0xF0, 0xF1 };
static const uint8_t BORDER_STR_VERT_R[3] = { 0x02, 0xF1, 0xF0 };

/* "COPYRIGHT 1982 STUART SMITH" */
static const uint8_t S_COPYRIGHT[28] = {
    0x1B, 0xC3, 0xCF, 0xD0, 0xD9, 0xD2, 0xC9, 0xC7, 0xC8, 0xD4, 0xA0, 0xB1, 0xB9, 0xB8,
    0xB2, 0xA0, 0xD3, 0xD4, 0xD5, 0xC1, 0xD2, 0xD4, 0xA0, 0xD3, 0xCD, 0xC9, 0xD4, 0xC8,
};
/* "PRESS SPACE BAR TO CONTINUE" */
static const uint8_t S_PRESS_SPACE_BOOT[28] = {
    0x1B, 0xD0, 0xD2, 0xC5, 0xD3, 0xD3, 0xA0, 0xD3, 0xD0, 0xC1, 0xC3, 0xC5, 0xA0, 0xC2,
    0xC1, 0xD2, 0xA0, 0xD4, 0xCF, 0xA0, 0xC3, 0xCF, 0xCE, 0xD4, 0xC9, 0xCE, 0xD5, 0xC5,
};
/* Three rows of a picture in font '5', glyphs $E8-$FE. */
static const uint8_t S_DECOR_ROW1[8] = { 0x07, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE };
static const uint8_t S_DECOR_ROW2[9] = { 0x08, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6 };
static const uint8_t S_DECOR_ROW3[9] = { 0x08, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE };

/*
    The 2x2 glyphs init_game_display draws, each with its top left at a
    text column and row. Glyphs 0-2 are of font group 2 (font '3' and
    up), drawn by DRAW_BORDER_CHAR. Glyphs 3-8 are the frame's
    ornaments, of font group 1 (font '1' and up), drawn by
    DRAW_BORDER_ICONS through PLOT_CHAR.
*/
static const uint8_t TITLE_GLYPH[9] = { 0x01, 0x0E, 0x04, 0x02, 0x01, 0x03, 0x04, 0x1C, 0x1C };
static const uint8_t TITLE_GLYPH_COL[9] = { 0x10, 0x12, 0x14, 0x04, 0x04, 0x22, 0x22, 0x10, 0x16 };
static const uint8_t TITLE_GLYPH_ROW[9] = { 0x03, 0x03, 0x03, 0x0B, 0x10, 0x10, 0x0B, 0x10, 0x0B };

/* Draws glyphs `i` up to `end` - 1 of TITLE_GLYPH, of font group
 * `charset`. PLOT_CHAR and DRAW_BORDER_CHAR draw one each. */
static void plot_title_glyphs(uint8_t i, uint8_t end, uint8_t charset)
{
    do {
        FONT_CHARNUM = TITLE_GLYPH[i];
        TEXT_COL = TITLE_GLYPH_COL[i];
        TEXT_ROW = TITLE_GLYPH_ROW[i];
        FONT_CHARSET = charset;
        print_fontchar_at_text_pos();
    } while (++i != end);
}

/* Prints string `s` at border label positions `first` to `last`, the
 * last first. One loop of DRAW_BORDER_LABELS. */
static void draw_border_strings(const uint8_t *s, uint8_t first, uint8_t last)
{
    for (;;) {
        TEXT_COL = BORDER_LABEL_COL[last];
        TEXT_ROW = BORDER_LABEL_ROW[last];
        print_string(s);
        if (last == first)
            break;
        last--;
    }
}

/* Draws the border frame in font '2'. DRAW_BORDER_LABELS. */
static void draw_border_labels(void)
{
    hrcg_cout(HRCG_CTRL_A);
    hrcg_cout(0xB2);
    draw_border_strings(BORDER_STR_HORIZ, 0, 5);
    draw_border_strings(BORDER_STR_VERT_L, 6, 15);
    draw_border_strings(BORDER_STR_VERT_R, 16, 25);
}

/* Prints `s` at text column `col` of row `row`. */
static void print_at(const uint8_t *s, uint8_t col, uint8_t row)
{
    TEXT_ROW = row;
    TEXT_COL = col;
    print_string(s);
}

/* Draws the title screen: the viewports, three glyphs of font '3' at the
 * top, the border frame and its ornaments, the copyright and the prompt
 * at the bottom, and a picture in font '5' at the bottom right.
 * INIT_GAME_DISPLAY. */
static void init_game_display(void)
{
    hrcg_cout(HRCG_CTRL_P);
    TEXT_STREAM_PTR = UI_TEXT_STREAM;
    text_renderer();

    plot_title_glyphs(0, 3, 2);
    draw_border_labels();
    plot_title_glyphs(3, 9, 1);

    print_ctrl_ab();
    print_at(S_COPYRIGHT, 0, 0x15);
    print_at(S_PRESS_SPACE_BOOT, 0, 0x17);

    hrcg_cout(HRCG_CTRL_A);
    hrcg_cout(0xB5);
    print_at(S_DECOR_ROW1, 0x1E, 0x15);
    /* The original increments TEXT_ROW after each row. */
    print_at(S_DECOR_ROW2, 0x1E, TEXT_ROW + 1);
    print_at(S_DECOR_ROW3, 0x1E, TEXT_ROW + 1);
}

/*
    Draws the title screen and plays the title tune, with the border
    animation running, until a key ends it. INIT_GAME_STATE.

    The original's $B1 to $80A6 is the font selector '1' in RENDER_FONT_
    CHAR's string, which the border animation draws with. Its comment
    calls the byte a "LDA (zp),Y opcode". Its last three stores write
    JMP TUNE_ADVANCE over the tune player's JMP (DISPATCH_VEC) at $7A49,
    which ends the animation, and clearing BORDER_ANIM_ON does that here.
*/
static void init_game_state(void)
{
    init_game_display();
    border_next = BORDER_RESTART;
    INPUT_MODE = 0;
    CHAR_CHARSET = 0xB1;
    tune_player(TUNE_ATTRACT, 0);
    BORDER_ANIM_ON = false;
}

/* ---- The rest of the start ------------------------------------------------ */

/*
    Asks for the keyboard or the joystick. Space chooses the keyboard,
    any other key is ignored, and a change of button 0 from its state
    at the start chooses the joystick. SELECT_INPUT_MODE, whose comments
    call $C061 button 1.
*/
static void select_input_mode(void)
{
    uint8_t button;
    uint8_t key;

    display_message(MSG_INPUT_PROMPT);
    button = BUTN[0] & 0x80;
    do {
        KBDSTRB = 0;
        while (!((key = KBD) & 0x80)) {
            if ((BUTN[0] ^ button) & 0x80) {
                INPUT_MODE = 1;
                set_cursor_row21();
                display_message(MSG_JOYSTICK_MODE);
                return;
            }
        }
    } while (key != 0xA0);
    INPUT_MODE = 0;
}

/*
    Sums the player characters of locations 1 on into TOTAL_MOB_COUNT,
    up to the record whose name's high byte is $FF. INIT_MOB_COUNT.

    The original advances CHAR_PTR with an add that takes the carry out
    of the sum, so a total past 255 would skip a byte. The C does the
    same. It leaves CHAR_PTR at the end record, as the original does.
*/
static void init_mob_count(void)
{
    TOTAL_MOB_COUNT = 0;
    CHAR_PTR = &LOCATIONS[1];
    while (((const uint8_t *)CHAR_PTR)[1] != 0xFF) {
        uint8_t count = CHAR_PTR->pc_count;
        uint8_t total = TOTAL_MOB_COUNT + count;

        TOTAL_MOB_COUNT = total;
        CHAR_PTR++;
        if (total < count)      /* the carry out of the sum */
            CHAR_PTR = (struct location *)((uint8_t *)CHAR_PTR + 1);
    }
}

/* Points RWTS_IOB_PTR at DOS's IOB. INIT_DISK_IOB. */
static void init_disk_iob(void)
{
    RWTS_IOB_PTR = &RWTS_IOB;
}

/*
    Points RESET at cold_restart, shows the high-resolution page 1, sets
    up the HRCG and the text window, plays the title screen, runs the
    save/load menu, and counts the player characters. GAME_INIT.

    SOFTEV gets the constant $A56F, which is where the linker script
    puts cold_restart, since taking cold_restart's address would make it
    ineligible for the ZP-passing ABI.
*/
/* Clears hi-res page 1, $2000-$3FFF, as RELOCATE_HIRES_STAGING does
 * when it copies the loader's staged pages out of it. */
static void clear_hires_page1(void)
{
    for (uint8_t *page = (uint8_t *)0x2000; page != (uint8_t *)0x4000;
         page += 256) {
        uint8_t i = 0;
        do {
            page[i] = 0;
        } while (++i != 0);
    }
}

void game_init(void)
{
    SOFTEV = 0xA56F;
    SETPWRC();
    BLINK_COL = 0x14;
    clear_hires_page1();

    (void)TXTCLR;
    (void)MIXCLR;
    (void)TXTPAGE1;
    (void)HIRES;

    init_disk_iob();
    hrcg_init();
    set_text_window_wrap();
    init_game_state();
    fill_map();
    set_text_window_bottom();

    LOCATION_POS = 0;
    LOCATION_STYLE = 0;
    ((uint8_t *)&ENTITY_PTR)[1] = 0;

    select_input_mode();
    run_chain(CHAIN_INIT_WORLD);
    init_mob_count();
    CURRENT_PLAYER = 1;
}
