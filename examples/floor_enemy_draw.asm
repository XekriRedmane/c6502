__zpabi_floor_enemy_draw_p0	EQU	$80
__zpabi_draw_sprite_p0	EQU	$81
__zpabi_draw_sprite_p1	EQU	$82
__zpabi_draw_sprite_p2	EQU	$83
__zpabi_draw_sprite_p3	EQU	$84
__zpabi_draw_sprite_p4	EQU	$85
__zpabi_draw_sprite_p5	EQU	$86
__zpabi_draw_sprite_p6	EQU	$87
__local_floor_enemy_draw_b0	EQU	$88
__local_floor_enemy_draw_b1	EQU	$89
__local_floor_enemy_draw_b2	EQU	$8A
__local_floor_enemy_draw_b3	EQU	$8B
__local_floor_enemy_draw_b4	EQU	$8C

; @zp-link-meta-begin
; def floor_enemy_draw param_bytes=1 local_bytes=5 indirect=false in_cycle=false
; ext draw_sprite param_bytes=7
; call floor_enemy_draw -> draw_sprite
; @zp-link-meta-end

floor_enemy_draw:
   SUBROUTINE

.floor_enemy_draw@asm_ssa_preheader@0:
.floor_enemy_draw@ssa_block@0:
   LDX   #$03
.loop@0_start:
   LDA   enemy_flag,X
   BNE   .if_end@0
.floor_enemy_draw@asm_ssa_block@0:
   JMP   .loop@0_continue
.if_end@0:
   LDY   enemy_col,X
   LDA   proj_screen_col,Y
   STA   __local_floor_enemy_draw_b3
   LDA   proj_frame_idx,Y
   TAY
   TXA
   CMP   #$00
   BEQ   .dispatch@0@case@0
.floor_enemy_draw@asm_ssa_block@1:
   CMP   #$01
   BEQ   .dispatch@0@case@1
.floor_enemy_draw@asm_ssa_block@2:
   CMP   #$02
   BEQ   .dispatch@0@case@2
.floor_enemy_draw@asm_ssa_block@3:
   LDA   floor_enemy_spr_s3_lo,Y
   STA   __local_floor_enemy_draw_b2
   JMP   .dispatch@0@end
.dispatch@0@case@0:
   LDA   floor_enemy_spr_s0_lo,Y
   STA   __local_floor_enemy_draw_b2
   JMP   .dispatch@0@end
.dispatch@0@case@1:
   LDA   floor_enemy_spr_s1_lo,Y
   STA   __local_floor_enemy_draw_b2
   JMP   .dispatch@0@end
.dispatch@0@case@2:
   LDA   floor_enemy_spr_s2_lo,Y
   STA   __local_floor_enemy_draw_b2
   JMP   .dispatch@0@end
.dispatch@0@end:
   TXA
   CMP   #$00
   BEQ   .dispatch@1@case@0
.floor_enemy_draw@asm_ssa_block@4:
   CMP   #$01
   BEQ   .dispatch@1@case@1
.floor_enemy_draw@asm_ssa_block@5:
   CMP   #$02
   BEQ   .dispatch@1@case@2
.floor_enemy_draw@asm_ssa_block@6:
   LDA   floor_enemy_spr_s3_hi,Y
   STA   __local_floor_enemy_draw_b0
   JMP   .dispatch@1@end
.dispatch@1@case@0:
   LDA   floor_enemy_spr_s0_hi,Y
   STA   __local_floor_enemy_draw_b0
   JMP   .dispatch@1@end
.dispatch@1@case@1:
   LDA   floor_enemy_spr_s1_hi,Y
   STA   __local_floor_enemy_draw_b0
   JMP   .dispatch@1@end
.dispatch@1@case@2:
   LDA   floor_enemy_spr_s2_hi,Y
   STA   __local_floor_enemy_draw_b0
   JMP   .dispatch@1@end
.dispatch@1@end:
   LDA   __local_floor_enemy_draw_b0
   STA   __local_floor_enemy_draw_b1
   LDA   enemy_y,X
   STA   __local_floor_enemy_draw_b0
   LDA   #$01
   STA   __zpabi_draw_sprite_p0
   LDA   #$05
   STA   __zpabi_draw_sprite_p1
   LDA   __local_floor_enemy_draw_b3
   STA   __zpabi_draw_sprite_p2
   LDA   __local_floor_enemy_draw_b0
   STA   __zpabi_draw_sprite_p3
   LDA   __local_floor_enemy_draw_b2
   STA   __zpabi_draw_sprite_p4
   LDA   __local_floor_enemy_draw_b1
   STA   __zpabi_draw_sprite_p5
   LDA   __zpabi_floor_enemy_draw_p0
   STA   __zpabi_draw_sprite_p6
   STX   __local_floor_enemy_draw_b4
   JSR   draw_sprite
   LDX   __local_floor_enemy_draw_b4
.loop@0_continue:
   DEX
   BPL   .floor_enemy_draw@asm_ssa_split@0
.floor_enemy_draw@asm_ssa_block@7:
   RTS
.floor_enemy_draw@asm_ssa_split@0:
   JMP   .loop@0_start

proj_screen_col:
   DC.B  $00, $00, $00, $00, $01, $01, $01, $02, $02, $02, $02, $03, $03, $03, $04, $04
   DC.B  $04, $04, $05, $05, $05, $06, $06, $06, $06, $07, $07, $07, $08, $08, $08, $08
   DC.B  $09, $09, $09, $0A, $0A, $0A, $0A, $0B, $0B, $0B, $0C, $0C, $0C, $0C, $0D, $0D
   DC.B  $0D, $0E, $0E, $0E, $0E, $0F, $0F, $0F, $10, $10, $10, $10, $11, $11, $11, $12
   DC.B  $12, $12, $12, $13, $13, $13, $14, $14, $14, $14, $15, $15, $15, $16, $16, $16
   DC.B  $16, $17, $17, $17, $18, $18, $18, $18, $19, $19, $19, $1A, $1A, $1A, $1A, $1B
   DC.B  $1B, $1B, $1C, $1C, $1C, $1C, $1D, $1D, $1D, $1E, $1E, $1E, $1E, $1F, $1F, $1F
   DC.B  $20, $20, $20, $20, $21, $21, $21, $22, $22, $22, $22, $23, $23, $23, $24, $24
   DC.B  $24, $24, $25, $25

proj_frame_idx:
   DC.B  $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01
   DC.B  $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03
   DC.B  $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05
   DC.B  $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00
   DC.B  $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02
   DC.B  $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04
   DC.B  $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06
   DC.B  $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01
   DC.B  $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03
   DC.B  $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05
   DC.B  $06, $00, $01, $02, $03

floor_enemy_spr_s0_lo:
   DC.B  $C7, $D1, $DB, $E5, $EF, $F9, $03

floor_enemy_spr_s0_hi:
   DC.B  $8D, $8D, $8D, $8D, $8D, $8D, $8E

floor_enemy_spr_s1_lo:
   DC.B  $81, $8B, $95, $9F, $A9, $B3, $BD

floor_enemy_spr_s1_hi:
   DC.B  $8D, $8D, $8D, $8D, $8D, $8D, $8D

floor_enemy_spr_s2_lo:
   DC.B  $3B, $45, $4F, $59, $63, $6D, $77

floor_enemy_spr_s2_hi:
   DC.B  $8D, $8D, $8D, $8D, $8D, $8D, $8D

floor_enemy_spr_s3_lo:
   DC.B  $F5, $FF, $09, $13, $1D, $27, $31

floor_enemy_spr_s3_hi:
   DC.B  $8C, $8C, $8D, $8D, $8D, $8D, $8D
