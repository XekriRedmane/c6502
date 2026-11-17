/*
    player.c -- moving the player one step each turn: falling, running,
    climbing, swinging on ropes, digging, and picking up gold.

    A player's place is a level cell (PLAYER_COL, PLAYER_ROW) and an
    adjustment of 0-4 on each axis, so a cell takes five steps to cross.
    A move that takes the adjustment past either end puts the player in
    the next cell. A dig takes twelve turns, and on the last one the
    brick disappears and the hole starts its timer.

    Departures from the original:

    - GET_SPRITE_AND_SCREEN_COORD_AT_PLAYER returns the player's sprite
      in A and its screen place in X and Y, which every caller passes
      straight to ERASE_SPRITE_AT_PIXEL_COORDS or
      DRAW_SPRITE_AT_PIXEL_COORDS. Here erase_player and draw_player
      compute the three arguments themselves.
    - TRY_DIGGING_LEFT and TRY_DIGGING_RIGHT are one routine,
      keep_digging, which picks the side from bit 7 of DIG_DIRECTION
      as MOVE_PLAYER does. start_digging is the code before their
      _check_can_dig_ entries, and MOVE_PLAYER's jumps to those
      entries are calls to keep_digging. A right dig counts
      DIG_ANIM_STATE from 0x0C to 0x18 where a left dig counts from 0
      to 0x0C, and keep_digging indexes the tables with the same
      offsets the original does.
    - DIG_DEBRIS_LEFT_SPRITES, DIG_DEBRIS_RIGHT_SPRITES,
      DIG_BRICK_SPRITES, DIG_NOTE_PITCHES, and DIG_NOTE_DURATIONS are
      one array, dig_tables, in the original's order. The right dig
      reads its debris sprites as DIG_DEBRIS_LEFT_SPRITES + 0x0C to
      + 0x17, which are the bytes of DIG_DEBRIS_RIGHT_SPRITES, and its
      bricks and notes 12 bytes before the tables' names. One array
      makes each of those reads the same byte the original reads.
    - Code the original repeats is in small functions. leave_cell
      restores the cell the player leaves in each of the five moves.
      step_down is the downward step that falling and TRY_MOVING_DOWN
      share. climb is TRY_MOVING_UP's tail, which TRY_MOVING_DOWN jumps
      to, and run_or_swing is the tail of TRY_MOVING_LEFT and
      TRY_MOVING_RIGHT.
    - DROP_PLAYER_IN_HOLE returns with the carry set on both of its
      paths, so keep_digging returns true after it. MOVE_PLAYER passes
      the dig's carry back to its caller, which doesn't read it, and
      move_player returns nothing.
    - TRY_MOVING_UP reads the row below the player when the player is
      off a ladder and below the cell's center. In row 15 that row is
      past the end of the original's row pointer tables, and it reads
      the cell from an address built out of
      CURR_LEVEL_ROW_SPRITES_PTR_PAGES[0] and HALF_SCREEN_COL_TABLE[0],
      where this reads LEVEL_BG[16]. The player can't be in that place.
      A level starts the player with PLAYER_Y_ADJ at 2, moving down and
      falling stop at 2 in row 15, and only a move up into a cell sets
      it to 4.

    Lode Runner, main.asm lines 1211-2022:

        ORG    $64BD
    MOVE_PLAYER:
        SUBROUTINE

        LDA     #$01
        STA     DIDNT_PICK_UP_GOLD   ; Reset DIDNT_PICK_UP_GOLD

        ; If we're digging, see if we can keep digging.
        LDA     DIG_DIRECTION
        BEQ     .not_digging
        BPL     .digging_right
        JMP     TRY_DIGGING_LEFT_check_can_dig_left

    .digging_right:
        JMP     TRY_DIGGING_RIGHT_check_can_dig_right

    .not_digging:
        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     PLAYER_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_LADDER
        BEQ     .check_for_keyboard_input_      ; ladder at background location?
        CMP     #SPRITE_ROPE
        BNE     .check_if_player_should_fall    ; rope at background location?
        LDA     PLAYER_Y_ADJ
        CMP     #$02
        BEQ     .check_for_keyboard_input_      ; player at exact sprite row?

        ; player is not on exact sprite row, fallthrough.

    .check_if_player_should_fall:
        LDA     PLAYER_Y_ADJ
        CMP     #$02
        BCC     .make_player_fall               ; player slightly above sprite row?

        LDY     PLAYER_ROW
        CPY     #MAX_GAME_ROW
        BEQ     .check_for_keyboard_input_      ; player exactly sprite row 15?

        ; Check the sprite at the player location
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES+1,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1


        LDY     PLAYER_COL
        LDA     (PTR1),Y
        CMP     #SPRITE_EMPTY
        BEQ     .make_player_fall
        CMP     #SPRITE_GUARD
        BEQ     .check_for_keyboard_input_
        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BEQ     .check_for_keyboard_input_
        CMP     #SPRITE_STONE
        BEQ     .check_for_keyboard_input_
        CMP     #SPRITE_LADDER
        BNE     .make_player_fall

    .check_for_keyboard_input_:
        JMP     .check_for_keyboard_input

    .make_player_fall:
        LDA     #$00
        STA     $9B                     ; $9B = 0
        JSR     GET_SPRITE_AND_SCREEN_COORD_AT_PLAYER
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS

        LDA     #$07                    ; Next anim state: player falling, facing left
        LDX     PLAYER_FACING_DIRECTION
        BMI     .player_facing_left
        LDA     #$0F                    ; Next anim state: player falling, facing right
    .player_facing_left:
        STA     PLAYER_ANIM_STATE

        JSR     NUDGE_PLAYER_TOWARDS_EXACT_COLUMN

        INC     PLAYER_Y_ADJ            ; Move down one
        LDA     PLAYER_Y_ADJ
        CMP     #$05
        BCS     .adjustment_overflow

        JSR     CHECK_FOR_GOLD_PICKED_UP_BY_PLAYER
        JMP     DRAW_PLAYER            ; tailcall

    .adjustment_overflow:
        LDA     #$00
        STA     PLAYER_Y_ADJ            ; Set vertical adjust to -2

        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     PLAYER_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BNE     .set_on_level
        LDA     #SPRITE_EMPTY
    .set_on_level:
        STA     (PTR1),Y
        INC     PLAYER_ROW          ; Move down
        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1


        LDY     PLAYER_COL
        LDA     #SPRITE_PLAYER
        STA     (PTR1),Y
        JMP     DRAW_PLAYER     ; tailcall

    .check_for_keyboard_input:
        LDA     $9B
        BNE     .check_for_key  ; $9B doesn't play note
        LDA     #$64
        LDX     #$08
        JSR     PLAY_NOTE       ; play note, pitch 0x64, duration 8.

    .check_for_key:
        LDA     #$20
        STA     $A4
        STA     $9B
        JSR     CHECK_FOR_INPUT
        LDA     KEY_COMMAND
        CMP     #$C9            ; 'I'
        BNE     .check_for_K
        JSR     TRY_MOVING_UP
        BCS     .check_for_J    ; couldn't move up
        RTS

    .check_for_K:
        CMP     #$CB            ; 'K'
        BNE     .check_for_U
        JSR     TRY_MOVING_DOWN
        BCS     .check_for_J
        RTS

    .check_for_U:
        CMP     #$D5            ; 'U'
        BNE     .check_for_O
        JSR     TRY_DIGGING_LEFT
        BCS     .check_for_J
        RTS

    .check_for_O:
        CMP     #$CF            ; 'O'
        BNE     .check_for_J
        JSR     TRY_DIGGING_RIGHT
        BCS     .check_for_J
        RTS

    .check_for_J:
        LDA     KEY_COMMAND_LR
        CMP     #$CA            ; 'J'
        BNE     .check_for_L
        JMP     TRY_MOVING_LEFT

    .check_for_L:
        CMP     #$CC            ; 'L'
        BNE     .end
        JMP     TRY_MOVING_RIGHT

    .end:
        RTS

        ORG    $65D3
    TRY_MOVING_LEFT:
        SUBROUTINE

        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDX     PLAYER_X_ADJ
        CPX     #$03
        BCS     .move_player_left       ; player slightly right, so can move left.

        LDY     PLAYER_COL
        BEQ     .cannot_move            ; col == 0, so cannot move.

        DEY
        LDA     (PTR1),Y
        CMP     #SPRITE_STONE
        BEQ     .cannot_move
        CMP     #SPRITE_BRICK
        BEQ     .cannot_move
        CMP     #SPRITE_TRAP
        BNE     .move_player_left       ; brick, stone, or trap to left, so cannot move.

    .cannot_move:
        RTS

    .move_player_left:
        JSR     GET_SPRITE_AND_SCREEN_COORD_AT_PLAYER
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        LDA     #$FF
        STA     PLAYER_FACING_DIRECTION             ; face left
        JSR     NUDGE_PLAYER_TOWARDS_EXACT_ROW
        DEC     PLAYER_X_ADJ
        BPL     .check_for_gold

        ; adjustment overflow
        LDY     PLAYER_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BNE     .set_on_level
        LDA     #SPRITE_EMPTY
    .set_on_level:
        STA     (PTR1),Y

        DEC     PLAYER_COL
        DEY
        LDA     #SPRITE_PLAYER
        STA     (PTR1),Y            ; Write player sprite to active page.
        LDA     #$04
        STA     PLAYER_X_ADJ        ; Set adjustment to +2
        BNE     .inc_anim_state     ; Unconditional

    .check_for_gold:
        JSR     CHECK_FOR_GOLD_PICKED_UP_BY_PLAYER

    .inc_anim_state:
        LDY     PLAYER_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_ROPE
        BEQ     .anim_state_monkeying

        LDA     #$00
        LDX     #$02
        BNE     .done               ; Unconditional

    .anim_state_monkeying:
        LDA     #$03
        LDX     #$05

    .done:
        JSR     INC_ANIM_STATE
        JMP     DRAW_PLAYER

        ORG    $6645
    TRY_MOVING_RIGHT:
        SUBROUTINE

        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDX     PLAYER_X_ADJ
        CPX     #$02
        BCC     .move_player_right      ; player slightly left, so can move right.

        LDY     PLAYER_COL
        CPY     #MAX_GAME_COL
        BEQ     .cannot_move            ; col == 27, so cannot move.

        INY
        LDA     (PTR1),Y
        CMP     #SPRITE_STONE
        BEQ     .cannot_move
        CMP     #SPRITE_BRICK
        BEQ     .cannot_move
        CMP     #SPRITE_TRAP
        BNE     .move_player_right      ; brick, stone, or trap to right, so cannot move.

    .cannot_move:
        RTS

    .move_player_right:
        JSR     GET_SPRITE_AND_SCREEN_COORD_AT_PLAYER
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        LDA     #$01
        STA     PLAYER_FACING_DIRECTION             ; face right
        JSR     NUDGE_PLAYER_TOWARDS_EXACT_ROW
        INC     PLAYER_X_ADJ
        LDA     PLAYER_X_ADJ
        CMP     #$05
        BCC     .check_for_gold

        ; adjustment overflow
        LDY     PLAYER_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BNE     .set_on_level
        LDA     #SPRITE_EMPTY
    .set_on_level:
        STA     (PTR1),Y

        INC     PLAYER_COL
        INY
        LDA     #SPRITE_PLAYER
        STA     (PTR1),Y            ; Write player sprite to active page.
        LDA     #$00
        STA     PLAYER_X_ADJ        ; Set adjustment to -2
        BEQ     .inc_anim_state     ; Unconditional

    .check_for_gold:
        JSR     CHECK_FOR_GOLD_PICKED_UP_BY_PLAYER

    .inc_anim_state:
        LDY     PLAYER_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_ROPE
        BEQ     .anim_state_monkeying

        LDA     #$08
        LDX     #$0A
        BNE     .done               ; Unconditional

    .anim_state_monkeying:
        LDA     #$0B
        LDX     #$0D

    .done:
        JSR     INC_ANIM_STATE
        JMP     DRAW_PLAYER

        ORG    $66BD
    TRY_MOVING_UP:
        SUBROUTINE

        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     PLAYER_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_LADDER
        BEQ     .ladder_here

        LDY     PLAYER_Y_ADJ
        CPY     #$03
        BCC     .cannot_move       ; if PLAYER_Y_ADJ <= 2

        ; and if there's no ladder below, you can't move up.
        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     PLAYER_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_LADDER
        BEQ     .move_player_up

    .cannot_move:
        SEC
        RTS

    .ladder_here:
        LDY     PLAYER_Y_ADJ
        CPY     #$03
        BCS     .move_player_up             ; if PLAYER_Y_ADJ > 2

        ; If you're at the top, you can't move up even if there's a ladder.
        LDY     PLAYER_ROW
        BEQ     .cannot_move       ; if PLAYER_ROW == 0, set carry and return

        ; You can't move up if there's a brick, stone, or trap above.
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS-1,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES-1,Y
        STA     PTR1+1
        LDY     PLAYER_COL
        LDA     (PTR1),Y                    ; Get the sprite on the row above.

        CMP     #SPRITE_BRICK
        BEQ     .cannot_move
        CMP     #SPRITE_STONE
        BEQ     .cannot_move
        CMP     #SPRITE_TRAP
        BEQ     .cannot_move       ; If brick, stone, or trap, set carry and return

    .move_player_up:
        JSR     GET_SPRITE_AND_SCREEN_COORD_AT_PLAYER
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        JSR     NUDGE_PLAYER_TOWARDS_EXACT_COLUMN
        DEC     PLAYER_Y_ADJ                ; Move player up
        BPL     TRY_MOVING_UP_check_for_gold

        ; PLAYER_Y_ADJ rolled over.

        ; Restore the sprite at the player's former location:
        ; If background page at player location is brick, put an empty at the
        ; (previous) player location on active page, otherwise copy the background
        ; sprite to the active page.
        LDY     PLAYER_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BNE     .set_on_real_page
        LDA     #SPRITE_EMPTY
    .set_on_real_page:
        STA     (PTR1),Y

        DEC     PLAYER_ROW                  ; Move player up
        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     PLAYER_COL
        LDA     #SPRITE_PLAYER
        STA     (PTR1),Y            ; Write player sprite to active page.
        LDA     #$04
        STA     PLAYER_Y_ADJ        ; Set adjustment to +2
        BNE     TRY_MOVING_UP_inc_anim_state     ; unconditional

    TRY_MOVING_UP_check_for_gold:
        JSR     CHECK_FOR_GOLD_PICKED_UP_BY_PLAYER

    TRY_MOVING_UP_inc_anim_state:
        LDA     #$10
        LDX     #$11
        JSR     INC_ANIM_STATE      ; player climbing on ladder
        JSR     DRAW_PLAYER
        CLC
        RTS

        ORG    $6766
    TRY_MOVING_DOWN:
        SUBROUTINE

        LDY     PLAYER_Y_ADJ
        CPY     #$02
        BCC     .move_player_down   ; player slightly above, so can move down.

        LDY     PLAYER_ROW
        CPY     #MAX_GAME_ROW
        BCS     .cannot_move        ; player on row >= 15, so cannot move.

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES+1,Y
        STA     PTR1+1

        LDY     PLAYER_COL
        LDA     (PTR1),Y
        CMP     #SPRITE_STONE
        BEQ     .cannot_move
        CMP     #SPRITE_BRICK
        BNE     .move_player_down   ; Row below is stone or brick, so cannot move.

    .cannot_move:
        SEC
        RTS

    .move_player_down:
        JSR     GET_SPRITE_AND_SCREEN_COORD_AT_PLAYER
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        JSR     NUDGE_PLAYER_TOWARDS_EXACT_COLUMN
        INC     PLAYER_Y_ADJ                ; Move player down
        LDA     PLAYER_Y_ADJ
        CMP     #$05
        BCC     .check_for_gold_

        ; adjustment overflow
        LDY     PLAYER_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BNE     .set_on_real_page
        LDA     #SPRITE_EMPTY
    .set_on_real_page:
        STA     (PTR1),Y

        INC     PLAYER_ROW
        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     PLAYER_COL
        LDA     #SPRITE_PLAYER
        STA     (PTR1),Y            ; Write player sprite to active page.
        LDA     #SPRITE_EMPTY
        STA     PLAYER_Y_ADJ        ; Set adjustment to -2
        JMP     TRY_MOVING_UP_inc_anim_state

    .check_for_gold_:
        JMP     TRY_MOVING_UP_check_for_gold

        ORG    $67D8
        SUBROUTINE

    .cannot_dig_:
        JMP     .stop_digging

    TRY_DIGGING_LEFT:
        LDA     #$FF
        STA     DIG_DIRECTION
        STA     KEY_COMMAND
        STA     KEY_COMMAND_LR      ; DIG_DIRECTION = KEY_COMMANDs = 0xFF
        LDA     #$00
        STA     DIG_ANIM_STATE      ; DIG_ANIM_STATE = 0

    TRY_DIGGING_LEFT_check_can_dig_left:
        LDY     PLAYER_ROW
        CPY     #MAX_GAME_ROW
        BCS     .cannot_dig_        ; row >= 15, so cannot dig.

        INY
        JSR     GET_PTRS_TO_CURR_LEVEL_SPRITE_DATA
        LDY     PLAYER_COL
        BEQ     .cannot_dig_        ; col == 0, so cannot dig left.

        DEY
        LDA     (PTR1),Y
        CMP     #SPRITE_BRICK
        BNE     .cannot_dig_        ; no brick below and to the left, so cannot dig left.

        LDY     PLAYER_ROW
        JSR     GET_PTRS_TO_CURR_LEVEL_SPRITE_DATA
        LDY     PLAYER_COL
        DEY
        LDA     (PTR1),Y
        CMP     #SPRITE_EMPTY
        BNE     .not_empty_to_left  ; not empty to the left, so maybe cannot dig left.

        ; Can dig!
        JSR     GET_SPRITE_AND_SCREEN_COORD_AT_PLAYER
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        JSR     NUDGE_PLAYER_TOWARDS_EXACT_COLUMN
        JSR     NUDGE_PLAYER_TOWARDS_EXACT_ROW
        LDY     DIG_ANIM_STATE
        LDA     DIG_NOTE_PITCHES,Y
        LDX     DIG_NOTE_DURATIONS,Y
        JSR     APPEND_NOTE

        LDX     DIG_ANIM_STATE
        LDA     #$00                ; running left
        CPX     #$06
        BCS     .note_0             ; DIG_ANIM_STATE >= 6
        LDA     #$06                ; digging left
    .note_0:
        STA     PLAYER_ANIM_STATE
        JSR     DRAW_PLAYER

        LDX     DIG_ANIM_STATE
        CPX     #$0C
        BEQ     .move_player_left
        CPX     #$00
        BEQ     .draw_curr_dig              ; Don't have to erase previous dig debris sprite

        ; Erase the previous dig debris sprite
        LDA     DIG_DEBRIS_LEFT_SPRITES-1,X
        PHA
        LDX     PLAYER_COL
        DEX
        LDY     PLAYER_ROW
        JSR     GET_SCREEN_COORDS_FOR
        PLA
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS

        LDX     DIG_ANIM_STATE
    .draw_curr_dig:
        LDA     DIG_DEBRIS_LEFT_SPRITES,X
        PHA
        LDX     PLAYER_COL
        DEX
        STX     GAME_COLNUM
        LDY     PLAYER_ROW
        STY     GAME_ROWNUM
        JSR     GET_SCREEN_COORDS_FOR
        PLA
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS     ; Draw current dig debris sprite above dig site

        LDX     DIG_ANIM_STATE
        LDA     DIG_BRICK_SPRITES,X
        INC     GAME_ROWNUM
        JSR     DRAW_SPRITE_PAGE1               ; Draw dig brick sprite at dig site

        INC     DIG_ANIM_STATE
        CLC
        RTS

    .not_empty_to_left:
        LDY     PLAYER_ROW
        INY
        STY     GAME_ROWNUM
        LDY     PLAYER_COL
        DEY
        STY     GAME_COLNUM
        LDA     #SPRITE_BRICK
        JSR     DRAW_SPRITE_PAGE1           ; Draw brick below and to the left of player

        LDX     DIG_ANIM_STATE
        BEQ     .stop_digging

        ; Erase previous dig debris sprite
        DEX
        LDA     DIG_DEBRIS_LEFT_SPRITES,X
        PHA
        LDY     PLAYER_ROW
        LDX     PLAYER_COL
        DEX
        JSR     GET_SCREEN_COORDS_FOR
        PLA
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS

    .stop_digging:
        LDA     #$00
        STA     DIG_DIRECTION
        SEC
        RTS

    .move_player_left:
        LDX     PLAYER_COL
        DEX
        JMP     DROP_PLAYER_IN_HOLE

        ORG    $689E
        SUBROUTINE

    .cannot_dig_:
        JMP     .stop_digging

    TRY_DIGGING_RIGHT:
        LDA     #$01
        STA     DIG_DIRECTION
        STA     KEY_COMMAND
        STA     KEY_COMMAND_LR      ; DIG_DIRECTION = KEY_COMMANDs = 0x01
        LDA     #$0C
        STA     DIG_ANIM_STATE      ; DIG_ANIM_STATE = 0x0C

    TRY_DIGGING_RIGHT_check_can_dig_right:
        LDY     PLAYER_ROW
        CPY     #MAX_GAME_ROW
        BCS     .cannot_dig_        ; row >= 15, so cannot dig.

        INY
        JSR     GET_PTRS_TO_CURR_LEVEL_SPRITE_DATA
        LDY     PLAYER_COL
        CPY     #MAX_GAME_COL
        BCS     .cannot_dig_        ; col >= 27, so cannot dig right.

        INY
        LDA     (PTR1),Y
        CMP     #SPRITE_BRICK
        BNE     .cannot_dig_        ; no brick below and to the right, so cannot dig right.

        LDY     PLAYER_ROW
        JSR     GET_PTRS_TO_CURR_LEVEL_SPRITE_DATA
        LDY     PLAYER_COL
        INY
        LDA     (PTR1),Y
        CMP     #SPRITE_EMPTY
        BNE     .not_empty_to_right  ; not empty to the right, so maybe cannot dig right.

        ; Can dig!
        JSR     GET_SPRITE_AND_SCREEN_COORD_AT_PLAYER
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        JSR     NUDGE_PLAYER_TOWARDS_EXACT_COLUMN
        JSR     NUDGE_PLAYER_TOWARDS_EXACT_ROW
        LDY     DIG_ANIM_STATE
        LDA     DIG_NOTE_PITCHES-12,Y
        LDX     DIG_NOTE_DURATIONS-12,Y
        JSR     APPEND_NOTE

        LDX     DIG_ANIM_STATE
        LDA     #$08                ; running right
        CPX     #$12
        BCS     .note_0             ; DIG_ANIM_STATE >= 0x12
        LDA     #$0E                ; digging right
    .note_0:
        STA     PLAYER_ANIM_STATE
        JSR     DRAW_PLAYER

        LDX     DIG_ANIM_STATE
        CPX     #$18
        BEQ     .move_player_right
        CPX     #$0C
        BEQ     .draw_curr_dig              ; Don't have to erase previous dig debris sprite

        ; Erase the previous dig debris sprite
        LDA     DIG_DEBRIS_LEFT_SPRITES-1,X
        PHA
        LDX     PLAYER_COL
        INX
        LDY     PLAYER_ROW
        JSR     GET_SCREEN_COORDS_FOR
        PLA
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS

        LDX     DIG_ANIM_STATE
    .draw_curr_dig:
        LDA     DIG_DEBRIS_LEFT_SPRITES,X
        PHA
        LDX     PLAYER_COL
        INX
        STX     GAME_COLNUM
        LDY     PLAYER_ROW
        STY     GAME_ROWNUM
        JSR     GET_SCREEN_COORDS_FOR
        PLA
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS     ; Draw current dig debris sprite above dig site

        INC     GAME_ROWNUM
        LDX     DIG_ANIM_STATE
        LDA     DIG_BRICK_SPRITES-12,X
        JSR     DRAW_SPRITE_PAGE1               ; Draw dig brick sprite at dig site

        INC     DIG_ANIM_STATE
        CLC
        RTS

    .not_empty_to_right:
        LDY     PLAYER_ROW
        INY
        STY     GAME_ROWNUM
        LDY     PLAYER_COL
        INY
        STY     GAME_COLNUM
        LDA     #SPRITE_BRICK
        JSR     DRAW_SPRITE_PAGE1           ; Draw brick below and to the right of player

        LDX     DIG_ANIM_STATE
        CPX     #$0C
        BEQ     .stop_digging

        ; Erase previous dig debris sprite
        DEX
        LDA     DIG_DEBRIS_LEFT_SPRITES,X
        PHA
        LDX     PLAYER_COL
        INX
        LDY     PLAYER_ROW
        JSR     GET_SCREEN_COORDS_FOR
        PLA
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS

    .stop_digging:
        LDA     #$00
        STA     DIG_DIRECTION
        SEC
        RTS

    .move_player_right:
        LDX     PLAYER_COL
        INX
        JMP     DROP_PLAYER_IN_HOLE

        ORG    $6968
    SPRITE_ANIM_SEQS:
        HEX     0B 0C 0D        ; player running left
        HEX     18 19 1A        ; player monkey swinging left
        HEX     0F              ; player digging left
        HEX     13              ; player falling, facing left
        HEX     09 10 11        ; player running right
        HEX     15 16 17        ; player monkey swinging right
        HEX     25              ; player digging right
        HEX     14              ; player falling, facing right
        HEX     0E 12           ; player climbing on ladder
        ORG    $697A
    DIG_DEBRIS_LEFT_SPRITES:
        HEX     1B 1B 1C 1C 1D 1D 1E 1E 00 00 00 00
    DIG_DEBRIS_RIGHT_SPRITES:
        HEX     26 26 27 27 1D 1D 1E 1E 00 00 00 00
    DIG_BRICK_SPRITES:
        HEX     1F 1F 20 20 21 21 22 22 23 23 24 24
    DIG_NOTE_PITCHES:
        HEX     20 20 20 20 20 20 20 20 24 24 24 24 24
    DIG_NOTE_DURATIONS:
        HEX     04 04 04 04 04 04 04 04 03 03 02 02 01

    Lode Runner, main.asm lines 2423-2610:

        ORG    $6B85
    GET_SPRITE_AND_SCREEN_COORD_AT_PLAYER:
        SUBROUTINE
        ; Using PLAYER_COL/ROW, PLAYER_X/Y_ADJ, and PLAYER_ANIM_STATE,
        ; return the player sprite in A, and the screen coords in X and Y.

        LDX     PLAYER_COL
        LDY     PLAYER_X_ADJ
        JSR     GET_HALF_SCREEN_COL_OFFSET_IN_Y_FOR
        STX     SPRITE_NUM          ; Used only as a temporary to save X
        LDY     PLAYER_ROW
        LDX     PLAYER_Y_ADJ
        JSR     GET_SCREEN_ROW_OFFSET_IN_X_FOR
        LDX     PLAYER_ANIM_STATE
        LDA     SPRITE_ANIM_SEQS,X
        LDX     SPRITE_NUM
        RTS

        ORG    $6B9D
    CHECK_FOR_GOLD_PICKED_UP_BY_PLAYER:
        SUBROUTINE

        LDA     PLAYER_X_ADJ
        CMP     #$02
        BNE     .end
        LDA     PLAYER_Y_ADJ
        CMP     #$02
        BNE     .end

        LDY     PLAYER_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     PLAYER_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_GOLD
        BNE     .end

        LSR     DIDNT_PICK_UP_GOLD  ; picked up gold
        DEC     GOLD_COUNT          ; GOLD_COUNT--

        LDY     PLAYER_ROW
        STY     GAME_ROWNUM
        LDY     PLAYER_COL
        STY     GAME_COLNUM
        LDA     #SPRITE_EMPTY
        STA     (PTR2),Y
        JSR     DRAW_SPRITE_PAGE2   ; Register and draw blank at player loc in background screen

        LDY     GAME_ROWNUM
        LDX     GAME_COLNUM
        JSR     GET_SCREEN_COORDS_FOR
        LDA     #SPRITE_GOLD
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS    ; Erase gold at player loc

        LDY     #$02
        LDA     #$50
        JSR     ADD_AND_UPDATE_SCORE            ; SCORE += 250
        JSR     LOAD_SOUND_DATA
        HEX     07 45 06 55 05 44 04 54 03 43 02 53 00

    .end:
        RTS

        ORG    $6BF4
    INC_ANIM_STATE:
        SUBROUTINE

        INC     PLAYER_ANIM_STATE
        CMP     PLAYER_ANIM_STATE
        BCC     .check_upper_bound      ; lower bound < PLAYER_ANIM_STATE?
        ; otherwise PLAYER_ANIM_STATE <= lower bound:

    .write_lower_bound:
        STA     PLAYER_ANIM_STATE       ; PLAYER_ANIM_STATE = lower bound
        RTS

    .check_upper_bound:
        CPX     PLAYER_ANIM_STATE
        BCC     .write_lower_bound       ; PLAYER_ANIM_STATE > upper bound?
        ; otherwise PLAYER_ANIM_STATE <= upper bound:
        RTS

        ORG    $6C02
    DRAW_PLAYER:
        SUBROUTINE

        JSR     GET_SPRITE_AND_SCREEN_COORD_AT_PLAYER
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS
        LDA     SCREENS_DIFFER
        BEQ     .end
        LDA     DIDNT_PICK_UP_GOLD
        BEQ     .end
        LSR     ALIVE       ; Set player as dead
    .end
        RTS

        ORG    $6C13
    NUDGE_PLAYER_TOWARDS_EXACT_COLUMN:
        SUBROUTINE

        LDA     PLAYER_X_ADJ
        CMP     #$02
        BCC     .player_slightly_left
        BEQ     .end

    .player_slightly_right:
        DEC     PLAYER_X_ADJ        ; Nudge player left
        JMP     CHECK_FOR_GOLD_PICKED_UP_BY_PLAYER

    .player_slightly_left:
        INC     PLAYER_X_ADJ        ; Nudge player right
        JMP     CHECK_FOR_GOLD_PICKED_UP_BY_PLAYER

    .end:
        RTS

        ORG    $6C26
    NUDGE_PLAYER_TOWARDS_EXACT_ROW:
        SUBROUTINE

        LDA     PLAYER_Y_ADJ
        CMP     #$02
        BCC     .player_slightly_above
        BEQ     .end

    .player_slightly_below:
        DEC     PLAYER_Y_ADJ        ; Nudge player up
        JMP     CHECK_FOR_GOLD_PICKED_UP_BY_PLAYER

    .player_slightly_above:
        INC     PLAYER_Y_ADJ        ; Nudge player down
        JMP     CHECK_FOR_GOLD_PICKED_UP_BY_PLAYER

    .end:
        RTS

        ORG    $6C39
    DROP_PLAYER_IN_HOLE:
        SUBROUTINE

        LDA     #$00
        STA     DIG_DIRECTION       ; Stop digging

        LDY     PLAYER_ROW
        INY                         ; Move player down

        STX     GAME_COLNUM
        STY     GAME_ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDA     #SPRITE_EMPTY
        LDY     GAME_COLNUM
        STA     (PTR1),Y                ; Set blank sprite at player location in active page
        JSR     DRAW_SPRITE_PAGE1
        LDA     #SPRITE_EMPTY
        JSR     DRAW_SPRITE_PAGE2       ; Draw blank at player location on both graphics pages

        DEC     GAME_ROWNUM
        LDA     #SPRITE_EMPTY
        JSR     DRAW_SPRITE_PAGE1       ; Draw blank at location above player
        INC     GAME_ROWNUM
        LDX     #$FF

    .loop:
        INX
        CPX     #$1E
        BEQ     .end
        LDA     BRICK_FILL_TIMERS,X
        BNE     .loop

        LDA     GAME_ROWNUM
        STA     BRICK_DIG_ROWS,X
        LDA     GAME_COLNUM
        STA     BRICK_DIG_COLS,X
        LDA     #$B4
        STA     BRICK_FILL_TIMERS,X
        SEC

    .end:
        RTS

*/
#include "lode_runner.h"

/* The key commands, high-bit ASCII. */
#define KEY_UP          0xC9    /* 'I' */
#define KEY_LEFT        0xCA    /* 'J' */
#define KEY_DOWN        0xCB    /* 'K' */
#define KEY_RIGHT       0xCC    /* 'L' */
#define KEY_DIG_RIGHT   0xCF    /* 'O' */
#define KEY_DIG_LEFT    0xD5    /* 'U' */

/* The player's sprite for each PLAYER_ANIM_STATE. */
static const uint8_t SPRITE_ANIM_SEQS[18] = {
    0x0B, 0x0C, 0x0D,   /* 0x00: running left */
    0x18, 0x19, 0x1A,   /* 0x03: swinging on a rope, facing left */
    0x0F,               /* 0x06: digging left */
    0x13,               /* 0x07: falling, facing left */
    0x09, 0x10, 0x11,   /* 0x08: running right */
    0x15, 0x16, 0x17,   /* 0x0B: swinging on a rope, facing right */
    0x25,               /* 0x0E: digging right */
    0x14,               /* 0x0F: falling, facing right */
    0x0E, 0x12,         /* 0x10: climbing a ladder */
};

/* The original's five dig tables, one after another, with each name
 * the offset of its table. A dig's turn, DIG_ANIM_STATE less its first
 * value, indexes the bricks and the notes. The debris sprites are
 * indexed by DIG_ANIM_STATE itself, so a right dig, which starts at
 * 0x0C, reads its debris from DIG_DEBRIS_RIGHT_SPRITES. */
enum {
    DIG_DEBRIS_LEFT_SPRITES = 0,
    DIG_DEBRIS_RIGHT_SPRITES = 12,
    DIG_BRICK_SPRITES = 24,
    DIG_NOTE_PITCHES = 36,
    DIG_NOTE_DURATIONS = 49,
};
static const uint8_t dig_tables[62] = {
    /* The debris above the hole, left dig. */
    0x1B, 0x1B, 0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x00, 0x00, 0x00, 0x00,
    /* The debris above the hole, right dig. */
    0x26, 0x26, 0x27, 0x27, 0x1D, 0x1D, 0x1E, 0x1E, 0x00, 0x00, 0x00, 0x00,
    /* The brick being dug. */
    0x1F, 0x1F, 0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23, 0x24, 0x24,
    /* The pitch of each turn's note. */
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x24, 0x24, 0x24, 0x24,
    0x24,
    /* The duration of each turn's note. */
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x03, 0x03, 0x02, 0x02,
    0x01,
};

/* The tune of a piece of gold picked up, for load_sound_data. */
static const uint8_t gold_tune[] = {
    0x07, 0x45, 0x06, 0x55, 0x05, 0x44, 0x04, 0x54, 0x03, 0x43, 0x02, 0x53,
    0x00,
};

/* These return row `row` of LEVEL_MAP and of LEVEL_BG. The original
 * looks the row up in its tables CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,
 * _PAGES, and _PAGES2, and the compiler looks it up in a table of row
 * addresses too, so LEVEL_MAP[row][col] is cheap. These calls are
 * still smaller in most of this file, because a call and the two
 * stores of the address it returns take 7 bytes where the lookup
 * takes 10, and outline_runs shares many of the calls. Written as
 * plain indexing everywhere, this file grew by 24 bytes, so only the
 * three functions where plain indexing was smaller use it. */
static uint8_t *map_row(uint8_t row)
{
    return LEVEL_MAP[row];
}

static uint8_t *bg_row(uint8_t row)
{
    return LEVEL_BG[row];
}

/* Takes the player's sprite off page 1. */
static void erase_player(void)
{
    erase_sprite_at_pixel_coords(SPRITE_ANIM_SEQS[PLAYER_ANIM_STATE],
                                 half_screen_col_for(PLAYER_COL, PLAYER_X_ADJ),
                                 screen_row_for(PLAYER_ROW, PLAYER_Y_ADJ));
}

/* Draws the player's sprite on page 1. If it touches another sprite,
 * the player dies, except in a move that picked up gold. */
static void draw_player(void)
{
    uint8_t screens_differ = draw_sprite_at_pixel_coords(
        SPRITE_ANIM_SEQS[PLAYER_ANIM_STATE],
        half_screen_col_for(PLAYER_COL, PLAYER_X_ADJ),
        screen_row_for(PLAYER_ROW, PLAYER_Y_ADJ));
    if (screens_differ && DIDNT_PICK_UP_GOLD)
        ALIVE >>= 1;
}

/* Picks up the gold in the player's cell when the player is exactly on
 * it, taking it off both pages, scoring 250, and queuing the gold
 * tune. */
static void check_for_gold_picked_up_by_player(void)
{
    if (PLAYER_X_ADJ != 2 || PLAYER_Y_ADJ != 2)
        return;
    uint8_t *cell = &LEVEL_BG[PLAYER_ROW][PLAYER_COL];
    if (*cell != SPRITE_GOLD)
        return;
    DIDNT_PICK_UP_GOLD >>= 1;
    GOLD_COUNT--;
    GAME_ROWNUM = PLAYER_ROW;
    GAME_COLNUM = PLAYER_COL;
    *cell = SPRITE_EMPTY;
    draw_sprite_page2(SPRITE_EMPTY);
    erase_sprite_at_pixel_coords(SPRITE_GOLD, HALF_SCREEN_COL_TABLE[GAME_COLNUM],
                                 SCREEN_ROW_TABLE[GAME_ROWNUM]);
    add_and_update_score(0x0250);
    load_sound_data(gold_tune);
}

/* Steps PLAYER_ANIM_STATE through `first` + 1 to `last`, and sets it
 * to `first` when the step leaves that range. A player who changes
 * from one kind of move to another therefore starts the new sequence
 * at its beginning. */
static void inc_anim_state(uint8_t first, uint8_t last)
{
    PLAYER_ANIM_STATE++;
    if (PLAYER_ANIM_STATE <= first || PLAYER_ANIM_STATE > last)
        PLAYER_ANIM_STATE = first;
}

/* These move the player one step towards the center of the cell
 * across or down, and pick up gold if that puts the player on it. */
static void nudge_player_towards_exact_column(void)
{
    if (PLAYER_X_ADJ == 2)
        return;
    if (PLAYER_X_ADJ < 2)
        PLAYER_X_ADJ++;
    else
        PLAYER_X_ADJ--;
    check_for_gold_picked_up_by_player();
}

static void nudge_player_towards_exact_row(void)
{
    if (PLAYER_Y_ADJ == 2)
        return;
    if (PLAYER_Y_ADJ < 2)
        PLAYER_Y_ADJ++;
    else
        PLAYER_Y_ADJ--;
    check_for_gold_picked_up_by_player();
}

/* Puts back in LEVEL_MAP what the player's cell holds behind the
 * player. A brick there is a dug hole, which is empty in LEVEL_MAP. */
static void leave_cell(void)
{
    uint8_t behind = bg_row(PLAYER_ROW)[PLAYER_COL];
    map_row(PLAYER_ROW)[PLAYER_COL] =
        behind == SPRITE_BRICK ? SPRITE_EMPTY : behind;
}

/* Puts the player in LEVEL_MAP at the player's cell. */
static void enter_cell(void)
{
    map_row(PLAYER_ROW)[PLAYER_COL] = SPRITE_PLAYER;
}

/* Returns true for the sprites the player can't move into sideways or
 * climb up into. */
static bool blocks_player(uint8_t sprite)
{
    return sprite == SPRITE_BRICK || sprite == SPRITE_STONE ||
           sprite == SPRITE_TRAP;
}

/* Moves the player one step down, into the cell below after the fifth
 * step. */
static void step_down(void)
{
    if (++PLAYER_Y_ADJ < 5) {
        check_for_gold_picked_up_by_player();
        return;
    }
    PLAYER_Y_ADJ = 0;
    leave_cell();
    PLAYER_ROW++;
    enter_cell();
}

/* Animates the player running, or swinging if the player's cell has a
 * rope, from the running state `first`, 0 for left and 8 for right,
 * and draws the player. */
static void run_or_swing(uint8_t first)
{
    if (bg_row(PLAYER_ROW)[PLAYER_COL] == SPRITE_ROPE)
        inc_anim_state(first + 3, first + 5);
    else
        inc_anim_state(first, first + 2);
    draw_player();
}

/* Moves the player one step left, unless the player is at or left of
 * the cell's center and the cell to the left is off the level or
 * holds brick, stone, or a trap. */
static void try_moving_left(void)
{
    if (PLAYER_X_ADJ < 3) {
        if (PLAYER_COL == 0)
            return;
        if (blocks_player(LEVEL_MAP[PLAYER_ROW][PLAYER_COL - 1]))
            return;
    }
    erase_player();
    PLAYER_FACING_DIRECTION = 0xFF;
    nudge_player_towards_exact_row();
    if ((int8_t)--PLAYER_X_ADJ >= 0) {
        check_for_gold_picked_up_by_player();
    } else {
        leave_cell();
        PLAYER_COL--;
        enter_cell();
        PLAYER_X_ADJ = 4;
    }
    run_or_swing(0x00);
}

/* Moves the player one step right, unless the player is at or right of
 * the cell's center and the cell to the right is off the level or
 * holds brick, stone, or a trap. */
static void try_moving_right(void)
{
    if (PLAYER_X_ADJ >= 2) {
        if (PLAYER_COL == MAX_GAME_COL)
            return;
        if (blocks_player(LEVEL_MAP[PLAYER_ROW][PLAYER_COL + 1]))
            return;
    }
    erase_player();
    PLAYER_FACING_DIRECTION = 0x01;
    nudge_player_towards_exact_row();
    if (++PLAYER_X_ADJ < 5) {
        check_for_gold_picked_up_by_player();
    } else {
        leave_cell();
        PLAYER_COL++;
        enter_cell();
        PLAYER_X_ADJ = 0;
    }
    run_or_swing(0x08);
}

/* Animates the player climbing and draws the player. Returns false,
 * since the player moved. */
static bool climb(void)
{
    inc_anim_state(0x10, 0x11);
    draw_player();
    return false;
}

/* Moves the player one step up, when the player is on a ladder or just
 * above the top of one. Returns true if the player can't move up. */
static bool try_moving_up(void)
{
    if (bg_row(PLAYER_ROW)[PLAYER_COL] == SPRITE_LADDER) {
        if (PLAYER_Y_ADJ < 3) {
            if (PLAYER_ROW == 0)
                return true;
            if (blocks_player(map_row(PLAYER_ROW - 1)[PLAYER_COL]))
                return true;
        }
    } else {
        /* Above a ladder's top, the player can still climb as far as
         * the center of the cell. */
        if (PLAYER_Y_ADJ < 3)
            return true;
        if (bg_row(PLAYER_ROW + 1)[PLAYER_COL] != SPRITE_LADDER)
            return true;
    }
    erase_player();
    nudge_player_towards_exact_column();
    if ((int8_t)--PLAYER_Y_ADJ >= 0) {
        check_for_gold_picked_up_by_player();
    } else {
        leave_cell();
        PLAYER_ROW--;
        enter_cell();
        PLAYER_Y_ADJ = 4;
    }
    return climb();
}

/* Moves the player one step down, unless the player is at or below the
 * cell's center and the cell below is off the level or holds brick or
 * stone. Returns true if the player can't move down. */
static bool try_moving_down(void)
{
    if (PLAYER_Y_ADJ >= 2) {
        if (PLAYER_ROW >= MAX_GAME_ROW)
            return true;
        uint8_t below = map_row(PLAYER_ROW + 1)[PLAYER_COL];
        if (below == SPRITE_STONE || below == SPRITE_BRICK)
            return true;
    }
    erase_player();
    nudge_player_towards_exact_column();
    step_down();
    return climb();
}

/* Empties the dug cell at `col` in the row below the player on both
 * pages, and the cell above it on page 1, and starts a timer to fill
 * the hole in again. */
static void drop_player_in_hole(uint8_t col)
{
    DIG_DIRECTION = 0;
    uint8_t row = PLAYER_ROW + 1;
    GAME_COLNUM = col;
    GAME_ROWNUM = row;
    map_row(row)[col] = SPRITE_EMPTY;
    draw_sprite_page1(SPRITE_EMPTY);
    draw_sprite_page2(SPRITE_EMPTY);
    GAME_ROWNUM--;
    draw_sprite_page1(SPRITE_EMPTY);
    GAME_ROWNUM++;
    /* As in the original, only entries 0-29 are searched. */
    for (uint8_t i = 0; i != 30; i++) {
        if (BRICK_FILL_TIMERS[i] == 0) {
            BRICK_DIG_ROWS[i] = GAME_ROWNUM;
            BRICK_DIG_COLS[i] = GAME_COLNUM;
            BRICK_FILL_TIMERS[i] = 0xB4;
            return;
        }
    }
}

static bool stop_digging(void)
{
    DIG_DIRECTION = 0;
    return true;
}

/* Takes the dig in DIG_DIRECTION one turn further, if the brick below
 * and beside the player is still there with nothing above it.
 * Returns false while the dig goes on, and true when it stops or ends
 * with the hole dug. */
static bool keep_digging(void)
{
    bool right = DIG_DIRECTION < 0x80;
    /* DIG_ANIM_STATE's first and last values. The first indexes the
     * start of the bricks and the notes. */
    uint8_t first = right ? 0x0C : 0x00;
    uint8_t last = first + 12;
    uint8_t row = PLAYER_ROW;
    uint8_t col;

    if (row >= MAX_GAME_ROW)
        return stop_digging();
    if (right) {
        if (PLAYER_COL >= MAX_GAME_COL)
            return stop_digging();
        col = PLAYER_COL + 1;
    } else {
        if (PLAYER_COL == 0)
            return stop_digging();
        col = PLAYER_COL - 1;
    }
    if (map_row(row + 1)[col] != SPRITE_BRICK)
        return stop_digging();

    uint8_t half_col = HALF_SCREEN_COL_TABLE[col];
    uint8_t screen_row = SCREEN_ROW_TABLE[row];

    if (map_row(row)[col] != SPRITE_EMPTY) {
        /* Something moved over the hole. Put the brick back whole, and
         * take away the last turn's debris. */
        GAME_ROWNUM = row + 1;
        GAME_COLNUM = col;
        draw_sprite_page1(SPRITE_BRICK);
        if (DIG_ANIM_STATE != first)
            erase_sprite_at_pixel_coords(
                dig_tables[DIG_DEBRIS_LEFT_SPRITES + DIG_ANIM_STATE - 1],
                half_col, screen_row);
        return stop_digging();
    }

    erase_player();
    nudge_player_towards_exact_column();
    nudge_player_towards_exact_row();
    uint8_t state = DIG_ANIM_STATE;
    uint8_t turn = state - first;
    append_note(dig_tables[DIG_NOTE_PITCHES + turn],
                dig_tables[DIG_NOTE_DURATIONS + turn]);
    /* The player digs for the first six turns and then stands as if
     * running. */
    if (right)
        PLAYER_ANIM_STATE = state < 0x12 ? 0x0E : 0x08;
    else
        PLAYER_ANIM_STATE = state < 0x06 ? 0x06 : 0x00;
    draw_player();

    if (state == last) {
        drop_player_in_hole(col);
        return true;
    }
    if (state != first)
        erase_sprite_at_pixel_coords(
            dig_tables[DIG_DEBRIS_LEFT_SPRITES + state - 1], half_col,
            screen_row);
    GAME_COLNUM = col;
    GAME_ROWNUM = row;
    draw_sprite_at_pixel_coords(dig_tables[DIG_DEBRIS_LEFT_SPRITES + state],
                                half_col, screen_row);
    GAME_ROWNUM++;
    draw_sprite_page1(dig_tables[DIG_BRICK_SPRITES + turn]);
    DIG_ANIM_STATE++;
    return false;
}

/* Starts a dig to the left (`direction` 0xFF) or right (0x01). Returns
 * true if the player can't dig there. */
static bool start_digging(uint8_t direction)
{
    DIG_DIRECTION = direction;
    KEY_COMMAND = direction;
    KEY_COMMAND_LR = direction;
    DIG_ANIM_STATE = direction == 0x01 ? 0x0C : 0x00;
    return keep_digging();
}

/* Falls one step, which may take the player into the cell below. */
static void fall(void)
{
    NOT_FALLING = 0;
    erase_player();
    PLAYER_ANIM_STATE = (PLAYER_FACING_DIRECTION & 0x80) ? 0x07 : 0x0F;
    nudge_player_towards_exact_column();
    step_down();
    draw_player();
}

/* Returns true if the player stands on a guard, brick, stone, or a
 * ladder, or on the bottom row. */
static bool stands_on_something(void)
{
    if (PLAYER_ROW == MAX_GAME_ROW)
        return true;
    uint8_t below = map_row(PLAYER_ROW + 1)[PLAYER_COL];
    if (below == SPRITE_EMPTY)
        return false;
    if (below == SPRITE_GUARD)
        return true;
    below = bg_row(PLAYER_ROW + 1)[PLAYER_COL];
    return below == SPRITE_BRICK || below == SPRITE_STONE ||
           below == SPRITE_LADDER;
}

/* Moves the player for this turn. A dig under way goes on, a player
 * with nothing underneath falls, and otherwise the player moves as the
 * key command says. */
void move_player(void)
{
    DIDNT_PICK_UP_GOLD = 1;
    if (DIG_DIRECTION != 0) {
        keep_digging();
        return;
    }

    /* The landing note's first pass count is the Y the original
     * leaves, which is the player's column except when the player
     * stands on the bottom row off a ladder or rope, where it's the
     * row. */
    uint8_t note_y = PLAYER_COL;
    uint8_t here = bg_row(PLAYER_ROW)[PLAYER_COL];
    if (here != SPRITE_LADDER && (here != SPRITE_ROPE || PLAYER_Y_ADJ != 2)) {
        if (PLAYER_Y_ADJ < 2 || !stands_on_something()) {
            fall();
            return;
        }
        if (PLAYER_ROW == MAX_GAME_ROW)
            note_y = MAX_GAME_ROW;
    }

    if (NOT_FALLING == 0)
        play_note(0x64, 0x08, note_y);
    FALL_PITCH = 0x20;
    NOT_FALLING = 0x20;
    check_for_input();

    /* A move up or down or a dig that can't be made gives way to the
     * move left or right. A dig sets KEY_COMMAND_LR to its direction,
     * so a dig that can't be made leaves the player still. */
    switch (KEY_COMMAND) {
    case KEY_UP:
        if (!try_moving_up())
            return;
        break;
    case KEY_DOWN:
        if (!try_moving_down())
            return;
        break;
    case KEY_DIG_LEFT:
        if (!start_digging(0xFF))
            return;
        break;
    case KEY_DIG_RIGHT:
        if (!start_digging(0x01))
            return;
        break;
    }
    if (KEY_COMMAND_LR == KEY_LEFT)
        try_moving_left();
    else if (KEY_COMMAND_LR == KEY_RIGHT)
        try_moving_right();
}
