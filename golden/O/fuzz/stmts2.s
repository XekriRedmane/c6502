SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$28
gp8	EQU	$29
__zpabi_f9__p11	EQU	$80
__zpabi_f9__pp12_0	EQU	$81
__zpabi_f9__pp12_1	EQU	$82
__zpabi_mix__v_0	EQU	$83
__zpabi_mix__v_1	EQU	$84
__zpabi_mix__v_2	EQU	$85
__zpabi_mix__v_3	EQU	$86
__local_f9__3	EQU	$8A
__local_f9__p11	EQU	$8B
__local_f9__w14	EQU	$8C
__local_f9__l13	EQU	$8D
__local_mix__0	EQU	$90
__local_mix__1	EQU	$91
__local_mix__2	EQU	$92
__local_mix__3	EQU	$93

; @zp-link-meta-begin
; def f9 params=__zpabi_f9__p10_0,__zpabi_f9__p10_1,__zpabi_f9__p11,__zpabi_f9__pp12_0,__zpabi_f9__pp12_1 locals=__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__p11,__local_f9__w14,__local_f9__l13,__local_f9__4,__local_f9__5 indirect=false in_cycle=false param_regs=A,X,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin1@run1 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$00,$00
; zp z5 size=1 at=auto def=true init=$00
; zp gp8 size=2 at=auto def=true init=<z4,>z4 weight=16
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
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
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

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   JSR   fin1@run2
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   JSR   fin1@run1
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   LDA   ga6+3
   JSR   fin1@run1
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin1@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin2:
   SUBROUTINE

   LDA   ga6+6
   STA   __zpabi_mix__v_0
   LDA   ga6+7
   JSR   fin1@run1
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   LDA   gs7+3
   STA   __zpabi_mix__v_2
   LDA   gs7+4
   JSR   fin0@run0
   LDA   gs7+5
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs7+5
   STA   __zpabi_mix__v_0
   JMP   mix

f9:
   SUBROUTINE

   LDA   #$00
   STA   __local_f9__l13
   LDY   #$00
   LDA   (__zpabi_f9__pp12_0),Y
   STA   __local_f9__w14
   INY
   LDA   (__zpabi_f9__pp12_0),Y
   STA   __local_f9__p11
   INY
   LDA   (__zpabi_f9__pp12_0),Y
   STA   __local_f9__3
   LDA   __local_f9__w14
   SEC
   SBC   #$01
   STA   __zpabi_mix__v_0
   LDA   __local_f9__p11
   SBC   #$00
   STA   __zpabi_mix__v_1
   LDA   __local_f9__3
   SBC   #$00
   AND   #$01
   EOR   #$01
   STA   __zpabi_mix__v_2
   DEC   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_0
   LDY   #$00
   STA   (__zpabi_f9__pp12_0),Y
   LDA   __zpabi_mix__v_1
   INY
   STA   (__zpabi_f9__pp12_0),Y
   LDA   __zpabi_mix__v_2
   INY
   STA   (__zpabi_f9__pp12_0),Y
   LDA   __local_f9__w14
   ORA   __local_f9__p11
   ORA   __local_f9__3
   BNE   .lb_skip@1
   JMP   .if_end@0
.lb_skip@1:
   LDA   #$00
   STA   __local_f9__w14
   LDA   __zpabi_f9__p11
   STA   __local_f9__p11
.loop@0_start:
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f9__3
   SEC
   SBC   #$01
   STA   __zpabi_mix__v_1
   INY
   LDA   (gp8),Y
   SBC   #$00
   STA   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_1
   DEY
   STA   (gp8),Y
   LDA   __zpabi_mix__v_2
   INY
   STA   (gp8),Y
   LDA   __local_f9__3
   AND   #$07
   BEQ   .switch@1_break
   CMP   #$02
   BEQ   .case@3
   CMP   #$07
   BEQ   .case@4
   BNE   .switch@1_break
.case@3:
   LDA   __local_f9__p11
   ASL
   ASL
   ASL
   STA   __local_f9__p11
   JSR   fin1@run2
.case@4:
   LDY   #$00
   LDA   (__zpabi_f9__pp12_0),Y
   EOR   #$FF
   STA   __zpabi_mix__v_0
   INY
   LDA   (__zpabi_f9__pp12_0),Y
   STA   __zpabi_mix__v_1
   INY
   LDA   (__zpabi_f9__pp12_0),Y
   STA   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_0
   LDY   #$00
   STA   (__zpabi_f9__pp12_0),Y
   LDA   __zpabi_mix__v_2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run0
.switch@1_break:
   LDA   gs7+1
   LDX   gs7+2
   STX   __zpabi_mix__v_0
   LDX   gs7+3
   STX   __zpabi_mix__v_1
   LDX   gs7+4
   STX   __zpabi_mix__v_2
   ORA   #$5A
   STA   gs7+1
   ORA   __zpabi_mix__v_0
   ORA   __zpabi_mix__v_1
   ORA   __zpabi_mix__v_2
   BNE   .loop@0_break
   INC   __local_f9__w14
   LDA   __local_f9__w14
   CMP   #$02
   BCS   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
.loop@0_break:
.if_end@0:
   LDA   __local_f9__l13
   STA   __zpabi_mix__v_1
   SEC
   SBC   #$01
   STA   __local_f9__l13
   LDA   __zpabi_mix__v_1
   STA   g1
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   __local_f9__l13
   STA   __zpabi_mix__v_2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   TAX
   LDA   __zpabi_mix__v_2
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   g3
   LDA   #$01
   LDY   #$00
   ORA   #$01
   STA   z4
   TYA
   STA   z4+1
   STA   ga6+5
   LDA   #$01
   STA   ga6+4
   LDA   #$20
   STA   g2
   TYA
   STA   g2+1
   STA   g2+2
   STA   g2+3
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
   DC.L  $0000007F

g3:
   DC.L  $0000007F

ga6:
   DC.W  $97E3
   DC.W  $0002
   DC.W  $0100
   DC.W  $000F

gs7:
   DS.B  1
   DC.L  $00000100
   DS.B  1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4
   STA   z4+1
   STA   z5
   LDA   #<z4
   STA   gp8
   LDA   #>z4
   STA   gp8+1
   RTS
