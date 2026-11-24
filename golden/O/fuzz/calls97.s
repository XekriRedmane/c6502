SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_f3__p0	EQU	$80
__zpabi_f0__p1	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__local_f2__p0_0	EQU	$81
__local_f3__0	EQU	$81
__zpabi_f0__pp_0	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f0__pp_1	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__local_main__0	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__local_main__1	EQU	$84
__zpabi_f1__p1	EQU	$84
__local_main__2	EQU	$85
__zpabi_f1__pp_0	EQU	$85
__local_main__3	EQU	$86
__zpabi_f1__pp_1	EQU	$86
__local_main__4	EQU	$87
__local_f1__1	EQU	$88
__local_main__mbox	EQU	$88
__local_f1__2	EQU	$89
__local_f1__3	EQU	$8A
__local_f1__p2_1	EQU	$8B
__local_f0__0	EQU	$8C
__local_f1__p2_0	EQU	$8C
__local_f0__1	EQU	$8D
__local_f0__2	EQU	$8E
__local_f0__3	EQU	$8F
__local_f0__4	EQU	$90
__local_f0__p0_1	EQU	$92
__local_f0__p0_0	EQU	$93

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__p0_1,__local_f0__p0_0 indirect=false in_cycle=false param_regs=A,X,,, carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__p2_1,__local_f1__p2_0 indirect=false in_cycle=false param_regs=,,,,,A,X,, carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1 locals=__local_f2__p0_1,__local_f2__p0_0 indirect=false in_cycle=false param_regs=A,X carry=true
; def f3 params=__zpabi_f3__p0 locals=__local_f3__p0,__local_f3__0 indirect=false in_cycle=false param_regs=A
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__mbox,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f2@tail0 section=.text local=true zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item g1 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f3:
   SUBROUTINE

   STA   __local_f3__p0
   LDA   #$00
   SEC
   SBC   __local_f3__p0
   STA   __local_f3__0
   LDA   #$00
   SBC   #$00
   STA   __local_f3__p0
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__0
   STA   HARGS+8
   LDA   __local_f3__p0
   STA   HARGS+9
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p0_0
   TXA
   ORA   __local_f2__p0_0
   JMP   f2@tail0

f2@tail0:
   SUBROUTINE

   BEQ   .cmp_end@2
   SEC
   RTS
.cmp_end@2:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p2_0
   STX   __local_f1__p2_1
   SEC
   LDA   #$00
   SBC   g0
   STA   __local_f1__1
   LDA   __zpabi_f1__p1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f1__3
   LDA   __zpabi_f1__p1
   AND   g0
   STA   HARGS+1
   LDA   __local_f1__1
   STA   HARGS
   JSR   mul8
   LDA   HARGS+2
   STA   __local_f1__2
   LDY   #$01
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__1
   DEY
   LDA   (__zpabi_f1__pp_0),Y
   CLC
   ADC   #$03
   BCC   .carry_inc@0
   INC   __local_f1__1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__1
   INY
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__2
   SEC
   SBC   __zpabi_f1__p1
   STA   __local_f1__2
   LDA   #$00
   SBC   __local_f1__3
   STA   __local_f1__1
   LDA   __zpabi_f1__p1
   SEC
   SBC   __local_f1__p2_0
   LDA   __local_f1__3
   SBC   __local_f1__p2_1
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   ORA   #$01
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   LDA   __local_f1__2
   STA   HARGS
   LDA   __local_f1__1
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+7
   ORA   HARGS+6
   JMP   f2@tail0

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   STX   __local_f0__p0_1
   TXA
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_f0__4
   LDY   #$00
   LDA   (__zpabi_f0__pp_0),Y
   CLC
   ADC   __local_f0__p0_0
   STA   __local_f0__1
   INY
   LDA   (__zpabi_f0__pp_0),Y
   ADC   __local_f0__p0_1
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__1
   DEY
   STA   (__zpabi_f0__pp_0),Y
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__3
   INY
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__2
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __local_f0__4
   CMP   __local_f0__0
   BNE   .thread@0
   CMP   __local_f0__1
   BNE   .thread@0
   CPX   __local_f0__2
   BNE   .thread@0
   LDA   __local_f0__p0_0
   CMP   __local_f0__3
   BEQ   .cmp_true@13
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   AND   #$07
   TAX
   BEQ   .shift_done@17
.shift_loop@16:
   LSR   __local_f0__p0_0
   DEX
   BNE   .shift_loop@16
.shift_done@17:
   LDA   __local_f0__p0_0
   JMP   f2@tail0

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDA   #$C4
   STA   __zpabi_f0__p1
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   LDX   #$00
   LDA   #$D8
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__4
   LDA   g0+1
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   __local_main__1
   EOR   g1+2
   STA   __local_main__1
   LDA   __local_main__0
   EOR   g1+3
   STA   __local_main__0
   LDA   __local_main__4
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
   DC.W  $000D

g1:
   DC.L  $00000005
