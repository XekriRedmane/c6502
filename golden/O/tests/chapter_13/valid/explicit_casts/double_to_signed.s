SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_double_to_int__d_0	EQU	$80
__zpabi_double_to_long__d_0	EQU	$80
__zpabi_double_to_int__d_1	EQU	$81
__zpabi_double_to_long__d_1	EQU	$81
__zpabi_double_to_int__d_2	EQU	$82
__zpabi_double_to_long__d_2	EQU	$82
__zpabi_double_to_int__d_3	EQU	$83
__zpabi_double_to_long__d_3	EQU	$83
__zpabi_double_to_int__d_4	EQU	$84
__zpabi_double_to_long__d_4	EQU	$84
__zpabi_double_to_int__d_5	EQU	$85
__zpabi_double_to_long__d_5	EQU	$85
__zpabi_double_to_int__d_6	EQU	$86
__zpabi_double_to_long__d_6	EQU	$86
__zpabi_double_to_int__d_7	EQU	$87
__zpabi_double_to_long__d_7	EQU	$87

; @zp-link-meta-begin
; def double_to_int params=__zpabi_double_to_int__d_0,__zpabi_double_to_int__d_1,__zpabi_double_to_int__d_2,__zpabi_double_to_int__d_3,__zpabi_double_to_int__d_4,__zpabi_double_to_int__d_5,__zpabi_double_to_int__d_6,__zpabi_double_to_int__d_7 locals=__local_double_to_int__0 indirect=false in_cycle=false
; def double_to_long params=__zpabi_double_to_long__d_0,__zpabi_double_to_long__d_1,__zpabi_double_to_long__d_2,__zpabi_double_to_long__d_3,__zpabi_double_to_long__d_4,__zpabi_double_to_long__d_5,__zpabi_double_to_long__d_6,__zpabi_double_to_long__d_7 locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; call main -> double_to_int
; call main -> double_to_long
; item double_to_int section=.text local=false zero=false
; item double_to_long section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

double_to_int:
   SUBROUTINE

   LDA   __zpabi_double_to_int__d_0
   STA   HARGS
   LDA   __zpabi_double_to_int__d_1
   STA   HARGS+1
   LDA   __zpabi_double_to_int__d_2
   STA   HARGS+2
   LDA   __zpabi_double_to_int__d_3
   STA   HARGS+3
   LDA   __zpabi_double_to_int__d_4
   STA   HARGS+4
   LDA   __zpabi_double_to_int__d_5
   STA   HARGS+5
   LDA   __zpabi_double_to_int__d_6
   STA   HARGS+6
   LDA   __zpabi_double_to_int__d_7
   STA   HARGS+7
   JSR   d2i
   LDA   HARGS+8
   LDX   HARGS+9
   RTS

double_to_long:
   SUBROUTINE

   LDA   __zpabi_double_to_long__d_0
   STA   HARGS
   LDA   __zpabi_double_to_long__d_1
   STA   HARGS+1
   LDA   __zpabi_double_to_long__d_2
   STA   HARGS+2
   LDA   __zpabi_double_to_long__d_3
   STA   HARGS+3
   LDA   __zpabi_double_to_long__d_4
   STA   HARGS+4
   LDA   __zpabi_double_to_long__d_5
   STA   HARGS+5
   LDA   __zpabi_double_to_long__d_6
   STA   HARGS+6
   LDA   __zpabi_double_to_long__d_7
   STA   HARGS+7
   JMP   d2l

main:
   SUBROUTINE

   LDA   #$33
   STA   __zpabi_double_to_long__d_0
   STA   __zpabi_double_to_long__d_1
   STA   __zpabi_double_to_long__d_2
   STA   __zpabi_double_to_long__d_3
   LDA   #$13
   STA   __zpabi_double_to_long__d_4
   LDA   #$88
   STA   __zpabi_double_to_long__d_5
   LDA   #$D3
   STA   __zpabi_double_to_long__d_6
   LDA   #$40
   STA   __zpabi_double_to_long__d_7
   JSR   double_to_long
   LDA   HARGS+11
   BNE   .jcmp_skip@0
   LDA   HARGS+10
   BNE   .jcmp_skip@0
   LDA   HARGS+9
   CMP   #$4E
   BNE   .jcmp_skip@0
   LDA   HARGS+8
   CMP   #$20
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$4E
   STA   __zpabi_double_to_int__d_0
   LDA   #$D1
   STA   __zpabi_double_to_int__d_1
   LDA   #$91
   STA   __zpabi_double_to_int__d_2
   LDA   #$5C
   STA   __zpabi_double_to_int__d_3
   LDA   #$FE
   STA   __zpabi_double_to_int__d_4
   LDA   #$3F
   STA   __zpabi_double_to_int__d_5
   LDA   #$59
   STA   __zpabi_double_to_int__d_6
   LDA   #$C0
   STA   __zpabi_double_to_int__d_7
   JSR   double_to_int
   CPX   #$FF
   BNE   .jcmp_skip@1
   CMP   #$9C
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS
