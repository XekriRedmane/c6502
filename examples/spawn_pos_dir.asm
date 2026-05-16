__local_spawn_pos_dir_b0	EQU	$80
__local_spawn_pos_dir_b1	EQU	$81
__local_spawn_pos_dir_b2	EQU	$82

; @zp-link-meta-begin
; def spawn_pos_dir param_bytes=0 local_bytes=3 indirect=false in_cycle=false
; @zp-link-meta-end

spawn_pos_dir:
   SUBROUTINE

   ; prologue: 1 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.spawn_pos_dir@asm_ssa_block@0:
   LDA   #<entity_active
   STA   __local_spawn_pos_dir_b0
   LDA   #>entity_active
   STA   __local_spawn_pos_dir_b0+1
   LDA   __local_spawn_pos_dir_b0
   STA   DPTR
   LDA   __local_spawn_pos_dir_b1
   STA   DPTR+1
   LDA   #$01
   PHA
   LDY   #$03
   LDA   (FP),Y
   TAY
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   LDA   #<rescue_dir
   STA   __local_spawn_pos_dir_b0
   LDA   #>rescue_dir
   STA   __local_spawn_pos_dir_b0+1
   LDA   __local_spawn_pos_dir_b0
   STA   DPTR
   LDA   __local_spawn_pos_dir_b1
   STA   DPTR+1
   LDA   #$01
   PHA
   LDY   #$03
   LDA   (FP),Y
   TAY
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   LDA   #<entity_floor_col
   STA   __local_spawn_pos_dir_b0
   LDA   #>entity_floor_col
   STA   __local_spawn_pos_dir_b0+1
   LDA   __local_spawn_pos_dir_b0
   STA   DPTR
   LDA   __local_spawn_pos_dir_b1
   STA   DPTR+1
   LDA   #$3E
   PHA
   LDY   #$03
   LDA   (FP),Y
   TAY
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   LDA   #<entity_xoff_idx
   STA   __local_spawn_pos_dir_b0
   LDA   #>entity_xoff_idx
   STA   __local_spawn_pos_dir_b0+1
   LDA   __local_spawn_pos_dir_b0
   STA   DPTR
   LDA   __local_spawn_pos_dir_b1
   STA   DPTR+1
   LDA   #$00
   PHA
   LDY   #$03
   LDA   (FP),Y
   TAY
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   LDA   #<rescue_anim
   STA   __local_spawn_pos_dir_b0
   LDA   #>rescue_anim
   STA   __local_spawn_pos_dir_b0+1
   LDA   __local_spawn_pos_dir_b0
   STA   DPTR
   LDA   __local_spawn_pos_dir_b1
   STA   DPTR+1
   LDA   #$00
   PHA
   LDY   #$03
   LDA   (FP),Y
   TAY
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   LDY   #$03
   LDA   (FP),Y
   TAX
   LDY   rescue_floor,X
   LDA   floor_thresh,Y
   SEC
   SBC   #$07
   STA   __local_spawn_pos_dir_b2
   LDA   #<entity_floor_pos
   STA   __local_spawn_pos_dir_b0
   LDA   #>entity_floor_pos
   STA   __local_spawn_pos_dir_b0+1
   LDA   __local_spawn_pos_dir_b0
   STA   DPTR
   LDA   __local_spawn_pos_dir_b1
   STA   DPTR+1
   LDA   __local_spawn_pos_dir_b2
   PHA
   LDY   #$03
   LDA   (FP),Y
   TAY
   PLA
   STA   (__local_spawn_pos_dir_b0),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$03
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
