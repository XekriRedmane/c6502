SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
g2	EQU	$28
g1	EQU	$29
@11.s19	EQU	$2B
__zpabi_f14__p16_0	EQU	$80
__zpabi_f6__p8_0	EQU	$80
__zpabi_f14__p16_1	EQU	$81
__zpabi_f6__p8_1	EQU	$81
__zpabi_f14__p16_2	EQU	$82
__zpabi_f14__p16_3	EQU	$83
__zpabi_f14__p17	EQU	$84
__zpabi_mix__v_0	EQU	$85
__zpabi_mix__v_1	EQU	$86
__zpabi_mix__v_2	EQU	$87
__zpabi_mix__v_3	EQU	$88
__local_f14__0	EQU	$89
__local_f6__w12	EQU	$89
__local_f14__1	EQU	$8A
__local_f6__i11	EQU	$8A
__local_main__2	EQU	$8B
__local_main__3	EQU	$8C
__local_main__4	EQU	$8D
__local_main__5	EQU	$8E
__local_main__i24	EQU	$8F
__local_main__l21	EQU	$90
__local_main__6	EQU	$91
__local_main__7	EQU	$92
__local_main__8	EQU	$93
__local_mix__0	EQU	$94
__local_mix__1	EQU	$95
__local_mix__2	EQU	$96
__local_mix__3	EQU	$97

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15,__zpabi_f14__p16_0,__zpabi_f14__p16_1,__zpabi_f14__p16_2,__zpabi_f14__p16_3,__zpabi_f14__p17 locals=__local_f14__0,__local_f14__1 indirect=false in_cycle=false param_regs=A,,,,,
; def f6 params=__zpabi_f6__p7_0,__zpabi_f6__p7_1,__zpabi_f6__p8_0,__zpabi_f6__p8_1 locals=__local_f6__w12,__local_f6__i11,__local_f6__0,__local_f6__1 indirect=false in_cycle=false param_regs=A,X,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__i24,__local_main__l21,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> mix
; call f6 -> mix
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
; item f6 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$00,$00
; zp g2 size=1 at=auto def=true init=$FF weight=20
; zp g1 size=2 at=auto def=true init=$00,$01 weight=15
; zp @11.s19 size=2 at=auto def=true init=$00,$01 weight=8 local=true
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
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   JSR   fin0@run0
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
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

   LDA   ga4+2
   STA   __zpabi_mix__v_0
   LDA   ga4+3
   JSR   fin0@run0
   LDA   ga4+4
   STA   __zpabi_mix__v_0
   LDA   ga4+5
   JSR   fin0@run0
   LDA   ga4+6
   STA   __zpabi_mix__v_0
   LDA   ga4+7
   JSR   fin0@run0
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   STA   __zpabi_mix__v_1
   LDA   gs5+3
   STA   __zpabi_mix__v_2
   LDA   gs5+4
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   gs5+5
   STA   __zpabi_mix__v_0
   LDA   gs5+6
   STA   __zpabi_mix__v_1
   LDA   gs5+7
   STA   __zpabi_mix__v_2
   LDA   gs5+8
   JMP   fin0@tail0

f6:
   SUBROUTINE

   LDA   #$02
   STA   __local_f6__w12
   LDA   #$00
   STA   __local_f6__i11
   LDA   gs5+1
   STA   HARGS
   LDA   gs5+2
   STA   HARGS+1
   LDA   gs5+3
   STA   HARGS+2
   LDA   gs5+4
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   JSR   lsr32
   LDA   #$80
   AND   HARGS+5
   AND   #$07
   CMP   #$03
   BEQ   .case@1
   CMP   #$06
   BEQ   .case@4
   CMP   #$07
   BEQ   .case@10
   BNE   .default@11
.case@1:
   LDA   #$01
   STA   __local_f6__i11
.f6@rot@0:
   LDA   #$00
   STA   __local_f6__w12
.loop@3_start:
   LDA   z3
   ORA   #$81
   STA   z3
   INC   __local_f6__w12
   LDA   __local_f6__w12
   CMP   #$04
   BCC   .loop@3_start
   DEC   __local_f6__i11
   BNE   .f6@rot@0
.case@4:
   LDA   gs5+5
   ORA   gs5+6
   ORA   gs5+7
   ORA   gs5+8
   BEQ   .if_else@1
   LDX   gs5+6
   LDA   gs5+5
   RTS
.if_else@1:
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@2:
   BCC   .mul_skip@3
   CLC
.mul_skip@3:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@2
   STA   __zpabi_mix__v_1
   LDA   HARGS
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
.case@10:
.default@11:
   LDX   #$00
   TXA
   RTS

f14:
   SUBROUTINE

   LDA   __zpabi_f14__p16_0
   SEC
   SBC   g1
   STA   __local_f14__1
   LDA   __zpabi_f14__p16_1
   SBC   g1+1
   STA   __local_f14__0
   LDA   __local_f14__1
   CLC
   ADC   #$01
   STA   __zpabi_mix__v_0
   LDA   __local_f14__0
   ADC   #$00
   JSR   fin0@run0
   LDA   __zpabi_f14__p17
   AND   #$07
   CMP   #$03
   BEQ   .case@13
   CMP   #$04
   BEQ   .case@14
   CMP   #$07
   BEQ   .case@20
   BNE   .default@21
.case@13:
.case@14:
   LDA   g1
   AND   #$07
   CMP   #$03
   BEQ   .switch@12_break
   CMP   #$07
   BNE   .switch@12_break
   LDA   @11.s19
   STA   __zpabi_mix__v_0
   LDA   @11.s19+1
.xjump@0:
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   JMP   .switch@12_break
.case@20:
   LDA   ga4+4
   AND   z3
   STA   __zpabi_mix__v_0
   LDA   ga4+5
   AND   z3+1
   JMP   .xjump@0
.default@21:
   LDA   ga4+6
   ORA   ga4+7
   BEQ   .cond_else@2
   LDA   __zpabi_f14__p16_0
   LDX   __zpabi_f14__p16_1
   RTS
.cond_else@2:
   LDA   #$00
   LDX   #$00
   RTS
.switch@12_break:
   LDA   g2
   LSR
   LSR
   TAX
   LDA   g2
   ROR
   ROR
   ROR
   AND   #$C0
   RTS

main:
   SUBROUTINE

   LDA   #$09
   STA   __local_main__l21
   LDA   #$00
   STA   __local_main__6
   STA   __local_main__7
   STA   __local_main__8
   STA   __local_main__i24
.loop@22_start:
   LDA   __local_main__i24
   CMP   #$03
   BCC   .lb_skip@0
   JMP   .loop@22_break
.lb_skip@0:
   LDA   __local_main__l21
   LDX   __local_main__6
   STX   __zpabi_mix__v_0
   LDX   __local_main__7
   LDX   __local_main__8
   ADC   g2
   STA   ga4+4
   LDA   __zpabi_mix__v_0
   ADC   #$00
   STA   ga4+5
   LDA   __local_main__i24
   AND   #$03
   ASL
   TAX
   LDA   ga4,X
   STA   __local_main__5
   LDA   ga4+1,X
   STA   __local_main__4
   LDA   gs5+5
   LDX   gs5+6
   STX   __local_main__2
   LDX   gs5+7
   STX   __zpabi_mix__v_0
   LDX   gs5+8
   STX   __zpabi_mix__v_1
   CLC
   SBC   __local_main__5
   LDA   __local_main__2
   SBC   __local_main__4
   LDA   __zpabi_mix__v_0
   SBC   #$00
   LDA   __zpabi_mix__v_1
   SBC   #$00
   LDA   #$00
   ROL
   STA   g1
   LDA   #$00
   STA   g1+1
   LDA   gs5+5
   AND   #$5A
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   gs5+6
   LDY   #$00
   STY   gs5+7
   STA   gs5+8
   LDA   __zpabi_mix__v_1
   STA   gs5+5
   LDA   #$E4
   CLC
   ADC   __zpabi_mix__v_1
   STA   g2
   INC   __local_main__i24
   JMP   .loop@22_start
.loop@22_break:
   LDA   __local_main__l21
   STA   __zpabi_mix__v_0
   LDA   __local_main__6
   STA   __zpabi_mix__v_1
   LDA   __local_main__7
   STA   __zpabi_mix__v_2
   LDA   __local_main__8
   JSR   fin0@run1
   LDA   g2
   EOR   #$03
   STA   g2
   LDA   gs5+1
   STA   __local_main__5
   LDA   gs5+2
   STA   __local_main__4
   LDA   gs5+3
   STA   __local_main__3
   LDA   gs5+4
   STA   __local_main__2
   LDA   #$05
   CLC
   ADC   g1
   STA   __zpabi_mix__v_0
   LDA   #$00
   ADC   g1+1
   STA   __zpabi_mix__v_1
   LDA   __local_main__5
   SEC
   SBC   __zpabi_mix__v_0
   LDA   __local_main__4
   SBC   __zpabi_mix__v_1
   LDA   __local_main__3
   SBC   #$00
   LDA   __local_main__2
   SBC   #$00
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_1
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g1
   SEC
   SBC   #$81
   STA   __zpabi_mix__v_0
   LDA   g1+1
   SBC   #$00
   STA   __zpabi_mix__v_1
   STA   g1+1
   LDA   __zpabi_mix__v_0
   STA   g1
   STA   gs5+5
   LDA   __zpabi_mix__v_1
   STA   gs5+6
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   gs5+7
   STA   gs5+8
   LDA   z3
   ORA   z3+1
   BEQ   .cond_else@4
   LDA   #$03
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   .cond_end@5
.cond_else@4:
   LDA   #$FF
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
.cond_end@5:
   LDA   g1
   CLC
   ADC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   g1+1
   ADC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
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

ga4:
   DC.W  $0100
   DC.W  $0080
   DC.W  $0002
   DC.W  $007F

gs5:
   DS.B  1
   DC.L  $00008000
   DC.L  $00008000

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   z3
   STA   z3+1
   STA   @11.s19
   LDA   #$01
   STA   g1+1
   STA   @11.s19+1
   LDA   #$FF
   STA   g2
   RTS
