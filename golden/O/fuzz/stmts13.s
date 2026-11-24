SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
g1	EQU	$28
__local_main__0	EQU	$80
__zpabi_f6__p7_0	EQU	$80
__zpabi_f6__p7_1	EQU	$81
__zpabi_f6__p7_2	EQU	$82
__zpabi_f6__p7_3	EQU	$83
__zpabi_f6__p7_4	EQU	$84
__zpabi_f6__p7_5	EQU	$85
__zpabi_f6__p8_0	EQU	$86
__zpabi_f6__p8_1	EQU	$87
__zpabi_f6__p8_2	EQU	$88
__zpabi_f6__p8_3	EQU	$89
__zpabi_f6__p9_0	EQU	$8A
__zpabi_f6__p9_1	EQU	$8B
__zpabi_f6__p9_2	EQU	$8C
__zpabi_f6__p9_3	EQU	$8D
__zpabi_f6__p9_4	EQU	$8E
__zpabi_f6__p9_5	EQU	$8F
__zpabi_f6__p9_6	EQU	$90
__zpabi_f6__p9_7	EQU	$91
__zpabi_f6__pp10_0	EQU	$92
__zpabi_f6__pp10_1	EQU	$93
__zpabi_mix__v_0	EQU	$94
__zpabi_mix__v_1	EQU	$95
__zpabi_mix__v_2	EQU	$96
__zpabi_mix__v_3	EQU	$97
__local_f6__0	EQU	$98
__local_mix__0	EQU	$9C
__local_mix__1	EQU	$9D
__local_mix__2	EQU	$9E
__local_mix__3	EQU	$9F

; @zp-link-meta-begin
; def f6 params=__zpabi_f6__p7_0,__zpabi_f6__p7_1,__zpabi_f6__p7_2,__zpabi_f6__p7_3,__zpabi_f6__p7_4,__zpabi_f6__p7_5,__zpabi_f6__p8_0,__zpabi_f6__p8_1,__zpabi_f6__p8_2,__zpabi_f6__p8_3,__zpabi_f6__p9_0,__zpabi_f6__p9_1,__zpabi_f6__p9_2,__zpabi_f6__p9_3,__zpabi_f6__p9_4,__zpabi_f6__p9_5,__zpabi_f6__p9_6,__zpabi_f6__p9_7,__zpabi_f6__pp10_0,__zpabi_f6__pp10_1 locals=__local_f6__0,__local_f6__1,__local_f6__2,__local_f6__3 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__i16 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f6 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run3 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f6 section=.text local=false zero=false
; item f6@run2 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$FF,$FF
; zp g1 size=1 at=auto def=true init=$00 weight=5
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run1
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   JSR   fin0@run0
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   JMP   fin0@tail0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g1
   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run3
   JMP   mix

fin0@run0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@tail0:
   SUBROUTINE

   JSR   fin0@run0@run3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga4+2
   STA   __zpabi_mix__v_0
   LDA   ga4+3
   JSR   fin0@run0
   LDA   ga4+4
   STA   __zpabi_mix__v_0
   LDA   ga4+5
   JSR   fin0@run0
   LDA   ga4+6
   STA   __zpabi_mix__v_0
   LDA   ga4+7
   JSR   fin0@run0
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   gs5+3
   STA   __zpabi_mix__v_0
   LDA   gs5+4
   JMP   fin0@tail0

f6:
   SUBROUTINE

   LDA   g1
   ASL
   CLC
   ADC   g1
   STA   g1
   BEQ   .if_else@1
   LDA   #$00
   STA   gs5+3
   LDA   #$80
   STA   gs5+4
.if_else@1:
   LDA   g1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run1
   LDA   #$FF
   EOR   #$02
   STA   g2
   LDA   __zpabi_f6__p8_1
   STA   g2+1
   LDA   __zpabi_f6__p8_2
   STA   g2+2
   LDA   __zpabi_f6__p8_3
   STA   g2+3
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   f6@run2
   LDA   gs5+3
   BNE   .carry_inc@0
   DEC   gs5+4
.carry_inc@0:
   DEC   gs5+3
   LDA   __zpabi_f6__p7_0
   SEC
   SBC   ga4+6
   STA   __local_f6__0
   LDA   __zpabi_f6__p7_1
   SBC   ga4+7
   AND   #$0F
   TAX
   LDA   __local_f6__0
   RTS

f6@run2:
   SUBROUTINE

   STA   g2+4
   STA   g2+5
   STA   g2+6
   STA   g2+7
   RTS

main:
   SUBROUTINE

   LDA   gs5+1
   LDX   gs5+2
   STX   __local_main__0
   SEC
   SBC   g2
   LDA   __local_main__0
   SBC   g2+1
   LDA   #$00
   SBC   g2+2
   LDA   #$00
   SBC   g2+3
   LDA   #$00
   SBC   g2+4
   LDA   #$00
   SBC   g2+5
   LDA   #$00
   SBC   g2+6
   LDA   #$00
   SBC   g2+7
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   AND   #$07
   CMP   #$01
   LDA   gs5+3
   LDX   gs5+4
   STX   __local_main__0
   CLC
   SBC   #$01
   LDA   __local_main__0
   SBC   #$00
   LDA   #$00
   ROL
   STA   g2
   LDA   #$00
   STA   g2+1
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   g2+2
   STA   g2+3
   JSR   f6@run2
   LDA   ga4
   LDX   ga4+1
   STX   __local_main__0
   SEC
   SBC   #$94
   LDA   __local_main__0
   SBC   #$5B
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   STA   gs5+1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   gs5+2
   LDX   #$04
.main@rot@0:
   LDA   ga4
   LSR   ga4+1
   ROR
   LSR   ga4+1
   ROR
   LSR   ga4+1
   ROR
   LSR   ga4+1
   ROR
   STA   ga4
   DEX
   BNE   .main@rot@0
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
   DC.L  $0000FFFF
   DC.L  $00000000

ga4:
   DC.W  $007F
   DC.W  $0FFF
   DS.B  2
   DC.W  $0FFF

gs5:
   DS.B  1
   DC.W  $000A
   DC.W  $8000

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   LDA   #$FF
   STA   z3
   STA   z3+1
   RTS
