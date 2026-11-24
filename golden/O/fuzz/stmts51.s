SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$28
z5	EQU	$29
g1	EQU	$2A
__zpabi_f14__p16_0	EQU	$80
__zpabi_f23__p25	EQU	$80
__zpabi_f14__p16_1	EQU	$81
__zpabi_f23__p26	EQU	$81
__zpabi_f14__p17_0	EQU	$82
__zpabi_f23__pp27_0	EQU	$82
__zpabi_f14__p17_1	EQU	$83
__zpabi_f23__pp27_1	EQU	$83
__zpabi_f14__pp18_0	EQU	$84
__local_f23__1	EQU	$85
__zpabi_f14__pp18_1	EQU	$85
__zpabi_mix__v_0	EQU	$86
__zpabi_mix__v_1	EQU	$87
__zpabi_mix__v_2	EQU	$88
__zpabi_mix__v_3	EQU	$89
__local_main__0	EQU	$8A
__local_main__1	EQU	$8B
__local_main__2	EQU	$8C
__local_main__3	EQU	$8D
__local_main__l31	EQU	$8E
__local_main__l32	EQU	$92
__local_f14__l21	EQU	$94
__local_f14__0	EQU	$95
__local_f14__1	EQU	$96
__local_f10__0	EQU	$97
__local_f10__1	EQU	$98
__local_f10__2	EQU	$99
__local_f10__3	EQU	$9A
__local_f10__4	EQU	$9B
__local_mix__0	EQU	$9C
__local_mix__1	EQU	$9D
__local_mix__2	EQU	$9E
__local_mix__3	EQU	$9F

; @zp-link-meta-begin
; def f10 params= locals=__local_f10__0,__local_f10__1,__local_f10__2,__local_f10__3,__local_f10__4 indirect=false in_cycle=false
; def f14 params=__zpabi_f14__p15,__zpabi_f14__p16_0,__zpabi_f14__p16_1,__zpabi_f14__p17_0,__zpabi_f14__p17_1,__zpabi_f14__pp18_0,__zpabi_f14__pp18_1 locals=__local_f14__l21,__local_f14__0,__local_f14__1 indirect=false in_cycle=false param_regs=A,,,,,,
; def f23 params=__zpabi_f23__p24_0,__zpabi_f23__p24_1,__zpabi_f23__p25,__zpabi_f23__p26,__zpabi_f23__pp27_0,__zpabi_f23__pp27_1 locals=__local_f23__0,__local_f23__1 indirect=false in_cycle=false param_regs=A,X,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l31,__local_main__5,__local_main__6,__local_main__7,__local_main__l32,__local_main__9 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call f14 -> f10
; call f14 -> mix
; call f23 -> f10
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f10
; call main -> f14
; call main -> f23
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run4 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@tail0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f10@run1 section=.text local=true zero=false
; item f14 section=.text local=false zero=false
; item f14@run5 section=.text local=true zero=false
; item f14@run2 section=.text local=true zero=false
; item f23 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item gp9 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$03,$00
; zp z4 size=1 at=auto def=true init=$20
; zp z5 size=1 at=auto def=true init=$01
; zp g1 size=2 at=auto def=true init=$00,$00 weight=8
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

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run3
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   JSR   fin0@run0
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run4
   JMP   mix

fin0@run0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   z5
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z5
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   JSR   fin0@run0
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   LDA   ga6+3
   JSR   fin0@run0
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   JMP   fin1@tail0

fin1@tail0:
   SUBROUTINE

   JSR   fin0@run0@run4
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+6
   STA   __zpabi_mix__v_0
   LDA   ga6+7
   JSR   fin0@run0
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   JSR   fin0@run0
   LDA   gs7+3
   STA   __zpabi_mix__v_0
   LDA   gs7+4
   JMP   fin1@tail0

f10:
   SUBROUTINE

   LDA   z3
   AND   #$07
   CMP   #$01
   BEQ   .case@1
   CMP   #$04
   BNE   .lb_skip@1
   JMP   .switch@0_break
.lb_skip@1:
   CMP   #$05
   BEQ   .if_else@2
   CMP   #$07
   BNE   .lb_skip@0
   JMP   .case@8
.lb_skip@0:
   JMP   .switch@0_break
.case@1:
   LDA   gs7+3
   STA   __local_f10__0
   LDA   gs7+4
   LSR
   ROR   __local_f10__0
   LSR
   ROR   __local_f10__0
   LSR
   ROR   __local_f10__0
   STA   gs7+4
   LDA   __local_f10__0
   STA   gs7+3
   STA   z5
   JMP   .switch@0_break
.if_else@2:
   LDA   #$FF
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   LDA   #$7F
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   z3
   STA   g1
   LDA   z3+1
   STA   g1+1
   LDX   #$06
.shift_loop@4:
   LSR   g1+1
   ROR   g1
   DEX
   BNE   .shift_loop@4
   JSR   f10@run1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f10__4
   INY
   LDA   (DPTR),Y
   STA   __local_f10__3
   INY
   LDA   (DPTR),Y
   STA   __local_f10__2
   INY
   LDA   (DPTR),Y
   STA   __local_f10__1
   LDA   __local_f10__4
   SEC
   SBC   #$01
   STA   __local_f10__0
   LDA   __local_f10__3
   SBC   #$00
   STA   __local_f10__3
   LDA   __local_f10__2
   SBC   #$00
   STA   __local_f10__2
   BCS   .carry_inc@0
   DEC   __local_f10__1
.carry_inc@0:
   LDA   __local_f10__0
   LDY   #$00
   STA   (DPTR),Y
   JSR   f10@run1
   LDA   __local_f10__3
   LDY   #$01
   STA   (DPTR),Y
   JSR   f10@run1
   LDA   __local_f10__2
   LDY   #$02
   STA   (DPTR),Y
   JSR   f10@run1
   LDA   __local_f10__1
   LDY   #$03
   STA   (DPTR),Y
   LDA   __local_f10__4
   AND   #$5A
   AND   #$07
   CMP   #$05
   BEQ   .vthread@0
   CMP   #$06
   BEQ   .case@6
   BNE   .default@7
.vthread@0:
   LDA   gs7+1
   AND   #$03
   LDY   #$00
   STY   gs7+2
   STA   gs7+1
   AND   z5
   STA   HARGS+8
   TYA
   STA   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   RTS
.case@6:
.default@7:
   DEC   z4
   LDA   gs7+1
   ORA   #$5A
   STA   gs7+1
.case@8:
.switch@0_break:
   LDA   #$FF
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f10@run1:
   SUBROUTINE

   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   RTS

f14:
   SUBROUTINE

   LDA   #$7F
   STA   __local_f14__l21
   LDA   #$02
   STA   __local_f14__l21
   LDA   gs7+3
   AND   #$01
   STA   gs7+3
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   gs7+4
   JSR   f14@run5
   JSR   f10
   LDA   HARGS+8
   STA   __local_f14__1
   LDX   HARGS+9
   LDA   z3
   SEC
   SBC   #$05
   STA   z3
   BCS   .carry_inc@1
   DEC   z3+1
.carry_inc@1:
   STA   g1
   LDA   z3+1
   STA   g1+1
   JSR   f14@run2
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f14__l21
   INY
   LDA   (DPTR),Y
   LSR
   STA   __local_f14__0
   ROR   __local_f14__l21
   LDA   __local_f14__l21
   DEY
   STA   (DPTR),Y
   JSR   f14@run2
   LDA   __local_f14__0
   LDY   #$01
   STA   (DPTR),Y
   LDA   __local_f14__1
   RTS

f14@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@run3

f14@run2:
   SUBROUTINE

   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   RTS

f23:
   SUBROUTINE

   LDA   ga6+4
   AND   #$81
   LDY   #$00
   STY   ga6+5
   STA   ga6+4
   CLC
   ADC   g1
   STA   __local_f23__1
   LDA   g1+1
   ADC   #$00
   INY
   STA   (__zpabi_f23__pp27_0),Y
   LDA   __local_f23__1
   DEY
   STA   (__zpabi_f23__pp27_0),Y
   LDA   #$00
   LDY   #$02
   STA   (__zpabi_f23__pp27_0),Y
   INY
   STA   (__zpabi_f23__pp27_0),Y
   LDX   #$06
.shift_loop@8:
   ASL   z3
   ROL   z3+1
   DEX
   BNE   .shift_loop@8
   LDA   z3
   ORA   z3+1
   BEQ   .if_else@9
   LDA   ga6+2
   SEC
   SBC   #$05
   STA   ga6+2
   BCS   .if_end@8
   DEC   ga6+3
   JMP   .if_end@8
.if_else@9:
   JSR   f10
.if_end@8:
   LDA   ga6+4
   LDX   ga6+5
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l31
   STA   __local_main__l31+2
   STA   __local_main__l31+3
   LDA   #$01
   STA   __local_main__l31+1
   LDA   #$FF
   STA   __local_main__l32
   STA   __local_main__l32+1
   STA   z4
   INC   gs7+3
   BNE   .inc_done@0
   INC   gs7+4
.inc_done@0:
   LDA   gs7+3
   STA   __local_main__2
   LDA   gs7+4
   ASL   __local_main__2
   ROL
   ASL   __local_main__2
   ROL
   ASL   __local_main__2
   ROL
   ASL   __local_main__2
   ROL
   STA   __local_main__3
   STA   gs7+4
   LDA   __local_main__2
   STA   gs7+3
   LDA   z5
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_main__0
   LDA   __local_main__2
   SEC
   SBC   z5
   STA   g1
   LDA   __local_main__3
   SBC   __local_main__0
   STA   g1+1
   LDX   #$05
.shift_loop@12:
   ASL   __local_main__l32
   ROL   __local_main__l32+1
   DEX
   BNE   .shift_loop@12
   LDA   __local_main__l32
   ORA   __local_main__l32+1
   BNE   .lb_skip@4
   JMP   .if_end@10
.lb_skip@4:
   LDA   gs7+1
   LDX   gs7+2
   STX   __local_main__1
   SEC
   SBC   #$03
   BCS   .carry_inc@3
   DEC   __local_main__1
.carry_inc@3:
   STA   gs7+1
   STA   __local_main__l31
   LDA   __local_main__1
   STA   gs7+2
   LDA   #$01
   ORA   __local_main__1
   STA   __local_main__l31+1
   LDA   #$00
   STA   __local_main__l31+2
   STA   __local_main__l31+3
   LDA   gs7+3
   AND   #$01
   LDY   #$00
   STY   gs7+4
   ORA   #$00
   STA   gs7+3
   BNE   .lb_skip@3
   JMP   .if_end@11
.lb_skip@3:
   LDA   gs7+1
   CLC
   ADC   g1
   AND   #$07
   CMP   #$07
   BNE   .switch@9_break
   TYA
   STA   __zpabi_mix__v_0
   JSR   f14@run5
.switch@9_break:
   JSR   f10@run1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_main__1
   INY
   LDA   (DPTR),Y
   STA   __local_main__0
   INY
   LDA   (DPTR),Y
   INY
   LDA   (DPTR),Y
   LDA   __local_main__1
   ORA   __local_main__0
   BNE   .lb_skip@2
   JMP   .if_end@12
.lb_skip@2:
   JSR   f10
   LDA   HARGS+8
   STA   __zpabi_mix__v_0
   LDA   HARGS+9
   STA   __zpabi_mix__v_1
   LDA   HARGS+10
   STA   __zpabi_mix__v_2
   LDA   HARGS+11
   JSR   fin0@run3
   LDA   __local_main__l31
   LDX   __local_main__l31+1
   STX   __local_main__0
   SEC
   SBC   g1
   STA   __local_main__2
   LDA   __local_main__0
   SBC   g1+1
   TAX
   LDA   __local_main__l32
   SEC
   SBC   ga6+6
   STA   __zpabi_f23__p25
   JSR   f14@run2
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_main__1
   INY
   LDA   (DPTR),Y
   LDA   __local_main__1
   EOR   #$FF
   STA   __zpabi_f23__p26
   LDA   #<__local_main__l31
   STA   __zpabi_f23__pp27_0
   LDA   #>__local_main__l31
   STA   __zpabi_f23__pp27_1
   LDA   __local_main__2
   JSR   f23
   LDA   gs7+3
   STA   __local_main__1
   LDA   gs7+4
   STA   __local_main__0
   LDA   #$00
   CLC
   SBC   __local_main__1
   LDA   #$00
   SBC   __local_main__0
   LDA   #$00
   ROL
   STA   __local_main__2
   LDA   ga6+2
   SEC
   SBC   #$03
   STA   __zpabi_f14__p17_0
   LDA   ga6+3
   SBC   #$00
   STA   __zpabi_f14__p17_1
   LDA   #$02
   STA   __zpabi_f14__p16_0
   LDA   #$00
   STA   __zpabi_f14__p16_1
   LDA   #<__local_main__l32
   STA   __zpabi_f14__pp18_0
   LDA   #>__local_main__l32
   STA   __zpabi_f14__pp18_1
   LDA   __local_main__2
   JSR   f14
.if_end@12:
.if_end@11:
.if_end@10:
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

g2:
   DC.L  $00007FFF

ga6:
   DC.W  $0001
   DC.W  $4E2F
   DC.W  $0002
   DC.W  $7FFF

gs7:
   DS.B  1
   DC.W  $0080
   DC.W  $DA49

gp8:
   DC.W  g2

gp9:
   DC.W  g1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g1+1
   STA   z3+1
   LDA   #$03
   STA   z3
   LDA   #$20
   STA   z4
   LDA   #$01
   STA   z5
   RTS
