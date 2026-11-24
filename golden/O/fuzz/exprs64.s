SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v0_0	EQU	$80
__zpabi_check__v0_1	EQU	$81
__zpabi_check__v0_2	EQU	$82
__zpabi_check__v1	EQU	$83
__zpabi_check__v2	EQU	$84
__zpabi_check__v3_0	EQU	$85
__zpabi_check__v3_1	EQU	$86
__zpabi_check__v4_0	EQU	$87
__zpabi_check__v4_1	EQU	$88
__zpabi_check__v5_0	EQU	$89
__zpabi_check__v5_1	EQU	$8A
__zpabi_check__v5_2	EQU	$8B
__local_check__fail_1	EQU	$8D
__local_check__fail_0	EQU	$8E
__local_check__0	EQU	$8F
__local_check__1	EQU	$90
__local_check__2	EQU	$91
__local_check__3	EQU	$92
__local_check__4	EQU	$93
__local_check__5	EQU	$94
__local_check__6	EQU	$95
__local_check__7	EQU	$96
__local_check__9	EQU	$98

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v1,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v0_2
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_1
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_0
   CMP   #$01
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
   ASL
   LDA   __zpabi_check__v0_1
   ROL
   STA   __local_check__6
   LDA   __zpabi_check__v0_2
   ROL
   STA   __local_check__5
   LDA   #$00
   ROL
   STA   __local_check__4
   LDA   __zpabi_check__v2
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_check__9
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDX   __local_check__9
   STX   __local_check__1
   AND   #$07
   STA   __local_check__3
   LDA   __zpabi_check__v2
   SEC
   SBC   #$02
   STA   __local_check__0
   LDA   __local_check__1
   SBC   #$00
   STA   __local_check__1
   BCS   .carry_inc@0
   DEC   __local_check__3
.carry_inc@0:
   LDA   __local_check__0
   STA   __local_check__2
   LDA   __local_check__3
   AND   #$07
   STA   __local_check__0
   LDA   __local_check__6
   CLC
   ADC   __local_check__2
   STA   __local_check__2
   LDA   __local_check__5
   ADC   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   ADC   __local_check__0
   AND   #$07
   STA   __local_check__0
   SEC
   LDA   #$00
   SBC   __local_check__2
   STA   __local_check__2
   LDA   #$00
   SBC   __local_check__1
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   AND   #$07
   BNE   .jcmp_skip@5
   LDA   __local_check__1
   BNE   .jcmp_skip@5
   LDA   __local_check__2
   CMP   #$82
   BEQ   .if_end@1
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   __zpabi_check__v5_2
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   LDY   #$00
   STY   HARGS+4
   STY   HARGS+5
   STA   HARGS+6
   LDA   #$80
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   #$45
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
   STY   HARGS+7
   JSR   udivmod32
   LDA   #$80
   CLC
   ADC   __zpabi_check__v5_0
   STA   __local_check__7
   LDA   #$00
   ADC   __zpabi_check__v5_1
   STA   __local_check__6
   LDA   #$00
   ADC   __zpabi_check__v5_2
   STA   __local_check__5
   LDA   #$00
   ADC   #$00
   STA   __local_check__4
   LDA   __zpabi_check__v1
   BEQ   .and_false@6
   LDA   #$01
   STA   __local_check__3
   LDA   #$00
   BEQ   .and_end@7
.and_false@6:
   LDA   #$00
   STA   __local_check__3
.and_end@7:
   ORA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__7
   EOR   __local_check__3
   STA   __local_check__3
   LDA   __local_check__6
   STA   __local_check__2
   LDA   __local_check__5
   EOR   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   EOR   __local_check__0
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_true@8
   CMP   __local_check__1
   BNE   .cmp_true@8
   CMP   __local_check__2
   BNE   .cmp_true@8
   LDA   HARGS+12
   CMP   __local_check__3
   BNE   .cmp_true@8
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@11
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@8
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   LDA   #$00
   CMP   __local_check__9
   BNE   .jcmp_skip@12
   LDA   #$01
   CMP   __zpabi_check__v2
   BEQ   .and_false@9
.jcmp_skip@12:
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   ORA   __zpabi_check__v5_2
   BEQ   .and_false@9
   LDA   #$01
   STA   __local_check__4
   LDA   #$00
   BEQ   .and_end@10
.and_false@9:
   LDA   #$00
   STA   __local_check__4
.and_end@10:
   ORA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$FF
   SEC
   SBC   __local_check__4
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   __local_check__2
   LDA   #$00
   SBC   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@16
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@11
.jcmp_skip@16:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@11:
   LDA   __zpabi_check__v5_2
   CMP   #$01
   BNE   .jcmp_skip@17
   LDA   __zpabi_check__v5_1
   CMP   #$96
   BNE   .jcmp_skip@17
   LDA   __zpabi_check__v5_0
   CMP   #$09
   BEQ   .if_end@12
.jcmp_skip@17:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@12:
   LDA   __zpabi_check__v4_1
   CMP   #$FF
   BNE   .jcmp_skip@18
   LDA   __zpabi_check__v4_0
   CMP   #$FF
   BEQ   .if_end@13
.jcmp_skip@18:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@13:
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

   LDA   #$00
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v1
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   LDA   #$80
   STA   __zpabi_check__v2
   LDA   #$FF
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   LDA   #$09
   STA   __zpabi_check__v5_0
   LDA   #$96
   STA   __zpabi_check__v5_1
   LDA   #$01
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v5_2
   JMP   check
