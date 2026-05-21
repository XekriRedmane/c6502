__zpabi_beam_target_draw__page_flag	EQU	$80
__zpabi_draw_sprite__width	EQU	$81
__zpabi_draw_sprite__height	EQU	$82
__zpabi_draw_sprite__sprite_x	EQU	$83
__zpabi_draw_sprite__sprite_y	EQU	$84
__zpabi_draw_sprite__tile_src_0	EQU	$85
__zpabi_draw_sprite__tile_src_1	EQU	$86
__zpabi_draw_sprite__page_flag	EQU	$87
__local_beam_target_draw__state	EQU	$88
__local_beam_target_draw__0	EQU	$89

; @zp-link-meta-begin
; def beam_target_draw params=__zpabi_beam_target_draw__page_flag locals=__local_beam_target_draw__state,__local_beam_target_draw__0 indirect=false in_cycle=false
; ext draw_sprite params=__zpabi_draw_sprite__width,__zpabi_draw_sprite__height,__zpabi_draw_sprite__sprite_x,__zpabi_draw_sprite__sprite_y,__zpabi_draw_sprite__tile_src_0,__zpabi_draw_sprite__tile_src_1,__zpabi_draw_sprite__page_flag
; ext score_add params=
; call beam_target_draw -> draw_sprite
; call beam_target_draw -> score_add
; @zp-link-meta-end

beam_target_draw:
   SUBROUTINE

   LDA   beam_state
   BNE   .if_end@0
   RTS
.if_end@0:
   AND   #$0F
   TAX
   LDA   floor_y_table,X
   SEC
   SBC   beam_y
   STA   __local_beam_target_draw__state
   LDA   #$00
   SBC   #$00
   LDA   __local_beam_target_draw__state
   BNE   .if_end@1
   RTS
.if_end@1:
   CMP   #$05
   BCC   .cond_end@3
   LDA   #$04
   STA   __local_beam_target_draw__state
.cond_end@3:
   LDA   #$02
   STA   __zpabi_draw_sprite__width
   LDA   __local_beam_target_draw__state
   STA   __zpabi_draw_sprite__height
   LDA   #$24
   STA   __zpabi_draw_sprite__sprite_x
   LDA   beam_y
   STA   __zpabi_draw_sprite__sprite_y
   LDA   #<beam_spr
   STA   __zpabi_draw_sprite__tile_src_0
   LDA   #>beam_spr
   STA   __zpabi_draw_sprite__tile_src_1
   LDA   __zpabi_beam_target_draw__page_flag
   STA   __zpabi_draw_sprite__page_flag
   JSR   draw_sprite
   LDY   #$03
.loop@0_start:
   LDA   enemy_flag,Y
   BEQ   .loop@0_continue
   LDX   enemy_col,Y
   LDA   proj_screen_col,X
   STA   __local_beam_target_draw__state
   LDA   #$25
   CMP   __local_beam_target_draw__state
   BCC   .loop@0_continue
   LDA   #$23
   CMP   __local_beam_target_draw__state
   BCS   .loop@0_continue
   LDA   enemy_y,Y
   STA   __local_beam_target_draw__0
   LDA   beam_y
   CMP   __local_beam_target_draw__0
   BCS   .loop@0_continue
   CLC
   ADC   #$03
   CMP   __local_beam_target_draw__0
   BCC   .loop@0_continue
   LDA   #$00
   STA   enemy_flag,Y
   LDA   #$00
   STA   beam_state
   LDA   #$0A
   STA   beam_snd_ctr
   LDA   #$00
   STA   score_add_lo
   LDA   #$01
   STA   score_add_hi
   JMP   score_add
.loop@0_continue:
   DEY
   BPL   .loop@0_start
   RTS

floor_y_table:
   DC.B  $00, $43, $6B, $93, $BB

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
