SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2_0	EQU	$82
__zpabi_check__v2_1	EQU	$83
__zpabi_check__v3	EQU	$84
__zpabi_check__v4	EQU	$85
__zpabi_check__v5_0	EQU	$86
__zpabi_check__v5_1	EQU	$87
__local_check__fail_1	EQU	$89
__local_check__fail_0	EQU	$8A
__local_check__0	EQU	$8B
__local_check__1	EQU	$8C
__local_check__2	EQU	$8D
__local_check__3	EQU	$8E
__local_check__4	EQU	$8F
__local_check__6	EQU	$91
__local_check__7	EQU	$92
__local_check__v0	EQU	$93

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__v0,__local_check__8,__local_check__9 indirect=false in_cycle=false param_regs=A,,,,,,,, changes=__zpabi_check__v4
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__3
   LDA   __zpabi_check__v5_0
   SEC
   SBC   __local_check__v0
   STA   __local_check__fail_0
   LDA   __zpabi_check__v5_1
   SBC   __local_check__3
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_check__fail_0
   CMP   #$FE
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v4
   BNE   .vthread@0
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
   LDA   __zpabi_check__v4
.vthread@0:
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__2
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   __zpabi_check__v4
   STA   HARGS+2
   LDA   __local_check__2
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   SEC
   SBC   __zpabi_check__v4
   LDA   HARGS+7
   SBC   __local_check__2
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__2
   SEC
   LDA   #$00
   SBC   __local_check__v0
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__3
   STA   __local_check__0
   LDA   __local_check__2
   CLC
   ADC   __local_check__1
   STA   __local_check__1
   LDA   #$00
   ADC   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __local_check__1
   CMP   #$FD
   BEQ   .if_end@3
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v4
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_check__7
   STA   __local_check__6
   STA   __local_check__0
   LDA   #$80
   ORA   __local_check__0
   STA   __local_check__4
   LDA   __zpabi_check__v3
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_check__0
   LDA   __zpabi_check__v5_0
   CLC
   ADC   __zpabi_check__v3
   STA   __local_check__3
   LDA   #$00
   ADC   __local_check__0
   STA   __local_check__2
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v4
   CLC
   ADC   __local_check__3
   STA   __local_check__3
   LDA   __local_check__7
   ADC   __local_check__2
   STA   __local_check__2
   LDA   __local_check__6
   ADC   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   ADC   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   __local_check__3
   CMP   #$80
   BEQ   .if_end@5
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   #$FF
   CLC
   SBC   __zpabi_check__v2_0
   LDA   #$FF
   SBC   __zpabi_check__v2_1
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@14
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   __local_check__v0
   BEQ   .vthread@1
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   BNE   .if_end@9
.vthread@1:
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
   JMP   .if_end@1

main:
   SUBROUTINE

   LDA   #$0E
   STA   __zpabi_check__v1_0
   LDA   #$39
   STA   __zpabi_check__v2_0
   LDA   #$06
   STA   __zpabi_check__v2_1
   LDA   #$80
   STA   __zpabi_check__v3
   LDA   #$FF
   STA   __zpabi_check__v4
   LDA   #$01
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v5_1
   LDA   #$03
   JMP   check
