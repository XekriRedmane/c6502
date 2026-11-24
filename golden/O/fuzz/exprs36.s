SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2_0	EQU	$82
__zpabi_check__v2_1	EQU	$83
__zpabi_check__v2_2	EQU	$84
__zpabi_check__v2_3	EQU	$85
__zpabi_check__v3_0	EQU	$86
__zpabi_check__v3_1	EQU	$87
__zpabi_check__v4_0	EQU	$88
__zpabi_check__v4_1	EQU	$89
__zpabi_check__v5	EQU	$8A
__local_check__fail_1	EQU	$8C
__local_check__fail_0	EQU	$8D
__local_check__0	EQU	$8E
__local_check__1	EQU	$8F
__local_check__2	EQU	$90
__local_check__5	EQU	$93
__local_check__6	EQU	$94
__local_check__v0_1	EQU	$95
__local_check__v0_0	EQU	$96

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__v0_1,__local_check__v0_0,__local_check__7,__local_check__8 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BEQ   .lnot_true@0
   LDA   #$00
   BEQ   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   STA   __local_check__fail_0
   LDA   __zpabi_check__v5
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__6
   LDA   #$00
   SEC
   SBC   __zpabi_check__v5
   LDA   #$00
   SBC   __local_check__6
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   JSR   check@run0
   STA   __local_check__fail_1
   LDA   #$00
   BNE   .cmp_true@5
   LDA   __local_check__fail_0
   CMP   __local_check__fail_1
   BNE   .cmp_true@5
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   ORA   #$80
   STA   __local_check__fail_0
   LDA   #$00
   BNE   .jcmp_skip@8
   LDA   __local_check__fail_0
   CMP   #$81
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@8:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v1_1
   CMP   #$E0
   BNE   .jcmp_skip@9
   LDA   __zpabi_check__v1_0
   CMP   #$04
   BEQ   .if_end@1
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v4_0
   SEC
   SBC   __zpabi_check__v4_0
   STA   __local_check__1
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v5
   LDA   __zpabi_check__v3_1
   SBC   __local_check__6
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   JSR   check@run0
   AND   __local_check__1
   EOR   #$FF
   STA   __local_check__1
   LDA   #$00
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__1
   CMP   #$FF
   BEQ   .if_end@3
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v3_1
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   __zpabi_check__v3_0
   CMP   #$FF
   BEQ   .if_end@4
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v2_0
   STA   HARGS
   STA   HARGS+4
   LDA   __zpabi_check__v2_1
   STA   HARGS+1
   STA   HARGS+5
   LDA   __zpabi_check__v2_2
   STA   HARGS+2
   STA   HARGS+6
   LDA   __zpabi_check__v2_3
   STA   HARGS+3
   STA   HARGS+7
   JSR   sdivmod32
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v3_0
   LDA   __zpabi_check__v3_1
   SBC   __zpabi_check__v3_1
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   JSR   check@run0
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   HARGS+6
   STA   HARGS+7
   LDA   HARGS+12
   STA   HARGS
   LDA   HARGS+13
   STA   HARGS+1
   LDA   HARGS+14
   STA   HARGS+2
   LDA   HARGS+15
   STA   HARGS+3
   LDA   __local_check__0
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   JSR   mul32
   LDA   __zpabi_check__v4_1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v4_0
   SEC
   SBC   HARGS+8
   LDA   __zpabi_check__v4_1
   SBC   HARGS+9
   LDA   __local_check__1
   SBC   HARGS+10
   LDA   __local_check__0
   SBC   HARGS+11
   BVC   .jcmp_novf@18
   EOR   #$80
.jcmp_novf@18:
   BPL   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __local_check__v0_0
   SEC
   SBC   __zpabi_check__v5
   LDA   __local_check__v0_1
   SBC   __local_check__6
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   JSR   check@run0
   AND   __zpabi_check__v5
   STA   __local_check__v0_1
   LDA   #$00
   AND   __local_check__6
   STA   __local_check__6
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __local_check__5
   LDA   __zpabi_check__v5
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$45
   AND   __zpabi_check__v5
   STA   __local_check__2
   LDA   #$23
   AND   __local_check__1
   STA   __local_check__1
   LDA   #$01
   AND   __local_check__0
   STA   __local_check__0
   LDA   __local_check__v0_1
   AND   __local_check__2
   STA   __local_check__2
   LDA   __local_check__6
   AND   __local_check__1
   STA   __local_check__1
   LDA   __local_check__5
   AND   __local_check__0
   BNE   .jcmp_skip@24
   LDA   __local_check__1
   BNE   .jcmp_skip@24
   LDA   __local_check__2
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@24:
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

   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$04
   STA   __zpabi_check__v1_0
   LDA   #$E0
   STA   __zpabi_check__v1_1
   LDA   #$02
   STA   __zpabi_check__v2_0
   LDA   #$00
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v4_1
   LDA   #$01
   STA   __zpabi_check__v4_0
   LDA   #$FF
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v5
   LDX   #$0E
   LDA   #$F1
   JMP   check
