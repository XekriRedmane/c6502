SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f2__0	EQU	$80
__zpabi_f0__p0_0	EQU	$80
__local_f2__p0	EQU	$81
__zpabi_f0__p0_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f0__p0_3	EQU	$83
__zpabi_f0__p1_0	EQU	$84
__zpabi_f0__p1_1	EQU	$85
__zpabi_f0__p2	EQU	$86
__zpabi_f1__pp_0	EQU	$87
__zpabi_f1__pp_1	EQU	$88
__local_main__0	EQU	$89
__local_main__1	EQU	$8A
__local_f0__0	EQU	$8F
__local_f0__1	EQU	$90
__local_f0__2	EQU	$91
__local_f0__3	EQU	$92
__local_f0__it_1	EQU	$95
__local_f0__it_0	EQU	$96
__local_f0__l0_3	EQU	$97
__local_f0__l0_2	EQU	$98
__local_f0__l0_1	EQU	$99
__local_f0__l0_0	EQU	$9A
__local_f0__box1	EQU	$9B
__local_f1__0	EQU	$9D
__local_f1__1	EQU	$9E
__local_f1__2	EQU	$9F
__local_f1__3	EQU	$A0
__local_f1__4	EQU	$A1
__local_f1__5	EQU	$A2
__local_f1__6	EQU	$A3
__local_f1__7	EQU	$A4
__local_f1__p0_1	EQU	$A5
__local_f1__p0_0	EQU	$A6

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p2 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__it_1,__local_f0__it_0,__local_f0__l0_3,__local_f0__l0_2,__local_f0__l0_1,__local_f0__l0_0,__local_f0__box1,__local_f0__7 indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__p0_1,__local_f1__p0_0 indirect=false in_cycle=false param_regs=A,X,,
; def f2 params=__zpabi_f2__p0 locals=__local_f2__0,__local_f2__p0 indirect=false in_cycle=false param_regs=A
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5 indirect=false in_cycle=false
; call f0 -> f1
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$77,$00 weight=6
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f2:
   SUBROUTINE

   STA   __local_f2__p0
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f2__0
   LDA   __local_f2__p0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f2__0
   STA   HARGS+3
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BEQ   .lnot_true@2
   LDA   #$00
   BEQ   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   LDX   #$00
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p0_0
   STX   __local_f1__p0_1
   TXA
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f1__7
   STA   __local_f1__6
   LDA   __local_f1__p0_0
   SEC
   SBC   __local_f1__p0_0
   TXA
   SBC   __local_f1__p0_1
   LDA   __local_f1__7
   SBC   __local_f1__7
   LDA   __local_f1__6
   SBC   __local_f1__6
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   g0
   STA   HARGS+2
   LDA   g0+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   __local_f1__5
   LDA   HARGS+5
   STA   __local_f1__4
   LDY   #$00
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__3
   INY
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__2
   LDA   __local_f1__p0_0
   ORA   __local_f1__p0_1
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_f1__1
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_f1__1
.and_end@1:
   LDA   __local_f1__3
   CLC
   ADC   __local_f1__1
   STA   __local_f1__1
   LDA   __local_f1__2
   ADC   #$00
   LDY   #$01
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__1
   DEY
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__p0_0
   BEQ   .and_false@2
   LDA   __local_f1__5
   ORA   __local_f1__4
   BEQ   .and_false@2
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__1
   INY
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__0
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_f1__1
   STA   HARGS
   LDA   __local_f1__0
   STA   HARGS+1
   LDA   __local_f1__p0_0
   STA   HARGS+4
   LDA   __local_f1__p0_1
   STA   HARGS+5
   LDA   __local_f1__7
   STA   HARGS+6
   LDA   __local_f1__6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_f1__1
   LDA   #$00
   BEQ   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f1__1
.and_end@3:
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f1__1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f0:
   SUBROUTINE

   LDA   __zpabi_f0__p1_1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_f0__l0_2
   STA   __local_f0__l0_3
   LDA   __zpabi_f0__p1_0
   STA   __local_f0__box1
   STA   __local_f0__l0_0
   LDA   __zpabi_f0__p1_1
   STA   __local_f0__box1+1
   STA   __local_f0__l0_1
   LDA   #$00
   STA   __local_f0__it_0
   LDY   #$00
   STY   __local_f0__it_1
.loop@0_start:
   LDA   __local_f0__it_0
   SEC
   SBC   #$03
   LDA   __local_f0__it_1
   SBC   #$00
   BVC   .jcmp_novf@13
   EOR   #$80
.jcmp_novf@13:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   __local_f0__l0_0
   SEC
   SBC   #$07
   STA   __local_f0__2
   LDA   __local_f0__l0_1
   SBC   #$00
   TAX
   LDA   #<__local_f0__box1
   STA   __zpabi_f1__pp_0
   LDA   #>__local_f0__box1
   STA   __zpabi_f1__pp_1
   LDA   __local_f0__2
   JSR   f1
   LDA   __local_f0__it_1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   HARGS+8
   CLC
   ADC   __local_f0__it_0
   STA   __local_f0__3
   LDA   HARGS+9
   ADC   __local_f0__it_1
   STA   __local_f0__2
   LDA   HARGS+10
   ADC   __local_f0__1
   STA   __local_f0__1
   LDA   HARGS+11
   ADC   __local_f0__0
   STA   __local_f0__0
   LDA   __local_f0__l0_0
   CLC
   ADC   __local_f0__3
   STA   __local_f0__l0_0
   LDA   __local_f0__l0_1
   ADC   __local_f0__2
   STA   __local_f0__l0_1
   LDA   __local_f0__l0_2
   ADC   __local_f0__1
   STA   __local_f0__l0_2
   LDA   __local_f0__l0_3
   ADC   __local_f0__0
   STA   __local_f0__l0_3
   INC   __local_f0__it_0
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_f0__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f0__l0_0
   CLC
   ADC   __local_f0__box1
   STA   __local_f0__1
   LDA   __local_f0__l0_1
   ADC   __local_f0__box1+1
   STA   __local_f0__0
   LDA   g0
   CLC
   ADC   __local_f0__1
   STA   g0
   LDA   g0+1
   ADC   __local_f0__0
   STA   g0+1
   LDA   __zpabi_f0__p1_0
   ORA   #$01
   STA   __local_f0__1
   LDA   __zpabi_f0__p1_1
   STA   __local_f0__0
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_f0__p0_0
   STA   HARGS
   LDA   __zpabi_f0__p0_1
   STA   HARGS+1
   LDA   __zpabi_f0__p0_2
   STA   HARGS+2
   LDA   __zpabi_f0__p0_3
   STA   HARGS+3
   LDA   __local_f0__1
   STA   HARGS+4
   LDA   __local_f0__0
   STA   HARGS+5
   JSR   sdivmod32
   LDA   HARGS+8
   STA   __local_f0__3
   LDA   HARGS+9
   STA   __local_f0__2
   LDA   __zpabi_f0__p1_0
   STA   HARGS
   STA   HARGS+2
   LDA   __zpabi_f0__p1_1
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   ORA   __local_f0__3
   STA   __local_f0__0
   LDA   HARGS+5
   ORA   __local_f0__2
   TAX
   LDA   __local_f0__0
   RTS

main:
   SUBROUTINE

   LDA   #$F5
   STA   __zpabi_f0__p0_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   STA   __zpabi_f0__p1_1
   LDA   #$33
   STA   __zpabi_f0__p1_0
   LDA   #$30
   STA   __zpabi_f0__p2
   JSR   f0
   EOR   g0
   STA   __local_main__0
   TXA
   EOR   g0+1
   STA   __local_main__1
   LDA   __local_main__0
   LDX   __local_main__1
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$77
   STA   g0
   RTS
