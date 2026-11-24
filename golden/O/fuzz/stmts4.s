SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$27
g3	EQU	$29
gp8	EQU	$2B
__zpabi_f24__p26_0	EQU	$80
__zpabi_f9__p11	EQU	$80
__local_f9__0	EQU	$81
__zpabi_f24__p26_1	EQU	$81
__local_f9__w15	EQU	$82
__zpabi_f24__p27	EQU	$82
__local_f9__1	EQU	$83
__zpabi_f24__pp28_0	EQU	$83
__local_f9__2	EQU	$84
__zpabi_f24__pp28_1	EQU	$84
__zpabi_mix__v_0	EQU	$85
__zpabi_mix__v_1	EQU	$86
__zpabi_mix__v_2	EQU	$87
__zpabi_mix__v_3	EQU	$88
__local_main__1	EQU	$8A
__local_main__l34	EQU	$8D
__local_f24__2	EQU	$90
__local_f24__3	EQU	$91
__local_f24__l29	EQU	$92
__local_mix__0	EQU	$93
__local_mix__1	EQU	$94
__local_mix__2	EQU	$95
__local_mix__3	EQU	$96

; @zp-link-meta-begin
; def f16 params=__zpabi_f16__pp17_0,__zpabi_f16__pp17_1 locals=__local_f16__0 indirect=false in_cycle=false param_regs=A,X
; def f24 params=__zpabi_f24__p25_0,__zpabi_f24__p25_1,__zpabi_f24__p26_0,__zpabi_f24__p26_1,__zpabi_f24__p27,__zpabi_f24__pp28_0,__zpabi_f24__pp28_1 locals=__local_f24__0,__local_f24__1,__local_f24__2,__local_f24__3,__local_f24__l29 indirect=false in_cycle=false param_regs=A,X,,,,,
; def f9 params=__zpabi_f9__p10_0,__zpabi_f9__p10_1,__zpabi_f9__p11 locals=__local_f9__0,__local_f9__w15,__local_f9__1,__local_f9__2 indirect=false in_cycle=false param_regs=A,X,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l34 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f24 -> mix
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f24
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run5 section=.text local=true zero=false
; item fin1@run3 section=.text local=true zero=false
; item fin1@run1 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f9@run6 section=.text local=true zero=false
; item f9@run2 section=.text local=true zero=false
; item f9@run0 section=.text local=true zero=false
; item f16 section=.text local=false zero=false
; item f24 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item @3.s14 section=.bss local=true zero=true
; item __zp_init section=.text local=false zero=false
; zp z4 size=1 at=auto def=true init=$FF
; zp z5 size=2 at=auto def=true init=$00,$00
; zp g3 size=2 at=auto def=true init=$00,$00 weight=25
; zp gp8 size=2 at=auto def=true init=<z4,>z4 weight=6
; runtime hargs_size=32 hargs_need=24
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
   JSR   fin0@run4
   LDA   g2+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g3+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   z4
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z4
   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   JSR   fin1@run3
   LDA   ga6
   JSR   fin1@run1
   LDA   ga6+1
   JSR   fin1@run1
   LDA   ga6+2
   JSR   fin1@run5
   JMP   mix

fin1@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin1@run1:
   SUBROUTINE

   JSR   fin1@run5
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+3
   JSR   fin1@run1
   LDA   gs7+1
   JSR   fin1@run1
   LDA   gs7+3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs7+2
   STA   __zpabi_mix__v_0
   LDA   gs7+3
   STA   __zpabi_mix__v_1
   JMP   mix

f9:
   SUBROUTINE

   LDA   z5
   AND   gs7+1
   STA   g1
   LDA   #$01
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g1+4
   STA   g1+5
   STA   g1+6
   STA   g1+7
   JSR   f9@run6
   LDA   #$00
   STA   __zpabi_mix__v_0
   JSR   f9@run6
   LDA   g1
   STA   __local_f9__2
   LDA   g1+1
   STA   __local_f9__1
   LDA   g1+2
   STA   __local_f9__w15
   LDA   g1+3
   STA   __local_f9__0
   LDA   g1
   SEC
   SBC   #$01
   STA   g1
   LDA   g1+1
   SBC   #$00
   STA   g1+1
   LDA   g1+2
   SBC   #$00
   STA   g1+2
   LDA   g1+3
   SBC   #$00
   STA   g1+3
   LDA   g1+4
   SBC   #$00
   STA   g1+4
   LDA   g1+5
   SBC   #$00
   STA   g1+5
   LDA   g1+6
   SBC   #$00
   STA   g1+6
   BCS   .carry_inc@0
   DEC   g1+7
.carry_inc@0:
   LDA   #$FF
   EOR   __local_f9__2
   STA   __zpabi_mix__v_0
   LDA   #$FF
   EOR   __local_f9__1
   STA   __zpabi_mix__v_1
   LDA   #$FF
   EOR   __local_f9__w15
   STA   __zpabi_mix__v_2
   LDA   #$FF
   EOR   __local_f9__0
   JSR   fin0@run4
   INC   g1
   BNE   .inc_done@0
   INC   g1+1
   BNE   .inc_done@0
   INC   g1+2
   BNE   .inc_done@0
   INC   g1+3
   BNE   .inc_done@0
   INC   g1+4
   BNE   .inc_done@0
   INC   g1+5
   BNE   .inc_done@0
   INC   g1+6
   BNE   .inc_done@0
   INC   g1+7
.inc_done@0:
   JSR   f9@run2
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   JSR   f9@run0
   LDA   #$06
   STA   HARGS+8
   JSR   lsr64
   LDA   HARGS+9
   STA   g1
   LDA   HARGS+10
   STA   g1+1
   LDA   HARGS+11
   STA   g1+2
   LDA   HARGS+12
   STA   g1+3
   LDA   HARGS+13
   STA   g1+4
   LDA   HARGS+14
   STA   g1+5
   LDA   HARGS+15
   STA   g1+6
   LDA   HARGS+16
   STA   g1+7
   LDA   g1
   CLC
   ADC   z4
   STA   z4
   LDA   #$00
   STA   __local_f9__w15
   JMP   .loop@0_continue
.f9@rot@0:
   LDX   #$05
.shift_loop@10:
   LDA   g3+1
   ASL
   ROR   g3+1
   ROR   g3
   DEX
   BNE   .shift_loop@10
   LDA   __local_f9__0
   STA   __local_f9__w15
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_f9__0
   LDA   __local_f9__w15
   CMP   #$02
   BCC   .f9@rot@0
.if_end@0:
   LDA   @3.s14
   CLC
   ADC   ga6
   STA   __local_f9__0
   LDA   @3.s14+1
   ADC   #$00
   TAX
   LDA   __local_f9__0
   RTS

f9@run6:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@run4

f9@run2:
   SUBROUTINE

   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   ORA   g1+4
   ORA   g1+5
   ORA   g1+6
   ORA   g1+7
   RTS

f9@run0:
   SUBROUTINE

   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   LDA   g1+4
   STA   HARGS+4
   LDA   g1+5
   STA   HARGS+5
   LDA   g1+6
   STA   HARGS+6
   LDA   g1+7
   STA   HARGS+7
   RTS

f16:
   SUBROUTINE

   LDA   ga6+1
   RTS

f24:
   SUBROUTINE

   LDA   #$02
   STA   __local_f24__l29
   LDA   z5+1
   LSR
   LDA   z5
   ROR
   STA   z5+1
   LDA   #$00
   ROR
   STA   z5
   LDA   __zpabi_f24__p26_0
   EOR   z5
   STA   __zpabi_mix__v_0
   LDA   __zpabi_f24__p26_1
   EOR   z5+1
   JSR   fin1@run3
   LDY   #$00
   LDA   (gp8),Y
   CLC
   ADC   #$01
   STA   (gp8),Y
   LDA   (gp8),Y
   STA   __local_f24__3
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f24__2
   LDA   gs7+2
   SEC
   SBC   __local_f24__3
   LDA   gs7+3
   SBC   __local_f24__2
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   LDY   #$00
   STA   (gp8),Y
   LDA   __local_f24__l29
   ORA   #$C6
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l34
   JSR   f9@run2
   BEQ   .cond_else@2
   LDA   #$00
   STA   __zpabi_f24__p26_0
   JMP   .cond_end@3
.cond_else@2:
   LDA   gs7+1
   STA   __zpabi_f24__p26_0
.cond_end@3:
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_main__1
   LDA   z5
   EOR   __local_main__1
   STA   __zpabi_f24__p27
   LDA   #$00
   STA   __zpabi_f24__p26_1
   LDA   #<__local_main__l34
   STA   __zpabi_f24__pp28_0
   LDA   #>__local_main__l34
   STA   __zpabi_f24__pp28_1
   LDX   #$00
   LDA   #$01
   JSR   f24
   LDA   gs7+1
   SEC
   SBC   #$FF
   STA   gs7+1
   LDA   #$FF
   STA   __zpabi_mix__v_0
   JSR   fin1@run3
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_main__1
   SEC
   LDA   #$00
   SBC   __local_main__1
   STA   gs7+1
   JSR   f9@run0
   LDA   #$5A
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   STA   g1
   LDA   HARGS+17
   STA   g1+1
   LDA   HARGS+18
   STA   g1+2
   LDA   HARGS+19
   STA   g1+3
   LDA   HARGS+20
   STA   g1+4
   LDA   HARGS+21
   STA   g1+5
   LDA   HARGS+22
   STA   g1+6
   LDA   HARGS+23
   STA   g1+7
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
   DC.L  $00000100
   DC.L  $00000000

g2:
   DC.W  $FFFF

ga6:
   DC.B  $FF, $57, $03, $02

gs7:
   DS.B  2
   DC.W  $0100

@3.s14:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g3
   STA   g3+1
   STA   z5
   STA   z5+1
   LDA   #$FF
   STA   z4
   LDA   #<z4
   STA   gp8
   LDA   #>z4
   STA   gp8+1
   RTS
