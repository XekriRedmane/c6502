__zpabi_snd_delay_up__pitch	EQU	$80
__zpabi_snd_delay_up__clicks	EQU	$81
__local_snd_delay_up__pitch	EQU	$83

; @zp-link-meta-begin
; def snd_delay_up params=__zpabi_snd_delay_up__pitch,__zpabi_snd_delay_up__clicks locals=__local_snd_delay_up__0,__local_snd_delay_up__pitch indirect=false in_cycle=false param_regs=A,X
; @zp-link-meta-end

snd_delay_up:
   SUBROUTINE

   STA   __local_snd_delay_up__pitch
   LDA   sfx_click_ptr
   STA   DPTR
   LDA   sfx_click_ptr+1
   STA   DPTR+1
.loop@0_start:
   INC   __local_snd_delay_up__pitch
.loop@1_continue:
   DEY
   BNE   .loop@1_continue
   CMP   (DPTR),Y
   DEX
   BNE   .loop@0_start
   RTS
