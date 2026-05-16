__zpabi_floor_enemy_advance_p0	EQU	$80
__zpabi_floor_enemy_advance_p1	EQU	$81
__zpabi_snd_delay_down_p0	EQU	$82
__zpabi_snd_delay_down_p1	EQU	$83
__local_floor_enemy_advance_b0	EQU	$84
__local_floor_enemy_advance_b1	EQU	$85
__local_floor_enemy_advance_b2	EQU	$86
__local_floor_enemy_advance_b3	EQU	$87

; @zp-link-meta-begin
; def floor_enemy_advance param_bytes=2 local_bytes=4 indirect=false in_cycle=false
; ext snd_delay_down param_bytes=2
; call floor_enemy_advance -> snd_delay_down
; @zp-link-meta-end

floor_enemy_advance:
   SUBROUTINE

.floor_enemy_advance@asm_ssa_preheader@0:
.floor_enemy_advance@ssa_block@0:
   LDA   #$03
   STA   __local_floor_enemy_advance_b3
.loop@0_start:
   LDA   __local_floor_enemy_advance_b3
   BPL   .lb_skip@3
   JMP   .loop@0_break
.lb_skip@3:
.floor_enemy_advance@asm_ssa_block@0:
   LDX   __local_floor_enemy_advance_b3
   LDA   enemy_flag,X
   STA   __local_floor_enemy_advance_b0
   BEQ   .lb_skip@2
   JMP   .if_else@1
.lb_skip@2:
.floor_enemy_advance@asm_ssa_block@1:
   LDA   jump_flag
   BMI   .lb_skip@1
   JMP   .if_end@2
.lb_skip@1:
.floor_enemy_advance@asm_ssa_block@2:
   LDA   #$00
   STA   jump_flag
   LDX   __zpabi_floor_enemy_advance_p1
   LDA   floor_enemy_spawn_sched,X
   STA   __local_floor_enemy_advance_b0
   BPL   .lb_skip@0
   JMP   .if_end@3
.lb_skip@0:
.floor_enemy_advance@asm_ssa_block@3:
   LDA   #$20
   STA   __zpabi_snd_delay_down_p0
   LDA   #$0A
   STA   __zpabi_snd_delay_down_p1
   JSR   snd_delay_down
   LDA   smc_move_left_op
   STA   DPTR
   LDA   smc_move_left_op+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   CMP   #$20
   BNE   .if_else@5
.floor_enemy_advance@asm_ssa_block@4:
   LDA   #<enemy_flag
   STA   __local_floor_enemy_advance_b0
   LDA   #>enemy_flag
   STA   __local_floor_enemy_advance_b0+1
   LDA   #$FF
   LDY   __local_floor_enemy_advance_b3
   STA   (__local_floor_enemy_advance_b0),Y
   LDA   #<enemy_col
   STA   __local_floor_enemy_advance_b0
   LDA   #>enemy_col
   STA   __local_floor_enemy_advance_b0+1
   LDA   #$3E
   LDY   __local_floor_enemy_advance_b3
   STA   (__local_floor_enemy_advance_b0),Y
   JMP   .if_end@4
.if_else@5:
   LDA   #<enemy_flag
   STA   __local_floor_enemy_advance_b0
   LDA   #>enemy_flag
   STA   __local_floor_enemy_advance_b0+1
   LDA   #$01
   LDY   __local_floor_enemy_advance_b3
   STA   (__local_floor_enemy_advance_b0),Y
   LDA   #<enemy_col
   STA   __local_floor_enemy_advance_b0
   LDA   #>enemy_col
   STA   __local_floor_enemy_advance_b0+1
   LDA   #$4A
   LDY   __local_floor_enemy_advance_b3
   STA   (__local_floor_enemy_advance_b0),Y
.if_end@4:
   LDA   __zpabi_floor_enemy_advance_p1
   CLC
   ADC   #$09
   STA   __local_floor_enemy_advance_b2
   LDA   #<enemy_y
   STA   __local_floor_enemy_advance_b0
   LDA   #>enemy_y
   STA   __local_floor_enemy_advance_b0+1
   LDA   __local_floor_enemy_advance_b0
   STA   DPTR
   LDA   __local_floor_enemy_advance_b1
   STA   DPTR+1
   LDA   __local_floor_enemy_advance_b2
   LDY   __local_floor_enemy_advance_b3
   STA   (__local_floor_enemy_advance_b0),Y
.if_end@3:
.if_end@2:
   JMP   .if_end@0
.if_else@1:
   LDA   __local_floor_enemy_advance_b0
   BPL   .if_else@7
.floor_enemy_advance@asm_ssa_block@5:
   LDA   __zpabi_floor_enemy_advance_p0
   BNE   .if_else@9
.floor_enemy_advance@ssa_block@7:
   LDA   #$07
   STA   __local_floor_enemy_advance_b1
   JMP   .if_end@8
.if_else@9:
   LDA   __zpabi_floor_enemy_advance_p0
   BPL   .if_else@11
.floor_enemy_advance@ssa_block@8:
   LDA   #$05
   STA   __local_floor_enemy_advance_b1
   JMP   .if_end@10
.if_else@11:
   LDA   #$09
   STA   __local_floor_enemy_advance_b1
.if_end@10:
.if_end@8:
   LDX   __local_floor_enemy_advance_b3
   LDA   enemy_col,X
   SEC
   SBC   __local_floor_enemy_advance_b1
   STA   __local_floor_enemy_advance_b2
   LDA   #<enemy_col
   STA   __local_floor_enemy_advance_b0
   LDA   #>enemy_col
   STA   __local_floor_enemy_advance_b0+1
   LDA   __local_floor_enemy_advance_b0
   STA   DPTR
   LDA   __local_floor_enemy_advance_b1
   STA   DPTR+1
   LDA   __local_floor_enemy_advance_b2
   LDY   __local_floor_enemy_advance_b3
   STA   (__local_floor_enemy_advance_b0),Y
   LDA   __local_floor_enemy_advance_b2
   SEC
   SBC   #$02
   CMP   #$8D
   BCC   .if_end@12
.floor_enemy_advance@asm_ssa_block@6:
   LDA   #<enemy_flag
   STA   __local_floor_enemy_advance_b0
   LDA   #>enemy_flag
   STA   __local_floor_enemy_advance_b0+1
   LDA   __local_floor_enemy_advance_b0
   STA   DPTR
   LDA   __local_floor_enemy_advance_b1
   STA   DPTR+1
   LDA   #$00
   LDY   __local_floor_enemy_advance_b3
   STA   (__local_floor_enemy_advance_b0),Y
.if_end@12:
   JMP   .if_end@6
.if_else@7:
   LDA   __zpabi_floor_enemy_advance_p0
   BNE   .if_else@14
.floor_enemy_advance@ssa_block@10:
   LDA   #$07
   STA   __local_floor_enemy_advance_b1
   JMP   .if_end@13
.if_else@14:
   LDA   __zpabi_floor_enemy_advance_p0
   BPL   .if_else@16
.floor_enemy_advance@ssa_block@11:
   LDA   #$09
   STA   __local_floor_enemy_advance_b1
   JMP   .if_end@15
.if_else@16:
   LDA   #$05
   STA   __local_floor_enemy_advance_b1
.if_end@15:
.if_end@13:
   LDX   __local_floor_enemy_advance_b3
   LDA   enemy_col,X
   CLC
   ADC   __local_floor_enemy_advance_b1
   STA   __local_floor_enemy_advance_b2
   LDA   #<enemy_col
   STA   __local_floor_enemy_advance_b0
   LDA   #>enemy_col
   STA   __local_floor_enemy_advance_b0+1
   LDA   __local_floor_enemy_advance_b0
   STA   DPTR
   LDA   __local_floor_enemy_advance_b1
   STA   DPTR+1
   LDA   __local_floor_enemy_advance_b2
   LDY   __local_floor_enemy_advance_b3
   STA   (__local_floor_enemy_advance_b0),Y
   LDA   __local_floor_enemy_advance_b2
   CMP   #$8F
   BCC   .if_end@17
.floor_enemy_advance@asm_ssa_block@7:
   LDA   #<enemy_flag
   STA   __local_floor_enemy_advance_b0
   LDA   #>enemy_flag
   STA   __local_floor_enemy_advance_b0+1
   LDA   __local_floor_enemy_advance_b0
   STA   DPTR
   LDA   __local_floor_enemy_advance_b1
   STA   DPTR+1
   LDA   #$00
   LDY   __local_floor_enemy_advance_b3
   STA   (__local_floor_enemy_advance_b0),Y
.if_end@17:
.if_end@6:
.if_end@0:
.loop@0_continue:
   DEC   __local_floor_enemy_advance_b3
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   STA   jump_flag
   RTS

enemy_flag:
   DS.B  4

enemy_col:
   DS.B  4

enemy_y:
   DS.B  4

jump_flag:
   DS.B  1

floor_enemy_spawn_sched:
   DC.B  $25, $26, $26, $26, $26, $27, $27, $27, $28, $28, $28, $28, $29, $29, $29, $2A
   DC.B  $2A, $2A, $2A, $2B, $2B, $2B, $2C, $2C, $2C, $2C, $2D, $2D, $2D, $2E, $2E, $2E
   DC.B  $2E, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00
