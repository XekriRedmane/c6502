SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_paint_strip_reg__x_pixel	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def paint_strip_reg params=__zpabi_paint_strip_reg__x_pixel,__zpabi_paint_strip_reg__color locals=__local_paint_strip_reg__x_pixel indirect=false in_cycle=false param_regs=A,X
; call main -> paint_strip_reg
; item paint_strip_reg section=.text local=false zero=false
; item main section=.text local=false zero=false
; item hud_buf section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

paint_strip_reg:
   SUBROUTINE

   STA   __local_paint_strip_reg__x_pixel
   TXA
   TAY
   LDX   __local_paint_strip_reg__x_pixel
   TYA
   STA   hud_buf,X
   TYA
   RTS

main:
   SUBROUTINE

   LDX   #$7F
   LDA   #$03
   JSR   paint_strip_reg
   LDX   #$40
   LDA   #$07
   JSR   paint_strip_reg
   LDA   hud_buf+3
   CLC
   ADC   hud_buf+7
   STA   __local_main__0
   LDA   #$00
   ADC   #$00
   TAX
   LDA   __local_main__0
   RTS

hud_buf:
   DS.B  40
