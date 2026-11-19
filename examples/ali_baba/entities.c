/*
    entities.c -- what is at a map cell, the glyph each thing is drawn
    with, the lists of mobs at each location, and the drawing of a
    sprite and of the blinking cell.

    A location's mobs form a list through the `next` byte of each mob
    record, which holds the 1-based index of the next mob, starting
    from the location's `mob_head`. Both bytes are at offset 2 of their
    records, so REORDER_CHAR walks a list from the location record as
    if it were the list's first node. The lists are kept in descending
    order of level, except record 0's, where a mob goes in at a random
    place. A location also has a list of 3-byte events, and
    FIND_ENTITY_AT_POS looks for a cell's occupant among the mobs
    first and then among the events.

    A sprite is a 2 x 2 block of HRCG characters. Each of the custom
    fonts at FONT_DATA holds 24 sprites of four glyphs each, so sprite
    n of a group is glyphs 4n + $20 to 4n + $23 of font n / 24 past
    the group's first font. RENDER_FONT_CHAR writes the font selector
    and the four glyphs into s_PRINT_FONT_CHAR and prints it.

    Departures from the original:

    - FIND_ENTITY_AT_POS keeps its pointers in the operands of its own
      instructions, which it patches with CHAR_PTR, each mob record,
      and each event entry. find_entity_at_pos keeps them in local
      variables.
    - DECR_GROUP_COUNT and INCR_GROUP_COUNT are adjust_pc_count, which
      takes the location, the mob, and the delta that the original
      keeps in $BE, $BC, and GROUP_COUNT_DELTA.
    - APPEARANCE_TO_FONTCHAR returns the glyph in A and the font group
      in Y. appearance_to_fontchar returns the glyph in the low byte
      and the group in the high byte.
    - CLASSIFY_ENTITY_TYPE leaves the name it chooses in HANDLER_PTR
      ($BC), and classify_entity_type returns it.
    - DRAW_CHAR_AT_POS converts the position into a column and a row,
      and back with COLROW_TO_POS before it calls FIND_ENTITY_AT_POS.
      COLROW_TO_POS(pos % 20, pos / 20) is pos for every byte and
      writes no memory, so draw_char_at_pos passes pos itself.
    - DRAW_BLINK_CHAR pushes PRINT_STRING_ADDR ($BC/$BD) around its
      call of RENDER_FONT_CHAR, because DISPLAY_MESSAGE reads the
      string pointer from there after the blink has been drawn. text.c
      keeps the pointer in a local, so draw_blink_char doesn't save it.
    - The scratch bytes are locals and return values. The original
      also leaves FIND_ENTITY_AT_POS's result in ENTITY_REC ($BE/$BF),
      GET_CHAR_RECORD's in CHAR_REC ($BE/$BF), GET_MOB_DATA's in
      MOB_DATA_PTR ($BA/$BB), each name it prints in PRINT_STRING_ADDR,
      and the mob that REORDER_CHAR moves in TARGET_REC ($BC/$BD), and
      SET_BLINK_TARGET stores MOB_PTR in ENTITY_REC. No routine reads
      any of these bytes afterwards.

    Quirks of the original that are kept:

    - GET_ENTITY_FONTCHAR and CLASSIFY_ENTITY_TYPE read an event
      entry's type from byte 1 of the record they're given, and
      SET_BLINK_TARGET passes MOB_PTR without the mob tag. For a mob
      record, byte 1 is the high byte of the mob's name pointer.
    - With an empty list, FIRST_GROUP_MEMBER goes on from the old
      MOB_PTR's next link, which reads $0002 when MOB_PTR is 0.
    - REORDER_CHAR's walk of the source list runs on past the end of
      the list, and reads the bytes before MOBS for a link of 0, when
      the mob isn't in the list.
    - RENDER_FONT_CHAR_DIRECT stores its font selector only for a
      sprite past the first font of the group. Otherwise
      s_PRINT_FONT_CHAR keeps the selector it had.

    Ali Baba, main.asm lines 1455-1502, 1942-2034, 2158-2254,
    2387-2405, 2720-2861, 3001-3108, 3307-3346, 3529-3567, 4087-4126,
    13050-13069, 14077-14176, and 14583-14599:

        ORG     $0925
    LOAD_CHAR_NAME_PTR:
        SUBROUTINE LOAD_CHAR_NAME_PTR
        ; Load entity name pointer and position cursor at the right status panel.
        ;
        ; Inputs:
        ;   ENTITY_PTR  — pointer to current entity record (bytes 0-1 = name string address)
        ;
        ; Behavior:
        ;   Reads the two-byte name pointer from bytes 0-1 of the entity record
        ;   and stores it in PRINT_STRING_ADDR.  Jumps to SETUP_TEXT_POS_COL20
        ;   to position the text cursor at column 20 (right status panel).
        ;
        ; Modifies:
        ;   PRINT_STRING_ADDR
        LDWIY    ENTITY_PTR,#$00,PRINT_STRING_ADDR
        JMP     SETUP_TEXT_POS_COL20
        ORG     $0933
    SETUP_CHAR_SPRITE:
        SUBROUTINE SETUP_CHAR_SPRITE
        ; Unpack entity position and render its sprite glyph with blink cursor.
        ;
        ; Inputs:
        ;   ENTITY_PTR   — pointer to current entity record (byte 3 = packed position)
        ;   CHAR_SPRITE  — glyph index to render for this entity
        ;
        ; Behavior:
        ;   Reads the packed map position from entity byte MFIELD_POSITION and
        ;   splits it into column and row via POS_TO_COLROW.  Stores in both
        ;   font cursor and blink cursor variables.  Loads CHAR_SPRITE as the
        ;   glyph, selects custom font charset 1, then tail-calls RENDER_FONT_CHAR.
        ;
        ; Modifies:
        ;   FONT_COL, FONT_ROW, FONT_CHARNUM, FONT_CHARSET,
        ;   BLINK_COL, BLINK_ROW, BLINK_CHAR
        ; Clobbers: A, X, Y
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y
        JSR     POS_TO_COLROW
        STA     FONT_COL
        STY     FONT_ROW
        STA     BLINK_COL
        STY     BLINK_ROW
        TAX
        MOVB    CHAR_SPRITE,FONT_CHARNUM
        STA     BLINK_CHAR
        STOB    #$01,FONT_CHARSET
        JMP     RENDER_FONT_CHAR
        ORG     $0B62
    FIND_ENTITY_AT_POS:
        SUBROUTINE FIND_ENTITY_AT_POS
        ; Find the entity (mob or character) at a given map position.
        ;
        ; Inputs:
        ;   A               — target map position (0-199)
        ;   CHAR_PTR        — base address of character record
        ;
        ; Behavior:
        ;   Walks the mob linked list first; if no mob matches, searches
        ;   the player character event table.  Uses self-modifying code
        ;   to patch absolute operands for indexed loads.
        ;
        ; Outputs:
        ;   ENTITY_REC/+1   — pointer to matching entity record
        ;                      (bit 7 of high byte = MOB_PTR_FLAG if mob)
        ; Clobbers: A, X, Y

        STA     ENTITY_SEARCH_POS          ; save target position
        LDA     CHAR_PTR+1                 ; \ patch base address into
        STA     _FIND_ENTITY_AT_POS.load3+2                   ;  | player search (high byte)
        STA     .load2+2                   ;  | mob search (high byte)
        LDA     CHAR_PTR                   ;  |
        STA     _FIND_ENTITY_AT_POS.load3+1                   ;  | player search (low byte)
        STA     .load2+1                   ; / mob search (low byte)
        JMP     .mob_search

        ; --- Player character event-table search ---
    _FIND_ENTITY_AT_POS.player_search:
        LDX     #CFIELD_EVENTS             ; \ copy event list pointer (bytes 6-7)
    _FIND_ENTITY_AT_POS.load3:                                ;  |
        LDA     $0B7B,X                    ;  | (self-modified: base = CHAR_PTR)
        STA     .load4-5,X                 ;  | patch .load4 operand
        INX                                ;  |
        BXNE    #CFIELD_EVENTS+2,_FIND_ENTITY_AT_POS.load3    ; / loop for both bytes
    .load4:
        LDY     $0B86                      ; (self-modified: read position from event table)
        BYNE    #EVENT_END,_FIND_ENTITY_AT_POS.check_player   ; $FF = end of table
        JMP     CHECK_AT_BOUNDARY
    _FIND_ENTITY_AT_POS.check_player:
        BYEQ    ENTITY_SEARCH_POS,_FIND_ENTITY_AT_POS.found_player ; position matches target?
        INC     .load4+1                   ; \
        BNE     .no_carry1                 ;  |
        INC     .load4+2                   ;  | advance pointer by 3
    .no_carry1:                            ;  | (EVENT_RECORD_SIZE = 3)
        INC     .load4+1                   ;  |
        BNE     .no_carry2                 ;  |
        INC     .load4+2                   ;  |
    .no_carry2:                            ;  |
        INC     .load4+1                   ;  |
        BNE     .load4                     ;  |
        INC     .load4+2                   ; /
        JMP     .load4
    _FIND_ENTITY_AT_POS.found_player:
        LDA     .load4+1                   ; \ ENTITY_REC = event table entry address
        STA     ENTITY_REC                 ;  | (bit 7 clear = character)
        LDA     .load4+2                   ;  |
        STA     ENTITY_REC+1               ; /
        RTS

        ; --- Mob linked-list search ---
    .mob_search:
        LDX     #MFIELD_NEXT               ; byte 2 = mob link index
    .load2:
        LDA     $0BBD,X                    ; (self-modified: base = CHAR_PTR)
        BAEQ    #$00,_FIND_ENTITY_AT_POS.player_search        ; zero -> no more mobs, try players
        JSR     .get_mob                   ; resolve mob data -> patch .load1
        LDX     #MFIELD_POSITION           ; byte 3 = map position
    .load1:
        LDY     $0BC9,X                    ; (self-modified: read from mob record)
        BYEQ    ENTITY_SEARCH_POS,_FIND_ENTITY_AT_POS.found_mob ; position matches target?
        LDA     .load1+1                   ; \ copy mob link address back
        STA     .load2+1                   ;  | to .load2 for next iteration
        LDA     .load1+2                   ;  |
        STA     .load2+2                   ; /
        JMP     .mob_search
    _FIND_ENTITY_AT_POS.found_mob:
        LDA     #MOB_PTR_FLAG              ; \ set bit 7 = mob pointer flag
        ORA     .load1+2                   ;  |
        STA     ENTITY_REC+1               ;  |
        LDA     .load1+1                   ;  |
        STA     ENTITY_REC                 ; / ENTITY_REC = mob ptr | MOB_PTR_FLAG
        RTS

        ; --- Helper: load mob record and patch .load1 ---
    .get_mob:
        JSR     GET_MOB_DATA               ; A = mob index -> MOB_DATA_PTR
        LDA     MOB_DATA_PTR               ; \
        STA     .load1+1                   ;  | patch .load1 with mob record base
        LDA     MOB_DATA_PTR+1             ;  |
        STA     .load1+2                   ; /
        RTS
        ORG     $0C7E
    GET_ENTITY_FONTCHAR:
        SUBROUTINE GET_ENTITY_FONTCHAR
        ; Determine font character for entity at ENTITY_REC.
        ;
        ; Input:  ENTITY_REC — pointer to entity record
        ; Output: FONT_CHARNUM set, FONT_CHARSET = 1
        ;
        ; Behavior:
        ;   Null pointer -> BLINK_ALT_CHAR.  Mob (bit 7) -> appearance
        ;   lookup.  Character -> top 2 bits of byte 1 select glyph.
        ; Clobbers: A, Y

        STOB    #$01,FONT_CHARSET      ; select custom font
        LDA     #$00
        BANE    ENTITY_REC+1,_GET_ENTITY_FONTCHAR.has_entity
        BANE    ENTITY_REC,_GET_ENTITY_FONTCHAR.low_only
    .empty:
        MOVB    BLINK_ALT_CHAR,FONT_CHARNUM ; empty cell
        RTS
    _GET_ENTITY_FONTCHAR.low_only:
        MOVB    DEFAULT_CHAR,FONT_CHARNUM ; ZP-only pointer
        RTS
    _GET_ENTITY_FONTCHAR.has_entity:
        LDA     ENTITY_REC+1
        BAPL    #$80,_GET_ENTITY_FONTCHAR.mob              ; bit 7 set -> mob record

        ; --- character record ---
        LDY     #MFIELD_TYPE           ; byte 1: type/class
        LDA     (ENTITY_REC),Y
        AND     #FLAGS_CLASS_MASK      ; top 2 bits
        BAEQ    #FLAGS_CLASS_MASK,_GET_ENTITY_FONTCHAR.both_bits
        BAEQ    #$40,_GET_ENTITY_FONTCHAR.bit6             ; bit 6 only -> $1C
        BPL     .empty                 ; neither -> empty
        LDA     #$1B                   ; bit 7 only -> $1B
        BNE     .store
    _GET_ENTITY_FONTCHAR.bit6:
        LDA     #$1C
    .store:
        STA     FONT_CHARNUM
        RTS
    _GET_ENTITY_FONTCHAR.both_bits:
        LDA     (ENTITY_REC),Y        ; reload type byte
        BAEQ    #EVENT_EMPTY,_GET_ENTITY_FONTCHAR.check_byte2 ; $FE -> check byte 2
        BAMI    #$D5,_GET_ENTITY_FONTCHAR.lt_d5
        LDA     #$1D                   ; >= $D5 -> $1D
        JMP     .store
    _GET_ENTITY_FONTCHAR.lt_d5:
        BAMI    #$D2,_GET_ENTITY_FONTCHAR.lt_d2
        LDA     #$18                   ; $D2-$D4 -> $18
        JMP     .store
    _GET_ENTITY_FONTCHAR.lt_d2:
        LDA     #$1A                   ; < $D2 -> $1A
        JMP     .store
    _GET_ENTITY_FONTCHAR.check_byte2:
        LDY     #MFIELD_NEXT           ; byte 2: event data
        LDA     (ENTITY_REC),Y
        AND     #STRENGTH_MASK         ; low 5 bits
        BEQ     .empty                 ; zero -> empty
        JMP     .store                 ; nonzero -> use as charnum

        ; --- mob record ---
    _GET_ENTITY_FONTCHAR.mob:
        MOVB    ENTITY_REC,MOB_DATA_PTR ; \ set up mob pointer
        LDA     ENTITY_REC+1            ;  | (clear bit 7)
        AND     #$7F                    ;  |
        STA     MOB_DATA_PTR+1          ; /
        LDY     #MFIELD_APPEARANCE     ; byte 4: appearance
        LDA     (MOB_DATA_PTR),Y
        JSR     APPEARANCE_TO_FONTCHAR
        JMP     .store
        ORG     $0CF7
    APPEARANCE_TO_FONTCHAR:
        SUBROUTINE APPEARANCE_TO_FONTCHAR
        ; Convert appearance index to font character number.
        ;
        ; Input:  A = appearance index
        ; Output: A = font character number, Y = font group (appearance / 21)
        ;         A preserved on carry-through paths

        LDY      #$00                  ; Y = font group counter
    .loop:
        CLC
        ADC      #$EB                  ; subtract 21 (256-21)
        BCC      .done                 ; underflow -> done
        INY                            ; next group
        BCS      .loop                 ; always taken
    .done:
        ADC      #$16                  ; restore remainder+1 (carry clear)
        BALT    #$05,_APPEARANCE_TO_FONTCHAR.low              ; remainder >= 5 -> return as-is
        RTS
    _APPEARANCE_TO_FONTCHAR.low:
        BYEQ    #$00,_APPEARANCE_TO_FONTCHAR.ret              ; group 0 -> return (chars 1-4)
        CLC
        ADC      #$20                  ; add 32 -> next font range
    _APPEARANCE_TO_FONTCHAR.ret:
        RTS
        ORG     $0D8A
    PRINT_MOB_NAME:
        SUBROUTINE PRINT_MOB_NAME
        ; Print the current mob or character's name centered on the status line.
        ;
        ; Inputs:
        ;   ENTITY_PTR  — pointer to mob/character record; bytes 0-1 hold name string pointer
        ;
        ; Behavior:
        ;   Loads the name string pointer from bytes 0-1 of the ENTITY_PTR record
        ;   into PRINT_STRING_ADDR, then jumps to PRINT_BOTTOM_CENTERED to print
        ;   the name centered on row 23.
        ;
        ; Modifies:
        ;   PRINT_STRING_ADDR
        ; Clobbers: A, Y

        LDWIY    ENTITY_PTR,#$00,PRINT_STRING_ADDR ;  PRINT_STRING_ADDR/PRINT_STRING_ADDR+1 = bytes 0-1 of ($F8)
        JMP     PRINT_BOTTOM_CENTERED ; center and print on row 23
        ORG     $0F84
    REORDER_CHAR:
        SUBROUTINE REORDER_CHAR
        ; Move a character from one group's chain into another, sorted by level.
        ;
        ; Inputs:
        ;   SOURCE_CHAR     — index of the character to remove from its current group
        ;   ACTIVE_CHAR     — destination group head index (0 = random insertion)
        ;   TARGET_REC      — record pointer for the character being moved
        ;
        ; Behavior:
        ;   Phase 1 walks SOURCE_CHAR's group chain to find the predecessor node,
        ;   patches it to skip over the removed node (unlinking it), and decrements
        ;   the source group's member count.
        ;   Phase 2 walks the destination group chain and inserts the removed node
        ;   at the correct position: in ascending level order when ACTIVE_CHAR != 0,
        ;   or at a random position when ACTIVE_CHAR = 0 (coin-flip via STEP_PRNG).
        ;   After insertion, adjusts TOTAL_MOB_COUNT if the moved character is an
        ;   NPC (appearance < NPC_APPEARANCE_MIN): decrements if source was group 0,
        ;   increments if destination is group 0.
        ;
        ; Modifies:
        ;   REORDER_LINK, REORDER_SORT_KEY, CURRENT_ROOM, TOTAL_MOB_COUNT,
        ;   CHAR_REC, MOB_DATA_PTR
        ; Clobbers: A, X, Y

    ; --- Phase 1: unlink character SOURCE_CHAR from its current chain ---
        LDA     SOURCE_CHAR            ; source character index
        JSR     GET_CHAR_RECORD        ; CHAR_REC = record pointer
        JSR     DECR_GROUP_COUNT       ; source group lost a member
    .walk1:
        LDY     #CFIELD_NEXT           ; \
        LDA     (CHAR_REC),Y           ;  | REORDER_LINK = byte 2 of current record
        STA     REORDER_LINK           ; /  (the "next" link we're removing)
        JSR     GET_MOB_DATA           ; MOB_DATA_PTR/MOB_DATA_PTR+1 = resolve link; TARGET_REC/TARGET_REC+1 = mob data
        LDA     MOB_DATA_PTR           ; \
        BANE    TARGET_REC,_REORDER_CHAR.advance1   ; if different, keep walking
        LDA     MOB_DATA_PTR+1         ;  |
        BANE    TARGET_REC+1,_REORDER_CHAR.advance1
        ; Found predecessor: TARGET_REC/TARGET_REC+1 points to the node being removed
        LDY     #CFIELD_NEXT
        LDA     REORDER_LINK           ; \  if removed node's link == CURRENT_ROOM,
        BANE    CURRENT_ROOM,_REORDER_CHAR.patch1
        LDA     (TARGET_REC),Y         ; \
        STA     CURRENT_ROOM           ; /  CURRENT_ROOM = (TARGET_REC)[CFIELD_NEXT] (successor's link)
    _REORDER_CHAR.patch1:
        LDA     (TARGET_REC),Y         ; \  predecessor.byte2 = removed.byte2
        STA     (CHAR_REC),Y           ; /  (skip over removed node)
        JMP     .phase2
    _REORDER_CHAR.advance1:
        MOVW    MOB_DATA_PTR,CHAR_REC ; CHAR_REC = MOB_DATA_PTR/MOB_DATA_PTR+1
        JMP     .walk1

    ; --- Phase 2: re-insert at correct sorted position in ACTIVE_CHAR's chain ---
    .phase2:
        LDY     #MFIELD_LEVEL          ; \
        LDA     (TARGET_REC),Y         ;  | REORDER_SORT_KEY = byte 11 of removed node
        AND     #LEVEL_MASK            ;  |   (level, 5-bit)
        STA     REORDER_SORT_KEY       ; /
        LDA     ACTIVE_CHAR            ; destination character index
        JSR     GET_CHAR_RECORD        ; CHAR_REC = dest record pointer
        JSR     INCR_GROUP_COUNT       ; dest group gained a member
    .walk2:
        LDY     #MFIELD_NEXT           ; \
        LDA     (CHAR_REC),Y           ;  | check byte 2 of current record
        BAEQ    #$00,_REORDER_CHAR.insert
        JSR     GET_MOB_DATA           ; resolve link
        LDA     ACTIVE_CHAR            ; \
        BEQ     .random                ; /  if dest index = 0, use random decision
        LDY     #MFIELD_LEVEL          ; \
        LDA     (MOB_DATA_PTR),Y       ;  | compare candidate's level
        AND     #LEVEL_MASK            ;  | with removed node's level
        BAPL    REORDER_SORT_KEY,_REORDER_CHAR.advance2 ; if >= , keep walking (insert later)
    _REORDER_CHAR.insert:
        LDY     #MFIELD_NEXT           ; \
        LDA     (CHAR_REC),Y           ;  | (TARGET_REC)[CFIELD_NEXT] = (CHAR_REC)[MFIELD_NEXT]
        STA     (TARGET_REC),Y         ;  |
        LDA     REORDER_LINK           ;  | (CHAR_REC)[MFIELD_NEXT] = saved link (insert node)
        STA     (CHAR_REC),Y           ; /
        LDY     #CFIELD_APPEARANCE     ; \
        LDA     (TARGET_REC),Y         ;  | if byte 4 of ($BC) >= $15, done
        BALT    #$15,_REORDER_CHAR.adjust
        RTS                            ; /
    _REORDER_CHAR.adjust:
        LDA     #$00                   ; \
        BANE    ACTIVE_CHAR,_REORDER_CHAR.chk28     ; decrement TOTAL_MOB_COUNT
        DEC     TOTAL_MOB_COUNT        ; /
    _REORDER_CHAR.chk28:
        BANE    SOURCE_CHAR,_REORDER_CHAR.done      ; increment TOTAL_MOB_COUNT
        INC     TOTAL_MOB_COUNT        ; /
    _REORDER_CHAR.done:
        RTS
    _REORDER_CHAR.advance2:
        MOVW    MOB_DATA_PTR,CHAR_REC ; CHAR_REC = MOB_DATA_PTR/MOB_DATA_PTR+1
        JMP     .walk2
    .random:
        JSR     STEP_PRNG              ; \  random decision
        LDA     PRNG_OUTPUT            ;  | if PRNG < 1 (i.e., == 0) -> insert
        BAMI    #$01,_REORDER_CHAR.insert ;  else keep walking
        BPL     _REORDER_CHAR.advance2              ; /
        ORG     $102F
    DECR_GROUP_COUNT:
        SUBROUTINE DECR_GROUP_COUNT
        ; Decrement or increment the group count byte of a mob data record.
        ;
        ; Inputs:
        ;   DATA_PTR    — pointer to mob data record ($BE/$BF); null = no-op
        ;   TARGET_REC  — pointer to character record ($BC/$BD); byte 4 (appearance)
        ;                 must be < NPC_APPEARANCE_MIN or call is a no-op
        ;
        ; Behavior:
        ;   Stores -1 or +1 in GROUP_COUNT_DELTA, then bails if DATA_PTR is null
        ;   or if the character's appearance index indicates an inactive NPC.
        ;   Otherwise adds the delta to byte 8 of the mob data record at DATA_PTR,
        ;   which encodes group count (bits 3-0), pending XP (bits 6-4), and status
        ;   (bit 7) in a packed byte.
        ;
        ; Modifies:
        ;   GROUP_COUNT_DELTA, mob data byte 8 at (DATA_PTR + MFIELD_XP)
        ; Clobbers: A, Y

        LDA     #$FF              ; delta = -1
        BNE     .apply            ; always taken
    INCR_GROUP_COUNT:
        LDA     #$01              ; delta = +1
    .apply:
        STA     GROUP_COUNT_DELTA ; save delta for later
        LDA     DATA_PTR          ; \
        BANE    #$00,_DECR_GROUP_COUNT.not_null
        RTS                       ; /
    _DECR_GROUP_COUNT.not_null:
        LDY     #CFIELD_APPEARANCE ; \
        LDA     (TARGET_REC),Y    ;  | bail if appearance >= $15 (inactive/dead)
        BALT    #NPC_APPEARANCE_MIN,_DECR_GROUP_COUNT.ok
        RTS                       ; /
    _DECR_GROUP_COUNT.ok:
        LDY     #MFIELD_XP        ; \
        LDA     (DATA_PTR),Y      ;  | group count (low nibble) += delta
        CLC                       ;  |
        ADC     GROUP_COUNT_DELTA ;  |
        STA     (DATA_PTR),Y      ; /
        RTS
        ORG     $1142
    PRINT_ENTITY_NAME:
        SUBROUTINE PRINT_ENTITY_NAME
        ; Print the target entity's name centered on the status line.
        ;
        ; Inputs:
        ;   EFFECT_REC  — pointer to target entity record; bytes 0-1 hold name string pointer
        ;
        ; Behavior:
        ;   Positions the cursor on row 21, loads the name string pointer from
        ;   bytes 0-1 of the EFFECT_REC record into PRINT_STRING_ADDR, prints
        ;   the name centered on the bottom status line, then resets the text
        ;   window via SCROLL_STATUS_LINE.
        ;
        ; Modifies:
        ;   PRINT_STRING_ADDR
        ; Clobbers: A, Y

        JSR     SET_CURSOR_ROW21
        LDWIY    EFFECT_REC,#$00,PRINT_STRING_ADDR ; name pointer low
        JSR     PRINT_BOTTOM_CENTERED
        JMP     SCROLL_STATUS_LINE
        ORG     $1156
    DRAW_CHAR_AT_POS:
        SUBROUTINE DRAW_CHAR_AT_POS
        ; Draw the entity sprite at a map position.
        ;
        ; Inputs:  A = map position (0-199)
        ; Behavior:
        ;   Converts position to font col/row, finds the entity there,
        ;   determines its font character, and renders it.
        ; Clobbers: A, X, Y

        JSR     POS_TO_COLROW       ; A=pos -> A=col, Y=row
        STA     FONT_COL            ; set font column
        STY     FONT_ROW            ; set font row
        JSR     COLROW_TO_POS       ; re-linearize (A = col + row*20)
        JSR     FIND_ENTITY_AT_POS  ; resolve entity at pos -> ENTITY_REC
        JSR     GET_ENTITY_FONTCHAR ; entity data -> font char number
        JMP     RENDER_FONT_CHAR    ; tail-call: render and return
        ORG     $116B
    CLASSIFY_ENTITY_TYPE:
        SUBROUTINE CLASSIFY_ENTITY_TYPE
        ; Classify entity at ENTITY_REC and set HANDLER_PTR.
        ;
        ; Input:  ENTITY_REC — pointer to entity record
        ; Output: HANDLER_PTR — handler address for entity type
        ;
        ; Behavior:
        ;   Decision tree on pointer value and byte 1/2 fields.
        ;   9 distinct handler addresses for empty slots, mobs,
        ;   shops, map features, treasure, and event types.
        ; Clobbers: A, X, Y

        LDA     #$00
        BANE    ENTITY_REC+1,_CLASSIFY_ENTITY_TYPE.not_zero_page ; high byte nonzero -> entity
        BANE    ENTITY_REC,_CLASSIFY_ENTITY_TYPE.type_C3    ; low byte nonzero -> ZP pointer
    _CLASSIFY_ENTITY_TYPE.type_E4:
        STOW    $7FE4,HANDLER_PTR      ; null pointer handler
        RTS
    _CLASSIFY_ENTITY_TYPE.type_C3:
        STOW    $7FC3,HANDLER_PTR      ; ZP pointer handler
        RTS
    _CLASSIFY_ENTITY_TYPE.not_zero_page:
        LDY     #MFIELD_TYPE           ; byte 1: entity type
        LDA     (ENTITY_REC),Y
        BMI     .check_ranges          ; bit 7 set -> check ranges
    _CLASSIFY_ENTITY_TYPE.type_FC:
        STOW    $7FFC,HANDLER_PTR      ; positive type handler
        RTS
    .check_ranges:
        BAMI    #$C0,_CLASSIFY_ENTITY_TYPE.type_E4          ; < $C0 -> null handler
        BAPL    #$D2,_CLASSIFY_ENTITY_TYPE.check_d5         ; >= $D2 -> check next
        STOW    $7FD3,HANDLER_PTR      ; $C0-$D1 handler
        RTS
    _CLASSIFY_ENTITY_TYPE.check_d5:
        BAPL    #$D5,_CLASSIFY_ENTITY_TYPE.check_fe         ; >= $D5 -> check event
        STOW    $8016,HANDLER_PTR      ; $D2-$D4 handler
        RTS
    _CLASSIFY_ENTITY_TYPE.check_fe:
        BAEQ    #EVENT_EMPTY,_CLASSIFY_ENTITY_TYPE.type_fe_entry ; $FE -> event classification
        STOW    $8028,HANDLER_PTR      ; $D5-$FD handler
        RTS
    _CLASSIFY_ENTITY_TYPE.type_fe_entry:
        LDY     #MFIELD_NEXT           ; byte 2: event data
        LDA     (ENTITY_REC),Y
        TAX
        AND     #STRENGTH_MASK         ; bits 4-0
        BNE     .check_c0_bits         ; nonzero -> check class bits
        STOW    $8003,HANDLER_PTR      ; strength = 0 handler
        RTS
    .check_c0_bits:
        TXA
        AND     #FLAGS_CLASS_MASK      ; bits 7-6
        BANE    #FLAGS_CLASS_MASK,_CLASSIFY_ENTITY_TYPE.check_1b ; both set -> check further
    .type_F0:
        STOW    $7FF0,HANDLER_PTR      ; class != $C0 handler
        RTS
    _CLASSIFY_ENTITY_TYPE.check_1b:
        TXA
        AND     #STRENGTH_MASK
        BAPL    #$1B,_CLASSIFY_ENTITY_TYPE.check_1d         ; >= $1B -> check $1D
        STOW    $7FCA,HANDLER_PTR      ; < $1B handler
        RTS
    _CLASSIFY_ENTITY_TYPE.check_1d:
        BAMI    #$1D,_CLASSIFY_ENTITY_TYPE.type_FC
        BEQ     .type_F0
        JMP     _CLASSIFY_ENTITY_TYPE.type_C3
        ORG     $12FA
    FIRST_GROUP_MEMBER:
        SUBROUTINE FIRST_GROUP_MEMBER
        ; Advance group-member cursor to the first or next mob in a linked chain.
        ;
        ; Inputs:
        ;   CHAR_PTR  — points to the group-head character record ($FA/$FB);
        ;               byte 2 (CFIELD_NEXT) holds the first mob link index
        ;   MOB_PTR   — points to the current mob record ($F4/$F5);
        ;               byte 2 (MFIELD_NEXT) holds the next mob link index
        ;
        ; Behavior:
        ;   FIRST_GROUP_MEMBER reads the next-link from byte 2 of the group head
        ;   (CHAR_PTR).  NEXT_GROUP_MEMBER reads it from byte 2 of the current
        ;   mob cursor (MOB_PTR).  If the link index is non-zero, GET_MOB_DATA
        ;   resolves it to a 16-byte record pointer and advances MOB_PTR.  If the
        ;   link index is zero the chain is exhausted and MOB_PTR is set to null.
        ;
        ; Outputs:
        ;   MOB_PTR  — updated to point to the next mob record, or $0000 at
        ;              end of chain
        ;
        ; Modifies:
        ;   MOB_PTR, MOB_DATA_PTR
        ; Clobbers: A, Y

        LDY     #CFIELD_NEXT   ; \
        LDA     (CHAR_PTR),Y   ;  | check byte 2 of group head ($FA)
        BNE     .resolve       ; /  if nonzero, follow it
    NEXT_GROUP_MEMBER:
        LDY     #MFIELD_NEXT   ; \
        LDA     (MOB_PTR),Y    ;  | check byte 2 of cursor ($F4)
        BNE     .resolve       ; /  if nonzero, follow it
        STA     MOB_PTR        ; \  end of chain:
        STA     MOB_PTR+1      ; /  null out MOB_PTR/MOB_PTR+1
        RTS
    .resolve:
        JSR     GET_MOB_DATA   ; resolve link -> MOB_DATA_PTR/MOB_DATA_PTR+1
        MOVW    MOB_DATA_PTR,MOB_PTR ;  MOB_PTR/MOB_PTR+1 = MOB_DATA_PTR/MOB_DATA_PTR+1
        RTS
        ORG     $1406
    PRINT_MEMBER_NAME:
        SUBROUTINE PRINT_MEMBER_NAME
        ; Print the current group member's name centered on the status line.
        ;
        ; Inputs:
        ;   MOB_PTR  — pointer to group member record; bytes 0-1 hold name string pointer
        ;
        ; Behavior:
        ;   Loads the name string pointer from bytes 0-1 of the MOB_PTR record
        ;   into PRINT_STRING_ADDR, then jumps to PRINT_BOTTOM_CENTERED to print
        ;   the name centered on row 23.
        ;
        ; Modifies:
        ;   PRINT_STRING_ADDR
        ; Clobbers: A, Y

        LDWIY    MOB_PTR,#$00,PRINT_STRING_ADDR
        JMP     PRINT_BOTTOM_CENTERED
        ORG     $1414
    REORDER_CURRENT_CHAR:
        SUBROUTINE REORDER_CURRENT_CHAR
        ; Re-sort the active character after a stat change.
        ;
        ; Inputs:
        ;   ENTITY_PTR      — pointer to the current character record
        ;   CURRENT_PLAYER  — index of the current player character
        ;
        ; Behavior:
        ;   Copies ENTITY_PTR into TARGET_REC and CURRENT_PLAYER into
        ;   SOURCE_CHAR, then tail-calls REORDER_CHAR to re-insert the
        ;   character into sorted order.
        ;
        ; Modifies:
        ;   TARGET_REC, SOURCE_CHAR
        ; Clobbers: A
        MOVW    ENTITY_PTR,TARGET_REC
        MOVB    CURRENT_PLAYER,SOURCE_CHAR
        JMP     REORDER_CHAR
        ORG     $16A3
    SET_BLINK_TARGET:
        SUBROUTINE SET_BLINK_TARGET
        ; Set blink cursor position and character from current combatant.
        ;
        ; Inputs:
        ;   IS_PLAYER_TURN  — 0 = mob, 1 = player
        ;   MOB_PTR         — pointer to current entity's record
        ;
        ; Behavior:
        ;   On mob's turn, unpacks position from mob record and converts
        ;   appearance to font char.  On player's turn, reads position
        ;   from CURRENT_COL/ROW and looks up font char via entity record.
        ;
        ; Outputs: BLINK_COL, BLINK_ROW, BLINK_CHAR
        ; Clobbers: A, X, Y

        LDA     IS_PLAYER_TURN              ; 0 = mob, 1 = player
        BAEQ    #$01,_SET_BLINK_TARGET.player

        ; --- mob path: unpack position and look up appearance ---
        LDY     #MFIELD_POSITION            ; byte 3: packed map position
        LDA     (MOB_PTR),Y
        JSR     POS_TO_COLROW               ; -> A = col, Y = row
        STA     BLINK_COL
        STY     BLINK_ROW
        LDY     #MFIELD_APPEARANCE          ; byte 4: appearance index
        LDA     (MOB_PTR),Y
        JSR     APPEARANCE_TO_FONTCHAR      ; -> A = font char number
        STA     BLINK_CHAR
        RTS

        ; --- player path: position already split ---
    _SET_BLINK_TARGET.player:
        MOVB    CURRENT_COL,BLINK_COL       ; copy player column
        MOVB    CURRENT_ROW,BLINK_ROW       ; copy player row
        MOVW    MOB_PTR,ENTITY_REC          ; point ENTITY_REC at player record
        JSR     GET_ENTITY_FONTCHAR         ; look up player's font char
        MOVB    FONT_CHARNUM,BLINK_CHAR     ; store result
        RTS
        ORG     $6EC9
    LOAD_CHAR_TO_FA:
        SUBROUTINE LOAD_CHAR_TO_FA
        ; Load a character record and expose its pointer via CHAR_PTR.
        ;
        ; Inputs:
        ;   A  — character index
        ;
        ; Behavior:
        ;   Calls GET_CHAR_RECORD to resolve the index into a record pointer
        ;   in DATA_PTR, then copies it to CHAR_PTR so callers can iterate
        ;   the record via the main-loop zero-page pointer.
        ;
        ; Modifies:
        ;   CHAR_PTR
        ; Clobbers: A, X, Y
        ; Wrapper: GET_CHAR_RECORD then copy CHAR_REC -> $FA/$FB.
        JSR     GET_CHAR_RECORD
        MOVW    DATA_PTR,CHAR_PTR
        RTS
        ORG     $7489
    RENDER_FONT_CHAR:
        SUBROUTINE RENDER_FONT_CHAR
        ; Draw a 2x2 custom font character at the current font position.
        ;
        ; Inputs:
        ;   FONT_COL, FONT_ROW   — font grid position
        ;   FONT_CHARNUM          — character index within font group
        ;   FONT_CHARSET          — font group (1 = primary, 2 = border)
        ;
        ; Behavior:
        ;   Converts font position to HRCG text cursor, maps charnum to
        ;   four consecutive glyph codes across HRCG charset pages, patches
        ;   s_PRINT_FONT_CHAR, and calls PRINT_STRING to render.
        ;
        ; Modifies: s_PRINT_FONT_CHAR (CHAR_CHARSET, glyph codes),
        ;           PRINT_STRING_ADDR
        ; Clobbers: A, X, Y

        JSR     FONT_POS_TO_TEXT_POS        ; convert font pos to HRCG cursor
    PRINT_FONTCHAR_AT_TEXT_POS:
        ; --- Map font group to initial HRCG charset selector ---
        LDX     #HRCG_CHARSET1_SEL          ; default: HRCG charset 1
        LDA     FONT_CHARSET
        BANE    #$02,_RENDER_FONT_CHAR.charset_ok            ; group 1 -> use charset 1
        INX                                 ; group 2: skip charset 2 (border)
        INX                                 ;          -> HRCG charset 3
    _RENDER_FONT_CHAR.charset_ok:
        STX     CHAR_CHARSET                ; patch selector in control string
        ; --- Walk charset pages until charnum fits ---
    RENDER_FONT_CHAR_DIRECT:                ; entry: render from FONT_CHARNUM directly
        LDA     FONT_CHARNUM
    .check_range:
        BALT    #CHARS_PER_FONT_PAGE,_RENDER_FONT_CHAR.compute_glyphs ; < 24? fits in current page
        INX                                 ; advance to next HRCG charset
        STX     CHAR_CHARSET
        SEC
        SBC     #CHARS_PER_FONT_PAGE        ; charnum -= 24
        JMP     .check_range
        ; --- Compute four consecutive glyph codes ---
    _RENDER_FONT_CHAR.compute_glyphs:
        ASL                                 ; charnum * 2
        ASL                                 ; charnum * 4
        CLC
        ADC     #GLYPH_BASE                 ; + $20 -> upper-left glyph code
        TAY
        STY     CHAR_UPPER_LEFT             ; glyph 0: upper-left
        INY
        STY     CHAR_UPPER_RIGHT            ; glyph 1: upper-right
        INY
        STY     CHAR_LOWER_LEFT             ; glyph 2: lower-left
        INY
        STY     CHAR_LOWER_RIGHT            ; glyph 3: lower-right
        ; --- Render via HRCG ---
        STOW    s_PRINT_FONT_CHAR,PRINT_STRING_ADDR ; aim string ptr at control string
        JMP     PRINT_STRING                ; HRCG renders the 2x2 block
        ORG     $74CB
    DRAW_BLINK_ALT:
        SUBROUTINE DRAW_BLINK_ALT
        ; Draw blink position with the alternate (tile) character.
        ;
        ; Behavior:
        ;   Saves BLINK_CHAR, swaps in BLINK_ALT_CHAR, draws, restores.
        ; Clobbers: A, X, Y

        LDA     BLINK_CHAR              ; save current blink char (entity sprite)
        PHA
        MOVB    BLINK_ALT_CHAR,BLINK_CHAR ; swap in alternate (location tile)
        JSR     DRAW_BLINK_CHAR         ; draw alternate at blink position
        PULB    BLINK_CHAR              ; restore sprite char
        RTS
        ORG     $74DD
    DRAW_BLINK_NORMAL:
        JSR     PRINT_CTRL_N                ; Ctrl-N: restore normal video

    DRAW_BLINK_CHAR:
        SUBROUTINE DRAW_BLINK_CHAR
        ; Render the blink character at the stored blink position.
        ;
        ; Behavior:
        ;   If BLINK_COL >= MAP_WIDTH (disabled), returns immediately.
        ;   Otherwise copies blink state to font vars, selects charset 1,
        ;   and calls RENDER_FONT_CHAR.
        ;
        ; Modifies: FONT_COL, FONT_ROW, FONT_CHARNUM, FONT_CHARSET
        ; Clobbers: A, X, Y

        LDA     BLINK_COL               ; load blink column
        BALT    #MAP_WIDTH,_DRAW_BLINK_CHAR.draw        ; < 20? -> active, draw it
        RTS                             ; blink disabled -- do nothing

    _DRAW_BLINK_CHAR.draw:
        STA     FONT_COL                ; set font column from blink state
        MOVB    BLINK_ROW,FONT_ROW      ; set font row
        MOVB    BLINK_CHAR,FONT_CHARNUM ; set character number
        STOB    #$01,FONT_CHARSET       ; charset 1 (custom font)
        PSHW    PRINT_STRING_ADDR       ; save (RENDER_FONT_CHAR overwrites)
        JSR     RENDER_FONT_CHAR        ; render the blink character
        PULW    PRINT_STRING_ADDR       ; restore caller's string pointer
        RTS
        ORG     $7723
    FONT_POS_TO_TEXT_POS:
        SUBROUTINE FONT_POS_TO_TEXT_POS
        ; Convert font grid position to HRCG text cursor.
        ; Each font cell = 2x2 HRCG cells, so double both coordinates.
        ;
        ; Inputs:  FONT_COL, FONT_ROW
        ; Outputs: TEXT_COL ($24), TEXT_ROW ($25)
        ; Clobbers: A

        LDA     FONT_COL            ; font column (0-based)
        ASL                         ; x2: one font col = two text cols
        STA     TEXT_COL             ; store HRCG cursor column
        LDA     FONT_ROW            ; font row (0-based)
        ASL                         ; x2: one font row = two text rows
        STA     TEXT_ROW             ; store HRCG cursor row
        RTS
*/
#include "ali_baba.h"

/* The HRCG font selector, after HRCG_CTRL_A, of the first font of
 * FONT_CHARSET 1. FONT_CHARSET 2 starts two fonts further on. */
constexpr uint8_t FONT_SELECTOR_1 = 0xB1;
/* The sprites in one font of 96 glyphs, and the glyph of a font's
 * first sprite. */
constexpr uint8_t SPRITES_PER_FONT = 24;
constexpr uint8_t FIRST_SPRITE_GLYPH = 0x20;

/* The sprites GET_ENTITY_FONTCHAR draws for event entries. */
enum {
    SPRITE_RANDOM_ENCOUNTER = 0x1B,     /* types $00-$3F */
    SPRITE_MOB_ENCOUNTER = 0x1C,        /* types $40-$7F */
    SPRITE_TREASURE = 0x1A,             /* types $C0-$D1 */
    SPRITE_SCENE = 0x18,                /* types $D2-$D4 */
    SPRITE_SHOP = 0x1D,                 /* types $D5-$FD and $FF */
};
/* An object entry (type $FE) draws the sprite in bits 4-0 of its
 * value. Bits 7-6 set make CLASSIFY_ENTITY_TYPE look at the sprite. */
constexpr uint8_t OBJECT_SPRITE_MASK = 0x1F;
constexpr uint8_t OBJECT_CLASS_MASK = 0xC0;

/* Prints ENTITY_PTR's name centered in the right-hand panel. */
void load_char_name_ptr(void) {
    setup_text_pos_col20(ENTITY_PTR->name);
}

/* Draws sprite CHAR_SPRITE at ENTITY_PTR's cell, and makes that cell
 * the blinking one with the same sprite. */
void setup_char_sprite(void) {
    uint16_t colrow = pos_to_colrow(ENTITY_PTR->pos);
    uint8_t col = (uint8_t)colrow;
    uint8_t row = (uint8_t)(colrow >> 8);

    FONT_COL = col;
    FONT_ROW = row;
    BLINK_COL = col;
    BLINK_ROW = row;
    FONT_CHARNUM = CHAR_SPRITE;
    BLINK_CHAR = CHAR_SPRITE;
    FONT_CHARSET = 1;
    render_font_char();
}

/* The event list is searched only when no mob of the location is at
 * `pos`, and it ends at an entry whose pos is EVENT_END. A search for
 * EVENT_EMPTY's pos finds the first free slot. */
entity_ref find_entity_at_pos(uint8_t pos) {
    uint8_t link;
    struct mob *m;
    struct event *ev;

    LOCATION_ID = pos;      /* ENTITY_SEARCH_POS */
    for (link = CHAR_PTR->mob_head; link != 0; link = m->next) {
        m = get_mob_data(link);
        if (m->pos == pos)
            return MOB_REF(m);
    }
    for (ev = CHAR_PTR->events; ev->pos != EVENT_END; ev++) {
        if (ev->pos == pos)
            return (entity_ref)ev;
    }
    return check_at_boundary(pos);
}

/* The sprite for an event entry depends on its type's top two bits.
 * Types $80-$BF draw as the empty cell. The original's comments give
 * $1B for $80-$BF and the empty cell for $00-$3F, which the code
 * doesn't bear out: after `CMP #$40`, its BPL to the empty cell is
 * taken for $80 and not for $00. */
uint8_t get_entity_fontchar(entity_ref e) {
    uint8_t sprite;

    FONT_CHARSET = 1;
    if ((e >> 8) == 0) {
        sprite = e == ENTITY_NONE ? BLINK_ALT_CHAR : DEFAULT_CHAR;
    } else if (ENTITY_IS_MOB(e)) {
        sprite = (uint8_t)appearance_to_fontchar(ENTITY_MOB(e)->appearance);
    } else {
        const struct event *ev = ENTITY_EVENT(e);
        uint8_t type = ev->type;
        uint8_t class = type & EVENT_ACTIVATED;

        if (class == 0x00) {
            sprite = SPRITE_RANDOM_ENCOUNTER;
        } else if (class == EVENT_TYPE_MOB) {
            sprite = SPRITE_MOB_ENCOUNTER;
        } else if (class == 0x80) {
            sprite = BLINK_ALT_CHAR;
        } else if (type == EVENT_TYPE_OBJECT) {
            sprite = ev->value & OBJECT_SPRITE_MASK;
            if (sprite == 0)
                sprite = BLINK_ALT_CHAR;
        } else if (type >= EVENT_TYPE_WEAPON_SHOP) {
            sprite = SPRITE_SHOP;
        } else if (type >= EVENT_TYPE_SCENE) {
            sprite = SPRITE_SCENE;
        } else {
            sprite = SPRITE_TREASURE;
        }
    }
    FONT_CHARNUM = sprite;
    return sprite;
}

/* Each group of 21 appearances is a font group. Appearance r of a group
 * is sprite r + 1, except that the first four of every group but the
 * first are sprites $21-$24. */
uint16_t appearance_to_fontchar(uint8_t appearance) {
    uint8_t group = appearance / 21;
    uint8_t sprite = appearance % 21 + 1;
    if (sprite < 5 && group != 0)
        sprite += 0x20;
    return sprite | (uint16_t)group << 8;
}

/* Prints ENTITY_PTR's name centered on the bottom line. */
void print_mob_name(void) {
    print_bottom_centered(ENTITY_PTR->name);
}

/* Adds `delta` (1 or -1) to the location's count of player
 * characters, when `m` is a player character and `loc` isn't record 0.
 * The original tests for record 0 by the low byte of its address.
 * get_char_record returns records 0-128, and of those only record 0's
 * address has a low byte of 0. The original's comments describe byte 8
 * as a group count in the mob record, where the code changes the
 * location's. */
static void adjust_pc_count(struct location *loc, const struct mob *m,
                            int8_t delta) {
    if (loc != &LOCATIONS[0] && m->appearance < NPC_APPEARANCE_MIN)
        loc->pc_count += delta;
}

/*
    Unlinks `m` from location SOURCE_CHAR's list and links it into
    ACTIVE_CHAR's. The walks follow a pointer to the link byte, which is
    a location's mob_head or a mob's next.

    In a location's list, `m` goes in before the first mob of a lower
    level. In record 0's list, each mob passed has a chance of about one
    half of being the one `m` goes before: `m` goes in when STEP_PRNG's
    output less 1 is negative as a signed byte, that is, for an output
    of 0 or $81-$FF. The original's comment says only for 0.

    TOTAL_MOB_COUNT counts the player characters outside record 0, so
    it goes down when one moves into record 0 and up when one leaves
    it. The original's header comment says the opposite, and calls a
    player character an NPC.
*/
void reorder_char(struct mob *m) {
    struct location *loc = get_char_record(SOURCE_CHAR);
    adjust_pc_count(loc, m, -1);
    uint8_t *link = &loc->mob_head;
    uint8_t index;
    struct mob *p;
    for (;;) {
        index = *link;
        p = get_mob_data(index);
        if (p == m)
            break;
        link = &p->next;
    }
    /* PROCESS_MOB_GROUPS's next mob moves on past `m`. */
    if (index == CURRENT_ROOM)
        CURRENT_ROOM = m->next;
    *link = m->next;

    uint8_t level = m->level & LEVEL_MASK;
    loc = get_char_record(ACTIVE_CHAR);
    adjust_pc_count(loc, m, 1);
    link = &loc->mob_head;
    while (*link != 0) {
        p = get_mob_data(*link);
        if (ACTIVE_CHAR == 0) {
            uint8_t r = step_prng();
            if (r == 0 || r > 0x80)
                break;
        } else if ((p->level & LEVEL_MASK) < level) {
            break;
        }
        link = &p->next;
    }
    m->next = *link;
    *link = index;

    if (m->appearance < NPC_APPEARANCE_MIN) {
        if (ACTIVE_CHAR == 0)
            TOTAL_MOB_COUNT--;
        if (SOURCE_CHAR == 0)
            TOTAL_MOB_COUNT++;
    }
}

/* Prints `m`'s name centered on row 21 and scrolls the status lines. */
void print_entity_name(const struct mob *m) {
    set_cursor_row21();
    print_bottom_centered(m->name);
    scroll_status_line();
}

void draw_char_at_pos(uint8_t pos) {
    uint16_t colrow = pos_to_colrow(pos);

    FONT_COL = (uint8_t)colrow;
    FONT_ROW = (uint8_t)(colrow >> 8);
    get_entity_fontchar(find_entity_at_pos(pos));
    render_font_char();
}

/*
    The name for an entity_ref, as PLAYER_ATTACK and DRAW_MAP_ENTITIES
    print it. An object entry (type $FE) is named by its value: bits
    4-0 of 0 are an invisible wall, and otherwise bits 7-6 not both set
    are an illusion, and with both set sprites $01-$1A are a statue,
    $1B-$1C a door, $1D an illusion, and $1E-$1F a wall. The original's
    comment on `BANE #FLAGS_CLASS_MASK` describes the branch the other
    way round.

    No caller passes a mob. For one, the original reads byte 1 at the
    tagged address in $C000-$D47F, and so does this.
*/
const uint8_t *classify_entity_type(entity_ref e) {
    if ((e >> 8) == 0)
        return e == ENTITY_NONE ? S_EMPTY_SPACE : S_A_WALL;
    const struct event *ev = ENTITY_EVENT(e);
    uint8_t type = ev->type;
    if (type < 0x80)
        return S_A_DOOR;
    if (type < EVENT_ACTIVATED)
        return S_EMPTY_SPACE;
    if (type < EVENT_TYPE_SCENE)
        return S_A_TREASURE_CHEST;
    if (type < EVENT_TYPE_WEAPON_SHOP)
        return S_SOME_RUNE_WRITING;
    if (type != EVENT_TYPE_OBJECT)
        return S_A_TRADING_OUTPOST;

    uint8_t value = ev->value;
    uint8_t sprite = value & OBJECT_SPRITE_MASK;
    if (sprite == 0)
        return S_AN_INVISIBLE_WALL;
    if ((value & OBJECT_CLASS_MASK) != OBJECT_CLASS_MASK)
        return S_AN_ILLUSION;
    if (sprite < 0x1B)
        return S_A_STATUE;
    if (sprite < 0x1D)
        return S_A_DOOR;
    if (sprite == 0x1D)
        return S_AN_ILLUSION;
    return S_A_WALL;
}

/* Points MOB_PTR at mob `link`, or at 0 for a link of 0. */
static void follow_link(uint8_t link) {
    MOB_PTR = link == 0 ? nullptr : get_mob_data(link);
}

void first_group_member(void) {
    uint8_t link = CHAR_PTR->mob_head;

    if (link == 0)
        link = MOB_PTR->next;
    follow_link(link);
}

void next_group_member(void) {
    follow_link(MOB_PTR->next);
}

/* Prints MOB_PTR's name centered on the bottom line. */
void print_member_name(void) {
    print_bottom_centered(MOB_PTR->name);
}

/* SOURCE_CHAR keeps CURRENT_PLAYER afterwards. */
void reorder_current_char(void) {
    SOURCE_CHAR = CURRENT_PLAYER;
    reorder_char(ENTITY_PTR);
}

/* On the player's turn the blinking cell is the attacker's, at
 * CURRENT_COL and CURRENT_ROW, drawn with the sprite of what MOB_PTR
 * points at. Otherwise it is MOB_PTR's own cell and sprite. */
void set_blink_target(void) {
    if (IS_PLAYER_TURN == 1) {
        BLINK_COL = CURRENT_COL;
        BLINK_ROW = CURRENT_ROW;
        BLINK_CHAR = get_entity_fontchar((entity_ref)MOB_PTR);
    } else {
        uint16_t colrow = pos_to_colrow(MOB_PTR->pos);
        BLINK_COL = (uint8_t)colrow;
        BLINK_ROW = (uint8_t)(colrow >> 8);
        BLINK_CHAR = (uint8_t)appearance_to_fontchar(MOB_PTR->appearance);
    }
}

void load_char_to_fa(uint8_t index) {
    CHAR_PTR = get_char_record(index);
}

void render_font_char(void) {
    font_pos_to_text_pos();
    print_fontchar_at_text_pos();
}

void print_fontchar_at_text_pos(void) {
    uint8_t selector = FONT_CHARSET == 2 ? FONT_SELECTOR_1 + 2 : FONT_SELECTOR_1;

    s_PRINT_FONT_CHAR.charset = selector;
    render_font_char_direct(selector);
}

/* The selector is that of the font before FONT_CHARNUM's, and
 * FONT_CHARNUM's own is written only when FONT_CHARNUM is 24 or more,
 * as in the original. */
void render_font_char_direct(uint8_t charset_sel) {
    if (FONT_CHARNUM >= SPRITES_PER_FONT)
        s_PRINT_FONT_CHAR.charset =
            charset_sel + FONT_CHARNUM / SPRITES_PER_FONT;
    uint8_t glyph = FONT_CHARNUM % SPRITES_PER_FONT * 4 + FIRST_SPRITE_GLYPH;
    s_PRINT_FONT_CHAR.upper_left = glyph;
    s_PRINT_FONT_CHAR.upper_right = glyph + 1;
    s_PRINT_FONT_CHAR.lower_left = glyph + 2;
    s_PRINT_FONT_CHAR.lower_right = glyph + 3;
    print_string((const uint8_t *)&s_PRINT_FONT_CHAR);
}

/* Draws the blinking cell, unless BLINK_COL turns the blink off. */
static void draw_blink_char(void) {
    if (BLINK_COL >= MAP_WIDTH)
        return;
    FONT_COL = BLINK_COL;
    FONT_ROW = BLINK_ROW;
    FONT_CHARNUM = BLINK_CHAR;
    FONT_CHARSET = 1;
    render_font_char();
}

/* Draws the blinking cell with the location's empty-cell sprite, the
 * blink's "off" phase. BLINK_CHAR is written and put back, as in the
 * original, which keeps it on the stack meanwhile. */
void draw_blink_alt(void) {
    uint8_t sprite = BLINK_CHAR;

    BLINK_CHAR = BLINK_ALT_CHAR;
    draw_blink_char();
    BLINK_CHAR = sprite;
}

void draw_blink_normal(void) {
    print_ctrl_n();
    draw_blink_char();
}

/* A map cell is 2 x 2 text cells. */
void font_pos_to_text_pos(void) {
    TEXT_COL = FONT_COL * 2;
    TEXT_ROW = FONT_ROW * 2;
}
