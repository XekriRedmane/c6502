SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__z_3	EQU	$84
__local_main__z_2	EQU	$85
__local_main__z_0	EQU	$87
__local_main__y_3	EQU	$88
__local_main__y_2	EQU	$89
__local_main__y_0	EQU	$8B
__local_main__x_7	EQU	$8C
__local_main__x_6	EQU	$8D
__local_main__x_5	EQU	$8E
__local_main__x_4	EQU	$8F
__local_main__x_3	EQU	$90
__local_main__x_2	EQU	$91
__local_main__x_1	EQU	$92
__local_main__x_0	EQU	$93
__local_main__w_7	EQU	$94
__local_main__w_6	EQU	$95
__local_main__w_5	EQU	$96
__local_main__w_4	EQU	$97
__local_main__w_3	EQU	$98
__local_main__w_2	EQU	$99
__local_main__w_1	EQU	$9A
__local_main__w_0	EQU	$9B

; @zp-link-meta-begin
; def main params= locals=__local_main__z_7,__local_main__z_6,__local_main__z_5,__local_main__z_4,__local_main__z_3,__local_main__z_2,__local_main__z_1,__local_main__z_0,__local_main__y_3,__local_main__y_2,__local_main__y_1,__local_main__y_0,__local_main__x_7,__local_main__x_6,__local_main__x_5,__local_main__x_4,__local_main__x_3,__local_main__x_2,__local_main__x_1,__local_main__x_0,__local_main__w_7,__local_main__w_6,__local_main__w_5,__local_main__w_4,__local_main__w_3,__local_main__w_2,__local_main__w_1,__local_main__w_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item zero section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=32
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   zero
   ORA   zero+1
   ORA   zero+2
   ORA   zero+3
   ORA   zero+4
   ORA   zero+5
   ORA   zero+6
   ORA   zero+7
   BNE   .lb_skip@0
   JMP   .main@asm_ssa_split@0
.lb_skip@0:
   LDA   #$03
   JSR   main@run1
   LDA   HARGS+24
   STA   __local_main__w_0
   LDA   HARGS+25
   STA   __local_main__w_1
   LDA   HARGS+26
   STA   __local_main__w_2
   LDA   HARGS+27
   STA   __local_main__w_3
   LDA   HARGS+28
   STA   __local_main__w_4
   LDA   HARGS+29
   STA   __local_main__w_5
   LDA   HARGS+30
   STA   __local_main__w_6
   LDA   HARGS+31
   STA   __local_main__w_7
   LDA   #$0A
   JSR   main@run1
   LDA   HARGS+16
   STA   __local_main__x_0
   LDA   HARGS+17
   STA   __local_main__x_1
   LDA   HARGS+18
   STA   __local_main__x_2
   LDA   HARGS+19
   STA   __local_main__x_3
   LDA   HARGS+20
   STA   __local_main__x_4
   LDA   HARGS+21
   STA   __local_main__x_5
   LDA   HARGS+22
   STA   __local_main__x_6
   LDA   HARGS+23
   STA   __local_main__x_7
   LDA   #$09
   STA   __local_main__y_0
   LDA   #$00
   STA   __local_main__y_2
   LDA   #$80
   STA   __local_main__y_3
   LDA   #$F7
   STA   __local_main__z_0
   LDA   #$7F
   STA   __local_main__z_3
   LDA   #$FF
   STA   __local_main__z_2
.if_end@0:
   LDA   __local_main__w_7
   BNE   .jcmp_skip@0
   LDA   __local_main__w_6
   BNE   .jcmp_skip@0
   LDA   __local_main__w_5
   BNE   .jcmp_skip@0
   LDA   __local_main__w_4
   BNE   .jcmp_skip@0
   LDA   __local_main__w_3
   BNE   .jcmp_skip@0
   LDA   __local_main__w_2
   BNE   .jcmp_skip@0
   LDA   __local_main__w_1
   BNE   .jcmp_skip@0
   LDA   __local_main__w_0
   CMP   #$03
   BEQ   .if_end@1
.jcmp_skip@0:
   LDA   #$01
   JSR   main@run0
   RTS
.if_end@1:
   LDA   __local_main__x_7
   BNE   .jcmp_skip@1
   LDA   __local_main__x_6
   BNE   .jcmp_skip@1
   LDA   __local_main__x_5
   BNE   .jcmp_skip@1
   LDA   __local_main__x_4
   BNE   .jcmp_skip@1
   LDA   __local_main__x_3
   BNE   .jcmp_skip@1
   LDA   __local_main__x_2
   BNE   .jcmp_skip@1
   LDA   __local_main__x_1
   BNE   .jcmp_skip@1
   LDA   __local_main__x_0
   CMP   #$0A
   BEQ   .if_end@2
.jcmp_skip@1:
   LDA   #$02
   JSR   main@run0
   RTS
.if_end@2:
   LDA   __local_main__y_3
   CMP   #$7F
   BNE   .jcmp_skip@2
   LDA   __local_main__y_2
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_main__y_0
   CMP   #$FF
   BEQ   .vthread@0
.jcmp_skip@2:
   LDA   #$03
   JSR   main@run0
   RTS
.vthread@0:
   LDA   __local_main__z_3
   CMP   #$80
   BNE   .jcmp_skip@3
   LDA   __local_main__z_2
   BNE   .jcmp_skip@3
   LDA   __local_main__z_0
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@3:
   LDA   #$04
   JSR   main@run0
   RTS
.if_end@4:
   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   JSR   main@run2
   RTS
.main@asm_ssa_split@0:
   LDA   #$03
   STA   __local_main__w_0
   LDA   #$00
   STA   __local_main__w_1
   STA   __local_main__w_2
   STA   __local_main__w_3
   STA   __local_main__w_4
   STA   __local_main__w_5
   STA   __local_main__w_6
   STA   __local_main__w_7
   STA   __local_main__x_1
   STA   __local_main__x_2
   STA   __local_main__x_3
   STA   __local_main__x_4
   STA   __local_main__x_5
   STA   __local_main__x_6
   STA   __local_main__x_7
   STA   __local_main__z_2
   LDA   #$0A
   STA   __local_main__x_0
   LDA   #$7F
   STA   __local_main__y_3
   LDA   #$01
   STA   __local_main__z_0
   LDA   #$80
   STA   __local_main__z_3
   LDA   #$FF
   STA   __local_main__y_0
   STA   __local_main__y_2
   JMP   .if_end@0

main@run2:
   SUBROUTINE

   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

main@run1:
   SUBROUTINE

   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STY   HARGS+7
   STY   HARGS+8
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JMP   sdivmod64

main@run0:
   SUBROUTINE

   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   JSR   main@run2
   RTS

zero:
   DS.B  8
