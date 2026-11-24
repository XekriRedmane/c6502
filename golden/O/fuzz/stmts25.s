SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$28
z5	EQU	$29
g2	EQU	$2A
__zpabi_f23__p25	EQU	$80
__zpabi_f23__pp26_0	EQU	$81
__zpabi_f23__pp26_1	EQU	$82
__zpabi_f14__p16_0	EQU	$83
__zpabi_mix__v_0	EQU	$83
__zpabi_f14__p16_1	EQU	$84
__zpabi_mix__v_1	EQU	$84
__zpabi_f14__p17_0	EQU	$85
__zpabi_mix__v_2	EQU	$85
__zpabi_f14__p17_1	EQU	$86
__zpabi_mix__v_3	EQU	$86
__zpabi_f14__pp18_0	EQU	$87
__zpabi_f14__pp18_1	EQU	$88
__local_main__l31	EQU	$89
__local_main__l34	EQU	$8A
__local_f23__2	EQU	$8B
__local_f9__l13	EQU	$8B
__local_f23__l29	EQU	$8D
__local_f9__pp10	EQU	$8D
__local_f9__1	EQU	$8E
__local_f14__l19	EQU	$8F
__local_f14__0	EQU	$90
__local_mix__0	EQU	$90
__local_mix__1	EQU	$91
__local_mix__2	EQU	$92
__local_mix__3	EQU	$93

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15,__zpabi_f14__p16_0,__zpabi_f14__p16_1,__zpabi_f14__p17_0,__zpabi_f14__p17_1,__zpabi_f14__pp18_0,__zpabi_f14__pp18_1 locals=__local_f14__l19,__local_f14__0 indirect=false in_cycle=false param_regs=A,,,,,,
; def f23 params=__zpabi_f23__p24_0,__zpabi_f23__p24_1,__zpabi_f23__p25,__zpabi_f23__pp26_0,__zpabi_f23__pp26_1 locals=__local_f23__0,__local_f23__1,__local_f23__2,__local_f23__3,__local_f23__l29,__local_f23__5 indirect=false in_cycle=false param_regs=A,X,,,
; def f9 params=__zpabi_f9__pp10_0,__zpabi_f9__pp10_1 locals=__local_f9__l13,__local_f9__0,__local_f9__pp10,__local_f9__1,__local_f9__2 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__l31,__local_main__l34 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f23 -> f14
; call f23 -> mix
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
; item fin0@run6 section=.text local=true zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run7 section=.text local=true zero=false
; item fin1@tail0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run8 section=.text local=true zero=false
; item f9 section=.text local=false zero=false
; item f9@run2 section=.text local=true zero=false
; item f14 section=.text local=false zero=false
; item f23 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item @9.s22 section=.bss local=true zero=true
; item @16.s30 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$FF,$7F
; zp z4 size=1 at=auto def=true init=$FF
; zp z5 size=1 at=auto def=true init=$01
; zp g2 size=1 at=auto def=true init=$35 weight=10
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

   JSR   fin0@run6
   JSR   fin0@run4
   LDA   g2
   JSR   fin0@run5
   JSR   fin0@run3
   JSR   fin0@run4
   LDA   z4
   JSR   fin0@run1
   JMP   mix

fin0@run6:
   SUBROUTINE

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   RTS

fin0@run5:
   SUBROUTINE

   JSR   fin0@run1
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   fin0@run0

fin0@run3:
   SUBROUTINE

   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z5
   STA   __zpabi_mix__v_0
   JSR   mix
   JSR   fin1@run7
   JSR   fin0@run4
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   LDA   ga6+3
   JSR   fin0@run4
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   JMP   fin1@tail0

fin1@run7:
   SUBROUTINE

   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   RTS

fin1@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+6
   STA   __zpabi_mix__v_0
   LDA   ga6+7
   JSR   fin0@run4
   LDA   gs7+1
   JSR   fin0@run5
   JSR   fin2@run8
   JMP   fin1@tail0

fin2@run8:
   SUBROUTINE

   LDA   gs7+2
   STA   __zpabi_mix__v_0
   LDA   gs7+3
   RTS

f9:
   SUBROUTINE

   STA   __local_f9__pp10
   STX   __local_f9__1
   LDA   #$7F
   STA   __local_f9__l13
   LDA   gs7+1
   SEC
   SBC   #$01
   STA   __local_f9__l13
   STA   gs7+1
   LDA   #$FD
   CLC
   ADC   __local_f9__l13
   STA   __zpabi_mix__v_0
   LDA   #$FF
   ADC   #$00
   STA   __zpabi_mix__v_1
   LDA   #$FF
   ADC   #$00
   STA   __zpabi_mix__v_2
   LDA   #$FF
   ADC   #$00
   STA   __zpabi_mix__v_3
   JSR   mix
   DEC   g2
   LDA   g2
   AND   #$07
   BNE   .lb_skip@0
   JMP   .switch@0_break
.lb_skip@0:
   CMP   #$03
   BNE   .default@3
   LDA   gs7+2
   LDX   gs7+3
   STX   __zpabi_mix__v_1
   EOR   g2
   STA   __zpabi_mix__v_0
   JSR   fin0@run0
   JMP   .switch@0_break
.default@3:
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   LDA   gs7+3
   LSR
   ROR   __zpabi_mix__v_1
   LDA   #$00
   ROR
   STA   gs7+2
   LDX   __zpabi_mix__v_1
   STX   gs7+3
   AND   #$07
   CMP   #$02
   BNE   .default@9
   JSR   f9@run2
   LDA   #$0F
   LDY   #$00
   STA   (DPTR),Y
   JSR   f9@run2
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   JMP   .switch@7_break
.default@9:
   DEC   z4
   LDA   z4
   AND   #$01
   JSR   fin0@run5
.switch@7_break:
   LDA   #$80
   LDY   #$00
   STA   (__local_f9__pp10),Y
.switch@0_break:
   JSR   f9@run2
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f9__pp10
   LDA   z5
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   LDA   __local_f9__pp10
   CLC
   ADC   z5
   STA   __local_f9__l13
   LDA   #$00
   ADC   __zpabi_mix__v_1
   TAX
   LDA   __local_f9__l13
   RTS

f9@run2:
   SUBROUTINE

   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   RTS

f14:
   SUBROUTINE

   LDA   #$00
   STA   __local_f14__l19
   STA   __local_f14__0
   LDA   __zpabi_f14__p16_0
   STA   z3
   LDA   __zpabi_f14__p16_1
   STA   z3+1
   DEC   z5
   INC   g2
   LDA   g2
   STA   gs7+1
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   g2
   LDA   __zpabi_f14__p17_0
   AND   ga6
   STA   __local_f14__l19
   LDA   g2
   EOR   __local_f14__l19
   STA   z5
   LDA   @9.s22
   LDX   @9.s22+1
   RTS

f23:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f23__l29
   LDA   #$7F
   STA   __local_f23__l29+1
   LDA   z5
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   z5
   JSR   fin0@run3
   LDA   #$00
   EOR   __zpabi_mix__v_0
   STA   __local_f23__2
   LDA   #$FF
   EOR   #$FF
   STA   __zpabi_f14__p17_0
   LDA   #$7F
   EOR   #$FF
   STA   __zpabi_f14__p17_1
   LDA   @16.s30
   STA   __zpabi_f14__p16_0
   LDA   @16.s30+1
   STA   __zpabi_f14__p16_1
   LDA   #<__local_f23__l29
   STA   __zpabi_f14__pp18_0
   LDA   #>__local_f23__l29
   STA   __zpabi_f14__pp18_1
   LDA   __local_f23__2
   JSR   f14
   LDA   #$7F
   STA   __zpabi_mix__v_1
   LDA   __zpabi_f23__p25
   SEC
   SBC   #$81
   STA   __zpabi_mix__v_0
   LDA   #$FF
   CLC
   ADC   __zpabi_mix__v_0
   BCC   .carry_inc@0
   INC   __zpabi_mix__v_1
.carry_inc@0:
   ORA   __zpabi_mix__v_1
   BEQ   .if_end@1
   INC   z4
   BEQ   .if_end@2
   JSR   fin2@run8
   LSR
   ROR   __zpabi_mix__v_0
   LSR
   ROR   __zpabi_mix__v_0
   LSR
   ROR   __zpabi_mix__v_0
   LSR
   STA   __zpabi_mix__v_1
   ROR   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_0
   EOR   #$FF
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   EOR   #$FF
   JSR   fin0@run4
.if_end@2:
.if_end@1:
   LDA   z5
   ORA   #$03
   STA   __zpabi_mix__v_1
   STA   z5
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   ORA   __zpabi_mix__v_1
   STA   @16.s30
   LDA   gs7+3
   ORA   __zpabi_mix__v_0
   STA   @16.s30+1
   JSR   fin1@run7
   STA   __zpabi_mix__v_1
   INC   __zpabi_mix__v_0
   BNE   .inc_done@0
   INC   __zpabi_mix__v_1
.inc_done@0:
   LDA   __zpabi_mix__v_0
   STA   ga6
   LDA   __zpabi_mix__v_1
   STA   ga6+1
   JSR   fin0@run0
   JSR   fin0@run6
   LDA   #$FF
   ORA   __zpabi_mix__v_0
   RTS

main:
   SUBROUTINE

   LDA   #$03
   STA   __local_main__l31
   LDA   #$00
   STA   __local_main__l34
   LDA   gs7+2
   SEC
   SBC   #$01
   STA   gs7+2
   BCS   .carry_inc@1
   DEC   gs7+3
.carry_inc@1:
   LDA   z3
   STA   __zpabi_mix__v_1
   LDA   z3+1
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   EOR   #$01
   STA   __zpabi_mix__v_1
   STA   z3
   LDA   __zpabi_mix__v_0
   STA   z3+1
   LDA   __zpabi_mix__v_1
   ORA   __zpabi_mix__v_0
   BEQ   .if_else@7
   JSR   fin0@run3
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   SEC
   SBC   #$01
   BCS   .carry_inc@2
   DEC   __zpabi_mix__v_1
.carry_inc@2:
   STA   z3
   LDA   __zpabi_mix__v_1
   STA   z3+1
   JSR   fin0@run3
   STA   __zpabi_mix__v_1
   INC   __zpabi_mix__v_0
   BNE   .inc_done@1
   INC   __zpabi_mix__v_1
.inc_done@1:
   LDA   __zpabi_mix__v_0
   STA   z3
   LDA   __zpabi_mix__v_1
   STA   z3+1
   JSR   fin0@run0
   JMP   .if_end@6
.if_else@7:
   JSR   f9@run2
   LDY   #$00
   LDA   (DPTR),Y
   ORA   #$5A
   STA   (DPTR),Y
   LDX   #>__local_main__l34
   LDA   #<__local_main__l34
   JSR   f9
.if_end@6:
   LDA   #$00
   STA   z4
   LDA   #$03
   EOR   __local_main__l31
   AND   #$07
   CMP   #$04
   BNE   .switch@12_break
   LDA   gs7+1
   STA   __zpabi_mix__v_1
   CLC
   ADC   #$01
   STA   gs7+1
   JSR   fin0@run3
   LDA   __zpabi_mix__v_1
   ORA   __zpabi_mix__v_0
   STA   g2
.switch@12_break:
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
   DC.W  $0080

ga6:
   DC.W  $FFFF
   DC.W  $FFFF
   DC.W  $7FFF
   DC.W  $0002

gs7:
   DS.B  1
   DC.B  $01
   DS.B  2

gp8:
   DC.W  z4

@9.s22:
   DS.B  2

@16.s30:
   DC.W  $FFFF

__zp_init:
   SUBROUTINE

   LDA   #$35
   STA   g2
   LDA   #$FF
   STA   z3
   STA   z4
   LDA   #$7F
   STA   z3+1
   LDA   #$01
   STA   z5
   RTS
