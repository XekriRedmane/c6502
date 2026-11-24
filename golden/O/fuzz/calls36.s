SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f1__p1_0	EQU	$80
__zpabi_f2__p0_0	EQU	$80
__zpabi_f4__p0_0	EQU	$80
__zpabi_f1__p1_1	EQU	$81
__zpabi_f2__p0_1	EQU	$81
__zpabi_f4__p0_1	EQU	$81
__zpabi_f1__p1_2	EQU	$82
__zpabi_f2__p2_0	EQU	$82
__zpabi_f4__p0_2	EQU	$82
__zpabi_f1__p1_3	EQU	$83
__zpabi_f2__p2_1	EQU	$83
__zpabi_f4__p0_3	EQU	$83
__zpabi_f1__p2_0	EQU	$84
__zpabi_f3__pp_0	EQU	$84
__zpabi_f4__p1	EQU	$84
__zpabi_f1__p2_1	EQU	$85
__zpabi_f3__pp_1	EQU	$85
__zpabi_f4__p2_0	EQU	$85
__local_main__0	EQU	$86
__zpabi_f4__p2_1	EQU	$86
__local_main__1	EQU	$87
__zpabi_f4__p2_2	EQU	$87
__local_main__2	EQU	$88
__zpabi_f4__p2_3	EQU	$88
__local_main__3	EQU	$89
__local_f0__0	EQU	$90
__local_f0__1	EQU	$91
__local_f0__2	EQU	$92
__local_f0__it_1	EQU	$93
__local_f0__it_0	EQU	$94
__local_f0__l1_1	EQU	$95
__local_f0__l1_0	EQU	$96
__local_f0__3	EQU	$97
__local_f0__4	EQU	$98
__local_f0__5	EQU	$99
__local_f0__p0	EQU	$9A
__local_f1__0	EQU	$9D
__local_f2__0	EQU	$9D
__local_f1__1	EQU	$9E
__local_f2__1	EQU	$9E
__local_f1__2	EQU	$9F
__local_f2__2	EQU	$9F
__local_f2__3	EQU	$A0
__local_f1__p0_1	EQU	$A1
__local_f2__p1	EQU	$A1
__local_f1__p0_0	EQU	$A2
__local_f2__box3	EQU	$A2
__local_f3__0	EQU	$A4
__local_f3__1	EQU	$A5
__local_f3__2	EQU	$A6
__local_f3__3	EQU	$A7
__local_f3__p0_1	EQU	$A8
__local_f3__p0_0	EQU	$A9

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__it_1,__local_f0__it_0,__local_f0__l1_1,__local_f0__l1_0,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__p0,__local_f0__6,__local_f0__7 indirect=false in_cycle=false param_regs=A
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3,__zpabi_f1__p2_0,__zpabi_f1__p2_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__p0_1,__local_f1__p0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p2_0,__zpabi_f2__p2_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__p1,__local_f2__box3,__local_f2__5 indirect=false in_cycle=false param_regs=,,A,X,,
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__pp_0,__zpabi_f3__pp_1 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__p0_1,__local_f3__p0_0 indirect=false in_cycle=false param_regs=A,X,, carry=true
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p0_2,__zpabi_f4__p0_3,__zpabi_f4__p1,__zpabi_f4__p2_0,__zpabi_f4__p2_1,__zpabi_f4__p2_2,__zpabi_f4__p2_3 locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9 indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f2
; call f2 -> f3
; call main -> f0
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f1@tail0 section=.text local=true zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$48 weight=25
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f4:
   SUBROUTINE

   LDA   __zpabi_f4__p2_0
   ORA   __zpabi_f4__p2_1
   ORA   __zpabi_f4__p2_2
   ORA   __zpabi_f4__p2_3
   BEQ   .and_false@0
   LDA   __zpabi_f4__p1
   BNE   .and_end@1
.and_false@0:
.and_end@1:
   LDA   __zpabi_f4__p0_0
   ORA   __zpabi_f4__p0_1
   ORA   __zpabi_f4__p0_2
   ORA   __zpabi_f4__p0_3
   BNE   .or_true@2
   LDA   __zpabi_f4__p2_0
   BEQ   .or_end@3
.or_true@2:
.or_end@3:
   LDA   __zpabi_f4__p1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f3:
   SUBROUTINE

   STA   __local_f3__p0_0
   STX   __local_f3__p0_1
   LDA   g0
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f3__0
   LDA   g0
   AND   __local_f3__p0_0
   STA   __local_f3__2
   LDA   __local_f3__0
   AND   __local_f3__p0_1
   STA   __local_f3__1
   LDA   g0
   BEQ   .lnot_true@2
   LDA   #$00
   BEQ   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   STA   __local_f3__0
   LDA   __local_f3__1
   BNE   .thread@0
   LDA   __local_f3__2
   CMP   __local_f3__0
   BEQ   .cmp_true@4
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   STA   __local_f3__3
   LDY   #$00
   LDA   (__zpabi_f3__pp_0),Y
   CLC
   ADC   __local_f3__p0_0
   STA   __local_f3__1
   INY
   LDA   (__zpabi_f3__pp_0),Y
   ADC   __local_f3__p0_1
   STA   (__zpabi_f3__pp_0),Y
   LDA   __local_f3__1
   DEY
   STA   (__zpabi_f3__pp_0),Y
   LDA   (__zpabi_f3__pp_0),Y
   STA   __local_f3__2
   INY
   LDA   (__zpabi_f3__pp_0),Y
   STA   __local_f3__0
   LDA   __local_f3__3
   SEC
   SBC   __local_f3__3
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f3__1
   LDA   __local_f3__2
   SEC
   SBC   #$02
   LDA   __local_f3__0
   SBC   #$00
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   AND   __local_f3__1
   CMP   #$01
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p1
   LDA   g0
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_f2__0
   LDA   g0
   CLC
   ADC   __zpabi_f2__p0_0
   STA   __local_f2__3
   LDA   __local_f2__0
   ADC   __zpabi_f2__p0_1
   STA   __local_f2__2
   LDA   g0
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_f2__0
   LDA   __local_f2__p1
   EOR   g0
   STA   __local_f2__1
   TXA
   EOR   __local_f2__0
   STA   __local_f2__0
   LDA   __local_f2__3
   SEC
   SBC   __local_f2__1
   LDA   __local_f2__2
   SBC   __local_f2__0
   LDA   #$00
   ROL
   STA   __local_f2__1
   LDA   __local_f2__p1
   SEC
   SBC   #$FF
   TXA
   SBC   #$7F
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   BCC   .and_false@4
   LDA   #$00
   AND   __local_f2__1
   BEQ   .and_false@4
   LDA   #$01
   STA   __local_f2__0
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   STA   __local_f2__0
.and_end@5:
   LDA   __local_f2__1
   STA   __local_f2__box3
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_f2__box3+1
   LDA   __local_f2__0
   ORA   __local_f2__p1
   LDY   #<__local_f2__box3
   STY   __zpabi_f3__pp_0
   LDY   #>__local_f2__box3
   STY   __zpabi_f3__pp_1
   JSR   f3
   LDA   __local_f2__p1
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f2__p1
   STA   HARGS+8
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p0_0
   STX   __local_f1__p0_1
   LDA   g0
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_f1__2
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_f1__1
   STA   __local_f1__0
   LDA   __zpabi_f1__p1_0
   SEC
   SBC   g0
   LDA   __zpabi_f1__p1_1
   SBC   __local_f1__2
   LDA   __zpabi_f1__p1_2
   SBC   __local_f1__1
   LDA   __zpabi_f1__p1_3
   SBC   __local_f1__0
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   SEC
   SBC   __local_f1__p0_0
   LDA   #$00
   SBC   __local_f1__p0_1
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   STA   __local_f1__0
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f1__0
   JMP   f1@tail0

f1@tail0:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0
   SEC
   LDA   #$00
   SBC   __local_f0__p0
   STA   __local_f0__it_1
   LDA   #$00
   SBC   #$00
   STA   __local_f0__2
   LDA   g0
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_f0__0
   LDA   __local_f0__2
   CMP   __local_f0__0
   BNE   .cmp_true@27
   LDA   __local_f0__it_1
   CMP   g0
   BNE   .cmp_true@27
   LDA   #$00
   BEQ   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   STA   __local_f0__5
   EOR   #$FF
   STA   __local_f0__l1_0
   LDA   #$00
   EOR   #$FF
   STA   __local_f0__l1_1
   LDA   g0
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   __local_f0__2
   LDA   __local_f0__l1_0
   SEC
   SBC   __local_f0__5
   STA   __local_f0__1
   LDA   __local_f0__l1_1
   SBC   #$00
   STA   __local_f0__0
   LDA   g0
   EOR   __local_f0__1
   STA   __local_f0__4
   LDA   __local_f0__2
   EOR   __local_f0__0
   STA   __local_f0__3
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
   BVC   .jcmp_novf@32
   EOR   #$80
.jcmp_novf@32:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   g0
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   HARGS+1
   LDA   __local_f0__4
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f0__3
   STA   HARGS+3
   LDA   g0
   STA   HARGS
   JSR   sdivmod16
   LDX   HARGS+5
   LDA   g0
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   __local_f0__0
   LDA   __local_f0__p0
   CLC
   ADC   g0
   STA   __zpabi_f1__p2_0
   LDA   #$00
   ADC   __local_f0__0
   STA   __zpabi_f1__p2_1
   LDA   #$FF
   STA   __zpabi_f1__p1_0
   LDA   #$00
   STA   __zpabi_f1__p1_1
   STA   __zpabi_f1__p1_2
   STA   __zpabi_f1__p1_3
   LDA   HARGS+4
   JSR   f1
   LDA   HARGS+8
   CLC
   ADC   __local_f0__it_0
   STA   __local_f0__1
   LDA   HARGS+9
   ADC   __local_f0__it_1
   STA   __local_f0__0
   LDA   __local_f0__l1_0
   CLC
   ADC   __local_f0__1
   STA   __local_f0__l1_0
   LDA   __local_f0__l1_1
   ADC   __local_f0__0
   STA   __local_f0__l1_1
   INC   __local_f0__it_0
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_f0__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f0__p0
   ORA   __local_f0__p0
   STA   __local_f0__0
   LDA   __local_f0__5
   CMP   #$64
   BEQ   .cmp_true@37
   LDA   #$00
   BEQ   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   STA   __zpabi_f2__p2_0
   LDA   __local_f0__l1_0
   STA   __zpabi_f2__p0_0
   LDA   #$00
   STA   __zpabi_f2__p0_1
   STA   __zpabi_f2__p2_1
   LDX   #$00
   LDA   __local_f0__0
   JSR   f2
   LDA   __local_f0__4
   EOR   HARGS+8
   STA   __local_f0__it_0
   LDA   __local_f0__3
   EOR   HARGS+9
   STA   __local_f0__it_1
   LDA   __local_f0__5
   ASL
   STA   __local_f0__1
   LDA   #$00
   STA   __local_f0__2
   ROL   __local_f0__2
   LDA   __local_f0__1
   CLC
   ADC   __local_f0__5
   BCC   .carry_inc@0
   INC   __local_f0__2
.carry_inc@0:
   ORA   __local_f0__2
   BNE   .or_true@8
   LDA   __local_f0__it_0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f0__it_1
   STA   HARGS+3
   LDA   __local_f0__5
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+4
   ORA   HARGS+5
   BNE   .or_true@8
   LDA   #$00
   STA   __local_f0__1
   JMP   .or_end@9
.or_true@8:
   LDA   #$01
   STA   __local_f0__1
   LDA   #$00
.or_end@9:
   ORA   #$00
   BPL   .sx_done@40
   LDA   #$FF
.sx_done@40:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f0__1
   JMP   f1@tail0

main:
   SUBROUTINE

   LDA   #$75
   JSR   f0
   LDA   g0
   BMI   .sx_neg@41
   LDA   #$00
   BEQ   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   HARGS+8
   EOR   g0
   STA   __local_main__3
   LDA   HARGS+9
   EOR   __local_main__2
   STA   __local_main__2
   LDA   HARGS+10
   EOR   __local_main__1
   STA   HARGS+2
   LDA   HARGS+11
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

   LDA   #$48
   STA   g0
   RTS
