__zpabi_sfx_tone__pitch	EQU	$80
__zpabi_sfx_tone__duration	EQU	$81
__local_sfx_tone__0	EQU	$82
__local_sfx_tone__y	EQU	$83
__local_sfx_tone__duration	EQU	$84

; @zp-link-meta-begin
; def sfx_tone params=__zpabi_sfx_tone__pitch,__zpabi_sfx_tone__duration locals=__local_sfx_tone__0,__local_sfx_tone__y,__local_sfx_tone__duration indirect=false in_cycle=false
; @zp-link-meta-end

sfx_tone:
   SUBROUTINE

.sfx_tone@asm_ssa_preheader@0:
.sfx_tone@ssa_preheader@0:
   LDA   __zpabi_sfx_tone__duration
   STA   __local_sfx_tone__duration
   LDA   sfx_click_ptr
   STA   DPTR
   LDA   sfx_click_ptr+1
   STA   DPTR+1
.loop@0_start:
   LDA   __zpabi_sfx_tone__pitch
   STA   __local_sfx_tone__y
.loop@1_continue:
   LDA   __local_sfx_tone__y
   SEC
   SBC   #$01
   STA   __local_sfx_tone__0
   LDA   __local_sfx_tone__0
   STA   __local_sfx_tone__y
   BEQ   .loop@1_break
.sfx_tone@asm_ssa_block@0:
   JMP   .loop@1_continue
.loop@1_break:
   LDY   #$00
   LDA   (DPTR),Y
.loop@0_continue:
   DEC   __local_sfx_tone__duration
   BNE   .sfx_tone@asm_ssa_split@0
.sfx_tone@asm_ssa_block@1:
   RTS
.sfx_tone@asm_ssa_split@0:
   JMP   .loop@0_start
