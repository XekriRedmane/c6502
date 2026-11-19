PLAYER_COL	EQU	$00
PLAYER_ROW	EQU	$01
PLAYER_X_ADJ	EQU	$02
PLAYER_Y_ADJ	EQU	$03
PLAYER_ANIM_STATE	EQU	$04
PLAYER_FACING_DIRECTION	EQU	$05
HARGS	EQU	$06
SSP	EQU	$0B
FP	EQU	$0D
DPTR	EQU	$0F
kbd_entry_index	EQU	$11
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
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$80
__zpabi_play_note__y	EQU	$80
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
__local_record_hi_score_data_to_disk__0	EQU	$AE
__local_record_hi_score_data_to_disk__initials	EQU	$AF
__local_record_hi_score_data_to_disk__1	EQU	$B0
__local_record_hi_score_data_to_disk__i	EQU	$B1
__local_record_hi_score_data_to_disk__2	EQU	$B2
__local_record_hi_score_data_to_disk__3	EQU	$B3
__local_record_hi_score_data_to_disk__4	EQU	$B4
__local_record_hi_score_data_to_disk__n	EQU	$B5
__local_hi_score_screen__0	EQU	$B8
__local_hi_score_screen__1	EQU	$B9
__local_hi_score_screen__i	EQU	$BA
__local_hi_score_screen__2	EQU	$BB
__local_hi_score_screen__3	EQU	$BC
__local_hi_score_screen__n	EQU	$BD
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
; def hi_score_screen params= locals=__local_hi_score_screen__0,__local_hi_score_screen__1,__local_hi_score_screen__i,__local_hi_score_screen__2,__local_hi_score_screen__3,__local_hi_score_screen__n indirect=false in_cycle=false
; def record_hi_score_data_to_disk params= locals=__local_record_hi_score_data_to_disk__0,__local_record_hi_score_data_to_disk__initials,__local_record_hi_score_data_to_disk__1,__local_record_hi_score_data_to_disk__i,__local_record_hi_score_data_to_disk__2,__local_record_hi_score_data_to_disk__3,__local_record_hi_score_data_to_disk__4,__local_record_hi_score_data_to_disk__n,__local_record_hi_score_data_to_disk__5,__local_record_hi_score_data_to_disk__6 indirect=false in_cycle=false
; ext DSKFORM params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A carry=true
; ext SEEKABS params=__zpabi_SEEKABS__track param_regs=A
; ext access_compressed_level_data params=__zpabi_access_compressed_level_data__command param_regs=A
; ext access_disk_or_reset_game params=
; ext access_hi_score_data_from_disk params=__zpabi_access_hi_score_data_from_disk__command param_regs=A
; ext add_and_update_score params=__zpabi_add_and_update_score__bcd_points_0,__zpabi_add_and_update_score__bcd_points_1 param_regs=A,X
; ext append_note params=__zpabi_append_note__pitch,__zpabi_append_note__duration param_regs=A,X
; ext bad_data_disk params=
; ext bcd_to_decimal2 params=__zpabi_bcd_to_decimal2__bcd param_regs=A
; ext beats params=__zpabi_beats__entry_0,__zpabi_beats__entry_1 param_regs=A,X
; ext beep params=
; ext char_to_sprite_num params=__zpabi_char_to_sprite_num__c param_regs=A
; ext check_for_input params=
; ext clear_hgr1 params=
; ext clear_hgr2 params=
; ext cold_start params=
; ext compress_and_save_level_data params=
; ext detect_lack_of_joystick params=
; ext draw_guard_sprite params=__zpabi_draw_guard_sprite__sprite param_regs=A
; ext draw_sprite_at_pixel_coords params=__zpabi_draw_sprite_at_pixel_coords__sprite,__zpabi_draw_sprite_at_pixel_coords__half_col,__zpabi_draw_sprite_at_pixel_coords__row param_regs=A,X,
; ext draw_sprite_page1 params=__zpabi_draw_sprite_page1__sprite param_regs=A
; ext draw_sprite_page2 params=__zpabi_draw_sprite_page2__sprite param_regs=A
; ext editor_command_loop params=
; ext enable_next_level_ladders params=
; ext erase_guard params=
; ext erase_sprite_at_pixel_coords params=__zpabi_erase_sprite_at_pixel_coords__sprite,__zpabi_erase_sprite_at_pixel_coords__half_col,__zpabi_erase_sprite_at_pixel_coords__row param_regs=A,X,
; ext half_screen_col_for params=__zpabi_half_screen_col_for__col,__zpabi_half_screen_col_for__x_adj param_regs=A,X
; ext handle_timers params=
; ext init_game_data params=
; ext iris_wipe params=
; ext level_editor params=
; ext load_guard_data params=
; ext load_level params=__zpabi_load_level__verbatim param_regs=A
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
; call hi_score_screen -> bcd_to_decimal2
; call hi_score_screen -> clear_hgr2
; call hi_score_screen -> newline
; call hi_score_screen -> put_char
; call hi_score_screen -> put_digit
; call hi_score_screen -> put_string
; call hi_score_screen -> to_decimal3
; call record_hi_score_data_to_disk -> access_hi_score_data_from_disk
; call record_hi_score_data_to_disk -> beep
; call record_hi_score_data_to_disk -> char_to_sprite_num
; call record_hi_score_data_to_disk -> hi_score_screen
; call record_hi_score_data_to_disk -> put_char
; call record_hi_score_data_to_disk -> wait_for_key
; item hi_score_screen section=.text local=false zero=false
; item record_hi_score_data_to_disk section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp PLAYER_COL size=1 at=$00 def=false init=
; zp PLAYER_ROW size=1 at=$01 def=false init=
; zp PLAYER_X_ADJ size=1 at=$02 def=false init=
; zp PLAYER_Y_ADJ size=1 at=$03 def=false init=
; zp PLAYER_ANIM_STATE size=1 at=$04 def=false init=
; zp PLAYER_FACING_DIRECTION size=1 at=$05 def=false init=
; zp kbd_entry_index size=1 at=auto def=true init=$60 weight=137 local=true
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
; runtime hargs_size=5 hargs_need=0
; @zp-link-meta-end

hi_score_screen:
   SUBROUTINE

   JSR   clear_hgr2
   LDA   #$40
   STA   DRAW_PAGE
   LDA   #$00
   STA   GAME_COLNUM
   STA   GAME_ROWNUM
   LDX   #>str@0
   LDA   #<str@0
   JSR   put_string
   LDA   #$01
   STA   __local_hi_score_screen__n
.loop@0_start:
   LDA   __local_hi_score_screen__n
   CMP   #$0B
   BCC   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   CMP   #$0A
   BNE   .if_else@1
   LDA   #$01
   JSR   put_digit
   LDA   #$00
   JSR   put_digit
   JMP   .if_end@0
.if_else@1:
   LDA   #$A0
   JSR   put_char
   LDA   __local_hi_score_screen__n
   JSR   put_digit
.if_end@0:
   LDX   #>str@1
   LDA   #<str@1
   JSR   put_string
   LDA   __local_hi_score_screen__n
   SEC
   SBC   #$01
   ASL
   ASL
   ASL
   STA   __local_hi_score_screen__i
   LDA   #<HI_SCORES
   CLC
   ADC   __local_hi_score_screen__i
   STA   __local_hi_score_screen__3
   LDA   #>HI_SCORES
   ADC   #$00
   STA   __local_hi_score_screen__2
   LDX   __local_hi_score_screen__i
   LDA   HI_SCORES+3,X
   BEQ   .if_end@2
   LDA   HI_SCORES,X
   JSR   put_char
   LDX   __local_hi_score_screen__i
   LDA   HI_SCORES+1,X
   JSR   put_char
   LDX   __local_hi_score_screen__i
   LDA   HI_SCORES+2,X
   JSR   put_char
   LDX   #>str@2
   LDA   #<str@2
   JSR   put_string
   LDX   __local_hi_score_screen__i
   LDA   HI_SCORES+3,X
   JSR   to_decimal3
   LDA   DIGITS
   JSR   put_digit
   LDA   DIGITS+1
   JSR   put_digit
   LDA   DIGITS+2
   JSR   put_digit
   LDX   #>str@3
   LDA   #<str@3
   JSR   put_string
   LDA   #$00
   STA   __local_hi_score_screen__i
   JMP   .loop@1_start
.hi_score_screen@rot@0:
   ADC   __local_hi_score_screen__3
   STA   __local_hi_score_screen__0
   LDA   #$00
   ADC   __local_hi_score_screen__2
   STA   __local_hi_score_screen__1
   LDY   #$04
   LDA   (__local_hi_score_screen__0),Y
   JSR   bcd_to_decimal2
   LDA   DIGITS+1
   JSR   put_digit
   LDA   DIGITS+2
   JSR   put_digit
   INC   __local_hi_score_screen__i
.loop@1_start:
   LDA   __local_hi_score_screen__i
   CMP   #$04
   BCC   .hi_score_screen@rot@0
.if_end@2:
   JSR   newline
   INC   __local_hi_score_screen__n
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   STA   TXTPAGE2
   LDA   #$20
   STA   DRAW_PAGE
   RTS

record_hi_score_data_to_disk:
   SUBROUTINE

   LDA   HI_SCORE_ELIGIBLE
   BNE   .if_end@5
   RTS
.if_end@5:
   LDA   SCORE
   ORA   SCORE+1
   ORA   SCORE+2
   ORA   SCORE+3
   BNE   .if_end@6
   RTS
.if_end@6:
   LDA   #$01
   JSR   access_hi_score_data_from_disk
   ORA   #$00
   BNE   .if_end@7
   RTS
.if_end@7:
   LDA   #$01
   STA   __local_record_hi_score_data_to_disk__n
.loop@3_continue:
   SEC
   SBC   #$01
   ASL
   ASL
   ASL
   STA   __local_record_hi_score_data_to_disk__4
   LDA   #<HI_SCORES
   CLC
   ADC   __local_record_hi_score_data_to_disk__4
   STA   __local_record_hi_score_data_to_disk__3
   LDA   #>HI_SCORES
   ADC   #$00
   STA   __local_record_hi_score_data_to_disk__2
   LDX   __local_record_hi_score_data_to_disk__4
   LDA   HI_SCORES+3,X
   STA   __local_record_hi_score_data_to_disk__0
   LDA   LEVELNUM
   CMP   __local_record_hi_score_data_to_disk__0
   BEQ   .i0_if_end@3
.xjump@0:
   CLC
   SBC   __local_record_hi_score_data_to_disk__0
   LDA   #$00
   ROL
   JMP   .i0@end
.i0_if_end@3:
   LDA   #$00
   STA   __local_record_hi_score_data_to_disk__i
.i0_loop@2_start:
   LDA   __local_record_hi_score_data_to_disk__i
   CMP   #$04
   BCS   .vthread@0
   LDA   #$03
   SEC
   SBC   __local_record_hi_score_data_to_disk__i
   TAX
   LDA   __local_record_hi_score_data_to_disk__i
   CLC
   ADC   __local_record_hi_score_data_to_disk__3
   STA   __local_record_hi_score_data_to_disk__0
   LDA   #$00
   ADC   __local_record_hi_score_data_to_disk__2
   STA   __local_record_hi_score_data_to_disk__initials
   LDY   #$04
   LDA   (__local_record_hi_score_data_to_disk__0),Y
   STA   __local_record_hi_score_data_to_disk__0
   LDA   SCORE,X
   CMP   __local_record_hi_score_data_to_disk__0
   BNE   .xjump@0
   INC   __local_record_hi_score_data_to_disk__i
   JMP   .i0_loop@2_start
.i0@end:
   BNE   .loop@3_break
.vthread@0:
   INC   __local_record_hi_score_data_to_disk__n
   LDA   __local_record_hi_score_data_to_disk__n
   CMP   #$0B
   BNE   .loop@3_continue
   RTS
.loop@3_break:
   LDX   #$48
   BNE   .loop@4_start
.record_hi_score_data_to_disk@rot@1:
   DEX
   LDA   HI_SCORES,X
   STA   HI_SCORES+8,X
.loop@4_start:
   CPX   __local_record_hi_score_data_to_disk__4
   BNE   .record_hi_score_data_to_disk@rot@1
   LDA   #<HI_SCORES
   CLC
   ADC   __local_record_hi_score_data_to_disk__4
   STA   __local_record_hi_score_data_to_disk__initials
   LDA   #>HI_SCORES
   ADC   #$00
   STA   __local_record_hi_score_data_to_disk__1
   CLC
   LDX   __local_record_hi_score_data_to_disk__4
   LDA   #$A0
   STA   HI_SCORES,X
   STA   HI_SCORES+1,X
   STA   HI_SCORES+2,X
   LDA   LEVELNUM
   STA   HI_SCORES+3,X
   LDA   #>(HI_SCORES+4)
   STA   __local_record_hi_score_data_to_disk__2
   TXA
   ADC   #<(HI_SCORES+4)
   STA   __local_record_hi_score_data_to_disk__i
   BCC   .carry_inc@0
   INC   __local_record_hi_score_data_to_disk__2
.carry_inc@0:
   LDA   SCORE+3
   LDY   #$00
   STA   (__local_record_hi_score_data_to_disk__i),Y
   LDA   SCORE+2
   INY
   STA   (__local_record_hi_score_data_to_disk__i),Y
   LDA   SCORE+1
   INY
   STA   (__local_record_hi_score_data_to_disk__i),Y
   LDA   SCORE
   INY
   STA   (__local_record_hi_score_data_to_disk__i),Y
   JSR   hi_score_screen
   LDA   #$40
   STA   DRAW_PAGE
   LDA   __local_record_hi_score_data_to_disk__n
   CLC
   ADC   #$04
   STA   GAME_ROWNUM
   LDA   #$07
   STA   GAME_COLNUM
   LDA   #$00
   STA   kbd_entry_index
.loop@5_start:
   LDY   kbd_entry_index
   LDA   (__local_record_hi_score_data_to_disk__initials),Y
   JSR   char_to_sprite_num
   JSR   wait_for_key
   STA   KBDSTRB
   CMP   #$8D
   BEQ   .loop@5_break
   CMP   #$88
   BNE   .if_end@10
   LDA   kbd_entry_index
   BNE   .if_end@11
   JSR   beep
   JMP   .loop@5_start
.if_end@11:
   DEC   kbd_entry_index
   DEC   GAME_COLNUM
   JMP   .loop@5_start
.if_end@10:
   CMP   #$95
   BNE   .if_end@12
   LDA   kbd_entry_index
   CMP   #$02
   BNE   .if_end@13
   JSR   beep
   JMP   .loop@5_start
.if_end@13:
   INC   GAME_COLNUM
   INC   kbd_entry_index
   JMP   .loop@5_start
.if_end@12:
   CMP   #$AE
   BEQ   .if_end@20
   CMP   #$A0
   BEQ   .if_end@20
   CMP   #$C1
   BCC   .record_hi_score_data_to_disk@scfold@0
   CMP   #$DB
   BCC   .if_end@20
.record_hi_score_data_to_disk@scfold@0:
   JSR   beep
   JMP   .loop@5_start
.if_end@20:
   LDY   kbd_entry_index
   STA   (__local_record_hi_score_data_to_disk__initials),Y
   JSR   put_char
   INC   kbd_entry_index
   LDA   kbd_entry_index
   CMP   #$03
   BCC   .loop@5_start
   BCS   .if_end@11
.loop@5_break:
   LDA   #$20
   STA   DRAW_PAGE
   LDA   #$02
   JSR   access_hi_score_data_from_disk
   JMP   long_delay

str@0:
   DC.B  $20, $20, $20, $20, $4C, $4F, $44, $45, $20, $52, $55, $4E, $4E, $45, $52, $20
   DC.B  $48, $49, $47, $48, $20, $53, $43, $4F, $52, $45, $53, $0D, $0D, $0D, $20, $20
   DC.B  $20, $20, $49, $4E, $49, $54, $49, $41, $4C, $53, $20, $4C, $45, $56, $45, $4C
   DC.B  $20, $20, $53, $43, $4F, $52, $45, $0D, $20, $20, $20, $20, $2D, $2D, $2D, $2D
   DC.B  $2D, $2D, $2D, $2D, $20, $2D, $2D, $2D, $2D, $2D, $20, $2D, $2D, $2D, $2D, $2D
   DC.B  $2D, $2D, $2D, $0D, $00

str@1:
   DC.B  $2E, $20, $20, $20, $20, $00

str@2:
   DC.B  $20, $20, $20, $20, $00

str@3:
   DC.B  $20, $20, $00

__zp_init:
   SUBROUTINE

   LDA   #$60
   STA   kbd_entry_index
   RTS
