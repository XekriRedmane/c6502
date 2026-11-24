SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g2	EQU	$28
g3	EQU	$2A
__zpabi_f19__p21_0	EQU	$80
__zpabi_mix__v_0	EQU	$80
__zpabi_f19__p21_1	EQU	$81
__zpabi_mix__v_1	EQU	$81
__zpabi_f19__p21_2	EQU	$82
__zpabi_mix__v_2	EQU	$82
__zpabi_f19__p21_3	EQU	$83
__zpabi_mix__v_3	EQU	$83
__local_f12__w18	EQU	$84
__local_main__l29	EQU	$84
__local_main__0	EQU	$85
__local_f12__1	EQU	$86
__local_main__1	EQU	$86
__local_f12__2	EQU	$87
__local_main__2	EQU	$87
__local_f12__3	EQU	$88
__local_f19__w27	EQU	$88
__local_f12__4	EQU	$89
__local_f19__0	EQU	$89
__local_f12__5	EQU	$8A
__local_f19__l23	EQU	$8A
__local_f12__6	EQU	$8B
__local_f19__1	EQU	$8B
__local_f12__7	EQU	$8C
__local_f19__2	EQU	$8C
__local_f12__i16	EQU	$8D
__local_f19__p20	EQU	$8D
__local_f12__l15	EQU	$8E
__local_f7__l9	EQU	$8F
__local_f7__0	EQU	$90
__local_mix__0	EQU	$91
__local_mix__1	EQU	$92
__local_mix__2	EQU	$93
__local_mix__3	EQU	$94

; @zp-link-meta-begin
; def f12 params=__zpabi_f12__pp13_0,__zpabi_f12__pp13_1 locals=__local_f12__w18,__local_f12__0,__local_f12__1,__local_f12__2,__local_f12__3,__local_f12__4,__local_f12__5,__local_f12__6,__local_f12__7,__local_f12__i16,__local_f12__l15 indirect=false in_cycle=false param_regs=A,X
; def f19 params=__zpabi_f19__p20_0,__zpabi_f19__p20_1,__zpabi_f19__p21_0,__zpabi_f19__p21_1,__zpabi_f19__p21_2,__zpabi_f19__p21_3 locals=__local_f19__w27,__local_f19__0,__local_f19__l23,__local_f19__1,__local_f19__2,__local_f19__p20 indirect=false in_cycle=false param_regs=A,X,,,,
; def f7 params= locals=__local_f7__l9,__local_f7__0 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__l29,__local_main__0,__local_main__1,__local_main__2 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f12 -> f7
; call f12 -> mix
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f19
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run1 section=.text local=true zero=false
; item f7 section=.text local=false zero=false
; item f7@run3 section=.text local=true zero=false
; item f12 section=.text local=false zero=false
; item f19 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g4 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item @13.s24 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$01,$00 weight=195
; zp g2 size=2 at=auto def=true init=$00,$00 weight=194
; zp g3 size=1 at=auto def=true init=$00 weight=20
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
   JSR   fin0@run2
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
   LDA   g3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   g4+2
   STA   __zpabi_mix__v_2
   LDA   g4+3
   JMP   fin0@tail0

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   fin0@run0

fin0@run0:
   SUBROUTINE

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
   JSR   fin0@run2
   LDA   ga5+2
   STA   __zpabi_mix__v_0
   LDA   ga5+3
   JSR   fin0@run2
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   JSR   fin0@run2
   LDA   ga5+6
   STA   __zpabi_mix__v_0
   LDA   ga5+7
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
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
   JSR   fin2@run1
   LDA   gs6+5
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin2@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

f7:
   SUBROUTINE

   LDA   #$01
   STA   __local_f7__l9
   LDA   #$00
   STA   __local_f7__0
   LDA   #$03
   STA   __zpabi_mix__v_0
   JSR   f7@run3
   LDA   #$FF
   STA   g4
   LDA   #$00
   STA   __zpabi_mix__v_0
   AND   g4+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   AND   g4+2
   STA   __zpabi_mix__v_2
   LDA   #$00
   AND   g4+3
   JSR   fin2@run1
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin2@run1
   LDA   gs6+5
   BEQ   .cmp_end@4
   LDA   #$01
.cmp_end@4:
   RTS

f7@run3:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin2@run1

f12:
   SUBROUTINE

   LDA   #$00
   STA   __local_f12__l15
   LDA   gs6+1
   ORA   #$5A
   STA   gs6+1
   LDA   #$04
   STA   __local_f12__i16
.loop@0_start:
   LDA   __local_f12__i16
   BNE   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   #$00
   STA   __local_f12__w18
.loop@1_continue:
   CLC
   ADC   #$01
   STA   __local_f12__7
   LDA   __local_f12__w18
   CMP   #$03
   BCC   .lb_skip@0
   JMP   .loop@1_break
.lb_skip@0:
   LDA   #$FF
   ORA   __local_f12__l15
   AND   #$07
   CMP   #$05
   BNE   .default@4
   INC   g1
   BNE   .switch@2_break
   INC   g1+1
   JMP   .switch@2_break
.default@4:
   SEC
   LDA   #$00
   SBC   g2
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   g2+1
   STA   __local_f12__w18
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f12__w18
   STA   __zpabi_mix__v_1
   JSR   mix
.switch@2_break:
   INC   g2
   BNE   .inc_done@1
   INC   g2+1
.inc_done@1:
   LDA   gs6+1
   LDX   gs6+2
   STX   __local_f12__6
   LDX   gs6+3
   STX   __local_f12__5
   LDX   gs6+4
   STX   __local_f12__4
   CLC
   ADC   #$FF
   STA   __local_f12__w18
   BCC   .carry_inc@0
   INC   __local_f12__6
   BNE   .carry_inc@0
   INC   __local_f12__5
   BNE   .carry_inc@0
   INC   __local_f12__4
.carry_inc@0:
   STA   gs6+1
   LDA   __local_f12__6
   STA   gs6+2
   LDA   __local_f12__5
   STA   gs6+3
   LDA   __local_f12__4
   STA   gs6+4
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_f12__3
   STA   __local_f12__2
   STA   __local_f12__1
   STA   __zpabi_mix__v_0
   LDA   __local_f12__w18
   CLC
   BCC   .carry_inc@1
   INC   __local_f12__6
   BNE   .carry_inc@1
   INC   __local_f12__5
   BNE   .carry_inc@1
   INC   __local_f12__4
   BNE   .carry_inc@1
   INC   __local_f12__3
   BNE   .carry_inc@1
   INC   __local_f12__2
   BNE   .carry_inc@1
   INC   __local_f12__1
   BNE   .carry_inc@1
   INC   __zpabi_mix__v_0
.carry_inc@1:
   ORA   __local_f12__6
   ORA   __local_f12__5
   ORA   __local_f12__4
   ORA   __local_f12__3
   ORA   __local_f12__2
   ORA   __local_f12__1
   ORA   __zpabi_mix__v_0
   BNE   .loop@1_break
   LDA   __local_f12__7
   STA   __local_f12__w18
   JMP   .loop@1_continue
.loop@1_break:
   DEC   __local_f12__i16
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   STA   __local_f12__w18
.loop@5_start:
   JSR   f7
   INC   __local_f12__w18
   BEQ   .loop@5_start
   LDA   g1
   ORA   ga5
   RTS

f19:
   SUBROUTINE

   STA   __local_f19__p20
   LDA   gs6+1
   STA   __local_f19__2
   LDA   gs6+2
   STA   __local_f19__1
   LDA   gs6+3
   STA   __local_f19__l23
   LDA   gs6+4
   STA   __local_f19__0
   LDA   #$00
   SEC
   SBC   __local_f19__2
   STA   __local_f19__w27
   LDA   __local_f19__2
   SBC   __local_f19__1
   STA   gs6+2
   LDA   __local_f19__1
   SBC   __local_f19__l23
   STA   gs6+3
   LDA   __local_f19__l23
   SBC   __local_f19__0
   STA   gs6+4
   LDA   __local_f19__w27
   STA   gs6+1
   EOR   __local_f19__p20
   STA   g3
   LDA   #$00
   AND   g3
   STA   g1
   LDA   #$00
   STA   g1+1
   LDA   @13.s24
   EOR   #$FF
   AND   #$07
   CMP   #$06
   BEQ   .switch@8_break
   LDA   #$0A
   STA   __local_f19__l23
   LDA   #$00
   STA   __local_f19__w27
   JMP   .loop@11_continue
.f19@rot@0:
   LDA   __local_f19__l23
   AND   #$5A
   STA   __local_f19__l23
   ADC   g1
   STA   gs6+5
   LDA   __local_f19__0
   STA   __local_f19__w27
.loop@11_continue:
   CLC
   ADC   #$01
   STA   __local_f19__0
   LDA   __local_f19__w27
   CMP   #$03
   BCC   .f19@rot@0
.switch@8_break:
   LDA   ga5
   ORA   ga5+1
   BEQ   .lnot_true@11
   LDA   #$00
   BEQ   .lnot_end@12
.lnot_true@11:
   LDA   #$01
.lnot_end@12:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l29
   STA   __local_main__0
   STA   __local_main__1
   STA   __local_main__2
   LDA   g2
   CLC
   ADC   #$FF
   STA   __local_main__0
   LDA   g2+1
   ADC   #$FF
   STA   __local_main__l29
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __zpabi_f19__p21_2
   STA   __zpabi_f19__p21_3
   LDA   __local_main__0
   STA   __zpabi_f19__p21_0
   LDA   __local_main__l29
   STA   __zpabi_f19__p21_1
   LDX   #$00
   TXA
   JSR   f19
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JSR   fin0@run0
   LDA   gs6+5
   STA   __local_main__l29
   SEC
   SBC   #$01
   STA   gs6+5
   LDA   __local_main__l29
   STA   __zpabi_mix__v_0
   JSR   f7@run3
   LDA   gs6+1
   STA   g3
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

g4:
   DC.L  $00000002

ga5:
   DC.W  $0003
   DC.W  $52FF
   DC.W  $007F
   DC.W  $FFFF

gs6:
   DS.B  1
   DC.L  $0000000A
   DS.B  1

@13.s24:
   DC.W  $0100

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g2
   STA   g2+1
   STA   g3
   LDA   #$01
   STA   g1
   RTS
