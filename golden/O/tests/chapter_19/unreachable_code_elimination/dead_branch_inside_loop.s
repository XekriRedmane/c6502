SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_target__i_7	EQU	$88
__local_target__i_6	EQU	$89
__local_target__i_5	EQU	$8A
__local_target__i_4	EQU	$8B
__local_target__i_3	EQU	$8C
__local_target__i_2	EQU	$8D
__local_target__i_1	EQU	$8E
__local_target__i_0	EQU	$8F
__local_target__result_7	EQU	$90
__local_target__result_6	EQU	$91
__local_target__result_5	EQU	$92
__local_target__result_4	EQU	$93
__local_target__result_3	EQU	$94
__local_target__result_2	EQU	$95
__local_target__result_1	EQU	$96
__local_target__result_0	EQU	$97

; @zp-link-meta-begin
; def callee params= locals=__local_callee__0,__local_callee__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target params= locals=__local_target__i_7,__local_target__i_6,__local_target__i_5,__local_target__i_4,__local_target__i_3,__local_target__i_2,__local_target__i_1,__local_target__i_0,__local_target__result_7,__local_target__result_6,__local_target__result_5,__local_target__result_4,__local_target__result_3,__local_target__result_2,__local_target__result_1,__local_target__result_0 indirect=false in_cycle=false
; call main -> target
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

callee:
   SUBROUTINE

   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+5
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   HARGS+4
   STA   HARGS+16
   LDA   HARGS+5
   STA   HARGS+17
   RTS

target:
   SUBROUTINE

   LDA   #$69
   STA   __local_target__result_0
   LDA   #$00
   STA   __local_target__result_1
   STA   __local_target__result_2
   STA   __local_target__result_3
   STA   __local_target__result_4
   STA   __local_target__result_5
   STA   __local_target__result_6
   STA   __local_target__result_7
   STA   __local_target__i_0
   STA   __local_target__i_1
   STA   __local_target__i_2
   STA   __local_target__i_3
   STA   __local_target__i_4
   STA   __local_target__i_5
   STA   __local_target__i_6
   LDY   #$00
   STY   __local_target__i_7
.loop@0_start:
   LDA   __local_target__i_0
   SEC
   SBC   #$64
   LDA   __local_target__i_1
   SBC   #$00
   LDA   __local_target__i_2
   SBC   #$00
   LDA   __local_target__i_3
   SBC   #$00
   LDA   __local_target__i_4
   SBC   #$00
   LDA   __local_target__i_5
   SBC   #$00
   LDA   __local_target__i_6
   SBC   #$00
   LDA   __local_target__i_7
   SBC   #$00
   BVC   .jcmp_novf@2
   EOR   #$80
.jcmp_novf@2:
   BMI   .lb_skip@6
   JMP   .loop@0_break
.lb_skip@6:
   LDA   __local_target__result_0
   SEC
   SBC   __local_target__i_0
   STA   __local_target__result_0
   LDA   __local_target__result_1
   SBC   __local_target__i_1
   STA   __local_target__result_1
   LDA   __local_target__result_2
   SBC   __local_target__i_2
   STA   __local_target__result_2
   LDA   __local_target__result_3
   SBC   __local_target__i_3
   STA   __local_target__result_3
   LDA   __local_target__result_4
   SBC   __local_target__i_4
   STA   __local_target__result_4
   LDA   __local_target__result_5
   SBC   __local_target__i_5
   STA   __local_target__result_5
   LDA   __local_target__result_6
   SBC   __local_target__i_6
   STA   __local_target__result_6
   LDA   __local_target__result_7
   SBC   __local_target__i_7
   STA   __local_target__result_7
   INC   __local_target__i_0
   BNE   .loop@0_start
   INC   __local_target__i_1
   BEQ   .lb_skip@5
   JMP   .loop@0_start
.lb_skip@5:
   INC   __local_target__i_2
   BEQ   .lb_skip@4
   JMP   .loop@0_start
.lb_skip@4:
   INC   __local_target__i_3
   BEQ   .lb_skip@3
   JMP   .loop@0_start
.lb_skip@3:
   INC   __local_target__i_4
   BEQ   .lb_skip@2
   JMP   .loop@0_start
.lb_skip@2:
   INC   __local_target__i_5
   BEQ   .lb_skip@1
   JMP   .loop@0_start
.lb_skip@1:
   INC   __local_target__i_6
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_target__i_7
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_target__result_0
   STA   HARGS+16
   LDA   __local_target__result_1
   STA   HARGS+17
   LDA   __local_target__result_2
   STA   HARGS+18
   LDA   __local_target__result_3
   STA   HARGS+19
   LDA   __local_target__result_4
   STA   HARGS+20
   LDA   __local_target__result_5
   STA   HARGS+21
   LDA   __local_target__result_6
   STA   HARGS+22
   LDA   __local_target__result_7
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   JSR   target
   LDA   HARGS+23
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+22
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+21
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+20
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+19
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+18
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+17
   CMP   #$ED
   BNE   .jcmp_skip@3
   LDA   HARGS+16
   CMP   #$13
   BEQ   .if_end@1
.jcmp_skip@3:
   LDA   #$01
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   JSR   main@run0
   RTS
.if_end@1:
   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   JSR   main@run0
   RTS

main@run0:
   SUBROUTINE

   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS
