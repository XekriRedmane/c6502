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
__zpabi_check__v1_2	EQU	$86
__zpabi_check__v1_3	EQU	$87
__zpabi_check__v2_0	EQU	$88
__zpabi_check__v2_1	EQU	$89
__zpabi_check__v2_2	EQU	$8A
__zpabi_check__v2_3	EQU	$8B
__zpabi_check__v3_0	EQU	$8C
__zpabi_check__v3_1	EQU	$8D
__zpabi_check__v4_0	EQU	$8E
__zpabi_check__v4_1	EQU	$8F
__zpabi_check__v4_2	EQU	$90
__zpabi_check__v4_3	EQU	$91
__zpabi_check__v5_0	EQU	$92
__zpabi_check__v5_1	EQU	$93
__local_check__fail_1	EQU	$95
__local_check__fail_0	EQU	$96
__local_check__0	EQU	$97
__local_check__1	EQU	$98
__local_check__2	EQU	$99
__local_check__3	EQU	$9A

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v2_0
   ORA   __zpabi_check__v2_1
   ORA   __zpabi_check__v2_2
   ORA   __zpabi_check__v2_3
   BEQ   .vthread@0
   LDA   __zpabi_check__v4_0
   SEC
   SBC   __zpabi_check__v5_0
   STA   __local_check__1
   LDA   __zpabi_check__v4_1
   SBC   __zpabi_check__v5_1
   STA   __local_check__0
   LDA   __zpabi_check__v4_2
   SBC   #$00
   STA   __local_check__fail_0
   LDA   __zpabi_check__v4_3
   SBC   #$00
   STA   __local_check__fail_1
   LDA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__fail_0
   ORA   __local_check__fail_1
   BEQ   .vthread@0
   LDA   __zpabi_check__v0_1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   __zpabi_check__v0_0
   SEC
   SBC   __zpabi_check__v4_0
   LDA   __zpabi_check__v0_1
   SBC   __zpabi_check__v4_1
   LDA   __local_check__fail_0
   SBC   __zpabi_check__v4_2
   LDA   __local_check__fail_1
   SBC   __zpabi_check__v4_3
   BCS   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.vthread@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@4:
   SEC
   SBC   __zpabi_check__v0_0
   STA   HARGS+4
   LDA   #$00
   SBC   __zpabi_check__v0_1
   STA   HARGS+5
   LDA   #$00
   SBC   __zpabi_check__v0_2
   STA   HARGS+6
   LDA   #$00
   SBC   __zpabi_check__v0_3
   STA   HARGS+7
   LDA   __zpabi_check__v0_0
   STA   HARGS
   LDA   __zpabi_check__v0_1
   STA   HARGS+1
   LDA   __zpabi_check__v0_2
   STA   HARGS+2
   LDA   __zpabi_check__v0_3
   STA   HARGS+3
   JSR   mul32
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   HARGS+8
   CMP   #$FF
   BEQ   .if_end@6
.jcmp_skip@2:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@6:
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   ORA   __zpabi_check__v1_2
   ORA   __zpabi_check__v1_3
   BEQ   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v2_0
   SEC
   SBC   __zpabi_check__v1_0
   LDA   __zpabi_check__v2_1
   SBC   __zpabi_check__v1_1
   LDA   __zpabi_check__v2_2
   SBC   __zpabi_check__v1_2
   LDA   __zpabi_check__v2_3
   SBC   __zpabi_check__v1_3
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@3
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@8
.jcmp_skip@3:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v4_0
   SEC
   SBC   #$80
   STA   __local_check__3
   LDA   __zpabi_check__v4_1
   SBC   #$00
   STA   __local_check__2
   LDA   __zpabi_check__v4_2
   SBC   #$00
   STA   __local_check__1
   LDA   __zpabi_check__v4_3
   SBC   #$00
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_true@4
   CMP   __local_check__1
   BNE   .cmp_true@4
   CMP   __local_check__2
   BNE   .cmp_true@4
   LDA   #$01
   CMP   __local_check__3
   BNE   .cmp_end@5
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   STA   __local_check__0
   LDA   __zpabi_check__v5_0
   CLC
   SBC   __local_check__0
   LDA   __zpabi_check__v5_1
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@7
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   __zpabi_check__v4_3
   CMP   #$80
   BNE   .jcmp_skip@8
   LDA   __zpabi_check__v4_2
   BNE   .jcmp_skip@8
   LDA   __zpabi_check__v4_1
   BNE   .jcmp_skip@8
   LDA   __zpabi_check__v4_0
   BEQ   .if_end@10
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@4

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_check__v0_0
   LDA   #$00
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   LDA   #$80
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v4_3
   LDA   #$2C
   STA   __zpabi_check__v3_0
   LDA   #$87
   STA   __zpabi_check__v3_1
   LDA   #$FF
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   JMP   check
