SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_main__0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f3__p1_0	EQU	$80
__local_main__1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f3__p1_1	EQU	$81
__zpabi_f1__p0_2	EQU	$82
__zpabi_f3__pp_0	EQU	$82
__zpabi_f1__p0_3	EQU	$83
__zpabi_f3__pp_1	EQU	$83
__local_f3__0	EQU	$84
__local_main__4	EQU	$84
__zpabi_f2__p1_0	EQU	$84
__local_f3__1	EQU	$85
__local_main__5	EQU	$85
__zpabi_f2__p1_1	EQU	$85
__local_main__6	EQU	$86
__zpabi_f2__pp_0	EQU	$86
__local_main__7	EQU	$87
__zpabi_f2__pp_1	EQU	$87
__local_f0__l0_1	EQU	$88
__local_f1__0	EQU	$88
__local_f0__l0_0	EQU	$89
__local_f1__1	EQU	$89
__local_f0__p0_1	EQU	$8A
__local_f0__p0_0	EQU	$8B
__local_f1__3	EQU	$8B
__local_f1__4	EQU	$8C
__local_f1__5	EQU	$8D
__local_f1__6	EQU	$8E
__local_f1__box2	EQU	$91
__local_rec__0	EQU	$94
__local_f2__1	EQU	$95
__local_rec__1	EQU	$95
__local_rec__2	EQU	$96
__local_rec__3	EQU	$97
__local_rec__4	EQU	$98

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1 locals=__local_f0__l0_1,__local_f0__l0_0,__local_f0__p0_1,__local_f0__p0_0 indirect=false in_cycle=false param_regs=A,X
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__8,__local_f1__box2,__local_f1__10,__local_f1__11 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1 indirect=false in_cycle=false param_regs=A,X,,,, carry=true
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__pp_0,__zpabi_f3__pp_1 locals=__local_f3__0,__local_f3__1 indirect=false in_cycle=false param_regs=A,X,,,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> rec
; call f1 -> f2
; call f1 -> rec
; call f3 -> rec
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f3@tail0 section=.text local=true zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$55,$00 weight=14
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

rec:
   SUBROUTINE

   ; prologue: 4 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$05
   LDA   (FP),Y
   ASL
   ASL
   ASL
   STA   __local_rec__0
   SEC
   SBC   (FP),Y
   STA   __local_rec__4
   DEY
   LDA   (FP),Y
   BMI   .jcmp_neg@0
   DEY
   ORA   (FP),Y
   BNE   .if_end@0
.jcmp_neg@0:
   LDY   #$05
   LDA   (FP),Y
   STA   __local_rec__0
   INY
   LDA   (FP),Y
   LDX   #$00
   LDA   __local_rec__0

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@0:
   LDY   #$05
   LDA   (FP),Y
   ASL
   STA   __local_rec__2
   INY
   LDA   (FP),Y
   ROL
   STA   __local_rec__3
   LDA   __local_rec__2
   CLC
   DEY
   ADC   (FP),Y
   STA   __local_rec__1
   LDA   __local_rec__3
   INY
   ADC   (FP),Y
   STA   __local_rec__0
   LDA   __local_rec__1
   PHA
   LDA   __local_rec__0
   PHA
   LDA   __local_rec__1
   CLC
   LDY   #$03
   ADC   (FP),Y
   STA   __local_rec__1
   LDA   __local_rec__0
   INY
   ADC   (FP),Y
   STA   __local_rec__0
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   STA   __local_rec__3
   INY
   LDA   (FP),Y
   SBC   #$00
   STA   __local_rec__2
   LDA   __local_rec__0
   AND   #$03
   STA   __local_rec__0
   LDA   __local_rec__4
   PHA
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   __local_rec__3
   LDY   #$01
   STA   (SSP),Y
   LDA   __local_rec__2
   INY
   STA   (SSP),Y
   LDA   __local_rec__1
   INY
   STA   (SSP),Y
   LDA   __local_rec__0
   INY
   STA   (SSP),Y
   JSR   rec
   STA   __local_rec__4
   STX   __local_rec__3
   PLA
   STA   __local_rec__2
   PLA
   PLA
   STA   __local_rec__0
   LDA   __local_rec__4
   CLC
   ADC   __local_rec__2
   STA   __local_rec__2
   BCC   .carry_inc@0
   INC   __local_rec__3
.carry_inc@0:
   LDA   __local_rec__0
   AND   #$0F
   CLC
   ADC   __local_rec__2
   STA   __local_rec__0
   LDA   __local_rec__3
   ADC   #$00
   AND   #$03
   TAX
   LDA   __local_rec__0

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS

f3:
   SUBROUTINE

   LDA   __zpabi_f3__pp_0
   STA   DPTR
   LDA   __zpabi_f3__pp_1
   STA   DPTR+1
   LDY   #$01
   LDA   (__zpabi_f3__pp_0),Y
   STA   __local_f3__1
   DEY
   LDA   (__zpabi_f3__pp_0),Y
   CLC
   BCC   .carry_inc@1
   INC   __local_f3__1
.carry_inc@1:
   LDY   #$00
   STA   (__zpabi_f3__pp_0),Y
   LDA   __local_f3__1
   INY
   STA   (__zpabi_f3__pp_0),Y
   LDA   #$01
   SEC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__0
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   __local_f3__0
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$00
   LDA   (__zpabi_f3__pp_0),Y
   INY
   ORA   (__zpabi_f3__pp_0),Y
   JMP   f3@tail0

f3@tail0:
   SUBROUTINE

   BEQ   .cmp_end@2
   SEC
   RTS
.cmp_end@2:
   CMP   #$01
   RTS

f2:
   SUBROUTINE

   LDA   __zpabi_f2__p1_0
   ORA   #$01
   STA   HARGS+2
   LDA   __zpabi_f2__p1_1
   STA   HARGS+3
   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+6
   STA   HARGS
   LDA   HARGS+7
   STA   HARGS+1
   LDA   g0
   EOR   #$FF
   STA   HARGS+2
   LDA   g0+1
   EOR   #$FF
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   HARGS
   LDA   HARGS+5
   STA   HARGS+1
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   CLC
   ADC   g0
   STA   __local_f2__1
   INY
   LDA   (__zpabi_f2__pp_0),Y
   ADC   g0+1
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__1
   DEY
   STA   (__zpabi_f2__pp_0),Y
   LDA   #$E8
   EOR   __zpabi_f2__p1_0
   STA   HARGS+2
   LDA   #$03
   EOR   __zpabi_f2__p1_1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   ORA   HARGS+4
   JMP   f3@tail0

f1:
   SUBROUTINE

   LDA   #$03
   CLC
   ADC   __zpabi_f1__p0_0
   STA   HARGS+2
   LDA   #$00
   ADC   __zpabi_f1__p0_1
   STA   HARGS+3
   LDA   #$00
   STA   HARGS
   STA   HARGS+1
   JSR   mul16
   LDA   g0+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f1__3
   LDA   g0
   CLC
   ADC   #$01
   STA   __local_f1__0
   LDA   g0+1
   ADC   #$00
   BCC   .carry_inc@3
   INC   __local_f1__3
.carry_inc@3:
   STA   __local_f1__1
   LDA   __local_f1__0
   STA   __zpabi_f2__p1_0
   LDA   __local_f1__3
   AND   #$3F
   LDX   __zpabi_f2__p1_0
   STX   __local_f1__6
   LDX   __local_f1__1
   STX   __local_f1__5
   EOR   #$20
   SEC
   SBC   #$20
   STA   __local_f1__4
   LDA   #$01
   STA   __local_f1__box2
   LDY   #$00
   STY   __local_f1__box2+1
   EOR   HARGS+4
   STA   __local_f1__3
   TYA
   EOR   HARGS+5
   TAX
   LDA   #$01
   SEC
   SBC   __local_f1__6
   STA   __zpabi_f2__p1_0
   TYA
   SBC   __local_f1__5
   STA   __zpabi_f2__p1_1
   LDA   #<__local_f1__box2
   STA   __zpabi_f2__pp_0
   LDA   #>__local_f1__box2
   STA   __zpabi_f2__pp_1
   LDA   __local_f1__3
   JSR   f2
   LDA   g0
   CLC
   ADC   __local_f1__box2
   STA   __local_f1__0
   LDA   g0+1
   ADC   __local_f1__box2+1
   STA   __zpabi_f2__p1_0
   LDA   __local_f1__0
   CLC
   ADC   #$04
   BCC   .carry_inc@2
   INC   __zpabi_f2__p1_0
.carry_inc@2:
   AND   #$FF
   STA   __local_f1__1
   LDA   __zpabi_f2__p1_0
   AND   #$3F
   EOR   #$20
   STA   __local_f1__0
   LDA   __local_f1__1
   STA   g0
   LDA   __local_f1__0
   SEC
   SBC   #$20
   STA   g0+1
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   __local_f1__6
   INY
   STA   (SSP),Y
   LDA   __local_f1__5
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   #$03
   AND   __local_f1__6
   STA   __zpabi_f2__p1_0
   LDA   #$00
   AND   __local_f1__5
   STA   __local_f1__1
   LDA   #$00
   AND   __local_f1__4
   STA   __local_f1__0
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   HARGS+11
   LDA   __zpabi_f2__p1_0
   STA   HARGS+8
   LDA   __local_f1__1
   STA   HARGS+9
   LDA   __local_f1__0
   STA   HARGS+10
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   STX   __local_f0__p0_1
   LDA   g0
   STA   __local_f0__l0_0
   LDA   g0+1
   STA   __local_f0__l0_1
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   __local_f0__l0_0
   INY
   STA   (SSP),Y
   LDA   __local_f0__l0_1
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __local_f0__p0_0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f0__p0_1
   STA   HARGS+3
   LDA   #$03
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+6
   ORA   #$07
   STA   __local_f0__l0_0
   LDA   HARGS+7
   AND   #$3F
   EOR   #$20
   SEC
   SBC   #$20
   TAX
   LDA   __local_f0__l0_0
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$F4
   JSR   f0
   STA   __local_main__7
   STX   __local_main__6
   TXA
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_main__5
   STA   __local_main__4
   LDA   g0+1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__7
   EOR   g0
   STA   HARGS
   TXA
   EOR   g0+1
   STA   HARGS+1
   LDA   __local_main__5
   EOR   __local_main__1
   STA   HARGS+2
   LDA   __local_main__4
   EOR   __local_main__0
   STA   HARGS+3
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   __local_main__7
   EOR   g0
   STA   __local_main__0
   LDA   __local_main__6
   EOR   g0+1
   STA   __local_main__1
   LDA   __local_main__0
   EOR   HARGS+5
   STA   __local_main__0
   LDA   __local_main__1
   EOR   HARGS+6
   TAX
   LDA   __local_main__0
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$55
   STA   g0
   RTS
