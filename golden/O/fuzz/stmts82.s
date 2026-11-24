SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_f8__p10_0	EQU	$80
__zpabi_f8__p10_1	EQU	$81
__zpabi_f8__p10_2	EQU	$82
__zpabi_f8__p10_3	EQU	$83
__zpabi_mix__v_0	EQU	$84
__zpabi_mix__v_1	EQU	$85
__zpabi_mix__v_2	EQU	$86
__zpabi_mix__v_3	EQU	$87
__local_main__0	EQU	$88
__local_main__1	EQU	$89
__local_main__2	EQU	$8A
__local_main__3	EQU	$8B
__local_main__4	EQU	$8C
__local_main__l16	EQU	$8D
__local_main__5	EQU	$8E
__local_f8__l12	EQU	$8F
__local_f8__0	EQU	$90
__local_f8__3	EQU	$93
__local_f8__4	EQU	$94
__local_f8__5	EQU	$95
__local_f8__6	EQU	$96
__local_mix__0	EQU	$97
__local_mix__1	EQU	$98
__local_mix__2	EQU	$99
__local_mix__3	EQU	$9A

; @zp-link-meta-begin
; def f8 params=__zpabi_f8__p9,__zpabi_f8__p10_0,__zpabi_f8__p10_1,__zpabi_f8__p10_2,__zpabi_f8__p10_3 locals=__local_f8__l12,__local_f8__0,__local_f8__1,__local_f8__2,__local_f8__3,__local_f8__4,__local_f8__5,__local_f8__6 indirect=false in_cycle=false param_regs=A,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__l16,__local_main__5 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f8
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run3 section=.text local=true zero=false
; item fin2@run2 section=.text local=true zero=false
; item f8 section=.text local=false zero=false
; item f8@run4 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item g4 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
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
   STA   __zpabi_mix__v_0
   LDA   g1+1
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run0
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   JSR   fin0@run0
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   g4+2
   STA   __zpabi_mix__v_2
   LDA   g4+3
   JMP   fin0@tail0

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

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   JSR   fin0@run0
   LDA   ga5+2
   STA   __zpabi_mix__v_0
   LDA   ga5+3
   JSR   fin0@run0
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   JSR   fin0@run0
   LDA   ga5+6
   STA   __zpabi_mix__v_0
   LDA   ga5+7
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   gs6+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin2@run2
   JSR   fin2@run3
   STA   __zpabi_mix__v_2
   LDA   gs6+5
   JMP   fin0@tail0

fin2@run3:
   SUBROUTINE

   LDA   gs6+2
   STA   __zpabi_mix__v_0
   LDA   gs6+3
   STA   __zpabi_mix__v_1
   LDA   gs6+4
   RTS

fin2@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+1
   STA   __zpabi_mix__v_0
   JMP   mix

f8:
   SUBROUTINE

   LDA   #$00
   STA   __local_f8__l12
   STA   __local_f8__0
   LDA   __zpabi_f8__p10_0
   STA   HARGS
   LDA   __zpabi_f8__p10_1
   STA   HARGS+1
   LDA   __zpabi_f8__p10_2
   STA   HARGS+2
   LDA   __zpabi_f8__p10_3
   STA   HARGS+3
   LDA   #$05
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   SEC
   STA   __zpabi_mix__v_0
   LDA   HARGS+6
   STA   __zpabi_mix__v_1
   LDA   HARGS+7
   STA   __zpabi_mix__v_2
   LDA   HARGS+8
   SBC   #$00
   JSR   f8@run4
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   gs6+1
   LDA   g4
   CLC
   ADC   #$FF
   STA   g4
   BCC   .carry_inc@0
   INC   g4+1
   BNE   .carry_inc@0
   INC   g4+2
   BNE   .carry_inc@0
   INC   g4+3
.carry_inc@0:
   JSR   fin2@run3
   STA   __local_f8__6
   LDA   gs6+5
   STA   __local_f8__5
   LSR
   LDA   __local_f8__6
   ROR
   STA   __local_f8__4
   LDA   __zpabi_mix__v_1
   ROR
   STA   __local_f8__3
   LDA   __zpabi_mix__v_0
   ROR
   STA   __local_f8__0
   LDA   #$00
   ROR
   CLC
   ADC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   __local_f8__0
   ADC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   LDA   __local_f8__3
   ADC   __local_f8__6
   STA   __local_f8__0
   LDA   __local_f8__4
   ADC   __local_f8__5
   LDX   __zpabi_mix__v_0
   STX   gs6+2
   LDX   __zpabi_mix__v_1
   STX   gs6+3
   LDX   __local_f8__0
   STX   gs6+4
   STX   __zpabi_mix__v_2
   STA   gs6+5
   AND   #$03
   JSR   f8@run4
   STA   __local_f8__l12
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f8__0
   LDA   g3
   ORA   __local_f8__l12
   STA   __local_f8__l12
   LDA   g3+1
   ORA   __local_f8__0
   TAX
   LDA   __local_f8__l12
   RTS

f8@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   gs6+1
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__l16
   LDA   #$00
   STA   __local_main__5
   LDA   g2
   LDX   g2+1
   AND   #$03
   STA   g2
   LDA   #$00
   STA   g2+1
   LDA   g2
   STA   __local_main__2
   LDA   g2+1
   LDA   g1
   SEC
   SBC   g4
   STA   __zpabi_f8__p10_0
   LDA   g1+1
   SBC   g4+1
   STA   __zpabi_f8__p10_1
   LDA   #$00
   SBC   g4+2
   STA   __zpabi_f8__p10_2
   LDA   #$00
   SBC   g4+3
   STA   __zpabi_f8__p10_3
   LDA   __local_main__2
   JSR   f8
   STA   __local_main__4
   JSR   main@run1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_main__3
   INY
   LDA   (DPTR),Y
   STA   __local_main__2
   INY
   LDA   (DPTR),Y
   STA   __local_main__1
   INY
   LDA   (DPTR),Y
   STA   __local_main__0
   LDA   __local_main__3
   EOR   #$03
   LDY   #$00
   STA   (DPTR),Y
   JSR   main@run1
   LDA   __local_main__2
   LDY   #$01
   STA   (DPTR),Y
   JSR   main@run1
   LDA   __local_main__1
   LDY   #$02
   STA   (DPTR),Y
   JSR   main@run1
   LDA   __local_main__0
   LDY   #$03
   STA   (DPTR),Y
   LDA   __local_main__4
   SEC
   SBC   #$01
   STA   g3
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   g3+1
   LDA   __local_main__l16
   LDX   __local_main__5
   STX   __local_main__0
   ORA   #$5A
   STA   __local_main__l16
   LDA   __local_main__0
   STA   __local_main__5
   LDA   gs6+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin2@run2
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run1:
   SUBROUTINE

   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   RTS

acc:
   DS.B  2

g1:
   DC.W  $FFFF

g2:
   DC.W  $0080

g3:
   DC.W  $8000

g4:
   DC.L  $00000001

ga5:
   DS.B  2
   DC.W  $FFFF
   DC.W  $7FFF
   DC.W  $0002

gs6:
   DS.B  2
   DC.L  $0000FFFF

gp7:
   DC.W  g4
