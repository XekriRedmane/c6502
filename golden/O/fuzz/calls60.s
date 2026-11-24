SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$28
__local_f3__p0	EQU	$80
__zpabi_f4__p0_0	EQU	$80
__zpabi_f4__p0_1	EQU	$81
__zpabi_f4__p0_2	EQU	$82
__zpabi_f4__p0_3	EQU	$83
__local_f1__l2_3	EQU	$84
__local_main__0	EQU	$84
__local_main__1	EQU	$85
__local_f1__it_1	EQU	$86
__local_main__2	EQU	$86
__local_f1__it_0	EQU	$87
__local_main__3	EQU	$87
__local_f1__l0_1	EQU	$88
__local_main__4	EQU	$88
__local_f1__l0_0	EQU	$89
__local_main__5	EQU	$89
__local_f1__0	EQU	$8A
__local_f1__l2_0	EQU	$8B
__local_main__7	EQU	$8B
__local_f1__p0	EQU	$8C
__local_f0__0	EQU	$8E
__local_f0__l0	EQU	$8F
__local_f2__0	EQU	$91
__local_f2__1	EQU	$92
__local_f2__2	EQU	$93
__local_f2__3	EQU	$94
__local_f2__it_1	EQU	$97
__local_f2__it_0	EQU	$98
__local_f2__l1_3	EQU	$99
__local_f2__l1_2	EQU	$9A
__local_f2__l1_1	EQU	$9B
__local_f2__l1_0	EQU	$9C

; @zp-link-meta-begin
; def f0 params= locals=__local_f0__0,__local_f0__l0,__local_f0__1 indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0 locals=__local_f1__l2_3,__local_f1__l2_2,__local_f1__it_1,__local_f1__it_0,__local_f1__l0_1,__local_f1__l0_0,__local_f1__0,__local_f1__l2_0,__local_f1__p0,__local_f1__1,__local_f1__2,__local_f1__3 indirect=false in_cycle=false param_regs=A
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__it_1,__local_f2__it_0,__local_f2__l1_3,__local_f2__l1_2,__local_f2__l1_1,__local_f2__l1_0 indirect=false in_cycle=false carry=true
; def f3 params=__zpabi_f3__p0 locals=__local_f3__p0 indirect=false in_cycle=false param_regs=A
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p0_2,__zpabi_f4__p0_3 locals=__local_f4__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9 indirect=false in_cycle=false
; call f0 -> f2
; call f1 -> f3
; call f1 -> f4
; call f2 -> f4
; call main -> f0
; item f4 section=.text local=false zero=false
; item f4@tail0 section=.text local=true zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$3A,$00 weight=23
; zp g0 size=4 at=auto def=true init=$27,$00,$00,$00 weight=26
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f4:
   SUBROUTINE

   LDA   __zpabi_f4__p0_0
   JMP   f4@tail0

f4@tail0:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f3:
   SUBROUTINE

   STA   __local_f3__p0
   EOR   __local_f3__p0
   JMP   f4@tail0

f2:
   SUBROUTINE

   LDA   g1
   SEC
   SBC   g1
   STA   __local_f2__1
   LDA   g1+1
   SBC   g1+1
   STA   __local_f2__0
   LDA   __local_f2__1
   SEC
   SBC   g1
   LDA   __local_f2__0
   SBC   g1+1
   LDA   #$00
   ROL
   STA   __local_f2__l1_0
   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_f2__l1_2
   STA   __local_f2__l1_3
   LDA   #$00
   STA   __local_f2__l1_1
   STA   __local_f2__it_0
   LDY   #$00
   STY   __local_f2__it_1
.loop@0_start:
   LDA   __local_f2__it_0
   SEC
   SBC   #$03
   LDA   __local_f2__it_1
   SBC   #$00
   BVC   .jcmp_novf@2
   EOR   #$80
.jcmp_novf@2:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   g1+1
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   g0
   AND   g1
   STA   __zpabi_f4__p0_0
   LDA   g0+1
   AND   g1+1
   STA   __zpabi_f4__p0_1
   LDA   g0+2
   AND   __local_f2__1
   STA   __zpabi_f4__p0_2
   LDA   g0+3
   AND   __local_f2__0
   STA   __zpabi_f4__p0_3
   JSR   f4
   LDA   __local_f2__it_1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   HARGS+8
   CLC
   ADC   __local_f2__it_0
   STA   __local_f2__3
   LDA   HARGS+9
   ADC   __local_f2__it_1
   STA   __local_f2__2
   LDA   HARGS+10
   ADC   __local_f2__1
   STA   __local_f2__1
   LDA   HARGS+11
   ADC   __local_f2__0
   STA   __local_f2__0
   LDA   __local_f2__l1_0
   CLC
   ADC   __local_f2__3
   STA   __local_f2__l1_0
   LDA   __local_f2__l1_1
   ADC   __local_f2__2
   STA   __local_f2__l1_1
   LDA   __local_f2__l1_2
   ADC   __local_f2__1
   STA   __local_f2__l1_2
   LDA   __local_f2__l1_3
   ADC   __local_f2__0
   STA   __local_f2__l1_3
   INC   __local_f2__it_0
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_f2__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g1
   CLC
   ADC   __local_f2__l1_0
   STA   g1
   LDA   g1+1
   ADC   __local_f2__l1_1
   STA   g1+1
   ORA   g1
   BEQ   .cmp_end@7
   SEC
   RTS
.cmp_end@7:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p0
   LDA   g1
   SEC
   SBC   g1
   LDA   g1+1
   SBC   g1+1
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   EOR   #$FF
   STA   __local_f1__l0_0
   LDA   #$00
   EOR   #$FF
   STA   __local_f1__l0_1
   LDA   g0
   STA   __local_f1__l2_0
   LDA   #$00
   CMP   g0+3
   BNE   .cmp_true@9
   CMP   g0+2
   BNE   .cmp_true@9
   CMP   g0+1
   BNE   .cmp_true@9
   LDA   __local_f1__p0
   CMP   __local_f1__l2_0
   BNE   .cmp_true@9
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_f1__0
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   HARGS+2
   STA   HARGS+3
   LDA   g0
   CLC
   ADC   __local_f1__p0
   STA   __local_f1__it_0
   LDA   g0+1
   ADC   #$00
   STA   __local_f1__it_1
   LDA   g0+2
   ADC   #$00
   STA   __zpabi_f4__p0_2
   LDA   g0+3
   ADC   #$00
   STA   __local_f1__l2_3
   LDA   __local_f1__it_0
   ORA   #$01
   STA   HARGS+4
   LDA   __local_f1__it_1
   STA   HARGS+5
   LDA   __zpabi_f4__p0_2
   STA   HARGS+6
   LDA   __local_f1__l2_3
   STA   HARGS+7
   LDA   __local_f1__0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod32
   LDA   HARGS+8
   STA   __local_f1__0
   LDA   #$00
   STA   __local_f1__it_0
   LDY   #$00
   STY   __local_f1__it_1
.loop@1_start:
   LDA   __local_f1__it_0
   SEC
   SBC   #$03
   LDA   __local_f1__it_1
   SBC   #$00
   BVC   .jcmp_novf@14
   EOR   #$80
.jcmp_novf@14:
   BPL   .loop@1_break
   LDA   g0
   SEC
   SBC   __local_f1__p0
   LDA   g0+1
   SBC   #$00
   LDA   g0+2
   SBC   #$00
   LDA   g0+3
   SBC   #$00
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   JSR   f3
   LDA   HARGS+8
   CLC
   ADC   __local_f1__it_0
   STA   __zpabi_f4__p0_2
   LDA   HARGS+9
   ADC   __local_f1__it_1
   STA   __local_f1__l2_3
   LDA   __local_f1__l0_0
   CLC
   ADC   __zpabi_f4__p0_2
   STA   __local_f1__l0_0
   LDA   __local_f1__l0_1
   ADC   __local_f1__l2_3
   STA   __local_f1__l0_1
   INC   __local_f1__it_0
   BNE   .loop@1_start
   INC   __local_f1__it_1
   JMP   .loop@1_start
.loop@1_break:
   LDA   __local_f1__l0_1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __zpabi_f4__p0_2
   STA   __local_f1__l2_3
   LDA   g0
   CLC
   ADC   __local_f1__l0_0
   STA   g0
   LDA   g0+1
   ADC   __local_f1__l0_1
   STA   g0+1
   LDA   g0+2
   ADC   __zpabi_f4__p0_2
   STA   g0+2
   LDA   g0+3
   ADC   __local_f1__l2_3
   STA   g0+3
   LDA   __local_f1__l0_0
   STA   __zpabi_f4__p0_0
   LDA   __local_f1__l0_1
   STA   __zpabi_f4__p0_1
   LDA   __zpabi_f4__p0_2
   STA   __zpabi_f4__p0_3
   JSR   f4
   LDA   __local_f1__0
   EOR   HARGS+8
   STA   __local_f1__l2_3
   LDA   #$FF
   SEC
   SBC   __local_f1__l2_3
   LDA   #$7F
   SBC   #$00
   LDA   #$00
   ROL
   SEC
   SBC   __local_f1__l2_0
   RTS

f0:
   SUBROUTINE

   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .and_false@8
   LDA   #$01
   BNE   .and_end@9
.and_false@8:
   LDA   #$00
.and_end@9:
   SEC
   SBC   g1
   STA   __local_f0__l0
   JSR   f2
   BCC   .if_else@11
   LDA   __local_f0__l0
   CLC
   ADC   #$03
   STA   __local_f0__l0
   JMP   .if_end@10
.if_else@11:
   LDA   __local_f0__l0
   EOR   #$05
   STA   __local_f0__l0
.if_end@10:
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f0__0
   LDA   g1
   CLC
   ADC   __local_f0__l0
   STA   g1
   LDA   g1+1
   ADC   __local_f0__0
   STA   g1+1
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JSR   f0
   STA   __local_main__7
   TXA
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __local_main__5
   STA   __local_main__4
   LDA   g1+1
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   g0+2
   EOR   __local_main__1
   STA   __local_main__1
   LDA   g0+3
   EOR   __local_main__0
   STA   __local_main__0
   LDA   __local_main__7
   EOR   __local_main__3
   STA   __local_main__3
   TXA
   EOR   __local_main__2
   STA   __local_main__2
   LDA   __local_main__5
   EOR   __local_main__1
   STA   HARGS+2
   LDA   __local_main__4
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
   STA   g0+2
   STA   g0+3
   STA   g1+1
   LDA   #$27
   STA   g0
   LDA   #$3A
   STA   g1
   RTS
