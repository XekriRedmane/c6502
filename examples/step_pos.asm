__zpabi_step_pos_p0	EQU	$80
__zpabi_step_pos_p1	EQU	$81
__zpabi_apply_bobble_p0	EQU	$82
__zpabi_apply_bobble_p1	EQU	$83
__local_step_pos_b0	EQU	$84
__local_step_pos_b1	EQU	$85
__local_step_pos_b2	EQU	$86

; @zp-link-meta-begin
; def step_pos param_bytes=2 local_bytes=3 indirect=false in_cycle=false
; ext apply_bobble param_bytes=2
; call step_pos -> apply_bobble
; @zp-link-meta-end

step_pos:
   SUBROUTINE

.step_pos@asm_ssa_block@0:
   LDA   __zpabi_step_pos_p1
   SEC
   SBC   #$01
   STA   __local_step_pos_b2
   LDX   __zpabi_step_pos_p0
   STA   rescue_anim,X
   LDA   entity_xoff_idx,X
   STA   __local_step_pos_b0
   LDA   entity_floor_col,X
   CLC
   ADC   #$03
   STA   __local_step_pos_b1
   LDA   __local_step_pos_b0
   ADC   #$00
   STA   __local_step_pos_b0
   LDA   __local_step_pos_b1
   STA   entity_floor_col,X
   LDA   __local_step_pos_b0
   STA   entity_xoff_idx,X
   LDA   __zpabi_step_pos_p0
   STA   __zpabi_apply_bobble_p0
   LDA   __local_step_pos_b2
   STA   __zpabi_apply_bobble_p1
   JMP   apply_bobble
