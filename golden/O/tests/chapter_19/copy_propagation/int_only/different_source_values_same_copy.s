SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__flag_0	EQU	$80
__zpabi_target__flag_1	EQU	$81
__zpabi_target__flag_2	EQU	$82
__zpabi_target__flag_3	EQU	$83
__zpabi_target__flag_4	EQU	$84
__zpabi_target__flag_5	EQU	$85
__zpabi_target__flag_6	EQU	$86
__zpabi_target__flag_7	EQU	$87
__zpabi_callee__a_0	EQU	$88
__zpabi_callee__a_1	EQU	$89
__zpabi_callee__a_2	EQU	$8A
__zpabi_callee__a_3	EQU	$8B
__zpabi_callee__a_4	EQU	$8C
__zpabi_callee__a_5	EQU	$8D
__zpabi_callee__a_6	EQU	$8E
__zpabi_callee__a_7	EQU	$8F
__zpabi_callee__b_0	EQU	$90
__zpabi_callee__b_1	EQU	$91
__zpabi_callee__b_2	EQU	$92
__zpabi_callee__b_3	EQU	$93
__zpabi_callee__b_4	EQU	$94
__zpabi_callee__b_5	EQU	$95
__zpabi_callee__b_6	EQU	$96
__zpabi_callee__b_7	EQU	$97

; @zp-link-meta-begin
; def callee params=__zpabi_callee__a_0,__zpabi_callee__a_1,__zpabi_callee__a_2,__zpabi_callee__a_3,__zpabi_callee__a_4,__zpabi_callee__a_5,__zpabi_callee__a_6,__zpabi_callee__a_7,__zpabi_callee__b_0,__zpabi_callee__b_1,__zpabi_callee__b_2,__zpabi_callee__b_3,__zpabi_callee__b_4,__zpabi_callee__b_5,__zpabi_callee__b_6,__zpabi_callee__b_7 locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1,__zpabi_target__flag_2,__zpabi_target__flag_3,__zpabi_target__flag_4,__zpabi_target__flag_5,__zpabi_target__flag_6,__zpabi_target__flag_7 locals= indirect=false in_cycle=false
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item target@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item @3.x section=.bss local=true zero=true
; item @4.y section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

callee:
   SUBROUTINE

   LDA   __zpabi_callee__a_0
   CLC
   ADC   __zpabi_callee__b_0
   STA   HARGS+16
   LDA   __zpabi_callee__a_1
   ADC   __zpabi_callee__b_1
   STA   HARGS+17
   LDA   __zpabi_callee__a_2
   ADC   __zpabi_callee__b_2
   STA   HARGS+18
   LDA   __zpabi_callee__a_3
   ADC   __zpabi_callee__b_3
   STA   HARGS+19
   LDA   __zpabi_callee__a_4
   ADC   __zpabi_callee__b_4
   STA   HARGS+20
   LDA   __zpabi_callee__a_5
   ADC   __zpabi_callee__b_5
   STA   HARGS+21
   LDA   __zpabi_callee__a_6
   ADC   __zpabi_callee__b_6
   STA   HARGS+22
   LDA   __zpabi_callee__a_7
   ADC   __zpabi_callee__b_7
   STA   HARGS+23
   RTS

target:
   SUBROUTINE

   LDA   __zpabi_target__flag_0
   ORA   __zpabi_target__flag_1
   ORA   __zpabi_target__flag_2
   ORA   __zpabi_target__flag_3
   ORA   __zpabi_target__flag_4
   ORA   __zpabi_target__flag_5
   ORA   __zpabi_target__flag_6
   ORA   __zpabi_target__flag_7
   BEQ   .if_else@1
   JSR   target@run0
   LDA   #$14
   STA   @4.y
   STA   @3.x
   JMP   .if_end@0
.if_else@1:
   JSR   target@run0
   LDA   #$64
   STA   @4.y
   STA   @3.x
.if_end@0:
   STA   __zpabi_callee__a_0
   STA   __zpabi_callee__b_0
   LDA   @3.x+1
   STA   __zpabi_callee__a_1
   STA   __zpabi_callee__a_2
   STA   __zpabi_callee__a_3
   STA   __zpabi_callee__a_4
   STA   __zpabi_callee__a_5
   STA   __zpabi_callee__a_6
   STA   __zpabi_callee__a_7
   STA   __zpabi_callee__b_1
   STA   __zpabi_callee__b_2
   STA   __zpabi_callee__b_3
   STA   __zpabi_callee__b_4
   STA   __zpabi_callee__b_5
   STA   __zpabi_callee__b_6
   STA   __zpabi_callee__b_7
   JMP   callee

target@run0:
   SUBROUTINE

   LDA   #$00
   STA   @4.y+1
   STA   @4.y+2
   STA   @4.y+3
   STA   @4.y+4
   STA   @4.y+5
   STA   @4.y+6
   STA   @4.y+7
   STA   @3.x+1
   STA   @3.x+2
   STA   @3.x+3
   STA   @3.x+4
   STA   @3.x+5
   STA   @3.x+6
   STA   @3.x+7
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_target__flag_0
   STA   __zpabi_target__flag_1
   JSR   main@run2
   BNE   .jcmp_skip@0
   LDA   HARGS+22
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
   CMP   #$C8
   BEQ   .if_end@2
.jcmp_skip@0:
   LDA   #$01
   JSR   main@run3
   RTS
.if_end@2:
   LDA   #$01
   STA   __zpabi_target__flag_0
   LDA   #$00
   STA   __zpabi_target__flag_1
   JSR   main@run2
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   BNE   .jcmp_skip@1
   LDA   HARGS+21
   BNE   .jcmp_skip@1
   LDA   HARGS+20
   BNE   .jcmp_skip@1
   LDA   HARGS+19
   BNE   .jcmp_skip@1
   LDA   HARGS+18
   BNE   .jcmp_skip@1
   LDA   HARGS+17
   BNE   .jcmp_skip@1
   LDA   HARGS+16
   CMP   #$28
   BEQ   .if_end@3
.jcmp_skip@1:
   LDA   #$02
   JSR   main@run3
   RTS
.if_end@3:
   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   JSR   main@run1
   RTS

main@run3:
   SUBROUTINE

   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   JMP   main@run1

main@run2:
   SUBROUTINE

   STA   __zpabi_target__flag_2
   STA   __zpabi_target__flag_3
   STA   __zpabi_target__flag_4
   STA   __zpabi_target__flag_5
   STA   __zpabi_target__flag_6
   STA   __zpabi_target__flag_7
   JSR   target
   LDA   HARGS+23
   RTS

main@run1:
   SUBROUTINE

   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

@3.x:
   DS.B  8

@4.y:
   DS.B  8
