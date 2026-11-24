SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_double_isnan__d_0	EQU	$80
__zpabi_double_isnan__d_1	EQU	$81
__zpabi_double_isnan__d_2	EQU	$82
__zpabi_double_isnan__d_3	EQU	$83
__zpabi_double_isnan__d_4	EQU	$84
__zpabi_double_isnan__d_5	EQU	$85
__zpabi_double_isnan__d_6	EQU	$86
__zpabi_double_isnan__d_7	EQU	$87
__local_main__0	EQU	$88

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target_nan params= locals= indirect=false in_cycle=false
; ext double_isnan params=__zpabi_double_isnan__d_0,__zpabi_double_isnan__d_1,__zpabi_double_isnan__d_2,__zpabi_double_isnan__d_3,__zpabi_double_isnan__d_4,__zpabi_double_isnan__d_5,__zpabi_double_isnan__d_6,__zpabi_double_isnan__d_7
; call main -> double_isnan
; call main -> target_nan
; item target_nan section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target_nan:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STA   HARGS+21
   LDA   #$F8
   STA   HARGS+22
   LDA   #$FF
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   JSR   target_nan
   LDA   HARGS+16
   STA   __zpabi_double_isnan__d_0
   LDA   HARGS+17
   STA   __zpabi_double_isnan__d_1
   LDA   HARGS+18
   STA   __zpabi_double_isnan__d_2
   LDA   HARGS+19
   STA   __zpabi_double_isnan__d_3
   LDA   HARGS+20
   STA   __zpabi_double_isnan__d_4
   LDA   HARGS+21
   STA   __zpabi_double_isnan__d_5
   LDA   HARGS+22
   STA   __zpabi_double_isnan__d_6
   LDA   HARGS+23
   STA   __zpabi_double_isnan__d_7
   JSR   double_isnan
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS
