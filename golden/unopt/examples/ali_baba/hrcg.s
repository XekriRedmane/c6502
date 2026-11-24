NULL_PAGE_LOW	EQU	$00
TEXT_STREAM_PTR	EQU	$06
RWTS_IOB_PTR	EQU	$08
NULL_PAGE_HIGH	EQU	$0A
HARGS	EQU	$10
SSP	EQU	$1B
FP	EQU	$1D
MONITOR_YSAV1_1F	EQU	$1F
WNDLFT	EQU	$20
WNDWDTH	EQU	$21
WNDTOP	EQU	$22
WNDBTM	EQU	$23
TEXT_COL	EQU	$24
TEXT_ROW	EQU	$25
MONITOR_GBAS	EQU	$26
BASL	EQU	$28
DOS_ZP	EQU	$2A
DISK_VM_ZP	EQU	$50
DPTR	EQU	$60
MENU_DESC_PTR	EQU	$C0
MENU_OPTION_PTR	EQU	$C2
TUNE_PC	EQU	$E4
MOB_PTR	EQU	$F4
ENTITY_PTR	EQU	$F8
CHAR_PTR	EQU	$FA
SOFTEV	EQU	$03F2
PWREDUP	EQU	$03F4
LOCATIONS	EQU	$4000
LOCATIONS_END	EQU	$4237
EVENT_THRESHOLD	EQU	$4239
SCENE_COMBAT_FLAG	EQU	$423A
SCENE_P3_DONE	EQU	$423B
SCENE_P1_DONE	EQU	$423C
SCENE_HOSTILE_DONE	EQU	$423D
SCENE_EVENT_RESULT	EQU	$423E
MOBS	EQU	$4C80
INPUT_MODE	EQU	$5A00
TOTAL_MOB_COUNT	EQU	$5A01
CURRENT_PLAYER	EQU	$5A02
CURRENT_ROOM	EQU	$5A03
CHAR_SPRITE	EQU	$5A04
CHAR_AI_MODE	EQU	$5A05
AI_WAIT_FLAG	EQU	$5A06
BROWSE_CHAR_INDEX	EQU	$5A07
VIEW_MAX_COL	EQU	$5A08
VIEW_MAX_ROW	EQU	$5A09
VIEW_MIN_COL	EQU	$5A0A
VIEW_MIN_ROW	EQU	$5A0B
FONT_COL	EQU	$5A0C
FONT_ROW	EQU	$5A0D
FONT_CHARNUM	EQU	$5A0E
FONT_CHARSET	EQU	$5A0F
DAMAGE_AMOUNT	EQU	$5A10
NEAREST_POS	EQU	$5A13
CANDIDATE_POS	EQU	$5A15
HEAL_NOTIFIED	EQU	$5A16
LOCATION_ID	EQU	$5A17
PRNG_OUTPUT	EQU	$5A18
PRNG_STATE	EQU	$5A19
NUM_DIGIT	EQU	$5A1B
NUM_DIVISOR	EQU	$5A1C
NUM_LEADING	EQU	$5A1E
ENCOUNTER_RESULT	EQU	$5A1F
COLOCATE_LINK	EQU	$5A20
COLOCATE_POS	EQU	$5A21
COLOCATE_WRAPPED	EQU	$5A22
HOSTILE_APP	EQU	$5A23
HOSTILE_A	EQU	$5A24
HOSTILE_Y	EQU	$5A25
ADJACENT_THREAT	EQU	$5A26
ADJACENT_POS	EQU	$5A27
SOURCE_CHAR	EQU	$5A28
ACTIVE_CHAR	EQU	$5A29
BOUNDS_MIN_COL	EQU	$5A51
BOUNDS_MAX_COL	EQU	$5A52
BOUNDS_MIN_ROW	EQU	$5A53
BOUNDS_MAX_ROW	EQU	$5A54
LOCATION_POS	EQU	$5A55
LOCATION_STYLE	EQU	$5A56
MSG_LINE_COUNT	EQU	$5A57
EVENT_SLOT_BYTE	EQU	$5A59
EVENT_SUB_VALUE	EQU	$5A5A
SAVED_EVENT_PTR	EQU	$5A5B
AI_PREV_DIR	EQU	$5A5D
EVENT_POS	EQU	$5A5E
EVENT_DEST_POS	EQU	$5A5F
TURN_START_COL	EQU	$5A61
TURN_START_ROW	EQU	$5A62
CURRENT_COL	EQU	$5A63
CURRENT_ROW	EQU	$5A64
MOVE_POINTS	EQU	$5A65
STEPS_TAKEN	EQU	$5A66
INPUT_DIR	EQU	$5A67
AI_BEST_DIST	EQU	$5A68
DIR_SCORES	EQU	$5A6A
AI_TARGET_POS	EQU	$5A71
AI_TARGET_COL	EQU	$5A72
AI_TARGET_ROW	EQU	$5A73
IS_PLAYER_TURN	EQU	$5A74
SAVED_MOB_LINK	EQU	$5A75
COMBAT_WILLINGNESS	EQU	$5A76
TARGET_DEFENSE	EQU	$5A77
COMBAT_STRENGTH	EQU	$5A78
MOB_DEFENSE	EQU	$5A79
DROP_AMOUNT	EQU	$5A7A
DROP_STEP	EQU	$5A7C
STAT_SCALE	EQU	$5A7E
STAT_LEVEL_BITS	EQU	$5A7F
SHOP_ITEM_MASK	EQU	$5A80
SHOP_PRICE	EQU	$5A81
SHOP_VALID_MASK	EQU	$5A83
LOCATION_STYLE_BITS	EQU	$5A84
SHOP_ITEM_OFFSET	EQU	$5A85
TARGET_PTR	EQU	$5A86
BEST_THREAT	EQU	$5A88
SCENE_FOUND_FLAG	EQU	$5A89
SCENE_HOSTILE_FLAG	EQU	$5A8A
SCENE_GROUP_IDX	EQU	$5A8B
WAIT_LOOP_COUNT	EQU	$5A8C
TUNE_LOOP_CTR	EQU	$5A8D
TUNE_LOOP_ADDR	EQU	$5A8E
TUNE_GOSUB_ADDR	EQU	$5A90
TUNE_PRNG_MASK	EQU	$5A92
TUNE_ACTION_VEC	EQU	$5A93
TEXT_RIGHT_MARGIN	EQU	$5A96
TEXT_LEFT_MARGIN	EQU	$5A97
TEXT_STREAM_IDX	EQU	$5A98
SCENE_NUMBER	EQU	$5A99
SAVED_ENTITY_PTR	EQU	$5A9C
SAVED_MOB_PTR	EQU	$5AA0
BLINK_COL	EQU	$5AA2
BLINK_ROW	EQU	$5AA3
BLINK_CHAR	EQU	$5AA4
SAVED_AI_MODE	EQU	$5AA6
ATTRACT_FLAG	EQU	$5AA7
TURN_TUNE_GUARD	EQU	$5AA9
INPUT_OPTION_COUNT	EQU	$5AAA
CURRENT_SELECTION	EQU	$5AAB
INPUT_BUFFER	EQU	$5AAC
SCENE_TEXT_DATA	EQU	$5C00
STATS_TEMPLATE	EQU	$7D83
STATS_PROMPT_KEYBOARD	EQU	$7E01
STATS_PROMPT_JOYSTICK	EQU	$7E18
STATUS_BORDER_DATA	EQU	$7E2F
S_PRESS_SPACE	EQU	$7E97
S_PRESS_BUTTON	EQU	$7EBA
SHOP_TABLE	EQU	$7EDD
SHOP_EFFECT_TABLE	EQU	$7F6D
XP_AWARD_TABLE	EQU	$7FBE
S_A_WALL	EQU	$7FC3
S_A_STATUE	EQU	$7FCA
S_A_TREASURE_CHEST	EQU	$7FD3
S_EMPTY_SPACE	EQU	$7FE4
S_AN_ILLUSION	EQU	$7FF0
S_A_DOOR	EQU	$7FFC
S_AN_INVISIBLE_WALL	EQU	$8003
S_SOME_RUNE_WRITING	EQU	$8016
S_A_TRADING_OUTPOST	EQU	$8028
ATTACK_VERB_STRINGS	EQU	$803A
s_PRINT_FONT_CHAR	EQU	$80A4
TUNE_TABLE	EQU	$80AE
TUNE_0	EQU	$80C6
TUNE_1_COMBAT_HIT	EQU	$80C7
TUNE_2_COMBAT_DAMAGE	EQU	$80D7
FONT_DATA	EQU	$83A5
GAME_MESSAGE_STRINGS	EQU	$9AA5
DISK_IO_CODE	EQU	$B300
DISK_READ_FIELD	EQU	$B300
RWTS_ENTRY	EQU	$B7B5
RWTS_IOB	EQU	$B7E8
KBD	EQU	$C000
KBDSTRB	EQU	$C010
SPKR	EQU	$C030
TXTCLR	EQU	$C050
MIXCLR	EQU	$C052
TXTPAGE1	EQU	$C054
HIRES	EQU	$C057
BUTN	EQU	$C061
PADDL	EQU	$C064
PTRIG	EQU	$C070
DISK_MOTOR_ON	EQU	$C0E9
DISK_BOOT_ROM	EQU	$C600
SETPWRC	EQU	$FB6F
VTAB	EQU	$FC22
VTABZ	EQU	$FC24
CLREOP	EQU	$FC42
HOME	EQU	$FC58
SCROLL	EQU	$FC70
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8

hrcg_row_address:
   SUBROUTINE

   ; prologue: 2 arg bytes, 46 local bytes
   SEC
   LDA   SSP
   SBC   #$30
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   HRCG_PAGE
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$05
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
   LDA   #$08
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$07
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDA   #$80
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$0D
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
   LDA   #$08
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$0F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   #$28
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$11
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$11
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$12
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   WNDLFT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   CLC
   LDY   #$21
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDA   #$00
   LDY   #$27
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$26
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
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   ORA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2C
   ORA   (FP),Y
   LDY   #$2E
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
   ADC   #$32
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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

hrcg_fill_line:
   SUBROUTINE

   ; prologue: 2 arg bytes, 32 local bytes
   SEC
   LDA   SSP
   SBC   #$22
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$21
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
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$23
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDA   (FP),Y
   INY
   STA   (SSP),Y
   JSR   hrcg_row_address
   LDY   #$02
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$08
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDY   #$24
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
.loop@1_start:
   LDA   #<hrcg
   LDY   #$0D
   STA   (FP),Y
   LDA   #>hrcg
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
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   CLC
   LDY   #$10
   ADC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$11
   ADC   (FP),Y
   LDY   #$13
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
   LDY   #$0C
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$14
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   WNDWDTH
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   SEC
   LDY   #$17
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   SBC   (FP),Y
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
   LDA   HRCG_SCAN_LINE
   LDY   #$1B
   STA   (FP),Y
   LDA   HRCG_SCAN_LINE+1
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$24
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

hrcg_clear_from:
   SUBROUTINE

   ; prologue: 2 arg bytes, 8 local bytes
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

.loop@2_start:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   hrcg_fill_line
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$02
   STA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   WNDBTM
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
   DEY
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
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
   BNE   .loop@2_start

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
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

hrcg_scroll_up:
   SUBROUTINE

   ; prologue: 0 arg bytes, 67 local bytes
   SEC
   LDA   SSP
   SBC   #$45
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$44
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   WNDTOP
   LDY   #$01
   STA   (FP),Y
.loop@3_start:
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$05
   STA   (FP),Y
   LDA   WNDBTM
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   SEC
   LDY   #$06
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .loop@3_break
.lb_skip@4:
   LDA   #$00
   LDY   #$0A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   hrcg_row_address
   LDY   #$0B
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   hrcg_row_address
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@4_start:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$08
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
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
   BNE   .lb_skip@3
   JMP   .loop@4_break
.lb_skip@3:
   LDA   #$00
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@5_start:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   WNDWDTH
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   SEC
   LDY   #$23
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   SBC   (FP),Y
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@5_break
.lb_skip@2:
   LDY   #$20
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$27
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   JMP   .loop@5_start
.loop@5_break:
   LDA   HRCG_SCAN_LINE
   LDY   #$32
   STA   (FP),Y
   LDA   HRCG_SCAN_LINE+1
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$32
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$33
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   HRCG_SCAN_LINE
   LDY   #$36
   STA   (FP),Y
   LDA   HRCG_SCAN_LINE+1
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$36
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$37
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   JMP   .loop@4_start
.loop@4_break:
   LDY   #$01
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:
   LDA   WNDBTM
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$43
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   hrcg_fill_line

   ; epilogue
   CLC
   LDA   FP
   ADC   #$45
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$44
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

hrcg_line_feed:
   SUBROUTINE

   ; prologue: 0 arg bytes, 14 local bytes
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   TEXT_ROW
   CLC
   ADC   #$01
   LDY   #$01
   STA   (FP),Y
   STA   TEXT_ROW
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   WNDBTM
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SEC
   LDY   #$04
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   SBC   (FP),Y
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@1
   LDA   TEXT_ROW
   JSR   VTABZ
   JMP   .if_end@0
.if_else@1:
   LDA   #<hrcg
   LDY   #$08
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@3
   LDA   WNDTOP
   STA   TEXT_ROW
   JSR   VTABZ
   JMP   .if_end@2
.if_else@3:
   LDA   TEXT_ROW
   LDY   #$0D
   STA   (FP),Y
   LDA   TEXT_ROW
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   TEXT_ROW
   JSR   hrcg_scroll_up
   JSR   SCROLL
.if_end@2:
.if_end@0:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$10
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

hrcg_advance_cursor:
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

   LDA   TEXT_COL
   CLC
   ADC   #$01
   LDY   #$01
   STA   (FP),Y
   STA   TEXT_COL
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   WNDWDTH
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SEC
   LDY   #$04
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   SBC   (FP),Y
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4

   ; epilogue
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
   RTS
.if_end@4:
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   STA   TEXT_COL
   JSR   hrcg_line_feed

   ; epilogue
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
   RTS

hrcg_draw_char:
   SUBROUTINE

   ; prologue: 2 arg bytes, 72 local bytes
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

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$20
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<hrcg
   INY
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   ORA   #$00
   BEQ   .and_false@7
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$41
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$10
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@5
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$5A
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@5
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@6:
   LDY   #$16
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
.if_end@9:
   LDA   #<hrcg
   LDY   #$18
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1E
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
.mul_loop@10:
   BCC   .mul_skip@11
   CLC
   ADC   #$08
.mul_skip@11:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@10
   LDY   #$21
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   LDY   #$20
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1D
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
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   TEXT_ROW
   LDY   #$01
   STA   (SSP),Y
   LDA   TEXT_COL
   INY
   STA   (SSP),Y
   JSR   hrcg_row_address
   LDY   #$26
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   TEXT_COL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   BASL
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   BASL+1
   DEY
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@6_start:
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$08
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .loop@6_break
.lb_skip@5:
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   CLC
   LDY   #$34
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$35
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<hrcg
   INY
   STA   (FP),Y
   LDA   #>hrcg
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
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3E
   EOR   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3F
   EOR   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$42
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   HRCG_SCAN_LINE
   LDY   #$43
   STA   (FP),Y
   LDA   HRCG_SCAN_LINE+1
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   CLC
   LDY   #$43
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$44
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   JMP   .loop@6_start
.loop@6_break:
   JSR   hrcg_advance_cursor

   ; epilogue
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$49
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

hrcg_select_font:
   SUBROUTINE

   ; prologue: 1 arg bytes, 30 local bytes
   SEC
   LDA   SSP
   SBC   #$20
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1F
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
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$31
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
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@10
   LDY   #$21
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$39
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@10
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@11
.and_false@10:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@11:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .if_else@13
.lb_skip@6:
   LDA   #<FONT_DATA
   LDY   #$0B
   STA   (FP),Y
   LDA   #>FONT_DATA
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$31
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   LDA   #$03
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$11
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$11
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$12
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #<hrcg
   INY
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$13
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   JMP   .if_end@12
.if_else@13:
   LDA   #<STD_FONT_DATA
   LDY   #$19
   STA   (FP),Y
   LDA   #>STD_FONT_DATA
   INY
   STA   (FP),Y
   LDA   #<hrcg
   INY
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$19
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
.if_end@12:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$21
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

hrcg_save:
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

   LDA   hrcg_saved_valid
   BEQ   .lnot_true@15
   LDA   #$00
   JMP   .lnot_end@16
.lnot_true@15:
   LDA   #$01
.lnot_end@16:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@14
   LDA   #$01
   STA   hrcg_saved_valid
   LDA   hrcg
   STA   hrcg_saved
   LDA   hrcg+1
   STA   hrcg_saved+1
   LDA   hrcg+2
   STA   hrcg_saved+2
   LDA   hrcg+3
   STA   hrcg_saved+3
   LDA   hrcg+4
   STA   hrcg_saved+4
.if_end@14:
   LDA   TEXT_COL
   STA   hrcg_saved_ch
   LDA   TEXT_ROW
   STA   hrcg_saved_cv

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

hrcg_restore_column:
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

   LDA   TEXT_COL
   LDY   #$01
   STA   (FP),Y
   LDA   hrcg_saved_ch
   STA   TEXT_COL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$02
   SBC   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   SBC   (FP),Y
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@15
   JSR   hrcg_line_feed
.if_end@15:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$09
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
   RTS

hrcg_restore:
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

   LDA   hrcg_saved_valid
   BEQ   .lnot_true@18
   LDA   #$00
   JMP   .lnot_end@19
.lnot_true@18:
   LDA   #$01
.lnot_end@19:
   LDY   #$01
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
.if_end@16:
   LDA   hrcg_saved
   STA   hrcg
   LDA   hrcg_saved+1
   STA   hrcg+1
   LDA   hrcg_saved+2
   STA   hrcg+2
   LDA   hrcg_saved+3
   STA   hrcg+3
   LDA   hrcg_saved+4
   STA   hrcg+4
   LDA   #$00
   STA   hrcg_saved_valid
   LDY   #$03
   STA   (FP),Y
   STA   hrcg_saved_ch
   LDA   WNDTOP
   STA   hrcg_saved_cv

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

hrcg_set_top_left:
   SUBROUTINE

   ; prologue: 0 arg bytes, 29 local bytes
   SEC
   LDA   SSP
   SBC   #$1F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   TEXT_COL
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   WNDLFT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   LDY   #$03
   ADC   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$04
   ADC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   SEC
   SBC   #$28
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
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@17
   LDA   #$27
   INY
   STA   (FP),Y
   LDY   #$08
   STA   (FP),Y
.if_end@17:
   LDY   #$08
   LDA   (FP),Y
   STA   WNDLFT
   LDA   TEXT_COL
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   WNDWDTH
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$0E
   SBC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   STA   WNDWDTH
   LDA   #$00
   INY
   STA   (FP),Y
   STA   TEXT_COL
   LDA   TEXT_ROW
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
   SBC   #$18
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@18
   LDA   #$17
   INY
   STA   (FP),Y
   LDY   #$16
   STA   (FP),Y
.if_end@18:
   LDY   #$16
   LDA   (FP),Y
   STA   WNDTOP
   LDA   hrcg_saved_valid
   BEQ   .lnot_true@22
   LDA   #$00
   JMP   .lnot_end@23
.lnot_true@22:
   LDA   #$01
.lnot_end@23:
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@19
   LDY   #$16
   LDA   (FP),Y
   STA   hrcg_saved_cv
.if_end@19:
   JSR   VTAB

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

hrcg_shifted_control:
   SUBROUTINE

   ; prologue: 1 arg bytes, 20 local bytes
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$15
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
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$13
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$03
   CMP   (FP),Y
   BEQ   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@21
   LDA   #<hrcg
   INY
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   JMP   .if_end@20
.if_else@21:
   LDY   #$17
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$17
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BEQ   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@22
   LDA   #<hrcg
   INY
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
.if_end@22:
.if_end@20:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$17
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

hrcg_control:
   SUBROUTINE

   ; prologue: 1 arg bytes, 44 local bytes
   SEC
   LDA   SSP
   SBC   #$2E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2D
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
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@18
   JMP   .case@8
.lb_skip@18:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@17
   JMP   .case@9
.lb_skip@17:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@16
   JMP   .case@10
.lb_skip@16:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@15
   JMP   .case@11
.lb_skip@15:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@14
   JMP   .case@12
.lb_skip@14:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$09
   BEQ   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@13
   JMP   .case@13
.lb_skip@13:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$0B
   BEQ   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@12
   JMP   .case@14
.lb_skip@12:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$0C
   BEQ   .cmp_true@42
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .case@15
.lb_skip@11:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$0E
   BEQ   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .case@16
.lb_skip@10:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$0F
   BEQ   .cmp_true@46
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .case@17
.lb_skip@9:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$10
   BEQ   .cmp_true@48
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .case@18
.lb_skip@8:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$16
   BEQ   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .case@19
.lb_skip@7:
   JMP   .switch@7_break
.case@8:
   LDA   #$80
   LDY   #$1B
   STA   (FP),Y
   STA   hrcg_next
   JMP   .switch@7_break
.case@9:
   JSR   hrcg_save
   JMP   .switch@7_break
.case@10:
   JSR   hrcg_restore_column
   JMP   .switch@7_break
.case@11:
   JSR   hrcg_restore
   JMP   .switch@7_break
.case@12:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   TEXT_ROW
   LDY   #$01
   STA   (SSP),Y
   LDA   TEXT_COL
   INY
   STA   (SSP),Y
   JSR   hrcg_clear_from
   JSR   CLREOP
   JMP   .switch@7_break
.case@13:
   LDA   #$7F
   LDY   #$1C
   STA   (FP),Y
   LDA   #<hrcg
   INY
   STA   (FP),Y
   LDA   #>hrcg
   INY
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
   JMP   .switch@7_break
.case@14:
   LDA   #<hrcg
   LDY   #$1F
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   JMP   .switch@7_break
.case@15:
   LDA   #<hrcg
   LDY   #$23
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   JMP   .switch@7_break
.case@16:
   LDA   #$00
   LDY   #$27
   STA   (FP),Y
   LDA   #<hrcg
   INY
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$27
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .switch@7_break
.case@17:
   LDA   #$40
   LDY   #$2A
   STA   (FP),Y
   STA   hrcg_next
   JMP   .switch@7_break
.case@18:
   LDA   WNDTOP
   STA   TEXT_ROW
   LDA   #$00
   LDY   #$2B
   STA   (FP),Y
   STA   TEXT_COL
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   WNDTOP
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   hrcg_clear_from
   JSR   HOME
   JMP   .switch@7_break
.case@19:
   JSR   hrcg_set_top_left
.switch@7_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

hrcg_init:
   SUBROUTINE

   ; prologue: 0 arg bytes, 34 local bytes
   SEC
   LDA   SSP
   SBC   #$24
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$23
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   MIXCLR
   LDY   #$01
   STA   (FP),Y
   LDA   HIRES
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<hrcg
   INY
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<hrcg
   LDY   #$06
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<hrcg
   LDY   #$0A
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   STA   hrcg_saved_valid
   LDY   #$0E
   STA   (FP),Y
   STA   hrcg_saved_ch
   INY
   STA   (FP),Y
   STA   hrcg_saved_cv
   LDA   WNDLFT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   TEXT_COL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$10
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   STA   TEXT_COL
   LDA   #$00
   INY
   STA   (FP),Y
   STA   WNDLFT
   INY
   STA   (FP),Y
   STA   WNDTOP
   LDA   #$28
   INY
   STA   (FP),Y
   STA   WNDWDTH
   LDA   #$18
   INY
   STA   (FP),Y
   STA   WNDBTM
   JSR   VTAB
   LDA   #<STD_FONT_DATA
   LDY   #$1B
   STA   (FP),Y
   LDA   #>STD_FONT_DATA
   INY
   STA   (FP),Y
   LDA   #<hrcg
   INY
   STA   (FP),Y
   LDA   #>hrcg
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   TXTPAGE1
   LDY   #$21
   STA   (FP),Y
   LDA   TXTCLR
   INY
   STA   (FP),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$24
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

hrcg_cout:
   SUBROUTINE

   ; prologue: 1 arg bytes, 28 local bytes
   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1D
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
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$7F
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   hrcg_next
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   hrcg_next
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$80
   BEQ   .cmp_true@52
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@24
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   hrcg_select_font
   JMP   .if_end@23
.if_else@24:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$20
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@54
   EOR   #$80
.cmp_novf@54:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@26
   LDY   #$07
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@19
   JMP   .if_end@25
.lb_skip@19:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   hrcg_draw_char
   JMP   .if_end@25
.if_else@26:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$15
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
   BVC   .cmp_novf@57
   EOR   #$80
.cmp_novf@57:
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
   BEQ   .if_end@28
   LDY   #$07
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$40
   BEQ   .cmp_true@58
   LDA   #$00
   JMP   .cmp_end@59
.cmp_true@58:
   LDA   #$01
.cmp_end@59:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@30
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   hrcg_shifted_control
   JMP   .if_end@29
.if_else@30:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   hrcg_control
.if_end@29:
.if_end@28:
.if_end@25:
.if_end@23:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

MAP_WIDTH:
   DC.B  $14

MAP_HEIGHT:
   DC.B  $0A

STRENGTH_MASK:
   DC.B  $1F

HP_MASK:
   DC.B  $3F

HP_FLAGS_MASK:
   DC.B  $C0

LEVEL_MASK:
   DC.B  $1F

LOW_NIBBLE_MASK:
   DC.B  $0F

FLAGS_STATE_MASK:
   DC.B  $03

FLAGS_ENGAGED_BIT:
   DC.B  $04

FLAGS_DEAD_BIT:
   DC.B  $10

FLAGS_FACTION_MASK:
   DC.B  $70

NPC_APPEARANCE_MIN:
   DC.B  $15

ENTITY_NONE:
   DS.B  2

ENTITY_EDGE:
   DC.W  $0001

SCENE_RETIRE:
   DC.B  $23

HRCG_PAGE:
   DC.B  $20

HRCG_SCAN_LINE:
   DC.W  $0400

hrcg:
   DS.B  5

hrcg_saved:
   DS.B  5

hrcg_saved_valid:
   DS.B  1

hrcg_saved_ch:
   DS.B  1

hrcg_saved_cv:
   DS.B  1

hrcg_next:
   DS.B  1

STD_FONT_DATA:
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00, $08, $08, $08, $08, $08, $00, $08, $00
   DC.B  $14, $14, $14, $00, $00, $00, $00, $00, $08, $1C, $3E, $7F, $1C, $1C, $1C, $1C
   DC.B  $08, $3C, $0A, $1C, $28, $1E, $08, $00, $06, $26, $10, $08, $04, $32, $30, $00
   DC.B  $04, $0A, $0A, $04, $2A, $12, $2C, $00, $08, $08, $08, $00, $00, $00, $00, $00
   DC.B  $08, $04, $02, $02, $02, $04, $08, $00, $08, $10, $20, $20, $20, $10, $08, $00
   DC.B  $08, $2A, $1C, $08, $1C, $2A, $08, $00, $00, $08, $08, $3E, $08, $08, $00, $00
   DC.B  $00, $00, $00, $00, $00, $8C, $88, $84, $00, $00, $00, $3E, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $8C, $00, $00, $20, $10, $08, $04, $02, $00, $00
   DC.B  $8C, $92, $22, $22, $22, $92, $8C, $00, $08, $0C, $08, $08, $08, $08, $1C, $00
   DC.B  $1C, $22, $20, $10, $84, $82, $3E, $00, $3E, $90, $10, $18, $20, $22, $1C, $00
   DC.B  $10, $18, $14, $12, $3E, $10, $10, $00, $3E, $02, $1E, $20, $20, $22, $1C, $00
   DC.B  $18, $04, $82, $1E, $22, $22, $1C, $00, $3E, $90, $10, $88, $08, $84, $04, $00
   DC.B  $1C, $22, $22, $1C, $22, $22, $1C, $00, $1C, $22, $22, $3C, $90, $10, $0C, $00
   DC.B  $00, $00, $8C, $00, $00, $8C, $00, $00, $00, $00, $8C, $00, $00, $8C, $88, $84
   DC.B  $08, $0C, $7E, $7F, $7E, $0C, $08, $00, $00, $00, $3E, $00, $3E, $00, $00, $00
   DC.B  $08, $18, $3F, $7F, $3F, $18, $08, $00, $1C, $22, $90, $88, $08, $00, $08, $00
   DC.B  $1C, $1C, $1C, $1C, $7F, $3E, $1C, $08, $08, $8C, $14, $92, $3E, $22, $22, $00
   DC.B  $1E, $22, $22, $1E, $22, $22, $1E, $00, $1C, $22, $02, $02, $02, $22, $1C, $00
   DC.B  $1E, $22, $22, $22, $22, $22, $1E, $00, $3E, $02, $02, $1E, $02, $02, $3E, $00
   DC.B  $3E, $02, $02, $1E, $02, $02, $02, $00, $3C, $02, $02, $02, $32, $22, $3C, $00
   DC.B  $22, $22, $22, $3E, $22, $22, $22, $00, $1C, $08, $08, $08, $08, $08, $1C, $00
   DC.B  $20, $20, $20, $20, $20, $22, $1C, $00, $22, $12, $0A, $06, $0A, $12, $22, $00
   DC.B  $02, $02, $02, $02, $02, $02, $3E, $00, $22, $36, $2A, $2A, $22, $22, $22, $00
   DC.B  $22, $22, $26, $2A, $32, $22, $22, $00, $1C, $22, $22, $22, $22, $22, $1C, $00
   DC.B  $1E, $22, $22, $1E, $02, $02, $02, $00, $1C, $22, $22, $22, $2A, $12, $2C, $00
   DC.B  $1E, $22, $22, $1E, $0A, $12, $22, $00, $1C, $22, $02, $1C, $20, $22, $1C, $00
   DC.B  $3E, $08, $08, $08, $08, $08, $08, $00, $22, $22, $22, $22, $22, $22, $1C, $00
   DC.B  $22, $22, $22, $92, $14, $8C, $08, $00, $22, $22, $22, $2A, $2A, $36, $22, $00
   DC.B  $22, $92, $14, $08, $14, $92, $22, $00, $22, $92, $14, $08, $08, $08, $08, $00
   DC.B  $3E, $90, $10, $08, $04, $82, $3E, $00, $3E, $06, $06, $06, $06, $06, $3E, $00
   DC.B  $00, $02, $04, $08, $10, $20, $00, $00, $3E, $30, $30, $30, $30, $30, $3E, $00
   DC.B  $00, $00, $08, $14, $22, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $7F
   DC.B  $04, $08, $10, $00, $00, $00, $00, $00, $00, $00, $1C, $20, $3C, $22, $3C, $00
   DC.B  $02, $02, $1E, $22, $22, $22, $1E, $00, $00, $00, $3C, $02, $02, $02, $3C, $00
   DC.B  $20, $20, $3C, $22, $22, $22, $3C, $00, $00, $00, $1C, $22, $3E, $02, $3C, $00
   DC.B  $18, $24, $04, $1E, $04, $04, $04, $00, $00, $00, $1C, $22, $22, $3C, $20, $1C
   DC.B  $02, $02, $1E, $22, $22, $22, $22, $00, $08, $00, $0C, $08, $08, $08, $1C, $00
   DC.B  $10, $00, $18, $10, $10, $10, $12, $0C, $02, $02, $22, $12, $0E, $12, $22, $00
   DC.B  $0C, $08, $08, $08, $08, $08, $1C, $00, $00, $00, $36, $2A, $2A, $2A, $22, $00
   DC.B  $00, $00, $1E, $22, $22, $22, $22, $00, $00, $00, $1C, $22, $22, $22, $1C, $00
   DC.B  $00, $00, $1E, $22, $22, $1E, $02, $02, $00, $00, $3C, $22, $22, $3C, $20, $20
   DC.B  $00, $00, $3A, $06, $02, $02, $02, $00, $00, $00, $3C, $02, $1C, $20, $1E, $00
   DC.B  $04, $04, $1E, $04, $04, $24, $18, $00, $00, $00, $22, $22, $22, $32, $2C, $00
   DC.B  $00, $00, $22, $92, $14, $8C, $08, $00, $00, $00, $22, $22, $2A, $2A, $36, $00
   DC.B  $00, $00, $22, $14, $08, $14, $22, $00, $00, $00, $22, $92, $14, $8C, $08, $86
   DC.B  $00, $00, $3E, $10, $08, $04, $3E, $00, $38, $0C, $0C, $06, $0C, $0C, $38, $00
   DC.B  $08, $08, $08, $08, $08, $08, $08, $08, $0E, $18, $18, $30, $18, $18, $0E, $00
   DC.B  $2C, $1A, $00, $00, $00, $00, $00, $00, $7F, $7F, $7F, $7F, $7F, $7F, $7F, $7F
