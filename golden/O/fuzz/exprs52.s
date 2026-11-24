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
__zpabi_check__v2_0	EQU	$88
__zpabi_check__v2_1	EQU	$89
__zpabi_check__v2_2	EQU	$8A
__zpabi_check__v2_3	EQU	$8B
__zpabi_check__v2_4	EQU	$8C
__zpabi_check__v3_0	EQU	$8D
__zpabi_check__v3_1	EQU	$8E
__zpabi_check__v3_2	EQU	$8F
__zpabi_check__v3_3	EQU	$90
__zpabi_check__v3_4	EQU	$91
__zpabi_check__v4_0	EQU	$92
__zpabi_check__v4_1	EQU	$93
__zpabi_check__v4_2	EQU	$94
__zpabi_check__v4_3	EQU	$95
__zpabi_check__v4_4	EQU	$96
__zpabi_check__v4_5	EQU	$97
__zpabi_check__v5_0	EQU	$98
__zpabi_check__v5_1	EQU	$99
__zpabi_check__v5_2	EQU	$9A
__zpabi_check__v5_3	EQU	$9B
__zpabi_check__v5_4	EQU	$9C
__local_check__fail_1	EQU	$9E
__local_check__fail_0	EQU	$9F
__local_check__0	EQU	$A0
__local_check__1	EQU	$A1
__local_check__2	EQU	$A2
__local_check__3	EQU	$A3
__local_check__4	EQU	$A4
__local_check__5	EQU	$A5
__local_check__9	EQU	$A9

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v2_4,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v3_4,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v4_4,__zpabi_check__v4_5,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3,__zpabi_check__v5_4 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=29
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v5_4
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v5_3
   CMP   #$70
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v5_2
   CMP   #$1C
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v5_1
   CMP   #$D3
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v5_0
   CMP   #$68
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
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_check__9
   LDA   #$27
   STA   HARGS
   LDA   #$38
   STA   HARGS+1
   LDA   #$D2
   STA   HARGS+2
   LDA   #$AB
   STA   HARGS+3
   LDA   #$FF
   JSR   check@run0
   LDA   __zpabi_check__v1_0
   STA   HARGS+8
   LDA   __zpabi_check__v1_1
   STA   HARGS+9
   LDA   __zpabi_check__v1_2
   STA   HARGS+10
   LDA   __zpabi_check__v1_3
   STA   HARGS+11
   LDA   __local_check__9
   STA   HARGS+12
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   #$80
   SEC
   SBC   __zpabi_check__v5_0
   LDA   #$00
   SBC   __zpabi_check__v5_1
   LDA   #$00
   SBC   __zpabi_check__v5_2
   LDA   #$00
   SBC   __zpabi_check__v5_3
   LDA   #$00
   SBC   __zpabi_check__v5_4
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   HARGS+24
   SEC
   SBC   __local_check__3
   LDA   HARGS+25
   SBC   #$00
   LDA   HARGS+26
   SBC   __local_check__2
   LDA   HARGS+27
   SBC   __local_check__1
   LDA   HARGS+28
   SBC   __local_check__0
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
   BMI   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v5_4
   BNE   .jcmp_skip@8
   LDA   __zpabi_check__v5_3
   CMP   #$70
   BNE   .jcmp_skip@8
   LDA   __zpabi_check__v5_2
   CMP   #$1C
   BNE   .jcmp_skip@8
   LDA   __zpabi_check__v5_1
   CMP   #$D3
   BNE   .jcmp_skip@8
   LDA   __zpabi_check__v5_0
   CMP   #$68
   BEQ   .if_end@4
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   #$FF
   SEC
   SBC   __zpabi_check__v1_0
   STA   __local_check__4
   LDA   #$FF
   SBC   __zpabi_check__v1_1
   STA   __local_check__3
   LDA   #$00
   SBC   __zpabi_check__v1_2
   STA   __local_check__2
   LDA   #$00
   SBC   __zpabi_check__v1_3
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__9
   AND   #$01
   EOR   #$01
   STA   __local_check__0
   DEC   __local_check__0
   LDA   __local_check__4
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BNE   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v1_0
   SEC
   SBC   __zpabi_check__v2_0
   STA   __local_check__4
   LDA   __zpabi_check__v1_1
   SBC   __zpabi_check__v2_1
   STA   __local_check__3
   LDA   __zpabi_check__v1_2
   SBC   __zpabi_check__v2_2
   STA   __local_check__2
   LDA   __zpabi_check__v1_3
   SBC   __zpabi_check__v2_3
   STA   __local_check__1
   LDA   __local_check__9
   SBC   __zpabi_check__v2_4
   AND   #$01
   EOR   #$01
   STA   __local_check__0
   DEC   __local_check__0
   LDA   __zpabi_check__v2_0
   SEC
   SBC   #$FF
   LDA   __zpabi_check__v2_1
   SBC   #$7F
   LDA   __zpabi_check__v2_2
   SBC   #$00
   LDA   __zpabi_check__v2_3
   SBC   #$00
   LDA   __zpabi_check__v2_4
   SBC   #$00
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   STA   HARGS+8
   LDA   __local_check__4
   STA   HARGS
   LDA   __local_check__3
   STA   HARGS+1
   LDA   __local_check__2
   STA   HARGS+2
   LDA   __local_check__1
   STA   HARGS+3
   LDA   __local_check__0
   JSR   check@run0
   JSR   asr64
   SEC
   LDA   #$00
   SBC   HARGS+9
   STA   __local_check__4
   LDA   #$00
   SBC   HARGS+10
   STA   __local_check__3
   LDA   #$00
   SBC   HARGS+11
   STA   __local_check__2
   LDA   #$00
   SBC   HARGS+12
   STA   __local_check__1
   LDA   #$00
   SBC   HARGS+13
   AND   #$01
   EOR   #$01
   SEC
   SBC   #$01
   BNE   .jcmp_skip@10
   LDA   __local_check__1
   CMP   #$40
   BNE   .jcmp_skip@10
   LDA   __local_check__2
   BNE   .jcmp_skip@10
   LDA   __local_check__3
   BNE   .jcmp_skip@10
   LDA   __local_check__4
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   __zpabi_check__v0_0
   ORA   __zpabi_check__v0_1
   ORA   __zpabi_check__v0_2
   ORA   __zpabi_check__v0_3
   BEQ   .if_end@10
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDA   #$FF
   EOR   __zpabi_check__v4_0
   STA   __local_check__5
   LDA   #$7F
   EOR   __zpabi_check__v4_1
   STA   __local_check__4
   LDA   #$00
   EOR   __zpabi_check__v4_2
   STA   __local_check__3
   LDA   #$00
   EOR   __zpabi_check__v4_3
   STA   __local_check__2
   LDA   #$00
   EOR   __zpabi_check__v4_4
   STA   __local_check__1
   LDA   #$00
   EOR   __zpabi_check__v4_5
   BNE   .jcmp_skip@11
   LDA   __local_check__1
   BNE   .jcmp_skip@11
   LDA   __local_check__2
   BNE   .jcmp_skip@11
   LDA   __local_check__3
   BNE   .jcmp_skip@11
   LDA   __local_check__4
   CMP   #$7F
   BNE   .jcmp_skip@11
   LDA   __local_check__5
   CMP   #$FE
   BEQ   .if_end@11
.jcmp_skip@11:
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

check@run0:
   SUBROUTINE

   STA   HARGS+4
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   RTS

main:
   SUBROUTINE

   LDA   #$80
   STA   __zpabi_check__v1_3
   LDA   #$02
   STA   __zpabi_check__v2_0
   LDA   #$FF
   STA   __zpabi_check__v3_4
   LDA   #$01
   STA   __zpabi_check__v4_0
   LDA   #$68
   STA   __zpabi_check__v5_0
   LDA   #$D3
   STA   __zpabi_check__v5_1
   LDA   #$1C
   STA   __zpabi_check__v5_2
   LDA   #$70
   STA   __zpabi_check__v5_3
   LDA   #$00
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v2_4
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   STA   __zpabi_check__v4_4
   STA   __zpabi_check__v4_5
   STA   __zpabi_check__v5_4
   JMP   check
