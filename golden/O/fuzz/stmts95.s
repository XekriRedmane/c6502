SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$28
g2	EQU	$2A
g1	EQU	$2C
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__0	EQU	$84
__local_main__1	EQU	$85
__local_f8__2	EQU	$86
__local_main__2	EQU	$86
__local_f8__p9	EQU	$87
__local_main__l17	EQU	$88
__local_f12__0	EQU	$89
__local_f12__1	EQU	$8A
__local_f12__2	EQU	$8B
__local_mix__0	EQU	$8B
__local_mix__1	EQU	$8C
__local_f12__i16	EQU	$8D
__local_mix__2	EQU	$8D
__local_f12__pp13	EQU	$8E
__local_mix__3	EQU	$8E
__local_f12__4	EQU	$8F

; @zp-link-meta-begin
; def f12 params=__zpabi_f12__pp13_0,__zpabi_f12__pp13_1 locals=__local_f12__0,__local_f12__1,__local_f12__2,__local_f12__3,__local_f12__i16,__local_f12__pp13,__local_f12__4 indirect=false in_cycle=false param_regs=A,X
; def f8 params=__zpabi_f8__p9 locals=__local_f8__0,__local_f8__1,__local_f8__2,__local_f8__p9,__local_f8__3 indirect=false in_cycle=false param_regs=A
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l17 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f12
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run6 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin0@tail0@run7 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f8@run5 section=.text local=true zero=false
; item f8@run4 section=.text local=true zero=false
; item f8@run1 section=.text local=true zero=false
; item f12 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
; item @2.s11 section=.data local=true zero=false
; item @5.s15 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$01,$00
; zp z4 size=2 at=auto def=true init=$7F,$00
; zp g2 size=2 at=auto def=true init=$00,$01 weight=19
; zp g1 size=2 at=auto def=true init=$80,$00 weight=18
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
   JSR   fin0@run6
   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   JSR   fin0@run2
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run0
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run0
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   JMP   fin0@tail0

fin0@run6:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run6
   JMP   mix

fin0@tail0:
   SUBROUTINE

   JSR   fin0@tail0@run7
   JMP   mix

fin0@tail0@run7:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga5+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run6
   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   JSR   fin0@run2
   LDA   ga5+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run6
   LDA   ga5+2
   STA   __zpabi_mix__v_0
   LDA   ga5+3
   JSR   fin0@run2
   LDA   ga5+5
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run6
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   JSR   fin0@run2
   LDA   ga5+7
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   JSR   fin0@run6
   LDA   ga5+6
   STA   __zpabi_mix__v_0
   LDA   ga5+7
   STA   __zpabi_mix__v_1
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+2
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   JSR   fin0@run6
   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   JSR   fin0@run2
   LDA   gs6+3
   STA   __zpabi_mix__v_0
   LDA   gs6+4
   JMP   fin0@tail0

f8:
   SUBROUTINE

   STA   __local_f8__p9
   LDA   gs6+2
   AND   #$01
   STA   __zpabi_mix__v_1
   LDA   gs6+1
   STA   ga5+6
   LDA   __zpabi_mix__v_1
   EOR   #$01
   SEC
   SBC   #$01
   STA   ga5+7
   LDA   z4
   ORA   #$03
   STA   z4
   CLC
   ADC   g1
   STA   __zpabi_mix__v_0
   LDA   z4+1
   ADC   g1+1
   JSR   fin0@tail0@run7
   JSR   mix
   LDA   ga5
   STA   __zpabi_mix__v_1
   LDA   ga5+1
   STA   __local_f8__2
   LDA   __zpabi_mix__v_1
   SEC
   SBC   #$03
   BCS   .carry_inc@0
   DEC   __local_f8__2
.carry_inc@0:
   AND   #$FF
   STA   __zpabi_mix__v_0
   LDA   __local_f8__2
   AND   #$01
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   STA   ga5
   LDA   __zpabi_mix__v_1
   EOR   #$01
   SEC
   SBC   #$01
   STA   ga5+1
   LDA   __local_f8__p9
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f8__2
   JSR   f8@run1
   LDA   __local_f8__p9
   AND   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   __local_f8__2
   AND   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   JSR   fin0@run0
   JSR   f8@run1
   LDA   __zpabi_mix__v_0
   ORA   #$03
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   JSR   f8@run5
   LDY   #$00
   STA   (DPTR),Y
   JSR   f8@run4
   LDA   __zpabi_mix__v_1
   LDY   #$01
   STA   (DPTR),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   JSR   fin0@run0
   LDA   @2.s11
   STA   __zpabi_mix__v_0
   LDA   @2.s11+1
   STA   __zpabi_mix__v_1
   INC   @2.s11
   BNE   .inc_done@0
   INC   @2.s11+1
   BNE   .inc_done@0
   INC   @2.s11+2
   BNE   .inc_done@0
   INC   @2.s11+3
.inc_done@0:
   LDA   gs6+3
   SEC
   SBC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   gs6+4
   SBC   __zpabi_mix__v_1
   JSR   f8@run5
   STA   z3
   LDA   __zpabi_mix__v_1
   STA   z3+1
   LDX   #$00
   LDA   #$0A
   RTS

f8@run5:
   SUBROUTINE

   AND   #$01
   EOR   #$01
   STA   __zpabi_mix__v_1
   DEC   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   RTS

f8@run4:
   SUBROUTINE

   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   RTS

f8@run1:
   SUBROUTINE

   JSR   f8@run4
   LDY   #$00
   LDA   (DPTR),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (DPTR),Y
   STA   __zpabi_mix__v_1
   RTS

f12:
   SUBROUTINE

   STA   __local_f12__pp13
   STX   __local_f12__4
   LDA   #$00
   STA   __local_f12__i16
.loop@0_start:
   LDA   __local_f12__i16
   CMP   #$02
   BCC   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   #$FF
   STA   g1
   LDA   #$00
   STA   g1+1
   LDA   gs6+1
   EOR   #$03
   STA   gs6+1
   AND   #$07
   CMP   #$01
   BEQ   .case@2
   CMP   #$02
   BEQ   .case@3
   CMP   #$03
   BEQ   .case@4
   CMP   #$04
   BNE   .lb_skip@0
   JMP   .switch@1_break
.lb_skip@0:
   JMP   .default@6
.case@2:
   LDA   g2
   LDX   g2+1
   AND   #$05
   STA   g2
   LDA   #$00
   STA   g2+1
   JMP   .switch@1_break
.case@3:
   LDA   gs6+3
   STA   __local_f12__0
   LDA   gs6+4
   STA   __local_f12__2
   LDA   __local_f12__0
   BNE   .carry_inc@1
   DEC   __local_f12__2
.carry_inc@1:
   DEC   __local_f12__0
   LDA   __local_f12__0
   STA   gs6+3
   LDA   __local_f12__2
   STA   gs6+4
   LDA   __local_f12__i16
   EOR   __local_f12__0
   STA   __local_f12__1
   LDA   #$00
   EOR   __local_f12__2
   AND   #$01
   EOR   #$01
   LDX   __local_f12__1
   STX   g1
   SEC
   SBC   #$01
   STA   g1+1
   JMP   .loop@0_continue
.case@4:
   LDA   z3
   STA   __local_f12__0
   LDA   z3+1
   STA   __local_f12__2
   INC   __local_f12__0
   BNE   .inc_done@1
   INC   __local_f12__2
.inc_done@1:
   LDA   __local_f12__0
   STA   __local_f12__1
   LDA   __local_f12__2
   AND   #$01
   EOR   #$01
   STA   __local_f12__0
   DEC   __local_f12__0
   LDA   __local_f12__1
   STA   z3
   LDA   __local_f12__0
   STA   z3+1
   STA   @5.s15+1
   LDA   __local_f12__1
   STA   @5.s15
   LDA   __local_f12__0
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   @5.s15+2
   STA   @5.s15+3
   STA   @5.s15+4
   STA   @5.s15+5
   STA   @5.s15+6
   STA   @5.s15+7
   JMP   .switch@1_break
.default@6:
   LDA   ga5+6
   ORA   ga5+7
   BEQ   .lnot_true@24
   LDA   #$00
   BEQ   .lnot_end@25
.lnot_true@24:
   LDA   #$01
.lnot_end@25:
   STA   z4
   LDA   #$00
   STA   z4+1
.switch@1_break:
   LDA   __local_f12__i16
   AND   #$07
   CMP   #$03
   BNE   .default@9
   LDA   ga5+6
   AND   ga5
   STA   __local_f12__1
   LDA   ga5+7
   AND   ga5+1
   ORA   __local_f12__1
   BNE   .switch@7_break
.default@9:
.switch@7_break:
.loop@0_continue:
   INC   __local_f12__i16
   JMP   .loop@0_start
.loop@0_break:
   LDA   @5.s15
   STA   __local_f12__0
   LDY   #$00
   LDA   (__local_f12__pp13),Y
   SBC   __local_f12__0
   RTS

main:
   SUBROUTINE

   LDA   #$03
   STA   __local_main__l17
   INC   __local_main__l17
   LDA   z3
   STA   __local_main__2
   LDA   z3+1
   ASL   __local_main__2
   ROL
   ASL   __local_main__2
   ROL
   ASL   __local_main__2
   ROL
   ASL   __local_main__2
   ROL
   LDX   __local_main__2
   STX   __local_main__1
   AND   #$01
   EOR   #$01
   STA   __local_main__0
   DEC   __local_main__0
   LDA   __local_main__1
   STA   z3
   LDA   __local_main__0
   STA   z3+1
   LDA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@3
   LDA   z4
   BNE   .carry_inc@2
   DEC   z4+1
.carry_inc@2:
   DEC   z4
   LDA   __local_main__l17
   AND   #$81
   STA   __local_main__l17
   JSR   main@run3
.if_end@3:
   LDX   #>__local_main__l17
   LDA   #<__local_main__l17
   JSR   f12
   JSR   main@run3
   LDA   gs6+3
   STA   __local_main__0
   LDA   gs6+4
   LSR
   ROR   __local_main__0
   LSR
   ROR   __local_main__0
   LSR
   STA   __local_main__1
   ROR   __local_main__0
   STA   gs6+4
   LDA   __local_main__0
   STA   gs6+3
   STA   g2
   LDA   __local_main__1
   STA   g2+1
   LDA   g2
   LDA   g2+1
   LDA   z3
   LDA   z3+1
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   fin0@run0

acc:
   DS.B  2

ga5:
   DC.W  $FF00
   DC.W  $0080
   DC.W  $0001
   DC.W  $00FF

gs6:
   DS.B  1
   DC.W  $0080
   DC.W  $0003

gp7:
   DC.W  g1

@2.s11:
   DC.L  $00000001

@5.s15:
   DC.L  $0000000C
   DC.L  $00000000

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g2
   STA   z3+1
   STA   z4+1
   LDA   #$80
   STA   g1
   LDA   #$01
   STA   g2+1
   STA   z3
   LDA   #$7F
   STA   z4
   RTS
