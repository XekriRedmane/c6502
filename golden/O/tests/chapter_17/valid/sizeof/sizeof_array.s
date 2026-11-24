SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; def sizeof_adjusted_param params=__zpabi_sizeof_adjusted_param__arr_0,__zpabi_sizeof_adjusted_param__arr_1 locals= indirect=false in_cycle=false param_regs=A,X
; item sizeof_adjusted_param section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

sizeof_adjusted_param:
   SUBROUTINE

   LDA   #$02
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS
