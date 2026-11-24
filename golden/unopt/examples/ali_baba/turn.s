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

run_chain:
   SUBROUTINE

   ; prologue: 1 arg bytes, 177 local bytes
   SEC
   LDA   SSP
   SBC   #$B3
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B2
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.loop@0_continue:
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$03
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@58
   JMP   .loop@0_break
.lb_skip@58:
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@57
   JMP   .case@2
.lb_skip@57:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@56
   JMP   .case@3
.lb_skip@56:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@55
   JMP   .case@4
.lb_skip@55:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@54
   JMP   .case@5
.lb_skip@54:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@53
   JMP   .case@6
.lb_skip@53:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@52
   JMP   .case@7
.lb_skip@52:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@51
   JMP   .case@8
.lb_skip@51:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$08
   BEQ   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@50
   JMP   .case@9
.lb_skip@50:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$09
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
   BEQ   .lb_skip@49
   JMP   .case@10
.lb_skip@49:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$0A
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@48
   JMP   .case@11
.lb_skip@48:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$0B
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@47
   JMP   .case@12
.lb_skip@47:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$0C
   BEQ   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@46
   JMP   .case@13
.lb_skip@46:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$0D
   BEQ   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@45
   JMP   .case@14
.lb_skip@45:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$0E
   BEQ   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@44
   JMP   .case@15
.lb_skip@44:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$0F
   BEQ   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@43
   JMP   .case@16
.lb_skip@43:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$10
   BEQ   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@42
   JMP   .case@17
.lb_skip@42:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$11
   BEQ   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@41
   JMP   .case@18
.lb_skip@41:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$12
   BEQ   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@40
   JMP   .case@19
.lb_skip@40:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$13
   BEQ   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@39
   JMP   .case@20
.lb_skip@39:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$14
   BEQ   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@38
   JMP   .case@21
.lb_skip@38:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$15
   BEQ   .cmp_true@42
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@37
   JMP   .case@22
.lb_skip@37:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$16
   BEQ   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@36
   JMP   .case@23
.lb_skip@36:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$17
   BEQ   .cmp_true@46
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@35
   JMP   .case@24
.lb_skip@35:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$18
   BEQ   .cmp_true@48
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@34
   JMP   .case@25
.lb_skip@34:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$19
   BEQ   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@33
   JMP   .case@26
.lb_skip@33:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$1A
   BEQ   .cmp_true@52
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@32
   JMP   .case@27
.lb_skip@32:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$1B
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
   BEQ   .lb_skip@31
   JMP   .case@28
.lb_skip@31:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$1C
   BEQ   .cmp_true@56
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@30
   JMP   .case@29
.lb_skip@30:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$1D
   BEQ   .cmp_true@58
   LDA   #$00
   JMP   .cmp_end@59
.cmp_true@58:
   LDA   #$01
.cmp_end@59:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@29
   JMP   .case@30
.lb_skip@29:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$1E
   BEQ   .cmp_true@60
   LDA   #$00
   JMP   .cmp_end@61
.cmp_true@60:
   LDA   #$01
.cmp_end@61:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@28
   JMP   .case@31
.lb_skip@28:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$1F
   BEQ   .cmp_true@62
   LDA   #$00
   JMP   .cmp_end@63
.cmp_true@62:
   LDA   #$01
.cmp_end@63:
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@27
   JMP   .case@32
.lb_skip@27:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$20
   BEQ   .cmp_true@64
   LDA   #$00
   JMP   .cmp_end@65
.cmp_true@64:
   LDA   #$01
.cmp_end@65:
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@26
   JMP   .case@33
.lb_skip@26:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$21
   BEQ   .cmp_true@66
   LDA   #$00
   JMP   .cmp_end@67
.cmp_true@66:
   LDA   #$01
.cmp_end@67:
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@25
   JMP   .case@34
.lb_skip@25:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$22
   BEQ   .cmp_true@68
   LDA   #$00
   JMP   .cmp_end@69
.cmp_true@68:
   LDA   #$01
.cmp_end@69:
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@24
   JMP   .case@35
.lb_skip@24:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$23
   BEQ   .cmp_true@70
   LDA   #$00
   JMP   .cmp_end@71
.cmp_true@70:
   LDA   #$01
.cmp_end@71:
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@23
   JMP   .case@36
.lb_skip@23:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$24
   BEQ   .cmp_true@72
   LDA   #$00
   JMP   .cmp_end@73
.cmp_true@72:
   LDA   #$01
.cmp_end@73:
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@22
   JMP   .case@37
.lb_skip@22:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$25
   BEQ   .cmp_true@74
   LDA   #$00
   JMP   .cmp_end@75
.cmp_true@74:
   LDA   #$01
.cmp_end@75:
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@21
   JMP   .case@38
.lb_skip@21:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$26
   BEQ   .cmp_true@76
   LDA   #$00
   JMP   .cmp_end@77
.cmp_true@76:
   LDA   #$01
.cmp_end@77:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@20
   JMP   .case@39
.lb_skip@20:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$27
   BEQ   .cmp_true@78
   LDA   #$00
   JMP   .cmp_end@79
.cmp_true@78:
   LDA   #$01
.cmp_end@79:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@19
   JMP   .case@40
.lb_skip@19:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$28
   BEQ   .cmp_true@80
   LDA   #$00
   JMP   .cmp_end@81
.cmp_true@80:
   LDA   #$01
.cmp_end@81:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@18
   JMP   .case@41
.lb_skip@18:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$29
   BEQ   .cmp_true@82
   LDA   #$00
   JMP   .cmp_end@83
.cmp_true@82:
   LDA   #$01
.cmp_end@83:
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@17
   JMP   .case@42
.lb_skip@17:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$2A
   BEQ   .cmp_true@84
   LDA   #$00
   JMP   .cmp_end@85
.cmp_true@84:
   LDA   #$01
.cmp_end@85:
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@16
   JMP   .case@43
.lb_skip@16:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$2B
   BEQ   .cmp_true@86
   LDA   #$00
   JMP   .cmp_end@87
.cmp_true@86:
   LDA   #$01
.cmp_end@87:
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@15
   JMP   .case@44
.lb_skip@15:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$2C
   BEQ   .cmp_true@88
   LDA   #$00
   JMP   .cmp_end@89
.cmp_true@88:
   LDA   #$01
.cmp_end@89:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@14
   JMP   .case@45
.lb_skip@14:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$2D
   BEQ   .cmp_true@90
   LDA   #$00
   JMP   .cmp_end@91
.cmp_true@90:
   LDA   #$01
.cmp_end@91:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@13
   JMP   .case@46
.lb_skip@13:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$2E
   BEQ   .cmp_true@92
   LDA   #$00
   JMP   .cmp_end@93
.cmp_true@92:
   LDA   #$01
.cmp_end@93:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@12
   JMP   .case@47
.lb_skip@12:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$2F
   BEQ   .cmp_true@94
   LDA   #$00
   JMP   .cmp_end@95
.cmp_true@94:
   LDA   #$01
.cmp_end@95:
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .case@48
.lb_skip@11:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$30
   BEQ   .cmp_true@96
   LDA   #$00
   JMP   .cmp_end@97
.cmp_true@96:
   LDA   #$01
.cmp_end@97:
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .case@49
.lb_skip@10:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$31
   BEQ   .cmp_true@98
   LDA   #$00
   JMP   .cmp_end@99
.cmp_true@98:
   LDA   #$01
.cmp_end@99:
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .case@50
.lb_skip@9:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$32
   BEQ   .cmp_true@100
   LDA   #$00
   JMP   .cmp_end@101
.cmp_true@100:
   LDA   #$01
.cmp_end@101:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .case@51
.lb_skip@8:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$33
   BEQ   .cmp_true@102
   LDA   #$00
   JMP   .cmp_end@103
.cmp_true@102:
   LDA   #$01
.cmp_end@103:
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .case@52
.lb_skip@7:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$34
   BEQ   .cmp_true@104
   LDA   #$00
   JMP   .cmp_end@105
.cmp_true@104:
   LDA   #$01
.cmp_end@105:
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .case@53
.lb_skip@6:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$35
   BEQ   .cmp_true@106
   LDA   #$00
   JMP   .cmp_end@107
.cmp_true@106:
   LDA   #$01
.cmp_end@107:
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .case@54
.lb_skip@5:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$36
   BEQ   .cmp_true@108
   LDA   #$00
   JMP   .cmp_end@109
.cmp_true@108:
   LDA   #$01
.cmp_end@109:
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@55
.lb_skip@4:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$37
   BEQ   .cmp_true@110
   LDA   #$00
   JMP   .cmp_end@111
.cmp_true@110:
   LDA   #$01
.cmp_end@111:
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .case@56
.lb_skip@3:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$38
   BEQ   .cmp_true@112
   LDA   #$00
   JMP   .cmp_end@113
.cmp_true@112:
   LDA   #$01
.cmp_end@113:
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .case@57
.lb_skip@2:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$39
   BEQ   .cmp_true@114
   LDA   #$00
   JMP   .cmp_end@115
.cmp_true@114:
   LDA   #$01
.cmp_end@115:
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .case@58
.lb_skip@1:
   LDY   #$07
   LDA   (FP),Y
   CMP   #$3A
   BEQ   .cmp_true@116
   LDA   #$00
   JMP   .cmp_end@117
.cmp_true@116:
   LDA   #$01
.cmp_end@117:
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .case@59
.lb_skip@0:
   JMP   .default@60
.case@2:
   JSR   game_turn_loop
   LDY   #$7D
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@3:
   JSR   move_loop
   LDY   #$7E
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@4:
   JSR   end_turn
   LDY   #$7F
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@5:
   JSR   turn_dispatch
   LDY   #$80
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@6:
   JSR   set_state_fight

   ; epilogue
   CLC
   LDA   FP
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B2
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@7:
   JSR   toggle_sound
   LDY   #$81
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@8:
   JSR   init_char_select
   LDY   #$82
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@9:
   JSR   auto_heal

   ; epilogue
   CLC
   LDA   FP
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B2
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@10:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   MENU_COMBAT_POS
   LDY   #$01
   STA   (SSP),Y
   JSR   enter_combat_state

   ; epilogue
   CLC
   LDA   FP
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B2
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@11:
   JSR   attack_right
   LDY   #$83
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@12:
   JSR   attack_left
   LDY   #$84
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@13:
   JSR   attack_up
   LDY   #$85
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@14:
   JSR   attack_down
   LDY   #$86
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@15:
   JSR   find_attack_target
   LDY   #$87
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@16:
   JSR   select_someone_else
   LDY   #$88
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@17:
   JSR   resolve_attack

   ; epilogue
   CLC
   LDA   FP
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B2
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@18:
   JSR   handle_mob_encounter
   LDY   #$89
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@19:
   JSR   teardown_input
   LDY   #$8A
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@20:
   JSR   menu_see_your_lot
   LDY   #$8B
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@21:
   JSR   menu_check_attack
   LDY   #$8C
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@22:
   JSR   menu_other_options
   LDY   #$8D
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@23:
   JSR   menu_game_settings
   LDY   #$8E
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@24:
   JSR   menu_game_settings_enter
   LDY   #$8F
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@25:
   JSR   settings_speed_up
   LDY   #$90
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@26:
   JSR   settings_speed_down
   LDY   #$91
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@27:
   JSR   settings_danger_up
   LDY   #$92
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@28:
   JSR   settings_danger_down
   LDY   #$93
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@29:
   JSR   menu_attack_target
   LDY   #$94
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@30:
   JSR   menu_drop_gold
   LDY   #$95
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@31:
   JSR   menu_drop_inc
   LDY   #$96
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@32:
   JSR   menu_drop_dec
   LDY   #$97
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@33:
   JSR   init_world
   LDY   #$98
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@34:
   JSR   slot_inc_enter
   LDY   #$99
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@35:
   JSR   slot_dec_enter
   LDY   #$9A
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@36:
   JSR   enter_save_game_slot
   LDY   #$9B
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@37:
   JSR   slot_inc_reenter
   LDY   #$9C
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@38:
   JSR   slot_dec_reenter
   LDY   #$9D
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@39:
   JSR   enter_scenario_disk
   LDY   #$9E
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@40:
   JSR   enter_char_select
   LDY   #$9F
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@41:
   JSR   char_select_init
   LDY   #$A0
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@42:
   JSR   char_select_next
   LDY   #$A1
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@43:
   JSR   char_select_prev
   LDY   #$A2
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@44:
   JSR   char_select_here
   LDY   #$A3
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@45:
   JSR   char_select_home
   LDY   #$A4
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@46:
   JSR   start_game_input
   LDY   #$A5
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@47:
   JSR   continue_game
   LDY   #$A6
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@48:
   JSR   save_game
   LDY   #$A7
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@49:
   JSR   cancel_scenario
   LDY   #$A8
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@50:
   JSR   format_scenario_disk
   LDY   #$A9
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@51:
   JSR   use_scenario_disk
   LDY   #$AA
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@52:
   JSR   retire_character

   ; epilogue
   CLC
   LDA   FP
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B2
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.case@53:
   JSR   flush_pending_xp
   LDY   #$AB
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@54:
   JSR   init_drop_gold
   LDY   #$AC
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@55:
   JSR   adj_drop_inc
   LDY   #$AD
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@56:
   JSR   adj_drop_dec
   LDY   #$AE
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@57:
   JSR   drop_gold_exec
   LDY   #$AF
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@58:
   JSR   shop_buy_handler
   LDY   #$B0
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.case@59:
   JSR   browse_handler
   LDY   #$B1
   STA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   JMP   .loop@0_continue
.default@60:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B2
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.loop@0_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B2
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

turn_dispatch:
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

   LDA   CHAR_AI_MODE
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@118
   LDA   #$00
   JMP   .cmp_end@119
.cmp_true@118:
   LDA   #$01
.cmp_end@119:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDA   #$12

   ; epilogue
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
   RTS
.if_end@0:
   JSR   select_combat_target
   LDY   #$05
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
   CMP   #$02
   BEQ   .cmp_true@120
   LDA   #$00
   JMP   .cmp_end@121
.cmp_true@120:
   LDA   #$01
.cmp_end@121:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@2
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
   JSR   enter_combat_state
   JMP   .if_end@1
.if_else@2:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0C
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
   BVC   .cmp_novf@122
   EOR   #$80
.cmp_novf@122:
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
   BEQ   .if_else@4
   JSR   start_combat
   JMP   .if_end@3
.if_else@4:
   JSR   auto_heal
.if_end@3:
.if_end@1:
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
   LDY   #$10
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

game_turn_loop:
   SUBROUTINE

   ; prologue: 0 arg bytes, 85 local bytes
   SEC
   LDA   SSP
   SBC   #$57
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$56
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
   STA   TURN_ACTIVE
   LDA   ENTITY_PTR
   CLC
   ADC   #$03
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
   JSR   pos_to_colrow
   LDY   #$05
   STA   (FP),Y
   TXA
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
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   STA   TURN_START_COL
   STA   CURRENT_COL
   DEY
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
   STA   TURN_START_ROW
   STA   CURRENT_ROW
   LDA   #$00
   INY
   STA   (FP),Y
   STA   STEPS_TAKEN
   JSR   clear_restricted
   LDA   ENTITY_PTR
   CLC
   ADC   #$06
   LDY   #$0E
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
   LDY   #$10
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   AND   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$14
   AND   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@123
   EOR   #$80
.cmp_novf@123:
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
   BEQ   .if_end@5
   JSR   set_restricted
.if_end@5:
   LDA   ENTITY_PTR
   CLC
   ADC   #$03
   LDY   #$19
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
   LDY   #$1B
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
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@124
   LDA   #$00
   JMP   .cmp_end@125
.cmp_true@124:
   LDA   #$01
.cmp_end@125:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   JSR   set_restricted
.if_end@6:
   LDA   CHAR_AI_MODE
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@126
   LDA   #$00
   JMP   .cmp_end@127
.cmp_true@126:
   LDA   #$01
.cmp_end@127:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   JSR   ai_choose_target
.if_end@7:
   LDA   ENTITY_PTR
   CLC
   ADC   #$0C
   LDY   #$25
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
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   LOW_NIBBLE_MASK
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   STA   MOVE_POINTS
   LDA   ENCOUNTER_RESULT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@128
   LDA   #$00
   JMP   .cmp_end@129
.cmp_true@128:
   LDA   #$01
.cmp_end@129:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@61
   JMP   .if_else@9
.lb_skip@61:
   LDA   MOVE_POINTS
   LDY   #$33
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
   BVC   .cmp_novf@130
   EOR   #$80
.cmp_novf@130:
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
   BNE   .lb_skip@60
   JMP   .if_end@8
.lb_skip@60:
   LDA   #$01
   LDY   #$37
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
   JSR   random_in_range
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   STA   MOVE_POINTS
   JMP   .if_end@8
.if_else@9:
   JSR   check_threats_here
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@131
   LDA   #$00
   JMP   .cmp_end@132
.cmp_true@131:
   LDA   #$01
.cmp_end@132:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@11
   LDA   MOVE_POINTS
   INY
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
   BVC   .cmp_novf@133
   EOR   #$80
.cmp_novf@133:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@11
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@12
.and_false@11:
   LDA   #$00
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@12:
   LDY   #$47
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@59
   JMP   .if_end@13
.lb_skip@59:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   MOVE_POINTS
   LDY   #$01
   STA   (SSP),Y
   JSR   random_in_range
   LDY   #$49
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
   BNE   .cmp_true@134
   LDA   #$00
   JMP   .cmp_end@135
.cmp_true@134:
   LDA   #$01
.cmp_end@135:
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@15
   LDY   #$4A
   LDA   (FP),Y
   STA   MOVE_POINTS
   JMP   .if_end@14
.if_else@15:
   LDA   #$01
   LDY   #$4F
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
   JSR   random_in_range
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   STA   MOVE_POINTS
.if_end@14:
.if_end@13:
.if_end@8:
   LDA   #$02

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$57
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$56
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

move_loop:
   SUBROUTINE

   ; prologue: 0 arg bytes, 150 local bytes
   SEC
   LDA   SSP
   SBC   #$98
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$97
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.loop@61_start:
   LDA   MOVE_POINTS
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@136
   LDA   #$00
   JMP   .cmp_end@137
.cmp_true@136:
   LDA   #$01
.cmp_end@137:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@66
   JMP   .and_false@16
.lb_skip@66:
   LDA   STEPS_TAKEN
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@138
   LDA   #$00
   JMP   .cmp_end@139
.cmp_true@138:
   LDA   #$01
.cmp_end@139:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@18
   LDA   CHAR_AI_MODE
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@140
   LDA   #$00
   JMP   .cmp_end@141
.cmp_true@140:
   LDA   #$01
.cmp_end@141:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@18
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@19
.or_true@18:
   LDA   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@19:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@16
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@17
.and_false@16:
   LDA   #$00
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@17:
   LDY   #$0F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@20
   LDA   #$03

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@20:
   LDA   CHAR_AI_MODE
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@142
   LDA   #$00
   JMP   .cmp_end@143
.cmp_true@142:
   LDA   #$01
.cmp_end@143:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@65
   JMP   .if_else@22
.lb_skip@65:
   JSR   npc_move_ai
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   STA   INPUT_DIR
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@144
   LDA   #$00
   JMP   .cmp_end@145
.cmp_true@144:
   LDA   #$01
.cmp_end@145:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@64
   JMP   .loop@61_start
.lb_skip@64:
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$09
   BEQ   .cmp_true@146
   LDA   #$00
   JMP   .cmp_end@147
.cmp_true@146:
   LDA   #$01
.cmp_end@147:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@24
   JSR   set_restricted
   LDA   #$00
   LDY   #$1F
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
   JSR   set_char_state
   JSR   auto_heal
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@24:
   LDY   #$16
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$05
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@148
   EOR   #$80
.cmp_novf@148:
   ASL
   LDA   #$00
   ROL
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@63
   JMP   .if_end@21
.lb_skip@63:
   JSR   check_and_restrict
   LDY   #$16
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@149
   LDA   #$00
   JMP   .cmp_end@150
.cmp_true@149:
   LDA   #$01
.cmp_end@150:
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@27
   LDA   #$07
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
   JSR   enter_combat_state
   JMP   .if_end@26
.if_else@27:
   LDY   #$16
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@151
   LDA   #$00
   JMP   .cmp_end@152
.cmp_true@151:
   LDA   #$01
.cmp_end@152:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@29
   JSR   start_combat
   JMP   .if_end@28
.if_else@29:
   JSR   set_state_fight
.if_end@28:
.if_end@26:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_else@22:
   JSR   configure_input
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@153
   LDA   #$00
   JMP   .cmp_end@154
.cmp_true@153:
   LDA   #$01
.cmp_end@154:
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@30
   LDA   #$12

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@30:
   JSR   get_input_table_ptr
   LDY   #$32
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
   LDY   #$01
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   setup_input
   JSR   read_keyboard
   LDY   #$34
   STA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   STA   INPUT_DIR
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@155
   LDA   #$00
   JMP   .cmp_end@156
.cmp_true@155:
   LDA   #$01
.cmp_end@156:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@31
   LDA   #$12

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@31:
.if_end@21:
   LDA   ENCOUNTER_RESULT
   LDY   #$39
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
   BVC   .cmp_novf@157
   EOR   #$80
.cmp_novf@157:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@32
   JSR   check_can_leave
   LDY   #$3D
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@158
   LDA   #$00
   JMP   .lnot_end@159
.lnot_true@158:
   LDA   #$01
.lnot_end@159:
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@32
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@33
.and_false@32:
   LDA   #$00
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@33:
   LDY   #$40
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@34
   LDA   #$03

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@34:
   LDA   #$00
   LDY   #$42
   STA   (FP),Y
   STA   ENCOUNTER_RESULT
   LDA   MOVE_POINTS
   INY
   STA   (FP),Y
   LDA   MOVE_POINTS
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   MOVE_POINTS
   LDA   STEPS_TAKEN
   INY
   STA   (FP),Y
   LDA   STEPS_TAKEN
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   STEPS_TAKEN
   LDY   #$16
   LDA   (FP),Y
   LDY   #$47
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
   BVC   .cmp_novf@160
   EOR   #$80
.cmp_novf@160:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@36
   LDA   CURRENT_ROW
   INY
   STA   (FP),Y
   LDA   CURRENT_ROW
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   CURRENT_ROW
   JMP   .if_end@35
.if_else@36:
   LDY   #$16
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@161
   LDA   #$00
   JMP   .cmp_end@162
.cmp_true@161:
   LDA   #$01
.cmp_end@162:
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@38
   LDA   CURRENT_ROW
   INY
   STA   (FP),Y
   LDA   CURRENT_ROW
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   CURRENT_ROW
   JMP   .if_end@37
.if_else@38:
   LDY   #$16
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@163
   LDA   #$00
   JMP   .cmp_end@164
.cmp_true@163:
   LDA   #$01
.cmp_end@164:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@40
   LDA   CURRENT_COL
   INY
   STA   (FP),Y
   LDA   CURRENT_COL
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   CURRENT_COL
   JMP   .if_end@39
.if_else@40:
   LDA   CURRENT_COL
   LDY   #$59
   STA   (FP),Y
   LDA   CURRENT_COL
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   CURRENT_COL
.if_end@39:
.if_end@37:
.if_end@35:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   CURRENT_COL
   LDY   #$01
   STA   (SSP),Y
   LDA   CURRENT_ROW
   INY
   STA   (SSP),Y
   JSR   colrow_to_pos
   LDY   #$5B
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
   JSR   find_entity_at_pos
   LDY   #$5C
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   CMP   ENTITY_NONE+1
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   ENTITY_NONE
   BEQ   .cmp_true@165
.thread@0:
   LDA   #$00
   JMP   .cmp_end@166
.cmp_true@165:
   LDA   #$01
.cmp_end@166:
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@41
   JSR   commit_move
   JSR   check_threats_here
   LDY   #$62
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@168
   LDA   #$00
   JMP   .cmp_end@169
.cmp_true@168:
   LDA   #$01
.cmp_end@169:
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@62
   JMP   .loop@61_start
.lb_skip@62:
   LDA   #$00
   LDY   #$67
   STA   (FP),Y
   STA   MOVE_POINTS
   INY
   STA   (FP),Y
   STA   TURN_ACTIVE
   JMP   .loop@61_start
.if_end@41:
   LDY   #$5E
   LDA   (FP),Y
   LDA   #$00
   LDY   #$69
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   AND   #$80
   LDY   #$6A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@170
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@170
   LDA   #$00
   JMP   .cmp_end@171
.cmp_true@170:
   LDA   #$01
.cmp_end@171:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@43
   LDA   #$11

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@43:
   LDY   #$5E
   LDA   (FP),Y
   CLC
   SBC   #$FF
   INY
   LDA   (FP),Y
   SBC   #$00
   LDA   #$00
   ROL
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@44
   LDY   #$5E
   LDA   (FP),Y
   LDA   #$00
   LDY   #$6F
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   AND   #$80
   LDY   #$70
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@173
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@173
   LDA   #$00
   JMP   .cmp_end@174
.cmp_true@173:
   LDA   #$01
.cmp_end@174:
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@176
   LDA   #$00
   JMP   .lnot_end@177
.lnot_true@176:
   LDA   #$01
.lnot_end@177:
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@44
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@45
.and_false@44:
   LDA   #$00
   LDY   #$75
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@45:
   LDY   #$75
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@178
   LDA   #$00
   JMP   .lnot_end@179
.lnot_true@178:
   LDA   #$01
.lnot_end@179:
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@46
   LDA   #$07
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
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   tune_player
   JSR   show_locked_msg
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@46:
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7F
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
   SBC   #$C0
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@180
   EOR   #$80
.cmp_novf@180:
   ASL
   LDA   #$00
   ROL
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@47
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   handle_encounter
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
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
   LDY   #$80
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$D2
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@181
   EOR   #$80
.cmp_novf@181:
   ASL
   LDA   #$00
   ROL
   LDY   #$87
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@48
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   pickup_treasure
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
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
   LDY   #$80
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$D5
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@182
   EOR   #$80
.cmp_novf@182:
   ASL
   LDA   #$00
   ROL
   LDY   #$8B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@49
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   trigger_scene_event
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
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
   LDY   #$80
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$DA
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@183
   EOR   #$80
.cmp_novf@183:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$8F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@50
   JSR   commit_move
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   display_shop
   LDY   #$91
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@50:
   LDY   #$80
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$FE
   BEQ   .cmp_true@184
   LDA   #$00
   JMP   .cmp_end@185
.cmp_true@184:
   LDA   #$01
.cmp_end@185:
   LDY   #$94
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@51
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   handle_event_slot
   LDY   #$96
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
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
.if_end@51:
   LDA   #$12

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

commit_move:
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

   LDA   CURRENT_COL
   STA   BLINK_COL
   STA   FONT_COL
   LDA   CURRENT_ROW
   STA   BLINK_ROW
   STA   FONT_ROW
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   CURRENT_COL
   LDY   #$01
   STA   (SSP),Y
   LDA   CURRENT_ROW
   INY
   STA   (SSP),Y
   JSR   colrow_to_pos
   LDY   #$01
   STA   (FP),Y
   LDA   ENTITY_PTR
   CLC
   ADC   #$03
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
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDA   CHAR_SPRITE
   STA   BLINK_CHAR
   STA   FONT_CHARNUM
   LDA   #$01
   LDY   #$04
   STA   (FP),Y
   STA   FONT_CHARSET
   JSR   render_font_char
   LDA   TURN_TUNE_ARMED
   STA   TURN_TUNE_GUARD
   JSR   reset_turn_pos

   ; epilogue
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
   RTS

reset_turn_pos:
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

   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   TURN_START_COL
   LDY   #$01
   STA   (SSP),Y
   LDA   TURN_START_ROW
   INY
   STA   (SSP),Y
   JSR   colrow_to_pos
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
   JSR   draw_char_at_pos
   LDA   CURRENT_COL
   STA   TURN_START_COL
   LDA   CURRENT_ROW
   STA   TURN_START_ROW
   LDA   TURN_TUNE_GUARD
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   TURN_TUNE_ARMED
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$04
   CMP   (FP),Y
   BEQ   .cmp_true@186
   LDA   #$00
   JMP   .cmp_end@187
.cmp_true@186:
   LDA   #$01
.cmp_end@187:
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@52
   LDA   TURN_TUNE_GUARD
   INY
   STA   (FP),Y
   LDA   TURN_TUNE_GUARD
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   TURN_TUNE_GUARD
   LDA   #$0B
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   tune_player
.if_end@52:

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

end_turn:
   SUBROUTINE

   ; prologue: 0 arg bytes, 24 local bytes
   SEC
   LDA   SSP
   SBC   #$1A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$19
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
   STA   TURN_ACTIVE
   JSR   check_and_restrict
   LDA   STEPS_TAKEN
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@188
   LDA   #$00
   JMP   .cmp_end@189
.cmp_true@188:
   LDA   #$01
.cmp_end@189:
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@53
   LDA   #$04

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@53:
   LDA   ENTITY_PTR
   CLC
   ADC   #$0C
   LDY   #$06
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
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   ROR
   STA   (FP),Y
   AND   #$07
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0E
   STA   (FP),Y
   LDA   STEPS_TAKEN
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   SEC
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$10
   SBC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@190
   LDA   #$00
   JMP   .sx_done@191
.sx_neg@190:
   LDA   #$FF
.sx_done@191:
   LDY   #$16
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
   BEQ   .if_end@54
   LDA   #$04

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$1A
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
.if_end@54:
   JSR   set_state_fight
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

set_state_fight:
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
   JSR   set_char_state

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

check_and_restrict:
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

   LDA   ENTITY_PTR
   CLC
   ADC   #$03
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
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
   JSR   clear_restricted
   LDY   #$05
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@192
   LDA   #$00
   JMP   .cmp_end@193
.cmp_true@192:
   LDA   #$01
.cmp_end@193:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@55
   JSR   set_restricted
.if_end@55:

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

set_char_state:
   SUBROUTINE

   ; prologue: 1 arg bytes, 18 local bytes
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

   LDA   ENTITY_PTR
   CLC
   ADC   #$0F
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
   LDA   FLAGS_STATE_MASK
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$09
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   AND   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   AND   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0C
   ORA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   ORA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   ENTITY_PTR
   CLC
   ADC   #$0F
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
   LDY   #$10
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$15
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
   RTS

clear_restricted:
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

   LDA   ENTITY_PTR
   CLC
   ADC   #$0F
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
   LDA   FLAGS_ENGAGED_BIT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$07
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   AND   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   AND   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y

   ; epilogue
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
   RTS

set_restricted:
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

   LDA   ENTITY_PTR
   CLC
   ADC   #$0F
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
   LDA   FLAGS_ENGAGED_BIT
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$03
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
   LDY   #$01
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$00
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

TURN_ACTIVE:
   DC.B  $4B

MENU_COMBAT_POS:
   DC.B  $14

TURN_TUNE_ARMED:
   DC.B  $16
