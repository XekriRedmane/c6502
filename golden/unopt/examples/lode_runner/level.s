PLAYER_COL	EQU	$00
PLAYER_ROW	EQU	$01
PLAYER_X_ADJ	EQU	$02
PLAYER_Y_ADJ	EQU	$03
PLAYER_ANIM_STATE	EQU	$04
PLAYER_FACING_DIRECTION	EQU	$05
HARGS	EQU	$06
SSP	EQU	$10
GUARD_LOC_COL	EQU	$12
GUARD_LOC_ROW	EQU	$13
GUARD_ANIM_STATE	EQU	$14
GUARD_FACING_DIRECTION	EQU	$15
GUARD_GOLD_TIMER	EQU	$16
GUARD_X_ADJ	EQU	$17
GUARD_Y_ADJ	EQU	$18
GUARD_NUM	EQU	$19
FP	EQU	$1A
DPTR	EQU	$1C
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

draw_level_page2:
   SUBROUTINE

   ; prologue: 0 arg bytes, 231 local bytes
   SEC
   LDA   SSP
   SBC   #$E9
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E8
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$0F
   LDY   #$01
   STA   (FP),Y
   STA   GAME_ROWNUM
.loop@0_start:
   LDA   #$1B
   LDY   #$02
   STA   (FP),Y
   STA   GAME_COLNUM
.loop@1_start:
   LDA   GAME_COLNUM
   LDY   #$03
   STA   (FP),Y
   LDA   #<LEVEL_MAP
   INY
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
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   ADC   #$1C
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   CLC
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   LDY   #$0F
   ADC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$10
   ADC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   VERBATIM
   BNE   .lb_skip@14
   JMP   .if_end@0
.lb_skip@14:
   LDY   #$14
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@13
   JMP   .case@3
.lb_skip@13:
   LDY   #$15
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@12
   JMP   .case@4
.lb_skip@12:
   LDY   #$15
   LDA   (FP),Y
   CMP   #$08
   BEQ   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .case@5
.lb_skip@11:
   LDY   #$15
   LDA   (FP),Y
   CMP   #$09
   BEQ   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .case@6
.lb_skip@10:
   LDY   #$15
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .case@7
.lb_skip@9:
   JMP   .switch@2_break
.case@3:
   LDA   LADDER_COUNT
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$2D
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@8
   JMP   .draw_level_page2@remove
.lb_skip@8:
   LDA   LADDER_COUNT
   CLC
   ADC   #$01
   LDY   #$25
   STA   (FP),Y
   STA   LADDER_COUNT
   LDA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   TAX
   LDA   GAME_ROWNUM
   STA   LADDER_LOCS_ROW,X
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   TAX
   LDY   #$03
   LDA   (FP),Y
   STA   LADDER_LOCS_COL,X
   JMP   .draw_level_page2@remove
.case@4:
   LDA   GOLD_COUNT
   LDY   #$2D
   STA   (FP),Y
   LDA   GOLD_COUNT
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   GOLD_COUNT
   JMP   .switch@2_break
.case@5:
   LDA   GUARD_COUNT
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$05
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .draw_level_page2@remove
.lb_skip@7:
   LDA   GUARD_COUNT
   CLC
   ADC   #$01
   LDY   #$33
   STA   (FP),Y
   STA   GUARD_COUNT
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
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
   LDY   #$03
   LDA   (FP),Y
   STA   GUARD_LOCS_COL,X
   LDY   #$26
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   TAX
   LDA   GAME_ROWNUM
   STA   GUARD_LOCS_ROW,X
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   TAX
   LDY   #$3A
   LDA   (FP),Y
   STA   GUARD_GOLD_TIMERS,X
   LDA   #$00
   LDY   #$3E
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   TAX
   LDY   #$3E
   LDA   (FP),Y
   STA   GUARD_ANIM_STATES,X
   LDA   #$02
   LDY   #$42
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   TAX
   LDY   #$42
   LDA   (FP),Y
   STA   GUARD_X_ADJS,X
   LDA   #$02
   LDY   #$46
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   TAX
   LDY   #$46
   LDA   (FP),Y
   STA   GUARD_Y_ADJS,X
   LDA   #$00
   LDY   #$4A
   STA   (FP),Y
   LDA   #<LEVEL_BG
   INY
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
.mul_loop@14:
   BCC   .mul_skip@15
   CLC
   ADC   #$1C
.mul_skip@15:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@14
   LDY   #$50
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   CLC
   LDY   #$4F
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$50
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   CLC
   LDY   #$56
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$57
   ADC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .switch@2_break
.case@6:
   LDA   PLAYER_COL
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@16
   LDA   #$00
   JMP   .lnot_end@17
.lnot_true@16:
   LDA   #$01
.lnot_end@17:
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .draw_level_page2@remove
.lb_skip@6:
   LDY   #$03
   LDA   (FP),Y
   STA   PLAYER_COL
   LDA   GAME_ROWNUM
   STA   PLAYER_ROW
   LDA   #$02
   LDY   #$60
   STA   (FP),Y
   STA   PLAYER_X_ADJ
   INY
   STA   (FP),Y
   STA   PLAYER_Y_ADJ
   LDA   #$08
   INY
   STA   (FP),Y
   STA   PLAYER_ANIM_STATE
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<LEVEL_BG
   INY
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
.mul_loop@18:
   BCC   .mul_skip@19
   CLC
   ADC   #$1C
.mul_skip@19:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@18
   LDY   #$69
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   CLC
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   CLC
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$70
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$63
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .switch@2_break
.case@7:
   LDA   #$01
   LDY   #$73
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   JMP   .switch@2_break
.draw_level_page2@remove:
   LDA   #$00
   LDY   #$74
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDA   #<LEVEL_MAP
   INY
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
.mul_loop@20:
   BCC   .mul_skip@21
   CLC
   ADC   #$1C
.mul_skip@21:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@20
   LDY   #$7B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   CLC
   LDY   #$7A
   ADC   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7B
   ADC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   CLC
   LDY   #$81
   ADC   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$82
   ADC   (FP),Y
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$75
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$85
   STA   (FP),Y
   LDA   #<LEVEL_BG
   INY
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
.mul_loop@22:
   BCC   .mul_skip@23
   CLC
   ADC   #$1C
.mul_skip@23:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@22
   LDY   #$8B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   CLC
   LDY   #$8A
   ADC   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8B
   ADC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   CLC
   LDY   #$91
   ADC   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$92
   ADC   (FP),Y
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$85
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.switch@2_break:
.if_end@0:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   draw_sprite_page2
   LDA   GAME_COLNUM
   SEC
   SBC   #$01
   LDY   #$95
   STA   (FP),Y
   STA   GAME_COLNUM
   LDA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$98
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
   BEQ   .lb_skip@5
   JMP   .loop@1_start
.lb_skip@5:
   LDA   GAME_ROWNUM
   SEC
   SBC   #$01
   LDY   #$9B
   STA   (FP),Y
   STA   GAME_ROWNUM
   LDA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$9E
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
   BEQ   .lb_skip@4
   JMP   .loop@0_start
.lb_skip@4:
   LDA   VERBATIM
   BEQ   .lnot_true@28
   LDA   #$00
   JMP   .lnot_end@29
.lnot_true@28:
   LDA   #$01
.lnot_end@29:
   LDY   #$A1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .if_end@4
.lb_skip@3:
   LDA   #<HGR2
   LDY   #$A3
   STA   (FP),Y
   LDA   #>HGR2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDA   #<HGR1
   INY
   STA   (FP),Y
   LDA   #>HGR1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
.loop@8_continue:
   LDA   #<HGR1
   LDY   #$AB
   STA   (FP),Y
   LDA   #>HGR1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$20
   LDY   #$AE
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$AE
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$AD
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$AF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@8_break
.lb_skip@2:
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$B3
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$B8
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .loop@8_continue
.loop@8_break:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$E9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E8
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@4:
   LDA   PLAYER_COL
   LDY   #$BA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$BC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$BD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$E9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E8
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@5:
   JSR   iris_wipe
   LDA   #$0F
   LDY   #$BE
   STA   (FP),Y
   STA   GAME_ROWNUM
.loop@9_start:
   LDA   #$1B
   LDY   #$BF
   STA   (FP),Y
   STA   GAME_COLNUM
.loop@10_start:
   LDA   #<LEVEL_MAP
   LDY   #$C0
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
.mul_loop@33:
   BCC   .mul_skip@34
   CLC
   ADC   #$1C
.mul_skip@34:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@33
   LDY   #$C5
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   CLC
   LDY   #$C4
   ADC   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C5
   ADC   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   LDA   GAME_COLNUM
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   CLC
   LDY   #$CB
   ADC   (FP),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$CC
   ADC   (FP),Y
   LDY   #$CE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$CF
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
   CMP   #$09
   BEQ   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$D3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@6
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$D5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$08
   BEQ   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$D7
   STA   (FP),Y
   LDA   #$00
   INY
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
   LDY   #$D9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$D9
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
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
.if_end@8:
   LDA   GAME_COLNUM
   SEC
   SBC   #$01
   LDY   #$DC
   STA   (FP),Y
   STA   GAME_COLNUM
   LDA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$DF
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
   BEQ   .lb_skip@1
   JMP   .loop@10_start
.lb_skip@1:
   LDA   GAME_ROWNUM
   SEC
   SBC   #$01
   LDY   #$E2
   STA   (FP),Y
   STA   GAME_ROWNUM
   LDA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$E5
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
   BEQ   .lb_skip@0
   JMP   .loop@9_start
.lb_skip@0:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$E9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E8
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

load_level:
   SUBROUTINE

   ; prologue: 1 arg bytes, 116 local bytes
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

.loop@11_start:
   LDY   #$77
   LDA   (FP),Y
   STA   VERBATIM
   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   STA   PLAYER_COL
   LDA   #$00
   INY
   STA   (FP),Y
   STA   LADDER_COUNT
   INY
   STA   (FP),Y
   STA   GOLD_COUNT
   INY
   STA   (FP),Y
   STA   GUARD_COUNT
   INY
   STA   (FP),Y
   STA   GUARD_NUM
   INY
   STA   (FP),Y
   STA   DIG_ANIM_STATE
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@12_start:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$1F
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@43
   EOR   #$80
.cmp_novf@43:
   ASL
   LDA   #$00
   ROL
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@12_break
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   TAX
   LDY   #$0D
   LDA   (FP),Y
   STA   BRICK_FILL_TIMERS,X
   LDY   #$08
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   JMP   .loop@12_start
.loop@12_break:
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@13_start:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$06
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@44
   EOR   #$80
.cmp_novf@44:
   ASL
   LDA   #$00
   ROL
   LDY   #$17
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
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   TAX
   LDY   #$19
   LDA   (FP),Y
   STA   GUARD_RESURRECTION_TIMERS,X
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   JMP   .loop@13_start
.loop@13_break:
   LDA   #$01
   LDY   #$1F
   STA   (FP),Y
   STA   ALIVE
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
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@14_start:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$10
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@45
   EOR   #$80
.cmp_novf@45:
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
   BNE   .lb_skip@16
   JMP   .loop@14_break
.lb_skip@16:
   LDA   #$00
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@15_start:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$1C
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@46
   EOR   #$80
.cmp_novf@46:
   ASL
   LDA   #$00
   ROL
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@15
   JMP   .loop@15_break
.lb_skip@15:
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   TAX
   LDA   DISK_BUFFER,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@10
   LDY   #$33
   LDA   (FP),Y
   LDY   #$38
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
   LDY   #$3A
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
   LDY   #$3C
   STA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   JMP   .if_end@9
.if_else@10:
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$0F
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$33
   STA   (FP),Y
.if_end@9:
   LDY   #$33
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$0A
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@47
   EOR   #$80
.cmp_novf@47:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$33
   STA   (FP),Y
.if_end@11:
   LDA   #<LEVEL_MAP
   LDY   #$49
   STA   (FP),Y
   LDA   #>LEVEL_MAP
   INY
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$4B
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
.mul_loop@48:
   BCC   .mul_skip@49
   CLC
   ADC   #$1C
.mul_skip@49:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@48
   LDY   #$4E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   CLC
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4E
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$2A
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   CLC
   LDY   #$54
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$55
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$33
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<LEVEL_BG
   LDY   #$58
   STA   (FP),Y
   LDA   #>LEVEL_BG
   INY
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$5A
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
.mul_loop@50:
   BCC   .mul_skip@51
   CLC
   ADC   #$1C
.mul_skip@51:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@50
   LDY   #$5D
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   CLC
   LDY   #$5C
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5D
   ADC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   CLC
   LDY   #$63
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$64
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$33
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   JMP   .loop@15_start
.loop@15_break:
   LDY   #$24
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   JMP   .loop@14_start
.loop@14_break:
   JSR   draw_level_page2
   LDY   #$6B
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@52
   LDA   #$00
   JMP   .lnot_end@53
.lnot_true@52:
   LDA   #$01
.lnot_end@53:
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@12

   ; epilogue
   CLC
   LDA   FP
   ADC   #$77
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
.if_end@12:
   LDA   DISK_LEVEL_LOC
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@54
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   JSR   reset_game
.if_end@13:
   LDA   #$00
   LDY   #$72
   STA   (FP),Y
   STA   DISK_LEVEL_LOC
   LDA   GUARD_PATTERN_OFFSET
   INY
   STA   (FP),Y
   LDA   GUARD_PATTERN_OFFSET
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   GUARD_PATTERN_OFFSET
   LDA   #$01
   LDY   #$77
   STA   (FP),Y
   JMP   .loop@11_start

compress_and_save_level_data:
   SUBROUTINE

   ; prologue: 0 arg bytes, 59 local bytes
   SEC
   LDA   SSP
   SBC   #$3D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3C
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@16_start:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$10
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@56
   EOR   #$80
.cmp_novf@56:
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
   BNE   .lb_skip@19
   JMP   .loop@16_break
.lb_skip@19:
   LDA   #$00
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@17_start:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$1C
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@57
   EOR   #$80
.cmp_novf@57:
   ASL
   LDA   #$00
   ROL
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@18
   JMP   .loop@17_break
.lb_skip@18:
   LDA   #<LEVEL_MAP
   LDY   #$11
   STA   (FP),Y
   LDA   #>LEVEL_MAP
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$13
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
.mul_loop@58:
   BCC   .mul_skip@59
   CLC
   ADC   #$1C
.mul_skip@59:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@58
   LDY   #$16
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$15
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@17
   JMP   .if_else@15
.lb_skip@17:
   LDY   #$21
   LDA   (FP),Y
   LDY   #$26
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
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   LDY   #$28
   STA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2D
   ORA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$2E
   ORA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$32
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
   LDY   #$31
   LDA   (FP),Y
   STA   DISK_BUFFER,X
   JMP   .if_end@14
.if_else@15:
   LDY   #$21
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
.if_end@14:
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   JMP   .loop@17_start
.loop@17_break:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   JMP   .loop@16_start
.loop@16_break:
   LDA   #$02
   LDY   #$3B
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
   ADC   #$3D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

access_compressed_level_data:
   SUBROUTINE

   ; prologue: 1 arg bytes, 64 local bytes
   SEC
   LDA   SSP
   SBC   #$42
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$41
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<DOS_IOB
   LDY   #$01
   STA   (FP),Y
   LDA   #>DOS_IOB
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$43
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   GAME_MODE
   LDY   #$05
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
   BVC   .cmp_novf@60
   EOR   #$80
.cmp_novf@60:
   ASL
   LDA   #$00
   ROL
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@21
   JMP   .if_end@16
.lb_skip@21:
   LDA   #<DEMO_LEVELS
   LDY   #$09
   STA   (FP),Y
   LDA   #>DEMO_LEVELS
   INY
   STA   (FP),Y
   LDA   LEVELNUM
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   LDA   #$01
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$0D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@18_start:
   LDY   #$14
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$15
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   TAX
   LDY   #$19
   LDA   (FP),Y
   STA   DISK_BUFFER,X
   LDY   #$14
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1D
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@61
   LDA   #$00
   JMP   .cmp_end@62
.cmp_true@61:
   LDA   #$01
.cmp_end@62:
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@20
   JMP   .loop@18_start
.lb_skip@20:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$43
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$41
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@16:
   LDA   DISK_LEVEL_LOC
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
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
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$28
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   DISK_LEVEL_LOC
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$0F
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
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
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$31
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<DISK_BUFFER
   LDY   #$36
   STA   (FP),Y
   LDA   #>DISK_BUFFER
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
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$36
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$3C
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
   ADC   #$03
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JSR   access_disk_or_reset_game

   ; epilogue
   CLC
   LDA   FP
   ADC   #$43
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$41
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

access_disk_or_reset_game:
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

   LDA   #<DOS_IOB
   LDY   #$01
   STA   (FP),Y
   LDA   #>DOS_IOB
   INY
   STA   (FP),Y
   PHA
   DEY
   LDA   (FP),Y
   TAY
   PLA
   JSR   RWTS
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@17
   JSR   reset_game
.if_end@17:

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

access_hi_score_data_from_disk:
   SUBROUTINE

   ; prologue: 1 arg bytes, 61 local bytes
   SEC
   LDA   SSP
   SBC   #$3F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<DOS_IOB
   LDY   #$01
   STA   (FP),Y
   LDA   #>DOS_IOB
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$40
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$0C
   LDY   #$05
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
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$05
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$0F
   LDY   #$0A
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
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<HI_SCORES
   LDY   #$0F
   STA   (FP),Y
   LDA   #>HI_SCORES
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
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$15
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
   ADC   #$03
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$15
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<DOS_IOB
   LDY   #$1A
   STA   (FP),Y
   LDA   #>DOS_IOB
   INY
   STA   (FP),Y
   PHA
   DEY
   LDA   (FP),Y
   TAY
   PLA
   JSR   RWTS
   LDA   #$00
   ROL
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@18
   JSR   reset_game
.if_end@18:
   LDA   #$00
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@19_start:
   LDY   #$21
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
   BVC   .cmp_novf@63
   EOR   #$80
.cmp_novf@63:
   ASL
   LDA   #$00
   ROL
   LDY   #$23
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   TAX
   LDA   HI_SCORE_MARKER,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   TAX
   LDA   HI_SCORE_DATA_MARKER,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2F
   EOR   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$30
   EOR   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   ORA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   ORA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   JMP   .loop@19_start
.loop@19_break:
   LDY   #$1E
   LDA   (FP),Y
   BEQ   .if_end@19
   LDA   #$00
   LDY   #$3A
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@19:
   LDA   HI_SCORE_MASTER_FLAG
   BEQ   .cond_else@20
   LDA   #$01
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .cond_end@21
.cond_else@20:
   LDA   #$FF
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.cond_end@21:
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$40
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

enable_next_level_ladders:
   SUBROUTINE

   ; prologue: 0 arg bytes, 90 local bytes
   SEC
   LDA   SSP
   SBC   #$5C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5B
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
   STA   LADDER_LOCS_COL
   LDA   LADDER_COUNT
   INY
   STA   (FP),Y
.loop@20_start:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@64
   LDA   #$00
   JMP   .cmp_end@65
.cmp_true@64:
   LDA   #$01
.cmp_end@65:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@24
   JMP   .loop@20_break
.lb_skip@24:
   LDY   #$02
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
   LDA   LADDER_LOCS_COL,X
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
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@23
   JMP   .loop@20_continue
.lb_skip@23:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   TAX
   LDA   LADDER_LOCS_ROW,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   GAME_COLNUM
   LDY   #$14
   LDA   (FP),Y
   STA   GAME_ROWNUM
   LDA   #<LEVEL_BG
   INY
   STA   (FP),Y
   LDA   #>LEVEL_BG
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$17
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
.mul_loop@66:
   BCC   .mul_skip@67
   CLC
   ADC   #$1C
.mul_skip@67:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@66
   LDY   #$1A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$20
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$21
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #<LEVEL_MAP
   INY
   STA   (FP),Y
   LDA   #>LEVEL_MAP
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$28
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
.mul_loop@68:
   BCC   .mul_skip@69
   CLC
   ADC   #$1C
.mul_skip@69:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@68
   LDY   #$2B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   CLC
   LDY   #$31
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$32
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@70
   LDA   #$00
   JMP   .cmp_end@71
.cmp_true@70:
   LDA   #$01
.cmp_end@71:
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@23
   LDA   #$01
   STA   LADDER_LOCS_COL
   JMP   .loop@20_continue
.if_end@23:
   LDA   #$03
   LDY   #$3C
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$35
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@72
   LDA   #$00
   JMP   .cmp_end@73
.cmp_true@72:
   LDA   #$01
.cmp_end@73:
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@24
   LDA   #$03
   INY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$42
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.if_end@24:
   LDA   #$03
   LDY   #$43
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
   LDA   #$03
   LDY   #$44
   STA   (FP),Y
   LDY   #$0B
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
   LDA   HALF_SCREEN_COL_TABLE,X
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   TAX
   LDA   SCREEN_ROW_TABLE,X
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$44
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   draw_sprite_at_pixel_coords
   LDY   #$4D
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   TAX
   LDY   #$4E
   LDA   (FP),Y
   STA   LADDER_LOCS_COL,X
.loop@20_continue:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@20_start
.loop@20_break:
   LDA   LADDER_LOCS_COL
   LDY   #$54
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@74
   LDA   #$00
   JMP   .cmp_end@75
.cmp_true@74:
   LDA   #$01
.cmp_end@75:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@25
   LDA   GOLD_COUNT
   INY
   STA   (FP),Y
   LDA   GOLD_COUNT
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   GOLD_COUNT
.if_end@25:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

HI_SCORE_DATA_MARKER:
   DC.B  $CC, $CF, $C4, $C5, $A0, $D2, $D5, $CE, $CE, $C5, $D2
