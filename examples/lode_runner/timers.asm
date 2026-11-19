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
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$80
__zpabi_play_note__y	EQU	$80
__local_handle_timers__0	EQU	$81
__local_handle_timers__i	EQU	$82
__local_draw_brick__row	EQU	$83
__local_draw_brick__0	EQU	$84
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
__local_fill_hole__0	EQU	$AE
__local_guard_resurrections__0	EQU	$AE
__local_fill_hole__1	EQU	$AF
__local_guard_resurrections__1	EQU	$AF
__local_fill_hole__2	EQU	$B0
__local_guard_resurrections__g	EQU	$B0
__local_fill_hole__3	EQU	$B1
__local_guard_resurrections__saved_guard_num	EQU	$B1
__local_kill_guard_in_hole__0	EQU	$B2
__local_kill_guard_in_hole__1	EQU	$B3
__local_kill_guard_in_hole__g	EQU	$B4
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
; def draw_brick params=__zpabi_draw_brick__row,__zpabi_draw_brick__col locals=__local_draw_brick__row,__local_draw_brick__0 indirect=false in_cycle=false param_regs=A,X
; def fill_hole params=__zpabi_fill_hole__i locals=__local_fill_hole__0,__local_fill_hole__1,__local_fill_hole__2,__local_fill_hole__3 indirect=false in_cycle=false param_regs=A
; def guard_resurrections params= locals=__local_guard_resurrections__0,__local_guard_resurrections__1,__local_guard_resurrections__g,__local_guard_resurrections__saved_guard_num indirect=false in_cycle=false
; def handle_timers params= locals=__local_handle_timers__0,__local_handle_timers__i indirect=false in_cycle=false
; def kill_guard_in_hole params= locals=__local_kill_guard_in_hole__0,__local_kill_guard_in_hole__1,__local_kill_guard_in_hole__g indirect=false in_cycle=false carry=true
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
; call draw_brick -> draw_sprite_page1
; call draw_brick -> draw_sprite_page2
; call fill_hole -> draw_brick
; call fill_hole -> kill_guard_in_hole
; call guard_resurrections -> draw_guard_sprite
; call guard_resurrections -> draw_sprite_page1
; call guard_resurrections -> draw_sprite_page2
; call guard_resurrections -> load_guard_data
; call guard_resurrections -> load_sound_data
; call handle_timers -> draw_sprite_page2
; call handle_timers -> erase_sprite_at_pixel_coords
; call handle_timers -> fill_hole
; call handle_timers -> guard_resurrections
; call kill_guard_in_hole -> add_and_update_score
; call kill_guard_in_hole -> erase_guard
; call kill_guard_in_hole -> load_guard_data
; item guard_resurrections section=.text local=true zero=false
; item kill_guard_in_hole section=.text local=true zero=false
; item draw_brick section=.text local=true zero=false
; item fill_hole section=.text local=true zero=false
; item handle_timers section=.text local=false zero=false
; item guard_hatched_tune section=.rodata local=true zero=false
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
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end

guard_resurrections:
   SUBROUTINE

   LDA   GUARD_COUNT
   STA   __local_guard_resurrections__g
   BNE   .if_end@0
   RTS
.if_end@0:
   LDA   GUARD_NUM
   STA   __local_guard_resurrections__saved_guard_num
.loop@0_start:
   LDX   __local_guard_resurrections__g
   LDA   GUARD_RESURRECTION_TIMERS,X
   BNE   .lb_skip@1
   JMP   .loop@0_continue
.lb_skip@1:
   STX   GUARD_NUM
   JSR   load_guard_data
   LDX   __local_guard_resurrections__g
   LDA   #$7F
   STA   GUARD_GOLD_TIMERS,X
   LDA   GUARD_LOCS_COL,X
   STA   GAME_COLNUM
   LDA   GUARD_LOCS_ROW,X
   STA   GAME_ROWNUM
   LDA   GUARD_RESURRECTION_TIMERS,X
   SEC
   SBC   #$01
   STA   GUARD_RESURRECTION_TIMERS,X
   BNE   .if_else@3
   LDA   #$01
   STA   GUARD_RESURRECTION_TIMERS,X
   LDX   GAME_ROWNUM
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_resurrections__0
   LDA   LEVEL_MAP@hi,X
   STA   __local_guard_resurrections__1
   LDY   GAME_COLNUM
   LDA   (__local_guard_resurrections__0),Y
   BNE   .if_end@2
   LDA   #$08
   STA   (__local_guard_resurrections__0),Y
   LDA   #$00
   JSR   draw_sprite_page2
   LDX   __local_guard_resurrections__g
   LDA   #$00
   STA   GUARD_GOLD_TIMERS,X
   STA   GUARD_RESURRECTION_TIMERS,X
   LDA   #$08
   JSR   draw_sprite_page1
   LDX   #>guard_hatched_tune
   LDA   #<guard_hatched_tune
   JSR   load_sound_data
   JMP   .if_end@2
.if_else@3:
   CMP   #$13
   BNE   .if_else@6
   LDA   #$39
   JSR   draw_sprite_page2
   LDA   #$39
   JSR   draw_guard_sprite
   JMP   .if_end@5
.if_else@6:
   CMP   #$0A
   BNE   .if_end@7
   LDA   #$3A
   JSR   draw_sprite_page2
   LDA   #$3A
   JSR   draw_guard_sprite
.if_end@7:
.if_end@5:
.if_end@2:
.loop@0_continue:
   DEC   __local_guard_resurrections__g
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   LDA   __local_guard_resurrections__saved_guard_num
   STA   GUARD_NUM
   RTS

kill_guard_in_hole:
   SUBROUTINE

   LDA   GUARD_COUNT
   STA   __local_kill_guard_in_hole__g
.loop@1_start:
   LDX   __local_kill_guard_in_hole__g
   LDA   GUARD_LOCS_COL,X
   CMP   GAME_COLNUM
   BNE   .if_end@10
   LDA   GUARD_LOCS_ROW,X
   CMP   GAME_ROWNUM
   BNE   .if_end@10
   LDA   GUARD_GOLD_TIMERS,X
   BPL   .if_end@11
   DEC   GOLD_COUNT
.if_end@11:
   LDA   #$7F
   STA   GUARD_GOLD_TIMERS,X
   STX   GUARD_NUM
   JSR   load_guard_data
   JSR   erase_guard
   LDA   #$01
   STA   GAME_ROWNUM
   JMP   .loop@2_continue
.kill_guard_in_hole@rot@0:
   INC   GUARD_RESURRECT_COL
   LDA   GUARD_RESURRECT_COL
   CMP   #$1C
   BCC   .loop@2_continue
   INC   GAME_ROWNUM
   LDA   #$00
   STA   GUARD_RESURRECT_COL
.loop@2_continue:
   LDX   GAME_ROWNUM
   LDA   LEVEL_BG@lo,X
   STA   __local_kill_guard_in_hole__0
   LDA   LEVEL_BG@hi,X
   STA   __local_kill_guard_in_hole__1
   LDY   GUARD_RESURRECT_COL
   LDA   (__local_kill_guard_in_hole__0),Y
   BNE   .kill_guard_in_hole@rot@0
   LDX   __local_kill_guard_in_hole__g
   TYA
   STA   GUARD_LOCS_COL,X
   LDA   GAME_ROWNUM
   STA   GUARD_LOCS_ROW,X
   LDA   #$14
   STA   GUARD_RESURRECTION_TIMERS,X
   LDA   #$02
   STA   GUARD_Y_ADJS,X
   STA   GUARD_X_ADJS,X
   LDA   #$00
   STA   GUARD_ANIM_STATES,X
   LDX   #$00
   LDA   #$75
   JSR   add_and_update_score
   SEC
   RTS
.if_end@10:
   DEC   __local_kill_guard_in_hole__g
   BEQ   .lb_skip@2
   JMP   .loop@1_start
.lb_skip@2:
   CLC
   RTS

draw_brick:
   SUBROUTINE

   STA   __local_draw_brick__row
   TXA
   TAY
   LDX   __local_draw_brick__row
   LDA   LEVEL_BG@lo,X
   STA   __local_draw_brick__row
   LDA   LEVEL_MAP@hi,X
   STA   __local_draw_brick__0
   LDA   #$01
   STA   (__local_draw_brick__row),Y
   JSR   draw_sprite_page1
   LDA   #$01
   JMP   draw_sprite_page2

fill_hole:
   SUBROUTINE

   TAX
   LDA   BRICK_DIG_ROWS,X
   STA   __local_fill_hole__3
   LDA   BRICK_DIG_COLS,X
   STA   __local_fill_hole__2
   LDA   BRICK_DIG_ROWS,X
   STA   GAME_ROWNUM
   LDA   BRICK_DIG_COLS,X
   STA   GAME_COLNUM
   LDX   __local_fill_hole__3
   LDA   LEVEL_BG@lo,X
   STA   __local_fill_hole__0
   LDA   LEVEL_MAP@hi,X
   STA   __local_fill_hole__1
   LDY   __local_fill_hole__2
   LDA   (__local_fill_hole__0),Y
   CMP   #$09
   BNE   .if_else@14
   LSR   ALIVE
   JMP   .if_end@13
.if_else@14:
   CMP   #$08
   BNE   .if_else@16
   LDA   LEVEL_BG@hi,X
   STA   __local_fill_hole__1
   LDA   #$01
   STA   (__local_fill_hole__0),Y
   LDX   __local_fill_hole__2
   LDA   __local_fill_hole__3
   JSR   draw_brick
   JSR   kill_guard_in_hole
   BCC   .if_end@15
   RTS
.if_else@16:
   CMP   #$07
   BNE   .if_end@18
   DEC   GOLD_COUNT
.if_end@18:
.if_end@15:
.if_end@13:
   LDX   __local_fill_hole__2
   LDA   __local_fill_hole__3
   JMP   draw_brick

handle_timers:
   SUBROUTINE

   JSR   guard_resurrections
   INC   GUARD_RESURRECT_COL
   LDA   GUARD_RESURRECT_COL
   CMP   #$1C
   BCC   .if_end@19
   LDA   #$00
   STA   GUARD_RESURRECT_COL
.if_end@19:
   LDA   #$1E
   STA   __local_handle_timers__i
.handle_timers@rot@1:
   LDX   __local_handle_timers__i
   LDA   BRICK_FILL_TIMERS,X
   STA   __local_handle_timers__0
   BEQ   .loop@3_continue
   DEC   __local_handle_timers__0
   LDA   __local_handle_timers__0
   STA   BRICK_FILL_TIMERS,X
   BNE   .if_end@21
   TXA
   JSR   fill_hole
   JMP   .loop@3_continue
.if_end@21:
   LDA   BRICK_DIG_COLS,X
   STA   GAME_COLNUM
   LDA   BRICK_DIG_ROWS,X
   STA   GAME_ROWNUM
   LDA   __local_handle_timers__0
   CMP   #$14
   BEQ   .vthread@0
   CMP   #$0A
   BNE   .if_end@24
   CMP   #$14
   BNE   .cond_else@25
.vthread@0:
   LDA   #$37
   BNE   .cond_end@26
.cond_else@25:
   LDA   #$38
.cond_end@26:
   JSR   draw_sprite_page2
   LDX   GAME_COLNUM
   LDY   HALF_SCREEN_COL_TABLE,X
   LDX   GAME_ROWNUM
   LDA   SCREEN_ROW_TABLE,X
   STA   __zpabi_erase_sprite_at_pixel_coords__row
   TYA
   TAX
   LDA   #$00
   JSR   erase_sprite_at_pixel_coords
.if_end@24:
.loop@3_continue:
   DEC   __local_handle_timers__i
   BPL   .handle_timers@rot@1
   RTS

guard_hatched_tune:
   DC.B  $02, $7C, $03, $78, $04, $74, $05, $70, $00

LEVEL_BG@lo:
   DC.B  $00, $1C, $38, $54, $70, $8C, $A8, $C4, $E0, $FC, $18, $34, $50, $6C, $88, $A4
   DC.B  $C0

LEVEL_BG@hi:
   DC.B  $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0B, $0B, $0B, $0B, $0B, $0B
   DC.B  $0B

LEVEL_MAP@hi:
   DC.B  $08, $08, $08, $08, $08, $08, $08, $08, $08, $08, $09, $09, $09, $09, $09, $09
   DC.B  $09
