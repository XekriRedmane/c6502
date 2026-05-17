__zpabi_step_pos__slot	EQU	$80
__zpabi_step_pos__anim_in	EQU	$81
__zpabi_apply_bobble__slot	EQU	$82
__zpabi_apply_bobble__bobble_idx	EQU	$83
__local_step_pos__0	EQU	$84
__local_step_pos__1	EQU	$85
__local_step_pos__2	EQU	$86

; @zp-link-meta-begin
; def step_pos params=__zpabi_step_pos__slot,__zpabi_step_pos__anim_in locals=__local_step_pos__0,__local_step_pos__1,__local_step_pos__2 indirect=false in_cycle=false
; ext apply_bobble params=__zpabi_apply_bobble__slot,__zpabi_apply_bobble__bobble_idx
; call step_pos -> apply_bobble
; @zp-link-meta-end

step_pos:
   SUBROUTINE

.step_pos@asm_ssa_block@0:
   LDA   __zpabi_step_pos__anim_in
   SEC
   SBC   #$01
   STA   __local_step_pos__2
   LDX   __zpabi_step_pos__slot
   STA   rescue_anim,X
   LDA   entity_xoff_idx,X
   STA   __local_step_pos__0
   LDA   entity_floor_col,X
   CLC
   ADC   #$03
   STA   __local_step_pos__1
   LDA   __local_step_pos__0
   ADC   #$00
   STA   __local_step_pos__0
   LDA   __local_step_pos__1
   STA   entity_floor_col,X
   LDA   __local_step_pos__0
   STA   entity_xoff_idx,X
   LDA   __zpabi_step_pos__slot
   STA   __zpabi_apply_bobble__slot
   LDA   __local_step_pos__2
   STA   __zpabi_apply_bobble__bobble_idx
   JMP   apply_bobble
