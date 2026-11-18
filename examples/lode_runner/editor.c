/*
    editor.c -- the level editor.

    The editor's title screen lists its commands, and each command
    prompts on the lines below it. PLAY LEVEL starts the game on a
    level of the data disk, CLEAR LEVEL empties a level, EDIT LEVEL
    lets the player draw a level with the keys, MOVE LEVEL copies a
    level from one data disk to another, INITIALIZE formats a disk as
    a data disk, and CLEAR SCORE FILE empties the disk's high scores.
    ESC at any prompt abandons the command.

    FORMAT_PATCH keeps INITIALIZE from formatting the master disk. The
    game's DOS has its format code patched to jump to FORMAT_PATCH at
    $8E00, with the slot times 16 in X. FORMAT_PATCH moves the head to
    track 0 and reads bytes looking for D4 D5 D6, a mark that only the
    master disk has. Finding it, it turns the motor off, says why, and
    goes back to the editor's title screen. After 8192 bytes without
    it, it goes on to DOS's format. Here it's format_patch, in a
    section of its own, which the linker script puts at $8E00.

    Departures from the original:

    - The original jumps to EDITOR_COMMAND_LOOP, START_LEVEL_EDITOR,
      EDIT_LEVEL, and EDIT_LEVEL_KEY_LOOP from routines it called with
      JSR, abandoning the stack. Here they're [[noreturn]] functions
      that those routines call.
    - EDITOR_ROUTINE_ADDRESS and LEVEL_EDIT_KEY_FUNCTIONS, which the
      original pushes for an RTS, are switches, and EDITOR_KEYS and
      LEVEL_EDIT_KEY_TABLE are their case labels. A command that jumps
      to EDITOR_COMMAND_LOOP_BEEP beeps and returns to the loop.
    - LEVEL_EDITOR's copy of INDIRECT_TARGET into RWTS_ADDR is left
      out, because this program calls RWTS directly (see level.c).
    - EDITOR_INITIALIZE_DISK's 15 NOPs, where a cracker removed a
      call, are left out.
    - READ_SAVED_INPUT_MODE's immediate operand, which LEVEL_EDITOR
      patches, is saved_input_mode.
    - EDIT_LEVEL's beep when LOAD_LEVEL returns with the carry set is
      left out. LOAD_LEVEL returns only through a BCC, so the carry is
      always clear, and load_level returns nothing.
    - GET_LEVEL_FROM_KEYBOARD returns its carry, and leaves the level
      less 1 in Y, which EDITOR_MOVE_LEVEL stores. That value is
      DISK_LEVEL_LOC, which get_level_from_keyboard has just stored,
      so the caller reads it from there. The number typed is summed in
      16 bits and tested once against 255, where the original tests
      the carry after each addition. The partial sums only grow, so a
      sum overflows one byte if and only if the total does.
    - EDITOR_INITIALIZE_DISK pushes DISK_LEVEL_LOC, and LEVEL_EDIT_SAVE1
      pushes GAME_ROWNUM and GAME_COLNUM. Here they're locals. On its
      error paths LEVEL_EDIT_SAVE1 jumps to EDIT_LEVEL_KEY_LOOP and
      leaves its two bytes on the stack, which level_edit_save1 doesn't
      do.
    - The three messages that clear page 2 and start at its top left
      share start_message.
    - FORMAT_PATCH's six NOPs after JSR SEEKABS are left out.
    - FORMAT_PATCH's count of bytes, Y and $4F in the original, is one
      16-bit count. DOS jumps there with its own Y, but the count
      starts from the Y that SEEKABS leaves. SEEKABS on the game's disk
      ends a seek with the TAY of its last step, which loads 0 for a
      seek toward track 0, and nothing it runs after that changes Y. A
      seek always happens, since DOS's current track is set to $44
      first. So Y is 0, and the count is 256 times $20, 8192 bytes.

    Lode Runner, main.asm lines 5215-6131:

        ORG    $7B84
    LEVEL_EDITOR:
        SUBROUTINE

        LDA     #$00
        STA     SCORE
        STA     SCORE+1
        STA     SCORE+2
        STA     SCORE+3

        LDA     INDIRECT_TARGET
        STA     RWTS_ADDR
        LDA     INDIRECT_TARGET+1
        STA     RWTS_ADDR+1

        LDA     #$05
        STA     LIVES
        STA     GAME_MODE
        LDA     INPUT_MODE
        STA     READ_SAVED_INPUT_MODE+1

        LDA     #KEYBOARD_MODE
        STA     INPUT_MODE

        STA     TXTPAGE1

        LDA     DISK_LEVEL_LOC
        CMP     #$96
        BCC     START_LEVEL_EDITOR
        LDA     #$00
        STA     DISK_LEVEL_LOC

    START_LEVEL_EDITOR:
        JSR     CLEAR_HGR1
        LDA     #$20
        STA     DRAW_PAGE
        LDA     #$00
        STA     GAME_COLNUM
        STA     GAME_ROWNUM

        ; "  LODE RUNNER BOARD EDITOR\r
        ; "----------------------------\r
        ; "  <ESC> ABORTS ANY COMMAND\r"
        JSR     PUT_STRING
        HEX     A0 A0 CC CF C4 C5 A0 D2 D5 CE CE C5 D2 A0 C2 CF
        HEX     C1 D2 C4 A0 C5 C4 C9 D4 CF D2 8D AD AD AD AD AD
        HEX     AD AD AD AD AD AD AD AD AD AD AD AD AD AD AD AD
        HEX     AD AD AD AD AD AD AD 8D A0 A0 BC C5 D3 C3 BE A0
        HEX     C1 C2 CF D2 D4 D3 A0 C1 CE D9 A0 C3 CF CD CD C1
        HEX     CE C4 8D 00

    EDITOR_COMMAND_LOOP:
        LDA     GAME_ROWNUM
        CMP     #$09
        BCS     START_LEVEL_EDITOR

        ; "\r"
        ; "COMMAND>"
        JSR     PUT_STRING
        HEX     8D C3 CF CD CD C1 CE C4 BE 00

        JSR     EDITOR_WAIT_FOR_KEY
        LDX     #$00

    .loop2:
        LDY     EDITOR_KEYS,X
        BEQ     EDITOR_COMMAND_LOOP_BEEP
        CMP     EDITOR_KEYS,X
        BEQ     .end
        INX
        BNE     .loop2

    EDITOR_COMMAND_LOOP_BEEP:
        JSR     BEEP
        JMP     EDITOR_COMMAND_LOOP

    .end:
        TXA
        ASL
        TAX
        LDA     EDITOR_ROUTINE_ADDRESS+1,X
        PHA
        LDA     EDITOR_ROUTINE_ADDRESS,X
        PHA
        RTS


        ORG    $7C4D
    EDITOR_KEYS:
        ; P (Play level)
        ; C (Clear level)
        ; E (Edit level)
        ; M (Move level)
        ; I (Initialize disk)
        ; S (clear high Scores)
        HEX     D0 C3 C5 CD C9 D3 00    ; P C E M I S
    EDITOR_ROUTINE_ADDRESS:
        WORD    EDITOR_PLAY_LEVEL-1
        WORD    EDITOR_CLEAR_LEVEL-1
        WORD    EDITOR_EDIT_LEVEL-1
        WORD    EDITOR_MOVE_LEVEL-1
        WORD    EDITOR_INITIALIZE_DISK-1
        WORD    EDITOR_CLEAR_HIGH_SCORES-1
        ORG    $7C60
    EDITOR_PLAY_LEVEL:
        SUBROUTINE

        ; "\r"
        ; ">>PLAY LEVEL"
        JSR     PUT_STRING
        HEX     8D BE BE D0 CC C1 D9 A0 CC C5 D6 C5 CC 00

        JSR     GET_LEVEL_FROM_KEYBOARD
        BCS     .beep

    READ_SAVED_INPUT_MODE:
        LDA     #$00
        STA     INPUT_MODE
        LDA     #GAME_MODE_PLAY_IN_EDITOR
        STA     GAME_MODE
        LDA     #$01
        STA     $9D
        LDA     DISK_LEVEL_LOC
        BEQ     .init_game_data_
        LSR     $9D

    .init_game_data_:
        JMP     INIT_GAME_DATA

    .beep:
        JMP     EDITOR_COMMAND_LOOP_BEEP

        ORG    $7C8E
    EDITOR_CLEAR_LEVEL:
        SUBROUTINE

        ; "\r"
        ; ">>CLEAR LEVEL"
        JSR     PUT_STRING
        HEX     8D BE BE C3 CC C5 C1 D2 A0 CC C5 D6 C5 CC 00

        JSR     GET_LEVEL_FROM_KEYBOARD
        BCS     .beep
        JSR     CHECK_FOR_VALID_DATA_DISK

        LDY     #$00
        TYA
    .loop:
        STA     DISK_BUFFER,Y
        INY
        BNE     .loop

        LDA     #DISK_ACCESS_WRITE
        JSR     ACCESS_COMPRESSED_LEVEL_DATA      ; write level
        JMP     EDITOR_COMMAND_LOOP

    .beep:
        JMP     EDITOR_COMMAND_LOOP_BEEP

        ORG    $7CBC
    EDITOR_EDIT_LEVEL:
        SUBROUTINE

        ; "\r"
        ; ">>EDIT LEVEL"
        JSR     PUT_STRING
        HEX     8D BE BE C5 C4 C9 D4 A0 CC C5 D6 C5 CC 00

        JSR     GET_LEVEL_FROM_KEYBOARD
        BCS     .beep
        JMP     EDIT_LEVEL

    .beep:
        JMP     EDITOR_COMMAND_LOOP_BEEP
        ORG    $7CD8
    EDITOR_MOVE_LEVEL:
        SUBROUTINE

        ; "\r"
        ; ">>MOVE LEVEL"
        JSR     PUT_STRING
        HEX     8D BE BE CD CF D6 C5 A0 CC C5 D6 C5 CC 00

        JSR     GET_LEVEL_FROM_KEYBOARD
        BCS     .beep
        STY     EDITOR_LEVEL_ENTRY      ; source level

        ; " TO LEVEL"
        JSR     PUT_STRING
        HEX     A0 D4 CF A0 CC C5 D6 C5 CC 00

        JSR     GET_LEVEL_FROM_KEYBOARD
        BCS     .beep
        STY     SAVED_VTOC_DATA         ; convenient place for target level

        ; "\r"
        ; "  SOURCE DISKETTE"
        JSR     PUT_STRING
        HEX     8D A0 A0 D3 CF D5 D2 C3 C5 A0 C4 C9 D3 CB C5 D4 D4 C5 00

        JSR     EDITOR_WAIT_FOR_KEY
        ; Deny and dump user back to editor if not valid data disk
        JSR     CHECK_FOR_VALID_DATA_DISK
        LDA     EDITOR_LEVEL_ENTRY              ; source level
        STA     DISK_LEVEL_LOC
        LDA     #DISK_ACCESS_READ
        JSR     ACCESS_COMPRESSED_LEVEL_DATA      ; read source level

        ; "\r"
        ; "  DESTINATION DISKETTE"
        JSR     PUT_STRING
        HEX     8D A0 A0 C4 C5 D3 D4 C9 CE C1 D4 C9 CF CE A0 C4 C9 D3 CB C5 D4 D4 C5 00

        JSR     EDITOR_WAIT_FOR_KEY
        ; Deny and dump user back to editor if not valid data disk
        JSR     CHECK_FOR_VALID_DATA_DISK
        LDA     SAVED_VTOC_DATA                 ; target level
        STA     DISK_LEVEL_LOC
        LDA     #DISK_ACCESS_WRITE
        JSR     ACCESS_COMPRESSED_LEVEL_DATA      ; write target level
        JMP     EDITOR_COMMAND_LOOP

    .beep:
        JMP     EDITOR_COMMAND_LOOP_BEEP

        ORG    $7D5D
    EDITOR_INITIALIZE_DISK:
        SUBROUTINE

        ; "\r"
        ; ">>INITIALIZE\r"
        ; "  THIS FORMATS THE DISKETTE\r"
        ; "  FOR USER CREATED LEVELS.\r"
        ; "  (CAUTION. IT ERASES THE\r"
        ; "   ENTIRE DISKETTE FIRST)\r"
        ; "\r"
        ; "  ARE YOU SURE (Y/N) "
        JSR     PUT_STRING
        HEX     8D BE BE C9 CE C9 D4 C9 C1 CC C9 DA C5 8D A0 A0
        HEX     D4 C8 C9 D3 A0 C6 CF D2 CD C1 D4 D3 A0 D4 C8 C5
        HEX     A0 C4 C9 D3 CB C5 D4 D4 C5 8D A0 A0 C6 CF D2 A0
        HEX     D5 D3 C5 D2 A0 C3 D2 C5 C1 D4 C5 C4 A0 CC C5 D6
        HEX     C5 CC D3 AE 8D A0 A0 A8 C3 C1 D5 D4 C9 CF CE AE
        HEX     A0 C9 D4 A0 C5 D2 C1 D3 C5 D3 A0 D4 C8 C5 8D A0
        HEX     A0 A0 C5 CE D4 C9 D2 C5 A0 C4 C9 D3 CB C5 D4 D4
        HEX     C5 A0 C6 C9 D2 D3 D4 A9 8D 8D A0 A0 C1 D2 C5 A0
        HEX     D9 CF D5 A0 D3 D5 D2 C5 A0 A8 D9 AF CE A9 A0 00

        JSR     EDITOR_WAIT_FOR_KEY
        CMP     #$D9        ; Y
        BNE     .end

        NOP     ; NOP x 15
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP

        LDA     DISK_LEVEL_LOC
        PHA

        ; Format the disk
        LDA     #DISK_ACCESS_FORMAT
        JSR     ACCESS_COMPRESSED_LEVEL_DATA

        ; Write the boot sector (T0S0)
        LDA     #<DISK_BOOT_SECTOR_DATA
        STA     IOB_READ_WRITE_BUFFER_PTR
        LDA     #>DISK_BOOT_SECTOR_DATA
        STA     IOB_READ_WRITE_BUFFER_PTR+1
        LDA     #$00
        STA     IOB_SECTOR_NUMBER
        STA     IOB_TRACK_NUMBER
        LDA     #$02
        STA     IOB_COMMAND_CODE
        JSR     ACCESS_DISK_OR_RESET_GAME   ; write T0S0 with DISK_BOOT_SECTOR_DATA.

        ; Read the VTOC (T17S0)
        LDA     #$E0
        STA     DISK_LEVEL_LOC              ; ends up being T17S0 (the VTOC)
        LDA     #DISK_ACCESS_READ
        JSR     ACCESS_COMPRESSED_LEVEL_DATA

        ; Copy from SAVED_VTOC_DATA to DISK_BUFFER and write it.
        LDY     #$37
    .loop:
        LDA     SAVED_VTOC_DATA+1,Y
        STA     DISK_BUFFER,Y
        DEY
        BPL     .loop

        LDA     #$02
        JSR     ACCESS_COMPRESSED_LEVEL_DATA

        ; Read the first catalog sector (T17S15)
        LDA     #$EF
        STA     DISK_LEVEL_LOC
        LDA     #DISK_ACCESS_READ
        JSR     ACCESS_COMPRESSED_LEVEL_DATA

        ; Copy from SAVED_FILE_DESCRIPTIVE_ENTRY_DATA the first file descriptive
        ; entry to DISK_BUFFER and write it.
        LDY     #$20
    .loop2:
        LDA     SAVED_FILE_DESCRIPTIVE_ENTRY_DATA,Y
        STA     DISK_BUFFER+11,Y
        DEY
        BPL     .loop2

        ; Write it back
        LDA     #DISK_ACCESS_WRITE
        JSR     ACCESS_COMPRESSED_LEVEL_DATA

        ; Read the high score sector
        LDA     #$01
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK

        ; Copy from HI_SCORE_DATA_MARKER and write it.
        LDY     #$0A
    .loop3:
        LDA     HI_SCORE_DATA_MARKER,Y
        STA     $1FF4,Y
        DEY
        BPL     .loop3

        ; Write it back
        LDA     #$02
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK

        PLA
        STA     DISK_LEVEL_LOC
    .end:
        JMP     EDITOR_COMMAND_LOOP

        ORG    $7E75
    EDITOR_CLEAR_HIGH_SCORES:
        SUBROUTINE

        ; "\r"
        ; ">>CLEAR SCORE FILE\r"
        ; "  THIS CLEARS THE HIGH\r"
        ; "  SCORE FILE OF ALL\r"
        ; "  ENTRIES.\r"
        ; "\r"
        ; "  ARE YOU SURE (Y/N) "
        JSR     PUT_STRING
        HEX     8D BE BE C3 CC C5 C1 D2 A0 D3 C3 CF D2 C5 A0 C6
        HEX     C9 CC C5 8D A0 A0 D4 C8 C9 D3 A0 C3 CC C5 C1 D2
        HEX     D3 A0 D4 C8 C5 A0 C8 C9 C7 C8 8D A0 A0 D3 C3 CF
        HEX     D2 C5 A0 C6 C9 CC C5 A0 CF C6 A0 C1 CC CC 8D A0
        HEX     A0 C5 CE D4 D2 C9 C5 D3 AE 8D 8D A0 A0 C1 D2 C5
        HEX     A0 D9 CF D5 A0 D3 D5 D2 C5 A0 A8 D9 AF CE A9 A0
        HEX     00

        JSR     EDITOR_WAIT_FOR_KEY
        CMP     #$D9        ; 'Y'
        BNE     .end

        LDA     #$01
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK      ; read table
        CMP     #$00
        BNE     .good_disk
        JSR     BAD_DATA_DISK
        JMP     START_LEVEL_EDITOR

    .good_disk:
        LDY     #$4F
        LDA     #$00

    .loop:
        STA     HI_SCORE_DATA,Y
        DEY
        BPL     .loop

        LDA     #$02
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK      ; write table

    .end:
        JMP     EDITOR_COMMAND_LOOP

        ORG    $7F01
    EDIT_LEVEL:
        SUBROUTINE

        JSR     CLEAR_HGR2
        LDA     #$40
        STA     DRAW_PAGE
        JSR     PUT_STATUS_DRAW
        LDA     #$20
        STA     DRAW_PAGE

        JSR     CHECK_FOR_VALID_DATA_DISK
        LDX     #$01
        STX     $AD
        DEX
        JSR     LOAD_LEVEL
        BCC     .start_editing
        JMP     EDITOR_COMMAND_LOOP_BEEP

    .start_editing:
        LDA     #$00
        STA     GAME_COLNUM
        STA     GAME_ROWNUM

    EDIT_LEVEL_KEY_LOOP:
        JSR     GET_KEY_FOR_EDIT_LEVEL
        CMP     #$BA
        BCS     .store_sprite_num       ; key >= '9'+1
        CMP     #$B0
        BCC     .store_sprite_num       ; key < '0'
        ; key is digit

        AND     #$0F
        STA     SPRITE_NUM
        LDY     GAME_ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     GAME_COLNUM
        LDA     SPRITE_NUM
        EOR     (PTR1),Y
        BEQ     .store_sprite
        LSR     $AD

    .store_sprite:
        LDA     SPRITE_NUM
        STA     (PTR1),Y
        JSR     DRAW_SPRITE_PAGE1
        JMP     EDIT_LEVEL_KEY_LOOP

    .store_sprite_num:
        STA     SPRITE_NUM
        LDY     #$FF

    .loop2:
        INY
        LDA     LEVEL_EDIT_KEY_TABLE,Y
        BEQ     EDIT_LEVEL_BEEP

        CMP     SPRITE_NUM
        BNE     .loop2

        TYA
        ASL
        TAY
        LDA     LEVEL_EDIT_KEY_FUNCTIONS+1,Y
        PHA
        LDA     LEVEL_EDIT_KEY_FUNCTIONS,Y
        PHA
        RTS

    EDIT_LEVEL_BEEP:
        JSR     BEEP
        JMP     EDIT_LEVEL_KEY_LOOP

        ORG    $7F74
    LEVEL_EDIT_UP:
        SUBROUTINE

        LDA     GAME_ROWNUM
        BEQ     EDIT_LEVEL_BEEP         ; nope!

        DEC     GAME_ROWNUM
        BPL     EDIT_LEVEL_KEY_LOOP     ; unconditional

    LEVEL_EDIT_LEFT:
        SUBROUTINE

        LDA     GAME_COLNUM
        BEQ     EDIT_LEVEL_BEEP         ; nope!

        DEC     GAME_COLNUM
        BPL     EDIT_LEVEL_KEY_LOOP     ; unconditional

    LEVEL_EDIT_RIGHT:
        SUBROUTINE

        LDA     GAME_COLNUM
        CMP     #MAX_GAME_COL
        BCS     EDIT_LEVEL_BEEP         ; nope!

        INC     GAME_COLNUM
        BNE     EDIT_LEVEL_KEY_LOOP     ; unconditional

    LEVEL_EDIT_DOWN:
        SUBROUTINE

        LDA     GAME_ROWNUM
        CMP     #MAX_GAME_ROW
        BCS     EDIT_LEVEL_BEEP         ; nope!

        INC     GAME_ROWNUM
        BNE     EDIT_LEVEL_KEY_LOOP     ; unconditional

    LEVEL_EDIT_SAVE1:
        SUBROUTINE

        LDA     GAME_ROWNUM
        PHA
        LDA     GAME_COLNUM
        PHA
        LDA     #$01
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK
        CMP     #$00
        BNE     .check_for_master_disk

        JSR     BAD_DATA_DISK
        JMP     .end

    .check_for_master_disk:
        CMP     #$01
        BNE     .save_data

        JSR     DONT_MANIPULATE_MASTER_DISK
        JMP     .end

    .save_data:
        JSR     COMPRESS_AND_SAVE_LEVEL_DATA
        PLA
        STA     GAME_COLNUM
        PLA
        STA     GAME_ROWNUM
        LDA     #$01
        STA     $AD
        RTS

    .end:
        LDA     #$00
        STA     GAME_COLNUM
        STA     GAME_ROWNUM
        JMP     EDIT_LEVEL_KEY_LOOP

    LEVEL_EDIT_SAVE:
        SUBROUTINE

        JSR     LEVEL_EDIT_SAVE1
        JMP     EDIT_LEVEL_KEY_LOOP

    LEVEL_EDIT_RIGHT_ARROW:
        SUBROUTINE

        LDA     DISK_LEVEL_LOC
        CMP     #$95

    LEVEL_EDIT_RIGHT_ARROW_CHECK:
        BEQ     EDIT_LEVEL_BEEP
        JSR     LEVEL_EDIT_CHANGE_LEVEL
        INC     DISK_LEVEL_LOC
        INC     LEVELNUM
        JMP     EDIT_LEVEL

    LEVEL_EDIT_LEFT_ARROW:
        SUBROUTINE

        LDA     DISK_LEVEL_LOC
        BEQ     LEVEL_EDIT_RIGHT_ARROW_CHECK
        JSR     LEVEL_EDIT_CHANGE_LEVEL
        DEC     LEVELNUM
        DEC     DISK_LEVEL_LOC
        JMP     EDIT_LEVEL

    LEVEL_EDIT_Q:
        SUBROUTINE

        JSR     LEVEL_EDIT_CHANGE_LEVEL
        JMP     START_LEVEL_EDITOR

    LEVEL_EDIT_CHANGE_LEVEL:
        SUBROUTINE

        LDA     $AD
        BNE     .end
        JSR     CLEAR_HGR2
        LDA     #$40
        STA     DRAW_PAGE
        LDA     #$00
        STA     GAME_COLNUM
        STA     GAME_ROWNUM

        ; "LEVEL HAS BEEN CHANGED BUT\r"
        ; "NOT SAVED. DO YOU WISH TO\r"
        ; "SAVE MODIFIED LEVEL (Y/N) "
        JSR     PUT_STRING
        HEX     CC C5 D6 C5 CC A0 C8 C1 D3 A0 C2 C5 C5 CE A0 C3
        HEX     C8 C1 CE C7 C5 C4 A0 C2 D5 D4 8D CE CF D4 A0 D3
        HEX     C1 D6 C5 C4 AE A0 C4 CF A0 D9 CF D5 A0 D7 C9 D3
        HEX     C8 A0 D4 CF 8D D3 C1 D6 C5 A0 CD CF C4 C9 C6 C9
        HEX     C5 C4 A0 CC C5 D6 C5 CC A0 A8 D9 AF CE A9 A0 00

        JSR     BEEP
        STA     TXTPAGE2

    .loop:
        LDA     #$00
        JSR     WAIT_FOR_KEY
        STA     KBDSTRB
        CMP     #$CE        ; 'N'
        BEQ     .end

        CMP     #$D9        ; 'Y'
        BNE     .loop

        JSR     LEVEL_EDIT_SAVE1

    .end:
        STA     TXTPAGE1
        LDA     #$00
        STA     GAME_COLNUM
        STA     GAME_ROWNUM
        RTS


        ORG    $807F
    CHECK_FOR_VALID_DATA_DISK:
        SUBROUTINE

        LDA     #$01
        JSR     ACCESS_HI_SCORE_DATA_FROM_DISK      ; read table
        CMP     #$00        ; bad table
        BNE     .check_for_master_disk

        JSR     BAD_DATA_DISK
        JMP     START_LEVEL_EDITOR

    .check_for_master_disk:
        CMP     #$01        ; master disk
        BNE     RETURN_FROM_SUBROUTINE

        JSR     DONT_MANIPULATE_MASTER_DISK
        JMP     START_LEVEL_EDITOR

        ORG    $8098
    DONT_MANIPULATE_MASTER_DISK:
        SUBROUTINE

        JSR     CLEAR_HGR2
        LDA     #$40
        STA     DRAW_PAGE
        LDA     #$00
        STA     GAME_COLNUM
        STA     GAME_ROWNUM

        ; "USER NOT ALLOWED TO\r"
        ; "MANIPULATE MASTER DISKETTE."
        JSR     PUT_STRING
        HEX     D5 D3 C5 D2 A0 CE CF D4 A0 C1 CC CC CF D7 C5 C4
        HEX     A0 D4 CF 8D CD C1 CE C9 D0 D5 CC C1 D4 C5 A0 CD
        HEX     C1 D3 D4 C5 D2 A0 C4 C9 D3 CB C5 D4 D4 C5 AE 00

        ; fallthrough to HIT_KEY_TO_CONTINUE

        ORG    $80D8
    HIT_KEY_TO_CONTINUE:
        SUBROUTINE

        ; "\r"
        ; "\r"
        ; "HIT A KEY TO CONTINUE "
        JSR     PUT_STRING
        HEX     8D 8D C8 C9 D4 A0 C1 A0 CB C5 D9 A0 D4 CF A0 C3
        HEX     CF CE D4 C9 CE D5 C5 A0 00

        JSR     BEEP
        STA     TXTPAGE2
        LDA     #$00
        JSR     WAIT_FOR_KEY
        STA     KBDSTRB
        STA     TXTPAGE1
    RETURN_FROM_SUBROUTINE:
        RTS

        ORG    $8106
    BAD_DATA_DISK:
        SUBROUTINE

        JSR     CLEAR_HGR2
        LDA     #$40
        STA     DRAW_PAGE
        LDA     #$00
        STA     GAME_COLNUM
        STA     GAME_ROWNUM

        ; "DISKETTE IN DRIVE IS NOT A\r"
        ; "LODE RUNNER DATA DISK."
        JSR     PUT_STRING
        HEX     C4 C9 D3 CB C5 D4 D4 C5 A0 C9 CE A0 C4 D2 C9 D6
        HEX     C5 A0 C9 D3 A0 CE CF D4 A0 C1 8D CC CF C4 C5 A0
        HEX     D2 D5 CE CE C5 D2 A0 C4 C1 D4 C1 A0 C4 C9 D3 CB
        HEX     AE 00

        JMP     HIT_KEY_TO_CONTINUE

        ORG    $814B
    GET_KEY_FOR_EDIT_LEVEL:
        SUBROUTINE

        LDY     GAME_ROWNUM
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_OFFSETS,Y
        STA     PTR1
        LDA     CURR_LEVEL_ROW_SPRITES_PTR_PAGES,Y
        STA     PTR1+1

        LDY     GAME_COLNUM
        LDA     (PTR1),Y
        JSR     WAIT_FOR_KEY_WITH_CURSOR_PAGE_1
        STA     KBDSTRB
        RTS

        ORG    $8162
    LEVEL_EDIT_KEY_TABLE:
        ; J (left)
        ; I (up)
        ; K (right)
        ; M (down)
        ; ctrl-S (save)
        ; right arrow
        ; left arrow
        ; ctrl-Q
        HEX     CA C9 CB CD 93 95 88 91 00
    LEVEL_EDIT_KEY_FUNCTIONS:
        WORD    LEVEL_EDIT_LEFT-1
        WORD    LEVEL_EDIT_UP-1
        WORD    LEVEL_EDIT_RIGHT-1
        WORD    LEVEL_EDIT_DOWN-1
        WORD    LEVEL_EDIT_SAVE-1
        WORD    LEVEL_EDIT_RIGHT_ARROW-1
        WORD    LEVEL_EDIT_LEFT_ARROW-1
        WORD    LEVEL_EDIT_Q-1


        ORG    $817B
    GET_LEVEL_FROM_KEYBOARD:
        SUBROUTINE

        LDY     DISK_LEVEL_LOC
        INY
        TYA
        JSR     TO_DECIMAL3     ; make 1-based
        LDA     GAME_COLNUM
        STA     SAVED_GAME_COLNUM
        LDY     #$00

        ; Print current level
    .loop:
        LDA     HUNDREDS,Y
        STY     KBD_ENTRY_INDEX     ; save Y
        JSR     PUT_DIGIT
        LDY     KBD_ENTRY_INDEX     ; restore Y
        INY
        CPY     #$03
        BCC     .loop

        LDA     SAVED_GAME_COLNUM
        STA     GAME_COLNUM
        LDY     #$00
        STY     KBD_ENTRY_INDEX

    .loop2
        LDX     KBD_ENTRY_INDEX
        LDA     HUNDREDS,X
        CLC
        ADC     #$3B            ; sprite = '0' + X
        JSR     WAIT_FOR_KEY_WITH_CURSOR_PAGE_1
        STA     KBDSTRB
        CMP     #$8D            ; return
        BEQ     .return_pressed

        CMP     #$88            ; backspace
        BNE     .check_for_fwd_arrow

        LDX     KBD_ENTRY_INDEX
        BEQ     .beep           ; can't backspace past the beginning

        DEC     KBD_ENTRY_INDEX
        DEC     GAME_COLNUM
        JMP     .loop2

    .check_for_fwd_arrow:
        CMP     #$95            ; fwd arrow
        BNE     .check_for_escape

        LDX     KBD_ENTRY_INDEX
        CPX     #$02
        BEQ     .beep           ; can't fwd past the end

        INC     GAME_COLNUM
        INC     KBD_ENTRY_INDEX
        JMP     .loop2

    .check_for_escape:
        CMP     #$9B            ; ESC
        BNE     .check_for_digit
        JMP     EDITOR_COMMAND_LOOP

    .check_for_digit:
        CMP     #$B0            ; '0'
        BCC     .beep           ; less than '0' not allowed
        CMP     #$BA            ; '9'+1
        BCS     .beep           ; greater than '9' not allowed

        SEC
        SBC     #$B0            ; char - '0'
        LDY     KBD_ENTRY_INDEX
        STA     HUNDREDS,Y
        JSR     PUT_DIGIT
        INC     KBD_ENTRY_INDEX
        LDA     KBD_ENTRY_INDEX
        CMP     #$03
        BCC     .loop2

        ; Don't allow a fourth digit
        DEC     KBD_ENTRY_INDEX
        DEC     GAME_COLNUM
        JMP     .loop2

    .beep:
        JSR     BEEP
        JMP     .loop2

    .return_pressed:
        LDA     SAVED_GAME_COLNUM
        CLC
        ADC     #$03
        STA     GAME_COLNUM
        LDA     #$00
        LDX     HUNDREDS
        BEQ     .add_tens

        CLC
    .loop_hundreds:
        ADC     #100
        BCS     .end
        DEX
        BNE     .loop_hundreds

    .add_tens:
        LDX     TENS
        BEQ     .add_units

        CLC
    .loop_tens:
        ADC     #10
        BCS     .end
        DEX
        BNE     .loop_tens

    .add_units:
        CLC
        ADC     UNITS
        BCS     .end

        STA     LEVELNUM
        TAY
        DEY
        STY     DISK_LEVEL_LOC
        CPY     #$96

    .end:
        RTS


        ; Player movement routines

        ORG    $823D
    EDITOR_WAIT_FOR_KEY:
        SUBROUTINE

        LDA     #$00
        JSR     WAIT_FOR_KEY_WITH_CURSOR_PAGE_1
        STA     KBDSTRB
        CMP     #$9B        ; ESC
        BNE     .return
        JMP     EDITOR_COMMAND_LOOP

    .return
        RTS

        ORG    $824D
    KBD_ENTRY_INDEX:
        HEX     60

        ORG    $824E
    SAVED_GAME_COLNUM:
        HEX     85
        ORG    $824F
    EDITOR_LEVEL_ENTRY:
        HEX     0F
    SRC_PTR     EQU     $3C     ; 2 bytes
    DEST_PTR    EQU     $3E     ; 2 bytes

        ORG    $8250
    SAVED_VTOC_DATA:
        HEX     60 02 11 0F 04 00 00 FE 00 00 00 00 00 00 00 00
        HEX     00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        HEX     00 00 00 00 00 00 00 00 7A 00 00 00 00 00 00 00
        HEX     00 FF FF 00 00 23 0F 00 01

        ORG    $8289
    SAVED_FILE_DESCRIPTIVE_ENTRY_DATA:
        HEX     22 ; Track of first track/sector list sector (T34)
        HEX     0F ; Sector of first track/sector list sector (S15)
        HEX     88 ; File type and flags: locked, S-type file
        ; File name: "^H^H^H^H^H^H^HLODE RUNNER DATA DISK  "
        HEX     88 88 88 88 88 88 88 CC CF C4 C5 A0 D2 D5 CE CE
        HEX     C5 D2 A0 C4 C1 D4 C1 A0 C4 C9 D3 CB A0 A0

    Lode Runner, main.asm lines 8147-8199:

        ORG    $8E00
    FORMAT_PATCH:
        LDA     #$44
        STA     $0478       ; Used by DOS as current track.
        LDA     #$00
        JSR     SEEKABS     ; DOS routine to move disk to track in A.
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        LDA     #$20
        STA     $4F

    .await_D4_D5_D6:
        DEY
        BNE     .read_byte1
        DEC     $4F
        BNE     .read_byte1
        JMP     DSKFORM     ; DOS routine to format disk

        HEX     EA EA

    .read_byte1:
        LDA     $C08C,X
        BPL     .read_byte1

    .check_for_D4:
        CMP     #$D4
        BNE     .await_D4_D5_D6
        NOP

    .read_byte2:
        LDA     $C08C,X
        BPL     .read_byte2

    .check_for_D5:
        CMP     #$D5
        BNE     .check_for_D4
        NOP

    .read_byte3:
        LDA     $C08C,X
        BPL     .read_byte3

        CMP     #$D6
        BNE     .check_for_D5

        LDA     $C088,X         ; Turn motor off
        JSR     DONT_MANIPULATE_MASTER_DISK
        JMP     START_LEVEL_EDITOR
*/
#include "lode_runner.h"

/* Keys, in high-bit ASCII as the keyboard returns them. */
#define KEY(c) ((c) | 0x80)
enum {
    KEY_LEFT_ARROW = 0x88,      /* also backspace */
    KEY_RETURN = 0x8D,
    KEY_CTRL_Q = 0x91,
    KEY_CTRL_S = 0x93,
    KEY_RIGHT_ARROW = 0x95,
    KEY_ESC = 0x9B,
};

/* The input mode the player had before the editor, which PLAY LEVEL
 * gives back. */
static uint8_t saved_input_mode = 0x00;

/* The digit get_level_from_keyboard's cursor is on, 0-2, and the
 * column its number starts at. */
static uint8_t kbd_entry_index = 0x60;
static uint8_t saved_game_colnum = 0x85;

/* MOVE LEVEL's source level, as DISK_LEVEL_LOC. */
static uint8_t editor_level_entry = 0x0F;

/* The first 0x38 bytes of a data disk's VTOC (track 17, sector 0),
 * from saved_vtoc_data[1] on. MOVE LEVEL keeps the target level in
 * saved_vtoc_data[0], which INITIALIZE doesn't copy. */
static uint8_t saved_vtoc_data[0x39] = {
    0x60, 0x02, 0x11, 0x0F, 0x04, 0x00, 0x00, 0xFE,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0x00, 0x00, 0x23, 0x0F, 0x00,
    0x01,
};

/* The catalog's first file entry on a data disk, a locked S file
 * whose track/sector list is at track 34, sector 15. Its name starts
 * with seven backspaces, which CATALOG prints, so that the name shows
 * as "LODE RUNNER DATA DISK". */
static const uint8_t SAVED_FILE_DESCRIPTIVE_ENTRY_DATA[0x21] = {
    0x22, 0x0F, 0x88,
    0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0xCC,
    0xCF, 0xC4, 0xC5, 0xA0, 0xD2, 0xD5, 0xCE, 0xCE,
    0xC5, 0xD2, 0xA0, 0xC4, 0xC1, 0xD4, 0xC1, 0xA0,
    0xC4, 0xC9, 0xD3, 0xCB, 0xA0, 0xA0,
};

[[noreturn]] static void edit_level(void);
[[noreturn]] static void edit_level_key_loop(void);

/* Waits for a key with a cursor on page 1, and returns it. ESC
 * abandons the command for the command loop. */
static uint8_t editor_wait_for_key(void) {
    uint8_t key = wait_for_key_with_cursor_page_1(0);
    KBDSTRB = 0;
    if (key == KEY_ESC)
        editor_command_loop();
    return key;
}

/* Clears page 2 and puts the cursor at its top left, for a message
 * that page 2 shows. */
static void start_message(void) {
    clear_hgr2();
    DRAW_PAGE = 0x40;
    GAME_COLNUM = 0;
    GAME_ROWNUM = 0;
}

/* Finishes a message on page 2, shows page 2 until a key is pressed,
 * and goes back to page 1. */
static void hit_key_to_continue(void) {
    put_string("\r\rHIT A KEY TO CONTINUE ");
    beep();
    TXTPAGE2 = 0;
    wait_for_key(0);
    KBDSTRB = 0;
    TXTPAGE1 = 0;
}

static void dont_manipulate_master_disk(void) {
    start_message();
    put_string("USER NOT ALLOWED TO\rMANIPULATE MASTER DISKETTE.");
    hit_key_to_continue();
}

void bad_data_disk(void) {
    start_message();
    put_string("DISKETTE IN DRIVE IS NOT A\rLODE RUNNER DATA DISK.");
    hit_key_to_continue();
}

/* Reads the high-score sector, and goes back to the editor's title
 * screen if the disk in the drive isn't a data disk or is the master
 * disk. */
static void check_for_valid_data_disk(void) {
    uint8_t disk = access_hi_score_data_from_disk(DISK_ACCESS_READ);
    if (disk == 0) {
        bad_data_disk();
        start_level_editor();
    }
    if (disk == 1) {
        dont_manipulate_master_disk();
        start_level_editor();
    }
}

/* Shows the current level's number at the cursor and lets the player
 * type a new one over it, with the arrows moving between the three
 * digits. ESC abandons the command.
 *
 * Returns true when the number typed isn't a level, and false when it
 * is. A level becomes LEVELNUM, and less 1, DISK_LEVEL_LOC. A number
 * from 0 to 255 is stored in both even when it isn't a level, so 0
 * leaves DISK_LEVEL_LOC at 0xFF. A number over 255 changes neither. */
static bool get_level_from_keyboard(void) {
    to_decimal3(DISK_LEVEL_LOC + 1);
    saved_game_colnum = GAME_COLNUM;
    for (kbd_entry_index = 0; kbd_entry_index < 3; kbd_entry_index++)
        put_digit(DIGITS[kbd_entry_index]);
    GAME_COLNUM = saved_game_colnum;
    kbd_entry_index = 0;

    for (;;) {
        uint8_t key = wait_for_key_with_cursor_page_1(
            SPRITE_DIGIT0 + DIGITS[kbd_entry_index]);
        KBDSTRB = 0;
        if (key == KEY_RETURN)
            break;
        switch (key) {
        case KEY_LEFT_ARROW:
            if (kbd_entry_index == 0)
                break;
            kbd_entry_index--;
            GAME_COLNUM--;
            continue;
        case KEY_RIGHT_ARROW:
            if (kbd_entry_index == 2)
                break;
            GAME_COLNUM++;
            kbd_entry_index++;
            continue;
        case KEY_ESC:
            editor_command_loop();
        default:;
            uint8_t digit = key - KEY('0');
            if (digit > 9)
                break;
            DIGITS[kbd_entry_index] = digit;
            put_digit(digit);
            /* The cursor stays on the last digit. */
            if (++kbd_entry_index >= 3) {
                kbd_entry_index--;
                GAME_COLNUM--;
            }
            continue;
        }
        beep();
    }

    GAME_COLNUM = saved_game_colnum + 3;
    uint16_t level = 0;
    for (uint8_t n = DIGITS[0]; n != 0; n--)
        level += 100;
    for (uint8_t n = DIGITS[1]; n != 0; n--)
        level += 10;
    level += DIGITS[2];
    if (level > 255)
        return true;
    LEVELNUM = level;
    DISK_LEVEL_LOC = level - 1;
    return DISK_LEVEL_LOC >= 150;
}

void level_editor(void) {
    for (uint8_t i = 0; i < 4; i++)
        SCORE[i] = 0;
    LIVES = 5;
    GAME_MODE = GAME_MODE_LEVEL_EDITOR;
    saved_input_mode = INPUT_MODE;
    INPUT_MODE = KEYBOARD_MODE;
    TXTPAGE1 = 0;
    if (DISK_LEVEL_LOC >= 150)
        DISK_LEVEL_LOC = 0;
    start_level_editor();
}

void start_level_editor(void) {
    clear_hgr1();
    DRAW_PAGE = 0x20;
    GAME_COLNUM = 0;
    GAME_ROWNUM = 0;
    put_string("  LODE RUNNER BOARD EDITOR\r"
               "----------------------------\r"
               "  <ESC> ABORTS ANY COMMAND\r");
    editor_command_loop();
}

/* Starts the game on the level typed, with the player's own input
 * mode. Only a game started on level 1 can make the high scores. */
static void editor_play_level(void) {
    put_string("\r>>PLAY LEVEL");
    if (get_level_from_keyboard()) {
        beep();
        return;
    }
    INPUT_MODE = saved_input_mode;
    GAME_MODE = GAME_MODE_PLAY_IN_EDITOR;
    HI_SCORE_ELIGIBLE = DISK_LEVEL_LOC == 0;
    init_game_data();
}

static void editor_clear_level(void) {
    put_string("\r>>CLEAR LEVEL");
    if (get_level_from_keyboard()) {
        beep();
        return;
    }
    check_for_valid_data_disk();
    uint8_t i = 0;
    do {
        DISK_BUFFER[i] = 0;
    } while (++i != 0);
    access_compressed_level_data(DISK_ACCESS_WRITE);
}

static void editor_edit_level(void) {
    put_string("\r>>EDIT LEVEL");
    if (get_level_from_keyboard()) {
        beep();
        return;
    }
    edit_level();
}

/* Copies a level, possibly to another disk. Each disk is checked after
 * the player says it's in the drive. */
static void editor_move_level(void) {
    put_string("\r>>MOVE LEVEL");
    if (get_level_from_keyboard()) {
        beep();
        return;
    }
    editor_level_entry = DISK_LEVEL_LOC;
    put_string(" TO LEVEL");
    if (get_level_from_keyboard()) {
        beep();
        return;
    }
    saved_vtoc_data[0] = DISK_LEVEL_LOC;

    put_string("\r  SOURCE DISKETTE");
    editor_wait_for_key();
    check_for_valid_data_disk();
    DISK_LEVEL_LOC = editor_level_entry;
    access_compressed_level_data(DISK_ACCESS_READ);

    put_string("\r  DESTINATION DISKETTE");
    editor_wait_for_key();
    check_for_valid_data_disk();
    DISK_LEVEL_LOC = saved_vtoc_data[0];
    access_compressed_level_data(DISK_ACCESS_WRITE);
}

/* Formats the disk and makes it a data disk. It writes the boot
 * sector, a VTOC, a catalog that lists one locked file, and the
 * high-score sector's mark. */
static void editor_initialize_disk(void) {
    put_string("\r>>INITIALIZE\r"
               "  THIS FORMATS THE DISKETTE\r"
               "  FOR USER CREATED LEVELS.\r"
               "  (CAUTION. IT ERASES THE\r"
               "   ENTIRE DISKETTE FIRST)\r"
               "\r"
               "  ARE YOU SURE (Y/N) ");
    if (editor_wait_for_key() != KEY('Y'))
        return;

    uint8_t saved_level = DISK_LEVEL_LOC;
    access_compressed_level_data(DISK_ACCESS_FORMAT);

    DOS_IOB.buffer_addr = (uint8_t *)DISK_BOOT_SECTOR_DATA;
    DOS_IOB.sector = 0;
    DOS_IOB.track = 0;
    DOS_IOB.command = DISK_ACCESS_WRITE;
    access_disk_or_reset_game();

    /* Level 0xE0 is track 3 + 0xE, sector 0, the VTOC. */
    DISK_LEVEL_LOC = 0xE0;
    access_compressed_level_data(DISK_ACCESS_READ);
    for (uint8_t i = 0; i < 0x38; i++)
        DISK_BUFFER[i] = saved_vtoc_data[i + 1];
    access_compressed_level_data(DISK_ACCESS_WRITE);

    /* Level 0xEF is track 17, sector 15, the catalog's first sector.
     * Its first file entry is at byte 11. */
    DISK_LEVEL_LOC = 0xEF;
    access_compressed_level_data(DISK_ACCESS_READ);
    for (uint8_t i = 0; i < sizeof SAVED_FILE_DESCRIPTIVE_ENTRY_DATA; i++)
        DISK_BUFFER[11 + i] = SAVED_FILE_DESCRIPTIVE_ENTRY_DATA[i];
    access_compressed_level_data(DISK_ACCESS_WRITE);

    access_hi_score_data_from_disk(DISK_ACCESS_READ);
    for (uint8_t i = 0; i < sizeof HI_SCORE_MARKER; i++)
        HI_SCORE_MARKER[i] = HI_SCORE_DATA_MARKER[i];
    access_hi_score_data_from_disk(DISK_ACCESS_WRITE);

    DISK_LEVEL_LOC = saved_level;
}

/* Empties the high-score table. Only a disk that isn't a data disk is
 * refused, so the master disk's table is cleared too. */
static void editor_clear_high_scores(void) {
    put_string("\r>>CLEAR SCORE FILE\r"
               "  THIS CLEARS THE HIGH\r"
               "  SCORE FILE OF ALL\r"
               "  ENTRIES.\r"
               "\r"
               "  ARE YOU SURE (Y/N) ");
    if (editor_wait_for_key() != KEY('Y'))
        return;

    if (access_hi_score_data_from_disk(DISK_ACCESS_READ) == 0) {
        bad_data_disk();
        start_level_editor();
    }
    uint8_t *scores = (uint8_t *)HI_SCORES;
    for (uint8_t i = 0; i < sizeof HI_SCORES; i++)
        scores[i] = 0;
    access_hi_score_data_from_disk(DISK_ACCESS_WRITE);
}

void editor_command_loop(void) {
    for (;;) {
        /* The title screen is redrawn once the prompts fill it. */
        if (GAME_ROWNUM >= 9)
            start_level_editor();
        put_string("\rCOMMAND>");
        switch (editor_wait_for_key()) {
        case KEY('P'):
            editor_play_level();
            break;
        case KEY('C'):
            editor_clear_level();
            break;
        case KEY('E'):
            editor_edit_level();
            break;
        case KEY('M'):
            editor_move_level();
            break;
        case KEY('I'):
            editor_initialize_disk();
            break;
        case KEY('S'):
            editor_clear_high_scores();
            break;
        default:
            beep();
            break;
        }
    }
}

/* Draws page 2's status line, loads the level from a data disk, and
 * starts editing it with the cursor at the top left. */
static void edit_level(void) {
    clear_hgr2();
    DRAW_PAGE = 0x40;
    put_status_draw();
    DRAW_PAGE = 0x20;
    check_for_valid_data_disk();
    LEVEL_SAVED = 1;
    load_level(false);
    GAME_COLNUM = 0;
    GAME_ROWNUM = 0;
    edit_level_key_loop();
}

/* Saves the level, and returns with the cursor where it was. When the
 * disk isn't a data disk or is the master disk, it shows why and goes
 * back to editing with the cursor at the top left. */
static void level_edit_save1(void) {
    uint8_t row = GAME_ROWNUM;
    uint8_t col = GAME_COLNUM;
    uint8_t disk = access_hi_score_data_from_disk(DISK_ACCESS_READ);
    if (disk == 0) {
        bad_data_disk();
    } else if (disk == 1) {
        dont_manipulate_master_disk();
    } else {
        compress_and_save_level_data();
        GAME_COLNUM = col;
        GAME_ROWNUM = row;
        LEVEL_SAVED = 1;
        return;
    }
    GAME_COLNUM = 0;
    GAME_ROWNUM = 0;
    edit_level_key_loop();
}

/* Asks whether to save the level if it has changed since it was
 * saved, and saves it if the player says so. Either way it shows page
 * 1 and puts the cursor at the top left. */
static void level_edit_change_level(void) {
    if (LEVEL_SAVED == 0) {
        start_message();
        put_string("LEVEL HAS BEEN CHANGED BUT\r"
                   "NOT SAVED. DO YOU WISH TO\r"
                   "SAVE MODIFIED LEVEL (Y/N) ");
        beep();
        TXTPAGE2 = 0;
        for (;;) {
            uint8_t key = wait_for_key(0);
            KBDSTRB = 0;
            if (key == KEY('N'))
                break;
            if (key == KEY('Y')) {
                level_edit_save1();
                break;
            }
        }
    }
    TXTPAGE1 = 0;
    GAME_COLNUM = 0;
    GAME_ROWNUM = 0;
}

/* The cell of LEVEL_MAP under the editor's cursor. */
static uint8_t *cursor_cell(void) {
    return &LEVEL_MAP[GAME_ROWNUM][GAME_COLNUM];
}

/* Waits for a key with the cell under the cursor blinking, and
 * returns it. */
static uint8_t get_key_for_edit_level(void) {
    uint8_t key = wait_for_key_with_cursor_page_1(*cursor_cell());
    KBDSTRB = 0;
    return key;
}

/* Puts `sprite` in the cell under the cursor and draws it there. A
 * different sprite from the one there marks the level as changed. */
static void put_sprite_at_cursor(uint8_t sprite) {
    uint8_t *cell = cursor_cell();
    if (*cell != sprite)
        LEVEL_SAVED = 0;
    *cell = sprite;
    draw_sprite_page1(sprite);
}

/* Edits the level a key at a time. A digit puts that sprite in the
 * cell under the cursor, I, J, K, and M move the cursor, ctrl-S saves
 * the level, the arrows go to the next or previous level, and ctrl-Q
 * goes back to the title screen. */
static void edit_level_key_loop(void) {
    for (;;) {
        uint8_t key = get_key_for_edit_level();
        uint8_t sprite = key - KEY('0');
        if (sprite <= 9) {
            put_sprite_at_cursor(sprite);
            continue;
        }
        switch (key) {
        case KEY('J'):
            if (GAME_COLNUM == 0)
                break;
            GAME_COLNUM--;
            continue;
        case KEY('I'):
            if (GAME_ROWNUM == 0)
                break;
            GAME_ROWNUM--;
            continue;
        case KEY('K'):
            if (GAME_COLNUM >= MAX_GAME_COL)
                break;
            GAME_COLNUM++;
            continue;
        case KEY('M'):
            if (GAME_ROWNUM >= MAX_GAME_ROW)
                break;
            GAME_ROWNUM++;
            continue;
        case KEY_CTRL_S:
            level_edit_save1();
            continue;
        case KEY_RIGHT_ARROW:
            if (DISK_LEVEL_LOC == 149)
                break;
            level_edit_change_level();
            DISK_LEVEL_LOC++;
            LEVELNUM++;
            edit_level();
        case KEY_LEFT_ARROW:
            if (DISK_LEVEL_LOC == 0)
                break;
            level_edit_change_level();
            LEVELNUM--;
            DISK_LEVEL_LOC--;
            edit_level();
        case KEY_CTRL_Q:
            level_edit_change_level();
            start_level_editor();
        }
        beep();
    }
}


/* Takes the place of DOS's format, which jumps here with the slot
 * times 16 in X. The linker script puts it at $8E00, where DOS
 * jumps. */
[[noreturn, c6502::section("format_patch")]]
void format_patch(uint8_t slot16 [[c6502::in_x]]) {
    /* $44 is track 34 in half tracks. SEEKABS steps the head from
     * there to track 0, which reaches track 0 from any track. */
    DOS_CURRENT_TRACK = 0x44;
    SEEKABS(0);

    uint16_t tries = 0x2000;
    uint8_t b;
await_mark:
    if (--tries == 0)
        DSKFORM();
    do {
        b = DISK_IO[slot16 + 4];
    } while (!(b & 0x80));
check_d4:
    if (b != 0xD4)
        goto await_mark;
    do {
        b = DISK_IO[slot16 + 4];
    } while (!(b & 0x80));
check_d5:
    if (b != 0xD5)
        goto check_d4;
    do {
        b = DISK_IO[slot16 + 4];
    } while (!(b & 0x80));
    if (b != 0xD6)
        goto check_d5;

    (void)DISK_IO[slot16];      /* motor off */
    dont_manipulate_master_disk();
    start_level_editor();
}
