SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
g3	EQU	$28
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_f9__1	EQU	$85
__local_main__w16	EQU	$85
__local_main__l13	EQU	$86
__local_main__1	EQU	$87
__local_main__2	EQU	$88
__local_mix__0	EQU	$90
__local_mix__1	EQU	$91
__local_mix__2	EQU	$92
__local_mix__3	EQU	$93

; @zp-link-meta-begin
; def f9 params=__zpabi_f9__p10_0,__zpabi_f9__p10_1 locals=__local_f9__0,__local_f9__1 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__w16,__local_main__l13,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run3 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@tail0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run2 section=.text local=true zero=false
; item f9 section=.text local=false zero=false
; item f9@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$80,$00
; zp g3 size=1 at=auto def=true init=$FF weight=7
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
   JSR   fin0@run4
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g3
   JSR   fin0@run0
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   JSR   fin0@run4
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run3
   JMP   mix

fin0@run0@run3:
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
   JMP   fin1@tail0

fin1@tail0:
   SUBROUTINE

   JSR   fin0@run0@run3
   JMP   mix

fin2:
   SUBROUTINE

   JSR   fin2@run2
   JSR   mix
   LDA   gs6+9
   JMP   fin1@tail0

fin2@run2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   LDA   gs6+3
   STA   __zpabi_mix__v_2
   LDA   gs6+4
   STA   __zpabi_mix__v_3
   RTS

f9:
   SUBROUTINE

   LDA   ga5+1
   BEQ   .lnot_true@0
   LDA   #$00
   BEQ   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   JSR   f9@run1
   STA   __zpabi_mix__v_0
   SEC
   LDA   #$00
   SBC   __zpabi_mix__v_0
   STA   __local_f9__1
   LDA   #$00
   SBC   #$00
   STA   __zpabi_mix__v_0
   LDA   #$02
   CLC
   ADC   __local_f9__1
   STA   __local_f9__1
   LDA   #$00
   ADC   __zpabi_mix__v_0
   ORA   __local_f9__1
   BEQ   .if_end@2
   ASL   z4
   ROL   z4+1
   LDA   z4
   LDX   z4+1
   RTS
.if_end@2:
   LDA   z4
   EOR   #$01
   LDX   z4+1
   RTS

f9@run1:
   SUBROUTINE

   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

main:
   SUBROUTINE

   LDA   ga5
   AND   #$03
   STA   g2
   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   STA   g2+4
   STA   g2+5
   STA   g2+6
   STA   g2+7
   JSR   f9@run1
   ASL
   ASL
   ASL
   ASL
   STA   __local_main__w16
   LDY   #$00
   STA   (DPTR),Y
   LDA   g2
   AND   __local_main__w16
   AND   #$07
   BEQ   .switch@4_break
   CMP   #$04
   BEQ   .switch@4_break
   JSR   f9@run1
   CMP   ga5+1
   BNE   .cmp_true@4
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   AND   #$07
   CMP   #$05
   BNE   .switch@8_break
   LDA   gs6+9
   STA   HARGS
   LDA   g3
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   gs6+9
   LDA   gs6+1
   LDX   gs6+2
   STX   __zpabi_mix__v_0
   ORA   #$81
   STA   gs6+1
   STA   g1
   LDA   __zpabi_mix__v_0
   STA   g1+1
.switch@8_break:
.switch@4_break:
   LDA   gs6+1
   STA   __zpabi_mix__v_1
   LDA   gs6+2
   STA   __zpabi_mix__v_2
   LDA   gs6+3
   STA   __zpabi_mix__v_3
   LDA   gs6+4
   STA   __local_main__2
   LDA   gs6+5
   STA   __local_main__1
   LDA   gs6+6
   STA   __local_main__l13
   LDA   gs6+7
   STA   __local_main__w16
   LDA   gs6+8
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   ASL
   LDA   __zpabi_mix__v_2
   ROL
   STA   gs6+1
   LDA   __zpabi_mix__v_3
   ROL
   STA   gs6+2
   LDA   __local_main__2
   ROL
   STA   gs6+3
   LDA   __local_main__1
   ROL
   STA   gs6+4
   LDA   __local_main__l13
   ROL
   STA   gs6+5
   LDA   __local_main__w16
   ROL
   STA   gs6+6
   LDA   __zpabi_mix__v_0
   ROL
   STA   gs6+7
   LDA   #$00
   ROL
   STA   gs6+8
   LDA   #$80
   STA   __local_main__l13
   LDA   #$00
   STA   __local_main__w16
   JMP   .loop@14_continue
.main@rot@0:
   LSR   __local_main__l13
   LDA   __local_main__l13
   STA   g2
   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   STA   g2+4
   STA   g2+5
   STA   g2+6
   STA   g2+7
   LDA   __zpabi_mix__v_0
   STA   __local_main__w16
.loop@14_continue:
   CLC
   ADC   #$01
   STA   __zpabi_mix__v_0
   LDA   __local_main__w16
   CMP   #$04
   BCC   .main@rot@0
   JSR   fin2@run2
   LDA   gs6+5
   STA   __local_main__2
   LDA   gs6+6
   STA   __local_main__1
   LDA   gs6+7
   STA   __local_main__l13
   LDA   gs6+8
   STA   __local_main__w16
   LDA   __zpabi_mix__v_0
   CLC
   ADC   #$01
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   ADC   #$00
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_2
   ADC   #$00
   STA   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_3
   ADC   #$00
   STA   __zpabi_mix__v_3
   LDA   __local_main__2
   ADC   #$00
   STA   gs6+5
   LDA   __local_main__1
   ADC   #$00
   STA   gs6+6
   LDA   __local_main__l13
   ADC   #$00
   STA   gs6+7
   LDA   __local_main__w16
   ADC   #$00
   STA   gs6+8
   LDA   __zpabi_mix__v_0
   STA   gs6+1
   LDA   __zpabi_mix__v_1
   STA   gs6+2
   LDA   __zpabi_mix__v_2
   STA   gs6+3
   LDA   __zpabi_mix__v_3
   STA   gs6+4
   JSR   mix
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
   DC.L  $0000000A
   DC.L  $00000000

ga5:
   DC.B  $FF, $FF, $80, $00

gs6:
   DS.B  1
   DC.L  $00000002
   DC.L  $00000000
   DC.B  $FF

gp7:
   DC.W  g3

gp8:
   DC.W  g2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   LDA   #$FF
   STA   g3
   LDA   #$80
   STA   z4
   RTS
