SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$2E
z5	EQU	$2F
g1	EQU	$31
gp9	EQU	$33
__zpabi_f17__p19	EQU	$80
__zpabi_f17__p20_0	EQU	$81
__zpabi_f17__p20_1	EQU	$82
__zpabi_f10__p11_0	EQU	$83
__zpabi_f10__p11_1	EQU	$84
__zpabi_f10__p11_2	EQU	$85
__zpabi_f10__p11_3	EQU	$86
__zpabi_f10__p11_4	EQU	$87
__zpabi_f10__p11_5	EQU	$88
__zpabi_f10__p11_6	EQU	$89
__zpabi_f10__p11_7	EQU	$8A
__zpabi_f10__p12	EQU	$8B
__zpabi_f10__p13_0	EQU	$8C
__zpabi_f10__p13_1	EQU	$8D
__zpabi_f10__p13_2	EQU	$8E
__zpabi_f10__p13_3	EQU	$8F
__zpabi_f10__pp14_0	EQU	$90
__zpabi_f10__pp14_1	EQU	$91
__zpabi_mix__v_0	EQU	$92
__zpabi_mix__v_1	EQU	$93
__zpabi_mix__v_2	EQU	$94
__zpabi_mix__v_3	EQU	$95
__local_f17__0	EQU	$98
__local_f17__1	EQU	$99
__local_f17__4	EQU	$9C
__local_f17__5	EQU	$9D
__local_f17__6	EQU	$9E
__local_f17__l22	EQU	$A4
__local_f10__l15	EQU	$A8
__local_f10__0	EQU	$A9
__local_f10__1	EQU	$AA
__local_mix__0	EQU	$AB
__local_mix__1	EQU	$AC
__local_mix__2	EQU	$AD
__local_mix__3	EQU	$AE

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11_0,__zpabi_f10__p11_1,__zpabi_f10__p11_2,__zpabi_f10__p11_3,__zpabi_f10__p11_4,__zpabi_f10__p11_5,__zpabi_f10__p11_6,__zpabi_f10__p11_7,__zpabi_f10__p12,__zpabi_f10__p13_0,__zpabi_f10__p13_1,__zpabi_f10__p13_2,__zpabi_f10__p13_3,__zpabi_f10__pp14_0,__zpabi_f10__pp14_1 locals=__local_f10__l15,__local_f10__0,__local_f10__1 indirect=false in_cycle=false
; def f17 params=__zpabi_f17__p18_0,__zpabi_f17__p18_1,__zpabi_f17__p19,__zpabi_f17__p20_0,__zpabi_f17__p20_1 locals=__local_f17__0,__local_f17__1,__local_f17__2,__local_f17__3,__local_f17__4,__local_f17__5,__local_f17__6,__local_f17__7,__local_f17__8,__local_f17__9,__local_f17__10,__local_f17__11,__local_f17__l22,__local_f17__13,__local_f17__14,__local_f17__15 indirect=false in_cycle=false param_regs=A,X,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call f17 -> f10
; call f17 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f17
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin0@tail0@run4 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f17 section=.text local=false zero=false
; item f17@run6 section=.text local=true zero=false
; item f17@run3 section=.text local=true zero=false
; item f17@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=8 at=auto def=true init=$FF,$7F,$00,$00,$00,$00,$00,$00
; zp z4 size=1 at=auto def=true init=$FF
; zp z5 size=2 at=auto def=true init=$01,$00
; zp g1 size=2 at=auto def=true init=$00,$01 weight=131
; zp gp9 size=2 at=auto def=true init=<g2,>g2 weight=65
; runtime hargs_size=32 hargs_need=0
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
   JSR   fin0@run2
   LDA   g2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run5
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   STA   __zpabi_mix__v_1
   LDA   z3+2
   STA   __zpabi_mix__v_2
   LDA   z3+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   z4
   JMP   fin0@tail0

fin0@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   JSR   fin0@tail0@run4
   JMP   mix

fin0@tail0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   JSR   fin0@run2
   LDA   ga6
   JSR   fin1@run0
   LDA   ga6+1
   JSR   fin1@run0
   LDA   ga6+2
   JMP   fin0@tail0

fin1@run0:
   SUBROUTINE

   JSR   fin0@tail0@run4
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+3
   JSR   fin1@run0
   LDA   gs7+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run5
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   gs7+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run5
   LDA   gs7+2
   STA   __zpabi_mix__v_0
   JMP   mix

f10:
   SUBROUTINE

   LDA   #$01
   STA   __local_f10__l15
   LDY   #$00
   LDA   (__zpabi_f10__pp14_0),Y
   JSR   fin1@run0
   LDA   #$01
   CLC
   ADC   ga6
   LDY   #$00
   STA   (gp9),Y
   LDA   #$00
   STA   __local_f10__l15
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDA   (DPTR),Y
   STA   __local_f10__1
   LDA   gs7+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f10__0
   LDA   __local_f10__1
   EOR   gs7+1
   STA   __local_f10__l15
   LDA   #$00
   EOR   __local_f10__0
   TAX
   LDA   __local_f10__l15
   RTS

f17:
   SUBROUTINE

   LDA   #$08
   STA   __local_f17__l22
   LDA   #$00
   STA   __local_f17__l22+1
   STA   __local_f17__l22+2
   STA   __local_f17__l22+3
   LDA   z5
   SEC
   SBC   z5
   STA   __zpabi_f10__p12
   LDA   g2
   AND   z4
   STA   __zpabi_f10__p13_0
   LDA   #$01
   STA   __zpabi_f10__p11_0
   LDA   #$00
   STA   __zpabi_f10__p11_1
   JSR   f17@run3
   STA   __zpabi_f10__p13_1
   STA   __zpabi_f10__p13_2
   STA   __zpabi_f10__p13_3
   JSR   f17@run1
   LDA   g1
   AND   #$01
   STA   __zpabi_f10__p11_0
   LDA   #$00
   STA   __zpabi_f10__p11_1
   LDA   z5
   AND   g1
   STA   __zpabi_f10__p13_0
   LDA   z5+1
   AND   g1+1
   STA   __zpabi_f10__p13_1
   LDA   #$00
   JSR   f17@run3
   STA   __zpabi_f10__p13_2
   STA   __zpabi_f10__p13_3
   LDA   z3
   STA   __zpabi_f10__p12
   JSR   f17@run1
   STA   __local_f17__1
   TXA
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f17__1
   JSR   f17@run6
   LDA   gs7+1
   STA   __local_f17__6
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f17__5
   STA   __local_f17__4
   STA   __zpabi_f10__p12
   LSR
   LDA   __local_f17__5
   ROR
   STA   __zpabi_f10__p13_0
   LDA   __local_f17__6
   ROR
   STA   __local_f17__1
   LDA   #$00
   ROR
   STA   __local_f17__0
   LDA   #$00
   SEC
   SBC   __local_f17__6
   STA   __zpabi_f10__p11_0
   LDA   __local_f17__0
   SBC   __local_f17__5
   STA   __zpabi_f10__p11_1
   LDA   __local_f17__1
   SBC   __local_f17__4
   STA   __zpabi_f10__p11_2
   LDA   __zpabi_f10__p13_0
   SBC   __zpabi_f10__p12
   STA   __zpabi_f10__p11_3
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_f10__p11_4
   LDA   z4
   SEC
   SBC   #$FF
   LDA   #$00
   SBC   #$7F
   LDA   #$00
   ROL
   STA   __zpabi_f10__p12
   LDA   z5
   SEC
   SBC   ga6+1
   LDA   z5+1
   SBC   #$00
   LDA   #$00
   ROL
   STA   __zpabi_f10__p13_0
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   __zpabi_f10__p13_2
   STA   __zpabi_f10__p13_3
   LDA   __zpabi_f10__p11_4
   STA   __zpabi_f10__p11_5
   STA   __zpabi_f10__p11_6
   STA   __zpabi_f10__p11_7
   LDA   #$00
   STA   __zpabi_f10__p13_1
   JSR   f17@run1
   STA   __local_f17__1
   TXA
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f17__1
   JSR   f17@run6
   LDA   gs7+2
   CLC
   ADC   #$03
   STA   gs7+2
   LDA   __local_f17__l22
   STA   __local_f17__0
   LDA   __local_f17__l22+1
   ASL   __local_f17__0
   ROL
   ASL   __local_f17__0
   ROL
   TAX
   LDA   __local_f17__0
   RTS

f17@run6:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JMP   mix

f17@run3:
   SUBROUTINE

   STA   __zpabi_f10__p11_2
   STA   __zpabi_f10__p11_3
   STA   __zpabi_f10__p11_4
   STA   __zpabi_f10__p11_5
   STA   __zpabi_f10__p11_6
   STA   __zpabi_f10__p11_7
   RTS

f17@run1:
   SUBROUTINE

   LDA   #<__local_f17__l22
   STA   __zpabi_f10__pp14_0
   LDA   #>__local_f17__l22
   STA   __zpabi_f10__pp14_1
   JMP   f10

main:
   SUBROUTINE

   LDA   ga6+2
   BEQ   .and_false@0
   LDA   #$01
   STA   __zpabi_f17__p20_0
   LDA   #$00
   BEQ   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __zpabi_f17__p20_0
.and_end@1:
   STA   __zpabi_f17__p19
   STA   __zpabi_f17__p20_1
   LDX   #$7F
   LDA   #$FE
   JSR   f17
   DEC   gs7+1
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

g2:
   DC.B  $FF

ga6:
   DC.B  $00, $03, $00, $FF

gs7:
   DS.B  1
   DC.B  $FF
   DC.B  $07

gp8:
   DC.W  z4

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   z3+2
   STA   z3+3
   STA   z3+4
   STA   z3+5
   STA   z3+6
   STA   z3+7
   STA   z5+1
   LDA   #$01
   STA   g1+1
   STA   z5
   LDA   #$FF
   STA   z3
   STA   z4
   LDA   #$7F
   STA   z3+1
   LDA   #<g2
   STA   gp9
   LDA   #>g2
   STA   gp9+1
   RTS
