SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_long_to_double__l_0	EQU	$80
__zpabi_long_to_double__l_1	EQU	$81
__zpabi_long_to_double__l_2	EQU	$82
__zpabi_long_to_double__l_3	EQU	$83
__zpabi_long_to_double__l_4	EQU	$84
__zpabi_long_to_double__l_5	EQU	$85
__zpabi_long_to_double__l_6	EQU	$86
__zpabi_long_to_double__l_7	EQU	$87

; @zp-link-meta-begin
; def int_to_double params=__zpabi_int_to_double__i_0,__zpabi_int_to_double__i_1 locals=__local_int_to_double__i_1 indirect=false in_cycle=false param_regs=A,X
; def long_to_double params=__zpabi_long_to_double__l_0,__zpabi_long_to_double__l_1,__zpabi_long_to_double__l_2,__zpabi_long_to_double__l_3,__zpabi_long_to_double__l_4,__zpabi_long_to_double__l_5,__zpabi_long_to_double__l_6,__zpabi_long_to_double__l_7 locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; call main -> int_to_double
; call main -> long_to_double
; item int_to_double section=.text local=false zero=false
; item long_to_double section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

int_to_double:
   SUBROUTINE

   STA   HARGS
   STX   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   STA   HARGS+16
   LDA   HARGS+3
   STA   HARGS+17
   LDA   HARGS+4
   STA   HARGS+18
   LDA   HARGS+5
   STA   HARGS+19
   LDA   HARGS+6
   STA   HARGS+20
   LDA   HARGS+7
   STA   HARGS+21
   LDA   HARGS+8
   STA   HARGS+22
   LDA   HARGS+9
   STA   HARGS+23
   RTS

long_to_double:
   SUBROUTINE

   LDA   __zpabi_long_to_double__l_0
   STA   HARGS
   LDA   __zpabi_long_to_double__l_1
   STA   HARGS+1
   LDA   __zpabi_long_to_double__l_2
   STA   HARGS+2
   LDA   __zpabi_long_to_double__l_3
   STA   HARGS+3
   LDA   __zpabi_long_to_double__l_4
   STA   HARGS+4
   LDA   __zpabi_long_to_double__l_5
   STA   HARGS+5
   LDA   __zpabi_long_to_double__l_6
   STA   HARGS+6
   LDA   __zpabi_long_to_double__l_7
   STA   HARGS+7
   JSR   ll2d
   LDA   HARGS+8
   STA   HARGS+16
   LDA   HARGS+9
   STA   HARGS+17
   LDA   HARGS+10
   STA   HARGS+18
   LDA   HARGS+11
   STA   HARGS+19
   LDA   HARGS+12
   STA   HARGS+20
   LDA   HARGS+13
   STA   HARGS+21
   LDA   HARGS+14
   STA   HARGS+22
   LDA   HARGS+15
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   LDX   #$FF
   LDA   #$9C
   JSR   int_to_double
   LDA   HARGS+23
   CMP   #$C0
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   CMP   #$59
   BNE   .jcmp_skip@0
   LDA   HARGS+21
   BNE   .jcmp_skip@0
   LDA   HARGS+20
   BNE   .jcmp_skip@0
   LDA   HARGS+19
   BNE   .jcmp_skip@0
   LDA   HARGS+18
   BNE   .jcmp_skip@0
   LDA   HARGS+17
   BNE   .jcmp_skip@0
   LDA   HARGS+16
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$00
   STA   __zpabi_long_to_double__l_0
   LDA   #$6C
   STA   __zpabi_long_to_double__l_1
   LDA   #$CA
   STA   __zpabi_long_to_double__l_2
   LDA   #$88
   STA   __zpabi_long_to_double__l_3
   LDA   #$FF
   STA   __zpabi_long_to_double__l_4
   STA   __zpabi_long_to_double__l_5
   STA   __zpabi_long_to_double__l_6
   STA   __zpabi_long_to_double__l_7
   JSR   long_to_double
   LDA   HARGS+23
   CMP   #$C1
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   CMP   #$DD
   BNE   .jcmp_skip@1
   LDA   HARGS+21
   CMP   #$CD
   BNE   .jcmp_skip@1
   LDA   HARGS+20
   CMP   #$65
   BNE   .jcmp_skip@1
   LDA   HARGS+19
   BNE   .jcmp_skip@1
   LDA   HARGS+18
   BNE   .jcmp_skip@1
   LDA   HARGS+17
   BNE   .jcmp_skip@1
   LDA   HARGS+16
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS
