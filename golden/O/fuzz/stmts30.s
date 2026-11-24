SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$27
z5	EQU	$2B
g1	EQU	$2F
@8.s19	EQU	$30
g2	EQU	$32
__local_f15__l17	EQU	$80
__zpabi_mix__v_0	EQU	$80
__local_f15__0	EQU	$81
__zpabi_mix__v_1	EQU	$81
__local_f15__1	EQU	$82
__zpabi_mix__v_2	EQU	$82
__local_f15__2	EQU	$83
__zpabi_mix__v_3	EQU	$83
__local_f15__3	EQU	$84
__local_main__w24	EQU	$84
__local_f15__4	EQU	$85
__local_main__w25	EQU	$85
__local_f15__5	EQU	$86
__local_main__l22	EQU	$86
__local_f15__6	EQU	$87
__local_main__0	EQU	$87
__local_f15__i20	EQU	$88
__local_main__1	EQU	$88
__local_f15__pp16	EQU	$89
__local_main__2	EQU	$89
__local_f15__7	EQU	$8A
__local_main__3	EQU	$8A
__local_main__4	EQU	$8B
__local_main__5	EQU	$8C
__local_main__6	EQU	$8D
__local_main__7	EQU	$8E
__local_main__8	EQU	$8F
__local_main__9	EQU	$90
__local_main__10	EQU	$91
__local_main__11	EQU	$92
__local_main__12	EQU	$93
__local_main__13	EQU	$94
__local_main__14	EQU	$95
__local_main__15	EQU	$96
__local_f8__0	EQU	$97
__local_f8__1	EQU	$98
__local_f8__2	EQU	$99
__local_f8__3	EQU	$9A
__local_f8__w14	EQU	$9B
__local_mix__0	EQU	$9C
__local_mix__1	EQU	$9D
__local_mix__2	EQU	$9E
__local_mix__3	EQU	$9F

; @zp-link-meta-begin
; def f15 params=__zpabi_f15__pp16_0,__zpabi_f15__pp16_1 locals=__local_f15__l17,__local_f15__0,__local_f15__1,__local_f15__2,__local_f15__3,__local_f15__4,__local_f15__5,__local_f15__6,__local_f15__i20,__local_f15__pp16,__local_f15__7 indirect=false in_cycle=false param_regs=A,X
; def f8 params= locals=__local_f8__0,__local_f8__1,__local_f8__2,__local_f8__3,__local_f8__w14 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__w24,__local_main__w25,__local_main__l22,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f8
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run1 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f15 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=1 at=auto def=true init=$FF
; zp z4 size=4 at=auto def=true init=$FF,$7F,$00,$00
; zp z5 size=4 at=auto def=true init=$F7,$B4,$00,$CE
; zp g1 size=1 at=auto def=true init=$00 weight=11
; zp @8.s19 size=2 at=auto def=true init=$00,$80 weight=8 local=true
; zp g2 size=1 at=auto def=true init=$FF weight=3
; runtime hargs_size=32 hargs_need=24
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
   JSR   fin0@run0
   LDA   g1
   JSR   fin0@run2
   LDA   g2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run0
   LDA   g2
   JSR   fin0@run2
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
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

   JSR   fin1@run1
   LDA   ga6
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run0
   LDA   ga6
   JSR   fin0@run2
   LDA   ga6+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run0
   LDA   ga6+1
   JSR   fin0@run2
   LDA   ga6+2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run0
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   LDA   z5+2
   STA   __zpabi_mix__v_2
   LDA   z5+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+3
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run0
   LDA   ga6+3
   JSR   fin0@run2
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   LDA   gs7+3
   STA   __zpabi_mix__v_2
   LDA   gs7+4
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   gs7+9
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   JSR   fin0@run0
   LDA   gs7+9
   STA   __zpabi_mix__v_0
   JMP   mix

f8:
   SUBROUTINE

   LDA   z5
   STA   __local_f8__3
   LDA   z5+1
   STA   __local_f8__2
   LDA   z5+2
   STA   __local_f8__1
   LDA   z5+3
   STA   __local_f8__0
   INC   z5
   BNE   .inc_done@0
   INC   z5+1
   BNE   .inc_done@0
   INC   z5+2
   BNE   .inc_done@0
   INC   z5+3
.inc_done@0:
   LDA   __local_f8__3
   STA   gs7+1
   LDA   __local_f8__2
   STA   gs7+2
   LDA   __local_f8__1
   STA   gs7+3
   LDA   __local_f8__0
   STA   gs7+4
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   gs7+5
   STA   gs7+6
   STA   gs7+7
   STA   gs7+8
   LDA   #$00
   STA   __local_f8__w14
.loop@1_start:
   LDA   z4
   STA   HARGS
   LDA   z4+1
   STA   HARGS+1
   LDA   z4+2
   STA   HARGS+2
   LDA   z4+3
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   ORA   HARGS+13
   ORA   HARGS+14
   ORA   HARGS+15
   BEQ   .if_end@0
   LDA   ga6+2
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f8__2
   STA   __local_f8__1
   STA   __local_f8__0
   LDA   z4
   CLC
   ADC   ga6+2
   STA   __local_f8__3
   LDA   z4+1
   ADC   __local_f8__2
   STA   __local_f8__2
   LDA   z4+2
   ADC   __local_f8__1
   STA   __local_f8__1
   LDA   z4+3
   ADC   __local_f8__0
   STA   __local_f8__0
   LDA   #$00
   CMP   __local_f8__0
   BNE   .cmp_end@19
   CMP   __local_f8__1
   BNE   .cmp_end@19
   CMP   __local_f8__2
   BNE   .cmp_end@19
   LDA   __local_f8__w14
   CMP   __local_f8__3
   BEQ   .cmp_true@18
   LDA   #$00
   BEQ   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   STA   z3
   LDA   #$00
   STA   __zpabi_mix__v_0
   JSR   fin0@run0
   JSR   mix
   JMP   .loop@1_break
.if_end@0:
   INC   __local_f8__w14
   LDA   __local_f8__w14
   CMP   #$03
   BCS   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
.loop@1_break:
   JSR   fin1@run1
   LDA   z3
   LDX   #$00
   RTS

f15:
   SUBROUTINE

   STA   __local_f15__pp16
   STX   __local_f15__7
   LDA   #$00
   STA   __local_f15__l17
   LDA   #$01
   STA   __local_f15__0
   INC   gs7+9
   LDA   gs7+9
   AND   #$07
   CMP   #$01
   BEQ   .case@3
   CMP   #$03
   BNE   .lb_skip@2
   JMP   .case@14
.lb_skip@2:
   JMP   .switch@2_break
.case@3:
   LDA   #$00
   STA   __local_f15__i20
.loop@4_start:
   LDA   __local_f15__i20
   CMP   #$04
   BCC   .lb_skip@1
   JMP   .default@13
.lb_skip@1:
   LDY   #$01
   LDA   (__local_f15__pp16),Y
   STA   __local_f15__6
   INY
   LDA   (__local_f15__pp16),Y
   STA   __local_f15__5
   INY
   LDA   (__local_f15__pp16),Y
   STA   __local_f15__4
   INY
   LDA   (__local_f15__pp16),Y
   STA   __local_f15__3
   INY
   LDA   (__local_f15__pp16),Y
   STA   __local_f15__2
   INY
   LDA   (__local_f15__pp16),Y
   STA   __local_f15__1
   INY
   LDA   (__local_f15__pp16),Y
   STA   __local_f15__0
   LDY   #$00
   LDA   (__local_f15__pp16),Y
   ADC   #$5A
   STA   __local_f15__l17
   BCC   .carry_inc@0
   INC   __local_f15__6
   BNE   .carry_inc@0
   INC   __local_f15__5
   BNE   .carry_inc@0
   INC   __local_f15__4
   BNE   .carry_inc@0
   INC   __local_f15__3
   BNE   .carry_inc@0
   INC   __local_f15__2
   BNE   .carry_inc@0
   INC   __local_f15__1
   BNE   .carry_inc@0
   INC   __local_f15__0
.carry_inc@0:
   LDY   #$00
   STA   (__local_f15__pp16),Y
   LDA   __local_f15__6
   INY
   STA   (__local_f15__pp16),Y
   LDA   __local_f15__5
   INY
   STA   (__local_f15__pp16),Y
   LDA   __local_f15__4
   INY
   STA   (__local_f15__pp16),Y
   LDA   __local_f15__3
   INY
   STA   (__local_f15__pp16),Y
   LDA   __local_f15__2
   INY
   STA   (__local_f15__pp16),Y
   LDA   __local_f15__1
   INY
   STA   (__local_f15__pp16),Y
   LDA   __local_f15__0
   INY
   STA   (__local_f15__pp16),Y
   LDA   __local_f15__l17
   ORA   __local_f15__6
   ORA   __local_f15__5
   ORA   __local_f15__4
   ORA   __local_f15__3
   ORA   __local_f15__2
   ORA   __local_f15__1
   ORA   __local_f15__0
   BNE   .default@13
   LDA   gs7+9
   STA   __local_f15__2
   SEC
   SBC   #$01
   STA   gs7+9
   LDA   __local_f15__2
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_f15__1
   LDA   __local_f15__i20
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@23:
   BCC   .mul_skip@24
   CLC
   ADC   __local_f15__i20
.mul_skip@24:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@23
   STA   __local_f15__0
   LDA   __local_f15__2
   ORA   HARGS
   STA   @8.s19
   LDA   __local_f15__1
   ORA   __local_f15__0
   STA   @8.s19+1
   LDY   #$00
   LDA   (__local_f15__pp16),Y
   EOR   #$05
   STA   (__local_f15__pp16),Y
   AND   #$07
   CMP   #$03
   BEQ   .case@6
   CMP   #$06
   BNE   .switch@5_break
   LDA   z3
   STA   gs7+9
.case@6:
.switch@5_break:
   INC   __local_f15__i20
   JMP   .loop@4_start
.shift_loop@25:
   LDA   ga6+3
   ASL
   ROR   ga6+3
   DEX
   BNE   .shift_loop@25
.default@13:
.case@14:
.switch@2_break:
   LDA   g1
   BEQ   .cond_else@11
   LDA   g2
   BPL   .cond_end@12
   RTS
.cond_else@11:
.cond_end@12:
   RTS

main:
   SUBROUTINE

   LDA   gs7+1
   STA   HARGS
   LDA   gs7+2
   STA   HARGS+1
   LDA   gs7+3
   STA   HARGS+2
   LDA   gs7+4
   STA   HARGS+3
   LDA   gs7+5
   STA   HARGS+4
   LDA   gs7+6
   STA   HARGS+5
   LDA   gs7+7
   STA   HARGS+6
   LDA   gs7+8
   STA   HARGS+7
   LDA   #$05
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   STA   gs7+1
   LDA   HARGS+17
   STA   gs7+2
   LDA   HARGS+18
   STA   gs7+3
   LDA   HARGS+19
   STA   gs7+4
   LDA   HARGS+20
   STA   gs7+5
   LDA   HARGS+21
   STA   gs7+6
   LDA   HARGS+22
   STA   gs7+7
   LDA   HARGS+23
   STA   gs7+8
   LDA   #$00
   STA   __local_main__l22
   STA   __local_main__w24
.loop@15_continue:
   CLC
   ADC   #$01
   STA   __local_main__15
   LDA   __local_main__w24
   CMP   #$03
   BCC   .lb_skip@3
   JMP   .loop@15_break
.lb_skip@3:
   LDA   __local_main__15
   AND   #$03
   STA   __local_main__14
   LDX   __local_main__14
   LDA   ga6,X
   SEC
   SBC   #$03
   STA   __local_main__13
   STA   ga6,X
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_main__12
   STA   __local_main__11
   STA   __local_main__10
   STA   __local_main__9
   STA   __local_main__8
   STA   __local_main__7
   STA   __local_main__6
   LDA   #$00
   SEC
   SBC   __local_main__15
   STA   __local_main__5
   JSR   main@run3
   STA   __local_main__4
   JSR   main@run3
   STA   __local_main__3
   LDA   #$80
   SBC   #$00
   STA   __local_main__2
   JSR   main@run3
   STA   __local_main__1
   JSR   main@run3
   STA   __local_main__0
   JSR   main@run3
   STA   __local_main__w25
   JSR   main@run3
   STA   __local_main__w24
   LDA   __local_main__13
   SEC
   SBC   __local_main__5
   STA   __local_main__5
   LDA   __local_main__12
   SBC   __local_main__4
   STA   __local_main__4
   LDA   __local_main__11
   SBC   __local_main__3
   STA   __local_main__3
   LDA   __local_main__10
   SBC   __local_main__2
   STA   __local_main__2
   LDA   __local_main__9
   SBC   __local_main__1
   STA   __local_main__1
   LDA   __local_main__8
   SBC   __local_main__0
   STA   __local_main__0
   LDA   __local_main__7
   SBC   __local_main__w25
   STA   __local_main__w25
   LDA   __local_main__6
   SBC   __local_main__w24
   STA   __local_main__w24
   LDA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   ORA   __local_main__w25
   ORA   __local_main__w24
   BEQ   .loop@15_break
   JSR   f8
   CLC
   LDX   __local_main__14
   INC   ga6,X
   LDA   __local_main__15
   ADC   ga6,X
   STA   g1
   LDA   __local_main__15
   ORA   __local_main__15
   STA   z4
   LDA   #$00
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   __local_main__w25
.loop@16_start:
   LDA   __local_main__l22
   SEC
   SBC   #$05
   STA   __local_main__l22
   AND   #$07
   CMP   #$02
   INC   __local_main__w25
   LDA   __local_main__w25
   CMP   #$02
   BCC   .loop@16_start
   LDA   __local_main__15
   STA   __local_main__w24
   JMP   .loop@15_continue
.loop@15_break:
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

   LDA   #$00
   SBC   #$00
   RTS

acc:
   DS.B  2

ga6:
   DC.B  $00, $00, $01, $00

gs7:
   DS.B  1
   DC.L  $00010000
   DC.L  $00000000
   DC.B  $01

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   z4+2
   STA   z4+3
   STA   z5+2
   STA   @8.s19
   LDA   #$FF
   STA   g2
   STA   z3
   STA   z4
   LDA   #$7F
   STA   z4+1
   LDA   #$F7
   STA   z5
   LDA   #$B4
   STA   z5+1
   LDA   #$CE
   STA   z5+3
   LDA   #$80
   STA   @8.s19+1
   RTS
