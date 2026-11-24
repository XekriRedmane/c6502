SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2	EQU	$82
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
__local_check__v0_1	EQU	$94
__local_check__v0_0	EQU	$95
__local_check__4	EQU	$98

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__v0_1,__local_check__v0_0,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,, changes=__zpabi_check__v3_2,__zpabi_check__v3_3
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   TXA
   BNE   .cmp_true@0
   LDA   __local_check__v0_0
   CMP   #$07
   BNE   .cmp_true@0
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   STA   __local_check__0
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_check__fail_0
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_check__fail_0
.and_end@1:
   LDA   __local_check__0
   SEC
   SBC   __local_check__fail_0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__fail_1
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_check__v4_0
   STA   HARGS
   LDA   __zpabi_check__v4_1
   STA   HARGS+1
   LDA   __zpabi_check__v4_2
   STA   HARGS+2
   LDA   __zpabi_check__v4_3
   STA   HARGS+3
   LDA   __local_check__fail_1
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   JSR   mul32
   LDA   HARGS+11
   CMP   #$AC
   BNE   .jcmp_skip@6
   LDA   HARGS+10
   CMP   #$BF
   BNE   .jcmp_skip@6
   LDA   HARGS+9
   CMP   #$57
   BNE   .jcmp_skip@6
   LDA   HARGS+8
   CMP   #$09
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@6:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   #$FF
   SEC
   SBC   __zpabi_check__v5_0
   LDA   #$7F
   SBC   __zpabi_check__v5_1
   LDA   #$00
   SBC   __zpabi_check__v5_2
   LDA   #$00
   SBC   __zpabi_check__v5_3
   JSR   check@run0
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BMI   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@4:
   LDA   __zpabi_check__v4_3
   CMP   #$AC
   BNE   .jcmp_skip@8
   LDA   __zpabi_check__v4_2
   CMP   #$BF
   BNE   .jcmp_skip@8
   LDA   __zpabi_check__v4_1
   CMP   #$57
   BNE   .jcmp_skip@8
   LDA   __zpabi_check__v4_0
   CMP   #$09
   BEQ   .if_end@5
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   __local_check__v0_1
   CMP   #$42
   BNE   .jcmp_skip@9
   LDA   __local_check__v0_0
   CMP   #$79
   BEQ   .if_end@7
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v1_0
   STA   __local_check__v0_0
   LDA   __zpabi_check__v3_1
   ORA   __zpabi_check__v1_1
   STA   __local_check__4
   LDA   __zpabi_check__v4_3
   BNE   .cmp_true@10
   LDA   __zpabi_check__v4_2
   BNE   .cmp_true@10
   LDA   __zpabi_check__v4_1
   BNE   .cmp_true@10
   LDA   __zpabi_check__v4_0
   BNE   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   __local_check__v0_1
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__v0_0
   EOR   __local_check__v0_1
   STA   __local_check__v0_0
   LDA   __local_check__4
   STA   __local_check__v0_1
   LDA   __zpabi_check__v3_2
   EOR   __local_check__1
   STA   __local_check__1
   LDA   __zpabi_check__v3_3
   EOR   __local_check__0
   STA   __local_check__0
   LDA   __local_check__v0_0
   CLC
   SBC   #$00
   LDA   __local_check__v0_1
   SBC   #$00
   LDA   __local_check__1
   SBC   #$00
   LDA   __local_check__0
   SBC   #$00
   JSR   check@run0
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@15
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@10
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDA   #$FF
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v5_0
   STA   HARGS+4
   LDA   __zpabi_check__v5_1
   STA   HARGS+5
   LDA   __zpabi_check__v5_2
   STA   HARGS+6
   LDA   __zpabi_check__v5_3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   STA   __local_check__0
   LDX   __zpabi_check__v1_0
   BEQ   .shift_done@17
.shift_loop@16:
   LSR   __local_check__0
   DEX
   BNE   .shift_loop@16
.shift_done@17:
   LDA   #$02
   SEC
   SBC   __local_check__0
   JSR   check@run0
   JSR   check@run0
   BVC   .jcmp_novf@18
   EOR   #$80
.jcmp_novf@18:
   BPL   .if_end@11
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
   JMP   .if_end@2

check@run0:
   SUBROUTINE

   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   LDA   #$80
   STA   __zpabi_check__v2
   LDA   #$D5
   STA   __zpabi_check__v3_0
   LDA   #$45
   STA   __zpabi_check__v3_1
   LDA   #$DC
   STA   __zpabi_check__v3_2
   LDA   #$B5
   STA   __zpabi_check__v3_3
   LDA   #$09
   STA   __zpabi_check__v4_0
   LDA   #$57
   STA   __zpabi_check__v4_1
   LDA   #$BF
   STA   __zpabi_check__v4_2
   LDA   #$AC
   STA   __zpabi_check__v4_3
   LDA   #$FF
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   LDX   #$42
   LDA   #$79
   JMP   check
