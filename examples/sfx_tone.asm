__zpabi_sfx_tone_p0	EQU	$80
__zpabi_sfx_tone_p1	EQU	$81
__local_sfx_tone_b0	EQU	$82
__local_sfx_tone_b1	EQU	$83
__local_sfx_tone_b2	EQU	$84

; @zp-link-meta-begin
; def sfx_tone param_bytes=2 local_bytes=3 indirect=false in_cycle=false
; @zp-link-meta-end

sfx_tone:
   SUBROUTINE

.sfx_tone@asm_ssa_preheader@0:
.sfx_tone@ssa_preheader@0:
   LDA   __zpabi_sfx_tone_p1
   STA   __local_sfx_tone_b2
   LDA   sfx_click_ptr
   STA   DPTR
   LDA   sfx_click_ptr+1
   STA   DPTR+1
.loop@0_start:
   LDA   __zpabi_sfx_tone_p0
   STA   __local_sfx_tone_b1
.loop@1_continue:
   LDA   __local_sfx_tone_b1
   SEC
   SBC   #$01
   STA   __local_sfx_tone_b0
   LDA   __local_sfx_tone_b0
   STA   __local_sfx_tone_b1
   BEQ   .loop@1_break
.sfx_tone@asm_ssa_block@0:
   JMP   .loop@1_continue
.loop@1_break:
   LDY   #$00
   LDA   (DPTR),Y
.loop@0_continue:
   DEC   __local_sfx_tone_b2
   BNE   .sfx_tone@asm_ssa_split@0
.sfx_tone@asm_ssa_block@1:
   RTS
.sfx_tone@asm_ssa_split@0:
   JMP   .loop@0_start
