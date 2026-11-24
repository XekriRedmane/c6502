SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f2__0	EQU	$80
__zpabi_f0__p1_0	EQU	$80
__zpabi_f1__p1_0	EQU	$80
__local_f2__1	EQU	$81
__zpabi_f0__p1_1	EQU	$81
__zpabi_f1__p1_1	EQU	$81
__local_f2__2	EQU	$82
__zpabi_f0__p1_2	EQU	$82
__zpabi_f1__p1_2	EQU	$82
__zpabi_f0__p1_3	EQU	$83
__zpabi_f1__p2_0	EQU	$83
__zpabi_f0__p2_0	EQU	$84
__zpabi_f1__p2_1	EQU	$84
__zpabi_f0__p2_1	EQU	$85
__zpabi_f1__p2_2	EQU	$85
__zpabi_f1__p2_3	EQU	$86
__zpabi_f1__p3_0	EQU	$87
__zpabi_f1__p3_1	EQU	$88
__zpabi_f1__pp_0	EQU	$89
__zpabi_f1__pp_1	EQU	$8A
__zpabi_f4__p1	EQU	$8B
__zpabi_f4__p2_0	EQU	$8C
__zpabi_f4__p2_1	EQU	$8D
__zpabi_f4__p3_0	EQU	$8E
__zpabi_f4__p3_1	EQU	$8F
__local_f1__0	EQU	$90
__local_main__0	EQU	$90
__local_f1__1	EQU	$91
__local_main__1	EQU	$91
__local_main__2	EQU	$92
__local_f1__3	EQU	$93
__local_main__3	EQU	$93
__local_f1__4	EQU	$94
__local_f1__5	EQU	$95
__local_main__5	EQU	$95
__local_f1__6	EQU	$96
__local_f1__7	EQU	$97
__local_f1__8	EQU	$98
__local_f0__1	EQU	$99
__local_f0__p0	EQU	$9A
__local_f1__10	EQU	$9A
__local_f1__11	EQU	$9B
__local_f1__12	EQU	$9C
__local_f0__4	EQU	$9D
__local_f1__p0	EQU	$9D
__local_f3__p0	EQU	$A1
__local_f3__0	EQU	$A2
__local_f3__1	EQU	$A3
__local_f3__2	EQU	$A4
__local_f3__3	EQU	$A5
__local_f3__4	EQU	$A6
__local_f4__0	EQU	$A7
__local_f4__p0	EQU	$A8
__local_f4__1	EQU	$A9
__local_f4__2	EQU	$AA

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2_0,__zpabi_f0__p2_1 locals=__local_f0__0,__local_f0__1,__local_f0__p0,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7 indirect=false in_cycle=false param_regs=A,,,,,,
; def f1 params=__zpabi_f1__p0,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3,__zpabi_f1__p3_0,__zpabi_f1__p3_1,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__8,__local_f1__9,__local_f1__10,__local_f1__11,__local_f1__12,__local_f1__p0 indirect=false in_cycle=false param_regs=A,,,,,,,,,,, carry=true
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false carry=true
; def f3 params=__zpabi_f3__p0 locals=__local_f3__p0,__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4 indirect=false in_cycle=false param_regs=A
; def f4 params=__zpabi_f4__p0,__zpabi_f4__p1,__zpabi_f4__p2_0,__zpabi_f4__p2_1,__zpabi_f4__p3_0,__zpabi_f4__p3_1 locals=__local_f4__0,__local_f4__p0,__local_f4__1,__local_f4__2 indirect=false in_cycle=false param_regs=A,,,,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; call f0 -> f3
; call f1 -> f3
; call f1 -> f4
; call f2 -> f4
; call f3 -> f4
; call main -> f0
; item f4 section=.text local=false zero=false
; item f4@tail0 section=.text local=true zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f1@run0 section=.text local=true zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$01 weight=21
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f4:
   SUBROUTINE

   STA   __local_f4__p0
   LDA   __zpabi_f4__p1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f4__0
   LDA   __local_f4__p0
   SEC
   SBC   __zpabi_f4__p1
   LDA   #$00
   SBC   __local_f4__0
   BVC   .jcmp_novf@2
   EOR   #$80
.jcmp_novf@2:
   BMI   .and_false@0
   LDX   #$01
   BNE   .and_end@1
.and_false@0:
   LDX   #$00
.and_end@1:
   LDA   __zpabi_f4__p2_0
   STA   __local_f4__1
   LDA   __zpabi_f4__p2_1
   STA   __local_f4__2
   CPX   #$00
   BEQ   .shift_done@4
.shift_loop@3:
   LSR   __local_f4__2
   ROR   __local_f4__1
   DEX
   BNE   .shift_loop@3
.shift_done@4:
   LDA   g0
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f4__0
   LDA   __zpabi_f4__p2_0
   EOR   g0
   STA   __local_f4__p0
   LDA   __zpabi_f4__p2_1
   EOR   __local_f4__0
   STA   __local_f4__0
   LDA   __local_f4__p0
   SEC
   SBC   __local_f4__1
   LDA   __local_f4__0
   SBC   __local_f4__2
   LDA   #$00
   ROL
   STA   __local_f4__p0
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   ORA   __local_f4__p0
   JMP   f4@tail0

f4@tail0:
   SUBROUTINE

   BEQ   .cmp_end@9
   SEC
   RTS
.cmp_end@9:
   CMP   #$01
   RTS

f3:
   SUBROUTINE

   STA   __local_f3__p0
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f3__4
   STA   __local_f3__3
   STA   __local_f3__1
   ORA   __local_f3__p0
   ORA   __local_f3__3
   ORA   __local_f3__1
   BEQ   .lnot_true@12
   LDA   #$00
   BEQ   .lnot_end@13
.lnot_true@12:
   LDA   #$01
.lnot_end@13:
   STA   __local_f3__2
   LDA   #$00
   CMP   __local_f3__1
   BNE   .cmp_end@15
   CMP   __local_f3__3
   BNE   .cmp_end@15
   CMP   __local_f3__4
   BNE   .cmp_end@15
   CMP   __local_f3__p0
   BNE   .cmp_end@15
   LDA   #$01
.cmp_end@15:
   STA   __local_f3__1
   LDA   __local_f3__p0
   AND   #$07
   TAX
   LDA   __local_f3__p0
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_f3__0
   CPX   #$00
   BEQ   .shift_done@20
.shift_loop@19:
   ASL   __local_f3__p0
   ROL   __local_f3__0
   DEX
   BNE   .shift_loop@19
.shift_done@20:
   LDA   __local_f3__1
   EOR   __local_f3__p0
   STA   __zpabi_f4__p3_0
   LDA   #$00
   EOR   __local_f3__0
   STA   __zpabi_f4__p3_1
   LDA   #$E8
   STA   __zpabi_f4__p1
   LDA   #$00
   STA   __zpabi_f4__p2_0
   LDY   #$00
   STY   __zpabi_f4__p2_1
   JSR   f4
   LDA   __local_f3__2
   RTS

f2:
   SUBROUTINE

   LDA   g0
   BEQ   .and_false@6
   LDA   #$01
   STA   __zpabi_f4__p3_0
   JMP   .and_end@7
.and_false@6:
   LDY   #$00
   STY   __zpabi_f4__p3_0
.and_end@7:
   LDA   g0
   STA   __zpabi_f4__p1
   LDA   #$07
   STA   __zpabi_f4__p2_0
   LDA   #$00
   STA   __zpabi_f4__p2_1
   STA   __zpabi_f4__p3_1
   LDA   #$01
   JSR   f4
   LDA   g0
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_f2__2
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   #$00
   CMP   __local_f2__0
   BNE   .cmp_true@23
   CMP   __local_f2__1
   BNE   .cmp_true@23
   CMP   __local_f2__2
   BNE   .cmp_true@23
   LDA   #$04
   CMP   g0
   BNE   .cmp_true@23
   LDA   #$00
   BEQ   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   STA   __local_f2__0
   LDA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_f2__0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$05
   STY   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+15
   ORA   HARGS+14
   ORA   HARGS+13
   ORA   HARGS+12
   JMP   f4@tail0

f1:
   SUBROUTINE

   STA   __local_f1__p0
   LDA   __zpabi_f1__p2_1
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_f1__12
   LDA   __local_f1__p0
   BEQ   .and_false@10
   LDA   __zpabi_f1__p2_0
   ORA   __zpabi_f1__p2_1
   ORA   __local_f1__12
   BEQ   .and_false@10
   LDA   #$01
   STA   __local_f1__5
   JMP   .vthread@0
.and_false@10:
   LDA   #$00
   STA   __local_f1__5
.vthread@0:
   LDA   __zpabi_f1__p1_0
   AND   g0
   STA   __zpabi_f4__p3_0
   LDA   __local_f1__5
   EOR   __zpabi_f4__p3_0
   STA   __local_f1__11
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   __local_f1__1
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   __local_f1__0
   LDA   __local_f1__11
   SEC
   SBC   __zpabi_f1__p2_0
   LDA   __local_f1__1
   SBC   __zpabi_f1__p2_1
   LDA   __local_f1__0
   SBC   __local_f1__12
   BVC   .cmp_novf@39
   EOR   #$80
.cmp_novf@39:
   ASL
   LDA   #$00
   ROL
   STA   __local_f1__4
   LDA   #$00
   BPL   .sx_done@41
   LDA   #$FF
.sx_done@41:
   STA   __local_f1__3
   LDA   __local_f1__p0
   BMI   .sx_neg@42
   LDA   #$00
   BEQ   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   STA   __local_f1__10
   LDA   __zpabi_f1__p2_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f1__p2_1
   STA   HARGS+5
   LDA   __local_f1__12
   STA   __local_f1__0
   LDA   __local_f1__p0
   STA   HARGS
   LDA   __local_f1__10
   STA   HARGS+1
   STA   HARGS+2
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+3
   LDA   __local_f1__0
   STA   HARGS+6
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+10
   AND   #$7F
   EOR   #$40
   SEC
   SBC   #$40
   STA   __local_f1__0
   LDA   HARGS+8
   SEC
   SBC   __local_f1__4
   LDA   HARGS+9
   SBC   #$00
   LDA   __local_f1__0
   SBC   __local_f1__3
   BVC   .cmp_novf@44
   EOR   #$80
.cmp_novf@44:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f1__8
   LDA   #$00
   BPL   .sx_done@46
   LDA   #$FF
.sx_done@46:
   STA   __local_f1__7
   STA   __local_f1__6
   LDY   #$01
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__1
   LDA   #$07
   SEC
   SBC   __zpabi_f1__p2_0
   LDA   #$00
   SBC   __zpabi_f1__p2_1
   LDA   #$00
   SBC   __zpabi_f1__p2_2
   LDA   #$00
   SBC   __zpabi_f1__p2_3
   LDA   #$00
   ROL
   CLC
   DEY
   ADC   (__zpabi_f1__pp_0),Y
   BCC   .carry_inc@0
   INC   __local_f1__1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__1
   INY
   STA   (__zpabi_f1__pp_0),Y
   LDA   #$07
   JSR   f3
   STA   __zpabi_f4__p3_0
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   BEQ   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   STA   __local_f1__1
   STA   __local_f1__0
   LDA   __zpabi_f1__p2_0
   EOR   __zpabi_f4__p3_0
   STA   __local_f1__5
   LDA   __zpabi_f1__p2_1
   EOR   __local_f1__1
   STA   __local_f1__4
   LDA   __local_f1__12
   EOR   __local_f1__0
   STA   __local_f1__3
   LDA   g0
   CLC
   ADC   __local_f1__5
   JSR   f1@run0
   LDA   __local_f1__5
   SEC
   SBC   #$E8
   LDA   __local_f1__4
   SBC   #$03
   LDA   __local_f1__3
   SBC   #$00
   BVC   .cmp_novf@51
   EOR   #$80
.cmp_novf@51:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f1__4
   LDA   __zpabi_f1__p3_0
   SEC
   SBC   __local_f1__8
   LDA   __zpabi_f1__p3_1
   SBC   #$00
   LDA   #$00
   SBC   __local_f1__7
   LDA   #$00
   SBC   __local_f1__6
   LDA   #$00
   ROL
   STA   __local_f1__3
   LDA   __local_f1__11
   BMI   .sx_neg@52
   LDA   #$00
   BEQ   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   STA   __zpabi_f4__p2_1
   LDA   __zpabi_f1__p1_0
   CLC
   ADC   __local_f1__p0
   STA   __zpabi_f4__p3_0
   LDA   __zpabi_f1__p1_1
   ADC   __local_f1__10
   STA   __zpabi_f4__p3_1
   LDA   __local_f1__3
   STA   __zpabi_f4__p1
   LDA   __local_f1__11
   STA   __zpabi_f4__p2_0
   LDA   __local_f1__4
   JSR   f4
   CLC
   RTS

f1@run0:
   SUBROUTINE

   SEC
   AND   #$03
   EOR   #$02
   SBC   #$02
   STA   g0
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0
   LDA   g0
   EOR   g0
   STA   __local_f0__1
   LDA   __local_f0__p0
   EOR   __local_f0__p0
   AND   #$07
   TAX
   BEQ   .shift_done@55
.shift_loop@54:
   LDA   __local_f0__1
   ASL
   ROR   __local_f0__1
   DEX
   BNE   .shift_loop@54
.shift_done@55:
   LDA   __local_f0__1
   STA   __local_f0__4
   LDA   __local_f0__p0
   AND   #$07
   TAX
   BEQ   .shift_done@59
.shift_loop@58:
   LDA   __local_f0__p0
   ASL
   ROR   __local_f0__p0
   DEX
   BNE   .shift_loop@58
.shift_done@59:
   LDA   __local_f0__p0
   JSR   f3
   STA   __local_f0__p0
   LDA   __local_f0__4
   EOR   __local_f0__p0
   CLC
   ADC   g0
   JSR   f1@run0
   LDX   __zpabi_f0__p2_1
   LDA   __zpabi_f0__p2_0
   RTS

main:
   SUBROUTINE

   LDA   #$AB
   STA   __zpabi_f0__p1_0
   LDA   #$63
   STA   __zpabi_f0__p2_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   STA   __zpabi_f0__p2_1
   LDA   #$FF
   JSR   f0
   STA   __local_main__5
   LDA   g0
   BMI   .sx_neg@64
   LDA   #$00
   BEQ   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__5
   EOR   g0
   STA   __local_main__3
   TXA
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

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   g0
   RTS
