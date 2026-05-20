__zpabi_beam_target_tick__beam_tick	EQU	$80
__zpabi_beam_target_tick__beam_seed_floor	EQU	$81
__zpabi_snd_delay_down__pitch	EQU	$82
__zpabi_snd_delay_up__pitch	EQU	$82
__zpabi_snd_delay_down__clicks	EQU	$83
__zpabi_snd_delay_up__clicks	EQU	$83
__local_beam_target_tick__0	EQU	$84

; @zp-link-meta-begin
; def beam_target_tick params=__zpabi_beam_target_tick__beam_tick,__zpabi_beam_target_tick__beam_seed_floor locals=__local_beam_target_tick__0,__local_beam_target_tick__1,__local_beam_target_tick__2 indirect=false in_cycle=false
; ext snd_delay_down params=__zpabi_snd_delay_down__pitch,__zpabi_snd_delay_down__clicks
; ext snd_delay_up params=__zpabi_snd_delay_up__pitch,__zpabi_snd_delay_up__clicks
; call beam_target_tick -> snd_delay_down
; call beam_target_tick -> snd_delay_up
; @zp-link-meta-end

beam_target_tick:
   SUBROUTINE

   LDX   beam_snd_ctr
   TXA
   BMI   .if_end@0
   SEC
   SBC   #$01
   STA   beam_snd_ctr
   LDA   beam_jingle,X
   STA   __zpabi_snd_delay_up__pitch
   LDA   #$0A
   STA   __zpabi_snd_delay_up__clicks
   JSR   snd_delay_up
.if_end@0:
   LDX   beam_state
   TXA
   BPL   .if_end@1
   AND   #$0F
   TAX
   LDA   floor_y_table,X
   STA   __local_beam_target_tick__0
   LDA   beam_y
   CMP   __local_beam_target_tick__0
   BNE   .if_end@2
   LDA   #$00
   STA   beam_state
   RTS
.if_end@2:
   CLC
   ADC   #$02
   STA   beam_y
   LDA   #$10
   STA   __zpabi_snd_delay_down__pitch
   LDA   #$0A
   STA   __zpabi_snd_delay_down__clicks
   JMP   snd_delay_down
.if_end@1:
   BEQ   .if_end@3
   LDA   floor_ceil,X
   STA   __local_beam_target_tick__0
   INC   __local_beam_target_tick__0
   LDA   __local_beam_target_tick__0
   CMP   beam_y
   BNE   .if_end@4
   TXA
   ORA   #$F0
   STA   beam_state
   RTS
.if_end@4:
   LDA   beam_y
   SEC
   SBC   #$02
   STA   beam_y
   LDA   #$10
   STA   __zpabi_snd_delay_up__pitch
   LDA   #$0A
   STA   __zpabi_snd_delay_up__clicks
   JMP   snd_delay_up
.if_end@3:
   LDA   __zpabi_beam_target_tick__beam_tick
   BNE   .if_end@5
   LDX   __zpabi_beam_target_tick__beam_seed_floor
   LDA   floor_y_table,X
   STA   beam_y
   LDA   __zpabi_beam_target_tick__beam_seed_floor
   STA   beam_state
.if_end@5:
   RTS

floor_y_table:
   DC.B  $00, $43, $6B, $93, $BB

beam_jingle:
   DC.B  $1E, $09, $16, $18, $1B, $0A, $07, $05, $1E, $1B, $19
