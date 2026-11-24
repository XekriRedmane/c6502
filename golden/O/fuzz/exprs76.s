SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v0_0	EQU	$80
__zpabi_check__v0_1	EQU	$81
__zpabi_check__v0_2	EQU	$82
__zpabi_check__v0_3	EQU	$83
__zpabi_check__v1_0	EQU	$84
__zpabi_check__v1_1	EQU	$85
__zpabi_check__v2_0	EQU	$86
__zpabi_check__v2_1	EQU	$87
__zpabi_check__v2_2	EQU	$88
__zpabi_check__v2_3	EQU	$89
__zpabi_check__v2_4	EQU	$8A
__zpabi_check__v2_5	EQU	$8B
__zpabi_check__v2_6	EQU	$8C
__zpabi_check__v3_0	EQU	$8D
__zpabi_check__v3_1	EQU	$8E
__zpabi_check__v3_2	EQU	$8F
__zpabi_check__v4_0	EQU	$90
__zpabi_check__v4_1	EQU	$91
__zpabi_check__v4_2	EQU	$92
__zpabi_check__v4_3	EQU	$93
__zpabi_check__v5_0	EQU	$94
__zpabi_check__v5_1	EQU	$95
__zpabi_check__v5_2	EQU	$96
__zpabi_check__v5_3	EQU	$97
__local_check__fail_1	EQU	$99
__local_check__fail_0	EQU	$9A
__local_check__1	EQU	$9C

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v2_4,__zpabi_check__v2_5,__zpabi_check__v2_6,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v4_3
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v4_2
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v4_1
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v4_0
   CMP   #$04
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v0_0
   STA   HARGS
   LDA   __zpabi_check__v0_1
   STA   HARGS+1
   LDA   __zpabi_check__v0_2
   STA   HARGS+2
   LDA   __zpabi_check__v0_3
   STA   HARGS+3
   LDA   __zpabi_check__v3_0
   STA   HARGS+4
   LDA   __zpabi_check__v3_1
   STA   HARGS+5
   LDA   __zpabi_check__v3_2
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+15
   BNE   .jcmp_skip@1
   LDA   HARGS+14
   CMP   #$01
   BNE   .jcmp_skip@1
   LDA   HARGS+13
   CMP   #$C0
   BNE   .jcmp_skip@1
   LDA   HARGS+12
   CMP   #$48
   BEQ   .if_end@1
.jcmp_skip@1:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __zpabi_check__v5_3
   CMP   #$CA
   BNE   .jcmp_skip@2
   LDA   __zpabi_check__v5_2
   CMP   #$0A
   BNE   .jcmp_skip@2
   LDA   __zpabi_check__v5_1
   CMP   #$4D
   BNE   .jcmp_skip@2
   LDA   __zpabi_check__v5_0
   CMP   #$50
   BEQ   .if_end@2
.jcmp_skip@2:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   ORA   __zpabi_check__v3_2
   BEQ   .and_false@5
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   ORA   __zpabi_check__v3_2
   BEQ   .and_false@5
   LDA   #$01
   STA   __local_check__1
   LDA   #$00
   BEQ   .and_end@6
.and_false@5:
   LDA   #$00
   STA   __local_check__1
.and_end@6:
   CMP   __zpabi_check__v5_3
   BNE   .if_end@7
   CMP   __zpabi_check__v5_2
   BNE   .if_end@7
   CMP   __zpabi_check__v5_1
   BNE   .if_end@7
   LDA   __local_check__1
   CMP   __zpabi_check__v5_0
   BNE   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v1_1
   CMP   #$E8
   BNE   .jcmp_skip@3
   LDA   __zpabi_check__v1_0
   CMP   #$9B
   BEQ   .if_end@8
.jcmp_skip@3:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
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

   LDA   #$FF
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v2_6
   LDA   #$9B
   STA   __zpabi_check__v1_0
   LDA   #$E8
   STA   __zpabi_check__v1_1
   LDA   #$00
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v2_4
   STA   __zpabi_check__v2_5
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   LDA   #$79
   STA   __zpabi_check__v3_0
   LDA   #$F5
   STA   __zpabi_check__v3_1
   LDA   #$01
   STA   __zpabi_check__v3_2
   LDA   #$04
   STA   __zpabi_check__v4_0
   LDA   #$50
   STA   __zpabi_check__v5_0
   LDA   #$4D
   STA   __zpabi_check__v5_1
   LDA   #$0A
   STA   __zpabi_check__v5_2
   LDA   #$CA
   STA   __zpabi_check__v5_3
   JMP   check
