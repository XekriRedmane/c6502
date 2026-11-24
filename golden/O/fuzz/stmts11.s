SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
gp6	EQU	$28
gp5	EQU	$2A
g2	EQU	$2C
__zpabi_f7__p8_0	EQU	$80
__zpabi_f7__p8_1	EQU	$81
__zpabi_f7__p8_2	EQU	$82
__zpabi_f7__p8_3	EQU	$83
__zpabi_f7__p9	EQU	$84
__zpabi_mix__v_0	EQU	$85
__zpabi_mix__v_1	EQU	$86
__zpabi_mix__v_2	EQU	$87
__zpabi_mix__v_3	EQU	$88
__local_main__w18	EQU	$89
__local_main__0	EQU	$8A
__local_main__1	EQU	$8B
__local_main__2	EQU	$8C
__local_main__3	EQU	$8D
__local_main__l17	EQU	$8E
__local_main__4	EQU	$8F
__local_f7__w14	EQU	$90
__local_f7__l12_1	EQU	$91
__local_f7__0	EQU	$92
__local_f7__p9	EQU	$93
__local_f7__w15	EQU	$94
__local_f7__l11	EQU	$95
__local_mix__0	EQU	$97
__local_mix__1	EQU	$98
__local_mix__2	EQU	$99
__local_mix__3	EQU	$9A

; @zp-link-meta-begin
; def f7 params=__zpabi_f7__p8_0,__zpabi_f7__p8_1,__zpabi_f7__p8_2,__zpabi_f7__p8_3,__zpabi_f7__p9 locals=__local_f7__w14,__local_f7__l12_1,__local_f7__0,__local_f7__p9,__local_f7__w15,__local_f7__l11,__local_f7__1 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def main params= locals=__local_main__w18,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l17,__local_main__4 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call main -> f7
; call main -> fin0
; call main -> fin1
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run1 section=.text local=true zero=false
; item f7 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga3 section=.data local=false zero=false
; item gs4 section=.data local=false zero=false
; item @3.s13 section=.bss local=true zero=true
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$00,$80 weight=137
; zp gp6 size=2 at=auto def=true init=<g1,>g1 weight=18
; zp gp5 size=2 at=auto def=true init=<g2,>g2 weight=17
; zp g2 size=1 at=auto def=true init=$02 weight=3
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
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   ga3
   JSR   fin0@run2
   LDA   ga3+1
   JMP   fin0@tail0

fin0@run2:
   SUBROUTINE

   JSR   fin0@run0
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@tail0:
   SUBROUTINE

   JSR   fin0@run0
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga3+2
   JSR   fin0@run2
   LDA   ga3+3
   JSR   fin0@run2
   LDA   gs4+1
   STA   __zpabi_mix__v_0
   LDA   gs4+2
   JSR   fin1@run1
   LDA   gs4+3
   JMP   fin0@tail0

fin1@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

f7:
   SUBROUTINE

   LDA   #$02
   STA   __local_f7__w14
   LDA   #$00
   STA   __local_f7__l12_1
   STA   __local_f7__0
   STA   __local_f7__p9
   STA   __local_f7__l11
   LDY   #$01
   LDA   (gp6),Y
   STA   __local_f7__l12_1
   DEY
   LDA   (gp6),Y
   SEC
   BCS   .carry_inc@0
   DEC   __local_f7__l12_1
.carry_inc@0:
   ORA   __local_f7__l12_1
   BEQ   .lnot_true@4
   LDA   #$00
   BEQ   .lnot_end@5
.lnot_true@4:
   LDA   #$01
.lnot_end@5:
   AND   #$07
   CMP   #$07
   BNE   .default@3
   LDA   #$0C
   STA   __local_f7__w15
   LDA   #$00
   STA   __local_f7__w14
   LDA   __zpabi_f7__p9
   STA   __local_f7__p9
   JMP   .loop@2_continue
.f7@rot@0:
   DEC   __local_f7__p9
   LDA   __local_f7__l11
   EOR   #$81
   STA   __local_f7__l12_1
   STA   __local_f7__l11
   LDA   __local_f7__0
   AND   #$03
   TAX
   LDA   __local_f7__l12_1
   EOR   ga3,X
   STA   __local_f7__w15
   LDA   __local_f7__0
   STA   __local_f7__w14
.loop@2_continue:
   LDA   __local_f7__w14
   CLC
   ADC   #$01
   STA   __local_f7__0
   LDA   __local_f7__w14
   CMP   #$04
   BCC   .f7@rot@0
   LDA   #$00
   SBC   __local_f7__w15
   STA   __local_f7__l11
   LDA   #$FF
   STA   __zpabi_mix__v_0
   JSR   fin1@run1
   JMP   .switch@0_break
.default@3:
   LDA   __zpabi_f7__p9
   RTS
.switch@0_break:
   LDY   #$00
   LDA   (gp5),Y
   BMI   .thread@0
   BEQ   .cmp_true@8
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   STA   __local_f7__0
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_f7__l12_1
   STA   __local_f7__w14
   LDA   __zpabi_f7__p8_0
   CLC
   ADC   __local_f7__0
   STA   __zpabi_mix__v_0
   LDA   __zpabi_f7__p8_1
   ADC   #$00
   STA   __zpabi_mix__v_1
   LDA   __zpabi_f7__p8_2
   ADC   __local_f7__l12_1
   STA   __zpabi_mix__v_2
   LDA   __zpabi_f7__p8_3
   ADC   __local_f7__w14
   STA   __zpabi_mix__v_3
   JSR   mix
   LDY   #$00
   LDA   (gp6),Y
   STA   __local_f7__l12_1
   LDA   #$00
   INY
   CMP   (gp6),Y
   BNE   .cmp_true@13
   LDA   #$07
   CMP   __local_f7__l12_1
   BNE   .cmp_true@13
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_f7__w14
   LDA   #$7F
   CLC
   ADC   __local_f7__w14
   STA   @3.s13
   LDA   #$00
   ADC   #$00
   STA   @3.s13+1
   LDA   __local_f7__p9
   SEC
   SBC   #$81
   AND   g2
   BEQ   .if_end@0
   LDA   #$00
   STA   __local_f7__w15
.loop@4_start:
   LDY   #$00
   LDA   (gp6),Y
   STA   __local_f7__l12_1
   INY
   LDA   (gp6),Y
   STA   __local_f7__w14
   LDA   gs4+1
   AND   __local_f7__l12_1
   STA   g1
   LDA   gs4+2
   AND   __local_f7__w14
   STA   g1+1
   INC   __local_f7__w15
   BEQ   .loop@4_start
.if_end@0:
   LDA   __local_f7__l11
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l17
   STA   __local_main__4
   LDA   gs4+1
   BNE   .carry_inc@1
   DEC   gs4+2
.carry_inc@1:
   DEC   gs4+1
   LDA   g2
   AND   #$07
   CMP   #$07
   BNE   .lb_skip@1
   JMP   .switch@5_break
.lb_skip@1:
   LDA   #$82
   STA   ga3+3
   LDA   #$00
   STA   __local_main__w18
.loop@8_continue:
   CLC
   ADC   #$01
   STA   __local_main__3
   LDA   __local_main__w18
   BEQ   .lb_skip@0
   JMP   .loop@8_break
.lb_skip@0:
   INC   g1
   BNE   .inc_done@0
   INC   g1+1
.inc_done@0:
   LDA   g1
   AND   #$07
   CMP   #$01
   BEQ   .case@10
   CMP   #$03
   BEQ   .case@11
   CMP   #$04
   BEQ   .case@11
   CMP   #$07
   BEQ   .case@13
.case@10:
.case@11:
   LDA   __local_main__3
   STA   __local_main__w18
   JMP   .loop@8_continue
.case@13:
   LDY   #$00
   LDA   (gp5),Y
   CLC
   ADC   #$01
   STA   __local_main__2
   STA   (gp5),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_main__1
   LDY   #$01
   LDA   (gp6),Y
   DEY
   ORA   (gp6),Y
   BEQ   .cond_else@1
   LDA   __local_main__l17
   STA   __local_main__0
   LDA   __local_main__4
   STA   __local_main__w18
   JMP   .cond_end@2
.cond_else@1:
   LDA   __local_main__l17
   STA   __local_main__0
   LDA   __local_main__4
   STA   __local_main__w18
.cond_end@2:
   LDA   __local_main__2
   AND   __local_main__0
   STA   __zpabi_mix__v_0
   LDA   __local_main__1
   AND   __local_main__w18
   JSR   fin1@run1
   JMP   .case@11
.loop@8_break:
.switch@5_break:
   LDA   ga3+3
   AND   gs4+3
   AND   #$07
   CMP   #$03
   BEQ   .case@15
   CMP   #$07
   BEQ   .case@16
   BNE   .switch@14_break
.case@15:
   LDA   #$FF
   STA   __zpabi_f7__p8_0
   LDA   #$00
   STA   __zpabi_f7__p8_1
   STA   __zpabi_f7__p8_2
   STA   __zpabi_f7__p8_3
   LDA   #$80
   STA   __zpabi_f7__p9
   JSR   f7
   STA   __local_main__l17
   LDA   #$00
   STA   __local_main__4
   LDA   __local_main__l17
   STA   __local_main__w18
   LDA   __local_main__4
   STA   __local_main__0
   INC   __local_main__w18
   BNE   .inc_done@1
   INC   __local_main__0
.inc_done@1:
   LDA   __local_main__w18
   STA   __local_main__l17
   LDA   __local_main__0
   STA   __local_main__4
.case@16:
.switch@14_break:
   JSR   fin0
   JSR   fin1
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

ga3:
   DC.B  $7F, $00, $FF, $80

gs4:
   DS.B  1
   DC.W  $8000
   DC.B  $FF

@3.s13:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   LDA   #$80
   STA   g1+1
   LDA   #$02
   STA   g2
   LDA   #<g2
   STA   gp5
   LDA   #>g2
   STA   gp5+1
   LDA   #<g1
   STA   gp6
   LDA   #>g1
   STA   gp6+1
   RTS
