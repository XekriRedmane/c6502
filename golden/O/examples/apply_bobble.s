SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_apply_bobble__1	EQU	$81
__local_apply_bobble__slot	EQU	$82

; @zp-link-meta-begin
; def apply_bobble params=__zpabi_apply_bobble__slot,__zpabi_apply_bobble__bobble_idx locals=__local_apply_bobble__0,__local_apply_bobble__1,__local_apply_bobble__slot indirect=false in_cycle=false param_regs=A,X
; item apply_bobble section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

apply_bobble:
   SUBROUTINE

   STA   __local_apply_bobble__slot
   LDA   rescue_bobble,X
   BPL   .if_else@1
   AND   #$7F
   LDX   __local_apply_bobble__slot
   CLC
   ADC   entity_floor_pos,X
   STA   entity_floor_pos,X
   RTS
.if_else@1:
   AND   #$7F
   STA   __local_apply_bobble__1
   LDX   __local_apply_bobble__slot
   LDA   entity_floor_pos,X
   SEC
   SBC   __local_apply_bobble__1
   STA   entity_floor_pos,X
   RTS
