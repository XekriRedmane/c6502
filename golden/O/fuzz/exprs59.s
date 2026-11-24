SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v2	EQU	$26
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_3	EQU	$83
__local_check__r_2	EQU	$84
__local_check__r_1	EQU	$85
__local_check__r_0	EQU	$86
__local_check__0	EQU	$87

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.bss local=false zero=true
; item v1 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.bss local=false zero=true
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v2 size=1 at=auto def=true init=$AE weight=4
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v0
   STA   HARGS
   STA   HARGS+2
   LDA   v0+1
   STA   HARGS+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   ORA   HARGS+5
   BEQ   .and_false@0
   LDA   v2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__r_3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   v5
   SEC
   SBC   v2
   LDA   v5+1
   SBC   __local_check__r_3
   LDA   v5+2
   SBC   __local_check__fail_0
   LDA   v5+3
   SBC   __local_check__fail_1
   BCS   .check@ssa_block@5
   LDA   v2
   BNE   .check@ssa_block@5
.and_false@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@4
.check@ssa_block@5:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@4:
   LDA   v3+4
   BNE   .jcmp_skip@4
   LDA   v3+3
   BNE   .jcmp_skip@4
   LDA   v3+2
   BNE   .jcmp_skip@4
   LDA   v3+1
   BNE   .jcmp_skip@4
   LDA   v3
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@5:
   LDA   v1
   EOR   v1
   STA   __local_check__0
   LDA   v1+1
   EOR   v1+1
   STA   __local_check__r_0
   LDA   v1+2
   EOR   v1+2
   STA   __local_check__r_1
   LDA   v1+3
   EOR   v1+3
   STA   __local_check__r_2
   LDA   v1+4
   EOR   v1+4
   STA   __local_check__r_3
   LDA   __local_check__0
   ORA   __local_check__r_0
   ORA   __local_check__r_1
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BEQ   .if_end@8
   LDA   v4+3
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   AND   #$3F
   STA   __local_check__r_3
   LDA   v1
   SEC
   SBC   v4
   LDA   v1+1
   SBC   v4+1
   LDA   v1+2
   SBC   v4+2
   LDA   v1+3
   SBC   v4+3
   LDA   v1+4
   SBC   __local_check__r_3
   BCS   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@8:
   LDA   v0
   SEC
   SBC   v1
   LDA   v0+1
   SBC   v1+1
   LDA   #$00
   SBC   v1+2
   LDA   #$00
   SBC   v1+3
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__r_1
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_check__r_2
   LDA   __local_check__r_1
   STA   __local_check__r_0
   LDA   __local_check__r_2
   AND   #$7F
   STA   __local_check__r_3
   LDA   __local_check__r_0
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@9:
   LDA   v2
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_check__r_1
   LDA   v5
   CLC
   SBC   #$01
   LDA   v5+1
   SBC   #$00
   LDA   v5+2
   SBC   #$00
   LDA   v5+3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__r_3
   LDA   #$00
   SEC
   SBC   __local_check__r_3
   STA   __local_check__r_2
   LDA   #$80
   SBC   #$00
   STA   __local_check__r_3
   LDA   v2
   EOR   __local_check__r_2
   STA   __local_check__r_2
   LDA   __local_check__r_1
   EOR   __local_check__r_3
   CMP   #$80
   BNE   .jcmp_skip@12
   LDA   __local_check__r_2
   CMP   #$51
   BEQ   .if_end@10
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@10:
   LDA   v2
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   HARGS+3
   LDA   v5
   STA   HARGS
   LDA   v5+1
   STA   HARGS+1
   LDA   v2
   STA   HARGS+2
   JSR   udivmod16
   LDA   HARGS+7
   BNE   .jcmp_skip@15
   LDA   HARGS+6
   CMP   #$0A
   BEQ   .if_end@11
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@11:
   LDA   v0
   ORA   v0+1
   BEQ   .lnot_true@16
   LDA   #$00
   BEQ   .lnot_end@17
.lnot_true@16:
   LDA   #$01
.lnot_end@17:
   STA   __local_check__r_3
   LDA   #$00
   BNE   .jcmp_skip@18
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@12
.jcmp_skip@18:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@12:
   LDA   v4
   ORA   v4+1
   ORA   v4+2
   ORA   v4+3
   BEQ   .if_end@13
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@13:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DS.B  2

v1:
   DS.B  4
   DC.B  $20

v3:
   DC.B  $01
   DS.B  4

v4:
   DS.B  4

v5:
   DC.L  $0000000A

__zp_init:
   SUBROUTINE

   LDA   #$AE
   STA   v2
   RTS
