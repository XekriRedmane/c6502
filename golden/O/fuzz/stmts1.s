SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$2E
z6	EQU	$32
g3	EQU	$33
@11.s26	EQU	$34
@21.s36	EQU	$35
__local_main__l39	EQU	$80
__zpabi_f29__p30_0	EQU	$80
__local_main__0	EQU	$81
__zpabi_f29__p30_1	EQU	$81
__zpabi_f29__p30_2	EQU	$82
__zpabi_f29__p30_3	EQU	$83
__zpabi_f29__p31_0	EQU	$84
__zpabi_f29__p31_1	EQU	$85
__zpabi_f29__p31_2	EQU	$86
__zpabi_f29__p31_3	EQU	$87
__zpabi_f29__pp32_0	EQU	$88
__zpabi_f29__pp32_1	EQU	$89
__zpabi_f17__p20	EQU	$8A
__zpabi_f17__pp21_0	EQU	$8B
__zpabi_f17__pp21_1	EQU	$8C
__zpabi_f17__p19_0	EQU	$8D
__zpabi_f17__p19_1	EQU	$8E
__zpabi_f17__p19_2	EQU	$8F
__zpabi_f17__p19_3	EQU	$90
__zpabi_mix__v_0	EQU	$91
__zpabi_mix__v_1	EQU	$92
__zpabi_mix__v_2	EQU	$93
__zpabi_mix__v_3	EQU	$94
__local_f29__0	EQU	$95
__local_f29__1	EQU	$96
__local_f29__2	EQU	$97
__local_f29__w38	EQU	$98
__local_f29__w37	EQU	$99
__local_f29__l34	EQU	$9A
__local_f29__l35	EQU	$9C
__local_f17__0	EQU	$A0
__local_f17__l24	EQU	$A1
__local_f17__1	EQU	$A2
__local_f17__l25	EQU	$A3
__local_f10__0	EQU	$A4
__local_f10__1	EQU	$A5
__local_f10__pp11	EQU	$A6
__local_f10__2	EQU	$A7
__local_mix__0	EQU	$A8
__local_mix__1	EQU	$A9
__local_mix__2	EQU	$AA
__local_mix__3	EQU	$AB

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__pp11_0,__zpabi_f10__pp11_1 locals=__local_f10__0,__local_f10__1,__local_f10__pp11,__local_f10__2 indirect=false in_cycle=false param_regs=A,X
; def f17 params=__zpabi_f17__p18,__zpabi_f17__p19_0,__zpabi_f17__p19_1,__zpabi_f17__p19_2,__zpabi_f17__p19_3,__zpabi_f17__p20,__zpabi_f17__pp21_0,__zpabi_f17__pp21_1 locals=__local_f17__w27,__local_f17__0,__local_f17__l24,__local_f17__1,__local_f17__l25 indirect=false in_cycle=false param_regs=A,,,,,,,
; def f29 params=__zpabi_f29__p30_0,__zpabi_f29__p30_1,__zpabi_f29__p30_2,__zpabi_f29__p30_3,__zpabi_f29__p31_0,__zpabi_f29__p31_1,__zpabi_f29__p31_2,__zpabi_f29__p31_3,__zpabi_f29__pp32_0,__zpabi_f29__pp32_1 locals=__local_f29__0,__local_f29__1,__local_f29__2,__local_f29__w38,__local_f29__w37,__local_f29__l34,__local_f29__4,__local_f29__l35,__local_f29__6,__local_f29__7 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__l39,__local_main__0 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call f17 -> f10
; call f17 -> mix
; call f29 -> f17
; call f29 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; share member=f17:__zpabi_f17__p19_0,__zpabi_f17__p19_1,__zpabi_f17__p19_2,__zpabi_f17__p19_3 holder=f29 live=f29:f17 writer=f29
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run1 section=.text local=true zero=false
; item f10 section=.text local=false zero=false
; item f17 section=.text local=false zero=false
; item f29 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.bss local=false zero=true
; item g2 section=.bss local=false zero=true
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item gp9 section=.data local=false zero=false
; item @2.s14 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=8 at=auto def=true init=$00,$01,$00,$00,$00,$00,$00,$00
; zp z5 size=4 at=auto def=true init=$00,$00,$00,$00
; zp z6 size=1 at=auto def=true init=$00
; zp g3 size=1 at=auto def=true init=$00 weight=13
; zp @11.s26 size=1 at=auto def=true init=$02 weight=10 local=true
; zp @21.s36 size=4 at=auto def=true init=$03,$00,$00,$00 weight=16 local=true
; runtime hargs_size=32 hargs_need=5
; @zp-link-meta-end

mix:
   SUBROUTINE

   LDA   acc
   ASL
   STA   __local_mix__2
   LDA   acc+1
   ROL
   STA   __local_mix__3
   LDA   #$00
   ROL
   ORA   __local_mix__2
   LDX   __local_mix__3
   STX   __local_mix__0
   EOR   __zpabi_mix__v_0
   STA   __local_mix__1
   LDA   __local_mix__0
   EOR   __zpabi_mix__v_1
   STA   __local_mix__0
   LDA   __local_mix__1
   EOR   __zpabi_mix__v_2
   STA   acc
   LDA   __local_mix__0
   EOR   __zpabi_mix__v_3
   STA   acc+1
   RTS

fin0:
   SUBROUTINE

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   JSR   fin0@run0
   LDA   g2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run3
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run3
   LDA   g3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   JMP   fin0@tail0

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   LDA   z5+2
   STA   __zpabi_mix__v_2
   LDA   z5+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   z6
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin1@run2
   LDA   ga7
   STA   __zpabi_mix__v_0
   LDA   ga7+1
   JSR   fin0@run0
   LDA   ga7+2
   STA   __zpabi_mix__v_0
   LDA   ga7+3
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin1@run2:
   SUBROUTINE

   JSR   fin0@run3
   LDA   z6
   STA   __zpabi_mix__v_0
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga7+4
   STA   __zpabi_mix__v_0
   LDA   ga7+5
   JSR   fin0@run0
   LDA   ga7+6
   STA   __zpabi_mix__v_0
   LDA   ga7+7
   JSR   fin0@run0
   LDA   gs8+1
   STA   __zpabi_mix__v_0
   JSR   fin2@run1
   LDA   gs8+2
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin2@run1:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

f10:
   SUBROUTINE

   STA   __local_f10__pp11
   STX   __local_f10__2
   LDA   @2.s14
   STA   __local_f10__1
   LDY   #$00
   LDA   (__local_f10__pp11),Y
   CLC
   ADC   #$01
   STA   __local_f10__0
   STA   (__local_f10__pp11),Y
   LDA   __local_f10__1
   ORA   __local_f10__0
   ORA   #$00
   BEQ   .if_else@1
   TYA
   RTS
.if_else@1:
   LDA   z6
   BMI   .sx_neg@6
   TYA
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin1@run2
   LDA   @2.s14
   BEQ   .if_else@3
   INC   ga7
   BNE   .inc_done@0
   INC   ga7+1
.inc_done@0:
   LDA   gs8+2
   SEC
   SBC   z4
   LDA   #$00
   SBC   z4+1
   LDA   #$00
   SBC   z4+2
   LDA   #$00
   SBC   z4+3
   LDA   #$00
   SBC   z4+4
   LDA   #$00
   SBC   z4+5
   LDA   #$00
   SBC   z4+6
   LDA   #$00
   SBC   z4+7
   LDA   #$00
   ROL
   RTS
.if_else@3:
   LDA   gs8+2
   STA   __local_f10__1
   ASL
   CLC
   ADC   __local_f10__1
   STA   gs8+2
   LDA   #$FF
   STA   __zpabi_mix__v_0
   JSR   fin2@run1
   LDA   #$80
   RTS

f17:
   SUBROUTINE

   LDA   #$7F
   STA   __local_f17__l24
   LDA   #$00
   STA   __local_f17__1
   STA   __local_f17__l25
   LDX   #>__local_f17__l25
   LDA   #<__local_f17__l25
   JSR   f10
   STA   __local_f17__l25
   LDA   #$7F
   SEC
   SBC   z5
   LDA   #$00
   SBC   z5+1
   LDA   #$00
   SBC   z5+2
   LDA   #$00
   SBC   z5+3
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   #$00
   STA   __zpabi_mix__v_0
   JMP   .loop@7_continue
.f17@rot@0:
   LDA   @11.s26
   LDX   #$FF
   SEC
.div_loop@11:
   INX
   SBC   #$03
   BCS   .div_loop@11
   ADC   #$03
   AND   #$07
   BEQ   .switch@8_break
   LDA   #$FF
   STA   gs8+1
.switch@8_break:
   LDA   __local_f17__0
   STA   __zpabi_mix__v_0
.loop@7_continue:
   CLC
   ADC   #$01
   STA   __local_f17__0
   LDA   __zpabi_mix__v_0
   CMP   #$04
   BCC   .f17@rot@0
   LDA   gs8+2
   ASL
   LDA   #$00
   ROL
   STA   gs8+2
   LDA   __local_f17__l24
   STA   __zpabi_mix__v_0
   LDA   __local_f17__1
   LDA   __zpabi_mix__v_0
   RTS

f29:
   SUBROUTINE

   LDA   #$00
   STA   __local_f29__l34
   LDA   #$80
   STA   __local_f29__l34+1
   LDA   #$FF
   STA   __local_f29__l35
   STA   __local_f29__l35+1
   LDA   gs8+2
   ORA   #$81
   STA   gs8+2
   STA   __zpabi_mix__v_0
   JSR   fin2@run1
   LDY   #$01
   LDA   (__zpabi_f29__pp32_0),Y
   STA   __local_f29__1
   DEY
   LDA   (__zpabi_f29__pp32_0),Y
   CLC
   ADC   #$03
   STA   __local_f29__0
   BCC   .carry_inc@0
   INC   __local_f29__1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f29__pp32_0),Y
   LDA   __local_f29__1
   INY
   STA   (__zpabi_f29__pp32_0),Y
   STA   g1+1
   LDA   __local_f29__0
   STA   g1
   LDA   #$00
   STA   __local_f29__w37
.loop@12_start:
   AND   #$03
   ASL
   TAX
   LDA   ga7,X
   AND   #$81
   STA   ga7,X
   LDA   #$00
   STA   ga7+1,X
   LDX   #$06
.shift_loop@12:
   ASL   __local_f29__l34
   ROL   __local_f29__l34+1
   DEX
   BNE   .shift_loop@12
   LDA   __local_f29__w37
   STA   __zpabi_f17__p19_0
   LDA   #$00
   STA   __zpabi_f17__p19_1
   STA   __zpabi_f17__p19_2
   STA   __zpabi_f17__p19_3
   STA   __local_f29__w38
.loop@13_start:
   LDA   #$FF
   CMP   gs8+2
   BNE   .cmp_true@14
   LDA   #$00
   BEQ   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   STA   __local_f29__2
   LDA   __local_f29__w38
   STA   HARGS
   LDA   __zpabi_f29__p31_0
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   __zpabi_f17__p20
   LDA   #<__local_f29__l34
   STA   __zpabi_f17__pp21_0
   LDA   #>__local_f29__l34
   STA   __zpabi_f17__pp21_1
   LDA   __local_f29__2
   JSR   f17
   STA   __local_f29__l35
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f29__l35+1
   INC   __local_f29__w38
   LDA   __local_f29__w38
   CMP   #$04
   BCC   .loop@13_start
   LDA   __local_f29__l35
   ORA   __local_f29__l35+1
   BEQ   .if_end@9
   LDA   __local_f29__l34
   STA   __local_f29__1
   LDA   __local_f29__l34+1
   STA   __local_f29__0
   LDA   @21.s36
   SBC   __local_f29__1
   LDA   @21.s36+1
   SBC   __local_f29__0
   LDA   @21.s36+2
   SBC   #$00
   LDA   @21.s36+3
   SBC   #$00
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   LDX   @21.s36
   STX   __zpabi_f17__p19_0
   LDX   @21.s36+1
   STX   __zpabi_f17__p19_1
   LDX   @21.s36+2
   STX   __zpabi_f17__p19_2
   LDX   @21.s36+3
   STX   __zpabi_f17__p19_3
   LDX   #$02
   STX   __zpabi_f17__p20
   LDX   #<__local_f29__l35
   STX   __zpabi_f17__pp21_0
   LDX   #>__local_f29__l35
   STX   __zpabi_f17__pp21_1
   JSR   f17
   LDX   #$00
   LDA   #$03
   RTS
.if_end@9:
   INC   __local_f29__w37
   LDA   __local_f29__w37
   BNE   .lb_skip@0
   JMP   .loop@12_start
.lb_skip@0:
   LDA   z5
   ORA   #$05
   STA   z5
   LDX   __local_f29__l34+1
   LDA   __local_f29__l34
   RTS

main:
   SUBROUTINE

   LDA   #$52
   STA   __local_main__l39
   LDA   #$23
   STA   __local_main__0
   LDA   ga7+2
   STA   __local_main__l39
   LDA   ga7+3
   STA   __local_main__0
   INC   __local_main__l39
   BNE   .inc_done@1
   INC   __local_main__0
.inc_done@1:
   LDA   __local_main__l39
   STA   ga7+2
   STA   __zpabi_mix__v_0
   LDA   __local_main__0
   STA   ga7+3
   JSR   fin0@run0
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

g1:
   DS.B  2

g2:
   DS.B  1

ga7:
   DC.W  $3375
   DC.W  $00FF
   DC.W  $0080
   DC.W  $0003

gs8:
   DS.B  2
   DC.B  $FF

gp9:
   DC.W  z5

@2.s14:
   DC.B  $04

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g3
   STA   z4
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   STA   z5
   STA   z5+1
   STA   z5+2
   STA   z5+3
   STA   z6
   STA   @21.s36+1
   STA   @21.s36+2
   STA   @21.s36+3
   LDA   #$01
   STA   z4+1
   LDA   #$02
   STA   @11.s26
   LDA   #$03
   STA   @21.s36
   RTS
