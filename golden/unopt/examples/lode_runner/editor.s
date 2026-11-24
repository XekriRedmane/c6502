PLAYER_COL	EQU	$00
PLAYER_ROW	EQU	$01
PLAYER_X_ADJ	EQU	$02
PLAYER_Y_ADJ	EQU	$03
PLAYER_ANIM_STATE	EQU	$04
PLAYER_FACING_DIRECTION	EQU	$05
HARGS	EQU	$06
SSP	EQU	$07
FP	EQU	$09
DPTR	EQU	$0B
GUARD_LOC_COL	EQU	$12
GUARD_LOC_ROW	EQU	$13
GUARD_ANIM_STATE	EQU	$14
GUARD_FACING_DIRECTION	EQU	$15
GUARD_GOLD_TIMER	EQU	$16
GUARD_X_ADJ	EQU	$17
GUARD_Y_ADJ	EQU	$18
GUARD_NUM	EQU	$19
__dos_zp	EQU	$26
GUARD_RESURRECT_COL	EQU	$53
NOTE_INDEX	EQU	$54
GUARD_GOLD_TIMER_START_VALUE	EQU	$5F
GUARD_PATTERNS	EQU	$60
GUARD_PHASE	EQU	$64
PADDLE_VALUES	EQU	$65
GAME_COLNUM	EQU	$85
GAME_ROWNUM	EQU	$86
DRAW_PAGE	EQU	$87
DIGITS	EQU	$89
FRAME_PERIOD	EQU	$8C
GUARD_COUNT	EQU	$8D
SCORE	EQU	$8E
GOLD_COUNT	EQU	$93
DIDNT_PICK_UP_GOLD	EQU	$94
INPUT_MODE	EQU	$95
DISK_LEVEL_LOC	EQU	$96
GUARD_PATTERN_OFFSET	EQU	$97
LIVES	EQU	$98
ENABLE_SOUND	EQU	$99
ALIVE	EQU	$9A
NOT_FALLING	EQU	$9B
DIG_DIRECTION	EQU	$9C
HI_SCORE_ELIGIBLE	EQU	$9D
KEY_COMMAND	EQU	$9E
KEY_COMMAND_LR	EQU	$9F
DIG_ANIM_STATE	EQU	$A0
VERBATIM	EQU	$A2
LADDER_COUNT	EQU	$A3
FALL_PITCH	EQU	$A4
WIPE_MODE	EQU	$A5
LEVELNUM	EQU	$A6
GAME_MODE	EQU	$A7
DEMO_PTR	EQU	$A8
DEMO_MOVE	EQU	$AA
DEMO_MOVE_TURNS	EQU	$AB
DEMO_RUNNING	EQU	$AC
LEVEL_SAVED	EQU	$AD
BLOCK_DATA	EQU	$DF
DOS_CURRENT_TRACK	EQU	$0478
LEVEL_MAP	EQU	$0800
LEVEL_BG	EQU	$0A00
LADDER_LOCS_COL	EQU	$0C00
LADDER_LOCS_ROW	EQU	$0C30
GUARD_LOCS_COL	EQU	$0C60
GUARD_LOCS_ROW	EQU	$0C68
GUARD_GOLD_TIMERS	EQU	$0C70
GUARD_X_ADJS	EQU	$0C78
GUARD_Y_ADJS	EQU	$0C80
GUARD_ANIM_STATES	EQU	$0C88
GUARD_FACING_DIRECTIONS	EQU	$0C90
GUARD_RESURRECTION_TIMERS	EQU	$0C98
BRICK_DIG_COLS	EQU	$0CA0
BRICK_DIG_ROWS	EQU	$0CC0
BRICK_FILL_TIMERS	EQU	$0CE0
DISK_BUFFER	EQU	$0D00
SOUND_DURATION	EQU	$0E00
SOUND_PITCH	EQU	$0E80
SPLASH_SCREEN	EQU	$0F00
ROW_TO_OFFSET_LO	EQU	$1A85
ROW_TO_OFFSET_HI	EQU	$1B45
HALF_SCREEN_COL_TABLE	EQU	$1C35
SCREEN_ROW_TABLE	EQU	$1C51
COL_BYTE_TABLE	EQU	$1C62
COL_SHIFT_TABLE	EQU	$1C7E
HALF_SCREEN_COL_BYTE_TABLE	EQU	$1C9A
HALF_SCREEN_COL_SHIFT_TABLE	EQU	$1D26
DISK_BOOT_SECTOR_DATA	EQU	$1DB2
HI_SCORES	EQU	$1F00
HI_SCORE_MARKER	EQU	$1FF4
HI_SCORE_MASTER_FLAG	EQU	$1FFF
HGR1	EQU	$2000
HGR2	EQU	$4000
DEMO_MOVES	EQU	$9B00
DEMO_LEVELS	EQU	$9E00
PIXEL_SHIFT_TABLE	EQU	$A200
SPRITE_DATA	EQU	$AD00
RWTS	EQU	$B7B5
DOS_IOB	EQU	$B7E8
SEEKABS	EQU	$B9A0
DSKFORM	EQU	$BEAF
KBD	EQU	$C000
KBDSTRB	EQU	$C010
SPKR	EQU	$C030
TXTCLR	EQU	$C050
MIXCLR	EQU	$C052
TXTPAGE1	EQU	$C054
TXTPAGE2	EQU	$C055
HIRES	EQU	$C057
BUTN0	EQU	$C061
BUTN1	EQU	$C062
PADDL	EQU	$C064
PTRIG	EQU	$C070
ROMIN_RDROM_WRRAM2	EQU	$C081
DISK_IO	EQU	$C088

editor_wait_for_key:
   SUBROUTINE

   ; prologue: 0 arg bytes, 8 local bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   wait_for_key_with_cursor_page_1
   LDY   #$02
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   KBDSTRB
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$9B
   BEQ   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   JSR   editor_command_loop
.if_end@0:
   LDY   #$03
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$09
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

start_message:
   SUBROUTINE

   ; prologue: 0 arg bytes, 3 local bytes
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$04
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   clear_hgr2
   LDA   #$40
   LDY   #$01
   STA   (FP),Y
   STA   DRAW_PAGE
   LDA   #$00
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   INY
   STA   (FP),Y
   STA   GAME_ROWNUM

   ; epilogue
   CLC
   LDA   FP
   ADC   #$05
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

hit_key_to_continue:
   SUBROUTINE

   ; prologue: 0 arg bytes, 7 local bytes
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$08
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<str@0
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@0
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   put_string
   JSR   beep
   LDA   #$00
   LDY   #$03
   STA   (FP),Y
   STA   TXTPAGE2
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   wait_for_key
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   KBDSTRB
   INY
   STA   (FP),Y
   STA   TXTPAGE1

   ; epilogue
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

dont_manipulate_master_disk:
   SUBROUTINE

   ; prologue: 0 arg bytes, 2 local bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   start_message
   LDA   #<str@1
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@1
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   put_string
   JSR   hit_key_to_continue

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

bad_data_disk:
   SUBROUTINE

   ; prologue: 0 arg bytes, 2 local bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   start_message
   LDA   #<str@2
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@2
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   put_string
   JSR   hit_key_to_continue

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

check_for_valid_data_disk:
   SUBROUTINE

   ; prologue: 0 arg bytes, 11 local bytes
   SEC
   LDA   SSP
   SBC   #$0D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0C
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   access_hi_score_data_from_disk
   LDY   #$02
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   JSR   bad_data_disk
   JSR   start_level_editor
.if_end@1:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   JSR   dont_manipulate_master_disk
   JSR   start_level_editor
.if_end@2:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

get_level_from_keyboard:
   SUBROUTINE

   ; prologue: 0 arg bytes, 124 local bytes
   SEC
   LDA   SSP
   SBC   #$7E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   DISK_LEVEL_LOC
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   to_decimal3
   LDA   GAME_COLNUM
   STA   saved_game_colnum
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   STA   kbd_entry_index
.loop@0_start:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@0_break
   LDA   kbd_entry_index
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   TAX
   LDA   DIGITS,X
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   put_digit
   LDA   kbd_entry_index
   LDY   #$0F
   STA   (FP),Y
   LDA   kbd_entry_index
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   kbd_entry_index
   JMP   .loop@0_start
.loop@0_break:
   LDA   saved_game_colnum
   STA   GAME_COLNUM
   LDA   #$00
   LDY   #$11
   STA   (FP),Y
   STA   kbd_entry_index
.loop@1_start:
   LDA   kbd_entry_index
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   TAX
   LDA   DIGITS,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$3B
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   DEY
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   wait_for_key_with_cursor_page_1
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   KBDSTRB
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$8D
   BEQ   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .loop@1_break
.lb_skip@5:
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$88
   BEQ   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@3
   LDY   #$22
   LDA   (FP),Y
   CMP   #$95
   BEQ   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .case@4
.lb_skip@6:
   LDY   #$22
   LDA   (FP),Y
   CMP   #$9B
   BEQ   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@5
.lb_skip@4:
   JMP   .default@6
.case@3:
   LDA   kbd_entry_index
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .switch@2_break
.lb_skip@3:
   LDA   kbd_entry_index
   LDY   #$2E
   STA   (FP),Y
   LDA   kbd_entry_index
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   kbd_entry_index
   LDA   GAME_COLNUM
   INY
   STA   (FP),Y
   LDA   GAME_COLNUM
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   JMP   .loop@1_start
.case@4:
   LDA   kbd_entry_index
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .switch@2_break
.lb_skip@2:
   LDA   GAME_COLNUM
   LDY   #$36
   STA   (FP),Y
   LDA   GAME_COLNUM
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   LDA   kbd_entry_index
   INY
   STA   (FP),Y
   LDA   kbd_entry_index
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   kbd_entry_index
   JMP   .loop@1_start
.case@5:
   JSR   editor_command_loop
.default@6:
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$B0
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   SEC
   LDY   #$3C
   SBC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3D
   SBC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$09
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .switch@2_break
.lb_skip@1:
   LDA   kbd_entry_index
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   TAX
   LDY   #$41
   LDA   (FP),Y
   STA   DIGITS,X
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   put_digit
   LDA   kbd_entry_index
   CLC
   ADC   #$01
   LDY   #$49
   STA   (FP),Y
   STA   kbd_entry_index
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
   LDA   kbd_entry_index
   LDY   #$4E
   STA   (FP),Y
   LDA   kbd_entry_index
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   kbd_entry_index
   LDA   GAME_COLNUM
   INY
   STA   (FP),Y
   LDA   GAME_COLNUM
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   JMP   .loop@1_start
.switch@2_break:
   JSR   beep
   JMP   .loop@1_start
.loop@1_break:
   LDA   saved_game_colnum
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   STA   GAME_COLNUM
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   DIGITS
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@7_start:
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@7_break
   LDY   #$57
   LDA   (FP),Y
   CLC
   ADC   #$64
   LDY   #$5F
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   ADC   #$00
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   JMP   .loop@7_start
.loop@7_break:
   LDA   DIGITS+1
   LDY   #$63
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@8_start:
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@8_break
   LDY   #$57
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$69
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   JMP   .loop@8_start
.loop@8_break:
   LDA   DIGITS+2
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   CLC
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   SBC   #$FF
   INY
   LDA   (FP),Y
   SBC   #$00
   LDA   #$00
   ROL
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$7E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@8:
   LDY   #$57
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   STA   LEVELNUM
   LDY   #$57
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$75
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   SBC   #$00
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   STA   DISK_LEVEL_LOC
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$96
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$7A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$7E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

level_editor:
   SUBROUTINE

   ; prologue: 0 arg bytes, 13 local bytes
   SEC
   LDA   SSP
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   SCORE
   INY
   LDA   (FP),Y
   STA   SCORE+1
   INY
   LDA   (FP),Y
   STA   SCORE+2
   INY
   LDA   (FP),Y
   STA   SCORE+3
   LDA   #$05
   INY
   STA   (FP),Y
   STA   LIVES
   INY
   STA   (FP),Y
   STA   GAME_MODE
   LDA   INPUT_MODE
   STA   saved_input_mode
   LDA   #$CB
   INY
   STA   (FP),Y
   STA   INPUT_MODE
   LDA   #$00
   INY
   STA   (FP),Y
   STA   TXTPAGE1
   LDA   DISK_LEVEL_LOC
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$96
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDA   #$00
   INY
   STA   (FP),Y
   STA   DISK_LEVEL_LOC
.if_end@9:
   JSR   start_level_editor

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

start_level_editor:
   SUBROUTINE

   ; prologue: 0 arg bytes, 5 local bytes
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$06
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   clear_hgr1
   LDA   #$20
   LDY   #$01
   STA   (FP),Y
   STA   DRAW_PAGE
   LDA   #$00
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   INY
   STA   (FP),Y
   STA   GAME_ROWNUM
   LDA   #<str@3
   INY
   STA   (FP),Y
   LDA   #>str@3
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   put_string
   JSR   editor_command_loop

   ; epilogue
   CLC
   LDA   FP
   ADC   #$07
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$06
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

editor_play_level:
   SUBROUTINE

   ; prologue: 0 arg bytes, 9 local bytes
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<str@4
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@4
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   put_string
   JSR   get_level_from_keyboard
   LDY   #$03
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@10
   JSR   beep

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0A
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@10:
   LDA   saved_input_mode
   STA   INPUT_MODE
   LDA   #$03
   LDY   #$04
   STA   (FP),Y
   STA   GAME_MODE
   LDA   DISK_LEVEL_LOC
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   STA   HI_SCORE_ELIGIBLE
   JSR   init_game_data

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0A
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

editor_clear_level:
   SUBROUTINE

   ; prologue: 0 arg bytes, 15 local bytes
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$10
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<str@5
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@5
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   put_string
   JSR   get_level_from_keyboard
   LDY   #$03
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@11
   JSR   beep

   ; epilogue
   CLC
   LDA   FP
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$10
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@11:
   JSR   check_for_valid_data_disk
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@9_start:
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   TAX
   LDY   #$06
   LDA   (FP),Y
   STA   DISK_BUFFER,X
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0A
   STA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@9_start
   LDA   #$02
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_compressed_level_data

   ; epilogue
   CLC
   LDA   FP
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$10
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

editor_edit_level:
   SUBROUTINE

   ; prologue: 0 arg bytes, 3 local bytes
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$04
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<str@6
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@6
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   put_string
   JSR   get_level_from_keyboard
   LDY   #$03
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@12
   JSR   beep

   ; epilogue
   CLC
   LDA   FP
   ADC   #$05
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$04
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@12:
   JSR   edit_level

   ; epilogue
   CLC
   LDA   FP
   ADC   #$05
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$04
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

editor_move_level:
   SUBROUTINE

   ; prologue: 0 arg bytes, 15 local bytes
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$10
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<str@7
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@7
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   put_string
   JSR   get_level_from_keyboard
   LDY   #$03
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@13
   JSR   beep

   ; epilogue
   CLC
   LDA   FP
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$10
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@13:
   LDA   DISK_LEVEL_LOC
   STA   editor_level_entry
   LDA   #<str@8
   LDY   #$04
   STA   (FP),Y
   LDA   #>str@8
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   put_string
   JSR   get_level_from_keyboard
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@14
   JSR   beep

   ; epilogue
   CLC
   LDA   FP
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$10
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@14:
   LDA   DISK_LEVEL_LOC
   STA   saved_vtoc_data
   LDA   #<str@9
   LDY   #$07
   STA   (FP),Y
   LDA   #>str@9
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   put_string
   JSR   editor_wait_for_key
   LDY   #$09
   STA   (FP),Y
   JSR   check_for_valid_data_disk
   LDA   editor_level_entry
   STA   DISK_LEVEL_LOC
   LDA   #$01
   LDY   #$0A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_compressed_level_data
   LDA   #<str@10
   LDY   #$0B
   STA   (FP),Y
   LDA   #>str@10
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   put_string
   JSR   editor_wait_for_key
   LDY   #$0D
   STA   (FP),Y
   JSR   check_for_valid_data_disk
   LDA   saved_vtoc_data
   LDY   #$0E
   STA   (FP),Y
   STA   DISK_LEVEL_LOC
   LDA   #$02
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_compressed_level_data

   ; epilogue
   CLC
   LDA   FP
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$10
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

editor_initialize_disk:
   SUBROUTINE

   ; prologue: 0 arg bytes, 91 local bytes
   SEC
   LDA   SSP
   SBC   #$5D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5C
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<str@11
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@11
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   put_string
   JSR   editor_wait_for_key
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$D9
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@15

   ; epilogue
   CLC
   LDA   FP
   ADC   #$5D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@15:
   LDA   DISK_LEVEL_LOC
   LDY   #$0A
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_compressed_level_data
   LDA   #<DISK_BOOT_SECTOR_DATA
   LDY   #$0C
   STA   (FP),Y
   LDA   #>DISK_BOOT_SECTOR_DATA
   INY
   STA   (FP),Y
   LDA   #<DOS_IOB
   INY
   STA   (FP),Y
   LDA   #>DOS_IOB
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$12
   STA   (FP),Y
   LDA   #<DOS_IOB
   INY
   STA   (FP),Y
   LDA   #>DOS_IOB
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$12
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$17
   STA   (FP),Y
   LDA   #<DOS_IOB
   INY
   STA   (FP),Y
   LDA   #>DOS_IOB
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$02
   LDY   #$1C
   STA   (FP),Y
   LDA   #<DOS_IOB
   INY
   STA   (FP),Y
   LDA   #>DOS_IOB
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JSR   access_disk_or_reset_game
   LDA   #$E0
   LDY   #$21
   STA   (FP),Y
   STA   DISK_LEVEL_LOC
   LDA   #$01
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_compressed_level_data
   LDA   #$00
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@10_start:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$38
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@35
   EOR   #$80
.cmp_novf@35:
   ASL
   LDA   #$00
   ROL
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@10_break
   LDY   #$24
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   TAX
   LDA   saved_vtoc_data+1,X
   INY
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   TAX
   LDY   #$2C
   LDA   (FP),Y
   STA   DISK_BUFFER,X
   LDY   #$24
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   JMP   .loop@10_start
.loop@10_break:
   LDA   #$02
   LDY   #$32
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_compressed_level_data
   LDA   #$EF
   LDY   #$33
   STA   (FP),Y
   STA   DISK_LEVEL_LOC
   LDA   #$01
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_compressed_level_data
   LDA   #$00
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@11_start:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   #$21
   CLC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@11_break
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   TAX
   LDA   SAVED_FILE_DESCRIPTIVE_ENTRY_DATA,X
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   TAX
   LDY   #$40
   LDA   (FP),Y
   STA   DISK_BUFFER+11,X
   LDY   #$36
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   JMP   .loop@11_start
.loop@11_break:
   LDA   #$02
   LDY   #$46
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_compressed_level_data
   LDA   #$01
   LDY   #$47
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_hi_score_data_from_disk
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@12_start:
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$0B
   CLC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@12_break
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   TAX
   LDA   HI_SCORE_DATA_MARKER,X
   INY
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   TAX
   LDY   #$54
   LDA   (FP),Y
   STA   HI_SCORE_MARKER,X
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   JMP   .loop@12_start
.loop@12_break:
   LDA   #$02
   LDY   #$5A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_hi_score_data_from_disk
   LDY   #$5B
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   STA   DISK_LEVEL_LOC

   ; epilogue
   CLC
   LDA   FP
   ADC   #$5D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

editor_clear_high_scores:
   SUBROUTINE

   ; prologue: 0 arg bytes, 36 local bytes
   SEC
   LDA   SSP
   SBC   #$26
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$25
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<str@12
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@12
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   put_string
   JSR   editor_wait_for_key
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$D9
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@16

   ; epilogue
   CLC
   LDA   FP
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@16:
   LDA   #$01
   LDY   #$0A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_hi_score_data_from_disk
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@17
   JSR   bad_data_disk
   JSR   start_level_editor
.if_end@17:
   LDA   #<HI_SCORES
   LDY   #$10
   STA   (FP),Y
   LDA   #>HI_SCORES
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@13_start:
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$50
   CLC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@13_break
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   JMP   .loop@13_start
.loop@13_break:
   LDA   #$02
   LDY   #$23
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_hi_score_data_from_disk
   LDY   #$24
   STA   (FP),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

editor_command_loop:
   SUBROUTINE

   ; prologue: 0 arg bytes, 21 local bytes
   SEC
   LDA   SSP
   SBC   #$17
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$16
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.loop@14_start:
   LDA   GAME_ROWNUM
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$09
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@18
   JSR   start_level_editor
.if_end@18:
   LDA   #<str@13
   LDY   #$05
   STA   (FP),Y
   LDA   #>str@13
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   put_string
   JSR   editor_wait_for_key
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BEQ   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .case@16
.lb_skip@8:
   LDY   #$08
   LDA   (FP),Y
   CMP   #$C3
   BEQ   .cmp_true@43
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .case@17
.lb_skip@7:
   LDY   #$08
   LDA   (FP),Y
   CMP   #$C5
   BEQ   .cmp_true@45
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@18
   LDY   #$08
   LDA   (FP),Y
   CMP   #$CD
   BEQ   .cmp_true@47
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@19
   LDY   #$08
   LDA   (FP),Y
   CMP   #$C9
   BEQ   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@20
   LDY   #$08
   LDA   (FP),Y
   CMP   #$D3
   BEQ   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@21
   JMP   .default@22
.case@16:
   JSR   editor_play_level
   JMP   .loop@14_start
.case@17:
   JSR   editor_clear_level
   JMP   .loop@14_start
.case@18:
   JSR   editor_edit_level
   JMP   .loop@14_start
.case@19:
   JSR   editor_move_level
   JMP   .loop@14_start
.case@20:
   JSR   editor_initialize_disk
   JMP   .loop@14_start
.case@21:
   JSR   editor_clear_high_scores
   JMP   .loop@14_start
.default@22:
   JSR   beep
   JMP   .loop@14_start

edit_level:
   SUBROUTINE

   ; prologue: 0 arg bytes, 5 local bytes
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$06
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   clear_hgr2
   LDA   #$40
   LDY   #$01
   STA   (FP),Y
   STA   DRAW_PAGE
   JSR   put_status_draw
   LDA   #$20
   LDY   #$02
   STA   (FP),Y
   STA   DRAW_PAGE
   JSR   check_for_valid_data_disk
   LDA   #$01
   LDY   #$03
   STA   (FP),Y
   STA   LEVEL_SAVED
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   JSR   load_level
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   STA   GAME_COLNUM
   INY
   STA   (FP),Y
   STA   GAME_ROWNUM
   JSR   edit_level_key_loop

   ; epilogue
   CLC
   LDA   FP
   ADC   #$07
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$06
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

level_edit_save1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 16 local bytes
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$11
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   GAME_ROWNUM
   LDY   #$01
   STA   (FP),Y
   LDA   GAME_COLNUM
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   access_hi_score_data_from_disk
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@53
   LDA   #$00
   JMP   .cmp_end@54
.cmp_true@53:
   LDA   #$01
.cmp_end@54:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@20
   JSR   bad_data_disk
   JMP   .if_end@19
.if_else@20:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@22
   JSR   dont_manipulate_master_disk
   JMP   .if_end@21
.if_else@22:
   JSR   compress_and_save_level_data
   LDY   #$02
   LDA   (FP),Y
   STA   GAME_COLNUM
   DEY
   LDA   (FP),Y
   STA   GAME_ROWNUM
   LDA   #$01
   LDY   #$0E
   STA   (FP),Y
   STA   LEVEL_SAVED

   ; epilogue
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@21:
.if_end@19:
   LDA   #$00
   LDY   #$0F
   STA   (FP),Y
   STA   GAME_COLNUM
   INY
   STA   (FP),Y
   STA   GAME_ROWNUM
   JSR   edit_level_key_loop

   ; epilogue
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

level_edit_change_level:
   SUBROUTINE

   ; prologue: 0 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   LEVEL_SAVED
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@57
   LDA   #$00
   JMP   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@10
   JMP   .if_end@23
.lb_skip@10:
   JSR   start_message
   LDA   #<str@14
   LDY   #$05
   STA   (FP),Y
   LDA   #>str@14
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   put_string
   JSR   beep
   LDA   #$00
   LDY   #$07
   STA   (FP),Y
   STA   TXTPAGE2
.loop@23_start:
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   wait_for_key
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   KBDSTRB
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$CE
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BEQ   .cmp_true@59
   LDA   #$00
   JMP   .cmp_end@60
.cmp_true@59:
   LDA   #$01
.cmp_end@60:
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@23_break
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$D9
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BEQ   .cmp_true@61
   LDA   #$00
   JMP   .cmp_end@62
.cmp_true@61:
   LDA   #$01
.cmp_end@62:
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@9
   JMP   .loop@23_start
.lb_skip@9:
   JSR   level_edit_save1
.loop@23_break:
.if_end@23:
   LDA   #$00
   LDY   #$18
   STA   (FP),Y
   STA   TXTPAGE1
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   INY
   STA   (FP),Y
   STA   GAME_ROWNUM

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

cursor_cell:
   SUBROUTINE

   ; prologue: 0 arg bytes, 15 local bytes
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$10
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<LEVEL_MAP
   LDY   #$01
   STA   (FP),Y
   LDA   #>LEVEL_MAP
   INY
   STA   (FP),Y
   LDA   GAME_ROWNUM
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@63:
   BCC   .mul_skip@64
   CLC
   ADC   #$1C
.mul_skip@64:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@63
   LDY   #$06
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   LDY   #$05
   ADC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   ADC   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   GAME_COLNUM
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$10
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS

get_key_for_edit_level:
   SUBROUTINE

   ; prologue: 0 arg bytes, 6 local bytes
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$07
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   cursor_cell
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$03
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   wait_for_key_with_cursor_page_1
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   KBDSTRB
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$08
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

put_sprite_at_cursor:
   SUBROUTINE

   ; prologue: 1 arg bytes, 12 local bytes
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   cursor_cell
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .cmp_true@65
   LDA   #$00
   JMP   .cmp_end@66
.cmp_true@65:
   LDA   #$01
.cmp_end@66:
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@26
   LDA   #$00
   INY
   STA   (FP),Y
   STA   LEVEL_SAVED
.if_end@26:
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   draw_sprite_page1

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

edit_level_key_loop:
   SUBROUTINE

   ; prologue: 0 arg bytes, 72 local bytes
   SEC
   LDA   SSP
   SBC   #$4A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$49
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.loop@24_start:
   JSR   get_key_for_edit_level
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$B0
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SEC
   LDY   #$05
   SBC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$09
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@67
   EOR   #$80
.cmp_novf@67:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@27
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   put_sprite_at_cursor
   JMP   .loop@24_start
.if_end@27:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$CA
   BEQ   .cmp_true@68
   LDA   #$00
   JMP   .cmp_end@69
.cmp_true@68:
   LDA   #$01
.cmp_end@69:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@23
   JMP   .case@26
.lb_skip@23:
   LDY   #$0F
   LDA   (FP),Y
   CMP   #$C9
   BEQ   .cmp_true@70
   LDA   #$00
   JMP   .cmp_end@71
.cmp_true@70:
   LDA   #$01
.cmp_end@71:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@22
   JMP   .case@27
.lb_skip@22:
   LDY   #$0F
   LDA   (FP),Y
   CMP   #$CB
   BEQ   .cmp_true@72
   LDA   #$00
   JMP   .cmp_end@73
.cmp_true@72:
   LDA   #$01
.cmp_end@73:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@21
   JMP   .case@28
.lb_skip@21:
   LDY   #$0F
   LDA   (FP),Y
   CMP   #$CD
   BEQ   .cmp_true@74
   LDA   #$00
   JMP   .cmp_end@75
.cmp_true@74:
   LDA   #$01
.cmp_end@75:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@20
   JMP   .case@29
.lb_skip@20:
   LDY   #$0F
   LDA   (FP),Y
   CMP   #$93
   BEQ   .cmp_true@76
   LDA   #$00
   JMP   .cmp_end@77
.cmp_true@76:
   LDA   #$01
.cmp_end@77:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@19
   JMP   .case@30
.lb_skip@19:
   LDY   #$0F
   LDA   (FP),Y
   CMP   #$95
   BEQ   .cmp_true@78
   LDA   #$00
   JMP   .cmp_end@79
.cmp_true@78:
   LDA   #$01
.cmp_end@79:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@18
   JMP   .case@31
.lb_skip@18:
   LDY   #$0F
   LDA   (FP),Y
   CMP   #$88
   BEQ   .cmp_true@80
   LDA   #$00
   JMP   .cmp_end@81
.cmp_true@80:
   LDA   #$01
.cmp_end@81:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@17
   JMP   .case@32
.lb_skip@17:
   LDY   #$0F
   LDA   (FP),Y
   CMP   #$91
   BEQ   .cmp_true@82
   LDA   #$00
   JMP   .cmp_end@83
.cmp_true@82:
   LDA   #$01
.cmp_end@83:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@16
   JMP   .case@33
.lb_skip@16:
   JMP   .switch@25_break
.case@26:
   LDA   GAME_COLNUM
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@84
   LDA   #$00
   JMP   .cmp_end@85
.cmp_true@84:
   LDA   #$01
.cmp_end@85:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@15
   JMP   .switch@25_break
.lb_skip@15:
   LDA   GAME_COLNUM
   LDY   #$25
   STA   (FP),Y
   LDA   GAME_COLNUM
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   JMP   .loop@24_start
.case@27:
   LDA   GAME_ROWNUM
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@86
   LDA   #$00
   JMP   .cmp_end@87
.cmp_true@86:
   LDA   #$01
.cmp_end@87:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@14
   JMP   .switch@25_break
.lb_skip@14:
   LDA   GAME_ROWNUM
   LDY   #$2B
   STA   (FP),Y
   LDA   GAME_ROWNUM
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   GAME_ROWNUM
   JMP   .loop@24_start
.case@28:
   LDA   GAME_COLNUM
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$1B
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@88
   EOR   #$80
.cmp_novf@88:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@13
   JMP   .switch@25_break
.lb_skip@13:
   LDA   GAME_COLNUM
   LDY   #$31
   STA   (FP),Y
   LDA   GAME_COLNUM
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   JMP   .loop@24_start
.case@29:
   LDA   GAME_ROWNUM
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$0F
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@89
   EOR   #$80
.cmp_novf@89:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@12
   JMP   .switch@25_break
.lb_skip@12:
   LDA   GAME_ROWNUM
   LDY   #$37
   STA   (FP),Y
   LDA   GAME_ROWNUM
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   GAME_ROWNUM
   JMP   .loop@24_start
.case@30:
   JSR   level_edit_save1
   JMP   .loop@24_start
.case@31:
   LDA   DISK_LEVEL_LOC
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$95
   BEQ   .cmp_true@90
   LDA   #$00
   JMP   .cmp_end@91
.cmp_true@90:
   LDA   #$01
.cmp_end@91:
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .switch@25_break
.lb_skip@11:
   JSR   level_edit_change_level
   LDA   DISK_LEVEL_LOC
   LDY   #$3D
   STA   (FP),Y
   LDA   DISK_LEVEL_LOC
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   DISK_LEVEL_LOC
   LDA   LEVELNUM
   INY
   STA   (FP),Y
   LDA   LEVELNUM
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   LEVELNUM
   JSR   edit_level
.case@32:
   LDA   DISK_LEVEL_LOC
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@92
   LDA   #$00
   JMP   .cmp_end@93
.cmp_true@92:
   LDA   #$01
.cmp_end@93:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .switch@25_break
   JSR   level_edit_change_level
   LDA   LEVELNUM
   LDY   #$45
   STA   (FP),Y
   LDA   LEVELNUM
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   LEVELNUM
   LDA   DISK_LEVEL_LOC
   INY
   STA   (FP),Y
   LDA   DISK_LEVEL_LOC
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   DISK_LEVEL_LOC
   JSR   edit_level
.case@33:
   JSR   level_edit_change_level
   JSR   start_level_editor
.switch@25_break:
   JSR   beep
   JMP   .loop@24_start

format_patch:
   SUBROUTINE

   ; prologue: 1 arg bytes, 55 local bytes
   SEC
   LDA   SSP
   SBC   #$39
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$38
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$44
   LDY   #$01
   STA   (FP),Y
   STA   DOS_CURRENT_TRACK
   LDA   #$00
   INY
   STA   (FP),Y
   JSR   SEEKABS
   LDA   #$00
   LDY   #$03
   STA   (FP),Y
   LDA   #$20
   INY
   STA   (FP),Y
.format_patch@await_mark:
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   INY
   LDA   (FP),Y
   BEQ   .cmp_true@94
.thread@0:
   LDA   #$00
   JMP   .cmp_end@95
.cmp_true@94:
   LDA   #$01
.cmp_end@95:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@34
   JSR   DSKFORM
.if_end@34:
.loop@34_start:
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   TAX
   LDA   DISK_IO+4,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@97
   LDA   #$00
   JMP   .lnot_end@98
.lnot_true@97:
   LDA   #$01
.lnot_end@98:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@34_start
.format_patch@check_d4:
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$D4
   BNE   .cmp_true@99
   LDA   #$00
   JMP   .cmp_end@100
.cmp_true@99:
   LDA   #$01
.cmp_end@100:
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@26
   JMP   .format_patch@await_mark
.lb_skip@26:
.loop@35_start:
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   TAX
   LDA   DISK_IO+4,X
   INY
   STA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@101
   LDA   #$00
   JMP   .lnot_end@102
.lnot_true@101:
   LDA   #$01
.lnot_end@102:
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@35_start
.format_patch@check_d5:
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$D5
   BNE   .cmp_true@103
   LDA   #$00
   JMP   .cmp_end@104
.cmp_true@103:
   LDA   #$01
.cmp_end@104:
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@25
   JMP   .format_patch@check_d4
.lb_skip@25:
.loop@36_start:
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   TAX
   LDA   DISK_IO+4,X
   INY
   STA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@105
   LDA   #$00
   JMP   .lnot_end@106
.lnot_true@105:
   LDA   #$01
.lnot_end@106:
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@36_start
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$D6
   BNE   .cmp_true@107
   LDA   #$00
   JMP   .cmp_end@108
.cmp_true@107:
   LDA   #$01
.cmp_end@108:
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@24
   JMP   .format_patch@check_d5
.lb_skip@24:
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   TAX
   LDA   DISK_IO,X
   INY
   STA   (FP),Y
   JSR   dont_manipulate_master_disk
   JSR   start_level_editor

   ; epilogue
   CLC
   LDA   FP
   ADC   #$3A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$38
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

str@0:
   DC.B  $0D, $0D, $48, $49, $54, $20, $41, $20, $4B, $45, $59, $20, $54, $4F, $20, $43
   DC.B  $4F, $4E, $54, $49, $4E, $55, $45, $20, $00

str@1:
   DC.B  $55, $53, $45, $52, $20, $4E, $4F, $54, $20, $41, $4C, $4C, $4F, $57, $45, $44
   DC.B  $20, $54, $4F, $0D, $4D, $41, $4E, $49, $50, $55, $4C, $41, $54, $45, $20, $4D
   DC.B  $41, $53, $54, $45, $52, $20, $44, $49, $53, $4B, $45, $54, $54, $45, $2E, $00

str@2:
   DC.B  $44, $49, $53, $4B, $45, $54, $54, $45, $20, $49, $4E, $20, $44, $52, $49, $56
   DC.B  $45, $20, $49, $53, $20, $4E, $4F, $54, $20, $41, $0D, $4C, $4F, $44, $45, $20
   DC.B  $52, $55, $4E, $4E, $45, $52, $20, $44, $41, $54, $41, $20, $44, $49, $53, $4B
   DC.B  $2E, $00

str@3:
   DC.B  $20, $20, $4C, $4F, $44, $45, $20, $52, $55, $4E, $4E, $45, $52, $20, $42, $4F
   DC.B  $41, $52, $44, $20, $45, $44, $49, $54, $4F, $52, $0D, $2D, $2D, $2D, $2D, $2D
   DC.B  $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D
   DC.B  $2D, $2D, $2D, $2D, $2D, $2D, $2D, $0D, $20, $20, $3C, $45, $53, $43, $3E, $20
   DC.B  $41, $42, $4F, $52, $54, $53, $20, $41, $4E, $59, $20, $43, $4F, $4D, $4D, $41
   DC.B  $4E, $44, $0D, $00

str@4:
   DC.B  $0D, $3E, $3E, $50, $4C, $41, $59, $20, $4C, $45, $56, $45, $4C, $00

str@5:
   DC.B  $0D, $3E, $3E, $43, $4C, $45, $41, $52, $20, $4C, $45, $56, $45, $4C, $00

str@6:
   DC.B  $0D, $3E, $3E, $45, $44, $49, $54, $20, $4C, $45, $56, $45, $4C, $00

str@7:
   DC.B  $0D, $3E, $3E, $4D, $4F, $56, $45, $20, $4C, $45, $56, $45, $4C, $00

str@8:
   DC.B  $20, $54, $4F, $20, $4C, $45, $56, $45, $4C, $00

str@9:
   DC.B  $0D, $20, $20, $53, $4F, $55, $52, $43, $45, $20, $44, $49, $53, $4B, $45, $54
   DC.B  $54, $45, $00

str@10:
   DC.B  $0D, $20, $20, $44, $45, $53, $54, $49, $4E, $41, $54, $49, $4F, $4E, $20, $44
   DC.B  $49, $53, $4B, $45, $54, $54, $45, $00

str@11:
   DC.B  $0D, $3E, $3E, $49, $4E, $49, $54, $49, $41, $4C, $49, $5A, $45, $0D, $20, $20
   DC.B  $54, $48, $49, $53, $20, $46, $4F, $52, $4D, $41, $54, $53, $20, $54, $48, $45
   DC.B  $20, $44, $49, $53, $4B, $45, $54, $54, $45, $0D, $20, $20, $46, $4F, $52, $20
   DC.B  $55, $53, $45, $52, $20, $43, $52, $45, $41, $54, $45, $44, $20, $4C, $45, $56
   DC.B  $45, $4C, $53, $2E, $0D, $20, $20, $28, $43, $41, $55, $54, $49, $4F, $4E, $2E
   DC.B  $20, $49, $54, $20, $45, $52, $41, $53, $45, $53, $20, $54, $48, $45, $0D, $20
   DC.B  $20, $20, $45, $4E, $54, $49, $52, $45, $20, $44, $49, $53, $4B, $45, $54, $54
   DC.B  $45, $20, $46, $49, $52, $53, $54, $29, $0D, $0D, $20, $20, $41, $52, $45, $20
   DC.B  $59, $4F, $55, $20, $53, $55, $52, $45, $20, $28, $59, $2F, $4E, $29, $20, $00

str@12:
   DC.B  $0D, $3E, $3E, $43, $4C, $45, $41, $52, $20, $53, $43, $4F, $52, $45, $20, $46
   DC.B  $49, $4C, $45, $0D, $20, $20, $54, $48, $49, $53, $20, $43, $4C, $45, $41, $52
   DC.B  $53, $20, $54, $48, $45, $20, $48, $49, $47, $48, $0D, $20, $20, $53, $43, $4F
   DC.B  $52, $45, $20, $46, $49, $4C, $45, $20, $4F, $46, $20, $41, $4C, $4C, $0D, $20
   DC.B  $20, $45, $4E, $54, $52, $49, $45, $53, $2E, $0D, $0D, $20, $20, $41, $52, $45
   DC.B  $20, $59, $4F, $55, $20, $53, $55, $52, $45, $20, $28, $59, $2F, $4E, $29, $20
   DC.B  $00

str@13:
   DC.B  $0D, $43, $4F, $4D, $4D, $41, $4E, $44, $3E, $00

str@14:
   DC.B  $4C, $45, $56, $45, $4C, $20, $48, $41, $53, $20, $42, $45, $45, $4E, $20, $43
   DC.B  $48, $41, $4E, $47, $45, $44, $20, $42, $55, $54, $0D, $4E, $4F, $54, $20, $53
   DC.B  $41, $56, $45, $44, $2E, $20, $44, $4F, $20, $59, $4F, $55, $20, $57, $49, $53
   DC.B  $48, $20, $54, $4F, $0D, $53, $41, $56, $45, $20, $4D, $4F, $44, $49, $46, $49
   DC.B  $45, $44, $20, $4C, $45, $56, $45, $4C, $20, $28, $59, $2F, $4E, $29, $20, $00

saved_input_mode:
   DS.B  1

kbd_entry_index:
   DC.B  $60

saved_game_colnum:
   DC.B  $85

editor_level_entry:
   DC.B  $0F

saved_vtoc_data:
   DC.B  $60, $02, $11, $0F, $04, $00, $00, $FE, $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00, $7A, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $FF, $FF, $00, $00, $23, $0F, $00, $01

SAVED_FILE_DESCRIPTIVE_ENTRY_DATA:
   DC.B  $22, $0F, $88, $88, $88, $88, $88, $88, $88, $88, $CC, $CF, $C4, $C5, $A0, $D2
   DC.B  $D5, $CE, $CE, $C5, $D2, $A0, $C4, $C1, $D4, $C1, $A0, $C4, $C9, $D3, $CB, $A0
   DC.B  $A0
