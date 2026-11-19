/*
    util.c -- the game's clock, the random numbers, positions and record
    lookups, the character browser of the "ADD" menu, the integrity
    check of the disk reader, and the restart that RESET runs.

    The game's unit of time is one call of the Monitor's WAIT with $50.
    A tick of the clock is WAIT_LOOP_COUNT such calls, a number that the
    game speed (ANIM_INDEX) picks, and the cell that blinks on the map
    changes between the mob's sprite and the location's tile after a
    number of ticks that the speed also picks. Every wait in the game
    goes through these routines, so the blink runs while the game
    waits.

    Departures from the original:

    - DISPATCH_ON_MODE compares PLAY_HIGH_TONE's first byte with $60,
      the RTS that TOGGLE_SOUND writes there to turn the sound off.
      dispatch_on_mode tests sound.c's SOUND_ON, which toggle_sound
      flips, because the C's code isn't at the original's addresses.
    - SAVE_ZP_CONTEXT and RESTORE_ZP_CONTEXT aren't translated. Their
      only caller was SCENE_LOOP, and map.c's scene_loop saves and
      restores ENTITY_PTR and MOB_PTR itself. The other two pointers
      they saved, $BC and $BE, are locals and parameters in C.
    - FIND_PREV_PC keeps its two indexes in locals, where the original
      uses INPUT_BUFFER[0] and [1]. Its caller goes on to
      ENTER_CHAR_SELECT, whose SETUP_INPUT clears the buffer before
      anything reads it.
    - VERIFY_DISK_IO checksums 34 bytes of the original's own
      LOAD_SCENE_DATA, which nothing writes, so it always passes.
      verify_disk_io does nothing.
    - VERIFY_INTEGRITY compares the checksum with the two bytes at
      INTEGRITY_DATA. verify_integrity compares it with a constant, so
      INTEGRITY_DATA isn't kept, and section .rodata.integrity is
      empty. On a mismatch the original jumps to INTEGRITY_FAIL_ADDR,
      which returns to VERIFY_INTEGRITY's caller, and the C calls
      integrity_fail() and returns.
    - The original's scratch bytes are locals: RNG_LIMIT ($5A11),
      RNG_MASK ($5A12), PRNG_TEMP ($5A1A), the checksum's pointers in
      $BA and $BE, CKSUM_ACC ($A527, two bytes of the code), and the
      $BA, $BC, and $BE that the other routines use.
    - CALL_ROM_WAIT is WAIT(WAIT_DURATION) where the original calls it.
    - A COLROW_TO_POS whose sum passes 255 falls into NUM_TO_DECIMAL in
      the original, and colrow_to_pos wraps instead. No caller passes
      such a column and row.
    - RANDOM_IN_RANGE's entries at +42 and +48, which only the dead
      ORPHANED_ENCOUNTER_HANDLER calls, aren't separate entries.
    - LOAD_END_OFFSET, ADVANCE_CKSUM_START, and ADVANCE_RANGE_CTR at
      $A590-$A5BB have no callers and aren't translated.
    - FIND_FIRST_PC returns its result in the carry, clear when it found
      a player character, and find_first_pc returns true then.
      POS_TO_COLROW returns the column in A and the row in Y, and
      pos_to_colrow returns them as the low and high bytes of a word.
    - The tables at $7AAD-$7ABF are statics here. WAIT_DURATION and
      DELAY_COUNT are constants, since nothing writes them.
    - DEC_WAIT_IF_NONZERO isn't translated. idle_anim_handler subtracts
      its 12 WAIT calls from WAIT_LOOP_COUNT at once, stopping at 0,
      which leaves the byte as the 12 calls of the original do.
    - COLD_RESTART fills each page from offset 0 and then from $FF
      down, and the tail from $A56F down. cold_restart fills both from
      the lowest address up, which stores the same bytes.

    Ali Baba, main.asm lines 1724-1941, 2057-2124, 2255-2386,
    2406-2433, 2862-2885, 3347-3433, 5387-5400, 14896-14947,
    15315-15342 (the variables util uses), and 17143-17287, with the
    lines that hold only a comment left out:

        ORG     $0A93
    CLAMP_CHAR_FIELD:
        SUBROUTINE CLAMP_CHAR_FIELD
        LDY     #MFIELD_STRENGTH           ; byte 5: strength
        LDA     (MOB_PTR),Y                ; load strength byte
        AND     #STRENGTH_MASK             ; bits 4-0 = minimum HP
        STA     MOB_DATA_PTR               ; save as floor value
        INY                                ; Y = MFIELD_HP (byte 6)
        LDA     (MOB_PTR),Y                ; load HP byte
        AND     #HP_MASK                   ; bits 5-0 = current HP
        BALT    MOB_DATA_PTR,_CLAMP_CHAR_FIELD.clamp        ; HP < floor -> clamp
        RTS                                ; HP >= floor -> done
    _CLAMP_CHAR_FIELD.clamp:
        LDA     (MOB_PTR),Y                ; reload HP byte
        AND     #HP_FLAGS_MASK             ; preserve flag bits 7-6
        ORA     MOB_DATA_PTR               ; set HP to floor value
        STA     (MOB_PTR),Y                ; store clamped HP
        RTS
        ORG     $0AAE
    FIND_FIRST_PC:
        SUBROUTINE FIND_FIRST_PC
        LDA     CHAR_LIST_HEAD             ; head of character linked list
    .check:
        CMP     #$00                       ; end of list?
        BNE     .have_entry                ; no -> examine this entry
        SEC                                ; yes -> not found
        RTS
    .have_entry:
        STA     BROWSE_CHAR_INDEX          ; save current mob index
        JSR     GET_MOB_DATA               ; -> MOB_DATA_PTR = mob record
        LDY     #MFIELD_APPEARANCE         ; byte 4: appearance
        LDA     (MOB_DATA_PTR),Y           ; load appearance
        BMI     .skip                      ; bit 7 set -> not a PC
        CMP     #NPC_APPEARANCE_MIN        ; < $15?
        BMI     COPY_PC_NAME_TO_BUF        ; yes -> player character
    .skip:
        LDY     #MFIELD_NEXT               ; byte 2: next link
        LDA     (MOB_DATA_PTR),Y           ; follow chain
        JMP     .check                     ; check next entry
        ORG     $0ACE
    COPY_PC_NAME_TO_BUF:
        LDA     MOB_DATA_PTR               ; \
        STA     MOB_PTR                    ;  | MOB_PTR = MOB_DATA_PTR
        LDA     MOB_DATA_PTR+1             ;  |
        STA     MOB_PTR+1                  ; /
        LDY     #MFIELD_NAME            ; byte 0: name ptr low
        LDA     (MOB_PTR),Y                ; load name ptr low
        STA     TARGET_REC                 ; \
        INY                                ;  | TARGET_REC = name string
        LDA     (MOB_PTR),Y                ;  | (Y = MFIELD_NAME+1)
        STA     TARGET_REC+1               ; /
        LDA     #<CHAR_SELECT_NAME         ; \
        STA     MOB_DATA_PTR               ;  | MOB_DATA_PTR = display buf
        LDA     #>CHAR_SELECT_NAME         ;  |
        STA     MOB_DATA_PTR+1             ; /
        LDY     #$00                       ; string offset 0
        LDA     (TARGET_REC),Y             ; byte 0 = name length
        TAX                                ; X = chars remaining
    .copy_loop:
        INY                                ; advance to next char
        LDA     (TARGET_REC),Y             ; load name character
        DEX                                ; decrement remaining count
        BPL     .store                     ; count >= 0 -> keep char
        LDA     #$20                       ; else pad with space
    .store:
        STA     (MOB_DATA_PTR),Y           ; store into display buffer
        CPY     #NAME_BUF_LEN              ; copied 20 chars?
        BNE     .copy_loop                 ; no -> continue
        CLC                                ; success
        RTS
        ORG     $0AFE
    FIND_NEXT_PC_WRAP:
        JSR     FIND_NEXT_PC               ; find next PC in list
        RTS
        ORG     $0B02
    FIND_NEXT_PC:
        LDY     #MFIELD_NEXT               ; byte 2: next link
        LDA     (MOB_PTR),Y                ; load next mob index
        BNE     .have_link                 ; nonzero -> follow it
        JSR     FIND_FIRST_PC              ; zero -> wrap to first PC
        RTS
    .have_link:
        STA     BROWSE_CHAR_INDEX          ; save mob index
        JSR     GET_MOB_DATA               ; -> MOB_DATA_PTR = mob record
        LDA     MOB_DATA_PTR               ; \
        STA     MOB_PTR                    ;  | MOB_PTR = MOB_DATA_PTR
        LDA     MOB_DATA_PTR+1             ;  |
        STA     MOB_PTR+1                  ; /
        LDY     #MFIELD_APPEARANCE         ; byte 4: appearance
        LDA     (MOB_PTR),Y                ; load appearance
        BMI     FIND_NEXT_PC               ; bit 7 set -> skip
        CMP     #NPC_APPEARANCE_MIN        ; < $15?
        BPL     FIND_NEXT_PC               ; no -> skip, try next
        JSR     COPY_PC_NAME_TO_BUF        ; yes -> copy name
        RTS
        ORG     $0B28
    FIND_PREV_PC:
        LDA     BROWSE_CHAR_INDEX          ; \
        STA     INPUT_BUFFER               ; / save current index
        JSR     FIND_FIRST_PC              ; start from first PC
    .scan:
        LDA     BROWSE_CHAR_INDEX          ; \
        STA     INPUT_BUFFER+1             ; / save as candidate predecessor
        JSR     FIND_NEXT_PC               ; advance to next PC
        LDA     BROWSE_CHAR_INDEX          ; wrapped back to original?
        CMP     INPUT_BUFFER               ; compare with saved index
        BNE     .scan                      ; no -> keep scanning
        LDA     INPUT_BUFFER+1             ; yes -> predecessor index
        STA     BROWSE_CHAR_INDEX          ; set as current
        JSR     GET_MOB_DATA               ; resolve to record pointer
        JSR     COPY_PC_NAME_TO_BUF        ; copy name to buffer
        RTS
        ORG     $0B4F
    INIT_CHAR_SELECT:
        STOB    #$01,SCENE_EVENT_RESULT    ; mark event handled
        JSR     SCENE_SETUP                ; set up scene boundaries
        JSR     FIND_FIRST_PC              ; find first player character
        BCC     .found                     ; carry clear -> found
        JMP     MOVE_LOOP                  ; carry set -> no PCs
    .found:
        JMP     ENTER_CHAR_SELECT          ; -> character selection menu
    ...

        ORG     $0C25
    POS_TO_COLROW:
        SUBROUTINE POS_TO_COLROW
        LDY     #$00                       ; row counter = 0
    .loop:
        CLC
        ADC     #$EC                       ; A -= MAP_WIDTH ($EC = -20)
        BCC     .done                      ; underflow -> went past 0
        INY                                ; row++
        BCS     .loop                      ; always taken (C=1)
    .done:
        ADC     #MAP_WIDTH                 ; add 20 back -> remainder
        RTS
        ORG     $0C32
    GET_MOB_DATA:
        SUBROUTINE GET_MOB_DATA
        CLC
        ADC     #$FF                       ; A = index - 1
        ROL2                               ; rotate left 4 times
        ROL2                               ;   = multiply by 16 (with carry)
        TAY                                ; save intermediate
        ROL                                ; one more rotate
        AND     #LOW_NIBBLE_MASK           ; high nybble -> page offset
        CLC
        ADC     MOB_BASE_HI                ; add base page
        STA     MOB_DATA_PTR+1             ; store high byte
        TYA
        AND     #HIGH_NIBBLE_MASK          ; low nybble -> byte offset
        CLC
        ADC     MOB_BASE_LO                ; add base offset
        STA     MOB_DATA_PTR               ; store low byte
        BCC     .done
        INC     MOB_DATA_PTR+1             ; propagate carry
    .done:
        RTS
    ...

        ORG     $0D10
    DELAY_WITH_ANIMATION:
        SUBROUTINE DELAY_WITH_ANIMATION
        LDA     DELAY_COUNT        ; load iteration count
    .loop:
        PHA                        ; save counter
        JSR     ANIM_TICK_AND_WAIT ; tick animation + timed wait
        PLA                        ; restore counter
        TAY
        DEY                        ; decrement
        TYA
        BNE     .loop              ; loop until zero
        RTS
        ORG     $0D1E
    TICK_ANIM_COUNTER:
        SUBROUTINE TICK_ANIM_COUNTER
        DEC     ANIM_FRAME_CTR    ; tick frame counter
        BNE     .reload           ; not zero -> just reload wait count
        JSR     DRAW_BLINK_ALT    ; counter hit zero -> draw alt char
    .reload:
        LDX     ANIM_INDEX        ; current animation index
        LDA     ANIM_WAIT_TABLE,X ; look up wait count for this frame
        STA     WAIT_LOOP_COUNT   ; store for timed_wait to use
        RTS
        ORG     $0D30
    ANIM_TICK_AND_WAIT:
        SUBROUTINE ANIM_TICK_AND_WAIT
        JSR     TICK_ANIM_COUNTER  ; tick counter, reload wait count
        JSR     TIMED_WAIT         ; burn time
    ANIM_CHECK_FRAME:
        LDA     ANIM_FRAME_CTR     ; did frame counter expire?
        BEQ     .advance           ; yes -> advance animation
        RTS                        ; no -> return
    .advance:
        JSR     DRAW_BLINK_NORMAL  ; redraw blink char in normal video
        LDX     ANIM_INDEX         ; reload animation index
        LDA     ANIM_FRAME_TABLE,X ; look up frame duration
        STA     ANIM_FRAME_CTR     ; reset frame counter
        RTS
        ORG     $0D49
    TIMED_WAIT:
        SUBROUTINE TIMED_WAIT
        LDA     WAIT_LOOP_COUNT ; anything to wait for?
        BNE     .wait           ; yes -> enter wait loop
        RTS                     ; no -> return immediately
    .wait:
        JSR     CALL_ROM_WAIT   ; delay one unit
        DEC     WAIT_LOOP_COUNT ; decrement counter
        BNE     .wait           ; loop until zero
        RTS
        ORG     $0D58
    CALL_ROM_WAIT:
        SUBROUTINE CALL_ROM_WAIT
        LDA     WAIT_DURATION ; load duration parameter
        JMP     ROM_WAIT      ; tail-call ROM WAIT
        ORG     $0D5E
    IDLE_ANIM_HANDLER:
        SUBROUTINE IDLE_ANIM_HANDLER
        LDA     WAIT_LOOP_COUNT        ; anything to wait for?
        BEQ     .expired               ; zero -> frame check
        LDA     INPUT_MODE
        BNE     .joystick              ; nonzero -> joystick mode
        JSR     CALL_ROM_WAIT          ; burn time
        DEC     WAIT_LOOP_COUNT        ; count down
        RTS
    .expired:
        JSR     ANIM_CHECK_FRAME       ; check/advance animation frame
        JSR     TICK_ANIM_COUNTER      ; tick counter, reload wait
        JMP     IDLE_ANIM_HANDLER      ; loop
    .joystick:
        LDX     #$0C                   ; 12 iterations
    .joy_loop:
        JSR     DEC_WAIT_IF_NONZERO    ; decrement if nonzero
        DEX
        BNE     .joy_loop
        RTS
    DEC_WAIT_IF_NONZERO:
        LDA     WAIT_LOOP_COUNT
        BEQ     .done
        DEC     WAIT_LOOP_COUNT
    .done:
        RTS
    ...

        ORG     $0D98
    COLROW_TO_POS:
        SUBROUTINE COLROW_TO_POS
        CLC
    .loop:
        DEY                                ; row--
        BPL     .add                       ; row >= 0 -> add another 20
        RTS                                ; Y exhausted, A = result
    .add:
        ADC     #MAP_WIDTH                 ; A += 20
        BCC     .loop                      ; no overflow -> continue
    ...

        ORG     $1053
    GET_CHAR_RECORD:
        SUBROUTINE GET_CHAR_RECORD
        TAX                                 ; X = index (loop counter)
        STOW    CHAR_TABLE_BASE,CHAR_REC    ; CHAR_REC = $4000 (base)
    .loop:
        DEX
        BPL     .advance                    ; records to skip -> advance
        RTS                                 ; index exhausted: CHAR_REC ready
    .advance:
        CLC                                 ; \
        LDA     CHAR_REC                    ;  | CHAR_REC += CHAR_RECORD_SIZE
        ADC     #CHAR_RECORD_SIZE           ;  | (9 bytes per record)
        STA     CHAR_REC                    ;  |
        LDA     CHAR_REC+1                  ;  |
        ADC     #$00                        ;  | propagate carry
        STA     CHAR_REC+1                  ; /
        BNE     .loop                       ; always taken: high byte >= $40
    ...

        ORG     $1317
    RANDOM_IN_RANGE:
        SUBROUTINE RANDOM_IN_RANGE
        STA     RNG_LIMIT                  ; save range limit
        BANE    #$00,_RANDOM_IN_RANGE.nonzero
        RTS                                ; range=0 -> return 0
    _RANDOM_IN_RANGE.nonzero:
        BAPL    #$02,_RANDOM_IN_RANGE.ge2
        LDY     #$06                       ;  | range 1: mask=$40, shift=6
        LDA     #$40                       ;  |
        BNE     .have_mask                 ; /
    _RANDOM_IN_RANGE.ge2:
        BAPL    #$04,_RANDOM_IN_RANGE.ge4
        LDY     #$05                       ;  | range 2-3: mask=$60, shift=5
        LDA     #$60                       ;  |
        BNE     .have_mask                 ; /
    _RANDOM_IN_RANGE.ge4:
        BAPL    #$08,_RANDOM_IN_RANGE.ge8
        LDY     #$04                       ;  | range 4-7: mask=$70, shift=4
        LDA     #$70                       ;  |
        BNE     .have_mask                 ; /
    _RANDOM_IN_RANGE.ge8:
        BAPL    #$10,_RANDOM_IN_RANGE.ge16
        LDY     #$03                       ;  | range 8-15: mask=$78, shift=3
        LDA     #$78                       ;  |
        BNE     .have_mask                 ; /
    _RANDOM_IN_RANGE.ge16:
        BAPL    #$20,_RANDOM_IN_RANGE.ge32
        LDY     #$02                       ;  | range 16-31: mask=$7C, shift=2
        LDA     #$7C                       ;  |
        BNE     .have_mask                 ; /
    _RANDOM_IN_RANGE.ge32:
        BAPL    #$40,_RANDOM_IN_RANGE.ge64
        LDY     #$01                       ;  | range 32-63: mask=$7E, shift=1
        LDA     #$7E                       ;  |
        BNE     .have_mask                 ; /
    _RANDOM_IN_RANGE.ge64:
        LDY     #$00                       ; range 64-127: mask=$7F, shift=0
        LDA     #$7F
    .have_mask:
        STA     RNG_MASK                   ; save bit mask
        LDA     RNG_LIMIT                  ; reload range limit
        STY     RNG_LIMIT                  ; save shift count in RNG_LIMIT
    .shift_loop:
        DEY                                ; \
        BMI     .shifted                   ;  | shift limit left by (shift count)
        CLC                                ;  | positions so it aligns with the
        ROL                                ;  | mask bits
        BNE     .shift_loop                ; /
    .shifted:
        LDY     RNG_LIMIT                  ; Y = shift count
        STA     RNG_LIMIT                  ; save shifted limit
    .sample:
        JSR     STEP_PRNG                  ; get next random byte
        LDA     PRNG_OUTPUT                ; load masked PRNG output
        AND     RNG_MASK                   ; mask to relevant bits
        BAEQ    RNG_LIMIT,_RANDOM_IN_RANGE.accept          ; equal -> accept
        BPL     .sample                    ; greater -> reject, retry
    _RANDOM_IN_RANGE.accept:
        DEY                                ; \
        BPL     .shift_right               ;  | shift result right by (shift count)
        RTS                                ;  | to recover the final value
    .shift_right:
        CLC                                ;  |
        ROR                                ;  |
        BCC     _RANDOM_IN_RANGE.accept                    ; / (always taken: masked bit is 0)
    ...

        ORG     $1F0C
        SUBROUTINE DISPATCH_ON_MODE
    DISPATCH_ON_MODE:
        LDA     PLAY_HIGH_TONE
        BAEQ    #$60,_DISPATCH_ON_MODE.mode_60
        JMP     ANIM_TICK_AND_WAIT
    _DISPATCH_ON_MODE.mode_60:
        JMP     DELAY_WITH_ANIMATION
    ...

        ORG     $78A8
    STEP_PRNG:
        SUBROUTINE STEP_PRNG
        LDA     PRNG_STATE                 ; val
        ASL                                ; val*2
        ASL                                ; val*4
        STA     PRNG_TEMP                  ; temp = val*4
        ASL                                ; val*8
        ASL                                ; val*16
        ASL                                ; val*32
        ASL                                ; val*64
        CLC
        ADC     PRNG_TEMP                  ; val*64 + val*4 = val*68
        CLC
        ADC     PRNG_STATE                 ; val*68 + val = val*69
        CLC
        ADC     #$53                       ; + $53
        STA     PRNG_STATE                 ; store full result
        LSR                                ; val >> 1
        STA     PRNG_OUTPUT                ; shifted copy for random tests
        RTS
    ...

        ORG     $7AAD
    WAIT_DURATION:
        DC.B    $50                     ; ROM WAIT duration parameter
    ...

        ORG     $7AAF
    ANIM_FRAME_CTR:
        DC.B    $03                     ; animation frame countdown
    ANIM_WAIT_TABLE:
        HEX     00 32 23 17 0C 05      ; per-frame wait counts (6 entries)
        ORG     $7AB6
    ANIM_FRAME_TABLE:
        HEX     00 02 02 04 09 13      ; per-frame frame counts (6 entries)
    ...

        ORG     $7ABD
    ANIM_INDEX:
        DC.B    $02                     ; current game speed / animation index
    ...

        ORG     $7ABF
    DELAY_COUNT:
        DC.B    $04                     ; delay iterations
    ...

        ORG     $A4E2
    INTEGRITY_DATA:
        HEX     AE 7A 4F 87        ; expected checksums (2 pairs)
    VERIFY_INTEGRITY:
        SUBROUTINE VERIFY_INTEGRITY
        LDA     #<DISK_IO_START    ; \ range: DISK_IO_START to DISK_IO_END
        STA     CKSUM_START        ;  |
        LDA     #>DISK_IO_START    ;  |
        STA     CKSUM_START+1      ;  |
        LDA     #<DISK_IO_END      ;  |
        STA     CKSUM_END          ;  |
        LDA     #>DISK_IO_END      ;  |
        STA     CKSUM_END+1        ; /
        JSR     COMPUTE_CHECKSUM
        CMP     INTEGRITY_DATA     ; compare low byte
        BNE     .fail
        TYA
        CMP     INTEGRITY_DATA+1   ; compare high byte
        BNE     .fail
        RTS                        ; checksums match
    .fail:
        JMP     INTEGRITY_FAIL_ADDR ; reinitialize on mismatch
    VERIFY_DISK_IO:
        LDA     #<SCENE_LOADER_START ; \ range: SCENE_LOADER_START to SCENE_LOADER_END
        STA     CKSUM_START          ;  |
        LDA     #>SCENE_LOADER_START ;  |
        STA     CKSUM_START+1        ;  |
        LDA     #<SCENE_LOADER_END ;  |
        STA     CKSUM_END          ;  |
        LDA     #>SCENE_LOADER_END ;  |
        STA     CKSUM_END+1          ; /
        JSR     COMPUTE_CHECKSUM
        CMP     INTEGRITY_DATA+2   ; compare low byte
        BNE     .fail
        TYA
        CMP     INTEGRITY_DATA+3   ; compare high byte
        BNE     .fail
        RTS
    CKSUM_ACC:
        DC.B    $B1                ; checksum accumulator low (self-modifying)
        DC.B    $A0                ; checksum accumulator high (+1)
    COMPUTE_CHECKSUM:
        SUBROUTINE COMPUTE_CHECKSUM
        CLC
        LDY     #$00
        STY     CKSUM_ACC       ; checksum low = 0
        STY     CKSUM_ACC+1       ; checksum high = 0
    .loop:
        LDA     (CKSUM_START),Y
        ADC     CKSUM_ACC
        STA     CKSUM_ACC
        JSR     .advance           ; advance pointer
        BCS     .done              ; reached end -> return
        LDA     (CKSUM_START),Y
        ADC     CKSUM_ACC+1
        STA     CKSUM_ACC+1
        EOR     CKSUM_ACC
        STA     CKSUM_ACC
        JSR     .advance
        BCC     .loop              ; not done -> continue
    .done:
        LDA     CKSUM_ACC       ; return checksum low in A
        LDY     CKSUM_ACC+1       ; return checksum high in Y
        RTS
    .advance:
        INC     CKSUM_START
        BNE     .no_carry
        INC     CKSUM_START+1
    .no_carry:
        LDA     CKSUM_START
        CMP     CKSUM_END
        BNE     .not_end
        LDA     CKSUM_START+1
        CMP     CKSUM_END+1
        BNE     .not_end
        SEC                        ; reached end of range
        RTS
    .not_end:
        CLC
        RTS
        ORG     $A56F
    COLD_RESTART:
        SUBROUTINE COLD_RESTART
        LDA     #<MAIN_ENTRY       ; \ CLEAR_PTR = $0800
        STA     CLEAR_PTR          ;  |
        LDA     #>MAIN_ENTRY       ;  |
        STA     CLEAR_PTR+1        ; /
        LDY     #$00
    .clear:
        STA     (CLEAR_PTR),Y
        DEY
        BNE     .clear
        INC     CLEAR_PTR+1
        LDA     CLEAR_PTR+1
        CMP     #WIPE_END_PAGE     ; stop at page $A5
        BNE     .clear
        LDY     #WIPE_TAIL_LEN
    .clear_tail:
        STA     (CLEAR_PTR),Y      ; clear $A500-$A56E
        DEY
        BNE     .clear_tail
        JMP     DISK_BOOT_ROM      ; reboot from Disk II
*/
#include "ali_baba.h"

/* ---- The game's clock ---------------------------------------------------- */

/* WAIT's argument for one unit of the game's time. WAIT($50) takes
 * (26 + 27 * 80 + 5 * 80 * 80) / 2 = 17,093 cycles. */
constexpr uint8_t WAIT_DURATION = 0x50;

/* The number of anim_tick_and_wait calls in delay_with_animation. */
constexpr uint8_t DELAY_COUNT = 4;

/* The game speed, 1-5, which input's settings change. It indexes the
 * two tables below, whose entry 0 is never used. */
uint8_t ANIM_INDEX = 2;

/* Counts down the ticks until the blinking cell changes between its
 * two glyphs. */
static uint8_t ANIM_FRAME_CTR = 3;

/* The WAIT calls between ticks, for each game speed. */
static const uint8_t ANIM_WAIT_TABLE[6] = { 0x00, 0x32, 0x23, 0x17, 0x0C, 0x05 };

/* The ticks the blinking cell spends in each glyph, for each game
 * speed. */
static const uint8_t ANIM_FRAME_TABLE[6] = { 0x00, 0x02, 0x02, 0x04, 0x09, 0x13 };

/* Counts one tick of the blink. On the tick that ends the sprite's
 * time, draws the location's tile in the blinking cell. Then sets
 * WAIT_LOOP_COUNT to the speed's number of WAIT calls per tick. */
void tick_anim_counter(void) {
    if (--ANIM_FRAME_CTR == 0)
        draw_blink_alt();
    WAIT_LOOP_COUNT = ANIM_WAIT_TABLE[ANIM_INDEX];
}

void timed_wait(void) {
    while (WAIT_LOOP_COUNT != 0) {
        WAIT(WAIT_DURATION);
        WAIT_LOOP_COUNT--;
    }
}

/* Spends one tick, then does anim_check_frame. */
uint8_t anim_tick_and_wait(void) {
    tick_anim_counter();
    timed_wait();
    return anim_check_frame();
}

/* Draws the sprite back in the blinking cell once the tile's ticks have
 * run out, and starts the count again. */
uint8_t anim_check_frame(void) {
    if (ANIM_FRAME_CTR == 0) {
        draw_blink_normal();
        ANIM_FRAME_CTR = ANIM_FRAME_TABLE[ANIM_INDEX];
    }
    return ANIM_FRAME_CTR;
}

void delay_with_animation(void) {
    for (uint8_t n = 0; n < DELAY_COUNT; n++)
        anim_tick_and_wait();
}

/* Waits four ticks with the sound off and one with it on. The original
 * tests whether TOGGLE_SOUND has written an RTS ($60) over
 * PLAY_HIGH_TONE's first byte. */
void dispatch_on_mode(void) {
    if (SOUND_ON)
        anim_tick_and_wait();
    else
        delay_with_animation();
}

/* The WAIT calls that one idle poll in joystick mode counts off. */
constexpr uint8_t JOYSTICK_POLL_WAITS = 12;

/* READ_KEYBOARD calls this once for each poll that finds no input. When
 * a tick's WAIT calls have all been spent, it starts the next tick. With
 * the keyboard it then makes one WAIT call, and with the joystick it
 * counts up to 12 of them off without waiting. The original counts them
 * off one at a time with DEC_WAIT_IF_NONZERO. */
void idle_anim_handler(void) {
    while (WAIT_LOOP_COUNT == 0) {
        anim_check_frame();
        tick_anim_counter();
    }
    if (INPUT_MODE == 0) {
        WAIT(WAIT_DURATION);
        WAIT_LOOP_COUNT--;
    } else if (WAIT_LOOP_COUNT > JOYSTICK_POLL_WAITS) {
        WAIT_LOOP_COUNT -= JOYSTICK_POLL_WAITS;
    } else {
        WAIT_LOOP_COUNT = 0;
    }
}

/* ---- Positions and records ------------------------------------------------ */

/* The original divides by repeated subtraction. */
uint16_t pos_to_colrow(uint8_t pos) {
    uint8_t col = pos % MAP_WIDTH;
    uint8_t row = pos / MAP_WIDTH;
    return col | (uint16_t)row << 8;
}

/* A sum past 255 wraps here. The original falls into NUM_TO_DECIMAL
 * instead, which no caller's arguments reach, since every row is 12 or
 * less and every column 19 or less. */
uint8_t colrow_to_pos(uint8_t col, uint8_t row) {
    return col + MAP_WIDTH * row;
}

/* The base is the word at $4000, MOB_BASE in the original, which is
 * $4C80, MOBS. An index of 0 is record 255, since the index - 1 is a
 * byte in the original. */
struct mob *get_mob_data(uint8_t index) {
    uint8_t n = index - 1;
    return ((struct location_header *)LOCATIONS)->mob_base + n;
}

/* An index of 129 or more gives LOCATIONS[0], as in the original, whose
 * loop counts the index down with DEX and BPL. */
struct location *get_char_record(uint8_t index) {
    if (index > 128)
        return LOCATIONS;
    return LOCATIONS + index;
}

/* The life of a mob in MOB_PTR's record is raised to its strength, with
 * life's two top bits kept. The original's header comment describes caps
 * at 31 and 63, which its code doesn't apply. */
void clamp_char_field(void) {
    uint8_t floor = MOB_PTR->strength & STRENGTH_MASK;
    if ((MOB_PTR->life & HP_MASK) < floor)
        MOB_PTR->life = (MOB_PTR->life & HP_FLAGS_MASK) | floor;
}

/* ---- Choosing a character to add -------------------------------------------- */

/* The length of the name in S_ADD_NAME.name. */
constexpr uint8_t NAME_BUF_LEN = 20;

/* Points MOB_PTR at `m` and writes its name into S_ADD_NAME.name[1..20],
 * padded with $20, a low-ASCII space. Character i is copied when the
 * length minus i is 0 or more as a signed byte, which is the original's
 * DEX and BPL. That differs from i <= length only for a name of 128
 * characters or more. */
static void copy_pc_name_to_buf(struct mob *m) {
    MOB_PTR = m;
    const uint8_t *name = m->name;
    for (uint8_t i = 1; i <= NAME_BUF_LEN; i++)
        S_ADD_NAME.name[i] = (int8_t)(name[0] - i) >= 0 ? name[i] : ' ';
}

/* A mob is a player character when its appearance is below $15. */
static bool is_player_character(const struct mob *m) {
    return m->appearance < NPC_APPEARANCE_MIN;
}

/* Walks the list of mobs at no location from its head, leaving
 * BROWSE_CHAR_INDEX at each mob it looks at. */
bool find_first_pc(void) {
    uint8_t index = ((struct location_header *)LOCATIONS)->char_list_head;
    while (index != 0) {
        BROWSE_CHAR_INDEX = index;
        struct mob *m = get_mob_data(index);
        if (is_player_character(m)) {
            copy_pc_name_to_buf(m);
            return true;
        }
        index = m->next;
    }
    return false;
}

/* Follows the list from MOB_PTR to the next player character, pointing
 * MOB_PTR at each mob on the way, and starts again from the head at the
 * end of the list. */
static void find_next_pc(void) {
    for (;;) {
        uint8_t index = MOB_PTR->next;
        if (index == 0) {
            find_first_pc();
            return;
        }
        BROWSE_CHAR_INDEX = index;
        MOB_PTR = get_mob_data(index);
        if (is_player_character(MOB_PTR)) {
            copy_pc_name_to_buf(MOB_PTR);
            return;
        }
    }
}

void find_next_pc_wrap(void) {
    find_next_pc();
}

/* Walks the player characters from the first until it comes back to
 * BROWSE_CHAR_INDEX, and takes the one it saw before that. The original
 * keeps `start` and `prev` in INPUT_BUFFER[0] and [1], which SETUP_INPUT
 * clears before the menu draws them. If the starting mob is no longer a
 * player character, the walk never comes back to it, and the loop never
 * ends, as in the original. */
void find_prev_pc(void) {
    uint8_t start = BROWSE_CHAR_INDEX;
    uint8_t prev;
    find_first_pc();
    do {
        prev = BROWSE_CHAR_INDEX;
        find_next_pc();
    } while (BROWSE_CHAR_INDEX != start);
    BROWSE_CHAR_INDEX = prev;
    copy_pc_name_to_buf(get_mob_data(prev));
}

/* Moves the character MOB_PTR shows into location ACTIVE_CHAR, then
 * shows the next one, or goes back to the turn when none is left. */
uint8_t init_char_select(void) {
    SCENE_EVENT_RESULT = 1;
    scene_setup();
    if (find_first_pc())
        return CHAIN_ENTER_CHAR_SELECT;
    return CHAIN_MOVE_LOOP;
}

/* ---- Random numbers -------------------------------------------------------- */

/* PRNG_OUTPUT is the new state shifted right once, so it runs from 0 to
 * $7F. */
uint8_t step_prng(void) {
    PRNG_STATE = 69 * PRNG_STATE + 0x53;
    PRNG_OUTPUT = PRNG_STATE >> 1;
    return PRNG_OUTPUT;
}

/*
    Picks the number by rejection sampling. The mask keeps the fewest
    top bits of the 7-bit PRNG_OUTPUT that can hold `limit`, and `limit`
    is shifted up to line up with them. A sample is taken when it is at
    most the shifted limit, so the result runs from 0 to `limit`
    inclusive, although the original's comments say limit - 1.

        limit       mask    shift
        1           $40     6
        2-3         $60     5
        4-7         $70     4
        8-15        $78     3
        16-31       $7C     2
        32-63       $7E     1
        64-129      $7F     0
        130-255     $40     6

    The original chooses the mask with a chain of CMP #2^k and BPL, which
    tests bit 7 of limit - 2^k. A limit of 130 or more fails the first
    test, so it gets the mask of a limit of 1. The original's test of a
    sample is CMP, BEQ, and BPL, which takes a sample when sample -
    scaled is 0 or has bit 7 set. That is sample <= scaled for every
    limit below 130, and the test here reads the sign of the difference
    as the original does, because the two differ for the larger limits.
*/
uint8_t random_in_range(uint8_t limit) {
    if (limit == 0)
        return 0;

    uint8_t shift = 0;
    if (limit >= 130)
        shift = 6;
    else
        while (limit << shift < 0x40)
            shift++;
    uint8_t mask = (0x7F >> shift) << shift;
    uint8_t scaled = limit << shift;

    uint8_t sample;
    do
        sample = step_prng() & mask;
    while ((int8_t)(sample - scaled) > 0);
    return sample >> shift;
}

/* ---- The integrity checks ---------------------------------------------------- */

/* The checksum of the disk reader at $B300-$B442. The original keeps it
 * in INTEGRITY_DATA ($A4E2), with $4F $87 after it for VERIFY_DISK_IO. */
constexpr uint16_t DISK_IO_CHECKSUM = 0x7AAE;

/* Sums the bytes from `p` up to `end`, alternately into the low byte and
 * into the high byte, which is then XORed into the low byte. Each add is
 * mod 256. Returns the high byte and the low byte as a word. */
[[c6502::section(".text.integrity")]]
static uint16_t compute_checksum(const uint8_t *p, const uint8_t *end) {
    uint8_t lo = 0;
    uint8_t hi = 0;
    for (;;) {
        lo += *p++;
        if (p == end)
            break;
        hi += *p++;
        lo ^= hi;
        if (p == end)
            break;
    }
    return lo | (uint16_t)hi << 8;
}

/* READ_KEYBOARD calls this once in about 64 calls. A changed disk reader
 * writes a random byte into the messages. */
[[c6502::section(".text.integrity")]]
void verify_integrity(void) {
    if (compute_checksum(DISK_IO_CODE, DISK_IO_CODE + sizeof DISK_IO_CODE)
        != DISK_IO_CHECKSUM)
        integrity_fail();
}

/* The original checksums 34 bytes of its own LOAD_SCENE_DATA, $6DD8-$6DF9,
 * against $4F $87. Nothing writes those bytes, so the check always
 * passes, and the C has no copy of them to check. */
[[c6502::section(".text.integrity")]]
void verify_disk_io(void) {
}

/* ---- Restarting -------------------------------------------------------------- */

/* Fills each byte from $0800 to $A4FF with its page number, and $A501
 * to $A56F with $A5, and boots from the disk. It runs from $A56F, so
 * its store there overwrites its own first byte, which has already run.
 * The end $A56F comes from that address. The original stores each page
 * from offset 0 and then from $FF down to 1, and the tail from $A56F
 * down. Either order leaves the same bytes. */
void cold_restart(void) {
    for (uint8_t *p = (uint8_t *)0x0800; p != (uint8_t *)0xA500; p++)
        *p = (uint16_t)p >> 8;
    for (uint8_t *p = (uint8_t *)0xA501; p <= (uint8_t *)0xA56F; p++)
        *p = 0xA5;
    DISK_BOOT_ROM();
}
