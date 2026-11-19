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
__local_add_and_update_score__bcd_points_1	EQU	$80
__local_row_to_addr__row	EQU	$80
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$80
__zpabi_play_note__y	EQU	$80
__local_add_and_update_score__i	EQU	$81
__local_clear_page__page_0	EQU	$81
__local_put_string__s_0	EQU	$81
__local_row_to_addr__0	EQU	$81
__local_clear_page__page_1	EQU	$82
__local_put_string__s_1	EQU	$82
__local_row_to_addr__page	EQU	$82
__local_to_decimal3__n	EQU	$82
__local_bcd_to_decimal2__bcd	EQU	$83
__local_draw_at_cursor__sprite	EQU	$84
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
__local_put_status_draw__line179	EQU	$AE
__local_put_status_draw__0	EQU	$AF
__local_put_status_draw__line178	EQU	$B0
__local_put_status_draw__1	EQU	$B1
__local_put_status_draw__line177	EQU	$B2
__local_put_status_draw__2	EQU	$B3
__local_put_status_draw__line176	EQU	$B4
__local_put_status_draw__3	EQU	$B5
__local_put_status_draw__4	EQU	$B6
__local_char_to_sprite_num__i	EQU	$B8
__local_char_to_sprite_num__c	EQU	$B9
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
; def add_and_update_score params=__zpabi_add_and_update_score__bcd_points_0,__zpabi_add_and_update_score__bcd_points_1 locals=__local_add_and_update_score__bcd_points_1,__local_add_and_update_score__i,__local_add_and_update_score__0 indirect=false in_cycle=false param_regs=A,X
; def bcd_to_decimal2 params=__zpabi_bcd_to_decimal2__bcd locals=__local_bcd_to_decimal2__bcd indirect=false in_cycle=false param_regs=A
; def char_to_sprite_num params=__zpabi_char_to_sprite_num__c locals=__local_char_to_sprite_num__0,__local_char_to_sprite_num__i,__local_char_to_sprite_num__c indirect=false in_cycle=false param_regs=A
; def clear_hgr1 params= locals= indirect=false in_cycle=false
; def clear_hgr2 params= locals= indirect=false in_cycle=false
; def clear_page params=__zpabi_clear_page__page_0,__zpabi_clear_page__page_1 locals=__local_clear_page__pages,__local_clear_page__page_0,__local_clear_page__page_1 indirect=false in_cycle=false param_regs=A,X
; def draw_at_cursor params=__zpabi_draw_at_cursor__sprite locals=__local_draw_at_cursor__sprite indirect=false in_cycle=false param_regs=A
; def newline params= locals= indirect=false in_cycle=false
; def put_char params=__zpabi_put_char__c locals=__local_put_char__c indirect=false in_cycle=false param_regs=A
; def put_digit params=__zpabi_put_digit__digit locals=__local_put_digit__digit indirect=false in_cycle=false param_regs=A
; def put_status params= locals= indirect=false in_cycle=false
; def put_status_byte params=__zpabi_put_status_byte__n,__zpabi_put_status_byte__col locals=__local_put_status_byte__col,__local_put_status_byte__n indirect=false in_cycle=false param_regs=A,X
; def put_status_draw params= locals=__local_put_status_draw__line179,__local_put_status_draw__0,__local_put_status_draw__line178,__local_put_status_draw__1,__local_put_status_draw__line177,__local_put_status_draw__2,__local_put_status_draw__line176,__local_put_status_draw__3,__local_put_status_draw__4 indirect=false in_cycle=false
; def put_status_level params= locals= indirect=false in_cycle=false
; def put_status_lives params= locals= indirect=false in_cycle=false
; def put_string params=__zpabi_put_string__s_0,__zpabi_put_string__s_1 locals=__local_put_string__0,__local_put_string__s_0,__local_put_string__s_1 indirect=false in_cycle=false param_regs=A,X
; def row_to_addr params=__zpabi_row_to_addr__row,__zpabi_row_to_addr__page locals=__local_row_to_addr__row,__local_row_to_addr__0,__local_row_to_addr__page indirect=false in_cycle=false param_regs=A,X
; def to_decimal3 params=__zpabi_to_decimal3__n locals=__local_to_decimal3__n indirect=false in_cycle=false param_regs=A
; ext DSKFORM params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A carry=true
; ext SEEKABS params=__zpabi_SEEKABS__track param_regs=A
; ext access_compressed_level_data params=__zpabi_access_compressed_level_data__command param_regs=A
; ext access_disk_or_reset_game params=
; ext access_hi_score_data_from_disk params=__zpabi_access_hi_score_data_from_disk__command param_regs=A
; ext append_note params=__zpabi_append_note__pitch,__zpabi_append_note__duration param_regs=A,X
; ext bad_data_disk params=
; ext beep params=
; ext check_for_input params=
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
; ext hi_score_screen params=
; ext init_game_data params=
; ext iris_wipe params=
; ext level_editor params=
; ext load_guard_data params=
; ext load_level params=__zpabi_load_level__verbatim param_regs=A
; ext load_sound_data params=__zpabi_load_sound_data__notes_0,__zpabi_load_sound_data__notes_1 param_regs=A,X
; ext long_delay params=
; ext move_guards params=
; ext move_player params=
; ext play_note params=__zpabi_play_note__pitch,__zpabi_play_note__duration,__zpabi_play_note__y param_regs=A,X,
; ext play_sound params= carry=true
; ext record_hi_score_data_to_disk params=
; ext reset_game params=
; ext screen_row_for params=__zpabi_screen_row_for__row,__zpabi_screen_row_for__y_adj param_regs=A,X
; ext sound_delay params=__zpabi_sound_delay__index param_regs=A
; ext sound_delay1 params=__zpabi_sound_delay1__count param_regs=A
; ext spinning_game_over params= carry=true
; ext start_level_editor params=
; ext wait_for_key params=__zpabi_wait_for_key__cursor param_regs=A
; ext wait_for_key_with_cursor_page_1 params=__zpabi_wait_for_key_with_cursor_page_1__cursor param_regs=A
; ext wait_key params=
; call add_and_update_score -> bcd_to_decimal2
; call add_and_update_score -> put_digit
; call clear_hgr1 -> clear_page
; call clear_hgr2 -> clear_page
; call draw_at_cursor -> draw_sprite_page1
; call draw_at_cursor -> draw_sprite_page2
; call put_char -> char_to_sprite_num
; call put_char -> draw_at_cursor
; call put_char -> newline
; call put_digit -> draw_at_cursor
; call put_status -> clear_hgr1
; call put_status -> clear_hgr2
; call put_status -> put_status_draw
; call put_status_byte -> put_digit
; call put_status_byte -> to_decimal3
; call put_status_draw -> add_and_update_score
; call put_status_draw -> put_status_level
; call put_status_draw -> put_status_lives
; call put_status_draw -> put_string
; call put_status_level -> put_status_byte
; call put_status_lives -> put_status_byte
; call put_string -> put_char
; item clear_page section=.text local=true zero=false
; item clear_hgr1 section=.text local=false zero=false
; item clear_hgr2 section=.text local=false zero=false
; item put_status section=.text local=false zero=false
; item put_status_draw section=.text local=false zero=false
; item row_to_addr section=.text local=false zero=false
; item put_status_byte section=.text local=true zero=false
; item put_status_lives section=.text local=false zero=false
; item put_status_level section=.text local=false zero=false
; item add_and_update_score section=.text local=false zero=false
; item bcd_to_decimal2 section=.text local=false zero=false
; item to_decimal3 section=.text local=false zero=false
; item draw_at_cursor section=.text local=true zero=false
; item put_digit section=.text local=false zero=false
; item char_to_sprite_num section=.text local=false zero=false
; item put_char section=.text local=false zero=false
; item newline section=.text local=false zero=false
; item put_string section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item PUNCTUATION section=.rodata local=true zero=false
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
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end

clear_page:
   SUBROUTINE

   STA   __local_clear_page__page_0
   STX   __local_clear_page__page_1
   LDX   #$20
   LDA   #$80
.clear_page@rot@0:
   LDY   #$00
.loop@1_start:
   STA   (__local_clear_page__page_0),Y
   INY
   BNE   .loop@1_start
   INC   __local_clear_page__page_1
   DEX
   BNE   .clear_page@rot@0
   RTS

clear_hgr1:
   SUBROUTINE

   LDX   #>HGR1
   LDA   #<HGR1
   JMP   clear_page

clear_hgr2:
   SUBROUTINE

   LDX   #>HGR2
   LDA   #<HGR2
   JMP   clear_page

put_status:
   SUBROUTINE

   JSR   clear_hgr1
   JSR   clear_hgr2
   JMP   put_status_draw

put_status_draw:
   SUBROUTINE

   LDA   DRAW_PAGE
   CMP   #$40
   BNE   .cond_else@0
   LDA   #<HGR2
   STA   __local_put_status_draw__line179
   LDA   #>HGR2
   STA   __local_put_status_draw__4
   JMP   .cond_end@1
.cond_else@0:
   LDA   #<HGR1
   STA   __local_put_status_draw__line179
   LDY   #>HGR1
   STY   __local_put_status_draw__4
.cond_end@1:
   LDA   __local_put_status_draw__line179
   CLC
   ADC   #$50
   STA   __local_put_status_draw__line176
   LDA   __local_put_status_draw__4
   ADC   #$03
   STA   __local_put_status_draw__3
   LDA   __local_put_status_draw__line179
   CLC
   ADC   #$50
   STA   __local_put_status_draw__line177
   LDA   __local_put_status_draw__4
   ADC   #$07
   STA   __local_put_status_draw__2
   LDA   __local_put_status_draw__line179
   CLC
   ADC   #$50
   STA   __local_put_status_draw__line178
   LDA   __local_put_status_draw__4
   ADC   #$0B
   STA   __local_put_status_draw__1
   LDA   __local_put_status_draw__line179
   CLC
   ADC   #$50
   STA   __local_put_status_draw__line179
   LDA   __local_put_status_draw__4
   ADC   #$0F
   STA   __local_put_status_draw__0
   LDY   #$27
.loop@2_start:
   LDA   #$AA
   STA   (__local_put_status_draw__line176),Y
   STA   (__local_put_status_draw__line177),Y
   STA   (__local_put_status_draw__line178),Y
   STA   (__local_put_status_draw__line179),Y
   DEY
   LDA   #$D5
   STA   (__local_put_status_draw__line176),Y
   STA   (__local_put_status_draw__line177),Y
   STA   (__local_put_status_draw__line178),Y
   STA   (__local_put_status_draw__line179),Y
   DEY
   BPL   .loop@2_start
   LDA   #$10
   STA   GAME_ROWNUM
   LDA   #$00
   STA   GAME_COLNUM
   LDX   #>str@0
   LDA   #<str@0
   JSR   put_string
   JSR   put_status_lives
   JSR   put_status_level
   LDX   #$00
   TXA
   JMP   add_and_update_score

row_to_addr:
   SUBROUTINE

   STA   __local_row_to_addr__row
   STX   __local_row_to_addr__page
   LDX   __local_row_to_addr__row
   LDA   ROW_TO_OFFSET_LO,X
   STA   __local_row_to_addr__0
   LDA   ROW_TO_OFFSET_HI,X
   ORA   __local_row_to_addr__page
   TAX
   LDA   __local_row_to_addr__0
   RTS

put_status_byte:
   SUBROUTINE

   STX   GAME_COLNUM
   JSR   to_decimal3
   LDA   #$10
   STA   GAME_ROWNUM
   LDA   DIGITS
   JSR   put_digit
   LDA   DIGITS+1
   JSR   put_digit
   LDA   DIGITS+2
   JMP   put_digit

put_status_lives:
   SUBROUTINE

   LDX   #$10
   LDA   LIVES
   JMP   put_status_byte

put_status_level:
   SUBROUTINE

   LDX   #$19
   LDA   LEVELNUM
   JMP   put_status_byte

add_and_update_score:
   SUBROUTINE

   STA   __local_add_and_update_score__i
   STX   __local_add_and_update_score__bcd_points_1
   SED
   CLC
   LDA   SCORE
   ADC   __local_add_and_update_score__i
   STA   SCORE
   LDA   SCORE+1
   ADC   __local_add_and_update_score__bcd_points_1
   STA   SCORE+1
   LDA   SCORE+2
   ADC   #$00
   STA   SCORE+2
   LDA   SCORE+3
   ADC   #$00
   STA   SCORE+3
   CLD
   LDA   #$05
   STA   GAME_COLNUM
   LDA   #$10
   STA   GAME_ROWNUM
   LDA   SCORE+3
   JSR   bcd_to_decimal2
   LDA   DIGITS+2
   JSR   put_digit
   LDA   #$02
   STA   __local_add_and_update_score__i
.loop@3_start:
   LDX   __local_add_and_update_score__i
   LDA   SCORE,X
   JSR   bcd_to_decimal2
   LDA   DIGITS+1
   JSR   put_digit
   LDA   DIGITS+2
   JSR   put_digit
   DEC   __local_add_and_update_score__i
   BPL   .loop@3_start
   RTS

bcd_to_decimal2:
   SUBROUTINE

   STA   __local_bcd_to_decimal2__bcd
   LSR
   LSR
   LSR
   LSR
   STA   DIGITS+1
   LDA   __local_bcd_to_decimal2__bcd
   AND   #$0F
   STA   DIGITS+2
   RTS

to_decimal3:
   SUBROUTINE

   STA   __local_to_decimal3__n
   LDA   #$00
   STA   DIGITS+1
   STA   DIGITS
   JMP   .loop@4_continue
.to_decimal3@rot@1:
   INC   DIGITS
   SBC   #$64
   STA   __local_to_decimal3__n
.loop@4_continue:
   LDA   __local_to_decimal3__n
   CMP   #$64
   BCS   .to_decimal3@rot@1
   JMP   .loop@5_continue
.to_decimal3@rot@2:
   INC   DIGITS+1
   SBC   #$0A
.loop@5_continue:
   CMP   #$0A
   BCS   .to_decimal3@rot@2
   STA   DIGITS+2
   RTS

draw_at_cursor:
   SUBROUTINE

   STA   __local_draw_at_cursor__sprite
   LDA   DRAW_PAGE
   CMP   #$40
   BNE   .if_else@3
   LDA   __local_draw_at_cursor__sprite
   JSR   draw_sprite_page2
   JMP   .if_end@2
.if_else@3:
   LDA   __local_draw_at_cursor__sprite
   JSR   draw_sprite_page1
.if_end@2:
   INC   GAME_COLNUM
   RTS

put_digit:
   SUBROUTINE

   CLC
   ADC   #$3B
   JMP   draw_at_cursor

char_to_sprite_num:
   SUBROUTINE

   STA   __local_char_to_sprite_num__c
   CMP   #$C1
   BCC   .if_end@6
   CMP   #$DB
   BCS   .if_end@6
   SEC
   SBC   #$7C
   RTS
.if_end@6:
   CMP   #$A0
   BNE   .if_end@7
   LDA   #$00
   RTS
.if_end@7:
   LDA   #$00
   STA   __local_char_to_sprite_num__i
.char_to_sprite_num@rot@3:
   LDX   __local_char_to_sprite_num__i
   LDA   __local_char_to_sprite_num__c
   CMP   PUNCTUATION,X
   BNE   .if_end@8
   LDA   #$5F
   CLC
   ADC   __local_char_to_sprite_num__i
   RTS
.if_end@8:
   INC   __local_char_to_sprite_num__i
   LDA   __local_char_to_sprite_num__i
   CMP   #$07
   BCC   .char_to_sprite_num@rot@3
   LDA   #$10
   RTS

put_char:
   SUBROUTINE

   CMP   #$8D
   BNE   .if_end@9
   JMP   newline
.if_end@9:
   JSR   char_to_sprite_num
   JMP   draw_at_cursor

newline:
   SUBROUTINE

   INC   GAME_ROWNUM
   LDA   #$00
   STA   GAME_COLNUM
   RTS

put_string:
   SUBROUTINE

   STA   __local_put_string__s_0
   STX   __local_put_string__s_1
   JMP   .loop@7_start
.put_string@rot@4:
   ORA   #$80
   JSR   put_char
   INC   __local_put_string__s_0
   BNE   .loop@7_start
   INC   __local_put_string__s_1
.loop@7_start:
   LDY   #$00
   LDA   (__local_put_string__s_0),Y
   BNE   .put_string@rot@4
   RTS

str@0:
   DC.B  $53, $43, $4F, $52, $45, $20, $20, $20, $20, $20, $20, $20, $20, $4D, $45, $4E
   DC.B  $20, $20, $20, $20, $4C, $45, $56, $45, $4C, $20, $20, $20, $00

PUNCTUATION:
   DC.B  $BE, $AE, $A8, $A9, $AF, $AD, $BC
