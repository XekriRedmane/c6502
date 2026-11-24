SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
g3	EQU	$28
g1	EQU	$2A
g2	EQU	$2E
__zpabi_f7__p9_0	EQU	$80
__zpabi_f7__p9_1	EQU	$81
__zpabi_f7__p9_2	EQU	$82
__zpabi_f7__p9_3	EQU	$83
__zpabi_f7__p10_0	EQU	$84
__zpabi_f7__p10_1	EQU	$85
__zpabi_f7__p10_2	EQU	$86
__zpabi_f7__p10_3	EQU	$87
__zpabi_f7__p10_4	EQU	$88
__zpabi_f7__p10_5	EQU	$89
__zpabi_f7__p10_6	EQU	$8A
__zpabi_f7__p10_7	EQU	$8B
__zpabi_f7__pp11_0	EQU	$8C
__zpabi_f7__pp11_1	EQU	$8D
__zpabi_mix__v_0	EQU	$8E
__zpabi_mix__v_1	EQU	$8F
__zpabi_mix__v_2	EQU	$90
__zpabi_mix__v_3	EQU	$91
__local_main__0	EQU	$92
__local_main__1	EQU	$93
__local_main__3	EQU	$95
__local_f15__4	EQU	$96
__local_f15__l17	EQU	$97
__local_main__l25	EQU	$97
__local_f15__5	EQU	$98
__local_f15__pp16	EQU	$99
__local_f15__6	EQU	$9A
__local_f15__l20	EQU	$9B
__local_f7__0	EQU	$A0
__local_f7__w13	EQU	$A1
__local_mix__0	EQU	$A2
__local_mix__1	EQU	$A3
__local_mix__2	EQU	$A4
__local_mix__3	EQU	$A5

; @zp-link-meta-begin
; def f15 params=__zpabi_f15__pp16_0,__zpabi_f15__pp16_1 locals=__local_f15__0,__local_f15__1,__local_f15__2,__local_f15__3,__local_f15__4,__local_f15__l17,__local_f15__5,__local_f15__pp16,__local_f15__6,__local_f15__l20,__local_f15__8,__local_f15__9,__local_f15__10 indirect=false in_cycle=false param_regs=A,X
; def f7 params=__zpabi_f7__p8,__zpabi_f7__p9_0,__zpabi_f7__p9_1,__zpabi_f7__p9_2,__zpabi_f7__p9_3,__zpabi_f7__p10_0,__zpabi_f7__p10_1,__zpabi_f7__p10_2,__zpabi_f7__p10_3,__zpabi_f7__p10_4,__zpabi_f7__p10_5,__zpabi_f7__p10_6,__zpabi_f7__p10_7,__zpabi_f7__pp11_0,__zpabi_f7__pp11_1 locals=__local_f7__p8,__local_f7__0,__local_f7__w13 indirect=false in_cycle=false param_regs=A,,,,,,,,,,,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__l25,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f15 -> f7
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f7
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run5 section=.text local=true zero=false
; item f7 section=.text local=false zero=false
; item f15 section=.text local=false zero=false
; item f15@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$03,$00
; zp g3 size=2 at=auto def=true init=$FF,$00 weight=29
; zp g1 size=4 at=auto def=true init=$FF,$FF,$00,$00 weight=28
; zp g2 size=2 at=auto def=true init=$80,$00 weight=11
; runtime hargs_size=32 hargs_need=11
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
   JSR   fin0@run3
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   JSR   fin0@run4
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
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run3:
   SUBROUTINE

   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   RTS

fin1:
   SUBROUTINE

   LDA   ga5
   JSR   fin1@run2
   LDA   ga5+1
   JSR   fin1@run2
   LDA   ga5+2
   JSR   fin1@run2
   LDA   ga5+3
   JSR   fin1@run0
   JMP   mix

fin1@run2:
   SUBROUTINE

   JSR   fin1@run0
   JMP   mix

fin1@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin2:
   SUBROUTINE

   LDA   gs6+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   gs6+3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   fin2@run5
   STA   __zpabi_mix__v_1
   JMP   mix

fin2@run5:
   SUBROUTINE

   LDA   gs6+2
   STA   __zpabi_mix__v_0
   LDA   gs6+3
   RTS

f7:
   SUBROUTINE

   CLC
   ADC   #$5A
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run4
   LDA   gs6+2
   LDX   gs6+3
   STX   __zpabi_mix__v_0
   EOR   #$FF
   STA   gs6+2
   EOR   #$01
   STA   g3
   LDA   __zpabi_mix__v_0
   STA   g3+1
   JSR   fin0@run3
   STA   __local_f7__0
   INC   __zpabi_mix__v_0
   BNE   .inc_done@0
   INC   __local_f7__0
.inc_done@0:
   LDA   __zpabi_mix__v_0
   STA   g2
   LDA   __local_f7__0
   STA   g2+1
   LDA   #$00
   STA   __local_f7__w13
.loop@0_start:
   JSR   fin0@run3
   LDA   __zpabi_mix__v_0
   AND   #$07
   CMP   #$02
   BEQ   .case@3
   CMP   #$04
   BEQ   .case@4
   BNE   .switch@2_break
.case@3:
   LDA   __local_f7__w13
   AND   #$03
   TAX
   INC   ga5,X
   LDA   g1
   EOR   ga5,X
   STA   g1
   LDA   ga5,X
   AND   #$01
   STA   ga5,X
   LDX   #$00
   RTS
.case@4:
   LDA   g3
   EOR   #$FA
   STA   z4
   LDA   g3+1
   EOR   #$5C
   STA   z4+1
   JSR   fin2@run5
   STA   __local_f7__0
   LDA   __zpabi_mix__v_0
   BNE   .carry_inc@0
   DEC   __local_f7__0
.carry_inc@0:
   DEC   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_0
   STA   gs6+2
   LDA   __local_f7__0
   STA   gs6+3
   LDA   __zpabi_mix__v_0
   AND   #$07
   CMP   #$02
   BEQ   .case@6
   CMP   #$05
   BEQ   .case@7
.case@6:
.case@7:
.switch@2_break:
   INC   __local_f7__w13
   LDA   __local_f7__w13
   CMP   #$04
   BCC   .loop@0_start
   LDA   #$00
   SBC   ga5+1
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   #$00
   TAX
   LDA   __zpabi_mix__v_0
   RTS

f15:
   SUBROUTINE

   STA   __local_f15__pp16
   STX   __local_f15__6
   LDA   #$7F
   STA   __local_f15__l17
   LDA   #$00
   STA   __local_f15__5
   LDA   #$FF
   STA   __local_f15__l20
   LDA   #$00
   STA   __local_f15__l20+1
   STA   __local_f15__l20+2
   STA   __local_f15__l20+3
   LDA   g3
   STA   __local_f15__l17
   LDA   g3+1
   STA   __local_f15__5
   LDA   g1
   SEC
   SBC   gs6+2
   STA   z4
   LDA   g1+1
   SBC   gs6+3
   STA   z4+1
   LDY   #$00
   LDA   (__local_f15__pp16),Y
   STA   __local_f15__4
   LDA   __local_f15__l17
   STA   __zpabi_f7__p9_0
   LDA   __local_f15__5
   STA   __zpabi_f7__p9_1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_f7__p9_2
   STA   __zpabi_f7__p9_3
   LDA   #$00
   STA   __zpabi_f7__p10_0
   JSR   f15@run1
   LDA   #$80
   STA   __zpabi_f7__p10_1
   LDA   #<__local_f15__l20
   STA   __zpabi_f7__pp11_0
   LDA   #>__local_f15__l20
   STA   __zpabi_f7__pp11_1
   LDA   __local_f15__4
   JSR   f7
   LDA   gs6+1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   TAX
   LDA   gs6+1
   RTS

f15@run1:
   SUBROUTINE

   STA   __zpabi_f7__p10_2
   STA   __zpabi_f7__p10_3
   STA   __zpabi_f7__p10_4
   STA   __zpabi_f7__p10_5
   STA   __zpabi_f7__p10_6
   STA   __zpabi_f7__p10_7
   RTS

main:
   SUBROUTINE

   LDA   #$7F
   STA   __local_main__l25
   LDA   #$00
   STA   __local_main__l25+1
   STA   __local_main__l25+2
   STA   __local_main__l25+3
   LDA   gs6+1
   STA   __local_main__0
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   HARGS+1
   LDA   __local_main__0
   STA   HARGS
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   g3+1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g1
   SEC
   SBC   g3
   STA   __zpabi_f7__p9_0
   LDA   g1+1
   SBC   g3+1
   STA   __zpabi_f7__p9_1
   LDA   g1+2
   SBC   __local_main__1
   STA   __zpabi_f7__p9_2
   LDA   g1+3
   SBC   __local_main__0
   STA   __zpabi_f7__p9_3
   LDA   #$00
   STA   __zpabi_f7__p10_0
   STA   __zpabi_f7__p10_1
   JSR   f15@run1
   LDA   #<__local_main__l25
   STA   __zpabi_f7__pp11_0
   LDA   #>__local_main__l25
   STA   __zpabi_f7__pp11_1
   LDA   HARGS+6
   JSR   f7
   LDA   gs6+1
   ORA   #$05
   STA   gs6+1
   AND   #$0F
   STA   __local_main__l25
   LDA   #$00
   STA   __local_main__l25+1
   STA   __local_main__l25+2
   STA   __local_main__l25+3
   LDA   g1
   AND   z4
   STA   __local_main__3
   LDA   g1+1
   AND   z4+1
   ORA   __local_main__3
   BEQ   .cond_else@1
   LDA   gs6+1
   STA   __local_main__1
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .cond_end@2
.sx_neg@20:
   LDA   #$FF
   BNE   .cond_end@2
.cond_else@1:
   LDA   g3
   STA   __local_main__1
   LDA   g3+1
.cond_end@2:
   STA   gs6+3
   LDA   __local_main__1
   STA   gs6+2
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
   DC.B  $01, $FF, $03, $00

gs6:
   DS.B  1
   DC.B  $7F
   DC.W  $0001

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+2
   STA   g1+3
   STA   g2+1
   STA   g3+1
   STA   z4+1
   LDA   #$FF
   STA   g1
   STA   g1+1
   STA   g3
   LDA   #$80
   STA   g2
   LDA   #$03
   STA   z4
   RTS
