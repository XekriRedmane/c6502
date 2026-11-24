SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$27
z6	EQU	$29
g3	EQU	$2B
gp9	EQU	$2C
g2	EQU	$2E
g1	EQU	$30
gp10	EQU	$31
__zpabi_f26__p29_0	EQU	$80
__zpabi_f26__p29_1	EQU	$81
__zpabi_f26__pp30_0	EQU	$82
__zpabi_f26__pp30_1	EQU	$83
__zpabi_f19__p21	EQU	$84
__zpabi_f19__p22	EQU	$85
__zpabi_f19__pp23_0	EQU	$86
__zpabi_f19__pp23_1	EQU	$87
__zpabi_f11__p13_0	EQU	$88
__zpabi_f11__p13_1	EQU	$89
__zpabi_f11__p13_2	EQU	$8A
__zpabi_f11__p13_3	EQU	$8B
__zpabi_mix__v_0	EQU	$8C
__zpabi_mix__v_1	EQU	$8D
__zpabi_mix__v_2	EQU	$8E
__zpabi_mix__v_3	EQU	$8F
__local_f26__w34	EQU	$90
__local_main__1	EQU	$91
__local_f26__1	EQU	$92
__local_f26__p29	EQU	$93
__local_main__3	EQU	$93
__local_f26__p27	EQU	$94
__local_main__i38	EQU	$94
__local_f26__l31	EQU	$95
__local_main__l36	EQU	$95
__local_f19__0	EQU	$98
__local_f19__l24	EQU	$99
__local_f19__1	EQU	$9A
__local_f11__p13_3	EQU	$9D
__local_f11__p13_2	EQU	$9E
__local_f11__i17	EQU	$A1
__local_mix__0	EQU	$A4
__local_mix__1	EQU	$A5
__local_mix__2	EQU	$A6
__local_mix__3	EQU	$A7

; @zp-link-meta-begin
; def f11 params=__zpabi_f11__p12_0,__zpabi_f11__p12_1,__zpabi_f11__p13_0,__zpabi_f11__p13_1,__zpabi_f11__p13_2,__zpabi_f11__p13_3 locals=__local_f11__0,__local_f11__1,__local_f11__p13_3,__local_f11__p13_2,__local_f11__p13_1,__local_f11__p13_0,__local_f11__i17,__local_f11__2,__local_f11__3 indirect=false in_cycle=false param_regs=A,X,,,, changes=__zpabi_f11__p13_0
; def f19 params=__zpabi_f19__p20_0,__zpabi_f19__p20_1,__zpabi_f19__p21,__zpabi_f19__p22,__zpabi_f19__pp23_0,__zpabi_f19__pp23_1 locals=__local_f19__0,__local_f19__l24,__local_f19__1 indirect=false in_cycle=false param_regs=A,X,,,,
; def f26 params=__zpabi_f26__p27,__zpabi_f26__p28,__zpabi_f26__p29_0,__zpabi_f26__p29_1,__zpabi_f26__pp30_0,__zpabi_f26__pp30_1 locals=__local_f26__w34,__local_f26__0,__local_f26__1,__local_f26__p29,__local_f26__p27,__local_f26__l31 indirect=false in_cycle=false param_regs=A,X,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__i38,__local_main__l36,__local_main__5,__local_main__6 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f11 -> mix
; call f19 -> f11
; call f19 -> mix
; call f26 -> f19
; call f26 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f11
; call main -> f19
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run1@run5 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run7 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run8 section=.text local=true zero=false
; item fin1@run4 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run3 section=.text local=true zero=false
; item f11 section=.text local=false zero=false
; item f11@run6 section=.text local=true zero=false
; item f19 section=.text local=false zero=false
; item f26 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run9 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=1 at=auto def=true init=$00
; zp z5 size=2 at=auto def=true init=$FF,$7F
; zp z6 size=2 at=auto def=true init=$0D,$00
; zp g3 size=1 at=auto def=true init=$00 weight=29
; zp gp9 size=2 at=auto def=true init=<g3,>g3 weight=38
; zp g2 size=2 at=auto def=true init=$80,$00 weight=36
; zp g1 size=1 at=auto def=true init=$01 weight=10
; zp gp10 size=2 at=auto def=true init=<z6,>z6 weight=18
; runtime hargs_size=32 hargs_need=20
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
   JSR   fin0@run2
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run0
   LDA   g3
   JSR   fin0@run1
   LDA   z4
   JMP   fin0@tail0

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   JSR   fin0@run1@run5
   JMP   mix

fin0@run1@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   fin0@run2

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run7
   JMP   mix

fin0@run0@run7:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@tail0:
   SUBROUTINE

   JSR   fin0@run1@run5
   JMP   mix

fin1:
   SUBROUTINE

   JSR   fin1@run8
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run2
   JSR   mix
   LDA   z6+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run2
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   JSR   fin1@run4
   LDA   ga7
   STA   __zpabi_mix__v_0
   LDA   ga7+1
   JSR   fin0@run0
   LDA   ga7+2
   STA   __zpabi_mix__v_0
   LDA   ga7+3
   JSR   fin0@run0@run7
   JMP   mix

fin1@run8:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   RTS

fin1@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga7+4
   STA   __zpabi_mix__v_0
   LDA   ga7+5
   JSR   fin0@run0
   LDA   ga7+6
   STA   __zpabi_mix__v_0
   LDA   ga7+7
   JSR   fin0@run0
   LDA   gs8+2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run2
   JSR   fin2@run3
   JSR   fin1@run4
   LDA   gs8+3
   JMP   fin0@tail0

fin2@run3:
   SUBROUTINE

   LDA   gs8+1
   STA   __zpabi_mix__v_0
   LDA   gs8+2
   RTS

f11:
   SUBROUTINE

   LDA   g2
   EOR   #$03
   STA   __zpabi_mix__v_0
   STA   g2
   LDA   #$CC
   ORA   __zpabi_mix__v_0
   STA   __local_f11__p13_2
   LDA   #$C7
   ORA   g2+1
   ORA   __local_f11__p13_2
   ORA   #$FE
   ORA   #$75
   BNE   .lb_skip@0
   JMP   .if_else@1
.lb_skip@0:
   LDA   #$03
   STA   __local_f11__i17
.loop@0_start:
   LDA   __local_f11__i17
   BEQ   .loop@0_break
   JSR   fin2@run3
   STA   __zpabi_mix__v_1
   LDA   g2
   SEC
   SBC   __zpabi_mix__v_0
   LDA   g2+1
   SBC   __zpabi_mix__v_1
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_1
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run2
   LDA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   JSR   f11@run6
   LDA   __zpabi_f11__p13_0
   AND   #$05
   STA   __zpabi_f11__p13_0
   LDY   #$00
   ORA   #$00
   BNE   .loop@0_break
   JSR   fin1@run8
   LDA   __zpabi_mix__v_0
   SEC
   SBC   #$FF
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   SBC   #$FF
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@10
   TYA
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run2
   JSR   mix
   DEC   __local_f11__i17
   JMP   .loop@0_start
.loop@0_break:
   LDA   z6
   CLC
   ADC   #$03
   STA   __zpabi_mix__v_0
   LDA   z6+1
   ADC   #$00
   AND   #$3F
   EOR   #$20
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   STA   z6
   LDA   __zpabi_mix__v_1
   SEC
   SBC   #$20
   STA   z6+1
.if_else@1:
   LDY   #$00
   LDA   (gp10),Y
   STA   __local_f11__p13_2
   INY
   LDA   (gp10),Y
   STA   __local_f11__p13_3
   LDA   __local_f11__p13_2
   SEC
   SBC   #$01
   STA   __zpabi_mix__v_0
   LDA   __local_f11__p13_3
   SBC   #$00
   AND   #$3F
   EOR   #$20
   SEC
   SBC   #$20
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   DEY
   STA   (gp10),Y
   LDA   __zpabi_mix__v_1
   INY
   STA   (gp10),Y
   LDA   __local_f11__p13_2
   CLC
   ADC   #$FF
   STA   gs8+1
   LDA   __local_f11__p13_3
   ADC   #$7F
   STA   gs8+2
   LDA   z4
   JSR   fin0@run1
   LDY   #$00
   LDA   (gp9),Y
   LDX   #$FF
   SEC
.div_loop@12:
   INX
   SBC   #$06
   BCS   .div_loop@12
   ADC   #$06
   RTS

f11@run6:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   mix

f19:
   SUBROUTINE

   LDA   #$80
   STA   __local_f19__l24
   LDA   #$00
   STA   __local_f19__1
   LDA   z5
   LDX   z5+1
   STX   __local_f19__0
   EOR   #$5A
   STA   z5
   LDX   __local_f19__0
   STX   z5+1
   LDY   #$00
   STA   (gp9),Y
   LDA   (gp9),Y
   CLC
   ADC   #$FF
   STA   (gp9),Y
   JSR   fin0@run1
   INC   gs8+3
   LDA   gs8+1
   LDX   gs8+2
   STX   __local_f19__1
   SEC
   SBC   #$01
   STA   __local_f19__0
   BCS   .carry_inc@0
   DEC   __local_f19__1
.carry_inc@0:
   STA   gs8+1
   LDA   __local_f19__1
   STA   gs8+2
   LDA   __local_f19__0
   ORA   __zpabi_f19__p22
   STA   __local_f19__l24
   LDA   __local_f19__1
   STA   __local_f19__0
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   JSR   fin0@run2
   LDA   __local_f19__l24
   STA   __zpabi_mix__v_0
   LDA   __local_f19__0
   JSR   fin1@run4
   LDA   g2
   AND   #$03
   BEQ   .if_end@3
   LDA   z4
   AND   __zpabi_f19__p21
   STA   __local_f19__0
   LDA   #$00
   STA   __zpabi_f11__p13_0
   STA   __zpabi_f11__p13_1
   STA   __zpabi_f11__p13_2
   STA   __zpabi_f11__p13_3
   LDX   #$00
   LDA   __local_f19__0
   JSR   f11
   STA   __local_f19__0
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __zpabi_mix__v_1
   JSR   fin0@run2
   LDA   __local_f19__0
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   gs8+1
   LDX   gs8+2
   STX   __local_f19__0
   ASL
   LDA   __local_f19__0
   ROL
   STA   gs8+1
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   gs8+2
.if_end@3:
   LDA   __zpabi_f19__p21
   STA   __local_f19__l24
   LDA   z4
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __zpabi_f11__p13_1
   STA   __zpabi_f11__p13_2
   STA   __zpabi_f11__p13_3
   LDA   z4
   STA   __zpabi_f11__p13_0
   LDX   #$00
   LDA   __local_f19__l24
   JSR   f11
   STA   __local_f19__l24
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDX   __local_f19__l24
   STX   __local_f19__l24
   STA   __local_f19__1
   LDA   ga7+4
   ORA   ga7+5
   BEQ   .lnot_true@21
   LDA   #$00
   RTS
.lnot_true@21:
   LDA   #$01
   RTS

f26:
   SUBROUTINE

   STA   __local_f26__p27
   LDA   #$77
   STA   __local_f26__l31
   LDA   #$FF
   STA   __local_f26__w34
   LDA   #$00
   STA   __local_f26__p29
.f26@rot@0:
   SEC
   LDA   #$00
   SBC   __local_f26__p29
   STA   __local_f26__1
   LDA   #$00
   SBC   #$00
   TAX
   LDA   #$00
   CMP   __zpabi_f26__p29_1
   BNE   .cmp_end@24
   LDA   #$7F
   CMP   __zpabi_f26__p29_0
   BEQ   .cmp_true@23
   LDA   #$00
   BEQ   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   STA   __zpabi_f19__p21
   LDA   #$02
   CLC
   ADC   gs8+3
   STA   __zpabi_f19__p22
   LDA   #<__local_f26__l31
   STA   __zpabi_f19__pp23_0
   LDA   #>__local_f26__l31
   STA   __zpabi_f19__pp23_1
   LDA   __local_f26__1
   JSR   f19
   INC   __local_f26__p29
   LDA   __local_f26__p29
   CMP   #$04
   BCC   .f26@rot@0
   LDA   gs8+3
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   STA   __local_f26__w34
   LDA   #$FF
   SBC   __zpabi_mix__v_0
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   __local_f26__w34
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   gs8+3
   LDA   z4
   INC   z4
   CLC
   ADC   __local_f26__p27
   STA   __zpabi_mix__v_0
   LDA   #$00
   ADC   #$00
   ORA   __zpabi_mix__v_0
   BNE   .lb_skip@1
   JMP   .f26@asm_ssa_split@0
.lb_skip@1:
   LDA   __zpabi_f26__p29_0
   EOR   #$81
   STA   __local_f26__p29
   LDA   __zpabi_f26__p29_1
   STA   __local_f26__w34
   LDA   g2
   SEC
   SBC   __local_f26__p29
   STA   __zpabi_mix__v_0
   LDA   g2+1
   SBC   __local_f26__w34
   JSR   fin0@run0
.if_end@4:
   LDA   #$00
   STA   __local_f26__w34
   JMP   .loop@4_continue
.f26@rot@1:
   LDA   g1
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_f26__w34
   LDA   g2
   EOR   #$05
   STA   g2
   LDA   g1
   SEC
   SBC   g2
   STA   ga7+2
   LDA   __local_f26__w34
   SBC   g2+1
   STA   ga7+3
   LDA   __local_f26__1
   STA   __local_f26__w34
.loop@4_continue:
   CLC
   ADC   #$01
   STA   __local_f26__1
   LDA   __local_f26__w34
   CMP   #$04
   BCC   .f26@rot@1
   LDA   __local_f26__p27
   SBC   gs8+3
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   #$00
   STA   __local_f26__w34
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   JSR   fin0@run2
   LDA   __local_f26__w34
   JSR   fin1@run4
   LDA   __local_f26__p29
   AND   #$03
   EOR   #$03
   STA   z4
   LDX   #$00
   LDA   #$01
   RTS
.f26@asm_ssa_split@0:
   LDA   __zpabi_f26__p29_0
   STA   __local_f26__p29
   JMP   .if_end@4

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l36
   LDA   z6
   ORA   z6+1
   BEQ   .cond_else@5
   LDA   z5
   STA   __local_main__i38
   LDX   z5+1
   JMP   .vthread@2
.cond_else@5:
   LDA   z6
   STA   __local_main__i38
   LDX   z6+1
.vthread@2:
   LDA   g3
   CLC
   ADC   #$7F
   STA   __zpabi_f19__p21
   LDA   g3
   STA   __zpabi_f19__p22
   JSR   main@run9
   LDA   __local_main__i38
   JSR   f19
   JSR   fin2@run3
   STA   __local_main__1
   LDA   __zpabi_mix__v_0
   BNE   .carry_inc@2
   DEC   __local_main__1
.carry_inc@2:
   DEC   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_0
   STA   gs8+1
   LDA   __local_main__1
   STA   gs8+2
   LDA   #$00
   SEC
   SBC   __zpabi_mix__v_0
   AND   #$07
   CMP   #$01
   BEQ   .switch@5_break
   CMP   #$02
   BEQ   .switch@5_break
   CMP   #$05
   BEQ   .case@8
   CMP   #$06
   BEQ   .case@9
   BNE   .default@10
.case@8:
   LDA   #$01
   STA   __zpabi_f11__p13_0
   LDA   #$00
   STA   __zpabi_f11__p13_1
   STA   __zpabi_f11__p13_2
   STA   __zpabi_f11__p13_3
   LDX   #$00
   LDA   #$7F
   JSR   f11
   JMP   .switch@5_break
.case@9:
   LDA   g3
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   g3
   LDA   z4
   LSR
   LDA   #$00
   ROR
   STA   z4
   JMP   .switch@5_break
.default@10:
   LDA   #$F5
   STA   g1
   LDA   z4
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   z4
.switch@5_break:
   LDY   #$00
   LDA   (gp9),Y
   ORA   #$05
   STA   (gp9),Y
   AND   #$07
   CMP   #$04
   BEQ   .case@12
   CMP   #$06
   BNE   .lb_skip@4
   JMP   .case@14
.lb_skip@4:
   CMP   #$07
   BNE   .lb_skip@3
   JMP   .case@15
.lb_skip@3:
   JMP   .default@16
.case@12:
   LDA   #$00
   STA   __local_main__i38
.loop@13_start:
   LDA   __local_main__i38
   CMP   #$02
   BCC   .lb_skip@2
   JMP   .switch@11_break
.lb_skip@2:
   LDA   g3
   EOR   z4
   STA   __local_main__3
   LDA   __local_main__l36
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   STA   HARGS+2
   LDA   __zpabi_mix__v_0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+3
   LDY   #$7F
   STY   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   __local_main__i38
   BEQ   .lnot_true@37
   LDA   #$00
   BEQ   .lnot_end@38
.lnot_true@37:
   LDA   #$01
.lnot_end@38:
   STA   __zpabi_f19__p22
   LDA   HARGS+8
   STA   __zpabi_f19__p21
   JSR   main@run9
   LDX   #$00
   LDA   __local_main__3
   JSR   f19
   JSR   fin0@run1
   LDA   #$00
   BPL   .vthread@1
   LDA   #$FF
   BNE   .cmp_true@41
   LDA   #$00
   BNE   .cmp_true@41
.vthread@1:
   LDA   __local_main__i38
   CMP   #$7F
   BNE   .cmp_true@41
   LDA   #$00
   BEQ   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@45
   LDA   #$FF
.sx_done@45:
   JSR   fin0@run2
   JSR   f11@run6
   LDA   g3
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   g3
   AND   __local_main__i38
   STA   __local_main__1
   LDA   #$00
   EOR   #$20
   SEC
   SBC   #$20
   STA   __zpabi_mix__v_0
   LDA   __local_main__1
   LDY   #$00
   STA   (gp10),Y
   LDA   __zpabi_mix__v_0
   INY
   STA   (gp10),Y
   INC   __local_main__i38
   JMP   .loop@13_start
.case@14:
.case@15:
.default@16:
.switch@11_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run9:
   SUBROUTINE

   LDA   #<__local_main__l36
   STA   __zpabi_f19__pp23_0
   LDA   #>__local_main__l36
   STA   __zpabi_f19__pp23_1
   RTS

acc:
   DS.B  2

ga7:
   DS.B  4
   DC.W  $000C
   DC.W  $0003

gs8:
   DS.B  1
   DC.W  $FFFF
   DC.B  $FF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   g3
   STA   z4
   STA   z6+1
   LDA   #$01
   STA   g1
   LDA   #$80
   STA   g2
   LDA   #$FF
   STA   z5
   LDA   #$7F
   STA   z5+1
   LDA   #$0D
   STA   z6
   LDA   #<g3
   STA   gp9
   LDA   #>g3
   STA   gp9+1
   LDA   #<z6
   STA   gp10
   LDA   #>z6
   STA   gp10+1
   RTS
