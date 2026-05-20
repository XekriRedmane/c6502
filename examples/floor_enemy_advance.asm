__zpabi_floor_enemy_advance__move_dir	EQU	$80
__zpabi_floor_enemy_advance__player_col	EQU	$81
__zpabi_snd_delay_down__pitch	EQU	$82
__zpabi_snd_delay_down__clicks	EQU	$83
__local_floor_enemy_advance__step	EQU	$85
__local_floor_enemy_advance__slot	EQU	$86

; @zp-link-meta-begin
; def floor_enemy_advance params=__zpabi_floor_enemy_advance__move_dir,__zpabi_floor_enemy_advance__player_col locals=__local_floor_enemy_advance__0,__local_floor_enemy_advance__step,__local_floor_enemy_advance__slot indirect=false in_cycle=false
; ext snd_delay_down params=__zpabi_snd_delay_down__pitch,__zpabi_snd_delay_down__clicks
; call floor_enemy_advance -> snd_delay_down
; @zp-link-meta-end

floor_enemy_advance:
   SUBROUTINE

   LDA   #$03
   STA   __local_floor_enemy_advance__slot
.loop@0_start:
   LDA   __local_floor_enemy_advance__slot
   BPL   .lb_skip@2
   JMP   .loop@0_break
.lb_skip@2:
   LDX   __local_floor_enemy_advance__slot
   LDA   enemy_flag,X
   BNE   .if_else@1
   LDA   jump_flag
   BMI   .lb_skip@1
   JMP   .if_end@0
.lb_skip@1:
   LDA   #$00
   STA   jump_flag
   LDX   __zpabi_floor_enemy_advance__player_col
   LDA   floor_enemy_spawn_sched,X
   BPL   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDA   #$20
   STA   __zpabi_snd_delay_down__pitch
   LDA   #$0A
   STA   __zpabi_snd_delay_down__clicks
   JSR   snd_delay_down
   LDA   smc_move_left_op
   STA   DPTR
   LDA   smc_move_left_op+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   CMP   #$20
   BNE   .if_else@5
   LDX   __local_floor_enemy_advance__slot
   LDA   #$FF
   STA   enemy_flag,X
   LDA   #$3E
   STA   enemy_col,X
   JMP   .if_end@4
.if_else@5:
   LDX   __local_floor_enemy_advance__slot
   LDA   #$01
   STA   enemy_flag,X
   LDA   #$4A
   STA   enemy_col,X
.if_end@4:
   LDA   __zpabi_floor_enemy_advance__player_col
   CLC
   ADC   #$09
   STA   enemy_y,X
   JMP   .if_end@0
.if_else@1:
   AND   #$80
   BEQ   .if_else@7
   LDA   __zpabi_floor_enemy_advance__move_dir
   BNE   .if_else@9
   LDA   #$07
   STA   __local_floor_enemy_advance__step
   JMP   .if_end@8
.if_else@9:
   AND   #$80
   BEQ   .if_else@11
   LDA   #$05
   STA   __local_floor_enemy_advance__step
   JMP   .if_end@10
.if_else@11:
   LDA   #$09
   STA   __local_floor_enemy_advance__step
.if_end@10:
.if_end@8:
   LDA   enemy_col,X
   SEC
   SBC   __local_floor_enemy_advance__step
   STA   enemy_col,X
   SEC
   SBC   #$02
   CMP   #$8D
   BCC   .if_end@6
   LDA   #$00
   STA   enemy_flag,X
   JMP   .if_end@6
.if_else@7:
   LDA   __zpabi_floor_enemy_advance__move_dir
   BNE   .if_else@14
   LDA   #$07
   STA   __local_floor_enemy_advance__step
   JMP   .if_end@13
.if_else@14:
   AND   #$80
   BEQ   .if_else@16
   LDA   #$09
   STA   __local_floor_enemy_advance__step
   JMP   .if_end@15
.if_else@16:
   LDA   #$05
   STA   __local_floor_enemy_advance__step
.if_end@15:
.if_end@13:
   LDA   enemy_col,X
   CLC
   ADC   __local_floor_enemy_advance__step
   STA   enemy_col,X
   CMP   #$8F
   BCC   .if_end@17
   LDA   #$00
   STA   enemy_flag,X
.if_end@17:
.if_end@6:
.if_end@0:
   DEC   __local_floor_enemy_advance__slot
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
