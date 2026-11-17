/*
    guards.c -- moving the guards, the AI routines that compute each
    guard's move, and the gold that guards pick up, carry, and drop.

    MOVE_GUARDS moves the guards whose bits are set in the turn's
    pattern. MOVE_GUARD copies the next guard's state into GUARD_LOC_COL
    and the rest of the zero-page copy, makes it fall, shake its way
    out of a hole, or take the move DETERMINE_GUARD_MOVE returns, and
    copies the state back. A guard's position is a level cell plus a
    0-4 adjustment in each direction, 2 being exactly on the cell, and
    a guard enters the next cell when the adjustment runs past 0 or 4.

    A guard on the player's row runs straight at the player when
    nothing between them would make it fall. Otherwise
    DETERMINE_GUARD_MOVE examines every column the guard can reach
    along its row without falling, and the row the guard would end up
    on by climbing a ladder up or dropping down from each one. It scores
    each of those rows with PSEUDO_DISTANCE and returns the move toward
    the lowest score.

    Departures from the original:

    - Every path through MOVE_GUARD, the TRY_GUARD_MOVE_* routines, and
      GUARD_DO_NOTHING returns to move_guard, which calls
      store_guard_data once. The original ends each path with a jump
      to STORE_GUARD_DATA. GUARD_FN_TABLE's dispatch is a switch.
    - GET_GUARD_SPRITE_AND_COORDS is erase_guard, draw_guard, and
      draw_guard_sprite, which compute the sprite and its coordinates
      where they're passed.
    - DETERMINE_GUARD_LEFT_RIGHT_LIMITS is guard_left_col_limit and
      guard_right_col_limit, which return the two limits. They're
      computed after the guard's own column has been scored, where the
      original computes them before, but they only read the level.
    - SHOULD_GUARD_MOVE_UP_OR_DOWN and the body of the loops in
      SHOULD_GUARD_MOVE_LEFT and SHOULD_GUARD_MOVE_RIGHT are one
      function, consider_column, since they differ only in the moves
      they score.
    - The AI's scratch bytes are parameters, locals, and return values.
      GUARD_ACTION and BEST_GUARD_DIST, which every scored column
      updates, are the static guard_action and best_guard_dist.
    - GUARD_FIND_CANDIDATE_ROW_BELOW returns 15 when its search reaches
      row 15. The original goes on to examine row 15, and reads the
      row below it through CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+16 and
      ..._PAGES2+16, which are the bytes after those tables and give
      the address $0008, so it reads zero page. Every path through the
      loop at row 15 returns 15 whatever those bytes hold.
    - GUARD_PATTERNS_LIST is defined here because its lines are in this
      unit, but only main.c reads it.

    Two quirks of the original are kept.

    - When the guard is in the player's cell, the AI's run to the left
      at the player goes past column 0 and wraps around through 255,
      reading the bytes after the guard's row as in the original.
    - In GUARD_FIND_CANDIDATE_ROW_BELOW, the test for a rope to the
      right of the column reads the row below when the test of the
      cell below and to the left has run, because the original's PTR2
      still points there.

    Lode Runner, main.asm lines 2611-4317:

        ORG    $6C82
    MOVE_GUARDS:
        SUBROUTINE

        LDX     GUARD_COUNT
        BEQ     .end

        ; Increment GUARD_PHASE mod 3
        INC     GUARD_PHASE
        LDY     GUARD_PHASE
        CPY     #$03
        BCC     .incremented_phase
        LDY     #$00
        STY     GUARD_PHASE
    .incremented_phase:

        LDA     GUARD_PATTERNS,Y
        STA     GUARD_PATTERN

    .loop:
        LSR     GUARD_PATTERN       ; Peel off the lsb
        BCC     .bit_done
        JSR     MOVE_GUARD          ; Move a guard
        LDA     ALIVE
        BEQ     .end                ; If player is dead, end.

    .bit_done:
        LDA     GUARD_PATTERN
        BNE     .loop

    .end:
        RTS

        ORG    $6CA7
    GUARD_PATTERNS_LIST:
        HEX     00 00 00
        HEX     00 01 01
        HEX     01 01 01
        HEX     01 03 01
        HEX     01 03 03
        HEX     03 03 03
        HEX     03 03 07
        HEX     03 07 07
        HEX     07 07 07
        HEX     07 07 0F
        HEX     07 0F 0F
        HEX     0F 0F 0F
        ORG    $6CCB
    GUARD_ANIM_SPRITES:
        HEX     08 2B 2C        ; running left
        HEX     30 31 32        ; monkey-traversing left
        HEX     36              ; falling left
        HEX     28 29 2A        ; running right
        HEX     2D 2E 2F        ; monkey-traversing right
        HEX     35              ; falling right
        HEX     33 34           ; climbing
        ORG    $6CDB
    MOVE_GUARD
        SUBROUTINE

        ; Increment GUARD_NUM mod GUARD_COUNT, except 1-based.
        INC     GUARD_NUM
        LDX     GUARD_COUNT
        CPX     GUARD_NUM
        BCS     .guard_num_incremented
        LDX     #$01
        STX     GUARD_NUM
    .guard_num_incremented:

        JSR     LOAD_GUARD_DATA
        LDA     GUARD_GOLD_TIMER
        BMI     .check_sprite_at_guard_pos
        BEQ     .check_sprite_at_guard_pos

        ; GUARD_GOLD_TIMER > 0:
        DEC     GUARD_GOLD_TIMER
        LDY     GUARD_GOLD_TIMER
        CPY     #$0D
        BCS     .guard_gold_timer_expired

        ; GUARD_GOLD_TIMER < 12
        JMP     .check_gold_timer

    .guard_gold_timer_expired:
        LDX     GUARD_NUM
        LDA     GUARD_RESURRECTION_TIMERS,X
        BEQ     .resurrect_guard_
        JMP     STORE_GUARD_DATA            ; tailcall

    .resurrect_guard_:
        JMP     .resurrect_guard

    .check_sprite_at_guard_pos:
        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GUARD_LOC_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_LADDER
        BEQ     .ladder_
        CMP     #SPRITE_ROPE
        BNE     .not_rope_or_ladder
        LDA     GUARD_Y_ADJ
        CMP     #$02
        BEQ     .ladder_

    .not_rope_or_ladder:
        LDA     GUARD_Y_ADJ
        CMP     #$02
        BCC     .blank_or_player           ; if GUARD_Y_ADJ < 2
        LDY     GUARD_LOC_ROW
        CPY     #MAX_GAME_ROW
        BEQ     .ladder_          ; Row == 15
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES+1,Y
        STA     PTR1+1

        LDY     GUARD_LOC_COL
        LDA     (PTR1),Y

        CMP     #SPRITE_EMPTY
        BEQ     .blank_or_player
        CMP     #SPRITE_PLAYER
        BEQ     .blank_or_player
        CMP     #SPRITE_GUARD
        BEQ     .ladder_

        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BEQ     .ladder_
        CMP     #SPRITE_STONE
        BEQ     .ladder_
        CMP     #SPRITE_LADDER
        BNE     .blank_or_player

    .ladder_:
        JMP     .ladder

    .blank_or_player:
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        JSR     NUDGE_GUARD_TOWARDS_EXACT_COLUMN
        LDA     #$06
        LDY     GUARD_FACING_DIRECTION
        BMI     .set_guard_anim_state
        LDA     #$0D
    .set_guard_anim_state
        STA     GUARD_ANIM_STATE

        INC     GUARD_Y_ADJ
        LDA     GUARD_Y_ADJ
        CMP     #$05
        BCS     $6dc0           ; If GUARD_Y_ADJ > 4

        LDA     GUARD_Y_ADJ
        CMP     #$02
        BNE     .resurrect_guard

        JSR     CHECK_FOR_GOLD_PICKED_UP_BY_GUARD
        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GUARD_LOC_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BNE     .resurrect_guard

        LDA     GUARD_GOLD_TIMER
        BPL     .reset_gold_timer
        DEC     GOLD_COUNT
    .reset_gold_timer:
        LDA     GUARD_GOLD_TIMER_START_VALUE
        STA     GUARD_GOLD_TIMER
        LDY     #$00
        LDA     #$75
        JSR     ADD_AND_UPDATE_SCORE        ; SCORE += 75

        ; Play the guard kill tune
        JSR     LOAD_SOUND_DATA
        HEX     06 20 04 30 02 40 00

    .resurrect_guard:
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS
        JMP     STORE_GUARD_DATA            ; tailcall

    .6dc0:
        LDA     #$00
        STA     GUARD_Y_ADJ                 ; set vertical adjust to -2
        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GUARD_LOC_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BNE     .set_real_sprite
        LDA     #SPRITE_EMPTY
    .set_real_sprite:
        STA     (PTR1),Y

        INC     GUARD_LOC_ROW               ; move guard down
        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GUARD_LOC_COL
        LDA     (PTR1),Y
        CMP     #SPRITE_PLAYER
        BNE     .get_background_sprite
        LSR     ALIVE                       ; set player to dead
    .get_background_sprite:
        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BNE     .place_guard_at_loc
        LDA     GUARD_GOLD_TIMER
        BPL     .place_guard_at_loc

        ; What's above the guard?
        LDY     GUARD_LOC_ROW
        DEY
        STY     GAME_ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GUARD_LOC_COL
        STY     GAME_COLNUM
        LDA     (PTR2),Y
        CMP     #SPRITE_EMPTY
        BEQ     .drop_gold
        DEC     GOLD_COUNT
        JMP     .6e46

    .drop_gold:
        LDA     #SPRITE_GOLD
        STA     (PTR1),Y
        STA     (PTR2),Y
        JSR     DRAW_SPRITE_PAGE2
        LDY     GAME_ROWNUM
        LDX     GAME_COLNUM
        JSR     GET_SCREEN_COORDS_FOR
        LDA     #SPRITE_GOLD
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS

    .6e46
        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDA     #$00
        STA     GUARD_GOLD_TIMER
        LDY     GUARD_LOC_COL

    .place_guard_at_loc
        LDA     #SPRITE_GUARD
        STA     (PTR1),Y

        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS
        JMP     STORE_GUARD_DATA            ; tailcall

    .check_gold_timer:
        CPY     #$07
        BCC     .ladder

        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        LDY     GUARD_GOLD_TIMER
        LDA     GUARD_X_ADJ_TABLE-7,Y       ; GUARD_X_ADJ_TABLE[GUARD_GOLD_TIMER-7]
        STA     GUARD_X_ADJ
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS
        JMP     STORE_GUARD_DATA            ; tailcall

        ORG    $6E7F
    GUARD_X_ADJ_TABLE:
        HEX     02 01 02 03 02 01
        ORG    $6E85
    .ladder
        LDX     GUARD_LOC_COL
        LDY     GUARD_LOC_ROW
        JSR     DETERMINE_GUARD_MOVE

        ; Go to a guard movement function in the GUARD_FN_TABLE
        ASL
        TAY
        LDA     GUARD_FN_TABLE+1,Y
        PHA
        LDA     GUARD_FN_TABLE,Y
        PHA
        RTS

        ORG    $6E97
    GUARD_FN_TABLE:
        WORD    STORE_GUARD_DATA-1
        WORD    TRY_GUARD_MOVE_LEFT-1
        WORD    TRY_GUARD_MOVE_RIGHT-1
        WORD    TRY_GUARD_MOVE_UP-1
        WORD    TRY_GUARD_MOVE_DOWN-1
        ORG    $6EA1
    GUARD_DO_NOTHING:
        SUBROUTINE

        ; if GUARD_GOLD_TIMER > 0, GUARD_GOLD_TIMER++
        LDA     GUARD_GOLD_TIMER
        BEQ     .store_guard_data
        BMI     .store_guard_data
        INC     GUARD_GOLD_TIMER
    .store_guard_data:
        JMP     STORE_GUARD_DATA

        ORG    $6EAC
    TRY_GUARD_MOVE_UP:
        SUBROUTINE

        LDY     GUARD_Y_ADJ
        CPY     #$03
        BCS     .move_up        ; vertical adjustment > 0

        LDY     GUARD_LOC_ROW
        BEQ     GUARD_DO_NOTHING
        DEY
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     GUARD_LOC_COL
        LDA     (PTR1),Y

        CMP     #SPRITE_BRICK
        BEQ     GUARD_DO_NOTHING
        CMP     #SPRITE_STONE
        BEQ     GUARD_DO_NOTHING
        CMP     #SPRITE_TRAP
        BEQ     GUARD_DO_NOTHING
        CMP     #SPRITE_GUARD
        BEQ     GUARD_DO_NOTHING

    .move_up:
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        JSR     NUDGE_GUARD_TOWARDS_EXACT_COLUMN
        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        DEC     GUARD_Y_ADJ
        BPL     TRY_GUARD_MOVE_UP_check_for_gold

        ; vertical adjustment underflow
        JSR     GUARD_DROP_GOLD
        LDY     GUARD_LOC_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BNE     .set_active_sprite
        LDA     #SPRITE_EMPTY
    .set_active_sprite:
        STA     (PTR1),Y

        DEC     GUARD_LOC_ROW
        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     GUARD_LOC_COL
        LDA     (PTR1),Y

        CMP     #SPRITE_PLAYER
        BNE     .set_guard_sprite

        ; kill player
        LSR     ALIVE

    .set_guard_sprite:
        LDA     #SPRITE_GUARD
        STA     (PTR1),Y

        LDA     #$04
        STA     GUARD_Y_ADJ         ; vertical adjust = +2
        BNE     TRY_GUARD_MOVE_UP_inc_anim_state     ; unconditional

    TRY_GUARD_MOVE_UP_check_for_gold:
        JSR     CHECK_FOR_GOLD_PICKED_UP_BY_GUARD

    TRY_GUARD_MOVE_UP_inc_anim_state:
        LDA     #$0E
        LDX     #$0F
        JSR     INC_GUARD_ANIM_STATE
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS
        JMP     STORE_GUARD_DATA

        ORG    $6F39
    TRY_GUARD_MOVE_DOWN:
        SUBROUTINE

        LDY     GUARD_Y_ADJ
        CPY     #$02
        BCC     .move_down        ; vertical adjustment < 0

        LDY     GUARD_LOC_ROW
        CPY     #MAX_GAME_ROW
        BCS     .store_guard_data
        INY
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     GUARD_LOC_COL
        LDA     (PTR1),Y

        CMP     #SPRITE_STONE
        BEQ     .store_guard_data
        CMP     #SPRITE_GUARD
        BEQ     .store_guard_data
        CMP     #SPRITE_BRICK
        BNE     .move_down

    .store_guard_data:
        JMP     STORE_GUARD_DATA

    .move_down:
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        JSR     NUDGE_GUARD_TOWARDS_EXACT_COLUMN
        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        INC     GUARD_Y_ADJ
        LDA     GUARD_Y_ADJ
        CMP     #$05
        BCC     .check_for_gold

        ; vertical adjustment overflow
        JSR     GUARD_DROP_GOLD
        LDY     GUARD_LOC_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BNE     .set_active_sprite
        LDA     #SPRITE_EMPTY
    .set_active_sprite:
        STA     (PTR1),Y

        INC     GUARD_LOC_ROW
        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     GUARD_LOC_COL
        LDA     (PTR1),Y

        CMP     #SPRITE_PLAYER
        BNE     .set_guard_sprite

        ; kill player
        LSR     ALIVE

    .set_guard_sprite:
        LDA     #SPRITE_GUARD
        STA     (PTR1),Y

        LDA     #$00
        STA     GUARD_Y_ADJ         ; vertical adjust = -2
        JMP     TRY_GUARD_MOVE_UP_inc_anim_state

    .check_for_gold:
        JMP     TRY_GUARD_MOVE_UP_check_for_gold

        ORG    $6FBC
    TRY_GUARD_MOVE_LEFT:
        SUBROUTINE

        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDX     GUARD_X_ADJ
        CPX     #$03
        BCS     .move_left           ; horizontal adjustment > 0

        ; horizontal adjustment <= 0
        LDY     GUARD_LOC_COL
        BEQ     .store_guard_data       ; Can't go any more left

        DEY
        LDA     (PTR1),Y
        CMP     #SPRITE_GUARD
        BEQ     .store_guard_data
        CMP     #SPRITE_STONE
        BEQ     .store_guard_data
        CMP     #SPRITE_BRICK
        BEQ     .store_guard_data

        LDA     (PTR2),Y
        CMP     #SPRITE_TRAP
        BNE     .move_left

    .store_guard_data:
        JMP     STORE_GUARD_DATA        ; tailcall

    .move_left:
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        JSR     NUDGE_GUARD_TOWARDS_EXACT_ROW
        LDA     #$FF
        STA     GUARD_FACING_DIRECTION      ; face left
        DEC     GUARD_X_ADJ
        BPL     .check_for_gold_pickup

        ; horizontal adjustment underflow
        JSR     GUARD_DROP_GOLD
        LDY     GUARD_LOC_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BNE     .store_sprite
        LDA     #SPRITE_EMPTY

    .store_sprite:
        STA     (PTR1),Y

        DEC     GUARD_LOC_COL
        DEY
        LDA     (PTR1),Y
        CMP     #SPRITE_PLAYER
        BNE     .place_guard_sprite

        ; kill player
        LSR     ALIVE

    .place_guard_sprite:
        LDA     #SPRITE_GUARD
        STA     (PTR1),Y

        LDA     #$04
        STA     GUARD_X_ADJ     ; horizontal adjustment = +2
        BNE     .determine_anim_set           ; unconditional

    .check_for_gold_pickup:
        JSR     CHECK_FOR_GOLD_PICKED_UP_BY_GUARD

    .determine_anim_set:
        LDY     GUARD_LOC_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_ROPE
        BEQ     .rope

        LDA     #$00
        LDX     #$02
        BNE     .inc_anim_state

    .rope:
        LDA     #$03
        LDX     #$05

    .inc_anim_state:
        JSR     INC_GUARD_ANIM_STATE
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS
        JMP     STORE_GUARD_DATA                ; tailcall

        ORG    $7047
    TRY_GUARD_MOVE_RIGHT:
        SUBROUTINE

        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDX     GUARD_X_ADJ
        CPX     #$02
        BCC     .move_right           ; horizontal adjustment < 0

        ; horizontal adjustment >= 0
        LDY     GUARD_LOC_COL
        CPY     #MAX_GAME_COL
        BEQ     .store_guard_data       ; Can't go any more right

        INY
        LDA     (PTR1),Y
        CMP     #SPRITE_GUARD
        BEQ     .store_guard_data
        CMP     #SPRITE_STONE
        BEQ     .store_guard_data
        CMP     #SPRITE_BRICK
        BEQ     .store_guard_data

        LDA     (PTR2),Y
        CMP     #SPRITE_TRAP
        BNE     .move_right

    .store_guard_data:
        JMP     STORE_GUARD_DATA        ; tailcall

    .move_right:
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     ERASE_SPRITE_AT_PIXEL_COORDS
        JSR     NUDGE_GUARD_TOWARDS_EXACT_ROW
        LDA     #$01
        STA     GUARD_FACING_DIRECTION      ; face right
        INC     GUARD_X_ADJ
        LDA     GUARD_X_ADJ
        CMP     #$05
        BCC     .check_for_gold_pickup

        ; horizontal adjustment overflow
        JSR     GUARD_DROP_GOLD
        LDY     GUARD_LOC_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_BRICK
        BNE     .store_sprite
        LDA     #SPRITE_EMPTY

    .store_sprite:
        STA     (PTR1),Y

        INC     GUARD_LOC_COL
        INY
        LDA     (PTR1),Y
        CMP     #SPRITE_PLAYER
        BNE     .place_guard_sprite

        ; kill player
        LSR     ALIVE

    .place_guard_sprite:
        LDA     #SPRITE_GUARD
        STA     (PTR1),Y

        LDA     #$00
        STA     GUARD_X_ADJ     ; horizontal adjustment = -2
        BEQ     .determine_anim_set           ; unconditional

    .check_for_gold_pickup:
        JSR     CHECK_FOR_GOLD_PICKED_UP_BY_GUARD

    .determine_anim_set:
        LDY     GUARD_LOC_COL
        LDA     (PTR2),Y
        CMP     #SPRITE_ROPE
        BEQ     .rope

        LDA     #$07
        LDX     #$09
        BNE     .inc_anim_state

    .rope:
        LDA     #$0A
        LDX     #$0C

    .inc_anim_state:
        JSR     INC_GUARD_ANIM_STATE
        JSR     GET_GUARD_SPRITE_AND_COORDS
        JSR     DRAW_SPRITE_AT_PIXEL_COORDS
        JMP     STORE_GUARD_DATA                ; tailcall

        ORG    $70D8
    DETERMINE_GUARD_MOVE:
        SUBROUTINE

        STX     TMP_GUARD_COL
        STY     TMP_GUARD_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     TMP_GUARD_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BNE     .end_if_row_is_not_player_row
        LDA     GUARD_GOLD_TIMER
        BEQ     .end_if_row_is_not_player_row
        BMI     .end_if_row_is_not_player_row
        LDA     #GUARD_ACTION_MOVE_UP
        RTS

    .end_if_row_is_not_player_row:
        LDY     TMP_GUARD_ROW
        CPY     PLAYER_ROW
        BEQ     .7100
        JMP     .end

    .7100:
        LDY     TMP_GUARD_COL
        STY     $57
        CPY     PLAYER_COL
        BCS     .loop2
        ; If TMP_GUARD_COL < PLAYER_COL:

    .loop:
        INC     $57
        LDY     TMP_GUARD_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     $57
        LDA     (PTR2),Y

        CMP     #SPRITE_LADDER
        BEQ     .is_ladder_or_rope
        CMP     #SPRITE_ROPE
        BEQ     .is_ladder_or_rope

        LDY     TMP_GUARD_ROW
        CPY     #MAX_GAME_ROW
        BEQ     .is_ladder_or_rope

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     $57
        LDA     (PTR2),Y
        CMP     #SPRITE_EMPTY
        BEQ     .end
        CMP     #SPRITE_TRAP
        BEQ     .end

    .is_ladder_or_rope:
        LDY     $57
        CPY     PLAYER_COL
        BNE     .loop

        ; PLAYER_COL == $57:
        LDA     #GUARD_ACTION_MOVE_RIGHT
        RTS

    .loop2:
        DEC     $57
        LDY     TMP_GUARD_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     $57
        LDA     (PTR2),Y

        CMP     #SPRITE_LADDER
        BEQ     .is_ladder_or_rope2
        CMP     #SPRITE_ROPE
        BEQ     .is_ladder_or_rope2

        LDY     TMP_GUARD_ROW
        CPY     #MAX_GAME_ROW
        BEQ     .is_ladder_or_rope2

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     $57
        LDA     (PTR2),Y
        CMP     #SPRITE_EMPTY
        BEQ     .end
        CMP     #SPRITE_TRAP
        BEQ     .end

    .is_ladder_or_rope2:
        LDY     $57
        CPY     PLAYER_COL
        BNE     .loop2

        ; PLAYER_COL == $57:
        LDA     #GUARD_ACTION_MOVE_LEFT
        RTS

    .end:
        LDA     #GUARD_ACTION_DO_NOTHING
        STA     GUARD_ACTION
        LDA     #$FF
        STA     BEST_GUARD_DIST
        LDX     TMP_GUARD_COL
        LDY     TMP_GUARD_ROW
        JSR     DETERMINE_GUARD_LEFT_RIGHT_LIMITS
        JSR     SHOULD_GUARD_MOVE_UP_OR_DOWN
        JSR     SHOULD_GUARD_MOVE_LEFT
        JSR     SHOULD_GUARD_MOVE_RIGHT
        LDA     GUARD_ACTION
        RTS

        ORG    $71A1
        SUBROUTINE
    .return:
        RTS

    SHOULD_GUARD_MOVE_LEFT:
        LDY     GUARD_LEFT_COL_LIMIT
        CPY     TMP_GUARD_COL
        BEQ     .return

        LDY     TMP_GUARD_ROW
        CPY     #MAX_GAME_ROW
        BEQ     .check_here

        ; Check below:

        ; Get background sprite at TMP_GUARD_ROW + 1, col = GUARD_LEFT_COL_LIMIT
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     GUARD_LEFT_COL_LIMIT
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BEQ     .check_here
        CMP     #SPRITE_STONE
        BEQ     .check_here

        LDX     GUARD_LEFT_COL_LIMIT
        LDY     TMP_GUARD_ROW
        JSR     GUARD_FIND_CANDIDATE_ROW_BELOW

        LDX     GUARD_LEFT_COL_LIMIT
        JSR     PSEUDO_DISTANCE
        CMP     BEST_GUARD_DIST
        BCS     .check_here        ; dist >= BEST_GUARD_DIST?

        ; dist < BEST_GUARD_DIST
        STA     BEST_GUARD_DIST     ; dist
        LDA     #GUARD_ACTION_MOVE_LEFT
        STA     GUARD_ACTION

    .check_here:
        LDY     TMP_GUARD_ROW
        BEQ     .next

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GUARD_LEFT_COL_LIMIT
        LDA     (PTR2),Y

        CMP     #SPRITE_LADDER
        BNE     .next

        ; Ladder here
        LDY     TMP_GUARD_ROW
        LDX     GUARD_LEFT_COL_LIMIT
        JSR     GUARD_FIND_CANDIDATE_ROW_ABOVE

        LDX     GUARD_LEFT_COL_LIMIT
        JSR     PSEUDO_DISTANCE
        CMP     BEST_GUARD_DIST
        BCS     .next        ; dist >= BEST_GUARD_DIST?

        ; dist < BEST_GUARD_DIST
        STA     BEST_GUARD_DIST     ; dist
        LDA     #GUARD_ACTION_MOVE_LEFT
        STA     GUARD_ACTION

    .next:
        INC     GUARD_LEFT_COL_LIMIT
        JMP     SHOULD_GUARD_MOVE_LEFT

        ORG    $720B
        SUBROUTINE
    .return:
        RTS

    SHOULD_GUARD_MOVE_RIGHT:
        LDY     GUARD_RIGHT_COL_LIMIT
        CPY     TMP_GUARD_COL
        BEQ     .return

        LDY     TMP_GUARD_ROW
        CPY     #MAX_GAME_ROW
        BEQ     .check_here

        ; Check below:

        ; Get background sprite at TMP_GUARD_ROW + 1, col = GUARD_RIGHT_COL_LIMIT
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     GUARD_RIGHT_COL_LIMIT
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BEQ     .check_here
        CMP     #SPRITE_STONE
        BEQ     .check_here

        LDX     GUARD_RIGHT_COL_LIMIT
        LDY     TMP_GUARD_ROW
        JSR     GUARD_FIND_CANDIDATE_ROW_BELOW

        LDX     GUARD_RIGHT_COL_LIMIT
        JSR     PSEUDO_DISTANCE
        CMP     BEST_GUARD_DIST
        BCS     .check_here        ; dist >= BEST_GUARD_DIST?

        ; dist < BEST_GUARD_DIST
        STA     BEST_GUARD_DIST     ; dist
        LDA     #GUARD_ACTION_MOVE_RIGHT
        STA     GUARD_ACTION

    .check_here:
        LDY     TMP_GUARD_ROW
        BEQ     .next

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GUARD_RIGHT_COL_LIMIT
        LDA     (PTR2),Y

        CMP     #SPRITE_LADDER
        BNE     .next

        ; Ladder here
        LDY     TMP_GUARD_ROW
        LDX     GUARD_RIGHT_COL_LIMIT
        JSR     GUARD_FIND_CANDIDATE_ROW_ABOVE

        LDX     GUARD_RIGHT_COL_LIMIT
        JSR     PSEUDO_DISTANCE
        CMP     BEST_GUARD_DIST
        BCS     .next        ; dist >= BEST_GUARD_DIST?

        ; dist < BEST_GUARD_DIST
        STA     BEST_GUARD_DIST     ; dist
        LDA     #GUARD_ACTION_MOVE_RIGHT
        STA     GUARD_ACTION

    .next:
        DEC     GUARD_RIGHT_COL_LIMIT
        JMP     SHOULD_GUARD_MOVE_RIGHT

        ORG    $7275
    SHOULD_GUARD_MOVE_UP_OR_DOWN:
        SUBROUTINE

        LDY     TMP_GUARD_ROW
        CPY     #MAX_GAME_ROW
        BEQ     .should_guard_move_up

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     TMP_GUARD_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BEQ     .should_guard_move_up
        CMP     #SPRITE_STONE
        BEQ     .should_guard_move_up

        LDX     TMP_GUARD_COL
        LDY     TMP_GUARD_ROW
        JSR     GUARD_FIND_CANDIDATE_ROW_BELOW
        LDX     TMP_GUARD_COL
        JSR     PSEUDO_DISTANCE
        CMP     BEST_GUARD_DIST
        BCS     .should_guard_move_up
        STA     BEST_GUARD_DIST
        LDA     #GUARD_ACTION_MOVE_DOWN
        STA     GUARD_ACTION

    .should_guard_move_up:
        LDY     TMP_GUARD_ROW
        BEQ     .end

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     TMP_GUARD_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_LADDER
        BNE     .end
        ; ladder

        LDX     TMP_GUARD_COL
        LDY     TMP_GUARD_ROW
        JSR     GUARD_FIND_CANDIDATE_ROW_ABOVE
        LDX     TMP_GUARD_COL
        JSR     PSEUDO_DISTANCE
        CMP     BEST_GUARD_DIST
        BCS     .end
        STA     BEST_GUARD_DIST
        LDA     #GUARD_ACTION_MOVE_UP
        STA     GUARD_ACTION

    .end:
        RTS

        ORG    $72D4
    PSEUDO_DISTANCE:
        SUBROUTINE

        STA     TMP
        CMP     PLAYER_ROW
        BNE     .tmp_not_player_row

        ; TMP == PLAYER_ROW
        ; return | X - GUARD_LOC_COL |

        CPX     GUARD_LOC_COL
        BCC     .x_lt_guard_col

        ; X >= GUARD_LOC_COL
        TXA
        ; A = X - GUARD_LOC_COL
        SEC
        SBC     GUARD_LOC_COL
        RTS

    .x_lt_guard_col:
        STX     TMP

        ; A = GUARD_LOC_COL - X
        LDA     GUARD_LOC_COL
        SEC
        SBC     TMP
        RTS

    .tmp_not_player_row:
        ; If TMP >= PLAYER_ROW, return 200 + | TMP - PLAYER_ROW |
        ; otherwise return 100 + | TMP - PLAYER_ROW |

        BCC     .tmp_lt_player_row

        ; TMP >= PLAYER_ROW
        ; A = TMP - PLAYER_ROW + 200
        SEC
        SBC     PLAYER_ROW
        CLC
        ADC     #200
        RTS

    .tmp_lt_player_row
        ; A = PLAYER_ROW - TMP + 100
        LDA     PLAYER_ROW
        SEC
        SBC     TMP
        CLC
        ADC     #100
        RTS

        ORG    $72FD
        SUBROUTINE
    .not_ladder:
        LDA     CHECK_TMP_ROW         ; row
        RTS

    GUARD_FIND_CANDIDATE_ROW_ABOVE:
        STY     CHECK_TMP_ROW         ; row
        STX     CHECK_TMP_COL         ; col

    .loop:
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     CHECK_TMP_COL         ; col
        LDA     (PTR2),Y    ; sprite on background

        CMP     #SPRITE_LADDER
        BNE     .not_ladder     ; no ladder at row, col -> just return row.

        ; There is a ladder at row, col
        DEC     CHECK_TMP_ROW     ; row--     ; up one
        LDY     CHECK_TMP_COL     ; col
        BEQ     .at_leftmost

        DEY             ; to left (col-1)
        LDA     (PTR2),Y

        ; To left of ladder is brick, stone, or ladder: .blocked_on_left
        CMP     #SPRITE_BRICK
        BEQ     .blocked_on_left
        CMP     #SPRITE_STONE
        BEQ     .blocked_on_left
        CMP     #SPRITE_LADDER
        BEQ     .blocked_on_left

        LDY     CHECK_TMP_ROW     ; row (that is now up one)
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     CHECK_TMP_COL     ; col
        DEY
        LDA     (PTR2),Y    ; sprite on background

        CMP     #SPRITE_ROPE
        BNE     .at_leftmost

        ; There is a rope above the ladder

    .blocked_on_left:
        ; If row <= PLAYER_ROW (on or above player row), return row
        LDY     CHECK_TMP_ROW     ; row
        STY     SCRATCH_5C
        CPY     PLAYER_ROW
        BCC     .return_scratch_5C
        BEQ     .return_scratch_5C

    .at_leftmost:
        LDY     CHECK_TMP_COL     ; col
        CPY     #MAX_GAME_COL
        BEQ     .at_rightmost

        ; Look at background sprite below and to the right
        LDY     CHECK_TMP_ROW     ; row
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     CHECK_TMP_COL
        INY
        LDA     (PTR2),Y        ; get background sprite at row+1, col+1

        ; Below and to the right of ladder is brick, stone, or ladder: .blocked_below
        CMP     #SPRITE_BRICK
        BEQ     .blocked_below
        CMP     #SPRITE_STONE
        BEQ     .blocked_below
        CMP     #SPRITE_LADDER
        BEQ     .blocked_below

        ; Look at background sprite to the right
        LDY     CHECK_TMP_ROW     ; row
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     CHECK_TMP_COL     ; col
        INY
        LDA     (PTR2),Y    ; get background sprite at row, col+1

        CMP     #SPRITE_ROPE
        BNE     .at_rightmost

        ; There is a rope to the right of the ladder

    .blocked_below:
        ; If row <= PLAYER_ROW (on or above player row), return row
        LDY     CHECK_TMP_ROW     ; row
        STY     SCRATCH_5C
        CPY     PLAYER_ROW
        BCC     .return_scratch_5C
        BEQ     .return_scratch_5C

    .at_rightmost:
        ; If row < 1, return row, otherwise loop.
        LDY     CHECK_TMP_ROW     ; row
        CPY     #$01
        BCC     .return_Y
        JMP     .loop

    .return_Y:
        TYA
        RTS

    .return_scratch_5C:
        LDA     SCRATCH_5C
        RTS


        ; Disk routines

    INDIRECT_TARGET             EQU     $36     ; Init with DEFAULT_INDIRECT_TARGET
    DISABLE_INTS_CALL_RWTS_PTR  EQU     $38     ; Init with DISABLE_INTS_CALL_RWTS
    DISABLE_INTS_CALL_RWTS      EQU     $B7B5

    JMP_RWTS        EQU     $23     ; JMP $0000, gets loaded with RWTS address later

        ORG    $739A
        SUBROUTINE
    .return_tmp_row:
        LDA     CHECK_TMP_ROW
        RTS

    GUARD_FIND_CANDIDATE_ROW_BELOW:
        STY     CHECK_TMP_ROW
        STX     CHECK_TMP_COL

        ; for CHECK_TMP_ROW = Y; CHECK_TMP_ROW <= MAX_GAME_ROW; CHECK_TMP_ROW++

    .loop:
        ; if background sprite below tmp coords is brick or stone, return tmp row.

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     CHECK_TMP_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BEQ     .return_tmp_row
        CMP     #SPRITE_STONE
        BEQ     .return_tmp_row

        ; Not brick or stone below
        ; if background sprite at tmp coords is empty, then next tmp row.

        LDY     CHECK_TMP_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     CHECK_TMP_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_EMPTY
        BEQ     .next

        CPY     #$00
        BEQ     .check_right      ; cannot check to left

        ; if background sprite to left of tmp coords is rope,
        ; then tmp_row -> curr_tmp_row
        DEY
        LDA     (PTR2),Y        ; Check to left

        CMP     #SPRITE_ROPE
        BEQ     .store_as_curr_tmp_row

        ; if background sprite to left and below tmp coords is brick, stone, or ladder,
        ; then tmp_row -> curr_tmp_row
        LDY     CHECK_TMP_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     CHECK_TMP_COL
        DEY
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BEQ     .store_as_curr_tmp_row
        CMP     #SPRITE_STONE
        BEQ     .store_as_curr_tmp_row
        CMP     #SPRITE_LADDER
        BNE     .check_right

        ; Otherwise check right

    .store_as_curr_tmp_row:
        ; Store tmp row as curr tmp row, and if at or below player, return curr tmp row.
        LDY     CHECK_TMP_ROW
        STY     CHECK_CURR_TMP_ROW
        CPY     PLAYER_ROW
        BCS     .return_curr_tmp_row
        ; CHECK_TMP_ROW < PLAYER_ROW

    .check_right:
        LDY     CHECK_TMP_COL
        CPY     #MAX_GAME_COL
        BCS     .next           ; can't check right

        ; if background sprite to right is rope,
        ; then tmp_row -> curr_tmp_row
        INY
        LDA     (PTR2),Y

        CMP     #SPRITE_ROPE
        BEQ     .store_as_curr_tmp_row_2

        ; if background sprite to right and below tmp coords is brick, stone, or ladder,
        ; then tmp_row -> curr_tmp_row
        LDY     CHECK_TMP_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     CHECK_TMP_COL
        INY
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BEQ     .store_as_curr_tmp_row_2
        CMP     #SPRITE_LADDER
        BEQ     .store_as_curr_tmp_row_2
        CMP     #SPRITE_STONE
        BNE     .next
        ; Brick, ladder, or stone.

    .store_as_curr_tmp_row_2:
        LDY     CHECK_TMP_ROW
        STY     CHECK_CURR_TMP_ROW
        CPY     PLAYER_ROW
        BCS     .return_curr_tmp_row
        ; CHECK_TMP_ROW < PLAYER_ROW

    .next:
        INC     CHECK_TMP_ROW
        LDY     CHECK_TMP_ROW
        CPY     #MAX_GAME_ROW+1
        BCS     .return_max_game_row
        JMP     .loop

    .return_max_game_row:
        LDA     #MAX_GAME_ROW
        RTS

    .return_curr_tmp_row:
        LDA     CHECK_CURR_TMP_ROW
        RTS

        ORG    $743E
    DETERMINE_GUARD_LEFT_RIGHT_LIMITS:
        SUBROUTINE

        STX     GUARD_LEFT_COL_LIMIT
        STX     GUARD_RIGHT_COL_LIMIT
        STY     ROWNUM

    .loop:
        LDA     GUARD_LEFT_COL_LIMIT
        BEQ     .loop2

        LDY     ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     GUARD_LEFT_COL_LIMIT
        DEY
        LDA     (PTR1),Y

        CMP     #SPRITE_BRICK
        BEQ     .loop2
        CMP     #SPRITE_STONE
        BEQ     .loop2

        CMP     #SPRITE_LADDER
        BEQ     .next
        CMP     #SPRITE_ROPE
        BEQ     .next

        LDY     ROWNUM
        CPY     #MAX_GAME_ROW
        BEQ     .next

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     GUARD_LEFT_COL_LIMIT
        DEY
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BEQ     .next
        CMP     #SPRITE_STONE
        BEQ     .next
        CMP     #SPRITE_LADDER
        BNE     .end_loop

    .next:
        DEC     GUARD_LEFT_COL_LIMIT
        BPL     .loop

    .end_loop:
        DEC     GUARD_LEFT_COL_LIMIT

    .loop2:
        LDA     GUARD_RIGHT_COL_LIMIT
        CMP     #MAX_GAME_COL
        BEQ     .end

        LDY     ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     GUARD_RIGHT_COL_LIMIT
        INY
        LDA     (PTR1),Y

        CMP     #SPRITE_BRICK
        BEQ     .end
        CMP     #SPRITE_STONE
        BEQ     .end

        CMP     #SPRITE_LADDER
        BEQ     .next_loop2
        CMP     #SPRITE_ROPE
        BEQ     .next_loop2

        LDY     ROWNUM
        CPY     #MAX_GAME_ROW
        BEQ     .next_loop2

        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS+1,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2+1,Y
        STA     PTR2+1

        LDY     GUARD_RIGHT_COL_LIMIT
        INY
        LDA     (PTR2),Y

        CMP     #SPRITE_BRICK
        BEQ     .next_loop2
        CMP     #SPRITE_STONE
        BEQ     .next_loop2
        CMP     #SPRITE_LADDER
        BNE     .end_loop2

    .next_loop2:
        INC     GUARD_RIGHT_COL_LIMIT
        BPL     .loop2

    .end_loop2:
        INC     GUARD_RIGHT_COL_LIMIT

    .end:
        RTS

        ORG    $74DF
    GET_GUARD_SPRITE_AND_COORDS:
        SUBROUTINE

        LDX     GUARD_LOC_COL
        LDY     GUARD_X_ADJ
        JSR     GET_HALF_SCREEN_COL_OFFSET_IN_Y_FOR
        STX     SPRITE_NUM

        LDY     GUARD_LOC_ROW
        LDX     GUARD_Y_ADJ
        JSR     GET_SCREEN_ROW_OFFSET_IN_X_FOR
        LDX     GUARD_ANIM_STATE
        LDA     GUARD_ANIM_SPRITES,X

        LDX     SPRITE_NUM
        RTS

        ORG    $74F7
    CHECK_FOR_GOLD_PICKED_UP_BY_GUARD:
        SUBROUTINE

        LDA     GUARD_X_ADJ
        CMP     #$02
        BNE     .end
        LDA     GUARD_Y_ADJ
        CMP     #$02
        BNE     .end

        LDY     GUARD_LOC_ROW
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GUARD_LOC_COL
        LDA     (PTR2),Y

        CMP     #SPRITE_GOLD
        BNE     .end

        LDA     GUARD_GOLD_TIMER        ; Does guard have gold already?
        BMI     .end

        LDA     #$FF
        SEC
        SBC     $53
        STA     GUARD_GOLD_TIMER        ; GUARD_GOLD_TIMER = 0xFF - $53

        ; Remove gold from screen
        LDA     #SPRITE_EMPTY
        STA     (PTR2),Y
        LDY     GUARD_LOC_ROW
        STY     GAME_ROWNUM
        LDY     GUARD_LOC_COL
        STY     GAME_COLNUM
        JSR     DRAW_SPRITE_PAGE2

        LDY     GAME_ROWNUM
        LDX     GAME_COLNUM
        JSR     GET_SCREEN_COORDS_FOR
        LDA     #SPRITE_GOLD
        JMP     ERASE_SPRITE_AT_PIXEL_COORDS        ; tailcall

    .end:
        RTS

        ORG    $753E
    GUARD_DROP_GOLD:
        SUBROUTINE

        LDA     GUARD_GOLD_TIMER
        BPL     .end
        INC     GUARD_GOLD_TIMER
        BNE     .end

        ; GUARD_GOLD_TIMER == 0
        LDY     GUARD_LOC_ROW
        STY     GAME_ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR2
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES2,Y
        STA     PTR2+1

        LDY     GUARD_LOC_COL
        STY     GAME_COLNUM
        LDA     (PTR2),Y

        CMP     #SPRITE_EMPTY
        BNE     .decrement_flag_0

        ; Put gold at location
        LDA     #SPRITE_GOLD
        STA     (PTR2),Y
        JSR     DRAW_SPRITE_PAGE2
        LDY     GAME_ROWNUM
        LDX     GAME_COLNUM
        JSR     GET_SCREEN_COORDS_FOR
        LDA     #SPRITE_GOLD
        JMP     DRAW_SPRITE_AT_PIXEL_COORDS

    .decrement_flag_0:
        DEC     GUARD_GOLD_TIMER

    .end:
        RTS

        ORG    $7574
    INC_GUARD_ANIM_STATE:
        SUBROUTINE

        INC     GUARD_ANIM_STATE
        CMP     GUARD_ANIM_STATE
        BCC     .check_upper_bound      ; lower bound < GUARD_ANIM_STATE?
        ; otherwise PLAYER_ANIM_STATE <= lower bound:

    .write_lower_bound:
        STA     GUARD_ANIM_STATE       ; GUARD_ANIM_STATE = lower bound
        RTS

    .check_upper_bound:
        CPX     GUARD_ANIM_STATE
        BCC     .write_lower_bound       ; GUARD_ANIM_STATE > upper bound?
        ; otherwise GUARD_ANIM_STATE <= upper bound:
        RTS

        ORG    $7582
    NUDGE_GUARD_TOWARDS_EXACT_COLUMN:
        SUBROUTINE

        LDA     GUARD_X_ADJ
        CMP     #$02
        BCC     .slightly_left
        BEQ     .end

    .slightly_right:
        DEC     GUARD_X_ADJ        ; Nudge guard left
        JMP     CHECK_FOR_GOLD_PICKED_UP_BY_GUARD

    .slightly_left:
        INC     GUARD_X_ADJ        ; Nudge guard right
        JMP     CHECK_FOR_GOLD_PICKED_UP_BY_GUARD

    .end:
        RTS

        ORG    $7595
    NUDGE_GUARD_TOWARDS_EXACT_ROW:
        SUBROUTINE

        LDA     GUARD_Y_ADJ
        CMP     #$02
        BCC     .slightly_above
        BEQ     .end

    .slightly_below:
        DEC     GUARD_Y_ADJ        ; Nudge guard up
        JMP     CHECK_FOR_GOLD_PICKED_UP_BY_GUARD

    .slightly_above:
        INC     GUARD_Y_ADJ        ; Nudge guard down
        JMP     CHECK_FOR_GOLD_PICKED_UP_BY_GUARD

    .end:
        RTS

        ORG    $75A8
    STORE_GUARD_DATA:
        SUBROUTINE

        LDX     GUARD_NUM
        LDA     GUARD_LOC_COL
        STA     GUARD_LOCS_COL,X
        LDA     GUARD_LOC_ROW
        STA     GUARD_LOCS_ROW,X
        LDA     GUARD_X_ADJ
        STA     GUARD_X_ADJS,X
        LDA     GUARD_Y_ADJ
        STA     GUARD_Y_ADJS,X
        LDA     GUARD_GOLD_TIMER
        STA     GUARD_GOLD_TIMERS,X
        LDA     GUARD_FACING_DIRECTION
        STA     GUARD_FACING_DIRECTIONS,X
        LDA     GUARD_ANIM_STATE
        STA     GUARD_ANIM_STATES,X
        RTS

    LOAD_GUARD_DATA:
        SUBROUTINE

        LDX     GUARD_NUM
        LDA     GUARD_LOCS_COL,X
        STA     GUARD_LOC_COL
        LDA     GUARD_LOCS_ROW,X
        STA     GUARD_LOC_ROW
        LDA     GUARD_X_ADJS,X
        STA     GUARD_X_ADJ
        LDA     GUARD_Y_ADJS,X
        STA     GUARD_Y_ADJ
        LDA     GUARD_ANIM_STATES,X
        STA     GUARD_ANIM_STATE
        LDA     GUARD_FACING_DIRECTIONS,X
        STA     GUARD_FACING_DIRECTION
        LDA     GUARD_GOLD_TIMERS,X
        STA     GUARD_GOLD_TIMER
        RTS

*/
#include "lode_runner.h"

static void move_guard(void);
static uint8_t determine_guard_move(void);

const uint8_t GUARD_PATTERNS_LIST[36] = {
    0x00, 0x00, 0x00,
    0x00, 0x01, 0x01,
    0x01, 0x01, 0x01,
    0x01, 0x03, 0x01,
    0x01, 0x03, 0x03,
    0x03, 0x03, 0x03,
    0x03, 0x03, 0x07,
    0x03, 0x07, 0x07,
    0x07, 0x07, 0x07,
    0x07, 0x07, 0x0F,
    0x07, 0x0F, 0x0F,
    0x0F, 0x0F, 0x0F,
};

/* The sprite for each GUARD_ANIM_STATE. */
static const uint8_t GUARD_ANIM_SPRITES[16] = {
    0x08, 0x2B, 0x2C,   /* 0-2: running left */
    0x30, 0x31, 0x32,   /* 3-5: hand over hand along a rope, left */
    0x36,               /* 6: falling, facing left */
    0x28, 0x29, 0x2A,   /* 7-9: running right */
    0x2D, 0x2E, 0x2F,   /* 10-12: hand over hand along a rope, right */
    0x35,               /* 13: falling, facing right */
    0x33, 0x34,         /* 14-15: climbing */
};

/* GUARD_X_ADJ for each GUARD_GOLD_TIMER from 7 to 12, which shakes the
 * guard from side to side just before it climbs out of a hole. */
static const uint8_t GUARD_X_ADJ_TABLE[6] = {2, 1, 2, 3, 2, 1};

/* Played when a guard falls into a hole. Each pair is a duration and
 * a pitch. */
static const uint8_t guard_in_hole_tune[] = {
    0x06, 0x20, 0x04, 0x30, 0x02, 0x40, 0x00,
};

/* Moves the guards whose bits are set in this phase's pattern, the
 * lowest bit first, and stops as soon as one of them kills the player.
 * Each bit moves the next guard in turn, whichever guard that is. */
void move_guards(void) {
    if (GUARD_COUNT == 0)
        return;
    if (++GUARD_PHASE >= 3)
        GUARD_PHASE = 0;
    uint8_t pattern = GUARD_PATTERNS[GUARD_PHASE];
    do {
        if (pattern & 1) {
            move_guard();
            if (!ALIVE)
                return;
        }
        pattern >>= 1;
    } while (pattern != 0);
}

/* Returns true while the current guard is stuck in a hole, which a
 * positive GUARD_GOLD_TIMER counts down. */
static bool guard_in_hole(void) {
    return (int8_t)GUARD_GOLD_TIMER > 0;
}

/* Copies the current guard's state back into the GUARD_* arrays. */
static void store_guard_data(void) {
    uint8_t g = GUARD_NUM;
    GUARD_LOCS_COL[g] = GUARD_LOC_COL;
    GUARD_LOCS_ROW[g] = GUARD_LOC_ROW;
    GUARD_X_ADJS[g] = GUARD_X_ADJ;
    GUARD_Y_ADJS[g] = GUARD_Y_ADJ;
    GUARD_GOLD_TIMERS[g] = GUARD_GOLD_TIMER;
    GUARD_FACING_DIRECTIONS[g] = GUARD_FACING_DIRECTION;
    GUARD_ANIM_STATES[g] = GUARD_ANIM_STATE;
}

/* Copies guard GUARD_NUM's state out of the GUARD_* arrays. */
void load_guard_data(void) {
    uint8_t g = GUARD_NUM;
    GUARD_LOC_COL = GUARD_LOCS_COL[g];
    GUARD_LOC_ROW = GUARD_LOCS_ROW[g];
    GUARD_X_ADJ = GUARD_X_ADJS[g];
    GUARD_Y_ADJ = GUARD_Y_ADJS[g];
    GUARD_ANIM_STATE = GUARD_ANIM_STATES[g];
    GUARD_FACING_DIRECTION = GUARD_FACING_DIRECTIONS[g];
    GUARD_GOLD_TIMER = GUARD_GOLD_TIMERS[g];
}

/* The double-pixel column and the screen line of the current guard's
 * sprite. */
static uint8_t guard_half_col(void) {
    return half_screen_col_for(GUARD_LOC_COL, GUARD_X_ADJ);
}

static uint8_t guard_screen_row(void) {
    return screen_row_for(GUARD_LOC_ROW, GUARD_Y_ADJ);
}

void erase_guard(void) {
    erase_sprite_at_pixel_coords(
        GUARD_ANIM_SPRITES[GUARD_ANIM_STATE], guard_half_col(),
        guard_screen_row());
}

void draw_guard_sprite(uint8_t sprite) {
    draw_sprite_at_pixel_coords(sprite, guard_half_col(), guard_screen_row());
}

/* Draws the current guard in its animation state. */
static void draw_guard(void) {
    draw_guard_sprite(GUARD_ANIM_SPRITES[GUARD_ANIM_STATE]);
}

/* Draws gold at GAME_COLNUM, GAME_ROWNUM on both pages. */
static void draw_gold_at_game_cell(void) {
    draw_sprite_page2(SPRITE_GOLD);
    draw_sprite_at_pixel_coords(
        SPRITE_GOLD, HALF_SCREEN_COL_TABLE[GAME_COLNUM],
        SCREEN_ROW_TABLE[GAME_ROWNUM]);
}

/* Picks up the gold in the guard's cell, if the guard is exactly on the
 * cell and isn't carrying gold already. The guard then carries it for
 * GUARD_RESURRECT_COL + 1 cells, since GUARD_DROP_GOLD counts
 * GUARD_GOLD_TIMER up to 0. GUARD_RESURRECT_COL steps every turn, so it
 * serves as a random number here. */
static void check_for_gold_picked_up_by_guard(void) {
    if (GUARD_X_ADJ != 2 || GUARD_Y_ADJ != 2)
        return;
    uint8_t row = GUARD_LOC_ROW;
    uint8_t col = GUARD_LOC_COL;
    if (LEVEL_BG[row][col] != SPRITE_GOLD || (int8_t)GUARD_GOLD_TIMER < 0)
        return;
    GUARD_GOLD_TIMER = 0xFF - GUARD_RESURRECT_COL;
    LEVEL_BG[row][col] = SPRITE_EMPTY;
    GAME_ROWNUM = row;
    GAME_COLNUM = col;
    draw_sprite_page2(SPRITE_EMPTY);
    erase_sprite_at_pixel_coords(
        SPRITE_GOLD, HALF_SCREEN_COL_TABLE[col], SCREEN_ROW_TABLE[row]);
}

/* Counts GUARD_GOLD_TIMER up as a guard carrying gold leaves a cell,
 * and drops the gold in the cell when the count reaches 0. If the cell
 * isn't empty, the count goes back to 0xFF, so the guard drops the gold
 * at the next cell it can. Only LEVEL_BG gets the gold, since the guard
 * is still in the cell in LEVEL_MAP and leave_cell copies it there. */
static void guard_drop_gold(void) {
    if ((int8_t)GUARD_GOLD_TIMER >= 0 || ++GUARD_GOLD_TIMER != 0)
        return;
    uint8_t row = GUARD_LOC_ROW;
    uint8_t col = GUARD_LOC_COL;
    GAME_ROWNUM = row;
    GAME_COLNUM = col;
    if (LEVEL_BG[row][col] == SPRITE_EMPTY) {
        LEVEL_BG[row][col] = SPRITE_GOLD;
        draw_gold_at_game_cell();
    } else {
        GUARD_GOLD_TIMER--;
    }
}

/* Steps GUARD_ANIM_STATE through the states from `first` to `last`,
 * going back to `first` after `last` or when the state was outside
 * them. */
static void inc_guard_anim_state(uint8_t first, uint8_t last) {
    uint8_t state = ++GUARD_ANIM_STATE;
    if (state <= first || state > last)
        GUARD_ANIM_STATE = first;
}

/* Moves the guard one step toward the middle of its column or row, as
 * it moves along the other direction, and picks up gold if that puts
 * it exactly on a cell. */
static void nudge_guard_towards_exact_column(void) {
    if (GUARD_X_ADJ == 2)
        return;
    if (GUARD_X_ADJ < 2)
        GUARD_X_ADJ++;
    else
        GUARD_X_ADJ--;
    check_for_gold_picked_up_by_guard();
}

static void nudge_guard_towards_exact_row(void) {
    if (GUARD_Y_ADJ == 2)
        return;
    if (GUARD_Y_ADJ < 2)
        GUARD_Y_ADJ++;
    else
        GUARD_Y_ADJ--;
    check_for_gold_picked_up_by_guard();
}

/* Takes the guard out of a cell of LEVEL_MAP, leaving what LEVEL_BG
 * holds there. A dug hole is still a brick in LEVEL_BG, so a guard
 * climbing out of one leaves the cell empty. */
static void leave_cell(uint8_t row, uint8_t col) {
    uint8_t sprite = LEVEL_BG[row][col];
    LEVEL_MAP[row][col] = sprite == SPRITE_BRICK ? SPRITE_EMPTY : sprite;
}

/* Puts the guard in a cell of LEVEL_MAP, killing the player if the
 * player is there. */
static void enter_cell(uint8_t row, uint8_t col) {
    if (LEVEL_MAP[row][col] == SPRITE_PLAYER)
        ALIVE >>= 1;
    LEVEL_MAP[row][col] = SPRITE_GUARD;
}

/* Returns true for the sprites a guard can stand on. */
static bool is_floor(uint8_t sprite) {
    return sprite == SPRITE_BRICK || sprite == SPRITE_STONE ||
           sprite == SPRITE_LADDER;
}

/* Returns true when the guard stays up, which it does on a ladder,
 * hanging exactly on a rope, or standing on the bottom row or on a
 * brick, stone, ladder, or another guard. A guard less than halfway
 * into its cell from above keeps falling. */
static bool guard_is_supported(void) {
    uint8_t row = GUARD_LOC_ROW;
    uint8_t col = GUARD_LOC_COL;
    uint8_t sprite = LEVEL_BG[row][col];
    if (sprite == SPRITE_LADDER)
        return true;
    if (sprite == SPRITE_ROPE && GUARD_Y_ADJ == 2)
        return true;
    if (GUARD_Y_ADJ < 2)
        return false;
    if (row == MAX_GAME_ROW)
        return true;
    sprite = LEVEL_MAP[row + 1][col];
    if (sprite == SPRITE_EMPTY || sprite == SPRITE_PLAYER)
        return false;
    if (sprite == SPRITE_GUARD)
        return true;
    return is_floor(LEVEL_BG[row + 1][col]);
}

/* Moves the falling guard down into the cell below. A guard carrying
 * gold that lands in a hole loses the gold. It's put in the cell above
 * the hole if that's empty, and otherwise GOLD_COUNT goes down so that
 * the level can be finished without it. */
static void guard_fall_into_row_below(void) {
    uint8_t col = GUARD_LOC_COL;
    GUARD_Y_ADJ = 0;
    leave_cell(GUARD_LOC_ROW, col);
    uint8_t row = ++GUARD_LOC_ROW;
    if (LEVEL_BG[row][col] == SPRITE_BRICK && (int8_t)GUARD_GOLD_TIMER < 0) {
        uint8_t above = row - 1;
        GAME_ROWNUM = above;
        GAME_COLNUM = col;
        if (LEVEL_BG[above][col] == SPRITE_EMPTY) {
            LEVEL_MAP[above][col] = SPRITE_GOLD;
            LEVEL_BG[above][col] = SPRITE_GOLD;
            draw_gold_at_game_cell();
        } else {
            GOLD_COUNT--;
        }
        GUARD_GOLD_TIMER = 0;
    }
    enter_cell(row, col);
}

/* Moves the guard down a step as it falls. A guard that reaches the
 * middle of a hole is trapped there for GUARD_GOLD_TIMER_START_VALUE
 * turns and scores 75 points. */
static void guard_fall(void) {
    erase_guard();
    nudge_guard_towards_exact_column();
    GUARD_ANIM_STATE = (int8_t)GUARD_FACING_DIRECTION < 0 ? 6 : 13;
    if (++GUARD_Y_ADJ >= 5) {
        guard_fall_into_row_below();
    } else if (GUARD_Y_ADJ == 2) {
        check_for_gold_picked_up_by_guard();
        if (LEVEL_BG[GUARD_LOC_ROW][GUARD_LOC_COL] == SPRITE_BRICK) {
            /* A guard trapped while carrying gold gives it up, and the
             * gold is gone. */
            if ((int8_t)GUARD_GOLD_TIMER < 0)
                GOLD_COUNT--;
            GUARD_GOLD_TIMER = GUARD_GOLD_TIMER_START_VALUE;
            add_and_update_score(0x0075);
            load_sound_data(guard_in_hole_tune);
        }
    }
    draw_guard();
}

/* Counts up the time left in a hole while the guard can't climb out,
 * so that it waits there. */
static void guard_do_nothing(void) {
    if (guard_in_hole())
        GUARD_GOLD_TIMER++;
}

/* Returns true when the guard can't climb into the cell above, because
 * it's on the top row or that cell holds a brick, stone, a trap, or a
 * guard. */
static bool guard_blocked_above(void) {
    uint8_t row = GUARD_LOC_ROW;
    if (row == 0)
        return true;
    uint8_t sprite = LEVEL_MAP[row - 1][GUARD_LOC_COL];
    return sprite == SPRITE_BRICK || sprite == SPRITE_STONE ||
           sprite == SPRITE_TRAP || sprite == SPRITE_GUARD;
}

static void try_guard_move_up(void) {
    /* A guard below the middle of its cell can always move up to it. */
    if (GUARD_Y_ADJ < 3 && guard_blocked_above()) {
        guard_do_nothing();
        return;
    }
    erase_guard();
    nudge_guard_towards_exact_column();
    uint8_t col = GUARD_LOC_COL;
    if ((int8_t)--GUARD_Y_ADJ < 0) {
        guard_drop_gold();
        leave_cell(GUARD_LOC_ROW, col);
        enter_cell(--GUARD_LOC_ROW, col);
        GUARD_Y_ADJ = 4;
    } else {
        check_for_gold_picked_up_by_guard();
    }
    inc_guard_anim_state(14, 15);
    draw_guard();
}

static void try_guard_move_down(void) {
    uint8_t col = GUARD_LOC_COL;
    /* A guard above the middle of its cell can always move down to it. */
    if (GUARD_Y_ADJ >= 2) {
        uint8_t row = GUARD_LOC_ROW;
        if (row >= MAX_GAME_ROW)
            return;
        uint8_t sprite = LEVEL_MAP[row + 1][col];
        if (sprite == SPRITE_STONE || sprite == SPRITE_GUARD ||
            sprite == SPRITE_BRICK)
            return;
    }
    erase_guard();
    nudge_guard_towards_exact_column();
    if (++GUARD_Y_ADJ >= 5) {
        guard_drop_gold();
        leave_cell(GUARD_LOC_ROW, col);
        enter_cell(++GUARD_LOC_ROW, col);
        GUARD_Y_ADJ = 0;
    } else {
        check_for_gold_picked_up_by_guard();
    }
    inc_guard_anim_state(14, 15);
    draw_guard();
}

static void try_guard_move_left(void) {
    uint8_t row = GUARD_LOC_ROW;
    /* A guard right of the middle of its cell can always move left to
     * it. */
    if (GUARD_X_ADJ < 3) {
        uint8_t col = GUARD_LOC_COL;
        if (col == 0)
            return;
        uint8_t sprite = LEVEL_MAP[row][col - 1];
        if (sprite == SPRITE_GUARD || sprite == SPRITE_STONE ||
            sprite == SPRITE_BRICK || LEVEL_BG[row][col - 1] == SPRITE_TRAP)
            return;
    }
    erase_guard();
    nudge_guard_towards_exact_row();
    GUARD_FACING_DIRECTION = 0xFF;
    if ((int8_t)--GUARD_X_ADJ < 0) {
        guard_drop_gold();
        leave_cell(row, GUARD_LOC_COL);
        enter_cell(row, --GUARD_LOC_COL);
        GUARD_X_ADJ = 4;
    } else {
        check_for_gold_picked_up_by_guard();
    }
    if (LEVEL_BG[row][GUARD_LOC_COL] == SPRITE_ROPE)
        inc_guard_anim_state(3, 5);
    else
        inc_guard_anim_state(0, 2);
    draw_guard();
}

static void try_guard_move_right(void) {
    uint8_t row = GUARD_LOC_ROW;
    /* A guard left of the middle of its cell can always move right to
     * it. */
    if (GUARD_X_ADJ >= 2) {
        uint8_t col = GUARD_LOC_COL;
        if (col == MAX_GAME_COL)
            return;
        uint8_t sprite = LEVEL_MAP[row][col + 1];
        if (sprite == SPRITE_GUARD || sprite == SPRITE_STONE ||
            sprite == SPRITE_BRICK || LEVEL_BG[row][col + 1] == SPRITE_TRAP)
            return;
    }
    erase_guard();
    nudge_guard_towards_exact_row();
    GUARD_FACING_DIRECTION = 0x01;
    if (++GUARD_X_ADJ >= 5) {
        guard_drop_gold();
        leave_cell(row, GUARD_LOC_COL);
        enter_cell(row, ++GUARD_LOC_COL);
        GUARD_X_ADJ = 0;
    } else {
        check_for_gold_picked_up_by_guard();
    }
    if (LEVEL_BG[row][GUARD_LOC_COL] == SPRITE_ROPE)
        inc_guard_anim_state(10, 12);
    else
        inc_guard_anim_state(7, 9);
    draw_guard();
}

/* Makes the move DETERMINE_GUARD_MOVE returns, if the way is clear. */
static void guard_take_move(void) {
    switch (determine_guard_move()) {
    case GUARD_ACTION_MOVE_LEFT:
        try_guard_move_left();
        break;
    case GUARD_ACTION_MOVE_RIGHT:
        try_guard_move_right();
        break;
    case GUARD_ACTION_MOVE_UP:
        try_guard_move_up();
        break;
    case GUARD_ACTION_MOVE_DOWN:
        try_guard_move_down();
        break;
    }
}

/* Moves the next guard, GUARD_NUM counting from 1 to GUARD_COUNT and
 * around again. While a trapped guard's GUARD_GOLD_TIMER is 13 or more, the guard stays still, and an
 * egg (GUARD_RESURRECTION_TIMERS) isn't drawn as a guard. From 12 to 7
 * the guard shakes, and below 7 it climbs out. */
static void move_guard(void) {
    if (++GUARD_NUM > GUARD_COUNT)
        GUARD_NUM = 1;
    load_guard_data();
    if (guard_in_hole()) {
        GUARD_GOLD_TIMER--;
        if (GUARD_GOLD_TIMER >= 13) {
            if (GUARD_RESURRECTION_TIMERS[GUARD_NUM] == 0)
                draw_guard();
        } else if (GUARD_GOLD_TIMER >= 7) {
            erase_guard();
            GUARD_X_ADJ = GUARD_X_ADJ_TABLE[GUARD_GOLD_TIMER - 7];
            draw_guard();
        } else {
            guard_take_move();
        }
    } else if (guard_is_supported()) {
        guard_take_move();
    } else {
        guard_fall();
    }
    store_guard_data();
}

/* ---- The AI ------------------------------------------------------------ */

/* The best move found so far, and its score from pseudo_distance. */
static uint8_t guard_action;
static uint8_t best_guard_dist;

/* Returns a score for the guard ending up at a row and column, lower
 * being better. On the player's row it's the number of columns between
 * there and the guard. A row above the player's scores 100 plus the
 * number of rows between, and a row below 200 plus the number of rows,
 * so any row above the player's scores better than any row below it. */
static uint8_t pseudo_distance(uint8_t row, uint8_t col) {
    if (row == PLAYER_ROW) {
        if (col >= GUARD_LOC_COL)
            return col - GUARD_LOC_COL;
        return GUARD_LOC_COL - col;
    }
    if (row > PLAYER_ROW)
        return row - PLAYER_ROW + 200;
    return PLAYER_ROW - row + 100;
}

/* Returns the row a guard climbing the ladder at row, col would reach.
 * That's the first row on or above the player's where it could step off
 * the ladder to the left or right, or else the top of the ladder.
 * Callers pass a row other than 0. */
static uint8_t guard_find_candidate_row_above(uint8_t row, uint8_t col) {
    for (;;) {
        if (LEVEL_BG[row][col] != SPRITE_LADDER)
            return row;
        uint8_t ladder_row = row;
        row--;
        /* The guard can step off the ladder at `row` onto a cell beside
         * it with a brick, stone, or ladder under it, or onto a rope
         * beside it. */
        if (col != 0 && (is_floor(LEVEL_BG[ladder_row][col - 1]) ||
                         LEVEL_BG[row][col - 1] == SPRITE_ROPE)) {
            if (row <= PLAYER_ROW)
                return row;
        }
        if (col != MAX_GAME_COL && (is_floor(LEVEL_BG[ladder_row][col + 1]) ||
                                    LEVEL_BG[row][col + 1] == SPRITE_ROPE)) {
            if (row <= PLAYER_ROW)
                return row;
        }
        if (row == 0)
            return row;
    }
}

/* Returns the row a guard dropping down the column at col from row
 * would reach. That's the first row on or below the player's where it
 * could go left or right, or else the row it would land on. Callers
 * pass a row other than 15. */
static uint8_t guard_find_candidate_row_below(uint8_t row, uint8_t col) {
    for (; row != MAX_GAME_ROW; row++) {
        uint8_t sprite = LEVEL_BG[row + 1][col];
        if (sprite == SPRITE_BRICK || sprite == SPRITE_STONE)
            return row;
        if (LEVEL_BG[row][col] == SPRITE_EMPTY)
            continue;
        /* A way off the column is a rope beside it, or a brick, stone,
         * or ladder below and beside it. The original tests for a rope
         * to the right in the row it last read, which is the row below
         * when it has tested below and to the left. */
        uint8_t right_row = row;
        if (col != 0) {
            if (LEVEL_BG[row][col - 1] == SPRITE_ROPE) {
                if (row >= PLAYER_ROW)
                    return row;
            } else {
                right_row = row + 1;
                if (is_floor(LEVEL_BG[row + 1][col - 1])) {
                    if (row >= PLAYER_ROW)
                        return row;
                }
            }
        }
        if (col < MAX_GAME_COL && (LEVEL_BG[right_row][col + 1] == SPRITE_ROPE ||
                                   is_floor(LEVEL_BG[row + 1][col + 1]))) {
            if (row >= PLAYER_ROW)
                return row;
        }
    }
    return MAX_GAME_ROW;
}

/* Returns the leftmost and rightmost columns the guard can reach along
 * its row. The guard can go past a ladder or rope, or a cell with a
 * brick, stone, or ladder under it, and stops at a brick or stone. The
 * first cell it would fall from is included. */
static uint8_t guard_left_col_limit(uint8_t row, uint8_t col) {
    while (col != 0) {
        uint8_t sprite = LEVEL_MAP[row][col - 1];
        if (sprite == SPRITE_BRICK || sprite == SPRITE_STONE)
            break;
        col--;
        if (sprite != SPRITE_LADDER && sprite != SPRITE_ROPE &&
            row != MAX_GAME_ROW) {
            if (!is_floor(LEVEL_BG[row + 1][col]))
                break;
        }
    }
    return col;
}

static uint8_t guard_right_col_limit(uint8_t row, uint8_t col) {
    while (col != MAX_GAME_COL) {
        uint8_t sprite = LEVEL_MAP[row][col + 1];
        if (sprite == SPRITE_BRICK || sprite == SPRITE_STONE)
            break;
        col++;
        if (sprite != SPRITE_LADDER && sprite != SPRITE_ROPE &&
            row != MAX_GAME_ROW) {
            if (!is_floor(LEVEL_BG[row + 1][col]))
                break;
        }
    }
    return col;
}

/* Makes `action` the best move if `dist` beats the best so far. */
static void consider_move(uint8_t dist, uint8_t action) {
    if (dist < best_guard_dist) {
        best_guard_dist = dist;
        guard_action = action;
    }
}

/* Scores dropping down from the guard's row at column `col`, as the
 * move `down`, and climbing a ladder there, as the move `up`. */
static void consider_column(uint8_t row, uint8_t col, uint8_t down, uint8_t up) {
    if (row != MAX_GAME_ROW) {
        uint8_t sprite = LEVEL_BG[row + 1][col];
        if (sprite != SPRITE_BRICK && sprite != SPRITE_STONE)
            consider_move(
                pseudo_distance(guard_find_candidate_row_below(row, col), col),
                down);
    }
    if (row != 0 && LEVEL_BG[row][col] == SPRITE_LADDER)
        consider_move(
            pseudo_distance(guard_find_candidate_row_above(row, col), col), up);
}

/* Returns true when a guard can cross the cell at row, col without
 * falling, because it holds a ladder or rope, it's on the bottom row,
 * or what's under it isn't empty or a trap. */
static bool guard_can_cross(uint8_t row, uint8_t col) {
    uint8_t sprite = LEVEL_BG[row][col];
    if (sprite == SPRITE_LADDER || sprite == SPRITE_ROPE || row == MAX_GAME_ROW)
        return true;
    sprite = LEVEL_BG[row + 1][col];
    return sprite != SPRITE_EMPTY && sprite != SPRITE_TRAP;
}

/* Returns the move straight along the row toward the player, when the
 * guard can cross every cell to the player's column, or
 * GUARD_ACTION_DO_NOTHING. When the guard is in the player's column,
 * the search to the left wraps around past column 0 as the original's
 * does. */
static uint8_t guard_run_at_player(uint8_t row, uint8_t col) {
    if (col < PLAYER_COL) {
        do {
            if (!guard_can_cross(row, ++col))
                return GUARD_ACTION_DO_NOTHING;
        } while (col != PLAYER_COL);
        return GUARD_ACTION_MOVE_RIGHT;
    }
    do {
        if (!guard_can_cross(row, --col))
            return GUARD_ACTION_DO_NOTHING;
    } while (col != PLAYER_COL);
    return GUARD_ACTION_MOVE_LEFT;
}

/* Returns the current guard's move, a GUARD_ACTION_*. A guard in a hole
 * climbs up out of it. */
static uint8_t determine_guard_move(void) {
    uint8_t col = GUARD_LOC_COL;
    uint8_t row = GUARD_LOC_ROW;
    if (LEVEL_BG[row][col] == SPRITE_BRICK && guard_in_hole())
        return GUARD_ACTION_MOVE_UP;
    if (row == PLAYER_ROW) {
        uint8_t action = guard_run_at_player(row, col);
        if (action != GUARD_ACTION_DO_NOTHING)
            return action;
    }
    guard_action = GUARD_ACTION_DO_NOTHING;
    best_guard_dist = 0xFF;
    consider_column(row, col, GUARD_ACTION_MOVE_DOWN, GUARD_ACTION_MOVE_UP);
    for (uint8_t c = guard_left_col_limit(row, col); c != col; c++)
        consider_column(row, c, GUARD_ACTION_MOVE_LEFT, GUARD_ACTION_MOVE_LEFT);
    for (uint8_t c = guard_right_col_limit(row, col); c != col; c--)
        consider_column(row, c, GUARD_ACTION_MOVE_RIGHT, GUARD_ACTION_MOVE_RIGHT);
    return guard_action;
}
