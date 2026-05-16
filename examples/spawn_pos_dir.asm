__zpabi_spawn_pos_dir_p0	EQU	$80
__local_spawn_pos_dir_b0	EQU	$81
__local_spawn_pos_dir_b1	EQU	$82
__local_spawn_pos_dir_b2	EQU	$83

; @zp-link-meta-begin
; def spawn_pos_dir param_bytes=1 local_bytes=3 indirect=false in_cycle=false
; @zp-link-meta-end

spawn_pos_dir:
   SUBROUTINE

.spawn_pos_dir@asm_ssa_block@0:
   LDA   #<entity_active
   STA   __local_spawn_pos_dir_b0
   LDA   #>entity_active
   STA   __local_spawn_pos_dir_b0+1
   LDA   #$01
   PHA
   LDY   __zpabi_spawn_pos_dir_p0
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   LDA   #<rescue_dir
   STA   __local_spawn_pos_dir_b0
   LDA   #>rescue_dir
   STA   __local_spawn_pos_dir_b0+1
   LDA   #$01
   PHA
   LDY   __zpabi_spawn_pos_dir_p0
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   LDA   #<entity_floor_col
   STA   __local_spawn_pos_dir_b0
   LDA   #>entity_floor_col
   STA   __local_spawn_pos_dir_b0+1
   LDA   #$3E
   PHA
   LDY   __zpabi_spawn_pos_dir_p0
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   LDA   #<entity_xoff_idx
   STA   __local_spawn_pos_dir_b0
   LDA   #>entity_xoff_idx
   STA   __local_spawn_pos_dir_b0+1
   LDA   #$00
   PHA
   LDY   __zpabi_spawn_pos_dir_p0
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   LDA   #<rescue_anim
   STA   __local_spawn_pos_dir_b0
   LDA   #>rescue_anim
   STA   __local_spawn_pos_dir_b0+1
   LDA   #$00
   PHA
   LDY   __zpabi_spawn_pos_dir_p0
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   LDX   __zpabi_spawn_pos_dir_p0
   LDY   rescue_floor,X
   LDA   floor_thresh,Y
   SEC
   SBC   #$07
   STA   __local_spawn_pos_dir_b2
   LDA   #<entity_floor_pos
   STA   __local_spawn_pos_dir_b0
   LDA   #>entity_floor_pos
   STA   __local_spawn_pos_dir_b0+1
   LDA   __local_spawn_pos_dir_b2
   PHA
   LDY   __zpabi_spawn_pos_dir_p0
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   RTS
