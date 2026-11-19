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
__local_access_hi_score_data_from_disk__0	EQU	$80
__local_compress_and_save_level_data__0	EQU	$80
__local_load_level__verbatim	EQU	$80
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$80
__zpabi_play_note__y	EQU	$80
__local_compress_and_save_level_data__1	EQU	$81
__local_load_level__0	EQU	$81
__local_access_hi_score_data_from_disk__differ	EQU	$82
__local_compress_and_save_level_data__row	EQU	$82
__local_load_level__sprite	EQU	$82
__local_compress_and_save_level_data__low	EQU	$83
__local_load_level__row	EQU	$83
__local_compress_and_save_level_data__index	EQU	$84
__local_load_level__index	EQU	$84
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
__local_draw_level_page2__sprite	EQU	$AE
__local_access_compressed_level_data__1	EQU	$AF
__local_draw_level_page2__dst	EQU	$AF
__local_enable_next_level_ladders__1	EQU	$AF
__local_access_compressed_level_data__2	EQU	$B0
__local_draw_level_page2__0	EQU	$B0
__local_enable_next_level_ladders__2	EQU	$B0
__local_draw_level_page2__src	EQU	$B1
__local_enable_next_level_ladders__3	EQU	$B1
__local_draw_level_page2__1	EQU	$B2
__local_enable_next_level_ladders__4	EQU	$B2
__local_enable_next_level_ladders__5	EQU	$B3
__local_enable_next_level_ladders__6	EQU	$B4
__local_enable_next_level_ladders__n	EQU	$B5
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

; @zp-link-meta-begin
; def access_compressed_level_data params=__zpabi_access_compressed_level_data__command locals=__local_access_compressed_level_data__0,__local_access_compressed_level_data__1,__local_access_compressed_level_data__2 indirect=false in_cycle=false param_regs=A
; def access_disk_or_reset_game params= locals=__local_access_disk_or_reset_game__0 indirect=false in_cycle=false
; def access_hi_score_data_from_disk params=__zpabi_access_hi_score_data_from_disk__command locals=__local_access_hi_score_data_from_disk__0,__local_access_hi_score_data_from_disk__1,__local_access_hi_score_data_from_disk__differ indirect=false in_cycle=false param_regs=A
; def compress_and_save_level_data params= locals=__local_compress_and_save_level_data__0,__local_compress_and_save_level_data__1,__local_compress_and_save_level_data__row,__local_compress_and_save_level_data__low,__local_compress_and_save_level_data__index indirect=false in_cycle=false
; def draw_level_page2 params= locals=__local_draw_level_page2__sprite,__local_draw_level_page2__dst,__local_draw_level_page2__0,__local_draw_level_page2__src,__local_draw_level_page2__1 indirect=false in_cycle=false carry=true
; def enable_next_level_ladders params= locals=__local_enable_next_level_ladders__0,__local_enable_next_level_ladders__1,__local_enable_next_level_ladders__2,__local_enable_next_level_ladders__3,__local_enable_next_level_ladders__4,__local_enable_next_level_ladders__5,__local_enable_next_level_ladders__6,__local_enable_next_level_ladders__n indirect=false in_cycle=false
; def load_level params=__zpabi_load_level__verbatim locals=__local_load_level__verbatim,__local_load_level__0,__local_load_level__sprite,__local_load_level__row,__local_load_level__index indirect=false in_cycle=false param_regs=A
; ext DSKFORM params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A carry=true
; ext SEEKABS params=__zpabi_SEEKABS__track param_regs=A
; ext add_and_update_score params=__zpabi_add_and_update_score__bcd_points_0,__zpabi_add_and_update_score__bcd_points_1 param_regs=A,X
; ext append_note params=__zpabi_append_note__pitch,__zpabi_append_note__duration param_regs=A,X
; ext bad_data_disk params=
; ext bcd_to_decimal2 params=__zpabi_bcd_to_decimal2__bcd param_regs=A
; ext beep params=
; ext char_to_sprite_num params=__zpabi_char_to_sprite_num__c param_regs=A
; ext check_for_input params=
; ext clear_hgr1 params=
; ext clear_hgr2 params=
; ext cold_start params=
; ext detect_lack_of_joystick params=
; ext draw_guard_sprite params=__zpabi_draw_guard_sprite__sprite param_regs=A
; ext draw_sprite_at_pixel_coords params=__zpabi_draw_sprite_at_pixel_coords__sprite,__zpabi_draw_sprite_at_pixel_coords__half_col,__zpabi_draw_sprite_at_pixel_coords__row param_regs=A,X,
; ext draw_sprite_page1 params=__zpabi_draw_sprite_page1__sprite param_regs=A
; ext draw_sprite_page2 params=__zpabi_draw_sprite_page2__sprite param_regs=A
; ext editor_command_loop params=
; ext erase_guard params=
; ext erase_sprite_at_pixel_coords params=__zpabi_erase_sprite_at_pixel_coords__sprite,__zpabi_erase_sprite_at_pixel_coords__half_col,__zpabi_erase_sprite_at_pixel_coords__row param_regs=A,X,
; ext half_screen_col_for params=__zpabi_half_screen_col_for__col,__zpabi_half_screen_col_for__x_adj param_regs=A,X
; ext handle_timers params=
; ext hi_score_screen params=
; ext init_game_data params=
; ext iris_wipe params=
; ext level_editor params=
; ext load_guard_data params=
; ext load_sound_data params=__zpabi_load_sound_data__notes_0,__zpabi_load_sound_data__notes_1 param_regs=A,X
; ext long_delay params=
; ext move_guards params=
; ext move_player params=
; ext newline params=
; ext play_note params=__zpabi_play_note__pitch,__zpabi_play_note__duration,__zpabi_play_note__y param_regs=A,X,
; ext play_sound params= carry=true
; ext put_char params=__zpabi_put_char__c param_regs=A
; ext put_digit params=__zpabi_put_digit__digit param_regs=A
; ext put_status params=
; ext put_status_draw params=
; ext put_status_level params=
; ext put_status_lives params=
; ext put_string params=__zpabi_put_string__s_0,__zpabi_put_string__s_1 param_regs=A,X
; ext record_hi_score_data_to_disk params=
; ext reset_game params=
; ext row_to_addr params=__zpabi_row_to_addr__row,__zpabi_row_to_addr__page param_regs=A,X
; ext screen_row_for params=__zpabi_screen_row_for__row,__zpabi_screen_row_for__y_adj param_regs=A,X
; ext sound_delay params=__zpabi_sound_delay__index param_regs=A
; ext sound_delay1 params=__zpabi_sound_delay1__count param_regs=A
; ext spinning_game_over params= carry=true
; ext start_level_editor params=
; ext to_decimal3 params=__zpabi_to_decimal3__n param_regs=A
; ext wait_for_key params=__zpabi_wait_for_key__cursor param_regs=A
; ext wait_for_key_with_cursor_page_1 params=__zpabi_wait_for_key_with_cursor_page_1__cursor param_regs=A
; ext wait_key params=
; call access_compressed_level_data -> access_disk_or_reset_game
; call access_disk_or_reset_game -> RWTS
; call access_hi_score_data_from_disk -> RWTS
; call compress_and_save_level_data -> access_compressed_level_data
; call draw_level_page2 -> draw_sprite_page2
; call draw_level_page2 -> iris_wipe
; call enable_next_level_ladders -> draw_sprite_at_pixel_coords
; call enable_next_level_ladders -> draw_sprite_page2
; call load_level -> access_compressed_level_data
; call load_level -> draw_level_page2
; item draw_level_page2 section=.text local=true zero=false
; item load_level section=.text local=false zero=false
; item compress_and_save_level_data section=.text local=false zero=false
; item access_compressed_level_data section=.text local=false zero=false
; item access_compressed_level_data@run0 section=.text local=true zero=false
; item access_disk_or_reset_game section=.text local=false zero=false
; item access_hi_score_data_from_disk section=.text local=false zero=false
; item enable_next_level_ladders section=.text local=false zero=false
; item HI_SCORE_DATA_MARKER section=.rodata local=false zero=false
; item LEVEL_BG@lo section=.rodata local=true zero=false merge=true
; item LEVEL_BG@hi section=.rodata local=true zero=false merge=true
; item LEVEL_MAP@hi section=.rodata local=true zero=false merge=true
; zp PLAYER_COL size=1 at=$00 def=false init=
; zp PLAYER_ROW size=1 at=$01 def=false init=
; zp PLAYER_X_ADJ size=1 at=$02 def=false init=
; zp PLAYER_Y_ADJ size=1 at=$03 def=false init=
; zp PLAYER_ANIM_STATE size=1 at=$04 def=false init=
; zp PLAYER_FACING_DIRECTION size=1 at=$05 def=false init=
; zp GUARD_LOC_COL size=1 at=$12 def=false init=
; zp GUARD_LOC_ROW size=1 at=$13 def=false init=
; zp GUARD_ANIM_STATE size=1 at=$14 def=false init=
; zp GUARD_FACING_DIRECTION size=1 at=$15 def=false init=
; zp GUARD_GOLD_TIMER size=1 at=$16 def=false init=
; zp GUARD_X_ADJ size=1 at=$17 def=false init=
; zp GUARD_Y_ADJ size=1 at=$18 def=false init=
; zp GUARD_NUM size=1 at=$19 def=false init=
; zp __dos_zp size=42 at=$26 def=false init=
; zp GUARD_RESURRECT_COL size=1 at=$53 def=false init=
; zp NOTE_INDEX size=1 at=$54 def=false init=
; zp GUARD_GOLD_TIMER_START_VALUE size=1 at=$5F def=false init=
; zp GUARD_PATTERNS size=3 at=$60 def=false init=
; zp GUARD_PHASE size=1 at=$64 def=false init=
; zp PADDLE_VALUES size=2 at=$65 def=false init=
; zp GAME_COLNUM size=1 at=$85 def=false init=
; zp GAME_ROWNUM size=1 at=$86 def=false init=
; zp DRAW_PAGE size=1 at=$87 def=false init=
; zp DIGITS size=3 at=$89 def=false init=
; zp FRAME_PERIOD size=1 at=$8C def=false init=
; zp GUARD_COUNT size=1 at=$8D def=false init=
; zp SCORE size=4 at=$8E def=false init=
; zp GOLD_COUNT size=1 at=$93 def=false init=
; zp DIDNT_PICK_UP_GOLD size=1 at=$94 def=false init=
; zp INPUT_MODE size=1 at=$95 def=false init=
; zp DISK_LEVEL_LOC size=1 at=$96 def=false init=
; zp GUARD_PATTERN_OFFSET size=1 at=$97 def=false init=
; zp LIVES size=1 at=$98 def=false init=
; zp ENABLE_SOUND size=1 at=$99 def=false init=
; zp ALIVE size=1 at=$9A def=false init=
; zp NOT_FALLING size=1 at=$9B def=false init=
; zp DIG_DIRECTION size=1 at=$9C def=false init=
; zp HI_SCORE_ELIGIBLE size=1 at=$9D def=false init=
; zp KEY_COMMAND size=1 at=$9E def=false init=
; zp KEY_COMMAND_LR size=1 at=$9F def=false init=
; zp DIG_ANIM_STATE size=1 at=$A0 def=false init=
; zp VERBATIM size=1 at=$A2 def=false init=
; zp LADDER_COUNT size=1 at=$A3 def=false init=
; zp FALL_PITCH size=1 at=$A4 def=false init=
; zp WIPE_MODE size=1 at=$A5 def=false init=
; zp LEVELNUM size=1 at=$A6 def=false init=
; zp GAME_MODE size=1 at=$A7 def=false init=
; zp DEMO_PTR size=2 at=$A8 def=false init=
; zp DEMO_MOVE size=1 at=$AA def=false init=
; zp DEMO_MOVE_TURNS size=1 at=$AB def=false init=
; zp DEMO_RUNNING size=1 at=$AC def=false init=
; zp LEVEL_SAVED size=1 at=$AD def=false init=
; zp BLOCK_DATA size=33 at=$DF def=false init=
; runtime hargs_size=10 hargs_need=0
; @zp-link-meta-end

draw_level_page2:
   SUBROUTINE

   LDA   #$0F
   STA   GAME_ROWNUM
.loop@0_start:
   LDA   #$1B
   STA   GAME_COLNUM
.loop@1_start:
   TAY
   LDX   GAME_ROWNUM
   LDA   LEVEL_BG@lo,X
   STA   __local_draw_level_page2__sprite
   LDA   LEVEL_MAP@hi,X
   STA   __local_draw_level_page2__dst
   LDA   (__local_draw_level_page2__sprite),Y
   STA   __local_draw_level_page2__sprite
   LDA   VERBATIM
   BNE   .lb_skip@2
   JMP   .if_end@0
.lb_skip@2:
   LDA   __local_draw_level_page2__sprite
   CMP   #$06
   BEQ   .case@3
   CMP   #$07
   BEQ   .case@4
   CMP   #$08
   BEQ   .case@5
   CMP   #$09
   BEQ   .case@6
   CMP   #$05
   BEQ   .case@7
   JMP   .vthread@0
.case@3:
   LDA   LADDER_COUNT
   CMP   #$2D
   BCS   .draw_level_page2@remove
   TAX
   INX
   STX   LADDER_COUNT
   LDA   GAME_ROWNUM
   STA   LADDER_LOCS_ROW,X
   TYA
   STA   LADDER_LOCS_COL,X
   JMP   .draw_level_page2@remove
.case@4:
   INC   GOLD_COUNT
   JMP   .switch@2_break
.case@5:
   LDA   GUARD_COUNT
   CMP   #$05
   BCS   .draw_level_page2@remove
   TAX
   INX
   STX   GUARD_COUNT
   TYA
   STA   GUARD_LOCS_COL,X
   LDA   GAME_ROWNUM
   STA   GUARD_LOCS_ROW,X
   LDA   #$00
   STA   GUARD_GOLD_TIMERS,X
   STA   GUARD_ANIM_STATES,X
   LDA   #$02
   STA   GUARD_X_ADJS,X
   STA   GUARD_Y_ADJS,X
   LDX   GAME_ROWNUM
.xjump@0:
   LDA   LEVEL_BG@lo,X
   STA   __local_draw_level_page2__dst
   LDA   LEVEL_BG@hi,X
   STA   __local_draw_level_page2__0
   LDA   #$00
   STA   (__local_draw_level_page2__dst),Y
   JMP   .switch@2_break
.case@6:
   LDA   PLAYER_COL
   BPL   .draw_level_page2@remove
   STY   PLAYER_COL
   STX   PLAYER_ROW
   LDA   #$02
   STA   PLAYER_X_ADJ
   STA   PLAYER_Y_ADJ
   LDA   #$08
   STA   PLAYER_ANIM_STATE
   JMP   .xjump@0
.case@7:
   LDA   #$01
   STA   __local_draw_level_page2__sprite
   JMP   .switch@2_break
.draw_level_page2@remove:
   LDX   GAME_ROWNUM
   LDA   LEVEL_BG@lo,X
   STA   __local_draw_level_page2__sprite
   LDA   LEVEL_MAP@hi,X
   STA   __local_draw_level_page2__dst
   LDA   #$00
   STA   (__local_draw_level_page2__sprite),Y
   LDX   GAME_ROWNUM
   LDA   LEVEL_BG@lo,X
   STA   __local_draw_level_page2__sprite
   LDA   LEVEL_BG@hi,X
   STA   __local_draw_level_page2__dst
   LDA   #$00
   STA   (__local_draw_level_page2__sprite),Y
   STA   __local_draw_level_page2__sprite
.switch@2_break:
.if_end@0:
   LDA   __local_draw_level_page2__sprite
.vthread@0:
   JSR   draw_sprite_page2
   DEC   GAME_COLNUM
   LDA   GAME_COLNUM
   BMI   .lb_skip@1
   JMP   .loop@1_start
.lb_skip@1:
   DEC   GAME_ROWNUM
   BMI   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   LDA   VERBATIM
   BNE   .if_end@4
   LDA   #<HGR2
   STA   __local_draw_level_page2__src
   LDA   #>HGR2
   STA   __local_draw_level_page2__1
   LDA   #<HGR1
   STA   __local_draw_level_page2__dst
   LDA   #>HGR1
   STA   __local_draw_level_page2__0
.loop@8_continue:
   LDA   __local_draw_level_page2__0
   CMP   #>(HGR1+8192)
   BNE   .jcmp_skip@0
   LDA   __local_draw_level_page2__dst
   CMP   #<HGR1
   BEQ   .tail@0
.jcmp_skip@0:
   LDY   #$00
   LDA   (__local_draw_level_page2__src),Y
   INC   __local_draw_level_page2__src
   BNE   .inc_done@0
   INC   __local_draw_level_page2__1
.inc_done@0:
   LDY   #$00
   STA   (__local_draw_level_page2__dst),Y
   INC   __local_draw_level_page2__dst
   BNE   .loop@8_continue
   INC   __local_draw_level_page2__0
   JMP   .loop@8_continue
.if_end@4:
   LDA   PLAYER_COL
   BPL   .if_end@5
   SEC
   RTS
.if_end@5:
   JSR   iris_wipe
   LDA   #$0F
   STA   GAME_ROWNUM
.loop@9_start:
   LDA   #$1B
   STA   GAME_COLNUM
.loop@10_start:
   LDX   GAME_ROWNUM
   LDA   LEVEL_BG@lo,X
   STA   __local_draw_level_page2__sprite
   LDA   LEVEL_MAP@hi,X
   STA   __local_draw_level_page2__dst
   LDY   GAME_COLNUM
   LDA   (__local_draw_level_page2__sprite),Y
   AND   #$FE
   CMP   #$08
   BNE   .if_end@8
   LDA   #$00
   JSR   draw_sprite_page2
.if_end@8:
   DEC   GAME_COLNUM
   BPL   .loop@10_start
   DEC   GAME_ROWNUM
   BPL   .loop@9_start
.tail@0:
   CLC
   RTS

load_level:
   SUBROUTINE

.loop@11_start:
   STA   VERBATIM
   LDA   #$FF
   STA   PLAYER_COL
   LDA   #$00
   STA   LADDER_COUNT
   STA   GOLD_COUNT
   STA   GUARD_COUNT
   STA   GUARD_NUM
   STA   DIG_ANIM_STATE
   LDX   #$00
.load_level@rot@0:
   STA   BRICK_FILL_TIMERS,X
   INX
   CPX   #$1F
   BCC   .load_level@rot@0
   LDX   #$00
.load_level@rot@1:
   STA   GUARD_RESURRECTION_TIMERS,X
   INX
   CPX   #$06
   BCC   .load_level@rot@1
   LDA   #$01
   STA   ALIVE
   JSR   access_compressed_level_data
   LDA   #$00
   STA   __local_load_level__index
   STA   __local_load_level__row
.load_level@rot@3:
   LDY   #$00
.load_level@rot@2:
   LDX   __local_load_level__index
   TYA
   LSR
   BCC   .if_else@10
   LDA   DISK_BUFFER,X
   LSR
   LSR
   LSR
   LSR
   STA   __local_load_level__sprite
   INC   __local_load_level__index
   JMP   .if_end@9
.if_else@10:
   LDA   DISK_BUFFER,X
   AND   #$0F
   STA   __local_load_level__sprite
.if_end@9:
   CMP   #$0A
   BCC   .if_end@11
   LDA   #$00
   STA   __local_load_level__sprite
.if_end@11:
   LDX   __local_load_level__row
   LDA   LEVEL_BG@lo,X
   STA   __local_load_level__verbatim
   LDA   LEVEL_MAP@hi,X
   STA   __local_load_level__0
   LDA   __local_load_level__sprite
   STA   (__local_load_level__verbatim),Y
   LDA   LEVEL_BG@hi,X
   STA   __local_load_level__0
   LDA   __local_load_level__sprite
   STA   (__local_load_level__verbatim),Y
   INY
   CPY   #$1C
   BCC   .load_level@rot@2
   INC   __local_load_level__row
   LDA   __local_load_level__row
   CMP   #$10
   BCC   .load_level@rot@3
   JSR   draw_level_page2
   BCS   .if_end@12
   RTS
.if_end@12:
   LDA   DISK_LEVEL_LOC
   BNE   .if_end@13
   JMP   reset_game
.if_end@13:
   LDA   #$00
   STA   DISK_LEVEL_LOC
   INC   GUARD_PATTERN_OFFSET
   LDA   #$01
   JMP   .loop@11_start

compress_and_save_level_data:
   SUBROUTINE

   LDA   #$00
   STA   __local_compress_and_save_level_data__index
   STA   __local_compress_and_save_level_data__low
   STA   __local_compress_and_save_level_data__row
.compress_and_save_level_data@rot@5:
   LDY   #$00
.compress_and_save_level_data@rot@4:
   LDX   __local_compress_and_save_level_data__row
   LDA   LEVEL_BG@lo,X
   STA   __local_compress_and_save_level_data__0
   LDA   LEVEL_MAP@hi,X
   STA   __local_compress_and_save_level_data__1
   LDA   (__local_compress_and_save_level_data__0),Y
   STA   __local_compress_and_save_level_data__0
   TYA
   LSR
   BCC   .if_else@15
   LDA   __local_compress_and_save_level_data__0
   ASL
   ASL
   ASL
   ASL
   ORA   __local_compress_and_save_level_data__low
   LDX   __local_compress_and_save_level_data__index
   STA   DISK_BUFFER,X
   INC   __local_compress_and_save_level_data__index
   JMP   .if_end@14
.if_else@15:
   LDA   __local_compress_and_save_level_data__0
   STA   __local_compress_and_save_level_data__low
.if_end@14:
   INY
   CPY   #$1C
   BCC   .compress_and_save_level_data@rot@4
   INC   __local_compress_and_save_level_data__row
   LDA   __local_compress_and_save_level_data__row
   CMP   #$10
   BCC   .compress_and_save_level_data@rot@5
   LDA   #$02
   JMP   access_compressed_level_data

access_compressed_level_data:
   SUBROUTINE

   STA   DOS_IOB+12
   LDA   GAME_MODE
   CMP   #$02
   BCS   .if_end@16
   LDA   #<DEMO_LEVELS
   STA   __local_access_compressed_level_data__1
   LDA   #>DEMO_LEVELS
   ADC   LEVELNUM
   STA   __local_access_compressed_level_data__2
   LDX   #$00
.loop@18_start:
   TXA
   TAY
   LDA   (__local_access_compressed_level_data__1),Y
   STA   DISK_BUFFER,X
   INX
   BNE   .loop@18_start
   RTS
.if_end@16:
   LDA   DISK_LEVEL_LOC
   LSR
   LSR
   LSR
   LSR
   CLC
   ADC   #$03
   STA   DOS_IOB+4
   LDA   DISK_LEVEL_LOC
   AND   #$0F
   STA   DOS_IOB+5
   LDA   #<DISK_BUFFER
   STA   DOS_IOB+8
   LDA   #>DISK_BUFFER
   JSR   access_compressed_level_data@run0
   JMP   access_disk_or_reset_game

access_compressed_level_data@run0:
   SUBROUTINE

   STA   DOS_IOB+9
   LDA   #$00
   STA   DOS_IOB+3
   RTS

access_disk_or_reset_game:
   SUBROUTINE

   LDA   #>DOS_IOB
   LDY   #<DOS_IOB
   JSR   RWTS
   BCC   .if_end@17
   JMP   reset_game
.if_end@17:
   RTS

access_hi_score_data_from_disk:
   SUBROUTINE

   STA   DOS_IOB+12
   LDA   #$0C
   STA   DOS_IOB+4
   LDA   #$0F
   STA   DOS_IOB+5
   LDA   #<HI_SCORES
   STA   DOS_IOB+8
   LDA   #>HI_SCORES
   JSR   access_compressed_level_data@run0
   LDA   #>DOS_IOB
   LDY   #<DOS_IOB
   JSR   RWTS
   BCC   .if_end@18
   JMP   reset_game
.if_end@18:
   LDA   #$00
   STA   __local_access_hi_score_data_from_disk__differ
   LDX   #$00
.access_hi_score_data_from_disk@rot@6:
   LDA   HI_SCORE_DATA_MARKER,X
   STA   __local_access_hi_score_data_from_disk__0
   LDA   HI_SCORE_MARKER,X
   EOR   __local_access_hi_score_data_from_disk__0
   ORA   __local_access_hi_score_data_from_disk__differ
   STA   __local_access_hi_score_data_from_disk__differ
   INX
   CPX   #$0B
   BCC   .access_hi_score_data_from_disk@rot@6
   ORA   #$00
   BEQ   .if_end@19
   LDA   #$00
   RTS
.if_end@19:
   LDA   HI_SCORE_MASTER_FLAG
   BEQ   .cond_else@20
   LDA   #$01
   RTS
.cond_else@20:
   LDA   #$FF
   RTS

enable_next_level_ladders:
   SUBROUTINE

   LDA   #$00
   STA   LADDER_LOCS_COL
   LDA   LADDER_COUNT
   STA   __local_enable_next_level_ladders__n
.loop@20_start:
   LDA   __local_enable_next_level_ladders__n
   BEQ   .loop@20_break
   LDX   __local_enable_next_level_ladders__n
   LDA   LADDER_LOCS_COL,X
   STA   __local_enable_next_level_ladders__6
   BMI   .loop@20_continue
   LDA   LADDER_LOCS_ROW,X
   STA   __local_enable_next_level_ladders__5
   LDA   __local_enable_next_level_ladders__6
   STA   GAME_COLNUM
   LDA   LADDER_LOCS_ROW,X
   STA   GAME_ROWNUM
   LDX   __local_enable_next_level_ladders__5
   LDA   LEVEL_BG@lo,X
   STA   __local_enable_next_level_ladders__3
   STA   __local_enable_next_level_ladders__1
   LDA   LEVEL_BG@hi,X
   STA   __local_enable_next_level_ladders__4
   LDA   LEVEL_MAP@hi,X
   STA   __local_enable_next_level_ladders__2
   LDY   __local_enable_next_level_ladders__6
   LDA   (__local_enable_next_level_ladders__3),Y
   BEQ   .if_end@23
   LDA   #$01
   STA   LADDER_LOCS_COL
   JMP   .loop@20_continue
.if_end@23:
   LDA   #$03
   STA   (__local_enable_next_level_ladders__3),Y
   LDA   (__local_enable_next_level_ladders__1),Y
   BNE   .if_end@24
   LDA   #$03
   STA   (__local_enable_next_level_ladders__1),Y
.if_end@24:
   LDA   #$03
   JSR   draw_sprite_page2
   LDX   __local_enable_next_level_ladders__6
   LDY   HALF_SCREEN_COL_TABLE,X
   LDX   __local_enable_next_level_ladders__5
   LDA   SCREEN_ROW_TABLE,X
   STA   __zpabi_draw_sprite_at_pixel_coords__row
   TYA
   TAX
   LDA   #$03
   JSR   draw_sprite_at_pixel_coords
   LDX   __local_enable_next_level_ladders__n
   LDA   #$FF
   STA   LADDER_LOCS_COL,X
.loop@20_continue:
   DEC   __local_enable_next_level_ladders__n
   JMP   .loop@20_start
.loop@20_break:
   LDA   LADDER_LOCS_COL
   BNE   .if_end@25
   DEC   GOLD_COUNT
.if_end@25:
   RTS

HI_SCORE_DATA_MARKER:
   DC.B  $CC, $CF, $C4, $C5, $A0, $D2, $D5, $CE, $CE, $C5, $D2

LEVEL_BG@lo:
   DC.B  $00, $1C, $38, $54, $70, $8C, $A8, $C4, $E0, $FC, $18, $34, $50, $6C, $88, $A4
   DC.B  $C0

LEVEL_BG@hi:
   DC.B  $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0B, $0B, $0B, $0B, $0B, $0B
   DC.B  $0B

LEVEL_MAP@hi:
   DC.B  $08, $08, $08, $08, $08, $08, $08, $08, $08, $08, $09, $09, $09, $09, $09, $09
   DC.B  $09
