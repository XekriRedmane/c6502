SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_snd_delay_up__pitch	EQU	$80
__zpabi_snd_delay_up__clicks	EQU	$81

; @zp-link-meta-begin
; def snd_delay_up params=__zpabi_snd_delay_up__pitch,__zpabi_snd_delay_up__clicks locals=__local_snd_delay_up__0,__local_snd_delay_up__pitch indirect=false in_cycle=false param_regs=A,X
; item snd_delay_up section=.text local=false zero=false
; runtime hargs_size=32
; @zp-link-meta-end

snd_delay_up:
   SUBROUTINE

   TAY
   LDA   sfx_click_ptr
   STA   DPTR
   LDA   sfx_click_ptr+1
   STA   DPTR+1
   TYA
.loop@0_start:
   TAY
   CLC
   ADC   #$01
.loop@1_continue:
   DEY
   BNE   .loop@1_continue
   CMP   (DPTR),Y
   DEX
   BNE   .loop@0_start
   RTS
