SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f0__p0_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f0__p1_0	EQU	$82
__zpabi_f0__p1_1	EQU	$83
__zpabi_f1__p0_0	EQU	$84
__zpabi_f1__p0_1	EQU	$85
__zpabi_f1__p0_2	EQU	$86
__zpabi_f1__p0_3	EQU	$87
__zpabi_f1__p2_0	EQU	$88
__zpabi_f1__p2_1	EQU	$89
__local_main__0	EQU	$8A
__local_main__2	EQU	$8C
__local_main__3	EQU	$8D
__local_f0__0	EQU	$91
__local_f0__it_1	EQU	$93
__local_f0__it_0	EQU	$94
__local_f0__l1_0	EQU	$95
__local_f0__l1_1	EQU	$96
__local_f0__p2_1	EQU	$97
__local_f0__p2_0	EQU	$98
__local_f2__0	EQU	$99
__local_f2__1	EQU	$9A
__local_f2__2	EQU	$9B
__local_f2__3	EQU	$9C
__local_f2__4	EQU	$9D
__local_f2__5	EQU	$9E

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p2_0,__zpabi_f0__p2_1 locals=__local_f0__0,__local_f0__1,__local_f0__it_1,__local_f0__it_0,__local_f0__l1_0,__local_f0__l1_1,__local_f0__p2_1,__local_f0__p2_0 indirect=false in_cycle=false param_regs=,,,,A,X carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p2_0,__zpabi_f1__p2_1 locals=__local_f1__0 indirect=false in_cycle=false param_regs=,,,,A,X,, carry=true
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7 indirect=false in_cycle=false
; def f3 params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f2
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=4 at=auto def=true init=$A4,$00,$00,$00 weight=13
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f3:
   SUBROUTINE

   LDX   #$FF
   LDA   #$9D
   RTS

f2:
   SUBROUTINE

   LDA   g0
   ORA   #$01
   STA   HARGS+4
   LDA   g0+1
   STA   HARGS+5
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+6
   STA   HARGS+2
   LDA   g0
   STA   HARGS
   LDA   g0+3
   STA   HARGS+7
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+12
   STA   __local_f2__5
   LDA   g0
   AND   #$07
   STA   HARGS+4
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   JSR   lsr32
   LDA   HARGS+5
   STA   __local_f2__4
   LDA   g0
   STA   __local_f2__3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f2__2
   STA   HARGS+1
   LDA   __local_f2__3
   STA   HARGS
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   SEC
   SBC   __local_f2__3
   LDA   HARGS+7
   SBC   __local_f2__2
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f2__3
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_f2__2
   STA   __local_f2__0
   LDA   __local_f2__5
   SEC
   SBC   __local_f2__4
   STA   __local_f2__1
   LDA   __local_f2__3
   CLC
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   __local_f2__2
   SBC   #$00
   LDA   __local_f2__0
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f2__0
   LDA   __local_f2__1
   SEC
   SBC   __local_f2__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

f1:
   SUBROUTINE

   LDA   g0+3
   ORA   g0+2
   ORA   g0+1
   ORA   g0
   BEQ   .cmp_end@6
   SEC
   RTS
.cmp_end@6:
   CMP   #$01
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p2_0
   STX   __local_f0__p2_1
   LDA   __zpabi_f0__p0_0
   SEC
   SBC   #$FF
   LDA   __zpabi_f0__p0_1
   SBC   #$7F
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f0__it_1
   LDA   #$00
   STA   __local_f0__l1_1
   LDA   __zpabi_f0__p0_0
   CLC
   SBC   __local_f0__p2_0
   LDA   __zpabi_f0__p0_1
   SBC   __local_f0__p2_1
   LDA   #$00
   ROL
   STA   __zpabi_f1__p0_0
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __zpabi_f1__p0_2
   STA   __zpabi_f1__p0_3
   LDA   __zpabi_f0__p1_0
   EOR   #$64
   STA   __local_f0__0
   LDX   __zpabi_f0__p1_1
   LDA   #$00
   STA   __zpabi_f1__p0_1
   LDA   #$FF
   STA   __zpabi_f1__p2_0
   LDA   #$7F
   STA   __zpabi_f1__p2_1
   LDA   __local_f0__0
   JSR   f1
   BCC   .if_else@1
   LDA   __local_f0__it_1
   CLC
   ADC   #$03
   STA   __local_f0__l1_0
   BCC   .if_end@0
   INC   __local_f0__l1_1
   JMP   .if_end@0
.if_else@1:
   LDA   __local_f0__it_1
   EOR   #$05
   STA   __local_f0__l1_0
   LDA   #$00
   STA   __local_f0__l1_1
.if_end@0:
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
   BVC   .jcmp_novf@9
   EOR   #$80
.jcmp_novf@9:
   BPL   .loop@0_break
   JSR   f2
   STA   __zpabi_f1__p0_0
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f0__0
   LDA   __zpabi_f1__p0_0
   CLC
   ADC   __local_f0__it_0
   STA   __zpabi_f1__p0_0
   LDA   __local_f0__0
   ADC   __local_f0__it_1
   STA   __local_f0__0
   LDA   __local_f0__l1_0
   CLC
   ADC   __zpabi_f1__p0_0
   STA   __local_f0__l1_0
   LDA   __local_f0__l1_1
   ADC   __local_f0__0
   STA   __local_f0__l1_1
   INC   __local_f0__it_0
   BNE   .loop@0_start
   INC   __local_f0__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g0
   CLC
   ADC   __local_f0__l1_0
   STA   g0
   LDA   g0+1
   ADC   __local_f0__l1_1
   STA   g0+1
   BCC   .carry_inc@1
   INC   g0+2
   BNE   .carry_inc@1
   INC   g0+3
.carry_inc@1:
   LDA   __local_f0__p2_1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_f1__p0_0
   STA   __local_f0__0
   LDA   g0
   SEC
   SBC   __local_f0__p2_0
   LDA   g0+1
   SBC   __local_f0__p2_1
   LDA   g0+2
   SBC   __zpabi_f1__p0_0
   LDA   g0+3
   SBC   __local_f0__0
   LDA   #$00
   ROL
   STA   __local_f0__0
   LDA   __zpabi_f0__p1_0
   SEC
   SBC   __local_f0__0
   LDA   __zpabi_f0__p1_1
   SBC   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$38
   STA   __zpabi_f0__p0_0
   LDA   #$8F
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p1_1
   LDX   #$00
   LDA   #$05
   JSR   f0
   LDA   #$00
   ROL
   EOR   g0
   STA   __local_main__3
   LDA   #$00
   EOR   g0+1
   STA   __local_main__2
   LDA   #$00
   EOR   g0+2
   STA   HARGS+2
   LDA   #$00
   EOR   g0+3
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
   LDA   #$A4
   STA   g0
   RTS
