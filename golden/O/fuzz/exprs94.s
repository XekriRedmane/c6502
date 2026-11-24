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

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v0+3
   CMP   v2+3
   BNE   .cmp_true@0
   LDA   v0+2
   CMP   v2+2
   BNE   .cmp_true@0
   LDA   v0+1
   CMP   v2+1
   BNE   .cmp_true@0
   LDA   v0
   CMP   v2
   BNE   .cmp_true@0
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   STA   __local_check__fail_0
   LDA   v2
   CLC
   SBC   v5
   LDA   v2+1
   SBC   v5+1
   LDA   v2+2
   SBC   v5+2
   LDA   v2+3
   SBC   v5+3
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   __local_check__fail_0
   SEC
   SBC   __local_check__fail_1
   STA   __local_check__r_1
   LDA   #$00
   SBC   #$00
   STA   __local_check__r_2
   LDA   v3
   AND   v2
   STA   __local_check__r_3
   LDA   v3+1
   AND   v2+1
   STA   __local_check__fail_0
   LDA   #$00
   AND   v2+2
   CMP   #$01
   BNE   .cmp_true@3
   LDA   __local_check__fail_0
   CMP   #$23
   BNE   .cmp_true@3
   LDA   __local_check__r_3
   CMP   #$45
   BNE   .cmp_true@3
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   ORA   __local_check__r_1
   STA   __local_check__fail_0
   LDA   __local_check__r_2
   BNE   .jcmp_skip@6
   LDA   __local_check__fail_0
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@6:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   SEC
   SBC   v5
   STA   __local_check__r_3
   LDA   #$BB
   ORA   __local_check__r_3
   CMP   #$BB
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v4+3
   CMP   #$05
   BNE   .jcmp_skip@7
   LDA   v4+2
   CMP   #$39
   BNE   .jcmp_skip@7
   LDA   v4+1
   CMP   #$22
   BNE   .jcmp_skip@7
   LDA   v4
   CMP   #$40
   BEQ   .if_end@2
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v1+1
   CMP   #$6E
   BNE   .jcmp_skip@8
   LDA   v1
   CMP   #$42
   BEQ   .if_end@6
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   v5+3
   CMP   #$FF
   BNE   .jcmp_skip@9
   LDA   v5+2
   CMP   #$FF
   BNE   .jcmp_skip@9
   LDA   v5+1
   CMP   #$FF
   BNE   .jcmp_skip@9
   LDA   v5
   CMP   #$FF
   BEQ   .if_end@7
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   v3
   STA   HARGS
   LDA   v3+1
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   LDA   v4
   STA   HARGS+4
   LDA   v4+1
   STA   HARGS+5
   LDA   v4+2
   STA   HARGS+6
   LDA   v4+3
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+12
   EOR   #$FF
   STA   __local_check__r_0
   LDA   HARGS+13
   EOR   #$FF
   STA   __local_check__r_1
   LDA   HARGS+14
   EOR   #$FF
   STA   __local_check__r_2
   LDA   HARGS+15
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   __local_check__r_2
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   __local_check__r_1
   CMP   #$7F
   BNE   .jcmp_skip@10
   LDA   __local_check__r_0
   CMP   #$FF
   BEQ   .if_end@8
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   v0+3
   CMP   #$80
   BNE   .jcmp_skip@11
   LDA   v0+2
   BNE   .jcmp_skip@11
   LDA   v0+1
   BNE   .jcmp_skip@11
   LDA   v0
   BEQ   .if_end@9
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@9:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@0

main:
   SUBROUTINE

   JMP   check

v0:
   DC.L  $80000000

v1:
   DC.W  $6E42

v2:
   DC.L  $80000000

v3:
   DC.W  $8000

v4:
   DC.L  $05392240

v5:
   DC.L  $FFFFFFFF
