SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27
g2	EQU	$28
__local_f1__0	EQU	$80
__zpabi_f0__p0_0	EQU	$80
__local_f1__l2_0	EQU	$81
__zpabi_f0__p0_1	EQU	$81
__local_f1__1	EQU	$82
__zpabi_f0__p0_2	EQU	$82
__local_f1__2	EQU	$83
__zpabi_f0__p0_3	EQU	$83
__local_f1__3	EQU	$84
__zpabi_f0__p1	EQU	$84
__local_f1__4	EQU	$85
__zpabi_f2__p0_0	EQU	$85
__local_f1__pp	EQU	$86
__zpabi_f2__p0_1	EQU	$86
__local_f1__5	EQU	$87
__zpabi_f2__p0_2	EQU	$87
__local_f1__l2_1	EQU	$88
__zpabi_f2__p0_3	EQU	$88
__zpabi_f2__p1_0	EQU	$89
__zpabi_f2__p1_1	EQU	$8A
__zpabi_f2__p2_0	EQU	$8B
__zpabi_f2__p2_1	EQU	$8C
__zpabi_f2__p2_2	EQU	$8D
__zpabi_f2__p2_3	EQU	$8E
__zpabi_f3__p0_0	EQU	$8F
__zpabi_f3__p0_1	EQU	$90
__zpabi_f3__p0_2	EQU	$91
__zpabi_f3__p0_3	EQU	$92
__zpabi_f3__p1	EQU	$93
__local_main__0	EQU	$94
__local_main__1	EQU	$95
__local_f0__1	EQU	$97
__local_f0__4	EQU	$9A
__local_f0__5	EQU	$9B
__local_f0__6	EQU	$9C
__local_f2__0	EQU	$9E
__local_f2__1	EQU	$9F
__local_f2__2	EQU	$A0
__local_f2__3	EQU	$A1
__local_f2__4	EQU	$A2
__local_f2__5	EQU	$A3
__local_f2__6	EQU	$A4
__local_f3__1	EQU	$A6

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__p1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7 indirect=false in_cycle=false changes=__zpabi_f0__p1
; def f1 params=__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__l2_0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__pp,__local_f1__5,__local_f1__l2_1,__local_f1__6,__local_f1__7,__local_f1__8 indirect=false in_cycle=false param_regs=A,X carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6 indirect=false in_cycle=false
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3 indirect=false in_cycle=false changes=__zpabi_f3__p1 carry=true
; def f4 params= locals= indirect=false in_cycle=false
; def f5 params= locals=__local_f5__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call f0 -> f2
; call f1 -> f3
; call f2 -> f3
; call f2 -> f5
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$37 weight=14
; zp g1 size=1 at=auto def=true init=$31 weight=11
; zp g2 size=1 at=auto def=true init=$AB weight=8
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f5:
   SUBROUTINE

   LDA   g0
   LDX   #$00
   RTS

f4:
   SUBROUTINE

   LDX   #$03
   LDA   #$E8
   RTS

f3:
   SUBROUTINE

   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_f3__p0_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f3__p0_1
   STA   HARGS+5
   LDA   __zpabi_f3__p0_2
   STA   HARGS+6
   LDA   __zpabi_f3__p0_3
   STA   HARGS+7
   LDA   g1
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod32
   LDA   HARGS+11
   BNE   .cmp_true@2
   LDA   HARGS+10
   BNE   .cmp_true@2
   LDA   HARGS+9
   CMP   #$7F
   BNE   .cmp_true@2
   LDA   HARGS+8
   CMP   #$FF
   BNE   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   AND   #$07
   TAX
   LDA   g2
   CPX   #$00
   BEQ   .shift_done@6
.shift_loop@5:
   LSR
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   AND   #$07
   TAX
   LDA   #$00
   STA   __local_f3__1
   CPX   #$00
   BEQ   .shift_done@8
.shift_loop@7:
   ASL   __zpabi_f3__p1
   ROL   __local_f3__1
   DEX
   BNE   .shift_loop@7
.shift_done@8:
   LDA   __local_f3__1
   ORA   __zpabi_f3__p1
   BEQ   .cmp_end@9
   SEC
   RTS
.cmp_end@9:
   CMP   #$01
   RTS

f2:
   SUBROUTINE

   SEC
   LDA   #$00
   SBC   __zpabi_f2__p2_0
   STA   __local_f2__4
   LDA   #$00
   SBC   __zpabi_f2__p2_1
   STA   __local_f2__3
   LDA   #$00
   SBC   __zpabi_f2__p2_2
   STA   __local_f2__2
   LDA   #$00
   SBC   __zpabi_f2__p2_3
   STA   __local_f2__1
   LDA   g0
   EOR   #$FC
   CLC
   SBC   __local_f2__4
   LDA   #$00
   SBC   __local_f2__3
   LDA   #$00
   SBC   __local_f2__2
   LDA   #$00
   SBC   __local_f2__1
   LDA   #$00
   ROL
   STA   __local_f2__6
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   __local_f2__5
   STA   __local_f2__4
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   g0
   SEC
   SBC   __local_f2__6
   LDA   #$00
   SBC   #$00
   LDA   __local_f2__1
   SBC   __local_f2__5
   LDA   __local_f2__0
   SBC   __local_f2__4
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_f3__p1
   LDA   g1
   STA   __zpabi_f3__p0_0
   LDA   #$00
   STA   __zpabi_f3__p0_1
   STA   __zpabi_f3__p0_2
   STA   __zpabi_f3__p0_3
   JSR   f3
   JSR   f5
   STA   __local_f2__0
   LDA   __local_f2__6
   EOR   __local_f2__0
   CLC
   ADC   g1
   STA   g1
   LDA   __zpabi_f2__p0_0
   CLC
   ADC   #$FC
   ORA   #$FC
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__pp
   STX   __local_f1__5
   LDA   g2
   STA   __local_f1__l2_0
   LDA   g0
   AND   #$07
   TAX
   LDA   #$00
   STA   __local_f1__1
   CPX   #$00
   BEQ   .shift_done@16
.shift_loop@15:
   ASL   __local_f1__l2_0
   ROL   __local_f1__1
   DEX
   BNE   .shift_loop@15
.shift_done@16:
   LDA   g2
   ORA   #$01
   STA   HARGS+1
   LDA   #$02
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+3
   SEC
   SBC   __local_f1__l2_0
   LDA   #$00
   SBC   __local_f1__1
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f1__4
   LDA   g1
   CLC
   SBC   #$03
   LDA   #$00
   ROL
   STA   __local_f1__l2_0
   LDA   g2
   EOR   g1
   ORA   __local_f1__l2_0
   STA   __local_f1__3
   LDA   #$00
   STA   __local_f1__l2_1
   LDA   __local_f1__4
   SEC
   SBC   #$FF
   STA   __local_f1__l2_0
   LDA   #$00
   SBC   #$7F
   STA   __local_f1__0
   LDY   #$00
   LDA   (__local_f1__pp),Y
   CLC
   ADC   __local_f1__l2_0
   STA   __local_f1__l2_0
   INY
   LDA   (__local_f1__pp),Y
   ADC   __local_f1__0
   STA   (__local_f1__pp),Y
   LDA   __local_f1__l2_0
   DEY
   STA   (__local_f1__pp),Y
   LDA   (__local_f1__pp),Y
   EOR   __local_f1__3
   STA   __zpabi_f3__p0_0
   INY
   LDA   (__local_f1__pp),Y
   EOR   __local_f1__l2_1
   STA   __zpabi_f3__p0_1
   LDA   #$00
   STA   __zpabi_f3__p0_2
   STA   __zpabi_f3__p0_3
   LDA   #$FF
   STA   __zpabi_f3__p1
   JSR   f3
   BCC   .if_else@1
   LDA   __local_f1__3
   CLC
   ADC   #$03
   STA   __local_f1__l2_0
   BCC   .if_end@0
   INC   __local_f1__l2_1
   JMP   .if_end@0
.if_else@1:
   LDA   __local_f1__3
   EOR   #$05
   STA   __local_f1__l2_0
.if_end@0:
   LDA   g0
   CLC
   ADC   __local_f1__l2_0
   STA   g0
   LDA   #$64
   CLC
   ADC   g1
   STA   __local_f1__5
   LDA   #$00
   ADC   #$00
   STA   __local_f1__pp
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f1__4
   STA   __local_f1__3
   LDA   #$03
   AND   __local_f1__l2_0
   STA   __local_f1__2
   LDA   #$00
   AND   __local_f1__l2_1
   STA   __local_f1__1
   LDA   __local_f1__2
   CLC
   SBC   __local_f1__5
   LDA   __local_f1__1
   SBC   __local_f1__pp
   LDA   #$00
   SBC   __local_f1__4
   LDA   #$00
   SBC   __local_f1__3
   RTS

f0:
   SUBROUTINE

   LDA   __zpabi_f0__p1
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __local_f0__1
   LDA   __zpabi_f0__p1
   AND   #$07
   TAX
   BEQ   .shift_done@23
.shift_loop@22:
   ASL   __zpabi_f0__p1
   ROL   __local_f0__1
   DEX
   BNE   .shift_loop@22
.shift_done@23:
   LDA   __zpabi_f0__p1
   EOR   #$FF
   STA   __local_f0__6
   LDA   #$00
   BPL   .sx_done@25
   LDA   #$FF
.sx_done@25:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_f0__p0_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f0__p0_1
   STA   HARGS+5
   LDA   __zpabi_f0__p0_2
   STA   HARGS+6
   LDA   __zpabi_f0__p0_3
   STA   HARGS+7
   LDA   g0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod32
   LDA   g2
   AND   __local_f0__6
   STA   __zpabi_f0__p1
   LDA   HARGS+12
   SEC
   SBC   __zpabi_f0__p1
   LDA   HARGS+13
   SBC   #$00
   LDA   HARGS+14
   SBC   #$00
   LDA   HARGS+15
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f0__5
   AND   #$07
   STA   HARGS+4
   LDA   __zpabi_f0__p0_0
   STA   HARGS
   LDA   __zpabi_f0__p0_1
   STA   HARGS+1
   LDA   __zpabi_f0__p0_2
   STA   HARGS+2
   LDA   __zpabi_f0__p0_3
   STA   HARGS+3
   JSR   lsr32
   LDA   HARGS+5
   EOR   __zpabi_f0__p0_0
   STA   __local_f0__4
   LDA   HARGS+6
   EOR   __zpabi_f0__p0_1
   STA   __zpabi_f2__p1_1
   LDA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   STA   __local_f0__1
   STA   __zpabi_f0__p1
   LDA   __zpabi_f0__p0_0
   SEC
   SBC   g2
   LDA   __zpabi_f0__p0_1
   SBC   #$00
   LDA   __zpabi_f0__p0_2
   SBC   __local_f0__1
   LDA   __zpabi_f0__p0_3
   SBC   __zpabi_f0__p1
   LDA   #$00
   ROL
   STA   __local_f0__1
   LDA   __local_f0__5
   SEC
   SBC   __local_f0__6
   LDA   #$00
   ROL
   SEC
   SBC   __local_f0__1
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f0__1
   LDA   #$00
   STA   __zpabi_f2__p0_0
   STA   __zpabi_f2__p0_1
   STA   __zpabi_f2__p0_2
   STA   __zpabi_f2__p0_3
   LDA   __local_f0__4
   STA   __zpabi_f2__p1_0
   LDA   g0
   STA   __zpabi_f2__p2_0
   LDA   #$00
   STA   __zpabi_f2__p2_1
   STA   __zpabi_f2__p2_2
   STA   __zpabi_f2__p2_3
   JSR   f2
   STA   __zpabi_f0__p1
   LDA   __local_f0__4
   EOR   __zpabi_f0__p1
   CLC
   ADC   g0
   STA   g0
   LDA   __local_f0__1
   BNE   .or_true@2
   LDA   __local_f0__6
   ORA   #$01
   STA   HARGS+2
   LDA   #$FF
   STA   HARGS
   LDA   #$7F
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BNE   .or_true@2
   LDA   #$00
   RTS
.or_true@2:
   LDA   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$79
   STA   __zpabi_f0__p0_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   LDA   #$43
   STA   __zpabi_f0__p1
   JSR   f0
   STA   __local_main__1
   LDA   g0
   EOR   g1
   EOR   g2
   STA   __local_main__0
   LDA   __local_main__1
   EOR   __local_main__0
   LDX   #$00
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$37
   STA   g0
   LDA   #$31
   STA   g1
   LDA   #$AB
   STA   g2
   RTS
