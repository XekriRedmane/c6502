SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f0__p1	EQU	$80
__zpabi_f1__p0_0	EQU	$81
__zpabi_f1__p0_1	EQU	$82
__zpabi_f1__p0_2	EQU	$83
__zpabi_f1__p1_0	EQU	$84
__zpabi_f1__p1_1	EQU	$85
__zpabi_f1__p1_2	EQU	$86
__zpabi_f1__p1_3	EQU	$87
__zpabi_f1__p2	EQU	$88
__local_main__0	EQU	$89
__local_main__2	EQU	$8B
__local_main__3	EQU	$8C
__local_f0__0	EQU	$90
__local_f0__it_1	EQU	$93
__local_f0__it_0	EQU	$94
__local_f0__l0_1	EQU	$95
__local_f0__l0_0	EQU	$96
__local_f0__3	EQU	$97
__local_f0__p0_1	EQU	$98
__local_f0__p0_0	EQU	$99
__local_f1__0	EQU	$9B
__local_f1__it_1	EQU	$9C
__local_f1__it_0	EQU	$9D
__local_f1__l0	EQU	$9E
__local_f2__0	EQU	$A0
__local_f2__1	EQU	$A1
__local_f2__2	EQU	$A2

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__it_1,__local_f0__it_0,__local_f0__l0_1,__local_f0__l0_0,__local_f0__3,__local_f0__p0_1,__local_f0__p0_0,__local_f0__4 indirect=false in_cycle=false param_regs=A,X,
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3,__zpabi_f1__p2 locals=__local_f1__0,__local_f1__it_1,__local_f1__it_0,__local_f1__l0,__local_f1__1 indirect=false in_cycle=false carry=true
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; call f0 -> f1
; call f1 -> f2
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$5C weight=12
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f2:
   SUBROUTINE

   LDA   g0
   STA   __local_f2__0
   AND   g0
   STA   __local_f2__2
   LDA   g0
   AND   #$07
   TAX
   LDA   #$00
   STA   __local_f2__1
   CPX   #$00
   BEQ   .shift_done@1
.shift_loop@0:
   ASL   __local_f2__0
   ROL   __local_f2__1
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   CMP   __local_f2__1
   BNE   .cmp_true@2
   LDA   __local_f2__2
   CMP   __local_f2__0
   BNE   .cmp_true@2
   LDA   #$00
   RTS
.cmp_true@2:
   LDA   #$01
   RTS

f1:
   SUBROUTINE

   LDA   g0
   AND   __zpabi_f1__p2
   STA   __local_f1__it_1
   LDA   g0
   AND   #$07
   TAX
   LDA   g0
   STA   __local_f1__0
   CPX   #$00
   BEQ   .shift_done@6
.shift_loop@5:
   LSR   __local_f1__0
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   __local_f1__it_1
   ORA   __local_f1__0
   AND   #$7F
   STA   __local_f1__l0
   LDA   #$00
   STA   __local_f1__it_0
   LDY   #$00
   STY   __local_f1__it_1
.loop@0_start:
   LDA   __local_f1__it_0
   SEC
   SBC   #$03
   LDA   __local_f1__it_1
   SBC   #$00
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BPL   .loop@0_break
   JSR   f2
   CLC
   ADC   __local_f1__it_0
   CLC
   ADC   __local_f1__l0
   AND   #$7F
   STA   __local_f1__l0
   INC   __local_f1__it_0
   BNE   .loop@0_start
   INC   __local_f1__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g0
   CLC
   ADC   __local_f1__l0
   STA   g0
   LDA   __zpabi_f1__p0_2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   ORA   __zpabi_f1__p0_2
   ORA   __zpabi_f1__p0_1
   ORA   __zpabi_f1__p0_0
   BEQ   .cmp_end@10
   SEC
   RTS
.cmp_end@10:
   CMP   #$01
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   STX   __local_f0__p0_1
   LDA   g0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f0__p0_0
   STA   HARGS
   STX   HARGS+1
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   STA   __local_f0__l0_0
   LDA   HARGS+5
   STA   __local_f0__l0_1
   LDA   __zpabi_f0__p1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_f0__3
   LDA   #$00
   STA   __local_f0__it_0
   LDY   #$00
   STY   __local_f0__it_1
.loop@1_start:
   LDA   __local_f0__it_0
   SEC
   SBC   #$03
   LDA   __local_f0__it_1
   SBC   #$00
   BVC   .jcmp_novf@13
   EOR   #$80
.jcmp_novf@13:
   BMI   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDA   __local_f0__l0_0
   SEC
   SBC   __zpabi_f0__p1
   LDA   __local_f0__l0_1
   SBC   __local_f0__3
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_f1__p0_0
   LDA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   STA   __zpabi_f1__p0_2
   LDA   __local_f0__l0_1
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __zpabi_f1__p1_2
   STA   __zpabi_f1__p1_3
   LDA   #$00
   AND   __zpabi_f0__p1
   AND   #$7F
   STA   __zpabi_f1__p2
   LDA   #$00
   STA   __zpabi_f1__p0_1
   LDA   __local_f0__l0_0
   STA   __zpabi_f1__p1_0
   LDA   __local_f0__l0_1
   STA   __zpabi_f1__p1_1
   JSR   f1
   LDA   #$00
   ROL
   CLC
   ADC   __local_f0__it_0
   STA   __zpabi_f1__p0_2
   LDA   #$00
   ADC   __local_f0__it_1
   STA   __local_f0__0
   LDA   __local_f0__l0_0
   CLC
   ADC   __zpabi_f1__p0_2
   STA   __local_f0__l0_0
   LDA   __local_f0__l0_1
   ADC   __local_f0__0
   STA   __local_f0__l0_1
   INC   __local_f0__it_0
   BEQ   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
   INC   __local_f0__it_1
   JMP   .loop@1_start
.loop@1_break:
   LDA   __local_f0__l0_0
   EOR   __local_f0__p0_0
   STA   __local_f0__it_1
   LDA   __local_f0__l0_1
   EOR   __local_f0__p0_1
   STA   __zpabi_f1__p0_0
   LDA   g0
   BEQ   .and_false@0
   LDA   #$01
   STA   __zpabi_f1__p0_2
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __zpabi_f1__p0_2
.and_end@1:
   LDA   __local_f0__it_1
   CLC
   ADC   __zpabi_f1__p0_2
   STA   HARGS+8
   LDA   __zpabi_f1__p0_0
   ADC   #$00
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$12
   STA   __zpabi_f0__p1
   LDX   #$00
   LDA   #$4D
   JSR   f0
   LDA   HARGS+8
   EOR   g0
   STA   __local_main__3
   LDA   HARGS+9
   STA   __local_main__2
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS
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

   LDA   #$5C
   STA   g0
   RTS
