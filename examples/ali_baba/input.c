/*
    input.c -- the menus, the keyboard and the joystick, the action
    menu's rules, the settings, and saving and loading games.

    A menu is drawn on rows 21-23. setup_input runs the menu's init
    routine, copies the enabled options' text into INPUT_BUFFER at their
    positions, and draws the three rows. input_loop then highlights the
    selected option and waits on read_keyboard. A direction moves the
    highlight to the option the selected one names for that direction,
    stepping past disabled options, and a move to nowhere plays
    TUNE_MENU_SELECTION. Fire returns the selected option's exit, a
    CHAIN_ state that run_chain (turn.c) runs next. Most menu exits are
    the chain routines of this file, which adjust a setting or a menu
    and reopen a menu.

    read_keyboard polls check_input, stepping the PRNG on every poll and
    running the idle animation between polls. In keyboard mode a
    direction is I, M, J, and K, in either case, or an arrow key, and
    fire is the space bar, RETURN, or ESC. In joystick mode check_input
    reads both paddles and button 0, and ignores the keyboard.

    A saved game is the game's records, $4000-$59FF, in 26 sectors on
    two tracks of a scenario disk, which this file formats and marks
    with "ALI" on track 0. The game disk is recognized by its VTOC, and
    the game prompts for it again after each save or load.

    Departures from the original:

    - A menu's descriptor is a struct menu, whose options are a
      separate array, since c6502 has no flexible array members. Each
      option's exit is a CHAIN_ state where the original has the
      routine's address, and input_loop returns it where the original
      jumps through INPUT_JMP_VEC. A descriptor's init routine is a
      MENU_INIT_ number, which call_input_init switches on. So
      INPUT_JMP_VEC is gone, and MENU_ATTACK_INIT and
      MENU_DROP_GOLD_INIT, which only call another unit's routine, are
      cases of that switch. The exits that went to the RTS at $626B
      are CHAIN_RETURN.
    - A JMP to a chain routine of another unit is `return CHAIN_X;`. A
      JMP to a chain routine of this file whose only work is to run a
      menu, such as ENTER_CHAR_SELECT, is a call of it, and so are
      CHAR_SELECT_INIT's branch into CHAR_SELECT_HOME and
      CHAR_SELECT_HOME's branch into CHAR_SELECT_HERE. None of these
      calls leads back to its caller.
    - The menus and their strings are C objects that the linker places,
      where the original has them at $607D-$6457 and $7ADC-$7D82. The
      one-character string at $7B64, which no menu names, is left out.
    - SHUFFLE_ENTITIES and SHUFFLE_COUNT aren't translated, because
      nothing calls the routine. INIT_STUB2, the RTS at $6B94 between
      WRITE_GAME_DATA and READ_GAME_DATA, isn't part of this file.
    - The original's scratch bytes are locals and parameters: $BA-$BF,
      the option pointer at $C2 (MENU_OPTION_PTR, which nothing outside
      these routines reads, so it isn't written), and
      INPUT_SCAN_OFFSET ($5B24), which is validate_input's `dir`.
      UNDER_LEVEL and SAFE_TO_REST, code bytes of INIT_GAME_STATE at
      $5B25 and $5B26, are the statics under_level and safe_to_rest.
      INPUT_OPTION_COUNT ($5AAA), DRAW_INPUT_OPTIONS' counter, which
      nothing else reads, is a local of draw_input_options.
    - CHECK_INPUT calls ROM PREAD, which returns its count in Y, and
      c6502 can't take a result in Y. read_paddle reads the paddle
      itself with PREAD's soft-switch accesses, a read of PTRIG and then
      reads of PADDL[n] until bit 7 clears, and counts in Y up to 255
      as PREAD does. Its loop takes 14 cycles a pass where PREAD's
      takes 11, so on a real Apple II its readings are about 11/14 of
      PREAD's, and the joystick's zones need the stick moved further.
    - WAIT_KEYPRESS stores whatever DISPLAY_MESSAGE left in A to the
      keyboard strobe. wait_keypress stores 0, since any write clears
      the strobe.
    - WRITE_GAME_DATA and READ_GAME_DATA are one loop,
      transfer_game_data, which takes the RWTS command. The original's
      DISK_READ and DISK_WRITE are RWTS_DISPATCH with that command.
      WRITE_GAME_DATA's result in A, which no caller reads, isn't
      returned.
    - VERIFY_GAME_DISK, VERIFY_DISK_SIGNATURE, WRITE_DISK_SIGNATURE, and
      FORMAT_DISK_TRACK return the carry as a bool, true where the
      carry is set, for a failed check or an error.
    - CONFIGURE_INPUT returns with TURN_ACTIVE or 0 in A, which nothing
      reads. configure_input returns the MOVE option's enable byte,
      which GAME_TURN_LOOP reads from the table next.
    - READ_KEYBOARD's callers test the Z flag of its final PLA, and
      read_keyboard returns the direction, whose value gives the same
      test.
    - INPUT_LOOP calls TUNE_PLAYER with the strength in Y left over from
      VALIDATE_INPUT, which TUNE_MENU_SELECTION doesn't use. input_loop
      passes 0.

    Ali Baba, main.asm lines 1626-1641, 10246-11198, 12117-12154,
    12213-12484, 12488-12596, 14177-14361, 15318-15320, 15329-15331,
    15343-15345, 15356-15375, 15386-15443, and 17001-17039 (the ORG
    chunks at $09E7, $5F19-$645E, $69C8, $6A47-$6B93, $6B95-$6C11,
    $750C-$761D, $7AAE, $7ABC, $7AC0, $7AC5-$7ACE, $7ADC-$7D82, and
    $A44C):

        ORG     $09E7
    RETIRE_CHARACTER:
        ; Retire the active character from the party.
        ;
        ; Behavior:
        ;   Clears ACTIVE_CHAR, calls REORDER_CURRENT_CHAR to update
        ;   the character list, then enters SCENE_LOOP with SCENE_RETIRE.
        ;
        ; Modifies: ACTIVE_CHAR
        ; Clobbers: A, X, Y

        LDA     #$00
        STA     ACTIVE_CHAR                ; clear active character
        JSR     REORDER_CURRENT_CHAR
        LDA     #SCENE_RETIRE              ; retirement scene
        JMP     SCENE_LOOP
    ...
        ORG     $5F19
    INPUT_LOOP:
        SUBROUTINE INPUT_LOOP
        ; Menu driver loop: highlight selection, read input, dispatch.
        ;
        ; Inputs:
        ;   X  — high byte of menu descriptor pointer
        ;   Y  — low byte of menu descriptor pointer
        ;
        ; Behavior:
        ;   Calls SETUP_INPUT to initialize the menu.  Reads the default
        ;   selection from descriptor byte 1.  Loops: calculates the record
        ;   pointer, highlights via SETUP_CURSOR_EXT, waits for keyboard
        ;   input.  On return key (0), loads the exit handler address from
        ;   record bytes 8-9 into INPUT_JMP_VEC and jumps through it.
        ;   Other keys are validated; if the selection changes, repositions
        ;   the cursor; if unchanged, plays the in-place tune.
        ;
        ; Modifies:
        ;   CURRENT_SELECTION, INPUT_JMP_VEC
        ; Clobbers: A, X, Y
        JSR     SETUP_INPUT
        LDY     #$01
        LDA     ($C0),Y
        STA     CURRENT_SELECTION
    .loop:
        LDA     CURRENT_SELECTION
        JSR     CALC_RECORD_PTR
        JSR     SETUP_CURSOR_EXT
        JSR     READ_KEYBOARD
        BEQ     .exit
        JSR     VALIDATE_INPUT
        BAEQ    CURRENT_SELECTION,_INPUT_LOOP.run_script
        BAEQ    #$00,_INPUT_LOOP.run_script
        STA     CURRENT_SELECTION
        JSR     SETUP_CURSOR
        JMP     .loop
    _INPUT_LOOP.run_script:
        LDX     #$12                ; tune 9: menu selection effect
        JSR     TUNE_PLAYER
        JMP     .loop
    .exit:
        LDWIY    $C2,#$08,INPUT_JMP_VEC
        JMP     (INPUT_JMP_VEC)
        ORG     $5F5E
    INPUT_JMP_VEC:
        DC.W    $A3E4                  ; input callback address (runtime-modified)
        ORG     $5F60
    SETUP_INPUT:
        SUBROUTINE SETUP_INPUT
        ; Initialize a menu: store descriptor, call init, populate display.
        ;
        ; Inputs:
        ;   X  — high byte of menu descriptor pointer
        ;   Y  — low byte of menu descriptor pointer
        ;
        ; Behavior:
        ;   Stores X/Y into $C1/$C0 as the menu descriptor pointer, calls
        ;   the menu's init routine via CALL_INPUT_INIT, clears the 120-byte
        ;   input buffer, draws all options with DRAW_INPUT_OPTIONS, and
        ;   renders the buffer to screen.
        ; Clobbers: A, X, Y
        ; Set up input: store X/Y in $C1/$C0, call setup routines.
        STX     $C1
        STY     $C0
        JSR     CALL_INPUT_INIT
        JSR     CLEAR_INPUT_BUFFER
        JSR     DRAW_INPUT_OPTIONS
        JMP     RENDER_INPUT_BUFFER
        ORG     $5F70
    DRAW_INPUT_OPTIONS:
        SUBROUTINE DRAW_INPUT_OPTIONS
        ; Populate the input display buffer with all enabled menu options.
        ;
        ; Inputs:
        ;   $C0/$C1  — pointer to menu descriptor (byte 0 = option count)
        ;
        ; Behavior:
        ;   Iterates options in reverse order (count down to 1).  For each
        ;   option, calls CALC_RECORD_PTR, checks byte 0 of the record
        ;   (enable flag); if non-zero, copies the option text into
        ;   INPUT_BUFFER via COPY_RECORD_DATA.
        ;
        ; Modifies:
        ;   INPUT_OPTION_COUNT, INPUT_BUFFER
        ; Clobbers: A, X, Y
        LDY     #$00
        LDA     ($C0),Y
        STA     INPUT_OPTION_COUNT
    .loop:
        LDA     INPUT_OPTION_COUNT
        JSR     CALC_RECORD_PTR
        LDY     #$00
        LDA     ($C2),Y
        BEQ     .skip
        JSR     COPY_RECORD_DATA
    .skip:
        DEC     INPUT_OPTION_COUNT
        BNE     .loop
        RTS
        ORG     $5F8C
        SUBROUTINE CALC_RECORD_PTR
        ; Compute pointer to a menu option's 10-byte record.
        ;
        ; Inputs:
        ;   A       — 1-based option index
        ;   $C0/$C1 — pointer to menu descriptor base
        ;
        ; Outputs:
        ;   $C2/$C3 — pointer to the selected option's record
        ;             ($C0/$C1 + A*10 - 6)
        ; Clobbers: A
    ; $C2/$C3 = $C0/$C1 + A*10 - 6
    CALC_RECORD_PTR:
        ASL
        STA     $BA
        ASL
        ASL
        CLC
        ADC     $BA
        ADC     #$FA
        CLC
        ADC     $C0
        STA     $C2
        LDA     $C1
        ADC     #$00
        STA     $C3
        RTS
        ORG     $5FA2
        SUBROUTINE SETUP_CURSOR_EXT
        ; Enable inverse video, then fall through to SETUP_CURSOR.
        ;
        ; Behavior:
        ;   Calls SET_INVERSE_VIDEO to highlight the current selection,
        ;   then falls through into SETUP_CURSOR to position and print.
        ; Clobbers: A
    ; JSR SET_INVERSE_VIDEO then falls through to SETUP_CURSOR
    SETUP_CURSOR_EXT:
        JSR     SET_INVERSE_VIDEO
        ORG     $5FA5
        SUBROUTINE SETUP_CURSOR
        ; Position cursor at option, print its string, restore normal video.
        ;
        ; Inputs:
        ;   $C2/$C3 — pointer to current option record
        ;
        ; Behavior:
        ;   Calls SET_CURSOR_POS to position from record byte 1, sends
        ;   HRCG ctrl-AB, loads the display string pointer from record
        ;   bytes 6-7, prints it, then sends ctrl-N to restore normal video.
        ;
        ; Modifies:
        ;   PRINT_STRING_ADDR, TEXT_COL, TEXT_ROW
        ; Clobbers: A, Y

    SETUP_CURSOR:
        JSR     SET_CURSOR_POS
        JSR     PRINT_CTRL_AB
        LDWIY    $C2,#$06,PRINT_STRING_ADDR
        JSR     PRINT_STRING
        JMP     PRINT_CTRL_N
        ORG     $5FBC
        SUBROUTINE SET_CURSOR_POS
        ; Convert linear menu position to screen row and column.
        ;
        ; Inputs:
        ;   $C2/$C3 — pointer to option record; byte 1 = linear position
        ;
        ; Behavior:
        ;   Reads the linear position from record byte 1.  Maps 0-39 to
        ;   row 21, 40-79 to row 22, 80+ to row 23.  The remainder within
        ;   each row becomes the column.  Stores in TEXT_COL and TEXT_ROW.
        ;
        ; Modifies:
        ;   TEXT_COL, TEXT_ROW
        ; Clobbers: A, Y
    ; Maps linear position at ($C2)+1 to TEXT_ROW ($25) and TEXT_COL ($24)
    ; Positions 0-39 -> row 21, 40-79 -> row 22, 80+ -> row 23
    SET_CURSOR_POS:
        LDY     #$01
        LDA     ($C2),Y
        BAGE    #$28,_SET_CURSOR_POS.row2
        LDY     #$15 ; row 21
        BNE     .set_pos
    _SET_CURSOR_POS.row2:
        SEC
        SBC     #$28
        BAGE    #$28,_SET_CURSOR_POS.row3
        LDY     #$16 ; row 22
        BNE     .set_pos
    _SET_CURSOR_POS.row3:
        LDY     #$17 ; row 23
        SEC
        SBC     #$28
    .set_pos:
        STY     TEXT_ROW
        STA     TEXT_COL
        RTS
        ORG     $5FDD
        SUBROUTINE VALIDATE_INPUT
        ; Scan for the next valid (enabled) menu option matching a key.
        ;
        ; Inputs:
        ;   A               — initial key code to validate
        ;   CURRENT_SELECTION — current selection index
        ;   $C2/$C3         — pointer to current option record
        ;
        ; Behavior:
        ;   Saves $C2/$C3 and the key code.  Loops calling READ_INPUT_CHAR
        ;   and CALC_RECORD_PTR until finding a record whose byte 0 (enable
        ;   flag) is non-zero.  Returns 0 for no match, CURRENT_SELECTION
        ;   for same selection, or the new index.  Restores $C2/$C3.
        ;
        ; Outputs:
        ;   A  — validated option index (0 = no match)
        ; Clobbers: A, Y
    ; Validates input: saves $C2/$C3, loops reading input until valid record found
    VALIDATE_INPUT:
        STA     INPUT_SCAN_OFFSET
        PSHW    $C2 ; save $C2/$C3
    .retry:
        JSR     READ_INPUT_CHAR
        BAEQ    #$00,_VALIDATE_INPUT.done
        BAEQ    CURRENT_SELECTION,_VALIDATE_INPUT.done
        PHA
        JSR     CALC_RECORD_PTR
        LDY     #$00
        LDA     ($C2),Y
        BNE     .found
        PLA
        JMP     .retry
    .found:
        PLA
    _VALIDATE_INPUT.done:
        TAY
        PULW    $C2 ; restore $C2/$C3
        TYA
        RTS
        ORG     $600A
        SUBROUTINE READ_INPUT_CHAR
        ; Read the next key code from the option record.
        ;
        ; Inputs:
        ;   INPUT_SCAN_OFFSET — current scan position
        ;   $C2/$C3           — pointer to option record
        ;
        ; Outputs:
        ;   A  — key code at ($C2)[INPUT_SCAN_OFFSET + 1]
        ; Clobbers: A, Y
    ; Returns character at ($C2) + INPUT_SCAN_OFFSET + 1
    READ_INPUT_CHAR:
        CLC
        LDA     INPUT_SCAN_OFFSET
        ADC     #$01
        TAY
        LDA     ($C2),Y
        RTS
        ORG     $6014
    CALL_INPUT_INIT:
        SUBROUTINE CALL_INPUT_INIT
        ; Execute the menu's initialization routine.
        ;
        ; Inputs:
        ;   $C0/$C1 — pointer to menu descriptor; bytes 2-3 = init routine address
        ;
        ; Behavior:
        ;   Reads the init routine address from descriptor bytes 2-3 into
        ;   INPUT_JMP_VEC and jumps through it.
        ; Clobbers: A, Y
        LDWIY    $C0,#$02,INPUT_JMP_VEC
        JMP     (INPUT_JMP_VEC)
        ORG     $6024
    CLEAR_INPUT_BUFFER:
        SUBROUTINE CLEAR_INPUT_BUFFER
        ; Fill the 120-byte input display buffer with spaces.
        ;
        ; Behavior:
        ;   Writes $A0 (high-bit space) to INPUT_BUFFER[0..$77].
        ;
        ; Modifies:
        ;   INPUT_BUFFER (120 bytes)
        ; Clobbers: A, Y
        LDY     #$77
        LDA     #$A0
    .loop:
        STA     INPUT_BUFFER,Y
        DEY
        BPL     .loop
        RTS
        ORG     $602F
    RENDER_INPUT_BUFFER:
        SUBROUTINE RENDER_INPUT_BUFFER
        ; Output the 120-byte input display buffer to the screen.
        ;
        ; Behavior:
        ;   Positions the cursor at row 21, column 0.  Sends HRCG ctrl-AB,
        ;   then outputs each byte of INPUT_BUFFER through ROM_COUT1.
        ;   Y is saved in $BA across each COUT call.
        ;
        ; Modifies:
        ;   TEXT_COL, TEXT_ROW
        ; Clobbers: A, X, Y
        STOW2    $1500,$24
        JSR     PRINT_CTRL_AB
        LDY     #$00
    _RENDER_INPUT_BUFFER.loop:
        LDA     INPUT_BUFFER,Y
        STY     $BA
        JSR     ROM_COUT1
        LDY     $BA
        INY
        BYNE    #$78,_RENDER_INPUT_BUFFER.loop
        RTS
        ORG     $604C
        SUBROUTINE COPY_RECORD_DATA
        ; Copy an option's display string into the input buffer.
        ;
        ; Inputs:
        ;   $C2/$C3 — pointer to option record; bytes 6-7 = string pointer,
        ;             byte 1 = column offset in INPUT_BUFFER
        ;
        ; Behavior:
        ;   Loads the string pointer from record bytes 6-7, reads the
        ;   string length from byte 0 of the string, computes the buffer
        ;   destination from the column offset, and copies characters into
        ;   INPUT_BUFFER backwards (last to first).
        ;
        ; Modifies:
        ;   PRINT_STRING_ADDR, INPUT_BUFFER (partial)
        ; Clobbers: A, X, Y
    ; Copies data from pointer at ($C2)+6/7 into INPUT_BUFFER
    COPY_RECORD_DATA:
        LDWIY    $C2,#$06,PRINT_STRING_ADDR
        LDY     #$01
        LDA     ($C2),Y
        STA     $BA
        DEY
        LDA     (PRINT_STRING_ADDR),Y
        TAY
        DEC     $BA
        CLC
        ADC     $BA
        TAX
    .copy_loop:
        LDA     (PRINT_STRING_ADDR),Y
        STA     INPUT_BUFFER,X
        DEX
        DEY
        BNE     .copy_loop
        RTS
        ORG     $6071
    STORE_SELECTION_TO_RECORD:
        ; Store CURRENT_SELECTION into byte 1 of the menu descriptor record.
        ;
        ; Inputs:
        ;   MENU_DESC_PTR — pointer to menu descriptor table
        ;   CURRENT_SELECTION — current selection index
        ;
        ; Behavior:
        ;   Saves Y, writes CURRENT_SELECTION into (MENU_DESC_PTR)+1, restores Y.
        ;
        ; Clobbers: A

        TYA                        ; \ save Y
        PHA                        ; /
        LDY     #$01               ; byte 1 of descriptor
        LDA     CURRENT_SELECTION
        STA     (MENU_DESC_PTR),Y  ; write selection to record
        PLA                        ; \ restore Y
        TAY                        ; /
        RTS
        ORG     $607D
    ; Game input option table (7 records x 10 bytes + 4-byte header).
    ; Record format: [enable, col_offset, key, ?, ?, ?, str_ptr_lo, str_ptr_hi, ?, ?]
    ; Byte 0 of records 1-6 is patched by CONFIGURE_INPUT based on combat state.
    ; Header: option count, max_range (patched), init_ptr lo/hi
    GAME_INPUT_TABLE:
    .header:
        HEX 07   ; 7 options
    .max_range = *
        HEX 03   ; max movement range (patched: UNDER_LEVEL*3+3)
        HEX D260 ; init function pointer ($60D2 = CONFIGURE_INPUT)
    .rec1:                  ; record 1 ($6081) - movement
    .rec1_enable = *
        HEX 01   ; enable (patched: MOVE_POINTS or 0)
        HEX 07040207
        HEX 05537D311A
    .rec2:                  ; record 2 ($608B) - combat option
    .rec2_enable = *
        HEX 01   ; enable (patched: ~UNDER_LEVEL)
        HEX 2F0103
        HEX 05063E7D0761
    .rec3:                  ; record 3 ($6095) - combat option
    .rec3_enable = *
        HEX 01   ; enable (patched: ~UNDER_LEVEL)
        HEX 570204
        HEX 0604787BB714
    .rec4:                  ; record 4 ($609F) - flee option
    .rec4_enable = *
        HEX 01   ; enable (patched: UNDER_LEVEL)
        HEX 570301
        HEX 03077F7BC61B
    .rec5:                  ; record 5 ($60A9)
        HEX 01140706
        HEX 0102457D8C61
    .rec6:                  ; record 6 ($60B3) - rest option
    .rec6_enable = *
        HEX 01   ; enable (patched: SAFE_TO_REST)
        HEX 3C0507
        HEX 0203737B3F14
    .rec7:                  ; record 7 ($60BD)
        HEX 01640605
        HEX 0401667B0161
        ORG     $60C7
    TEARDOWN_INPUT:
        SUBROUTINE TEARDOWN_INPUT
        ; Present the default game action menu to the player.
        ;
        ; Behavior:
        ;   Loads the GAME_INPUT_TABLE pointer via GET_INPUT_TABLE_PTR,
        ;   then jumps to INPUT_LOOP to run the standard action menu.
        ;   Entry point from TURN_DISPATCH for player-controlled turns.
        ; Clobbers: A, X, Y
        ; Teardown input and jump to $5F19.
        JSR     GET_INPUT_TABLE_PTR
        JMP     INPUT_LOOP
        ORG     $60CD
    GET_INPUT_TABLE_PTR:
        SUBROUTINE GET_INPUT_TABLE_PTR
        ; Return a pointer to GAME_INPUT_TABLE.
        ;
        ; Outputs:
        ;   X — high byte of GAME_INPUT_TABLE
        ;   Y — low byte of GAME_INPUT_TABLE
        ;
        ; Clobbers: X, Y

        ; Return the address of GAME_INPUT_TABLE in X (hi) and Y (lo).
        ;
        ; Outputs:
        ;   X  — high byte of GAME_INPUT_TABLE
        ;   Y  — low byte of GAME_INPUT_TABLE
        ; Return X=high byte, Y=low byte (pointer to GAME_INPUT_TABLE).
        LDX     #>GAME_INPUT_TABLE
        LDY     #<GAME_INPUT_TABLE
        RTS
        ORG     $60D2
    CONFIGURE_INPUT:
        SUBROUTINE CONFIGURE_INPUT
        ; Patch GAME_INPUT_TABLE enable flags from the current turn state.
        ;
        ; Behavior:
        ;   Calls CHECK_COMBAT_ELIGIBILITY, then patches the enable bytes:
        ;   rec6 (rest) from SAFE_TO_REST, rec4 (flee) from UNDER_LEVEL,
        ;   rec2/3 (combat) from ~UNDER_LEVEL, rec1 (movement) from
        ;   MOVE_POINTS (cleared to 0 if TURN_ACTIVE is 0).  Sets max
        ;   movement range to UNDER_LEVEL + 3.
        ;
        ; Modifies:
        ;   GAME_INPUT_TABLE (enable flags and max range)
        ; Clobbers: A
        ; Configure input handler: patch game input table enable flags
        ; from UNDER_LEVEL/SAFE_TO_REST/MOVE_POINTS/TURN_ACTIVE.
        JSR     CHECK_COMBAT_ELIGIBILITY
        MOVB    SAFE_TO_REST,GAME_INPUT_TABLE+54 ; rec6 byte 0 (rest option)
        MOVB    UNDER_LEVEL,GAME_INPUT_TABLE+34 ; rec4 byte 0 (flee option)
        EOR     #$01
        STA     GAME_INPUT_TABLE+14 ; rec2 byte 0 (combat option)
        STA     GAME_INPUT_TABLE+24 ; rec3 byte 0 (combat option)
        EOR     #$01
        CLC
        ADC     #$03
        STA     GAME_INPUT_TABLE+1  ; max movement range
        MOVB    MOVE_POINTS,GAME_INPUT_TABLE+4 ; rec1 byte 0 (movement)
        LDA     TURN_ACTIVE
        BEQ     .clear
        RTS
    .clear:
        STA     GAME_INPUT_TABLE+4  ; rec1 byte 0 (movement)
        RTS
        ORG     $6101
    MENU_SEE_YOUR_LOT:
        JSR     DISPLAY_CHAR_STATS ; show character stats
        JMP     TEARDOWN_INPUT     ; return to main menu

    MENU_CHECK_ATTACK:
        JSR     CHECK_ATTACK_ELIGIBLE ; check for hostile at same position
        BCC     ENTER_ATTACK_MENU  ; no hostile here -> choose direction
        JMP     FIND_ATTACK_TARGET ; hostile here -> target directly
        ORG     $610F
    ATTACK_DIR_TABLE:
        ; 5 options, max_range=1, init=$626B
        HEX     05 01 6B 62
        ; rec0: DEFEND
        HEX     01 39 03 05 02 04
        DC.W    $7B78              ; "DEFEND"
        DC.W    ENTER_COMBAT_STATE
        ; rec1: LEFT
        HEX     01 29 03 05 04 01
        DC.W    $7C6A              ; left arrow
        DC.W    ATTACK_LEFT
        ; rec2: ABOVE
        HEX     01 0C 05 01 02 04
        DC.W    $7C59              ; up arrow
        DC.W    ATTACK_UP
        ; rec3: RIGHT
        HEX     01 44 03 05 01 02
        DC.W    $7C76              ; right arrow
        DC.W    ATTACK_RIGHT
        ; rec4: BELOW
        HEX     01 5C 01 03 02 04
        DC.W    $7C83              ; down arrow
        DC.W    ATTACK_DOWN

        ; --- enter attack (target of BCC from MENU_CHECK_ATTACK) ---
    ENTER_ATTACK_MENU:
        LDX     #>ATTACK_DIR_TABLE ; X = high byte
        LDY     #<ATTACK_DIR_TABLE ; Y = low byte
        JMP     INPUT_LOOP
        ORG     $614C
    OTHER_OPTIONS_TABLE:
        ; 6 options, max_range=1, init=$626B
        HEX     06 01 6B 62
        ; rec0: MOVE       rec1: DROP GOLD   rec2: DROP ARMOR
        ; rec3: OTHER OPT  rec4: RETIRE      rec5: ADD SOMEONE
        HEX     01 05 03 02 06 04 53 7D 31 1A
        HEX     01 2D 01 03 04 05 FA 7A 17 6F
        HEX     01 55 02 01 05 06 DC 7A E9 6E
        HEX     01 15 06 05 01 02 45 7D F1 61
        HEX     01 3D 04 06 02 03 E7 7A E7 09
        ; rec5: ADD SOMEONE
        HEX     01 65 05 04 03 01
        DC.W    $7AEE              ; "ADD SOMEONE"
        DC.W    CHAR_SELECT_INIT
        ORG     $618C
    MENU_OTHER_OPTIONS:
        LDX     #>OTHER_OPTIONS_TABLE
        LDY     #<OTHER_OPTIONS_TABLE
        JMP     INPUT_LOOP
        ORG     $6193
    GAME_SETTINGS_TABLE:
        ; 9 options, max_range=1, init=$61FD
        ; header: 9 options, max_range=1, init=MENU_SETTINGS_INIT
        DC.B    $09,$01
        DC.W    MENU_SETTINGS_INIT
        ; rec0: MOVE             rec1: GAME SPEED      rec2: # (speed up)
        ; rec3: @ (speed down)   rec4: SOUND IS ON      rec5: DANGER :
        ; rec6: # (danger up)    rec7: @ (danger down)  rec8: SAVE GAME
        HEX     01 03 05 09 08 03 53 7D 31 1A
        HEX     01 13 00 00 00 00 58 7D 6B 62
        HEX     01 23 07 07 01 04 60 7B 09 62
        HEX     01 25 08 08 03 09 62 7B 20 62
        HEX     01 53 09 01 09 07 04 7B 84 09
        HEX     01 66 00 00 00 00 1B 7B 6B 62
        HEX     01 73 03 03 05 08 60 7B 2D 62
        HEX     01 75 04 04 07 01 62 7B 36 62
        ; rec8: SAVE GAME
        HEX     01 2B 01 05 04 05
        DC.W    $7B11              ; "SAVE GAME"
        DC.W    ENTER_SAVE_GAME_SLOT
        ORG     $61F1
    MENU_GAME_SETTINGS:
        SUBROUTINE MENU_GAME_SETTINGS
        ; Enter the game settings menu.
        ;
        ; Behavior:
        ;   Patches max_range to 1 in GAME_SETTINGS_TABLE, then enters
        ;   INPUT_LOOP with the settings table pointer.
        ;
        ; Clobbers: A, X, Y

        LDA     #$01
        STA     GAME_SETTINGS_TABLE+1 ; patch max_range = 1
    MENU_GAME_SETTINGS_ENTER:
        LDX     #>GAME_SETTINGS_TABLE
        LDY     #<GAME_SETTINGS_TABLE
        JMP     INPUT_LOOP

    MENU_SETTINGS_INIT:
        JSR SETTINGS_DISPLAY_INIT              ; splash screen setup
        LDA     ANIM_INDEX         ; game speed value
        ORA     #$B0               ; convert to ASCII digit
        STA     $7D66              ; patch into "GAME SPEED : X" string
        RTS
        ORG     $6209
    SETTINGS_SPEED_UP:
        SUBROUTINE SETTINGS_SPEED_UP
        ; Increase game speed by one step (clamped to max).
        ;
        ; Behavior:
        ;   Increments ANIM_INDEX (game speed), clamps to MAX_GAME_SPEED,
        ;   then re-enters the settings menu. SETTINGS_SPEED_DOWN decrements
        ;   (clamped to 1).
        ;
        ; Modifies: ANIM_INDEX
        ; Clobbers: A, X

        JSR     STORE_SELECTION_TO_RECORD
        INC     ANIM_INDEX         ; speed++
        LDA     ANIM_INDEX
        CMP     MAX_GAME_SPEED     ; at max?
        BCC     .done
        LDA     MAX_GAME_SPEED     ; clamp to max
        STA     ANIM_INDEX
    .done:
        JMP     MENU_GAME_SETTINGS_ENTER ; re-enter settings menu

    SETTINGS_SPEED_DOWN:
        JSR     STORE_SELECTION_TO_RECORD
        DEC     ANIM_INDEX         ; speed--
        BNE     .done              ; nonzero -> ok
        INC     ANIM_INDEX         ; clamp to 1
        BNE     .done              ; (always taken)

    SETTINGS_DANGER_UP:
        JSR     STORE_SELECTION_TO_RECORD
        JSR INC_THRESHOLD_WRAP              ; increment danger threshold
        JMP     MENU_GAME_SETTINGS_ENTER

    SETTINGS_DANGER_DOWN:
        JSR     STORE_SELECTION_TO_RECORD
        JSR DEC_THRESHOLD_WRAP              ; decrement danger threshold
        JMP     MENU_GAME_SETTINGS_ENTER
        ORG     $623F
    ATTACK_TARGET_TABLE:
        ; 3 options, max_range=2, init=MENU_ATTACK_INIT
        HEX     03 02
        DC.W    MENU_ATTACK_INIT
        ; rec0: ATTACK [target]
        HEX     01 06 03 02 00 00
        DC.W    $7C94              ; "ATTACK" + name buffer
        DC.W    RESOLVE_ATTACK
        ; rec1: SOMEONE ELSE
        HEX     01 2E 01 03 00 00
        DC.W    $7CB0              ; "SOMEONE ELSE"
        DC.W    SELECT_SOMEONE_ELSE
        ; rec2: CANCEL
        HEX     01 56 02 01 00 00
        DC.W    $7CC4              ; "CANCEL"
        DC.W    $626B              ; no-op (RTS)

    MENU_ATTACK_TARGET:
        LDX     #>ATTACK_TARGET_TABLE
        LDY     #<ATTACK_TARGET_TABLE
        JMP     INPUT_LOOP

    MENU_ATTACK_INIT:
        JSR     INIT_ATTACK_DISPLAY ; copy target name to display buffer
        RTS
        ORG     $626C
    DROP_GOLD_TABLE:
        ; 5 options, max_range=2, init=MENU_DROP_GOLD_INIT
        HEX     05 02
        DC.W    MENU_DROP_GOLD_INIT
        ; rec0: DUCATS   rec1: # (inc)  rec2: @ (dec)
        ; rec3: DROP     rec4: KEEP ALL
        ; rec0: DUCATS (display only)
        HEX     01 2A 00 00 00 00
        DC.W    $7CD9              ; "DUCATS"
        DC.W    MENU_DROP_GOLD     ; no-op (re-enter menu)
        ; rec1: # (increase drop amount)
        HEX     01 43 04 04 04 03
        DC.W    $7B60              ; "#"
        DC.W    MENU_DROP_DEC
        ; rec2: @ (decrease drop amount)
        HEX     01 45 04 04 02 04
        DC.W    $7B62              ; "@"
        DC.W    MENU_DROP_INC
        ; rec3: DROP GOLD
        HEX     01 52 02 03 03 05
        DC.W    $7AFA              ; "DROP GOLD"
        DC.W    DROP_GOLD_EXEC
        ; rec4: KEEP ALL GOLD
        HEX     01 69 02 03 04 04
        DC.W    $7CF1              ; "KEEP ALL GOLD"
        DC.W    MOVE_LOOP

    MENU_DROP_GOLD:
        LDX     #>DROP_GOLD_TABLE
        LDY     #<DROP_GOLD_TABLE
        JMP     INPUT_LOOP

    MENU_DROP_GOLD_INIT:
        JSR     INIT_DROP_DISPLAY  ; convert drop amount to decimal
        RTS

    MENU_DROP_INC:
        JSR     STORE_SELECTION_TO_RECORD
        JMP     ADJ_DROP_INC       ; binary search: subtract step

    MENU_DROP_DEC:
        JSR     STORE_SELECTION_TO_RECORD
        JMP     ADJ_DROP_DEC       ; binary search: add step

    ; Save and load
        ORG     $62B9
    WORLD_INIT_DATA:
    SAVE_LOAD_TABLE:
        ; 4 options, max_range=1, init=SAVE_LOAD_INIT
        DC.B    $04,$01
        DC.W    SAVE_LOAD_INIT
        ; rec0: NEW GAME
        HEX     01 05 02 02 00 00
        DC.W    $7BF3              ; "BEGIN NEW GAME"
        DC.W    $626B              ; no-op init
        ; rec1: CONTINUE GAME NUMBER NN
        HEX     01 55 01 01 04 03
        DC.W    $7BDB              ; "CONTINUE GAME NUMBER NN"
        DC.W    CONTINUE_GAME      ; continue game handler
        ; rec2: # (increment slot)
        HEX     01 6F 01 01 02 04
        DC.W    $7B60              ; "#"
        DC.W    SLOT_INC_ENTER
        ; rec3: @ (decrement slot)
        HEX     01 71 01 01 03 02
        DC.W    $7B62              ; "@"
        DC.W    SLOT_DEC_ENTER
        ORG     $62E5
    INIT_WORLD:
        SUBROUTINE INIT_WORLD
        ; Pass world initialization data to the game setup loop.
        ;
        ; Behavior:
        ;   Loads X/Y with the address of WORLD_INIT_DATA and tail-calls INPUT_LOOP.
        ;
        ; Clobbers: X, Y

        LDX     #>WORLD_INIT_DATA ; \ X/Y = pointer to init data
        LDY     #<WORLD_INIT_DATA ; /
        JMP     INPUT_LOOP        ; initialize world from data table

        ORG     $62EC
    SAVE_LOAD_INIT:
        JSR     FORMAT_SLOT_NUMBER ; format current slot number into strings
        RTS
        ORG     $62F0
    SLOT_INC:
        INC     SLOT_NUMBER
        LDA     SLOT_NUMBER
        CMP     MAX_SLOT_NUMBER
        BCC     SLOT_UPDATE        ; still below max -> update display
        DEC     SLOT_NUMBER        ; undo: clamp to max
        JMP     SLOT_UPDATE

    SLOT_DEC:
        DEC     SLOT_NUMBER
        BNE     SLOT_UPDATE        ; nonzero -> update display
        INC     SLOT_NUMBER        ; undo: wrap 0 -> 1

    SLOT_UPDATE:
        JSR     STORE_SELECTION_TO_RECORD
        JMP     FORMAT_SLOT_NUMBER

    SLOT_INC_ENTER:
        JSR     SLOT_INC
        JMP     INIT_WORLD

    SLOT_DEC_ENTER:
        JSR     SLOT_DEC
        JMP     INIT_WORLD

    FORMAT_SLOT_NUMBER:
        LDA     #$A0               ; space (high ASCII)
        STA     S_CONTINUE_TENS    ; default tens digit to space
        STA     S_SAVE_SLOT_TENS
        LDA     SLOT_NUMBER
        CMP     #$0A               ; >= 10?
        BCC     .ones              ; no -> skip tens
        SEC
        SBC     #$0A               ; remainder
        PHA
        LDA     #$B1               ; "1" (high ASCII)
        STA     S_CONTINUE_TENS
        STA     S_SAVE_SLOT_TENS
        PLA
    .ones:
        ORA     #$B0               ; convert 0-9 to high ASCII digit
        STA     S_CONTINUE_ONES
        STA     S_SAVE_SLOT_ONES
        RTS
        ORG     $6340
    SAVE_GAME_SLOT_TABLE:
        ; 4 options, max_range=2, init=FORMAT_SLOT_NUMBER
        DC.B    $04,$02
        DC.W    FORMAT_SLOT_NUMBER
        ; rec0: MOVE
        HEX     01 04 02 02 02 02
        DC.W    $7D53              ; "MOVE"
        DC.W    MOVE_LOOP
        ; rec1: SAVE GAME IN SLOT NUMBER NN
        HEX     01 2C 01 01 04 03
        DC.W    $7D67              ; "SAVE GAME IN SLOT NUMBER 00"
        DC.W    SAVE_GAME          ; save game handler
        ; rec2: # (increment)
        HEX     01 49 01 01 02 04
        DC.W    $7B60              ; "#"
        DC.W    SLOT_INC_REENTER
        ; rec3: @ (decrement)
        HEX     01 4B 01 01 03 02
        DC.W    $7B62              ; "@"
        DC.W    SLOT_DEC_REENTER
        ORG     $636C
    ENTER_SAVE_GAME_SLOT:
        LDX     #>SAVE_GAME_SLOT_TABLE
        LDY     #<SAVE_GAME_SLOT_TABLE
        JMP     INPUT_LOOP

    SLOT_DEC_REENTER:
        JSR     SLOT_DEC
        JMP     ENTER_SAVE_GAME_SLOT

    SLOT_INC_REENTER:
        JSR     SLOT_INC
        JMP     ENTER_SAVE_GAME_SLOT
        ORG     $637F
    SCENARIO_DISK_TABLE:
        ; 3 options, max_range=1, init=$626B (RTS = no-op)
        DC.B    $03,$01
        DC.W    $626B
        ; rec0: USE PREVIOUSLY FORMATTED SCENARIO DISK
        HEX     01 00 03 02 00 00
        DC.W    $7C02              ; "USE PREVIOUSLY FORMATTED SCENARIO DISK"
        DC.W    USE_SCENARIO_DISK  ; use existing disk handler
        ; rec1: FORMAT AND USE NEW SCENARIO DISK
        HEX     01 28 01 03 00 00
        DC.W    $7C29              ; "FORMAT AND USE NEW SCENARIO DISK"
        DC.W    FORMAT_SCENARIO_DISK ; format new disk handler
        ; rec2: CANCEL REQUEST
        HEX     01 50 02 01 00 00
        DC.W    $7C4A              ; "CANCEL REQUEST"
        DC.W    CANCEL_SCENARIO    ; cancel handler
        ORG     $63A1
    ENTER_SCENARIO_DISK:
        LDX     #>SCENARIO_DISK_TABLE
        LDY     #<SCENARIO_DISK_TABLE
        JMP     INPUT_LOOP
        ORG     $63A8
    CHAR_SELECT_TABLE:
        ; 6 options, max_range=2, init=$626B (patched at runtime)
        DC.B    $06,$02
        DC.W    $626B
        ; rec0: MOVE
        HEX     01 05 06 02 06 02
        DC.W    $7D53              ; "MOVE"
        DC.W    MOVE_LOOP
        ; rec1: ADD [character name]
        HEX     01 2D 01 05 01 03
        DC.W    $7B47              ; "ADD" + CHAR_SELECT_NAME buffer
        DC.W    INIT_CHAR_SELECT   ; add character handler
        ; rec2: # (next PC)
        HEX     01 49 01 05 02 04
        DC.W    $7B60              ; "#"
        DC.W    CHAR_SELECT_NEXT
        ; rec3: @ (previous PC)
        HEX     01 4B 01 05 03 05
        DC.W    $7B62              ; "@"
        DC.W    CHAR_SELECT_PREV
        ; rec4: TO THIS ROOM (dynamically enabled)
        HEX     01 55 02 06 04 06
        DC.W    $7B27              ; "TO THIS ROOM"
        DC.W    CHAR_SELECT_HERE
        ; rec5: TO ALI BABA'S HOME (dynamically enabled)
        HEX     00 55 05 01 05 01
        DC.W    $7B34              ; "TO ALI BABA'S HOME"
        DC.W    CHAR_SELECT_HOME
        ORG     $63E8
    ENTER_CHAR_SELECT:
        LDX     #>CHAR_SELECT_TABLE
        LDY     #<CHAR_SELECT_TABLE
        JMP     INPUT_LOOP

    CHAR_SELECT_INIT:
        SUBROUTINE CHAR_SELECT_INIT
        ; Initialize the character selection menu.
        ;
        ; Behavior:
        ;   Finds the first player character via FIND_FIRST_PC. If none
        ;   found, displays a message and returns to the move loop.
        ;   Otherwise configures the menu table based on whether
        ;   CURRENT_PLAYER is Ali Baba and enters INPUT_LOOP.
        ;
        ; Clobbers: A, X, Y

        JSR     FIND_FIRST_PC      ; find first player character
        BCC     CHAR_SELECT_HOME   ; carry clear -> found, enter init path
        JSR     SET_CURSOR_ROW21   ; no PCs: position cursor
        LDA     #MSG_SAVED_ME               ; message $35: "YOU SAVED ME..."
        JSR     DISPLAY_MESSAGE
        JMP     MOVE_LOOP          ; back to main loop

    CHAR_SELECT_NEXT:
        JSR     STORE_SELECTION_TO_RECORD
        JSR     FIND_NEXT_PC_WRAP  ; advance to next PC
        JMP     ENTER_CHAR_SELECT

    CHAR_SELECT_PREV:
        JSR     STORE_SELECTION_TO_RECORD
        JSR     FIND_PREV_PC       ; retreat to previous PC
        JMP     ENTER_CHAR_SELECT

    ; "TO THIS ROOM" handler: set ACTIVE_CHAR=1, disable rec4, -> .patch
    CHAR_SELECT_HERE:
        LDA     #$01
        STA     ACTIVE_CHAR
        LDX     #$00
        BEQ     .patch             ; always taken

    ; "TO ALI BABA'S HOME" handler AND .found target from CHAR_SELECT_INIT.
    ; If CURRENT_PLAYER==9 -> CHAR_SELECT_HERE (enable rec5, disable rec4).
    ; Otherwise store CURRENT_PLAYER in ACTIVE_CHAR, disable rec5, enable rec4.
    CHAR_SELECT_HOME:
        LDA     CURRENT_PLAYER
        CMP     #$09
        BEQ     CHAR_SELECT_HERE
        STA     ACTIVE_CHAR
        LDA     #$00               ; disable rec5
        LDX     #$01               ; enable rec4

    .patch:
        STA     CHAR_SELECT_TABLE+54 ; rec5 byte 0 (enable/disable)
        STX     CHAR_SELECT_TABLE+44 ; rec4 byte 0 (enable/disable)
        LDY     #$02
        STY     CHAR_SELECT_TABLE+1  ; max_range = 2
        JMP     ENTER_CHAR_SELECT
        ORG     $6436
    ALT_INPUT_TABLE:
        ; 3 options, max_range=2, init=$626B (RTS = no-op)
        DC.B    $03,$02
        DC.W    $626B
        ; rec0: PAY [amount] FOR [item]
        HEX     01 05 03 02 00 00
        DC.W    S_PAY_FOR          ; "PAY 000000 FOR ..."
        DC.W    SHOP_BUY_HANDLER   ; buy handler
        ; rec1: BUY SOMETHING ELSE
        HEX     01 2D 01 03 00 00
        DC.W    $7D23              ; "BUY SOMETHING ELSE"
        DC.W    BROWSE_HANDLER     ; browse: advance to next item
        ; rec2: NO SALE
        HEX     01 55 02 01 00 00
        DC.W    $7D36              ; "NO SALE"
        DC.W    $626B              ; no-op (return)
        ORG     $6458
    START_GAME_INPUT:
        SUBROUTINE START_GAME_INPUT
        ; Enter the alternative input loop (shops/special scenes).
        ;
        ; Behavior:
        ;   Loads ALT_INPUT_TABLE pointer into X/Y and tail-calls INPUT_LOOP.
        ;
        ; Clobbers: A, X, Y

        LDX     #>ALT_INPUT_TABLE
        LDY     #<ALT_INPUT_TABLE
    ...
        ORG     $69C8
    CONTINUE_GAME:
        SUBROUTINE CONTINUE_GAME
        ; Load a saved game from the appropriate disk.
        ;
        ; Inputs:
        ;   SLOT_NUMBER — save slot index
        ;
        ; Behavior:
        ;   If slot is on game disk, verifies disk and reads save data.
        ;   If slot is on scenario disk, prompts for insert, verifies
        ;   signature, reads data, then re-verifies game disk.
        ;
        ; Clobbers: A, X, Y

        LDA     SLOT_NUMBER
        CMP     GAME_DISK_SLOTS
        BCS     .scenario          ; slot >= 1 -> scenario disk needed
        JSR     ENSURE_GAME_DISK   ; verify game disk
        JMP     READ_GAME_DATA     ; load save from game disk

    .scenario:
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_INSERT_SCENARIO               ; "INSERT SCENARIO DISK"
        JSR     DISPLAY_MESSAGE
        JSR     WAIT_KEYPRESS      ; wait for user
        JSR     VERIFY_DISK_SIGNATURE ; check "ALI" signature
        BCS     .bad_disk          ; carry set -> signature mismatch
        JSR     READ_GAME_DATA     ; load from scenario disk
        JSR     PROMPT_INSERT_DISK ; "INSERT GAME DISK"
        JMP     ENSURE_GAME_DISK   ; re-verify game disk

    .bad_disk:
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_TOUGH_HIDE               ; error message
        JSR     DISPLAY_MESSAGE
        JSR     ENSURE_GAME_DISK
        JMP     INIT_WORLD         ; back to save/load menu
    ...
        ORG     $6A47
    SAVE_GAME:
        SUBROUTINE SAVE_GAME
        ; Save current game state to game or scenario disk.
        ;
        ; Inputs:
        ;   SLOT_NUMBER — save slot index
        ;
        ; Behavior:
        ;   If slot is on game disk, verifies disk and writes save data.
        ;   If slot is on scenario disk, shows intro scene on first use,
        ;   sets SCENARIO_INIT_FLAG, then enters scenario disk handler.
        ;
        ; Clobbers: A, X, Y

        LDA     SLOT_NUMBER
        CMP     GAME_DISK_SLOTS
        BCS     .scenario          ; slot >= 1 -> scenario disk
        JSR     ENSURE_GAME_DISK
        JSR     WRITE_GAME_DATA
        JMP     MOVE_LOOP

    SCENARIO_INIT_FLAG:
        DC.B    $00                ; $6A58: self-modifying flag (0=first, 1=initialized)

    .scenario:
        LDA     SCENARIO_INIT_FLAG
        BNE     .already_init      ; flag set -> skip scene
        JSR     ENSURE_GAME_DISK
        LDA     #$42               ; scene $42: scenario disk prompt
        JSR     SCENE_LOOP
        LDA     #$01
        STA     SCENARIO_INIT_FLAG ; set flag (self-modifying)
    .already_init:
        JMP     ENTER_SCENARIO_DISK
        ORG     $6A6E
    CANCEL_SCENARIO:
        JSR     ENSURE_GAME_DISK
        JMP     MOVE_LOOP

    SCENARIO_INSERT_PROMPT:
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_INSERT_SCENARIO               ; "INSERT SCENARIO DISK"
        JSR     DISPLAY_MESSAGE
        JMP     ENTER_SCENARIO_DISK

    FORMAT_SCENARIO_DISK:
        SUBROUTINE FORMAT_SCENARIO_DISK
        ; Format and initialize a blank scenario disk for saving.
        ;
        ; Behavior:
        ;   Checks for existing signature, formats track if needed,
        ;   writes signature and game data. Re-prompts for game disk
        ;   and returns to the move loop on completion.
        ;
        ; Clobbers: A, X, Y

        JSR     VERIFY_GAME_DISK   ; check game disk inserted
        BCC     SCENARIO_INSERT_PROMPT ; not game disk -> prompt
        JSR     VERIFY_DISK_SIGNATURE ; check "ALI" signature
        BCC     .write             ; no signature -> blank disk, write
        JSR     FORMAT_DISK_TRACK  ; format the track
        BCC     .check_sig         ; format ok -> check signature
    .show_msg:
        JSR     DISPLAY_INSERT_MSG ; display confirmation
    .cleanup:
        JSR     PROMPT_INSERT_DISK ; "INSERT GAME DISK"
        JSR     ENSURE_GAME_DISK
        JMP     MOVE_LOOP

    .check_sig:
        JSR     WRITE_DISK_SIGNATURE ; write "ALI" to disk
        BCS     .show_msg          ; carry set -> show insert msg, then cleanup
        BCC     .write             ; carry clear -> write data

    USE_SCENARIO_DISK:
        JSR     VERIFY_DISK_SIGNATURE ; check "ALI" signature
        BCS     .sig_error         ; no match -> error
    .write:
        JSR     WRITE_GAME_DATA    ; save game data to disk
        JMP     .cleanup

    .sig_error:
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_TOUGH_HIDE               ; error message
        JSR     DISPLAY_MESSAGE
        JMP     .cleanup

    DISPLAY_INSERT_MSG:
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_BREAKS_FREE               ; "INSERT" confirmation message
        JMP     DISPLAY_MESSAGE
        ORG     $6ABF
    ENSURE_GAME_DISK:
        JSR     VERIFY_GAME_DISK
        BCS     .wrong_disk
        RTS
    .wrong_disk:
        JSR     PROMPT_INSERT_DISK
        JMP     ENSURE_GAME_DISK

    PROMPT_INSERT_DISK:
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_INSERT_GAME               ; "INSERT GAME DISK"
        JSR     DISPLAY_MESSAGE

    WAIT_KEYPRESS:
        LDA     #MSG_PRESS_KEY               ; "PRESS ANY KEY"
        JSR     DISPLAY_MESSAGE
        STA     $C010              ; clear keyboard strobe
    .wait:
        LDA     $C000              ; read keyboard
        BPL     .wait              ; loop until key pressed
        RTS
        ORG     $6AE1
    FORMAT_DISK_TRACK:
        LDY     #$0C               ; \ RWTS format command
        LDA     #$04               ;  | command = 4 (format track)
        JMP     RWTS_DISPATCH      ; /

    WRITE_DISK_SIGNATURE:
        SUBROUTINE WRITE_DISK_SIGNATURE
        ; Write the 3-byte "ALI" signature to the scenario disk.
        ;
        ; Behavior:
        ;   Copies DISK_SIGNATURE into SCENE_TEXT_DATA, configures IOB,
        ;   then attempts up to 3 disk writes.
        ;
        ; Outputs:
        ;   C — clear on success, set on failure
        ; Clobbers: A, X, Y

        LDA     #$02               ; retry counter
        STA     $BF
        LDX     #$02
    .copy:
        LDA     DISK_SIGNATURE,X   ; copy "ALI" to buffer
        STA     SCENE_TEXT_DATA,X
        DEX
        BPL     .copy
        JSR     SETUP_DATA_IOB     ; set IOB buffer = $5C00
    .retry:
        JSR     DISK_WRITE         ; write to disk
        BCS     .error
        RTS                        ; success (carry clear)
    .error:
        DEC     $BF
        BPL     .retry
        SEC                        ; all retries failed
        RTS

    VERIFY_DISK_SIGNATURE:
        SUBROUTINE VERIFY_DISK_SIGNATURE
        ; Read and verify the "ALI" disk signature.
        ;
        ; Behavior:
        ;   Configures IOB, reads the signature sector with up to 3
        ;   retries, and compares 3 bytes against DISK_SIGNATURE.
        ;
        ; Outputs:
        ;   C — clear if signature matches, set on mismatch or error
        ; Clobbers: A, X, Y

        JSR     SETUP_DATA_IOB     ; set IOB buffer = $5C00
        LDA     #$02               ; retry counter
        STA     $BF
    .retry:
        JSR     DISK_READ          ; read from disk
        BCC     .check             ; success -> verify bytes
        DEC     $BF
        BPL     .retry
    .fail:
        SEC                        ; all retries failed / mismatch
        RTS
    .check:
        LDX     #$02
    .cmp:
        LDA     DISK_SIGNATURE,X   ; expected "ALI"
        CMP     SCENE_TEXT_DATA,X  ; actual
        BNE     .fail              ; mismatch -> fail immediately
        DEX
        BPL     .cmp
        CLC                        ; signature match
        RTS
        ORG     $6B27
    SETUP_DATA_IOB:
        LDA     #$00
        LDY     #$04
        STA     (RWTS_IOB_PTR),Y   ; IOB+4 = track 0
        INY
        STA     (RWTS_IOB_PTR),Y   ; IOB+5 = sector 0
        LDA     #$00
        LDY     #$08
        STA     (RWTS_IOB_PTR),Y   ; IOB+8 = buffer lo ($00)
        INY
        LDA     #$5C
        STA     (RWTS_IOB_PTR),Y   ; IOB+9 = buffer hi ($5C -> $5C00)
        RTS

    VERIFY_GAME_DISK:
        SUBROUTINE VERIFY_GAME_DISK
        ; Verify that the inserted disk is the game disk.
        ;
        ; Behavior:
        ;   Reads track $11 sector $00 via RWTS with up to 3 retries.
        ;   Checks that the first three bytes are $03, $0F, $10 (VTOC).
        ;
        ; Outputs:
        ;   C — clear if game disk confirmed, set on mismatch
        ; Clobbers: A, X, Y

        JSR     SETUP_DATA_IOB
        LDA     GAME_SIG_TRACK     ; track $11
        LDY     #$04
        STA     (RWTS_IOB_PTR),Y   ; IOB+4 = track
        LDA     GAME_SIG_SECTOR    ; sector $00
        INY
        STA     (RWTS_IOB_PTR),Y   ; IOB+5 = sector
        LDA     #$02               ; retry counter
        STA     $BE
    .retry:
        JSR     DISK_READ
        BCC     .check
        DEC     $BE
        BNE     .retry
    .fail:
        SEC                        ; all retries failed / mismatch
        RTS
    .check:
        LDA     SCENE_TEXT_DATA    ; byte 0
        CMP     #$03
        BNE     .fail
        LDA     SCENE_TEXT_DATA+1  ; byte 1
        CMP     #$0F
        BNE     .fail
        LDA     SCENE_TEXT_DATA+2  ; byte 2
        CMP     #$10
        BNE     .fail
        CLC                        ; game disk confirmed
        RTS
        ORG     $6B72
    WRITE_GAME_DATA:
        SUBROUTINE WRITE_GAME_DATA
        ; Write 26 sectors of game data to disk from $4000+.
        ;
        ; Behavior:
        ;   Configures IOB for the current save slot, then writes
        ;   SAVE_SECTOR_COUNT consecutive sectors, advancing buffer
        ;   and sector after each. Retries once on error.
        ;
        ; Outputs:
        ;   A — 0 on success, 1 on failure
        ; Clobbers: A, X, Y

        LDA     #$02               ; outer retry counter
        STA     $BF
        JSR     SETUP_SLOT_IOB     ; calculate track from slot
        LDA     SAVE_SECTOR_COUNT  ; 26 sectors
        STA     $BE
    .sector:
        JSR     DISK_WRITE
        BCS     .error
        JSR     ADVANCE_SECTOR
        DEC     $BE
        BNE     .sector
        LDA     #$00               ; success
        RTS
    .error:
        DEC     $BF
        BNE     .sector            ; retry remaining sectors
        LDA     #$01               ; failure
        RTS
    ...
        ORG     $6B95
    READ_GAME_DATA:
        SUBROUTINE READ_GAME_DATA
        ; Read 26 sectors of game data from disk into $4000+.
        ;
        ; Behavior:
        ;   Configures IOB for the current save slot, then reads
        ;   SAVE_SECTOR_COUNT consecutive sectors. Retries once on
        ;   error; on persistent failure, cold-restarts the system.
        ;
        ; Clobbers: A, X, Y

        LDA     #$02               ; outer retry counter
        STA     $BF
        JSR     SETUP_SLOT_IOB
        LDA     SAVE_SECTOR_COUNT  ; 26 sectors
        STA     $BE
    .sector:
        JSR     DISK_READ
        BCS     .error
        JSR     ADVANCE_SECTOR
        DEC     $BE
        BNE     .sector
        LDA     #$00               ; success
        RTS
    .error:
        DEC     $BF
        BNE     .sector
        JSR     SET_CURSOR_ROW21
        LDA     #MSG_READ_ERROR               ; read error message
        JSR     DISPLAY_MESSAGE
        JMP     COLD_RESTART       ; fatal: reboot from disk

    SETUP_SLOT_IOB:
        SUBROUTINE SETUP_SLOT_IOB
        ; Configure the RWTS IOB for a save-slot disk operation.
        ;
        ; Inputs:
        ;   SLOT_NUMBER — save slot index
        ;
        ; Behavior:
        ;   Determines game vs scenario disk, computes starting track
        ;   as slot*2 + base, sets IOB fields: track, sector 0,
        ;   buffer at [[MOB_BASE_LO]].
        ;
        ; Clobbers: A, X, Y

        LDX     #$00               ; disk index 0 (game disk)
        LDA     SLOT_NUMBER
        CMP     GAME_DISK_SLOTS
        BCC     .set_track         ; slot < threshold -> game disk
        SEC
        SBC     GAME_DISK_SLOTS    ; adjust for scenario disk
        INX                        ; disk index 1 (scenario disk)
    .set_track:
        ASL                        ; slot * 2 (2 tracks per slot)
        CLC
        ADC     SAVE_TRACK_BASE,X  ; add base track for this disk
        LDY     #$04
        STA     (RWTS_IOB_PTR),Y   ; IOB+4 = track
        INY
        LDA     #$00
        STA     (RWTS_IOB_PTR),Y   ; IOB+5 = sector 0
        LDA     #$00
        LDY     #$08
        STA     (RWTS_IOB_PTR),Y   ; IOB+8 = buffer lo ($00)
        LDA     #$40
        INY
        STA     (RWTS_IOB_PTR),Y   ; IOB+9 = buffer hi ($40 -> $4000)
        LDA     #$00
        LDY     #$0B
        STA     (RWTS_IOB_PTR),Y   ; IOB+$0B = byte count (0 = 256)
        RTS

    ADVANCE_SECTOR:
        SUBROUTINE ADVANCE_SECTOR
        ; Advance IOB to the next sector and buffer page.
        ;
        ; Behavior:
        ;   Increments the buffer page (IOB+9) and sector (IOB+5).
        ;   If sector reaches 13, wraps to 0 and increments track.
        ;
        ; Clobbers: A, X, Y

        LDY     #$09
        LDA     (RWTS_IOB_PTR),Y   ; buffer page
        TAX
        INX                        ; next page
        TXA
        STA     (RWTS_IOB_PTR),Y
        LDY     #$05
        LDA     (RWTS_IOB_PTR),Y   ; sector number
        TAX
        INX
        CPX     #$0D               ; 13 sectors per track
        BCS     .next_track
        TXA
        STA     (RWTS_IOB_PTR),Y   ; store incremented sector
        RTS
    .next_track:
        LDA     #$00               ; sector = 0
        STA     (RWTS_IOB_PTR),Y
        DEY                        ; Y = 4 (track)
        LDA     (RWTS_IOB_PTR),Y
        TAX
        INX                        ; next track
        TXA
        STA     (RWTS_IOB_PTR),Y
        RTS
    ...
        ORG     $750C
    CHECK_COMBAT_ELIGIBILITY:
        SUBROUTINE CHECK_COMBAT_ELIGIBILITY
        ; Check whether the current entity is eligible to enter combat.
        ;
        ; Inputs:
        ;   ENTITY_PTR — pointer to the entity record
        ;
        ; Behavior:
        ;   Extracts level threshold from byte 12; if STEPS_TAKEN is below
        ;   it, sets UNDER_LEVEL. Otherwise checks strength, adjacent
        ;   threats, and encounter state. Sets SAFE_TO_REST=1 if no threats.
        ;
        ; Outputs: SAFE_TO_REST, UNDER_LEVEL
        ; Clobbers: A, X, Y

        STOB    #$00,SAFE_TO_REST
        STA     UNDER_LEVEL
        LDY     #$0C
        LDA     (ENTITY_PTR),Y
        ROR
        AND     #$07
        BAMI    STEPS_TAKEN,_CHECK_COMBAT_ELIGIBILITY.under_level
        LDA     STEPS_TAKEN
        BNE     _CHECK_COMBAT_ELIGIBILITY.done
        LDY     #$05
        LDA     (ENTITY_PTR),Y
        AND     #$1F
        STA     $BA
        INY
        LDA     (ENTITY_PTR),Y
        AND     #$3F
        BAPL    $BA,_CHECK_COMBAT_ELIGIBILITY.done
        LDY     #$03
        LDA     (ENTITY_PTR),Y
        JSR     CHECK_ADJACENT_THREATS
        LDA     ADJACENT_THREAT
        BNE     _CHECK_COMBAT_ELIGIBILITY.done
        LDY     #$03
        LDA     (ENTITY_PTR),Y
        JSR     CHECK_ENCOUNTER
        LDA     ENCOUNTER_RESULT
        BAEQ    #$02,_CHECK_COMBAT_ELIGIBILITY.done
        STOB    #$01,SAFE_TO_REST
        JMP     _CHECK_COMBAT_ELIGIBILITY.done
    _CHECK_COMBAT_ELIGIBILITY.under_level:
        STOB    #$01,UNDER_LEVEL
    _CHECK_COMBAT_ELIGIBILITY.done:
        RTS
        ORG     $755E
    CHECK_INPUT:
        SUBROUTINE CHECK_INPUT
        ; Poll keyboard/joystick for directional or fire input.
        ;
        ; Outputs:
        ;   A — direction index (0=fire, 1=up, 2=down, 3=left, 4=right)
        ;       or raw key code for non-direction keys
        ;   N flag — clear if input available, set if none
        ; Clobbers: A, X, Y

        ; --- Check keyboard ---
        LDA     $C000              ; read keyboard register
        BPL     .no_key            ; no key pressed
        TAX                        ; save key code
        LDA     INPUT_MODE
        BNE     .joy_read          ; joystick mode -> read paddles
        TXA                        ; restore key code
        ; --- Match against direction key tables ---
        LDX     #$04               ; 5 entries (index 4..0)
    .check_keys:
        CMP     DIR_KEYS_UPPER,X
        BEQ     .found_dir
        CMP     DIR_KEYS_LOWER,X
        BEQ     .found_dir
        CMP     DIR_KEYS_CTRL,X
        BEQ     .found_dir
        DEX
        BPL     .check_keys
        BMI     .no_match          ; no direction key matched
    .found_dir:
        TXA                        ; direction index -> A (N clear)
        RTS

        ; --- Direction key lookup tables ---
        ; Index: 0=fire, 1=up, 2=down, 3=left, 4=right
    DIR_KEYS_UPPER:
        DC.B    $A0,$C9,$CD,$CA,$CB ; space, I, M, J, K
    DIR_KEYS_LOWER:
        DC.B    $8D,$E9,$ED,$EA,$EB ; CR, i, m, j, k
    DIR_KEYS_CTRL:
        DC.B    $9B,$8B,$8A,$88,$95 ; ESC, ctrl-K, ctrl-J, ctrl-H, ctrl-U

        ; --- No key ---
    .no_key:
        LDA     INPUT_MODE
        BNE     .joy_read          ; joystick mode -> read paddles
    .no_match:
        LDA     #$FF               ; \ clear keyboard strobe
        STA     $C010              ; / (A=$FF -> N set = no input)
        RTS
    .check_button:
        LDA     $C061              ; read paddle button
        BPL     .no_match          ; not pressed -> no input
        LDA     #$00               ; button = fire (index 0, N clear)
        RTS

        ; --- Joystick axis reading ---
    .joy_read:
        JSR     JOY_DELAY          ; wait for paddle settle
        LDX     #$00               ; paddle 0 (horizontal)
        JSR     $FB1E              ; PREAD: Y = paddle 0 value
        LDA     JOY_AXIS_X         ; \ map X-axis value to zone
        JSR     JOY_TO_ZONE        ;  |
        STA     JOY_AXIS_X         ; / store zone (0/1/2)
        JSR     JOY_DELAY
        LDX     #$01               ; paddle 1 (vertical)
        JSR     $FB1E              ; PREAD: Y = paddle 1 value
        LDA     JOY_AXIS_Y         ; \ map Y-axis value to zone
        JSR     JOY_TO_ZONE        ;  |
        STA     JOY_AXIS_Y         ; / store zone (0/1/2)
        ; --- Map zones to direction ---
        CMP     #$01
        BEQ     .y_center          ; Y center -> check X
        BMI     .y_low             ; Y low -> up candidate
        ; Y high (zone 2) -> check X for disambiguation
        LDA     JOY_AXIS_X
        CMP     #$01
        BNE     .no_dir            ; X not center -> ambiguous
        LDX     #$01               ; X center + Y high = up
    .joy_dir:
        JMP     .found_dir
    .y_low:
        LDA     JOY_AXIS_X
        CMP     #$01
        BNE     .no_dir            ; X not center -> ambiguous
        LDX     #$02               ; X center + Y low = down
        BNE     .joy_dir           ; always -> relay to .found_dir
    .y_center:
        LDA     JOY_AXIS_X
        CMP     #$01
        BEQ     .no_dir            ; both center -> no direction
        BPL     .x_high
        LDX     #$04               ; X low + Y center = right
        BNE     .joy_dir
    .x_high:
        LDX     #$03               ; X high + Y center = left
        BNE     .joy_dir
    .no_dir:
        JMP     .check_button      ; no direction -> check fire button

        ; --- Joystick delay ---
    JOY_DELAY:
        LDA     JOY_SETTLE_DELAY   ; paddle settle delay
        JSR     ROM_WAIT
        RTS

        ; --- Joystick axis threshold data ---
    JOY_AXIS_X:
        DC.B    $01                ; X-axis zone (runtime storage)
    JOY_AXIS_Y:
        DC.B    $01                ; Y-axis zone (runtime storage)
    JOY_THRESH_LO:
        DC.B    $1A,$1D,$1D        ; low thresholds (horiz, vert, vert)
    JOY_THRESH_HI:
        DC.B    $03,$03,$06        ; high thresholds (horiz, vert, vert)

        ; --- Map paddle value to zone (0=low, 1=center, 2=high) ---
    JOY_TO_ZONE:
        TAX                        ; save old zone
        TYA                        ; A = paddle value (from PREAD)
        LSR                        ; \ divide by 8
        LSR                        ;  |
        LSR                        ; /
        CMP     JOY_THRESH_LO,X   ; above low threshold?
        BCS     .zone_hi_check     ; yes -> check high
        CMP     JOY_THRESH_HI,X   ; above high threshold?
        BCS     .zone_center       ; yes -> center
        LDA     #$02               ; below both = high zone
        RTS
    .zone_center:
        LDA     #$01               ; between thresholds = center
        RTS
    .zone_hi_check:
        LDA     #$00               ; above low threshold = low zone
        RTS
    ...
        ORG     $7AAE
    JOY_SETTLE_DELAY:
        DC.B    $C8                     ; paddle settle delay (200)
    ...
        ORG     $7ABC
    MAX_GAME_SPEED:
        DC.B    $05                     ; maximum game speed index
    ...
        ORG     $7AC0
    SLOT_NUMBER:
        DC.B    $01                     ; current save/load slot number
    ...
        ORG     $7AC5
    GAME_DISK_SLOTS:
        DC.B    $01                     ; save slots on game disk
        ORG     $7AC6
    MAX_SLOT_NUMBER:
        DC.B    $11                     ; maximum slot number (17)
        ORG     $7AC7
    SAVE_TRACK_BASE:
        HEX     13 01                   ; base track: [0]=game ($13), [1]=scenario ($01)
        ORG     $7AC9
    SAVE_SECTOR_COUNT:
        DC.B    $1A                     ; sectors per save game (26)
        ORG     $7ACA
    GAME_SIG_TRACK:
        DC.B    $11                     ; game disk signature track
    GAME_SIG_SECTOR:
        DC.B    $00                     ; game disk signature sector
        ORG     $7ACC
    DISK_SIGNATURE:
        HEX     C1 CC C9                ; "ALI" (high ASCII)
    ...
        ORG     $7ADC
    MENU_OPTION_STRINGS:
        ; Menu option strings ($7ADC-$7D82) — length-prefixed high ASCII
        HEX 0A C4 D2 CF D0 A0 C1 D2 CD CF D2
        HEX 06 D2 C5 D4 C9 D2 C5
        HEX 0B C1 C4 C4 A0 D3 CF CD C5 CF CE C5
        HEX 09 C4 D2 CF D0 A0 C7 CF CC C4
        HEX 0C D3 CF D5 CE C4 A0 C9 D3 A0 CF CE A0
        HEX 09 D3 C1 D6 C5 A0 C7 C1 CD C5
        HEX 0B C4 C1 CE C7 C5 D2 A0 BA A0 A0 A0
        HEX 0C D4 CF A0 D4 C8 C9 D3 A0 D2 CF CF CD
        HEX 12 D4 CF A0 C1 CC C9 A0 C2 C1 C2 C1 A7 D3 A0 C8 CF CD C5
        ; "ADD" + 20-byte name buffer (overwritten at runtime)
        HEX 18 C1 C4 C4 A0 B1 A0 A0 AE A0 A0 AE B5 A0 C6 CC FE A0 D8 C1 A0 B5 A0 F2 C2
        HEX 01 A3
        HEX 01 C0
        HEX 01 A0
        HEX 0C D3 C5 C5 A0 D9 CF D5 D2 A0 CC CF D4
        HEX 04 D2 C5 D3 D4
        HEX 06 C4 C5 C6 C5 CE C4
        HEX 04 D3 D4 CF D0
    S_CHEST_OPENS:
        HEX 1B D4 C8 C5 A0 D4 D2 C5 C1 D3 D5 D2 C5 A0 C3 C8 C5 D3 D4 A0 CF D0 C5 CE D3 A0 D5 D0
        ; Treasure display (digit fields overwritten at runtime)
    S_REVEAL_GOLD:
        HEX 1D D4 CF A0 D2 C5 D6 C5 C1 CC A0 A0 A0 CC A0 EB B0 A0 C7 CF CC C4 A0 C4 D5 C3 C1 D4 D3 AE
    S_NOW_HAVE_GOLD:
        HEX 1B D9 CF D5 A0 CE CF D7 A0 C8 C1 D6 C5 A0 A0 E4 CF D2 A0 B0 A0 C4 D5 C3 C1 D4 D3 AE
        DC.B    $00                ; separator
        HEX 17 C3 CF CE D4 C9 CE D5 C5 A0 C7 C1 CD C5 A0 CE D5 CD C2 C5 D2 A0 CE CE
        HEX 0E C2 C5 C7 C9 CE A0 CE C5 D7 A0 C7 C1 CD C5
        HEX 26 D5 D3 C5 A0 D0 D2 C5 D6 C9 CF D5 D3 CC D9 A0 C6 CF D2 CD C1 D4 D4 C5 C4 A0 D3 C3 C5 CE C1 D2 C9 CF A0 C4 C9 D3 CB
        HEX 20 C6 CF D2 CD C1 D4 A0 C1 CE C4 A0 D5 D3 C5 A0 CE C5 D7 A0 D3 C3 C5 CE C1 D2 C9 CF A0 C4 C9 D3 CB
        HEX 0E C3 C1 CE C3 C5 CC A0 D2 C5 D1 D5 C5 D3 D4
        HEX 10 C1 D4 D4 C1 C3 CB A0 C1 C2 CF D6 C5 A0 D9 CF D5
        HEX 0B C1 D4 D4 C1 C3 CB A0 CC C5 C6 D4
        HEX 0C C1 D4 D4 C1 C3 CB A0 D2 C9 C7 C8 D4
        HEX 10 C1 D4 D4 C1 C3 CB A0 C2 C5 CC CF D7 A0 D9 CF D5
        ; "ATTACK" + 20-byte name buffer (overwritten at runtime)
        HEX 1B C1 D4 D4 C1 C3 CB A0 B5 E9 BC A5 A0 D3 C9 C2 C3 CD A0 A0 D4 B1 A0 A0 A0 A0 A0 C9
        HEX 13 C1 D4 D4 C1 C3 CB A0 D3 CF CD C5 CF CE C5 A0 C5 CC D3 C5
        HEX 0D C3 C1 CE C3 C5 CC A0 C1 D4 D4 C1 C3 CB
    S_MISSES:
        HEX 06 CD C9 D3 D3 C5 D3
        ; "DUCATS TO DROP" + display buffer (overwritten at runtime)
        HEX 17 C4 D5 C3 C1 D4 D3 A0 D4 CF A0 C4 D2 CF D0 A0 BA A0 A0 B4 CC A0 BA B0
        HEX 0D CB C5 C5 D0 A0 C1 CC CC A0 C7 CF CC C4
    S_PAY_FOR:
        ; Price digits and item name are overwritten at runtime by DISPLAY_SHOP.
        ; The 20 bytes after "FOR " are development leftovers.
        HEX 23 D0 C1 D9 A0 B0 B0 B0 B0 B0 B0 A0 C6 CF D2 A0 A0 DF A0 A0 A0 A0 D9 A0 E0 A0 A0 A0 C2 B7 A0 B1 C5 A0 C1 F5
        HEX 12 C2 D5 D9 A0 D3 CF CD C5 D4 C8 C9 CE C7 A0 C5 CC D3 C5
        HEX 07 CE CF A0 D3 C1 CC C5
        HEX 06 C1 D4 D4 C1 C3 CB
        HEX 0D CF D4 C8 C5 D2 A0 CF D0 D4 C9 CF CE D3
        HEX 04 CD CF D6 C5
        HEX 0E C7 C1 CD C5 A0 D3 D0 C5 C5 C4 A0 BA A0 B0
        HEX 1B D3 C1 D6 C5 A0 C7 C1 CD C5 A0 C9 CE A0 D3 CC CF D4 A0 CE D5 CD C2 C5 D2 A0 B0 B0
    ...
        ORG     $A44C
    READ_KEYBOARD:
        SUBROUTINE READ_KEYBOARD
        ; Poll for keyboard input with idle animation processing.
        ;
        ; Behavior:
        ;   1. Clears keyboard strobe, ticks animation via ANIM_TICK_AND_WAIT.
        ;   2. 1-in-64 chance: calls VERIFY_INTEGRITY (PRNG AND $7E == 0).
        ;   3. Poll loop: steps PRNG, checks keyboard/joystick via $755E.
        ;      On no input, runs idle handler at [[IDLE_ANIM_HANDLER]] and loops.
        ;   4. On keypress: clears strobe, runs frame check, returns key.
        ;
        ; Outputs:
        ;   A — key code (high bit set)
        ; Clobbers: A, X, Y

        LDA     #$FF
        STA     $C010              ; clear keyboard strobe
        JSR     ANIM_TICK_AND_WAIT ; tick animation + burn time
        LDA     PRNG_OUTPUT
        AND     #$7E               ; 1-in-64 chance (bits 6-1 all zero)
        BNE     .display
        JSR     VERIFY_INTEGRITY   ; periodic integrity check

    .display:
        JSR     TICK_ANIM_COUNTER  ; tick animation counter
    .poll:
        JSR     STEP_PRNG
        JSR     CHECK_INPUT         ; poll keyboard/joystick for input
        BPL     .got_key           ; key available (N flag clear)
        JSR     IDLE_ANIM_HANDLER  ; run idle animation
        JMP     .poll

    .got_key:
        STA     $C010              ; clear keyboard strobe
        PHA
        JSR     ANIM_CHECK_FRAME   ; check frame counter (skip tick/wait)
        PLA
        RTS
*/
#include "ali_baba.h"

/* ---- Menu strings --------------------------------------------------- */

/*
    The menus' text, $7ADC-$7D82 in the original, each a length byte and
    then high-bit ASCII. The strings with fields that other units write
    are declared in ali_baba.h. Some strings hold leftover bytes after
    their text or in their fields, which are kept as the original's image
    has them, since a field's unwritten bytes are drawn.
*/
static uint8_t S_DROP_ARMOR[11] = {     /* $7ADC "DROP ARMOR" */
    0x0A, 0xC4, 0xD2, 0xCF, 0xD0, 0xA0, 0xC1, 0xD2, 0xCD, 0xCF, 0xD2,
};
static uint8_t S_RETIRE[7] = {          /* $7AE7 "RETIRE" */
    0x06, 0xD2, 0xC5, 0xD4, 0xC9, 0xD2, 0xC5,
};
static uint8_t S_ADD_SOMEONE[12] = {    /* $7AEE "ADD SOMEONE" */
    0x0B, 0xC1, 0xC4, 0xC4, 0xA0, 0xD3, 0xCF, 0xCD, 0xC5, 0xCF, 0xCE, 0xC5,
};
static uint8_t S_DROP_GOLD[10] = {      /* $7AFA "DROP GOLD" */
    0x09, 0xC4, 0xD2, 0xCF, 0xD0, 0xA0, 0xC7, 0xCF, 0xCC, 0xC4,
};
struct sound_is_on_string S_SOUND_IS_ON = {     /* $7B04 "SOUND IS ON " */
    { 0x0C, 0xD3, 0xCF, 0xD5, 0xCE, 0xC4, 0xA0, 0xC9, 0xD3, 0xA0, 0xCF },
    { 0xCE, 0xA0 },
};
static uint8_t S_SAVE_GAME[10] = {      /* $7B11 "SAVE GAME" */
    0x09, 0xD3, 0xC1, 0xD6, 0xC5, 0xA0, 0xC7, 0xC1, 0xCD, 0xC5,
};
struct danger_string S_DANGER = {               /* $7B1B "DANGER :   " */
    { 0x0B, 0xC4, 0xC1, 0xCE, 0xC7, 0xC5, 0xD2 },
    { 0xA0, 0xBA, 0xA0, 0xA0, 0xA0 },
};
static uint8_t S_TO_THIS_ROOM[13] = {   /* $7B27 "TO THIS ROOM" */
    0x0C, 0xD4, 0xCF, 0xA0, 0xD4, 0xC8, 0xC9, 0xD3, 0xA0, 0xD2, 0xCF, 0xCF,
    0xCD,
};
static uint8_t S_TO_ALI_BABAS_HOME[19] = {  /* $7B34 "TO ALI BABA'S HOME" */
    0x12, 0xD4, 0xCF, 0xA0, 0xC1, 0xCC, 0xC9, 0xA0, 0xC2, 0xC1, 0xC2, 0xC1,
    0xA7, 0xD3, 0xA0, 0xC8, 0xCF, 0xCD, 0xC5,
};
struct add_name_string S_ADD_NAME = {           /* $7B47 "ADD " and a name */
    { 0x18, 0xC1, 0xC4, 0xC4 },
    {
        0xA0, 0xB1, 0xA0, 0xA0, 0xAE, 0xA0, 0xA0, 0xAE, 0xB5, 0xA0, 0xC6,
        0xCC, 0xFE, 0xA0, 0xD8, 0xC1, 0xA0, 0xB5, 0xA0, 0xF2, 0xC2,
    },
};
/* The original's labels say "increase" for '#' and "decrease" for '@',
 * and that is how the slot, speed, and danger menus use them. The
 * drop-gold menu crosses them (menu_drop_inc). */
static uint8_t S_INCREASE[2] = {        /* $7B60 "#" */
    0x01, 0xA3,
};
static uint8_t S_DECREASE[2] = {        /* $7B62 "@" */
    0x01, 0xC0,
};
static uint8_t S_SEE_YOUR_LOT[13] = {   /* $7B66 "SEE YOUR LOT" */
    0x0C, 0xD3, 0xC5, 0xC5, 0xA0, 0xD9, 0xCF, 0xD5, 0xD2, 0xA0, 0xCC, 0xCF,
    0xD4,
};
static uint8_t S_REST[5] = {            /* $7B73 "REST" */
    0x04, 0xD2, 0xC5, 0xD3, 0xD4,
};
static uint8_t S_DEFEND[7] = {          /* $7B78 "DEFEND" */
    0x06, 0xC4, 0xC5, 0xC6, 0xC5, 0xCE, 0xC4,
};
static uint8_t S_STOP[5] = {            /* $7B7F "STOP" */
    0x04, 0xD3, 0xD4, 0xCF, 0xD0,
};
/* $7B84-$7BDA, which events.c prints as one message. */
struct chest_message S_CHEST_MESSAGE = {
    {
        0x1B, 0xD4, 0xC8, 0xC5, 0xA0, 0xD4, 0xD2, 0xC5, 0xC1, 0xD3, 0xD5,
        0xD2, 0xC5, 0xA0, 0xC3, 0xC8, 0xC5, 0xD3, 0xD4, 0xA0, 0xCF, 0xD0,
        0xC5, 0xCE, 0xD3, 0xA0, 0xD5, 0xD0,
    },
    { 0x1D, 0xD4, 0xCF, 0xA0, 0xD2, 0xC5, 0xD6, 0xC5, 0xC1, 0xCC, 0xA0 },
    { 0xA0, 0xA0, 0xCC, 0xA0, 0xEB },
    {
        0xB0, 0xA0, 0xC7, 0xCF, 0xCC, 0xC4, 0xA0, 0xC4, 0xD5, 0xC3, 0xC1,
        0xD4, 0xD3, 0xAE,
    },
    {
        0x1B, 0xD9, 0xCF, 0xD5, 0xA0, 0xCE, 0xCF, 0xD7, 0xA0, 0xC8, 0xC1,
        0xD6, 0xC5, 0xA0,
    },
    { 0xA0, 0xE4, 0xCF, 0xD2, 0xA0 },
    { 0xB0, 0xA0, 0xC4, 0xD5, 0xC3, 0xC1, 0xD4, 0xD3, 0xAE },
    0x00,
};
/* $7BDB "CONTINUE GAME NUMBER NN", whose NN format_slot_number writes. */
struct continue_game_string {
    uint8_t text[22];           /* the length, 23, and "CONTINUE GAME NUMBER " */
    uint8_t number[2];          /* S_CONTINUE_TENS and S_CONTINUE_ONES */
};
static struct continue_game_string S_CONTINUE_GAME_NUMBER = {
    {
        0x17, 0xC3, 0xCF, 0xCE, 0xD4, 0xC9, 0xCE, 0xD5, 0xC5, 0xA0, 0xC7,
        0xC1, 0xCD, 0xC5, 0xA0, 0xCE, 0xD5, 0xCD, 0xC2, 0xC5, 0xD2, 0xA0,
    },
    { 0xCE, 0xCE },
};
static uint8_t S_BEGIN_NEW_GAME[15] = { /* $7BF3 "BEGIN NEW GAME" */
    0x0E, 0xC2, 0xC5, 0xC7, 0xC9, 0xCE, 0xA0, 0xCE, 0xC5, 0xD7, 0xA0, 0xC7,
    0xC1, 0xCD, 0xC5,
};
/* $7C02 "USE PREVIOUSLY FORMATTED SCENARIO DISK" */
static uint8_t S_USE_PREVIOUS_DISK[39] = {
    0x26, 0xD5, 0xD3, 0xC5, 0xA0, 0xD0, 0xD2, 0xC5, 0xD6, 0xC9, 0xCF, 0xD5,
    0xD3, 0xCC, 0xD9, 0xA0, 0xC6, 0xCF, 0xD2, 0xCD, 0xC1, 0xD4, 0xD4, 0xC5,
    0xC4, 0xA0, 0xD3, 0xC3, 0xC5, 0xCE, 0xC1, 0xD2, 0xC9, 0xCF, 0xA0, 0xC4,
    0xC9, 0xD3, 0xCB,
};
/* $7C29 "FORMAT AND USE NEW SCENARIO DISK" */
static uint8_t S_FORMAT_NEW_DISK[33] = {
    0x20, 0xC6, 0xCF, 0xD2, 0xCD, 0xC1, 0xD4, 0xA0, 0xC1, 0xCE, 0xC4, 0xA0,
    0xD5, 0xD3, 0xC5, 0xA0, 0xCE, 0xC5, 0xD7, 0xA0, 0xD3, 0xC3, 0xC5, 0xCE,
    0xC1, 0xD2, 0xC9, 0xCF, 0xA0, 0xC4, 0xC9, 0xD3, 0xCB,
};
static uint8_t S_CANCEL_REQUEST[15] = { /* $7C4A "CANCEL REQUEST" */
    0x0E, 0xC3, 0xC1, 0xCE, 0xC3, 0xC5, 0xCC, 0xA0, 0xD2, 0xC5, 0xD1, 0xD5,
    0xC5, 0xD3, 0xD4,
};
static uint8_t S_ATTACK_ABOVE[17] = {   /* $7C59 "ATTACK ABOVE YOU" */
    0x10, 0xC1, 0xD4, 0xD4, 0xC1, 0xC3, 0xCB, 0xA0, 0xC1, 0xC2, 0xCF, 0xD6,
    0xC5, 0xA0, 0xD9, 0xCF, 0xD5,
};
static uint8_t S_ATTACK_LEFT[12] = {    /* $7C6A "ATTACK LEFT" */
    0x0B, 0xC1, 0xD4, 0xD4, 0xC1, 0xC3, 0xCB, 0xA0, 0xCC, 0xC5, 0xC6, 0xD4,
};
static uint8_t S_ATTACK_RIGHT[13] = {   /* $7C76 "ATTACK RIGHT" */
    0x0C, 0xC1, 0xD4, 0xD4, 0xC1, 0xC3, 0xCB, 0xA0, 0xD2, 0xC9, 0xC7, 0xC8,
    0xD4,
};
static uint8_t S_ATTACK_BELOW[17] = {   /* $7C83 "ATTACK BELOW YOU" */
    0x10, 0xC1, 0xD4, 0xD4, 0xC1, 0xC3, 0xCB, 0xA0, 0xC2, 0xC5, 0xCC, 0xCF,
    0xD7, 0xA0, 0xD9, 0xCF, 0xD5,
};
struct attack_target_string S_ATTACK_TARGET = { /* $7C94 "ATTACK " and a name */
    { 0x1B, 0xC1, 0xD4, 0xD4, 0xC1, 0xC3, 0xCB },
    {
        0xA0, 0xB5, 0xE9, 0xBC, 0xA5, 0xA0, 0xD3, 0xC9, 0xC2, 0xC3, 0xCD,
        0xA0, 0xA0, 0xD4, 0xB1, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xC9,
    },
};
static uint8_t S_ATTACK_SOMEONE_ELSE[20] = {    /* $7CB0 "ATTACK SOMEONE ELSE" */
    0x13, 0xC1, 0xD4, 0xD4, 0xC1, 0xC3, 0xCB, 0xA0, 0xD3, 0xCF, 0xCD, 0xC5,
    0xCF, 0xCE, 0xC5, 0xA0, 0xC5, 0xCC, 0xD3, 0xC5,
};
static uint8_t S_CANCEL_ATTACK[14] = {  /* $7CC4 "CANCEL ATTACK" */
    0x0D, 0xC3, 0xC1, 0xCE, 0xC3, 0xC5, 0xCC, 0xA0, 0xC1, 0xD4, 0xD4, 0xC1,
    0xC3, 0xCB,
};
uint8_t S_MISSES[7] = {                 /* $7CD2 "MISSES" */
    0x06, 0xCD, 0xC9, 0xD3, 0xD3, 0xC5, 0xD3,
};
/* $7CD9 "DUCATS TO DROP :" and digits */
struct ducats_to_drop_string S_DUCATS_TO_DROP = {
    {
        0x17, 0xC4, 0xD5, 0xC3, 0xC1, 0xD4, 0xD3, 0xA0, 0xD4, 0xCF, 0xA0,
        0xC4, 0xD2, 0xCF, 0xD0, 0xA0, 0xBA, 0xA0,
    },
    { 0xA0, 0xB4, 0xCC, 0xA0, 0xBA },
    0xB0,
};
static uint8_t S_KEEP_ALL_GOLD[14] = {  /* $7CF1 "KEEP ALL GOLD" */
    0x0D, 0xCB, 0xC5, 0xC5, 0xD0, 0xA0, 0xC1, 0xCC, 0xCC, 0xA0, 0xC7, 0xCF,
    0xCC, 0xC4,
};
struct pay_for_string S_PAY_FOR = {     /* $7CFF "PAY 000000 FOR " and an item */
    { 0x23, 0xD0, 0xC1, 0xD9, 0xA0 },
    { 0xB0, 0xB0, 0xB0, 0xB0, 0xB0 },
    { 0xB0, 0xA0, 0xC6, 0xCF, 0xD2, 0xA0 },
    {
        0xA0, 0xDF, 0xA0, 0xA0, 0xA0, 0xA0, 0xD9, 0xA0, 0xE0, 0xA0, 0xA0,
        0xA0, 0xC2, 0xB7, 0xA0, 0xB1, 0xC5, 0xA0, 0xC1, 0xF5,
    },
};
static uint8_t S_BUY_SOMETHING_ELSE[19] = { /* $7D23 "BUY SOMETHING ELSE" */
    0x12, 0xC2, 0xD5, 0xD9, 0xA0, 0xD3, 0xCF, 0xCD, 0xC5, 0xD4, 0xC8, 0xC9,
    0xCE, 0xC7, 0xA0, 0xC5, 0xCC, 0xD3, 0xC5,
};
static uint8_t S_NO_SALE[8] = {         /* $7D36 "NO SALE" */
    0x07, 0xCE, 0xCF, 0xA0, 0xD3, 0xC1, 0xCC, 0xC5,
};
static uint8_t S_ATTACK[7] = {          /* $7D3E "ATTACK" */
    0x06, 0xC1, 0xD4, 0xD4, 0xC1, 0xC3, 0xCB,
};
static uint8_t S_OTHER_OPTIONS[14] = {  /* $7D45 "OTHER OPTIONS" */
    0x0D, 0xCF, 0xD4, 0xC8, 0xC5, 0xD2, 0xA0, 0xCF, 0xD0, 0xD4, 0xC9, 0xCF,
    0xCE, 0xD3,
};
static uint8_t S_MOVE[5] = {            /* $7D53 "MOVE" */
    0x04, 0xCD, 0xCF, 0xD6, 0xC5,
};
/* $7D58 "GAME SPEED : 0", whose digit menu_settings_init writes. */
struct game_speed_string {
    uint8_t text[14];           /* the length, 14, and "GAME SPEED : " */
    uint8_t digit;              /* S_GAME_SPEED_DIGIT */
};
static struct game_speed_string S_GAME_SPEED = {
    {
        0x0E, 0xC7, 0xC1, 0xCD, 0xC5, 0xA0, 0xD3, 0xD0, 0xC5, 0xC5, 0xC4,
        0xA0, 0xBA, 0xA0,
    },
    0xB0,
};
/* $7D67 "SAVE GAME IN SLOT NUMBER 00", whose 00 format_slot_number
 * writes. */
struct save_in_slot_string {
    uint8_t text[26];           /* the length, 27, and "SAVE GAME IN SLOT NUMBER " */
    uint8_t number[2];          /* S_SAVE_SLOT_TENS and S_SAVE_SLOT_ONES */
};
static struct save_in_slot_string S_SAVE_IN_SLOT = {
    {
        0x1B, 0xD3, 0xC1, 0xD6, 0xC5, 0xA0, 0xC7, 0xC1, 0xCD, 0xC5, 0xA0,
        0xC9, 0xCE, 0xA0, 0xD3, 0xCC, 0xCF, 0xD4, 0xA0, 0xCE, 0xD5, 0xCD,
        0xC2, 0xC5, 0xD2, 0xA0,
    },
    { 0xB0, 0xB0 },
};

/* ---- Menus ---------------------------------------------------------- */

/*
    Each option is { enabled, position, { up, down, left, right },
    text, exit }. Moving in a direction goes to the option numbered in
    `next`, stepping on past disabled options, and 0 is no move. An
    option that no `next` names, such as "GAME SPEED : 0", is a label
    that can't be highlighted. The options with exit CHAIN_RETURN went to
    $626B in the original, the RTS of MENU_ATTACK_INIT.
*/
static struct menu_option GAME_INPUT_OPTIONS[7] = {
    { 1, 0x07, { 4, 2, 7, 5 }, S_MOVE, CHAIN_MOVE_LOOP },
    { 1, 0x2F, { 1, 3, 5, 6 }, S_ATTACK, CHAIN_MENU_CHECK_ATTACK },
    { 1, 0x57, { 2, 4, 6, 4 }, S_DEFEND, CHAIN_ENTER_COMBAT_STATE },
    { 1, 0x57, { 3, 1, 3, 7 }, S_STOP, CHAIN_SET_STATE_FIGHT },
    { 1, 0x14, { 7, 6, 1, 2 }, S_OTHER_OPTIONS, CHAIN_MENU_OTHER_OPTIONS },
    { 1, 0x3C, { 5, 7, 2, 3 }, S_REST, CHAIN_AUTO_HEAL },
    { 1, 0x64, { 6, 5, 4, 1 }, S_SEE_YOUR_LOT, CHAIN_MENU_SEE_YOUR_LOT },
};
struct menu GAME_INPUT_TABLE = { 7, 3, MENU_INIT_CONFIGURE_INPUT, GAME_INPUT_OPTIONS };
/* The indexes in GAME_INPUT_OPTIONS of the options configure_input
 * enables and disables. */
enum {
    MOVE_OPTION = 0,
    ATTACK_OPTION = 1,
    DEFEND_OPTION = 2,
    STOP_OPTION = 3,
    REST_OPTION = 5,
};

static struct menu_option ATTACK_DIR_OPTIONS[5] = {
    { 1, 0x39, { 3, 5, 2, 4 }, S_DEFEND, CHAIN_ENTER_COMBAT_STATE },
    { 1, 0x29, { 3, 5, 4, 1 }, S_ATTACK_LEFT, CHAIN_ATTACK_LEFT },
    { 1, 0x0C, { 5, 1, 2, 4 }, S_ATTACK_ABOVE, CHAIN_ATTACK_UP },
    { 1, 0x44, { 3, 5, 1, 2 }, S_ATTACK_RIGHT, CHAIN_ATTACK_RIGHT },
    { 1, 0x5C, { 1, 3, 2, 4 }, S_ATTACK_BELOW, CHAIN_ATTACK_DOWN },
};
struct menu ATTACK_DIR_TABLE = { 5, 1, MENU_INIT_NONE, ATTACK_DIR_OPTIONS };

static struct menu_option OTHER_OPTIONS_OPTIONS[6] = {
    { 1, 0x05, { 3, 2, 6, 4 }, S_MOVE, CHAIN_MOVE_LOOP },
    { 1, 0x2D, { 1, 3, 4, 5 }, S_DROP_GOLD, CHAIN_INIT_DROP_GOLD },
    { 1, 0x55, { 2, 1, 5, 6 }, S_DROP_ARMOR, CHAIN_FLUSH_PENDING_XP },
    { 1, 0x15, { 6, 5, 1, 2 }, S_OTHER_OPTIONS, CHAIN_MENU_GAME_SETTINGS },
    { 1, 0x3D, { 4, 6, 2, 3 }, S_RETIRE, CHAIN_RETIRE_CHARACTER },
    { 1, 0x65, { 5, 4, 3, 1 }, S_ADD_SOMEONE, CHAIN_CHAR_SELECT_INIT },
};
struct menu OTHER_OPTIONS_TABLE = { 6, 1, MENU_INIT_NONE, OTHER_OPTIONS_OPTIONS };

static struct menu_option GAME_SETTINGS_OPTIONS[9] = {
    { 1, 0x03, { 5, 9, 8, 3 }, S_MOVE, CHAIN_MOVE_LOOP },
    { 1, 0x13, { 0, 0, 0, 0 }, (uint8_t *)&S_GAME_SPEED, CHAIN_RETURN },
    { 1, 0x23, { 7, 7, 1, 4 }, S_INCREASE, CHAIN_SETTINGS_SPEED_UP },
    { 1, 0x25, { 8, 8, 3, 9 }, S_DECREASE, CHAIN_SETTINGS_SPEED_DOWN },
    { 1, 0x53, { 9, 1, 9, 7 }, (uint8_t *)&S_SOUND_IS_ON, CHAIN_TOGGLE_SOUND },
    { 1, 0x66, { 0, 0, 0, 0 }, (uint8_t *)&S_DANGER, CHAIN_RETURN },
    { 1, 0x73, { 3, 3, 5, 8 }, S_INCREASE, CHAIN_SETTINGS_DANGER_UP },
    { 1, 0x75, { 4, 4, 7, 1 }, S_DECREASE, CHAIN_SETTINGS_DANGER_DOWN },
    { 1, 0x2B, { 1, 5, 4, 5 }, S_SAVE_GAME, CHAIN_ENTER_SAVE_GAME_SLOT },
};
struct menu GAME_SETTINGS_TABLE = { 9, 1, MENU_INIT_SETTINGS, GAME_SETTINGS_OPTIONS };

static struct menu_option ATTACK_TARGET_OPTIONS[3] = {
    { 1, 0x06, { 3, 2, 0, 0 }, (uint8_t *)&S_ATTACK_TARGET, CHAIN_RESOLVE_ATTACK },
    { 1, 0x2E, { 1, 3, 0, 0 }, S_ATTACK_SOMEONE_ELSE, CHAIN_SELECT_SOMEONE_ELSE },
    { 1, 0x56, { 2, 1, 0, 0 }, S_CANCEL_ATTACK, CHAIN_RETURN },
};
struct menu ATTACK_TARGET_TABLE = { 3, 2, MENU_INIT_ATTACK, ATTACK_TARGET_OPTIONS };

/* Option 1 is a label. Its exit, MENU_DROP_GOLD, can't be reached. */
static struct menu_option DROP_GOLD_OPTIONS[5] = {
    { 1, 0x2A, { 0, 0, 0, 0 }, (uint8_t *)&S_DUCATS_TO_DROP, CHAIN_MENU_DROP_GOLD },
    { 1, 0x43, { 4, 4, 4, 3 }, S_INCREASE, CHAIN_MENU_DROP_DEC },
    { 1, 0x45, { 4, 4, 2, 4 }, S_DECREASE, CHAIN_MENU_DROP_INC },
    { 1, 0x52, { 2, 3, 3, 5 }, S_DROP_GOLD, CHAIN_DROP_GOLD_EXEC },
    { 1, 0x69, { 2, 3, 4, 4 }, S_KEEP_ALL_GOLD, CHAIN_MOVE_LOOP },
};
struct menu DROP_GOLD_TABLE = { 5, 2, MENU_INIT_DROP_GOLD, DROP_GOLD_OPTIONS };

/* WORLD_INIT_DATA in the original. "BEGIN NEW GAME" returns from the
 * chain to game_init. */
static struct menu_option SAVE_LOAD_OPTIONS[4] = {
    { 1, 0x05, { 2, 2, 0, 0 }, S_BEGIN_NEW_GAME, CHAIN_RETURN },
    { 1, 0x55, { 1, 1, 4, 3 }, (uint8_t *)&S_CONTINUE_GAME_NUMBER, CHAIN_CONTINUE_GAME },
    { 1, 0x6F, { 1, 1, 2, 4 }, S_INCREASE, CHAIN_SLOT_INC_ENTER },
    { 1, 0x71, { 1, 1, 3, 2 }, S_DECREASE, CHAIN_SLOT_DEC_ENTER },
};
struct menu SAVE_LOAD_TABLE = { 4, 1, MENU_INIT_SAVE_LOAD, SAVE_LOAD_OPTIONS };

static struct menu_option SAVE_GAME_SLOT_OPTIONS[4] = {
    { 1, 0x04, { 2, 2, 2, 2 }, S_MOVE, CHAIN_MOVE_LOOP },
    { 1, 0x2C, { 1, 1, 4, 3 }, (uint8_t *)&S_SAVE_IN_SLOT, CHAIN_SAVE_GAME },
    { 1, 0x49, { 1, 1, 2, 4 }, S_INCREASE, CHAIN_SLOT_INC_REENTER },
    { 1, 0x4B, { 1, 1, 3, 2 }, S_DECREASE, CHAIN_SLOT_DEC_REENTER },
};
struct menu SAVE_GAME_SLOT_TABLE = { 4, 2, MENU_INIT_FORMAT_SLOT_NUMBER, SAVE_GAME_SLOT_OPTIONS };

static struct menu_option SCENARIO_DISK_OPTIONS[3] = {
    { 1, 0x00, { 3, 2, 0, 0 }, S_USE_PREVIOUS_DISK, CHAIN_USE_SCENARIO_DISK },
    { 1, 0x28, { 1, 3, 0, 0 }, S_FORMAT_NEW_DISK, CHAIN_FORMAT_SCENARIO_DISK },
    { 1, 0x50, { 2, 1, 0, 0 }, S_CANCEL_REQUEST, CHAIN_CANCEL_SCENARIO },
};
struct menu SCENARIO_DISK_TABLE = { 3, 1, MENU_INIT_NONE, SCENARIO_DISK_OPTIONS };

/* Options 5 and 6 share a position, and char_select_patch enables one of
 * them. */
static struct menu_option CHAR_SELECT_OPTIONS[6] = {
    { 1, 0x05, { 6, 2, 6, 2 }, S_MOVE, CHAIN_MOVE_LOOP },
    { 1, 0x2D, { 1, 5, 1, 3 }, (uint8_t *)&S_ADD_NAME, CHAIN_INIT_CHAR_SELECT },
    { 1, 0x49, { 1, 5, 2, 4 }, S_INCREASE, CHAIN_CHAR_SELECT_NEXT },
    { 1, 0x4B, { 1, 5, 3, 5 }, S_DECREASE, CHAIN_CHAR_SELECT_PREV },
    { 1, 0x55, { 2, 6, 4, 6 }, S_TO_THIS_ROOM, CHAIN_CHAR_SELECT_HERE },
    { 0, 0x55, { 5, 1, 5, 1 }, S_TO_ALI_BABAS_HOME, CHAIN_CHAR_SELECT_HOME },
};
struct menu CHAR_SELECT_TABLE = { 6, 2, MENU_INIT_NONE, CHAR_SELECT_OPTIONS };
/* The indexes in CHAR_SELECT_OPTIONS of options 5 and 6. */
enum { THIS_ROOM_OPTION = 4, ALI_BABAS_HOME_OPTION = 5 };

/* The shop's menu. */
static struct menu_option ALT_INPUT_OPTIONS[3] = {
    { 1, 0x05, { 3, 2, 0, 0 }, (uint8_t *)&S_PAY_FOR, CHAIN_SHOP_BUY_HANDLER },
    { 1, 0x2D, { 1, 3, 0, 0 }, S_BUY_SOMETHING_ELSE, CHAIN_BROWSE_HANDLER },
    { 1, 0x55, { 2, 1, 0, 0 }, S_NO_SALE, CHAIN_RETURN },
};
struct menu ALT_INPUT_TABLE = { 3, 2, MENU_INIT_NONE, ALT_INPUT_OPTIONS };

/* ---- Keys and the joystick ------------------------------------------- */

/* What check_input returns when there is no direction or fire. */
constexpr uint8_t NO_INPUT = 0xFF;

/* The keys for each DIR_, fire first: space, I, M, J, and K; return and
 * the lower-case letters; and ESC and the arrow keys (ctrl-K, ctrl-J,
 * ctrl-H, ctrl-U). DIR_KEYS_UPPER, DIR_KEYS_LOWER, and DIR_KEYS_CTRL in
 * the original. */
static const uint8_t DIR_KEYS[3][5] = {
    { 0xA0, 0xC9, 0xCD, 0xCA, 0xCB },
    { 0x8D, 0xE9, 0xED, 0xEA, 0xEB },
    { 0x9B, 0x8B, 0x8A, 0x88, 0x95 },
};

/*
    A paddle's reading, divided by 8, falls in one of three zones. A
    reading that rises past ZONE_THRESH_PLUS enters ZONE_PLUS (right on
    paddle 0, down on paddle 1), and one that falls below
    ZONE_THRESH_MINUS enters ZONE_MINUS (left or up). Each threshold
    depends on the zone the stick was in, so that a stick held near a
    threshold doesn't flicker between two zones. The original's comment
    on the tables, "(horiz, vert, vert)", doesn't describe their index.
*/
enum { ZONE_PLUS = 0, ZONE_CENTER = 1, ZONE_MINUS = 2 };
static const uint8_t ZONE_THRESH_PLUS[3] = { 0x1A, 0x1D, 0x1D };    /* JOY_THRESH_LO */
static const uint8_t ZONE_THRESH_MINUS[3] = { 0x03, 0x03, 0x06 };   /* JOY_THRESH_HI */

/* Each paddle's zone at its last reading. JOY_AXIS_X and JOY_AXIS_Y. */
static uint8_t joy_zone_x = ZONE_CENTER;
static uint8_t joy_zone_y = ZONE_CENTER;

/* The WAIT before each paddle read, about 100,000 cycles, which lets the
 * other paddle's timer run out first. JOY_SETTLE_DELAY. */
constexpr uint8_t JOY_SETTLE_DELAY = 0xC8;

/* ---- Settings and save slots ----------------------------------------- */

constexpr uint8_t MIN_GAME_SPEED = 1;
constexpr uint8_t MAX_GAME_SPEED = 5;

/* The save slot the save/load and save menus show, 1-16. Slots below
 * GAME_DISK_SLOTS are on the game disk and the rest on a scenario disk,
 * so every slot is on a scenario disk. */
static uint8_t slot_number = 1;
constexpr uint8_t GAME_DISK_SLOTS = 1;
constexpr uint8_t MAX_SLOT_NUMBER = 0x11;   /* one past the last slot */

/* A slot takes two tracks, from this track on the game disk ([0]) or a
 * scenario disk ([1]). It holds the game's records, $4000-$59FF, in 26
 * sectors, 13 to a track. */
static const uint8_t SAVE_TRACK_BASE[2] = { 0x13, 0x01 };
constexpr uint8_t SAVE_SECTOR_COUNT = 26;
constexpr uint8_t SECTORS_PER_TRACK = 13;

/* The VTOC's sector, whose first bytes verify_game_disk checks. */
const uint8_t GAME_SIG_TRACK = 0x11;
const uint8_t GAME_SIG_SECTOR = 0x00;

/* "ALI", which write_disk_signature writes at the start of a scenario
 * disk's track 0, sector 0. */
static const uint8_t DISK_SIGNATURE[3] = { 0xC1, 0xCC, 0xC9 };

/* 0 until the scene that shows how to save on a scenario disk has
 * played. A byte in SAVE_GAME's code in the original. */
static uint8_t scenario_init_flag = 0;

/* What check_combat_eligibility finds for configure_input. Code bytes of
 * INIT_GAME_STATE in the original, UNDER_LEVEL ($5B25) and SAFE_TO_REST
 * ($5B26). */
static bool under_level;
static bool safe_to_rest;

static uint8_t check_input(void);

/* ---- The menu driver ------------------------------------------------- */

/* Returns option `n` (1-based) of the menu MENU_DESC_PTR points at.
 * CALC_RECORD_PTR. */
static struct menu_option *option(uint8_t n) {
    return &MENU_DESC_PTR->options[n - 1];
}

/* Moves the cursor to an option's position, which counts 40 cells to a
 * line from the start of row 21. A position of 120 or more, which no
 * menu has, stays on row 23 at a column past 39, as in the original,
 * which subtracts 40 at most twice. SET_CURSOR_POS. */
static void set_cursor_pos(uint8_t pos) {
    if (pos < 80) {
        TEXT_ROW = 21 + pos / 40;
        TEXT_COL = pos % 40;
    } else {
        TEXT_ROW = 23;
        TEXT_COL = pos - 80;
    }
}

/* Draws option `o` in the current video mode, and then selects normal
 * video. SETUP_CURSOR. */
static void draw_option(const struct menu_option *o) {
    set_cursor_pos(o->position);
    print_ctrl_ab();
    print_string(o->text);
    print_ctrl_n();
}

/* Runs the menu's init routine. CALL_INPUT_INIT, which jumped through
 * the descriptor's bytes 2-3. */
static void call_input_init(uint8_t init);

/* Fills the three menu rows with spaces. CLEAR_INPUT_BUFFER. */
static void clear_input_buffer(void) {
    for (uint8_t i = 0; i < sizeof INPUT_BUFFER; i++)
        INPUT_BUFFER[i] = 0xA0;
}

/* Copies option `o`'s text into INPUT_BUFFER at its position.
 * COPY_RECORD_DATA, which copies from the last character to the first. */
static void copy_record_data(const struct menu_option *o) {
    uint8_t length = o->text[0];
    const uint8_t *chars = o->text + 1;
    uint8_t *at = &INPUT_BUFFER[o->position];
    for (uint8_t i = 0; i < length; i++)
        at[i] = chars[i];
}

/* Copies the enabled options' text into INPUT_BUFFER, from the last
 * option to the first, so that an option drawn later covers one at the
 * same position. DRAW_INPUT_OPTIONS, which counts the options in
 * INPUT_OPTION_COUNT ($5AAA). Nothing else reads that byte, and the
 * count is a local here. */
static void draw_input_options(void) {
    for (uint8_t n = MENU_DESC_PTR->count; n != 0; n--) {
        const struct menu_option *o = option(n);
        if (o->enabled)
            copy_record_data(o);
    }
}

/* Draws the three menu rows, rows 21-23. RENDER_INPUT_BUFFER. */
static void render_input_buffer(void) {
    TEXT_ROW = 21;
    TEXT_COL = 0;
    print_ctrl_ab();
    for (uint8_t i = 0; i < sizeof INPUT_BUFFER; i++)
        hrcg_cout(INPUT_BUFFER[i]);
}

void setup_input(struct menu *m) {
    MENU_DESC_PTR = m;
    call_input_init(m->init);
    clear_input_buffer();
    draw_input_options();
    render_input_buffer();
}

/* Returns the option a move in direction `dir` from option `o` reaches:
 * the first enabled option in a chain of `next` links in that direction,
 * the current selection, or 0. Nothing ends a chain of disabled options
 * that loops, as in the original, but no menu has one. VALIDATE_INPUT
 * and READ_INPUT_CHAR. */
static uint8_t validate_input(uint8_t dir, const struct menu_option *o) {
    for (;;) {
        uint8_t n = o->next[dir - 1];
        if (n == 0 || n == CURRENT_SELECTION)
            return n;
        o = option(n);
        if (o->enabled)
            return n;
    }
}

uint8_t input_loop(struct menu *m) {
    setup_input(m);
    CURRENT_SELECTION = m->selection;
    for (;;) {
        const struct menu_option *o = option(CURRENT_SELECTION);
        set_inverse_video();
        draw_option(o);
        uint8_t dir = read_keyboard();
        if (dir == DIR_NONE)
            return o->exit;
        uint8_t n = validate_input(dir, o);
        if (n == CURRENT_SELECTION || n == 0) {
            tune_player(TUNE_MENU_SELECTION, 0);
        } else {
            CURRENT_SELECTION = n;
            draw_option(o);     /* the old option, in normal video */
        }
    }
}

/* Stores the highlighted option as the menu's first selection, so that
 * the menu opens on it again. STORE_SELECTION_TO_RECORD. */
static void store_selection_to_record(void) {
    MENU_DESC_PTR->selection = CURRENT_SELECTION;
}

/* ---- Keyboard and joystick --------------------------------------------- */

/* Reads paddle `n` as ROM PREAD does, with PREAD's soft-switch accesses:
 * PTRIG, then PADDL[n] until bit 7 clears, counting the reads with bit 7
 * set up to 255. The reading is the time the loop takes, so its cycles
 * a pass matter, and `count` is `register` to keep it in Y, where PREAD
 * counts. With the count in zero page, the loop would take 17 cycles a
 * pass. PREAD's loop takes 11, and this one 14, because the compiler
 * stores each reading (see notes/input.md). */
static uint8_t read_paddle(uint8_t n) {
    register uint8_t count = 0;
    (void)PTRIG;
    while (PADDL[n] & 0x80) {
        if (++count == 0)
            return 0xFF;
    }
    return count;
}

/* Returns the zone of a paddle `reading` for a stick that was in
 * `old_zone`. JOY_TO_ZONE. */
static uint8_t joy_to_zone(uint8_t old_zone, uint8_t reading) {
    reading >>= 3;
    if (reading >= ZONE_THRESH_PLUS[old_zone])
        return ZONE_PLUS;
    if (reading >= ZONE_THRESH_MINUS[old_zone])
        return ZONE_CENTER;
    return ZONE_MINUS;
}

/* Returns the direction of the joystick, DIR_NONE for button 0, or
 * NO_INPUT, which also clears the keyboard strobe. Each paddle is read
 * after a WAIT. A stick held on a diagonal gives no direction. */
static uint8_t check_joystick(void) {
    WAIT(JOY_SETTLE_DELAY);
    joy_zone_x = joy_to_zone(joy_zone_x, read_paddle(0));
    WAIT(JOY_SETTLE_DELAY);
    joy_zone_y = joy_to_zone(joy_zone_y, read_paddle(1));
    if (joy_zone_y == ZONE_CENTER) {
        if (joy_zone_x == ZONE_PLUS)
            return DIR_RIGHT;
        if (joy_zone_x == ZONE_MINUS)
            return DIR_LEFT;
    } else if (joy_zone_x == ZONE_CENTER) {
        return joy_zone_y == ZONE_MINUS ? DIR_UP : DIR_DOWN;
    }
    if (BUTN[0] & 0x80)
        return DIR_NONE;
    KBDSTRB = 0xFF;
    return NO_INPUT;
}

/* Returns the direction of the key or the joystick, DIR_NONE for fire,
 * or NO_INPUT. In keyboard mode a key that isn't a direction key, or no
 * key, clears the strobe. In joystick mode a key is ignored. The
 * original's comment says a key other than a direction key is returned
 * as it is, which the code doesn't do. CHECK_INPUT. */
static uint8_t check_input(void) {
    uint8_t key = KBD;
    if (INPUT_MODE != 0)
        return check_joystick();
    if (key & 0x80) {
        for (uint8_t dir = 0; dir < 5; dir++) {
            if (key == DIR_KEYS[0][dir] || key == DIR_KEYS[1][dir]
                    || key == DIR_KEYS[2][dir])
                return dir;
        }
    }
    KBDSTRB = 0xFF;
    return NO_INPUT;
}

uint8_t read_keyboard(void) {
    KBDSTRB = 0xFF;
    anim_tick_and_wait();
    if ((PRNG_OUTPUT & 0x7E) == 0)
        verify_integrity();
    tick_anim_counter();
    uint8_t dir;
    for (;;) {
        step_prng();
        dir = check_input();
        if (dir != NO_INPUT)
            break;
        idle_anim_handler();
    }
    KBDSTRB = dir;
    anim_check_frame();
    return dir;
}

/* ---- The action menu --------------------------------------------------- */

/* Sets under_level when ENTITY_PTR has taken more steps this turn than
 * its speed's bits 3-1 allow. Otherwise, before its first step, sets
 * safe_to_rest when its life is below its strength, no hostile mob is
 * next to it, and none shares its cell. CHECK_COMBAT_ELIGIBILITY. */
static void check_combat_eligibility(void) {
    safe_to_rest = false;
    under_level = false;
    /* The original tests the sign of the byte allowed - STEPS_TAKEN. A
     * player's MOVE_POINTS can count down from 255, so STEPS_TAKEN can
     * pass 135, where the sign and STEPS_TAKEN > allowed differ. */
    uint8_t allowed = (ENTITY_PTR->speed >> 1) & 7;
    if ((int8_t)(allowed - STEPS_TAKEN) < 0) {
        under_level = true;
        return;
    }
    if (STEPS_TAKEN != 0)
        return;
    /* The original subtracts and tests the sign, which for these ranges
     * is the same comparison. */
    if ((ENTITY_PTR->life & HP_MASK) >= (ENTITY_PTR->strength & STRENGTH_MASK))
        return;
    check_adjacent_threats(ENTITY_PTR->pos);
    if (ADJACENT_THREAT != 0)
        return;
    if (check_encounter(ENTITY_PTR->pos) == 2)     /* a hostile mob */
        return;
    safe_to_rest = true;
}

uint8_t configure_input(void) {
    check_combat_eligibility();
    GAME_INPUT_OPTIONS[REST_OPTION].enabled = safe_to_rest;
    GAME_INPUT_OPTIONS[STOP_OPTION].enabled = under_level;
    GAME_INPUT_OPTIONS[ATTACK_OPTION].enabled = !under_level;
    GAME_INPUT_OPTIONS[DEFEND_OPTION].enabled = !under_level;
    /* The options are numbered from 1. */
    GAME_INPUT_TABLE.selection = (under_level ? STOP_OPTION : DEFEND_OPTION) + 1;
    GAME_INPUT_OPTIONS[MOVE_OPTION].enabled = TURN_ACTIVE != 0 ? MOVE_POINTS : 0;
    return GAME_INPUT_OPTIONS[MOVE_OPTION].enabled;
}

struct menu *get_input_table_ptr(void) {
    return &GAME_INPUT_TABLE;
}

uint8_t teardown_input(void) {
    return input_loop(get_input_table_ptr());
}

uint8_t menu_see_your_lot(void) {
    display_char_stats();
    return CHAIN_TEARDOWN_INPUT;
}

/* Attacks the hostile mob in ENTITY_PTR's cell, or opens the menu of
 * directions to attack in when there is none. MENU_CHECK_ATTACK and ENTER_ATTACK_MENU. */
uint8_t menu_check_attack(void) {
    if (check_attack_eligible())
        return CHAIN_FIND_ATTACK_TARGET;
    return input_loop(&ATTACK_DIR_TABLE);
}

uint8_t menu_other_options(void) {
    return input_loop(&OTHER_OPTIONS_TABLE);
}

void retire_character(void) {
    ACTIVE_CHAR = 0;
    reorder_current_char();
    scene_loop(SCENE_RETIRE);
}

/* ---- Settings ---------------------------------------------------------- */

uint8_t menu_game_settings(void) {
    GAME_SETTINGS_TABLE.selection = 1;
    return menu_game_settings_enter();
}

uint8_t menu_game_settings_enter(void) {
    return input_loop(&GAME_SETTINGS_TABLE);
}

/* Writes the game speed and the danger setting into their options.
 * MENU_SETTINGS_INIT. */
static void menu_settings_init(void) {
    settings_display_init();
    S_GAME_SPEED.digit = 0xB0 + ANIM_INDEX;
}

/* Only these two change ANIM_INDEX, which starts at 2, so it stays
 * within MIN_GAME_SPEED and MAX_GAME_SPEED. */
uint8_t settings_speed_up(void) {
    store_selection_to_record();
    if (ANIM_INDEX < MAX_GAME_SPEED)
        ANIM_INDEX++;
    return CHAIN_MENU_GAME_SETTINGS_ENTER;
}

uint8_t settings_speed_down(void) {
    store_selection_to_record();
    if (ANIM_INDEX > MIN_GAME_SPEED)
        ANIM_INDEX--;
    return CHAIN_MENU_GAME_SETTINGS_ENTER;
}

uint8_t settings_danger_up(void) {
    store_selection_to_record();
    inc_threshold_wrap();
    return CHAIN_MENU_GAME_SETTINGS_ENTER;
}

uint8_t settings_danger_down(void) {
    store_selection_to_record();
    dec_threshold_wrap();
    return CHAIN_MENU_GAME_SETTINGS_ENTER;
}

/* ---- Attack target and drop gold --------------------------------------- */

uint8_t menu_attack_target(void) {
    return input_loop(&ATTACK_TARGET_TABLE);
}

uint8_t menu_drop_gold(void) {
    return input_loop(&DROP_GOLD_TABLE);
}

/* '@' leads to ADJ_DROP_INC, which subtracts the step from the amount,
 * and '#' to ADJ_DROP_DEC, which adds it, as in the original. */
uint8_t menu_drop_inc(void) {
    store_selection_to_record();
    return CHAIN_ADJ_DROP_INC;
}

uint8_t menu_drop_dec(void) {
    store_selection_to_record();
    return CHAIN_ADJ_DROP_DEC;
}

/* ---- Save slots ---------------------------------------------------------- */

/* Writes slot_number into the save/load and save menus' strings, with a
 * space for a tens digit of 0. FORMAT_SLOT_NUMBER. */
static void format_slot_number(void) {
    uint8_t tens = 0xA0;
    uint8_t ones = slot_number;
    if (ones >= 10) {
        tens = 0xB1;
        ones -= 10;
    }
    S_CONTINUE_GAME_NUMBER.number[0] = tens;
    S_SAVE_IN_SLOT.number[0] = tens;
    S_CONTINUE_GAME_NUMBER.number[1] = 0xB0 + ones;
    S_SAVE_IN_SLOT.number[1] = 0xB0 + ones;
}

/* Keeps the highlighted option and shows the new slot. SLOT_UPDATE. */
static void slot_update(void) {
    store_selection_to_record();
    format_slot_number();
}

/* Step slot_number up or down, held within 1-16. SLOT_INC and SLOT_DEC. */
static void slot_inc(void) {
    if (slot_number < MAX_SLOT_NUMBER - 1)
        slot_number++;
    slot_update();
}

static void slot_dec(void) {
    if (slot_number > 1)
        slot_number--;
    slot_update();
}

uint8_t init_world(void) {
    return input_loop(&SAVE_LOAD_TABLE);
}

uint8_t slot_inc_enter(void) {
    slot_inc();
    return CHAIN_INIT_WORLD;
}

uint8_t slot_dec_enter(void) {
    slot_dec();
    return CHAIN_INIT_WORLD;
}

uint8_t enter_save_game_slot(void) {
    return input_loop(&SAVE_GAME_SLOT_TABLE);
}

uint8_t slot_dec_reenter(void) {
    slot_dec();
    return CHAIN_ENTER_SAVE_GAME_SLOT;
}

uint8_t slot_inc_reenter(void) {
    slot_inc();
    return CHAIN_ENTER_SAVE_GAME_SLOT;
}

static void call_input_init(uint8_t init) {
    switch (init) {
    case MENU_INIT_CONFIGURE_INPUT:
        configure_input();
        break;
    case MENU_INIT_SETTINGS:
        menu_settings_init();
        break;
    case MENU_INIT_ATTACK:              /* MENU_ATTACK_INIT */
        init_attack_display();
        break;
    case MENU_INIT_DROP_GOLD:           /* MENU_DROP_GOLD_INIT */
        init_drop_display();
        break;
    case MENU_INIT_SAVE_LOAD:           /* SAVE_LOAD_INIT */
    case MENU_INIT_FORMAT_SLOT_NUMBER:
        format_slot_number();
        break;
    }
}

/* ---- Adding a character ------------------------------------------------- */

uint8_t enter_char_select(void) {
    return input_loop(&CHAR_SELECT_TABLE);
}

/* Shows the first player character of the unplaced list as the one to
 * add, or says there is none. */
uint8_t char_select_init(void) {
    if (find_first_pc())
        return char_select_home();
    set_cursor_row21();
    display_message(MSG_SAVED_ME);
    return CHAIN_MOVE_LOOP;
}

uint8_t char_select_next(void) {
    store_selection_to_record();
    find_next_pc_wrap();
    return CHAIN_ENTER_CHAR_SELECT;
}

uint8_t char_select_prev(void) {
    store_selection_to_record();
    find_prev_pc();
    return CHAIN_ENTER_CHAR_SELECT;
}

/* Enables "TO ALI BABA'S HOME" or "TO THIS ROOM", the option for the
 * other destination, highlights the name, and reopens the menu. The
 * shared tail of CHAR_SELECT_HERE and CHAR_SELECT_HOME. */
static uint8_t char_select_patch(uint8_t home_enabled, uint8_t here_enabled) {
    CHAR_SELECT_OPTIONS[ALI_BABAS_HOME_OPTION].enabled = home_enabled;
    CHAR_SELECT_OPTIONS[THIS_ROOM_OPTION].enabled = here_enabled;
    CHAR_SELECT_TABLE.selection = 2;
    return enter_char_select();
}

/* Sends the character to location 1, Ali Baba's home. */
uint8_t char_select_here(void) {
    ACTIVE_CHAR = 1;
    return char_select_patch(1, 0);
}

/* Sends the character to the current location, or to Ali Baba's home
 * from location 9. */
uint8_t char_select_home(void) {
    if (CURRENT_PLAYER == 9)
        return char_select_here();
    ACTIVE_CHAR = CURRENT_PLAYER;
    return char_select_patch(0, 1);
}

uint8_t start_game_input(void) {
    return input_loop(&ALT_INPUT_TABLE);
}

/* ---- The disk ----------------------------------------------------------- */

/* Points the IOB at track 0, sector 0, and the buffer SCENE_TEXT_DATA.
 * SETUP_DATA_IOB. */
static void setup_data_iob(void) {
    RWTS_IOB_PTR->track = 0;
    RWTS_IOB_PTR->sector = 0;
    RWTS_IOB_PTR->buffer_addr = SCENE_TEXT_DATA;
}

/* Returns true unless the disk in the drive is the game disk, whose VTOC
 * sector starts with $03 $0F $10. Tries the read twice. The original
 * returns the carry, set for a disk that isn't the game disk.
 * VERIFY_GAME_DISK. */
static bool verify_game_disk(void) {
    setup_data_iob();
    RWTS_IOB_PTR->track = GAME_SIG_TRACK;
    RWTS_IOB_PTR->sector = GAME_SIG_SECTOR;
    uint8_t tries = 2;
    while (disk_read()) {
        if (--tries == 0)
            return true;
    }
    return SCENE_TEXT_DATA[0] != 0x03 || SCENE_TEXT_DATA[1] != 0x0F
        || SCENE_TEXT_DATA[2] != 0x10;
}

/* Waits for a key, leaving it in KBD. The strobe is cleared before the
 * wait, so READ_KEYBOARD's first strobe clears the key. WAIT_KEYPRESS. */
static void wait_keypress(void) {
    display_message(MSG_PRESS_KEY);
    KBDSTRB = 0;
    while (!(KBD & 0x80)) {
    }
}

/* Prompts for the game disk and waits for a key. PROMPT_INSERT_DISK. */
static void prompt_insert_disk(void) {
    set_cursor_row21();
    display_message(MSG_INSERT_GAME);
    wait_keypress();
}

/* Prompts for the game disk until it is in the drive. ENSURE_GAME_DISK. */
static void ensure_game_disk(void) {
    while (verify_game_disk())
        prompt_insert_disk();
}

/* Formats the disk in the drive. Returns true on an error.
 * FORMAT_DISK_TRACK, whose name says a track. */
static bool format_disk_track(void) {
    return rwts_dispatch(RWTS_FORMAT);
}

/* Writes "ALI" at the start of track 0, sector 0, with three tries.
 * Returns true on an error. WRITE_DISK_SIGNATURE. */
static bool write_disk_signature(void) {
    for (uint8_t i = 0; i < sizeof DISK_SIGNATURE; i++)
        SCENE_TEXT_DATA[i] = DISK_SIGNATURE[i];
    setup_data_iob();
    for (uint8_t tries = 3; tries != 0; tries--) {
        if (!disk_write())
            return false;
    }
    return true;
}

/* Returns true unless track 0, sector 0 starts with "ALI", trying the
 * read three times. VERIFY_DISK_SIGNATURE. */
static bool verify_disk_signature(void) {
    setup_data_iob();
    uint8_t tries = 3;
    while (disk_read()) {
        if (--tries == 0)
            return true;
    }
    for (uint8_t i = 0; i < sizeof DISK_SIGNATURE; i++) {
        if (SCENE_TEXT_DATA[i] != DISK_SIGNATURE[i])
            return true;
    }
    return false;
}

/* Points the IOB at the first sector of slot_number's tracks, with the
 * game's records as the buffer. SETUP_SLOT_IOB. */
static void setup_slot_iob(void) {
    bool scenario = slot_number >= GAME_DISK_SLOTS;
    uint8_t slot = scenario ? slot_number - GAME_DISK_SLOTS : slot_number;
    RWTS_IOB_PTR->track = SAVE_TRACK_BASE[scenario] + 2 * slot;
    RWTS_IOB_PTR->sector = 0;
    RWTS_IOB_PTR->buffer_addr = (uint8_t *)LOCATIONS;
    RWTS_IOB_PTR->partial_byte_count = 0;
}

/* Steps the IOB to the next sector and the next page of the buffer.
 * ADVANCE_SECTOR. */
static void advance_sector(void) {
    RWTS_IOB_PTR->buffer_addr += 0x100;
    if (++RWTS_IOB_PTR->sector >= SECTORS_PER_TRACK) {
        RWTS_IOB_PTR->sector = 0;
        RWTS_IOB_PTR->track++;
    }
}

/* Reads or writes (RWTS_ command) the game's records in slot_number.
 * A failed sector is tried again, but a second failure anywhere in the
 * save ends it. Returns true on an error. The loop of WRITE_GAME_DATA
 * and READ_GAME_DATA, which return 1 on an error in A where no caller
 * reads it. */
static bool transfer_game_data(uint8_t command) {
    uint8_t tries = 2;
    uint8_t left = SAVE_SECTOR_COUNT;
    setup_slot_iob();
    for (;;) {
        if (rwts_dispatch(command)) {
            if (--tries == 0)
                return true;
        } else {
            advance_sector();
            if (--left == 0)
                return false;
        }
    }
}

/* WRITE_GAME_DATA. */
static void write_game_data(void) {
    transfer_game_data(RWTS_WRITE);
}

/* Reads the saved game, and restarts the machine when that fails.
 * READ_GAME_DATA. */
static void read_game_data(void) {
    if (transfer_game_data(RWTS_READ)) {
        set_cursor_row21();
        display_message(MSG_READ_ERROR);
        cold_restart();
    }
}

/* Loads the game saved in slot_number. A slot on the game disk is read
 * from it. For a slot on a scenario disk, which is every slot, the
 * routine prompts for that disk, reads it if its signature matches, and
 * prompts for the game disk again. A wrong disk goes back to the save/load
 * menu. */
uint8_t continue_game(void) {
    if (slot_number < GAME_DISK_SLOTS) {
        ensure_game_disk();
        read_game_data();
        return CHAIN_RETURN;
    }
    set_cursor_row21();
    display_message(MSG_INSERT_SCENARIO);
    wait_keypress();
    if (verify_disk_signature()) {
        set_cursor_row21();
        display_message(MSG_TOUGH_HIDE);
        ensure_game_disk();
        return CHAIN_INIT_WORLD;
    }
    read_game_data();
    prompt_insert_disk();
    ensure_game_disk();
    return CHAIN_RETURN;
}

/* Saves the game in slot_number. For a scenario disk, the first save
 * plays scene $42 first, and the scenario disk menu does the saving. */
uint8_t save_game(void) {
    if (slot_number < GAME_DISK_SLOTS) {
        ensure_game_disk();
        write_game_data();
        return CHAIN_MOVE_LOOP;
    }
    if (scenario_init_flag == 0) {
        ensure_game_disk();
        scene_loop(0x42);
        scenario_init_flag = 1;
    }
    return enter_scenario_disk();
}

uint8_t enter_scenario_disk(void) {
    return input_loop(&SCENARIO_DISK_TABLE);
}

/* Prompts for the game disk again, and returns to the move. The common end
 * of the scenario disk menu's options (.cleanup). */
static uint8_t scenario_done(void) {
    prompt_insert_disk();
    ensure_game_disk();
    return CHAIN_MOVE_LOOP;
}

uint8_t cancel_scenario(void) {
    ensure_game_disk();
    return CHAIN_MOVE_LOOP;
}

/* Prints the message for a disk that couldn't be formatted.
 * DISPLAY_INSERT_MSG. */
static void display_insert_msg(void) {
    set_cursor_row21();
    display_message(MSG_BREAKS_FREE);
}

/* Saves the game on a scenario disk, formatting it and writing its
 * signature unless it already has one. With the game disk still in the
 * drive, prompts for the scenario disk and reopens the menu.
 * SCENARIO_INSERT_PROMPT and FORMAT_SCENARIO_DISK. */
uint8_t format_scenario_disk(void) {
    if (!verify_game_disk()) {
        set_cursor_row21();
        display_message(MSG_INSERT_SCENARIO);
        return enter_scenario_disk();
    }
    if (verify_disk_signature()) {
        if (format_disk_track() || write_disk_signature()) {
            display_insert_msg();
            return scenario_done();
        }
    }
    write_game_data();
    return scenario_done();
}

/* Saves the game on a scenario disk that has its signature. */
uint8_t use_scenario_disk(void) {
    if (verify_disk_signature()) {
        set_cursor_row21();
        display_message(MSG_TOUGH_HIDE);
    } else {
        write_game_data();
    }
    return scenario_done();
}
