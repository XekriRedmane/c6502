SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
g1	EQU	$28
g2	EQU	$29
__zpabi_f6__p8_0	EQU	$80
__zpabi_f6__p8_1	EQU	$81
__zpabi_mix__v_0	EQU	$82
__zpabi_mix__v_1	EQU	$83
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_main__0	EQU	$86
__local_main__1	EQU	$87
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_main__w14	EQU	$8B
__local_f6__1	EQU	$8D
__local_f6__2	EQU	$8E
__local_mix__0	EQU	$90
__local_mix__1	EQU	$91
__local_mix__2	EQU	$92
__local_mix__3	EQU	$93

; @zp-link-meta-begin
; def f6 params=__zpabi_f6__p7,__zpabi_f6__p8_0,__zpabi_f6__p8_1 locals=__local_f6__0,__local_f6__1,__local_f6__2 indirect=false in_cycle=false param_regs=A,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__w14 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f6 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f6
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run2 section=.text local=true zero=false
; item f6 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga4 section=.data local=false zero=false
; item gs5 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$FF,$FF
; zp g1 size=1 at=auto def=true init=$FF weight=12
; zp g2 size=2 at=auto def=true init=$7F,$00 weight=11
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run1
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   JSR   fin0@run1
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   STA   __zpabi_mix__v_1
   LDA   ga4+2
   STA   __zpabi_mix__v_2
   LDA   ga4+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga4+4
   STA   __zpabi_mix__v_0
   LDA   ga4+5
   STA   __zpabi_mix__v_1
   LDA   ga4+6
   STA   __zpabi_mix__v_2
   LDA   ga4+7
   JSR   fin0@run0
   LDA   ga4+8
   STA   __zpabi_mix__v_0
   LDA   ga4+9
   STA   __zpabi_mix__v_1
   LDA   ga4+10
   STA   __zpabi_mix__v_2
   LDA   ga4+11
   JSR   fin0@run0
   LDA   ga4+12
   STA   __zpabi_mix__v_0
   LDA   ga4+13
   STA   __zpabi_mix__v_1
   LDA   ga4+14
   STA   __zpabi_mix__v_2
   LDA   ga4+15
   JSR   fin0@run0
   LDA   gs5+2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   STA   __zpabi_mix__v_1
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs5+3
   JSR   fin2@run2
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin2@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   RTS

f6:
   SUBROUTINE

   LDA   g1
   STA   __local_f6__2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f6__1
   LDA   gs5+3
   SEC
   SBC   __local_f6__2
   LDA   #$00
   SBC   __local_f6__1
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   EOR   gs5+1
   STA   ga4+4
   LDA   #$00
   EOR   gs5+2
   STA   ga4+5
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   ga4+6
   STA   ga4+7
   LDA   gs5+3
   SEC
   SBC   #$02
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
   RTS

main:
   SUBROUTINE

   LDA   ga4
   LDX   ga4+1
   STX   __local_main__3
   LDX   ga4+2
   STX   __local_main__2
   LDX   ga4+3
   STX   __local_main__1
   SEC
   SBC   #$01
   STA   __local_main__0
   LDA   __local_main__3
   SBC   #$00
   STA   __local_main__3
   LDA   __local_main__2
   SBC   #$00
   STA   ga4+2
   LDA   __local_main__1
   SBC   #$00
   STA   ga4+3
   LDA   __local_main__0
   STA   ga4
   LDA   __local_main__3
   STA   ga4+1
   LDA   __local_main__0
   EOR   g2
   STA   gs5+1
   LDA   __local_main__3
   EOR   g2+1
   STA   gs5+2
   LDA   #$00
   STA   __local_main__w14
.loop@0_start:
   CLC
   SBC   g2
   LDA   #$00
   SBC   g2+1
   LDA   #$00
   ROL
   STA   __local_main__0
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__0
   JSR   fin2@run2
   JSR   mix
   LDA   __local_main__w14
   AND   #$03
   ASL
   ASL
   TAX
   LDA   ga4,X
   AND   #$5A
   STA   ga4,X
   LDA   #$00
   STA   ga4+1,X
   STA   ga4+2,X
   STA   ga4+3,X
   LDA   gs5+3
   AND   z3
   LDX   g1
   STX   __zpabi_f6__p8_0
   LDX   #$00
   STX   __zpabi_f6__p8_1
   JSR   f6
   JSR   fin2@run2
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   INC   __local_main__w14
   LDA   __local_main__w14
   BEQ   .loop@0_start
   LDA   gs5+3
   CLC
   ADC   #$05
   STA   gs5+3
   LDA   g1
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

ga4:
   DC.L  $00008000
   DC.L  $00000003
   DC.L  $000000FF
   DC.L  $80000000

gs5:
   DS.B  4

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   LDA   #$FF
   STA   g1
   STA   z3
   STA   z3+1
   LDA   #$7F
   STA   g2
   RTS
