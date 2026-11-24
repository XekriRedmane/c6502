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
__zpabi_check__v3_0	EQU	$8A
__zpabi_check__v3_1	EQU	$8B
__zpabi_check__v3_2	EQU	$8C
__zpabi_check__v3_3	EQU	$8D
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
__local_check__4	EQU	$9B
__local_check__5	EQU	$9C
__local_check__7	EQU	$9E

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v4_3
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v4_2
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v4_1
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v4_0
   CMP   #$FF
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   #$FF
   SEC
   SBC   __zpabi_check__v0_0
   LDA   #$FF
   SBC   __zpabi_check__v0_1
   LDA   #$00
   SBC   __zpabi_check__v0_2
   LDA   #$00
   SBC   __zpabi_check__v0_3
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .if_end@3
   LDA   __zpabi_check__v0_0
   EOR   #$80
   STA   __local_check__3
   LDA   __zpabi_check__v0_1
   EOR   #$FF
   STA   __local_check__2
   LDA   __zpabi_check__v0_2
   EOR   #$FF
   STA   __local_check__1
   LDA   __zpabi_check__v0_3
   EOR   #$FF
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   #$FF
   CLC
   ADC   __zpabi_check__v2_0
   STA   __local_check__3
   LDA   #$FF
   ADC   __zpabi_check__v2_1
   STA   __local_check__2
   LDA   #$FF
   ADC   __zpabi_check__v2_2
   STA   __local_check__5
   LDA   #$FF
   ADC   __zpabi_check__v2_3
   STA   __local_check__4
   LDA   __zpabi_check__v1_1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __zpabi_check__v1_0
   STA   __local_check__3
   LDA   __local_check__2
   ORA   __zpabi_check__v1_1
   STA   __local_check__2
   LDA   __local_check__5
   ORA   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   ORA   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__3
   CMP   #$FF
   BEQ   .if_end@4
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v3_3
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   CMP   #$FF
   BNE   .cmp_end@6
   LDA   __local_check__0
   CMP   #$FF
   BEQ   .cmp_true@5
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   STA   __local_check__7
   LDA   #$00
   STA   HARGS
   LDY   #$01
   STY   HARGS+1
   DEY
   STY   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v0_0
   STA   HARGS+4
   JSR   lsr32
   LDA   __zpabi_check__v2_3
   CMP   __zpabi_check__v4_3
   BNE   .cmp_true@8
   LDA   __zpabi_check__v2_2
   CMP   __zpabi_check__v4_2
   BNE   .cmp_true@8
   LDA   __zpabi_check__v2_1
   CMP   __zpabi_check__v4_1
   BNE   .cmp_true@8
   LDA   __zpabi_check__v2_0
   CMP   __zpabi_check__v4_0
   BNE   .cmp_true@8
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__2
   SEC
   SBC   HARGS+5
   LDA   #$00
   SBC   HARGS+6
   LDA   __local_check__1
   SBC   HARGS+7
   LDA   __local_check__0
   SBC   HARGS+8
   LDA   #$00
   ROL
   CLC
   ADC   __local_check__7
   STA   __local_check__1
   LDA   #$00
   ADC   #$00
   BNE   .jcmp_skip@13
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v4_3
   CMP   #$FF
   BNE   .jcmp_skip@14
   LDA   __zpabi_check__v4_2
   CMP   #$FF
   BNE   .jcmp_skip@14
   LDA   __zpabi_check__v4_1
   CMP   #$FF
   BNE   .jcmp_skip@14
   LDA   __zpabi_check__v4_0
   CMP   #$FF
   BEQ   .if_end@6
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   __zpabi_check__v0_0
   SEC
   SBC   __zpabi_check__v2_0
   LDA   __zpabi_check__v0_1
   SBC   __zpabi_check__v2_1
   LDA   __zpabi_check__v0_2
   SBC   __zpabi_check__v2_2
   LDA   __zpabi_check__v0_3
   SBC   __zpabi_check__v2_3
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$7F
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   ORA   __zpabi_check__v3_2
   ORA   __zpabi_check__v3_3
   BEQ   .and_false@9
   LDA   __zpabi_check__v1_0
   SEC
   SBC   __zpabi_check__v1_0
   LDA   __zpabi_check__v1_1
   SBC   __zpabi_check__v1_1
   BVC   .jcmp_novf@17
   EOR   #$80
.jcmp_novf@17:
   BMI   .and_false@9
   LDA   #$01
   STA   __local_check__1
   LDA   #$00
   BEQ   .and_end@10
.and_false@9:
   LDA   #$00
   STA   __local_check__1
.and_end@10:
   STA   HARGS+1
   LDA   __local_check__2
   STA   HARGS
   LDA   __local_check__1
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BEQ   .if_end@11
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

   LDA   #$0A
   STA   __zpabi_check__v0_0
   LDA   #$01
   STA   __zpabi_check__v1_0
   LDA   #$80
   STA   __zpabi_check__v3_3
   LDA   #$FF
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   LDA   #$00
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   JMP   check
