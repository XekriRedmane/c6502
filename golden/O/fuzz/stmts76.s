SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
g2	EQU	$28
g1	EQU	$2A
g3	EQU	$2E
gp7	EQU	$30
__local_f27__0	EQU	$80
__zpabi_f18__pp21_0	EQU	$80
__local_f27__1	EQU	$81
__zpabi_f18__pp21_1	EQU	$81
__local_f27__pp28	EQU	$82
__zpabi_f9__p11_0	EQU	$82
__local_f27__2	EQU	$83
__zpabi_f9__p11_1	EQU	$83
__local_f27__3	EQU	$84
__zpabi_f9__p12	EQU	$84
__local_f27__4	EQU	$85
__zpabi_f9__pp13_0	EQU	$85
__local_f27__l32	EQU	$86
__zpabi_f9__pp13_1	EQU	$86
__zpabi_mix__v_0	EQU	$87
__zpabi_mix__v_1	EQU	$88
__zpabi_mix__v_2	EQU	$89
__zpabi_mix__v_3	EQU	$8A
__local_f18__0	EQU	$8B
__local_main__0	EQU	$8B
__local_main__1	EQU	$8C
__local_f18__2	EQU	$8D
__local_f18__p20	EQU	$8E
__local_main__5	EQU	$91
__local_main__6	EQU	$92
__local_f18__l25	EQU	$93
__local_f9__1	EQU	$95
__local_f9__2	EQU	$96
__local_f9__3	EQU	$97
__local_f9__4	EQU	$98
__local_f9__5	EQU	$99
__local_mix__0	EQU	$9B
__local_mix__1	EQU	$9C
__local_mix__2	EQU	$9D
__local_mix__3	EQU	$9E

; @zp-link-meta-begin
; def f18 params=__zpabi_f18__p19,__zpabi_f18__p20,__zpabi_f18__pp21_0,__zpabi_f18__pp21_1 locals=__local_f18__0,__local_f18__1,__local_f18__2,__local_f18__p20,__local_f18__3,__local_f18__4,__local_f18__5,__local_f18__6,__local_f18__l25 indirect=false in_cycle=false param_regs=A,X,,
; def f27 params=__zpabi_f27__pp28_0,__zpabi_f27__pp28_1 locals=__local_f27__0,__local_f27__1,__local_f27__pp28,__local_f27__2,__local_f27__3,__local_f27__4,__local_f27__l32 indirect=false in_cycle=false param_regs=A,X
; def f9 params=__zpabi_f9__p10_0,__zpabi_f9__p10_1,__zpabi_f9__p11_0,__zpabi_f9__p11_1,__zpabi_f9__p12,__zpabi_f9__pp13_0,__zpabi_f9__pp13_1 locals=__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__4,__local_f9__5,__local_f9__6 indirect=false in_cycle=false param_regs=A,X,,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__w40,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f18 -> f9
; call f18 -> mix
; call f27 -> mix
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
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run2 section=.text local=true zero=false
; item f9 section=.text local=false zero=false
; item f9@run4 section=.text local=true zero=false
; item f9@run3 section=.text local=true zero=false
; item f18 section=.text local=false zero=false
; item f27 section=.text local=false zero=false
; item f27@run5 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item @5.s17 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$00,$00
; zp g2 size=2 at=auto def=true init=$FF,$7F weight=130
; zp g1 size=4 at=auto def=true init=$FF,$FF,$00,$00 weight=142
; zp g3 size=2 at=auto def=true init=$FF,$FF weight=43
; zp gp7 size=2 at=auto def=true init=<z4,>z4 weight=7
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
   JSR   fin0@run1
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run0
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   JSR   fin0@run0
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   STA   __zpabi_mix__v_1
   LDA   ga5+2
   STA   __zpabi_mix__v_2
   LDA   ga5+3
   JSR   fin0@run1
   LDA   ga5+7
   STA   __zpabi_mix__v_0
   LDA   ga5+8
   STA   __zpabi_mix__v_1
   LDA   ga5+9
   STA   __zpabi_mix__v_2
   LDA   ga5+10
   JSR   fin0@run1
   LDA   ga5+14
   STA   __zpabi_mix__v_0
   LDA   ga5+15
   STA   __zpabi_mix__v_1
   LDA   ga5+16
   STA   __zpabi_mix__v_2
   LDA   ga5+17
   JSR   fin0@run1
   LDA   ga5+21
   STA   __zpabi_mix__v_0
   LDA   ga5+22
   STA   __zpabi_mix__v_1
   LDA   ga5+23
   STA   __zpabi_mix__v_2
   LDA   ga5+24
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   LDA   gs6+3
   STA   __zpabi_mix__v_2
   LDA   gs6+4
   JSR   fin0@run1
   JSR   fin2@run2
   JMP   fin0@tail0

fin2@run2:
   SUBROUTINE

   LDA   gs6+8
   STA   __zpabi_mix__v_0
   LDA   gs6+9
   STA   __zpabi_mix__v_1
   LDA   gs6+10
   STA   __zpabi_mix__v_2
   LDA   gs6+11
   RTS

f9:
   SUBROUTINE

   LDA   __zpabi_f9__p12
   AND   #$5A
   STA   __zpabi_mix__v_1
   LDA   #$03
   AND   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   JSR   f9@run3
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f9__2
   INY
   LDA   (DPTR),Y
   STA   __local_f9__3
   LDX   #$05
.shift_loop@0:
   ASL   __local_f9__2
   ROL   __local_f9__3
   DEX
   BNE   .shift_loop@0
   LDA   __local_f9__2
   LDY   #$00
   STA   (DPTR),Y
   JSR   f9@run3
   LDA   __local_f9__3
   LDY   #$01
   STA   (DPTR),Y
   DEY
   LDA   (gp7),Y
   CLC
   SBC   g2
   INY
   LDA   (gp7),Y
   SBC   g2+1
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_1
   LDA   __local_f9__2
   EOR   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   LDA   __local_f9__3
   JSR   fin0@run0
   LDA   #$FF
   SEC
   SBC   __zpabi_f9__p11_0
   AND   #$5A
   BNE   .lb_skip@0
   JMP   .if_end@2
.lb_skip@0:
   LDA   g1
   STA   __local_f9__5
   LDA   g1+1
   STA   __local_f9__4
   LDA   g1+2
   STA   __local_f9__3
   LDA   g1+3
   STA   __local_f9__2
   LDY   #$01
   LDA   (gp7),Y
   STA   __zpabi_mix__v_1
   DEY
   LDA   (gp7),Y
   ORA   #$05
   STA   __local_f9__1
   STA   (gp7),Y
   LDA   __local_f9__5
   CLC
   ADC   __local_f9__1
   STA   __local_f9__1
   LDA   __local_f9__4
   ADC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   BCC   .carry_inc@0
   INC   __local_f9__3
   BNE   .carry_inc@0
   INC   __local_f9__2
.carry_inc@0:
   LDA   __local_f9__1
   ORA   __zpabi_mix__v_1
   ORA   __local_f9__3
   ORA   __local_f9__2
   BEQ   .if_end@3
   JSR   f9@run3
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f9__1
   INY
   LDA   (DPTR),Y
   STA   __zpabi_mix__v_1
   LDA   __local_f9__1
   ORA   #$5A
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   JSR   f9@run4
   JSR   f9@run4
.if_end@3:
.if_end@2:
   LDA   gs6+8
   SEC
   SBC   #$5A
   STA   HARGS+16
   LDA   gs6+9
   SBC   #$00
   STA   HARGS+17
   LDA   gs6+10
   SBC   #$00
   STA   HARGS+18
   LDA   gs6+11
   SBC   #$00
   STA   HARGS+19
   LDA   #$00
   STA   HARGS+20
   LDY   #$00
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

f9@run4:
   SUBROUTINE

   LSR   @5.s17+2
   ROR   @5.s17+1
   ROR   @5.s17
   RTS

f9@run3:
   SUBROUTINE

   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   RTS

f18:
   SUBROUTINE

   STX   __local_f18__p20
   LDA   #$03
   STA   __local_f18__l25
   LDY   #$00
   LDA   (__zpabi_f18__pp21_0),Y
   STA   __zpabi_f9__p11_0
   INY
   LDA   (__zpabi_f18__pp21_0),Y
   STA   __zpabi_f9__p11_1
   LDA   ga5+21
   STA   __zpabi_f9__p12
   LDA   #<__local_f18__l25
   STA   __zpabi_f9__pp13_0
   LDA   #>__local_f18__l25
   STA   __zpabi_f9__pp13_1
   LDX   #$01
   LDA   #$00
   JSR   f9
   LDA   HARGS+16
   STA   __local_f18__l25
   JSR   fin2@run2
   JSR   fin0@run1
   LDA   gs6+1
   ORA   gs6+2
   ORA   gs6+3
   ORA   gs6+4
   ORA   gs6+5
   ORA   gs6+6
   ORA   gs6+7
   BEQ   .cond_else@8
   LDA   __local_f18__p20
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f18__2
   JMP   .cond_end@9
.cond_else@8:
   LDA   #$00
   STA   __local_f18__p20
   STA   __local_f18__2
.cond_end@9:
   LDA   HARGS+16
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f18__0
   LDA   __local_f18__p20
   SEC
   SBC   HARGS+16
   STA   __zpabi_mix__v_0
   LDA   __local_f18__2
   SBC   __local_f18__0
   JSR   fin0@run0
   LDA   g1
   STA   __local_f18__0
   LDX   g1+1
   LDA   g1+2
   LDA   g1+3
   LDA   __local_f18__0
   RTS

f27:
   SUBROUTINE

   STA   __local_f27__pp28
   STX   __local_f27__2
   LDA   #$81
   STA   __local_f27__l32
   LDA   #$FF
   EOR   z4
   STA   __local_f27__1
   LDA   #$7F
   EOR   z4+1
   LDY   #$01
   STA   (__local_f27__pp28),Y
   LDA   __local_f27__1
   DEY
   STA   (__local_f27__pp28),Y
   LDA   gs6+8
   AND   #$81
   STA   __local_f27__2
   LDA   #$00
   JSR   f27@run5
   STY   gs6+11
   STY   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   LDA   __local_f27__2
   STA   gs6+8
   STY   __zpabi_mix__v_3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g3
   EOR   #$FF
   STA   g3
   LDA   g2
   CLC
   ADC   #$05
   STA   g2
   BCC   .carry_inc@1
   INC   g2+1
.carry_inc@1:
   LDA   g1
   STA   __local_f27__4
   LDA   g1+1
   STA   __local_f27__3
   LDA   g1+2
   STA   __local_f27__2
   LDA   g1+3
   STA   __local_f27__pp28
   LDA   __local_f27__4
   SEC
   SBC   #$01
   STA   __local_f27__1
   LDA   __local_f27__3
   SBC   #$00
   STA   __local_f27__0
   LDA   __local_f27__2
   SBC   #$00
   STA   __local_f27__2
   BCS   .carry_inc@2
   DEC   __local_f27__pp28
.carry_inc@2:
   LDA   __local_f27__1
   STA   g1
   LDA   __local_f27__0
   STA   g1+1
   LDA   __local_f27__2
   STA   g1+2
   LDA   __local_f27__pp28
   STA   g1+3
   LDA   __local_f27__l32
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f27__1
   LDA   __local_f27__4
   CLC
   ADC   __local_f27__l32
   STA   __local_f27__0
   LDA   __local_f27__3
   ADC   __local_f27__1
   TAX
   LDA   __local_f27__0
   RTS

f27@run5:
   SUBROUTINE

   LDY   #$00
   STY   gs6+9
   STY   gs6+10
   RTS

main:
   SUBROUTINE

   LDY   #$00
   LDA   (gp7),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (gp7),Y
   JSR   fin0@run0
   LDY   #$01
   LDA   (gp7),Y
   DEY
   ORA   (gp7),Y
   BNE   .lb_skip@1
   JMP   .main@asm_ssa_split@0
.lb_skip@1:
   LDA   #$00
   STA   __zpabi_mix__v_2
   CLC
.loop@2_start:
   ADC   g3
   STA   __local_main__1
   LDA   #$00
   ADC   g3+1
   BNE   .thread@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .cmp_true@8
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   STA   __zpabi_mix__v_3
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   LDX   __zpabi_mix__v_3
   STX   g1
   LDX   #$00
   STX   g1+1
   STA   g1+2
   STA   g1+3
   LDA   g3
   EOR   #$03
   STA   g3
   LDA   #$00
   STA   gs6+8
   JSR   f27@run5
   STA   gs6+11
   INC   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_2
   CMP   #$04
   BCC   .loop@2_start
   LDA   g1
   LDX   g1+1
   STX   __zpabi_mix__v_2
   LDX   g1+2
   STX   __zpabi_mix__v_3
   LDX   g1+3
   STX   __local_main__1
   SBC   #$01
   STA   __local_main__0
   LDA   __zpabi_mix__v_2
   SBC   #$00
   STA   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_3
   SBC   #$00
   STA   __zpabi_mix__v_3
   BCS   .carry_inc@3
   DEC   __local_main__1
.carry_inc@3:
   LDA   __local_main__0
   STA   g1
   LDA   __zpabi_mix__v_2
   STA   g1+1
   LDA   __zpabi_mix__v_3
   STA   g1+2
   LDA   __local_main__1
   STA   g1+3
   LDA   #$47
   STA   __zpabi_mix__v_0
   LDA   #$A1
   STA   __zpabi_mix__v_1
   LDA   #$F1
   STA   __zpabi_mix__v_2
   LDA   #$42
   STA   __zpabi_mix__v_3
.if_end@10:
   LDY   #$00
   LDA   (gp7),Y
   STA   __local_main__0
   INY
   LDA   (gp7),Y
   STA   __local_main__1
   LDA   __local_main__0
   BNE   .carry_inc@4
   DEC   __local_main__1
.carry_inc@4:
   DEC   __local_main__0
   LDA   __local_main__0
   LDY   #$00
   STA   (gp7),Y
   LDA   __local_main__1
   INY
   STA   (gp7),Y
   JSR   mix
   LDA   #$06
   STA   ga5+21
   LDA   #$00
   STA   ga5+22
   LDY   #$00
   STY   ga5+23
   STY   ga5+24
   STY   ga5+25
   STY   ga5+26
   STA   ga5+27
   LDA   gs6+8
   STA   __zpabi_mix__v_2
   LDA   gs6+9
   STA   __zpabi_mix__v_3
   LDA   gs6+10
   STA   __local_main__6
   LDA   gs6+11
   STA   __local_main__5
   LSR
   LDA   __local_main__6
   ROR
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_3
   ROR
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_2
   ROR
   STA   __local_main__1
   TYA
   ROR
   CLC
   ADC   __zpabi_mix__v_2
   STA   __zpabi_mix__v_2
   LDA   __local_main__1
   ADC   __zpabi_mix__v_3
   STA   __zpabi_mix__v_3
   LDA   __zpabi_mix__v_1
   ADC   __local_main__6
   STA   __local_main__1
   LDA   __zpabi_mix__v_0
   ADC   __local_main__5
   LDX   __zpabi_mix__v_2
   STX   gs6+8
   STX   gs6+1
   LDX   __zpabi_mix__v_3
   STX   gs6+9
   STX   gs6+2
   LDX   __local_main__1
   STX   gs6+10
   STX   gs6+3
   STA   gs6+11
   STA   gs6+4
   TYA
   STA   gs6+5
   STY   gs6+6
   STA   gs6+7
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS
.main@asm_ssa_split@0:
   LDA   #$A7
   STA   __zpabi_mix__v_0
   LDA   #$86
   STA   __zpabi_mix__v_1
   LDA   #$63
   STA   __zpabi_mix__v_2
   LDA   #$0D
   STA   __zpabi_mix__v_3
   JMP   .if_end@10

acc:
   DS.B  2

ga5:
   DC.B  $FF
   DC.B  $FF
   DC.B  $FF
   DC.B  $7F
   DS.B  3
   DC.B  $02
   DS.B  9
   DC.B  $80
   DS.B  3
   DC.B  $FF
   DC.B  $FF
   DC.B  $FF
   DC.B  $7F
   DS.B  3

gs6:
   DS.B  3
   DC.B  $01
   DS.B  4
   DC.L  $7FFFFFFF

gp8:
   DC.W  g3

@5.s17:
   DC.B  $FF
   DC.B  $7F
   DS.B  1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+2
   STA   g1+3
   STA   z4
   STA   z4+1
   LDA   #$FF
   STA   g1
   STA   g1+1
   STA   g2
   STA   g3
   STA   g3+1
   LDA   #$7F
   STA   g2+1
   LDA   #<z4
   STA   gp7
   LDA   #>z4
   STA   gp7+1
   RTS
