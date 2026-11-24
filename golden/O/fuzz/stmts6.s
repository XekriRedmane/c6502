SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$28
g2	EQU	$2A
g1	EQU	$2C
__zpabi_f14__pp16_0	EQU	$80
__zpabi_f14__pp16_1	EQU	$81
__zpabi_f7__p9_0	EQU	$82
__zpabi_mix__v_0	EQU	$82
__zpabi_f7__p9_1	EQU	$83
__zpabi_mix__v_1	EQU	$83
__zpabi_f7__p10	EQU	$84
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_f14__0	EQU	$86
__local_main__0	EQU	$86
__local_f14__2	EQU	$88
__local_f14__3	EQU	$89
__local_f14__4	EQU	$8A
__local_main__l20	EQU	$8A
__local_f14__5	EQU	$8B
__local_main__4	EQU	$8B
__local_f14__6	EQU	$8C
__local_main__5	EQU	$8C
__local_main__6	EQU	$8D
__local_f7__0	EQU	$8E
__local_mix__0	EQU	$92
__local_mix__1	EQU	$93
__local_mix__2	EQU	$94
__local_mix__3	EQU	$95

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15,__zpabi_f14__pp16_0,__zpabi_f14__pp16_1 locals=__local_f14__0,__local_f14__1,__local_f14__2,__local_f14__3,__local_f14__4,__local_f14__5,__local_f14__6,__local_f14__7 indirect=false in_cycle=false param_regs=A,,
; def f7 params=__zpabi_f7__p8_0,__zpabi_f7__p8_1,__zpabi_f7__p9_0,__zpabi_f7__p9_1,__zpabi_f7__p10 locals=__local_f7__0,__local_f7__1 indirect=false in_cycle=false param_regs=A,X,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l20,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> f7
; call f14 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f7 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item @9.s18 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$FF,$FF
; zp z4 size=2 at=auto def=true init=$00,$00
; zp g2 size=2 at=auto def=true init=$00,$00 weight=10
; zp g1 size=1 at=auto def=true init=$FF weight=4
; runtime hargs_size=32 hargs_need=22
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
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run1
   JSR   mix
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   JSR   fin0@run1
   JSR   mix
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   JSR   fin0@run1
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga5
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin1@run0
   LDA   ga5
   JSR   fin1@run2
   LDA   ga5+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin1@run0
   LDA   ga5+1
   JSR   fin1@run2
   LDA   ga5+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin1@run0
   LDA   ga5+2
   JSR   fin1@run2
   LDA   ga5+3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin1@run0
   LDA   ga5+3
   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   LDA   gs6+3
   STA   __zpabi_mix__v_2
   LDA   gs6+4
   JSR   fin0@run3
   LDA   gs6+5
   STA   __zpabi_mix__v_0
   LDA   gs6+6
   STA   __zpabi_mix__v_1
   LDA   gs6+7
   STA   __zpabi_mix__v_2
   LDA   gs6+8
   STA   __zpabi_mix__v_3
   JMP   mix

f7:
   SUBROUTINE

   LDA   g2
   BNE   .carry_inc@0
   DEC   g2+1
.carry_inc@0:
   DEC   g2
   AND   #$FF
   AND   #$07
   CMP   #$01
   BEQ   .switch@0_break
   CMP   #$06
   BNE   .default@6
   INC   gs6+1
   BNE   .inc_done@0
   INC   gs6+2
   BNE   .inc_done@0
   INC   gs6+3
   BNE   .inc_done@0
   INC   gs6+4
.inc_done@0:
   LDA   gs6+1
   STA   ga5+1
   LDA   #$01
   EOR   z3
   STA   __local_f7__0
   LDA   #$FF
   CLC
   ADC   __local_f7__0
   AND   #$07
   CMP   #$07
.default@6:
.switch@0_break:
   LDA   ga5+2
   STA   __local_f7__0
   SEC
   SBC   #$01
   STA   ga5+2
   LDA   __local_f7__0
   AND   #$07
   CMP   #$04
   LDA   g1
   AND   z3
   RTS

f14:
   SUBROUTINE

   LDA   gs6+1
   LDX   gs6+2
   STX   __local_f14__2
   LDX   gs6+3
   STX   __zpabi_mix__v_2
   LDX   gs6+4
   STX   __local_f14__0
   ASL
   LDA   __local_f14__2
   ROL
   STA   __local_f14__3
   LDA   __zpabi_mix__v_2
   ROL
   STA   __local_f14__2
   LDA   __local_f14__0
   ROL
   STA   __zpabi_mix__v_2
   LDA   #$00
   ROL
   STA   __local_f14__0
   LDA   __local_f14__3
   STA   gs6+1
   LDA   __local_f14__2
   STA   gs6+2
   LDA   __zpabi_mix__v_2
   STA   gs6+3
   LDA   __local_f14__0
   STA   gs6+4
   LDA   __local_f14__3
   STA   __zpabi_mix__v_0
   LDA   __local_f14__2
   STA   __zpabi_mix__v_1
   LDA   __local_f14__0
   JSR   fin0@run3
   LDA   gs6+1
   STA   __zpabi_mix__v_2
   LDA   gs6+5
   STA   __local_f14__6
   LDA   gs6+6
   STA   __local_f14__5
   LDA   gs6+7
   STA   __local_f14__4
   LDA   gs6+8
   STA   __local_f14__2
   LDA   g1
   SEC
   SBC   __zpabi_mix__v_2
   STA   __zpabi_mix__v_2
   LDA   __local_f14__6
   SEC
   SBC   @9.s18
   AND   __zpabi_mix__v_2
   STA   __local_f14__3
   LDA   __local_f14__6
   STA   HARGS
   LDA   __local_f14__5
   STA   HARGS+1
   LDA   __local_f14__4
   STA   HARGS+2
   LDA   __local_f14__2
   STA   HARGS+3
   LDA   #$07
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   STA   g2
   LDA   HARGS+9
   STA   g2+1
   LDA   gs6+5
   STA   __zpabi_mix__v_2
   LDA   gs6+6
   STA   __local_f14__0
   LDA   __zpabi_mix__v_2
   AND   #$C1
   STA   __local_f14__2
   LDA   __local_f14__0
   AND   #$18
   TAX
   LDA   g1
   CLC
   SBC   z3
   LDA   #$00
   SBC   z3+1
   LDA   #$00
   ROL
   STA   __zpabi_f7__p9_0
   LDA   gs6+1
   SEC
   SBC   g2
   STA   __zpabi_f7__p10
   LDA   #$00
   STA   __zpabi_f7__p9_1
   LDA   __local_f14__2
   JSR   f7
   STA   __local_f14__0
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin1@run0
   LDA   __local_f14__0
   JSR   fin1@run2
   LDA   g2
   EOR   #$03
   STA   g2
   LDA   #$00
   STA   __zpabi_mix__v_0
   JSR   fin1@run0
   JSR   mix
   LDA   __local_f14__3
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   TAX
   LDA   __local_f14__3
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l20
   STA   __local_main__4
   STA   __local_main__5
   LDA   #$7F
   STA   __local_main__6
   LDA   __local_main__l20
   LDX   __local_main__4
   LDX   __local_main__5
   LDX   __local_main__6
   AND   #$81
   STA   __local_main__l20
   LDX   #$00
   STX   __local_main__4
   STX   __local_main__5
   STX   __local_main__6
   AND   #$07
   BEQ   .case@12
   CMP   #$04
   BEQ   .case@25
   CMP   #$06
   BEQ   .case@26
   CMP   #$07
   BEQ   .case@27
   BNE   .switch@11_break
.case@12:
   LDA   z4
   STA   __local_main__0
   BNE   .carry_inc@1
   DEC   z4+1
.carry_inc@1:
   DEC   z4
   LDA   z3
   ORA   __local_main__0
   AND   #$07
   CMP   #$03
   BEQ   .switch@11_break
   CMP   #$04
   BEQ   .switch@11_break
   CMP   #$06
   BEQ   .switch@11_break
.case@25:
.case@26:
.case@27:
.switch@11_break:
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

ga5:
   DC.B  $FF, $0B, $00, $03

gs6:
   DS.B  1
   DC.L  $000000FF
   DC.L  $00007FFF

@9.s18:
   DC.W  $0002

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   STA   g2+1
   STA   z4
   STA   z4+1
   LDA   #$FF
   STA   g1
   STA   z3
   STA   z3+1
   RTS
