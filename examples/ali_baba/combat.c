/*
    combat.c -- encounters, attacks, damage, and healing, and what
    happens when a mob walks into an encounter entry, an object, or
    another mob.

    A cell may hold several mobs, which are linked through their next
    bytes in the list of the location they are in. FIND_MOB_AT_SAME_POS
    walks that list from MOB_PTR to the next mob in MOB_PTR's cell, and
    most of this unit is built on it. Two mobs are hostile when the font
    groups of their appearances are on different sides, or when both
    are hostile groups with different glyphs.

    An attack goes through RESOLVE_ATTACK, which rolls against the
    attacker's willingness and then makes the attack. MOB_ATTACK is an
    attack by ENTITY_PTR on a mob, whose armor reduces the strength
    CALC_COMBAT_STRENGTH rolls. PLAYER_ATTACK is an attack on anything
    else in a cell: an event entry, or the cell itself. IS_PLAYER_TURN
    selects between them, and its name describes neither. It is 1 when
    the target isn't a mob.

    Departures from the original:

    - The original's scratch bytes are parameters, locals, and return
      values. EFFECT_REC ($F6) is apply_damage's and modify_char_stats'
      `rec`, NUM_VALUE ($BC) modify_char_stats' `amount`, ENTITY_REC
      and EVENT_PTR ($BE) a local or the event handlers' `ev`, and
      TARGET_REC ($BC) check_hostile's `other` or a local.
      LEVEL_THRESHOLD ($BA/$BB), SAVED_CHAR ($BA), and the values the
      original keeps in X and Y across calls are locals.
    - SUM_HOSTILE_AT_POS reads ENTITY_PTR where the original reads
      TARGET_REC, since both of its callers set TARGET_REC from
      ENTITY_PTR just before they call it. UNITS.md proposed a
      parameter for that pointer.
    - The chain routines return the CHAIN_ state of the routine the
      original jumps to. Each ATTACK_ entry calls the next where the
      original falls into it, and the last calls find_attack_target.
      A jump to an ordinary routine (RESOLVE_ATTACK, SHOW_LOCKED_MSG,
      APPLY_DAMAGE_TO_CURRENT) is a call followed by CHAIN_RETURN.
    - EVENT_PRNG_CHECK, which HANDLE_EVENT_SLOT falls into and which
      PLAYER_ATTACK and HANDLE_EVENT_SLOT jump to, is the function
      event_prng_check, and each of those calls it and then returns.
      The other tail jumps are calls too.
    - tune_player gets 0 as the strength for tunes other than 1 and 2,
      which ignore it. The original leaves whatever Y holds.
    - The original compares some bytes as signed numbers. Where both
      operands are small enough that a signed and an unsigned
      comparison agree, the C compares them unsigned. The comparisons
      with the PRNG's output, which covers the whole byte, keep the
      signed test, in AUTO_HEAL, HANDLE_ENCOUNTER, HANDLE_MOB_ENCOUNTER,
      and EVENT_PRNG_CHECK.
    - GET_ENTITY_THREAT returns 0 for every distance from 2 up. The
      original returns 2 for a distance of 129 or more, which
      MANHATTAN_DISTANCE can't return, since a position's column is at
      most 19 and its row at most 12.
    - WAIT_LOOP_INIT, a constant byte at $7ABE, is a #define.

    The original's quirks are kept.

    - PLAYER_ATTACK on a cell with nothing in it has MOB_PTR = 0, or 1
      for a cell on the viewport's edge, and reads and writes the event
      entry there, at $0000-$0003. The header reserves those bytes.
    - When PICK_RANDOM_MEMBER's walk reaches the end of the list before
      the member it counts to, it goes on reading through MOB_PTR = 0.
    - CHECK_ENCOUNTER tests each mob's hostility against the mob it
      looked at before it, so a mob counts as hostile only when it and
      the mob before it in the cell are on different sides.
    - CHECK_ADJACENT_THREATS takes pos - 1 and pos + 1 as the left and
      right neighbors at the ends of a row too.
    - RESOLVE_ATTACK clears bit 4 of the attacker's level byte when its
      rage bit (bit 5) is set, which lowers the level and leaves the
      rage bit set.
    - PROCESS_ENTITY_SCRIPT deletes the event entry and redraws its
      cell. Its comment says it restores the entry.
    - ENTER_COMBAT_STATE sets the restricted flag when a hostile mob is
      in the cell. Its comment says the opposite.

    Ali Baba, main.asm lines 2498-2719, 2886-3000, 3109-3157, 3267-3306, 3568-3675, 3700-4086, 4127-4563, 4974-5105, 5152-5386, 11202-11214, 13362-13469, 15335-15337:

        ORG     $0E25
    CHECK_ENCOUNTER:
        SUBROUTINE CHECK_ENCOUNTER
        ; Check for hostile encounter at entity's position.
        ;
        ; Input:  A = position (passed to FIND_ENTITY_AT_POS)
        ; Output: ENCOUNTER_RESULT = 0 (none), 1 (friendly), 2 (hostile)
        ;         MOB_PTR = encountered mob (if found)
        ; Clobbers: A, X, Y

        JSR     FIND_ENTITY_AT_POS     ; -> ENTITY_REC
        LDA     ENTITY_REC+1
        BAEQ    #$00,_CHECK_ENCOUNTER.none             ; no entity found
        BAMI    #$80,_CHECK_ENCOUNTER.none             ; bit 7 clear = character, skip
        AND     #$7F                   ; strip mob flag
        STA     MOB_PTR+1              ; \ set up mob pointer
        MOVB    ENTITY_REC,MOB_PTR     ; /
    _CHECK_ENCOUNTER.search:
        MOVW    MOB_PTR,TARGET_REC     ; save current mob as reference
        JSR     FIND_MOB_AT_SAME_POS   ; find next mob at same position
        BAMI    #$01,_CHECK_ENCOUNTER.none             ; 0 = none found
        BEQ     .friendly              ; 1 = wrapped (same faction)
        LDY     #MFIELD_HP       ; result >= 2: found ahead
        LDA     (MOB_PTR),Y
        AND     #HP_MASK         ; bits 5-0 = HP
        BALT    #$03,_CHECK_ENCOUNTER.search           ; type < 3 = inactive, skip
        JSR     CHECK_HOSTILE
        BAEQ    #$00,_CHECK_ENCOUNTER.search           ; same faction -> skip
        STOB    #$02,ENCOUNTER_RESULT  ; hostile encounter
        RTS
    _CHECK_ENCOUNTER.none:
        LDA     #$00
        BEQ     .store
    .friendly:
        LDA     #$01
    .store:
        STA     ENCOUNTER_RESULT
        RTS
        ORG     $0E6C
    FIND_MOB_AT_SAME_POS:
        SUBROUTINE FIND_MOB_AT_SAME_POS
        ; Walk mob linked list for next mob at same map position.
        ;
        ; Input:  MOB_PTR — starting mob, CHAR_PTR — group head
        ; Output: A=0 none found, A=1 found (wrapped), A=2 found (ahead)
        ;         MOB_PTR updated to found mob
        ; Modifies: COLOCATE_POS, COLOCATE_LINK, COLOCATE_WRAPPED
        ; Clobbers: A, Y

        LDY     #MFIELD_POSITION       ; save starting mob's position
        LDA     (MOB_PTR),Y
        STA     COLOCATE_POS
        DEY                            ; Y = MFIELD_NEXT
        LDA     (MOB_PTR),Y            ; save next-link as stop condition
        STA     COLOCATE_LINK
        STY     COLOCATE_WRAPPED       ; init = 2 (found ahead)
    .check_link:
        BANE    #$00,_FIND_MOB_AT_SAME_POS.resolve          ; valid link -> follow
        LDY     #$01                   ; \ wrap to first mob
        STY     COLOCATE_WRAPPED       ;  | mark as wrapped (1)
        INY                            ;  | Y = CFIELD_NEXT
        LDA     (CHAR_PTR),Y           ; / first mob from group head
    _FIND_MOB_AT_SAME_POS.resolve:
        JSR     GET_MOB_DATA           ; A -> MOB_DATA_PTR
        LDY     #MFIELD_NEXT
        LDA     (MOB_DATA_PTR),Y       ; this mob's next-link
        BANE    COLOCATE_LINK,_FIND_MOB_AT_SAME_POS.not_self ; differs from start -> not self
        LDA     #$00                   ; full circle -> not found
        RTS
    _FIND_MOB_AT_SAME_POS.not_self:
        INY                            ; Y = MFIELD_POSITION
        LDA     (MOB_DATA_PTR),Y
        BAEQ    COLOCATE_POS,_FIND_MOB_AT_SAME_POS.found    ; same position -> found
        DEY                            ; Y = MFIELD_NEXT
        LDA     (MOB_DATA_PTR),Y       ; follow next link
        JMP     .check_link
    _FIND_MOB_AT_SAME_POS.found:
        MOVW    MOB_DATA_PTR,MOB_PTR   ; update MOB_PTR to found mob
        LDA     COLOCATE_WRAPPED       ; return 1 (wrapped) or 2 (ahead)
        RTS
        ORG     $0EB1
    CHECK_HOSTILE:
        SUBROUTINE CHECK_HOSTILE
        ; Compare faction hostility of two entities.
        ;
        ; Input:  MOB_PTR — first entity, TARGET_REC — second entity
        ; Output: A=1 hostile to each other, A=0 same faction
        ; Clobbers: A, Y

        LDY     #MFIELD_APPEARANCE     ; byte 4: appearance
        LDA     (MOB_PTR),Y            ; first entity appearance
        STA     HOSTILE_APP
        LDA     (TARGET_REC),Y         ; second entity appearance
        JSR     APPEARANCE_TO_FONTCHAR ; -> Y = font group
        JSR     GET_HOSTILITY          ; -> Y = hostility flag
        STA     HOSTILE_A
        STY     HOSTILE_Y
        LDA     HOSTILE_APP            ; first entity appearance
        JSR     APPEARANCE_TO_FONTCHAR
        JSR     GET_HOSTILITY
        BYNE    HOSTILE_Y,_CHECK_HOSTILE.different   ; different factions -> hostile
        BYEQ    #$00,_CHECK_HOSTILE.same             ; both friendly -> same side
        BANE    HOSTILE_A,_CHECK_HOSTILE.different   ; both hostile, different sub -> hostile
    _CHECK_HOSTILE.same:
        LDA     #$00
        RTS
    _CHECK_HOSTILE.different:
        LDA     #$01
        RTS
        ORG     $0EE3
    GET_HOSTILITY:
        SUBROUTINE GET_HOSTILITY
        ; Classify font group as friendly or hostile.
        ;
        ; Input:  Y = font group (appearance / 21)
        ; Output: Y=0 friendly (groups 0-2, 7), Y=1 hostile (groups 3-6)
        ;         A preserved

        CPY     #$03                   ; groups 0-2 = friendly
        BCC     _GET_HOSTILITY.friendly
        BYEQ    #$07,_GET_HOSTILITY.friendly         ; group 7 = friendly
        LDY     #$01                   ; groups 3-6 = hostile
        RTS
    _GET_HOSTILITY.friendly:
        LDY     #$00
        RTS
        ORG     $0EF1
    CHECK_ADJACENT_THREATS:
        SUBROUTINE CHECK_ADJACENT_THREATS
        ; Sum hostile strength in 4 cardinal directions, compare
        ; against entity's defense.
        ;
        ; Input:  A = map position, ENTITY_PTR = character
        ; Output: A=1 safe (defense >= threat), A=0 overwhelmed
        ;         ADJACENT_THREAT = total hostile strength
        ; Clobbers: A, X, Y

        STA     ADJACENT_POS           ; save center position
        STOB    #$00,ADJACENT_THREAT   ; clear threat accumulator
        MOVW    ENTITY_PTR,TARGET_REC

        ; --- check left ---
        LDA     ADJACENT_POS
        BEQ     .skip_left             ; pos 0 -> no left
        SEC
        SBC     #$01                   ; pos - 1
        JSR     SUM_HOSTILE_AT_POS
    .skip_left:
        ; --- check right ---
        LDA     ADJACENT_POS
        BAEQ    #$C7,_CHECK_ADJACENT_THREATS.skip_right       ; pos 199 -> no right
        CLC
        ADC     #$01                   ; pos + 1
        JSR     SUM_HOSTILE_AT_POS
    _CHECK_ADJACENT_THREATS.skip_right:
        ; --- check up ---
        LDA     ADJACENT_POS
        BMI     .do_up                 ; >= 128 -> has up
        BAMI    #MAP_WIDTH,_CHECK_ADJACENT_THREATS.skip_up    ; row 0 -> no up
    .do_up:
        SEC
        SBC     #MAP_WIDTH             ; pos - 20
        JSR     SUM_HOSTILE_AT_POS
    _CHECK_ADJACENT_THREATS.skip_up:
        ; --- check down ---
        LDA     ADJACENT_POS
        BPL     .do_down               ; < 128 -> has down
        BAPL    #$B4,_CHECK_ADJACENT_THREATS.skip_down        ; row 9 -> no down
    .do_down:
        CLC
        ADC     #MAP_WIDTH             ; pos + 20
        JSR     SUM_HOSTILE_AT_POS
    _CHECK_ADJACENT_THREATS.skip_down:
        ; --- compare defense vs threat ---
        LDY     #MFIELD_STRENGTH       ; byte 5: defense in bits 4-1
        LDA     (ENTITY_PTR),Y
        ROR                            ; shift bits 4-1 to 3-0
        AND     #DEFENSE_HALF_MASK
        BAPL    ADJACENT_THREAT,_CHECK_ADJACENT_THREATS.safe  ; defense >= threat -> safe
        LDA     #$00                   ; overwhelmed
        RTS
    _CHECK_ADJACENT_THREATS.safe:
        LDA     #$01                   ; safe
        RTS
        ORG     $0F49
    SUM_HOSTILE_AT_POS:
        SUBROUTINE SUM_HOSTILE_AT_POS
        ; Sum hostile mob strength at a position into ADJACENT_THREAT.
        ;
        ; Input:  A = map position, TARGET_REC = reference entity
        ;         ADJACENT_THREAT = running total
        ; Output: ADJACENT_THREAT updated (capped at $7F)
        ; Clobbers: A, X, Y

        JSR     FIND_ENTITY_AT_POS     ; -> ENTITY_REC
        LDA     ENTITY_REC+1
        BEQ     _SUM_HOSTILE_AT_POS.done                  ; no entity
        BAMI    #$80,_SUM_HOSTILE_AT_POS.done             ; bit 7 clear = character, skip
        AND     #$7F                   ; strip mob flag
        STA     MOB_PTR+1              ; \ set up mob pointer
        MOVB    ENTITY_REC,MOB_PTR     ; /
    _SUM_HOSTILE_AT_POS.check_mob:
        JSR     CHECK_HOSTILE
        BAEQ    #$00,_SUM_HOSTILE_AT_POS.next             ; not hostile -> skip
        LDY     #MFIELD_FLAGS          ; byte 15: status flags
        LDA     (MOB_PTR),Y
        AND     #FLAGS_ENGAGED_BIT     ; bit 2: already in combat?
        BNE     _SUM_HOSTILE_AT_POS.next                  ; yes -> skip
        LDY     #MFIELD_STRENGTH       ; byte 5: strength
        LDA     (MOB_PTR),Y
        AND     #STRENGTH_MASK         ; bits 4-0
        CLC
        ADC     ADJACENT_THREAT        ; accumulate
        BPL     .store
        LDA     #$7F                   ; cap at 127
    .store:
        STA     ADJACENT_THREAT
    _SUM_HOSTILE_AT_POS.next:
        JSR     FIND_MOB_AT_SAME_POS   ; next mob at same position
        BAEQ    #$02,_SUM_HOSTILE_AT_POS.check_mob        ; found -> check it
    _SUM_HOSTILE_AT_POS.done:
        RTS
        ORG     $106F
    APPLY_DAMAGE:
        SUBROUTINE APPLY_DAMAGE
        ; Apply damage to entity at (EFFECT_REC).
        ;
        ; Input:  A = raw damage, EFFECT_REC = target record
        ; Behavior:
        ;   Masks damage to 5 bits, displays severity message,
        ;   subtracts from HP (byte 6 bits 5-0).  If HP <= 0:
        ;   death handler.  If survives: status message by HP tier.
        ; Modifies: DAMAGE_AMOUNT, entity bytes 6/15, ACTIVE_CHAR
        ; Clobbers: A, X, Y

        ; --- damage message selection ---
        AND     #STRENGTH_MASK         ; mask to 5-bit damage
        STA     DAMAGE_AMOUNT
        JSR     SET_CURSOR_ROW21
        LDA     DAMAGE_AMOUNT
        LDX     #$03                   ; base message index
    .ror_loop:
        INX                            ; next severity tier
        CLC
        ROR                            ; count leading zeros
        BNE     .ror_loop
        TXA
        JSR     DISPLAY_MESSAGE        ; show damage descriptor

        ; --- HP reduction ---
        LDY     DAMAGE_AMOUNT
        LDX     #$04                   ; tune 2: damage sound
        JSR     TUNE_PLAYER
        LDY     #MFIELD_HP       ; byte 6: HP + flags
        LDA     (EFFECT_REC),Y
        TAX                            ; save original byte
        AND     #HP_MASK               ; isolate HP (bits 5-0)
        SEC
        SBC     DAMAGE_AMOUNT          ; HP - damage
        BCC     .dead                  ; underflow -> dead
        BEQ     .dead                  ; zero -> dead
        STA     DAMAGE_AMOUNT          ; save remaining HP
        TXA
        AND     #FLAGS_CLASS_MASK      ; preserve flags (bits 7-6)
        ORA     DAMAGE_AMOUNT
        STA     (EFFECT_REC),Y         ; store updated byte 6

        ; --- survival status ---
        JSR     SET_CURSOR_ROW21
        LDA     DAMAGE_AMOUNT
        BAPL    #$03,_APPLY_DAMAGE.check8           ; HP >= 3: check next tier
        LDY     #MFIELD_FLAGS          ; \ set restricted flag
        LDA     (EFFECT_REC),Y         ;  |
        AND     #$F8                   ;  | clear low 3 bits
        ORA     #FLAGS_ENGAGED_BIT     ;  | set bit 2
        STA     (EFFECT_REC),Y         ; /
        JSR     PRINT_ENTITY_NAME
        LDA     #MSG_FEELING_WEAK      ; "is feeling rather weak"
        JSR     DISPLAY_MESSAGE
        RTS
    _APPLY_DAMAGE.check8:
        BAPL    #$08,_APPLY_DAMAGE.ge8              ; HP >= 8: minor
        JSR     PRINT_ENTITY_NAME
        LDA     #MSG_LOTS_OF_FIGHT     ; "still has lots of fight"
        JMP     DISPLAY_MESSAGE
    _APPLY_DAMAGE.ge8:
        JSR     PRINT_ENTITY_NAME
        LDA     #MSG_HIT_WALL          ; "just a scratch"
        JMP     DISPLAY_MESSAGE

        ; --- death handler ---
    .dead:
        LDY     #MFIELD_APPEARANCE     ; byte 4: appearance
        LDA     (EFFECT_REC),Y
        LDX     #$08                   ; tune 4: player death
        BALT    #NPC_APPEARANCE_MIN,_APPLY_DAMAGE.skip_flag ; < $15: player
        LDY     #MFIELD_FLAGS          ; \ set death flag (bit 4)
        LDA     (EFFECT_REC),Y         ;  |
        ORA     #$10                   ;  |
        STA     (EFFECT_REC),Y         ; /
        LDX     #$0A                   ; tune 5: NPC death
    _APPLY_DAMAGE.skip_flag:
        TXA
        PHA                            ; save tune index
        JSR     PRINT_ENTITY_NAME

        ; --- random death message ---
        LDA     #$03
        JSR     RANDOM_IN_RANGE        ; random(3)
        BAEQ    #$00,_APPLY_DAMAGE.rand0
        LDA     #$0E                   ; death variant A
        BNE     .show_death
    _APPLY_DAMAGE.rand0:
        LDA     #$02
        JSR     RANDOM_IN_RANGE        ; random(2)
        CLC
        ADC     #$27                   ; message $27 or $28
    .show_death:
        JSR     DISPLAY_MESSAGE        ; death message
        PLA
        TAX
        JSR     TUNE_PLAYER            ; death tune

        ; --- death cleanup ---
        STOB    #$00,ACTIVE_CHAR
        MOVB    CURRENT_PLAYER,SOURCE_CHAR
        MOVW    EFFECT_REC,TARGET_REC
        JSR     REORDER_CHAR           ; remove from group
        LDWIY    EFFECT_REC,#MFIELD_GOLD,TARGET_REC
        LDY     #$00                   ; Y=0: subtract mode
        JSR     MODIFY_CHAR_STATS      ; zero out gold
        LDY     #MFIELD_POSITION
        LDA     (EFFECT_REC),Y
        JSR     UPDATE_EVENT_LIST      ; process death events
        LDY     #MFIELD_POSITION
        LDA     (EFFECT_REC),Y
        JMP     DRAW_CHAR_AT_POS       ; redraw and return
        ORG     $11FC
    MAP_VALUE_TO_STRING:
        SUBROUTINE MAP_VALUE_TO_STRING
        ; Map a damage/effect value to a verb string and display it.
        ;
        ; Input:  A = numeric value (damage level)
        ; Output: displays the corresponding verb string
        ;
        ; Behavior:
        ;   Values 0-7 map directly to string indices 0-7.
        ;   Values 8-15 compress to indices 8-11.
        ;   Values 16+ compress to indices 12-15.
        ;   Walks the ATTACK_VERB_STRINGS list to find the
        ;   Nth entry and prints it via PRINT_BOTTOM_CENTERED.
        ;
        ; Modifies: PRINT_STRING_ADDR
        ; Clobbers: A, X, Y

        ; --- map value to string index ---
        BAMI    #$08,_MAP_VALUE_TO_STRING.use_as_is        ; < 8 -> use directly
        BAPL    #$10,_MAP_VALUE_TO_STRING.high_range       ; < 16 -> mid range
        ROR                            ; \ compress bits 2-1
        AND     #$03                   ;  | to 2-bit index
        ORA     #$08                   ; / -> index 8-11
        BNE     _MAP_VALUE_TO_STRING.use_as_is
    _MAP_VALUE_TO_STRING.high_range:
        ROR                            ; \ compress bits 4-3
        ROR                            ;  | to 2-bit index
        AND     #$03                   ;  |
        ORA     #$0C                   ; / -> index 12-15
    _MAP_VALUE_TO_STRING.use_as_is:
        TAX                            ; X = string index

        ; --- walk string list to Xth entry ---
        STOW    ATTACK_VERB_STRINGS,PRINT_STRING_ADDR
        LDY     #$00
    .loop:
        DEX
        BNE     .next                  ; not there yet
        JMP     PRINT_BOTTOM_CENTERED  ; print and return
    .next:
        SEC                            ; \ advance ptr by length + 1
        LDA     (PRINT_STRING_ADDR),Y  ;  | (length byte)
        ADC     PRINT_STRING_ADDR      ;  |
        STA     PRINT_STRING_ADDR      ;  |
        LDA     #$00                   ;  |
        ADC     PRINT_STRING_ADDR+1    ;  |
        STA     PRINT_STRING_ADDR+1    ; /
        BNE     .loop
        ORG     $12D0
    PICK_RANDOM_MEMBER:
        SUBROUTINE PICK_RANDOM_MEMBER
        ; Pick a random player character from the current group.
        ;
        ; Input:  CHAR_PTR — group leader's character record
        ; Output: MOB_PTR  — selected member (zeroed if empty group)
        ;
        ; Behavior:
        ;   Reads group size from CFIELD_XP (byte 8 bits 3-0), picks
        ;   a random index, then walks the linked list skipping dead
        ;   (bit 7 set) and NPC (appearance >= $15) members until the
        ;   Nth eligible member is found.  Retries if random index = 0.
        ;
        ; Modifies: MOB_PTR
        ; Clobbers: A, X, Y

    _PICK_RANDOM_MEMBER.loop:
        LDY     #CFIELD_XP             ; byte 8: group count in bits 3-0
        LDA     (CHAR_PTR),Y
        BEQ     .zero                  ; empty group
        JSR     RANDOM_IN_RANGE        ; random(size)
        BAEQ    #$00,_PICK_RANDOM_MEMBER.loop             ; 0 -> retry
        TAX                            ; X = count down to Nth
        JSR     FIRST_GROUP_MEMBER
    .check:
        LDY     #MFIELD_APPEARANCE     ; byte 4: appearance index
        LDA     (MOB_PTR),Y
        BMI     _PICK_RANDOM_MEMBER.next                  ; bit 7 set -> dead/invalid, skip
        BAPL    #NPC_APPEARANCE_MIN,_PICK_RANDOM_MEMBER.next             ; >= $15 -> NPC, skip
        DEX
        BNE     _PICK_RANDOM_MEMBER.next                  ; not Nth yet
        RTS                            ; found
    _PICK_RANDOM_MEMBER.next:
        JSR     NEXT_GROUP_MEMBER
        JMP     .check
    .zero:
        STA     MOB_PTR                ; \ clear pointer (A=0 from BEQ)
        STA     MOB_PTR+1              ; /
        RTS
        ORG     $1425
    ; ---------------------------------------------------------------
    ; APPLY_DAMAGE_TO_CURRENT — apply damage to current entity
    ;
    ; Convenience wrapper: copies ENTITY_PTR into EFFECT_REC and
    ; tail-calls APPLY_DAMAGE.  A is preserved across the pointer
    ; copy via X as a temporary.
    ;
    ; In:  A = damage amount
    ;      ENTITY_PTR = target entity record
    ; Out: EFFECT_REC = ENTITY_PTR; falls through to APPLY_DAMAGE
    ; ---------------------------------------------------------------
    APPLY_DAMAGE_TO_CURRENT:
        SUBROUTINE APPLY_DAMAGE_TO_CURRENT
        ; Apply damage A to entity at ENTITY_PTR.
        ; Input: A = damage amount
        ; Output: copies ENTITY_PTR to EFFECT_REC, jumps to APPLY_DAMAGE
        ; Clobbers: X
        TAX                             ; save damage amount in X
        MOVW    ENTITY_PTR,EFFECT_REC   ; target <- current entity
        TXA                             ; restore damage amount
        JMP     APPLY_DAMAGE            ; apply damage to EFFECT_REC
        ORG     $1432
    APPLY_DAMAGE_TO_MOB:
        SUBROUTINE APPLY_DAMAGE_TO_MOB
        ; Apply damage to the mob at MOB_PTR.
        ;
        ; Input:  A = damage amount, MOB_PTR = target mob
        ; Output: EFFECT_REC = MOB_PTR, jumps to APPLY_DAMAGE
        ; Clobbers: X

        TAX                            ; save damage value
        MOVW    MOB_PTR,EFFECT_REC     ; copy target pointer
        TXA                            ; restore damage value
        JMP     APPLY_DAMAGE
        ORG     $143F
    AUTO_HEAL:
        SUBROUTINE AUTO_HEAL
        ; Randomly restore HP for injured entities.
        ;
        ; Input:  ENTITY_PTR — entity record
        ; Behavior:
        ;   If HP (byte 6, bits 5-0) < 40, 50% chance to start
        ;   healing.  Loops incrementing HP while PRNG >= $50.
        ;   For player characters (appearance < $15), displays
        ;   MSG_REGAIN_CONSC the first time.
        ; Modifies: HEAL_NOTIFIED, entity byte 6
        ; Clobbers: A, X, Y

        LDY     #MFIELD_HP       ; byte 6: HP in bits 5-0
        LDA     (ENTITY_PTR),Y
        AND     #HP_MASK               ; isolate HP (bits 5-0)
        BAMI    #$28,_AUTO_HEAL.do_heal          ; HP < 40 -> try healing
        RTS
    _AUTO_HEAL.do_heal:
        LDA     #$01
        JSR     RANDOM_IN_RANGE        ; 50% chance
        BANE    #$00,_AUTO_HEAL.start_heal       ; nonzero -> proceed
        RTS
    _AUTO_HEAL.start_heal:
        STOB    #$00,HEAL_NOTIFIED     ; clear notification flag
    _AUTO_HEAL.heal_loop:
        JSR     INCREMENT_HP           ; +1 HP
        JSR     STEP_PRNG
        LDA     PRNG_OUTPUT
        BAPL    #$50,_AUTO_HEAL.heal_loop        ; keep healing while PRNG < $50
        LDY     #MFIELD_APPEARANCE     ; check if player character
        LDA     (ENTITY_PTR),Y
        BALT    #NPC_APPEARANCE_MIN,_AUTO_HEAL.player           ; appearance < $15 -> player
    .done:
        RTS
    _AUTO_HEAL.player:
        LDA     HEAL_NOTIFIED          ; already notified?
        BNE     .done
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        LDA     #MSG_REGAIN_CONSC      ; "struggles back to consciousness"
        JMP     DISPLAY_MESSAGE
        ORG     $1482
    INCREMENT_HP:
        SUBROUTINE INCREMENT_HP
        ; Add 1 to entity's HP; notify when HP reaches 3.
        ;
        ; Input:  ENTITY_PTR — entity record
        ; Behavior:
        ;   Increments byte 6 (HP in bits 5-0).  When HP
        ;   reaches exactly 3 (no longer "badly hurt"), prints
        ;   the entity name and sets HEAL_NOTIFIED = $22
        ;   (which also serves as the message number for
        ;   DISPLAY_MESSAGE via fall-through).
        ; Modifies: entity byte 6, HEAL_NOTIFIED
        ; Clobbers: A, Y

        LDY     #MFIELD_HP       ; byte 6: HP in bits 5-0
        LDA     (ENTITY_PTR),Y
        CLC
        ADC     #$01                   ; HP += 1
        STA     (ENTITY_PTR),Y
        AND     #HP_MASK               ; isolate HP (bits 5-0)
        BAEQ    #$03,_INCREMENT_HP.notify           ; HP == 3 -> threshold crossed
        RTS
    _INCREMENT_HP.notify:
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        STOB    #$22,HEAL_NOTIFIED     ; mark notified (A=$22 = message)
        JMP     DISPLAY_MESSAGE
        ORG     $14B7
    ; ---------------------------------------------------------------
    ; ENTER_COMBAT_STATE — transition character to combat-ready
    ;
    ; Evaluates the encounter at the character's position.  If no
    ; hostile is co-located (result != 2), the restricted flag is
    ; set to limit the character's options.  State is set to 2
    ; (combat ready).
    ;
    ; In:  ENTITY_PTR = current character record
    ; Out: char state = 2; restricted flag set if no hostile present
    ; ---------------------------------------------------------------
    ENTER_COMBAT_STATE:
        SUBROUTINE ENTER_COMBAT_STATE
        ; Transition the current entity to combat-ready state (state 2).
        ;
        ; Behavior:
        ;   Evaluates the encounter at the entity's position, clears the
        ;   restricted flag, then re-sets it if no hostile is co-located.
        ;   Sets the character state to 2 (combat ready).
        ;
        ; Clobbers: A, Y

        ; Transition entity to combat-ready state (state 2).
        ; Checks encounter, clears/sets restricted flag.
        ; Clobbers: A, Y
        JSR     CHECK_ENCOUNTER         ; evaluate encounter at position
        JSR     CLEAR_RESTRICTED        ; start with restriction cleared
        LDA     #$02
        BANE    ENCOUNTER_RESULT,_ENTER_COMBAT_STATE.skip  ; hostile co-located? skip
        JSR     SET_RESTRICTED          ; no hostile -> restrict options
    _ENTER_COMBAT_STATE.skip:
        LDA     #$02                    ; state 2 = combat ready
        JSR     SET_CHAR_STATE
        RTS
        ORG     $14CD
    ; ---------------------------------------------------------------
    ; START_COMBAT — begin active attack
    ;
    ; Sets char state to 3 (attacking), loads the pre-selected
    ; combat target into TARGET_REC, and jumps to RESOLVE_ATTACK
    ; for the willingness check and damage calculation.
    ;
    ; In:  ENTITY_PTR = current character record
    ;      TARGET_PTR/HI = pre-selected target address
    ; Out: char state = 3; control transfers to RESOLVE_ATTACK
    ; ---------------------------------------------------------------
    START_COMBAT:
        SUBROUTINE START_COMBAT
        ; Enter active attack (state 3) and resolve attack.
        ; Input: TARGET_PTR/HI = pre-selected target
        ; Clobbers: A, X, Y
        LDA     #$03                    ; state 3 = attacking
        JSR     SET_CHAR_STATE
        JSR     LOAD_TARGET_PTR         ; TARGET_REC <- target address
        JMP     RESOLVE_ATTACK          ; perform attack resolution
        ORG     $14D8
    ATTACK_RIGHT:
        INC     CURRENT_COL
        BPL     FIND_ATTACK_TARGET ; positive -> valid, else fall through undoes it

    ATTACK_LEFT:
        DEC     CURRENT_COL
        BPL     FIND_ATTACK_TARGET ; positive -> valid, else fall through undoes it

    ATTACK_UP:
        DEC     CURRENT_ROW
        BPL     FIND_ATTACK_TARGET ; positive -> valid, else fall through undoes it

    ATTACK_DOWN:
        INC     CURRENT_ROW        ; fall through to FIND_ATTACK_TARGET
        ORG     $14EA
    FIND_ATTACK_TARGET:
        SUBROUTINE FIND_ATTACK_TARGET
        ; Convert direction col/row to position, find entity, dispatch.
        ; Input: CURRENT_COL/ROW = target square
        ; Output: IS_PLAYER_TURN set, MOB_PTR loaded, jumps to RESOLVE_ATTACK
        ; Clobbers: A, X, Y
        LDA     CURRENT_COL
        LDY     CURRENT_ROW
        JSR     COLROW_TO_POS      ; A = map position
        JSR     FIND_ENTITY_AT_POS ; $BE/$BF = entity pointer
        LDA     $BF
        BEQ     .player            ; $BF = 0 -> no entity / player
        CMP     #$80
        BPL     .mob               ; bit 7 set -> mob

    .player:
        LDA     #$01
        STA     IS_PLAYER_TURN
        LDA     $BE
        STA     MOB_PTR
        LDA     $BF
        STA     MOB_PTR+1
        JMP     RESOLVE_ATTACK

    .mob:
        LDA     #$00
        STA     IS_PLAYER_TURN
        LDA     $BE
        STA     MOB_PTR
        LDA     $BF
        AND     #$7F               ; strip bit 7 (mob indicator)
        STA     MOB_PTR+1
        JSR     FIND_MOB_AT_SAME_POS ; check for multiple mobs
        CMP     #$00
        BNE     .multiple          ; nonzero -> multiple mobs here
        JMP     RESOLVE_ATTACK     ; only one -> attack directly

    .multiple:
        JSR     NEXT_COLOCATED_MOB ; cycle to first candidate
        JSR     NEXT_COLOCATED_MOB ; cycle to second candidate
        CMP     #$00
        BNE     .choose
        JMP     RESOLVE_ATTACK     ; no valid candidate -> attack directly

    .choose:
        JMP     MENU_ATTACK_TARGET ; present target selection menu
        ORG     $1537
    INIT_ATTACK_DISPLAY:
        SUBROUTINE INIT_ATTACK_DISPLAY
        ; Copy target name to ATTACK_NAME_BUF for menu display.
        ; Input: MOB_PTR = target entity
        ; Clobbers: A, X, Y
        LDY     #$00
        LDA     (MOB_PTR),Y        ; byte 0 = record pointer lo
        STA     $BC
        INY
        LDA     (MOB_PTR),Y        ; byte 1 = record pointer hi
        STA     $BD
        LDY     #$00
        LDA     ($BC),Y            ; name length
        TAX
        LDA     #<ATTACK_NAME_BUF
        STA     $BA
        LDA     #>ATTACK_NAME_BUF
        STA     $BB
    .copy:
        INY
        LDA     ($BC),Y            ; name character
        DEX
        BPL     .store             ; X >= 0 -> still within name
        LDA     #$20               ; past end -> space
    .store:
        STA     ($BA),Y
        CPY     #$14               ; 20 characters
        BNE     .copy
        RTS
        ORG     $155E
    SELECT_SOMEONE_ELSE:
        ; "SOMEONE ELSE" menu handler: advance to next colocated mob.
        ; Input:  MOB_PTR — current mob
        ; Output: jumps to MENU_ATTACK_TARGET
        ; Clobbers: A, X, Y

        JSR     NEXT_COLOCATED_MOB     ; advance to next mob
        JMP     MENU_ATTACK_TARGET     ; re-enter target menu

    CHECK_ATTACK_ELIGIBLE:
        SUBROUTINE CHECK_ATTACK_ELIGIBLE
        ; Check if a hostile mob is at the entity's position.
        ;
        ; Input:  ENTITY_PTR — entity to check
        ; Output: C=1 hostile found (MOB_PTR set to it)
        ;         C=0 no hostile at position
        ; Modifies: MOB_PTR, TARGET_REC
        ; Clobbers: A, X, Y

        LDA     ENTITY_PTR             ; \ copy ENTITY_PTR
        STA     TARGET_REC             ;  | to TARGET_REC
        STA     MOB_PTR                ;  | and MOB_PTR
        LDA     ENTITY_PTR+1           ;  |
        STA     TARGET_REC+1           ;  |
        STA     MOB_PTR+1              ; /
    .scan:
        JSR     FIND_MOB_AT_SAME_POS
        LDA     ENTITY_PTR             ; \ check if pointer changed
        CMP     MOB_PTR                ;  |
        BNE     .found                 ;  |
        LDA     ENTITY_PTR+1           ;  |
        CMP     MOB_PTR+1              ;  |
        BEQ     .none                  ; / same pointer -> none found
    .found:
        JSR     CHECK_HOSTILE          ; A=0 if friendly
        CMP     #$00
        BEQ     .scan                  ; not hostile -> keep scanning
        SEC                            ; hostile found
        RTS
    .none:
        CLC                            ; no hostile found
        RTS

    NEXT_COLOCATED_MOB:
        SUBROUTINE NEXT_COLOCATED_MOB
        ; Advance MOB_PTR to the next valid NPC at the same position.
        ;
        ; Input:  MOB_PTR — current mob
        ; Output: A=1 valid target found, A=0 no new mob
        ;         MOB_PTR updated if found
        ; Modifies: MOB_PTR, SAVED_MOB_LINK
        ; Clobbers: A, X, Y

        LDY     #MFIELD_NEXT           ; byte 2: next link
        LDA     (MOB_PTR),Y
        STA     SAVED_MOB_LINK         ; save current link
    .retry:
        JSR     FIND_MOB_AT_SAME_POS
        LDY     #MFIELD_NEXT           ; byte 2: new mob's link
        LDA     (MOB_PTR),Y
        CMP     SAVED_MOB_LINK         ; same as saved?
        BNE     .check                 ; different -> check validity
        LDA     #$00                   ; wrapped around, none found
        RTS
    .check:
        LDY     #MFIELD_APPEARANCE     ; byte 4: appearance
        LDA     (MOB_PTR),Y
        CMP     #NPC_APPEARANCE_MIN    ; >= $15 = valid NPC target?
        BCC     .retry                 ; below threshold -> skip
        LDA     #$01                   ; valid target found
        RTS
        ORG     $15AB
    RESOLVE_ATTACK:
        SUBROUTINE RESOLVE_ATTACK
        ; Compute combat willingness and resolve attack or hesitation.
        ;
        ; Input:  ENTITY_PTR    — attacker record
        ;         MOB_PTR       — target record
        ;         IS_PLAYER_TURN — 1 = player, 0 = AI
        ;
        ; Output: branches to PLAYER_ATTACK or MOB_ATTACK on hit;
        ;         prints "MISSES" and returns on hesitation.
        ;
        ; Behavior:
        ;   Player willingness is fixed at 28.  AI willingness starts
        ;   at the entity's level (byte 11 bits 4-0) and accumulates
        ;   modifiers from flag bits, combat state, HP, and defense.
        ;   Clamped to 0-30, then random roll 0-30 determines hit.
        ;
        ; Modifies: COMBAT_WILLINGNESS, TARGET_DEFENSE, entity byte 11
        ; Clobbers: A, X, Y

        ; --- set blink cursor to target ---
        JSR     SET_BLINK_TARGET

        ; --- player: fixed willingness ---
        LDA     IS_PLAYER_TURN
        BANE    #$01,_RESOLVE_ATTACK.not_player
        STOB    #$1C,COMBAT_WILLINGNESS ; player = 28 (always willing)
        JMP     _RESOLVE_ATTACK.clamp_done

        ; --- AI: base level ---
    _RESOLVE_ATTACK.not_player:
        LDY     #MFIELD_LEVEL          ; byte 11: flags | level
        LDA     (ENTITY_PTR),Y
        TAX
        AND     #LEVEL_MASK            ; level (bits 4-0) = base willingness
        STA     COMBAT_WILLINGNESS

        ; --- AI: attacker flag bits (byte 11) ---
        TXA
        AND     #LEVEL_BIT5_MASK       ; bit 5: rage flag
        BEQ     .no_bit5
        TXA
        AND     #LEVEL_CLEAR_BIT4      ; consume rage: clear bit 4
        STA     (ENTITY_PTR),Y
        LDA     #$18                   ; +24 (rage bonus)
        JSR     ADD_WILLINGNESS
    .no_bit5:

        ; --- AI: target flag bits (byte 11) ---
        LDA     (MOB_PTR),Y           ; target byte 11
        TAY
        AND     #TARGET_BIT6_MASK      ; bit 6: cautious
        BEQ     .no_bit6
        LDA     #$FC                   ; -4 (target cautious)
        JSR     ADD_WILLINGNESS
    .no_bit6:
        TYA
        AND     #TARGET_BIT7_MASK      ; bit 7: aggressive
        BEQ     .no_bit7
        LDA     #$08                   ; +8 (target aggressive)
        JSR     ADD_WILLINGNESS

        ; --- AI: target combat state (byte 15) ---
    .no_bit7:
        LDY     #MFIELD_FLAGS          ; byte 15: status flags
        LDA     (MOB_PTR),Y
        TAX
        AND     #FLAGS_ENGAGED_BIT     ; bit 2: combat engaged
        BEQ     .no_restrict
        LDA     #$06                   ; +6 (already fighting)
        JSR     ADD_WILLINGNESS
    .no_restrict:
        TXA
        AND     #FLAGS_STATE_MASK      ; bits 1-0: combat state
        BAPL    #$01,_RESOLVE_ATTACK.ge1
        LDA     #$03                   ; +3 (state 0: idle)
        JSR     ADD_WILLINGNESS
        JMP     _RESOLVE_ATTACK.state_done
    _RESOLVE_ATTACK.ge1:
        BNE     .ne1
        LDA     #$FF                   ; -1 (state 1: guarding)
        JSR     ADD_WILLINGNESS
        JMP     _RESOLVE_ATTACK.state_done
    .ne1:
        BANE    #$03,_RESOLVE_ATTACK.state_done
        LDA     #$FD                   ; -3 (state 3: retreating)
        JSR     ADD_WILLINGNESS

        ; --- AI: target HP check ---
    _RESOLVE_ATTACK.state_done:
        LDY     #MFIELD_HP       ; byte 6: HP in bits 5-0
        LDA     (MOB_PTR),Y
        AND     #HP_MASK
        BAGE    #$03,_RESOLVE_ATTACK.hp_ok            ; HP >= 3 -> not critical
        LDA     #$18                   ; +24 (target critically wounded)
        JSR     ADD_WILLINGNESS

        ; --- AI: defense differential (same position only) ---
    _RESOLVE_ATTACK.hp_ok:
        LDY     #MFIELD_POSITION       ; byte 3: map position
        LDA     (ENTITY_PTR),Y
        CMP     (MOB_PTR),Y
        BNE     .diff_pos              ; not co-located -> skip
        LDY     #MFIELD_STRENGTH       ; byte 5: defense in bits 4-1
        LDA     (MOB_PTR),Y           ; \ target defense
        ROR                            ;  | shift bits 4-1 to 3-0
        AND     #DEFENSE_HALF_MASK     ;  |
        STA     TARGET_DEFENSE         ; /
        LDA     (ENTITY_PTR),Y        ; \ attacker defense
        ROR                            ;  | shift bits 4-1 to 3-0
        AND     #DEFENSE_HALF_MASK     ; /
        SEC
        SBC     TARGET_DEFENSE         ; delta = attacker - target
        JSR     ADD_WILLINGNESS

        ; --- clamp willingness to 0-30 ---
    .diff_pos:
        LDA     COMBAT_WILLINGNESS
        BPL     .not_neg
        STOB    #$00,COMBAT_WILLINGNESS ; floor at 0
        JMP     _RESOLVE_ATTACK.clamp_done
    .not_neg:
        BAMI    #$1F,_RESOLVE_ATTACK.clamp_done
        STOB    #$1E,COMBAT_WILLINGNESS ; ceiling at 30

        ; --- random roll: attack or hesitate ---
    _RESOLVE_ATTACK.clamp_done:
        LDA     #$1F                   ; range 0-30
        JSR     RANDOM_IN_RANGE
        BAMI    COMBAT_WILLINGNESS,_RESOLVE_ATTACK.attack ; roll < willingness -> hit
        BEQ     _RESOLVE_ATTACK.attack                ; roll == willingness -> hit

        ; --- hesitate: print "[name] MISSES" ---
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        STOW    S_MISSES,TARGET_REC
        JSR     PRINT_BOTTOM_CENTERED
        JSR     SCROLL_STATUS_LINE
        LDA     #$01
        BAEQ    IS_PLAYER_TURN,_RESOLVE_ATTACK.is_player
        JSR     PRINT_MEMBER_NAME
    _RESOLVE_ATTACK.is_player:
        JSR     DELAY_WITH_ANIMATION
        RTS

        ; --- dispatch attack ---
    _RESOLVE_ATTACK.attack:
        LDA     IS_PLAYER_TURN
        BEQ     .mob_attack
        JMP     PLAYER_ATTACK
    .mob_attack:
        JMP     MOB_ATTACK
        ORG     $169B
    ADD_WILLINGNESS:
        SUBROUTINE ADD_WILLINGNESS
        ; Add A to the combat willingness accumulator.
        ;
        ; Input:  A = willingness delta (signed)
        ; Output: COMBAT_WILLINGNESS += A
        ; Clobbers: A

        CLC
        ADC     COMBAT_WILLINGNESS
        STA     COMBAT_WILLINGNESS
        RTS
        ORG     $16E0
    CALC_COMBAT_STRENGTH:
        SUBROUTINE CALC_COMBAT_STRENGTH
        ; Compute effective attack power for the current combatant.
        ;
        ; Input:  ENTITY_PTR — attacker, MOB_PTR — target
        ;         IS_PLAYER_TURN, CHAR_AI_MODE
        ; Output: COMBAT_STRENGTH (0-$1F)
        ;
        ; Behavior:
        ;   Same position -> melee (MFIELD_DEFENSE), else ranged
        ;   (MFIELD_ATTACK).  Low nibble rolled twice and summed.
        ;   Player-controlled on mob's turn: fumble check (roll
        ;   against $0D or $50; zero = fumble with +0..9 bonus).
        ;
        ; Modifies: COMBAT_STRENGTH, may mark stat field $81
        ; Clobbers: A, X, Y

        ; --- select melee or ranged stat ---
        LDA     IS_PLAYER_TURN
        BAEQ    #$01,_CALC_COMBAT_STRENGTH.use_ranged       ; not player turn -> ranged
        LDY     #MFIELD_POSITION
        LDA     (MOB_PTR),Y
        CMP     (ENTITY_PTR),Y
        BNE     _CALC_COMBAT_STRENGTH.use_ranged            ; different pos -> ranged
        LDY     #MFIELD_DEFENSE        ; same pos -> melee
        BNE     .got_field
    _CALC_COMBAT_STRENGTH.use_ranged:
        LDY     #MFIELD_ATTACK         ; ranged stat
    .got_field:

        ; --- roll base combat strength ---
        TYA                            ; save field index
        PHA
        LDA     (ENTITY_PTR),Y
        AND     #DEFENSE_HALF_MASK     ; low nibble = base
        TAX
        JSR     RANDOM_IN_RANGE        ; first roll
        STA     COMBAT_STRENGTH
        TXA
        JSR     RANDOM_IN_RANGE        ; second roll
        SEC                            ; +1 via carry
        ADC     COMBAT_STRENGTH
        STA     COMBAT_STRENGTH

        ; --- fumble check (player-controlled, mob's turn) ---
        LDA     CHAR_AI_MODE
        BEQ     .check_turn
    .exit_pla:
        PLA
        RTS
    .check_turn:
        LDA     IS_PLAYER_TURN
        BNE     .exit_pla              ; player's turn -> no fumble
        PLA
        TAY                            ; Y = field index
        TAX                            ; X = saved for later
        LDA     (ENTITY_PTR),Y
        AND     #TARGET_BIT7_MASK      ; test stat high bit
        BNE     .use_50
        LDA     #$0D                   ; bit 7 clear: 1-in-13
        BNE     .do_random
    .use_50:
        LDA     #$50                   ; bit 7 set: 1-in-80
    .do_random:
        JSR     RANDOM_IN_RANGE
        BAEQ    #$00,_CALC_COMBAT_STRENGTH.fumbled          ; zero = fumble
        RTS

        ; --- fumble: mark field, show scene, add bonus ---
    _CALC_COMBAT_STRENGTH.fumbled:
        TXA
        TAY
        LDA     #$81                   ; fumble marker
        STA     (ENTITY_PTR),Y
        JSR     SET_CURSOR_ROW21
        LDA     #$01                   ; scene 1: fumble
        JSR     SCENE_LOOP
        LDA     #$0A                   ; bonus range 0-9
        JSR     RANDOM_IN_RANGE
        SEC
        ADC     COMBAT_STRENGTH
        BALT    #$1F,_CALC_COMBAT_STRENGTH.cap_ok           ; under cap?
        LDA     #$1F                   ; clamp to $1F
    _CALC_COMBAT_STRENGTH.cap_ok:
        STA     COMBAT_STRENGTH
        RTS
        ORG     $1751
    PLAYER_ATTACK:
        SUBROUTINE PLAYER_ATTACK
        ; Resolve player-initiated attack on target entity.
        ;
        ; Input:  MOB_PTR — target, ENTITY_PTR — attacker
        ; Behavior:
        ;   Computes combat strength, classifies target (dead/event/
        ;   normal), rolls hit (0-12 vs strength), handles locked
        ;   containers and event slot processing.
        ; Clobbers: A, X, Y

        ; --- setup: strength, name, tune ---
        JSR     CALC_COMBAT_STRENGTH
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        LDA     COMBAT_STRENGTH
        JSR     MAP_VALUE_TO_STRING    ; strength descriptor
        JSR     SCROLL_STATUS_LINE
        MOVW    MOB_PTR,ENTITY_REC     ; ENTITY_REC = target
        JSR     CLASSIFY_ENTITY_TYPE
        JSR     PRINT_BOTTOM_CENTERED
        LDY     COMBAT_STRENGTH
        LDX     #$02                   ; tune 1: combat hit
        JSR     TUNE_PLAYER

        ; --- target classification ---
        LDY     #MFIELD_TYPE
        LDA     (MOB_PTR),Y           ; target type byte
        BAEQ    #EVENT_EMPTY,_PLAYER_ATTACK.check_event ; $FE = dead/empty
        BALT    #$40,_PLAYER_ATTACK.roll_hit         ; < $40 = normal entity
    _PLAYER_ATTACK.immune:
        JSR     DISPATCH_ON_MODE
        JSR     SCROLL_STATUS_LINE
        LDA     #MSG_TOUCHED           ; "as soon as it is touched"
        JMP     DISPLAY_MESSAGE

        ; --- hit roll ---
    _PLAYER_ATTACK.roll_hit:
        LDA     #$0D                   ; range 0-12
        JSR     RANDOM_IN_RANGE
        BAGE    COMBAT_STRENGTH,_PLAYER_ATTACK.immune ; roll >= strength: miss
        JSR     DISPATCH_ON_MODE
        LDY     #MFIELD_TYPE           ; \ set damage-taken flag (bit 6)
        LDA     (ENTITY_REC),Y         ;  |
        ORA     #$40                   ;  |
        STA     (ENTITY_REC),Y         ; /
        DEY                            ; Y = MFIELD_CHAR (byte 0)
        LDA     (ENTITY_REC),Y
        JSR     POS_TO_COLROW
        STY     FONT_ROW
        STA     FONT_COL
        JSR     GET_ENTITY_FONTCHAR
        JSR     RENDER_FONT_CHAR       ; redraw entity
        LDX     #$0C                   ; tune 6: hit confirmed
        JSR     TUNE_PLAYER
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_INSERT_DISK       ; hit confirmation
        JMP     DISPLAY_MESSAGE

        ; --- event slot processing ---
    _PLAYER_ATTACK.check_event:
        LDY     #MFIELD_NEXT           ; byte 2: event data
        LDA     (MOB_PTR),Y
        TAX
        AND     #FLAGS_CLASS_MASK      ; isolate activation bits
        BANE    #FLAGS_CLASS_MASK,_PLAYER_ATTACK.check_type ; $C0 = activated event
        TXA
        AND     #STRENGTH_MASK         ; sub-type (bits 4-0)
        BAPL    #$1D,_PLAYER_ATTACK.immune           ; >= $1D: immune
        JSR     PROCESS_ENTITY_SCRIPT
        JSR     SET_CURSOR_ROW21
        LDA     #$02                   ; scene 2
        JMP     SCENE_LOOP

        ; --- container / locked object ---
    _PLAYER_ATTACK.check_type:
        BAEQ    #$00,_PLAYER_ATTACK.immune           ; no activation: immune
        LDA     COMBAT_STRENGTH
        BAPL    #$0A,_PLAYER_ATTACK.strong_enough    ; strength >= 10: can open
        TXA
        BAPL    #$80,_PLAYER_ATTACK.blocked          ; bit 7: locked container
        LDA     COMBAT_STRENGTH
        BAMI    #$04,_PLAYER_ATTACK.blocked          ; strength < 4: too weak
    _PLAYER_ATTACK.strong_enough:
        TXA
        AND     #STRENGTH_MASK         ; sub-type (bits 4-0)
        BAMI    #STRENGTH_MASK,_PLAYER_ATTACK.take_item ; sub-type = max: take item
        STX     $BA                    ; \ decrement counter
        DEC     $BA                    ;  |
        LDA     $BA                    ;  |
        LDY     #MFIELD_NEXT           ;  |
        STA     (MOB_PTR),Y            ; / store decremented event byte
        LDX     #$0E                   ; tune 7: interaction
        JSR     TUNE_PLAYER
        JSR     DISPATCH_ON_MODE
        LDY     #MFIELD_CHAR           ; byte 0: display char
        LDA     (MOB_PTR),Y
        JSR     DRAW_CHAR_AT_POS       ; redraw at position
        JSR     SET_CURSOR_ROW21
    _PLAYER_ATTACK.show_found:
        LDA     #MSG_TOPPLES           ; "the sculpture topples over"
        JSR     DISPLAY_MESSAGE
        JMP     EVENT_PRNG_CHECK
    _PLAYER_ATTACK.take_item:
        JSR     PROCESS_ENTITY_SCRIPT
        JSR     SET_CURSOR_ROW21
        LDY     #MFIELD_NEXT           ; byte 2: event data
        LDA     (MOB_PTR),Y
        AND     #STRENGTH_MASK         ; sub-type
        BAPL    #$1B,_PLAYER_ATTACK.show_found       ; >= $1B: show found
        LDA     #$03                   ; scene 3
        JSR     SCENE_LOOP
        JMP     EVENT_PRNG_CHECK
    _PLAYER_ATTACK.blocked:
        JSR     DISPATCH_ON_MODE
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_HEAVY_ROCK        ; "a heavy piece of rock"
        JMP     DISPLAY_MESSAGE
        ORG     $1849
    PROCESS_ENTITY_SCRIPT:
        SUBROUTINE PROCESS_ENTITY_SCRIPT
        ; Temporarily hide an entity, play a tune, then restore it.
        ;
        ; Input:  MOB_PTR — entity record to process
        ; Behavior:
        ;   Saves display char (byte 0), marks slot empty ($FE),
        ;   plays interaction tune, dispatches animation, then
        ;   restores and redraws the entity.
        ; Modifies: SAVED_CHAR, MOB_PTR byte 0
        ; Clobbers: A, X, Y

        LDY     #MFIELD_CHAR           ; \ save display character
        LDA     (MOB_PTR),Y            ;  |
        STA     SAVED_CHAR             ; /
        LDA     #EVENT_EMPTY           ; mark slot empty ($FE)
        STA     (MOB_PTR),Y
        LDX     #$0E                   ; tune index: entity interaction
        JSR     TUNE_PLAYER
        JSR     DISPATCH_ON_MODE
        LDA     SAVED_CHAR             ; restore saved character
        JMP     DRAW_CHAR_AT_POS       ; redraw at entity position
        ORG     $1860
    MOB_ATTACK:
        SUBROUTINE MOB_ATTACK
        ; Resolve a mob's attack against the current character.
        ;
        ; Input:  ENTITY_PTR — target, MOB_PTR — attacker
        ; Behavior:
        ;   Computes strength, compares vs defense (byte 8 low 4 bits).
        ;   Defense=0: full damage. Strength>defense: partial hit.
        ;   Strength<=defense: absorbed (or critical if HP<3).
        ; Modifies: COMBAT_STRENGTH, MOB_DEFENSE, SCENE_COMBAT_FLAG
        ; Clobbers: A, X, Y

        ; --- setup: strength, names, tune ---
        JSR     CALC_COMBAT_STRENGTH
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        LDA     COMBAT_STRENGTH
        JSR     MAP_VALUE_TO_STRING    ; strength descriptor
        JSR     SCROLL_STATUS_LINE
        JSR     PRINT_MEMBER_NAME      ; defender name
        LDY     COMBAT_STRENGTH
        LDX     #$02                   ; tune 1: combat hit
        JSR     TUNE_PLAYER

        ; --- player character special case ---
        LDY     #MFIELD_APPEARANCE
        LDA     (ENTITY_PTR),Y
        BAGE    #NPC_APPEARANCE_MIN,_MOB_ATTACK.npc ; NPC -> skip
        STOB    #$00,SCENE_COMBAT_FLAG  ; player under attack
    _MOB_ATTACK.npc:
        JSR     DISPATCH_ON_MODE

        ; --- read defense from byte 8 ---
        LDY     #MFIELD_XP             ; byte 8: status/XP/count
        LDA     (MOB_PTR),Y
        STA     MOB_DEFENSE            ; save full byte
        AND     #DEFENSE_HALF_MASK     ; low 4 bits = defense
        BANE    #$00,_MOB_ATTACK.has_defense
        LDA     COMBAT_STRENGTH        ; defense=0: full damage
        JMP     APPLY_DAMAGE_TO_MOB

        ; --- compare strength vs defense ---
    _MOB_ATTACK.has_defense:
        JSR     SCROLL_STATUS_LINE
        LDX     #$17                   ; msg $17 (sign bit set)
        LDA     MOB_DEFENSE
        BMI     .negative
        LDX     #$18                   ; msg $18 (sign bit clear)
    .negative:
        LDA     MOB_DEFENSE
        AND     #DEFENSE_HALF_MASK     ; isolate defense
        STA     MOB_DEFENSE
        LDA     COMBAT_STRENGTH
        SEC
        SBC     MOB_DEFENSE            ; strength - defense
        BEQ     .absorbed
        BPL     .partial               ; positive -> partial hit

        ; --- absorbed: attack blocked ---
    .absorbed:
        INX                            ; $17->$19 or $18->$1A
        INX
        LDY     #MFIELD_HP       ; byte 6: HP
        LDA     (MOB_PTR),Y
        AND     #HP_MASK
        BAMI    #$03,_MOB_ATTACK.critical         ; HP < 3 -> critical block
        TXA
        JMP     DISPLAY_MESSAGE        ; "blocked" message
    _MOB_ATTACK.critical:
        LDA     #MSG_ENOUGH            ; "ENOUGH IS ENOUGH!"
        JMP     DISPLAY_MESSAGE

        ; --- partial hit: remainder = effective damage ---
    .partial:
        STA     COMBAT_STRENGTH        ; save remainder
        TXA
        JSR     DISPLAY_MESSAGE        ; "hit" message
        LDA     COMBAT_STRENGTH
        JMP     APPLY_DAMAGE_TO_MOB  ; apply partial damage
        ORG     $18E4
    SELECT_COMBAT_TARGET:
        SUBROUTINE SELECT_COMBAT_TARGET
        ; Select a combat target for the current AI entity.
        ;
        ; Input:  CHAR_AI_MODE  — AI behavior mode (0-5+)
        ;         ENTITY_PTR    — current entity record
        ;         STEPS_TAKEN   — steps taken this turn
        ;
        ; Output: A = 1: target found (TARGET_PTR/HI set)
        ;         A = 2: engage combat (no specific target)
        ;         A = 4: safe (no threats)
        ;
        ; Modifies: BEST_THREAT, TARGET_PTR, TARGET_PTR+1
        ; Clobbers: A, X, Y

        ; --- dispatch on AI mode ---
        LDA     CHAR_AI_MODE
        BAMI    #$03,_SELECT_COMBAT_TARGET.path_low         ; mode < 3 -> check encounter/threats
        BAMI    #$05,_SELECT_COMBAT_TARGET.path_mid         ; mode 3-4 -> pick highest-threat mob

        ; --- path high (mode >= 5): pick strongest mob ---
        JSR     FIRST_GROUP_MEMBER
        STOB    #$00,BEST_THREAT
    _SELECT_COMBAT_TARGET.loop_high:
        JSR     GET_ENTITY_THREAT
        BAEQ    #$00,_SELECT_COMBAT_TARGET.skip_high        ; not hostile -> skip
        LDY     #MFIELD_STRENGTH
        LDA     (MOB_PTR),Y
        AND     #STRENGTH_MASK         ; strength (bits 4-0)
        BAMI    BEST_THREAT,_SELECT_COMBAT_TARGET.skip_high ; not stronger -> skip
        STA     BEST_THREAT            ; new best
        MOVB    MOB_PTR,TARGET_PTR
        MOVB    MOB_PTR+1,TARGET_PTR+1
    _SELECT_COMBAT_TARGET.skip_high:
        JSR     NEXT_GROUP_MEMBER
        LDA     #$00
        BANE    MOB_PTR+1,_SELECT_COMBAT_TARGET.loop_high   ; more members -> continue
        BAEQ    BEST_THREAT,_SELECT_COMBAT_TARGET.path_low  ; no target -> fall through
        LDA     #$01                   ; target found
        RTS

        ; --- path low (mode < 3): check encounter and threats ---
    _SELECT_COMBAT_TARGET.path_low:
        LDA     #$00
        BANE    STEPS_TAKEN,_SELECT_COMBAT_TARGET.ret_2     ; already moved -> engage
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y        ; entity's map position
        JSR     CHECK_ENCOUNTER
        LDA     #$02
        BAEQ    ENCOUNTER_RESULT,_SELECT_COMBAT_TARGET.ret_2 ; encounter found -> engage
        JSR     CHECK_THREATS_HERE
        BANE    #$01,_SELECT_COMBAT_TARGET.ret_2            ; threats present -> engage
        LDA     #$04                   ; safe
        RTS
    _SELECT_COMBAT_TARGET.ret_2:
        LDA     #$02                   ; engage combat
        RTS

        ; --- path mid (mode 3-4): pick highest-threat mob ---
    _SELECT_COMBAT_TARGET.path_mid:
        STOB    #$00,BEST_THREAT
        JSR     FIRST_GROUP_MEMBER
    _SELECT_COMBAT_TARGET.loop_mid:
        JSR     GET_ENTITY_THREAT
        BAEQ    BEST_THREAT,_SELECT_COMBAT_TARGET.skip_mid  ; threat == 0 -> skip
        BMI     _SELECT_COMBAT_TARGET.skip_mid              ; negative threat -> skip
        STA     BEST_THREAT            ; new best threat
        MOVB    MOB_PTR,TARGET_PTR
        MOVB    MOB_PTR+1,TARGET_PTR+1
    _SELECT_COMBAT_TARGET.skip_mid:
        JSR     NEXT_GROUP_MEMBER
        LDA     #$00
        BANE    MOB_PTR+1,_SELECT_COMBAT_TARGET.loop_mid    ; more members -> continue
        BAEQ    BEST_THREAT,_SELECT_COMBAT_TARGET.path_low  ; no target -> fall through
        LDA     #$01                   ; target found
        RTS
        ORG     $1979
    GET_ENTITY_THREAT:
        SUBROUTINE GET_ENTITY_THREAT
        ; Evaluate threat level of mob at MOB_PTR vs entity at ENTITY_PTR.
        ;
        ; Input:  ENTITY_PTR — reference entity
        ;         MOB_PTR    — mob to evaluate
        ; Output: A = 0 (no threat), 1 (adjacent), 2 (co-located)
        ; Modifies: TARGET_REC
        ; Clobbers: A, Y

        ; --- check hostility ---
        LDA     ENTITY_PTR             ; \ copy ENTITY_PTR to TARGET_REC
        STA     TARGET_REC             ;  |
        LDA     ENTITY_PTR+1           ;  |
        STA     TARGET_REC+1           ; /
        JSR     CHECK_HOSTILE
        CMP     #$00
        BNE     .hostile               ; hostile -> measure distance
        RTS                            ; not hostile -> return 0

        ; --- measure distance ---
    .hostile:
        LDY     #MFIELD_POSITION       ; mob's map position
        LDA     (MOB_PTR),Y
        JSR     MANHATTAN_DISTANCE     ; A = distance to entity
        CMP     #$01
        BNE     .not_adjacent
        RTS                            ; distance = 1 -> return 1 (adjacent)
    .not_adjacent:
        BPL     .far                   ; distance > 1 -> too far
        LDA     #$02                   ; distance = 0 -> return 2 (co-located)
        RTS
    .far:
        LDA     #$00                   ; return 0 (too far)
        RTS
        ORG     $199D
    LOAD_TARGET_PTR:
        SUBROUTINE LOAD_TARGET_PTR
        ; Load the pre-selected combat target into MOB_PTR.
        ;
        ; Input:  TARGET_PTR/HI — saved target from SELECT_COMBAT_TARGET
        ; Output: MOB_PTR = target pointer, IS_PLAYER_TURN = 0
        ; Clobbers: A

        MOVB    TARGET_PTR,MOB_PTR
        MOVB    TARGET_PTR+1,MOB_PTR+1
        STOB    #$00,IS_PLAYER_TURN    ; AI's turn (not player)
        RTS
        ORG     $1C01
    HANDLE_ENCOUNTER:
        SUBROUTINE HANDLE_ENCOUNTER
        ; Process encounter when stepping on an event cell.
        ;
        ; Input:  EVENT_PTR — event list entry (3 bytes)
        ;         ENTITY_PTR — character stepping on event
        ; Behavior:
        ;   Type < $40: random encounter with avoidance check.
        ;   Type >= $40: specific encounter with position update.
        ; Clobbers: A, X, Y

        ; --- random encounter path (event type $00-$3F) ---
        LDA     #$00
        JSR     SET_CHAR_STATE             ; state = idle
        LDY     #EVFIELD_TYPE
        LDA     (EVENT_PTR),Y              ; event type byte
        BAPL    #$40,_HANDLE_ENCOUNTER.specific             ; >= $40 -> specific encounter
        LDA     #$14                       ; range = 20
        JSR     RANDOM_IN_RANGE
        STA     PRNG_OUTPUT
        LDY     #MFIELD_STRENGTH
        LDA     (ENTITY_PTR),Y            ; char strength field
        AND     #STRENGTH_MASK             ; bits 4-0 = avoidance stat
        BAMI    PRNG_OUTPUT,_HANDLE_ENCOUNTER.roll_type     ; stat < random -> encounter
        JMP     .avoided                   ; stat >= random -> avoid
    _HANDLE_ENCOUNTER.roll_type:
        JSR     STEP_PRNG
        LDA     #$74
        BAPL    PRNG_OUTPUT,_HANDLE_ENCOUNTER.normal ; $74 >= random -> normal encounter
        JMP     .special                   ; rare: special encounter
    _HANDLE_ENCOUNTER.normal:
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_DOOR_OPENS
        JSR     DISPLAY_MESSAGE            ; "encounter" message
        LDX     #$14              ; tune 10: encounter effect
        JMP     TUNE_PLAYER

        ; --- specific encounter path (type >= $40) ---
    _HANDLE_ENCOUNTER.specific:
        LDY     #EVFIELD_POSITION
        LDA     (EVENT_PTR),Y              ; event position byte
        STA     EVENT_POS
        LDA     #$01                       ; state = engaged
        JSR     SET_CHAR_STATE
        LDY     #EVFIELD_TYPE
        LDA     (EVENT_PTR),Y
        AND     #FLAGS_STATUS_MASK         ; low 6 bits = encounter ID
        BNE     .have_id
        JSR     STEP_PRNG                  ; ID 0 -> derive from PRNG
        LDA     PRNG_OUTPUT
        ROR
        ROR
        AND     #$1C
        ORA     #$01
    .have_id:
        STA     ACTIVE_CHAR                ; store encounter ID
        ROR
        BCC     _HANDLE_ENCOUNTER.no_clear
        LDY     #MFIELD_APPEARANCE
        LDA     (ENTITY_PTR),Y             ; char appearance
        BMI     _HANDLE_ENCOUNTER.no_clear
        BAMI    #$2A,_HANDLE_ENCOUNTER.no_clear
        BAPL    #$3F,_HANDLE_ENCOUNTER.no_clear
        STOB    #$00,ACTIVE_CHAR ; clear ID if char in range $2A-$3E
    _HANDLE_ENCOUNTER.no_clear:
        LDY     #$02
        LDA     (EVENT_PTR),Y              ; event byte 2 = destination pos
        STA     EVENT_DEST_POS
        JSR     COMMIT_MOVE                ; commit move
        MOVB    WAIT_LOOP_INIT,WAIT_LOOP_COUNT
        JSR     REORDER_CURRENT_CHAR
        JSR     TIMED_WAIT
        LDY     #MFIELD_POSITION
        LDA     EVENT_DEST_POS
        STA     (ENTITY_PTR),Y             ; update char packed position
        LDA     ACTIVE_CHAR
        BAEQ    CURRENT_PLAYER,_HANDLE_ENCOUNTER.same_scene ; compare encounter ID with current scene
        JMP     RESET_TURN_POS             ; different -> reset position
    _HANDLE_ENCOUNTER.same_scene:
        LDA     EVENT_DEST_POS
        JSR     POS_TO_COLROW
        STA     CURRENT_COL
        STY     CURRENT_ROW
        JMP     COMMIT_MOVE                ; commit to new position

        ; --- encounter avoided ---
    .avoided:
        LDY     #$01
        LDA     (EVENT_PTR),Y
        ORA     #$40                       ; mark event as activated
        STA     (EVENT_PTR),Y
        MOVB    CURRENT_COL,TURN_START_COL
        MOVB    CURRENT_ROW,TURN_START_ROW
        JSR     RESET_TURN_POS             ; reset to turn start
        MOVB    TURN_START_COL,BLINK_COL
        MOVB    TURN_START_ROW,BLINK_ROW
        MOVB    FONT_CHARNUM,BLINK_CHAR
        LDX     #$0C              ; tune 6: combat avoided effect
        JSR     TUNE_PLAYER
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_INSERT_DISK
        JMP     DISPLAY_MESSAGE            ; "avoided encounter" message

        ; --- special encounter (rare) ---
    .special:
        JSR     SET_CURSOR_ROW21
        LDA     #$09
        JSR     SCENE_LOOP                 ; trigger scene 9
        LDA     #$01
        JMP     APPLY_DAMAGE_TO_CURRENT
        ORG     $1CF1
    ; SHOW_LOCKED_MSG — display locked/blocked message, reset to idle
    SHOW_LOCKED_MSG:
        SUBROUTINE SHOW_LOCKED_MSG
        ; Display the locked/statue message and reset entity to idle.
        ;
        ; Behavior:
        ;   Positions cursor at row 21, prints the mob name, scrolls the
        ;   status line, displays the statue message, and sets state to 0.
        ;
        ; Clobbers: A, Y

        ; Display "locked container" message and reset state.
        ; Clobbers: A, Y
        JSR     SET_CURSOR_ROW21        ; position cursor
        JSR     PRINT_MOB_NAME          ; print blocking entity name
        JSR     SCROLL_STATUS_LINE
        LDA     #MSG_HIT_STATUE         ; "CHARGES INTO A STONE STATUE."
        JSR     DISPLAY_MESSAGE
        LDA     #$00                    ; state 0 = idle
        JMP     SET_CHAR_STATE
        ORG     $1D4D
    HANDLE_EVENT_SLOT:
        SUBROUTINE HANDLE_EVENT_SLOT
        ; Process event tile interaction.
        ; Input: EVENT_PTR = event entry, ENTITY_PTR = character
        ; Behavior: branches on event byte 2 top bits for
        ;   activated events, locked objects, and treasure.
        ; Clobbers: A, X, Y
        ; Handle empty slot interaction: treasure pickup, traps,
        ; and event activation.
        LDA     #$00
        JSR     SET_CHAR_STATE
        LDY     #$02
        LDA     (EVENT_PTR),Y
        STA     EVENT_SLOT_BYTE
        AND     #$C0
        BANE    #$C0,_HANDLE_EVENT_SLOT.not_active
        LDA     (EVENT_PTR),Y
        AND     #$1F
        TAX
        BAPL    #$1D,_HANDLE_EVENT_SLOT.no_remove
        LDY     #$00
        LDA     #$FE
        STA     (EVENT_PTR),Y
        TXA
        BAEQ    #$00,_HANDLE_EVENT_SLOT.no_remove
        LDA     #$0A
        JSR     SCENE_LOOP
    _HANDLE_EVENT_SLOT.no_remove:
        JSR     COMMIT_MOVE
        JSR     CHECK_THREATS_HERE
        BANE    #$00,_HANDLE_EVENT_SLOT.has_threat
        STA     MOVE_POINTS
    _HANDLE_EVENT_SLOT.has_threat:
        JMP     MOVE_LOOP
    _HANDLE_EVENT_SLOT.not_active:
        PHA
        LDX     #$0E              ; tune 7: entity interaction
        JSR     TUNE_PLAYER
        PLA
        BANE    #$00,_HANDLE_EVENT_SLOT.has_type
        LDA     EVENT_SLOT_BYTE
        AND     #$1F
        BAMI    #$1B,_HANDLE_EVENT_SLOT.small_val
        JMP     SHOW_LOCKED_MSG
    _HANDLE_EVENT_SLOT.small_val:
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        LDA     #MSG_DEATH
        JSR     DISPLAY_MESSAGE
        LDA     #$01
        JMP     APPLY_DAMAGE_TO_CURRENT
    _HANDLE_EVENT_SLOT.has_type:
        BAEQ    #$40,_HANDLE_EVENT_SLOT.type40
        LDA     #$19
        JSR     RANDOM_IN_RANGE
        STA     PRNG_OUTPUT
        LDY     #$05
        LDA     (ENTITY_PTR),Y
        AND     #$1F
        BAPL    PRNG_OUTPUT,_HANDLE_EVENT_SLOT.type40
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_HEAVY_ROCK
        JSR     DISPLAY_MESSAGE
        LDY     #$02
        LDA     EVENT_SLOT_BYTE
        AND     #$3F
        ORA     #$40
        STA     (EVENT_PTR),Y
    EVENT_PRNG_CHECK:              ; $1DDC - external entry point
        JSR     STEP_PRNG
        LDA     PRNG_OUTPUT
        BAMI    #$14,_HANDLE_EVENT_SLOT.trap_damage
        RTS
    _HANDLE_EVENT_SLOT.trap_damage:
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_DUST_CLOUD
        JSR     DISPLAY_MESSAGE
        JSR     SET_RESTRICTED
        JSR     PRINT_MOB_NAME
        JSR     DELAY_WITH_ANIMATION
        JSR     STEP_PRNG
        LDA     PRNG_OUTPUT
        ROL2
        LDA     #$01
        ADC     #$00
        JMP     APPLY_DAMAGE_TO_CURRENT
    _HANDLE_EVENT_SLOT.type40:
        LDA     EVENT_SLOT_BYTE
        AND     #$1F
        STA     EVENT_SUB_VALUE
        BAPL    #$1B,_HANDLE_EVENT_SLOT.big_treasure
        JSR     SET_CURSOR_ROW21
        LDA     #$13
        JSR     SCENE_LOOP
        JMP     _HANDLE_EVENT_SLOT.clear_slot
    _HANDLE_EVENT_SLOT.big_treasure:
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_TOPPLES
        JSR     DISPLAY_MESSAGE
        LDA     EVENT_SUB_VALUE
        BAMI    #$1F,_HANDLE_EVENT_SLOT.clear_slot
        DEC     EVENT_SUB_VALUE
        LDA     EVENT_SLOT_BYTE
        AND     #$E0
        ORA     EVENT_SUB_VALUE
        LDY     #$02
        STA     (EVENT_PTR),Y
        JMP     .update_pos
    _HANDLE_EVENT_SLOT.clear_slot:
        LDY     #$00
        LDA     #$FE
        STA     (EVENT_PTR),Y
    .update_pos:
        MOVB    CURRENT_COL,TURN_START_COL
        MOVB    CURRENT_ROW,TURN_START_ROW
        JSR     RESET_TURN_POS
        JSR     ANIM_TICK_AND_WAIT
        JMP     EVENT_PRNG_CHECK
        ORG     $1E5A
    HANDLE_MOB_ENCOUNTER:
        SUBROUTINE HANDLE_MOB_ENCOUNTER
        ; Handle mob-on-mob movement encounter with flee mechanics.
        ;
        ; Input:  ENTITY_PTR — moving character
        ;         CURRENT_COL/ROW — destination
        ; Behavior:
        ;   Evaluates threat at destination. If hostiles present,
        ;   computes flee threshold from strength vs threat.
        ;   If character doesn't flee, commits move and checks
        ;   for combat encounters.
        ; Clobbers: A, X, Y

        ; --- evaluate threat at destination ---
        STOB    #$00,ADJACENT_THREAT
        MOVW    ENTITY_PTR,TARGET_REC
        LDA     CURRENT_COL
        LDY     CURRENT_ROW
        JSR     COLROW_TO_POS
        JSR     SUM_HOSTILE_AT_POS
        LDA     ADJACENT_THREAT
        BEQ     _HANDLE_MOB_ENCOUNTER.no_flee               ; no hostiles -> skip flee

        ; --- compute flee threshold ---
        LDY     #MFIELD_STRENGTH
        LDA     (ENTITY_PTR),Y
        AND     #STRENGTH_MASK         ; strength (bits 4-0)
        CLC
        ADC     #$10                   ; courage = strength + $10
        SEC
        SBC     ADJACENT_THREAT        ; minus adjacent threat
        BMI     .clamp_low             ; negative -> clamp
        BAMI    #$1D,_HANDLE_MOB_ENCOUNTER.in_range         ; < $1D -> use as-is
        LDA     #$1C                   ; clamp high to $1C
        BNE     _HANDLE_MOB_ENCOUNTER.in_range
    .clamp_low:
        LDA     #$04                   ; clamp low to $04
    _HANDLE_MOB_ENCOUNTER.in_range:
        ASL                            ; \ threshold *= 4
        ASL                            ; /
        ORA     #$03                   ; set low 2 bits
        TAX

        ; --- flee roll ---
        JSR     STEP_PRNG
        TXA
        BAPL    PRNG_OUTPUT,_HANDLE_MOB_ENCOUNTER.no_flee   ; threshold >= random -> hold
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        LDA     #MSG_FLYING_TACKLE     ; "leaps into a flying tackle"
        JSR     DISPLAY_MESSAGE
        MOVB    TURN_START_COL,CURRENT_COL ; reset to start
        MOVB    TURN_START_ROW,CURRENT_ROW
        JMP     END_TURN               ; fled

        ; --- no flee: commit move and check encounter ---
    _HANDLE_MOB_ENCOUNTER.no_flee:
        JSR     COMMIT_MOVE
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y
        JSR     CHECK_ENCOUNTER
        LDA     ENCOUNTER_RESULT
        BAPL    #$02,_HANDLE_MOB_ENCOUNTER.hostile          ; result >= 2 -> hostile
        JSR     CHECK_THREATS_HERE
        BAEQ    #$01,_HANDLE_MOB_ENCOUNTER.one_threat       ; exactly 1 -> continue
        JMP     END_TURN
    _HANDLE_MOB_ENCOUNTER.one_threat:
        JMP     MOVE_LOOP

        ; --- hostile: restrict co-located group members ---
    _HANDLE_MOB_ENCOUNTER.hostile:
        JSR     FIRST_GROUP_MEMBER
    .walk_group:
        LDA     MOB_PTR+1
        BEQ     .group_done            ; null -> end of list
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y
        CMP     (MOB_PTR),Y            ; same position?
        BNE     .next_member
        JSR     SET_TARGET_RESTRICTED  ; restrict member
    .next_member:
        JSR     NEXT_GROUP_MEMBER
        JMP     .walk_group
    .group_done:
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        LDA     #MSG_UNCONSCIOUS       ; "is unconscious"
        JSR     DISPLAY_MESSAGE
        JMP     END_TURN
        ORG     $1F03
    ; SET_TARGET_RESTRICTED — pin mob at current position
    SET_TARGET_RESTRICTED:
        SUBROUTINE SET_TARGET_RESTRICTED
        ; Set the combat-engaged flag (bit 2) on the mob at MOB_PTR.
        ;
        ; Inputs:
        ;   MOB_PTR — pointer to the mob record
        ;
        ; Behavior:
        ;   ORs bit 2 into byte 15 of the mob record, marking it as
        ;   pinned in combat.
        ;
        ; Modifies: mob record byte 15
        ; Clobbers: A, Y

        ; Set restricted flag (bit 2) on entity at MOB_PTR byte 15.
        ; Clobbers: A, Y
        LDY     #MFIELD_FLAGS           ; byte 15: status flags
        LDA     (MOB_PTR),Y
        ORA     #$04                    ; set bit 2 = combat engaged
        STA     (MOB_PTR),Y
        RTS
        ORG     $645F
    CHECK_THREATS_HERE:
        SUBROUTINE CHECK_THREATS_HERE
        ; Check adjacent threats at current cursor position.
        ;
        ; Input:  CURRENT_COL/ROW, ENTITY_PTR
        ; Output: A=1 safe, A=0 overwhelmed (see CHECK_ADJACENT_THREATS)
        ; Clobbers: A, X, Y

        LDA     CURRENT_COL
        LDY     CURRENT_ROW
        JSR     COLROW_TO_POS          ; pack to position byte
        JMP     CHECK_ADJACENT_THREATS ; tail-call
        ORG     $704D
    MODIFY_CHAR_STATS:
        SUBROUTINE MODIFY_CHAR_STATS
        ; Modify gold and recompute level encoding.
        ;
        ; Input:  EFFECT_REC     — pointer to entity record
        ;         NUM_VALUE+0/+1 — 16-bit gold amount
        ;         Y              — 0 = subtract, nonzero = add
        ;
        ; Output: bytes $0D/$0E (gold) updated,
        ;         byte $0C (level encoding) recomputed
        ;
        ; Modifies: STAT_SCALE, STAT_LEVEL_BITS, LEVEL_THRESHOLD/HI
        ; Clobbers: A, X, Y

        ; --- add or subtract gold ---
        BYEQ    #$00,_MODIFY_CHAR_STATS.subtract
        LDY     #MFIELD_GOLD        ; \ add NUM_VALUE to gold
        LDA     (EFFECT_REC),Y         ;  |
        CLC                            ;  |
        ADC     NUM_VALUE              ;  |
        STA     (EFFECT_REC),Y         ;  |
        INY                            ;  | Y = MFIELD_GOLD+1
        LDA     (EFFECT_REC),Y         ;  |
        ADC     NUM_VALUE+1            ;  |
        STA     (EFFECT_REC),Y         ; /
        JMP     .aftermath
    _MODIFY_CHAR_STATS.subtract:
        LDY     #MFIELD_GOLD        ; \ subtract NUM_VALUE from gold
        SEC                            ;  |
        LDA     (EFFECT_REC),Y         ;  |
        SBC     NUM_VALUE              ;  |
        STA     (EFFECT_REC),Y         ;  |
        INY                            ;  | Y = MFIELD_GOLD+1
        LDA     (EFFECT_REC),Y         ;  |
        SBC     NUM_VALUE+1            ;  |
        STA     (EFFECT_REC),Y         ; /

        ; --- recompute level encoding from gold and strength ---
    .aftermath:
        LDY     #MFIELD_STRENGTH       ; \ compute threshold step
        LDA     (EFFECT_REC),Y         ;  | from strength (bits 4-0)
        ROL2                           ;  | shift left 3 bits
        ROL                            ;  |
        AND     #$F8                   ;  |
        STA     STAT_SCALE             ; /
        LDY     #MFIELD_LEVEL_ENC      ; \ unpack current level encoding
        LDA     (EFFECT_REC),Y         ;  |
        PHA                            ;  |
        AND     #$0F                   ;  | X = low nibble (base level)
        TAX                            ;  |
        PLA                            ;  |
        ROR                            ;  | \ extract high nibble
        ROR                            ;  |  | (XP multiplier)
        ROR                            ;  |  |
        ROR                            ;  | /
        AND     #$0F                   ;  |
        TAY                            ; / Y = high nibble
    .decy:
        DEY                            ; \ add high nibble to base level
        BMI     .done_cnt              ;  |
        INX                            ;  |
        BPL     .decy                  ; /
    .done_cnt:
        INY                            ; Y = 0
        STY     STAT_LEVEL_BITS        ; init level bits accumulator
        STY     LEVEL_THRESHOLD        ; \ init 16-bit threshold
        STY     LEVEL_THRESHOLD+1     ; / accumulator to 0
    .outer_loop:
        LDY     #MFIELD_GOLD+1
        BXEQ    #$00,_MODIFY_CHAR_STATS.store_final      ; no levels remain -> store
        CLC                            ; \ add STAT_SCALE to threshold
        LDA     LEVEL_THRESHOLD        ;  |
        ADC     STAT_SCALE             ;  |
        STA     LEVEL_THRESHOLD        ;  |
        BCC     .no_carry              ;  |
        INC     LEVEL_THRESHOLD+1     ; /
    .no_carry:
        LDA     (EFFECT_REC),Y         ; gold high byte
        BALT    LEVEL_THRESHOLD+1,_MODIFY_CHAR_STATS.store_final ; gold_hi < thresh_hi -> done
        BNE     .next_iter             ; gold_hi > thresh_hi -> next
        DEY                            ; Y = MFIELD_GOLD
        LDA     (EFFECT_REC),Y         ; gold low byte
        BALT    LEVEL_THRESHOLD,_MODIFY_CHAR_STATS.store_final ; gold_lo < thresh_lo -> done
    .next_iter:
        DEX                            ; one fewer level
        CLC                            ; \ advance level bits by $10
        LDA     STAT_LEVEL_BITS        ;  | (one level in high nibble)
        ADC     #$10                   ;  |
        STA     STAT_LEVEL_BITS        ; /
        BNE     .outer_loop
    _MODIFY_CHAR_STATS.store_final:
        TXA                            ; remaining levels = low nibble
        ORA     STAT_LEVEL_BITS        ; merge with level bits
        LDY     #MFIELD_LEVEL_ENC
        STA     (EFFECT_REC),Y         ; store recomputed encoding
        RTS
        ORG     $70D4
    MODIFY_CURRENT_STATS:
        SUBROUTINE MODIFY_CURRENT_STATS
        ; Wrapper: modify stats for the current entity.
        ;
        ; Input:  ENTITY_PTR — current entity
        ;         NUM_VALUE  — amount, Y — direction (see MODIFY_CHAR_STATS)
        ; Clobbers: A, X, Y

        MOVW    ENTITY_PTR,EFFECT_REC  ; copy entity ptr to effect rec
        JMP     MODIFY_CHAR_STATS      ; tail-call
        ORG     $7ABE
    WAIT_LOOP_INIT:
        DC.B    $20                     ; initial wait loop count
*/
#include "ali_baba.h"

/* The number of ROM WAIT calls TIMED_WAIT makes while a specific
 * encounter carries the mob off. The original keeps it in a code-page
 * byte, $7ABE, which nothing writes. */
#define WAIT_LOOP_INIT 0x20

/* The high byte of MOB_PTR, which the original tests for the end of a
 * location's mob list. */
#define MOB_PTR_HI ((uint8_t)((uint16_t)MOB_PTR >> 8))

/* CHAR_AI_MODE values from which SELECT_COMBAT_TARGET hunts a target. */
#define AI_MODE_HUNT_CLOSEST 3
#define AI_MODE_HUNT_STRONGEST 5

static uint8_t find_mob_at_same_pos(void);
static void sum_hostile_at_pos(uint8_t pos);
static void apply_damage(struct mob *rec, uint8_t damage);
static void modify_char_stats(struct mob *rec, uint16_t amount, bool add);
static void player_attack(void);
static void mob_attack(void);
static void process_entity_script(void);
static void event_prng_check(void);

/* ---- Who is where, and who is hostile -------------------------------- */

/* Looks through MOB_PTR's cell for a mob that is conscious and hostile
 * to the mob looked at before it. Each candidate's hostility is tested
 * against the previous candidate, so the first is tested against the
 * mob find_entity_at_pos returned. A search that wraps around to the
 * head of the location's list counts as friendly without a test. */
uint8_t check_encounter(uint8_t pos) {
    entity_ref e = find_entity_at_pos(pos);
    uint8_t result = 0;
    if (ENTITY_IS_MOB(e)) {
        MOB_PTR = ENTITY_MOB(e);
        for (;;) {
            const struct mob *ref = MOB_PTR;
            uint8_t found = find_mob_at_same_pos();
            if (found == 0)
                break;
            if (found == 1) {
                result = 1;
                break;
            }
            uint8_t hp = MOB_PTR->life & HP_MASK;
            if (hp >= 3 && check_hostile(ref)) {
                result = 2;
                break;
            }
        }
    }
    ENCOUNTER_RESULT = result;
    return result;
}

/* Moves MOB_PTR to the next mob in CHAR_PTR's list that is in MOB_PTR's
 * cell, going round to the head of the list after the end. Returns 0
 * when the walk comes back to MOB_PTR itself, which leaves MOB_PTR
 * unchanged, 2 for a mob found before the end of the list, and 1 for
 * one found after going round. The walk recognizes the starting mob by
 * its next link, so it stops at any mob whose link is the same. */
static uint8_t find_mob_at_same_pos(void) {
    COLOCATE_POS = MOB_PTR->pos;
    uint8_t link = MOB_PTR->next;
    COLOCATE_LINK = link;
    COLOCATE_WRAPPED = 2;
    for (;;) {
        if (link == 0) {
            COLOCATE_WRAPPED = 1;
            link = CHAR_PTR->mob_head;
        }
        struct mob *m = get_mob_data(link);
        if (m->next == COLOCATE_LINK)
            return 0;
        if (m->pos == COLOCATE_POS) {
            MOB_PTR = m;
            return COLOCATE_WRAPPED;
        }
        link = m->next;
    }
}

/* Returns 0 for the friendly font groups, 0-2 and 7, and 1 for the
 * others. */
static uint8_t get_hostility(uint8_t group) {
    return group >= 3 && group != 7;
}

bool check_hostile(const struct mob *other) {
    HOSTILE_APP = MOB_PTR->appearance;
    uint16_t glyph_group = appearance_to_fontchar(other->appearance);
    HOSTILE_A = (uint8_t)glyph_group;
    HOSTILE_Y = get_hostility((uint8_t)(glyph_group >> 8));
    glyph_group = appearance_to_fontchar(HOSTILE_APP);
    uint8_t hostility = get_hostility((uint8_t)(glyph_group >> 8));
    if (hostility != HOSTILE_Y)
        return true;
    /* Two hostile mobs are on different sides when their glyphs
     * differ. */
    return hostility != 0 && (uint8_t)glyph_group != HOSTILE_A;
}

/* The left and right neighbors are pos - 1 and pos + 1 even at the
 * edge of a row, as in the original, so a mob in column 0 counts the
 * mob at the end of the row above. */
uint8_t check_adjacent_threats(uint8_t pos) {
    ADJACENT_POS = pos;
    ADJACENT_THREAT = 0;
    if (pos != 0)
        sum_hostile_at_pos(pos - 1);
    if (pos != MAP_WIDTH * MAP_HEIGHT - 1)
        sum_hostile_at_pos(pos + 1);
    if (pos >= MAP_WIDTH)
        sum_hostile_at_pos(pos - MAP_WIDTH);
    if (pos < MAP_WIDTH * (MAP_HEIGHT - 1))
        sum_hostile_at_pos(pos + MAP_WIDTH);
    uint8_t defense = (ENTITY_PTR->strength >> 1) & 0x0F;
    return defense >= ADJACENT_THREAT;
}

/* Adds the strength of each mob at `pos` that is hostile to ENTITY_PTR
 * and not engaged in combat to ADJACENT_THREAT, which stops at $7F.
 * Leaves MOB_PTR on the last mob it looked at. */
static void sum_hostile_at_pos(uint8_t pos) {
    entity_ref e = find_entity_at_pos(pos);
    if (!ENTITY_IS_MOB(e))
        return;
    MOB_PTR = ENTITY_MOB(e);
    do {
        if (check_hostile(ENTITY_PTR) && !(MOB_PTR->flags & FLAGS_ENGAGED_BIT)) {
            uint8_t threat = (MOB_PTR->strength & STRENGTH_MASK) + ADJACENT_THREAT;
            if (threat >= 0x80)
                threat = 0x7F;
            ADJACENT_THREAT = threat;
        }
    } while (find_mob_at_same_pos() == 2);
}

uint8_t check_threats_here(void) {
    return check_adjacent_threats(colrow_to_pos(CURRENT_COL, CURRENT_ROW));
}

/* Moves MOB_PTR along CHAR_PTR's list to the next mob in its cell
 * with an appearance of $15 or more, and returns 1. When the walk
 * comes back round to a mob whose next link is the starting mob's, it
 * returns 0 and leaves MOB_PTR there. */
static uint8_t next_colocated_mob(void) {
    SAVED_MOB_LINK = MOB_PTR->next;
    do {
        find_mob_at_same_pos();
        if (MOB_PTR->next == SAVED_MOB_LINK)
            return 0;
    } while (MOB_PTR->appearance < NPC_APPEARANCE_MIN);
    return 1;
}

/* Draws a number from 1 to CHAR_PTR's count of player characters,
 * drawing again on 0, and walks the location's list to that player
 * character. Mobs with an appearance of $15 or more aren't counted. */
void pick_random_member(void) {
    uint8_t n;
    do {
        uint8_t count = CHAR_PTR->pc_count;
        if (count == 0) {
            /* This stores count, which is 0, as the original does. A
             * constant 0 makes c6502 address MOB_PTR's fields in the
             * loop below through a copy of the pointer (see
             * notes/combat.md). */
            MOB_PTR = (struct mob *)(uint16_t)count;
            return;
        }
        n = random_in_range(count);
    } while (n == 0);
    first_group_member();
    for (;;) {
        if (MOB_PTR->appearance < NPC_APPEARANCE_MIN && --n == 0)
            return;
        next_group_member();
    }
}

bool check_attack_eligible(void) {
    MOB_PTR = ENTITY_PTR;
    for (;;) {
        find_mob_at_same_pos();
        if (MOB_PTR == ENTITY_PTR)
            return false;
        if (check_hostile(ENTITY_PTR))
            return true;
    }
}

/* ---- Damage and healing ------------------------------------------------ */

/*
    Damages `rec` by `damage` & $1F, with a message for the size of the
    blow and one for how the mob takes it. A mob left with 2 or less
    life is marked engaged in state 0. A mob whose life runs out
    leaves its location's list, and its gold drops where it stood.
*/
static void apply_damage(struct mob *rec, uint8_t damage) {
    damage &= STRENGTH_MASK;
    DAMAGE_AMOUNT = damage;
    set_cursor_row21();
    /* The message is 4 for a blow of 0 or 1, and one more for each
     * further bit of the blow, up to 8 for 16-31. */
    uint8_t msg = 3;
    do {
        msg++;
        damage >>= 1;
    } while (damage != 0);
    display_message(msg);
    tune_player(TUNE_COMBAT_DAMAGE, DAMAGE_AMOUNT);

    uint8_t life = rec->life;
    uint8_t hp = life & HP_MASK;
    if (hp > DAMAGE_AMOUNT) {
        uint8_t left = hp - DAMAGE_AMOUNT;
        DAMAGE_AMOUNT = left;
        rec->life = (life & HP_FLAGS_MASK) | left;
        set_cursor_row21();
        if (left < 3) {
            rec->flags = (rec->flags & 0xF8) | FLAGS_ENGAGED_BIT;
            print_entity_name(rec);
            display_message(MSG_FEELING_WEAK);
        } else if (left < 8) {
            print_entity_name(rec);
            display_message(MSG_LOTS_OF_FIGHT);
        } else {
            print_entity_name(rec);
            display_message(MSG_HIT_WALL);
        }
        return;
    }

    uint8_t tune = TUNE_MOB_DEATH;
    if (rec->appearance >= NPC_APPEARANCE_MIN) {
        rec->flags |= FLAGS_DEAD_BIT;
        tune = TUNE_NPC_DEATH;
    }
    print_entity_name(rec);
    if (random_in_range(3) != 0)
        msg = 0x0E;
    else
        msg = 0x27 + random_in_range(2);
    display_message(msg);
    tune_player(tune, 0);

    ACTIVE_CHAR = 0;
    SOURCE_CHAR = CURRENT_PLAYER;
    reorder_char(rec);
    uint16_t gold = rec->gold;
    modify_char_stats(rec, gold, false);
    update_event_list(rec->pos, gold);
    draw_char_at_pos(rec->pos);
}

void apply_damage_to_current(uint8_t damage) {
    apply_damage(ENTITY_PTR, damage);
}

static void apply_damage_to_mob(uint8_t damage) {
    apply_damage(MOB_PTR, damage);
}

/* Adds 1 to ENTITY_PTR's life byte, carrying into its flag bits at 63,
 * and announces the mob when its life reaches 3. HEAL_NOTIFIED records
 * the announcement with the message's number. */
static void increment_hp(void) {
    uint8_t life = ++ENTITY_PTR->life;
    if ((life & HP_MASK) == 3) {
        set_cursor_row21();
        print_mob_name();
        scroll_status_line();
        /* The flag holds the number of the message it records. */
        HEAL_NOTIFIED = 0x22;
        display_message(0x22);
    }
}

/* Half the time, adds 1 to the life of an ENTITY_PTR below 40 and keeps
 * adding while the PRNG's output is from $50 to $CF, which the
 * original's signed comparison with $50 selects. A player character
 * that wasn't announced on reaching 3 then gets a message of its own. */
void auto_heal(void) {
    uint8_t hp = ENTITY_PTR->life & HP_MASK;
    if (hp >= 40)
        return;
    if (random_in_range(1) == 0)
        return;
    HEAL_NOTIFIED = 0;
    do {
        increment_hp();
    } while ((int8_t)(step_prng() - 0x50) >= 0);
    if (ENTITY_PTR->appearance < NPC_APPEARANCE_MIN && HEAL_NOTIFIED == 0) {
        set_cursor_row21();
        print_mob_name();
        scroll_status_line();
        display_message(MSG_REGAIN_CONSC);
    }
}

/* ---- Attacks ------------------------------------------------------------ */

/* The original's comment says that the restricted flag is set when no
 * hostile is in the cell. The code sets it when one is. */
void enter_combat_state(uint8_t pos) {
    uint8_t result = check_encounter(pos);
    clear_restricted();
    if (result == 2)
        set_restricted();
    set_char_state(2);
}

/* Points MOB_PTR at the target select_combat_target chose, as a mob. */
static void load_target_ptr(void) {
    MOB_PTR = TARGET_PTR;
    IS_PLAYER_TURN = 0;
}

void start_combat(void) {
    set_char_state(3);
    load_target_ptr();
    resolve_attack();
}

/* Each step that leaves its byte negative goes on into the next entry,
 * so ATTACK_RIGHT's fall into ATTACK_LEFT undoes its step, ATTACK_LEFT's
 * fall into ATTACK_UP leaves the column at $FF and steps the row, and
 * ATTACK_UP's fall into ATTACK_DOWN undoes its step. */
uint8_t attack_right(void) {
    if ((int8_t)++CURRENT_COL >= 0)
        return find_attack_target();
    return attack_left();
}

uint8_t attack_left(void) {
    if ((int8_t)--CURRENT_COL >= 0)
        return find_attack_target();
    return attack_up();
}

uint8_t attack_up(void) {
    if ((int8_t)--CURRENT_ROW >= 0)
        return find_attack_target();
    return attack_down();
}

uint8_t attack_down(void) {
    ++CURRENT_ROW;
    return find_attack_target();
}

/* Attacks what is in the cell at CURRENT_COL, CURRENT_ROW. Anything
 * other than a mob (an event entry, or 0 or 1 for an empty cell) goes
 * to player_attack with IS_PLAYER_TURN 1. Among two or more mobs that
 * aren't player characters, the attack menu chooses. */
uint8_t find_attack_target(void) {
    entity_ref e = find_entity_at_pos(colrow_to_pos(CURRENT_COL, CURRENT_ROW));
    if (!ENTITY_IS_MOB(e)) {
        IS_PLAYER_TURN = 1;
        MOB_PTR = (struct mob *)e;
    } else {
        IS_PLAYER_TURN = 0;
        MOB_PTR = ENTITY_MOB(e);
        if (find_mob_at_same_pos() != 0) {
            next_colocated_mob();
            if (next_colocated_mob() != 0)
                return CHAIN_MENU_ATTACK_TARGET;
        }
    }
    resolve_attack();
    return CHAIN_RETURN;
}

/* Copies MOB_PTR's name into the attack menu's 20 places, with $20 after
 * the name's end. The count of characters left is tested as a signed
 * byte, as in the original. */
void init_attack_display(void) {
    const uint8_t *name = MOB_PTR->name;
    uint8_t left = name[0];
    for (uint8_t i = 1; i <= 20; i++) {
        uint8_t c = name[i];
        if ((int8_t)--left < 0)
            c = 0x20;
        ATTACK_NAME_BUF[i] = c;
    }
}

uint8_t select_someone_else(void) {
    next_colocated_mob();
    return CHAIN_MENU_ATTACK_TARGET;
}

/*
    Decides whether ENTITY_PTR goes through with its attack on MOB_PTR,
    and makes the attack if it does, which it does when a roll of 0-31
    is at most its willingness. Attacking anything but a mob
    (IS_PLAYER_TURN 1) has a willingness of 28. An attack on a mob has
    a willingness of the attacker's level, adjusted for its rage and
    for the target's flags, state, life, and defense, and held within
    0-30.
*/
void resolve_attack(void) {
    set_blink_target();
    if (IS_PLAYER_TURN == 1) {
        COMBAT_WILLINGNESS = 28;
    } else {
        uint8_t level = ENTITY_PTR->level;
        COMBAT_WILLINGNESS = level & LEVEL_MASK;
        if (level & 0x20) {
            /* The original clears bit 4, the top bit of the level, and
             * leaves the rage bit set. */
            ENTITY_PTR->level = level & 0xEF;
            COMBAT_WILLINGNESS += 24;
        }
        uint8_t target_level = MOB_PTR->level;
        if (target_level & 0x40)
            COMBAT_WILLINGNESS -= 4;
        if (target_level & 0x80)
            COMBAT_WILLINGNESS += 8;
        uint8_t flags = MOB_PTR->flags;
        if (flags & FLAGS_ENGAGED_BIT)
            COMBAT_WILLINGNESS += 6;
        switch (flags & FLAGS_STATE_MASK) {
        case 0: COMBAT_WILLINGNESS += 3; break;
        case 1: COMBAT_WILLINGNESS -= 1; break;
        case 3: COMBAT_WILLINGNESS -= 3; break;
        }
        uint8_t hp = MOB_PTR->life & HP_MASK;
        if (hp < 3)
            COMBAT_WILLINGNESS += 24;
        if (ENTITY_PTR->pos == MOB_PTR->pos) {
            TARGET_DEFENSE = (MOB_PTR->strength >> 1) & 0x0F;
            COMBAT_WILLINGNESS += ((ENTITY_PTR->strength >> 1) & 0x0F) - TARGET_DEFENSE;
        }
        if ((int8_t)COMBAT_WILLINGNESS < 0)
            COMBAT_WILLINGNESS = 0;
        else if (COMBAT_WILLINGNESS >= 31)
            COMBAT_WILLINGNESS = 30;
    }

    if (random_in_range(31) <= COMBAT_WILLINGNESS) {
        if (IS_PLAYER_TURN != 0)
            player_attack();
        else
            mob_attack();
        return;
    }
    set_cursor_row21();
    print_mob_name();
    scroll_status_line();
    print_bottom_centered(S_MISSES);
    scroll_status_line();
    if (IS_PLAYER_TURN != 1)
        print_member_name();
    delay_with_animation();
}

/*
    Sets COMBAT_STRENGTH to 1 plus two rolls of 0 to ENTITY_PTR's
    weapon: the dagger against a mob in its own cell, otherwise the
    sword. A player character attacking a mob fumbles 1 time in 14
    (in 81 with bit 7 of the weapon set), which marks the weapon $81,
    plays scene 1, and adds 1-11 to the strength, up to 31.
*/
static void calc_combat_strength(void) {
    uint8_t weapon = MFIELD_ATTACK;
    if (IS_PLAYER_TURN != 1 && MOB_PTR->pos == ENTITY_PTR->pos)
        weapon = MFIELD_DEFENSE;
    uint8_t *attacker = (uint8_t *)ENTITY_PTR;
    uint8_t skill = attacker[weapon] & 0x0F;
    COMBAT_STRENGTH = random_in_range(skill);
    COMBAT_STRENGTH += random_in_range(skill) + 1;

    if (CHAR_AI_MODE != 0 || IS_PLAYER_TURN != 0)
        return;
    if (random_in_range((attacker[weapon] & 0x80) ? 0x50 : 0x0D) != 0)
        return;
    attacker[weapon] = 0x81;
    set_cursor_row21();
    scene_loop(1);
    uint8_t strength = random_in_range(10) + COMBAT_STRENGTH + 1;
    if (strength >= 31)
        strength = 31;
    COMBAT_STRENGTH = strength;
}

/* Prints the verb for an attack of strength `value`. Verbs 1-7 are for
 * strengths 1-7, and then each verb covers two strengths up to 15 and
 * four up to 31. The verbs are strings one after another, so the loop
 * steps over index - 1 of them. Strengths are 1-31, so the original's
 * signed comparisons with 8 and 16 are unsigned ones here. */
static void map_value_to_string(uint8_t value) {
    uint8_t index = value;
    if (value >= 16)
        index = ((value >> 2) & 3) | 0x0C;
    else if (value >= 8)
        index = ((value >> 1) & 3) | 0x08;
    const uint8_t *verb = ATTACK_VERB_STRINGS;
    while (--index != 0)
        verb += verb[0] + 1;
    print_bottom_centered(verb);
}

/*
    Attacks something other than a mob. MOB_PTR holds what
    find_entity_at_pos returned, an event entry's address, 0 for an
    empty cell, or 1 for a cell on the viewport's edge, and the code
    reads it as an event entry in every case. An attack on a cell with
    no entry therefore reads and writes $0000-$0003, as the original's
    does. A hit on a random encounter (type below $40) turns it into a
    specific one. A strong enough blow on an object (type $FE) breaks
    it open or plays its scene.
*/
static void player_attack(void) {
    calc_combat_strength();
    set_cursor_row21();
    print_mob_name();
    scroll_status_line();
    map_value_to_string(COMBAT_STRENGTH);
    scroll_status_line();
    struct event *target = (struct event *)MOB_PTR;
    print_bottom_centered(classify_entity_type((entity_ref)(uint16_t)target));
    tune_player(TUNE_COMBAT_HIT, COMBAT_STRENGTH);

    uint8_t type = target->type;
    if (type == EVENT_TYPE_OBJECT) {
        uint8_t value = target->value;
        uint8_t kind = value & 0xC0;
        uint8_t sub = value & 0x1F;
        if (kind == 0xC0) {
            if (sub < 0x1D) {
                process_entity_script();
                set_cursor_row21();
                scene_loop(2);
                return;
            }
        } else if (kind != 0) {
            if (COMBAT_STRENGTH >= 10
                || ((value & 0x80) == 0 && COMBAT_STRENGTH >= 4)) {
                if (sub < 0x1F) {
                    process_entity_script();
                    set_cursor_row21();
                    uint8_t kept = target->value & 0x1F;
                    if (kept < 0x1B) {
                        scene_loop(3);
                        event_prng_check();
                        return;
                    }
                } else {
                    target->value = value - 1;
                    tune_player(TUNE_ENTITY_INTERACTION, 0);
                    dispatch_on_mode();
                    draw_char_at_pos(target->pos);
                    set_cursor_row21();
                }
                display_message(MSG_TOPPLES);
                event_prng_check();
                return;
            }
            dispatch_on_mode();
            set_cursor_row21();
            display_message(MSG_HEAVY_ROCK);
            return;
        }
    } else if (type < 0x40 && random_in_range(0x0D) < COMBAT_STRENGTH) {
        dispatch_on_mode();
        target->type |= 0x40;
        uint16_t colrow = pos_to_colrow(target->pos);
        FONT_ROW = (uint8_t)(colrow >> 8);
        FONT_COL = (uint8_t)colrow;
        get_entity_fontchar((entity_ref)(uint16_t)target);
        render_font_char();
        tune_player(TUNE_COMBAT_AVOIDED, 0);
        set_cursor_row21();
        display_message(MSG_INSERT_DISK);
        return;
    }
    dispatch_on_mode();
    scroll_status_line();
    display_message(MSG_TOUCHED);
}

/* Deletes the event entry MOB_PTR points at, plays TUNE_ENTITY_INTERACTION,
 * and redraws the entry's cell. The original's comment says that it
 * restores the entry, which the code doesn't. */
static void process_entity_script(void) {
    struct event *entry = (struct event *)MOB_PTR;
    uint8_t pos = entry->pos;
    entry->pos = EVENT_EMPTY;
    tune_player(TUNE_ENTITY_INTERACTION, 0);
    dispatch_on_mode();
    draw_char_at_pos(pos);
}

/* Makes ENTITY_PTR's attack on the mob MOB_PTR. The target's armor
 * (bits 3-0 of its armor byte) is taken off the strength, and bit 7 of
 * that byte picks the message. The original's comment calls MOB_PTR
 * the attacker, which the code doesn't bear out. */
static void mob_attack(void) {
    calc_combat_strength();
    set_cursor_row21();
    print_mob_name();
    scroll_status_line();
    map_value_to_string(COMBAT_STRENGTH);
    scroll_status_line();
    print_member_name();
    tune_player(TUNE_COMBAT_HIT, COMBAT_STRENGTH);
    if (ENTITY_PTR->appearance < NPC_APPEARANCE_MIN)
        SCENE_COMBAT_FLAG = 0;
    dispatch_on_mode();

    MOB_DEFENSE = MOB_PTR->armor;
    if ((MOB_DEFENSE & 0x0F) == 0) {
        apply_damage_to_mob(COMBAT_STRENGTH);
        return;
    }
    scroll_status_line();
    uint8_t msg = (MOB_DEFENSE & 0x80) ? 0x17 : 0x18;
    MOB_DEFENSE &= 0x0F;
    uint8_t through = COMBAT_STRENGTH - MOB_DEFENSE;
    if (through != 0 && (int8_t)through >= 0) {
        COMBAT_STRENGTH = through;
        display_message(msg);
        apply_damage_to_mob(COMBAT_STRENGTH);
    } else if ((uint8_t)(MOB_PTR->life & HP_MASK) < 3) {
        display_message(MSG_ENOUGH);
    } else {
        display_message(msg + 2);
    }
}

/* Returns 2 when MOB_PTR is hostile to ENTITY_PTR and in its cell, 1
 * when it is hostile and next to it, and 0 otherwise. The original
 * returns 2 for a distance of 129 or more too, which no two positions
 * are apart. */
static uint8_t get_entity_threat(void) {
    if (!check_hostile(ENTITY_PTR))
        return 0;
    uint8_t distance = manhattan_distance(MOB_PTR->pos, ENTITY_PTR->pos);
    if (distance == 1)
        return 1;
    return distance == 0 ? 2 : 0;
}

/*
    An AI mob with CHAR_AI_MODE 5 or more picks the strongest hostile
    mob next to it or in its cell, and one with mode 3 or 4 the closest,
    and either returns 1 with TARGET_PTR on it. A mob with no such
    target, or with a lower mode, returns 2 to enter combat when it has
    moved, when a hostile mob shares its cell, or when the hostile
    strength next to it is more than its defense, and 4 to rest
    otherwise.
*/
uint8_t select_combat_target(void) {
    if (CHAR_AI_MODE >= AI_MODE_HUNT_CLOSEST) {
        if (CHAR_AI_MODE >= AI_MODE_HUNT_STRONGEST) {
            first_group_member();
            BEST_THREAT = 0;
            do {
                if (get_entity_threat() != 0) {
                    uint8_t strength = MOB_PTR->strength & STRENGTH_MASK;
                    if (strength >= BEST_THREAT) {
                        BEST_THREAT = strength;
                        TARGET_PTR = MOB_PTR;
                    }
                }
                next_group_member();
            } while (MOB_PTR_HI != 0);
        } else {
            BEST_THREAT = 0;
            first_group_member();
            do {
                uint8_t threat = get_entity_threat();
                if (threat > BEST_THREAT) {
                    BEST_THREAT = threat;
                    TARGET_PTR = MOB_PTR;
                }
                next_group_member();
            } while (MOB_PTR_HI != 0);
        }
        if (BEST_THREAT != 0)
            return 1;
    }
    if (STEPS_TAKEN != 0)
        return 2;
    if (check_encounter(ENTITY_PTR->pos) == 2)
        return 2;
    if (check_threats_here() != 1)
        return 2;
    return 4;
}

/* ---- Walking into things ------------------------------------------------ */

/*
    ENTITY_PTR walks into an encounter entry, a type below $C0. For a
    random encounter (type below $40), a mob whose strength reaches a
    roll of 0-20 avoids it, which marks the entry with $40. Otherwise
    the door opens, or now and then scene 9 plays and costs a point of
    life. A specific encounter carries the mob to the entry's value, a
    position in location ACTIVE_CHAR, which the type's low 6 bits give
    or the PRNG picks.
*/
void handle_encounter(struct event *ev) {
    set_char_state(0);
    if (ev->type < EVENT_TYPE_MOB) {
        PRNG_OUTPUT = random_in_range(20);
        uint8_t strength = ENTITY_PTR->strength & STRENGTH_MASK;
        if (strength >= PRNG_OUTPUT) {
            ev->type |= 0x40;
            TURN_START_COL = CURRENT_COL;
            TURN_START_ROW = CURRENT_ROW;
            reset_turn_pos();
            BLINK_COL = TURN_START_COL;
            BLINK_ROW = TURN_START_ROW;
            BLINK_CHAR = FONT_CHARNUM;
            tune_player(TUNE_COMBAT_AVOIDED, 0);
            set_cursor_row21();
            display_message(MSG_INSERT_DISK);
            return;
        }
        /* The original compares $74 with the PRNG's output as signed
         * numbers, so outputs from $75 to $F4 play the scene. */
        if ((int8_t)(0x74 - step_prng()) < 0) {
            set_cursor_row21();
            scene_loop(9);
            apply_damage_to_current(1);
            return;
        }
        set_cursor_row21();
        display_message(MSG_DOOR_OPENS);
        tune_player(TUNE_ENCOUNTER, 0);
        return;
    }

    EVENT_POS = ev->pos;
    set_char_state(1);
    uint8_t dest = ev->type & 0x3F;
    if (dest == 0)
        dest = ((step_prng() >> 2) & 0x1C) | 1;
    ACTIVE_CHAR = dest;
    if (dest & 1) {
        uint8_t look = ENTITY_PTR->appearance;
        if (look >= 0x2A && look < 0x3F)
            ACTIVE_CHAR = 0;
    }
    EVENT_DEST_POS = ev->value;
    commit_move();
    WAIT_LOOP_COUNT = WAIT_LOOP_INIT;
    reorder_current_char();
    timed_wait();
    ENTITY_PTR->pos = EVENT_DEST_POS;
    if (ACTIVE_CHAR != CURRENT_PLAYER) {
        reset_turn_pos();
        return;
    }
    uint16_t colrow = pos_to_colrow(EVENT_DEST_POS);
    CURRENT_COL = (uint8_t)colrow;
    CURRENT_ROW = (uint8_t)(colrow >> 8);
    commit_move();
}

void show_locked_msg(void) {
    set_cursor_row21();
    print_mob_name();
    scroll_status_line();
    display_message(MSG_HIT_STATUE);
    set_char_state(0);
}

/*
    ENTITY_PTR walks into an object entry (type $FE). Bits 7-6 of the
    entry's value select what the object does, and bits 4-0 are a
    count or a kind.

    - $C0: the mob steps onto it, and below a kind of $1D the entry is
      deleted and, for a kind other than 0, scene $0A plays.
    - $00: kind $1B and up acts as a statue, and a lower kind costs a
      point of life.
    - $80: a mob whose strength doesn't reach a roll of 0-25 is hit by
      a rock, and the entry becomes a $40 one.
    - $40, and $80 after a good roll: below kind $1B, scene $13 plays
      and the entry is deleted. From $1B the sculpture topples, and a
      kind of $1F counts down to $1E and keeps the entry.
*/
uint8_t handle_event_slot(struct event *ev) {
    set_char_state(0);
    uint8_t value = ev->value;
    EVENT_SLOT_BYTE = value;
    uint8_t kind = value & 0xC0;
    if (kind == 0xC0) {
        uint8_t sub = value & 0x1F;
        if (sub < 0x1D) {
            ev->pos = EVENT_EMPTY;
            if (sub != 0)
                scene_loop(0x0A);
        }
        commit_move();
        if (check_threats_here() == 0)
            MOVE_POINTS = 0;
        return CHAIN_MOVE_LOOP;
    }

    tune_player(TUNE_ENTITY_INTERACTION, 0);
    if (kind == 0) {
        uint8_t sub = EVENT_SLOT_BYTE & 0x1F;
        if (sub >= 0x1B) {
            show_locked_msg();
            return CHAIN_RETURN;
        }
        set_cursor_row21();
        print_mob_name();
        scroll_status_line();
        display_message(MSG_DEATH);
        apply_damage_to_current(1);
        return CHAIN_RETURN;
    }

    if (kind != 0x40) {
        PRNG_OUTPUT = random_in_range(0x19);
        uint8_t strength = ENTITY_PTR->strength & STRENGTH_MASK;
        if (strength < PRNG_OUTPUT) {
            set_cursor_row21();
            display_message(MSG_HEAVY_ROCK);
            ev->value = (EVENT_SLOT_BYTE & 0x3F) | 0x40;
            event_prng_check();
            return CHAIN_RETURN;
        }
    }

    EVENT_SUB_VALUE = EVENT_SLOT_BYTE & 0x1F;
    bool keep = false;
    set_cursor_row21();
    if (EVENT_SUB_VALUE < 0x1B) {
        scene_loop(0x13);
    } else {
        display_message(MSG_TOPPLES);
        if (EVENT_SUB_VALUE >= 0x1F) {
            EVENT_SUB_VALUE--;
            ev->value = (EVENT_SLOT_BYTE & 0xE0) | EVENT_SUB_VALUE;
            keep = true;
        }
    }
    if (!keep)
        ev->pos = EVENT_EMPTY;
    TURN_START_COL = CURRENT_COL;
    TURN_START_ROW = CURRENT_ROW;
    reset_turn_pos();
    anim_tick_and_wait();
    event_prng_check();
    return CHAIN_RETURN;
}

/* Springs a trap on ENTITY_PTR when the PRNG's next output is below $14
 * or from $94 up, which the original's signed comparison with $14
 * selects. The trap restricts the mob and costs it 1 or 2 points of
 * life, 2 when bit 6 of the output after that is set. */
static void event_prng_check(void) {
    if ((int8_t)(step_prng() - 0x14) >= 0)
        return;
    set_cursor_row21();
    display_message(MSG_DUST_CLOUD);
    set_restricted();
    print_mob_name();
    delay_with_animation();
    apply_damage_to_current((step_prng() & 0x40) ? 2 : 1);
}

static void set_target_restricted(void) {
    MOB_PTR->flags |= FLAGS_ENGAGED_BIT;
}

/*
    ENTITY_PTR walks into the cell at CURRENT_COL, CURRENT_ROW, where
    there are mobs. With hostile mobs there, it tackles them unless a
    PRNG roll beats its courage, its strength + $10 less their strength
    held within 4-$1C. A failed tackle puts it back where its turn
    started and ends the turn. Otherwise it moves in, and when a hostile
    mob is then in its cell, every mob of the location in that cell is
    marked engaged and the turn ends.
*/
uint8_t handle_mob_encounter(void) {
    ADJACENT_THREAT = 0;
    sum_hostile_at_pos(colrow_to_pos(CURRENT_COL, CURRENT_ROW));
    if (ADJACENT_THREAT != 0) {
        uint8_t courage = (ENTITY_PTR->strength & STRENGTH_MASK) + 0x10 - ADJACENT_THREAT;
        if ((int8_t)courage < 0)
            courage = 4;
        else if (courage >= 0x1D)
            courage = 0x1C;
        uint8_t limit = (courage << 2) | 3;
        /* The original compares the limit with the PRNG's output as
         * signed numbers. */
        if ((int8_t)(limit - step_prng()) < 0) {
            set_cursor_row21();
            print_mob_name();
            scroll_status_line();
            display_message(MSG_FLYING_TACKLE);
            CURRENT_COL = TURN_START_COL;
            CURRENT_ROW = TURN_START_ROW;
            return CHAIN_END_TURN;
        }
    }

    commit_move();
    if (check_encounter(ENTITY_PTR->pos) < 2)
        return check_threats_here() == 1 ? CHAIN_MOVE_LOOP : CHAIN_END_TURN;

    first_group_member();
    while (MOB_PTR_HI != 0) {
        if (ENTITY_PTR->pos == MOB_PTR->pos)
            set_target_restricted();
        next_group_member();
    }
    set_cursor_row21();
    print_mob_name();
    scroll_status_line();
    display_message(MSG_UNCONSCIOUS);
    return CHAIN_END_TURN;
}

/* ---- Gold and the speed byte -------------------------------------------- */

/*
    Adds `amount` to rec's gold, or subtracts it, and recomputes rec's
    speed byte. The two nibbles of the old byte are summed into a count
    of levels, and each level that the gold pays for moves one level
    from the low nibble to the high one. Level n costs n times the
    mob's strength times 8, and the high nibble holds at most 15.
*/
static void modify_char_stats(struct mob *rec, uint16_t amount, bool add) {
    uint16_t gold = rec->gold;
    if (add)
        gold += amount;
    else
        gold -= amount;
    rec->gold = gold;
    STAT_SCALE = (uint8_t)(rec->strength << 3);
    uint8_t speed = rec->speed;
    uint8_t levels = (speed & 0x0F) + (speed >> 4);
    STAT_LEVEL_BITS = 0;
    uint16_t threshold = 0;
    while (levels != 0) {
        threshold += STAT_SCALE;
        if (gold < threshold)
            break;
        levels--;
        STAT_LEVEL_BITS += 0x10;
        if (STAT_LEVEL_BITS == 0)
            break;
    }
    rec->speed = levels | STAT_LEVEL_BITS;
}

void modify_current_stats(uint16_t amount, bool add) {
    modify_char_stats(ENTITY_PTR, amount, add);
}
