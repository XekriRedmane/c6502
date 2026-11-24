SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_sfx_tone__y	EQU	$81
__local_sfx_tone__pitch	EQU	$83

; @zp-link-meta-begin
; def sfx_tone params=__zpabi_sfx_tone__pitch,__zpabi_sfx_tone__duration locals=__local_sfx_tone__0,__local_sfx_tone__y,__local_sfx_tone__duration,__local_sfx_tone__pitch indirect=false in_cycle=false param_regs=A,X
; item sfx_tone section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

sfx_tone:
   SUBROUTINE

   STA   __local_sfx_tone__pitch
   LDA   sfx_click_ptr
   STA   DPTR
   LDA   sfx_click_ptr+1
   STA   DPTR+1
.loop@0_start:
   LDA   __local_sfx_tone__pitch
   STA   __local_sfx_tone__y
.loop@1_continue:
   LDA   __local_sfx_tone__y
   SEC
   SBC   #$01
   STA   __local_sfx_tone__y
   BNE   .loop@1_continue
   LDY   #$00
   CMP   (DPTR),Y
   DEX
   BNE   .loop@0_start
   RTS
