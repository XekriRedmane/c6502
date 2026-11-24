NULL_PAGE_LOW	EQU	$00
TEXT_STREAM_PTR	EQU	$06
RWTS_IOB_PTR	EQU	$08
NULL_PAGE_HIGH	EQU	$0A
HARGS	EQU	$10
SSP	EQU	$11
FP	EQU	$13
DPTR	EQU	$15
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
MENU_DESC_PTR	EQU	$C0
MENU_OPTION_PTR	EQU	$C2
TUNE_PC	EQU	$E4
MOB_PTR	EQU	$F4
ENTITY_PTR	EQU	$F8
CHAR_PTR	EQU	$FA
SOFTEV	EQU	$03F2
PWREDUP	EQU	$03F4
HIRES_PAGE_1	EQU	$2000
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

draw_border_cell:
   SUBROUTINE

   ; prologue: 2 arg bytes, 9 local bytes
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

   LDY   #$0D
   LDA   (FP),Y
   STA   FONT_CHARNUM
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   TAX
   LDA   border_row,X
   INY
   STA   (FP),Y
   STA   TEXT_ROW
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   TAX
   LDA   border_col,X
   INY
   STA   (FP),Y
   STA   TEXT_COL
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
   JSR   render_font_char_direct

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0D
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

move_border_icons:
   SUBROUTINE

   ; prologue: 2 arg bytes, 36 local bytes
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
.loop@0_start:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
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
   LDY   #$03
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
   LDY   #$02
   LDA   (FP),Y
   STA   border_glyph,X
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
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   draw_border_cell
   LDA   #$00
   LDY   #$0B
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .if_end@0
.if_else@1:
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
   TAX
   LDA   border_glyph,X
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
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDY   #$03
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   TAX
   LDA   border_glyph,X
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   LDY   #$18
   STA   (FP),Y
   LDY   #$03
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
   TAX
   LDY   #$18
   LDA   (FP),Y
   STA   border_glyph,X
   LDA   #$00
   LDY   #$1C
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   draw_border_cell
.if_end@2:
.if_end@0:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$21
   CMP   (FP),Y
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$23
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
   ADC   #$28
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

move_border_icon_list:
   SUBROUTINE

   ; prologue: 1 arg bytes, 5 local bytes
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

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
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
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$08
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   move_border_icons

   ; epilogue
   CLC
   LDA   FP
   ADC   #$08
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

start_border_passes:
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

   LDA   border_anim_index
   LDY   #$01
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   STA   border_pass
   LDA   BORDER_TICK_CELL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   TAX
   LDY   #$01
   LDA   (FP),Y
   STA   border_glyph,X
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   BORDER_TICK_CELL
   STA   (SSP),Y
   LDA   (FP),Y
   INY
   STA   (SSP),Y
   JSR   draw_border_cell
   LDA   border_anim_index
   CLC
   ADC   #$01
   LDY   #$06
   STA   (FP),Y
   STA   border_anim_index
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$16
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
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
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDA   #$05
   INY
   STA   (FP),Y
   STA   border_anim_index
.if_end@3:

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

anim_border_icons:
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

   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   random_in_range
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
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
   BEQ   .if_end@4

   ; epilogue
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
   RTS
.if_end@4:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$07
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
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDY   #$21
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
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
   LDY   #$21
   STA   (FP),Y
.if_end@5:
   LDY   #$20
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
   LDA   border_glyph,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$21
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
   LDY   #$14
   LDA   (FP),Y
   STA   border_glyph,X
   LDA   #$00
   LDY   #$18
   STA   (FP),Y
   LDY   #$20
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
   TAX
   LDY   #$18
   LDA   (FP),Y
   STA   border_glyph,X
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$21
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   draw_border_cell
   LDA   #$00
   LDY   #$1C
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$20
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   draw_border_cell

   ; epilogue
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
   RTS

border_anim_step:
   SUBROUTINE

   ; prologue: 0 arg bytes, 37 local bytes
   SEC
   LDA   SSP
   SBC   #$27
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$26
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   border_next
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@2
   LDY   #$01
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .case@3
.lb_skip@2:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .case@4
.lb_skip@1:
   JMP   .default@5
.case@2:
   LDA   #$1C
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
   JSR   move_border_icon_list
   LDA   #$25
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
   JSR   move_border_icon_list
   LDA   #$01
   LDY   #$0B
   STA   (FP),Y
   STA   border_next

   ; epilogue
   CLC
   LDA   FP
   ADC   #$27
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$26
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@3:
   LDA   #$00
   LDY   #$0C
   STA   (FP),Y
   LDA   #$09
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   move_border_icons
   LDA   #$00
   LDY   #$0E
   STA   (FP),Y
   STA   border_next

   ; epilogue
   CLC
   LDA   FP
   ADC   #$27
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$26
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@4:
   LDA   #$0A
   LDY   #$0F
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
   JSR   move_border_icon_list
   LDA   #$13
   LDY   #$10
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
   JSR   move_border_icon_list
   LDA   border_pass
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@7
   JSR   start_border_passes
   JMP   .switch@1_break
.if_else@7:
   LDA   border_pass
   LDY   #$15
   STA   (FP),Y
   LDA   border_pass
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   border_pass
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
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@9
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$12
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
.xjump@0:
   LDY   #$03
   STA   (SSP),Y
   JSR   anim_border_icons
   JMP   .switch@1_break
.if_else@9:
   LDA   border_pass
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .switch@1_break
   LDA   #$02
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$24
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$22
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   JMP   .xjump@0
.default@5:
   JSR   start_border_passes
.switch@1_break:
   LDA   #$02
   LDY   #$25
   STA   (FP),Y
   STA   border_next

   ; epilogue
   CLC
   LDA   FP
   ADC   #$27
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

plot_title_glyphs:
   SUBROUTINE

   ; prologue: 3 arg bytes, 21 local bytes
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

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
.loop@6_start:
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$04
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
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
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
   BNE   .lb_skip@3
   JMP   .loop@6_break
.lb_skip@3:
   LDY   #$01
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
   LDA   TITLE_GLYPH,X
   INY
   STA   (FP),Y
   STA   FONT_CHARNUM
   LDY   #$01
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
   TAX
   LDA   TITLE_GLYPH_COL,X
   INY
   STA   (FP),Y
   STA   TEXT_COL
   LDY   #$01
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
   LDA   TITLE_GLYPH_ROW,X
   INY
   STA   (FP),Y
   STA   TEXT_ROW
   LDY   #$1A
   LDA   (FP),Y
   STA   FONT_CHARSET
   JSR   print_fontchar_at_text_pos
   LDY   #$01
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   JMP   .loop@6_start
.loop@6_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$16
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

draw_border_strings:
   SUBROUTINE

   ; prologue: 4 arg bytes, 17 local bytes
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

   LDY   #$17
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
.loop@7_start:
   LDY   #$01
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   TAX
   LDA   BORDER_LABEL_COL,X
   INY
   STA   (FP),Y
   STA   TEXT_COL
   LDY   #$01
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   TAX
   LDA   BORDER_LABEL_ROW,X
   INY
   STA   (FP),Y
   STA   TEXT_ROW
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   print_string
   LDY   #$01
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0A
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
   LDY   #$0E
   CMP   (FP),Y
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .loop@7_start
.lb_skip@4:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$17
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
   RTS

draw_border_labels:
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

   LDA   #$81
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
   JSR   hrcg_cout
   LDA   #$B2
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
   JSR   hrcg_cout
   LDA   #<BORDER_STR_HORIZ
   LDY   #$03
   STA   (FP),Y
   LDA   #>BORDER_STR_HORIZ
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$05
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   draw_border_strings
   LDA   #<BORDER_STR_VERT_L
   LDY   #$07
   STA   (FP),Y
   LDA   #>BORDER_STR_VERT_L
   INY
   STA   (FP),Y
   LDA   #$06
   INY
   STA   (FP),Y
   LDA   #$0F
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   draw_border_strings
   LDA   #<BORDER_STR_VERT_R
   LDY   #$0B
   STA   (FP),Y
   LDA   #>BORDER_STR_VERT_R
   INY
   STA   (FP),Y
   LDA   #$10
   INY
   STA   (FP),Y
   LDA   #$19
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   draw_border_strings

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

print_at:
   SUBROUTINE

   ; prologue: 4 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$06
   LDA   (FP),Y
   STA   TEXT_ROW
   DEY
   LDA   (FP),Y
   STA   TEXT_COL
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   print_string

   ; epilogue
   CLC
   LDA   FP
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

init_game_display:
   SUBROUTINE

   ; prologue: 0 arg bytes, 39 local bytes
   SEC
   LDA   SSP
   SBC   #$29
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$28
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$90
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
   JSR   hrcg_cout
   LDA   #<UI_TEXT_STREAM
   LDY   #$02
   STA   (FP),Y
   LDA   #>UI_TEXT_STREAM
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   TEXT_STREAM_PTR
   INY
   LDA   (FP),Y
   STA   TEXT_STREAM_PTR+1
   JSR   text_renderer
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$04
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   plot_title_glyphs
   JSR   draw_border_labels
   LDA   #$03
   LDY   #$07
   STA   (FP),Y
   LDA   #$09
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   plot_title_glyphs
   JSR   print_ctrl_ab
   LDA   #<S_COPYRIGHT
   LDY   #$0A
   STA   (FP),Y
   LDA   #>S_COPYRIGHT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$15
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   print_at
   LDA   #<S_PRESS_SPACE_BOOT
   LDY   #$0E
   STA   (FP),Y
   LDA   #>S_PRESS_SPACE_BOOT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$17
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   print_at
   LDA   #$81
   LDY   #$12
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
   LDA   #$B5
   LDY   #$13
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
   LDA   #<S_DECOR_ROW1
   LDY   #$14
   STA   (FP),Y
   LDA   #>S_DECOR_ROW1
   INY
   STA   (FP),Y
   LDA   #$1E
   INY
   STA   (FP),Y
   LDA   #$15
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   print_at
   LDA   #<S_DECOR_ROW2
   LDY   #$18
   STA   (FP),Y
   LDA   #>S_DECOR_ROW2
   INY
   STA   (FP),Y
   LDA   #$1E
   INY
   STA   (FP),Y
   LDA   TEXT_ROW
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$18
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   print_at
   LDA   #<S_DECOR_ROW3
   LDY   #$20
   STA   (FP),Y
   LDA   #>S_DECOR_ROW3
   INY
   STA   (FP),Y
   LDA   #$1E
   INY
   STA   (FP),Y
   LDA   TEXT_ROW
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$20
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   print_at

   ; epilogue
   CLC
   LDA   FP
   ADC   #$29
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$28
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

init_game_state:
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

   JSR   init_game_display
   LDA   #$03
   LDY   #$01
   STA   (FP),Y
   STA   border_next
   LDA   #$00
   INY
   STA   (FP),Y
   STA   INPUT_MODE
   LDA   #$B1
   INY
   STA   (FP),Y
   LDA   #<s_PRINT_FONT_CHAR
   INY
   STA   (FP),Y
   LDA   #>s_PRINT_FONT_CHAR
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$07
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
   LDA   #$03
   LDY   #$08
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   tune_player
   LDA   #$00
   STA   BORDER_ANIM_ON

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

select_input_mode:
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

   LDA   #$38
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
   LDA   BUTN
   LDY   #$02
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
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@8_start:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   STA   KBDSTRB
.loop@9_continue:
   LDA   KBD
   LDY   #$0A
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
   SBC   #$80
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
   ASL
   LDA   #$00
   ROL
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .loop@9_break
.lb_skip@7:
   LDA   BUTN
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$14
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .loop@9_continue
.lb_skip@6:
   LDA   #$01
   LDY   #$19
   STA   (FP),Y
   STA   INPUT_MODE
   JSR   set_cursor_row21
   LDA   #$2F
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
   JSR   display_message

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
.loop@9_break:
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$A0
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .loop@8_start
.lb_skip@5:
   LDA   #$00
   LDY   #$1F
   STA   (FP),Y
   STA   INPUT_MODE

   ; epilogue
   CLC
   LDA   FP
   ADC   #$21
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

init_mob_count:
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   STA   TOTAL_MOB_COUNT
   LDA   #<LOCATIONS
   INY
   STA   (FP),Y
   LDA   #>LOCATIONS
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@30:
   BCC   .mul_skip@31
   CLC
   ADC   #$09
.mul_skip@31:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@30
   LDY   #$05
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   CLC
   LDY   #$04
   ADC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$05
   ADC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   CHAR_PTR
   INY
   LDA   (FP),Y
   STA   CHAR_PTR+1
.loop@10_continue:
   LDA   CHAR_PTR
   STA   DPTR
   LDA   CHAR_PTR+1
   STA   DPTR+1
   LDY   #$00
   LDA   (CHAR_PTR),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@9
   JMP   .loop@10_break
.lb_skip@9:
   LDA   CHAR_PTR
   CLC
   ADC   #$08
   LDY   #$0E
   STA   (FP),Y
   LDA   CHAR_PTR+1
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
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   TOTAL_MOB_COUNT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$12
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   STA   TOTAL_MOB_COUNT
   LDA   CHAR_PTR
   INY
   STA   (FP),Y
   LDA   CHAR_PTR+1
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@34:
   BCC   .mul_skip@35
   CLC
   ADC   #$09
.mul_skip@35:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@34
   LDY   #$1C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDA   CHAR_PTR
   CLC
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   CHAR_PTR+1
   DEY
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   CHAR_PTR
   INY
   LDA   (FP),Y
   STA   CHAR_PTR+1
   LDA   TOTAL_MOB_COUNT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   SEC
   LDY   #$21
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   SBC   (FP),Y
   BVC   .cmp_novf@36
   EOR   #$80
.cmp_novf@36:
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
   JMP   .loop@10_continue
.lb_skip@8:
   LDA   CHAR_PTR
   CLC
   ADC   #$01
   LDY   #$25
   STA   (FP),Y
   LDA   CHAR_PTR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   CHAR_PTR
   INY
   LDA   (FP),Y
   STA   CHAR_PTR+1
   JMP   .loop@10_continue
.loop@10_break:

   ; epilogue
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
   RTS

init_disk_iob:
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

   LDA   #<RWTS_IOB
   LDY   #$01
   STA   (FP),Y
   LDA   #>RWTS_IOB
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   RWTS_IOB_PTR
   INY
   LDA   (FP),Y
   STA   RWTS_IOB_PTR+1

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

clear_hires_page1:
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@11_start:
   LDA   #$00
   CLC
   LDY   #$01
   SBC   (FP),Y
   LDA   #$20
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
   BNE   .lb_skip@10
   JMP   .loop@11_break
.lb_skip@10:
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   LDA   #<HIRES_PAGE_1
   INY
   STA   (FP),Y
   LDA   #>HIRES_PAGE_1
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   CLC
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   INY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0E
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@11_start
.loop@11_break:

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

game_init:
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

   LDA   #$6F
   STA   SOFTEV
   LDA   #$A5
   STA   SOFTEV+1
   JSR   SETPWRC
   LDA   MAP_WIDTH
   STA   BLINK_COL
   JSR   clear_hires_page1
   LDA   TXTCLR
   LDY   #$01
   STA   (FP),Y
   LDA   MIXCLR
   INY
   STA   (FP),Y
   LDA   TXTPAGE1
   INY
   STA   (FP),Y
   LDA   HIRES
   INY
   STA   (FP),Y
   JSR   init_disk_iob
   JSR   hrcg_init
   JSR   set_text_window_wrap
   JSR   init_game_state
   JSR   fill_map
   JSR   set_text_window_bottom
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   STA   LOCATION_POS
   INY
   STA   (FP),Y
   STA   LOCATION_STYLE
   LDA   ENTITY_PTR
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   ENTITY_PTR
   INY
   LDA   (FP),Y
   STA   ENTITY_PTR+1
   JSR   select_input_mode
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$20
   LDY   #$01
   STA   (SSP),Y
   JSR   run_chain
   JSR   init_mob_count
   LDA   #$01
   LDY   #$09
   STA   (FP),Y
   STA   CURRENT_PLAYER

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

BORDER_ANIM_ON:
   DC.B  $01

BORDER_CELLS:
   DC.B  $2E

BORDER_TICK_CELL:
   DC.B  $09

border_glyph:
   DC.B  $00, $00, $00, $00, $00, $0B, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $0F, $00, $00, $00, $00, $00, $00, $00, $05, $00, $00, $00, $00, $00, $00, $00
   DC.B  $14, $00, $00, $00, $00, $00, $00, $00, $00, $0E, $00, $00, $00, $00

border_row:
   DC.B  $09, $0A, $0B, $0C, $0D, $0E, $0F, $10, $11, $12, $10, $10, $10, $10, $10, $10
   DC.B  $10, $10, $10, $10, $10, $10, $10, $10, $10, $10, $10, $10, $0B, $0B, $0B, $0B
   DC.B  $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B

border_col:
   DC.B  $13, $13, $13, $13, $13, $13, $13, $13, $13, $13, $06, $07, $08, $09, $0A, $0B
   DC.B  $0C, $0D, $0E, $20, $1F, $1E, $1D, $1C, $1B, $1A, $19, $18, $06, $07, $08, $09
   DC.B  $0A, $0B, $0C, $0D, $0E, $20, $1F, $1E, $1D, $1C, $1B, $1A, $19, $18

border_anim_index:
   DC.B  $06

border_next:
   DS.B  1

border_pass:
   DS.B  1

UI_TEXT_STREAM:
   DC.B  $7D, $80, $02, $2F, $00, $00, $01, $0C, $09, $00, $00, $02, $01, $02, $01, $00
   DC.B  $00, $2F, $83, $01, $14, $08, $05, $00, $06, $0F, $12, $14, $19, $00, $14, $08
   DC.B  $09, $05, $16, $05, $13, $7F

BORDER_LABEL_COL:
   DC.B  $01, $01, $01, $16, $16, $16, $01, $01, $01, $01, $01, $01, $16, $16, $16, $16
   DC.B  $10, $10, $10, $10, $25, $25, $25, $25, $25, $25

BORDER_LABEL_ROW:
   DC.B  $09, $0E, $13, $09, $0E, $13, $0A, $0B, $0C, $0D, $0F, $12, $0A, $0D, $0F, $12
   DC.B  $0A, $0D, $0F, $12, $0A, $0D, $0F, $10, $11, $12

BORDER_STR_HORIZ:
   DC.B  $11, $F0, $F1, $F0, $F1, $F0, $F1, $F0, $F1, $F0, $F1, $F0, $F1, $F0, $F1, $F0
   DC.B  $F1, $F0

BORDER_STR_VERT_L:
   DC.B  $02, $F0, $F1

BORDER_STR_VERT_R:
   DC.B  $02, $F1, $F0

S_COPYRIGHT:
   DC.B  $1B, $C3, $CF, $D0, $D9, $D2, $C9, $C7, $C8, $D4, $A0, $B1, $B9, $B8, $B2, $A0
   DC.B  $D3, $D4, $D5, $C1, $D2, $D4, $A0, $D3, $CD, $C9, $D4, $C8

S_PRESS_SPACE_BOOT:
   DC.B  $1B, $D0, $D2, $C5, $D3, $D3, $A0, $D3, $D0, $C1, $C3, $C5, $A0, $C2, $C1, $D2
   DC.B  $A0, $D4, $CF, $A0, $C3, $CF, $CE, $D4, $C9, $CE, $D5, $C5

S_DECOR_ROW1:
   DC.B  $07, $E8, $E9, $EA, $EB, $EC, $ED, $EE

S_DECOR_ROW2:
   DC.B  $08, $EF, $F0, $F1, $F2, $F3, $F4, $F5, $F6

S_DECOR_ROW3:
   DC.B  $08, $F7, $F8, $F9, $FA, $FB, $FC, $FD, $FE

TITLE_GLYPH:
   DC.B  $01, $0E, $04, $02, $01, $03, $04, $1C, $1C

TITLE_GLYPH_COL:
   DC.B  $10, $12, $14, $04, $04, $22, $22, $10, $16

TITLE_GLYPH_ROW:
   DC.B  $03, $03, $03, $0B, $10, $10, $0B, $10, $0B
