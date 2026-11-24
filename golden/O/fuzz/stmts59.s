SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
gp6	EQU	$26
g2	EQU	$28
gp5	EQU	$2A
g1	EQU	$2C
__zpabi_f7__p8_0	EQU	$80
__zpabi_f7__p8_1	EQU	$81
__zpabi_f7__p8_2	EQU	$82
__zpabi_f7__p8_3	EQU	$83
__zpabi_f7__p8_4	EQU	$84
__zpabi_f7__p8_5	EQU	$85
__zpabi_f7__p8_6	EQU	$86
__zpabi_f7__p8_7	EQU	$87
__zpabi_f7__p9	EQU	$88
__zpabi_f7__p10	EQU	$89
__zpabi_mix__v_0	EQU	$8A
__zpabi_mix__v_1	EQU	$8B
__zpabi_mix__v_2	EQU	$8C
__zpabi_mix__v_3	EQU	$8D
__local_main__0	EQU	$8F
__local_main__1	EQU	$90
__local_main__2	EQU	$91
__local_main__3	EQU	$92
__local_main__l17	EQU	$93
__local_main__4	EQU	$94
__local_f7__w14	EQU	$95
__local_f7__w16	EQU	$96
__local_f7__l11_0	EQU	$9A
__local_mix__0	EQU	$9B
__local_mix__1	EQU	$9C
__local_mix__2	EQU	$9D
__local_mix__3	EQU	$9E

; @zp-link-meta-begin
; def f7 params=__zpabi_f7__p8_0,__zpabi_f7__p8_1,__zpabi_f7__p8_2,__zpabi_f7__p8_3,__zpabi_f7__p8_4,__zpabi_f7__p8_5,__zpabi_f7__p8_6,__zpabi_f7__p8_7,__zpabi_f7__p9,__zpabi_f7__p10 locals=__local_f7__w14,__local_f7__w16,__local_f7__0,__local_f7__p10,__local_f7__l11_1,__local_f7__l11_0 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def main params= locals=__local_main__w20,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l17,__local_main__4 indirect=false in_cycle=false
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
; item fin0@run3 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run1 section=.text local=true zero=false
; item f7 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga3 section=.data local=false zero=false
; item gs4 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp gp6 size=2 at=auto def=true init=<g1,>g1 weight=64
; zp g2 size=2 at=auto def=true init=$87,$EE weight=35
; zp gp5 size=2 at=auto def=true init=<g2,>g2 weight=11
; zp g1 size=1 at=auto def=true init=$02 weight=4
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
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run3
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
   LDA   ga3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run0
   LDA   ga3
   JSR   fin0@run2
   LDA   ga3+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run0
   LDA   ga3+1
   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga3+2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run0
   LDA   ga3+2
   JSR   fin0@run2
   LDA   ga3+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run0
   LDA   ga3+3
   JSR   fin0@run2
   LDA   gs4+1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run0
   LDA   gs4+1
   JSR   fin0@run2
   JSR   fin1@run1
   STA   __zpabi_mix__v_2
   LDA   gs4+5
   STA   __zpabi_mix__v_3
   JMP   mix

fin1@run1:
   SUBROUTINE

   LDA   gs4+2
   STA   __zpabi_mix__v_0
   LDA   gs4+3
   STA   __zpabi_mix__v_1
   LDA   gs4+4
   RTS

f7:
   SUBROUTINE

   LDA   #$FA
   STA   __local_f7__w14
   LDA   #$FD
   STA   __local_f7__w16
   JSR   fin1@run1
   STA   __local_f7__w16
   LDA   gs4+5
   STA   __local_f7__w14
   LDA   __zpabi_mix__v_0
   SEC
   SBC   #$01
   STA   gs4+2
   LDA   __zpabi_mix__v_1
   SBC   #$00
   STA   gs4+3
   LDA   __local_f7__w16
   SBC   #$00
   STA   gs4+4
   LDA   __local_f7__w14
   SBC   #$00
   STA   gs4+5
   LDA   __local_f7__w16
   STA   __zpabi_mix__v_2
   LDA   __local_f7__w14
   JSR   fin0@run3
   LDY   #$01
   LDA   (gp5),Y
   STA   __local_f7__w16
   DEY
   LDA   (gp5),Y
   SEC
   SBC   #$01
   STA   __local_f7__w14
   BCS   .carry_inc@0
   DEC   __local_f7__w16
.carry_inc@0:
   LDY   #$00
   STA   (gp5),Y
   LDA   __local_f7__w16
   INY
   STA   (gp5),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f7__w14
   STA   __zpabi_mix__v_0
   LDA   __local_f7__w16
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   #$00
   STA   __zpabi_mix__v_0
   JSR   fin0@run0
   JSR   mix
   LDA   #$00
   STA   __local_f7__l11_0
   STA   __local_f7__w14
   LDA   __zpabi_f7__p10
   STA   __zpabi_mix__v_0
   JMP   .loop@0_continue
.f7@rot@1:
   LDY   #$01
   LDA   (gp5),Y
   DEY
   ORA   (gp5),Y
   BEQ   .loop@0_break
   LDA   __zpabi_mix__v_0
   ASL
   ASL
   ASL
   ASL
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __local_f7__w16
   CLC
   JMP   .loop@2_continue
.f7@rot@0:
   LDA   __local_f7__l11_0
   AND   #$81
   STA   __local_f7__l11_0
   LDA   __local_f7__w14
   STA   __local_f7__w16
.loop@2_continue:
   ADC   #$01
   STA   __local_f7__w14
   LDA   __local_f7__w16
   CMP   #$03
   BCC   .f7@rot@0
   LDA   __zpabi_mix__v_1
   STA   __local_f7__w14
.loop@0_continue:
   LDA   __local_f7__w14
   CLC
   ADC   #$01
   STA   __zpabi_mix__v_1
   LDA   __local_f7__w14
   CMP   #$03
   BCC   .f7@rot@1
.loop@0_break:
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   LDA   #$07
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDX   HARGS+5
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l17
   STA   __local_main__4
   LDA   g1
   SEC
   SBC   #$5A
   STA   g1
   LDA   __local_main__l17
   LDA   __local_main__4
   LDA   #$00
   STA   __zpabi_mix__v_3
.loop@3_continue:
   CLC
   ADC   #$01
   STA   __local_main__3
   LDA   __zpabi_mix__v_3
   BEQ   .lb_skip@0
   JMP   .loop@3_break
.lb_skip@0:
   LDA   g2
   SEC
   SBC   #$01
   STA   g2
   BCS   .carry_inc@1
   DEC   g2+1
.carry_inc@1:
   ORA   g2+1
   BEQ   .cond_else@2
   LDA   g2
   STA   __local_main__2
   LDA   g2+1
   STA   __local_main__1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_main__0
   STA   __zpabi_mix__v_3
   JMP   .cond_end@3
.cond_else@2:
   LDA   gs4+2
   STA   __local_main__2
   LDA   gs4+3
   STA   __local_main__1
   LDA   gs4+4
   STA   __local_main__0
   LDA   gs4+5
   STA   __zpabi_mix__v_3
   LDA   #$F2
   CLC
   ADC   __local_main__2
   STA   __local_main__2
   LDA   #$00
   ADC   __local_main__1
   STA   __local_main__1
   LDA   #$00
   ADC   __local_main__0
   STA   __local_main__0
   LDA   #$00
   ADC   __zpabi_mix__v_3
   STA   __zpabi_mix__v_3
.cond_end@3:
   LDA   __local_main__2
   STA   __zpabi_mix__v_0
   LDA   __local_main__1
   STA   __zpabi_mix__v_1
   LDA   __local_main__0
   STA   __zpabi_mix__v_2
   JSR   mix
   INC   gs4+2
   BNE   .inc_done@0
   INC   gs4+3
   BNE   .inc_done@0
   INC   gs4+4
   BNE   .inc_done@0
   INC   gs4+5
.inc_done@0:
   LDA   __local_main__3
   STA   __zpabi_mix__v_3
   JMP   .loop@3_continue
.loop@3_break:
   LDY   #$00
   LDA   (gp5),Y
   EOR   #$FF
   STA   __local_main__1
   INY
   LDA   (gp5),Y
   EOR   #$FF
   STA   __local_main__0
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __zpabi_f7__p8_2
   STA   __zpabi_f7__p8_3
   STA   __zpabi_f7__p8_4
   STA   __zpabi_f7__p8_5
   STA   __zpabi_f7__p8_6
   STA   __zpabi_f7__p8_7
   LDA   g1
   CLC
   ADC   ga3+2
   STA   __zpabi_f7__p9
   LDA   __local_main__1
   STA   __zpabi_f7__p8_0
   LDA   __local_main__0
   STA   __zpabi_f7__p8_1
   LDA   #$00
   STA   __zpabi_f7__p10
   JSR   f7
   LDA   __local_main__l17
   STA   __zpabi_mix__v_3
   LDA   __local_main__4
   STA   __local_main__2
   LDA   __zpabi_mix__v_3
   SEC
   SBC   #$5A
   STA   __local_main__1
   BCS   .carry_inc@2
   DEC   __local_main__2
.carry_inc@2:
   STA   __local_main__l17
   LDA   __local_main__2
   STA   __local_main__4
   LDA   __local_main__1
   EOR   g2
   STA   __zpabi_mix__v_0
   LDA   __local_main__2
   EOR   g2+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run3
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
   DC.B  $1F, $00, $FF, $7F

gs4:
   DS.B  1
   DC.B  $80
   DC.L  $80000000

__zp_init:
   SUBROUTINE

   LDA   #$02
   STA   g1
   LDA   #$87
   STA   g2
   LDA   #$EE
   STA   g2+1
   LDA   #<g2
   STA   gp5
   LDA   #>g2
   STA   gp5+1
   LDA   #<g1
   STA   gp6
   LDA   #>g1
   STA   gp6+1
   RTS
