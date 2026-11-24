SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1	EQU	$80
__zpabi_check__v2_0	EQU	$81
__zpabi_check__v2_1	EQU	$82
__zpabi_check__v2_2	EQU	$83
__zpabi_check__v2_3	EQU	$84
__zpabi_check__v3_0	EQU	$85
__zpabi_check__v3_1	EQU	$86
__zpabi_check__v4_0	EQU	$87
__zpabi_check__v4_1	EQU	$88
__zpabi_check__v4_2	EQU	$89
__zpabi_check__v4_3	EQU	$8A
__zpabi_check__v5_0	EQU	$8B
__zpabi_check__v5_1	EQU	$8C
__local_check__fail_1	EQU	$8E
__local_check__fail_0	EQU	$8F
__local_check__0	EQU	$90

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v5_1
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v5_0
   CMP   #$FF
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v3_0
   LDA   __zpabi_check__v3_1
   SBC   __zpabi_check__v3_1
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$07
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   AND   __zpabi_check__v1
   CMP   #$01
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v1
   CMP   #$2F
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v4_0
   STA   HARGS
   LDA   __zpabi_check__v4_1
   STA   HARGS+1
   LDA   __zpabi_check__v4_2
   STA   HARGS+2
   LDA   __zpabi_check__v4_3
   STA   HARGS+3
   LDA   #$45
   STA   HARGS+4
   LDA   #$23
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BEQ   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v1
   CMP   #$2F
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v5_0
   SEC
   SBC   #$45
   LDA   __zpabi_check__v5_1
   SBC   #$00
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   AND   #$01
   ASL
   CMP   #$02
   BEQ   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v4_0
   LDA   __zpabi_check__v3_1
   SBC   __zpabi_check__v4_1
   LDA   #$00
   SBC   __zpabi_check__v4_2
   LDA   #$00
   SBC   __zpabi_check__v4_3
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   CMP   __zpabi_check__v3_1
   BNE   .if_end@9
   LDA   __local_check__0
   CMP   __zpabi_check__v3_0
   BNE   .if_end@9
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

   LDA   #$2F
   STA   __zpabi_check__v1
   LDA   #$A6
   STA   __zpabi_check__v2_0
   LDA   #$4F
   STA   __zpabi_check__v2_1
   LDA   #$E6
   STA   __zpabi_check__v2_2
   LDA   #$CA
   STA   __zpabi_check__v2_3
   LDA   #$EE
   STA   __zpabi_check__v3_0
   LDA   #$44
   STA   __zpabi_check__v3_1
   LDA   #$09
   STA   __zpabi_check__v4_0
   LDA   #$00
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   LDA   #$FF
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   LDX   #$00
   LDA   #$01
   JMP   check
