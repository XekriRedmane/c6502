/*
    ai.c -- how the computer's mobs choose where to go, and the weapon
    and armor shops.

    At the start of an AI mob's turn, ai_choose_target picks a target
    position by the mob's AI mode, which is its appearance's font
    group (CHAR_AI_MODE). Modes 0 and 1 may join the party of a player
    character in the same location. Mode 2 heads for the nearest
    encounter entry while its life is at least its strength. Mode 3
    hunts the nearest hostile mob and mode 5 and up the strongest, and
    both fall back to EVENT_POS when there is none. Mode 4 wanders with
    no target. For each step, npc_move_ai scores staying put and the
    four moves by how far they leave the mob from its target and by
    what is on the square, adds a random amount, and takes the lowest
    score. A move back the way the mob came costs 3 more.

    A shop entry of an event list opens the shop's menu for a player's
    mob. The shop shows one item at a time, the next one of its eight
    slots that the entry's mask enables, priced at the slot's price
    times the location's tier plus 1. Buying a weapon sets the sword or
    the dagger, and buying armor sets the armor and moves part of the
    level into it.

    Departures from the original:

    - MANHATTAN_DISTANCE measures from ENTITY_PTR's position, and
      EVALUATE_DIRECTION stores the square it tests into that byte for
      the call and puts the old value back from AI_SAVED_POS ($5A69).
      manhattan_distance takes the position to measure from as a
      parameter, so the C makes neither store.
    - The original's scratch bytes are locals and parameters. They are
      AI_DIRECTION ($5A6F), AI_TEST_POS ($5A70), and NEAREST_DIST
      ($5A14) in page $5A, and in zero page the lowest score and its
      direction ($BA, $BB), the event entry's pointer and the strongest
      mob's position and strength ($BE, $BF), and the price and the
      handler word (NUM_VALUE, $BC).
    - INC_MOB_COUNTERS adds its penalty to DIR_SCORES one INC at a
      time, and would fall into AI_CHOOSE_TARGET if a score reached
      $80. No score passes 21 before the scaling, so the C adds the
      penalty in one step.
    - SHOP_DATA_PTR ($E6) points at the shop's record of SHOP_TABLE,
      which DISPLAY_SHOP finds by calling ADVANCE_SHOP_PTR once for
      each record it skips. Here the record is the static pointer
      `shop`, which display_shop sets by indexing SHOP_TABLE.
      ADD_PRICE_COMPONENT, which the original calls once for each tier
      and once more, is a multiplication.
    - The entry's slot is SHOP_ITEM_OFFSET bytes into the record, as in
      the original, and the C adds that byte offset to the record's
      address. The header keeps SHOP_ITEM_OFFSET at $5A85, where the
      original keeps it.
    - SHOP_BUY_HANDLER jumps through the slot's handler word to one of
      SHOP_BUY_EFFECT_0-4 or XP_AWARD_0-4, each of which loads its
      number and branches to APPLY_SHOP_EFFECT or AWARD_XP, which jump
      to BROWSE_HANDLER. shop_buy_handler compares the word with the
      ten addresses and calls apply_shop_effect(n) or award_xp(n). Only
      the slots that no shipped mask enables hold any other word. The
      original would jump into data through one of those, and the C
      calls neither effect.
    - The chain's exits are CHAIN_ states, as in every unit. DISPLAY_SHOP
      returns CHAIN_RETURN for an AI mob and otherwise
      CHAIN_START_GAME_INPUT, and the shop's handlers return
      CHAIN_BROWSE_HANDLER where the original jumps to BROWSE_HANDLER.
      BROWSE_HANDLER is the middle of DISPLAY_SHOP, and the part they
      share is show_item.

    Several comments in the original disagree with its code, and the C
    follows the code. The shop's tier is LOCATION_STYLE's bits 6-5, the
    strength bonus is bits 4-3, AWARD_XP overwrites the armor byte, a
    square that isn't the target costs 9, mode 2 seeks an encounter
    when its life is at least its strength, and a mob that
    FIND_ENTITY_AT_POS finds costs nothing.

    Three quirks are kept. FIND_NEAREST_EVENT returns the last call's
    result when no entry qualifies. With an empty mask, the shop's
    search for an enabled slot never ends. AWARD_XP never ends when the
    level and the pending amount add up to 0.

    Ali Baba, main.asm lines 3434-3528, 11724-12116, and 13470-13826:

        ORG     $138D
    FIND_NEAREST_EVENT:
        SUBROUTINE FIND_NEAREST_EVENT
        ; Find the nearest non-activated event to the current entity.
        ;
        ; Input:  ENTITY_PTR  — entity whose position to measure from
        ;         CHAR_PTR    — bytes 6-7 of record hold event list pointer
        ;
        ; Output: A = packed position of nearest event
        ;
        ; Behavior:
        ;   Walks the 3-byte event list (position, type, data).  Skips
        ;   activated events (type >= $C0) and empty slots ($FE).  For
        ;   each candidate, computes Manhattan distance and keeps the
        ;   closest.
        ;
        ; Modifies: NEAREST_DIST, NEAREST_POS, CANDIDATE_POS, EVENT_PTR
        ; Clobbers: A, X, Y

        STOB    #$7F,NEAREST_DIST      ; initial min = 127
        LDWIY    CHAR_PTR,#CFIELD_EVENTS,EVENT_PTR ; load event list pointer
        JMP     .check_end

        ; --- evaluate one event ---
    _FIND_NEAREST_EVENT.eval:
        LDY     #EVFIELD_TYPE
        LDA     (EVENT_PTR),Y          ; event type byte
        BAGE    #EVENT_ACTIVATED,_FIND_NEAREST_EVENT.advance ; activated -> skip
        LDY     #EVFIELD_POSITION
        LDA     (EVENT_PTR),Y          ; event position byte
        STA     CANDIDATE_POS
        JSR     MANHATTAN_DISTANCE
        BAPL    NEAREST_DIST,_FIND_NEAREST_EVENT.advance  ; not closer -> skip
        STA     NEAREST_DIST           ; new minimum distance
        MOVB    CANDIDATE_POS,NEAREST_POS ; record its position

        ; --- advance to next entry ---
    _FIND_NEAREST_EVENT.advance:
        LDA     EVENT_PTR
        CLC
        ADC     #EVENT_RECORD_SIZE     ; next 3-byte entry
        STA     EVENT_PTR
        BCC     .check_end
        INC     EVENT_PTR+1

        ; --- check list termination ---
    .check_end:
        LDY     #EVFIELD_POSITION
        LDA     (EVENT_PTR),Y
        BAEQ    #EVENT_EMPTY,_FIND_NEAREST_EVENT.advance  ; empty slot -> skip
        BANE    #EVENT_END,_FIND_NEAREST_EVENT.eval       ; not end -> evaluate
        LDA     NEAREST_POS            ; return nearest position
        RTS
        ORG     $13DB
    MANHATTAN_DISTANCE:
        SUBROUTINE MANHATTAN_DISTANCE
        ; Compute Manhattan distance between a position and the current entity.
        ;
        ; Input:  A = packed map position to measure from
        ; Output: A = |col_diff| + |row_diff|
        ;
        ; Uses ENTITY_PTR byte MFIELD_POSITION as the entity's position.
        ; FONT_COL/FONT_ROW and DIST_COL used as scratch.
        ; Clobbers: A, X, Y

        ; --- unpack target position ---
        JSR     POS_TO_COLROW          ; A -> col in A, row in Y
        STA     FONT_COL               ; save target col
        STY     FONT_ROW               ; save target row

        ; --- unpack entity position ---
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y         ; entity's packed position
        JSR     POS_TO_COLROW

        ; --- |col difference| ---
        SEC
        SBC     FONT_COL               ; entity col - target col
        BCS     .pos1
        EOR     #$FF                   ; \ negate if negative
        ADC     #$01                   ; / (two's complement abs)
    .pos1:
        STA     DIST_COL               ; save |delta col|

        ; --- |row difference| ---
        TYA
        SEC
        SBC     FONT_ROW               ; entity row - target row
        BCS     .pos2
        EOR     #$FF                   ; \ negate if negative
        ADC     #$01                   ; /
    .pos2:
        CLC
        ADC     DIST_COL               ; A = |delta col| + |delta row|
        RTS

    ...

        ORG     $6742
    NPC_MOVE_AI:
        SUBROUTINE NPC_MOVE_AI
        ; Choose a movement direction for an NPC entity.
        ;
        ; Input:  ENTITY_PTR     — entity to move
        ;         CHAR_AI_MODE   — AI mode ($04 = forced movement)
        ;         AI_TARGET_POS  — desired target position (0 = none)
        ;         AI_PREV_DIR    — previous direction (penalized to avoid loops)
        ;
        ; Output: A = chosen direction index (0-4)
        ;         AI_PREV_DIR updated
        ;
        ; Behavior:
        ;   Initializes DIR_SCORES for 5 options (stay + 4 directions).
        ;   Evaluates each direction via EVALUATE_DIRECTION, which penalizes
        ;   walls, entities, and non-improving moves.  Scores are scaled,
        ;   randomized, and the lowest-scoring direction is chosen.  The
        ;   result is XOR-encoded to produce a direction value.
        ;
        ; Modifies: DIR_SCORES, AI_DIRECTION, AI_TEST_POS, AI_BEST_DIST,
        ;           AI_SAVED_POS, AI_MIN_SCORE, AI_BEST_DIR, AI_PREV_DIR
        ; Clobbers: A, X, Y

        JSR     ANIM_TICK_AND_WAIT

        ; --- check if AI should run ---
        LDA     CHAR_AI_MODE
        BAEQ    #$04,_NPC_MOVE_AI.do_ai            ; mode 4 = forced movement
        LDA     AI_TARGET_POS
        BANE    #$00,_NPC_MOVE_AI.do_ai            ; has target -> run AI
        RTS                            ; no target, not forced -> skip

        ; --- initialize direction scores ---
    _NPC_MOVE_AI.do_ai:
        STOB    #$00,DIR_SCORES+1      ; \ clear scores for 4 directions
        STA     DIR_SCORES+2           ;  |
        STA     DIR_SCORES+3           ;  |
        STA     DIR_SCORES+4           ; /
        STOB    #$0A,DIR_SCORES        ; stay score = 10 (mild penalty)
        LDA     AI_TARGET_POS
        JSR     MANHATTAN_DISTANCE
        STA     AI_BEST_DIST           ; current distance to target
        BAEQ    #$00,_NPC_MOVE_AI.no_target
        STOB    #$13,DIR_SCORES        ; stay score = 19 (strong penalty if has target)
    _NPC_MOVE_AI.no_target:
        LDX     AI_PREV_DIR            ; \ penalize previous direction (+3)
        BEQ     .no_prev               ;  | to avoid back-and-forth loops
        INC     DIR_SCORES,X           ;  |
        INC     DIR_SCORES,X           ;  |
        INC     DIR_SCORES,X           ; /
    .no_prev:

        ; --- evaluate 4 directions ---
        STOB    #$01,AI_DIRECTION      ; direction 1: up
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y
        SEC
        SBC     #MAP_WIDTH             ; position - 20 = one row up
        STA     AI_TEST_POS
        JSR     EVALUATE_DIRECTION

        INC     AI_DIRECTION           ; direction 2: down
        LDA     AI_TEST_POS
        CLC
        ADC     #MAP_WIDTH*2           ; position + 20 = one row down (from up)
        STA     AI_TEST_POS
        JSR     EVALUATE_DIRECTION

        INC     AI_DIRECTION           ; direction 3: left
        LDA     AI_TEST_POS
        SEC
        SBC     #MAP_WIDTH+1           ; back to center, then -1 = one col left
        STA     AI_TEST_POS
        JSR     EVALUATE_DIRECTION

        INC     AI_DIRECTION           ; direction 4: right
        INC     AI_TEST_POS            ; \ position + 2 = one col right (from left)
        INC     AI_TEST_POS            ; /
        JSR     EVALUATE_DIRECTION

        ; --- scale scores and add randomness ---
        STOB    #AI_RANDOM_NORMAL,AI_DIRECTION ; reuse AI_DIRECTION as random range
        LDA     CHAR_AI_MODE
        BANE    #$04,_NPC_MOVE_AI.not_phase4
        STOB    #AI_RANDOM_PHASE4,AI_DIRECTION ; wider randomness in phase 4
    _NPC_MOVE_AI.not_phase4:
        LDX     #$04
        CLC
    .score_loop:
        LDA     DIR_SCORES,X           ; \ scale score x4
        ROL2                           ; /
        STA     DIR_SCORES,X
        LDA     AI_DIRECTION           ; random range
        JSR     RANDOM_IN_RANGE
        CLC
        ADC     DIR_SCORES,X           ; add random offset
        STA     DIR_SCORES,X
        DEX
        BPL     .score_loop

        ; --- find direction with lowest score ---
        STOB    #$7F,AI_MIN_SCORE      ; initial min = 127
        LDX     #$04
    .find_min:
        LDA     DIR_SCORES,X
        BAGE    AI_MIN_SCORE,_NPC_MOVE_AI.not_lower
        STA     AI_MIN_SCORE           ; new minimum
        STX     AI_BEST_DIR            ; record direction
    _NPC_MOVE_AI.not_lower:
        DEX
        BPL     .find_min

        ; --- encode direction index to movement value ---
        LDA     AI_BEST_DIR
        BAPL    #$03,_NPC_MOVE_AI.ge3
        EOR     #$03                   ; directions 0-2: XOR 3
        BPL     .store_dir
    _NPC_MOVE_AI.ge3:
        EOR     #$07                   ; directions 3-4: XOR 7
    .store_dir:
        STA     AI_PREV_DIR            ; save for next turn's bias
        LDA     AI_BEST_DIR            ; return direction index
        RTS
        ORG     $6815
    EVALUATE_DIRECTION:
        SUBROUTINE EVALUATE_DIRECTION
        ; Score a candidate direction for NPC movement AI.
        ;
        ; Input:  AI_TEST_POS    — candidate position to evaluate
        ;         AI_TARGET_POS  — AI's desired target position
        ;         AI_DIRECTION   — index into DIR_SCORES for this direction
        ;         AI_BEST_DIST   — current best distance to target
        ;         ENTITY_PTR     — entity being moved
        ;
        ; Output: DIR_SCORES[AI_DIRECTION] updated with penalty points
        ;
        ; Behavior:
        ;   Penalizes the direction based on: already at target (+9),
        ;   not closer than current best (+2), wall (+7), and entity
        ;   type at the candidate square (friendly +3, hostile +1..+6,
        ;   locked event +7, safe event +1).
        ;
        ; Modifies: DIR_SCORES, AI_SAVED_POS
        ; Clobbers: A, X, Y

        ; --- check if already at target ---
        LDA     AI_TARGET_POS
        BAEQ    AI_TEST_POS,_EVALUATE_DIRECTION.skip_target
        LDY     #$09                   ; heavy penalty: already there
        JSR     INC_MOB_COUNTERS

        ; --- check distance improvement ---
    _EVALUATE_DIRECTION.skip_target:
        LDY     #MFIELD_POSITION       ; \ save entity's current position
        LDA     (ENTITY_PTR),Y         ;  |
        STA     AI_SAVED_POS           ; /
        LDA     AI_TEST_POS            ; \ temporarily set entity to test pos
        STA     (ENTITY_PTR),Y         ; / (for MANHATTAN_DISTANCE)
        LDA     AI_TARGET_POS
        BEQ     _EVALUATE_DIRECTION.no_distance           ; no target -> skip distance check
        JSR     MANHATTAN_DISTANCE
        BALT    AI_BEST_DIST,_EVALUATE_DIRECTION.no_distance ; closer -> no penalty
        LDY     #$02                   ; penalty: not closer
        JSR     INC_MOB_COUNTERS

        ; --- restore position and check occupant ---
    _EVALUATE_DIRECTION.no_distance:
        LDY     #MFIELD_POSITION       ; \ restore original position
        LDA     AI_SAVED_POS           ;  |
        STA     (ENTITY_PTR),Y         ; /
        LDA     AI_TEST_POS
        JSR     FIND_ENTITY_AT_POS     ; what's at the candidate square?
        LDA     ENTITY_REC+1
        BNE     .has_entity            ; nonzero hi byte -> entity found
        LDA     ENTITY_REC
        BNE     .wall                  ; nonzero lo byte -> wall/obstacle
        RTS                            ; empty square -> no penalty

    .wall:
        LDY     #$07                   ; heavy penalty: wall
        JMP     INC_MOB_COUNTERS

        ; --- classify entity by type byte ---
    .has_entity:
        BALT    #$80,_EVALUATE_DIRECTION.mob              ; bit 7 clear -> mob
        RTS                            ; bit 7 set -> player char, no penalty

    _EVALUATE_DIRECTION.mob:
        LDY     #MFIELD_TYPE
        LDA     (ENTITY_REC),Y        ; entity type/class byte
        BAGE    #$80,_EVALUATE_DIRECTION.ge80
        LDY     #$03                   ; type < $80: friendly mob
        JMP     INC_MOB_COUNTERS
    _EVALUATE_DIRECTION.ge80:
        BAGE    #$C0,_EVALUATE_DIRECTION.geC0
        LDY     #$06                   ; type $80-$BF: hostile (strong)
        JMP     INC_MOB_COUNTERS
    _EVALUATE_DIRECTION.geC0:
        BAGE    #$D2,_EVALUATE_DIRECTION.geD2
        LDY     #$01                   ; type $C0-$D1: hostile (weak)
        JMP     INC_MOB_COUNTERS
    _EVALUATE_DIRECTION.geD2:
        BAGE    #$D5,_EVALUATE_DIRECTION.geD5
        LDY     #$02                   ; type $D2-$D4: hostile (medium)
        JMP     INC_MOB_COUNTERS
    _EVALUATE_DIRECTION.geD5:
        BAGE    #$D8,_EVALUATE_DIRECTION.geD8
        LDY     #$03                   ; type $D5-$D7: shop/special
        JMP     INC_MOB_COUNTERS

        ; --- event entity: check lock status ---
    _EVALUATE_DIRECTION.geD8:
        INY                            ; Y = MFIELD_NEXT (byte 2: event data for type >= $D8)
        LDA     (ENTITY_REC),Y
        BAGE    #$40,_EVALUATE_DIRECTION.safe
        LDY     #$07                   ; locked event: heavy penalty
        JMP     INC_MOB_COUNTERS
    _EVALUATE_DIRECTION.safe:
        LDY     #$01                   ; safe event: light penalty
    ; falls through to INC_MOB_COUNTERS
        ORG     $689F
    INC_MOB_COUNTERS:
        SUBROUTINE INC_MOB_COUNTERS
        ; Add Y to the direction penalty score for the current AI direction.
        ;
        ; Input:  Y             — penalty count (decremented to zero)
        ;         AI_DIRECTION  — index into DIR_SCORES array
        ;
        ; Output: DIR_SCORES[AI_DIRECTION] incremented by Y
        ; Clobbers: X, Y, flags

        LDX     AI_DIRECTION           ; current direction index
        DEY                            ; pre-decrement (Y=0 -> done)
        BPL     .do_inc
        RTS
    .do_inc:
        INC     DIR_SCORES,X           ; add one penalty point
        BPL     INC_MOB_COUNTERS       ; loop until Y < 0
        ORG     $68AB
    AI_CHOOSE_TARGET:
        SUBROUTINE AI_CHOOSE_TARGET
        ; Select a movement target for an NPC based on its AI mode.
        ;
        ; Input:  ENTITY_PTR    — NPC entity record
        ;         CHAR_PTR      — character index record
        ;         MOB_PTR       — mob data pointer (set by group iteration)
        ;         CHAR_AI_MODE  — AI behavior mode (0-5+)
        ;
        ; Output: AI_TARGET_POS — packed position of chosen target
        ;         AI_TARGET_COL — target column
        ;         AI_TARGET_ROW — target row
        ;         AI_PREV_DIR   — cleared to 0
        ;
        ; Behavior:
        ;   Mode 0/1: attempt to join player's party; if no match,
        ;     fall through to mode 2.
        ;   Mode 2: if strength > HP, target nearest event;
        ;     otherwise no target.
        ;   Mode 3: scan group for nearest hostile mob.
        ;   Mode 4: no target (forced movement by NPC_MOVE_AI).
        ;   Mode 5+: scan group for strongest hostile mob.
        ;
        ; Modifies: AI_TARGET_POS, AI_TARGET_COL, AI_TARGET_ROW,
        ;           AI_PREV_DIR, AI_BEST_POS, AI_BEST_STR, TARGET_REC
        ; Clobbers: A, X, Y

        ; --- dispatch by AI mode ---
        STOB    #$00,AI_PREV_DIR
        LDA     CHAR_AI_MODE
        BAPL    #$02,_AI_CHOOSE_TARGET.mode_ge2

        ; --- mode 0/1: check if NPC should join party ---
        JSR     PICK_RANDOM_MEMBER
        LDA     MOB_PTR+1
        BAEQ    #$00,_AI_CHOOSE_TARGET.simple_target    ; no match -> simple target
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_JOINS_PARTY                   ; "joins party" message
        JSR     DISPLAY_MESSAGE
        JSR     PRINT_MEMBER_NAME
        JSR     SCROLL_STATUS_LINE
        JSR     DELAY_WITH_ANIMATION
        LDA     #MSG_THANK_YOU                   ; confirmation message
        JSR     DISPLAY_MESSAGE
        STOB    #$00,CHAR_AI_MODE      ; switch to player control
        LDY     #MFIELD_APPEARANCE     ; \ adjust appearance (remove NPC flag)
        LDA     (ENTITY_PTR),Y         ;  |
        SEC                            ;  |
        SBC     #NPC_APPEARANCE_MIN                   ;  |
        STA     (ENTITY_PTR),Y         ; /
        LDY     #MFIELD_FLAGS          ; \ copy class bits (7-6) from source
        LDA     (MOB_PTR),Y            ;  | to entity's flags field
        AND     #FLAGS_CLASS_MASK      ;  |
        STA     AI_SCRATCH             ;  |
        LDA     (ENTITY_PTR),Y         ;  |
        AND     #FLAGS_STATUS_MASK     ;  | keep status bits (5-0)
        ORA     AI_SCRATCH             ;  | merge class bits from source
        STA     (ENTITY_PTR),Y         ; /
        LDY     #CFIELD_XP             ; \ increment party size in group record
        LDA     (CHAR_PTR),Y           ;  |
        CLC                            ;  |
        ADC     #$01                   ;  |
        STA     (CHAR_PTR),Y           ; /
        INC     TOTAL_MOB_COUNT        ; increment global count
        RTS

        ; --- mode 2: compare strength vs HP for target ---
    _AI_CHOOSE_TARGET.mode_ge2:
        BNE     .mode_ge3
    _AI_CHOOSE_TARGET.simple_target:
        LDY     #MFIELD_STRENGTH       ; strength (bits 4-0)
        LDA     (ENTITY_PTR),Y
        AND     #STRENGTH_MASK
        STA     AI_SCRATCH
        INY                            ; Y = MFIELD_HP
        LDA     (ENTITY_PTR),Y        ; HP (bits 5-0)
        AND     #HP_MASK
        BAPL    AI_SCRATCH,_AI_CHOOSE_TARGET.use_encounter ; HP >= strength -> seek encounter
        STOB    #$00,AI_TARGET_POS     ; no target
        JMP     .done
    _AI_CHOOSE_TARGET.use_encounter:
        JSR     FIND_NEAREST_EVENT
        STA     AI_TARGET_POS
        JMP     .done

        ; --- mode 3: find nearest hostile ---
    .mode_ge3:
        BAPL    #$04,_AI_CHOOSE_TARGET.mode_ge4
        MOVW    ENTITY_PTR,TARGET_REC
        STOB    #$3C,AI_TARGET_COL     ; initial min distance = 60
        JSR     FIRST_GROUP_MEMBER
    .loop3:
        JSR     CHECK_HOSTILE
        BAEQ    #$00,_AI_CHOOSE_TARGET.skip3            ; not hostile -> skip
        LDY     #MFIELD_POSITION
        LDA     (MOB_PTR),Y
        JSR     MANHATTAN_DISTANCE
        BAGE    AI_TARGET_COL,_AI_CHOOSE_TARGET.skip3   ; not closer -> skip
        STA     AI_TARGET_COL          ; new minimum distance
        LDY     #MFIELD_POSITION
        LDA     (MOB_PTR),Y
        STA     AI_TARGET_POS          ; target = this mob's position
    _AI_CHOOSE_TARGET.skip3:
        JSR     NEXT_GROUP_MEMBER
        LDA     MOB_PTR+1
        BNE     .loop3
        LDA     AI_TARGET_COL
        BANE    #$3C,_AI_CHOOSE_TARGET.found3           ; found one (distance changed from $3C)
        MOVB    EVENT_POS,AI_TARGET_POS ; no hostile -> fallback to event
    _AI_CHOOSE_TARGET.found3:
        JMP     .done

        ; --- mode 4: no target (forced movement) ---
    _AI_CHOOSE_TARGET.mode_ge4:
        BNE     .mode_gt4
        STOB    #$00,AI_TARGET_POS
        JMP     .done

        ; --- mode 5+: find strongest hostile ---
    .mode_gt4:
        STOB    #$00,AI_BEST_STR       ; max strength seen = 0
        MOVW    ENTITY_PTR,TARGET_REC
        JSR     FIRST_GROUP_MEMBER
    .loop5:
        JSR     CHECK_HOSTILE
        BAEQ    #$00,_AI_CHOOSE_TARGET.skip5            ; not hostile -> skip
        LDY     #MFIELD_STRENGTH
        LDA     (MOB_PTR),Y
        AND     #STRENGTH_MASK         ; strength (bits 4-0)
        BAMI    AI_BEST_STR,_AI_CHOOSE_TARGET.skip5     ; not stronger -> skip
        STA     AI_BEST_STR            ; new max strength
        LDY     #MFIELD_POSITION
        LDA     (MOB_PTR),Y
        STA     AI_BEST_POS            ; record position
    _AI_CHOOSE_TARGET.skip5:
        JSR     NEXT_GROUP_MEMBER
        LDA     MOB_PTR+1
        BNE     .loop5
        LDA     AI_BEST_STR
        BEQ     .no_target5            ; none found
        MOVB    AI_BEST_POS,AI_TARGET_POS
        JMP     .done
    .no_target5:
        MOVB    EVENT_POS,AI_TARGET_POS ; fallback to event
        JMP     .done

        ; --- convert target to col/row ---
    .done:
        LDA     AI_TARGET_POS
        JSR     POS_TO_COLROW
        STA     AI_TARGET_COL
        STY     AI_TARGET_ROW
        RTS

    ...

        ORG     $70DF
    DISPLAY_SHOP:
        SUBROUTINE DISPLAY_SHOP
        ; Display the shop menu for a trading post entity.
        ;
        ; Inputs:
        ;   CHAR_AI_MODE   — if nonzero, returns immediately (AI can't shop)
        ;   ENTITY_REC     — pointer to shop entity record:
        ;                     +SHOP_TYPE_FIELD: shop type ($D5=weapons, $D6=armor)
        ;                     +SHOP_AVAIL_FIELD: item validity bitmask
        ;   LOCATION_STYLE — bits 7-6 encode location tier (0-3), scales prices
        ;
        ; Behavior:
        ;   Selects the shop record from SHOP_TABLE, displays the shop scene,
        ;   then iterates 8 item slots.  For each valid slot (per bitmask),
        ;   computes price = base_price * (tier + 1), formats the
        ;   "PAY nnnnnn FOR [item]" string, and enters START_GAME_INPUT.
        ;
        ; Modifies:
        ;   SHOP_DATA_PTR, SHOP_VALID_MASK, SHOP_ITEM_MASK, SHOP_ITEM_OFFSET,
        ;   SHOP_PRICE, SHOP_PRICE+1, LOCATION_STYLE_BITS,
        ;   SHOP_PRICE_DIGITS, SHOP_ITEM_NAME, PRINT_STRING_ADDR
        ; Clobbers: A, X, Y

        LDA     CHAR_AI_MODE
        BEQ     .do_shop
        RTS
    .do_shop:
        STOW    SHOP_TABLE,SHOP_DATA_PTR
        LDY     #SHOP_TYPE_FIELD       ; shop type ($D5=weapons, $D6=armor)
        LDA     (ENTITY_REC),Y
        SEC
        SBC     #$D5                   ; record index (0 or 1)
        TAX
    .count_loop:
        DEX
        BMI     .counted
        JSR     ADVANCE_SHOP_PTR       ; skip to selected record
        JMP     .count_loop
    .counted:
        LDY     #SHOP_AVAIL_FIELD      ; item validity bitmask
        LDA     (ENTITY_REC),Y
        STA     SHOP_VALID_MASK
        JSR     SET_CURSOR_ROW21
        LDY     #SHOP_SCENE_OFFSET     ; display shop scene
        LDA     (SHOP_DATA_PTR),Y
        JSR     SCENE_LOOP
        JSR     LOAD_CHAR_NAME_PTR
        JSR     SETUP_CHAR_SPRITE

        ; --- iterate through item slots ---
    .item_start:
        STOB    #$80,SHOP_ITEM_MASK    ; start at slot 0 (bit 7)
        STOB    #$00,SHOP_ITEM_OFFSET  ; start at record offset 0
    .item_loop:
        LDA     SHOP_VALID_MASK        ; check if current slot is valid
        AND     SHOP_ITEM_MASK
        BNE     .show_item
        JMP     .next_item

        ; --- display one item ---
    .show_item:
        STOB    #$00,SHOP_PRICE        ; clear price accumulator
        STA     SHOP_PRICE+1
        LDY     SHOP_ITEM_OFFSET       ; Y = offset + 4 (price byte)
        INY
        INY
        INY
        INY
        LDA     LOCATION_STYLE         ; \ extract bits 7-6: location tier (0-3)
        ROL2                           ;  | rotate down to bits 1-0
        ROL2                           ;  |
        AND     #$03                   ; /
        TAX                            ; X = tier (price loop count)
        STA     LOCATION_STYLE_BITS
    .price_loop:                       ; price = base_price * (tier + 1)
        JSR     ADD_PRICE_COMPONENT
        DEX
        BPL     .price_loop
        LDA     #$20                   ; \ clear price and item name fields
        LDX     #$04                   ;  | in "PAY 000000 FOR" string
    .clear1:                           ;  |
        STA     SHOP_PRICE_DIGITS,X    ;  |
        DEX                            ;  |
        BPL     .clear1                ;  |
        LDX     #$13                   ;  |
    .clear2:                           ;  |
        STA     SHOP_ITEM_NAME,X       ;  |
        DEX                            ;  |
        BPL     .clear2                ; /
        MOVB    SHOP_PRICE,PRINT_STRING_ADDR
        MOVB    SHOP_PRICE+1,PRINT_STRING_ADDR+1
        STOW    SHOP_PRICE_DIGITS,$BA
        JSR     NUM_TO_DECIMAL         ; write price digits
        LDY     SHOP_ITEM_OFFSET       ; \ copy item name string address
        INY                            ;  | from shop record (+2/+3)
        INY                            ;  | into PRINT_STRING_ADDR
        LDA     (SHOP_DATA_PTR),Y      ;  |
        STA     PRINT_STRING_ADDR      ;  |
        INY                            ;  |
        LDA     (SHOP_DATA_PTR),Y      ;  |
        STA     PRINT_STRING_ADDR+1    ; /
        LDY     #$00                   ; \ copy item name into display string
        LDA     (PRINT_STRING_ADDR),Y  ;  | Y = string length
        TAY                            ;  |
        TAX                            ;  |
        DEX                            ;  | X = length - 1 (dest index)
    .copy_name:                        ;  |
        LDA     (PRINT_STRING_ADDR),Y  ;  |
        STA     SHOP_ITEM_NAME,X       ;  |
        DEY                            ;  |
        DEX                            ;  |
        BPL     .copy_name             ; /
        JMP     START_GAME_INPUT       ; enter buy/browse/no-sale menu

        ; --- advance to next slot ---
    BROWSE_HANDLER:                        ; entry from "BUY SOMETHING ELSE" menu
    .next_item:
        CLC
        LDA     SHOP_ITEM_MASK         ; rotate mask right
        ROR
        BCC     .more_items            ; didn't carry out -> more slots
        JMP     .item_start            ; carried out -> restart
    .more_items:
        STA     SHOP_ITEM_MASK
        LDA     SHOP_ITEM_OFFSET       ; advance offset by 5
        ADC     #$05
        STA     SHOP_ITEM_OFFSET
        JMP     .item_loop
        ORG     $71AC
    SHOP_BUY_HANDLER:
        SUBROUTINE SHOP_BUY_HANDLER
        ; Handle the "buy" action in the shop menu.
        ;
        ; Inputs:
        ;   ENTITY_PTR — buyer's entity record
        ;   SHOP_PRICE/+1 — item price
        ;   SHOP_DATA_PTR — pointer to current shop record
        ;   SHOP_ITEM_OFFSET — offset to current item's handler
        ;
        ; Behavior:
        ;   Compares gold (bytes $0D/$0E) against SHOP_PRICE.
        ;   If gold < price, displays "can't afford" and returns.
        ;   Otherwise deducts price and jumps to item effect handler.
        ;
        ; Clobbers: A, X, Y

        ; --- Compare gold against price ---
        LDY     #MFIELD_GOLD+1         ; byte $0E: gold high
        LDA     (ENTITY_PTR),Y
        CMP     SHOP_PRICE+1           ; compare high byte
        BCC     .cant_afford           ; gold < price
        BNE     .can_afford            ; gold > price
        DEY                            ; byte $0D: gold low
        LDA     (ENTITY_PTR),Y
        CMP     SHOP_PRICE             ; compare low byte
        BCS     .can_afford
    .cant_afford:
        JSR     SET_CURSOR_ROW21
        LDA     #$31                   ; message: "can't afford"
        JSR     DISPLAY_MESSAGE
        JMP BROWSE_HANDLER                  ; return to shop menu
    .can_afford:
        ; --- Deduct price and execute purchase ---
        LDA     SHOP_PRICE             ; \ NUM_VALUE = price
        STA     NUM_VALUE              ;  |
        LDA     SHOP_PRICE+1           ;  |
        STA     NUM_VALUE+1            ; /
        LDY     #$00
        JSR MODIFY_CURRENT_STATS                  ; deduct price from gold
        JSR     SET_CURSOR_ROW21
        LDA     #$26                   ; message: "bought"
        JSR     DISPLAY_MESSAGE
        LDY     SHOP_ITEM_OFFSET       ; \ load item effect handler
        LDA     (SHOP_DATA_PTR),Y      ;  | from shop record
        STA     NUM_VALUE              ;  |
        INY                            ;  |
        LDA     (SHOP_DATA_PTR),Y      ;  |
        STA     NUM_VALUE+1            ; /
        JMP     (NUM_VALUE)            ; execute item effect
        ORG     $71F0
    ADVANCE_SHOP_PTR:
        SUBROUTINE ADVANCE_SHOP_PTR
        ; Advance SHOP_DATA_PTR to the next shop record.
        ;
        ; Input:  SHOP_DATA_PTR — current record
        ; Output: SHOP_DATA_PTR += SHOP_RECORD_SIZE (41 bytes)
        ; Clobbers: A

        CLC
        LDA     SHOP_DATA_PTR
        ADC     #SHOP_RECORD_SIZE
        STA     SHOP_DATA_PTR
        BCC     .done
        INC     SHOP_DATA_PTR+1
    .done:
        RTS
        ORG     $71FC
    ADD_PRICE_COMPONENT:
        SUBROUTINE ADD_PRICE_COMPONENT
        ; Add one price byte from shop record to SHOP_PRICE.
        ;
        ; Input:  Y = offset into shop record, SHOP_DATA_PTR
        ; Output: SHOP_PRICE += (SHOP_DATA_PTR),Y
        ; Clobbers: A

        CLC
        LDA     (SHOP_DATA_PTR),Y
        ADC     SHOP_PRICE
        STA     SHOP_PRICE
        BCC     .done
        INC     SHOP_PRICE+1
    .done:
        RTS
        ORG     $720B
    SHOP_BUY_EFFECT_0:
        LDX     #$00
        LDY     #MFIELD_ATTACK     ; attack stat (swords, clubs)
        BNE     APPLY_SHOP_EFFECT

    SHOP_BUY_EFFECT_1:
        LDX     #$01
        LDY     #MFIELD_ATTACK
        BNE     APPLY_SHOP_EFFECT

    SHOP_BUY_EFFECT_2:
        LDX     #$02
        LDY     #MFIELD_ATTACK
        BNE     APPLY_SHOP_EFFECT

    SHOP_BUY_EFFECT_3:
        LDX     #$03
        LDY     #MFIELD_DEFENSE    ; defense stat (armor, daggers, shivs)
        BNE     APPLY_SHOP_EFFECT

    SHOP_BUY_EFFECT_4:
        LDX     #$04
        LDY     #MFIELD_DEFENSE
        BNE     APPLY_SHOP_EFFECT

    APPLY_SHOP_EFFECT:
        LDA     SHOP_EFFECT_TABLE,X ; base effect value
        CLC
        ADC     LOCATION_STYLE_BITS ; + location tier modifier
        STA     (ENTITY_PTR),Y     ; store to weapon/armor field
        TYA                        ; \ save target field offset
        TAX                        ; / in X
        LDY     #MFIELD_STRENGTH
        LDA     (ENTITY_PTR),Y     ; strength/defense byte
        ROR                        ; \ extract bits 2-1
        ROR                        ;  | as bits 1-0
        ROR                        ; /
        AND     #$03               ; mask to 2 bits (defense bonus)
        PHA
        TXA                        ; restore target field offset
        TAY
        PLA
        CLC
        ADC     (ENTITY_PTR),Y     ; add defense bonus
        STA     (ENTITY_PTR),Y
        JMP     BROWSE_HANDLER     ; return to browse handler
        ORG     $7249
    XP_AWARD_0:
        LDX     #$00
        BEQ     AWARD_XP

    XP_AWARD_1:
        LDX     #$01
        BNE     AWARD_XP

    XP_AWARD_2:
        LDX     #$02
        BNE     AWARD_XP

    XP_AWARD_3:
        LDX     #$03
        BNE     AWARD_XP

    XP_AWARD_4:
        LDX     #$04
        BNE     AWARD_XP

    AWARD_XP:
        SUBROUTINE AWARD_XP
        ; Award experience points after a shop buy action.
        ;
        ; Inputs:
        ;   X              — action type index (0-4), set by entry points
        ;   ENTITY_PTR     — pointer to entity receiving the XP
        ;   XP_AWARD_TABLE — base XP values indexed by action type
        ;   LOCATION_STYLE_BITS — location tier modifier (0-3)
        ;
        ; Behavior:
        ;   Extracts current level and pending XP from entity record.
        ;   Adds the award value (table[X] + tier) to byte 8, then
        ;   clamps so pending XP does not exceed the accumulated level.
        ;   Writes the updated level back to byte 11 and jumps to the
        ;   browse handler.
        ;
        ; Modifies:
        ;   ENTITY_PTR bytes 8 and 11, XP_ACCUM
        ; Clobbers: A, X, Y

        ; --- extract current level and pending XP ---
        LDY     #MFIELD_LEVEL    ; byte 11: flags|level
        LDA     (ENTITY_PTR),Y
        PHA
        AND     #LEVEL_MASK            ; level (bits 4-0)
        STA     XP_ACCUM
        PLA
        AND     #LEVEL_FLAG_MASK       ; preserve flag bits (7-5)
        STA     (ENTITY_PTR),Y         ; clear level in record
        LDY     #MFIELD_XP       ; byte 8: status|pending_xp|count
        LDA     (ENTITY_PTR),Y
        JSR     EXTRACT_PENDING_XP     ; A = pending XP (0-7)
        CLC
        ADC     XP_ACCUM               ; accumulated = level + pending
        STA     XP_ACCUM

        ; --- add XP award and clamp ---
        LDA     XP_AWARD_TABLE,X       ; base XP for action type
        CLC
        ADC     LOCATION_STYLE_BITS    ; + location tier modifier
    .clamp:
        STA     (ENTITY_PTR),Y         ; write to byte 8
        JSR     EXTRACT_PENDING_XP     ; extract new pending XP
        CMP     XP_ACCUM               ; exceeds accumulated level?
        BCC     .ok                    ; no -> done
        SEC                            ; \ yes -> reduce by one XP unit
        LDA     (ENTITY_PTR),Y         ;  |
        SBC     #PENDING_XP_UNIT       ;  |
        JMP     .clamp                 ; / retry

        ; --- write updated level back ---
    .ok:
        TAX                            ; X = new pending XP
        LDA     XP_ACCUM
        STX     XP_ACCUM
        SEC
        SBC     XP_ACCUM               ; remaining level = accumulated - pending
        LDY     #MFIELD_LEVEL    ; merge with saved flag bits
        ORA     (ENTITY_PTR),Y
        STA     (ENTITY_PTR),Y
        JMP     BROWSE_HANDLER

    EXTRACT_PENDING_XP:
        ; Extract the pending XP field from a packed entity byte.
        ;
        ; Input:  A = entity byte 8 (status|pending_xp|count)
        ; Output: A = pending XP value (0-7), from bits 6-4
        ; Clobbers: flags
        ROR                            ; \ extract bits 6-4 of A
        ROR                            ;  | shift right 4 positions
        ROR                            ;  |
        ROR                            ; /
        AND     #$07                   ; mask to 3 bits (0-7)
        RTS
*/
#include "ali_baba.h"

/* CHAR_AI_MODE's modes. Modes 0 and 1 may join the party. */
enum {
    AI_MODE_SEEK_ENCOUNTER = 2,
    AI_MODE_HUNT_NEAREST = 3,
    AI_MODE_WANDER = 4,
    /* 5 and up hunt the strongest hostile mob. */
};

/* The random amount npc_move_ai adds to each score is 0 to this. */
constexpr uint8_t AI_RANDOM_NORMAL = 0x06;
constexpr uint8_t AI_RANDOM_WANDER = 0x0F;

/* The class bits of a mob's flags, and the rest. */
constexpr uint8_t FLAGS_CLASS_MASK = 0xC0;
constexpr uint8_t FLAGS_STATUS_MASK = 0x3F;

/* The flag bits above the level in a mob's level byte. */
constexpr uint8_t LEVEL_FLAG_MASK = 0xE0;
/* One unit of the pending level amount, bits 6-4 of a mob's armor. */
constexpr uint8_t PENDING_XP_UNIT = 0x10;

/* The low-ASCII space that fills S_PAY_FOR's fields before they are
 * written, since num_to_decimal leaves the places of leading zeros as
 * they are. */
constexpr uint8_t LOW_SPACE = 0x20;

uint8_t manhattan_distance(uint8_t pos, uint8_t from) {
    uint16_t to = pos_to_colrow(pos);
    uint8_t col = (uint8_t)to;
    uint8_t row = (uint8_t)(to >> 8);
    FONT_COL = col;
    FONT_ROW = row;

    uint16_t at = pos_to_colrow(from);
    uint8_t from_col = (uint8_t)at;
    uint8_t from_row = (uint8_t)(at >> 8);
    uint8_t dcol = from_col >= col ? from_col - col : col - from_col;
    uint8_t drow = from_row >= row ? from_row - row : row - from_row;
    return dcol + drow;
}

uint8_t find_nearest_event(void) {
    uint8_t nearest_dist = 0x7F;
    for (const struct event *ev = CHAR_PTR->events; ev->pos != EVENT_END; ev++) {
        if (ev->pos == EVENT_EMPTY || ev->type >= EVENT_ACTIVATED)
            continue;
        uint8_t pos = ev->pos;
        /* ENTER_LOCATION's ENTITY_PTR points at this byte. */
        CANDIDATE_POS = pos;
        uint8_t dist = manhattan_distance(pos, ENTITY_PTR->pos);
        if (dist < nearest_dist) {
            nearest_dist = dist;
            NEAREST_POS = pos;
        }
    }
    return NEAREST_POS;
}

/* The penalty for stepping onto event entry `ev`. Entries of types $D8
 * and up are told apart by their value. */
static uint8_t event_penalty(const struct event *ev) {
    uint8_t type = ev->type;
    if (type < 0x80)
        return 3;
    if (type < EVENT_ACTIVATED)
        return 6;
    if (type < EVENT_TYPE_SCENE)
        return 1;
    if (type < EVENT_TYPE_WEAPON_SHOP)
        return 2;
    if (type < 0xD8)
        return 3;
    return ev->value < 0x40 ? 7 : 1;
}

/* Adds to DIR_SCORES[dir] the penalties for stepping to `test_pos`. */
static void evaluate_direction(uint8_t dir, uint8_t test_pos) {
    if (AI_TARGET_POS != test_pos)
        DIR_SCORES[dir] += 9;
    /* A step that doesn't bring the mob nearer its target. */
    if (AI_TARGET_POS != 0 && manhattan_distance(AI_TARGET_POS, test_pos) >= AI_BEST_DIST)
        DIR_SCORES[dir] += 2;

    entity_ref e = find_entity_at_pos(test_pos);
    if (ENTITY_IS_MOB(e) || e == ENTITY_NONE)
        return;
    if (ENTITY_IS_EVENT(e))
        DIR_SCORES[dir] += event_penalty(ENTITY_EVENT(e));
    else
        DIR_SCORES[dir] += 7;       /* the viewport's edge */
}

/* The direction back from each direction, which the next step's scores
 * penalize. The original takes it with an XOR, which gives DIR_LEFT for
 * staying put. */
static const uint8_t REVERSE_DIR[] = {
    [DIR_NONE] = DIR_LEFT,
    [DIR_UP] = DIR_DOWN,
    [DIR_DOWN] = DIR_UP,
    [DIR_LEFT] = DIR_RIGHT,
    [DIR_RIGHT] = DIR_LEFT,
};

uint8_t npc_move_ai(void) {
    anim_tick_and_wait();
    if (CHAR_AI_MODE != AI_MODE_WANDER && AI_TARGET_POS == 0)
        return DIR_NONE;

    /* Staying put costs 10, or 19 away from the target. */
    DIR_SCORES[DIR_UP] = 0;
    DIR_SCORES[DIR_DOWN] = 0;
    DIR_SCORES[DIR_LEFT] = 0;
    DIR_SCORES[DIR_RIGHT] = 0;
    DIR_SCORES[DIR_NONE] = 10;
    AI_BEST_DIST = manhattan_distance(AI_TARGET_POS, ENTITY_PTR->pos);
    if (AI_BEST_DIST != 0)
        DIR_SCORES[DIR_NONE] = 19;
    if (AI_PREV_DIR != DIR_NONE)
        DIR_SCORES[AI_PREV_DIR] += 3;

    uint8_t pos = ENTITY_PTR->pos;
    evaluate_direction(DIR_UP, pos - MAP_WIDTH);
    evaluate_direction(DIR_DOWN, pos + MAP_WIDTH);
    evaluate_direction(DIR_LEFT, pos - 1);
    evaluate_direction(DIR_RIGHT, pos + 1);

    uint8_t range = CHAR_AI_MODE == AI_MODE_WANDER ? AI_RANDOM_WANDER : AI_RANDOM_NORMAL;
    for (uint8_t dir = DIR_RIGHT + 1; dir-- != 0;)
        DIR_SCORES[dir] = 4 * DIR_SCORES[dir] + random_in_range(range);

    /* The lowest score wins, and a tie goes to the higher direction.
     * Every score is below $7F, so some direction always wins. */
    uint8_t best_score = 0x7F;
    uint8_t best = DIR_NONE;
    for (uint8_t dir = DIR_RIGHT + 1; dir-- != 0;) {
        if (DIR_SCORES[dir] < best_score) {
            best_score = DIR_SCORES[dir];
            best = dir;
        }
    }

    AI_PREV_DIR = REVERSE_DIR[best];
    return best;
}

/* An AI mob of mode 0 or 1 joins the party of MOB_PTR, a player
 * character in the same location. */
static void join_party(void) {
    set_cursor_row21();
    display_message(MSG_JOINS_PARTY);
    print_member_name();
    scroll_status_line();
    delay_with_animation();
    display_message(MSG_THANK_YOU);
    CHAR_AI_MODE = 0;
    ENTITY_PTR->appearance -= NPC_APPEARANCE_MIN;
    ENTITY_PTR->flags = (ENTITY_PTR->flags & FLAGS_STATUS_MASK) | (MOB_PTR->flags & FLAGS_CLASS_MASK);
    CHAR_PTR->pc_count++;
    TOTAL_MOB_COUNT++;
}

void ai_choose_target(void) {
    uint8_t mode = CHAR_AI_MODE;
    AI_PREV_DIR = DIR_NONE;

    if (mode < AI_MODE_SEEK_ENCOUNTER) {
        pick_random_member();
        if (!IN_PAGE_0(MOB_PTR)) {
            join_party();
            return;
        }
        mode = AI_MODE_SEEK_ENCOUNTER;
    }

    if (mode == AI_MODE_SEEK_ENCOUNTER) {
        if ((ENTITY_PTR->life & HP_MASK) >= (ENTITY_PTR->strength & STRENGTH_MASK))
            AI_TARGET_POS = find_nearest_event();
        else
            AI_TARGET_POS = 0;
    } else if (mode == AI_MODE_HUNT_NEAREST) {
        /* AI_TARGET_COL holds the nearest distance so far. */
        AI_TARGET_COL = 0x3C;
        first_group_member();
        do {
            if (check_hostile(ENTITY_PTR)) {
                uint8_t dist = manhattan_distance(MOB_PTR->pos, ENTITY_PTR->pos);
                if (dist < AI_TARGET_COL) {
                    AI_TARGET_COL = dist;
                    AI_TARGET_POS = MOB_PTR->pos;
                }
            }
            next_group_member();
        } while (!IN_PAGE_0(MOB_PTR));
        if (AI_TARGET_COL == 0x3C)
            AI_TARGET_POS = EVENT_POS;
    } else if (mode == AI_MODE_WANDER) {
        AI_TARGET_POS = 0;
    } else {
        /* A hostile mob of strength 0 counts as none. */
        uint8_t best_strength = 0;
        uint8_t best_pos;
        first_group_member();
        do {
            if (check_hostile(ENTITY_PTR)) {
                uint8_t strength = MOB_PTR->strength & STRENGTH_MASK;
                if (strength >= best_strength) {
                    best_strength = strength;
                    best_pos = MOB_PTR->pos;
                }
            }
            next_group_member();
        } while (!IN_PAGE_0(MOB_PTR));
        AI_TARGET_POS = best_strength != 0 ? best_pos : EVENT_POS;
    }

    uint16_t colrow = pos_to_colrow(AI_TARGET_POS);
    AI_TARGET_COL = (uint8_t)colrow;
    AI_TARGET_ROW = (uint8_t)(colrow >> 8);
}

/* ---- The shops ---- */

/* The record of SHOP_TABLE for the shop open. Event types $D7-$DA
 * point past the table's two records, as in the original. */
static const struct shop *shop;

/* The slot SHOP_ITEM_OFFSET (5 times the slot's number) selects. */
static const struct shop_slot *current_slot(void) {
    return (const struct shop_slot *)((const uint8_t *)shop + SHOP_ITEM_OFFSET);
}

/* Steps to the next slot, or back to slot 0 after slot 7. */
static void next_slot(void) {
    if (SHOP_ITEM_MASK & 1) {
        SHOP_ITEM_MASK = 0x80;
        SHOP_ITEM_OFFSET = 0;
    } else {
        SHOP_ITEM_MASK >>= 1;
        SHOP_ITEM_OFFSET += sizeof(struct shop_slot);
    }
}

/* Shows the first enabled slot from the current one on: its price for
 * this location in S_PAY_FOR.price, and its name in S_PAY_FOR.item. */
static uint8_t show_item(void) {
    while ((SHOP_VALID_MASK & SHOP_ITEM_MASK) == 0)
        next_slot();

    const struct shop_slot *slot = current_slot();
    uint8_t tier = (LOCATION_STYLE >> 5) & 3;
    LOCATION_STYLE_BITS = tier;
    SHOP_PRICE = slot->price * (tier + 1);

    for (uint8_t i = 0; i < 5; i++)
        S_PAY_FOR.price[i] = LOW_SPACE;
    for (uint8_t i = 0; i < 20; i++)
        S_PAY_FOR.item[i] = LOW_SPACE;
    num_to_decimal(SHOP_PRICE, S_PAY_FOR.price);

    /* The copy runs down from the last character and ends when the
     * index turns negative, so a length of 0 copies the length byte
     * to S_PAY_FOR.item[255]. */
    const uint8_t *name = slot->name;
    uint8_t from = name[0];
    uint8_t to = from - 1;
    do {
        S_PAY_FOR.item[to] = name[from];
        from--;
        to--;
    } while ((int8_t)to >= 0);

    return CHAIN_START_GAME_INPUT;
}

uint8_t display_shop(struct event *ev) {
    if (CHAR_AI_MODE != 0)
        return CHAIN_RETURN;
    shop = &SHOP_TABLE[ev->type - EVENT_TYPE_WEAPON_SHOP];
    SHOP_VALID_MASK = ev->value;
    set_cursor_row21();
    scene_loop(shop->scene);
    load_char_name_ptr();
    setup_char_sprite();
    SHOP_ITEM_MASK = 0x80;
    SHOP_ITEM_OFFSET = 0;
    return show_item();
}

uint8_t browse_handler(void) {
    next_slot();
    return show_item();
}

/* A weapon: sets the sword (effects 0-2) or the dagger (3-4) to the
 * effect's value plus the location's tier, and adds the strength's
 * bits 4-3. SHOP_BUY_EFFECT_0-4 and APPLY_SHOP_EFFECT. */
static void apply_shop_effect(uint8_t n) {
    uint8_t *weapon = n < 3 ? &ENTITY_PTR->sword : &ENTITY_PTR->dagger;
    *weapon = SHOP_EFFECT_TABLE[n] + LOCATION_STYLE_BITS + ((ENTITY_PTR->strength >> 3) & 3);
}

/* The pending level amount in an armor byte. EXTRACT_PENDING_XP. */
static uint8_t pending_xp(uint8_t armor) {
    return (armor >> 4) & 7;
}

/* Armor: replaces the armor byte with the effect's value plus the
 * tier, and moves the level into the armor's pending amount, which is
 * lowered until it is less than the level and the old pending amount
 * together. The level keeps the rest. XP_AWARD_0-4 and AWARD_XP. */
static void award_xp(uint8_t n) {
    uint8_t level = ENTITY_PTR->level;
    ENTITY_PTR->level = level & LEVEL_FLAG_MASK;
    uint8_t total = (level & LEVEL_MASK) + pending_xp(ENTITY_PTR->armor);

    uint8_t armor = XP_AWARD_TABLE[n] + LOCATION_STYLE_BITS;
    while (pending_xp(armor) >= total)
        armor -= PENDING_XP_UNIT;
    ENTITY_PTR->armor = armor;
    ENTITY_PTR->level |= total - pending_xp(armor);
}

/* The addresses of the original's SHOP_BUY_EFFECT_0-4 and
 * XP_AWARD_0-4, which SHOP_TABLE's slots hold. */
static const uint16_t WEAPON_EFFECT_ADDRS[5] = {
    SHOP_BUY_EFFECT_0_ADDR, SHOP_BUY_EFFECT_1_ADDR, SHOP_BUY_EFFECT_2_ADDR,
    SHOP_BUY_EFFECT_3_ADDR, SHOP_BUY_EFFECT_4_ADDR,
};
static const uint16_t ARMOR_EFFECT_ADDRS[5] = {
    XP_AWARD_0_ADDR, XP_AWARD_1_ADDR, XP_AWARD_2_ADDR,
    XP_AWARD_3_ADDR, XP_AWARD_4_ADDR,
};

uint8_t shop_buy_handler(void) {
    if (ENTITY_PTR->gold < SHOP_PRICE) {
        set_cursor_row21();
        display_message(MSG_TOO_POOR);
        return CHAIN_BROWSE_HANDLER;
    }
    modify_current_stats(SHOP_PRICE, false);
    set_cursor_row21();
    display_message(MSG_SHUFFLES_OFF);

    /* A slot's handler word is one of these, and its position in the
     * list is the effect's number. */
    uint16_t handler = current_slot()->handler;
    for (uint8_t n = 0; n < 5; n++) {
        if (handler == WEAPON_EFFECT_ADDRS[n])
            apply_shop_effect(n);
        else if (handler == ARMOR_EFFECT_ADDRS[n])
            award_xp(n);
    }
    return CHAIN_BROWSE_HANDLER;
}
