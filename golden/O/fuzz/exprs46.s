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
__zpabi_check__v2_3	EQU	$87
__zpabi_check__v3_0	EQU	$88
__zpabi_check__v3_1	EQU	$89
__zpabi_check__v3_2	EQU	$8A
__zpabi_check__v3_3	EQU	$8B
__zpabi_check__v4	EQU	$8C
__zpabi_check__v5_0	EQU	$8D
__zpabi_check__v5_1	EQU	$8E
__zpabi_check__v5_2	EQU	$8F
__zpabi_check__v5_3	EQU	$90
__local_check__fail_1	EQU	$92
__local_check__fail_0	EQU	$93
__local_check__0	EQU	$94
__local_check__1	EQU	$95
__local_check__2	EQU	$96
__local_check__3	EQU	$97
__local_check__4	EQU	$98
__local_check__5	EQU	$99
__local_check__6	EQU	$9A
__local_check__7	EQU	$9B
__local_check__8	EQU	$9C
__local_check__9	EQU	$9D
__local_check__10	EQU	$9E
__local_check__v0	EQU	$9F

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__v0,__local_check__11,__local_check__12 indirect=false in_cycle=false param_regs=A,,,,,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=22
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
   STA   __local_check__10
   LDA   __zpabi_check__v4
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__9
   STA   __local_check__8
   STA   __local_check__7
   LDA   __zpabi_check__v3_3
   CMP   __local_check__7
   BNE   .thread@0
   LDA   __zpabi_check__v3_2
   CMP   __local_check__8
   BNE   .thread@0
   LDA   __zpabi_check__v3_1
   CMP   __local_check__9
   BNE   .thread@0
   LDA   __zpabi_check__v3_0
   CMP   __zpabi_check__v4
   BEQ   .cmp_true@4
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   TAX
   LDA   __local_check__v0
   CPX   #$00
   BEQ   .shift_done@8
.shift_loop@7:
   ASL
   DEX
   BNE   .shift_loop@7
.shift_done@8:
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __local_check__v0
   CMP   #$01
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   __zpabi_check__v5_0
   CLC
   SBC   #$01
   LDA   __zpabi_check__v5_1
   SBC   #$00
   LDA   __zpabi_check__v5_2
   SBC   #$00
   LDA   __zpabi_check__v5_3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@9
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v5_3
   CMP   #$D3
   BNE   .jcmp_skip@10
   LDA   __zpabi_check__v5_2
   CMP   #$39
   BNE   .jcmp_skip@10
   LDA   __zpabi_check__v5_1
   CMP   #$3B
   BNE   .jcmp_skip@10
   LDA   __zpabi_check__v5_0
   CMP   #$0F
   BEQ   .if_end@6
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   __local_check__v0
   ASL
   STA   __local_check__0
   LDA   __local_check__10
   ROL
   ASL   __local_check__0
   ROL
   ASL   __local_check__0
   ROL
   STA   __local_check__1
   LDA   __local_check__0
   SEC
   SBC   __local_check__v0
   STA   __local_check__2
   LDA   __local_check__1
   SBC   __local_check__10
   STA   __local_check__1
   LDA   __zpabi_check__v3_3
   CMP   __zpabi_check__v2_3
   BNE   .cmp_true@11
   LDA   __zpabi_check__v3_2
   CMP   __zpabi_check__v2_2
   BNE   .cmp_true@11
   LDA   __zpabi_check__v3_1
   CMP   __zpabi_check__v2_1
   BNE   .cmp_true@11
   LDA   __zpabi_check__v3_0
   CMP   __zpabi_check__v2_0
   BNE   .cmp_true@11
   LDA   #$00
   BEQ   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   SEC
   SBC   __local_check__2
   LDA   #$00
   SBC   __local_check__1
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   JSR   check@run0
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   STA   __local_check__5
   STA   __local_check__4
   LDA   __local_check__v0
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_check__v0
   STA   HARGS
   LDA   __zpabi_check__v1_0
   STA   HARGS+4
   LDA   __zpabi_check__v1_1
   STA   HARGS+5
   LDA   __zpabi_check__v1_2
   STA   HARGS+6
   LDA   __zpabi_check__v1_3
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   EOR   #$FF
   STA   __local_check__3
   LDA   HARGS+9
   EOR   #$FF
   STA   __local_check__2
   LDA   HARGS+10
   EOR   #$FF
   STA   __local_check__1
   LDA   HARGS+11
   EOR   #$FF
   STA   __local_check__0
   LDA   __local_check__6
   SEC
   SBC   __local_check__3
   LDA   #$00
   SBC   __local_check__2
   LDA   __local_check__5
   SBC   __local_check__1
   LDA   __local_check__4
   SBC   __local_check__0
   BVC   .jcmp_novf@19
   EOR   #$80
.jcmp_novf@19:
   BPL   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v4
   STA   __local_check__3
   LDA   __zpabi_check__v3_1
   SBC   __local_check__9
   STA   __local_check__2
   LDA   __zpabi_check__v3_2
   SBC   __local_check__8
   STA   __local_check__1
   LDA   __zpabi_check__v3_3
   SBC   __local_check__7
   STA   __local_check__0
   LDA   #$96
   SEC
   SBC   __local_check__3
   LDA   #$E9
   SBC   __local_check__2
   LDA   #$FF
   SBC   __local_check__1
   LDA   #$FF
   SBC   __local_check__0
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   JSR   check@run0
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   STA   __local_check__5
   STA   __local_check__4
   LDA   __local_check__v0
   SEC
   SBC   #$FF
   LDA   __local_check__10
   SBC   #$FF
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
   JSR   check@run0
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@25
   LDA   #$FF
.sx_done@25:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__3
   CLC
   ADC   __local_check__2
   STA   __local_check__3
   LDA   #$00
   ADC   #$00
   STA   __local_check__2
   LDA   __local_check__5
   ADC   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   ADC   __local_check__0
   BNE   .jcmp_skip@26
   LDA   __local_check__1
   BNE   .jcmp_skip@26
   LDA   __local_check__2
   BNE   .jcmp_skip@26
   LDA   __local_check__3
   CMP   #$02
   BEQ   .if_end@8
.jcmp_skip@26:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@1

check@run0:
   SUBROUTINE

   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$0E
   STA   __zpabi_check__v1_0
   LDA   #$00
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   LDA   #$01
   STA   __zpabi_check__v2_0
   LDA   #$F9
   STA   __zpabi_check__v3_0
   LDA   #$AA
   STA   __zpabi_check__v3_1
   LDA   #$7B
   STA   __zpabi_check__v3_2
   LDA   #$F2
   STA   __zpabi_check__v3_3
   LDA   #$80
   STA   __zpabi_check__v4
   LDA   #$0F
   STA   __zpabi_check__v5_0
   LDA   #$3B
   STA   __zpabi_check__v5_1
   LDA   #$39
   STA   __zpabi_check__v5_2
   LDA   #$D3
   STA   __zpabi_check__v5_3
   LDA   #$01
   JMP   check
