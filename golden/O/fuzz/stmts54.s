SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$27
g3	EQU	$29
__local_f9__0	EQU	$80
__zpabi_f14__p16_0	EQU	$80
__local_f9__1	EQU	$81
__zpabi_f14__p16_1	EQU	$81
__local_f9__2	EQU	$82
__zpabi_f14__p16_2	EQU	$82
__local_f9__3	EQU	$83
__zpabi_f14__p16_3	EQU	$83
__local_f9__4	EQU	$84
__zpabi_f14__pp17_0	EQU	$84
__local_f9__5	EQU	$85
__zpabi_f14__pp17_1	EQU	$85
__local_f9__6	EQU	$86
__zpabi_mix__v_0	EQU	$86
__local_f9__7	EQU	$87
__zpabi_mix__v_1	EQU	$87
__zpabi_mix__v_2	EQU	$88
__zpabi_mix__v_3	EQU	$89
__local_f14__w20	EQU	$8A
__local_main__l21	EQU	$8A
__local_f14__0	EQU	$8B
__local_main__0	EQU	$8B
__local_main__1	EQU	$8C
__local_main__2	EQU	$8D
__local_main__3	EQU	$8E
__local_main__4	EQU	$8F
__local_main__5	EQU	$90
__local_f14__6	EQU	$91
__local_main__6	EQU	$91
__local_f14__7	EQU	$92
__local_f14__p15_1	EQU	$97
__local_f14__p15_0	EQU	$98
__local_mix__0	EQU	$99
__local_mix__1	EQU	$9A
__local_mix__2	EQU	$9B
__local_mix__3	EQU	$9C

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15_0,__zpabi_f14__p15_1,__zpabi_f14__p16_0,__zpabi_f14__p16_1,__zpabi_f14__p16_2,__zpabi_f14__p16_3,__zpabi_f14__pp17_0,__zpabi_f14__pp17_1 locals=__local_f14__w20,__local_f14__0,__local_f14__1,__local_f14__2,__local_f14__3,__local_f14__4,__local_f14__5,__local_f14__6,__local_f14__7,__local_f14__p16_3,__local_f14__p16_2,__local_f14__p16_1,__local_f14__p16_0,__local_f14__p15_1,__local_f14__p15_0 indirect=false in_cycle=false param_regs=A,X,,,,,, changes=__zpabi_f14__p16_0
; def f9 params=__zpabi_f9__pp10_0,__zpabi_f9__pp10_1 locals=__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__4,__local_f9__5,__local_f9__6,__local_f9__7 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__l21,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run1 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run2 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item g4 section=.data local=false zero=false
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=1 at=auto def=true init=$35
; zp z6 size=2 at=auto def=true init=$FF,$7F
; zp g3 size=2 at=auto def=true init=$FF,$7F weight=516
; runtime hargs_size=32 hargs_need=17
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
   JSR   fin0@run3
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run0
   LDA   g3+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   g4+2
   STA   __zpabi_mix__v_2
   LDA   g4+3
   JMP   fin0@tail0

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

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
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   JSR   fin1@run1
   LDA   ga7
   STA   __zpabi_mix__v_0
   LDA   ga7+1
   STA   __zpabi_mix__v_1
   LDA   ga7+2
   STA   __zpabi_mix__v_2
   LDA   ga7+3
   JSR   fin0@run0
   LDA   ga7+8
   STA   __zpabi_mix__v_0
   LDA   ga7+9
   STA   __zpabi_mix__v_1
   LDA   ga7+10
   STA   __zpabi_mix__v_2
   LDA   ga7+11
   JMP   fin0@tail0

fin1@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin2:
   SUBROUTINE

   LDA   ga7+16
   STA   __zpabi_mix__v_0
   LDA   ga7+17
   STA   __zpabi_mix__v_1
   LDA   ga7+18
   STA   __zpabi_mix__v_2
   LDA   ga7+19
   JSR   fin0@run0
   LDA   ga7+24
   STA   __zpabi_mix__v_0
   LDA   ga7+25
   STA   __zpabi_mix__v_1
   LDA   ga7+26
   STA   __zpabi_mix__v_2
   LDA   ga7+27
   JSR   fin0@run0
   LDA   gs8+1
   STA   __zpabi_mix__v_0
   LDA   gs8+2
   STA   __zpabi_mix__v_1
   LDA   gs8+3
   STA   __zpabi_mix__v_2
   LDA   gs8+4
   JSR   fin0@run0
   LDA   gs8+9
   JSR   fin0@run3
   JMP   fin0@tail0

f9:
   SUBROUTINE

   LDA   z5
   STA   __local_f9__0
   LDX   #$05
.shift_loop@4:
   LDA   __local_f9__0
   ASL
   ROR   __local_f9__0
   DEX
   BNE   .shift_loop@4
   LDA   __local_f9__0
   STA   z5
   LDA   gs8+1
   EOR   #$FF
   STA   gs8+1
   LDA   ga7+16
   STA   __local_f9__7
   LDA   ga7+17
   STA   __local_f9__6
   LDA   ga7+18
   STA   __local_f9__5
   LDA   ga7+19
   STA   __local_f9__4
   LDA   ga7+20
   STA   __local_f9__3
   LDA   ga7+21
   STA   __local_f9__2
   LDA   ga7+22
   STA   __local_f9__1
   LDA   ga7+23
   STA   __local_f9__0
   LDA   #$00
   SEC
   SBC   __local_f9__7
   STA   ga7+16
   LDA   __local_f9__7
   SBC   __local_f9__6
   STA   ga7+17
   LDA   __local_f9__6
   SBC   __local_f9__5
   STA   ga7+18
   LDA   __local_f9__5
   SBC   __local_f9__4
   STA   ga7+19
   LDA   __local_f9__4
   SBC   __local_f9__3
   STA   ga7+20
   LDA   __local_f9__3
   SBC   __local_f9__2
   STA   ga7+21
   LDA   __local_f9__2
   SBC   __local_f9__1
   STA   ga7+22
   LDA   __local_f9__1
   SBC   __local_f9__0
   STA   ga7+23
   LDA   z5
   STA   __local_f9__0
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   HARGS+1
   LDA   __local_f9__0
   STA   HARGS
   LDA   #$04
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   RTS

f14:
   SUBROUTINE

   STA   __local_f14__p15_0
   STX   __local_f14__p15_1
   LDA   z5
   EOR   #$01
   STA   __local_f14__w20
   STA   z5
   LDA   #$00
   EOR   __local_f14__w20
   STA   gs8+9
   LDA   #$00
   STA   __local_f14__w20
   JMP   .loop@0_continue
.f14@rot@0:
   LDA   #$00
   LDY   #$00
   AND   __zpabi_f14__p16_0
   STA   g3
   STY   g3+1
   LDA   ga7+16
   AND   #$5A
   STA   __local_f14__6
   TYA
   STY   ga7+17
   STA   ga7+18
   LDA   __local_f14__6
   STA   ga7+16
   STY   ga7+19
   STY   ga7+20
   STY   ga7+21
   STY   ga7+22
   STY   ga7+23
   BEQ   .xjump@0
   LDA   __local_f14__p15_0
   ORA   __local_f14__p15_1
   BEQ   .cond_else@2
   LDA   __local_f14__7
   JMP   .cond_end@3
.cond_else@2:
   TYA
.cond_end@3:
   LDY   #$00
   STA   (__zpabi_f14__pp17_0),Y
.xjump@0:
   LDA   __local_f14__7
   STA   __local_f14__w20
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_f14__7
   LDA   __local_f14__w20
   CMP   #$04
   BCC   .f14@rot@0
   LDA   g1
   CLC
   ADC   #$7F
   STA   __local_f14__0
   LDA   #$00
   ADC   #$00
   ORA   __local_f14__0
   BEQ   .if_end@4
   LDY   #$00
   LDA   (__zpabi_f14__pp17_0),Y
   STA   __local_f14__0
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f14__w20
   LDA   __local_f14__0
   SEC
   SBC   __local_f14__p15_0
   STA   __zpabi_mix__v_0
   LDA   __local_f14__w20
   SBC   __local_f14__p15_1
   JSR   fin1@run1
   LDA   gs8+9
   AND   #$01
   STA   gs8+9
.if_end@4:
   LDA   g4
   AND   #$01
   STA   g4
   LDA   #$00
   STA   g4+1
   LDY   #$00
   STY   g4+2
   STY   g4+3
   STY   g4+4
   STY   g4+5
   STY   g4+6
   STA   g4+7
   LDA   #$7F
   RTS

main:
   SUBROUTINE

   LDA   #$03
   STA   __local_main__l21
   LDA   #$00
   STA   __local_main__0
   STA   __local_main__1
   STA   __local_main__2
   STA   __local_main__3
   STA   __local_main__4
   STA   __local_main__5
   STA   __local_main__6
   LDA   g3
   BNE   .carry_inc@0
   DEC   g3+1
.carry_inc@0:
   DEC   g3
   JSR   main@run2
   STX   __local_main__1
   LDX   g2+2
   STX   __local_main__0
   LDX   g2+3
   STX   __local_main__l21
   ORA   #$5A
   STA   g2
   LDA   __local_main__1
   STA   g2+1
   LDA   __local_main__0
   STA   g2+2
   LDA   __local_main__l21
   STA   g2+3
   LDA   gs8+1
   SEC
   SBC   #$01
   STA   gs8+1
   LDA   gs8+2
   SBC   #$00
   STA   gs8+2
   LDA   gs8+3
   SBC   #$00
   STA   gs8+3
   LDA   gs8+4
   SBC   #$00
   STA   gs8+4
   LDA   gs8+5
   SBC   #$00
   STA   gs8+5
   LDA   gs8+6
   SBC   #$00
   STA   gs8+6
   LDA   gs8+7
   SBC   #$00
   STA   gs8+7
   BCS   .carry_inc@1
   DEC   gs8+8
.carry_inc@1:
   LDA   z5
   STA   __local_main__0
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_main__l21
   LDA   #$FF
   CLC
   ADC   __local_main__0
   STA   __local_main__0
   LDA   #$FF
   ADC   __local_main__l21
   ORA   __local_main__0
   BNE   .lb_skip@1
   JMP   .if_end@5
.lb_skip@1:
   LDA   ga7+16
   SEC
   SBC   #$05
   STA   ga7+16
   LDA   ga7+17
   SBC   #$00
   STA   ga7+17
   LDA   ga7+18
   SBC   #$00
   STA   ga7+18
   LDA   ga7+19
   SBC   #$00
   STA   ga7+19
   LDA   ga7+20
   SBC   #$00
   STA   ga7+20
   LDA   ga7+21
   SBC   #$00
   STA   ga7+21
   LDA   ga7+22
   SBC   #$00
   STA   ga7+22
   BCS   .carry_inc@2
   DEC   ga7+23
.carry_inc@2:
   LDA   ga7+8
   STA   HARGS
   LDA   ga7+9
   STA   HARGS+1
   LDA   ga7+10
   STA   HARGS+2
   LDA   ga7+11
   STA   HARGS+3
   LDA   ga7+12
   STA   HARGS+4
   LDA   ga7+13
   STA   HARGS+5
   LDA   ga7+14
   STA   HARGS+6
   LDA   ga7+15
   STA   HARGS+7
   LDA   #$03
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   ORA   HARGS+12
   ORA   HARGS+13
   ORA   HARGS+14
   ORA   HARGS+15
   ORA   HARGS+16
   BNE   .lb_skip@0
   JMP   .if_end@6
.lb_skip@0:
   JSR   main@run2
   STX   __local_main__1
   LDX   g2+2
   STX   __local_main__0
   LDX   g2+3
   STX   __local_main__l21
   SEC
   SBC   g4
   STA   gs8+1
   LDA   __local_main__1
   SBC   g4+1
   STA   gs8+2
   LDA   __local_main__0
   SBC   g4+2
   STA   gs8+3
   LDA   __local_main__l21
   SBC   g4+3
   STA   gs8+4
   LDA   #$00
   SBC   g4+4
   STA   gs8+5
   LDA   #$00
   SBC   g4+5
   STA   gs8+6
   LDA   #$00
   SBC   g4+6
   STA   gs8+7
   LDA   #$00
   SBC   g4+7
   STA   gs8+8
   JSR   main@run2
   STX   __local_main__1
   LDX   g2+2
   STX   __local_main__0
   LDX   g2+3
   STX   __local_main__l21
   EOR   #$5A
   STA   g2
   LDX   __local_main__1
   STX   g2+1
   LDX   __local_main__0
   STX   g2+2
   LDX   __local_main__l21
   STX   g2+3
   STA   __zpabi_mix__v_0
   LDA   __local_main__1
   STA   __zpabi_mix__v_1
   LDA   __local_main__0
   STA   __zpabi_mix__v_2
   LDA   __local_main__l21
   JSR   fin0@run0
   LDA   z5
   AND   #$07
   CMP   #$01
.if_end@6:
.if_end@5:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run2:
   SUBROUTINE

   LDA   g2
   LDX   g2+1
   RTS

acc:
   DS.B  2

g1:
   DC.B  $FF

g2:
   DC.L  $00000080

g4:
   DC.L  $00008000
   DC.L  $00000000

ga7:
   DC.L  $00008000
   DC.L  $00000000
   DC.L  $7FFFFFFF
   DC.L  $00000000
   DC.L  $0000007F
   DC.L  $00000000
   DC.L  $0000FFFF
   DC.L  $00000000

gs8:
   DS.B  9
   DC.B  $FF

__zp_init:
   SUBROUTINE

   LDA   #$FF
   STA   g3
   STA   z6
   LDA   #$7F
   STA   g3+1
   STA   z6+1
   LDA   #$35
   STA   z5
   RTS
