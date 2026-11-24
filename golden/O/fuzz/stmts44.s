SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
g2	EQU	$27
g4	EQU	$28
__zpabi_f9__p11_0	EQU	$80
__zpabi_f9__p11_1	EQU	$81
__zpabi_mix__v_0	EQU	$82
__zpabi_mix__v_1	EQU	$83
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_main__0	EQU	$86
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_main__4	EQU	$8A
__local_main__5	EQU	$8B
__local_main__6	EQU	$8C
__local_main__7	EQU	$8D
__local_f9__l12	EQU	$8E
__local_f9__p10	EQU	$92
__local_mix__0	EQU	$93
__local_mix__1	EQU	$94
__local_mix__2	EQU	$95
__local_mix__3	EQU	$96

; @zp-link-meta-begin
; def f9 params=__zpabi_f9__p10_0,__zpabi_f9__p10_1,__zpabi_f9__p11_0,__zpabi_f9__p11_1 locals=__local_f9__l12,__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__p10 indirect=false in_cycle=false param_regs=A,X,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f9
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.bss local=false zero=true
; item g3 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=1 at=auto def=true init=$FF
; zp g2 size=1 at=auto def=true init=$FF weight=6
; zp g4 size=2 at=auto def=true init=$03,$00 weight=11
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
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   JSR   fin0@run0
   LDA   g2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g3
   JSR   fin0@run1
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   JMP   fin0@tail0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin1@run2
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   STA   __zpabi_mix__v_1
   LDA   ga6+2
   STA   __zpabi_mix__v_2
   LDA   ga6+3
   JSR   fin0@run0
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   STA   __zpabi_mix__v_1
   LDA   ga6+6
   STA   __zpabi_mix__v_2
   LDA   ga6+7
   JSR   fin0@run0
   LDA   ga6+8
   STA   __zpabi_mix__v_0
   LDA   ga6+9
   STA   __zpabi_mix__v_1
   LDA   ga6+10
   STA   __zpabi_mix__v_2
   LDA   ga6+11
   STA   __zpabi_mix__v_3
   JMP   mix

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin2:
   SUBROUTINE

   LDA   ga6+12
   STA   __zpabi_mix__v_0
   LDA   ga6+13
   STA   __zpabi_mix__v_1
   LDA   ga6+14
   STA   __zpabi_mix__v_2
   LDA   ga6+15
   JSR   fin0@run0
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   LDA   gs7+3
   STA   __zpabi_mix__v_2
   LDA   gs7+4
   JSR   fin0@run0
   LDA   gs7+5
   STA   __zpabi_mix__v_0
   LDA   gs7+6
   JMP   fin0@tail0

f9:
   SUBROUTINE

   STA   __local_f9__p10
   LDA   #$00
   STA   __local_f9__l12
   LDA   gs7+5
   ORA   gs7+6
   BEQ   .if_else@1
   LDA   g2
   ORA   #$03
   STA   g2
   AND   #$81
   JSR   fin0@run1
   JMP   .if_end@0
.if_else@1:
   LDA   #$00
   STA   g4+1
   LDA   #$7F
   EOR   g4
   STA   __zpabi_mix__v_0
   LDA   #$00
   JSR   fin1@run2
.if_end@0:
   LDA   ga6+12
   STA   HARGS
   LDA   ga6+13
   STA   HARGS+1
   LDA   ga6+14
   STA   HARGS+2
   LDA   ga6+15
   STA   HARGS+3
   LDA   #$04
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   STA   ga6+12
   LDA   HARGS+6
   STA   ga6+13
   LDA   HARGS+7
   STA   ga6+14
   LDA   HARGS+8
   STA   ga6+15
   LDA   g4
   STA   HARGS
   LDA   __local_f9__p10
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   AND   #$01
   STA   z5
   LDA   g1
   SEC
   SBC   gs7+1
   STA   __local_f9__l12
   LDA   g1+1
   SBC   gs7+2
   TAX
   LDA   __local_f9__l12
   RTS

main:
   SUBROUTINE

   LDA   gs7+5
   LDX   gs7+6
   STX   __zpabi_mix__v_1
   SEC
   SBC   #$81
   STA   __local_main__0
   LDA   __zpabi_mix__v_1
   SBC   #$00
   STA   gs7+6
   LDA   __local_main__0
   STA   gs7+5
   STA   z5
   LDA   g3
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@4:
   BCC   .mul_skip@5
   CLC
.mul_skip@5:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@4
   TAX
   LDA   ga6+12
   STA   __zpabi_f9__p11_0
   LDA   ga6+13
   STA   __zpabi_f9__p11_1
   LDA   HARGS
   JSR   f9
   STA   __zpabi_mix__v_1
   TXA
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JSR   mix
   LDA   #$81
   STA   g1
   LDA   #$FF
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g1+4
   STA   g1+5
   STA   g1+6
   STA   g1+7
   INC   g4
   BNE   .inc_done@0
   INC   g4+1
.inc_done@0:
   LDA   ga6+8
   STA   __local_main__7
   LDA   ga6+9
   STA   __local_main__6
   LDA   ga6+10
   STA   __local_main__5
   LDA   ga6+11
   STA   __local_main__4
   LDA   g2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_main__2
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_mix__v_1
   STA   __local_main__0
   LDA   __local_main__4
   CMP   __local_main__0
   BNE   .if_else@5
   LDA   __local_main__5
   CMP   __zpabi_mix__v_1
   BNE   .if_else@5
   LDA   __local_main__6
   CMP   __local_main__2
   BNE   .if_else@5
   LDA   __local_main__7
   CMP   g2
   BNE   .if_else@5
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   CLC
   ADC   #$05
   STA   __local_main__0
   LDA   #$00
   ADC   #$00
   TAX
   LDA   #$00
   STA   __zpabi_f9__p11_0
   STA   __zpabi_f9__p11_1
   LDA   __local_main__0
   JSR   f9
   JMP   .if_end@4
.if_else@5:
   LDA   g4
   BNE   .carry_inc@0
   DEC   g4+1
.carry_inc@0:
   DEC   g4
.if_end@4:
   LDA   ga6
   STA   __local_main__3
   LDA   ga6+1
   STA   __local_main__2
   LDA   ga6+2
   STA   __zpabi_mix__v_1
   LDA   ga6+3
   STA   __local_main__0
   LDA   __local_main__3
   SEC
   SBC   #$01
   STA   ga6
   LDA   __local_main__2
   SBC   #$00
   STA   ga6+1
   LDA   __zpabi_mix__v_1
   SBC   #$00
   STA   ga6+2
   LDA   __local_main__0
   SBC   #$00
   STA   ga6+3
   LDA   __local_main__3
   ORA   __local_main__2
   ORA   __zpabi_mix__v_1
   ORA   __local_main__0
   BEQ   .if_end@8
   LDA   g2
   EOR   #$81
   STA   __local_main__0
   STA   g2
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_mix__v_1
   LDA   __local_main__0
   CLC
   ADC   #$FF
   STA   __local_main__0
   BCC   .carry_inc@1
   INC   __zpabi_mix__v_1
.carry_inc@1:
   LDA   __zpabi_mix__v_1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__0
   STA   __zpabi_mix__v_0
   JSR   mix
.if_end@8:
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
   DS.B  8

g3:
   DC.B  $01

ga6:
   DC.L  $0000FFFF
   DC.L  $00000007
   DC.L  $58D4DA7A
   DC.L  $00000080

gs7:
   DS.B  1
   DC.L  $0000007F
   DC.W  $0002

gp8:
   DC.W  g3

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g4+1
   LDA   #$FF
   STA   g2
   STA   z5
   LDA   #$03
   STA   g4
   RTS
