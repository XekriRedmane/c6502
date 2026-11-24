SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
__zpabi_f7__pp9_0	EQU	$80
__zpabi_f7__pp9_1	EQU	$81
__zpabi_mix__v_0	EQU	$82
__zpabi_mix__v_1	EQU	$83
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_f7__p8	EQU	$86
__local_main__w15	EQU	$86
__local_main__0	EQU	$87
__local_main__1	EQU	$88
__local_mix__0	EQU	$89
__local_mix__1	EQU	$8A
__local_mix__2	EQU	$8B
__local_mix__3	EQU	$8C

; @zp-link-meta-begin
; def f7 params=__zpabi_f7__p8,__zpabi_f7__pp9_0,__zpabi_f7__pp9_1 locals=__local_f7__p8,__local_f7__0 indirect=false in_cycle=false param_regs=A,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__w15,__local_main__0,__local_main__1 indirect=false in_cycle=false
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
; item fin0@run1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f7 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.bss local=false zero=true
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item gp6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=1 at=auto def=true init=$01
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   fin0@run1
   JSR   mix
   LDA   z3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   JSR   fin0@run2
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga4+2
   STA   __zpabi_mix__v_0
   LDA   ga4+3
   JSR   fin1@run0
   LDA   ga4+4
   STA   __zpabi_mix__v_0
   LDA   ga4+5
   JSR   fin1@run0
   LDA   ga4+6
   STA   __zpabi_mix__v_0
   LDA   ga4+7
   JSR   fin1@run0
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   JSR   fin0@run1
   JMP   mix

fin1@run0:
   SUBROUTINE

   JSR   fin0@run2
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs5+3
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs5+2
   STA   __zpabi_mix__v_0
   LDA   gs5+3
   STA   __zpabi_mix__v_1
   JMP   mix

f7:
   SUBROUTINE

   STA   __local_f7__p8
   LDA   #$01
   STA   __zpabi_mix__v_0
   JSR   fin0@run1
   JSR   mix
   LDY   #$00
   LDA   (__zpabi_f7__pp9_0),Y
   CLC
   ADC   #$01
   STA   (__zpabi_f7__pp9_0),Y
   LDA   __local_f7__p8
   BEQ   .cond_else@0
   BPL   .vthread@0
   RTS
.cond_else@0:
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
.vthread@0:
   RTS

main:
   SUBROUTINE

   LDA   ga4+2
   STA   __local_main__0
   LDA   ga4+3
   STA   __local_main__w15
   LDA   __local_main__0
   SEC
   SBC   #$01
   STA   ga4+2
   LDA   __local_main__w15
   SBC   #$00
   STA   ga4+3
   LDA   __local_main__0
   STA   z3
   LDA   #$00
   STA   __local_main__w15
.loop@0_start:
   INC   __local_main__w15
   LDA   __local_main__w15
   CMP   #$03
   BCC   .loop@0_start
   LDA   ga4+6
   STA   __local_main__1
   LDA   ga4+7
   STA   __local_main__0
   LDA   gs5+1
   SBC   __local_main__1
   LDA   #$00
   SBC   __local_main__0
   LDA   #$00
   ROL
   LDX   gp6
   STX   DPTR
   LDX   gp6+1
   STX   DPTR+1
   LDY   #$00
   STA   (DPTR),Y
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
   DC.W  $FFFF

g2:
   DS.B  1

ga4:
   DC.W  $00FF
   DC.W  $0100
   DC.W  $0080
   DC.W  $FFFF

gs5:
   DS.B  1
   DC.B  $80
   DC.W  $00FF

gp6:
   DC.W  g2

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   z3
   RTS
