SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
g2	EQU	$28
@4.s14	EQU	$2C
gp6	EQU	$2E
__local_main__0	EQU	$80
__zpabi_f8__p9_0	EQU	$80
__local_main__1	EQU	$81
__zpabi_f8__p9_1	EQU	$81
__local_main__l18	EQU	$82
__zpabi_f8__p9_2	EQU	$82
__zpabi_f8__p9_3	EQU	$83
__zpabi_f8__p9_4	EQU	$84
__zpabi_f8__p9_5	EQU	$85
__zpabi_f8__p9_6	EQU	$86
__zpabi_f8__p9_7	EQU	$87
__zpabi_f8__p10	EQU	$88
__zpabi_f8__p11	EQU	$89
__zpabi_mix__v_0	EQU	$8A
__zpabi_mix__v_1	EQU	$8B
__zpabi_mix__v_2	EQU	$8C
__zpabi_mix__v_3	EQU	$8D
__local_f8__w17	EQU	$8E
__local_f8__0	EQU	$8F
__local_f8__1	EQU	$90
__local_f8__3	EQU	$93
__local_f8__l12	EQU	$94
__local_f8__4	EQU	$95
__local_mix__0	EQU	$96
__local_mix__1	EQU	$97
__local_mix__2	EQU	$98
__local_mix__3	EQU	$99

; @zp-link-meta-begin
; def f8 params=__zpabi_f8__p9_0,__zpabi_f8__p9_1,__zpabi_f8__p9_2,__zpabi_f8__p9_3,__zpabi_f8__p9_4,__zpabi_f8__p9_5,__zpabi_f8__p9_6,__zpabi_f8__p9_7,__zpabi_f8__p10,__zpabi_f8__p11 locals=__local_f8__w17,__local_f8__0,__local_f8__1,__local_f8__2,__local_f8__l13,__local_f8__3,__local_f8__l12,__local_f8__4 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__l18 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f8 -> mix
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
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$00,$01
; zp g2 size=4 at=auto def=true init=$01,$00,$00,$00 weight=146
; zp @4.s14 size=2 at=auto def=true init=$00,$01 weight=16 local=true
; zp gp6 size=2 at=auto def=true init=<g1,>g1 weight=9
; runtime hargs_size=32 hargs_need=10
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
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2
   JSR   fin0@run0
   LDA   z3+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run1
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   STA   __zpabi_mix__v_1
   LDA   ga4+2
   STA   __zpabi_mix__v_2
   LDA   ga4+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga4+4
   STA   __zpabi_mix__v_0
   LDA   ga4+5
   STA   __zpabi_mix__v_1
   LDA   ga4+6
   STA   __zpabi_mix__v_2
   LDA   ga4+7
   JSR   fin1@run2
   LDA   ga4+8
   STA   __zpabi_mix__v_0
   LDA   ga4+9
   STA   __zpabi_mix__v_1
   LDA   ga4+10
   STA   __zpabi_mix__v_2
   LDA   ga4+11
   JSR   fin1@run2
   LDA   ga4+12
   STA   __zpabi_mix__v_0
   LDA   ga4+13
   STA   __zpabi_mix__v_1
   LDA   ga4+14
   STA   __zpabi_mix__v_2
   LDA   ga4+15
   JSR   fin1@run2
   LDA   gs5+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   STA   __zpabi_mix__v_1
   JMP   mix

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs5+3
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

f8:
   SUBROUTINE

   LDA   #$00
   STA   __local_f8__l12
   STA   __local_f8__4
   STA   __local_f8__w17
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_f8__3
   LDA   __local_f8__w17
   BEQ   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   ga4
   AND   #$07
   CMP   #$02
   BEQ   .case@2
   CMP   #$07
   BEQ   .case@3
.case@2:
.case@3:
   LDA   __local_f8__3
   AND   __local_f8__3
   AND   #$07
   BNE   .lb_skip@0
   JMP   .switch@5_break
.lb_skip@0:
   CMP   #$01
   BEQ   .case@7
   CMP   #$04
   BEQ   .case@8
   JMP   .switch@5_break
.case@7:
   LDA   z3+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run1
.case@8:
   LDA   gs5+1
   STA   HARGS
   LDA   gs5+2
   STA   HARGS+1
   LDA   z3
   STA   HARGS+2
   LDA   z3+1
   STA   HARGS+3
   JSR   mul16
   LDY   #$00
   LDA   (gp6),Y
   STA   __local_f8__0
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f8__w17
   LDA   z3
   AND   __local_f8__0
   STA   __local_f8__0
   LDA   z3+1
   AND   __local_f8__w17
   STA   __local_f8__w17
   LDA   HARGS+4
   AND   __local_f8__0
   STA   __local_f8__0
   LDA   HARGS+5
   AND   __local_f8__w17
   STA   __local_f8__w17
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f8__0
   STA   __zpabi_mix__v_0
   LDA   __local_f8__w17
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   @4.s14+1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f8__0
   STA   __local_f8__w17
   LDA   #$00
   EOR   @4.s14
   STA   __zpabi_mix__v_0
   LDA   #$00
   EOR   @4.s14+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   EOR   __local_f8__0
   STA   __zpabi_mix__v_2
   LDA   #$00
   EOR   __local_f8__w17
   JSR   fin1@run2
.switch@5_break:
   LDA   #$00
   STA   __local_f8__w17
   JMP   .loop@9_continue
.f8@rot@0:
   LDA   __local_f8__l12
   LDX   __local_f8__4
   STX   __local_f8__0
   SEC
   SBC   #$05
   BCS   .carry_inc@0
   DEC   __local_f8__0
.carry_inc@0:
   STA   __local_f8__l12
   LDA   __local_f8__0
   STA   __local_f8__4
   LDA   __local_f8__1
   STA   __local_f8__w17
.loop@9_continue:
   CLC
   ADC   #$01
   STA   __local_f8__1
   LDA   __local_f8__w17
   CMP   #$03
   BCC   .f8@rot@0
   LDA   __local_f8__3
   STA   __local_f8__w17
   JMP   .loop@0_continue
.loop@0_break:
   LDA   g2
   LDX   g2+1
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l18
   LDA   gs5+3
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   gs5+3
   LDA   gs5+1
   LDX   gs5+2
   STX   __local_main__0
   ORA   #$81
   STA   __local_main__1
   STA   gs5+1
   ORA   __local_main__0
   BNE   .lb_skip@2
   JMP   .if_end@0
.lb_skip@2:
   LDA   __local_main__1
   STA   ga4+4
   LDA   #$00
   STA   ga4+5
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   ga4+6
   STA   ga4+7
   LDA   __local_main__1
   EOR   #$81
   LDY   #$00
   STY   gs5+2
   STA   gs5+1
   AND   #$07
   BEQ   .case@11
   CMP   #$01
   BEQ   .case@13
   CMP   #$03
   BEQ   .case@17
   CMP   #$05
   BEQ   .case@18
   BNE   .default@19
.case@11:
   LDA   z3
   AND   #$1F
   EOR   #$10
   SEC
   SBC   #$10
   STA   g1
   STY   gs5+3
   INC   g2
   BNE   .inc_done@0
   INC   g2+1
   BNE   .inc_done@0
   INC   g2+2
   BNE   .inc_done@0
   INC   g2+3
.inc_done@0:
   LDA   g2
   ORA   #$02
   JSR   fin0@run0
   JMP   .switch@10_break
.case@13:
   LDA   __local_main__l18
   STA   __local_main__l18
   LDY   #$00
   LDA   (gp6),Y
   STA   __local_main__1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_main__0
   LDA   #$00
   CMP   __local_main__0
   BNE   .cmp_end@19
   LDA   __local_main__l18
   CMP   __local_main__1
   BEQ   .cmp_true@18
   LDA   #$00
   BEQ   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   AND   #$07
   CMP   #$06
.case@17:
.case@18:
.default@19:
.switch@10_break:
.if_end@0:
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
   DC.B  $FF

ga4:
   DC.L  $00000100
   DC.L  $00000001
   DC.L  $7FFFFFFF
   DC.L  $7FFFFFFF

gs5:
   DS.B  1
   DC.W  $007F
   DC.B  $E8

gp7:
   DC.W  z3

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   STA   z3
   STA   @4.s14
   LDA   #$01
   STA   g2
   STA   z3+1
   STA   @4.s14+1
   LDA   #<g1
   STA   gp6
   LDA   #>g1
   STA   gp6+1
   RTS
