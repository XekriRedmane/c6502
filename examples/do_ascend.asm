__zpabi_do_ascend__asc_floor	EQU	$80
__zpabi_do_ascend__hit_max	EQU	$81
__zpabi_sfx_tone__pitch	EQU	$82
__zpabi_snd_delay_up__pitch	EQU	$82
__zpabi_sfx_tone__duration	EQU	$83
__zpabi_snd_delay_up__clicks	EQU	$83
__local_do_ascend__0	EQU	$84
__local_do_ascend__col	EQU	$85

; @zp-link-meta-begin
; def do_ascend params=__zpabi_do_ascend__asc_floor,__zpabi_do_ascend__hit_max locals=__local_do_ascend__0,__local_do_ascend__col indirect=false in_cycle=false
; ext sfx_tone params=__zpabi_sfx_tone__pitch,__zpabi_sfx_tone__duration
; ext snd_delay_up params=__zpabi_snd_delay_up__pitch,__zpabi_snd_delay_up__clicks
; call do_ascend -> sfx_tone
; call do_ascend -> snd_delay_up
; @zp-link-meta-end

do_ascend:
   SUBROUTINE

   LDA   player_col
   STA   __local_do_ascend__col
   LDX   __zpabi_do_ascend__asc_floor
   LDA   floor_ceil,X
   STA   __local_do_ascend__0
   LDA   __local_do_ascend__col
   CMP   __local_do_ascend__0
   BNE   .if_end@0
   LDA   #$05
   STA   __zpabi_sfx_tone__pitch
   LDA   #$04
   STA   __zpabi_sfx_tone__duration
   JSR   sfx_tone
   DEC   beam_tick
   RTS
.if_end@0:
   SEC
   SBC   #$04
   STA   __local_do_ascend__col
   STA   player_col
   LDA   floor_ceil,X
   STA   __local_do_ascend__0
   LDA   __local_do_ascend__col
   CMP   __local_do_ascend__0
   BNE   .if_end@1
   LDA   #$00
   STA   move_dir
   LDA   #$05
   STA   __zpabi_sfx_tone__pitch
   LDA   #$04
   STA   __zpabi_sfx_tone__duration
   JSR   sfx_tone
   DEC   beam_tick
   RTS
.if_end@1:
   LDA   floor_thresh,X
   STA   __local_do_ascend__0
   LDA   __local_do_ascend__col
   CMP   __local_do_ascend__0
   BNE   .if_end@2
   LDA   __zpabi_do_ascend__asc_floor
   STA   beam_seed_floor
   LDA   __zpabi_do_ascend__asc_floor
   STA   floor_mirror
   LDA   __zpabi_do_ascend__asc_floor
   STA   dsc_floor
   LDA   #$FF
   STA   ent_rescued
   LDX   __zpabi_do_ascend__hit_max
.loop@0_start:
   TXA
   BMI   .loop@0_break
   LDA   #$FF
   STA   entity_hit_state,X
   DEX
   JMP   .loop@0_start
.loop@0_break:
.if_end@2:
   LDA   #$04
   STA   __zpabi_snd_delay_up__pitch
   LDA   #$08
   STA   __zpabi_snd_delay_up__clicks
   JMP   snd_delay_up
