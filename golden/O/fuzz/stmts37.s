SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$28
z6	EQU	$29
g2	EQU	$2A
g3	EQU	$2B
g1	EQU	$2C
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__0	EQU	$84
__local_main__i27	EQU	$85
__local_main__1	EQU	$86
__local_main__2	EQU	$87
__local_main__3	EQU	$88
__local_main__i28	EQU	$89
__local_main__l26	EQU	$8A
__local_f13__w16	EQU	$8B
__local_f18__l20	EQU	$8B
__local_f13__0	EQU	$8C
__local_f18__w24	EQU	$8C
__local_f13__1	EQU	$8D
__local_f18__p19	EQU	$8D
__local_f13__2	EQU	$8E
__local_f13__pp14	EQU	$8F
__local_f13__3	EQU	$90
__local_f13__l15	EQU	$91
__local_f9__0	EQU	$94
__local_f9__1	EQU	$95
__local_f9__2	EQU	$96
__local_f9__3	EQU	$97
__local_f9__4	EQU	$98
__local_f9__5	EQU	$99
__local_f9__6	EQU	$9A
__local_f9__7	EQU	$9B
__local_mix__0	EQU	$9E
__local_mix__1	EQU	$9F
__local_mix__2	EQU	$A0
__local_mix__3	EQU	$A1

; @zp-link-meta-begin
; def f13 params=__zpabi_f13__pp14_0,__zpabi_f13__pp14_1 locals=__local_f13__w16,__local_f13__0,__local_f13__1,__local_f13__2,__local_f13__pp14,__local_f13__3,__local_f13__l15,__local_f13__5,__local_f13__6 indirect=false in_cycle=false param_regs=A,X
; def f18 params=__zpabi_f18__p19 locals=__local_f18__l20,__local_f18__w24,__local_f18__p19 indirect=false in_cycle=false param_regs=A
; def f9 params=__zpabi_f9__pp10_0,__zpabi_f9__pp10_1 locals=__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__4,__local_f9__5,__local_f9__6,__local_f9__7,__local_f9__8,__local_f9__9 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__i27,__local_main__1,__local_main__2,__local_main__3,__local_main__i28,__local_main__l26 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f13 -> f9
; call f13 -> mix
; call f18 -> mix
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f13
; call main -> f18
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run7 section=.text local=true zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run6 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run3 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f13 section=.text local=false zero=false
; item f18 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item ga7 section=.bss local=false zero=true
; item gs8 section=.data local=false zero=false
; item @9.s23 section=.bss local=true zero=true
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$01,$00
; zp z5 size=1 at=auto def=true init=$FF
; zp z6 size=1 at=auto def=true init=$03
; zp g2 size=1 at=auto def=true init=$FF weight=26
; zp g3 size=1 at=auto def=true init=$00 weight=20
; zp g1 size=2 at=auto def=true init=$00,$00 weight=7
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

   LDA   g1+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run4
   LDA   g2
   JSR   fin0@run0
   LDA   g3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run7
   LDA   g3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run7:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   fin0@run5

fin0@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run4:
   SUBROUTINE

   JSR   fin0@run5
   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run6
   JMP   mix

fin0@run0@run6:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   fin0@run5

fin1:
   SUBROUTINE

   LDA   z5
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run7
   LDA   z5
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   z6
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin1@run3
   LDA   ga7
   JSR   fin0@run0
   LDA   ga7+1
   JSR   fin0@run0@run6
   JMP   mix

fin1@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga7+2
   JSR   fin0@run0
   LDA   ga7+3
   JSR   fin0@run0
   LDA   gs8+1
   STA   __zpabi_mix__v_0
   LDA   gs8+2
   STA   __zpabi_mix__v_1
   LDA   gs8+3
   STA   __zpabi_mix__v_2
   LDA   gs8+4
   JSR   fin1@run3
   LDA   gs8+5
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run7
   LDA   gs8+5
   STA   __zpabi_mix__v_0
   JMP   mix

f9:
   SUBROUTINE

   LDA   g3
   STA   __local_f9__1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f9__0
   LDA   z4
   STA   HARGS
   LDA   z4+1
   STA   HARGS+1
   LDA   #$5A
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   z4
   LDA   HARGS+5
   STA   z4+1
   LDA   __local_f9__1
   AND   z4
   STA   g1
   LDA   __local_f9__0
   AND   z4+1
   STA   g1+1
   LDA   gs8+1
   STA   __local_f9__3
   LDA   gs8+2
   STA   __local_f9__2
   LDA   gs8+3
   STA   __local_f9__1
   LDA   gs8+4
   STA   __local_f9__0
   LDA   __local_f9__3
   CLC
   ADC   #$01
   STA   gs8+1
   LDA   __local_f9__2
   ADC   #$00
   STA   gs8+2
   LDA   __local_f9__1
   ADC   #$00
   STA   gs8+3
   LDA   __local_f9__0
   ADC   #$00
   STA   gs8+4
   LDA   __local_f9__3
   AND   ga7
   STA   g2
   LDA   #$00
   BNE   .switch@0_break
   LDA   g1
   SEC
   SBC   #$01
   STA   g1
   BCS   .carry_inc@0
   DEC   g1+1
.carry_inc@0:
   LDA   gs8+5
   STA   __local_f9__0
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f9__1
   LDA   g1
   EOR   __local_f9__0
   STA   __local_f9__0
   LDA   g1+1
   EOR   __local_f9__1
   TAX
   LDA   __local_f9__0
   RTS
.switch@0_break:
   LDA   z4
   SEC
   SBC   #$01
   BCS   .carry_inc@2
   DEC   z4+1
.carry_inc@2:
   STA   z4
   STA   z6
   LDA   gs8+1
   STA   __local_f9__7
   LDA   gs8+2
   STA   __local_f9__6
   LDA   gs8+3
   STA   __local_f9__5
   LDA   gs8+4
   STA   __local_f9__4
   LDA   #$04
   ORA   g1
   STA   __local_f9__3
   LDA   g1+1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f9__1
   STA   __local_f9__0
   LDA   __local_f9__7
   ORA   __local_f9__3
   STA   __zpabi_mix__v_0
   LDA   __local_f9__6
   ORA   g1+1
   STA   __zpabi_mix__v_1
   LDA   __local_f9__5
   ORA   __local_f9__1
   STA   __zpabi_mix__v_2
   LDA   __local_f9__4
   ORA   __local_f9__0
   JSR   fin1@run3
   LDA   z6
   STA   __local_f9__1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f9__0
   SEC
   LDA   #$00
   SBC   __local_f9__1
   STA   __local_f9__1
   LDA   #$00
   SBC   __local_f9__0
   STA   __local_f9__0
   LDA   __local_f9__1
   EOR   #$FF
   STA   __local_f9__1
   LDA   __local_f9__0
   EOR   #$FF
   STA   __local_f9__0
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   JSR   fin0@run5
   LDA   __local_f9__1
   STA   __zpabi_mix__v_0
   LDA   __local_f9__0
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g1
   LDX   g1+1
   RTS

f13:
   SUBROUTINE

   STA   __local_f13__pp14
   STX   __local_f13__3
   LDA   #$00
   STA   __local_f13__l15
   STA   __local_f13__l15+1
   LDA   z6
   STA   __local_f13__w16
   LDX   #$06
.shift_loop@20:
   LDA   __local_f13__w16
   ASL
   ROR   __local_f13__w16
   DEX
   BNE   .shift_loop@20
   LDA   __local_f13__w16
   STA   z6
   STA   __local_f13__2
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_f13__1
   LDA   z5
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_f13__w16
   LDA   __local_f13__2
   SEC
   SBC   z5
   STA   __local_f13__0
   LDA   __local_f13__1
   SBC   __local_f13__w16
   ORA   __local_f13__0
   BEQ   .if_end@1
   LDA   #$00
   STA   __local_f13__w16
   JMP   .loop@2_continue
.f13@rot@0:
   LDX   #>__local_f13__l15
   LDA   #<__local_f13__l15
   JSR   f9
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin1@run3
   LDX   #>__local_f13__l15
   LDA   #<__local_f13__l15
   JSR   f9
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin1@run3
   LDA   __local_f13__1
   STA   __local_f13__w16
.loop@2_continue:
   CLC
   ADC   #$01
   STA   __local_f13__1
   LDA   __local_f13__w16
   CMP   #$04
   BCC   .f13@rot@0
.if_end@1:
   LDY   #$00
   LDA   (__local_f13__pp14),Y
   EOR   #$FF
   STA   (__local_f13__pp14),Y
   JSR   fin0@run0
   LDA   z5
   EOR   #$5A
   STA   z5
   STA   gs8+5
   STA   __local_f13__0
   LDA   ga7
   EOR   __local_f13__0
   AND   #$07
   BEQ   .case@4
   CMP   #$07
   BEQ   .case@6
.case@4:
.case@6:
   LDA   gs8+1
   LDX   gs8+2
   RTS

f18:
   SUBROUTINE

   STA   __local_f18__p19
   LDA   #$00
   STA   __local_f18__l20
   STA   __local_f18__w24
   LDA   @9.s23
   STA   HARGS
   LDA   @9.s23+1
   STA   HARGS+1
   LDA   #$FF
   STA   HARGS+2
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   AND   #$07
   BEQ   .switch@8_break
   CMP   #$01
   BEQ   .switch@8_break
   CMP   #$07
   BEQ   .switch@8_break
   LDA   #$00
   STA   __local_f18__w24
   JMP   .loop@13_continue
.f18@rot@1:
   INC   __local_f18__p19
   LDA   g2
   SEC
   SBC   #$5A
   STA   g2
   LDA   __local_f18__l20
   STA   __local_f18__w24
.loop@13_continue:
   CLC
   ADC   #$01
   STA   __local_f18__l20
   LDA   __local_f18__w24
   CMP   #$04
   BCC   .f18@rot@1
   LDA   gs8+5
   STA   __local_f18__l20
   LDX   #$04
.shift_loop@28:
   LDA   __local_f18__l20
   ASL
   ROR   __local_f18__l20
   DEX
   BNE   .shift_loop@28
   LDA   __local_f18__l20
   STA   gs8+5
   STA   __local_f18__w24
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   __local_f18__l20
   LDA   __local_f18__w24
   CLC
   ADC   #$FF
   STA   gs8+1
   LDA   __local_f18__l20
   ADC   #$FF
   STA   gs8+2
   BMI   .sx_neg@32
   LDA   #$00
   BEQ   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   STA   gs8+3
   STA   gs8+4
.switch@8_break:
   LDA   g1+1
   BMI   .sx_neg@34
   LDA   #$00
   BEQ   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   JSR   fin0@run4
   LDA   #$01
   STA   gs8+1
   LDA   #$00
   STA   gs8+2
   LDY   #$00
   STY   gs8+3
   STA   gs8+4
   LDA   g3
   BMI   .sx_neg@36
   TYA
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   STA   __local_f18__l20
   LDA   g3
   CLC
   ADC   #$FF
   STA   __local_f18__w24
   LDA   __local_f18__l20
   ADC   #$FF
   STA   __local_f18__l20
   LDA   __local_f18__w24
   SEC
   SBC   #$85
   LDA   __local_f18__l20
   SBC   #$B0
   TYA
   ROL
   STA   __local_f18__l20
   TYA
   BPL   .sx_done@39
   LDA   #$FF
.sx_done@39:
   JSR   fin0@run5
   LDA   __local_f18__l20
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   @9.s23
   STA   __local_f18__l20
   LDA   @9.s23+1
   STA   __local_f18__w24
   LDX   #$05
.shift_loop@40:
   LDA   __local_f18__w24
   ASL
   ROR   __local_f18__w24
   ROR   __local_f18__l20
   DEX
   BNE   .shift_loop@40
   LDA   __local_f18__l20
   STA   @9.s23
   STA   gs8+5
   LDA   __local_f18__w24
   STA   @9.s23+1
   LDA   #$FF
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_2
   JSR   fin1@run3
   LDA   __local_f18__p19
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __local_main__l26
   LDA   #$00
   STA   __local_main__i27
.main@rot@2:
   LDA   z5
   AND   __local_main__i27
   JSR   f18
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   INC   __local_main__i27
   LDA   __local_main__i27
   CMP   #$04
   BCC   .main@rot@2
   LDA   ga7+1
   LSR
   STA   g3
   LDA   #$02
   STA   __local_main__i28
.loop@15_start:
   LDA   __local_main__i28
   BNE   .lb_skip@1
   JMP   .loop@15_break
.lb_skip@1:
   LDA   gs8+5
   STA   __local_main__i27
   BMI   .sx_neg@42
   LDA   #$00
   BEQ   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   STA   __local_main__0
   LDA   __local_main__i27
   AND   z4
   STA   gs8+1
   LDA   __local_main__0
   AND   z4+1
   STA   gs8+2
   LDA   #$00
   STA   gs8+3
   STA   gs8+4
   LDA   g3
   AND   #$07
   CMP   #$01
   BEQ   .case@17
   CMP   #$02
   BNE   .lb_skip@0
   JMP   .switch@16_break
.lb_skip@0:
   CMP   #$03
   BEQ   .case@19
   CMP   #$05
   BEQ   .case@20
   BNE   .default@21
.case@17:
   LDA   gs8+1
   STA   __local_main__3
   LDA   gs8+2
   STA   __local_main__2
   LDA   gs8+3
   STA   __local_main__1
   STA   __local_main__i27
   LDA   ga7+1
   CLC
   ADC   #$81
   STA   ga7+1
   CLC
   ADC   __local_main__3
   STA   __zpabi_mix__v_0
   LDA   __local_main__2
   ADC   #$00
   STA   __zpabi_mix__v_1
   LDA   __local_main__1
   ADC   #$00
   STA   __zpabi_mix__v_2
   LDA   __local_main__i27
   ADC   #$00
   JSR   fin1@run3
   JMP   .switch@16_break
.case@19:
   JSR   main@run2
   LDA   #$03
   STA   HARGS+4
   JSR   asl32
   JSR   main@run1
   STA   gs8+5
   LDX   #>__local_main__l26
   LDA   #<__local_main__l26
   JSR   f13
   JMP   .switch@16_break
.case@20:
.default@21:
   LDA   z6
   AND   #$07
   CMP   #$01
   BEQ   .case@23
   CMP   #$03
   BEQ   .case@24
   CMP   #$04
   BEQ   .case@25
   CMP   #$06
   BEQ   .case@26
   BNE   .switch@22_break
.case@23:
   JSR   main@run2
   LDA   #$05
   STA   HARGS+4
   JSR   lsr32
   JSR   main@run1
   STA   g2
   JMP   .switch@22_break
.case@24:
   LDA   z6
   CMP   #$80
   ROR
   CMP   #$80
   ROR
   STA   z6
.case@25:
.case@26:
.switch@22_break:
.switch@16_break:
   DEC   __local_main__i28
   JMP   .loop@15_start
.loop@15_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run2:
   SUBROUTINE

   LDA   gs8+1
   STA   HARGS
   LDA   gs8+2
   STA   HARGS+1
   LDA   gs8+3
   STA   HARGS+2
   STA   HARGS+3
   RTS

main@run1:
   SUBROUTINE

   LDA   HARGS+6
   STA   gs8+2
   LDA   HARGS+7
   STA   gs8+3
   LDA   HARGS+8
   STA   gs8+4
   LDA   HARGS+5
   STA   gs8+1
   RTS

acc:
   DS.B  2

ga7:
   DS.B  4

gs8:
   DS.B  1
   DC.L  $00000080
   DC.B  $7F

@9.s23:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g1+1
   STA   g3
   STA   z4+1
   LDA   #$FF
   STA   g2
   STA   z5
   LDA   #$01
   STA   z4
   LDA   #$03
   STA   z6
   RTS
