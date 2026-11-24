SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$27
g3	EQU	$2B
g1	EQU	$2D
g2	EQU	$2E
__zpabi_f16__pp19_0	EQU	$80
__zpabi_f16__pp19_1	EQU	$81
__zpabi_f9__p11_0	EQU	$82
__zpabi_f9__p11_1	EQU	$83
__zpabi_f9__p11_2	EQU	$84
__zpabi_f9__p11_3	EQU	$85
__zpabi_f9__p11_4	EQU	$86
__zpabi_f9__p11_5	EQU	$87
__zpabi_f9__p11_6	EQU	$88
__zpabi_f9__p11_7	EQU	$89
__zpabi_f9__pp12_0	EQU	$8A
__zpabi_f9__pp12_1	EQU	$8B
__zpabi_mix__v_0	EQU	$8C
__zpabi_mix__v_1	EQU	$8D
__zpabi_mix__v_2	EQU	$8E
__zpabi_mix__v_3	EQU	$8F
__local_f16__0	EQU	$90
__local_f16__1	EQU	$91
__local_main__l24_0	EQU	$91
__local_f16__2	EQU	$92
__local_main__0	EQU	$92
__local_f16__3	EQU	$93
__local_main__1	EQU	$93
__local_f16__p17	EQU	$94
__local_main__2	EQU	$94
__local_f16__l21	EQU	$95
__local_main__3	EQU	$95
__local_main__4	EQU	$96
__local_main__l25	EQU	$A0
__local_mix__0	EQU	$AB
__local_mix__1	EQU	$AC
__local_mix__2	EQU	$AD
__local_mix__3	EQU	$AE

; @zp-link-meta-begin
; def f16 params=__zpabi_f16__p17,__zpabi_f16__p18,__zpabi_f16__pp19_0,__zpabi_f16__pp19_1 locals=__local_f16__0,__local_f16__1,__local_f16__2,__local_f16__3,__local_f16__p17,__local_f16__l21 indirect=false in_cycle=false param_regs=A,X,,
; def f9 params=__zpabi_f9__p10_0,__zpabi_f9__p10_1,__zpabi_f9__p11_0,__zpabi_f9__p11_1,__zpabi_f9__p11_2,__zpabi_f9__p11_3,__zpabi_f9__p11_4,__zpabi_f9__p11_5,__zpabi_f9__p11_6,__zpabi_f9__p11_7,__zpabi_f9__pp12_0,__zpabi_f9__pp12_1 locals=__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__4,__local_f9__5,__local_f9__6,__local_f9__7 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__l24_1,__local_main__l24_0,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__l25,__local_main__15,__local_main__16 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f16 -> f9
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
; item fin0@run4 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f16 section=.text local=false zero=false
; item f16@run3 section=.text local=true zero=false
; item f16@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run5 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g4 section=.data local=false zero=false
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=1 at=auto def=true init=$03
; zp z6 size=4 at=auto def=true init=$FF,$00,$00,$00
; zp g3 size=2 at=auto def=true init=$0D,$00 weight=30
; zp g1 size=1 at=auto def=true init=$04 weight=14
; zp g2 size=1 at=auto def=true init=$00 weight=10
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
   JSR   fin0@run2
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
   JSR   fin0@run4
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   JMP   fin0@tail0

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run2:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@run1

fin0@run1:
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
   JSR   fin0@run2
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   STA   __zpabi_mix__v_1
   LDA   z6+2
   STA   __zpabi_mix__v_2
   LDA   z6+3
   JSR   fin0@run1
   LDA   ga7
   STA   __zpabi_mix__v_0
   LDA   ga7+1
   STA   __zpabi_mix__v_1
   LDA   ga7+2
   STA   __zpabi_mix__v_2
   LDA   ga7+3
   JSR   fin0@run1
   LDA   ga7+8
   STA   __zpabi_mix__v_0
   LDA   ga7+9
   STA   __zpabi_mix__v_1
   LDA   ga7+10
   STA   __zpabi_mix__v_2
   LDA   ga7+11
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga7+16
   STA   __zpabi_mix__v_0
   LDA   ga7+17
   STA   __zpabi_mix__v_1
   LDA   ga7+18
   STA   __zpabi_mix__v_2
   LDA   ga7+19
   JSR   fin0@run1
   LDA   ga7+24
   STA   __zpabi_mix__v_0
   LDA   ga7+25
   STA   __zpabi_mix__v_1
   LDA   ga7+26
   STA   __zpabi_mix__v_2
   LDA   ga7+27
   JSR   fin0@run1
   LDA   gs8+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs8+1
   JSR   fin0@run4
   LDA   gs8+2
   STA   __zpabi_mix__v_0
   LDA   gs8+3
   JMP   fin0@tail0

f9:
   SUBROUTINE

   LDA   z5
   AND   #$01
   STA   z5
   LDA   ga7
   LDX   ga7+1
   STX   __zpabi_mix__v_1
   LDX   ga7+2
   STX   __zpabi_mix__v_2
   LDX   ga7+3
   STX   __zpabi_mix__v_3
   EOR   #$03
   STA   ga7
   JSR   fin0@run4
   LDA   #$02
   STA   __zpabi_mix__v_0
   JSR   fin0@run2
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS

f16:
   SUBROUTINE

   STA   __local_f16__p17
   LDA   #$00
   STA   __local_f16__l21
   SEC
   SBC   g1
   STA   ga7+8
   LDA   #$00
   SBC   #$00
   STA   ga7+9
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   ga7+10
   STA   ga7+11
   STA   ga7+12
   STA   ga7+13
   STA   ga7+14
   STA   ga7+15
   LDA   #$80
   CLC
   ADC   z5
   STA   __local_f16__2
   LDA   #$00
   ADC   #$00
   TAX
   LDA   #$FF
   STA   __zpabi_f9__p11_0
   STA   __zpabi_f9__p11_1
   JSR   f16@run3
   LDA   __local_f16__2
   JSR   f9
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_f16__0
   LDA   #$00
   CLC
   ADC   #$FF
   STA   __local_f16__1
   LDA   __local_f16__0
   ADC   #$7F
   STA   z6+1
   LDA   __local_f16__1
   STA   z6
   LDA   #$00
   STA   z6+2
   STA   z6+3
   LDA   g1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f16__2
   LDA   gs8+1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f16__0
   LDA   g1
   EOR   gs8+1
   STA   ga7
   LDA   __local_f16__2
   EOR   __local_f16__0
   STA   ga7+1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   ga7+2
   STA   ga7+3
   STA   ga7+4
   STA   ga7+5
   STA   ga7+6
   STA   ga7+7
   LDA   #$81
   STA   __local_f16__l21
   AND   #$07
   CMP   #$03
   BNE   .lb_skip@0
   JMP   .switch@0_break
.lb_skip@0:
   CMP   #$05
   BEQ   .case@2
   CMP   #$06
   BEQ   .case@3
.case@2:
.case@3:
   LDA   g1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f16__2
   LDY   #$00
   LDA   (__zpabi_f16__pp19_0),Y
   STA   __local_f16__1
   INY
   LDA   (__zpabi_f16__pp19_0),Y
   STA   __local_f16__0
   LDA   g1
   ORA   __local_f16__1
   STA   __local_f16__1
   LDA   __local_f16__2
   ORA   __local_f16__0
   ORA   __local_f16__1
   BEQ   .if_end@1
   LDA   g2
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f16__0
   LDA   #$7F
   CLC
   ADC   g2
   STA   __local_f16__3
   LDA   #$00
   ADC   __local_f16__0
   TAX
   LDA   g1
   STA   __zpabi_f9__p11_0
   LDA   #$00
   STA   __zpabi_f9__p11_1
   JSR   f16@run3
   LDA   __local_f16__3
   JSR   f9
   INC   g3
   BNE   .inc_done@0
   INC   g3+1
.inc_done@0:
.if_end@1:
.switch@0_break:
   LDA   __local_f16__p17
   RTS

f16@run3:
   SUBROUTINE

   JSR   f16@run0
   LDA   #<__local_f16__l21
   STA   __zpabi_f9__pp12_0
   LDA   #>__local_f16__l21
   STA   __zpabi_f9__pp12_1
   RTS

f16@run0:
   SUBROUTINE

   STA   __zpabi_f9__p11_2
   STA   __zpabi_f9__p11_3
   STA   __zpabi_f9__p11_4
   STA   __zpabi_f9__p11_5
   STA   __zpabi_f9__p11_6
   STA   __zpabi_f9__p11_7
   RTS

main:
   SUBROUTINE

   LDA   #$0E
   STA   __local_main__l25
   LDA   z6
   STA   HARGS
   LDA   z6+1
   STA   HARGS+1
   LDA   z6+2
   STA   HARGS+2
   LDA   z6+3
   STA   HARGS+3
   LDA   #$05
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   z6
   LDA   HARGS+6
   STA   z6+1
   LDA   HARGS+7
   STA   z6+2
   LDA   HARGS+8
   STA   z6+3
   LDA   g3
   CLC
   ADC   #$03
   STA   g3
   BCC   .carry_inc@0
   INC   g3+1
.carry_inc@0:
   LDA   g1
   CLC
   ADC   #$03
   STA   g1
   ORA   g4
   ORA   g4+1
   BNE   .lb_skip@3
   JMP   .main@asm_ssa_split@0
.lb_skip@3:
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
   LDA   ga7+16
   STA   HARGS+8
   LDA   ga7+17
   STA   HARGS+9
   LDA   ga7+18
   STA   HARGS+10
   LDA   ga7+19
   STA   HARGS+11
   LDA   ga7+20
   STA   HARGS+12
   LDA   ga7+21
   STA   HARGS+13
   LDA   ga7+22
   STA   HARGS+14
   LDA   ga7+23
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   ORA   HARGS+21
   ORA   HARGS+22
   ORA   HARGS+23
   BNE   .lb_skip@2
   JMP   .main@asm_ssa_split@1
.lb_skip@2:
   LDA   z5
   LSR
   LDA   #$00
   ROR
   STA   z5
   STA   g3
   LDA   #$FC
   STA   __local_main__l24_0
   LDA   #$00
   STA   g3+1
.if_end@3:
   LDA   gs8+1
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_main__0
   LDA   gs8+1
   SEC
   SBC   g3
   LDA   __local_main__0
   SBC   g3+1
   LDA   #$00
   ROL
   STA   ga7+24
   LDA   #$00
   STA   ga7+25
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   ga7+26
   STA   ga7+27
   STA   ga7+28
   STA   ga7+29
   STA   ga7+30
   STA   ga7+31
   LDA   g3+1
   CLC
   ADC   #$02
   ORA   g3
   BNE   .lb_skip@1
   JMP   .if_end@4
.lb_skip@1:
   LDA   gs8+2
   ORA   gs8+3
   BEQ   .cond_else@5
   LDA   g2
   STA   __local_main__2
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   TAX
   JMP   .cond_end@6
.cond_else@5:
   LDA   gs8+1
   STA   __local_main__2
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   TAX
.cond_end@6:
   LDA   #$01
   STA   __zpabi_f9__p11_0
   LDA   #$00
   STA   __zpabi_f9__p11_1
   JSR   f16@run0
   JSR   main@run5
   LDA   __local_main__2
   JSR   f9
   LDA   ga7+16
   LDX   ga7+17
   STX   __zpabi_mix__v_1
   LDX   ga7+18
   STX   __zpabi_mix__v_2
   LDX   ga7+19
   STX   __zpabi_mix__v_3
   LDX   ga7+20
   STX   __local_main__4
   LDX   ga7+21
   STX   __local_main__3
   LDX   ga7+22
   STX   __local_main__2
   LDX   ga7+23
   STX   __local_main__1
   SEC
   SBC   #$01
   STA   __local_main__0
   LDA   __zpabi_mix__v_1
   SBC   #$00
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_2
   SBC   #$00
   STA   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_3
   SBC   #$00
   STA   __zpabi_mix__v_3
   LDA   __local_main__4
   SBC   #$00
   STA   ga7+20
   LDA   __local_main__3
   SBC   #$00
   STA   ga7+21
   LDA   __local_main__2
   SBC   #$00
   STA   ga7+22
   LDA   __local_main__1
   SBC   #$00
   STA   ga7+23
   LDA   __local_main__0
   STA   ga7+16
   LDA   __zpabi_mix__v_1
   STA   ga7+17
   LDA   __zpabi_mix__v_2
   STA   ga7+18
   LDA   __zpabi_mix__v_3
   STA   ga7+19
   LDA   __local_main__0
   JSR   fin0@run4
.if_end@4:
   LDA   #$FF
.vthread@0:
   AND   __local_main__l24_0
   STA   __local_main__1
   LDX   #$00
   LDA   g4
   SEC
   SBC   __local_main__l24_0
   LDA   g4+1
   SBC   #$00
   TXA
   ROL
   STA   __local_main__0
   TXA
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   JSR   f16@run0
   LDA   __local_main__0
   STA   __zpabi_f9__p11_0
   LDA   #$00
   STA   __zpabi_f9__p11_1
   JSR   main@run5
   LDA   __local_main__1
   JSR   f9
   LDA   HARGS+8
   STA   __zpabi_mix__v_0
   LDA   HARGS+9
   STA   __zpabi_mix__v_1
   LDA   HARGS+10
   STA   __zpabi_mix__v_2
   LDA   HARGS+11
   JSR   fin0@run1
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS
.main@asm_ssa_split@0:
   LDA   #$FF
   STA   __local_main__l24_0
   JMP   .vthread@0
.main@asm_ssa_split@1:
   LDA   #$FF
   STA   __local_main__l24_0
   JMP   .if_end@3

main@run5:
   SUBROUTINE

   LDA   #<__local_main__l25
   STA   __zpabi_f9__pp12_0
   LDA   #>__local_main__l25
   STA   __zpabi_f9__pp12_1
   RTS

acc:
   DS.B  2

g4:
   DC.W  $00FF

ga7:
   DC.L  $00000100
   DC.L  $00000000
   DC.L  $0000007F
   DC.L  $00000000
   DC.L  $00000003
   DC.L  $00000000
   DC.L  $00000100
   DC.L  $00000000

gs8:
   DS.B  1
   DC.B  $02
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   STA   g3+1
   STA   z6+1
   STA   z6+2
   STA   z6+3
   LDA   #$04
   STA   g1
   LDA   #$0D
   STA   g3
   LDA   #$03
   STA   z5
   LDA   #$FF
   STA   z6
   RTS
