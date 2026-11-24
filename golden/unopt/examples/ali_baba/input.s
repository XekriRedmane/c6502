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

option:
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

   LDA   MENU_DESC_PTR
   CLC
   ADC   #$03
   LDY   #$01
   STA   (FP),Y
   LDA   MENU_DESC_PTR+1
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
   LDY   #$03
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$09
   STA   HARGS+2
   LDA   #$00
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
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
   PLA
   TAX
   PLA
   RTS

set_cursor_pos:
   SUBROUTINE

   ; prologue: 1 arg bytes, 22 local bytes
   SEC
   LDA   SSP
   SBC   #$18
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$17
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
   SBC   #$50
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_else@1
.lb_skip@0:
   LDY   #$19
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
   LDA   #$28
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$07
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   #$15
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   DEY
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   STA   TEXT_ROW
   LDY   #$19
   LDA   (FP),Y
   LDY   #$0C
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
   LDA   #$28
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$0E
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   STA   TEXT_COL
   JMP   .if_end@0
.if_else@1:
   LDA   #$17
   LDY   #$11
   STA   (FP),Y
   STA   TEXT_ROW
   LDY   #$19
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$50
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   STA   TEXT_COL
.if_end@0:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$19
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

draw_option:
   SUBROUTINE

   ; prologue: 2 arg bytes, 7 local bytes
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
   CLC
   ADC   #$01
   LDY   #$01
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$02
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
   JSR   set_cursor_pos
   JSR   print_ctrl_ab
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$04
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$07
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   print_string
   JSR   print_ctrl_n

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0B
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

clear_input_buffer:
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$78
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
   BEQ   .loop@0_break
   LDA   #$A0
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   TAX
   LDY   #$09
   LDA   (FP),Y
   STA   INPUT_BUFFER,X
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:

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

copy_record_data:
   SUBROUTINE

   ; prologue: 2 arg bytes, 49 local bytes
   SEC
   LDA   SSP
   SBC   #$33
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$32
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
   CLC
   ADC   #$06
   LDY   #$01
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   ADC   #$00
   LDY   #$02
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$09
   STA   (FP),Y
   LDY   #$35
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #<INPUT_BUFFER
   INY
   STA   (FP),Y
   LDA   #>INPUT_BUFFER
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   LDY   #$35
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$23
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
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
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
   BNE   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDY   #$20
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$27
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$10
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
   LDY   #$1D
   LDA   (FP),Y
   CLC
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$1E
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
   JMP   .loop@1_start
.loop@1_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$35
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$32
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

draw_input_options:
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

   LDA   MENU_DESC_PTR
   STA   DPTR
   LDA   MENU_DESC_PTR+1
   STA   DPTR+1
   LDY   #$00
   LDA   (MENU_DESC_PTR),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@2_break
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$02
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   JSR   option
   LDY   #$07
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   BEQ   .if_end@2
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$09
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   copy_record_data
.if_end@2:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:

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

render_input_buffer:
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

   LDA   #$15
   LDY   #$01
   STA   (FP),Y
   STA   TEXT_ROW
   LDA   #$00
   INY
   STA   (FP),Y
   STA   TEXT_COL
   JSR   print_ctrl_ab
   LDA   #$00
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$78
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
   BEQ   .loop@3_break
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   TAX
   LDA   INPUT_BUFFER,X
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
   JSR   hrcg_cout
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:

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

setup_input:
   SUBROUTINE

   ; prologue: 2 arg bytes, 3 local bytes
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

   INY
   LDA   (FP),Y
   STA   MENU_DESC_PTR
   INY
   LDA   (FP),Y
   STA   MENU_DESC_PTR+1
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$01
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   ADC   #$00
   LDY   #$02
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
   JSR   call_input_init
   JSR   clear_input_buffer
   JSR   draw_input_options
   JSR   render_input_buffer

   ; epilogue
   CLC
   LDA   FP
   ADC   #$07
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

validate_input:
   SUBROUTINE

   ; prologue: 3 arg bytes, 28 local bytes
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

.loop@4_start:
   LDY   #$20
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$01
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   ADC   #$00
   LDY   #$02
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
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
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   CURRENT_SELECTION
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BEQ   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$18
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDY   #$0D
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$21
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
   PLA
   RTS
.if_end@5:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   option
   LDY   #$1A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$21
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
   ORA   #$00
   BNE   .lb_skip@2
   JMP   .loop@4_start
.lb_skip@2:
   LDY   #$0D
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$21
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
   PLA
   RTS

input_loop:
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

   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   setup_input
   LDY   #$23
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$01
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   ADC   #$00
   LDY   #$02
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
   STA   CURRENT_SELECTION
.loop@5_start:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   CURRENT_SELECTION
   LDY   #$01
   STA   (SSP),Y
   JSR   option
   LDY   #$04
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   JSR   set_inverse_video
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   draw_option
   JSR   read_keyboard
   LDY   #$08
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
   CMP   #$00
   BEQ   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   LDY   #$06
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS
.if_end@7:
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$09
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   validate_input
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   CURRENT_SELECTION
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BEQ   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@8
   LDY   #$12
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@8
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@9
.or_true@8:
   LDA   #$01
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@9:
   LDY   #$1D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@11
   LDA   #$09
   INY
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   tune_player
   JMP   .loop@5_start
.if_else@11:
   LDY   #$12
   LDA   (FP),Y
   STA   CURRENT_SELECTION
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   draw_option
   JMP   .loop@5_start

store_selection_to_record:
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

   LDA   MENU_DESC_PTR
   CLC
   ADC   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   MENU_DESC_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   CURRENT_SELECTION
   LDY   #$00
   STA   (DPTR),Y

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

read_paddle:
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   PTRIG
   INY
   STA   (FP),Y
.loop@6_continue:
   LDY   #$14
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   TAX
   LDA   PADDL,X
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
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@6_break
   LDY   #$02
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0C
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@6_continue
   LDA   #$FF
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$14
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
.loop@6_break:
   LDY   #$02
   LDA   (FP),Y

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

joy_to_zone:
   SUBROUTINE

   ; prologue: 2 arg bytes, 28 local bytes
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

   LDY   #$20
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LSR
   LSR
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   ASL
   INY
   ORA   (FP),Y
   STA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   TAX
   LDA   ZONE_THRESH_PLUS,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   SEC
   LDY   #$0C
   SBC   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0D
   SBC   (FP),Y
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
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
   BEQ   .if_end@13
   LDA   #$00
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$20
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
   PLA
   RTS
.if_end@13:
   LDY   #$20
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   TAX
   LDA   ZONE_THRESH_MINUS,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   SEC
   LDY   #$17
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$18
   SBC   (FP),Y
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
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
   BEQ   .if_end@14
   LDA   #$01
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$20
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
   PLA
   RTS
.if_end@14:
   LDA   #$02
   LDY   #$1C
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$20
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

check_joystick:
   SUBROUTINE

   ; prologue: 0 arg bytes, 38 local bytes
   SEC
   LDA   SSP
   SBC   #$28
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$27
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   JOY_SETTLE_DELAY
   JSR   WAIT
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
   JSR   read_paddle
   LDY   #$02
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   joy_zone_x
   DEY
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   joy_to_zone
   LDY   #$03
   STA   (FP),Y
   STA   joy_zone_x
   LDA   JOY_SETTLE_DELAY
   JSR   WAIT
   LDA   #$01
   LDY   #$04
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
   JSR   read_paddle
   LDY   #$05
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   joy_zone_y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   joy_to_zone
   LDY   #$06
   STA   (FP),Y
   STA   joy_zone_y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .if_else@16
.lb_skip@4:
   LDA   joy_zone_x
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@17
   LDA   #$04
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$27
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@17:
   LDA   joy_zone_x
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .if_end@15
.lb_skip@3:
   LDA   #$03
   LDY   #$14
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$27
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_else@16:
   LDA   joy_zone_x
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@19
   LDA   joy_zone_y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@20
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .cond_end@21
.cond_else@20:
   LDA   #$02
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.cond_end@21:
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$27
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
.if_end@15:
   LDA   BUTN
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@22
   LDA   #$00
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$27
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@22:
   LDA   #$FF
   LDY   #$26
   STA   (FP),Y
   STA   KBDSTRB
   LDA   NO_INPUT

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$28
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

check_input:
   SUBROUTINE

   ; prologue: 0 arg bytes, 84 local bytes
   SEC
   LDA   SSP
   SBC   #$56
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$55
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
   LDA   INPUT_MODE
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@23
   JSR   check_joystick
   LDY   #$07
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$56
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$55
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@23:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@8
   JMP   .if_end@24
.lb_skip@8:
   LDA   #$00
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@7_start:
   LDY   #$0E
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
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
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
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .loop@7_break
.lb_skip@7:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<DIR_KEYS
   INY
   STA   (FP),Y
   LDA   #>DIR_KEYS
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@31:
   BCC   .mul_skip@32
   CLC
   ADC   #$05
.mul_skip@32:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@31
   LDY   #$17
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   CLC
   LDY   #$16
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$17
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0D
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$19
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BEQ   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .or_true@27
.lb_skip@6:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<DIR_KEYS
   INY
   STA   (FP),Y
   LDA   #>DIR_KEYS
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@35:
   BCC   .mul_skip@36
   CLC
   ADC   #$05
.mul_skip@36:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@35
   LDY   #$2B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   CLC
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$0D
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$36
   CMP   (FP),Y
   BEQ   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@27
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@28
.or_true@27:
   LDA   #$01
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@28:
   LDY   #$3A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .or_true@25
.lb_skip@5:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<DIR_KEYS
   INY
   STA   (FP),Y
   LDA   #>DIR_KEYS
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@39:
   BCC   .mul_skip@40
   CLC
   ADC   #$05
.mul_skip@40:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@39
   LDY   #$41
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   CLC
   LDY   #$40
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$41
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   CLC
   LDY   #$47
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$4C
   CMP   (FP),Y
   BEQ   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@25
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@26
.or_true@25:
   LDA   #$01
   LDY   #$50
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@26:
   LDY   #$50
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@29
   LDY   #$0D
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$56
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$55
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@29:
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   JMP   .loop@7_start
.loop@7_break:
.if_end@24:
   LDA   #$FF
   LDY   #$54
   STA   (FP),Y
   STA   KBDSTRB
   LDA   NO_INPUT

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$56
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

read_keyboard:
   SUBROUTINE

   ; prologue: 0 arg bytes, 18 local bytes
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

   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   STA   KBDSTRB
   JSR   anim_tick_and_wait
   LDY   #$02
   STA   (FP),Y
   LDA   PRNG_OUTPUT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$7E
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@43
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@30
   JSR   verify_integrity
.if_end@30:
   JSR   tick_anim_counter
.loop@8_start:
   JSR   step_prng
   LDY   #$09
   STA   (FP),Y
   JSR   check_input
   LDY   #$0A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   NO_INPUT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .cmp_true@45
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@8_break
   JSR   idle_anim_handler
   JMP   .loop@8_start
.loop@8_break:
   LDY   #$0B
   LDA   (FP),Y
   STA   KBDSTRB
   JSR   anim_check_frame
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$14
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

check_combat_eligibility:
   SUBROUTINE

   ; prologue: 0 arg bytes, 62 local bytes
   SEC
   LDA   SSP
   SBC   #$40
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   STA   safe_to_rest
   STA   under_level
   LDA   ENTITY_PTR
   CLC
   ADC   #$0C
   LDY   #$01
   STA   (FP),Y
   LDA   ENTITY_PTR+1
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
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ROR
   STA   (FP),Y
   AND   #$07
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   STEPS_TAKEN
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   SEC
   LDY   #$0E
   SBC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$14
   STA   (FP),Y
   ASL
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
   BEQ   .if_end@32
   LDA   #$01
   STA   under_level

   ; epilogue
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@32:
   LDA   STEPS_TAKEN
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@33

   ; epilogue
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@33:
   LDA   ENTITY_PTR
   CLC
   ADC   #$06
   LDY   #$1B
   STA   (FP),Y
   LDA   ENTITY_PTR+1
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   HP_MASK
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$20
   AND   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$21
   AND   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   ENTITY_PTR
   CLC
   ADC   #$05
   INY
   STA   (FP),Y
   LDA   ENTITY_PTR+1
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
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   STRENGTH_MASK
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   SEC
   LDY   #$2B
   SBC   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2C
   SBC   (FP),Y
   BVC   .cmp_novf@51
   EOR   #$80
.cmp_novf@51:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@34

   ; epilogue
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@34:
   LDA   ENTITY_PTR
   CLC
   ADC   #$03
   LDY   #$2F
   STA   (FP),Y
   LDA   ENTITY_PTR+1
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
   LDY   #$31
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
   JSR   check_adjacent_threats
   LDY   #$32
   STA   (FP),Y
   LDA   ADJACENT_THREAT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@52
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@35

   ; epilogue
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@35:
   LDA   ENTITY_PTR
   CLC
   ADC   #$03
   LDY   #$37
   STA   (FP),Y
   LDA   ENTITY_PTR+1
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
   LDY   #$39
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
   JSR   check_encounter
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@54
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@36

   ; epilogue
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
   RTS
.if_end@36:
   LDA   #$01
   STA   safe_to_rest

   ; epilogue
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

configure_input:
   SUBROUTINE

   ; prologue: 0 arg bytes, 61 local bytes
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

   JSR   check_combat_eligibility
   LDA   #<GAME_INPUT_OPTIONS
   LDY   #$01
   STA   (FP),Y
   LDA   #>GAME_INPUT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #$05
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@56:
   BCC   .mul_skip@57
   CLC
   ADC   #$09
.mul_skip@57:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@56
   LDY   #$04
   STA   (FP),Y
   LDA   HARGS
   DEY
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   safe_to_rest
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<GAME_INPUT_OPTIONS
   LDY   #$07
   STA   (FP),Y
   LDA   #>GAME_INPUT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@58:
   BCC   .mul_skip@59
   CLC
   ADC   #$09
.mul_skip@59:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@58
   LDY   #$0A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   under_level
   LDY   #$00
   STA   (DPTR),Y
   ORA   #$00
   BEQ   .lnot_true@60
   LDA   #$00
   JMP   .lnot_end@61
.lnot_true@60:
   LDA   #$01
.lnot_end@61:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   #<GAME_INPUT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #>GAME_INPUT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@62:
   BCC   .mul_skip@63
   CLC
   ADC   #$09
.mul_skip@63:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@62
   LDY   #$13
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   LDY   #$12
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   ADC   (FP),Y
   LDY   #$15
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
   LDA   under_level
   BEQ   .lnot_true@64
   LDA   #$00
   JMP   .lnot_end@65
.lnot_true@64:
   LDA   #$01
.lnot_end@65:
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #<GAME_INPUT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #>GAME_INPUT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@66:
   BCC   .mul_skip@67
   CLC
   ADC   #$09
.mul_skip@67:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@66
   LDY   #$1C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$18
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   under_level
   BEQ   .cond_else@37
   LDA   #$03
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .cond_end@38
.cond_else@37:
   LDA   #$02
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.cond_end@38:
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #<GAME_INPUT_TABLE
   INY
   STA   (FP),Y
   LDA   #>GAME_INPUT_TABLE
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$23
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   TURN_ACTIVE
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@68
   LDA   #$00
   JMP   .cmp_end@69
.cmp_true@68:
   LDA   #$01
.cmp_end@69:
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@39
   LDA   MOVE_POINTS
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   JMP   .cond_end@40
.cond_else@39:
   LDA   #$00
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
.cond_end@40:
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #<GAME_INPUT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #>GAME_INPUT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@70:
   BCC   .mul_skip@71
   CLC
   ADC   #$09
.mul_skip@71:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@70
   LDY   #$34
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   CLC
   LDY   #$33
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$34
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$30
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<GAME_INPUT_OPTIONS
   LDY   #$37
   STA   (FP),Y
   LDA   #>GAME_INPUT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@72:
   BCC   .mul_skip@73
   CLC
   ADC   #$09
.mul_skip@73:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@72
   LDY   #$3A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
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

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$3F
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

get_input_table_ptr:
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

   LDA   #<GAME_INPUT_TABLE
   LDY   #$01
   STA   (FP),Y
   LDA   #>GAME_INPUT_TABLE
   INY
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
   PLA
   TAX
   PLA
   RTS

teardown_input:
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

   JSR   get_input_table_ptr
   LDY   #$01
   STA   (FP),Y
   TXA
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
   JSR   input_loop
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

menu_see_your_lot:
   SUBROUTINE

   JSR   display_char_stats
   LDA   #$12
   RTS

menu_check_attack:
   SUBROUTINE

   ; prologue: 0 arg bytes, 4 local bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   check_attack_eligible
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@41
   LDA   #$0E

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@41:
   LDA   #<ATTACK_DIR_TABLE
   LDY   #$02
   STA   (FP),Y
   LDA   #>ATTACK_DIR_TABLE
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
   DEY
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   JSR   input_loop
   LDY   #$04
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$06
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

menu_other_options:
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

   LDA   #<OTHER_OPTIONS_TABLE
   LDY   #$01
   STA   (FP),Y
   LDA   #>OTHER_OPTIONS_TABLE
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
   JSR   input_loop
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

retire_character:
   SUBROUTINE

   ; prologue: 0 arg bytes, 1 local bytes
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$02
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
   STA   ACTIVE_CHAR
   JSR   reorder_current_char
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   SCENE_RETIRE
   LDY   #$01
   STA   (SSP),Y
   JSR   scene_loop

   ; epilogue
   CLC
   LDA   FP
   ADC   #$03
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$02
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

menu_game_settings:
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

   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #<GAME_SETTINGS_TABLE
   INY
   STA   (FP),Y
   LDA   #>GAME_SETTINGS_TABLE
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
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   JSR   menu_game_settings_enter
   LDY   #$06
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

menu_game_settings_enter:
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

   LDA   #<GAME_SETTINGS_TABLE
   LDY   #$01
   STA   (FP),Y
   LDA   #>GAME_SETTINGS_TABLE
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
   JSR   input_loop
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

menu_settings_init:
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

   JSR   settings_display_init
   LDA   ANIM_INDEX
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$B0
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   DEY
   ADC   (FP),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #<S_GAME_SPEED
   INY
   STA   (FP),Y
   LDA   #>S_GAME_SPEED
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0E
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

settings_speed_up:
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

   JSR   store_selection_to_record
   LDA   ANIM_INDEX
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   MAX_GAME_SPEED
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$03
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   BVC   .cmp_novf@74
   EOR   #$80
.cmp_novf@74:
   ASL
   LDA   #$00
   ROL
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@42
   LDA   ANIM_INDEX
   INY
   STA   (FP),Y
   LDA   ANIM_INDEX
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   ANIM_INDEX
.if_end@42:
   LDA   #$17

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

settings_speed_down:
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

   JSR   store_selection_to_record
   LDA   ANIM_INDEX
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   MIN_GAME_SPEED
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$01
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   SBC   (FP),Y
   BVC   .cmp_novf@75
   EOR   #$80
.cmp_novf@75:
   ASL
   LDA   #$00
   ROL
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@43
   LDA   ANIM_INDEX
   INY
   STA   (FP),Y
   LDA   ANIM_INDEX
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   ANIM_INDEX
.if_end@43:
   LDA   #$17

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

settings_danger_up:
   SUBROUTINE

   JSR   store_selection_to_record
   JSR   inc_threshold_wrap
   LDA   #$17
   RTS

settings_danger_down:
   SUBROUTINE

   JSR   store_selection_to_record
   JSR   dec_threshold_wrap
   LDA   #$17
   RTS

menu_attack_target:
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

   LDA   #<ATTACK_TARGET_TABLE
   LDY   #$01
   STA   (FP),Y
   LDA   #>ATTACK_TARGET_TABLE
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
   JSR   input_loop
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

menu_drop_gold:
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

   LDA   #<DROP_GOLD_TABLE
   LDY   #$01
   STA   (FP),Y
   LDA   #>DROP_GOLD_TABLE
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
   JSR   input_loop
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

menu_drop_inc:
   SUBROUTINE

   JSR   store_selection_to_record
   LDA   #$36
   RTS

menu_drop_dec:
   SUBROUTINE

   JSR   store_selection_to_record
   LDA   #$37
   RTS

format_slot_number:
   SUBROUTINE

   ; prologue: 0 arg bytes, 47 local bytes
   SEC
   LDA   SSP
   SBC   #$31
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$30
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$A0
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   slot_number
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
   SBC   #$0A
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
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@44
   LDA   #$B1
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$0A
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$03
   STA   (FP),Y
.if_end@44:
   LDA   #<S_CONTINUE_GAME_NUMBER
   LDY   #$0E
   STA   (FP),Y
   LDA   #>S_CONTINUE_GAME_NUMBER
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$16
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$02
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<S_SAVE_IN_SLOT
   LDY   #$14
   STA   (FP),Y
   LDA   #>S_SAVE_IN_SLOT
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$1A
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
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
   LDY   #$02
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$B0
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   DEY
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #<S_CONTINUE_GAME_NUMBER
   INY
   STA   (FP),Y
   LDA   #>S_CONTINUE_GAME_NUMBER
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$16
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$B0
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   DEY
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   #<S_SAVE_IN_SLOT
   INY
   STA   (FP),Y
   LDA   #>S_SAVE_IN_SLOT
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$1A
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$29
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$31
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$30
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

slot_update:
   SUBROUTINE

   JSR   store_selection_to_record
   JSR   format_slot_number
   RTS

slot_inc:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
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

   LDA   slot_number
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   MAX_SLOT_NUMBER
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
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
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$05
   SBC   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   BVC   .cmp_novf@77
   EOR   #$80
.cmp_novf@77:
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
   BEQ   .if_end@45
   LDA   slot_number
   INY
   STA   (FP),Y
   LDA   slot_number
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   slot_number
.if_end@45:
   JSR   slot_update

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
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
   RTS

slot_dec:
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

   LDA   slot_number
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@78
   EOR   #$80
.cmp_novf@78:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@46
   LDA   slot_number
   INY
   STA   (FP),Y
   LDA   slot_number
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   slot_number
.if_end@46:
   JSR   slot_update

   ; epilogue
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
   RTS

init_world:
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

   LDA   #<SAVE_LOAD_TABLE
   LDY   #$01
   STA   (FP),Y
   LDA   #>SAVE_LOAD_TABLE
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
   JSR   input_loop
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

slot_inc_enter:
   SUBROUTINE

   JSR   slot_inc
   LDA   #$20
   RTS

slot_dec_enter:
   SUBROUTINE

   JSR   slot_dec
   LDA   #$20
   RTS

enter_save_game_slot:
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

   LDA   #<SAVE_GAME_SLOT_TABLE
   LDY   #$01
   STA   (FP),Y
   LDA   #>SAVE_GAME_SLOT_TABLE
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
   JSR   input_loop
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

slot_dec_reenter:
   SUBROUTINE

   JSR   slot_dec
   LDA   #$23
   RTS

slot_inc_reenter:
   SUBROUTINE

   JSR   slot_inc
   LDA   #$23
   RTS

call_input_init:
   SUBROUTINE

   ; prologue: 1 arg bytes, 15 local bytes
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
   BEQ   .cmp_true@79
   LDA   #$00
   JMP   .cmp_end@80
.cmp_true@79:
   LDA   #$01
.cmp_end@80:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .case@10
.lb_skip@10:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@81
   LDA   #$00
   JMP   .cmp_end@82
.cmp_true@81:
   LDA   #$01
.cmp_end@82:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .case@11
.lb_skip@9:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@83
   LDA   #$00
   JMP   .cmp_end@84
.cmp_true@83:
   LDA   #$01
.cmp_end@84:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@12
   LDY   #$01
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@85
   LDA   #$00
   JMP   .cmp_end@86
.cmp_true@85:
   LDA   #$01
.cmp_end@86:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@13
   LDY   #$01
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@87
   LDA   #$00
   JMP   .cmp_end@88
.cmp_true@87:
   LDA   #$01
.cmp_end@88:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@14
   LDY   #$01
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@89
   LDA   #$00
   JMP   .cmp_end@90
.cmp_true@89:
   LDA   #$01
.cmp_end@90:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@15
   JMP   .switch@9_break
.case@10:
   JSR   configure_input
   LDY   #$0F
   STA   (FP),Y
   JMP   .switch@9_break
.case@11:
   JSR   menu_settings_init
   JMP   .switch@9_break
.case@12:
   JSR   init_attack_display
   JMP   .switch@9_break
.case@13:
   JSR   init_drop_display
   JMP   .switch@9_break
.case@14:
.case@15:
   JSR   format_slot_number
.switch@9_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$12
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

enter_char_select:
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

   LDA   #<CHAR_SELECT_TABLE
   LDY   #$01
   STA   (FP),Y
   LDA   #>CHAR_SELECT_TABLE
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
   JSR   input_loop
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

char_select_init:
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

   JSR   find_first_pc
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@47
   JSR   char_select_home
   LDY   #$02
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS
.if_end@47:
   JSR   set_cursor_row21
   LDA   #$35
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
   JSR   display_message
   LDA   #$02

   ; epilogue
   PHA
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
   PLA
   RTS

char_select_next:
   SUBROUTINE

   JSR   store_selection_to_record
   JSR   find_next_pc_wrap
   LDA   #$27
   RTS

char_select_prev:
   SUBROUTINE

   JSR   store_selection_to_record
   JSR   find_prev_pc
   LDA   #$27
   RTS

char_select_patch:
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

   LDA   #<CHAR_SELECT_OPTIONS
   LDY   #$01
   STA   (FP),Y
   LDA   #>CHAR_SELECT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #$05
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@91:
   BCC   .mul_skip@92
   CLC
   ADC   #$09
.mul_skip@92:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@91
   LDY   #$04
   STA   (FP),Y
   LDA   HARGS
   DEY
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$15
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<CHAR_SELECT_OPTIONS
   LDY   #$07
   STA   (FP),Y
   LDA   #>CHAR_SELECT_OPTIONS
   INY
   STA   (FP),Y
   LDA   #$04
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@93:
   BCC   .mul_skip@94
   CLC
   ADC   #$09
.mul_skip@94:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@93
   LDY   #$0A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$02
   LDY   #$0D
   STA   (FP),Y
   LDA   #<CHAR_SELECT_TABLE
   INY
   STA   (FP),Y
   LDA   #>CHAR_SELECT_TABLE
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JSR   enter_char_select
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

char_select_here:
   SUBROUTINE

   ; prologue: 0 arg bytes, 4 local bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
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
   STA   ACTIVE_CHAR
   INY
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
   DEY
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   JSR   char_select_patch
   LDY   #$04
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$06
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

char_select_home:
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

   LDA   CURRENT_PLAYER
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$09
   BEQ   .cmp_true@95
   LDA   #$00
   JMP   .cmp_end@96
.cmp_true@95:
   LDA   #$01
.cmp_end@96:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@48
   JSR   char_select_here
   LDY   #$05
   STA   (FP),Y

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
.if_end@48:
   LDA   CURRENT_PLAYER
   STA   ACTIVE_CHAR
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   LDA   #$01
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   char_select_patch
   LDY   #$08
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0A
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

start_game_input:
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

   LDA   #<ALT_INPUT_TABLE
   LDY   #$01
   STA   (FP),Y
   LDA   #>ALT_INPUT_TABLE
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
   JSR   input_loop
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

setup_data_iob:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$04
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$05
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$04
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<SCENE_TEXT_DATA
   LDY   #$07
   STA   (FP),Y
   LDA   #>SCENE_TEXT_DATA
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$08
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
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
   RTS

verify_game_disk:
   SUBROUTINE

   ; prologue: 0 arg bytes, 32 local bytes
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

   JSR   setup_data_iob
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$04
   LDY   #$01
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   GAME_SIG_TRACK
   LDY   #$00
   STA   (DPTR),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$05
   LDY   #$03
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   GAME_SIG_SECTOR
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$02
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@16_continue:
   JSR   disk_read
   LDY   #$07
   STA   (FP),Y
   ORA   #$00
   BEQ   .loop@16_break
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$08
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@97
   LDA   #$00
   JMP   .cmp_end@98
.cmp_true@97:
   LDA   #$01
.cmp_end@98:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@16_continue
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$22
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
   PLA
   RTS
.loop@16_break:
   LDA   SCENE_TEXT_DATA
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@99
   LDA   #$00
   JMP   .cmp_end@100
.cmp_true@99:
   LDA   #$01
.cmp_end@100:
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@52
   LDA   SCENE_TEXT_DATA+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$0F
   BNE   .cmp_true@101
   LDA   #$00
   JMP   .cmp_end@102
.cmp_true@101:
   LDA   #$01
.cmp_end@102:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@52
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@53
.or_true@52:
   LDA   #$01
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@53:
   LDY   #$17
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@50
   LDA   SCENE_TEXT_DATA+2
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .cmp_true@103
   LDA   #$00
   JMP   .cmp_end@104
.cmp_true@103:
   LDA   #$01
.cmp_end@104:
   LDY   #$1C
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
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@51:
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$22
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

wait_keypress:
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

   LDA   #$30
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
   JSR   display_message
   LDA   #$00
   LDY   #$02
   STA   (FP),Y
   STA   KBDSTRB
.loop@17_continue:
   LDA   KBD
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$07
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
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@17_continue

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0B
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

prompt_insert_disk:
   SUBROUTINE

   ; prologue: 0 arg bytes, 1 local bytes
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$02
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   set_cursor_row21
   LDA   #$03
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
   JSR   display_message
   JSR   wait_keypress

   ; epilogue
   CLC
   LDA   FP
   ADC   #$03
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$02
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

ensure_game_disk:
   SUBROUTINE

   ; prologue: 0 arg bytes, 1 local bytes
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$02
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.loop@18_continue:
   JSR   verify_game_disk
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BEQ   .loop@18_break
   JSR   prompt_insert_disk
   JMP   .loop@18_continue
.loop@18_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$03
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$02
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

format_disk_track:
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

   LDA   #$04
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
   JSR   rwts_dispatch
   LDY   #$02
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$04
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

write_disk_signature:
   SUBROUTINE

   ; prologue: 0 arg bytes, 28 local bytes
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@19_start:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$03
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
   BEQ   .loop@19_break
   LDY   #$02
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
   LDA   DISK_SIGNATURE,X
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   TAX
   LDY   #$0C
   LDA   (FP),Y
   STA   SCENE_TEXT_DATA,X
   LDY   #$02
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@19_start
.loop@19_break:
   JSR   setup_data_iob
   LDA   #$03
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@20_start:
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@107
   LDA   #$00
   JMP   .cmp_end@108
.cmp_true@107:
   LDA   #$01
.cmp_end@108:
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@20_break
   JSR   disk_write
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@109
   LDA   #$00
   JMP   .lnot_end@110
.lnot_true@109:
   LDA   #$01
.lnot_end@110:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@54
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$1E
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
   PLA
   RTS
.if_end@54:
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   JMP   .loop@20_start
.loop@20_break:
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$1E
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
   PLA
   RTS

verify_disk_signature:
   SUBROUTINE

   ; prologue: 0 arg bytes, 32 local bytes
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

   JSR   setup_data_iob
   LDA   #$03
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@21_continue:
   JSR   disk_read
   LDY   #$03
   STA   (FP),Y
   ORA   #$00
   BEQ   .loop@21_break
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$04
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@111
   LDA   #$00
   JMP   .cmp_end@112
.cmp_true@111:
   LDA   #$01
.cmp_end@112:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@21_continue
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$22
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
   PLA
   RTS
.loop@21_break:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@22_start:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
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
   LDA   #$03
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
   BNE   .lb_skip@11
   JMP   .loop@22_break
.lb_skip@11:
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   TAX
   LDA   SCENE_TEXT_DATA,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   TAX
   LDA   DISK_SIGNATURE,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .cmp_true@113
   LDA   #$00
   JMP   .cmp_end@114
.cmp_true@113:
   LDA   #$01
.cmp_end@114:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@56
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$22
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
   PLA
   RTS
.if_end@56:
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   JMP   .loop@22_start
.loop@22_break:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$22
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
   PLA
   RTS

setup_slot_iob:
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

   LDA   slot_number
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   GAME_DISK_SLOTS
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$03
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   BVC   .cmp_novf@115
   EOR   #$80
.cmp_novf@115:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .cond_else@57
   LDA   slot_number
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   GAME_DISK_SLOTS
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   SEC
   LDY   #$0B
   SBC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   JMP   .cond_end@58
.cond_else@57:
   LDA   slot_number
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
.cond_end@58:
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   TAX
   LDA   SAVE_TRACK_BASE,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1B
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
.mul_loop@116:
   BCC   .mul_skip@117
   CLC
   ADC   #$02
.mul_skip@117:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@116
   LDY   #$1E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$04
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$21
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$24
   STA   (FP),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$05
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$24
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<LOCATIONS
   LDY   #$27
   STA   (FP),Y
   LDA   #>LOCATIONS
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$08
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
   ADC   #$00
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
   LDY   #$28
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$2B
   STA   (FP),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$0B
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
   ADC   #$00
   INY
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

advance_sector:
   SUBROUTINE

   ; prologue: 0 arg bytes, 23 local bytes
   SEC
   LDA   SSP
   SBC   #$19
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$18
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$08
   LDY   #$01
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
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
   LDY   #$03
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$06
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$05
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$05
   LDY   #$07
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
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
   LDY   #$09
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   SECTORS_PER_TRACK
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SEC
   LDY   #$0D
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   SBC   (FP),Y
   BVC   .cmp_novf@118
   EOR   #$80
.cmp_novf@118:
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
   BEQ   .if_end@59
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$05
   INY
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   RWTS_IOB_PTR
   CLC
   ADC   #$04
   LDY   #$14
   STA   (FP),Y
   LDA   RWTS_IOB_PTR+1
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
   LDY   #$16
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.if_end@59:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$19
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$18
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

transfer_game_data:
   SUBROUTINE

   ; prologue: 1 arg bytes, 14 local bytes
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

   LDA   #$02
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   SAVE_SECTOR_COUNT
   INY
   STA   (FP),Y
   JSR   setup_slot_iob
.loop@23_start:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   rwts_dispatch
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@61
   LDY   #$02
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@119
   LDA   #$00
   JMP   .cmp_end@120
.cmp_true@119:
   LDA   #$01
.cmp_end@120:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@23_start
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$11
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
   PLA
   RTS
.if_else@61:
   JSR   advance_sector
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@121
   LDA   #$00
   JMP   .cmp_end@122
.cmp_true@121:
   LDA   #$01
.cmp_end@122:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@12
   JMP   .loop@23_start
.lb_skip@12:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$11
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
   PLA
   RTS

write_game_data:
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

   LDA   #$02
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
   JSR   transfer_game_data
   LDY   #$02
   STA   (FP),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
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

read_game_data:
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
   JSR   transfer_game_data
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@64
   JSR   set_cursor_row21
   LDA   #$3B
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
   JSR   display_message
   JSR   cold_restart
.if_end@64:

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

continue_game:
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

   LDA   slot_number
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   GAME_DISK_SLOTS
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$03
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   BVC   .cmp_novf@123
   EOR   #$80
.cmp_novf@123:
   ASL
   LDA   #$00
   ROL
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@65
   JSR   ensure_game_disk
   JSR   read_game_data
   LDA   #$00

   ; epilogue
   PHA
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
   PLA
   RTS
.if_end@65:
   JSR   set_cursor_row21
   LDA   #$33
   LDY   #$07
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
   JSR   display_message
   JSR   wait_keypress
   JSR   verify_disk_signature
   LDY   #$08
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@66
   JSR   set_cursor_row21
   LDA   #$16
   LDY   #$09
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
   JSR   display_message
   JSR   ensure_game_disk
   LDA   #$20

   ; epilogue
   PHA
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
   PLA
   RTS
.if_end@66:
   JSR   read_game_data
   JSR   prompt_insert_disk
   JSR   ensure_game_disk
   LDA   #$00

   ; epilogue
   PHA
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
   PLA
   RTS

save_game:
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

   LDA   slot_number
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   GAME_DISK_SLOTS
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$03
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   BVC   .cmp_novf@124
   EOR   #$80
.cmp_novf@124:
   ASL
   LDA   #$00
   ROL
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@67
   JSR   ensure_game_disk
   JSR   write_game_data
   LDA   #$02

   ; epilogue
   PHA
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
   PLA
   RTS
.if_end@67:
   LDA   scenario_init_flag
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@125
   LDA   #$00
   JMP   .cmp_end@126
.cmp_true@125:
   LDA   #$01
.cmp_end@126:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@68
   JSR   ensure_game_disk
   LDA   #$42
   LDY   #$0B
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
   JSR   scene_loop
   LDA   #$01
   LDY   #$0C
   STA   (FP),Y
   STA   scenario_init_flag
.if_end@68:
   JSR   enter_scenario_disk
   LDY   #$0D
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

enter_scenario_disk:
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

   LDA   #<SCENARIO_DISK_TABLE
   LDY   #$01
   STA   (FP),Y
   LDA   #>SCENARIO_DISK_TABLE
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
   JSR   input_loop
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

scenario_done:
   SUBROUTINE

   JSR   prompt_insert_disk
   JSR   ensure_game_disk
   LDA   #$02
   RTS

cancel_scenario:
   SUBROUTINE

   JSR   ensure_game_disk
   LDA   #$02
   RTS

display_insert_msg:
   SUBROUTINE

   ; prologue: 0 arg bytes, 1 local bytes
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$02
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   set_cursor_row21
   LDA   #$1B
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
   JSR   display_message

   ; epilogue
   CLC
   LDA   FP
   ADC   #$03
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$02
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

format_scenario_disk:
   SUBROUTINE

   ; prologue: 0 arg bytes, 12 local bytes
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

   JSR   verify_game_disk
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@127
   LDA   #$00
   JMP   .lnot_end@128
.lnot_true@127:
   LDA   #$01
.lnot_end@128:
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@69
   JSR   set_cursor_row21
   LDA   #$33
   LDY   #$04
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
   JSR   display_message
   JSR   enter_scenario_disk
   LDY   #$05
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0E
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
   PLA
   RTS
.if_end@69:
   JSR   verify_disk_signature
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@70
   JSR   format_disk_track
   LDY   #$07
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@71
   JSR   write_disk_signature
   LDY   #$08
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@71
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@72
.or_true@71:
   LDA   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@72:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@73
   JSR   display_insert_msg
   JSR   scenario_done
   LDY   #$0B
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0E
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
   PLA
   RTS
.if_end@73:
.if_end@70:
   JSR   write_game_data
   JSR   scenario_done
   LDY   #$0C
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0E
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

use_scenario_disk:
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

   JSR   verify_disk_signature
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@75
   JSR   set_cursor_row21
   LDA   #$16
   LDY   #$02
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   JSR   display_message
   JMP   .if_end@74
.if_else@75:
   JSR   write_game_data
.if_end@74:
   JSR   scenario_done
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
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

GAME_INPUT_TABLE:
   DC.B  $07
   DC.B  $03
   DC.B  $01
   DC.W  GAME_INPUT_OPTIONS

ATTACK_DIR_TABLE:
   DC.B  $05
   DC.B  $01
   DS.B  1
   DC.W  ATTACK_DIR_OPTIONS

OTHER_OPTIONS_TABLE:
   DC.B  $06
   DC.B  $01
   DS.B  1
   DC.W  OTHER_OPTIONS_OPTIONS

GAME_SETTINGS_TABLE:
   DC.B  $09
   DC.B  $01
   DC.B  $02
   DC.W  GAME_SETTINGS_OPTIONS

ATTACK_TARGET_TABLE:
   DC.B  $03
   DC.B  $02
   DC.B  $03
   DC.W  ATTACK_TARGET_OPTIONS

DROP_GOLD_TABLE:
   DC.B  $05
   DC.B  $02
   DC.B  $04
   DC.W  DROP_GOLD_OPTIONS

SAVE_LOAD_TABLE:
   DC.B  $04
   DC.B  $01
   DC.B  $05
   DC.W  SAVE_LOAD_OPTIONS

SAVE_GAME_SLOT_TABLE:
   DC.B  $04
   DC.B  $02
   DC.B  $06
   DC.W  SAVE_GAME_SLOT_OPTIONS

SCENARIO_DISK_TABLE:
   DC.B  $03
   DC.B  $01
   DS.B  1
   DC.W  SCENARIO_DISK_OPTIONS

CHAR_SELECT_TABLE:
   DC.B  $06
   DC.B  $02
   DS.B  1
   DC.W  CHAR_SELECT_OPTIONS

ALT_INPUT_TABLE:
   DC.B  $03
   DC.B  $02
   DS.B  1
   DC.W  ALT_INPUT_OPTIONS

S_SOUND_IS_ON:
   DC.B  $0C, $D3, $CF, $D5, $CE, $C4, $A0, $C9, $D3, $A0, $CF
   DC.B  $CE, $A0

S_DANGER:
   DC.B  $0B, $C4, $C1, $CE, $C7, $C5, $D2
   DC.B  $A0, $BA, $A0, $A0, $A0

S_ADD_NAME:
   DC.B  $18, $C1, $C4, $C4
   DC.B  $A0, $B1, $A0, $A0, $AE, $A0, $A0, $AE, $B5, $A0, $C6, $CC, $FE, $A0, $D8, $C1
   DC.B  $A0, $B5, $A0, $F2, $C2

S_CHEST_MESSAGE:
   DC.B  $1B, $D4, $C8, $C5, $A0, $D4, $D2, $C5, $C1, $D3, $D5, $D2, $C5, $A0, $C3, $C8
   DC.B  $C5, $D3, $D4, $A0, $CF, $D0, $C5, $CE, $D3, $A0, $D5, $D0
   DC.B  $1D, $D4, $CF, $A0, $D2, $C5, $D6, $C5, $C1, $CC, $A0
   DC.B  $A0, $A0, $CC, $A0, $EB
   DC.B  $B0, $A0, $C7, $CF, $CC, $C4, $A0, $C4, $D5, $C3, $C1, $D4, $D3, $AE
   DC.B  $1B, $D9, $CF, $D5, $A0, $CE, $CF, $D7, $A0, $C8, $C1, $D6, $C5, $A0
   DC.B  $A0, $E4, $CF, $D2, $A0
   DC.B  $B0, $A0, $C4, $D5, $C3, $C1, $D4, $D3, $AE
   DS.B  1

S_ATTACK_TARGET:
   DC.B  $1B, $C1, $D4, $D4, $C1, $C3, $CB
   DC.B  $A0, $B5, $E9, $BC, $A5, $A0, $D3, $C9, $C2, $C3, $CD, $A0, $A0, $D4, $B1, $A0
   DC.B  $A0, $A0, $A0, $A0, $C9

S_MISSES:
   DC.B  $06, $CD, $C9, $D3, $D3, $C5, $D3

S_DUCATS_TO_DROP:
   DC.B  $17, $C4, $D5, $C3, $C1, $D4, $D3, $A0, $D4, $CF, $A0, $C4, $D2, $CF, $D0, $A0
   DC.B  $BA, $A0
   DC.B  $A0, $B4, $CC, $A0, $BA
   DC.B  $B0

S_PAY_FOR:
   DC.B  $23, $D0, $C1, $D9, $A0
   DC.B  $B0, $B0, $B0, $B0, $B0
   DC.B  $B0, $A0, $C6, $CF, $D2, $A0
   DC.B  $A0, $DF, $A0, $A0, $A0, $A0, $D9, $A0, $E0, $A0, $A0, $A0, $C2, $B7, $A0, $B1
   DC.B  $C5, $A0, $C1, $F5

GAME_SIG_TRACK:
   DC.B  $11

GAME_SIG_SECTOR:
   DS.B  1

S_DROP_ARMOR:
   DC.B  $0A, $C4, $D2, $CF, $D0, $A0, $C1, $D2, $CD, $CF, $D2

S_RETIRE:
   DC.B  $06, $D2, $C5, $D4, $C9, $D2, $C5

S_ADD_SOMEONE:
   DC.B  $0B, $C1, $C4, $C4, $A0, $D3, $CF, $CD, $C5, $CF, $CE, $C5

S_DROP_GOLD:
   DC.B  $09, $C4, $D2, $CF, $D0, $A0, $C7, $CF, $CC, $C4

S_SAVE_GAME:
   DC.B  $09, $D3, $C1, $D6, $C5, $A0, $C7, $C1, $CD, $C5

S_TO_THIS_ROOM:
   DC.B  $0C, $D4, $CF, $A0, $D4, $C8, $C9, $D3, $A0, $D2, $CF, $CF, $CD

S_TO_ALI_BABAS_HOME:
   DC.B  $12, $D4, $CF, $A0, $C1, $CC, $C9, $A0, $C2, $C1, $C2, $C1, $A7, $D3, $A0, $C8
   DC.B  $CF, $CD, $C5

S_INCREASE:
   DC.B  $01, $A3

S_DECREASE:
   DC.B  $01, $C0

S_SEE_YOUR_LOT:
   DC.B  $0C, $D3, $C5, $C5, $A0, $D9, $CF, $D5, $D2, $A0, $CC, $CF, $D4

S_REST:
   DC.B  $04, $D2, $C5, $D3, $D4

S_DEFEND:
   DC.B  $06, $C4, $C5, $C6, $C5, $CE, $C4

S_STOP:
   DC.B  $04, $D3, $D4, $CF, $D0

S_CONTINUE_GAME_NUMBER:
   DC.B  $17, $C3, $CF, $CE, $D4, $C9, $CE, $D5, $C5, $A0, $C7, $C1, $CD, $C5, $A0, $CE
   DC.B  $D5, $CD, $C2, $C5, $D2, $A0
   DC.B  $CE, $CE

S_BEGIN_NEW_GAME:
   DC.B  $0E, $C2, $C5, $C7, $C9, $CE, $A0, $CE, $C5, $D7, $A0, $C7, $C1, $CD, $C5

S_USE_PREVIOUS_DISK:
   DC.B  $26, $D5, $D3, $C5, $A0, $D0, $D2, $C5, $D6, $C9, $CF, $D5, $D3, $CC, $D9, $A0
   DC.B  $C6, $CF, $D2, $CD, $C1, $D4, $D4, $C5, $C4, $A0, $D3, $C3, $C5, $CE, $C1, $D2
   DC.B  $C9, $CF, $A0, $C4, $C9, $D3, $CB

S_FORMAT_NEW_DISK:
   DC.B  $20, $C6, $CF, $D2, $CD, $C1, $D4, $A0, $C1, $CE, $C4, $A0, $D5, $D3, $C5, $A0
   DC.B  $CE, $C5, $D7, $A0, $D3, $C3, $C5, $CE, $C1, $D2, $C9, $CF, $A0, $C4, $C9, $D3
   DC.B  $CB

S_CANCEL_REQUEST:
   DC.B  $0E, $C3, $C1, $CE, $C3, $C5, $CC, $A0, $D2, $C5, $D1, $D5, $C5, $D3, $D4

S_ATTACK_ABOVE:
   DC.B  $10, $C1, $D4, $D4, $C1, $C3, $CB, $A0, $C1, $C2, $CF, $D6, $C5, $A0, $D9, $CF
   DC.B  $D5

S_ATTACK_LEFT:
   DC.B  $0B, $C1, $D4, $D4, $C1, $C3, $CB, $A0, $CC, $C5, $C6, $D4

S_ATTACK_RIGHT:
   DC.B  $0C, $C1, $D4, $D4, $C1, $C3, $CB, $A0, $D2, $C9, $C7, $C8, $D4

S_ATTACK_BELOW:
   DC.B  $10, $C1, $D4, $D4, $C1, $C3, $CB, $A0, $C2, $C5, $CC, $CF, $D7, $A0, $D9, $CF
   DC.B  $D5

S_ATTACK_SOMEONE_ELSE:
   DC.B  $13, $C1, $D4, $D4, $C1, $C3, $CB, $A0, $D3, $CF, $CD, $C5, $CF, $CE, $C5, $A0
   DC.B  $C5, $CC, $D3, $C5

S_CANCEL_ATTACK:
   DC.B  $0D, $C3, $C1, $CE, $C3, $C5, $CC, $A0, $C1, $D4, $D4, $C1, $C3, $CB

S_KEEP_ALL_GOLD:
   DC.B  $0D, $CB, $C5, $C5, $D0, $A0, $C1, $CC, $CC, $A0, $C7, $CF, $CC, $C4

S_BUY_SOMETHING_ELSE:
   DC.B  $12, $C2, $D5, $D9, $A0, $D3, $CF, $CD, $C5, $D4, $C8, $C9, $CE, $C7, $A0, $C5
   DC.B  $CC, $D3, $C5

S_NO_SALE:
   DC.B  $07, $CE, $CF, $A0, $D3, $C1, $CC, $C5

S_ATTACK:
   DC.B  $06, $C1, $D4, $D4, $C1, $C3, $CB

S_OTHER_OPTIONS:
   DC.B  $0D, $CF, $D4, $C8, $C5, $D2, $A0, $CF, $D0, $D4, $C9, $CF, $CE, $D3

S_MOVE:
   DC.B  $04, $CD, $CF, $D6, $C5

S_GAME_SPEED:
   DC.B  $0E, $C7, $C1, $CD, $C5, $A0, $D3, $D0, $C5, $C5, $C4, $A0, $BA, $A0
   DC.B  $B0

S_SAVE_IN_SLOT:
   DC.B  $1B, $D3, $C1, $D6, $C5, $A0, $C7, $C1, $CD, $C5, $A0, $C9, $CE, $A0, $D3, $CC
   DC.B  $CF, $D4, $A0, $CE, $D5, $CD, $C2, $C5, $D2, $A0
   DC.B  $B0, $B0

GAME_INPUT_OPTIONS:
   DC.B  $01
   DC.B  $07
   DC.B  $04, $02, $07, $05
   DC.W  S_MOVE
   DC.B  $02
   DC.B  $01
   DC.B  $2F
   DC.B  $01, $03, $05, $06
   DC.W  S_ATTACK
   DC.B  $14
   DC.B  $01
   DC.B  $57
   DC.B  $02, $04, $06, $04
   DC.W  S_DEFEND
   DC.B  $09
   DC.B  $01
   DC.B  $57
   DC.B  $03, $01, $03, $07
   DC.W  S_STOP
   DC.B  $05
   DC.B  $01
   DC.B  $14
   DC.B  $07, $06, $01, $02
   DC.W  S_OTHER_OPTIONS
   DC.B  $15
   DC.B  $01
   DC.B  $3C
   DC.B  $05, $07, $02, $03
   DC.W  S_REST
   DC.B  $08
   DC.B  $01
   DC.B  $64
   DC.B  $06, $05, $04, $01
   DC.W  S_SEE_YOUR_LOT
   DC.B  $13

ATTACK_DIR_OPTIONS:
   DC.B  $01
   DC.B  $39
   DC.B  $03, $05, $02, $04
   DC.W  S_DEFEND
   DC.B  $09
   DC.B  $01
   DC.B  $29
   DC.B  $03, $05, $04, $01
   DC.W  S_ATTACK_LEFT
   DC.B  $0B
   DC.B  $01
   DC.B  $0C
   DC.B  $05, $01, $02, $04
   DC.W  S_ATTACK_ABOVE
   DC.B  $0C
   DC.B  $01
   DC.B  $44
   DC.B  $03, $05, $01, $02
   DC.W  S_ATTACK_RIGHT
   DC.B  $0A
   DC.B  $01
   DC.B  $5C
   DC.B  $01, $03, $02, $04
   DC.W  S_ATTACK_BELOW
   DC.B  $0D

OTHER_OPTIONS_OPTIONS:
   DC.B  $01
   DC.B  $05
   DC.B  $03, $02, $06, $04
   DC.W  S_MOVE
   DC.B  $02
   DC.B  $01
   DC.B  $2D
   DC.B  $01, $03, $04, $05
   DC.W  S_DROP_GOLD
   DC.B  $35
   DC.B  $01
   DC.B  $55
   DC.B  $02, $01, $05, $06
   DC.W  S_DROP_ARMOR
   DC.B  $34
   DC.B  $01
   DC.B  $15
   DC.B  $06, $05, $01, $02
   DC.W  S_OTHER_OPTIONS
   DC.B  $16
   DC.B  $01
   DC.B  $3D
   DC.B  $04, $06, $02, $03
   DC.W  S_RETIRE
   DC.B  $33
   DC.B  $01
   DC.B  $65
   DC.B  $05, $04, $03, $01
   DC.W  S_ADD_SOMEONE
   DC.B  $28

GAME_SETTINGS_OPTIONS:
   DC.B  $01
   DC.B  $03
   DC.B  $05, $09, $08, $03
   DC.W  S_MOVE
   DC.B  $02
   DC.B  $01
   DC.B  $13
   DS.B  4
   DC.W  S_GAME_SPEED
   DS.B  1
   DC.B  $01
   DC.B  $23
   DC.B  $07, $07, $01, $04
   DC.W  S_INCREASE
   DC.B  $18
   DC.B  $01
   DC.B  $25
   DC.B  $08, $08, $03, $09
   DC.W  S_DECREASE
   DC.B  $19
   DC.B  $01
   DC.B  $53
   DC.B  $09, $01, $09, $07
   DC.W  S_SOUND_IS_ON
   DC.B  $06
   DC.B  $01
   DC.B  $66
   DS.B  4
   DC.W  S_DANGER
   DS.B  1
   DC.B  $01
   DC.B  $73
   DC.B  $03, $03, $05, $08
   DC.W  S_INCREASE
   DC.B  $1A
   DC.B  $01
   DC.B  $75
   DC.B  $04, $04, $07, $01
   DC.W  S_DECREASE
   DC.B  $1B
   DC.B  $01
   DC.B  $2B
   DC.B  $01, $05, $04, $05
   DC.W  S_SAVE_GAME
   DC.B  $23

ATTACK_TARGET_OPTIONS:
   DC.B  $01
   DC.B  $06
   DC.B  $03, $02, $00, $00
   DC.W  S_ATTACK_TARGET
   DC.B  $10
   DC.B  $01
   DC.B  $2E
   DC.B  $01, $03, $00, $00
   DC.W  S_ATTACK_SOMEONE_ELSE
   DC.B  $0F
   DC.B  $01
   DC.B  $56
   DC.B  $02, $01, $00, $00
   DC.W  S_CANCEL_ATTACK
   DS.B  1

DROP_GOLD_OPTIONS:
   DC.B  $01
   DC.B  $2A
   DS.B  4
   DC.W  S_DUCATS_TO_DROP
   DC.B  $1D
   DC.B  $01
   DC.B  $43
   DC.B  $04, $04, $04, $03
   DC.W  S_INCREASE
   DC.B  $1F
   DC.B  $01
   DC.B  $45
   DC.B  $04, $04, $02, $04
   DC.W  S_DECREASE
   DC.B  $1E
   DC.B  $01
   DC.B  $52
   DC.B  $02, $03, $03, $05
   DC.W  S_DROP_GOLD
   DC.B  $38
   DC.B  $01
   DC.B  $69
   DC.B  $02, $03, $04, $04
   DC.W  S_KEEP_ALL_GOLD
   DC.B  $02

SAVE_LOAD_OPTIONS:
   DC.B  $01
   DC.B  $05
   DC.B  $02, $02, $00, $00
   DC.W  S_BEGIN_NEW_GAME
   DS.B  1
   DC.B  $01
   DC.B  $55
   DC.B  $01, $01, $04, $03
   DC.W  S_CONTINUE_GAME_NUMBER
   DC.B  $2E
   DC.B  $01
   DC.B  $6F
   DC.B  $01, $01, $02, $04
   DC.W  S_INCREASE
   DC.B  $21
   DC.B  $01
   DC.B  $71
   DC.B  $01, $01, $03, $02
   DC.W  S_DECREASE
   DC.B  $22

SAVE_GAME_SLOT_OPTIONS:
   DC.B  $01
   DC.B  $04
   DC.B  $02, $02, $02, $02
   DC.W  S_MOVE
   DC.B  $02
   DC.B  $01
   DC.B  $2C
   DC.B  $01, $01, $04, $03
   DC.W  S_SAVE_IN_SLOT
   DC.B  $2F
   DC.B  $01
   DC.B  $49
   DC.B  $01, $01, $02, $04
   DC.W  S_INCREASE
   DC.B  $24
   DC.B  $01
   DC.B  $4B
   DC.B  $01, $01, $03, $02
   DC.W  S_DECREASE
   DC.B  $25

SCENARIO_DISK_OPTIONS:
   DC.B  $01
   DS.B  1
   DC.B  $03, $02, $00, $00
   DC.W  S_USE_PREVIOUS_DISK
   DC.B  $32
   DC.B  $01
   DC.B  $28
   DC.B  $01, $03, $00, $00
   DC.W  S_FORMAT_NEW_DISK
   DC.B  $31
   DC.B  $01
   DC.B  $50
   DC.B  $02, $01, $00, $00
   DC.W  S_CANCEL_REQUEST
   DC.B  $30

CHAR_SELECT_OPTIONS:
   DC.B  $01
   DC.B  $05
   DC.B  $06, $02, $06, $02
   DC.W  S_MOVE
   DC.B  $02
   DC.B  $01
   DC.B  $2D
   DC.B  $01, $05, $01, $03
   DC.W  S_ADD_NAME
   DC.B  $07
   DC.B  $01
   DC.B  $49
   DC.B  $01, $05, $02, $04
   DC.W  S_INCREASE
   DC.B  $29
   DC.B  $01
   DC.B  $4B
   DC.B  $01, $05, $03, $05
   DC.W  S_DECREASE
   DC.B  $2A
   DC.B  $01
   DC.B  $55
   DC.B  $02, $06, $04, $06
   DC.W  S_TO_THIS_ROOM
   DC.B  $2B
   DS.B  1
   DC.B  $55
   DC.B  $05, $01, $05, $01
   DC.W  S_TO_ALI_BABAS_HOME
   DC.B  $2C

ALT_INPUT_OPTIONS:
   DC.B  $01
   DC.B  $05
   DC.B  $03, $02, $00, $00
   DC.W  S_PAY_FOR
   DC.B  $39
   DC.B  $01
   DC.B  $2D
   DC.B  $01, $03, $00, $00
   DC.W  S_BUY_SOMETHING_ELSE
   DC.B  $3A
   DC.B  $01
   DC.B  $55
   DC.B  $02, $01, $00, $00
   DC.W  S_NO_SALE
   DS.B  1

NO_INPUT:
   DC.B  $FF

DIR_KEYS:
   DC.B  $A0, $C9, $CD, $CA, $CB
   DC.B  $8D, $E9, $ED, $EA, $EB
   DC.B  $9B, $8B, $8A, $88, $95

ZONE_THRESH_PLUS:
   DC.B  $1A, $1D, $1D

ZONE_THRESH_MINUS:
   DC.B  $03, $03, $06

joy_zone_x:
   DC.B  $01

joy_zone_y:
   DC.B  $01

JOY_SETTLE_DELAY:
   DC.B  $C8

MIN_GAME_SPEED:
   DC.B  $01

MAX_GAME_SPEED:
   DC.B  $05

slot_number:
   DC.B  $01

GAME_DISK_SLOTS:
   DC.B  $01

MAX_SLOT_NUMBER:
   DC.B  $11

SAVE_TRACK_BASE:
   DC.B  $13, $01

SAVE_SECTOR_COUNT:
   DC.B  $1A

SECTORS_PER_TRACK:
   DC.B  $0D

DISK_SIGNATURE:
   DC.B  $C1, $CC, $C9

scenario_init_flag:
   DS.B  1

under_level:
   DS.B  1

safe_to_rest:
   DS.B  1
