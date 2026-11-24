SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
g1	EQU	$28
__local_main__0	EQU	$80
__zpabi_f15__pp17_0	EQU	$80
__zpabi_f6__p8_0	EQU	$80
__local_main__l23	EQU	$81
__zpabi_f15__pp17_1	EQU	$81
__zpabi_f6__p8_1	EQU	$81
__zpabi_f6__p8_2	EQU	$82
__zpabi_mix__v_0	EQU	$82
__zpabi_f6__p8_3	EQU	$83
__zpabi_mix__v_1	EQU	$83
__zpabi_f6__pp9_0	EQU	$84
__zpabi_mix__v_2	EQU	$84
__zpabi_f6__pp9_1	EQU	$85
__zpabi_mix__v_3	EQU	$85
__local_f15__l18	EQU	$86
__local_f6__w14	EQU	$86
__local_f15__p16_1	EQU	$87
__local_f6__0	EQU	$87
__local_f15__p16_0	EQU	$88
__local_f6__1	EQU	$88
__local_f6__2	EQU	$89
__local_f6__l12	EQU	$8A
__local_mix__0	EQU	$8A
__local_f6__l11	EQU	$8B
__local_mix__1	EQU	$8B
__local_f6__3	EQU	$8C
__local_mix__2	EQU	$8C
__local_mix__3	EQU	$8D

; @zp-link-meta-begin
; def f15 params=__zpabi_f15__p16_0,__zpabi_f15__p16_1,__zpabi_f15__pp17_0,__zpabi_f15__pp17_1 locals=__local_f15__l18,__local_f15__p16_1,__local_f15__p16_0 indirect=false in_cycle=false param_regs=A,X,,
; def f6 params=__zpabi_f6__p7,__zpabi_f6__p8_0,__zpabi_f6__p8_1,__zpabi_f6__p8_2,__zpabi_f6__p8_3,__zpabi_f6__pp9_0,__zpabi_f6__pp9_1 locals=__local_f6__w14,__local_f6__0,__local_f6__1,__local_f6__2,__local_f6__l12,__local_f6__l11,__local_f6__3 indirect=false in_cycle=false param_regs=A,,,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__l23 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f15 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin1@tail0 section=.text local=true zero=false
; item fin1@tail0@run1 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f6 section=.text local=false zero=false
; item f15 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g3 section=.data local=false zero=false
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=2 at=auto def=true init=$7F,$00 weight=67
; zp g1 size=1 at=auto def=true init=$00 weight=6
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
   JSR   fin0@run0
   JSR   mix
   LDA   g2+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   g3+2
   STA   __zpabi_mix__v_2
   LDA   g3+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   ga4
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run0
   LDA   ga4
   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga4+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run0
   LDA   ga4+1
   JSR   fin1@run2
   LDA   ga4+2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run0
   LDA   ga4+2
   JSR   fin1@run2
   LDA   ga4+3
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run0
   LDA   ga4+3
   JSR   fin1@run2
   LDA   gs5+1
   JMP   fin1@tail0

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin1@tail0:
   SUBROUTINE

   JSR   fin1@tail0@run1
   JMP   mix

fin1@tail0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin2:
   SUBROUTINE

   LDA   gs5+2
   JMP   fin1@tail0

f6:
   SUBROUTINE

   LDA   #$03
   STA   __local_f6__l11
   LDA   #$00
   STA   __local_f6__3
   STA   __local_f6__l12
   STA   __local_f6__w14
   JMP   .loop@0_continue
.f6@rot@1:
   LDA   gs5+2
   BEQ   .if_end@0
   LDA   __local_f6__2
   STA   __local_f6__w14
   JMP   .loop@0_continue
.if_end@0:
   LDA   __local_f6__l11
   STA   gs5+1
   LDA   __local_f6__3
   LDA   #$00
   STA   __local_f6__w14
   JMP   .loop@1_continue
.f6@rot@0:
   LDA   __local_f6__l12
   LDY   #$00
   AND   (__zpabi_f6__pp9_0),Y
   AND   #$07
   CMP   #$03
   LDA   g2
   ORA   g2+1
   BNE   .loop@1_break
   LDA   __local_f6__1
   STA   __local_f6__w14
.loop@1_continue:
   CLC
   ADC   #$01
   STA   __local_f6__1
   LDA   __local_f6__w14
   CMP   #$04
   BCC   .f6@rot@0
.loop@1_break:
   LDA   gs5+1
   BEQ   .and_false@2
   LDA   __local_f6__2
   BNE   .vthread@0
.and_false@2:
   LDA   __local_f6__2
.vthread@0:
   STA   __local_f6__w14
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_f6__2
   LDA   __local_f6__w14
   BEQ   .f6@rot@1
   LDA   ga4+1
   STA   __local_f6__0
   LSR
   LDA   #$00
   ROR
   CLC
   ADC   __local_f6__0
   STA   ga4+1
   ORA   g1
   STA   g1
   LDA   #$7F
   STA   gs5+1
   LDA   g1
   RTS

f15:
   SUBROUTINE

   STA   __local_f15__p16_0
   STX   __local_f15__p16_1
   LDA   #$03
   STA   __local_f15__l18
   STA   g2
   LDA   #$00
   STA   g2+1
   LDA   __local_f15__l18
   SEC
   SBC   #$01
   STA   __local_f15__l18
   SEC
   LDA   #$00
   SBC   __local_f15__p16_0
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   __local_f15__p16_1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g1
   LDA   #$FF
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__l23
   LDA   __local_main__l23
   CLC
   ADC   #$FF
   STA   __local_main__0
   STA   __local_main__l23
   LDA   #$81
   AND   __local_main__0
   JSR   fin1@tail0@run1
   JSR   mix
   LDA   g3
   EOR   #$01
   STA   g3
   LDA   g1
   LDA   gs5+1
   SEC
   SBC   #$05
   STA   __local_main__0
   STA   gs5+1
   LDA   #$01
   ORA   __local_main__0
   STA   g2
   LDA   #$00
   STA   g2+1
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

g3:
   DC.L  $00000001

ga4:
   DC.B  $00, $02, $FF, $01

gs5:
   DS.B  1
   DC.B  $7F
   DS.B  1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g2+1
   LDA   #$7F
   STA   g2
   RTS
