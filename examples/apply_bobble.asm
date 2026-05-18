__zpabi_apply_bobble__slot	EQU	$80
__zpabi_apply_bobble__bobble_idx	EQU	$81
__local_apply_bobble__1	EQU	$83

; @zp-link-meta-begin
; def apply_bobble params=__zpabi_apply_bobble__slot,__zpabi_apply_bobble__bobble_idx locals=__local_apply_bobble__0,__local_apply_bobble__1 indirect=false in_cycle=false
; @zp-link-meta-end

apply_bobble:
   SUBROUTINE

.apply_bobble@asm_ssa_block@0:
   LDY   __zpabi_apply_bobble__slot
   LDX   __zpabi_apply_bobble__bobble_idx
   LDA   rescue_bobble,X
   BPL   .if_else@1
.apply_bobble@asm_ssa_block@1:
   AND   #$7F
   STA   __local_apply_bobble__1
   LDA   entity_floor_pos,Y
   CLC
   ADC   __local_apply_bobble__1
   STA   entity_floor_pos,Y
   JMP   .if_end@0
.if_else@1:
   AND   #$7F
   STA   __local_apply_bobble__1
   LDA   entity_floor_pos,Y
   SEC
   SBC   __local_apply_bobble__1
   STA   entity_floor_pos,Y
.if_end@0:
   RTS
