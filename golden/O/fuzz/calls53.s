SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
__zpabi_f3__pp_0	EQU	$80
__zpabi_f3__pp_1	EQU	$81
__local_f1__0	EQU	$82
__local_main__0	EQU	$82
__local_f1__l0	EQU	$83
__local_f1__1	EQU	$84
__local_main__2	EQU	$84
__local_f1__2	EQU	$85
__local_main__3	EQU	$85
__local_f1__p0_1	EQU	$86
__local_f1__p0_0	EQU	$87
__local_f1__3	EQU	$88
__local_f1__4	EQU	$89
__local_f1__5	EQU	$8A
__local_f1__6	EQU	$8B
__local_f0__l1_0	EQU	$8C
__local_f1__box2	EQU	$8C
__local_f0__0	EQU	$8D
__local_f0__l1_1	EQU	$8E
__local_f1__box3	EQU	$8E
__local_f0__1	EQU	$8F
__local_f0__box2	EQU	$90
__local_f2__0	EQU	$94
__local_f2__1	EQU	$95
__local_f2__2	EQU	$96
__local_f2__3	EQU	$97
__local_f2__pp	EQU	$98
__local_f2__4	EQU	$99
__local_f2__box3	EQU	$9A
__local_f3__0	EQU	$9D
__local_f3__1	EQU	$9E
__local_f3__2	EQU	$9F
__local_f3__3	EQU	$A0

; @zp-link-meta-begin
; def f0 params= locals=__local_f0__l1_0,__local_f0__0,__local_f0__l1_1,__local_f0__1,__local_f0__box2,__local_f0__3,__local_f0__4,__local_f0__5 indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1 locals=__local_f1__0,__local_f1__l0,__local_f1__1,__local_f1__2,__local_f1__p0_1,__local_f1__p0_0,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__box2,__local_f1__8,__local_f1__box3,__local_f1__10,__local_f1__11,__local_f1__12,__local_f1__13 indirect=false in_cycle=false param_regs=A,X carry=true
; def f2 params=__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__pp,__local_f2__4,__local_f2__box3,__local_f2__6,__local_f2__7 indirect=false in_cycle=false param_regs=A,X carry=true
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__pp_0,__zpabi_f3__pp_1 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9 indirect=false in_cycle=false
; call f0 -> f2
; call f1 -> f2
; call f1 -> f3
; call f2 -> f3
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$B7,$00 weight=11
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f3:
   SUBROUTINE

   LDY   #$00
   LDA   (__zpabi_f3__pp_0),Y
   STA   __local_f3__3
   INY
   LDA   (__zpabi_f3__pp_0),Y
   STA   __local_f3__2
   LDA   g1+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   g0+3
   CMP   __local_f3__0
   BNE   .cmp_true@2
   LDA   g0+2
   CMP   __local_f3__1
   BNE   .cmp_true@2
   LDA   g0+1
   CMP   g1+1
   BNE   .cmp_true@2
   LDA   g0
   CMP   g1
   BNE   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   CLC
   ADC   __local_f3__3
   BCC   .carry_inc@0
   INC   __local_f3__2
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f3__pp_0),Y
   LDA   __local_f3__2
   INY
   STA   (__zpabi_f3__pp_0),Y
   LDA   g0
   LDX   g0+1
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__pp
   STX   __local_f2__4
   LDA   g0
   STA   __local_f2__2
   LDY   #$00
   LDA   (__local_f2__pp),Y
   STA   __local_f2__0
   INY
   LDA   (__local_f2__pp),Y
   STA   __local_f2__1
   INC   __local_f2__0
   BNE   .inc_done@0
   INC   __local_f2__1
.inc_done@0:
   LDA   __local_f2__0
   LDY   #$00
   STA   (__local_f2__pp),Y
   LDA   __local_f2__1
   INY
   STA   (__local_f2__pp),Y
   LDA   __local_f2__2
   STA   __local_f2__box3
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f2__box3+1
   LDA   #<__local_f2__box3
   STA   __zpabi_f3__pp_0
   LDA   #>__local_f2__box3
   STA   __zpabi_f3__pp_1
   LDX   #$00
   LDA   #$01
   JSR   f3
   STA   __local_f2__0
   LDA   __local_f2__2
   EOR   __local_f2__0
   CLC
   ADC   __local_f2__box3
   STA   __local_f2__3
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_f2__2
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   g0
   CLC
   ADC   __local_f2__3
   STA   g0
   LDA   g0+1
   ADC   __local_f2__2
   STA   g0+1
   LDA   g0+2
   ADC   __local_f2__1
   STA   g0+2
   LDA   g0+3
   ADC   __local_f2__0
   STA   g0+3
   LDY   #$00
   LDA   (__local_f2__pp),Y
   STA   __local_f2__1
   LDA   #$00
   INY
   CMP   (__local_f2__pp),Y
   BNE   .cmp_true@9
   LDA   #$03
   CMP   __local_f2__1
   BNE   .cmp_true@9
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_f2__2
   LDA   g1
   AND   #$07
   TAX
   LDA   #$FF
   STA   __local_f2__0
   LDA   #$7F
   STA   __local_f2__1
   CPX   #$00
   BEQ   .shift_done@13
.shift_loop@12:
   ASL   __local_f2__0
   ROL   __local_f2__1
   DEX
   BNE   .shift_loop@12
.shift_done@13:
   LDA   #$00
   CMP   __local_f2__1
   BNE   .cmp_end@15
   LDA   __local_f2__2
   CMP   __local_f2__0
   BEQ   .cmp_true@14
   CLC
   RTS
.cmp_true@14:
   SEC
   RTS
.cmp_end@15:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p0_0
   STX   __local_f1__p0_1
   LDA   g0
   STA   __local_f1__6
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_f1__5
   LDA   g0
   SEC
   SBC   g1
   STA   __local_f1__2
   LDA   g0+1
   SBC   g1+1
   STA   __local_f1__1
   LDA   g0+2
   SBC   #$00
   STA   __local_f1__l0
   LDA   g0+3
   SBC   #$00
   STA   __local_f1__0
   LDA   __local_f1__2
   ORA   __local_f1__1
   ORA   __local_f1__l0
   ORA   __local_f1__0
   BEQ   .and_false@2
   LDA   __local_f1__p0_0
   ORA   __local_f1__p0_1
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_f1__4
   LDA   #$00
   STA   __local_f1__3
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f1__4
   LDY   #$00
   STY   __local_f1__3
.and_end@3:
   ORA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   __local_f1__p0_0
   STA   __local_f1__p0_1
   LDA   __local_f1__6
   STA   __local_f1__box2
   LDA   __local_f1__5
   STA   __local_f1__box2+1
   LDX   #>__local_f1__box2
   LDA   #<__local_f1__box2
   JSR   f2
   BCC   .if_else@5
   LDA   __local_f1__6
   CLC
   ADC   #$03
   JMP   .if_end@4
.if_else@5:
   LDA   __local_f1__6
   EOR   #$05
.if_end@4:
   CLC
   ADC   __local_f1__box2
   STA   __local_f1__5
   STA   __local_f1__box3
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_f1__box3+1
   LDA   __local_f1__4
   STA   HARGS
   LDA   __local_f1__3
   STA   HARGS+1
   LDA   __local_f1__p0_0
   STA   HARGS+2
   LDA   __local_f1__p0_1
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS+4
   LDA   #$7F
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDX   HARGS+13
   LDA   #<__local_f1__box3
   STA   __zpabi_f3__pp_0
   LDA   #>__local_f1__box3
   STA   __zpabi_f3__pp_1
   LDA   HARGS+12
   JSR   f3
   STA   __local_f1__2
   STX   __local_f1__1
   TXA
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_f1__l0
   STA   __local_f1__0
   LDA   __local_f1__4
   EOR   __local_f1__2
   STA   __local_f1__4
   LDA   __local_f1__3
   EOR   __local_f1__1
   STA   __local_f1__3
   LDA   __local_f1__p0_0
   EOR   __local_f1__l0
   STA   __local_f1__p0_0
   LDA   __local_f1__p0_1
   EOR   __local_f1__0
   STA   __local_f1__p0_1
   LDA   __local_f1__box3
   STA   __local_f1__2
   LDA   __local_f1__box3+1
   STA   __local_f1__1
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_f1__l0
   STA   __local_f1__0
   LDA   __local_f1__4
   CLC
   ADC   __local_f1__2
   STA   __local_f1__2
   LDA   __local_f1__3
   ADC   __local_f1__1
   STA   __local_f1__1
   LDA   __local_f1__p0_0
   ADC   __local_f1__l0
   STA   __local_f1__l0
   LDA   __local_f1__p0_1
   ADC   __local_f1__0
   STA   __local_f1__0
   LDA   g1
   CLC
   ADC   __local_f1__2
   STA   g1
   LDA   g1+1
   ADC   __local_f1__1
   STA   g1+1
   LDA   __local_f1__5
   BNE   .or_true@6
   LDA   __local_f1__2
   ORA   __local_f1__1
   ORA   __local_f1__l0
   ORA   __local_f1__0
   BNE   .or_true@6
   CLC
   RTS
.or_true@6:
   SEC
   RTS

f0:
   SUBROUTINE

   LDA   g1
   STA   __local_f0__1
   STA   __local_f0__box2
   LDA   g1+1
   STA   __local_f0__l1_1
   STA   __local_f0__box2+1
   LDX   #>__local_f0__box2
   LDA   #<__local_f0__box2
   JSR   f2
   BCC   .if_else@9
   LDA   __local_f0__1
   CLC
   ADC   #$03
   BCC   .if_end@8
   INC   __local_f0__l1_1
   JMP   .if_end@8
.if_else@9:
   LDA   __local_f0__1
   EOR   #$05
.if_end@8:
   CLC
   ADC   __local_f0__box2
   STA   __local_f0__1
   LDA   __local_f0__l1_1
   ADC   __local_f0__box2+1
   STA   __local_f0__l1_1
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __local_f0__0
   STA   __local_f0__l1_0
   LDA   __local_f0__1
   SEC
   SBC   g0
   STA   __local_f0__1
   LDA   __local_f0__l1_1
   SBC   g0+1
   STA   __local_f0__l1_1
   LDA   __local_f0__0
   SBC   g0+2
   STA   __local_f0__0
   LDA   __local_f0__l1_0
   SBC   g0+3
   STA   __local_f0__l1_0
   LDA   #$00
   SEC
   SBC   __local_f0__1
   LDA   #$00
   SBC   __local_f0__l1_1
   LDA   #$00
   SBC   __local_f0__0
   LDA   #$00
   SBC   __local_f0__l1_0
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   STA   __local_f0__l1_0
   LDA   #$00
   BPL   .sx_done@31
   LDA   #$FF
.sx_done@31:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f0__l1_0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

main:
   SUBROUTINE

   JSR   f0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   HARGS+8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   HARGS+9
   EOR   __local_main__2
   STA   __local_main__2
   LDA   HARGS+10
   EOR   g0+2
   STA   HARGS+2
   LDA   HARGS+11
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

g0:
   DC.L  $000000B6

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   LDA   #$B7
   STA   g1
   RTS
