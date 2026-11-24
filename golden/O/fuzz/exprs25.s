SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1	EQU	$80
__zpabi_check__v2_0	EQU	$81
__zpabi_check__v2_1	EQU	$82
__zpabi_check__v2_2	EQU	$83
__zpabi_check__v2_3	EQU	$84
__zpabi_check__v3	EQU	$85
__zpabi_check__v4	EQU	$86
__zpabi_check__v5_0	EQU	$87
__zpabi_check__v5_1	EQU	$88
__zpabi_check__v5_2	EQU	$89
__zpabi_check__v5_3	EQU	$8A
__local_check__fail_1	EQU	$8C
__local_check__fail_0	EQU	$8D
__local_check__0	EQU	$8E
__local_check__1	EQU	$8F
__local_check__2	EQU	$90
__local_check__3	EQU	$91
__local_check__4	EQU	$92
__local_check__5	EQU	$93
__local_check__6	EQU	$94
__local_check__7	EQU	$95
__local_check__8	EQU	$96
__local_check__9	EQU	$97
__local_check__10	EQU	$98
__local_check__v0_1	EQU	$99
__local_check__v0_0	EQU	$9A

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__v0_1,__local_check__v0_0,__local_check__11,__local_check__12,__local_check__13,__local_check__14 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,
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
   LDA   __zpabi_check__v3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__10
   LDA   __zpabi_check__v3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__0
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   #$00
   CLC
   SBC   __zpabi_check__v3
   LDA   #$80
   SBC   __local_check__0
   LDA   #$00
   SBC   __local_check__fail_0
   LDA   #$00
   SBC   __local_check__fail_1
   LDA   #$00
   ROL
   ORA   __zpabi_check__v3
   STA   __local_check__1
   LDA   __local_check__10
   STA   __local_check__0
   LDA   __local_check__v0_0
   EOR   #$FE
   STA   __local_check__fail_0
   TXA
   EOR   #$FF
   STA   __local_check__fail_1
   LDA   __local_check__1
   SEC
   SBC   __local_check__fail_0
   LDA   __local_check__0
   SBC   __local_check__fail_1
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
   LDA   __zpabi_check__v3
   SEC
   SBC   #$02
   LDA   __local_check__10
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   __zpabi_check__v4
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__0
   LDA   __zpabi_check__v4
   SEC
   SBC   __zpabi_check__v1
   LDA   __local_check__0
   SBC   #$00
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   STA   HARGS+2
   LDA   __local_check__1
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__5
   STA   __local_check__4
   SEC
   LDA   #$00
   SBC   __zpabi_check__v5_0
   STA   __local_check__3
   LDA   #$00
   SBC   __zpabi_check__v5_1
   STA   __local_check__2
   LDA   #$00
   SBC   __zpabi_check__v5_2
   STA   __local_check__1
   LDA   #$00
   SBC   __zpabi_check__v5_3
   STA   __local_check__0
   LDA   HARGS+4
   EOR   __local_check__3
   STA   __local_check__3
   LDA   HARGS+5
   EOR   __local_check__2
   STA   __local_check__2
   LDA   __local_check__5
   EOR   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   EOR   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   __local_check__3
   CMP   #$F4
   BEQ   .if_end@1
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __zpabi_check__v4
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_check__9
   STA   __local_check__8
   STA   __local_check__7
   STA   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v4
   STA   HARGS
   JSR   check@run0
   JSR   mul32
   SEC
   LDA   #$00
   SBC   HARGS+8
   STA   __local_check__3
   LDA   #$00
   SBC   HARGS+9
   STA   __local_check__2
   LDA   #$00
   SBC   HARGS+10
   STA   __local_check__1
   LDA   #$00
   SBC   HARGS+11
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __local_check__v0_0
   EOR   #$FF
   STA   __local_check__5
   LDA   __local_check__v0_1
   EOR   #$FF
   STA   __local_check__4
   LDA   __local_check__v0_0
   CLC
   ADC   #$7F
   STA   __local_check__3
   LDA   __local_check__v0_1
   ADC   #$00
   STA   __local_check__2
   LDA   #$00
   ADC   #$00
   STA   __local_check__1
   LDA   #$00
   ADC   #$00
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_end@14
   CMP   __local_check__1
   BNE   .cmp_end@14
   LDA   __local_check__4
   CMP   __local_check__2
   BNE   .thread@0
   LDA   __local_check__5
   CMP   __local_check__3
   BEQ   .cmp_true@13
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   STA   __local_check__5
   STA   __local_check__4
   LDA   __zpabi_check__v1
   SEC
   SBC   __zpabi_check__v3
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__10
   STA   __local_check__0
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_check__1
   STA   HARGS
   LDA   __local_check__0
   STA   HARGS+1
   LDA   __zpabi_check__v5_0
   STA   HARGS+4
   LDA   __zpabi_check__v5_1
   STA   HARGS+5
   LDA   __zpabi_check__v5_2
   STA   HARGS+6
   LDA   __zpabi_check__v5_3
   STA   HARGS+7
   JSR   sdivmod32
   LDA   __local_check__6
   SEC
   SBC   HARGS+12
   LDA   #$00
   SBC   HARGS+13
   LDA   __local_check__5
   SBC   HARGS+14
   LDA   __local_check__4
   SBC   HARGS+15
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@21
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@21:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v4
   STA   HARGS
   LDA   __local_check__9
   STA   HARGS+1
   LDA   __local_check__8
   STA   HARGS+2
   LDA   __local_check__7
   STA   HARGS+3
   JSR   check@run0
   JSR   udivmod32
   LDA   HARGS+15
   CMP   #$7F
   BNE   .jcmp_skip@22
   LDA   HARGS+14
   CMP   #$FF
   BNE   .jcmp_skip@22
   LDA   HARGS+13
   CMP   #$FF
   BNE   .jcmp_skip@22
   LDA   HARGS+12
   CMP   #$96
   BEQ   .if_end@4
.jcmp_skip@22:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v1
   CMP   #$80
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __local_check__v0_1
   CMP   #$C4
   BNE   .jcmp_skip@23
   LDA   __local_check__v0_0
   CMP   #$BB
   BEQ   .if_end@7
.jcmp_skip@23:
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

   LDA   __zpabi_check__v2_0
   STA   HARGS+4
   LDA   __zpabi_check__v2_1
   STA   HARGS+5
   LDA   __zpabi_check__v2_2
   STA   HARGS+6
   LDA   __zpabi_check__v2_3
   STA   HARGS+7
   RTS

main:
   SUBROUTINE

   LDA   #$80
   STA   __zpabi_check__v1
   STA   __zpabi_check__v2_3
   LDA   #$FF
   STA   __zpabi_check__v3
   LDA   #$96
   STA   __zpabi_check__v4
   LDA   #$0B
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   LDX   #$C4
   LDA   #$BB
   JMP   check
