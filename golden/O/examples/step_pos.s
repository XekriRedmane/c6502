SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_step_pos__0	EQU	$81
__local_step_pos__slot	EQU	$82

; @zp-link-meta-begin
; def step_pos params=__zpabi_step_pos__slot,__zpabi_step_pos__anim_in locals=__local_step_pos__anim_in,__local_step_pos__0,__local_step_pos__slot indirect=false in_cycle=false param_regs=A,X
; ext apply_bobble params=__zpabi_apply_bobble__slot,__zpabi_apply_bobble__bobble_idx param_regs=A,X
; call step_pos -> apply_bobble
; item step_pos section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

step_pos:
   SUBROUTINE

   STA   __local_step_pos__slot
   DEX
   TXA
   TAY
   LDX   __local_step_pos__slot
   TYA
   STA   rescue_anim,X
   LDA   entity_xoff_idx,X
   STA   __local_step_pos__0
   LDA   entity_floor_col,X
   CLC
   ADC   #$03
   BCC   .carry_inc@0
   INC   __local_step_pos__0
.carry_inc@0:
   STA   entity_floor_col,X
   LDA   __local_step_pos__0
   STA   entity_xoff_idx,X
   TYA
   TAX
   LDA   __local_step_pos__slot
   JMP   apply_bobble
