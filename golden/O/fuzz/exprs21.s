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
__zpabi_check__v3	EQU	$86
__zpabi_check__v4	EQU	$87
__zpabi_check__v5_0	EQU	$88
__zpabi_check__v5_1	EQU	$89
__local_check__fail_1	EQU	$8B
__local_check__fail_0	EQU	$8C
__local_check__0	EQU	$8D
__local_check__1	EQU	$8E
__local_check__v0	EQU	$8F
__local_check__2	EQU	$90
__local_check__3	EQU	$91

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__v0,__local_check__2,__local_check__3,__local_check__4,__local_check__5 indirect=false in_cycle=false param_regs=A,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   LDA   __zpabi_check__v2_0
   STA   HARGS
   LDA   __zpabi_check__v2_1
   STA   HARGS+1
   LDA   __zpabi_check__v5_0
   STA   HARGS+2
   LDA   __zpabi_check__v5_1
   STA   HARGS+3
   JSR   udivmod16
   LDA   __zpabi_check__v5_0
   CLC
   ADC   #$CF
   STA   __local_check__fail_0
   LDA   __zpabi_check__v5_1
   ADC   #$FF
   STA   __local_check__fail_1
   LDA   HARGS+5
   CMP   __local_check__fail_1
   BNE   .thread@0
   LDA   HARGS+4
   CMP   __local_check__fail_0
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   AND   #$02
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   BEQ   .if_end@4
   LDA   __zpabi_check__v2_0
   SEC
   SBC   #$80
   LDA   __zpabi_check__v2_1
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   BCC   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDY   #$00
   STY   __local_check__fail_1
   BPL   .if_end@4
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v1_3
   BNE   .thread@1
   LDA   __zpabi_check__v1_2
   BNE   .thread@1
   LDA   __zpabi_check__v1_1
   BNE   .thread@1
   LDA   __zpabi_check__v1_0
   CMP   __local_check__v0
   BEQ   .cmp_true@6
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __local_check__0
   LDA   #$01
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__v0
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v1_3
   CMP   __local_check__0
   BNE   .cmp_true@12
   LDA   __zpabi_check__v1_2
   CMP   __local_check__1
   BNE   .cmp_true@12
   LDA   __zpabi_check__v1_1
   BNE   .cmp_true@12
   LDA   __zpabi_check__v1_0
   CMP   __local_check__v0
   BNE   .cmp_true@12
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@15
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v2_1
   BNE   .jcmp_skip@16
   LDA   __zpabi_check__v2_0
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@16:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
   LDA   __zpabi_check__v2_1
   BNE   .jcmp_skip@17
   LDA   __zpabi_check__v2_0
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@17:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v4
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_check__v0
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v4
   SEC
   SBC   __zpabi_check__v1_0
   LDA   __local_check__v0
   SBC   __zpabi_check__v1_1
   LDA   __local_check__1
   SBC   __zpabi_check__v1_2
   LDA   __local_check__0
   SBC   __zpabi_check__v1_3
   LDA   #$00
   ROL
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   __local_check__2
   STA   __local_check__v0
   SEC
   LDA   #$00
   SBC   __zpabi_check__v3
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   STA   __local_check__0
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_check__1
   STA   HARGS
   LDA   __local_check__0
   STA   HARGS+1
   LDA   __zpabi_check__v1_0
   STA   HARGS+4
   LDA   __zpabi_check__v1_1
   STA   HARGS+5
   LDA   __zpabi_check__v1_2
   STA   HARGS+6
   LDA   __zpabi_check__v1_3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   HARGS+4
   LDA   HARGS+9
   STA   HARGS+5
   LDA   HARGS+10
   STA   HARGS+6
   LDA   HARGS+11
   STA   HARGS+7
   LDA   __local_check__3
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_check__2
   STA   HARGS+2
   LDA   __local_check__v0
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BEQ   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   SEC
   SBC   #$01
   LDA   HARGS+5
   SBC   #$00
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__1
   LDA   #$00
   BNE   .jcmp_skip@24
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@11
.jcmp_skip@24:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@11:
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

   LDA   #$07
   STA   __zpabi_check__v1_0
   LDA   #$01
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v4
   LDA   #$08
   STA   __zpabi_check__v3
   LDA   #$02
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v5_1
   LDA   #$FF
   JMP   check
