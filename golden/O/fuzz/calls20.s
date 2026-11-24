SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$28
__zpabi_f0__p0_0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f1__p2_0	EQU	$82
__zpabi_f0__p0_3	EQU	$83
__zpabi_f1__p2_1	EQU	$83
__local_main__0	EQU	$84
__zpabi_f1__p2_2	EQU	$84
__local_main__1	EQU	$85
__zpabi_f1__p2_3	EQU	$85
__local_main__2	EQU	$86
__zpabi_f2__p1	EQU	$86
__local_main__3	EQU	$87
__zpabi_f3__p0	EQU	$87
__zpabi_f3__p2_0	EQU	$88
__local_main__5	EQU	$89
__zpabi_f3__p2_1	EQU	$89
__zpabi_f3__pp_0	EQU	$8A
__zpabi_f3__pp_1	EQU	$8B
__local_f1__0	EQU	$8C
__local_f1__1	EQU	$8D
__local_f1__2	EQU	$8E
__local_f1__p1_1	EQU	$8F
__local_f1__p1_0	EQU	$90
__local_f1__box3	EQU	$91
__local_f2__0	EQU	$95
__local_f2__1	EQU	$96
__local_f2__3	EQU	$98
__local_f2__4	EQU	$99
__local_f2__p0	EQU	$9A
__local_f2__box3	EQU	$9B
__local_f3__1	EQU	$9E

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3 locals= indirect=false in_cycle=false carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__p1_1,__local_f1__p1_0,__local_f1__box3,__local_f1__4,__local_f1__5,__local_f1__6 indirect=false in_cycle=false param_regs=,,A,X,,,, carry=true
; def f2 params=__zpabi_f2__p0,__zpabi_f2__p1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__p0,__local_f2__box3,__local_f2__6 indirect=false in_cycle=false param_regs=A,
; def f3 params=__zpabi_f3__p0,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p2_0,__zpabi_f3__p2_1,__zpabi_f3__pp_0,__zpabi_f3__pp_1 locals=__local_f3__0,__local_f3__1 indirect=false in_cycle=false param_regs=,A,X,,,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; call f1 -> f2
; call f1 -> f3
; call f2 -> f3
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$06,$00 weight=9
; zp g0 size=1 at=auto def=true init=$53 weight=4
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f3:
   SUBROUTINE

   LDY   #$00
   LDA   (__zpabi_f3__pp_0),Y
   CLC
   ADC   g1
   STA   __local_f3__1
   INY
   LDA   (__zpabi_f3__pp_0),Y
   ADC   g1+1
   STA   (__zpabi_f3__pp_0),Y
   LDA   __local_f3__1
   DEY
   STA   (__zpabi_f3__pp_0),Y
   SEC
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p0
   LDA   __zpabi_f2__p1
   EOR   #$FF
   STA   HARGS
   LDA   #$00
   EOR   #$FF
   STA   HARGS+1
   LDA   __local_f2__p0
   ORA   #$01
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   STA   __local_f2__4
   STA   __local_f2__box3
   LDA   HARGS+5
   STA   __local_f2__3
   STA   __local_f2__box3+1
   LDA   __local_f2__p0
   LDX   #$FF
   SEC
.div_loop@0:
   INX
   SBC   #$03
   BCS   .div_loop@0
   LDA   __local_f2__4
   SEC
   SBC   __local_f2__p0
   LDA   __local_f2__3
   SBC   #$00
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_f3__p2_0
   LDA   #$64
   STA   __zpabi_f3__p0
   LDA   #$00
   STA   __zpabi_f3__p2_1
   LDA   #<__local_f2__box3
   STA   __zpabi_f3__pp_0
   LDA   #>__local_f2__box3
   STA   __zpabi_f3__pp_1
   TXA
   LDX   #$00
   JSR   f3
   LDA   #$00
   ROL
   STA   __local_f2__0
   LDA   __local_f2__4
   EOR   __local_f2__0
   LDX   __local_f2__3
   STX   __local_f2__0
   CLC
   ADC   __local_f2__box3
   STA   __local_f2__1
   LDA   __local_f2__0
   ADC   __local_f2__box3+1
   STA   __local_f2__0
   LDA   __local_f2__1
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f2__0
   STA   HARGS+3
   LDA   __local_f2__p0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   __local_f2__p0
   EOR   __zpabi_f2__p1
   ORA   HARGS+6
   STA   __local_f2__1
   LDA   HARGS+7
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f2__1
   STA   HARGS+8
   LDA   HARGS+7
   STA   HARGS+9
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p1_0
   STX   __local_f1__p1_1
   LDA   __zpabi_f1__p2_0
   AND   #$3F
   LSR
   LSR
   LSR
   STA   __local_f1__2
   LDA   g1
   ORA   g1+1
   BEQ   .and_false@2
   LDA   __zpabi_f1__p0_0
   ORA   __zpabi_f1__p0_1
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_f1__1
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f1__1
.and_end@3:
   LDA   g1
   STA   __zpabi_f2__p1
   LDA   __local_f1__1
   JSR   f2
   LDA   #$64
   EOR   HARGS+8
   STA   __local_f1__1
   LDA   #$00
   EOR   HARGS+9
   STA   __local_f1__0
   LDA   g1
   CLC
   ADC   __local_f1__1
   STA   g1
   LDA   g1+1
   ADC   __local_f1__0
   STA   g1+1
   LDA   __local_f1__2
   STA   __local_f1__box3
   LDA   #$00
   STA   __local_f1__box3+1
   LDA   __local_f1__p1_0
   SEC
   SBC   __zpabi_f1__p0_0
   LDA   __local_f1__p1_1
   SBC   __zpabi_f1__p0_1
   LDA   #$00
   ROL
   STA   __zpabi_f3__p0
   LDA   g1
   STA   __zpabi_f3__p2_0
   LDA   g1+1
   STA   __zpabi_f3__p2_1
   LDA   #<__local_f1__box3
   STA   __zpabi_f3__pp_0
   LDA   #>__local_f1__box3
   STA   __zpabi_f3__pp_1
   LDX   #$00
   LDA   #$07
   JSR   f3
   LDA   g0
   EOR   #$64
   BEQ   .cmp_end@4
   SEC
   RTS
.cmp_end@4:
   CMP   #$01
   RTS

f0:
   SUBROUTINE

   CLC
   RTS

main:
   SUBROUTINE

   LDA   #$13
   STA   __zpabi_f0__p0_0
   LDA   #$01
   STA   __zpabi_f0__p0_1
   LDA   #$00
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__5
   LDA   g1+1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   #$00
   EOR   g1+1
   STA   __local_main__2
   LDA   #$00
   EOR   __local_main__1
   STA   __local_main__1
   LDA   #$00
   EOR   __local_main__0
   STA   __local_main__0
   LDA   __local_main__5
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
   LDA   #$53
   STA   g0
   LDA   #$06
   STA   g1
   RTS
