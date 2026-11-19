/*
    events.c -- what happens on the map without a fight: the event
    lists' gold, chests, and scenes, getting away from a fight, the
    random events between passes, the story's scenes, and the OTHER
    OPTIONS menu's gold dropping and level taking.

    Each location has a list of 3-byte event entries (struct event in
    ali_baba.h). Walking onto an entry's cell runs it. Gold dropped or
    left by the dead goes into the list, as a new entry or added to
    one already in the cell. Between passes over the locations,
    random_events wakes encounters, moves mobs that are out of play
    into locations of their faction, and takes mobs out of empty
    locations, each with a chance set by the DANGER setting.

    Departures from the original:

    - The routines that GAME_TURN_LOOP reaches with the entry in
      EVENT_PTR ($BE) take it as the parameter `ev`. TRIGGER_SCENE_EVENT
      saves EVENT_PTR in $5A5B across COMMIT_MOVE and reloads it, and
      trigger_scene_event keeps the store to SAVED_EVENT_PTR, which
      nothing reads, and uses `ev`.
    - The scratch bytes are locals and parameters: the gold amount
      (NUM_VALUE), the event pointer, CHAR_INDEX ($F6), FACTION_MASK
      ($F7), and TRAP_TYPE ($5A58). PICK_RANDOM_MOB and PICK_RANDOM_CHAR
      return CHAR_INDEX, which their callers pass on, and
      FIND_MOB_BY_FACTION returns whether it found a mob.
    - PICKUP_TREASURE reads the entry through MOB_PTR after it copies
      EVENT_PTR there. pickup_treasure makes the same copy, which other
      units read, and reads the entry through `ev`. Nothing it calls
      changes MOB_PTR in the original, and SCENE_LOOP restores it.
    - INC_STRENGTH and INC_LEVEL are one function, inc_stat, which takes
      the field's offset as the original's shared body takes it in Y.
      ADJ_DROP_INC and ADJ_DROP_DEC share adj_drop, which takes the
      direction that the original passes in X.
    - The chain routines' JMPs to MENU_DROP_GOLD, MENU_OTHER_OPTIONS,
      and MOVE_LOOP return the CHAIN_ states, and ADJ_DROP's JMP to
      INIT_DROP_GOLD returns what init_drop_gold returns.
    - EVENT_PICK_MOB, EVENT_PICK_CHAR, and EVENT_FACTION_SCENE, which
      only call a picking routine and go on to another, are written out
      in random_events. COPY_BE_TO_FA is the end of each picking
      routine.
    - DROP_GOLD_EXEC clears DROP_STEP's high byte before it calls
      MODIFY_CURRENT_STATS and stores the random step's low byte after.
      drop_gold_exec stores both bytes after the call. Nothing reads
      DROP_STEP in between.
    - INIT_DROP_GOLD stores DROP_AMOUNT a byte at a time, and
      init_drop_gold stores the whole word, which writes the high byte
      of 0 a second time when the gold is below 256.
    - ENTITY_PTR's gold and CHAR_PTR's event pointer are read a byte at
      a time by offset (ENTITY_BYTE and CHAR_BYTE), because c6502
      computes the address of a 2-byte member before it reads the
      member through a zero-page pointer.

    Where the original's comments and its code disagree, the C follows
    the code, and the function comments say where.

    Ali Baba, main.asm lines 3158-3266, 5106-5151, 11215-11723,
    12933-13049, 13070-13361:

        ORG     $1231
    CHECK_CAN_LEAVE:
        SUBROUTINE CHECK_CAN_LEAVE
        ; PRNG check whether entity can leave area.
        ;
        ; Input:  MOB_PTR -- mob whose name to print
        ; Output: A = 1 (can leave), A = 0 (blocked)
        ; Behavior:
        ;   - Steps PRNG; if output >= $40 (~75%):
        ;     prints mob name + msg $1C, returns 1
        ;   - Otherwise: msg $1D, returns 0
        ; Clobbers: A, X, Y

        JSR     STEP_PRNG
        LDA     PRNG_OUTPUT
        BAPL    #$40,_CHECK_CAN_LEAVE.pass
        JMP     .fail
    _CHECK_CAN_LEAVE.pass:
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        LDA     #MSG_ESCAPE_FAIL       ; "fails to escape"
        JSR     DISPLAY_MESSAGE
        LDA     #$01                   ; success
        RTS
    .fail:
        JSR     SET_CURSOR_ROW21
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        LDA     #MSG_TACKLE_FAIL       ; "fails to tackle"
        JSR     DISPLAY_MESSAGE
        LDA     #$00                   ; failure
        RTS
        ORG     $1260
    UPDATE_EVENT_LIST:
        SUBROUTINE UPDATE_EVENT_LIST
        ; Insert or update a 3-byte event entry.
        ;
        ; Input:  A          -- map position
        ;         NUM_VALUE   -- value low byte
        ;         NUM_VALUE+1 -- value high byte
        ;         CHAR_PTR   -- char record (fields
        ;                       6-7 = event list ptr)
        ; Output: event list updated in place
        ; Behavior:
        ;   - If value = 0, returns immediately
        ;   - Scans 3-byte entries via CFIELD_EVENTS:
        ;     $FF = end -> msg $2A (list full)
        ;     $FE = empty -> insert new entry
        ;     same pos + type $C0-$D0 -> add value
        ;     else -> advance to next entry
        ; Clobbers: A, Y

        STA     EVENT_SAVED_POS        ; save position
        LDA     #$00
        BANE    NUM_VALUE,_UPDATE_EVENT_LIST.has_value
        BANE    NUM_VALUE+1,_UPDATE_EVENT_LIST.has_value
        RTS
    _UPDATE_EVENT_LIST.has_value:
        LDWIY    CHAR_PTR,#CFIELD_EVENTS,EVENT_PTR ; event list ptr
    .scan:
        LDY     #EVFIELD_POSITION
        LDA     (EVENT_PTR),Y          ; byte 0 = position
        BANE    #EVENT_END,_UPDATE_EVENT_LIST.not_end    ; $FF = end of list
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_ARMOR_WORTHLESS
        JMP     DISPLAY_MESSAGE        ; list full message
    _UPDATE_EVENT_LIST.not_end:
        BANE    #EVENT_EMPTY,_UPDATE_EVENT_LIST.check_match ; $FE = empty slot
        ; --- Insert new entry into empty slot ---
        LDA     EVENT_SAVED_POS
        STA     (EVENT_PTR),Y          ; byte 0 = position
        INY                            ; Y = EVFIELD_TYPE
        LDA     NUM_VALUE+1
        AND     #EVENT_VALUE_HI_MASK   ; keep high 3 bits of value
        ORA     #EVENT_ACTIVATED       ; mark as activated ($C0+)
        STA     (EVENT_PTR),Y
        INY                            ; Y = EVFIELD_VALUE
        LDA     NUM_VALUE
        STA     (EVENT_PTR),Y          ; byte 2 = value low
        RTS
    _UPDATE_EVENT_LIST.check_match:
        BANE    EVENT_SAVED_POS,_UPDATE_EVENT_LIST.next  ; same position?
        INY                            ; Y = EVFIELD_TYPE
        LDA     (EVENT_PTR),Y          ; byte 1 = type
        BPL     _UPDATE_EVENT_LIST.next                  ; not activated (< $80)
        BAMI    #EVENT_ACTIVATED,_UPDATE_EVENT_LIST.next ; below $C0
        BAPL    #EVENT_ACCUM_MAX,_UPDATE_EVENT_LIST.next ; above $D0 -> not accumulable
        ; --- Add value to existing entry ---
        CLC
        LDA     NUM_VALUE
        INY                            ; Y = EVFIELD_VALUE
        ADC     (EVENT_PTR),Y          ; byte 2 += value low
        STA     (EVENT_PTR),Y
        DEY                            ; Y = EVFIELD_TYPE
        LDA     NUM_VALUE+1
        AND     #EVENT_VALUE_HI_MASK   ; high 3 bits of value
        ADC     (EVENT_PTR),Y          ; byte 1 += value high
        STA     (EVENT_PTR),Y
        RTS
    _UPDATE_EVENT_LIST.next:
        CLC
        LDA     EVENT_PTR
        ADC     #EVENT_RECORD_SIZE     ; next 3-byte entry
        STA     EVENT_PTR
        BCC     .no_carry
        INC     EVENT_PTR+1
    .no_carry:
        JMP     .scan

    ...

        ORG     $1D04
    TRIGGER_SCENE_EVENT:
        SUBROUTINE TRIGGER_SCENE_EVENT
        ; Trigger a scripted scene from event data.
        ;
        ; Input:  EVENT_PTR  -- 3-byte event entry
        ;         ENTITY_PTR -- current character
        ; Output: none (side-effects only)
        ; Behavior:
        ;   - Clears char turn state, commits move
        ;   - Event byte 2 = scene index
        ;   - Event byte 1 = type:
        ;     $D2: trigger scene only
        ;     $D3: trigger scene, mark empty
        ;     $D4: scene + bonus $0F + 1-3 dmg
        ; Clobbers: A, X, Y

        LDA     #$00
        JSR     SET_CHAR_STATE          ; clear char turn state
        MOVW    EVENT_PTR,$5A5B         ; save EVENT_PTR across call
        JSR     COMMIT_MOVE             ; commit move
        MOVW    $5A5B,EVENT_PTR         ; restore EVENT_PTR
        LDY     #$02
        LDA     (EVENT_PTR),Y           ; event byte 2 = scene index
        JSR     SCENE_LOOP              ; trigger scene
        LDY     #$01
        LDA     (EVENT_PTR),Y           ; event byte 1 = type
        BAPL    #$D3,_TRIGGER_SCENE_EVENT.d3_or_above
        RTS                             ; $D2: trigger scene only
    _TRIGGER_SCENE_EVENT.d3_or_above:
        BNE     .d4
        LDA     #$FE                    ; $D3: mark event empty
        DEY
        STA     (EVENT_PTR),Y
        RTS
    .d4:
        LDA     #$FE                    ; $D4: mark event empty
        DEY
        STA     (EVENT_PTR),Y
        LDA     #$0F
        JSR     SCENE_LOOP              ; trigger bonus scene $0F
        LDA     #$03
        JSR     RANDOM_IN_RANGE         ; random(3)
        CLC
        ADC     #$01                    ; 1-3
        JMP     APPLY_DAMAGE_TO_CURRENT ; apply damage to char

    ...

        ORG     $646B
    PICKUP_TREASURE:
        SUBROUTINE PICKUP_TREASURE
        ; Handle treasure chest pickup from event data.
        ;
        ; Input:  EVENT_PTR  -- 3-byte event entry
        ;         ENTITY_PTR -- current character
        ; Output: event slot marked empty ($FE)
        ; Behavior:
        ;   - Clears char turn state, commits move
        ;   - Byte 1 < $C9: normal treasure
        ;     (subtract $C0 for index, byte 2 =
        ;     quantity, add to inventory if room)
        ;   - Byte 1 >= $C9, != $D1: trapped chest
        ;     (4 trap types: HP loss, counter dec,
        ;     gold zeroed; scene $36-$39)
        ;   - Byte 1 = $D1: special chest (scene
        ;     $0B, inc fields 5+11, 1/3 bonus)
        ; Clobbers: A, X, Y

        LDA     #$00
        JSR     SET_CHAR_STATE          ; clear char turn state
        MOVW    EVENT_PTR,MOB_PTR       ; copy EVENT_PTR -> MOB_PTR
        JSR     COMMIT_MOVE             ; commit move
        LDY     #$01
        LDA     (MOB_PTR),Y            ; event byte 1 = treasure type
        BAPL    #$C9,_PICKUP_TREASURE.trapped           ; >= $C9 -> trapped or special
        SEC
        SBC     #$C0                    ; $C0-$C8: treasure index 0-8
        STA     NUM_VALUE+1
    .rejoin:
        LDY     #$02
        LDA     (MOB_PTR),Y            ; event byte 2 = quantity
        STA     NUM_VALUE
        LDY     #$01
        JSR     MODIFY_CURRENT_STATS   ; apply treasure to stats
        LDY     #MFIELD_APPEARANCE
        LDA     (ENTITY_PTR),Y         ; char field 4 = appearance
        BAGE    #$15,_PICKUP_TREASURE.mark_used        ; inventory full -> skip
        JSR     CLEAR_TREASURE_DIGITS  ; clear display digit fields
        STOW    S_REVEAL_DIGITS,DECIMAL_OUT_PTR ; output -> quantity digits
        JSR     NUM_TO_DECIMAL         ; format quantity
        LDWIY    ENTITY_PTR,#MFIELD_GOLD,NUM_VALUE ; gold low
        STOW    S_NOW_HAVE_DIGITS,DECIMAL_OUT_PTR ; output -> gold digits
        JSR     NUM_TO_DECIMAL         ; format gold amount
        JSR     SET_CURSOR_ROW21
        STOW    S_CHEST_OPENS,NUM_VALUE ; message pointer
        JSR     CLEAR_AND_DISPLAY      ; display pickup message
    _PICKUP_TREASURE.mark_used:
        LDY     #$00
        LDA     #$FE                   ; mark event slot empty
        STA     (MOB_PTR),Y
        RTS

        ; --- trapped chest (type >= $C9) ---
    _PICKUP_TREASURE.trapped:
        BANE    #$D1,_PICKUP_TREASURE.not_d1
        JMP     .handle_d1             ; $D1: special chest
    _PICKUP_TREASURE.not_d1:
        LDA     #$06
        JSR     RANDOM_IN_RANGE        ; random(6)
        AND     #$03                   ; trap type 0-3
        STA     TRAP_TYPE
        LDY     #MFIELD_APPEARANCE
        LDA     (ENTITY_PTR),Y        ; char field 4 = appearance
        BMI     _PICKUP_TREASURE.apply_trap
        BAPL    #$15,_PICKUP_TREASURE.apply_trap
        JSR     SET_CURSOR_ROW21
        LDA     #$36
        CLC
        ADC     TRAP_TYPE              ; scene $36 + trap type
        JSR     SCENE_LOOP             ; show trap scene
    _PICKUP_TREASURE.apply_trap:
        LDA     TRAP_TYPE
        BAMI    #$01,_PICKUP_TREASURE.trap0            ; trap 0: lose 1-5 HP
        BEQ     .trap1                 ; trap 1: lose 2-9 HP
        BAEQ    #$02,_PICKUP_TREASURE.trap2            ; trap 2: decrement level
        BNE     .trap3                 ; trap 3: zero gold
    _PICKUP_TREASURE.trap0:
        LDA     #$05
        JSR     RANDOM_IN_RANGE
        CLC
        ADC     #$01                   ; 1-5 damage
        JSR     APPLY_DAMAGE_TO_CURRENT
        JMP     .after_trap
    .trap1:
        LDA     #$08
        JSR     RANDOM_IN_RANGE
        CLC
        ADC     #$02                   ; 2-9 damage
        JSR     APPLY_DAMAGE_TO_CURRENT
        JMP     .after_trap
    _PICKUP_TREASURE.trap2:
        LDY     #MFIELD_LEVEL
        LDA     (ENTITY_PTR),Y        ; char field 11 = level
        AND     #LEVEL_MASK            ; isolate level bits 4-0
        BEQ     .after_trap            ; already zero
        LDA     (ENTITY_PTR),Y
        SEC
        SBC     #$01                   ; decrement low 5 bits
        STA     (ENTITY_PTR),Y
        JMP     .after_trap
    .trap3:
        LDY     #MFIELD_GOLD
        LDA     #$00
        STA     (ENTITY_PTR),Y        ; zero gold low
        INY
        STA     (ENTITY_PTR),Y        ; zero gold high
    .after_trap:
        LDY     #MFIELD_NEXT
        LDA     (ENTITY_PTR),Y        ; char field 2 = room
        BAEQ    CURRENT_ROOM,_PICKUP_TREASURE.same_room ; current room?
        DEY
        LDA     (MOB_PTR),Y           ; event byte 1
        SEC
        SBC     #$09                   ; step back 9 in type
        STA     (MOB_PTR),Y
        RTS
    _PICKUP_TREASURE.same_room:
        DEY
        LDA     (MOB_PTR),Y           ; event byte 1
        SEC
        SBC     #$C9                   ; convert to treasure index
        STA     NUM_VALUE+1
        JMP     .rejoin                ; process as normal treasure

        ; --- special chest $D1 ---
    .handle_d1:
        LDA     #$0B
        JSR     SCENE_LOOP             ; show scene $0B
        JSR     INC_STRENGTH     ; inc field 5 counter
        JSR     INC_LEVEL      ; inc field 11 counter
        LDA     #$03
        JSR     RANDOM_IN_RANGE        ; random(3)
        BAEQ    #$00,_PICKUP_TREASURE.lucky
        RTS                            ; 2/3 chance: nothing
    _PICKUP_TREASURE.lucky:
        LDA     #$0C
        JSR     SCENE_LOOP             ; show bonus scene $0C
        JMP     _PICKUP_TREASURE.mark_used             ; mark event used
        ORG     $657B
    INC_STRENGTH:
        SUBROUTINE INC_STRENGTH
        ; Increment a 5-bit counter in a character record field.
        ;
        ; Entry points:
        ;   INC_STRENGTH — increments MFIELD_STRENGTH (byte 5)
        ;   INC_LEVEL    — increments MFIELD_LEVEL (byte 11)
        ;
        ; Inputs:
        ;   ENTITY_PTR — pointer to entity record
        ;
        ; Behavior:
        ;   Isolates bits 4-0, checks if already at max (31),
        ;   and if not, adds 1 to the full byte (preserving upper bits).
        ;
        ; Clobbers: A, X, Y

        LDY     #MFIELD_STRENGTH       ; byte 5: strength
        JMP     .body
    INC_LEVEL:
        LDY     #MFIELD_LEVEL          ; byte 11: level
    .body:
        LDA     (ENTITY_PTR),Y
        TAX                            ; save original byte
        AND     #LEVEL_MASK            ; isolate low 5 bits
        BANE    #LEVEL_MASK,_INC_STRENGTH.inc       ; already at max (31)?
        RTS
    _INC_STRENGTH.inc:
        CLC
        TXA                            ; restore original byte
        ADC     #$01                   ; increment (preserves upper bits)
        STA     (ENTITY_PTR),Y
        RTS
        ORG     $6593
    CLEAR_TREASURE_DIGITS:
        SUBROUTINE CLEAR_TREASURE_DIGITS
        ; Clear the digit fields in the treasure pickup message strings.
        ;
        ; Behavior:
        ;   Fills S_REVEAL_DIGITS (5 bytes) and S_NOW_HAVE_DIGITS (5 bytes)
        ;   with spaces to erase stale digits before formatting new values.
        ;
        ; Modifies: S_REVEAL_DIGITS, S_NOW_HAVE_DIGITS
        ; Clobbers: A, X

        LDA     #$20                   ; space character
        LDX     #$04                   ; 5 bytes (index 4..0)
    .loop:
        STA     S_REVEAL_DIGITS,X      ; quantity digit field
        STA     S_NOW_HAVE_DIGITS,X    ; gold digit field
        DEX
        BPL     .loop
        RTS
        ORG     $65A1
    CLEAR_AND_DISPLAY:
        SUBROUTINE CLEAR_AND_DISPLAY
        ; Clear message state and display message.
        ;
        ; Input:  NUM_VALUE -- message address
        ; Output: none (displays message)
        ; Behavior:
        ;   - Zeroes MSG_LINE_COUNT ($5A57)
        ;   - Tail-calls DISPLAY_MESSAGE_LOOP
        ; Clobbers: A, X, Y

        STOB    #$00,MSG_LINE_COUNT    ; reset line counter
        JMP     DISPLAY_MESSAGE_LOOP   ; display message
        ORG     $65A9
    RANDOM_EVENTS:
        SUBROUTINE RANDOM_EVENTS
        ; ---- RANDOM_EVENTS ----
        ; Cascading random event dispatcher.
        ; Called between rounds to generate world activity.
        ;
        ; Input:  EVENT_THRESHOLD ($4239) = probability cutoff (~7%)
        ; Output: fires 0-10 events depending on PRNG rolls
        ; Clobbers: A, X, Y, CHAR_PTR, MOB_PTR, CHAR_INDEX, FACTION_MASK
        ;
        ; Each step rolls PRNG; if roll < threshold, the event fires
        ; AND the next step is tested.  Step 6 inverts the test: if
        ; roll >= threshold, a burst of 4 spawns + faction scene fires.
        JSR     STEP_PRNG               ; step 1: mob event
        LDA     PRNG_OUTPUT
        BAPL    EVENT_THRESHOLD,_RANDOM_EVENTS.ev2
        JSR     EVENT_PICK_MOB
    _RANDOM_EVENTS.ev2:
        JSR     STEP_PRNG               ; step 2: char event
        LDA     PRNG_OUTPUT
        BAPL    EVENT_THRESHOLD,_RANDOM_EVENTS.ev3
        JSR     EVENT_PICK_CHAR
    _RANDOM_EVENTS.ev3:
        JSR     STEP_PRNG               ; step 3: faction scene
        LDA     PRNG_OUTPUT
        BAPL    EVENT_THRESHOLD,_RANDOM_EVENTS.ev4
        JSR     EVENT_FACTION_SCENE
    _RANDOM_EVENTS.ev4:
        JSR     STEP_PRNG               ; step 4: spawn mob
        LDA     PRNG_OUTPUT
        BAPL    EVENT_THRESHOLD,_RANDOM_EVENTS.ev5
        JSR     EVENT_SPAWN_MOB
    _RANDOM_EVENTS.ev5:
        JSR     STEP_PRNG               ; step 5: spawn mob
        LDA     PRNG_OUTPUT
        BAPL    EVENT_THRESHOLD,_RANDOM_EVENTS.ev6
        JSR     EVENT_SPAWN_MOB
    _RANDOM_EVENTS.ev6:
        JSR     STEP_PRNG               ; step 6: burst gate (inverted)
        LDA     PRNG_OUTPUT
        BAMI    EVENT_THRESHOLD,_RANDOM_EVENTS.burst
        RTS                             ; no burst this round
    _RANDOM_EVENTS.burst:
        JSR     EVENT_SPAWN_MOB         ; burst: 4 spawns + faction scene
        JSR     EVENT_SPAWN_MOB
        JSR     EVENT_SPAWN_MOB
        JSR     EVENT_SPAWN_MOB
        JSR     PICK_RANDOM_CHAR
        JMP     EVENT_FACTION_SCENE_CONT
        ORG     $660D
    EVENT_PICK_MOB:
        SUBROUTINE EVENT_PICK_CHAR
        ; ---- EVENT_PICK_MOB ----
        ; Select a random mob group (weighted by size) and process its events.
        ;
        ; Input:  TOTAL_MOB_COUNT = total mob population
        ; Output: CHAR_PTR -> selected mob's char record
        ; Clobbers: A, X, Y, CHAR_INDEX, MOB_PTR, DATA_PTR
        JSR     PICK_RANDOM_MOB         ; weighted mob selection -> CHAR_PTR
        JMP     PROCESS_CHAR_EVENTS
        ORG     $6613
    EVENT_PICK_CHAR:
        SUBROUTINE EVENT_FACTION_SCENE
        ; ---- EVENT_PICK_CHAR ----
        ; Select a random character (uniform) and process its events.
        ;
        ; Input:  CURRENT_PLAYER = upper bound of player range
        ; Output: CHAR_PTR -> selected char record
        ; Clobbers: A, X, Y, CHAR_INDEX, DATA_PTR
        JSR     PICK_RANDOM_CHAR        ; uniform char selection -> CHAR_PTR
        JMP     PROCESS_CHAR_EVENTS
        ORG     $6619
    EVENT_FACTION_SCENE:
        SUBROUTINE EVENT_FACTION_SCENE_CONT
        ; ---- EVENT_FACTION_SCENE ----
        ; Pick a random mob, find another mob with matching faction,
        ; and trigger a scene between them.
        ;
        ; Input:  TOTAL_MOB_COUNT, char records at [[MOB_BASE_LO]]+
        ; Output: SCENE_SETUP called if faction match found,
        ;         else falls through to EVENT_SPAWN_MOB
        ; Clobbers: A, X, Y, CHAR_PTR, MOB_PTR, CHAR_INDEX,
        ;           FACTION_MASK, SOURCE_CHAR, ACTIVE_CHAR
        ;
        ; Alternate entry EVENT_FACTION_SCENE_CONT skips mob pick,
        ; reuses CHAR_PTR set by caller (burst path).
        JSR     PICK_RANDOM_MOB
    EVENT_FACTION_SCENE_CONT:               ; alternate entry from RANDOM_EVENTS burst
        LDA     CHAR_PTR                    ; check low byte of CHAR_PTR
        BANE    #$09,_EVENT_FACTION_SCENE_CONT.has_char              ; still at base ($4009)?
        RTS                                 ; no character selected -> skip
    _EVENT_FACTION_SCENE_CONT.has_char:
        LDY     #CFIELD_STYLE               ; byte 5: faction in bits 6-5
        LDA     (CHAR_PTR),Y
        AND     #CFIELD_FACTION_MASK        ; isolate faction bits
        STA     FACTION_MASK                ; save for FIND_MOB_BY_FACTION
        STOW    $4000,CHAR_PTR              ; reset CHAR_PTR to record base
        JSR     FIND_MOB_BY_FACTION         ; search for matching faction
        BANE    #$00,_EVENT_FACTION_SCENE_CONT.found
        JMP     EVENT_SPAWN_MOB             ; no match -> try spawning
    _EVENT_FACTION_SCENE_CONT.found:
        STOB    #$00,SOURCE_CHAR            ; clear source char
        MOVB    CHAR_INDEX,ACTIVE_CHAR      ; set active char = found index
        JSR     SCENE_SETUP
        JSR     CHECK_LOCATION_MATCH
        RTS
        ORG     $664E
    EVENT_SPAWN_MOB:
        SUBROUTINE EVENT_SPAWN_MOB
        ; ---- EVENT_SPAWN_MOB ----
        ; Activate a dormant character by placing it into a matching
        ; mob's group.  Picks a random char; if already active (XP
        ; field nonzero), retries once.  On success, reorders the
        ; char into the mob group and clamps its stats.
        ;
        ; Input:  CURRENT_PLAYER, char records at [[MOB_BASE_LO]]+
        ; Output: REORDER_CHAR + CLAMP_CHAR_FIELD called if spawned
        ; Clobbers: A, X, Y, CHAR_PTR, MOB_PTR, CHAR_INDEX,
        ;           FACTION_MASK, SOURCE_CHAR, ACTIVE_CHAR, TARGET_REC
        JSR     PICK_RANDOM_CHAR
        LDY     #CFIELD_XP                  ; byte 8: status/group count
        LDA     (CHAR_PTR),Y
        BEQ     .inactive                   ; zero = dormant slot
        JSR     PICK_RANDOM_CHAR            ; retry once
        LDY     #CFIELD_XP
        LDA     (CHAR_PTR),Y
        BEQ     .inactive
        RTS                                 ; both active -> give up
    .inactive:
        LDY     #CFIELD_STYLE               ; byte 5: faction in bits 6-5
        LDA     (CHAR_PTR),Y
        AND     #CFIELD_FACTION_MASK        ; isolate faction bits
        STA     FACTION_MASK
        JSR     FIND_MOB_BY_FACTION
        BANE    #$00,_EVENT_SPAWN_MOB.found
        RTS                                 ; no matching mob -> skip
    _EVENT_SPAWN_MOB.found:
        MOVB    CHAR_INDEX,SOURCE_CHAR
        STOB    #$00,ACTIVE_CHAR
        MOVW    MOB_PTR,TARGET_REC          ; pass mob ptr to REORDER_CHAR
        JSR     REORDER_CHAR
        JMP     CLAMP_CHAR_FIELD
        ORG     $6689
    PICK_RANDOM_MOB:
        SUBROUTINE PICK_RANDOM_MOB
        ; ---- PICK_RANDOM_MOB ----
        ; Weighted random mob selection.  Generates a random number in
        ; [1..TOTAL_MOB_COUNT], then walks char records subtracting
        ; each group count (CFIELD_XP byte) until the accumulator
        ; hits zero or goes negative.
        ;
        ; Input:  TOTAL_MOB_COUNT = total mob population
        ; Output: CHAR_PTR -> selected char record
        ;         CHAR_INDEX = selected player index
        ; Clobbers: A, X, Y, MOB_PTR, DATA_PTR
        ;
        ; Falls through to PICK_RANDOM_CHAR if no mobs exist.
        LDA     TOTAL_MOB_COUNT
        BEQ     PICK_RANDOM_CHAR            ; no mobs -> pick char instead
        JSR     RANDOM_IN_RANGE             ; A = random 0..count-1
        BAEQ    #$00,PICK_RANDOM_MOB        ; zero = retry (need 1+)
        STA     MOB_PTR                     ; remaining count
        STOB    #$01,CHAR_INDEX             ; start at player 1
    .next_char:
        LDA     CHAR_INDEX
        JSR     GET_CHAR_RECORD             ; DATA_PTR -> char record
        LDY     #CFIELD_XP                  ; byte 8: group count in bits 3-0
        LDA     MOB_PTR                     ; remaining
        SEC
        SBC     (DATA_PTR),Y                ; subtract this char's group count
        STA     MOB_PTR
        BEQ     .found                      ; exact match
        BMI     .found                      ; overshot -> select this one
        INC     CHAR_INDEX
        BNE     .next_char                  ; always branches (index < 256)
    .found:
    COPY_BE_TO_FA:
        MOVW    DATA_PTR,CHAR_PTR           ; copy result to CHAR_PTR
        RTS
        ORG     $66BA
    PICK_RANDOM_CHAR:
        SUBROUTINE PICK_RANDOM_CHAR
        ; ---- PICK_RANDOM_CHAR ----
        ; Uniform random character selection from players 2..CURRENT_PLAYER-1.
        ; Computes range = CURRENT_PLAYER - 3, picks random offset,
        ; adds base index 2.
        ;
        ; Input:  CURRENT_PLAYER = upper bound + 1 of player range
        ; Output: CHAR_PTR -> selected char record
        ;         CHAR_INDEX = selected player index
        ; Clobbers: A, X, Y, DATA_PTR
        LDA     CURRENT_PLAYER
        SEC
        SBC     #$03                        ; range = CURRENT_PLAYER - 3
        JSR     RANDOM_IN_RANGE             ; A = random 0..range-1
        CLC
        ADC     #$02                        ; offset to player 2
        STA     CHAR_INDEX                  ; save selected index
        JSR     GET_CHAR_RECORD             ; DATA_PTR -> char record
        JMP     COPY_BE_TO_FA               ; copy DATA_PTR -> CHAR_PTR
        ORG     $66CE
    PROCESS_CHAR_EVENTS:
        SUBROUTINE PROCESS_CHAR_EVENTS
        ; Walk the character's 3-byte event list and trigger pending events.
        ;
        ; Input:  CHAR_PTR       -- character record (bytes 6-7 = event list ptr)
        ;         CHAR_INDEX     -- current player index
        ; Output: triggers CHECK_LOCATION_MATCH for each pending event
        ; Behavior:
        ;   - Loads event list pointer from CFIELD_EVENTS
        ;   - For each 3-byte entry:
        ;     $FF in byte 0 = end of list -> RTS
        ;     byte 1 bit 7 set -> skip (already activated)
        ;     byte 1 < $40 -> skip (not pending)
        ;     byte 1 in $40-$7F -> subtract $40, write back,
        ;       copy CHAR_INDEX to ACTIVE_CHAR, JMP CHECK_LOCATION_MATCH
        ;   - Advances EVENT_PTR by 3 for each skipped entry
        ; Clobbers: A, Y
        LDWIY    CHAR_PTR,#CFIELD_EVENTS,EVENT_PTR ; char record bytes 6-7 = event list ptr
    .scan:
        LDY     #$00
        LDA     (EVENT_PTR),Y ; event byte 0
        BANE    #$FF,_PROCESS_CHAR_EVENTS.check   ; $FF = end of list
        RTS
    _PROCESS_CHAR_EVENTS.check:
        INY
        LDA     (EVENT_PTR),Y ; event byte 1
        BMI     _PROCESS_CHAR_EVENTS.advance      ; bit 7 set -> skip
        BAMI    #$40,_PROCESS_CHAR_EVENTS.advance ; < $40?
        SEC
        SBC     #$40          ; normalize to 0-based
        STA     (EVENT_PTR),Y ; write back
        MOVB    CHAR_INDEX,ACTIVE_CHAR ; save triggering player index
        JMP     CHECK_LOCATION_MATCH
    _PROCESS_CHAR_EVENTS.advance:
        CLC
        LDA     EVENT_PTR
        ADC     #$03          ; next 3-byte entry
        STA     EVENT_PTR
        BCC     .no_carry
        INC     EVENT_PTR+1
    .no_carry:
        JMP     .scan
        ORG     $6706
    FIND_MOB_BY_FACTION:
        SUBROUTINE FIND_MOB_BY_FACTION
        ; Search the mob linked list for a faction match.
        ;
        ; Input:  CHAR_PTR       -- character record (byte 2 = first mob link)
        ;         FACTION_MASK   -- expected faction value (bits 6-4 of MFIELD_FLAGS)
        ; Output: A = 1 if found (MOB_PTR -> matching mob record)
        ;         A = 0 if end of list reached (no match)
        ; Behavior:
        ;   - Follows linked list via CFIELD_NEXT / MFIELD_NEXT
        ;   - For each mob, checks MFIELD_FLAGS bit 7 (active)
        ;     and bits 6-4 (faction) against FACTION_MASK
        ;   - Stops at first match or zero link
        ; Clobbers: A, Y, MOB_DATA_PTR
        LDY     #CFIELD_NEXT
        LDA     (CHAR_PTR),Y       ; char record byte 2 = first link
    .check_link:
        BNE     .resolve
        RTS                        ; zero link -> return A=0
    .resolve:
        JSR     GET_MOB_DATA       ; resolve link -> MOB_DATA_PTR
        MOVW    MOB_DATA_PTR,MOB_PTR ; MOB_PTR = mob data pointer
        LDY     #MFIELD_FLAGS
        LDA     (MOB_PTR),Y        ; mob data byte $0F
        BPL     _FIND_MOB_BY_FACTION.next              ; bit 7 clear -> skip
        AND     #FLAGS_FACTION_MASK ; bits 6-4
        BANE    FACTION_MASK,_FIND_MOB_BY_FACTION.next ; match faction?
        LDA     #$01               ; found -> return 1
        RTS
    _FIND_MOB_BY_FACTION.next:
        LDY     #MFIELD_NEXT
        LDA     (MOB_PTR),Y        ; next link
        JMP     .check_link
        ORG     $672E
    CHECK_LOCATION_MATCH:
        SUBROUTINE CHECK_LOCATION_MATCH
        ; Check if the active character's position matches LOCATION_POS.
        ;
        ; Behavior:
        ;   Loads the character record for ACTIVE_CHAR, compares position
        ;   with LOCATION_POS. If they match, clears LOCATION_POS to force
        ;   a map redraw on the next ENTER_LOCATION.
        ;
        ; Modifies: LOCATION_POS (cleared on match)
        ; Clobbers: A, X, Y

        ; Load char record for ACTIVE_CHAR ($5A29), check if DATA_PTR matches
        ; LOCATION_POS.  If so, clear LOCATION_POS to force a redraw.
        LDA     ACTIVE_CHAR
        JSR     GET_CHAR_RECORD
        LDA     DATA_PTR
        BAEQ    LOCATION_POS,_CHECK_LOCATION_MATCH.match
        RTS
    _CHECK_LOCATION_MATCH.match:
        STOB    #$00,LOCATION_POS ; clear to trigger redraw
        RTS

    ...

        ORG     $6DFA
    SCENE_LOADER_END:
    CHECK_SCENE_EVENTS:
        SUBROUTINE CHECK_SCENE_EVENTS
        ; Scene event dispatcher for players 1 and 3.
        ;
        ; Behavior:
        ;   Scans mob groups for hostile encounters via SCAN_FOR_HOSTILE.
        ;   Dispatches to SCENE_LOOP with mode 4-8 depending on player
        ;   number and encounter state. Processes mob groups on hostile
        ;   encounters.
        ;
        ; Clobbers: A, X, Y

        ; Scene event dispatcher for players 1 & 3.  Checks encounter
        ; state and dispatches to SCENE_LOOP with mode 4-8 based on
        ; game state flags.  Called from MAIN_ENTRY's character loop.
        LDA     #$00
        BANE    SCENE_HOSTILE_DONE,_CHECK_SCENE_EVENTS.done     ; yes -> skip
        STOB    #$40,SCENE_GROUP_IDX ; start scanning from group $40
        JSR     SCAN_FOR_HOSTILE             ; scan for hostile encounter
        LDA     #$01
        BANE    SCENE_FOUND_FLAG,_CHECK_SCENE_EVENTS.done       ; no -> skip
        BANE    CURRENT_PLAYER,_CHECK_SCENE_EVENTS.other_player ; is this player 1?
        BANE    SCENE_HOSTILE_FLAG,_CHECK_SCENE_EVENTS.hostile  ; hostile encounter found?
        BAEQ    SCENE_P1_DONE,_CHECK_SCENE_EVENTS.done          ; yes -> skip
        STA     SCENE_P1_DONE                ; mark as done
        LDA     #$04
        JMP     SCENE_LOOP                   ; mode 4: player 1 non-hostile scene
    _CHECK_SCENE_EVENTS.done:
        RTS
    _CHECK_SCENE_EVENTS.other_player:
        BAEQ    SCENE_P3_DONE,_CHECK_SCENE_EVENTS.done          ; yes -> skip
        STA     SCENE_P3_DONE                ; mark as done
        JSR     SET_CURSOR_ROW21
        LDA     #$05
        JMP     SCENE_LOOP                   ; mode 5: other player scene
    _CHECK_SCENE_EVENTS.hostile:
        STA     SCENE_HOSTILE_DONE           ; mark hostile as processed
        LDA     #$06
        JSR     SCENE_LOOP                   ; mode 6: hostile encounter intro
        JSR     PROCESS_SCENE_GROUPS         ; process mob groups
        LDA     SCENE_EVENT_RESULT
        BNE     _CHECK_SCENE_EVENTS.done                        ; nonzero = event handled, stop
        LDA     #$07
        JSR     SCENE_LOOP                   ; mode 7: post-encounter
        LDA     SCENE_COMBAT_FLAG
        BEQ     _CHECK_SCENE_EVENTS.done                        ; no combat -> done
        JSR     SCROLL_STATUS_LINE
        LDA     #$08
        JMP     SCENE_LOOP                   ; mode 8: combat resolution
        ORG     $6E5A
    SCAN_FOR_HOSTILE:
        SUBROUTINE SCAN_FOR_HOSTILE
        ; Scan the mob group chain for hostile encounters.
        ;
        ; Behavior:
        ;   Walks the linked list of mob groups via byte 2 (next pointer).
        ;   Sets SCENE_FOUND_FLAG=1 when the target group is encountered.
        ;   Calls CHECK_HOSTILE on each group; sets SCENE_HOSTILE_FLAG
        ;   nonzero if any hostile is found.
        ;
        ; Outputs: SCENE_FOUND_FLAG, SCENE_HOSTILE_FLAG
        ; Clobbers: A, X, Y

        ; Scan mob groups starting from SCENE_GROUP_IDX for hostile encounters.
        ; Sets SCENE_FOUND_FLAG=1 if the group matches, SCENE_HOSTILE_FLAG
        ; nonzero if a hostile is found.
        STOB    #$00,SCENE_FOUND_FLAG
        STA     SCENE_HOSTILE_FLAG
        LDA     SCENE_GROUP_IDX
        JSR     GET_MOB_DATA                   ; resolve group -> MOB_DATA_PTR/MOB_DATA_PTR+1
        MOVW    MOB_DATA_PTR,MOB_PTR ;  MOB_PTR/MOB_PTR+1 = mob data pointer
        MOVW    CHAR_PTR,MOB_DATA_PTR ;  save $FA/$FB into MOB_DATA_PTR/MOB_DATA_PTR+1
    .walk:
        LDY     #$02
        LDA     (MOB_DATA_PTR),Y               ; mob data byte 2 = next link
        BNE     .has_link
        RTS                                    ; end of chain
    .has_link:
        BANE    SCENE_GROUP_IDX,_SCAN_FOR_HOSTILE.check_hostile ; matches the target group?
        LDX     #$01
        STX     SCENE_FOUND_FLAG               ; mark as found
    _SCAN_FOR_HOSTILE.check_hostile:
        JSR     GET_MOB_DATA                   ; resolve link -> MOB_DATA_PTR/MOB_DATA_PTR+1
        MOVW    MOB_DATA_PTR,TARGET_REC ;  TARGET_REC/TARGET_REC+1 = resolved mob data
        JSR     CHECK_HOSTILE                  ; check if hostile
        BEQ     .walk                          ; not hostile -> continue
        STA     SCENE_HOSTILE_FLAG             ; hostile found
        BNE     .walk                          ; always branches, continue scan
        ORG     $6E9E
    PROCESS_SCENE_GROUPS:
        SUBROUTINE PROCESS_SCENE_GROUPS
        ; Process mob groups $3F, $41, $42 for hostile reordering.
        ;
        ; Behavior:
        ;   Saves the player 3 record pointer, then calls SCAN_AND_REORDER
        ;   for groups $3F, $41, and $42 (skipping $40). Restores the
        ;   current player's record pointer on exit.
        ;
        ; Clobbers: A, X, Y

        ; Process mob groups $3F, $41, $42 with SCAN_AND_REORDER,
        ; bookended by LOAD_CHAR_TO_FA for save/restore.
        STOB    #$03,SOURCE_CHAR
        JSR     LOAD_CHAR_TO_FA  ; save player 3's record to $FA/$FB
        STOB    #$00,ACTIVE_CHAR
        STOB    #$3F,SCENE_GROUP_IDX
        JSR     SCAN_AND_REORDER ; group $3F
        INC     SCENE_GROUP_IDX  ; $40 (skip -- that's the player's group)
        INC     SCENE_GROUP_IDX  ; $41
        JSR     SCAN_AND_REORDER ; group $41
        INC     SCENE_GROUP_IDX  ; $42
        JSR     SCAN_AND_REORDER ; group $42
        LDA     CURRENT_PLAYER
        JSR     LOAD_CHAR_TO_FA  ; restore current player's record
        RTS

    ...

        ORG     $6ED5
    SCAN_AND_REORDER:
        SUBROUTINE SCAN_AND_REORDER
        ; Scan for hostile in the current group and reorder if found.
        ;
        ; Inputs:
        ;   SCENE_GROUP_IDX — group index to scan
        ;
        ; Behavior:
        ;   Calls SCAN_FOR_HOSTILE; if SCENE_FOUND_FLAG is set, copies
        ;   MOB_PTR into TARGET_REC and calls REORDER_CHAR.
        ;
        ; Clobbers: A, X, Y

        ; Call SCAN_FOR_HOSTILE for current SCENE_GROUP_IDX.
        ; If a match is found, copy MOB_PTR/MOB_PTR+1 -> TARGET_REC/TARGET_REC+1 and reorder.
        JSR     SCAN_FOR_HOSTILE
        LDA     SCENE_FOUND_FLAG
        BNE     .found
        RTS                  ; nothing found
    .found:
        MOVW    MOB_PTR,TARGET_REC ;  TARGET_REC/TARGET_REC+1 = mob data pointer
        JMP     REORDER_CHAR
        ORG     $6EE9
    FLUSH_PENDING_XP:
        SUBROUTINE FLUSH_PENDING_XP
        ; Flush pending XP from entity byte 8 into the level in byte 11.
        ;
        ; Inputs:
        ;   ENTITY_PTR — pointer to the entity record
        ;
        ; Behavior:
        ;   Extracts bits 6-4 of byte 8 as pending XP (0-7), clears the
        ;   pending bits, and adds the XP to byte 11 (level). Displays
        ;   the XP gained message, or exits to MOVE_LOOP if none pending.
        ;
        ; Modifies: entity bytes 8 and 11
        ; Clobbers: A, X, Y

        LDY     #$08
        LDA     (ENTITY_PTR),Y     ; byte 8: status|pending_xp|group_count
        TAX                        ; save original
        AND     #$80               ; keep only status flag (bit 7)
        STA     (ENTITY_PTR),Y     ; clear pending XP and group count
        TXA
        AND     #$7F               ; strip status flag
        BEQ     .no_xp             ; bits 6-0 all zero -> nothing to flush
        ROR                        ; \ extract bits 6-4 (pending XP)
        ROR                        ;  | shift right 4 positions
        ROR                        ;  |
        ROR                        ; /
        AND     #$07               ; mask to 3 bits (0-7)
        CLC
        LDY     #$0B
        ADC     (ENTITY_PTR),Y     ; add pending XP to level
        STA     (ENTITY_PTR),Y     ; store updated level
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_XP_GAINED               ; XP gained message
        JMP     DISPLAY_MESSAGE

    .no_xp:
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_NO_XP               ; no XP message
        JSR     DISPLAY_MESSAGE
        JMP     MOVE_LOOP
        ORG     $6F17
    INIT_DROP_GOLD:
        SUBROUTINE INIT_DROP_GOLD
        ; Initialize the drop gold binary search interface.
        ;
        ; Inputs:
        ;   ENTITY_PTR — pointer to the entity record
        ;
        ; Behavior:
        ;   Calls FIND_MSB on the entity's gold to set DROP_AMOUNT to the
        ;   highest power-of-2. Copies to DROP_STEP and halves it, then
        ;   enters the drop gold menu. If no gold, displays a message.
        ;
        ; Modifies: DROP_AMOUNT, DROP_STEP
        ; Clobbers: A, X, Y

        LDA     #$00
        STA     DROP_AMOUNT
        LDY     #$0E
        LDA     (ENTITY_PTR),Y     ; gold high byte
        JSR     FIND_MSB           ; X = highest set bit mask
        STX     DROP_AMOUNT+1
        CPX     #$00
        BNE     .has_gold
        DEY                        ; Y = $0D
        LDA     (ENTITY_PTR),Y     ; gold low byte
        JSR     FIND_MSB
        STX     DROP_AMOUNT
        BNE     .has_gold
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_NO_GOLD               ; "no gold" message
        JSR     DISPLAY_MESSAGE
        JMP     MENU_OTHER_OPTIONS

    .has_gold:
        LDA     DROP_AMOUNT     ; \ copy amount to step
        STA     DROP_STEP       ;  |
        LDA     DROP_AMOUNT+1     ;  |
        STA     DROP_STEP+1       ; /
        JSR     HALVE_STEP         ; step = amount / 2
        JMP     MENU_DROP_GOLD     ; enter drop gold menu
        ORG     $6F52
    INIT_DROP_DISPLAY:
        SUBROUTINE INIT_DROP_DISPLAY
        ; Format the drop gold display buffer with the current amount.
        ;
        ; Behavior:
        ;   Clears DROP_DISPLAY_BUF with spaces, copies DROP_AMOUNT into
        ;   NUM_VALUE, sets DECIMAL_OUT_PTR to DROP_DISPLAY_BUF, and calls
        ;   NUM_TO_DECIMAL.
        ;
        ; Modifies: DROP_DISPLAY_BUF, NUM_VALUE, DECIMAL_OUT_PTR
        ; Clobbers: A, X

        LDX     #$04
        LDA     #$20               ; space
    .clear:
        STA     DROP_DISPLAY_BUF,X
        DEX
        BPL     .clear
        LDA     DROP_AMOUNT
        STA     $BC
        LDA     DROP_AMOUNT+1
        STA     $BD
        LDA     #<DROP_DISPLAY_BUF
        STA     $BA
        LDA     #>DROP_DISPLAY_BUF
        STA     $BB
        JSR     NUM_TO_DECIMAL
        RTS
        ORG     $6F72
    ADJ_DROP_INC:
        LDX     #$03               ; # pressed
        BNE     .entry

    ADJ_DROP_DEC:
        LDX     #$04               ; @ pressed

    .entry:
        LDA     #$00
        CMP     DROP_STEP
        BNE     .nonzero
        CMP     DROP_STEP+1
        BEQ     INIT_DROP_GOLD     ; step = 0 -> reinitialize

    .nonzero:
        TXA
        CMP     #$03
        BEQ     .dec_amount        ; X=3 (#) -> subtract step

        ; X=4 (@): add step, halve, check bounds
        JSR     ADD_DROP
        JSR     HALVE_STEP
        LDY     #$0E
        LDA     (ENTITY_PTR),Y     ; actual gold hi
        CMP     DROP_AMOUNT+1
        BCC     .over_limit        ; drop > gold -> too much
        BNE     .done              ; drop < gold -> ok
        DEY
        LDA     (ENTITY_PTR),Y     ; actual gold lo
        CMP     DROP_AMOUNT
        BCS     .done              ; drop <= gold -> ok

    .over_limit:
        JSR     SUB_DROP           ; undo: amount -= step
        JSR     SUB_DROP           ; undo again (net: amount -= step)
        LDX     #$04               ; switch to @ mode
        BNE     .entry             ; retry with reduced step

    .dec_amount:
        JSR     SUB_DROP           ; amount -= step
        JSR     HALVE_STEP         ; step /= 2

    .done:
        JMP     MENU_DROP_GOLD     ; re-enter drop gold menu
        ORG     $6FB5
    DROP_GOLD_EXEC:
        SUBROUTINE DROP_GOLD_EXEC
        ; Execute the gold drop transaction.
        ;
        ; Inputs:
        ;   ENTITY_PTR — pointer to the entity record
        ;   DROP_AMOUNT — amount of gold to drop
        ;
        ; Behavior:
        ;   Subtracts DROP_AMOUNT from entity's gold, then subtracts a
        ;   random loss (1-25). If gold remains, creates a gold event on
        ;   the map. Exits via JMP MOVE_LOOP.
        ;
        ; Modifies: entity gold, event list
        ; Clobbers: A, X, Y

        LDA     DROP_AMOUNT
        STA     $BC
        LDA     DROP_AMOUNT+1
        STA     $BD
        LDY     #$00
        STY     DROP_STEP+1
        JSR     MODIFY_CURRENT_STATS ; subtract gold from entity
        LDA     #$19
        JSR     RANDOM_IN_RANGE    ; random 0-24
        STA     DROP_STEP
        INC     DROP_STEP       ; step = 1-25
        JSR     SET_CURSOR_ROW21
        JSR     SUB_DROP           ; amount -= random step (gold lost)
        LDA     DROP_AMOUNT+1
        BMI     .lost              ; negative -> all lost
        BNE     .has_gold
        LDA     DROP_AMOUNT
        BEQ     .lost              ; zero -> all lost

    .has_gold:
        LDA     #MSG_GOLD_DROPPED               ; gold dropped message
        JSR     DISPLAY_MESSAGE
        LDA     DROP_AMOUNT
        STA     $BC
        LDA     DROP_AMOUNT+1
        STA     $BD
        LDY     #$03
        LDA     (ENTITY_PTR),Y     ; entity position
        JSR     UPDATE_EVENT_LIST  ; create gold event on map
        JMP     MOVE_LOOP

    .lost:
        LDA     #MSG_GOLD_LOST               ; gold lost message
        JSR     DISPLAY_MESSAGE
        JMP     MOVE_LOOP
        ORG     $7005
    FIND_MSB:
        SUBROUTINE FIND_MSB
        ; Find the most significant set bit in A.
        ;
        ; Inputs:
        ;   A — value to scan
        ;
        ; Behavior:
        ;   Starting from bit 7, rotates A left and checks each bit.
        ;   Returns the mask of the first set bit found.
        ;
        ; Outputs:
        ;   X — single-bit mask of the MSB (e.g., $40 for bit 6)
        ; Clobbers: A, X

        SEC
        LDX     #$80               ; start with bit 7 mask
    .test:
        ROL                        ; shift A left through carry
        BCC     .next              ; bit was 0 -> try next
        RTS                        ; bit was 1 -> X = mask
    .next:
        PHA
        TXA
        LSR                        ; shift mask right
        TAX
        PLA
        CLC
        JMP     .test

    ADD_DROP:
        CLC
        LDA     DROP_AMOUNT
        ADC     DROP_STEP
        STA     DROP_AMOUNT
        LDA     DROP_AMOUNT+1
        ADC     DROP_STEP+1
        STA     DROP_AMOUNT+1
        RTS

    SUB_DROP:
        SEC
        LDA     DROP_AMOUNT
        SBC     DROP_STEP
        STA     DROP_AMOUNT
        LDA     DROP_AMOUNT+1
        SBC     DROP_STEP+1
        STA     DROP_AMOUNT+1
        RTS

    HALVE_STEP:
        CLC
        LDA     DROP_STEP+1
        ROR
        STA     DROP_STEP+1
        LDA     DROP_STEP
        ROR
        STA     DROP_STEP
        RTS
*/
#include "ali_baba.h"

/* The mask of a gold amount's bits 8-10, which an event entry keeps in
 * the low bits of its type. EVENT_VALUE_HI_MASK in the original. */
constexpr uint8_t EVENT_VALUE_HI_MASK = 0x07;

/* A location's faction, bits 6-5 of its style. */
constexpr uint8_t STYLE_FACTION_MASK = 0x60;

/* ENTITY_PTR's byte at `offset`, with which the gold is read a byte at
 * a time. c6502 reads a 2-byte member through a zero-page pointer by
 * computing the member's address first (notes/events.md), and this
 * reads the byte with LDY #offset and LDA (ENTITY_PTR),Y, as the
 * original does. */
#define ENTITY_BYTE(offset) (((uint8_t *)ENTITY_PTR)[offset])

/* CHAR_PTR's byte at `offset`, with which the event list's address,
 * bytes 6 and 7, is read a byte at a time for the same reason. */
#define CHAR_BYTE(offset) (((uint8_t *)CHAR_PTR)[offset])

/* The low byte of LOCATIONS[1]'s address, $4009. Location 1 is the only
 * record whose address has this low byte. */
constexpr uint8_t LOCATION_1_LO = 0x09;

/* A mob that a faction search finds has bit 7 of its flags set. */
constexpr uint8_t FLAGS_CLASS_BIT = 0x80;

/* The scenes this unit plays. */
enum {
    SCENE_SPECIAL_CHEST = 0x0B,
    SCENE_SPECIAL_BONUS = 0x0C,
    SCENE_DAMAGE = 0x0F,
    SCENE_TRAP_0 = 0x36,        /* one scene for each trap, $36-$39 */
    SCENE_P1_HOSTILE = 0x04,
    SCENE_P3 = 0x05,
    SCENE_P1_FIRST = 0x06,
    SCENE_P1_SECOND = 0x07,
    SCENE_P1_COMBAT = 0x08,
};

/* The mob index CHECK_SCENE_EVENTS looks for in player 1's and 3's
 * lists, and the three that PROCESS_SCENE_GROUPS moves out of
 * location 3. */
constexpr uint8_t SCENE_GROUP = 0x40;
constexpr uint8_t SCENE_GROUP_FIRST = 0x3F;

/* Message $2A, which UPDATE_EVENT_LIST prints when the list is full. Its
 * EQU name is MSG_ARMOR_WORTHLESS, which describes another message. */
constexpr uint8_t MSG_EVENT_LIST_FULL = 0x2A;

/* ---- Leaving a fight ---------------------------------------------------- */

/* One time in two, ENTITY_PTR gets away. The original compares the
 * PRNG's output, 0-127, with $40 as a signed byte, which is the same as
 * an unsigned comparison for those values. */
bool check_can_leave(void) {
    if (step_prng() >= 0x40) {
        set_cursor_row21();
        print_mob_name();
        scroll_status_line();
        display_message(MSG_ESCAPE_FAIL);
        return true;
    }
    set_cursor_row21();
    print_mob_name();
    scroll_status_line();
    display_message(MSG_TACKLE_FAIL);
    return false;
}

/* ---- Gold on the map ----------------------------------------------------- */

/*
    Adds gold to CHAR_PTR's location at `pos`. An entry of gold already
    there, of type $C0-$D0, takes the amount. Otherwise the first empty
    slot becomes a gold entry. Only bits 0-10 of the amount are kept.

    The addition adds the low bytes, and then bits 8-10 and the carry
    to the type, with nothing to stop the type passing $D0. A type of
    $D1 or more then makes the entry a special chest, a scene, or a
    shop.
*/
void update_event_list(uint8_t pos, uint16_t value) {
    if (value == 0)
        return;
    uint8_t value_hi = (uint8_t)(value >> 8) & EVENT_VALUE_HI_MASK;
    uint8_t value_lo = (uint8_t)value;
    for (struct event *e = (struct event *)((uint16_t)CHAR_BYTE(7) << 8 | CHAR_BYTE(6));
         ; e++) {
        if (e->pos == EVENT_END) {
            set_cursor_row21();
            display_message(MSG_EVENT_LIST_FULL);
            return;
        }
        if (e->pos == EVENT_EMPTY) {
            e->pos = pos;
            e->type = value_hi | EVENT_TYPE_GOLD;
            e->value = value_lo;
            return;
        }
        if (e->pos == pos && e->type >= EVENT_TYPE_GOLD && e->type <= EVENT_ACCUM_MAX) {
            uint8_t sum = e->value + value_lo;
            e->value = sum;
            e->type += value_hi + (sum < value_lo);
            return;
        }
    }
}

/* ---- Scenes and chests on the map ---------------------------------------- */

/* Plays the scene of an entry of type $D2-$D4. A $D2 entry stays, a
 * $D3 entry is emptied, and a $D4 entry is emptied and also plays the
 * damage scene and costs ENTITY_PTR 1-4 life. */
void trigger_scene_event(struct event *ev) {
    set_char_state(0);
    SAVED_EVENT_PTR = ev;
    commit_move();
    scene_loop(ev->value);
    uint8_t type = ev->type;
    if (type < EVENT_TYPE_SCENE_ONCE)
        return;
    ev->pos = EVENT_EMPTY;
    if (type == EVENT_TYPE_SCENE_ONCE)
        return;
    scene_loop(SCENE_DAMAGE);
    apply_damage_to_current(random_in_range(3) + 1);
}

/* Adds 1 to ENTITY_PTR's byte at `offset`, whose bits 4-0 are a count,
 * unless the count is 31 already. INC_STRENGTH and INC_LEVEL's shared
 * body, which takes the offset in Y. */
static void inc_stat(uint8_t offset) {
    if ((ENTITY_BYTE(offset) & STRENGTH_MASK) != STRENGTH_MASK)
        ENTITY_BYTE(offset)++;
}

/* Fills the two five-digit fields of the chest message with $20, so
 * that the digits num_to_decimal leaves out show as spaces. */
static void clear_treasure_digits(void) {
    for (int8_t i = 4; i >= 0; i--) {
        S_CHEST_MESSAGE.reveal_digits[i] = 0x20;
        S_CHEST_MESSAGE.now_have_digits[i] = 0x20;
    }
}

/* Prints the group of strings at `strings` from the top of the status
 * lines. */
static void clear_and_display(const uint8_t *strings) {
    MSG_LINE_COUNT = 0;
    display_message_loop(strings);
}

/*
    Opens a chest, an entry of type $C0-$D1.

    A plain chest ($C0-$C8) gives ENTITY_PTR its gold. A trapped chest
    ($C9-$D0) springs one of four traps first, chosen at random, with
    its scene for a player's character: 1-6 damage, 2-10 damage, a level
    lost, or all the mob's gold lost. When the trap has killed the mob,
    which takes it out of its location's list, the chest stays, nine
    types lower, which makes it a plain chest of the same gold. The special chest
    ($D1) raises strength and level, and one time in four plays a
    bonus scene and is emptied.

    As in the original, MOB_PTR is pointed at the entry and left there.
    The gold is reported only for a player's character.
*/
void pickup_treasure(struct event *ev) {
    set_char_state(0);
    MOB_PTR = (struct mob *)ev;
    commit_move();
    uint8_t type = ev->type;
    uint8_t gold_hi;
    if (type == EVENT_TYPE_SPECIAL_CHEST) {
        scene_loop(SCENE_SPECIAL_CHEST);
        inc_stat(MFIELD_STRENGTH);
        inc_stat(MFIELD_LEVEL);
        if (random_in_range(3) != 0)
            return;
        scene_loop(SCENE_SPECIAL_BONUS);
        ev->pos = EVENT_EMPTY;
        return;
    }
    if (type < EVENT_TYPE_TRAPPED_CHEST) {
        gold_hi = type - EVENT_TYPE_GOLD;
    } else {
        uint8_t trap = random_in_range(6) & 3;
        if (ENTITY_PTR->appearance < NPC_APPEARANCE_MIN) {
            set_cursor_row21();
            scene_loop(SCENE_TRAP_0 + trap);
        }
        switch (trap) {
        case 0:
            apply_damage_to_current(random_in_range(5) + 1);
            break;
        case 1:
            apply_damage_to_current(random_in_range(8) + 2);
            break;
        case 2:
            if ((ENTITY_PTR->level & LEVEL_MASK) != 0)
                ENTITY_PTR->level--;
            break;
        default:
            ENTITY_BYTE(MFIELD_GOLD) = 0;
            ENTITY_BYTE(MFIELD_GOLD + 1) = 0;
            break;
        }
        if (ENTITY_PTR->next != CURRENT_ROOM) {
            ev->type -= EVENT_TYPE_TRAPPED_CHEST - EVENT_TYPE_GOLD;
            return;
        }
        gold_hi = ev->type - EVENT_TYPE_TRAPPED_CHEST;
    }
    uint16_t gold = (uint16_t)gold_hi << 8 | ev->value;
    modify_current_stats(gold, true);
    if (ENTITY_PTR->appearance < NPC_APPEARANCE_MIN) {
        clear_treasure_digits();
        num_to_decimal(gold, S_CHEST_MESSAGE.reveal_digits);
        num_to_decimal((uint16_t)ENTITY_BYTE(MFIELD_GOLD + 1) << 8 | ENTITY_BYTE(MFIELD_GOLD),
                       S_CHEST_MESSAGE.now_have_digits);
        set_cursor_row21();
        clear_and_display(S_CHEST_MESSAGE.opens);
    }
    ev->pos = EVENT_EMPTY;
}

/* ---- Random events -------------------------------------------------------- */

/* Sets LOCATION_POS to 0, so that the next enter_location redraws, when
 * location ACTIVE_CHAR is the one drawn last. LOCATION_POS holds the
 * low byte of that location's record address, and the records are at
 * the original's addresses, so the low bytes compare as they did. */
static void check_location_match(void) {
    if ((uint8_t)(uint16_t)get_char_record(ACTIVE_CHAR) == LOCATION_POS)
        LOCATION_POS = 0;
}

/* Points CHAR_PTR at a random location from 2 to CURRENT_PLAYER - 1,
 * and returns its index. */
static uint8_t pick_random_char(void) {
    uint8_t index = random_in_range(CURRENT_PLAYER - 3) + 2;
    CHAR_PTR = get_char_record(index);
    return index;
}

/*
    Points CHAR_PTR at a location chosen with a weight of its count of
    player characters, and returns its index. It draws a number from 1
    to TOTAL_MOB_COUNT, then takes each location's count from it in
    turn until it reaches 0 or goes negative. With no characters in
    play it picks as pick_random_char does.

    The number is kept in MOB_PTR's low byte, as in the original, which
    leaves it there.
*/
static uint8_t pick_random_mob(void) {
    uint8_t r;
    do {
        if (TOTAL_MOB_COUNT == 0)
            return pick_random_char();
        r = random_in_range(TOTAL_MOB_COUNT);
    } while (r == 0);
    *(uint8_t *)&MOB_PTR = r;
    uint8_t index = 1;
    struct location *loc;
    for (;;) {
        loc = get_char_record(index);
        *(uint8_t *)&MOB_PTR -= loc->pc_count;
        if (*(uint8_t *)&MOB_PTR == 0 || (*(uint8_t *)&MOB_PTR & 0x80))
            break;
        index++;
    }
    CHAR_PTR = loc;
    return index;
}

/* Wakes the first sleeping encounter in CHAR_PTR's event list, an entry
 * of type $40-$7F, which becomes $00-$3F. Location `index` becomes
 * ACTIVE_CHAR. The walk tests empty slots' types like any other's. */
static void process_char_events(uint8_t index) {
    for (struct event *e = (struct event *)((uint16_t)CHAR_BYTE(7) << 8 | CHAR_BYTE(6));
         e->pos != EVENT_END; e++) {
        uint8_t type = e->type;
        if (type < EVENT_TYPE_MOB || (type & 0x80))
            continue;
        e->type = type - EVENT_TYPE_MOB;
        ACTIVE_CHAR = index;
        check_location_match();
        return;
    }
}

/* Points MOB_PTR at the first mob of CHAR_PTR's list that has bit 7 of
 * its flags set and whose faction is `faction`, and returns true. When
 * no mob matches, returns false, with MOB_PTR on the last mob looked
 * at. */
static bool find_mob_by_faction(uint8_t faction) {
    for (uint8_t link = CHAR_PTR->mob_head; link != 0; link = MOB_PTR->next) {
        MOB_PTR = get_mob_data(link);
        uint8_t flags = MOB_PTR->flags;
        if ((flags & FLAGS_CLASS_BIT) && (flags & FLAGS_FACTION_MASK) == faction)
            return true;
    }
    return false;
}

/*
    Takes a mob out of play from a random location that holds no player
    characters, trying two locations. The mob is the first in the
    location's list of the location's own faction, and it moves to
    location 0, the list of the mobs at no location. clamp_char_field
    then raises its life to its strength. The original names this
    EVENT_SPAWN_MOB.
*/
static void event_spawn_mob(void) {
    uint8_t index = pick_random_char();
    if (CHAR_PTR->pc_count != 0) {
        index = pick_random_char();
        if (CHAR_PTR->pc_count != 0)
            return;
    }
    if (!find_mob_by_faction(CHAR_PTR->style & STYLE_FACTION_MASK))
        return;
    SOURCE_CHAR = index;
    ACTIVE_CHAR = 0;
    reorder_char(MOB_PTR);
    clamp_char_field();
}

/*
    Moves the first mob of CHAR_PTR's faction that is at no location
    into location `index` at a random position. CHAR_PTR is left at the
    header record, LOCATIONS[0], whose mob list holds the mobs at no
    location.
    When no mob matches, event_spawn_mob runs instead. Nothing happens
    when the location picked is location 1.
*/
static void event_faction_scene_cont(uint8_t index) {
    if ((uint8_t)(uint16_t)CHAR_PTR == LOCATION_1_LO)
        return;
    uint8_t faction = CHAR_PTR->style & STYLE_FACTION_MASK;
    CHAR_PTR = LOCATIONS;
    if (!find_mob_by_faction(faction)) {
        event_spawn_mob();
        return;
    }
    SOURCE_CHAR = 0;
    ACTIVE_CHAR = index;
    scene_setup();
    check_location_match();
}

/*
    Runs each of the first five events in turn with a chance of
    EVENT_THRESHOLD in 128, and then the burst with the same chance.
    The original's comment calls the burst's test inverted, but its BMI
    takes the burst on the same test as the others. PRNG_OUTPUT is
    0-127 and EVENT_THRESHOLD 0-$63, so the original's signed compare
    is the unsigned one here.
*/
void random_events(void) {
    if (step_prng() < EVENT_THRESHOLD)
        process_char_events(pick_random_mob());
    if (step_prng() < EVENT_THRESHOLD)
        process_char_events(pick_random_char());
    if (step_prng() < EVENT_THRESHOLD)
        event_faction_scene_cont(pick_random_mob());
    if (step_prng() < EVENT_THRESHOLD)
        event_spawn_mob();
    if (step_prng() < EVENT_THRESHOLD)
        event_spawn_mob();
    if (step_prng() >= EVENT_THRESHOLD)
        return;
    event_spawn_mob();
    event_spawn_mob();
    event_spawn_mob();
    event_spawn_mob();
    event_faction_scene_cont(pick_random_char());
}

/* ---- The story's scenes ------------------------------------------------- */

/* Sets SCENE_FOUND_FLAG when mob SCENE_GROUP_IDX is in CHAR_PTR's list,
 * and SCENE_HOSTILE_FLAG when any mob of the list is hostile to it.
 * MOB_PTR is left on mob SCENE_GROUP_IDX. */
static void scan_for_hostile(void) {
    SCENE_FOUND_FLAG = 0;
    SCENE_HOSTILE_FLAG = 0;
    MOB_PTR = get_mob_data(SCENE_GROUP_IDX);
    for (uint8_t link = CHAR_PTR->mob_head; link != 0;) {
        if (link == SCENE_GROUP_IDX)
            SCENE_FOUND_FLAG = 1;
        struct mob *m = get_mob_data(link);
        if (check_hostile(m))
            SCENE_HOSTILE_FLAG = 1;
        link = m->next;
    }
}

/* Moves mob SCENE_GROUP_IDX from location SOURCE_CHAR to ACTIVE_CHAR
 * when it is in CHAR_PTR's list. */
static void scan_and_reorder(void) {
    scan_for_hostile();
    if (SCENE_FOUND_FLAG)
        reorder_char(MOB_PTR);
}

/* Moves mobs $3F, $41, and $42 out of location 3 to no location, when
 * they are there, and points CHAR_PTR back at CURRENT_PLAYER's
 * location. */
static void process_scene_groups(void) {
    SOURCE_CHAR = 3;
    load_char_to_fa(3);
    ACTIVE_CHAR = 0;
    SCENE_GROUP_IDX = SCENE_GROUP_FIRST;
    scan_and_reorder();
    SCENE_GROUP_IDX++;          /* mob $40 stays */
    SCENE_GROUP_IDX++;
    scan_and_reorder();
    SCENE_GROUP_IDX++;
    scan_and_reorder();
    load_char_to_fa(CURRENT_PLAYER);
}

/*
    Plays the story's scenes once mob $40 has joined location 1 or 3,
    which is CHAR_PTR's (main calls this for those two only).

    In location 3 it plays scene 5, once. In location 1 it plays scene
    4 once when a mob there is hostile to mob $40. When none is, it
    plays scene 6, takes mobs $3F, $41, and $42 out of location 3, and
    unless SCENE_EVENT_RESULT is set, plays scene 7 and then, when
    SCENE_COMBAT_FLAG is set, scene 8. That happens once. The
    original's comment calls this second path the hostile one, and its
    code takes it when SCENE_HOSTILE_FLAG is 0. The original compares
    SCENE_FOUND_FLAG and SCENE_HOSTILE_FLAG with 1, and scan_for_hostile
    stores only 0 and 1 in them, so the C tests them for 0.
*/
void check_scene_events(void) {
    if (SCENE_HOSTILE_DONE != 0)
        return;
    SCENE_GROUP_IDX = SCENE_GROUP;
    scan_for_hostile();
    if (!SCENE_FOUND_FLAG)
        return;
    if (CURRENT_PLAYER != 1) {
        if (SCENE_P3_DONE == 1)
            return;
        SCENE_P3_DONE = 1;
        set_cursor_row21();
        scene_loop(SCENE_P3);
        return;
    }
    if (SCENE_HOSTILE_FLAG) {
        if (SCENE_P1_DONE == 1)
            return;
        SCENE_P1_DONE = 1;
        scene_loop(SCENE_P1_HOSTILE);
        return;
    }
    SCENE_HOSTILE_DONE = 1;
    scene_loop(SCENE_P1_FIRST);
    process_scene_groups();
    if (SCENE_EVENT_RESULT != 0)
        return;
    scene_loop(SCENE_P1_SECOND);
    if (SCENE_COMBAT_FLAG == 0)
        return;
    scroll_status_line();
    scene_loop(SCENE_P1_COMBAT);
}

/* ---- The OTHER OPTIONS menu's exits ------------------------------------------ */

/* Adds ENTITY_PTR's pending levels, bits 6-4 of its armor byte, to its
 * level, and clears them and bits 3-0. When bits 6-0 are all 0, it
 * prints that there is nothing to add and goes back to the move. */
uint8_t flush_pending_xp(void) {
    uint8_t armor = ENTITY_PTR->armor;
    ENTITY_PTR->armor = armor & 0x80;
    if ((armor & 0x7F) == 0) {
        set_cursor_row21();
        display_message(MSG_NO_XP);
        return CHAIN_MOVE_LOOP;
    }
    ENTITY_PTR->level += (armor >> 4) & 7;
    set_cursor_row21();
    display_message(MSG_XP_GAINED);
    return CHAIN_RETURN;
}

/* Returns the mask of the highest bit set in `v`, or 0 for 0. */
static uint8_t find_msb(uint8_t v) {
    uint8_t mask = 0x80;
    while (mask != 0 && (v & mask) == 0)
        mask >>= 1;
    return mask;
}

/*
    Starts the drop-gold menu. The amount to drop starts at the highest
    power of 2 in ENTITY_PTR's gold, and the step at half of that. The
    menu's two keys then halve the step at each press while they move
    the amount down or up, a binary search for the amount the player
    has in mind.
*/
uint8_t init_drop_gold(void) {
    uint8_t mask = find_msb(ENTITY_BYTE(MFIELD_GOLD + 1));
    DROP_AMOUNT = (uint16_t)mask << 8;
    if (mask == 0) {
        mask = find_msb(ENTITY_BYTE(MFIELD_GOLD));
        DROP_AMOUNT = mask;
        if (mask == 0) {
            set_cursor_row21();
            display_message(MSG_NO_GOLD);
            return CHAIN_MENU_OTHER_OPTIONS;
        }
    }
    DROP_STEP = DROP_AMOUNT;
    DROP_STEP >>= 1;
    return CHAIN_MENU_DROP_GOLD;
}

void init_drop_display(void) {
    for (int8_t i = 4; i >= 0; i--)
        S_DUCATS_TO_DROP.number[i] = 0x20;
    num_to_decimal(DROP_AMOUNT, S_DUCATS_TO_DROP.number);
}

/*
    Moves the amount by the step and halves the step: down for
    adj_drop_inc and up for adj_drop_dec, whatever their names say. An
    amount moved up past ENTITY_PTR's gold goes back down by the halved
    step twice, and the move up is tried again with the step it has
    then. A step of 0 starts the menu over.
*/
static uint8_t adj_drop(bool down) {
    for (;;) {
        if (DROP_STEP == 0)
            return init_drop_gold();
        if (down) {
            DROP_AMOUNT -= DROP_STEP;
            DROP_STEP >>= 1;
            return CHAIN_MENU_DROP_GOLD;
        }
        DROP_AMOUNT += DROP_STEP;
        DROP_STEP >>= 1;
        if (((uint16_t)ENTITY_BYTE(MFIELD_GOLD + 1) << 8 | ENTITY_BYTE(MFIELD_GOLD)) >= DROP_AMOUNT)
            return CHAIN_MENU_DROP_GOLD;
        DROP_AMOUNT -= DROP_STEP;
        DROP_AMOUNT -= DROP_STEP;
    }
}

uint8_t adj_drop_inc(void) {
    return adj_drop(true);
}

uint8_t adj_drop_dec(void) {
    return adj_drop(false);
}

/*
    Drops DROP_AMOUNT of ENTITY_PTR's gold where it stands. 1-26 of the
    ducats are lost, and what is left goes into the location's event
    list, unless nothing is left. The original compares the amount
    left as a signed number.
*/
uint8_t drop_gold_exec(void) {
    modify_current_stats(DROP_AMOUNT, false);
    DROP_STEP = (uint8_t)(random_in_range(25) + 1);
    set_cursor_row21();
    DROP_AMOUNT -= DROP_STEP;
    if ((int16_t)DROP_AMOUNT < 0 || DROP_AMOUNT == 0) {
        display_message(MSG_GOLD_LOST);
    } else {
        display_message(MSG_GOLD_DROPPED);
        update_event_list(ENTITY_PTR->pos, DROP_AMOUNT);
    }
    return CHAIN_MOVE_LOOP;
}
