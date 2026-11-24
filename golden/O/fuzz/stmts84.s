SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
g2	EQU	$28
__zpabi_f20__p21_0	EQU	$80
__zpabi_f6__pp8_0	EQU	$80
__zpabi_f20__p21_1	EQU	$81
__zpabi_f6__pp8_1	EQU	$81
__local_main__0	EQU	$82
__zpabi_f20__p21_2	EQU	$82
__zpabi_f20__p21_3	EQU	$83
__local_main__2	EQU	$84
__zpabi_f20__p21_4	EQU	$84
__zpabi_f20__p21_5	EQU	$85
__local_main__l26	EQU	$86
__zpabi_f20__p21_6	EQU	$86
__zpabi_f20__p21_7	EQU	$87
__zpabi_f20__pp22_0	EQU	$88
__zpabi_f20__pp22_1	EQU	$89
__local_f6__p7_0	EQU	$8A
__zpabi_f13__p15_0	EQU	$8A
__zpabi_f13__p15_1	EQU	$8B
__zpabi_f13__p15_2	EQU	$8C
__zpabi_f13__p15_3	EQU	$8D
__zpabi_mix__v_0	EQU	$8E
__zpabi_mix__v_1	EQU	$8F
__zpabi_mix__v_2	EQU	$90
__zpabi_mix__v_3	EQU	$91
__local_f20__0	EQU	$92
__local_f20__1	EQU	$93
__local_f13__0	EQU	$98
__local_f13__2	EQU	$9A
__local_f13__w18	EQU	$9B
__local_f13__3	EQU	$9C
__local_f13__p14	EQU	$9D
__local_mix__0	EQU	$9F
__local_mix__1	EQU	$A0
__local_mix__2	EQU	$A1
__local_mix__3	EQU	$A2

; @zp-link-meta-begin
; def f13 params=__zpabi_f13__p14,__zpabi_f13__p15_0,__zpabi_f13__p15_1,__zpabi_f13__p15_2,__zpabi_f13__p15_3 locals=__local_f13__0,__local_f13__1,__local_f13__2,__local_f13__w18,__local_f13__3,__local_f13__p14,__local_f13__4 indirect=false in_cycle=false param_regs=A,,,,
; def f20 params=__zpabi_f20__p21_0,__zpabi_f20__p21_1,__zpabi_f20__p21_2,__zpabi_f20__p21_3,__zpabi_f20__p21_4,__zpabi_f20__p21_5,__zpabi_f20__p21_6,__zpabi_f20__p21_7,__zpabi_f20__pp22_0,__zpabi_f20__pp22_1 locals=__local_f20__0,__local_f20__1,__local_f20__2,__local_f20__3,__local_f20__4,__local_f20__5 indirect=false in_cycle=false
; def f6 params=__zpabi_f6__p7_0,__zpabi_f6__p7_1,__zpabi_f6__pp8_0,__zpabi_f6__pp8_1 locals=__local_f6__0,__local_f6__p7_0,__local_f6__p7_1 indirect=false in_cycle=false param_regs=A,X,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l26,__local_main__5,__local_main__6 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f13 -> mix
; call f20 -> f13
; call f20 -> mix
; call f6 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f13
; call main -> f6
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
; item fin1@run3 section=.text local=true zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f6 section=.text local=false zero=false
; item f13 section=.text local=false zero=false
; item f20 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$7F,$00
; zp g2 size=2 at=auto def=true init=$FF,$7F weight=7
; runtime hargs_size=32 hargs_need=12
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

   LDA   g1+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   JSR   fin0@run1
   LDA   z3+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   STA   __zpabi_mix__v_1
   LDA   ga4+2
   STA   __zpabi_mix__v_2
   LDA   ga4+3
   JMP   fin0@tail0

fin0@run1:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

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

   JSR   fin1@run2
   STA   __zpabi_mix__v_1
   LDA   ga4+6
   STA   __zpabi_mix__v_2
   LDA   ga4+7
   JSR   fin0@run0
   LDA   ga4+8
   STA   __zpabi_mix__v_0
   LDA   ga4+9
   STA   __zpabi_mix__v_1
   LDA   ga4+10
   STA   __zpabi_mix__v_2
   LDA   ga4+11
   JSR   fin0@run0
   LDA   ga4+12
   STA   __zpabi_mix__v_0
   LDA   ga4+13
   STA   __zpabi_mix__v_1
   LDA   ga4+14
   STA   __zpabi_mix__v_2
   LDA   ga4+15
   JSR   fin0@run0
   LDA   gs5+1
   JSR   fin1@run3
   JMP   fin0@tail0

fin1@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin1@run2:
   SUBROUTINE

   LDA   ga4+4
   STA   __zpabi_mix__v_0
   LDA   ga4+5
   RTS

fin2:
   SUBROUTINE

   LDA   gs5+2
   STA   __zpabi_mix__v_0
   LDA   gs5+3
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

f6:
   SUBROUTINE

   STA   __local_f6__p7_0
   LDA   #$80
   AND   ga4+4
   STA   gs5+1
   LDA   __local_f6__p7_0
   AND   #$80
   STA   __local_f6__p7_0
   LDA   #$00
   AND   __local_f6__p7_0
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   DEC   gs5+1
   LDX   #$00
   LDA   #$04
   RTS

f13:
   SUBROUTINE

   STA   __local_f13__p14
   JSR   fin1@run2
   STA   __local_f13__2
   LDA   ga4+6
   STA   __local_f13__w18
   LDA   ga4+7
   STA   __local_f13__3
   ASL
   ROR   __local_f13__3
   ROR   __local_f13__w18
   ROR   __local_f13__2
   ROR   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_0
   STA   ga4+4
   LDA   __local_f13__2
   STA   ga4+5
   LDA   __local_f13__w18
   STA   ga4+6
   LDA   __local_f13__3
   STA   ga4+7
   LDA   __zpabi_mix__v_0
   AND   #$07
   CMP   #$01
   BEQ   .case@1
   CMP   #$02
   BEQ   .case@3
   CMP   #$04
   BEQ   .case@5
   BNE   .default@6
.case@1:
   LDA   #$00
   STA   __local_f13__w18
.loop@2_start:
   LDA   gs5+2
   STA   __local_f13__2
   LDA   gs5+3
   STA   __local_f13__0
   LDA   #$00
   SEC
   SBC   __local_f13__2
   STA   __zpabi_mix__v_0
   LDA   __local_f13__2
   SBC   __local_f13__0
   STA   gs5+3
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   STA   gs5+2
   JSR   fin0@run1
   INC   ga4+12
   BNE   .inc_done@0
   INC   ga4+13
   BNE   .inc_done@0
   INC   ga4+14
   BNE   .inc_done@0
   INC   ga4+15
.inc_done@0:
   INC   __local_f13__w18
   LDA   __local_f13__w18
   CMP   #$02
   BCC   .loop@2_start
   BCS   .switch@0_break
.case@3:
   LDA   #$00
   SEC
   SBC   __local_f13__p14
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   #$00
   LSR
   STA   z3+1
   LDA   __zpabi_mix__v_0
   ROR
   LSR   z3+1
   ROR
   LSR   z3+1
   ROR
   LSR   z3+1
   ROR
   STA   z3
.case@5:
.default@6:
.switch@0_break:
   LDA   __local_f13__p14
   SEC
   SBC   #$03
   RTS

f20:
   SUBROUTINE

   LDA   gs5+1
   STA   __local_f20__0
   SEC
   SBC   #$01
   STA   gs5+1
   LDA   __local_f20__0
   AND   #$07
   CMP   #$01
   BEQ   .case@8
   CMP   #$02
   INC   gs5+2
   BNE   .inc_done@1
   INC   gs5+3
.inc_done@1:
.case@8:
   LDA   g2
   EOR   #$FF
   STA   __local_f20__1
   LDA   g2+1
   EOR   #$FF
   ORA   __local_f20__1
   BEQ   .if_end@3
   INC   gs5+1
.if_end@3:
   LDA   gs5+2
   STA   __zpabi_f13__p15_0
   LDA   gs5+3
   STA   __zpabi_f13__p15_1
   LDA   #$00
   STA   __zpabi_f13__p15_2
   STA   __zpabi_f13__p15_3
   JSR   f13
   LDA   g2
   AND   ga4+8
   STA   __zpabi_mix__v_0
   LDA   g2+1
   AND   ga4+9
   STA   __zpabi_mix__v_1
   LDA   #$00
   AND   ga4+10
   STA   __zpabi_mix__v_2
   LDA   #$00
   AND   ga4+11
   JSR   fin0@run0
   LDA   gs5+1
   STA   HARGS
   LDA   #$5A
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   gs5+1
   JSR   fin1@run3
   JSR   fin0@run0
   LDA   z3
   SEC
   SBC   g2
   STA   HARGS+8
   LDA   z3+1
   SBC   g2+1
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __local_main__l26
   LDA   #$00
   STA   __local_main__l26+1
   STA   __local_main__l26+2
   LDA   g2
   ASL
   ASL
   ASL
   AND   #$07
   BNE   .lb_skip@5
   JMP   .switch@12_break
.lb_skip@5:
   CMP   #$02
   BEQ   .case@14
   CMP   #$04
   BNE   .lb_skip@4
   JMP   .case@22
.lb_skip@4:
   CMP   #$05
   BNE   .lb_skip@3
   JMP   .case@23
.lb_skip@3:
   JMP   .default@24
.case@14:
   LDA   ga4+15
   BNE   .thread@0
   LDA   ga4+14
   BNE   .thread@0
   LDA   ga4+13
   BNE   .thread@0
   LDA   ga4+12
   BEQ   .cmp_true@4
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   AND   #$07
   BNE   .lb_skip@2
   JMP   .switch@12_break
.lb_skip@2:
   CMP   #$02
   BNE   .lb_skip@1
   JMP   .switch@12_break
.lb_skip@1:
   CMP   #$04
   BEQ   .case@18
   CMP   #$06
   BEQ   .case@19
   BNE   .default@21
.case@18:
.case@19:
   LDA   g1
   STA   __local_main__2
   LDX   g1+1
   LDA   #<__local_main__l26
   STA   __zpabi_f6__pp8_0
   LDA   #>__local_main__l26
   STA   __zpabi_f6__pp8_1
   LDA   __local_main__2
   JSR   f6
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JSR   fin0@run1
.default@21:
   LDA   #$02
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+2
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+3
   LDA   #$05
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   __local_main__l26
   LDA   HARGS+6
   STA   __local_main__l26+1
   LDA   HARGS+7
   STA   __local_main__l26+2
   LDA   HARGS+5
   ORA   HARGS+6
   ORA   HARGS+7
   BEQ   .lb_skip@0
   JMP   .switch@12_break
.lb_skip@0:
   LDA   #$00
   SEC
   SBC   HARGS+5
   LDA   #$00
   SBC   HARGS+6
   LDA   #$01
   SBC   HARGS+7
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   g1
   LDA   #$00
   STA   g1+1
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_main__0
   LDA   gs5+1
   SEC
   SBC   HARGS+5
   LDA   #$00
   SBC   HARGS+6
   LDA   __local_main__0
   SBC   HARGS+7
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   LDX   #$7F
   STX   __zpabi_f13__p15_0
   LDX   #$80
   STX   __zpabi_f13__p15_1
   LDX   #$00
   STX   __zpabi_f13__p15_2
   STX   __zpabi_f13__p15_3
   JSR   f13
   STA   __local_main__l26
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_main__l26+1
   STA   __local_main__l26+2
   JMP   .switch@12_break
.case@22:
   LDA   g2
   ASL
   STA   __local_main__0
   LDA   g2+1
   ROL
   ASL   __local_main__0
   ROL
   ASL   __local_main__0
   ROL
   ASL   __local_main__0
   ROL
   STA   g2+1
   STA   __zpabi_mix__v_1
   LDA   __local_main__0
   STA   g2
   STA   __zpabi_mix__v_0
   JSR   fin0@run1
   LDA   g1
   ORA   g1+1
   BEQ   .cond_else@6
   LDA   #$00
   STA   __local_main__2
   JMP   .cond_end@7
.cond_else@6:
   LDA   #$02
   STA   __local_main__2
   LDA   #$00
.cond_end@7:
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __zpabi_f13__p15_3
   LDA   __local_main__2
   STA   __zpabi_f13__p15_0
   LDA   #$00
   STA   __zpabi_f13__p15_1
   STA   __zpabi_f13__p15_2
   LDA   #$FF
   JSR   f13
.case@23:
.default@24:
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
   DC.W  $FFFF

ga4:
   DC.L  $7FFFFFFF
   DC.L  $7FFFFFFF
   DC.L  $0000007F
   DC.L  $00000002

gs5:
   DS.B  1
   DC.B  $7F
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z3+1
   LDA   #$FF
   STA   g2
   LDA   #$7F
   STA   g2+1
   STA   z3
   RTS
