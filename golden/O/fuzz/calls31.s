SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27
__local_f1__0	EQU	$80
__zpabi_f0__p1_0	EQU	$80
__zpabi_f2__p0_0	EQU	$80
__local_f1__1	EQU	$81
__zpabi_f0__p1_1	EQU	$81
__zpabi_f2__p0_1	EQU	$81
__zpabi_f0__p1_2	EQU	$82
__zpabi_f2__p0_2	EQU	$82
__zpabi_f0__p1_3	EQU	$83
__zpabi_f2__p0_3	EQU	$83
__local_f1__4	EQU	$84
__zpabi_f0__p2	EQU	$84
__zpabi_f2__p1_0	EQU	$84
__local_f1__pp	EQU	$85
__zpabi_f0__p3	EQU	$85
__zpabi_f2__p1_1	EQU	$85
__local_f1__5	EQU	$86
__local_main__0	EQU	$86
__zpabi_f2__p2	EQU	$86
__local_main__1	EQU	$87
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_main__5	EQU	$8B
__local_main__6	EQU	$8C
__local_f0__0	EQU	$8F
__local_f0__1	EQU	$90

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2,__zpabi_f0__p3 locals=__local_f0__0,__local_f0__1 indirect=false in_cycle=false param_regs=A,,,,,, carry=true
; def f1 params=__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__pp,__local_f1__5 indirect=false in_cycle=false param_regs=A,X
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p2 locals=__local_f2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$C3 weight=4
; zp g1 size=2 at=auto def=true init=$24,$00 weight=7
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f2:
   SUBROUTINE

   LDA   g1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__pp
   STX   __local_f1__5
   LDA   g1+1
   LSR
   STA   __local_f1__1
   LDA   g1
   ROR
   AND   g1
   STA   __local_f1__4
   LDA   g1+1
   AND   __local_f1__1
   STA   __local_f1__0
   LDA   __local_f1__4
   ORA   g1
   STA   __local_f1__1
   LDA   __local_f1__0
   ORA   g1+1
   STA   __local_f1__0
   LDY   #$00
   LDA   (__local_f1__pp),Y
   CLC
   ADC   __local_f1__1
   STA   __local_f1__1
   INY
   LDA   (__local_f1__pp),Y
   ADC   __local_f1__0
   STA   (__local_f1__pp),Y
   LDA   __local_f1__1
   DEY
   STA   (__local_f1__pp),Y
   LDA   (__local_f1__pp),Y
   AND   __local_f1__4
   RTS

f0:
   SUBROUTINE

   SEC
   LDA   #$00
   SBC   __zpabi_f0__p2
   STA   __local_f0__1
   LDA   #$00
   SBC   #$00
   STA   __local_f0__0
   LDA   __local_f0__1
   EOR   #$FF
   STA   __local_f0__1
   LDA   __local_f0__0
   EOR   #$FF
   ORA   __local_f0__1
   BEQ   .cmp_end@0
   SEC
   RTS
.cmp_end@0:
   CMP   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$68
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   LDA   #$D9
   STA   __zpabi_f0__p2
   LDA   #$9D
   STA   __zpabi_f0__p3
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__6
   LDA   g0
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_main__2
   STA   __local_main__5
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   __local_main__2
   EOR   g1+1
   STA   __local_main__2
   LDA   __local_main__5
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__6
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   LDA   #$C3
   STA   g0
   LDA   #$24
   STA   g1
   RTS
