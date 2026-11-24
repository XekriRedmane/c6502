NULL_PAGE_LOW	EQU	$00
TEXT_STREAM_PTR	EQU	$06
RWTS_IOB_PTR	EQU	$08
NULL_PAGE_HIGH	EQU	$0A
HARGS	EQU	$10
SSP	EQU	$15
FP	EQU	$17
DPTR	EQU	$19
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
tone_padding	EQU	$E3
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

play_high_tone:
   SUBROUTINE

   ; prologue: 3 arg bytes, 6 local bytes
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

   LDY   #$0B
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
.loop@0_start:
   LDA   CLICKS_PER_UNIT
   LDY   #$02
   STA   (FP),Y
.loop@1_start:
   LDA   SPKR
   LDY   #$03
   STA   (FP),Y
.loop@2_keep_start:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$04
   STA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BNE   .loop@2_keep_start
   LDY   #$09
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   INY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
   BNE   .loop@1_start
   LDY   #$0A
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$06
   STA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ORA   #$00
   BNE   .loop@0_start

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0B
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

play_low_tone:
   SUBROUTINE

   ; prologue: 3 arg bytes, 12 local bytes
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

   LDY   #$11
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
.loop@3_start:
   LDA   CLICKS_PER_UNIT
   LDY   #$02
   STA   (FP),Y
.loop@4_start:
   LDA   SPKR
   LDY   #$03
   STA   (FP),Y
.loop@5_keep_start:
   LDA   tone_padding
   LDY   #$04
   STA   (FP),Y
   LDA   tone_padding
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   tone_padding
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$06
   STA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BNE   .loop@5_keep_start
   LDA   tone_padding
   LDY   #$07
   STA   (FP),Y
   LDA   tone_padding
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   tone_padding
   INY
   STA   (FP),Y
   LDA   tone_padding
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   tone_padding
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   INY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$0B
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
   BNE   .loop@4_start
   LDY   #$10
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$0C
   STA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@0
   JMP   .loop@3_start
.lb_skip@0:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$11
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

tune_player:
   SUBROUTINE

   ; prologue: 2 arg bytes, 123 local bytes
   SEC
   LDA   SSP
   SBC   #$7D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7C
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
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   ADC   #$02
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   LDY   #$04
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   KBDSTRB
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   STA   ATTRACT_FLAG
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$0D
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
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@1
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   TUNE_1_COMBAT_HIT+14
   JMP   .if_end@0
.if_else@1:
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   TUNE_2_COMBAT_DAMAGE+12
.if_end@2:
.if_end@0:
   LDY   #$7E
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
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$20
   STA   (FP),Y
   TAX
   LDA   TUNE_TABLE,X
   INY
   STA   (FP),Y
   LDA   TUNE_TABLE+1,X
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   TUNE_PC
   INY
   LDA   (FP),Y
   STA   TUNE_PC+1
   LDA   #$00
   INY
   STA   (FP),Y
   STA   TUNE_LOOP_CTR
   LDA   #$01
   INY
   STA   (FP),Y
   STA   TUNE_PRNG_MASK
.loop@6_start:
   LDA   TUNE_PC
.vthread@0:
   STA   DPTR
   LDA   TUNE_PC+1
   STA   DPTR+1
   LDY   #$00
   LDA   (TUNE_PC),Y
   LDY   #$25
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
   SBC   #$27
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
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@13
   JMP   .if_else@4
.lb_skip@13:
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JSR   step_prng
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   TUNE_PRNG_MASK
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$30
   AND   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$31
   AND   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$32
   EOR   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$33
   EOR   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   TUNE_PC
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   TUNE_PC+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   TUNE_PC
   INY
   LDA   (FP),Y
   STA   TUNE_PC+1
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   SOUND_ON
   BNE   .lb_skip@12
   JMP   .if_end@5
.lb_skip@12:
   LDA   TUNE_ACTION_VEC+1
   CMP   PLAY_LOW_TONE_ADDR+1
   BNE   .thread@0
   LDA   TUNE_ACTION_VEC
   CMP   PLAY_LOW_TONE_ADDR
   BEQ   .cmp_true@9
.thread@0:
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@7
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$37
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   play_low_tone
   JMP   .if_end@6
.if_else@7:
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$37
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   play_high_tone
.if_end@6:
   LDY   #$37
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
.if_end@5:
   LDA   BORDER_ANIM_ON
   BNE   .lb_skip@11
   JMP   .if_end@3
.lb_skip@11:
   JSR   border_anim_step
   JMP   .if_end@3
.if_else@4:
   LDY   #$26
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$1F
   BEQ   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .case@8
.lb_skip@10:
   LDY   #$3E
   LDA   (FP),Y
   CMP   #$20
   BEQ   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .case@9
.lb_skip@9:
   LDY   #$3E
   LDA   (FP),Y
   CMP   #$21
   BEQ   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .case@10
.lb_skip@8:
   LDY   #$3E
   LDA   (FP),Y
   CMP   #$22
   BEQ   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .case@11
.lb_skip@7:
   LDY   #$3E
   LDA   (FP),Y
   CMP   #$23
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .case@12
.lb_skip@6:
   LDY   #$3E
   LDA   (FP),Y
   CMP   #$24
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .case@13
.lb_skip@5:
   LDY   #$3E
   LDA   (FP),Y
   CMP   #$25
   BEQ   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@14
.lb_skip@4:
   LDY   #$3E
   LDA   (FP),Y
   CMP   #$26
   BEQ   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .case@15
.lb_skip@3:
   JMP   .default@16
.case@8:
   LDA   TUNE_PC
   LDY   #$50
   STA   (FP),Y
   LDA   TUNE_PC+1
   INY
   STA   (FP),Y
   LDA   TUNE_PC
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   TUNE_PC+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   TUNE_PC
   INY
   LDA   (FP),Y
   STA   TUNE_PC+1
   STA   TUNE_LOOP_ADDR+1
   LDA   TUNE_PC
   STA   TUNE_LOOP_ADDR
   JMP   .vthread@0
.case@9:
   LDA   TUNE_PC
   LDY   #$54
   STA   (FP),Y
   LDA   TUNE_PC+1
   INY
   STA   (FP),Y
   LDA   TUNE_PC
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   TUNE_PC+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   TUNE_PC
   INY
   LDA   (FP),Y
   STA   TUNE_PC+1
   STA   TUNE_GOSUB_ADDR+1
   LDA   TUNE_PC
   STA   TUNE_GOSUB_ADDR
   JMP   .vthread@0
.case@10:
   LDA   TUNE_GOSUB_ADDR
   STA   TUNE_PC
   LDA   TUNE_GOSUB_ADDR+1
   STA   TUNE_PC+1
   JMP   .loop@6_start
.case@11:
   LDA   PLAY_LOW_TONE_ADDR
   STA   TUNE_ACTION_VEC
   LDA   PLAY_LOW_TONE_ADDR+1
   STA   TUNE_ACTION_VEC+1
   JMP   .switch@7_break
.case@12:
   LDA   PLAY_HIGH_TONE_ADDR
   STA   TUNE_ACTION_VEC
   LDA   PLAY_HIGH_TONE_ADDR+1
   STA   TUNE_ACTION_VEC+1
   JMP   .switch@7_break
.case@13:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$7F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@14:
   LDA   #$0F
   LDY   #$58
   STA   (FP),Y
   STA   TUNE_PRNG_MASK
   JMP   .switch@7_break
.case@15:
   LDA   #$00
   LDY   #$59
   STA   (FP),Y
   STA   TUNE_PRNG_MASK
   JMP   .switch@7_break
.default@16:
   LDY   #$26
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   TUNE_LOOP_CTR
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$5A
   SBC   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$5B
   SBC   (FP),Y
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDA   TUNE_LOOP_CTR
   INY
   STA   (FP),Y
   LDA   TUNE_LOOP_CTR
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   TUNE_LOOP_CTR
   LDA   TUNE_LOOP_ADDR
   STA   TUNE_PC
   LDA   TUNE_LOOP_ADDR+1
   STA   TUNE_PC+1
   JMP   .loop@6_start
.if_end@9:
   LDA   #$00
   LDY   #$62
   STA   (FP),Y
   STA   TUNE_LOOP_CTR
.switch@7_break:
.if_end@3:
   LDA   TUNE_PC
   LDY   #$63
   STA   (FP),Y
   LDA   TUNE_PC+1
   INY
   STA   (FP),Y
   LDA   TUNE_PC
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   TUNE_PC+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   TUNE_PC
   INY
   LDA   (FP),Y
   STA   TUNE_PC+1
   LDA   ATTRACT_FLAG
   BNE   .lb_skip@2
   JMP   .if_end@10
.lb_skip@2:
   LDA   KBD
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11

   ; epilogue
   CLC
   LDA   FP
   ADC   #$7F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@11:
   LDA   INPUT_MODE
   BNE   .lb_skip@1
   JMP   .if_end@12
.lb_skip@1:
   LDA   BUTN
   LDY   #$6C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13

   ; epilogue
   CLC
   LDA   FP
   ADC   #$7F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@13:
   LDA   #$01
   LDY   #$71
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   BUTN+1
   LDY   #$72
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@14

   ; epilogue
   CLC
   LDA   FP
   ADC   #$7F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@14:
.if_end@12:
.if_end@10:
   LDA   WAIT_LOOP_COUNT
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$79
   STA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   STA   WAIT_LOOP_COUNT
   JMP   .loop@6_start

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

SOUND_ON:
   DC.B  $01

PLAY_HIGH_TONE_ADDR:
   DC.W  $7A7F

PLAY_LOW_TONE_ADDR:
   DC.W  $7A91

CLICKS_PER_UNIT:
   DC.B  $0A

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   tone_padding
   RTS
