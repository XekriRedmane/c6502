SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
@5.s15	EQU	$28
g2	EQU	$29
g3	EQU	$2A
g1	EQU	$2C
__local_main__w21	EQU	$80
__zpabi_f7__p8_0	EQU	$80
__local_main__l20	EQU	$81
__zpabi_f7__p8_1	EQU	$81
__local_main__0	EQU	$82
__zpabi_f7__p8_2	EQU	$82
__local_main__1	EQU	$83
__zpabi_f7__p8_3	EQU	$83
__local_main__2	EQU	$84
__zpabi_f7__p9_0	EQU	$84
__zpabi_f7__p9_1	EQU	$85
__zpabi_f7__p10_0	EQU	$86
__zpabi_f7__p10_1	EQU	$87
__zpabi_f7__p10_2	EQU	$88
__zpabi_f7__p10_3	EQU	$89
__zpabi_f7__pp11_0	EQU	$8A
__zpabi_f7__pp11_1	EQU	$8B
__zpabi_mix__v_0	EQU	$8C
__zpabi_mix__v_1	EQU	$8D
__zpabi_mix__v_2	EQU	$8E
__zpabi_mix__v_3	EQU	$8F
__local_f7__w18	EQU	$90
__local_f7__i16	EQU	$94
__local_mix__0	EQU	$96
__local_mix__1	EQU	$97
__local_mix__2	EQU	$98
__local_mix__3	EQU	$99

; @zp-link-meta-begin
; def f7 params=__zpabi_f7__p8_0,__zpabi_f7__p8_1,__zpabi_f7__p8_2,__zpabi_f7__p8_3,__zpabi_f7__p9_0,__zpabi_f7__p9_1,__zpabi_f7__p10_0,__zpabi_f7__p10_1,__zpabi_f7__p10_2,__zpabi_f7__p10_3,__zpabi_f7__pp11_0,__zpabi_f7__pp11_1 locals=__local_f7__w18,__local_f7__0,__local_f7__1,__local_f7__2,__local_f7__i16,__local_f7__l13 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__w21,__local_main__l20,__local_main__0,__local_main__1,__local_main__2 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f7 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$00,$01
; zp @5.s15 size=1 at=auto def=true init=$B0 weight=64 local=true
; zp g2 size=1 at=auto def=true init=$80 weight=11
; zp g3 size=2 at=auto def=true init=$00,$01 weight=10
; zp g1 size=4 at=auto def=true init=$0C,$00,$00,$00 weight=18
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
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   JSR   fin0@run2
   LDA   g2
   JSR   fin0@run0
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   JSR   fin0@run2
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

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run1
   JMP   mix

fin0@run0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga5
   JSR   fin0@run0
   LDA   ga5+1
   JSR   fin0@run0
   LDA   ga5+2
   JSR   fin0@run0
   LDA   ga5+3
   JSR   fin0@run0@run1
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+1
   JSR   fin0@run0
   LDA   gs6+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+2
   STA   __zpabi_mix__v_0
   JMP   mix

f7:
   SUBROUTINE

   INC   z4
   BNE   .inc_done@0
   INC   z4+1
.inc_done@0:
   LDY   #$00
   LDA   (__zpabi_f7__pp11_0),Y
   ORA   #$03
   STA   (__zpabi_f7__pp11_0),Y
   LDA   #$04
   STA   __local_f7__i16
.f7@rot@0:
   LDA   gs6+2
   STA   __local_f7__w18
   CLC
   ADC   #$01
   STA   gs6+2
   LDA   __local_f7__w18
   STA   @5.s15
   LDA   z4
   ORA   z4+1
   BEQ   .cond_else@0
   LDA   ga5+1
   STA   __zpabi_mix__v_0
   LDA   #$00
   BEQ   .cond_end@1
.cond_else@0:
   LDA   #$00
   AND   gs6+1
   STA   __zpabi_mix__v_0
   LDA   #$00
.cond_end@1:
   LDY   #$00
   STY   __zpabi_mix__v_1
   STY   __zpabi_mix__v_2
   JSR   fin0@run2
   LDA   #$00
   STA   __local_f7__w18
.loop@2_start:
   LDA   #$01
   STA   gs6+1
   INC   __local_f7__w18
   LDA   __local_f7__w18
   CMP   #$03
   BCC   .loop@2_start
   DEC   __local_f7__i16
   BNE   .f7@rot@0
   LDY   #$00
   LDA   (__zpabi_f7__pp11_0),Y
   SBC   #$01
   STA   (__zpabi_f7__pp11_0),Y
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   g3
   STA   z4
   LDA   g3+1
   STA   z4+1
   LDA   #$A7
   STA   __local_main__l20
   CLC
   ADC   g1
   STA   __local_main__2
   LDA   g1+1
   ADC   #$00
   STA   __local_main__1
   LDA   g1+2
   ADC   #$00
   STA   __local_main__0
   LDA   g1+3
   ADC   #$00
   STA   __local_main__w21
   LDA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   ORA   __local_main__w21
   BEQ   .if_end@2
   LDA   #$00
   STA   __local_main__w21
.loop@3_start:
   LSR   __local_main__l20
   INC   __local_main__w21
   LDA   __local_main__w21
   CMP   #$04
   BCC   .loop@3_start
   LDA   z4
   ORA   #$01
   STA   g2
.if_end@2:
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

ga5:
   DC.B  $00, $FF, $80, $00

gs6:
   DS.B  1
   DC.B  $7F
   DC.B  $01

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g3
   STA   z4
   LDA   #$0C
   STA   g1
   LDA   #$80
   STA   g2
   LDA   #$01
   STA   g3+1
   STA   z4+1
   LDA   #$B0
   STA   @5.s15
   RTS
