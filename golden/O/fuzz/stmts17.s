SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
g3	EQU	$2A
g2	EQU	$2C
__local_f13__0	EQU	$80
__zpabi_f7__p8_0	EQU	$80
__local_f13__1	EQU	$81
__zpabi_f7__p8_1	EQU	$81
__local_f13__2	EQU	$82
__zpabi_f7__p8_2	EQU	$82
__zpabi_f7__p8_3	EQU	$83
__zpabi_f7__p8_4	EQU	$84
__zpabi_f7__p8_5	EQU	$85
__zpabi_f7__p8_6	EQU	$86
__zpabi_f7__p8_7	EQU	$87
__zpabi_f7__pp9_0	EQU	$88
__zpabi_f7__pp9_1	EQU	$89
__zpabi_mix__v_0	EQU	$8A
__zpabi_mix__v_1	EQU	$8B
__zpabi_mix__v_2	EQU	$8C
__zpabi_mix__v_3	EQU	$8D
__local_main__w21	EQU	$8E
__local_f7__1	EQU	$8F
__local_f7__2	EQU	$90
__local_main__1	EQU	$90
__local_f7__3	EQU	$91
__local_main__2	EQU	$91
__local_f7__i11	EQU	$92
__local_main__3	EQU	$92
__local_f7__w12	EQU	$93
__local_main__4	EQU	$93
__local_main__l19	EQU	$94
__local_main__l18	EQU	$95
__local_main__5	EQU	$96
__local_main__6	EQU	$97
__local_main__7	EQU	$98
__local_mix__0	EQU	$99
__local_mix__1	EQU	$9A
__local_mix__2	EQU	$9B
__local_mix__3	EQU	$9C

; @zp-link-meta-begin
; def f13 params= locals=__local_f13__0,__local_f13__1,__local_f13__2,__local_f13__3 indirect=false in_cycle=false
; def f7 params=__zpabi_f7__p8_0,__zpabi_f7__p8_1,__zpabi_f7__p8_2,__zpabi_f7__p8_3,__zpabi_f7__p8_4,__zpabi_f7__p8_5,__zpabi_f7__p8_6,__zpabi_f7__p8_7,__zpabi_f7__pp9_0,__zpabi_f7__pp9_1 locals=__local_f7__0,__local_f7__1,__local_f7__2,__local_f7__3,__local_f7__i11,__local_f7__w12 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__w21,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__l19,__local_main__l18,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run3 section=.text local=true zero=false
; item f7 section=.text local=false zero=false
; item f13 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=4 at=auto def=true init=$00,$80,$00,$00
; zp g3 size=2 at=auto def=true init=$FF,$FF weight=29
; zp g2 size=4 at=auto def=true init=$00,$00,$01,$00 weight=28
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
   JSR   fin0@run1
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   fin0@run2
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run0
   LDA   g3+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run1
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run2:
   SUBROUTINE

   LDA   g1+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   STA   __zpabi_mix__v_1
   LDA   ga5+2
   STA   __zpabi_mix__v_2
   LDA   ga5+3
   JSR   fin0@run0
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   STA   __zpabi_mix__v_1
   LDA   ga5+6
   STA   __zpabi_mix__v_2
   LDA   ga5+7
   JSR   fin0@run0
   LDA   ga5+8
   STA   __zpabi_mix__v_0
   LDA   ga5+9
   STA   __zpabi_mix__v_1
   LDA   ga5+10
   STA   __zpabi_mix__v_2
   LDA   ga5+11
   JSR   fin0@run0
   LDA   ga5+12
   STA   __zpabi_mix__v_0
   LDA   ga5+13
   STA   __zpabi_mix__v_1
   LDA   ga5+14
   STA   __zpabi_mix__v_2
   LDA   ga5+15
   STA   __zpabi_mix__v_3
   JMP   mix

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
   JSR   fin0@run1
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
   JSR   fin0@run1
   JSR   fin2@run3
   STA   __zpabi_mix__v_1
   JMP   mix

fin2@run3:
   SUBROUTINE

   LDA   gs6+2
   STA   __zpabi_mix__v_0
   LDA   gs6+3
   RTS

f7:
   SUBROUTINE

   LDA   #$01
   STA   __local_f7__i11
.loop@0_start:
   LDA   __local_f7__i11
   BEQ   .loop@0_break
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __local_f7__3
   LDA   g2+2
   STA   __local_f7__2
   LDA   g2+3
   STA   __local_f7__1
   LDA   __zpabi_mix__v_0
   SEC
   SBC   #$01
   STA   __zpabi_mix__v_0
   LDA   __local_f7__3
   SBC   #$00
   STA   __local_f7__3
   LDA   __local_f7__2
   SBC   #$00
   STA   __local_f7__2
   BCS   .carry_inc@0
   DEC   __local_f7__1
.carry_inc@0:
   LDA   __zpabi_mix__v_0
   STA   g2
   LDA   __local_f7__3
   STA   g2+1
   LDA   __local_f7__2
   STA   g2+2
   LDA   __local_f7__1
   STA   g2+3
   LDA   gs6+1
   SEC
   SBC   #$01
   STA   __zpabi_mix__v_0
   STA   gs6+1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   DEC   __local_f7__i11
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   STA   __local_f7__w12
.loop@1_start:
   LDX   #$05
.shift_loop@10:
   ASL   gs6+2
   ROL   gs6+3
   DEX
   BNE   .shift_loop@10
   LDX   #$05
.shift_loop@12:
   LDA   g3+1
   ASL
   ROR   g3+1
   ROR   g3
   DEX
   BNE   .shift_loop@12
   LDA   g3
   AND   #$07
   CMP   #$01
   BEQ   .case@3
   CMP   #$03
   BEQ   .case@4
   CMP   #$04
   BEQ   .case@5
   CMP   #$05
   BNE   .lb_skip@3
   JMP   .case@6
.lb_skip@3:
   JMP   .default@7
.case@3:
   JSR   fin2@run3
   STA   __local_f7__1
   INC   __zpabi_mix__v_0
   BNE   .inc_done@0
   INC   __local_f7__1
.inc_done@0:
   LDA   __zpabi_mix__v_0
   STA   gs6+2
   LDA   __local_f7__1
   STA   gs6+3
   LDA   __zpabi_mix__v_0
   ORA   __local_f7__1
   BEQ   .lb_skip@2
   JMP   .switch@2_break
.lb_skip@2:
   LDA   ga5+4
   SEC
   SBC   #$05
   STA   ga5+4
   LDA   ga5+5
   SBC   #$00
   STA   ga5+5
   LDA   ga5+6
   SBC   #$00
   STA   ga5+6
   BCC   .lb_skip@1
   JMP   .switch@2_break
.lb_skip@1:
   DEC   ga5+7
   JMP   .switch@2_break
.case@4:
.case@5:
   LDA   __local_f7__w12
   AND   #$03
   ASL
   ASL
   TAX
   LDA   ga5,X
   STA   __local_f7__i11
   LDA   ga5+1,X
   STA   __local_f7__3
   LDA   ga5+2,X
   STA   __local_f7__2
   LDA   ga5+3,X
   STA   __zpabi_mix__v_0
   LDA   __local_f7__i11
   ORA   __local_f7__3
   ORA   __local_f7__2
   ORA   __zpabi_mix__v_0
   BEQ   .lnot_true@14
   LDA   #$00
   BEQ   .lnot_end@15
.lnot_true@14:
   LDA   #$01
.lnot_end@15:
   STA   __local_f7__1
   LDA   #$00
   CMP   __zpabi_mix__v_0
   BNE   .cmp_true@16
   CMP   __local_f7__2
   BNE   .cmp_true@16
   CMP   __local_f7__3
   BNE   .cmp_true@16
   LDA   __local_f7__w12
   CMP   __local_f7__i11
   BNE   .cmp_true@16
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   __zpabi_mix__v_0
   LDA   __local_f7__1
   SEC
   SBC   __zpabi_mix__v_0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   JSR   fin0@run1
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
.case@6:
.default@7:
.switch@2_break:
   INC   __local_f7__w12
   BNE   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
   LDA   gs6+1
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   TAX
   LDA   __zpabi_mix__v_0
   RTS

f13:
   SUBROUTINE

   LDA   gs6+1
   SEC
   SBC   #$01
   STA   __local_f13__1
   STA   gs6+1
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_f13__0
   LDA   __local_f13__1
   EOR   g3
   STA   __local_f13__1
   LDA   __local_f13__0
   EOR   g3+1
   ORA   __local_f13__1
   BEQ   .if_end@2
   LDA   gs6+1
   STA   __local_f13__1
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_f13__0
   LDA   __local_f13__1
   EOR   g3
   STA   g3
   LDA   __local_f13__0
   EOR   g3+1
   STA   g3+1
   LDA   g2
   LDX   g2+1
   STX   __local_f13__2
   LDX   g2+2
   STX   __local_f13__1
   LDX   g2+3
   STX   __local_f13__0
   SEC
   SBC   #$02
   STA   HARGS+8
   LDA   __local_f13__2
   SBC   #$00
   STA   HARGS+9
   LDA   __local_f13__1
   SBC   #$00
   STA   HARGS+10
   LDA   __local_f13__0
   SBC   #$00
   STA   HARGS+11
   RTS
.if_end@2:
   LDA   #$FF
   STA   HARGS+8
   LDY   #$FF
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l18
   STA   __local_main__5
   LDA   #$00
   STA   __local_main__6
   STA   __local_main__7
   LDA   g1
   EOR   #$01
   STA   __zpabi_mix__v_0
   LDA   g1+1
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   JSR   fin0@run1
   JSR   fin0@run2
   LDA   #$FF
   STA   __local_main__l19
   LDA   #$00
   STA   __local_main__w21
.loop@14_continue:
   CLC
   ADC   #$01
   STA   __local_main__4
   LDA   __local_main__w21
   CMP   #$04
   BCC   .lb_skip@5
   JMP   .loop@14_break
.lb_skip@5:
   LDA   #$00
   STA   __local_main__w21
.loop@15_continue:
   CLC
   ADC   #$01
   STA   __local_main__3
   LDA   __local_main__w21
   CMP   #$02
   BCC   .lb_skip@4
   JMP   .loop@15_break
.lb_skip@4:
   LDA   __local_main__l18
   LDX   __local_main__5
   STX   __local_main__1
   LDX   __local_main__6
   STX   __zpabi_mix__v_0
   LDX   __local_main__7
   STX   __local_main__w21
   EOR   #$FF
   STA   __local_main__l18
   LDX   __local_main__1
   STX   __local_main__5
   LDX   __zpabi_mix__v_0
   STX   __local_main__6
   LDX   __local_main__w21
   STX   __local_main__7
   AND   #$07
   CMP   #$02
   BEQ   .case@17
   CMP   #$04
   BEQ   .case@18
   BNE   .default@19
.case@17:
.case@18:
   LDA   gs6+2
   BNE   .carry_inc@2
   DEC   gs6+3
.carry_inc@2:
   DEC   gs6+2
   LDA   __local_main__l19
   CLC
   ADC   #$03
   STA   __local_main__l19
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   __zpabi_mix__v_1
   JSR   fin0@run1
   LDA   __local_main__l19
   STA   __zpabi_mix__v_0
   JSR   mix
   JMP   .switch@16_break
.default@19:
   LDA   __local_main__l18
   LDX   __local_main__5
   STX   __local_main__2
   LDX   __local_main__6
   STX   __local_main__1
   LDX   __local_main__7
   STX   __zpabi_mix__v_0
   SEC
   SBC   #$01
   STA   __local_main__w21
   LDA   __local_main__2
   SBC   #$00
   STA   __local_main__2
   LDA   __local_main__1
   SBC   #$00
   STA   __local_main__1
   BCS   .carry_inc@3
   DEC   __zpabi_mix__v_0
.carry_inc@3:
   LDA   __local_main__w21
   STA   __local_main__l18
   LDA   __local_main__2
   STA   __local_main__5
   LDA   __local_main__1
   STA   __local_main__6
   LDA   __zpabi_mix__v_0
   STA   __local_main__7
.switch@16_break:
   LDA   gs6+2
   BNE   .carry_inc@4
   DEC   gs6+3
.carry_inc@4:
   DEC   gs6+2
   LDA   #$00
   STA   gs6+2
   LDA   #$01
   STA   gs6+3
   LDA   __local_main__3
   STA   __local_main__w21
   JMP   .loop@15_continue
.loop@15_break:
   LDA   g2
   LDX   g2+1
   STX   __local_main__1
   LDX   g2+2
   LDX   g2+3
   LDX   gs6+2
   STX   __zpabi_mix__v_0
   LDX   gs6+3
   STX   __local_main__w21
   SBC   __zpabi_mix__v_0
   STA   g3
   LDA   __local_main__1
   SBC   __local_main__w21
   STA   g3+1
   LDA   __local_main__4
   STA   __local_main__w21
   JMP   .loop@14_continue
.loop@14_break:
   LDX   #$05
.shift_loop@37:
   LDA   gs6+1
   ASL
   ROR   gs6+1
   DEX
   BNE   .shift_loop@37
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
   DS.B  2

ga5:
   DC.L  $0000FFFF
   DC.L  $00000003
   DS.B  4
   DC.L  $7FFFFFFF

gs6:
   DS.B  1
   DC.B  $7F
   DC.W  $69CE

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   STA   g2+1
   STA   g2+3
   STA   z4
   STA   z4+2
   STA   z4+3
   LDA   #$01
   STA   g2+2
   LDA   #$FF
   STA   g3
   STA   g3+1
   LDA   #$80
   STA   z4+1
   RTS
