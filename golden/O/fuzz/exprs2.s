SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_3	EQU	$83
__local_check__r_2	EQU	$84
__local_check__r_1	EQU	$85
__local_check__r_0	EQU	$86
__local_check__0	EQU	$87
__local_check__1	EQU	$88
__local_check__2	EQU	$89
__local_check__3	EQU	$8A

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.bss local=false zero=true
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.bss local=false zero=true
; item v5 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   #$98
   AND   v4
   STA   __local_check__r_2
   LDA   #$C8
   AND   v4+1
   STA   __local_check__r_3
   LDA   v4+2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__fail_1
   LDA   __local_check__r_2
   AND   v2
   STA   __local_check__r_2
   LDA   __local_check__r_3
   AND   v2+1
   STA   __local_check__r_3
   LDA   v4+2
   AND   v2+2
   STA   __local_check__fail_0
   LDA   __local_check__fail_1
   AND   v2+3
   STA   __local_check__fail_1
   LDA   __local_check__r_2
   ORA   __local_check__r_3
   ORA   __local_check__fail_0
   ORA   __local_check__fail_1
   BNE   .check@ssa_block@3
   LDA   v4
   ORA   v4+1
   ORA   v4+2
   BNE   .check@ssa_block@3
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@2
.check@ssa_block@3:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   v4+2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__r_3
   LDA   v4
   ORA   v2
   STA   __local_check__3
   LDA   v4+1
   ORA   v2+1
   STA   __local_check__2
   LDA   v4+2
   ORA   v2+2
   STA   __local_check__1
   LDA   __local_check__r_3
   ORA   v2+3
   STA   __local_check__0
   LDA   v0
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_check__r_1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDX   v0
   STX   HARGS+4
   LDX   __local_check__r_1
   STX   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDA   __local_check__3
   STA   HARGS
   LDA   __local_check__2
   STA   HARGS+1
   LDA   __local_check__1
   STA   HARGS+2
   LDA   __local_check__0
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+8
   EOR   v2
   STA   __local_check__r_0
   LDA   HARGS+9
   EOR   v2+1
   STA   __local_check__r_1
   LDA   HARGS+10
   EOR   v2+2
   STA   __local_check__r_2
   LDA   HARGS+11
   EOR   v2+3
   BNE   .jcmp_skip@8
   LDA   __local_check__r_2
   BNE   .jcmp_skip@8
   LDA   __local_check__r_1
   BNE   .jcmp_skip@8
   LDA   __local_check__r_0
   CMP   #$09
   BEQ   .if_end@4
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   v0
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS
   STA   HARGS+1
   LDA   v0
   STA   HARGS+2
   JSR   mul16
   LDA   HARGS+5
   BNE   .jcmp_skip@11
   LDA   HARGS+4
   CMP   #$80
   BEQ   .if_end@5
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   v1
   ORA   v1+1
   BEQ   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   v2+3
   BNE   .jcmp_skip@12
   LDA   v2+2
   BNE   .jcmp_skip@12
   LDA   v2+1
   BNE   .jcmp_skip@12
   LDA   v2
   CMP   #$09
   BEQ   .if_end@9
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@9:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DC.B  $80

v1:
   DS.B  2

v2:
   DC.L  $00000009

v3:
   DC.B  $0A

v4:
   DS.B  3

v5:
   DC.W  $0001
