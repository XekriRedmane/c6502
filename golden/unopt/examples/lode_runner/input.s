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

paddle_timer_running:
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

   LDA   PADDL
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   PADDL+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   ORA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   ORA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_end@0
   LDA   #$01
.cmp_end@0:
   LDY   #$0F
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$11
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

read_paddles:
   SUBROUTINE

   ; prologue: 0 arg bytes, 40 local bytes
   SEC
   LDA   SSP
   SBC   #$2A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$29
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
   STA   PADDLE_VALUES
   INY
   STA   (FP),Y
   STA   PADDLE_VALUES+1
   LDA   PTRIG
   INY
   STA   (FP),Y
.loop@0_start:
   LDA   PADDL+1
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDA   #<PADDLE_VALUES
   INY
   STA   (FP),Y
   LDA   #>PADDLE_VALUES
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.if_end@0:
   LDA   PADDL
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDA   #<PADDLE_VALUES
   INY
   STA   (FP),Y
   LDA   #>PADDLE_VALUES
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$19
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.if_end@1:
   JSR   paddle_timer_running
   LDY   #$1A
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@1
   LDA   #$00
   JMP   .lnot_end@2
.lnot_true@1:
   LDA   #$01
.lnot_end@2:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$29
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@2:
   LDA   PADDLE_VALUES
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   PADDLE_VALUES+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$21
   ORA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$22
   ORA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@3
   LDA   #$00
   JMP   .lnot_end@4
.lnot_true@3:
   LDA   #$01
.lnot_end@4:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$29
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

past_threshold:
   SUBROUTINE

   ; prologue: 2 arg bytes, 18 local bytes
   SEC
   LDA   SSP
   SBC   #$14
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$13
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$16
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$2E
   BEQ   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDY   #$15
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   SEC
   LDY   #$07
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$13
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@3:
   LDY   #$15
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   SEC
   LDY   #$0E
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   SBC   (FP),Y
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$16
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

button_pressed:
   SUBROUTINE

   ; prologue: 0 arg bytes, 19 local bytes
   SEC
   LDA   SSP
   SBC   #$15
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$14
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   INPUT_MODE
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$CB
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .and_false@4
.lb_skip@1:
   LDA   BUTN1
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@6
   LDA   BUTN0
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
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@6
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@7
.or_true@6:
   LDA   #$01
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$0F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$15
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

read_joystick_for_command:
   SUBROUTINE

   ; prologue: 0 arg bytes, 31 local bytes
   SEC
   LDA   SSP
   SBC   #$21
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$20
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   BUTN1
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@9
   LDA   #$D5
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .if_end@8
.if_else@9:
   LDA   BUTN0
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@11
   LDA   #$CF
   INY
   STA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   JMP   .if_end@10
.if_else@11:
   JSR   read_paddles
   LDA   PADDLE_VALUES
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   paddle_thresholds+1
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
   LDY   #$10
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   past_threshold
   LDY   #$11
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@13
   LDA   #$CC
   INY
   STA   (FP),Y
   STA   KEY_COMMAND_LR
   JMP   .if_end@12
.if_else@13:
   LDA   paddle_thresholds
   LDY   #$13
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   past_threshold
   LDY   #$14
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@15
   LDA   #$CA
   INY
   STA   (FP),Y
   STA   KEY_COMMAND_LR
   JMP   .if_end@14
.if_else@15:
   LDA   #$C0
   LDY   #$16
   STA   (FP),Y
   STA   KEY_COMMAND_LR
.if_end@14:
.if_end@12:
   LDA   PADDLE_VALUES+1
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   paddle_thresholds+2
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
   LDY   #$19
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   past_threshold
   LDY   #$1A
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@17
   LDA   #$C9
   INY
   STA   (FP),Y
   STA   KEY_COMMAND
   JMP   .if_end@16
.if_else@17:
   LDA   paddle_thresholds+3
   LDY   #$1C
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$18
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   past_threshold
   LDY   #$1D
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@19
   LDA   #$CB
   INY
   STA   (FP),Y
   STA   KEY_COMMAND
   JMP   .if_end@18
.if_else@19:
   LDA   #$C0
   LDY   #$1F
   STA   (FP),Y
   STA   KEY_COMMAND
.if_end@18:
.if_end@16:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$21
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$20
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@10:
.if_end@8:
   STA   KEY_COMMAND
   LDY   #$07
   LDA   (FP),Y
   STA   KEY_COMMAND_LR

   ; epilogue
   CLC
   LDA   FP
   ADC   #$21
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$20
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

check_for_mode_1_input:
   SUBROUTINE

   ; prologue: 0 arg bytes, 45 local bytes
   SEC
   LDA   SSP
   SBC   #$2F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   KBD
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@20
   JSR   button_pressed
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@20
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@21
.or_true@20:
   LDA   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@21:
   LDY   #$07
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@22
   LDA   DEMO_RUNNING
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$0B
   STA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   STA   DEMO_RUNNING
   LDA   ALIVE
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$10
   STA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   STA   ALIVE
   LDA   #$01
   INY
   STA   (FP),Y
   STA   LIVES

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@22:
   LDA   DEMO_MOVE_TURNS
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@23
   LDA   DEMO_PTR
   CLC
   INY
   STA   (FP),Y
   LDA   DEMO_PTR+1
   ADC   #$00
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
   LDY   #$1A
   STA   (FP),Y
   STA   DEMO_MOVE
   LDA   DEMO_PTR
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   DEMO_PTR+1
   ADC   #$00
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
   LDY   #$1D
   STA   (FP),Y
   STA   DEMO_MOVE_TURNS
   LDA   DEMO_PTR
   CLC
   ADC   #$02
   INY
   STA   (FP),Y
   LDA   DEMO_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DEMO_PTR
   INY
   LDA   (FP),Y
   STA   DEMO_PTR+1
.if_end@23:
   LDA   DEMO_MOVE
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$0F
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   TAX
   LDA   VALID_KEY_COMMANDS,X
   INY
   STA   (FP),Y
   STA   KEY_COMMAND
   LDA   DEMO_MOVE
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LSR
   LSR
   LSR
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   INY
   ORA   (FP),Y
   STA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   TAX
   LDA   VALID_KEY_COMMANDS,X
   INY
   STA   (FP),Y
   STA   KEY_COMMAND_LR
   LDA   DEMO_MOVE_TURNS
   INY
   STA   (FP),Y
   LDA   DEMO_MOVE_TURNS
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   DEMO_MOVE_TURNS

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2F
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

wait_key_queued:
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

.loop@1_start:
   LDA   KBD
   LDY   #$01
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
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@13
   LDA   #$00
   JMP   .lnot_end@14
.lnot_true@13:
   LDA   #$01
.lnot_end@14:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@1_start
   LDY   #$02
   LDA   (FP),Y
   STA   KBDSTRB
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

check_for_input:
   SUBROUTINE

   ; prologue: 0 arg bytes, 116 local bytes
   SEC
   LDA   SSP
   SBC   #$76
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$75
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.loop@2_start:
   LDA   GAME_MODE
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@24
   JSR   check_for_mode_1_input

   ; epilogue
   CLC
   LDA   FP
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$75
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@24:
   LDA   KBD
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   STA   KBDSTRB
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@17
   LDA   #$00
   JMP   .lnot_end@18
.lnot_true@17:
   LDA   #$01
.lnot_end@18:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@25
   LDA   INPUT_MODE
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$CB
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@26
   JSR   read_joystick_for_command
.if_end@26:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$75
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@25:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$9E
   BEQ   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@16
   JMP   .case@4
.lb_skip@16:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$80
   BEQ   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@15
   JMP   .case@5
.lb_skip@15:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$9B
   BEQ   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@14
   JMP   .case@6
.lb_skip@14:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$92
   BEQ   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@13
   JMP   .case@8
.lb_skip@13:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$81
   BEQ   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@12
   JMP   .case@9
.lb_skip@12:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$93
   BEQ   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .case@10
.lb_skip@11:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$8A
   BEQ   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .case@11
.lb_skip@10:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$8B
   BEQ   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .case@12
.lb_skip@9:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$88
   BEQ   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .case@13
.lb_skip@8:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$95
   BEQ   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .case@14
.lb_skip@7:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$98
   BEQ   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .case@15
.lb_skip@6:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$99
   BEQ   .cmp_true@43
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .case@16
.lb_skip@5:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$8D
   BEQ   .cmp_true@45
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@17
.lb_skip@4:
   JMP   .switch@3_break
.case@4:
   LDA   LIVES
   LDY   #$2D
   STA   (FP),Y
   LDA   LIVES
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   LIVES
   LDA   LEVELNUM
   INY
   STA   (FP),Y
   LDA   LEVELNUM
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   LEVELNUM
   LDA   DISK_LEVEL_LOC
   INY
   STA   (FP),Y
   LDA   DISK_LEVEL_LOC
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   DISK_LEVEL_LOC
   LDA   ALIVE
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$35
   STA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   STA   ALIVE
   LDA   HI_SCORE_ELIGIBLE
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$3A
   STA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   STA   HI_SCORE_ELIGIBLE

   ; epilogue
   CLC
   LDA   FP
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$75
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@5:
   LDA   LIVES
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@47
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@27
   LDA   LIVES
   INY
   STA   (FP),Y
   LDA   LIVES
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   LIVES
.if_end@27:
   JSR   put_status_lives
   LDA   HI_SCORE_ELIGIBLE
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$45
   STA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   STA   HI_SCORE_ELIGIBLE
   JMP   .loop@2_start
.case@6:
.loop@7_continue:
   JSR   wait_key_queued
   LDY   #$48
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$9B
   BNE   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@7_continue
   JMP   .loop@2_start
.case@8:
   LDA   #$01
   LDY   #$4D
   STA   (FP),Y
   STA   LIVES
   LDA   ALIVE
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$50
   STA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   STA   ALIVE

   ; epilogue
   CLC
   LDA   FP
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$75
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@9:
   LDA   ALIVE
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$55
   STA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   STA   ALIVE

   ; epilogue
   CLC
   LDA   FP
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$75
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@10:
   LDA   ENABLE_SOUND
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   STA   ENABLE_SOUND
   JMP   .loop@2_start
.case@11:
   LDA   #$CA
   LDY   #$5D
   STA   (FP),Y
   STA   INPUT_MODE
   JMP   .loop@2_start
.case@12:
   LDA   #$CB
   LDY   #$5E
   STA   (FP),Y
   STA   INPUT_MODE
   JMP   .loop@2_start
.case@13:
   LDA   FRAME_PERIOD
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$0F
   BNE   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .loop@2_start
.lb_skip@3:
   LDA   FRAME_PERIOD
   LDY   #$63
   STA   (FP),Y
   LDA   FRAME_PERIOD
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   FRAME_PERIOD
   JMP   .loop@2_start
.case@14:
   LDA   FRAME_PERIOD
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@53
   LDA   #$00
   JMP   .cmp_end@54
.cmp_true@53:
   LDA   #$01
.cmp_end@54:
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@2_start
.lb_skip@2:
   LDA   FRAME_PERIOD
   LDY   #$69
   STA   (FP),Y
   LDA   FRAME_PERIOD
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   FRAME_PERIOD
   JMP   .loop@2_start
.case@15:
   LDA   paddle_thresholds
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   paddle_thresholds+1
   INY
   STA   (FP),Y
   STA   paddle_thresholds
   DEY
   LDA   (FP),Y
   STA   paddle_thresholds+1
   JMP   .loop@2_start
.case@16:
   LDA   paddle_thresholds+2
   LDY   #$6E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   paddle_thresholds+3
   INY
   STA   (FP),Y
   STA   paddle_thresholds+2
   DEY
   LDA   (FP),Y
   STA   paddle_thresholds+3
   JMP   .loop@2_start
.case@17:
   JSR   return_handler
   JMP   .loop@2_start
.switch@3_break:
   LDA   INPUT_MODE
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$CA
   BEQ   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@31
   JSR   read_joystick_for_command
   JMP   .if_end@30
.if_else@31:
   LDY   #$06
   LDA   (FP),Y
   STA   KEY_COMMAND
   LDA   (FP),Y
   STA   KEY_COMMAND_LR
.if_end@30:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$75
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

return_handler:
   SUBROUTINE

   ; prologue: 0 arg bytes, 133 local bytes
   SEC
   LDA   SSP
   SBC   #$87
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$86
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   hi_score_screen
   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@18_start:
   JSR   button_pressed
   LDY   #$07
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@32
   LDA   KBD
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
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@32
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@33
.or_true@32:
   LDA   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@33:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@24
   JMP   .loop@18_break
.lb_skip@24:
   LDY   #$02
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$0F
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@37
   LDY   #$04
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$10
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@37
   LDA   #$00
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@38
.or_true@37:
   LDA   #$01
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@38:
   LDY   #$11
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@35
   LDY   #$06
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$13
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@35
   LDA   #$00
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@36
.or_true@35:
   LDA   #$01
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@36:
   LDY   #$14
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@23
   JMP   .loop@18_start
.lb_skip@23:
.loop@18_break:
   LDA   #$00
   LDY   #$16
   STA   (FP),Y
   STA   KBDSTRB
   INY
   STA   (FP),Y
   STA   TXTPAGE1
   JSR   clear_hgr2
   LDA   #$0F
   LDY   #$18
   STA   (FP),Y
   STA   GAME_ROWNUM
.loop@19_start:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$1B
   STA   (FP),Y
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@22
   JMP   .loop@19_break
.lb_skip@22:
   LDA   #$1B
   LDY   #$1E
   STA   (FP),Y
   STA   GAME_COLNUM
.loop@20_start:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   LDY   #$21
   STA   (FP),Y
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@21
   JMP   .loop@20_break
.lb_skip@21:
   LDA   #<LEVEL_BG
   LDY   #$24
   STA   (FP),Y
   LDA   #>LEVEL_BG
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
.mul_loop@61:
   BCC   .mul_skip@62
   CLC
   ADC   #$1C
.mul_skip@62:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@61
   LDY   #$29
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   CLC
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   GAME_COLNUM
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$30
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$33
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
   CMP   #$05
   BEQ   .cmp_true@63
   LDA   #$00
   JMP   .cmp_end@64
.cmp_true@63:
   LDA   #$01
.cmp_end@64:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@39
   LDA   #$01
   INY
   STA   (FP),Y
   LDY   #$34
   STA   (FP),Y
.if_end@39:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$34
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   draw_sprite_page2
   LDA   GAME_COLNUM
   LDY   #$3A
   STA   (FP),Y
   LDA   GAME_COLNUM
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   JMP   .loop@20_start
.loop@20_break:
   LDA   GAME_ROWNUM
   LDY   #$3C
   STA   (FP),Y
   LDA   GAME_ROWNUM
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   GAME_ROWNUM
   JMP   .loop@19_start
.loop@19_break:
   LDA   #$1E
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@21_start:
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@65
   LDA   #$00
   JMP   .sx_done@66
.sx_neg@65:
   LDA   #$FF
.sx_done@66:
   LDY   #$42
   STA   (FP),Y
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@20
   JMP   .loop@21_break
.lb_skip@20:
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   TAX
   LDA   BRICK_FILL_TIMERS,X
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
   BEQ   .cmp_true@67
   LDA   #$00
   JMP   .cmp_end@68
.cmp_true@67:
   LDA   #$01
.cmp_end@68:
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@19
   JMP   .loop@21_continue
.lb_skip@19:
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   TAX
   LDA   BRICK_DIG_ROWS,X
   INY
   STA   (FP),Y
   STA   GAME_ROWNUM
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   TAX
   LDA   BRICK_DIG_COLS,X
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   LDY   #$49
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$15
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@69
   EOR   #$80
.cmp_novf@69:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@42
   LDA   #$00
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
.xjump@0:
   LDY   #$01
   STA   (SSP),Y
   JSR   draw_sprite_page2
   JMP   .if_end@41
.if_else@42:
   LDY   #$49
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$0B
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@70
   EOR   #$80
.cmp_novf@70:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@44
   LDA   #$37
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
   JMP   .xjump@0
.if_else@44:
   LDA   #$38
   LDY   #$60
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
   JSR   draw_sprite_page2
.if_end@41:
.loop@21_continue:
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   JMP   .loop@21_start
.loop@21_break:
   LDA   GUARD_COUNT
   LDY   #$63
   STA   (FP),Y
.loop@22_start:
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@71
   LDA   #$00
   JMP   .cmp_end@72
.cmp_true@71:
   LDA   #$01
.cmp_end@72:
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@18
   JMP   .loop@22_break
.lb_skip@18:
   LDY   #$63
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   TAX
   LDA   GUARD_RESURRECTION_TIMERS,X
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
   BEQ   .cmp_true@73
   LDA   #$00
   JMP   .cmp_end@74
.cmp_true@73:
   LDA   #$01
.cmp_end@74:
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@17
   JMP   .loop@22_continue
.lb_skip@17:
   LDY   #$63
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   TAX
   LDA   GUARD_LOCS_COL,X
   INY
   STA   (FP),Y
   STA   GAME_COLNUM
   LDY   #$63
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   TAX
   LDA   GUARD_LOCS_ROW,X
   INY
   STA   (FP),Y
   STA   GAME_ROWNUM
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$14
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@75
   EOR   #$80
.cmp_novf@75:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@22_continue
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$0B
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@76
   EOR   #$80
.cmp_novf@76:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@47
   LDA   #$39
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .cond_end@48
.cond_else@47:
   LDA   #$3A
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.cond_end@48:
   LDY   #$81
   LDA   (FP),Y
   LDY   #$83
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
   JSR   draw_sprite_page2
.loop@22_continue:
   LDY   #$63
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   JMP   .loop@22_start
.loop@22_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$87
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$86
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

check_joystick_or_delay:
   SUBROUTINE

   ; prologue: 0 arg bytes, 51 local bytes
   SEC
   LDA   SSP
   SBC   #$35
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$34
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   INPUT_MODE
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$CB
   BEQ   .cmp_true@77
   LDA   #$00
   JMP   .cmp_end@78
.cmp_true@77:
   LDA   #$01
.cmp_end@78:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@26
   JMP   .if_end@49
.lb_skip@26:
   LDA   #$02
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@23_start:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@79
   LDA   #$00
   JMP   .cmp_end@80
.cmp_true@79:
   LDA   #$01
.cmp_end@80:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@23_break
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@24_keep_start:
   SEC
   SBC   #$01
   LDY   #$0D
   STA   (FP),Y
   DEY
   STA   (FP),Y
   ORA   #$00
   BNE   .loop@24_keep_start
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   JMP   .loop@23_start
.loop@23_break:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$35
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$34
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@49:
   JSR   read_paddles
   LDA   #$00
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@25_start:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$02
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@81
   EOR   #$80
.cmp_novf@81:
   ASL
   LDA   #$00
   ROL
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@25
   JMP   .loop@25_break
.lb_skip@25:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   TAX
   LDA   PADDLE_VALUES,X
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
   SEC
   SBC   #$12
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@82
   EOR   #$80
.cmp_novf@82:
   ASL
   LDA   #$00
   ROL
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@50
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$3B
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@83
   EOR   #$80
.cmp_novf@83:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@50
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@51
.or_true@50:
   LDA   #$01
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@51:
   LDY   #$23
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@52
   LDY   #$1A
   LDA   (FP),Y
   STA   wait_result
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$35
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$34
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@52:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   JMP   .loop@25_start
.loop@25_break:
   LDA   BUTN1
   LDY   #$27
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
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@84
   LDA   #$00
   JMP   .lnot_end@85
.lnot_true@84:
   LDA   #$01
.lnot_end@85:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@53
   LDA   BUTN0
   INY
   STA   (FP),Y
   LDY   #$28
   STA   (FP),Y
.if_end@53:
   LDY   #$28
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@54
   LDY   #$28
   LDA   (FP),Y
   STA   wait_result
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$35
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$34
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@54:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$35
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$34
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

poll_for_key:
   SUBROUTINE

   ; prologue: 1 arg bytes, 17 local bytes
   SEC
   LDA   SSP
   SBC   #$13
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$12
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$68
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@26_start:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@86
   LDA   #$00
   JMP   .cmp_end@87
.cmp_true@86:
   LDA   #$01
.cmp_end@87:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@27
   JMP   .loop@26_break
.lb_skip@27:
   LDA   KBD
   LDY   #$07
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
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@55
   LDY   #$08
   LDA   (FP),Y
   STA   wait_result
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$14
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$12
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@55:
   JSR   check_joystick_or_delay
   LDY   #$0D
   STA   (FP),Y
   ORA   #$00
   BEQ   .and_false@56
   LDY   #$14
   LDA   (FP),Y
   BEQ   .and_false@56
   LDA   #$01
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@57
.and_false@56:
   LDA   #$00
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@57:
   LDY   #$0E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@58
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$14
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$12
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@58:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@26_start
.loop@26_break:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$14
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$12
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

wait_for_key:
   SUBROUTINE

   ; prologue: 1 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   cursor_sprite
.loop@27_start:
   LDA   cursor_sprite
   BEQ   .cond_else@59
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
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
   JMP   .cond_end@60
.cond_else@59:
   LDA   #$0A
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.cond_end@60:
   LDY   #$03
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
   JSR   draw_sprite_page2
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
   JSR   poll_for_key
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BNE   .loop@27_break
   LDA   #$00
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
   JSR   draw_sprite_page2
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
   JSR   poll_for_key
   LDY   #$08
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@88
   LDA   #$00
   JMP   .lnot_end@89
.lnot_true@88:
   LDA   #$01
.lnot_end@89:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@28
   JMP   .loop@27_start
.lb_skip@28:
.loop@27_break:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   cursor_sprite
   LDY   #$01
   STA   (SSP),Y
   JSR   draw_sprite_page2
   LDA   wait_result

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

wait_for_key_with_cursor_page_1:
   SUBROUTINE

   ; prologue: 1 arg bytes, 7 local bytes
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

   INY
   LDA   (FP),Y
   STA   cursor_sprite
.loop@28_start:
   LDA   cursor_sprite
   BEQ   .cond_else@62
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .cond_end@63
.cond_else@62:
   LDA   #$0A
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.cond_end@63:
   LDY   #$01
   LDA   (FP),Y
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
   JSR   draw_sprite_page1
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   JSR   poll_for_key
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BNE   .loop@28_break
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   cursor_sprite
   LDY   #$01
   STA   (SSP),Y
   JSR   draw_sprite_page1
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   JSR   poll_for_key
   LDY   #$05
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@90
   LDA   #$00
   JMP   .lnot_end@91
.lnot_true@90:
   LDA   #$01
.lnot_end@91:
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@29
   JMP   .loop@28_start
.lb_skip@29:
.loop@28_break:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   cursor_sprite
   LDY   #$01
   STA   (SSP),Y
   JSR   draw_sprite_page1
   LDA   wait_result

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$08
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

wait_key:
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

.loop@29_start:
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   STA   KBDSTRB
   LDA   KBD
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
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@29_start

   ; epilogue
   CLC
   LDA   FP
   ADC   #$08
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

detect_lack_of_joystick:
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

   LDA   PTRIG
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$10
   INY
   STA   (FP),Y
.loop@30_start:
   CMP   #$00
   BNE   .cmp_true@92
   LDY   #$02
   LDA   (FP),Y
   BNE   .cmp_true@92
   LDA   #$00
   JMP   .cmp_end@93
.cmp_true@92:
   LDA   #$01
.cmp_end@93:
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@30_break
   JSR   paddle_timer_running
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@95
   LDA   #$00
   JMP   .lnot_end@96
.lnot_true@95:
   LDA   #$01
.lnot_end@96:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@65

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
.if_end@65:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$0B
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SBC   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   JMP   .loop@30_start
.loop@30_break:
   LDA   #$CB
   LDY   #$0D
   STA   (FP),Y
   STA   INPUT_MODE

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0F
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

VALID_KEY_COMMANDS:
   DC.B  $C9, $CA, $CB, $CC, $CF, $D5, $A0

paddle_thresholds:
   DC.B  $12, $2E
   DC.B  $12, $2E

cursor_sprite:
   DC.B  $06

wait_result:
   DS.B  1
