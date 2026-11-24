SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$28
z6	EQU	$29
g2	EQU	$2B
g1	EQU	$2C
__zpabi_f14__p15_0	EQU	$80
__zpabi_f9__pp11_0	EQU	$80
__zpabi_f14__p15_1	EQU	$81
__zpabi_f9__pp11_1	EQU	$81
__zpabi_f14__p15_2	EQU	$82
__zpabi_mix__v_0	EQU	$82
__zpabi_f14__p15_3	EQU	$83
__zpabi_mix__v_1	EQU	$83
__zpabi_f14__p15_4	EQU	$84
__zpabi_mix__v_2	EQU	$84
__zpabi_f14__p15_5	EQU	$85
__zpabi_mix__v_3	EQU	$85
__zpabi_f14__p15_6	EQU	$86
__local_f9__1	EQU	$87
__zpabi_f14__p15_7	EQU	$87
__zpabi_f14__p17_0	EQU	$88
__zpabi_f14__p17_1	EQU	$89
__zpabi_f14__p17_2	EQU	$8A
__zpabi_f14__p17_3	EQU	$8B
__zpabi_f14__p17_4	EQU	$8C
__local_f9__p10	EQU	$8D
__zpabi_f14__p17_5	EQU	$8D
__zpabi_f14__p17_6	EQU	$8E
__zpabi_f14__p17_7	EQU	$8F
__zpabi_f14__p16	EQU	$90
__local_main__w24	EQU	$91
__local_main__1	EQU	$93
__local_main__l22_7	EQU	$9A
__local_main__l22_6	EQU	$9B
__local_main__l22_5	EQU	$9C
__local_main__l22_4	EQU	$9D
__local_main__l22_3	EQU	$9E
__local_main__l22_2	EQU	$9F
__local_main__l22_1	EQU	$A0
__local_main__l22_0	EQU	$A1
__local_f14__l18	EQU	$A2
__local_mix__0	EQU	$A2
__local_mix__1	EQU	$A3
__local_mix__2	EQU	$A4
__local_mix__3	EQU	$A5

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15_0,__zpabi_f14__p15_1,__zpabi_f14__p15_2,__zpabi_f14__p15_3,__zpabi_f14__p15_4,__zpabi_f14__p15_5,__zpabi_f14__p15_6,__zpabi_f14__p15_7,__zpabi_f14__p16,__zpabi_f14__p17_0,__zpabi_f14__p17_1,__zpabi_f14__p17_2,__zpabi_f14__p17_3,__zpabi_f14__p17_4,__zpabi_f14__p17_5,__zpabi_f14__p17_6,__zpabi_f14__p17_7 locals=__local_f14__l18,__local_f14__0 indirect=false in_cycle=false
; def f9 params=__zpabi_f9__p10,__zpabi_f9__pp11_0,__zpabi_f9__pp11_1 locals=__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__4,__local_f9__5,__local_f9__6,__local_f9__p10 indirect=false in_cycle=false param_regs=A,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__w24,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__l22_7,__local_main__l22_6,__local_main__l22_5,__local_main__l22_4,__local_main__l22_3,__local_main__l22_2,__local_main__l22_1,__local_main__l22_0 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f14
; call main -> fin0
; call main -> fin1
; call main -> fin2
; share member=f14:__zpabi_f14__p16 holder=main live=main:f14 writer=main
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g3 section=.data local=false zero=false
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item @8.s21 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$02,$00
; zp z5 size=1 at=auto def=true init=$00
; zp z6 size=2 at=auto def=true init=$00,$80
; zp g2 size=1 at=auto def=true init=$01 weight=130
; zp g1 size=1 at=auto def=true init=$01 weight=67
; runtime hargs_size=32 hargs_need=9
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run4
   LDA   g1
   JSR   fin0@run5
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   g3+2
   STA   __zpabi_mix__v_2
   LDA   g3+3
   JSR   fin0@run0
   LDA   z4+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run4
   LDA   z5
   JSR   fin0@run5
   LDA   z6+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   ga7
   STA   __zpabi_mix__v_0
   LDA   ga7+1
   STA   __zpabi_mix__v_1
   LDA   ga7+2
   STA   __zpabi_mix__v_2
   LDA   ga7+3
   JSR   fin0@run0
   LDA   ga7+4
   STA   __zpabi_mix__v_0
   LDA   ga7+5
   STA   __zpabi_mix__v_1
   LDA   ga7+6
   STA   __zpabi_mix__v_2
   LDA   ga7+7
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga7+8
   STA   __zpabi_mix__v_0
   LDA   ga7+9
   STA   __zpabi_mix__v_1
   LDA   ga7+10
   STA   __zpabi_mix__v_2
   LDA   ga7+11
   JSR   fin0@run0
   LDA   ga7+12
   STA   __zpabi_mix__v_0
   LDA   ga7+13
   STA   __zpabi_mix__v_1
   LDA   ga7+14
   STA   __zpabi_mix__v_2
   LDA   ga7+15
   JSR   fin0@run0
   LDA   gs8+1
   STA   __zpabi_mix__v_0
   LDA   gs8+2
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   gs8+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run4
   LDA   gs8+3
   STA   __zpabi_mix__v_0
   JMP   mix

f9:
   SUBROUTINE

   STA   __local_f9__p10
   LDA   #$03
   CLC
   ADC   ga7+8
   STA   __zpabi_mix__v_1
   LDA   #$00
   ADC   ga7+9
   STA   __local_f9__1
   LDA   #$00
   ADC   ga7+10
   STA   __zpabi_mix__v_2
   LDA   #$00
   ADC   ga7+11
   LSR
   ROR   __zpabi_mix__v_2
   ROR   __local_f9__1
   ROR   __zpabi_mix__v_1
   LSR
   STA   __zpabi_mix__v_3
   ROR   __zpabi_mix__v_2
   ROR   __local_f9__1
   ROR   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   LDA   __local_f9__1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g3
   STA   HARGS
   LDA   g3+1
   STA   HARGS+1
   LDA   g3+2
   STA   HARGS+2
   LDA   g3+3
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   __zpabi_mix__v_0
   LDA   HARGS+6
   STA   __zpabi_mix__v_1
   LDA   HARGS+7
   STA   __zpabi_mix__v_2
   LDA   HARGS+8
   JSR   fin0@run0
   INC   g3
   BNE   .inc_done@0
   INC   g3+1
   BNE   .inc_done@0
   INC   g3+2
   BNE   .inc_done@0
   INC   g3+3
.inc_done@0:
   LDA   #$03
   AND   g3
   STA   __zpabi_mix__v_0
   LDA   #$00
   AND   g3+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   AND   g3+2
   STA   __zpabi_mix__v_2
   LDA   #$00
   AND   g3+3
   JSR   fin0@run0
   LDY   #$00
   LDA   (__zpabi_f9__pp11_0),Y
   ORA   #$5A
   STA   __zpabi_mix__v_3
   STA   (__zpabi_f9__pp11_0),Y
   INY
   LDA   (__zpabi_f9__pp11_0),Y
   STA   __zpabi_mix__v_2
   LDA   gs8+3
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_3
   ORA   gs8+3
   STA   __local_f9__1
   LDA   __zpabi_mix__v_2
   ORA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f9__1
   JSR   fin0@run5
   LDA   __local_f9__p10
   LSR
   LSR
   LSR
   LDX   #$00
   RTS

f14:
   SUBROUTINE

   LDA   #$00
   STA   __local_f14__l18
   LDA   __local_f14__l18
   STA   __local_f14__l18
   LDA   g1
   EOR   #$FF
   STA   g1
   ORA   __local_f14__l18
   STA   gs8+3
   LDA   @8.s21
   AND   #$03
   STA   @8.s21
   STA   ga7+12
   LDA   #$00
   STA   @8.s21+1
   STA   ga7+13
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   ga7+14
   STA   ga7+15
   LDA   #$68
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   g2
   LDA   ga7+4
   CLC
   ADC   #$2C
   STA   __local_main__1
   LDA   z5
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   HARGS+1
   LDA   z5
   STA   HARGS
   LDA   #$05
   JSR   main@run2
   LDA   HARGS+7
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   JSR   main@run1
   LDA   #$04
   STA   __zpabi_f14__p15_0
   LDA   #$00
   STA   __zpabi_f14__p15_1
   STA   __zpabi_f14__p15_2
   JSR   main@run3
   LDA   __local_main__1
   STA   __zpabi_f14__p16
   LDA   HARGS+6
   STA   __zpabi_f14__p17_0
   LDA   HARGS+7
   STA   __zpabi_f14__p17_1
   JSR   f14
   STA   __local_main__l22_0
   LDA   #$00
   STA   __local_main__l22_1
   STA   __local_main__l22_2
   STA   __local_main__l22_3
   STA   __local_main__l22_4
   STA   __local_main__l22_5
   STA   __local_main__l22_6
   STA   __local_main__l22_7
   STA   __local_main__w24
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __zpabi_f14__p16
   LDA   __local_main__w24
   CMP   #$03
   BCC   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   g1
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __zpabi_f14__p15_1
   LDA   g1
   ADC   g2
   STA   __zpabi_f14__p15_0
   BCC   .carry_inc@0
   INC   __zpabi_f14__p15_1
.carry_inc@0:
   LDA   __zpabi_f14__p15_1
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __zpabi_f14__p15_2
   LDA   gs8+3
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __zpabi_f14__p17_1
   JSR   main@run1
   LDA   __zpabi_f14__p15_2
   JSR   main@run3
   LDA   gs8+3
   STA   __zpabi_f14__p17_0
   JSR   f14
   LDA   __zpabi_f14__p16
   STA   __local_main__1
   AND   #$07
   BEQ   .case@2
   CMP   #$01
   BEQ   .case@5
   CMP   #$06
   BEQ   .case@6
   BNE   .case@6
.case@2:
   LDA   z5
   EOR   __local_main__1
   AND   #$07
   CMP   #$06
   LDA   gs8+3
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   HARGS+1
   LDA   gs8+3
   STA   HARGS
   LDA   #$07
   JSR   main@run2
   LDA   HARGS+6
   STA   g2
   JMP   .case@6
.case@5:
   LDA   __local_main__l22_0
   SEC
   SBC   #$01
   STA   __local_main__l22_0
   LDA   __local_main__l22_1
   SBC   #$00
   STA   __local_main__l22_1
   LDA   __local_main__l22_2
   SBC   #$00
   STA   __local_main__l22_2
   LDA   __local_main__l22_3
   SBC   #$00
   STA   __local_main__l22_3
   LDA   __local_main__l22_4
   SBC   #$00
   STA   __local_main__l22_4
   LDA   __local_main__l22_5
   SBC   #$00
   STA   __local_main__l22_5
   LDA   __local_main__l22_6
   SBC   #$00
   STA   __local_main__l22_6
   BCS   .carry_inc@1
   DEC   __local_main__l22_7
.carry_inc@1:
.case@6:
   LDA   __zpabi_f14__p16
   STA   __local_main__w24
   JMP   .loop@0_continue
.loop@0_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run3:
   SUBROUTINE

   STA   __zpabi_f14__p15_3
   STA   __zpabi_f14__p15_4
   STA   __zpabi_f14__p15_5
   STA   __zpabi_f14__p15_6
   STA   __zpabi_f14__p15_7
   RTS

main@run2:
   SUBROUTINE

   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JMP   sdivmod16

main@run1:
   SUBROUTINE

   STA   __zpabi_f14__p17_2
   STA   __zpabi_f14__p17_3
   STA   __zpabi_f14__p17_4
   STA   __zpabi_f14__p17_5
   STA   __zpabi_f14__p17_6
   STA   __zpabi_f14__p17_7
   RTS

acc:
   DS.B  2

g3:
   DC.L  $00000080

ga7:
   DC.L  $00010000
   DC.L  $00000080
   DC.L  $00000007
   DC.L  $8B8C1AB3

gs8:
   DS.B  1
   DC.W  $0001
   DC.B  $FF

@8.s21:
   DC.W  $0005

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   STA   z5
   STA   z6
   LDA   #$01
   STA   g1
   STA   g2
   LDA   #$02
   STA   z4
   LDA   #$80
   STA   z6+1
   RTS
