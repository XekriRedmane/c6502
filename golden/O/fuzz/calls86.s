SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
g1	EQU	$27
__local_main__0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__local_main__1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__local_main__2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__local_main__3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__local_main__4	EQU	$84
__zpabi_f1__p0_4	EQU	$84
__zpabi_f1__p0_5	EQU	$85
__zpabi_f1__p0_6	EQU	$86
__zpabi_f1__p0_7	EQU	$87
__local_main__8	EQU	$88
__zpabi_f1__p1	EQU	$88
__zpabi_f1__p2_0	EQU	$89
__zpabi_f1__p2_1	EQU	$8A
__zpabi_f1__pp_0	EQU	$8B
__zpabi_f1__pp_1	EQU	$8C
__zpabi_f2__p0_0	EQU	$8D
__zpabi_f2__p0_1	EQU	$8E
__zpabi_f2__p0_2	EQU	$8F
__local_f0__0	EQU	$90
__local_f1__0	EQU	$90
__local_f0__1	EQU	$91
__local_f1__it_1	EQU	$91
__local_f1__it_0	EQU	$92
__local_f0__it_1	EQU	$93
__local_f1__l0	EQU	$93
__local_f0__it_0	EQU	$94
__local_f0__l0	EQU	$95

; @zp-link-meta-begin
; def f0 params= locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__it_1,__local_f0__it_0,__local_f0__l0 indirect=false in_cycle=false carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p0_4,__zpabi_f1__p0_5,__zpabi_f1__p0_6,__zpabi_f1__p0_7,__zpabi_f1__p1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__it_1,__local_f1__it_0,__local_f1__l0,__local_f1__1,__local_f1__2 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2 locals=__local_f2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call f0 -> f2
; call f1 -> f2
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=1 at=auto def=true init=$B5 weight=15
; zp g1 size=1 at=auto def=true init=$C4 weight=13
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

f2:
   SUBROUTINE

   LDA   __zpabi_f2__p0_0
   EOR   g2
   RTS

f1:
   SUBROUTINE

   LDA   __zpabi_f1__p2_0
   SEC
   SBC   __zpabi_f1__p2_0
   LDA   __zpabi_f1__p2_1
   SBC   __zpabi_f1__p2_1
   LDA   #$00
   ROL
   STA   __local_f1__l0
   LDY   #$01
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__it_1
   LDA   g2
   AND   g1
   CLC
   DEY
   ADC   (__zpabi_f1__pp_0),Y
   BCC   .carry_inc@0
   INC   __local_f1__it_1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__it_1
   INY
   STA   (__zpabi_f1__pp_0),Y
   LDA   #$00
   STA   __local_f1__it_0
   DEY
   STY   __local_f1__it_1
.loop@0_start:
   LDA   __local_f1__it_0
   SEC
   SBC   #$03
   LDA   __local_f1__it_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   #$07
   STA   __zpabi_f2__p0_0
   LDA   #$00
   STA   __zpabi_f2__p0_1
   STA   __zpabi_f2__p0_2
   JSR   f2
   CLC
   ADC   __local_f1__it_0
   CLC
   ADC   __local_f1__l0
   STA   __local_f1__l0
   INC   __local_f1__it_0
   BNE   .loop@0_start
   INC   __local_f1__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g1
   CLC
   ADC   __local_f1__l0
   STA   g1
   LDA   __zpabi_f1__p1
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f1__0
   LDA   __zpabi_f1__p1
   SEC
   SBC   __zpabi_f1__p1
   LDA   __local_f1__0
   SBC   __local_f1__0
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   CLC
   ADC   __local_f1__l0
   RTS

f0:
   SUBROUTINE

   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   g0
   CLC
   SBC   g2
   LDA   g0+1
   SBC   #$00
   LDA   g0+2
   SBC   __local_f0__1
   LDA   g0+3
   SBC   __local_f0__0
   LDA   #$00
   ROL
   STA   __local_f0__0
   LDA   #$03
   AND   __local_f0__0
   STA   __local_f0__l0
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
   BVC   .jcmp_novf@6
   EOR   #$80
.jcmp_novf@6:
   BPL   .loop@1_break
   LDA   g1
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   HARGS+1
   LDA   g2
   STA   HARGS+2
   LDA   g1
   STA   HARGS
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDX   HARGS+4
   STX   __zpabi_f2__p0_0
   LDX   HARGS+5
   STX   __zpabi_f2__p0_1
   AND   #$01
   STA   __zpabi_f2__p0_2
   JSR   f2
   CLC
   ADC   __local_f0__it_0
   CLC
   ADC   __local_f0__l0
   STA   __local_f0__l0
   INC   __local_f0__it_0
   BNE   .loop@1_start
   INC   __local_f0__it_1
   JMP   .loop@1_start
.loop@1_break:
   CLC
   RTS

main:
   SUBROUTINE

   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__8
   LDA   g1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   __local_main__2
   STA   __local_main__2
   LDA   g0+2
   EOR   __local_main__1
   STA   __local_main__1
   LDA   g0+3
   EOR   __local_main__0
   STA   __local_main__4
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__4
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

g0:
   DC.L  $000000BE

__zp_init:
   SUBROUTINE

   LDA   #$C4
   STA   g1
   LDA   #$B5
   STA   g2
   RTS
