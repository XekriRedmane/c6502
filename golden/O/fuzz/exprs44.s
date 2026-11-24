SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1	EQU	$80
__zpabi_check__v2_0	EQU	$81
__zpabi_check__v2_1	EQU	$82
__zpabi_check__v3_0	EQU	$83
__zpabi_check__v3_1	EQU	$84
__zpabi_check__v3_2	EQU	$85
__zpabi_check__v3_3	EQU	$86
__zpabi_check__v4_0	EQU	$87
__zpabi_check__v4_1	EQU	$88
__zpabi_check__v4_2	EQU	$89
__zpabi_check__v4_3	EQU	$8A
__zpabi_check__v5_0	EQU	$8B
__zpabi_check__v5_1	EQU	$8C
__zpabi_check__v5_2	EQU	$8D
__zpabi_check__v5_3	EQU	$8E
__local_check__fail_1	EQU	$90
__local_check__fail_0	EQU	$91
__local_check__0	EQU	$92
__local_check__1	EQU	$93
__local_check__2	EQU	$94
__local_check__3	EQU	$95
__local_check__v0_1	EQU	$96
__local_check__v0_0	EQU	$97

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__v0_1,__local_check__v0_0,__local_check__4,__local_check__5 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   LDA   __zpabi_check__v2_0
   CLC
   SBC   __local_check__v0_0
   LDA   __zpabi_check__v2_1
   SBC   __local_check__v0_1
   LDA   #$00
   ROL
   STA   __local_check__3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__2
   LDA   __zpabi_check__v3_0
   EOR   #$7F
   SEC
   SBC   #$45
   LDA   __zpabi_check__v3_1
   SBC   #$00
   LDA   __zpabi_check__v3_2
   SBC   #$00
   LDA   __zpabi_check__v3_3
   SBC   #$00
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   SEC
   SBC   __local_check__3
   LDA   #$00
   SBC   __local_check__2
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   #$00
   BNE   .jcmp_skip@4
   LDA   __local_check__fail_1
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@4:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v1
   CMP   #$06
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v5_3
   BNE   .jcmp_skip@5
   LDA   __zpabi_check__v5_2
   BNE   .jcmp_skip@5
   LDA   __zpabi_check__v5_1
   BNE   .jcmp_skip@5
   LDA   __zpabi_check__v5_0
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v2_0
   STA   HARGS
   LDA   __zpabi_check__v2_1
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   SEC
   SBC   #$07
   LDA   HARGS+5
   SBC   #$00
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   SEC
   SBC   #$01
   LDA   #$00
   SBC   #$00
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BMI   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BEQ   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__1
   LDA   #$00
   CMP   __zpabi_check__v2_1
   BNE   .cmp_end@11
   LDA   #$02
   CMP   __zpabi_check__v2_0
   BEQ   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   __local_check__0
   LDA   __local_check__1
   BNE   .cmp_true@13
   LDA   __zpabi_check__v1
   CMP   __local_check__0
   BNE   .cmp_true@13
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_check__0
   LDA   __zpabi_check__v2_1
   BNE   .cmp_true@16
   LDA   __zpabi_check__v2_0
   CMP   __local_check__0
   BNE   .cmp_true@16
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@19
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@19:
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

   LDA   #$06
   STA   __zpabi_check__v1
   LDA   #$72
   STA   __zpabi_check__v2_0
   LDA   #$07
   STA   __zpabi_check__v2_1
   LDA   #$01
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v5_0
   LDA   #$80
   STA   __zpabi_check__v4_3
   LDA   #$00
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   LDX   #$00
   JMP   check
