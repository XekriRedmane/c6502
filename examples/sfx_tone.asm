__zpabi_sfx_tone__pitch	EQU	$80
__zpabi_sfx_tone__duration	EQU	$81
__local_sfx_tone__0	EQU	$82
__local_sfx_tone__y	EQU	$83

; @zp-link-meta-begin
; def sfx_tone params=__zpabi_sfx_tone__pitch,__zpabi_sfx_tone__duration locals=__local_sfx_tone__0,__local_sfx_tone__y,__local_sfx_tone__duration indirect=false in_cycle=false
; @zp-link-meta-end

sfx_tone:
   SUBROUTINE

   LDX   __zpabi_sfx_tone__duration
   LDA   sfx_click_ptr
   STA   DPTR
   LDA   sfx_click_ptr+1
   STA   DPTR+1
.loop@0_start:
   LDA   __zpabi_sfx_tone__pitch
   STA   __local_sfx_tone__y
.loop@1_continue:
   LDA   __local_sfx_tone__y
   STA   __local_sfx_tone__0
   DEC   __local_sfx_tone__0
   LDA   __local_sfx_tone__0
   STA   __local_sfx_tone__y
   BNE   .loop@1_continue
   LDY   #$00
   CMP   (DPTR),Y
   DEX
   BNE   .loop@0_start
   RTS
