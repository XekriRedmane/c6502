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
saved_enable_sound	EQU	$0D
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
__local_init_game_data__n	EQU	$80
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$80
__zpabi_play_note__y	EQU	$80
__zpabi_poll_inputs__y	EQU	$80
__local_poll_inputs__x	EQU	$83
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
__local_reset_game__0	EQU	$AE
__local_reset_game__p_0	EQU	$AF
__local_reset_game__p_1	EQU	$B0
__local_reset_game__1	EQU	$B1
__local_reset_game__2	EQU	$B2
__local_reset_game__row	EQU	$B3
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
; def append_level_cleared_note params=__zpabi_append_level_cleared_note__n locals=__local_append_level_cleared_note__n indirect=false in_cycle=false param_regs=A
; def cold_start params= locals= indirect=false in_cycle=false
; def init_game_data params= locals=__local_init_game_data__n,__local_init_game_data__0 indirect=false in_cycle=false
; def key_pressed params=__zpabi_key_pressed__key locals=__local_key_pressed__key indirect=false in_cycle=false param_regs=A
; def long_delay params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def play_level params= locals= indirect=false in_cycle=false carry=true
; def poll_inputs params=__zpabi_poll_inputs__mode,__zpabi_poll_inputs__x,__zpabi_poll_inputs__y locals=__local_poll_inputs__mode,__local_poll_inputs__y,__local_poll_inputs__x indirect=false in_cycle=false param_regs=A,X, changes=__zpabi_poll_inputs__y
; def reset_game params= locals=__local_reset_game__0,__local_reset_game__p_0,__local_reset_game__p_1,__local_reset_game__1,__local_reset_game__2,__local_reset_game__row indirect=false in_cycle=false
; def show_hi_scores params= locals= indirect=false in_cycle=false
; def start_at_level_1 params= locals= indirect=false in_cycle=false
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
; ext beep params=
; ext char_to_sprite_num params=__zpabi_char_to_sprite_num__c param_regs=A
; ext check_for_input params=
; ext clear_hgr1 params=
; ext clear_hgr2 params=
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
; ext iris_wipe params=
; ext level_cleared params=
; ext level_editor params=
; ext load_guard_data params=
; ext load_level params=__zpabi_load_level__verbatim param_regs=A
; ext load_sound_data params=__zpabi_load_sound_data__notes_0,__zpabi_load_sound_data__notes_1 param_regs=A,X
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
; ext row_to_addr params=__zpabi_row_to_addr__row,__zpabi_row_to_addr__page param_regs=A,X
; ext screen_row_for params=__zpabi_screen_row_for__row,__zpabi_screen_row_for__y_adj param_regs=A,X
; ext sound_delay params=__zpabi_sound_delay__index param_regs=A
; ext sound_delay1 params=__zpabi_sound_delay1__count param_regs=A
; ext spinning_game_over params= carry=true
; ext start_level params=
; ext start_level_editor params=
; ext to_decimal3 params=__zpabi_to_decimal3__n param_regs=A
; ext wait_for_key params=__zpabi_wait_for_key__cursor param_regs=A
; ext wait_for_key_with_cursor_page_1 params=__zpabi_wait_for_key_with_cursor_page_1__cursor param_regs=A
; ext wait_key params=
; call append_level_cleared_note -> append_note
; call append_level_cleared_note -> play_sound
; call cold_start -> access_hi_score_data_from_disk
; call cold_start -> detect_lack_of_joystick
; call init_game_data -> access_hi_score_data_from_disk
; call init_game_data -> add_and_update_score
; call init_game_data -> append_level_cleared_note
; call init_game_data -> bad_data_disk
; call init_game_data -> key_pressed
; call init_game_data -> load_level
; call init_game_data -> load_sound_data
; call init_game_data -> play_level
; call init_game_data -> play_sound
; call init_game_data -> put_status
; call init_game_data -> put_status_lives
; call init_game_data -> record_hi_score_data_to_disk
; call init_game_data -> spinning_game_over
; call init_game_data -> wait_for_key_with_cursor_page_1
; call init_game_data -> wait_key
; call key_pressed -> access_hi_score_data_from_disk
; call long_delay -> poll_inputs
; call long_delay -> wait_key
; call main -> poll_inputs
; call play_level -> enable_next_level_ladders
; call play_level -> handle_timers
; call play_level -> move_guards
; call play_level -> move_player
; call play_level -> play_sound
; call poll_inputs -> key_pressed
; call reset_game -> clear_hgr1
; call reset_game -> row_to_addr
; call show_hi_scores -> hi_score_screen
; item start_at_level_1 section=.text local=true zero=false
; item show_hi_scores section=.text local=true zero=false
; item poll_inputs section=.text local=true zero=false
; item key_pressed section=.text local=true zero=false
; item main section=.text local=false zero=false
; item cold_start section=.text local=false zero=false
; item reset_game section=.text local=false zero=false
; item long_delay section=.text local=false zero=false
; item append_level_cleared_note section=.text local=true zero=false
; item play_level section=.text local=true zero=false
; item init_game_data section=.text local=false zero=false
; item TIMES_3_TABLE section=.rodata local=true zero=false
; item GUARD_GOLD_TIMER_START_VALUES section=.rodata local=true zero=false
; item DIED_TUNE section=.rodata local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp PLAYER_COL size=1 at=$00 def=false init=
; zp PLAYER_ROW size=1 at=$01 def=false init=
; zp PLAYER_X_ADJ size=1 at=$02 def=false init=
; zp PLAYER_Y_ADJ size=1 at=$03 def=false init=
; zp PLAYER_ANIM_STATE size=1 at=$04 def=false init=
; zp PLAYER_FACING_DIRECTION size=1 at=$05 def=false init=
; zp saved_enable_sound size=1 at=auto def=true init=$00 weight=9 local=true
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

start_at_level_1:
   SUBROUTINE

   LDA   #$00
   STA   DISK_LEVEL_LOC
   LDA   #$01
   STA   LEVELNUM
   STA   HI_SCORE_ELIGIBLE
   LDA   #$02
   STA   GAME_MODE
   JMP   init_game_data

show_hi_scores:
   SUBROUTINE

   JSR   hi_score_screen
   LDA   #$02
   STA   GAME_MODE
   JMP   long_delay

poll_inputs:
   SUBROUTINE

   STX   __local_poll_inputs__x
.loop@0_start:
   CMP   #$CB
   BEQ   .if_end@4
   LDA   BUTN1
   BMI   .poll_inputs@scfold@0
   LDA   BUTN0
   BPL   .if_end@4
.poll_inputs@scfold@0:
   JMP   start_at_level_1
.if_end@4:
   LDA   KBD
   BPL   .if_end@5
   JMP   key_pressed
.if_end@5:
   DEC   __local_poll_inputs__x
   BNE   .if_end@10
   DEC   __zpabi_poll_inputs__y
   BNE   .if_end@10
   DEC   GAME_ROWNUM
   BEQ   .loop@0_break
.if_end@10:
   LDA   INPUT_MODE
   JMP   .loop@0_start
.loop@0_break:
   LDA   GAME_MODE
   BNE   .if_end@11
   LDA   #$01
   STA   GAME_MODE
   STA   LEVELNUM
   STA   DEMO_RUNNING
   STA   HI_SCORE_ELIGIBLE
   LDA   ENABLE_SOUND
   STA   saved_enable_sound
   LDA   #$00
   STA   ENABLE_SOUND
   JMP   init_game_data
.if_end@11:
   CMP   #$01
   BNE   .if_end@12
   JMP   show_hi_scores
.if_end@12:
   JMP   reset_game

key_pressed:
   SUBROUTINE

   STA   KBDSTRB
   CMP   #$85
   BNE   .if_end@13
   JMP   level_editor
.if_end@13:
   CMP   #$8D
   BNE   .if_end@14
   LDA   #$01
   JSR   access_hi_score_data_from_disk
   JMP   show_hi_scores
.if_end@14:
   JMP   start_at_level_1

main:
   SUBROUTINE

   LDA   ROMIN_RDROM_WRRAM2
   LDA   ROMIN_RDROM_WRRAM2
   LDA   TXTCLR
   LDA   MIXCLR
   LDA   TXTPAGE1
   LDA   HIRES
   LDA   KBDSTRB
   LDA   #$02
   STA   __zpabi_poll_inputs__y
   LDX   #$22
   LDA   #$CA
   JMP   poll_inputs

cold_start:
   SUBROUTINE

   JSR   detect_lack_of_joystick
   LDA   #$01
   JSR   access_hi_score_data_from_disk
   JMP   reset_game

reset_game:
   SUBROUTINE

   JSR   clear_hgr1
   LDA   #$20
   STA   DRAW_PAGE
   LDA   #<SPLASH_SCREEN
   STA   __local_reset_game__p_0
   LDA   #>SPLASH_SCREEN
   STA   __local_reset_game__p_1
   LDA   #$00
   STA   GAME_MODE
   STA   DISK_LEVEL_LOC
   STA   __local_reset_game__row
   JMP   .loop@1_start
.reset_game@rot@0:
   LDX   #$20
   JSR   row_to_addr
   STA   __local_reset_game__1
   STX   __local_reset_game__2
   LDX   #$00
.loop@2_start:
   LDY   #$00
   LDA   (__local_reset_game__p_0),Y
   STA   __local_reset_game__0
   INC   __local_reset_game__p_0
   BNE   .inc_done@0
   INC   __local_reset_game__p_1
.inc_done@0:
   ORA   #$00
   BEQ   .loop@2_break
   BPL   .if_else@16
   TXA
   TAY
   LDA   __local_reset_game__0
   STA   (__local_reset_game__1),Y
   INX
   BMI   .loop@2_break
   JMP   .loop@2_start
.if_else@16:
   LDX   __local_reset_game__0
   JMP   .loop@2_start
.loop@2_break:
   INC   __local_reset_game__row
.loop@1_start:
   LDA   __local_reset_game__row
   CMP   #$C0
   BCC   .reset_game@rot@0
   LDA   #$00
   STA   TXTPAGE1
   STA   HIRES
   STA   MIXCLR
   STA   TXTCLR
   JMP   long_delay

long_delay:
   SUBROUTINE

   JSR   wait_key
   LDA   #$03
   STA   GAME_ROWNUM
   LDA   #$FF
   STA   __zpabi_poll_inputs__y
   LDX   #$FF
   LDA   INPUT_MODE
   JMP   poll_inputs

append_level_cleared_note:
   SUBROUTINE

   ASL
   ASL
   ASL
   ASL
   LDX   #$06
   JSR   append_note
   JMP   play_sound

play_level:
   SUBROUTINE

.loop@3_start:
   JSR   move_player
   LDA   ALIVE
   BEQ   .tail@0
   JSR   play_sound
   LDA   GOLD_COUNT
   BNE   .if_end@20
   JSR   enable_next_level_ladders
.if_end@20:
   LDA   PLAYER_ROW
   BNE   .if_end@27
   LDA   PLAYER_Y_ADJ
   CMP   #$02
   BNE   .if_end@27
   LDA   GOLD_COUNT
   BEQ   .play_level@scfold@0
   CMP   #$FF
   BNE   .if_end@27
.play_level@scfold@0:
   SEC
   RTS
.if_end@27:
   JSR   handle_timers
   LDA   ALIVE
   BEQ   .tail@0
   JSR   play_sound
   JSR   move_guards
   LDA   ALIVE
   BNE   .loop@3_start
.tail@0:
   CLC
   RTS

init_game_data:
   SUBROUTINE

   LDA   #$00
   STA   SCORE
   STA   SCORE+1
   STA   SCORE+2
   STA   SCORE+3
   STA   GUARD_PATTERN_OFFSET
   STA   WIPE_MODE
   STA   GUARD_RESURRECT_COL
   STA   DEMO_MOVE_TURNS
   LDA   #<DEMO_MOVES
   STA   DEMO_PTR
   LDA   #>DEMO_MOVES
   STA   DEMO_PTR+1
   LDA   #$05
   STA   LIVES
   LDA   GAME_MODE
   CMP   #$02
   BCC   .if_end@33
   LDA   #$01
   JSR   access_hi_score_data_from_disk
   ORA   #$00
   BNE   .if_end@33
   JSR   bad_data_disk
   JMP   reset_game
.if_end@33:
   JSR   put_status
   LDA   #$00
   STA   TXTPAGE1
.loop@5_start:
   LDA   #$01
   JSR   load_level
   LDA   #$00
   STA   KEY_COMMAND
   STA   KEY_COMMAND_LR
   LDA   GAME_MODE
   CMP   #$02
   BCC   .i0_if_end@18
   JSR   wait_key
   LDA   PLAYER_COL
   STA   GAME_COLNUM
   LDA   PLAYER_ROW
   STA   GAME_ROWNUM
   LDA   #$09
   JSR   wait_for_key_with_cursor_page_1
.i0_if_end@18:
   LDA   #$00
   STA   DIG_DIRECTION
   STA   NOTE_INDEX
   LDA   GUARD_PATTERN_OFFSET
   CLC
   ADC   GUARD_COUNT
   TAX
   LDY   TIMES_3_TABLE,X
   LDA   GUARD_PATTERNS_LIST,Y
   STA   GUARD_PATTERNS
   LDA   GUARD_PATTERNS_LIST+1,Y
   STA   GUARD_PATTERNS+1
   LDA   GUARD_PATTERNS_LIST+2,Y
   STA   GUARD_PATTERNS+2
   LDX   GUARD_PATTERN_OFFSET
   LDA   GUARD_GOLD_TIMER_START_VALUES,X
   STA   GUARD_GOLD_TIMER_START_VALUE
   JSR   play_level
   BCC   .if_end@34
   INC   LEVELNUM
   INC   DISK_LEVEL_LOC
   LDA   LIVES
   CMP   #$FF
   BEQ   .i1_if_end@30
   INC   LIVES
.i1_if_end@30:
   LDA   #$0F
   STA   __local_init_game_data__n
.i1_loop@4_start:
   LDA   __local_init_game_data__n
   BEQ   .loop@5_start
   LDX   #$01
   LDA   #$00
   JSR   add_and_update_score
   LDA   __local_init_game_data__n
   JSR   append_level_cleared_note
   LDA   __local_init_game_data__n
   JSR   append_level_cleared_note
   LDA   __local_init_game_data__n
   JSR   append_level_cleared_note
   DEC   __local_init_game_data__n
   JMP   .i1_loop@4_start
.if_end@34:
   DEC   LIVES
   JSR   put_status_lives
   LDX   #>DIED_TUNE
   LDA   #<DIED_TUNE
   JSR   load_sound_data
.loop@6_continue:
   JSR   play_sound
   BCS   .loop@6_continue
   LDA   GAME_MODE
   CMP   #$02
   BCS   .if_end@35
   LDA   saved_enable_sound
   STA   ENABLE_SOUND
   LDA   KBD
   STA   __local_init_game_data__n
   LDA   DEMO_RUNNING
   BNE   .if_end@36
   LDA   __local_init_game_data__n
   JMP   key_pressed
.if_end@36:
   JMP   long_delay
.if_end@35:
   LDA   LIVES
   BEQ   .lb_skip@0
   JMP   .loop@5_start
.lb_skip@0:
   JSR   record_hi_score_data_to_disk
   JSR   spinning_game_over
   BCC   .if_end@38
   LDA   game_over_key
   JMP   key_pressed
.if_end@38:
   JMP   long_delay

TIMES_3_TABLE:
   DC.B  $00, $03, $06, $09, $0C, $0F, $12, $15, $18

GUARD_GOLD_TIMER_START_VALUES:
   DC.B  $26, $26, $2E, $44, $47, $49, $4A, $4B, $4C, $4D, $4E, $4F, $50

DIED_TUNE:
   DC.B  $02, $40, $02, $40, $03, $50, $03, $50, $04, $60, $04, $60, $05, $70, $05, $70
   DC.B  $06, $80, $06, $80, $07, $90, $07, $90, $08, $A0, $08, $A0, $09, $B0, $09, $B0
   DC.B  $0A, $C0, $0A, $C0, $0B, $D0, $0B, $D0, $0C, $E0, $0C, $E0, $0D, $F0, $0D, $F0
   DC.B  $00

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   saved_enable_sound
   RTS
