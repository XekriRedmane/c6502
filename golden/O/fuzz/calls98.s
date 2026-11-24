SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f0__p1_0	EQU	$80
__zpabi_f4__p1_0	EQU	$80
__zpabi_f0__p1_1	EQU	$81
__zpabi_f4__p1_1	EQU	$81
__zpabi_f0__p1_2	EQU	$82
__local_f4__p0_0	EQU	$83
__zpabi_f0__p1_3	EQU	$83
__local_f4__0	EQU	$84
__zpabi_f0__p1_4	EQU	$84
__local_f4__1	EQU	$85
__zpabi_f0__p1_5	EQU	$85
__zpabi_f0__p2	EQU	$86
__local_f1__0	EQU	$87
__zpabi_f2__p0_0	EQU	$87
__zpabi_f2__p0_1	EQU	$88
__local_f1__2	EQU	$89
__zpabi_f2__p0_2	EQU	$89
__zpabi_f2__p0_3	EQU	$8A
__zpabi_f2__p1_0	EQU	$8B
__zpabi_f2__p1_1	EQU	$8C
__zpabi_f2__p1_2	EQU	$8D
__zpabi_f2__p1_3	EQU	$8E
__zpabi_f2__p2	EQU	$8F
__local_main__0	EQU	$90
__local_main__1	EQU	$91
__local_main__2	EQU	$92
__local_main__3	EQU	$93
__local_main__4	EQU	$94
__local_main__5	EQU	$95
__local_main__6	EQU	$96
__local_main__7	EQU	$97
__local_main__8	EQU	$98
__local_f0__0	EQU	$9B
__local_f0__1	EQU	$9C
__local_f0__2	EQU	$9D
__local_f0__3	EQU	$9E
__local_f0__4	EQU	$9F
__local_f0__5	EQU	$A0
__local_f0__8	EQU	$A3
__local_f0__9	EQU	$A4
__local_f0__10	EQU	$A5
__local_f0__11	EQU	$A6
__local_f0__12	EQU	$A7
__local_f0__13	EQU	$A8
__local_f0__p0_1	EQU	$A9
__local_f0__p0_0	EQU	$AA
__local_f2__0	EQU	$AE
__local_f2__1	EQU	$AF
__local_f2__2	EQU	$B0
__local_f2__3	EQU	$B1
__local_f2__4	EQU	$B2
__local_f3__0	EQU	$B6
__local_f3__1	EQU	$B7
__local_f3__2	EQU	$B8
__local_f3__3	EQU	$B9
__local_f3__4	EQU	$BA

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p1_4,__zpabi_f0__p1_5,__zpabi_f0__p2 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__8,__local_f0__9,__local_f0__10,__local_f0__11,__local_f0__12,__local_f0__13,__local_f0__p0_1,__local_f0__p0_0,__local_f0__14,__local_f0__15,__local_f0__16 indirect=false in_cycle=false param_regs=A,X,,,,,,,
; def f1 params= locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3,__zpabi_f2__p2 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7 indirect=false in_cycle=false carry=true
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5 indirect=false in_cycle=false param_regs=A,X
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p1_0,__zpabi_f4__p1_1 locals=__local_f4__p0_1,__local_f4__p0_0,__local_f4__0,__local_f4__1 indirect=false in_cycle=false param_regs=A,X,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f2
; call f2 -> f3
; call main -> f0
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$5A weight=12
; runtime hargs_size=32 hargs_need=23
; @zp-link-meta-end

f4:
   SUBROUTINE

   STA   __local_f4__p0_0
   LDA   g2
   ORA   g2+1
   ORA   g2+2
   ORA   g2+3
   ORA   g2+4
   ORA   g2+5
   ORA   g2+6
   BNE   .or_true@0
   TXA
   ORA   __local_f4__p0_0
   BEQ   .or_end@1
.or_true@0:
.or_end@1:
   LDA   g0
   STA   __local_f4__0
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f4__1
   LDA   g1
   AND   #$07
   TAX
   BEQ   .shift_done@3
.shift_loop@2:
   ASL   __local_f4__0
   ROL   __local_f4__1
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDA   __local_f4__1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f4__p0_0
   ORA   __local_f4__p0_0
   ORA   __local_f4__1
   ORA   __local_f4__0
   BEQ   .cmp_end@6
   SEC
   RTS
.cmp_end@6:
   CMP   #$01
   RTS

f3:
   SUBROUTINE

   LDA   g0
   BMI   .thread@0
   LDA   #$00
   BEQ   .sx_done@13
.thread@0:
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   __local_f3__4
   STA   __local_f3__3
   STA   __local_f3__2
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   #$00
   SEC
   SBC   g2
   LDA   #$00
   SBC   g2+1
   LDA   __local_f3__4
   SBC   g2+2
   LDA   __local_f3__3
   SBC   g2+3
   LDA   __local_f3__2
   SBC   g2+4
   LDA   __local_f3__1
   SBC   g2+5
   LDA   __local_f3__0
   SBC   g2+6
   LDA   #$00
   ROL
   STA   __local_f3__0
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   LDA   __local_f3__0
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   RTS

f2:
   SUBROUTINE

   LDA   g0
   BEQ   .and_false@2
   LDA   __zpabi_f2__p2
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f2__4
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f2__3
   STA   __local_f2__2
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   g1+5
   CMP   __local_f2__0
   BNE   .jcmp_skip@20
   LDA   g1+4
   CMP   __local_f2__1
   BNE   .jcmp_skip@20
   LDA   g1+3
   CMP   __local_f2__2
   BNE   .jcmp_skip@20
   LDA   g1+2
   CMP   __local_f2__3
   BNE   .jcmp_skip@20
   LDA   g1+1
   CMP   __local_f2__4
   BNE   .jcmp_skip@20
   LDA   g1
   CMP   __zpabi_f2__p2
   BEQ   .and_false@2
.jcmp_skip@20:
   LDA   #$01
   STA   __local_f2__3
   LDA   #$00
   STA   __local_f2__2
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f2__3
   STA   __local_f2__2
.and_end@3:
   LDA   __zpabi_f2__p2
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_f2__0
   LDA   __zpabi_f2__p2
   ORA   #$01
   STA   __local_f2__1
   LDA   __local_f2__0
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_f2__p0_0
   STA   HARGS
   LDA   __zpabi_f2__p0_1
   STA   HARGS+1
   LDA   __zpabi_f2__p0_2
   STA   HARGS+2
   LDA   __zpabi_f2__p0_3
   STA   HARGS+3
   LDA   __local_f2__1
   STA   HARGS+4
   LDA   __local_f2__0
   STA   HARGS+5
   JSR   sdivmod32
   LDX   HARGS+13
   LDA   HARGS+12
   JSR   f3
   LDA   __local_f2__2
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_f2__p0_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f2__p0_1
   STA   HARGS+5
   LDA   __zpabi_f2__p0_2
   STA   HARGS+6
   LDA   __zpabi_f2__p0_3
   STA   HARGS+7
   LDA   __local_f2__3
   STA   HARGS
   LDA   __local_f2__2
   STA   HARGS+1
   JSR   sdivmod32
   LDA   HARGS+15
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   #$00
   SEC
   SBC   HARGS+12
   LDA   #$00
   SBC   HARGS+13
   LDA   #$00
   SBC   HARGS+14
   LDA   #$00
   SBC   HARGS+15
   LDA   #$00
   SBC   __local_f2__1
   LDA   #$00
   SBC   __local_f2__0
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   RTS

f1:
   SUBROUTINE

   LDA   g0
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   __local_f1__2
   LDA   g0
   BMI   .sx_neg@32
   LDA   #$00
   BEQ   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   STA   __local_f1__0
   LDA   g0
   SEC
   SBC   g0
   LDA   __local_f1__2
   SBC   __local_f1__0
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f1__0
   LDA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f1__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   STX   __local_f0__p0_1
   LDA   g0
   STA   __local_f0__13
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   __local_f0__12
   STA   __local_f0__11
   STA   __local_f0__10
   LDA   g1
   STA   __local_f0__9
   LDA   g1+1
   STA   __local_f0__8
   LDA   __local_f0__p0_0
   SEC
   SBC   g2
   STA   HARGS
   TXA
   SBC   g2+1
   STA   HARGS+1
   LDA   #$00
   SBC   g2+2
   STA   HARGS+2
   LDA   #$00
   SBC   g2+3
   STA   HARGS+3
   LDA   #$00
   SBC   g2+4
   STA   HARGS+4
   LDA   #$00
   SBC   g2+5
   STA   HARGS+5
   LDA   #$00
   SBC   g2+6
   STA   HARGS+6
   LDA   __local_f0__9
   STA   HARGS+8
   LDA   __local_f0__8
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+7
   STA   HARGS+10
   LDY   #$00
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   g0
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   __local_f0__2
   BMI   .sx_neg@41
   LDA   #$00
   BEQ   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   g0
   CLC
   SBC   HARGS+16
   LDA   __local_f0__2
   SBC   HARGS+17
   LDA   __local_f0__1
   SBC   HARGS+18
   LDA   __local_f0__0
   SBC   HARGS+19
   LDA   #$00
   ROL
   STA   __local_f0__5
   LDA   #$00
   BPL   .sx_done@44
   LDA   #$FF
.sx_done@44:
   STA   __local_f0__4
   STA   __local_f0__3
   LDA   #$03
   SEC
   SBC   __local_f0__9
   STA   __local_f0__2
   LDA   #$00
   SBC   __local_f0__8
   STA   __local_f0__1
   LDA   g0
   BMI   .sx_neg@45
   LDA   #$00
   BEQ   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   STA   HARGS+1
   LDA   __local_f0__p0_0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f0__p0_1
   STA   HARGS+3
   LDA   g0
   STA   HARGS
   JSR   udivmod16
   LDA   HARGS+6
   STA   __zpabi_f2__p1_0
   LDA   HARGS+7
   STA   __zpabi_f2__p1_1
   LDA   __local_f0__13
   SEC
   SBC   __local_f0__5
   LDA   __local_f0__12
   SBC   #$00
   LDA   __local_f0__11
   SBC   __local_f0__4
   LDA   __local_f0__10
   SBC   __local_f0__3
   LDA   #$00
   ROL
   STA   __local_f0__0
   LDA   #$03
   ORA   __local_f0__0
   STA   __zpabi_f2__p2
   LDA   __local_f0__2
   STA   __zpabi_f2__p0_0
   LDA   __local_f0__1
   STA   __zpabi_f2__p0_1
   LDA   #$00
   STA   __zpabi_f2__p0_2
   STA   __zpabi_f2__p0_3
   STA   __zpabi_f2__p1_2
   STA   __zpabi_f2__p1_3
   JSR   f2
   JSR   f1
   LDA   __local_f0__13
   EOR   HARGS+8
   STA   __local_f0__5
   LDA   __local_f0__12
   EOR   HARGS+9
   STA   __local_f0__4
   LDA   __local_f0__11
   EOR   HARGS+10
   STA   __local_f0__3
   LDA   __local_f0__10
   EOR   HARGS+11
   STA   __local_f0__2
   BMI   .sx_neg@47
   LDA   #$00
   BEQ   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   g1
   CLC
   ADC   __local_f0__5
   STA   __local_f0__5
   LDA   g1+1
   ADC   __local_f0__4
   STA   __local_f0__4
   LDA   g1+2
   ADC   __local_f0__3
   STA   __local_f0__3
   LDA   g1+3
   ADC   __local_f0__2
   STA   __local_f0__2
   LDA   g1+4
   ADC   __local_f0__1
   STA   __local_f0__1
   LDA   g1+5
   ADC   __local_f0__0
   AND   #$0F
   EOR   #$08
   LDX   __local_f0__5
   STX   g1
   LDX   __local_f0__4
   STX   g1+1
   LDX   __local_f0__3
   STX   g1+2
   LDX   __local_f0__2
   STX   g1+3
   LDX   __local_f0__1
   STX   g1+4
   SEC
   SBC   #$08
   STA   g1+5
   LDA   __zpabi_f0__p2
   AND   #$07
   TAX
   LDA   #$03
   STA   __local_f0__2
   LDA   #$00
   STA   __local_f0__3
   CPX   #$00
   BEQ   .shift_done@50
.shift_loop@49:
   ASL   __local_f0__2
   ROL   __local_f0__3
   DEX
   BNE   .shift_loop@49
.shift_done@50:
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   ORA   g1+4
   ORA   g1+5
   BEQ   .and_false@4
   LDA   #$01
   STA   __local_f0__1
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   STA   __local_f0__1
.and_end@5:
   LDA   __local_f0__3
   BNE   .thread@1
   LDA   __local_f0__2
   CMP   __local_f0__1
   BEQ   .cmp_true@51
.thread@1:
   LDA   #$00
   RTS
.cmp_true@51:
   LDA   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$20
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   STA   __zpabi_f0__p1_4
   STA   __zpabi_f0__p1_5
   LDA   #$18
   STA   __zpabi_f0__p2
   LDX   #$01
   LDA   #$08
   JSR   f0
   STA   __local_main__8
   LDA   g0
   BMI   .sx_neg@54
   LDA   #$00
   BEQ   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   STA   __local_main__6
   STA   __local_main__5
   STA   __local_main__4
   LDA   g0
   EOR   g1
   STA   __local_main__7
   LDA   __local_main__6
   EOR   g1+1
   STA   __local_main__6
   LDA   __local_main__5
   EOR   g1+2
   STA   __local_main__5
   LDA   __local_main__4
   EOR   g1+3
   STA   __local_main__4
   LDA   __local_main__7
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__6
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   g2+2
   STA   __local_main__1
   LDA   __local_main__4
   EOR   g2+3
   STA   __local_main__0
   LDA   __local_main__8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   #$00
   EOR   __local_main__2
   STA   __local_main__2
   LDA   #$00
   EOR   __local_main__1
   STA   HARGS+2
   LDA   #$00
   EOR   __local_main__0
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS
   LDA   __local_main__2
   STA   HARGS+1
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   __local_main__3
   EOR   HARGS+5
   STA   __local_main__0
   LDA   __local_main__2
   EOR   HARGS+6
   TAX
   LDA   __local_main__0
   RTS

g1:
   DC.B  $0F
   DS.B  5

g2:
   DC.B  $48
   DS.B  6

__zp_init:
   SUBROUTINE

   LDA   #$5A
   STA   g0
   RTS
