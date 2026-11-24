SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v2	EQU	$80
__zpabi_check__v3_0	EQU	$81
__zpabi_check__v3_1	EQU	$82
__zpabi_check__v4_0	EQU	$83
__zpabi_check__v4_1	EQU	$84
__zpabi_check__v4_2	EQU	$85
__zpabi_check__v4_3	EQU	$86
__zpabi_check__v5	EQU	$87
__local_check__fail	EQU	$89
__local_check__0	EQU	$8A
__local_check__1	EQU	$8B
__local_check__2	EQU	$8C
__local_check__3	EQU	$8D
__local_check__v0	EQU	$8F

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5 locals=__local_check__fail,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__v1,__local_check__v0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   TXA
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __local_check__v0
   LDA   __zpabi_check__v3_1
   SBC   #$00
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   TXA
   SEC
   SBC   __zpabi_check__v4_0
   LDA   __local_check__3
   SBC   __zpabi_check__v4_1
   LDA   __local_check__2
   SBC   __zpabi_check__v4_2
   LDA   __local_check__1
   SBC   __zpabi_check__v4_3
   LDA   #$00
   ROL
   STA   __local_check__fail
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   HARGS+3
   LDA   __local_check__0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_check__fail
   STA   HARGS+2
   JSR   sdivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$08
   STA   __local_check__fail
   LDX   #$00
.if_end@7:
   LDA   __local_check__v0
   CMP   #$80
   BEQ   .if_end@8
   LDA   __local_check__fail
   ORA   #$10
   STA   __local_check__fail
.if_end@8:
   LDA   __zpabi_check__v5
   BMI   .sx_neg@5
   TXA
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__2
   SEC
   TXA
   SBC   __zpabi_check__v5
   STA   __local_check__1
   TXA
   SBC   __local_check__2
   STA   __local_check__0
   LDA   __local_check__1
   CLC
   ADC   __zpabi_check__v3_0
   STA   __local_check__1
   LDA   __local_check__0
   ADC   __zpabi_check__v3_1
   CMP   #$5E
   BNE   .jcmp_skip@7
   LDA   __local_check__1
   CMP   #$38
   BEQ   .if_end@9
.jcmp_skip@7:
   LDA   __local_check__fail
   ORA   #$20
   STA   __local_check__fail
.if_end@9:
   LDA   __zpabi_check__v3_0
   SEC
   SBC   #$45
   LDA   __zpabi_check__v3_1
   SBC   #$23
   TXA
   ROL
   STA   __local_check__0
   TXA
   BNE   .cmp_true@8
   LDA   __local_check__0
   CMP   #$01
   BNE   .cmp_true@8
   TXA
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   AND   __zpabi_check__v5
   BEQ   .if_end@10
   LDA   __local_check__fail
   ORA   #$40
   STA   __local_check__fail
.if_end@10:
   LDA   __zpabi_check__v3_1
   CMP   #$5E
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v3_0
   CMP   #$3D
   BEQ   .if_end@11
.jcmp_skip@11:
   LDA   __local_check__fail
   ORA   #$80
   STA   __local_check__fail
.if_end@11:
   LDA   __local_check__fail
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail
   LDX   #$00
   JMP   .if_end@7

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_check__v2
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   LDA   #$3D
   STA   __zpabi_check__v3_0
   LDA   #$5E
   STA   __zpabi_check__v3_1
   LDA   #$05
   STA   __zpabi_check__v5
   LDX   #$01
   LDA   #$80
   JMP   check
