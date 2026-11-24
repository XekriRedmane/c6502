SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__zpabi_copysign__x_0	EQU	$80
__local_main__1	EQU	$81
__zpabi_copysign__x_1	EQU	$81
__local_main__2	EQU	$82
__zpabi_copysign__x_2	EQU	$82
__local_main__3	EQU	$83
__zpabi_copysign__x_3	EQU	$83
__local_main__4	EQU	$84
__zpabi_copysign__x_4	EQU	$84
__local_main__5	EQU	$85
__zpabi_copysign__x_5	EQU	$85
__local_main__6	EQU	$86
__zpabi_copysign__x_6	EQU	$86
__local_main__7	EQU	$87
__zpabi_copysign__x_7	EQU	$87
__zpabi_copysign__y_0	EQU	$88
__local_target__result_7	EQU	$89
__zpabi_copysign__y_1	EQU	$89
__zpabi_copysign__y_2	EQU	$8A
__zpabi_copysign__y_3	EQU	$8B
__zpabi_copysign__y_4	EQU	$8C
__zpabi_copysign__y_5	EQU	$8D
__zpabi_copysign__y_6	EQU	$8E
__zpabi_copysign__y_7	EQU	$8F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1 locals=__local_target__result_7,__local_target__result_6,__local_target__result_5,__local_target__result_4,__local_target__result_3,__local_target__result_2,__local_target__result_1,__local_target__result_0 indirect=false in_cycle=false param_regs=A,X
; ext copysign params=__zpabi_copysign__x_0,__zpabi_copysign__x_1,__zpabi_copysign__x_2,__zpabi_copysign__x_3,__zpabi_copysign__x_4,__zpabi_copysign__x_5,__zpabi_copysign__x_6,__zpabi_copysign__x_7,__zpabi_copysign__y_0,__zpabi_copysign__y_1,__zpabi_copysign__y_2,__zpabi_copysign__y_3,__zpabi_copysign__y_4,__zpabi_copysign__y_5,__zpabi_copysign__y_6,__zpabi_copysign__y_7
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target:
   SUBROUTINE

   STX   __local_target__result_7
   ORA   __local_target__result_7
   BEQ   .target@asm_ssa_split@0
   LDA   #$00
   LDY   #$80
   STY   __local_target__result_7
.if_end@0:
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STA   HARGS+22
   LDA   __local_target__result_7
   STA   HARGS+23
   RTS
.target@asm_ssa_split@0:
   LDA   #$00
   STA   __local_target__result_7
   JMP   .if_end@0

main:
   SUBROUTINE

   LDX   #$00
   TXA
   JSR   target
   JSR   main@run1
   STA   __local_main__7
   LDA   HARGS+17
   STA   __local_main__6
   LDA   HARGS+18
   STA   __local_main__5
   LDA   HARGS+19
   STA   __local_main__4
   LDA   HARGS+20
   STA   __local_main__3
   LDA   HARGS+21
   STA   __local_main__2
   LDA   HARGS+22
   STA   __local_main__1
   LDA   HARGS+23
   STA   __local_main__0
   LDX   #$00
   LDA   #$01
   JSR   target
   JSR   main@run1
   STA   HARGS
   LDA   HARGS+17
   STA   HARGS+1
   LDA   HARGS+18
   STA   HARGS+2
   LDA   HARGS+19
   STA   HARGS+3
   LDA   HARGS+20
   STA   HARGS+4
   LDA   HARGS+21
   STA   HARGS+5
   LDA   HARGS+22
   STA   HARGS+6
   LDA   HARGS+23
   STA   HARGS+7
   LDA   __local_main__7
   STA   HARGS+8
   LDA   __local_main__6
   STA   HARGS+9
   LDA   __local_main__5
   STA   HARGS+10
   LDA   __local_main__4
   STA   HARGS+11
   LDA   __local_main__3
   STA   HARGS+12
   LDA   __local_main__2
   STA   HARGS+13
   LDA   __local_main__1
   STA   HARGS+14
   LDA   __local_main__0
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   LDX   #$00
   RTS

main@run1:
   SUBROUTINE

   LDA   HARGS+16
   STA   HARGS+8
   LDA   HARGS+17
   STA   HARGS+9
   LDA   HARGS+18
   JSR   main@run0
   LDA   HARGS+16
   RTS

main@run0:
   SUBROUTINE

   STA   HARGS+10
   LDA   HARGS+19
   STA   HARGS+11
   LDA   HARGS+20
   STA   HARGS+12
   LDA   HARGS+21
   STA   HARGS+13
   LDA   HARGS+22
   STA   HARGS+14
   LDA   HARGS+23
   STA   HARGS+15
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$F0
   STA   HARGS+6
   LDA   #$3F
   STA   HARGS+7
   JMP   ddiv
