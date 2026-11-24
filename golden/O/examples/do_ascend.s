SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_do_ascend__asc_floor	EQU	$82

; @zp-link-meta-begin
; def do_ascend params=__zpabi_do_ascend__asc_floor,__zpabi_do_ascend__hit_max locals=__local_do_ascend__0,__local_do_ascend__col,__local_do_ascend__asc_floor indirect=false in_cycle=false param_regs=A,X
; ext sfx_tone params=__zpabi_sfx_tone__pitch,__zpabi_sfx_tone__duration param_regs=A,X
; ext snd_delay_up params=__zpabi_snd_delay_up__pitch,__zpabi_snd_delay_up__clicks param_regs=A,X
; call do_ascend -> sfx_tone
; call do_ascend -> snd_delay_up
; item do_ascend section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

do_ascend:
   SUBROUTINE

   STA   __local_do_ascend__asc_floor
   TXA
   TAY
   LDA   player_col
   LDX   __local_do_ascend__asc_floor
   CMP   floor_ceil,X
   BNE   .if_end@0
   LDX   #$04
   LDA   #$05
   JSR   sfx_tone
   DEC   beam_tick
   RTS
.if_end@0:
   SEC
   SBC   #$04
   STA   player_col
   CMP   floor_ceil,X
   BNE   .if_end@1
   LDA   #$00
   STA   move_dir
   LDX   #$04
   LDA   #$05
   JSR   sfx_tone
   DEC   beam_tick
   RTS
.if_end@1:
   CMP   floor_thresh,X
   BNE   .if_end@2
   TXA
   STA   beam_seed_floor
   STA   floor_mirror
   STA   dsc_floor
   LDA   #$FF
   STA   ent_rescued
   JMP   .loop@0_start
.do_ascend@rot@0:
   LDA   #$FF
   STA   entity_hit_state,Y
   DEY
.loop@0_start:
   TYA
   BPL   .do_ascend@rot@0
.if_end@2:
   LDX   #$08
   LDA   #$04
   JMP   snd_delay_up
