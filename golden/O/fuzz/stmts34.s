SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
g3	EQU	$27
gp7	EQU	$28
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__w15	EQU	$85
__local_main__l14	EQU	$86
__local_f8__2	EQU	$8A
__local_f8__3	EQU	$8B
__local_f8__l10	EQU	$8C
__local_f8__4	EQU	$8D
__local_mix__0	EQU	$8E
__local_mix__1	EQU	$8F
__local_mix__2	EQU	$90
__local_mix__3	EQU	$91

; @zp-link-meta-begin
; def f8 params=__zpabi_f8__pp9_0,__zpabi_f8__pp9_1 locals=__local_f8__0,__local_f8__1,__local_f8__2,__local_f8__3,__local_f8__l10,__local_f8__4 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__w15,__local_main__l14,__local_main__2 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f8
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run2 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run4 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=1 at=auto def=true init=$FF
; zp g3 size=1 at=auto def=true init=$00 weight=17
; zp gp7 size=2 at=auto def=true init=<g2,>g2 weight=8
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
   JSR   fin0@run3
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run0
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run1
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JSR   fin0@run1
   LDA   g2
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JSR   fin0@run0@run2
   JMP   mix

fin0@run0@run2:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   JSR   fin1@run4
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
   JSR   fin0@run0@run2
   JMP   mix

fin1@run4:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   RTS

fin2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   JSR   fin0@run0
   LDA   gs6+4
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+3
   STA   __zpabi_mix__v_0
   LDA   gs6+4
   STA   __zpabi_mix__v_1
   JMP   mix

f8:
   SUBROUTINE

   LDA   #$02
   STA   __local_f8__l10
   LDA   #$00
   STA   __local_f8__4
   JSR   fin1@run4
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   BNE   .carry_inc@0
   DEC   __zpabi_mix__v_1
.carry_inc@0:
   DEC   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_0
   STA   ga5
   LDA   __zpabi_mix__v_1
   STA   ga5+1
   JSR   fin0@run0@run2
   JSR   mix
   LDA   gs6+1
   AND   #$03
   LDY   #$00
   STY   gs6+2
   ORA   #$00
   STA   gs6+1
   BNE   .if_end@0
   LDA   gs6+3
   STA   __local_f8__3
   LDA   gs6+4
   STA   __local_f8__2
   LSR
   LDA   __local_f8__3
   ROR
   STA   __zpabi_mix__v_1
   TYA
   ROR
   CLC
   ADC   __local_f8__3
   STA   gs6+3
   LDA   __zpabi_mix__v_1
   ADC   __local_f8__2
   STA   gs6+4
   STY   g2+1
.if_end@0:
   LDA   gs6+1
   EOR   #$FF
   STA   gs6+1
   STA   __zpabi_mix__v_0
   TYA
   JSR   fin0@run3
   ASL
   STA   __zpabi_mix__v_0
   LDA   g2+1
   ROL
   ASL   __zpabi_mix__v_0
   ROL
   ASL   __zpabi_mix__v_0
   ROL
   ORA   __zpabi_mix__v_0
   BEQ   .if_end@2
   LDA   __local_f8__l10
   STA   __local_f8__2
   LDX   __local_f8__4
   CLC
   ADC   #$01
   STA   __zpabi_mix__v_1
   TXA
   ADC   #$00
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   STA   __local_f8__l10
   LDA   __zpabi_mix__v_0
   STA   __local_f8__4
   LDA   __local_f8__2
   SEC
   SBC   #$80
   BCS   .carry_inc@1
   DEX
.carry_inc@1:
   RTS
.if_end@2:
   LDX   #$FF
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$80
   STA   __local_main__l14
   LDA   #$00
   STA   __local_main__l14+1
   STA   __local_main__w15
.loop@1_start:
   LDA   g3
   CLC
   ADC   #$FF
   STA   g3
   LDY   #$00
   STA   (gp7),Y
   LDA   #$00
   INY
   STA   (gp7),Y
   INC   __local_main__w15
   LDA   __local_main__w15
   CMP   #$02
   BCC   .loop@1_start
   LDX   #>__local_main__l14
   LDA   #<__local_main__l14
   JSR   f8
   LDA   gs6+3
   STA   ga5+4
   LDA   gs6+4
   STA   ga5+5
   LDX   #$05
.shift_loop@4:
   ASL   ga5+4
   ROL   ga5+5
   DEX
   BNE   .shift_loop@4
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
   DC.B  $FF

g2:
   DC.W  $0004

ga5:
   DC.W  $8000
   DS.B  6

gs6:
   DS.B  3
   DC.W  $0001

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g3
   LDA   #$FF
   STA   z4
   LDA   #<g2
   STA   gp7
   LDA   #>g2
   STA   gp7+1
   RTS
