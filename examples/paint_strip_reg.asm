SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_paint_strip_reg__x_pixel	EQU	$80
__zpabi_paint_strip_reg__color	EQU	$81
__local_main__0	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def paint_strip_reg params=__zpabi_paint_strip_reg__x_pixel,__zpabi_paint_strip_reg__color locals= indirect=false in_cycle=false param_regs=A,X
; call main -> paint_strip_reg
; @zp-link-meta-end

paint_strip_reg:
   SUBROUTINE

   STA   __zpabi_paint_strip_reg__x_pixel
   STX   __zpabi_paint_strip_reg__color
   LDX   __zpabi_paint_strip_reg__x_pixel
   LDA   __zpabi_paint_strip_reg__color
   STA   hud_buf,X
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
