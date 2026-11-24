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
__zpabi_check__v2	EQU	$88
__zpabi_check__v3_0	EQU	$89
__zpabi_check__v3_1	EQU	$8A
__zpabi_check__v4_0	EQU	$8B
__zpabi_check__v4_1	EQU	$8C
__zpabi_check__v5_0	EQU	$8D
__zpabi_check__v5_1	EQU	$8E
__local_check__fail_1	EQU	$90
__local_check__fail_0	EQU	$91
__local_check__0	EQU	$92
__local_check__1	EQU	$93
__local_check__2	EQU	$94
__local_check__3	EQU	$95
__local_check__4	EQU	$96
__local_check__5	EQU	$97
__local_check__6	EQU	$98
__local_check__7	EQU	$99

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v0_0
   ORA   __zpabi_check__v0_1
   ORA   __zpabi_check__v0_2
   ORA   __zpabi_check__v0_3
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__7
   LDA   __zpabi_check__v2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__2
   STA   __local_check__1
   LSR
   LDA   __local_check__1
   ROR
   STA   __local_check__6
   LDA   __local_check__2
   ROR
   STA   __local_check__5
   LDA   __zpabi_check__v2
   ROR
   STA   __local_check__4
   LDA   #$00
   ROR
   STA   __local_check__3
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   ORA   __zpabi_check__v1_2
   ORA   __zpabi_check__v1_3
   BEQ   .lnot_true@4
   LDA   #$00
   BEQ   .lnot_end@5
.lnot_true@4:
   LDA   #$01
.lnot_end@5:
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__2
   SEC
   SBC   __local_check__3
   LDA   #$00
   SBC   __local_check__4
   LDA   __local_check__1
   SBC   __local_check__5
   LDA   __local_check__0
   SBC   __local_check__6
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__4
   LDA   __zpabi_check__v5_1
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v5_0
   SEC
   SBC   __zpabi_check__v0_0
   STA   __local_check__3
   LDA   __zpabi_check__v5_1
   SBC   __zpabi_check__v0_1
   STA   __local_check__2
   LDA   __local_check__1
   SBC   __zpabi_check__v0_2
   STA   __local_check__1
   LDA   __local_check__0
   SBC   __zpabi_check__v0_3
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   #$FF
   LDA   __local_check__2
   SBC   #$FF
   LDA   __local_check__1
   SBC   #$FF
   LDA   __local_check__0
   SBC   #$FF
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   JSR   check@run0
   SEC
   SBC   __local_check__4
   LDA   #$00
   SBC   #$00
   BVC   .jcmp_novf@12
   EOR   #$80
.jcmp_novf@12:
   BPL   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v3_1
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v0_0
   STA   __local_check__3
   LDA   __zpabi_check__v3_1
   ORA   __zpabi_check__v0_1
   STA   __local_check__2
   LDA   __local_check__1
   ORA   __zpabi_check__v0_2
   STA   __local_check__1
   LDA   __local_check__0
   ORA   __zpabi_check__v0_3
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   __zpabi_check__v4_0
   LDA   __local_check__2
   SBC   __zpabi_check__v4_1
   LDA   __local_check__1
   SBC   #$00
   LDA   __local_check__0
   SBC   #$00
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   JSR   check@run0
   STA   __local_check__0
   LDA   #$02
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   JSR   check@run0
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@17
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@17:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v3_1
   CMP   #$80
   BNE   .jcmp_skip@18
   LDA   __zpabi_check__v3_0
   BEQ   .if_end@4
.jcmp_skip@18:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v4_0
   SEC
   SBC   __zpabi_check__v2
   LDA   __zpabi_check__v4_1
   SBC   __local_check__7
   LDA   #$00
   ROL
   STA   __local_check__5
   LDA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   __local_check__4
   LDA   __zpabi_check__v3_1
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$07
   SEC
   SBC   __zpabi_check__v3_0
   LDA   #$00
   SBC   __zpabi_check__v3_1
   LDA   #$00
   SBC   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   #$00
   AND   __local_check__2
   STA   HARGS+4
   LDA   __local_check__5
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_check__4
   STA   HARGS+2
   STA   HARGS+3
   JSR   asr32
   LDA   HARGS+8
   BNE   .jcmp_skip@25
   LDA   HARGS+7
   BNE   .jcmp_skip@25
   LDA   HARGS+6
   BNE   .jcmp_skip@25
   LDA   HARGS+5
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@25:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v4_0
   CLC
   ADC   __zpabi_check__v1_0
   STA   __local_check__6
   LDA   __zpabi_check__v4_1
   ADC   __zpabi_check__v1_1
   STA   __local_check__5
   LDA   #$00
   ADC   __zpabi_check__v1_2
   STA   __local_check__4
   LDA   #$00
   ADC   __zpabi_check__v1_3
   STA   __local_check__3
   LDA   __zpabi_check__v5_0
   CLC
   ADC   #$02
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   ADC   #$00
   STA   __local_check__0
   LDA   __local_check__1
   SEC
   SBC   #$FF
   LDA   __local_check__0
   SBC   #$00
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   JSR   check@run0
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@28
   LDA   #$FF
.sx_done@28:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__3
   CMP   __local_check__0
   BNE   .if_end@6
   LDA   __local_check__4
   CMP   __local_check__1
   BNE   .if_end@6
   LDA   __local_check__5
   BNE   .if_end@6
   LDA   __local_check__6
   CMP   __local_check__2
   BNE   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
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

   LDA   #$00
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v3_0
   LDA   #$0F
   STA   __zpabi_check__v1_0
   LDA   #$FF
   STA   __zpabi_check__v2
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   LDA   #$80
   STA   __zpabi_check__v3_1
   LDA   #$30
   STA   __zpabi_check__v5_0
   LDA   #$DF
   STA   __zpabi_check__v5_1
   JMP   check
