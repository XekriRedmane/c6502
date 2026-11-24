SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v1_2	EQU	$82
__zpabi_check__v1_3	EQU	$83
__zpabi_check__v2_0	EQU	$84
__zpabi_check__v2_1	EQU	$85
__zpabi_check__v2_2	EQU	$86
__zpabi_check__v3_0	EQU	$87
__zpabi_check__v3_1	EQU	$88
__zpabi_check__v3_2	EQU	$89
__zpabi_check__v3_3	EQU	$8A
__zpabi_check__v4	EQU	$8B
__zpabi_check__v5	EQU	$8C
__local_check__fail_1	EQU	$8E
__local_check__fail_0	EQU	$8F
__local_check__0	EQU	$90
__local_check__1	EQU	$91
__local_check__4	EQU	$94
__local_check__5	EQU	$95
__local_check__6	EQU	$96
__local_check__7	EQU	$97
__local_check__v0_1	EQU	$98
__local_check__v0_0	EQU	$99

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__v0_1,__local_check__v0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,, changes=__zpabi_check__v3_0,__zpabi_check__v3_1
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=16
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   LDA   #$FF
   ORA   __zpabi_check__v2_0
   STA   __local_check__fail_0
   LDA   #$FF
   ORA   __zpabi_check__v2_1
   STA   __local_check__fail_1
   LDA   #$FF
   ORA   __zpabi_check__v2_2
   STA   __local_check__1
   LDA   __local_check__fail_0
   CLC
   ADC   __local_check__v0_0
   STA   __local_check__fail_0
   TXA
   ADC   __local_check__fail_1
   STA   __local_check__fail_1
   BCC   .carry_inc@0
   INC   __local_check__1
.carry_inc@0:
   LDA   __local_check__fail_0
   STA   __local_check__0
   LDA   __local_check__fail_1
   STA   __local_check__fail_0
   LDA   __local_check__1
   AND   #$03
   EOR   #$02
   SEC
   SBC   #$02
   BNE   .jcmp_skip@0
   LDA   __local_check__fail_0
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_check__0
   CMP   #$FE
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v1_3
   CMP   #$80
   BNE   .jcmp_skip@1
   LDA   __zpabi_check__v1_2
   BNE   .jcmp_skip@1
   LDA   __zpabi_check__v1_1
   BNE   .jcmp_skip@1
   LDA   __zpabi_check__v1_0
   BEQ   .if_end@1
.jcmp_skip@1:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v5
   BEQ   .lnot_true@2
   LDA   #$00
   BEQ   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@4
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v3_2
   STA   __local_check__1
   LDA   __zpabi_check__v3_3
   CMP   #$80
   BNE   .jcmp_skip@5
   LDA   __local_check__1
   BNE   .jcmp_skip@5
   LDA   __zpabi_check__v3_1
   BNE   .jcmp_skip@5
   LDA   __zpabi_check__v3_0
   BEQ   .if_end@3
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __local_check__v0_0
   STA   HARGS
   STX   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v3_0
   STA   HARGS+4
   LDA   __zpabi_check__v3_1
   STA   HARGS+5
   LDA   __zpabi_check__v3_2
   STA   HARGS+6
   LDA   __zpabi_check__v3_3
   JSR   check@run0
   LDA   HARGS+8
   SEC
   SBC   __zpabi_check__v3_0
   LDA   HARGS+9
   SBC   __zpabi_check__v3_1
   LDA   #$00
   SBC   __zpabi_check__v3_2
   LDA   #$00
   SBC   __zpabi_check__v3_3
   BVC   .jcmp_novf@6
   EOR   #$80
.jcmp_novf@6:
   BPL   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v1_3
   CMP   #$80
   BNE   .jcmp_skip@7
   LDA   __zpabi_check__v1_2
   BNE   .jcmp_skip@7
   LDA   __zpabi_check__v1_1
   BNE   .jcmp_skip@7
   LDA   __zpabi_check__v1_0
   BEQ   .vthread@0
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
   LDA   __zpabi_check__v1_0
.vthread@0:
   STA   HARGS
   LDA   __zpabi_check__v1_1
   STA   HARGS+1
   LDA   __zpabi_check__v1_2
   STA   HARGS+2
   LDA   __zpabi_check__v1_3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STA   HARGS+6
   LDA   #$80
   JSR   check@run0
   LDA   HARGS+12
   STA   __local_check__7
   LDA   HARGS+13
   STA   __local_check__6
   LDA   HARGS+14
   STA   __local_check__5
   LDA   HARGS+15
   STA   __local_check__4
   LDA   __zpabi_check__v2_0
   STA   HARGS
   LDA   __zpabi_check__v2_1
   STA   HARGS+1
   LDA   __zpabi_check__v2_2
   STA   HARGS+2
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+3
   LDA   __zpabi_check__v2_0
   STA   HARGS+4
   LDA   __zpabi_check__v2_1
   STA   HARGS+5
   LDA   __zpabi_check__v2_2
   STA   HARGS+6
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   JSR   check@run0
   LDA   HARGS+10
   AND   #$03
   EOR   #$02
   SEC
   SBC   #$02
   STA   __local_check__1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__0
   LDA   __local_check__7
   SEC
   SBC   HARGS+8
   LDA   __local_check__6
   SBC   HARGS+9
   LDA   __local_check__5
   SBC   __local_check__1
   LDA   __local_check__4
   SBC   __local_check__0
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   EOR   #$01
   STA   __local_check__1
   LDA   #$00
   BNE   .jcmp_skip@11
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   __local_check__v0_1
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   __local_check__v0_0
   CMP   #$FF
   BEQ   .if_end@7
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@7:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@0

check@run0:
   SUBROUTINE

   STA   HARGS+7
   JMP   sdivmod32

main:
   SUBROUTINE

   LDA   #$80
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v3_3
   LDA   #$FE
   STA   __zpabi_check__v2_2
   LDA   #$0B
   STA   __zpabi_check__v4
   LDA   #$00
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v5
   LDX   #$FF
   TXA
   JMP   check
