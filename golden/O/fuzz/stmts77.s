SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$27
z6	EQU	$29
g2	EQU	$2A
g3	EQU	$2C
gp10	EQU	$2D
g1	EQU	$2F
gp9	EQU	$30
__local_f23__0	EQU	$80
__zpabi_f16__p19_0	EQU	$80
__local_f23__1	EQU	$81
__zpabi_f16__p19_1	EQU	$81
__local_f23__pp24	EQU	$82
__zpabi_mix__v_0	EQU	$82
__local_f23__2	EQU	$83
__zpabi_mix__v_1	EQU	$83
__local_f23__l26	EQU	$84
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_main__w33	EQU	$86
__local_main__1	EQU	$88
__local_main__2	EQU	$89
__local_main__l32	EQU	$8A
__local_f11__l14	EQU	$8C
__local_f16__w22	EQU	$8C
__local_f11__0	EQU	$8D
__local_f16__p18	EQU	$8D
__local_f11__1	EQU	$8E
__local_f16__0	EQU	$8E
__local_f11__2	EQU	$8F
__local_f16__l20_3	EQU	$8F
__local_f11__3	EQU	$90
__local_f16__l21	EQU	$90
__local_f11__4	EQU	$91
__local_f16__l20_2	EQU	$91
__local_f11__5	EQU	$92
__local_f16__l20_1	EQU	$92
__local_f11__6	EQU	$93
__local_f16__l20_0	EQU	$93
__local_mix__0	EQU	$95
__local_mix__1	EQU	$96
__local_mix__2	EQU	$97
__local_mix__3	EQU	$98

; @zp-link-meta-begin
; def f11 params=__zpabi_f11__pp12_0,__zpabi_f11__pp12_1 locals=__local_f11__l14,__local_f11__0,__local_f11__1,__local_f11__2,__local_f11__3,__local_f11__4,__local_f11__5,__local_f11__6 indirect=false in_cycle=false param_regs=A,X
; def f16 params=__zpabi_f16__p17,__zpabi_f16__p18,__zpabi_f16__p19_0,__zpabi_f16__p19_1 locals=__local_f16__w22,__local_f16__p18,__local_f16__0,__local_f16__l20_3,__local_f16__l21,__local_f16__l20_2,__local_f16__l20_1,__local_f16__l20_0,__local_f16__1 indirect=false in_cycle=false param_regs=A,X,,
; def f23 params=__zpabi_f23__pp24_0,__zpabi_f23__pp24_1 locals=__local_f23__0,__local_f23__1,__local_f23__pp24,__local_f23__2,__local_f23__l26 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__w33,__local_main__0,__local_main__1,__local_main__2,__local_main__l32,__local_main__4 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f16 -> mix
; call f23 -> f11
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f16
; call main -> f23
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run2 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run3 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f11 section=.text local=false zero=false
; item f16 section=.text local=false zero=false
; item f23 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=1 at=auto def=true init=$09
; zp z5 size=2 at=auto def=true init=$7F,$00
; zp z6 size=1 at=auto def=true init=$7F
; zp g2 size=2 at=auto def=true init=$FF,$00 weight=227
; zp g3 size=1 at=auto def=true init=$FF weight=44
; zp gp10 size=2 at=auto def=true init=<z5,>z5 weight=85
; zp g1 size=1 at=auto def=true init=$00 weight=17
; zp gp9 size=2 at=auto def=true init=<z6,>z6 weight=27
; runtime hargs_size=32 hargs_need=11
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
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run1
   LDA   g3
   JSR   fin0@run0
   LDA   z4
   JMP   fin0@tail0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run2
   JMP   mix

fin0@run0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@tail0:
   SUBROUTINE

   JSR   fin0@run0@run2
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   JSR   fin0@run1
   LDA   z6
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z6
   JSR   fin1@run3
   LDA   ga7
   JSR   fin0@run0
   LDA   ga7+1
   JMP   fin0@tail0

fin1@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga7+2
   JSR   fin0@run0
   LDA   ga7+3
   JSR   fin0@run0
   LDA   gs8+1
   JSR   fin0@run0
   LDA   gs8+2
   JMP   fin0@tail0

f11:
   SUBROUTINE

   LDA   #$74
   STA   __local_f11__l14
   LDA   #$9A
   STA   __local_f11__0
   LDA   #$29
   STA   __local_f11__1
   LDA   #$E0
   STA   __local_f11__2
   LDA   #$0E
   STA   __local_f11__3
   LDA   #$94
   STA   __local_f11__4
   LDA   #$F0
   STA   __local_f11__5
   LDA   #$E3
   STA   __local_f11__6
   LDA   g3
   AND   #$03
   LDX   #$00
   RTS

f16:
   SUBROUTINE

   LDY   #$00
   LDA   (gp9),Y
   STA   g2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   g2+1
   TXA
   AND   __zpabi_f16__p19_0
   JSR   fin0@run0
   LDA   #$FF
   STA   __local_f16__l20_0
   STA   __local_f16__l20_1
   STA   __local_f16__l20_2
   STA   __local_f16__l21
   LDA   #$7F
   STA   __local_f16__l20_3
   LDA   #$00
   STA   __local_f16__w22
   JMP   .loop@10_continue
.f16@rot@0:
   LDY   #$00
   LDA   (gp9),Y
   STA   __local_f16__l20_0
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f16__l20_1
   STA   __local_f16__l20_2
   STA   __local_f16__l20_3
   LDA   gs8+1
   AND   __local_f16__l20_0
   ORA   __local_f16__p18
   STA   g3
   LDA   #$FE
   STA   __local_f16__l21
   LDA   __local_f16__p18
   STA   __local_f16__w22
.loop@10_continue:
   CLC
   ADC   #$01
   STA   __local_f16__p18
   LDA   __local_f16__w22
   BEQ   .f16@rot@0
   LDA   z4
   EOR   #$5A
   STA   z4
   BEQ   .if_end@0
   LDY   #$00
   LDA   (gp10),Y
   EOR   #$FF
   RTS
.if_end@0:
   LDY   #$00
   LDA   (gp9),Y
   SEC
   SBC   #$05
   STA   __local_f16__p18
   STA   (gp9),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f16__0
   LDA   __local_f16__p18
   EOR   ga7+3
   ORA   __local_f16__0
   BEQ   .if_end@1
   LDA   gs8+1
   STA   __local_f16__l20_3
   LDY   #$00
   LDA   (gp10),Y
   STA   __local_f16__0
   INY
   LDA   (gp10),Y
   STA   __local_f16__p18
   LDA   __local_f16__0
   SEC
   SBC   #$01
   BCS   .carry_inc@0
   DEC   __local_f16__p18
.carry_inc@0:
   LDY   #$00
   STA   (gp10),Y
   LDA   __local_f16__p18
   INY
   STA   (gp10),Y
   LDA   __local_f16__l21
   STA   HARGS
   LDA   __local_f16__l20_3
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   EOR   __local_f16__0
   RTS
.if_end@1:
   LDA   z4
   LDA   __zpabi_f16__p19_1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f16__p18
   STA   __local_f16__w22
   LDA   __zpabi_f16__p19_0
   CLC
   SBC   __local_f16__l20_0
   LDA   __zpabi_f16__p19_1
   SBC   __local_f16__l20_1
   LDA   __local_f16__p18
   SBC   __local_f16__l20_2
   LDA   __local_f16__w22
   SBC   __local_f16__l20_3
   LDA   #$00
   ROL
   RTS

f23:
   SUBROUTINE

   STA   __local_f23__pp24
   STX   __local_f23__2
   LDA   #$FF
   STA   __local_f23__l26
   LDA   g3
   STA   __local_f23__1
   CLC
   ADC   #$01
   STA   g3
   LDA   __local_f23__1
   AND   #$07
   CMP   #$05
   BNE   .default@14
   LDA   z4
   SEC
   SBC   #$01
   STA   z4
   LDY   #$00
   LDA   (gp9),Y
   STA   __local_f23__1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f23__0
   LDA   z5
   CLC
   SBC   __local_f23__1
   LDA   z5+1
   SBC   __local_f23__0
   LDA   #$00
   ROL
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$07
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   STA   gs8+2
   LDA   g2
   BNE   .carry_inc@1
   DEC   g2+1
.carry_inc@1:
   DEC   g2
   RTS
.default@14:
   LDA   gs8+2
   ORA   #$05
   STA   gs8+2
   LDY   #$00
   STA   (__local_f23__pp24),Y
   LDA   #$00
   INY
   STA   (__local_f23__pp24),Y
   LDA   ga7
   EOR   #$01
   STA   ga7
   LDX   #>__local_f23__l26
   LDA   #<__local_f23__l26
   JSR   f11
   LDA   #$00
   RTS

main:
   SUBROUTINE

   LDA   z6
   BEQ   .and_false@2
   LDA   gs8+2
   BEQ   .and_false@2
   LDA   #$01
   STA   g2
   LDA   #$00
   BEQ   .and_end@3
.and_false@2:
   LDA   #$00
   STA   g2
.and_end@3:
   STA   g2+1
   LDA   #$FF
   STA   __local_main__l32
   LDA   #$00
   STA   __local_main__l32+1
   STA   __local_main__w33
.loop@17_continue:
   CLC
   ADC   #$01
   STA   __local_main__2
   LDA   __local_main__w33
   CMP   #$03
   BCC   .lb_skip@0
   JMP   .loop@17_break
.lb_skip@0:
   LSR   g2+1
   ROR   g2
   LDA   g2
   ORA   g2+1
   BEQ   .if_else@5
   LDA   __local_main__l32
   STA   HARGS
   LDA   __local_main__l32+1
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDY   #$00
   LDA   (gp10),Y
   STA   __zpabi_mix__v_1
   INY
   LDA   (gp10),Y
   BNE   .thread@0
   LDA   __zpabi_mix__v_1
   CMP   #$FF
   BEQ   .cmp_true@12
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   TAX
   LDA   __local_main__l32
   STA   __zpabi_mix__v_1
   LDA   __local_main__l32+1
   STA   __local_main__w33
   LDA   #$01
   SEC
   SBC   __zpabi_mix__v_1
   STA   __zpabi_f16__p19_0
   LDA   #$00
   SBC   __local_main__w33
   STA   __zpabi_f16__p19_1
   LDA   HARGS+4
   JSR   f16
.xjump@0:
   LDA   __local_main__2
   STA   __local_main__w33
   JMP   .loop@17_continue
.if_else@5:
   LDX   #>__local_main__l32
   LDA   #<__local_main__l32
   JSR   f23
   LDA   gs8+1
   AND   #$07
   CMP   #$01
   BEQ   .switch@18_break
   CMP   #$05
   BEQ   .switch@18_break
   LDA   g1
   EOR   #$03
   STA   __local_main__1
   STA   g1
   LDA   g3
   STA   __local_main__w33
   LSR
   LSR
   LSR
   LSR
   STA   __zpabi_mix__v_1
   LDA   __local_main__w33
   ASL
   ASL
   ASL
   ASL
   ORA   __local_main__1
   STA   __local_main__w33
   LDA   __zpabi_mix__v_1
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__w33
   JSR   fin1@run3
   LDA   __local_main__l32+1
   STA   __zpabi_mix__v_1
   ASL
   ROR   __zpabi_mix__v_1
   LDA   __local_main__l32
   ROR
   STA   __local_main__w33
   LDA   __zpabi_mix__v_1
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__w33
   JSR   fin1@run3
.switch@18_break:
   LDA   g2
   ORA   #$5A
   STA   g2
   JMP   .xjump@0
.loop@17_break:
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

ga7:
   DC.B  $00, $4C, $00, $00

gs8:
   DS.B  1
   DC.B  $80
   DS.B  1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g2+1
   STA   z5+1
   LDA   #$FF
   STA   g2
   STA   g3
   LDA   #$09
   STA   z4
   LDA   #$7F
   STA   z5
   STA   z6
   LDA   #<z6
   STA   gp9
   LDA   #>z6
   STA   gp9+1
   LDA   #<z5
   STA   gp10
   LDA   #>z5
   STA   gp10+1
   RTS
