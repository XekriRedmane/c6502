__zpabi_paint_strip_reg__x_pixel	EQU	$80
__zpabi_paint_strip_reg__color	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def paint_strip_reg params=__zpabi_paint_strip_reg__x_pixel,__zpabi_paint_strip_reg__color locals= indirect=false in_cycle=false param_regs=X,Y
; call main -> paint_strip_reg
; @zp-link-meta-end

paint_strip_reg:
   SUBROUTINE

   STY   __zpabi_paint_strip_reg__color
   LDA   __zpabi_paint_strip_reg__color
   STA   hud_buf,X
   RTS

main:
   SUBROUTINE

   LDX   #$03
   LDY   #$7F
   JSR   paint_strip_reg
   LDX   #$07
   LDY   #$40
   JSR   paint_strip_reg
   LDX   #$03
   LDA   hud_buf,X
   STA   __local_main__1
   LDX   #$07
   LDA   hud_buf,X
   STA   __local_main__0
   LDA   __local_main__1
   CLC
   ADC   __local_main__0
   STA   HARGS
   LDA   #$00
   ADC   #$00
   STA   HARGS+1
   RTS

hud_buf:
   DS.B  40
