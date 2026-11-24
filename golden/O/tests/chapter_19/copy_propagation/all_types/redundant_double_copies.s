SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__flag2_0	EQU	$80
__zpabi_target__flag2_1	EQU	$81
__zpabi_target__y_0	EQU	$82
__zpabi_target__y_1	EQU	$83
__zpabi_target__y_2	EQU	$84
__zpabi_target__y_3	EQU	$85
__zpabi_target__y_4	EQU	$86
__zpabi_target__y_5	EQU	$87
__zpabi_target__y_6	EQU	$88
__zpabi_target__y_7	EQU	$89

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1,__zpabi_target__flag2_0,__zpabi_target__flag2_1,__zpabi_target__y_0,__zpabi_target__y_1,__zpabi_target__y_2,__zpabi_target__y_3,__zpabi_target__y_4,__zpabi_target__y_5,__zpabi_target__y_6,__zpabi_target__y_7 locals= indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   __zpabi_target__y_0
   STA   HARGS
   STA   HARGS+8
   LDA   __zpabi_target__y_1
   STA   HARGS+1
   STA   HARGS+9
   LDA   __zpabi_target__y_2
   STA   HARGS+2
   STA   HARGS+10
   LDA   __zpabi_target__y_3
   STA   HARGS+3
   STA   HARGS+11
   LDA   __zpabi_target__y_4
   STA   HARGS+4
   STA   HARGS+12
   LDA   __zpabi_target__y_5
   STA   HARGS+5
   STA   HARGS+13
   LDA   __zpabi_target__y_6
   STA   HARGS+6
   STA   HARGS+14
   LDA   __zpabi_target__y_7
   STA   HARGS+7
   STA   HARGS+15
   JMP   dadd

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_target__flag2_0
   LDA   #$00
   STA   __zpabi_target__flag2_1
   STA   __zpabi_target__y_0
   STA   __zpabi_target__y_1
   STA   __zpabi_target__y_2
   STA   __zpabi_target__y_3
   STA   __zpabi_target__y_4
   STA   __zpabi_target__y_5
   LDA   #$24
   STA   __zpabi_target__y_6
   LDA   #$40
   STA   __zpabi_target__y_7
   LDX   #$00
   TXA
   JSR   target
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   CMP   #$34
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
   BEQ   .if_end@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS
