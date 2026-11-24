SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$27
gp7	EQU	$29
g1	EQU	$2B
__zpabi_f8__p10	EQU	$80
__zpabi_mix__v_0	EQU	$81
__zpabi_mix__v_1	EQU	$82
__zpabi_mix__v_2	EQU	$83
__zpabi_mix__v_3	EQU	$84
__local_main__0	EQU	$85
__local_main__1	EQU	$86
__local_main__2	EQU	$87
__local_main__3	EQU	$88
__local_f8__i12	EQU	$8A
__local_f8__0	EQU	$8B
__local_f8__p9_1	EQU	$8C
__local_f8__p9_0	EQU	$8D
__local_mix__0	EQU	$8F
__local_mix__1	EQU	$90
__local_mix__2	EQU	$91
__local_mix__3	EQU	$92

; @zp-link-meta-begin
; def f8 params=__zpabi_f8__p9_0,__zpabi_f8__p9_1,__zpabi_f8__p10 locals=__local_f8__w13,__local_f8__i12,__local_f8__0,__local_f8__p9_1,__local_f8__p9_0,__local_f8__1 indirect=false in_cycle=false param_regs=A,X,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
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
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run2 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp z3 size=1 at=auto def=true init=$01
; zp z4 size=2 at=auto def=true init=$FF,$FF
; zp gp7 size=2 at=auto def=true init=<g2,>g2 weight=133
; zp g1 size=1 at=auto def=true init=$01 weight=3
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
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run1
   LDA   z3
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

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run2
   JMP   mix

fin0@run0@run2:
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
   JSR   fin0@run0@run2
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

f8:
   SUBROUTINE

   STA   __local_f8__p9_0
   STX   __local_f8__p9_1
   LDA   z3
   STA   __local_f8__0
   INC   z3
   LDA   ga5+2
   CLC
   ADC   #$FF
   STA   __local_f8__i12
   LDA   #$00
   ADC   #$00
   STA   __zpabi_mix__v_1
   LDA   __local_f8__0
   ORA   __local_f8__i12
   STA   z4
   LDA   __zpabi_mix__v_1
   STA   z4+1
   LSR
   LDA   z4
   ROR
   STA   __local_f8__i12
   LDA   #$00
   ROR
   ORA   __local_f8__i12
   BEQ   .if_end@0
   LDA   #$02
   STA   __local_f8__i12
.f8@rot@0:
   LDA   gs6+1
   ORA   #$81
   STA   gs6+1
   DEC   __local_f8__i12
   BNE   .f8@rot@0
.if_end@0:
   LDA   g2+1
   LSR
   LDA   g2
   ROR
   STA   __local_f8__i12
   LDA   #$00
   ROR
   SEC
   SBC   g2
   STA   __zpabi_mix__v_0
   LDA   __local_f8__i12
   SBC   g2+1
   JSR   fin0@run1
   LDA   #$00
   STA   __zpabi_mix__v_1
.loop@1_continue:
   CLC
   ADC   #$01
   STA   __local_f8__0
   LDA   __zpabi_mix__v_1
   CMP   #$04
   BCS   .loop@1_break
   LDA   gs6+1
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   gs6+1
   STA   z3
   LDA   __local_f8__0
   CLC
   ADC   __local_f8__0
   STA   __local_f8__i12
   LDA   #$00
   ADC   #$00
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f8__i12
   STA   __zpabi_mix__v_0
   JSR   mix
   LDY   #$00
   LDA   (gp7),Y
   STA   __zpabi_mix__v_1
   INY
   LDA   (gp7),Y
   STA   __local_f8__i12
   LDA   __zpabi_mix__v_1
   BNE   .carry_inc@0
   DEC   __local_f8__i12
.carry_inc@0:
   DEC   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_1
   LDY   #$00
   STA   (gp7),Y
   LDA   __local_f8__i12
   INY
   STA   (gp7),Y
   LDA   __local_f8__0
   STA   __zpabi_mix__v_1
   JMP   .loop@1_continue
.loop@1_break:
   LDA   __local_f8__p9_0
   EOR   #$FF
   STA   __zpabi_mix__v_1
   LDA   __local_f8__p9_1
   EOR   #$FF
   TAX
   LDA   __zpabi_mix__v_1
   RTS

main:
   SUBROUTINE

   LDA   gs6+2
   BEQ   .if_end@3
   LDA   gs6+1
   BNE   .if_end@3
   LDA   g1
   STA   g1
.if_end@3:
   LDY   #$00
   LDA   (gp7),Y
   STA   __local_main__3
   SEC
   SBC   #$01
   STA   __local_main__1
   INY
   LDA   (gp7),Y
   SBC   #$00
   STA   __local_main__0
   LDA   __local_main__1
   DEY
   STA   (gp7),Y
   LDA   __local_main__0
   INY
   STA   (gp7),Y
   LDA   __local_main__3
   ORA   gs6+2
   AND   #$07
   CMP   #$03
   DEY
   LDA   (gp7),Y
   STA   __local_main__2
   INY
   LDA   (gp7),Y
   STA   __local_main__1
   LDA   z3
   SEC
   SBC   #$01
   STA   __local_main__0
   STA   z3
   LDA   __local_main__2
   SEC
   SBC   __local_main__0
   BCS   .carry_inc@1
   DEC   __local_main__1
.carry_inc@1:
   ORA   __local_main__1
   BEQ   .if_else@5
   LDY   #$00
   LDA   (gp7),Y
   STA   __local_main__3
   INY
   LDA   (gp7),Y
   STA   __local_main__2
   LDA   __local_main__3
   CLC
   ADC   #$01
   STA   __local_main__1
   LDA   __local_main__2
   ADC   #$00
   STA   __local_main__0
   LDA   __local_main__1
   DEY
   STA   (gp7),Y
   LDA   __local_main__0
   INY
   STA   (gp7),Y
   LDA   z3
   BEQ   .lnot_true@10
   LDA   #$00
   BEQ   .lnot_end@11
.lnot_true@10:
   LDA   #$01
.lnot_end@11:
   CLC
   ADC   __local_main__3
   STA   __zpabi_mix__v_0
   LDA   __local_main__2
   ADC   #$00
   JSR   fin0@run1
   JMP   .if_end@4
.if_else@5:
   LDA   z3
   ORA   #$03
   STA   z3
.if_end@4:
   LDA   ga5
   EOR   ga5+3
   STA   __zpabi_f8__p10
   LDX   #$00
   LDA   #$FF
   JSR   f8
   LDA   ga5
   SEC
   SBC   #$FF
   LDA   #$00
   ROL
   STA   __local_main__1
   LDA   gs6+1
   BEQ   .lnot_true@12
   LDA   #$00
   BEQ   .lnot_end@13
.lnot_true@12:
   LDA   #$01
.lnot_end@13:
   STA   __zpabi_f8__p10
   LDX   #$00
   LDA   __local_main__1
   JSR   f8
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
   DS.B  2

ga5:
   DC.B  $02, $FF, $00, $FF

gs6:
   DS.B  3

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   g1
   STA   z3
   LDA   #$FF
   STA   z4
   STA   z4+1
   LDA   #<g2
   STA   gp7
   LDA   #>g2
   STA   gp7+1
   RTS
