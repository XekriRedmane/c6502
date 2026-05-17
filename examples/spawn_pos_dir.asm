__zpabi_spawn_pos_dir_p0	EQU	$80

; @zp-link-meta-begin
; def spawn_pos_dir param_bytes=1 local_bytes=1 indirect=false in_cycle=false
; @zp-link-meta-end

spawn_pos_dir:
   SUBROUTINE

.spawn_pos_dir@asm_ssa_block@0:
   LDX   __zpabi_spawn_pos_dir_p0
   LDA   #$01
   STA   entity_active,X
   STA   rescue_dir,X
   LDA   #$3E
   STA   entity_floor_col,X
   LDA   #$00
   STA   entity_xoff_idx,X
   STA   rescue_anim,X
   LDY   rescue_floor,X
   LDA   floor_thresh,Y
   SEC
   SBC   #$07
   STA   entity_floor_pos,X
   RTS
