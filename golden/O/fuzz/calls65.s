SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__zpabi_f0__p0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f2__p0_0	EQU	$80
__zpabi_f3__p0_0	EQU	$80
__zpabi_f0__p2_0	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f2__p0_1	EQU	$81
__zpabi_f3__p0_1	EQU	$81
__zpabi_f0__p2_1	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__zpabi_f2__p0_2	EQU	$82
__zpabi_f3__p0_2	EQU	$82
__zpabi_f0__p2_2	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__zpabi_f2__p0_3	EQU	$83
__zpabi_f3__p0_3	EQU	$83
__zpabi_f0__p2_3	EQU	$84
__zpabi_f1__p1_0	EQU	$84
__zpabi_f2__p1	EQU	$84
__zpabi_f3__p1_0	EQU	$84
__local_main__0	EQU	$85
__zpabi_f1__p1_1	EQU	$85
__zpabi_f2__p2_0	EQU	$85
__zpabi_f3__p1_1	EQU	$85
__local_main__1	EQU	$86
__zpabi_f1__p1_2	EQU	$86
__zpabi_f2__p2_1	EQU	$86
__zpabi_f3__p1_2	EQU	$86
__local_main__2	EQU	$87
__zpabi_f1__p1_3	EQU	$87
__zpabi_f2__p2_2	EQU	$87
__zpabi_f3__p1_3	EQU	$87
__local_f3__0	EQU	$88
__local_main__3	EQU	$88
__zpabi_f1__p2	EQU	$88
__zpabi_f2__p2_3	EQU	$88
__local_f3__l0_0	EQU	$89
__zpabi_f2__p3	EQU	$89
__local_f3__1	EQU	$8A
__local_main__5	EQU	$8A
__zpabi_f2__pp_0	EQU	$8A
__local_f3__2	EQU	$8B
__zpabi_f2__pp_1	EQU	$8B
__local_f2__0	EQU	$8C
__local_f3__3	EQU	$8C
__local_f2__1	EQU	$8D
__local_f3__4	EQU	$8D
__local_f2__2	EQU	$8E
__local_f3__5	EQU	$8E
__local_f0__2	EQU	$8F
__local_f3__l0_1	EQU	$91
__local_f3__8	EQU	$92
__local_f4__0	EQU	$93
__local_f4__1	EQU	$94
__local_f4__2	EQU	$95
__local_f4__3	EQU	$96
__local_f4__4	EQU	$97
__local_f4__5	EQU	$98
__local_f4__p0_1	EQU	$99
__local_f4__p0_0	EQU	$9A
__local_f5__0	EQU	$9D
__local_f5__1	EQU	$9E
__local_f5__p0	EQU	$9F
__local_f5__2	EQU	$A0

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p2_2,__zpabi_f0__p2_3 locals=__local_f0__0,__local_f0__1,__local_f0__2 indirect=false in_cycle=false param_regs=,A,,,, carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3,__zpabi_f1__p2 locals=__local_f1__0 indirect=false in_cycle=false carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3,__zpabi_f2__p3,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2 indirect=false in_cycle=false carry=true
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p1_2,__zpabi_f3__p1_3 locals=__local_f3__0,__local_f3__l0_0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5,__local_f3__6,__local_f3__7,__local_f3__l0_1,__local_f3__8 indirect=false in_cycle=false
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__p0_1,__local_f4__p0_0,__local_f4__6,__local_f4__7 indirect=false in_cycle=false param_regs=A,X carry=true
; def f5 params=__zpabi_f5__p0 locals=__local_f5__0,__local_f5__1,__local_f5__p0,__local_f5__2 indirect=false in_cycle=false param_regs=A carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; call f3 -> f4
; call f3 -> f5
; call f4 -> f5
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f2@tail0 section=.text local=true zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$71 weight=9
; zp g0 size=2 at=auto def=true init=$83,$00 weight=9
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f5:
   SUBROUTINE

   STA   __local_f5__p0
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f5__2
   LDA   __local_f5__p0
   AND   #$07
   TAX
   LDA   #$00
   STA   __local_f5__0
   STA   __local_f5__1
   CPX   #$00
   BEQ   .shift_done@3
.shift_loop@2:
   ASL   __local_f5__0
   ROL   __local_f5__1
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDA   __local_f5__0
   AND   #$07
   TAX
   BEQ   .shift_done@5
.shift_loop@4:
   LDA   __local_f5__2
   ASL
   ROR   __local_f5__2
   ROR   __local_f5__p0
   DEX
   BNE   .shift_loop@4
.shift_done@5:
   LDA   __local_f5__2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f5__1
   STA   __local_f5__0
   LDA   __local_f5__p0
   ORA   __local_f5__2
   ORA   __local_f5__1
   ORA   __local_f5__0
   BEQ   .lnot_true@8
   LDA   #$00
   BEQ   .lnot_end@9
.lnot_true@8:
   LDA   #$01
.lnot_end@9:
   EOR   #$01
   STA   __local_f5__p0
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f5__1
   LDA   __local_f5__p0
   STA   __local_f5__0
   LDX   #$03
.shift_loop@12:
   LDA   __local_f5__1
   ASL
   ROR   __local_f5__1
   ROR   __local_f5__0
   DEX
   BNE   .shift_loop@12
   LDA   __local_f5__0
   ORA   __local_f5__1
   BEQ   .and_false@2
   LDA   __local_f5__p0
   BNE   .and_false@2
   SEC
   RTS
.and_false@2:
   CLC
   RTS

f4:
   SUBROUTINE

   STA   __local_f4__p0_0
   STX   __local_f4__p0_1
   LDA   g0
   AND   #$07
   TAX
   LDA   __local_f4__p0_0
   STA   __local_f4__3
   LDA   __local_f4__p0_1
   STA   __local_f4__4
   CPX   #$00
   BEQ   .shift_done@15
.shift_loop@14:
   ASL   __local_f4__3
   ROL   __local_f4__4
   DEX
   BNE   .shift_loop@14
.shift_done@15:
   LDA   __local_f4__3
   CLC
   ADC   g0
   STA   __local_f4__2
   LDA   __local_f4__4
   ADC   g0+1
   STA   __local_f4__1
   LDA   __local_f4__2
   JSR   f5
   LDA   #$00
   ROL
   STA   __local_f4__0
   LDA   __local_f4__2
   EOR   __local_f4__0
   STA   __local_f4__5
   LDA   __local_f4__1
   STA   __local_f4__4
   LDA   g0
   CLC
   ADC   __local_f4__5
   STA   g0
   LDA   g0+1
   ADC   __local_f4__4
   STA   g0+1
   LDA   __local_f4__p0_0
   AND   __local_f4__p0_0
   STA   __local_f4__1
   LDA   __local_f4__p0_1
   AND   __local_f4__p0_1
   ORA   __local_f4__1
   BEQ   .and_false@4
   LDA   __local_f4__5
   SEC
   SBC   #$E9
   LDA   __local_f4__4
   SBC   #$03
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   BCC   .and_false@4
   RTS
.and_false@4:
   CLC
   RTS

f3:
   SUBROUTINE

   LDA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   STA   __local_f3__l0_0
   STA   __local_f3__0
   LDA   g1
   SEC
   SBC   __zpabi_f3__p0_0
   LDA   #$00
   SBC   __zpabi_f3__p0_1
   LDA   __local_f3__l0_0
   SBC   __zpabi_f3__p0_2
   LDA   __local_f3__0
   SBC   __zpabi_f3__p0_3
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__1
   LDA   g1
   EOR   #$FF
   STA   __local_f3__l0_0
   LDA   #$00
   EOR   #$FF
   STA   __local_f3__0
   LDA   __local_f3__1
   AND   __local_f3__l0_0
   STA   __local_f3__8
   LDA   #$00
   AND   __local_f3__0
   STA   __local_f3__l0_1
   LDA   #$07
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_f3__p1_0
   STA   HARGS+4
   LDA   __zpabi_f3__p1_1
   STA   HARGS+5
   LDA   __zpabi_f3__p1_2
   STA   HARGS+6
   LDA   __zpabi_f3__p1_3
   STA   HARGS+7
   JSR   mul32
   LDA   #$00
   SEC
   SBC   HARGS+8
   STA   __local_f3__5
   LDA   #$00
   SBC   HARGS+9
   STA   __local_f3__4
   LDA   #$00
   SBC   HARGS+10
   STA   __local_f3__3
   LDA   #$00
   SBC   HARGS+11
   STA   __local_f3__2
   LDA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   STA   __local_f3__l0_0
   STA   __local_f3__0
   LDA   g1
   SEC
   SBC   __local_f3__5
   LDA   #$00
   SBC   __local_f3__4
   LDA   __local_f3__l0_0
   SBC   __local_f3__3
   LDA   __local_f3__0
   SBC   __local_f3__2
   LDA   #$00
   ROL
   JSR   f5
   LDX   __zpabi_f3__p1_1
   LDA   __zpabi_f3__p1_0
   JSR   f4
   BCC   .if_else@9
   LDA   __local_f3__8
   CLC
   ADC   #$03
   STA   __local_f3__l0_0
   BCC   .if_end@8
   INC   __local_f3__l0_1
   JMP   .if_end@8
.if_else@9:
   LDA   __local_f3__8
   EOR   #$05
   STA   __local_f3__l0_0
.if_end@8:
   LDA   __zpabi_f3__p1_0
   SEC
   SBC   __zpabi_f3__p1_0
   LDA   __zpabi_f3__p1_1
   SBC   __zpabi_f3__p1_1
   LDA   __zpabi_f3__p1_2
   SBC   __zpabi_f3__p1_2
   LDA   __zpabi_f3__p1_3
   SBC   __zpabi_f3__p1_3
   LDA   #$00
   ROL
   ORA   __local_f3__l0_0
   LDX   __local_f3__l0_1
   RTS

f2:
   SUBROUTINE

   LDA   __zpabi_f2__p2_0
   ORA   __zpabi_f2__p2_1
   ORA   __zpabi_f2__p2_2
   ORA   __zpabi_f2__p2_3
   BNE   .or_true@10
   LDA   __zpabi_f2__p0_0
   ORA   __zpabi_f2__p0_1
   ORA   __zpabi_f2__p0_2
   ORA   __zpabi_f2__p0_3
   BEQ   .or_end@11
.or_true@10:
.or_end@11:
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__1
   INY
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__2
   LDA   __zpabi_f2__p3
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_f2__0
   LDA   __local_f2__1
   CLC
   ADC   __zpabi_f2__p3
   STA   __local_f2__1
   LDA   __local_f2__2
   ADC   __local_f2__0
   LDY   #$01
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__1
   DEY
   STA   (__zpabi_f2__pp_0),Y
   LDA   __zpabi_f2__p3
   JMP   f2@tail0

f2@tail0:
   SUBROUTINE

   BEQ   .cmp_end@25
   SEC
   RTS
.cmp_end@25:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   LDA   g1
   JMP   f2@tail0

f0:
   SUBROUTINE

   LDA   g0
   ORA   g0+1
   BEQ   .and_false@12
   LDA   __zpabi_f0__p2_0
   ORA   __zpabi_f0__p2_1
   ORA   __zpabi_f0__p2_2
   ORA   __zpabi_f0__p2_3
   BEQ   .and_false@12
   LDA   #$00
   STA   __local_f0__2
   JMP   .and_end@13
.and_false@12:
   LDA   #$01
   STA   __local_f0__2
.and_end@13:
   SEC
   LDA   #$00
   SBC   __zpabi_f0__p2_0
   ORA   g1
   AND   #$07
   TAX
   BEQ   .shift_done@28
.shift_loop@27:
   LSR   __local_f0__2
   DEX
   BNE   .shift_loop@27
.shift_done@28:
   LDA   __local_f0__2
   CMP   #$01
   BEQ   .cmp_true@29
   CLC
   RTS
.cmp_true@29:
   SEC
   RTS

main:
   SUBROUTINE

   LDA   #$04
   STA   __zpabi_f0__p0
   LDA   #$64
   STA   __zpabi_f0__p2_0
   LDA   #$00
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p2_2
   STA   __zpabi_f0__p2_3
   LDA   #$02
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__5
   LDA   g0+1
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_main__1
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__5
   EOR   __local_main__3
   STA   __local_main__3
   LDA   #$00
   EOR   g0+1
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

   LDA   #$00
   STA   g0+1
   LDA   #$83
   STA   g0
   LDA   #$71
   STA   g1
   RTS
