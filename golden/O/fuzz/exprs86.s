SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v2_0	EQU	$80
__zpabi_check__v2_1	EQU	$81
__zpabi_check__v2_2	EQU	$82
__zpabi_check__v2_3	EQU	$83
__zpabi_check__v2_4	EQU	$84
__zpabi_check__v2_5	EQU	$85
__zpabi_check__v3_0	EQU	$86
__zpabi_check__v3_1	EQU	$87
__zpabi_check__v3_2	EQU	$88
__zpabi_check__v4	EQU	$89
__zpabi_check__v5_0	EQU	$8A
__zpabi_check__v5_1	EQU	$8B
__zpabi_check__v5_2	EQU	$8C
__zpabi_check__v5_3	EQU	$8D
__zpabi_check__v5_4	EQU	$8E
__zpabi_check__v5_5	EQU	$8F
__zpabi_check__v5_6	EQU	$90
__zpabi_check__v5_7	EQU	$91
__local_check__fail_1	EQU	$93
__local_check__fail_0	EQU	$94
__local_check__0	EQU	$95
__local_check__1	EQU	$96
__local_check__2	EQU	$97
__local_check__3	EQU	$98
__local_check__4	EQU	$99
__local_check__5	EQU	$9A
__local_check__6	EQU	$9B
__local_check__7	EQU	$9C
__local_check__8	EQU	$9D
__local_check__9	EQU	$9E
__local_check__10	EQU	$9F
__local_check__11	EQU	$A0
__local_check__12	EQU	$A1
__local_check__13	EQU	$A2
__local_check__14	EQU	$A3
__local_check__v0	EQU	$A4
__local_check__v1	EQU	$A5

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v2_4,__zpabi_check__v2_5,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3,__zpabi_check__v5_4,__zpabi_check__v5_5,__zpabi_check__v5_6,__zpabi_check__v5_7 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14,__local_check__v0,__local_check__v1,__local_check__15 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run4 section=.text local=true zero=false
; item check@run3 section=.text local=true zero=false
; item check@run2 section=.text local=true zero=false
; item check@run1 section=.text local=true zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=32
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   STX   __local_check__v1
   TXA
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
   CMP   #$FE
   BNE   .jcmp_skip@0
   CMP   #$1E
   BNE   .jcmp_skip@0
   CMP   #$34
   BNE   .jcmp_skip@0
   CMP   #$69
   BNE   .jcmp_skip@0
   CMP   #$AB
   BNE   .jcmp_skip@0
   CMP   #$1A
   BNE   .jcmp_skip@0
   CMP   #$23
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v4
   CMP   #$EA
   BEQ   .check@jthread@0
.jcmp_skip@0:
   LDA   #$FF
   EOR   __zpabi_check__v3_0
   STA   __local_check__2
   LDA   #$FF
   EOR   __zpabi_check__v3_1
   STA   __local_check__1
   LDA   #$01
   EOR   __zpabi_check__v3_2
   STA   __local_check__0
   LDA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .vthread@0
.check@jthread@0:
   LDA   __local_check__v0
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_check__2
   LDA   #$00
   CMP   __zpabi_check__v3_2
   BNE   .cmp_true@3
   CMP   __zpabi_check__v3_1
   BNE   .cmp_true@3
   LDA   #$02
   CMP   __zpabi_check__v3_0
   BNE   .cmp_true@3
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_check__0
   LDA   __local_check__v0
   EOR   __local_check__0
   ORA   __local_check__2
   BNE   .if_end@8
.vthread@0:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@8:
   LDA   #$FF
   CLC
   SBC   __zpabi_check__v2_0
   LDA   #$FF
   SBC   __zpabi_check__v2_1
   LDA   #$FF
   SBC   __zpabi_check__v2_2
   LDA   #$7F
   SBC   __zpabi_check__v2_3
   LDA   #$00
   SBC   __zpabi_check__v2_4
   LDA   #$00
   SBC   __zpabi_check__v2_5
   LDA   #$00
   ROL
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v5_0
   EOR   __local_check__6
   STA   __local_check__14
   LDA   __zpabi_check__v5_1
   STA   __local_check__13
   LDA   __zpabi_check__v5_2
   EOR   __local_check__5
   STA   __local_check__12
   LDA   __zpabi_check__v5_3
   EOR   __local_check__4
   STA   __local_check__11
   LDA   __zpabi_check__v5_4
   EOR   __local_check__3
   STA   __local_check__10
   LDA   __zpabi_check__v5_5
   EOR   __local_check__2
   STA   __local_check__9
   LDA   __zpabi_check__v5_6
   EOR   __local_check__1
   STA   __local_check__8
   LDA   __zpabi_check__v5_7
   EOR   __local_check__0
   STA   __local_check__7
   LDA   __local_check__v0
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__1
   LDA   #$01
   SEC
   SBC   __local_check__v0
   LDA   #$00
   SBC   __local_check__1
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   JSR   check@run3
   SEC
   SBC   __local_check__v0
   LDA   #$00
   SBC   __local_check__1
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   JSR   check@run3
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__14
   SEC
   SBC   __local_check__6
   LDA   __local_check__13
   SBC   #$00
   LDA   __local_check__12
   SBC   __local_check__5
   LDA   __local_check__11
   SBC   __local_check__4
   LDA   __local_check__10
   SBC   __local_check__3
   LDA   __local_check__9
   SBC   __local_check__2
   LDA   __local_check__8
   SBC   __local_check__1
   LDA   __local_check__7
   SBC   __local_check__0
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   JSR   check@run3
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@15
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@9:
   TXA
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_check__v0
   TXA
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_check__14
   STA   __local_check__13
   STA   __local_check__12
   STA   __local_check__11
   STA   __local_check__10
   STA   __local_check__9
   STA   __local_check__8
   LDA   #$FF
   ORA   __local_check__v1
   STA   __local_check__7
   LDA   #$7F
   ORA   __local_check__14
   STA   __local_check__6
   LDA   __local_check__13
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   TXA
   SEC
   SBC   __local_check__7
   LDA   __local_check__14
   SBC   __local_check__6
   LDA   __local_check__13
   SBC   __local_check__5
   LDA   __local_check__12
   SBC   __local_check__4
   LDA   __local_check__11
   SBC   __local_check__3
   LDA   __local_check__10
   SBC   __local_check__2
   LDA   __local_check__9
   SBC   __local_check__1
   LDA   __local_check__8
   SBC   __local_check__0
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__4
   LDA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   LDX   __local_check__4
   STX   HARGS
   LDX   #$00
   JSR   check@run0
   JSR   check@run2
   LDY   #$01
   STY   HARGS+8
   DEY
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   JSR   check@run4
   LDA   HARGS+21
   AND   #$03
   BNE   .jcmp_skip@23
   LDA   HARGS+20
   BNE   .jcmp_skip@23
   LDA   HARGS+19
   BNE   .jcmp_skip@23
   LDA   HARGS+18
   BNE   .jcmp_skip@23
   LDA   HARGS+17
   BNE   .jcmp_skip@23
   LDA   HARGS+16
   CMP   #$01
   BEQ   .if_end@10
.jcmp_skip@23:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@10:
   LDA   __local_check__v0
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDX   __local_check__v1
   STX   HARGS
   LDX   __local_check__v0
   JSR   check@run0
   LDA   __zpabi_check__v2_0
   STA   HARGS+8
   LDA   __zpabi_check__v2_1
   STA   HARGS+9
   LDA   __zpabi_check__v2_2
   STA   HARGS+10
   LDA   __zpabi_check__v2_3
   STA   HARGS+11
   LDA   __zpabi_check__v2_4
   STA   HARGS+12
   LDA   __zpabi_check__v2_5
   STA   HARGS+13
   JSR   check@run2
   JSR   check@run4
   LDA   HARGS+16
   STA   HARGS
   LDA   HARGS+17
   STA   HARGS+1
   LDA   HARGS+18
   STA   HARGS+2
   LDA   HARGS+19
   STA   HARGS+3
   LDA   HARGS+20
   STA   HARGS+4
   LDA   HARGS+21
   AND   #$03
   STA   HARGS+5
   JSR   check@run2
   LDA   __zpabi_check__v5_0
   STA   HARGS+8
   LDA   __zpabi_check__v5_1
   STA   HARGS+9
   LDA   __zpabi_check__v5_2
   STA   HARGS+10
   LDA   __zpabi_check__v5_3
   STA   HARGS+11
   LDA   __zpabi_check__v5_4
   STA   HARGS+12
   LDA   __zpabi_check__v5_5
   STA   HARGS+13
   LDA   __zpabi_check__v5_6
   STA   HARGS+14
   LDA   __zpabi_check__v5_7
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   ORA   HARGS+25
   ORA   HARGS+26
   ORA   HARGS+27
   ORA   HARGS+28
   ORA   HARGS+29
   ORA   HARGS+30
   ORA   HARGS+31
   BNE   .check@ssa_block@14
   JSR   check@run1
   BEQ   .if_end@13
.check@ssa_block@14:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@13:
   LDA   #$00
   CMP   __zpabi_check__v5_7
   BNE   .cmp_true@26
   CMP   __zpabi_check__v5_6
   BNE   .cmp_true@26
   CMP   __zpabi_check__v5_5
   BNE   .cmp_true@26
   CMP   __zpabi_check__v5_4
   BNE   .cmp_true@26
   CMP   __zpabi_check__v5_3
   BNE   .cmp_true@26
   CMP   __zpabi_check__v5_2
   BNE   .cmp_true@26
   CMP   __zpabi_check__v5_1
   BNE   .cmp_true@26
   LDA   __zpabi_check__v4
   CMP   __zpabi_check__v5_0
   BNE   .cmp_true@26
   LDA   #$00
   BEQ   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   STA   __local_check__0
   LDA   #$01
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   #$00
   BVC   .jcmp_novf@29
   EOR   #$80
.jcmp_novf@29:
   BPL   .if_end@18
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@18:
   JSR   check@run1
   BEQ   .if_end@19
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@19:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .jcmp_skip@0

check@run4:
   SUBROUTINE

   STA   HARGS+14
   STA   HARGS+15
   JMP   mul64

check@run3:
   SUBROUTINE

   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

check@run2:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   RTS

check@run1:
   SUBROUTINE

   LDA   __zpabi_check__v2_0
   ORA   __zpabi_check__v2_1
   ORA   __zpabi_check__v2_2
   ORA   __zpabi_check__v2_3
   ORA   __zpabi_check__v2_4
   ORA   __zpabi_check__v2_5
   RTS

check@run0:
   SUBROUTINE

   STX   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   STA   HARGS+4
   AND   #$03
   STA   HARGS+5
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_check__v3_0
   LDA   #$FF
   STA   __zpabi_check__v4
   LDA   #$0C
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v2_4
   STA   __zpabi_check__v2_5
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   STA   __zpabi_check__v5_4
   STA   __zpabi_check__v5_5
   STA   __zpabi_check__v5_6
   STA   __zpabi_check__v5_7
   LDX   #$00
   LDA   #$80
   JMP   check
